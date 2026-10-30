# M2B3.5 — Functional Unreal inventory UI (UMG, keyboard, mouse, gamepad)

Status words: **Implemented / Tested / Proposed / Unverified**. Date: 2026-10-29. Base `6c7109a` plus the uncommitted M2B3.1–M2B3.5 changes. Nothing was committed or pushed; the Git stash, the installed engine and the isolated Fab plugin were not touched.

Master consultation (CLAUDE.md §11): this milestone is a presentation layer over the existing Core/view-model. It introduces **no inventory rule, item, name, icon, balance value or final UI design**. Layout, texts and key assignment are an **engineering proposal**, not master canon. Master basis: nothing found that fixes an inventory screen design; none was adopted.

## 1. What was delivered

| Item | Where | State |
|---|---|---|
| `USpnInventoryScreen` — the UMG screen (equipment, items, details, actions, capacity, container tabs, message line, hints, discard dialog) | `Source/SupernaturalProt/Spn/UI/SpnInventoryScreen.h/.cpp` | Implemented, Tested |
| `USpnUiButton` — one selectable line (item row, slot, tab, action, dialog button) with hover/selection/focus-ring/disabled states and icon-swatch fallback | `Spn/UI/SpnUiButton.h/.cpp` | Implemented, Tested |
| `USpnInventoryUiSubsystem` (per local player) + `USpnInventoryUiSettings` (open keys, language override) | `Spn/UI/SpnInventoryUiSubsystem.h/.cpp` | Implemented, Tested in a rendered session |
| `FSpnUiText` — German/English key table with fallback chain | `Spn/UI/SpnUiText.h/.cpp` | Implemented, Tested |
| Developer commands `Spn.UI.Inventory open|close|toggle`, `Spn.UI.Language en|de|auto` | `Spn/UI/SpnInventoryUiConsoleCommands.cpp` | Implemented |
| 12 widget automation tests `SpnGame.InventoryUi.*` | `Spn/Tests/SpnInventoryUiAutomation.cpp` | Implemented, passing |
| Rendered gameplay QA `SpnQA.InventoryUi` (36 checks, 11 real screenshots) and runner suite `InventoryUi` | `Spn/Tests/SpnQaInventoryUiAutomation.cpp`, `Tools/run_gameplay_qa.ps1` | Implemented, passing |
| Packaging stage setting for the JSON catalog | `Config/DefaultGame.ini` | Configured, **Unverified** (no packaged build made) |
| `DeveloperSettings` module dependency | `SupernaturalProt.Build.cs` | Implemented |

No save-format change was needed: the screen only reads/writes through the existing subsystems. The catalog fingerprint and compatibility policies of M2B3.4 are unchanged.

## 2. Architecture

```
 Core ── USpnInventorySubsystem ── OnInventoryChangedDetailed ──► USpnInventoryViewModel ──native delegates──► USpnInventoryScreen ──► USpnUiButton pools
                │                       (touched containers)            (panels, rows, selection,            (dirty flags, pooled widgets,
 USpnSaveSubsystem ── OnLoadFinished ───────────────────────────────►     actions, messages)                   windowed row list)
                                                                                 ▲
 Slate input pre-processor (USpnInventoryUiSubsystem) ── HandleKey / HandleAnalog ┘   mouse: USpnUiButton → screen intents
```

* The screen contains **no inventory rule**. Which actions exist, whether they are possible and why not comes from the view-model, which asks the Core with a dry run (`Probe`). Every action goes `screen → view-model → USpnInventorySubsystem::Apply → Core`; a rejected action changes nothing (the Core transaction is all-or-nothing) and the screen shows the Core's reason.
* The view-model is created by the screen (`Setup`) and shut down in `Teardown`; closing the screen unbinds every delegate, so a closed screen costs nothing and receives no events.
* The UMG tree is built purely in C++ (`WidgetTree->ConstructWidget<>`), no Blueprint asset is required.

### Widget hierarchy

