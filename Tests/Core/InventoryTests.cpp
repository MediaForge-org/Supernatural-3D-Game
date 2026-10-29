#include <doctest.h>
#include <limits>
#include <spn/core/Inventory.h>

#include "MapTestData.h" // Cid, Lcg

using namespace spn::core;
using namespace spn::testdata;

namespace {

// Test items. The numbers (stack limits, weights) are arbitrary test values, not game balance.
struct Fixture {
    ItemCatalog catalog;
    ContentId coin = Cid("test:item.coin");       // stackable, max 10
    ContentId bandage = Cid("test:item.bandage"); // stackable, max 5, weight 100 g, volume 50 ml
    ContentId knife = Cid("test:item.knife");     // unique, weight 300 g, equippable in hand
    ContentId coat = Cid("test:item.coat");       // unique, equippable on torso
    ContentId brick = Cid("test:item.brick");     // unique, very heavy
    ContentId hand = Cid("test:slot.hand");
    ContentId torso = Cid("test:slot.torso");

    Fixture() {
        const auto add = [&](ItemDefinition d) { REQUIRE_FALSE(catalog.Add(std::move(d)).has_value()); };
        ItemDefinition c;
        c.id = coin;
        c.category = Cid("test:cat.currency");
        c.maxStack = 10;
        add(c);
        ItemDefinition b;
        b.id = bandage;
        b.category = Cid("test:cat.medical");
        b.maxStack = 5;
        b.weightGrams = 100;
        b.volumeMilliliters = 50;
        add(b);
        ItemDefinition k;
        k.id = knife;
        k.category = Cid("test:cat.tool");
        k.weightGrams = 300;
        k.equipSlots = {hand};
        add(k);
        ItemDefinition t;
        t.id = coat;
        t.category = Cid("test:cat.clothing");
        t.equipSlots = {torso};
        add(t);
        ItemDefinition br;
        br.id = brick;
        br.category = Cid("test:cat.junk");
        br.weightGrams = std::numeric_limits<std::uint64_t>::max() / 2 + 1;
        add(br);
    }

    ContainerRules Rules(std::optional<std::uint32_t> stacks = std::nullopt, std::optional<std::uint64_t> weight = std::nullopt,
                         std::optional<std::uint64_t> volume = std::nullopt) const {
        ContainerRules r;
        r.kind = Cid("test:container.generic");
        r.maxStacks = stacks;
        r.maxWeightGrams = weight;
        r.maxVolumeMilliliters = volume;
        r.equipSlots = {hand, torso};
        return r;
    }
};

constexpr ContainerId kBag{1};
constexpr ContainerId kTrunk{2};

Result<TransactionResult, InventoryError> Run(InventoryState& s, const InventoryCommand& c) { return s.Apply(c); }

InventoryErrorCode CodeOf(const Result<TransactionResult, InventoryError>& r) {
    REQUIRE_FALSE(r.HasValue());
    return r.Error().code;
}

void Create(InventoryState& s, ContainerId id, const ContainerRules& rules) { REQUIRE(Run(s, CreateContainer{id, rules}).HasValue()); }

ItemInstanceId AddOne(InventoryState& s, ContainerId c, const ContentId& def, std::uint32_t qty = 1) {
    const auto r = Run(s, AddItem{c, def, qty, {}});
    REQUIRE(r.HasValue());
    REQUIRE_FALSE(r.Value().createdStacks.empty());
    return r.Value().createdStacks.front();
}

std::uint32_t Quantity(const InventoryState& s, ContainerId c, ItemInstanceId i) {
    const ItemStack* stack = s.FindStack(c, i);
    return stack != nullptr ? stack->quantity : 0;
}

} // namespace

// --- Catalog --------------------------------------------------------------------------------------------------------------------

TEST_CASE("ItemCatalog validates definitions") {
    ItemCatalog catalog;
    ItemDefinition ok;
    ok.id = Cid("t:a");
    ok.category = Cid("t:c");
    CHECK_FALSE(catalog.Add(ok).has_value());
    CHECK(catalog.Add(ok)->code == InventoryErrorCode::DuplicateDefinition);
    ItemDefinition noId = ok;
    noId.id = ContentId();
    CHECK(catalog.Add(noId)->code == InventoryErrorCode::InvalidDefinition);
    ItemDefinition noCategory = ok;
    noCategory.id = Cid("t:b");
    noCategory.category = ContentId();
    CHECK(catalog.Add(noCategory)->code == InventoryErrorCode::InvalidDefinition);
    ItemDefinition zeroStack = ok;
    zeroStack.id = Cid("t:z");
    zeroStack.maxStack = 0;
    CHECK(catalog.Add(zeroStack)->code == InventoryErrorCode::InvalidDefinition);
    ItemDefinition hugeStack = zeroStack;
    hugeStack.maxStack = kMaxStackLimit + 1;
    CHECK(catalog.Add(hugeStack)->code == InventoryErrorCode::InvalidDefinition);
    ItemDefinition stackableEquippable = ok;
    stackableEquippable.id = Cid("t:s");
    stackableEquippable.maxStack = 2;
    stackableEquippable.equipSlots = {Cid("t:slot")};
    CHECK(catalog.Add(stackableEquippable)->code == InventoryErrorCode::InvalidDefinition);
    CHECK(catalog.Size() == 1); // failed adds left nothing behind
    CHECK(catalog.Find(Cid("t:a")) != nullptr);
    CHECK(catalog.Find(Cid("t:z")) == nullptr);
}

