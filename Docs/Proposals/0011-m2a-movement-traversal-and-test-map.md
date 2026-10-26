# 0011 — M2-A / M2-A.1: four gaits, human jump and step height, network-ready movement, movement test map — ENGINEERING PROPOSAL (IMPLEMENTED ON FEDORA, NOT YET BUILT OR RUN IN UNREAL)

Status: M2-A written 2026-10-25 (committed as `bd62eb6`), M2-A.1 written 2026-10-26 for developer review. **Not master canon.** The values in section 2 come from the developer's work orders and are *initial test values*, not final gameplay numbers. Everything in sections 3 to 5 was written on Fedora without Unreal Engine: **no Unreal code in this package has been compiled by Unreal or run in Unreal.** What was checked instead is listed in section 7, with its limits. Windows verification steps are in [Windows-M2A-Handover.md](../Setup/Windows-M2A-Handover.md). **M2-A is not accepted** before the criteria in section 9 are met.

## 1. Master basis (consultation protocol, CLAUDE.md section 11)

| Source | Finding | Status |
|---|---|---|
| `bash Tools/master-search.sh` for jump height, gait, sprint, walk speed, movement speed, step height, Stufenhöhe, vault, mantle, step-up (tiers 1 and 2; searched 2026-10-25 and 2026-10-26) | **No hits.** The master contains no movement numbers and no step height. | nothing found |
| [Proposal 0009](0009-traversal-jump-and-vault-direction.md) §2, §3a (developer, 2026-10-22) | jump only by active input; human jump strength; height-matched low-obstacle movement later; animation must match movement; a 50 cm wall must not cause an exaggerated jump; jump key = cross | BESCHLOSSEN (as stated in the Kampfsystem register) |
| [Kampfsystem 08](../Game/Kampfsystem/08-Kampfbewegung-und-Traversal-Integration.md) §3 (COMBAT-DEC-BT-001..008, 011, 012, 016) | same decisions; injury/exhaustion may affect traversal later; sprinting trains endurance (RICHTUNG); movement local/authoritative per player (VORSCHLAG) | as listed there |
| [Kampfsystem 06](../Game/Kampfsystem/06-Steuerung-und-Eingabekontext.md) §2, COMBAT-DEC-ST-005, -009, -017, -018 | bound today: Space, Shift, E, cross = jump; **no pad binding for sprint or interact**; slow, quiet walking comes from a small stick deflection, not from a sneak key; keyboard/mouse combat bindings and input priorities are OPEN | as listed there |
| `CLAUDE.md` section 6 | co-op up to four players, host-save-authoritative; avoid single-player-only assumptions | guardrail |

Everything else below (numbers, key bindings, class layout) is **our own proposal** and is marked as such.

## 2. Decisions taken by the work orders
**M2-A (2026-10-25).** Vertical jump height **45 cm** (configurable). Active input only; no automatic jump, no double jump, no supernatural air control; the vertical jump height is not the maximum obstacle height. Four gaits: Walk 1.4, Jog 3.0, Run 5.0, Sprint 8.0 m/s (provisional), natural acceleration and braking, analog input, configurable. Sprint must not be a local-only `MaxWalkSpeed` change; integrate with CharacterMovement prediction. Test map `Lvl_SpnMovementTest`, about 300 x 300 m, a development environment and **not** a geographic excerpt of the open world.

**M2-A.1 (2026-10-26).** Normal maximum **step-up height 25 cm**, independent of the **45 cm** jump height; both configurable on their own. A 45 cm wall must not be crossed by normal walking. Vault and mantle stay a later milestone. No uncontrolled growth in scope: stability, maintainability and testability first. The sprint interruption in sharp turns may be fixed if a simple, safe fix exists; larger prediction changes are postponed. No competing second state machine. Camera: only prepare tests, no larger changes. Controls: do not change existing bindings without reason, document conflicts with combat, set no final new bindings. No acceptance before a real Unreal build, a generated map, executed Unreal tests, manual movement tests and network checks.

## 3. What exists

