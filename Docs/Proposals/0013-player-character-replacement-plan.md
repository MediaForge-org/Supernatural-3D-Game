# 0013 — Replacing the placeholder player character: analysis and integration plan — ENGINEERING PROPOSAL

Status: written 2026-11-13 for developer review. **Not master canon.** Developer approval 2026-11-13: steps **S1–S3** only (section 8 records
what was implemented). S4–S8 and every decision in section 6 stay OPEN. The current player character (UE5 template mannequin) stays the
placeholder. No main character, sex, face, body or pipeline is chosen by this document.

## 1. Basis

| Source | Finding | Status |
|---|---|---|
| `Docs/Decisions/0005` §1 (master basis table A1–A11, read 2026-10-20) | photoreal humans as far as reasonable (A1); deep modular creator for player and NPCs (A2); scalable NPC fidelity (A3); one player-created protagonist, male and female fully viable (A4); third-party content kept separate (A9) | as classified there (tier 1–3) |
| `Docs/Proposals/0003` §4 | pipeline comparison (MetaHuman / own Blender pipeline / modular parts / commercial creators), licence notes (MetaHuman terms **unverified**), proposal of two spikes before deciding | PROPOSAL |
| `Docs/ProtagonistV1/README.md` | MetaHuman Character `MHC_ProtagonistV1` exists as an unapproved face preview; **no rig, no assembly, not playable**; developer reading of "V1 = default look, not a fixed hero" still OPEN | WIP |
| `Docs/Proposals/0009`, `0011` | jump by active input only, human jump strength, height-matched vault/mantle later; four gaits; animation state interface `GetMovementState()` | developer direction / proposal |
| `Planning/Master` | **not reachable on this Windows machine** (the archive lives on the Fedora laptop); no new master search was run for this document. Run `bash Tools/master-search.sh character creator metahuman skeleton clothing animation` on Fedora before any decision. | gap |

## 2. Current architecture (read from code and content, 2026-11-13)

| Area | What exists | Notes for a model change |
|---|---|---|
| Pawn | `ASupernaturalProtCharacter : ACharacter` (C++), blueprint `BP_ThirdPersonCharacter` sets mesh and anim blueprint | the C++ class holds no content references; the visual body is chosen in the blueprint |
| Capsule | radius 42 / half height 96 in C++ (the blueprint runs with radius 35, measured by SpnWorldQA) | gameplay, doors, step height and all QA tests depend on it: **must not change with the body** |
| Movement | `USpnCharacterMovementComponent`: gaits Walk/Jog/Run/Sprint, human jump 45 cm, step 25 cm, network-ready; animation interface `GetMovementState()`, `GetObservedGait()`, `GetEffectiveGait()`, `GetSecondsSinceLanding()` | independent of the mesh |
| Dodge, vault, mantle, crouch | **do not exist** (0009 direction only) | animation sets for them are part of a later traversal milestone |
| Controller | `ASupernaturalProtPlayerController`, Enhanced Input (move, look, jump, sprint, gait cycle V, interact E, force F) | independent of the mesh |
| Interaction | `USpnInteractionComponent`: trace from the camera, 250 cm | independent of the mesh |
| Camera | spring arm 400 cm, stair smoothing, hides the mesh when the camera is very close | arm height/offset depend on body height; keep as data |
| Mesh / skeleton | template `SKM_Manny_Simple` / `SKM_Quinn_Simple`, skeleton `SK_Mannequin`, control rigs `CR_Mannequin_*`, physics asset `PA_Mannequin` | the UE5 mannequin hierarchy is the de-facto standard for retargeting |
| Animation | template `ABP_Unarmed` (blueprint), blendspace `BS_Idle_Walk_Run`, Unarmed/Pistol/Rifle/Death sets; no run/sprint clips (0011 §4.12) | the anim blueprint reads the template variables, not yet the SPN movement-state interface |
| NPC | `ASpnNpc : ACharacter` with a **static cylinder + sphere** as body | needs the same visual path as the player later |
| Appearance data | Core `AppearanceSpec` (0005): body provider as a content id, clothing per slot, layers, parameters, detail tiers, recipes for NPCs | pipeline-agnostic; nothing in Unreal reads it yet |
| MetaHuman | plugin `MetaHumanCharacter` enabled, `MHC_ProtagonistV1` (face preview only) | not assembled, not rigged |

