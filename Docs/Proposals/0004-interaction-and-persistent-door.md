# 0004 — Small universal interaction system and first persistent test door (Phase 0.3 preparation) — ENGINEERING PROPOSAL

Status: proposal for developer review, written 2026-10-20; decisions D1 and D2 approved
by the developer the same day (section 2.1). **Not master canon.** Implementation status (2026-10-22): increments 1 to 5 are done, see section 9. The door is a **test object**, not a design for doors in the game.
No world size, map, story, lore, gameplay value or co-op/network decision is made here.

Paths in section 1 are relative to
`Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (read-only). `PD` abbreviates
`02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md`
(tier 3 in `Docs/MasterIndex.md`: a "living source of truth" dated 2026-08-31, older than
the tier 1 files; line numbers as read on 2026-10-20). Searches were made with
`Tools/master-search.sh` and the relevant sections were read, not only matched.

## 1. Master basis

### 1.1 Requirements taken over

| # | Requirement (paraphrased) | Status as read | Source (path, section, line) |
|---|---|---|---|
| M1 | Central persistent world state is "the memory of the world"; durable state such as changed or destroyed locations and discovered knowledge is tracked | CONFIRMED (tier 3 decision list; not contradicted by tier 1) | `PD`, "World state and persistence", L79–80 |
| M2 | Important persistent entities use stable IDs, not transient object identity | CONFIRMED (tier 3) | `PD`, "World state and persistence", L81 |
| M3 | World Partition loads runtime content; the world state decides which persistent variant/state applies when it loads | CONFIRMED (tier 3) | `PD`, "World state and persistence", L82 |
| M4 | Major actions persist and do not casually reset: opened gates, removed artifacts, changed locations, freed/killed entities | CANON DIRECTION (tier 1, section is titled "direction") | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md`, "Realm traversal and persistence direction", L75 |
| M5 | Saves are segmented, incremental, crash-safe, migratable, recoverable and compressed rather than one blob; reliability outranks size | CONFIRMED (tier 3) | `PD`, "World state and persistence", L83–84 |
| M6 | Save/load/migration/world-state tests are required; persistence and save/load are high-risk and need deterministic, replay-based tests; real bugs become regression tests | CONFIRMED (tier 3) | `PD`, "Testing, tools and quality", L264; "Testing philosophy", L379–388 |
| M7 | Unreal for engine-level systems; bespoke C++ for world state, persistence and modding behaviour; C++20 baseline | CONFIRMED / CANON BASELINE | `PD`, "Technology", L13–16; `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/02_DECISION_REGISTER.md`, D-005 |
| M8 | Mods are additive by default through registries and hooks; overrides need explicit layer/load-order rules | CONFIRMED (tier 3) | `PD`, "Modding", L239 |
| M9 | Mods can be enabled/disabled per individual save | CONFIRMED | `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md`, "Profiles and save-specific activation", L27–31 |
| M10 | Story co-op up to four players is a **direction**; replication and authority choices are open | DIRECTION / OPEN | `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md` L20; `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/03_OPEN_QUESTIONS.md` item 7 |
| M11 | Doors have acoustic outcomes depending on operation and condition (old doors may creak); horror emerges partly from doors, darkness, power and AI | CONFIRMED (tier 3), **later feature**, not part of this slice | `PD`, "Stealth, perception and horror AI", L306; "NPCs and AI", L76 |
| M12 | Phase 0 aims at a small working integration including interactions, items, save/load and world state, not full systems before a first playable test | CLAUDE.md §4 (project instruction); the master records the same production strategy | `CLAUDE.md` §4; `01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md`, "Production strategy" |

### 1.2 Searched but not found

- **No specification of a universal interaction system** (verbs, prompts, input, taxonomy).
  The only hits are single interactions in draft prompts, for example
  `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/prompts/09-vehicles-controls/012_create-engine-restart-interaction.md`.
  The "interaction category taxonomy" in `OPEN_DECISIONS.md` ("Adult / romance / interaction")
  concerns a different topic.
- **No door state model** (locked, jammed, keys, lockpicking, breakable doors).
- Prompts in `.../docs/prompts/` are all `DRAFT_REWRITE_REQUIRED` / `NOT_PASSED`
  (`.../docs/governance/PROMPT_CATALOG.csv`). `.../prompts/11-persistence-worldstate/` and
  `.../prompts/01-foundation-architecture/017_create-project-wide-stable-soft-reference-policy.md`
  are topic lists for later work, not requirements used here.

Absence of hits is not proof that nothing exists; the search covered tiers 1–3.

## 2. What is confirmed, what is proposed, what is open

