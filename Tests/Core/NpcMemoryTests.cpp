#include <algorithm>
#include <cmath>
#include <doctest.h>
#include <spn/core/NpcMemory.h>

using namespace spn::core;
using doctest::Approx;

namespace {

constexpr std::uint64_t kPlayer = 9000; // a player entity: it has no NPC record
EntityId Id(std::uint64_t v) { return EntityId(v); }

NpcIdentity Npc(std::uint64_t id, PersistenceTier tier = PersistenceTier::A, const char* label = "") {
    NpcIdentity n;
    n.id = Id(id);
    n.tier = tier;
    n.label = label;
    return n;
}

NpcEventInput Event(double time, NpcEventKind kind, std::uint64_t actor, std::uint64_t subject = 0, double intensity = 1.0) {
    NpcEventInput e;
    e.time = time;
    e.kind = kind;
    e.actor = Id(actor);
    e.subject = Id(subject);
    e.intensity = intensity;
    return e;
}

NpcMemoryStore MakeStore() {
    NpcMemoryStore s;
    REQUIRE_FALSE(s.AddNpc(Npc(1, PersistenceTier::A, "Alice")).has_value());
    REQUIRE_FALSE(s.AddNpc(Npc(2, PersistenceTier::B, "Bob")).has_value());
    REQUIRE_FALSE(s.AddNpc(Npc(3, PersistenceTier::C)).has_value());
    return s;
}

std::uint64_t MustRecord(NpcMemoryStore& s, const NpcEventInput& e) {
    auto r = s.RecordEvent(e);
    REQUIRE(r.HasValue());
    return r.Value();
}

} // namespace

TEST_CASE("NpcMemory: default configuration is valid, broken ones are rejected") {
    CHECK_FALSE(Validate(NpcMemoryConfig{}).has_value());
    NpcMemoryConfig c;
    c.capacity[0] = 0;
    CHECK(Validate(c).has_value());
    c = {};
    c.halfLifeSeconds[1] = 0.0;
    CHECK(Validate(c).has_value());
    c = {};
    c.halfLifeSeconds[2] = std::nan("");
    CHECK(Validate(c).has_value());
    c = {};
    c.effects[1].importance = 1.5;
    CHECK(Validate(c).has_value());
    c = {};
    c.effects[3].fear = -0.1;
    CHECK(Validate(c).has_value());
    c = {};
    c.witnessWeight = 2.0;
    CHECK(Validate(c).has_value());
    c = {};
    c.forgetBelow = 1.0;
    CHECK(Validate(c).has_value());
}

TEST_CASE("NpcMemory: identities have stable unique ids and survive") {
    NpcMemoryStore s;
    CHECK(s.AddNpc(Npc(0)) == NpcMemoryError{NpcMemoryErrorCode::InvalidId, 0});
    REQUIRE_FALSE(s.AddNpc(Npc(5, PersistenceTier::B, "Eve")).has_value());
    CHECK(s.AddNpc(Npc(5)) == NpcMemoryError{NpcMemoryErrorCode::DuplicateNpc, 0});
    REQUIRE(s.Find(Id(5)) != nullptr);
    CHECK(s.Find(Id(5))->label == "Eve");
    CHECK(s.Find(Id(5))->tier == PersistenceTier::B);
    CHECK(s.Find(Id(6)) == nullptr);
    CHECK(s.IsAlive(Id(5)));
    CHECK(s.IsAlive(Id(kPlayer))); // unknown entities (players) are never reported dead
    CHECK(s.Knows(Id(5)));
    CHECK_FALSE(s.Knows(Id(kPlayer)));
    NpcIdentity longName = Npc(7);
    longName.label = std::string(300, 'x');
    CHECK(s.AddNpc(longName)->code == NpcMemoryErrorCode::InvalidValue);
    NpcMemoryConfig small;
    small.maxNpcs = 2;
    NpcMemoryStore limited(small);
    REQUIRE_FALSE(limited.AddNpc(Npc(1)).has_value());
    REQUIRE_FALSE(limited.AddNpc(Npc(2)).has_value());
    CHECK(limited.AddNpc(Npc(3))->code == NpcMemoryErrorCode::TooManyNpcs);
    CHECK(limited.NpcIds() == std::vector<EntityId>{Id(1), Id(2)});
}

