#pragma once

// On-foot movement rules that do not depend on the engine (Docs/Proposals/0011, M2-A / M2-A.1):
// the four gaits, the vertical jump height, the step-up height, directional speed scale, the
// observable movement state and the obstacle classes that follow from what the character can
// physically do today.
//
// All values are PLACEHOLDER TEST VALUES from the M2-A / M2-A.1 work orders (2026-10-25/26), not final
// gameplay numbers: later injuries, exhaustion and supernatural abilities will change them.
// Units are SI (metres, seconds). The Unreal adapter converts to centimetres.
//
// ENGINEERING PROPOSAL, not master canon.

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

namespace spn::core {

/// Ordered from slowest to fastest; the order is used by GaitLimits (maxGait).
enum class Gait : std::uint8_t { Walk = 0, Jog = 1, Run = 2, Sprint = 3 };

inline constexpr std::size_t kGaitCount = 4;

[[nodiscard]] const char* GaitName(Gait gait) noexcept;

/// Gait from its 2-bit wire value (0..3); nullopt for anything else.
[[nodiscard]] std::optional<Gait> GaitFromIndex(unsigned index) noexcept;

struct GaitSettings {
    double speed = 0.0;        ///< Top speed on level ground, m/s.
    double acceleration = 0.0; ///< Acceleration while holding full input, m/s^2.
};

struct MovementConfig {
    /// Indexed by Gait.
    std::array<GaitSettings, kGaitCount> gaits{{
        {1.8, 2.0}, // Walk
        {3.5, 3.0}, // Jog
        {5.5, 3.5}, // Run
        {8.0, 4.0}, // Sprint
    }};
    /// Gait used while no other gait is requested.
    Gait defaultGait = Gait::Jog;

    double brakingDeceleration = 8.0; ///< No input, m/s^2.
    /// Speed factor for pure backward / sideways input (applies only to movement that does not
    /// turn the character towards its travel direction). 1 = same as forward.
    double backwardSpeedFactor = 0.6;
    double strafeSpeedFactor = 0.85;
    /// Starting a sprint needs input within this angle of the facing direction (degrees, 0..180).
    double sprintMaxAngleDegrees = 50.0;
    /// While already at sprint speed, the sprint is kept up to this wider angle (degrees, >= the
    /// start angle). Without it a sharp turn drops the sprint for the time the character needs to
    /// turn towards the new direction, and the speed falls with it.
    double sprintSustainAngleDegrees = 100.0;

    /// How fast the character turns towards its movement direction (degrees per second).
    double turnRateDegrees = 500.0;
    /// Speed of the smallest analog stick deflection that still moves the character, m/s.
    double minAnalogSpeed = 0.2;

    /// Vertical apex height of an active jump (standing, flat ground), m. Independent of the step
    /// height and NOT the maximum obstacle height.
    double jumpHeight = 0.45;
    /// Highest ledge walked up without jumping, m (mirrors the movement component's step height).
    /// Independent of the jump height.
    double maxStepHeight = 0.25;

