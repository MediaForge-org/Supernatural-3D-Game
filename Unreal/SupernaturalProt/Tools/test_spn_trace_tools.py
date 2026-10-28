"""Tests of spn_trace_tools.py on synthetic traces."""
import json
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spn_trace_tools as T  # noqa: E402

COLUMNS = ["t", "speed", "gait", "local_angle", "actor_yaw", "vel_heading", "accel_x", "accel_y"]


def turn_rows(side, lose_sprint_at=None, frames=30, dt=1 / 60.0):
    """Mirror-symmetric synthetic turn: the actor turns 500 deg/s towards side*90 degrees, speed dips to 5.8 m/s."""
    rows = []
    for i in range(frames):
        t = (i + 1) * dt
        turned = min(500.0 * t, 90.0)
        speed = 800.0 - 220.0 * min(t / 0.25, 1.0)
        gait = 2 if lose_sprint_at is not None and lose_sprint_at <= t < lose_sprint_at + 0.1 else 3
        rows.append([t, speed, gait, side * (90.0 - turned), 90.0 + side * turned, 90.0 + side * turned, 0.0, side])
    return rows


def write(path, items):
    with open(path, "w", encoding="utf-8") as handle:
        for case, rows in items:
            handle.write(json.dumps({"suite": "Turns", "id": case, "epoch": 1.0, "columns": COLUMNS, "rows": rows}) + "\n")


class TraceTest(unittest.TestCase):
    def load(self, items):
        with tempfile.TemporaryDirectory() as root:
            write(os.path.join(root, "traces.jsonl"), items)
            return T.read_traces(root)

    def test_symmetric_turns(self):
        traces, problems = self.load([("sprint_left_90#1", turn_rows(-1)), ("sprint_right_90#1", turn_rows(+1))])
        self.assertEqual(problems, [])
        result = T.mirrored_turns(traces)
        self.assertEqual(result["verdict"], "symmetric")
        self.assertEqual(result["pairs"][0]["flags"], [])

    def test_gait_loss_on_one_side_is_found_and_named(self):
        traces, _ = self.load([("sprint_left_90#1", turn_rows(-1, lose_sprint_at=0.05)), ("sprint_right_90#1", turn_rows(+1))])
        result = T.mirrored_turns(traces)
        self.assertEqual(result["verdict"], "asymmetric")
        self.assertIn("sprint lost", result["pairs"][0]["flags"][0])
        self.assertTrue(any("sprint-sustain" in h for h in result["hints"]))   # rotation mirrors, the gait does not
        self.assertAlmostEqual(result["pairs"][0]["left"]["first_sprint_loss_t"], 0.05, delta=0.02)

    def test_rotation_asymmetry_points_to_input_or_actor_rotation(self):
        slow_left = [r[:] for r in turn_rows(-1)]
        for r in slow_left:
            r[3] *= 0.5            # the local angle of the left turn is not the mirror of the right one
        traces, _ = self.load([("sprint_left_90#1", slow_left), ("sprint_right_90#1", turn_rows(+1))])
        result = T.mirrored_turns(traces)
        self.assertEqual(result["verdict"], "asymmetric")
        self.assertTrue(any("input mapping" in h for h in result["hints"]))

    def test_only_matching_trials_are_compared_and_missing_data_is_reported(self):
        traces, _ = self.load([("sprint_left_90#1", turn_rows(-1)), ("sprint_right_90#2", turn_rows(+1)), ("jog_left_90#1", turn_rows(-1))])
        self.assertEqual(T.mirrored_turns(traces)["verdict"], "no_data")        # no common trial: never "symmetric" by default
        self.assertEqual(T.mirrored_turns([])["verdict"], "no_data")

    def test_bad_lines_are_reported_not_dropped_silently(self):
        with tempfile.TemporaryDirectory() as root:
            path = os.path.join(root, "traces.jsonl")
            write(path, [("a#1", turn_rows(1))])
            with open(path, "a", encoding="utf-8") as handle:
                handle.write("not json\n")
                handle.write(json.dumps({"suite": "Turns", "id": "b#1", "columns": ["t"], "rows": [[1, 2]]}) + "\n")
                handle.write(json.dumps({"suite": "Turns", "id": "c#1", "columns": ["t"], "rows": [["x"]]}) + "\n")
            traces, problems = T.read_traces(root)
        self.assertEqual(len(traces), 1)
        self.assertEqual(len(problems), 3)
        self.assertEqual(T.read_traces("/nonexistent/dir")[0], [])

    def test_cli_exit_codes(self):
        import io
        from contextlib import redirect_stdout
        with tempfile.TemporaryDirectory() as root:
            write(os.path.join(root, "traces.jsonl"), [("sprint_left_90#1", turn_rows(-1, lose_sprint_at=0.05)), ("sprint_right_90#1", turn_rows(+1))])
            with redirect_stdout(io.StringIO()):
                self.assertEqual(T.main(["x", root]), 1)
            write(os.path.join(root, "traces.jsonl"), [("sprint_left_90#1", turn_rows(-1)), ("sprint_right_90#1", turn_rows(+1))])
            with redirect_stdout(io.StringIO()):
                self.assertEqual(T.main(["x", root]), 0)


