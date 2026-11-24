# 0016 — Protagonist V1: feasibility phase report (2026-11-14/15) — ENGINEERING REPORT

Status: for developer review. **No final model, no final character editor, no final clothing, nothing bought, no cloud, no Epic login.**
Order of 2026-11-14 ("Protagonist V1: Machbarkeitsphase"), after Proposal 0015 was accepted. Decisions taken by the developer:
- V1 provisionally 193 cm, 32 years;
- the key art is the master for the visual identity;
- the face sheet is for geometry;
- the full-body sheet is for proportions and clothing;
- Blender may be installed for free from the official source.

`Planning/Master` is not reachable on this machine; no master search ran. The order is engineering feasibility, not a gameplay system or
a persistence format.

Images: `Artifacts/VisualProgress/ProtagonistV1_Feasibility/`.

## 1. Results at a glance

| Task | Result |
|---|---|
| 1 Reference board | done: 12 normalised head views (colour + grey), conflicts and gaps marked; originals unchanged (hashes checked) |
| 2 MetaHuman test body | done: config preset `mh_template`, switchable, gameplay unchanged; all movement / ledge / interaction QA green with it |
| 3 Multi-view reconstruction | partly: multi-view measurement works (automatic tracking in 8 of 12 views); the Blender → MetaHuman round trip is **blocked** at the vertex layout; **no experimental head was created** |
| 4 Editor architecture | assessed from the engine source (UE 5.8.4): what runs at runtime and what is editor-only; no decision |
| 5 Performance / quality | measured: placeholder vs MetaHuman test body (GPU, game thread, memory, LOD, feet) |

## 2. Task 1: reference board

**Tools**
- `Tools/protagonist/reference_board.py` (Blender, numpy);
- `Tools/protagonist/compose_board.ps1`.

**Outputs**
- `reference_board_color.png` and `reference_board_gray.png`;
- per-view tiles and `board/landmarks.json`.

**Method**
- Every head view is cropped from its own panel only.
- It is scaled so that eye line → chin = 200 px, with the eye line at the same height.
- The grading is neutralised: grey-world white balance and a 1–99 % luminance stretch over the face region. This makes the views
  comparable; it does **not** recover the skin colour.
- Landmarks are hand-placed (±3–5 px on the 1536 × 1024 sheets). The key-art eye line was corrected by 20 px after the engine tracker
  (task 3) located it.

**Measured with the engine's local landmark tracker on the normalised tiles** (`board/tracked_measures.json`; eye-chin = 200 px)

| View | Mouth width / eye span | Lid opening (px) | Lip height (px) |
|---|---|---|---|
| fs_front | 0.58 | 14.2 | 30.5 |
| fb_front | 0.56 | 14.0 | 32.3 |
| fb_front_neutral | 0.58 | 14.4 | 28.6 |
| 3/4 views (4) | 0.55–0.59 | 12.4–14.9 | 28.8–33.8 |

The front and 3/4 views agree with each other: mouth to eye span within ±3 %, lid opening within ±1 px.

**Contradictions**
1. The two full-body profiles are labelled "LEFT" and "RIGHT" but **both show his right side**. The only profile of his left side is
   face-sheet panel 3.
2. In the full-body profiles the nose is longer and more hooked than in the face-sheet profiles.
3. The face-sheet front looks older (deep forehead lines, heavier beard) than the full-body fronts.
4. Key art vs front views: lips about 10–20 % fuller, and the mouth relatively lower. Head tilt and 3/4 perspective explain part of it,
   not provably all.
5. Face-sheet 3/4 right: the head is raised, so the view is not usable for proportions.

**Missing (every view)**
- hairline, forehead and ears (hidden by the hair);
- neutral flat light (no albedo);
- teeth;
- the back of the head without hair;
- orthographic or long-lens views;
- any length reference (now 193 cm by decision).
- Profiles: the engine tracker finds no face in any of the 4 profiles (see section 4), so depth (eye depth, brow ridge, nose and chin
  projection) still needs hand landmarks on the profiles.