    /// Below this horizontal speed the character counts as standing (m/s).
    double idleSpeed = 0.05;
    /// How long after touching down the Landing state lasts (seconds).
    double landingSeconds = 0.25;
};

/// nullopt when valid, else a short reason. Rules: finite, positive speeds and accelerations;
/// gait speeds strictly ascending; default gait below Sprint; factors in (0, 1]; sprint start angle
/// in [0, 180] and sustain angle in [start angle, 180]; turn rate in (0, 3600] degrees/s; minimum
/// analog speed in [0, walk speed]; jump height in (0, 2] m; step height in [0, 1] m; idle speed in
/// [0, walk speed); landing time in [0, 2] s. The jump height and the step height are validated on
/// their own: neither is derived from, nor limited by, the other. No other tuning rule is implied.
[[nodiscard]] std::optional<std::string> Validate(const MovementConfig& config);

/// Top speed of `gait` in m/s. An out-of-range enum value is treated as Sprint (never indexes out of range).
[[nodiscard]] double GaitSpeed(const MovementConfig& config, Gait gait) noexcept;
[[nodiscard]] double GaitAcceleration(const MovementConfig& config, Gait gait) noexcept;

/// Allowed upper bounds on movement, set by the game (injury, exhaustion, load): a gait ceiling
/// and a speed multiplier. The defaults restrict nothing. Nothing in M2-A sets them.
struct GaitLimits {
    Gait maxGait = Gait::Sprint;
    double speedScale = 1.0; ///< 0 < scale <= 1
};

/// What the player is pushing the stick/keys towards, in the character's frame.
/// `forward`/`right` are the components of the input vector (|v| <= 1); right is +.
struct MoveIntent {
    double forward = 0.0;
    double right = 0.0;
};

/// The gait actually used. `base` is the gait the player has chosen for normal movement (Sprint
/// counts as Run); `sprintRequested` is the held sprint input. Sprint is used only when requested,
/// not above limits.maxGait, and with input pointing within sprintMaxAngleDegrees of straight ahead; once the
/// character is at sprint speed (`currentSpeed`, m/s, horizontal) the wider sprintSustainAngleDegrees
/// applies instead, so a sharp turn does not drop the sprint. Otherwise the result is `base`.
/// The result never exceeds limits.maxGait. `currentSpeed` is part of the replicated movement state
/// (the velocity), so client and server see the same value.
[[nodiscard]] Gait ResolveGait(const MovementConfig& config, Gait base, bool sprintRequested,
                               const GaitLimits& limits, MoveIntent intent, double currentSpeed = 0.0) noexcept;

/// The gait whose top speed is nearest to `horizontalSpeed` (m/s); exactly between two gaits gives the
/// lower one (floating-point rounding can decide a tie either way). Negative or non-finite speeds count as 0. Meant for display and animation; it never
/// feeds back into the speed rules except through the sprint-sustain test of ResolveGait.
[[nodiscard]] Gait ObservedGait(const MovementConfig& config, double horizontalSpeed) noexcept;

/// What the character is doing, derived only from physical quantities. There is no stored state
/// machine: the same sample always gives the same state, so every client (and the animation) can
/// compute it. Ordered as the wire/animation convention: gaits first.
enum class MovementState : std::uint8_t { Idle = 0, Walk, Jog, Run, Sprint, Jumping, Falling, Landing };

inline constexpr std::size_t kMovementStateCount = 8;

[[nodiscard]] const char* MovementStateName(MovementState state) noexcept;

struct MotionSample {
    double horizontalSpeed = 0.0;      ///< m/s
    double verticalVelocity = 0.0;     ///< m/s, up is positive
    bool onGround = true;              ///< standing or walking on a floor
    /// Seconds since the last touch-down; negative or infinite means "no recent landing".
    double secondsSinceLanding = -1.0;
};

/// Airborne: Jumping while moving up, Falling otherwise. On the ground: Landing for
/// config.landingSeconds after a touch-down, then Idle below config.idleSpeed, else the observed gait.
[[nodiscard]] MovementState ClassifyMotion(const MovementConfig& config, const MotionSample& sample) noexcept;

/// Multiplier (backwardSpeedFactor..1) for input direction relative to facing. Only meaningful
/// when the character does not rotate towards its movement. Zero input gives 1.
[[nodiscard]] double DirectionSpeedScale(const MovementConfig& config, MoveIntent intent) noexcept;

/// Final top speed in m/s: gait speed * limits.speedScale (scale clamped to [0, 1], NaN = 1; corrupt gaits count as Sprint). Directional scale is separate.
[[nodiscard]] double EffectiveSpeed(const MovementConfig& config, Gait gait, const GaitLimits& limits) noexcept;

/// Launch velocity (m/s) so that a jump reaches `jumpHeight` under `gravity` (m/s^2, positive).
/// Returns 0 for non-positive arguments.
[[nodiscard]] double JumpVelocityForHeight(double jumpHeight, double gravity) noexcept;

/// Apex height (m) of a jump with `launchVelocity` under `gravity`; 0 for non-positive gravity.
[[nodiscard]] double JumpApexHeight(double launchVelocity, double gravity) noexcept;

/// What the character can do about a ledge of the given height with today's abilities.
/// There is no vault or mantle yet (Proposal 0009): taller obstacles are Blocked until a later
/// traversal milestone adds moves and thresholds.
///
/// This is a rule of thumb about the HEIGHT only. Whether a jump really gets the character on top of a
/// ledge also depends on the run-up, the timing, the depth of the obstacle and the collision shapes.
enum class ObstacleClass : std::uint8_t {
    StepUp,        ///< height <= maxStepHeight: walking up is enough (given suitable collision geometry).
    JumpCandidate, ///< above the step height and below the jump apex: a jump MAY get over it; not guaranteed.
    Blocked,       ///< at or above the jump apex and above the step height: no ability of today overcomes it.
};

[[nodiscard]] const char* ObstacleClassName(ObstacleClass c) noexcept;

/// `height` in m (negative counts as 0). The apex of a jump is reached for an instant, so a ledge exactly
/// as high as the jump height is Blocked unless the step height already covers it. NaN counts as Blocked.
[[nodiscard]] ObstacleClass ClassifyObstacle(const MovementConfig& config, double height) noexcept;

} // namespace spn::core