- **Developer's preferred technical direction (2026-10-20, not a final decision about
  all interactions in the finished game):** a lean, reusable interaction system with
  stable object IDs and `WorldFacts` as the persistent state source. Sections 3–6 are
  written to this direction and stay revisable as further systems (inventory,
  investigation, dialogue) define what they need.
- **Confirmed requirements to respect:** M1–M9 (state in the persistent world state,
  stable IDs, no reset of major actions, migratable saves, tests for save/load).
- **Direction only:** co-op (M10). Do not build networking, but do not bake in
  single-player assumptions.
- **Engineering proposal (this document, sections 3–6):** the interaction mechanism, the
  door facts, the Unreal bridge and the tests.
- **Approved technical decisions for Phase 0.3:** D1 and D2 in section 2.1.
- **Open (developer decision needed, section 7):** door persistence class, lock model,
  save slot layout, co-op authority, bridge versus export macro, authored defaults and
  mod registration, input design. Each is decided when its implementation phase needs it.

### 2.1 Approved technical decisions for Phase 0.3
Confirmed by the developer on 2026-10-20. They are **approved engineering decisions for
Phase 0.3, not story canon, not master canon and not a final design for all interactions
or objects in the finished game.** They may be revised by a later explicit developer
decision.

| ID | Decision | Consequences |
|---|---|---|
| D1 | Phase 0.3 implements only the interaction action `Use`. The system must stay extensible without implementing further actions now. | `InteractionVerb` is an `enum class` with the single value `Use`; handlers receive the verb as an argument, so adding a value later changes no existing call site. No other verb, placeholder verb or verb-specific code is written. |
| D2 | Stable object IDs have at most 96 characters and the grammar `[a-z][a-z0-9_.]*`. This limit is a **technical prototype rule** for now. | One validation function in Core is the single place for the rule; violations give `InvalidId`. 96 plus the door suffixes fits the existing fact-key limit of 128: `placeholder.door.` (17) + 96 + `.locked` (7) = 120. The limit is not a save-format rule yet because it is not stored as such; fact keys built from IDs are still bound by the v1 key rules (`Docs/Decisions/0004-world-facts-save-format-v1.md`). Raising it later is compatible with old saves; lowering it is not. |

D2 settles the id grammar for the prototype only. How real placed objects obtain their
ids (hand-named, GUID plus label, registry) is not decided by it and is revisited before
real content is placed (see question 7).

## 3. Proposal: engine-independent core (`Core`, tested on Fedora)

### 3.1 Principles
1. **State lives only in `WorldFacts`** (M1, M4). The interaction system holds no state
   of its own; a door is a set of facts.
2. **Pure rules, explicit commands:** a handler reads the store and returns the commands
   it wants applied. The executor applies them with a `cause` for the journal.
3. **No literal player-facing text in Core.** Feedback is a `ContentId`
   (for example `placeholder:feedback.door.locked`); UI, audio and localisation map it.
4. **Absent fact = authored default.** Level data supplies the default state
   (`closed`, `unlocked`). Only deviations are written, which keeps saves small and means
   a placed door costs nothing until used.
5. **Never write a no-op:** `WorldFacts` journals every valid command, so a refused or
   unchanged interaction issues no command.
6. **Unknown kinds are data, not errors in the save.** A save containing facts for an
   interactable kind that is not registered (for example from a mod disabled for this
   save, M9) loads and re-saves unchanged; only *using* it fails with `UnknownKind`.

### 3.2 Sketch (names and signatures are not final)

```cpp
// Core/include/spn/core/Interaction.h
enum class InteractionVerb : std::uint8_t { Use };   // D1: only `Use` now; values are appended later

struct InteractableRef {
    ContentId kind;      // e.g. placeholder:interactable.door
    std::string id;      // D2: stable id, [a-z][a-z0-9_.]*, 1..96 characters
};

struct InteractionOutcome {
    std::vector<ChangeRecord> changes;  // empty if nothing changed
    ContentId feedback;                  // empty = none
};

enum class InteractionErrorCode : std::uint8_t { InvalidId, UnknownKind, StateCorrupt, Refused, Fact };

class InteractionRegistry {            // kind -> handler; mods register additively (M8)
public:
    Result<void, InteractionError> Register(ContentId kind, InteractionHandler handler);
    Result<InteractionOutcome, InteractionError>
    Interact(WorldFacts& facts, const InteractableRef&, InteractionVerb,
             const InteractionDefaults&, std::string_view cause) const;
    std::vector<InteractionVerb> AvailableVerbs(const WorldFacts&, const InteractableRef&,
                                                const InteractionDefaults&) const;
};
```

`InteractionDefaults` carries the authored initial state (for the door `open`, `locked`).
Placement data owns it, so changing a level default affects only doors never touched by
the player (documented, tested).

