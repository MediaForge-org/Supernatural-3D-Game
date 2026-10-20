# 0005 — Character appearance foundation (Phase 0.4, revised) — ENGINEERING PROPOSAL

Status: implementation proposal for developer review. **Not master canon.** It fixes no
world size, no map, no art pipeline and no gameplay value. The first version of this phase
was reviewed against the requirements below and extended; section 3 lists what the review
found.

## 1. Requirements, separated by status

### CONFIRMED by the developer (requirement level)
Source: `Docs/Proposals/0003` section 1 (developer statement of 2026-10-18) and the
developer's decisions of the planning phase. These are *requirements*, not designs.
- Humans as photorealistic as possible (long-term), modular NPC assets.
- An extensive character editor for the player **and** NPCs: faces, body proportions, skin,
  hair, clothing, tattoos, further appearance traits. *Not implemented now.*
- Individually looking NPCs.
- NPC detail and simulation levels that depend on distance.
- Third-party / licence-restricted content is kept separate from own content.
- Long-term open source; no third-party copyrighted material in the public repo.
- World size is not fixed (about 25 candidate sizes); future systems must be size-independent.

### In the master (read-only check of `Planning/Master`, corrected 2026-10-20)
- `01_CURRENT_CANONICAL_STATE/` (tier 1, including the addenda of 2026-10-15 and 2026-10-18)
  contains no character-editor, appearance, photorealism or LOD requirement. The
  requirements are in a review-only daily delta (tier 2) and in older reference documents
  (tier 3); see the **Master basis** table below. An earlier version of this document said
  the master has no such requirement; that was wrong and is corrected here. No design
  decision of this document changes because of the correction.
- Modding (`01_CURRENT_CANONICAL_STATE/MODDING/`): `.spnmod`, provider-independent,
  project-owned manifest/registry; **mods can be enabled/disabled per save; disabled mods
  and their state stay preserved; critical missing mods block save loading by default, with
  an expert override after backup** (reconstructed state 2026-09-09, block 5 of 2026-09-11).
  → consequences for appearance data are in section 4.4.
- Accepted earlier: realistic injury, clothing/inventory, many individually authored NPCs,
  companions (Blocks 9/10, as summarised in 0003).

#### Master basis (paths relative to `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/`)
`PD` = `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md`
(tier 3, dated 2026-08-31); `CCR` = `99_DAILY_DELTAS/2026-10-18/SPN_DAILY_DELTA_2026-10-18/docs/design/CHARACTER_CREATOR_AND_RELEASE_POLICY.md`
(tier 2). Lines as read on 2026-10-20. The delta is a review package, so its
"user-confirmed" statements are **reported developer decisions that are not yet part of the
tier 1 canon**, and the delta itself says they are a design goal, not evidence that assets or
systems exist.

