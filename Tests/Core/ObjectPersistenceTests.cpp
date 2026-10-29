#include <doctest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <random>
#include <spn/core/Devices.h>
#include <spn/core/InteractionSystem.h>
#include <spn/core/ObjectPersistence.h>
#include <spn/core/TestDoor.h>

using namespace spn::core;
namespace fs = std::filesystem;

namespace {

ObjectConfig Door(const char* id, const char* region, bool locked = false) {
    ObjectConfig c;
    c.id = id;
    c.kind = TestDoorKind();
    c.region = region;
    c.defaults = {{"open", false}, {"locked", locked}, {"disabled", false}};
    return c;
}
ObjectConfig Switch(const char* id, const char* region) {
    ObjectConfig c;
    c.id = id;
    c.kind = SwitchKind();
    c.region = region;
    c.defaults = {{"on", false}, {"disabled", false}};
    return c;
}
ObjectConfig Lever(const char* id, const char* region) {
    ObjectConfig c;
    c.id = id;
    c.kind = LeverKind();
    c.region = region;
    c.defaults = {{"position", std::int64_t{0}}, {"disabled", false}};
    return c;
}

struct Fixture {
    WorldObjectCatalog catalog;
    WorldFacts facts;
    Fixture() {
        REQUIRE_FALSE(catalog.Add(Door("door_a", "cell_0_0")).has_value());
        REQUIRE_FALSE(catalog.Add(Door("door_b", "cell_0_0", true)).has_value());
        REQUIRE_FALSE(catalog.Add(Switch("sw_a", "cell_0_1")).has_value());
        REQUIRE_FALSE(catalog.Add(Lever("lv_a", "cell_0_1")).has_value());
    }
    void Set(const std::string& key, FactValue v) { REQUIRE(facts.Apply(SetFact{key, std::move(v)}, "test").HasValue()); }
};

fs::path TempDir(const char* name) {
    const fs::path dir = fs::temp_directory_path() / (std::string("spn_persist_") + name + "_" + std::to_string(std::random_device{}()));
    fs::create_directories(dir);
    return dir;
}

} // namespace

TEST_CASE("catalog validates ids, regions, kinds, defaults and duplicates") {
    WorldObjectCatalog c;
    CHECK_FALSE(c.Add(Door("d1", "r1")).has_value());
    CHECK(c.Add(Door("d1", "r2"))->code == PersistErrorCode::DuplicateId);
    CHECK(c.Add(Door("Bad", "r1"))->code == PersistErrorCode::InvalidId);
    CHECK(c.Add(Door("d2", "Bad Region"))->code == PersistErrorCode::InvalidRegion);
    ObjectConfig noKind = Door("d3", "r1");
    noKind.kind = ContentId{};
    CHECK(c.Add(noKind)->code == PersistErrorCode::UnknownKind);
    ObjectConfig badField = Door("d4", "r1");
    badField.defaults["Bad.Field"] = true;
    CHECK(c.Add(badField)->code == PersistErrorCode::InvalidDefaults);
    CHECK(c.Size() == 1);
    // ownership: nested ids and dotted ids resolve to the right object
    WorldObjectCatalog n;
    REQUIRE_FALSE(n.Add(Door("a", "r")).has_value());
    REQUIRE_FALSE(n.Add(Door("a.b", "r")).has_value());
    std::string field;
    CHECK(n.OwnerOf("placeholder.door.a.b.open", &field)->id == "a.b");
    CHECK(field == "open");
    CHECK(n.OwnerOf("placeholder.door.a.open", &field)->id == "a");
    CHECK(n.OwnerOf("placeholder.door.a.bz", &field)->id == "a");
    CHECK(field == "bz");
    CHECK(n.OwnerOf("placeholder.door.c.open") == nullptr);
    CHECK(n.OwnerOf("placeholder.door.a.") == nullptr);
    CHECK(n.OwnerOf("nodot") == nullptr);
    CHECK(n.OwnerOf("placeholder.door.a.Open") == nullptr);
}

