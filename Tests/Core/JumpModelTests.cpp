#include <doctest.h>

#include <cmath>
#include <limits>

#include <spn/core/JumpModel.h>
#include <spn/core/Movement.h>

using namespace spn::core;

namespace {
constexpr double kG = 9.81;
const double kV0 = JumpVelocityForHeight(0.45, kG); // the 45 cm jump of the default config
JumpAttempt Attempt(double speed, double distance, double ledge, double gravity = kG, double v0 = kV0) {
    return JumpAttempt{v0, gravity, speed, distance, ledge};
}
} // namespace

TEST_CASE("jump height over time is ballistic and returns to zero") {
    CHECK(JumpHeightAt(kV0, kG, 0.0) == doctest::Approx(0.0));
    CHECK(JumpHeightAt(kV0, kG, kV0 / kG) == doctest::Approx(0.45).epsilon(1e-6)); // the apex
    CHECK(JumpHeightAt(kV0, kG, 2.0 * kV0 / kG) == doctest::Approx(0.0).epsilon(1e-6));
    CHECK(JumpHeightAt(kV0, kG, std::numeric_limits<double>::quiet_NaN()) == 0.0);
}

TEST_CASE("the take-off window for the map's ledge heights at run and sprint speed") {
    // 30 cm ledge: feet are 30 cm up after 0.128 s, so a 5.5 m/s take-off closer than 0.70 m hits the face while rising.
    const JumpWindow w30 = ComputeJumpWindow(kV0, kG, 0.30, 5.5);
    CHECK(w30.reachable);
    CHECK(w30.riseTime == doctest::Approx(0.1281).epsilon(0.002));
    CHECK(w30.minDistance == doctest::Approx(0.705).epsilon(0.005));
    CHECK(w30.maxDistance == doctest::Approx(2.63).epsilon(0.01));
    const JumpWindow w40 = ComputeJumpWindow(kV0, kG, 0.40, 5.5);
    CHECK(w40.minDistance == doctest::Approx(1.11).epsilon(0.01));
    CHECK(w40.maxDistance - w40.minDistance < w30.maxDistance - w30.minDistance); // higher ledge: narrower window
    // Sprint needs more run-off distance for the same ledge.
    CHECK(ComputeJumpWindow(kV0, kG, 0.30, 8.0).minDistance == doctest::Approx(1.025).epsilon(0.005));
    // A ledge at the apex or above is never reachable.
    CHECK_FALSE(ComputeJumpWindow(kV0, kG, 0.45, 5.5).reachable);
    CHECK_FALSE(ComputeJumpWindow(kV0, kG, 0.60, 5.5).reachable);
}

TEST_CASE("outcome classification across the window") {
    CHECK(EvaluateJump(Attempt(5.5, 0.3, 0.30)) == JumpOutcome::BlockedRising);
    CHECK(EvaluateJump(Attempt(5.5, 0.7, 0.30)) == JumpOutcome::BlockedRising);
    CHECK(EvaluateJump(Attempt(5.5, 0.72, 0.30)) == JumpOutcome::Clears);
    CHECK(EvaluateJump(Attempt(5.5, 2.0, 0.30)) == JumpOutcome::Clears);
    CHECK(EvaluateJump(Attempt(5.5, 2.7, 0.30)) == JumpOutcome::FallsShort);
    CHECK(EvaluateJump(Attempt(5.5, 1.0, 0.45)) == JumpOutcome::TooHigh);
    CHECK(EvaluateJump(Attempt(5.5, 1.0, 0.60)) == JumpOutcome::TooHigh);
    // Boundaries of the window itself.
    const JumpWindow w = ComputeJumpWindow(kV0, kG, 0.30, 5.5);
    CHECK(EvaluateJump(Attempt(5.5, w.minDistance, 0.30)) == JumpOutcome::Clears);
    CHECK(EvaluateJump(Attempt(5.5, w.maxDistance, 0.30)) == JumpOutcome::Clears);
    CHECK(EvaluateJump(Attempt(5.5, w.maxDistance + 0.01, 0.30)) == JumpOutcome::FallsShort);
}