// --- Items and stacks -----------------------------------------------------------------------------------------------------------

TEST_CASE("AddItem: stackable items fill existing stacks before creating new ones") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const ItemInstanceId first = AddOne(s, kBag, f.coin, 7);
    CHECK(Quantity(s, kBag, first) == 7);
    const auto more = Run(s, AddItem{kBag, f.coin, 25, {}});
    REQUIRE(more.HasValue());
    CHECK(Quantity(s, kBag, first) == 10);            // topped up to the limit
    CHECK(more.Value().createdStacks.size() == 3);    // 3 went into the first stack; the other 22 became 10 + 10 + 2
    CHECK(s.CountOf(kBag, f.coin) == 32);
    CHECK(s.FindContainer(kBag)->stacks.size() == 4); // 10, 10, 10, 2
    CHECK_FALSE(s.CheckInvariants().has_value());
}

TEST_CASE("AddItem: non-stackable items are unique instances with their own state") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    FieldMap state{{"sharpness", std::int64_t{7}}, {"engraved", true}, {"note", std::string("from dad")}};
    const auto a = Run(s, AddItem{kBag, f.knife, 1, state});
    const auto b = Run(s, AddItem{kBag, f.knife, 1, {}});
    REQUIRE(a.HasValue());
    REQUIRE(b.HasValue());
    const ItemInstanceId ia = a.Value().createdStacks.front();
    const ItemInstanceId ib = b.Value().createdStacks.front();
    CHECK(ia != ib);
    CHECK(s.FindStack(kBag, ia)->state == state);
    CHECK(s.FindStack(kBag, ib)->state.empty());
    CHECK(s.CountOf(kBag, f.knife) == 2);
    CHECK(s.LocateStack(ia) == kBag);
    CHECK(s.LocateStack(ItemInstanceId(999)).IsNull());
}

TEST_CASE("AddItem: invalid operations are rejected and change nothing") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const InventoryState before = s;
    const std::uint64_t revision = s.Revision();
    CHECK(CodeOf(Run(s, AddItem{ContainerId(9), f.coin, 1, {}})) == InventoryErrorCode::UnknownContainer);
    CHECK(CodeOf(Run(s, AddItem{kBag, Cid("test:item.ghost"), 1, {}})) == InventoryErrorCode::UnknownDefinition);
    CHECK(CodeOf(Run(s, AddItem{kBag, f.coin, 0, {}})) == InventoryErrorCode::InvalidQuantity);
    CHECK(CodeOf(Run(s, AddItem{kBag, f.knife, 2, {}})) == InventoryErrorCode::InvalidQuantity); // unique items: one at a time
    CHECK(CodeOf(Run(s, AddItem{kBag, f.coin, 1, FieldMap{{"x", std::int64_t{1}}}})) == InventoryErrorCode::InvalidState); // fungible: no state
    CHECK(CodeOf(Run(s, AddItem{kBag, f.knife, 1, FieldMap{{"Bad Name", std::int64_t{1}}}})) == InventoryErrorCode::InvalidState);
    FieldMap tooMany;
    for (std::size_t i = 0; i <= kMaxItemStateFields; ++i) {
        tooMany.emplace("f" + std::to_string(i), std::int64_t{1});
    }
    CHECK(CodeOf(Run(s, AddItem{kBag, f.knife, 1, tooMany})) == InventoryErrorCode::InvalidState);
    CHECK(s == before);
    CHECK(s.Revision() == revision); // rejected commands do not count as changes
}

TEST_CASE("RemoveItem: partial and full removal, invalid quantities") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const ItemInstanceId coins = AddOne(s, kBag, f.coin, 8);
    CHECK(Run(s, RemoveItem{kBag, coins, 3}).HasValue());
    CHECK(Quantity(s, kBag, coins) == 5);
    CHECK(CodeOf(Run(s, RemoveItem{kBag, coins, 6})) == InventoryErrorCode::InvalidQuantity);
    CHECK(CodeOf(Run(s, RemoveItem{kBag, coins, 0})) == InventoryErrorCode::InvalidQuantity);
    CHECK(CodeOf(Run(s, RemoveItem{kBag, ItemInstanceId(77), 1})) == InventoryErrorCode::UnknownItem);
    CHECK(Run(s, RemoveItem{kBag, coins, 5}).HasValue());
    CHECK(s.FindStack(kBag, coins) == nullptr); // an empty stack does not linger
    CHECK(s.FindContainer(kBag)->stacks.empty());
}

TEST_CASE("SplitStack: creates a new stack, preserves the total, validates") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const ItemInstanceId coins = AddOne(s, kBag, f.coin, 8);
    const auto split = Run(s, SplitStack{kBag, coins, 3});
    REQUIRE(split.HasValue());
    REQUIRE(split.Value().createdStacks.size() == 1);
    const ItemInstanceId piece = split.Value().createdStacks.front();
    CHECK(Quantity(s, kBag, coins) == 5);
    CHECK(Quantity(s, kBag, piece) == 3);
    CHECK(s.CountOf(kBag, f.coin) == 8);
    CHECK(CodeOf(Run(s, SplitStack{kBag, coins, 5})) == InventoryErrorCode::InvalidQuantity); // would leave an empty stack
    CHECK(CodeOf(Run(s, SplitStack{kBag, coins, 0})) == InventoryErrorCode::InvalidQuantity);
    const ItemInstanceId knife = AddOne(s, kBag, f.knife);
    CHECK(CodeOf(Run(s, SplitStack{kBag, knife, 1})) == InventoryErrorCode::NotStackable);
}

