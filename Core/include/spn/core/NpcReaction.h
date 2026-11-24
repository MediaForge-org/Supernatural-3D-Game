#pragma once

// NpcReaction (NightBuild05): the small, deterministic core of "an NPC notices something, reacts, remembers it".
//
//   * Gameplay noises: position, audible range, strength (0..1) and a kind that separates HARMLESS from SUSPICIOUS events. Hearing is attenuated
//     by solid walls (the engine side counts them); sight needs a clear line (no exception for "very close").
//   * NpcReactionBrain: Idle -> Investigate -> Alert -> Flee -> ReturnToRoutine with explicit priorities, minimum dwell times, timeouts and
//     de-duplication of repeated noises, so an NPC can never flip between states without a new event (see the tests: with no input the brain
//     always settles in Idle in bounded time).
//   * Memory: incidents are written into the EXISTING NpcMemoryStore (no second memory system, no change of the SPN-NPCMEM 1 format). Wariness is
//     derived from those memories and changes later behaviour (harmless noises are investigated, sighting escalates faster). It is deliberately
//     NOT a nemesis system: there is no rivalry, no growth, no faction state.
//
// VOLATILE vs PERSISTENT: the brain state (current reaction, timers, goals) is NEVER saved. What is saved are the memories. After a load the brain is
// re-derived from them (InitialStateFromMemory).
//
// All numbers are PLACEHOLDER gameplay parameters, not master canon. Units: metres, seconds.

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include <spn/core/Movement.h>
#include <spn/core/Npc.h>
#include <spn/core/NpcMemory.h>
#include <spn/core/Vec.h>

namespace spn::core {

// ------------------------------------------------------------------ noises and senses

enum class NoiseKind : std::uint8_t { Footstep = 0, DoorUse, DoorBreak, Impact, Shout };
inline constexpr std::size_t kNoiseKindCount = 5;
[[nodiscard]] std::string_view ToString(NoiseKind kind) noexcept;
/// DoorBreak, Impact and Shout (an NPC raising the alarm) are suspicious; footsteps and normal door use are harmless. A harmless noise that is very loud is suspicious as well.
[[nodiscard]] bool IsSuspicious(NoiseKind kind, double strength, double suspiciousStrength = 0.75) noexcept;

struct GameplayNoise {
    Vec2 position;
    double range = 0.0;    ///< m: audible radius at sensitivity 1 and no walls in between
    double strength = 0.0; ///< 0..1: how loud / alarming (also the severity written to memory)
    NoiseKind kind = NoiseKind::Footstep;
};

struct HearingConfig {
    double sensitivity = 1.0;       ///< scales the audible range of this listener
    double wallAttenuation = 0.5;   ///< range factor per solid wall between source and listener
};

struct HearingResult {
    bool heard = false;
    double distance = 0.0;
    double effectiveRange = 0.0;
};
/// Non-finite input or negative values: not heard. `walls` = number of solid obstacles between source and listener (counted by the engine side).
[[nodiscard]] HearingResult CanHear(const HearingConfig& config, const GameplayNoise& noise, Vec2 listener, int walls) noexcept;

/// Sight: inside the sight range and the view cone with a CLEAR line. Without a clear line nothing is seen at any distance.
[[nodiscard]] bool CanSee(const NpcPerceptionConfig& config, Vec2 eye, double facingDegrees, Vec2 target, bool lineClear) noexcept;

// ------------------------------------------------------------------ reaction state machine

enum class NpcReactionState : std::uint8_t { Idle = 0, Investigate, Alert, Flee, ReturnToRoutine };
inline constexpr std::size_t kNpcReactionStateCount = 5;
[[nodiscard]] std::string_view ToString(NpcReactionState state) noexcept;

struct NpcReactionConfig {
    double minDwell = 0.75;           ///< a state is not LEFT downwards (calming down) earlier; escalations ignore it
    double lookSeconds = 4.0;         ///< time spent searching at the investigation spot
    double investigateTimeout = 25.0; ///< whole investigation (walking + searching), measured from the first noise of the episode
    double alertHoldSeconds = 6.0;    ///< Alert lasts this long after the player was last seen
    double fleeTriggerDistance = 6.0; ///< in Alert: a visible player this close ...
    double fleeTriggerSeconds = 1.5;  ///< ... for this long makes the NPC flee
    double fleeTriggerSecondsWary = 0.5; ///< ... and for this long when the NPC is wary (remembers something suspicious): faster, but the Alert state is still visible
    double fleeSeconds = 10.0;        ///< minimum flee duration
    double safeDistance = 15.0;       ///< flee may end when the player is farther away (or out of sight)
    double returnTimeout = 40.0;      ///< walking back takes at most this long
    double arrivalRadius = 1.0;
    double sameNoiseRadius = 3.0;     ///< a suspicious noise this close to the current target only refreshes it
    double noiseReactionCooldown = 1.0; ///< at most one noise-driven (re)targeting per this interval
};
[[nodiscard]] std::optional<std::string> Validate(const NpcReactionConfig& config);

struct HeardNoise {
    Vec2 position;
    double severity = 0.0; ///< 0..1
    bool suspicious = false;
};

struct ReactionInput {
    double dt = 0.0;
    Vec2 self;
    Vec2 home;                  ///< routine position the NPC returns to
    bool seesPlayer = false;
    Vec2 playerPosition;        ///< only meaningful with seesPlayer
    std::optional<HeardNoise> noise; ///< at most one new audible noise this tick
    bool wary = false;          ///< derived from memory (AssessWariness)
};

struct ReactionOutput {
    NpcReactionState state = NpcReactionState::Idle;
    NpcReactionState previous = NpcReactionState::Idle;
    bool changed = false;
    bool raiseAlarm = false;    ///< true exactly once per episode, on the tick the NPC first spots the player while suspicious
    bool hasGoal = false;
    Vec2 goal;
    Gait gait = Gait::Walk;
    bool facePlayer = false;
    bool fleeAway = false;      ///< Flee: move away from `threat`
    Vec2 threat;
};

class NpcReactionBrain {
public:
    explicit NpcReactionBrain(NpcReactionConfig config = {}) : config_(config) {}

