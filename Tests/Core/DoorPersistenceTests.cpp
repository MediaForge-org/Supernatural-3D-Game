#include <cstddef>
#include <cstdint>
#include <doctest.h>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <spn/core/TestDoor.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace spn::core;

// Proposal 0004, increment 2: door persistence through the unchanged WorldFacts v1
// format (test cases 11-14). Generic format damage (headers, every byte, golden file)
// is covered in WorldFactsTests.cpp and WorldFactsCompatibilityTests.cpp; these tests
// add the door semantics: registry, authored defaults and interactions after a load.
// No golden fixture for the placeholder door keys: they are not a contract yet.

namespace {

InteractableRef Door(std::string id) {
    return InteractableRef{TestDoorKind(), std::move(id)};
}

InteractionRegistry MakeRegistry() {
    InteractionRegistry registry;
    REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
    return registry;
}

InteractionDefaults Defaults(bool open, bool locked) {
    InteractionDefaults defaults;
    defaults.values.emplace(kTestDoorOpenState, open);
    defaults.values.emplace(kTestDoorLockedState, locked);
    return defaults;
}

void Use(const InteractionRegistry& registry, WorldFacts& facts, const InteractableRef& door,
         const InteractionDefaults& defaults = {}) {
    const auto result = registry.Interact(facts, door, InteractionVerb::Use, defaults, "test.use");
    REQUIRE_MESSAGE(result.HasValue(), "unexpected error: ", ToString(result.Error().code));
}

/// The open state the door handler sees: the fact, else the authored default.
bool IsOpen(const WorldFacts& facts, std::string_view id, bool authoredOpen = false) {
    const bool* open = facts.FindAs<bool>(TestDoorOpenKey(id));
    return open != nullptr ? *open : authoredOpen;
}

WorldFacts Load(std::string_view text) {
    auto loaded = WorldFacts::Deserialize(text);
    REQUIRE_MESSAGE(loaded.HasValue(), "load failed: ", ToString(loaded.Error().code));
    return std::move(loaded).Value();
}

/// How a caller replaces its live store: only a fully loaded store is taken over.
std::optional<FactError> LoadInto(WorldFacts& live, std::string_view text) {
    auto loaded = WorldFacts::Deserialize(text);
    if (!loaded) {
        return loaded.Error();
    }
    live = std::move(loaded).Value();
    return std::nullopt;
}

std::vector<std::string> Lines(const std::string& text) {
    std::vector<std::string> lines;
    std::size_t start = 0;
    for (std::size_t end = text.find('\n'); end != std::string::npos;
         end = text.find('\n', start)) {
        lines.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    REQUIRE(start == text.size()); // documents end with '\n'
    return lines;
}

std::string Join(const std::vector<std::string>& lines) {
    std::string text;
    for (const std::string& line : lines) {
        text.append(line).append("\n");
    }
    return text;
}

/// Replaces tab-separated field `index` of `line`.
std::string WithField(const std::string& line, std::size_t index, std::string_view value) {
    std::size_t start = 0;
    for (std::size_t i = 0; i < index; ++i) {
        start = line.find('\t', start);
        REQUIRE(start != std::string::npos);
        ++start;
    }
    const std::size_t end = line.find('\t', start);
    std::string result = line;
    result.replace(start, end == std::string::npos ? std::string::npos : end - start, value);
    return result;
}

// Field indices of a v1 change line: change seq op key arg previous current cause.
constexpr std::size_t kArgField = 4;
constexpr std::size_t kCurrentField = 6;

/// A short session: door_a opened and closed again, door_b (authored open) closed,
/// door_c refused while locked, door_d opened.
WorldFacts PlaySession(const InteractionRegistry& registry) {
    WorldFacts facts;
    Use(registry, facts, Door("door_a"));
    Use(registry, facts, Door("door_b"), Defaults(true, false));
    Use(registry, facts, Door("door_a"));
    const auto refused = registry.Interact(facts, Door("door_c"), InteractionVerb::Use,
                                           Defaults(false, true), "test.use");
    REQUIRE_FALSE(refused.HasValue());
    Use(registry, facts, Door("door_d"));
    return facts;
}

} // namespace

// --- Case 11: save and load -------------------------------------------------------

TEST_CASE("Case 11: an opened, then closed door survives save and load") {
    const auto registry = MakeRegistry();
    WorldFacts facts;

    Use(registry, facts, Door("door_a"));
    const std::string savedOpen = facts.Serialize();
    Use(registry, facts, Door("door_a"));
    const std::string savedClosed = facts.Serialize();

    const WorldFacts loadedOpen = Load(savedOpen);
    CHECK(IsOpen(loadedOpen, "door_a"));
    CHECK(loadedOpen.Revision() == 1);
    CHECK(loadedOpen.Serialize() == savedOpen);

    const WorldFacts loadedClosed = Load(savedClosed);
    CHECK_FALSE(IsOpen(loadedClosed, "door_a"));
    // Closed again, but touched: the fact stays and overrides an "open" default.
    CHECK(loadedClosed.Find(TestDoorOpenKey("door_a")) != nullptr);
    CHECK_FALSE(IsOpen(loadedClosed, "door_a", true));
    CHECK(loadedClosed == facts);
    CHECK(loadedClosed.Journal() == facts.Journal());
    CHECK(loadedClosed.Serialize() == savedClosed);
}

TEST_CASE("Case 11: several independent doors are restored, each with its own state") {
    const auto registry = MakeRegistry();
    const WorldFacts original = PlaySession(registry);
    const WorldFacts loaded = Load(original.Serialize());

    CHECK(loaded == original);
    CHECK_FALSE(IsOpen(loaded, "door_a"));       // opened and closed
    CHECK_FALSE(IsOpen(loaded, "door_b", true)); // authored open, closed by the player
    CHECK(IsOpen(loaded, "door_d"));
    CHECK(loaded.Find(TestDoorOpenKey("door_c")) == nullptr); // refused: nothing stored
    CHECK(loaded.Find(TestDoorLockedKey("door_c")) == nullptr);
    CHECK(loaded.Facts().size() == 3);
    CHECK(loaded.Revision() == 4);

    // Using one loaded door changes only that door.
    WorldFacts next = loaded;
    Use(registry, next, Door("door_d"));
    CHECK_FALSE(IsOpen(next, "door_d"));
    CHECK(next.Facts().at(TestDoorOpenKey("door_a")) == loaded.Facts().at(TestDoorOpenKey("door_a")));
    CHECK(next.Facts().at(TestDoorOpenKey("door_b")) == loaded.Facts().at(TestDoorOpenKey("door_b")));
}

TEST_CASE("Case 11: playing on after a load equals playing on without the load") {
    const auto registry = MakeRegistry();
    WorldFacts continuous = PlaySession(registry);
    WorldFacts reloaded = Load(continuous.Serialize());

    for (WorldFacts* facts : {&continuous, &reloaded}) {
        Use(registry, *facts, Door("door_a"));
        Use(registry, *facts, Door("door_e"), Defaults(true, false));
    }
    CHECK(reloaded == continuous);
    CHECK(reloaded.Serialize() == continuous.Serialize());
    CHECK(reloaded.Journal().back().sequence == 6);
}

TEST_CASE("Case 11: sessions, saves and repeated loads are deterministic") {
    const auto registry = MakeRegistry();
    const std::string first = PlaySession(registry).Serialize();
    const std::string second = PlaySession(registry).Serialize();
    CHECK(first == second);

    const WorldFacts loadA = Load(first);
    const WorldFacts loadB = Load(first);
    CHECK(loadA == loadB);
    CHECK(Load(loadA.Serialize()).Serialize() == first);
}

// --- Case 12: historical door states ----------------------------------------------

TEST_CASE("Case 12: StateAt shows the door states after every recorded change") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    struct Seen {
        bool a;
        bool b;
    };
    std::vector<Seen> expected{{false, true}}; // sequence 0: authored defaults (b open)
    const auto record = [&] {
        expected.push_back({IsOpen(facts, "door_a"), IsOpen(facts, "door_b", true)});
    };

