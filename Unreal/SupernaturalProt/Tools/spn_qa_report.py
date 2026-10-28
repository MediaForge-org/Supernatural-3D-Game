"""Builds results.json and report.md of one gameplay QA run (Tools/run_gameplay_qa.ps1) from the files the run leaves in its directory:

  records.jsonl        one JSON line per check, written by the SpnQA.* automation tests (Spn/Tests/SpnQaRecorder.h)
  run_info.json        written by the wrapper: engine, git, map, suite, video start epoch, paths
  automation/index.json  Unreal's own test report (optional)
  perf in records      the SpnQA.Performance record carries the frame statistics as text

Statuses: PASS, FAIL, BLOCKED, NOT_IMPLEMENTED, NOT_TESTED. A test that produced no record is NOT_TESTED, never PASS.
Pure standard library; tested in test_spn_qa_report.py.
"""

import argparse
import datetime
import json
import os
import re
import sys

STATUSES = ("PASS", "FAIL", "BLOCKED", "NOT_IMPLEMENTED", "NOT_TESTED")


def read_records(path):
    records = []
    if not os.path.isfile(path):
        return records
    with open(path, encoding="utf-8") as handle:
        for line in handle:
            line = line.strip()
            if line:
                try:
                    records.append(json.loads(line))
                except ValueError:
                    records.append({"suite": "?", "id": "unparsable record", "status": "BLOCKED", "detail": line[:200], "epoch": 0})
    return records


def video_offset(record, video_start_epoch):
    if not video_start_epoch or not record.get("epoch"):
        return None
    return max(0.0, record["epoch"] - video_start_epoch)


