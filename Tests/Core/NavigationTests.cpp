#include <cmath>
#include <doctest.h>
#include <spn/core/Navigation.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

Destination Dest(MapPosition p, double radius = 0.0) {
    Destination d;
    d.position = p;
    d.arrivalRadiusMetres = radius;
    return d;
}

RouteSegment Travel(MapPosition a, MapPosition b, TravelMode mode = TravelMode::OnFoot) { return RouteSegment{SegmentKind::Travel, mode, a, b}; }
RouteSegment Transition(MapPosition a, MapPosition b) { return RouteSegment{SegmentKind::LayerTransition, TravelMode::OnFoot, a, b}; }

/// Surface walk to a building door, step through the door onto floor 0, walk to the target: a route across two layers.
Route HouseRoute() {
    Route r;
    r.source = RouteSource::External;
    r.destination = Dest(At(kFloor0, 10, 0));
    r.segments = {Travel(At(kSurface, 0, 0), At(kSurface, 30, 40), TravelMode::Driving), Travel(At(kSurface, 30, 40), At(kSurface, 30, 50)),
                  Transition(At(kSurface, 30, 50), At(kFloor0, 0, 0)), Travel(At(kFloor0, 0, 0), At(kFloor0, 10, 0))};
    return r;
}

} // namespace

TEST_CASE("Destinations resolve from POIs, markers and free positions") {
    WorldMapState map(MakeTestDefinition());
    const auto poi = DestinationFromPoi(map, Cid("test:poi.shop"), 5.0);
    REQUIRE(poi.HasValue());
    CHECK(poi.Value().position == At(kSurface, 100, 0));
    CHECK(poi.Value().poi == Cid("test:poi.shop"));
    CHECK(poi.Value().arrivalRadiusMetres == 5.0);
    CHECK(DestinationFromPoi(map, Cid("test:poi.none")).Error().code == NavigationErrorCode::UnknownPoi);
    CHECK(DestinationFromPoi(map, Cid("test:region.cave")).Error().code == NavigationErrorCode::UnknownPoi); // regions are not destinations

    const MarkerId wp = map.AddPlayerWaypoint(At(kSurface, 40, 50)).Value();
    const auto marker = DestinationFromMarker(map, wp);
    REQUIRE(marker.HasValue());
    CHECK(marker.Value().marker == wp);
    CHECK(marker.Value().position == At(kSurface, 40, 50));
    (void)map.MoveMarker(wp, At(kSurface, 60, 70));
    CHECK(DestinationFromMarker(map, wp).Value().position == At(kSurface, 60, 70)); // follows the marker
    (void)map.RemoveMarker(wp);
    CHECK(DestinationFromMarker(map, wp).Error().code == NavigationErrorCode::UnknownMarker);

    CHECK(DestinationFromPosition(map, At(kSurface, 10, 10)).HasValue());
    CHECK(DestinationFromPosition(map, At(99, 0, 0)).Error().code == NavigationErrorCode::UnknownLayer);
    CHECK(DestinationFromPosition(map, At(kSurface, 31000, 0)).Error().code == NavigationErrorCode::InvalidPosition);
    CHECK(DestinationFromPosition(map, At(kSurface, std::nan(""), 0)).Error().code == NavigationErrorCode::InvalidPosition);
    for (const double bad : {-1.0, std::nan(""), kInf}) {
        CHECK(DestinationFromPosition(map, At(kSurface, 0, 0), bad).Error().code == NavigationErrorCode::InvalidRadius);
        CHECK(DestinationFromPoi(map, Cid("test:poi.shop"), bad).Error().code == NavigationErrorCode::InvalidRadius);
    }
}

TEST_CASE("Straight-line route is labelled as such and covers exactly one segment") {
    const Destination d = Dest(At(kSurface, 300, 400), 2.0);
    const auto route = MakeStraightLineRoute(At(kSurface, 0, 0), d, TravelMode::Driving);
    REQUIRE(route.HasValue());
    CHECK(route.Value().source == RouteSource::StraightLine);
    REQUIRE(route.Value().segments.size() == 1);
    CHECK(route.Value().segments[0].mode == TravelMode::Driving);
    CHECK(RouteLength(route.Value()) == doctest::Approx(500.0));
    CHECK(ValidateRoute(route.Value()) == RouteValidity::Valid);
    CHECK(ValidateRouteFrom(route.Value(), At(kSurface, 0, 0)) == RouteValidity::Valid);
    CHECK(MakeStraightLineRoute(At(kUnderground, 0, 0), d, TravelMode::OnFoot).Error().code == NavigationErrorCode::LayerMismatch);
    CHECK(MakeStraightLineRoute(At(kSurface, std::nan(""), 0), d, TravelMode::OnFoot).Error().code == NavigationErrorCode::InvalidPosition);
    CHECK(MakeStraightLineRoute(MapPosition{}, d, TravelMode::OnFoot).Error().code == NavigationErrorCode::InvalidPosition); // null layer
}

