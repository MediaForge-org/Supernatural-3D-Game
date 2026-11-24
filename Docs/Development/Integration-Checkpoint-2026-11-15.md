# Integration checkpoint 2026-11-15: state, tests, risks, Git plan

Inspection only. **No feature added, no test changed, nothing deleted, no commit, push or tag.** Planning folders, `References/`, the QA
test maps and the blend space are unchanged (checked with `git status`).

Branch `main` at `704e54d`, remote `origin` exists. Working tree:
- 52 modified tracked files;
- 207 new untracked files;
- 0 deleted.

## 1. Inventory of the working tree

### 1.1 Modified, tracked (52)

| Kind | Files |
|---|---|
| Core C++ (save `DefinitionMismatch`, game events) + tests | `Core/CMakeLists.txt`, `GameEvents.{h,cpp}`, `SaveSlot.{h,cpp}`, `GameSave.cpp`, `Tests/Core/CMakeLists.txt`, `GameSaveTests.cpp` |
| Unreal C++ | 33 files: SpnCore save / map; movement (ledge assist); interaction / doors; session / settings; UI; character; `Build.cs` |
| Unreal QA tests | `SpnMovementMapAutomation.cpp`, `SpnNightBuild04Automation.cpp`, `SpnQaBuildingAutomation.cpp`, `SpnQaMovementAutomation.cpp`, `SpnQaRecorder.h`, `SpnMapSubsystemAutomation.cpp` (see §3) |
| Config / project | `DefaultEngine.ini` (Nanite foliage, runtime navmesh), `DefaultGame.ini` (character presets, wardrobe, staging), `SupernaturalProt.uproject` (+ plugin `ProceduralVegetationEditor`, from 2026-11-12) |
| Unreal asset (LFS) | `ABP_Unarmed.uasset`: reparented to `USpnAnimInstance`, 386 → 397 KB |
| Tools / QA | `Tools/run_gameplay_qa.ps1`, `qa_inventory.json`, `create_spn_building_test_map.py` |
| Docs | `Gameplay-QA-Inventory.md` (generated), `0009-traversal-...md` |

### 1.2 New, untracked (207)

| Kind | Count | Size |
|---|---|---|
| Unreal C++ (incl. 13 new test files) | 61 | ≈ 1.1 MB |
| Unreal Python tools (`Tools/spn_world/*`, `character_*`, `locomotion_*`, ...) and JSON (cine runs) | ≈ 75 | |
| Core: `LedgeAssist`, `NpcReaction`, `BodyCoverage` + tests | 9 | 76 KB |
| `Tools/` (SpnWorldTool C++, PowerShell scripts, `Tools/protagonist/*`) | 15 | 120 KB |
| Docs (proposals 0013–0018, development notes, NightBuild05) | 19 | 200 KB |
| Content data JSON (`SpnWorld/Data/SpnWorldMap.json`, `SpnWorldDecorDoors.json`) | 2 | |
| **Generated images** `Artifacts/VisualProgress/**` (PNG / JPG, partly derived from the reference images) | 60 | **21 MB** |

### 1.3 Ignored and therefore NOT secured (important)

`.gitignore` excludes `*.uasset` / `*.umap`. LFS (`.gitattributes`) applies only to files that were force-added: 757 tracked.

**886 Unreal content files, 2.67 GB, exist only on this disk:**

| Content | Size | Note |
|---|---|---|
| **QA test maps** `Lvl_SpnMovementTest` (headless movement tests), `Lvl_SpnNpcTest` (suite Npc), `Lvl_SpnMovementTestV2Env`, `Lvl_SpnMovementTestV3Interiors` (Traversal / Access), `Lvl_SpnMainMenu` + `SpnTest/Materials` | ≈ 4 MB | **Tests depend on them**; generator scripts exist (regenerable, but not byte-identical) |
| `SpnWorld/*` (`Lvl_SpnWorld` 26 MB, textures, meshes, vegetation, materials) | ≈ 530 MB | own and generated; partly Fab-derived (`FabDerived`) |
| `Fab/*`, `Megaplant_Library/*` | ≈ 1.75 GB | **third-party content: must not go into a (later public) repository**; licence |
| `SpnCharacterPrep/*` (MetaHuman test bodies, retargeters, candidates) | ≈ 363 MB | generated with scripts; MetaHuman licence, so keep out of git |
| `Characters/ProtagonistV1/*` (`MHC_ProtagonistV1`, comparison level) | 20 MB | **source of the male test body** (`character_mh_male_setup.py` copies it); not regenerable without manual work |

