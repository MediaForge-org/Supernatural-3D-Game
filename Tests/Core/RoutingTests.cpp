#include <algorithm>
#include <cmath>
#include <doctest.h>
#include <spn/core/GeometryMath.h>
#include <spn/core/Routing.h>
#include <spn/core/SyntheticMapRegion.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::core::synthetic;
using doctest::Approx;

namespace {

Destination Dest(MapPosition p, double radius = 0.0) {
    Destination d;
    d.position = p;
    d.arrivalRadiusMetres = radius;
    return d;
}

MapPosition At(const SyntheticMapRegion& r, double x, double y) { return MapPosition{r.layer, Vec2{x, y}}; }

RouteOptions Options(TravelMode mode, std::initializer_list<std::uint64_t> closed = {}) {
    RouteOptions o;
    o.mode = mode;
    for (const std::uint64_t id : closed) {
        o.closedSegments.insert(RoadSegmentId(id));
    }
    return o;
}

bool UsesRoad(const PlannedRoute& p, std::uint64_t id) {
    return std::any_of(p.legs.begin(), p.legs.end(), [&](const RouteLeg& l) { return l.kind == LegKind::Road && l.road == RoadSegmentId(id); });
}

std::vector<std::uint64_t> RoadIds(const PlannedRoute& p) {
    std::vector<std::uint64_t> ids;
    for (const RouteLeg& l : p.legs) {
        if (l.kind == LegKind::Road) { ids.push_back(l.road.Value()); }
    }
    return ids;
}

/// Properties every planned route must have, whatever the scenario.
void CheckInvariants(const SyntheticMapRegion& r, const PlannedRoute& p, const MapPosition& origin, const MapPosition& dest) {
    CHECK(ValidateRouteFrom(p.route, origin) == RouteValidity::Valid);
    CHECK(p.route.source == RouteSource::External); // never presented as a straight line
    REQUIRE(!p.route.segments.empty());
    CHECK(p.route.segments.back().to == dest);
    CHECK(p.lengthMetres == Approx(RouteLength(p.route)));
    CHECK(p.lengthMetres + 1e-9 >= p.straightLineMetres);
    double legSum = 0.0;
    std::size_t next = 0;
    for (const RouteLeg& l : p.legs) {
        CHECK(l.firstSegment == next); // legs tile the route without gaps
        next += l.segmentCount;
        legSum += l.lengthMetres;
    }
    CHECK(next == p.route.segments.size());
    CHECK(legSum == Approx(p.lengthMetres));
    CHECK(p.roadLengthMetres + p.connectorLengthMetres == Approx(p.lengthMetres));
    // Only bridges and tunnels may cross the river: every other piece (surface roads and both connectors) stays out of the water.
    const AreaDef* river = r.geometry->FindArea(MapAreaId(1));
    REQUIRE(river != nullptr);
    for (const RouteLeg& l : p.legs) {
        if (l.kind == LegKind::Road && l.structure != RoadStructure::Surface) { continue; }
        for (std::size_t i = l.firstSegment; i < l.firstSegment + l.segmentCount; ++i) {
            const RouteSegment& s = p.route.segments[i];
            CHECK_FALSE(SegmentCrossesRingBoundary(s.from.xy, s.to.xy, river->outline));
        }
    }
}

PlannedRoute MustPlan(const SyntheticMapRegion& r, const MapPosition& from, const MapPosition& to, const RouteOptions& o) {
    auto plan = PlanRoute(*r.geometry, from, Dest(to), o);
    REQUIRE(plan.HasValue());
    return std::move(plan).Value();
}

} // namespace

TEST_CASE("Routing: a vehicle crosses the river only on the far bridge, never in a straight line") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute p = MustPlan(r, r.westStreet, r.eastStreet, Options(TravelMode::Driving));
    CheckInvariants(r, p, r.westStreet, r.eastStreet);
    CHECK(p.straightLineMetres == Approx(1000.0));
    CHECK(p.lengthMetres == Approx(3400.0)); // 400 + 600 + 600 + 200 (bridge) + 600 + 300 + 300 + 400 along the roads
    CHECK(p.connectorLengthMetres == 0.0);   // both ends are on a road
    CHECK(UsesRoad(p, kNorthBridge));
    CHECK_FALSE(UsesRoad(p, kFootBridge));
    CHECK_FALSE(UsesRoad(p, kMotorTunnel));
    // the bridge is the only piece that crosses x = 1000
    std::size_t crossings = 0;
    for (const RouteSegment& s : p.route.segments) {
        if ((s.from.xy.x < 1000.0) != (s.to.xy.x < 1000.0)) { ++crossings; }
    }
    CHECK(crossings == 1);
    CHECK(RoadIds(p) == std::vector<std::uint64_t>{kStreetWestBC, kRoadWestCD, kRoadWestDE, kNorthBridge, kRoadEastDE, kRoadEastFD, kRoadEastCF, kStreetEastCB});
}

