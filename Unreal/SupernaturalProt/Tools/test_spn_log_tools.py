"""Tests of the log readers on synthetic logs. The synthetic jumps come from a small simulation of the same kinematics the C++ JumpModel
describes, so the reader and the model are checked against each other; real engine logs have not been seen yet."""

import math
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import spn_log_tools as T  # noqa: E402

HEADER = "LogSupernaturalProt: Display: SpnMoveDiag,time,x,y,z,vx,vy,vz,mode,floorDist,walkable,events"


def diag_line(f):
    return "[2026.10.27-03.00.00:000][ 12]LogSupernaturalProt: Display: SpnMoveDiag,%.4f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%d,%.2f,%d,%d" % (
        f["time"], f["x"], f["y"], f["z"], f["vx"], f["vy"], f["vz"], f["mode"], f["floor"], f["walk"], f["ev"])


def simulate_jump(speed=550.0, v0=297.0, g=980.0, ledge=30.0, distance=20.0, engine_style=True, dt=1 / 60.0):
    """Frames as the recorder would log them: 10 frames walking, take-off, ballistic flight, face contact stops the speed (engine style)."""
    frames, t, x, z, vz, vx = [], 0.0, 0.0, 0.0, 0.0, speed
    for _ in range(10):
        frames.append(dict(time=t, x=x, y=0.0, z=z, vx=vx, vy=0.0, vz=0.0, mode=1, floor=0.0, walk=1, ev=0))
        x += vx * dt
        t += dt
    vz = v0
    takeoff_x = x
    first = True
    prev_speed = vx
    for _ in range(200):
        ev = 1 if first else 0
        first = False
        vz -= g * dt
        z += vz * dt
        if engine_style and x - takeoff_x >= distance and z < ledge and vx > 0:
            vx = 0.0                    # the face takes the speed
        elif not engine_style or vx > 0:
            pass
        x += vx * dt
        if prev_speed > 100 and vx < prev_speed * 0.6:
            ev |= 8
        prev_speed = vx
        if z <= 0.0 and vz < 0:
            frames.append(dict(time=t, x=x, y=0.0, z=0.0, vx=vx, vy=0.0, vz=0.0, mode=1, floor=0.0, walk=1, ev=ev | 2))
            break
        frames.append(dict(time=t, x=x, y=0.0, z=z, vx=vx, vy=0.0, vz=vz, mode=3, floor=max(z, 0.0), walk=0, ev=ev))
        t += dt
    return frames


def log_of(*dumps):
    lines = ["LogTemp: unrelated line"]
    for d in dumps:
        lines.append(HEADER)
        lines += [diag_line(f) for f in d]
        lines.append("LogTemp: more noise SpnMoveDiag mentioned in text only")
    return "\n".join(lines)


