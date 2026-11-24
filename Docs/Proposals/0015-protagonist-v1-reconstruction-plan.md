# 0015 — Protagonist V1: reference assessment and production plan (2026-11-14) — ENGINEERING PROPOSAL

Status: for developer review. **No model generated, nothing bought or downloaded, nothing chosen.** Developer order of 2026-11-14
("Protagonist V1"):
- assess all references in `References/Characters/Protagonist`;
- keep the identity: face, hair, beard, body proportions, overall presence;
- use the existing MetaHuman / retarget pipeline where it fits;
- separate body, head, hair and clothing;
- V1 is only a preset of a long-term character editor;
- write a concrete production plan: effort, quality risks, animation integration, performance targets.

Basis: `Docs/ProtagonistV1/README.md` (first preview, 2026-10-31), Proposal 0013 (pipeline), Proposal 0014 (first player character).
`Planning/Master` is not reachable on this machine; no new master search ran. The master findings P1/P2 in the README (§1) still apply:
a player-created protagonist and an extensive character creator. **The developer's order resolves the open question in README §1:
V1 is a preset, not a fixed hero.**

## 1. The references (all three files read)

| File | Content | Size |
|---|---|---|
| `protagonist_v1_reference.png` | key art: head and torso, 3/4 view to the character's left, dramatic dusk light, wet / dirty skin | 1536 × 1024 |
| `protagonist_v1_face_sheet.png` | 20 panels: 7 head views (front, 3/4, both profiles, back, looking down), 6 detail crops (eye, nose, cheek / stubble, mouth, ear, hair), 6 expression / lighting variants | 1536 × 1024 |
| `protagonist_v1_fullbody_sheet.png` | 8 head views (front, front neutral, 3/4 both, profiles, up, down), 8-view full-body turnaround, 2 environment shots, 4 action / idle poses, 8 detail crops (eye, nose, mouth, skin, both hands, boot, jacket) | 1536 × 1024 |

The images are generated renders (consistent style, no EXIF camera). The file list printed inside the full-body sheet
("Refeences\Protagonist\V1\v1_fullbody_front.png" …) is part of the picture: **these single files do not exist**.

### 1.1 Identity as read from the sheets (to be confirmed by you)

- Face: long, narrow; straight long nose with a defined tip; deep-set eyes, hazel / green-brown; straight low brows, strong brow ridge;
  lean cheeks with visible cheekbones; angular jaw, slightly square chin; horizontal forehead lines; age about 30–40.
- Beard: short full stubble (3–6 mm look), moustache slightly denser, connected to the chin.
- Hair: medium-long (chin to collar), dark brown with warm red-brown highlights, wavy, messy, wet / tousled, falls over forehead and
  ears; no visible parting.
- Body: tall, lean, athletic, not bulky; about 7.5 head heights; long legs.
- Clothing (V1 outfit): weathered brown-black hooded field jacket, plaid flannel shirt, black T-shirt, leaf / arrowhead pendant on a
  cord, chest straps and pouches, dark cargo trousers with a thigh holster, laced leather boots, large backpack with a long object
  (rifle / tool) on the back.
- Presence: tired, alert, weathered, grounded.

### 1.2 Suitability for 3D reconstruction

| Panel group | Use | Quality |
|---|---|---|
| Head front (neutral), 3/4 both sides, profiles both sides (face sheet panels 1–5; full-body sheet head row) | **multi-view landmarks** for the head shape: front gives widths, profiles give depth (eye depth, brow ridge, nose profile, chin projection) — exactly what the single-view fit of 2026-10-31 could not see | usable; about 200 × 250 px per head, so ±1–2 mm on a real head at best |
| Key art | likeness check (the "does it look like him" image), skin and mood | good, but one view and dramatic light |
| Detail crops (eye, nose, mouth, ear, skin, hands) | painting reference for skin, pores, stubble density, iris colour, ear shape | good as reference, not as texture (lighting baked in) |
| Full-body turnaround (8 views) | proportions (head height ratio, leg / arm length, shoulder width), clothing layers and silhouette | usable for ratios; each figure is only about 130 × 370 px, so no measurements in cm |
| Action / idle poses, environments | mood, how the clothes fold, idle reference for animation | reference only |
| Look down / look up / back | crown, back hair volume, neck | reference only |

