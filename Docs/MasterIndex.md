# Master index — where to look first

Status: navigation aid. **This file is not a second canon database.** It lists entry
points into the external planning master and says how much weight a kind of source
carries. It contains no copied requirements and decides nothing. A listed file can
still be outdated, provisional or superseded: read the section and its status words.

Paths are relative to the cumulative master directory
`Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (read-only, not in Git).
Search with `bash Tools/master-search.sh` (see the end of this file). Entry points were
checked to exist on 2026-10-20; the master can change, so a missing path means "search
again", not "requirement gone".

Abbreviations used in the tables:
`PD` = `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md`,
`OD` = `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/OPEN_DECISIONS.md`,
`LIB` = `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs`.
Sections are named by heading (line numbers move).

## 1. Tiers: how much a place weighs

A location gives a first guess only. **The status words in the section you read decide**
(LOCKED, CONFIRMED, CANON, PROVISIONAL, PROPOSAL, OPEN, BLOCKED, UNVERIFIED), and a file
name or a search hit never makes something canon. A newer file is not automatically more
binding: apply the supersession rules in
`01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION.md` (latest dated variant:
`01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION_2026-09-27.md`) and later
explicit developer decisions.

| Tier | Location | Typical content | Weight |
|---|---|---|---|
| 1 | `01_CURRENT_CANONICAL_STATE/` (incl. `ADDENDA_2026/`) | status files, addenda, decision registers, open/conflict lists | canon candidates; the addendum of 2026-10-18 says itself that acceptance of unresolved items still needs review |
| 2 | `99_DAILY_DELTAS/`, `99_CANON_AUDITS/` | review-only packages: decisions as reported, proposals, evidence, status reports | evidence and proposals; reported test results are not reproduced |
| 3 | `LIB/` (current prompt library) | `reference/PROJECT_DECISIONS.md`, `reference/OPEN_DECISIONS.md`, planning backlog, prompts | broad but older reference (2026-08-31 and earlier); **all 848 catalogued prompts have status `DRAFT_REWRITE_REQUIRED` / quality gate `NOT_PASSED`** (`LIB/governance/PROMPT_CATALOG.csv`): they are work-package drafts, not requirements |
| 4 | `04_HISTORY/`, `02_EMBEDDED_BASE_ARCHIVES/03_PROMPT_LIBRARY_HISTORY/`, `99_RECONSTRUCTION_SOURCES/`, `_SOURCE_ARCHIVE/`, `02_WORLD_700/` | history, chat transcripts, raw sources | evidence only; searched only with `--history` |

Classification used in our docs: **CONFIRMED / LOCKED**, **PROVISIONAL**, **PROPOSAL**,
**OPEN**, and **unclear or contradictory** (then ask the developer; see CLAUDE.md §11).
Known inconsistency to keep in mind: world-size figures differ between dated files
(September ranges vs. the October scenario); none of them is a geometry lock.

## 2. Always read first (small)

| File | Why |
|---|---|
| `00_START_HERE/README_MASTER.md` | supersession order and the major canonical additions up to 2026-09-05 |
| `01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION.md` | current supersession rules |
| `01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md` | production strategy and phase targets as recorded in the master |
| `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/02_DECISION_REGISTER.md` | decision table with status per row (LOCKED / PROVISIONAL / OPEN ...) |
| `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/01_CONFIRMED_AND_PROVISIONAL.md` | integrated, source-qualified decisions |
| `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/02_OPEN_AND_CONFLICTS.md` | open questions and reconciliation |
| `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/03_ENGINEERING_PROPOSALS.md` | engineering items explicitly **not** ratified |

## 3. Topics

### Story, main missions, side content
- `PD`: sections "Story and cases", "Acts, chapters and controlled story reactivity", "Long-term story threats", "Organic world-event / case emergence", "Mission ratings and trophy longevity".
- `LIB/planning-backlog/25-organic-world-events-cases.md`, `LIB/planning-backlog/34-hunter-crew-hq-acts-missions.md`, `LIB/planning-backlog/42-achievements-mission-medals-profile-platform.md`.
- `OD`: section "Product / world / story". Prompts (drafts): `LIB/prompts/03-cases-investigation/`.
- No final story, mission order or lore-specific placement exists as canon (CLAUDE.md §3).

### Characters, NPCs, character creator
- `PD`: sections "NPCs and AI", "Player-created protagonist, Hunter crew and HQ", "Voice / facial / body production".
- `LIB/planning-backlog/33-custom-protagonist-character-creator.md` (status "PLANNING INVENTORY"), `LIB/planning-backlog/22-companions-planning-orders.md`.
- `99_DAILY_DELTAS/2026-10-18/SPN_DAILY_DELTA_2026-10-18/docs/design/CHARACTER_CREATOR_AND_RELEASE_POLICY.md` (section "User-confirmed long-term direction": photorealism goal, deep modular creator for player and NPCs, staged implementation).
- `99_DAILY_DELTAS/2026-10-19/SPN_DAILY_DELTA_2026-10-19/docs/design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md` (proposal, explicitly not a locked schema).
- `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md`, section "Block 10 — social/life sandbox accepted direction".
- Prompts (drafts): `LIB/prompts/05-npc-ai-relations/`.

### Open world and cities
- `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (material-world hierarchy), `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` (main world size decision), `01_CURRENT_CANONICAL_STATE/CANONICAL_UPDATE_2026-09-08.md`.
- `01_CURRENT_CANONICAL_STATE/WORLD_1500/STATUS_2026-09-10.md`, `01_CURRENT_CANONICAL_STATE/WORLD_700/STATUS_2026-09-10.md` (keep the two lineages apart).
- `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md` (section "World and cities").
- `99_DAILY_DELTAS/2026-10-18/SPN_DAILY_DELTA_2026-10-18/docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md`, `99_DAILY_DELTAS/2026-10-19/SPN_DAILY_DELTA_2026-10-19/docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md`, `99_DAILY_DELTAS/2026-10-18/SPN_DAILY_DELTA_2026-10-18/docs/specs/MASTER_MAP_ACCEPTANCE_SPEC.md` (map candidates are not approved geography).
- `PD`: section "World". Prompts (drafts): `LIB/prompts/02-world-open-world/`.

