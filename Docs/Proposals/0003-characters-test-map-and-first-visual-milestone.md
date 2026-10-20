# 0003 — Character requirements, WorldFacts review, test map and first visual milestone

Status: **PLANNING PROPOSAL / ENGINEERING PROPOSAL**, written 2026-10-18 for developer
review. It is not master canon and does not edit anything under `Planning/Master`.
Labels: CONFIRMED (stated by the developer), UNVERIFIED (not checked against a primary
source), PROPOSAL (my suggestion), OPEN (needs a decision).

## 1. Character requirements (new, CONFIRMED by developer on 2026-10-18)

Wording as given: as photorealistic as possible humans; an extensive character editor
for player and NPCs (faces, body proportions, skin, hair, clothing, tattoos, further
appearance traits); individually looking NPCs; NPC detail and simulation levels that
depend on distance. A full editor is **not** being implemented now.

Canon check (read-only): the master has no explicit character-editor or photorealism
requirement. The only related text is a proposal in the 2026-10-14 daily delta
(`POPULATION_AND_SPATIAL_SCENARIOS.md`, item 5): specify NPC simulation LOD and crowd
budgets independently of the lore population. The requirement above is therefore
**new input**, to be recorded in a separate canon addendum when the developer chooses.
Related canon already accepted (Block 9/10): realistic injury, clothing/inventory,
many individually authored NPCs, companions.

### Architectural guardrails (PROPOSAL; none of this is built yet)
1. **Appearance is data, not a mesh.** An engine-independent `AppearanceSpec`
   (parameter values + references to parts by stable ids) that lives in `Core`, is
   serializable, versioned and testable on Fedora. Rendering stays in Unreal adapters.
2. **Seeds for crowds.** A generic NPC stores `seed + overrides`, not a full parameter
   set. Deterministic generation keeps saves small and makes co-op sync cheap. Authored
   characters store full specs.
3. **Pipeline-agnostic.** The spec must not assume MetaHuman. A MetaHuman, a Blender
   based or a custom modular body is just a different "body provider" behind it.
4. **Two LOD axes, defined in data:** *visual LOD* (mesh, hair cards vs. strands,
   skin shader cost, clothing physics) and *simulation LOD* (full AI, simplified
   routine, statistical/off-screen). Tier thresholds are configuration, not constants.
5. **Stable ids for parts** (`StrongId` tags) so mods (`.spnmod`, canon baseline) can
   add parts and saves can reference them.
6. **Layered skin state** (tattoos, wounds, dirt) as an ordered list of decal/material
   layers on top of the base skin; it ties into the injury canon and WorldFacts.
7. **Third-party assets stay out of the public repo** (see section 4): the repo holds
   code, own data and a manifest; licensed content is fetched locally.

## 2. Review of the current C++ state

Project: `Core` (`StrongId`, `Result`, `WorldFacts`), 45 doctest cases, four presets
(Clang/GCC, with and without ASan+UBSan) all green on Fedora; no Unreal project yet.
Repo is local only, nothing committed. `graphify-out/` appeared in the worktree during
this session (not created by me; already listed in `.gitignore`).

### WorldFacts risks (measured with a throw-away benchmark outside the repo, -O2 Clang)

| Journal size | Apply total | Save size | Load (parse + replay + verify) | Peak RSS (save + load in one process) |
|---|---|---|---|---|
| 100 000 | 123 ms | 5.8 MB | 137 ms | 83 MB |
| 1 000 000 | 1.17 s | 60.6 MB | 1.47 s | 579 MB |

About 1.2 µs per command, ~60 bytes per saved record, roughly 250 bytes of RAM per
record while loaded. These are measurements on one machine with 1 000 keys and tiny
values, not a game workload.

1. **Journal growth** (medium). Fine for hours of play at normal rates, but unbounded
   and linear in load time. A busy simulation writing facts per tick would reach 1M
   records quickly. Mitigations: (a) rule that high-frequency/derived state does not go
   through WorldFacts; (b) snapshot + journal-tail format (v2) with compaction; (c) a
   size alarm. Compaction conflicts with "reconstruct history from change events"
   (canon 09-05), so *which* history is kept is an OPEN design question.
2. **Save compatibility** (high). Loading replays commands through today's validation
   (key rule, 128/4096/128 limits, type-fixing rule). Tightening any rule makes old
   saves unloadable, and unknown future ops are rejected by old loaders. Only the
   header version is checked; there is no migration path. Mitigations: commit a golden
   v1 save as a fixture test; pin validation rules per format version; add a
   `Migrate(vN -> vN+1)` step before replay; treat limit changes as format changes.
3. **Replay** (low/medium). Replay is deterministic (no time, no randomness, ordered
   containers). Missing: "state at sequence N" (needed for debugging, undo-style tools,
   historical-world-state ideas) — a prefix replay is a small addition. Replay assumes
   command semantics never change.
