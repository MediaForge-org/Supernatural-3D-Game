"""Keeps the gameplay QA inventory (qa_inventory.json) in step with the code, so a new gameplay system cannot be added without a test or an
explicit, written coverage gap.

  check_qa_inventory.py --check   exit 1 when: a UCLASS/UINTERFACE class, a Core header, a Spn.* console command or a Default*Key binding
                                  is not listed in any entry; a listed test does not exist in the sources; an entry has neither a test
                                  nor a gap; or Docs/Development/Gameplay-QA-Inventory.md is out of date.
  check_qa_inventory.py --write   regenerates that Markdown document.

Scanned sources: Unreal/SupernaturalProt/Source/** (minus ignore_source_dirs), Core/include/spn/core/*.h, Tests/Core/*.cpp.
Test names are taken from IMPLEMENT_*AUTOMATION_TEST macros ("SpnGame.x.y") and "core:<path>" entries must be existing files.
Pure standard library, tested by test_check_qa_inventory.py.
"""

import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJECT = os.path.dirname(HERE)
REPO = os.path.dirname(os.path.dirname(PROJECT))
INVENTORY = os.path.join(HERE, "qa_inventory.json")
DOC = os.path.join(REPO, "Docs", "Development", "Gameplay-QA-Inventory.md")
SOURCE = os.path.join(PROJECT, "Source")
CORE_HEADERS = os.path.join(REPO, "Core", "include", "spn", "core")


def load_inventory(path=INVENTORY):
    with open(path, encoding="utf-8") as handle:
        return json.load(handle)


def walk(root, suffixes, ignore_dirs=()):
    for base, dirs, files in os.walk(root):
        dirs[:] = [d for d in dirs if d not in ignore_dirs]
        for name in files:
            if name.endswith(suffixes):
                yield os.path.join(base, name)


def read(path):
    with open(path, encoding="utf-8", errors="replace") as handle:
        return handle.read()


def scan_classes(source_root, ignore_dirs):
    """UCLASS / UINTERFACE class names of the headers (generated headers and tests excluded)."""
    found = set()
    for path in walk(source_root, (".h",), ignore_dirs):
        if "Tests" in path.split(os.sep):
            continue
        text = read(path)
        for match in re.finditer(r"\b(?:UCLASS|UINTERFACE)\s*\((?:[^()]|\([^()]*\))*\)\s*class\s+(?:[A-Z_]+_API\s+)?([A-Za-z0-9_]+)", text):
            found.add(match.group(1))
    return found


def scan_core_headers(core_dir):
    return {name for name in os.listdir(core_dir) if name.endswith(".h")} if os.path.isdir(core_dir) else set()


def scan_console_commands(source_root, ignore_dirs):
    found = set()
    for path in walk(source_root, (".cpp",), ignore_dirs):
        if "Tests" in path.split(os.sep):
            continue
        for match in re.finditer(r'FAutoConsole(?:Command|CommandWithWorld|CommandWithWorldAndArgs|CommandWithArgs)\w*\s+\w+\s*\(\s*TEXT\("([^"]+)"', read(path)):
            found.add(match.group(1))
    return found


def scan_key_bindings(source_root, ignore_dirs):
    found = set()
    for path in walk(source_root, (".h",), ignore_dirs):
        for match in re.finditer(r"\bFKey\s+(Default[A-Za-z]*Key)\b", read(path)):
            found.add(match.group(1))
    return found


def scan_tests(source_root):
    found = set()
    for path in walk(source_root, (".cpp",)):
        for match in re.finditer(r'IMPLEMENT_[A-Z_]*AUTOMATION_TEST[A-Z_]*\s*\(\s*\w+\s*,\s*"([^"]+)"', read(path)):
            found.add(match.group(1))
    return found


def listed(inventory, field):
    items = set()
    for entry in inventory["entries"]:
        items.update(entry.get(field, []))
    return items


def test_exists(test, automation_tests, repo=REPO):
    if test.startswith("core:"):
        return os.path.isfile(os.path.join(repo, test[len("core:"):]))
    return test in automation_tests


