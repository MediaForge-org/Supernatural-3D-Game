#include <doctest.h>

#include <algorithm>
#include <spn/core/Devices.h>
#include <spn/core/GameSave.h>
#include <spn/core/SaveStore.h>
#include <spn/core/TestDoor.h>

#include "MapTestData.h"
#include "SaveTestFs.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

// One complete synthetic game state: facts + objects (with an unloaded region) + map + inventory. All values are test data.
struct Game {
    WorldObjectCatalog catalog;
    WorldFacts facts;
    RegionStateCache regions;
    std::shared_ptr<const WorldMapDefinition> mapDefinition = MakeTestDefinition();
    WorldMapState map{mapDefinition};
    ItemCatalog items;
    InventoryState inventory{items};

    static ObjectConfig Door(const char* id, const char* region) {
        ObjectConfig c;
        c.id = id;
        c.kind = TestDoorKind();
        c.region = region;
        c.defaults = {{"open", false}, {"locked", false}, {"disabled", false}};
        return c;
    }

    Game() {
        REQUIRE_FALSE(catalog.Add(Door("door_a", "cell_0_0")).has_value());
        REQUIRE_FALSE(catalog.Add(Door("door_b", "cell_0_1")).has_value());
        ItemDefinition coin;
        coin.id = Cid("test:item.coin");
        coin.category = Cid("test:cat.currency");
        coin.maxStack = 10;
        REQUIRE_FALSE(items.Add(coin).has_value());

        REQUIRE(facts.Apply(SetFact{TestDoorOpenKey("door_a"), true}, "test").HasValue());
        REQUIRE(facts.Apply(SetFact{TestDoorLockedKey("door_b"), true}, "test").HasValue());
        (void)regions.Unload("cell_0_1", facts, catalog); // door_b's state now lives only in the cache
        REQUIRE(facts.Apply(SetFact{"quest.stage", std::int64_t{4}}, "test").HasValue());

        REQUIRE(map.AddPlayerWaypoint(At(kSurface, 100.0, 200.0), "my waypoint").HasValue());
        REQUIRE(inventory.Apply(CreateContainer{ContainerId{1}, ContainerRules{}}).HasValue());
        REQUIRE(inventory.Apply(AddItem{ContainerId{1}, Cid("test:item.coin"), 23, {}}).HasValue());
    }

    GameSaveSources Sources() const { return {&facts, &regions, &catalog, &map, &inventory}; }
    GameLoadContext Context() const { return {mapDefinition, {}, &items}; }
};

} // namespace

TEST_CASE("game save: every domain's section holds exactly the bytes of its own Serialize()") {
    Game g;
    const auto slot = CaptureGameSave(g.Sources());
    REQUIRE(slot.HasValue());
    CHECK(slot.Value().Find(kSectionFacts)->payload == g.facts.Serialize());
    CHECK(slot.Value().Find(kSectionMap)->payload == g.map.Serialize());
    CHECK(slot.Value().Find(kSectionInventory)->payload == g.inventory.Serialize());
    CHECK(slot.Value().Find(kSectionObjects)->payload == SerializeRecords(g.regions.Snapshot(g.facts, g.catalog)));
    CHECK(slot.Value().Find(kSectionFacts)->required);
    CHECK_FALSE(slot.Value().Find(kSectionMap)->required);
    // the embedded formats still carry their own frozen headers
    CHECK(slot.Value().Find(kSectionMap)->payload.starts_with("SPN-MAPSTATE 1\n"));
    CHECK(slot.Value().Find(kSectionInventory)->payload.starts_with("SPN-INVENTORY 1\n"));
    CHECK_FALSE(GameSaveSchema().sections.empty());
    CHECK_FALSE(slot.Value().Validate(GameSaveSchema()).has_value());
}

TEST_CASE("game save: capture is read-only and deterministic") {
    Game g;
    const std::string facts = g.facts.Serialize(), map = g.map.Serialize(), inv = g.inventory.Serialize();
    const auto a = CaptureGameSave(g.Sources());
    const auto b = CaptureGameSave(g.Sources());
    REQUIRE(a.HasValue());
    REQUIRE(b.HasValue());
    CHECK(a.Value().Serialize() == b.Value().Serialize());
    CHECK(g.facts.Serialize() == facts);
    CHECK(g.map.Serialize() == map);
    CHECK(g.inventory.Serialize() == inv);
    CHECK(g.regions.RetainedCount() == 1);
}