TEST_CASE("NpcMemory: an event is remembered by actor, subject and listed witnesses only") {
    NpcMemoryStore s = MakeStore();
    NpcEventInput e = Event(10.0, NpcEventKind::Helped, kPlayer, 1);
    e.witnesses = {Witness{Id(2), true, 0.8}};
    const std::uint64_t seq = MustRecord(s, e);
    REQUIRE(s.Memories(Id(1)) != nullptr);
    REQUIRE(s.Memories(Id(1))->size() == 1);
    CHECK(s.Memories(Id(1))->front().sequence == seq);
    CHECK(s.Memories(Id(1))->front().accuracy == 1.0);
    CHECK(s.Memories(Id(1))->front().firstHand);
    REQUIRE(s.Memories(Id(2))->size() == 1);
    CHECK(s.Memories(Id(2))->front().accuracy == Approx(0.8));
    CHECK(s.Memories(Id(3))->empty()); // nobody told Carol: she saw nothing
    CHECK(s.Memories(Id(kPlayer)) == nullptr);
    // sequences increase; a witness that is also the subject is recorded once, as the subject (accuracy 1)
    NpcEventInput twice = Event(11.0, NpcEventKind::Injured, kPlayer, 1);
    twice.witnesses = {Witness{Id(1), false, 0.1}, Witness{Id(1), true, 0.5}};
    const std::uint64_t seq2 = MustRecord(s, twice);
    CHECK(seq2 > seq);
    CHECK(s.Memories(Id(1))->size() == 2);
    CHECK(s.Memories(Id(1))->back().accuracy == 1.0);
    CHECK(s.Memories(Id(1))->back().firstHand);
}

TEST_CASE("NpcMemory: invalid events are rejected and change nothing") {
    NpcMemoryStore s = MakeStore();
    MustRecord(s, Event(100.0, NpcEventKind::Encounter, kPlayer, 1));
    const std::string before = s.Serialize();
    CHECK(s.RecordEvent(Event(50.0, NpcEventKind::Encounter, kPlayer, 1)).Error().code == NpcMemoryErrorCode::StaleEvent); // one world clock, never backwards
    CHECK(s.RecordEvent(Event(std::nan(""), NpcEventKind::Encounter, kPlayer, 1)).Error().code == NpcMemoryErrorCode::InvalidTime);
    CHECK(s.RecordEvent(Event(-1.0, NpcEventKind::Encounter, kPlayer, 1)).Error().code == NpcMemoryErrorCode::InvalidTime);
    CHECK(s.RecordEvent(Event(200.0, NpcEventKind::Encounter, 0, 1)).Error().code == NpcMemoryErrorCode::InvalidId);
    CHECK(s.RecordEvent(Event(200.0, NpcEventKind::Encounter, kPlayer, 1, 1.5)).Error().code == NpcMemoryErrorCode::InvalidValue);
    CHECK(s.RecordEvent(Event(200.0, NpcEventKind::Encounter, kPlayer, 1, std::nan(""))).Error().code == NpcMemoryErrorCode::InvalidValue);
    NpcEventInput badWitness = Event(200.0, NpcEventKind::Witnessed, kPlayer, 1);
    badWitness.witnesses = {Witness{Id(777), true, 1.0}};
    CHECK(s.RecordEvent(badWitness).Error().code == NpcMemoryErrorCode::InvalidWitness);
    badWitness.witnesses = {Witness{Id(2), true, 1.5}};
    CHECK(s.RecordEvent(badWitness).Error().code == NpcMemoryErrorCode::InvalidValue);
    NpcEventInput badLocation = Event(200.0, NpcEventKind::Encounter, kPlayer, 1);
    badLocation.location.x = std::nan("");
    CHECK(s.RecordEvent(badLocation).Error().code == NpcMemoryErrorCode::InvalidValue);
    CHECK(s.Serialize() == before);
    // equal time is fine (several events in one tick)
    CHECK(s.RecordEvent(Event(100.0, NpcEventKind::Encounter, kPlayer, 2)).HasValue());
}

