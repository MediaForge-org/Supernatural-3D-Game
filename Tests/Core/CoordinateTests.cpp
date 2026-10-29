#include <cmath>
#include <doctest.h>
#include <limits>
#include <numbers>
#include <spn/core/Coordinates.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

constexpr double kInfinity = std::numeric_limits<double>::infinity();
constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
constexpr UnrealAxes kBoth[] = {UnrealAxes::EastSouthUp, UnrealAxes::NorthEastUp};

UnrealPoint Ue(const UnrealFrame& f, WorldPoint p) {
    auto r = WorldToUnreal(f, p);
    REQUIRE(r.HasValue());
    return r.Value();
}

} // namespace

TEST_CASE("canonical frame: metres, x east, y north, z up - the Unreal presets map axes and scale explicitly") {
    const WorldPoint p{1.0, 2.0, 3.0}; // 1 m east, 2 m north, 3 m up
    UnrealFrame a;
    a.axes = UnrealAxes::EastSouthUp;
    CHECK(Ue(a, p) == UnrealPoint{100.0, -200.0, 300.0}); // north is UE -Y
    UnrealFrame b;
    b.axes = UnrealAxes::NorthEastUp;
    CHECK(Ue(b, p) == UnrealPoint{200.0, 100.0, 300.0}); // north is UE +X
    // the canonical frame (east, north, up) is right-handed and Unreal is left-handed, so a correct conversion is a mirror: determinant -1 (times 100^3).
    // Both presets mirror exactly one horizontal axis relative to compass directions and keep up as up; a conversion with det +1 would be a bug.
    for (const auto axes : kBoth) {
        const UnrealFrame f{axes, {}};
        const auto e = Ue(f, {1, 0, 0}), n = Ue(f, {0, 1, 0}), u = Ue(f, {0, 0, 1});
        const double det = e.x * (n.y * u.z - n.z * u.y) - e.y * (n.x * u.z - n.z * u.x) + e.z * (n.x * u.y - n.y * u.x);
        CHECK(det == doctest::Approx(-1.0e6));
        CHECK(u.z == 100.0);
    }
}

TEST_CASE("unreal conversion: positive and negative coordinates, configurable origin, vertical coordinates") {
    for (const auto axes : kBoth) {
        const UnrealFrame zero{axes, {}};
        CHECK(Ue(zero, {0, 0, 0}) == UnrealPoint{0, 0, 0});
        const WorldPoint pts[] = {{-12345.678, 9876.543, -42.5}, {1.0e6, -2.0e6, 120.25}, {-9.9e6, 9.9e6, -9.9e4}};
        const UnrealFrame shifted{axes, {1000.0, -2000.0, 5.0}};
        for (const auto& p : pts) {
            for (const auto& f : {zero, shifted}) {
                const auto back = UnrealToWorld(f, Ue(f, p));
                REQUIRE(back.HasValue());
                CHECK(back.Value().x == doctest::Approx(p.x).epsilon(1e-12));
                CHECK(back.Value().y == doctest::Approx(p.y).epsilon(1e-12));
                CHECK(back.Value().z == doctest::Approx(p.z).epsilon(1e-12));
            }
        }
        // the origin maps to UE zero, and a point one metre east of it is exactly 100 cm along the east axis
        CHECK(Ue(shifted, shifted.originMetres) == UnrealPoint{0, 0, 0});
        const WorldPoint east{shifted.originMetres.x + 1.0, shifted.originMetres.y, shifted.originMetres.z};
        const auto d = Ue(shifted, east);
        CHECK(std::fabs(axes == UnrealAxes::EastSouthUp ? d.x : d.y) == doctest::Approx(100.0));
    }
}