TEST_CASE("game save: capture, store, load and parse reproduce the whole state through the real publish protocol") {
    Game g;
    MemoryFs fs;
    SaveSlotStore store(fs, "slot.sav");
    REQUIRE_FALSE(store.Save(CaptureGameSave(g.Sources()).Value(), GameSaveSchema()).has_value());
    const auto loadedSlot = store.Load(GameSaveSchema());
    REQUIRE(loadedSlot.HasValue());
    const auto game = ParseGameSave(loadedSlot.Value().slot, g.Context());
    REQUIRE(game.HasValue());
    CHECK(game.Value().facts.Serialize() == g.facts.Serialize());
    REQUIRE(game.Value().map.has_value());
    CHECK(game.Value().map->SameSavedState(g.map));
    REQUIRE(game.Value().inventory.has_value());
    CHECK(*game.Value().inventory == g.inventory);
    REQUIRE(game.Value().objects.has_value());
    CHECK(*game.Value().objects == g.regions.Snapshot(g.facts, g.catalog));
    CHECK(game.Value().unreadSections.empty());
    // the unloaded region's retained state is in the objects section even though the live facts do not hold it
    const auto it = std::find_if(game.Value().objects->begin(), game.Value().objects->end(), [](const ObjectRecord& r) { return r.id == "door_b"; });
    REQUIRE(it != game.Value().objects->end());
    CHECK(it->fields.at("locked") == FactValue(true));
    CHECK(g.facts.Find(TestDoorLockedKey("door_b")) == nullptr);
}

TEST_CASE("game save: optional domains may be absent - the facts section is required") {
    Game g;
    GameSaveSources onlyFacts;
    onlyFacts.facts = &g.facts;
    const auto slot = CaptureGameSave(onlyFacts);
    REQUIRE(slot.HasValue());
    CHECK(slot.Value().Count() == 2); // facts + meta
    const auto game = ParseGameSave(slot.Value(), GameLoadContext{});
    REQUIRE(game.HasValue());
    CHECK_FALSE(game.Value().map.has_value());
    CHECK_FALSE(game.Value().inventory.has_value());
    CHECK_FALSE(game.Value().objects.has_value());

    SaveSlot noFacts = slot.Value();
    noFacts.Remove(kSectionFacts);
    CHECK(ParseGameSave(noFacts, GameLoadContext{}).Error().code == SaveErrorCode::MissingRequiredSection);
    CHECK(CaptureGameSave(GameSaveSources{}).Error().code == SaveErrorCode::MissingContext);
    GameSaveSources half = onlyFacts;
    half.regions = &g.regions; // regions without the catalog
    CHECK(CaptureGameSave(half).Error().code == SaveErrorCode::MissingContext);
}

TEST_CASE("game save: a present section without its definitions is refused, not guessed") {
    Game g;
    const auto slot = CaptureGameSave(g.Sources()).Value();
    GameLoadContext noMap = g.Context();
    noMap.mapDefinition = nullptr;
    CHECK(ParseGameSave(slot, noMap).Error().code == SaveErrorCode::MissingContext);
    GameLoadContext noItems = g.Context();
    noItems.itemCatalog = nullptr;
    CHECK(ParseGameSave(slot, noItems).Error().code == SaveErrorCode::MissingContext);
}

TEST_CASE("game save: a corrupt domain payload with a valid checksum is rejected by that domain's parser, and nothing is returned") {
    Game g;
    for (const auto& id : {kSectionFacts, kSectionObjects, kSectionMap, kSectionInventory}) {
        SaveSlot slot = CaptureGameSave(g.Sources()).Value();
        SaveSection s = *slot.Find(id);
        s.payload = "garbage that the container happily checksums\n";
        REQUIRE_FALSE(slot.Put(s).has_value());
        const auto parsed = SaveSlot::Parse(slot.Serialize()); // survives the container...
        REQUIRE(parsed.HasValue());
        const auto game = ParseGameSave(parsed.Value(), g.Context());
        REQUIRE_FALSE(game.HasValue()); // ...but not the domain parser
        CHECK(game.Error().code == SaveErrorCode::SectionRejected);
        CHECK(game.Error().detail.starts_with(std::string(id)));
    }
}

