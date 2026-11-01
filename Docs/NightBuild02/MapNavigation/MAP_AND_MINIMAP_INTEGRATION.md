# Map and minimap data flow, and what Unreal still needs (NightBuild02)

## Implemented in Core (`MapRender.h`)
`BuildMapRenderFrame(geometry, mapState, view, viewport, config, routeOverlay)` returns `MapRenderFrame`: screen-space polylines/polygons in draw order (areas -> lines -> buildings -> roads; roads least important first, tunnels below surface below bridges), the player's heading arrow, the route split into `routeTravelled` / `routeRemaining` (clipped to the circle or rectangle viewport), the true destination marker position and, if it is off screen, an edge indicator. It reuses `MinimapTransform`: zoom, NorthUp/HeadingUp rotation and double-precision world offsets behave exactly like the existing minimap.

Per frame it does not scan the world: spatial-index query for the viewport, then LOD cull (per road class / area kind / buildings, by metres per pixel), discovery cull (features tied to an Undiscovered region), screen cull, Douglas-Peucker in pixel space, and per-kind budgets (`truncated` flag). Defaults are placeholders.

Measured on synthetic grids: 0.17 ms per frame (about 250 features) independent of grid size (19,800 to 718,800 segments), Release build.

## Wiring a UI to it (not done, needs Unreal / developer decisions)
1. `USpnMapSubsystem` owns a `NavigationSession` (needs the `MapGeometry` as a loaded asset or world-partition cell data) and exposes SetWaypoint / Cancel / Update to Blueprint and to `SpnMapScreen`.
2. `SpnMapScreen` / `SpnMapView`: on a click convert screen -> world with `MinimapTransform::ScreenToWorld`, call `SetWaypoint`; draw `MapRenderFrame` (UMG `OnPaint` / `FSlateDrawElement::MakeLines`, polygons as triangulated meshes or `MakeCustomVerts`).
3. `SpnHudWidget` minimap: build a `MinimapView` from the pawn (position via `WorldToUnreal` inverse, heading via `UnrealYawDegreesToBearing`), call `BuildMapRenderFrame` when the player moved more than a pixel or the session `Generation()` changed.
4. Save: pass `GameSaveSources::npcMemory` in `SpnSaveSubsystem`; the active navigation itself is runtime only (the waypoint marker already persists).
5. Data: an importer that turns WorldForge output (road graph contract `RC-01`) into `MapGeometryData`, region by region; `MergeGeometryData` for cross-region routing.
All of this is **unconfirmed under Unreal**; the Core code compiles and is tested only with clang on Fedora.

## Known gaps
No label/text placement, no icon atlas, no fog-of-war mask texture (only region-level discovery), no road-name rendering, no turn-by-turn instructions. No Unreal types were added.
