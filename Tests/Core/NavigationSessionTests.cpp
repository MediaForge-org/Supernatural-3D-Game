#include <cmath>
#include <doctest.h>
#include <spn/core/NavigationSession.h>
#include <spn/core/SyntheticMapRegion.h>

using namespace spn::core;
using namespace spn::core::synthetic;
using doctest::Approx;

namespace {

MapPosition At(double x, double y) { return MapPosition{MapLayerId(kLayerId), Vec2{x, y}}; }

NavigationConfig Config(TravelMode mode) {
    NavigationConfig c;
    c.options.mode = mode;
    return c;
}

struct Fixture {
    SyntheticMapRegion region = MakeSyntheticMapRegion();
    WorldMapState map{region.definition};
    NavigationSession session;
    explicit Fixture(TravelMode mode = TravelMode::Driving) : session(region.geometry, Config(mode)) {}
    MapPosition At(double x, double y) const { return MapPosition{region.layer, Vec2{x, y}}; }
};

} // namespace

TEST_CASE("Navigation: setting a waypoint creates the marker at the target and a road route to it") {
    Fixture f;
    const auto marker = f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "Test", 0.0);
    REQUIRE(marker.HasValue());
    const Marker* m = f.map.FindMarker(marker.Value());
    REQUIRE(m != nullptr);
    CHECK(m->position == f.region.eastStreet); // the marker stays at the real target
    CHECK(m->category == PlayerWaypointCategory());
    CHECK(f.session.Status() == NavigationStatus::Active);
    const PlannedRoute* route = f.session.CurrentRoute();
    REQUIRE(route != nullptr);
    CHECK(route->lengthMetres == Approx(3400.0)); // over the bridge, not 1000 m straight
    CHECK(route->route.destination.marker == marker.Value());
    CHECK(ValidateRouteFrom(route->route, f.region.westStreet) == RouteValidity::Valid);
    CHECK(f.session.OwnedWaypoint() == marker.Value());
    CHECK(f.session.Generation() == 1);
}

TEST_CASE("Navigation: a new waypoint replaces the session's previous one") {
    Fixture f;
    const MarkerId first = f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "A", 0.0).Value();
    const MarkerId second = f.session.SetWaypoint(f.map, f.region.westStreet, f.region.shopInside, "B", 1.0).Value();
    CHECK(second != first);
    CHECK(f.map.FindMarker(first) == nullptr);
    CHECK(f.map.FindMarker(second) != nullptr);
    CHECK(f.map.MarkerCount() == 1);
    CHECK(f.session.CurrentRoute()->destinationBuilding == BuildingId(kShopWest));
    // a waypoint the session did not create is left alone
    const MarkerId foreign = f.map.AddPlayerWaypoint(f.region.houseInside, "mine").Value();
    (void)f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "C", 2.0);
    CHECK(f.map.FindMarker(foreign) != nullptr);
    CHECK(f.map.MarkerCount() == 2);
}

TEST_CASE("Navigation: remaining distance counts down along the real route") {
    Fixture f;
    REQUIRE(f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "", 0.0).HasValue());
    const Route route = f.session.CurrentRoute()->route;
    const double total = f.session.CurrentRoute()->lengthMetres;
    double travelled = 0.0;
    double lastRemaining = total + 1.0;
    double now = 0.0;
    for (const RouteSegment& s : route.segments) {
        const double len = SegmentLength(s);
        const Vec2 mid{(s.from.xy.x + s.to.xy.x) * 0.5, (s.from.xy.y + s.to.xy.y) * 0.5};
        now += 0.5;
        const NavigationUpdate u = f.session.Update(MapPosition{f.region.layer, mid}, now, &f.map);
        if (u.status == NavigationStatus::Arrived) { break; }
        REQUIRE(u.status == NavigationStatus::Active);
        CHECK_FALSE(u.offRoute);
        CHECK_FALSE(u.routeChanged);
        CHECK(u.offRouteMetres == Approx(0.0).epsilon(1e-9));
        CHECK(u.remainingMetres == Approx(total - travelled - len * 0.5).epsilon(1e-9));
        CHECK(u.remainingMetres < lastRemaining);
        lastRemaining = u.remainingMetres;
        travelled += len;
    }
    CHECK(f.session.Generation() == 1); // never re-planned while on the route
}

