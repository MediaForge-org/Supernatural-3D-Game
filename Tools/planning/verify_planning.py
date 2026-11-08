#!/usr/bin/env python3
"""Read-only check of Docs/Planning: every source reference must point to an existing file and line.

Usage: python3 Tools/planning/verify_planning.py
Exit code 0 = all references resolve; 1 = at least one broken reference.
Also writes Docs/Planning/progress.json (package status), no other file is changed.
"""
import json
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
PLANNING = ROOT / "Docs" / "Planning"
REF = re.compile(r"`?((?:Docs|Planning|CLAUDE\.md|Tools)[^`:\s|)]*\.(?:md|py|json))(?::(\d+)(?:[–-](\d+))?)?`?")


def check_file(path: pathlib.Path):
    problems, count = [], 0
    text = path.read_text(encoding="utf-8")
    for m in REF.finditer(text):
        rel, start, end = m.group(1), m.group(2), m.group(3)
        target = ROOT / rel
        count += 1
        if not target.exists():
            problems.append(f"{path.name}: Datei fehlt: {rel}")
            continue
        if start:
            n_lines = sum(1 for _ in target.open(encoding="utf-8", errors="replace"))
            last = int(end or start)
            if int(start) > n_lines or last > n_lines:
                problems.append(f"{path.name}: Zeile {start}-{last} außerhalb von {rel} ({n_lines} Zeilen)")
    return count, problems


def main() -> int:
    total, problems = 0, []
    for name in ["DECISIONS.md", "SOURCES.md", "CONFLICTS.md", "OPEN_QUESTIONS.md", "README.md"]:
        f = PLANNING / name
        if f.exists():
            c, p = check_file(f)
            total += c
            problems += p
    status = {
        "package": 2,
        "references_checked": total,
        "broken_references": problems,
        "files": ["README.md", "DECISIONS.md", "SOURCES.md", "CONFLICTS.md", "OPEN_QUESTIONS.md", "CHANGELOG.md"],
        "next": ["Paket 2: Docs/Game/Entscheidungsstatus.md §6.5–7", "Paket 3: Docs/Game/Gameplay-Systeme.md",
                 "Paket 4: Docs/Game/Spielvision.md", "Paket 5: Docs/Game/Spielwelt-und-Untergrund.md",
                 "Paket 6: 01_CURRENT_CANONICAL_STATE (Master)"],
    }
    (PLANNING / "progress.json").write_text(json.dumps(status, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"references checked: {total}, broken: {len(problems)}")
    for p in problems:
        print("  -", p)
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