| # | Requirement (paraphrased) | Status as read | Source (path, section, line) | Used in this document |
|---|---|---|---|---|
| A1 | Human characters as photorealistic as reasonably achievable within evolving production limits | user-confirmed direction (reported, tier 2) | `CCR`, "User-confirmed long-term direction", L5 | long-term goal; no rendering decision (section 1, 4.5) |
| A2 | Deep modular character creator for player characters and generated NPCs: faces, body types and proportions, hair, clothing, appearance details | user-confirmed direction (reported, tier 2); a design goal | `CCR` L6 | parameter groups, edits, editor foundation |
| A3 | Scalable NPC fidelity/performance; staged implementation; not every endgame customization needed for Phase 0 | user-confirmed direction (reported, tier 2) | `CCR` L7–8 | detail tiers, "no editor UI now" |
| A4 | One deeply customizable player-created protagonist; male and female both fully viable without story/content locks | CONFIRMED ("preferred current model", tier 3) | `PD`, "Player-created protagonist, Hunter crew and HQ", L618 | no sex-specific restriction in the model; representation choices stay OPEN |
| A5 | Creator inventory: face/body customization, cosmetic anatomy separate from gameplay power, save/versioning rules for creator parameters, regression tests for extreme valid body combinations | planned direction; file status "PLANNING INVENTORY", work packages not yet written | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/planning-backlog/33-custom-protagonist-character-creator.md`, "Canonical direction", L4–L17 | versioned formats, validation; gameplay effects of the body are not modelled |
| A6 | Not every NPC needs full simulation; important NPCs can be full Actors, background/distant populations lighter | CONFIRMED (tier 3) | `PD`, "NPCs and AI", L73 | simulation tier is the NPC system's job, not appearance's (section 2) |
| A7 | Asset LOD/Nanite suitability is part of the DCC/asset specification | CONFIRMED (tier 3), asset level | `PD`, "AI-assisted Blender / scene production", L555; "Prompt library, Scrum and implementation orchestration", L394 | LOD of meshes is the renderer's concern; `DetailTierTable` only supplies distance thresholds |
| A8 | Important characters get authored depth; generic NPCs may use systemic content | CONFIRMED (tier 1), about dialogue and relationship depth; says nothing about looks | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md`, "Block 10 — social/life sandbox accepted direction", L42 | context only; individual looks of NPCs stay a developer requirement from 0003 |
| A9 | Eventual open source of code and planning master only after a per-file rights audit; third-party assets and generated media are not presumed redistributable | reported intent, no license decided (tier 2); same rule in `CLAUDE.md` §9 | `CCR`, "Open-source and project-data handling", L12–15 | separation of third-party content (section 1) |
| A10 | Data-driven character definitions and LOD tiers; stable identity/appearance serialization before expanding customization | PROPOSAL (labelled "proposals only") | `CCR`, "Suggested architecture notes (proposals only)", L19–21 | consistent with this document; not used as a requirement |
| A11 | Candidate data boundaries and a testable first slice for appearance; undecided: morph count, rig topology, mesh generation technique, photoreal targets by distance, collision, editor UI, NPC budget, likeness/consent constraints, licensing | PROPOSAL (review-only, "not a locked schema") | `99_DAILY_DELTAS/2026-10-19/SPN_DAILY_DELTA_2026-10-19/docs/design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md`, "Candidate modular data boundaries", "Testable first slice", "Not yet decided" | consistent; the undecided items stay in section 6 |

Not found in the master: a specification of parameters, morph counts, distance numbers or
LOD tier counts (none is decided here either), and any statement that NPCs must look
individually (that requirement comes from the developer statement in 0003).

### ENGINEERING PROPOSALS (this document; all changeable)
Everything under "What exists" below: id scheme, fixed-point parameters, catalog, provider
compatibility, recipes, edits, tier table, formats, limits, parameter names and ranges.

### OPEN (nothing decided) — see section 6.

## 2. Layering (separation of concerns)
```
Unreal rendering adapter (later)   reads AppearanceSpec; maps ids/params to meshes/materials;
        ^                          decides what a detail tier means visually
Appearance (Core)                  spec, catalog, validation, edits, recipes, generator, formats
        ^ (spec or recipe only)
NPC simulation (later)             holds an AppearanceSource per character; decides what a
                                   simulation tier means (full AI / routine / statistical)
```
Appearance code includes neither `WorldFacts` nor anything engine-related (checked by grep),
and knows nothing about NPC behaviour. Runtime looks (fresh wounds, dirt from play) are
*state*, not spec; they become layers from injury/world state later (OPEN).