class DiagTest(unittest.TestCase):
    def test_parse_splits_dumps_and_skips_noise_and_bad_lines(self):
        text = log_of(simulate_jump(), simulate_jump(distance=200.0)) + "\nLogSupernaturalProt: Display: SpnMoveDiag,1,2,3\nSpnMoveDiag,a,b,c,d,e,f,g,h,i,j,k"
        dumps = T.parse_diag(text)
        self.assertEqual(len(dumps), 2)
        self.assertTrue(all(len(d) > 20 for d in dumps))
        self.assertEqual(T.parse_diag("nothing here"), [])
        self.assertEqual(T.parse_diag(HEADER), [[]])

    def test_blocked_rising_jump_shows_the_collapse_and_the_vertical_climb(self):
        dump = T.parse_diag(log_of(simulate_jump(distance=20.0, ledge=30.0)))[0]
        s = T.summarize_jump(dump, ledge_cm=30.0, distance_cm=20.0)
        self.assertTrue(s["jump"] and s["landed"] and s["speed_collapse"])
        self.assertAlmostEqual(s["takeoff_speed_cm_s"], 550.0, delta=1)
        self.assertAlmostEqual(s["apex_cm"], 45.0, delta=3.5)   # the 60 Hz Euler simulation undershoots the analytic apex by a few cm
        self.assertLess(s["min_horizontal_speed_ratio"], 0.01)
        self.assertGreater(s["vertical_climb_s"], 0.0)
        self.assertEqual(s["prediction"]["outcome"], "BlockedRising")
        self.assertAlmostEqual(s["prediction"]["min_distance"], 70.5, delta=1.0)     # the C++ test value 0.705 m, in cm

    def test_clean_jump_keeps_its_speed(self):
        dump = T.parse_diag(log_of(simulate_jump(distance=150.0, ledge=30.0, engine_style=False)))[0]
        s = T.summarize_jump(dump, ledge_cm=30.0, distance_cm=150.0)
        self.assertFalse(s["speed_collapse"])
        self.assertGreater(s["min_horizontal_speed_ratio"], 0.99)
        self.assertEqual(s["prediction"]["outcome"], "Clears")
        self.assertNotIn("vertical_climb_s", s)

    def test_dump_without_a_jump_and_step_ups(self):
        frames = [dict(time=i / 60.0, x=i * 5.0, y=0.0, z=0.0 if i != 5 else 20.0, vx=300.0, vy=0.0, vz=0.0, mode=1, floor=0.0, walk=1, ev=4 if i == 5 else 0) for i in range(20)]
        s = T.summarize_jump(T.parse_diag(log_of(frames))[0])
        self.assertEqual(s, {"jump": False, "step_ups": 1, "frames": 20})

    def test_prediction_port_matches_the_core_model_numbers(self):
        v0 = math.sqrt(2 * 980.0 * 45.0)
        self.assertAlmostEqual(T.predict_outcome(v0, 980.0, 550.0, 100.0, 30.0)["min_distance"], 70.5, delta=0.6)
        self.assertAlmostEqual(T.predict_outcome(v0, 980.0, 550.0, 100.0, 40.0)["min_distance"], 111.0, delta=1.0)
        self.assertAlmostEqual(T.predict_outcome(v0, 980.0, 800.0, 100.0, 30.0)["min_distance"], 102.5, delta=1.0)
        self.assertEqual(T.predict_outcome(v0, 980.0, 550.0, 100.0, 45.0)["outcome"], "TooHigh")
        self.assertEqual(T.predict_outcome(v0, 980.0, 550.0, 300.0, 30.0)["outcome"], "FallsShort")
        self.assertEqual(T.predict_outcome(0.0, 980.0, 550.0, 100.0, 30.0)["outcome"], "TooHigh")


class GeneratorLogTest(unittest.TestCase):
    OK = """LogPython: SpnMovementTestMap: native size of cube = (100.0, 100.0, 100.0)
LogPython: SpnMovementTestMap: create: no level yet
LogPython: SpnMovementTestMap: created /Game/SpnTest/Lvl_SpnMovementTestV2 with 540 shape actors for 1094 primitives (76 instance groups), 10 doors, 18 reset points"""

    def test_saved(self):
        v = T.generator_verdict(self.OK)
        self.assertEqual((v["status"], v["shape_actors"], v["primitives"], v["instance_groups"], v["doors"]), ("saved", 540, 1094, 76, 10))
        self.assertEqual(v["action"], "create")

    def test_kept(self):
        v = T.generator_verdict("LogPython: SpnMovementTestMap: keep: level exists, pass --force to rebuild\nLogPython: Warning: SpnMovementTestMap: /Game/X exists; kept unchanged (pass --force to rebuild it)")
        self.assertEqual(v["status"], "kept")
        self.assertEqual(len(v["warnings"]), 1)

    def test_failures_are_never_reported_as_saved(self):
        for bad in ("LogPython: Error: SpnMovementTestMap: generation failed, the level was NOT saved: boom",
                    "LogPython: Error: SpnMovementTestMap: layout data invalid, nothing was changed: duplicate label X",
                    "LogPython: Error: SpnMovementTestMap: 3 SPN_ actors without generator tag (older generator version); use --adopt-legacy. Nothing was deleted or changed.",
                    "LogPython: Error: SpnMovementTestMap: class not found, build the editor target first: /Script/SupernaturalProt.SpnTestDoor",
                    "LogPython: Error: SpnMovementTestMap: the generated level does not match the layout, NOT saved: doors: expected 10, found 9"):
            self.assertEqual(T.generator_verdict(bad)["status"], "failed", bad)
        self.assertEqual(T.generator_verdict(self.OK + "\nLogPython: Error: SpnMovementTestMap: could not save /Game/X")["status"], "failed")
        self.assertEqual(T.generator_verdict("")["status"], "unknown")
        self.assertEqual(T.generator_verdict("LogPython: SpnMovementTestMap: abort: 2 SPN_ actors")["status"], "failed")


if __name__ == "__main__":
    unittest.main()