    Use(registry, facts, Door("door_a"));
    record();
    Use(registry, facts, Door("door_b"), Defaults(true, false));
    record();
    Use(registry, facts, Door("door_a"));
    record();
    Use(registry, facts, Door("door_b"), Defaults(true, false));
    record();
    REQUIRE(facts.Revision() == expected.size() - 1);

    // Same answers from the live store and from a loaded copy.
    const WorldFacts loaded = Load(facts.Serialize());
    const std::vector<const WorldFacts*> stores{&facts, &loaded};
    for (const WorldFacts* store : stores) {
        for (std::uint64_t sequence = 0; sequence <= store->Revision(); ++sequence) {
            CAPTURE(sequence);
            const auto past = store->StateAt(sequence);
            REQUIRE(past.HasValue());
            CHECK(past.Value().Revision() == sequence);
            CHECK(IsOpen(past.Value(), "door_a") == expected[sequence].a);
            CHECK(IsOpen(past.Value(), "door_b", true) == expected[sequence].b);
        }
    }
    CHECK(facts.StateAt(facts.Revision() + 1).Error().code == FactErrorCode::SequenceOutOfRange);
}

TEST_CASE("Case 12: interacting on a historical state does not touch the original") {
    const auto registry = MakeRegistry();
    const WorldFacts original = PlaySession(registry);
    const std::string savedBefore = original.Serialize();

    auto past = original.StateAt(1); // only door_a opened
    REQUIRE(past.HasValue());
    WorldFacts branch = std::move(past).Value();
    CHECK(IsOpen(branch, "door_a"));
    Use(registry, branch, Door("door_a"));
    CHECK_FALSE(IsOpen(branch, "door_a"));
    CHECK(branch.Revision() == 2);

    CHECK(original.Serialize() == savedBefore);
    CHECK(Load(branch.Serialize()) == branch);
}

