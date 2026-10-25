#pragma once

// On-foot movement rules that do not depend on the engine (Docs/Proposals/0011, M2-A):
// the four gaits, the vertical jump height, directional speed scale and the obstacle
// classes that follow from what the character can physically do today.
//
// All values are PLACEHOLDER TEST VALUES from the M2-A work order (2026-10-25), not final
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
        {1.4, 2.0}, // Walk
        {3.0, 3.0}, // Jog
        {5.0, 3.5}, // Run
        {8.0, 4.0}, // Sprint
    }};
    /// Gait used while no other gait is requested.
    Gait defaultGait = Gait::Jog;

    double brakingDeceleration = 8.0; ///< No input, m/s^2.
    /// Speed factor for pure backward / sideways input (applies only to movement that does not
    /// turn the character towards its travel direction). 1 = same as forward.
    double backwardSpeedFactor = 0.6;
    double strafeSpeedFactor = 0.85;
    /// Sprinting needs input within this angle of the facing direction (degrees, 0..180).
    double sprintMaxAngleDegrees = 50.0;

    /// Vertical apex height of an active jump (standing, flat ground), m. Not the maximum
    /// obstacle height.
    double jumpHeight = 0.45;
    /// Highest ledge walked up without jumping, m (mirrors the movement component's step height).
    double maxStepHeight = 0.45;
};

/// nullopt when valid, else a short reason. Rules: finite, positive speeds and accelerations;
/// gait speeds strictly ascending; default gait below Sprint; factors in (0, 1]; sprint angle in
/// [0, 180]; jump height in (0, 2] m; step height in [0, 1] m. No other tuning rule is implied.
[[nodiscard]] std::optional<std::string> Validate(const MovementConfig& config);

/// Top speed of `gait` in m/s.
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
/// not above limits.maxGait, and with input pointing forward within sprintMaxAngleDegrees; otherwise
/// the result is `base`. The result never exceeds limits.maxGait.
[[nodiscard]] Gait ResolveGait(const MovementConfig& config, Gait base, bool sprintRequested,
                               const GaitLimits& limits, MoveIntent intent) noexcept;

/// Multiplier (backwardSpeedFactor..1) for input direction relative to facing. Only meaningful
/// when the character does not rotate towards its movement. Zero input gives 1.
[[nodiscard]] double DirectionSpeedScale(const MovementConfig& config, MoveIntent intent) noexcept;

/// Final top speed in m/s: gait speed * limits.speedScale. Directional scale is separate.
[[nodiscard]] double EffectiveSpeed(const MovementConfig& config, Gait gait, const GaitLimits& limits) noexcept;

/// Launch velocity (m/s) so that a jump reaches `jumpHeight` under `gravity` (m/s^2, positive).
/// Returns 0 for non-positive arguments.
[[nodiscard]] double JumpVelocityForHeight(double jumpHeight, double gravity) noexcept;

/// Apex height (m) of a jump with `launchVelocity` under `gravity`; 0 for non-positive gravity.
[[nodiscard]] double JumpApexHeight(double launchVelocity, double gravity) noexcept;

/// What the character can do about a ledge of the given height with today's abilities.
/// There is no vault or mantle yet (Proposal 0009): taller obstacles are Blocked until a later
/// traversal milestone adds moves and thresholds.
enum class ObstacleClass : std::uint8_t {
    StepUp,  ///< height <= maxStepHeight: walking up is enough.
    Jump,    ///< above the step height, within the jump height.
    Blocked, ///< above both.
};

[[nodiscard]] const char* ObstacleClassName(ObstacleClass c) noexcept;

/// `height` in m, >= 0 (negative is treated as 0).
[[nodiscard]] ObstacleClass ClassifyObstacle(const MovementConfig& config, double height) noexcept;

} // namespace spn::core