TEST_CASE("SplitStack respects the stack capacity of the container") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules(1));
    const ItemInstanceId coins = AddOne(s, kBag, f.coin, 8);
    CHECK(CodeOf(Run(s, SplitStack{kBag, coins, 3})) == InventoryErrorCode::StackLimitReached);
    CHECK(Quantity(s, kBag, coins) == 8);
}

TEST_CASE("MergeStacks: merges up to the limit, removes the empty source, validates") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const ItemInstanceId a = AddOne(s, kBag, f.coin, 10);
    const ItemInstanceId b = AddOne(s, kBag, f.coin, 6); // a is full, so this becomes its own stack
    const ItemInstanceId c = Run(s, SplitStack{kBag, b, 2}).Value().createdStacks.front(); // b = 4, c = 2
    CHECK(CodeOf(Run(s, MergeStacks{kBag, b, a, 0})) == InventoryErrorCode::StackLimitReached); // target full
    CHECK(Run(s, MergeStacks{kBag, c, b, 0}).HasValue());
    CHECK(Quantity(s, kBag, b) == 6);
    CHECK(s.FindStack(kBag, c) == nullptr); // source emptied and gone
    CHECK(Run(s, RemoveItem{kBag, a, 5}).HasValue());
    CHECK(Run(s, MergeStacks{kBag, b, a, 0}).HasValue()); // only 5 fit: the rest stays behind
    CHECK(Quantity(s, kBag, a) == 10);
    CHECK(Quantity(s, kBag, b) == 1);
    CHECK(s.CountOf(kBag, f.coin) == 11);
    CHECK(Run(s, RemoveItem{kBag, a, 1}).HasValue());
    CHECK(CodeOf(Run(s, MergeStacks{kBag, b, a, 2})) == InventoryErrorCode::InvalidQuantity); // more than the source holds
    CHECK(Run(s, MergeStacks{kBag, b, a, 1}).HasValue());                                     // explicit amount
    CHECK(Quantity(s, kBag, a) == 10);
    CHECK(s.FindStack(kBag, b) == nullptr);
    CHECK(CodeOf(Run(s, MergeStacks{kBag, a, a, 0})) == InventoryErrorCode::InvalidId);
    const ItemInstanceId gauze = AddOne(s, kBag, f.bandage, 2);
    CHECK(CodeOf(Run(s, MergeStacks{kBag, gauze, a, 0})) == InventoryErrorCode::StackMismatch);
    const ItemInstanceId k1 = AddOne(s, kBag, f.knife);
    const ItemInstanceId k2 = AddOne(s, kBag, f.knife);
    CHECK(CodeOf(Run(s, MergeStacks{kBag, k1, k2, 0})) == InventoryErrorCode::NotStackable);
    CHECK(CodeOf(Run(s, MergeStacks{kBag, ItemInstanceId(500), a, 0})) == InventoryErrorCode::UnknownItem);
    CHECK_FALSE(s.CheckInvariants().has_value());
}

// --- Transfers ------------------------------------------------------------------------------------------------------------------

TEST_CASE("TransferItem: a whole unique item keeps its id and state") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    Create(s, kTrunk, f.Rules());
    FieldMap state{{"note", std::string("keep me")}};
    const ItemInstanceId knife = Run(s, AddItem{kBag, f.knife, 1, state}).Value().createdStacks.front();
    CHECK(Run(s, TransferItem{kBag, kTrunk, knife, 1}).HasValue());
    CHECK(s.FindStack(kBag, knife) == nullptr);
    REQUIRE(s.FindStack(kTrunk, knife) != nullptr);
    CHECK(s.FindStack(kTrunk, knife)->state == state);
    CHECK(s.LocateStack(knife) == kTrunk);
    CHECK_FALSE(s.CheckInvariants().has_value());
}

TEST_CASE("TransferItem: stackable items merge into matching stacks at the destination") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    Create(s, kTrunk, f.Rules());
    const ItemInstanceId bagCoins = AddOne(s, kBag, f.coin, 7);
    const ItemInstanceId trunkCoins = AddOne(s, kTrunk, f.coin, 8);
    // Moving all 7: 2 fill the trunk stack, the other 5 become a stack that KEEPS the source id.
    const auto r = Run(s, TransferItem{kBag, kTrunk, bagCoins, 7});
    REQUIRE(r.HasValue());
    CHECK(r.Value().createdStacks.empty()); // no new id was needed
    CHECK(Quantity(s, kTrunk, trunkCoins) == 10);
    CHECK(Quantity(s, kTrunk, bagCoins) == 5);
    CHECK(s.FindStack(kBag, bagCoins) == nullptr);
    CHECK(s.CountOf(kBag, f.coin) + s.CountOf(kTrunk, f.coin) == 15);
}

