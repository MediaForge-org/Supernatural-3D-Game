# 0002 — WorldFacts (Phase 0.2) — ENGINEERING PROPOSAL

Status: implementation proposal for developer review. **Not master canon.** It
fixes no gameplay value, no map/world decision and no co-op/network authority.

## Canon basis (read-only, from `Planning/Master`)
- Major actions persist and do not casually reset: opened gates, removed
  artifacts, changed locations, freed/killed entities
  (`01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md`).
- Historical world state is to be rebuilt from change events rather than full
  per-year copies (`CANONICAL_ADDENDUM_2026-09-05.md`). This motivates the journal;
  time travel itself is **not** implemented or presupposed here.
- Each realm needs its own persistence rules (09-04/09-06): WorldFacts has no
  notion of realms; separation is left to key naming / multiple stores (OPEN).
- Deterministic persistence tests are asked for in the 2026-10-17 delta notes
  (non-canon proposal document).
- CLAUDE.md: host-save-authoritative co-op is a direction; networking is OPEN.

## What it is
`spn::core::WorldFacts`: named facts (`bool`, `int64`, `string`), changed only by
explicit commands (`SetFact`, `RemoveFact`, `AdjustInt`), each applied atomically
and recorded in a gapless journal (`ChangeRecord`: sequence, command, previous,
current, free-form cause). Errors are values (`Result<T, FactError>`), no exceptions.

## Proposals to review (all changeable)
1. **Key rule:** `[a-z][a-z0-9_.]*`, 1..128 chars. Dots are only a naming convention.
2. **Type is fixed per key** while it exists (Remove, then Set to change type).
3. **Limits:** string value 4096 bytes, cause 128 bytes (placeholders, untuned).
4. **Every valid command is journaled**, even if the value is unchanged; failed
   commands leave state and journal untouched.
5. **Event-log persistence:** the save file is the journal; state is derived by
   replay and every record's previous/current is verified against the replay.
   No separate snapshot section, so file and state cannot disagree.
6. **Canonical text format v1:** `SPNFACTS 1`, tab-separated `change` lines,
   `end<TAB><count>` trailer, `\n` line ends. The loader only accepts the exact
   spelling the writer produces (re-encoding check), so damaged data is never
   silently normalized. Truncation, trailing data and count mismatch are errors.
7. **Strings:** bytes are stored as-is (UTF-8 is *not* validated); control
   characters are escaped.
8. **No authority model:** anyone holding the store may apply commands. `cause`
   is audit text only and carries no permission semantics.

Format rules, compatibility strategy and historical replay (`StateAt`) are documented
in `0004-world-facts-save-format-v1.md`.

## Known limitations / OPEN
- Journal grows without bound; snapshot/compaction would need format v2.
- No UTF-8 validation, no per-save metadata (save id, host/guest scope).
- No conflict handling or compare-and-set (relevant for co-op; undecided).
- No namespaces per realm / per save scope; no mod-defined fact registration.
- Text vs. binary format and size limits are provisional.