TEST_CASE("only changed state is captured - defaults, unrelated facts and explicit default values are ignored") {
    Fixture f;
    CHECK(CaptureChanged(f.facts, f.catalog).empty());
    f.Set(TestDoorOpenKey("door_a"), true);
    f.Set(TestDoorLockedKey("door_b"), true);       // equals its authored default (locked): not a change
    f.Set("placeholder.unrelated.thing.value", std::int64_t{5});
    f.Set("placeholder.door.not_in_catalog.open", true);
    const auto records = CaptureChanged(f.facts, f.catalog);
    REQUIRE(records.size() == 1);
    CHECK(records[0].id == "door_a");
    CHECK(records[0].region == "cell_0_0");
    CHECK(records[0].fields.at("open") == FactValue(true));
    CHECK(records[0].revision == 1);
    f.Set(TestDoorOpenKey("door_a"), false); // back to the default: nothing to store, history still counts
    CHECK(CaptureChanged(f.facts, f.catalog).empty());
    f.Set(TestDoorOpenKey("door_a"), true);
    CHECK(CaptureChanged(f.facts, f.catalog)[0].revision == 3);
    CHECK(CaptureChanged(f.facts, f.catalog, "cell_0_1").empty());
    CHECK(CaptureChanged(f.facts, f.catalog, "cell_0_0").size() == 1);
}

TEST_CASE("dirty tracking returns exactly the objects touched since a revision") {
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    const auto mark = f.facts.Revision();
    CHECK(DirtyObjects(f.facts, f.catalog, mark).empty());
    f.Set(SwitchOnKey("sw_a"), true);
    f.Set(SwitchDisabledKey("sw_a"), false);
    f.Set("placeholder.door.not_in_catalog.open", true);
    f.Set(LeverPositionKey("lv_a"), std::int64_t{1});
    CHECK(DirtyObjects(f.facts, f.catalog, mark) == std::vector<std::string>{"lv_a", "sw_a"});
    CHECK(DirtyObjects(f.facts, f.catalog, 0).size() == 3);
    CHECK(DirtyObjects(f.facts, f.catalog, 9999).empty()); // a revision from the future is not an error
}

TEST_CASE("round trip: serialize, parse, restore into a fresh world gives the same changed state") {
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    f.Set(TestDoorLockedKey("door_b"), false);
    f.Set(SwitchOnKey("sw_a"), true);
    f.Set(LeverPositionKey("lv_a"), std::int64_t{1});
    const auto records = CaptureChanged(f.facts, f.catalog);
    REQUIRE(records.size() == 4);
    const std::string text = SerializeRecords(records);
    auto parsed = ParseRecords(text);
    REQUIRE(parsed.HasValue());
    CHECK(parsed.Value() == records);
    CHECK(SerializeRecords(parsed.Value()) == text); // canonical

    WorldFacts fresh;
    GameEventBus bus;
    std::uint64_t restoredEvents = 0;
    ScopedSubscription sub(bus, bus.Subscribe([&](const GameEvent& e) { restoredEvents += e.count; }, {GameEventType::PersistentStateRestored}));
    const auto report = ApplyRecords(fresh, f.catalog, parsed.Value(), RestoreMode::Overlay, std::nullopt, &bus);
    CHECK(report.restored.size() == 4);
    CHECK(report.missing.empty());
    CHECK(report.rejected.empty());
    CHECK(restoredEvents == 4);
    CHECK(CaptureChanged(fresh, f.catalog).size() == 4);
    CHECK(*fresh.FindAs<bool>(TestDoorOpenKey("door_a")) == true);
    CHECK(*fresh.FindAs<bool>(TestDoorLockedKey("door_b")) == false);
    CHECK(*fresh.FindAs<std::int64_t>(LeverPositionKey("lv_a")) == 1);
}

