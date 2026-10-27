// Randomised invariant tests (fixed seeds, SplitMix64: identical on every platform) for the movement-related core modules. They check
// properties that must hold for ANY input, including NaN/infinity injections, instead of fixed expected numbers.

#include <doctest.h>

#include <cmath>
#include <limits>

#include <spn/core/CameraArm.h>
#include <spn/core/DeterministicRandom.h>
#include <spn/core/GroundSteering.h>
#include <spn/core/InteractionReach.h>
#include <spn/core/Locomotion.h>
#include <spn/core/Movement.h>
#include <spn/core/Npc.h>
#include <spn/core/Traversal.h>

using namespace spn::core;

namespace {

constexpr double kPi = 3.14159265358979323846;

struct Rng {
    SplitMix64 g;
    explicit Rng(std::uint64_t seed) : g(seed) {}
    double Unit() { return static_cast<double>(g.Next() >> 11) * (1.0 / 9007199254740992.0); }
    double Range(double lo, double hi) { return lo + (hi - lo) * Unit(); }
    bool Chance(double p) { return Unit() < p; }
    unsigned Below(unsigned n) { return static_cast<unsigned>(g.NextBelow(n)); }
    // Mostly sane values, sometimes extreme or non-finite ones.
    double Wild(double lo, double hi) {
        const unsigned r = Below(40);
        if (r == 0) return std::numeric_limits<double>::quiet_NaN();
        if (r == 1) return std::numeric_limits<double>::infinity();
        if (r == 2) return -std::numeric_limits<double>::infinity();
        if (r == 3) return 1e12;
        if (r == 4) return -1e12;
        return Range(lo, hi);
    }
};

double Angle(Vec2 v) { return std::atan2(v.y, v.x); }

double AngleBetween(Vec2 a, Vec2 b) {
    double d = std::fmod(Angle(b) - Angle(a) + 3.0 * kPi, 2.0 * kPi) - kPi;
    return std::abs(d);
}

} // namespace

TEST_CASE("property: SteerVelocity is finite, never speeds up beyond target or acceleration, and turns no faster than allowed") {
    Rng rng(0x5EED0001);
    const SteeringConfig cfg;
    for (int i = 0; i < 20000; ++i) {
        const Vec2 v{rng.Range(-9.0, 9.0), rng.Range(-9.0, 9.0)};
        const SteeringInput in{{rng.Range(-1.0, 1.0), rng.Range(-1.0, 1.0)}, rng.Range(0.0, 8.0), rng.Range(0.1, 6.0), rng.Range(0.1, 12.0)};
        const double dt = rng.Range(0.0, 0.1);
        const Vec2 out = SteerVelocity(cfg, v, in, dt);
        REQUIRE(std::isfinite(out.x));
        REQUIRE(std::isfinite(out.y));
        const double s0 = Speed(v), s1 = Speed(out);
        // The speed can rise only through acceleration, never above the target (unless it was already above it) and never from the turn.
        CHECK(s1 <= std::max(s0, in.targetSpeed) + 1e-9);
        CHECK(s1 <= s0 + in.acceleration * dt + 1e-9);
        CHECK(s1 >= 0.0);
        const double len = Speed(in.direction);
        if (len < 1e-9) {
            CHECK(s1 <= s0 + 1e-9);                        // no input: only braking
        } else if (s0 > 1e-6 && s1 > 1e-6) {
            const double maxTurn = cfg.maxHeadingRateDegrees * kPi / 180.0 * dt + 1e-9;
            CHECK(AngleBetween(v, out) <= maxTurn + 1e-6);   // heading change bounded by the fastest allowed rate
        }
    }
}

TEST_CASE("property: SteerVelocity survives NaN / infinity in any argument") {
    Rng rng(0x5EED0002);
    const SteeringConfig cfg;
    for (int i = 0; i < 20000; ++i) {
        const Vec2 v{rng.Wild(-9.0, 9.0), rng.Wild(-9.0, 9.0)};
        const SteeringInput in{{rng.Wild(-1.0, 1.0), rng.Wild(-1.0, 1.0)}, rng.Wild(0.0, 8.0), rng.Wild(0.0, 6.0), rng.Wild(0.0, 12.0)};
        const Vec2 out = SteerVelocity(cfg, v, in, rng.Wild(0.0, 0.1));
        if (std::isfinite(v.x) && std::isfinite(v.y)) {
            CHECK(std::isfinite(out.x));
            CHECK(std::isfinite(out.y));
        }
    }
}