TEST_CASE("the blocked-rising case is the vertical climb the player sees") {
    // Take off 20 cm before a 30 cm ledge at run speed: the face is hit after 0.036 s, the feet pass the ledge top 0.092 s later.
    const JumpAttempt a = Attempt(5.5, 0.2, 0.30);
    CHECK(EvaluateJump(a) == JumpOutcome::BlockedRising);
    CHECK(VerticalClimbSeconds(a) == doctest::Approx(0.128 - 0.0364).epsilon(0.01));
    CHECK(VerticalClimbSeconds(Attempt(5.5, 2.0, 0.30)) == 0.0);       // clears: no vertical phase
    CHECK(VerticalClimbSeconds(Attempt(5.5, 1.0, 0.60)) == 0.0);       // too high: not a climb either
    // The vertical phase grows with the ledge height and shrinks with the take-off distance.
    CHECK(VerticalClimbSeconds(Attempt(5.5, 0.2, 0.40)) > VerticalClimbSeconds(Attempt(5.5, 0.2, 0.30)));
    CHECK(VerticalClimbSeconds(Attempt(5.5, 0.5, 0.30)) < VerticalClimbSeconds(Attempt(5.5, 0.2, 0.30)));
}

TEST_CASE("a stronger gravity at the same apex shortens the dead distance in front of a ledge") {
    // Same 45 cm apex with gravity x2: v0 = sqrt(2 g H). The minimum take-off distance falls by sqrt(2).
    const double g2 = 2.0 * kG;
    const double v2 = JumpVelocityForHeight(0.45, g2);
    const JumpWindow normal = ComputeJumpWindow(kV0, kG, 0.30, 5.5);
    const JumpWindow heavy = ComputeJumpWindow(v2, g2, 0.30, 5.5);
    CHECK(heavy.minDistance < normal.minDistance);
    CHECK(heavy.minDistance == doctest::Approx(normal.minDistance / std::sqrt(2.0)).epsilon(0.001));
    CHECK(JumpApexHeight(v2, g2) == doctest::Approx(0.45).epsilon(1e-9)); // the apex itself is unchanged
}

TEST_CASE("jump model rejects nonsense input without crashing") {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    CHECK_FALSE(ComputeJumpWindow(0.0, kG, 0.3, 5.5).reachable);
    CHECK_FALSE(ComputeJumpWindow(kV0, 0.0, 0.3, 5.5).reachable);
    CHECK_FALSE(ComputeJumpWindow(kV0, kG, 0.3, 0.0).reachable);   // standing jump: no horizontal travel to time
    CHECK_FALSE(ComputeJumpWindow(kV0, kG, -0.1, 5.5).reachable);
    CHECK_FALSE(ComputeJumpWindow(kV0, kG, nan, 5.5).reachable);
    CHECK_FALSE(ComputeJumpWindow(inf, kG, 0.3, 5.5).reachable);
    CHECK(EvaluateJump(Attempt(5.5, nan, 0.30)) == JumpOutcome::BlockedRising);
    CHECK(EvaluateJump(Attempt(5.5, -1.0, 0.30)) == JumpOutcome::BlockedRising);
    CHECK(std::string(JumpOutcomeName(JumpOutcome::Clears)) == "Clears");
}

TEST_CASE("face contact: engine-style response vs ledge assist for a 30 cm ledge at run speed") {
    // Take off 0.2 m before the face (the window starts at 0.70 m): BlockedRising.
    FaceJumpParams p;
    p.attempt = Attempt(5.5, 0.2, 0.30);
    const FaceJumpResult engine = SimulateFaceJump(p);
    CHECK(engine.outcome == JumpOutcome::BlockedRising);
    CHECK(engine.timeFeetAboveLedge == doctest::Approx(0.128).epsilon(0.01));
    CHECK(engine.airTimeLeft == doctest::Approx(0.3495).epsilon(0.01));    // from 0.128 s to 0.4776 s
    // Engine-style: 0.5 * 1.2 * 0.35^2 = 0.073 m of forward travel while 0.30 m are needed: the character does not reach the ledge.
    CHECK(engine.travelAchieved == doctest::Approx(0.0733).epsilon(0.02));
    CHECK_FALSE(engine.onLedge);
    // Assist: the take-off speed is kept, 5.5 m/s * 0.35 s = 1.9 m of travel: the ledge is reached with a wide margin.
    p.keepSpeed = true;
    const FaceJumpResult assist = SimulateFaceJump(p);
    CHECK(assist.onLedge);
    CHECK(assist.travelAchieved > 10.0 * engine.travelAchieved);
}