    ReactionOutput Update(const ReactionInput& in);
    [[nodiscard]] NpcReactionState State() const noexcept { return state_; }
    [[nodiscard]] double TimeInState() const noexcept { return timeInState_; }
    [[nodiscard]] std::uint64_t TransitionCount() const noexcept { return transitions_; }
    /// Put the brain into a state without running a transition (after loading). Timers restart.
    void Restore(NpcReactionState state, Vec2 target);
    [[nodiscard]] Vec2 Target() const noexcept { return target_; }

private:
    void Enter(NpcReactionState next, ReactionOutput& out);
    NpcReactionConfig config_;
    NpcReactionState state_ = NpcReactionState::Idle;
    double timeInState_ = 0.0;
    double episodeTime_ = 0.0;      ///< since the first suspicious noise of the episode (0 in Idle)
    double sinceSeen_ = 1e9;
    double closeTime_ = 0.0;
    double lookTimer_ = 0.0;
    double sinceNoiseReaction_ = 1e9;
    bool suspicious_ = false;       ///< this episode started with something suspicious
    bool alarmRaised_ = false;
    Vec2 target_;
    Vec2 lastPlayer_;
    std::uint64_t transitions_ = 0;
};

// ------------------------------------------------------------------ memory

/// Entity id of a world object (door ...) for use as the `subject` of a memory: FNV-1a of the id with a reserved high bit, never colliding with
/// NPC ids that stay below 2^62. Stable across runs and platforms.
[[nodiscard]] EntityId ObjectEntityId(std::string_view objectId) noexcept;
[[nodiscard]] bool IsObjectEntity(EntityId id) noexcept;
/// Actor used when an NPC heard something but does not know who caused it (a belief must not contain knowledge the NPC does not have).
[[nodiscard]] EntityId UnknownActor() noexcept;

enum class IncidentKind : std::uint8_t {
    HeardSuspicious, ///< Witnessed, actor unknown
    HeardDoorBroken, ///< Witnessed, actor unknown, subject = the door object
    SawPlayer,       ///< Encounter, actor = player
    Alarmed,         ///< Threatened, actor = player: the NPC raised the alarm or fled
};
struct Incident {
    IncidentKind kind = IncidentKind::HeardSuspicious;
    double time = 0.0;
    Vec2 where;
    double severity = 0.5;      ///< 0..1
    EntityId object;            ///< HeardDoorBroken: ObjectEntityId(door)
    double accuracy = 1.0;      ///< heard from afar / through walls is less certain
};
/// Writes the incident into the memory of `npc` (via NpcMemoryStore::RecordEvent). The NPC must be registered.
[[nodiscard]] std::optional<NpcMemoryError> RememberIncident(NpcMemoryStore& store, EntityId npc, EntityId player, const Incident& incident);

struct WarinessConfig {
    double waryAt = 0.3;           ///< suspicion at or above this = wary
    double resumeWindow = 120.0;   ///< an alarm this recent (game seconds) is "still going on" after a load
};
struct NpcWariness {
    double suspicion = 0.0;        ///< 0..1, decays with the memory
    bool wary = false;
    bool recentAlarm = false;
    bool brokeInRemembered = false;
    bool playerSeenRemembered = false;
    std::optional<Vec2> lastIncident;
    double lastIncidentTime = 0.0;
};
[[nodiscard]] NpcWariness AssessWariness(const NpcMemoryStore& store, EntityId npc, EntityId player, double now, const WarinessConfig& config = {});

/// Initial state of the (volatile) brain after a load: Investigate towards the last incident if an alarm is still recent, otherwise Idle.
struct RestoredReaction {
    NpcReactionState state = NpcReactionState::Idle;
    Vec2 target;
};
[[nodiscard]] RestoredReaction InitialStateFromMemory(const NpcWariness& wariness);

} // namespace spn::core