// --- Case 13: unregistered interactable kinds ------------------------------------

TEST_CASE("Case 13: facts of an unregistered kind load, round-trip and stay untouched") {
    // A save written while a (hypothetical) mod kind was registered: its facts live in
    // the same store. In this session the mod is disabled for the save (M9).
    const auto modKind = ContentId::Parse("mod_x:interactable.chest");
    REQUIRE(modKind.has_value());
    const auto registry = MakeRegistry();
    WorldFacts session;
    Use(registry, session, Door("door_a"));
    REQUIRE(session.Apply(SetFact{"mod_x.chest.chest_a.open", true}, "mod_x.use").HasValue());
    REQUIRE(session.Apply(SetFact{"mod_x.chest.chest_a.items", std::int64_t{3}}, "mod_x.use")
                .HasValue());
    const std::string saved = session.Serialize();

    WorldFacts loaded = Load(saved);
    CHECK_FALSE(registry.IsRegistered(*modKind));
    CHECK(loaded.Serialize() == saved);

    // Using the unknown kind fails and changes nothing.
    const InteractableRef chest{*modKind, "chest_a"};
    const auto used = registry.Interact(loaded, chest, InteractionVerb::Use, {}, "test.use");
    REQUIRE_FALSE(used.HasValue());
    CHECK(used.Error().code == InteractionErrorCode::UnknownKind);
    CHECK(registry.AvailableVerbs(loaded, chest, {}).Error().code ==
          InteractionErrorCode::UnknownKind);
    CHECK(loaded.Serialize() == saved);

    // Doors keep working; the mod facts and their history are kept on re-save.
    Use(registry, loaded, Door("door_a"));
    Use(registry, loaded, Door("door_b"));
    CHECK_FALSE(IsOpen(loaded, "door_a"));
    CHECK(IsOpen(loaded, "door_b"));
    CHECK(*loaded.FindAs<bool>("mod_x.chest.chest_a.open"));
    CHECK(*loaded.FindAs<std::int64_t>("mod_x.chest.chest_a.items") == 3);
    const std::string resaved = loaded.Serialize();
    // The new save is the old journal plus two door records (minus the old trailer).
    CHECK(resaved.starts_with(saved.substr(0, saved.rfind("end\t"))));
    CHECK(Load(resaved) == loaded);
}

// --- Case 14: damaged saves -------------------------------------------------------

