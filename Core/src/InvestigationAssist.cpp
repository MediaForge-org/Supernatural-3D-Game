#include <spn/core/InvestigationAssist.h>

#include <set>
#include <utility>

namespace spn::core {

namespace {

constexpr std::string_view kPresetKey = "placeholder.assist.preset";
constexpr std::string_view kCauseSelect = "assist:select-preset";
constexpr std::string_view kCauseOverride = "assist:override";

constexpr std::string_view kNames[kAssistOptionCount] = {"found_marker", "known_hint", "dialogue_hint"};

std::size_t Index(AssistOption option) noexcept { return static_cast<std::size_t>(option); }

} // namespace

std::string_view AssistOptionName(AssistOption option) noexcept { return kNames[Index(option)]; }

std::optional<AssistOption> ParseAssistOption(std::string_view name) noexcept {
    for (std::size_t i = 0; i < kAssistOptionCount; ++i) {
        if (kNames[i] == name) return static_cast<AssistOption>(i);
    }
    return std::nullopt;
}

Result<AssistPresetTable, AssistPresetTable::Error> AssistPresetTable::Create(std::vector<AssistPreset> presets) {
    if (presets.empty()) return Error::Empty;
    std::set<std::string> seen;
    for (const auto& preset : presets) {
        if (!IsValidLocalId(preset.id)) return Error::InvalidId;
        if (!seen.insert(preset.id).second) return Error::Duplicate;
        for (std::size_t i = 0; i < kAssistOptionCount; ++i) {
            if (!preset.values.contains(static_cast<AssistOption>(i))) return Error::MissingOption;
        }
    }
    return AssistPresetTable(std::move(presets));
}

const AssistPreset* AssistPresetTable::Find(std::string_view id) const noexcept {
    for (const auto& preset : presets_) {
        if (preset.id == id) return &preset;
    }
    return nullptr;
}

std::string AssistPresetKey() { return std::string(kPresetKey); }

std::string AssistOverrideKey(AssistOption option) {
    return "placeholder.assist." + std::string(AssistOptionName(option)) + ".override";
}

Result<bool, AssistError> SelectAssistPreset(WorldFacts& facts, const AssistPresetTable& table, std::string_view presetId,
                                             std::string_view cause) {
    if (!IsValidLocalId(presetId)) return AssistError{AssistErrorCode::InvalidId, std::nullopt};
    if (table.Find(presetId) == nullptr) return AssistError{AssistErrorCode::UnknownPreset, std::nullopt};
    const FactValue* current = facts.Find(kPresetKey);
    if (current != nullptr && std::get_if<std::string>(current) != nullptr && *std::get_if<std::string>(current) == presetId) {
        return false;
    }
    const auto applied = facts.Apply(SetFact{std::string(kPresetKey), std::string(presetId)}, cause.empty() ? kCauseSelect : cause);
    if (!applied) return AssistError{AssistErrorCode::Store, applied.Error()};
    return true;
}

Result<bool, AssistError> SetAssistOverride(WorldFacts& facts, AssistOption option, bool value, std::string_view cause) {
    const std::string key = AssistOverrideKey(option);
    const FactValue* current = facts.Find(key);
    if (current != nullptr) {
        const bool* flag = std::get_if<bool>(current);
        if (flag == nullptr) return AssistError{AssistErrorCode::StateCorrupt, std::nullopt};
        if (*flag == value) return false;
    }
    const auto applied = facts.Apply(SetFact{key, value}, cause.empty() ? kCauseOverride : cause);
    if (!applied) return AssistError{AssistErrorCode::Store, applied.Error()};
    return true;
}

Result<bool, AssistError> ClearAssistOverride(WorldFacts& facts, AssistOption option, std::string_view cause) {
    const std::string key = AssistOverrideKey(option);
    const FactValue* current = facts.Find(key);
    if (current == nullptr) return false;
    if (std::get_if<bool>(current) == nullptr) return AssistError{AssistErrorCode::StateCorrupt, std::nullopt};
    const auto applied = facts.Apply(RemoveFact{key}, cause.empty() ? kCauseOverride : cause);
    if (!applied) return AssistError{AssistErrorCode::Store, applied.Error()};
    return true;
}

Result<std::string, AssistError> SelectedAssistPreset(const WorldFacts& facts) {
    const FactValue* value = facts.Find(kPresetKey);
    if (value == nullptr) return std::string{};
    const std::string* id = std::get_if<std::string>(value);
    if (id == nullptr) return AssistError{AssistErrorCode::StateCorrupt, std::nullopt};
    return *id;
}

Result<bool, AssistError> EffectiveAssist(const WorldFacts& facts, const AssistPresetTable& table, AssistOption option) {
    const FactValue* override = facts.Find(AssistOverrideKey(option));
    if (override != nullptr) {
        const bool* flag = std::get_if<bool>(override);
        if (flag == nullptr) return AssistError{AssistErrorCode::StateCorrupt, std::nullopt};
        return *flag;
    }
    const auto selected = SelectedAssistPreset(facts);
    if (!selected) return selected.Error();
    if (selected.Value().empty()) return false; // no preset: every help is off
    const AssistPreset* preset = table.Find(selected.Value());
    if (preset == nullptr) return AssistError{AssistErrorCode::UnknownPreset, std::nullopt};
    return preset->values.at(option);
}

std::string_view ToString(AssistErrorCode code) noexcept {
    switch (code) {
        case AssistErrorCode::InvalidId: return "invalid_id";
        case AssistErrorCode::UnknownPreset: return "unknown_preset";
        case AssistErrorCode::StateCorrupt: return "state_corrupt";
        case AssistErrorCode::Store: return "store";
    }
    return "unknown";
}

} // namespace spn::core