### 1.3 What is missing or unreliable

1. **No absolute scale.** Height is unknown (the 183 cm in `protagonist_v1_appearance.json` is an estimate). A decision is needed.
2. **Identity drift between panels.** Generated sheets are not one consistent person: comparing the panels, the front view looks older
   and more lined than the 3/4 views; lip fullness and jaw width vary. A reconstruction must pick a "master" view per feature.
3. **The two profiles of the full-body sheet ("LEFT" and "RIGHT") show the same side** of the face; real opposite profiles exist only
   in the face sheet.
4. **Hair hides the forehead, hairline, temples and ears** in every view. The skull shape and hairline have to be invented.
5. **No neutral lighting.** All panels have strong directional light, dirt and wetness. Albedo (skin colour without light) cannot be
   taken from them; skin tone has to be set by eye.
6. **No orthographic / long-lens views**, unknown focal length; perspective distortion differs from panel to panel.
7. Not shown at all: teeth, tongue, neutral expression with eyes fully open, back of the head without hair, hands' palms, the
   clothing's back details under the backpack, the long object on the back, the boot soles.
8. Low resolution (1536 × 1024 for 20 panels): fine skin detail must come from the MetaHuman skin set, not from the images.

**What would help most (you could produce it with the same tool that made the sheets, or I can describe it exactly):**
- a head turnaround of the same person, neutral expression, flat even light, hair pulled back or wet-combed so the hairline and ears
  show: front, both 3/4, both profiles, at least 1024 px per view;
- the same for the body in a tight base layer (A-pose), front / side / back;
- one sentence on height and age.
These are inputs, not purchases; consistency between the new and old images must again be checked.

## 2. Lessons from the first attempt (2026-10-31)

`Saved/ProtagonistV1/v5_sheet.png`, `v5_views_sheet.png`: the automatic landmark fit on the single key-art view got the 2D landmarks
within about 4–5 px, but **the face does not look like him** (rounder, softer, eyes not deep-set, fuller lips, weak brow ridge and jaw,
no hair in the renders). Reasons: one view, no depth, the Victor preset as a start, and landmark sculpting only moves 79 points. More
iterations of the same method will not fix this. That is the "arbitrary MetaHuman that only looks roughly similar" the order rules out.

## 3. Technical options (UE 5.8.4, checked locally)

Locally available engine plugins (no download): `MetaHumanCharacter` (beta; Creator modules are **Editor-only**, runtime module for
the assembled character), `MetaHuman` (Identity / Mesh-to-MetaHuman, Animator), `DNACalib` (runtime module), `RigLogic`, `HairStrands`
(+ `HairStrandsMutable`), `ChaosClothAsset` / `ChaosOutfitAsset`, `Mutable` + `MutableClothing` (beta, runtime customisation),
`MotionWarping`, `LiveLink*`. **Blender is not installed** on this machine.

| Option | How | Likeness | Effort | Verdict |
|---|---|---|---|---|
| A. MetaHuman Creator only (presets, landmarks, sculpt) | what was tried; multi-view landmarks would improve it | medium; limited by preset blending | low | not enough on its own |
| **B. Blender head sculpt → Mesh-to-MetaHuman / conform** | sculpt a neutral, hairless head in Blender against front + profile + 3/4 image planes; conform the MetaHuman face to it (`conform_to_target_meshes` / Identity from mesh); then the MetaHuman rig, skin, eyes, teeth | **high**: depth is sculpted by eye from the profiles; the rig and LODs stay MetaHuman quality | medium-high | **recommended** |
| C. Fully custom character in Blender (no MetaHuman) | own topology, rig, facial blendshapes, LODs | high in theory | very high (face rig) | only if MetaHuman is ruled out (licence / open source) |
| D. Image-to-3D generator or photogrammetry service | AI head from images → conform | unknown; generated heads are usually soft | low-medium | needs approval (external service / download); possible shortcut for the B base mesh, not a replacement for the sculpt pass |

