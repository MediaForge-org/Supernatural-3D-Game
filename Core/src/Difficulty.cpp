#include <spn/core/Difficulty.h>

#include <algorithm>
#include <cmath>
#include <set>

namespace spn::core {

bool IsValidDifficultyId(std::string_view id) noexcept
{
    if (id.empty() || id.size() > 32) {
        return false;
    }
    return std::all_of(id.begin(), id.end(), [](char c) { return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'; });
}

std::optional<std::string> Validate(const DifficultyProfile& p)
{
    if (!IsValidDifficultyId(p.id)) {
        return "difficulty id '" + p.id + "' is not valid ([a-z0-9_], 1..32 characters)";
    }
    if (!std::isfinite(p.respawnHealth) || p.respawnHealth <= 0.0 || p.respawnHealth > 1.0) {
        return "difficulty '" + p.id + "': respawnHealth must be in (0, 1]";
    }
    if (!std::isfinite(p.creatureReactionDelay) || p.creatureReactionDelay < 0.0 || p.creatureReactionDelay > 30.0) {
        return "difficulty '" + p.id + "': creatureReactionDelay must be in [0, 30] s";
    }
    if (!std::isfinite(p.creatureSearchSeconds) || p.creatureSearchSeconds <= 0.0 || p.creatureSearchSeconds > 600.0) {
        return "difficulty '" + p.id + "': creatureSearchSeconds must be in (0, 600] s";
    }
    if (!std::isfinite(p.creatureHearingScale) || p.creatureHearingScale < 0.25 || p.creatureHearingScale > 4.0) {
        return "difficulty '" + p.id + "': creatureHearingScale must be in [0.25, 4]";
    }
    return std::nullopt;
}

std::optional<std::string> ValidateDifficultyList(const std::vector<DifficultyProfile>& profiles)
{
    if (profiles.empty()) {
        return std::string("the difficulty list is empty");
    }
    std::set<std::string> seen;
    for (const DifficultyProfile& p : profiles) {
        if (auto error = Validate(p)) {
            return error;
        }
        if (!seen.insert(p.id).second) {
            return "difficulty id '" + p.id + "' is used twice";
        }
    }
    return std::nullopt;
}

std::optional<std::size_t> FindDifficulty(const std::vector<DifficultyProfile>& profiles, std::string_view id) noexcept
{
    for (std::size_t i = 0; i < profiles.size(); ++i) {
        if (profiles[i].id == id) {
            return i;
        }
    }
    return std::nullopt;
}

double RespawnHealth(const DifficultyProfile& profile, double maxHealth) noexcept
{
    if (!std::isfinite(maxHealth) || maxHealth <= 0.0) {
        return 0.0;
    }
    const double fraction = std::isfinite(profile.respawnHealth) ? std::clamp(profile.respawnHealth, 0.01, 1.0) : 1.0;
    return maxHealth * fraction;
}

} // namespace spn::core
