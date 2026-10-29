#include <cmath>
#include <doctest.h>
#include <numbers>
#include <spn/core/Minimap.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

constexpr double kHalfPi = std::numbers::pi / 2.0;

/// 100 px circle, 1 m per pixel at zoom 1: easy numbers (these are test values, not UI defaults).
MinimapConfig Circle() {
    MinimapConfig c;
    c.shape = MinimapShape::Circle;
    c.halfWidthPx = 100.0;
    c.baseMetresPerPixel = 1.0;
    return c;
}

MinimapConfig Rect() {
    MinimapConfig c;
    c.shape = MinimapShape::Rectangle;
    c.halfWidthPx = 100.0;
    c.halfHeightPx = 50.0;
    return c;
}

MinimapView View(MapPosition player, double heading = 0.0, MinimapRotation rot = MinimapRotation::NorthUp, double zoom = 1.0) {
    return MinimapView{player, heading, zoom, rot};
}

MinimapTransform Make(const MinimapConfig& c, const MinimapView& v) {
    auto t = MinimapTransform::Create(c, v);
    REQUIRE(t.HasValue());
    return t.Value();
}

void CheckNear(Vec2 a, Vec2 b, double eps = 1e-9) {
    CHECK(a.x == doctest::Approx(b.x).epsilon(eps).scale(1.0));
    CHECK(a.y == doctest::Approx(b.y).epsilon(eps).scale(1.0));
}

} // namespace

// --- Transform ------------------------------------------------------------------------------------------------------------------

TEST_CASE("Minimap NorthUp: east is right, north is up (screen y grows downwards)") {
    const auto t = Make(Circle(), View(At(kSurface, 1000, 2000)));
    CheckNear(t.WorldToScreen({1000, 2000}), {0, 0});
    CheckNear(t.WorldToScreen({1050, 2000}), {50, 0});
    CheckNear(t.WorldToScreen({1000, 2050}), {0, -50});
    CheckNear(t.WorldToScreen({950, 1950}), {-50, 50});
    CHECK(t.RotationRadians() == 0.0);
    CHECK(t.NorthOnScreenRadians() == 0.0);
}

TEST_CASE("Minimap HeadingUp: the player's heading points up") {
    const MinimapConfig c = Circle();
    const Vec2 player{0, 0};
    // Facing east: east is up, north is to the left, south to the right, west down.
    const auto east = Make(c, View(At(kSurface, 0, 0), kHalfPi, MinimapRotation::HeadingUp));
    CheckNear(east.WorldToScreen({50, 0}), {0, -50});
    CheckNear(east.WorldToScreen({0, 50}), {-50, 0});
    CheckNear(east.WorldToScreen({0, -50}), {50, 0});
    CheckNear(east.WorldToScreen({-50, 0}), {0, 50});
    // Facing south: everything is point-mirrored.
    const auto south = Make(c, View(At(kSurface, 0, 0), std::numbers::pi, MinimapRotation::HeadingUp));
    CheckNear(south.WorldToScreen({0, -50}), {0, -50});
    CheckNear(south.WorldToScreen({50, 0}), {-50, 0});
    // Facing north equals NorthUp.
    const auto north = Make(c, View(At(kSurface, 0, 0), 0.0, MinimapRotation::HeadingUp));
    CheckNear(north.WorldToScreen({30, 40}), {30, -40});
    // Facing north-east: a point 70.71 m north-east is straight up.
    const auto ne = Make(c, View(At(kSurface, 0, 0), kHalfPi / 2.0, MinimapRotation::HeadingUp));
    CheckNear(ne.WorldToScreen({50, 50}), {0, -std::hypot(50.0, 50.0)});
    // The compass needle: with east up, north appears 90 degrees counter-clockwise from up = 270 degrees clockwise.
    CHECK(east.NorthOnScreenRadians() == doctest::Approx(3.0 * kHalfPi));
    (void)player;
}

