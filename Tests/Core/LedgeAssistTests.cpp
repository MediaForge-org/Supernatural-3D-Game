#include <doctest.h>

#include <cmath>
#include <limits>

#include <spn/core/LedgeAssist.h>

using namespace spn::core;

namespace {
const LedgeAssistConfig kCfg;
constexpr double kRadius = 0.42;

/// A clear, flat, deep ledge right in front (10 cm), approached head-on at `speed`.
LedgeProbe Ledge(double height, double speed = 3.5, double depth = 1.0) {
    LedgeProbe p;
    p.faceFound = true;
    p.faceDistance = 0.10;
    p.approachSpeed = speed;
    p.topFound = true;
    p.height = height;
    p.depth = depth;
    return p;
}

/// A thin wall with a free floor behind at the same level.
LedgeProbe Thin(double height, double speed, double depth = 0.30) {
    LedgeProbe p = Ledge(height, speed, depth);
    p.topClear = false; // too narrow to stand on
    p.landingFound = true;
    p.landingClear = true;
    return p;
}

LedgeMove MoveFor(const LedgeProbe& p) { return PlanLedgeAssist(kCfg, p, kRadius).move; }

/// Lowest horizontal speed along the path while it is still rising or crossing, by finite differences (m/s).
double MinPathSpeed(const LedgePlan& plan, double dt = 1.0 / 120.0) {
    double lowest = 1.0e9;
    LedgePathPoint prev = SampleLedgePath(plan, 0.0);
    for (double t = dt; t <= plan.TotalSeconds() + 1e-9; t += dt) {
        const LedgePathPoint cur = SampleLedgePath(plan, t);
        lowest = std::min(lowest, (cur.forward - prev.forward) / dt);
        prev = cur;
    }
    return lowest;
}
} // namespace

TEST_CASE("ledge assist: default config is valid, broken ones are rejected") {
    CHECK_FALSE(Validate(kCfg).has_value());
    LedgeAssistConfig c = kCfg;
    c.midMax = 0.3; // below lowMax
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.highMax = std::numeric_limits<double>::quiet_NaN();
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.reachStanding = 1.0; // above reachMax
    CHECK(Validate(c).has_value());
}

TEST_CASE("ledge assist: the developer's three height bands pick the move") {
    CHECK(MoveFor(Ledge(0.20)) == LedgeMove::None);    // walked up
    CHECK(MoveFor(Ledge(0.25)) == LedgeMove::None);
    CHECK(MoveFor(Ledge(0.30)) == LedgeMove::StepOn);
    CHECK(MoveFor(Ledge(0.40)) == LedgeMove::StepOn);
    CHECK(MoveFor(Ledge(0.45)) == LedgeMove::Mantle);  // deep: onto it
    CHECK(MoveFor(Ledge(1.20)) == LedgeMove::Mantle);
    CHECK(MoveFor(Ledge(1.21)) == LedgeMove::ClimbUp);
    CHECK(MoveFor(Ledge(1.80)) == LedgeMove::ClimbUp);
    CHECK(MoveFor(Ledge(1.81)) == LedgeMove::None);    // too high
    CHECK(MoveFor(Thin(0.90, 4.0)) == LedgeMove::Vault);
    CHECK(MoveFor(Thin(0.35, 0.0)) == LedgeMove::Vault); // the low step-over needs no run-up
    CHECK(MoveFor(Thin(0.90, 1.0)) == LedgeMove::None);  // a vault needs a run-up, and there is no room on top
    CHECK(MoveFor(Thin(1.50, 5.0)) == LedgeMove::None);  // nothing vaults the high band
}