TEST_CASE("game save: sections from different gameplay moments are detected through slot.meta") {
    Game g;
    const auto first = CaptureGameSave(g.Sources()).Value();
    REQUIRE(g.inventory.Apply(AddItem{ContainerId{1}, Cid("test:item.coin"), 1, {}}).HasValue()); // time passes
    REQUIRE(g.facts.Apply(SetFact{"quest.stage", std::int64_t{5}}, "test").HasValue());
    const auto second = CaptureGameSave(g.Sources()).Value();

    SaveSlot mixedInventory = first;
    REQUIRE_FALSE(mixedInventory.Put(*second.Find(kSectionInventory)).has_value());
    auto r = ParseGameSave(mixedInventory, g.Context());
    REQUIRE_FALSE(r.HasValue());
    CHECK(r.Error().code == SaveErrorCode::ConsistencyMismatch);

    SaveSlot mixedFacts = first;
    REQUIRE_FALSE(mixedFacts.Put(*second.Find(kSectionFacts)).has_value());
    CHECK(ParseGameSave(mixedFacts, g.Context()).Error().code == SaveErrorCode::ConsistencyMismatch);

    SaveSlot noInventoryRevision = first;
    const std::string meta = first.Find(kSectionMeta)->payload;
    REQUIRE_FALSE(noInventoryRevision.Put(SaveSection{std::string(kSectionMeta), 1, false, meta.substr(0, meta.find('\n') + 1)}).has_value());
    CHECK(ParseGameSave(noInventoryRevision, g.Context()).Error().code == SaveErrorCode::ConsistencyMismatch);

    SaveSlot malformedMeta = first;
    REQUIRE_FALSE(malformedMeta.Put(SaveSection{std::string(kSectionMeta), 1, false, "facts_revision x\n"}).has_value());
    CHECK(ParseGameSave(malformedMeta, g.Context()).Error().code == SaveErrorCode::SectionRejected);

    CHECK(ParseGameSave(first, g.Context()).HasValue());  // a coherent snapshot is accepted
    CHECK(ParseGameSave(second, g.Context()).HasValue());
}

TEST_CASE("game save: unknown optional sections written by a newer build survive a load/modify/save cycle of this build") {
    Game g;
    SaveSlot older = CaptureGameSave(g.Sources()).Value();
    REQUIRE_FALSE(older.Add(SaveSection{"quest.journal", 4, false, "newer data this build cannot read"}).has_value());
    REQUIRE_FALSE(older.Put(SaveSection{std::string(kSectionMap), 2, false, "SPN-MAPSTATE 2 (from the future)"}).has_value());
    const auto parsed = ParseGameSave(older, g.Context());
    REQUIRE(parsed.HasValue());
    CHECK(parsed.Value().unreadSections == std::vector<std::string>{"quest.journal", "world.map"});
    CHECK_FALSE(parsed.Value().map.has_value()); // not read, not guessed
    // saving with this build carries them over unchanged
    GameSaveSources sources = g.Sources();
    sources.map = nullptr; // this build has no readable map to write
    const auto rewritten = CaptureGameSave(sources, &older);
    REQUIRE(rewritten.HasValue());
    CHECK(rewritten.Value().Find("quest.journal")->payload == "newer data this build cannot read");
    CHECK(rewritten.Value().Find(kSectionMap)->version == 2);
    CHECK(rewritten.Value().Find(kSectionMap)->payload == "SPN-MAPSTATE 2 (from the future)");
    // a section this build writes itself is never overridden by the old one
    const auto full = CaptureGameSave(g.Sources(), &older);
    CHECK(full.Value().Find(kSectionMap)->version == 1);
}

TEST_CASE("game save: existing formats stay readable - SPN-MAPSTATE 1 and SPN-INVENTORY 1 payloads written before the container are accepted as sections") {
    // A payload produced by the plain Serialize() of M2B1 (no container involved) is embedded verbatim and parses with the unchanged parsers.
    Game g;
    SaveSlot slot;
    REQUIRE_FALSE(slot.Add(SaveSection{std::string(kSectionFacts), 1, true, g.facts.Serialize()}).has_value());
    REQUIRE_FALSE(slot.Add(SaveSection{std::string(kSectionMap), 1, false, g.map.Serialize()}).has_value());
    REQUIRE_FALSE(slot.Add(SaveSection{std::string(kSectionInventory), 1, false, g.inventory.Serialize()}).has_value());
    const auto game = ParseGameSave(slot, g.Context()); // no slot.meta: older writers need not provide it
    REQUIRE(game.HasValue());
    CHECK(game.Value().map->SameSavedState(g.map));
    CHECK(*game.Value().inventory == g.inventory);
}
