# Night Build 01 — integrated gameplay vertical slice (test level)

Date: 2026-10-30. Baseline: `14b839b` (the brief named `6c7109a`; the M2B3.4/M2B3.5 work it called "uncommitted" was already committed and the working tree was clean — only `Unreal/SupernaturalProt/Tools/__pycache__/` was untracked). `stash@{0}` was not touched. **Nothing was committed, pushed, tagged or released. Nothing under `Core/` was changed.** All changes are uncommitted in the working tree; the list is in `IMPLEMENTATION_STATUS.md` and `git status`.

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
| `QA_RESULTS.md` | commands, exact counts, scenario results, measurements |
| `KNOWN_ISSUES.md` | unresolved failures, limitations, things not verified |
| `NEXT_STEPS.md` | proposed next work |
| `OPUS_ESCALATION.md` | the unresolved 30 cm walking problem with all evidence |
| `logs/` | build logs, test logs, scenario logs |

Scripts: `Tools/windows/nb01_drive.ps1` (OS-level input, screenshots, recording), `nb01_run_integration.ps1` (the 20-step scenario), `nb01_run_smoke.ps1`; level generator `Unreal/SupernaturalProt/Tools/create_spn_main_menu_map.py`. Run the scripts with Windows PowerShell 5.1 (`powershell.exe -ExecutionPolicy Bypass -File ...`).

## Honest summary

Delivered end to end and exercised in a real, rendered game session: main menu, pause menu, settings, save slots (list, new slot, overwrite confirmation, catalog-mismatch confirmation, corrupt/missing handling), world map, minimap, compass, waypoint with straight-line navigation and arrival, HUD, interaction prompts, inventory-backed pickups and containers (capacity refusal, no duplication, persistence), stair camera smoothing. **Not delivered:** the 30 cm walking failures (unresolved, escalation written), the other late-jump/ledge investigations, inventory UI improvements (package M), music/effects volume, UI navigation settings, hardware controller validation, saving the player's position.