4. **Co-op and origin** (OPEN, by design). No actor/origin, no compare-and-set, no
   conflict rules; adding fields later is a format change (see 2).
5. **Observers.** Caseboards/journals/UI will need change notifications; today they
   would poll. Additive, low risk.
6. **Threading.** Not thread-safe; Unreal game thread vs. async loading needs a rule.
7. **Unreal adapter cost.** `std::string`/`std::variant` need conversion to
   `FString`/`FName`; `Result::Value()` on error would abort without exceptions
   (documented precondition).
8. **UTF-8** is not validated; Unreal text is UTF-16 internally, so invalid input needs
   a defined policy at the adapter.

Recommended hardening, small and before building on it: golden v1 fixture + compat
policy, prefix replay, record the "what goes through WorldFacts" rule.

## 3. First small playable 3D test map (Unreal) — PROPOSAL

Scope: a clearly labeled **placeholder** level (`SPN_TestMap_Placeholder`), not a
world-map candidate and unrelated to WORLD_700 or any larger-world geometry.

- Project: Unreal C++ project under `Unreal/SPNGame/` with one gameplay module; start
  from the engine's Third Person template, then strip what is not needed.
- Level: flat base plus a few slopes, a staircase and a ramp, a low wall, one trigger
  box ("door") — roughly 100 m × 100 m. Size and shapes are throw-away test values.
- Character: engine mannequin as placeholder (named/labeled as such).
- Controls: move, look, jump, sprint. Camera: third person with collision.
- Optional M1.5 (proves Core integration): the trigger box applies a `SetFact`
  command to a `WorldFacts` instance and a debug text shows key, value and revision.
- Core integration into Unreal builds is the highest technical risk. Options to
  evaluate in a Windows spike (decision after the spike, not now):
  (a) build `spn_core` with CMake/MSVC as a prebuilt library consumed as third-party
  module (keeps Core engine-independent and tested); (b) compile Core sources inside an
  Unreal module via directory junction/copy step (simplest, source duplication risk).
  Watch-outs: Unreal builds without exceptions/RTTI by default and uses its own
  toolchain settings; keep Core headers free of both (already so).
- Version control: `.gitignore` currently ignores `*.uasset`/`*.umap`, so Unreal
  Content cannot be versioned yet. Needs a content policy (section 7, D2).

## 4. Workflows for photorealistic humans

Licensing facts below come from secondary sources unless noted; **the primary source
(Epic MetaHuman EULA page) could not be fetched during this session (HTTP 403)**, so
everything about MetaHuman terms is UNVERIFIED and must be read by the developer.

| Option | Realism / effort | Fit with Unreal | Licence and open-source aspects |
|---|---|---|---|
| **MetaHuman** (Epic) | Highest realism for the least art effort; runtime customization is limited to what the rig/groom/material parameters allow | Native, editor-integrated since UE 5.6 (reported) | Reported change in June 2025: covered by the Unreal Engine EULA (free below US$1M revenue), characters usable in other engines and sellable. Older terms restricted end-user character creators and redistribution. **Assumption until verified: MetaHuman assets must not be committed to a public repo.** A shipped game can still use them; the open-source repo would carry only a manifest and local-fetch instructions. |
| **Blender-based own pipeline** (e.g. parametric base meshes such as MPFB/MakeHuman-derived, own sculpt/retopo, shape keys, texture painting) | Realism depends on art skill/time; large effort for faces; fully controllable | Export FBX/glTF to Unreal; morph targets, skeletal mesh, material layers | Blender itself is GPL but its output is yours. Check each add-on/base-mesh licence (MakeHuman-derived code is AGPL; check the licence of exported meshes/assets and textures). Best open-source story because everything can be published. |
| **Modular character assets** (own or licensed parts: bodies, heads, hair, clothing, as swappable skeletal meshes with shared skeleton, morph targets and layered materials) | Scales to many NPCs; realism comes from the part quality | Good: shared skeleton, master-material parameters, `AppearanceSpec` maps onto it directly | Own parts: publishable. Marketplace/Fab parts: licences normally forbid redistribution in source form → keep them out of the public repo and use a manifest. |
| Commercial creators (e.g. Reallusion Character Creator) | Strong results, extra toolchain | Import pipeline | Proprietary, redistribution restrictions; not a good match for the open-source goal unless limited to non-published local content. |

PROPOSAL: do not decide the human pipeline now. Build the milestone with the engine
mannequin, keep `AppearanceSpec` pipeline-agnostic, and run two short spikes after
M1 — a MetaHuman spike on Windows and a Blender/modular spike (works on Fedora) — then
compare realism per hour of work, runtime-customization freedom, memory per NPC at LOD
tiers, and licence/open-source consequences. Hair strands, skin shading and facial
rigs are the expensive parts; budget distance tiers from the start (RTX 3090, 24 GB
is ample for a few hero characters but not for crowds at top quality).

