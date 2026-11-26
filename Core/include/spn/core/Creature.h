#pragma once
// Creature (NightBuild07, first case "Die Stille Mühle", Docs/Proposals/0019): the deterministic decision core of a BLIND creature that hunts by
// sound. It follows the planned perception rules of Docs/Game/Kreaturen/10 section 1 in their smallest form:
//   * perception with uncertainty instead of the player's exact position: the brain only ever knows where a NOISE was, or that something
//     touches it (contact range); it never receives the player position as such, except for the catch check, which needs real proximity;
//   * search phases: pursuit (Chase) -> local search (Search, the search radius grows with time, the creature loses accuracy, not the fact that
//     something was there) -> wary patrol (more sensitive for a while);
//   * difficulty changes durations (search time) and the reaction delay before a pursuit (the player's head start), never the knowledge itself.
// The weakness is a MECHANIC, not bonus damage (DECISIONS.md S-08): a loud bell stuns the creature for a while.
//
// VOLATILE: nothing of this is saved. After a load or a death the creature starts again from its patrol (TOD-01: back to the checkpoint).
// All numbers are PLACEHOLDER gameplay parameters, not master canon. Units: metres, seconds.
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <spn/core/Vec.h>

namespace spn::core {

enum class CreatureState : std::uint8_t { Patrol = 0, Investigate, Chase, Search, Stunned };
inline constexpr std::size_t kCreatureStateCount = 5;
[[nodiscard]] std::string_view ToString(CreatureState state) noexcept;

/// How the creature moves in a state (the engine maps it to a speed).
enum class CreaturePace : std::uint8_t { Still = 0, Prowl, Hunt };
[[nodiscard]] std::string_view ToString(CreaturePace pace) noexcept;

struct CreatureConfig {
    double chaseStrength = 0.75;      ///< a heard noise at least this strong starts a pursuit (shots, impacts, the bell) ...
    double chaseHearDistance = 25.0;  ///< ... if it is not farther than this
    double closeHearDistance = 8.0;   ///< while investigating / searching / chasing, ANY heard noise this close is treated as the prey
    double contactRange = 1.5;        ///< something this close is felt, even when it makes no sound
    double catchRange = 1.1;          ///< in pursuit: the prey this close is caught
    double reactionDelay = 1.0;       ///< seconds the creature listens before a pursuit starts (difficulty: the head start)
    double chaseLoseSeconds = 4.0;    ///< pursuit without a new noise or contact for this long, with the last target reached: search
    double chaseGiveUpSeconds = 12.0; ///< ... and without a new noise for this long even if the target was NOT reached (a closed door in the way)
    double searchSeconds = 20.0;      ///< local search duration (difficulty)
    double searchRadiusStart = 2.0;   ///< m around the last known position
    double searchRadiusGrowth = 0.5;  ///< m per second: the last known position gets less exact
    double searchRadiusMax = 12.0;
    double investigateSeconds = 10.0; ///< time spent at an investigated noise before returning to the patrol
    double stunSeconds = 8.0;
    double waryHearingScale = 1.5;    ///< hearing range factor while wary (after a search)
    double warySeconds = 60.0;
    double arrivalRadius = 1.0;
};
[[nodiscard]] std::optional<std::string> Validate(const CreatureConfig& config);

struct CreatureNoise {
    Vec2 position;
    double strength = 0.0; ///< 0..1, as emitted
    double distance = 0.0; ///< m from the creature (the engine side has already decided that it was heard at all)
};

struct CreatureInput {
    double dt = 0.0;
    Vec2 self;
    std::optional<CreatureNoise> noise; ///< at most one heard noise this tick (the loudest)
    /// Distance to the prey (the player) in m, if the engine side has one; used for contact and catch only (a blind creature cannot see it).
    std::optional<double> preyDistance;
    Vec2 preyPosition;                  ///< only meaningful with preyDistance
};

struct CreatureOutput {
    CreatureState state = CreatureState::Patrol;
    CreatureState previous = CreatureState::Patrol;
    bool changed = false;
    bool listening = false;   ///< it heard its prey and waits reactionDelay before the pursuit (the player's head start)
    bool caught = false;      ///< exactly once per catch
    bool hasGoal = false;     ///< false in Patrol: the engine runs the patrol route
    Vec2 goal;
    double searchRadius = 0.0; ///< Search: radius around `goal` the engine picks search points in
    CreaturePace pace = CreaturePace::Prowl;
};

class CreatureBrain {
public:
    explicit CreatureBrain(CreatureConfig config = {}) : config_(config) {}

    CreatureOutput Update(const CreatureInput& in);
    /// The bell: stunned for config.stunSeconds from any state. Returns false if already stunned (the timer restarts anyway).
    bool Stun();
    /// Back to the start of its routine (after a death / load): Patrol, not wary, no target.
    void Reset();

    [[nodiscard]] CreatureState State() const noexcept { return state_; }
    [[nodiscard]] double TimeInState() const noexcept { return timeInState_; }
    [[nodiscard]] bool Wary() const noexcept { return waryLeft_ > 0.0; }
    /// The hearing range factor the engine applies to noises (wary: larger).
    [[nodiscard]] double HearingScale() const noexcept { return Wary() ? config_.waryHearingScale : 1.0; }
    [[nodiscard]] Vec2 Target() const noexcept { return target_; }
    [[nodiscard]] std::uint64_t TransitionCount() const noexcept { return transitions_; }
    [[nodiscard]] const CreatureConfig& Config() const noexcept { return config_; }

private:
    void Enter(CreatureState next, CreatureOutput& out);

    CreatureConfig config_;
    CreatureState state_ = CreatureState::Patrol;
    double timeInState_ = 0.0;
    double sinceContact_ = 0.0;   ///< Chase: since the last noise / contact that confirmed the prey
    double listenLeft_ = -1.0;    ///< >= 0 while listening before a pursuit
    double waryLeft_ = 0.0;
    bool caughtThisChase_ = false;
    Vec2 target_;
    Vec2 pendingTarget_;
    std::uint64_t transitions_ = 0;
};

/// The bell reaches a creature this far away (m); walls between halve the range each (the engine counts them). Non-finite input: false.
[[nodiscard]] bool BellReaches(double distance, double bellRange, int walls) noexcept;

/// Saving is refused while any creature is hunting the player (DECISIONS.md TOD-03: a save in a pursuit could trap the player).
[[nodiscard]] bool SaveAllowed(const std::vector<CreatureState>& creatures) noexcept;

} // namespace spn::core