TEST_CASE("unreal conversion: round trip at the edge of the world loses less than a micrometre") {
    const UnrealFrame f{UnrealAxes::EastSouthUp, {}};
    const double edge = kMaxMapCoordinateMetres;
    const WorldPoint pts[] = {{edge, edge, 0.0}, {-edge, -edge, 0.0}, {edge, -edge, kMaxWorldElevationMetres}, {123456.789012, -7654321.098765, -99999.5}};
    double worst = 0.0;
    for (const auto& p : pts) {
        const auto back = UnrealToWorld(f, Ue(f, p));
        REQUIRE(back.HasValue());
        worst = std::fmax(worst, std::fmax(std::fabs(back.Value().x - p.x), std::fmax(std::fabs(back.Value().y - p.y), std::fabs(back.Value().z - p.z))));
    }
    CHECK(worst < 1.0e-6);
    // a 2,200 km2 world is about 47 km across; there the error is far below that
    const WorldPoint small{23456.789, -23456.789, 100.123456};
    const auto back = UnrealToWorld(f, Ue(f, small));
    CHECK(std::fabs(back.Value().x - small.x) < 1.0e-9);
}

TEST_CASE("unreal conversion: invalid numbers and out-of-range values are rejected, never wrapped or clamped") {
    const UnrealFrame f{};
    for (const double bad : {kNaN, kInfinity, -kInfinity}) {
        for (int axis = 0; axis < 3; ++axis) {
            WorldPoint p{1, 2, 3};
            (axis == 0 ? p.x : axis == 1 ? p.y : p.z) = bad;
            REQUIRE_FALSE(WorldToUnreal(f, p).HasValue());
            CHECK(WorldToUnreal(f, p).Error().code == CoordinateErrorCode::NonFinite);
            UnrealPoint u{1, 2, 3};
            (axis == 0 ? u.x : axis == 1 ? u.y : u.z) = bad;
            CHECK(UnrealToWorld(f, u).Error().code == CoordinateErrorCode::NonFinite);
        }
    }
    CHECK(WorldToUnreal(f, {kMaxMapCoordinateMetres, 0, 0}).HasValue()); // the limit itself is valid
    CHECK(WorldToUnreal(f, {std::nextafter(kMaxMapCoordinateMetres, kInfinity), 0, 0}).Error().code == CoordinateErrorCode::OutOfRange);
    CHECK(WorldToUnreal(f, {0, -1.0e300, 0}).Error().code == CoordinateErrorCode::OutOfRange);
    CHECK(WorldToUnreal(f, {0, 0, kMaxWorldElevationMetres + 1.0}).Error().code == CoordinateErrorCode::OutOfRange);
    // overflow protection: absurd UE values never become a huge or infinite world value
    CHECK(UnrealToWorld(f, {1.0e300, 0, 0}).Error().code == CoordinateErrorCode::OutOfRange);
    CHECK(UnrealToWorld(f, {1.7e308, 1.7e308, 1.7e308}).Error().code == CoordinateErrorCode::OutOfRange);
    // a result that lands outside the world after adding the origin is rejected too
    const UnrealFrame edge{UnrealAxes::EastSouthUp, {kMaxMapCoordinateMetres, 0, 0}};
    CHECK(UnrealToWorld(edge, {100.0, 0, 0}).Error().code == CoordinateErrorCode::OutOfRange);
    // invalid frames
    const UnrealFrame bad{UnrealAxes::EastSouthUp, {kNaN, 0, 0}};
    CHECK(ValidateFrame(bad).has_value());
    CHECK(WorldToUnreal(bad, {0, 0, 0}).Error().code == CoordinateErrorCode::InvalidFrame);
    CHECK(UnrealToWorld(bad, {0, 0, 0}).Error().code == CoordinateErrorCode::InvalidFrame);
    CHECK_FALSE(ValidateFrame(UnrealFrame{}).has_value());
}

TEST_CASE("deltas use the axis map and the scale but never the origin") {
    for (const auto axes : kBoth) {
        const Vec3 d{3.0, -4.0, 0.5};
        const auto u = WorldDeltaToUnreal(axes, d);
        REQUIRE(u.HasValue());
        const auto back = UnrealDeltaToWorld(axes, u.Value());
        REQUIRE(back.HasValue());
        CHECK(back.Value() == d);
        CHECK(std::hypot(u.Value().x, u.Value().y) == doctest::Approx(500.0)); // a 5 m horizontal length stays 500 cm
    }
    CHECK_FALSE(WorldDeltaToUnreal(UnrealAxes::EastSouthUp, {kNaN, 0, 0}).HasValue());
    CHECK_FALSE(WorldDeltaToUnreal(UnrealAxes::EastSouthUp, {1.0e300, 0, 0}).HasValue());
}

