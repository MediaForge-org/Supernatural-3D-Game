#include <cstddef>
#include <cstdint>
#include <doctest.h>
#include <limits>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <spn/core/Pickup.h>
#include <spn/core/TestDoor.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace spn::core;

// Proposal 0007, Phase 0.5: pickup interactable and per-holder inventory counts (placeholders).

namespace {

constexpr std::string_view kPlayer = "player.local";

InteractableRef Pickup(std::string id) {
    return InteractableRef{TestPickupKind(), std::move(id)};
}

InteractionRegistry MakeRegistry() {
    InteractionRegistry registry;
    REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
    REQUIRE_FALSE(RegisterTestPickup(registry).has_value());
    return registry;
}

InteractionDefaults Authored(std::string item, std::int64_t amount = 1) {
    InteractionDefaults defaults;
    defaults.values.emplace(kTestPickupItemState, std::move(item));
    defaults.values.emplace(kTestPickupAmountState, amount);
    return defaults;
}

/// A pickup interaction that must succeed.
InteractionOutcome Take(const InteractionRegistry& registry, WorldFacts& facts, std::string id,
                        const InteractionDefaults& defaults,
                        std::string_view holder = kDefaultInteractionHolder,
                        std::string_view cause = "test") {
    auto result = registry.Interact(facts, Pickup(std::move(id)), InteractionVerb::Use, defaults,
                                    cause, holder);
    REQUIRE_MESSAGE(result.HasValue(), "unexpected error: ", ToString(result.Error().code));
    return std::move(result).Value();
}

/// A pickup interaction that must fail.
InteractionError TakeFails(const InteractionRegistry& registry, WorldFacts& facts, std::string id,
                           const InteractionDefaults& defaults,
                           std::string_view holder = kDefaultInteractionHolder) {
    const auto result = registry.Interact(facts, Pickup(std::move(id)), InteractionVerb::Use,
                                          defaults, "test", holder);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

std::int64_t Count(const WorldFacts& facts, std::string_view holder, std::string_view item) {
    const auto count = InventoryCount(facts, holder, item);
    REQUIRE(count.HasValue());
    return count.Value();
}

} // namespace

TEST_CASE("Pickup 1: Use takes the object once and adds its amount to the holder's count") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto outcome = Take(registry, facts, "crate_a", Authored("test_item", 3), kPlayer, "unreal.use:Player");

    CHECK(outcome.changes.size() == 2);
    CHECK(facts.Revision() == 2);
    for (const ChangeRecord& change : outcome.changes) {
        CHECK(change.cause == "unreal.use:Player");
    }
    CHECK(*facts.FindAs<bool>("placeholder.pickup.crate_a.taken"));
    CHECK(*facts.FindAs<std::int64_t>("placeholder.inventory.player.local.test_item.count") == 3);
    CHECK(Count(facts, kPlayer, "test_item") == 3);
}

TEST_CASE("Pickup 2: a second Use on the same instance is refused and writes nothing") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    Take(registry, facts, "crate_a", Authored("test_item", 2));
    const WorldFacts before = facts;

    const InteractionError error = TakeFails(registry, facts, "crate_a", Authored("test_item", 2));
    CHECK(error.code == InteractionErrorCode::Refused);
    CHECK(error.feedback == TestPickupGoneFeedback());
    CHECK(facts == before);
    CHECK(facts.Revision() == 2);
    CHECK(Count(facts, kPlayer, "test_item") == 2);
}

TEST_CASE("Pickup 3: authored defaults - amount defaults to 1, bad item or amount is rejected") {
    const auto registry = MakeRegistry();
    WorldFacts facts;

    InteractionDefaults noAmount;
    noAmount.values.emplace(kTestPickupItemState, std::string("test_item"));
    Take(registry, facts, "crate_a", noAmount);
    CHECK(Count(facts, kPlayer, "test_item") == 1);

    const WorldFacts before = facts;
    CHECK(TakeFails(registry, facts, "crate_b", InteractionDefaults{}).code == InteractionErrorCode::StateCorrupt);
    for (const char* bad : {"", "Upper", "1st", "has.dot", "with space", "dash-item", "_lead"}) {
        CAPTURE(bad);
        CHECK(TakeFails(registry, facts, "crate_b", Authored(bad)).code == InteractionErrorCode::InvalidId);
    }
    CHECK(TakeFails(registry, facts, "crate_b", Authored(std::string(kMaxItemIdLength + 1, 'a'))).code == InteractionErrorCode::InvalidId);
    CHECK(TakeFails(registry, facts, "crate_b", Authored("test_item", 0)).code == InteractionErrorCode::StateCorrupt);
    CHECK(TakeFails(registry, facts, "crate_b", Authored("test_item", -4)).code == InteractionErrorCode::StateCorrupt);

    InteractionDefaults wrongType;
    wrongType.values.emplace(kTestPickupItemState, true);
    CHECK(TakeFails(registry, facts, "crate_b", wrongType).code == InteractionErrorCode::StateCorrupt);
    InteractionDefaults wrongAmount;
    wrongAmount.values.emplace(kTestPickupItemState, std::string("test_item"));
    wrongAmount.values.emplace(kTestPickupAmountState, std::string("two"));
    CHECK(TakeFails(registry, facts, "crate_b", wrongAmount).code == InteractionErrorCode::StateCorrupt);
    CHECK(facts == before);
}

