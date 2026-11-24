# Ledge-assist animations: inventory and integration plan (2026-11-14)

Developer decision of 2026-11-14 (item 1): hybrid approach.
- First check every existing animation and identify suitable starting motions for step-up, vault and mantle.
- Then write a concrete integration plan with retargeting, IK and motion warping.
- No purchases, downloads or final asset choice without approval.

Status: **inventory done, plan written, nothing integrated.** No asset was bought, downloaded, copied into the project or chosen.

## 1. How the inventory was made

`Unreal/SupernaturalProt/Tools/animation_inventory.py` is read-only and runs headless (`-run=pythonscript`). It writes
`Saved/SpnCharacter/animation_inventory.json`. For every AnimSequence it records:
- skeleton, length, frames and the root-motion flag;
- the root displacement from start to end;
- the pelvis height range;
- the highest point of each hand and foot relative to the pelvis at the start. A hand or foot raised far above the pelvis marks a hand
  plant or a step onto something.

Scope:
- the project content (`/Game/Characters`, the three template variants);
- the engine's local example plugin `MoverExamples`. It was mounted for this run only with `-EnablePlugins=...`; the project file is
  unchanged.

Result: 137 clips, 10 montages, 7 blend spaces, the mannequin control rigs, and an IK rig plus retargeter. 97 clips are in the project,
40 in `MoverExamples`.

## 2. What exists

**No clip in the project shows a step-up, vault, mantle or climb.** The useful material (measured):

| Clip | Where | Length | Root motion | Pelvis | What it offers |
|---|---|---|---|---|---|
| `VaultOver` (+ `VaultOver_Montage`) | engine plugin `MoverExamples` (experimental) | 2.03 s | yes, 390 cm forward | 63 → 216 cm (rises 150 cm) | **a complete vault**: run-up, hand plant (hands up to 186 cm above the starting pelvis), legs over (feet +114 cm), landing. The montage already uses **motion warping**: three `AnimNotifyState_MotionWarping` windows with the targets `VaultStartPos`, `VaultStepPos`, `VaultLandingPos`. |
| `MM_Jump` | project | 0.87 s | no | rises 46 cm | arms raised (hands +85 cm): the reach phase of a mantle or climb |
| `MM_Dash` / `MM_Dash_Forward` | project / plugin | 0.97 s | yes (885 cm) / no | rises 50 cm | a forward lunge with the body raised: the drive of a running vault in the middle band |
| `MM_Land` | project | 0.87 s | no | dips 27 cm | landing behind a vault, the end of a drop |
| `MM_Unarmed_Crouch_Exit` | plugin | 0.87 s | no | 42 → 94 cm | standing up from a crouch: the last phase of a mantle or climb (knees on the edge → standing) |
| `MM_Unarmed_Crouch_Entry` / `_Idle` | plugin | 0.73 s / loop | no | 94 → 42 cm | the crouched pose on top of a ledge |
| `MF_Unarmed_Jog_Fwd`, `MF_Unarmed_Walk_Fwd`, `MM_Run_Fwd` | project / plugin | cycles | yes (jog 1060 cm) | ±5 cm | the base of a step-on (one step with IK onto the edge) |
| `MM_Attack_03` | project | 1.67 s | yes | – | right foot raised 65 cm above the pelvis (a kick); reference for a high knee, not usable as is |
| Pose assets `Manny_thigh_*`, `Manny_calf_*`, `Manny_upperarm_*` | plugin | single poses | – | – | additive poses for raising one leg or arm: the leg lift of a step-over |
| `CR_Mannequin_FootIK`, `CR_Mannequin_Body`, `CR_Mannequin_Procedural` | project | control rigs | – | – | foot IK and full-body control: hand and foot placement on the edge |

Licence and suitability of `VaultOver`: it is local engine content, nothing is downloaded. The plugin's own description says it is
"not intended to be used directly in a shipping product". So it is suitable as a **prototype and reference**, not as a final asset.
Using it, even only for a prototype, needs your approval: enable the plugin or copy the clip into the project.

## 3. Starting motion per move

| Move (band) | Starting motion | Method | Quality to expect |
|---|---|---|---|
| StepOn (≤ 40 cm) | the jog / walk cycle, unchanged | IK only: the leading foot is placed on the edge (foot IK target = edge point from the probe); the pelvis follows the capsule path | good; no new clip needed |
| Low step-over (≤ 40 cm, thin) | the walk cycle + additive `Manny_thigh_*` / `Manny_calf_*` pose (one leg lifted) | additive pose for the swing leg, foot IK on the far side | acceptable for a prototype |
| Vault (40–120 cm, thin, run-up) | `VaultOver` (prototype) or `MM_Dash` + hand IK | montage with motion warping onto the three targets; time-scaled to the core plan's duration | `VaultOver`: good, but made for a higher obstacle (pelvis rises 150 cm), so it needs warping downwards. `MM_Dash`: rough. |
| Mantle (40–120 cm, onto it) | `MM_Jump` (reach, frames up to the apex) → hands on the edge (IK) → `MM_Unarmed_Crouch_Exit` (stand up on top) | montage of three sections, motion warping (`MantleGrabPos`, `MantleTopPos`), hand IK during the pull-up | prototype quality; the pull-up itself is missing as a motion |
| ClimbUp (120–180 cm) | as for Mantle, with a longer hang (`MM_Jump` apex pose held) | as for Mantle; hands IK-pinned to the edge while the capsule rises | weakest: there is no pull-up motion at all |