## 3. Options compared

Statements about third-party tools are general knowledge or come from 0003; anything marked *verify* must be checked in a spike
before a decision. No licence text was read for this document.

| Criterion | MetaHuman | Own standardised base body (e.g. Blender, parametric base mesh, own sculpt) | Licensed base models (Fab / marketplace / commercial creators) |
|---|---|---|---|
| Visual quality (faces, skin, hair) | highest for the effort; strand hair and facial rig included | depends on art time; faces are the expensive part | varies per product; often good bodies, weaker faces |
| Performance | heavy at full detail (strand grooms, high-poly faces, many joints); needs LOD and groom fallbacks for NPC crowds — *measure on the RTX 3090* | fully controllable (polycount, LODs, materials) | varies; often game-ready LODs |
| Rigging / skeleton | body skeleton close to the UE5 mannequin hierarchy plus extra joints — *verify* retargeting of the existing anim sets | own choice; recommended: UE5 mannequin bone names so the existing sets and IK retargeters apply | varies; retarget via IK Retargeter |
| Clothing | limited preset garments; own garments need fitting to the MH body | full control; clothing modelled against the standard body, swappable per slot | depends on the kit; often fitted to its own body only |
| Animations | existing UE5 mannequin sets retargetable — *verify*; facial animation strong | existing sets via shared skeleton | via retargeting |
| Extensibility (creator, many NPCs, mods) | runtime customisation limited to what the rig/groom/material parameters allow; mods would need MH assets | best fit for a deep creator and for mods (morphs, own parts, publishable) | licence usually forbids redistribution → mods and open source restricted |
| Licence / open source | MetaHuman assets must not go into a public repo (assumption in 0003, *verify*); usable in a shipped game | own work: publishable | not publishable as source; manifest + local fetch only |

Reading of the requirements (A1–A4, A9): one pipeline alone fits poorly. A **hybrid** is plausible (for example MetaHuman or high-end
heads for main characters, a standardised modular body for the player creator and NPCs), but that is a **developer decision**.

## 4. Proposed integration architecture (independent of the pipeline decision)

1. **Separate the visual body from the gameplay pawn.** A component `USpnCharacterVisualComponent` (on the player character and on
   `ASpnNpc`) builds the visual from data: a body skeletal mesh as the leader pose plus follower skeletal meshes for head, hair and
   one per clothing slot (`SetLeaderPoseComponent`), materials and morph parameters. Input is an `AppearanceSpec` (0005) resolved
   through a provider table (content id → assets). The capsule, movement and interaction never read the visual.
2. **One C++ anim instance base** (`USpnAnimInstance`) that reads `GetMovementState()`, gaits, speed, landing time and later traversal
   states from `USpnCharacterMovementComponent`. Every body's anim blueprint derives from it, so a new body cannot break movement logic.
3. **One standard skeleton contract:** UE5 mannequin bone names and proportions as the reference; anything else is retargeted with an
   IK Retargeter asset per provider. Sockets for interaction and later props (hand_r/l, head) are part of the contract.
4. **Body proportions do not change gameplay:** capsule, step height, jump height and interaction reach stay fixed per character
   class. Camera arm offset and mesh offset come from the body data (height parameter) so the camera does not clip into tall/short bodies.
5. **Detail tiers:** the visual component switches representation by distance using `DetailTierTable` (0005): full (hero) → reduced
   (no strand hair, fewer follower meshes, merged clothing) → crowd.
