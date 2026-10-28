"""Reader and analysis of traces.jsonl, the measurement traces the SpnQA.* tests write next to records.jsonl (SpnQa::Trace in
Spn/Tests/SpnQaRecorder.h). A trace is NOT a check: it has no status; it is evidence for diagnosing a failed or suspicious check.

Line format: {"suite","id","epoch","columns":[...],"rows":[[...],...]}. Turns traces (id "<case>#<trial>") have the columns
t, speed, gait (Core Gait index: 0 walk .. 3 sprint), local_angle (acceleration direction in the actor frame, deg), actor_yaw, vel_heading
(deg), accel_x, accel_y, one row per frame for 0.5 s after the forward key was released and the side key pressed.

Usage:  python3 Tools/spn_trace_tools.py <run_dir or traces.jsonl>     -> JSON report, exit 0 symmetric / no turn traces, 1 asymmetric
Pure standard library; tested in test_spn_trace_tools.py on synthetic traces (no real Unreal trace has been seen yet).
"""

import json
import math
import os
import sys

SPRINT = 3
START_YAW = 90.0   # SpnQA.Turns starts every trial facing +Y (yaw 90)


def read_traces(path):
    """(traces, problems). Each trace: {"suite","id","rows": [dict by column]}. Bad lines are reported, never silently dropped."""
    traces, problems = [], []
    if os.path.isdir(path):
        path = os.path.join(path, "traces.jsonl")
    if not os.path.isfile(path):
        return traces, problems
    with open(path, encoding="utf-8") as handle:
        for number, line in enumerate(handle, 1):
            line = line.strip()
            if not line:
                continue
            try:
                doc = json.loads(line)
                columns = doc["columns"]
                rows = [dict(zip(columns, row)) for row in doc["rows"]]
                if any(len(row) != len(columns) for row in doc["rows"]):
                    raise ValueError("row length differs from the column count")
                if not all(isinstance(v, (int, float)) and math.isfinite(v) for row in doc["rows"] for v in row):
                    raise ValueError("non-finite or non-numeric value")
                traces.append({"suite": doc["suite"], "id": doc["id"], "rows": rows})
            except (ValueError, KeyError, TypeError) as error:
                problems.append("traces.jsonl line %d: %s" % (number, error))
    return traces, problems


def turn_metrics(rows):
    """Facts about one turn trace. sprint_lost_s integrates the frame times where the effective gait was not sprint."""
    if not rows:
        return None
    lost, first_loss, previous_t = 0.0, None, 0.0
    for row in rows:
        dt = max(row["t"] - previous_t, 0.0)
        previous_t = row["t"]
        if int(round(row["gait"])) != SPRINT:
            lost += dt
            if first_loss is None:
                first_loss = row["t"]
    return {"frames": len(rows), "min_speed": min(r["speed"] for r in rows), "sprint_lost_s": round(lost, 4), "first_sprint_loss_t": first_loss,
            "yaw_change": rows[-1]["actor_yaw"] - START_YAW, "first_local_angle": rows[0]["local_angle"]}


def _wrap(angle):
    return (angle + 180.0) % 360.0 - 180.0


def mirrored_turns(traces, left="sprint_left_90", right="sprint_right_90", speed_tol=0.05, lost_tol_s=0.03, angle_tol_deg=10.0):
    """Pairs trial n of the left and the right case and compares them. A mirror-symmetric character gives (nearly) equal speed and sprint
    loss, a negated local angle and a negated yaw change. Returns {"pairs": [...], "verdict": "symmetric" | "asymmetric" | "no_data", "hints": [...]}."""
    def by_trial(case):
        out = {}
        for trace in traces:
            if trace["suite"] == "Turns" and trace["id"].split("#")[0] == case:
                out[int(trace["id"].split("#")[1]) if "#" in trace["id"] else 0] = trace["rows"]
        return out
    lefts, rights = by_trial(left), by_trial(right)
    pairs, hints = [], set()
    for trial in sorted(set(lefts) & set(rights)):
        ml, mr = turn_metrics(lefts[trial]), turn_metrics(rights[trial])
        if ml is None or mr is None:
            continue
        frames = min(len(lefts[trial]), len(rights[trial]))
        angle_err = max((abs(_wrap(lefts[trial][i]["local_angle"] + rights[trial][i]["local_angle"])) for i in range(frames)), default=0.0)
        speed_diff = abs(ml["min_speed"] - mr["min_speed"]) / max(ml["min_speed"], mr["min_speed"], 1e-9)
        lost_diff = abs(ml["sprint_lost_s"] - mr["sprint_lost_s"])
        yaw_err = abs(_wrap(ml["yaw_change"] + mr["yaw_change"]))
        flags = []
        if speed_diff > speed_tol:
            flags.append("min speed differs by %.0f%%" % (speed_diff * 100))
        if lost_diff > lost_tol_s:
            flags.append("sprint lost %.3f s (left) vs %.3f s (right)" % (ml["sprint_lost_s"], mr["sprint_lost_s"]))
        if angle_err > angle_tol_deg:
            flags.append("local input angle is not mirrored (max error %.1f deg)" % angle_err)
        if yaw_err > angle_tol_deg:
            flags.append("actor yaw change is not mirrored (error %.1f deg)" % yaw_err)
        pairs.append({"trial": trial, "left": ml, "right": mr, "speed_diff": round(speed_diff, 4), "sprint_lost_diff_s": round(lost_diff, 4),
                      "local_angle_mirror_error": round(angle_err, 2), "yaw_mirror_error": round(yaw_err, 2), "flags": flags})
        if angle_err > angle_tol_deg or yaw_err > angle_tol_deg:
            hints.add("rotation or input direction is asymmetric: look at the input mapping (A/D), control vs actor yaw, orient-to-movement")
        elif lost_diff > lost_tol_s or speed_diff > speed_tol:
            hints.add("rotation mirrors but the gait/speed does not: look at the sprint-sustain threshold and frame timing (sampling)")
    if not pairs:
        return {"pairs": [], "verdict": "no_data", "hints": []}
    asymmetric = any(p["flags"] for p in pairs)
    if asymmetric and len({bool(p["flags"]) for p in pairs}) > 1:
        hints.add("only some trials are asymmetric: timing/noise, repeat with more trials before changing code")
    return {"pairs": pairs, "verdict": "asymmetric" if asymmetric else "symmetric", "hints": sorted(hints)}