TEST_CASE("Routing: pedestrians take the foot bridge, vehicles cannot use it") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute walk = MustPlan(r, r.westStreet, r.eastStreet, Options(TravelMode::OnFoot));
    CheckInvariants(r, walk, r.westStreet, r.eastStreet);
    CHECK(walk.lengthMetres == Approx(1600.0));
    CHECK(UsesRoad(walk, kFootBridge));
    CHECK(UsesRoad(walk, kFootEastApproach));
    for (const RouteSegment& s : walk.route.segments) { CHECK(s.mode == TravelMode::OnFoot); }

    const PlannedRoute drive = MustPlan(r, r.westStreet, r.eastStreet, Options(TravelMode::Driving));
    CHECK_FALSE(UsesRoad(drive, kFootBridge));
    CHECK(drive.lengthMetres > walk.lengthMetres);
    for (const RouteSegment& s : drive.route.segments) { CHECK(s.mode == TravelMode::Driving); }
}

TEST_CASE("Routing: pedestrians may use the motorway tunnel's neighbours but never the motorway itself") {
    const auto r = MakeSyntheticMapRegion();
    // From the west street to the motorway tunnel mouth area on foot: motorways and links are vehicle-only.
    const MapPosition motorway = At(r, 500, -800);
    const auto walk = PlanRoute(*r.geometry, r.westStreet, Dest(motorway), Options(TravelMode::OnFoot));
    REQUIRE_FALSE(walk.HasValue());
    CHECK(walk.Error().code == RouteFailure::DestinationNotAccessible); // nothing walkable within reach of the motorway
    const PlannedRoute drive = MustPlan(r, r.westStreet, motorway, Options(TravelMode::Driving));
    CHECK(UsesRoad(drive, kOnRampWest));
    CheckInvariants(r, drive, r.westStreet, motorway);
}

TEST_CASE("Routing: closing the bridge sends vehicles through the tunnel, up the ramps and around the roundabout") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute p = MustPlan(r, r.westStreet, r.eastStreet, Options(TravelMode::Driving, {kNorthBridge}));
    CheckInvariants(r, p, r.westStreet, r.eastStreet);
    CHECK_FALSE(UsesRoad(p, kNorthBridge));
    CHECK(UsesRoad(p, kMotorTunnel));
    const double expected = 500.0 + 2.0 * std::hypot(30.0, 400.0) + 2000.0 + std::hypot(20.0, 400.0) + std::hypot(20.0, 370.0) + 6.0 * std::hypot(21.0, 9.0) + 420.0 + 50.0;
    CHECK(p.lengthMetres == Approx(expected).epsilon(1e-9));
    // The ring is one-way (counter-clockwise): entering at the south node the vehicle must go 4 -> 1 -> 2 -> 3, the long way round.
    const auto ids = RoadIds(p);
    const std::vector<std::uint64_t> wanted{kRingSegment4, kRingSegment1, kRingSegment2};
    const auto ring = std::search(ids.begin(), ids.end(), wanted.begin(), wanted.end());
    CHECK(ring != ids.end());
    CHECK_FALSE(UsesRoad(p, kRingSegment3));
}

TEST_CASE("Routing: pedestrians walk a roundabout in either direction") {
    const auto r = MakeSyntheticMapRegion();
    const MapPosition ring3 = At(r, 1970, 0);   // ring node 3
    const MapPosition ring4 = At(r, 2000, -30); // ring node 4; the one-way ring segment 3 runs 3 -> 4
    const double step = 2.0 * std::hypot(21.0, 9.0);
    const PlannedRoute with = MustPlan(r, ring3, ring4, Options(TravelMode::OnFoot));
    CHECK(UsesRoad(with, kRingSegment3));
    CHECK(with.lengthMetres == Approx(step));
    const PlannedRoute against = MustPlan(r, ring4, ring3, Options(TravelMode::OnFoot));
    CHECK(UsesRoad(against, kRingSegment3)); // 4 -> 3 walks the one-way segment backwards
    CHECK(against.lengthMetres == Approx(step));
    const PlannedRoute car = MustPlan(r, ring4, ring3, Options(TravelMode::Driving));
    CHECK_FALSE(UsesRoad(car, kRingSegment3));
    CHECK(car.lengthMetres == Approx(3.0 * step)); // 4 -> 1 -> 2 -> 3 around the ring
    CheckInvariants(r, car, ring4, ring3);
}

TEST_CASE("Routing: a closed-off river leaves the other bank unreachable and says so") {
    const auto r = MakeSyntheticMapRegion();
    const auto drive = PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), Options(TravelMode::Driving, {kNorthBridge, kMotorTunnel}));
    REQUIRE_FALSE(drive.HasValue());
    CHECK(drive.Error().code == RouteFailure::NoPath);
    CHECK(drive.Error().stats.statesExpanded > 0); // it searched the reachable network
    const auto walk = PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), Options(TravelMode::OnFoot, {kNorthBridge, kFootBridge}));
    REQUIRE_FALSE(walk.HasValue());
    CHECK(walk.Error().code == RouteFailure::NoPath);
    // pedestrians are not bound by the vehicle tunnel: closing it changes nothing for them
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), Options(TravelMode::OnFoot, {kMotorTunnel})).HasValue());
}

