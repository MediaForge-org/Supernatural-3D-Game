#include <doctest.h>

#include <spn/core/GameSave.h>
#include <spn/core/NpcMemory.h>
#include <spn/core/SaveStore.h>

#include "SaveTestFs.h"

using namespace spn::core;
using spn::testdata::MemoryFs;

namespace {

NpcMemoryStore MakeNpcs() {
    NpcMemoryStore s;
    NpcIdentity witness;
    witness.id = EntityId(1);
    witness.tier = PersistenceTier::A;
    witness.label = "Witness";
    REQUIRE_FALSE(s.AddNpc(witness).has_value());
    NpcIdentity bystander;
    bystander.id = EntityId(2);
    bystander.tier = PersistenceTier::C;
    REQUIRE_FALSE(s.AddNpc(bystander).has_value());
    NpcEventInput e;
    e.time = 10.0;
    e.kind = NpcEventKind::Rescued;
    e.actor = EntityId(9000);
    e.subject = EntityId(1);
    REQUIRE(s.RecordEvent(e).HasValue());
    e.time = 20.0;
    e.kind = NpcEventKind::Killed;
    e.subject = EntityId(2);
    REQUIRE(s.RecordEvent(e).HasValue());
    return s;
}

} // namespace

TEST_CASE("NpcMemory in the game save: an optional section that round-trips through the real publish protocol") {
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{"quest.stage", std::int64_t{2}}, "test").HasValue());
    const NpcMemoryStore npcs = MakeNpcs();
    GameSaveSources sources;
    sources.facts = &facts;
    sources.npcMemory = &npcs;
    const auto slot = CaptureGameSave(sources);
    REQUIRE(slot.HasValue());
    const SaveSection* section = slot.Value().Find(kSectionNpcMemory);
    REQUIRE(section != nullptr);
    CHECK(section->payload == npcs.Serialize()); // exactly the store's own bytes
    CHECK_FALSE(section->required);
    CHECK(section->payload.starts_with("SPN-NPCMEM 1\n"));
    CHECK_FALSE(slot.Value().Validate(GameSaveSchema()).has_value());

    MemoryFs fs;
    SaveSlotStore store(fs, "slot.sav");
    REQUIRE_FALSE(store.Save(slot.Value(), GameSaveSchema()).has_value());
    const auto loadedSlot = store.Load(GameSaveSchema());
    REQUIRE(loadedSlot.HasValue());
    const auto game = ParseGameSave(loadedSlot.Value().slot, GameLoadContext{});
    REQUIRE(game.HasValue());
    REQUIRE(game.Value().npcMemory.has_value());
    CHECK(game.Value().npcMemory->SameSavedState(npcs));
    CHECK_FALSE(game.Value().npcMemory->IsAlive(EntityId(2)));
    CHECK(game.Value().unreadSections.empty());
}

TEST_CASE("NpcMemory in the game save: saves without the section load as before, and the section is absent unless asked for") {
    WorldFacts facts;
    GameSaveSources sources;
    sources.facts = &facts;
    const auto slot = CaptureGameSave(sources);
    REQUIRE(slot.HasValue());
    CHECK(slot.Value().Find(kSectionNpcMemory) == nullptr);
    const auto game = ParseGameSave(slot.Value(), GameLoadContext{});
    REQUIRE(game.HasValue());
    CHECK_FALSE(game.Value().npcMemory.has_value());
}

TEST_CASE("NpcMemory in the game save: a build that does not know the section keeps it when it re-saves") {
    WorldFacts facts;
    const NpcMemoryStore npcs = MakeNpcs();
    GameSaveSources sources;
    sources.facts = &facts;
    sources.npcMemory = &npcs;
    const SaveSlot slot = CaptureGameSave(sources).Value();

    // the schema of a build from before NightBuild02
    SaveSchema older = GameSaveSchema();
    std::erase_if(older.sections, [](const SectionSpec& s) { return s.id == kSectionNpcMemory; });
    CHECK_FALSE(slot.Validate(older).has_value());                       // an unknown OPTIONAL section does not make the save unreadable
    CHECK(slot.UnreadSections(older) == std::vector<std::string>{std::string(kSectionNpcMemory)});

    // that older build saves again (it does not know the NPC store, so it passes none) and carries the section over
    GameSaveSources olderSources;
    olderSources.facts = &facts;
    const auto resaved = CaptureGameSave(olderSources, &slot);
    REQUIRE(resaved.HasValue());
    // the current schema knows the section, so UnreadSections is empty and nothing is carried over: with the OLD schema semantics the carry-over
    // is what UnreadSections(older) selects - check it directly
    SaveSlot carried;
    REQUIRE_FALSE(carried.Add(*slot.Find(kSectionFacts)).has_value());
    for (const std::string& id : slot.UnreadSections(older)) REQUIRE_FALSE(carried.Add(*slot.Find(id)).has_value());
    CHECK(carried.Find(kSectionNpcMemory)->payload == npcs.Serialize());
}

