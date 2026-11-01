#include <algorithm>
#include <cmath>
#include <doctest.h>
#include <numbers>
#include <spn/core/MapRender.h>
#include <spn/core/NavigationSession.h>
#include <spn/core/SyntheticMapRegion.h>

using namespace spn::core;
using namespace spn::core::synthetic;
using doctest::Approx;

namespace {

MinimapConfig Viewport(double halfPx = 200.0) {
    MinimapConfig c;
    c.shape = MinimapShape::Rectangle;
    c.halfWidthPx = halfPx;
    c.halfHeightPx = halfPx;
    c.minZoom = 0.0005;
    c.maxZoom = 16.0;
    return c;
}

MinimapView View(const SyntheticMapRegion& r, double x, double y, double zoom = 1.0, MinimapRotation rot = MinimapRotation::NorthUp, double heading = 0.0) {
    MinimapView v;
    v.player = MapPosition{r.layer, Vec2{x, y}};
    v.zoom = zoom;
    v.rotation = rot;
    v.headingRadians = heading;
    return v;
}

MapRenderFrame MustBuild(const SyntheticMapRegion& r, const MinimapView& v, const MinimapConfig& vp = Viewport(), const MapRenderConfig& cfg = {}, const WorldMapState* map = nullptr,
                         const RouteOverlay& overlay = {}) {
    auto frame = BuildMapRenderFrame(*r.geometry, map, v, vp, cfg, overlay);
    REQUIRE(frame.HasValue());
    return std::move(frame).Value();
}

bool HasRoad(const MapRenderFrame& f, std::uint64_t id) {
    return std::any_of(f.roads.begin(), f.roads.end(), [&](const RenderPolyline& p) { return p.id == id; });
}

double Length(const std::vector<Vec2>& pts) {
    double sum = 0.0;
    for (std::size_t i = 1; i < pts.size(); ++i) { sum += std::hypot(pts[i].x - pts[i - 1].x, pts[i].y - pts[i - 1].y); }
    return sum;
}

} // namespace

TEST_CASE("MapRender: a frame near the west street shows its roads, buildings and areas, not the far hamlet") {
    const auto r = MakeSyntheticMapRegion();
    const MapRenderFrame f = MustBuild(r, View(r, 450, 0));
    CHECK(HasRoad(f, kStreetWestAB));
    CHECK(HasRoad(f, kStreetWestBC));
    CHECK_FALSE(HasRoad(f, kHamletRoadAB)); // 2 km away
    CHECK_FALSE(HasRoad(f, kNorthBridge));
    CHECK(f.buildings.size() >= 2);          // shop, warehouse (row houses are ~400 m west of the view)
    CHECK(f.areas.size() >= 1);
    CHECK(f.stats.candidates < r.geometry->Roads().Segments().size() + r.geometry->Buildings().size() + r.geometry->Areas().size() + r.geometry->Lines().size());
    CHECK(f.stats.emitted == f.roads.size() + f.buildings.size() + f.areas.size() + f.lines.size());
    CHECK(f.metresPerPixel == Approx(1.0));
    for (const RenderPolyline& p : f.roads) {
        for (const Vec2 v : p.points) { CHECK(std::isfinite(v.x)); CHECK(std::isfinite(v.y)); }
    }
}

TEST_CASE("MapRender: roads are drawn least important first, bridges above surface roads above tunnels") {
    const auto r = MakeSyntheticMapRegion();
    MapRenderConfig cfg;
    const MapRenderFrame f = MustBuild(r, View(r, 1000, -300, 0.2), Viewport(300.0), cfg); // zoomed out so the foot bridge, roads and the tunnel are in view
    REQUIRE(f.roads.size() > 5);
    int lastStructure = 0;
    for (const RenderPolyline& p : f.roads) {
        const int s = p.structure == RoadStructure::Surface ? 1 : (p.structure == RoadStructure::Bridge ? 2 : 0);
        CHECK(s >= lastStructure);
        lastStructure = s;
    }
    // within surface roads the class order is Footway (last enum value) ... Motorway
    int lastClass = 100;
    for (const RenderPolyline& p : f.roads) {
        if (p.structure != RoadStructure::Surface) { continue; }
        CHECK(static_cast<int>(p.roadClass) <= lastClass);
        lastClass = static_cast<int>(p.roadClass);
    }
}