TEST_CASE("Navigation: arrival is detected within the destination radius and stays arrived") {
    Fixture f(TravelMode::OnFoot);
    REQUIRE(f.session.SetWaypoint(f.map, f.region.westStreet, f.region.shopInside, "", 0.0).HasValue());
    CHECK(f.session.Update(At(320, 31), 1.0, &f.map).status == NavigationStatus::Active); // at the entrance, 14 m from the marker
    const NavigationUpdate u = f.session.Update(At(320, 44), 2.0, &f.map);                  // within 3 m of the marker
    CHECK(u.status == NavigationStatus::Arrived);
    CHECK(u.remainingMetres == 0.0);
    CHECK(f.session.Update(At(320, 44), 3.0, &f.map).status == NavigationStatus::Arrived);
    // a wider arrival radius arrives earlier
    Fixture g(TravelMode::OnFoot);
    REQUIRE(g.session.SetWaypoint(g.map, g.region.westStreet, g.region.shopInside, "", 0.0, 20.0).HasValue());
    CHECK(g.session.Update(At(320, 31), 1.0, &g.map).status == NavigationStatus::Arrived);
    // walking away resumes guidance
    const NavigationUpdate away = f.session.Update(At(400, 0), 10.0, &f.map);
    CHECK(away.status == NavigationStatus::Active);
    CHECK(away.routeChanged);
}

TEST_CASE("Navigation: leaving the route triggers a re-plan after the cooldown, from the new position") {
    Fixture f;
    REQUIRE(f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "", 0.0).HasValue());
    const std::uint64_t gen = f.session.Generation();
    const MapPosition lost = At(100, 0); // on the west street, but the route runs east from (500, 0)
    const NavigationUpdate early = f.session.Update(lost, 0.5, &f.map);
    CHECK(early.offRoute);
    CHECK_FALSE(early.rerouted); // cooldown (2 s) not over
    CHECK(early.offRouteMetres > 30.0);
    CHECK(f.session.Generation() == gen);
    const NavigationUpdate later = f.session.Update(lost, 3.0, &f.map);
    CHECK(later.rerouted);
    CHECK(later.routeChanged);
    CHECK(later.status == NavigationStatus::Active);
    CHECK_FALSE(later.offRoute);
    CHECK(f.session.Generation() == gen + 1);
    const PlannedRoute* route = f.session.CurrentRoute();
    REQUIRE(route != nullptr);
    CHECK(route->route.segments.front().from == lost);
    CHECK(route->lengthMetres == Approx(3400.0 + 400.0)); // 400 m further than from (500, 0)
    CHECK(later.remainingMetres == Approx(route->lengthMetres));
}