TEST_CASE("output does not depend on insertion order and is stable text") {
    Fixture a, b;
    a.Set(TestDoorOpenKey("door_a"), true);
    a.Set(SwitchOnKey("sw_a"), true);
    b.Set(SwitchOnKey("sw_a"), true);
    b.Set(TestDoorOpenKey("door_a"), true);
    CHECK(SerializeRecords(CaptureChanged(a.facts, a.catalog)) == SerializeRecords(CaptureChanged(b.facts, b.catalog)));
    CHECK(SerializeRecords({}) == "SPN-OBJECTS 1\nobjects 0\nend\n");
    auto empty = ParseRecords("SPN-OBJECTS 1\nobjects 0\nend\n");
    REQUIRE(empty.HasValue());
    CHECK(empty.Value().empty());
}

TEST_CASE("string fields survive spaces, percent signs, newlines and the empty string") {
    ObjectRecord r;
    r.id = "note_a";
    r.kind = *ContentId::Parse("placeholder:interactable.note");
    r.region = "cell_0_0";
    r.fields = {{"text", std::string("a b%c\nd\xC3\xA4")}, {"empty", std::string()}, {"n", std::int64_t{-9223372036854775807LL - 1}}, {"flag", true}};
    auto parsed = ParseRecords(SerializeRecords({r}));
    REQUIRE(parsed.HasValue());
    CHECK(parsed.Value() == std::vector<ObjectRecord>{r});
}

TEST_CASE("malformed input is rejected with a precise code - nothing partial") {
    const std::string good = "SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield open b 1\nend\n";
    REQUIRE(ParseRecords(good).HasValue());
    const auto code = [](const std::string& text) { auto r = ParseRecords(text); return r.HasValue() ? PersistErrorCode::NoValidFile : r.Error().code; };
    CHECK(code("") == PersistErrorCode::InvalidHeader);
    CHECK(code("garbage\n") == PersistErrorCode::InvalidHeader);
    CHECK(code("SPN-OBJECTS x\nobjects 0\nend\n") == PersistErrorCode::InvalidHeader);
    CHECK(code("SPN-OBJECTS 2\nobjects 0\nend\n") == PersistErrorCode::UnsupportedVersion);
    CHECK(code("SPN-OBJECTS 0\nobjects 0\nend\n") == PersistErrorCode::UnsupportedVersion);
    CHECK(code("SPN-OBJECTS 1\nobjects 0\nend") == PersistErrorCode::TruncatedData);   // no final newline
    CHECK(code("SPN-OBJECTS 1\n") == PersistErrorCode::TruncatedData);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nend\n") == PersistErrorCode::MalformedLine);
    CHECK(code("SPN-OBJECTS 1\nobjects 5000000\nend\n") == PersistErrorCode::MalformedLine);
    CHECK(code("SPN-OBJECTS 1\nobjects 0\nend\nextra\n") == PersistErrorCode::TrailingData);
    CHECK(code("SPN-OBJECTS 1\nobjects 0\n") == PersistErrorCode::TruncatedData);
    CHECK(code("SPN-OBJECTS 1\nobjects 0\nnot_end\n") == PersistErrorCode::CountMismatch);
    CHECK(code("SPN-OBJECTS 1\nobjects 2\nobject door_a placeholder:interactable.door cell_0_0 3 0\nend\n") == PersistErrorCode::MalformedLine);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject Door_A placeholder:interactable.door cell_0_0 3 0\nend\n") == PersistErrorCode::InvalidId);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a nonsense cell_0_0 3 0\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door BAD 3 0\nend\n") == PersistErrorCode::InvalidRegion);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 -3 0\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 03 0\nend\n") == PersistErrorCode::InvalidToken); // no leading zeros: one spelling per value
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nend\n") == PersistErrorCode::MalformedLine);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield open b 2\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield open i 99999999999999999999\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield open x 1\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield Open b 1\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield t s %zz\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 1\nfield t s %4\nend\n") == PersistErrorCode::InvalidToken);
    CHECK(code("SPN-OBJECTS 1\nobjects 1\nobject door_a placeholder:interactable.door cell_0_0 3 2\nfield open b 1\nfield open b 0\nend\n") == PersistErrorCode::DuplicateObject);
    CHECK(code("SPN-OBJECTS 1\nobjects 2\nobject d placeholder:interactable.door c 1 0\nobject d placeholder:interactable.door c 1 0\nend\n") == PersistErrorCode::DuplicateObject);
}

