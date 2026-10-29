#include <cstddef>
#include <cstdint>
#include <doctest.h>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <spn/core/TestDoor.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace spn::core;

// Proposal 0004, increment 1: interaction core and test door (test cases 1-10).

namespace {

ContentId Id(std::string_view text) {
    const auto id = ContentId::Parse(text);
    REQUIRE(id.has_value());
    return *id;
}

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

/// An interaction that must succeed.
InteractionOutcome Use(const InteractionRegistry& registry, WorldFacts& facts,
                       const InteractableRef& target, const InteractionDefaults& defaults = {},
                       std::string_view cause = "test") {
    auto result = registry.Interact(facts, target, InteractionVerb::Use, defaults, cause);
    REQUIRE_MESSAGE(result.HasValue(), "unexpected error: ", ToString(result.Error().code));
    return std::move(result).Value();
}

/// An interaction that must fail.
InteractionError UseFails(const InteractionRegistry& registry, WorldFacts& facts,
                          const InteractableRef& target, const InteractionDefaults& defaults = {}) {
    const auto result = registry.Interact(facts, target, InteractionVerb::Use, defaults, "test");
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

/// The open state as the door handler sees it, via the fact or the default.
bool IsOpen(const WorldFacts& facts, std::string_view id, const InteractionDefaults& defaults) {
    if (const bool* open = facts.FindAs<bool>(TestDoorOpenKey(id))) {
        return *open;
    }
    const auto found = defaults.values.find(kTestDoorOpenState);
    return found != defaults.values.end() && std::get<bool>(found->second);
}

void Set(WorldFacts& facts, const std::string& key, FactValue value) {
    REQUIRE(facts.Apply(SetFact{key, std::move(value)}, "setup").HasValue());
}

} // namespace

// --- D2: stable id rule ---------------------------------------------------------

TEST_CASE("Interactable ids: accepted at 1 and 96 characters") {
    CHECK(IsValidInteractableId("a"));
    CHECK(IsValidInteractableId(std::string(kMaxInteractableIdLength, 'a')));
    CHECK(IsValidInteractableId("test_map.door_01.a"));
    CHECK(IsValidInteractableId("d0"));
    CHECK(kMaxInteractableIdLength == 96);
}

TEST_CASE("Interactable ids: rejected outside the grammar or length") {
    CHECK_FALSE(IsValidInteractableId(""));
    CHECK_FALSE(IsValidInteractableId(std::string(kMaxInteractableIdLength + 1, 'a')));
    CHECK_FALSE(IsValidInteractableId("Door"));
    CHECK_FALSE(IsValidInteractableId("door_A"));
    CHECK_FALSE(IsValidInteractableId("1door"));
    CHECK_FALSE(IsValidInteractableId(".door"));
    CHECK_FALSE(IsValidInteractableId("_door"));
    CHECK_FALSE(IsValidInteractableId("door 1"));
    CHECK_FALSE(IsValidInteractableId("door-1"));
    CHECK_FALSE(IsValidInteractableId("door:1"));
    CHECK_FALSE(IsValidInteractableId("t\xc3\xbcr"));                   // non-ASCII (UTF-8 ü)
    CHECK_FALSE(IsValidInteractableId(std::string_view("do\0or", 5))); // embedded NUL
    CHECK_FALSE(IsValidInteractableId("do\tor"));
    CHECK_FALSE(IsValidInteractableId("door\n"));
}

TEST_CASE("Interactable ids: the longest id with the longest door suffix is a valid fact key") {
    const std::string id(kMaxInteractableIdLength, 'z');
    const std::string longest = TestDoorLockedKey(id);
    CHECK(longest.size() == 120);
    CHECK(longest.size() <= kMaxFactKeyLength);
    CHECK(IsValidFactKey(longest));
    CHECK(IsValidFactKey(TestDoorOpenKey(id)));

    WorldFacts facts;
    const auto registry = MakeRegistry();
    Set(facts, TestDoorLockedKey(id), false);
    const auto outcome = Use(registry, facts, Door(id));
    CHECK(outcome.changes.size() == 1);
    CHECK(facts.FindAs<bool>(TestDoorOpenKey(id)) != nullptr);
}

TEST_CASE("Interact and AvailableVerbs reject invalid ids without touching the store") {
    WorldFacts facts;
    const auto registry = MakeRegistry();
    for (const std::string& bad : {std::string(), std::string(97, 'a'), std::string("Door"),
                                   std::string("door-1")}) {
        const auto error = UseFails(registry, facts, Door(bad));
        CHECK(error.code == InteractionErrorCode::InvalidId);
        CHECK(error.subject == bad);
        const auto verbs = registry.AvailableVerbs(facts, Door(bad), {});
        REQUIRE_FALSE(verbs.HasValue());
        CHECK(verbs.Error().code == InteractionErrorCode::InvalidId);
    }
    CHECK(facts.Revision() == 0);
}

// --- D1: verbs ------------------------------------------------------------------

TEST_CASE("Verbs: exactly one verb exists today (adding one must update this test)") {
    CHECK(kAllInteractionVerbs.size() == 1);
    CHECK(kAllInteractionVerbs[0] == InteractionVerb::Use);
    CHECK(ToString(InteractionVerb::Use) == "use");
    // The value after the last listed verb has no name: the list and enum agree.
    const auto next = static_cast<InteractionVerb>(kAllInteractionVerbs.size());
    CHECK(ToString(next) == "unknown");
}

TEST_CASE("Verbs: a door offers exactly Use, locked or not") {
    WorldFacts facts;
    const auto registry = MakeRegistry();
    for (const bool locked : {false, true}) {
        const auto verbs = registry.AvailableVerbs(facts, Door("door_a"), Defaults(false, locked));
        REQUIRE(verbs.HasValue());
        CHECK(verbs.Value() == std::vector<InteractionVerb>{InteractionVerb::Use});
    }
}

// --- Registry -------------------------------------------------------------------

TEST_CASE("Registry: duplicate kinds and incomplete handlers are rejected") {
    InteractionRegistry registry;
    REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
    CHECK(registry.IsRegistered(TestDoorKind()));

    const auto duplicate = RegisterTestDoor(registry);
    REQUIRE(duplicate.has_value());
    CHECK(duplicate->code == InteractionErrorCode::DuplicateKind);
    CHECK(duplicate->subject == TestDoorKind().Value());

    const auto emptyKind = registry.Register(ContentId{}, MakeTestDoorHandler());
    REQUIRE(emptyKind.has_value());
    CHECK(emptyKind->code == InteractionErrorCode::InvalidRegistration);

    InteractionHandler incomplete = MakeTestDoorHandler();
    incomplete.plan = nullptr;
    const auto missing = registry.Register(Id("test:interactable.other"), incomplete);
    REQUIRE(missing.has_value());
    CHECK(missing->code == InteractionErrorCode::InvalidRegistration);
    CHECK_FALSE(registry.IsRegistered(Id("test:interactable.other")));
}

TEST_CASE("Registry: an unregistered kind is UnknownKind and changes nothing") {
    WorldFacts facts;
    Set(facts, "placeholder.door.door_a.open", false);
    const auto before = facts;
    const auto registry = MakeRegistry();
    const InteractableRef other{Id("mod:interactable.chest"), "chest_a"};

    const auto error = UseFails(registry, facts, other);
    CHECK(error.code == InteractionErrorCode::UnknownKind);
    CHECK(error.subject == "mod:interactable.chest");
    const auto verbs = registry.AvailableVerbs(facts, other, {});
    REQUIRE_FALSE(verbs.HasValue());
    CHECK(verbs.Error().code == InteractionErrorCode::UnknownKind);
    CHECK(facts == before);
}

// --- Test door (cases 1-6) ------------------------------------------------------

TEST_CASE("Door 1+2: Use opens and closes, one journal record per change with the cause") {
    WorldFacts facts;
    const auto registry = MakeRegistry();
    const auto door = Door("door_a");

    const auto opened = Use(registry, facts, door, {}, "player.use");
    REQUIRE(opened.changes.size() == 1);
    CHECK(opened.changes[0].cause == "player.use");
    CHECK(opened.changes[0].sequence == 1);
    CHECK(opened.changes[0].command == FactCommand{SetFact{"placeholder.door.door_a.open", true}});
    CHECK_FALSE(opened.changes[0].previous.has_value());
    CHECK(opened.feedback.IsEmpty());
    CHECK(facts.Revision() == 1);
    CHECK(IsOpen(facts, "door_a", {}));

    const auto closed = Use(registry, facts, door, {}, "player.use");
    REQUIRE(closed.changes.size() == 1);
    CHECK(closed.changes[0].sequence == 2);
    CHECK(closed.changes[0].previous == std::optional<FactValue>(true));
    CHECK(closed.changes[0].current == std::optional<FactValue>(false));
    CHECK(facts.Revision() == 2);
    CHECK_FALSE(IsOpen(facts, "door_a", {}));
    CHECK(facts.Journal().back() == closed.changes[0]);
}

TEST_CASE("Door 3: a locked door refuses with the locked feedback and writes nothing") {
    const auto registry = MakeRegistry();
    const auto door = Door("door_a");

    SUBCASE("locked by authored default") {
        WorldFacts facts;
        const auto error = UseFails(registry, facts, door, Defaults(false, true));
        CHECK(error.code == InteractionErrorCode::Refused);
        CHECK(error.feedback == TestDoorLockedFeedback());
        CHECK(error.feedback.Value() == "placeholder:feedback.door.locked");
        CHECK(facts.Revision() == 0);
    }
    SUBCASE("locked by fact, overriding an unlocked default") {
        WorldFacts facts;
        Set(facts, TestDoorLockedKey("door_a"), true);
        const auto error = UseFails(registry, facts, door, Defaults(false, false));
        CHECK(error.code == InteractionErrorCode::Refused);
        CHECK(facts.Revision() == 1);
        CHECK(facts.Find(TestDoorOpenKey("door_a")) == nullptr);
    }
    SUBCASE("unlocked by fact, overriding a locked default") {
        WorldFacts facts;
        Set(facts, TestDoorLockedKey("door_a"), false);
        CHECK(Use(registry, facts, door, Defaults(false, true)).changes.size() == 1);
        CHECK(IsOpen(facts, "door_a", {}));
    }
}

TEST_CASE("Door 3b: the lock guards opening only - all four open/locked combinations") {
    const auto registry = MakeRegistry();
    const auto door = Door("door_a");
    SUBCASE("closed + unlocked: opens") {
        WorldFacts facts;
        CHECK(Use(registry, facts, door, Defaults(false, false)).changes.size() == 1);
        CHECK(IsOpen(facts, "door_a", Defaults(false, false)));
    }
    SUBCASE("closed + locked: refused with the locked feedback, nothing written") {
        WorldFacts facts;
        const auto error = UseFails(registry, facts, door, Defaults(false, true));
        CHECK(error.code == InteractionErrorCode::Refused);
        CHECK(error.feedback == TestDoorLockedFeedback());
        CHECK(facts.Revision() == 0);
    }
    SUBCASE("open + unlocked: closes") {
        WorldFacts facts;
        CHECK(Use(registry, facts, door, Defaults(true, false)).changes.size() == 1);
        CHECK_FALSE(IsOpen(facts, "door_a", Defaults(true, false)));
    }
    SUBCASE("open + locked: closes, and stays locked, so it cannot be reopened") {
        WorldFacts facts;
        const auto defaults = Defaults(true, true);
        CHECK(Use(registry, facts, door, defaults).changes.size() == 1);
        CHECK_FALSE(IsOpen(facts, "door_a", defaults));
        const auto error = UseFails(registry, facts, door, defaults); // closed + still locked now
        CHECK(error.feedback == TestDoorLockedFeedback());
        CHECK_FALSE(IsOpen(facts, "door_a", defaults));
    }
    SUBCASE("a disabled door is refused in every state, locked or not") {
        for (const bool open : {false, true}) {
            WorldFacts facts;
            REQUIRE(facts.Apply(SetFact{TestDoorDisabledKey("door_a"), true}, "t").HasValue());
            CHECK(UseFails(registry, facts, door, Defaults(open, false)).feedback == TestDoorDisabledFeedback());
        }
    }
}

TEST_CASE("Door 4: absent facts follow the authored defaults, including an open door") {
    WorldFacts facts;
    const auto registry = MakeRegistry();

    CHECK_FALSE(IsOpen(facts, "closed_door", Defaults(false, false)));
    Use(registry, facts, Door("closed_door"), Defaults(false, false));
    CHECK(facts.FindAs<bool>(TestDoorOpenKey("closed_door")) != nullptr);
    CHECK(*facts.FindAs<bool>(TestDoorOpenKey("closed_door")));

    // Authored open: the first Use closes it.
    Use(registry, facts, Door("open_door"), Defaults(true, false));
    CHECK_FALSE(*facts.FindAs<bool>(TestDoorOpenKey("open_door")));

    // No defaults at all: closed and unlocked.
    Use(registry, facts, Door("bare_door"));
    CHECK(*facts.FindAs<bool>(TestDoorOpenKey("bare_door")));
    CHECK(facts.Revision() == 3);
}

TEST_CASE("Door 5: a changed level default affects only doors never touched") {
    WorldFacts facts;
    const auto registry = MakeRegistry();
    const auto closedDefault = Defaults(false, false);
    const auto openDefault = Defaults(true, false);

    // Touched twice: back to closed, but now stored as a fact.
    Use(registry, facts, Door("touched"), closedDefault);
    Use(registry, facts, Door("touched"), closedDefault);

    // The level designer changes the default to "open".
    CHECK_FALSE(IsOpen(facts, "touched", openDefault));
    CHECK(IsOpen(facts, "untouched", openDefault));

    // Use follows the same rule: touched opens, untouched closes.
    Use(registry, facts, Door("touched"), openDefault);
    Use(registry, facts, Door("untouched"), openDefault);
    CHECK(*facts.FindAs<bool>(TestDoorOpenKey("touched")));
    CHECK_FALSE(*facts.FindAs<bool>(TestDoorOpenKey("untouched")));
}

TEST_CASE("Door 6: a wrong fact or default type is StateCorrupt and changes nothing") {
    const auto registry = MakeRegistry();
    const auto door = Door("door_a");

    SUBCASE("open stored as integer") {
        WorldFacts facts;
        Set(facts, TestDoorOpenKey("door_a"), std::int64_t{1});
        const auto before = facts;
        const auto error = UseFails(registry, facts, door);
        CHECK(error.code == InteractionErrorCode::StateCorrupt);
        CHECK(error.subject == TestDoorOpenKey("door_a"));
        CHECK(facts == before);
        const auto verbs = registry.AvailableVerbs(facts, door, {});
        REQUIRE_FALSE(verbs.HasValue());
        CHECK(verbs.Error().code == InteractionErrorCode::StateCorrupt);
    }
    SUBCASE("locked stored as string") {
        WorldFacts facts;
        Set(facts, TestDoorLockedKey("door_a"), std::string("yes"));
        const auto before = facts;
        const auto error = UseFails(registry, facts, door);
        CHECK(error.code == InteractionErrorCode::StateCorrupt);
        CHECK(error.subject == TestDoorLockedKey("door_a"));
        CHECK(facts == before);
    }
    SUBCASE("authored default of the wrong type") {
        WorldFacts facts;
        InteractionDefaults defaults;
        defaults.values.emplace(kTestDoorOpenState, std::int64_t{0});
        const auto error = UseFails(registry, facts, door, defaults);
        CHECK(error.code == InteractionErrorCode::StateCorrupt);
        CHECK(facts.Revision() == 0);
    }
}

// --- Consistency, determinism, atomicity (cases 8-10) -----------------------------

TEST_CASE("Case 8: every offered verb either succeeds or is Refused with feedback") {
    const auto registry = MakeRegistry();
    for (const bool open : {false, true}) {
        for (const bool locked : {false, true}) {
            WorldFacts facts;
            const auto defaults = Defaults(open, locked);
            const auto verbs = registry.AvailableVerbs(facts, Door("door_a"), defaults);
            REQUIRE(verbs.HasValue());
            for (const InteractionVerb verb : verbs.Value()) {
                const auto result =
                    registry.Interact(facts, Door("door_a"), verb, defaults, "test");
                // The lock guards opening only: refused exactly when the door is closed and locked.
                const bool refused = locked && !open;
                if (result.HasValue()) {
                    CHECK_FALSE(refused);
                } else {
                    CHECK(refused);
                    CHECK(result.Error().code == InteractionErrorCode::Refused);
                    CHECK_FALSE(result.Error().feedback.IsEmpty());
                }
            }
        }
    }
}

TEST_CASE("Case 9: the same sequence gives equal journals and identical save bytes") {
    const auto registry = MakeRegistry();
    const auto play = [&registry](std::string_view actor) {
        WorldFacts facts;
        const std::string cause = std::string("use.by.") + std::string(actor);
        Use(registry, facts, Door("door_a"), {}, cause);
        Use(registry, facts, Door("door_b"), Defaults(true, false), cause);
        Use(registry, facts, Door("door_a"), {}, cause);
        static_cast<void>(registry.Interact(facts, Door("door_c"), InteractionVerb::Use,
                                            Defaults(false, true), cause));
        return facts;
    };

    const WorldFacts first = play("player_1");
    const WorldFacts second = play("player_1");
    CHECK(first == second);
    CHECK(first.Serialize() == second.Serialize());
    CHECK(first.Revision() == 3);

    // The actor only shows up in the cause, never in the resulting state.
    const WorldFacts other = play("player_2");
    CHECK(other.Facts() == first.Facts());
    CHECK(other.Serialize() != first.Serialize());
}

TEST_CASE("Case 10: refused, unknown and failing interactions never partially modify") {
    const auto testKind = Id("test:interactable.multi");
    InteractionRegistry registry = MakeRegistry();
    InteractionHandler multi;
    multi.availableVerbs = [](const WorldFacts&, const InteractableRef&,
                              const InteractionDefaults&)
        -> Result<std::vector<InteractionVerb>, InteractionError> {
        return std::vector<InteractionVerb>{InteractionVerb::Use};
    };
    // Two valid commands, then one WorldFacts rejects (type change of an existing key).
    multi.plan = [](const WorldFacts&, const InteractableRef&, InteractionVerb,
                    const InteractionDefaults&) -> Result<InteractionPlan, InteractionError> {
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{"test.multi.a", true});
        plan.commands.emplace_back(SetFact{"test.multi.b", std::int64_t{1}});
        plan.commands.emplace_back(SetFact{"test.multi.a", std::int64_t{2}});
        return plan;
    };
    REQUIRE_FALSE(registry.Register(testKind, multi).has_value());

