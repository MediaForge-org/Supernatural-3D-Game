# Unstable `SpnQA.Doors.*.opens` — cause and options (2026-11-13)

Technical analysis for developer review. Nothing in the door system was changed by it; the fix is a gameplay decision (CLAUDE.md §3/§7).

## Symptom

`SpnQA.Doors.<Building>.opens` fails for 5–8 of the 10 district doors per run, and **which** doors fail changes from run to run:

| Run (2026-11-13) | Failing `opens` | Passing `opens` |
|---|---|---|
| Doors 04:02 | House1, Cottage2, StorageHall, CourtHouse, Office1, Cottage3, TestHouse | Cottage1, Workshop, TestHut |
| Doors 04:05 (`r.Nanite.Foliage` off) | Cottage1, House1, Cottage2, Workshop, Office1, Cottage3, TestHouse | StorageHall, CourtHouse, TestHut |
| Quick 05:31 | Cottage1, Cottage2, Office1, Cottage3, TestHouse | the others |

The reference run of 2026-11-02 (before the visual-build work) shows the same picture. The world/vegetation changes and
`r.Nanite.Foliage` are not involved (identical results with it switched off).

Failure detail, always the same shape: `E pressed but the door did not open (IsOpen 1, angle -59)` — the door state is *open*, the leaf
stopped at 57–69° instead of 90°.

## Cause (proven)

1. **The game log of every failing door says** `SpnTestDoor_NN (<id>): the swing is blocked by a pawn, waiting` — written by
   `ASpnTestDoor::Tick` (NightBuild05) when the swept leaf would overlap a pawn.
2. **The pawn is the player who opened it.** District doors swing **outward** by design (`Tools/spn_district_layout.py`,
   `door_open_angle`: an inward leaf blocked the 140 cm stair halls), i.e. towards the person standing outside.
3. **The player stands at the edge of the swing.** The test walks until 200 cm from the leaf centre, releases W and the character glides
   on. Cottage1 example: one run stopped 127 cm in front (door opened in 0.49 s to 89°: PASS), another 123 cm in front (leaf stopped at 59°:
   FAIL). Geometry: hinge about 50 cm to the side of the centre line, leaf 100 cm wide → at about 60° the free edge comes within
   36–40 cm of the capsule centre, the capsule radius is 35–42 cm. A few centimetres of glide decide the result: **timing component**.
4. Before NightBuild05 the leaf ignored pawns (and could push one); the `opens` tests were written for that behaviour. NightBuild05 made
   "a door never pushes a pawn" a rule (`SpnGame.NightBuild05.DoorNeverPushesAPawn`, `DoorStopsAtPawnAtAnyFrameRate`) and applied it
   to opening as well.

Classification: **interaction/design conflict** (outward door + stop-at-pawn + opener standing in the swing), made **flaky by timing**
(glide distance). Not a state or initialisation problem (`IsOpen` is correct, the target angle is correct, the leaf resumes when the
way is free).

## Effect in play

A player who walks up to an outward door and presses E at a normal distance (about 1.2 m) gets a door that hits them and stays at
about 60°; the remaining gap (about 50 cm) is too narrow to pass, and the door continues only after the player steps back. This is
a real usability defect, not only a test problem.

## Options (developer decision 2026-11-13: option A, implemented — see "Implementation" below)

| # | Change | Effect | Concerns |
|---|---|---|---|
| A | **The opener steps back automatically** a short distance (out of the swing) before/while the door opens (as in many third-person games) | door opens fully, never pushes anyone, test stable | new movement behaviour (short forced move); needs a design OK |
| B | The opener is ignored by the opening sweep; the leaf passes through the opener's capsule | simple | visual clipping, and the capsule is pushed by the leaf collision = breaks "never pushes a pawn" |
| C | Doors open away from the opener (double-acting) | no conflict | contradicts the outward-swing reason (narrow halls); not physical for framed doors |
| D | E is refused while the opener stands inside the swing (feedback "step back") | consistent with the pawn rule | annoying in play; test would need a new approach distance |
| E | Keep the behaviour, change the test to stand outside the swing | test stable | the in-play defect remains; the user asked not to fix tests by relaxing them |