TEST_CASE("Navigation: a closed road re-plans the route, an unrelated closure does not") {
    Fixture f;
    REQUIRE(f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "", 0.0).HasValue());
    const std::uint64_t gen = f.session.Generation();
    const NavigationUpdate unrelated = f.session.SetClosedSegments({RoadSegmentId(kHamletRoadAB)}, f.region.westStreet, 1.0);
    CHECK_FALSE(unrelated.routeChanged);
    CHECK(f.session.Generation() == gen);
    const NavigationUpdate closed = f.session.SetClosedSegments({RoadSegmentId(kNorthBridge)}, f.region.westStreet, 2.0);
    CHECK(closed.routeChanged);
    CHECK(closed.rerouted);
    CHECK(closed.status == NavigationStatus::Active);
    CHECK(f.session.CurrentRoute()->lengthMetres > 4000.0); // the tunnel detour
    CHECK_FALSE(RouteUsesAny(*f.session.CurrentRoute(), {RoadSegmentId(kNorthBridge)}));
    // everything across the river closed: an explicit NoRoute, no stale route left to follow
    const NavigationUpdate none = f.session.SetClosedSegments({RoadSegmentId(kNorthBridge), RoadSegmentId(kMotorTunnel)}, f.region.westStreet, 3.0);
    CHECK(none.status == NavigationStatus::NoRoute);
    CHECK(none.failure == RouteFailure::NoPath);
    CHECK(f.session.CurrentRoute() == nullptr);
    CHECK(f.map.FindMarker(f.session.OwnedWaypoint()) != nullptr); // the marker is still at the target
    // reopening: the next update after the cooldown finds a route again
    (void)f.session.SetClosedSegments({}, f.region.westStreet, 3.5);
    const NavigationUpdate back = f.session.Update(f.region.westStreet, 3.6, &f.map);
    CHECK(back.status == NavigationStatus::Active);
    CHECK(f.session.CurrentRoute()->lengthMetres == Approx(3400.0));
}

TEST_CASE("Navigation: moving the marker changes the destination, removing it aborts the route") {
    Fixture f(TravelMode::OnFoot);
    const MarkerId marker = f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "", 0.0).Value();
    const std::uint64_t gen = f.session.Generation();
    REQUIRE(f.map.MoveMarker(marker, f.region.houseInside).has_value() == false);
    const NavigationUpdate moved = f.session.Update(f.region.westStreet, 1.0, &f.map);
    CHECK(moved.routeChanged);
    CHECK(f.session.Generation() == gen + 1);
    CHECK(f.session.CurrentDestination()->position == f.region.houseInside);
    CHECK(f.session.CurrentRoute()->destinationBuilding == BuildingId(kHouseEast));
    CHECK(f.session.CurrentRoute()->route.segments.back().to == f.region.houseInside);

    REQUIRE_FALSE(f.map.RemoveMarker(marker).has_value());
    const NavigationUpdate gone = f.session.Update(f.region.westStreet, 2.0, &f.map);
    CHECK(gone.status == NavigationStatus::Cancelled);
    CHECK(f.session.CurrentRoute() == nullptr);
    CHECK_FALSE(f.session.CurrentDestination().has_value());
    CHECK(f.session.Update(f.region.westStreet, 3.0, &f.map).status == NavigationStatus::Cancelled);
}

TEST_CASE("Navigation: an unreachable target is reported, with the waypoint still set") {
    Fixture f;
    const MarkerId hamlet = f.session.SetWaypoint(f.map, f.region.westStreet, f.region.hamletTarget, "", 0.0).Value();
    CHECK(f.session.Status() == NavigationStatus::NoRoute);
    CHECK(f.session.LastFailure() == RouteFailure::NoPath);
    CHECK(f.session.CurrentRoute() == nullptr);
    CHECK(f.map.FindMarker(hamlet)->position == f.region.hamletTarget);
    const NavigationUpdate u = f.session.Update(f.region.westStreet, 5.0, &f.map);
    CHECK(u.status == NavigationStatus::NoRoute);
    CHECK(u.failure == RouteFailure::NoPath);

    const MarkerId island = f.session.SetWaypoint(f.map, f.region.westStreet, f.region.islandTarget, "", 6.0).Value();
    CHECK(f.map.FindMarker(hamlet) == nullptr);
    CHECK(f.session.LastFailure() == RouteFailure::DestinationNotAccessible);

    f.session.Cancel(&f.map); // abort removes the session's waypoint
    CHECK(f.map.FindMarker(island) == nullptr);
    CHECK(f.session.Status() == NavigationStatus::Cancelled);
    CHECK(f.session.OwnedWaypoint().IsNull());
}

