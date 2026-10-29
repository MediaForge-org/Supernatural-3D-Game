# M2B3.1 — Windows validation of the portable Core (and the real Unreal build)

Status words: **Implemented / Tested / Proposed / Unverified**. Date: 2026-10-29. Repository `da1af14` (branch `main`) plus the uncommitted M2B3.1 changes (listed in §7). Nothing was committed, pushed, stashed or applied; `stash@{0}` (older Windows M2A3/M2A4 work) is untouched. The Fab plugin stays isolated (`Engine\Plugins\Fab` absent; moved earlier to `C:\Users\Aquaman\Documents\SPN-Backups\Fab-isolated\Fab`); the installed engine was not modified.
Logs of every run: `Docs/M2B3/logs/`.

## 1. Toolchain (as reported by the tools)

| Item | Value |
|---|---|
| OS | Windows 11 Pro 10.0.26200, Ryzen 7 3700X, RTX 3090, 32 GB |
| MSVC (primary) | `cl` **19.51.36260** for x64 (toolset dir `14.51.36231`) in Visual Studio "18" Community; a second toolset `14.44.35207` is installed but was **not** used |
| clang-cl (additional) | clang **22.1.3** (`x86_64-pc-windows-msvc`, VS-bundled LLVM) |
| CMake / Ninja | 4.3.1-msvc1 / 1.13.2 (both from VS); generator Ninja |
| Windows SDK | `10.0.26100.0` present (also `10.0.22621.0`); the one picked by `vcvars64` was not printed |
| Project options | `/W4 /permissive- /WX` (`SPN_WARNINGS_AS_ERRORS=ON`), `-std:c++20`, Debug = `/Od /RTC1 /MDd /Zi`, Release = CMake default `/O2 /Ob2 /DNDEBUG` |
| Python | `python3.15.exe` 3.15.2 under `C:\Users\Aquaman\.local\bin` (the `python` on PATH is the Store stub and cannot run; Unreal's bundled Python also works) |

Build trees are outside the repository: `C:\Users\Aquaman\spn-build\<name>`. The script used is a 15-line `cmd` wrapper (vcvars64 → `cmake -S … -G Ninja -DCMAKE_BUILD_TYPE=<cfg> -DSPN_WARNINGS_AS_ERRORS=ON -DCMAKE_CXX_COMPILER=<cl|clang-cl>` → `cmake --build -j 8 -- -k 0` → `ctest -j 4`); the exact command line is in `SAVE_WINDOWS_VALIDATION.md` §4.

## 2. Result of the first Windows build — the Core did **not** compile under MSVC

Fedora results do not transfer: the first fresh MSVC Debug build (`logs/msvc-debug-run1.log`, `-k 0` to see all errors) failed with 7 diagnostics in 4 files:

| File | Diagnostic | Cause | Fix (behaviour unchanged) |
|---|---|---|---|
| `Core/src/Inventory.cpp:23-24` | C3861 `__builtin_add_overflow`, `__builtin_mul_overflow` | GCC/Clang builtins | portable checked add / multiply (`out = a + b; return out >= a`; `b > max / a`) — the existing overflow tests (`Capacity: weight arithmetic that overflows …`) pass unchanged |
| `Core/src/SaveStore.cpp:34` | C4996 `fopen` (an error under `/WX`) | CRT deprecation | `_wfopen_s` with the native wide path |
| `Tests/Core/BenchInventoryTx.cpp:31,37` | C3861 `malloc_usable_size` | glibc only | `_msize` on Windows |
| `Tests/Core/InventoryTxTests.cpp:188` | C4996 `getenv` | CRT deprecation | `_dupenv_s` helper |

Also changed for correctness on Windows (found by reading, then tested): ANSI `path.string()` → wide/UTF-8 paths and `MoveFileExW(WRITE_THROUGH)` in `SaveStore.cpp` (details: `SAVE_WINDOWS_VALIDATION.md` §1). No implementation file outside `Inventory.cpp` and `SaveStore.cpp` changed.

## 3. Final results — four clean configurations

Each from an empty build directory; `-Werror`-equivalent `/WX`; **0 compiler warnings** in all four (a grep of the logs finds none; the only CMake notice is "CMAKE_C_COMPILER not used", the project is C++ only).

| Configuration | Build steps | Wall time incl. configure + CTest | CTest | CTest time | Whole `spn_core_tests.exe` in one process |
|---|---|---|---|---|---|
| MSVC 19.51 Debug | 80/80 | 41 s | **480 of 480 passed** | 10.8 s | 478 cases, 911 046 assertions, all pass |
| MSVC 19.51 Release | 80/80 | 46 s | **480 of 480 passed** | 4.3 s | 478 cases, 911 046 assertions, all pass |
| clang-cl 22.1.3 Debug | 80/80 | 38 s | **480 of 480 passed** | 8.3 s | 478 cases, 911 046 assertions, all pass |
| clang-cl 22.1.3 Release | 80/80 | 37 s | **480 of 480 passed** | 4.2 s | 478 cases, 911 046 assertions, all pass |

Logs: `clean-msvc-debug.log`, `clean-msvc-release.log`, `clean-clangcl-debug.log`, `clean-clangcl-release.log`. No test failed; nothing was skipped or excluded.

CTest entries 480 = 478 doctest cases + `movement_test_map_layout` (Python) + `spn_core_test_names` (guard against `;` in case names). Fedora counted 478 because it also registers `unreal_stub_check`; that test is `if(UNIX)` by design (bash + stand-in headers) and does not exist on Windows, **the real Unreal build below replaces it**. The 478 doctest cases are the 475 of Fedora plus 3 new Windows save tests.

Coverage by area (doctest cases per source file, MSVC Release; counts overlap where one file name matches several patterns):

| Area | Cases | Result |
|---|---|---|
| Coordinates (`CoordinateTests`) | 9 | pass |
| World map (`MapTests`) | 52 | pass |
| Minimap (`MinimapTests`) | 23 | pass |
| Inventory (`InventoryTests`, `InventoryTxTests`) | 35 | pass |
| Navigation (`NavigationTests`) | 9 | pass |
| SaveSlot + SaveStore (`SaveSlotTests`) | 28 (3 new) | pass |
| GameSave (`GameSaveTests`) | 9 | pass |
| Movement / locomotion / jump / ground steering | 17 / 5 / 11 / 8 | pass |
| Interaction (reach, system, objects, doors) | 47 | pass |
| WorldFacts | 55 | pass |

The ground-steering test whose tolerance direction was corrected in M2B2.1 (`GroundSteeringTests.cpp:239`), the three formerly failing M2B2 tests and the door state matrix pass on MSVC and clang-cl too.

## 4. Real Unreal Editor build — **Succeeded**

`Build.bat SupernaturalProtEditor Win64 Development -Project=…\SupernaturalProt.uproject -WaitMutex -NoHotReload`, Unreal Engine 5.8.4 (CL 58902052), Fab isolated, engine not modified.

| Build | Log | Result |
|---|---|---|
| after the sync (13 Core files compiled by Unreal's toolchain: Coordinates, Minimap, Navigation, WorldMap, SpatialGrid, SaveSlot, SaveStore, GameSave, Inventory, InventoryPersistence, Appearance*, TestDoor) | `unreal-editor-build-1.log` | **Succeeded**, 16 actions, 13.0 s |
| + `SpnWorldFrame` adapter and tests | `unreal-editor-build-2.log` | failed once: `EAutomationTestFlags::Type` is not a type in 5.8 (my test code); fixed |
| fixed | `unreal-editor-build-3/4/5.log` | **Succeeded** (incremental, ≈ 7 s each) |

No compiler warning or error remained. This is a *different* verification than the Fedora stub check: the stand-in headers were not used.

## 5. Unreal automation and QA — existing behaviour is intact

All with `Tools/run_gameplay_qa.ps1 -NoVideo`, Fab still isolated.

| Run | Command | Result |
|---|---|---|
| V2 `Full -Regression` before the adapter (`20261029-161453`) | `-Suite Full -Regression` | QA **PASS 166, FAIL 2, BLOCKED 0, NOT_IMPLEMENTED 3, NOT_TESTED 1** |
| V2 `Full -Regression` final (`20261029-164836`) | same | **identical** QA numbers; automation **54 tests, 51 success, 3 failed**; the 5 new `SpnCore.WorldFrame.*` tests are among the successes |
| V3 Doors (`163929`) | `-Suite Doors -Map …V3Interiors` | **89/89 PASS** |
| V3 Access (`164229`) | `-Suite Access` | **15/15 PASS** |
| V3 Traversal (`164351`) | `-Suite Traversal` | **22/22 PASS** |
| V3 MovementDemo (`164613`) | `-Suite MovementDemo` | **27/27 PASS** |
| `SpnCore.WorldFrame` alone | `UnrealEditor-Cmd … Automation RunTests SpnCore.WorldFrame` | 5/5 success (`unreal-worldframe-tests.log`) |
| Python (`python3.15 -I -m unittest discover -s Tools -p "test_*.py"`) | in `Unreal/SupernaturalProt` | **219 tests OK** |
| QA inventory gate | `check_qa_inventory.py --check` | 23 systems, 0 problems |

**Failures that exist and are not new** (identical to the M2A4.1 report on `ed87970`, i.e. before M2B3.1): 2 QA checks `Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run` (→ `SpnQA.Jump`), and `SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps` (30 cm). Unchanged by this milestone; open items for the movement track (M2A4.2).

## 6. Environment findings

* `python` on the PATH is the Microsoft Store stub (exit with an install message). Tools that need Python (`movement_test_map_layout` via CMake) found `python3.15.exe`; scripts that call `python` fail. Proposal: document `py`/`python3` use or disable the alias.
* The Core presets in `CMakePresets.json` name `clang++`/`g++` and have no Windows preset (MSVC needs `vcvars64`). Proposal for a later step: add `windows-msvc` / `windows-clangcl` presets (not done: it changes shared build files).
* `Docs/Setup/Windows-M2A-Handover.md` is referenced by the build files but was not re-read.
* `Unreal/SupernaturalProt/Tools/__pycache__/` is created by running the Python tests and is untracked (it was untracked before this milestone too).

## 7. Files changed or added in M2B3.1

Modified: `Core/src/Inventory.cpp`, `Core/src/SaveStore.cpp`, `Tests/Core/BenchInventoryTx.cpp`, `Tests/Core/InventoryTxTests.cpp`, `Tests/Core/SaveSlotTests.cpp`, `Tests/Core/CMakeLists.txt`, `Unreal/SupernaturalProt/Tools/qa_inventory.json`, `Docs/Development/Gameplay-QA-Inventory.md` (regenerated).
Added: `Tests/Core/SaveKillTest.cpp`, `Unreal/SupernaturalProt/Source/SpnCore/Public/SpnWorldFrame.h`, `Private/SpnWorldFrame.cpp`, `Private/Tests/SpnWorldFrameAutomation.cpp`, `Docs/M2B3/*.md`, `Docs/M2B3/logs/*`.
Not touched: maps/assets, controller mappings, WorldForge, Planning/Master, the Git stash, the installed engine.
