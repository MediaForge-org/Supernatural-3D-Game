#pragma once

// Road routing on a MapGeometry: A* over directed road segments (edge-based, so turn restrictions and one-way streets are exact).
//
// What a route is: [start connector] + road legs + [end connector].
//   * Road legs follow road segments only, in a direction the travel mode may use (one-way rules, access by road class, closures, turn
//     restrictions). Their length is the real polyline length.
//   * A connector is a short straight leg between a free position and the point where it enters the road network (the nearest accessible road
//     point, or a building entrance). Its length is limited (RouteOptions::max*SnapMetres) and it may not cross water (rivers, coastlines,
//     water areas), so a connector can never become a shortcut across a river. Connectors are labelled in RouteLeg::kind.
//   * There is no straight-line fallback. If no route exists, PlanRoute fails with an explicit RouteFailure. An air line is only available through
//     MakeStraightLineRoute (Navigation.h), whose RouteSource::StraightLine marks it as such.
//
// Determinism: ties between equal-cost routes are broken by (cost, kind, state index) with indices following the id order of the sorted network,
// so the same data and options give the same route on every platform and run.
//
// ENGINEERING FOUNDATION (NightBuild02), not master canon. Costs are plain metres (optionally scaled per road class by a factor >= 1). No speeds,
// traffic or ETA: those would be unapproved tuning values.

#include <array>
#include <cstddef>
#include <cstdint>
#include <set>
#include <spn/core/MapGeometry.h>
#include <spn/core/Navigation.h>
#include <string_view>
#include <vector>

namespace spn::core {

struct RouteOptions {
    TravelMode mode = TravelMode::OnFoot;
    std::set<RoadSegmentId> closedSegments; ///< blocked for this search (roadworks, scripted closures); the network itself stays immutable
    /// Largest straight connector from the origin / the destination access point to the road network. Placeholder values, not tuned.
    double maxStartSnapMetres = 100.0;
    double maxDestinationSnapMetres = 100.0;
    /// A connector may run at most this far through water (a position on a bridge deck lies over a Water area). Placeholder, not tuned.
    double maxWetConnectorMetres = 10.0;
    /// Only roads within (nearest road distance + this slack) are access candidates. Without it a start ON a road could take a straight connector
    /// across a roundabout or a field to a farther road - a hidden air-line shortcut. Placeholder, not tuned.
    double snapSlackMetres = 5.0;
    std::size_t maxSnapCandidates = 6; ///< road segments considered per access point (nearest first)
    /// Cost multiplier per road class (index = RoadClass). Each factor must be finite and >= 1 (keeps the A* heuristic admissible); 1 = pure distance.
    std::array<double, kRoadClassCount> classCostFactor = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    std::size_t maxExpansions = 5'000'000; ///< search budget; exceeding it fails with SearchLimitReached
};

enum class RouteFailure : std::uint8_t {
    InvalidInput,            ///< non-finite or out-of-range position, invalid options
    LayerMismatch,           ///< origin and destination are on different layers (a road route cannot change layer)
    UnknownLayer,            ///< the layer has no road data
    OriginNotOnNetwork,      ///< no accessible road within reach of the origin (or only across water)
    DestinationNotAccessible,///< no accessible road / building entrance within reach of the destination (or only across water)
    NoPath,                  ///< both ends connect to the network but not to each other (islands, closures, one-way rules)
    SearchLimitReached,
};
[[nodiscard]] std::string_view ToString(RouteFailure failure) noexcept;

struct RouteStats {
    std::size_t startCandidates = 0;
    std::size_t goalCandidates = 0;
    std::size_t statesExpanded = 0;
    std::size_t statesPushed = 0;
    friend bool operator==(const RouteStats&, const RouteStats&) = default;
};

struct RouteError {
    RouteFailure code;
    RouteStats stats;
    friend bool operator==(const RouteError&, const RouteError&) = default;
};

enum class LegKind : std::uint8_t { StartConnector, Road, EndConnector };
[[nodiscard]] std::string_view ToString(LegKind kind) noexcept;

/// A run of `Route::segments` belonging together. Road legs describe one (possibly partial) road segment.
struct RouteLeg {
    LegKind kind = LegKind::Road;
    RoadSegmentId road;        ///< Road legs only
    RoadClass roadClass = RoadClass::Residential;
    RoadStructure structure = RoadStructure::Surface;
    std::size_t firstSegment = 0; ///< index into Route::segments
    std::size_t segmentCount = 0;
    double lengthMetres = 0.0;
};

struct PlannedRoute {
    Route route; ///< source External; passes ValidateRoute; starts at the origin and ends exactly at the destination position
    std::vector<RouteLeg> legs;
    double lengthMetres = 0.0;           ///< whole route (connectors included) = RouteLength(route)
    double roadLengthMetres = 0.0;
    double connectorLengthMetres = 0.0;
    double straightLineMetres = 0.0;     ///< origin -> destination as the crow flies, for comparison only
    BuildingId destinationBuilding;      ///< non-null if the destination lies in a building and an entrance was used
    Vec2 accessPoint;                    ///< where the route leaves the road network (the entrance or the snapped road point)
    RouteStats stats;
};

[[nodiscard]] Result<PlannedRoute, RouteError> PlanRoute(const MapGeometry& geometry, const MapPosition& origin, const Destination& destination,
                                                          const RouteOptions& options);

/// Up to `count` different routes, best first (the first is what PlanRoute returns). Alternatives are found by penalising the roads of the routes
/// found so far; their lengths are real lengths. An alternative longer than `maxStretch` x the best route is dropped. Deterministic.
[[nodiscard]] Result<std::vector<PlannedRoute>, RouteError> PlanRouteAlternatives(const MapGeometry& geometry, const MapPosition& origin,
                                                                                   const Destination& destination, const RouteOptions& options,
                                                                                   std::size_t count, double maxStretch = 1.6);

/// True if any road leg of the route uses a segment of `closed`.
[[nodiscard]] bool RouteUsesAny(const PlannedRoute& route, const std::set<RoadSegmentId>& closed);

} // namespace spn::core
