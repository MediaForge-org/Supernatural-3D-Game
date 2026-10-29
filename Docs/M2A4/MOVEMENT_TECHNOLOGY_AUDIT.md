# M2A4.1 Movement Technology Audit (2026-10-28)

Status: engineering analysis and proposals for the developer. Nothing here is master canon and nothing here ratifies a design decision. Values named "current"
are the placeholder test values of the repository, not approved tuning (CLAUDE.md section 3).

Evidence labels used in every section:

- **[EXISTING]** exists in the repository and was read or run.
- **[VERIFIED]** a property of the installed engine (UE 5.8.4, `C:\Program Files\Unreal Engine\UE_5.8`), read from its source, plugin descriptors or measured in this project.
- **[PROPOSED]** a suggestion of this audit, not implemented.
- **[UNVERIFIED]** an assumption or general knowledge that was not checked against the installed engine or the project.

## 1. Existing movement architecture [EXISTING]

```
Enhanced Input (keys / DualSense) ──► ASupernaturalProtCharacter (C++) ──► USpnCharacterMovementComponent : UCharacterMovementComponent
                                                                               │  settings & gait rules        ▲
                                                                               ▼                               │ tests
                                          SpnCore module (UE) ──bridge──► Core library (portable C++20, spn::core::*)    SpnQA.* (real key input)
                                          FSpnMovementSettings            Movement / Locomotion / GroundSteering / JumpModel / Traversal   SpnGame.* (automation)
                                                                                                               core: CTest (353 tests)
```

- **Rules live in the portable Core** (`Core/include/spn/core`, 24 sources compiled straight into the `SpnCore` UE module): gait table and resolution (`Movement`, `Locomotion`),
  ground steering (`GroundSteering`), jump kinematics (`JumpModel`), obstacle classes (`Traversal`). They are unit-tested without Unreal (353 CTest cases).
- **Unreal adapter**: `Spn/SpnCharacterMovementComponent.{h,cpp}` derives from `UCharacterMovementComponent`. It overrides `GetMaxSpeed`, `GetMaxAcceleration`, `CalcVelocity`
  (new in M2A4.1), `OnMovementModeChanged`, `InitializeComponent`/`BeginPlay`, and carries the network flags (sprint request, base gait) through `FSavedMove_SpnCharacter` /
  `UpdateFromCompressedFlags` / `ClientUpdatePositionAfterServerUpdate` so client replay is deterministic.
- **Character**: `SupernaturalProtCharacter.cpp` (capsule 42 × 96, `bOrientRotationToMovement = true`, no controller yaw, `JumpMaxCount = 1`, no hold-to-jump-higher), spring-arm camera
  400 cm with collision recovery, `USpnInteractionComponent` (E, trace 250 cm). Input actions come from `IMC_Default` plus runtime-created contexts for sprint (LeftShift /
  left thumbstick press), gait cycle (V) and interact (E).
- **Diagnostics**: `USpnMovementDiagnostics` (`spn.MoveDiag`) writes per-frame CSV of velocity, floor distance, step-up/landing events.
- **Animation**: template mannequin (`Content/Characters/Mannequins/Anims`, 102 assets, `ABP_Unarmed`, `BS_Idle_Walk_Run` blend space, Walk/Jog/Jump clip folders). No Motion Matching, no warping, no IK in use.
- **Physics**: the project uses no physics simulation for the character; it is a kinematic CMC capsule using scene sweeps. `DefaultEngine.ini` has no physics overrides (no sub-stepping, solver or
  async-physics settings were found); engine default gravity is `-980` (`BaseEngine.ini`, `DefaultGravityZ`).

## 2. Current Unreal component configuration [EXISTING], with engine defaults [VERIFIED]

Source of truth: `FSpnMovementSettings` (`SpnCore/Public/SpnMovement.h`) applied by `USpnCharacterMovementComponent::ApplySettings()`.

