#include <algorithm>
#include <cmath>
#include <doctest.h>
#include <spn/core/GeometryMath.h>
#include <spn/core/MapGeometry.h>
#include <spn/core/Navigation.h>
#include <spn/core/SyntheticMapRegion.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::core::synthetic;
using spn::testdata::Cid;

namespace {
using doctest::Approx;
}

TEST_CASE("GeometryMath: projection onto segments and polylines") {
    const auto p = ProjectPointToSegment({5, 5}, {0, 0}, {10, 0});
    CHECK(p.point == Vec2{5, 0});
    CHECK(p.t == Approx(0.5));
    CHECK(p.distance == Approx(5.0));
    CHECK(ProjectPointToSegment({-3, 4}, {0, 0}, {10, 0}).point == Vec2{0, 0}); // clamps to the start
    CHECK(ProjectPointToSegment({20, 0}, {0, 0}, {10, 0}).t == 1.0);           // and to the end
    CHECK(ProjectPointToSegment({3, 4}, {1, 1}, {1, 1}).distance == Approx(std::hypot(2.0, 3.0))); // degenerate segment

    const std::vector<Vec2> line{{0, 0}, {10, 0}, {10, 10}};
    const auto cum = CumulativeLengths(line);
    CHECK(cum == std::vector<double>{0.0, 10.0, 20.0});
    const auto proj = ProjectPointToPolyline({12, 4}, line, cum);
    CHECK(proj.point == Vec2{10, 4});
    CHECK(proj.offsetMetres == Approx(14.0));
    CHECK(proj.edge == 1);
    CHECK(PointAtOffset(line, cum, 15.0) == Vec2{10, 5});
    CHECK(PointAtOffset(line, cum, -1.0) == Vec2{0, 0});
    CHECK(PointAtOffset(line, cum, 99.0) == Vec2{10, 10});
}