6. **Third-party content stays outside git** (as with Fab today): providers point to local content; a missing provider falls back to
   the mannequin and is reported (same rule as `fab.py` in the world generator).

## 5. Plan in steps (each step keeps the current QA green)

| Step | Content | Needs a decision / approval | Acceptance |
|---|---|---|---|
| S1 | `USpnAnimInstance` + reparent `ABP_Unarmed` to it; no visual change | no | SpnQA Full + Movement + World unchanged; anim state matches `GetMovementState()` in a new automation test |
| S2 | `USpnCharacterVisualComponent` with the mannequin as the only provider; clothing slot demo with a second mannequin mesh as follower | no | same tests; leader-pose followers stay attached during all gaits, jump, stairs (screenshots) |
| S3 | camera/mesh offsets from body height; test bodies at 160 / 195 cm (scaled mannequin) | no | no camera clipping in SpnWorldQA and stairs camera test; capsule unchanged |
| S4 | **Spike A — MetaHuman:** assemble `MHC_ProtagonistV1` (rig, textures), retarget the mannequin sets, measure GPU ms and memory per character at 1440p and per detail tier | **yes:** Epic account login for the cloud rig, visual approval of V1, licence check | movement QA with the MH body; numbers in a report |
| S5 | **Spike B — standardised own body** (Fedora/Blender): base body on the mannequin skeleton, one garment per slot, morph ranges | yes: tool and base-mesh licence approval | same QA; numbers; clothing workflow hours |
| S6 | developer decision: pipeline per character class (player creator, story characters, NPCs) | **yes** | written in `Docs/Decisions` with the master basis |
| S7 | NPCs on the visual component; several humans with recipes from `AppearanceSpec`; separate clothing per slot | after S6 | NPC QA + performance budget per tier |
| S8 | traversal and dodge animation sets (0009 milestone) | yes (traversal milestone) | traversal QA |

## 6. Open decisions for the developer

1. Reading of Protagonist V1: default look of a player-created protagonist, or a fixed hero? (OPEN in the V1 README)
2. Pipeline per character class (S6), after the two spikes.
3. Licence review of MetaHuman and of any base mesh or marketplace kit (the licence texts were not read for this document).
4. Performance budget per detail tier (number of hero characters on screen, NPC count at 1440p / 60 fps on the RTX 3090).
5. Whether facial animation (dialogue) is needed for the vertical slice.

## 7. Risks

- Strand hair and high-detail faces cost much GPU time; the forest section alone already uses most of the 60 fps budget at 1440p
  (`Artifacts/VisualProgress/PERF_REPORT_2026-11-13.md`).
- A body with other proportions can change foot placement on stairs and the camera on narrow interiors; S3 tests this before any real body.
- Retargeted animations may slide (foot IK); the existing control rig `CR_Mannequin_FootIK` is the starting point.

## 8. Implemented S1–S3 (2026-11-13, approved by the developer)

Approval terms: no final character; movement and interaction must not be damaged; prepare an exchangeable presentation; check skeleton,
animation and retargeting dependencies; mark decisions missing from the planning archives as open.

