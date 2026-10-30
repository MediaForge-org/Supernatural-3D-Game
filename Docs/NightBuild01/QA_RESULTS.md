# Night Build 01 — QA results (2026-10-30, Windows 11, UE 5.8, RTX 3090)

All numbers below were produced in this session; logs are in `Docs/NightBuild01/logs/`. Nothing was fabricated; where something was not run it says so.

## 1. Automated tests

| Suite | Command | Result |
|---|---|---|
| Portable Core, MSVC 19.51 Debug, `/W4 /WX` | `cmake` + `ninja` + `ctest` (`logs/core-msvc-debug.log`) | **493 / 493 passed** (baseline before the work: 493 / 493; `Core/` unchanged) |
| Core, Clang | not re-run (Core unchanged; earlier validation in `Docs/M2B3/WINDOWS_CORE_VALIDATION.md`) | — |
| Unreal editor build | `Build.bat SupernaturalProtEditor Win64 Development` (`logs/editor-build-15.log`) | Succeeded, 0 errors |
| Unreal automation, full `SpnGame` + `SpnCore` (`logs/regression-full-2.log`) | `Automation RunTests SpnGame+SpnCore` | **118 tests: 116 passed, 2 failed** — the failures are `SpnGame.Movement.Obstacles` and `SpnGame.Movement.StairsAndRamps`, identical to the baseline (`logs/movement-baseline.log`: 14/16 movement tests pass, the same two fail). **No new regression.** |
| of which new in Night Build 01 | | `SpnCore.Map.{Waypoint,Navigation,Minimap,Discovery,Save}`, `SpnCore.Save.{InspectSlot,CatalogMismatchFeedback}`, `SpnGame.NightBuild.{Pickup,PickupPersistence,Container,Settings,UiText}`, `SpnGame.Movement.StairCamera` — **13 of 13 passed** |
| Python tool tests | `python -I -m unittest discover -s Tools -p "test_*.py"` | **219 tests OK** |
| QA inventory gate | `Tools/check_qa_inventory.py` | **27 systems, 0 problems** (3 entries added to `qa_inventory.json` for the new systems, each with tests and a stated gap) |
| `SpnQA.*` gameplay suites (video QA of movement, doors, inventory UI) | **not re-run** | — |

Not weakened: no existing test was changed or loosened; no tolerance was widened.

## 2. Measurements

**A1 stair camera smoothing** (`SpnGame.Movement.StairCamera`, 15 cm staircase, jog, per-frame values):

| | camera step per frame (max) | capsule step per frame (max) | max lag | end height |
|---|---|---|---|---|
| smoothing off (before) | 15.00 cm | 15.00 cm | 0.00 cm | 242.1 |
| smoothing on (after) | **4.14 cm** | 15.00 cm | 22.61 cm (limit 24) | 242.1 |

The camera moves 72 % less per frame; the physical capsule is unchanged. One second after stopping the lag is below 0.5 cm; while airborne the lag is zero. Not measured: ramps, sprint, 25-30 cm steps, descending.

**Movement baseline vs now:** `Obstacles` / `StairsAndRamps` unchanged (30 cm crossed by walking), see `OPUS_ESCALATION.md`.

## 3. Integrated scenario (scripted OS input; `Tools/windows/nb01_run_integration.ps1`)

Final run: `Unreal/SupernaturalProt/Saved/NightBuild01/final-video/` (`steps.txt` = state after every step, 30+ screenshots, `run.mp4`, `game.log`). Saves of the run live in its own `saves/` folder (`-SpnSaveDir`), never in the player's real slots.