TEST_CASE("GeometryMath: slicing a polyline keeps the exact end points and orientation") {
    const std::vector<Vec2> line{{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    const auto cum = CumulativeLengths(line);
    const auto fwd = SlicePolyline(line, cum, 5.0, {5, 0}, 25.0, {5, 10});
    CHECK(fwd == std::vector<Vec2>{{5, 0}, {10, 0}, {10, 10}, {5, 10}});
    const auto back = SlicePolyline(line, cum, 25.0, {5, 10}, 5.0, {5, 0});
    CHECK(back == std::vector<Vec2>{{5, 10}, {10, 10}, {10, 0}, {5, 0}});
    CHECK(PolylineLength(fwd) == Approx(20.0));
}

TEST_CASE("GeometryMath: segment intersection includes touching and rejects near misses") {
    CHECK(SegmentsIntersect({0, 0}, {10, 10}, {0, 10}, {10, 0}));
    CHECK(SegmentsIntersect({0, 0}, {10, 0}, {10, 0}, {10, 5}));    // shared end point
    CHECK(SegmentsIntersect({0, 0}, {10, 0}, {5, 0}, {15, 0}));     // collinear overlap
    CHECK_FALSE(SegmentsIntersect({0, 0}, {10, 0}, {11, 0}, {15, 0})); // collinear, apart
    CHECK_FALSE(SegmentsIntersect({0, 0}, {10, 0}, {5, 1}, {5, 9}));
    double t = -1.0;
    CHECK(SegmentIntersectionParameter({0, 0}, {10, 0}, {4, -5}, {4, 5}, t));
    CHECK(t == Approx(0.4));
    CHECK_FALSE(SegmentIntersectionParameter({0, 0}, {10, 0}, {0, 1}, {10, 1}, t)); // parallel
}

TEST_CASE("GeometryMath: polygons - containment, area, simplicity") {
    const std::vector<Vec2> square{{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    CHECK(PolygonContains(square, {5, 5}));
    CHECK(PolygonContains(square, {10, 5})); // boundary counts as inside
    CHECK_FALSE(PolygonContains(square, {10.01, 5}));
    CHECK(PolygonArea(square) == Approx(100.0));
    CHECK(IsSimpleRing(square));
    CHECK_FALSE(IsSimpleRing({{0, 0}, {10, 10}, {10, 0}, {0, 10}})); // bow tie
    CHECK_FALSE(IsSimpleRing({{0, 0}, {5, 0}, {10, 0}}));            // zero area
    CHECK_FALSE(IsSimpleRing({{0, 0}, {1, 1}}));
    const std::vector<Vec2> concave{{0, 0}, {10, 0}, {10, 10}, {5, 2}, {0, 10}};
    CHECK(PolygonContains(concave, {2, 2}));
    CHECK_FALSE(PolygonContains(concave, {5, 8})); // inside the notch
    CHECK(SegmentCrossesRingBoundary({-5, 5}, {5, 5}, square));
    CHECK_FALSE(SegmentCrossesRingBoundary({2, 2}, {8, 8}, square)); // completely inside
}

TEST_CASE("GeometryMath: Douglas-Peucker keeps shape within the tolerance") {
    std::vector<Vec2> wiggle;
    for (int i = 0; i <= 100; ++i) {
        wiggle.push_back({static_cast<double>(i), (i % 2 == 0) ? 0.0 : 0.05});
    }
    const auto s = SimplifyPolyline(wiggle, 0.1);
    CHECK(s.size() == 2);
    CHECK(s.front() == wiggle.front());
    CHECK(s.back() == wiggle.back());
    const std::vector<Vec2> corner{{0, 0}, {10, 0}, {10, 10}};
    CHECK(SimplifyPolyline(corner, 0.5) == corner); // the corner is far from the chord
    CHECK(SimplifyPolyline(corner, 0.0) == corner);
    CHECK(SimplifyPolyline(corner, std::nan("")) == corner);
    // a long smooth input is handled iteratively (no stack growth) and keeps the end points
    std::vector<Vec2> arc;
    for (int i = 0; i <= 200000; ++i) {
        const double a = 3.0 * static_cast<double>(i) / 200000.0;
        arc.push_back({1000.0 * std::cos(a), 1000.0 * std::sin(a)});
    }
    const auto simplified = SimplifyPolyline(arc, 0.5);
    CHECK(simplified.size() < arc.size() / 100);
    CHECK(simplified.front() == arc.front());
    CHECK(simplified.back() == arc.back());
    // every dropped point lies within the tolerance of the simplified line
    for (std::size_t i = 0; i < arc.size(); i += 997) {
        double best = 1e18;
        for (std::size_t k = 0; k + 1 < simplified.size(); ++k) { best = std::min(best, ProjectPointToSegment(arc[i], simplified[k], simplified[k + 1]).distance); }
        CHECK(best <= 0.5 + 1e-9);
    }
}

TEST_CASE("GeometryMath: clipping segments to a rectangle and a circle") {
    Vec2 a, b;
    const MapRect rect{{-10, -10}, {10, 10}};
    CHECK(ClipSegmentToRect({-20, 0}, {20, 0}, rect, a, b));
    CHECK(a == Vec2{-10, 0});
    CHECK(b == Vec2{10, 0});
    CHECK(ClipSegmentToRect({0, 0}, {5, 5}, rect, a, b)); // fully inside: unchanged
    CHECK(b == Vec2{5, 5});
    CHECK_FALSE(ClipSegmentToRect({-20, 20}, {20, 20}, rect, a, b));
    CHECK_FALSE(ClipSegmentToRect({-20, 11}, {20, 11}, rect, a, b)); // parallel and outside
    CHECK(ClipSegmentToCircle({-20, 0}, {20, 0}, {0, 0}, 10.0, a, b));
    CHECK(a.x == Approx(-10.0));
    CHECK(b.x == Approx(10.0));
    CHECK_FALSE(ClipSegmentToCircle({-20, 11}, {20, 11}, {0, 0}, 10.0, a, b));
    CHECK(ClipSegmentToCircle({1, 1}, {2, 2}, {0, 0}, 10.0, a, b));
    CHECK(a == Vec2{1, 1});
}

TEST_CASE("BoxGrid: queries return sorted unique keys and filter by the exact box") {
    BoxGrid grid(100.0);
    grid.Insert(7, MapRect{{0, 0}, {250, 10}}); // spans three cells
    grid.Insert(3, MapRect{{300, 300}, {310, 310}});
    grid.Insert(9, MapRect{{-1e6, -1e6}, {1e6, 1e6}}); // huge: kept in the always-checked list
    std::vector<std::uint32_t> out;
    grid.Query(MapRect{{200, 0}, {260, 20}}, out);
    CHECK(out == std::vector<std::uint32_t>{7, 9});
    out.clear();
    grid.Query(MapRect{{290, 290}, {320, 320}}, out);
    CHECK(out == std::vector<std::uint32_t>{3, 9});
    out.clear();
    grid.Query(MapRect{{50, 50}, {20, 20}}, out); // inverted
    CHECK(out.empty());
    out.clear();
    grid.Query(MapRect{{0, 0}, {std::nan(""), 1}}, out);
    CHECK(out.empty());
}

TEST_CASE("Synthetic region: data validates and the indexes know every feature") {
    const SyntheticMapRegion r = MakeSyntheticMapRegion();
    REQUIRE(r.geometry != nullptr);
    CHECK(r.geometry->Roads().Segments().size() == 38);
    CHECK(r.geometry->Roads().Nodes().size() == 32);
    CHECK(r.geometry->Buildings().size() == 13);
    CHECK(r.geometry->Areas().size() == 13);
    CHECK(r.geometry->Lines().size() == 3);
    const BuildingDef* shop = r.geometry->BuildingAt(r.shopInside);
    REQUIRE(shop != nullptr);
    CHECK(shop->id == BuildingId(kShopWest));
    CHECK(r.geometry->BuildingAt(r.westStreet) == nullptr);
    CHECK(r.geometry->FindBuilding(BuildingId(kHouseEast)) != nullptr);
    CHECK(r.geometry->FindBuilding(BuildingId(999)) == nullptr);
    CHECK(r.geometry->FindArea(MapAreaId(9))->elevationMetres == 850.0);
    // layer filtering: a rect around the river finds the river but not the sea
    const auto areas = r.geometry->AreasInRect(r.layer, MapRect{{960, 0}, {1040, 50}});
    CHECK(std::find(areas.begin(), areas.end(), 0u) != areas.end());
    CHECK(r.geometry->AreasInRect(MapLayerId(77), MapRect{{960, 0}, {1040, 50}}).empty());
}

TEST_CASE("Synthetic region: lengths follow the polyline, not the straight distance of the end nodes") {
    const SyntheticMapRegion r = MakeSyntheticMapRegion();
    const RoadNetwork& net = r.geometry->Roads();
    const auto idx = net.SegmentIndex(RoadSegmentId(kOnRampWest));
    REQUIRE(idx.has_value());
    CHECK(net.Info(*idx).length == Approx(2.0 * std::hypot(30.0, 400.0)));
    CHECK(net.Info(*idx).length > 800.0); // end nodes are exactly 800 m apart
    const auto bridge = net.SegmentIndex(RoadSegmentId(kNorthBridge));
    CHECK(net.Info(*bridge).length == Approx(200.0));
    CHECK(net.Info(*bridge).structure == RoadStructure::Bridge);
}

TEST_CASE("Road access: class defaults and one-way rules per travel mode") {
    const SyntheticMapRegion r = MakeSyntheticMapRegion();
    const RoadNetwork& net = r.geometry->Roads();
    const auto seg = [&](std::uint64_t id) { return *net.SegmentIndex(RoadSegmentId(id)); };
    CHECK(net.Allows(seg(kFootBridge), TravelMode::OnFoot, true));
    CHECK_FALSE(net.Allows(seg(kFootBridge), TravelMode::Driving, true));
    CHECK_FALSE(net.Allows(seg(kMotorTunnel), TravelMode::OnFoot, true));
    CHECK(net.Allows(seg(kMotorTunnel), TravelMode::Driving, false));
    CHECK(net.Allows(seg(kOneWayEastFG), TravelMode::Driving, true));
    CHECK_FALSE(net.Allows(seg(kOneWayEastFG), TravelMode::Driving, false));
    CHECK(net.Allows(seg(kOneWayEastFG), TravelMode::OnFoot, false)); // pedestrians ignore one-way
    CHECK(DefaultAccess(RoadClass::Motorway) == kAccessVehicle);
    CHECK(DefaultAccess(RoadClass::Footway) == kAccessFoot);
    CHECK(DefaultAccess(RoadClass::ForestTrack) == (kAccessFoot | kAccessVehicle));
}

TEST_CASE("Road snapping: mode, tunnels, closures and ordering") {
    const SyntheticMapRegion r = MakeSyntheticMapRegion();
    const RoadNetwork& net = r.geometry->Roads();
    const MapPosition nearFoot{r.layer, Vec2{1000, -296}}; // 4 m north of the foot bridge deck
    auto foot = net.Snap(nearFoot, 50.0, SnapRules{TravelMode::OnFoot}, 3);
    REQUIRE(!foot.empty());
    CHECK(foot.front().segment == RoadSegmentId(kFootBridge));
    CHECK(foot.front().distanceMetres == Approx(4.0));
    CHECK(foot.front().point == Vec2{1000, -300});
    CHECK(foot.front().offsetMetres == Approx(100.0));
    CHECK(net.Snap(nearFoot, 50.0, SnapRules{TravelMode::Driving}, 3).empty()); // vehicles may not use a foot bridge

    const MapPosition aboveTunnel{r.layer, Vec2{1000, -790}};
    CHECK(net.Snap(aboveTunnel, 50.0, SnapRules{TravelMode::Driving}, 3).empty()); // a tunnel cannot be entered from outside
    const auto inside = net.Snap(aboveTunnel, 50.0, SnapRules{TravelMode::Driving, true}, 3);
    REQUIRE(inside.size() == 1);
    CHECK(inside.front().segment == RoadSegmentId(kMotorTunnel));

    const std::set<RoadSegmentId> closed{RoadSegmentId(kFootBridge)};
    CHECK(net.Snap(nearFoot, 50.0, SnapRules{TravelMode::OnFoot, false, &closed}, 3).empty());
    CHECK(net.Snap(MapPosition{MapLayerId(9), Vec2{1000, -296}}, 50.0, SnapRules{TravelMode::OnFoot}, 3).empty()); // other layer
    CHECK(net.Snap(nearFoot, -1.0, SnapRules{TravelMode::OnFoot}, 3).empty());
    CHECK(net.Snap(nearFoot, 50.0, SnapRules{TravelMode::OnFoot}, 0).empty());
    CHECK(net.Snap(MapPosition{r.layer, Vec2{std::nan(""), 0}}, 50.0, SnapRules{TravelMode::OnFoot}, 3).empty());

    // junction: equal distance to two segments -> lower id first
    const auto junction = net.Snap(MapPosition{r.layer, Vec2{900, 0}}, 10.0, SnapRules{TravelMode::OnFoot}, 8);
    REQUIRE(junction.size() >= 3);
    for (std::size_t i = 1; i < junction.size(); ++i) {
        CHECK(junction[i - 1].distanceMetres <= junction[i].distanceMetres);
        if (junction[i - 1].distanceMetres == junction[i].distanceMetres) {
            CHECK(junction[i - 1].segment < junction[i].segment);
        }
    }
}

TEST_CASE("Connectors: wet length ignores islands and tolerates a step from a bridge deck") {
    const SyntheticMapRegion r = MakeSyntheticMapRegion();
    const MapGeometry& g = *r.geometry;
    CHECK(g.WetLengthMetres(r.layer, {900, 700}, {1100, 700}) == Approx(60.0)); // river width
    CHECK(g.WetLengthMetres(r.layer, {900, 130}, {1100, 130}) == Approx(60.0 - 30.0)); // minus the island
    CHECK(g.WetLengthMetres(r.layer, {900, 700}, {960, 700}) == 0.0);
    CHECK(g.ConnectorBlocked(r.layer, {900, 700}, {1100, 700}));
    CHECK_FALSE(g.ConnectorBlocked(r.layer, {1000, 1196}, {1000, 1200})); // 4 m step on a bridge deck
    CHECK(g.ConnectorBlocked(r.layer, {1000, 1150}, {1000, 1200}));        // 50 m through the river
    CHECK(g.ConnectorBlocked(r.layer, {-100, -1100}, {-100, -1300}));      // crosses the coastline / sea
    CHECK_FALSE(g.ConnectorBlocked(r.layer, {100, 100}, {100, 400}));      // open land
}

namespace {
MapGeometryData TinyData() {
    MapGeometryData d;
    const MapLayerId layer(kLayerId);
    d.nodes = {RoadNodeDef{RoadNodeId(1), MapPosition{layer, Vec2{0, 0}}}, RoadNodeDef{RoadNodeId(2), MapPosition{layer, Vec2{100, 0}}}};
    RoadSegmentDef s;
    s.id = RoadSegmentId(1);
    s.from = RoadNodeId(1);
    s.to = RoadNodeId(2);
    d.segments = {s};
    return d;
}
GeometryErrorCode CreateCode(MapGeometryData d) {
    const auto def = MakeSyntheticMapDefinition();
    const auto g = MapGeometry::Create(*def, std::move(d));
    return g.HasValue() ? static_cast<GeometryErrorCode>(255) : g.Error().code;
}
} // namespace

TEST_CASE("MapGeometry validation: roads") {
    const auto def = MakeSyntheticMapDefinition();
    CHECK(MapGeometry::Create(*def, TinyData()).HasValue());
    { auto d = TinyData(); d.nodes[1].id = RoadNodeId(1); CHECK(CreateCode(d) == GeometryErrorCode::DuplicateId); }
    { auto d = TinyData(); d.nodes[1].id = RoadNodeId(0); CHECK(CreateCode(d) == GeometryErrorCode::InvalidId); }
    { auto d = TinyData(); d.segments[0].to = RoadNodeId(9); CHECK(CreateCode(d) == GeometryErrorCode::UnknownNode); }
    { auto d = TinyData(); d.segments.push_back(d.segments[0]); CHECK(CreateCode(d) == GeometryErrorCode::DuplicateId); }
    { auto d = TinyData(); d.segments[0].to = d.segments[0].from; CHECK(CreateCode(d) == GeometryErrorCode::DegenerateGeometry); } // zero-length loop
    { auto d = TinyData(); d.segments[0].to = d.segments[0].from; d.segments[0].shape = {{50, 50}}; CHECK(CreateCode(d) == static_cast<GeometryErrorCode>(255)); } // a real loop is fine
    { auto d = TinyData(); d.nodes[1].position.xy = {std::nan(""), 0}; CHECK(CreateCode(d) == GeometryErrorCode::InvalidCoordinate); }
    { auto d = TinyData(); d.nodes[1].position.xy = {1e9, 0}; CHECK(CreateCode(d) == GeometryErrorCode::InvalidCoordinate); }
    { auto d = TinyData(); d.nodes[1].position.xy = {9000, 0}; CHECK(CreateCode(d) == GeometryErrorCode::OutsideLayerBounds); }
    { auto d = TinyData(); d.nodes[1].position.layer = MapLayerId(7); CHECK(CreateCode(d) == GeometryErrorCode::UnknownLayer); }
    { auto d = TinyData(); d.segments[0].shape = {{0, 9000}}; CHECK(CreateCode(d) == GeometryErrorCode::OutsideLayerBounds); }
    { auto d = TinyData(); d.segments[0].access = 0; CHECK(CreateCode(d) == GeometryErrorCode::InvalidAccess); }
    { auto d = TinyData(); d.segments[0].access = 8; CHECK(CreateCode(d) == GeometryErrorCode::InvalidAccess); }
    { auto d = TinyData(); d.segments[0].region = Cid("synthetic:region.nowhere"); CHECK(CreateCode(d) == GeometryErrorCode::UnknownRegion); }
    { auto d = TinyData(); d.segments[0].name = "bad\nname"; CHECK(CreateCode(d) == GeometryErrorCode::InvalidLabel); }
    { auto d = TinyData(); d.restrictions = {TurnRestriction{RoadNodeId(1), RoadSegmentId(1), RoadSegmentId(5), true}}; CHECK(CreateCode(d) == GeometryErrorCode::UnknownSegment); }
    { auto d = TinyData(); d.restrictions = {TurnRestriction{RoadNodeId(7), RoadSegmentId(1), RoadSegmentId(1), true}}; CHECK(CreateCode(d) == GeometryErrorCode::UnknownNode); }
    CHECK(MapGeometry::Create(*def, TinyData(), 0.0).Error().code == GeometryErrorCode::InvalidGridCell);
    CHECK(MapGeometry::Create(*def, TinyData(), std::nan("")).Error().code == GeometryErrorCode::InvalidGridCell);
}

TEST_CASE("MapGeometry validation: restrictions must touch their node, segments must stay on one layer") {
    MapGeometryData d = MakeSyntheticGeometryData();
    // segment 101 (W_A - W_B) does not touch E_D
    d.restrictions.push_back(TurnRestriction{RoadNodeId(kEastD), RoadSegmentId(kStreetWestAB), RoadSegmentId(kStreetEastDI), true});
    CHECK(CreateCode(d) == GeometryErrorCode::RestrictionNotAdjacent);

    const auto def = MakeSyntheticMapDefinition();
    MapLayerDef other;
    other.id = MapLayerId(2);
    other.key = Cid("synthetic:layer.other");
    other.bounds = MapRect{{-100, -100}, {100, 100}};
    auto two = WorldMapDefinition::Create({def->Layers().front(), other}, {}, {});
    REQUIRE(two.HasValue());
    MapGeometryData t = TinyData();
    t.nodes[1].position.layer = MapLayerId(2);
    t.nodes[1].position.xy = {50, 50};
    CHECK(MapGeometry::Create(two.Value(), t).Error().code == GeometryErrorCode::LayerMismatch);
}

TEST_CASE("MapGeometry validation: buildings, areas and lines") {
    { auto d = MakeSyntheticGeometryData(); d.buildings[0].footprint = {{0, 0}, {10, 10}, {10, 0}, {0, 10}}; CHECK(CreateCode(d) == GeometryErrorCode::InvalidPolygon); }
    { auto d = MakeSyntheticGeometryData(); d.buildings[0].entrances = {{320, 300}}; CHECK(CreateCode(d) == GeometryErrorCode::InvalidEntrance); }
    { auto d = MakeSyntheticGeometryData(); d.buildings[0].entrances = {{320, 30.5}}; CHECK(CreateCode(d) == static_cast<GeometryErrorCode>(255)); } // within 1 m
    { auto d = MakeSyntheticGeometryData(); d.buildings.push_back(d.buildings[0]); CHECK(CreateCode(d) == GeometryErrorCode::DuplicateId); }
    { auto d = MakeSyntheticGeometryData(); d.buildings[0].heightMetres = -3.0; CHECK(CreateCode(d) == GeometryErrorCode::InvalidCoordinate); }
    { auto d = MakeSyntheticGeometryData(); d.buildings[0].footprint[0] = {-99999, 0}; CHECK(CreateCode(d) == GeometryErrorCode::OutsideLayerBounds); }
    { auto d = MakeSyntheticGeometryData(); d.areas[0].outline = {{0, 0}, {1, 1}}; CHECK(CreateCode(d) == GeometryErrorCode::InvalidPolygon); }
    { auto d = MakeSyntheticGeometryData(); d.areas[0].region = Cid("synthetic:region.nowhere"); CHECK(CreateCode(d) == GeometryErrorCode::UnknownRegion); }
    { auto d = MakeSyntheticGeometryData(); d.areas[0].elevationMetres = std::nan(""); CHECK(CreateCode(d) == GeometryErrorCode::InvalidCoordinate); }
    { auto d = MakeSyntheticGeometryData(); d.lines[0].points = {{5, 5}}; CHECK(CreateCode(d) == GeometryErrorCode::DegenerateGeometry); }
    { auto d = MakeSyntheticGeometryData(); d.lines[0].points = {{5, 5}, {5, 5}}; CHECK(CreateCode(d) == GeometryErrorCode::DegenerateGeometry); }
    { auto d = MakeSyntheticGeometryData(); d.lines[0].label = std::string(200, 'x'); CHECK(CreateCode(d) == GeometryErrorCode::InvalidLabel); }
    { auto d = MakeSyntheticGeometryData(); d.lines[0].layer = MapLayerId(5); CHECK(CreateCode(d) == GeometryErrorCode::UnknownLayer); }
}

TEST_CASE("MapGeometry: the input order never changes the stored order") {
    const auto def = MakeSyntheticMapDefinition();
    MapGeometryData a = MakeSyntheticGeometryData();
    MapGeometryData b = a;
    std::reverse(b.nodes.begin(), b.nodes.end());
    std::reverse(b.segments.begin(), b.segments.end());
    std::reverse(b.buildings.begin(), b.buildings.end());
    std::reverse(b.areas.begin(), b.areas.end());
    std::reverse(b.lines.begin(), b.lines.end());
    const auto ga = MapGeometry::Create(*def, a);
    const auto gb = MapGeometry::Create(*def, b);
    REQUIRE(ga.HasValue());
    REQUIRE(gb.HasValue());
    CHECK(ga.Value().ToData() == gb.Value().ToData());
}

TEST_CASE("MergeGeometryData: identical shared features merge, conflicting ones are rejected") {
    MapGeometryData a = MakeSyntheticGeometryData();
    MapGeometryData b;
    b.nodes = {a.nodes[0], a.nodes[1]}; // shared border nodes
    auto merged = MergeGeometryData(a, b);
    REQUIRE(merged.HasValue());
    CHECK(merged.Value().nodes.size() == a.nodes.size());
    b.nodes[0].position.xy.x += 1.0; // same id, different position
    const auto conflict = MergeGeometryData(a, b);
    REQUIRE_FALSE(conflict.HasValue());
    CHECK(conflict.Error().code == GeometryErrorCode::MergeConflict);
    CHECK(conflict.Error().id == a.nodes[0].id.Value());
}