| Step | Implementation | Files |
|---|---|---|
| S1 | `USpnAnimInstance` (C++): read-only `SpnMovementState`, `SpnObservedGait`, `SpnGroundSpeed`, `SpnVerticalSpeed`, `bSpnInAir`, `SpnSecondsSinceLanding`, `bSpnSteppingAside` from `USpnCharacterMovementComponent`. `ABP_Unarmed` reparented to it by `Tools/reparent_abp_unarmed.py` (headless, compile clean); its graphs are unchanged and still read the template variables, so the animation looks as before | `Spn/Character/SpnAnimInstance.*`, `Content/.../ABP_Unarmed.uasset` (the only changed content asset; undo: `git restore` it) |
| S2 | `USpnCharacterVisualComponent` on the player character: presets (`FSpnBodyPreset`: body mesh, anim class, follower meshes, height) as soft references, leader-pose followers without collision, missing assets fall back to the blueprint's mannequin and are logged. No active preset = the blueprint's presentation exactly as before. Console `Spn.Body <id>|none`, command line `-SpnBody=<id>` | `Spn/Character/SpnCharacterVisualComponent.*`, `SupernaturalProtCharacter.*` (component, `GetCameraBase`/`SetCameraBase`) |
| S3 | body height scales the mesh around its origin (feet on the capsule bottom) and moves the camera pivot by `(height − reference) × 0.9` (PLACEHOLDER) through the character's stair-camera base. Capsule, step, jump, speeds, interaction reach unchanged. Test presets `test_160` / `test_195` = scaled mannequin, no new content | same |

Tests (all run 2026-11-13):

| Test | Result |
|---|---|
| `SpnGame.Character.AnimBaseReadsTheMovementState` (ABP_Unarmed derives from the base; anim state = `GetMovementState()` for standing, walking, jogging, sprinting, jumping, falling) | PASS |
| `SpnGame.Character.BodyHeightKeepsGameplay` (160/195: height, mesh scale, feet, camera pivot; capsule, step, jump, walk speed unchanged; `none` restores; unknown id refused) | PASS |
| `SpnGame.Character.FollowersAndMissingAssets` (follower demo with `SKM_Quinn_Simple`: leader pose, attachment, no collision; a preset with three missing assets falls back and reports 3) | PASS |
| `run_gameplay_qa.ps1 -Suite Doors` (incl. Buildings: stairs, interiors) with `-SpnBody=test_160` / `test_195` | 89/0 each |
| `run_gameplay_qa.ps1 -Suite World` (camera/feet clipping at street, slope, interior) with `-SpnBody=test_160` / `test_195` | 27/27 each |

Measured: the template body `SKM_Quinn_Simple` is 180.2 cm high (imported bounds); the camera boom rests at the capsule centre (z 0).

### Skeleton, animation and retargeting dependencies (asset registry, read-only, `Tools/character_dependency_report.py` → `Saved/SpnCharacter/dependency_report.json`)

| Item | Finding | Consequence |
|---|---|---|
| Skeleton | one human skeleton: `SK_Mannequin`; on it 2 meshes (`SKM_Manny_Simple`, `SKM_Quinn_Simple`), 97 anim sequences, 3 blend spaces, 4 montages, 4 anim blueprints (`ABP_Unarmed` + 3 template variants) | every current animation needs this skeleton or a retarget |
| Player chain | `BP_ThirdPersonCharacter` → `SKM_Quinn_Simple` + `ABP_Unarmed` → `BS_Idle_Walk_Run`, `MM_Idle`, `MM_Jump`, `MM_Fall_Loop`, `MM_Land`, `CR_Mannequin_FootIK` | a new body must provide these (or replacements) through its anim blueprint |
| Rigs | control rigs `CR_Mannequin_Body`, `CR_Mannequin_FootIK`, `CR_Mannequin_Procedural`; physics asset `PA_Mannequin` | foot IK and procedural parts are mannequin-specific |
| Retargeting | **no IK Rig and no IK Retargeter asset exists** | any body on another skeleton (MetaHuman, own body, marketplace) needs IK Rigs for both skeletons and a retargeter (S4/S5 work) |
| MetaHuman | `MHC_ProtagonistV1` only (face preview, no assembled skeletal mesh) | nothing to retarget yet (S4) |
| Other skeletal meshes | Megaplants tree parts (own skeletons, foliage) | not characters; irrelevant here |
| Template variants | `Variant_Combat/Platforming/SideScrolling` characters also reference the mannequin meshes | untouched; not used by SPN levels |

### Open (not decided by S1–S3; the planning archive was not reachable on this machine — run the master search on Fedora)

