#include <cmath>
#include <doctest.h>
#include <limits>
#include <spn/core/Movement.h>

using namespace spn::core;

// M2-A (Docs/Proposals/0011): movement rules. The numbers are the placeholder test values of the
// work order, so these tests pin the rules and the defaults, not final gameplay values.

namespace {

constexpr double kGravity = 9.8; // m/s^2, engine default 980 cm/s^2

} // namespace

TEST_CASE("the default movement config is valid and holds the four work-order speeds") {
    const MovementConfig config;
    CHECK_FALSE(Validate(config).has_value());
    CHECK(GaitSpeed(config, Gait::Walk) == doctest::Approx(1.4));
    CHECK(GaitSpeed(config, Gait::Jog) == doctest::Approx(3.0));
    CHECK(GaitSpeed(config, Gait::Run) == doctest::Approx(5.0));
    CHECK(GaitSpeed(config, Gait::Sprint) == doctest::Approx(8.0));
    CHECK(config.jumpHeight == doctest::Approx(0.45));
    CHECK(config.defaultGait != Gait::Sprint);
}

TEST_CASE("Validate rejects broken configs") {
    const auto invalid = [](auto&& edit) {
        MovementConfig config;
        edit(config);
        return Validate(config).has_value();
    };
    CHECK(invalid([](MovementConfig& c) { c.gaits[0].speed = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.gaits[1].speed = -1.0; }));
    CHECK(invalid([](MovementConfig& c) { c.gaits[2].acceleration = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.gaits[3].speed = std::numeric_limits<double>::quiet_NaN(); }));
    CHECK(invalid([](MovementConfig& c) { c.gaits[3].speed = std::numeric_limits<double>::infinity(); }));
    // Strictly ascending: equal and descending speeds fail.
    CHECK(invalid([](MovementConfig& c) { c.gaits[1].speed = c.gaits[0].speed; }));
    CHECK(invalid([](MovementConfig& c) { c.gaits[2].speed = 2.0; }));
    CHECK(invalid([](MovementConfig& c) { c.defaultGait = Gait::Sprint; }));
    CHECK(invalid([](MovementConfig& c) { c.brakingDeceleration = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.backwardSpeedFactor = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.backwardSpeedFactor = 1.1; }));
    CHECK(invalid([](MovementConfig& c) { c.strafeSpeedFactor = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.sprintMaxAngleDegrees = -1.0; }));
    CHECK(invalid([](MovementConfig& c) { c.sprintMaxAngleDegrees = 181.0; }));
    CHECK(invalid([](MovementConfig& c) { c.jumpHeight = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.jumpHeight = 2.5; }));
    CHECK(invalid([](MovementConfig& c) { c.maxStepHeight = -0.1; }));
    CHECK(invalid([](MovementConfig& c) { c.maxStepHeight = 1.5; }));
    // Boundary values that are still fine.
    CHECK_FALSE(invalid([](MovementConfig& c) { c.backwardSpeedFactor = 1.0; }));
    CHECK_FALSE(invalid([](MovementConfig& c) { c.maxStepHeight = 0.0; }));
    CHECK_FALSE(invalid([](MovementConfig& c) { c.sprintMaxAngleDegrees = 180.0; }));
}

TEST_CASE("gait indices round-trip and out-of-range values are rejected") {
    for (unsigned i = 0; i < kGaitCount; ++i) {
        const auto gait = GaitFromIndex(i);
        REQUIRE(gait.has_value());
        CHECK(static_cast<unsigned>(*gait) == i);
    }
    CHECK_FALSE(GaitFromIndex(4).has_value());
    CHECK_FALSE(GaitFromIndex(255).has_value());
    CHECK(std::string(GaitName(Gait::Sprint)) == "Sprint");
}

TEST_CASE("jump velocity and apex height are consistent") {
    const double v = JumpVelocityForHeight(0.45, kGravity);
    CHECK(v == doctest::Approx(std::sqrt(2.0 * 9.8 * 0.45)));
    CHECK(JumpApexHeight(v, kGravity) == doctest::Approx(0.45));

    // 45 cm under engine gravity is a launch of about 297 cm/s (the prototype used 500 cm/s = 127 cm).
    CHECK(v * 100.0 == doctest::Approx(297.0).epsilon(0.005));
    CHECK(JumpApexHeight(5.0, kGravity) == doctest::Approx(1.2755).epsilon(0.001));

    // Lower gravity needs a lower launch for the same height; the apex stays the same.
    const double moon = JumpVelocityForHeight(0.45, 1.6);
    CHECK(moon < v);
    CHECK(JumpApexHeight(moon, 1.6) == doctest::Approx(0.45));

    // Invalid inputs give 0, never NaN or a negative number.
    CHECK(JumpVelocityForHeight(0.0, kGravity) == 0.0);
    CHECK(JumpVelocityForHeight(-1.0, kGravity) == 0.0);
    CHECK(JumpVelocityForHeight(0.45, 0.0) == 0.0);
    CHECK(JumpVelocityForHeight(0.45, -9.8) == 0.0);
    CHECK(JumpVelocityForHeight(std::numeric_limits<double>::quiet_NaN(), kGravity) == 0.0);
    CHECK(JumpApexHeight(3.0, 0.0) == 0.0);
    CHECK(JumpApexHeight(-3.0, kGravity) == 0.0);
}

TEST_CASE("gait limits cap the gait and scale the speed") {
    const MovementConfig config;
    GaitLimits none;
    CHECK(EffectiveSpeed(config, Gait::Sprint, none) == doctest::Approx(8.0));

    GaitLimits hurt;
    hurt.maxGait = Gait::Jog;
    hurt.speedScale = 0.5;
    CHECK(EffectiveSpeed(config, Gait::Sprint, hurt) == doctest::Approx(1.5)); // capped to Jog, halved
    CHECK(EffectiveSpeed(config, Gait::Walk, hurt) == doctest::Approx(0.7));
}

