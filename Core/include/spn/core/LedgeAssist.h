#pragma once

// Ledge assist (developer decision 2026-11-14): a context-dependent move over or onto an obstacle, started ONLY by the explicit
// jump / traversal input while the obstacle is right in front of the character. Three PROVISIONAL height bands (developer):
//   up to 0.40 m      StepOn (step onto it) or a low step-over (Vault) when it is thin
//   0.40 .. 1.20 m    Vault (thin obstacle, run-up, free landing behind) or Mantle (onto a ledge the character can stand on)
//   1.20 .. 1.80 m    ClimbUp (hands on the edge, slow pull-up; ends standing on top)
// Above 1.80 m nothing happens; below the movement step height ordinary walking handles it.
//
// This file decides WHICH move fits what the engine measured (LedgeProbe) and describes the capsule path of the move
// (SampleLedgePath). It contains no engine code; the Unreal movement component measures, checks collisions and moves.
// Units: metres, seconds, metres per second. Every number is a PLACEHOLDER TEST VALUE (CLAUDE.md section 3), not tuning.
//
// Path model (no animation exists yet): the capsule bottom rises with an ease-out curve while the capsule moves forward. The
// lower `legAllowance` of the capsule (the planted leg: one foot on the edge, one still behind) may pass beside the edge; the
// rest of the body (the "upper body", capsule bottom raised by legAllowance) never enters the obstacle. So a low ledge keeps
// most of the run-up, a high one stops the body at the face until the hips are up (as a person would).

#include <cstdint>
#include <optional>
#include <string>