TEST_CASE("Route validation names the first structural problem") {
    Route empty;
    CHECK(ValidateRoute(empty) == RouteValidity::Empty);

    CHECK(ValidateRoute(HouseRoute()) == RouteValidity::Valid);

    Route gap = HouseRoute();
    gap.segments[1].from.xy.x += 0.001;
    CHECK(ValidateRoute(gap) == RouteValidity::Discontinuous); // exact continuity is required

    Route crossing = HouseRoute();
    crossing.segments[2].kind = SegmentKind::Travel; // walking "through" a layer change
    CHECK(ValidateRoute(crossing) == RouteValidity::LayerMismatch);

    Route fakeTransition = HouseRoute();
    fakeTransition.segments[0].kind = SegmentKind::LayerTransition; // a transition that stays on one layer
    CHECK(ValidateRoute(fakeTransition) == RouteValidity::LayerMismatch);

    Route nan = HouseRoute();
    nan.segments[3].to.xy.y = std::nan("");
    CHECK(ValidateRoute(nan) == RouteValidity::InvalidCoordinate);

    Route nullLayer = HouseRoute();
    nullLayer.segments[0].from.layer = MapLayerId();
    CHECK(ValidateRoute(nullLayer) == RouteValidity::InvalidCoordinate);

    Route short_ = HouseRoute();
    short_.destination = Dest(At(kFloor0, 500, 0));
    CHECK(ValidateRoute(short_) == RouteValidity::DestinationNotReached);
    short_.destination.arrivalRadiusMetres = 490.0; // within the arrival radius it counts
    CHECK(ValidateRoute(short_) == RouteValidity::Valid);

    Route otherLayerEnd = HouseRoute();
    otherLayerEnd.destination = Dest(At(kSurface, 10, 0), 1e9); // right numbers, wrong layer: never "arrived"
    CHECK(ValidateRoute(otherLayerEnd) == RouteValidity::DestinationNotReached);

    Route partial = HouseRoute();
    partial.complete = false;
    partial.destination = Dest(At(kFloor0, 500, 0));
    CHECK(ValidateRoute(partial) == RouteValidity::Incomplete);

    Route badRadius = HouseRoute();
    badRadius.destination.arrivalRadiusMetres = -1.0;
    CHECK(ValidateRoute(badRadius) == RouteValidity::InvalidCoordinate);
}

TEST_CASE("Route validation from an origin") {
    const Route r = HouseRoute();
    CHECK(ValidateRouteFrom(r, At(kSurface, 0, 0)) == RouteValidity::Valid);
    CHECK(ValidateRouteFrom(r, At(kSurface, 1, 0)) == RouteValidity::StartMismatch);
    CHECK(ValidateRouteFrom(Route{}, At(kSurface, 0, 0)) == RouteValidity::Empty);
    Route partial = HouseRoute();
    partial.complete = false;
    CHECK(ValidateRouteFrom(partial, At(kSurface, 9, 9)) == RouteValidity::StartMismatch);
}

TEST_CASE("Route lengths: planar per segment, zero for transitions, split by travel mode") {
    const Route r = HouseRoute();
    CHECK(SegmentLength(r.segments[0]) == doctest::Approx(50.0));
    CHECK(SegmentLength(r.segments[1]) == doctest::Approx(10.0));
    CHECK(SegmentLength(r.segments[2]) == 0.0);
    CHECK(SegmentLength(r.segments[3]) == doctest::Approx(10.0));
    CHECK(RouteLength(r) == doctest::Approx(70.0));
    CHECK(RouteLengthInMode(r, TravelMode::Driving) == doctest::Approx(50.0));
    CHECK(RouteLengthInMode(r, TravelMode::OnFoot) == doctest::Approx(20.0));
    CHECK(RouteLength(Route{}) == 0.0);
    // Cross-layer positions have no planar distance: it must not be invented.
    CHECK_FALSE(PlanarDistance(At(kSurface, 0, 0), At(kFloor0, 0, 0)).has_value());
}

