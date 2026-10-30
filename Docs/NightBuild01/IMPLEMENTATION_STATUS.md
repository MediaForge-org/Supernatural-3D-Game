# Night Build 01 — implementation status per work package

## Completion matrix (after the completion pass, 2026-10-30) — supersedes the table below where they differ

PASS / PARTIAL / FAIL / NOT IMPLEMENTED. "Evidence" names a test or file; details in `QA_RESULTS.md`.

| Pkg | Topic | Status | Evidence / what is missing |
|---|---|---|---|
| A1 | Stair camera smoothing | **PASS** | `SpnGame.Movement.StairCamera`; unchanged in the completion pass |
| A2 | Known movement failures | **PARTIAL** (movement investigation 2026-10-30) | RESOLVED: `Obstacles`, `StairsAndRamps` (floor-rise rule), `ledge_80_blocks_even_with_a_jump` (wall-contact steering). OPEN, design decision: `late_jump_ledge_45_jog`, `late_jump_ledge_40_run`. `MOVEMENT_INVESTIGATION_REPORT.md` |
| A3 | Regression coverage | **PARTIAL** | all movement suites re-run (see `QA_RESULTS.md`); no new movement coverage beyond `StairCamera` |
| B | World map UI | **PASS** (test data) | journey run + `SpnCore.Map.*`; controller path simulated only |
| C | Minimap | **PASS** | `SpnCore.Map.Minimap`, journey screenshots |
| D | Navigation | **PASS** (straight line) | `SpnCore.Map.Navigation`; journey: distance 50 m to 38 m by walking toward the waypoint |
| E | Main menu | **PASS** | journey; a disabled remembered entry no longer keeps the selection (fixed) |
| F | Pause menu | **PASS** | `SpnGame.Input.RouterPriority`, journey |
| G | Settings | **PARTIAL** | persisted: master, music, effects (engine sound classes; **the project has no sounds yet**), UI scale, sensitivity, invert, language, graphics, minimap, camera smoothing. Persistence tested by ini reload (`SpnGame.NightBuild.SettingsAudioUi`), **not across a real process restart**. Not offered: subtitles / colour-blind modes / key rebinding (nothing to attach them to) |
| H | Save-slot UI | **PASS** | `SpnCore.Save.*`, journey |
| I | Unified HUD | **PASS** | journey screenshots |
| J | Interaction | **PARTIAL** | doors, pickups, containers; no new interaction rules |
| K | World pickups | **PASS** | `SpnGame.NightBuild.Pickup/PickupPersistence/ObjectsWorld`; identity guard |
| L | World containers | **PARTIAL** | contents + identity + recreation PASS (`ObjectsWorld`); Core `ObjectPersistence` / `world.objects` **NOT integrated** (decision, `COMPLETION_REPORT.md` section 5) |
| M | Inventory UI | **PARTIAL (mostly done)** | PASS: list/grid toggle, sort (6 modes, both directions), category filter, grid keyboard navigation, weight/volume/condition in rows, view options never touch the Core, performance measured (`SpnGame.InventoryUi2.*`). NOT done: redesigned equipment panel, responsive-layout verification (the screen scales to fit, only checked at 16:9), real item icons (swatch placeholders) |
| N | Input routing | **PASS (simulated input)** | `SpnGame.Input.RouterPriority/ConsoleTyping/ModeAfterTravel`; physical keyboard layouts and DualSense **not verified** |
| O | Integrated scenario | **PASS** (scripted, no teleport) | `Tools/windows/nb01_run_journey.ps1`, 7/7 restore checks incl. position |
| P | QA | see `QA_RESULTS.md` | |
| Q | Video | **PASS** (scripted input) | `Saved/NightBuild01/completion-video/run.mp4`, 1920x1080, 60 fps, real rendering |

---

(The table below is the original Night Build 01 status; where it says "not saved: the player position", "no teleport-free scenario" or "package M not started", the matrix above applies.)


Status words: **Done** = implemented, compiled, exercised in a real game session and covered by an automated test where one makes sense. **Partial** = works but with stated gaps. **Not done** = not started. Evidence names files under `Docs/NightBuild01/logs/` and `Unreal/SupernaturalProt/Saved/NightBuild01/` (screenshots, video).