TEST_CASE("TransferItem: partial transfer leaves the rest and allocates ids deterministically") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    Create(s, kTrunk, f.Rules());
    const ItemInstanceId coins = AddOne(s, kBag, f.coin, 9);
    const auto r = Run(s, TransferItem{kBag, kTrunk, coins, 4});
    REQUIRE(r.HasValue());
    REQUIRE(r.Value().createdStacks.size() == 1);
    CHECK(Quantity(s, kBag, coins) == 5);
    CHECK(Quantity(s, kTrunk, r.Value().createdStacks.front()) == 4);
    CHECK(CodeOf(Run(s, TransferItem{kBag, kTrunk, coins, 6})) == InventoryErrorCode::InvalidQuantity);
    CHECK(CodeOf(Run(s, TransferItem{kBag, kTrunk, coins, 0})) == InventoryErrorCode::InvalidQuantity);
    CHECK(CodeOf(Run(s, TransferItem{kBag, kBag, coins, 1})) == InventoryErrorCode::SameContainer);
    CHECK(CodeOf(Run(s, TransferItem{kBag, ContainerId(9), coins, 1})) == InventoryErrorCode::UnknownContainer);
    CHECK(CodeOf(Run(s, TransferItem{kBag, kTrunk, ItemInstanceId(404), 1})) == InventoryErrorCode::UnknownItem);
}

TEST_CASE("Capacity: stack count, weight and volume limits are enforced on add and transfer") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules(2, 1000, 120)); // 2 stacks, 1000 g, 120 ml
    Create(s, kTrunk, f.Rules());
    AddOne(s, kBag, f.bandage, 2); // 200 g, 100 ml
    CHECK(CodeOf(Run(s, AddItem{kBag, f.bandage, 1, {}})) == InventoryErrorCode::VolumeLimitReached); // 150 ml > 120 ml
    CHECK(s.CountOf(kBag, f.bandage) == 2);
    AddOne(s, kBag, f.knife);      // 500 g total, 2 stacks
    CHECK(CodeOf(Run(s, AddItem{kBag, f.coin, 1, {}})) == InventoryErrorCode::StackLimitReached); // third stack
    CHECK(CodeOf(Run(s, AddItem{kBag, f.knife, 1, {}})) == InventoryErrorCode::StackLimitReached);
    // Weight: free a stack slot, then exceed 1000 g with knives (300 g each).
    Create(s, ContainerId(3), f.Rules(std::nullopt, 500));
    AddOne(s, ContainerId(3), f.knife);
    CHECK(CodeOf(Run(s, AddItem{ContainerId(3), f.knife, 1, {}})) == InventoryErrorCode::WeightLimitReached); // 600 g > 500 g
    // Transfers are checked against the destination.
    const ItemInstanceId k = AddOne(s, kTrunk, f.knife);
    CHECK(CodeOf(Run(s, TransferItem{kTrunk, ContainerId(3), k, 1})) == InventoryErrorCode::WeightLimitReached);
    CHECK(s.FindStack(kTrunk, k) != nullptr);
    CHECK(s.TotalWeightGrams(kBag) == 500);
    CHECK(s.TotalVolumeMilliliters(kBag) == 100);
    CHECK_FALSE(s.CheckInvariants().has_value());
}

TEST_CASE("Capacity: an exactly full container is allowed, one more is not") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules(std::nullopt, 900));
    CHECK(Run(s, AddItem{kBag, f.knife, 1, {}}).HasValue());
    CHECK(Run(s, AddItem{kBag, f.knife, 1, {}}).HasValue());
    CHECK(Run(s, AddItem{kBag, f.knife, 1, {}}).HasValue()); // 900 g = limit
    CHECK(CodeOf(Run(s, AddItem{kBag, f.bandage, 1, {}})) == InventoryErrorCode::WeightLimitReached);
}

TEST_CASE("Capacity: weight arithmetic that overflows is an error, never a wrapped value") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules()); // no limits at all: the totals are still computed safely
    CHECK(Run(s, AddItem{kBag, f.brick, 1, {}}).HasValue()); // 2^63 g
    CHECK(CodeOf(Run(s, AddItem{kBag, f.brick, 1, {}})) == InventoryErrorCode::ArithmeticOverflow); // 2^64 g does not fit
    CHECK(s.CountOf(kBag, f.brick) == 1);
    CHECK(s.TotalWeightGrams(kBag) == (std::numeric_limits<std::uint64_t>::max() / 2 + 1));
}

TEST_CASE("Category whitelist: containers can restrict what they accept") {
    Fixture f;
    InventoryState s(f.catalog);
    ContainerRules medical = f.Rules();
    medical.allowedCategories = {Cid("test:cat.medical")};
    Create(s, kBag, medical);
    Create(s, kTrunk, f.Rules());
    CHECK(Run(s, AddItem{kBag, f.bandage, 1, {}}).HasValue());
    CHECK(CodeOf(Run(s, AddItem{kBag, f.coin, 1, {}})) == InventoryErrorCode::CategoryNotAllowed);
    const ItemInstanceId coins = AddOne(s, kTrunk, f.coin, 3);
    CHECK(CodeOf(Run(s, TransferItem{kTrunk, kBag, coins, 3})) == InventoryErrorCode::CategoryNotAllowed);
}

// --- Equipment ------------------------------------------------------------------------------------------------------------------

