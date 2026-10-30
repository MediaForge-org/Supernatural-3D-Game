# Night Build 01 — Completion & Stabilization: report (2026-10-30)

Baseline: `799a64e` (pushed). **Nothing was committed, pushed, tagged or released. `stash@{0}` untouched. `Core/` unchanged. No installed engine file modified. The Fab plugin was not touched.** All results below were produced in this session from this working tree; where something was not run it says so. Status words: PASS / FAIL / BLOCKED / NOT RUN / NOT IMPLEMENTED.

## 1. What was done

| Area | Result |
|---|---|
| Save/load: player pose | **PASS.** Optional section `player.pose` (see `ARCHITECTURE.md` §5). Position (canonical metres through `SpnWorldFrame`), heading, pitch, gait, crouch, level. Restored once the pawn of the right level exists; unusable pose → explicit notice, never a silent default. Legacy slots load and say so. |
| World-object persistence | **PASS for pickups and containers through real level recreation** (`SpnGame.NightBuild.ObjectsWorld`: take, refuse, transfer, save, back to the main menu, load, recreated actors match). Identity guard added. **Core `ObjectPersistence` NOT integrated, on purpose** — see §5. |
| No-teleport journey | **PASS (scripted OS input, closed loop).** `Tools/windows/nb01_run_journey.ps1`: all 20 steps with real keyboard + relative mouse input; no F5, no state injection. See §4. |
| Input routing | **PASS (simulated input).** 3 new automated tests; found and fixed a real bug (mouse capture mode inherited from the menu level). Physical keyboard/DualSense not verified. |
| Inventory UI 2.0 | **Mostly done** (list/grid, sort, category filter, grid navigation, weight/volume/condition in rows; tests + measurements). Equipment-panel redesign and responsive-layout verification **not done**. |
| Settings | **Extended:** music + effects volume (engine sound classes, no sounds exist yet — the page says so), UI scale. Persistence tested by ini reload, **not** across a real process restart. |
| Movement | **Stair camera preserved. The 5 known failures remain open** (`OPUS_ESCALATION.md` updated; no speculative fix attempted, as instructed). |

## 2. Bugs found by the new tests and the scripted run (all fixed)

1. **Mouse look only worked while a mouse button was held after New Game from the menu.** The menu level leaves the viewport's mouse-capture mode on `CaptureDuringMouseDown`; the gameplay level inherited it. Fix: `USpnUiSubsystem::ApplyGameplayInput`. Test: `SpnGame.Input.ModeAfterTravel` (verified to FAIL with the fix removed).
2. **The player's container could be missing** when the process-wide world-container registry held a stale id (the cupboard then looked like the player's body and `FindPlayerContainer` never created the real one). Fix: the session looks the container up by its fixed id. Found because `PoseWorld` failed only when run after other tests.
3. **Main menu: a remembered but disabled entry (Continue without a save) kept the selection**, so Down/Up could land on Quit. Fix in `USpnMenuScreen::SetItems`.
4. **Inventory screen: a sort/filter change did not redraw the rows** (the view-model reports one generic change). Fixed in the new `SyncViewChange` (found by `InventoryUi2.ViewSortFilter`).

## 3. Save compatibility and guarantees

* Container format `SPN-SAVESLOT 1` unchanged; `player.pose` is optional, version 1, checksummed like every section.
* **Old save, new build:** loads; no pose → player at the level start + notice `ui.pose.legacy`. Test: `SpnCore.Save.PlayerPose` (legacy part).
* **New save, old build:** the old build treats `player.pose` as an unknown optional section and carries it over unchanged when it re-saves. (A re-save by an old build therefore keeps a possibly stale pose; the next save by this build replaces it.) Not tested with a real old binary — follows from the existing "unknown optional sections are preserved" test `SpnCore.SaveSubsystem.UnknownSectionsArePreserved`.
* **Corruption:** a pose with valid checksums but bad content rejects the generation (previous generation tried) and changes nothing live — facts, map, inventory untouched, no listener called (`SpnCore.Save.PlayerPoseCorruptIsAtomic`). A newer `player.pose` version refuses load and overwrite. Catalog-fingerprint validation unchanged and still never auto-accepted.
* **Failure atomicity:** phase 1 (parse everything) before phase 2 (swap everything), as before; the pose is part of phase 1.
* **Not claimed:** guarantees against power loss (no fsync in the portable layer, as documented earlier), network/co-op authority.
* **Master consultation (CLAUDE.md §11):** `Planning/Master` is not available on this Windows machine (`Tools/master-search.sh` fails: planning master not available), so the save/persistence requirements could **not** be checked against the canon. The change is a proposal of this build (optional section, no change to existing formats) and is flagged for the developer's review.

## 4. Gameplay evidence

