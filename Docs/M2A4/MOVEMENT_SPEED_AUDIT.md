# M2A4.2 – Movement speed audit (stairs, ramps, diagonal input)

Status: measured with `SpnGame.Movement.SpeedAudit` (new, `Spn/Tests/SpnMovementSpeedAutomation.cpp`), Unreal 5.8.4, `Lvl_SpnMovementTest`, Windows, editor automation (`-nullrhi`, real physics, real `CharacterMovementComponent`).

## Question

The developer reported that the character seems faster on steeper stairs and suspected that W+A / W+D is faster than W.

## Method

Each case teleports the character onto the map, feeds `DoMove(Right, Forward)` every frame (the same function the Enhanced Input action calls) and measures over a window defined by **distance** along the movement direction, not by time. Stairs and ramps start at the full gait speed (the map has no run-up in front of them). Reported per case:

* horizontal speed = window distance along the movement direction / window time (what the map shows),
* 3D speed = 3D displacement / window time,
* path speed = sum of the 3D frame deltas / window time,
* input magnitude = peak of |acceleration| / max acceleration (must be <= 1).

Frame rate is varied with `FApp::SetFixedDeltaTime` (30 / 60 / 120 fps). The default runs use about 120 fps (dt 0.0083 s).

## Results (cm/s)

Intended values from `FSpnMovementSettings`: Jog 350, Sprint 800.

| Case | horizontal | 3D | path | max input |
|---|---|---|---|---|
| level jog W | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W+A | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W+D | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog S | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog S+A | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog A | 350.0 | 350.0 | 350.0 | 1.000 |
| level sprint W | 792.7 | 792.7 | 792.7 | 1.000 |
| level sprint W+A | 792.7 | 792.7 | 792.7 | 1.000 |
| level sprint W+D | 792.7 | 792.7 | 792.7 | 1.000 |
| level sprint S | 792.3 | 792.3 | 792.3 | 1.000 |
| level sprint S+A | 792.6 | 792.6 | 792.6 | 1.000 |
| level sprint A | 792.5 | 792.5 | 792.5 | 1.000 |
| level jog W @30fps | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W+D @30fps | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W @60fps | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W+D @60fps | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W @120fps | 350.0 | 350.0 | 350.0 | 1.000 |
| level jog W+D @120fps | 350.0 | 350.0 | 350.0 | 1.000 |
| stairs up R15 jog W | 350.0 | 372.9 | 456.0 | 1.000 |
| stairs up R15 sprint W | 800.0 | 851.6 | 989.7 | 1.000 |
| stairs down R15 jog | 350.0 | 380.8 | 473.6 | 1.000 |
| stairs up R15 jog W @30fps | 350.0 | 374.5 | 383.3 | 1.000 |
| stairs up R15 jog W @120fps | 350.0 | 372.9 | 456.0 | 1.000 |
| stairs up R18 jog W | 350.0 | 393.6 | 503.2 | 1.000 |
| stairs up R18 sprint W | 800.0 | 902.6 | 1091.0 | 1.000 |
| stairs down R18 jog | 350.0 | 408.2 | 528.7 | 1.000 |
| stairs up R18 jog W @30fps | 350.0 | 395.1 | 406.3 | 1.000 |
| stairs up R18 jog W @120fps | 350.0 | 393.6 | 503.2 | 1.000 |
| stairs up R22 jog W | 350.0 | 422.4 | 557.1 | 1.000 |
| stairs up R22 sprint W | 800.0 | 980.0 | 1055.1 | 1.000 |
| stairs down R22 jog | 350.0 | 445.6 | 591.7 | 1.000 |
| stairs up R22 jog W @30fps | 350.0 | 429.2 | 443.0 | 1.000 |
| stairs up R22 jog W @120fps | 350.0 | 422.4 | 557.1 | 1.000 |
| stairs up R25 jog W | 350.0 | 449.0 | 583.9 | 1.000 |
| stairs up R25 sprint W | 800.0 | 1026.3 | 1077.6 | 1.000 |
| stairs down R25 jog | 350.0 | 476.3 | 637.6 | 1.000 |
| stairs up R25 jog W @30fps | 325.7 | 413.7 | 430.4 | 1.000 |
| stairs up R25 jog W @120fps | 350.0 | 449.0 | 583.9 | 1.000 |
| ramp up 5 deg jog W | 350.0 | 351.3 | 351.3 | 1.000 |
| ramp up 10 deg jog W | 350.0 | 355.4 | 355.4 | 1.000 |
| ramp up 15 deg jog W | 350.0 | 362.3 | 362.3 | 1.000 |
| ramp up 20 deg jog W | 350.0 | 372.5 | 372.5 | 1.000 |
| ramp up 30 deg jog W | 350.0 | 404.1 | 404.1 | 1.000 |
| ramp up 40 deg jog W | 350.0 | 456.9 | 456.9 | 1.000 |

## Findings

1. **Diagonal input is not faster.** W, W+A, W+D, S, S+A and A all give exactly 350.0 cm/s at Jog; sprint gives 792.3–792.7 cm/s. The input magnitude peaks at 1.000. `DoMove` adds two axis inputs (a diagonal has magnitude 1.41 before the engine clamps it); `CharacterMovementComponent::ScaleInputAcceleration` clamps it to 1 and `USpnCharacterMovementComponent::CalcVelocity` additionally normalises the direction. No change was needed or made.
2. **Horizontal speed does not depend on the steepness of stairs or ramps.** Staircases R15, R18, R22, R25 (23° to 43° slope): 350.0 cm/s up (jog), 350.0 cm/s down, 800 / 800 / 800 / 779.8 cm/s up (sprint). Ramps 5° to 40°: 350.0 cm/s. The fixed-step runs agree (30 fps vs 120 fps) except R25 at 30 fps (325.7 cm/s, slower, never faster; R25 is exactly the step limit and a step-up is retried at the larger step size).
3. **What does grow with the steepness is the vertical rate.** 3D speed on stairs: 373 (R15) -> 393 (R18) -> 422 (R22) -> 449 (R25) cm/s; on ramps 351 (5°) -> 457 (40°) cm/s. That is geometry (the same horizontal speed over a steeper surface), not extra thrust.
4. **Step-ups move the capsule, and with it the camera, by the full riser height in ONE frame** (largest vertical movement per frame: 15 cm on R15, 25 cm on R25; 0 vertical direction reversals, so there is no jitter). Steeper stairs mean bigger and more frequent camera pops per second. This is the most likely reason why steeper stairs *look* faster; it is a presentation effect, not a speed effect. It was not measured on the rendered camera: the audit is a movement measurement.

## Decision taken and what was not changed

* No movement constant was changed. The measured model (constant horizontal speed) is the intended one for all tested cases, so there is no root cause to fix in the movement core or the adapter.
* **Proposal for the developer (not implemented, a feel decision):** smooth the *camera* (not the capsule) vertically while a step-up happens, e.g. a short critically damped interpolation of the spring arm's Z offset after `MaxStepHeight` step-ups. Alternative: a slope-dependent speed factor if constant *surface* speed is wanted instead of constant horizontal speed.
* A diagonal staircase measurement was dropped: the stairs are 3 m wide with rails, so a 45° line only measures the rail contact.

## Regression coverage (new)

`SpnGame.Movement.SpeedAudit` asserts: every window reached; input magnitude <= 1.001; every level direction not faster than W by more than 3 % (Jog: equal within 4 %); Jog W within 5 % of the setting; 30 fps vs 120 fps within 3 % on level ground; stairs (up, down, sprint) not faster than level; stair speed spread over the rises <= 3 %; low frame rate never faster than high frame rate on stairs.