TEST_CASE("NpcMemory: the dead stay dead, act no more and remember no more") {
    NpcMemoryStore s = MakeStore();
    MustRecord(s, Event(10.0, NpcEventKind::Encounter, kPlayer, 1));
    NpcEventInput kill = Event(20.0, NpcEventKind::Killed, kPlayer, 1);
    kill.witnesses = {Witness{Id(2), true, 1.0}};
    MustRecord(s, kill);
    CHECK_FALSE(s.IsAlive(Id(1)));
    CHECK(s.IsAlive(Id(2)));
    CHECK(s.Memories(Id(1))->size() == 2); // its own death is its last memory
    // a dead entity cannot be the actor
    CHECK(s.RecordEvent(Event(30.0, NpcEventKind::Helped, 1, 2)).Error().code == NpcMemoryErrorCode::ActorDead);
    // and records nothing new, even as a listed witness or as the subject of a later event
    NpcEventInput later = Event(40.0, NpcEventKind::Witnessed, kPlayer, 1);
    later.witnesses = {Witness{Id(1), true, 1.0}, Witness{Id(2), true, 1.0}};
    MustRecord(s, later);
    CHECK(s.Memories(Id(1))->size() == 2);
    CHECK(s.Memories(Id(2))->size() == 2); // the kill it witnessed + the later event it witnessed; the dead Alice recorded nothing
    // a Killed event on a player entity (no NPC record) kills nobody in the store
    MustRecord(s, Event(50.0, NpcEventKind::Killed, 2, kPlayer));
    CHECK(s.IsAlive(Id(kPlayer)));
    // the death survives a save
    const auto loaded = NpcMemoryStore::Deserialize(s.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK_FALSE(loaded.Value().IsAlive(Id(1)));
}

TEST_CASE("NpcMemory: dispositions follow what the NPC remembers the other having done") {
    NpcMemoryStore s = MakeStore();
    CHECK(s.DispositionOf(Id(1), Id(kPlayer), 0.0).Value().memories == 0);
    MustRecord(s, Event(0.0, NpcEventKind::Helped, kPlayer, 1));
    const Disposition helped = s.DispositionOf(Id(1), Id(kPlayer), 0.0).Value();
    CHECK(helped.trust == Approx(0.5));
    CHECK(helped.fear == 0.0);
    CHECK(helped.hostility == 0.0);
    CHECK(helped.memories == 1);
    // a different person is unaffected
    CHECK(s.DispositionOf(Id(1), Id(2), 0.0).Value().memories == 0);
    MustRecord(s, Event(0.0, NpcEventKind::Injured, kPlayer, 1));
    const Disposition both = s.DispositionOf(Id(1), Id(kPlayer), 0.0).Value();
    CHECK(both.trust == Approx(0.5 - 0.4));
    CHECK(both.fear == Approx(0.5));
    CHECK(both.hostility == Approx(0.5));
    // sums are clamped
    for (int i = 0; i < 5; ++i) MustRecord(s, Event(0.0, NpcEventKind::Betrayed, kPlayer, 1));
    const Disposition bad = s.DispositionOf(Id(1), Id(kPlayer), 0.0).Value();
    CHECK(bad.trust == -1.0);
    CHECK(bad.fear == 1.0);
    CHECK(bad.hostility == 1.0);
    CHECK(s.DispositionOf(Id(99), Id(kPlayer), 0.0).Error().code == NpcMemoryErrorCode::UnknownNpc);
    CHECK(s.DispositionOf(Id(1), Id(kPlayer), std::nan("")).Error().code == NpcMemoryErrorCode::InvalidTime);
}

TEST_CASE("NpcMemory: witnessing counts less than experiencing, and a rumour counts less than first-hand knowledge") {
    NpcMemoryStore s = MakeStore();
    NpcEventInput e = Event(0.0, NpcEventKind::Helped, kPlayer, 1);
    e.witnesses = {Witness{Id(2), true, 1.0}, Witness{Id(3), false, 0.5}};
    MustRecord(s, e);
    const double subject = s.DispositionOf(Id(1), Id(kPlayer), 0.0).Value().trust;
    const double witness = s.DispositionOf(Id(2), Id(kPlayer), 0.0).Value().trust;
    const double rumour = s.DispositionOf(Id(3), Id(kPlayer), 0.0).Value().trust;
    CHECK(subject == Approx(0.5));
    CHECK(witness == Approx(0.5 * 0.3));
    CHECK(rumour == Approx(0.5 * 0.3 * 0.5)); // witness weight x accuracy
    CHECK(s.Memories(Id(3))->front().firstHand == false);
}

TEST_CASE("NpcMemory: memories fade with the half-life of the tier, as a pure function of time") {
    NpcMemoryStore s = MakeStore();
    MustRecord(s, Event(0.0, NpcEventKind::Helped, kPlayer, 1)); // tier A: half-life 864000 s
    MustRecord(s, Event(0.0, NpcEventKind::Helped, kPlayer, 3)); // tier C: half-life 21600 s
    const double halfA = s.Config().halfLifeSeconds[0];
    const double halfC = s.Config().halfLifeSeconds[2];
    CHECK(s.DispositionOf(Id(1), Id(kPlayer), halfA).Value().trust == Approx(0.25));
    CHECK(s.DispositionOf(Id(1), Id(kPlayer), 2 * halfA).Value().trust == Approx(0.125));
    CHECK(s.DispositionOf(Id(3), Id(kPlayer), halfC).Value().trust == Approx(0.25));
    CHECK(s.DispositionOf(Id(3), Id(kPlayer), 10 * halfC).Value().trust < 0.001);
    // asking does not change the store, and asking at an earlier time gives the earlier value (no hidden state)
    const std::string before = s.Serialize();
    CHECK(s.DispositionOf(Id(1), Id(kPlayer), 5 * halfA).Value().trust < s.DispositionOf(Id(1), Id(kPlayer), halfA).Value().trust);
    CHECK(s.DispositionOf(Id(1), Id(kPlayer), 0.0).Value().trust == Approx(0.5));
    CHECK(s.Serialize() == before);
}

TEST_CASE("NpcMemory: a full memory drops the least worth keeping, never the grave events, and never exceeds the capacity") {
    NpcMemoryConfig cfg;
    cfg.capacity = {4, 4, 4};
    NpcMemoryStore s(cfg);
    REQUIRE_FALSE(s.AddNpc(Npc(1)).has_value());
    const std::uint64_t injured = MustRecord(s, Event(0.0, NpcEventKind::Injured, kPlayer, 1));
    for (double t = 1.0; t <= 3.0; t += 1.0) MustRecord(s, Event(t, NpcEventKind::Encounter, kPlayer, 1));
    REQUIRE(s.Memories(Id(1))->size() == 4);
    // two more small events: the oldest small ones go, the injury stays
    MustRecord(s, Event(4.0, NpcEventKind::Encounter, kPlayer, 1));
    MustRecord(s, Event(5.0, NpcEventKind::Witnessed, kPlayer, 1));
    REQUIRE(s.Memories(Id(1))->size() == 4);
    const auto& mem = *s.Memories(Id(1));
    CHECK(std::any_of(mem.begin(), mem.end(), [&](const MemoryRecord& m) { return m.sequence == injured; }));
    CHECK(std::is_sorted(mem.begin(), mem.end(), [](const MemoryRecord& a, const MemoryRecord& b) { return a.sequence < b.sequence; }));
    // a flood of small events cannot push the injury out, and the list never grows
    for (int i = 0; i < 500; ++i) MustRecord(s, Event(6.0 + i, NpcEventKind::Encounter, kPlayer, 1));
    CHECK(s.Memories(Id(1))->size() == 4);
    CHECK(std::any_of(s.Memories(Id(1))->begin(), s.Memories(Id(1))->end(), [&](const MemoryRecord& m) { return m.sequence == injured; }));
}

TEST_CASE("NpcMemory: tiers keep different amounts") {
    NpcMemoryStore s = MakeStore();
    for (int i = 0; i < 100; ++i) {
        NpcEventInput e = Event(i, NpcEventKind::Helped, kPlayer, 1);
        e.witnesses = {Witness{Id(2), true, 1.0}, Witness{Id(3), true, 1.0}};
        MustRecord(s, e);
    }
    CHECK(s.Memories(Id(1))->size() == 64);
    CHECK(s.Memories(Id(2))->size() == 16);
    CHECK(s.Memories(Id(3))->size() == 4);
}

TEST_CASE("NpcMemory: advancing time forgets faded memories, idempotently and composably") {
    const auto build = [] {
        NpcMemoryStore s = MakeStore();
        REQUIRE(s.RecordEvent(Event(0.0, NpcEventKind::Helped, kPlayer, 3)).HasValue());
        REQUIRE(s.RecordEvent(Event(0.0, NpcEventKind::Rescued, kPlayer, 1)).HasValue());
        REQUIRE(s.RecordEvent(Event(100000.0, NpcEventKind::Helped, kPlayer, 3)).HasValue());
        return s;
    };
    NpcMemoryStore a = build();
    const double halfC = a.Config().halfLifeSeconds[2];
    CHECK(a.AdvanceTo(1.0) == 0);          // nothing has faded yet
    CHECK(a.Clock() == 100000.0);          // never behind the newest event
    const std::size_t forgotten = a.AdvanceTo(100000.0 + 6 * halfC);
    CHECK(forgotten == 2);                 // both of Carol's memories (0.5^6 < 0.02); Alice's rescue is tier A and still strong
    CHECK(a.Memories(Id(3))->empty());
    CHECK(a.Memories(Id(1))->size() == 1);
    CHECK(a.AdvanceTo(100000.0 + 6 * halfC) == 0); // idempotent
    // composable: stepping in several hops equals one jump
    NpcMemoryStore hops = build();
    for (double t = 200000.0; t < 100000.0 + 6 * halfC; t += 25000.0) (void)hops.AdvanceTo(t);
    (void)hops.AdvanceTo(100000.0 + 6 * halfC);
    NpcMemoryStore jump = build();
    (void)jump.AdvanceTo(100000.0 + 6 * halfC);
    CHECK(hops.SameSavedState(jump));
    // backwards and non-finite times do nothing
    const std::string before = jump.Serialize();
    CHECK(jump.AdvanceTo(5.0) == 0);
    CHECK(jump.AdvanceTo(std::nan("")) == 0);
    CHECK(jump.AdvanceTo(std::numeric_limits<double>::infinity()) == 0);
    CHECK(jump.Serialize() == before);
    // after a long jump an old event is stale
    CHECK(jump.RecordEvent(Event(10.0, NpcEventKind::Encounter, kPlayer, 1)).Error().code == NpcMemoryErrorCode::StaleEvent);
}

TEST_CASE("NpcMemory: memory stays bounded over a century of simulated events") {
    NpcMemoryStore s;
    constexpr std::uint64_t kNpcs = 200;
    for (std::uint64_t i = 1; i <= kNpcs; ++i) REQUIRE_FALSE(s.AddNpc(Npc(i, static_cast<PersistenceTier>(i % 3))).has_value());
    std::uint64_t state = 12345;
    const auto rnd = [&](std::uint64_t n) {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        return (state >> 33) % n;
    };
    const std::size_t bound = (kNpcs / 3 + 1) * 64 + (kNpcs / 3 + 1) * 16 + (kNpcs / 3 + 1) * 4;
    for (int i = 0; i < 20000; ++i) {
        NpcEventInput e = Event(static_cast<double>(i) * 157680.0, static_cast<NpcEventKind>(rnd(7)), kPlayer, 1 + rnd(kNpcs)); // never Killed (kind 7)
        e.witnesses = {Witness{Id(1 + rnd(kNpcs)), true, 1.0}};
        REQUIRE(s.RecordEvent(e).HasValue());
        if (i % 1000 == 0) {
            (void)s.AdvanceTo(e.time);
            CHECK(s.TotalMemories() <= bound);
        }
    }
    CHECK(s.TotalMemories() <= bound);
    CHECK(s.Clock() > 365.0 * 86400.0 * 90.0); // about a century
}

TEST_CASE("NpcMemory: the same events give the same store, whatever the order of the witness list") {
    const auto feed = [](NpcMemoryStore& s, bool reversed) {
        NpcEventInput e = Event(5.0, NpcEventKind::Witnessed, kPlayer, 0);
        e.witnesses = {Witness{Id(1), true, 0.9}, Witness{Id(2), false, 0.4}, Witness{Id(3), true, 0.6}};
        if (reversed) std::reverse(e.witnesses.begin(), e.witnesses.end());
        REQUIRE(s.RecordEvent(e).HasValue());
        REQUIRE(s.RecordEvent(Event(6.0, NpcEventKind::Betrayed, kPlayer, 2)).HasValue());
    };
    NpcMemoryStore a = MakeStore();
    NpcMemoryStore b = MakeStore();
    feed(a, false);
    feed(b, true);
    CHECK(a.SameSavedState(b));
    CHECK(a.Serialize() == b.Serialize());
    CHECK(a.Revision() == b.Revision());
}

TEST_CASE("NpcMemory: save and load round-trips exactly, including identities, deaths and clock") {
    NpcMemoryStore s = MakeStore();
    NpcIdentity named = Npc(10, PersistenceTier::B, "Frau M\xC3\xBCller\t(Nachbarin)");
    named.archetype = *ContentId::Parse("test:npc.neighbour");
    REQUIRE_FALSE(s.AddNpc(named).has_value());
    NpcEventInput e = Event(12.5, NpcEventKind::Rescued, kPlayer, 10, 0.75);
    e.location = Vec2{123.456, -78.9};
    e.witnesses = {Witness{Id(1), false, 0.3333333333333333}};
    MustRecord(s, e);
    MustRecord(s, Event(20.0, NpcEventKind::Killed, 2, 3));
    (void)s.AdvanceTo(30.0);
    const std::string text = s.Serialize();
    const auto loaded = NpcMemoryStore::Deserialize(text);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().Serialize() == text);
    CHECK(loaded.Value().SameSavedState(s));
    CHECK(loaded.Value().Clock() == 30.0);
    CHECK(loaded.Value().Find(Id(10))->label == named.label);
    CHECK(loaded.Value().Find(Id(10))->archetype == named.archetype);
    CHECK(loaded.Value().Memories(Id(10))->front().location == Vec2{123.456, -78.9});
    CHECK(loaded.Value().Memories(Id(1))->front().accuracy == 0.3333333333333333);
    CHECK(loaded.Value().Revision() == 0); // revision is not saved
    // the loaded store keeps working: next sequence continues, rules still apply
    NpcMemoryStore copy = loaded.Value();
    const std::uint64_t next = MustRecord(copy, Event(40.0, NpcEventKind::Helped, kPlayer, 1));
    CHECK(next == MustRecord(s, Event(40.0, NpcEventKind::Helped, kPlayer, 1)));
    CHECK(copy.RecordEvent(Event(41.0, NpcEventKind::Helped, 3, 1)).Error().code == NpcMemoryErrorCode::ActorDead);
    // the empty store round-trips, too
    const NpcMemoryStore empty;
    CHECK(NpcMemoryStore::Deserialize(empty.Serialize()).Value().Serialize() == empty.Serialize());
}

