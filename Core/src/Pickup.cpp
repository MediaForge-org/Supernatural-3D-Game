#include <spn/core/Pickup.h>
#include <utility>
#include <variant>

namespace spn::core {

namespace {

constexpr std::string_view kPickupPrefix = "placeholder.pickup.";
constexpr std::string_view kInventoryPrefix = "placeholder.inventory.";
constexpr std::string_view kCountSuffix = ".count";

ContentId MustParse(std::string_view text) {
    // Only called with the literals below, which are valid ids.
    return *ContentId::Parse(text);
}

InteractionError Error(InteractionErrorCode code, std::string subject) {
    return InteractionError{code, std::move(subject), {}, std::nullopt};
}

/// Authored item: required string holding a valid item id.
Result<std::string, InteractionError> ReadItem(const InteractionDefaults& defaults) {
    const auto found = defaults.values.find(kTestPickupItemState);
    if (found == defaults.values.end()) {
        return Error(InteractionErrorCode::StateCorrupt, std::string(kTestPickupItemState));
    }
    const auto* item = std::get_if<std::string>(&found->second);
    if (item == nullptr) {
        return Error(InteractionErrorCode::StateCorrupt, std::string(kTestPickupItemState));
    }
    if (!IsValidItemId(*item)) {
        return Error(InteractionErrorCode::InvalidId, *item);
    }
    return *item;
}

/// Authored amount: integer >= 1, default 1.
Result<std::int64_t, InteractionError> ReadAmount(const InteractionDefaults& defaults) {
    const auto found = defaults.values.find(kTestPickupAmountState);
    if (found == defaults.values.end()) {
        return std::int64_t{1};
    }
    const auto* amount = std::get_if<std::int64_t>(&found->second);
    if (amount == nullptr || *amount < 1) {
        return Error(InteractionErrorCode::StateCorrupt, std::string(kTestPickupAmountState));
    }
    return *amount;
}

/// The fact if present, else the authored default, else false. Error: wrong type.
Result<bool, InteractionError> ReadTaken(const WorldFacts& facts, const std::string& key,
                                         const InteractionDefaults& defaults) {
    if (const FactValue* fact = facts.Find(key)) {
        const bool* value = std::get_if<bool>(fact);
        if (value == nullptr) {
            return Error(InteractionErrorCode::StateCorrupt, key);
        }
        return *value;
    }
    const auto found = defaults.values.find(kTestPickupTakenState);
    if (found == defaults.values.end()) {
        return false;
    }
    const bool* value = std::get_if<bool>(&found->second);
    if (value == nullptr) {
        return Error(InteractionErrorCode::StateCorrupt, std::string(kTestPickupTakenState));
    }
    return *value;
}

} // namespace

bool IsValidItemId(std::string_view item) noexcept {
    if (item.empty() || item.size() > kMaxItemIdLength) {
        return false;
    }
    const auto isLower = [](char ch) { return ch >= 'a' && ch <= 'z'; };
    if (!isLower(item.front())) {
        return false;
    }
    for (const char ch : item) {
        const bool isDigit = ch >= '0' && ch <= '9';
        if (!isLower(ch) && !isDigit && ch != '_') {
            return false;
        }
    }
    return true;
}

const ContentId& TestPickupKind() {
    static const ContentId kind = MustParse("placeholder:interactable.pickup");
    return kind;
}

const ContentId& TestPickupGoneFeedback() {
    static const ContentId feedback = MustParse("placeholder:feedback.pickup.gone");
    return feedback;
}

std::string TestPickupTakenKey(std::string_view id) {
    return std::string(kPickupPrefix).append(id).append(".taken");
}

std::string InventoryCountKey(std::string_view holder, std::string_view item) {
    return std::string(kInventoryPrefix).append(holder).append(1, '.').append(item).append(kCountSuffix);
}

InteractionHandler MakeTestPickupHandler() {
    InteractionHandler handler;
    handler.availableVerbs = [](const WorldFacts& facts, const InteractableRef& target,
                                const InteractionDefaults& defaults)
        -> Result<std::vector<InteractionVerb>, InteractionError> {
        const auto item = ReadItem(defaults);
        if (!item) {
            return item.Error();
        }
        const auto taken = ReadTaken(facts, TestPickupTakenKey(target.id), defaults);
        if (!taken) {
            return taken.Error();
        }
        // Use is offered on a taken pickup too: trying it reports "gone".
        return std::vector<InteractionVerb>{InteractionVerb::Use};
    };
    handler.planForHolder = [](const WorldFacts& facts, const InteractableRef& target,
                               InteractionVerb verb, const InteractionDefaults& defaults,
                               std::string_view holder) -> Result<InteractionPlan, InteractionError> {
        const auto item = ReadItem(defaults);
        if (!item) {
            return item.Error();
        }
        const auto amount = ReadAmount(defaults);
        if (!amount) {
            return amount.Error();
        }
        const std::string takenKey = TestPickupTakenKey(target.id);
        const auto taken = ReadTaken(facts, takenKey, defaults);
        if (!taken) {
            return taken.Error();
        }
        // D1: Use is the only verb. Adding one makes this switch warn (-Wswitch, /W4).
        switch (verb) {
        case InteractionVerb::Use:
        case InteractionVerb::Force: // never offered by this kind (Interact checks AvailableVerbs first)
            break;
        }
        if (taken.Value()) {
            return InteractionError{InteractionErrorCode::Refused, target.id,
                                    TestPickupGoneFeedback(), std::nullopt};
        }
        const std::string countKey = InventoryCountKey(holder, item.Value());
        const FactValue* current = facts.Find(countKey);
        if (current != nullptr && !std::holds_alternative<std::int64_t>(*current)) {
            return Error(InteractionErrorCode::StateCorrupt, countKey);
        }
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{takenKey, true});
        // AdjustInt needs an existing fact: the first item of a kind creates the count.
        if (current == nullptr) {
            plan.commands.emplace_back(SetFact{countKey, amount.Value()});
        } else {
            plan.commands.emplace_back(AdjustInt{countKey, amount.Value()});
        }
        return plan;
    };
    return handler;
}

