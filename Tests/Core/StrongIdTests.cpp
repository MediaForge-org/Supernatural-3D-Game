#include <algorithm>
#include <cstdint>
#include <doctest.h>
#include <limits>
#include <set>
#include <spn/core/StrongId.h>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

// Placeholder tag types used only by these tests.
struct ItemTag {};
struct PlaceTag {};

using ItemId = spn::core::StrongId<ItemTag>;
using PlaceId = spn::core::StrongId<PlaceTag>;
using SmallItemId = spn::core::StrongId<ItemTag, std::uint32_t>;

// --- Compile-time type-safety checks -------------------------------------

template <typename A, typename B>
concept EqualityComparable = requires(A a, B b) { a == b; };
template <typename A, typename B>
concept Orderable = requires(A a, B b) { a < b; };

static_assert(EqualityComparable<ItemId, ItemId>);
static_assert(Orderable<ItemId, ItemId>);
// Different tags never compare, and ids never compare with raw integers.
static_assert(!EqualityComparable<ItemId, PlaceId>);
static_assert(!Orderable<ItemId, PlaceId>);
static_assert(!EqualityComparable<ItemId, std::uint64_t>);
static_assert(!Orderable<ItemId, int>);
// Different representations of the same tag are distinct types as well.
static_assert(!EqualityComparable<ItemId, SmallItemId>);
static_assert(!std::is_same_v<ItemId, PlaceId>);

// No implicit conversions in either direction; construction is explicit.
static_assert(!std::is_convertible_v<std::uint64_t, ItemId>);
static_assert(!std::is_convertible_v<ItemId, std::uint64_t>);
static_assert(!std::is_convertible_v<ItemId, PlaceId>);
static_assert(!std::is_convertible_v<PlaceId, ItemId>);
static_assert(std::is_constructible_v<ItemId, std::uint64_t>);
static_assert(!std::is_constructible_v<ItemId, PlaceId>);

// Plain value semantics, no overhead, safe to use across module boundaries.
static_assert(sizeof(ItemId) == sizeof(std::uint64_t));
static_assert(sizeof(SmallItemId) == sizeof(std::uint32_t));
static_assert(std::is_trivially_copyable_v<ItemId>);
static_assert(std::is_standard_layout_v<ItemId>);
static_assert(std::is_nothrow_default_constructible_v<ItemId>);
static_assert(std::is_nothrow_copy_constructible_v<ItemId>);

// Usable in constant expressions.
static_assert(ItemId{}.IsNull());
static_assert(ItemId{5}.Value() == 5);
static_assert(ItemId{1} < ItemId{2});
static_assert(ItemId{3} == ItemId{3});

// Hash support exists for each distinct id type.
static_assert(std::is_invocable_r_v<std::size_t, std::hash<ItemId>, ItemId>);
static_assert(std::is_invocable_r_v<std::size_t, std::hash<PlaceId>, PlaceId>);

} // namespace

TEST_CASE("StrongId: default-constructed id is the null id") {
    constexpr ItemId id;
    CHECK(id.IsNull());
    CHECK(id.Value() == 0);
    CHECK(id == ItemId{0});
}

TEST_CASE("StrongId: explicit construction keeps the value") {
    constexpr ItemId id{42};
    CHECK_FALSE(id.IsNull());
    CHECK(id.Value() == 42);
}

TEST_CASE("StrongId: maximum representable value is preserved") {
    constexpr auto kMax = std::numeric_limits<std::uint64_t>::max();
    CHECK(ItemId{kMax}.Value() == kMax);
    CHECK(SmallItemId{std::numeric_limits<std::uint32_t>::max()}.Value() ==
          std::numeric_limits<std::uint32_t>::max());
}

TEST_CASE("StrongId: comparison operators are consistent") {
    const ItemId a{1};
    const ItemId b{2};
    const ItemId a2{1};

    CHECK(a == a2);
    CHECK(a != b);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK(a <= a2);
    CHECK(b > a);
    CHECK(b >= a);
    CHECK(a >= a2);
    CHECK_FALSE(b < a);
    CHECK_FALSE(a < a2);
}

TEST_CASE("StrongId: ordered containers sort by value") {
    const std::set<ItemId> ids{ItemId{30}, ItemId{10}, ItemId{20}, ItemId{10}};
    REQUIRE(ids.size() == 3);
    std::vector<std::uint64_t> values;
    values.reserve(ids.size());
    for (const ItemId id : ids) {
        values.push_back(id.Value());
    }
    CHECK(values == std::vector<std::uint64_t>{10, 20, 30});

    std::vector<ItemId> v{ItemId{3}, ItemId{1}, ItemId{2}};
    std::ranges::sort(v);
    CHECK(v == std::vector<ItemId>{ItemId{1}, ItemId{2}, ItemId{3}});
}

TEST_CASE("StrongId: std::hash is consistent with equality") {
    const std::hash<ItemId> hasher;
    CHECK(hasher(ItemId{7}) == hasher(ItemId{7}));
    CHECK(hasher(ItemId{7}) == std::hash<std::uint64_t>{}(7));
    CHECK(hasher(ItemId{7}) != hasher(ItemId{8}));
}

TEST_CASE("StrongId: usable as unordered container key") {
    std::unordered_set<ItemId> set;
    CHECK(set.insert(ItemId{1}).second);
    CHECK(set.insert(ItemId{2}).second);
    CHECK_FALSE(set.insert(ItemId{1}).second);
    CHECK(set.size() == 2);
    CHECK(set.contains(ItemId{2}));
    CHECK_FALSE(set.contains(ItemId{3}));

    std::unordered_map<PlaceId, int> map;
    map[PlaceId{9}] = 90;
    map[PlaceId{9}] += 1;
    REQUIRE(map.size() == 1);
    CHECK(map.at(PlaceId{9}) == 91);
}

TEST_CASE("StrongId: same numeric value in different domains stays distinct types") {
    // Equality across tags does not compile (see static_asserts above); here we
    // only confirm the values themselves are independent per type.
    const ItemId item{5};
    const PlaceId place{5};
    CHECK(item.Value() == place.Value());
}