### Underground world and realms
- `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md` (section "Underground lore and Hunter bunker"), `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/01_CONFIRMED_AND_PROVISIONAL.md` (sections "Underground society and narrative", "Society decisions U24–U32", "AI and communication U33–U36").
- Open items: `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/02_OPEN_AND_CONFLICTS.md` (city size, population, bunker vs. HQ, family key origin).
- Source deltas: `99_DAILY_DELTAS/2026-10-14/SPN_DAILY_DELTA_2026-10-14/docs/canon/UNDERGROUND_CITIES_DECISIONS.md`, `99_DAILY_DELTAS/2026-10-14/SPN_DAILY_DELTA_2026-10-14/docs/canon/HUNTER_BUNKER_AND_FAMILY_KEY.md`, `99_DAILY_DELTAS/2026-10-15/SPN_DAILY_DELTA_2026-10-15/docs/canon/UNDERGROUND_SOCIETY_DECISIONS_24-32.md`, `99_DAILY_DELTAS/2026-10-16/SPN_DAILY_DELTA_2026-10-16/docs/canon/AI_DECISIONS_33_34.md`, `99_DAILY_DELTAS/2026-10-17/SPN_DAILY_DELTA_2026-10-17/docs/canon/UNDERGROUND_AI_AND_COMMUNICATION_U35_U36.md`.
- Realms (Hell, Purgatory, Heaven): `01_CURRENT_CANONICAL_STATE/REALMS/STATUS_2026-09-10.md`, `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` (sections "Realm planning — base-game minimum", "Realm traversal and persistence direction"). Sizes and location lists there are non-locked ranges and candidates.

### Interactions and gameplay
- `PD`: sections "Investigation modes", "Stealth, perception and horror AI", "Companion autonomy, planning and field commands", "Melee / grappling", "Gunplay / weapons", "Difficulty / configuration".
- **No specification of a universal interaction system was found** (searched on 2026-10-20). Only single interactions exist as draft prompts, for example `LIB/prompts/09-vehicles-controls/012_create-engine-restart-interaction.md`. See `Docs/Proposals/0004-interaction-and-persistent-door.md` (implemented as a placeholder slice, status in its section 9).
- Phase targets: `01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md`. Open gameplay choices: `OD`.

