# Map geometry data model (NightBuild02)

Status: **implemented and tested on Fedora (clang 22, Debug, ASan/UBSan)**; **unconfirmed under Unreal / MSVC**. Engineering proposal (`Docs/Proposals/0012`), synthetic data only.

## Relation to the existing map
`WorldMapDefinition` / `WorldMapState` (layers, regions, POIs, markers, discovery, minimap transform) are **unchanged**. `MapGeometry` is a second, immutable object that is *validated against* a `WorldMapDefinition` (known layer ids, features inside the layer bounds, known region ids). There is no competing map system: markers, discovery and the minimap transform keep working exactly as before and are reused by the new code.

## Types (`Core/include/spn/core/MapGeometry.h`)
| Type | Meaning |
|---|---|
| `RoadNodeId`, `RoadSegmentId`, `BuildingId`, `MapAreaId`, `MapLineId` | stable ids (`StrongId<uint64>`), 0 = null, assigned by the data source, never reused |
| `RoadNodeDef` | node: id + `MapPosition` (layer + x/y metres, double) |
| `RoadSegmentDef` | segment between two nodes: class, structure, direction, roundabout flag, optional access override, intermediate `shape` points, optional region + name. **Length = polyline length**, never the node distance |
| `RoadClass` | Motorway, MotorwayLink, Expressway, Primary, Secondary, Residential, Service, FieldTrack, ForestTrack, Footway |
| `RoadStructure` | Surface, Bridge, Tunnel, Underpass (tunnel/underpass cannot be entered mid-span from outside) |
| `RoadDirection` | Both, Forward, Backward - applies to **vehicles**; pedestrians ignore one-way rules |
| `TurnRestriction` | forbids segment A -> segment B at a node (vehicles only or all) |
| `BuildingDef` | footprint ring, entrances (must lie within 1 m of the footprint), category, region, label, height |
| `AreaDef` / `AreaKind` | Water, Island, Forest, Park, Green, Industrial, Parking, Plaza, District, Mountain (+ optional elevation) |
| `LineFeatureDef` / `LineKind` | River, Stream, Coastline, Contour (+ optional elevation) |
| `MapGeometryData` | raw importer output; order irrelevant |
| `RoadNetwork` | validated graph: sorted nodes/segments, incident lists, per-layer spatial index, restriction table |
| `MapGeometry` | roads + buildings + areas + lines, per-layer `BoxGrid` indexes, queries |

Coverage of the requested elements: motorways/links/expressways/primary/secondary/residential/service/field/forest/footways, junctions (degree >= 3), roundabouts (one-way ring), bridges, tunnels, underpasses, one-way, dead ends (derived), building footprints + entrances, districts, rivers, lakes, coasts, forests, parks, green areas, mountains with elevation, islands, parking, plazas, industrial areas. Interior floors / layer transitions in routing are **not** implemented (see KNOWN_ISSUES).

## Validation (`GeometryErrorCode`)
InvalidId, DuplicateId, UnknownLayer, OutsideLayerBounds, InvalidCoordinate (non-finite or beyond +-10,000 km), UnknownNode, UnknownSegment, LayerMismatch, DegenerateGeometry (zero length), InvalidPolygon (non-simple / zero area), InvalidEntrance, InvalidAccess, UnknownRegion, InvalidLabel, TooManyPoints, RestrictionNotAdjacent, MergeConflict, InvalidGridCell. `Create` never partially succeeds.

## Determinism, scale
* Everything is sorted by id on creation; shuffled input gives identical stored order, routes and statistics (tests).
* Doubles in metres; every query subtracts in double. A shifted copy of the synthetic region at +5,000 km / +3,000 km routes and renders identically (tests).
* Regional data: `MergeGeometryData(a, b)` merges region packages; equal ids must be identical (border nodes may be present in both). The merge of a west/east split reproduces the original data bit for bit and routes across the border (test).
* Spatial index: `BoxGrid` (uniform grid over bounding boxes, huge features in an always-checked list). Segment lookups for snapping and rendering touch only cells near the query.