## 3. Task 2: MetaHuman template body as a switchable test preset

**Implementation (C++; gameplay untouched)**
- `USpnRetargetFollowAnimInstance` (new, `Spn/Character/SpnRetargetFollowAnimInstance.*`): a native anim instance whose proxy runs one
  engine node, "Retarget Pose From Mesh". No anim blueprint asset was authored.
- `USpnCharacterVisualComponent`: new preset fields `RetargetMesh`, `Retargeter` and `RetargetIKLodThreshold`, and presets from config
  (`ConfigPresets`). In retarget mode:
  - the gameplay mesh keeps its mesh, `ABP_Unarmed`, foot IK and scale;
  - it is hidden but keeps updating its bones;
  - a new component shows the MetaHuman body, posed every frame from it;
  - followers (the head) follow that body;
  - its LOD is its own (`bSyncAttachParentLOD = false`).
  - `none` restores everything.
- `ASupernaturalProtCharacter::Tick`: the close-camera hide also covers the shown body and its followers.
- Preset `mh_template` (`Config/DefaultGame.ini`):
  - `/MetaHumanCharacter/Body/IdentityTemplate/SKM_Body` + `/MetaHumanCharacter/Face/SKM_Face` (engine content);
  - off by default; `-SpnBody=mh_template` or `Spn.Body mh_template`.
- `Unreal/SupernaturalProt/Tools/character_mh_test_setup.py`: creates the local, gitignored assets `IK_SpnMHTemplate` and
  `RTG_SpnMannequinToMHTemplate`.
  - Found and fixed: the engine MetaHuman IK rig ends the legs at the ball and puts the foot goals on `ball_*`, while our mannequin rig
    ends at the ankle with its own Foot chain. Unchanged, this gave tiptoe feet 5–7 cm in the floor.
  - Also found: the ops of the copied retargeter still pointed at the mannequin rig.
- `Build.cs`: `IKRig` (an engine plugin enabled by default).

**Tests**
- Headless: `SpnGame.Character.RetargetedTestBody` (new), plus the three existing `SpnGame.Character.*`.
- Rendered: `SpnQA.CharacterBody` (new, suite `-Suite Character`). Placeholder and test body run the same scenarios: views, LOD
  distances, memory, walk / jog / sprint, jump, mantle 90 cm, step-on 35 cm.

| Run | Body | Result |
|---|---|---|
| headless `SpnGame+SpnCore` (incl. the new `SpnGame.Character.RetargetedTestBody`) | default | 185 / 185 |
| `20261114-233305` `-Suite Character` | both | 18 / 0 |
| `20261114-233552` `-Suite Movement -ExtraGameArgs -SpnBody=mh_template` (walk, turns, jump, **ledge assist 14 cases**, locomotion) | MetaHuman | 58 / 0 |
| `20261114-234219` `-Suite Doors -ExtraGameArgs -SpnBody=mh_template` (doors, interaction reach, buildings) | MetaHuman | 89 / 0 |

The game log of both suite runs confirms "body preset 'mh_template' … retargeted body SKM_Body".

**Visual comparison:** `compare_front_full.png`, `compare_side_full.png`, `compare_front_head.png`, `compare_threequarter_head.png`,
`compare_mantle_90.png`, `compare_step_on_35.png`. The state before the foot fix is in `before_footfix_mh_front_full.png`.

**Limits found**
- The engine template body is a **~160 cm female body** (`Female/Medium/NormalWeight` skeleton; head bone at 140.7 cm). The V1 target
  is a 193 cm man. Local male-tall body DNAs exist (`Optional/Body/FixedCompatibility/m_tal_{nrw,ovw,unw}.dna`), but none was imported.
