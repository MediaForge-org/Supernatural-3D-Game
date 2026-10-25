# 0011 — M2-A: four gaits, human jump height, network-ready movement, movement test map — ENGINEERING PROPOSAL (IMPLEMENTED ON FEDORA, NOT YET RUN IN UNREAL)

Status: written 2026-10-25 for developer review. **Not master canon.** The values in section 2 come from the developer's M2-A work order (2026-10-25) and are *initial test values*, not final gameplay numbers. Everything in sections 3 and 4 was written on Fedora without Unreal Engine: **no Unreal code in this package has been compiled or run.** Windows verification steps are in [Windows-M2A-Handover.md](../Setup/Windows-M2A-Handover.md).

## 1. Master basis (consultation protocol, CLAUDE.md section 11)

| Source | Finding | Status |
|---|---|---|
| `bash Tools/master-search.sh` for jump height, gait, sprint, walk speed, movement speed (tiers 1 and 2) | **No hits.** The master contains no movement numbers. | nothing found |
| [Proposal 0009](0009-traversal-jump-and-vault-direction.md) §2, §3a (developer, 2026-10-22) | jump only by active input; human jump strength; height-matched low-obstacle movement later; animation must match movement; a 50 cm wall must not cause an exaggerated jump; jump key = cross | BESCHLOSSEN (as stated in the Kampfsystem register) |
| [Kampfsystem 08](../Game/Kampfsystem/08-Kampfbewegung-und-Traversal-Integration.md) §3 (COMBAT-DEC-BT-001..008, 011, 012, 016) | same decisions; injury/exhaustion may affect traversal later; sprinting trains endurance (RICHTUNG); movement local/authoritative per player (VORSCHLAG) | as listed there |
| `CLAUDE.md` section 6 | co-op up to four players, host-save-authoritative; avoid single-player-only assumptions | guardrail |

Everything else below (numbers, key bindings, class layout) is **our own proposal** and is marked as such.

## 2. Decisions taken by the work order (2026-10-25)
- Vertical jump height **45 cm** (configurable, initial test value, not final for all characters). Active input only; no automatic jump, no double jump, no supernatural air control; the vertical jump height is not the maximum obstacle height.
- Four gaits: Walk 1.4, Jog 3.0, Run 5.0, Sprint 8.0 m/s (provisional), with natural acceleration and braking, analog input, smooth transitions, configurable parameters.
- Sprint must not be a local-only `MaxWalkSpeed` change (client/server divergence); integrate with CharacterMovement prediction.
- Test map `Lvl_SpnMovementTest`, about 300 x 300 m, a development environment and **not** a geographic excerpt of the open world.

## 3. What was built (all PLACEHOLDER test values)

