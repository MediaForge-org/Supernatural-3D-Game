#include <algorithm>
#include <cmath>
#include <spn/core/Navigation.h>

namespace spn::core {

namespace {
NavigationError Err(NavigationErrorCode code) { return NavigationError{code}; }
bool ValidRadius(double r) { return std::isfinite(r) && r >= 0.0; }
bool ValidPos(const MapPosition& p) { return !p.layer.IsNull() && IsValidMapCoordinate(p.xy); }
} // namespace

std::string_view ToString(TravelMode mode) noexcept {
    switch (mode) {
    case TravelMode::OnFoot: return "OnFoot";
    case TravelMode::Driving: return "Driving";
    }
    return "Unknown";
}

std::string_view ToString(NavigationErrorCode code) noexcept {
    switch (code) {
    case NavigationErrorCode::InvalidPosition: return "InvalidPosition";
    case NavigationErrorCode::InvalidRadius: return "InvalidRadius";
    case NavigationErrorCode::UnknownLayer: return "UnknownLayer";
    case NavigationErrorCode::UnknownPoi: return "UnknownPoi";
    case NavigationErrorCode::UnknownMarker: return "UnknownMarker";
    case NavigationErrorCode::LayerMismatch: return "LayerMismatch";
    case NavigationErrorCode::InvalidSegment: return "InvalidSegment";
    }
    return "Unknown";
}

std::string_view ToString(RouteValidity validity) noexcept {
    switch (validity) {
    case RouteValidity::Valid: return "Valid";
    case RouteValidity::Empty: return "Empty";
    case RouteValidity::InvalidCoordinate: return "InvalidCoordinate";
    case RouteValidity::LayerMismatch: return "LayerMismatch";
    case RouteValidity::Discontinuous: return "Discontinuous";
    case RouteValidity::DestinationNotReached: return "DestinationNotReached";
    case RouteValidity::StartMismatch: return "StartMismatch";
    case RouteValidity::Incomplete: return "Incomplete";
    }
    return "Unknown";
}

Result<Destination, NavigationError> DestinationFromPoi(const WorldMapState& map, const ContentId& poi, double arrivalRadiusMetres) {
    if (!ValidRadius(arrivalRadiusMetres)) {
        return Err(NavigationErrorCode::InvalidRadius);
    }
    const PointOfInterestDef* def = map.Definition().FindPoi(poi);
    if (def == nullptr) {
        return Err(NavigationErrorCode::UnknownPoi);
    }
    Destination d;
    d.position = def->position;
    d.arrivalRadiusMetres = arrivalRadiusMetres;
    d.poi = poi;
    return d;
}

Result<Destination, NavigationError> DestinationFromMarker(const WorldMapState& map, MarkerId marker, double arrivalRadiusMetres) {
    if (!ValidRadius(arrivalRadiusMetres)) {
        return Err(NavigationErrorCode::InvalidRadius);
    }
    const Marker* m = map.FindMarker(marker);
    if (m == nullptr) {
        return Err(NavigationErrorCode::UnknownMarker);
    }
    Destination d;
    d.position = m->position;
    d.arrivalRadiusMetres = arrivalRadiusMetres;
    d.poi = m->linkedPoi;
    d.marker = marker;
    return d;
}

Result<Destination, NavigationError> DestinationFromPosition(const WorldMapState& map, const MapPosition& position, double arrivalRadiusMetres) {
    if (!ValidRadius(arrivalRadiusMetres)) {
        return Err(NavigationErrorCode::InvalidRadius);
    }
    const MapLayerDef* layer = map.Definition().FindLayer(position.layer);
    if (layer == nullptr) {
        return Err(NavigationErrorCode::UnknownLayer);
    }
    if (!ValidPos(position) || !layer->bounds.Contains(position.xy)) {
        return Err(NavigationErrorCode::InvalidPosition);
    }
    Destination d;
    d.position = position;
    d.arrivalRadiusMetres = arrivalRadiusMetres;
    return d;
}

bool HasArrived(const Destination& destination, const MapPosition& position) noexcept {
    const auto distance = PlanarDistance(destination.position, position);
    return distance && *distance <= destination.arrivalRadiusMetres;
}

RouteValidity ValidateRoute(const Route& route) {
    if (route.segments.empty()) {
        return RouteValidity::Empty;
    }
    for (std::size_t i = 0; i < route.segments.size(); ++i) {
        const RouteSegment& s = route.segments[i];
        if (!ValidPos(s.from) || !ValidPos(s.to)) {
            return RouteValidity::InvalidCoordinate;
        }
        const bool sameLayer = s.from.layer == s.to.layer;
        if ((s.kind == SegmentKind::Travel) != sameLayer) {
            return RouteValidity::LayerMismatch;
        }
        if (i > 0 && !(route.segments[i - 1].to == s.from)) {
            return RouteValidity::Discontinuous;
        }
    }
    if (!ValidPos(route.destination.position) || !ValidRadius(route.destination.arrivalRadiusMetres)) {
        return RouteValidity::InvalidCoordinate;
    }
    if (!route.complete) {
        return RouteValidity::Incomplete;
    }
    return HasArrived(route.destination, route.segments.back().to) ? RouteValidity::Valid : RouteValidity::DestinationNotReached;
}

RouteValidity ValidateRouteFrom(const Route& route, const MapPosition& origin) {
    const RouteValidity structural = ValidateRoute(route);
    if (structural == RouteValidity::Empty || structural == RouteValidity::InvalidCoordinate || structural == RouteValidity::LayerMismatch ||
        structural == RouteValidity::Discontinuous) {
        return structural;
    }
    if (!(route.segments.front().from == origin)) {
        return RouteValidity::StartMismatch;
    }
    return structural;
}

Result<Route, NavigationError> MakeStraightLineRoute(const MapPosition& origin, const Destination& destination, TravelMode mode) {
    if (!ValidPos(origin) || !ValidPos(destination.position)) {
        return Err(NavigationErrorCode::InvalidPosition);
    }
    if (origin.layer != destination.position.layer) {
        return Err(NavigationErrorCode::LayerMismatch);
    }
    Route route;
    route.source = RouteSource::StraightLine;
    route.destination = destination;
    route.segments.push_back(RouteSegment{SegmentKind::Travel, mode, origin, destination.position});
    return route;
}

double SegmentLength(const RouteSegment& s) noexcept {
    if (s.kind != SegmentKind::Travel) {
        return 0.0;
    }
    return PlanarDistance(s.from, s.to).value_or(0.0);
}

double RouteLength(const Route& route) noexcept {
    double total = 0.0;
    for (const RouteSegment& s : route.segments) {
        total += SegmentLength(s);
    }
    return total;
}

double RouteLengthInMode(const Route& route, TravelMode mode) noexcept {
    double total = 0.0;
    for (const RouteSegment& s : route.segments) {
        total += s.mode == mode ? SegmentLength(s) : 0.0;
    }
    return total;
}

std::optional<RouteProgress> ProjectOntoRoute(const Route& route, const MapPosition& position) {
    if (!std::isfinite(position.xy.x) || !std::isfinite(position.xy.y)) {
        return std::nullopt;
    }
    std::vector<double> lengths;
    lengths.reserve(route.segments.size());
    double total = 0.0;
    for (const RouteSegment& s : route.segments) {
        lengths.push_back(SegmentLength(s));
        total += lengths.back();
    }
    std::optional<RouteProgress> best;
    double before = 0.0; // route length before the current segment
    for (std::size_t i = 0; i < route.segments.size(); ++i) {
        const RouteSegment& s = route.segments[i];
        if (s.kind == SegmentKind::Travel && s.from.layer == position.layer) {
            const double ex = s.to.xy.x - s.from.xy.x;
            const double ey = s.to.xy.y - s.from.xy.y;
            const double lenSq = ex * ex + ey * ey;
            const double t = lenSq > 0.0 ? std::clamp(((position.xy.x - s.from.xy.x) * ex + (position.xy.y - s.from.xy.y) * ey) / lenSq, 0.0, 1.0) : 0.0;
            const double px = s.from.xy.x + ex * t;
            const double py = s.from.xy.y + ey * t;
            const double off = std::hypot(position.xy.x - px, position.xy.y - py);
            if (!best || off < best->offRouteMetres) {
                const double along = t * lengths[i];
                best = RouteProgress{i, along, off, std::max(0.0, total - before - along)};
            }
        }
        before += lengths[i];
    }
    return best;
}

} // namespace spn::core