TEST_CASE("Routing: one-way streets bind vehicles, not pedestrians") {
    const auto r = MakeSyntheticMapRegion();
    const MapPosition from = At(r, 1400, 300); // on the one-way street, which only runs eastwards
    const MapPosition to = At(r, 1200, 300);   // 200 m behind
    const PlannedRoute walk = MustPlan(r, from, to, Options(TravelMode::OnFoot));
    CHECK(walk.lengthMetres == Approx(200.0));
    const PlannedRoute drive = MustPlan(r, from, to, Options(TravelMode::Driving));
    CheckInvariants(r, drive, from, to);
    CHECK(drive.lengthMetres == Approx(1300.0)); // east to the junction (150), around a block (1050) and back in along the one-way direction (100)
    CHECK(drive.lengthMetres > 6.0 * walk.lengthMetres);
    // ahead of the vehicle there is no detour
    const PlannedRoute ahead = MustPlan(r, from, At(r, 1500, 300), Options(TravelMode::Driving));
    CHECK(ahead.lengthMetres == Approx(100.0));
}

TEST_CASE("Routing: a turn restriction forbids the turn for vehicles only") {
    const auto r = MakeSyntheticMapRegion();
    const MapPosition from = At(r, 1100, 450); // on the road E_F - E_D, heading north would turn east at E_D
    const MapPosition to = At(r, 1325, 600);   // on the east street E_D - E_I
    const PlannedRoute walk = MustPlan(r, from, to, Options(TravelMode::OnFoot));
    CHECK(walk.lengthMetres == Approx(150.0 + 225.0));
    const PlannedRoute drive = MustPlan(r, from, to, Options(TravelMode::Driving));
    CheckInvariants(r, drive, from, to);
    CHECK(drive.lengthMetres == Approx(150.0 + 450.0 + 300.0 + 225.0)); // south, east on the one-way street, north, west back along the east street
    CHECK_FALSE(UsesRoad(drive, kRoadEastDE));
}

TEST_CASE("Routing: dead-end spurs are reachable and left again") {
    const auto r = MakeSyntheticMapRegion();
    const MapPosition deep = At(r, 1550, -240);
    const PlannedRoute in = MustPlan(r, r.eastStreet, deep, Options(TravelMode::Driving));
    CheckInvariants(r, in, r.eastStreet, deep);
    CHECK(in.lengthMetres == Approx(50.0 + 240.0));
    const PlannedRoute out = MustPlan(r, deep, r.westStreet, Options(TravelMode::Driving));
    CheckInvariants(r, out, deep, r.westStreet);
    CHECK(UsesRoad(out, kSpurEastB));
    // the first road leg leaves the spur northwards - no pointless turn-around
    const auto ids = RoadIds(out);
    REQUIRE(ids.size() >= 2);
    CHECK(ids.front() == kSpurEastB);
    CHECK(ids[1] != kSpurEastB);
}

TEST_CASE("Routing: the destination marker stays in the building, the route ends at its entrance") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute p = MustPlan(r, r.westStreet, r.shopInside, Options(TravelMode::OnFoot));
    CheckInvariants(r, p, r.westStreet, r.shopInside);
    CHECK(p.destinationBuilding == BuildingId(kShopWest));
    CHECK(p.accessPoint == Vec2{320, 30}); // the entrance
    CHECK(p.route.destination.position == r.shopInside);
    CHECK(p.roadLengthMetres == Approx(50.0 + 130.0));
    CHECK(p.connectorLengthMetres == Approx(30.0 + 15.0)); // road -> entrance -> destination inside
    REQUIRE(p.legs.size() >= 2);
    CHECK(p.legs.back().kind == LegKind::EndConnector);
    CHECK(p.legs.front().kind == LegKind::Road);
    // the last connector point before the destination is the entrance
    const RouteSegment& last = p.route.segments.back();
    CHECK(last.from.xy == Vec2{320, 30});
}

TEST_CASE("Routing: with two entrances the cheaper way in is used") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute p = MustPlan(r, r.westStreet, r.warehouseInside, Options(TravelMode::OnFoot));
    CheckInvariants(r, p, r.westStreet, r.warehouseInside);
    CHECK(p.destinationBuilding == BuildingId(kWarehouseTwoEntrances));
    CHECK(p.accessPoint == Vec2{730, 40}); // south door, 40 m from the street; the north door is 80 m
    CHECK(p.lengthMetres == Approx(230.0 + 40.0 + 20.0));
}

TEST_CASE("Routing: a building without an entrance needs a road within reach of its position") {
    const auto r = MakeSyntheticMapRegion();
    const auto far = PlanRoute(*r.geometry, r.westStreet, Dest(r.noEntranceInside), Options(TravelMode::OnFoot));
    REQUIRE_FALSE(far.HasValue());
    CHECK(far.Error().code == RouteFailure::DestinationNotAccessible); // 115 m to the nearest road, default reach 100 m
    RouteOptions wide = Options(TravelMode::OnFoot);
    wide.maxDestinationSnapMetres = 130.0;
    const PlannedRoute p = MustPlan(r, r.westStreet, r.noEntranceInside, wide);
    CheckInvariants(r, p, r.westStreet, r.noEntranceInside);
    CHECK(p.destinationBuilding.IsNull()); // no entrance was used
    CHECK(p.connectorLengthMetres == Approx(115.0));
}

