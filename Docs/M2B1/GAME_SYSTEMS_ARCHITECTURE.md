# M2B1 — Game systems foundation: architecture overview

Status: engineering notes and proposals, **not master canon**. Written during M2B1 on the Fedora machine.
All code is portable C++20 in `Core/` (no Unreal types, no rendering, no I/O except where stated). Nothing here was run in Unreal;
**no gameplay integration is claimed**.

## 1. What M2B1 delivers

| System | Header(s) | Source(s) | Doc |
|---|---|---|---|
| Spatial index | `SpatialGrid.h` | `SpatialGrid.cpp` | `MAP_AND_MINIMAP.md` |
| World map domain | `WorldMap.h` | `WorldMap.cpp` | `MAP_AND_MINIMAP.md` |
| Minimap calculations | `Minimap.h` | `Minimap.cpp` | `MAP_AND_MINIMAP.md` |
| Inventory domain (+ persistence) | `Inventory.h` | `Inventory.cpp`, `InventoryPersistence.cpp` | `INVENTORY_FOUNDATION.md` |
| Navigation domain | `Navigation.h` | `Navigation.cpp` | `NAVIGATION_FOUNDATION.md` |

Changed existing files (additive only): `Core/CMakeLists.txt`, `Tests/Core/CMakeLists.txt` (new sources/tests/bench target),
`Core/src/TextParsing.h` (new internal helpers `PercentEncode/Decode`, `FormatDouble`, `ParseCanonicalDouble`; existing functions untouched),
`Unreal/SupernaturalProt/Tools/qa_inventory.json` and the generated `Docs/Development/Gameplay-QA-Inventory.md` (the repository's QA guard requires
every Core header to be listed; each entry states honestly that there is no Unreal integration).

## 2. Master basis (consultation per `CLAUDE.md` §11)

Searched with `Tools/master-search.sh` (tiers 1 and 2): `minimap`, `inventory`, `waypoint`, `fast travel`, `map marker`, `Karte`, `GPS`,
`smartphone`, `weight`, `Gewicht`, `storage`, `island`, `Insel`, `persistent world state`, `save`.

| # | Finding | Status as read | Source |
|---|---|---|---|
| B1 | Body inventory is physically limited; vehicles provide storage; HQ can hold very large resupplied stocks | accepted direction, **no numbers, no model** | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (player-state section), `PLANNING_STATUS_2026-09-05.md` (Block 10) |
| B2 | Smartphone ecosystem may include navigation and traffic apps; coverage/dead zones matter | accepted direction, no spec | `CANONICAL_ADDENDUM_2026-09-05.md` |
| B3 | Clothing has storage utility | accepted direction | `CANONICAL_ADDENDUM_2026-09-05.md` |
| B4 | Map candidates (2,200 km² with an eastern island, Metropolis polygon, lakes) are acceptance **specs and candidates**, not approved geography | review-only deltas | `99_DAILY_DELTAS/2026-10-19/.../docs/specs/MAP_ACCEPTANCE_CHECKS_2026-10-19.md`, `docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md` (listed in `Docs/MasterIndex.md`) |
| B5 | Modding is provider-independent with local `.spnmod`; mods can add content | confirmed direction | `01_CURRENT_CANONICAL_STATE/MODDING/...` |

**Searched, not found:** any specification of a minimap, map markers, waypoints, fast travel, map discovery, item taxonomy, stacking rules,
weight or slot numbers, route/GPS behaviour. Therefore **no gameplay value is invented**: every limit (stack sizes, weight, capacity, zoom range, view size,
indicator range, marker limits) is injected data or an explicitly marked placeholder; the Core contains no item list, no city, no coordinates.
B4 is the only reason the design makes no single-landmass assumption; the code does not read or depend on those documents.

## 3. Conventions reused (no duplicate infrastructure)