TEST_CASE("Projection onto a route gives progress and remaining distance") {
    const Route r = HouseRoute();
    const auto start = ProjectOntoRoute(r, At(kSurface, 0, 0));
    REQUIRE(start.has_value());
    CHECK(start->segmentIndex == 0);
    CHECK(start->alongSegmentMetres == doctest::Approx(0.0));
    CHECK(start->offRouteMetres == doctest::Approx(0.0));
    CHECK(start->remainingMetres == doctest::Approx(70.0));

    const auto mid = ProjectOntoRoute(r, At(kSurface, 15, 20)); // exactly halfway along segment 0
    REQUIRE(mid.has_value());
    CHECK(mid->segmentIndex == 0);
    CHECK(mid->alongSegmentMetres == doctest::Approx(25.0));
    CHECK(mid->remainingMetres == doctest::Approx(45.0));

    const auto off = ProjectOntoRoute(r, At(kSurface, 30 + 7, 45)); // 7 m beside segment 1
    REQUIRE(off.has_value());
    CHECK(off->segmentIndex == 1);
    CHECK(off->offRouteMetres == doctest::Approx(7.0));
    CHECK(off->remainingMetres == doctest::Approx(15.0)); // 5 m left on segment 1 + 10 m inside

    const auto inside = ProjectOntoRoute(r, At(kFloor0, 4, 3));
    REQUIRE(inside.has_value());
    CHECK(inside->segmentIndex == 3);
    CHECK(inside->offRouteMetres == doctest::Approx(3.0));
    CHECK(inside->remainingMetres == doctest::Approx(6.0));

    const auto past = ProjectOntoRoute(r, At(kFloor0, 50, 0)); // beyond the end clamps to the last point
    REQUIRE(past.has_value());
    CHECK(past->remainingMetres == doctest::Approx(0.0));
    CHECK(past->offRouteMetres == doctest::Approx(40.0));

    CHECK_FALSE(ProjectOntoRoute(r, At(kUnderground, 0, 0)).has_value()); // no segment on that layer
    CHECK_FALSE(ProjectOntoRoute(Route{}, At(kSurface, 0, 0)).has_value());
    CHECK_FALSE(ProjectOntoRoute(r, At(kSurface, std::nan(""), 0)).has_value());
}

TEST_CASE("Projection with a degenerate segment and equal candidates is deterministic") {
    Route r;
    r.destination = Dest(At(kSurface, 10, 0));
    r.segments = {Travel(At(kSurface, 0, 0), At(kSurface, 0, 0)), Travel(At(kSurface, 0, 0), At(kSurface, 10, 0))};
    const auto p = ProjectOntoRoute(r, At(kSurface, 0, 0));
    REQUIRE(p.has_value());
    CHECK(p->segmentIndex == 0); // tie: the earlier segment wins
    CHECK(p->remainingMetres == doctest::Approx(10.0));
    CHECK(std::isfinite(p->alongSegmentMetres));
}

TEST_CASE("HasArrived uses the layer and the arrival radius") {
    const Destination d = Dest(At(kSurface, 100, 100), 5.0);
    CHECK(HasArrived(d, At(kSurface, 100, 105)));      // exactly on the radius
    CHECK_FALSE(HasArrived(d, At(kSurface, 100, 105.01)));
    CHECK_FALSE(HasArrived(d, At(kFloor0, 100, 100))); // same numbers on another layer
    CHECK(HasArrived(Dest(At(kSurface, 1, 1)), At(kSurface, 1, 1)));
    CHECK_FALSE(HasArrived(Dest(At(kSurface, 1, 1)), At(kSurface, 1, 1.0001)));
}

TEST_CASE("Navigation enums have stable names") {
    CHECK(ToString(TravelMode::OnFoot) == "OnFoot");
    CHECK(ToString(TravelMode::Driving) == "Driving");
    CHECK(ToString(RouteValidity::Discontinuous) == "Discontinuous");
    CHECK(ToString(NavigationErrorCode::LayerMismatch) == "LayerMismatch");
}