TEST_CASE("Minimap: rotation never changes distances") {
    Lcg rng(5);
    for (int i = 0; i < 200; ++i) {
        const double heading = rng.Uniform(-20, 20);
        const auto t = Make(Circle(), View(At(kSurface, 0, 0), heading, MinimapRotation::HeadingUp, 2.0));
        const Vec2 w{rng.Uniform(-500, 500), rng.Uniform(-500, 500)};
        const Vec2 s = t.WorldToScreen(w);
        CHECK(std::hypot(s.x, s.y) * t.MetresPerPixel() == doctest::Approx(std::hypot(w.x, w.y)).epsilon(1e-12));
    }
}

TEST_CASE("Minimap: ScreenToWorld inverts WorldToScreen for every heading, zoom and magnitude") {
    Lcg rng(11);
    for (int i = 0; i < 300; ++i) {
        const double heading = rng.Uniform(-50, 50);
        const double zoom = rng.Uniform(0.25, 8.0);
        const Vec2 player{rng.Uniform(-9e6, 9e6), rng.Uniform(-9e6, 9e6)};
        const auto rot = (i % 2 == 0) ? MinimapRotation::NorthUp : MinimapRotation::HeadingUp;
        const auto t = Make(Circle(), View({MapLayerId(kFar), player}, heading, rot, zoom));
        const Vec2 world{player.x + rng.Uniform(-300, 300), player.y + rng.Uniform(-300, 300)};
        const Vec2 back = t.ScreenToWorld(t.WorldToScreen(world));
        CHECK(back.x == doctest::Approx(world.x).epsilon(1e-12).scale(1.0));
        CHECK(back.y == doctest::Approx(world.y).epsilon(1e-12).scale(1.0));
        CHECK(std::abs(back.x - world.x) < 1e-6); // sub-micrometre even at 9,000 km
    }
}

TEST_CASE("Minimap: large absolute coordinates cost no precision") {
    const double far = 9'000'000.0;
    const auto near = Make(Circle(), View(At(kFar, 0, 0)));
    const auto distant = Make(Circle(), View(At(kFar, far, -far)));
    const Vec2 a = near.WorldToScreen({12.375, -3.125});
    const Vec2 b = distant.WorldToScreen({far + 12.375, -far - 3.125});
    CHECK(a.x == b.x); // bit-identical: the subtraction happens in double before anything else
    CHECK(a.y == b.y);
}

TEST_CASE("Minimap zoom: scale changes, is clamped, and 0 < zoom is required") {
    const MinimapConfig c = Circle();
    CHECK(Make(c, View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 1.0)).MetresPerPixel() == 1.0);
    const auto in = Make(c, View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 2.0));
    CHECK(in.MetresPerPixel() == 0.5);
    CheckNear(in.WorldToScreen({10, 0}), {20, 0});
    CHECK(Make(c, View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 1000.0)).Zoom() == c.maxZoom);
    CHECK(Make(c, View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 1e-9)).Zoom() == c.minZoom);
    for (const double bad : {0.0, -1.0, std::nan(""), kInf}) {
        CHECK(MinimapTransform::Create(c, View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, bad)).Error().code == MinimapErrorCode::InvalidView);
    }
}

TEST_CASE("Minimap: non-finite input and invalid configs are rejected, extreme headings normalize") {
    const MinimapConfig c = Circle();
    CHECK(MinimapTransform::Create(c, View(At(kSurface, std::nan(""), 0))).Error().code == MinimapErrorCode::InvalidView);
    CHECK(MinimapTransform::Create(c, View(At(kSurface, 0, kInf))).Error().code == MinimapErrorCode::InvalidView);
    CHECK(MinimapTransform::Create(c, View(At(kSurface, 0, 0), std::nan(""))).Error().code == MinimapErrorCode::InvalidView);
    CHECK(MinimapTransform::Create(c, View(At(kSurface, 0, 0), kInf)).Error().code == MinimapErrorCode::InvalidView);
    for (int i = 0; i < 7; ++i) {
        MinimapConfig bad = Circle();
        switch (i) {
        case 0: bad.halfWidthPx = 0.0; break;
        case 1: bad.baseMetresPerPixel = -1.0; break;
        case 2: bad.minZoom = 4.0, bad.maxZoom = 2.0; break;
        case 3: bad.indicatorInsetPx = bad.halfWidthPx; break;
        case 4: bad.offscreenIndicatorRangeMetres = -1.0; break;
        case 5: bad.halfWidthPx = std::nan(""); break;
        default: bad = Rect(), bad.halfHeightPx = 0.0; break;
        }
        CHECK(ValidateConfig(bad).has_value());
        CHECK(MinimapTransform::Create(bad, View(At(kSurface, 0, 0))).Error().code == MinimapErrorCode::InvalidConfig);
    }
    // A huge but finite heading is reduced mathematically, not rejected, and still gives a finite, distance-preserving mapping.
    const auto t = Make(c, View(At(kSurface, 0, 0), 1.0e9, MinimapRotation::HeadingUp));
    CHECK(t.RotationRadians() >= 0.0);
    CHECK(t.RotationRadians() < 2.0 * std::numbers::pi);
    const Vec2 s = t.WorldToScreen({30, 40});
    CHECK(std::hypot(s.x, s.y) == doctest::Approx(50.0).epsilon(1e-12));
}

