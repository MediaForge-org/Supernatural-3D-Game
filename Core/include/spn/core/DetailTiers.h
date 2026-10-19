#pragma once

// Distance-to-tier mapping for level-of-detail decisions.
//
// Confirmed requirement (Docs/Proposals/0003 section 1): NPC detail and simulation levels
// depend on distance. This type is only the DATA side: a validated table of thresholds that
// is loaded from configuration (tier counts and distances are NOT decided, nothing here is a
// gameplay value). Two instances are expected, one for visual detail and one for simulation
// detail; what a tier means (mesh quality, hair cards vs. strands, full AI vs. routine vs.
// statistical) is up to the Unreal adapter and the NPC simulation. Distances are integer
// centimetres, so nothing depends on world size.
//
// ENGINEERING PROPOSAL, not master canon.

#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace spn::core {

inline constexpr std::size_t kMaxDetailTiers = 16;

class DetailTierTable {
public:
    /// `upperBoundsCm` are the inclusive upper distance limits of tier 0, 1, ..., n-1; the
    /// last tier (index n) is unbounded. May be empty (a single tier). Must be strictly
    /// ascending, > 0, and at most kMaxDetailTiers - 1 entries. Returns nullopt if
    /// the table is invalid.
    [[nodiscard]] static std::optional<DetailTierTable>
    Create(std::vector<std::uint32_t> upperBoundsCm);

    /// Tier for a distance: 0 = most detailed. Distances above the last bound give the
    /// last tier.
    [[nodiscard]] std::size_t TierFor(std::uint32_t distanceCm) const noexcept;

    [[nodiscard]] std::size_t TierCount() const noexcept { return upperBoundsCm_.size() + 1; }
    [[nodiscard]] const std::vector<std::uint32_t>& UpperBoundsCm() const noexcept {
        return upperBoundsCm_;
    }

    friend bool operator==(const DetailTierTable&, const DetailTierTable&) = default;

private:
    explicit DetailTierTable(std::vector<std::uint32_t> bounds)
        : upperBoundsCm_(std::move(bounds)) {}
    std::vector<std::uint32_t> upperBoundsCm_;
};

} // namespace spn::core