Recommendation: **A**, limited to the player opening a door (NPCs already never stand in the swing because they cannot open doors yet,
`Docs/NightBuild05/KNOWN_ISSUES.md` item 9). Acceptance: the existing `SpnQA.Doors` and NightBuild05 door tests unchanged and green over
three consecutive runs.

## Same cause elsewhere

`SpnQA.HouseJourney.archive_door_opens` (FAIL, leaf 39–40/90) and the following `HouseJourney.journey` (BLOCKED, stuck 98 cm before the
next waypoint) show the same log line for `archive_door` in every failing run (2026-11-02 and 2026-11-13 13:14). When the leaf happens to
clear the player, the journey passes (2026-11-13 04:34). Option A would fix it as well (checked in the Full run of the acceptance, see
`Artifacts/VisualProgress/PROGRESS.md`).

## Verification done

- Three Doors/Quick runs on 2026-11-13 (results above), one with `r.Nanite.Foliage=False` to exclude the world changes.
- Log lines and report positions compared per door (`Saved/GameplayQA/20261113-040250`, `-040537`, `-053107`).

## Implementation (option A, developer decision 2026-11-13)

Decision text: step back intelligently only when needed, a short natural movement (no teleport), never through walls, NPCs or other
obstacles, physical door collision kept, door stays blocked if there is no room, interactions / door states / persistence unchanged.

| Part | What it does |
|---|---|
| `ASpnTestDoor::RunVerb` | remembers the pawn whose Use opened the door (`GetOpener()`); a script (no user) never has an opener |
| `ASpnTestDoor::Tick` | unchanged physical stop: the leaf waits at any pawn in the sweep (`IsSweepBlockedByPawn`, any-hit; since 2026-11-24 without `OverlapMulti`: while blocked, a second test with the opener ignored tells whether the opener is the only blocker). New: if the **only** blocker is the opener, the door asks it **once per swing** to step aside |
| `ASpnTestDoor::LetOpenerStepAside` | direction radially away from the hinge; target distance = leaf reach + capsule radius + `StepAsideMargin` (12 cm, PLACEHOLDER). A capsule sweep (`ECC_Pawn`, ignoring the character and the door) must be free and the step ≤ `StepAsideMaxLength` (90 cm, PLACEHOLDER) — otherwise **no step**, the door keeps waiting and shows "obstructed" |
| `ASupernaturalProtCharacter::StartStepAside` / `TickStepAside` | the step is ordinary movement input at half input (walking pace): collision, step height and animation apply, nothing is teleported. The character keeps facing the door; move input is ignored for the fraction of a second. Ends at the target (≤ 4 cm), after 1.2 s, or when it gains < 2 cm in a 0.3 s window (something solid in the way) |
| world state / save | untouched: the open fact is written at Use as before; the leaf angle stays presentation |

First QA round showed every step ending as "blocked" after 0.25 s with 6–10 cm left: the stall test was a per-frame speed (< 0.5 cm per
frame ≈ 30 cm/s at 60 fps), and the calm step from standstill is about that slow. Replaced by the frame-rate independent window test above;
afterwards every step ended **at the target** (about 13 cm in 0.31–0.39 s).

### Tests and results (2026-11-13)

| Test | Result |
|---|---|
| `SpnGame.Doors.OpenerStepsOutOfTheSwing` / `NoRoomToStepBackKeepsTheDoorBlocked` / `OnlyTheOpenerIsAskedToStepAside` (new, headless: decision, target, no teleport, wall = no step and door stays blocked, other pawns and scripted opens never moved) | 3/3 PASS |
| Door set headless (`SpnGame.Doors`, `NightBuild04`, `NightBuild05`, `TestDoor`) | 26/26 PASS |
| `run_gameplay_qa.ps1 -Suite Doors` (Doors + DoorReach + Buildings), after the stall fix | 89/0 in **4** consecutive runs (`20261113-141702`, `-142003`, `-142303`, `-144159`); 11–16 steps per run, all "ended at the target" |
| same suite with the test bodies 160 / 195 cm | 89/0 each |