### Combat, injury, weapons and difficulty
- `PD`: sections "Human anatomy, injury and medicine", "Death / failure — OPEN", "Difficulty / configuration", "Gunplay / weapons", "Ballistics", "Melee / grappling", "Multiple-opponent combat, improvised actions and surrender", "Stealth, perception and horror AI", "Body / physical response / recoil", "Gore / age presentation", "Cash / law / evidence".
- `OD`: section "Combat / injury / gore" (open: Combat Focus, ballistic, tissue and armor parameters, gore thresholds, minor-NPC violence presentation, melee depth) and the death/rescue item in "Product / world / story".
- Prompts (drafts, topic lists only): `LIB/prompts/06-survival-anatomy-medical/`, `07-gunplay-ballistics-ammo/`, `08-melee-grappling/`, `10-difficulty-hud-accessibility/`, and the weapon and animation parts of `14-assets-animation-gore/`.
- Several combat decisions (death/checkpoint rule, learning of fighting techniques) exist only in chat transcripts (`99_RECONSTRUCTION_SOURCES/…/GESPRAECHE/`, chats 23 and 35), not in `PD`; they are collected with sources in `Docs/Game/Kampfsystem/` (planning documentation, not canon).

### WorldFacts and save games
- `PD`: sections "World state and persistence", "Testing, tools and quality", "Testing philosophy".
- `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` (section "Realm traversal and persistence direction"), `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (section "Time travel / historical world states").
- Prompts (drafts, useful as topic lists): `LIB/prompts/11-persistence-worldstate/` (segmented saves, migration framework, old-version fixtures, crash recovery), `LIB/prompts/01-foundation-architecture/013_create-save-version-schema-version-constants.md`.
- Reports about our own C++ (not canon, not reproduced): `99_DAILY_DELTAS/2026-10-19/SPN_DAILY_DELTA_2026-10-19/docs/specs/WORLDFACTS_PHASE_0_3_2026-10-19.md`, `99_DAILY_DELTAS/2026-10-18/SPN_DAILY_DELTA_2026-10-18/docs/status/CPP_PHASE_0_PROGRESS.md`. Co-op persistence: `LIB/planning-backlog/35-online-coop-lobby-persistence.md`.
- Our own decisions live in `Docs/Decisions/` (0002, 0004, 0005, 0006); the interaction slice is in `Docs/Proposals/0004-interaction-and-persistent-door.md`; the M1 map draft, acceptance results (Technical Acceptance PASS, 2026-10-23) and the not-checked list are in `Docs/Proposals/0008-m1-test-map-and-acceptance.md`; the investigation, evidence and law documentation is in `Docs/Game/Ermittlung/`; the living-world / NPC memory / nemesis decision with its master basis is in `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md`.

### Vehicles and traffic
- `PD`: sections "Vehicles", "Vehicles / racing". `LIB/planning-backlog/38-vehicle-physics-trims-racing.md`.
- `99_DAILY_DELTAS/2026-10-15/SPN_DAILY_DELTA_2026-10-15/docs/specs/ROAD_VEHICLES_2026-10-15.md`, `99_DAILY_DELTAS/2026-10-15/SPN_DAILY_DELTA_2026-10-15/docs/specs/RAILWAY_SYSTEM_2026-10-15.md` (contain proposals next to established preferences; the 2026-10-18 conflict list says to review individually).
- Prompts (drafts): `LIB/prompts/09-vehicles-controls/`.
- Traffic, parking, fuel, jobs, police checks and insurance rest almost entirely on chat 23 block 5 (`23#1024A` questions, `23#1025E` fifty numbered answers) and on `WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT.md` / `…_8_TO_9_CURRENT.md`; `master-search.sh traffic|Verkehr|parking|garage` finds a single tier-1 hit (`CANONICAL_ADDENDUM_2026-09-05.md`, smartphone navigation and traffic apps). There is no traffic model, no traffic law and no vehicle list in the master. The decisions are collected with sources, conflicts and open questions in `Docs/Game/Fahrzeuge-und-Verkehr/` (planning documentation, not canon). The road-graph contract for the generated map lives outside this repository (`SPN_WorldForge`, `RC-01`, an M2 pre-contract).

