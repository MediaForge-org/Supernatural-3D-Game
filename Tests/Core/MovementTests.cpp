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
    CHECK(GaitSpeed(config, Gait::Walk) == doctest::Approx(1.8));
    CHECK(GaitSpeed(config, Gait::Jog) == doctest::Approx(3.5));
    CHECK(GaitSpeed(config, Gait::Run) == doctest::Approx(5.5));
    CHECK(GaitSpeed(config, Gait::Sprint) == doctest::Approx(8.0));
    CHECK(config.jumpHeight == doctest::Approx(0.45));
    CHECK(config.maxStepHeight == doctest::Approx(0.25)); // M2-A.1: a human step, not the jump height
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
    CHECK(invalid([](MovementConfig& c) { c.maxStepHeight = std::numeric_limits<double>::quiet_NaN(); }));
    CHECK(invalid([](MovementConfig& c) { c.jumpHeight = std::numeric_limits<double>::quiet_NaN(); }));
    CHECK(invalid([](MovementConfig& c) { c.sprintSustainAngleDegrees = 40.0; })); // below the start angle (50)
    CHECK(invalid([](MovementConfig& c) { c.sprintSustainAngleDegrees = 181.0; }));
    CHECK(invalid([](MovementConfig& c) { c.turnRateDegrees = 0.0; }));
    CHECK(invalid([](MovementConfig& c) { c.turnRateDegrees = 4000.0; }));
    CHECK(invalid([](MovementConfig& c) { c.minAnalogSpeed = -0.1; }));
    CHECK(invalid([](MovementConfig& c) { c.minAnalogSpeed = 2.0; })); // above the walk speed
    CHECK(invalid([](MovementConfig& c) { c.idleSpeed = -0.01; }));
    CHECK(invalid([](MovementConfig& c) { c.idleSpeed = 1.8; })); // must stay below the walk speed
    CHECK(invalid([](MovementConfig& c) { c.landingSeconds = -0.1; }));
    CHECK(invalid([](MovementConfig& c) { c.landingSeconds = 2.5; }));
    CHECK(invalid([](MovementConfig& c) { c.gaits[0].acceleration = std::numeric_limits<double>::infinity(); }));
    // Boundary values that are still fine.
    CHECK_FALSE(invalid([](MovementConfig& c) { c.backwardSpeedFactor = 1.0; }));
    CHECK_FALSE(invalid([](MovementConfig& c) { c.maxStepHeight = 0.0; }));
    CHECK_FALSE(invalid([](MovementConfig& c) { c.sprintMaxAngleDegrees = 180.0; c.sprintSustainAngleDegrees = 180.0; }));
    CHECK_FALSE(invalid([](MovementConfig& c) { c.sprintSustainAngleDegrees = c.sprintMaxAngleDegrees; })); // equal: no hysteresis
    CHECK_FALSE(invalid([](MovementConfig& c) { c.landingSeconds = 0.0; }));
    CHECK_FALSE(invalid([](MovementConfig& c) { c.idleSpeed = 0.0; }));
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
    CHECK(EffectiveSpeed(config, Gait::Sprint, hurt) == doctest::Approx(1.75)); // capped to Jog, halved
    CHECK(EffectiveSpeed(config, Gait::Walk, hurt) == doctest::Approx(0.9));
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

TEST_CASE("obstacle classes: step-up, jump candidate and blocked follow the two heights") {
    const MovementConfig config; // step 25 cm, jump 45 cm
    struct Expected { double height; ObstacleClass cls; };
    // The nine heights of the M2-A.1 work order plus the boundaries.
    const Expected table[] = {
        {0.00, ObstacleClass::StepUp},        {0.20, ObstacleClass::StepUp},
        {0.25, ObstacleClass::StepUp},        // exactly the step height: still a step
        {0.2501, ObstacleClass::JumpCandidate},
        {0.30, ObstacleClass::JumpCandidate}, {0.40, ObstacleClass::JumpCandidate},
        {0.4499, ObstacleClass::JumpCandidate},
        {0.45, ObstacleClass::Blocked},       // the jump apex is reached for an instant only
        {0.50, ObstacleClass::Blocked},       {0.75, ObstacleClass::Blocked},
        {1.00, ObstacleClass::Blocked},       {1.50, ObstacleClass::Blocked},
    };
    for (const Expected& e : table) {
        CAPTURE(e.height);
        CHECK(ClassifyObstacle(config, e.height) == e.cls);
    }
    CHECK(ClassifyObstacle(config, -0.5) == ObstacleClass::StepUp); // negative counts as 0
}

