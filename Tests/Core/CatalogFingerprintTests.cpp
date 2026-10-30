#include <doctest.h>

#include <spn/core/CatalogFingerprint.h>
#include <spn/core/GameSave.h>
#include <spn/core/Inventory.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

ItemDefinition Def(const char* id, const char* category, std::uint32_t maxStack, std::uint64_t weight, std::uint64_t volume, std::set<ContentId> slots = {}) {
    ItemDefinition d;
    d.id = Cid(id);
    d.category = Cid(category);
    d.maxStack = maxStack;
    d.weightGrams = weight;
    d.volumeMilliliters = volume;
    d.equipSlots = std::move(slots);
    return d;
}

// Test data only.
ItemCatalog BaseCatalog() {
    ItemCatalog c;
    REQUIRE_FALSE(c.Add(Def("test:item.coin", "test:cat.currency", 10, 5, 1)).has_value());
    REQUIRE_FALSE(c.Add(Def("test:item.lamp", "test:cat.tool", 1, 400, 250, {Cid("test:slot.hand")})).has_value());
    REQUIRE_FALSE(c.Add(Def("test:item.coat", "test:cat.clothing", 1, 1200, 4000, {Cid("test:slot.torso"), Cid("test:slot.back")})).has_value());
    return c;
}

} // namespace

