#!/usr/bin/env bash
# Rebuild the question-answer matrix modules (Docs/Game/Adult/36-40) from the planning master.
# Read-only on the master; writes only into WORK_DIR (default: a temporary directory).
#
# Usage: SPN_MASTER=/path/to/SPN_FULL_CUMULATIVE_MASTER_2026-09-27 bash Tools/adult-audit/run.sh [WORK_DIR]
# Needs: python3. Nothing is copied into the repository; compare ./stage with Docs/Game/Adult.
set -euo pipefail

: "${SPN_MASTER:?set SPN_MASTER to the cumulative master directory}"
repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
work="${1:-$(mktemp -d)}"
export SPN_ADULT_DOCS="$repo/Docs/Game/Adult"
tools="$repo/Tools/adult-audit"

mkdir -p "$work"
cd "$work"
python3 -I "$tools/split_chats.py" "$SPN_MASTER" "$work"
# The scripts import each other's JSON files from the current directory.
python3 "$tools/parse_packages.py"
python3 "$tools/overrides_and_register_map.py"
python3 "$tools/render_rows.py"
python3 "$tools/write_markdown.py"
echo "Matrix modules written to $work/stage"
