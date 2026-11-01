# Routing and navigation rules (NightBuild02)

Implemented in `Core/include/spn/core/Routing.h` and `NavigationSession.h`. Tested on Fedora; not validated under Unreal.

## Algorithm
Edge-based **A\*** over directed road segments (state = segment x direction). Edge-based search makes turn restrictions and one-way rules exact. Cost = real polyline length times an optional per-class factor (>= 1, default 1; the reported length is always the real one). Heuristic = straight distance to the nearest goal access point plus its connector (admissible because factors are >= 1). Lazy-deletion priority queue ordered by (f, goal-before-state, state index); ties therefore resolve identically on every run and platform.

## What a route is
`[start connector] + road legs + [end connector]`, packed into the existing `Route` type (source `External`, Travel segments in the chosen mode) so `ValidateRoute`, `ProjectOntoRoute`, `RouteLength` and the minimap code work on it. `PlannedRoute.legs` labels every piece; `lengthMetres == RouteLength(route)`.

* **Road legs** follow road segments only, in a direction the mode may use, not closed, respecting turn restrictions. U-turns only at dead ends.
* **Connectors** are straight legs from a free position to the road network (nearest accessible road point, or a building entrance, then the destination inside the building). Limits (`RouteOptions`, placeholders): 100 m, at most 6 candidate roads within `nearest + 5 m`, and a connector may not cross a river or coastline and may run at most 10 m through water (tolerance for a position on a bridge deck). Candidates blocked by water are skipped before the slack rule, so a nearer road across the river does not hide the roads on the own bank.
* **No air-line fallback.** Failure is an explicit `RouteFailure`: `InvalidInput`, `LayerMismatch`, `UnknownLayer`, `OriginNotOnNetwork`, `DestinationNotAccessible`, `NoPath`, `SearchLimitReached`. An air line exists only as `MakeStraightLineRoute` (`RouteSource::StraightLine`).

## Access and direction rules
| Rule | Foot | Vehicle |
|---|---|---|
| Motorway, MotorwayLink, Expressway | no | yes |
| Footway | yes | no |
| Other classes | yes | yes |
| One-way (`Forward`/`Backward`) | ignored | obeyed |
| Turn restriction | only if `vehiclesOnly == false` | obeyed |
| Tunnel / underpass as destination snap | never | never |
| Tunnel / underpass as origin snap | yes (a player inside) | yes |
| Closed segments (`closedSegments`) | blocked | blocked |

Bridges and tunnels connect to other roads **only at shared nodes**; crossing geometry without a node does not connect (tested with the river bridge, the motorway tunnel under the river).

## Waypoint behaviour (`NavigationSession`)
1. `SetWaypoint` adds a persistent player waypoint marker at the real target (replacing the session's previous one), plans the route from the player, installs it.
2. `Update(player, now, map)` projects the player onto a window of 48 route segments (full search only when lost), reports remaining distance along the route, off-route distance, arrival (radius = max(destination radius, 3 m)).
3. Off route (> 30 m): re-plan from the new position, at most every 2 s; the old route stays visible meanwhile (`offRoute = true`).
4. Marker moved -> new destination and re-plan; marker removed -> `Cancelled`; `Cancel(&map)` removes the session's waypoint.
5. `SetClosedSegments` re-plans only if the current route uses a closed segment (or there is no route). No route: `NoRoute` + `LastFailure()`, retry after the cooldown; the marker stays at the target.
6. `Generation()` increments with every route change so a UI redraws its route layer only then.

## Alternatives
`PlanRouteAlternatives(count, maxStretch=1.6)`: penalise the roads of each found route (x1.6 per use), re-plan, keep distinct routes within the stretch limit; real lengths, sorted, first = optimum.

## Complexity and measured cost (Release, one core, synthetic grids)
See `PERFORMANCE.md`. A route search touches only states near the line between the points; a short route in a 720,000-segment grid expands 28 states in 0.16 ms.