TEST_CASE("NormalizeAngle stays in the half-open range from zero to two pi") {
    CHECK(NormalizeAngle(0.0) == 0.0);
    CHECK(NormalizeAngle(2.0 * std::numbers::pi) == 0.0);
    CHECK(NormalizeAngle(-kHalfPi) == doctest::Approx(3.0 * kHalfPi));
    CHECK(NormalizeAngle(-1e-18) < 2.0 * std::numbers::pi); // -tiny + 2*pi rounds to 2*pi: must wrap to 0
    CHECK(NormalizeAngle(-1e-18) >= 0.0);
    CHECK(NormalizeAngle(7.0 * std::numbers::pi) == doctest::Approx(std::numbers::pi));
}

// --- Viewport geometry ----------------------------------------------------------------------------------------------------------

TEST_CASE("Viewport: circle and rectangle containment is inclusive with a tiny tolerance") {
    const MinimapConfig c = Circle();
    CHECK(InsideViewport(c, {0, 0}));
    CHECK(InsideViewport(c, {100, 0}));
    CHECK(InsideViewport(c, {0, -100}));
    CHECK(InsideViewport(c, {70.7106781186, 70.7106781186}));          // on the circle within rounding
    CHECK_FALSE(InsideViewport(c, {70.72, 70.72}));
    CHECK_FALSE(InsideViewport(c, {100.001, 0}));
    CHECK(InsideViewport(c, {100.0 + kViewportEpsilonPx / 2.0, 0}));
    const MinimapConfig r = Rect();
    CHECK(InsideViewport(r, {100, 50}));   // the corner belongs to the rectangle (but not to the circle)
    CHECK(InsideViewport(r, {-100, -50}));
    CHECK_FALSE(InsideViewport(r, {100.01, 0}));
    CHECK_FALSE(InsideViewport(r, {0, 50.01}));
    CHECK_FALSE(InsideViewport(c, {100, 50}));
}

TEST_CASE("Viewport: projection to the edge keeps the direction and honours the inset") {
    const MinimapConfig c = Circle();
    CheckNear(ProjectToViewportEdge(c, {300, 0}, 0.0), {100, 0});
    CheckNear(ProjectToViewportEdge(c, {0, -5}, 10.0), {0, -90});
    const Vec2 diag = ProjectToViewportEdge(c, {300, 300}, 0.0);
    CHECK(std::hypot(diag.x, diag.y) == doctest::Approx(100.0));
    CHECK(diag.x == doctest::Approx(diag.y));
    CheckNear(ProjectToViewportEdge(c, {0, 0}, 0.0), {0, -100}); // zero vector: up, never NaN
    const MinimapConfig r = Rect(); // 100 x 50 half sizes
    CheckNear(ProjectToViewportEdge(r, {1000, 0}, 0.0), {100, 0});
    CheckNear(ProjectToViewportEdge(r, {0, 1000}, 0.0), {0, 50});
    CheckNear(ProjectToViewportEdge(r, {200, 100}, 0.0), {100, 50}); // exactly toward the corner
    CheckNear(ProjectToViewportEdge(r, {400, 100}, 0.0), {100, 25}); // hits the right edge first
    CheckNear(ProjectToViewportEdge(r, {100, 400}, 0.0), {12.5, 50}); // hits the bottom edge first
    CheckNear(ProjectToViewportEdge(r, {1000, 0}, 8.0), {92, 0});
    CHECK(ViewportReachPx(c) == 100.0);
    CHECK(ViewportReachPx(r) == doctest::Approx(std::hypot(100.0, 50.0)));
}