| # | Step | Verified by | Result |
|---|---|---|---|
| 1-2 | Launch, main menu | screenshot | menu rendered, keyboard navigation moves the selection |
| 3 | New game | state dump | all counters 0, no waypoint, cupboard seeded (3 bandages, 1 notebook) |
| 4 | Move the character | real `W`/`D` keys | position changed (-10550,-4700) → (-10061,-4463) |
| 5 | Stairs and ramps | — | **not part of this scenario** (existing movement suites; one of them fails) |
| 6 | Approach building / pickup | station F5 + `E` | rations ×2 in the inventory, `taken{yard=1}` |
| 7-8 | Open a door | `E` | `door_cottage1_open=1` |
| 9 | Enter the building | real `W` | x -9600 → -9285 (inside) |
| 10 | Pick up an item | `E` | flashlight=1 |
| 10b | Bulky item refused | `E` | crate=0, `taken{crate=0}` (60 l does not fit a 30 l body container), notification shown |
| 11-12 | Open a container, transfer | `E`, inventory UI keys | container view opened; bandage 3 moved cupboard → player (Core transfer): player 3, cupboard 0 |
| 13 | Open inventory, equip | `I`, `Enter` | `equipped:spn_test:item.flashlight@spn_test:slot.hand` |
| 14-16 | World map, waypoint, navigate | `M`, arrows, `Enter`, hold `W` | waypoint "Waypoint" at 150 m east; after 10 m of walking `dist=140.1` (decreasing), bearing 90 |
| 17 | Save | pause menu → Save → new slot | `activeSlot=save_01` |
| 18 | Back to main menu | pause → confirm | menu shows Continue |
| 18b | **New game** (proof state) | state dump | counters 0, waypoint none, door closed, cupboard reseeded |
| 19-20 | Continue / load, verify | state dump vs saved | **inventory, equipped flashlight, cupboard contents, taken flags, door state and waypoint all RESTORED, each different from the fresh game** (6 of 6 checks `RESTORED`; player position is not saved — documented) |

Hooks that are not real input: F5 (teleport next to the object and aim the camera), F9 (state dump to the log); both exist only with `-SpnQaKeys`.

## 4. Evidence index

* Video: `Unreal/SupernaturalProt/Saved/NightBuild01/final-video/run.mp4` — 1920×1080, 60 fps, 119.7 s, NVENC, scripted input (not committed; `Saved/` is ignored).
* Screenshots: same folder; menu (`01_`), HUD (`04_walked`), prompts, container view, inventory, map with waypoint, pause, save slots, settings, loaded game.
* Earlier runs (including the failed attempts) are kept next to it (`integration-*`, `video-run`, `smoke`, `run1`, `rectest`).
* Build logs `editor-build-*.log` (UTF-16), test logs `nb-tests-1.log`, `regression-full-2.log`, `stair-camera-1.log`, `movement-*.log`.

## 5. Not verified

Physical DualSense; resolution/display-mode changes after applying them; behaviour with a world larger than the 400 m test area; the `SpnQA.*` video suites; Clang builds of the Unreal module; packaged builds.

---

# Completion pass (2026-10-30) — fresh results of this session

The tables above are the earlier Night Build 01 results (historical). Everything below was run on the final working tree of the completion pass (HEAD `799a64e` + uncommitted changes); logs in `Docs/NightBuild01/logs/`.

## A. Automated tests

| Suite | Command / log | Result |
|---|---|---|
| Core, MSVC Debug (`/W4 /WX`) | `spn-build\nbc-core-msvc.cmd`, `logs/core-msvc-completion.log` | **PASS 493 / 493** (Core unchanged) |
| Core, clang-cl Debug | `spn-build\nbc-core-clang.cmd`, `logs/core-clang-completion.log` | **PASS 493 / 493** |
| Unreal editor build | `Build.bat SupernaturalProtEditor Win64 Development` (`logs/nbc-build-final.log`) | **PASS** (one pre-existing engine deprecation warning, `EKeys::Virtual_Accept`) |
| Unreal automation `SpnGame` + `SpnCore` (headless, NullRHI) | `logs/regression-final.log` | **130 tests: 128 PASS, 2 FAIL** — `SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps` (the known failures, unchanged, not weakened) |
| of which new in the completion pass (12) | | `SpnCore.Save.PlayerPoseFormat`, `SpnCore.Save.PlayerPose`, `SpnCore.Save.PlayerPoseCorruptIsAtomic`, `SpnGame.NightBuild.PoseWorld`, `SpnGame.NightBuild.ObjectsWorld`, `SpnGame.NightBuild.SettingsAudioUi`, `SpnGame.Input.RouterPriority`, `SpnGame.Input.ConsoleTyping`, `SpnGame.Input.ModeAfterTravel`, `SpnGame.InventoryUi2.ViewSortFilter`, `SpnGame.InventoryUi2.GridView`, `SpnGame.InventoryUi2.Performance` — **12 / 12 PASS** |
| `SpnQA.*` rendered (real game window, no video) | `Tools/run_gameplay_qa.ps1 -Suite Full -NoVideo -NoBuild`, `logs/spnqa-rendered-full.log`, report `Saved/GameplayQA/20261030-124911/report.md` | **PASS 219, FAIL 2, BLOCKED 0, NOT_IMPLEMENTED 3, NOT_TESTED 1.** FAIL: `Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run` (known, `OPUS_ESCALATION.md`). `SpnQA.InventoryUi` (incl. the console steps) PASSES rendered. The NOT_IMPLEMENTED / NOT_TESTED items are statements of the QA suites themselves (`SaveLoad.player_position_saved` and `save_ui_and_autosave` are about the console-command path of that level, the texts were updated; `Spawn.npc_vs_player`: no NPC pawn exists) |
| `SpnQA.*` headless (NullRHI) | `logs/spnqa-1.log` | 13 of 15 suites pass; `Jump` fails as above; **`InventoryUi` FAILS headless** (console key, mouse clicks and layout need a focused rendered window) and passes rendered. Not a regression of this pass; reported so nobody runs it headless and misreads it |
| Python tests | `python -I -m unittest discover -s Unreal/SupernaturalProt/Tools -p "test_*.py"` | **PASS 219** |
| QA inventory gate | `Tools/check_qa_inventory.py --check` (also run by `run_gameplay_qa.ps1`) | **PASS: 27 systems, 0 problems** (the generated `Docs/Development/Gameplay-QA-Inventory.md` was regenerated with `--write` and is part of the change) |

