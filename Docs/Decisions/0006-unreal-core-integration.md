# 0006 — Unreal integration of the C++20 core (engineering notes)

Status: engineering notes, **not** master canon. First working integration,
2026-10-19, UE 5.8.3 (launcher build), MSVC 14.51, Windows 11. Reviewable.

## Windows/MSVC build fix
- Cause: doctest v2.4.12 forward-declares `std::basic_ostream` unless
  `DOCTEST_CONFIG_USE_STD_HEADERS` is set. Stringifying `std::string_view`
  instantiates MSVC's `operator<<` (`__msvc_string_view.hpp`) on that incomplete
  type → C2027. libc++ already forces the option; libstdc++ happened to work.
- Fix: `DOCTEST_CONFIG_USE_STD_HEADERS` on the `spn_doctest` interface target
  (tests only; Core flags unchanged).
- `/EHsc` was never missing: CMake's MSVC default `CMAKE_CXX_FLAGS` is
  `/DWIN32 /D_WINDOWS /EHsc`. Passing `-DCMAKE_CXX_FLAGS=/FIostream` replaced
  that default, which is what removed `/EHsc`.
- Windows MAX_PATH: cloning doctest fails in deep build directories (long
  example file names). Keep build dirs short, e.g. `build/<name>` in the repo.

## Integration approach
- Unreal module `SpnCore` (in the Unreal project, `Source/SpnCore/`) compiles
  `Core/src/*.cpp` **directly from this repository** via UBT's
  `ModuleRules.ConditionalAddModuleDirectory`. Nothing is copied.
- Reason: UE uses `/MD` (release CRT) in every configuration and
  `_HAS_EXCEPTIONS=0` in game builds. A CMake-prebuilt `spn_core.lib` (`/EHsc`,
  `_HAS_EXCEPTIONS=1`, debug CRT in Debug) would mix incompatible settings.
  Compiling with UBT gives each UE configuration matching flags automatically.
- Core path: env var `SPN_CORE_ROOT`, else the repository that contains the
  project (`<repo>/Unreal/SupernaturalProt`, layout since commit `cd7d0da`), else a
  sibling checkout `../Supernatural-3D-Game` next to the Unreal project. No
  absolute paths. (2026-10-20: the in-repo case was added; before it, the in-repo
  project failed with a RulesError unless `SPN_CORE_ROOT` was set.)
- Core TUs: no PCH, no unity build (each .cpp stays its own TU, as in CMake).
- Core headers are a *private* include path of `SpnCore`. In modular (editor)
  builds `SpnCore` is a DLL and Core symbols are not exported, so other modules
  use the exported bridge functions (`SPNCORE_API`), not Core types directly.

## World state subsystem (2026-10-20, Proposal 0004 increment 3)
- `USpnWorldStateSubsystem` (game instance subsystem, module `SpnCore`) owns one
  `WorldFacts` store and the `InteractionRegistry` (test door registered). Core types sit
  behind a `TPimplPtr`, so the public header uses Unreal types only (bridge, see open
  point below). `SpnCore` now also depends on `CoreUObject` and `Engine`.
- Slot file (placeholder for M5): `Saved/SpnSaves/<slot>/worldfacts.spnfacts`, WorldFacts
  v1 bytes unchanged. Placeholder slot rule: 1..64 chars, `[a-z][a-z0-9_]*` (no dots: Windows
  drops trailing dots; no separators: no path escape).
- Save: write `<file>.tmp`, full flush, read back and compare, then one atomic rename over
  the old file (`MoveFileExW(REPLACE_EXISTING | WRITE_THROUGH)` on Windows, `rename()` via
  `IPlatformFile::MoveFile` elsewhere). `IFileManager::Move(Replace)` is not used: it deletes
  the target before moving. A failure before the rename leaves the old save untouched.
- Load: missing slot, unreadable file or a `Deserialize` error returns an error status
  (core error text and line) and keeps the current store; only success replaces it and
  broadcasts `OnWorldStateReplaced`.
- Known warnings: `SpnCore` builds without PCH, so each TU including `UObject/Class.h`
  shows 7 C4996 warnings from that UE 5.8.3 header (`UE_INTERNAL`/deprecated overrides in
  `UClass`). The project's shared PCH compiles report the same 7; not from SPN code.

## Open (needs developer decision)
- Direct use of Core types from gameplay modules: either keep a bridge layer,
  or add an opt-in export macro to Core headers (empty in CMake). Not decided.
- The Unreal project is not under version control yet; its location relative
  to this repo (currently a sibling directory) is not decided.
- Linux UE build of `SpnCore` untested; Linux CMake build of the doctest change
  not run in this session (no Linux/WSL available on the Windows machine).
