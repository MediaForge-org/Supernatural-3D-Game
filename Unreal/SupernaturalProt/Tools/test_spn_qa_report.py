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


if __name__ == "__main__":
    unittest.main()