def format_offset(seconds):
    if seconds is None:
        return "-"
    return "%d:%04.1f" % (int(seconds // 60), seconds % 60)


def expected_suites(run_info):
    return list(run_info.get("expected_suites", []))


def summarize(records, run_info):
    counts = {status: 0 for status in STATUSES}
    for record in records:
        counts[record.get("status", "NOT_TESTED")] = counts.get(record.get("status", "NOT_TESTED"), 0) + 1
    seen = {record.get("suite") for record in records}
    missing = [suite for suite in expected_suites(run_info) if suite not in seen]
    counts["NOT_TESTED"] += len(missing)
    return counts, missing


def parse_performance(records):
    for record in records:
        if record.get("suite") == "Performance" and record.get("id") == "frame_stats":
            detail = record.get("detail", "")
            fields = {}
            patterns = {
                "avg_fps": r"avg ([0-9.]+) FPS", "avg_ms": r"\(([0-9.]+) ms\)", "p95_ms": r"p95 ([0-9.]+) ms", "p99_ms": r"p99 ([0-9.]+) ms",
                "worst_ms": r"worst ([0-9.]+) ms", "game_thread_ms": r"game thread ([0-9.]+) ms", "render_thread_ms": r"render thread ([0-9.]+) ms",
                "gpu_ms": r"GPU ([0-9.]+) ms", "draw_calls": r"draw calls ([0-9.]+)", "memory_mb": r"used physical memory ([0-9.]+) MB",
            }
            for key, pattern in patterns.items():
                match = re.search(pattern, detail)
                if match:
                    fields[key] = float(match.group(1))
            return fields
    return {}


def build_results(records, run_info, automation_tests=None):
    counts, missing = summarize(records, run_info)
    start = run_info.get("video_start_epoch")
    entries = []
    for record in records:
        entry = dict(record)
        entry["video_offset_s"] = video_offset(record, start)
        entries.append(entry)
    return {
        "run": run_info,
        "counts": counts,
        "suites_without_records": missing,
        "performance": parse_performance(records),
        "checks": entries,
        "automation_tests": automation_tests or [],
    }


def load_automation_tests(index_path):
    """Test name + state from Unreal's index.json (best effort: the format is Unreal's, not ours)."""
    if not os.path.isfile(index_path):
        return []
    try:
        with open(index_path, encoding="utf-8-sig") as handle:
            data = json.load(handle)
    except ValueError:
        return []
    tests = []
    for test in data.get("tests", []):
        tests.append({"name": test.get("fullTestPath") or test.get("testDisplayName"), "state": test.get("state"),
                      "errors": (test.get("errors") if "errors" in test else None)})
    return tests


def md_escape(text):
    return str(text).replace("|", "\\|").replace("\n", " ")


def render_markdown(results, shots_dir_listing=None):
    run = results["run"]
    counts = results["counts"]
    lines = ["# Gameplay QA - %s" % run.get("run_name", ""), ""]
    lines += [
        "| | |", "|---|---|",
        "| Unreal | %s |" % md_escape(run.get("engine_version", "?")),
        "| Git | %s%s |" % (run.get("git_commit", "?"), " (uncommitted changes in the working tree)" if run.get("git_dirty") else ""),
        "| Map | %s |" % md_escape(run.get("map", "?")),
        "| Suite | %s |" % md_escape(run.get("suite", "?")),
        "| Date | %s |" % md_escape(run.get("date", "?")),
        "| Video | %s |" % md_escape(run.get("video") or "none (%s)" % run.get("video_note", "not recorded")),
        "| Game exit code | %s%s |" % (run.get("exit_code", "?"), " (timeout, process stopped by the wrapper)" if run.get("timed_out") else ""),
        "", "## Result", "",
        "PASS %d, FAIL %d, BLOCKED %d, NOT_IMPLEMENTED %d, NOT_TESTED %d" % tuple(counts.get(s, 0) for s in STATUSES), "",
    ]
    if results["suites_without_records"]:
        lines += ["Suites that produced no record (counted as NOT_TESTED): %s" % ", ".join(results["suites_without_records"]), ""]
    perf = results.get("performance") or {}
    if perf:
        lines += ["## Performance (during recording)", "",
                  "avg %.1f FPS, frame %.2f ms (p95 %.2f, p99 %.2f, worst %.1f), game thread %.2f ms, render thread %.2f ms, GPU %.2f ms, draw calls %.0f, memory %.0f MB" % (
                      perf.get("avg_fps", 0), perf.get("avg_ms", 0), perf.get("p95_ms", 0), perf.get("p99_ms", 0), perf.get("worst_ms", 0),
                      perf.get("game_thread_ms", 0), perf.get("render_thread_ms", 0), perf.get("gpu_ms", 0), perf.get("draw_calls", 0), perf.get("memory_mb", 0)),
                  "(the video encoder runs on the same machine; numbers are lower bounds of an unrecorded run)", ""]
    problems = [c for c in results["checks"] if c.get("status") in ("FAIL", "BLOCKED")]
    lines += ["## Failures and blocked checks (%d)" % len(problems), ""]
    if problems:
        lines += ["| Video | Check | Status | Position | Speed | Detail |", "|---|---|---|---|---|---|"]
        for c in problems:
            pos = c.get("pos", [0, 0, 0])
            lines.append("| %s | %s.%s | %s | (%.0f, %.0f, %.0f) | %.0f cm/s | %s |" % (
                format_offset(c.get("video_offset_s")), c.get("suite"), md_escape(c.get("id")), c.get("status"), pos[0], pos[1], pos[2],
                c.get("speed2d", 0), md_escape(c.get("detail"))))
        lines.append("")
    else:
        lines += ["none", ""]
    ni = [c for c in results["checks"] if c.get("status") in ("NOT_IMPLEMENTED", "NOT_TESTED")]
    lines += ["## Not implemented / not tested (%d)" % len(ni), ""]
    lines += ["- %s.%s [%s]: %s" % (c.get("suite"), c.get("id"), c.get("status"), c.get("detail")) for c in ni] or ["none"]
    lines += ["", "## All checks", "", "| Video | Check | Status | Detail |", "|---|---|---|---|"]
    for c in results["checks"]:
        lines.append("| %s | %s.%s | %s | %s |" % (format_offset(c.get("video_offset_s")), c.get("suite"), md_escape(c.get("id")), c.get("status"), md_escape(c.get("detail"))))
    if results.get("automation_tests"):
        lines += ["", "## Unreal automation tests", "", "| Test | State |", "|---|---|"]
        lines += ["| %s | %s |" % (t.get("name"), t.get("state")) for t in results["automation_tests"]]
    if shots_dir_listing:
        lines += ["", "## Screenshots", ""] + ["- %s" % s for s in shots_dir_listing]
    clips = run.get("clips") or []
    if clips:
        lines += ["", "## Clips of failed checks", ""] + ["- %s" % c for c in clips]
    lines.append("")
    return "\n".join(lines)


def failing_suites(results_path):
    """Test names (SpnQA.<suite>) with at least one FAIL/BLOCKED check or no record, for the wrapper's -Suite Failed."""
    with open(results_path, encoding="utf-8") as handle:
        results = json.load(handle)
    names = {"SpnQA." + c["suite"] for c in results["checks"] if c.get("status") in ("FAIL", "BLOCKED")}
    names |= {"SpnQA." + s for s in results.get("suites_without_records", [])}
    return sorted(names)


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument("run_dir")
    parser.add_argument("--failing-suites", action="store_true", help="print the failing test names of an earlier run_dir/results.json and exit")
    args = parser.parse_args(argv)
    if args.failing_suites:
        print("+".join(failing_suites(os.path.join(args.run_dir, "results.json"))))
        return 0
    info_path = os.path.join(args.run_dir, "run_info.json")
    run_info = {}
    if os.path.isfile(info_path):
        with open(info_path, encoding="utf-8-sig") as handle:
            run_info = json.load(handle)
    records = read_records(os.path.join(args.run_dir, "records.jsonl"))
    results = build_results(records, run_info, load_automation_tests(os.path.join(args.run_dir, "automation", "index.json")))
    shots = sorted(os.listdir(os.path.join(args.run_dir, "shots"))) if os.path.isdir(os.path.join(args.run_dir, "shots")) else []
    with open(os.path.join(args.run_dir, "results.json"), "w", encoding="utf-8") as handle:
        json.dump(results, handle, indent=2)
    with open(os.path.join(args.run_dir, "report.md"), "w", encoding="utf-8") as handle:
        handle.write(render_markdown(results, shots))
    counts = results["counts"]
    print("QA result: PASS %d, FAIL %d, BLOCKED %d, NOT_IMPLEMENTED %d, NOT_TESTED %d" % tuple(counts.get(s, 0) for s in STATUSES))
    return 1 if counts.get("FAIL", 0) or counts.get("BLOCKED", 0) else 0


if __name__ == "__main__":
    sys.exit(main())
