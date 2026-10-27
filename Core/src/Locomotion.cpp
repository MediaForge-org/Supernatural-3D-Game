#include <spn/core/Locomotion.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

double WrapDegrees(double a) noexcept {
    if (!std::isfinite(a)) return 0.0;
    a = std::fmod(a + 180.0, 360.0);
    if (a < 0.0) a += 360.0;
    return a - 180.0;
}

} // namespace

const char* LocomotionAnimName(LocomotionAnim anim) noexcept {
    switch (anim) {
    case LocomotionAnim::Idle: return "Idle";
    case LocomotionAnim::Walk: return "Walk";
    case LocomotionAnim::Jog: return "Jog";
    case LocomotionAnim::Run: return "Run";
    case LocomotionAnim::Sprint: return "Sprint";
    case LocomotionAnim::JumpStart: return "JumpStart";
    case LocomotionAnim::Falling: return "Falling";
    case LocomotionAnim::Landing: return "Landing";
    case LocomotionAnim::TurnInPlace: return "TurnInPlace";
    case LocomotionAnim::Traversal: return "Traversal";
    }
    return "Unknown";
}

LocomotionParams ComputeLocomotion(const MovementConfig& config, const LocomotionInput& in) noexcept {
    LocomotionParams out;
    const double speed = std::isfinite(in.motion.horizontalSpeed) ? std::max(in.motion.horizontalSpeed, 0.0) : 0.0;
    out.inAir = !in.motion.onGround;
    if (speed >= config.idleSpeed) out.directionDegrees = WrapDegrees(in.velocityYawDegrees - in.facingYawDegrees);

    // Gait coordinate: piecewise linear through (0 -> 0), (walk -> 1), (jog -> 2), (run -> 3), (sprint -> 4).
    const double walk = GaitSpeed(config, Gait::Walk), jog = GaitSpeed(config, Gait::Jog);
    const double run = GaitSpeed(config, Gait::Run), sprint = GaitSpeed(config, Gait::Sprint);
    if (speed <= walk) out.gaitBlend = speed / walk;
    else if (speed <= jog) out.gaitBlend = 1.0 + (speed - walk) / (jog - walk);
    else if (speed <= run) out.gaitBlend = 2.0 + (speed - jog) / (run - jog);
    else out.gaitBlend = std::min(3.0 + (speed - run) / (sprint - run), 4.0);

    const Gait nearest = ObservedGait(config, speed);
    out.playRate = std::clamp(speed / GaitSpeed(config, nearest), 0.5, 1.5);

    if (in.traversing) {
        out.anim = LocomotionAnim::Traversal;
        return out;
    }
    switch (ClassifyMotion(config, in.motion)) {
    case MovementState::Jumping: out.anim = in.justJumped ? LocomotionAnim::JumpStart : LocomotionAnim::Falling; break;
    case MovementState::Falling: out.anim = LocomotionAnim::Falling; break;
    case MovementState::Landing: out.anim = LocomotionAnim::Landing; break;
    case MovementState::Walk: out.anim = LocomotionAnim::Walk; break;
    case MovementState::Jog: out.anim = LocomotionAnim::Jog; break;
    case MovementState::Run: out.anim = LocomotionAnim::Run; break;
    case MovementState::Sprint: out.anim = LocomotionAnim::Sprint; break;
    case MovementState::Idle:
        out.anim = std::abs(WrapDegrees(in.desiredTurnDegrees)) > kTurnInPlaceMinDegrees ? LocomotionAnim::TurnInPlace : LocomotionAnim::Idle;
        out.playRate = 1.0;
        break;
    }
    return out;
}

LocomotionAnim ResolveAvailableAnim(LocomotionAnim wanted, AnimationMask available) noexcept {
    const auto has = [&](LocomotionAnim a) { return (available & AnimBit(a)) != 0; };
    switch (wanted) {
    case LocomotionAnim::Sprint: if (has(LocomotionAnim::Sprint)) return LocomotionAnim::Sprint; [[fallthrough]];
    case LocomotionAnim::Run: if (has(LocomotionAnim::Run)) return LocomotionAnim::Run; [[fallthrough]];
    case LocomotionAnim::Jog: if (has(LocomotionAnim::Jog)) return LocomotionAnim::Jog; [[fallthrough]];
    case LocomotionAnim::Walk: if (has(LocomotionAnim::Walk)) return LocomotionAnim::Walk; return LocomotionAnim::Idle;
    case LocomotionAnim::JumpStart: if (has(LocomotionAnim::JumpStart)) return LocomotionAnim::JumpStart; [[fallthrough]];
    case LocomotionAnim::Falling: if (has(LocomotionAnim::Falling)) return LocomotionAnim::Falling; return LocomotionAnim::Idle;
    case LocomotionAnim::Landing: return has(LocomotionAnim::Landing) ? LocomotionAnim::Landing : LocomotionAnim::Idle;
    case LocomotionAnim::TurnInPlace: return has(LocomotionAnim::TurnInPlace) ? LocomotionAnim::TurnInPlace : LocomotionAnim::Idle;
    case LocomotionAnim::Traversal: return has(LocomotionAnim::Traversal) ? LocomotionAnim::Traversal : LocomotionAnim::Idle;
    case LocomotionAnim::Idle: break;
    }
    return LocomotionAnim::Idle;
}

} // namespace spn::core
