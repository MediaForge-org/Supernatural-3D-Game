# 0014 — Proposal: the first real player character (2026-11-14) — ENGINEERING PROPOSAL

Status: for developer review. **Not master canon, no model chosen.** Developer order of 2026-11-14 (item 5):
- examine the next safe steps of the existing character pipeline;
- prepare the integration of a high-quality human base model, without fixing a final model;
- existing movement, interaction and animations must be kept;
- write a clear proposal for the first real character.

Basis: Proposal 0013 (S1–S3 implemented, sections 9–11). The developer direction recorded there: humans as standardised base bodies with
exchangeable clothing; monsters later on a hybrid rig. `Planning/Master` is not reachable on this machine; no master search ran. The
open questions in 0013 §6 stay open.

## 1. What is locally available today (no download, no licence acceptance, no purchase)

| Source | What | Status |
|---|---|---|
| UE5 mannequin (`SKM_Manny_Simple`, `SKM_Quinn_Simple`) | the current stand-in body, reference skeleton contract | in use; READY in the acceptance check (0013 §10) |
| **MetaHuman template body** in the enabled engine plugin `MetaHumanCharacter` | `/MetaHumanCharacter/Body/IdentityTemplate/SKM_Body` (body without head), `/MetaHumanCharacter/Face/SKM_Face` (head, 875 bones), skeleton `metahuman_base_skel`, `IK_MH_IKRig`, `RTG_MH_IKRig`, body LOD settings | engine content, local; **tested today, see section 2** |
| `MHC_ProtagonistV1` | the MetaHuman Character asset of the protagonist study (face preview); no assembled skeletal mesh | assembling the body / face / textures is 0013 step S4 and needs your approval (Epic account, cloud services, licence check) |
| Retarget tooling | `IK_SpnMannequin`, `RTG_SpnMannequin_Template`, `character_candidate_check.py`, `character_skeleton_contract.py` | in place (0013 §9–10) |

## 2. Safe step done today: the MetaHuman template body against the acceptance check

`character_candidate_check.py` was run on the template body:
- inputs: `SPN_BODY=/MetaHumanCharacter/Body/IdentityTemplate/SKM_Body`, `SPN_FOLLOWERS=/MetaHumanCharacter/Face/SKM_Face`;
- new option `SPN_TARGET_IKRIG=/MetaHumanCharacter/Animation/Retargeting/IK_MH_IKRig`;
- output only in the gitignored `/Game/SpnCharacterPrep/Candidate`; nothing in the game uses it.

| Check | Result |
|---|---|
| Skeleton contract | RETARGET: own skeleton, all required humanoid bones present. The mannequin IK helper bones (`ik_foot_*`, `ik_hand_*`) are missing; they are not needed with IK Rig based retargeting. Proportions leg 0.54 / arm 0.33 of the height; contract 0.53 / 0.34. |
| Animation base | **All 20 clips of the player's anim blueprint retargeted** (walk and jog in 8 directions each, idle, jump, fall, land). The frame count is identical for every clip. Verdict: "RETARGETS (visual review needed)". |
| Head as a follower | The face mesh has its own skeleton with 875 bones, most of them facial. The current follower rule ("every follower bone must exist in the body") fails, as expected for MetaHuman heads. A head slot needs its own rule: leader pose for the shared neck and head bones plus its own face anim blueprint. |
| Height | 131 cm measured; **not reliable**, because the body mesh has no head geometry. The real height needs the assembled character. |

Not done (deliberately): no visual review of the retargeted clips, no performance measurement, no change to the player or a level.

## 3. Proposal for the first real character

**MetaHuman protagonist on the existing gameplay character, animations by runtime retargeting from the mannequin.**

| Part | Proposal | Why |
|---|---|---|
| Body and face | `MHC_ProtagonistV1` assembled as a MetaHuman (body type from the parametric MetaHuman body), one LOD set for the player | highest human quality for the effort; the study already exists; the template proves the skeleton and retarget path locally |
| Gameplay | unchanged: `ASupernaturalProtCharacter`, capsule 42 / 96 cm, step, jump, ledge assist, interaction | 0013 §4 rule 4: the body never changes gameplay |
| Presentation | a preset in `USpnCharacterVisualComponent` (exists): body = MH body, followers = MH face (head slot rule from section 2), clothing slots as further followers on the MH body skeleton | exchangeable; a missing asset falls back to the mannequin (exists) |
| Animation | keep `ABP_Unarmed` on the hidden mannequin and drive the MH body from it with **runtime retargeting** (`Retarget Pose From Mesh` with a copy of `RTG_SpnMannequin_Template` → MH). Bake (batch retarget) later only if performance needs it. | every existing animation, the ledge-assist plan (`Ledge-Animation-Plan-2026-11-14.md`) and the foot IK stay valid; nothing has to be re-authored |
| Clothing | standardised garments fitted to the MH body (follower meshes); first test with the MetaHuman default garments | matches the direction "standardised base bodies + exchangeable clothing" |
| NPCs | not in this step (0013 S7). The later decision per character class stays open (0013 S6). | |

Alternative, if MetaHuman is ruled out (licence, open source, performance): an own standardised base body on the mannequin skeleton (0013 S5).
It is publishable and mod-friendly, but faces and skin cost a lot of art time.

## 4. Steps to it (each keeps the QA green; approvals marked)

| Step | Content | Approval |
|---|---|---|
| C1 | visual review of the 20 retargeted clips on the template body (editor; screenshots) | no |
| C2 | runtime retarget test with the **template** body: anim blueprint `ABP_SpnMetaHumanFollow` (`Retarget Pose From Mesh`), preset `mh_template`, then `SpnQA` Movement / Ledge / World with `-SpnBody=mh_template`; GPU ms and memory at 1440p | no (engine content, local; the preset stays off by default) |
| C3 | assemble `MHC_ProtagonistV1` (body, face, textures) | **yes**: Epic account / cloud services, licence check, visual approval of V1 |
| C4 | preset `protagonist_v1`, the head-slot rule in the acceptance check, camera height from the real body height | no (after C3) |
| C5 | performance budget for the hero character per detail tier (strand hair vs card hair) | **yes** (budget decision, 0013 §6.4) |
| C6 | first garment set as followers | asset source per garment needs approval |

The next safe step without any approval is **C1 + C2**: the existing movement / interaction / animation set on a real high-quality human
body, with numbers, before anything is chosen.

## 5. Risks

- MetaHuman licence terms and the open-source goal (CLAUDE.md §9): MetaHuman assets must not go into a public repository. Same rule as
  for Fab content: local content, outside git. The licence text has not been read for this proposal.
- Performance: the forest section already uses most of the 60 fps budget at 1440p. A hero MetaHuman with strand hair may cost several
  milliseconds. C2 measures the template, C5 sets the budget.
- Runtime retargeting costs CPU per frame. Bake if needed.
- Proportions: a body that is much shorter or taller changes the foot contact on stairs and the hand position at the ledge. The capsule
  stays; the camera pivot follows the height (0013 S3).
