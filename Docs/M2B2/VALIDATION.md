# M2B2 — Validation

Status words (**Implemented / Tested / Proposed / Unverified**): see `WORLD_COORDINATE_CONVENTIONS.md`.
> **M2B2.1 update (2026-10-29):** the three failures that M2B2 left open were resolved; results in section 2 are from the final run, section 3 explains each root cause. Sections that describe the state *before* M2B2.1 say so.

Everything below was executed on 2026-10-29 on the Fedora 44 laptop (GCC 16.2.1, Clang 22.1.8, CMake + Ninja, doctest 2.4.12). Numbers are copied from the tool output, not estimated.

## 1. Method

* Presets `fedora-gcc`, `fedora-clang`, `fedora-gcc-asan-ubsan`, `fedora-clang-asan-ubsan` (`CMakeUserPresets.json`; build trees in `~/.cache/spn-build/` because the NTFS worktree has no exec bits). For each: build directory **deleted**, configure, full build with `-Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow`, `ctest`.
* Each of the 474 doctest cases was also run **by itself** with a correct filter (`runeach.py`, kept in the session scratch area, not in the repo) — this is what finally makes the isolation question answerable (section 3).
* Python tests: every `Unreal/SupernaturalProt/Tools/test_*.py` with `python3 -I`; `check_qa_inventory.py --check`; `stubcheck/run_stub_check.sh`.
* Benchmarks: `INVENTORY_PERFORMANCE.md`. Filesystem probe: `UNIFIED_SAVE_ARCHITECTURE.md` section 4.

## 2. Results (final run, after M2B2.1)

| Preset | Build | Compiler warnings | CTest | Sanitizer reports |
|---|---|---|---|---|
| `fedora-gcc` | OK (fresh) | 0 | **478 of 478 pass** | – |
| `fedora-clang` | OK (fresh) | 0 | **478 of 478 pass** | – |
| `fedora-gcc-asan-ubsan` | OK (fresh) | 0 | **478 of 478 pass** | **0** |
| `fedora-clang-asan-ubsan` | OK (fresh) | 0 | **478 of 478 pass** | **0** |

* Build directories were deleted before each preset; `-Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow`.
* Doctest cases: **475** (474 + the new door state-matrix case). CTest entries: 478 = 475 cases + `movement_test_map_layout`, `unreal_stub_check`, `spn_core_test_names` (all three pass).
* Each of the 475 cases was also run **by itself** with a correct filter in every preset: 0 fail, 0 matched nothing.
* **Whole binary in one process, no exclusions** (gcc and clang-asan-ubsan): 475 of 475 cases, 910,996 assertions, all pass. The single-process abort of M2B2 is gone.
* Python: **10 of 10** test files pass (`python3 -I`); QA inventory gate: 23 systems, 0 problems; test-name integrity guard `spn_core_test_names`: pass; unreal stub check: OK (stand-in headers — **not** an Unreal build).
* Sanitizer presets use `-fsanitize=address,undefined -fno-sanitize-recover=undefined`, so a UBSan finding would abort; none did, and no log contains `runtime error` / AddressSanitizer / LeakSanitizer text.
* Hand-run results (benchmarks, filesystem probe, fault injection) are unchanged from M2B2; fault injection is part of the doctest suite.
* *Before M2B2.1* the same run gave 474 of 477 with the 3 failures below.

## 3. The three formerly failing tests — root causes and resolution

All three were committed in `ed87970`/`5891caf` (M2A3/M2A4), failed deterministically alone and under both compilers, and had been hidden by a harness defect (next paragraph). None was a flaky or isolation problem.

**Harness defect (fixed in M2B2).** `doctest_discover_tests` (vendored `doctestAddTests.cmake`, line 52) splits the case list at `;`. 17 case names contained `;`, so CTest registered fragments that match **no** test case; doctest ran 0 cases and exited 0 — a green entry without an assertion. All three cases below had a `;` in their name, which is why they looked fine "under CTest" in M2B1. Fix: the 17 names were changed (`;` → ` -`, names only) and the CTest guard `spn_core_test_names` (`Tests/Core/CheckTestNames.cmake`) fails if a case name contains `;`. The guard caught four such names in M2B2's own new tests.

### 3.1 Door lock vs. open/closed state — implementation changed (decision of the developer)