MODE_WALKING, MODE_FALLING = 1, 3   # EMovementMode


def jump_facts(rows):
    """Facts about one SpnQA.Jump trace (columns t, y_to_face, z, vy, vz, mode, floor_dist, walkable, x_drift; y_to_face = distance of the
    capsule front to the obstacle face, <= 0 once the capsule touches/passes it). The classification answers the open question of the
    M2A2 report: does the capsule get over a ledge in the AIR (speed taken by the face, vertical climb) or by WALKING/perching on its edge
    after the apex (walking mode with a rising z and a walkable floor)."""
    if len(rows) < 3:
        return None
    z0 = rows[0]["z"]
    apex = max(r["z"] for r in rows) - z0
    speed0 = abs(rows[0]["vy"])
    contact = next((i for i, r in enumerate(rows) if r["y_to_face"] <= 0.0), None)
    facts = {"frames": len(rows), "apex_cm": round(apex, 2), "takeoff_vy": round(rows[0]["vy"], 1), "contact_index": contact}
    if contact is None or abs(rows[0]["y_to_face"]) < 1e-9 and all(r["y_to_face"] == 0.0 for r in rows):
        facts["classification"] = "no_ledge"
        return facts
    after = rows[contact:]
    facts.update(contact_t=round(after[0]["t"], 3), z_at_contact=round(after[0]["z"] - z0, 2), vy_at_contact=round(after[0]["vy"], 1))
    stalled = [r for r in after if abs(r["vy"]) < 0.2 * speed0 and r["vz"] > 0 and r["mode"] == MODE_FALLING]
    facts["air_climb_frames"] = len(stalled)
    rising_walk = [r for i, r in enumerate(after[1:], 1) if r["mode"] == MODE_WALKING and r["z"] - after[i - 1]["z"] > 0.5]
    facts["walking_rise_frames"] = len(rising_walk)
    facts["walking_rise_cm"] = round(sum(r["z"] - after[i - 1]["z"] for i, r in enumerate(after[1:], 1) if r["mode"] == MODE_WALKING and r["z"] - after[i - 1]["z"] > 0.5), 2)
    facts["min_vy_ratio_after_contact"] = round(min(r["vy"] for r in after) / rows[0]["vy"], 3) if rows[0]["vy"] > 1e-6 else None
    facts["crossed"] = after[-1]["y_to_face"] < -100.0
    if facts["walking_rise_cm"] >= 3.0:
        facts["classification"] = "climbed_by_walking_after_contact"       # step-up / perch: not the jump itself
    elif stalled:
        facts["classification"] = "vertical_climb_in_air"                   # the face took the horizontal speed while still rising
    elif facts["crossed"]:
        facts["classification"] = "cleared_in_air"
    else:
        facts["classification"] = "blocked"
    return facts


def main(argv):
    if len(argv) != 2:
        print(__doc__)
        return 2
    traces, problems = read_traces(argv[1])
    report = {"traces": len(traces), "problems": problems, "turns": mirrored_turns(traces),
              "jumps": {t["id"]: jump_facts(t["rows"]) for t in traces if t["suite"] == "Jump"}}
    print(json.dumps(report, indent=1))
    return 1 if report["turns"]["verdict"] == "asymmetric" or problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