TEST_CASE("every truncation of a valid file is rejected, never accepted as a shorter valid file") {
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    f.Set(SwitchOnKey("sw_a"), true);
    f.Set(LeverPositionKey("lv_a"), std::int64_t{1});
    const std::string text = SerializeRecords(CaptureChanged(f.facts, f.catalog));
    REQUIRE(ParseRecords(text).HasValue());
    for (std::size_t n = 0; n < text.size(); ++n) CHECK_MESSAGE(!ParseRecords(std::string_view(text).substr(0, n)).HasValue(), "prefix of length ", n);
}

TEST_CASE("random single-byte corruption never crashes the parser") {
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    f.Set(SwitchOnKey("sw_a"), true);
    const std::string text = SerializeRecords(CaptureChanged(f.facts, f.catalog));
    std::mt19937 rng(7);
    for (int i = 0; i < 2000; ++i) {
        std::string mutated = text;
        mutated[rng() % mutated.size()] = static_cast<char>(rng() % 256);
        (void)ParseRecords(mutated); // the result may be valid (a harmless change) or an error; it must not crash (ASan/UBSan runs)
    }
}

TEST_CASE("restore: unknown objects become orphans, mismatches are rejected untouched, unknown fields are skipped") {
    Fixture f;
    ObjectRecord good;
    good.id = "door_a"; good.kind = TestDoorKind(); good.region = "cell_0_0"; good.fields = {{"open", true}, {"future_field", std::int64_t{4}}};
    ObjectRecord orphan;
    orphan.id = "door_gone"; orphan.kind = TestDoorKind(); orphan.region = "cell_9_9"; orphan.fields = {{"open", true}};
    ObjectRecord wrongKind = good; wrongKind.id = "sw_a"; wrongKind.region = "cell_0_1"; // kind says door, catalog says switch
    ObjectRecord wrongRegion = good; wrongRegion.id = "door_b"; wrongRegion.region = "cell_1_1";
    ObjectRecord wrongType;
    wrongType.id = "lv_a"; wrongType.kind = LeverKind(); wrongType.region = "cell_0_1"; wrongType.fields = {{"position", true}}; // default is an integer
    ObjectRecord longText = good; longText.id = "door_b"; // valid region, string too long for a bool default is a type mismatch anyway
    const auto report = ApplyRecords(f.facts, f.catalog, {good, orphan, wrongKind, wrongRegion, wrongType});
    CHECK(report.restored == std::vector<std::string>{"door_a"});
    REQUIRE(report.missing.size() == 1);
    CHECK(report.missing[0].id == "door_gone");
    CHECK(report.rejected.size() == 3);
    CHECK(report.unknownFields == std::vector<std::string>{"door_a.future_field"});
    CHECK(*f.facts.FindAs<bool>(TestDoorOpenKey("door_a")) == true);
    CHECK(f.facts.Find(SwitchOnKey("sw_a")) == nullptr);
    CHECK(f.facts.Find(LeverPositionKey("lv_a")) == nullptr);
    // live state of another type than the record: rejected, live state kept
    Fixture g;
    g.Set(TestDoorOpenKey("door_a"), std::int64_t{1});
    CHECK(ApplyRecords(g.facts, g.catalog, {good}).rejected.size() == 1);
    CHECK(*g.facts.FindAs<std::int64_t>(TestDoorOpenKey("door_a")) == 1);
}