TEST_CASE("property: the traversal state machine only moves forward and the flags stay consistent") {
    Rng rng(0x5EED0003);
    const TraversalTimings t;
    for (int run = 0; run < 400; ++run) {
        TraversalState s;
        for (int step = 0; step < 60; ++step) {
            const TraversalState before = s;
            switch (rng.Below(6)) {
            case 0: {
                const TraversalMove m = static_cast<TraversalMove>(rng.Below(kTraversalMoveCount));
                const MovementState ms = static_cast<MovementState>(rng.Below(kMovementStateCount));
                if (CanStartTraversal(s, m, ms)) {
                    s = StartTraversal(m);
                    CHECK((before.phase == TraversalPhase::Idle || before.phase == TraversalPhase::Done));
                    CHECK(before.cooldown == 0.0);
                    CHECK((m == TraversalMove::Vault || m == TraversalMove::Mantle || m == TraversalMove::Climb));
                    CHECK(ms != MovementState::Falling);
                    CHECK(ms != MovementState::Landing);
                }
                break;
            }
            case 1: s = AdvanceTraversal(s, t, rng.Wild(0.0, 0.5)); break;
            case 2: s = AbortTraversal(s, t); break;
            case 3: s = FailTraversal(s); break;
            case 4: s = AdvanceTraversal(s, t, rng.Range(0.0, 3.0)); break;
            default: break;
            }
            CHECK(s.cooldown >= 0.0);
            CHECK(s.phaseSeconds >= 0.0);
            if (s.phase != before.phase && before.phase != TraversalPhase::Idle && before.phase != TraversalPhase::Done)
                CHECK(static_cast<int>(s.phase) >= static_cast<int>(before.phase));          // never backwards inside a move
            if (s.phase == TraversalPhase::Done) CHECK(s.result != TraversalResult::None);
            if (s.phase == TraversalPhase::Approach || s.phase == TraversalPhase::Active) CHECK(s.result == TraversalResult::None);
            CHECK(TraversalOwnsMovement(s) == (s.phase == TraversalPhase::Approach || s.phase == TraversalPhase::Active));
            if (before.phase == TraversalPhase::Active) CHECK(s.phase != TraversalPhase::Approach);      // no restart in the middle
            if (s.phase != TraversalPhase::Done) CHECK(ExitMovementState(s, true) == MovementState::Falling);
        }
    }
}

TEST_CASE("property: ClassifyTraversalChecked never allows more than ClassifyTraversal and refuses unsafe probes") {
    Rng rng(0x5EED0004);
    const MovementConfig move;
    const TraversalConfig cfg;
    const TraversalSafety safety;
    for (int i = 0; i < 20000; ++i) {
        TraversalProbe p;
        p.obstacle = TraversalObstacle{rng.Wild(-0.5, 4.0), rng.Wild(0.0, 3.0), rng.Wild(0.0, 4.0), rng.Wild(0.0, 8.0), rng.Chance(0.3)};
        p.topSurfaceAngleDegrees = rng.Wild(0.0, 90.0);
        p.landingClearDistance = rng.Wild(0.0, 3.0);
        p.capsuleFitsOnTop = rng.Chance(0.8);
        p.pathClear = rng.Chance(0.8);
        p.approachAngleDegrees = rng.Wild(-90.0, 90.0);
        p.obstacleWidth = rng.Wild(0.0, 3.0);
        p.landingWidth = rng.Wild(0.0, 3.0);
        p.sideClearance = rng.Wild(0.0, 1.0);
        p.obstacleSpeed = rng.Wild(0.0, 3.0);
        p.groundValid = rng.Chance(0.9);
        const TraversalDecision base = ClassifyTraversal(move, cfg, p.obstacle);
        const TraversalDecision checked = ClassifyTraversalChecked(move, cfg, safety, p);
        const bool same = checked.move == base.move;
        const bool refused = checked.move == TraversalMove::Blocked;
        const bool allowed = same || refused;
        CHECK(allowed);
        if (checked.move == TraversalMove::Vault || checked.move == TraversalMove::Mantle || checked.move == TraversalMove::Climb) {
            CHECK(p.pathClear);
            CHECK(p.groundValid);
            CHECK(p.obstacleWidth >= safety.capsuleDiameter);
            CHECK(p.landingWidth >= safety.capsuleDiameter);
            CHECK(p.obstacleSpeed <= safety.maxObstacleSpeed);
            CHECK(std::abs(p.approachAngleDegrees) <= safety.maxApproachAngle);
            if (checked.move == TraversalMove::Vault) CHECK(p.landingClearDistance >= safety.minVaultLanding);
            else {
                CHECK(p.capsuleFitsOnTop);
                CHECK(p.topSurfaceAngleDegrees <= safety.maxTopSurfaceAngle);
            }
        }
    }
}

TEST_CASE("property: CameraArm stays within bounds, shortens at once and grows no faster than the recover speed") {
    Rng rng(0x5EED0005);
    const CameraArmConfig cfg;
    CameraArmState s{4.0, 0.0};
    for (int i = 0; i < 50000; ++i) {
        const double target = DesiredArmLength(cfg, CameraArmContext{rng.Chance(0.3), rng.Chance(0.3)}, rng.Chance(0.5) ? std::optional<double>(rng.Wild(-1.0, 8.0)) : std::nullopt);
        CHECK(target >= cfg.minLength);
        CHECK(target <= cfg.baseLength + cfg.sprintExtra + cfg.airborneExtra + 1e-9);
        const double dt = rng.Range(0.0, 0.1);
        const CameraArmState next = StepCameraArm(cfg, s, target, dt);
        CHECK(next.length >= cfg.minLength - 1e-9);
        if (target <= s.length) CHECK(next.length == target);
        else CHECK(next.length <= s.length + cfg.recoverSpeed * dt + 1e-9);
        CHECK(next.growTimer >= 0.0);
        s = next;
    }
}