Unreal EULA vs. open source (UNVERIFIED, general understanding): our own project code
can be public; engine source and Epic content cannot be redistributed by us. Have the
EULA checked before the publication audit.

## 5. First visual milestone M1 — "Third-person character on walkable terrain"

Definition of done (all measurable):
1. Project opens on the Windows PC and starts in the editor (PIE) without errors.
2. Placeholder character walks, runs, jumps over the test geometry (slope, stairs,
   ramp, wall) with working collision and a collision-safe third-person camera.
3. Level and character are labeled placeholder; no map is presented as final.
4. A standalone (non-editor) build starts and is playable.
5. Frame time measured and noted (no target locked; just the number on the 3090).
6. The Unreal source in the repo builds from a clean checkout following a written
   `Docs/` setup note.
Out of scope: character editor, MetaHuman, NPCs, vehicles, terrain streaming,
photorealism.

## 6. What needs Fedora vs. the Windows PC (RTX 3090)

| Work item | Fedora laptop | Windows PC |
|---|---|---|
| Core C++ (WorldFacts, StrongId, future AppearanceSpec), tests, sanitizers, lint | Yes (primary) | MSVC build/test check later |
| CMake, scripts, docs, planning proposals | Yes | – |
| Blender modelling, shape keys, texture work | Yes (RAM-limited for heavy sculpts) | Yes (better) |
| Writing Unreal C++ source text | Possible, but **cannot be compiled or tested** here | Compile and run |
| Unreal Editor, Content, level building | **No**: Fedora is not an officially supported host (Epic recommends Ubuntu 22.04 and 32 GB RAM), laptop has 16 GB RAM and integrated graphics | Yes |
| Packaging, performance, rendering, MetaHuman, skin/hair | No | Yes |
| Visual Studio / MSVC toolchain | – | Yes |

Open mechanics: how files move between machines (section 7, D3). I cannot reach the
Windows PC from here; every Windows step is run by the developer, with me preparing
checklists and reviewing results.

## 7. Prioritized plan with rough effort (at 8–15 h/week)

Estimates are order-of-magnitude, not commitments; Unreal integration has the widest
uncertainty.

| # | Step | Where | Effort | Depends on |
|---|---|---|---|---|
| 1 | Developer commits current state; decisions D1–D3 | Fedora | 1–2 h | – |
| 2 | WorldFacts hardening: golden v1 fixture, compat policy, prefix replay | Fedora | 6–10 h | – |
| 3 | Windows readiness check (OS, GPU driver, free disk, Visual Studio, Epic launcher) | Windows | 1–3 h | – |
| 4 | Install Unreal (version per D1), empty C++ project builds, Third Person template runs | Windows | 4–8 h (mostly downloads) | 3, D1 |
| 5 | Spike: bring `spn_core` into the Unreal build (options in section 3) | Windows + Fedora | 6–15 h | 4 |
| 6 | M1 placeholder level + character + camera, setup note | Windows | 8–20 h | 4 |
| 7 | M1.5 trigger sets a fact, debug overlay | Windows + Fedora | 4–8 h | 5, 6 |
| 8 | Licence verification (MetaHuman, Unreal EULA, add-ons) | Developer | 2–4 h | – |
| 9 | `AppearanceSpec` data design + tests (no art) | Fedora | 6–10 h | 8 helps |
| 10 | Spikes: MetaHuman (Windows), Blender/modular (Fedora) | both | 8–16 h each | 6, 8 |

Steps 3–7 total about 25–60 h, i.e. roughly 3–7 weeks at 8–15 h/week. The tentative
"tiny playable placeholder within 2–4 weeks of development start" (CLAUDE.md) is
realistic only if the Core-integration spike goes smoothly.

## 8. Decisions actually needed from the developer

- **D1 — Unreal version.** Recommendation: the newest stable release that the Windows
  PC supports at install time, then pin it (MetaHuman is reported to be built into the
  editor from 5.6; a secondary source lists 5.8 as current stable — verify). Needed
  before step 4.
- **D2 — Unreal Content policy.** Content cannot currently be versioned (`*.uasset`,
  `*.umap` ignored). Options: code only plus hand-written setup notes; or allow own
  placeholder content under an explicit `!` path, with or without Git LFS. Needed
  before step 6.
- **D3 — How files reach the Windows PC.** Git remote (I may not create one), shared
  disk/network share, or manual copy. Needed before step 4.
- **D4 — Default for third-party character content in the public repo.** Recommendation:
  none; manifest + local fetch. Needed before any character asset is added.

Everything else (Core integration method, character pipeline, LOD tier numbers, file
format changes) stays OPEN until the spikes produce evidence.
