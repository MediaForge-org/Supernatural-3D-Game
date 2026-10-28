"""Tests of spn_qa_report.py: statuses are counted honestly, a suite without records is NOT_TESTED (never PASS), video offsets come from epochs."""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spn_qa_report as report

RECORDS = [
    {"suite": "Doors", "id": "A.opens", "status": "PASS", "detail": "ok", "epoch": 1010.0, "pos": [1, 2, 3], "speed2d": 0},
    {"suite": "Doors", "id": "A.close", "status": "FAIL", "detail": "no", "epoch": 1070.5, "pos": [1, 2, 3], "speed2d": 5},
    {"suite": "SaveLoad", "id": "pos", "status": "NOT_IMPLEMENTED", "detail": "x", "epoch": 1080.0, "pos": [0, 0, 0], "speed2d": 0},
]


class ReportTest(unittest.TestCase):
    def test_counts_and_missing_suites(self):
        info = {"expected_suites": ["Doors", "SaveLoad", "Jump"], "video_start_epoch": 1000.0}
        results = report.build_results(RECORDS, info)
        self.assertEqual(results["counts"]["PASS"], 1)
        self.assertEqual(results["counts"]["FAIL"], 1)
        self.assertEqual(results["counts"]["NOT_IMPLEMENTED"], 1)
        self.assertEqual(results["counts"]["NOT_TESTED"], 1)  # Jump produced nothing
        self.assertEqual(results["suites_without_records"], ["Jump"])

    def test_video_offsets(self):
        results = report.build_results(RECORDS, {"expected_suites": [], "video_start_epoch": 1000.0})
        self.assertAlmostEqual(results["checks"][1]["video_offset_s"], 70.5)
        self.assertEqual(report.format_offset(70.5), "1:10.5")
        self.assertIsNone(report.build_results(RECORDS, {"expected_suites": []})["checks"][0]["video_offset_s"])

    def test_markdown_lists_failures_and_gaps(self):
        text = report.render_markdown(report.build_results(RECORDS, {"expected_suites": ["Doors"], "video_start_epoch": 1000.0}))
        self.assertIn("Doors.A.close", text)
        self.assertIn("NOT_IMPLEMENTED", text)
        self.assertIn("PASS 1, FAIL 1", text)

    def test_performance_parse(self):
        rec = [{"suite": "Performance", "id": "frame_stats", "status": "PASS", "epoch": 1,
                "detail": "avg 120.5 FPS (8.30 ms), p95 9.00 ms, p99 11.00 ms, worst 35.0 ms, game thread 2.50 ms, render thread 3.00 ms, GPU 4.40 ms, draw calls 200, used physical memory 3000 MB, 700 frames"}]
        perf = report.parse_performance(rec)
        self.assertEqual(perf["avg_fps"], 120.5)
        self.assertEqual(perf["memory_mb"], 3000.0)

    def test_invalid_status_is_blocked_not_dropped_and_duplicates_are_reported(self):
        records = RECORDS + [{"suite": "Doors", "id": "A.opens", "status": "OK", "detail": "x", "epoch": 1.0}, {"id": "no suite", "status": "PASS"}]
        results = report.build_results(records, {"expected_suites": []})
        self.assertEqual(results["counts"]["BLOCKED"], 1)                       # "OK" is not a status
        self.assertEqual(sum(results["counts"].values()), len(records))        # nothing is lost
        text = " ".join(results["record_issues"])
        self.assertIn("invalid status 'OK'", text)
        self.assertIn("Doors.A.opens recorded 2 times", text)
        self.assertIn("lacks suite or id", text)
        self.assertIn("invalid status", [c for c in results["checks"] if c.get("status") == "BLOCKED"][0]["detail"])

    def test_comparison_finds_regressions_fixes_and_vanished_checks(self):
        def run(*pairs):
            return {"checks": [{"suite": k.split(".")[0], "id": k.split(".", 1)[1], "status": s} for k, s in pairs]}
        old = run(("Doors.a", "PASS"), ("Doors.b", "FAIL"), ("Jump.c", "FAIL"), ("Jump.d", "PASS"), ("Jump.e", "NOT_IMPLEMENTED"), ("Jump.f", "PASS"), ("Jump.f", "FAIL"))
        new = run(("Doors.a", "FAIL"), ("Doors.b", "PASS"), ("Jump.c", "FAIL"), ("Jump.e", "PASS"), ("Jump.g", "PASS"), ("Jump.f", "FAIL"))
        diff = report.compare_runs(old, new)
        self.assertEqual(diff["regressions"], [("Doors.a", "PASS", "FAIL")])
        self.assertEqual(diff["fixed"], [("Doors.b", "FAIL", "PASS")])
        self.assertEqual(sorted(diff["still_failing"]), [("Jump.c", "FAIL"), ("Jump.f", "FAIL")])   # the worst of several records counts
        self.assertEqual(diff["disappeared"], [("Jump.d", "PASS")])
        self.assertEqual(diff["new_checks"], [("Jump.g", "PASS")])
        self.assertEqual(diff["other"], [("Jump.e", "NOT_IMPLEMENTED", "PASS")])
        self.assertIn("Regressions", "\n".join(report.render_comparison(diff, "prev")))
        self.assertEqual(report.render_comparison(report.compare_runs(old, old), "prev")[2], "**Still failing (3)**")

    def test_artifact_problems_and_previous_run_lookup(self):
        import json
        import tempfile
        with tempfile.TemporaryDirectory() as root:
            for name in ("20261001-1000", "20261002-1000", "20261003-1000"):
                os.makedirs(os.path.join(root, name))
            for name in ("20261001-1000", "20261002-1000"):
                with open(os.path.join(root, name, "results.json"), "w") as handle:
                    json.dump({"checks": []}, handle)
            current = os.path.join(root, "20261003-1000")
            self.assertEqual(os.path.basename(report.previous_run_dir(current)), "20261002-1000")
            self.assertIsNone(report.previous_run_dir(os.path.join(root, "20261001-1000")))
            problems = report.artifact_problems(current, {"video": "gameplay.mp4", "clips": ["clips/a.mp4"]})
            self.assertEqual(len(problems), 3)   # video, clip, records.jsonl
            open(os.path.join(current, "records.jsonl"), "w").close()
            open(os.path.join(current, "gameplay.mp4"), "w").close()
            self.assertIn("video is empty: gameplay.mp4", report.artifact_problems(current, {"video": "gameplay.mp4"}))
            self.assertIn("records.jsonl is empty", report.artifact_problems(current, {}))

    def test_main_writes_comparison_and_returns_failure_for_fails(self):
        import io
        import json
        import tempfile
        from contextlib import redirect_stdout
        with tempfile.TemporaryDirectory() as root:
            for name, status in (("20261001-1000", "PASS"), ("20261002-1000", "FAIL")):
                run = os.path.join(root, name)
                os.makedirs(run)
                with open(os.path.join(run, "records.jsonl"), "w") as handle:
                    handle.write(json.dumps({"suite": "Doors", "id": "x", "status": status, "detail": "d", "epoch": 1.0}) + "\n")
                with redirect_stdout(io.StringIO()) as out:
                    code = report.main([run])
            self.assertEqual(code, 1)
            self.assertIn("REGRESSIONS since the previous run: 1", out.getvalue())
            with open(os.path.join(root, "20261002-1000", "report.md"), encoding="utf-8") as handle:
                self.assertIn("Doors.x: PASS -> FAIL", handle.read())


if __name__ == "__main__":
    unittest.main()
