#pragma once

// Navigation domain foundation: destinations, route data and route measurements. It is a DATA MODEL plus geometry on given
// polylines. It contains NO pathfinding and NO road graph: a route is either supplied by an external provider (the future
// WorldForge road graph, Unreal navigation) or is an explicitly labelled straight line (RouteSource::StraightLine) that makes no
// claim about walkability or roads.
//
// ENGINEERING FOUNDATION (M2B1), not master canon. Master consultation found no navigation, GPS or waypoint specification
// beyond "smartphone navigation/traffic apps may exist" (CANONICAL_ADDENDUM_2026-09-05.md); modes, tolerances and states below are
// proposals.

#include <cstddef>
#include <cstdint>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Result.h>
#include <spn/core/WorldMap.h>
#include <string_view>
#include <vector>

namespace spn::core {

/// Appended, never reordered. Which modes exist in the game is open; these two are needed by the foot/driving ambitions.
enum class TravelMode : std::uint8_t { OnFoot, Driving };
[[nodiscard]] std::string_view ToString(TravelMode mode) noexcept;

/// Where the route comes from.
enum class RouteSource : std::uint8_t {
    StraightLine, ///< one direct segment; NOT a path (ignores obstacles, roads, terrain)
    External,     ///< supplied by a navigation provider (road graph, navmesh); the Core only validates and measures it
};

/// What the player wants to reach. At most one reference (poi or marker) in addition to the resolved position.
struct Destination {
    MapPosition position;               ///< resolved target position
    double arrivalRadiusMetres = 0.0;   ///< within this distance (same layer) counts as arrived; 0 = exact position only
    ContentId poi;                      ///< optional: POI this destination was resolved from
    MarkerId marker;                    ///< optional (non-null): marker this destination was resolved from
    friend bool operator==(const Destination&, const Destination&) = default;
};

enum class NavigationErrorCode : std::uint8_t {
    InvalidPosition, InvalidRadius, UnknownLayer, UnknownPoi, UnknownMarker, LayerMismatch, InvalidSegment,
};
[[nodiscard]] std::string_view ToString(NavigationErrorCode code) noexcept;
struct NavigationError {
    NavigationErrorCode code;
    friend bool operator==(const NavigationError&, const NavigationError&) = default;
};

/// Resolves a destination from a POI id (position of the definition) or a marker id (current marker position).
[[nodiscard]] Result<Destination, NavigationError> DestinationFromPoi(const WorldMapState& map, const ContentId& poi, double arrivalRadiusMetres = 0.0);
[[nodiscard]] Result<Destination, NavigationError> DestinationFromMarker(const WorldMapState& map, MarkerId marker, double arrivalRadiusMetres = 0.0);
/// A free position; validated against the layer bounds of the map definition.
[[nodiscard]] Result<Destination, NavigationError> DestinationFromPosition(const WorldMapState& map, const MapPosition& position, double arrivalRadiusMetres = 0.0);

enum class SegmentKind : std::uint8_t {
    Travel,          ///< straight movement inside one layer
    LayerTransition, ///< stairs, door, ramp, elevator: from and to are on DIFFERENT layers; no planar length
};

struct RouteSegment {
    SegmentKind kind = SegmentKind::Travel;
    TravelMode mode = TravelMode::OnFoot;
    MapPosition from;
    MapPosition to;
    friend bool operator==(const RouteSegment&, const RouteSegment&) = default;
};

struct Route {
    RouteSource source = RouteSource::External;
    Destination destination;
    std::vector<RouteSegment> segments;
    /// False if the provider could only route part of the way (the last segment then need not end at the destination).
    bool complete = true;
    friend bool operator==(const Route&, const Route&) = default;
};

enum class RouteValidity : std::uint8_t {
    Valid,
    Empty,             ///< no segments
    InvalidCoordinate, ///< a non-finite or out-of-range position
    LayerMismatch,     ///< Travel across layers, or a LayerTransition inside one layer
    Discontinuous,     ///< a segment does not start where the previous one ended
    DestinationNotReached, ///< complete route whose end is not within the arrival radius of the destination
    StartMismatch,     ///< (ValidateRouteFrom only) first segment does not start at the given origin
    Incomplete,        ///< structurally fine but marked incomplete: usable as guidance, not as a finished route
};
[[nodiscard]] std::string_view ToString(RouteValidity validity) noexcept;

/// Structural validation. The first problem found, in segment order. Pure; needs no map.
[[nodiscard]] RouteValidity ValidateRoute(const Route& route);
/// As ValidateRoute, additionally requires the route to start exactly at `origin`.
[[nodiscard]] RouteValidity ValidateRouteFrom(const Route& route, const MapPosition& origin);

/// A single Travel segment from `origin` to the destination, source StraightLine. Fails if the layers differ (a straight line
/// cannot cross layers) or a position is invalid. Says nothing about whether the line can be walked or driven.
[[nodiscard]] Result<Route, NavigationError> MakeStraightLineRoute(const MapPosition& origin, const Destination& destination, TravelMode mode);

/// Length of one segment in metres: planar distance for Travel, 0 for LayerTransition.
[[nodiscard]] double SegmentLength(const RouteSegment& segment) noexcept;
/// Sum of segment lengths (0 for an empty route). Does not validate; use ValidateRoute first for untrusted routes.
[[nodiscard]] double RouteLength(const Route& route) noexcept;
/// Length travelled in one mode.
[[nodiscard]] double RouteLengthInMode(const Route& route, TravelMode mode) noexcept;

struct RouteProgress {
    std::size_t segmentIndex = 0;
    double alongSegmentMetres = 0.0;   ///< from the segment start to the projected point
    double offRouteMetres = 0.0;       ///< distance from the position to the projected point
    double remainingMetres = 0.0;      ///< from the projected point to the end of the route
};
/// Projects a position onto the Travel segments of its layer; nullopt if the route has none there or the route is empty / a
/// coordinate is not finite. The nearest projection wins; ties go to the earlier segment, so the result is deterministic.
[[nodiscard]] std::optional<RouteProgress> ProjectOntoRoute(const Route& route, const MapPosition& position);
/// True if the position is on the destination layer and within the arrival radius of the destination position.
[[nodiscard]] bool HasArrived(const Destination& destination, const MapPosition& position) noexcept;

} // namespace spn::core
