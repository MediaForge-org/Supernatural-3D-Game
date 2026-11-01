#include <doctest.h>

#include <algorithm>
#include <cmath>
#include <limits>

#include <spn/core/GroundSteering.h>
#include <spn/core/Movement.h>

using namespace spn::core;

namespace {

constexpr double kPi = 3.14159265358979323846;
const SteeringConfig kSteer;
const MovementConfig kMove;

struct Run {
    Vec2 v;
    double minSpeed;
    double timeToRegain; ///< seconds until 90 % of the target speed again, -1 when never
};

// Sprint at 8 m/s along +X, then hold input at `turnDegrees`; ResolveGait decides the target as the real component would.
Run SprintThenTurn(double turnDegrees, double seconds = 3.0) {
    const double dt = 1.0 / 60.0;
    Vec2 v{8.0, 0.0};
    const double rad = turnDegrees * kPi / 180.0;
    const Vec2 dir{std::cos(rad), std::sin(rad)};
    Run r{v, 8.0, -1.0};
    for (double t = 0.0; t < seconds; t += dt) {
        // Input relative to the facing, which follows the velocity heading in this model.
        const double heading = Speed(v) > 0.01 ? std::atan2(v.y, v.x) : 0.0;
        const double rel = std::atan2(dir.y, dir.x) - heading;
        const MoveIntent intent{std::cos(rel), std::sin(rel)};
        const Gait g = ResolveGait(kMove, Gait::Jog, true, GaitLimits{}, intent, Speed(v));
        SteeringInput in{dir, GaitSpeed(kMove, g), GaitAcceleration(kMove, g), kMove.brakingDeceleration};
        v = SteerVelocity(kSteer, v, in, dt);
        r.minSpeed = std::min(r.minSpeed, Speed(v));
        if (r.timeToRegain < 0.0 && Speed(v) >= 0.9 * 8.0 && t > 0.05 && turnDegrees != 0.0) r.timeToRegain = t;
    }
    r.v = v;
    return r;
}

} // namespace

TEST_CASE("steering config validation") {
    CHECK_FALSE(Validate(kSteer).has_value());
    SteeringConfig c = kSteer;
    c.minHeadingRateDegrees = 1000.0;
    CHECK(Validate(c).has_value());
    c = kSteer;
    c.speedLossPerRadian = 0.9;
    CHECK(Validate(c).has_value());
    c = kSteer;
    c.fullSpeed = 0.0;
    CHECK(Validate(c).has_value());
    c = kSteer;
    c.maxHeadingRateDegrees = std::numeric_limits<double>::infinity();
    CHECK(Validate(c).has_value());
}

TEST_CASE("sprinting straight keeps the sprint speed") {
    const Run r = SprintThenTurn(0.0);
    CHECK(Speed(r.v) == doctest::Approx(8.0).epsilon(0.001));
    CHECK(r.minSpeed == doctest::Approx(8.0).epsilon(0.001));
    CHECK(r.v.y == doctest::Approx(0.0).epsilon(1e-6));
}

TEST_CASE("turns cost speed in proportion to the angle, not more than the model says, and the sprint is regained") {
    const Run r45 = SprintThenTurn(45.0);
    const Run r90 = SprintThenTurn(90.0);
    const Run r180 = SprintThenTurn(180.0);
    // 45: a mild loss; 90: well above the 462 cm/s measured with the engine's friction blend; 180: the most.
    CHECK(r45.minSpeed > 6.5);
    CHECK(r90.minSpeed > 5.5);
    CHECK(r90.minSpeed < r45.minSpeed);
    CHECK(r180.minSpeed < r90.minSpeed);
    CHECK(r180.minSpeed > 3.0);
    // The sprint is kept through the turn (not dropped to Jog) and rebuilt afterwards.
    for (const Run* r : {&r45, &r90, &r180}) {
        CHECK(Speed(r->v) == doctest::Approx(8.0).epsilon(0.01));
        CHECK(r->timeToRegain > 0.0);
        CHECK(r->timeToRegain < 2.0);
    }
    // The final heading points along the new input.
    CHECK(r90.v.y > 7.9);
    CHECK(std::abs(r90.v.x) < 0.1);
}