// --- Frame ----------------------------------------------------------------------------------------------------------------------

namespace {
WorldMapState MapWithMarkers(std::vector<Vec2> positions, std::uint32_t layer = kSurface, const char* category = "test:cat.note") {
    WorldMapState map(MakeBareDefinition()); // no POIs: only the markers below can appear in a frame
    for (const Vec2 p : positions) {
        MarkerSpec s;
        s.position = At(layer, p.x, p.y);
        s.category = Cid(category);
        REQUIRE(map.AddMarker(s).HasValue());
    }
    return map;
}
} // namespace

TEST_CASE("Minimap frame: markers inside the circle, nearest first, with screen positions and distances") {
    const WorldMapState map = MapWithMarkers({{60, 0}, {0, 20}, {0, -300}});
    const auto frame = BuildMinimapFrame(map, View(At(kSurface, 0, 0)), Circle());
    REQUIRE(frame.HasValue());
    const MinimapFrame& f = frame.Value();
    REQUIRE(f.markers.size() == 2); // the third is 300 m away, outside the 100 m view
    CHECK(f.indicators.empty());    // indicators are off by default
    CHECK(f.markers[0].hit.marker->id == MarkerId(2));
    CheckNear(f.markers[0].screen, {0, -20});
    CHECK(f.markers[0].distanceMetres == doctest::Approx(20.0));
    CheckNear(f.markers[1].screen, {60, 0});
    CHECK(f.layer == MapLayerId(kSurface));
    CHECK(f.metresPerPixel == 1.0);
}

TEST_CASE("Minimap frame: zoom changes what fits in the view") {
    const WorldMapState map = MapWithMarkers({{150, 0}});
    CHECK(BuildMinimapFrame(map, View(At(kSurface, 0, 0)), Circle()).Value().markers.empty());
    const auto zoomedOut = BuildMinimapFrame(map, View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 0.5), Circle()).Value();
    REQUIRE(zoomedOut.markers.size() == 1); // 0.5x zoom: 2 m per pixel, the view now spans 200 m
    CheckNear(zoomedOut.markers[0].screen, {75, 0});
    const auto zoomedIn = BuildMinimapFrame(MapWithMarkers({{40, 0}}), View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 4.0), Circle()).Value();
    CHECK(zoomedIn.markers.empty()); // 40 m = 160 px at 4x: outside a 100 px circle
}

TEST_CASE("Minimap frame: boundary markers are classified consistently under rotation") {
    // A marker exactly 100 m away (= the 100 px radius): inside in every orientation despite rounding in sin/cos.
    for (int i = 0; i < 64; ++i) {
        const double heading = static_cast<double>(i) * 0.1;
        const WorldMapState map = MapWithMarkers({{100, 0}});
        const auto frame = BuildMinimapFrame(map, View(At(kSurface, 0, 0), heading, MinimapRotation::HeadingUp), Circle());
        CHECK(frame.Value().markers.size() == 1);
    }
    const WorldMapState outside = MapWithMarkers({{100.01, 0}});
    CHECK(BuildMinimapFrame(outside, View(At(kSurface, 0, 0), 1.0, MinimapRotation::HeadingUp), Circle()).Value().markers.empty());
}

TEST_CASE("Minimap frame: rectangle viewport corners and aspect") {
    // 100 x 50 half-size rectangle. A marker at (90, 45) is inside, (90, 55) is outside though within the circumscribed radius.
    const WorldMapState map = MapWithMarkers({{90, 45}, {90, 55}});
    const auto frame = BuildMinimapFrame(map, View(At(kSurface, 0, 0)), Rect());
    REQUIRE(frame.HasValue());
    CHECK(frame.Value().markers.size() == 1);
    CHECK(frame.Value().markers[0].hit.marker->id == MarkerId(1));
}