TEST_CASE("Routing: water blocks access connectors, so the island target has no route") {
    const auto r = MakeSyntheticMapRegion();
    for (const TravelMode mode : {TravelMode::OnFoot, TravelMode::Driving}) {
        RouteOptions o = Options(mode);
        o.maxDestinationSnapMetres = 2000.0; // even a generous reach does not help: every road is across the water
        o.maxSnapCandidates = 12;
        const auto plan = PlanRoute(*r.geometry, r.westStreet, Dest(r.islandTarget), o);
        REQUIRE_FALSE(plan.HasValue());
        CHECK(plan.Error().code == RouteFailure::DestinationNotAccessible);
        CHECK(plan.Error().stats.goalCandidates == 0);
    }
    // standing on the island there is no way off it either
    const auto off = PlanRoute(*r.geometry, r.islandTarget, Dest(r.westStreet), Options(TravelMode::OnFoot));
    REQUIRE_FALSE(off.HasValue());
    CHECK(off.Error().code == RouteFailure::OriginNotOnNetwork);
}

TEST_CASE("Routing: a position on a bridge deck is on the network, a position in the river is not") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute onDeck = MustPlan(r, At(r, 1000, 1200), r.eastStreet, Options(TravelMode::Driving));
    CHECK(onDeck.connectorLengthMetres == 0.0);
    CHECK(UsesRoad(onDeck, kNorthBridge));
    const PlannedRoute nearDeck = MustPlan(r, At(r, 1000, 1196), r.eastStreet, Options(TravelMode::Driving));
    CHECK(nearDeck.connectorLengthMetres == Approx(4.0)); // a 4 m step onto the deck is allowed
    const auto inRiver = PlanRoute(*r.geometry, At(r, 1000, 1160), Dest(r.eastStreet), Options(TravelMode::Driving));
    REQUIRE_FALSE(inRiver.HasValue());
    CHECK(inRiver.Error().code == RouteFailure::OriginNotOnNetwork);
}

TEST_CASE("Routing: a disconnected road network is reachable by position but not by path") {
    const auto r = MakeSyntheticMapRegion();
    const auto in = PlanRoute(*r.geometry, r.westStreet, Dest(r.hamletTarget), Options(TravelMode::Driving));
    REQUIRE_FALSE(in.HasValue());
    CHECK(in.Error().code == RouteFailure::NoPath); // the hamlet's entrance is on a road, but that road connects to nothing
    CHECK(in.Error().stats.goalCandidates > 0);
    const auto out = PlanRoute(*r.geometry, r.hamletTarget, Dest(r.westStreet), Options(TravelMode::OnFoot));
    REQUIRE_FALSE(out.HasValue());
    CHECK(out.Error().code == RouteFailure::NoPath);
    // inside the hamlet everything works
    const PlannedRoute local = MustPlan(r, At(r, 2650, 1500), At(r, 2900, 1700), Options(TravelMode::OnFoot));
    CHECK(local.lengthMetres == Approx(250.0 + 200.0));
}

TEST_CASE("Routing: an origin far from every road is rejected with OriginNotOnNetwork") {
    const auto r = MakeSyntheticMapRegion();
    const auto plan = PlanRoute(*r.geometry, At(r, 300, 1500), Dest(r.eastStreet), Options(TravelMode::OnFoot));
    REQUIRE_FALSE(plan.HasValue());
    CHECK(plan.Error().code == RouteFailure::OriginNotOnNetwork);
    CHECK(plan.Error().stats.startCandidates == 0);
}

TEST_CASE("Routing: an off-road origin gets a labelled start connector, not a hidden shortcut") {
    const auto r = MakeSyntheticMapRegion();
    const PlannedRoute p = MustPlan(r, r.westOpenField, r.eastStreet, Options(TravelMode::OnFoot));
    CheckInvariants(r, p, r.westOpenField, r.eastStreet);
    REQUIRE(!p.legs.empty());
    CHECK(p.legs.front().kind == LegKind::StartConnector);
    CHECK(p.legs.front().lengthMetres <= 100.0);
    CHECK(p.connectorLengthMetres == Approx(p.legs.front().lengthMetres));
    CHECK(p.roadLengthMetres > 1000.0);
}