TEST_CASE("Replace mode restores exactly: live changes not in the saved records are reset to defaults") {
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    const auto saved = CaptureChanged(f.facts, f.catalog);
    f.Set(SwitchOnKey("sw_a"), true);               // changed after the save
    f.Set(TestDoorOpenKey("door_b"), true);
    f.Set("placeholder.unrelated.thing.value", std::int64_t{1});
    const auto report = ApplyRecords(f.facts, f.catalog, saved, RestoreMode::Replace);
    CHECK(report.restored.size() == 1);
    CHECK(*f.facts.FindAs<bool>(TestDoorOpenKey("door_a")) == true);
    CHECK(f.facts.Find(SwitchOnKey("sw_a")) == nullptr);
    CHECK(f.facts.Find(TestDoorOpenKey("door_b")) == nullptr);
    CHECK(f.facts.Find("placeholder.unrelated.thing.value") != nullptr); // facts of no catalog object are untouched
    // region-scoped replace leaves other regions alone
    Fixture h;
    h.Set(TestDoorOpenKey("door_a"), true);
    h.Set(SwitchOnKey("sw_a"), true);
    ApplyRecords(h.facts, h.catalog, {}, RestoreMode::Replace, "cell_0_0");
    CHECK(h.facts.Find(TestDoorOpenKey("door_a")) == nullptr);
    CHECK(h.facts.Find(SwitchOnKey("sw_a")) != nullptr);
}

TEST_CASE("regions: unload keeps changed state, removes live facts, reload restores, other regions are untouched") {
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    f.Set(SwitchOnKey("sw_a"), true);
    RegionStateCache cache;
    CHECK(cache.IsLoaded("cell_0_0"));
    CHECK(cache.Unload("cell_0_0", f.facts, f.catalog) == 1);
    CHECK_FALSE(cache.IsLoaded("cell_0_0"));
    CHECK(cache.Unload("cell_0_0", f.facts, f.catalog) == 0); // twice is a no-op
    CHECK(f.facts.Find(TestDoorOpenKey("door_a")) == nullptr);
    CHECK(f.facts.Find(SwitchOnKey("sw_a")) != nullptr);
    CHECK(cache.RetainedCount() == 1);
    // the snapshot still contains the unloaded object
    const auto snapshot = cache.Snapshot(f.facts, f.catalog);
    REQUIRE(snapshot.size() == 2);
    CHECK(snapshot[0].id == "door_a");
    CHECK(snapshot[1].id == "sw_a");
    // restore from a save made while unloaded, into a world that has the region loaded again
    GameEventBus bus;
    int restored = 0;
    ScopedSubscription sub(bus, bus.Subscribe([&](const GameEvent& e) { restored += static_cast<int>(e.count); }, {GameEventType::PersistentStateRestored}));
    const auto report = cache.Load("cell_0_0", f.facts, f.catalog, &bus);
    CHECK(report.restored == std::vector<std::string>{"door_a"});
    CHECK(restored == 1);
    CHECK(cache.IsLoaded("cell_0_0"));
    CHECK(cache.RetainedCount() == 0);
    CHECK(*f.facts.FindAs<bool>(TestDoorOpenKey("door_a")) == true);
    CHECK(cache.Load("cell_0_0", f.facts, f.catalog).restored.empty()); // loading a loaded region is a no-op
    CHECK(cache.Unload("cell_9_9", f.facts, f.catalog) == 0);           // unknown region: nothing to keep, no error
    CHECK_FALSE(cache.IsLoaded("cell_9_9"));
}

