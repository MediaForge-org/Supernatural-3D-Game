#include <doctest.h>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <spn/core/InteractionReach.h>
#include <spn/core/TestDoor.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <utility>
#include <vector>

using namespace spn::core;

// NightBuild04: door states (closed / open / locked / blocked / broken), key unlock, forcing, persistence and candidate picking.

namespace {

InteractableRef Door(std::string id) { return InteractableRef{TestDoorKind(), std::move(id)}; }

InteractionRegistry MakeRegistry() {
    InteractionRegistry registry;
    REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
    return registry;
}

struct Authored {
    bool open = false;
    bool locked = false;
    bool disabled = false;
    bool breakable = false;
    std::string key;
};

InteractionDefaults Defaults(const Authored& a) {
    InteractionDefaults d;
    d.values.emplace(kTestDoorOpenState, a.open);
    d.values.emplace(kTestDoorLockedState, a.locked);
    d.values.emplace(kTestDoorDisabledState, a.disabled);
    d.values.emplace(kTestDoorBreakableState, a.breakable);
    if (!a.key.empty()) d.values.emplace(kTestDoorKeyState, a.key);
    return d;
}

TestDoorStatus Status(const WorldFacts& facts, const InteractionDefaults& d, const char* id = "d") {
    const auto info = ReadTestDoorInfo(facts, id, d);
    REQUIRE(info.HasValue());
    return info.Value().status;
}

} // namespace

TEST_CASE("Door states: closed -> open -> closed, repeated, with exact status") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto d = Defaults({});
    CHECK(Status(facts, d) == TestDoorStatus::Closed);
    for (int i = 0; i < 25; ++i) {
        REQUIRE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());
        CHECK(Status(facts, d) == ((i % 2 == 0) ? TestDoorStatus::Open : TestDoorStatus::Closed));
    }
    CHECK(Status(facts, d) == TestDoorStatus::Open);   // 25 uses: odd count
}

TEST_CASE("Door states: locked refuses Use without a key and changes nothing") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto d = Defaults({.locked = true, .key = "key.cellar"});
    CHECK(Status(facts, d) == TestDoorStatus::Locked);
    const auto before = facts.Revision();
    const auto result = registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t");
    REQUIRE_FALSE(result.HasValue());
    CHECK(result.Error().code == InteractionErrorCode::Refused);
    CHECK(result.Error().feedback == TestDoorLockedFeedback());
    CHECK(facts.Revision() == before);
}

TEST_CASE("Door states: key unlocks, the door stays shut, the next Use opens it") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto d = Defaults({.locked = true, .key = "key.cellar"});
    REQUIRE_FALSE(SetTestKeyHeld(facts, "key.other", true, "t").has_value());   // a different key does nothing
    CHECK_FALSE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());
    REQUIRE_FALSE(SetTestKeyHeld(facts, "key.cellar", true, "t").has_value());
    const auto unlocked = registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t");
    REQUIRE(unlocked.HasValue());
    CHECK(unlocked.Value().feedback == TestDoorUnlockedFeedback());
    CHECK(Status(facts, d) == TestDoorStatus::Closed);
    REQUIRE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());
    CHECK(Status(facts, d) == TestDoorStatus::Open);
}

TEST_CASE("Door states: an open locked door can be closed, then the lock holds it shut again") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto d = Defaults({.open = true, .locked = true, .key = {}});
    REQUIRE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());   // closes
    CHECK(Status(facts, d) == TestDoorStatus::Locked);
    CHECK_FALSE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());
}