### 3.1 Portable core (`Core/`, tested on Fedora)
`spn/core/Movement.h` / `src/Movement.cpp` (units SI, metres):
- `Gait`, `MovementConfig` (defaults below), `Validate`, `GaitSpeed` / `GaitAcceleration` (an out-of-range enum value is clamped instead of indexing past the array), `GaitLimits` (hook for exhaustion/injury, nothing sets it yet).
- `ResolveGait(config, base, sprintRequested, limits, intent, currentSpeed)`: sprint only on request, below the gait ceiling, and with input within `sprintMaxAngleDegrees` (50) of straight ahead; **once the character is at sprint speed the wider `sprintSustainAngleDegrees` (100) applies** (section 4.4).
- `ObservedGait` (nearest gait speed), `ClassifyMotion` / `MovementState` (Idle, Walk, Jog, Run, Sprint, Jumping, Falling, Landing) as a **pure function of velocity, floor contact and time since touch-down** (section 4.5).
- `DirectionSpeedScale` (backward 0.6, strafe 0.85), `JumpVelocityForHeight` / `JumpApexHeight` (`v = sqrt(2 g h)`).
- `ClassifyObstacle`: `StepUp` (height <= step height), `JumpCandidate` (above the step height and **strictly below** the jump apex), `Blocked` (otherwise, also NaN/inf). The old class `Jump` was renamed: it claimed a jump gets over every ledge up to the jump height, which depends on run-up, timing, depth and collision.
- `MovementConfig` defaults: step height **0.25 m** (was 0.45), jump height 0.45 m, turn rate 500 deg/s, minimum analog speed 0.2 m/s, idle speed 0.05 m/s, landing time 0.25 s. Step height and jump height are validated and used **independently** (a step height above the jump height is valid, if odd).

### 3.2 Unreal bridge (`SpnCore` module)
`SpnMovement.h/.cpp`: `FSpnMovementSettings` (USTRUCT, centimetres, all tunables incl. the new `MaxStepHeight` 25, `SprintSustainAngleDegrees`, `TurnRateDegrees`, `MinAnalogSpeed`, `IdleSpeed`, `LandingSeconds`), `ESpnGait`, `ESpnMovementState`, `ESpnObstacleClass` (`StepUp`, `JumpCandidate`, `Blocked`), and functions that forward to the core. `SupernaturalProt.Build.cs` now lists `SpnCore` as a **public** dependency because a public header of the game module includes `SpnMovement.h`.

### 3.3 Movement component (`SupernaturalProt` module)
`Spn/SpnCharacterMovementComponent.*` (`USpnCharacterMovementComponent`, no tick):
- `GetMaxSpeed()` / `GetMaxAcceleration()` return the effective gait's values while walking; `MaxWalkSpeed` is **not** changed at runtime.
- **Prediction (unchanged wire format in M2-A.1):** the base gait (Walk/Jog/Run) and the sprint request travel in the compressed move flags `FLAG_Custom_0..2` through a custom `FSavedMove_Character` and `FNetworkPredictionData_Client_Character`. The server resolves the gait itself from its own limits and the move's input; a forged value 3 is read as Run. The new sprint-sustain test reads only the **velocity**, which is predicted state, so no new flag or replicated variable was needed. Standard pattern, **unverified here**.
- Falling keeps the take-off momentum (`GetMaxSpeed` in the air = max(gait speed, current speed)); air braking 0, air control 0.05.
- Jump: `JumpZVelocity` is derived from `Settings.JumpHeight` and the real gravity at `InitializeComponent` and `BeginPlay`; `MaxStepHeight` comes from `Settings.MaxStepHeight`. `ApplySettings()` validates first and keeps the old values on error. Both values are applied by separate lines and tested separately.
- `ApplySettings()` now also applies `RotationRate` (turn rate) and `MinAnalogWalkSpeed`; the character constructor no longer sets them (a duplicate that `ApplySettings` would have overridden anyway).
- **Animation interface:** `GetMovementState()` (Idle/Walk/Jog/Run/Sprint/Jumping/Falling/Landing), `GetObservedGait()`, `GetEffectiveGait()`, `GetSecondsSinceLanding()`. `OnMovementModeChanged` records the touch-down time (falling -> ground); nothing is stored or replicated, so any proxy computes the same state from its velocity.
- Backward/strafe factors apply only when the character does **not** orient to its movement (inactive today).

`ASupernaturalProtCharacter` creates this component (`SetDefaultSubobjectClass`), forwards `DoSprintStart/End` to it, adds `DoCycleGait` (key **V**, placeholder), sprint on L3 through the new property `DefaultSprintGamepadKey` (set it to "Invalid" to map none), `JumpMaxCount = 1`, `JumpMaxHoldTime = 0`.