Side finding, fixed: `SpnGame.NightBuild04.DoorLockedKeyUnlock` / `DoorBlockedRefusesBoth` failed in every `-game` run independent of the
doors: they look for German prompt words, but the test world's settings subsystems re-apply the saved language "auto" and a de-DE Windows
falls back to the 'en' culture in `-game`. The two tests now pin the UI language to `de` after creating their world and restore it.

Video: `Artifacts/VisualProgress/17_Door_StepAside.mp4` (22 s; three district doors at normal speed, one at half speed).

Not covered: a player-controlled run by a human; NPCs opening doors (they cannot yet); a door opened while the player walks (the step
then competes with nothing: move input is ignored for ≤ 0.4 s). `StepAsideMargin`/`StepAsideMaxLength` are placeholders, not tuned.

## Validation round 2: narrow corridors, blocked ways back, character sizes (2026-11-13, later)

Order: validate the step in narrow corridors, with blocked ways back and different character sizes; 12 cm margin and 90 cm maximum stay.

**Defect found and fixed.** The first implementation tried only the radial direction away from the hinge. In a 140 cm corridor that
diagonal ends with the capsule in the side wall (target x ≈ 94 + radius > 120), so the door refused a step although walking straight back
along the corridor fits (79 cm). `LetOpenerStepAside` now builds candidates:

- directions: radial, straight back from the door plane, and both rotated in 15° steps up to 90°;
- only on the opener's side of the door plane, and never turning towards the leaf (the angle around the hinge must not decrease; the leaf
  stands between the closed position and the opener and is ignored by the sweep because it touches the opener);
- per direction the shortest step that ends beyond leaf reach + capsule radius + 12 cm; only steps ≤ 90 cm;
- shortest first; a candidate is taken when the capsule sweep (radius − 1 cm) is free **and** a trace finds walkable floor (normal z ≥ 0.7)
  within 30 cm below the feet at the target (no step off a porch or stair edge).

The log names the chosen candidate ("40 deg from straight back, candidate 1 of 15") or, without room, how many were tried and why the last
failed ("blocked by …", "no walkable floor at the target", "94 cm needed").

**Walked tests.** `SpnGame.Doors.Walked*` tick a headless world (60 Hz) so the character really walks with its movement component and collision.
Two test-harness findings, both documented in the test: every tick function runs at most once per engine frame, so the loop advances
`GFrameCounter`; a character without controller stays in `MOVE_None` until `SetDefaultMovementMode()`.

| Scenario | Result (headless, 2 runs) |
|---|---|
| open space, capsule 35 | step 69 cm (40° from straight back), arrived in 0.95 s, < 8 cm per frame asserted (no teleport), door then fully open |
| corridor 140 cm (faces x −20 / 120) | candidate 2 (30°), 70 cm, stays inside the corridor also after braking, door fully open, not inside a wall |
| dead end 150 cm behind the door | no room (8 directions tried, last blocked by the end wall): not moved, door stops at the character and waits, door state open |
| pawn behind the player in the corridor | no room (blocked by the pawn): neither moved |
| porch edge 100 cm behind the door | no room ("no walkable floor"): stays on the porch, door waits |
| capsule radius 25 / 35 / 50 | steps 59 / 69 / 84 cm, all arrive beyond reach + radius + 12 cm, door fully open |
| capsule radius 60 | needs 94 cm > 90: no step, door waits (the 90 cm maximum holds) |

Measured braking: after the step input ends the character slides 6–9 cm further in the same direction (away from the leaf); the tests accept
"arrived or ≤ 12 cm beyond, never short". The rendered runs end "at the target" within 4 cm because their steps are shorter (~13 cm).

Rendered: `run_gameplay_qa.ps1 -Suite Doors` with the candidate logic 89/0 (`20261113-174130`); further runs in the acceptance below.
Body height presets (`-SpnBody=test_160/195`) do not change the capsule, so they do not change the step; different capsule radii are covered
by the walked size test.
