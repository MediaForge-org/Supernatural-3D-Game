# M2B1 — World map and minimap foundation

Status: engineering notes/proposals, **not master canon**. Headers: `SpatialGrid.h`, `WorldMap.h`, `Minimap.h`. Tests: `Tests/Core/MapTests.cpp`, `MinimapTests.cpp`. Master basis: none found for map/minimap (see `GAME_SYSTEMS_ARCHITECTURE.md` §2). No geography is invented; all test coordinates are synthetic (`Tests/Core/MapTestData.h`).

## 1. Implemented

| Requirement | Where | Notes |
|---|---|---|
| Large world coordinates | `kMaxMapCoordinateMetres` (±10,000 km), `IsValidMapCoordinate` | doubles; offsets subtracted before narrowing |
| Regions | `MapRegionDef`, `RegionsAt` | rectangular bounds (an index, not the true outline); overlapping allowed; smallest first |
| Map bounds | `MapLayerDef::bounds` | every region, POI and marker must lie inside |
| Map layers | `MapLayerDef`, `MapLayerKind {Surface, Underground, Interior, Realm}`, `group`, `level` | surface, underground, interior floors, other realms |
| Points of interest | `PointOfInterestDef` (authored, immutable) | `MapVisibility {Hidden, Always, WhenKnown}` |
| Persistent / temporary markers | `Marker`, `MarkerPersistence` | temporary ones may carry `expiresAt`; `PruneExpired(now)` |
| Player-created waypoints | `AddPlayerWaypoint`, `PlayerWaypointCategory()` | optional `MapStateConfig::maxPlayerMarkers` (limit value is a design decision, unset by default) |
| Discovery state | `DiscoveryState {Undiscovered, Known, Visited}` | monotonic (never lowered); POIs and regions; `DiscoverPoisAround`, `DiscoverRegionsAt` |
| Marker categories | `ContentId category` | data-driven; only `base:marker.player_waypoint` is known to the Core |
| Visibility | POI rule + discovery; marker `visible` flag | `MapFilter::includeHidden` overrides |
| Filtering | `MapFilter` (categories, POIs, persistent, temporary, player, system, hidden) | |
| Queries by position/radius | `QueryRadius`, `QueryRect` | per layer, spatial index, deterministic order |
| Islands / underground / multi-level interiors | layers + regions | no single-landmass assumption anywhere |
| Stable identifiers | `ContentId` for authored entities, `MarkerId` monotonic and never reused | marker counter is saved |
| Persistence | `Serialize` / `Deserialize` (`SPN-MAPSTATE 1`) | see §6 |

Minimap (`Minimap.h`), data only — it draws nothing:

| Requirement | Where |
|---|---|
| World→minimap transform, player-centred | `MinimapTransform::WorldToScreen`, inverse `ScreenToWorld` (tap-to-place) |
| North-up / heading-up | `MinimapRotation`; `NorthOnScreenRadians()` for a compass needle |
| Configurable zoom | `MinimapView::zoom`, clamped to `[minZoom, maxZoom]` |
| Circular and rectangular viewports | `MinimapShape`, `InsideViewport`, `ProjectToViewportEdge`, `ViewportReachPx` |
| Marker visibility | the same `MapFilter` and POI rules as the map |
| Off-screen indicators | `MinimapIndicator` (edge position, angle clockwise from up, distance), optional range and inset |
| Distances | `distanceMetres` on every marker/indicator |
| Layer selection | the frame is built for `view.player.layer`; build another view to show another layer |
| Precision / large coordinates / rotation / viewport / float edges | see §3 |

## 2. Public API (essentials)

