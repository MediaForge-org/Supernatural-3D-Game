# 0010 — Living world, persistent NPC memory and nemesis/rivalry system — PRODUCT DECISION + ARCHITECTURE PROPOSAL

Status: written 2026-10-22. **Section 1 records a binding product decision** (stated by the developer on 2026-10-22). Sections 3 to 9 are an **engineering proposal**: building blocks and strategies are suggestions, not decisions. **Not master canon** and nothing under `C:\SPN\Master` was changed; section 2 lists what the master already says. **No code is written for this system now. It is not part of M1.**

Language note: the player-facing product statement is also recorded in German in `Docs/Game/Gameplay-Systeme.md` section 8 and `Docs/Game/Entscheidungsstatus.md`; this document keeps the technical detail in English like the other proposals.

## 1. Binding product decision (CONFIRMED by the developer, 2026-10-22)
The finished game contains a deep, persistent living-world system with NPC memory and a nemesis/rivalry system. It is a **mandatory part of the long-term vision**; it is **not an M1 feature** and its implementation order is open (Development phases set order, not scope; `CLAUDE.md` section 1).

| ID | Requirement (binding as product direction) |
|---|---|
| A | **Persistent NPC identities.** Important NPCs keep a permanent identity, personal traits, social relationships, goals and an individual life history. |
| B | **Long-term memory.** NPCs can remember relevant encounters, decisions, help, betrayal, injuries and conflicts. Memories influence later behaviour. |
| C | **Nemesis and rivalry.** Surviving opponents can return, develop personal enmities, learn from earlier encounters and adapt their tactics. This explicitly includes intelligent supernatural creatures. |
| D | **Autonomous factions and organisations.** Monster groups, hunter organisations, criminal networks, authorities and other groups pursue their own goals, form alliances, claim territory, fight each other and change strategy. |
| E | **Persistent consequences.** Injuries, deaths, rescues, destroyed infrastructure, conflicts and other relevant events can have lasting effects. |
| F | **Independent life development.** Important NPCs develop without direct player interaction: change relationships, move, work, join or leave organisations, react to world events. |
| G | **Scalable world simulation** for a world of roughly 2,200 km^2 (a working scenario, not an approved footprint): high fidelity near the player, reduced simulation far away, event-based advancement of NPCs and areas that are not loaded, persistence of important outcomes. Not every NPC runs as a full Unreal actor. |
| H | **Integration** with WorldFacts, investigation/caseboard, monster AI and bestiary, injuries and medicine, reputation, police and justice, organisations and territories, dialogue and relationships, save/load, co-op and host-save persistence. The simulation must not create contradictory states between these systems. |

Reference: the Red Dead Redemption 2 mod at `https://www.nexusmods.com/reddeadredemption2/mods/10967` serves only as a conceptual comparison named by the developer. It is not a dependency and not a licence to take code; **the page was not opened or evaluated for this document**. Our systems are specified independently.

## 2. Master basis (consultation per `CLAUDE.md` section 11)
Search made on 2026-10-22 with `Tools/master-search.sh` (`SPN_MASTER_DIR=/c/SPN/Master`, tiers 1 to 3) plus the project's own `Docs/`. Hits are pointers; status words are as read.

