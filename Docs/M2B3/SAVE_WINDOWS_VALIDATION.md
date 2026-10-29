# M2B3.1 — Save system on Windows: validation

Status words: **Implemented / Tested / Proposed / Unverified** (as in `Docs/M2B2/WORLD_COORDINATE_CONVENTIONS.md`).
All runs: 2026-10-29, Windows 11, local NTFS volume `C:` (fixed drive), MSVC 19.51 / clang-cl 22.1.3, repository state `da1af14` plus the uncommitted M2B3.1 changes.
Raw logs: `Docs/M2B3/logs/`. Nothing was run against project assets or user save files; every test used a temporary directory (`%TEMP%` or `C:\Users\Aquaman\spn-save-test\…`).

## 1. What the Windows implementation does (read from the code)

`Core/src/SaveStore.cpp`, `StdSaveFileSystem` (the production file system used by `SaveSlotStore`):

| Step | Windows implementation | Status |
|---|---|---|
| write a temporary file | `_wfopen_s` (wide path) → `fwrite` → `fflush` → `_commit(_fileno)` → `fclose` | Implemented, Tested |
| read back / verify | `std::ifstream` (wide path), compared with what was written | Tested |
| keep previous generation | previous good file is rewritten to `.prev.tmp` and renamed to `.prev` | Tested |
| publish | **`MoveFileExW(MOVEFILE_REPLACE_EXISTING \| MOVEFILE_WRITE_THROUGH)`** | Implemented (changed in M2B3.1), Tested for replacement and error reporting; durability **Unverified** |
| directory flush | **no-op** (Windows has no directory fsync); `MOVEFILE_WRITE_THROUGH` is the substitute | Unverified |
| load | current file, else previous generation; strict parse + per-section checksum | Tested |

Changes made in M2B3.1 (all were needed by a real Windows build or by the tests below):