- The template material is a region-coloured preview material, not skin. The face is the archetype (no protagonist likeness).
- There is a visible collar seam between the body and head meshes; no grooms, no clothing, no facial animation.
- The log line "height 180 cm" in retarget mode reports the mannequin reference, not the shown body.

## 4. Task 3: multi-view reconstruction

`Tools/protagonist/mh_multiview_experiment.py` (Unreal, works headless with a RHI) and `Tools/protagonist/blender_sculpt_probe.py`
(Blender). Everything ran on a **copy**, `/Game/Characters/ProtagonistV1/_Scratch/MHC_ProtagonistV1_MV`; the original is untouched.

| Step | Result |
|---|---|
| MetaHuman editor subsystem without the editor UI | works: edit session, 93 3D face landmarks, 1397 face model coefficients |
| Engine 2D face tracking (local, `TrackFaceLandmarksFromImage`) on the board tiles | **8 of 12 views**: all fronts, all 3/4 and the key art, 16 curves / 358 points each (lids, lips, nasolabial folds). **0 of 4 profiles** (no face detected) |
| Head geometry out of MetaHuman | works: conform input of the archetype head, 33 845 vertices / 64 094 triangles, as OBJ |
| Blender edit with the vertex order kept | works: deterministic test edit (nose +4 mm forward, face 3 % narrower), 14 388 vertices moved, order and count kept |
| Back into the MetaHuman state (`FitStateToTargetVertices`) | **failed** (returns false, no landmark moved). Engine source: the function expects the **DNA head mesh (mesh 0) vertex layout**, while the conform export is the render mesh of all face parts. Python offers no access to the DNA vertex order. |
| Auto-rigging | not called (cloud) |

**Assessment**
- A better approximation from several views is **possible in principle**:
  - front and 3/4 widths come from the automatic tracker;
  - depth comes from hand landmarks on the two face-sheet profiles;
  - the working path into MetaHuman is the conform / template import (`ImportFromTemplate`, `ConformToTargetMeshes`).
- The missing link is a **head in the DNA mesh-0 layout**. A small C++ editor helper can supply it:
  - read `ArchetypeDNA/SKM_Face.dna` (local) with the engine DNA reader;
  - write it as OBJ in DNA vertex order;
  - estimate: 0.5–1 day. It was not built today.
- **Topology / rig:**
  - A fitted state stays in the MetaHuman identity model by construction (topology kept).
  - A face rig (joints and blend shapes) for a new shape comes **only from the cloud auto-rig** in this pipeline; that cannot be proven
    locally.
  - **Full MetaHuman editability after a Blender sculpt is therefore NOT shown.**

## 5. Task 4: character editor architecture (engine source UE 5.8.4; nothing decided)

| Area | MetaHuman (Character / Palette) | DNACalib + RigLogic | Mutable (beta) |
|---|---|---|---|
| Face shape | Creator (sculpt, landmarks, presets) is **editor-only** (`MetaHumanCharacterEditor`) | DNACalib (runtime module) edits DNA in memory: joints, vertex positions, blend-shape deltas. **Applying that to the skeletal mesh (`USkelMeshDNAUtils::UpdateJoints / UpdateBaseMesh / UpdateMorphTargets / RebuildRenderData`) is `WITH_EDITORONLY_DATA`, so not in a shipped game.** | no DNA / RigLogic support (0 references in the plugin source) |
| Body proportions | parametric body only in the editor; at runtime pick a prebuilt body | — | morph-target parameters at runtime (float), mesh regeneration async |
| Skin / material variants | `UMetaHumanInstance`: runtime assembly from a collection, post-assembly parameters (material parameters) | — | colour, texture, projector parameters (tattoos, scars), layered textures |
| Hair / beard | collection slots (prebuilt grooms) | — | `HairStrandsMutable` (**experimental**) |
| Clothing | outfit pipeline, slot selections at runtime | — | mesh parameters, hidden-face removal, merged meshes |
| Status | `MetaHumanCharacter` beta; `MetaHumanCrowd` experimental | DNACalib stable; RigLogic stable | Mutable beta; clothing / dataflow / groom extensions partly experimental |