- `StrongId<Tag, Rep>` (`StrongId.h`): `MapLayerId`, `MarkerId`, `ContainerId`, `ItemInstanceId`. Null id = 0.
- `ContentId` (`ContentId.h`): stable, namespaced text ids for layers, regions, POIs, categories, item definitions, slots, container kinds. Chosen because mods can add content without an id allocator (B5).
- `Result<T,E>` (`Result.h`): no exceptions. Error codes are enums with `ToString`, appended never reordered.
- `Vec2` (`Vec.h`): metres. `FieldMap` / `IsValidFieldName` (`ObjectPersistence.h`) for per-item state.
- Text save format style of `WorldFacts` / `ObjectPersistence` ("MAGIC version", canonical spelling, strict validation, higher version rejected as `UnsupportedVersion`, never partial).
- doctest + CTest, the existing presets and warning set (`-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror`).
- Name clash found and avoided: `LayerKind` already exists in `Appearance.h`; the map type is `MapLayerKind`.

## 4. Cross-cutting design decisions (proposals for review)

1. **Definition vs state.** Authored data (`WorldMapDefinition`, `ItemCatalog`) is validated once and immutable; mutable state (`WorldMapState`, `InventoryState`) references it. Content can be replaced (WorldForge export, mods) without changing state code.
2. **Layers instead of "a world".** A `MapLayer` is an independent coordinate plane (surface, underground, one interior floor, another realm). Islands are regions/layers; multi-level interiors are layers with a shared `group` and a `level`.
3. **Coordinates:** metres, doubles, x = east, y = north, hard limit ±10,000 km (`kMaxMapCoordinateMetres`) so cell indices and squared distances stay exact. Offsets are subtracted in double before narrowing.
4. **Determinism:** all containers that are iterated or returned are ordered (`std::map`, sorted vectors); spatial query results are sorted by key; hit ordering is `(distance, source, id)`; ids are allocated monotonically and never reused; failed transactions consume no ids.
5. **Transactions:** inventory commands are plain data (`std::variant`), so they can be validated, logged, replayed and later replicated. `ApplyAll` is all-or-nothing.
6. **No authority model.** None of these APIs knows who may call them (co-op host authority is OPEN). Commands being value types keeps a host-authoritative design cheap.
7. **Persistence:** see §5.

## 5. Persistence design (investigation result)

Findings:
- `WorldFacts` stores scalar facts (`bool/int64/string`) under flat keys of at most 128 characters with a change journal. Markers (variable-length collections with positions, ids and free text) and item stacks (nested containers, equipment) do not map onto scalar facts without inventing a key schema and losing validation.
- `ObjectPersistence` stores *changed state of authored interactive objects* by catalog id and region. Map markers and inventory containers are runtime-created, not catalog objects.

Decision: **do not touch `WorldFacts` or any existing format.** Each system has its own versioned, canonical, deterministic text format, written in the same style and validated completely on load:

| Format | Class | Contains | Not saved |
|---|---|---|---|
| `SPN-MAPSTATE 1` | `WorldMapState` | discovery (POI/region), persistent markers, marker-id counter | temporary markers (runtime only), the definition |
| `SPN-INVENTORY 1` | `InventoryState` | containers + rules, stacks (+ state fields), equipment, id counter, revision | item catalog |

Both are covered by round-trip, determinism, corruption and **frozen golden-text** tests. The existing `WriteFileAtomic` / `LoadRecordsFile` (temp file, verify, `.bak`, rename) takes arbitrary text and can carry these sections.

**Not done (needs a decision before it can be validated):** a save-slot container that bundles WorldFacts, object records, map state and inventory into one atomic unit. Today each section would be written separately, so a crash between two writes could leave them inconsistent. Required integration is listed in §7.

## 6. Test and measurement summary

See each system doc and `Docs/M2B1/MAP_AND_MINIMAP.md` §8 for the benchmark table. Exact counts are in the final validation section of this file (§8).

## 7. Integration requirements

