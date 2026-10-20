# 0004 — WorldFacts save format v1 and compatibility (Phase 0.3) — ENGINEERING PROPOSAL

Status: technical documentation of the format implemented in Phase 0.2, plus a
compatibility strategy proposed for review. **Not master canon.** It fixes no world
size, map, gameplay value or co-op/network decision.

## 1. Format v1 (frozen)

A document is UTF-8-agnostic bytes with `\n` line ends and no other raw control
characters outside the field structure:

```
SPNFACTS 1                                              header, version = canonical decimal
change<TAB>seq<TAB>op<TAB>key<TAB>arg<TAB>previous<TAB>current<TAB>cause     (0..n lines)
end<TAB>n                                               trailer, n = number of change lines
```

- `seq`: 1-based, gapless, canonical decimal (no `+`, no leading zeros).
- `op`: `set` (arg = value), `remove` (arg = `-`), `adjust` (arg = `i:<delta>`).
- Value tokens: `b:true`, `b:false`, `i:<int64 decimal>`, `s:"<escaped>"`; `-` in
  `previous`/`current` means "no value" (fact absent before / removed).
- String escapes: `\\`, `\"`, `\n`, `\r`, `\t`; all other bytes < 0x20 and 0x7f as
  `\xHH` (lower-case hex); every other byte, including UTF-8, raw.
- `cause` is always a quoted string (`""` if empty).
- The file **is** the journal. State is derived by replaying every command; each
  record's `previous`/`current` must match the replay result (`JournalMismatch`).

### Strictness rules (the compatibility contract)
1. **Canonical-only loading.** A token is accepted only if re-encoding it yields the
   identical text. Consequently `Serialize(Deserialize(x)) == x` for every accepted
   `x`; damaged data is never silently normalized.
2. **No partial success.** Any error rejects the whole document
   (`FactError{code, line}`); no store is returned.
3. **No unknown content.** Unknown versions, ops, fields, extra lines or trailing
   bytes are errors, never ignored (`UnsupportedVersion`, `InvalidToken`,
   `MalformedLine`, `TrailingData`).
4. **Replay uses the live rules.** Loading calls `Apply`, so key rules, size limits
   and type rules apply to saved data. Tightening a limit in code can therefore make
   an old save unloadable — see "Rules that are part of the format" below.

### Rules that are part of the format
Changing any of these changes which saves load, so it is a format change:
key grammar `[a-z][a-z0-9_.]*` (≤128), string value ≤ 4096 bytes, cause ≤ 128 bytes,
fixed type per key, int64 range and overflow rule, "every valid command is
journaled", escape set and spelling, field order, trailer.
Loosening a limit (e.g. longer strings) keeps old saves valid but makes new saves
unreadable by old builds: still a version bump.

## 2. How v1 is protected against accidental change
- `Tests/Core/Data/worldfacts_v1_golden.spnfacts`: frozen, representative save
  (all ops, all types, escapes, UTF-8, int64 extremes, no-op set, remove-then-retype).
  Marked `-text` in `.gitattributes` so Git never rewrites its bytes.
- `WorldFactsCompatibilityTests.cpp` pins the file's size and FNV-1a hash (edited
  fixture ⇒ test fails), checks load → expected state, byte-identical re-save,
  determinism over repeated loads, that today's writer reproduces the golden bytes,
  every proper prefix rejected, single-byte corruption, structural damage with exact
  error codes, and tampered journal values.
- `static_assert(kFactsSaveFormatVersion == 1)` in those tests forces a conscious
  decision when the version changes.

**Rule:** never regenerate or edit the golden file to make a test pass. A failing
golden test means either a bug or an intentional format change (section 3).

## 3. Extension strategy for later versions (proposal)
1. **Any byte-level change ⇒ new version number** in the header (`SPNFACTS 2`).
   No optional/extension fields inside v1.