```cpp
// Definition (authored, validated once)
auto def = WorldMapDefinition::Create(layers, regions, pois, /*gridCellMetres*/ 256.0);   // Result<WorldMapDefinition, MapError>
// State
WorldMapState map(std::make_shared<const WorldMapDefinition>(std::move(def).Value()));
Result<MarkerId, MapError>  AddMarker(MarkerSpec);          AddPlayerWaypoint(MapPosition, label);
std::optional<MapError>     RemoveMarker(id);  MoveMarker(id, pos);  SetMarkerVisible(id, bool);
std::size_t                 PruneExpired(double now);
Result<bool, MapError>      Discover(ContentId, DiscoveryState);
Result<std::vector<MapQueryHit>, MapError> QueryRadius(MapPosition center, double r, const MapFilter& = {});
Result<std::vector<MapQueryHit>, MapError> QueryRect(MapLayerId, MapRect, const MapFilter& = {});
std::string Serialize() const;  static Result<WorldMapState, MapError> Deserialize(text, definition, config);
// Minimap
Result<MinimapFrame, MinimapError> BuildMinimapFrame(const WorldMapState&, const MinimapView&, const MinimapConfig&, const MapFilter& = {});
Result<MinimapTransform, MinimapError> MinimapTransform::Create(config, view);
```
`MapQueryHit` holds pointers into the state (valid until it is next modified).

## 3. Conventions and numerical notes

- World: metres, x east, y north. Heading: compass bearing in radians, clockwise from north (0 = north, π/2 = east); any finite value is normalized to [0, 2π).
- Screen (minimap output): pixels, origin = viewport centre, x right, **y down**, player at (0,0). NorthUp: north is up. HeadingUp: the heading is up; the mapping is `right = dx·cos − dy·sin`, `forward = dx·sin + dy·cos`.
- Precision: `dx = world.x − player.x` is computed in double first. Test: a player at 9,000 km produces **bit-identical** screen coordinates to a player at the origin for the same relative offset; `ScreenToWorld(WorldToScreen(p))` returns `p` within 1e-6 m at ±9,000 km over random headings/zooms.
- Viewport edge: inclusive, with `kViewportEpsilonPx = 1e-6` px tolerance so a marker exactly on the edge is not flipped by `sin/cos` rounding (test: 64 headings).
- Non-finite world/screen values are rejected (`InvalidView`) or counted in `MinimapFrame::rejected`, never drawn. A zero offset projects "up" instead of dividing by zero. Zoom outside the range is clamped; zoom ≤ 0 or non-finite is an error.
- The minimap performs **one** radius query of `max(circumscribed view radius, indicator range)` — no global scan.

## 4. Invariants (enforced and tested)

1. Ids: layer ids unique and non-null; `ContentId`s of regions and POIs share one namespace; marker ids unique, increasing, never reused (also after load).
2. Every region/POI/marker lies on a known layer and inside that layer's bounds with valid coordinates; labels ≤ 128 bytes without control characters.
3. Discovery only increases; only ids known to the definition can be discovered.
4. Temporary markers never reach the save; persistent ones round-trip exactly (doubles via shortest round-trip text).
5. The spatial index and the marker table never diverge (add, remove, move, load).
6. Query results are sorted by `(distance, POI before marker, id)`; identical input gives identical output.
7. A failed command changes nothing.

## 5. Proposed, not implemented

- Fast travel, map fog-of-war textures, polygon outlines, per-realm projections, region hierarchy, marker clustering, route overlays on the minimap, proximity auto-discovery driven by a tick, discovery sharing between co-op players.
- `maxPlayerMarkers`, discovery states, and visibility rules are proposals.

## 6. Persistence — `SPN-MAPSTATE 1`

```
SPN-MAPSTATE 1
nextmarker <n>            (>= 1, greater than every marker id)
discovery <count>
d <contentId> <known|visited>              (strictly ascending ids)
markers <count>
m <id> <layerId> <x> <y> <category> <poi|-> <player|system> <0|1> <label, %XX-encoded, '%' = empty>
end
```
Strict: canonical numbers only (`123.4567890123450` is rejected), counts must match, ids ascending, every reference validated against the definition, higher version → `UnsupportedVersion`, trailing data rejected, never partially applied. Frozen golden text is part of the test suite. Integration with the save slot: see `GAME_SYSTEMS_ARCHITECTURE.md` §5.

## 7. Test results

Files `MapTests.cpp` (spatial grid, definition, markers, discovery, queries, persistence, golden) and `MinimapTests.cpp` (transform, rotation, zoom, viewport, frames, indicators, layers, precision). Run counts: `GAME_SYSTEMS_ARCHITECTURE.md` §8. Property-style checks: grid vs brute force over 50 random queries (radius and rect, negative coordinates), insertion-order independence, rotation preserves distance, transform inverse over 300 random cases.