```
UOverlay (root)
├─ UBorder                      dimmed backdrop (the game stays visible behind it)
├─ UScaleBox (ScaleToFit)       designed for 1920×1080, fits any resolution/aspect ratio
│  └─ USizeBox 1840×1000 → UBorder (frame) → UVerticalBox
│     ├─ header   UBorder → UHorizontalBox: title, TEST CONTENT badge, container tabs (USpnUiButton), "Esc"
│     ├─ body     UHorizontalBox
│     │  ├─ left   equipment panel (USpnUiButton per slot)  +  capacity panel (3× label/value/UProgressBar)
│     │  ├─ centre items panel: UScrollBox → UVerticalBox of USpnUiButton rows (+ empty text)
│     │  └─ right  details panel (name, description, stat lines)  +  actions panel (USpnUiButton per action)
│     └─ footer   message line + input hints (keyboard or controller)
└─ UBorder (modal dialog layer, collapsed unless a discard is pending)
```

Rows, tabs, slots and actions are **pooled**: widgets are created once and re-used (collapsed when unused), so selection changes and item operations do not create or destroy widgets.

### Update mechanism

The screen listens to the view-model's native delegates and keeps dirty flags:

| Event | What is refreshed |
|---|---|
| selection change | selection visuals of the visible rows, details, actions, equipment focus (rows are rebuilt only when a long list's window has to move) |
| `OnContainerChanged(id)` | tabs; rows + capacity only if `id` is the **active** container; equipment only if `id` is the primary container |
| inventory replaced (load, reset, catalog change) | everything |
| a change in a container the screen does not show | nothing (the view-model does not even notify) |

Long lists are windowed: at most `MaxRowsShown` = 100 row widgets exist; the window follows the selection and the wheel moves the selection.

## 3. Input

The screen is **not** a gameplay binding and changes none of the existing mappings (Enhanced Input contexts, `DefaultInput.ini`, DualSense: X jump, Circle dodge, Square interact/attack, Triangle heavy attack; keyboard E interact are untouched).

* **Opening**: `USpnInventoryUiSubsystem` registers a Slate input pre-processor. While the screen is closed it only looks for the open keys (`USpnInventoryUiSettings::OpenKey`, default `I`; `OpenGamepadKey`, default gamepad Special-Left = DualSense Create/Share) and **consumes nothing else**; it also ignores keys while the console is open. QA check `closed_ui_does_not_steal_gameplay_input` presses E, Space, W, Shift, V, the four DualSense face buttons and a trigger and verifies none is consumed.
* **While open**: the subsystem ignores move/look input on the controller, shows the mouse cursor and switches to `FInputModeGameAndUI`; the pre-processor routes keys and analog values to the screen so it works independent of widget focus. The screen is modal (everything not handled is swallowed).
* **Closing** restores: move/look input, cursor visibility, `FInputModeGameOnly`, and flushes held keys. QA check `input_state_restored` verifies it in the session.

| Action | Keyboard / mouse | Gamepad (DualSense) |
|---|---|---|
| open / close | `I` (configurable) | Create/Share (configurable) |
| move selection | Up/Down, PageUp/PageDown, Home/End, mouse click, wheel | D-pad, left stick |
| switch panel (equipment ↔ items ↔ actions) | Left/Right, right click on a row (actions) | D-pad left/right |
| previous/next container | Shift+Tab / Tab, click on a tab | L1 / R1 |
| use (equip/unequip, run action, confirm) | Enter, Space, double click on a row | Cross |
| back (actions → items, then close; cancels a dialog) | Esc, Backspace | Circle |
| quantity − / + | `-` / `+` | L2 / R2 |
| discard (asks first) | Delete | Square |
| scroll the list | wheel | right stick |

The hints in the footer follow the last device used (keyboard or controller). Focus is shown by a bright ring on the active row/slot/action; a selection outside the active panel keeps a thin accent outline. Mouse hover does not move the selection (no accidental selection while using the keyboard).

**Controller validation**: the gamepad path is validated with **simulated gamepad key events** (`Gamepad_DPad_*`, `Gamepad_FaceButton_Right`, shoulders) sent through `FSlateApplication`, the same entry point as a real pad. **No physical DualSense was connected or tested.** Analog stick stepping (threshold 0.55, repeat after 0.4 s then every 0.13 s) is implemented but only exercised with direct `HandleAnalog` calls, not on hardware. This criterion is therefore reported as *implemented and validated by simulation, not on hardware*.

## 4. Actions and safety

Equip / Unequip / Transfer (to any other bound container) / Split / Merge / Discard come from `USpnInventoryViewModel::GetAvailableActions()`. Unavailable actions are shown dimmed with the reason underneath ("not available: No room: too many different items."); activating one shows the same reason in the message line and changes nothing (QA check `rejected_action_changes_nothing` compares the Core revision). **Discard always asks first**; the dialog starts on *Cancel*, Esc/Circle cancels, nothing is discarded silently. Holding Enter does not repeat an action.

## 5. Localization

German and English from one key table (`FSpnUiText`): UI labels (`ui.*`), every view-model error key (`spn.inventory.error.*`, `spn.inventory.action.*`, `spn.save.*`), and the temporary test content (`item.spn_test:*`, `cat.*`, `slot.*`, `kind.*`). No user-facing text is hard-coded in widget logic. Fallback chain: active language → English → caller fallback (e.g. the view-model's own text) → the key itself. The language follows the game culture; `USpnInventoryUiSettings::LanguageOverride` or `Spn.UI.Language en|de|auto` force one. Item names from an optional `USpnItemPresentationSet` win over the built-in test names. Test `Localization.GermanEnglishAndFallback` checks that **every** key has both a German and an English text.

*Design decision / limitation*: an in-code table was used instead of Unreal `FText` string tables/gather because `.po` files can't be produced and reviewed in this environment. Moving to string tables later only changes `FSpnUiText::Get`.

## 6. Save/load, reset and reopening

* The open screen follows `OnInventoryReplaced` (load, reset, catalog change): rows, equipment, capacity and tabs are rebuilt, a stale selection is dropped, nothing from the previous state remains (test `SaveLoad.ResetAndRefresh`, QA `open_screen_refreshes_after_load`).
* A **failed load** leaves the display untouched (the save subsystem keeps the old state; test checks the same rows after loading a missing slot).
* Closing and reopening rebuilds from the current state (`Lifecycle.ReopenShowsCurrentState`).
* The view-model keeps selection across load when the stack still exists, otherwise clears it (M2B3.4 behaviour).

## 7. Test data, presentation assets, packaging

* The screen shows the temporary `spn_test:` catalog (`Content/SpnTest/Inventory/TestItemCatalog.json`) and says so with a **TEST CONTENT** badge when the catalog is flagged test data. No fake contents exist: every row is a Core stack.
* No icon textures exist: items get a muted colour swatch with the first letter (stable colour from the category id). An item with a `USpnItemPresentationSet` entry uses its display name; icon texture rendering is not wired yet (no assets to show).
* **Packaging (Unverified)**: the catalog is a plain JSON file read via `FPaths::ProjectContentDir()`; `DefaultGame.ini` now lists `+DirectoriesToAlwaysStageAsNonUFS=(Path="SpnTest/Inventory")` so a packaged build should copy it next to the content. **No packaged build was made**, so this is not claimed to work. In the editor and in `-game`/PIE the file is read from the project (verified by all tests and the rendered QA).

## 8. Performance

Measured in the editor (development build, `-nullrhi` for automation, rendered for QA), test `Performance.LargeInventory` (one unlimited test container with 600 stacks plus the three standard containers):

| Operation | Result |
|---|---|
| open screen + switch to the 600-stack container (4 container switches, includes first Slate construction) | about 42–48 ms total |
| selection step with the window moving (200 steps of Down) | about 1.2 ms average |
| a mutation in the shown container incl. refresh | about 2 ms |
| a mutation in a container not shown | no row rebuild (asserted by the test) |
| widgets for 600 stacks | 100 row widgets (windowed) |

Selection changes inside the window only touch the visible row states, details and actions. Full refreshes only happen on load/reset/open. **Limits**: numbers are single runs on the development machine, not a benchmark; the windowed list does not use native scroll for lists above 100 stacks (wheel moves the selection); the details block rebuilds its few stat lines on every selection change (cheap, not pooled).

A bug found by these measurements and fixed: a selection-only notification could leave the "full refresh" flag armed, causing a needless full rebuild on the next unrelated change.

## 9. Tests and evidence

* `SpnGame.InventoryUi.*` (12): construction empty; populated display equals Core; keyboard + D-pad navigation; equip/unequip + equipment panel; unavailable action explains the reason and a rejected action changes nothing (revision compared); container switching + transfer; discard needs confirmation (cancel keeps, confirm removes); save/load/reset refresh incl. failed load; reopen shows the current state; Back/Circle/I behaviour and modal dialog; German/English/fallback incl. all keys; performance with 600 stacks. They read the **text and state of the real widgets**, not the view-model.
* `SpnQA.InventoryUi`: one session on the V2 test map with the real character: closed UI does not steal gameplay keys → `I` opens (viewport, cursor, move input blocked) → navigate/inspect → equip with Enter → rejected transfer → Tab + transfer → real **Slate mouse** click and right click on a row → discard dialog (cancel) → **simulated** gamepad → Esc closes (input state restored) → save → reset → open (empty) → load while open (refreshes) → reopen → German. Screenshots are taken from the rendered game.
* Evidence of the final run: see section 10.

## 10. Final results

All numbers are from the final run on 2026-10-29 (Windows 11, Unreal 5.8.4, MSVC 19.51 Release, RTX 3090); nothing was committed.

| Check | Result |
|---|---|
| Real Editor build `SupernaturalProtEditor Win64 Development` | Succeeded |
| Core CTest, fresh MSVC Release tree | 493/493 passed |
| Unreal automation `SpnCore` | 57 performed, 57 passed |
| Unreal automation `SpnGame.InventoryUi` (new) | 12 performed, 12 passed |
| Unreal automation `SpnGame.Movement` | 15 performed, 13 passed, 2 failed: `Obstacles`, `StairsAndRamps` (known M2A4, unrelated) |
| Python tests (`python3.15 -I -m unittest discover`) | 219 passed |
| QA inventory gate `check_qa_inventory.py --check` | 24 systems, 24 with tests, 0 problems |
| `SpnQA.InventoryUi` rendered session (run `20261029-221010`) | PASS 36, FAIL 0, BLOCKED 0 |
| Gameplay QA V2 `Full -Regression` (run `20261029-214035`) | PASS 202, FAIL 2 (`Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run`, known M2A2/M2A4 issue), NOT_IMPLEMENTED 3, NOT_TESTED 1 |
| Gameplay QA V3 Doors / Access / Traversal / MovementDemo | PASS 89 / 15 / 22 / 27, FAIL 0 each |

`SpnGame.World` matched 0 tests (the filter does not exist); the other `SpnGame.*` groups run inside the V2 `Full` suite. The flaky `MovementDemo.ledge_80_blocks_even_with_a_jump` passed in this run.

### Screenshots (real captures of the rendered game, `Docs/M2B3/screenshots/`)

| File | Shows |
|---|---|
| `InventoryUi_01_open.png` | screen opened with `I` over the game |
| `InventoryUi_02_details.png` | keyboard navigation, details panel |
| `InventoryUi_03_equipped.png` | flashlight equipped: slot, EQUIPPED badge, Unequip action |
| `InventoryUi_04_rejected.png` | transfer into the full small box disabled, reason shown and in the message line |
| `InventoryUi_05_transferred.png` | after a transfer to the trunk |
| `InventoryUi_06_mouse.png` | after a real mouse click / right click |
| `InventoryUi_07_discard_dialog.png` | discard confirmation (starts on Cancel) |
| `InventoryUi_08_gamepad_hints.png` | controller hints after simulated gamepad input |
| `InventoryUi_09_empty_after_reset.png` | empty inventory after a reset |
| `InventoryUi_10_restored_after_load.png` | contents and equipment restored by loading while the screen is open |
| `InventoryUi_11_german.png` | German texts |

### Procedure to reproduce

```
# build + rendered session with screenshots (artifacts in Unreal/SupernaturalProt/Saved/GameplayQA/<run>/)
powershell -File Tools/run_gameplay_qa.ps1 -Suite InventoryUi -NoVideo
# widget tests
UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.InventoryUi" -TestExit="Automation Test Queue Empty" -unattended -nullrhi -log
# by hand: play the project, open the console and run  Spn.Inv.Setup  then press I  (or Spn.UI.Inventory open)
```

## 11. Known limitations / open points

* No physical DualSense validation (simulated events only); analog-stick stepping not exercised on hardware.
* Packaged build not made; the staging setting is unverified.
* No icon textures; swatch fallback only. No grid view (list only); sorting/filter controls of the view-model are not exposed in the UI yet.
* Localization is an in-code table, not Unreal string tables.
* The open key and the layout are proposals; there is no gameplay-mapping conflict check beyond the closed-state key test (the screen opens with `I` only when a player controller exists and the console is closed).
* Co-op: the screen is per local player and reads that player's game instance inventory; there is no networked or per-player inventory model yet.
* The M2A4 movement failures (`Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run`, `SpnGame.Movement.Obstacles`, `StairsAndRamps`, and the flaky `MovementDemo.ledge_80_blocks_even_with_a_jump`) are unrelated and tracked separately.
