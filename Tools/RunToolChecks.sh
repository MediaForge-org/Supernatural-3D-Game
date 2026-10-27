#!/usr/bin/env bash
# Python-side checks of the map/building generators (no Unreal needed): syntax, unit tests, Unreal-source static checks, stub check.
# Usage: bash Tools/RunToolChecks.sh        (run via bash: NTFS worktrees keep no executable bits)
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/../Unreal/SupernaturalProt"
export PYTHONDONTWRITEBYTECODE=1
echo "=== syntax";       python3 - <<'PY'
import glob, sys
for f in sorted(glob.glob("Tools/*.py")):
    compile(open(f, encoding="utf-8").read(), f, "exec")
print("syntax ok")
PY
echo "=== unit tests";   python3 -m unittest discover -s Tools -p "test_spn_*.py"
echo "=== stub check";   bash Tools/stubcheck/run_stub_check.sh
echo "=== benchmark";    python3 Tools/benchmark_generator.py 3
