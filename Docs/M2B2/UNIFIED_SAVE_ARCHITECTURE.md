# M2B2 — Unified Save-Slot Architecture

Status words (**Implemented / Tested / Proposed / Unverified**) are defined in `WORLD_COORDINATE_CONVENTIONS.md`.

Code: `Core/include/spn/core/SaveSlot.h`, `SaveStore.h`, `GameSave.h` and the matching `Core/src/*.cpp`.
Tests: `Tests/Core/SaveSlotTests.cpp`, `GameSaveTests.cpp` (helper `SaveTestFs.h`), golden file `Tests/Core/Data/saveslot_v1_golden.spnsave`.
Hand-run tool: `spn_core_save_probe <dir>` (`Tests/Core/SaveProbe.cpp`).

## 1. Master basis and existing state

| Finding | Source | Status |
|---|---|---|
| Co-op: up to four players, **host-save-authoritative**, guest progress scoped to the host save | `CLAUDE.md` section 6 | direction; network design still to be reviewed |
| Mods: per-save activation; **critical missing mods block save loading by default** | `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_RECONSTRUCTED_2026-09-09.md`, `…_BLOCK_05_CONFIRMED_2026-09-11.md` | confirmed direction (rules text); no save format defined |
| WorldFacts v1 save format, frozen | `Docs/Decisions/0004-world-facts-save-format-v1.md` | our decision |
| Open: host/guest save metadata, compare-and-set for co-op, binary format, journal growth | `99_DAILY_DELTAS/2026-10-19/.../WORLDFACTS_PHASE_0_3_2026-10-19.md`, `OPEN_QUESTIONS_2026-10-19.md` | review-only delta, open |
| Nothing in the master specifies a save-slot container, atomic publication or a save UI | `Tools/master-search.sh save slot autosave host-authoritative` | none found |

Everything below is therefore an engineering **proposal**. The container is designed so that the two confirmed directions fit later without a format change: co-op guest data and active-mod lists become *additional sections*, and "critical mod missing blocks loading" is exactly the semantics of a **required** section that a build cannot read.

### Reused, not duplicated

| Existing piece | Reuse |
|---|---|
| `WorldFacts::Serialize/Deserialize` (journal, v1) | embedded verbatim as section `world.facts` |
| `SerializeRecords/ParseRecords` (object records), `RegionStateCache::Snapshot` | embedded verbatim as `world.objects` |
| `WorldMapState::Serialize/Deserialize` (`SPN-MAPSTATE 1`) | embedded verbatim as `world.map` |
| `InventoryState::Serialize/Deserialize` (`SPN-INVENTORY 1`) | embedded verbatim as `player.inventory` |
| `Result<T,E>`, `TextParsing.h` canonical-number parsing | used by the new parser |
| `WriteFileAtomic` (object persistence) | **not** reused: it validates the backup with the object-record parser, so it cannot serve other formats. The new `SaveSlotStore` generalizes the same idea (tmp file, read-back, `.bak`, rename). `WriteFileAtomic` is untouched. |

## 2. Container format `SPN-SAVESLOT 1` — **Implemented, Tested**

```
SPN-SAVESLOT 1\n
sections <N>\n
section <id> <version> <req|opt> <payload-bytes> <crc32 of payload, 8 lowercase hex>\n
<payload-bytes raw bytes>\n                    repeated N times, ids in strictly ascending byte order
end <crc32 of every byte before this line, 8 lowercase hex>\n
(end of file; any further byte is an error)
```

* Section id: dotted lowercase segments `[a-z0-9_]`, 1..64 characters (`IsValidSectionId`). Version ≥ 1. Numbers are canonical decimals (no `+`, no leading zeros). `req` = a reader that cannot interpret this section must refuse the save; `opt` = it may keep it and ignore it.
* Payloads are opaque bytes (NUL, newlines and text that looks like a header are fine: the length is authoritative). Limits: 1,024 sections, 256 MiB per payload (parser limits against absurd input, not design targets).
* **Deterministic:** sections are stored in a `std::map` and written in ascending id order; the same sections always produce the same bytes (tested, also with different insertion orders). **Canonical:** the parser accepts nothing but this exact spelling, so `Serialize(Parse(x)) == x` for every accepted `x`.
* Integrity: CRC-32 (IEEE 802.3, zlib polynomial). It detects accidental corruption: every strict prefix of a valid file is rejected, and every single-byte flip of a sample file is rejected (both exhaustively tested). It does **not** protect against deliberate tampering, and no encryption or compression is used — no need was shown. `crc32("123456789") == 0xCBF43926` is tested.
* Frozen: `saveslot_v1_golden.spnsave` was produced by an independent Python script (`zlib.crc32`), not by this code; the test parses it and requires a byte-identical re-serialization. `.gitattributes` marks `*.spnsave` binary so line-ending normalization can never alter it.

### Error reporting