TEST_CASE("Pickup 4: counts add up per item, and ListInventory is ordered by item id") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    Take(registry, facts, "crate_a", Authored("zebra", 2));
    Take(registry, facts, "crate_b", Authored("alpha", 1));
    Take(registry, facts, "crate_c", Authored("zebra", 5));

    CHECK(Count(facts, kPlayer, "zebra") == 7);
    CHECK(Count(facts, kPlayer, "alpha") == 1);
    CHECK(Count(facts, kPlayer, "never_seen") == 0);

    const auto list = ListInventory(facts, kPlayer);
    REQUIRE(list.HasValue());
    CHECK(list.Value() == std::vector<InventoryEntry>{{"alpha", 1}, {"zebra", 7}});
    const auto empty = ListInventory(facts, "coop.p2");
    REQUIRE(empty.HasValue());
    CHECK(empty.Value().empty());
}

TEST_CASE("Pickup 5: holders are separate, and an instance is taken once for everybody") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    Take(registry, facts, "crate_a", Authored("test_item", 2), "coop.p2");
    Take(registry, facts, "crate_b", Authored("test_item", 4), kPlayer);

    CHECK(Count(facts, "coop.p2", "test_item") == 2);
    CHECK(Count(facts, kPlayer, "test_item") == 4);

    // The other holder cannot take what is already gone.
    CHECK(TakeFails(registry, facts, "crate_a", Authored("test_item", 2), kPlayer).code == InteractionErrorCode::Refused);
    CHECK(Count(facts, kPlayer, "test_item") == 4);

    // A holder id that extends another ("player" vs "player.local") must not leak.
    Take(registry, facts, "crate_c", Authored("test_item", 1), "player");
    const auto shorter = ListInventory(facts, "player");
    REQUIRE(shorter.HasValue());
    CHECK(shorter.Value() == std::vector<InventoryEntry>{{"test_item", 1}});
    const auto longer = ListInventory(facts, kPlayer);
    REQUIRE(longer.HasValue());
    CHECK(longer.Value() == std::vector<InventoryEntry>{{"test_item", 4}});
}

TEST_CASE("Pickup 6: the default holder is player.local, and invalid holders change nothing") {
    const auto registry = MakeRegistry();
    WorldFacts implicitHolder;
    WorldFacts explicitHolder;
    const auto result = registry.Interact(implicitHolder, Pickup("crate_a"), InteractionVerb::Use, Authored("test_item"), "test");
    REQUIRE(result.HasValue());
    Take(registry, explicitHolder, "crate_a", Authored("test_item"), "player.local");
    CHECK(implicitHolder == explicitHolder);

    WorldFacts facts;
    const WorldFacts before = facts;
    for (const char* bad : {"", "Player", "9p", "has space", "p-1"}) {
        CAPTURE(bad);
        const InteractionError error = TakeFails(registry, facts, "crate_a", Authored("test_item"), bad);
        CHECK(error.code == InteractionErrorCode::InvalidId);
    }
    const std::string tooLong(kMaxHolderIdLength + 1, 'p');
    CHECK(TakeFails(registry, facts, "crate_a", Authored("test_item"), tooLong).code == InteractionErrorCode::InvalidId);
    CHECK(facts == before);

    CHECK_FALSE(InventoryCount(facts, "Bad", "test_item").HasValue());
    CHECK_FALSE(InventoryCount(facts, kPlayer, "bad.item").HasValue());
    CHECK_FALSE(ListInventory(facts, "Bad").HasValue());
}

TEST_CASE("Pickup 7: wrong fact types give StateCorrupt and change nothing") {
    const auto registry = MakeRegistry();

    WorldFacts badCount;
    REQUIRE(badCount.Apply(SetFact{"placeholder.inventory.player.local.test_item.count", std::string("many")}).HasValue());
    WorldFacts before = badCount;
    CHECK(TakeFails(registry, badCount, "crate_a", Authored("test_item")).code == InteractionErrorCode::StateCorrupt);
    CHECK(badCount == before);
    CHECK_FALSE(InventoryCount(badCount, kPlayer, "test_item").HasValue());
    CHECK_FALSE(ListInventory(badCount, kPlayer).HasValue());

    WorldFacts badTaken;
    REQUIRE(badTaken.Apply(SetFact{"placeholder.pickup.crate_a.taken", std::int64_t{1}}).HasValue());
    before = badTaken;
    CHECK(TakeFails(registry, badTaken, "crate_a", Authored("test_item")).code == InteractionErrorCode::StateCorrupt);
    CHECK(badTaken == before);
}

