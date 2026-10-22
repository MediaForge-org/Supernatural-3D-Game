# 0008 — M1: draft for ramp, stairs and wall in the test map, and the remaining M1 acceptance work — ENGINEERING PROPOSAL (DRAFT)

Status: draft written 2026-10-22 for developer review. **Not master canon.** Sections 1 to 8 are the original draft (written before the map was changed); the implementation status after approval is in section 9. It refines Proposal 0003 section 5 (milestone M1) and adds no gameplay system. All shapes are PLACEHOLDER test values.

## 1. State of the six M1 criteria (Proposal 0003 section 5)

| # | Criterion | State on 2026-10-22 | Evidence / gap |
|---|---|---|---|
| 1 | Project opens on Windows and starts in the editor (PIE) without errors | **Partly met** | `SupernaturalProtEditor` builds (UE 5.8.3); 14 of 14 automation tests pass, including `SpnGame.TestMap.PlayDoors`, which plays the map in PIE (`-nullrhi`). A manual PIE session by the developer has not been done. |
| 2 | Placeholder character walks, runs, jumps over slope, stairs, ramp, wall with collision and a collision-safe camera | **Not met** | Character (engine third-person template), jump, move/look and a spring-arm camera exist. The map has **no** ramp, stairs or wall. No sprint exists (`MaxWalkSpeed` 500, one speed). See sections 2 and 3. |
| 3 | Level and character labelled placeholder; no map presented as final | **Partly met** | Map and actors are named `SPN_*` and documented as placeholders; doors and pickups show a "PLACEHOLDER" label. The character is the engine mannequin; no in-game label. |
| 4 | A standalone (non-editor) build starts and is playable | **Open** | No packaged build exists. `-game` play via the editor binary is covered by the automation test only. See section 4. |
| 5 | Frame time measured and noted | **Open** | Nothing measured. See section 5. |
| 6 | Builds from a clean checkout following a written `Docs/` setup note | **Open** | README section 28.4 describes the Windows steps; it has not been followed on a clean clone, and there is no dedicated setup note. See section 6. |

## 2. Review of the existing movement and camera (read-only, no change made)
Source: `Source/SupernaturalProt/SupernaturalProtCharacter.cpp`.
- Capsule 42 x 96 cm; `bOrientRotationToMovement`; `MaxWalkSpeed` 500, `JumpZVelocity` 500, `AirControl` 0.35, braking 2000 / 1500. Default `MaxStepHeight` (45 cm) and walkable floor angle (about 44.8 degrees) are not overridden.
- Camera: spring arm, `TargetArmLength` 400, `bUsePawnControlRotation`, follow camera. `bDoCollisionTest` is the engine default (true) in C++; the Blueprint `BP_ThirdPersonCharacter` could override it, which is **not verified** (binary asset).
- No sprint action, no crouch. Whether "runs" in criterion 2 means a separate sprint is a **question for you** (section 7, Q1); 500 cm/s is already a fast walk.
- E interaction and Move/Look/Jump bindings come from the template contexts plus the runtime E mapping. No gamepad binding for E.

## 3. Draft: ramp, stairs and wall in the existing test map

### 3.1 Layout (map not yet touched)
Floor 40 x 40 m (X and Y from -2000 to +2000 cm), Player Start at the origin facing +X, doors at X 400, pickups at X 250. New geometry lies **behind the doors at X >= 900**, side by side, so doors and pickups keep their positions and the existing layout tests stay valid. Values are chosen inside the default movement limits.

| Piece | Position (cm) | Shape | Purpose |
|---|---|---|---|
| `SPN_M1_Ramp` | X 900 to about 1673, Y -800 | slab 800 x 300 x 20, pitch +15 degrees, top face starts at the floor, rises about 207 cm | walkable slope |
| `SPN_M1_Stair_01..10` + `SPN_M1_StairLanding` | X 900 to 1500, Y 0 | 10 solid steps, rise 18 cm, run 30 cm, 300 wide, then a 300 cm landing at 180 cm | steps below the 45 cm step limit |
| `SPN_M1_Wall` | X 1300, Y 800 | box 40 thick x 600 wide x 100 high | blocks walking, jumpable (apex about 127 cm) |