TEST_CASE("MapRender: zooming out drops small roads and buildings first") {
    const auto r = MakeSyntheticMapRegion();
    const MapRenderFrame close = MustBuild(r, View(r, 1000, -200, 1.0, MinimapRotation::NorthUp), Viewport(400.0));
    CHECK(HasRoad(close, kFootBridge));
    CHECK(close.buildings.size() > 0);
    // 12 m/pixel: buildings and footways are gone, primary roads and motorways remain
    const MapRenderFrame far = MustBuild(r, View(r, 1000, 300, 1.0 / 12.0), Viewport(400.0));
    CHECK(far.metresPerPixel == Approx(12.0));
    CHECK(far.buildings.empty());
    CHECK_FALSE(HasRoad(far, kFootBridge));
    CHECK(HasRoad(far, kNorthBridge));
    CHECK(far.stats.culledLod > 0);
    // the river is still drawn as an area and a line at this zoom
    CHECK(std::any_of(far.areas.begin(), far.areas.end(), [](const RenderPolygon& a) { return a.areaKind == AreaKind::Water; }));
    CHECK(std::any_of(far.lines.begin(), far.lines.end(), [](const RenderPolyline& l) { return l.lineKind == LineKind::River; }));
}

TEST_CASE("MapRender: rotation and zoom map the world exactly like the minimap transform") {
    const auto r = MakeSyntheticMapRegion();
    // NorthUp: a point east of the player is to the right, north is up (screen y grows downwards)
    const MapRenderFrame north = MustBuild(r, View(r, 450, 0));
    const auto edge = std::find_if(north.roads.begin(), north.roads.end(), [](const RenderPolyline& p) { return p.id == kStreetWestBC; });
    REQUIRE(edge != north.roads.end());
    CHECK(edge->points.front().x == Approx(0.0)); // the road starts at the player's node (450, 0)
    CHECK(edge->points.back().x == Approx(450.0)); // roads are not clipped to the viewport: 450 m east = 450 px at 1 m/px
    // HeadingUp facing east: east is screen-up
    const MapRenderFrame east = MustBuild(r, View(r, 450, 0, 1.0, MinimapRotation::HeadingUp, std::numbers::pi / 2.0));
    const auto e2 = std::find_if(east.roads.begin(), east.roads.end(), [](const RenderPolyline& p) { return p.id == kStreetWestBC; });
    REQUIRE(e2 != east.roads.end());
    CHECK(e2->points.back().x == Approx(0.0).epsilon(1e-9));
    CHECK(e2->points.back().y < 0.0); // up
    CHECK(east.playerHeadingOnScreenRadians == Approx(0.0).epsilon(1e-12));
    CHECK(east.northOnScreenRadians == Approx(3.0 * std::numbers::pi / 2.0)); // north points to the screen's left
    // zoom 2 halves metres per pixel and doubles the offsets
    const MapRenderFrame zoomed = MustBuild(r, View(r, 450, 0, 2.0));
    const auto e3 = std::find_if(zoomed.roads.begin(), zoomed.roads.end(), [](const RenderPolyline& p) { return p.id == kStreetWestBC; });
    REQUIRE(e3 != zoomed.roads.end());
    CHECK(zoomed.metresPerPixel == Approx(0.5));
    CHECK(e3->points.back().x == Approx(900.0));
}

TEST_CASE("MapRender: the frame is identical 5000 km from the origin") {
    const auto r = MakeSyntheticMapRegion();
    constexpr double kShiftX = 5'000'000.0;
    constexpr double kShiftY = -3'000'000.0;
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
    MapRenderConfig cfg;
    cfg.respectDiscovery = false;
    const MinimapConfig vp = Viewport();
    const MapRenderFrame a = MustBuild(r, View(r, 420, 10, 1.5, MinimapRotation::HeadingUp, 0.7), vp, cfg);
    const auto b = BuildMapRenderFrame(far.Value(), nullptr, View(r, 420 + kShiftX, 10 + kShiftY, 1.5, MinimapRotation::HeadingUp, 0.7), vp, cfg);
    REQUIRE(b.HasValue());
    REQUIRE(a.roads.size() == b.Value().roads.size());
    REQUIRE(a.buildings.size() == b.Value().buildings.size());
    for (std::size_t i = 0; i < a.roads.size(); ++i) {
        REQUIRE(a.roads[i].points.size() == b.Value().roads[i].points.size());
        for (std::size_t k = 0; k < a.roads[i].points.size(); ++k) {
            CHECK(b.Value().roads[i].points[k].x == Approx(a.roads[i].points[k].x).epsilon(1e-6));
            CHECK(b.Value().roads[i].points[k].y == Approx(a.roads[i].points[k].y).epsilon(1e-6));
        }
    }
}