* `fopen` → `_wfopen_s` with the native wide path. The old `path.string()` goes through the ANSI code page and breaks for names such as `Spielstände` or Japanese characters, and `fopen` is a hard error under `/W4 /WX` (C4996).
* Rename: `std::filesystem::rename` → `MoveFileExW(... WRITE_THROUGH)` (Windows only). Reason: the standard call gives no statement about write-through; the Win32 call documents that it returns after the move reached the disk (for the file system's own rules).
* Paths in error messages and the `.prev`/`.tmp` names are built from `path` objects (`Suffixed`, `Display` via UTF-8) instead of `path.string()`.
* `DescribeFilesystem` now reports the volume on Windows (`NTFS on C:\ (fixed drive)`).

## 2. Results

### 2.1 Unit and integration tests (CTest, doctest) — **all pass**

480 CTest entries, 478 doctest cases (3 new Windows cases listed below), MSVC Debug/Release and clang-cl Debug/Release all green; details in `WINDOWS_CORE_VALIDATION.md`. Save-related cases that ran on Windows: container format, every byte flip and prefix rejected, fault injection at 64 interruption points (simulated), recovery after a torn write, real-file-system round trip, publish primitives on the temp volume.

New in M2B3.1 (`Tests/Core/SaveSlotTests.cpp`), real file system:

| Test | Result |
|---|---|
| non-ASCII directory and file names (`Spielstände 日本 Übergänge/slot äöü.sav`) round trip, exactly two files remain | pass |
| path > 300 characters (`\\?\` extended-length prefix, 6 × 60-character directories) | pass |
| (Windows only) a save file held open by another handle without `FILE_SHARE_DELETE`: the new save **fails with `Io`**, the previous generation is still loadable, the temporary file is removed, and saving works again after release | pass |

Not covered: paths longer than 260 characters **without** the `\\?\` prefix (depends on the machine's `LongPathsEnabled` setting, not checked), network and removable volumes, OneDrive-redirected folders.

### 2.2 `spn_core_save_probe` on the real volume

```
directory:                C:\Users\Aquaman\spn-save-test\probe
filesystem:               NTFS on C:\ (fixed drive)
rename replaces existing: yes
file flush reported ok:   yes
directory flush ok:       yes        <- a no-op on Windows; "yes" proves nothing here
notes:                    primitives behaved as required; this cannot prove power-loss safety (a flush cannot be observed)
```

### 2.3 Process-kill test (`spn_core_save_kill_test`, new, hand-run)

`Tests/Core/SaveKillTest.cpp` (Windows only, not registered with CTest). A parent starts a child of the same executable that publishes generation after generation (two sections, ~290 KB per slot), terminates it with **`TerminateProcess`** after a random 0…N ms and loads the slot with the production `SaveSlotStore`. After every kill it checks: the store loads (never "no valid save" once a save existed), the loaded slot is *exactly* the content of some generation (no partial or mixed slot), and the generation is not older than the last one the child reported complete or the parent loaded earlier.

Final runs (Release build, NTFS `C:`), logs `save-kill-final-*.log`:

| Run | Kills | Max delay | Highest generation | Loaded from current / previous | Kills that left temp files | Violations |
|---|---|---|---|---|---|---|
| seed 21 | 500 | 120 ms | 1073 | 499 / 0 (1 kill before the first save) | 419 | **0** |
| seed 22 | 500 | 30 ms | 51 | 489 / 0 (11 before the first save) | 473 | **0** |

Earlier runs of the same tool: `save-kill-run-11…14.log` (500/500/500/300 kills, seeds 11–14): 0 violations; plus 1 200 kills with the first version of the tool (`save-kill-run1…3.log`). **The first version reported 4 violations; all were defects of the test's progress file** (it was truncated and rewritten, so a kill could leave it empty or stale and the check compared against a wrong value), not store defects: in each case the loaded slot was complete and newer than expected. The progress file is now append-only and the rule is "never older"; the old logs are kept for transparency.

Total with the final invariant (runs 11–14 and 21–22): 2 800 kills, **0 violations**, 2 234 of them left temporary files behind, i.e. interrupted the writer mid-save (so the interrupted-write and recovery paths were really hit), **the previous-generation fallback was never needed** (always loaded from the current file: NTFS rename is atomic, so a killed process never leaves a broken current file).

**What this does and does not show**

* It shows: process termination at arbitrary moments (including during `WriteSynced`, the read-back, the `.prev` copy and the rename) never produced an unloadable, partial or older-than-reported slot, and leftover temp files did not disturb the next save.
* It does **not** show crash safety against power loss, blue screens or a disk that reorders writes. `TerminateProcess` leaves everything already handed to the OS intact, so a missing physical flush is invisible. The directory flush is a no-op. **Do not claim power-loss safety from this evidence.** A real test would need a VM with disk write-cache control or a power cut.
* Recovery from a damaged current file (previous generation fallback) is covered by the existing unit tests, not by the kill test.

## 3. Findings and risks

| # | Finding | Severity | Status |
|---|---|---|---|
| S1 | `fopen`, `__builtin_*_overflow` (Inventory), `malloc_usable_size` (benchmark), `getenv` (test) did not compile or warned under MSVC | build break | fixed (portable code, no behaviour change) |
| S2 | ANSI `path.string()` for file names | data loss on non-ASCII names (a save would silently use a different name or fail) | fixed, tested |
| S3 | Directory flush is a no-op on Windows | durability gap | mitigated by `WRITE_THROUGH`, **unverified** |
| S4 | `MoveFileExW` fails while an antivirus/indexer holds the destination open without delete sharing | a save attempt can fail | reported as `Io`, old save kept (tested); the game should retry a few times and tell the player — **no retry logic exists yet** |
| S5 | Windows `windows.h` is included by a Core `.cpp` that is also compiled by Unreal's build | macro clashes | the real Unreal build compiled it without issue; Core TUs include no Unreal headers (see `UNREAL_INTEGRATION_PLAN.md`) |
| S6 | The game's runtime still uses the placeholder world-facts save (`SpnWorldStateSubsystem`), not this store | integration | open, planned |

## 4. Reproduce

```powershell
# Core presets use VS 2026 (VS "18") MSVC/clang-cl; build tree outside the repository
$vs = 'C:\Program Files\Microsoft Visual Studio\18\Community'
cmd /c "call `"$vs\VC\Auxiliary\Build\vcvars64.bat`" && cmake -S . -B C:\Users\Aquaman\spn-build\msvc-release -G Ninja -DCMAKE_BUILD_TYPE=Release -DSPN_WARNINGS_AS_ERRORS=ON -DCMAKE_CXX_COMPILER=cl && cmake --build C:\Users\Aquaman\spn-build\msvc-release && ctest --test-dir C:\Users\Aquaman\spn-build\msvc-release"
$b = 'C:\Users\Aquaman\spn-build\msvc-release\Tests\Core'
& "$b\spn_core_save_probe.exe" C:\Users\Aquaman\spn-save-test\probe
& "$b\spn_core_save_kill_test.exe" run C:\Users\Aquaman\spn-save-test\kill 500 21 120    # directory, kills, seed, max delay in ms
```
