# Night Build 01 — integrated gameplay vertical slice (test level)

Date: 2026-10-30. Baseline: `14b839b` (the brief named `6c7109a`; the M2B3.4/M2B3.5 work it called "uncommitted" was already committed and the working tree was clean — only `Unreal/SupernaturalProt/Tools/__pycache__/` was untracked). `stash@{0}` was not touched. **Nothing was committed, pushed, tagged or released. Nothing under `Core/` was changed.** All changes are uncommitted in the working tree; the list is in `IMPLEMENTATION_STATUS.md` and `git status`.

## Completion pass (2026-10-30) — read `COMPLETION_REPORT.md` first

The vertical slice is now persistent and teleport-free: the player pose is saved (`player.pose`), pickups and containers survive a real level recreation, the 20-step journey runs with real keyboard and mouse input and no F5 shortcut (`Tools/windows/nb01_run_journey.ps1`), the input router has dedicated tests (and a real mouse-capture bug was found and fixed), the inventory screen has list/grid, sort and category filter, and the settings gained music / effects volume (no sounds exist yet) and UI scale. **Still open:** the five movement failures (`OPUS_ESCALATION.md`), Core `ObjectPersistence` integration (a decision), hardware gamepad validation, equipment-panel redesign and icons. Completion matrix: `IMPLEMENTATION_STATUS.md`. Results: `QA_RESULTS.md`. Video: `Unreal/SupernaturalProt/Saved/NightBuild01/completion-video/run.mp4` (scripted input, not committed).

## What you can do now

Launch the game on the main menu level and play the whole loop with keyboard and mouse:

```
UnrealEditor.exe Unreal\SupernaturalProt\SupernaturalProt.uproject /Game/SpnTest/Lvl_SpnMainMenu -game -windowed -ResX=1920 -ResY=1080
```

Main menu → New Game → the test district (`Lvl_SpnMovementTestV3Interiors`) with HUD (compass, minimap, hints) → walk to the yard pickup and the cottage (E: take, open the door, open the cupboard, move items in the inventory screen, equip) → `M` map, place a waypoint, walk toward it → `Esc` pause, save into a slot → main menu → Continue → everything is back. Keys: `M` map, `I` inventory, `Esc` pause / back, `E` interact, arrows/Enter/Esc in menus; the map and menu footers list the rest.

The flow is **opt-in** (it starts with the menu level, `-SpnGameFlow`, or the console command `Spn.Game.Enable`); all earlier levels and tests behave as before.

## Documents

| File | Content |
|---|---|
| `IMPLEMENTATION_STATUS.md` | per work package: done / partial / not done, with evidence |
| `ARCHITECTURE.md` | layers, single source of truth, input priority, persistence, open decisions |
| `COMPLETION_REPORT.md` | the completion pass: what was done, bugs found, save compatibility, open decisions, performance, files |
| `QA_RESULTS.md` | commands, exact counts, scenario results, measurements |
| `KNOWN_ISSUES.md` | unresolved failures, limitations, things not verified |
| `NEXT_STEPS.md` | proposed next work |
| `OPUS_ESCALATION.md` | the unresolved 30 cm walking problem with all evidence |
| `logs/` | build logs, test logs, scenario logs |

Scripts: `Tools/windows/nb01_drive.ps1` (OS-level input, screenshots, recording), `nb01_run_journey.ps1` (the 20-step journey WITHOUT teleport, closed-loop steering), `nb01_run_integration.ps1` (the earlier scenario with F5 stations), `nb01_run_smoke.ps1`; level generator `Unreal/SupernaturalProt/Tools/create_spn_main_menu_map.py`. Run the scripts with Windows PowerShell 5.1 (`powershell.exe -ExecutionPolicy Bypass -File ...`).

## Honest summary

Delivered end to end and exercised in a real, rendered game session: main menu, pause menu, settings, save slots (list, new slot, overwrite confirmation, catalog-mismatch confirmation, corrupt/missing handling), world map, minimap, compass, waypoint with straight-line navigation and arrival, HUD, interaction prompts, inventory-backed pickups and containers (capacity refusal, no duplication, persistence), stair camera smoothing. **Not delivered (updated in the completion pass):** the movement failures (unresolved, escalation written), Core ObjectPersistence integration (decision pending), hardware controller validation, a redesigned equipment panel and item icons. Saving the player position, the sort/filter/grid inventory controls and music/effects volume were delivered in the completion pass.