All pieces are engine cubes with the existing prototype grid material, collision enabled. Only `Lvl_SpnDoorTest` would change; no other map.

### 3.2 Checks for movement and camera collision
Automated (new, SPN-only; draft kept outside the repository, see 3.3):
1. `SpnGame.TestMap.M1Layout` (editor): pieces exist, have collision, lie behind the doors and on the floor; ramp pitch between 5 and 40 degrees; every stair tread rises 5 to 45 cm above the previous one; landing level with the top tread; wall between 50 and 120 cm high.
2. `SpnGame.TestMap.M1Traversal` (PIE or `-game`): teleports the character to the foot of each piece and applies forward input (`DoMove`) until a goal or a timeout: stairs (reach tread height without jumping), ramp (gain height), wall (stopped in front of it, still on the floor). Also asserts on the camera boom (`bDoCollisionTest`, `bUsePawnControlRotation`) and jump velocity.
- Not covered automatically: that the camera really avoids clipping into the wall (needs a render or a trace test with a defined camera pose) and subjective feel. Option: a trace-based test that places the character with its back to the wall and compares the camera location with the wall face. Not drafted yet.

Manual (developer, PIE):
- Walk up the stairs and the ramp, back down, jump onto the landing from the ramp side, walk into the wall, jump over it.
- Stand with your back to the wall and turn the camera: it must not pass through the wall or show the inside of it.
- Check that `E` still works on the doors and pickups, and `Spn.SaveWorld`/`Spn.LoadWorld`/`Spn.Inventory` still behave.

