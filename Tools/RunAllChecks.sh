#!/usr/bin/env bash
# Configure, build and test every preset (max. 4 parallel jobs, see CMakePresets.json).
# Usage: [SPN_PRESET_PREFIX=fedora-] bash Tools/RunAllChecks.sh [preset...]
# Run via `bash`: an NTFS-backed worktree does not keep executable bits. For the
# same reason, build outputs cannot be executed there; use the local
# (gitignored) CMakeUserPresets.json presets, selected with SPN_PRESET_PREFIX.
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."

presets=("$@")
if [ ${#presets[@]} -eq 0 ]; then
    prefix="${SPN_PRESET_PREFIX:-}"
    presets=("${prefix}clang" "${prefix}gcc" "${prefix}clang-asan-ubsan" "${prefix}gcc-asan-ubsan")
fi

for preset in "${presets[@]}"; do
    echo "=== ${preset}: configure"
    cmake --preset "${preset}"
    echo "=== ${preset}: build"
    cmake --build --preset "${preset}"
    echo "=== ${preset}: test"
    ctest --preset "${preset}"
done