TEST_CASE("obstacle classification never claims a jump clears every obstacle up to the jump height") {
    // A candidate is only a candidate: the class names say so and no class is called "Jump".
    CHECK(std::string(ObstacleClassName(ObstacleClass::JumpCandidate)) == "JumpCandidate");
    CHECK(std::string(ObstacleClassName(ObstacleClass::StepUp)) == "StepUp");
    CHECK(std::string(ObstacleClassName(ObstacleClass::Blocked)) == "Blocked");
}

TEST_CASE("step height and jump height are independent") {
    MovementConfig base;

    // Changing only the jump height leaves the step-up boundary where it was.
    MovementConfig highJump = base;
    highJump.jumpHeight = 0.90;
    CHECK(highJump.maxStepHeight == base.maxStepHeight);
    CHECK(ClassifyObstacle(highJump, 0.25) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(highJump, 0.26) == ObstacleClass::JumpCandidate);
    CHECK(ClassifyObstacle(highJump, 0.80) == ObstacleClass::JumpCandidate); // moved with the jump height
    CHECK(ClassifyObstacle(highJump, 0.90) == ObstacleClass::Blocked);
    CHECK_FALSE(Validate(highJump).has_value());

    // Changing only the step height leaves the jump apex where it was.
    MovementConfig highStep = base;
    highStep.maxStepHeight = 0.40;
    CHECK(highStep.jumpHeight == base.jumpHeight);
    CHECK(ClassifyObstacle(highStep, 0.40) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(highStep, 0.41) == ObstacleClass::JumpCandidate);
    CHECK(ClassifyObstacle(highStep, 0.45) == ObstacleClass::Blocked); // the apex did not move
    CHECK(JumpVelocityForHeight(highStep.jumpHeight, kGravity) == doctest::Approx(JumpVelocityForHeight(base.jumpHeight, kGravity)));

    // A step height above the jump height is a valid (if odd) setting: walking up wins, nothing in between.
    MovementConfig stepAboveJump = base;
    stepAboveJump.maxStepHeight = 0.60;
    CHECK_FALSE(Validate(stepAboveJump).has_value());
    CHECK(ClassifyObstacle(stepAboveJump, 0.50) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(stepAboveJump, 0.60) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(stepAboveJump, 0.61) == ObstacleClass::Blocked);

    // The step height does not enter the jump velocity at all.
    for (const double step : {0.0, 0.10, 0.25, 0.45, 1.0}) {
        MovementConfig c = base;
        c.maxStepHeight = step;
        CAPTURE(step);
        CHECK(JumpApexHeight(JumpVelocityForHeight(c.jumpHeight, kGravity), kGravity) == doctest::Approx(0.45));
    }
}

TEST_CASE("obstacle classification at numeric edges") {
    const MovementConfig config;
    CHECK(ClassifyObstacle(config, std::numeric_limits<double>::quiet_NaN()) == ObstacleClass::Blocked);
    CHECK(ClassifyObstacle(config, std::numeric_limits<double>::infinity()) == ObstacleClass::Blocked);
    CHECK(ClassifyObstacle(config, -std::numeric_limits<double>::infinity()) == ObstacleClass::Blocked);
    CHECK(ClassifyObstacle(config, std::numeric_limits<double>::max()) == ObstacleClass::Blocked);
    CHECK(ClassifyObstacle(config, std::numeric_limits<double>::denorm_min()) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(config, -0.0) == ObstacleClass::StepUp);

    MovementConfig noStep = config;
    noStep.maxStepHeight = 0.0;
    CHECK(ClassifyObstacle(noStep, 0.0) == ObstacleClass::StepUp);
    CHECK(ClassifyObstacle(noStep, 0.01) == ObstacleClass::JumpCandidate);
}