TEST_CASE("Equip and unequip: slot rules") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const ItemInstanceId knife = AddOne(s, kBag, f.knife);
    const ItemInstanceId knife2 = AddOne(s, kBag, f.knife);
    const ItemInstanceId coat = AddOne(s, kBag, f.coat);
    const ItemInstanceId coins = AddOne(s, kBag, f.coin, 1);

    CHECK(Run(s, EquipItem{kBag, knife, f.hand}).HasValue());
    CHECK(s.FindContainer(kBag)->equipped.at(f.hand) == knife);
    CHECK(CodeOf(Run(s, EquipItem{kBag, knife2, f.hand})) == InventoryErrorCode::SlotOccupied);
    CHECK(CodeOf(Run(s, EquipItem{kBag, knife, f.hand})) == InventoryErrorCode::AlreadyEquipped);
    CHECK(CodeOf(Run(s, EquipItem{kBag, knife2, f.torso})) == InventoryErrorCode::NotEquippable); // the knife cannot go on the torso
    CHECK(CodeOf(Run(s, EquipItem{kBag, coins, f.hand})) == InventoryErrorCode::NotEquippable);
    CHECK(CodeOf(Run(s, EquipItem{kBag, coat, Cid("test:slot.nowhere")})) == InventoryErrorCode::UnknownSlot);
    CHECK(CodeOf(Run(s, EquipItem{kBag, ItemInstanceId(404), f.hand})) == InventoryErrorCode::UnknownItem);
    CHECK(Run(s, EquipItem{kBag, coat, f.torso}).HasValue());

    // Equipped items are pinned until unequipped.
    Create(s, kTrunk, f.Rules());
    CHECK(CodeOf(Run(s, RemoveItem{kBag, knife, 1})) == InventoryErrorCode::ItemEquipped);
    CHECK(CodeOf(Run(s, TransferItem{kBag, kTrunk, knife, 1})) == InventoryErrorCode::ItemEquipped);
    CHECK(Run(s, UnequipSlot{kBag, f.hand}).HasValue());
    CHECK(CodeOf(Run(s, UnequipSlot{kBag, f.hand})) == InventoryErrorCode::SlotEmpty);
    CHECK(CodeOf(Run(s, UnequipSlot{kBag, Cid("test:slot.nowhere")})) == InventoryErrorCode::UnknownSlot);
    CHECK(Run(s, TransferItem{kBag, kTrunk, knife, 1}).HasValue());
    CHECK_FALSE(s.CheckInvariants().has_value());
}

TEST_CASE("Equip: swapping needs an explicit unequip and equip in one transaction") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    const ItemInstanceId a = AddOne(s, kBag, f.knife);
    const ItemInstanceId b = AddOne(s, kBag, f.knife);
    REQUIRE(Run(s, EquipItem{kBag, a, f.hand}).HasValue());
    const auto swap = s.ApplyAll({UnequipSlot{kBag, f.hand}, EquipItem{kBag, b, f.hand}});
    REQUIRE(swap.HasValue());
    CHECK(s.FindContainer(kBag)->equipped.at(f.hand) == b);
}

// --- Containers -----------------------------------------------------------------------------------------------------------------

TEST_CASE("Containers: create, duplicate ids, destroy only when empty") {
    Fixture f;
    InventoryState s(f.catalog);
    CHECK(CodeOf(Run(s, CreateContainer{ContainerId(), f.Rules()})) == InventoryErrorCode::InvalidId);
    Create(s, kBag, f.Rules());
    CHECK(CodeOf(Run(s, CreateContainer{kBag, f.Rules()})) == InventoryErrorCode::DuplicateContainer);
    const ItemInstanceId coin = AddOne(s, kBag, f.coin);
    CHECK(CodeOf(Run(s, DestroyContainer{kBag})) == InventoryErrorCode::ContainerNotEmpty);
    CHECK(Run(s, RemoveItem{kBag, coin, 1}).HasValue());
    CHECK(Run(s, DestroyContainer{kBag}).HasValue());
    CHECK(s.FindContainer(kBag) == nullptr);
    CHECK(CodeOf(Run(s, DestroyContainer{kBag})) == InventoryErrorCode::UnknownContainer);
    CHECK(Run(s, CreateContainer{kBag, f.Rules()}).HasValue()); // the id can be used again
}

// --- Transactions ---------------------------------------------------------------------------------------------------------------

TEST_CASE("ApplyAll: success commits everything with consecutive ids and one revision") {
    Fixture f;
    InventoryState s(f.catalog);
    const auto r = s.ApplyAll({CreateContainer{kBag, f.Rules()}, CreateContainer{kTrunk, f.Rules()}, AddItem{kBag, f.knife, 1, {}},
                               AddItem{kBag, f.knife, 1, {}}, AddItem{kTrunk, f.coin, 25, {}}});
    REQUIRE(r.HasValue());
    CHECK(r.Value().revision == 1);
    CHECK(s.Revision() == 1);
    CHECK(r.Value().createdStacks == std::vector<ItemInstanceId>{ItemInstanceId(1), ItemInstanceId(2), ItemInstanceId(3), ItemInstanceId(4), ItemInstanceId(5)});
    CHECK(s.NextInstanceId() == ItemInstanceId(6));
}