TEST_CASE("ledge assist: explicit input only within reach, head-on, onto something solid") {
    LedgeProbe p = Ledge(0.90, 0.0);
    p.faceDistance = 0.29;
    CHECK(MoveFor(p) == LedgeMove::Mantle);
    p.faceDistance = 0.35; // standing reach is 0.30 m
    CHECK(MoveFor(p) == LedgeMove::None);
    p.approachSpeed = 3.5; // the reach grows with speed: 0.30 + 0.08 * 3.5 = 0.58 m
    CHECK(MoveFor(p) == LedgeMove::Mantle);
    p.faceDistance = 0.80; // the QA wall case (80 cm): ordinary jump
    CHECK(MoveFor(p) == LedgeMove::None);
    p.approachSpeed = 8.0; // even sprinting the reach is capped at 0.60 m
    CHECK(MoveFor(p) == LedgeMove::None);
    CHECK(LedgeReach(kCfg, 8.0) == doctest::Approx(0.60));

    p = Ledge(0.90);
    p.approachAngle = 50.0;
    CHECK(PlanLedgeAssist(kCfg, p, kRadius).move == LedgeMove::None);
    p.approachAngle = 40.0;
    CHECK(MoveFor(p) == LedgeMove::Mantle);

    p = Ledge(0.90);
    p.surfaceAllowed = false; // a pawn / moving object
    CHECK(MoveFor(p) == LedgeMove::None);
    p = Ledge(0.90);
    p.faceFound = false;
    CHECK(MoveFor(p) == LedgeMove::None);
    p = Ledge(0.90);
    p.topFound = false; // wall continues above the probe
    CHECK(MoveFor(p) == LedgeMove::None);
}

TEST_CASE("ledge assist: grip, slope, headroom and room on top are checked") {
    LedgeProbe p = Ledge(0.90);
    p.depth = 0.10;
    CHECK(MoveFor(p) == LedgeMove::None); // nothing to grip
    p = Ledge(0.90);
    p.topAngle = 50.0;
    CHECK(MoveFor(p) == LedgeMove::None);
    p = Ledge(0.90);
    p.liftClear = false;
    CHECK(MoveFor(p) == LedgeMove::None);
    p = Ledge(0.90);
    p.topClear = false; // a ceiling over the ledge, deep top
    CHECK(MoveFor(p) == LedgeMove::None);
    p = Ledge(1.50);
    p.depth = 0.30; // too narrow to stand on: no climb
    CHECK(MoveFor(p) == LedgeMove::None);

    LedgeProbe v = Thin(0.90, 4.0);
    v.landingRise = -1.5; // a drop behind
    CHECK(MoveFor(v) == LedgeMove::None);
    v = Thin(0.90, 4.0);
    v.landingClear = false;
    CHECK(MoveFor(v) == LedgeMove::None);
}

TEST_CASE("ledge assist: invalid measurements never start a move") {
    LedgeProbe p = Ledge(0.90);
    p.height = std::numeric_limits<double>::quiet_NaN();
    CHECK(MoveFor(p) == LedgeMove::None);
    p = Ledge(0.90);
    p.approachSpeed = std::numeric_limits<double>::infinity();
    CHECK(MoveFor(p) == LedgeMove::None);
    CHECK(PlanLedgeAssist(kCfg, Ledge(0.90), 0.0).move == LedgeMove::None);
}

TEST_CASE("ledge assist: the path rises, moves forward monotonically and ends where planned") {
    for (const double h : {0.30, 0.45, 0.90, 1.20, 1.50, 1.80}) {
        for (const double v : {0.0, 3.5, 5.5}) {
            const LedgePlan plan = PlanLedgeAssist(kCfg, Ledge(h, v), kRadius);
            REQUIRE(plan.move != LedgeMove::None);
            LedgePathPoint prev = SampleLedgePath(plan, 0.0);
            CHECK(prev.forward == doctest::Approx(0.0));
            CHECK(prev.up == doctest::Approx(0.0));
            for (double t = 0.005; t <= plan.TotalSeconds() + 0.05; t += 0.005) {
                const LedgePathPoint cur = SampleLedgePath(plan, t);
                CHECK(cur.forward >= prev.forward - 1e-12);
                CHECK(cur.up >= prev.up - 1e-12);
                // the upper body never enters the obstacle: axis distance to the face at least the required one
                CHECK(plan.faceAxisDistance - cur.forward >= LedgeRequiredAxisDistance(plan, cur.up) - 1e-9);
                prev = cur;
            }
            const LedgePathPoint end = SampleLedgePath(plan, plan.TotalSeconds());
            CHECK(end.done);
            CHECK(end.up == doctest::Approx(h + kCfg.clearance));
            CHECK(end.forward == doctest::Approx(plan.forwardEnd));
            // standing on top: the capsule's back is past the face
            CHECK(end.forward - plan.faceAxisDistance >= kRadius);
        }
    }
}

