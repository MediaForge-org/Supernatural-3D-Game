# Ledge assist: step on, vault, mantle, climb up (2026-11-14)

Developer decision of 2026-11-14 (item 1): a realistic, context-dependent ledge assist with an explicit jump / traversal input,
PROVISIONAL height bands up to 40 cm, 40–120 cm and 120–180 cm. It checks whether the edge can actually be gripped, whether the
way is free of collisions, and uses movement that fits the height. No unrealistic automatic jumps. The two open traversal tests may
only be fixed by a real solution. Status: **implemented and tested**. Open points are in section 7.

## Master basis

`Planning/Master` is not reachable on this Windows machine (no `Planning` folder), so no master search ran. The repository's own
records were read instead:

| Requirement | Status in the source | Source |
|---|---|---|
| The player triggers a jump actively; it is never an automatic reaction to an obstacle | CONFIRMED (developer 2026-10-22) | Proposal 0009 §2 (1); `Docs/Game/Kampfsystem/08` COMBAT-DEC-BT-001 |
| The jump strength stays human and is not raised to clear obstacles | CONFIRMED | Proposal 0009 §2 (2); BT-002 |
| A movement that fits the height (step up, vault, mantle) instead of an over-high jump | CONFIRMED, implementation was left for a later milestone | Proposal 0009 §2 (3); BT-003 |
| The movement must match the animation | CONFIRMED (developer 2026-10-22) | Proposal 0009 §3a |
| Height and depth come from traces and decide the move; the thresholds are configuration | was a PROPOSAL; the bands were decided by the developer on 2026-11-14 | Proposal 0009 §4; BT-015 |

The pass changes no world, story or persistence data. The ledge assist saves nothing.

## 1. Behaviour

The ledge assist only starts on the **jump press** (Space / Cross), on the ground, with an obstacle face right in front:

| Measured top above the floor | Move | Ends | Horizontal speed kept | Duration (examples, measured) |
|---|---|---|---|---|
| up to 25 cm | none (walking climbs it, `MaxStepHeight`) | – | – | – |
| 25–40 cm, deep enough to stand on | **StepOn**: step onto it | standing on top | 90 % | 35 cm at a jog: 0.29 s |
| 25–40 cm, thin, free floor behind | **Vault** (low step-over), no run-up needed | on the floor behind | 90 % | 35 cm at a walk: 0.69 s |
| 40–120 cm, thin (≤ 80 cm), a run-up (≥ 2 m/s), free floor behind | **Vault**: hand on top, legs across | in the air behind it, then landing | 85 % → 60 % (by height) | 90 cm at a run: 0.44 s |
| 40–120 cm, room to stand on top | **Mantle**: onto the ledge | standing on top | 80 % → 30 % (by height) | 90 cm from a stand: 1.00 s; 110 cm at a jog: 0.73 s |
| 120–180 cm, room to stand on top | **ClimbUp**: body stops at the wall, hands on the edge, pull-up | standing on top, speed 0 | 0 % | 150 cm at a jog: 1.23 s; 160 cm from a stand: 1.27 s |
| above 180 cm | none: ordinary jump | – | – | – |

The press starts a move only:
- **within reach**: capsule front to face at most 30 cm standing, plus 0.08 s of approach speed, never more than 60 cm. A press
  further away is an ordinary jump (the old QA cases "jump 125 cm before the ledge" are unchanged by design);
- **head-on**: at most 45° between the stick direction and the face;
- **on a surface that may be stood on**: not a pawn, not a simulated or moving object (`CanCharacterStepUp`);
- when the **edge can be gripped**: a walkable top found from above at least 15 cm deep, and nothing continuing above the highest
  band right behind the edge (a higher wall, an overhang);
- when the **way is free**: the capsule can rise in place by the height (no ceiling), the capsule can move across at top height to the
  end pose, and it stands there on a walkable floor (Mantle / StepOn / ClimbUp); or the landing pose behind a thin obstacle is free
  (Vault).

When a check fails, the press is an ordinary jump and the log says why ("no room to rise (ceiling)", "approach too oblique" ...).
Running into an obstacle without pressing never starts anything.

## 2. Motion model (no animation yet)

The capsule follows a path from the core (`SampleLedgePath`). The capsule bottom rises with an ease-out curve while it moves forward.
The lower 35 cm of the capsule (`LegAllowance`, the planted leg: one foot on the edge, one still behind) may pass beside the edge;
the rest of the body never enters the obstacle. As a result:
- a low ledge keeps most of the run-up (the leg goes onto the edge, the body carries on);
- a high ledge stops the body at the face until the hips are high enough (realistic for a mantle or climb, and measured: 0–38 %
  speed while it happens);
- **timing matters for a vault**: pressed 45 cm before a 90 cm fence at a run, 69 % of the speed is kept; pressed only 19 cm before it,
  the body hits the fence first (5 % for a moment) and vaults anyway. Both cases are tested.