TEST_CASE("bearing and unreal yaw: north, east, south, west for both presets, and the inverse") {
    constexpr double kHalfPi = std::numbers::pi / 2.0;
    const auto yaw = [](UnrealAxes a, double bearing) { return BearingToUnrealYawDegrees(a, bearing).Value(); };
    // NorthEastUp: UE +X is north, so yaw = bearing
    CHECK(yaw(UnrealAxes::NorthEastUp, 0.0) == doctest::Approx(0.0));
    CHECK(yaw(UnrealAxes::NorthEastUp, kHalfPi) == doctest::Approx(90.0));   // east
    CHECK(yaw(UnrealAxes::NorthEastUp, 2 * kHalfPi) == doctest::Approx(180.0));
    CHECK(yaw(UnrealAxes::NorthEastUp, 3 * kHalfPi) == doctest::Approx(-90.0)); // west
    // EastSouthUp: UE +X is east
    CHECK(yaw(UnrealAxes::EastSouthUp, kHalfPi) == doctest::Approx(0.0));       // east
    CHECK(yaw(UnrealAxes::EastSouthUp, 0.0) == doctest::Approx(-90.0));         // north is to the left of UE +X
    CHECK(yaw(UnrealAxes::EastSouthUp, 2 * kHalfPi) == doctest::Approx(90.0));  // south
    CHECK(yaw(UnrealAxes::EastSouthUp, 3 * kHalfPi) == doctest::Approx(180.0)); // west: the +180 end of the range, never -180
    // consistency with the position map: a heading's yaw points at the UE axis direction of the same compass direction
    for (const auto axes : kBoth) {
        const double bearing = 0.7;
        const Vec3 dir{std::sin(bearing), std::cos(bearing), 0.0}; // east, north components of a bearing
        const UnrealPoint u = WorldDeltaToUnreal(axes, dir).Value();
        const double ueYaw = std::atan2(u.y, u.x) * 180.0 / std::numbers::pi;
        CHECK(ueYaw == doctest::Approx(yaw(axes, bearing)));
        const double back = UnrealYawDegreesToBearing(axes, yaw(axes, bearing)).Value();
        CHECK(back == doctest::Approx(bearing));
    }
    CHECK(yaw(UnrealAxes::NorthEastUp, 10 * kHalfPi * 4.0 + 0.25) == doctest::Approx(0.25 * 180.0 / std::numbers::pi)); // many turns
    CHECK(UnrealYawDegreesToBearing(UnrealAxes::NorthEastUp, -90.0).Value() == doctest::Approx(3 * kHalfPi));
    CHECK_FALSE(BearingToUnrealYawDegrees(UnrealAxes::NorthEastUp, kNaN).HasValue());
    CHECK_FALSE(UnrealYawDegreesToBearing(UnrealAxes::NorthEastUp, kInfinity).HasValue());
}