Option B in detail: the sculpt only has to deliver the **shape**. The MetaHuman conform gives topology, the facial rig (RigLogic DNA),
eyes, teeth, LODs and skin; the result stays a normal MetaHuman Character asset, editable in MetaHuman Creator afterwards.

## 4. Separation into parts (for the character editor)

| Part | Asset | Skeleton / binding | Exchangeable |
|---|---|---|---|
| Body | MetaHuman parametric body (height, build, proportions) | `metahuman_base_skel` | body preset per character |
| Head / face | MetaHuman face (DNA, own facial skeleton) | follower of the body; head-slot rule (0014 §2) | face preset (DNA) |
| Skin / eyes / teeth | MetaHuman skin and eye materials, plus an own detail layer (scars, freckles) as a mask | material parameters | yes, without new meshes |
| Hair (head) | groom asset (strands) + card LOD | groom binding to the face mesh | hair slot |
| Brows, lashes, beard, moustache, peach fuzz | MetaHuman grooms | groom bindings | separate slots each |
| Clothing layers | T-shirt, flannel, jacket, trousers, boots, harness / pouches, pendant, holster, backpack | skinned to the body skeleton (followers), cloth sim only where it helps (jacket hem, hood) | slot per layer |
| Dirt / wetness | material layer (parameters), not baked into the albedo | — | runtime state (weather, story) |

V1 = one preset that names an entry per slot. The same slots serve NPCs later (0013).

## 5. Character editor (long term) — what fits, what is open

- **MetaHuman Creator cannot run in the shipped game** (its modules are Editor-only). An in-game editor therefore needs its own runtime
  path. Candidates, both local engine plugins, **to be prototyped**:
  - `DNACalib` (runtime module) for face changes on the DNA (blend between face presets, scale regions);
  - `Mutable` (beta) for clothing layers, body morph and material parameters at runtime, including grooms (`HairStrandsMutable`).
- Until that is proven, the editor can work with **presets made in the editor** (faces, bodies, hair) plus runtime material parameters.
- This keeps V1 a preset: none of its values become game rules.

## 6. Production plan

Hours are my estimates for one person who knows Blender; **not measured**, ±50 %. At 8–15 h per week of your time, the art steps are
the bottleneck. What I can do myself: scripts, Unreal integration, measurements, landmark fits, comparison renders. What I cannot do
well: hand sculpting, groom styling and clothing modelling by eye.

| Step | Content | Effort | Who | Approval |
|---|---|---|---|---|
| P0 | reference board: crops of all usable views, scaled to the same head height; your choice of the "master" view per feature; height and age | 3–5 h | me + 1 h your review | no |
| P1 | missing references (§1.3): neutral head / body turnaround | 2–6 h | you (same tool as the sheets) | your decision |
| P2 | install Blender (free, open source) | 0.5 h | you or me | **yes** (download) |
| P3 | multi-view landmark fit in Unreal as a better start (front + both profiles + 3/4; extends `spn_mh_fit_math.py` to several cameras) | 6–10 h | me | no |
| P4 | head sculpt in Blender (neutral, hairless) against the image planes, starting from the exported P3 head | 15–30 h | artist / you | no |
| P5 | conform the MetaHuman face to the sculpt; adjust in Creator; comparison renders front / 3/4 / profile against the references | 6–10 h | me + review | no |
| P6 | cloud rig and texture assembly of `MHC_ProtagonistV1` | 2–4 h | me with your Epic login | **yes** (Epic account, cloud, licence check) |
| P7 | skin: tone, roughness, scars / redness masks, dirt / wetness layer | 8–15 h | me + review | no |
| P8 | hair groom in Blender (hair curves → Alembic → groom), card LOD; beard / brows from the MetaHuman set, tuned | 15–30 h (+8–15 h cards) | artist / you | no |
| P9 | V1 outfit in Blender: 9 pieces (§1.1), cloth draping, retopology, UVs, bake, skinning to the MetaHuman body | 60–120 h | artist / you | approval per external asset if any |
| P10 | integration: preset `protagonist_v1` in `USpnCharacterVisualComponent`, head-slot rule, camera height from the real height, LODs, QA | 10–20 h | me | no |
| P11 | performance and visual acceptance (§8, §9) | 4–6 h | me + your approval | **yes** (visual approval of V1) |