TEST_CASE("MapRender: undiscovered regions hide their features until discovered") {
    const auto r = MakeSyntheticMapRegion();
    WorldMapState map(r.definition);
    MapRenderConfig cfg;
    const MinimapView v = View(r, 500, 200, 0.1);
    const MapRenderFrame hidden = MustBuild(r, v, Viewport(), cfg, &map);
    const MapRenderFrame all = MustBuild(r, v, Viewport(), [] { MapRenderConfig c; c.respectDiscovery = false; return c; }(), &map);
    const auto districts = [](const MapRenderFrame& f) { return std::count_if(f.areas.begin(), f.areas.end(), [](const RenderPolygon& a) { return a.areaKind == AreaKind::District; }); };
    CHECK(districts(all) >= 1);
    CHECK(districts(hidden) == 0);
    CHECK(hidden.stats.culledDiscovery >= 1);
    // discovering the west region reveals the west district (features without a region are always shown)
    REQUIRE(map.Discover(*ContentId::Parse("synthetic:region.west"), DiscoveryState::Known).HasValue());
    CHECK(districts(MustBuild(r, v, Viewport(), cfg, &map)) >= 1);
    // without a map state the filter cannot apply and nothing is hidden
    CHECK(districts(MustBuild(r, v, Viewport(), cfg, nullptr)) == districts(all));
}

TEST_CASE("MapRender: the per-kind budget keeps the most important roads and says it truncated") {
    const auto r = MakeSyntheticMapRegion();
    MapRenderConfig cfg;
    cfg.maxRoads = 4;
    const MapRenderFrame f = MustBuild(r, View(r, 1000, -300, 0.05), Viewport(300.0), cfg);
    CHECK(f.stats.truncated);
    REQUIRE(f.roads.size() <= 4);
    for (const RenderPolyline& p : f.roads) {
        CHECK(static_cast<int>(p.roadClass) <= static_cast<int>(RoadClass::Primary)); // motorways, links, expressways, primary roads survive
    }
    MapRenderConfig roomy;
    CHECK_FALSE(MustBuild(r, View(r, 1000, -300, 0.05), Viewport(300.0), roomy).stats.truncated);
}

TEST_CASE("MapRender: simplification reduces vertices in pixel space but never below an end point pair or a triangle") {
    const auto r = MakeSyntheticMapRegion();
    MapRenderConfig coarse;
    coarse.lod.simplifyTolerancePx = 50.0;
    const MapRenderFrame f = MustBuild(r, View(r, 100, -400, 0.5), Viewport(300.0), coarse);
    CHECK(f.stats.pointsAfter < f.stats.pointsBefore);
    for (const RenderPolyline& p : f.roads) { CHECK(p.points.size() >= 2); }
    for (const RenderPolygon& b : f.buildings) { CHECK(b.points.size() >= 3); }
    for (const RenderPolygon& a : f.areas) { CHECK(a.points.size() >= 3); }
    MapRenderConfig exact;
    exact.lod.simplifyTolerancePx = 0.0;
    const MapRenderFrame g = MustBuild(r, View(r, 100, -400, 0.5), Viewport(300.0), exact);
    CHECK(g.stats.pointsAfter == g.stats.pointsBefore);
}