## 3. Review of the first Phase 0.4 version → fixes
| # | Finding | Requirement affected | Fix |
|---|---|---|---|
| 1 | No way to store "seed + deliberate deviations" | individually looking NPCs, cheap crowds (0003 guardrail 2) | `AppearanceRecipe` + `AppearanceOverrides`, `ResolveRecipe`, `AppearanceSource` (spec **or** recipe) with its own canonical format |
| 2 | Missing/disabled mod content would make a save unusable or would have to be deleted | canon: per-save mods, preserved state | `MakeAvailableAppearance` (non-destructive view + list of omitted parts), `ReferencedNamespaces` (what mods a character needs) |
| 3 | Content could not say which body it fits; with several body providers (MetaHuman vs modular) the generator/validator could combine incompatible parts | pipeline-agnostic providers, modular assets | `providers` lists on parameter and content definitions (empty = all); enforced by validation, generator, edits, availability view; `ValidateConsistency` for dangling references |
| 4 | Every parameter was drawn from one bell curve (freckles, grey hair as common as anything) | individually (realistically) looking NPCs | `GenerationShape` per parameter (`Bell`, `Uniform`, `TowardDefault`) as catalog data |
| 5 | Loader/catalog accepted unbounded numbers of parameters/layers | robustness for mods and saves | `kMaxAppearanceParameters` (512), `kMaxAppearanceLayers` (32) enforced in validate, load, catalog |
| 6 | Nothing existed that a character editor could be built on | character editor | `AppearanceEdit` (10 edit types), `ApplyEdit` (atomic, fully validated), `AppearanceEditor` with undo/redo |
| 7 | Distance-dependent detail had no data representation | confirmed LOD requirement | `DetailTierTable` (validated thresholds in integer cm; two instances: visual and simulation) |
| 8 | Duplicate text-parsing helpers | maintainability | internal `Core/src/TextParsing.h` |
| 9 | Generator output depended on a catalog the save did not remember | reproducibility, mods | recipe stores generator version + catalog fingerprint; `FingerprintPolicy` (`Require` default) |

## 4. What exists

### 4.1 Building blocks
| Part | Files | Notes |
|---|---|---|
| `ContentId` | `ContentId.h/.cpp` | `namespace:dotted.path`, segments `[a-z][a-z0-9_]*`, ≤ 96 chars. Readable, stable, no central allocator: a mod uses its own namespace. Empty id = "none". |
| `AppearanceSpec` | `Appearance.h` | schema version, body provider, hair, facial hair, parameters, clothing per slot (map ⇒ at most one item per slot), ordered layers. |
| `AppearanceCatalog` | `Appearance.h/.cpp` | parameter and content definitions; duplicates rejected; `Fingerprint()`; `ValidateConsistency()`. |
| Validation | `ValidateAppearance` | reports **all** problems in deterministic order with code + subject. |
| Spec format | `SerializeAppearance` / `DeserializeAppearance` | `SPNAPPEARANCE 1`, strict canonical-only. |
| Generator | `AppearanceGenerator.h/.cpp` | `GenerateAppearance(catalog, seed, options)`. |
| Recipes | `AppearanceRecipe.h/.cpp` | recipe, overrides, resolve, `SPNRECIPE 1`, `DeserializeSource`. |
| Edits | `AppearanceEdit.h/.cpp` | edit variants, `ApplyEdit`, `AppearanceEditor`. |
| Detail tiers | `DetailTiers.h/.cpp` | `DetailTierTable::Create`, `TierFor(distanceCm)`. |
| Random | `DeterministicRandom.h` | SplitMix64, `MakeStream`, `Fnv1a64`; no `std::` distributions. |

### 4.2 Data conventions (proposals)
- **Parameters are fixed-point `int32`**, never floats: exact serialization, identical on
  every compiler. A unit's physical meaning belongs to the rendering adapter. Built-in set:
  morph-like parameters bipolar −1000..1000 (default 0), amount/colour unipolar 0..1000.
  The 29 built-in parameters (face 12, body 6, skin 4, eyes 3, hair 4) are **placeholders**.
- **Parameter definitions are data** (`ParameterDef`: group, min, max, default, generation
  shape, providers). Absent parameter in a spec = catalog default.
- **Body provider is just a content id** — MetaHuman, a Blender modular body or a custom
  body are different providers behind the same spec (0003 guardrail 3).