### 3.3 Draft code
A generator extension and the two tests exist as an unverified draft in
`C:\Users\Aquaman\AppData\Local\Temp\claude\C--Projekte-Supernatural-3D-Game\39c319a7-b501-494b-98a3-d9f82c77890c\scratchpad\m1-draft\`
(`create_spn_door_test_map.m1.py`, `SpnTestMapM1Automation.cpp`). The test file compiled once; the traversal test has **never been run**. It is not part of the repository. Applying it is a separate step after your approval (section 7).

## 4. Packaged Windows build (criterion 4): what is needed
- **Maps:** `DefaultEngine.ini` has `GameDefaultMap=/Game/ThirdPerson/Lvl_ThirdPerson`, so a packaged build would start the template map, not the test map, and the test map would not be cooked unless it is listed. Needed: either `-map=/Game/SpnTest/Lvl_SpnDoorTest` on the command line, or a packaging map list / default map setting (config change, your decision).
- **Command (sketch, not run):** `RunUAT.bat BuildCookRun -project=<uproject> -platform=Win64 -clientconfig=Development -build -cook -stage -pak -archive -archivedirectory=<dir> -map=/Game/SpnTest/Lvl_SpnDoorTest -unattended`. The staged build needs the "Game" target `SupernaturalProt.Target.cs` (exists).
- **Check list:** the packaged build must contain `SpnCore`; debug commands (`Spn.*`) are not in Shipping, so use Development for the first package; `Saved/SpnSaves` is written relative to the packaged project.
- **Open points:** Windows SDK and Visual Studio components for packaging (the editor build works; packaging prerequisites not verified), disk space, whether the `.umap` is available as real content (it is an LFS file).

## 5. Frame-time measurement on the RTX 3090 (criterion 5)
- Measure the packaged Development build (not PIE), in the test map, at a fixed spot and a fixed resolution; note GPU driver, resolution, scalability and build configuration. Suggested: `stat unit` and `stat fps` in-game, plus a short `CSVProfile` or `-benchmark -fps` run, three runs, report average and worst frames. No target is locked: record the number only.
- Prerequisite: criterion 4. The headless runs so far use `-nullrhi` and say nothing about rendering cost.
- Note that `nvidia-smi` exists on this PC; the measurement itself must be run and read by the developer or in a session with a real renderer.

## 6. Complete setup note (criterion 6)
Needed: a short `Docs/` note (or README section 28.4 completed) that was **followed on a clean clone**: required software and versions (UE 5.8.3, Visual Studio with the C++ workloads incl. `.vsconfig` of the project, Git LFS), clone, `git lfs pull`, generating project files, build command, opening the map, running automation tests, known warnings, short build paths (MAX_PATH), `SPN_CORE_ROOT` behaviour. The clean-clone test must not use the nested repository inside this working tree.

## 7. Decisions needed from you
1. **Sprint:** is a separate run action part of M1, or is the single walk speed enough for "runs"? (Speed values are not decided by this draft.)
2. **Where the geometry goes:** into `Lvl_SpnDoorTest` (this draft; keeps the three doors, two pickups and all tests) or a separate M1 map.
3. **Approval to apply the draft** (generator extension, regenerate the map, add the two tests).
4. **Packaging:** map list via command line or config change; which configuration for the first package.

## 9. Implementation status (2026-10-22, after approval)
Approved by the developer: geometry in `Lvl_SpnDoorTest`, Shift sprint, jumpable wall, backup before map changes (backup: `C:\Projekte\_SPN_Backups\2026-10-22_pre-M1`, outside the repository).

| Criterion | State |
|---|---|
| 1 PIE without errors | Automated PIE tests pass; **manual PIE test not done** |
| 2 walk/run/jump over ramp, stairs, wall; safe camera | Geometry placed (`SPN_M1_*`), sprint added (`DoSprintStart/End`, Left Shift, 800 cm/s placeholder); `M1Layout` and `M1Traversal` pass headless (`-nullrhi`). Camera collision against the wall is **only checked as a setting**, not visually or by trace; manual check open |
| 3 placeholder labelling | unchanged (partly met) |
| 4 standalone build | Packaged Development build created and started with the M1 map; default map unchanged (map passed via `-map` and a start argument) |
| 5 frame time | Measured once, see README §26.5; two runs, windowed 1080p |
| 6 clean-checkout setup | Note written (`Docs/Setup/Windows-Setup-und-Tests.md`); **not followed on a clean clone** |

Tests on 2026-10-22: Core 198 of 198 (Windows MSVC Debug); Unreal automation 16 of 16 (`SpnCore+SpnGame`).

## 10. Final audit (2026-10-22) — status: CONDITIONAL PASS
Legend: **A** = passed automatically, **M** = confirmed manually (developer's statement and logs of the packaged build), **-** = not checked, **F** = failed.

| # | Criterion | Result | Evidence and gaps |
|---|---|---|---|
| 1 | Starts and plays without errors | **A** + **M** | Automated PIE tests (`PlayDoors`, `M1Traversal`) pass. Developer played the packaged build; logs of that session contain no `Warning:`/`Error:` lines. A manual session **inside the editor (PIE)** was not performed. |
| 2a | Walk, sprint (Shift), jump | **A** (sprint binding, speed change, jump) + **M** (developer statement) | `M1Traversal`. No log evidence for the feel. |
| 2b | Ramp, stairs | **A** | `M1Layout`, `M1Traversal`. Developer reports trying them (no log evidence). |
| 2c | Wall | **A** blocks and is crossed with a jump; **M** crossed | Crossing works only because the template jump (about 127 cm apex) is high; no vault/mantle exists, see Proposal 0009. |
| 2d | Collision-safe camera | **-** | Only the spring-arm settings are asserted (`bDoCollisionTest`). Not verified visually or by trace; the developer's statement does not mention it. |
| 3 | Labelled placeholder | partly **A/M** | Map and actors are named/labelled as placeholders; the character is the engine mannequin without an in-game label. |
| 4 | Standalone build starts and is playable | **M** + **A** | Packaged Development build; the map loads with the start argument (checked in logs on 3 builds). Without the argument it starts the template map `Lvl_ThirdPerson` (checked). |
| 5 | Frame time measured | **A** (measurement) | One measurement series (README §26.5), windowed 1080p, character standing still. |
| 6 | Builds from a clean checkout with a written note | **A** (partly) | The note exists. Not a Git clone: a copy of the working tree without build artefacts, `.git` and the nested repository was built from scratch (editor build succeeded in about 4 min, 17 of 17 automation tests and 198 of 198 Core tests pass in that copy). A real `git clone` plus `git lfs pull` was not tested (the M1 changes are not committed). |

### Save/load and inventory (code and tests)
- Pickups persist and cannot be taken twice: `TestPickup.TakeOnce`, `.SaveLoadRestoresInventory` (taken stays refused after loading).
- Inventory after loading: **A** by `.SaveLoadRestoresInventory` (count restored 5 -> 3, and 3 in a new session). **Not shown in the manual logs** (`Spn.Inventory` was not run after loading in the packaged build).
- Doors after loading: **A** (`LoadShowsSavedState`, `PlayDoors`) and **M** (log: `door_b` shown open, `door_a` and `door_locked` closed after `Spn.LoadWorld`, revision 0 -> 5).
- Missing or damaged saves: **A** `SpnCore.WorldState.FailedLoadKeepsState` (doors) and the new `SpnGame.TestPickup.FailedLoadKeepsInventory` (inventory and placed pickups stay unchanged after a missing and a truncated slot, taken pickup gives nothing again).
- The manual log shows `Command not recognized: spn.sav` and `spn.load` (incomplete typed commands); the full commands were run afterwards. Not a defect.

### Build and warnings
- Cook: only `LogText: Warning: Saving FText "..." which has been initialized from FString` for the PLACEHOLDER labels (5 labels, harmless; not fixed). Runtime logs of the packaged build: no `Warning:`/`Error:` lines. `Failed to load 'aqProf.dll' / 'VtuneApi*.dll' / 'WinPixGpuCapturer.dll'` are optional profiler/debug DLL lookups, harmless.
- Reproducibility: a second `BuildCookRun` into a new folder succeeded with the same 48 files; they are **not bit-identical** (executable, pdb, pak, utoc differ; the executable also changed because a test was added between the runs).
- Default map unchanged (`Lvl_ThirdPerson`). Changing it is not required by M1 and would hide the template map; the start argument is the documented way.

### Tests run on 2026-10-22 (final)
Core 198 of 198 (Windows MSVC Debug, also in the clean copy); Unreal automation 17 of 17 (`SpnCore+SpnGame`, `-nullrhi`; also in the clean copy). Not run again: the four Fedora presets.

### Open manual checks
Camera at the wall (back to the wall, rotate); `Spn.Inventory` after `Spn.LoadWorld`; manual run in the editor (PIE) if wanted; real `git clone` build after you commit.

## 11. Complete final verification (2026-10-22, second audit) — status: CONDITIONAL PASS
Sources: automated tests, logs of the packaged build, rendered screenshots from `-game`/PIE runs with the RTX 3090 (viewed), and contact sheets taken from the developer's two OBS recordings (viewed at low resolution; no frame-accurate review). Legend as in section 10.

### Updated criteria
| # | Criterion | Result | Evidence / remaining gap |
|---|---|---|---|
| 1 | Runs without errors | **A** + **M** | 19 of 19 automated tests (PIE and `-game`); packaged build logs without `Warning:`/`Error:` except the intended refusal warnings of the missing/damaged-save tests. Manual run inside the editor (PIE) still not done. |
| 2a | Walk, sprint, jump | **A** + **M** | `M1Traversal`; BP values read from the Blueprint class: max walk speed 500, sprint speed 800, jump Z 500, step height 45, walkable angle 44.8 degrees (identical to the C++ defaults). |
| 2b | Ramp, stairs | **A** | `M1Layout`, `M1Traversal` (walked up without jumping). |
| 2c | Wall | **A** | blocks; crossed with an active jump; diagonal contact slides along the wall (not stuck). |
| 2d | Camera collision | **A** (new) | `SpnGame.TestMap.M1Camera`: real collision queries on the camera the player sees (overlap and line trace on the Camera channel, arm length bound, floor), 756 samples at wall, close wall and stairs, pitch -80 to +80, 720 fast random look inputs: 0 violations; negative control (`-SpnCameraNegativeControl`, collision off) fails with 322 violations. Screenshots viewed. **Open design point:** see finding 3. |
| 3 | Labelled placeholder | partly | unchanged |
| 4 | Packaged build | **A** + **M** | New package `C:\Projekte\_SPN_Builds\M1_audit`: `SpnGame.TestMap.PlayPickups` passes inside the packaged build; start with the map argument loads the M1 map, start without it loads `Lvl_ThirdPerson`. |
| 5 | Frame time | **A** | one series (README 26.5); not repeated after the camera fix (no render-cost change expected, not measured). |
| 6 | Clean checkout | **BLOCKED UNTIL COMMIT** (git clone); copy-based build **A** | see Git section below. |

### Interaction, inventory, save/load (packaged Development build, separate processes)
- `PlayPickups` (editor PIE and packaged): fresh state; E out of range (450 cm, range 250) does nothing; E in range takes `pickup_a` (1); three repeated E: still 1; `pickup_b`: `test_item = 3`; E on a taken pickup: still 3; `door_a`, `door_b` open; `door_locked` refuses; `Spn.Inventory` prints `test_item=3`; `Spn.SaveWorld`; door_a closed; `Spn.LoadWorld`: door_a/door_b open, locked door closed, both pickups taken and hidden, `test_item = 3`.
- Process 1 saved slot `spn_m1_audit` (file: three pickup records, `adjust ... i:1 i:2 i:3`, door_a and door_b open, `end 6`). **Process 2** (new game process): `Spn.Inventory` -> empty, revision 0; `Spn.LoadWorld spn_m1_audit` -> revision 0 -> 6, both pickups taken, door_a/door_b open; `Spn.Inventory` -> `test_item=3`.
- **Process 3:** loaded the slot, then a missing slot (`SlotNotFound`) and a truncated copy (`CorruptData truncated data at line 4`): both rejected, `Spn.Inventory` still `test_item=3`.
- The developer's own manual save (`debug`) also contains the final record `adjust ... i:2 i:1 i:3` -> the saved count was 3.
- Distinction: world state (facts) and inventory counter are verified from the save file and `Spn.Inventory`; the visual state (hidden pickups, door pose) from the automated actor checks, not from a visual review of the packaged game.
- `Spn.Inventory`, `Spn.SaveWorld` and `Spn.LoadWorld` are compiled only when `!UE_BUILD_SHIPPING` (`SpnWorldStateConsoleCommands.cpp`), so they exist in the Development package and are absent from a Shipping build. A Shipping build was not made; this is read from the code.

### Findings
| # | Severity | Finding | Cause | Files | Fix / regression test |
|---|---|---|---|---|---|
| 1 | medium | With the back to a wall the camera was squeezed to 33-60 cm from the pivot, i.e. inside the character's body: the screen showed the character's back/hips (seen in a rendered screenshot and in the developer's recording). | Spring arm shortens the arm against the wall; no handling of a very short arm. | `SupernaturalProtCharacter.h/.cpp` | Fixed: mesh hidden in the owner's view while the camera is closer than `HideMeshCameraDistance` (70 cm, placeholder). Test: `M1Camera` (mesh hidden/visible by arm length) and re-rendered screenshot. |
| 2 | low (tests) | Automation PIE sessions share one game instance, so world state of one play test leaked into the next (doors open at the start of the next test, order-dependent). | Test environment behaviour; real PIE restarts and new processes start empty (packaged log shows revision 0). | `Spn/Tests/SpnTestPlayReset.h`, `SpnDoorTestMapAutomation.cpp`, `SpnTestMapM1PlayPickupsAutomation.cpp` | Fixed: play tests reset the world state through the normal load path first. Full suite passes in alphabetical order (`PlayDoors` before `PlayPickups`). |
| 3 | medium (design, open) | Camera pops between "squeezed at the character" and "far above" near the 1 m wall: arm 59 cm at pitch -10, 400 cm with the camera 137 cm up at pitch -20 (wall); 34 cm -> 400 cm between -20 and -30 (close). Matches the high perspective seen in the recording. | The wall is lower than the camera boom can lift over; the arm length is a step function of pitch there. Not a collision error (camera never inside geometry). | - | **Not changed** (needs a decision, e.g. pitch limits, boom pivot, smoothing). Pitch limits are not overridden by the project (up to near-vertical views are possible; -80 was tested). |
| 4 | low | The floor is 40 x 40 m with no boundary; recordings show the void beyond its edge. Falling off was **not tested**; the map's kill height was not checked. | Placeholder map. | - | Not changed; suggest boundary or kill/respawn volume in a later map revision. |
| 5 | cosmetic | The ramp is a 20 cm slab with an empty space below; the 5 `FText` cook warnings remain. | Placeholder geometry / label text set from strings. | - | Not changed. |

### Movement values and later traversal (see Proposal 0009)
Jump is bound to Space and `Gamepad_FaceButton_Bottom` (DualSense cross) in `IMC_Default`; sprint (Shift) and interact (E) have no gamepad binding. Jump Z 500 gives an apex of about 127 cm; a 50 cm wall would need a much weaker jump or a vault move. Nothing was changed.

### Git
`.gitignore` ignores `*.uasset` and `*.umap`, but all existing Content (868 tracked files in the project, including `__ExternalActors__`/`__ExternalObjects__` and the test map) is already tracked through Git LFS rules in `.gitattributes`; no new Content asset was created, so nothing relevant is hidden. `Unreal/SupernaturalProt/Build/` only holds generated open-order logs. The M1 changes are uncommitted, so a real clone test is `BLOCKED UNTIL COMMIT`; after a commit (with `git lfs` active) a fresh clone should be buildable because every file the copy-based build needed is tracked.

## 12. Camera pop fixed and final checks (2026-10-22, third audit) — status: CONDITIONAL PASS
**Finding 3 of section 11 (camera pop) — cause and fix.** Cause: the spring arm sweeps a probe sphere (radius 12 cm) from the pivot to the desired camera position. At the 1 m wall the swept sphere clears the wall top only above a pitch of roughly -12 to -15 degrees, so the free arm length changes from a few centimetres to the full 400 cm within a fraction of a degree; the camera jumped between "pressed against the character" and "far and high". Fix (`SupernaturalProtCharacter.cpp/.h`): the boom's `TargetArmLength` acts as a cap that follows the collision result **down at once** (so nothing is clipped and shortening is never smoothed) and **grows back at most `CameraArmRecoverySpeed` (300 cm/s, placeholder; 0 = old behaviour)**. Mouse control, pitch range and the collision test are unchanged.

| Measure (test `SpnGame.TestMap.M1Camera`, wall, pitch -10 then -25) | Before (`-SpnCameraNoSmoothing`, old behaviour) | After |
|---|---|---|
| Arm in the first frame after swinging over the edge | 400 cm (pop) | 62-63 cm |
| Largest arm growth per frame | whole length in one frame | 2.5 cm per frame at the test frame rate (limit 300 cm/s times frame time) |
| Time until the full 400 cm | 1 frame | about 1.1 s |
| Swing back into the wall | immediate | immediate (arm under 100 cm after 0.1 s) |
| Camera-collision violations (overlap, line of sight, arm bound, floor, mesh visibility) | 0 | 0 over 1057 samples (including recovery and swing back) |

Rendered stills of the zoom-out (`SpnCam_recovery_0/1/2.png`, viewed with the RTX 3090): a smooth pull-back from a close view over the character to a view above the wall. Not reviewed: feel in motion at game speed; the value 300 cm/s is a placeholder for you to tune. Limitation: while the arm grows back the character can be seen from a short distance (about 70 cm and more, outside the body); the mesh stays hidden below 70 cm.

**Checks after the fix**
- Core 198 of 198; Unreal automation 19 of 19 (`SpnCore+SpnGame`); editor build and Development package (`C:\Projekte\_SPN_Builds\M1_final`) succeeded.
- Packaged build: `PlayPickups` passes inside it; a new process loads slot `spn_m1_audit` and shows `test_item=3` (it starts with `empty, revision 0`); a missing slot gives `SlotNotFound`, a truncated slot `CorruptData`, inventory unchanged; start without a map argument loads `Lvl_ThirdPerson`. Logs contain no errors, no crash dumps; the only warnings are the intended refusal messages and the 5 cook `FText` hints.
- **Not done:** manual run in the editor (PIE); real `git clone` test (BLOCKED UNTIL COMMIT); frame time not re-measured after the camera change.

**Open M1 blockers:** the clone test after your commit. All other criteria are met or documented. The manual editor PIE run is not a blocker if you accept the packaged-build play and the automated PIE tests as equivalent.

## 8. Non-goals
No new gameplay system, no vehicles, NPCs, character editor, final assets, map candidate or geography.