TEST_CASE("Navigation: the destination changes cleanly with a new SetDestination") {
    Fixture f(TravelMode::OnFoot);
    Destination a;
    a.position = f.region.eastStreet;
    REQUIRE(f.session.SetDestination(f.region.westStreet, a, 0.0).status == NavigationStatus::Active);
    const double lenA = f.session.CurrentRoute()->lengthMetres;
    Destination b;
    b.position = f.region.shopInside;
    const NavigationUpdate u = f.session.SetDestination(f.region.westStreet, b, 1.0);
    CHECK(u.routeChanged);
    CHECK(f.session.CurrentRoute()->lengthMetres < lenA);
    CHECK(f.session.CurrentRoute()->route.destination.position == f.region.shopInside);
    CHECK(f.session.Generation() == 2);
}

TEST_CASE("Navigation: invalid positions and an idle session never crash or change state") {
    Fixture f;
    CHECK(f.session.Update(f.region.westStreet, 0.0).status == NavigationStatus::Idle);
    CHECK(f.session.SetClosedSegments({}, f.region.westStreet, 0.0).status == NavigationStatus::Idle);
    REQUIRE(f.session.SetWaypoint(f.map, f.region.westStreet, f.region.eastStreet, "", 0.0).HasValue());
    const double before = f.session.Update(f.region.westStreet, 1.0, &f.map).remainingMetres;
    const NavigationUpdate nan = f.session.Update(MapPosition{f.region.layer, Vec2{std::nan(""), 0}}, 2.0, &f.map);
    CHECK(nan.status == NavigationStatus::Active);
    CHECK(f.session.Update(f.region.westStreet, 3.0, &f.map).remainingMetres == Approx(before));
    // a waypoint outside the map is refused and leaves the session as it was
    const auto bad = f.session.SetWaypoint(f.map, f.region.westStreet, MapPosition{f.region.layer, Vec2{99999, 0}}, "", 4.0);
    CHECK_FALSE(bad.HasValue());
    CHECK(f.session.Status() == NavigationStatus::Active);
}

TEST_CASE("Navigation: a long route is tracked with a small projection window") {
    // 2 km of 10 m road pieces: the window (48 segments) is far smaller than the route
    const MapLayerId layer(kLayerId);
    MapGeometryData d;
    for (std::uint64_t i = 0; i <= 200; ++i) {
        d.nodes.push_back(RoadNodeDef{RoadNodeId(i + 1), MapPosition{layer, Vec2{10.0 * static_cast<double>(i), 0}}});
    }
    for (std::uint64_t i = 0; i < 200; ++i) {
        RoadSegmentDef s;
        s.id = RoadSegmentId(i + 1);
        s.from = RoadNodeId(i + 1);
        s.to = RoadNodeId(i + 2);
        d.segments.push_back(s);
    }
    const auto def = MakeSyntheticMapDefinition();
    auto g = MapGeometry::Create(*def, std::move(d));
    REQUIRE(g.HasValue());
    NavigationSession session(std::make_shared<const MapGeometry>(std::move(g).Value()), Config(TravelMode::OnFoot));
    Destination dest;
    dest.position = MapPosition{layer, Vec2{2000, 0}};
    REQUIRE(session.SetDestination(MapPosition{layer, Vec2{0, 0}}, dest, 0.0).status == NavigationStatus::Active);
    for (int step = 1; step <= 195; ++step) {
        const double x = 10.0 * step + 3.0;
        const NavigationUpdate u = session.Update(MapPosition{layer, Vec2{x, 0}}, 0.1 * step);
        REQUIRE(u.status == NavigationStatus::Active);
        REQUIRE(u.remainingMetres == Approx(2000.0 - x));
        REQUIRE_FALSE(u.routeChanged);
    }
    // a jump far ahead (teleport) is found by the fallback search over the whole route, without re-planning
    const NavigationUpdate jump = session.Update(MapPosition{layer, Vec2{1900, 2}}, 50.0);
    CHECK(jump.remainingMetres == Approx(100.0));
    CHECK_FALSE(jump.routeChanged);
}