TEST_CASE("NpcMemory: the saved format is stable (golden text)") {
    NpcMemoryStore s;
    REQUIRE_FALSE(s.AddNpc(Npc(1, PersistenceTier::A, "Ann")).has_value());
    REQUIRE_FALSE(s.AddNpc(Npc(2, PersistenceTier::C)).has_value());
    NpcEventInput e = Event(1.5, NpcEventKind::Helped, kPlayer, 1, 0.5);
    e.location = Vec2{10.0, 20.0};
    e.witnesses = {Witness{Id(2), false, 0.25}};
    MustRecord(s, e);
    const std::string golden =
        "SPN-NPCMEM 1\n"
        "clock 1.5\n"
        "nextseq 2\n"
        "npcs 2\n"
        "n 1 A 1 - Ann 1\n"
        "m 1 1.5 helped 9000 1 1 1 0.5 10 20\n"
        "n 2 C 1 - % 1\n"
        "m 1 1.5 helped 9000 1 0 0.25 0.5 10 20\n"
        "end\n";
    CHECK(s.Serialize() == golden);
    const auto loaded = NpcMemoryStore::Deserialize(golden);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().Serialize() == golden);
}

TEST_CASE("NpcMemory: damaged saves are rejected with the reason, never partially loaded") {
    NpcMemoryStore s = MakeStore();
    NpcEventInput e = Event(2.0, NpcEventKind::Injured, kPlayer, 1);
    e.witnesses = {Witness{Id(2), true, 0.5}};
    MustRecord(s, e);
    MustRecord(s, Event(3.0, NpcEventKind::Helped, kPlayer, 1));
    const std::string good = s.Serialize();
    REQUIRE(NpcMemoryStore::Deserialize(good).HasValue());
    const auto code = [](const std::string& text) { return NpcMemoryStore::Deserialize(text).Error().code; };
    const auto replace = [&](std::string_view from, std::string_view to) {
        std::string t = good;
        const auto at = t.find(from);
        REQUIRE(at != std::string::npos);
        t.replace(at, from.size(), to);
        return t;
    };
    CHECK(code("") == NpcMemoryErrorCode::TruncatedData);
    CHECK(code("SPN-OTHER 1\n") == NpcMemoryErrorCode::InvalidHeader);
    CHECK(code(replace("SPN-NPCMEM 1", "SPN-NPCMEM 2")) == NpcMemoryErrorCode::UnsupportedVersion);
    CHECK(code(good + "x\n") == NpcMemoryErrorCode::TrailingData);
    CHECK(code(good.substr(0, good.size() - 1)) == NpcMemoryErrorCode::TruncatedData);
    CHECK(code(replace("npcs 3", "npcs 4")) == NpcMemoryErrorCode::MalformedLine); // the fourth record is the "end" line
    CHECK(code(replace("npcs 3", "npcs 2")) == NpcMemoryErrorCode::MalformedLine);
    CHECK(code(replace("n 2 B", "n 1 B")) == NpcMemoryErrorCode::InvalidOrder);       // ids must ascend
    CHECK(code(replace("helped", "healed")) == NpcMemoryErrorCode::InvalidToken);     // unknown kind
    CHECK(code(replace(" 0.5 ", " 1.5 ")) == NpcMemoryErrorCode::InvalidToken);       // accuracy / intensity above 1
    CHECK(code(replace("clock 3", "clock 2")) == NpcMemoryErrorCode::InvalidToken);   // a memory from the future
    CHECK(code(replace("nextseq 3", "nextseq 2")) == NpcMemoryErrorCode::InvalidToken); // sequence >= next
    CHECK(code(replace("clock 3", "clock 03")) == NpcMemoryErrorCode::InvalidToken);  // non-canonical spelling
    CHECK(code(replace("n 3 C 1", "n 3 D 1")) == NpcMemoryErrorCode::InvalidToken);
    CHECK(code(replace("n 3 C 1", "n 3 C 2")) == NpcMemoryErrorCode::InvalidToken);
    CHECK(code(replace("n 3 C 1 - % 0", "n 3 C 1 - % 5000")) == NpcMemoryErrorCode::InvalidToken); // memory count above the parser limit
    // sequences and times must not go backwards inside one NPC
    std::string swapped = good;
    const auto first = swapped.find("m 1 2 injured");
    const auto second = swapped.find("m 2 3 helped");
    REQUIRE(first != std::string::npos);
    REQUIRE(second != std::string::npos);
    CHECK(code(replace("m 2 3 helped", "m 1 3 helped")) == NpcMemoryErrorCode::InvalidOrder);
}

