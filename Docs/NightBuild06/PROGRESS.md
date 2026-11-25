# NightBuild06 – running progress log

Order: developer, 2026-11-24 ("NIGHT BUILD 06 – Integration, Character & Gameplay Stabilization"). This file is updated after every
finished section so nothing is lost if the session stops. The final report is `README.md` in this folder.

No commits, pushes or tags. Branch `checkpoint/2026-11-15` (the developer's commits `0c93987..d7fe160`), not pushed.

## Baseline (2026-11-24, before any NightBuild06 change)

| Check | Result |
|---|---|
| Core `ctest` (MSVC Release, `build/msvc`) | 640 / 640 |
| Python unit tests (11 files) | 230 / 230 (incl. `test_spn_unreal_static` 11/11: the door rule passes since the door fix of 2026-11-24, before this order) |
| Unreal headless `SpnGame+SpnCore`, editor context (`UnrealEditor-Cmd`, as the checkpoint ran it) | after the Phase C changes: 188 / 188 (186 of the checkpoint + 2 new) |
| Same, but as a `-game` process on `Lvl_SpnMovementTest` (my first attempt) | 177 / 181, 4 movement map tests fail (character does not move: "steady speed 0"). A launch-mode difference, not a regression: in the editor context the same tests pass. Recorded so nobody repeats it. |

## Phase A – state and backup

- [x] Drives: only `C:` (one NVMe disk, 930 GB, 208 GB free). **No separate drive: a real backup is blocked.**
- [x] Interim copy on the same disk (protects against accidental deletion / generator overwrite, NOT against a disk failure):
      `C:\SpnBackup\NB06_2026-11-24\Content` (1646 files, 2 939 059 617 bytes) + `Artifacts` (images, no videos / ZIPs).
      Verified file by file with SHA-256: 1646 / 1646 identical, 0 mismatches. List: `C:\SpnBackup\NB06_2026-11-24\SHA256SUMS_Content.txt`.
- [x] Dependency report (new read-only tool `Unreal/SupernaturalProt/Tools/asset_dependency_report.py`, asset registry, incl.
      one-file-per-actor packages) → `Saved/SpnAssets/asset_dependencies.json`.
- [x] Asset manifest (new tool `Unreal/SupernaturalProt/Tools/asset_manifest.py`) → `Docs/NightBuild06/asset_manifest.csv` (1646 rows),
      `asset_manifest_summary.json`.
- Finding: the **tracked** map `Lvl_SpnBuildingTest` needs 13 untracked packages (`SpnTest/Materials`); `Lvl_SpnMovementTest`,
  `V2Env`, `V3Interiors`, `NpcTest`, `MainMenu` are untracked themselves. All missing test-map dependencies are under `SpnTest/` (own,
  script-generated, 4.0 MB, no Fab / MetaHuman). A fresh checkout today cannot run the headless or QA suites.

## Phase B – bugs

- [x] Door sweep (`FindSweepBlockers`): fixed before this order (2026-11-24): any-hit query per frame, second query only while blocked.
- [x] Stale QA text `Spawn.npc_vs_player`: NPCs exist since NightBuild05; status NOT_IMPLEMENTED → NOT_TESTED (not placed in V2), text
      and `qa_inventory.json` gap corrected, `Gameplay-QA-Inventory.md` regenerated (`check_qa_inventory.py --check`: 0 problems).

## Phase C – body collision profiles

- [x] Core `BodyCollision.h/.cpp` + `Tests/Core/BodyCollisionTests.cpp` (8 cases): Keep (default) / FromHeight / Explicit, limits, crouch,
      passage fit. ctest 648 / 648.
- [x] SpnCore bridge `SpnBodyCollision.h/.cpp`; `ASupernaturalProtCharacter::ApplyBodyCollision` (feet, mesh, camera, eye height stay;
      refused without room / crouched / in a scripted move); presets get `Collision` + `ExplicitCollision`; test presets `test_160_fit`,
      `test_195_fit`, `mh_male_193_fit`. Default presets unchanged (35 / 90).
- [x] Headless `SpnGame.Character.BodyCollision*` 2 / 2. Measured: blueprint capsule 35 / 90, crouched 40, eye 64; test_195_fit → 35 / 98.5,
      crouched 43.8, head 2 cm inside (plain test_195: 15 cm above the capsule).
- [x] Rendered QA `Movement` (incl. Jump, LedgeAssist, Locomotion) with `mh_male_193_fit`: 58 / 0 (run `20261124-230913`).
- [x] Doors 89 / 0, Building 42 / 0, Traversal 22 / 0, Access 15 / 0 with `mh_male_193_fit` (series "night", 2026-11-24 23:20-23:40).

## Phase E – traversal and doors

- [x] New `SpnGame.Movement.JumpWithLedgeAssist` (headless, map test): ledge assist ON, one press in reach (30 cm) and one early (150 cm)
      at all 12 thin obstacles. 1 / 1. Measured: up to 25 cm walked (no move); 30-120 cm Vault; 150 cm (40 cm deep) refused "no room to
      stand on top"; an early press never starts a move, and from 45 cm up the early jump does not get over (by design: the assist only
      starts in reach, there is no catch in the air).