During the rise only the obstacle's own components are ignored by the capsule sweep; everything else (another wall, a pawn, a moving
object) still blocks. If the path is blocked on the way, the move is released into a fall at the current place (never pushed
through). The obstacle collides again as soon as the capsule bottom is above its top.

## 3. Implementation

| Part | File |
|---|---|
| Rules and path (engine-independent, C++20, metres) | `Core/include/spn/core/LedgeAssist.h`, `Core/src/LedgeAssist.cpp` |
| Core tests (10 cases, 10 244 assertions) | `Tests/Core/LedgeAssistTests.cpp` |
| Bridge in cm, settings struct `FSpnLedgeAssistSettings` | `Unreal/.../Source/SpnCore/Public/SpnLedgeAssist.h`, `Private/SpnLedgeAssist.cpp` |
| Measurement, path validation, custom movement mode | `Unreal/.../Spn/SpnLedgeAssistMovement.cpp` (`USpnCharacterMovementComponent::DoJump`, `PhysCustom`) |
| Settings, state, diagnostics record | `Unreal/.../Spn/SpnCharacterMovementComponent.h` (`LedgeAssist`, `IsLedgeAssisting`, `GetLastLedgeRecord`) |
| Rendered QA test | `Unreal/.../Spn/Tests/SpnQaLedgeAssistAutomation.cpp` (`SpnQA.LedgeAssist`, suite `Ledge`, also in `Movement` and `Full`) |

Measurement (in `PlanLedge`): small sphere sweeps forward at heights from the step height up to 180 cm find the face; a line from
above, 6 cm behind the edge, finds the top; samples every 10 cm follow the top for the depth; capsule sweeps check the rise, the way
across and the end or landing pose. The plan is made optimistically, then validated; a failed sweep re-plans without that option
(for example a Vault falls back to a Mantle). Heights are measured from the floor, not from the hovering capsule.

The existing older core vocabulary (`Traversal.h`, M2-A.2) is unchanged. Its thresholds differ from the developer's bands, so the new
rules live in `LedgeAssist.h`.

## 4. The two QA failures

`SpnQA.Jump` `late_jump_ledge_45_jog` and `late_jump_ledge_40_run` (Space pressed 10 cm before a ledge about as high as the jump apex)
failed since M2A2. The test code is **unchanged**; only an outdated comment was updated. The ledge assist now turns the press into a
Mantle (45 cm) or a StepOn (40 cm):

| Case | Before | Now (run `20261114-124137`) |
|---|---|---|
| late_jump_ledge_45_jog | FAIL: speed collapses to 4–10 %, mounts after 0.45 s | PASS: lowest speed 269 cm/s (77 %), across after 0.66 s |
| late_jump_ledge_40_run | FAIL | PASS: lowest speed 495 cm/s (90 %), across after 0.39 s |

Every other jump case is unchanged, including the walls and ledges that a jump must **not** get over (`ledge_60_*`, `ledge_70/80_run`,
`ledge_100_sprint`, `wall_150_collision_jog`: those presses are 80–125 cm away, outside the reach).

## 5. Tests

- Core: `ctest` 631/631 (the 10 new ledge cases included).
- `SpnQA.LedgeAssist`: 14/14 (run `20261114-133424`, recorded: `Artifacts/VisualProgress/25_Ledge_Assist.mp4`): step on 35 cm, step over a
  thin 35 cm wall, mantle 90 cm standing, mantle 110 cm at a jog, vault 90 cm at a run (on time and late), climb 150 cm at a jog and
  160 cm standing; refusals: thin 90 cm wall without a run-up, 200 cm wall, no press, pressed 100 cm away, a ceiling 120 cm above the top,
  a 60° approach. Each check reads the outcome from the character (position, feet height, speed, movement mode) and the reason from
  the movement component's record.
- `-Suite Movement` 58/58 (gaits, turns, all jumps, ledge assist, locomotion).
- `-Suite Full`: section 8 of `Artifacts/VisualProgress/DECISIONS_FOLLOWUP_2026-11-14.md`.

## 6. Performance

The probe only runs on a jump press (about 20 sweeps and line traces); nothing ticks otherwise. Route performance after all of today's
changes: section 6 of the follow-up report.

## 7. Open points

- **No animation.** The capsule moves correctly, but the mannequin plays its jump or fall pose during the move. The developer decision
  of 2026-10-22 requires the animation to match the movement; that needs vault / mantle / climb assets (none exist yet). The movement
  component exposes `GetActiveLedgeMove()` for a later animation blueprint.
- **Co-op prediction is not done.** The move runs in the movement component (server and owning client both start it from the jump
  flag), but its state is not in the saved moves, so a server correction during a move is not replayed exactly. Single player only.
- Tested on runtime boxes on the movement test map. Not yet tried on the generated world's fences, walls and porches.
- All numbers are placeholders (reach, grip depth, leg allowance, durations, speed shares). The three bands are the developer's
  provisional bands.
- A ledge assist inside a fight, as an escape (`CQ-31`) or with injuries and exhaustion is not part of this pass.