TEST_CASE("Routing: invalid input is rejected before any search") {
    const auto r = MakeSyntheticMapRegion();
    CHECK(PlanRoute(*r.geometry, At(r, std::nan(""), 0), Dest(r.eastStreet), Options(TravelMode::OnFoot)).Error().code == RouteFailure::InvalidInput);
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(At(r, 1e12, 0)), Options(TravelMode::OnFoot)).Error().code == RouteFailure::InvalidInput);
    CHECK(PlanRoute(*r.geometry, MapPosition{}, Dest(r.eastStreet), Options(TravelMode::OnFoot)).Error().code == RouteFailure::InvalidInput);
    RouteOptions bad = Options(TravelMode::OnFoot);
    bad.classCostFactor[0] = 0.5; // a factor below 1 would break the A* heuristic
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), bad).Error().code == RouteFailure::InvalidInput);
    bad = Options(TravelMode::OnFoot);
    bad.classCostFactor[3] = std::nan("");
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), bad).Error().code == RouteFailure::InvalidInput);
    bad = Options(TravelMode::OnFoot);
    bad.maxStartSnapMetres = -1.0;
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), bad).Error().code == RouteFailure::InvalidInput);
    bad = Options(TravelMode::OnFoot);
    bad.maxSnapCandidates = 0;
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), bad).Error().code == RouteFailure::InvalidInput);
    // layers: a road route cannot change layer; a layer without roads is reported as such
    CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(MapPosition{MapLayerId(2), Vec2{0, 0}}), Options(TravelMode::OnFoot)).Error().code == RouteFailure::LayerMismatch);
    CHECK(PlanRoute(*r.geometry, MapPosition{MapLayerId(2), Vec2{0, 0}}, Dest(MapPosition{MapLayerId(2), Vec2{9, 9}}), Options(TravelMode::OnFoot)).Error().code ==
          RouteFailure::UnknownLayer);
}

TEST_CASE("Routing: the expansion budget stops a search with an explicit failure") {
    const auto r = MakeSyntheticMapRegion();
    RouteOptions o = Options(TravelMode::Driving);
    o.maxExpansions = 3;
    const auto plan = PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), o);
    REQUIRE_FALSE(plan.HasValue());
    CHECK(plan.Error().code == RouteFailure::SearchLimitReached);
}

TEST_CASE("Routing: class cost factors change the choice but not the reported length") {
    const auto r = MakeSyntheticMapRegion();
    RouteOptions o = Options(TravelMode::OnFoot);
    o.classCostFactor[static_cast<std::size_t>(RoadClass::Footway)] = 10.0; // pedestrians avoid footways
    const PlannedRoute p = MustPlan(r, r.westStreet, r.eastStreet, o);
    CHECK_FALSE(UsesRoad(p, kFootBridge));
    CHECK(UsesRoad(p, kNorthBridge));
    CHECK(p.lengthMetres == Approx(3400.0)); // the real length, not the weighted cost
}

TEST_CASE("Routing: alternatives are distinct, valid and sorted, the first is the best route") {
    const auto r = MakeSyntheticMapRegion();
    const RouteOptions o = Options(TravelMode::Driving);
    const auto alt = PlanRouteAlternatives(*r.geometry, r.westStreet, Dest(r.eastStreet), o, 3);
    REQUIRE(alt.HasValue());
    const auto& routes = alt.Value();
    REQUIRE(routes.size() >= 2);
    CHECK(routes.size() <= 3);
    const PlannedRoute best = MustPlan(r, r.westStreet, r.eastStreet, o);
    CHECK(routes.front().lengthMetres == Approx(best.lengthMetres));
    CHECK(RoadIds(routes.front()) == RoadIds(best));
    for (std::size_t i = 0; i < routes.size(); ++i) {
        CheckInvariants(r, routes[i], r.westStreet, r.eastStreet);
        CHECK(routes[i].lengthMetres + 1e-9 >= best.lengthMetres);
        CHECK(routes[i].lengthMetres <= best.lengthMetres * 1.6 + 1e-9);
        for (std::size_t j = i + 1; j < routes.size(); ++j) { CHECK(RoadIds(routes[i]) != RoadIds(routes[j])); }
        if (i > 1) { CHECK(routes[i - 1].lengthMetres <= routes[i].lengthMetres); }
    }
    // an unreachable destination reports the failure instead of an empty list
    const auto none = PlanRouteAlternatives(*r.geometry, r.westStreet, Dest(r.hamletTarget), o, 3);
    REQUIRE_FALSE(none.HasValue());
    CHECK(none.Error().code == RouteFailure::NoPath);
    CHECK(PlanRouteAlternatives(*r.geometry, r.westStreet, Dest(r.eastStreet), o, 1).Value().size() == 1);
}

TEST_CASE("Routing: results do not depend on the order of the input data") {
    const auto r = MakeSyntheticMapRegion();
    MapGeometryData shuffled = MakeSyntheticGeometryData();
    std::reverse(shuffled.nodes.begin(), shuffled.nodes.end());
    std::reverse(shuffled.segments.begin(), shuffled.segments.end());
    std::reverse(shuffled.buildings.begin(), shuffled.buildings.end());
    std::rotate(shuffled.areas.begin(), shuffled.areas.begin() + 4, shuffled.areas.end());
    const auto g = MapGeometry::Create(*r.definition, std::move(shuffled));
    REQUIRE(g.HasValue());
    for (const TravelMode mode : {TravelMode::OnFoot, TravelMode::Driving}) {
        for (const MapPosition& to : {r.eastStreet, r.shopInside, r.warehouseInside, r.houseInside}) {
            const auto a = PlanRoute(*r.geometry, r.westStreet, Dest(to), Options(mode));
            const auto b = PlanRoute(g.Value(), r.westStreet, Dest(to), Options(mode));
            REQUIRE(a.HasValue());
            REQUIRE(b.HasValue());
            CHECK(a.Value().route == b.Value().route);
            CHECK(RoadIds(a.Value()) == RoadIds(b.Value()));
            CHECK(a.Value().stats == b.Value().stats);
        }
    }
    // repeated runs are identical, too
    const auto first = PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), Options(TravelMode::Driving));
    for (int i = 0; i < 5; ++i) {
        CHECK(PlanRoute(*r.geometry, r.westStreet, Dest(r.eastStreet), Options(TravelMode::Driving)).Value().route == first.Value().route);
    }
}

