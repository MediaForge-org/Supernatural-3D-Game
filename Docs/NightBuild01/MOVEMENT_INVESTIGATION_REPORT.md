# Movement investigation after Night Build 01 (2026-10-30)

Baseline: `aed91e2`. Nothing committed or pushed; `stash@{0}` untouched; no engine file modified (engine sources were only READ). Windows 11, UE 5.8, RTX 3090.
Status words: VERIFIED (measured in a real engine run in this session), HYPOTHESIS (consistent with the evidence, not proven).

## 1. Baseline

| Case | Baseline (same code, earlier today: `logs/regression-final.log`, rendered report `Saved/GameplayQA/20261030-124911`) and fresh pre-fix diagnostics of this session |
|---|---|
| `SpnGame.Movement.Obstacles` | FAIL: 30 cm thin ledge crossed by walking (fresh trace `Saved/SpnDiag/` pre-fix run: capsule Z 92.15 -> 122.15 in ONE frame at X 2945) |
| `SpnGame.Movement.StairsAndRamps` | FAIL: staircase with 30 cm risers climbed |
| `SpnQA.Jump.late_jump_ledge_45_jog` | FAIL (lands on top after 0.44 s, crossing > 1 s) |
| `SpnQA.Jump.late_jump_ledge_40_run` | FAIL (same) |
| `SpnQA.MovementDemo.ledge_80_blocks_even_with_a_jump` | intermittent: 7 FAIL in the 16 historical rendered runs found under `Saved/GameplayQA/` |

## 2. Instrumentation added

* **`spn.MoveTrace`** (console variable, default 0): one trace line per engine event of the SPN character: `MoveAlongFloor` (begin/end, floor), `StepUp` (OK/REFUSED with the hit), `AdjustFloorHeight` (Z before/after, the floor used), `SlideAlongSurface` (Z/X change, normal), `PhysFalling`, and the new floor-limit decision. Bounded buffer (20 000 lines), read by tests with `ConsumeMoveTrace()`. Off = one cvar read per call, no allocation.
* **Diagnostic automation `SpnDiag.Movement.*`** (`Spn/Tests/SpnMovementDiagAutomation.cpp`, never part of the `SpnGame` filter, asserts nothing): `LedgeWalk` (25/30/40 cm), `StairWalk` (25/30/40 cm risers), `LateJump` (45 cm jog / 40 cm run, jump when the capsule front is 10 cm from the face; variants: floor rule on/off, movement sub-step 1/20 vs 1/120 s, capsule radius 30/34 cm), `WallPush` (pushing into a wall after a 30 cm/s sideways nudge, steering on/off). Real character, real movement input (`DoMove`, `Jump`), real map; frame-by-frame traces in `Unreal/SupernaturalProt/Saved/SpnDiag/<variant>/`.
* Frame-rate comparison: the same runs with `-benchmark -fps=30 / 60 / 120` (fixed engine delta time) and the default headless rate.

## 3. Root causes

### Cases 1 and 2 — 30 cm ledge / 30 cm stair walked up — VERIFIED

Trace of the frame in which the capsule rose (pre-fix, `ledge_walk_30`):

```
MAF begin z=92.15 x=2943.55 v=350.0 floor{walk=1 dist=2.15}
STEPUP REFUSED z 92.15 -> 92.15   hit n=(-0.98,0,0.20) ip=(2980.0, -2100, 30.0)     <- the step rule says no (correct)
SLIDE  z 92.15 -> 92.15 (dz 0.00)                                                  <- the slide does not lift either
ADJUST z 92.15 -> 122.15 (dz 30.00) floor{walk=1 dist=-27.85 hit n=(0,0,1) ip=(2980.3, -2100, 30.0)}   <- here
```

`StepUp` correctly refuses the 30 cm ledge and the slide lifts nothing (the previous hypothesis of "many small slide lifts" was wrong: it is ONE lift of 30 cm). The lift comes from the **floor check after the move** (`FindFloor` -> `ComputeFloorDist`): with `bUseFlatBaseForFloorChecks` the engine sweeps a box turned 45 degrees whose corners reach the full capsule radius; the first sweep starts penetrating the ledge, the engine retries from 48.6 cm above the capsule bottom (`ShrinkScaleOverlap`), the box corner lands on the ledge TOP, and the result is a walkable floor at -27.85 cm (negative floor distances down to -radius are allowed "to pull out of penetrations"). The perch validation then probes with the smaller perch radius, finds the ORIGINAL ground under the capsule and therefore accepts the perch. `AdjustFloorHeight` moves the capsule up by 30 cm. `MaxStepHeight` is never consulted on this path. A 40 cm ledge is not affected only because the capsule's rounded bottom keeps the axis 2 cm further from the face, so the box corner no longer reaches the top (geometry, not a rule). The stair case is the same mechanism (594 limit events in the 30 cm stair trace after the fix).