TEST_CASE("ApplyAll: any failing command rolls the whole transaction back") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    Create(s, kTrunk, f.Rules(std::nullopt, 400));
    const ItemInstanceId coins = AddOne(s, kBag, f.coin, 5);
    const ItemInstanceId knife = AddOne(s, kBag, f.knife);
    const InventoryState before = s;
    const std::string serializedBefore = s.Serialize();

    const auto r = s.ApplyAll({
        AddItem{kBag, f.coin, 20, {}},                       // 0: ok (creates stacks, allocates ids)
        TransferItem{kBag, kTrunk, knife, 1},                // 1: ok (300 g in a 400 g trunk)
        RemoveItem{kBag, coins, 5},                          // 2: ok
        CreateContainer{ContainerId(3), f.Rules()},          // 3: ok
        AddItem{kTrunk, f.knife, 1, {}},                     // 4: fails: 600 g > 400 g
        AddItem{kBag, f.coin, 1, {}},                        // 5: never reached
    });
    REQUIRE_FALSE(r.HasValue());
    CHECK(r.Error().code == InventoryErrorCode::WeightLimitReached);
    CHECK(r.Error().commandIndex == 4);
    CHECK(s == before);                       // containers, ids counter and revision identical
    CHECK(s.Serialize() == serializedBefore);
    CHECK(s.FindContainer(ContainerId(3)) == nullptr);
    CHECK(s.FindStack(kBag, coins) != nullptr);
    CHECK(s.NextInstanceId() == before.NextInstanceId()); // ids burned inside the failed transaction are not consumed
    CHECK(s.Revision() == before.Revision());
    // The very same state accepts a corrected transaction afterwards.
    CHECK(s.ApplyAll({AddItem{kBag, f.coin, 20, {}}, TransferItem{kBag, kTrunk, knife, 1}}).HasValue());
}

TEST_CASE("ApplyAll: commands see the effects of earlier commands in the same transaction") {
    Fixture f;
    InventoryState s(f.catalog);
    // Create, fill, and destroy-attempt in one go: the destroy must see the staged contents and fail, undoing everything.
    const auto r = s.ApplyAll({CreateContainer{kBag, f.Rules()}, AddItem{kBag, f.coin, 1, {}}, DestroyContainer{kBag}});
    REQUIRE_FALSE(r.HasValue());
    CHECK(r.Error().code == InventoryErrorCode::ContainerNotEmpty);
    CHECK(r.Error().commandIndex == 2);
    CHECK(s.Containers().empty());
    // A container created and destroyed within one transaction leaves no trace.
    const auto ok = s.ApplyAll({CreateContainer{kBag, f.Rules()}, DestroyContainer{kBag}});
    REQUIRE(ok.HasValue());
    CHECK(s.Containers().empty());
    CHECK(s.Revision() == 1);
}

TEST_CASE("ApplyAll: empty list is a no-op - failed single commands do not advance the revision") {
    Fixture f;
    InventoryState s(f.catalog);
    const auto empty = s.ApplyAll({});
    REQUIRE(empty.HasValue());
    CHECK(s.Revision() == 0);
    CHECK(Run(s, DestroyContainer{kBag}).HasValue() == false);
    CHECK(s.Revision() == 0);
}

TEST_CASE("Determinism: identical command sequences give identical states and text") {
    Fixture f;
    const auto script = [&](InventoryState& s) {
        Create(s, kBag, f.Rules());
        Create(s, kTrunk, f.Rules(std::nullopt, 5000));
        AddOne(s, kBag, f.coin, 33);
        const ItemInstanceId k = AddOne(s, kBag, f.knife);
        AddOne(s, kBag, f.bandage, 12);
        REQUIRE(Run(s, EquipItem{kBag, k, f.hand}).HasValue());
        REQUIRE(Run(s, TransferItem{kBag, kTrunk, ItemInstanceId(1), 7}).HasValue());
        REQUIRE(Run(s, SplitStack{kBag, ItemInstanceId(1), 1}).HasValue());
    };
    InventoryState a(f.catalog);
    InventoryState b(f.catalog);
    script(a);
    script(b);
    CHECK(a == b);
    CHECK(a.Serialize() == b.Serialize());
}

