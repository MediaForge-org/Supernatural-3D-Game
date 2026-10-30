# OPUS_ESCALATION — walking up 30 cm ledges (`SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps`)

Status: **UNRESOLVED.** Time-boxed (about 60 minutes of focused investigation in Night Build 01, after the M2A4.1 attempts). The code is back at the committed state for these files (`git diff` of `SpnCharacterMovementComponent.*` and `SpnMovement.h` is empty). The two tests fail exactly as they did at the baseline; they were not weakened.

## Exact problem

`USpnCharacterMovementComponent` sets `MaxStepHeight = 25 cm`. A character that **walks** (no jump) crosses a 30 cm thin ledge and climbs a staircase with 30 cm risers. The tests (`SpnMovementMapAutomation.cpp:671` and `:739`) classify 30 cm as "JumpCandidate / must block when walking".

| Case (baseline, `logs/movement-baseline.log`) | Result |
|---|---|
| 25 cm thin obstacle, walking | crossed (max Z 117) |
| **30 cm** thin obstacle, walking | **crossed (max Z 122, end Z 119)** — expected: blocked |
| 40 cm thin obstacle, walking | blocked (max Z 97) |
| staircase 25 cm rise | climbed |
| **staircase 30 cm rise** | **climbed** — expected: blocked |
| staircase 40 cm rise | blocked |

(`StairsAndRamps`: 1 failure, `Obstacles`: 1 failure; the other 14 `SpnGame.Movement.*` tests of that run pass.)

## Relevant code

`Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnCharacterMovementComponent.cpp` (`ApplySettings` sets `MaxStepHeight`, `PerchRadiusThreshold = 14`, `bUseFlatBaseForFloorChecks = true`; `ComputeSlideVector` override with `bPreventGroundSlopeBoosting`). Engine reference: `CharacterMovementComponent.cpp` `StepUp`, `MoveAlongFloor`, `CanStepUp`.

## Reproduction

```
UnrealEditor-Cmd.exe Unreal\SupernaturalProt\SupernaturalProt.uproject -ExecCmds="Automation RunTests SpnGame.Movement.Obstacles+SpnGame.Movement.StairsAndRamps; Quit" -unattended -NullRHI -NoSound
```
About 3 minutes. Logs: `Docs/NightBuild01/logs/movement-baseline.log` (full 16-test run), `movement-fix1.log`, `movement-diag1.log`.

## Previous attempts and why they failed

1. **M2A4.1 (earlier session, per the comments in `SpnMovement.h`):** `PerchRadiusThreshold` 12 / 14 / 16 (identical results), `bUseFlatBaseForFloorChecks`, and a ground slope-boosting guard in `ComputeSlideVector`. None changed the 30 cm walking result.
2. **Night Build 01, attempt A — strict step-up check.** Overrode `CanStepUp`: refuse when a downward line trace just inside the contact face finds the obstacle top more than `MaxStepHeight + 1.5 cm` above the feet. Result: **no change** (30 cm still crossed). Instrumentation showed `CanStepUp` *is* called and refuses (`ledge top 37.9 cm` and `27.8 cm` above the feet; the feet sit about 2.8 cm below the floor top in this setup, so a 25 cm ledge measures 27.8 and would also be refused, i.e. this approach would break legitimate 25 cm steps).
3. **Night Build 01, attempt B — instrumentation of `MoveAlongFloor`.** Logged every ground move that lifted the capsule by more than 2 cm in one `MoveAlongFloor` call: **zero such events** during the whole `Obstacles` run, although the capsule reaches Z 122.

## What this tells us

The capsule does not get onto the 30 cm ledge through `StepUp` (attempt A refuses it) and not through a single large lift (attempt B). The remaining mechanism is **many small lifts**: each frame the move slides up the rounded capsule bottom along the ledge corner (`SlideAlongSurface` in `MoveAlongFloor`) by under 2 cm until the corner is below the capsule centre line — the known "rolling over a ledge corner" behaviour the earlier session already described. The final rest at Z 119 (27 cm above the floor) with X beyond the far face means the capsule ends perched on the top.

## Suggested next steps

* Log per-frame `Z`, `Velocity.Z`, `CurrentFloor.FloorDist` / walkable flag, movement mode and the `Hit.ImpactNormal` of the blocking hit for the 30 cm case with `spn.MoveDiag 1` (the recorder exists: `SpnMovementDiagnostics`); look for the frame series with Z rising 0.5-2 cm per frame.
* Try an override of `SlideAlongSurface` / `HandleImpact` for ground moves that clamps the vertical component of the slide to 0 unless the surface is walkable (`IsWalkable(Hit)` evaluated on the impact normal, not the sweep normal).
* Or question the spec: with a rounded capsule bottom (radius 42) the physically meaningful limit is above `MaxStepHeight`; the owner may prefer `MaxStepHeight` of about 20 cm so that 30 cm blocks (this changes what stairs with 25 cm risers do — a decision for the developer).
