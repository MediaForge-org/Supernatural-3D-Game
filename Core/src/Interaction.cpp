#include <spn/core/Interaction.h>
#include <algorithm>
#include <utility>
#include <variant>

namespace spn::core {

namespace {

InteractionError MakeError(InteractionErrorCode code, std::string subject) {
    return InteractionError{code, std::move(subject), {}, std::nullopt};
}

/// True if applying `command` would leave the store as it is.
bool IsNoOp(const WorldFacts& facts, const FactCommand& command) {
    if (const auto* set = std::get_if<SetFact>(&command)) {
        const FactValue* current = facts.Find(set->key);
        return current != nullptr && *current == set->value;
    }
    if (const auto* remove = std::get_if<RemoveFact>(&command)) {
        return facts.Find(remove->key) == nullptr;
    }
    const auto& adjust = std::get<AdjustInt>(command);
    // A zero delta on a missing or non-integer fact is still an error: let Apply report it.
    return adjust.delta == 0 && facts.FindAs<std::int64_t>(adjust.key) != nullptr;
}

/// Applies the commands in order. On success returns the journal records.
Result<std::vector<ChangeRecord>, InteractionError>
ApplyAll(WorldFacts& facts, const std::vector<FactCommand>& commands, std::string_view cause) {
    std::vector<ChangeRecord> changes;
    for (const FactCommand& command : commands) {
        if (IsNoOp(facts, command)) {
            continue;
        }
        auto applied = facts.Apply(command, cause);
        if (!applied) {
            InteractionError error = MakeError(InteractionErrorCode::Fact, {});
            error.fact = applied.Error();
            return error;
        }
        changes.push_back(std::move(applied).Value());
    }
    return changes;
}

} // namespace

bool IsValidInteractableId(std::string_view id) noexcept {
    if (id.empty() || id.size() > kMaxInteractableIdLength) {
        return false;
    }
    const auto isLower = [](char ch) { return ch >= 'a' && ch <= 'z'; };
    if (!isLower(id.front())) {
        return false;
    }
    for (const char ch : id) {
        const bool isDigit = ch >= '0' && ch <= '9';
        if (!isLower(ch) && !isDigit && ch != '_' && ch != '.') {
            return false;
        }
    }
    return true;
}

bool IsValidHolderId(std::string_view holder) noexcept {
    // Same grammar as interactable ids, with the shorter holder limit.
    return holder.size() <= kMaxHolderIdLength && IsValidInteractableId(holder);
}

std::string_view ToString(InteractionVerb verb) noexcept {
    switch (verb) {
    case InteractionVerb::Use:
        return "use";
    case InteractionVerb::Force:
        return "force";
    }
    return "unknown";
}

const ContentId& UnsupportedVerbFeedback() {
    static const ContentId feedback = *ContentId::Parse("placeholder:feedback.verb.unsupported");
    return feedback;
}

std::string_view ToString(InteractionErrorCode code) noexcept {
    switch (code) {
    case InteractionErrorCode::InvalidId:
        return "invalid id";
    case InteractionErrorCode::UnknownKind:
        return "unknown kind";
    case InteractionErrorCode::StateCorrupt:
        return "state corrupt";
    case InteractionErrorCode::Refused:
        return "refused";
    case InteractionErrorCode::Fact:
        return "fact error";
    case InteractionErrorCode::DuplicateKind:
        return "duplicate kind";
    case InteractionErrorCode::InvalidRegistration:
        return "invalid registration";
    }
    return "unknown error";
}

std::optional<InteractionError> InteractionRegistry::Register(ContentId kind,
                                                              InteractionHandler handler) {
    if (kind.IsEmpty() || !handler.availableVerbs || (!handler.plan && !handler.planForHolder)) {
        return MakeError(InteractionErrorCode::InvalidRegistration, kind.Value());
    }
    if (handlers_.contains(kind)) {
        return MakeError(InteractionErrorCode::DuplicateKind, kind.Value());
    }
    handlers_.emplace(std::move(kind), std::move(handler));
    return std::nullopt;
}

bool InteractionRegistry::IsRegistered(const ContentId& kind) const {
    return handlers_.contains(kind);
}

Result<const InteractionHandler*, InteractionError>
InteractionRegistry::Resolve(const InteractableRef& target) const {
    if (!IsValidInteractableId(target.id)) {
        return MakeError(InteractionErrorCode::InvalidId, target.id);
    }
    const auto found = handlers_.find(target.kind);
    if (found == handlers_.end()) {
        return MakeError(InteractionErrorCode::UnknownKind, target.kind.Value());
    }
    return &found->second;
}

Result<InteractionOutcome, InteractionError>
InteractionRegistry::Interact(WorldFacts& facts, const InteractableRef& target,
                              InteractionVerb verb, const InteractionDefaults& defaults,
                              std::string_view cause, std::string_view holder) const {
    const auto handler = Resolve(target);
    if (!handler) {
        return handler.Error();
    }
    if (!IsValidHolderId(holder)) {
        return MakeError(InteractionErrorCode::InvalidId, std::string(holder));
    }
    const InteractionHandler& rules = *handler.Value();
    // The kind must offer the verb (a switch has no Force); otherwise nothing is planned or written.
    const auto offered = rules.availableVerbs(facts, target, defaults);
    if (!offered) {
        return offered.Error();
    }
    if (std::find(offered.Value().begin(), offered.Value().end(), verb) == offered.Value().end()) {
        return InteractionError{InteractionErrorCode::Refused, target.id, UnsupportedVerbFeedback(), std::nullopt};
    }
    auto plan = rules.planForHolder ? rules.planForHolder(facts, target, verb, defaults, holder)
                                    : rules.plan(facts, target, verb, defaults);
    if (!plan) {
        return plan.Error();
    }

    // A single command is atomic in WorldFacts itself. Several commands are applied
    // to a copy first, so a failing later command cannot leave earlier ones behind.
    const std::vector<FactCommand>& commands = plan.Value().commands;
    Result<std::vector<ChangeRecord>, InteractionError> changes = std::vector<ChangeRecord>{};
    if (commands.size() <= 1) {
        changes = ApplyAll(facts, commands, cause);
    } else {
        WorldFacts staged = facts;
        changes = ApplyAll(staged, commands, cause);
        if (changes) {
            facts = std::move(staged);
        }
    }
    if (!changes) {
        return changes.Error();
    }
    return InteractionOutcome{std::move(changes).Value(), std::move(plan.Value().feedback)};
}

Result<std::vector<InteractionVerb>, InteractionError>
InteractionRegistry::AvailableVerbs(const WorldFacts& facts, const InteractableRef& target,
                                    const InteractionDefaults& defaults) const {
    const auto handler = Resolve(target);
    if (!handler) {
        return handler.Error();
    }
    return handler.Value()->availableVerbs(facts, target, defaults);
}

} // namespace spn::core
