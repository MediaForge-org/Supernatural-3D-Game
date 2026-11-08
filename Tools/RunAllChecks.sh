#!/usr/bin/env bash
# Configure, build and test every preset (max. 4 parallel jobs, see CMakePresets.json).
# Usage: [SPN_PRESET_PREFIX=fedora-] [SPN_QA_REPORT=path] bash Tools/RunAllChecks.sh [preset...]
#
# Exit codes: 0 = every preset configured, built and passed all tests;
#             1 = at least one preset failed (see the report and the log lines "FAIL <preset> <step>").
# Report: a plain-text summary per preset (default build/qa-report.txt, build/ is gitignored).
# Run via `bash`: an NTFS-backed worktree does not keep executable bits. For the
# same reason, build outputs cannot be executed there; use the local
# (gitignored) CMakeUserPresets.json presets, selected with SPN_PRESET_PREFIX.
set -uo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."

presets=("$@")
if [ ${#presets[@]} -eq 0 ]; then
    prefix="${SPN_PRESET_PREFIX:-}"
    presets=("${prefix}clang" "${prefix}gcc" "${prefix}clang-asan-ubsan" "${prefix}gcc-asan-ubsan")
fi
report="${SPN_QA_REPORT:-build/qa-report.txt}"
mkdir -p "$(dirname "$report")"
{
    echo "SPN QA report: $(date -Iseconds)"
    echo "host: $(uname -srm)"
} > "$report"

failed=0
run_step() { # run_step <preset> <step> <command...>
    local preset="$1" step="$2"; shift 2
    if "$@"; then
        echo "PASS ${preset} ${step}" | tee -a "$report"
    else
        echo "FAIL ${preset} ${step} (exit $?)" | tee -a "$report"
        failed=1
        return 1
    fi
}

for preset in "${presets[@]}"; do
    echo "=== ${preset}"
    run_step "$preset" configure cmake --preset "${preset}" || continue
    run_step "$preset" build cmake --build --preset "${preset}" || continue
    # ctest's own summary line is kept in the report for later comparison.
    if out=$(ctest --preset "${preset}" 2>&1); then
        echo "$out" | tail -n 3
        echo "PASS ${preset} test: $(echo "$out" | grep -o '[0-9]* tests passed, [0-9]* tests failed out of [0-9]*' | tail -n 1)" | tee -a "$report"
    else
        echo "$out" | tail -n 60
        echo "FAIL ${preset} test: $(echo "$out" | grep -o '[0-9]* tests passed, [0-9]* tests failed out of [0-9]*' | tail -n 1)" | tee -a "$report"
        failed=1
    fi
done

if [ "$failed" -eq 0 ]; then
    echo "RESULT: all presets passed (report: ${report})" | tee -a "$report"
    exit 0
fi
echo "RESULT: failures found (report: ${report})" | tee -a "$report"
exit 1