TEST_CASE("NpcMemory: any single-byte corruption is rejected or re-serializes identically") {
    NpcMemoryStore s = MakeStore();
    NpcEventInput e = Event(2.25, NpcEventKind::Betrayed, kPlayer, 1, 0.5);
    e.location = Vec2{3.5, -4.25};
    e.witnesses = {Witness{Id(2), false, 0.5}, Witness{Id(3), true, 0.75}};
    MustRecord(s, e);
    MustRecord(s, Event(3.0, NpcEventKind::Killed, kPlayer, 3));
    const std::string good = s.Serialize();
    for (std::size_t i = 0; i < good.size(); ++i) {
        for (const char replacement : {'0', '1', '9', 'x', ' ', '\n', '-', '\0'}) {
            if (good[i] == replacement) continue;
            std::string bad = good;
            bad[i] = replacement;
            const auto loaded = NpcMemoryStore::Deserialize(bad);
            if (loaded.HasValue()) {
                INFO("byte " << i << " -> '" << replacement << "'");
                CHECK(loaded.Value().Serialize() == bad); // accepted only if it is canonical and consistent
            }
        }
    }
}

TEST_CASE("NpcMemory: every truncation of a save is rejected") {
    NpcMemoryStore s = MakeStore();
    MustRecord(s, Event(2.0, NpcEventKind::Helped, kPlayer, 1));
    const std::string good = s.Serialize();
    for (std::size_t len = 0; len < good.size(); ++len) {
        INFO("prefix of " << len << " bytes");
        CHECK_FALSE(NpcMemoryStore::Deserialize(good.substr(0, len)).HasValue());
    }
}