Also ignored and fine to stay so (regenerable or too large):
- `Saved/` (QA reports, logs);
- videos `Artifacts/**/*.mp4` (≈ 2 GB);
- ZIP archives (≈ 690 MB);
- `build/`, `Binaries/`, `Intermediate/`.

### 1.4 Size, LFS, secrets

| Check | Result |
|---|---|
| Versioned files over 1 MB outside LFS | only the 3 reference PNGs (2.2–2.7 MB), unchanged |
| Pack size | 80 MB |
| Untracked files over 1 MB | 3 PNGs in `Artifacts` |
| Secret-like strings | only `SecurityToken=` in `DefaultEngine.ini`: the UE template's Android file-server token, versioned since `cd7d0da`, not new |
| Machine-specific paths | only `%LOCALAPPDATA%` look-ups in scripts and one note in 0016; no absolute user paths in code |

## 2. QA test changes (§3 of the order)

| File | Change | Assessment |
|---|---|---|
| `SpnMovementMapAutomation.cpp` (`SpnGame.Movement.Obstacles`) | ledge assist **switched off** during the jump-physics test, on again afterwards | **Condition changed, criteria unchanged.** Reason: since 2026-11-14 a jump press in front of an obstacle starts a mantle. The jump physics stay checked; the ledge assist has its own tests. Effect: this test no longer covers the interplay of jump and ledge assist. |
| `SpnQaBuildingAutomation.cpp` | `Press F` → `Hold F` 2.2 s for breaking doors | follows the new hold-to-break mechanic; checks unchanged |
| `SpnNightBuild04Automation.cpp` | UI language pinned to "de"; Force on the pad = Square **held** instead of Triangle; Force pad key removed from the "Interact ≠ Force" key check | language pin: stabilisation. The key check is **deliberately narrowed** (decision of 2026-11-14: Square = tap Interact / hold Force); replaced by "hold needed ≥ 0.2 s", "Triangle free", and the rendered test `SpnWorldQA.RouteControls` |
| `SpnQaMovementAutomation.cpp` | comment only | – |
| `SpnQaRecorder.h` | optional `-SpnQaNoOverlay` | no effect on checks |
| `SpnMapSubsystemAutomation.cpp` | new cases (old SpnWorld save, `OtherWorld`) | extension |
| `run_gameplay_qa.ps1` | new suites (Ledge, Character, Npc, World), more expected groups, video capture ddagrab | extension only, nothing removed |
| `qa_inventory.json` | gap texts extended, new entries | nothing removed |
| `create_spn_building_test_map.py` | new variant `SPN_VARIANT=npc` | default path unchanged; `Lvl_SpnBuildingTest` is unchanged in git |
| Test maps | `Lvl_SpnBuildingTest`, `Lvl_SpnDoorTest`, `Lvl_SpnMovementTestV2`, `Lvl_SpnNavDemo`: **unchanged** | the untracked maps (§1.3) have no git baseline: they cannot be compared |

No test was removed; no threshold was loosened. Two conditions changed deliberately: Obstacles and the pad key check.

## 3. Test results (run on 2026-11-15)