TEST_CASE("local frames: translation in double precision keeps millimetres at the far edge - the float32 step explains why they are needed") {
    const LocalFrame frame{{9'000'000.25, -8'000'000.75, 12.5}};
    const WorldPoint p{9'000'001.5, -7'999'999.5, 14.0};
    const auto local = WorldToLocal(frame, p);
    REQUIRE(local.HasValue());
    CHECK(local.Value() == Vec3{1.25, 1.25, 1.5}); // exact: the subtraction happens in double
    CHECK(LocalToWorld(frame, local.Value()).Value() == p);
    // a float32 local coordinate is accurate; a float32 world coordinate is not
    CHECK(Float32StepMetres(1000.0) < 1.0e-4);
    CHECK(Float32StepMetres(9'000'000.0) >= 1.0);
    CHECK(Float32StepMetres(0.0) < 1.0e-30);
    CHECK(Float32StepMetres(-1000.0) == Float32StepMetres(1000.0));
    CHECK(std::isinf(Float32StepMetres(kNaN)));
    CHECK(std::isinf(Float32StepMetres(1.0e300)));
    const float asFloat = static_cast<float>(local.Value().x);
    CHECK(static_cast<double>(asFloat) == local.Value().x); // 1.25 is exactly representable
    // errors
    CHECK(WorldToLocal(frame, {kNaN, 0, 0}).Error().code == CoordinateErrorCode::NonFinite);
    CHECK(WorldToLocal(LocalFrame{{kInfinity, 0, 0}}, {0, 0, 0}).Error().code == CoordinateErrorCode::InvalidFrame);
    CHECK(LocalToWorld(frame, {2.0e6, 0, 0}).Error().code == CoordinateErrorCode::OutOfRange);
    CHECK(LocalToWorld(frame, {kNaN, 0, 0}).Error().code == CoordinateErrorCode::NonFinite);
}

TEST_CASE("map and minimap adapters agree with the canonical frame") {
    const MapLayerId layer{3};
    const auto mp = ToMapPosition(layer, {100.0, -200.0, 7.0});
    REQUIRE(mp.HasValue());
    CHECK(mp.Value().layer == layer);
    CHECK(mp.Value().xy == Vec2{100.0, -200.0}); // x east, y north: same numbers, z dropped
    CHECK(FromMapPosition(mp.Value(), 7.0).Value() == WorldPoint{100.0, -200.0, 7.0});
    CHECK_FALSE(ToMapPosition(layer, {kNaN, 0, 0}).HasValue());
    CHECK_FALSE(FromMapPosition(mp.Value(), kMaxWorldElevationMetres * 2).HasValue());

    MinimapConfig config;
    config.halfWidthPx = 100.0;
    config.baseMetresPerPixel = 1.0;
    MinimapView view;
    view.player = {layer, {100.0, -200.0}};
    const auto t = MinimapTransform::Create(config, view);
    REQUIRE(t.HasValue());
    // north-up: 10 m east of the player is 10 px right of the centre; 10 m north is 10 px UP, i.e. screen y -10
    const auto east = WorldToMinimapScreen(t.Value(), {110.0, -200.0, 50.0});
    CHECK(east.Value().x == doctest::Approx(10.0));
    CHECK(east.Value().y == doctest::Approx(0.0));
    CHECK(WorldToMinimapScreen(t.Value(), {100.0, -190.0, 0.0}).Value().y == doctest::Approx(-10.0));
    const auto back = MinimapScreenToWorld(t.Value(), {10.0, -10.0}, 4.0);
    CHECK(back.Value().x == doctest::Approx(110.0));
    CHECK(back.Value().y == doctest::Approx(-190.0));
    CHECK(back.Value().z == 4.0);
    CHECK_FALSE(WorldToMinimapScreen(t.Value(), {0, kInfinity, 0}).HasValue());
    CHECK_FALSE(MinimapScreenToWorld(t.Value(), {kNaN, 0}).HasValue());
}

TEST_CASE("the unreal presets are consistent with the map: a map x/y step equals the unreal step of the same compass direction") {
    // Minimap screen space is y-DOWN and Unreal viewport space is y-DOWN too; but world space is not screen space. This guards the mix-up:
    // "north" in the canonical frame is +y, never a screen axis.
    for (const auto axes : kBoth) {
        const UnrealFrame f{axes, {}};
        const auto north = Ue(f, {0, 1, 0});
        const auto east = Ue(f, {1, 0, 0});
        if (axes == UnrealAxes::NorthEastUp) {
            CHECK(north == UnrealPoint{100, 0, 0});
            CHECK(east == UnrealPoint{0, 100, 0});
        } else {
            CHECK(north == UnrealPoint{0, -100, 0});
            CHECK(east == UnrealPoint{100, 0, 0});
        }
    }
}