namespace spn::core {

enum class LedgeMove : std::uint8_t {
    None = 0, ///< no assisted move: the input stays an ordinary jump
    StepOn,   ///< low band: step onto the obstacle and keep going
    Vault,    ///< over a thin obstacle (hand on top, legs across), lands behind it
    Mantle,   ///< onto a ledge the character can stand on
    ClimbUp,  ///< high band: hands on the edge, pull up, stand on top
};

[[nodiscard]] const char* LedgeMoveName(LedgeMove move) noexcept;

struct LedgeAssistConfig {
    double stepHeight = 0.25;       ///< m; up to here ordinary walking climbs it (movement MaxStepHeight)
    double lowMax = 0.40;           ///< m; band 1 (developer, provisional)
    double midMax = 1.20;           ///< m; band 2 (developer, provisional)
    double highMax = 1.80;          ///< m; band 3 (developer, provisional)
    double reachStanding = 0.30;    ///< m between the capsule front and the face at which a standing press starts a move
    double reachPerSpeed = 0.08;    ///< s; the reach grows with the approach speed (the step before the obstacle)
    double reachMax = 0.60;         ///< m; pressed further away the input is an ordinary jump
    double minGripDepth = 0.15;     ///< m of top surface the hands / the foot need behind the edge
    double maxApproachAngle = 45.0; ///< degrees between the travel direction and the face normal
    double maxTopAngle = 44.8;      ///< degrees; the top must be walkable
    double vaultMaxDepth = 0.80;    ///< m; thicker obstacles are mantled, never vaulted
    double vaultMinSpeed = 2.0;     ///< m/s run-up for a vault in the middle band (the low step-over needs none)
    double maxVaultDrop = 1.0;      ///< m the floor behind may lie below the floor in front
    double maxVaultRise = 0.25;     ///< m the floor behind may lie above the floor in front
    double legAllowance = 0.35;     ///< m of the capsule bottom that may pass beside the edge while rising (the planted leg)
    double clearance = 0.03;        ///< m the capsule bottom ends above the top
    double overEndMargin = 0.05;    ///< m the capsule's back ends past the face (Mantle/StepOn/ClimbUp) or past the far side (Vault)
    double minForwardSpeed = 0.8;   ///< m/s forward drift while rising from a standstill (towards the face)
    double minOverSpeed = 1.0;      ///< m/s onto the top when there is no run-up
};

[[nodiscard]] std::optional<std::string> Validate(const LedgeAssistConfig& config);

/// What the engine measured in front of the character along the travel direction (all metres / degrees / m/s).
struct LedgeProbe {
    bool faceFound = false;        ///< a blocking, near-vertical face within reach
    double faceDistance = 0.0;     ///< capsule front to the face
    double approachAngle = 0.0;    ///< degrees between the travel direction and the face's inward normal
    double approachSpeed = 0.0;    ///< horizontal speed towards the face
    bool surfaceAllowed = true;    ///< the hit object may be stood on (not a pawn, not a moving / simulated object)
    bool topFound = false;         ///< a top surface found from above behind the edge (not inside geometry)
    double height = 0.0;           ///< top above the floor the character stands on
    double topAngle = 0.0;         ///< slope of the top, degrees
    double depth = 0.0;            ///< continuous top along the travel direction from the face (capped by the probe)
    bool liftClear = true;         ///< the capsule can rise in place by height + clearance (no ceiling)
    bool topClear = true;          ///< the capsule fits standing on top at the end pose and the way there is free
    bool landingFound = false;     ///< walkable floor behind a thin obstacle
    double landingRise = 0.0;      ///< floor behind minus floor in front (negative = lower)
    bool landingClear = false;     ///< the way across at top height and the landing pose are free
};

struct LedgePlan {
    LedgeMove move = LedgeMove::None;
    const char* reason = "";
    double height = 0.0;           ///< m, top above the start floor
    double depth = 0.0;            ///< m, measured top depth
    double radius = 0.42;          ///< m, capsule radius
    double faceAxisDistance = 0.0; ///< m, capsule axis to the face at the start (faceDistance + radius)
    double approachSpeed = 0.0;    ///< m/s
    double liftSeconds = 0.0;      ///< rising phase
    double overSeconds = 0.0;      ///< onto the top / across at top height
    double liftForwardSpeed = 0.0; ///< m/s forward while rising (before the geometric limit)
    double overSpeed = 0.0;        ///< m/s forward over the top
    double exitSpeed = 0.0;        ///< m/s horizontal speed handed back to normal movement
    double riseHeight = 0.0;       ///< m the capsule bottom rises (height + clearance)
    double forwardEnd = 0.0;       ///< m the capsule axis travels in total
    double legAllowance = 0.35;
    bool endsOnTop = false;        ///< Mantle / StepOn / ClimbUp end standing on top; Vault ends in the air behind it
    [[nodiscard]] double TotalSeconds() const noexcept { return liftSeconds + overSeconds; }
};

/// The move for this measurement, or None with the reason. Deterministic; NaN or negative measurements give None.
[[nodiscard]] LedgePlan PlanLedgeAssist(const LedgeAssistConfig& config, const LedgeProbe& probe, double capsuleRadius) noexcept;

/// Reach in front of the capsule (m) within which a press starts a move, for an approach speed (m/s).
[[nodiscard]] double LedgeReach(const LedgeAssistConfig& config, double approachSpeed) noexcept;

/// Horizontal speed kept by a move (share of the approach speed) in a band; also the forward speed while rising.
[[nodiscard]] double LedgeSpeedRetention(const LedgeAssistConfig& config, LedgeMove move, double height) noexcept;

struct LedgePathPoint {
    double forward = 0.0; ///< m the capsule axis moved along the travel direction
    double up = 0.0;      ///< m the capsule bottom rose above the start floor
    bool done = false;    ///< t reached the end of the move
};

/// Capsule position at `seconds` after the start (clamped to the move). Monotonic in both coordinates.
[[nodiscard]] LedgePathPoint SampleLedgePath(const LedgePlan& plan, double seconds) noexcept;

/// Smallest allowed distance (m) between the capsule axis and the face for a capsule bottom at `up` (the upper-body rule above).
[[nodiscard]] double LedgeRequiredAxisDistance(const LedgePlan& plan, double up) noexcept;

} // namespace spn::core
