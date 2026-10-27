#pragma once

// Traversal foundations (overnight work order M2-A.2, package 12): which move fits an obstacle, and the phases of a move.
// Rules only - NO animation, NO collision sweeps, NO movement. Vault / Mantle / Climb are not playable until an Unreal
// integration exists (root motion or motion warping, collision checks); this header only fixes the vocabulary and the
// validation rules so that integration has something testable to call.
//
// Units: metres, seconds. Every threshold is a PLACEHOLDER TEST VALUE, not a gameplay decision (CLAUDE.md section 3).
// ENGINEERING PROPOSAL, not master canon.

#include <cstdint>
#include <optional>
#include <string>

#include <spn/core/Movement.h>

namespace spn::core {

enum class TraversalMove : std::uint8_t {
    None = 0,  ///< Nothing in the way (height 0 or below).
    StepUp,    ///< Walk up: height <= movement step height.
    Jump,      ///< An active jump may clear it (height below the jump apex); not guaranteed.
    Vault,     ///< Pass OVER a thin obstacle at speed (hand on top, legs swing across).
    Mantle,    ///< Pull up onto a ledge deep enough to stand on.
    Climb,     ///< Taller than a mantle: needs both hands, slow, only where allowed.
    Blocked,   ///< No ability of today overcomes it.
};

inline constexpr std::size_t kTraversalMoveCount = 7;

[[nodiscard]] const char* TraversalMoveName(TraversalMove move) noexcept;

struct TraversalConfig {
    // Vault: thin obstacle, needs a run-up.
    double vaultMaxHeight = 1.20;    ///< m
    double vaultMaxDepth = 0.80;     ///< m, thickness along the travel direction
    double vaultMinSpeed = 2.0;      ///< m/s horizontal approach speed
    // Mantle: pull up onto a ledge the character can stand on.
    double mantleMaxHeight = 1.90;   ///< m
    double mantleMinLedgeDepth = 0.60; ///< m, at least the capsule diameter (0.84 m) is NOT required: feet may overhang
    double standingHeadroom = 1.90;  ///< m of free space above the ledge the character needs afterwards
    // Climb: the tallest move.
    double climbMaxHeight = 3.00;    ///< m
    double climbMaxApproachSpeed = 3.0; ///< m/s, faster approaches are refused (no running into a wall)
};

[[nodiscard]] std::optional<std::string> Validate(const TraversalConfig& config);

/// What stands in the way, measured by the (future) Unreal sweep. Values < 0 are treated as 0; NaN is invalid.
struct TraversalObstacle {
    double height = 0.0;          ///< m above the floor the character stands on
    double depth = 0.0;           ///< m along the travel direction
    double headroom = 100.0;      ///< m of free space above the top (large = open sky)
    double approachSpeed = 0.0;   ///< m/s horizontal speed towards the obstacle
    bool climbable = false;       ///< level design / surface says climbing is allowed here
};

struct TraversalDecision {
    TraversalMove move = TraversalMove::Blocked;
    const char* reason = "";
};

/// Highest-priority move that fits: StepUp, Jump (before any vault/mantle: the cheapest ability wins), Vault, Mantle, Climb.
/// Deterministic; invalid input (NaN/negative speed treated as 0, NaN height) gives Blocked.
[[nodiscard]] TraversalDecision ClassifyTraversal(const MovementConfig& movement, const TraversalConfig& config, const TraversalObstacle& obstacle) noexcept;

/// Phases of an ACTIVE move. `Approach` (aligning to the obstacle) -> `Active` (the move itself) -> `Recover` (back to normal
/// movement) -> `Done`. A move cannot be cancelled in `Active` (no teleport out of it).
enum class TraversalPhase : std::uint8_t { Idle = 0, Approach, Active, Recover, Done };

/// How a move ended. None while running.
enum class TraversalResult : std::uint8_t { None = 0, Success, Aborted, Failed };

struct TraversalState {
    TraversalMove move = TraversalMove::None;
    TraversalPhase phase = TraversalPhase::Idle;
    double phaseSeconds = 0.0;  ///< time spent in the current phase
    TraversalResult result = TraversalResult::None;
    double cooldown = 0.0;      ///< seconds until a new move may start (set when a move ends: no double trigger)
};

struct TraversalTimings {
    double approach = 0.15;
    double active = 0.7;
    double recover = 0.2;
    double cooldown = 0.3;      ///< after Done
};

/// Only a real traversal move (Vault, Mantle, Climb) can start, only from Idle/Done, and not while falling.
[[nodiscard]] bool CanStartTraversal(const TraversalState& state, TraversalMove move, MovementState current) noexcept;
[[nodiscard]] TraversalState StartTraversal(TraversalMove move) noexcept;
/// Advance by `dt` seconds (non-negative); phases switch when their time is used up. Idle/Done do not change.
[[nodiscard]] TraversalState AdvanceTraversal(const TraversalState& state, const TraversalTimings& timings, double dt) noexcept;
/// Approach -> Done with result Aborted (e.g. input released, target moved). Only possible BEFORE the move itself starts; later
/// phases are returned unchanged (there is no teleport out of an active move).
[[nodiscard]] TraversalState AbortTraversal(const TraversalState& state, const TraversalTimings& timings) noexcept;
/// Active -> Recover with result Failed (the sweep found the path blocked after all): the character is released in a controlled way
/// and ExitMovementState says whether it falls or lands. Other phases are returned unchanged.
[[nodiscard]] TraversalState FailTraversal(const TraversalState& state) noexcept;
/// The movement state after Done: Failed/Aborted give Falling when nothing is below, else Landing; a successful Vault ends in
/// Landing (Falling without ground); a successful Mantle/Climb ends standing on the ledge (Idle, or Falling when the ground check
/// fails). `state` must be Done, otherwise Falling is returned (never trust an unfinished move).
[[nodiscard]] MovementState ExitMovementState(const TraversalState& state, bool groundBelow) noexcept;

/// What the Unreal sweeps measured around the obstacle (all in the character's travel frame). Defaults describe a clear, flat ledge.
struct TraversalProbe {
    TraversalObstacle obstacle;
    double topSurfaceAngleDegrees = 0.0; ///< slope of the top surface (0 = flat)
    double landingClearDistance = 100.0; ///< m of free, walkable floor behind a thin obstacle
    bool capsuleFitsOnTop = true;        ///< a capsule sweep on top of the ledge found no blocker
    bool pathClear = true;               ///< nothing but the obstacle itself between the character and the obstacle face
    double approachAngleDegrees = 0.0;   ///< angle between the travel direction and the obstacle's inward normal (0 = head-on)
    double obstacleWidth = 100.0;        ///< m of obstacle across the travel direction in front of the character (a corner or a post is narrow)
    double landingWidth = 100.0;         ///< m of free width on the landing / on top, across the travel direction
    double sideClearance = 100.0;        ///< m free beside the obstacle on the tighter side (a wall flush with the ledge leaves ~0)
    double obstacleSpeed = 0.0;          ///< m/s the obstacle itself moves (vehicle, door, platform)
    bool groundValid = true;             ///< the surface behind / on top is walkable and not hazardous
};

struct TraversalSafety {
    double maxTopSurfaceAngle = 44.8;    ///< same as the walkable floor angle
    double minVaultLanding = 1.0;        ///< m of free floor needed behind a vaulted obstacle
    double maxApproachAngle = 45.0;      ///< steeper approaches are refused (no vault sideways along a wall)
    double capsuleDiameter = 0.84;       ///< obstacle and landing must be at least this wide
    double minSideClearance = 0.15;      ///< a vault needs this much room beside the obstacle for the swinging legs
    double maxObstacleSpeed = 0.1;       ///< moving obstacles are refused
};

/// ClassifyTraversal plus the safety rules for Vault / Mantle / Climb: the path must be clear, the approach roughly head-on, a vault
/// needs a free landing, a mantle or climb needs a flat enough top that a capsule fits on. Violations give Blocked with a reason;
/// StepUp, Jump and None are not restricted (they are ordinary movement).
[[nodiscard]] TraversalDecision ClassifyTraversalChecked(const MovementConfig& movement, const TraversalConfig& config,
                                                         const TraversalSafety& safety, const TraversalProbe& probe) noexcept;

/// True while the move owns the character's movement (Approach and Active); normal input must not move it then.
[[nodiscard]] bool TraversalOwnsMovement(const TraversalState& state) noexcept;

} // namespace spn::core