TEST_CASE("without input the character brakes along its heading and stops - input resumes it") {
    const double dt = 1.0 / 60.0;
    Vec2 v{6.0, 8.0};
    const SteeringInput none{{0.0, 0.0}, 8.0, 4.0, 8.0};
    double t = 0.0;
    while (Speed(v) > 0.0 && t < 5.0) {
        const Vec2 next = SteerVelocity(kSteer, v, none, dt);
        CHECK(Speed(next) <= Speed(v));
        if (Speed(next) > 0.0) CHECK(next.x / Speed(next) == doctest::Approx(0.6).epsilon(1e-6)); // heading kept
        v = next;
        t += dt;
    }
    CHECK(Speed(v) == 0.0);
    CHECK(t == doctest::Approx(10.0 / 8.0).epsilon(0.05)); // 10 m/s at 8 m/s^2
    // Resume from rest: starts straight along the input, accelerating at the gait's rate.
    const SteeringInput go{{0.0, 1.0}, 8.0, 4.0, 8.0};
    v = SteerVelocity(kSteer, v, go, 0.5);
    CHECK(v.x == doctest::Approx(0.0).epsilon(1e-9));
    CHECK(v.y == doctest::Approx(2.0).epsilon(1e-9));
}

TEST_CASE("steering never produces non-finite or growing speeds") {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const Vec2 v{3.0, 4.0};
    const Vec2 same = SteerVelocity(kSteer, v, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, nan);
    CHECK(same.x == 3.0);
    CHECK(same.y == 4.0);
    CHECK(SteerVelocity(kSteer, v, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, -1.0).x == 3.0);
    const Vec2 bad = SteerVelocity(kSteer, v, SteeringInput{{nan, 0.0}, nan, nan, nan}, 0.1);
    CHECK(std::isfinite(bad.x));
    CHECK(Speed(bad) <= 5.0);
    // Above the target speed the speed falls by the braking rate.
    const Vec2 slow = SteerVelocity(kSteer, {8.0, 0.0}, SteeringInput{{1.0, 0.0}, 3.5, 3.0, 8.0}, 0.1);
    CHECK(Speed(slow) == doctest::Approx(7.2));
}

TEST_CASE("speed loss grows monotonically with the turn angle: 45, 90, 135, 180 and the arc is wider at sprint than at walk") {
    double last = 8.0;
    for (const double angle : {0.0, 45.0, 90.0, 135.0, 180.0}) {
        const Run r = SprintThenTurn(angle);
        CHECK(r.minSpeed <= last + 1e-9);
        CHECK(r.minSpeed > 2.5);                          // a turn is never a stop
        CHECK(Speed(r.v) == doctest::Approx(8.0).epsilon(0.01));
        last = r.minSpeed;
    }
    CHECK(SprintThenTurn(135.0).minSpeed > SprintThenTurn(180.0).minSpeed);
    // The heading rate shrinks with speed: a walker turns 90 degrees faster than a sprinter.
    const auto secondsToTurn90 = [](double speed) {
        Vec2 v{speed, 0.0};
        for (int i = 1; i <= 600; ++i) {
            v = SteerVelocity(kSteer, v, SteeringInput{{0.0, 1.0}, speed, 1000.0, 1000.0}, 1.0 / 120.0);
            if (v.y > 0.99 * Speed(v)) return i / 120.0;
        }
        return -1.0;
    };
    const double walk = secondsToTurn90(1.8);
    const double sprint = secondsToTurn90(8.0);
    CHECK(walk > 0.0);
    CHECK(sprint > walk);
    CHECK(sprint < 0.5);
}