TEST_CASE("NpcMemory: a large population saves and loads") {
    NpcMemoryStore s;
    for (std::uint64_t i = 1; i <= 5000; ++i) REQUIRE_FALSE(s.AddNpc(Npc(i, static_cast<PersistenceTier>(i % 3))).has_value());
    for (int k = 0; k < 20000; ++k) {
        NpcEventInput e = Event(static_cast<double>(k), static_cast<NpcEventKind>(k % 7), kPlayer, 1 + static_cast<std::uint64_t>(k) % 5000);
        REQUIRE(s.RecordEvent(e).HasValue());
    }
    const std::string text = s.Serialize();
    const auto loaded = NpcMemoryStore::Deserialize(text);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().Serialize() == text);
    CHECK(loaded.Value().TotalMemories() == s.TotalMemories());
    CHECK(loaded.Value().NpcCount() == 5000);
}

TEST_CASE("NpcMemory: a save with more memories than today's capacity loads and is trimmed by the next memory") {
    NpcMemoryConfig big;
    big.capacity = {50, 50, 50};
    NpcMemoryStore wide(big);
    REQUIRE_FALSE(wide.AddNpc(Npc(1, PersistenceTier::C)).has_value());
    for (int i = 0; i < 50; ++i) REQUIRE(wide.RecordEvent(Event(i, NpcEventKind::Encounter, kPlayer, 1)).HasValue());
    auto narrow = NpcMemoryStore::Deserialize(wide.Serialize()); // default capacity for tier C is 4
    REQUIRE(narrow.HasValue());
    CHECK(narrow.Value().Memories(Id(1))->size() == 50);
    REQUIRE(narrow.Value().RecordEvent(Event(60.0, NpcEventKind::Helped, kPlayer, 1)).HasValue());
    CHECK(narrow.Value().Memories(Id(1))->size() == 4);
}