* **Root cause:** `TestDoor.cpp` refused every use of a locked door, including *closing* an open one. That conflicts with the intended model (lock state and physical open/closed state are separate; the lock guards opening). The test at `InteractionSystemTests.cpp:247` already expected the intended behaviour.
* **Fix (one condition):** `if (door.locked && !door.open)` instead of `if (door.locked)`. A closed locked door still refuses with `placeholder:feedback.door.locked` and writes nothing; an open door closes whether locked or not and then stays locked, so it cannot be reopened. A disabled door is still refused in every state. No physically impossible transition is introduced (an open door cannot be "opened", a closed one cannot be "closed").
* **Tests:** new case "Door 3b" covers all four open/locked combinations (+ disabled in both open states, + closing a locked open door followed by a refused reopening). Two existing tests encoded the old rule and were updated *to the new rule*, not loosened: "Case 8" (refused exactly when `locked && !open`, previously `locked`) and "Case 10" (its refused example used a locked-default door whose fact made it open; it now uses a closed locked door `door_c`). All other door/persistence/environment tests pass unchanged (the closed-locked case of `EnvScenarioTests`, `DoorPersistenceTests`, `InteractionTests` "Door 3").
* **Not verified:** the Unreal side delegates the decision to the Core and its automation test only uses a closed locked door, so no change is expected there, but no Unreal test was run.

### 3.2 Interaction cone — test corrected, cone untouched

* **Root cause:** the test (`InteractionSystemTests.cpp:422`) expected 3 reachable targets while facing one of them. Reach is `angle − asin(radius / distance) ≤ maxAngleDegrees`, with `maxAngleDegrees = 60` and a default target radius of 0.3 m (`InteractionReach.cpp`). Facing west: west 0° → reachable; south (1 m, 17.5° wide) at 90° → 72.5° effective; east (0.5 m, 36.9° wide) at 180° → 143° effective. Only 1 target is inside the cone; the expectation `3` contradicted the configured cone. The follow-up `*SelectTarget(...)` on an empty optional was undefined behaviour (it aborted under libstdc++ assertions and killed the single-process run).
* **Fix:** the cone and the default radius were **not** changed. The test now asserts the real behaviour from several directions (west → `{west}`; south → `{east, south, west}` because the side targets are at 90° − 37° = 53° effective; diagonal → `{south, west}`; north → `{east, west}`, selection tie broken by id; east with east removed → nothing) and all dereferences go through a guarded helper that returns `"<none>"`. The later check "the far object does not leak" now expects `{east}`.

### 3.3 Ground steering (~0.205 m/s) — test comparison had the wrong direction; implementation correct

* **Investigation:** the check at `GroundSteeringTests.cpp:239` compares two runs of a test-local model of Unreal's friction blend: *real* (the sprint gait is dropped for a moment) and *kept* (sprint held). Measured with that model: real minimum 4.633, kept 4.838, difference −0.2047 m/s at the test's step 1/120 s. Step-size check (same code, time step 1/60, 1/120, 1/240, 1/1000 s): differences −0.194, −0.205, −0.201, −0.189 m/s — **converged; time step and numerical precision are not the cause.** The sprint is lost for ≈ 56 ms (all steps), during which the model brakes at `brakingDeceleration = 8 m/s²` to the Jog limit: physically plausible. The *real* minimum (4.63) lies inside the Windows measurement (4.61..4.64); the *kept* one (4.84) does not — so the sprint loss is part of reality, not an artefact.
* **Root cause:** the assertion `real.min − kept.min > −0.05` demands that the real run is *not more than 0.05 below* the kept run, i.e. `kept − real < 0.05`. The very next line of the same test allows `kept − real < 0.4`, so the two lines contradict each other; the 0.05 was never derived from the model. The meaningful sanity check in this direction is the reverse: dropping the sprint can only *lower* the minimum, so `real.min − kept.min` must not be positive.
* **Fix:** `CHECK(real.min − kept.min < 0.05)` (direction corrected, same 0.05 slack), plus a new lower bound `kept.min − real.min > 0.1` (the gait loss is not free), keeping the existing upper bound `< 0.4`. **No tolerance was increased**; the test got stricter (three bounds instead of two). No implementation file changed: the steering and gait code (`GroundSteering.cpp`, `Movement.cpp`) are untouched and consistent with the measurement.

## 4. Unreal stub check — fixed (stand-ins only)

Cause (confirmed by reading the files): **outdated hand-written stand-ins**, not a portability defect. `SpnCharacterMovementComponent.{h,cpp}` (M2A4) use `CalcVelocity`, `ComputeSlideVector`, `HandleSlopeBoosting`, `FHitResult`, `PerchRadiusThreshold`, `bUseFlatBaseForFloorChecks`, `bHasRequestedVelocity`, `HasAnimRootMotion`, `CurrentRootMotion.HasOverrideVelocity`, `MIN_TICK_TIME`, `ComputeAnalogInputModifier`, `FVector2D::GetSafeNormal/IsNearlyZero`; `CoreMinimal.h` of the stand-ins lacked all of them (14 compile errors). The Unreal sources are portable-code-correct; no Core target includes Unreal code.

