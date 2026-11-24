# 0017 — Protagonist V1, technical phase 2 (2026-11-15) — ENGINEERING REPORT

Status: for developer review. **No final character, nothing bought, no cloud, no Epic login.** Developer order of 2026-11-15 ("technische
Phase 2"), priorities A–E. `Planning/Master` is not reachable on this machine; no master search ran. The order is engineering (test body,
animation, tooling), not a gameplay system or a persistence format.

Images and videos:
- `Artifacts/VisualProgress/ProtagonistV1_Phase2/` (`compare3_*.png`, `videos/`);
- earlier state: `ProtagonistV1_Feasibility/`.

## Results at a glance

| Task | Result |
|---|---|
| A Male test body | **done**: preset `mh_male_193`, a local parametric male MetaHuman body at 193 cm. Generated locally with the public engine API: not scaled, no rig, no cloud. |
| B Foot sliding | **cause found and fixed** for sprint and for every retargeted body; walk / jog / run on the placeholder are unchanged (limited by the clips) |
| C Head round trip | **prototype done, only public interfaces**. Lossless only between Unreal and Blender. The way back into MetaHuman is a model fit, not a copy. The lossless DNA route is blocked without the cloud rig. **Stopped there as ordered.** |
| D Quality / performance | videos for walk, jog, sprint, jump, ledge (3 bodies); editor measurements of GPU, CPU and memory; **no packaged build** (see D) |
| E QA | see the table in E |

## A. Male test body (`mh_male_193`)

- Source: a **copy** of `MHC_ProtagonistV1`, named `/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/MHC_SpnTestMaleTall`. Its face is the
  unfinished 2026-10-31 preview: **no likeness, no character choice**.
- The local engine body `m_tal_nrw.dna` (fixed-compatibility "male, tall, normal weight") was tried first. `ImportBodyWholeRig` refuses it:
  `CombinedBodyCannotBeImportedAsWholeRig`.
- Used instead: the parametric MetaHuman body with the height constraint at 193 cm. Measured after the solve: **193.1 cm**; generated mesh
  feet to head top: 193.3 cm.
- Meshes, materials and textures come from the public, exported C++ API `UMetaHumanCharacterEditorSubsystem::TryGenerateCharacterAssets`
  through the new editor-only helper `USpnMetaHumanPrepLibrary`. A full `BuildMetaHuman` would need a face rig and downloaded textures
  (cloud).
- **No scaling anywhere.** The preset only uses its measured height (`HeightCm=193`) to move the camera pivot (+11.5 cm at factor 0.9).
- Own IK rig and retargeter (`IK_SpnMHMaleTall`, `RTG_SpnMannequinToMHMaleTall`), set up by `Tools/character_mh_test_setup.py`.

**Checks** (idle pose, measured on the shown body from the capsule bottom; `SpnQA.CharacterBody`, run `20261115-002651`)

| | Placeholder (Quinn) | MH template | MH male 193 |
|---|---|---|---|
| Head bone | 156.6 cm | 138.1 | 169.0 |
| Top of the shown meshes (idle, slightly bent) | 174.3 | 157.3 | 188.2 (193.3 standing straight) |
| Pelvis | 93.4 | 82.4 | 93.3 |
| Hand (right) | 87.0 | 77.1 | 90.1 |
| Hip to ankle | 85.4 | 75.0 | 88.3 |
| Shoulder joints apart | 33.2 | 28.2 | 37.2 |
| Capsule (radius / half height) | 35 / 90 | unchanged | unchanged |
| Camera base z | 0 | 0 | +11.5 |
| Mantle 90 cm: hand closest to the top surface | 1.0 cm | 0.2 cm | 5.0 cm |

**Proportions:** the 193 cm body has the same pelvis height as the mannequin (93 cm), but a longer upper body and a larger head.

**Collision capsule (open point):**
- The capsule stays 180 cm tall (gameplay rule). The head of the 193 cm body reaches about 8–13 cm above it.
- Under low ceilings and door frames below about 195 cm, the head can clip through, because the capsule does not cover it.
- Decision needed: either keep the capsule and design door / passage heights to at least 205 cm, or change the protagonist's capsule
  (a gameplay change, needs approval).

**IK / hands:** there is no hand IK to ledges or door handles (not before this phase either). The hands come to within 1–5 cm of the ledge
top without it. A hand target for the ledge move is part of the animation plan (`Ledge-Animation-Plan-2026-11-14.md`).

