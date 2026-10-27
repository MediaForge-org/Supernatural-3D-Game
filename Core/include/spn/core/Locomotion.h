#pragma once

// Animation-facing locomotion parameters (overnight work order M2-A.2, package 14): the numbers and states an animation graph
// needs, derived only from physical quantities so every client computes the same thing. Pure functions; no Unreal types.
//
// This is the DATA CONTRACT between movement and a future animation blueprint. It does not mean any animation, blend space or
// motion matching exists or works: there are no animation assets yet, and nothing here was run against an AnimInstance.
// ENGINEERING PROPOSAL, not master canon. Units: metres, seconds, degrees.

#include <cstdint>

#include <spn/core/Movement.h>

namespace spn::core {

/// What the animation graph can play. Gait states first (same order as MovementState), then the rest.
enum class LocomotionAnim : std::uint8_t { Idle = 0, Walk, Jog, Run, Sprint, JumpStart, Falling, Landing, TurnInPlace, Traversal };

inline constexpr std::size_t kLocomotionAnimCount = 10;

[[nodiscard]] const char* LocomotionAnimName(LocomotionAnim anim) noexcept;

struct LocomotionParams {
    LocomotionAnim anim = LocomotionAnim::Idle;
    /// Continuous gait coordinate for a 1-D blend space: 0 = standing, 1 = Walk speed, 2 = Jog, 3 = Run, 4 = Sprint (linear between).
    double gaitBlend = 0.0;
    /// Playback rate that matches the feet to the ground: speed / top speed of the nearest gait (1 at a gait's own speed). Clamped 0.5..1.5.
    double playRate = 1.0;
    /// Signed angle between facing and velocity (-180..180, + = velocity to the right); lean / strafe input.
    double directionDegrees = 0.0;
    bool inAir = false;
};

struct LocomotionInput {
    MotionSample motion;            ///< as for ClassifyMotion
    double facingYawDegrees = 0.0;  ///< where the character looks
    double velocityYawDegrees = 0.0; ///< where it moves (ignored below the idle speed)
    double desiredTurnDegrees = 0.0; ///< angle to the wanted facing while standing
    bool traversing = false;        ///< a traversal move owns the movement
    bool justJumped = false;        ///< the jump started this frame (rising, first sample)
};

/// Turn in place starts when standing and the wanted facing is off by more than this (degrees).
inline constexpr double kTurnInPlaceMinDegrees = 45.0;

[[nodiscard]] LocomotionParams ComputeLocomotion(const MovementConfig& config, const LocomotionInput& input) noexcept;

/// Bit set of animations that exist as assets (bit = 1 << LocomotionAnim).
using AnimationMask = std::uint32_t;
[[nodiscard]] constexpr AnimationMask AnimBit(LocomotionAnim a) noexcept { return AnimationMask{1} << static_cast<unsigned>(a); }

/// The animation to play when the wanted one is missing: Sprint -> Run -> Jog -> Walk -> Idle, JumpStart -> Falling -> Idle,
/// Landing -> Idle, TurnInPlace -> Idle, Traversal -> Idle (never a teleport; the movement itself is unchanged). Idle is always usable.
[[nodiscard]] LocomotionAnim ResolveAvailableAnim(LocomotionAnim wanted, AnimationMask available) noexcept;

} // namespace spn::core
