# Locomotion: measured cause of the remaining foot sliding, and the exact clips that are missing (NightBuild06, Phase D)

Status: analysis and requirements for the developer. **No clip was bought, downloaded or generated, and no play-rate trick was added.**
The movement speeds are gameplay placeholders (Core `MovementConfig`): walk 180, jog 350, run 550, sprint 800 cm/s. They were not changed.

## 1. What the project has (measured, `Saved/SpnCharacter/animation_inventory.json`, root travel / clip length)

| Clip family (UE5 template, unarmed) | Directions | Authored speed | Root motion |
|---|---|---|---|
| `MF_Unarmed_Walk_*` | 8 (Fwd, Bwd, Left, Right, 4 diagonals) | **300 cm/s** | yes |
| `MF_Unarmed_Jog_*` | 8 | **600 cm/s** | yes |
| `MM_Unarmed_Crouch_Walk_*` | 4 | 300 cm/s | yes |
| `MM_Idle`, `MM_Unarmed_Crouch_Idle` | – | 0 | no |
| `MM_Jump`, `MM_Fall_Loop`, `MM_Land` | in place | 0 | no |
| `MM_Unarmed_Crouch_Entry / Exit` | in place | 0 | no |
| `MM_Dash` (combat variant) | Fwd | 916 cm/s | yes |

`BS_Idle_Walk_Run` samples 0 / 300 / 600 cm/s. `ABP_Unarmed` (parent `USpnAnimInstance`) feeds it its own template variable `GroundSpeed`.
It still reads the template variables, not the `Spn*` ones (0013 S1).

## 2. Measured cause per gait

Median horizontal speed of a planted foot in cm/s, from `SpnQA.CharacterBody`:
- before: 0017 (run `20261115-002651`);
- after: this night's run, `README.md` §Measurements.

| Gameplay speed | What the blend plays | Sliding (placeholder) | Cause |
|---|---|---|---|
| walk 180 | idle ↔ walk(300) blend at 60 % | 3.8 | fine: the walk clip slowed by the blend |
| jog 350 | walk(300) 83 % + jog(600) 17 % | 31 | **two cadences blended**: the walk foot plants, the jog foot moves |
| run 550 | walk 17 % + jog 83 % | 50 | same, the other way round |
| sprint 800 | jog(600) at play rate × 1.33 (`USpnAnimInstance`, 0017) | 49–52 | cadence fixed; the stride of a jog is too short for 800 |
| start / stop | no start or stop clip | 10 | the blend ramps; there are no plant-and-push clips |
| 90° turn jog | no pivot clip | 31 | the jog cycle is rotated with the capsule |

Why C++ alone cannot fix jog / run: the blend-space input is computed inside the anim blueprint graph from the template variables, so the
C++ base cannot feed it a speed-matched value. Scaling the play rate does not help either: a blend of two clips with different cadences
has no single correct rate.

Rejected alternatives (they would hide the gap, not close it):
- changing the gameplay speeds to 300 / 600: a gameplay change, not allowed without a decision;
- foot locking (pinning the feet): makes the legs stretch visibly at 350 / 550;
- another blend-space edit from Python: on 2026-11-15 it broke the weighting (sliding 3.8 → 61 cm/s) and was reverted.

## 3. Options (developer decision)

| Option | What | Needs | Effect on sliding (expected, not measured) |
|---|---|---|---|
| **A. Clips at the gameplay speeds** | the clips in section 4 | licensed or own clips (motion capture / marketplace), approval | removes the cause; the best quality |
| B. Stride warping in the anim graph | engine `AnimationWarping` plugin node "Stride Warping" on the jog / run clips (no new asset) | change of `ABP_Unarmed` or a new C++-driven anim blueprint; the plugin is part of the engine (no download) | jog / run close to walk quality at moderate speed ranges (about ±30 % of the authored speed); visible limits at 800 |
| C. Both | B now, A later | – | – |

Recommendation: **B as an interim step**, because it needs no asset and no purchase, then A for the final quality. B changes an anim graph
asset (LFS) and the visual result has to be judged by eye. Not started tonight.

## 4. Exact clip list (requirement for option A)

Skeleton: UE5 mannequin (`SK_Mannequin`), or any skeleton with an IK rig the existing retargeters can map. Root motion with a root bone
that carries the travel, or in place with a `Distance` curve. Foot sync markers `l_foot_plant` / `r_foot_plant`. 30 fps or more. Loops
must close (first frame = last frame + one stride).

| # | Clip | Speed (cm/s) | Directions | Notes |
|---|---|---|---|---|
| 1 | Walk loop | 180 | Fwd, Bwd, Left, Right, 4 diagonals | or keep the 300 set and add 180 for the blend |
| 2 | Jog loop | 350 | 8 | the main gap |
| 3 | Run loop | 550 | Fwd, Fwd-L, Fwd-R (+ Bwd optional) | |
| 4 | Sprint loop | 800 | Fwd | upper-body lean |
| 5 | Starts | walk / jog / run | 0°, 90° L/R, 180° L/R from idle | plant and push |
| 6 | Stops | walk / jog / run | Fwd, left- and right-foot variants | |
| 7 | Pivots (direction change) | jog / run | 180° L/R, 90° L/R | while moving |
| 8 | Turn in place | 0 | 90° and 180° L/R | |
| 9 | Jump | standing, jog, run | Start, Apex, Fall loop, Land (light / heavy), Land into jog / run | the in-place `MM_Jump` / `MM_Land` exist, the moving ones do not |
| 10 | Stairs | walk, jog | up and down, step height 15–22 cm | or foot IK (`CR_Mannequin_FootIK`) on the flat cycles |
| 11 | Crouch walk | MaxWalkSpeedCrouched | 8 | 4 directions at 300 exist |
| 12 | Ledge moves | per the bands of 2026-11-14 | Fwd | StepOn 25–40 cm, step-over (low, thin), Vault 40–120 cm thin, Mantle 40–120 cm, ClimbUp 120–180 cm; motion-warping targets like the engine sample `VaultOver` (`Ledge-Animation-Plan-2026-11-14.md`) |
| 13 | Door / interaction | 0 | reach and push, pull, step aside | |

Licence for every clip: a commercial game licence that allows a later open-source release of the code (the clips themselves stay out of a
public repository unless their licence allows it; CLAUDE.md §9).

## 5. What was measured tonight (additional scenarios, same statistic)

`SpnQA.CharacterBody` got five more scenarios: walk → jog, run → sprint, braking out of a run, jogging backwards (S) and jogging sideways
(D). The character turns to the stick direction, so S / D measure the turn plus jogging in the new direction. They run on the placeholder,
`mh_male_193` and `mh_male_193_fit`. The values are in `README.md` §Measurements.