TEST_CASE("ResolveGait keeps the sprint through a sharp turn once at sprint speed") {
    const MovementConfig config; // start 50, sustain 100 degrees; sprint speed range starts at 6.75 m/s
    const GaitLimits none;
    const auto resolve = [&](MoveIntent in, double speed) { return ResolveGait(config, Gait::Jog, true, none, in, speed); };

    const MoveIntent turn90{0.0, 1.0};    // 90 degrees off the facing
    const MoveIntent turn120{-0.5, 0.87}; // about 120 degrees
    const MoveIntent behind{-1.0, 0.0};
    // Starting a sprint still needs the narrow angle.
    CHECK(resolve(turn90, 0.0) == Gait::Jog);
    CHECK(resolve(turn90, 3.0) == Gait::Jog);
    CHECK(resolve(turn90, 6.4) == Gait::Jog); // not yet at sprint speed
    // At sprint speed the wider angle holds the sprint.
    CHECK(resolve(turn90, 8.0) == Gait::Sprint);
    CHECK(resolve(turn90, 6.8) == Gait::Sprint);
    // Beyond the sustain angle the sprint ends, however fast the character is.
    CHECK(resolve(turn120, 8.0) == Gait::Jog);
    CHECK(resolve(behind, 8.0) == Gait::Jog);
    // Forward input is unaffected.
    CHECK(resolve({1.0, 0.0}, 0.0) == Gait::Sprint);

    // No hysteresis when both angles are equal.
    MovementConfig same = config;
    same.sprintSustainAngleDegrees = same.sprintMaxAngleDegrees;
    CHECK(ResolveGait(same, Gait::Jog, true, none, turn90, 8.0) == Gait::Jog);

    // The limits still win: a gait ceiling stops the sprint at any speed or angle.
    GaitLimits tired;
    tired.maxGait = Gait::Run;
    CHECK(ResolveGait(config, Gait::Jog, true, tired, {1.0, 0.0}, 8.0) == Gait::Jog);
    // Garbage speeds behave like standing still.
    CHECK(resolve(turn90, std::numeric_limits<double>::quiet_NaN()) == Gait::Jog);
    CHECK(resolve(turn90, -8.0) == Gait::Jog);
}

TEST_CASE("ObservedGait picks the nearest gait speed") {
    const MovementConfig config; // 1.8 / 3.5 / 5.5 / 8.0
    CHECK(ObservedGait(config, 0.0) == Gait::Walk);
    CHECK(ObservedGait(config, 1.8) == Gait::Walk);
    CHECK(ObservedGait(config, 2.64) == Gait::Walk); // the midpoint between Walk and Jog is 2.65
    CHECK(ObservedGait(config, 2.66) == Gait::Jog);
    CHECK(ObservedGait(config, 3.5) == Gait::Jog);
    CHECK(ObservedGait(config, 4.49) == Gait::Jog);  // midpoint Jog / Run is 4.5
    CHECK(ObservedGait(config, 4.51) == Gait::Run);
    CHECK(ObservedGait(config, 6.74) == Gait::Run);  // midpoint Run / Sprint is 6.75
    CHECK(ObservedGait(config, 6.76) == Gait::Sprint);
    CHECK(ObservedGait(config, 50.0) == Gait::Sprint);
    CHECK(ObservedGait(config, -3.0) == Gait::Walk);
    CHECK(ObservedGait(config, std::numeric_limits<double>::quiet_NaN()) == Gait::Walk);
    CHECK(ObservedGait(config, std::numeric_limits<double>::infinity()) == Gait::Walk);
}

