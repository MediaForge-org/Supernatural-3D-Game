#pragma once

// NPC foundation (overnight work order M2-A.2 phase 2, package I): a small, deterministic, engine-independent core for
// "what is this NPC doing, where does it want to go, what does it notice". NOT an AI: no behaviour trees, schedules, memory,
// relationships or simulation (those belong to the later NPC design in the master plan and are not decided here). It gives the
// future systems three testable pieces: a state machine with legal transitions, a movement intent towards a goal that reuses the
// gait speeds of Movement.h, and a perception test for sight and hearing.
//
// All numbers are PLACEHOLDER TEST VALUES. Units: metres, seconds, degrees. ENGINEERING PROPOSAL, not master canon.

#include <cstdint>
#include <optional>
#include <string>

#include <spn/core/Movement.h>
#include <spn/core/Vec.h>

namespace spn::core {

enum class NpcState : std::uint8_t { Idle = 0, Moving, Interacting, Alert, Incapacitated };

inline constexpr std::size_t kNpcStateCount = 5;

[[nodiscard]] const char* NpcStateName(NpcState state) noexcept;

/// What happened this tick (all derived by the engine side from sweeps, timers and perception).
struct NpcEvents {
    bool hasGoal = false;          ///< a movement goal is set
    bool goalReached = false;
    bool interactionRequested = false; ///< the NPC should use something in reach
    bool interactionFinished = false;
    bool threatPerceived = false;  ///< something dangerous was seen / heard this tick
    bool threatGone = false;       ///< nothing perceived for the calm-down time
    bool incapacitated = false;
    bool recovered = false;
};

/// One transition. Priority: Incapacitated beats everything (and only `recovered` leaves it, into Idle); then a perceived threat
/// (Alert; an Interacting NPC is interrupted); Alert ends only with threatGone. Otherwise Idle <-> Moving follows the goal, and
/// Interacting runs until interactionFinished. Deterministic; the same inputs always give the same state.
[[nodiscard]] NpcState NextNpcState(NpcState current, const NpcEvents& events) noexcept;

/// Which transitions are legal at all (used to validate saved or networked states). Staying in a state is always legal.
[[nodiscard]] bool IsLegalNpcTransition(NpcState from, NpcState to) noexcept;

using NpcPoint = Vec2;

struct NpcMoveIntent {
    NpcPoint direction;            ///< unit vector towards the goal, (0,0) when arrived or invalid
    double targetSpeed = 0.0;      ///< m/s for SteerVelocity: the gait speed, scaled down inside the slow-down radius
    bool arrived = false;
};

struct NpcMoveConfig {
    double arrivalRadius = 0.5;    ///< m
    double slowDownRadius = 3.0;   ///< m: inside it the speed falls linearly to minSpeedFraction * gait speed at the arrival radius
    double minSpeedFraction = 0.3;
};

[[nodiscard]] std::optional<std::string> Validate(const NpcMoveConfig& config);

/// Intent to walk from `position` to `goal` in `gait`. NaN input gives a zero intent (not arrived).
[[nodiscard]] NpcMoveIntent ComputeMoveIntent(const MovementConfig& movement, const NpcMoveConfig& config, NpcPoint position, NpcPoint goal, Gait gait) noexcept;

struct NpcPerceptionConfig {
    double sightRange = 20.0;      ///< m
    double fieldOfViewDegrees = 110.0; ///< total opening angle
    double hearingRange = 8.0;     ///< m, for loud stimuli (scaled by the stimulus loudness)
    double closeSenseRange = 1.5;  ///< m: anything this close is noticed regardless of facing
};

enum class Perception : std::uint8_t { None = 0, Heard, Seen };

struct NpcStimulus {
    NpcPoint position;
    double loudness = 0.0;         ///< 0 = silent, 1 = normal footsteps at a run, 2 = loud
    bool lineOfSightClear = true;
};

/// Seen: inside the sight range and the view cone with a clear line (or within the close range). Heard: within hearingRange *
/// loudness, no line of sight needed. Seen wins over Heard. `facingDegrees` is the NPC's heading (0 = +X, counter-clockwise).
[[nodiscard]] Perception Perceive(const NpcPerceptionConfig& config, NpcPoint position, double facingDegrees, const NpcStimulus& stimulus) noexcept;

} // namespace spn::core
