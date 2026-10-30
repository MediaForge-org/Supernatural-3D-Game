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