**Interaction heights:** the interaction reach is gameplay (camera and capsule) and unchanged. Visually the hand hangs 3 cm higher than the
mannequin's.

## B. Foot sliding: diagnosis and fix

**Diagnosis** (`Unreal/SupernaturalProt/Tools/locomotion_stride_check.py`, `Saved/SpnCharacter/locomotion_stride.json`)
- All walk / jog clips are **root-motion clips**: walk authored at **300 cm/s**, jog at **600 cm/s** (root travel over one loop).
- The blend space `BS_Idle_Walk_Run` has its samples exactly at 0 / 300 / 600 and ends at 600.
- The gameplay speeds are 180 / 350 / 550 / 800 cm/s (Core `MovementConfig`).
- Consequences:
  1. **Sprint (800):** the jog clip kept its 600 cm/s cadence, so the planted foot slid by the difference. Measured 127–178 cm/s against a
     capsule speed of 727–744.
  2. **Retargeted bodies:** the copied retargeter had **no IK goal op**, so the leg goals followed the FK chains, whose stride scales with
     the leg length while the capsule speed does not. The extra sliding is speed × (1 − leg ratio): for the 160 cm template at walking
     speed 180 × 0.12 ≈ 22 cm/s, **measured 21.6**. My "blend to source" setting of 2026-11-14 used the deprecated pre-5.6 chain struct and
     had no effect.
  3. **Jog (350) and run (550)** lie between the walk and jog samples. Two clips with different cadences are blended, which leaves about
     30–50 cm/s on every body. It needs clips authored for those speeds (or other gameplay speeds): not changed.

**Fixes (the cause, not foot locking)**
- `USpnAnimInstance` (the C++ parent of `ABP_Unarmed`):
  - above the fastest authored clip speed (`SpnLocomotionAuthoredTopSpeed` = 600, measured) the mesh plays faster by speed / 600;
  - capped at 1.6, smoothed;
  - **only on the ground, never during the ledge assist or in the air**.