### 3.4 Play-area guard
`ASpnPlayAreaGuard`: a box volume; when a pawn leaves it, the pawn is put back on the authority at the **reset point nearest to where it left** (ground plane), or at `SafeLocation` when there are none (no tick, end-overlap event only). The map's walls are the first barrier.

### 3.5 Test map (areas A to H)
`Unreal/SupernaturalProt/Tools/spn_movement_map_layout.py` (pure data, deterministic) describes **393 shapes + 2 doors + 14 reset points**; `create_spn_movement_test_map.py` spawns them in the editor and saves **`/Game/SpnTest/Lvl_SpnMovementTest`**. **The `.umap` does not exist yet.**

| Area | Zones | Content |
|---|---|---|
| A flat | plaza, speed lane | open plaza at the player start; a lane along Y (x = -125 m, 260 m long, only 1 cm markers every 10 m) for the speed tests |
| B slopes | ramps, hills | ramps 5, 10, 15, 20, 30, 40, 50 degrees with side rails, landing and end rail; three domed hills (rim slope under the walkable angle) |
| C stairs | stairs | seven staircases of ten steps, rise 15, 18, 22, **25**, 30, 40, 50 cm, with side walls and end rail |
| D obstacles | obstacles | thin (40 cm deep) and thick (200 cm deep) rows at 20, **25**, 30, 40, **45**, 50, 60, 75, 80, 100, 120, 150 cm |
| E uneven | terraces, rocks, surfaces | terraced hill in 20 cm steps; 80 low tilted rocks (every edge under the step height); five visual surface patches |
| F camera | alley, pillars | 150 cm alley with a 230 cm low section; 5 x 5 pillar field (4 m apart) |
| G buildings | hut, house, warehouse | hut (800 x 600, door, window); two-storey house (hall, rooms, doorways, 17-step interior stairs with rail, windows, one door each); warehouse with 100 cm dock, loading ramp with rails, pillars, raised windows |
| H boundary | four walls, guard, reset points | 10 m thick, 20 m high walls; guard volume; reset point beside every zone |

No open drop edges: every ramp, landing, staircase, the house stairwell and the dock ramp have rails or side walls of 120 cm (more than step height plus jump apex); warehouse doorways to the 1 m drop were replaced by raised windows. Nothing is bought or downloaded; only engine shapes and existing prototype materials. Actor budget: 393 static mesh actors plus lights, doors, guard (no tick, static); not measured.

### 3.6 Tests
| Test | Where | Ran? |
|---|---|---|
| 215 core doctest cases incl. the movement cases in `Tests/Core/MovementTests.cpp` (config validation, independence of step and jump height, obstacle table for the nine heights, sprint hysteresis, observed gait, motion states, numeric edges, out-of-range gaits) | Fedora, 4 presets | **yes**, section 7 |
| 46 Python checks (`test_spn_movement_map_layout.py`): determinism incl. other hash seeds, unique labels, bounds, zones, areas, obstacle heights and classes, ramps, stairs, rails, openings, doors in doorways, reset points, guard, speed lane, **consistency with the editor script, with the C++ test constants and with the C++/core defaults** | Fedora, also as ctest `movement_test_map_layout` | **yes** |
| Stub syntax check of the bridge, the movement component and the guard (`Tools/stubcheck`, ctest `unreal_stub_check`, Linux only) | Fedora | **yes**, limits in section 7 |
| `SpnGame.Movement.Settings`, `.CharacterDefaults`, `.PredictionFlags` | Unreal, no map | **no** |
| `SpnGame.Movement.MapLayout`, `.Jump`, `.JumpMoving`, `.Gaits`, `.SprintTurn`, `.StepJumpIndependence`, `.Obstacles`, `.StairsAndRamps`, `.Boundary`, `.AlleyCamera`, `.CameraClearance` | Unreal, need the generated map | **no** |
| `SpnGame.TestMap.M1Layout` / `M1Traversal` (adapted, see 4.2) | Unreal | **no** |
| `SpnGame.Movement.NetworkSprint` | `Tools/prepared/*.cpp.txt`, **not in the build** | **no** |

## 4. Findings and consequences