| Test | Result |
|---|---|
| Core `ctest` (MSVC Release) | **640 / 640** |
| Python unit tests (11 files, 230 tests) | **10 files OK, 1 FAILED** (see 3.1) |
| Unreal headless `SpnGame+SpnCore` | **186 / 186** |
| QA `Full` (`20261115-022006`, incl. Doors, Buildings, Movement, Jump, LedgeAssist, CharacterBody, Locomotion, Camera, SaveLoad, Performance, HouseJourney, NpcReaction, InventoryUi, BuildingAccess) | **342 pass, 0 fail**, 3 NOT_IMPLEMENTED, 1 NOT_TESTED (known, see 3.2) |
| QA `World` (`20261115-023659`) | 88 / 0 |
| QA `Traversal` on V3Interiors (`20261115-024301`) | 22 / 0 |
| QA `Access` on V3Interiors (`20261115-024525`) | 15 / 0 |
| QA `Movement` with `mh_male_193` (`20261115-024648`) | 58 / 0 |
| QA `Doors` with `mh_male_193` (`20261115-025315`) | 89 / 0 |
| QA `Building` (HouseJourney) with `mh_male_193` (`20261115-025619`) | 42 / 0 |

### 3.1 Failed test

`test_spn_unreal_static.DoorSourceTest.test_query_uses_object_type_pawn_only_and_ignores_itself`:
- The rule is: no `OverlapMulti` (result-array allocation) in the door's obstruction query.
- `ASpnTestDoor::FindSweepBlockers` (door step-aside, 2026-11-13) uses `OverlapMultiByObjectType` and is called **every frame of a swinging
  door** from `Tick`.
- Functionally correct; the QA door tests pass.
- It breaks the project's own static rule, and it is a small performance risk with many doors.
- **Open bug**; the test was deliberately not changed.

### 3.2 Not passed, known (Full)

- `SaveLoad.player_position_saved`, `SaveLoad.save_ui_and_autosave`: not implemented on the console path (the game flow has them).
- `SaveLoad.inventory_save_load`: covered elsewhere.
- `Spawn.npc_vs_player`: no NPC in the V2 level. Its text ("Core Npc module is not integrated") is **out of date** since NightBuild05.

### 3.3 Not runnable here

| Check | Why |
|---|---|
| Packaged build / packaged performance | no packaged build of the project exists yet; the cook list for config-only test assets is missing |
| DualSense on hardware | no controller connected; input checked in software only |
| Fedora build / Linux | not on this machine |
| `Planning/Master` consultation | the link is missing on this machine |
| MetaHuman cloud steps (rig, textures, DNA export) | not approved |
| Visual check of wearables / zone hiding | no garment and no split body exist |

## 4. Status by system

| System | Implemented | Prepared only | Tested | Untested | Known bugs | Risks | Next step |
|---|---|---|---|---|---|---|---|
| Ledge assist (StepOn / Vault / Mantle / ClimbUp) | Core planner + CMC custom mode, explicit jump input | climbing animations (plan A / C) | Core 10 cases, `SpnQA.LedgeAssist` 14 / 14, also with the 193 cm body | co-op prediction of the move | – | no animation: the capsule moves visibly "without arms" | approve the animation sources (MotionWarping, `VaultOver`) |
| Traversal / door passages | step-aside, door sweep, decor doors | – | Traversal 22 / 0, Access 15 / 0, Doors 89 / 0 (both bodies), HouseJourney 42 / 0 | doors vs NPCs; stairs at the door | `FindSweepBlockers` allocates per frame (3.1) | 193 cm head over a 180 cm capsule: low passages | move the query to `OverlapAny` + a separate blocker search only when blocked |
| Controller layout | Square tap / hold, D-pad up, touchpad, Options, L3 | "sprint toggle / hold" setting | NightBuild04 keys, `SpnWorldQA.RouteControls` | hardware | – | L3 usability (analysis in Controller-Input) | hardware test |
| Save `OtherWorld` | Core `DefinitionMismatch`, UI messages | – | Core + `SpnCore.Map.*` | – | – | – | – |
| MetaHuman test bodies | `mh_template`, `mh_male_193` (runtime retarget, no scaling) | protagonist V1 (face) | CharacterBody 36 / 0, Movement / Doors / Building with the male body, 20 / 20 clips retargeted | packaged cost, render thread, LOD transitions in motion | the render-thread counter reads 0 with a retargeted body (measurement) | **assets only local** (§1.3); MetaHuman licence; textures 178 MB resident | back up the local assets; packaged measurement |
| Retargeting / foot sliding | leg goals blended to the source, cadence above 600 cm/s | – | medians placeholder / template / male 193 (cm/s), see the table below | 8-direction strafe in detail | – | jog / run cadence needs clips | clips or speeds (decision) |
| Head round trip | public interfaces, prototype | lossless DNA route (cloud) | Blender pass-through lossless; template import = a 2–3.5 mm fit | rig after import | – | likeness limit of the fit | decide the head route |
| BodyCoverage / wardrobe | Core rules, SpnCore bridge, `USpnWardrobeComponent`, neutral marker | real garments, zone-split body, combined hide mask, save schema v2 | Core 8 cases, `SpnGame.Character.WardrobeLayersAndDetails` | **nothing visible on screen** (no assets) | – | MetaHuman bodies carry painted underwear; save integration open | P2: split the test body into zones |
| SpnWorld (placeholder world) | generator, streaming, lighting, route | – | World 88 / 0 | – | – | ≈ 530 MB assets only local; Fab-derived content | backup / LFS decision |
| NPC (NightBuild05) | perception, reactions, memory | – | NpcReaction 29 in Full, headless | – | the `Spawn.npc_vs_player` text is outdated | `Lvl_SpnNpcTest` not versioned | version the test map |