TEST_CASE("standing start and sprint abort follow the gait rates") {
    const double dt = 1.0 / 60.0;
    Vec2 v{0.0, 0.0};
    double t = 0.0;
    while (Speed(v) < 8.0 - 1e-9 && t < 10.0) {
        v = SteerVelocity(kSteer, v, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, dt);
        t += dt;
    }
    CHECK(t == doctest::Approx(2.0).epsilon(0.02));       // 8 m/s at 4 m/s^2
    // Sprint abort: input released at sprint speed stops in speed / braking seconds along the same heading.
    double stop = 0.0;
    while (Speed(v) > 0.0 && stop < 10.0) {
        v = SteerVelocity(kSteer, v, SteeringInput{{0.0, 0.0}, 0.0, 4.0, 8.0}, dt);
        stop += dt;
    }
    CHECK(stop == doctest::Approx(1.0).epsilon(0.03));
    CHECK(v.y == 0.0);
    // Input held but the gait drops to Jog: the speed falls to 3.5 at the braking rate, not instantly.
    Vec2 w{8.0, 0.0};
    w = SteerVelocity(kSteer, w, SteeringInput{{1.0, 0.0}, 3.5, 3.0, 8.0}, dt);
    CHECK(Speed(w) == doctest::Approx(8.0 - 8.0 * dt));
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Explanation of the sprint-turn measurement of the Windows run (M2A2-Windows-Integration-Report.md section 4: 800 -> 461..464 cm/s,
// back at 95 % after about 1.02 s). This is a MODEL of Unreal's UCharacterMovementComponent::CalcVelocity as written down from the engine
// source (friction blend of the velocity towards "input direction * current speed", constant braking above the gait's top speed, rotation
// to the input at a constant rate). It is not run against the engine; the Unreal side stays "pending" until the QA is repeated.

namespace {

struct EngineTurn {
    double minSpeed = 0.0;
    double timeTo95 = -1.0;
    double secondsWithoutSprint = 0.0;
};

// 8 m/s along +X, input switched to `side` * 90 degrees. `holdSprint` = the sprint is never dropped (isolates the friction effect).
EngineTurn EngineFrictionTurn(double side, bool holdSprint, double dt = 1.0 / 120.0) {
    constexpr double kTurnFriction = 8.0;     // FSpnMovementSettings::TurnFriction (GroundFriction)
    constexpr double kRotationRate = 500.0;   // deg/s, FSpnMovementSettings::TurnRateDegrees
    Vec2 v{8.0, 0.0};
    double yaw = 0.0;                          // degrees, the capsule orientation (bOrientRotationToMovement)
    const double inputDeg = side * 90.0;
    const Vec2 dir{std::cos(inputDeg * kPi / 180.0), std::sin(inputDeg * kPi / 180.0)};
    EngineTurn r{8.0, -1.0, 0.0};
    for (double t = 0.0; t < 2.0; t += dt) {
        const double rel = (inputDeg - yaw) * kPi / 180.0;   // input in the capsule's frame, as GetLocalInput() computes it
        const Gait g = holdSprint ? Gait::Sprint
                                  : ResolveGait(kMove, Gait::Jog, true, GaitLimits{}, MoveIntent{std::cos(rel), std::sin(rel)}, Speed(v));
        if (g != Gait::Sprint) r.secondsWithoutSprint += dt;
        const double top = GaitSpeed(kMove, g);
        const double speed = Speed(v);
        const bool over = speed > top * 1.01;
        if (over) {
            const double s = std::max(speed - kMove.brakingDeceleration * dt, top);   // braking never goes below the limit
            v = {v.x / speed * s, v.y / speed * s};
        } else {
            const double k = std::min(dt * kTurnFriction, 1.0);
            v = {v.x - (v.x - dir.x * speed) * k, v.y - (v.y - dir.y * speed) * k};
        }
        v = {v.x + dir.x * GaitAcceleration(kMove, g) * dt, v.y + dir.y * GaitAcceleration(kMove, g) * dt};
        if (!over && Speed(v) > top) {
            const double s = Speed(v);
            v = {v.x / s * top, v.y / s * top};
        }
        yaw += std::clamp(inputDeg - yaw, -kRotationRate * dt, kRotationRate * dt);
        r.minSpeed = std::min(r.minSpeed, Speed(v));
        if (r.timeTo95 < 0.0 && t > 0.2 && Speed(v) >= 0.95 * 8.0) r.timeTo95 = t;
    }
    return r;
}

} // namespace

TEST_CASE("the engine's friction blend alone explains the measured sprint-turn speed dip - losing the sprint gait is a second-order effect") {
    const EngineTurn real = EngineFrictionTurn(+1.0, false);
    const EngineTurn kept = EngineFrictionTurn(+1.0, true);
    // Measured on Windows: minimum 461..464 of 800 (57.6..58 %), back at 95 % after about 1.02 s.
    CHECK(real.minSpeed / 8.0 > 0.55);
    CHECK(real.minSpeed / 8.0 < 0.62);
    CHECK(real.timeTo95 > 0.9);
    CHECK(real.timeTo95 < 1.15);
    // Even with the sprint held the whole time the dip is nearly as deep: the cause is the friction formula, not the gait switch.
    CHECK(kept.minSpeed / 8.0 > 0.55);
    // Direction of the comparison: dropping the sprint can only LOWER the minimum (the gait limit falls, the model brakes at 8 m/s^2 above it), so the
    // real run may not end up above the held-sprint run. The cost is the ~56 ms without sprint times the braking rate: measured 0.19..0.205 m/s,
    // stable for time steps from 1/60 to 1/1000 s (about 6 % of the 3.4 m/s dip), and the real run (4.63) matches the Windows measurement
    // (4.61..4.64) while the held-sprint run (4.84) does not. The upper bound below stays at 0.4 m/s; a lower bound proves the gait loss is not free.
    CHECK(real.minSpeed - kept.minSpeed < 0.05);
    CHECK(kept.minSpeed - real.minSpeed > 0.1);
    CHECK(kept.minSpeed - real.minSpeed < 0.4);
    // The model is mirror symmetric: the left/right difference seen in Unreal (sprint ends only to the left) is NOT explained by it.
    const EngineTurn left = EngineFrictionTurn(-1.0, false);
    CHECK(left.minSpeed == doctest::Approx(real.minSpeed).epsilon(1e-9));
    CHECK(left.secondsWithoutSprint == doctest::Approx(real.secondsWithoutSprint).epsilon(1e-9));
    // The sprint gait is only dropped briefly (the input angle falls below the 50 degree start angle as the capsule turns at 500 deg/s).
    // This is also why SpnQA.Turns logged a 'local input angle' of 41..44 degrees: it samples the angle in the ACTOR frame 30..100 ms after the
    // key press, when the capsule has already turned by up to 50 degrees. The input itself was a real 90 degrees.
    CHECK(real.secondsWithoutSprint < 0.15);
}

// --- NightBuild02: frame-rate behaviour and extreme inputs (measured first, then pinned; no production change was needed) ---

namespace {

struct TurnOutcome {
    double minSpeed;
    double timeToHeading;
};

TurnOutcome TurnAtFrameRate(double turnDegrees, double fps) {
    const double dt = 1.0 / fps;
    const double rad = turnDegrees * kPi / 180.0;
    Vec2 v{8.0, 0.0};
    SteeringInput in{{std::cos(rad), std::sin(rad)}, 8.0, 4.0, 8.0};
    TurnOutcome out{8.0, -1.0};
    double t = 0.0;
    for (int i = 0; i < static_cast<int>(2.0 * fps); ++i) {
        v = SteerVelocity(kSteer, v, in, dt);
        t += dt;
        out.minSpeed = std::min(out.minSpeed, Speed(v));
        if (out.timeToHeading < 0.0 && std::abs(std::atan2(v.y, v.x) - rad) < 1e-6) out.timeToHeading = t;
    }
    return out;
}

} // namespace

TEST_CASE("sprint turns give the same speed dip and turn time at 20 to 240 fps within frame quantisation") {
    for (const double turn : {45.0, 90.0, 135.0}) {
        const TurnOutcome reference = TurnAtFrameRate(turn, 1000.0);
        for (const double fps : {20.0, 30.0, 60.0, 90.0, 144.0, 240.0}) {
            const TurnOutcome o = TurnAtFrameRate(turn, fps);
            INFO("turn " << turn << " deg at " << fps << " fps");
            CHECK(std::abs(o.minSpeed - reference.minSpeed) <= 0.03 * reference.minSpeed); // measured: below 1.5 %
            REQUIRE(o.timeToHeading > 0.0);
            CHECK(std::abs(o.timeToHeading - reference.timeToHeading) <= 1.0 / fps + 0.01); // one frame of quantisation
        }
    }
}

TEST_CASE("steering survives extreme time steps, speeds and directions without non-finite output or speed growth") {
    const double huge = 1.0e300;
    const double inf = std::numeric_limits<double>::infinity();
    const double nan = std::numeric_limits<double>::quiet_NaN();
    // a one-hour frame must neither overshoot the target nor flip the heading wildly
    const Vec2 afterHour = SteerVelocity(kSteer, {0.0, 0.0}, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, 3600.0);
    CHECK(Speed(afterHour) == doctest::Approx(8.0));
    CHECK(afterHour.y == doctest::Approx(0.0).epsilon(1e-9));
    // enormous but finite directions and velocities are normalised / kept finite
    const Vec2 a = SteerVelocity(kSteer, {1.0, 0.0}, SteeringInput{{huge, huge}, 8.0, 4.0, 8.0}, 0.016);
    CHECK(std::isfinite(a.x));
    CHECK(std::isfinite(a.y));
    CHECK(Speed(a) <= 8.0 + 1e-9);
    const Vec2 b = SteerVelocity(kSteer, {1.0e200, 1.0e200}, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, 0.016);
    CHECK(std::isfinite(b.x));
    CHECK(std::isfinite(b.y));
    CHECK(Speed(b) <= Speed(Vec2{1.0e200, 1.0e200}));
    // denormal direction counts as no input; non-finite everything is ignored
    CHECK(SteerVelocity(kSteer, {3.0, 0.0}, SteeringInput{{1e-320, 0.0}, 8.0, 4.0, 8.0}, 0.1).x < 3.0); // braking
    const Vec2 keep{2.0, 1.0};
    CHECK(SteerVelocity(kSteer, keep, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, nan) == keep);
    CHECK(SteerVelocity(kSteer, keep, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, inf) == keep);
    CHECK(SteerVelocity(kSteer, keep, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, -1.0) == keep);
    CHECK(SteerVelocity(kSteer, {nan, 0.0}, SteeringInput{{1.0, 0.0}, 8.0, 4.0, 8.0}, 0.1).y == 0.0);
    const Vec2 c = SteerVelocity(kSteer, keep, SteeringInput{{nan, inf}, nan, inf, -inf}, 0.1);
    CHECK(std::isfinite(c.x));
    CHECK(std::isfinite(c.y));
    CHECK(Speed(c) <= Speed(keep) + 1e-12);
}

TEST_CASE("steering fuzz: 200000 random steps never leave the finite range or exceed max(previous speed, target) + acceleration * dt") {
    std::uint64_t state = 0x9E3779B97F4A7C15ULL;
    const auto next = [&] {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        return static_cast<double>(state >> 11) / 9007199254740992.0; // [0, 1)
    };
    Vec2 v{0.0, 0.0};
    for (int i = 0; i < 200000; ++i) {
        const double dt = next() < 0.05 ? next() * 5.0 : next() * 0.05;
        SteeringInput in{{(next() - 0.5) * (next() < 0.1 ? 0.0 : 4.0), (next() - 0.5) * 4.0}, next() * 10.0, next() * 8.0, next() * 16.0};
        const double before = Speed(v);
        v = SteerVelocity(kSteer, v, in, dt);
        REQUIRE(std::isfinite(v.x));
        REQUIRE(std::isfinite(v.y));
        REQUIRE(Speed(v) <= std::max(before, in.targetSpeed) + 1e-9);
    }
}