Conclusion: StepOn and the low step-over can be done well with existing material and IK. Vault has a usable prototype source
(`VaultOver`). Mantle and ClimbUp can only be assembled roughly from existing parts. A real pull-up (mantle / climb) and a lower vault
need **new clips**: own recordings, purchased or generated. That needs your approval, per the decision.

## 4. Integration plan

### 4.1 Who moves the body

Motion warping only bends root motion. So the plan has to say who moves the capsule. There are two consistent variants:

- **A: the core path moves the capsule (as today); in-place clips + IK.**
  - The montage's root motion is extracted (in-place) and the section is time-scaled to the plan's phases: lift until
    `LiftSeconds`, then over the top until `TotalSeconds`.
  - Hands and feet reach the edge through IK, with targets from the probe.
  - No motion warping needed. Tests, determinism and later co-op prediction stay as they are.
  - Good for StepOn and the low step-over; for a vault the mesh only follows the capsule's simplified curve.
- **C: the montage's root motion moves the capsule, warped onto the probe's targets** (Epic's pattern, as in `VaultOver_Montage`).
  - The core plan still decides whether a move may start and validates the corridor beforehand.
  - It supplies the warp targets (`StartPos` = capsule start, `GrabPos` / `StepPos` = edge point, `TopPos` = end pose, `LandingPos` =
    landing behind) and the expected end pose and duration.
  - The custom movement mode then applies the warped root motion instead of sampling the path, and checks it against the core path.
    If it deviates by more than a tolerance (e.g. 10 cm) or hits something, the move is released as today.
  - Best visual result for Vault / Mantle / Climb. Costs: co-op prediction of root motion (`FRootMotionSource`) and a second path to
    test.

Recommendation:
- **A** for StepOn and the low step-over (step A2 below, quick).
- **C** for Vault, Mantle and ClimbUp once real clips exist (steps A3–A5).
- `SpnQA.LedgeAssist` keeps its criteria (outcome, end position, speed, duration within its ranges) in both variants.
### 4.2 IK

- Feet: `CR_Mannequin_FootIK` (exists). For StepOn, the leading foot's target is the edge point (+ ball offset).
- Hands: two-bone IK (or `CR_Mannequin_Body`) on `hand_l` / `hand_r`, target = edge point ± shoulder width. Pinned while the
  `GrabWindow` curve of the montage is 1.
- Blend IK in and out with montage curves (`IK_Hand_L/R`, `IK_Foot_L/R`) so the clips stay usable without IK.

### 4.3 Retargeting (onto the later body)

- The clips stay on the UE mannequin skeleton (the contract, 0013 §9). Every new clip is authored or imported for it.
- A later body gets them through the existing chain: `RTG_SpnMannequin_Template` and its copy per body, run by
  `character_candidate_check.py` / batch retarget.
- Proven on 2026-11-14 for the local MetaHuman template body: the 20 player clips retarget with matching frame counts (0013 §11).
- Warp targets and IK goals are bone-name based (`hand_l`, `foot_l`, `pelvis`), so they survive the retarget.

### 4.4 Code steps (each keeps the QA green; none started)

| Step | Content | Needs approval |
|---|---|---|
| A1 | `USpnAnimInstance` gets `SpnLedgeMove`, `SpnLedgeProgress` (0..1), `SpnLedgeTargets` (edge / top / landing) from `USpnCharacterMovementComponent` | no |
| A2 | StepOn and low step-over: foot IK onto the edge in `ABP_Unarmed` (existing clips and rig) | no; it changes the `ABP_Unarmed` asset |
| A3 | enable the engine plugin `MotionWarping` (code only, local) and add a `UMotionWarpingComponent` to the character | yes (new engine plugin dependency) |
| A4 | prototype montages for Vault / Mantle / ClimbUp. Vault: copy `VaultOver` from `MoverExamples` into a prototype folder (outside git, like Fab). Mantle / Climb: assembled from `MM_Jump` + `Crouch_Exit` | **yes**: `VaultOver` is example content "not for shipping" |
| A5 | rendered QA: `SpnQA.LedgeAssist` additionally records the hand and foot distance to the edge at the grab frame (≤ 10 cm), plus a video per band | no |
| A6 | final clips: own recording, purchase or generation | **yes** (your decision) |

The ledge-assist QA (`SpnQA.LedgeAssist` 14/14, the jump tests) must stay unchanged throughout: the animation only follows.

## 5. Open points

- `VaultOver` was measured, not looked at: whether it reads well at 90 cm is unknown until A4.
- `MotionWarping` is a new engine plugin for the project (A3).
- No hand / pull-up motion exists in any local source.