`SaveError{code, detail}`; codes: `BadHeader`, `UnsupportedContainerVersion`, `MalformedLine`, `Truncated`, `BadLength`, `SectionChecksumMismatch`, `TrailerChecksumMismatch`, `TrailingData`, `CountMismatch`, `InvalidSectionId`, `DuplicateSection`, `SectionOrder`, `MissingRequiredSection`, `UnsupportedSectionVersion`, `UnsupportedRequiredSection`, `SectionRejected`, `MissingContext`, `ConsistencyMismatch`, `Io`, `VerifyFailed`, `NoValidSave`. `detail` names the section; logic must switch on `code`.

### Forward compatibility rules (`SaveSlot::Validate(schema)`)

| situation | result |
|---|---|
| required section of the schema missing | `MissingRequiredSection` |
| unknown **optional** section | accepted, kept in the slot (a rewrite preserves it), listed by `UnreadSections` |
| optional section with a **newer** version than the build reads | accepted, kept, not interpreted |
| unknown **required** section | `UnsupportedRequiredSection` |
| known required section with a newer version | `UnsupportedSectionVersion` |
| container version ≠ 1 | `UnsupportedContainerVersion` |

## 3. Game sections and snapshot semantics — **Implemented, Tested**

| section | required | payload | note |
|---|---|---|---|
| `world.facts` | yes | `WorldFacts::Serialize()` | the whole fact store incl. journal; object facts live in it |
| `world.objects` | no | `SerializeRecords(RegionStateCache::Snapshot(...))` | adds what the live facts do **not** hold: state of unloaded regions and orphans (tested: a retained door appears here but not in `facts`) |
| `world.map` | no | `WorldMapState::Serialize()` | `SPN-MAPSTATE 1`, unchanged |
| `player.inventory` | no | `InventoryState::Serialize()` | `SPN-INVENTORY 1`, unchanged |
| `slot.meta` | no | `facts_revision N\ninventory_revision N\n` | which gameplay moment the sections were taken at |

"Optional" means *absent = that domain was never saved*; a fresh state is created by the caller. Whether a shipped game requires a map section is a **Proposed** policy decision, not a container limitation (change `GameSaveSchema()`).

**Snapshot boundary.** `CaptureGameSave(const GameSaveSources&, const SaveSlot* previous)` takes only `const` pointers and returns a value: it cannot mutate live state (tested: all domain serializations are identical before and after). It is synchronous, so all sections describe the same instant **provided no other thread mutates the state meanwhile** — the Core is single-threaded; a threaded host must capture on the game thread. As a guard it reads the facts and inventory revisions before and after serialization and returns `ConsistencyMismatch` if either moved (for example an event listener that changed state mid-capture). Limitation: `WorldMapState` has no revision counter, so the guard does not cover the map.

**Load is two-phase.** `ParseGameSave(slot, context)` builds *new* objects (`LoadedGame`) and touches nothing live; the caller swaps them in only if it succeeded. `GameLoadContext` supplies what a section needs but does not contain: the `WorldMapDefinition` (and map config) and the `ItemCatalog`. A present section without its context yields `MissingContext` (never a guess). A domain parser rejecting a payload yields `SectionRejected` with the section id (tested for all four domains with a *valid checksum* around garbage, i.e. container-level integrity cannot hide domain-level corruption). `slot.meta` is cross-checked against the revisions found in the sections; sections spliced from different moments are rejected with `ConsistencyMismatch` (tested).

**Carry-over.** `CaptureGameSave(sources, &previous)` copies every section the build cannot read (`UnreadSections`) from the previous slot, so saving with an older build does not drop newer optional data (tested). Sections the build writes itself are never overridden.

## 4. Atomic publication — `SaveSlotStore` — **Implemented, Tested (logic + fault injection); durability Unverified**

Files next to the main path: `<base>`, `<base>.prev`, `<base>.tmp`, `<base>.prev.tmp`.

`Save(slot, schema)`:

1. `slot.Validate(schema)` — a slot without its required sections is never published.
2. `Remove(tmp)`; `WriteSynced(tmp, bytes)` (write, flush, fsync); read back and compare; a mismatch aborts with `VerifyFailed` and removes the tmp file.
3. If `<base>` exists **and parses as a container**: `WriteSynced(prev.tmp, old bytes)`, `Rename(prev.tmp → prev)`. A corrupt main file never replaces a good `.prev` (tested).
4. `Rename(tmp → base)` — the single publishing step; then `SyncDirectory`. If that last flush fails, `Save` returns `Io` with detail starting `"not durable"`: the new save is visible, durability is not guaranteed.

`Load(schema)`: main file → on any *corruption-type* error the previous generation → else `NoValidSave`. **Exception:** if the main file is intact but newer than this build understands (unsupported container/section version, unknown required section) it is reported, **not** silently replaced by an older generation (tested) — that would roll the player back unnoticed. A leftover `.tmp` is never loaded (it may be incomplete or superseded) and is replaced by the next save (tested).

**Invariant:** once any save has succeeded, at every instant at least one of `base` and `base.prev` is a complete, valid save (the main file is only ever replaced by a rename of a fully written, flushed, verified file).