Foot sliding: median of a planted foot in cm/s (Full run `20261115-022006`):

| Gait | Placeholder | MH template | MH male 193 |
|---|---|---|---|
| Walk | 3.8 | 5.2 | 4.9 |
| Jog | 31.6 | 33.1 | 30.4 |
| Run | 49.9 | 50.6 | 51.4 |
| Sprint | 52.6 | 56.8 | 48.9 |
| 90° turn | 30.9 | 31.4 | 29.8 |
| Start / stop | 9.8 | 10.1 | 10.4 |

## 5. Risks (priority)

1. **Data loss:** 2.67 GB of content, including test maps the tests need and the V1 MetaHuman asset, exist on this disk only.
2. **Licence / publication:** Fab, Megaplant and MetaHuman content must never land in git by a blanket `git add -f` (open-source goal, CLAUDE.md §9).
3. **Build coupling:** the C++ commits must go in together (Core ↔ SpnCore ↔ game module ↔ config presets).
4. Line endings: `.gitattributes` normalises to LF; on `git add` Git converts CRLF files (expected warnings, content unchanged).
5. `ProceduralVegetationEditor` in the `.uproject` (2026-11-12): needed only for the vegetation tools. Check before committing.

## 6. Git checkpoint plan

**Order:** first secure the local data that git does not cover, then four focused commits on a separate branch. Execute only after
review. Nothing below has been run.

### 0. Backup of the unversioned content (outside git)

```powershell
# to an external drive / second disk; adjust the target
robocopy "C:\Projekte\Supernatural-3D-Game\Unreal\SupernaturalProt\Content" "E:\SpnBackup\2026-11-15\Content" /E /XO /R:1 /W:1
robocopy "C:\Projekte\Supernatural-3D-Game\Artifacts" "E:\SpnBackup\2026-11-15\Artifacts" /E /XO /R:1 /W:1
```

### 1. Start state and branch

```powershell
cd C:\Projekte\Supernatural-3D-Game
git status --short | Measure-Object        # expected: 260 lines (52 modified, 207 new + this report)
git diff --check                          # expected: line-ending warnings only, no whitespace errors
git switch -c checkpoint/2026-11-15       # keep main untouched
```

### 2. Four commits

After each `git add`:
- check `git diff --cached --stat` against the list;
- check that `git lfs status` shows **only** `ABP_Unarmed.uasset` as an LFS object (commit B);
- then commit.

- Commit A builds on its own (ctest).
- Commits A + B together build the Unreal project.
- C and D are tools and text.

### Commit A: Core-Regeln und Core-Tests (17 Dateien)