TEST_CASE("ClassifyMotion distinguishes idle, the four gaits, jumping, falling and landing") {
    const MovementConfig config;
    const auto state = [&](double speed, double vertical, bool ground, double sinceLanding = -1.0) {
        return ClassifyMotion(config, MotionSample{speed, vertical, ground, sinceLanding});
    };
    CHECK(state(0.0, 0.0, true) == MovementState::Idle);
    CHECK(state(0.04, 0.0, true) == MovementState::Idle);
    CHECK(state(0.05, 0.0, true) == MovementState::Walk);
    CHECK(state(1.8, 0.0, true) == MovementState::Walk);
    CHECK(state(3.5, 0.0, true) == MovementState::Jog);
    CHECK(state(5.5, 0.0, true) == MovementState::Run);
    CHECK(state(8.0, 0.0, true) == MovementState::Sprint);

    // In the air only the vertical direction matters, whatever the horizontal speed (a sprinting jump).
    CHECK(state(0.0, 2.97, false) == MovementState::Jumping);
    CHECK(state(8.0, 2.97, false) == MovementState::Jumping);
    CHECK(state(8.0, 0.0, false) == MovementState::Falling); // the apex itself counts as falling
    CHECK(state(0.0, -4.0, false) == MovementState::Falling);

    // Landing lasts landingSeconds after the touch-down, then the ground states resume.
    CHECK(state(0.0, 0.0, true, 0.0) == MovementState::Landing);
    CHECK(state(5.0, 0.0, true, 0.10) == MovementState::Landing);
    CHECK(state(5.0, 0.0, true, 0.2499) == MovementState::Landing);
    CHECK(state(5.0, 0.0, true, 0.25) == MovementState::Run);
    CHECK(state(0.0, 0.0, true, 0.25) == MovementState::Idle);
    CHECK(state(0.0, 0.0, true, -1.0) == MovementState::Idle); // no recent landing
    CHECK(state(0.0, 0.0, true, std::numeric_limits<double>::infinity()) == MovementState::Idle);
    CHECK(state(0.0, 0.0, false, 0.0) == MovementState::Falling); // in the air again: not landing

    // A jump sequence in order: idle, rising, falling, landing, idle.
    MovementConfig noLanding = config;
    noLanding.landingSeconds = 0.0;
    CHECK(ClassifyMotion(noLanding, MotionSample{0.0, 0.0, true, 0.0}) == MovementState::Idle);

    // Garbage in, a defined state out.
    CHECK(state(std::numeric_limits<double>::quiet_NaN(), 0.0, true) == MovementState::Idle);
    CHECK(state(0.0, std::numeric_limits<double>::quiet_NaN(), false) == MovementState::Falling);
    for (std::size_t i = 0; i < kMovementStateCount; ++i) {
        CHECK(std::string(MovementStateName(static_cast<MovementState>(i))) != "Unknown");
    }
}

TEST_CASE("gait functions never index out of range") {
    const MovementConfig config;
    const auto corrupt = static_cast<Gait>(7);
    CHECK(GaitSpeed(config, corrupt) == doctest::Approx(8.0));
    CHECK(GaitAcceleration(config, corrupt) == doctest::Approx(4.0));
    GaitLimits broken;
    broken.maxGait = corrupt;
    CHECK(EffectiveSpeed(config, Gait::Run, broken) == doctest::Approx(5.5));
}

TEST_CASE("acceleration parameters are per gait and positive, braking is one value") {
    MovementConfig config;
    for (std::size_t i = 0; i < kGaitCount; ++i) {
        CHECK(GaitAcceleration(config, static_cast<Gait>(i)) > 0.0);
    }
    // Faster gaits accelerate at least as hard in the defaults (a tuning default, not a rule).
    CHECK(GaitAcceleration(config, Gait::Sprint) >= GaitAcceleration(config, Gait::Walk));
    // Time to reach the top speed from standing is finite and below 3 s at every gait (sanity of the defaults).
    for (std::size_t i = 0; i < kGaitCount; ++i) {
        const Gait gait = static_cast<Gait>(i);
        CAPTURE(GaitName(gait));
        CHECK(GaitSpeed(config, gait) / GaitAcceleration(config, gait) < 3.0);
    }
    // Braking from sprint speed takes the time v / a.
    CHECK(GaitSpeed(config, Gait::Sprint) / config.brakingDeceleration == doctest::Approx(1.0));
    config.gaits[2].acceleration = 1e-9;
    CHECK_FALSE(Validate(config).has_value()); // tiny but positive is valid: the rule is "positive", not "plausible"
}
