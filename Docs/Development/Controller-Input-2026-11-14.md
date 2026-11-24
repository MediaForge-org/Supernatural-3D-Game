# Input table: keyboard / mouse and PS5 DualSense (updated 2026-11-14, developer decision 3)

Sources, read on 2026-11-14:
- the mapping contexts `IMC_Default` and `IMC_MouseLook`, dumped read-only with `Unreal/SupernaturalProt/Tools/input_mapping_report.py`
  → `Saved/SpnWorldGen/input_mappings.json`;
- the runtime mappings in `ASupernaturalProtCharacter` (`Default*Key`, `ResolvePadActions`);
- the UI routers `USpnUiSubsystem`, `USpnInventoryUiSubsystem`, `SpnMapScreen`, `SpnInventoryScreen` and `SpnMenuScreen`.

**Status: checked in software (automated tests with injected key events), NOT confirmed on hardware.** No physical controller was
connected. The UE key names are the XInput/SDL names; that the DualSense delivers the touchpad click as `Gamepad_Special_Left` on this PC
(Steam Input / DS4Windows / native) has not been verified.

## Developer decision 3 (2026-11-14) and how it is implemented

| Decision | Implemented |
|---|---|
| Triangle stays reserved for a later heavy attack | Triangle does nothing in gameplay (no map, no force). Inside the map screen it no longer closes the map either. |
| Touchpad = map | The touchpad (`Gamepad_Special_Left`) opens the map in gameplay and closes it again on the map screen. |
| Options = pause | Unchanged (it already was). |
| L3 = sprint or gait change | Tap L3: sprint on (the toggle ends at the next tap or 0.3 s after the stick is let go). Hold L3 for 0.5 s: next gait (walk → jog → run), with a short HUD note. |
| Check conflicts, keep the keyboard | See the conflict list below. The keyboard is unchanged: E, F (hold), Shift (hold), V, M, I, Esc. |

What had to move because of it (my choices, PROVISIONAL, one property each):
- **Force (break a door open) on the pad: hold Square** (0.4 s, then the 1.6 s break). Square is Interact (tap) and the context action in
  the combat plan; a hold only forces where forcing is offered (a locked, breakable door in focus); elsewhere the hold does nothing.
  Property `DefaultForceGamepadKey` / `ForcePadHoldSeconds`.
- **Inventory on the pad: D-pad up** (was the touchpad). Inside the inventory the D-pad navigates, so it does not close it; Circle
  (back) or Options closes it. Property `USpnInventoryUiSettings::OpenGamepadKey`.

## Table

| Function | Keyboard / mouse | DualSense (UE key) | Implemented |
|---|---|---|---|
| Move | W A S D, arrow keys | left stick (`Gamepad_Left2D`, dead zone) | yes (`IMC_Default`) |
| Camera | mouse (`IMC_MouseLook`) | right stick (`Gamepad_Right2D`) | yes |
| Jump / ledge assist | Space | Cross (`FaceButton_Bottom`) | yes; in front of an obstacle the press starts the ledge assist (`Ledge-Assist-2026-11-14.md`) |
| Interact | E | Square tap (`FaceButton_Left`) | yes |
| Force (break a door open) | F held | **Square held** (0.4 s + break time) | yes (runtime action with an Enhanced Input hold trigger) |
| Sprint | Left Shift held | **L3 tap = toggle** | yes |
| Change gait (walk / jog / run) | V | **L3 held 0.5 s** | yes |
| Map | M | **Touchpad** (`Special_Left`) | yes (UI router) |
| Inventory | I | **D-pad up** | yes (`USpnInventoryUiSubsystem`) |
| Pause menu | Esc | Options (`Special_Right`) | yes |
| Menu: move / confirm / back | arrow keys, Enter, Esc / Backspace | D-pad or left stick, Cross, Circle | yes |
| Map screen | arrow keys / WASD pan, mouse, M or Esc close | left stick pan, L2/R2 zoom, L1/R1 next entry, Cross waypoint, Square remove, L3 centre, **touchpad or Circle close** | yes (hint line `ui.map.hints.pad` updated) |
| Inventory screen: change container | Tab / Shift+Tab | L1 / R1 | yes |
| Heavy attack (planned: Triangle) | – | Triangle reserved | not implemented (no combat system) |
| Dodge (planned: Circle) | – | – | not implemented; Circle is free in gameplay |

## Conflicts checked

- **Square**: tap = Interact, hold = Force. A tap at a locked door gives the "locked" answer and never breaks it; only the hold breaks it
  (tested). Interact fires on the press, so a hold also gives the "locked" answer first; that is the real door answering and stays.
