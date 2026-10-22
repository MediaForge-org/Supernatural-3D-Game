#pragma once

// Interaction: a small, engine-independent mechanism for "an actor uses an object".
// The system holds no state of its own: handlers read WorldFacts and return the
// commands they want applied; the registry applies them with a journal cause.
//
// ENGINEERING PROPOSAL, not master canon (Docs/Proposals/0004). Approved for Phase 0.3:
// D1 (only the verb Use) and D2 (stable ids: [a-z][a-z0-9_.]*, at most 96 chars, a
// technical prototype rule). No co-op authority is assumed: the caller decides who
// may call Interact.

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Result.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

/// D2: maximum length of a stable interactable id (prototype rule, not a format rule).
inline constexpr std::size_t kMaxInteractableIdLength = 96;

/// D2, the single place for the id rule: 1..kMaxInteractableIdLength chars,
/// first [a-z], rest [a-z0-9_.].
[[nodiscard]] bool IsValidInteractableId(std::string_view id) noexcept;

/// Who performs an interaction (Docs/Proposals/0007). A prototype rule, not a design for
/// players, NPCs or co-op: 1..kMaxHolderIdLength chars, first [a-z], rest [a-z0-9_.].
/// Single player uses kDefaultInteractionHolder; co-op later passes the player's own id.
inline constexpr std::size_t kMaxHolderIdLength = 24;
inline constexpr std::string_view kDefaultInteractionHolder = "player.local";
[[nodiscard]] bool IsValidHolderId(std::string_view holder) noexcept;

/// D1: only `Use` exists today. New values are appended, never reordered.
enum class InteractionVerb : std::uint8_t {
    Use,
};

/// Every verb, in enum order. Extend together with the enum (a test checks both agree).
inline constexpr std::array kAllInteractionVerbs{InteractionVerb::Use};

[[nodiscard]] std::string_view ToString(InteractionVerb verb) noexcept;

/// The object being interacted with: what kind it is and its stable id.
struct InteractableRef {
    ContentId kind;  // e.g. placeholder:interactable.door
    std::string id;  // stable id, see IsValidInteractableId
    friend bool operator==(const InteractableRef&, const InteractableRef&) = default;
};

/// Authored initial state from placement data, by state name (e.g. "open" -> false).
/// A handler uses it only while the matching fact is absent.
struct InteractionDefaults {
    std::map<std::string, FactValue, std::less<>> values;
};

// --- Errors -------------------------------------------------------------------

enum class InteractionErrorCode : std::uint8_t {
    InvalidId,           // id (or holder) breaks its id rule
    UnknownKind,         // no handler registered for the kind
    StateCorrupt,        // a fact or default has an unexpected type
    Refused,             // the handler declined; see feedback
    Fact,                // WorldFacts rejected a command; see fact
    DuplicateKind,       // Register: kind already registered (no silent override)
    InvalidRegistration, // Register: empty kind or missing handler function
};

[[nodiscard]] std::string_view ToString(InteractionErrorCode code) noexcept;

struct InteractionError {
    InteractionErrorCode code;
    std::string subject;            // offending id / kind / fact key; empty if n/a
    ContentId feedback;             // Refused: reason for UI/audio; empty otherwise
    std::optional<FactError> fact;  // set for code == Fact
    friend bool operator==(const InteractionError&, const InteractionError&) = default;
};

// --- Handlers -----------------------------------------------------------------

/// What a handler wants to happen. No commands = nothing changes, nothing is journaled.
struct InteractionPlan {
    std::vector<FactCommand> commands;
    ContentId feedback; // empty = none
};

/// Rules for one interactable kind. Both functions are pure: they only read `facts`.
/// The registry has already validated the id and verb before calling them.
struct InteractionHandler {
    using VerbsFn = std::function<Result<std::vector<InteractionVerb>, InteractionError>(
        const WorldFacts&, const InteractableRef&, const InteractionDefaults&)>;
    using PlanFn = std::function<Result<InteractionPlan, InteractionError>(
        const WorldFacts&, const InteractableRef&, InteractionVerb, const InteractionDefaults&)>;

    /// Verbs offered right now. Offered means Interact either succeeds or is Refused.
    /// Optional, for kinds whose rules depend on who acts (Proposal 0007). When set it is
    /// used instead of `plan`; the holder is already validated. Kinds that do not care
    /// (the test door) set only `plan` and are unchanged.
    using HolderPlanFn = std::function<Result<InteractionPlan, InteractionError>(
        const WorldFacts&, const InteractableRef&, InteractionVerb, const InteractionDefaults&,
        std::string_view holder)>;

    VerbsFn availableVerbs;
    PlanFn plan;
    HolderPlanFn planForHolder;
};

/// The result of a successful interaction.
struct InteractionOutcome {
    std::vector<ChangeRecord> changes; // journal records written; empty if nothing changed
    ContentId feedback;                // empty = none
};

/// kind -> handler. Kinds are added additively (mods later); duplicates are rejected.
class InteractionRegistry {
public:
    [[nodiscard]] std::optional<InteractionError> Register(ContentId kind,
                                                           InteractionHandler handler);
    [[nodiscard]] bool IsRegistered(const ContentId& kind) const;

    /// Validates the target, asks its handler for a plan and applies it atomically:
    /// on any error `facts` is unchanged. Commands that would not change the store
    /// (setting the current value, adjusting by 0, removing an absent fact) are skipped,
    /// so an unchanged state writes no journal record.
    [[nodiscard]] Result<InteractionOutcome, InteractionError>
    Interact(WorldFacts& facts, const InteractableRef& target, InteractionVerb verb,
             const InteractionDefaults& defaults, std::string_view cause,
             std::string_view holder = kDefaultInteractionHolder) const;

    [[nodiscard]] Result<std::vector<InteractionVerb>, InteractionError>
    AvailableVerbs(const WorldFacts& facts, const InteractableRef& target,
                   const InteractionDefaults& defaults) const;

private:
    [[nodiscard]] Result<const InteractionHandler*, InteractionError>
    Resolve(const InteractableRef& target) const;

    std::map<ContentId, InteractionHandler> handlers_;
};

} // namespace spn::core
