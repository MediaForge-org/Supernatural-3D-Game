#include <doctest.h>

#include <spn/core/Locomotion.h>

using namespace spn::core;

namespace {
const MovementConfig kCfg;
LocomotionParams Compute(double speed, double vertical = 0.0, bool ground = true, double sinceLanding = -1.0, bool traversing = false, double turn = 0.0) {
    LocomotionInput in;
    in.motion = MotionSample{speed, vertical, ground, sinceLanding};
    in.traversing = traversing;
    in.desiredTurnDegrees = turn;
    return ComputeLocomotion(kCfg, in);
}
} // namespace

TEST_CASE("locomotion animation follows the movement state") {
    CHECK(Compute(0.0).anim == LocomotionAnim::Idle);
    CHECK(Compute(1.8).anim == LocomotionAnim::Walk);
    CHECK(Compute(3.5).anim == LocomotionAnim::Jog);
    CHECK(Compute(5.5).anim == LocomotionAnim::Run);
    CHECK(Compute(8.0).anim == LocomotionAnim::Sprint);
    CHECK(Compute(0.0, 2.0, false).anim == LocomotionAnim::Falling);
    CHECK(Compute(0.0, -3.0, false).anim == LocomotionAnim::Falling);
    CHECK(Compute(5.0, 0.0, true, 0.1).anim == LocomotionAnim::Landing);
    CHECK(Compute(8.0, 0.0, true, -1.0, true).anim == LocomotionAnim::Traversal);
    LocomotionInput jump;
    jump.motion = MotionSample{3.0, 3.0, false, -1.0};
    jump.justJumped = true;
    CHECK(ComputeLocomotion(kCfg, jump).anim == LocomotionAnim::JumpStart);
    CHECK(ComputeLocomotion(kCfg, jump).inAir);
}

TEST_CASE("turn in place only when standing and the wanted facing is far off") {
    CHECK(Compute(0.0, 0.0, true, -1.0, false, 90.0).anim == LocomotionAnim::TurnInPlace);
    CHECK(Compute(0.0, 0.0, true, -1.0, false, -170.0).anim == LocomotionAnim::TurnInPlace);
    CHECK(Compute(0.0, 0.0, true, -1.0, false, 30.0).anim == LocomotionAnim::Idle);
    CHECK(Compute(0.0, 0.0, true, -1.0, false, 350.0).anim == LocomotionAnim::Idle); // wraps to -10
    CHECK(Compute(3.5, 0.0, true, -1.0, false, 120.0).anim == LocomotionAnim::Jog);
}

TEST_CASE("gait blend is continuous and play rate matches the feet") {
    CHECK(Compute(0.0).gaitBlend == doctest::Approx(0.0));
    CHECK(Compute(1.8).gaitBlend == doctest::Approx(1.0));
    CHECK(Compute(3.5).gaitBlend == doctest::Approx(2.0));
    CHECK(Compute(5.5).gaitBlend == doctest::Approx(3.0));
    CHECK(Compute(8.0).gaitBlend == doctest::Approx(4.0));
    CHECK(Compute(50.0).gaitBlend == doctest::Approx(4.0));
    double last = -1.0;
    for (double s = 0.0; s <= 9.0; s += 0.05) {
        const double b = Compute(s).gaitBlend;
        CHECK(b >= last);
        last = b;
    }
    CHECK(Compute(3.5).playRate == doctest::Approx(1.0));
    CHECK(Compute(3.0).playRate < 1.0);
    CHECK(Compute(0.9).playRate == doctest::Approx(0.5)); // clamped
    CHECK(Compute(0.0).playRate == doctest::Approx(1.0));
}

TEST_CASE("direction angle is signed and wrapped") {
    LocomotionInput in;
    in.motion = MotionSample{3.5, 0.0, true, -1.0};
    in.facingYawDegrees = 170.0;
    in.velocityYawDegrees = -170.0;
    CHECK(ComputeLocomotion(kCfg, in).directionDegrees == doctest::Approx(20.0));
    in.velocityYawDegrees = 80.0;
    CHECK(ComputeLocomotion(kCfg, in).directionDegrees == doctest::Approx(-90.0));
    in.motion.horizontalSpeed = 0.0;
    CHECK(ComputeLocomotion(kCfg, in).directionDegrees == 0.0);
}

TEST_CASE("missing animations fall back without ever returning a missing one") {
    const AnimationMask onlyWalk = AnimBit(LocomotionAnim::Walk);
    CHECK(ResolveAvailableAnim(LocomotionAnim::Sprint, onlyWalk) == LocomotionAnim::Walk);
    CHECK(ResolveAvailableAnim(LocomotionAnim::Run, AnimBit(LocomotionAnim::Jog)) == LocomotionAnim::Jog);
    CHECK(ResolveAvailableAnim(LocomotionAnim::Sprint, AnimBit(LocomotionAnim::Sprint)) == LocomotionAnim::Sprint);
    CHECK(ResolveAvailableAnim(LocomotionAnim::JumpStart, AnimBit(LocomotionAnim::Falling)) == LocomotionAnim::Falling);
    CHECK(ResolveAvailableAnim(LocomotionAnim::Landing, 0) == LocomotionAnim::Idle);
    CHECK(ResolveAvailableAnim(LocomotionAnim::Traversal, onlyWalk) == LocomotionAnim::Idle);
    for (unsigned w = 0; w < kLocomotionAnimCount; ++w) {
        for (AnimationMask mask : {AnimationMask{0}, onlyWalk, AnimationMask{0x3FF}}) {
            const auto got = ResolveAvailableAnim(static_cast<LocomotionAnim>(w), mask);
            CHECK((got == LocomotionAnim::Idle || (mask & AnimBit(got)) != 0));
        }
    }
}