**Runtime capability, in short**
- At runtime the engine can choose and combine **prebuilt** parts and change materials: MetaHuman instance or Mutable.
- **Free face sculpting at runtime is not provided** by any of the three. It would need face-shape morph targets authored in the editor
  (for example from DNA variants) or an own mesh deformer.

**Performance and maintainability:** not measured (no customizable-object or collection assets exist; building them is authoring work).
Mutable generates merged meshes (fewer draw calls) at the cost of asynchronous generation time and memory. The MetaHuman face (RigLogic)
and Mutable bodies would run side by side, not integrated.

**Candidates to prototype (no decision)**
- E1: MetaHuman instance + collection (prebuilt faces / hair / clothing + material parameters) + face morph targets from the editor;
- E2: Mutable for body, clothing and skin + the MetaHuman face as a separate component;
- E3: our own slots (`USpnCharacterVisualComponent`) + morph targets.

## 6. Task 5: performance and quality (run `20261114-233305`, RTX 3090, 1920 × 1080 window, editor `-game` Development)

| Measure | Placeholder (Quinn) | MetaHuman test body | Note |
|---|---|---|---|
| GPU frame, close views | 5.68–5.86 ms | 5.83–6.27 ms | about +0.2–0.4 ms; frame-to-frame noise ±0.2 ms |
| GPU frame, 3–50 m | 5.49–5.81 ms | 5.59–6.18 ms | difference within the noise |
| Game thread, close views | 7.2–8.2 ms | 9.1–9.5 ms | about **+1.3–2 ms** |
| Game thread, distance stages | 4.5–4.9 ms | 6.0–6.6 ms | about **+1.5 ms** |
| of which the retarget node | — | 0.50–0.53 ms | engine measurement (editor build) |
| Render thread | 10.2–11.1 ms | **not measured** | the counter read 0 while the MetaHuman preset was active (cause unknown) |
| Triangles drawn, LOD 0 (scene) | 177 k | 253 k | +76 k for body + face |
| LOD at 12 / 25 / 50 m | 1 / 2 / 2 | 1 / 3 / 3 (after the fix) | before the fix the body stayed at LOD 0 at every distance (it followed the hidden mannequin) |
| Textures of the shown parts | 5.5 MB (6) | 5.0 MB resident / 8.1 MB all mips (8) | template materials; a real MetaHuman with 4K skin and grooms will be far higher |
| Meshes | 5.6 MB (89 bones) | 11.9 MB (body 342 + face 875 bones) | |
| Speed walk / jog / sprint | 180 / 350 / 725 cm/s | 180 / 350 / 727 cm/s | gameplay unchanged (checked, ±3 %) |
| Planted foot above the floor, walk / jog / sprint | 0.8 / 1.2 / 1.9 cm | 0.8 / 1.3 / 1.9 cm | before the fix −5.8 / −4.7 / −3.6 cm |
| **Foot sliding** (median, planted foot), walk / jog / sprint | 3.8 / 31.5 / 127 cm/s | **21.6 / 69.6 / 210 cm/s** | **open**: the shorter body takes shorter steps than the capsule moves |

- Blending the leg goals to the source (horizontal) had no measurable effect.
- Remaining remedies: the stride-warp / speed-plant retarget ops (the engine's own MetaHuman retargeter uses them; speed planting needs
  speed curves in the clips), or a body of the target height.

Against the orientation targets of 0015 §8:
- GPU ≤ 1.5 ms for the player: met by the template body (no grooms, simple materials).
- Game thread ≤ 0.5 ms for retargeting + RigLogic: retargeting alone 0.5 ms; the whole difference is about +1.5 ms in the editor
  build. A packaged game must be measured before a budget decision.

