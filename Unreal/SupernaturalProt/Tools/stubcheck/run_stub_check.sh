#!/usr/bin/env bash
# Syntax check of the Unreal-side movement code against a hand-written stand-in for the engine (see include/CoreMinimal.h).
# Proves only that these files agree with each other; it is NOT an Unreal build and cannot find a wrong engine API.
# Usage: bash Unreal/SupernaturalProt/Tools/stubcheck/run_stub_check.sh   (needs clang++ or g++)
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
project="$(cd "$here/../.." && pwd)"
repo="$(cd "$project/../.." && pwd)"
cxx="${CXX:-$(command -v clang++ || command -v g++)}"
flags=(-std=c++20 -fsyntax-only -Wall -Wextra -Wshadow -Wno-unused-parameter -Werror
       -I "$here/include"
       -I "$project/Source/SpnCore/Public"
       -I "$project/Source/SupernaturalProt"
       -I "$repo/Core/include")
for source in \
    "$project/Source/SpnCore/Private/SpnMovement.cpp" \
    "$project/Source/SupernaturalProt/Spn/SpnCharacterMovementComponent.cpp" \
    "$project/Source/SupernaturalProt/Spn/SpnPlayAreaGuard.cpp"; do
    echo "stub check: ${source#$project/}"
    "$cxx" "${flags[@]}" "$source"
done
echo "stub check: OK (stand-in headers, not the engine)"