std::optional<InteractionError> RegisterTestPickup(InteractionRegistry& registry) {
    return registry.Register(TestPickupKind(), MakeTestPickupHandler());
}

Result<std::int64_t, InteractionError>
InventoryCount(const WorldFacts& facts, std::string_view holder, std::string_view item) {
    if (!IsValidHolderId(holder)) {
        return Error(InteractionErrorCode::InvalidId, std::string(holder));
    }
    if (!IsValidItemId(item)) {
        return Error(InteractionErrorCode::InvalidId, std::string(item));
    }
    const std::string key = InventoryCountKey(holder, item);
    const FactValue* fact = facts.Find(key);
    if (fact == nullptr) {
        return std::int64_t{0};
    }
    const auto* count = std::get_if<std::int64_t>(fact);
    if (count == nullptr) {
        return Error(InteractionErrorCode::StateCorrupt, key);
    }
    return *count;
}

Result<std::vector<InventoryEntry>, InteractionError>
ListInventory(const WorldFacts& facts, std::string_view holder) {
    if (!IsValidHolderId(holder)) {
        return Error(InteractionErrorCode::InvalidId, std::string(holder));
    }
    const std::string prefix = std::string(kInventoryPrefix).append(holder).append(1, '.');
    std::vector<InventoryEntry> entries;
    for (auto it = facts.Facts().lower_bound(prefix); it != facts.Facts().end(); ++it) {
        const std::string& key = it->first;
        if (!key.starts_with(prefix)) {
            break; // keys are ordered: past the prefix range
        }
        if (key.size() <= prefix.size() + kCountSuffix.size() || !key.ends_with(kCountSuffix)) {
            continue;
        }
        const std::string_view item(key.data() + prefix.size(),
                                    key.size() - prefix.size() - kCountSuffix.size());
        // A holder whose id extends this one ("player.local" vs "player") has dots in the rest.
        if (!IsValidItemId(item)) {
            continue;
        }
        const auto* count = std::get_if<std::int64_t>(&it->second);
        if (count == nullptr) {
            return Error(InteractionErrorCode::StateCorrupt, key);
        }
        entries.push_back(InventoryEntry{std::string(item), *count});
    }
    return entries;
}

} // namespace spn::core