TEST_CASE("orphans survive save, load and a content change - live state wins over retained state") {
    Fixture f;
    ObjectRecord orphan;
    orphan.id = "door_gone"; orphan.kind = TestDoorKind(); orphan.region = "cell_9_9"; orphan.fields = {{"open", true}};
    RegionStateCache cache;
    cache.AddOrphans({orphan});
    f.Set(TestDoorOpenKey("door_a"), true);
    auto snapshot = cache.Snapshot(f.facts, f.catalog);
    REQUIRE(snapshot.size() == 2);
    CHECK(snapshot[0].id == "door_a");
    CHECK(snapshot[1] == orphan);
    // retained and live state of one object: the live one wins
    cache.Unload("cell_0_0", f.facts, f.catalog);
    f.Set(TestDoorOpenKey("door_a"), false);
    f.Set(TestDoorLockedKey("door_a"), true);
    snapshot = cache.Snapshot(f.facts, f.catalog);
    const auto door = std::find_if(snapshot.begin(), snapshot.end(), [](const ObjectRecord& r) { return r.id == "door_a"; });
    REQUIRE(door != snapshot.end());
    CHECK(door->fields.contains("locked"));
}

TEST_CASE("duplicate ids: the catalog refuses them and a file with duplicates does not parse") {
    Fixture f;
    CHECK(f.catalog.Add(Door("door_a", "cell_5_5"))->code == PersistErrorCode::DuplicateId);
    CHECK(f.catalog.Add(Switch("door_a", "cell_0_0"))->code == PersistErrorCode::DuplicateId); // same id, other kind
}

TEST_CASE("atomic file writing: round trip, backup of the last good file, failure leaves the old file intact") {
    const fs::path dir = TempDir("files");
    const fs::path file = dir / "objects.spnobj";
    Fixture f;
    f.Set(TestDoorOpenKey("door_a"), true);
    const std::string first = SerializeRecords(CaptureChanged(f.facts, f.catalog));
    CHECK_FALSE(WriteFileAtomic(file, first).has_value());
    CHECK_FALSE(fs::exists(file.string() + ".tmp"));
    CHECK_FALSE(fs::exists(file.string() + ".bak")); // nothing to back up yet
    auto loaded = LoadRecordsFile(file);
    REQUIRE(loaded.HasValue());
    CHECK_FALSE(loaded.Value().usedBackup);
    CHECK(loaded.Value().records.size() == 1);

    f.Set(SwitchOnKey("sw_a"), true);
    const std::string second = SerializeRecords(CaptureChanged(f.facts, f.catalog));
    CHECK_FALSE(WriteFileAtomic(file, second).has_value());
    CHECK(LoadRecordsFile(file).Value().records.size() == 2);
    REQUIRE(fs::exists(file.string() + ".bak"));

    // a corrupt main file is recovered from the backup, which a later write must not destroy
    { std::ofstream out(file, std::ios::binary | std::ios::trunc); out << "SPN-OBJECTS 1\nobjects 3\nobj"; }
    auto recovered = LoadRecordsFile(file);
    REQUIRE(recovered.HasValue());
    CHECK(recovered.Value().usedBackup);
    CHECK(recovered.Value().records.size() == 1); // the backup holds the first version
    CHECK_FALSE(WriteFileAtomic(file, second).has_value());
    CHECK(LoadRecordsFile(file).Value().records.size() == 2);
    {
        std::ifstream bak(file.string() + ".bak", std::ios::binary);
        std::string text((std::istreambuf_iterator<char>(bak)), std::istreambuf_iterator<char>());
        CHECK(text == first); // the corrupt main file did not overwrite the good backup
    }

    // a leftover temp file from an interrupted write is ignored
    { std::ofstream out(file.string() + ".tmp", std::ios::binary | std::ios::trunc); out << "SPN-OBJECTS 1\nobjects 99\n"; }
    CHECK(LoadRecordsFile(file).Value().records.size() == 2);
    CHECK_FALSE(WriteFileAtomic(file, first).has_value());
    CHECK_FALSE(fs::exists(file.string() + ".tmp"));
    fs::remove_all(dir);
}

