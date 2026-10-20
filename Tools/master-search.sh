#!/usr/bin/env bash
# Read-only, tiered text search in the external planning master (Planning/Master).
#
# A hit is a pointer, not canon: read the section and its status words
# (LOCKED / CONFIRMED / PROVISIONAL / PROPOSAL / OPEN) before relying on it.
# See Docs/MasterIndex.md and the "Master consultation protocol" in CLAUDE.md.
#
# Usage: Tools/master-search.sh [options] TERM [TERM...]
#   Several TERMs are alternatives (OR). Terms are literal text by default, so
#   spaces and characters such as . ( ) [ ] * ? are safe; use --regex for patterns.
#
# Options:
#   --tier LIST     tiers to search, comma separated (default: 1,2)
#                     1  01_CURRENT_CANONICAL_STATE        canon candidates, addenda
#                     2  99_DAILY_DELTAS, 99_CANON_AUDITS  review packages (proposals/evidence)
#                     3  current prompt library docs       older reference, DRAFT prompts
#   --history       also search tier 4 (history, reconstruction, source archives)
#   --all           tiers 1-4
#   --regex         treat TERMs as regular expressions
#   --case          case-sensitive (default: case-insensitive)
#   --count         only list files with the number of matching lines
#   --limit N       max output lines per tier (default 60; 0 = unlimited)
#   -C N            context lines around each hit (default 0)
#   -h, --help      this text
#
# Master location: $SPN_MASTER_DIR if set, else <repo>/Planning/Master (found relative
# to this script). Output paths are relative to the cumulative master directory
# (SPN_FULL_CUMULATIVE_MASTER_*), as written in Docs/ source references.
set -euo pipefail

usage() { sed -n '2,/^set -euo/p' "$0" | sed '$d' | sed 's/^# \{0,1\}//'; }

tiers="1,2"
regex=0
case_sensitive=0
count_only=0
limit=60
context=0
terms=()

while [ $# -gt 0 ]; do
    case "$1" in
        --tier)    [ $# -ge 2 ] || { echo "error: --tier needs a list" >&2; exit 2; }
                   tiers="$2"; shift 2 ;;
        --history) tiers="${tiers},4"; shift ;;
        --all)     tiers="1,2,3,4"; shift ;;
        --regex)   regex=1; shift ;;
        --case)    case_sensitive=1; shift ;;
        --count)   count_only=1; shift ;;
        --limit)   [ $# -ge 2 ] || { echo "error: --limit needs a number" >&2; exit 2; }
                   limit="$2"; shift 2 ;;
        -C)        [ $# -ge 2 ] || { echo "error: -C needs a number" >&2; exit 2; }
                   context="$2"; shift 2 ;;
        -h|--help) usage; exit 0 ;;
        --)        shift; terms+=("$@"); break ;;
        -*)        echo "error: unknown option '$1' (see --help)" >&2; exit 2 ;;
        *)         terms+=("$1"); shift ;;
    esac
done

