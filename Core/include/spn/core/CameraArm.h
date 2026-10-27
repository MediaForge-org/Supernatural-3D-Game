#pragma once

// Third-person camera arm rules (overnight work order M2-A.2 phase 2, package G2): how long the arm should be given what the
// collision probe found, and how the length moves over time. Pure arithmetic for the (future) Unreal adapter; the existing
// camera component is NOT changed. Goals: never look through a wall (pull in at once), never pop out through a doorway
// (recover slowly and only after the view has been clear for a moment), and a little more distance while sprinting.
//
// All values are PLACEHOLDER TEST VALUES. Units: metres, seconds. ENGINEERING PROPOSAL, not master canon.

#include <optional>
#include <string>

namespace spn::core {

struct CameraArmConfig {
    double baseLength = 4.0;       ///< m, normal distance
    double sprintExtra = 0.5;      ///< m added while sprinting
    double airborneExtra = 0.0;    ///< m added in the air (0 = none)
    double minLength = 0.6;        ///< m, closest the camera may come (tight rooms)
    double collisionMargin = 0.15; ///< m kept between the camera and the wall behind which it would sit
    double recoverSpeed = 2.5;     ///< m/s the arm grows back after a pull-in
    double recoverDelay = 0.25;    ///< s the target must stay longer than the arm before it grows back (doorway flicker)
    double blendSpeed = 3.0;       ///< m/s for changes of the free (unobstructed) target length, e.g. sprint on/off
};

[[nodiscard]] std::optional<std::string> Validate(const CameraArmConfig& config);

struct CameraArmContext {
    bool sprinting = false;
    bool airborne = false;
};

/// Desired length: base (+ extras), shortened to the free distance found by the probe (`hitDistance` = distance from the pivot to the
/// obstacle along the arm, nullopt = nothing hit) minus the margin, never below minLength.
[[nodiscard]] double DesiredArmLength(const CameraArmConfig& config, const CameraArmContext& context, std::optional<double> hitDistance) noexcept;

struct CameraArmState {
    double length = 4.0;
    double growTimer = 0.0;  ///< seconds the target has been longer than the arm
};

/// One step: a shorter target applies immediately (no clipping through the wall); a longer target only after recoverDelay, then at
/// recoverSpeed. dt <= 0 or non-finite changes nothing.
[[nodiscard]] CameraArmState StepCameraArm(const CameraArmConfig& config, CameraArmState state, double target, double dt) noexcept;

} // namespace spn::core