- [x] New `SpnGame.Doors.NpcInTheSwingIsNeverPushed` (real `ASpnNpc`) and `SpnGame.Doors.OpenerStepClearsTheLeafForEveryBodyProfile`
      (default, test_160_fit, test_195_fit). Headless 31 / 31 together with SpnGame.Npc / Doors / NightBuild05 / Character.

## Phase F – NPC

- [x] Player footstep noises (NightBuild05 KNOWN_ISSUES 6): Core `FootstepNoiseFor` (+2 Core cases, ctest 650 / 650), character emits a
      Footstep noise every 150 cm on the ground; never suspicious on its own. Calm NPCs ignore it, wary NPCs do not. NPC hearing skips its
      wall traces for noises beyond the free range (same verdict). Headless `SpnGame.Npc.PlayerFootstepsAreGameplayNoises`,
      `SpnGame.Npc.FootstepsMatterOnlyToAWaryNpc` 2 / 2.
- [x] Rendered `Npc` 29 / 0 and `World` 88 / 0 with the footsteps (same as the checkpoint counts).

## Phase D – animation (measurement)

- [x] `SpnQA.CharacterBody` gets more measured scenarios (same foot statistics): walk → jog, run → sprint, braking from a run, jog
      backwards (S), jog sideways (D). Character 54 / 0 with none / mh_male_193 / mh_male_193_fit (run `20261124-234229`).
- Finding: the blend-space input comes from `ABP_Unarmed`'s own template variables, so C++ cannot feed it a speed-matched value. The
  remaining jog / run sliding (30-50 cm/s, cause in 0017 §B.3) needs clips at those speeds or an anim-graph change (stride warping):
  both need a decision; nothing was faked with play rates.

## Defects found on the way (fixed)

- `SpnQA.CharacterBody` was **Fail at the automation level in every run since 2026-11-14** (runs 20261114-232124 ... 20261115-022006),
  hidden because all its QA checks were PASS: `GetUsedTextures(..., EMaterialQualityLevel::Num)` trips an engine ensure in UE 5.8.
  Now `High`; the run 20261124-234229 reports `Success`.
- `-SpnQaBodies=a,b,c` ran only the first body (`FParse::Value` stops at a comma). Fixed (`bShouldStopOnSeparator = false`).

## Phase H – modular body (rendered proof)

- [x] New check `CharacterBody.<body>_modular_marker` on all three bodies: the neutral engine-sphere marker exists and is visible on the
      SHOWN body with its category enabled and the chest exposed; under a data-only opaque test vest no detail component exists. PASS on
      placeholder, mh_male_193, mh_male_193_fit; screenshots looked at (exposed: sphere visible; covered: gone). Observation: the marker
      offset (spine_03 + 12 cm Y) is authored for the mannequin's bone axes and sits at the belly on the MetaHuman body.

## Phase G – route performance (2560 x 1440, 90 % TSR, `cine/route_perf.json`, run `route_perf_nb06`)

| Shot | 2026-11-14 (fps / 1 % low) | 2026-11-24 (fps / 1 % low) | frames > 33 ms | worst frame |
|---|---|---|---|---|
| main_street | 64.4 / 54.3 | 70.1 / 58.7 | 0 | 20.0 ms, render thread |
| forest_track | 52.6 / 46.8 | 56.8 / 49.9 | 0 | 21.8 ms, render thread |
| oak_north_route | 58.0 / 52.0 | 62.6 / 54.4 | 0 | 26.2 ms, render thread |

Nothing in tonight's changes touches rendering; the gain is attributed to machine state (driver / caches), not claimed as an
improvement. New in the perf report: `frames_over_33ms`, `frames_over_50ms`, `worst_frames` (time, ms, game / render / GPU, compiling).

## Phase I – packaging

- [x] `Tools/package_dev_build.ps1`: first attempt failed (C4530 in SpnCore, Editor.h in a test), both fixed; second attempt BUILD SUCCESSFUL (2.55 GB, 11.9 min, 0 compiler warnings). SpnWorldQA inside the packaged exe: 88 / 88.

## Final tests

ctest 650/650, Python 238/238, headless 193/193, QA Full 360 PASS / 0 FAIL (20261125-002551). Report: README.md.
