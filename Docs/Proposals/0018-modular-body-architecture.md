# 0018 — Modular human body architecture (2026-11-15) — ENGINEERING REPORT AND PROPOSAL

Status: for developer review. **Nothing here decides an open adult feature.** Developer order of 2026-11-15 ("Anatomische und modulare
Charakterarchitektur").
- No explicit asset was made, bought or downloaded.
- The only placeholder is a neutral engine sphere.
- No adult gameplay system.
- Planning folders untouched. They were read, as the architecture must follow them: `Docs/Game/Adult/02` (packages, technology),
  `06` (body), `07` (customisation), `23` (body system), `28` (saving, mods, builds).

**Planning rules this design follows** (📋 = planned direction in those documents; nothing here changes them)
- **One human core** (02 §1, 06): the optional package has no own body, rig or character system. It only registers extensions **as data**,
  through the same public interfaces as any mod. The core asks no question like "is the package installed?".
- The base game has configurable, **non-explicit** nudity (02 §2).
- Additional anatomical detail options belong to the package's editor menu, in the **same** editor (07 §2, 23 KS-KM-06).
- Package code and content are **not** part of the base build or the console cooks (02 §2, §4).
- Package state is saved **separately** (02 §5.3).

## 1. Inventory (read-only)

`Unreal/SupernaturalProt/Tools/character_body_inventory.py` → `Saved/SpnCharacter/body_inventory.json`.

| Body | Bones | Material slots | Notes |
|---|---|---|---|
| UE mannequin Manny / Quinn | 161 | 2 (stylised suit) | the gameplay skeleton contract; no anatomy, no clothing separation |
| MetaHuman template `SKM_Body` | 342 (228 of them in the hands incl. correctives) | 1 | grey preview material |
| MetaHuman face `SKM_Face` | 875 (843 facial) | 15 (head LODs, teeth, eyes, lashes, ...) | own facial skeleton |
| MH male 193 test body (generated) | 342 | 1, `MI_Body_Skin` with 824 parameters | see below |

**Findings**
- **The underwear of MetaHuman bodies is NOT geometry but material:** `Underwear_Mask`, `Underwear_Body_BaseColor/Normal`,
  `Underwear_Chest_BaseColor/Normal`, `Show Top Underwear`. The body is one closed mesh.
- I checked the material parameters and the skeleton, not the surface detail of the mesh. The skeleton has **no** extra bones for soft
  tissue or anatomical detail (no breast / glute / jiggle bones). The MetaHuman body is made for clothing on top, not for anatomical
  completeness.
- **The skin material already supports:**
  - a built-in **body hide mask** for clothing: `HideMaskTexture`, `HideMaskMaxCullValue`, `HideMaskMinKeepValue`,
    `HideMaskMaxShrinkDistance`;
  - dirt (`Dirt Mask / Color / Normal / Roughness`) and blood (`Blood Mask`);
  - freckles, `Region*` skin colour regions, make-up;
  - hair masks for brows / beard / scalp (stubble painted into the skin).
- **Clothing slots:** the engine plugin has no MetaHuman wardrobe or outfit assets locally (0 found). The project's Core already has the
  data model `AppearanceSpec` (`Docs/Decisions/0005`):
  - clothing slots Head / Torso / Legs / Feet / Hands / Outer / Accessory;
  - layers Tattoo / Scar / Birthmark / Makeup / Dirt;
  - hair and facial hair;
  - body providers;
  - a mod-capable catalog.
  It has **no** underwear slot, no body hair and no detail geometry.
- **Reusable as it is:**
  - the gameplay skeleton contract and runtime retargeting (0016 / 0017);
  - leader pose for followers;
  - the MetaHuman skin material parameters;
  - RigLogic for the face;
  - the IK rig / retargeter setup;
  - `AppearanceSpec` / catalog / editor (undo / redo) in Core.

**Missing for anatomically complete adult bodies** (none of it may be created without separate approval):
1. A base body mesh per sex **without** painted-on underwear, with complete anatomy, on the MetaHuman skeleton (or a parametric body
   model that has it). MetaHuman has no such body.
2. Optional detail meshes per zone, skinned to the same skeleton, with LODs and morph targets that match the body shapes.
3. Underwear as separate garments (meshes), plus a skin texture set without underwear.
4. Body-hair grooms or cards (and the hide mask under clothing).
5. Soft-tissue secondary motion (06 §3): bones or physics in the skeleton, which the MetaHuman skeleton lacks.

## 2. Architecture

```text
            Gameplay pawn (unchanged): capsule, movement, interaction, ledge assist, hidden gameplay mesh + ABP_Unarmed
                                   |  pose (runtime retargeting, 0016)
            Shown body  (USpnCharacterVisualComponent: body preset = body provider)
             |-- head / face        (follower, own facial rig)                      exchangeable
             |-- hair, brows, beard, body hair   (groom slots)                      exchangeable
             |-- skin: material parameters + layers (tattoo / scar / dirt / blood, AppearanceSpec layers)
             |-- wearables by layer: Underwear < Base < Outer < Accessory           USpnWardrobeComponent
             |       each its OWN component (leader pose), never merged into the body
             |-- hidden body zones under opaque wearables  (zone sections or the MetaHuman hide mask)
             '-- optional detail geometry per zone                                  USpnWardrobeComponent
                     only if the policy enables its category AND the zone is exposed
            Rules (engine-independent): spn::core::ResolveCoverage  (Core/include/spn/core/BodyCoverage.h)
            Policy (enabled detail categories): decided OUTSIDE the core - settings, build, installed packs
```

- **Clothing can never be fused with the body.**
  - A wearable is data plus its own mesh; the API has no merge path.
  - The test `SpnGame.Character.WardrobeLayersAndDetails` checks own component, leader pose, the body asset untouched, no collision.
- **Neutral core.**
  - Zones, layers and "detail categories" are generic: a relief scar, a piercing, a prosthesis or anatomical detail of a pack are all the
    same mechanism.
  - A content pack registers detail definitions and a category.
  - Whether a category is enabled is the policy's decision (user setting, platform build, package installed), not the core's.
  - This is exactly the "same public interfaces, no special query" rule of 02 §1.
- **Sexes and proportions:** body providers (male / female base bodies) are `AppearanceSpec.bodyProvider` + body presets. Size and
  proportions are parameters (Core) → body preset / morphs, see section 4. The capsule stays gameplay (0017 A: an open point for 193 cm).

## 3. Implemented prototype (no new assets)

| Part | File | What it does |
|---|---|---|
| Rules | `Core/include/spn/core/BodyCoverage.h`, `Core/src/BodyCoverage.cpp`, `Tests/Core/BodyCoverageTests.cpp` (8 cases) | 15 body zones, 4 wear layers; covered / hidden / exposed zones; opaque vs sheer; hide only where covered; same-layer conflicts reported; detail only with an enabled category and an exposed zone; order independent. **Not saved.** |
| Bridge | `SpnCore/Public/SpnBodyCoverage.h`, `Private/SpnBodyCoverage.cpp` | Unreal types (`ESpnBodyZone`, `ESpnWearLayer`, `FSpnWearableCoverage`, `FSpnBodyDetailDef`, `FSpnPresentationPolicy`, `FSpnCoverageResult`) |
| Application | `Spn/Character/SpnWardrobeComponent.{h,cpp}`, created on the player character | see the list below |
| Hook | `USpnCharacterVisualComponent::OnPresentationChanged` | after every body preset change (mannequin ↔ MetaHuman) everything is rebuilt on the new shown body |
| Data | `Config/DefaultGame.ini` `[/Script/SupernaturalProt.SpnWardrobeComponent]` | one detail marker: an engine sphere on `spine_03`, category `base:test_marker`, **disabled** by default |
| Test | `SpnGame.Character.WardrobeLayersAndDetails` (headless) | layers, separate components, sheer vs opaque, detail on / off / covered, unknown ids, re-attach after a preset change, capsule unchanged |

What `USpnWardrobeComponent` does:
- one follower component per worn item;
- hides body zones through material slots `SpnZone_<zone>`;
- creates detail components only for what the rules return: a skinned follower, or a rigid part on a bone with a draw distance;
- no tick (event driven).

**Not on screen yet:** no body mesh is split into `SpnZone_` slots, and no garment exists. The hiding and the wearables therefore show only
in data and components. Proof on screen needs a split body (section 6, P2).

## 4. Anatomical completeness: the technical interface

| Requirement | Design |
|---|---|
| Fits the existing skeletons | detail = a skeletal mesh on the **same skeleton** as the shown body, leader pose (as the head). Rigid parts: a static mesh on a bone. No new skeleton. |
| Clean transitions | variant **zone meshes** with an identical border vertex ring: the body is split along the zones; a zone variant replaces the body's zone section and shares its border vertices exactly. This gives no seams, no overlap and no z-fighting. Alternative for small details: overlay geometry with a shrink-wrapped border. |
| Skinning / deformation | weights transferred from the body (Blender Data Transfer or UE skin-weight transfer) and corrected locally. Secondary motion needs soft-tissue bones or physics: the skeleton contract has to be extended (decision, 06 §3). |
| Body shapes | details must follow the body proportions: matching morph targets per body shape, or a runtime deformer (proximity wrap, Deformer Graph). **Main risk** (see section 7). |
| Levels of detail | own LODs per detail mesh; a draw distance (already in `FSpnBodyDetailItem`); simulation LOD (02 §6). |
| Hiding covered geometry | zone sections (`SpnZone_<zone>`, implemented) or the MetaHuman hide mask (`HideMaskTexture`, which is in the skin material; one combined mask per worn set is needed). |
| No cost when dressed | implemented: a detail **does not exist** while its zone is covered (test). Hidden zone sections are not drawn. |
| Builds and packages (02 §4) | detail content lives in its own content package / plugin that the base and console builds do not cook. The core code is generic and present everywhere. |

## 5. Character editor: what is possible at runtime and what not

| Change | At runtime (in the shipped game) | Needs the editor / an offline build |
|---|---|---|
| Choose body preset, head, hair, beard, body hair, garments, details | yes: swap prebuilt parts (`USpnCharacterVisualComponent`, `USpnWardrobeComponent`, or the MetaHuman instance / collection) | the parts themselves |
| Height | yes, without scaling: a body preset per size band plus morphs; uniform scaling is ruled out (0017) | the presets / morph targets |
| Proportions (shoulders, hips, muscle, fat) | yes, with **morph targets** authored in the editor, or Mutable float parameters | the MetaHuman parametric body exists only in the editor |
| Free face sculpting | **no** (MetaHuman Creator is editor-only; DNA → mesh is editor-only, 0016 §5) | yes: in the editor; at runtime only morph sliders authored before |
| Skin tone, freckles, regions, make-up, dirt, blood, wetness | yes: material parameters of the MetaHuman skin material (present) | — |
| Tattoos, scars | yes: texture layers (AppearanceSpec layers → baked / blended material layers; Mutable projectors could do it) | authoring of the motifs |
| Optional anatomical variants | yes, as prebuilt detail meshes + morphs, behind the policy | the meshes (separate approval) |

**Is Mutable sensible?** It looks promising for the body, garment and skin part, and is not introduced:
- runtime morph and texture layering, projectors for tattoos, removal of hidden body faces under clothing, merged meshes for NPC crowds;
- but it is beta (grooms experimental) and has no DNA / RigLogic support (0016 §5), so the MetaHuman face stays next to it;
- the decision should follow a prototype with one garment and one body (0016 N6).

## 6. Compatibility

| Area | Result |
|---|---|
| Animations / retargeting | unchanged: wearables and details follow the shown body by leader pose; the retargeted MetaHuman body gets them as well (test). |
| Collision capsule | not touched (test). Open: 193 cm head vs a 180 cm capsule (0017 A). |
| IK / ledge assist | not touched; ledge QA with the male body, see section 8. |
| Mesh slots | wearables per layer, the head as a follower, details per zone; one component each. |
| Save / Load | **not integrated, on purpose.** The save format of the appearance is `AppearanceSpec` (0005). An underwear slot, body hair and detail ids are a schema change (persistence format → master consultation and approval). Package parts belong in a separate save area (02 §5.3). The worn list and policy are not saved today. |
| Performance | each wearable is its own skinned component (+1 draw call per material, skinning on the GPU). A detail costs nothing while covered. Measured for the 193 cm body: GPU +0.8–1.2 ms close up, textures 178 MB resident (0017 D). No garment cost measured (none exists). For NPC crowds: merged meshes (Mutable / mesh merge) later. |
| Multiplayer (later) | replicate small data only: `AppearanceSpec` / worn ids. Each client builds the meshes itself. The **policy is local per client** (what someone allows to be shown on their own screen), never pushed by the server. That fits "the package is not usable in the official online mode" (02 §2). |

## 6a. Tests run (2026-11-15)

| Test | Result |
|---|---|
| Core `ctest` (incl. 8 `body coverage` cases) | 640 / 640 |
| headless `SpnGame+SpnCore` (incl. `SpnGame.Character.WardrobeLayersAndDetails`) | 186 / 186 |
| Rendered QA with the wardrobe component on the character | see the 0017 E table |

Rendered QA runs (all passed):

| Suite | Body | Result |
|---|---|---|
| Movement | default | 58 / 0 |
| Doors | default | 89 / 0 |
| World | default | 88 / 0 |
| Movement | male 193 | 58 / 0 |
| Doors | male 193 | 89 / 0 |
| Character | 3 bodies | 36 / 0 |

No rendered test of wearables or details exists yet: there is nothing to show without a garment or a split body (P2 / P4).

## 7. Production plan (estimates ±50 %, not measured)

| Step | Content | Hours | Approval |
|---|---|---|---|
| P1 | appearance schema v2 (underwear slot, body-hair slot, detail ids, worn layers) + migration; master consultation | 8–14 | **yes** (persistence format) |
| P2 | split the existing test body into `SpnZone_` sections in Blender (no new anatomy; a split only) + test on screen | 6–10 | no |
| P3 | combined hide mask for MetaHuman bodies (one mask per worn set, render-target compositing) | 8–12 | no |
| P4 | first real garment set incl. separate underwear (Blender), `SpnQA` wardrobe run, performance | 20–40 per set | asset source |
| P5 | body hair as grooms or cards, with a mask under clothing | 10–20 | asset source |
| P6 | morph-target based proportion sliders (body), runtime | 15–25 | no |
| P7 | soft-tissue bones / physics in the skeleton contract | 15–30 | **yes** (skeleton contract, 06 §3) |
| P8 | anatomically complete base bodies + optional detail meshes (male / female), border-ring-compatible zone variants, morphs per body shape | 80–200+ (artist work) | **yes, separate** (explicit assets, package structure, legal, 10) |
| P9 | package / plugin separation of the detail content (not in base / console cooks), the policy from settings and packages | 10–20 | after decisions in 02 / 11 / 34 |
| P10 | Mutable prototype (garment + body + tattoo projector) | 12–20 | no |

Order: P2 → P3 → P4 make the architecture visible without explicit content. P1 and P7 are decisions. P8 / P9 only after the open adult
decisions (11, 34).