JUMP_COLUMNS = ["t", "y_to_face", "z", "vy", "vz", "mode", "floor_dist", "walkable", "x_drift"]


def jump_rows(kind, ledge=50.0, takeoff_gap=10.0, speed=550.0, v0=297.0, g=980.0, dt=1 / 120.0):
    """kind: 'air_block' (the face stops vy, the capsule climbs vertically), 'perch' (apex just under the ledge, then walking mode rises
    over the edge), 'clear' (high enough, flies over)."""
    rows, y, z, vy, vz, mode, t = [], takeoff_gap, 0.0, speed, v0, MODE_FALLING_T, 0.0
    while t < 1.4:
        vz -= g * dt if mode == MODE_FALLING_T else 0.0
        z += vz * dt if mode == MODE_FALLING_T else 0.0
        if kind == "air_block" and y <= 0.0 and z < ledge:
            vy = 20.0
        if kind == "perch" and y <= 0.0 and z >= ledge - 8.0 and mode == MODE_FALLING_T and vz < 0:
            mode, vz = 1, 0.0           # lands on the corner: walking mode, then rises 8 cm in small steps
        if kind == "perch" and mode == 1 and z < ledge:
            z += min(1.2, ledge - z)
        if kind == "clear" and y <= 0.0:
            z = max(z, ledge + 1.0) if t > 0.3 else z
        y -= vy * dt
        rows.append([t, y, z, vy, vz, mode, 0.0, 1.0 if mode == 1 else 0.0, 0.0])
        t += dt
    return [dict(zip(JUMP_COLUMNS, r)) for r in rows]


MODE_FALLING_T = 3


class JumpTraceTest(unittest.TestCase):
    def test_air_block_vs_perch_climb_vs_no_ledge(self):
        air = T.jump_facts(jump_rows("air_block"))
        self.assertEqual(air["classification"], "vertical_climb_in_air")
        self.assertGreater(air["air_climb_frames"], 0)
        perch = T.jump_facts(jump_rows("perch"))
        self.assertEqual(perch["classification"], "climbed_by_walking_after_contact")
        self.assertGreaterEqual(perch["walking_rise_cm"], 3.0)
        flat = [dict(r, y_to_face=0.0) for r in jump_rows("clear")]
        self.assertEqual(T.jump_facts(flat)["classification"], "no_ledge")
        self.assertIsNone(T.jump_facts(flat[:2]))

    def test_apex_and_contact_facts(self):
        facts = T.jump_facts(jump_rows("clear", ledge=30.0, takeoff_gap=400.0))
        self.assertAlmostEqual(facts["apex_cm"], 297.0 ** 2 / (2 * 980.0), delta=4.0)     # kinematic apex of the synthetic jump (semi-implicit Euler at 120 Hz loses a few cm)
        self.assertIsNotNone(facts["contact_index"])
        self.assertIn("z_at_contact", facts)


if __name__ == "__main__":
    unittest.main()