## 7. Effort for the next phase (estimates, ±50 %, not measured)

| Step | Content | Hours | Approval |
|---|---|---|---|
| N1 | DNA head exporter (C++ editor helper, DNA mesh-0 layout as OBJ) + round-trip proof (Blender edit → template import → landmarks / renders) | 6–10 | no |
| N2 | profile hand landmarks (both face-sheet profiles) + multi-view proportion fit on the copy, comparison renders | 8–14 | no |
| N3 | male tall test body (local `m_tal_*` DNA) at 193 cm instead of the template, measured again | 3–5 | no |
| N4 | foot sliding: stride-warp / speed-plant ops, or speed curves; target ≤ 1.5 × the placeholder's value | 4–8 | no |
| N5 | render-thread and packaged-game measurement (cost without the editor) | 3–5 | no |
| N6 | editor prototypes E1 / E2, one small asset each, runtime cost | 12–20 | no (local engine plugins) |
| N7 | head sculpt in Blender against the board (artist work) | 15–30 | you / artist |
| N8 | auto-rig + assembly of the fitted face | 2–4 | **yes** (Epic login, cloud, licence) |

## 8. Recommendation for the final production path

1. **Keep the presentation architecture tested today**:
   - gameplay mesh + `ABP_Unarmed` hidden;
   - the MetaHuman body posed by runtime retargeting;
   - head and parts as followers;
   - presets from config.
   Gameplay stayed unchanged in 165 QA checks with a different skeleton.
2. **Body:** MetaHuman parametric body, male, 193 cm, starting from a local male-tall body (N3). Fix foot sliding before anything visual
   is judged (N4).
3. **Face:**
   - hand sculpt in Blender against the board, with the key art as identity master and the face-sheet profiles for depth;
   - into MetaHuman through the template import in DNA layout (N1, N2, N7);
   - fine-tune in MetaHuman Creator;
   - only then the cloud auto-rig (N8, your approval).
   Not recommended: more landmark-only fitting from images (the 2026-10-31 result and the missing depth show its limit).
4. **Hair, beard, clothing:** separate slots as in 0015 §4. Grooms and garments in Blender; the cards / LOD variants for gameplay are
   required.
5. **Character editor:**
   - decide only after the prototypes E1 / E2 (N6);
   - at runtime only prebuilt parts + material parameters + morph targets authored in the editor;
   - free face sculpting in the game is not available from the engine.

## 9. Files of this phase

| Group | Files |
|---|---|
| C++ | `Spn/Character/SpnRetargetFollowAnimInstance.{h,cpp}` (new); `Spn/Character/SpnCharacterVisualComponent.{h,cpp}`; `SupernaturalProtCharacter.cpp`; `SupernaturalProt.Build.cs` |
| Tests | `Spn/Tests/SpnQaCharacterBodyAutomation.cpp` (new); `Spn/Tests/SpnCharacterPresentationAutomation.cpp` |
| Config | `Config/DefaultGame.ini` (preset `mh_template`) |
| Tools | `Unreal/SupernaturalProt/Tools/character_mh_test_setup.py` (new); `Tools/protagonist/{reference_board.py, compose_board.ps1, mh_multiview_experiment.py, blender_sculpt_probe.py}` (new); `Tools/run_gameplay_qa.ps1` (suite Character); `Tools/qa_inventory.json` |
| Docs | this report; `Docs/Development/Gameplay-QA-Inventory.md` (regenerated) |

**Local only (gitignored / outside the repository):**
- `/Game/SpnCharacterPrep/MetaHumanTest/*`;
- `/Game/Characters/ProtagonistV1/_Scratch/MHC_ProtagonistV1_MV`;
- `Saved/SpnCharacter/*`;
- Blender 5.3.0 portable in `%LOCALAPPDATA%\Programs\BlenderPortable` (official zip, SHA256 checked against blender.org).

The winget MSI installation waited for an administrator prompt and was stopped.