| Pkg | Topic | Status | What exists / what is missing |
|---|---|---|---|
| A1 | Stair camera smoothing | **Done** | Exponential decay (`exp(-dt/τ)`, τ = 0.14 s, frame-rate independent), boom offset only, bounded to 24 cm, off while airborne / on teleports, setting `Stair camera smoothing`. Measured (`SpnGame.Movement.StairCamera`): max camera step per frame 15.00 cm → 4.14 cm on the 15 cm staircase; capsule step (15.00 cm) and end height (242.1) identical, i.e. physics untouched; lag decays after stopping. Not measured: ramps, sprint, mixed transitions (only the 15 cm staircase and a jump). |
| A2 | Known movement failures | **Not resolved** | `Obstacles` and `StairsAndRamps` reproduced (same single cause: 30 cm walks up), two different repair attempts, root cause narrowed (many small slide lifts, not `StepUp`), time-boxed, code reverted → `OPUS_ESCALATION.md`. The `late_jump_*` cases and the intermittent `ledge_80` case were not investigated. |
| A3 | Regression coverage | **Partial** | Existing movement suite re-run (baseline 14/16 pass, see `QA_RESULTS.md`); before/after measurement for A1 recorded; no new coverage for jump/sprint/diagonal beyond the existing tests. |
| B | World map UI | **Done** (test data) | Open/close (M / Triangle / button), player arrow + heading, zoom (wheel, Q/E, triggers), pan (drag, keys, stick), POI + marker symbols with discovery state (hollow = known, filled = visited), selection (click, Tab/R1), waypoint (right click, Enter/Cross, button, POI waypoint), removal (Del/Square/button), legend, region, scale bar, north marker, save/load refresh (`OnMapReplaced` + 150 ms text refresh). Mouse + keyboard + controller paths exist; **controller not tested on hardware**. Large-world: only the visible rectangle is queried and drawn. Test area only. |
| C | Minimap | **Done** | Circle minimap from `BuildMinimapFrame` (same map state), player + heading, nearby POIs and the waypoint, edge indicators, distance caption, north-up / heading-up setting, zoom setting, visibility setting, rebuild only on change (see ARCHITECTURE §2). Test `SpnCore.Map.Minimap` checks pixel positions for both orientations, zoom and the edge indicator. |
| D | Navigation | **Done** (straight line) | Active waypoint, bearing, distance, relative bearing, compass bar (N/E/S/W, ticks, waypoint diamond + distance), arrival radius with one-shot event, persistence through the map section, POI/marker-based destination. **No routing, no pathfinding** (stated in the UI). |
| E | Main menu | **Done** | Level `Lvl_SpnMainMenu` (engine primitives, moon light, fog, warm point lights — temporary), no pawn; New Game, Continue (disabled without a loadable save, shows the slot), Load Game, Settings, Quit. All entries functional (screenshots `01_main_menu.png`, `25_main_menu_with_continue.png`). No Impala model or production asset used. |
| F | Pause menu | **Done** | Resume, Save, Load, Settings, Return to Main Menu (confirmation), Quit (confirmation). World paused, input mode saved/restored, HUD suppressed; pause cannot open while the inventory or another page is open. |
| G | Settings | **Partial** | Graphics: display mode, resolution, quality preset, VSync, frame-rate limit (through `UGameUserSettings`, applied and saved at once). Audio: master volume only (drives the engine volume multiplier). Input: mouse and controller sensitivity, invert Y (wired into the look handlers). Interface: minimap visible / orientation / zoom, stair camera smoothing, language (auto/en/de). Persisted to the user ini; test `SpnGame.NightBuild.Settings` proves persistence, clamping, volume and language effect. **Not done:** music/effects volume (no audio buses exist), "UI navigation settings". Resolution/mode changes were not re-verified visually after applying. |
| H | Save-slot UI | **Done** | Slot list with metadata (date, world revision, inventory yes/no, map markers), new slot (next free `save_NN`), overwrite confirmation, error texts for every `ESpnSaveResult`, catalog-mismatch confirmation (never automatic), corrupt/newer/missing handling, previous-generation notice. Tests: `SpnCore.Save.InspectSlot`, `SpnCore.Save.CatalogMismatchFeedback`, `SpnCore.Map.Save`. No delete function (deliberately: nothing deletes saves). |
| I | Unified HUD | **Done** | One widget: compass, minimap, interaction prompt (moved from the component's own Slate widget; the old widget remains as fallback when no HUD exists), notifications (5 lines, 5 s fade), contextual hints (keyboard or controller labels by last input). No statistics shown that do not exist. |
| J | Interaction | **Partial** | Doors (existing), world pickups, world containers through the existing interaction component: prompt, 250 cm range, line of sight by sweep, "taken" availability, capacity feedback. No new interaction rules/verbs; the "context-sensitive attack" concept was not touched (no Square remap). |
| K | World pickups | **Done** | `ASpnWorldPickup`: dry run, flag, Core transaction, rollback; no duplication; no partial pickup; refusal keeps the object; notifications; persistence via the taken flag + inventory. Tests `SpnGame.NightBuild.Pickup` / `PickupPersistence`; real run: ration ×2, flashlight taken; the crate (60 l) refused. |
| L | World containers | **Partial** | `ASpnWorldContainer`: stable id, Core-backed contents, seed only once, container view in the inventory UI, Core transfers, capacity/category refusal, contents persisted (test `SpnGame.NightBuild.Container`; real run: bandages moved cupboard → player and restored after a load). **Missing dependency:** actor/world-object persistence (`ObjectPersistence`) is not integrated in Unreal: only contents persist, not the actor itself. |
| M | Inventory UI improvements | **Not done** | Only: container tabs filter, container view focus. No grid/list toggle, sorting, category filters, icons, layout work. |
| N | Input routing | **Partial** | Single router with fixed priority (ARCHITECTURE §3), console/text entry never hijacked (checked before every other rule), inventory ↔ pause ↔ map exclusion, input mode restored (observed in the run: `stack=0 paused=0` after closing). No dedicated unit test for the router yet; behaviour verified in the scenario state dumps. Gamepad navigation implemented, **no hardware validation**. |
| O | Integrated scenario | **Done** (scripted) | All 20 steps executed and verified by state dumps: `Tools/windows/nb01_run_integration.ps1`. Step 5 (stairs/ramps) is **not** part of the scenario; positioning between objects uses the QA hook F5. Details and results in `QA_RESULTS.md`. |
| P | QA | see `QA_RESULTS.md` | |
| Q | Video | **Done** (scripted input) | `run.mp4`, 1920×1080 at 60 fps, NVENC, real rendering. Scripted QA input, not manual play. |