TEST_CASE("Pickup 8: integer overflow of the count is an error and the pickup stays untaken") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{InventoryCountKey(kPlayer, "test_item"), std::numeric_limits<std::int64_t>::max()}).HasValue());
    const WorldFacts before = facts;

    const InteractionError error = TakeFails(registry, facts, "crate_a", Authored("test_item", 1));
    CHECK(error.code == InteractionErrorCode::Fact);
    REQUIRE(error.fact.has_value());
    CHECK(error.fact->code == FactErrorCode::IntegerOverflow);
    CHECK(facts == before); // the taken flag was not left behind
    CHECK(facts.FindAs<bool>("placeholder.pickup.crate_a.taken") == nullptr);
}

TEST_CASE("Pickup 9: an authored taken=true default is refused without any fact, and AvailableVerbs is {Use}") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    InteractionDefaults defaults = Authored("test_item");
    defaults.values.emplace(kTestPickupTakenState, true);

    CHECK(TakeFails(registry, facts, "crate_a", defaults).code == InteractionErrorCode::Refused);
    CHECK(facts.Revision() == 0);

    const auto verbs = registry.AvailableVerbs(facts, Pickup("crate_a"), defaults);
    REQUIRE(verbs.HasValue());
    CHECK(verbs.Value() == std::vector<InteractionVerb>{InteractionVerb::Use});

    // Offered ⇒ Interact succeeds or is refused (consistency with the door tests).
    WorldFacts fresh;
    const auto fresh_verbs = registry.AvailableVerbs(fresh, Pickup("crate_b"), Authored("test_item"));
    REQUIRE(fresh_verbs.HasValue());
    CHECK(fresh_verbs.Value() == std::vector<InteractionVerb>{InteractionVerb::Use});
    CHECK_FALSE(registry.AvailableVerbs(fresh, Pickup("crate_b"), InteractionDefaults{}).HasValue());
}

TEST_CASE("Pickup 10: invalid instance ids and unknown kinds are rejected before any change") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    CHECK(TakeFails(registry, facts, "Bad Id", Authored("test_item")).code == InteractionErrorCode::InvalidId);
    CHECK(TakeFails(registry, facts, std::string(kMaxInteractableIdLength + 1, 'a'), Authored("test_item")).code == InteractionErrorCode::InvalidId);

    InteractionRegistry doorsOnly;
    REQUIRE_FALSE(RegisterTestDoor(doorsOnly).has_value());
    CHECK(TakeFails(doorsOnly, facts, "crate_a", Authored("test_item")).code == InteractionErrorCode::UnknownKind);
    CHECK(facts.Revision() == 0);
}

TEST_CASE("Pickup 11: the longest ids still form valid fact keys (key-length rule)") {
    const std::string holder(kMaxHolderIdLength, 'h');
    const std::string item(kMaxItemIdLength, 'i');
    const std::string id(kMaxInteractableIdLength, 'p');
    CHECK(IsValidHolderId(holder));
    CHECK(IsValidItemId(item));
    CHECK(IsValidFactKey(InventoryCountKey(holder, item)));
    CHECK(IsValidFactKey(TestPickupTakenKey(id)));
    CHECK(InventoryCountKey(holder, item).size() <= kMaxFactKeyLength);

    const auto registry = MakeRegistry();
    WorldFacts facts;
    Take(registry, facts, id, Authored(item, 9), holder);
    CHECK(Count(facts, holder, item) == 9);
}

TEST_CASE("Pickup 12: item and holder id rules") {
    CHECK(IsValidItemId("a"));
    CHECK(IsValidItemId("old_key_2"));
    CHECK(IsValidItemId(std::string(kMaxItemIdLength, 'x')));
    CHECK_FALSE(IsValidItemId(""));
    CHECK_FALSE(IsValidItemId(std::string(kMaxItemIdLength + 1, 'x')));
    CHECK_FALSE(IsValidItemId("Key"));
    CHECK_FALSE(IsValidItemId("9key"));
    CHECK_FALSE(IsValidItemId("a.b"));
    CHECK_FALSE(IsValidItemId("a b"));
    CHECK_FALSE(IsValidItemId(std::string_view("a\0b", 3)));

    CHECK(IsValidHolderId("player.local"));
    CHECK(IsValidHolderId(kDefaultInteractionHolder));
    CHECK(IsValidHolderId(std::string(kMaxHolderIdLength, 'h')));
    CHECK_FALSE(IsValidHolderId(std::string(kMaxHolderIdLength + 1, 'h')));
    CHECK_FALSE(IsValidHolderId(""));
    CHECK_FALSE(IsValidHolderId("Player"));
}