TEST_CASE("Minimap frame: off-screen indicators point at markers beyond the view, within the range") {
    MinimapConfig c = Circle();
    c.offscreenIndicatorRangeMetres = 500.0;
    c.indicatorInsetPx = 10.0;
    const WorldMapState map = MapWithMarkers({{0, 300}, {-400, 0}, {0, -600}, {20, 0}});
    const auto frame = BuildMinimapFrame(map, View(At(kSurface, 0, 0)), c);
    REQUIRE(frame.HasValue());
    const MinimapFrame& f = frame.Value();
    CHECK(f.markers.size() == 1);        // only (20, 0)
    REQUIRE(f.indicators.size() == 2);   // (0, 600) is beyond the 500 m range
    CHECK(f.indicators[0].hit.marker->id == MarkerId(1)); // nearest first: 300 m north
    CheckNear(f.indicators[0].screen, {0, -90});
    CHECK(f.indicators[0].angleRadians == doctest::Approx(0.0));
    CHECK(f.indicators[0].distanceMetres == doctest::Approx(300.0));
    CHECK(f.indicators[1].hit.marker->id == MarkerId(2)); // 400 m west
    CheckNear(f.indicators[1].screen, {-90, 0});
    CHECK(f.indicators[1].angleRadians == doctest::Approx(3.0 * kHalfPi)); // 270 degrees clockwise from up = left
    for (const MinimapIndicator& ind : f.indicators) {
        CHECK(std::hypot(ind.screen.x, ind.screen.y) == doctest::Approx(90.0)); // on the inset edge
    }
}

TEST_CASE("Minimap frame: indicators rotate with the heading-up view") {
    MinimapConfig c = Circle();
    c.offscreenIndicatorRangeMetres = 1000.0;
    const WorldMapState map = MapWithMarkers({{0, 400}});
    // Facing east, a marker to the north is on the player's left.
    const auto f = BuildMinimapFrame(map, View(At(kSurface, 0, 0), kHalfPi, MinimapRotation::HeadingUp), c).Value();
    REQUIRE(f.indicators.size() == 1);
    CheckNear(f.indicators[0].screen, {-100, 0});
    CHECK(f.indicators[0].angleRadians == doctest::Approx(3.0 * kHalfPi));
    CHECK(f.northOnScreenRadians == doctest::Approx(3.0 * kHalfPi));
}

TEST_CASE("Minimap frame: rectangle indicators sit on the rectangle edge") {
    MinimapConfig c = Rect();
    c.offscreenIndicatorRangeMetres = 1000.0;
    const WorldMapState map = MapWithMarkers({{400, 100}});
    const auto f = BuildMinimapFrame(map, View(At(kSurface, 0, 0)), c).Value();
    REQUIRE(f.indicators.size() == 1);
    CheckNear(f.indicators[0].screen, {100, -25}); // direction (4, -1) hits the right edge at x = 100
}

TEST_CASE("Minimap frame: a marker exactly at the player is drawn at the centre") {
    const WorldMapState map = MapWithMarkers({{500, 500}});
    const auto f = BuildMinimapFrame(map, View(At(kSurface, 500, 500), 2.0, MinimapRotation::HeadingUp), Circle()).Value();
    REQUIRE(f.markers.size() == 1);
    CheckNear(f.markers[0].screen, {0, 0});
    CHECK(f.markers[0].distanceMetres == 0.0);
}

TEST_CASE("Minimap frame: only the player's layer is shown, layer choice follows the view") {
    WorldMapState map(MakeBareDefinition());
    MarkerSpec a;
    a.position = At(kSurface, 5, 5);
    a.category = Cid("test:cat.note");
    MarkerSpec b = a;
    b.position = At(kUnderground, 5, 5);
    MarkerSpec c = a;
    c.position = At(kFloor1, 5, 5);
    for (const MarkerSpec& s : {a, b, c}) {
        REQUIRE(map.AddMarker(s).HasValue());
    }
    const auto ids = [&](std::uint32_t layer) {
        std::vector<MarkerId> out;
        const auto frame = BuildMinimapFrame(map, View(At(layer, 0, 0)), Circle());
        for (const MinimapMarker& m : frame.Value().markers) {
            out.push_back(m.hit.marker->id);
        }
        return out;
    };
    CHECK(ids(kSurface) == std::vector<MarkerId>{MarkerId(1)});
    CHECK(ids(kUnderground) == std::vector<MarkerId>{MarkerId(2)});
    CHECK(ids(kFloor1) == std::vector<MarkerId>{MarkerId(3)});
    CHECK(ids(kFloor0).empty());
    CHECK(BuildMinimapFrame(map, View(At(99, 0, 0)), Circle()).Error().code == MinimapErrorCode::UnknownLayer);
}