### 4.1 Step height 25 cm and jump height 45 cm (decided by the work order)
With the two values apart, a ledge up to 25 cm is walked up, a ledge between 25 and 45 cm is a **jump candidate**, and from 45 cm up nothing available today overcomes it (the feet rise at most 45 cm, and only for an instant). Obstacles of 50 cm and higher are impassable until vault/mantle exist. Whether a jump really gets over a 30 or 40 cm ledge, and whether the engine still accepts the 25 cm ledge exactly at the limit, **cannot be known without Unreal**: the Unreal obstacle test asserts only the clear cases (20 cm crossed; 30 cm and higher not crossed by walking; 50 cm and higher not crossed even with jumps) and **reports** the measured result for 25, 30, 40 and 45 cm (`MEASURED (not asserted)` in the log). Those numbers are what you need to decide on `MaxStepHeight`.

### 4.2 M1 test adaptation
The M1 wall is 100 cm; it was crossable only because the template jump reached 127 cm (Proposal 0009 §3). `SpnGame.TestMap.M1Traversal` asserts the opposite for the wall-jump case; the sprint check uses the new request API; the diagonal wall case runs 3.5 s. The M1 stair check now compares with the default step height (25 cm; the M1 stairs rise 18 cm, so they stay walkable). No test was disabled.

### 4.3 Version note
Earlier documents state UE 5.8.3; the work orders say 5.8.4. The `.uproject` uses a GUID engine association and the content was saved by 5.6 headers. See the handover.

### 4.4 Sprint in sharp turns (fixed, unverified in Unreal)
Cause: sprint needed input within 50 degrees of the character's **facing**; with `bOrientRotationToMovement` the character turns only at the turn rate, so a 90 degree turn dropped the sprint for a few tenths of a second and the over-speed braking (8 m/s²) took 2 to 3 m/s. Fix: once the speed is in the sprint band (nearest gait = Sprint, above 6.5 m/s) the allowed angle widens to 100 degrees (`SprintSustainAngleDegrees`); starting a sprint still needs 50. Beyond 100 degrees the sprint ends as before. The fix reads only the velocity, so client and server decide identically without a new flag; it needed no change to the prediction structure. `SpnGame.Movement.SprintTurn` tests 90 degrees left/right (sprint kept, speed stays above 90 %) and 150 degrees (sprint ended).
Remaining risk (not verified): `GetMaxAcceleration()` depends on the gait, and the engine evaluates the analog modifier as `|Acceleration| / GetMaxAcceleration()` after scaling the input with the same function. In a frame where the gait changes between those two calls the modifier can be below 1 for one frame (for example 300/400 = 0.75 when sprint starts). The cap applies to the input-driven speed only; a character already faster than the cap keeps its speed, so a visible effect is not expected, but it was not observed.

### 4.5 Movement states (no second state machine)
The question was whether the structure can tell Idle, Walk, Jog, Run, Sprint, Jump, Falling and Landing apart. Walk to Sprint, Falling and Idle could be read from the speed and `IsFalling()`; "rising" and "landing" could not. `GetMovementState()` adds them without storing a state: velocity (horizontal speed, vertical sign), floor contact and the time of the last touch-down. Nothing is replicated; a proxy derives the same state from the velocity it already has. The touch-down time comes from `OnMovementModeChanged` (falling to ground); **whether this fires on simulated proxies in 5.8 is unverified**, so Landing may not show on remote characters. The apex counts as Falling. `SpnGame.Movement.Jump` asserts the order Jumping, Falling, Landing, Idle for a standing jump.

### 4.6 Jump behaviour
Launch velocity `v = sqrt(2 g h)` from the real gravity (297 cm/s at 980 cm/s²), flight time `2 v / g` (about 0.61 s for 45 cm; the template jump took about 1.02 s), momentum kept in the air, air control 0.05, no air braking, one jump per press (`JumpMaxCount = 1`, `JumpMaxHoldTime = 0`), no double jump, no automatic hop when the key is held. `SpnGame.Movement.Jump` checks apex, `JumpZVelocity` against `sqrt(2 g h)`, air time, take-off count (also with the key held) and the absence of horizontal drift when standing; `.JumpMoving` checks the same at jog, run and sprint with "no speed gain, at most 10 % loss in the air". Jump during sprint is therefore covered by a test, not by a claim.

### 4.7 Camera review (static, nothing changed)
Existing: spring arm with collision test (probe channel Camera, engine default probe size 12 cm), mesh hidden when the camera is under 70 cm from the pivot, arm shortened at once and grown back at 300 cm/s (character `Tick`). Notes: the arm limit reads the camera position of the previous frame (one frame of lag, harmless); there is no camera lag, so a fast turn moves the camera at once; stairs and uneven ground get no special handling beyond the arm collision; the engine's default view pitch limits stay. Prepared tests: `AlleyCamera` (low ceiling, narrow walls) and the new `CameraClearance` (hut interior, house hall, half way up the house stairs, between pillars, on a staircase, low alley; the view swings 240 degrees/s with pitch moving between -35 and +10 degrees; every frame a 4 cm sphere at the camera must not overlap anything that blocks the camera channel). No camera code was changed because none of this could be run.