```powershell
git add -- `
  "Core/CMakeLists.txt" `
  "Core/include/spn/core/GameEvents.h" `
  "Core/include/spn/core/SaveSlot.h" `
  "Core/src/GameEvents.cpp" `
  "Core/src/GameSave.cpp" `
  "Core/src/SaveSlot.cpp" `
  "Tests/Core/CMakeLists.txt" `
  "Tests/Core/GameSaveTests.cpp" `
  "Core/include/spn/core/BodyCoverage.h" `
  "Core/include/spn/core/LedgeAssist.h" `
  "Core/include/spn/core/NpcReaction.h" `
  "Core/src/BodyCoverage.cpp" `
  "Core/src/LedgeAssist.cpp" `
  "Core/src/NpcReaction.cpp" `
  "Tests/Core/BodyCoverageTests.cpp" `
  "Tests/Core/LedgeAssistTests.cpp" `
  "Tests/Core/NpcReactionTests.cpp"
git diff --cached --stat
git diff --cached --check
git lfs status
# erst nach Durchsicht:
# git commit -m "feat(core): ledge assist, NPC reaction, body coverage rules; save DefinitionMismatch"
```

### Commit B: Unreal-Quellcode, Konfiguration, Projektdatei, ABP_Unarmed (LFS) (89 Dateien)

```powershell
git add -- `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnMapSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnSaveSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/Tests/SpnMapSubsystemAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Public/SpnMapSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SpnCore/Public/SpnSaveSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnBodyCoverage.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnLedgeAssist.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnNpcMemoryInternal.h" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnNpcMemorySubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/SpnNpcReaction.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Private/Tests/SpnNpcMemoryAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SpnCore/Public/SpnBodyCoverage.h" `
  "Unreal/SupernaturalProt/Source/SpnCore/Public/SpnLedgeAssist.h" `
  "Unreal/SupernaturalProt/Source/SpnCore/Public/SpnNpcMemorySubsystem.h" `
  "Unreal/SupernaturalProt/Source/SpnCore/Public/SpnNpcReaction.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnCharacterMovementComponent.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnCharacterMovementComponent.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnInteractable.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnInteractionComponent.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnInteractionComponent.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnSessionSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnSessionSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnSettingsSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnSettingsSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnTestDoor.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnTestDoor.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnWorldObjects.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnMovementMapAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnNightBuild04Automation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaBuildingAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaMovementAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaRecorder.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnHudWidget.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnHudWidget.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnInventoryScreen.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnInventoryUiSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnInventoryUiSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnMapScreen.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnUiNotify.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnUiSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnUiSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/UI/SpnUiTextNightBuild.inc" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/SupernaturalProt.Build.cs" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/SupernaturalProtCharacter.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/SupernaturalProtCharacter.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnLedgeAssistMovement.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnNpc.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnNpc.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnDoorStepAsideAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnNightBuild05Automation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnNpcHearingProbeAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaLedgeAssistAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaNpcAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnAnimInstance.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnAnimInstance.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnCharacterVisualComponent.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnCharacterVisualComponent.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnMetaHumanPrepLibrary.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnMetaHumanPrepLibrary.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnRetargetFollowAnimInstance.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnRetargetFollowAnimInstance.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnWardrobeComponent.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Character/SpnWardrobeComponent.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnCharacterPresentationAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaCharacterBodyAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaSpnWorldAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaSpnWorldControlsAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnQaSpnWorldRouteAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/Tests/SpnWorldQualityAutomation.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnCineSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnCineSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnDecorDoorSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnDecorDoorSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnRouteGuideSubsystem.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnRouteGuideSubsystem.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnSkinnedGroup.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnSkinnedGroup.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnWorldBuildLibrary.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnWorldBuildLibrary.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnWorldLighting.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnWorldLighting.h" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnWorldWarmup.cpp" `
  "Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/World/SpnWorldWarmup.h" `
  "Unreal/SupernaturalProt/Config/DefaultEngine.ini" `
  "Unreal/SupernaturalProt/Config/DefaultGame.ini" `
  "Unreal/SupernaturalProt/Content/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.uasset" `
  "Unreal/SupernaturalProt/SupernaturalProt.uproject" `
  "Unreal/SupernaturalProt/Content/SpnWorld/Data/SpnWorldDecorDoors.json" `
  "Unreal/SupernaturalProt/Content/SpnWorld/Data/SpnWorldMap.json"