TEST_CASE("sha256: known-answer vectors (FIPS 180-4 / NIST)") {
    CHECK(Sha256Hex("") == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    CHECK(Sha256Hex("abc") == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    CHECK(Sha256Hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") == "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    CHECK(Sha256Hex(std::string(1000000, 'a')) == "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"); // one million 'a'
    // padding boundaries: 55, 56 and 64 bytes straddle the length-field edge
    CHECK(Sha256Hex(std::string(55, 'x')).size() == 64);
    CHECK(Sha256Hex(std::string(55, 'x')) != Sha256Hex(std::string(56, 'x')));
    CHECK(Sha256Hex(std::string(56, 'x')) != Sha256Hex(std::string(64, 'x')));
}

TEST_CASE("catalog fingerprint: canonical text and frozen value (any change of either is a deliberate format version bump)") {
    const ItemCatalog catalog = BaseCatalog();
    CHECK(CanonicalCatalogText(catalog) ==
          "SPN-CATALOG-FP 1\n"
          "def test:item.coat test:cat.clothing 1 1200 4000 test:slot.back,test:slot.torso\n"
          "def test:item.coin test:cat.currency 10 5 1 -\n"
          "def test:item.lamp test:cat.tool 1 400 250 test:slot.hand\n");
    const std::string fingerprint = CatalogFingerprint(catalog);
    CHECK(fingerprint.starts_with("fp1:"));
    CHECK(fingerprint.size() == 4 + 64);
    CHECK(fingerprint == "fp1:" + Sha256Hex(CanonicalCatalogText(catalog)));
    CHECK(fingerprint == "fp1:7cefae27e1b0f5c12c8b87f204f0118104a24d3ed2fc095020f9231a0e7e38bc"); // frozen (SHA-256 of the canonical text above)
    CHECK(IsWellFormedFingerprint(fingerprint));
    CHECK(IsComparableFingerprint(fingerprint));
}

TEST_CASE("catalog fingerprint: independent of insertion order, sensitive to every gameplay-relevant field") {
    const std::string base = CatalogFingerprint(BaseCatalog());

    ItemCatalog reversed;
    REQUIRE_FALSE(reversed.Add(Def("test:item.coat", "test:cat.clothing", 1, 1200, 4000, {Cid("test:slot.back"), Cid("test:slot.torso")})).has_value());
    REQUIRE_FALSE(reversed.Add(Def("test:item.lamp", "test:cat.tool", 1, 400, 250, {Cid("test:slot.hand")})).has_value());
    REQUIRE_FALSE(reversed.Add(Def("test:item.coin", "test:cat.currency", 10, 5, 1)).has_value());
    CHECK(CatalogFingerprint(reversed) == base); // order of definitions and of slots does not matter

    const auto with = [](auto&& change) {
        ItemCatalog c;
        auto defs = std::vector<ItemDefinition>{Def("test:item.coin", "test:cat.currency", 10, 5, 1), Def("test:item.lamp", "test:cat.tool", 1, 400, 250, {Cid("test:slot.hand")}),
                                                Def("test:item.coat", "test:cat.clothing", 1, 1200, 4000, {Cid("test:slot.torso"), Cid("test:slot.back")})};
        change(defs);
        for (auto& d : defs) REQUIRE_FALSE(c.Add(std::move(d)).has_value());
        return CatalogFingerprint(c);
    };
    std::set<std::string> seen{base};
    const auto distinct = [&](const std::string& fp, const char* what) {
        INFO(what);
        CHECK(fp != base);
        CHECK(seen.insert(fp).second); // and different from every other variation
    };
    distinct(with([](auto& d) { d[0].maxStack = 11; }), "max stack");
    distinct(with([](auto& d) { d[0].weightGrams = 6; }), "weight");
    distinct(with([](auto& d) { d[0].volumeMilliliters = 2; }), "volume");
    distinct(with([](auto& d) { d[0].category = Cid("test:cat.money"); }), "category");
    distinct(with([](auto& d) { d[1].equipSlots = {Cid("test:slot.belt")}; }), "equip slot replaced");
    distinct(with([](auto& d) { d[2].equipSlots.erase(Cid("test:slot.back")); }), "equip slot removed");
    distinct(with([](auto& d) { d[0].id = Cid("test:item.coins"); }), "item id");
    distinct(with([](auto& d) { d.pop_back(); }), "definition removed");
    distinct(with([](auto& d) { d.push_back(Def("test:item.extra", "test:cat.tool", 1, 0, 0)); }), "definition added");
    // a value that shifts one digit into the next field must not collide ("1 20" vs "12 0")
    ItemCatalog a, b;
    REQUIRE_FALSE(a.Add(Def("test:item.x", "test:cat.y", 1, 20, 0)).has_value());
    REQUIRE_FALSE(b.Add(Def("test:item.x", "test:cat.y", 1, 2, 0)).has_value());
    CHECK(CatalogFingerprint(a) != CatalogFingerprint(b));
}

TEST_CASE("catalog fingerprint: syntax helpers") {
    const std::string good = CatalogFingerprint(BaseCatalog());
    CHECK(IsWellFormedFingerprint(good));
    CHECK_FALSE(IsWellFormedFingerprint(""));
    CHECK_FALSE(IsWellFormedFingerprint("fp1:"));
    CHECK_FALSE(IsWellFormedFingerprint(good.substr(0, good.size() - 1)));
    CHECK_FALSE(IsWellFormedFingerprint(good + "0"));
    std::string upper = good;
    upper[10] = 'A' <= upper[10] && upper[10] <= 'F' ? upper[10] : 'G';
    CHECK_FALSE(IsWellFormedFingerprint(upper));
    CHECK_FALSE(IsWellFormedFingerprint("xp1:" + good.substr(4)));
    CHECK_FALSE(IsWellFormedFingerprint("fp:" + good.substr(4)));
    const std::string future = "fp2:" + good.substr(4);
    CHECK(IsWellFormedFingerprint(future));
    CHECK_FALSE(IsComparableFingerprint(future)); // a newer algorithm: well-formed, but this build cannot recompute it
}

// === InventoryState::Probe (dry run used by the Unreal view-model to learn which actions are possible) ====================================

TEST_CASE("inventory probe: reports exactly what ApplyAll would report and never changes anything") {
    const ItemCatalog catalog = BaseCatalog();
    InventoryState live(catalog);
    ContainerRules body;
    body.maxStacks = 2;
    body.equipSlots = {Cid("test:slot.hand")};
    REQUIRE(live.Apply(CreateContainer{ContainerId{1}, body}).HasValue());
    REQUIRE(live.Apply(CreateContainer{ContainerId{2}, ContainerRules{}}).HasValue());
    REQUIRE(live.Apply(AddItem{ContainerId{1}, Cid("test:item.coin"), 7, {}}).HasValue());
    const ItemInstanceId lamp = live.ApplyAll({AddItem{ContainerId{1}, Cid("test:item.lamp"), 1, {}}}).Value().createdStacks[0];

    const std::vector<std::vector<InventoryCommand>> cases = {
        {},                                                                                // empty
        {EquipItem{ContainerId{1}, lamp, Cid("test:slot.hand")}},                          // ok
        {EquipItem{ContainerId{1}, lamp, Cid("test:slot.torso")}},                         // slot not offered
        {TransferItem{ContainerId{1}, ContainerId{2}, lamp, 1}},                           // ok
        {AddItem{ContainerId{1}, Cid("test:item.coat"), 1, {}}},                           // stack limit of container 1 (2 stacks)
        {AddItem{ContainerId{2}, Cid("test:item.coin"), 3, {}}, RemoveItem{ContainerId{2}, ItemInstanceId{999}, 1}}, // second command fails
        {CreateContainer{ContainerId{3}, ContainerRules{}}, CreateContainer{ContainerId{4}, ContainerRules{}}},      // ok, creates two containers
        {SplitStack{ContainerId{1}, ItemInstanceId{1}, 2}, MergeStacks{ContainerId{1}, ItemInstanceId{3}, ItemInstanceId{1}, 0}},
    };
    for (std::size_t i = 0; i < cases.size(); ++i) {
        INFO("case " << i);
        InventoryState copy = live;
        const auto expected = copy.ApplyAll(cases[i]);
        const InventoryState before = live;
        const auto probed = live.Probe(cases[i]);
        CHECK(probed.has_value() == !expected.HasValue());
        if (probed && !expected.HasValue()) {
            CHECK(probed->code == expected.Error().code);
            CHECK(probed->commandIndex == expected.Error().commandIndex);
        }
        CHECK(live == before);                       // containers, stacks, equipment, id counter, revision
        CHECK_FALSE(live.CheckInvariants().has_value());
        CHECK(live.Serialize() == before.Serialize());
    }
    // the next real transaction allocates the ids a probe "used"
    const ItemInstanceId next = live.NextInstanceId();
    CHECK(live.Probe({AddItem{ContainerId{2}, Cid("test:item.lamp"), 1, {}}}) == std::nullopt);
    CHECK(live.NextInstanceId() == next);
    CHECK(live.ApplyAll({AddItem{ContainerId{2}, Cid("test:item.lamp"), 1, {}}}).Value().createdStacks[0] == next);
    CHECK(&live.Catalog() == &catalog);
}
