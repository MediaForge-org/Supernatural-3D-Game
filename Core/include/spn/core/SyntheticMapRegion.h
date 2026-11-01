#pragma once

// SYNTHETIC test region for map, routing and navigation tests (and a later Unreal demonstration level).
//
// This is NOT geography, NOT a map candidate and NOT related to SPN WorldForge output: every coordinate, name and id is an arbitrary test value
// ("synthetic:" namespace). It exists so that road routing, building entrances, bridges, tunnels, one-way streets, closures, roundabouts,
// disconnected networks and unreachable targets can be tested deterministically. Real world data replaces it through the import path later.
//
// Layout (metres, x east, y north, one surface layer), all features are listed in SyntheticMapRegion.cpp:
//   * A river (water area + river line) runs north-south at x = 970..1030. Crossings: a Primary BRIDGE far north (y = 1200), a pedestrian-only
//     FOOT BRIDGE (y = -300) and a motorway TUNNEL under the river (y = -800, reached by motorway links only at x = 0 and x = 2000).
//   * West and east street networks with a one-way street, a dead-end spur, a roundabout (counter-clockwise, one-way), a turn restriction,
//     field and forest tracks, a motorway with on/off ramps.
//   * Buildings with entrances, one without an entrance, one with two entrances; forest, park, plaza, parking, industrial area, districts, a
//     mountain with a contour line, a lake, a sea with a coastline.
//   * A small hamlet network that is NOT connected to the rest (unreachable by road), and a one-tile ISLAND in the river (no accessible road).

#include <memory>
#include <spn/core/MapGeometry.h>
#include <spn/core/WorldMap.h>

namespace spn::core::synthetic {

inline constexpr std::uint32_t kLayerId = 1;

// Node ids
inline constexpr std::uint64_t kWestA = 1, kWestB = 2, kWestC = 3, kWestD = 4, kWestE = 5, kWestFootEnd = 6;
inline constexpr std::uint64_t kEastC = 11, kEastF = 12, kEastD = 13, kEastE = 14, kEastFootEnd = 15, kEastB = 16, kEastG = 17, kEastI = 18, kEastH = 19;
inline constexpr std::uint64_t kRing1 = 21, kRing2 = 22, kRing3 = 23, kRing4 = 24;
inline constexpr std::uint64_t kMotorJ1 = 32, kMotorTunnelWest = 33, kMotorTunnelEast = 34, kMotorJ2 = 35;
inline constexpr std::uint64_t kHamletA = 41, kHamletB = 42, kHamletC = 43;

// Segment ids
inline constexpr std::uint64_t kStreetWestAB = 101, kStreetWestBC = 102, kRoadWestCD = 103, kRoadWestDE = 104, kRoadWestFootApproach = 105;
inline constexpr std::uint64_t kNorthBridge = 106, kFootBridge = 107, kFootEastApproach = 108;
inline constexpr std::uint64_t kRoadEastCF = 109, kRoadEastFD = 110, kRoadEastDE = 111, kStreetEastCB = 112, kOneWayEastFG = 113, kStreetEastGB = 114;
inline constexpr std::uint64_t kStreetEastGI = 115, kStreetEastDI = 116, kStreetEastIH = 117, kStreetEastBRing = 118, kSpurEastB = 126;
inline constexpr std::uint64_t kRingSegment1 = 121, kRingSegment2 = 122, kRingSegment3 = 123, kRingSegment4 = 124, kRingExitNorth = 125;
inline constexpr std::uint64_t kMotorWestToJ1 = 131, kMotorJ1ToTunnel = 132, kMotorTunnel = 133, kMotorTunnelToJ2 = 134, kMotorJ2East = 135;
inline constexpr std::uint64_t kOnRampWest = 141, kOffRampWest = 142, kOnRampEast = 143, kOffRampEast = 144;
inline constexpr std::uint64_t kFieldTrack = 151, kForestTrack = 152, kWestSpur = 153, kHamletRoadAB = 161, kHamletRoadBC = 162;

// Building ids
inline constexpr std::uint64_t kShopWest = 1, kHouseEast = 2, kHouseNoEntrance = 3, kWarehouseTwoEntrances = 4, kHamletHouse = 5;

struct SyntheticMapRegion {
    std::shared_ptr<const WorldMapDefinition> definition;
    std::shared_ptr<const MapGeometry> geometry;
    MapLayerId layer;

    // Positions used by tests and demos (all on `layer`).
    MapPosition westStreet;     ///< (500, 0): on the west street
    MapPosition eastStreet;     ///< (1500, 0): on the east street, across the river
    MapPosition westOpenField;  ///< (250, 80): off-road, 80 m from the west street
    MapPosition shopInside;     ///< inside kShopWest
    MapPosition houseInside;    ///< inside kHouseEast
    MapPosition noEntranceInside; ///< inside kHouseNoEntrance (further than the default snap distance from any road)
    MapPosition warehouseInside;  ///< inside kWarehouseTwoEntrances
    MapPosition islandTarget;   ///< on the river island: no accessible road
    MapPosition hamletTarget;   ///< inside kHamletHouse: on a road network that is not connected
};

/// The definition (one surface layer, three regions, five POIs) the geometry is validated against.
[[nodiscard]] std::shared_ptr<const WorldMapDefinition> MakeSyntheticMapDefinition();
/// The raw feature data (nodes, segments, restrictions, buildings, areas, lines).
[[nodiscard]] MapGeometryData MakeSyntheticGeometryData();
/// Definition + validated geometry + named positions. Aborts (assert) only if the built-in data were invalid, which the tests exclude.
[[nodiscard]] SyntheticMapRegion MakeSyntheticMapRegion();

} // namespace spn::core::synthetic
