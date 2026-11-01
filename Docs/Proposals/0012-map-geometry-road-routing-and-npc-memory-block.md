# 0012 — Map geometry, road routing, waypoint navigation and the first NPC-memory block — ENGINEERING PROPOSAL

Status: written 2026-11-01 (NightBuild02). **Engineering proposal, not master canon.** Nothing under `Planning/Master` was changed. All data in tests and demos is synthetic (`synthetic:` namespace); no geography, road, building or city of the future world is defined or implied.

## 1. Master basis (consultation per `CLAUDE.md` section 11)
Searched with `Tools/master-search.sh road graph routing navigation` (tier 1/2 default) and `Docs/MasterIndex.md`.

| # | Finding | Status as read | Source |
|---|---|---|---|
| M1 | No routing, GPS, road-graph or building-entrance specification in the master. "Smartphone navigation/traffic apps may exist". | PROVISIONAL (single mention) | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (via `Docs/MasterIndex.md` section on traffic) |
| M2 | The road-graph contract of the generated map lives outside this repository (`SPN_WorldForge`, `RC-01`, an M2 pre-contract). | external / OPEN | `Docs/MasterIndex.md` (traffic bullet) |
| M3 | Final main-world footprint (about 2,000-2,200 km2), terrain, roads, settlements and city dimensions are OPEN; map candidates are not approved geography. | OPEN | `CLAUDE.md` section 3; `99_DAILY_DELTAS/2026-10-18/.../WORLD_MAP_DECISIONS_2026-10-18.md` (review-only) |
| M4 | Living world, persistent NPC memory, nemesis: binding product direction, implementation order open; first technical phase L1 = entity ids, event records, bounded memory in portable Core with tests. | CONFIRMED (product decision 2026-10-22) / proposal for the design | `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` sections 1, 4, 10 |
| M5 | Persistence tiers A-D; relationship values are hidden internal values; NPCs can be wrong; important event memories persist. | planned | `Docs/Game/Gameplay-Systeme.md` section 8; `Docs/Game/Spielwelt-und-Untergrund.md` section 13 (see 0010 table L1/L2) |
| M6 | Road/vehicle details (traffic, parking, fuel, vehicle list) are not in the master; collected as planning documentation only. | OPEN | `Docs/Game/Fahrzeuge-und-Verkehr/` |

Consequences: the road model, access rules, road classes, area kinds, connector rules and every number below are **our proposals**; nothing was derived from canon. The developer explicitly ordered map/navigation and an NPC building block for NightBuild02 (order of 2026-11-01), which is the authority for building them now; it does not ratify any value.

## 2. What was built (all in portable Core, no engine types)
| Area | Files | Summary |
|---|---|---|
| Geometry math | `GeometryMath.h/.cpp` | projection, polyline slicing, segment intersection, polygon tests, Douglas-Peucker (iterative), clipping to rect / circle |
| Features + road graph | `MapGeometry.h`, `MapGeometry.cpp`, `RoadNetwork.cpp` | roads (10 classes, bridge/tunnel/underpass, one-way, roundabout, access, turn restrictions), buildings with entrances, areas, lines, spatial indexes, validation, merge of region packages |
| Routing | `Routing.h/.cpp` | edge-based A*, closures, alternatives, building entrances, labelled connectors, explicit failures |
| Waypoint navigation | `NavigationSession.h/.cpp` | waypoint -> route, progress, off-route re-plan, marker following, closures, arrival, abort |
| Map / minimap data | `MapRender.h/.cpp` | culling, LOD, simplification, route overlay split, discovery filter |
| Test region | `SyntheticMapRegion.h/.cpp` | one complete synthetic region |
| NPC memory | `NpcMemory.h/.cpp`, `GameSave` (additive) | identities, typed events, bounded memories, hidden dispositions, deterministic time, versioned format, optional save section `world.npcs` |

## 3. Decisions that stay OPEN (not decided by this work)
Real road classes and their rules; speeds, traffic and ETA; whether connectors may cross buildings; how elevation (bridges over roads) is modelled (no z in `MapPosition`); the WorldForge import format; hierarchical routing for the full world; NPC memory model, decay constants, tier thresholds, journal/compaction question; whether `world.npcs` stays an optional section.

## 4. Derived tests
See `Docs/NightBuild02/TEST_RESULTS.md`. Tests assert the *rules of this proposal* (e.g. "no straight-line route", "connectors do not cross water"), not canon values.
