# M2B1 — Navigation foundation

Status: engineering notes/proposals, **not master canon**. Header `Navigation.h`, source `Navigation.cpp`, tests `Tests/Core/NavigationTests.cpp`. Master basis: only B2 (smartphone navigation/traffic apps may exist; no spec) — see `GAME_SYSTEMS_ARCHITECTURE.md` §2.

**This is a data model and geometry on given polylines. It contains no pathfinding and no road network, and it does not claim to.** The future navigation graph comes from SPN WorldForge and Unreal navigation.

## 1. Implemented

| Requirement | Where |
|---|---|
| Waypoints | `MapPosition` (layer + xy); player waypoints are map markers (`WorldMapState::AddPlayerWaypoint`) |
| Destinations | `Destination` (position, arrival radius, optional POI/marker reference); `DestinationFromPoi`, `DestinationFromMarker` (follows the marker's current position), `DestinationFromPosition` (validated against layer bounds) |
| Route metadata | `Route::source` (`StraightLine` / `External`), `complete`, `destination` |
| Distance calculations | `PlanarDistance` (nullopt across layers), `SegmentLength`, `RouteLength`, `RouteLengthInMode` |
| Route segments | `RouteSegment {kind, mode, from, to}`; `SegmentKind::Travel` (one layer) and `LayerTransition` (stairs/door/elevator: different layers, no planar length) |
| Travel modes | `TravelMode {OnFoot, Driving}` per segment (a route can walk, then drive) |
| Route validity states | `RouteValidity {Valid, Empty, InvalidCoordinate, LayerMismatch, Discontinuous, DestinationNotReached, StartMismatch, Incomplete}` via `ValidateRoute` / `ValidateRouteFrom` |
| Progress along a route | `ProjectOntoRoute` → segment, distance along, distance off route, remaining distance; `HasArrived` |
| A trivial route | `MakeStraightLineRoute` — one segment, labelled `StraightLine`, rejected across layers. It makes **no** statement about walkability, roads or obstacles |

## 2. API essentials

```cpp
Result<Destination, NavigationError> DestinationFromPoi(const WorldMapState&, const ContentId&, double arrivalRadius = 0);
Result<Route, NavigationError>       MakeStraightLineRoute(origin, destination, TravelMode);
RouteValidity ValidateRoute(const Route&);   ValidateRouteFrom(const Route&, origin);
double RouteLength(const Route&);            RouteLengthInMode(const Route&, TravelMode);
std::optional<RouteProgress> ProjectOntoRoute(const Route&, const MapPosition&);
bool HasArrived(const Destination&, const MapPosition&);
```

## 3. Invariants and rules

- Validation order is fixed (first problem in segment order; destination checks last): `Empty` → `InvalidCoordinate` / `LayerMismatch` / `Discontinuous` per segment → destination radius/coordinate → `Incomplete` → `DestinationNotReached`.
- Continuity is **exact** (`segments[i].to == segments[i+1].from`); a provider must emit shared points.
- A `Travel` segment stays on one layer; a `LayerTransition` changes layer. The destination counts as reached only on its own layer within its arrival radius.
- `ProjectOntoRoute` considers `Travel` segments on the position's layer, picks the nearest projection, ties go to the earlier segment; degenerate (zero-length) segments are safe.

## 4. Proposed, not implemented

Pathfinding, road graph queries, traffic, route cost/ETA (needs mode speeds — tuning values are not approved), rerouting policy, turn-by-turn instructions, GPS/phone UI and coverage dead zones (B2), fast travel. None of these is designed here.

## 5. Test results

`NavigationTests.cpp`: destinations from POI/marker/position (including a moved and a removed marker, bad radii, bounds); straight-line route and its refusals; each `RouteValidity` state; start mismatch; lengths per segment kind and mode, no cross-layer distance; progress/remaining distance on a two-layer route including clamping past the end; degenerate segment; arrival by layer and radius. Counts: `GAME_SYSTEMS_ARCHITECTURE.md` §8. No performance measurement (the operations are linear in the route's segment count and not a hot path); the cost of `QueryRadius` that feeds destinations is in `MAP_AND_MINIMAP.md` §8.

## 6. Requirements for the real implementation

**WorldForge:** a road/path graph with stable node ids, per-edge travel mode(s), one-way and layer-transition edges, a documented layer/id contract matching `MapLayerDef`. (The `RC-01` road-graph pre-contract referenced in `Docs/MasterIndex.md` lives outside this repository and was not consulted for a format.)

**Unreal:** a provider that turns the engine's navigation query result into `Route::segments` (cm → m, axis mapping), marks it `External` and `complete` or not; a vehicle-aware provider for `Driving`; a UI that consumes `RouteProgress`. Re-validation on every new route with `ValidateRouteFrom` against the actual pawn position.

## 7. Known limitations

- Routes are not persisted (they are derived; the destination marker is persisted by the map state).
- `HasArrived` uses planar distance only (no height/floor tolerance beyond the layer).
- Straight-line routes ignore everything; they exist to test the model and as an explicit placeholder.
