#pragma once

// Kinematic model of a jump against a ledge (overnight work order M2-A.2 phase 2, package D3): when does a jump get the feet above
// a ledge BEFORE the horizontal movement reaches the ledge face? Pure arithmetic, no collision, no engine.
//
// Why: in the engine, an airborne character that touches a ledge face while its feet are still below the ledge top loses its
// horizontal velocity at the face and keeps rising, i.e. it climbs "vertically" and only moves onto the ledge afterwards (reported
// as "scheinbar vertikal, dann abrupt aufs Hindernis"). Step-up is a walking-mode mechanism; this model assumes none while airborne.
// The model explains HOW CLOSE to a ledge a jump may start; it does not prove what the engine does: that needs the diagnostics
// component (Spn/SpnMovementDiagnostics) on Windows.
//
// Units: metres, seconds. ENGINEERING PROPOSAL, not master canon.

#include <cstdint>

namespace spn::core {

/// Height (m) of the feet above the take-off point `t` seconds after launch (launch velocity v0 up, gravity g > 0). Ballistic; no ground.
[[nodiscard]] double JumpHeightAt(double launchVelocity, double gravity, double t) noexcept;

struct JumpWindow {
    bool reachable = false;     ///< the apex is higher than the ledge
    double riseTime = 0.0;      ///< s after launch when the feet first reach the ledge height
    double fallTime = 0.0;      ///< s after launch when they sink below it again
    double minDistance = 0.0;   ///< m: take-off closer than this to the ledge face hits the face while still rising
    double maxDistance = 0.0;   ///< m: take-off farther than this lands before the ledge (or hits the face while falling)
};

/// Window of take-off distances (horizontal distance from the take-off point to the ledge face) at which a jump with
/// `launchVelocity` and horizontal `speed` has its feet above `ledgeHeight` when it arrives at the face. Not reachable when the apex is
/// at or below the ledge, or the arguments are not finite / not positive.
[[nodiscard]] JumpWindow ComputeJumpWindow(double launchVelocity, double gravity, double ledgeHeight, double speed) noexcept;

enum class JumpOutcome : std::uint8_t {
    Clears,         ///< feet above the ledge at the face
    BlockedRising,  ///< touches the face while rising: loses the horizontal speed there, climbs vertically
    FallsShort,     ///< touches the face (or the ground) while falling, below the ledge top
    TooHigh,        ///< the apex does not reach the ledge
};

[[nodiscard]] const char* JumpOutcomeName(JumpOutcome outcome) noexcept;

struct JumpAttempt {
    double launchVelocity = 0.0;
    double gravity = 9.81;
    double speed = 0.0;         ///< horizontal speed at take-off, m/s
    double distanceToFace = 0.0;///< m
    double ledgeHeight = 0.0;   ///< m
};

[[nodiscard]] JumpOutcome EvaluateJump(const JumpAttempt& attempt) noexcept;

/// For BlockedRising: how long (s) the character keeps rising along the face before its feet pass the ledge top; this is the visible
/// "vertical" phase. 0 for every other outcome.
[[nodiscard]] double VerticalClimbSeconds(const JumpAttempt& attempt) noexcept;

// --- Contact with the ledge face (phase 3, package G3) ---------------------------------------------------------------------------
//
// Model of what happens when the jump touches the face while still rising (BlockedRising): the engine-style response removes the
// horizontal velocity at the face (slide along the wall), the character keeps rising, and once the feet are above the ledge top it
// has to move `overhang` metres forward to stand on it. In the air the forward acceleration is only airControl * groundAcceleration.
// Two responses are compared: `KeepSpeed == false` = the engine-style one above; `KeepSpeed == true` = the proposed "ledge assist"
// (do not delete the horizontal speed while the apex clears the ledge). ASSUMPTIONS, not engine facts: the face stops the speed
// completely, the character needs `overhang` of forward travel after clearing the top, air acceleration is constant.

struct FaceJumpParams {
    JumpAttempt attempt;          ///< the take-off (use a distance below the window to get a face contact)
    double airAcceleration = 1.2; ///< m/s^2 forward while airborne (airControl 0.35 * 3.5 m/s^2 Run acceleration = 1.2)
    double overhang = 0.30;       ///< m the capsule centre must travel beyond the face before it is supported by the ledge
    bool keepSpeed = false;       ///< ledge assist: keep the take-off speed instead of stopping at the face
};

struct FaceJumpResult {
    JumpOutcome outcome = JumpOutcome::TooHigh;
    bool onLedge = false;          ///< the character ends up standing on the ledge top
    double timeFeetAboveLedge = 0.0; ///< s after launch when the feet pass the ledge top (the end of the vertical climb)
    double airTimeLeft = 0.0;      ///< s still airborne above the ledge height after that moment
    double travelNeeded = 0.0;     ///< m of forward travel still needed at that moment
    double travelAchieved = 0.0;   ///< m covered before the feet sink below the ledge top again
};

/// Evaluates the attempt with the given response. For Clears/FallsShort/TooHigh no face contact happens in the rising phase and the
/// result only reports the outcome and the times (onLedge = outcome == Clears).
[[nodiscard]] FaceJumpResult SimulateFaceJump(const FaceJumpParams& params) noexcept;

} // namespace spn::core
