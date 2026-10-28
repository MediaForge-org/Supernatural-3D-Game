#pragma once

// InteractionSystem (V2): deterministic target selection and structured interaction requests on top of InteractionRegistry (rules per kind),
// WorldFacts (state) and GameEventBus (events). It adds what the registry deliberately does not know:
//   * where interactables are, which one the actor means (selection with priority) and whether the actor can reach it,
//   * enabled/disabled targets, per-target cooldowns (caller supplied clock), engine-side blocking rules (e.g. an obstructed door),
//   * a closed set of result classes (InteractStatus) instead of free text,
//   * duplicate/reentrant execution protection,
//   * events for attempts, results and state changes, and explicit links between objects (switch -> door lock).
// It never interacts by itself: every state change starts with an explicit Interact call.
//
// Lifetime: the system holds references to the registry, the facts and the bus; they must outlive it. Single threaded, no globals.
// ENGINEERING FOUNDATION (M2A3), placeholder kinds, not master canon.

#include <cstdint>
#include <functional>
#include <map>
#include <unordered_map>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/GameEvents.h>
#include <spn/core/Interaction.h>
#include <spn/core/InteractionReach.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

inline constexpr int kMaxLinkDepth = 16;
inline constexpr double kMaxWorldCoordinate = 1.0e7; ///< m; positions beyond this are rejected (grid keys stay in range)
inline constexpr double kInteractionGridCell = 4.0;  ///< m

/// Prefix of all fact keys of an object: `<namespace>.<last kind segment>.<id>.` , e.g. placeholder.door.front_door. (the existing convention).
/// Empty for an empty kind or an invalid id.
[[nodiscard]] std::string ObjectFactPrefix(const ContentId& kind, std::string_view id);

/// The fact field whose change counts as "the state of the object changed" (links and events): door open, switch on, button presses,
/// lever position. Empty for other kinds.
[[nodiscard]] std::string_view PrimaryStateField(const ContentId& kind) noexcept;

/// Maps a Refused feedback id to a status by the last path segment: locked, disabled, blocked/cooldown, exhausted/already.
[[nodiscard]] InteractStatus ClassifyRefusal(const ContentId& feedback) noexcept;

[[nodiscard]] const ContentId& InteractionCooldownFeedback();   // placeholder:feedback.interaction.cooldown
[[nodiscard]] const ContentId& InteractionReentrantFeedback();  // placeholder:feedback.interaction.reentrant
[[nodiscard]] const ContentId& InteractionActorStateFeedback(); // placeholder:feedback.interaction.actor_state

struct InteractableDescriptor {
    std::string id;                  ///< IsValidInteractableId
    ContentId kind;                  ///< must be registered in the registry
    ReachVec3 centre;                ///< m, finite
    double radius = 0.3;             ///< m, >= 0, finite; bigger targets are easier to reach and aim at
    int priority = 0;                ///< higher wins when several are reachable (then smaller angle, distance, id)
    InteractionDefaults defaults;    ///< authored configuration, never written back
    bool enabled = true;             ///< runtime switch; a disabled target is still selectable but answers Disabled
    std::int64_t cooldownMs = 0;     ///< minimum time between two successful uses, >= 0
};

enum class SystemErrorCode : std::uint8_t {
    InvalidId, DuplicateId, UnknownKind, InvalidNumber, UnknownObject, InvalidLink, DuplicateLink, LinkCycle,
};
[[nodiscard]] std::string_view ToString(SystemErrorCode code) noexcept;
struct SystemError {
    SystemErrorCode code;
    std::string subject;
    friend bool operator==(const SystemError&, const SystemError&) = default;
};

struct ActorView {
    std::string actor{kDefaultInteractionHolder};
    ReachVec3 eye;
    ReachVec3 viewDirection{1.0, 0.0, 0.0};
    ActorInteractState state;
};

struct InteractionRequest {
    ActorView actor;
    std::string targetId;
    InteractionVerb verb = InteractionVerb::Use;
    std::int64_t nowMs = 0;       ///< caller clock, monotonic non-decreasing per system; used for cooldowns only
    bool requireReach = true;     ///< false: scripted/mechanism use that does not depend on the actor's position
};

struct InteractionResult {
    InteractStatus status = InteractStatus::InvalidInput;
    std::string targetId;
    ContentId feedback;
    std::vector<ChangeRecord> changes;  ///< all journal records written, including the ones caused by links
    std::string detail;                 ///< short diagnostic text, no semantics
};

