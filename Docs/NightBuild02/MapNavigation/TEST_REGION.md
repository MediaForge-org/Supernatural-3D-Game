# The synthetic test region (NightBuild02)

`Core/include/spn/core/SyntheticMapRegion.h` / `.cpp`: arbitrary coordinates in the `synthetic:` namespace. **Not geography, not a map candidate, unrelated to WorldForge.** Usable by automated tests and, later, by an Unreal demonstration level (the Core code builds the data; a level would only need an importer-style visualiser).

| Requirement | In the region |
|---|---|
| several road classes | motorway + 4 ramps, primary, secondary, residential, service, field track, forest track, footway |
| river | water area x = 970..1030 and a river line, y = -1000..1800 |
| bridge | Primary bridge (y = 1200, only vehicle crossing besides the tunnel) and a pedestrian-only foot bridge (y = -300) |
| tunnel | motorway tunnel under the river (y = -800), reachable only through the ramps at x = 0 and x = 2000 |
| buildings | shop (1 entrance), house (1 entrance), warehouse (2 entrances), house without entrance, hamlet house, a row of 8 houses |
| one-way streets | one-way street E_F -> E_G and a counter-clockwise one-way roundabout |
| turn restriction | no vehicle turn from the east south road onto the east street at E_D |
| dead ends | service spur south of the east street, a residential spur west |
| closed segment | not baked in: tests close any segment through `RouteOptions::closedSegments` / `NavigationSession::SetClosedSegments` |
| several reachable targets | positions `westStreet`, `eastStreet`, `shopInside`, `houseInside`, `warehouseInside`, `westOpenField` |
| unreachable targets | `islandTarget` (island in the river: no accessible road, `DestinationNotAccessible`), `hamletTarget` (own road network, not connected: `NoPath`), `noEntranceInside` (115 m from a road: out of reach until the snap distance is raised) |
| areas | forest, park, plaza, parking, industrial area, meadow, mountain (850 m) + contour line, lake, sea + coastline, two districts bound to regions |
| regions | `synthetic:region.west`, `.east`, `.hamlet` (discovery filter tests) |

Expected values worth knowing (asserted by tests): west street -> east street by car 3,400 m over the north bridge (1,000 m straight), on foot 1,600 m over the foot bridge, by car with the bridge closed 4,680.4 m through the tunnel and around the roundabout.