1. Everything in section 6 (V1 reading, pipeline per character class, licences, performance budget per tier, facial animation).
2. Reference skeleton contract: the UE5 mannequin is the de-facto standard here, but it is **not** a recorded decision.
3. Whether the anim blueprint of a future body should read only the `Spn*` variables (proposed) — `ABP_Unarmed` still uses the template's.
4. `CameraHeightFactor` 0.9 and the 160/195 cm test range are placeholders, not body-range decisions.
5. NPC bodies (`ASpnNpc`) on the same component: S7, after S6.

## 9. Retarget preparation for S4/S5 (2026-11-13, developer order: prepare technically, no final character, S4/S5 not implemented)

Everything here is tooling around the existing template mannequin; no body is imported, chosen or downloaded. The created assets live only in
`/Game/SpnCharacterPrep` (gitignored like every `.uasset`; the scripts are the source and recreate them).

| Tool (`Unreal/SupernaturalProt/Tools/`) | What it does | Writes |
|---|---|---|
| `character_skeleton_contract.py` | read-only: the reference skeleton contract from `SK_Mannequin` (161 bones, 180.2 cm, A-pose with the upper arm 54.8° below horizontal, Z up, faces +Y, sockets `weapon_r_muzzle`, `foot_l/r_Socket`, `HandGrip_L/R`, proportions leg 0.53 / arm 0.34 / pelvis 0.54 of the height); checks any candidate mesh (`SPN_CANDIDATES=path,path`): required humanoid bones, recommended IK helper bones, Z-up, pose type, proportions → verdict DIRECT / RETARGET / RETARGET + BONE MAPPING | `Saved/SpnCharacter/skeleton_contract.json`, `skeleton_check.json` |
| `character_retarget_prep.py` | creates `IK_SpnMannequin` (engine auto retarget definition: 29 chains incl. fingers, retarget root `pelvis`; auto full-body IK with goals LeftFootIK/RightFootIK/LeftHandIK/RightHandIK) and `RTG_SpnMannequin_Template` (source rig = mannequin, default op stack Pelvis Motion → FK Chains → Run IK Rig → Root Motion → Remap Curves, exact chain mapping); dry run of the batch path | `Saved/SpnCharacter/retarget_prep.json`, assets in `/Game/SpnCharacterPrep` |
| `character_dependency_report.py` | read-only asset-registry report (section 8) | `Saved/SpnCharacter/dependency_report.json` |
| `character_api_probe.py` | read-only: the IK Rig / retarget Python API of UE 5.8 (`run_batch_retarget` with `IKRetargetBatchOperationInputs`; `duplicate_and_retarget` is deprecated) | `Saved/SpnCharacter/ikrig_api.json` |

Verified 2026-11-13:
- Contract checker: SKM_Quinn_Simple / SKM_Manny_Simple → DIRECT; a non-human skeletal mesh (Megaplants beech) → RETARGET + BONE MAPPING with
  the missing bones listed; a missing asset → ERROR.
- Batch retarget dry run (Quinn → Manny, `MM_Idle`, `MF_Unarmed_Walk_Fwd`, `MF_Unarmed_Jog_Fwd`) into `/Game/SpnCharacterPrep/DryRun`:
  3 clips, frames and length identical to the sources (227 / 45 / 53 frames). This proves the tool chain on this engine version; it says
  nothing about the quality of a retarget to a different body.

What a spike (S4 MetaHuman / S5 own body) would do next, once approved — not started:
1. Import or assemble the candidate body (S4 needs the Epic account login and the licence check; S5 the tool/base-mesh approval).
2. Run `character_skeleton_contract.py` with `SPN_CANDIDATES=<mesh>` → verdict and pose difference (a T-pose body needs a retarget pose).
3. Create `IK_<Body>` for the target (auto definition if the engine recognises the skeleton, else chains by hand matching the contract's chain
   names), set it as TARGET of a copy of `RTG_SpnMannequin_Template`, align the retarget pose, map chains.