TEST_CASE("Property: random command sequences never break invariants or lose items") {
    Fixture f;
    InventoryState s(f.catalog);
    const std::vector<ContainerId> containers{ContainerId(1), ContainerId(2), ContainerId(3)};
    Create(s, containers[0], f.Rules());
    Create(s, containers[1], f.Rules(6, 2000, 400));
    Create(s, containers[2], f.Rules(std::nullopt, 800));
    Lcg rng(2026);
    const std::vector<ContentId> defs{f.coin, f.bandage, f.knife, f.coat};
    std::map<ContentId, std::uint64_t> expected; // total per definition across all containers
    int accepted = 0;
    int rejected = 0;
    for (int step = 0; step < 4000; ++step) {
        const ContainerId c1 = containers[rng.Below(3)];
        const ContainerId c2 = containers[rng.Below(3)];
        // Pick an existing stack of c1 (if any) for stack-targeting commands.
        ItemInstanceId pick;
        if (const Container* c = s.FindContainer(c1); c != nullptr && !c->stacks.empty()) {
            auto it = c->stacks.begin();
            std::advance(it, rng.Below(static_cast<std::uint32_t>(c->stacks.size())));
            pick = it->first;
        }
        const ContentId& def = defs[rng.Below(4)];
        const std::uint32_t qty = 1 + rng.Below(12);
        InventoryCommand cmd;
        switch (rng.Below(8)) {
        case 0: cmd = AddItem{c1, def, f.catalog.Find(def)->Stackable() ? qty : 1, {}}; break;
        case 1: cmd = RemoveItem{c1, pick, 1 + rng.Below(3)}; break;
        case 2: cmd = TransferItem{c1, c2, pick, 1 + rng.Below(5)}; break;
        case 3: cmd = SplitStack{c1, pick, 1 + rng.Below(3)}; break;
        case 4: cmd = MergeStacks{c1, pick, ItemInstanceId(1 + rng.Below(40)), rng.Below(4)}; break;
        case 5: cmd = EquipItem{c1, pick, rng.Below(2) == 0 ? f.hand : f.torso}; break;
        case 6: cmd = UnequipSlot{c1, rng.Below(2) == 0 ? f.hand : f.torso}; break;
        default: cmd = AddItem{c1, defs[rng.Below(2)], qty, {}}; break;
        }
        // Total before, for the conservation check.
        std::map<ContentId, std::uint64_t> before;
        for (const auto& [id, c] : s.Containers()) {
            for (const auto& [sid, stack] : c.stacks) {
                before[stack.definition] += stack.quantity;
            }
        }
        const InventoryState snapshot = s;
        const auto result = s.Apply(cmd);
        std::map<ContentId, std::uint64_t> after;
        for (const auto& [id, c] : s.Containers()) {
            for (const auto& [sid, stack] : c.stacks) {
                after[stack.definition] += stack.quantity;
            }
        }
        if (result.HasValue()) {
            ++accepted;
            if (std::holds_alternative<TransferItem>(cmd) || std::holds_alternative<SplitStack>(cmd) || std::holds_alternative<MergeStacks>(cmd) ||
                std::holds_alternative<EquipItem>(cmd) || std::holds_alternative<UnequipSlot>(cmd)) {
                CHECK(after == before); // these never create or destroy items
            }
        } else {
            ++rejected;
            CHECK(s == snapshot); // a rejected command leaves the state exactly as it was
        }
        const auto violation = s.CheckInvariants();
        if (violation) {
            FAIL("invariant broken at step " << step << ": " << ToString(violation->code) << " " << violation->detail);
        }
        // Serialization is a pure function of the state.
        if (step % 500 == 0) {
            auto loaded = InventoryState::Deserialize(s.Serialize(), f.catalog);
            REQUIRE(loaded.HasValue());
            CHECK(loaded.Value() == s);
        }
    }
    CHECK(accepted > 300);
    CHECK(rejected > 300); // the generator really exercises both paths
}

// --- Persistence ----------------------------------------------------------------------------------------------------------------

namespace {
InventoryState Populated(const Fixture& f) {
    InventoryState s(f.catalog);
    ContainerRules body = f.Rules(20, 40000, 30000);
    body.allowedCategories = {Cid("test:cat.clothing"), Cid("test:cat.currency"), Cid("test:cat.medical"), Cid("test:cat.tool")};
    REQUIRE(Run(s, CreateContainer{kBag, body}).HasValue());
    REQUIRE(Run(s, CreateContainer{kTrunk, f.Rules()}).HasValue());
    AddOne(s, kBag, f.coin, 23);
    AddOne(s, kBag, f.bandage, 3);
    const ItemInstanceId knife =
        Run(s, AddItem{kBag, f.knife, 1, FieldMap{{"note", std::string("a note with spaces\nand a newline % sign")}, {"sharpness", std::int64_t{-4}}, {"worn", false}}})
            .Value()
            .createdStacks.front();
    REQUIRE(Run(s, EquipItem{kBag, knife, f.hand}).HasValue());
    AddOne(s, kTrunk, f.coat);
    return s;
}
} // namespace

TEST_CASE("Inventory persistence: round trip is exact and deterministic") {
    Fixture f;
    const InventoryState original = Populated(f);
    const std::string text = original.Serialize();
    CHECK(text.starts_with("SPN-INVENTORY 1\n"));
    CHECK(text == original.Serialize());
    auto loaded = InventoryState::Deserialize(text, f.catalog);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == original);
    CHECK(loaded.Value().Serialize() == text);
    // The restored state keeps working: ids continue after the saved counter and equipment is intact.
    const ItemInstanceId fresh = AddOne(loaded.Value(), kTrunk, f.knife);
    CHECK(fresh == original.NextInstanceId());
    CHECK(loaded.Value().FindContainer(kBag)->equipped.size() == 1);
}

TEST_CASE("Inventory persistence: empty state and a save of an unlimited container") {
    Fixture f;
    InventoryState empty(f.catalog);
    CHECK(InventoryState::Deserialize(empty.Serialize(), f.catalog).Value() == empty);
    InventoryState s(f.catalog);
    ContainerRules unlimited;
    REQUIRE(Run(s, CreateContainer{kBag, unlimited}).HasValue()); // no kind, no limits, no slots
    CHECK(InventoryState::Deserialize(s.Serialize(), f.catalog).Value() == s);
}

