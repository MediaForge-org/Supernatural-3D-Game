# Gameplay demonstration video (M2A4.2 / M2B3.5)

File: `Docs/M2B3/Demo/SPN_M2A4-2_M2B3-5_gameplay_demo_1080p60.mp4` (`*.mp4` is git-ignored; do not commit or push it without the developer's decision).

| | |
|---|---|
| Duration | 621.4 s (about 10 min 21 s) |
| Resolution / frame rate | 1920x1080, 60 fps, H.264 (libx264, CRF 20, yuv420p), no audio |
| Size | about 222 MB |
| Recording | Unreal Engine 5.8.4 game process (`-game`, windowed, rendering on the RTX 3090), the client area captured with FFmpeg `gdigrab` by `Tools/run_gameplay_qa.ps1`; chapter bars (first 6 s of each chapter) added afterwards with FFmpeg `drawtext`; chapters joined without re-encoding. |
| Source runs | Locomotion `20261030-013146`, MovementDemo `20261030-013514`, Traversal `20261030-013739`, Doors `20261030-014004`, Access `20261030-014306`, InventoryUi `20261030-013358` (under `Unreal/SupernaturalProt/Saved/GameplayQA/`, with `report.md`, `results.json`, screenshots) |

## What "real" means here

Everything is the running game: real character, real `CharacterMovementComponent`, real collision, real UMG inventory, real save/load. The **input is scripted** by the QA harness (`SpnQA.*` automation tests drive `DoMove`/sprint/jump/interact, and for the inventory real Slate key and mouse events, and the gamepad as simulated key events). Nobody played with a keyboard or a controller during the recording. The coloured lines at the top left are the QA recorder's live check results with measured values (e.g. "speed 800 cm/s, at least 760 expected"); they are not mock-ups. The environment is the temporary grey-box test maps; the items are the temporary `spn_test:` items.

## Chapters

| Time | Chapter | Shows |
|---|---|---|
| 0:00 | Title card | project, milestone, how the footage was made |
| 0:06 | 1 Locomotion | walk, jog, run, sprint, stops, turns; speed values in the overlay |
| 1:13 | 2 Movement demo | sprint, stop, turns, reversal in the street of the V3 test map |
| 3:25 | 3 Continuous traversal | stairs, ramps and building interiors without teleporting |
| 5:36 | 4 Doors | doors, interaction reach, enterable test buildings (89 checks) |
| 8:23 | 5 Building access | locked and key doors |
| 9:36 | 6 Inventory | open with I, real items, details, equip, containers and transfer, discard confirmation, rejected actions with reasons, mouse, simulated gamepad, save, reset, load, German; at the end the **console hotfix**: `Spn.Inv.Setup`, `Spn.Inv.List`, `Spn.UI.Inventory open` typed key by key through the viewport console |

(Chapter times are approximate: sum of the segment lengths.)

## Not in the video / not implemented

* Driving, NPCs, dialogue, evidence, monsters: not implemented yet.
* Minimap, WorldFacts, navigation markers have automated tests but were not part of these suites and are not shown.
* Inventory: no icons (colour swatches), no grid view, no sort/filter UI.
* No jump / ledge chapter: the jump QA still contains the two known failures (`late_jump_ledge_45_jog`, `late_jump_ledge_40_run`), so it was left out instead of showing a failing scene.
* The movement speed audit (`SpnGame.Movement.SpeedAudit`) has no footage; it is a numeric test (`Docs/M2A4/MOVEMENT_SPEED_AUDIT.md`).

## Known issues visible in the footage

* The QA overlay text is drawn over the game, in the inventory chapter also over the inventory panels (the recorder clears it only for the screenshots).
* Step-ups on stairs move the camera by the whole riser height in one frame (see the speed audit); visible in the traversal and doors chapters.
* The in-video chapter bars cover the lower 150 px for 6 s at the start of each chapter.