TEST_CASE("ledge assist: higher obstacles take longer and keep less speed") {
    const LedgePlan step = PlanLedgeAssist(kCfg, Ledge(0.35), kRadius);
    const LedgePlan low = PlanLedgeAssist(kCfg, Ledge(0.50), kRadius);
    const LedgePlan high = PlanLedgeAssist(kCfg, Ledge(1.10), kRadius);
    const LedgePlan climb = PlanLedgeAssist(kCfg, Ledge(1.60), kRadius);
    CHECK(step.TotalSeconds() < low.TotalSeconds());
    CHECK(low.TotalSeconds() < high.TotalSeconds());
    CHECK(high.TotalSeconds() < climb.TotalSeconds());
    CHECK(step.exitSpeed > low.exitSpeed);
    CHECK(low.exitSpeed > high.exitSpeed);
    CHECK(climb.exitSpeed == doctest::Approx(0.0));
    CHECK(climb.TotalSeconds() >= 1.0); // a pull-up is slow
    CHECK(step.TotalSeconds() <= 0.5);
    // from a standstill a mantle is slower than with a run-up
    CHECK(PlanLedgeAssist(kCfg, Ledge(0.90, 0.0), kRadius).TotalSeconds() > PlanLedgeAssist(kCfg, Ledge(0.90, 3.5), kRadius).TotalSeconds());
}

TEST_CASE("ledge assist: the two QA late-jump cases keep at least half their speed and get across within a second") {
    // SpnQA.Jump late_jump_ledge_45_jog and late_jump_ledge_40_run: pressed 10 cm before a 1 m deep block.
    const LedgePlan jog = PlanLedgeAssist(kCfg, Ledge(0.45, 3.5), kRadius);
    const LedgePlan run = PlanLedgeAssist(kCfg, Ledge(0.40, 5.5), kRadius);
    CHECK(jog.move == LedgeMove::Mantle);
    CHECK(run.move == LedgeMove::StepOn);
    CHECK(MinPathSpeed(jog) >= 0.5 * 3.5);
    CHECK(MinPathSpeed(run) >= 0.5 * 5.5);
    CHECK(jog.TotalSeconds() < 0.6);
    CHECK(run.TotalSeconds() < 0.6);
}

TEST_CASE("ledge assist: a high mantle and a climb stop the body at the face first") {
    const LedgePlan climb = PlanLedgeAssist(kCfg, Ledge(1.50, 3.5), kRadius);
    // early in the move the capsule front may not pass the face (the body is still below the edge)
    const LedgePathPoint early = SampleLedgePath(climb, 0.2 * climb.liftSeconds);
    CHECK(climb.faceAxisDistance - early.forward >= kRadius - 1e-9);
    const LedgePlan vault = PlanLedgeAssist(kCfg, Thin(0.90, 4.0), kRadius);
    CHECK(vault.move == LedgeMove::Vault);
    CHECK_FALSE(vault.endsOnTop);
    CHECK(vault.forwardEnd >= vault.faceAxisDistance + 0.30 + kRadius); // past the far side
    CHECK(vault.exitSpeed >= 0.6 * 4.0);
}

TEST_CASE("ledge assist: deterministic") {
    const LedgePlan a = PlanLedgeAssist(kCfg, Ledge(0.77, 2.9), kRadius);
    const LedgePlan b = PlanLedgeAssist(kCfg, Ledge(0.77, 2.9), kRadius);
    CHECK(a.move == b.move);
    CHECK(a.TotalSeconds() == b.TotalSeconds());
    CHECK(SampleLedgePath(a, 0.123).forward == SampleLedgePath(b, 0.123).forward);
    CHECK(std::string(LedgeMoveName(LedgeMove::ClimbUp)) == "ClimbUp");
}