| CMC property | Value in this project | Origin |
|---|---|---|
| `MaxWalkSpeed` | 350 (default gait Jog); real speed from `GetMaxSpeed()`: Walk 180, Jog 350, Run 550, Sprint 800 cm/s | Settings, gait table |
| `MaxAcceleration` | per gait: 200 / 300 / 350 / 400 cm/s² (`GetMaxAcceleration()`) | Settings |
| `BrakingDecelerationWalking` | 800 cm/s² (constant deceleration, `bUseSeparateBrakingFriction = true`, `BrakingFriction = 0`) | Settings |
| `GroundFriction` | 8 (`TurnFriction`); **no longer used for plain walking since M2A4.1** (Core steering, see section 5) | Settings |
| `RotationRate` | yaw 500 °/s, `bOrientRotationToMovement = true` | Settings, character |
| `AirControl` / `BrakingDecelerationFalling` | 0.05 / 0 (take-off momentum kept) | Settings |
| `JumpZVelocity` | derived from `JumpHeight` 45 cm and gravity (`JumpVelocityForHeight`) → apex 43–45 cm measured | Settings, `RefreshJumpVelocity` |
| `GravityScale` | 1.0 (not overridden), `DefaultGravityZ -980` | engine default |
| `MaxStepHeight` | 25 cm | Settings |
| `WalkableFloorAngle` | engine default (walkable Z 0.71 printed by `SpnMoveDiagCfg`, about 44.8°) | engine default, [VERIFIED] by the diagnostic line |
| `PerchRadiusThreshold` | **14 cm** (was 0) — new in M2A4.1 | Settings, measured |
| `bUseFlatBaseForFloorChecks` | **true** (was false) — new in M2A4.1 | Settings, measured |
| `bMaintainHorizontalGroundVelocity` | engine default true [VERIFIED in `CharacterMovementComponent.cpp:790`] | engine |
| Capsule | radius 42, half height 96 | character |

Movement modes in use: `MOVE_Walking`, `MOVE_Falling`. `NavWalking` is classified as ground by the SPN state; swimming/flying do not exist yet.
The gait is a request (base gait + sprint request) resolved by the Core (`ResolveGait`), including the "sustain" angle rule for sprint.

## 3. Findings about the engine's CMC that explain the open regression failures [VERIFIED]

Read in `Engine/Source/Runtime/Engine/Private/Components/CharacterMovementComponent.cpp` and confirmed by measurement (see the regression report):

1. `CanStepUp()` returns false while `MovementMode == MOVE_Falling` (line 7525 ff.): there is **no step-up in the air**. A jump over a ledge works through (a) the rounded capsule bottom rolling over the
   corner after landing on it and (b) the walking step-up (`MaxStepHeight`) afterwards.