TEST_CASE("Minimap frame: visibility rules and category filters reach the minimap") {
    WorldMapState map(MakeTestDefinition());
    const MinimapConfig c = Circle();
    // POIs: church (0, 300) is too far; use a bigger view via zoom-out so POIs are in range.
    MinimapView v = View(At(kSurface, 0, 0), 0, MinimapRotation::NorthUp, 0.25);
    auto ids = [&](const MapFilter& f = {}) {
        std::vector<std::string> out;
        const auto frame = BuildMinimapFrame(map, v, c, f);
        for (const MinimapMarker& m : frame.Value().markers) {
            out.push_back(m.hit.poi != nullptr ? m.hit.poi->id.Value() : "marker");
        }
        return out;
    };
    CHECK(ids() == std::vector<std::string>{"test:poi.church"}); // Always; shop undiscovered; secret hidden
    (void)map.Discover(Cid("test:poi.shop"), DiscoveryState::Known);
    CHECK(ids() == std::vector<std::string>{"test:poi.shop", "test:poi.church"});
    MapFilter shops;
    shops.categories = {Cid("test:cat.shop")};
    CHECK(ids(shops) == std::vector<std::string>{"test:poi.shop"});
}

TEST_CASE("Minimap frame: large world coordinates, far from the origin") {
    WorldMapState map(MakeTestDefinition());
    const double x = 9'899'950.0;
    const double y = -9'899'970.0;
    const auto frame = BuildMinimapFrame(map, View(At(kFar, x, y)), Circle());
    REQUIRE(frame.HasValue());
    REQUIRE(frame.Value().markers.size() == 1); // the "edge" POI: 50 m east, 30 m south
    CheckNear(frame.Value().markers[0].screen, {50.0, 30.0}, 1e-9);
    CHECK(frame.Value().markers[0].distanceMetres == doctest::Approx(std::hypot(50.0, 30.0)).epsilon(1e-12));
}

TEST_CASE("Minimap frame: an invalid config or view is reported, not rendered") {
    const WorldMapState map = MapWithMarkers({{1, 1}});
    MinimapConfig bad = Circle();
    bad.baseMetresPerPixel = 0.0;
    CHECK(BuildMinimapFrame(map, View(At(kSurface, 0, 0)), bad).Error().code == MinimapErrorCode::InvalidConfig);
    CHECK(BuildMinimapFrame(map, View(At(kSurface, std::nan(""), 0)), Circle()).Error().code == MinimapErrorCode::InvalidView);
}

TEST_CASE("Minimap frame: tiny metres-per-pixel does not produce non-finite screen coordinates silently") {
    // 1e-3 m/px at max zoom: a marker 1000 km away is 1e9 px away, still finite; it is simply an off-screen candidate.
    MinimapConfig c = Circle();
    c.baseMetresPerPixel = 1.0e-3;
    c.offscreenIndicatorRangeMetres = 2.0e6;
    WorldMapState map(MakeTestDefinition());
    MarkerSpec s;
    s.position = At(kFar, 1.0e6, 0.0);
    s.category = Cid("test:cat.note");
    REQUIRE(map.AddMarker(s).HasValue());
    const auto f = BuildMinimapFrame(map, View(At(kFar, 0, 0), 0.3, MinimapRotation::HeadingUp, 8.0), c).Value();
    CHECK(f.rejected == 0);
    REQUIRE(f.indicators.size() == 1);
    CHECK(std::isfinite(f.indicators[0].screen.x));
    CHECK(std::hypot(f.indicators[0].screen.x, f.indicators[0].screen.y) == doctest::Approx(100.0));
}
