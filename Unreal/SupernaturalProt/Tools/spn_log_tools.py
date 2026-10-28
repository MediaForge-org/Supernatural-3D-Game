"""Readers for the two things the Windows editor writes to its log: the map generator's "SpnMovementTestMap:" lines and the movement
recorder's "SpnMoveDiag," CSV blocks (Spn/SpnMovementDiagnostics). Pure Python, tested on synthetic logs on Fedora; the real log
format is as written by the C++/Python sources in this repository, but no real Unreal log has been seen yet.

Usage:
    python3 Tools/spn_log_tools.py generator <editor.log>      -> prints a JSON verdict, exit code 0 = generator finished and saved
    python3 Tools/spn_log_tools.py diag <editor.log> [--ledge-cm 30 --distance-cm 20]   -> JSON summary per recorded jump
"""

import json
import math
import re
import sys

GEN_PREFIX = "SpnMovementTestMap:"
FAIL_MARKERS = ("generation failed", "NOT saved", "could not", "not found", "layout data invalid", "does not match the layout", "Nothing was deleted")
CREATED = re.compile(r"created (\S+) with (\d+) shape actors for (\d+) primitives \((\d+) instance groups\), (\d+) doors, (\d+) reset points")
ACTION = re.compile(r"SpnMovementTestMap: (create|keep|abort|rebuild): (.*)")


def generator_verdict(text):
    """{"status": "saved" | "kept" | "failed" | "unknown", ...} from the lines that start with the generator prefix."""
    lines = [l for l in text.splitlines() if GEN_PREFIX in l]
    verdict = {"status": "unknown", "lines": len(lines), "action": None, "errors": [], "warnings": []}
    for line in lines:
        m = ACTION.search(line)
        if m:
            verdict["action"] = m.group(1)
            verdict["reason"] = m.group(2).strip()
        if "Error" in line.split(GEN_PREFIX)[0] or any(f in line for f in FAIL_MARKERS):
            verdict["errors"].append(line.split(GEN_PREFIX, 1)[1].strip())
        elif "Warning" in line.split(GEN_PREFIX)[0]:
            verdict["warnings"].append(line.split(GEN_PREFIX, 1)[1].strip())
        c = CREATED.search(line)
        if c:
            verdict.update(map=c.group(1), shape_actors=int(c.group(2)), primitives=int(c.group(3)), instance_groups=int(c.group(4)),
                           doors=int(c.group(5)), reset_points=int(c.group(6)))
    if verdict["errors"] and "map" not in verdict:
        verdict["status"] = "failed"
    elif verdict["errors"]:
        verdict["status"] = "failed"          # a created-line plus an error: do not trust the save
    elif "map" in verdict:
        verdict["status"] = "saved"
    elif verdict["action"] == "keep":
        verdict["status"] = "kept"
    elif verdict["action"] == "abort":
        verdict["status"] = "failed"
    return verdict


DIAG = re.compile(r"SpnMoveDiag,(.*)$")
COLUMNS = ("time", "x", "y", "z", "vx", "vy", "vz", "mode", "floor_dist", "walkable", "events")
EVENT_JUMP, EVENT_LAND, EVENT_STEP, EVENT_COLLAPSE = 1, 2, 4, 8


def parse_diag(text):
    """List of dumps, each a list of frame dicts (header lines separate the dumps)."""
    dumps, current = [], None
    for line in text.splitlines():
        m = DIAG.search(line)
        if not m:
            continue
        body = m.group(1).strip()
        if body.startswith("time"):
            current = []
            dumps.append(current)
            continue
        parts = body.split(",")
        if current is None or len(parts) != len(COLUMNS):
            continue
        try:
            values = [float(p) for p in parts]
        except ValueError:
            continue
        if not all(math.isfinite(v) for v in values):
            continue
        current.append(dict(zip(COLUMNS, values)))
    return dumps


CFG = re.compile(r"SpnMoveDiagCfg,(.*)$")
HIT = re.compile(r"SpnMoveDiagHit,(.*)$")
HIT_COLUMNS = ("time", "nx", "ny", "nz", "ix", "iy", "iz", "walkable")


def parse_diag_meta(text):
    """Per dump (same order as parse_diag): {"config": {name: float} | None, "hits": [dict]}. The C++ writes the config and hit lines BEFORE the
    frame header of their dump, so they belong to the next header."""
    metas, pending = [], {"config": None, "hits": []}
    for line in text.splitlines():
        m = CFG.search(line)
        if m:
            config = {}
            for item in m.group(1).strip().split(","):
                key, _, value = item.partition("=")
                try:
                    config[key.strip()] = float(value)
                except ValueError:
                    pass
            pending["config"] = config or None
            continue
        m = HIT.search(line)
        if m:
            parts = m.group(1).strip().split(",", len(HIT_COLUMNS))
            if len(parts) == len(HIT_COLUMNS) + 1:
                try:
                    hit = dict(zip(HIT_COLUMNS, (float(p) for p in parts[:len(HIT_COLUMNS)])))
                except ValueError:
                    continue
                hit["component"] = parts[-1].strip()
                pending["hits"].append(hit)
            continue
        m = DIAG.search(line)
        if m and m.group(1).strip().startswith("time"):
            metas.append(pending)
            pending = {"config": None, "hits": []}
    return metas


def classify_hit(hit, walkable_z=0.71):
    """Face of the obstacle (wall, normal.z ~ 0), a rounded contact (capsule bottom on a corner: 0 < nz < walkable), a walkable contact
    (nz >= walkable_z: the engine treats it as floor and can walk or perch on it) or a top face."""
    nz = hit["nz"]
    if nz >= 0.99:
        return "top_face"
    if nz >= walkable_z:
        return "walkable_corner_contact"
    if nz > 0.05:
        return "steep_corner_contact"
    return "wall_face"