## 8. Performance (measured)

`spn_core_bench_m2b1`, **Release** (`-O3`, GCC 16.2.1), Intel Core Ultra 7 256V, Fedora, one run of the listed operation counts, machine otherwise idle. Single-run wall-clock means, **no statistics** (no repetitions/variance). Workloads are synthetic stress sizes spread uniformly over 2,500 km², not predictions of real marker counts. Debug numbers (the default presets) are several times slower and were not collected.

| Operation | Points / state | Time per op |
|---|---|---|
| `SpatialGrid::QueryRadius` r = 150 m | 10,000 | 0.40 µs |
| | 100,000 | 0.84 µs |
| | 1,000,000 | 9.6 µs |
| brute-force scan r = 150 m (comparison) | 100,000 / 1,000,000 | 111 µs / 1,500 µs |
| `QueryRadius` r = 2,000 m | 100,000 / 1,000,000 | 67 µs / 838 µs |
| `WorldMapState::AddMarker` | 100,000 markers | 0.77 µs |
| `AddPlayerWaypoint` + `RemoveMarker` | | 0.40 µs |
| `QueryRadius` r = 150 m (POIs+markers, filtered or not) | 20,000 POIs + 100,000 markers | 6.3 µs |
| `QueryRadius` r = 5,000 m (~3,000 hits) | same | 3.9 ms |
| `BuildMinimapFrame` (128 px circle, 1 m/px, heading-up, 500 m indicator range) | same | 51 µs |
| `WorldToScreen` | | 6.9 ns |
| `Serialize` / `Deserialize` 100,000 markers | | 64 ms (7.4 MiB) / 161 ms |

Memory (glibc `mallinfo2` delta): spatial grid ≈ 1.3 / 6.7 / 44 MiB for 10k / 100k / 1M points; 100,000 markers in `WorldMapState` ≈ 29.7 MiB (about 300 bytes each, dominated by the ordered table and label strings). Not measured: cache behaviour, concurrent access, Unreal frame cost.

Observations: the grid's advantage over a scan is ~130× at 100k points. The 6.3 µs map query is slower than the raw grid (0.84 µs) because each hit costs an ordered-table lookup, a discovery lookup (ContentId string compare) and a sort; if this ever matters, caching a visibility flag per POI and using a flat marker table are the first levers. A dense 5 km query (3.9 ms) is not a per-frame case.

## 9. Known limitations

- Region bounds are axis-aligned rectangles; `RegionsAt` is linear in the regions of a layer (fine for hundreds, not millions).
- No polygon land mask, no height, no "is this point on land".
- `MapQueryHit` pointers are invalidated by any state change; copy what you keep.
- Single-threaded; no locking.
- Marker count limit 1,000,000 and ±10,000 km are sanity bounds, not design values.

## 10. Unreal integration requirements

- Coordinate adapter (cm ↔ m, axis mapping — **undecided**, shared with WorldForge).
- A world subsystem owning `WorldMapState`; per-frame `MinimapView` from the player pawn (position, yaw as compass bearing).
- A widget that draws `MinimapFrame`; icons per category are content.
- Save-slot wiring and discovery triggers (overlap volumes calling `Discover`).
- Automation tests on Windows; none exist.

## 11. WorldForge integration requirements

- Export of layers (with bounds, group/level), regions and POIs in a form `WorldMapDefinition::Create` can ingest (stable `ContentId`s, unique across regions+POIs, positions inside layer bounds). **No file format is defined here**; the contract belongs to WorldForge (`RC-01` pre-contract is referenced in `Docs/MasterIndex.md`).
- Stable ids across WorldForge regenerations, otherwise saved discovery and markers linked to POIs go stale (unknown ids are rejected on load, never silently dropped).
- True region outlines (polygons) if hit-testing beyond bounding boxes is needed.
- Nothing in M2B1 assumes a world size, island layout or city; the larger-world workstream and `WORLD_700` lineage are untouched.