TEST_CASE("Pickup 13: determinism - the same sequence gives equal stores and bytes") {
    const auto registry = MakeRegistry();
    WorldFacts a;
    WorldFacts b;
    for (WorldFacts* facts : {&a, &b}) {
        Take(registry, *facts, "crate_a", Authored("alpha", 2));
        Take(registry, *facts, "crate_b", Authored("alpha", 3), "coop.p2");
        Take(registry, *facts, "crate_c", Authored("beta"));
    }
    CHECK(a == b);
    CHECK(a.Serialize() == b.Serialize());
}

TEST_CASE("Pickup 14: save/load round trip keeps counts, taken state and history") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    Take(registry, facts, "crate_a", Authored("alpha", 2));
    Take(registry, facts, "crate_b", Authored("beta", 1));
    const std::string saved = facts.Serialize();

    auto loaded = WorldFacts::Deserialize(saved);
    REQUIRE(loaded.HasValue());
    WorldFacts restored = std::move(loaded).Value();
    CHECK(restored == facts);
    CHECK(restored.Serialize() == saved); // byte-identical re-save
    CHECK(Count(restored, kPlayer, "alpha") == 2);
    CHECK(Count(restored, kPlayer, "beta") == 1);

    // A taken pickup stays taken after the load, an untouched one can still be taken.
    CHECK(TakeFails(registry, restored, "crate_a", Authored("alpha", 2)).code == InteractionErrorCode::Refused);
    Take(registry, restored, "crate_c", Authored("alpha", 5));
    CHECK(Count(restored, kPlayer, "alpha") == 7);

    // History: before the first pickup the inventory was empty, after it the count was 2.
    const auto empty = facts.StateAt(0);
    REQUIRE(empty.HasValue());
    CHECK(Count(empty.Value(), kPlayer, "alpha") == 0);
    const auto afterFirst = facts.StateAt(2);
    REQUIRE(afterFirst.HasValue());
    CHECK(Count(afterFirst.Value(), kPlayer, "alpha") == 2);
    CHECK(Count(afterFirst.Value(), kPlayer, "beta") == 0);
}

TEST_CASE("Pickup 15: a save with pickup facts loads even if the kind is not registered (mod case)") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    Take(registry, facts, "crate_a", Authored("alpha", 2));
    auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == facts);

    InteractionRegistry noPickups;
    REQUIRE_FALSE(RegisterTestDoor(noPickups).has_value());
    WorldFacts copy = loaded.Value();
    CHECK(TakeFails(noPickups, copy, "crate_b", Authored("alpha")).code == InteractionErrorCode::UnknownKind);
    CHECK(copy == facts);
}

TEST_CASE("Pickup 16: registration - planForHolder alone is a valid handler, neither function is not") {
    InteractionRegistry registry;
    InteractionHandler none;
    none.availableVerbs = [](const WorldFacts&, const InteractableRef&, const InteractionDefaults&)
        -> Result<std::vector<InteractionVerb>, InteractionError> {
        return std::vector<InteractionVerb>{InteractionVerb::Use};
    };
    const auto rejected = registry.Register(*ContentId::Parse("placeholder:interactable.none"), none);
    REQUIRE(rejected.has_value());
    CHECK(rejected->code == InteractionErrorCode::InvalidRegistration);

    REQUIRE_FALSE(RegisterTestPickup(registry).has_value());
    const auto duplicate = RegisterTestPickup(registry);
    REQUIRE(duplicate.has_value());
    CHECK(duplicate->code == InteractionErrorCode::DuplicateKind);
}

TEST_CASE("Pickup 17: the test door ignores the holder, except that it must be valid") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const InteractableRef door{TestDoorKind(), "door_a"};
    const auto opened = registry.Interact(facts, door, InteractionVerb::Use, InteractionDefaults{}, "test", "coop.p2");
    REQUIRE(opened.HasValue());
    CHECK(opened.Value().changes.size() == 1);
    const WorldFacts before = facts;
    const auto bad = registry.Interact(facts, door, InteractionVerb::Use, InteractionDefaults{}, "test", "Bad Holder");
    REQUIRE_FALSE(bad.HasValue());
    CHECK(bad.Error().code == InteractionErrorCode::InvalidId);
    CHECK(facts == before);
}