git diff --cached --stat
git diff --cached --check
git lfs status
# erst nach Durchsicht:
# git commit -m "feat(unreal): ledge assist, controller layout, doors, NPC, SpnWorld systems, character presentation (MetaHuman test bodies, wardrobe)"
```

### Commit C: Werkzeuge und QA (73 Dateien)

```powershell
git add -- `
  "Tools/run_gameplay_qa.ps1" `
  "Unreal/SupernaturalProt/Tools/create_spn_building_test_map.py" `
  "Unreal/SupernaturalProt/Tools/qa_inventory.json" `
  "Tools/protagonist/blender_fbx_passthrough.py" `
  "Tools/protagonist/blender_sculpt_probe.py" `
  "Tools/protagonist/compose_board.ps1" `
  "Tools/protagonist/mh_head_roundtrip.py" `
  "Tools/protagonist/mh_multiview_experiment.py" `
  "Tools/protagonist/reference_board.py" `
  "Unreal/SupernaturalProt/Tools/animation_inventory.py" `
  "Unreal/SupernaturalProt/Tools/character_api_probe.py" `
  "Unreal/SupernaturalProt/Tools/character_body_inventory.py" `
  "Unreal/SupernaturalProt/Tools/character_candidate_check.py" `
  "Unreal/SupernaturalProt/Tools/character_dependency_report.py" `
  "Unreal/SupernaturalProt/Tools/character_mh_male_setup.py" `
  "Unreal/SupernaturalProt/Tools/character_mh_test_setup.py" `
  "Unreal/SupernaturalProt/Tools/character_retarget_prep.py" `
  "Unreal/SupernaturalProt/Tools/character_skeleton_contract.py" `
  "Unreal/SupernaturalProt/Tools/input_mapping_report.py" `
  "Unreal/SupernaturalProt/Tools/level_content_report.py" `
  "Unreal/SupernaturalProt/Tools/locomotion_stride_check.py" `
  "Unreal/SupernaturalProt/Tools/reparent_abp_unarmed.py" `
  "Unreal/SupernaturalProt/Tools/texture_streaming_audit.py" `
  "Tools/SpnWorldTool/CMakeLists.txt" `
  "Tools/SpnWorldTool/common.h" `
  "Tools/SpnWorldTool/main.cpp" `
  "Tools/SpnWorldTool/terrain.cpp" `
  "Tools/SpnWorldTool/textures.cpp" `
  "Tools/play_spnworld.ps1" `
  "Tools/spn_visual.ps1" `
  "Tools/spn_visual_phases.ps1" `
  "Tools/spn_world_build.ps1" `
  "Unreal/SupernaturalProt/Tools/spn_world/archkit.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/build_world.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/buildings.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/baseline.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/baseline_probe.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/forest_profile.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/gameplay.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/gameplay_fab.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/interior.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/lighting.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/moody.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/quality_perf.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/route_check.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/route_perf.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/showcase.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/sp_perf.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/sp_video.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/streaming_long.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/streaming_probe.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/streaming_probe_cold.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/top_view.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/tour.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine/track_variation.json" `
  "Unreal/SupernaturalProt/Tools/spn_world/cine_variants.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/fab.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/gameplay.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/layout.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/make_cine.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/make_streaming_long.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/make_streaming_probe.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/map_data.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/materials.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/meshkit.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/nature.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/props.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/qa_points.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/roads.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/terrain_gen.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/ue_kit.py" `
  "Unreal/SupernaturalProt/Tools/spn_world/woodland.py" `
  "Docs/Development/Gameplay-QA-Inventory.md"