4. Batch-retarget the player chain's clips (`BS_Idle_Walk_Run` inputs, `MM_Idle`, `MM_Jump`, `MM_Fall_Loop`, `MM_Land`, walk/jog sets) into the
   spike's folder; an anim blueprint derived from `USpnAnimInstance`; a preset in `USpnCharacterVisualComponent`.
5. Movement QA with `-SpnBody=<preset>`; foot sliding, foot IK (`CR_Mannequin_FootIK` is mannequin-specific), GPU ms and memory per tier.

Open (unchanged, not decided by this preparation): pipeline per character class, the reference skeleton contract itself (the UE5 mannequin is
proposed, not decided), the reading of Protagonist V1, licences, performance budgets, facial animation.

## 10. One reproducible acceptance check for a later body (night build 2026-11-14; preparation only, no body chosen, nothing downloaded)

`Unreal/SupernaturalProt/Tools/character_candidate_check.py` (headless, `SPN_BODY=<mesh>`, `SPN_FOLLOWERS=<mesh,...>`). It writes
`Saved/SpnCharacter/candidate_check.json` and creates assets only in `/Game/SpnCharacterPrep/Candidate`. Steps:

1. Skeleton contract (section 9).
2. Followers. Clothing, head and hair slots are leader-pose followers of `USpnCharacterVisualComponent`; every follower bone must exist in the body.
3. Animation base. The clips the player's anim blueprint really uses (asset registry, through the blend spaces) are batch-retargeted to the
   body with `RTG_SpnMannequin_Template`, and the frame counts are compared.
4. Proportions and camera: height, head height, leg/arm ratios, and the camera pivot shift the visual component applies. The capsule stays
   unchanged.

Verdict: READY / NEEDS RETARGET RIG / BLOCKED.

Run 2026-11-14 with the stand-ins `SKM_Manny_Simple` (body) and `SKM_Quinn_Simple` (dummy clothing slot): **READY**.
- Contract: DIRECT.
- Follower: 161 bones, none missing.
- **20 clips** of `ABP_Unarmed` retargeted, frame counts identical.
- Height 180.5 cm, head 161.8 cm, leg 0.529 / arm 0.34 of the height, camera shift 0.3 cm.

This proves the chain end to end on this engine version. It says nothing about the quality for a body with other proportions.

Known noise: describing a "Simple" mesh logs `GetBoneParent: Bone ... not found` errors for corrective bones the shared skeleton has and
the mesh does not. The commandlet then exits 1, but the report is complete. The parent class of the anim blueprint is not readable from
Python in the commandlet (`null` in the report); `ABP_Unarmed` was reparented to `USpnAnimInstance` in S1 (section 8).

Direction recorded from the developer's night-build order (2026-11-14), not a decision on the pipeline:
- humans: standardised base bodies with exchangeable clothing (the follower slots above);
- monsters: later a hybrid skeleton/rig architecture.

Neither is implemented beyond this check. S4/S5 are not started.

## 11. 2026-11-14 (afternoon): the local MetaHuman template body

`character_candidate_check.py` has a new option `SPN_TARGET_IKRIG`. For a body on its own skeleton it makes a copy of
`RTG_SpnMannequin_Template` in the candidate folder, with that IK rig as target (chains mapped by name, bones auto-aligned).

Run on the MetaHuman template body that ships with the enabled `MetaHumanCharacter` plugin (`SKM_Body`, face `SKM_Face`, rig
`IK_MH_IKRig`), with nothing downloaded or chosen:
- the 20 player clips retarget with identical frame counts;
- the head fails the follower rule (875 bones, facial bones missing in the body): a head slot needs its own rule;
- the height (131 cm) is not reliable, because the body has no head geometry.

The proposal for the first real character and its steps are in `Docs/Proposals/0014-first-player-character-proposal.md`.