### Fault-injection results (in-memory filesystem, deterministic)

`Tests/Core/SaveTestFs.h` counts every mutating operation and breaks the N-th in four ways: `Error` (no effect), `ErrorAfterEffect` (effect happened, error reported), `Crash` (power loss before the operation; later operations fail), `TornCrash` (a write persists only its first half, then power loss). A save over an existing one has **6** mutating operations, the first save **4**. The tests run every mode at every operation, "reboot", `Load`, and require **exactly the old or exactly the new save, never anything else**, then a fresh save on top of the leftovers must succeed. Covered: first save (old state = nothing, so "no save" or the complete new save), save over one generation, save over two generations — 4 × (6 + 6 + 4) = 64 interruption points. All pass. This proves the *protocol's logic*; it does not prove a real disk behaves like the simulation.

### Filesystem reality — **measured**, not assumed

`ProbePublishCapabilities` / `spn_core_save_probe <dir>` runs the protocol's primitives once (write+flush, rename onto an existing file, directory flush) and reports:

| volume | `statfs` | rename replaces existing | file flush ok | directory flush ok |
|---|---|---|---|---|
| repository volume (`/mnt/Festplatte/...`, NTFS via FUSE) | `0x65735546` FUSE (NTFS-3G: durability depends on the user-space driver) | yes | yes | yes |
| `~/.cache` | btrfs | yes | yes | yes |
| `/tmp` | tmpfs (memory only) | yes | yes | yes |

(Run on this machine on the day of the milestone; results are not a promise for other mounts.) **What this does and does not mean:** the primitives behave as the protocol needs. A flush cannot be *observed*, so nothing here proves power-loss safety — least of all on a FUSE NTFS driver, and tmpfs has no durability at all. **We do not claim crash-safe atomic persistence on the NTFS repository volume.** The Core tests that use the real filesystem (`StdSaveFileSystem`) write to `std::filesystem::temp_directory_path()` (here: tmpfs) and check logic only. Real durability must be judged on the target (Windows NTFS) with a platform test.

Windows: `StdSaveFileSystem` uses `std::filesystem::rename` (replace semantics on MSVC's STL) and `_commit` for the file flush; `SyncDirectory` is a **documented no-op** there (no portable equivalent; `MoveFileEx(MOVEFILE_WRITE_THROUGH)` would be the route). This code path was **never compiled or run on Windows** (**Unverified**).

## 5. Compatibility guarantees — **Tested**

* No existing serialized representation changed. The inventory change of this milestone (cached totals, `INVENTORY_PERFORMANCE.md`) is runtime-only; `SPN-INVENTORY 1` bytes are identical (the M2B1 frozen-format tests run unchanged and pass).
* Pre-container payloads (the bytes `Serialize()` produced before M2B2) are accepted as section payloads (test "existing formats stay readable"); a slot without `slot.meta` loads (older writers need not provide it).
* `WorldFacts` journal, object records, `SPN-MAPSTATE 1`, `SPN-INVENTORY 1`: unchanged code paths and goldens.

## 6. Known limitations

* No migration framework: a section version bump needs a reader for the old version; none exists yet because only version 1 exists.
* CRC-32 only; no tamper protection, encryption or compression (not needed so far).
* Whole-file writes: a very large facts journal makes every save rewrite everything (journal growth/compaction is an open question in the master delta).
* No cross-slot features: slot naming, directory layout, autosave rotation, screenshots/thumbnails, play-time metadata — all absent.
* Map snapshot guard missing (no map revision counter).
* Thread safety: none (single-threaded Core).
* Durability on real hardware and on Windows: unverified.
* Host/guest (co-op) scoping and mod-list sections: not designed beyond leaving room for them.

## 7. Open design decisions (not made here)

1. Which sections are *required* in a shipped game (today only `world.facts`).
2. Slot directory layout, file naming, number of generations kept (today: current + one previous), autosave policy.
3. Whether the object section stays redundant with the facts section or the facts section is split into world/objects (today both exist; they are taken from the same instant).
4. Co-op: how guest progress is scoped inside a host slot (extra sections with a guest-id namespace?).
5. Mods: section(s) for active mods; "critical missing mod blocks load" maps to a `req` section.
6. Binary encoding or compression, if file size ever matters.

## 8. Requirements for the next integrations

**Windows / Unreal (Unverified until done):** compile `SaveStore.cpp` with MSVC/clang-cl (`<io.h>`, `_commit`); test `StdSaveFileSystem` on NTFS including a forced kill during a save; decide the save directory (`FPaths::ProjectSavedDir()`); connect `Spn.SaveWorld` / `Spn.LoadWorld` to `CaptureGameSave` / `SaveSlotStore` / `ParseGameSave` (not done — the game's existing save commands are unchanged); load the swap-in only after `ParseGameSave` succeeded.
**WorldForge:** none directly. If it ships a world definition, `WorldMapDefinition` must be built from it *before* `ParseGameSave` (the map section depends on the definition's layers/ids).
