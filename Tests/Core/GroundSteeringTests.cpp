#include <doctest.h>

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

TEST_CASE("without input the character brakes along its heading and stops; input resumes it") {
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