Negative check: with `ApplyGameplayInput` disabled, `SpnGame.Input.ModeAfterTravel` FAILS with "mouse capture mode is CaptureDuringMouseDown"; with the fix it passes (so the test can see the bug).

## B. Journey without teleport (scripted input)

Final run: `Unreal/SupernaturalProt/Saved/NightBuild01/completion-video/` — `run.mp4` (1920x1080, 60 fps, h264, 132.6 s, real rendering), `steps.txt` (the game state after every step), 29 screenshots, `game.log`, `pose.txt` (last pose), `turn_trace.txt` (steering trace), `saves/` (the run's own save root). Script: `Tools/windows/nb01_run_journey.ps1` — **scripted input, not manual play.**

| # | Step | Result |
|---|---|---|
| 1-2 | launch, main menu | menu rendered (Continue disabled without a save) |
| 3 | New game | state empty, cupboard seeded, pose = level start (-10550, -4700) |
| 4-6 | walk to the yard pickup, take it | rations x2 in the inventory (first try) |
| 7-8 | walk to the cottage, open the door | `door_cottage1_open=1` (first try) |
| 9 | walk through the doorway | inside the building (x -9325) |
| 10b | room 2: bulky crate | **refused** (`crate=0`, stays in the world) |
| 10 | room 2: flashlight | taken |
| 11 | room 1: open the cupboard | container view open (**needed a second try**, see `steps.txt`) |
| 12 | transfer bandages cupboard to player (inventory UI keys) | player 3, cupboard 0 (Core transfer) |
| 13-14 | close, open inventory, equip the flashlight | `equipped:...flashlight@...slot.hand` |
| 14b | leave the building on foot, walk to the street | on the street (-10200, -5440) |
| 15 | world map, waypoint with the keyboard crosshair | waypoint set |
| 16 | navigate on foot by the compass bearing | distance **50.0 m to 38.1 m** (decreased) |
| 17 | pause menu, save into a new slot | `activeSlot=save_01` |
| 18 | back to the main menu (confirm) | menu shows Continue |
| 18b | **new game** (proof state) | all counters 0, no waypoint, door closed, cupboard reseeded, pose = level start |
| 19-20 | Continue, verify | **7 of 7 checks RESTORED**, each different from the fresh game: player inventory, cupboard contents, taken flags, door state, waypoint, equipped item, **player position** `pos=(-10210,-4247,94)` identical before and after |
| - | no-teleport evidence | walked ~6 184 cm in total, largest position change between two samples 26.9 cm, F5 never pressed, no `NB01-QA teleported` line in the log |

The log of the run contains no `LogSupernaturalProt`/`LogSpnCore` warning or error.

## C. Performance (inventory UI, headless: logic and layout, no GPU)

See `COMPLETION_REPORT.md` section 6 (100 / 1 000 / 5 000 stacks: open, list-to-grid, sort, filter, selection step).

## D. Not verified in this pass

Physical DualSense; real process restart for the new settings (only ini reload); rendered frame time of the grid; layouts other than 16:9; packaged builds; Clang build of the Unreal module; a real old binary loading a new save (follows from the existing unknown-section test); the Core `ObjectPersistence` integration (not implemented).
