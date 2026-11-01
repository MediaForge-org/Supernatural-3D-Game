#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <spn/core/GeometryMath.h>
#include <spn/core/Routing.h>
#include <unordered_map>

namespace spn::core {

std::string_view ToString(RouteFailure failure) noexcept {
    switch (failure) {
    case RouteFailure::InvalidInput: return "InvalidInput";
    case RouteFailure::LayerMismatch: return "LayerMismatch";
    case RouteFailure::UnknownLayer: return "UnknownLayer";
    case RouteFailure::OriginNotOnNetwork: return "OriginNotOnNetwork";
    case RouteFailure::DestinationNotAccessible: return "DestinationNotAccessible";
    case RouteFailure::NoPath: return "NoPath";
    case RouteFailure::SearchLimitReached: return "SearchLimitReached";
    }
    return "Unknown";
}

std::string_view ToString(LegKind kind) noexcept {
    switch (kind) {
    case LegKind::StartConnector: return "StartConnector";
    case LegKind::Road: return "Road";
    case LegKind::EndConnector: return "EndConnector";
    }
    return "Unknown";
}

namespace {

constexpr double kInf = std::numeric_limits<double>::infinity();
constexpr std::uint32_t kNone = 0xFFFFFFFFu;
/// Search-only weight of connector length: at equal real length a road is preferred over a straight connector (a start point 50 m beside a
/// junction must walk the road, not a straight line along it). Reported lengths are never weighted.
constexpr double kConnectorBias = 1.0 + 1.0e-7;

struct StartCandidate {
    RoadSnap snap;
    double connectorLength = 0.0;
};

struct GoalCandidate {
    RoadSnap snap;
    std::vector<Vec2> endPoints; ///< snap point -> (access point) -> destination
    double connectorLength = 0.0;
    Vec2 accessPoint;
};

struct PseudoStart {
    std::uint32_t candidate = 0;
    bool forward = true;
};

struct StateRec {
    double g = kInf;
    std::uint32_t parent = kNone;
    bool settled = false;
};

struct Event {
    double total;
    std::uint32_t goal;
    std::uint32_t parent;
};

struct Item {
    double f;
    std::uint32_t kind; // 0 = goal event (preferred on ties), 1 = state
    std::uint32_t id;   // event index / state id
    friend bool operator>(const Item& a, const Item& b) {
        if (a.f != b.f) { return a.f > b.f; }
        if (a.kind != b.kind) { return a.kind > b.kind; }
        return a.id > b.id;
    }
};

using Penalties = std::unordered_map<std::uint32_t, double>;

bool ValidOptions(const RouteOptions& o) {
    if (!std::isfinite(o.maxStartSnapMetres) || o.maxStartSnapMetres < 0.0 || !std::isfinite(o.maxDestinationSnapMetres) || o.maxDestinationSnapMetres < 0.0 ||
        !std::isfinite(o.snapSlackMetres) || o.snapSlackMetres < 0.0 || !std::isfinite(o.maxWetConnectorMetres) || o.maxWetConnectorMetres < 0.0 || o.maxSnapCandidates == 0) {
        return false;
    }
    return std::all_of(o.classCostFactor.begin(), o.classCostFactor.end(), [](double f) { return std::isfinite(f) && f >= 1.0; });
}

RouteError Fail(RouteFailure code, const RouteStats& stats = {}) { return RouteError{code, stats}; }

Result<PlannedRoute, RouteError> PlanImpl(const MapGeometry& geometry, const MapPosition& origin, const Destination& destination, const RouteOptions& options,
                                           const Penalties* penalties) {
    const RoadNetwork& net = geometry.Roads();
    RouteStats stats;
    if (!ValidOptions(options) || origin.layer.IsNull() || !IsValidMapCoordinate(origin.xy) || !IsValidMapCoordinate(destination.position.xy)) {
        return Fail(RouteFailure::InvalidInput);
    }
    if (origin.layer != destination.position.layer) {
        return Fail(RouteFailure::LayerMismatch);
    }
    const MapLayerId layer = origin.layer;
    const Vec2 destPoint = destination.position.xy;
    if (!net.HasLayer(layer)) {
        return Fail(RouteFailure::UnknownLayer);
    }
    const TravelMode mode = options.mode;

    // --- start candidates ---
    // Slack is measured from the nearest USABLE candidate (one whose connector is not blocked by water), so a road across the river that happens
    // to be nearer does not hide the roads on the own bank.
    const auto usable = [&](const MapPosition& from, const SnapRules& rules, double reach) {
        std::vector<RoadSnap> kept;
        for (const RoadSnap& s : net.Snap(from, reach, rules, options.maxSnapCandidates * 4)) {
            if (s.distanceMetres > 0.0 && geometry.ConnectorBlocked(layer, from.xy, s.point, options.maxWetConnectorMetres)) { continue; }
            if (!kept.empty() && s.distanceMetres > kept.front().distanceMetres + options.snapSlackMetres) { break; }
            kept.push_back(s);
            if (kept.size() >= options.maxSnapCandidates) { break; }
        }
        return kept;
    };
    std::vector<StartCandidate> starts;
    for (const RoadSnap& s : usable(origin, SnapRules{mode, true, &options.closedSegments}, options.maxStartSnapMetres)) {
        starts.push_back(StartCandidate{s, s.distanceMetres});
    }
    stats.startCandidates = starts.size();
    if (starts.empty()) {
        return Fail(RouteFailure::OriginNotOnNetwork, stats);
    }

    // --- goal candidates: building entrances if the destination is inside a building that has some, else the destination itself ---
    const BuildingDef* building = geometry.BuildingAt(destination.position);
    const bool useEntrances = building != nullptr && !building->entrances.empty();
    std::vector<Vec2> accessPoints;
    if (useEntrances) {
        accessPoints = building->entrances;
    } else {
        accessPoints.push_back(destPoint);
    }
    std::vector<GoalCandidate> goals;
    for (const Vec2 access : accessPoints) {
        const MapPosition accessPos{layer, access};
        for (const RoadSnap& s : usable(accessPos, SnapRules{mode, false, &options.closedSegments}, options.maxDestinationSnapMetres)) {
            GoalCandidate g;
            g.snap = s;
            g.accessPoint = access;
            g.endPoints.push_back(s.point);
            if (!(access == g.endPoints.back())) { g.endPoints.push_back(access); }
            if (!(destPoint == g.endPoints.back())) { g.endPoints.push_back(destPoint); }
            g.connectorLength = PolylineLength(g.endPoints);
            goals.push_back(std::move(g));
        }
    }
    stats.goalCandidates = goals.size();
    if (goals.empty()) {
        return Fail(RouteFailure::DestinationNotAccessible, stats);
    }

    // --- search ---
    const auto factorOf = [&](std::uint32_t seg) {
        double f = options.classCostFactor[static_cast<std::size_t>(net.Info(seg).roadClass)];
        if (penalties != nullptr) {
            const auto it = penalties->find(seg);
            if (it != penalties->end()) { f *= it->second; }
        }
        return f;
    };
    const auto heuristic = [&](Vec2 p) {
        double best = kInf;
        for (const GoalCandidate& g : goals) {
            best = std::min(best, Distance(p, g.snap.point) + g.connectorLength);
        }
        return best;
    };

    const std::uint32_t realStates = static_cast<std::uint32_t>(net.Segments().size()) * 2u; // state id = segment * 2 + (forward ? 0 : 1)
    std::unordered_map<std::uint32_t, StateRec> rec;
    std::vector<PseudoStart> pseudo;
    std::vector<Event> events;
    std::priority_queue<Item, std::vector<Item>, std::greater<>> queue;

    const auto relax = [&](std::uint32_t seg, bool forward, double entryCost, std::uint32_t parent) {
        const std::uint32_t id = seg * 2u + (forward ? 0u : 1u);
        StateRec& r = rec[id];
        if (entryCost < r.g) {
            r.g = entryCost;
            r.parent = parent;
            const RoadNetwork::SegmentInfo& info = net.Info(seg);
            queue.push(Item{entryCost + heuristic(forward ? info.points.front() : info.points.back()), 1, id});
            ++stats.statesPushed;
        }
    };
    // Expands the moves available after travelling segment `seg` in direction `forward` up to its end node, with `costAtEnd` accumulated.
    const auto expand = [&](std::uint32_t seg, bool forward, double costAtEnd, std::uint32_t parent) {
        const RoadNetwork::SegmentInfo& info = net.Info(seg);
        const std::uint32_t node = forward ? info.toNode : info.fromNode;
        bool any = false;
        std::optional<bool> uturn;
        for (const RoadNetwork::Incident& inc : net.IncidentAt(node)) {
            const bool leavesForward = inc.startsHere;
            if (inc.segment == seg && leavesForward != forward) {
                uturn = leavesForward;
                continue;
            }
            if (!net.Allows(inc.segment, mode, leavesForward) || options.closedSegments.contains(net.Segments()[inc.segment].id) ||
                net.TurnForbidden(node, seg, inc.segment, mode)) {
                continue;
            }
            relax(inc.segment, leavesForward, costAtEnd, parent);
            any = true;
        }
        if (!any && uturn.has_value() && net.Allows(seg, mode, *uturn) && !net.TurnForbidden(node, seg, seg, mode)) {
            relax(seg, *uturn, costAtEnd, parent); // dead end: the only way on is back
        }
    };

    for (std::uint32_t c = 0; c < starts.size(); ++c) {
        const RoadSnap& s = starts[c].snap;
        const double len = net.Info(s.segmentIndex).length;
        for (const bool forward : {true, false}) {
            if (!net.Allows(s.segmentIndex, mode, forward)) { continue; }
            const std::uint32_t pid = realStates + static_cast<std::uint32_t>(pseudo.size());
            pseudo.push_back(PseudoStart{c, forward});
            const double factor = factorOf(s.segmentIndex);
            const double partial = forward ? len - s.offsetMetres : s.offsetMetres;
            // goals on the same segment, ahead of the start in the travel direction
            for (std::uint32_t gi = 0; gi < goals.size(); ++gi) {
                const GoalCandidate& g = goals[gi];
                if (g.snap.segmentIndex != s.segmentIndex) { continue; }
                const double ahead = forward ? g.snap.offsetMetres - s.offsetMetres : s.offsetMetres - g.snap.offsetMetres;
                if (ahead < 0.0) { continue; }
                const double total = (starts[c].connectorLength + g.connectorLength) * kConnectorBias + ahead * factor;
                events.push_back(Event{total, gi, pid});
                queue.push(Item{total, 0, static_cast<std::uint32_t>(events.size() - 1)});
            }
            expand(s.segmentIndex, forward, starts[c].connectorLength * kConnectorBias + partial * factor, pid);
        }
    }

    std::uint32_t chosenEvent = kNone;
    while (!queue.empty()) {
        const Item item = queue.top();
        queue.pop();
        if (item.kind == 0) {
            chosenEvent = item.id;
            break;
        }
        StateRec& r = rec[item.id];
        if (r.settled) { continue; }
        r.settled = true;
        if (++stats.statesExpanded > options.maxExpansions) {
            return Fail(RouteFailure::SearchLimitReached, stats);
        }
        const std::uint32_t seg = item.id / 2u;
        const bool forward = (item.id & 1u) == 0;
        const RoadNetwork::SegmentInfo& info = net.Info(seg);
        const double factor = factorOf(seg);
        const double entry = r.g;
        for (std::uint32_t gi = 0; gi < goals.size(); ++gi) {
            const GoalCandidate& g = goals[gi];
            if (g.snap.segmentIndex != seg) { continue; }
            const double along = forward ? g.snap.offsetMetres : info.length - g.snap.offsetMetres;
            const double total = entry + along * factor + g.connectorLength * kConnectorBias;
            events.push_back(Event{total, gi, item.id});
            queue.push(Item{total, 0, static_cast<std::uint32_t>(events.size() - 1)});
        }
        expand(seg, forward, entry + info.length * factor, item.id);
    }
    if (chosenEvent == kNone) {
        return Fail(RouteFailure::NoPath, stats);
    }

    // --- reconstruct ---
    const Event& ev = events[chosenEvent];
    const GoalCandidate& goal = goals[ev.goal];
    std::vector<std::uint32_t> chain; // states from the goal's state back to the pseudo start
    for (std::uint32_t s = ev.parent; s != kNone;) {
        chain.push_back(s);
        if (s >= realStates) { break; }
        s = rec[s].parent;
    }
    std::reverse(chain.begin(), chain.end());

    struct Piece {
        LegKind kind;
        std::uint32_t segment;
        std::vector<Vec2> points;
    };
    std::vector<Piece> pieces;
    const PseudoStart& ps = pseudo[chain.front() - realStates];
    const StartCandidate& sc = starts[ps.candidate];
    if (sc.snap.distanceMetres > 0.0) {
        pieces.push_back(Piece{LegKind::StartConnector, 0, {origin.xy, sc.snap.point}});
    }
    const auto fullSlice = [&](std::uint32_t seg, bool forward) {
        const RoadNetwork::SegmentInfo& info = net.Info(seg);
        return forward ? SlicePolyline(info.points, info.cumulative, 0.0, info.points.front(), info.length, info.points.back())
                       : SlicePolyline(info.points, info.cumulative, info.length, info.points.back(), 0.0, info.points.front());
    };
    {
        const RoadNetwork::SegmentInfo& info = net.Info(sc.snap.segmentIndex);
        if (chain.size() == 1) {
            pieces.push_back(Piece{LegKind::Road, sc.snap.segmentIndex,
                                   SlicePolyline(info.points, info.cumulative, sc.snap.offsetMetres, sc.snap.point, goal.snap.offsetMetres, goal.snap.point)});
        } else {
            const double end = ps.forward ? info.length : 0.0;
            pieces.push_back(Piece{LegKind::Road, sc.snap.segmentIndex,
                                   SlicePolyline(info.points, info.cumulative, sc.snap.offsetMetres, sc.snap.point, end, ps.forward ? info.points.back() : info.points.front())});
        }
    }
    for (std::size_t i = 1; i < chain.size(); ++i) {
        const std::uint32_t seg = chain[i] / 2u;
        const bool forward = (chain[i] & 1u) == 0;
        if (i + 1 < chain.size()) {
            pieces.push_back(Piece{LegKind::Road, seg, fullSlice(seg, forward)});
        } else {
            const RoadNetwork::SegmentInfo& info = net.Info(seg);
            pieces.push_back(Piece{LegKind::Road, seg,
                                   forward ? SlicePolyline(info.points, info.cumulative, 0.0, info.points.front(), goal.snap.offsetMetres, goal.snap.point)
                                           : SlicePolyline(info.points, info.cumulative, info.length, info.points.back(), goal.snap.offsetMetres, goal.snap.point)});
        }
    }
    pieces.push_back(Piece{LegKind::EndConnector, 0, goal.endPoints});

    PlannedRoute out;
    out.route.source = RouteSource::External;
    out.route.destination = destination;
    out.route.complete = true;
    std::vector<Vec2> path{origin.xy};
    for (const Piece& piece : pieces) {
        RouteLeg leg;
        leg.kind = piece.kind;
        leg.firstSegment = path.size() - 1;
        if (piece.kind == LegKind::Road) {
            leg.road = net.Segments()[piece.segment].id;
            leg.roadClass = net.Info(piece.segment).roadClass;
            leg.structure = net.Info(piece.segment).structure;
        }
        for (const Vec2 p : piece.points) {
            if (!(p == path.back())) {
                leg.lengthMetres += Distance(path.back(), p);
                path.push_back(p);
                ++leg.segmentCount;
            }
        }
        if (leg.segmentCount > 0) {
            out.legs.push_back(leg);
        }
    }
    for (std::size_t i = 1; i < path.size(); ++i) {
        out.route.segments.push_back(RouteSegment{SegmentKind::Travel, mode, MapPosition{layer, path[i - 1]}, MapPosition{layer, path[i]}});
    }
    for (const RouteLeg& leg : out.legs) {
        (leg.kind == LegKind::Road ? out.roadLengthMetres : out.connectorLengthMetres) += leg.lengthMetres;
    }
    out.lengthMetres = RouteLength(out.route);
    out.straightLineMetres = Distance(origin.xy, destPoint);
    out.destinationBuilding = useEntrances ? building->id : BuildingId{};
    out.accessPoint = goal.accessPoint;
    out.stats = stats;
    return out;
}

} // namespace

Result<PlannedRoute, RouteError> PlanRoute(const MapGeometry& geometry, const MapPosition& origin, const Destination& destination, const RouteOptions& options) {
    return PlanImpl(geometry, origin, destination, options, nullptr);
}

Result<std::vector<PlannedRoute>, RouteError> PlanRouteAlternatives(const MapGeometry& geometry, const MapPosition& origin, const Destination& destination,
                                                                     const RouteOptions& options, std::size_t count, double maxStretch) {
    auto first = PlanImpl(geometry, origin, destination, options, nullptr);
    if (!first) {
        return first.Error();
    }
    std::vector<PlannedRoute> out;
    out.push_back(std::move(first).Value());
    if (count <= 1 || !std::isfinite(maxStretch) || maxStretch < 1.0) {
        return out;
    }
    const auto roadsOf = [&](const PlannedRoute& r) {
        std::vector<std::uint64_t> ids;
        for (const RouteLeg& leg : r.legs) {
            if (leg.kind == LegKind::Road) { ids.push_back(leg.road.Value()); }
        }
        return ids;
    };
    Penalties penalties;
    const auto penalise = [&](const PlannedRoute& r) {
        for (const RouteLeg& leg : r.legs) {
            if (leg.kind == LegKind::Road) {
                const auto idx = geometry.Roads().SegmentIndex(leg.road);
                if (idx.has_value()) { auto [it, inserted] = penalties.try_emplace(*idx, 1.0); (void)inserted; it->second *= 1.6; }
            }
        }
    };
    penalise(out.front());
    const double limit = out.front().lengthMetres * maxStretch;
    for (std::size_t attempt = 0; attempt < count * 4 && out.size() < count; ++attempt) {
        auto next = PlanImpl(geometry, origin, destination, options, &penalties);
        if (!next) {
            break;
        }
        PlannedRoute candidate = std::move(next).Value();
        penalise(candidate); // always push the next search away, even from a route we drop
        const auto ids = roadsOf(candidate);
        const bool duplicate = std::any_of(out.begin(), out.end(), [&](const PlannedRoute& r) { return roadsOf(r) == ids; });
        if (!duplicate && candidate.lengthMetres <= limit) {
            out.push_back(std::move(candidate));
        }
    }
    std::stable_sort(out.begin() + 1, out.end(), [](const PlannedRoute& a, const PlannedRoute& b) { return a.lengthMetres < b.lengthMetres; });
    return out;
}

bool RouteUsesAny(const PlannedRoute& route, const std::set<RoadSegmentId>& closed) {
    return std::any_of(route.legs.begin(), route.legs.end(), [&](const RouteLeg& leg) { return leg.kind == LegKind::Road && closed.contains(leg.road); });
}

} // namespace spn::core