TEST_CASE("Case 14: a damaged door save is rejected and the live store stays as it was") {
    const auto registry = MakeRegistry();
    const std::string valid = PlaySession(registry).Serialize();
    const std::vector<std::string> lines = Lines(valid);
    REQUIRE(lines.size() == 6); // header, 4 changes, trailer

    // The live store of the caller holds a different, valid game.
    WorldFacts live;
    Use(registry, live, Door("live_door"));
    const WorldFacts liveBefore = live;
    const std::string liveSaved = live.Serialize();

    const auto rejected = [&](const std::string& damaged) {
        const auto error = LoadInto(live, damaged);
        REQUIRE(error.has_value());
        CHECK(live == liveBefore);
        CHECK(live.Serialize() == liveSaved);
        return *error;
    };

    SUBCASE("truncated at every byte") {
        for (std::size_t size = 0; size < valid.size(); ++size) {
            CAPTURE(size);
            rejected(valid.substr(0, size));
        }
    }
    SUBCASE("a missing change line") {
        std::vector<std::string> damaged = lines;
        damaged.erase(damaged.begin() + 2);
        CHECK(rejected(Join(damaged)).code == FactErrorCode::SequenceMismatch);
    }
    SUBCASE("wrong or unknown version") {
        for (const char* header : {"SPNFACTS 0", "SPNFACTS 2", "SPNFACTS 01"}) {
            std::vector<std::string> damaged = lines;
            damaged[0] = header;
            CAPTURE(header);
            const FactErrorCode code = rejected(Join(damaged)).code;
            CHECK((code == FactErrorCode::UnsupportedVersion || code == FactErrorCode::InvalidHeader));
        }
    }
    SUBCASE("tampered door value: recorded result differs from the replay") {
        std::vector<std::string> damaged = lines;
        damaged[1] = WithField(lines[1], kCurrentField, "b:false"); // door_a opened -> "false"
        CHECK(rejected(Join(damaged)) == FactError{FactErrorCode::JournalMismatch, 2});
    }
    SUBCASE("tampered door command: the following record no longer matches") {
        std::vector<std::string> damaged = lines;
        // door_a "opened" rewritten consistently as "set false"; the later close of
        // door_a still claims "previous b:true".
        damaged[1] = WithField(WithField(lines[1], kArgField, "b:false"), kCurrentField, "b:false");
        CHECK(rejected(Join(damaged)) == FactError{FactErrorCode::JournalMismatch, 4});
    }
    SUBCASE("tampered door value type") {
        std::vector<std::string> damaged = lines;
        damaged[1] = WithField(WithField(lines[1], kArgField, "i:1"), kCurrentField, "i:1");
        CHECK(rejected(Join(damaged)).line == 4); // close of door_a: type mismatch on replay
    }
    SUBCASE("a door key broken into an invalid key") {
        std::vector<std::string> damaged = lines;
        std::string line = lines[1];
        line.replace(line.find("door_a"), 6, "Door_A");
        damaged[1] = line;
        CHECK(rejected(Join(damaged)) == FactError{FactErrorCode::InvalidKey, 2});
    }

    // After all rejections the valid save still loads into the live store.
    CHECK_FALSE(LoadInto(live, valid).has_value());
    CHECK(live.Serialize() == valid);
}

TEST_CASE("Case 14: a valid save with a wrongly typed door fact loads but is never used") {
    // The v1 format knows nothing about doors: an integer under a door key is a valid
    // save. The door handler must refuse it as StateCorrupt and write nothing.
    WorldFacts written;
    REQUIRE(written.Apply(SetFact{TestDoorOpenKey("door_a"), std::int64_t{1}}, "external").HasValue());
    const std::string saved = written.Serialize();

    const auto registry = MakeRegistry();
    WorldFacts loaded = Load(saved);
    const auto used = registry.Interact(loaded, Door("door_a"), InteractionVerb::Use, {}, "test.use");
    REQUIRE_FALSE(used.HasValue());
    CHECK(used.Error().code == InteractionErrorCode::StateCorrupt);
    CHECK(used.Error().subject == TestDoorOpenKey("door_a"));
    CHECK(loaded.Serialize() == saved);

    // Other doors in the same save are unaffected.
    Use(registry, loaded, Door("door_b"));
    CHECK(IsOpen(loaded, "door_b"));
}