| # | Finding | Status as read | Source |
|---|---|---|---|
| L1 | Important NPCs have memory; relationship values are internal (trust, fear, respect, suspicion, ...), never shown as bars; important event memories persist; NPC beliefs may be wrong | planned (project docs, 📋) | `Docs/Game/Gameplay-Systeme.md` section 8; README 13.2 |
| L2 | Persistence tiers A to D (A story/hunter/close relationships, maximal; B merchants, neighbours, recurring contacts; C ordinary residents, lighter; D crowds, strongly abstracted) | planned (📋) | `Docs/Game/Spielwelt-und-Untergrund.md` section 13; README 13.1 |
| L3 | NPCs have routines, move, change jobs, age, die; the world keeps running without the player; crime runs independently; emergency services and repair of damage | planned (📋) | `Docs/Game/Spielwelt-und-Untergrund.md` section 13; README 13.1, 1.2 |
| L4 | Intelligent creatures remember the player, companions, injuries, traps, weapons, mercy and hostility, only what they plausibly observed; monsters are not respawning spawn points; named individuals with history; scars persist; returning individuals show the consequences of earlier fights | planned (📋) | `Docs/Game/Spielvision.md` sections 9.2, 9.3; README 7.3 |
| L5 | Hunters, loose network and several organisations; small agency units know parts of the truth; concrete factions are **open** | planned / open | README 7.4, `Docs/Game/Spielvision.md` section 10 |
| L6 | World state is "the memory of the world": NPC alive/dead, relationships, changed or destroyed places, monster activity, reputation, ...; stable IDs; segmented, incremental, crash-safe saves | confirmed (tier 3) / planned | `PD` "World state and persistence" L79-84 (see Proposal 0004 section 1), `Docs/Game/Gameplay-Systeme.md` section 16 |
| L7 | NPC simulation LOD and crowd budgets independent of the lore population; "detailed behaviour near the player and bounded offscreen updates for remote AI/NPC activity"; budgets unselected | **proposal, review-only delta, not canon** | `99_DAILY_DELTAS/2026-10-14/.../POPULATION_AND_SPATIAL_SCENARIOS.md` item 5; `99_DAILY_DELTAS/2026-10-16/.../AI_SIMULATION_BOUNDARIES.md` (candidate architecture 6) |
| L8 | Population planning figure: several hundred thousand NPCs; simulation levels proposed by an assistant are undecided | provisional / open (`W-TV-010`) | `Docs/Game/Spielwelt-und-Untergrund.md` section 13 |
| L9 | Prompt library topics on hidden relationship model, remembered-event dialogue hook, persistent relationship state record | DRAFT_REWRITE_REQUIRED, not requirements | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/governance/PROMPT_ORDER.md` (SPN-05-007..013, SPN-11-006) |

Searched, **not found** in tiers 1 and 2 of the master: the terms "nemesis", "rivalry", "grudge", "living world", "NPC memory". The nemesis/rivalry wording (requirement C) is therefore **new input**, to be recorded in a separate canon addendum when the developer chooses; the underlying ideas (L4, L5) are already in the planning documents. No conflict between sources was found. L7 shows that only the LOD idea exists as a (non-canon) proposal.

## 3. Target picture
A persistent cast of individuals and groups whose history is written into the world state: a survivor you wounded returns changed (scarred, better prepared, with allies, knowing what worked against them), a faction you ignored takes a district, an NPC you helped moves to a better home and remembers you. The simulation is **bounded and event-driven**: nothing needs a full actor to progress, and what the player can observe is always consistent with what the persistent state says.

## 4. Possible technical building blocks (PROPOSAL; none decided)
1. **Entity records, not actors.** Each persistent NPC/creature/organisation is a data record with a stable ID (D2 grammar and fact-key limits apply, see Proposal 0004). Unreal actors are a temporary presentation of a record (load/unload by distance, World Partition).
2. **Event journal as source of truth.** Meaningful events (encounter, injury, help, betrayal, death, relocation, joining a group) are appended as events with cause, time, participants and location. Derived state (memory, relationship, reputation, enmity) is computed from events and can be rebuilt. This fits the existing journal design of WorldFacts (replay, `StateAt`).
3. **Memory model.** Per individual: a bounded set of remembered events with strength, observed-or-heard flag, accuracy (beliefs may be wrong), decay and importance, plus derived dispositions. Only plausible observations create memories (L4).
4. **Nemesis/rivalry model.** A survivor entity keeps an encounter history, a learned model of the player's methods (what worked, what failed), injuries and scars, resources and allies, and goals. Return, escalation and tactic change are decided by rules from this record. For supernatural creatures, adaptation is bounded by the species rules and weaknesses (bestiary), never by free invention.
5. **Faction model.** Organisations as entities with goals, resources, territory claims, relations to other factions and a strategy state machine; advanced in coarse time steps off-screen.
6. **Simulation tiers (extends A to D).** (a) Full actors with local AI near the player; (b) reduced simulation of known locations with simplified routines; (c) schedule/statistical advancement of far regions; (d) pure event-based fast-forward of unloaded individuals: on load, a record is advanced from its last time to now by deterministic or seeded rules. Tier thresholds are configuration.
7. **Determinism and seeds.** Off-screen advancement uses a seed per entity and time step so that a loaded region reconstructs the same outcome in single player and co-op, and tests can replay it.
8. **Debug and explainability.** Tools that show why an NPC acts (memory, goals, events); hidden from the shipping UI (consistent with hidden relationship values).

## 5. Scaling strategy
- Budget the world, not every NPC: a small set of hand-written/important individuals (tier A/B) with full records; ordinary residents (C) as seed plus overrides ("recipes" in the style of the appearance system, `Docs/Decisions/0005`); crowds (D) not stored individually.
- Per-frame cost only for loaded areas; far regions advance by events at coarse intervals. Numbers (entity counts, tick budgets, memory per record) are **not chosen**; measure with a prototype before fixing them. The 2,200 km^2 figure is a scenario; nothing may hard-code it.
- Promotion/demotion between tiers must be lossless for **important outcomes**: what is demoted keeps its identity, relationships, injuries and goals.

## 6. Persistence requirements
- Everything the living world needs after a load must be in the save (identity, memories, relationships, enmity, faction state, consequences). Saves stay segmented, crash-safe and migratable (existing M5 direction).
- **WorldFacts limits to respect** (Proposal 0003 section 2): the journal grows without bound, so high-frequency or derived simulation state does **not** go through WorldFacts; a snapshot plus journal-tail format and compaction are open and conflict with "reconstruct history from change events" (which history to keep is an open decision). The format v1 and its compatibility rules stay unchanged; new record kinds need a versioned, migratable format.
- Co-op: host-save-authoritative (`CLAUDE.md` section 6); guests' progress is scoped to the host save. The simulation runs on the authority only; replication of simulation state is an open topic. WorldFacts has no compare-and-set yet.
- Mods: new kinds must be registered additively; a save containing unknown kinds must load and re-save unchanged (as for interactables, Proposal 0004).

## 7. Consistency between systems (requirement H)
The following are proposed rules to avoid contradictions; they are not implemented.
1. **One owner per fact.** Each kind of state has exactly one authoritative store (entity record, injury state, case/evidence state, reputation, territory); others hold references by stable ID and read, they do not copy.
2. **Events, not direct writes.** Cross-system effects (a death changes relationships, reputation, a case, a territory) are expressed as events processed in a defined order, so replay gives the same result.
3. **Invariants as tests.** Examples: a dead individual takes no actions; an injury is visible in every system that shows the person; a witness only knows what they observed or heard; a case cannot cite an event that did not happen; a faction does not hold territory its members cannot reach; state restored from a save equals state before saving.
4. **Observation consistency.** What the player sees after loading a region equals the persistent record advanced to now; no "pop" resets of important outcomes.
5. **Time model.** One world clock (history and time travel are separate open topics); off-screen advancement never runs ahead of it.

## 8. Dependencies on existing and planned systems
`WorldFacts` (keys, journal, limits), interaction registry and stable IDs, `USpnWorldStateSubsystem`, appearance recipes (individual looks), investigation/caseboard, monster AI and bestiary (species rules and weaknesses), injuries and medicine, reputation, police and justice, organisations and territories, dialogue and relationship context (hidden values), save/load and migrations, co-op authority, modding registries. Most of these do not exist yet; this system must not be started before at least its minimal inputs exist (section 10).

## 9. Risks
- **Scope and performance:** the largest simulation in the project; unbounded growth of memories/events and of the journal; hidden CPU cost off-screen. Mitigation: budgets, prototypes, bounded memory, coarse stepping.
- **Incoherence:** simulation contradicting authored story, cases or other systems. Mitigation: ownership and event rules (section 7), authored "protected" entities (as for story-critical NPCs).
- **Fun and readability:** enmity that the player cannot read or influence. Mitigation: hidden values with visible consequences, tuning on prototypes.
- **Save size and compatibility:** growth and format changes. Mitigation: versioned formats, compaction decision, golden-file tests.
- **Determinism and co-op:** divergent off-screen outcomes. Mitigation: seeded advancement, authority-only simulation.
- **Legal/IP:** conceptual similarity to other games' systems. Mitigation: independent design, no code or asset reuse, review before publication.

## 10. Later development phases (indicative; not commitments)
- **L0 (now):** this decision recorded; no code.
- **L1:** data model for entity IDs, event records and a bounded memory record in portable Core C++ with tests (no Unreal); decide the WorldFacts/journal boundary and the compaction question first.
- **L2:** one persistent named NPC with memory in the vertical slice (a witness who remembers, with consequences in dialogue and the case) on top of the investigation and dialogue systems.
- **L3:** one returning enemy (nemesis prototype) for a single creature type, bounded by its species rules.
- **L4:** tiered simulation and event-based fast-forward over a small test region; measure budgets.
- **L5:** factions and territories; scaling toward the full world; co-op replication decision.
Phases follow the project phases (`CLAUDE.md` section 4): only the tag `[LATER]`/`[FUTURE/AI]` sequencing applies, scope is not cut.

## 11. Open decisions (not decided here)
Entity and event schema; memory model and decay; which history the journal keeps (compaction); simulation tier thresholds and budgets; how a nemesis learns (rules vs data); faction list and goals (lore); protected/authored NPC rules; what the player can observe of enmity; replication of simulation state; world clock and time travel interaction; mod extension points.

## 12. Non-goals of this document
No code, no schema, no numbers, no lore (names of factions or antagonists), no change to WorldFacts, save format v1 or the master.