2. With the engine defaults a jump with a 45 cm apex can clear 60–70 cm ledges (measured: 60/70 cm mounted with `PerchRadiusThreshold = 0`, flat base on).
   A walking 30 cm step is crossed **independently of `MaxStepHeight`** (measured in M2A2 and again in M2A4.1: limits 10, 20 and 25 all cross 30 cm). Hypotheses tested and **eliminated** in M2A4.1: the
   rounded-base step measurement (`bUseFlatBaseForFloorChecks = true`: no change), the perch threshold (14 cm: no change for walking), a reduced `MaxStepHeight` (the `StepUp` down-sweep adds `2 * MAX_FLOOR_DIST` = 4.8 cm;
   subtracting it: no change, and it broke the invariant checked by `SpnGame.Movement.StepJumpIndependence`, so it was reverted) and the slope-boosting guard on the ground (no change, still in the code). The cause of the
   walking 30 cm climb is therefore **not identified** (candidates, not tested: sliding up the capsule's hemisphere in `SlideAlongSurface` inside `StepUp`, or the capsule shape itself).
3. `PerchRadiusThreshold` (default 0) controls whether the capsule may rest on an edge close to its rim. With 12–16 cm the capsule no longer rolls over corners of ledges ≥ 60 cm (measured).
4. `CalcVelocity()` blends the velocity vector towards the input direction by `GroundFriction`; the blend shrinks the vector magnitude on direction changes (measured 800 → 462 cm/s in a 90° sprint turn).

## 4. Technology comparison

| Technology | Role | Status in the installed engine | Use now? | Evidence |
|---|---|---|---|---|
| **CharacterMovementComponent** | kinematic character movement, prediction, step-up, floor logic | production, used by the project | **keep, extend** | [EXISTING] |
| **Chaos physics** | rigid bodies, scene queries, vehicles later | default and only physics engine of UE 5.8 | already used (queries); character stays kinematic | [VERIFIED] no PhysX/Jolt plugin in `Engine/Plugins` |
| **Unreal Mover** (+ ChaosMover, MoverAnimNext, MoverExamples) | CMC successor, rollback networking via Network Prediction / Chaos networked physics | `Mover.uplugin`: `IsExperimentalVersion: true`; README: "Experimental. Many features are incomplete or missing. APIs and data formats are subject to change at any time." | **no migration** (as demanded) | [VERIFIED] plugin descriptor and README |
| **Pose Search / Motion Matching** | animation selection from a database of poses | plugin present, not enabled by default, no beta/experimental flag in the descriptor; depends on AnimationWarping, BlendStack | later (needs animation data) | [VERIFIED] `PoseSearch.uplugin` |
| **Motion Warping** | adjusts root motion towards a target (vault/mantle alignment) | `MotionWarping.uplugin` version 0.1, `IsBetaVersion: true`; has `MotionWarpingCharacterAdapter` for ACharacter/CMC and a generic adapter interface | later, with traversal animations | [VERIFIED] descriptor and headers |
| **Animation Warping** (stride/orientation/slope warping) | foot/stride adaptation | plugin present, stable flags (1.0), disabled by default | useful soon with real locomotion clips | [VERIFIED] descriptor |
| **IK Rig / IK Retargeter / Control Rig** | retargeting, procedural rigs, foot IK | IKRig and ControlRig enabled by default | retargeting when the final character arrives; Control Rig for foot IK later | [VERIFIED] descriptors |
| **Full Body IK** | solver for IK Rig / Control Rig | plugin under `Experimental/` but not flagged experimental | later (hand placement on obstacles, interactions) | [VERIFIED] directory, [UNVERIFIED] maturity |
| **Enhanced Input** | key / gamepad mapping, contexts | used by the character | keep | [EXISTING] |
| **Animation Blueprint + state machine, Root Motion** | animation logic | template `ABP_Unarmed` + blend space | keep now; locomotion state machine needs the Core state (`ESpnMovementState`, already exported) | [EXISTING] |
| **Gameplay Tags** | state/ability tags | engine module present | use when states multiply (combat, injuries); not needed for locomotion | [VERIFIED] module exists, [PROPOSED] use |
| **Jolt Physics** | alternative physics engine | **not in the installed engine** (no plugin found) | **no** | [VERIFIED] absent; any use would be a third-party integration |
| **NVIDIA PhysX** | former UE physics engine | **not in UE 5.8** (not part of this engine install; when it was removed was not checked) | **no** | [VERIFIED] absent |

Short assessments:

- **Chaos vs. Jolt/PhysX**: nothing in the movement problems found here is a physics-engine problem. All defects traced back to CMC settings (capsule rounding, friction blend) or stale data. A second physics
  engine would add a dependency, a second collision world and no measurable gain for a kinematic character. Recommendation: no integration. [PROPOSED]
  Whether Chaos is bit-deterministic across machines/builds was not tested. [UNVERIFIED] Determinism matters for co-op only through the CMC's own client replay, which the project already handles via saved moves.
- **Mover**: experimental by the engine's own flags and README. A migration would touch the movement component, saved-move network code (replaced by Network Prediction / fixed tick), the
  Core bridge, 150+ gameplay QA checks, the diagnostics, and requires fixed-tick project settings (README). Its potential value (layered moves, modular modes, rollback) is real but not needed for M2A4.2–M2A4.5.
  Re-evaluate when the engine flag changes or when co-op rollback is scheduled. Risk: API churn (stated in the README). [VERIFIED] README, [PROPOSED] decision.
- **Motion Matching**: an animation-selection technique. It does not change movement physics and would not have prevented any defect in this report. It needs a pose database built from many locomotion clips
  (start/stop/turn/pivot/idle), which the project does not have (the template set is a blend space plus ~100 clips). CPU cost grows with database size; memory with the number of poses. Costs not measured here. [UNVERIFIED]
- **Motion Warping**: aligns animation root motion to a target transform (vault hand position, mantle top). It does not validate physics: the obstacle height check, clearance sweeps and capsule movement must still come from
  gameplay code (Core `Traversal` classification + sweeps). Warping only after validation. [PROPOSED]

## 5. Changes made in M2A4.1 (details and measurements: `MOVEMENT_REGRESSION_REPORT.md`)

1. **Ground steering** wired in: `USpnCharacterMovementComponent::CalcVelocity` calls `SpnMovement::SteerVelocity` → Core `SteerVelocity` for walking with orientation to movement; engine path otherwise (root motion, requested velocity, crouch, strafing, other modes).
2. **`PerchRadiusThreshold` = 14 cm** and **`bUseFlatBaseForFloorChecks` = true** as settings (`FSpnMovementSettings`), applied in `ApplySettings()`. Measured effect: jump-ups of 60 cm and higher are blocked
   at every speed (before: 60/70 cm were mounted). `bUseFlatBaseForFloorChecks` alone had no measurable effect on the cases tested; it is kept because both values were measured together.
2b. `bPreventGroundSlopeBoosting` / `ComputeSlideVector` override: the engine's falling-only guard applied on the ground. **No measurable effect** (see section 3); kept because the validated binaries contain it, first candidate for removal.
3. V2 level regenerated (stale asset, backup kept). New tests `SpnQA.Locomotion`, `SpnQA.MovementDemo`; QA experiment hooks (`-SpnQaPerch=`, `-SpnQaFlatBase=`) that never set a threshold.

## 6. Limitations still present

- **Late jump into a ledge as high as the jump apex** (Jump.late_jump_ledge_*): the horizontal speed collapses to 4–10 % at the wall and the capsule mounts the ledge 0.45 s after take-off. Cause: the CMC removes the velocity component into the wall in the air.
  This is a traversal-design question (ledge assist or mantle), planned for the vault milestone. [PROPOSED]
- 50 cm ledges are still cleared at **run** speed (550 cm/s) by the momentum, blocked at jog speed; 60 cm and higher block at every speed. [VERIFIED by measurement]
- The capsule centre stops 35 cm in front of a wall (radius 42): a 7 cm overlap that was not investigated (contact offset or test geometry). [UNVERIFIED cause]
- **Walking over 30 cm**: still possible (`SpnGame.Movement.Obstacles` 30 cm, `StairsAndRamps` 30 cm rise fail, as in the M2A2 baseline); 40 cm and higher block when walking. Cause not identified (section 3).
- The engine's walking step-up still uses sweeps: moving platforms, physics bodies as floors and uneven terrain were **not** tested in this milestone.
- Gamepad: sprint on left-thumbstick press is a placeholder binding (`DefaultSprintGamepadKey`). The confirmed DualSense layout (X jump, Circle dodge, Square context/attack, Triangle open) is **not implemented in code or tested**; only the keyboard path (E, Shift, Space, V) is QA-tested. `IMC_Default` was not opened (editor asset), so the actual jump mapping of the DualSense is [UNVERIFIED]. No mapping was changed.

## 7. Recommended architecture [PROPOSED]

1. Keep **CMC + Core rules**; extend the Core (it is the testable place) and keep the UE component a thin adapter. No second physics engine, no Mover migration now.
2. Add a Core **traversal query** (obstacle height, clearance, landing space) used by gameplay code, then drive animation through **Motion Warping** and optionally Pose Search later. Physical validation stays in sweeps.
3. Expose the Core movement state (`ESpnMovementState`, gait, speed, heading rate, local input) to the Animation Blueprint as the single source for animation (already available), so an animation upgrade never needs a movement change.
4. Replace the template mannequin locomotion by clips authored for the final character; use Animation Warping (stride/orientation) before investing in a Motion Matching database.
5. Introduce **Gameplay Tags** with the first combat/injury state; not for locomotion.
6. Keep the QA infrastructure as the safety net: real key input, per-frame traces, no teleport in demonstrations.

## 8. Migration risks

| Risk | Where | Note |
|---|---|---|
| Mover API churn | any Mover work | engine says so itself |
| Network replay of custom moves | CMC saved moves | the SPN flags are already replay-safe (NetworkSprint tests); steering depends only on Velocity, Acceleration and DeltaTime |
| Capsule shape changes | step/ledge behaviour | the perch/flat-base values were measured for capsule 42 × 96; a different capsule needs a new measurement |
| Motion Matching data | animation | needs hundreds of clips for credible quality [UNVERIFIED] |
| Perch threshold too high | ledges | 21 cm already blocks a 45 cm ledge that is below the apex (measured), so the usable range is narrow (12–16 cm) |

## 9. Performance considerations

- `CalcVelocity` replacement: one `SteerVelocity` call per character per tick (scalar math, no allocation). Not separately profiled (see the validation report); the game thread of the V3 performance run is 3.7 ms in total, so movement is a small share (not measured separately).
- `bUseFlatBaseForFloorChecks` uses a flat-base sweep for floor checks instead of the capsule sweep; cost not measured separately. [UNVERIFIED]
- FPS alone is not evidence of movement cost. A dedicated movement CPU measurement (many characters, `stat` groups) is proposed for M2A4.2.

## 10. Testing strategy

- Core: CTest for deterministic rules (`GroundSteeringTests`, `JumpModel`, `Traversal`).
- UE automation: `SpnGame.Movement.*` (settings, prediction flags, obstacles, stairs, sprint turn).
- Gameplay QA (`SpnQA.*`, real key events, video and traces): Movement, Turns, Jump, **Locomotion** (new), Doors, DoorReach, Buildings, BuildingAccess, ContinuousTraversal, **MovementDemo** (new).
- Rule: limits are derived from the settings; experiments may use `-SpnQa*` overrides but never decide a pass/fail threshold.

## 11. Future animation requirements [PROPOSED]

Locomotion clips for the final character at the four gaits with start/stop/pivot, jump (take-off, apex, landing at several speeds), step up/down, stairs up/down, idle variations, turn-in-place, door interaction;
later vault (low/medium), mantle (1–2 m), ledge hang, with root-motion curves and warp windows. Foot-lock curves for IK. Rig compatible with IK Retargeter.

## 12. Proposed next milestones (not approved)

- **M2A4.2 Traversal queries**: Core obstacle/clearance model, capsule/ledge sweeps, debug visualisation, QA for heights 20–220 cm; ledge assist decision for the late-jump case.
- **M2A4.3 Locomotion animation foundation**: ABP driven by Core state, stride/orientation warping, foot IK; start/stop/pivot clips.
- **M2A4.4 Vault and mantle prototype**: Motion Warping on validated queries, input contexts, collision-safe aborts.
- **M2A4.5 Uneven terrain and platforms**: slopes, steps, moving bases, physics bodies as floors, performance budget for 50+ characters.