### 3.3 Test door facts (placeholder)
| Fact key | Type | Meaning |
|---|---|---|
| `placeholder.door.<id>.open` | bool | current state; absent = authored default |
| `placeholder.door.<id>.locked` | bool | absent = authored default; the test sets it directly, no key/lockpick model |

- `Use` on an unlocked door writes `.open` = not current. On a locked door: refusal
  (`Refused`, feedback `placeholder:feedback.door.locked`), **no command, no journal entry**.
- The `placeholder.` prefix marks these keys as non-final. Renaming real door keys later is
  a save-compatibility event: it needs the migration path from `Docs/Decisions/0004`,
  not a silent rename.
- A wrong type behind a key (for example `.open` as integer) yields `StateCorrupt`
  and changes nothing.

### 3.4 Save and load of the door state
The door state is part of the existing `WorldFacts` journal and format v1
(`SPNFACTS 1`). **No format change**: `Serialize()` / `Deserialize()` already provide
strict, all-or-nothing loading, and `StateAt(n)` already answers "was this door open
before event n". Loading restores the facts; actors read them when they start and when a
load replaces the store.

## 4. Proposal: integration into the Unreal prototype (Windows, UE 5.8.3)

Context: `Docs/Decisions/0006-unreal-core-integration.md`. Core is compiled into module
`SpnCore` by UBT. In modular builds Core symbols are not exported, so game modules use
exported bridge functions, not Core types. The open choice "bridge layer vs export macro"
stays open; this proposal uses the bridge.

| Piece | Where | Responsibility |
|---|---|---|
| `USpnWorldStateSubsystem` (GameInstance subsystem) | `SpnCore` module (new files only) | owns one `WorldFacts` and the `InteractionRegistry`; exposes `TryInteract`, `ReadFlag`, `SaveToSlot`, `LoadFromSlot`; broadcasts a delegate when the store was replaced by a load |
| `ISpnInteractable` + `ASpnTestDoor` | game module `SupernaturalProt` (new files only) | placeholder mesh (cube, labelled placeholder), `StableId` (FName), authored defaults, reads the fact on `BeginPlay` and after a load, animates between states |
| Interact input and trace | existing `ASupernaturalProtCharacter` (small, reviewed change) | one input action, short line trace; the call goes through a function that runs on the authority only |
| Slot file | `Saved/SpnSaves/<slot>/worldfacts.spnfacts` | written to a temporary file and renamed, so a crash cannot leave a half-written save; **placeholder** for the segmented, crash-safe save system of M5 |

Authority and co-op (M10): the interaction call is structured as *request → authority
applies → state replicates*. In single player the local machine is the authority. No
replication code is written now; the host owns `WorldFacts`, consistent with the
host-save-authoritative direction in `CLAUDE.md` §6.

Load failure: if `Deserialize` fails, the current store stays untouched, the error code
and line are logged, and the player sees no half-loaded world.

Needed in the Unreal Editor (Windows): one input action asset, a test map with one door,
placing `ASpnTestDoor`. These are binary assets (LFS rules in `.gitattributes`) and
cannot be created on Fedora.

## 5. Tests and acceptance criteria

### 5.1 Core tests (Fedora, all four presets, doctest)
`Tests/Core/InteractionTests.cpp`:
1. Unlocked closed door: `Use` opens it; exactly one journal record with the given cause.
2. `Use` again closes it; two records; `Revision()` increments by one per change.
3. Locked door: `Use` refuses with the locked feedback id; `Revision()` unchanged.
4. Absent facts follow the authored defaults, including a door authored as open.
5. Level default changed after the player touched the door: touched door keeps its fact,
   untouched door follows the new default.
6. Wrong fact type → `StateCorrupt`, store unchanged.
7. Stable id rule (D2): accepted at 1 and at 96 characters; rejected at 0 and at 97, with
   an uppercase letter, a leading digit or dot, a space, a hyphen, a non-ASCII byte and an
   embedded control character → `InvalidId`. A 96-character id with the longest door suffix
   produces a valid fact key. Unregistered kind → `UnknownKind`.
7a. Verb rule (D1): the registry and handler signature take `InteractionVerb`; a test
   asserts that the enumeration has exactly one value today (so adding one forces a conscious
   test update), and that `AvailableVerbs` returns exactly `{Use}` for a door.
8. `AvailableVerbs` is consistent with `Interact` (offered ⇒ may succeed, refused ⇒ reported).
9. Determinism: the same command sequence on two stores gives equal journals and identical
   `Serialize()` bytes. The actor name appears only in the `cause`, never in the result.
10. A refused or unknown-kind interaction never partially modifies the store.

`Tests/Core/DoorPersistenceTests.cpp`:
11. Play sequence → `Serialize` → `Deserialize`: door states and journal equal, re-save
    byte-identical.