def summarize_hits(frames, hits, walkable_z=0.71):
    """Hits during a jump/step window summarised for the Windows analysis: count per class and the first walkable corner contact."""
    classes = {}
    first_walkable = None
    for hit in hits:
        name = classify_hit(hit, walkable_z)
        classes[name] = classes.get(name, 0) + 1
        if name == "walkable_corner_contact" and first_walkable is None:
            first_walkable = hit
    out = {"hits": len(hits), "classes": classes, "components": sorted({h["component"] for h in hits})}
    if first_walkable is not None:
        out["first_walkable_corner_contact"] = {"time": first_walkable["time"], "nz": first_walkable["nz"], "impact_z": first_walkable["iz"]}
    return out


def summarize_jump(frames, ledge_cm=None, distance_cm=None, gravity_cm=980.0):
    """Facts about the first jump in a dump. With ledge/distance given, also the kinematic prediction (see Core JumpModel)."""
    jump = next((i for i, f in enumerate(frames) if int(f["events"]) & EVENT_JUMP), None)
    if jump is None:
        return {"jump": False, "step_ups": sum(1 for f in frames if int(f["events"]) & EVENT_STEP), "frames": len(frames)}
    f0 = frames[jump]
    speed0 = math.hypot(f0["vx"], f0["vy"])
    land = next((i for i in range(jump, len(frames)) if int(frames[i]["events"]) & EVENT_LAND), None)
    end = land if land is not None else len(frames) - 1
    air = frames[jump:end + 1]
    z0 = frames[jump - 1]["z"] if jump > 0 else f0["z"]
    apex = max(f["z"] for f in air) - z0
    collapse = next((i for i in range(jump, end + 1) if int(frames[i]["events"]) & EVENT_COLLAPSE), None)
    out = {
        "jump": True, "takeoff_speed_cm_s": round(speed0, 1), "takeoff_vz_cm_s": round(f0["vz"], 1), "apex_cm": round(apex, 1),
        "air_time_s": round(frames[end]["time"] - f0["time"], 3), "landed": land is not None,
        "speed_collapse": collapse is not None, "step_ups": sum(1 for f in frames if int(f["events"]) & EVENT_STEP),
        "max_step_up_cm": round(max([frames[i]["z"] - frames[i - 1]["z"] for i in range(1, len(frames)) if int(frames[i]["events"]) & EVENT_STEP] or [0.0]), 2),
        "min_horizontal_speed_ratio": round(min(math.hypot(f["vx"], f["vy"]) for f in air) / speed0, 3) if speed0 > 1e-6 else None,
    }
    if collapse is not None:
        out["collapse_after_s"] = round(frames[collapse]["time"] - f0["time"], 3)
        rising = [f for f in frames[collapse:end + 1] if f["vz"] > 0 and math.hypot(f["vx"], f["vy"]) < 0.2 * speed0]
        out["vertical_climb_s"] = round(len(rising) / max(len(air) / max(out["air_time_s"], 1e-6), 1e-6), 3) if rising else 0.0
    if ledge_cm is not None and distance_cm is not None and speed0 > 1e-6 and f0["vz"] > 0:
        # The first airborne frame is already one step into the flight: add that step's gravity back to estimate the launch velocity.
        dt = f0["time"] - frames[jump - 1]["time"] if jump > 0 else 1.0 / 60.0
        out["prediction"] = predict_outcome(f0["vz"] + gravity_cm * dt, gravity_cm, speed0, distance_cm, ledge_cm)
    return out


def predict_outcome(v0, g, speed, distance, ledge):
    """Port of Core JumpModel::EvaluateJump (any consistent unit). Returns the outcome name and the take-off window."""
    disc = v0 * v0 - 2.0 * g * ledge
    if v0 <= 0 or g <= 0 or speed <= 0 or ledge <= 0 or disc <= 1e-9:
        return {"outcome": "TooHigh"}
    root = math.sqrt(disc)
    lo, hi = speed * (v0 - root) / g, speed * (v0 + root) / g
    outcome = "BlockedRising" if distance < lo else "FallsShort" if distance > hi else "Clears"
    return {"outcome": outcome, "min_distance": round(lo, 2), "max_distance": round(hi, 2)}


def main(argv):
    if len(argv) < 3 or argv[1] not in ("generator", "diag"):
        print(__doc__)
        return 2
    with open(argv[2], encoding="utf-8", errors="replace") as handle:
        text = handle.read()
    if argv[1] == "generator":
        verdict = generator_verdict(text)
        print(json.dumps(verdict, indent=1))
        return 0 if verdict["status"] in ("saved", "kept") else 1
    opts = {argv[i]: argv[i + 1] for i in range(3, len(argv) - 1, 2)}
    ledge = float(opts["--ledge-cm"]) if "--ledge-cm" in opts else None
    dist = float(opts["--distance-cm"]) if "--distance-cm" in opts else None
    dumps, metas = parse_diag(text), parse_diag_meta(text)
    result = []
    for index, dump in enumerate(dumps):
        item = summarize_jump(dump, ledge, dist)
        meta = metas[index] if index < len(metas) else {"config": None, "hits": []}
        if meta["config"]:
            item["config"] = meta["config"]
            item["hit_summary"] = summarize_hits(dump, meta["hits"], meta["config"].get("walkableZ", 0.71))
        elif meta["hits"]:
            item["hit_summary"] = summarize_hits(dump, meta["hits"])
        result.append(item)
    print(json.dumps(result, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