TEST_CASE("ResolveGait: without a sprint request the base gait is used, clamped by the limits") {
    const MovementConfig config;
    const GaitLimits none;
    const MoveIntent forward{1.0, 0.0};
    CHECK(ResolveGait(config, Gait::Walk, false, none, forward) == Gait::Walk);
    CHECK(ResolveGait(config, Gait::Jog, false, none, forward) == Gait::Jog);
    CHECK(ResolveGait(config, Gait::Run, false, none, forward) == Gait::Run);
    CHECK(ResolveGait(config, Gait::Sprint, false, none, forward) == Gait::Run); // Sprint is never a base gait

    GaitLimits tired;
    tired.maxGait = Gait::Jog;
    CHECK(ResolveGait(config, Gait::Run, false, tired, forward) == Gait::Jog);
    CHECK(ResolveGait(config, Gait::Run, true, tired, forward) == Gait::Jog); // sprint above the ceiling
    CHECK(ResolveGait(config, Gait::Walk, true, tired, forward) == Gait::Walk);
}

TEST_CASE("ResolveGait: sprint needs forward input within the angle, else the base gait") {
    const MovementConfig config; // 50 degrees
    const GaitLimits none;
    const auto sprint = [&](MoveIntent in, Gait base = Gait::Jog) { return ResolveGait(config, base, true, none, in); };
    CHECK(sprint({1.0, 0.0}) == Gait::Sprint);
    CHECK(sprint({0.5, 0.0}) == Gait::Sprint);   // analog half stick
    CHECK(sprint({1.0, 1.0}) == Gait::Sprint);   // 45 degrees
    CHECK(sprint({1.0, -1.0}) == Gait::Sprint);
    CHECK(sprint({1.0, 1.4}) == Gait::Jog);      // ~54.5 degrees: falls back to the chosen base gait
    CHECK(sprint({0.0, 1.0}) == Gait::Jog);      // sideways
    CHECK(sprint({-1.0, 0.0}) == Gait::Jog);     // backwards
    CHECK(sprint({0.0, 0.0}) == Gait::Jog);      // no input
    CHECK(sprint({-1.0, 0.0}, Gait::Walk) == Gait::Walk);
    CHECK(sprint({-1.0, 0.0}, Gait::Run) == Gait::Run);

    MovementConfig narrow;
    narrow.sprintMaxAngleDegrees = 90.0;
    CHECK(ResolveGait(narrow, Gait::Jog, true, none, {1e-9, 1.0}) == Gait::Sprint);
    narrow.sprintMaxAngleDegrees = 0.0;
    CHECK(ResolveGait(narrow, Gait::Jog, true, none, {1.0, 0.0}) == Gait::Sprint);
    CHECK(ResolveGait(narrow, Gait::Jog, true, none, {1.0, 0.01}) == Gait::Jog);
}

TEST_CASE("direction speed scale: forward 1, sideways strafe factor, backward factor") {
    const MovementConfig config;
    CHECK(DirectionSpeedScale(config, {1.0, 0.0}) == doctest::Approx(1.0));
    CHECK(DirectionSpeedScale(config, {0.3, 0.0}) == doctest::Approx(1.0)); // analog magnitude is not direction
    CHECK(DirectionSpeedScale(config, {0.0, 1.0}) == doctest::Approx(config.strafeSpeedFactor));
    CHECK(DirectionSpeedScale(config, {0.0, -1.0}) == doctest::Approx(config.strafeSpeedFactor));
    CHECK(DirectionSpeedScale(config, {-1.0, 0.0}) == doctest::Approx(config.backwardSpeedFactor));
    CHECK(DirectionSpeedScale(config, {0.0, 0.0}) == doctest::Approx(1.0));
    // Always within [backward, 1], continuous around the circle.
    double previous = DirectionSpeedScale(config, {1.0, 0.0});
    for (int degrees = 1; degrees <= 360; ++degrees) {
        const double radians = degrees * 3.14159265358979323846 / 180.0;
        const double scale = DirectionSpeedScale(config, {std::cos(radians), std::sin(radians)});
        CHECK(scale >= config.backwardSpeedFactor - 1e-9);
        CHECK(scale <= 1.0 + 1e-9);
        CHECK(std::abs(scale - previous) < 0.02);
        previous = scale;
    }
}

TEST_CASE("obstacle classes follow the step and jump heights, and 50 cm is not a high jump") {
    const MovementConfig config; // step 45 cm, jump 45 cm
    for (const double h : {0.0, 0.20, 0.30, 0.40, 0.45}) {
        CHECK(ClassifyObstacle(config, h) == ObstacleClass::StepUp);
    }
    for (const double h : {0.50, 0.60, 0.80, 1.00, 1.20, 1.50}) {
        // With equal step and jump heights nothing in between can be jumped: no high jump is required or allowed.
        CHECK(ClassifyObstacle(config, h) == ObstacleClass::Blocked);
    }
    CHECK(ClassifyObstacle(config, -0.5) == ObstacleClass::StepUp);

    MovementConfig lowStep = config;
    lowStep.maxStepHeight = 0.30;
    CHECK(ClassifyObstacle(lowStep, 0.30) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(lowStep, 0.40) == ObstacleClass::Jump);
    CHECK(ClassifyObstacle(lowStep, 0.45) == ObstacleClass::Jump);
    CHECK(ClassifyObstacle(lowStep, 0.46) == ObstacleClass::Blocked);
}
