#include <doctest.h>

#include <algorithm>
#include <spn/core/CatalogFingerprint.h>
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

TEST_CASE("game save: an intact save of another game world is told apart from damaged data (DefinitionMismatch)") {
    Game g;
    REQUIRE(g.map.Discover(Cid("test:poi.shop"), DiscoveryState::Known).HasValue());
    const auto slot = SaveSlot::Parse(CaptureGameSave(g.Sources()).Value().Serialize());
    REQUIRE(slot.HasValue());
    // the same layers, but none of the POIs: the map of another level
    GameLoadContext other = g.Context();
    other.mapDefinition = MakeBareDefinition();
    const auto foreign = ParseGameSave(slot.Value(), other);
    REQUIRE_FALSE(foreign.HasValue());
    CHECK(foreign.Error().code == SaveErrorCode::DefinitionMismatch);
    CHECK(foreign.Error().detail.starts_with(std::string(kSectionMap)));
    CHECK(ToString(SaveErrorCode::DefinitionMismatch) == "DefinitionMismatch");
    // damaged map data stays SectionRejected (covered for every domain above); against its own definition the slot loads
    CHECK(ParseGameSave(slot.Value(), g.Context()).HasValue());
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

// === catalog fingerprint in the slot (M2B3.4) ============================================================================================

namespace {

std::string CatalogPayload(const std::string& fingerprint, std::uint64_t revision) {
    return "catalog_fingerprint " + fingerprint + "\ninventory_revision " + std::to_string(revision) + "\n";
}

SaveSlot WithSection(SaveSlot slot, std::string_view id, std::uint32_t version, std::string payload) {
    REQUIRE_FALSE(slot.Put(SaveSection{std::string(id), version, false, std::move(payload)}).has_value());
    return slot;
}

} // namespace

TEST_CASE("catalog check: capture writes slot.catalog next to the inventory, load verifies it") {
    Game g;
    const SaveSlot slot = CaptureGameSave(g.Sources()).Value();
    const SaveSection* section = slot.Find(kSectionCatalog);
    REQUIRE(section != nullptr);
    CHECK(section->version == 1);
    CHECK_FALSE(section->required);
    CHECK(section->payload == CatalogPayload(CatalogFingerprint(g.items), g.inventory.Revision()));
    CHECK_FALSE(slot.Validate(GameSaveSchema()).has_value());

    const auto game = ParseGameSave(slot, g.Context());
    REQUIRE(game.HasValue());
    CHECK(game.Value().catalogCheck == CatalogCheck::Match);
    CHECK(game.Value().storedFingerprint == CatalogFingerprint(g.items));
    CHECK(game.Value().currentFingerprint == game.Value().storedFingerprint);

    // a slot without an inventory has no catalog section and nothing to check
    GameSaveSources factsOnly;
    factsOnly.facts = &g.facts;
    const SaveSlot small = CaptureGameSave(factsOnly).Value();
    CHECK(small.Find(kSectionCatalog) == nullptr);
    CHECK(ParseGameSave(small, g.Context()).Value().catalogCheck == CatalogCheck::NotApplicable);
}

TEST_CASE("catalog check: a legacy slot (inventory without slot.catalog) loads, explicitly marked as unverified") {
    Game g;
    SaveSlot slot = CaptureGameSave(g.Sources()).Value();
    REQUIRE(slot.Remove(kSectionCatalog));
    const auto game = ParseGameSave(slot, g.Context());
    REQUIRE(game.HasValue());
    CHECK(game.Value().catalogCheck == CatalogCheck::LegacyNoFingerprint);
    CHECK(game.Value().storedFingerprint.empty());
    CHECK(game.Value().currentFingerprint == CatalogFingerprint(g.items));
    REQUIRE(game.Value().inventory.has_value());
    CHECK(game.Value().inventory->Serialize() == g.inventory.Serialize()); // fully restored
}

TEST_CASE("catalog check: a fingerprint of another inventory revision is stale (an older build re-saved the inventory)") {
    Game g;
    const SaveSlot slot = CaptureGameSave(g.Sources()).Value();
    const SaveSlot stale = WithSection(slot, kSectionCatalog, 1, CatalogPayload("fp1:" + std::string(64, '0'), g.inventory.Revision() + 5));
    const auto game = ParseGameSave(stale, g.Context());
    REQUIRE(game.HasValue());
    CHECK(game.Value().catalogCheck == CatalogCheck::StaleFingerprint);
}

TEST_CASE("catalog check: changed definitions are refused by default and never reset the inventory - accepting is explicit and still validated") {
    Game g;
    const SaveSlot slot = CaptureGameSave(g.Sources()).Value();

    // same id, other weight: the inventory still parses, the fingerprint says the meaning changed
    ItemCatalog heavier;
    ItemDefinition coin;
    coin.id = Cid("test:item.coin");
    coin.category = Cid("test:cat.currency");
    coin.maxStack = 10;
    coin.weightGrams = 50;
    REQUIRE_FALSE(heavier.Add(coin).has_value());
    GameLoadContext context = g.Context();
    context.itemCatalog = &heavier;
    const auto refused = ParseGameSave(slot, context);
    REQUIRE_FALSE(refused.HasValue());
    CHECK(refused.Error().code == SaveErrorCode::CatalogMismatch);
    CHECK(refused.Error().detail.find(CatalogFingerprint(g.items)) != std::string::npos);
    CHECK(refused.Error().detail.find(CatalogFingerprint(heavier)) != std::string::npos);

    context.acceptCatalogMismatch = true;
    const auto accepted = ParseGameSave(slot, context);
    REQUIRE(accepted.HasValue());
    CHECK(accepted.Value().catalogCheck == CatalogCheck::MismatchAccepted);
    CHECK(accepted.Value().storedFingerprint != accepted.Value().currentFingerprint);
    REQUIRE(accepted.Value().inventory.has_value());
    CHECK(accepted.Value().inventory->CountOf(ContainerId{1}, Cid("test:item.coin")) == 23); // content kept, not reset
    CHECK(accepted.Value().inventory->TotalWeightGrams(ContainerId{1}) == 23 * 50);         // weights follow the NEW definition

    // accepting does not skip validation: a catalog under which the saved inventory is invalid is still rejected
    ItemCatalog smaller;
    coin.maxStack = 2; // the saved stacks of 10 no longer fit
    REQUIRE_FALSE(smaller.Add(coin).has_value());
    context.itemCatalog = &smaller;
    const auto invalid = ParseGameSave(slot, context);
    REQUIRE_FALSE(invalid.HasValue());
    CHECK(invalid.Error().code == SaveErrorCode::SectionRejected);

    // a catalog that lost the item entirely is rejected by the parser as well
    ItemCatalog without;
    ItemDefinition other = coin;
    other.id = Cid("test:item.other");
    REQUIRE_FALSE(without.Add(other).has_value());
    context.itemCatalog = &without;
    CHECK(ParseGameSave(slot, context).Error().code == SaveErrorCode::SectionRejected);
}

TEST_CASE("catalog check: damaged or inconsistent slot.catalog sections are rejected, never read as no fingerprint") {
    Game g;
    const SaveSlot slot = CaptureGameSave(g.Sources()).Value();
    const std::string good = CatalogFingerprint(g.items);
    const std::uint64_t revision = g.inventory.Revision();
    const std::vector<std::string> malformed = {
        "",
        "garbage\n",
        "catalog_fingerprint " + good + "\n",                                                                  // revision line missing
        "catalog_fingerprint " + good + "\ninventory_revision " + std::to_string(revision),                    // no final newline
        "inventory_revision " + std::to_string(revision) + "\ncatalog_fingerprint " + good + "\n",             // wrong order
        "catalog_fingerprint fp1:XYZ\ninventory_revision " + std::to_string(revision) + "\n",
        "catalog_fingerprint " + good + "\ninventory_revision 01\n",                                            // not canonical
        "catalog_fingerprint " + good + "\ninventory_revision " + std::to_string(revision) + "\nextra 1\n",
    };
    for (const std::string& payload : malformed) {
        INFO("payload: " << payload);
        const auto game = ParseGameSave(WithSection(slot, kSectionCatalog, 1, payload), g.Context());
        REQUIRE_FALSE(game.HasValue());
        CHECK(game.Error().code == SaveErrorCode::SectionRejected);
    }
    // a catalog section without an inventory is inconsistent
    SaveSlot noInventory = slot;
    REQUIRE(noInventory.Remove(kSectionInventory));
    REQUIRE(noInventory.Remove(kSectionMeta));
    CHECK(ParseGameSave(noInventory, g.Context()).Error().code == SaveErrorCode::ConsistencyMismatch);
    // newer versions are never ignored
    CHECK(ParseGameSave(WithSection(slot, kSectionCatalog, 2, CatalogPayload(good, revision)), g.Context()).Error().code == SaveErrorCode::UnsupportedSectionVersion);
    const std::string future = "fp2:" + good.substr(4);
    CHECK(ParseGameSave(WithSection(slot, kSectionCatalog, 1, CatalogPayload(future, revision)), g.Context()).Error().code == SaveErrorCode::UnsupportedSectionVersion);
}

TEST_CASE("catalog check: backward compatibility - a build that does not know slot.catalog ignores it and keeps it") {
    Game g;
    const SaveSlot slot = CaptureGameSave(g.Sources()).Value();
    SaveSchema older = GameSaveSchema(); // the schema of M2B3.3 builds had no slot.catalog
    std::erase_if(older.sections, [](const SectionSpec& spec) { return spec.id == kSectionCatalog; });
    CHECK_FALSE(slot.Validate(older).has_value());
    const auto unread = slot.UnreadSections(older);
    CHECK(std::find(unread.begin(), unread.end(), std::string(kSectionCatalog)) != unread.end());
    // and the container round trip keeps the bytes
    const auto reparsed = SaveSlot::Parse(slot.Serialize());
    REQUIRE(reparsed.HasValue());
    CHECK(reparsed.Value().Find(kSectionCatalog)->payload == slot.Find(kSectionCatalog)->payload);
}