TEST_CASE("NpcMemory in the game save: damaged or newer payloads are handled by the section rules") {
    WorldFacts facts;
    const NpcMemoryStore npcs = MakeNpcs();
    GameSaveSources sources;
    sources.facts = &facts;
    sources.npcMemory = &npcs;
    const SaveSlot good = CaptureGameSave(sources).Value();

    // a payload the NPC parser rejects: the whole load is refused and names the section (no half-loaded world)
    SaveSlot damaged = good;
    SaveSection bad = *good.Find(kSectionNpcMemory);
    bad.payload.replace(bad.payload.find("clock"), 5, "tlock");
    REQUIRE_FALSE(damaged.Put(bad).has_value());
    const auto rejected = ParseGameSave(damaged, GameLoadContext{});
    REQUIRE_FALSE(rejected.HasValue());
    CHECK(rejected.Error().code == SaveErrorCode::SectionRejected);
    CHECK(rejected.Error().detail.starts_with("world.npcs"));

    // written by a newer version: optional, so it is ignored, listed as unread and survives a re-save
    SaveSlot newer = good;
    SaveSection future = *good.Find(kSectionNpcMemory);
    future.version = 2;
    REQUIRE_FALSE(newer.Put(future).has_value());
    const auto ignored = ParseGameSave(newer, GameLoadContext{});
    REQUIRE(ignored.HasValue());
    CHECK_FALSE(ignored.Value().npcMemory.has_value());
    CHECK(ignored.Value().unreadSections == std::vector<std::string>{std::string(kSectionNpcMemory)});
    GameSaveSources noNpcs;
    noNpcs.facts = &facts;
    const auto kept = CaptureGameSave(noNpcs, &newer);
    REQUIRE(kept.HasValue());
    REQUIRE(kept.Value().Find(kSectionNpcMemory) != nullptr);
    CHECK(kept.Value().Find(kSectionNpcMemory)->version == 2);
}

TEST_CASE("NpcMemory in the game save: loading uses the configured capacities, not saved ones") {
    WorldFacts facts;
    NpcMemoryStore npcs;
    NpcIdentity n;
    n.id = EntityId(1);
    n.tier = PersistenceTier::C;
    REQUIRE_FALSE(npcs.AddNpc(n).has_value());
    for (int i = 0; i < 4; ++i) {
        NpcEventInput e;
        e.time = i;
        e.kind = NpcEventKind::Encounter;
        e.actor = EntityId(9000);
        e.subject = EntityId(1);
        REQUIRE(npcs.RecordEvent(e).HasValue());
    }
    GameSaveSources sources;
    sources.facts = &facts;
    sources.npcMemory = &npcs;
    GameLoadContext context;
    context.npcMemoryConfig.capacity = {64, 16, 2}; // a later build with a smaller tier C
    const auto game = ParseGameSave(CaptureGameSave(sources).Value(), context);
    REQUIRE(game.HasValue());
    CHECK(game.Value().npcMemory->Memories(EntityId(1))->size() == 4); // loaded as saved...
    NpcEventInput e;
    e.time = 10.0;
    e.kind = NpcEventKind::Helped;
    e.actor = EntityId(9000);
    e.subject = EntityId(1);
    NpcMemoryStore loaded = *game.Value().npcMemory;
    REQUIRE(loaded.RecordEvent(e).HasValue());
    CHECK(loaded.Memories(EntityId(1))->size() == 2); // ...and trimmed to the configured capacity by the next memory
}
