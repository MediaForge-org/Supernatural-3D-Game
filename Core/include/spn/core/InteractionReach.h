#pragma once

// Reach and actor-state preconditions for interactions (overnight work order M2-A.2 phase 2, package H).
// The M1 InteractionRegistry decides WHAT an interaction does; this decides whether the actor may TRY it: close enough, looking
// roughly at it, not blocked, and in a state that allows using things. Pure functions on numbers measured by the engine side
// (eye position, view direction, one line-of-sight result); no registry change, no state.
//
// All values are PLACEHOLDER TEST VALUES (the Unreal component uses 250 cm / a 15 cm sphere trace today). Units: metres, degrees.
// ENGINEERING PROPOSAL, not master canon.

#include <cstdint>
#include <optional>
#include <string>

#include <spn/core/Vec.h>

namespace spn::core {

using ReachVec3 = Vec3;

struct ReachConfig {
    double maxDistance = 2.5;          ///< m from the eyes to the nearest point of the target
    double maxAngleDegrees = 60.0;     ///< between the view direction and the direction to the target centre
    double maxHeightDifference = 1.5;  ///< m between the eyes and the target centre
    bool requireLineOfSight = true;
};

[[nodiscard]] std::optional<std::string> Validate(const ReachConfig& config);

enum class ReachResult : std::uint8_t { Ok = 0, TooFar, OutOfView, TooHighOrLow, Obstructed, InvalidInput };

[[nodiscard]] const char* ReachResultName(ReachResult result) noexcept;

struct ReachQuery {
    ReachVec3 eye;
    ReachVec3 viewDirection;   ///< need not be normalised; zero = invalid
    ReachVec3 targetCentre;
    double targetRadius = 0.0; ///< m; a big target is easier to reach and to aim at
    bool lineOfSightClear = true;
};

/// Order of the checks (first failure wins): invalid input, height difference, distance (to the surface, i.e. centre distance minus
/// radius), view angle (widened by the target's angular size), line of sight.
[[nodiscard]] ReachResult CheckReach(const ReachConfig& config, const ReachQuery& query) noexcept;

/// Who is trying. Anything that takes the character's hands or attention blocks interactions.
struct ActorInteractState {
    bool incapacitated = false;
    bool traversing = false;   ///< a vault / mantle / climb owns the movement
    bool inVehicle = false;    ///< vehicle interactions are a separate system
    bool inDialogue = false;
};

[[nodiscard]] bool MayInteract(const ActorInteractState& state) noexcept;

} // namespace spn::core