TEST_CASE("file failures are reported, never silent") {
    const fs::path dir = TempDir("fail");
    const auto missingDir = dir / "no_such_dir" / "objects.spnobj";
    const auto error = WriteFileAtomic(missingDir, "SPN-OBJECTS 1\nobjects 0\nend\n");
    REQUIRE(error.has_value());
    CHECK(error->code == PersistErrorCode::Io);
    const auto none = LoadRecordsFile(dir / "absent.spnobj");
    REQUIRE_FALSE(none.HasValue());
    CHECK(none.Error().code == PersistErrorCode::NoValidFile);
    // both files corrupt
    { std::ofstream(dir / "x.spnobj") << "junk"; std::ofstream(dir / "x.spnobj.bak") << "junk"; }
    CHECK(LoadRecordsFile(dir / "x.spnobj").Error().code == PersistErrorCode::NoValidFile);
    fs::remove_all(dir);
}

TEST_CASE("end to end: play, save, load into a new session, continue - door lock state and device state come back") {
    // session 1
    WorldObjectCatalog catalog;
    REQUIRE_FALSE(catalog.Add(Door("front", "cell_0_0", true)).has_value());
    REQUIRE_FALSE(catalog.Add(Switch("sw_front", "cell_0_0")).has_value());
    InteractionRegistry registry;
    REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
    REQUIRE_FALSE(RegisterDevices(registry).has_value());
    WorldFacts facts;
    GameEventBus bus;
    InteractionSystem system(registry, facts, &bus);
    for (const auto& [id, kind, x] : {std::tuple{"front", TestDoorKind(), 1.0}, std::tuple{"sw_front", SwitchKind(), 1.0}}) {
        InteractableDescriptor d;
        d.id = id; d.kind = kind; d.centre = {x, id[0] == 'f' ? -0.4 : 0.4, 1.4};
        d.defaults.values = id[0] == 'f' ? InteractionDefaults{{{"locked", true}}}.values : InteractionDefaults{}.values;
        REQUIRE_FALSE(system.AddInteractable(d).has_value());
    }
    REQUIRE_FALSE(system.AddLink({"sw_front", 1, "front", TestDoorLockedKey("front"), false, std::nullopt}).has_value());
    InteractionRequest r;
    r.actor.eye = {0.0, 0.0, 1.5};
    r.targetId = "front";
    CHECK(system.Interact(r).status == InteractStatus::Locked);
    r.targetId = "sw_front";
    CHECK(system.Interact(r).status == InteractStatus::Success);   // unlocks the front door through the link
    r.targetId = "front";
    CHECK(system.Interact(r).status == InteractStatus::Success);   // opens it
    const std::string saved = SerializeRecords(CaptureChanged(facts, catalog));

    // session 2: a new process, same catalog and registry, nothing but the saved text
    WorldFacts facts2;
    auto parsed = ParseRecords(saved);
    REQUIRE(parsed.HasValue());
    ApplyRecords(facts2, catalog, parsed.Value(), RestoreMode::Replace);
    GameEventBus bus2;
    InteractionSystem system2(registry, facts2, &bus2);
    for (const auto& [id, d] : std::vector<std::pair<std::string, const InteractableDescriptor*>>{{"front", system.Find("front")}, {"sw_front", system.Find("sw_front")}}) {
        REQUIRE(d != nullptr);
        REQUIRE_FALSE(system2.AddInteractable(*d).has_value());
    }
    r.targetId = "front"; // open and unlocked: closing is allowed
    const auto closed = system2.Interact(r);
    CHECK(closed.status == InteractStatus::Success);
    CHECK(*facts2.FindAs<bool>(TestDoorOpenKey("front")) == false);
    CHECK(*facts2.FindAs<bool>(TestDoorLockedKey("front")) == false); // the unlock persisted
    CHECK(*facts2.FindAs<bool>(SwitchOnKey("sw_front")) == true);
}
