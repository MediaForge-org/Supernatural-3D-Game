# OPUS_ESCALATION

> **Update 2026-10-30 (movement investigation): cases 1, 2 and 5 are RESOLVED with verified root causes; cases 3 and 4 have a verified mechanism and wait for a design decision. Full evidence: `MOVEMENT_INVESTIGATION_REPORT.md`. The sections below are the history; several hypotheses in them were refuted (see the summary at the end).**
 — walking up 30 cm ledges (`SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps`)

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

---

# Update (Night Build 01 completion, 2026-10-30): all five open movement cases, fresh evidence

No code was changed for movement in this pass (as instructed: no speculative fixes; the stair-camera smoothing of A1 is untouched and its test `SpnGame.Movement.StairCamera` passes). This section collects what a separate, Opus-assisted investigation needs.

## The five open cases

| Case | Where | Fresh result (this session) | Expected |
|---|---|---|---|
| `SpnGame.Movement.Obstacles` | `Spn/Tests/SpnMovementMapAutomation.cpp:671` | **FAIL** (headless full run `logs/regression-complete-1.log`: 128 of 130 pass, these two fail) | a 30 cm thin ledge blocks a walking character |
| `SpnGame.Movement.StairsAndRamps` | same file `:739` | **FAIL** | a staircase with 30 cm risers blocks a walking character |
| `SpnQA.Jump.late_jump_ledge_45_jog` | `Spn/Tests/SpnQaMovementAutomation.cpp:378` | **FAIL** (rendered `run_gameplay_qa.ps1 -Suite Full`, report `Saved/GameplayQA/20261030-124911/report.md`): take-off 350 cm/s, lowest speed before landing 34 cm/s (10 %), apex 45 cm (spec 45), landed after 0.44 s, Z at landing 137 (ground 97), 0.6 s after landing 221 cm/s, "crossed no" | crossing the 45 cm ledge with a late jump must not take longer than 1 s |
| `SpnQA.Jump.late_jump_ledge_40_run` | `:379` | **FAIL**: take-off 550 cm/s, lowest speed 39 cm/s (7 %), apex 45 cm, landed after 0.46 s, Z 132, 0.6 s after landing 258 cm/s | same |
| `SpnQA.MovementDemo` / `ledge_80_blocks_even_with_a_jump` | `SpnQaMovementDemoAutomation.cpp:128` | **PASS in this session** (rendered full run, and in the headless run); it was intermittent earlier and has not reproduced now. Not "fixed": no change was made, so the intermittency is simply unobserved. | an 80 cm ledge blocks even with a jump |

## Reproduction

```
# the two SpnGame cases (headless, about 3 min)
UnrealEditor-Cmd.exe Unreal\SupernaturalProt\SupernaturalProt.uproject -ExecCmds="Automation RunTests SpnGame.Movement.Obstacles+SpnGame.Movement.StairsAndRamps; Quit" -unattended -NullRHI -NoSound
# the late-jump cases (they need the rendered session; the headless run reports the same two failures)
powershell.exe -ExecutionPolicy Bypass -File Tools
un_gameplay_qa.ps1 -Suite Jump -NoVideo
# the intermittent 80 cm case
powershell.exe -ExecutionPolicy Bypass -File Tools
un_gameplay_qa.ps1 -Suite MovementDemo -NoVideo      # repeat 10x and count
```

## Relevant code

* `Spn/SpnCharacterMovementComponent.{h,cpp}` (`ApplySettings`: `MaxStepHeight = 25`, `PerchRadiusThreshold`, `bUseFlatBaseForFloorChecks`; `ComputeSlideVector`), `Spn/SpnMovement.h` (the constants and the M2A4.1 history in its comments), the jump model in Core (`Core/include/spn/core/JumpModel.h`, `Movement.h`; Core was not touched).
* Engine reference (read-only, do NOT modify the installed engine): `CharacterMovementComponent.cpp`: `StepUp`, `CanStepUp`, `MoveAlongFloor`, `SlideAlongSurface`, `HandleImpact`, `PhysFalling` (landing on a ledge edge).

## What the late-jump numbers suggest (hypotheses, not facts)