Source: gameplay code (configuration `bUseFlatBaseForFloorChecks`, introduced in M2A4.1 to enforce the step limit) combined with engine floor-check behaviour; not test geometry, not the test expectation.

### Cases 3 and 4 — late jumps — VERIFIED mechanism, outcome is a design question

Rendered trace (`Saved/GameplayQA/20261030-134131/traces.jsonl`) and the diagnostic runs show, for both cases, 1-2 frames after take-off (feet still 6-11 cm above the ground): the capsule's LOWER hemisphere touches the ledge corner, the engine's falling slide with its slope-boosting guard removes the horizontal velocity in one frame (350 -> 33 cm/s, 550 -> 36 cm/s; at 30 fps to 0), and the character then rises almost vertically in front of the face. Whether it ends on top is decided by sub-centimetre effects: the apex is 45.0 cm for a 45 cm ledge (knife edge), and the residual 0-40 cm/s air drift decides whether the capsule is over the top when it comes down.

* **Frame-rate dependent (VERIFIED):** default sub-step (1/20 s): 45 jog crosses at 30 fps, not at 60/120; 40 run crosses at 60/120, not at 30. Two causes: the take-off point (the test triggers "10 cm before the face" per frame: 18 cm of travel per frame at 30 fps and 550 cm/s) and the integration of the first contact.
* **Movement sub-step 1/120 s (`MaxSimulationTimeStep`):** the 45 cm case becomes frame-rate independent (blocked at 30/60/120); the 40 cm case still differs (blocked at 30/60, crosses at 120) because the take-off point still differs per frame rate. Not adopted (see section 9).
* **Capsule radius 30 / 34 cm instead of 42 (experiment only):** both cases are BLOCKED at every frame rate. The template's wide capsule is therefore not what blocks: a body that leaves the ground 10 cm in front of a ledge at the height of its own apex is stopped by the face. Getting onto the ledge in that situation is a **mantle / ledge-assist** (the test text itself says so: "known issue ... needs a ledge assist / mantle", planned for the vault milestone).
* Conclusion: the failures are not a physics defect that can be fixed without inventing a traversal rule. Per the movement design (explicit jump input, no automatic vault/mantle without an established rule) these two tests stay FAIL and need a **developer decision** (section 10.1). They were not changed.
* The new floor rule does not influence them (floor rule on/off gives identical traces).

### Case 5 — `ledge_80_blocks_even_with_a_jump` intermittent — VERIFIED root cause

Evidence from all 16 historical rendered runs: every FAIL has the character FAR from the 80 cm wall (175, 608, 4489, 7435 cm), never on or behind it; the 80 cm wall was never climbed (max Z = jump apex). The three most recent failures stand at (-9090, 7955): 35 cm beyond the east end of the 12 m wall, stopped by House1. The trace of one of them (`20261030-022607`) shows, after the jump lands in front of the wall, the character **accelerating sideways along the wall from 0 to 130 cm/s while the input points straight into it** (the logged control yaw is constant 90 degrees - the "mouse drift" hypothesis was tested and rejected).

