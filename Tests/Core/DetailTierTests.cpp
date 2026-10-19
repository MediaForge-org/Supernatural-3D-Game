#include <cstdlib>
#include <doctest.h>
#include <spn/core/DetailTiers.h>

using spn::core::DetailTierTable;

namespace {
/// Builds a table that must be valid.
DetailTierTable Make(std::vector<std::uint32_t> bounds) {
    auto table = DetailTierTable::Create(std::move(bounds));
    if (!table) {
        FAIL("expected a valid tier table");
        std::abort(); // unreachable: FAIL aborts the test case
    }
    return *std::move(table);
}
} // namespace

TEST_CASE("DetailTiers: maps distances to tiers with inclusive upper bounds") {
    const DetailTierTable tiers = Make({1000, 5000, 20000});
    CHECK(tiers.TierCount() == 4);
    CHECK(tiers.TierFor(0) == 0);
    CHECK(tiers.TierFor(1000) == 0);
    CHECK(tiers.TierFor(1001) == 1);
    CHECK(tiers.TierFor(5000) == 1);
    CHECK(tiers.TierFor(5001) == 2);
    CHECK(tiers.TierFor(20000) == 2);
    CHECK(tiers.TierFor(20001) == 3);
    CHECK(tiers.TierFor(UINT32_MAX) == 3);
}

TEST_CASE("DetailTiers: an empty table is a single tier") {
    const DetailTierTable tiers = Make({});
    CHECK(tiers.TierCount() == 1);
    CHECK(tiers.TierFor(0) == 0);
    CHECK(tiers.TierFor(UINT32_MAX) == 0);
}

TEST_CASE("DetailTiers: rejects invalid tables") {
    CHECK_FALSE(DetailTierTable::Create({0}).has_value());          // zero bound
    CHECK_FALSE(DetailTierTable::Create({100, 100}).has_value());   // not strictly ascending
    CHECK_FALSE(DetailTierTable::Create({200, 100}).has_value());   // descending
    CHECK_FALSE(DetailTierTable::Create({1, 2, 2, 3}).has_value()); // duplicate in the middle

    std::vector<std::uint32_t> many;
    for (std::uint32_t i = 1; i < spn::core::kMaxDetailTiers; ++i) {
        many.push_back(i);
    }
    CHECK(DetailTierTable::Create(many).has_value()); // kMaxDetailTiers - 1 bounds = max tiers
    many.push_back(spn::core::kMaxDetailTiers);
    CHECK_FALSE(DetailTierTable::Create(many).has_value());
}

TEST_CASE("DetailTiers: independent tables for visual and simulation detail") {
    const DetailTierTable visual = Make({2000, 8000});
    const DetailTierTable simulation = Make({10000, 50000, 200000});
    CHECK(visual.TierFor(9000) == 2);
    CHECK(simulation.TierFor(9000) == 0);
    CHECK(visual == Make({2000, 8000}));
}
