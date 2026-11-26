#pragma once
// Difficulty profiles (NightBuild07). DECISIONS.md DIF-01: several difficulty levels, their NUMBER and NAMES are open; the developer plans many
// (example names "Story ... Meisterhaft", not fixed). So a difficulty is DATA: an ordered list of profiles, any count, each with an id and the
// values the systems read. Nothing in code knows "easy" or "hard".
//   * respawnHealth: health fraction after a death and the reset to the checkpoint (TOD-04: the easier, the more; about 10 % on the hardest).
//   * creature values: the reaction delay before a pursuit (the player's head start after a shot) and the search duration
//     (Kreaturen/10: search phase durations depend on the difficulty).
// Investigation assists are NOT part of this (DIF-02: configured independently). DIF-03: a difficulty never changes what the character knows.
// All numbers are PLACEHOLDERS (CLAUDE.md section 3).
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

struct DifficultyProfile {
    std::string id;                    ///< stable key in settings and saves: [a-z0-9_], 1..32 characters
    double respawnHealth = 1.0;        ///< (0, 1]
    double creatureReactionDelay = 1.0; ///< s, >= 0 (0: no head start)
    double creatureSearchSeconds = 20.0; ///< s, > 0
    double creatureHearingScale = 1.0; ///< [0.25, 4]
};

[[nodiscard]] bool IsValidDifficultyId(std::string_view id) noexcept;
[[nodiscard]] std::optional<std::string> Validate(const DifficultyProfile& profile);
/// The whole list: not empty, every profile valid, ids unique.
[[nodiscard]] std::optional<std::string> ValidateDifficultyList(const std::vector<DifficultyProfile>& profiles);
/// Index of the profile with this id; nullopt if none.
[[nodiscard]] std::optional<std::size_t> FindDifficulty(const std::vector<DifficultyProfile>& profiles, std::string_view id) noexcept;
/// The health a character has after a respawn: the profile's fraction of `maxHealth`, never 0 (a respawn must be survivable for a moment).
[[nodiscard]] double RespawnHealth(const DifficultyProfile& profile, double maxHealth) noexcept;

} // namespace spn::core