TEST_CASE("Door states: Force breaks a locked breakable door for good") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto d = Defaults({.locked = true, .breakable = true, .key = {}});
    const auto forced = registry.Interact(facts, Door("d"), InteractionVerb::Force, d, "t");
    REQUIRE(forced.HasValue());
    CHECK(forced.Value().feedback == TestDoorBrokenFeedback());
    const auto info = ReadTestDoorInfo(facts, "d", d);
    REQUIRE(info.HasValue());
    CHECK(info.Value().status == TestDoorStatus::Open);
    CHECK(info.Value().broken);
    CHECK_FALSE(info.Value().locked);
    // It closes and opens normally afterwards, but never locks again, and cannot be broken twice.
    REQUIRE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());
    CHECK(Status(facts, d) == TestDoorStatus::Broken);
    REQUIRE(registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t").HasValue());
    const auto again = registry.Interact(facts, Door("d"), InteractionVerb::Force, d, "t");
    REQUIRE_FALSE(again.HasValue());
    CHECK(again.Error().feedback == TestDoorBrokenFeedback());
    // A later lock write does not revive the broken lock.
    REQUIRE_FALSE(SetTestDoorLocked(facts, "d", true, "t").has_value());
    CHECK_FALSE(ReadTestDoorInfo(facts, "d", d).Value().locked);
}

TEST_CASE("Door states: Force is refused on unbreakable, unlocked and open doors without any write") {
    const auto registry = MakeRegistry();
    struct Case { Authored a; const ContentId* feedback; };
    const Case cases[] = {
        {{.locked = true, .breakable = false, .key = {}}, &TestDoorUnbreakableFeedback()},
        {{.locked = false, .breakable = true, .key = {}}, &TestDoorNotLockedFeedback()},
        {{.open = true, .locked = true, .breakable = true, .key = {}}, &TestDoorNotLockedFeedback()},
    };
    for (const auto& c : cases) {
        WorldFacts facts;
        const auto d = Defaults(c.a);
        const auto result = registry.Interact(facts, Door("d"), InteractionVerb::Force, d, "t");
        REQUIRE_FALSE(result.HasValue());
        CHECK(result.Error().feedback == *c.feedback);
        CHECK(facts.Revision() == 0);
    }
}

TEST_CASE("Door states: a disabled (blocked) door refuses both verbs, clearing the block restores it") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto d = Defaults({.locked = true, .disabled = true, .breakable = true, .key = {}});
    CHECK(Status(facts, d) == TestDoorStatus::Blocked);
    for (const auto verb : kAllInteractionVerbs) {
        const auto result = registry.Interact(facts, Door("d"), verb, d, "t");
        REQUIRE_FALSE(result.HasValue());
        CHECK(result.Error().feedback == TestDoorDisabledFeedback());
    }
    CHECK(facts.Revision() == 0);
    REQUIRE_FALSE(SetTestDoorDisabled(facts, "d", false, "t").has_value());
    CHECK(Status(facts, d) == TestDoorStatus::Locked);
}

TEST_CASE("Door states: verbs a kind does not offer are refused (nothing is planned or written)") {
    InteractionRegistry registry;
    WorldFacts facts;
    InteractionHandler only;
    only.availableVerbs = [](const WorldFacts&, const InteractableRef&, const InteractionDefaults&)
        -> Result<std::vector<InteractionVerb>, InteractionError> { return std::vector<InteractionVerb>{InteractionVerb::Use}; };
    only.plan = [](const WorldFacts&, const InteractableRef&, InteractionVerb, const InteractionDefaults&)
        -> Result<InteractionPlan, InteractionError> {
        InteractionPlan p;
        p.commands.emplace_back(SetFact{"placeholder.x.on", true});
        return p;
    };
    const auto kind = *ContentId::Parse("placeholder:interactable.only_use");
    REQUIRE_FALSE(registry.Register(kind, only).has_value());
    const auto result = registry.Interact(facts, InteractableRef{kind, "x"}, InteractionVerb::Force, {}, "t");
    REQUIRE_FALSE(result.HasValue());
    CHECK(result.Error().feedback == UnsupportedVerbFeedback());
    CHECK(facts.Revision() == 0);
}