2. Add `worldfacts_v2_golden.spnfacts` and its tests; **keep the v1 file and tests**.
3. The loader dispatches on the header version and keeps one strict parser per
   supported version. Unknown versions stay `UnsupportedVersion`; no best-effort.
4. Writers always write the newest version. Reading old and re-saving is the only
   upgrade path: `Deserialize(v1)` → in-memory store → `Serialize()` (v2). Migration
   that changes *meaning* (renamed keys, retyped facts) is an explicit, separately
   tested function from old store to new store, never part of the parser.
5. Candidates that need v2 and are deliberately **not** implemented yet: snapshot /
   journal compaction, per-save metadata (save id, host/guest scope), realm or
   namespace separation, UTF-8 validation, compare-and-set for co-op, binary
   encoding. All remain OPEN.
6. Fact keys are content-defined later (world size, scale and realms are undecided);
   the format deliberately knows nothing about world dimensions.

## 4. Historical queries
`WorldFacts::StateAt(sequence)` rebuilds the store after journal record `sequence`
(0 = empty, `Revision()` = equal copy) by replaying the journal prefix into a new
store. The queried store is never modified (`const`). `sequence > Revision()` ⇒
`SequenceOutOfRange`. The returned store has its own journal prefix and can be
serialized or continued independently (a branch; the original is unaffected).
Cost is O(sequence); no caching or snapshots yet (needed only with compaction).

## 5. Limits are pinned by a second frozen fixture
`Tests/Core/Data/worldfacts_v1_limits.spnfacts` holds a save at the exact maximum key
(128), string (4096 decoded bytes, also as 4096 escaped newlines = 8192 bytes on disk)
and cause (128 bytes) lengths. `WorldFactsCompatibilityTests.cpp` pins its size and
hash, loads it, re-saves it byte-identically, checks that today's writer reproduces
it, and rejects 129 / 4097 / 129 on load (`InvalidKey`, `ValueTooLong`,
`CauseTooLong`). The golden v1 file only reaches 28-character keys, so before this
fixture a tightened limit would not have been noticed. Same rule as for the golden
file: never edit or regenerate it; a failing test means a bug or a format change.

## 6. What belongs in WorldFacts — ENGINEERING POLICY
Status: technical policy for review. **Not story canon**; it decides no gameplay
content, world size, realm model or co-op authority.

**Belongs in WorldFacts:** persistent, discrete world changes and their event
history, in the sense of the canon notes cited in `0002` (opened gates, removed
artifacts, freed/killed entities, case and evidence progress, relationship stages),
where the history of the change itself may matter later (debugging, historical
queries via `StateAt`, history rebuilt from change events).

**Belongs in separate systems:**
- High-frequency simulation, physics, positions, vehicle state, animation state:
  derived or transient, owned by the simulation / engine and saved by it.
- Large data blocks or collections: the string limit (4096 bytes) is intentional.
- Complete character appearance data: `AppearanceSpec` / `AppearanceRecipe`
  (`0005`). WorldFacts may at most hold a reference such as a character id.
- State the engine serializes anyway, and anything cheaply recomputable from facts.

**Guideline, not a rule:** every journaled command costs about 60 bytes on disk and
load time grows linearly with the journal (measured in `Docs/Proposals/0003`, one
machine, small values). A state that changes about once per second or more often is
usually a sign that it belongs elsewhere; "once per second" is an orientation, not a
threshold, and a rarely changing fact can still be wrong here while a bursty but
history-relevant one can be right. Decide per fact family.

**OPEN architecture decision — snapshot / journal compaction:** the journal is
unbounded. Compaction (snapshot plus journal tail) needs format v2 and conflicts
with "history from change events" unless it is decided which history is kept
(everything, per-key last N, milestones only). Not decided, not implemented. Also
OPEN: UTF-8 validation, per-save metadata, realm/namespace separation, co-op
authority and compare-and-set, observers/threading rules.