TEST_CASE("face contact: more air control helps the engine-style response, a lower ledge needs less") {
    FaceJumpParams p;
    p.attempt = Attempt(5.5, 0.2, 0.30);
    p.airAcceleration = 3.5;                        // air control 1.0
    CHECK(SimulateFaceJump(p).travelAchieved == doctest::Approx(0.5 * 3.5 * 0.3495 * 0.3495).epsilon(0.02));
    p.attempt = Attempt(5.5, 0.2, 0.15);            // a 15 cm ledge: much more air time above it
    const FaceJumpResult low = SimulateFaceJump(p);
    CHECK(low.airTimeLeft > 0.45);
    p.airAcceleration = 1.2;
    CHECK(SimulateFaceJump(p).travelAchieved > 0.1);
    // Higher ledge near the apex: very little air time left, the engine-style response cannot make it.
    p.attempt = Attempt(5.5, 0.2, 0.44);
    CHECK_FALSE(SimulateFaceJump(p).onLedge);
}

TEST_CASE("face contact: results for non-contact outcomes and bad input") {
    FaceJumpParams p;
    p.attempt = Attempt(5.5, 1.5, 0.30);
    const FaceJumpResult clears = SimulateFaceJump(p);
    CHECK(clears.outcome == JumpOutcome::Clears);
    CHECK(clears.onLedge);
    CHECK(clears.travelAchieved == 0.0);
    p.attempt = Attempt(5.5, 1.0, 0.60);
    CHECK(SimulateFaceJump(p).outcome == JumpOutcome::TooHigh);
    CHECK_FALSE(SimulateFaceJump(p).onLedge);
    p.attempt = Attempt(5.5, 3.5, 0.30);
    CHECK(SimulateFaceJump(p).outcome == JumpOutcome::FallsShort);
    CHECK_FALSE(SimulateFaceJump(p).onLedge);
    p.attempt = Attempt(5.5, 0.2, 0.30);
    p.airAcceleration = 0.0;                        // no air control and no assist: nothing moves the character forward
    CHECK_FALSE(SimulateFaceJump(p).onLedge);
    p.keepSpeed = true;
    CHECK(SimulateFaceJump(p).onLedge);
    p.attempt.speed = 0.0;                          // standing jump: no horizontal model
    CHECK(SimulateFaceJump(p).outcome == JumpOutcome::TooHigh);
}

TEST_CASE("jump window properties hold over a grid of speeds, ledge heights and gravities") {
    for (double g : {9.81, 14.7, 19.62}) {
        const double v0 = JumpVelocityForHeight(0.45, g);
        for (double speed : {1.8, 3.5, 5.5, 8.0}) {
            double lastMin = -1.0;
            for (double h = 0.05; h < 0.449; h += 0.025) {
                const JumpWindow w = ComputeJumpWindow(v0, g, h, speed);
                REQUIRE(w.reachable);
                CHECK(w.riseTime > 0.0);
                CHECK(w.fallTime > w.riseTime);
                CHECK(w.minDistance >= lastMin);                     // a higher ledge needs a longer run-off
                lastMin = w.minDistance;
                CHECK(JumpHeightAt(v0, g, w.riseTime) == doctest::Approx(h).epsilon(1e-6));
                CHECK(JumpHeightAt(v0, g, w.fallTime) == doctest::Approx(h).epsilon(1e-6));
                CHECK(w.maxDistance / w.minDistance == doctest::Approx(w.fallTime / w.riseTime).epsilon(1e-9));
                // every distance inside the window clears, just outside does not
                CHECK(EvaluateJump(JumpAttempt{v0, g, speed, 0.5 * (w.minDistance + w.maxDistance), h}) == JumpOutcome::Clears);
                CHECK(EvaluateJump(JumpAttempt{v0, g, speed, w.minDistance * 0.99, h}) == JumpOutcome::BlockedRising);
                CHECK(EvaluateJump(JumpAttempt{v0, g, speed, w.maxDistance * 1.01, h}) == JumpOutcome::FallsShort);
            }
        }
    }
}

TEST_CASE("face contact: how much forward travel the ledge needs decides whether the engine-style response gets on top (sensitivity)") {
    // The model cannot yet say what the engine does: the result flips with the travel the capsule needs after clearing the ledge top.
    FaceJumpParams p;
    p.attempt = Attempt(5.5, 0.2, 0.30);
    const double achieved = SimulateFaceJump(p).travelAchieved;     // about 0.073 m with air control 0.35
    p.overhang = achieved * 0.9;
    CHECK(SimulateFaceJump(p).onLedge);                              // a perch-like threshold: the jump "snaps" onto the ledge
    p.overhang = achieved * 1.1;
    CHECK_FALSE(SimulateFaceJump(p).onLedge);                        // a full capsule radius (0.30 m): it falls back
    p.overhang = 0.42;                                               // the capsule radius
    CHECK_FALSE(SimulateFaceJump(p).onLedge);
}