TEST_CASE("Door states: key, lock, break and open state survive a save and load") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    const auto locked = Defaults({.locked = true, .breakable = true, .key = {}});
    const auto keyed = Defaults({.locked = true, .key = "key.cellar"});
    REQUIRE(registry.Interact(facts, Door("front"), InteractionVerb::Force, locked, "t").HasValue());
    REQUIRE_FALSE(SetTestKeyHeld(facts, "key.cellar", true, "t").has_value());
    REQUIRE(registry.Interact(facts, Door("cellar"), InteractionVerb::Use, keyed, "t").HasValue());   // unlocks only
    const std::string saved = facts.Serialize();
    auto loaded = WorldFacts::Deserialize(saved);
    REQUIRE(loaded.HasValue());
    WorldFacts& restored = loaded.Value();
    CHECK(Status(restored, locked, "front") == TestDoorStatus::Open);
    CHECK(ReadTestDoorInfo(restored, "front", locked).Value().broken);
    CHECK(Status(restored, keyed, "cellar") == TestDoorStatus::Closed);
    CHECK(IsTestKeyHeld(restored, "key.cellar"));
    CHECK(restored.Serialize() == saved);
    // After the load the restored world keeps working.
    REQUIRE(registry.Interact(restored, Door("cellar"), InteractionVerb::Use, keyed, "t").HasValue());
    CHECK(Status(restored, keyed, "cellar") == TestDoorStatus::Open);
}

TEST_CASE("Door states: a wrong-typed or invalid key default is StateCorrupt, not a crash") {
    const auto registry = MakeRegistry();
    WorldFacts facts;
    InteractionDefaults d;
    d.values.emplace(kTestDoorKeyState, true);   // must be a string
    const auto result = registry.Interact(facts, Door("d"), InteractionVerb::Use, d, "t");
    REQUIRE_FALSE(result.HasValue());
    CHECK(result.Error().code == InteractionErrorCode::StateCorrupt);
    InteractionDefaults badKey;
    badKey.values.emplace(kTestDoorKeyState, std::string("Not Valid"));
    CHECK(registry.Interact(facts, Door("d"), InteractionVerb::Use, badKey, "t").Error().code == InteractionErrorCode::StateCorrupt);
}

// --- Candidate priority ----------------------------------------------------------

namespace {
InteractionCandidate Candidate(std::string id, ReachVec3 centre, double radius = 0.3, bool los = true, int priority = 0) {
    InteractionCandidate c;
    c.id = std::move(id);
    c.query.eye = {0, 0, 1.6};
    c.query.viewDirection = {1, 0, 0};
    c.query.targetCentre = centre;
    c.query.targetRadius = radius;
    c.query.lineOfSightClear = los;
    c.priority = priority;
    return c;
}
} // namespace

TEST_CASE("Candidates: the central closer one wins, walls and range and view angle exclude") {
    const ReachConfig config;
    std::vector<InteractionCandidate> list{
        Candidate("side", {1.5, 1.2, 1.2}),            // off to the side
        Candidate("centre", {2.0, 0.0, 1.2}),          // straight ahead, further
        Candidate("behind_wall", {1.0, 0.0, 1.2}, 0.3, false),
        Candidate("far", {9.0, 0.0, 1.2}),
        Candidate("behind_me", {-1.0, 0.0, 1.2}),
    };
    const auto pick = PickInteractionCandidate(config, list);
    REQUIRE(pick.has_value());
    CHECK(list[pick->index].id == "centre");
    list.erase(list.begin() + 1);   // without the centre one the side one is still reachable
    CHECK(list[PickInteractionCandidate(config, list)->index].id == "side");
    list.erase(list.begin());
    CHECK_FALSE(PickInteractionCandidate(config, list).has_value());   // only wall / far / behind remain
}

TEST_CASE("Candidates: equal scores are decided by priority, then by order") {
    const ReachConfig config;
    std::vector<InteractionCandidate> list{
        Candidate("a", {1.5, 0.0, 1.2}, 0.3, true, 0),
        Candidate("b", {1.5, 0.0, 1.2}, 0.3, true, 5),
        Candidate("c", {1.5, 0.0, 1.2}, 0.3, true, 5),
    };
    CHECK(list[PickInteractionCandidate(config, list)->index].id == "b");
}