### 4.8 Controls (documentation; no new final bindings)
| Action | Keyboard / mouse | Xbox-compatible pad | DualSense | Source / status |
|---|---|---|---|---|
| Move / look | WASD / mouse | left / right stick | left / right stick | template input assets; analog stick magnitude scales the speed (tested in `Gaits`, half and quarter stick) |
| Jump | Space | A | Cross | Proposal 0009 §3a, decided |
| Sprint (hold) | Left Shift | L3 (stick click) | L3 | Shift: README §26.1. **L3 is an M2-A placeholder, not decided** (Kampfsystem 06: no pad sprint binding exists); property `DefaultSprintGamepadKey`, can be cleared |
| Cycle base gait | V | none | none | **M2-A placeholder, not decided**; pads cannot change the gait |
| Interact | E | none | none | Kampfsystem 06: pad binding missing |
The pad keys are the engine's generic gamepad keys; whether Windows reports a DualSense as such depends on the driver/Steam Input setup and is **not verified**. DualSense specifics (touchpad, adaptive triggers, haptics) are not used.
Possible conflicts with combat (all OPEN there): L3 is unassigned in the combat register but is a common stick-click action elsewhere; `V` is unassigned for keyboard/mouse combat (COMBAT-DEC-ST-017 is open); the discrete gait key overlaps with COMBAT-DEC-ST-005 (slow walking from a small stick deflection, no mandatory sneak key): both exist now, the key can be dropped or become keyboard-only; Shift and Space are tied to sprint and jump while combat input priorities are open (COMBAT-DEC-ST-018); cross means upshift while driving (COMBAT-DEC-ST-010), so mode-dependent bindings need the decision CQ-25.

### 4.9 Static Unreal code review (limits first)
A static review is **not** a build. Reviewed by reading: headers and includes, `UCLASS/USTRUCT/UENUM/UPROPERTY` use, module dependencies (`SpnCore` public for the game module, `Core`, `CoreUObject`, `Engine` for `SpnCore`), `SPNCORE_API` on the struct and all bridge functions, the movement component (constructor, `InitializeComponent`, overrides), saved move and prediction data, the guard, the automation tests and the editor script. Found and fixed: unused constant in the component, missing `class UPrimitiveComponent;` forward declaration in the guard header, the Gaits test running into the ramp zone, `RefreshJumpVelocity()` asking for the gravity of a component without a world (a null-world access in the engine; guarded now), a stale constructor duplicate of `RotationRate`/`MinAnalogWalkSpeed`. Deliberately left: `FMath::Max(float, double)` mixes, because the M1 code (`LastDoorX = FMath::Max(LastDoorX, ...X)`) compiled with UE 5.8 on Windows; the new code avoids them where cheap anyway.
**Assumptions about the engine API that were not checked against an engine** (look here first on a compile error): `FSavedMove_Character::{FLAG_Custom_0..3, Clear, GetCompressedFlags, CanCombineWith(const FSavedMovePtr&, ACharacter*, float) const, SetMoveFor, PrepMoveFor}`; `FNetworkPredictionData_Client_Character::AllocateNewMove` returning `FSavedMovePtr`; `UCharacterMovementComponent::{GetPredictionData_Client() const, ClientPredictionData, UpdateFromCompressedFlags(uint8), OnMovementModeChanged(EMovementMode, uint8), GetMaxSpeed, GetMaxAcceleration, MinAnalogWalkSpeed, BrakingFriction, bUseSeparateBrakingFriction, BrakingDecelerationFalling}`; `UMovementComponent::GetGravityZ`; `FAutomationTestBase::{AddExpectedError, AddInfo}`; `UWorld::OverlapBlockingTestByChannel`; `SCENE_QUERY_STAT`; `FKey::IsValid`. The first four were the standard pattern in earlier engine versions.

