# Protagonist V1 — MetaHuman preview (work in progress, NOT approved)

Status 2026-10-31: **first preview for visual review.** Not integrated into the game, not a replacement for the current player
character, no rig, no cloud textures, no assembly. Nothing here is master canon; values are placeholders.

## 1. Master basis (read-only check, `Tools/master-search.sh`, 2026-10-31)

| # | Finding | Status as read | Source |
|---|---|---|---|
| P1 | "one player-created protagonist"; male and female fully viable; extensive Character Creator (face sculpting, body, hair, eyes, skin, scars, tattoos …) | CANONICAL DELTA (tier 3, 2026-08-31) | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/DECISION_CHANGELOG_2026-08-31.md`, section 1 |
| P2 | Photorealistic humans; deep modular creator for player and NPCs | reported developer direction (tier 2) | `99_DAILY_DELTAS/2026-10-18/.../CHARACTER_CREATOR_AND_RELEASE_POLICY.md` L5–6 (see also `Docs/Decisions/0005`, A1/A2/A4) |
| P3 | Tier 1 (current canonical state) | no hit for protagonist appearance / MetaHuman | — |

**OPEN (developer decision):** P1 describes a player-created protagonist, while this task asks for one fixed male "Protagonist V1"
built from an accepted reference image. This work treats V1 as a *default/reference look* that a future character creator can
start from; it does not lock a fixed hero, sex or face. Please confirm or correct that reading.

## 2. What exists (Unreal content, `.uasset` is git-ignored)

| Asset | Content |
|---|---|
| `/Game/Characters/ProtagonistV1/MHC_ProtagonistV1` | MetaHuman Character. Start: copy of Epic preset **Victor** (closest male preset in geometry, see §4), then face fit, skin, eyes, body, grooms |
| `/Game/Characters/ProtagonistV1/Reference/T_ProtagonistV1_Reference`, `M_ProtagonistV1_ReferenceBoard` | imported copy of the accepted reference image + unlit board material |
| `/Game/Characters/ProtagonistV1/Comparison/LV_ProtagonistV1_Compare` | PLACEHOLDER comparison level: lights, reference board, cine cameras `Cam_ReferenceMatch` (fitted camera), `Cam_Front`, `Cam_ThreeQuarter`, `Cam_Side` |
| `/Game/Characters/ProtagonistV1/_Scratch/…` | experiments (preset copies, probe level); may be deleted |

Engine plugins enabled in `SupernaturalProt.uproject` (approved 2026-10-31): `PythonScriptPlugin` (editor only), `MetaHumanCharacter`.
Not enabled: experimental `MetaHumanGenerator` toolset (only body/skin/eye-colour wrappers; same calls are made directly).

## 3. Tools (`Unreal/SupernaturalProt/Tools/MetaHuman/`)

| File | Purpose |
|---|---|
| `spn_mh_fit_math.py` + `test_spn_mh_fit_math.py` | camera model, LM camera fit, contour offsets, symmetric deltas — pure Python, 11 unit tests |
| `spn_mh_editor.py` | editor helpers (open for edit, preview actor, lights, SceneCapture, contour tracker) |
| `spn_mh_fit_face.py` | iterative face fit to the reference (see §4) |
| `spn_mh_apply_appearance.py` + `protagonist_v1_appearance.json` | skin, eyes, body constraints, grooms in exchangeable slots |
| `spn_mh_capture_views.py` | comparison renders → `Unreal/SupernaturalProt/Saved/ProtagonistV1/` |
| `spn_mh_setup_compare.py` | reference import + comparison level |
| `protagonist_v1_reference_annotations.json` | hand-placed 2D landmark targets on the reference (±5 px, low-confidence points marked) |
| `spn_remote_exec.py` | runs a script in a running editor (remote execution enabled via command line only) |

Run order (editor started with `-ini:EditorPerProjectUserSettings:[/Script/PythonScriptPlugin.PythonScriptUserSettings]:bRemoteExecution=True`):
```
python spn_remote_exec.py file spn_mh_fit_face.py /Game/Characters/ProtagonistV1/MHC_ProtagonistV1 <reference.png> protagonist_v1_reference_annotations.json 6 0.7 20 fit
python spn_remote_exec.py file spn_mh_apply_appearance.py /Game/Characters/ProtagonistV1/MHC_ProtagonistV1 protagonist_v1_appearance.json
python spn_remote_exec.py file spn_mh_capture_views.py spawn /Game/Characters/ProtagonistV1/MHC_ProtagonistV1
python spn_remote_exec.py file spn_mh_capture_views.py refresh      # after the groom bindings finished (minutes)
python spn_remote_exec.py file spn_mh_capture_views.py capture v1 <Saved>/ProtagonistV1/fit_fit.json
python spn_remote_exec.py file spn_mh_setup_compare.py <reference.png> <Saved>/ProtagonistV1/fit_fit.json
```
The project copies of the scripts were consolidated from the session's working versions. Actually run from the project copies:
`spn_mh_setup_compare.py` (built the comparison level) and `spn_mh_capture_views.py spawn`. The face fit and the appearance were
produced by the working versions (same algorithm); `spn_mh_fit_face.py` / `spn_mh_apply_appearance.py` / `capture` from the
project copies have **not** been run yet.

## 4. Face: what the API allows and what was done

**No photo-to-MetaHuman in UE 5.8.4.** Documented face inputs: DNA file, template mesh, MetaHuman Identity, custom/target mesh
conform (`conform_to_target_meshes`, can use 2D face tracking of a *rendered portrait of that mesh* as extra term), landmark
sculpting (`translate_face_landmarks`, 79 landmarks), raw model coefficients (1397 undocumented floats). A Mesh-to-MetaHuman
path needs a 3D head mesh of the person; none exists for the reference (would need photogrammetry, a 3D artist or an external
image-to-3D tool — not used, needs approval).

Automated fit (`spn_mh_fit_face.py`):
1. Preset choice: camera fit of all 29 presets' landmarks to the reference targets. Differences between usable presets were
   within annotation noise (7.1–8.5 px RMS); 8 presets returned unusable landmark data. Victor was picked visually from the
   male candidates rendered with the fitted camera (long narrow face, straight nose).
2. Camera fit: about 29° turn to the character's right, camera slightly below eye level; FOV fixed to 20° (portrait lens).
3. Landmark fit: annotation RMS 7.5 → ~4.9 px, contour RMS (eyes, lips, nasolabial) 4.9 → ~3.7 px on a 1536×1024 image; then
   noise-limited. Best iteration kept automatically.

**Limits (honest):** one 3/4 view, stylised image, hair over forehead/ear. Depth is not observable: eye depth (deep-set eyes),
brow ridge, cheekbone and jaw projection, nose profile, ear shape. These need manual sculpting in MetaHuman Creator (§6).

## 5. Comparison renders

`Unreal/SupernaturalProt/Saved/ProtagonistV1/` (git-ignored): `v5_sheet.png` (reference vs. V1 in the fitted camera),
`v5_views_sheet.png` (front / 3-4 / side), single views `v5_*.png`, `cand_sheet.png` (preset candidates), `ref_annot.png`
(annotations), `compare_level_overview.png`, `fit_main20.json` (camera + fit log), `coeffs_main20_best.json`.

**Assessment (stop rule): the face still differs clearly from the reference** — front view rounder and softer, eyes not
deep-set, fuller lips, weaker brow ridge and jaw. V1 is therefore NOT presented as finished; §6 lists the corrections. Known render issue: the preview actor's **hair strands were missing in most SceneCapture renders** (beard and
eyebrows render); they appeared once after the groom components were re-initialised (§7). Judge the face on the renders and the
hair in MetaHuman Creator's own viewport.

## 6. Manual face check list (MetaHuman Creator → Face → Sculpt / Move, with `LV_ProtagonistV1_Compare` reference board)

Observed differences reference ↔ V1 (to be confirmed by the developer):
- eyes: reference deeper-set and narrower with heavier upper-lid shadow → push eye region back, lower upper lid slightly
- brow ridge: reference stronger, brows straight and low → raise brow-ridge volume, lower brow line
- cheeks: reference leaner with visible cheekbones → reduce cheek volume, bring cheekbone forward
- jaw/chin: reference squarer, slightly wider jaw → widen jaw angle, square chin
- nose: reference long and straight with a defined tip → check profile in side view
- skin: reference weathered, sun-tanned, light scars/redness → texture variant (`face_texture_index`), roughness; no tattoos
- hair: reference medium-long, messy, dark brown, swept back → current `Hair_M_BobMessy` is only an approximation; candidates
  `Hair_L_MessyClumps`, `Hair_M_Layered`; a matching groom does not exist in the free set (**missing look**)
- clothing: only `WI_DefaultGarment` is available; jacket, flannel shirt, straps, pendant are **missing looks**

## 7. Known issues / not done

- Hair strands in SceneCapture: missing unless the bindings finished and the groom components were re-initialised; even then not
  reliable. Not fixed. Next option: Movie Render Queue (would need the MovieRenderPipeline plugin — not enabled, needs approval).
- Viewport screenshots (`HighResShot`, `AutomationLibrary.take_high_res_screenshot`) produced no file while the editor was not
  the foreground window.
- Phase 3 (rig via cloud auto-rigging, texture download, assembly, test map, movement integration) **not started** — waits for
  visual approval and the Epic-account login.
- No change to the existing player character, movement, maps or other assets. No commits.
