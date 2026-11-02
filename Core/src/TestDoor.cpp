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
    bool broken = false;
    bool breakable = false;
    std::string keyId;   // authored; empty = no key opens this door
};

/// Authored-only flag (no fact): absent = false. Nullopt: wrong type.
std::optional<bool> ReadAuthoredFlag(const InteractionDefaults& defaults, std::string_view state) {
    const auto found = defaults.values.find(state);
    if (found == defaults.values.end()) {
        return false;
    }
    const bool* value = std::get_if<bool>(&found->second);
    return value != nullptr ? std::optional<bool>(*value) : std::nullopt;
}

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
    const std::string brokenKey = TestDoorBrokenKey(id);
    const auto broken = ReadFlag(facts, brokenKey, defaults, kTestDoorBrokenState);
    if (!broken) {
        return Corrupt(brokenKey);
    }
    const auto breakable = ReadAuthoredFlag(defaults, kTestDoorBreakableState);
    if (!breakable) {
        return Corrupt(std::string(kTestDoorBreakableState));
    }
    DoorState state{*open, *locked, *disabled, *broken, *breakable, {}};
    if (const auto key = defaults.values.find(kTestDoorKeyState); key != defaults.values.end()) {
        const std::string* text = std::get_if<std::string>(&key->second);
        if (text == nullptr || (!text->empty() && !IsValidInteractableId(*text))) {
            return Corrupt(std::string(kTestDoorKeyState));
        }
        state.keyId = *text;
    }
    return state;
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

const ContentId& TestDoorUnlockedFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.unlocked");
    return feedback;
}

const ContentId& TestDoorBrokenFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.broken");
    return feedback;
}

const ContentId& TestDoorUnbreakableFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.unbreakable");
    return feedback;
}

const ContentId& TestDoorNotLockedFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.door.not_locked");
    return feedback;
}

std::string TestDoorBrokenKey(std::string_view id) {
    return std::string(kKeyPrefix).append(id).append(".broken");
}

std::string TestKeyHeldKey(std::string_view keyId) {
    return std::string("placeholder.key.").append(keyId).append(".held");
}

std::optional<FactError> SetTestKeyHeld(WorldFacts& facts, std::string_view keyId, bool held, std::string_view cause) {
    auto applied = facts.Apply(SetFact{TestKeyHeldKey(keyId), held}, cause);
    return applied ? std::nullopt : std::optional<FactError>(applied.Error());
}

bool IsTestKeyHeld(const WorldFacts& facts, std::string_view keyId) {
    const FactValue* fact = facts.Find(TestKeyHeldKey(keyId));
    const bool* value = fact != nullptr ? std::get_if<bool>(fact) : nullptr;
    return value != nullptr && *value;
}

Result<TestDoorInfo, InteractionError> ReadTestDoorInfo(const WorldFacts& facts, std::string_view id,
                                                        const InteractionDefaults& defaults) {
    const auto door = ReadDoor(facts, id, defaults);
    if (!door) {
        return door.Error();
    }
    const DoorState& s = door.Value();
    TestDoorInfo info;
    info.open = s.open;
    info.locked = s.locked && !s.broken;   // a broken lock holds nothing
    info.disabled = s.disabled;
    info.broken = s.broken;
    info.breakable = s.breakable;
    info.keyId = s.keyId;
    info.keyHeld = !s.keyId.empty() && IsTestKeyHeld(facts, s.keyId);
    if (s.disabled) {
        info.status = TestDoorStatus::Blocked;
    } else if (s.open) {
        info.status = TestDoorStatus::Open;
    } else if (info.locked) {
        info.status = TestDoorStatus::Locked;
    } else if (s.broken) {
        info.status = TestDoorStatus::Broken;
    } else {
        info.status = TestDoorStatus::Closed;
    }
    return info;
}

std::string_view ToString(TestDoorStatus status) noexcept {
    switch (status) {
    case TestDoorStatus::Closed: return "closed";
    case TestDoorStatus::Open: return "open";
    case TestDoorStatus::Locked: return "locked";
    case TestDoorStatus::Blocked: return "blocked";
    case TestDoorStatus::Broken: return "broken";
    }
    return "unknown";
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
        // Both verbs are offered always: trying one that does not apply reports why (locked, unbreakable, ...).
        return std::vector<InteractionVerb>{InteractionVerb::Use, InteractionVerb::Force};
    };
    handler.plan = [](const WorldFacts& facts, const InteractableRef& target,
                      InteractionVerb verb, const InteractionDefaults& defaults)
        -> Result<InteractionPlan, InteractionError> {
        const auto door = ReadDoor(facts, target.id, defaults);
        if (!door) {
            return door.Error();
        }
        const DoorState& state = door.Value();
        const auto refuse = [&](const ContentId& feedback) {
            return InteractionError{InteractionErrorCode::Refused, target.id, feedback, std::nullopt};
        };
        if (state.disabled) {
            return refuse(TestDoorDisabledFeedback());
        }
        const bool locked = state.locked && !state.broken;   // a broken door has no working lock
        InteractionPlan plan;
        switch (verb) {
        case InteractionVerb::Use:
            // The lock guards OPENING only (closed + locked). An open door can be closed whatever its lock state; the lock then keeps it shut.
            if (locked && !state.open) {
                if (!state.keyId.empty() && IsTestKeyHeld(facts, state.keyId)) {
                    // The key turns the lock; the door stays shut until the next Use (unlock and open are two deliberate steps).
                    plan.commands.emplace_back(SetFact{TestDoorLockedKey(target.id), false});
                    plan.feedback = TestDoorUnlockedFeedback();
                    return plan;
                }
                return refuse(TestDoorLockedFeedback());
            }
            plan.commands.emplace_back(SetFact{TestDoorOpenKey(target.id), !state.open});
            return plan;
        case InteractionVerb::Force:
            if (state.broken) {
                return refuse(TestDoorBrokenFeedback());
            }
            if (!state.breakable) {
                return refuse(TestDoorUnbreakableFeedback());
            }
            if (!locked || state.open) {
                return refuse(TestDoorNotLockedFeedback());   // nothing to force: it is unlocked (or open)
            }
            // Breaking splinters the lock for good: broken, no longer locked, and the door swings open.
            plan.commands.emplace_back(SetFact{TestDoorBrokenKey(target.id), true});
            plan.commands.emplace_back(SetFact{TestDoorLockedKey(target.id), false});
            plan.commands.emplace_back(SetFact{TestDoorOpenKey(target.id), true});
            plan.feedback = TestDoorBrokenFeedback();
            return plan;
        }
        return refuse(UnsupportedVerbFeedback());
    };
    return handler;
}

std::optional<InteractionError> RegisterTestDoor(InteractionRegistry& registry) {
    return registry.Register(TestDoorKind(), MakeTestDoorHandler());
}

} // namespace spn::core