Mechanism (reproduced deterministically by `SpnDiag.Movement.WallPush`): the SPN ground steering (Core `SteerVelocity`) turns the velocity towards the input and keeps/accelerates its magnitude. Pressed against a wall, the engine sets the velocity after every move to the actual (sliding) displacement, so the steering "turns" again every frame and accumulates sideways speed: 30 cm/s nudge -> **133 cm/s and 285 cm drift in 3 s** (engine friction path: 3.5 cm). A landing next to the wall leaves such a nudge only in some lanes (the demo's lateral position varies between runs), hence the intermittency. Source: gameplay code (steering vs. blocked moves), not the test.

Earlier failure types in the history (175 cm / 608 cm while still walking, 7435 cm round a 4 m wall) predate fixes already made to the test (`WaitSettled`, 12 m wall).

## 4. Code changes

| File | Change |
|---|---|
| `SpnCore/Public/SpnMovement.h` | new setting `bLimitFloorRiseToStepHeight` (default true), documented with the measurement |
| `SupernaturalProt/Spn/SpnCharacterMovementComponent.{h,cpp}` | `FindFloor` override: while walking, a floor that would lift the capsule by more than `MaxStepHeight` is not accepted; the floor is looked up again with the perch probe (radius - PerchRadiusThreshold); without a walkable floor within the limit the floor is invalid and the normal fall/ledge logic decides. `CalcVelocity`: when the previous move slid along a non-walkable surface and the input points into it, only the input component ALONG the wall steers (straight into the wall = brake). `SlideAlongSurface` records that wall contact. Trace overrides (`MoveAlongFloor`, `StepUp`, `AdjustFloorHeight`, `SlideAlongSurface`, `PhysFalling`) behind `spn.MoveTrace` |
| `Spn/Tests/SpnMovementWallAutomation.cpp` (new) | regression test `SpnGame.Movement.WallPushNoSideDrift` (straight push: drift <= 10 cm and rest; 30 degree push: still slides) |
| `Spn/Tests/SpnMovementDiagAutomation.cpp` (new) | the diagnostic runs (section 2) |
| `Spn/Tests/SpnTestMapM1Automation.cpp` | `M1Traversal` diagonal wall case: checked at the moment the slide is shown instead of after a fixed 3.5 s (justification below) |
| `Tools/qa_inventory.json`, `Docs/Development/Gameplay-QA-Inventory.md` | new test registered / regenerated |

**Test change, justified:** `SpnGame.TestMap.M1Traversal` asserted that a character walking 45 degrees into a 6 m wall is still in front of the wall after 3.5 s. That held only because the defective steering feedback slowed the slide down. With the fix the slide runs at the engine's speed (350 x cos 45 = 247 cm/s) and the character reaches the end of the wall and walks round it (correct). The check now evaluates "stopped by the wall" and "slid along it" at the moment the slide is proven (Y past WallY + 100); both assertions are unchanged, no tolerance was widened. No other test was modified.

## 5. Before / after

| Measurement | Before | After |
|---|---|---|
| 30 cm thin ledge, walking (jog) | crossed, Z +30 cm in one frame | **blocked** (max Z 97 = ground) |
| 25 cm thin ledge, walking | crossed (StepUp) | crossed (unchanged) |
| 40 cm thin ledge, walking | blocked | blocked |
| 30 / 40 cm ledge with jump attempts | crossed by jumping | crossed by jumping (unchanged) |
| Staircase 25 cm risers | climbed | climbed |
| Staircase 30 cm risers | climbed | **blocked** |
| Walking cases at 30 / 60 / 120 fps | - | identical outcome at every rate |
| Wall push after a 30 cm/s nudge | 285 cm / 133 cm/s along the wall | **1.7 cm**, comes to rest |
| Walking 30 degrees into a wall | slides | slides (494 cm in 3 s, the end of the obstacle reached) |
| Stair camera smoothing (`StairCamera`) | 4.14 cm/frame | unchanged (test passes) |
| Late jump 45 jog / 40 run | FAIL | FAIL (unchanged, design decision) |

## 6. Tests executed (fresh, final tree of this investigation)

| Suite | Result | Log / report |
|---|---|---|
| Unreal editor build | PASS | `logs/mv-build-10.log` |
| `SpnGame.Movement.*` (headless) | **18 / 18 PASS** (incl. `Obstacles`, `StairsAndRamps`, `StairCamera`, `SprintTurn`, new `WallPushNoSideDrift`) | `logs/mv-movement-2.log`, `logs/mv-wall-1.log` |
| Unreal `SpnGame` + `SpnCore` (headless, NullRHI) | **131 / 131 PASS** (baseline 128 / 130; +1 new test) | `logs/mv-regression-final.log` |
| first full run before the M1 test adjustment | 130 / 131: `SpnGame.TestMap.M1Traversal` diagonal-wall timing (section 4) | `logs/mv-regression-1.log` |
| `SpnQA.*` rendered, full suite | **PASS 219, FAIL 2** (`Jump.late_jump_ledge_45_jog`, `late_jump_ledge_40_run`), NOT_IMPLEMENTED 3, NOT_TESTED 1 (statements of the suites, unchanged) | `Saved/GameplayQA/20261030-142357/report.md` |
| `SpnQA.Jump` rendered (after the floor rule) | PASS 15, FAIL 2 (same two) | `Saved/GameplayQA/20261030-134131/` |
| `SpnQA.MovementDemo` rendered, repeated (V3 level) | **3 x 27 / 27 PASS**; run 3 in the formerly failing lane | `Saved/GameplayQA/20261030-143719`, `-143942`, `-144205` |
| Night Build 01 journey (scripted OS input, no teleport, no video) | PASS: 7 / 7 restore checks, navigation 50.0 to 38.1 m, no stuck walk | `Saved/NightBuild01/movement-journey/steps.txt` |
| Core MSVC Debug / clang-cl Debug | PASS 493 / 493 / PASS 493 / 493 (Core unchanged) | `logs/mv-core-msvc.log`, `logs/mv-core-clang.log` |
| Python tool tests | PASS 219 | |
| QA inventory gate | PASS (27 systems, 0 problems) | |
| Diagnostics `SpnDiag.Movement.*` at 30 / 60 / 120 fps | see section 3 / 8 | `logs/mv-diag-fps-*.log`, `mv-diag-sub-*.log`, `mv-diag-r-*.log`, `Saved/SpnDiag/` |

Real gameplay validation: all of it is real engine execution with movement input (no teleport during the measured runs; the SpnGame map tests place the character at the start of each case, as before). Walking over small steps (25 cm), being blocked at 30/40 cm, jumping onto 30-50 cm ledges, stairs (15-25 cm climbed, 30+ blocked), ramps (5-40 degrees walkable, 50 not), sprint turns, landings, pushing into and sliding along walls were exercised by the suites above; the rendered SpnQA suites drive the character with simulated key input in a real window. Not done: a recorded video of the movement cases (the traces and rendered QA runs are the evidence), hardware gamepad.

## 7. Remaining failures

`SpnQA.Jump.late_jump_ledge_45_jog` and `SpnQA.Jump.late_jump_ledge_40_run` - unchanged, root cause documented, blocked on design decision 10.1. Case 5: root cause fixed and covered by a deterministic regression test; four rendered runs after the fix passed, one of them in exactly the lane (sidewalk, x -9722, Z 104) in which the three latest historical failures happened. Repeated runs cannot prove the absence of an intermittent failure; the evidence is the reproduced mechanism and its removal.

## 8. Frame-rate sensitivity

Walking / step cases: none observed (outcome identical at 30, 60 and 120 fps with fixed engine delta time). Late jump: strong (section 3): the take-off point is quantized per frame (18 cm of travel per frame at 30 fps and run speed) and the engine sub-step (`MaxSimulationTimeStep` 0.05 s = one sub-step per frame down to 20 fps) leaves the first contact frame-rate dependent; 1/120 s makes the 45 cm case consistent, not the 40 cm case (take-off point).

## 9. Performance

* Floor rule: one extra `ComputeFloorDist` only in frames where the regular floor check reports a rise above the step limit (pressed against a ledge between the step limit and roughly 40 cm); no cost otherwise.
* Wall-contact steering: no query, two members, set by a slide the engine already performs.
* Trace: one console-variable read per overridden call when off; no allocation, no permanent logging.

## 10. Design decisions still required

1. **Late jump onto a ledge at apex height (cases 3/4):** keep "blocked" (current physics, the two tests stay red), introduce a ledge assist / mantle rule (explicit input? height window below the ledge top, minimum speed, animation), or change the test expectation. Not decided here; no automatic mantle was added.
2. **Movement sub-step:** adopt `MaxSimulationTimeStep` = 1/120 s for the player character (consistency vs. up to 4 sub-steps per frame at 30 fps; affects network prediction)?
3. **Capsule radius:** the template's 42 cm (84 cm wide body) is wider than a human; it was measured NOT to be the cause of cases 3/4, but door and corridor design depend on it.
4. **Slide speed along walls:** now engine parity (tangential component of the input); a slower "rubbing" slide would be a new rule.
5. **`bUseFlatBaseForFloorChecks`:** kept on; the new floor rule makes the step limit hold with it. Whether the flat base is still wanted is open (it was added in M2A4.1 only for the step limit).

## 11. Recommended next steps

Decide 10.1; if a mantle is wanted, specify it as its own traversal action and give the late-jump tests the matching expectation. Keep `SpnDiag.Movement.*` and `spn.MoveTrace` as the investigation harness. Extend `WallPushNoSideDrift` to corners and doorways when the level geometry grows. Consider 10.2 together with the first network-play milestone.