**Unreal (Windows):**
- Adapters between Unreal `FVector` (cm, UE axes) and `Vec2` metres; the axis mapping (which Unreal axis is "north") is **not defined yet** and must be decided once, together with WorldForge's frame.
- A subsystem that owns `WorldMapState` / `InventoryState`, loads the definitions from data assets, and feeds `MinimapView` from the player pawn each frame.
- UI widgets consume `MinimapFrame` (screen pixels, y down, origin = viewport centre) — no UI exists.
- Save wiring (see §5) and replication (commands are values; the replication channel is not designed).
- Placeholder pickups (`Pickup.h`, `placeholder.inventory.<holder>.<item>.count`) are untouched; a migration to `InventoryState` is a separate, documented save-compatibility event (`Docs/Decisions/0004`).
- Automation tests for any of the above; none were written or run.

**WorldForge:** see `MAP_AND_MINIMAP.md` §9 and `NAVIGATION_FOUNDATION.md` §6.

## 8. Final validation (Fedora, 2026-10-29)

Fresh configure and full rebuild (build directories deleted first) of all four presets, then CTest. GCC 16.2.1, Clang 22.1.8, `-Werror` with the project warning set.

| Preset | Build | CTest |
|---|---|---|
| `fedora-gcc` | no warnings | 446 of 447 pass |
| `fedora-clang` | no warnings | 446 of 447 pass |
| `fedora-gcc-asan-ubsan` | no warnings, **no sanitizer reports** | 446 of 447 pass |
| `fedora-clang-asan-ubsan` | no warnings, **no sanitizer reports** | 446 of 447 pass |

- The one failing CTest entry in every preset is `unreal_stub_check`. It compiles the Unreal movement sources against hand-written engine stand-ins and fails on files M2B1 did not touch (`SpnCharacterMovementComponent.*`: the stand-ins lack members the M2A4 code uses, e.g. `CalcVelocity`, `FHitResult`). It is a pre-existing failure from the pulled commits and is **not** an Unreal build.
- Doctest cases: 335 before M2B1, 426 now (+91 new in `MapTests`, `MinimapTests`, `NavigationTests`, `InventoryTests`, including two frozen-format tests). The CTest count (447) is higher than the case count because the doctest CMake discovery splits some existing test names that contain commas (the unmodified baseline already registers 354 entries for 335 cases).
- Pre-existing isolation problem, unchanged by M2B1: run as **one process**, the unfiltered `spn_core_tests` binary aborts in `InteractionSystemTests.cpp:412` (and reports two more failures, `InteractionSystemTests.cpp:247`, `GroundSteeringTests.cpp:239`) because those tests depend on state left by earlier cases; the same three pass when each runs alone, as CTest does. The identical failures reproduce on an untouched export of `ed87970`. The new tests were run both through CTest and as a group (89 cases, 11,496 assertions before the two golden tests were added).
- **CORRECTION (M2B2, 2026-10-29) — the three bullets above about CTest counts and "isolation" were wrong.** The extra CTest entries were not split at commas but at `;` (the vendored `doctestAddTests.cmake` turns the case list into a CMake list). 17 test cases whose *names* contain `;` (two of them added by M2B1: the `ApplyAll: empty list…` and `WorldMapState: add, find…` cases) were registered as name fragments that match no test case; CTest reported them as "passed" without running a single assertion. The three "isolation" failures do **not** pass when run alone: they fail deterministically on their own (they are real test-versus-code disagreements, see `Docs/M2B2/VALIDATION.md`); the earlier "pass individually under CTest" came from exactly this empty run. Run each of the 426 cases individually with a correct filter: all M2B1 cases pass. M2B2 renamed the 17 cases (name only) and added a CTest guard (`spn_core_test_names`) so this cannot recur. The "446 of 447" figures above therefore overstate what ran; see `Docs/M2B2/VALIDATION.md` for the corrected numbers.
- Python tests (`Unreal/SupernaturalProt/Tools/test_*.py`, run with `python3 -I`): 10 files, all passing, including `test_check_qa_inventory.py` after the new Core headers were registered in `qa_inventory.json`.