12. `StateAt(n)` shows the earlier door state for every prefix of a recorded sequence.
13. A save with facts of an unregistered interactable kind loads, round-trips unchanged,
    and does not alter other doors (mod-disabled-per-save case, M9).
14. A damaged save (truncated, wrong version, tampered value) is rejected and the
    in-memory store of the caller is unchanged.

Regression rules: no golden fixture for the placeholder keys (they are not a contract
yet); the existing golden and limits fixtures of format v1 stay untouched.

### 5.2 Unreal checks (Windows, after the Core part passes)
- Automation test in `SpnCore` (pattern of `SpnWorldFactsSmokeTestAutomation.cpp`):
  open → save to a temporary slot → new subsystem → load → door fact restored; corrupted
  slot file rejected with an error code, state unchanged.
- Manual acceptance in the test map: door closed at start; interact opens it; save, restart
  PIE, load: door is open; a locked variant refuses and logs the feedback id; a second door
  is unaffected.

### 5.3 Acceptance criteria (definition of done for this slice)
1. Criteria 1–14 above pass on `clang`, `gcc`, `clang-asan-ubsan`, `gcc-asan-ubsan`
   without warnings, and the existing tests (currently 154) still pass.
2. No change to `WorldFacts`, to format v1 or to any existing fixture.
3. `Core` still includes no Unreal headers (grep check as in Phase 0.4).
4. The Unreal part builds on Windows and the automation test passes.
5. `Docs/Decisions` gets one short record of what was built; this proposal's open points
   are either decided by the developer or remain labelled OPEN.

## 6. Increments (each small and reversible)
1. **Core:** `Interaction.h/.cpp`, door handler, `InteractionTests.cpp`, CMake entries.
   Fedora only; ends with the test results of all four presets.
2. **Core persistence tests:** `DoorPersistenceTests.cpp` (cases 11–14).
3. **Unreal bridge:** `USpnWorldStateSubsystem` plus automation test (Windows).
4. **Unreal door:** `ASpnTestDoor`, input action, test map (Windows, editor).
5. Decision record and update of `Docs/MasterIndex.md` if sources were added.

## 7. Open decisions (not decided here)
1. ~~**Taxonomy**~~ **Decided for Phase 0.3 (D1):** only `Use`, extensible. The wider verb
   set (inspect, take, talk, ...) is not specified in the master; later systems
   (inventory, investigation, dialogue) should drive it.
2. **Persistence class of doors:** every toggle journals one record. Whether ordinary doors
   persist at all, or only specific ones (M4 names gates), and whether frequent toggles are
   coalesced, belongs to the storage policy in `Docs/Decisions/0004-world-facts-save-format-v1.md`
   §6 and to compaction (open).
3. **Lock model:** keys, lockpicking, jamming, breaking, noise (M11).
4. **Save layout:** segmented saves, manifest and atomic commit (M5) versus the single
   placeholder slot file used here.
5. **Co-op authority** and conflicts when two players use one door in the same frame
   (WorldFacts has no compare-and-set).
6. **Bridge versus export macro** for Core types in game modules (open in `0006`).
7. ~~**Stable IDs for placed actors**~~ **Decided as a prototype rule (D2):** grammar
   `[a-z][a-z0-9_.]*`, at most 96 characters; the test door is hand-named. The id source
   for real content under World Partition (a registry is only a draft prompt) is revisited
   before such content exists.
8. **Where authored defaults live** (actor property or data asset), and mod registration
   of new interactable kinds (M8).
9. **Input and prompt UI** (trace versus overlap, prompt design, controller bindings).

## 8. Non-goals
No inventory, keys, lockpicking, door sounds, AI use of doors, breakable doors, multiple
verbs, replication, mod loading, segmented save system, UI, or final door assets. No
change to `Planning/Master`, `WorldFacts` or the save format.

## 9. Implementation status (2026-10-22)
| Criterion (section 5.3) | State |
|---|---|
| 1. Cases 1–14 and all existing tests, four presets, no warnings | **Done on Fedora**: 181 of 181 tests pass in `fedora-gcc`, `fedora-clang`, `fedora-gcc-asan-ubsan`, `fedora-clang-asan-ubsan`, zero compiler warnings (the "currently 154" in 5.3 is historical) |
| 2. No change to `WorldFacts`, format v1 or fixtures | Done (golden and limits tests unchanged and passing) |
| 3. `Core` includes no Unreal headers | Done (grep over `Core` and `Tests` finds none) |
| 4. Unreal part builds on Windows and the automation test passes | Last documented on Windows on 2026-10-20 (10 automation tests); **not re-run on 2026-10-22** (no Unreal on the Fedora machine) |
| 5. Decision record, open points labelled | Engineering notes in `Docs/Decisions/0006-unreal-core-integration.md`; open points in section 7 stay open |