### 3.1 Portable core (`Core/`, tested on Fedora)
`spn/core/Movement.h` / `src/Movement.cpp`: `Gait`, `MovementConfig`, `Validate`, `GaitSpeed/GaitAcceleration`, `ResolveGait` (sprint only on request, below the gait ceiling and with input within 50 degrees of straight ahead, else the chosen base gait), `DirectionSpeedScale` (backward 0.6, strafe 0.85), `JumpVelocityForHeight` / `JumpApexHeight` (`v = sqrt(2 g h)`), `ClassifyObstacle` (StepUp / Jump / Blocked from the character's own step and jump heights; **no new thresholds, no vault/mantle**). `GaitLimits` (gait ceiling, speed multiplier) is the hook for exhaustion/injury; nothing sets it yet.

### 3.2 Unreal bridge (`SpnCore` module)
`SpnMovement.h/.cpp`: `FSpnMovementSettings` (USTRUCT, centimetres, all tunables), `ESpnGait`, `ESpnObstacleClass`, and functions that forward to the core.

### 3.3 Movement component (`SupernaturalProt` module)
`Spn/SpnCharacterMovementComponent.*` (`USpnCharacterMovementComponent`, no tick):
- `GetMaxSpeed()` / `GetMaxAcceleration()` return the effective gait's values while walking; `MaxWalkSpeed` is **not** changed at runtime.
- **Prediction:** the base gait (Walk/Jog/Run) and the sprint request travel in the compressed move flags `FLAG_Custom_0..2` through a custom `FSavedMove_Character` (`GetCompressedFlags`, `SetMoveFor`, `PrepMoveFor` for replay, `CanCombineWith` refusing to merge different requests) and a custom `FNetworkPredictionData_Client_Character`. The server resolves the gait itself from its own limits and the move's input; a forged value 3 is read as Run. This is the standard Unreal pattern; it is **unverified here**.
- Falling keeps the take-off momentum (`GetMaxSpeed` in the air = max(gait speed, current speed)); `BrakingDecelerationFalling` is 0, air control 0.05 (the template had 0.35 and 1500).
- Jump: `JumpZVelocity` is derived from `Settings.JumpHeight` and the real gravity (`GetGravityZ()` incl. `GravityScale`) at `InitializeComponent` and `BeginPlay`, after Blueprint values were loaded. `RefreshJumpVelocity()` must be called if gravity changes at runtime (an override of `DoJump` would do this per jump; skipped because its signature changed between engine versions and could not be checked).
- Braking is constant (`BrakingDecelerationWalking`, `BrakingFriction` 0); turning follows `GroundFriction` (`TurnFriction`).
- `GetObservedGait()` (speed-based, valid on every proxy) and `GetEffectiveGait()` are the animation parameters.
- Backward/strafe factors apply only when the character does **not** orient to its movement. The current character does (`bOrientRotationToMovement`), so they are inactive until an aiming/lock-on mode exists.

`ASupernaturalProtCharacter` now creates this component (`SetDefaultSubobjectClass`), forwards `DoSprintStart/End` to it, adds `DoCycleGait` (key **V**, placeholder) and L3 for sprint, and sets `JumpMaxCount = 1`, `JumpMaxHoldTime = 0`. `SprintSpeed` and the `MaxWalkSpeed` swapping were removed.

### 3.4 Play-area guard
`ASpnPlayAreaGuard`: a box volume; when a pawn leaves it, the pawn is put back at `SafeLocation` on the authority (no tick, end-overlap event only). The map's walls are the first barrier.

### 3.5 Test map
`Unreal/SupernaturalProt/Tools/spn_movement_map_layout.py` (pure data, deterministic) describes 319 shapes + 2 doors; `create_spn_movement_test_map.py` spawns them in the editor and saves **`/Game/SpnTest/Lvl_SpnMovementTest`**. **The `.umap` does not exist yet.** Contents: 300 x 300 m floor; four 10 m thick, 20 m high boundary walls; thin and thick obstacle rows (20, 30, 40, 50, 60, 80, 100, 120, 150 cm); ramps 5, 10, 15, 20, 30, 40, 50 degrees; six staircases (rise 15, 18, 22, 30, 40, 50 cm); three half-buried-sphere hills; a terraced hill (20 cm steps); 120 low tilted rocks; five surface patches (visual materials only); a camera alley with a 230 cm low section; a hut, a two-storey house (interior stairs, doorways, windows, one `ASpnTestDoor`) and a warehouse (100 cm dock, loading ramp, pillars). Nothing is bought or downloaded; only engine shapes and existing prototype materials.

### 3.6 Tests
| Test | Where | Ran? |
|---|---|---|
| 9 core movement test cases (`Tests/Core/MovementTests.cpp`) | Fedora, 4 presets | **yes**, see section 5 |
| 19 Python layout checks | Fedora, also as ctest `movement_test_map_layout` | **yes** |
| `SpnGame.Movement.Settings`, `.CharacterDefaults`, `.PredictionFlags` | Unreal, no map | no |
| `SpnGame.Movement.Jump`, `.Gaits`, `.Obstacles`, `.StairsAndRamps`, `.Boundary`, `.AlleyCamera`, `.MapLayout` | Unreal, need the generated map | no |
| `SpnGame.TestMap.M1Traversal` (adapted, see 4.2) | Unreal | no |
| `SpnGame.Movement.NetworkSprint` | `Tools/prepared/*.cpp.txt`, **not in the build** | no |

## 4. Findings and consequences you should decide on

### 4.1 With a 45 cm jump the jump adds no obstacle capability
`MaxStepHeight` is 45 cm, the jump apex is 45 cm. A ledge at or below 45 cm is walked up; above 45 cm a standing jump cannot reach it (the feet rise at most 45 cm). Consequently **obstacles of 50 cm and higher cannot be crossed at all** until the traversal milestone (vault/mantle) exists, and the 20/30/40 cm obstacles never need a jump. The jump still crosses gaps. This follows from the given numbers; it is not a bug. Options for later: lower `MaxStepHeight` (a real human steps up about 30 to 40 cm) so the 40 to 45 cm range becomes "jump" territory, or accept it until vaulting exists. Decision needed from you; the code keeps both values configurable and independent.

### 4.2 M1 test adaptation
The M1 wall is 100 cm. It was crossable only because the template jump reached 127 cm (Proposal 0009 §3). `SpnGame.TestMap.M1Traversal` now asserts the opposite for the wall-jump case (the wall is not crossed by jumping); the sprint check uses the new request API; the diagonal wall case runs 3.5 s instead of 1.5 s because the default speed is 3 m/s instead of 5 m/s. No test was disabled.

### 4.3 Version note
Earlier documents state UE 5.8.3; the work order says 5.8.4. The `.uproject` uses a GUID engine association (`{DE4DD34C-...}`) and the content was saved by 5.6 headers (`++UE5+Release-5.6` in assets). See the handover for the check.

### 4.4 Defaults I chose (change freely, all in `FSpnMovementSettings`)
Default base gait **Jog**; accelerations 2.0 / 3.0 / 3.5 / 4.0 m/s² (Walk..Sprint); braking 8 m/s²; turn friction 8 (engine default); backward 0.6, strafe 0.85; sprint cone 50 degrees; air control 0.05; air braking 0; key V cycles Walk/Jog/Run; L3 sprints. None of these comes from the work order or the master.

### 4.5 Sprint and turning
Sprint needs input within 50 degrees of the character's facing. With `bOrientRotationToMovement` the character turns towards the input at 500 deg/s, so a sharp direction change briefly drops from Sprint to the base gait (about 0.2 to 0.4 s) and the character brakes at 8 m/s² during that time. This is intended as "credible direction changes", but only play-testing can say whether it feels right.

### 4.6 Not covered
Exhaustion, injuries, load (hooks only); replication of `SetMovementLimits` (must be set identically on server and owning client); vault/mantle; crouch; animation clips for run/sprint; surface-dependent footsteps (the project has no physical materials); NPC use of the component (the base `UCharacterMovementComponent` still works for NPCs).

## 5. Fedora results (2026-10-25)
Core + layout: 208 of 208 ctest cases passed in all four presets (Clang and GCC, each plain and with ASan/UBSan; warnings are errors). The 19 Python layout checks pass; two runs of the layout generator give byte-identical JSON. The map script was executed against a fake of the editor API (checks only the script's own logic; 326 actors). `git diff --check` is clean. The two broken README anchors found by `check_links.py` existed before this work. No Unreal build, no Unreal test.

## 6. Open questions
1. Keep `MaxStepHeight` 45 cm or lower it (4.1)?
2. Are the defaults in 4.4 acceptable as a starting point?
3. Which gaits should combat/aiming force (e.g. no sprint with a drawn weapon)? Not in the sources.