1. **The late jump starts 10 cm before the wall** (the 10 in the table). With a 45 cm apex and a 45 cm ledge the capsule bottom reaches the ledge top only at the very end of the rise; the horizontal speed collapses to 7-10 % of the take-off speed against the face ("lowest speed 34 / 39 cm/s") and the capsule lands on the top edge after ~0.45 s, then needs more than 1 s to be "across". That is a *ledge-assist / mantle* feature gap, not a tuning bug (the test text says so: "known issue, M2A2 report section 5, open: needs a ledge assist / mantle"). A fix is a design decision (automatic mantle assist? shorter/longer ledge margin in the spec?), not a one-line change.
2. **The 30 cm walking cases and the late jump may share one mechanism:** the capsule's rounded bottom rolling over a ledge corner in many sub-centimetre slide lifts (`OPUS_ESCALATION` §"What this tells us"). A per-frame recorder (below) would show whether the late-jump landing "creeps" the last centimetres the same way.
3. **The intermittent 80 cm case** was observed in an earlier build only; with frame-time dependent sweeps a different sub-stepping (editor frame rate, `MaxSimulationTimeStep`, `MaxSimulationIterations`) could explain intermittency. It passed in both sessions today (headless 60 fps cap vs. rendered), which does not exclude it.

## Suggested diagnostic experiments (in this order)

1. `spn.MoveDiag 1` during the 30 cm case; plot Z, `Velocity.Z`, `CurrentFloor.FloorDist`, movement mode and the blocking hit's `ImpactNormal` per frame (`SpnMovementDiagnostics`). Look for the 0.5-2 cm-per-frame Z series.
2. Same recorder for `late_jump_ledge_45_jog`; compare the last 0.2 s before "landed".
3. Run the 30 cm case at fixed 30 / 60 / 120 fps (`-FPS`/`t.MaxFPS` with `p.FixedFrameRate`) to see whether the creep depends on the frame time (it would also explain the intermittent 80 cm case).
4. Override `SlideAlongSurface` for `MOVE_Walking` to drop the vertical slide component when the impact normal is not walkable (as listed above) and see whether the late-jump numbers change too.
5. Decide the specification question first if experiments 1-4 show the physics is "correct": `MaxStepHeight` 20 vs 25 cm, whether 30 cm is allowed to be walked, and whether a mantle assist is wanted (developer decisions; not taken here).

## Previous unsuccessful repair attempts (cumulative)

M2A4.1: perch radius 12/14/16, flat-base floor checks, slope-boosting guard. Night Build 01: strict `CanStepUp` override (refused but not the mechanism), `MoveAlongFloor` lift instrumentation (no lift above 2 cm). Completion pass: none (by instruction).

---

# Resolution summary (movement investigation, 2026-10-30)

| Case | Status | Verified root cause | Change |
|---|---|---|---|
| `SpnGame.Movement.Obstacles` (30 cm walked) | **RESOLVED** | Not "many small slide lifts" (refuted: the trace shows ONE +30 cm lift). `StepUp` refuses correctly; the post-move floor check (flat base = 45-degree box reaching the full radius, restarted 48.6 cm above the capsule bottom) lands on the ledge top, reports a walkable floor at -27.85 cm, the perch probe accepts it because it sees the old ground, and `AdjustFloorHeight` lifts the capsule 30 cm in one frame. | `FindFloor` override + `bLimitFloorRiseToStepHeight`: a floor that would lift the capsule above `MaxStepHeight` while walking is replaced by the perch-probe floor |
| `SpnGame.Movement.StairsAndRamps` (30 cm stairs) | **RESOLVED** | same mechanism | same change |
| `SpnQA.Jump.late_jump_ledge_45_jog` | **OPEN - design decision** | lower hemisphere hits the ledge corner 1-2 frames after take-off, horizontal speed collapses (engine falling slide + slope-boosting guard); outcome is a knife edge (apex 45.0 cm = ledge 45 cm) and frame-rate dependent. Not caused by the capsule radius (30/34 cm also blocked) or by the floor rule. Getting onto the ledge here = mantle / ledge assist | none (no automatic mantle without a rule) |
| `SpnQA.Jump.late_jump_ledge_40_run` | **OPEN - design decision** | same | none |
| `ledge_80_blocks_even_with_a_jump` (intermittent) | **RESOLVED (root cause)** | not the 80 cm wall (never climbed) and not mouse drift (yaw logged constant): the SPN ground steering plus the engine's per-move velocity reset made a character pressed straight into a wall accelerate sideways (30 cm/s nudge -> 133 cm/s, 285 cm in 3 s), slide to the end of the wall and walk round it | `CalcVelocity`: pressed into a wall, only the input component along the wall steers; regression test `SpnGame.Movement.WallPushNoSideDrift` (drift 285 -> 1.7 cm) |

Refuted hypotheses: "many small slide lifts" (case 1), "mouse yaw drift in the rendered run" (case 5), "the wide 42 cm capsule blocks the late jump" (cases 3/4).
