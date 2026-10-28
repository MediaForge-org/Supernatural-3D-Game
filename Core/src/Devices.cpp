#include <spn/core/Devices.h>

#include <cstdint>
#include <utility>
#include <variant>
#include <vector>

namespace spn::core {

namespace {

ContentId MustParse(std::string_view text) { return *ContentId::Parse(text); }

InteractionError Corrupt(std::string subject) {
    return InteractionError{InteractionErrorCode::StateCorrupt, std::move(subject), {}, std::nullopt};
}

InteractionError Refuse(std::string subject, ContentId feedback) {
    return InteractionError{InteractionErrorCode::Refused, std::move(subject), std::move(feedback), std::nullopt};
}

std::string Key(std::string_view family, std::string_view id, std::string_view field) {
    return std::string("placeholder.").append(family).append(".").append(id).append(".").append(field);
}

const FactValue* Authored(const InteractionDefaults& defaults, std::string_view name) {
    const auto found = defaults.values.find(name);
    return found == defaults.values.end() ? nullptr : &found->second;
}

/// fact, else authored default, else `fallback`. nullopt = wrong type.
std::optional<bool> ReadBool(const WorldFacts& facts, const std::string& key, const InteractionDefaults& defaults, std::string_view name, bool fallback) {
    if (const FactValue* fact = facts.Find(key)) {
        const bool* v = std::get_if<bool>(fact);
        return v != nullptr ? std::optional<bool>(*v) : std::nullopt;
    }
    if (const FactValue* authored = Authored(defaults, name)) {
        const bool* v = std::get_if<bool>(authored);
        return v != nullptr ? std::optional<bool>(*v) : std::nullopt;
    }
    return fallback;
}

std::optional<std::int64_t> ReadInt(const WorldFacts& facts, const std::string& key, const InteractionDefaults& defaults, std::string_view name, std::int64_t fallback) {
    if (const FactValue* fact = facts.Find(key)) {
        const auto* v = std::get_if<std::int64_t>(fact);
        return v != nullptr ? std::optional<std::int64_t>(*v) : std::nullopt;
    }
    if (const FactValue* authored = Authored(defaults, name)) {
        const auto* v = std::get_if<std::int64_t>(authored);
        return v != nullptr ? std::optional<std::int64_t>(*v) : std::nullopt;
    }
    return fallback;
}

/// The defaults of a configuration value that is NOT state (never overridden by a fact).
std::optional<std::int64_t> ReadConfigInt(const InteractionDefaults& defaults, std::string_view name, std::int64_t fallback) {
    if (const FactValue* authored = Authored(defaults, name)) {
        const auto* v = std::get_if<std::int64_t>(authored);
        return v != nullptr ? std::optional<std::int64_t>(*v) : std::nullopt;
    }
    return fallback;
}

Result<std::vector<InteractionVerb>, InteractionError> UseOnly() { return std::vector<InteractionVerb>{InteractionVerb::Use}; }

} // namespace

const ContentId& SwitchKind() { static const ContentId k = MustParse("placeholder:interactable.switch"); return k; }
const ContentId& ButtonKind() { static const ContentId k = MustParse("placeholder:interactable.button"); return k; }
const ContentId& LeverKind() { static const ContentId k = MustParse("placeholder:interactable.lever"); return k; }
const ContentId& DeviceDisabledFeedback() { static const ContentId k = MustParse("placeholder:feedback.device.disabled"); return k; }
const ContentId& ButtonExhaustedFeedback() { static const ContentId k = MustParse("placeholder:feedback.button.exhausted"); return k; }

std::string SwitchOnKey(std::string_view id) { return Key("switch", id, "on"); }
std::string SwitchDisabledKey(std::string_view id) { return Key("switch", id, "disabled"); }
std::string ButtonPressesKey(std::string_view id) { return Key("button", id, "presses"); }
std::string ButtonDisabledKey(std::string_view id) { return Key("button", id, "disabled"); }
std::string LeverPositionKey(std::string_view id) { return Key("lever", id, "position"); }
std::string LeverDisabledKey(std::string_view id) { return Key("lever", id, "disabled"); }

InteractionHandler MakeSwitchHandler() {
    InteractionHandler h;
    h.availableVerbs = [](const WorldFacts& facts, const InteractableRef& t, const InteractionDefaults& d) -> Result<std::vector<InteractionVerb>, InteractionError> {
        if (!ReadBool(facts, SwitchOnKey(t.id), d, "on", false)) return Corrupt(SwitchOnKey(t.id));
        if (!ReadBool(facts, SwitchDisabledKey(t.id), d, "disabled", false)) return Corrupt(SwitchDisabledKey(t.id));
        return UseOnly();
    };
    h.plan = [](const WorldFacts& facts, const InteractableRef& t, InteractionVerb, const InteractionDefaults& d) -> Result<InteractionPlan, InteractionError> {
        const auto on = ReadBool(facts, SwitchOnKey(t.id), d, "on", false);
        if (!on) return Corrupt(SwitchOnKey(t.id));
        const auto disabled = ReadBool(facts, SwitchDisabledKey(t.id), d, "disabled", false);
        if (!disabled) return Corrupt(SwitchDisabledKey(t.id));
        if (*disabled) return Refuse(t.id, DeviceDisabledFeedback());
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{SwitchOnKey(t.id), !*on});
        return plan;
    };
    return h;
}