TEST_CASE("Routing: equal-cost routes resolve the same way every time") {
    // Two parallel streets of identical length between the same nodes: the lower segment id must win, in both input orders.
    const MapLayerId layer(1);
    const auto node = [&](std::uint64_t id, double x, double y) { return RoadNodeDef{RoadNodeId(id), MapPosition{layer, Vec2{x, y}}}; };
    const auto seg = [&](std::uint64_t id, std::uint64_t a, std::uint64_t b, std::vector<Vec2> shape) {
        RoadSegmentDef s;
        s.id = RoadSegmentId(id);
        s.from = RoadNodeId(a);
        s.to = RoadNodeId(b);
        s.shape = std::move(shape);
        return s;
    };
    MapGeometryData d;
    d.nodes = {node(1, 0, 0), node(2, 100, 0)};
    d.segments = {seg(10, 1, 2, {{50, 20}}), seg(20, 1, 2, {{50, -20}})};
    const auto def = MakeSyntheticMapDefinition();
    const auto g1 = MapGeometry::Create(*def, d);
    std::reverse(d.segments.begin(), d.segments.end());
    const auto g2 = MapGeometry::Create(*def, d);
    REQUIRE(g1.HasValue());
    REQUIRE(g2.HasValue());
    const MapPosition from{layer, Vec2{0, 0}};
    const MapPosition to{layer, Vec2{100, 0}};
    const auto a = PlanRoute(g1.Value(), from, Dest(to), Options(TravelMode::OnFoot));
    const auto b = PlanRoute(g2.Value(), from, Dest(to), Options(TravelMode::OnFoot));
    REQUIRE(a.HasValue());
    REQUIRE(b.HasValue());
    CHECK(a.Value().route == b.Value().route);
    CHECK(RoadIds(a.Value()) == RoadIds(b.Value()));
}

TEST_CASE("Routing: a route stays correct 5000 km from the origin of the coordinate system") {
    const auto r = MakeSyntheticMapRegion();
    constexpr double kShiftX = 5'000'000.0;
    constexpr double kShiftY = 3'000'000.0;
    MapGeometryData d = MakeSyntheticGeometryData();
    for (auto& n : d.nodes) { n.position.xy.x += kShiftX; n.position.xy.y += kShiftY; }
    for (auto& s : d.segments) { for (auto& p : s.shape) { p.x += kShiftX; p.y += kShiftY; } s.region = ContentId{}; }
    for (auto& b : d.buildings) { for (auto& p : b.footprint) { p.x += kShiftX; p.y += kShiftY; } for (auto& p : b.entrances) { p.x += kShiftX; p.y += kShiftY; } }
    for (auto& a : d.areas) { for (auto& p : a.outline) { p.x += kShiftX; p.y += kShiftY; } a.region = ContentId{}; }
    for (auto& l : d.lines) { for (auto& p : l.points) { p.x += kShiftX; p.y += kShiftY; } }
    MapLayerDef layer;
    layer.id = r.layer;
    layer.key = *ContentId::Parse("synthetic:layer.far");
    layer.bounds = MapRect{{-kMaxMapCoordinateMetres, -kMaxMapCoordinateMetres}, {kMaxMapCoordinateMetres, kMaxMapCoordinateMetres}};
    const auto def = WorldMapDefinition::Create({layer}, {}, {});
    REQUIRE(def.HasValue());
    const auto far = MapGeometry::Create(def.Value(), std::move(d));
    REQUIRE(far.HasValue());
    const auto shift = [&](MapPosition p) { p.xy.x += kShiftX; p.xy.y += kShiftY; return p; };
    for (const TravelMode mode : {TravelMode::OnFoot, TravelMode::Driving}) {
        for (const MapPosition& to : {r.eastStreet, r.shopInside, r.warehouseInside}) {
            const auto near = PlanRoute(*r.geometry, r.westStreet, Dest(to), Options(mode));
            const auto moved = PlanRoute(far.Value(), shift(r.westStreet), Dest(shift(to)), Options(mode));
            REQUIRE(near.HasValue());
            REQUIRE(moved.HasValue());
            CHECK(moved.Value().lengthMetres == Approx(near.Value().lengthMetres).epsilon(1e-9));
            CHECK(RoadIds(moved.Value()) == RoadIds(near.Value()));
            CHECK(moved.Value().route.segments.size() == near.Value().route.segments.size());
        }
    }
}