- **L3**: in gameplay only sprint / gait; on the map screen it centres the map; in the inventory it keeps its screen function. The sprint
  toggle is cleared by Shift release as well, so keyboard and pad never leave a sprint stuck on.
- **Touchpad**: map only. It no longer opens or closes the inventory.
- **D-pad up**: opens the inventory in gameplay; inside the inventory, the map and the menus it navigates as before.
- **Triangle**: free in gameplay. The inventory screen still uses it for one of its own actions (unchanged screen key).
- **Enhanced Input consumption**: the pad actions sit in their own runtime context above the Interact context and do not consume the
  key, so Square still reaches Interact. (Found by the test: the first version consumed nothing, but a one-shot hold trigger reported
  "completed" right after it fired and ended the break at 1 %; fixed with a repeating hold.)

## Tests (software only)

- `SpnGame.NightBuild04.InputKeysAreSeparate` (headless): updated to the new decision (Force on the pad = Square held, Triangle unused by
  Interact / Force / Sprint, L3 = sprint / gait). It still checks that the keyboard keys of Interact and Force are separate.
- `SpnWorldQA.RouteControls` (rendered, in `-Suite World`): Triangle does nothing; touchpad opens and closes the map; D-pad up opens the
  inventory and Circle closes it; Options opens and closes the pause menu; L3 tap → sprint speed 800 cm/s; letting go of the stick ends
  the toggle; L3 hold → next gait; Square tap at a locked, breakable test door → still locked; Square held → broken and open.
  Gameplay keys are injected through the player controller, UI keys through Slate.

## History

Night build 2026-11-14 (before the decision): Triangle was Force and Map at once and could never force a door; a first fix let Triangle
force while a breakable door was in focus. That fix is superseded by the layout above (`USpnUiSubsystem::IsForceTargetInFocus` is no
longer used by the router).

## L3 double assignment: possible usability problems (developer order 2026-11-14, analysis only, nothing re-assigned)

The layout above stays as decided ("provisional"). Checked from the code (`SupernaturalProtCharacter.cpp`: `DoSprintPadPressed / Released`,
`TickPadSprint`) and against the movement code; not tested on hardware.

| # | Problem | Why (code) | Severity | Possible remedy (not done, needs your decision) |
|---|---|---|---|---|
| 1 | **Sprint starts late.** The tap is recognised on RELEASE (a hold would mean gait). | release decides tap vs hold | low: a click is usually 80–150 ms; noticeable when you sprint off from a standstill | sprint on press and undo it if the hold reaches 0.5 s (a short sprint burst then precedes a gait change) |
| 2 | **Clicking the stick while steering is error-prone.** Hard steering can click L3 by accident (unwanted sprint); holding L3 for 0.5 s while steering is awkward, and the direction drifts. | L3 is the steering stick itself | medium: a known general issue of stick clicks | a settings option "sprint: toggle / hold" and moving the gait change elsewhere (see 4) |
| 3 | **A hold during a running sprint changes the gait and keeps the sprint.** The gait change is invisible until the sprint ends. | the hold sets `bPadHoldUsed` and leaves the toggle as it was | low to medium: surprising | end the sprint on a gait change, or ignore the hold while sprinting |
| 4 | **The gait change is largely redundant on the pad.** The stick deflection already scales the speed (`CalcVelocity`: target speed = gait speed × `ComputeAnalogInputModifier()`): half deflection at the jog gait is about walking pace. | analog input | design note | keep L3 hold only for "run" as the base gait, or drop the pad gait change and rely on deflection + sprint |
| 5 | **Cycling needs several holds.** Walk → Jog → Run → Walk; from Run back to Jog takes two holds (> 1 s). | `CycleBaseGait` | low | hold = toggle between two gaits only |
| 6 | **The sprint ends at every short stop.** 0.3 s without stick input ends the toggle (door, look around, narrow passage). | `SprintToggleReleaseSeconds` | low; intended, but it may annoy | a longer time (0.6–1.0 s), or end it only on speed < walk |
| 7 | **No visible state.** Neither the toggle nor the gait is shown, apart from a short note on a gait change. | HUD | low | a small sprint / gait icon on the HUD |
| 8 | **Accessibility.** Stick clicks are hard for some players. | – | medium | the "toggle / hold" option of 2, and later free re-mapping (combat doc COMBAT-DEC-ST-001: full controller support with re-mapping is decided) |

Not affected: the map screen uses L3 to centre (another context), the inventory uses it inside its own screen, Shift on the keyboard is
unchanged, and the toggle is cleared when Shift is released (no stuck sprint).

Recommendation for the next decision (nothing changed now): keep L3 tap = sprint toggle. Make "sprint: toggle / hold" a setting. Decide
on the hold for gait after a hardware test (problems 2, 4).
