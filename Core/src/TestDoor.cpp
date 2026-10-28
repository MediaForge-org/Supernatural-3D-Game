#include <spn/core/TestDoor.h>
#include <utility>
#include <variant>
#include <vector>

namespace spn::core {

namespace {

constexpr std::string_view kKeyPrefix = "placeholder.door.";

ContentId MustParse(std::string_view text) {
    // Only called with the literals below, which are valid ids.
    return *ContentId::Parse(text);
}

/// The fact if present, else the authored default, else false. Nullopt: wrong type.
std::optional<bool> ReadFlag(const WorldFacts& facts, const std::string& key,
                             const InteractionDefaults& defaults, std::string_view state) {
    if (const FactValue* fact = facts.Find(key)) {
        const bool* value = std::get_if<bool>(fact);
        return value != nullptr ? std::optional<bool>(*value) : std::nullopt;
    }
    const auto found = defaults.values.find(state);
    if (found == defaults.values.end()) {
        return false;
    }
    const bool* value = std::get_if<bool>(&found->second);
    return value != nullptr ? std::optional<bool>(*value) : std::nullopt;
}

InteractionError Corrupt(std::string subject) {
    return InteractionError{InteractionErrorCode::StateCorrupt, std::move(subject), {},
                            std::nullopt};
}

struct DoorState {
    bool open = false;
    bool locked = false;
    bool disabled = false;
};

Result<DoorState, InteractionError> ReadDoor(const WorldFacts& facts, std::string_view id,
                                             const InteractionDefaults& defaults) {
    const std::string openKey = TestDoorOpenKey(id);
    const std::string lockedKey = TestDoorLockedKey(id);
    const auto open = ReadFlag(facts, openKey, defaults, kTestDoorOpenState);
    if (!open) {
        return Corrupt(openKey);
    }
    const auto locked = ReadFlag(facts, lockedKey, defaults, kTestDoorLockedState);
    if (!locked) {
        return Corrupt(lockedKey);
    }
    const std::string disabledKey = TestDoorDisabledKey(id);
    const auto disabled = ReadFlag(facts, disabledKey, defaults, kTestDoorDisabledState);
    if (!disabled) {
        return Corrupt(disabledKey);
    }
    return DoorState{*open, *locked, *disabled};
}

} // namespace

const ContentId& TestDoorKind() {
    static const ContentId kind = MustParse("placeholder:interactable.door");
    return kind;
}

const ContentId& TestDoorLockedFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.locked");
    return feedback;
}

const ContentId& TestDoorDisabledFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.disabled");
    return feedback;
}

const ContentId& TestDoorBlockedFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.blocked");
    return feedback;
}

std::string TestDoorDisabledKey(std::string_view id) {
    return std::string(kKeyPrefix).append(id).append(".disabled");
}

std::optional<FactError> SetTestDoorLocked(WorldFacts& facts, std::string_view id, bool locked, std::string_view cause) {
    auto applied = facts.Apply(SetFact{TestDoorLockedKey(id), locked}, cause);
    return applied ? std::nullopt : std::optional<FactError>(applied.Error());
}

std::optional<FactError> SetTestDoorDisabled(WorldFacts& facts, std::string_view id, bool disabled, std::string_view cause) {
    auto applied = facts.Apply(SetFact{TestDoorDisabledKey(id), disabled}, cause);
    return applied ? std::nullopt : std::optional<FactError>(applied.Error());
}

std::string TestDoorOpenKey(std::string_view id) {
    return std::string(kKeyPrefix).append(id).append(".open");
}

std::string TestDoorLockedKey(std::string_view id) {
    return std::string(kKeyPrefix).append(id).append(".locked");
}

InteractionHandler MakeTestDoorHandler() {
    InteractionHandler handler;
    handler.availableVerbs = [](const WorldFacts& facts, const InteractableRef& target,
                                const InteractionDefaults& defaults)
        -> Result<std::vector<InteractionVerb>, InteractionError> {
        const auto door = ReadDoor(facts, target.id, defaults);
        if (!door) {
            return door.Error();
        }
        // Use is offered on a locked door too: trying it reports "locked".
        return std::vector<InteractionVerb>{InteractionVerb::Use};
    };
    handler.plan = [](const WorldFacts& facts, const InteractableRef& target,
                      InteractionVerb verb, const InteractionDefaults& defaults)
        -> Result<InteractionPlan, InteractionError> {
        const auto door = ReadDoor(facts, target.id, defaults);
        if (!door) {
            return door.Error();
        }
        // D1: Use is the only verb. Adding one makes this switch warn (-Wswitch, /W4),
        // forcing a conscious decision for doors instead of a silent fall-through.
        switch (verb) {
        case InteractionVerb::Use:
            break;
        }
        if (door.Value().disabled) {
            return InteractionError{InteractionErrorCode::Refused, target.id,
                                    TestDoorDisabledFeedback(), std::nullopt};
        }
        if (door.Value().locked) {
            return InteractionError{InteractionErrorCode::Refused, target.id,
                                    TestDoorLockedFeedback(), std::nullopt};
        }
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{TestDoorOpenKey(target.id), !door.Value().open});
        return plan;
    };
    return handler;
}

std::optional<InteractionError> RegisterTestDoor(InteractionRegistry& registry) {
    return registry.Register(TestDoorKind(), MakeTestDoorHandler());
}

} // namespace spn::core