TEST_CASE("Routing: two region packages merged route exactly like the whole") {
    const auto r = MakeSyntheticMapRegion();
    const MapGeometryData whole = MakeSyntheticGeometryData();
    MapGeometryData west;
    MapGeometryData east;
    const auto nodeById = [&](RoadNodeId id) { return *std::find_if(whole.nodes.begin(), whole.nodes.end(), [&](const RoadNodeDef& n) { return n.id == id; }); };
    const auto addNode = [](MapGeometryData& d, const RoadNodeDef& n) {
        if (std::none_of(d.nodes.begin(), d.nodes.end(), [&](const RoadNodeDef& x) { return x.id == n.id; })) { d.nodes.push_back(n); }
    };
    for (const RoadSegmentDef& s : whole.segments) {
        MapGeometryData& target = nodeById(s.from).position.xy.x <= 1000.0 ? west : east; // by start node; border nodes are copied into both
        target.segments.push_back(s);
        addNode(target, nodeById(s.from));
        addNode(target, nodeById(s.to));
    }
    for (const TurnRestriction& t : whole.restrictions) { (nodeById(t.via).position.xy.x <= 1000.0 ? west : east).restrictions.push_back(t); }
    for (const BuildingDef& b : whole.buildings) { (b.footprint[0].x <= 1000.0 ? west : east).buildings.push_back(b); }
    for (const AreaDef& a : whole.areas) { (a.outline[0].x <= 1000.0 ? west : east).areas.push_back(a); }
    for (const LineFeatureDef& l : whole.lines) { (l.points[0].x <= 1000.0 ? west : east).lines.push_back(l); }
    // restriction segments must exist in the package: copy the two segments of the east restriction (both are east already)
    REQUIRE(RoadNetwork::Create(west.nodes, west.segments, west.restrictions).HasValue());
    REQUIRE(RoadNetwork::Create(east.nodes, east.segments, east.restrictions).HasValue());
    // each package alone cannot cross the river
    const auto westOnly = MapGeometry::Create(*r.definition, west);
    REQUIRE(westOnly.HasValue());
    CHECK(PlanRoute(westOnly.Value(), r.westStreet, Dest(r.eastStreet), Options(TravelMode::Driving)).HasValue() == false);
    const auto merged = MergeGeometryData(west, east);
    REQUIRE(merged.HasValue());
    const auto geometry = MapGeometry::Create(*r.definition, merged.Value());
    REQUIRE(geometry.HasValue());
    for (const TravelMode mode : {TravelMode::OnFoot, TravelMode::Driving}) {
        const auto a = PlanRoute(*r.geometry, r.westStreet, Dest(r.houseInside), Options(mode));
        const auto b = PlanRoute(geometry.Value(), r.westStreet, Dest(r.houseInside), Options(mode));
        REQUIRE(a.HasValue());
        REQUIRE(b.HasValue());
        CHECK(a.Value().route == b.Value().route);
    }
    CHECK(merged.Value() == MakeSyntheticGeometryData()); // the merge reproduces the original data exactly (sorted)
}

namespace {

/// n x n street grid with `spacing` metres between nodes. If wallAfterColumn >= 0 the east-west links between that column and the next exist only in
/// `gapRow`. Node id = row * n + column + 1.
struct GridWorld {
    std::shared_ptr<const WorldMapDefinition> definition;
    std::shared_ptr<const MapGeometry> geometry;
    MapLayerId layer{1};
};

GridWorld MakeGrid(int n, double spacing, int wallAfterColumn = -1, int gapRow = 0) {
    const MapLayerId layer(1);
    MapLayerDef l;
    l.id = layer;
    l.key = *ContentId::Parse("synthetic:layer.grid");
    l.bounds = MapRect{{-10000, -10000}, {100000, 100000}};
    auto def = WorldMapDefinition::Create({l}, {}, {});
    MapGeometryData d;
    const auto id = [&](int row, int col) { return static_cast<std::uint64_t>(row * n + col + 1); };
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            d.nodes.push_back(RoadNodeDef{RoadNodeId(id(row, col)), MapPosition{layer, Vec2{col * spacing, row * spacing}}});
        }
    }
    std::uint64_t seg = 1;
    const auto link = [&](std::uint64_t a, std::uint64_t b) {
        RoadSegmentDef s;
        s.id = RoadSegmentId(seg++);
        s.from = RoadNodeId(a);
        s.to = RoadNodeId(b);
        d.segments.push_back(s);
    };
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if (col + 1 < n && !(col == wallAfterColumn && row != gapRow)) { link(id(row, col), id(row, col + 1)); }
            if (row + 1 < n) { link(id(row, col), id(row + 1, col)); }
        }
    }
    GridWorld g;
    g.layer = layer;
    g.definition = std::make_shared<const WorldMapDefinition>(std::move(def).Value());
    auto geometry = MapGeometry::Create(*g.definition, std::move(d), 500.0);
    g.geometry = std::make_shared<const MapGeometry>(std::move(geometry).Value());
    return g;
}

} // namespace