- **Provider compatibility** is declared on the part (`providers` list; empty = fits all), so
  a mod adding clothing for one body just lists that body.
- **Layers** (tattoo, scar, birthmark, makeup, dirt) are an ordered list = stacking order;
  the same asset may repeat only in different regions. Wounds are runtime state, not spec.

### 4.3 Formats (strict, canonical-only, like WorldFacts; detail in code comments and tests)
```
SPNAPPEARANCE 1                       SPNRECIPE 1
body<TAB><id>                         seed<TAB><uint64>
hair<TAB><id|->                       generator<TAB><uint32>
facial_hair<TAB><id|->                catalog<TAB><uint64 fingerprint|->
param<TAB><id><TAB><int32>  *         options<TAB>bald<TAB>facial<TAB>tattoo<TAB>scar<TAB>maxTattoos<TAB>maxScars
clothing<TAB><slot><TAB><id> *        [body<TAB><id>] [hair<TAB><id|->] [facial_hair<TAB><id|->]
layer<TAB><kind><TAB><id>             param<TAB><id><TAB><int32> *      (ascending)
     <TAB><region><TAB><opacity> *    clothing<TAB><slot><TAB><id|-> *  (ascending; "-" = remove)
end<TAB><n>                           [layers<TAB><n> followed by n layer lines]
                                      end<TAB><n>
```
`*` = repeated. `n` = lines between header and `end`. Integers canonical decimal, one
spelling per token, fixed line order, no unknown lines, entry counts bounded. **Loading is
syntax-only; validation against a catalog is a separate step**, so "corrupt file" and
"missing mod" stay distinguishable. Status: **provisional** — no saves exist yet, so formats
may still change without migration; freeze each with a golden file (as in doc 0004) the
moment the first real save uses it. (Today the formats are pinned by inline golden texts in
the tests.)

### 4.4 Behaviour tied to the confirmed mod rules
- Loading never drops references to unavailable content, so a disabled mod's data stays in
  the save and returns when the mod is enabled again.
- `ReferencedNamespaces(spec)` lets the mod layer decide which mods a character needs; the
  *policy* (what is "critical", blocking vs. expert override) is outside this module.
- `MakeAvailableAppearance` produces what can be shown **now** plus the list of omitted
  parts. It fails only if the body provider is unusable, because choosing a fallback body is
  a policy decision.
- A recipe whose catalog changed fails with `CatalogChanged` instead of silently producing a
  different person; `FingerprintPolicy::Ignore` is the explicit opt-out.

### 4.5 Generation contract
`GenerateAppearance(catalog, seed, options)` is a pure function of
(seed, `kAppearanceGeneratorVersion`=1, options, catalog content).
- Body provider is drawn first; only parameters and content that fit it are generated.
- Each parameter has its own stream keyed by its id: **adding parameters never changes
  existing values** (tested, also with provider-restricted parameters).
- Content choices index into the id-sorted candidates, so they **do change when matching
  content is added or removed** — hence the recipe's fingerprint check.
- Shapes: `Bell` = mean of three uniform draws; `Uniform`; `TowardDefault` = cubic skew from
  the default towards the far end (rare). All integer arithmetic.
- Golden outputs for two seeds are byte-identical on GCC and Clang, with and without
  sanitizers. MSVC/Windows is not yet tested.
- Version 1 was revised during this review (shapes, provider filtering) because no consumer
  existed yet. From the first use in saved data on, any change requires a new version.

### 4.6 Editor foundation (no UI)
`ApplyEdit(spec, catalog, edit)` returns the new spec or the first validation problem;
nothing changes on error. Edits: set/reset parameter, body provider, hair, facial hair,
clothing slot, add/remove/move layer, layer opacity. `AppearanceEditor` adds undo/redo (copies
of small specs, capped at 256, no-op edits recorded nowhere). Changing the body provider fails
while incompatible parts remain (silent dropping would lose user work; a "convert" policy is
OPEN). Everything a UI needs for sliders is in the catalog (`Parameters()`: group, range,
default); labels/localisation are not Core's job.

