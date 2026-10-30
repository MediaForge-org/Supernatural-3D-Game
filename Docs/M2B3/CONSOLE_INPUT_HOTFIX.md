# M2B3.5 hotfix – console / text input vs. inventory hotkey

## Reported behaviour

In the editor the inventory opened by itself while a console command such as `Spn.Inv.Setup` or `Spn.Inv.List` was being typed; after the `.` the input could not be continued; Escape dismissed the state.

## Root cause (derived from the code, then reproduced by the regression steps)

`USpnInventoryUiSubsystem` registers an `IInputProcessor` (pre-processor). Pre-processors see every key **before** the focused widget. The old guard only asked the *viewport* console (`UConsole::ConsoleActive()`). Typing into any other Slate text field – the Output Log command box of the editor, search boxes – was not covered, so the **`I`** of `Spn.`**`I`**`nv.…` / `Spn.UI.`**`I`**`nventory` was taken as the open key, the inventory opened, took the keyboard (modal, `FInputModeGameAndUI`) and swallowed the rest. A second defect: while the inventory was open its modal key handling also swallowed the console key (tilde), so the console could not be opened at all and Escape closed the inventory instead.

Not verified: the exact editor widget the developer typed into. The reproduction below uses the viewport console and a Slate editable text; the Output Log box is covered by the same `EditableText` focus check but was not driven by an automated test.

## Fix (`Spn/UI/SpnInventoryUiSubsystem.cpp/.h`)

* `IsTextEntryFocused()`: true when the keyboard-focused Slate widget is an editable text / input box. `ProcessKeyDown` returns false (the key is not touched) while this is true or the viewport console is active.
* Keys listed in `UInputSettings::ConsoleKeys` are never consumed, also not while the inventory is open.
* Nothing else changed: the hotkey still works with the viewport focused, the open screen is still modal for all other keys, close restores the previous input mode.

## Verification – real session, `SpnQA.InventoryUi`

Run `Tools\run_gameplay_qa.ps1 -Suite InventoryUi`. New steps (all through `FSlateApplication`, i.e. the same path as the keyboard):

* a real `SEditableText` gets focus: `I` does not open the inventory and `IsTextEntryFocused()` is true; after the field is removed `I` opens it again.
* the **real viewport console** is opened with the console key and `Spn.Inv.Setup`, `Spn.Inv.List` and `Spn.UI.Inventory open` are typed **key by key** (key down, character, key up, then Enter). After every key the test checks that the inventory did not open. Execution is proven from the captured log (`Spn.Inv.Setup:` / `Spn.Inv.List:` lines) and, for the UI command, by the inventory opening exactly once after Enter.
* console while the inventory is open; Escape closes the inventory with the console closed; Escape closes only the console; five open (console key) / close (Escape) cycles leave `IsMoveInputIgnored` false.

Result: see the final table in `INVENTORY_UI.md` section 10 and the run reports; the run before the second fix of the test itself had 52 PASS / 1 FAIL (the failing check assumed a second console key press closes the console; in Unreal it switches from the input line to the large console – the check was corrected, not the product).

## Not covered

* The fix was not demonstrated with the `Output Log` box of a running PIE editor by an automated test (no way to drive the editor's own widgets here). A manual check is recommended: open the inventory with `I`, press `~`, type `Spn.Inv.List`, Enter.
* No before/after run with the old code was recorded; the old behaviour is derived from the code.

## Validation run of 2026-10-30 (all freshly executed after the last code change)

| Check | Result |
|---|---|
| Editor build `SupernaturalProtEditor Win64 Development` | succeeded |
| Core CTest, MSVC Release (fresh tree) | 493/493 |
| Core CTest, clang-cl Release (fresh tree) | 493/493 |
| `SpnCore` automation | 57/57 |
| `SpnGame.InventoryUi` automation | 12/12 |
| `SpnGame.Movement` (incl. new `SpeedAudit`) | 14 of 16; failing: `Obstacles`, `StairsAndRamps` (known M2A4 failures, unchanged) |
| `SpnQA.InventoryUi` (real session, incl. console steps) | PASS 53, FAIL 0 |
| Python tests | 219 OK |
| QA inventory gate | 24 systems, 0 problems |
| V2 `Full -Regression` | PASS 219, FAIL 2 (`Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run`), NOT_IMPLEMENTED 3 and NOT_TESTED 1 (documented gaps) |
| V3 Doors / Access / Traversal | 89 / 15 / 22 PASS, 0 FAIL |
| V3 MovementDemo | 26 PASS, 1 FAIL (`ledge_80_blocks_even_with_a_jump`, known flaky: the character stopped 4496 cm before the obstacle; the same suite recorded for the video had 27 PASS, 0 FAIL) |

No physical DualSense was connected; gamepad input is simulated. No packaged build was made or tested.