Change: `Unreal/SupernaturalProt/Tools/stubcheck/include/CoreMinimal.h`, +15 lines of stand-in declarations, same caveat as the rest of that file ("what the author BELIEVES the engine has"). No production code was modified to satisfy the stubs. The check now passes — which proves only that the files agree with each other, **not** that the engine has these members with these signatures. A real Windows build is still required (section 7).

## 5. What was tested by what (M2B2 deliverables)

| deliverable | tests | key evidence |
|---|---|---|
| Coordinates (9 cases) | `CoordinateTests.cpp` | determinant −1 (right-handed ENU → left-handed UE), round trip < 1 µm at ±10,000 km, rejection of NaN/∞/limit+ε/1e300/1.7e308, origin handling, yaw for both presets, local frames, map/minimap wrappers |
| Container (≈ 16 cases) | `SaveSlotTests.cpp` | CRC check value, ids, determinism, independent golden, every prefix rejected, every byte flip rejected, 12+ header/length/checksum/order/duplicate/trailing/count cases, schema rules, 200 randomized slots with damage |
| Store + faults (≈ 9 cases) | `SaveSlotTests.cpp` | 64 interruption points × (old or new, never partial), recovery after torn writes, newer-build refusal, leftover temp files, read-back mismatch, directory-flush failure, real `StdSaveFileSystem` |
| Game adapters (9 cases) | `GameSaveTests.cpp` | embedded payloads are byte-identical to each domain's `Serialize()`, read-only capture, full round trip through the store, missing context, domain rejection behind a valid checksum, mixed-moment detection, unknown-section carry-over, legacy payloads |
| Inventory transactions (5 cases + 30 unchanged M2B1 cases) | `InventoryTxTests.cpp` | byte-identical transcript vs. the old implementation, exact rollback, id never reused, cached totals = recomputed; three mutation checks detected |
| Test-name guard | `spn_core_test_names` | fails on any `;` in a case name |

## 6. Known limitations of the validation

* Everything ran on **one** machine (Fedora, x86-64, GCC + Clang). No MSVC, no Windows, no ARM.
* Atomic publication is tested by **simulated** faults and on tmpfs/btrfs/FUSE-NTFS behaviour probes; **no real power-loss or kill test** was run, so crash safety on real disks is unverified. On the repository volume (NTFS via FUSE) we explicitly make no crash-safety claim.
* Benchmarks: single machine, other load present, `-march=native`, synthetic workload; see `INVENTORY_PERFORMANCE.md` for the measured noise.
* The coordinate presets were checked only for self-consistency; nothing was run in Unreal or against WorldForge output.
* `unreal_stub_check` success does not mean the Unreal project builds.
* The sanitizer search looked for the standard ASan/UBSan/LSan message texts in all build and test logs; zero hits. The sanitizer presets use `-fsanitize=address,undefined -fno-sanitize-recover=undefined` (`CMakeLists.txt`), so any UBSan finding would abort the test process; none did, and the logs contain no `runtime error`.

## 7. Windows / Unreal / WorldForge — what must still be done

**Windows (nothing below was done here):**
1. Build the Core with MSVC/clang-cl; `SaveStore.cpp` (`_commit`, `<io.h>`), `Inventory.cpp` (`__builtin_*_overflow`) are the portability risks.
2. Run `ctest` there; all 478 entries pass on Fedora.
3. Save protocol: run `spn_core_save_probe` on the real save volume (NTFS) and do a kill-during-save test.

**Unreal Engine 5.8.4:**
1. Decide and test the axis preset (`WORLD_COORDINATE_CONVENTIONS.md` section 8).
2. Write the adapter module; replace the stub check by a real build; wire `Spn.SaveWorld/LoadWorld` to the save-slot API.
3. Run the existing QA automation on Windows (not run here).

**SPN WorldForge:** provide the frame/handedness/datum statement, layer ids and region data; the Core has no geography.

## 8. Decisions that are the developer's (not made here)

1. UE axis preset and world origin (with WorldForge, `CQ-101`).
2. (Resolved in M2B2.1: door lock guards opening only; cone unchanged; steering comparison direction.) Still yours to confirm: that "an open locked door may be closed" is the final gameplay rule — it is implemented as decided in the M2B2.1 task, the master has no door-lock specification.
3. Which save sections are required in a shipped game; slot layout and generations; co-op and mod sections.
4. Whether the stackable-search index (`INVENTORY_PERFORMANCE.md` section 6) is wanted.