* Final run: `Unreal/SupernaturalProt/Saved/NightBuild01/completion-video/` (generated, not committed): `run.mp4` (1920x1080, 60 fps, 132.6 s), `steps.txt` (state after every step), 29 screenshots, `game.log`. Exact numbers in `QA_RESULTS.md` completion section B: 7 of 7 restore checks incl. the player position, navigation distance 50.0 m to 38.1 m, crate refused, one retried interaction (cupboard, second try).
* **What "no teleport" means here, precisely:** the script only READS the player pose (a QA feed `pose.txt`, `-SpnQaKeys`) and the F9 state dump; it never sets position, never presses F5. Walking = W held + relative mouse movement steering toward targets; interactions = E; menus = real keys. A mouse-look calibration step measures degrees per mouse count first. The run reports the largest position change between two samples (no jumps) and warns if a QA teleport appears in the log.
* **Scripted input, not manual play.** Failed or retried interactions are visible in `steps.txt` (`Try-Interact` retries up to 3 times and logs each try).

## 5. Open decisions for the developer

1. **Integrate Core `ObjectPersistence`?** Not done. It would store the state of catalog objects a second time (the facts already hold it) and only pays off with region streaming. Needs a decision on whether a second copy is wanted and on the `world.objects` section policy.
2. `player.pose` as the format for the player's rest position (master unreachable from this machine, see §3).
3. Axis preset / origin (`CQ-101`), still open; the pose follows whatever frame the map uses.
4. Music/effects: which audio architecture (sound classes vs. submixes vs. middleware) before real sounds arrive.
5. The 30 cm step rule (`OPUS_ESCALATION.md`).

## 6. Performance (Inventory UI 2.0)

`SpnGame.InventoryUi2.Performance` (headless, NullRHI: widget construction and layout logic, **no GPU rendering**), item stacks in one container, window of 100 widgets:

| stacks | open + switch to the container | list → grid | sort | category filter | selection step (avg) |
|---|---|---|---|---|---|
| 100 | 45.5 ms (includes the first widget construction) | 58.7 ms | 2.7 ms | 2.4 ms | 0.03 ms |
| 1 000 | 8.9 ms | 37.0 ms | 4.2 ms | 3.6 ms | 0.65 ms |
| 5 000 | 16.9 ms | 57.8 ms | 10.6 ms | 8.0 ms | 1.54 ms |

The widget count stays bounded by the window (100). A change in an unrelated container does not rebuild the visible list (`SpnGame.InventoryUi.Performance.LargeInventory`: 600 stacks, selection step 1.25 ms, mutation + refresh 2.6 ms). Not measured: rendered frame time of the grid, memory.

## 7. Files changed

New: `SpnCore/Public/SpnPlayerPose.h`, `SpnCore/Private/SpnPlayerPose.cpp`, `SpnCore/Private/Tests/SpnPlayerPoseAutomation.cpp`, `SupernaturalProt/Spn/Tests/{SpnNightBuildPoseWorldAutomation,SpnNightBuildObjectsWorldAutomation,SpnNightBuildInputRouterAutomation,SpnInventoryUi2Automation}.cpp`, `Tools/windows/nb01_run_journey.ps1`, this report.
Changed: `SpnSaveSubsystem.{h,cpp}` (pose section, `OnPoseLoaded`, `PoseProvider`, `InspectSlot` pose info), `SpnInventoryViewModel.{h,cpp}` (panel categories, getters), `SpnSessionSubsystem.{h,cpp}` (pose provider/apply, level choice, `StartNewGameInPlace`, player-container lookup), `SpnSettingsSubsystem.{h,cpp}`, `SpnWorldObjects.{h,cpp}` (identity guard), `UI/SpnUiSubsystem.{h,cpp}` (gameplay input mode, QA pose feed, settings rows), `UI/SpnMenuScreen.cpp`, `UI/SpnUiButton.{h,cpp}` (tile layout), `UI/SpnInventoryScreen.{h,cpp}` (toolbar, grid, sort, filter), `UI/SpnUiText.cpp`, `UI/SpnUiTextNightBuild.inc`, `Tests/SpnNightBuildAutomation.cpp`, `Tools/qa_inventory.json`, `Docs/Development/Gameplay-QA-Inventory.md` (generated), docs under `Docs/NightBuild01/`.

## 8. Test results (fresh, final tree)

| Suite | Result |
|---|---|
| Core MSVC Debug | PASS 493 / 493 |
| Core clang-cl Debug | PASS 493 / 493 |
| Unreal `SpnGame` + `SpnCore` headless | 130 tests: **128 PASS, 2 FAIL** (`Movement.Obstacles`, `Movement.StairsAndRamps`: known, open) |
| `SpnQA.*` rendered | PASS 219, **FAIL 2** (`Jump.late_jump_ledge_45_jog`, `late_jump_ledge_40_run`: known, open), NOT_IMPLEMENTED 3, NOT_TESTED 1 |
| Python | PASS 219 |
| QA inventory gate | PASS (27 systems, 0 problems) |
| Scripted journey (no teleport) | PASS (7 / 7 restore checks) |
| Physical DualSense | NOT RUN (no hardware) |
| Real process restart of the new settings | NOT RUN |
| Core `ObjectPersistence` / `world.objects` | NOT IMPLEMENTED (decision) |

## 9. Honest completion matrix

See `IMPLEMENTATION_STATUS.md` (packages A to Q). Summary: PASS A1, B, C, D, E, F, H, I, K, N (simulated input), O, Q; PARTIAL A3, G, J, L, M; FAIL (open, escalated) A2.

## 10. Suggested next milestones

`NEXT_STEPS.md`, section "After the completion pass".
