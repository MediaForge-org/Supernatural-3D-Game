#include <algorithm>
#include <cmath>
#include <limits>
#include <spn/core/GeometryMath.h>
#include <spn/core/NavigationSession.h>

namespace spn::core {

std::string_view ToString(NavigationStatus status) noexcept {
    switch (status) {
    case NavigationStatus::Idle: return "Idle";
    case NavigationStatus::Active: return "Active";
    case NavigationStatus::Arrived: return "Arrived";
    case NavigationStatus::NoRoute: return "NoRoute";
    case NavigationStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

NavigationSession::NavigationSession(std::shared_ptr<const MapGeometry> geometry, NavigationConfig config) : geometry_(std::move(geometry)), config_(std::move(config)) {}

void NavigationSession::ClearRoute() {
    if (route_.has_value()) {
        ++generation_;
    }
    route_.reset();
    remainingAfter_.clear();
    segment_ = 0;
    along_ = 0.0;
    offRoute_ = 0.0;
    remaining_ = 0.0;
}

void NavigationSession::InstallRoute(PlannedRoute route) {
    route_ = std::move(route);
    ++generation_;
    const auto& segs = route_->route.segments;
    remainingAfter_.assign(segs.size(), 0.0);
    double sum = 0.0;
    for (std::size_t i = segs.size(); i-- > 0;) {
        remainingAfter_[i] = sum;
        sum += SegmentLength(segs[i]);
    }
    segment_ = 0;
    along_ = 0.0;
    offRoute_ = 0.0;
    remaining_ = route_->lengthMetres;
}

NavigationUpdate NavigationSession::Report(bool routeChanged, bool rerouted) const {
    NavigationUpdate u;
    u.status = status_;
    u.routeChanged = routeChanged;
    u.rerouted = rerouted;
    u.failure = status_ == NavigationStatus::NoRoute ? failure_ : std::nullopt;
    if (status_ == NavigationStatus::Active && route_.has_value()) {
        u.remainingMetres = remaining_;
        u.offRouteMetres = offRoute_;
        u.offRoute = offRoute_ > config_.offRouteMetres;
        u.segmentIndex = segment_;
        u.alongSegmentMetres = along_;
    }
    return u;
}

NavigationUpdate NavigationSession::Plan(const MapPosition& player, double now, bool automatic) {
    lastPlanTime_ = now;
    if (!destination_.has_value()) { // callers set it first; never dereference an empty optional
        return Report(false, false);
    }
    auto planned = PlanRoute(*geometry_, player, *destination_, config_.options);
    if (planned) {
        failure_.reset();
        InstallRoute(std::move(planned).Value());
        status_ = NavigationStatus::Active;
        (void)Project(player);
        return Report(true, automatic);
    }
    ClearRoute();
    failure_ = planned.Error().code;
    status_ = NavigationStatus::NoRoute;
    return Report(false, false);
}

NavigationUpdate NavigationSession::SetDestination(const MapPosition& player, const Destination& destination, double now) {
    destination_ = destination;
    return Plan(player, now, false);
}

Result<MarkerId, MapError> NavigationSession::SetWaypoint(WorldMapState& map, const MapPosition& player, const MapPosition& target, std::string label, double now,
                                                          double arrivalRadiusMetres) {
    auto marker = map.AddPlayerWaypoint(target, std::move(label));
    if (!marker) {
        return marker.Error();
    }
    if (!ownedWaypoint_.IsNull() && ownedWaypoint_ != marker.Value()) {
        (void)map.RemoveMarker(ownedWaypoint_);
    }
    ownedWaypoint_ = marker.Value();
    Destination d;
    d.position = target;
    d.arrivalRadiusMetres = std::isfinite(arrivalRadiusMetres) && arrivalRadiusMetres > 0.0 ? arrivalRadiusMetres : 0.0;
    d.marker = marker.Value();
    (void)SetDestination(player, d, now);
    return marker.Value();
}

void NavigationSession::Cancel(WorldMapState* map) {
    if (map != nullptr && !ownedWaypoint_.IsNull()) {
        (void)map->RemoveMarker(ownedWaypoint_);
    }
    ownedWaypoint_ = MarkerId{};
    destination_.reset();
    failure_.reset();
    ClearRoute();
    status_ = NavigationStatus::Cancelled;
}

// Nearest point on the route within a window of segments around the last position; the whole route if the window finds nothing close.
bool NavigationSession::Project(const MapPosition& player) {
    if (!route_.has_value() || route_->route.segments.empty() || !IsValidMapCoordinate(player.xy)) {
        return false;
    }
    const auto& segs = route_->route.segments;
    const auto scan = [&](std::size_t from, std::size_t to, double& bestDist, std::size_t& bestSeg, double& bestAlong) {
        for (std::size_t i = from; i < to; ++i) {
            const SegmentProjection p = ProjectPointToSegment(player.xy, segs[i].from.xy, segs[i].to.xy);
            if (p.distance < bestDist) {
                bestDist = p.distance;
                bestSeg = i;
                bestAlong = p.t * SegmentLength(segs[i]);
            }
        }
    };
    double bestDist = std::numeric_limits<double>::infinity();
    std::size_t bestSeg = 0;
    double bestAlong = 0.0;
    const std::size_t window = std::max<std::size_t>(config_.projectionWindow, 1);
    const std::size_t lo = segment_ > 2 ? segment_ - 2 : 0;
    const std::size_t hi = std::min(segs.size(), segment_ + window);
    scan(lo, hi, bestDist, bestSeg, bestAlong);
    if (!(bestDist <= config_.offRouteMetres) && (lo > 0 || hi < segs.size())) {
        scan(0, segs.size(), bestDist, bestSeg, bestAlong); // lost: search everything (rare)
    }
    segment_ = bestSeg;
    along_ = bestAlong;
    offRoute_ = bestDist;
    remaining_ = remainingAfter_[bestSeg] + (SegmentLength(segs[bestSeg]) - bestAlong);
    return true;
}

NavigationUpdate NavigationSession::Update(const MapPosition& player, double now, const WorldMapState* map) {
    if (status_ == NavigationStatus::Idle || status_ == NavigationStatus::Cancelled || !destination_.has_value()) {
        return Report(false, false);
    }
    bool replan = false;
    if (map != nullptr && !destination_->marker.IsNull()) {
        const Marker* marker = map->FindMarker(destination_->marker);
        if (marker == nullptr) {
            Cancel(nullptr); // the followed marker is gone: route abort
            return Report(false, false);
        }
        if (!(marker->position == destination_->position)) {
            destination_->position = marker->position;
            destination_->poi = ContentId{};
            replan = true;
            status_ = NavigationStatus::Active;
        }
    }
    const double radius = std::max(destination_->arrivalRadiusMetres, config_.minArrivalRadiusMetres);
    if (!replan && player.layer == destination_->position.layer && IsValidMapCoordinate(player.xy) && Distance(player.xy, destination_->position.xy) <= radius) {
        status_ = NavigationStatus::Arrived;
        return Report(false, false);
    }
    if (status_ == NavigationStatus::Arrived) {
        status_ = NavigationStatus::Active; // walked away again: keep guiding
        replan = true;
    }
    if (replan) {
        return Plan(player, now, false);
    }
    if (status_ == NavigationStatus::NoRoute) {
        if (now - lastPlanTime_ >= config_.rerouteCooldownSeconds) {
            return Plan(player, now, true);
        }
        return Report(false, false);
    }
    if (!Project(player)) {
        return Report(false, false);
    }
    if (offRoute_ > config_.offRouteMetres && now - lastPlanTime_ >= config_.rerouteCooldownSeconds) {
        return Plan(player, now, true);
    }
    return Report(false, false);
}

NavigationUpdate NavigationSession::SetClosedSegments(std::set<RoadSegmentId> closed, const MapPosition& player, double now) {
    const bool affected = route_.has_value() && RouteUsesAny(*route_, closed);
    config_.options.closedSegments = std::move(closed);
    if (!destination_.has_value() || status_ == NavigationStatus::Cancelled) {
        return Report(false, false);
    }
    if (affected || status_ == NavigationStatus::NoRoute) {
        return Plan(player, now, true);
    }
    return Report(false, false);
}

} // namespace spn::core