- Retargeter (`Tools/character_mh_test_setup.py`): an engine "Blend To Source" op "Spn Leg Goals" as a child of "Run IK Rig" (the layout
  of the engine's own `RTG_MH_IKRig`). The leg goals take the source foot position **horizontally** (weights 1, 1, 0); the vertical stays
  scaled to the body.
- **Rejected:** a sprint row in the blend space written from Python (jog clips at 900 with rate 1.5). It made the sliding worse on every
  body (walk 3.8 → 61 cm/s), because the Python edit changed the blend-space weighting. The original asset was restored; git shows it
  unchanged.

**Measured again under identical conditions** (same lane, same input, `SpnQA.CharacterBody`). Median horizontal speed of a planted foot in
cm/s; the 90th percentile is in the run's report.

| | Placeholder before → after | MH template before → after | MH male 193 (new) |
|---|---|---|---|
| Walk 180 | 3.8 → 3.8 | 21.6 → 5.2 | 4.5 |
| Jog 350 | 31.5 → 31.4 | 69.6 → 33.2 | 30.5 |
| Run 550 | 48.7 (new case) → 49.9 | – → 48.1 | 52.5 |
| Sprint (mean 740) | 177.8 → 51.1 | 210 → 54.7 | 45.3 |
| Start + stop (walk) | 9.8 → 10.3 | – → 12.6 | 10.5 |
| 90° turn while jogging | 31.1 → 30.9 | – → 30.8 | 30.0 |

"Before" for the placeholder: run `20261115-002416`, the same scenarios with the original blend space. "Before" for the MH template: run
`20261114-233305`. "After": run `20261115-002651`.

**No new visible errors:**
- jump and ledge (mantle 90, step-on 35) complete on all three bodies with the same times (1.00–1.01 s / 0.58–0.59 s);
- Movement / Ledge QA, see E.

## C. MetaHuman head round trip (unchanged head)

`Tools/protagonist/mh_head_roundtrip.py`, `Tools/protagonist/blender_fbx_passthrough.py`. Output: `Saved/SpnCharacter/head_roundtrip_*.json`
and `/Game/SpnCharacterPrep/MetaHumanTest/HeadRoundTrip`.

**Interfaces used (public and Python-exposed)**
- `MetaHumanCharacterExportBlueprintLibrary.export_dna` / `export_dcc`;
- `AssetExportTask` (FBX);
- `MetaHumanCharacterEditorSubsystem.import_from_template` / `import_from_face_dna` / `translate_face_landmarks`;
- plus `TryGenerateCharacterAssets` (public C++).

| Step | Result |
|---|---|
| `ExportDNA` (head `.dna`) | **refused**: "the character has no face DNA. Create a rig before exporting the head DNA" |
| `ExportDCC` (package for DCC tools) | **refused**: "not rigged", "textures missing (download texture sources)", i.e. cloud |
| FBX export of the head mesh | works in the editor (in a commandlet the FBX exporter asserts on `MeshObject`) |
| Blender 5.3 import → export without an edit → import | **lossless**: LOD 0 has 34 657 vertices, 64 094 faces, 192 282 loops, **identical vertex order, positions and UVs** (difference 0), 668 skin vertex groups identical. The other 7 LODs are also identical in count and order. |
| Back into MetaHuman (`ImportFromTemplate`, vertices matched by UV) | `SUCCESS`, both directly (A) and through Blender (B). **A and B are identical** after mesh generation (max 0.02 cm, same index buffer, same UVs): Blender adds no error. |
| Is it the same head? | **No, it is a fit into the MetaHuman face model**, see the table below |
| Still editable in MetaHuman? | **yes**: after the import the face model takes landmark edits (nose-tip landmark moves; 1.2 mm for a requested 3 mm, limited by the model) |
| Rig state after the import | `Unrigged`: a face rig only comes from the cloud auto-rig |
| DNA import (lossless, with the rig) | **not possible**: no DNA can be exported without the cloud rig |

**Fit accuracy of the unchanged head** (head skin, LOD 0; order and UV independent nearest-neighbour compare)

| Variant | Head size (RMS radius) | Shape difference to the original, size-normalised |
|---|---|---|
| Default face before the import (reference) | 10.1 → 9.17 cm | mean 3.1 mm, p95 8.4 mm |
| `SCALING_ROTATION_TRANSLATION` (engine default) | 10.1 → 9.16 cm | mean 2.1 mm, p95 5.7 mm |
| `ROTATION_TRANSLATION` | 10.1 → 10.1 cm | mean 3.5 mm, p95 9.1 mm |
| `NONE` | 10.1 → 12.8 cm | mean 12 mm (unusable) |

- The 2026-10-31 V1 asset reports 93 face landmarks and a different UV layout than newly created characters (79 landmarks; only 0.8 % of
  the UVs agree). Landmark-by-landmark comparison is therefore impossible; the shape compare above is.
- **Conclusion:** with public interfaces only, a sculpted head goes back into MetaHuman as a *fit* of about 2–3.5 mm mean deviation, and
  either with a size change or with a larger shape error. Topology, vertex order and UVs of the *generated* head are MetaHuman's own and
  stay consistent (A = B). A lossless transfer of a sculpt including the rig would need the DNA route, and that needs the cloud rig first.

**Alternatives** (decision needed; nothing done):
1. Accept the fit: sculpt in Blender, `ImportFromTemplate`, then correct in MetaHuman Creator. Quality limit about 2 mm mean.
2. With your approval for the cloud auto-rig: rig V1 once, then DNA export → Blender (DNA layout) → DNA import with the whole rig (the
   documented lossless path).
3. Detail beyond the model as an extra layer: the normal map from the sculpt, or morph targets on the final head (outside the MetaHuman edit).

Not used, as ordered: private engine internals, our own DNA writing, editing the DNA layout via C++ internals.

## D. Character quality and performance

**Animations:**
- All locomotion, jump and ledge scenarios run on all three bodies with unchanged gameplay (speeds ±3 %).
- The batch retarget of all 20 player clips onto the male body: see E (`character_candidate_check.py`).

**Videos** (`Artifacts/VisualProgress/ProtagonistV1_Phase2/videos/`)
- `<body>_{walk,jog,sprint,jump,mantle_90,step_on_35}.mp4` for each body;
- `<body>_all_moves.mp4` per body;
- `compare_placeholder_vs_male193.mp4` side by side.
- Cut from the recording of run `20261115-002651`.

**Editor measurements** (editor `-game`, Development, RTX 3090, 1920 × 1080, run `20261115-002651`)

| | Placeholder | MH template | MH male 193 |
|---|---|---|---|
| GPU frame, close views | 5.5–5.6 ms | 5.8–6.1 ms | **6.3–6.8 ms** |
| GPU frame, 3–50 m | 5.3–5.5 ms | 5.4–5.8 ms | 5.9–6.4 ms |
| Game thread, close / distance | 7.0–7.8 / 4.3–4.8 ms | 9.1–9.2 / 6.0–6.2 ms | 9.3–10.8 / 6.2–6.7 ms |
| of which the retarget node | – | 0.52 ms | 0.50 ms |
| Render thread | 9.4–10.9 ms | not measurable (counter 0 with a retargeted body) | the same |
| Triangles drawn (scene), LOD 0 | 177 k | 253 k | 254–261 k |
| LOD at 12 / 25 / 50 m | 1 / 2 / 2 | 1 / 3 / 3 | 1 / 2 / 3 |
| Textures (count, resident / all mips, largest) | 6, 5.5 / 5.5 MB, 1K | 8, 5.0 / 8.1 MB, 2K | **72, 178 / 518 MB, 4K** |
| Meshes | 5.6 MB | 11.9 MB | 11.9 MB |

- The male body carries the locally synthesised MetaHuman skin textures (4K): that is the real texture budget of a MetaHuman.
- Orientation targets (0015 §8):
  - GPU ≤ 1.5 ms for the player: met (+0.8–1.2 ms at close range, no hair, no clothing);
  - textures ≤ 300 MB: resident 178 MB met, all mips 518 MB not (texture streaming decides).

**Packaged build: NOT measured.** All numbers are editor measurements: editor overhead is included in the game thread, and editor-only
measurements are included (the retarget node's own timing exists only with `WITH_EDITOR`). A packaged measurement needs:
- a cook list for the test assets, which are referenced only from config (`/Game/SpnCharacterPrep/MetaHumanTest`);
- a CSV-profiler run with `-SpnBody=…`.
Estimate: 3–5 h, including the first packaged build of the project (`DefaultGame.ini` still says "no packaged build has been made yet").

## E. QA

| Suite | Body | Run | Result |
|---|---|---|---|
| `-Suite Character` (`SpnQA.CharacterBody`, 3 bodies × 13 scenarios + views / LOD / memory) | all | `20261115-002651`, again `20261115-014832` (with the wardrobe code) | 36 / 0, 36 / 0 |
| `-Suite Movement` (walk, turns, jump, **14 ledge cases**, locomotion) | default | `20261115-012335` | 58 / 0 |
| `-Suite Doors` (doors, interaction reach, buildings) | default | `20261115-013001` | 89 / 0 |
| `-Suite World` (`Lvl_SpnWorld` incl. save → menu → load) | default | `20261115-013301` | 88 / 0 |
| `-Suite Movement` | `mh_male_193` | `20261115-013904` | 58 / 0 |
| `-Suite Doors` | `mh_male_193` | `20261115-014530` | 89 / 0 |
| headless `SpnGame+SpnCore` (incl. `RetargetedTestBody`, `WardrobeLayersAndDetails`) | default | log 2026-11-15 | 186 / 186 |
| Core `ctest` (incl. `BodyCoverage`, 0018) | – | – | 640 / 640 |
| `character_candidate_check.py`, the player's 20 clips batch-retargeted to the male body | `mh_male_193` | `Saved/SpnCharacter/candidate_check.json` | 20 / 20 retargeted, frame counts identical; verdict "RETARGETS (visual review needed)". Its height (159.8 cm) measures the body mesh **without** the head; standing with the head it is 193.3 cm (A). |

Fixed on the way:
- `character_candidate_check.py` deleted and re-duplicated its retargeter copy. The delete fails silently in a commandlet, so the check
  reported BLOCKED; it now reuses the copy.
- QA wrapper calls must splat a hashtable (my first background run passed "-Suite" as a value and ran nothing; repeated).

## Continuation point

1. Decide on the capsule vs the 193 cm head (A).
2. Decide on the head route (C): fit (1), or cloud rig + DNA route (2).
3. Jog / run cadence (B3): new clips for 350 / 550 cm/s, or other gameplay speeds.
4. Packaged measurement (D).
5. Then the head sculpt work starts.