def check(inventory, classes, core_headers, commands, keys, automation_tests, repo=REPO):
    problems = []
    for label, found, field in (("class", classes, "classes"), ("Core header", core_headers, "core_headers"), ("console command", commands, "console"), ("key binding", keys, "keys")):
        for item in sorted(found - listed(inventory, field)):
            problems.append("%s %s is not in qa_inventory.json (add it to an entry's \"%s\" list with a test or a gap)" % (label, item, field))
    seen_ids = set()
    for entry in inventory["entries"]:
        if entry["id"] in seen_ids:
            problems.append("duplicate entry id " + entry["id"])
        seen_ids.add(entry["id"])
        tests = entry.get("tests", [])
        if not tests and not entry.get("gap"):
            problems.append("entry %s has neither a test nor a gap description" % entry["id"])
        for test in tests:
            if not test_exists(test, automation_tests, repo):
                problems.append("entry %s lists the test %s, which does not exist in the sources" % (entry["id"], test))
        for field, found in (("classes", classes), ("core_headers", core_headers), ("console", commands), ("keys", keys)):
            for item in entry.get(field, []):
                if item not in found:
                    problems.append("entry %s lists %s %s, which no longer exists in the sources" % (entry["id"], field, item))
    return problems


def render(inventory, automation_tests, repo=REPO):
    lines = [
        "# Gameplay QA inventory", "",
        "Generated by `Unreal/SupernaturalProt/Tools/check_qa_inventory.py --write` from `Tools/qa_inventory.json`. Do not edit by hand; add new systems to the JSON.",
        "`Tools/run_gameplay_qa.ps1` refuses to run while a gameplay class, Core header, `Spn.*` console command or key binding is missing from the inventory.", "",
        "Test kinds: `SpnQA.*` = gameplay QA (real key input, recorded, in the run report); `SpnGame.*` / `SpnCore.*` = Unreal automation regression tests; `core:` = engine-independent Core tests (CMake/CTest, run on the Fedora machine, not by the Windows QA script).", "",
        "| System | Covered by | Coverage gap |", "|---|---|---|",
    ]
    for entry in inventory["entries"]:
        tests = ", ".join("`%s`" % t for t in entry.get("tests", [])) or "**none**"
        gap = entry.get("gap", "") or "none known"
        lines.append("| **%s**<br>%s | %s | %s |" % (entry["id"], entry["title"], tests, gap.replace("|", "/")))
    used = listed(inventory, "tests")
    unlisted = sorted(t for t in automation_tests if t not in used)
    lines += ["", "## Automation tests not assigned to any system", ""]
    lines += ["- `%s`" % t for t in unlisted] or ["none"]
    lines += ["", "## Ignored sources", "", inventory.get("ignore_note", ""), ""]
    return "\n".join(lines)


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    inventory = load_inventory()
    ignore = tuple(inventory.get("ignore_source_dirs", []))
    automation_tests = scan_tests(SOURCE)
    problems = check(inventory, scan_classes(SOURCE, ignore), scan_core_headers(CORE_HEADERS), scan_console_commands(SOURCE, ignore),
                     scan_key_bindings(SOURCE, ignore), automation_tests)
    text = render(inventory, automation_tests)
    if "--write" in argv:
        os.makedirs(os.path.dirname(DOC), exist_ok=True)
        with open(DOC, "w", encoding="utf-8", newline="\n") as handle:
            handle.write(text)
        print("wrote " + DOC)
    elif "--check" in argv:
        current = read(DOC).replace("\r\n", "\n") if os.path.isfile(DOC) else ""
        if current != text:
            problems.append("Docs/Development/Gameplay-QA-Inventory.md is out of date: run check_qa_inventory.py --write")
    for problem in problems:
        print("QA INVENTORY: " + problem)
    covered = sum(1 for e in inventory["entries"] if e.get("tests"))
    print("QA inventory: %d systems, %d with tests, %d with a documented gap, %d problem(s)" % (len(inventory["entries"]), covered, sum(1 for e in inventory["entries"] if e.get("gap")), len(problems)))
    return 1 if problems and "--write" not in argv else 0


if __name__ == "__main__":
    sys.exit(main())
