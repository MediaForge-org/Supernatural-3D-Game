#pragma once

// Velocity steering on the ground (overnight work order M2-A.2, package 4): how the velocity VECTOR follows the input direction.
//
// Why: the engine's default ground friction blends the velocity vector towards "input direction * speed". For a 90 degree turn at
// sprint speed that blend passes through a much shorter vector (about 0.7 of the speed at the middle of the turn, then the
// remaining acceleration has to rebuild it): a measured drop from about 800 to 462 cm/s was reported (M2-A.1 Windows run).
// This function is the reference model of a turn that bends the heading at a limited rate and loses speed in proportion to
// the angle turned. It is pure arithmetic; NOT yet used by the Unreal movement component (that file has uncommitted Windows
// changes; see Docs/Development/M2A2-Fedora-To-Windows-Handover.md for the integration patch proposal).
//
// All values are PLACEHOLDER TEST VALUES. ENGINEERING PROPOSAL, not master canon. Units: metres, seconds.

#include <optional>
#include <string>

#include <spn/core/Vec.h>

namespace spn::core {

struct SteeringConfig {
    /// How fast the velocity heading may rotate (degrees per second) at walking speed; it shrinks towards
    /// `minHeadingRateDegrees` at `fullSpeed` so a sprinter takes a wider arc.
    double maxHeadingRateDegrees = 720.0;
    double minHeadingRateDegrees = 300.0;
    double fullSpeed = 8.0;          ///< m/s where the minimum rate applies
    /// Fraction of the speed lost per radian of heading change (0.15: a 90 degree turn costs about 23 %).
    double speedLossPerRadian = 0.15;
};

[[nodiscard]] std::optional<std::string> Validate(const SteeringConfig& config);

struct SteeringInput {
    Vec2 direction;            ///< desired heading (any length; zero = no input)
    double targetSpeed = 0.0;  ///< m/s the gait allows right now (EffectiveSpeed * analog scale)
    double acceleration = 0.0; ///< m/s^2 while input is held
    double braking = 0.0;      ///< m/s^2 without input, or above the target speed
};

/// One step of dt seconds (dt >= 0, non-finite -> unchanged). Without input the speed falls by `braking`, the heading is kept.
/// With input the heading rotates towards the direction by at most the allowed rate (losing speed per angle turned), then the
/// speed moves towards targetSpeed by acceleration / braking. At rest the character starts straight along the input direction.
[[nodiscard]] Vec2 SteerVelocity(const SteeringConfig& config, Vec2 velocity, const SteeringInput& input, double dt) noexcept;

[[nodiscard]] double Speed(Vec2 v) noexcept;

} // namespace spn::core
