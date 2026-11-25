#pragma once

// Configurable investigation assistance (SPN-DEV-02, decisions DIF-01..03, DLG/EVD in
// Docs/Planning/DECISIONS.md section J).
//
// Two separate things are stored:
//   a preset     a named set of values, supplied as DATA by the caller (AssistPresetTable).
//                No difficulty names or balancing values are defined here (DIF-01: the number
//                and names of difficulty levels are open).
//   overrides    single options changed by the player, independent of the preset (DIF-02).
// Effective value of an option = its override if present, otherwise the preset's value.
// Without a preset every option is off. Changing the preset keeps the overrides.
//
// Assistance never changes what the character knows (DIF-03): this module writes only its own
// placeholder keys, and no solution or conclusion is derived here (EVD-04).
//
// Keys (WorldFacts):
//   placeholder.assist.preset                 string  preset id; absent = no preset
//   placeholder.assist.<option>.override      bool    absent = follow the preset

#include <cstdint>
#include <map>
#include <optional>
#include <spn/core/InvestigationIds.h>
#include <spn/core/Result.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

/// The investigation help options. Their ids are stable; more options are added as new entries.
enum class AssistOption : std::uint8_t { FoundMarker, KnownHint, DialogueHint };
inline constexpr std::size_t kAssistOptionCount = 3;

[[nodiscard]] std::string_view AssistOptionName(AssistOption option) noexcept; ///< "found_marker", ...
[[nodiscard]] std::optional<AssistOption> ParseAssistOption(std::string_view name) noexcept;

struct AssistPreset {
    std::string id;                          ///< local id
    std::map<AssistOption, bool> values;     ///< must contain every option
    friend bool operator==(const AssistPreset&, const AssistPreset&) = default;
};

class AssistPresetTable {
public:
    enum class Error : std::uint8_t { InvalidId, Duplicate, MissingOption, Empty };

    [[nodiscard]] static Result<AssistPresetTable, Error> Create(std::vector<AssistPreset> presets);
    [[nodiscard]] const AssistPreset* Find(std::string_view id) const noexcept;
    [[nodiscard]] const std::vector<AssistPreset>& Presets() const noexcept { return presets_; }

private:
    explicit AssistPresetTable(std::vector<AssistPreset> presets) : presets_(std::move(presets)) {}
    std::vector<AssistPreset> presets_;
};

enum class AssistErrorCode : std::uint8_t {
    InvalidId,       ///< malformed id
    UnknownPreset,   ///< the preset id is not in the table
    StateCorrupt,    ///< a stored assist fact has the wrong type
    Store            ///< WorldFacts refused the write
};

struct AssistError {
    AssistErrorCode code = AssistErrorCode::StateCorrupt;
    std::optional<FactError> fact;
    friend bool operator==(const AssistError&, const AssistError&) = default;
};

[[nodiscard]] std::string AssistPresetKey();
[[nodiscard]] std::string AssistOverrideKey(AssistOption option);

/// Selects the preset (validated against the table). Keeps the overrides. Idempotent.
[[nodiscard]] Result<bool, AssistError> SelectAssistPreset(WorldFacts& facts, const AssistPresetTable& table,
                                                           std::string_view presetId, std::string_view cause = {});
/// Sets one override. Idempotent: the same value is not written again.
[[nodiscard]] Result<bool, AssistError> SetAssistOverride(WorldFacts& facts, AssistOption option, bool value,
                                                          std::string_view cause = {});
/// Removes one override so the option follows the preset again. Returns true if one was removed.
[[nodiscard]] Result<bool, AssistError> ClearAssistOverride(WorldFacts& facts, AssistOption option, std::string_view cause = {});

/// The effective value of one option now.
[[nodiscard]] Result<bool, AssistError> EffectiveAssist(const WorldFacts& facts, const AssistPresetTable& table, AssistOption option);
/// The selected preset id ("" if none).
[[nodiscard]] Result<std::string, AssistError> SelectedAssistPreset(const WorldFacts& facts);

[[nodiscard]] std::string_view ToString(AssistErrorCode code) noexcept;

} // namespace spn::core