git diff --cached --stat
git diff --cached --check
git lfs status
# erst nach Durchsicht:
# git commit -m "chore(tools): QA suites Character/Ledge/Npc/World, character and world build tools, QA inventory"
```

### Commit D: Dokumentation (21 Dateien, inkl. dieses Berichts)

```powershell
git add -- `
  "Docs/Proposals/0009-traversal-jump-and-vault-direction.md" `
  "Docs/Development/Controller-Input-2026-11-14.md" `
  "Docs/Development/Door-Swing-Analysis-2026-11-13.md" `
  "Docs/Development/Fab-Asset-Sources.md" `
  "Docs/Development/LEVEL_REGISTRY.md" `
  "Docs/Development/Ledge-Animation-Plan-2026-11-14.md" `
  "Docs/Development/Ledge-Assist-2026-11-14.md" `
  "Docs/Development/NPC-Hearing-Alarm-Analysis-2026-11-13.md" `
  "Docs/Development/SpnWorld-Gameplay-Phase1.md" `
  "Docs/Development/Streaming-PopIn-2026-11-14.md" `
  "Docs/Development/Visual-Build-Tools.md" `
  "Docs/NightBuild05/IMPLEMENTATION_REPORT.md" `
  "Docs/NightBuild05/KNOWN_ISSUES.md" `
  "Docs/NightBuild05/README.md" `
  "Docs/Proposals/0013-player-character-replacement-plan.md" `
  "Docs/Proposals/0014-first-player-character-proposal.md" `
  "Docs/Proposals/0015-protagonist-v1-reconstruction-plan.md" `
  "Docs/Proposals/0016-protagonist-v1-feasibility-report.md" `
  "Docs/Proposals/0017-protagonist-v1-phase2-report.md" `
  "Docs/Proposals/0018-modular-body-architecture.md" `
  "Docs/Development/Integration-Checkpoint-2026-11-15.md"
git diff --cached --stat
git diff --cached --check
git lfs status
# erst nach Durchsicht:
# git commit -m "docs: proposals 0013-0018, development notes, NightBuild05 report"
```

### 3. Optional, only after a decision: test maps the tests need (LFS, force-add because of `.gitignore`)

```powershell
git add -f -- `
  "Unreal/SupernaturalProt/Content/SpnTest/Lvl_SpnMovementTest.umap" `
  "Unreal/SupernaturalProt/Content/SpnTest/Lvl_SpnMovementTestV2Env.umap" `
  "Unreal/SupernaturalProt/Content/SpnTest/Lvl_SpnMovementTestV3Interiors.umap" `
  "Unreal/SupernaturalProt/Content/SpnTest/Lvl_SpnNpcTest.umap" `
  "Unreal/SupernaturalProt/Content/SpnTest/Lvl_SpnMainMenu.umap"
git add -f -- "Unreal/SupernaturalProt/Content/SpnTest/Materials"
git lfs status                            # must list every .umap/.uasset as an LFS object
git diff --cached --stat
# git commit -m "test(content): version the QA test maps used by headless and QA suites (LFS)"
```

Before that, check: what do these maps reference? (Fab materials in `V2Env` / `V3Interiors` would be broken references in git, or
third-party content.)
`UnrealEditor-Cmd.exe ... -run=pythonscript -script=Tools/level_content_report.py` exists for that.

### 4. Deliberately NOT included

- `Artifacts/VisualProgress/**` (60 images, 21 MB, partly derived from the protagonist references): decision needed, either LFS for
  `Artifacts/**/*.png|jpg` or outside git.
- Everything under `Content/Fab`, `Content/Megaplant_Library`, `Content/SpnCharacterPrep`, `Content/Characters/ProtagonistV1`,
  `Content/SpnWorld` (assets): licence / size, see §5.
- `Saved/`, videos, ZIPs, `build/`.

### 5. Afterwards (developer)

```powershell
git log --oneline -6
git status --short                        # expected: only the 60 artifact images remain
# push only after your decision: git push -u origin checkpoint/2026-11-15
```