Total about 130–260 h; of that about 100–200 h of art (P4, P8, P9). The head first (P0–P7, about 40–75 h) gives a playable V1 in the
current placeholder clothing; the outfit follows.

## 7. Animation integration

- Gameplay character unchanged (capsule, movement, ledge assist, interaction): 0013 §4 rule 4.
- Body: runtime retargeting from the hidden mannequin running `ABP_Unarmed` (0014 §3; all 20 clips already retarget onto the MetaHuman
  skeleton). Bake later only if the CPU cost demands it.
- Face: MetaHuman face anim blueprint with RigLogic; idle blinks and breathing first; Live Link Face / MetaHuman Animator later for
  cutscenes (local plugins, no new download).
- Ledge animation plan (`Ledge-Animation-Plan-2026-11-14.md`): hand and foot IK targets work unchanged on the MetaHuman skeleton;
  the foot IK needs a boot-sole offset.
- Clothing: the jacket hem and hood as Chaos cloth or as AnimDynamics; the backpack as a rigid attachment with a small spring;
  the hair groom with physics only on the strands tier.
- Tests: the existing Movement, Ledge and World QA must stay green with the V1 preset (`-SpnBody=protagonist_v1`), as 0014 C2 plans.

## 8. Performance targets (PROPOSAL, to be measured; RTX 3090, 1440p, 60 fps)

The forest section already uses most of the 60 fps budget (0014 §5). Proposed budget for the player character in the gameplay camera:

| Item | Target |
|---|---|
| GPU cost of the player (body, face, hair, clothing, shadows) | ≤ 1.5 ms gameplay, ≤ 4 ms in close-up cutscenes |
| Face LOD in gameplay | LOD 1–2 (LOD 0 only in close-ups) |
| Hair | cards in gameplay on medium / low; strands only on high and in cutscenes |
| Triangles at the gameplay LOD (body + clothing, without face) | ≤ 120 k |
| Textures | face up to 4K, body and clothing 2K; streaming budget for the player ≤ 300 MB |
| Runtime retarget + RigLogic CPU | ≤ 0.5 ms game thread |

These are proposals, not approved budgets (0014 C5 / 0013 §6.4). P11 measures them.

## 9. Quality risks

| Risk | Effect | Counter-measure |
|---|---|---|
| inconsistent identity across the generated panels | the result looks like "a" version, not "the" | P0: you choose master views per feature; P1 consistent turnaround |
| the conform smooths the sculpt | loss of deep-set eyes, brow ridge | check after the conform; correct in Creator; compare profiles |
| hair: the free MetaHuman set has no matching style | the look changes most visibly with the hair | own groom (P8); a card version is needed for gameplay anyway |
| skin tone from dramatic light | too orange or too pale | neutral lighting test scene, decide by eye |
| clothing effort | V1 long without his jacket | head first, outfit second; placeholder garments in between |
| MetaHuman licence and the open-source goal | MetaHuman assets may not go into a public repository | keep them local, outside git (like Fab content); read the licence text before P6 |
| beta plugins (`MetaHumanCharacter`, `Mutable`) | API changes in later UE versions | wrapper scripts, no deep engine dependencies |
| performance of strands and cloth | frame budget in the forest | cards and LODs from the start; measure early (0014 C2) |

## 10. Decisions for the developer

1. Confirm the identity description of §1.1 and choose the master view per feature (front panel or 3/4 for the jaw, etc.).
2. Height (and age) of V1.
3. Missing references: will you produce a neutral turnaround (§1.3)?
4. Path B (Blender sculpt → MetaHuman conform): yes? Installing Blender (free) approved?
5. Who does the art steps P4 / P8 / P9 (you, an artist, or a shortcut via option D with its own approval)?
6. P6: Epic login, cloud assembly, licence check — when?
7. The performance budget of §8 as the starting point?

## 11. Next safe steps without approval

- P0 reference board (crops into `Saved/`, no change to the references);
- P3 multi-view landmark fit on the existing `MHC_ProtagonistV1` (local, reversible; produces comparison renders, not the final model);
- 0014 C1 + C2 (template body as a switchable test body with QA and performance numbers), which also measures the cost of the slots of §4.
