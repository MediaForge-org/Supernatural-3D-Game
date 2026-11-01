#pragma once

// NavigationSession: the player's active route. It ties a destination (usually a map waypoint) to a PlannedRoute on the road network and keeps
// it current while the player moves:
//   SetWaypoint / SetDestination  plan a road route from the player to the target (explicit failure if there is none)
//   Update                        progress along the route, remaining distance, off-route detection, re-planning, arrival, marker following
//   SetClosedSegments             a road closes or opens: re-plans when the current route is affected
//   Cancel                        route abort
// The target marker stays at the real target; the route leads to the access point (road point or building entrance).
//
// Cost per Update: one projection over a small window of route segments (not the whole route, not the world); a re-plan only when the player
// left the route (with a cooldown) or the target / closures changed.
//
// ENGINEERING FOUNDATION (NightBuild02), not master canon. Thresholds in NavigationConfig are placeholders. Not saved: the active navigation is
// runtime state (the waypoint marker itself is persistent through WorldMapState).

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <spn/core/Routing.h>
#include <string>
#include <string_view>

namespace spn::core {

enum class NavigationStatus : std::uint8_t {
    Idle,      ///< no destination
    Active,    ///< following a route
    Arrived,   ///< within the arrival radius of the destination
    NoRoute,   ///< a destination exists but no road route does; see LastFailure()
    Cancelled, ///< aborted (by the caller, or the followed marker was removed)
};
[[nodiscard]] std::string_view ToString(NavigationStatus status) noexcept;

struct NavigationConfig {
    RouteOptions options;
    double offRouteMetres = 30.0;           ///< farther from the route than this = off route
    double rerouteCooldownSeconds = 2.0;    ///< minimum game time between two automatic re-plans (also the retry interval while NoRoute)
    double minArrivalRadiusMetres = 3.0;    ///< arrival radius = max(destination radius, this)
    std::size_t projectionWindow = 48;      ///< route segments searched around the last known position per Update
};

struct NavigationUpdate {
    NavigationStatus status = NavigationStatus::Idle;
    bool routeChanged = false;     ///< a new route was planned in this call (see Generation())
    bool rerouted = false;         ///< ... because the player left the route or closures changed
    bool offRoute = false;         ///< the player is farther than offRouteMetres from the route (re-plan pending / cooling down / failed)
    double remainingMetres = 0.0;  ///< along the route from the player's projection to the destination (0 when arrived / no route)
    double offRouteMetres = 0.0;   ///< distance from the route
    std::size_t segmentIndex = 0;  ///< Route::segments index of the projection
    double alongSegmentMetres = 0.0;
    std::optional<RouteFailure> failure; ///< set when status is NoRoute
};

class NavigationSession {
public:
    NavigationSession(std::shared_ptr<const MapGeometry> geometry, NavigationConfig config = {});

    /// Plans a route from `player` to `destination`. Replaces any previous destination. A failed plan is a state (NoRoute), not an error.
    NavigationUpdate SetDestination(const MapPosition& player, const Destination& destination, double now);

    /// Adds a persistent player waypoint at `target` (replacing the waypoint this session created earlier) and navigates to it. The waypoint is
    /// created even when no route exists. Fails only if the marker cannot be added (e.g. target outside the map).
    [[nodiscard]] Result<MarkerId, MapError> SetWaypoint(WorldMapState& map, const MapPosition& player, const MapPosition& target, std::string label, double now,
                                                         double arrivalRadiusMetres = 0.0);

    /// Aborts navigation. With `map`, also removes the waypoint this session created (if it still exists).
    void Cancel(WorldMapState* map = nullptr);

    /// Advances the session. With `map`, a destination that came from a marker follows it: moved marker = new destination, removed marker = Cancelled.
    NavigationUpdate Update(const MapPosition& player, double now, const WorldMapState* map = nullptr);

    /// Replaces the set of closed road segments. Re-plans immediately if the current route uses one of them (or if there is no route).
    NavigationUpdate SetClosedSegments(std::set<RoadSegmentId> closed, const MapPosition& player, double now);

    [[nodiscard]] NavigationStatus Status() const noexcept { return status_; }
    [[nodiscard]] const PlannedRoute* CurrentRoute() const noexcept { return route_.has_value() ? &*route_ : nullptr; }
    [[nodiscard]] const std::optional<Destination>& CurrentDestination() const noexcept { return destination_; }
    [[nodiscard]] std::optional<RouteFailure> LastFailure() const noexcept { return failure_; }
    /// Increments whenever a different route is installed or the route is cleared; a UI redraws its route layer when this changes.
    [[nodiscard]] std::uint64_t Generation() const noexcept { return generation_; }
    [[nodiscard]] const NavigationConfig& Config() const noexcept { return config_; }
    [[nodiscard]] MarkerId OwnedWaypoint() const noexcept { return ownedWaypoint_; }

private:
    NavigationUpdate Plan(const MapPosition& player, double now, bool automatic);
    NavigationUpdate Report(bool routeChanged, bool rerouted) const;
    void InstallRoute(PlannedRoute route);
    void ClearRoute();
    bool Project(const MapPosition& player);

    std::shared_ptr<const MapGeometry> geometry_;
    NavigationConfig config_;
    NavigationStatus status_ = NavigationStatus::Idle;
    std::optional<Destination> destination_;
    std::optional<PlannedRoute> route_;
    std::vector<double> remainingAfter_; ///< remainingAfter_[i] = length of route segments i+1 .. end
    std::optional<RouteFailure> failure_;
    std::uint64_t generation_ = 0;
    double lastPlanTime_ = -1.0e300;
    MarkerId ownedWaypoint_;
    // last projection
    std::size_t segment_ = 0;
    double along_ = 0.0;
    double offRoute_ = 0.0;
    double remaining_ = 0.0;
};

} // namespace spn::core
