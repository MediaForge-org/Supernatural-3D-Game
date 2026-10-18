# 0001 — Phase 0.1: Core foundation (engineering notes)

Status: engineering notes, **not** master canon. Nothing here ratifies or changes
any OPEN/PROVISIONAL canon item (world size, map, Unreal version, network
architecture, gameplay/driving numbers all stay open).

## Developer decisions (2026-10-18)
- C++20, CMake, Ninja. `Core/` is independent of Unreal Engine.
- Test framework: **doctest** (MIT), fetched via CMake `FetchContent`, pinned to
  the commit of release v2.4.12 (`1da23a3e8119ec5cce4f9388e91b065e20bf06f5`).
  (Catch2 was approved first, then replaced by doctest before it was integrated.)
- `StrongId<Tag>` first; `WorldFacts` only after StrongId is complete.

## Engineering choices made while implementing (reviewable)
- Value-initialized `StrongId` (0) is the null id; no other value has meaning.
- Naming: types/functions PascalCase, variables camelCase, private members `name_`.
  Public Core interfaces avoid exceptions, RTTI and iostream (Unreal-friendly).
- doctest's own CMakeLists is not used (old minimum-version policy vs. CMake 4);
  only its header and `doctest.cmake` are consumed. Its `-Wno-*` flags apply to
  the test executables only; Core keeps the full warning set (`-Werror` in presets).
- Build directories must live outside the NTFS worktree: that mount gives files no
  exec bit, so test binaries cannot run there. Local `CMakeUserPresets.json`
  (gitignored) points builds to `$HOME/.cache/spn-build/<preset>`.

## Sanitizer runtimes (Fedora 44)
- The `*-asan-ubsan` presets need `libasan` and `libubsan` (GCC) and
  `compiler-rt` (Clang), installed from the official `fedora`/`updates` repos
  at the versions matching the installed compilers.