### 4.10 Performance review (no measurements, no FPS claims)
- `GetMaxSpeed`/`GetMaxAcceleration` rebuild the small core config (about 20 doubles, no heap) two to three times per movement update. Cost is negligible by construction; a cache would need invalidation on every edit of the `Settings` struct (also from Blueprints), which is more risk than gain. **Not changed.**
- No tick in the component and in the guard; the character's existing tick does two distance computations. Tests allocate only inside the editor test run.
- Actors: 393 static, non-ticking mesh actors, each its own draw; instancing or merging would cut the count but make the labelled test objects harder to find. Not done; measure first (handover step 14).
- Collision: boxes, three half spheres and 25 + 6 cylinders from engine shapes with simple collision; no complex collision against triangles. Not measured.
- Generator: layout build 3.4 ms in Python; the editor script now caches the meshes and the actor subsystem (it called `load_asset` for every one of the 393 shapes). Spawn time in the editor not measured.

### 4.11 Not covered
Exhaustion, injuries, load (hooks only); replication of `SetMovementLimits` and of runtime edits of `Settings` (server and owning client must start from the same asset values); vault/mantle; crouch; animation clips for run/sprint; surface-dependent footsteps (no physical materials); NPC use of the component; DualSense features.

### 4.12 Animation
Existing assets: `ABP_Unarmed`, blendspace `BS_Idle_Walk_Run`, Walk/Jog sets in eight directions, `MM_Jump`, `MM_Fall_Loop`, `MM_Land`. No run/sprint clips. The blendspace speed range cannot be read from the binary files. **Not checked:** foot sliding at 140/300/500/800 cm/s, and whether jump start/fall/land fit the new 0.61 s flight time. C++ is prepared (`GetMovementState`, `GetObservedGait`, `GetEffectiveGait`, `Velocity`, `GetSecondsSinceLanding`); the editor steps are in the handover. Nothing here may be called "animation done".

## 5. Defaults I chose (change freely, all in `FSpnMovementSettings`)
Default base gait **Jog**; accelerations 2.0 / 3.0 / 3.5 / 4.0 m/s² (Walk..Sprint); braking 8 m/s²; turn friction 8 (engine default); turn rate 500 deg/s (from the template); minimum analog speed 20 cm/s (template 20); backward 0.6, strafe 0.85; sprint start cone 50 degrees, sustain cone 100 degrees; air control 0.05; air braking 0; idle speed 5 cm/s; landing state 0.25 s; key V cycles Walk/Jog/Run; L3 sprints. None of these comes from a work order or the master.

## 6. Fedora results
**2026-10-25 (M2-A):** core + layout 208 of 208 in four presets.
**2026-10-26 (M2-A.1):** all four presets (Clang and GCC, each plain and with ASan/UBSan, warnings as errors) pass 217 of 217 ctest tests (215 core doctest cases, the Python layout test, the stub check). The Python layout checks pass 46 of 46 when run directly; changing a C++ test constant, or the C++ step height, makes the consistency checks fail (tried by hand, then reverted). The generator gives the same JSON in processes with different hash seeds. The editor script ran against a fake of the editor API (393 shapes, 2 doors, 14 reset points; checks only the script's logic). `git diff --check` clean. No Unreal build, no Unreal test.

## 7. What the Fedora checks can and cannot show
- **Core tests:** the rules are correct as written. They say nothing about Character Movement behaviour.
- **Python tests:** the layout data is consistent with itself, with the editor script (keys, properties, class paths), with the C++ test constants and with the default values. They do not show that Unreal builds the geometry as described (collision of the engine shapes, scale, materials).
- **Stub check:** compiles the bridge, the component and the guard against hand-written engine stand-ins. It catches typos and header/source mismatches (it found two). It cannot find a wrong engine signature, because the stand-in states what the author believes. The automation tests and the character are not covered by it.

## 8. Open decisions
1. Keep `MaxStepHeight` at 25 cm after seeing the measured 25/30/40/45 cm results (4.1)?
2. Are the defaults in section 5 acceptable as a starting point (sustain angle 100, landing time 0.25 s, turn rate 500)?
3. L3 as the pad sprint key and V as the gait key (4.8): keep, change or drop? Gait on a pad?
4. Which gaits should combat/aiming force (for example no sprint with a drawn weapon)? Not in the sources.
5. Engine version 5.8.3 or 5.8.4 (4.3).

## 9. Acceptance (M2-A is not accepted before all of these)
Unreal editor build succeeds; the map is generated and saved; the Unreal tests run (numbers reported); the movement is played by hand (including a controller); the main network functions are checked (listen server and client, with latency); the animation findings are turned into editor tasks. Details and commands: handover, sections 3 and 4.