/// Links "when <source> reaches state X, set <target key> to V". Both objects must be registered, the target key must belong to the
/// target object (ObjectFactPrefix), the link graph must stay acyclic and a (source, target, key) combination exists once.
struct Link {
    std::string source;
    std::optional<std::int64_t> whenState; ///< nullopt = on every change of the source's primary state
    std::string target;
    std::string targetKey;
    FactValue value;
    std::optional<FactValue> otherwise;    ///< applied when the source changes to a state other than whenState
};

/// BlockRule for doors: closing is blocked while `swingObstructed(door id)` is true (the engine overlap query of the door's swing area
/// and closing path: a capsule/box test against pawns). Opening is never blocked by it. An invalid or missing function blocks nothing.
/// The core cannot decide this itself; see the Unreal adapter (ASpnTestDoor) for the query and the Windows verification note.
[[nodiscard]] std::function<bool(const WorldFacts&, const InteractableRef&, const InteractionDefaults&)>
MakeDoorCloseObstructionRule(std::function<bool(std::string_view doorId)> swingObstructed);

class InteractionSystem {
public:
    using LineOfSightFn = std::function<bool(const InteractableDescriptor&)>;
    /// true = the transition the facts would make is blocked right now by something only the engine knows (an actor in the door's swing).
    using BlockRule = std::function<bool(const WorldFacts&, const InteractableRef&, const InteractionDefaults&)>;

    InteractionSystem(const InteractionRegistry& registry, WorldFacts& facts, GameEventBus* bus = nullptr, ReachConfig reach = {});

    [[nodiscard]] std::optional<SystemError> AddInteractable(InteractableDescriptor descriptor);
    bool RemoveInteractable(std::string_view id);
    /// False for an unknown id.
    bool SetEnabled(std::string_view id, bool enabled);
    [[nodiscard]] const InteractableDescriptor* Find(std::string_view id) const;
    [[nodiscard]] std::size_t Count() const noexcept { return objects_.size(); }

    void SetLineOfSight(LineOfSightFn fn) { lineOfSight_ = std::move(fn); }
    /// Replaces the rule of a kind (empty function removes it).
    void SetBlockRule(const ContentId& kind, BlockRule rule);

    /// The target the actor means, or nullopt. Deterministic: reachable candidates ordered by priority (high first), angle, distance, id.
    [[nodiscard]] std::optional<std::string> SelectTarget(const ActorView& actor) const;
    /// All reachable candidate ids in selection order (for prompts and tests).
    [[nodiscard]] std::vector<std::string> ReachableTargets(const ActorView& actor) const;

    [[nodiscard]] InteractionResult Interact(const InteractionRequest& request);

    [[nodiscard]] std::optional<SystemError> AddLink(Link link);
    [[nodiscard]] std::size_t LinkCount() const noexcept { return links_.size(); }

    /// Forgets cooldown bookkeeping (runtime-only data; call after a world load).
    void ResetRuntimeState() { lastUseMs_.clear(); }

private:
    struct Candidate;
    [[nodiscard]] std::vector<Candidate> Rank(const ActorView& actor) const;
    [[nodiscard]] std::vector<Candidate> RankScan(const ActorView& actor, double viewLength) const;
    [[nodiscard]] std::vector<Candidate> RankCandidates(const ActorView& actor, double viewLength, const std::vector<const InteractableDescriptor*>& pool) const;
    void PublishChange(const ChangeRecord& change, const InteractableDescriptor& object, const std::string& actor);
    void RunLinks(const InteractableDescriptor& source, const ChangeRecord& change, std::vector<ChangeRecord>& out, int depth);
    [[nodiscard]] bool Reaches(std::string_view from, std::string_view to) const;

    const InteractionRegistry& registry_;
    WorldFacts& facts_;
    GameEventBus* bus_;
    ReachConfig reach_;
    std::map<std::string, InteractableDescriptor, std::less<>> objects_;
    std::map<ContentId, BlockRule> blockRules_;
    std::unordered_map<std::uint64_t, std::vector<const InteractableDescriptor*>> grid_; // broad phase: 4 m cells on x/y, nodes of objects_ are address-stable
    double maxRadius_ = 0.0;
    std::vector<Link> links_;
    std::map<std::string, std::int64_t, std::less<>> lastUseMs_;
    LineOfSightFn lineOfSight_;
    bool busy_ = false;
};

} // namespace spn::core