### 4.7 Detail tiers
`DetailTierTable` maps a distance (integer cm, independent of world size) to a tier index with
inclusive upper bounds; the last tier is unbounded; at most 16 tiers. It carries **no
numbers of its own**: tier counts and distances come from configuration (undecided). Visual and
simulation detail each get an instance. No hysteresis yet (flicker at tier borders is an
adapter-side concern to settle with the first real use).

## 5. Limits (proposals; constants in code)
ids ≤ 96 chars (namespace ≤ 32); layers ≤ 32; parameters ≤ 512; opacity 0..1000; generation
chances 0..1000 per mille; tiers ≤ 16; editor history 256.

## 6. OPEN decisions (need the developer; nothing below is decided)
1. **Real parameter set**: names, ranges, physical meaning — depends on the chosen body
   provider (MetaHuman vs. Blender modular spikes, 0003 step 10).
2. **Age, sex/gender presentation, ethnicity-like variation**: deliberately *not* fields.
   Whether they are parameters, provider choices or generation profiles is undecided; needs
   care (stereotyping, canon for specific characters). `skin.age_wear` etc. are not tied to age.
3. **Generation profiles** (per population/role distributions, correlations such as age ↔
   grey hair): only per-parameter shapes exist.
4. **Authored vs. generic NPC policy**: which characters are specs, which recipes; when a
   generic NPC is "promoted" to an authored one (store resolved spec).
5. **Catalog drift policy** for recipes (fail vs. regenerate vs. store spec) — currently
   fail-by-default, configurable per call.
6. **Mod rules**: whether a mod may override a base definition (duplicates are errors now);
   enforcement that a mod only defines ids in its own namespace (belongs to the manifest
   design); `.spnmod` manifest itself.
7. **Fallback body provider** when the stored one is unavailable (policy of the caller).
8. **Body-provider change in the editor**: fail (now) vs. convert/drop incompatible parts.
9. **Wounds / injury / dirt**: how runtime state becomes layers; whether `LayerKind` needs a
   `Wound`; link to WorldFacts and injury canon.
10. **LOD**: tier counts, distances, meaning of each tier, hysteresis, per-LOD content in the
    spec (deliberately none today).
11. **Not modelled yet**: eyebrows/lashes, teeth, nails, body hair, per-item clothing variants
    (colour, wear), expression/pose, voice.
12. **Co-op**: whether the host sends recipes or specs (network design is OPEN, CLAUDE.md §6).
13. **Format freeze**: first real save ⇒ golden files for `SPNAPPEARANCE`/`SPNRECIPE`.
14. **Open-source/licensing**: only own, generic placeholder ids exist in the repo; no
    third-party asset names or data. Third-party character content stays out (0003 D4).

## 7. Tests
`ContentIdTests`, `AppearanceTests` (catalog, one test per validation error, provider
compatibility, availability view, namespaces, formats incl. golden/round trip/every prefix/
single-byte corruption/limits), `AppearanceGeneratorTests` (RNG reference vector, determinism,
validity of 1500 seeds, variety, distribution shapes, extension stability, provider filtering,
options, errors, frozen goldens), `AppearanceRecipeTests` (resolve, overrides, version and
catalog changes, golden/round trip, strict loader, size advantage), `AppearanceEditTests` (all
edits, atomicity, provider rules, editor history), `DetailTierTests`.

## 8. Risks
- Placeholder parameter names/shapes may harden before a real provider exists.
- Fixed-point scale convention must be repeated in every adapter; untested against a renderer.
- Seed-generated content choices depend on catalog contents (mitigated by the fingerprint).
- Generated NPCs are plausible-looking *data*, not validated for realism or diversity; shapes
  are untuned and the generator makes no demographic claims.
- Hash-based stream seeds (FNV-1a) are not cryptographic; irrelevant here.
- Windows/MSVC untested (integer-only code, expected to match).