TEST_CASE("MapRender: the route is clipped to the viewport and split at the player's progress") {
    const auto r = MakeSyntheticMapRegion();
    WorldMapState map(r.definition);
    NavigationConfig nc;
    nc.options.mode = TravelMode::Driving;
    NavigationSession nav(r.geometry, nc);
    REQUIRE(nav.SetWaypoint(map, r.westStreet, r.eastStreet, "", 0.0).HasValue());
    const PlannedRoute* route = nav.CurrentRoute();
    REQUIRE(route != nullptr);
    // the player is 700 m into the route: 400 m east on the street + 300 m up the west road
    const MapPosition player{r.layer, Vec2{900, 300}};
    const NavigationUpdate u = nav.Update(player, 1.0, &map);
    REQUIRE(u.status == NavigationStatus::Active);
    RouteOverlay overlay;
    overlay.route = &route->route;
    overlay.segmentIndex = u.segmentIndex;
    overlay.alongSegmentMetres = u.alongSegmentMetres;
    MinimapView v = View(r, 900, 300, 1.0);
    const MapRenderFrame f = MustBuild(r, v, Viewport(200.0), {}, &map, overlay);
    REQUIRE(!f.routeRemaining.empty());
    REQUIRE(!f.routeTravelled.empty());
    // 1 m per pixel: the viewport covers +-200 m around the player, so only the last 200 m of the 700 m travelled are visible
    double travelled = 0.0;
    for (const auto& piece : f.routeTravelled) { travelled += Length(piece) * f.metresPerPixel; }
    double remaining = 0.0;
    for (const auto& piece : f.routeRemaining) { remaining += Length(piece) * f.metresPerPixel; }
    CHECK(travelled == Approx(200.0));
    CHECK(remaining == Approx(600.0)); // 200 m of the west road ahead + 400 m where the route returns south along the east bank (x = 1100, inside the view)
    CHECK(f.routeRemaining.size() >= 2); // two separate pieces: the route leaves the viewport and re-enters it
    // the pieces meet at the player (screen centre)
    const Vec2 end = f.routeTravelled.back().back();
    const Vec2 start = f.routeRemaining.front().front();
    CHECK(end.x == Approx(0.0).epsilon(1e-6));
    CHECK(end.y == Approx(0.0).epsilon(1e-6));
    CHECK(start.x == Approx(0.0).epsilon(1e-6));
    CHECK(start.y == Approx(0.0).epsilon(1e-6));
    // every route point lies inside the viewport (clipped), and the route follows an emitted road
    for (const auto& piece : f.routeRemaining) {
        for (const Vec2 p : piece) { CHECK(std::abs(p.x) <= 200.0 + 1e-6); CHECK(std::abs(p.y) <= 200.0 + 1e-6); }
    }
    // the destination marker is 600 m east and 300 m south of the player: off screen: an indicator on the viewport edge points at it
    REQUIRE(f.destinationScreen.has_value());
    CHECK_FALSE(f.destinationInside);
    REQUIRE(f.destinationIndicator.has_value());
    CHECK(std::max(std::abs(f.destinationIndicator->x), std::abs(f.destinationIndicator->y)) == Approx(200.0));
    // the true marker position is not clipped
    CHECK(f.destinationScreen->x == Approx(600.0));
    CHECK(f.destinationScreen->y == Approx(300.0)); // north is up, so a target south of the player has positive screen y
}

TEST_CASE("MapRender: a nearby destination is inside the viewport and has no edge indicator, a circle viewport clips at its radius") {
    const auto r = MakeSyntheticMapRegion();
    NavigationConfig nc;
    nc.options.mode = TravelMode::OnFoot;
    NavigationSession nav(r.geometry, nc);
    Destination d;
    d.position = r.shopInside;
    REQUIRE(nav.SetDestination(MapPosition{r.layer, Vec2{400, 0}}, d, 0.0).status == NavigationStatus::Active);
    RouteOverlay overlay;
    overlay.route = &nav.CurrentRoute()->route;
    MinimapConfig circle = Viewport(100.0);
    circle.shape = MinimapShape::Circle;
    const MapRenderFrame f = MustBuild(r, View(r, 400, 0, 1.0), circle, {}, nullptr, overlay);
    CHECK(f.destinationInside);
    CHECK_FALSE(f.destinationIndicator.has_value());
    for (const auto& piece : f.routeRemaining) {
        for (const Vec2 p : piece) { CHECK(std::hypot(p.x, p.y) <= 100.0 + 1e-6); }
    }
    // no overlay, or an overlay on another layer, draws no route
    CHECK(MustBuild(r, View(r, 400, 0, 1.0), circle).routeRemaining.empty());
}

TEST_CASE("MapRender: invalid view and viewport are rejected") {
    const auto r = MakeSyntheticMapRegion();
    MinimapConfig bad = Viewport();
    bad.halfWidthPx = -1.0;
    CHECK_FALSE(BuildMapRenderFrame(*r.geometry, nullptr, View(r, 0, 0), bad, {}).HasValue());
    MinimapView v = View(r, 0, 0);
    v.player.xy.x = std::nan("");
    CHECK_FALSE(BuildMapRenderFrame(*r.geometry, nullptr, v, Viewport(), {}).HasValue());
    // a layer without features yields an empty frame, not an error
    MinimapView other = View(r, 0, 0);
    other.player.layer = MapLayerId(2);
    const auto frame = BuildMapRenderFrame(*r.geometry, nullptr, other, Viewport(), {});
    // the minimap transform itself accepts any layer id; the geometry has nothing there
    if (frame.HasValue()) { CHECK(frame.Value().roads.empty()); }
}