TEST_CASE("Routing at scale: corner to corner on a 150 x 150 street grid takes the Manhattan distance") {
    const GridWorld g = MakeGrid(150, 50.0);
    const std::size_t segments = g.geometry->Roads().Segments().size();
    CHECK(segments == 2 * 150 * 149);
    const MapPosition from{g.layer, Vec2{0, 0}};
    const MapPosition to{g.layer, Vec2{149 * 50.0, 149 * 50.0}};
    RouteOptions o;
    o.mode = TravelMode::Driving;
    const auto plan = PlanRoute(*g.geometry, from, Dest(to), o);
    REQUIRE(plan.HasValue());
    CHECK(plan.Value().lengthMetres == Approx(2.0 * 149 * 50.0));
    CHECK(ValidateRouteFrom(plan.Value().route, from) == RouteValidity::Valid);
    CHECK(plan.Value().stats.statesExpanded <= 2 * segments); // each directed segment at most once
    // determinism at scale
    const auto again = PlanRoute(*g.geometry, from, Dest(to), o);
    REQUIRE(again.HasValue());
    CHECK(again.Value().route == plan.Value().route);
    CHECK(again.Value().stats == plan.Value().stats);
}

TEST_CASE("Routing at scale: a short route inside a large network does not search the network") {
    const GridWorld g = MakeGrid(150, 50.0);
    const std::size_t segments = g.geometry->Roads().Segments().size();
    RouteOptions o;
    o.mode = TravelMode::OnFoot;
    const auto plan = PlanRoute(*g.geometry, MapPosition{g.layer, Vec2{3500, 3500}}, Dest(MapPosition{g.layer, Vec2{3650, 3600}}), o);
    REQUIRE(plan.HasValue());
    CHECK(plan.Value().lengthMetres == Approx(250.0));
    CHECK(plan.Value().stats.statesExpanded < 200);        // A* stays near the straight line between the points
    CHECK(plan.Value().stats.statesExpanded * 100 < segments); // < 1 % of the network
    // an unreachable goal is the worst case: the whole connected network is searched once, then NoPath
    MapGeometryData island = g.geometry->ToData();
    island.nodes.push_back(RoadNodeDef{RoadNodeId(900001), MapPosition{g.layer, Vec2{-5000, -5000}}});
    island.nodes.push_back(RoadNodeDef{RoadNodeId(900002), MapPosition{g.layer, Vec2{-5000, -4900}}});
    RoadSegmentDef s;
    s.id = RoadSegmentId(900001);
    s.from = RoadNodeId(900001);
    s.to = RoadNodeId(900002);
    island.segments.push_back(s);
    const auto withIsland = MapGeometry::Create(*g.definition, std::move(island));
    REQUIRE(withIsland.HasValue());
    const auto none = PlanRoute(withIsland.Value(), MapPosition{g.layer, Vec2{0, 0}}, Dest(MapPosition{g.layer, Vec2{-5000, -4950}}), o);
    REQUIRE_FALSE(none.HasValue());
    CHECK(none.Error().code == RouteFailure::NoPath);
    CHECK(none.Error().stats.statesExpanded <= 2 * segments);
}

TEST_CASE("Routing at scale: a wall with one gap forces the detour through the gap") {
    constexpr int n = 60;
    constexpr double spacing = 40.0;
    const GridWorld g = MakeGrid(n, spacing, /*wallAfterColumn=*/29, /*gapRow=*/n - 1);
    RouteOptions o;
    o.mode = TravelMode::Driving;
    const MapPosition from{g.layer, Vec2{0, 0}};
    const MapPosition to{g.layer, Vec2{(n - 1) * spacing, 0}};
    const auto plan = PlanRoute(*g.geometry, from, Dest(to), o);
    REQUIRE(plan.HasValue());
    // up to the gap row, across, back down: the straight distance plus twice the way to the gap
    CHECK(plan.Value().lengthMetres == Approx((n - 1) * spacing + 2.0 * (n - 1) * spacing));
    CHECK(plan.Value().straightLineMetres == Approx((n - 1) * spacing));
    // every east-west crossing of the wall happens in the gap row
    for (const RouteSegment& s : plan.Value().route.segments) {
        const bool crosses = (s.from.xy.x < 29.5 * spacing) != (s.to.xy.x < 29.5 * spacing);
        if (crosses) { CHECK(s.from.xy.y == Approx((n - 1) * spacing)); }
    }
    // closing the single gap link makes the east side unreachable
    RouteOptions closed = o;
    const auto gapSegment = g.geometry->Roads().Snap(MapPosition{g.layer, Vec2{29.5 * spacing, (n - 1) * spacing}}, 1.0, SnapRules{TravelMode::Driving}, 1);
    REQUIRE(gapSegment.size() == 1);
    closed.closedSegments.insert(gapSegment.front().segment);
    const auto blocked = PlanRoute(*g.geometry, from, Dest(to), closed);
    REQUIRE_FALSE(blocked.HasValue());
    CHECK(blocked.Error().code == RouteFailure::NoPath);
}
