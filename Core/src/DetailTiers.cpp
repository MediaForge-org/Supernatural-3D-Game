#include <algorithm>
#include <spn/core/DetailTiers.h>
#include <utility>

namespace spn::core {

std::optional<DetailTierTable> DetailTierTable::Create(std::vector<std::uint32_t> upperBoundsCm) {
    if (upperBoundsCm.size() >= kMaxDetailTiers) {
        return std::nullopt;
    }
    if (!upperBoundsCm.empty() && upperBoundsCm.front() == 0) {
        return std::nullopt;
    }
    if (std::ranges::adjacent_find(upperBoundsCm, std::greater_equal<>{}) != upperBoundsCm.end()) {
        return std::nullopt;
    }
    return DetailTierTable(std::move(upperBoundsCm));
}

std::size_t DetailTierTable::TierFor(std::uint32_t distanceCm) const noexcept {
    const auto it = std::ranges::lower_bound(upperBoundsCm_, distanceCm);
    return static_cast<std::size_t>(it - upperBoundsCm_.begin());
}

} // namespace spn::core