### Supernatural beings and abilities
- `PD`: sections "Creature / supernatural framework — expanded", "Peaceful supernatural beings and independent hunter agency", "Hunter Archive epistemology", "Wildlife / study / harvesting / monsters".
- `LIB/planning-backlog/24-creature-supernatural-framework.md`, `LIB/planning-backlog/37-wildlife-study-harvest-forensics.md`.
- Prompts (drafts): `LIB/prompts/04-hunter-archive-progress/`. No specification of player supernatural abilities was found; "Progression" is marked OPEN in `PD`.

### Modding
- **Confirmed:** `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md`, status in `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_BLOCK_STATUS_2026-09-11.md`. `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_RECONSTRUCTED_2026-09-09.md` is a reconstruction (evidence).
- `PD`: section "Modding"; `OD`: section "Creator economy / modding". Prompts (drafts): `LIB/prompts/12-modding-sdk/`.

### Graphics, animation, sound
- `PD`: sections "Rendering, graphics settings and platform performance targets", "Audio production, spatial acoustics and quality", "Voice production strategy", "AI-assisted Blender / scene production — workflow lessons".
- `LIB/planning-backlog/20-audio-production-spatial-qa.md`, `LIB/planning-backlog/21-rendering-performance-platforms.md`, `LIB/planning-backlog/28-visual-reference-system.md`, `LIB/planning-backlog/29-ai-assisted-blender-production.md`.
- `99_DAILY_DELTAS/2026-10-19/SPN_DAILY_DELTA_2026-10-19/docs/design/AI_3D_PIPELINE_WATCH_2026-10-19.md` (tool watch, not a decision). Prompts (drafts): `LIB/prompts/14-assets-animation-gore/`.

## 4. Using a source in our docs
Record each adopted requirement in the document that uses it, as a "Master basis"
table: requirement, status as read, path and section (relative to the cumulative
directory above), and the consuming code or test. Keep proposals of ours separate and
labelled ENGINEERING PROPOSAL / POLICY. Never edit anything under `Planning/Master`.

## 5. Search workflow
Linux / Fedora (and Git Bash or WSL on Windows, with ripgrep installed):

```bash
bash Tools/master-search.sh "opened gates"                  # tiers 1+2 (default)
bash Tools/master-search.sh --tier 1,2,3 door interaction   # several terms = alternatives
bash Tools/master-search.sh --history --count "Tesla"       # include tier 4, file counts only
bash Tools/master-search.sh --regex 'WORLD_(700|1500)'      # regular expression
```

Terms are literal by default, so spaces and characters like `( ) . *` need no escaping.
Output lines are `path:line:text`, relative to the cumulative directory. Exit codes:
0 hits, 1 no hits, 2 usage error, 3 master not available, 4 no search tool. The master
location is `Planning/Master` or `$SPN_MASTER_DIR` (needed where the symlink does not
exist, e.g. Windows without symlink rights; it may point at the archive directory or at
the cumulative directory).

Windows, PowerShell with ripgrep (the master is only read):

```powershell
$env:SPN_MASTER_DIR = 'D:\SPN_FULL_MASTER_2026-09-27'   # example path, adjust
$root = (Get-ChildItem $env:SPN_MASTER_DIR -Directory -Filter 'SPN_FULL_CUMULATIVE_MASTER_*' | Select-Object -First 1).FullName
Push-Location $root
rg -n -i -F -e 'opened gates' --glob '*.md' --glob '!**/extracted_*/**' 01_CURRENT_CANONICAL_STATE 99_DAILY_DELTAS 99_CANON_AUDITS
Pop-Location
```

Without ripgrep (slower, PowerShell only):

```powershell
Get-ChildItem (Join-Path $root '01_CURRENT_CANONICAL_STATE') -Recurse -Filter *.md |
  Select-String -SimpleMatch -Pattern 'opened gates' |
  ForEach-Object { '{0}:{1}: {2}' -f $_.Path, $_.LineNumber, $_.Line.Trim() }
```

The snippets were run with PowerShell 7 on Linux (ripgrep and `Select-String` variants, same hits). Windows PowerShell 5.1 and native Windows paths are **not yet tested**.

Reading hits is the actual work: open the section, find its status words, and note
whether it is CONFIRMED, PROVISIONAL, PROPOSAL, OPEN or unclear before using it.