TEST_CASE("property: locomotion parameters are bounded and the animation fallback never returns a missing animation") {
    Rng rng(0x5EED0006);
    const MovementConfig cfg;
    for (int i = 0; i < 20000; ++i) {
        LocomotionInput in;
        in.motion = MotionSample{rng.Wild(0.0, 10.0), rng.Wild(-8.0, 8.0), rng.Chance(0.6), rng.Chance(0.3) ? rng.Range(0.0, 0.6) : -1.0};
        in.facingYawDegrees = rng.Wild(-720.0, 720.0);
        in.velocityYawDegrees = rng.Wild(-720.0, 720.0);
        in.desiredTurnDegrees = rng.Wild(-720.0, 720.0);
        in.traversing = rng.Chance(0.1);
        in.justJumped = rng.Chance(0.1);
        const LocomotionParams p = ComputeLocomotion(cfg, in);
        CHECK(p.gaitBlend >= 0.0);
        CHECK(p.gaitBlend <= 4.0);
        CHECK(p.playRate >= 0.5);
        CHECK(p.playRate <= 1.5);
        CHECK(p.directionDegrees >= -180.0);
        CHECK(p.directionDegrees <= 180.0);
        CHECK(static_cast<unsigned>(p.anim) < kLocomotionAnimCount);
        const AnimationMask mask = static_cast<AnimationMask>(rng.Below(1u << kLocomotionAnimCount));
        const LocomotionAnim shown = ResolveAvailableAnim(p.anim, mask);
        const bool available = (mask & AnimBit(shown)) != 0;
        const bool usable = shown == LocomotionAnim::Idle || available;
        CHECK(usable);
    }
}

TEST_CASE("property: reach and perception are monotone in distance and target size") {
    Rng rng(0x5EED0007);
    const ReachConfig reach;
    const NpcPerceptionConfig perc;
    for (int i = 0; i < 20000; ++i) {
        ReachQuery q;
        q.eye = {0.0, 0.0, 1.6};
        q.viewDirection = {rng.Range(-1.0, 1.0), rng.Range(-1.0, 1.0), rng.Range(-0.3, 0.3)};
        q.targetCentre = {rng.Range(-4.0, 4.0), rng.Range(-4.0, 4.0), rng.Range(0.5, 2.5)};
        q.targetRadius = rng.Range(0.0, 0.5);
        q.lineOfSightClear = rng.Chance(0.9);
        if (Speed(Vec2{q.viewDirection.x, q.viewDirection.y}) < 0.05) continue;
        if (CheckReach(reach, q) == ReachResult::Ok) {
            ReachQuery bigger = q;
            bigger.targetRadius += 0.3;
            CHECK(CheckReach(reach, bigger) == ReachResult::Ok);                 // a bigger target is never harder, in all four checks
        }
        const Vec2 npc{0.0, 0.0};
        const double facing = rng.Range(-360.0, 360.0);
        const NpcStimulus s{{rng.Range(-30.0, 30.0), rng.Range(-30.0, 30.0)}, rng.Range(0.0, 2.0), rng.Chance(0.8)};
        if (Perceive(perc, npc, facing, s) == Perception::Seen) {
            NpcStimulus nearer = s;
            nearer.position = {s.position.x * 0.5, s.position.y * 0.5};          // same bearing, half the distance
            CHECK(Perceive(perc, npc, facing, nearer) == Perception::Seen);
        }
        if (Perceive(perc, npc, facing, s) == Perception::Heard) {
            NpcStimulus louder = s;
            louder.loudness = s.loudness + 0.5;
            CHECK(Perceive(perc, npc, facing, louder) != Perception::None);
        }
    }
}

TEST_CASE("property: gait and motion functions return valid enumerators for any input") {
    Rng rng(0x5EED0008);
    const MovementConfig cfg;
    for (int i = 0; i < 20000; ++i) {
        const Gait base = static_cast<Gait>(rng.Below(6));                      // includes corrupt values 4 and 5
        const GaitLimits limits{static_cast<Gait>(rng.Below(6)), rng.Wild(0.0, 1.0)};
        const MoveIntent intent{rng.Wild(-1.0, 1.0), rng.Wild(-1.0, 1.0)};
        const Gait g = ResolveGait(cfg, base, rng.Chance(0.5), limits, intent, rng.Wild(0.0, 10.0));
        CHECK(static_cast<unsigned>(g) < kGaitCount);
        const MovementState st = ClassifyMotion(cfg, MotionSample{rng.Wild(0.0, 10.0), rng.Wild(-8.0, 8.0), rng.Chance(0.5), rng.Wild(-1.0, 1.0)});
        CHECK(static_cast<unsigned>(st) < kMovementStateCount);
        const double eff = EffectiveSpeed(cfg, base, limits);
        CHECK(std::isfinite(eff) == true);
        CHECK(eff >= 0.0);
    }
}