[ ${#terms[@]} -gt 0 ] || { echo "error: no search term given (see --help)" >&2; exit 2; }
[[ "$limit" =~ ^[0-9]+$ && "$context" =~ ^[0-9]+$ ]] || { echo "error: --limit and -C need non-negative integers" >&2; exit 2; }
[[ "$tiers" =~ ^[1-4](,[1-4])*$ ]] || { echo "error: --tier takes values 1-4, e.g. 1,2,3" >&2; exit 2; }

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
master="${SPN_MASTER_DIR:-$script_dir/../Planning/Master}"

if [ ! -d "$master" ]; then
    {
        echo "error: planning master not available: '$master'"
        echo "  Planning/Master is a symlink to the external master archive and is not part of Git."
        echo "  Fedora: check 'ls -l Planning/Master' and that the target drive is mounted."
        echo "  Other machines: set SPN_MASTER_DIR to the archive directory (see Docs/MasterIndex.md)."
    } >&2
    exit 3
fi

root=""
for candidate in "$master"/SPN_FULL_CUMULATIVE_MASTER_*; do
    if [ -d "$candidate" ]; then root="$candidate"; break; fi
done
if [ -z "$root" ]; then
    # SPN_MASTER_DIR may already point at the cumulative directory.
    if [ -d "$master/01_CURRENT_CANONICAL_STATE" ]; then root="$master"; fi
fi
[ -n "$root" ] || { echo "error: no SPN_FULL_CUMULATIVE_MASTER_* directory found in '$master'" >&2; exit 3; }

tier_dirs() {
    case "$1" in
        1) echo "01_CURRENT_CANONICAL_STATE" ;;
        2) echo "99_DAILY_DELTAS 99_CANON_AUDITS" ;;
        3) echo "02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs" ;;
        4) echo "04_HISTORY 02_EMBEDDED_BASE_ARCHIVES/03_PROMPT_LIBRARY_HISTORY 99_RECONSTRUCTION_SOURCES _SOURCE_ARCHIVE 06_NEW_REFERENCES_2026-09-10 02_WORLD_700 03_EXTERNAL_LIBRARY_REGISTRY" ;;
    esac
}
tier_label() {
    case "$1" in
        1) echo "TIER 1 - current canonical state (check each section's status words)" ;;
        2) echo "TIER 2 - daily deltas and audits (review packages: proposals and evidence, not canon)" ;;
        3) echo "TIER 3 - older reference docs; prompts are DRAFT_REWRITE_REQUIRED, not canon (tier 1 wins on conflict)" ;;
        4) echo "TIER 4 - history and source archives (evidence only)" ;;
    esac
}

have_rg=0
command -v rg >/dev/null 2>&1 && have_rg=1
if [ "$have_rg" -eq 0 ] && ! command -v grep >/dev/null 2>&1; then
    echo "error: neither ripgrep (rg) nor grep found" >&2; exit 4
fi

run_search() { # $@ = existing directories relative to $root
    if [ "$have_rg" -eq 1 ]; then
        local args=(--no-heading --with-filename --color never --glob '*.md' --glob '!**/extracted_*/**')
        [ "$case_sensitive" -eq 1 ] || args+=(-i)
        [ "$regex" -eq 1 ] || args+=(-F)
        if [ "$count_only" -eq 1 ]; then args+=(--count); else args+=(-n -C "$context"); fi
        local t
        for t in "${terms[@]}"; do args+=(-e "$t"); done
        rg "${args[@]}" -- "$@"
    else
        local args=(-r -H -I --include='*.md' --exclude-dir='extracted_*')
        [ "$case_sensitive" -eq 1 ] || args+=(-i)
        [ "$regex" -eq 1 ] && args+=(-E) || args+=(-F)
        if [ "$count_only" -eq 1 ]; then args+=(-c); else args+=(-n -C "$context"); fi
        local t
        for t in "${terms[@]}"; do args+=(-e "$t"); done
        grep "${args[@]}" -- "$@" | { [ "$count_only" -eq 1 ] && grep -v ':0$' || cat; }
    fi
}

echo "master: ${root#"$script_dir"/../}"
echo "terms: ${terms[*]}  (alternatives, $([ "$regex" -eq 1 ] && echo regex || echo literal))"
cd "$root"
found_any=0
IFS=',' read -r -a tier_list <<< "$tiers"
for tier in "${tier_list[@]}"; do
    dirs=()
    for d in $(tier_dirs "$tier"); do
        [ -d "$d" ] && dirs+=("$d")
    done
    echo
    echo "=== $(tier_label "$tier")"
    if [ ${#dirs[@]} -eq 0 ]; then
        echo "(directories of this tier not present in this master)"
        continue
    fi
    out="$(run_search "${dirs[@]}" 2>/dev/null || true)"
    if [ -z "$out" ]; then
        echo "(no hits)"
        continue
    fi
    found_any=1
    total="$(printf '%s\n' "$out" | wc -l)"
    if [ "$limit" -gt 0 ] && [ "$total" -gt "$limit" ]; then
        printf '%s\n' "$out" | head -n "$limit"
        echo "... $((total - limit)) more lines (raise --limit, narrow the terms, or use --count)"
    else
        printf '%s\n' "$out"
    fi
done
[ "$found_any" -eq 1 ] || exit 1