InteractionHandler MakeButtonHandler() {
    InteractionHandler h;
    h.availableVerbs = [](const WorldFacts& facts, const InteractableRef& t, const InteractionDefaults& d) -> Result<std::vector<InteractionVerb>, InteractionError> {
        if (!ReadInt(facts, ButtonPressesKey(t.id), d, "", 0)) return Corrupt(ButtonPressesKey(t.id));
        if (!ReadBool(facts, ButtonDisabledKey(t.id), d, "disabled", false)) return Corrupt(ButtonDisabledKey(t.id));
        return UseOnly();
    };
    h.plan = [](const WorldFacts& facts, const InteractableRef& t, InteractionVerb, const InteractionDefaults& d) -> Result<InteractionPlan, InteractionError> {
        // the press counter is pure state: an authored default for it does not exist (name "" is never configured)
        const auto presses = ReadInt(facts, ButtonPressesKey(t.id), d, "", 0);
        if (!presses || *presses < 0) return Corrupt(ButtonPressesKey(t.id));
        const auto disabled = ReadBool(facts, ButtonDisabledKey(t.id), d, "disabled", false);
        if (!disabled) return Corrupt(ButtonDisabledKey(t.id));
        const auto maxPresses = ReadConfigInt(d, "max_presses", 0);
        if (!maxPresses || *maxPresses < 0) return Corrupt("max_presses");
        if (*disabled) return Refuse(t.id, DeviceDisabledFeedback());
        if (*maxPresses > 0 && *presses >= *maxPresses) return Refuse(t.id, ButtonExhaustedFeedback());
        if (*presses == INT64_MAX) return Corrupt(ButtonPressesKey(t.id));
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{ButtonPressesKey(t.id), *presses + 1});
        return plan;
    };
    return h;
}

InteractionHandler MakeLeverHandler() {
    InteractionHandler h;
    const auto read = [](const WorldFacts& facts, const InteractableRef& t, const InteractionDefaults& d, std::int64_t& position, std::int64_t& positions, bool& disabled) -> std::optional<InteractionError> {
        const auto count = ReadConfigInt(d, "positions", 2);
        if (!count || *count < kMinLeverPositions || *count > kMaxLeverPositions) return Corrupt("positions");
        const auto pos = ReadInt(facts, LeverPositionKey(t.id), d, "position", 0);
        if (!pos || *pos < 0 || *pos >= *count) return Corrupt(LeverPositionKey(t.id));
        const auto dis = ReadBool(facts, LeverDisabledKey(t.id), d, "disabled", false);
        if (!dis) return Corrupt(LeverDisabledKey(t.id));
        position = *pos;
        positions = *count;
        disabled = *dis;
        return std::nullopt;
    };
    h.availableVerbs = [read](const WorldFacts& facts, const InteractableRef& t, const InteractionDefaults& d) -> Result<std::vector<InteractionVerb>, InteractionError> {
        std::int64_t p = 0, n = 0;
        bool dis = false;
        if (auto error = read(facts, t, d, p, n, dis)) return *error;
        return UseOnly();
    };
    h.plan = [read](const WorldFacts& facts, const InteractableRef& t, InteractionVerb, const InteractionDefaults& d) -> Result<InteractionPlan, InteractionError> {
        std::int64_t p = 0, n = 0;
        bool dis = false;
        if (auto error = read(facts, t, d, p, n, dis)) return *error;
        if (dis) return Refuse(t.id, DeviceDisabledFeedback());
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{LeverPositionKey(t.id), (p + 1) % n});
        return plan;
    };
    return h;
}

std::optional<InteractionError> RegisterDevices(InteractionRegistry& registry) {
    if (auto e = registry.Register(SwitchKind(), MakeSwitchHandler())) return e;
    if (auto e = registry.Register(ButtonKind(), MakeButtonHandler())) return e;
    return registry.Register(LeverKind(), MakeLeverHandler());
}

} // namespace spn::core