    WorldFacts facts;
    Set(facts, TestDoorOpenKey("door_a"), true);
    const WorldFacts before = facts;

    const auto failed = UseFails(registry, facts, InteractableRef{testKind, "multi_a"});
    CHECK(failed.code == InteractionErrorCode::Fact);
    REQUIRE(failed.fact.has_value());
    CHECK(failed.fact->code == FactErrorCode::TypeMismatch);
    CHECK(facts == before);

    // door_a is open in the facts: a locked-by-default OPEN door may be closed, so the refused case needs a closed door
    CHECK(UseFails(registry, facts, Door("door_c"), Defaults(false, true)).code ==
          InteractionErrorCode::Refused);
    Set(facts, TestDoorLockedKey("door_b"), true);
    const WorldFacts lockedBefore = facts;
    CHECK(UseFails(registry, facts, Door("door_b")).code == InteractionErrorCode::Refused);
    CHECK(UseFails(registry, facts, InteractableRef{Id("mod:x.y"), "a"}).code ==
          InteractionErrorCode::UnknownKind);
    CHECK(facts == lockedBefore);
}

TEST_CASE("No-op commands from a handler write no journal record") {
    const auto testKind = Id("test:interactable.steady");
    InteractionRegistry registry;
    InteractionHandler steady;
    steady.availableVerbs = [](const WorldFacts&, const InteractableRef&,
                               const InteractionDefaults&)
        -> Result<std::vector<InteractionVerb>, InteractionError> {
        return std::vector<InteractionVerb>{InteractionVerb::Use};
    };
    // Re-sets the current value, adjusts by 0, removes an absent fact, then one real change.
    steady.plan = [](const WorldFacts&, const InteractableRef&, InteractionVerb,
                     const InteractionDefaults&) -> Result<InteractionPlan, InteractionError> {
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{"test.steady.flag", true});
        plan.commands.emplace_back(AdjustInt{"test.steady.count", 0});
        plan.commands.emplace_back(RemoveFact{"test.steady.absent"});
        plan.commands.emplace_back(AdjustInt{"test.steady.count", 1});
        return plan;
    };
    REQUIRE_FALSE(registry.Register(testKind, steady).has_value());

    WorldFacts facts;
    Set(facts, "test.steady.flag", true);
    Set(facts, "test.steady.count", std::int64_t{5});
    const auto outcome = Use(registry, facts, InteractableRef{testKind, "steady_a"});
    REQUIRE(outcome.changes.size() == 1);
    CHECK(outcome.changes[0].command == FactCommand{AdjustInt{"test.steady.count", 1}});
    CHECK(facts.Revision() == 3);
    CHECK(*facts.FindAs<std::int64_t>("test.steady.count") == 6);

    // A plan made only of no-ops succeeds with no changes at all.
    InteractionRegistry noopRegistry;
    InteractionHandler noop = steady;
    noop.plan = [](const WorldFacts&, const InteractableRef&, InteractionVerb,
                   const InteractionDefaults&) -> Result<InteractionPlan, InteractionError> {
        InteractionPlan plan;
        plan.commands.emplace_back(SetFact{"test.steady.flag", true});
        return plan;
    };
    REQUIRE_FALSE(noopRegistry.Register(testKind, noop).has_value());
    const auto unchanged = Use(noopRegistry, facts, InteractableRef{testKind, "steady_a"});
    CHECK(unchanged.changes.empty());
    CHECK(facts.Revision() == 3);
}

TEST_CASE("Door state round-trips through the unchanged WorldFacts v1 format") {
    // Minimal WorldFacts link check; the full persistence cases 11-14 are increment 2.
    WorldFacts facts;
    const auto registry = MakeRegistry();
    Use(registry, facts, Door("door_a"));
    const auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == facts);
    CHECK(IsOpen(loaded.Value(), "door_a", {}));
    CHECK(facts.Serialize().starts_with("SPNFACTS 1\n"));
}