TEST_CASE("Inventory persistence: corrupt, inconsistent and newer data is rejected as a whole") {
    Fixture f;
    const std::string good = Populated(f).Serialize();
    const auto code = [&](const std::string& text) {
        const auto r = InventoryState::Deserialize(text, f.catalog);
        REQUIRE_FALSE(r.HasValue());
        return r.Error().code;
    };
    const auto replace = [&](std::string text, const std::string& from, const std::string& to) {
        const auto at = text.find(from);
        REQUIRE_MESSAGE(at != std::string::npos, "missing: " << from);
        text.replace(at, from.size(), to);
        return text;
    };
    CHECK(code("") == InventoryErrorCode::InvalidHeader);
    CHECK(code("SPN-INVENTORY 2\n") == InventoryErrorCode::UnsupportedVersion);
    CHECK(code("SPN-INVENTORYX 1\n") == InventoryErrorCode::InvalidHeader);
    CHECK(code(good.substr(0, good.size() - 4)) == InventoryErrorCode::TruncatedData);
    CHECK(code(good.substr(0, good.size() - 1)) == InventoryErrorCode::TruncatedData);
    CHECK(code(good + "extra\n") == InventoryErrorCode::TrailingData);
    CHECK(code(replace(good, "containers 2", "containers 3")) != InventoryErrorCode::Inconsistent); // some parse-level error
    CHECK(code(replace(good, "test:item.coin", "test:item.ghost")) == InventoryErrorCode::Inconsistent);   // not in the catalog
    CHECK(code(replace(good, "stack 1 test:item.coin 10", "stack 1 test:item.coin 11")) == InventoryErrorCode::Inconsistent); // over maxStack
    CHECK(code(replace(good, "stack 1 test:item.coin 10", "stack 1 test:item.coin 0")) == InventoryErrorCode::Inconsistent);
    CHECK(code(replace(good, "equip test:slot.hand 5", "equip test:slot.hand 1")) == InventoryErrorCode::Inconsistent);  // coins are not equippable
    CHECK(code(replace(good, "equip test:slot.hand 5", "equip test:slot.hand 77")) == InventoryErrorCode::Inconsistent); // stack does not exist
    CHECK(code(replace(good, "next 7", "next 3")) == InventoryErrorCode::Inconsistent);   // a stack id >= next would collide later
    CHECK(code(replace(good, "next 7", "next 0")) == InventoryErrorCode::InvalidToken);
    CHECK(code(replace(good, "field worn b 0", "field worn b 2")) == InventoryErrorCode::InvalidToken);
    CHECK(code(replace(good, "field sharpness i -4", "field sharpness i -04")) == InventoryErrorCode::InvalidToken); // non-canonical integer
    CHECK(code(replace(good, "field sharpness i -4", "field Sharpness i -4")) == InventoryErrorCode::MalformedLine);
    CHECK(code(replace(good, " 40000 ", " 100 ")) == InventoryErrorCode::WeightLimitReached); // saved contents exceed the (edited) weight limit
}

TEST_CASE("Inventory persistence: a stack in two containers is inconsistent") {
    Fixture f;
    InventoryState s(f.catalog);
    Create(s, kBag, f.Rules());
    Create(s, kTrunk, f.Rules());
    AddOne(s, kBag, f.knife);
    std::string text = s.Serialize();
    // Duplicate the stack line into the second container and fix its counts.
    const std::size_t at = text.find("stack 1 ");
    REQUIRE(at != std::string::npos);
    const std::string stackLine = text.substr(at, text.find('\n', at) - at + 1);
    const std::size_t trunk = text.find("container 2 ");
    const std::size_t trunkEnd = text.find('\n', trunk);
    text.insert(trunkEnd + 1, stackLine);
    text.replace(text.find(" 0 0\n", trunk), 5, " 1 0\n");
    const auto r = InventoryState::Deserialize(text, f.catalog);
    REQUIRE_FALSE(r.HasValue());
    CHECK(r.Error().code == InventoryErrorCode::Inconsistent);
}

TEST_CASE("InventoryErrorCode names are complete") {
    CHECK(ToString(InventoryErrorCode::InvalidDefinition) == "InvalidDefinition");
    CHECK(ToString(InventoryErrorCode::Inconsistent) == "Inconsistent");
    CHECK(ToString(InventoryErrorCode::WeightLimitReached) == "WeightLimitReached");
}

TEST_CASE("Inventory persistence: frozen version-1 text still loads and re-serializes byte for byte") {
    // Hand-written golden text; changing it means the on-disk format changed (a save-compatibility event, not a test update).
    Fixture f;
    const std::string golden =
        "SPN-INVENTORY 1\n"
        "next 4\n"
        "revision 5\n"
        "containers 1\n"
        "container 1 test:container.generic 4 5000 - test:cat.currency,test:cat.tool test:slot.hand 2 1\n"
        "stack 1 test:item.coin 7 0\n"
        "stack 3 test:item.knife 1 1\n"
        "field sharpness i 3\n"
        "equip test:slot.hand 3\n"
        "end\n";
    auto loaded = InventoryState::Deserialize(golden, f.catalog);
    REQUIRE(loaded.HasValue());
    const InventoryState& s = loaded.Value();
    CHECK(s.Revision() == 5);
    CHECK(s.NextInstanceId() == ItemInstanceId(4));
    REQUIRE(s.FindContainer(kBag) != nullptr);
    CHECK(s.FindContainer(kBag)->rules.maxStacks == std::optional<std::uint32_t>(4));
    CHECK(s.FindContainer(kBag)->rules.maxWeightGrams == std::optional<std::uint64_t>(5000));
    CHECK_FALSE(s.FindContainer(kBag)->rules.maxVolumeMilliliters.has_value());
    CHECK(s.CountOf(kBag, f.coin) == 7);
    CHECK(s.FindStack(kBag, ItemInstanceId(3))->state.at("sharpness") == FactValue(std::int64_t{3}));
    CHECK(s.FindContainer(kBag)->equipped.at(f.hand) == ItemInstanceId(3));
    CHECK(s.Serialize() == golden);
}
