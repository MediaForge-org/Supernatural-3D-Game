#include <doctest.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <spn/core/Devices.h>
#include <spn/core/InteractionSystem.h>
#include <spn/core/TestDoor.h>

using namespace spn::core;

namespace {

struct World {
    InteractionRegistry registry;
    WorldFacts facts;
    GameEventBus bus;
    InteractionSystem system;
    std::vector<GameEvent> events;
    ScopedSubscription recorder;

    World() : system((Init(), registry), facts, &bus) {
        recorder = ScopedSubscription(bus, bus.Subscribe([this](const GameEvent& e) { events.push_back(e); }));
    }
    bool Init() {
        REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
        REQUIRE_FALSE(RegisterDevices(registry).has_value());
        return true;
    }
    void Add(const char* id, const ContentId& kind, ReachVec3 centre, int priority = 0, std::int64_t cooldown = 0, InteractionDefaults defaults = {}) {
        InteractableDescriptor d;
        d.id = id;
        d.kind = kind;
        d.centre = centre;
        d.priority = priority;
        d.cooldownMs = cooldown;
        d.defaults = std::move(defaults);
        REQUIRE_FALSE(system.AddInteractable(std::move(d)).has_value());
    }
    InteractionRequest Req(const char* target, std::int64_t now = 0) {
        InteractionRequest r;
        r.actor.eye = {0.0, 0.0, 1.6};
        r.actor.viewDirection = {1.0, 0.0, 0.0};
        r.targetId = target;
        r.nowMs = now;
        return r;
    }
    std::vector<GameEventType> Types() const {
        std::vector<GameEventType> t;
        for (const auto& e : events) t.push_back(e.type);
        return t;
    }
};

} // namespace

TEST_CASE("registration validates ids, kinds, numbers and duplicates") {
    World w;
    InteractableDescriptor d;
    d.id = "door_a";
    d.kind = TestDoorKind();
    d.centre = {1.0, 0.0, 1.6};
    CHECK_FALSE(w.system.AddInteractable(d).has_value());
    CHECK(w.system.AddInteractable(d)->code == SystemErrorCode::DuplicateId);
    InteractableDescriptor bad = d;
    bad.id = "Bad Id";
    CHECK(w.system.AddInteractable(bad)->code == SystemErrorCode::InvalidId);
    bad = d; bad.id = "x1"; bad.kind = *ContentId::Parse("mod:interactable.unknown");
    CHECK(w.system.AddInteractable(bad)->code == SystemErrorCode::UnknownKind);
    bad = d; bad.id = "x2"; bad.centre.x = std::numeric_limits<double>::quiet_NaN();
    CHECK(w.system.AddInteractable(bad)->code == SystemErrorCode::InvalidNumber);
    bad = d; bad.id = "x3"; bad.radius = -1.0;
    CHECK(w.system.AddInteractable(bad)->code == SystemErrorCode::InvalidNumber);
    bad = d; bad.id = "x4"; bad.cooldownMs = -5;
    CHECK(w.system.AddInteractable(bad)->code == SystemErrorCode::InvalidNumber);
    CHECK(w.system.Count() == 1);
    CHECK(w.system.RemoveInteractable("door_a"));
    CHECK_FALSE(w.system.RemoveInteractable("door_a"));
    CHECK_FALSE(w.system.SetEnabled("door_a", false));
}

TEST_CASE("selection is deterministic: priority, then angle, then distance, then id; unreachable targets never win") {
    World w;
    w.Add("far", SwitchKind(), {2.4, 0.0, 1.6});
    w.Add("near_b", SwitchKind(), {1.0, 0.0, 1.6});
    w.Add("near_a", SwitchKind(), {1.0, 0.0, 1.6});
    w.Add("too_far", SwitchKind(), {9.0, 0.0, 1.6}, 99);
    w.Add("behind", SwitchKind(), {-1.0, 0.0, 1.6}, 99);
    ActorView actor;
    actor.eye = {0.0, 0.0, 1.6};
    CHECK(w.system.ReachableTargets(actor) == std::vector<std::string>{"near_a", "near_b", "far"}); // same angle and distance: id decides
    CHECK(*w.system.SelectTarget(actor) == "near_a");
    w.Add("important", SwitchKind(), {2.0, 0.0, 1.6}, 5);
    CHECK(*w.system.SelectTarget(actor) == "important"); // priority beats distance
    // angle: looking slightly toward "side" makes it win over an equal-priority centre target
    w.Add("side", SwitchKind(), {1.0, 0.5, 1.6}, 5);
    actor.viewDirection = {1.0, 0.45, 0.0};
    CHECK(*w.system.SelectTarget(actor) == "side");
    // actor state and invalid views select nothing
    actor.state.incapacitated = true;
    CHECK_FALSE(w.system.SelectTarget(actor).has_value());
    actor.state = {};
    actor.viewDirection = {0.0, 0.0, 0.0};
    CHECK_FALSE(w.system.SelectTarget(actor).has_value());
    actor.viewDirection = {std::numeric_limits<double>::infinity(), 0.0, 0.0};
    CHECK_FALSE(w.system.SelectTarget(actor).has_value());
}

TEST_CASE("line of sight hides a target from selection and blocks the request") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.0, 1.6});
    w.system.SetLineOfSight([](const InteractableDescriptor&) { return false; });
    ActorView actor;
    actor.eye = {0.0, 0.0, 1.6};
    CHECK_FALSE(w.system.SelectTarget(actor).has_value());
    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::Blocked);
}

TEST_CASE("request statuses: invalid input, invalid target, out of range, disabled") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.0, 1.6});
    w.Add("remote", SwitchKind(), {20.0, 0.0, 1.6});
    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::Success);

    CHECK(w.system.Interact(w.Req("Not Valid")).status == InteractStatus::InvalidInput);
    CHECK(w.system.Interact(w.Req("sw", -1)).status == InteractStatus::InvalidInput);
    auto r = w.Req("sw");
    r.actor.actor = "Bad Actor";
    CHECK(w.system.Interact(r).status == InteractStatus::InvalidInput);
    r = w.Req("sw");
    r.actor.eye.x = std::numeric_limits<double>::quiet_NaN();
    CHECK(w.system.Interact(r).status == InteractStatus::InvalidInput);
    r = w.Req("sw");
    r.verb = static_cast<InteractionVerb>(77);
    CHECK(w.system.Interact(r).status == InteractStatus::InvalidInput);

    CHECK(w.system.Interact(w.Req("missing")).status == InteractStatus::InvalidTarget);
    CHECK(w.system.Interact(w.Req("remote")).status == InteractStatus::OutOfRange);
    r = w.Req("remote");
    r.requireReach = false;
    CHECK(w.system.Interact(r).status == InteractStatus::Success); // scripted use ignores the actor position

    REQUIRE(w.system.SetEnabled("sw", false));
    const auto revision = w.facts.Revision();
    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::Disabled);
    CHECK(w.facts.Revision() == revision);

    auto blind = w.Req("sw");
    REQUIRE(w.system.SetEnabled("sw", true));
    blind.actor.state.inDialogue = true;
    const auto res = w.system.Interact(blind);
    CHECK(res.status == InteractStatus::Blocked);
    CHECK(res.feedback == InteractionActorStateFeedback());
}

TEST_CASE("door flow: locked, unlock, open, obstructed close is blocked, close succeeds when free") {
    World w;
    w.Add("front", TestDoorKind(), {1.0, 0.0, 1.0});
    REQUIRE_FALSE(SetTestDoorLocked(w.facts, "front", true, "setup").has_value());
    bool obstructed = false;
    w.system.SetBlockRule(TestDoorKind(), MakeDoorCloseObstructionRule([&](std::string_view id) { return id == "front" && obstructed; }));

    auto locked = w.system.Interact(w.Req("front"));
    CHECK(locked.status == InteractStatus::Locked);
    CHECK(locked.feedback == TestDoorLockedFeedback());
    CHECK(locked.changes.empty());

    REQUIRE_FALSE(SetTestDoorLocked(w.facts, "front", false, "unlock").has_value());
    obstructed = true; // an actor in the swing does not stop OPENING
    CHECK(w.system.Interact(w.Req("front")).status == InteractStatus::Success);
    CHECK(*w.facts.FindAs<bool>(TestDoorOpenKey("front")) == true);

    const auto revision = w.facts.Revision();
    const auto blocked = w.system.Interact(w.Req("front"));
    CHECK(blocked.status == InteractStatus::Blocked);
    CHECK(blocked.feedback == TestDoorBlockedFeedback());
    CHECK(*w.facts.FindAs<bool>(TestDoorOpenKey("front")) == true); // stays open
    CHECK(w.facts.Revision() == revision);

    obstructed = false;
    CHECK(w.system.Interact(w.Req("front")).status == InteractStatus::Success);
    CHECK(*w.facts.FindAs<bool>(TestDoorOpenKey("front")) == false);

    w.system.SetBlockRule(TestDoorKind(), nullptr); // removing the rule removes the blocking
    obstructed = true;
    CHECK(w.system.Interact(w.Req("front")).status == InteractStatus::Success);
    CHECK(w.system.Interact(w.Req("front")).status == InteractStatus::Success);
}

TEST_CASE("obstruction rule ignores corrupt state and missing callbacks; authored open doors can be blocked too") {
    WorldFacts facts;
    const InteractableRef ref{TestDoorKind(), "d"};
    InteractionDefaults none;
    CHECK_FALSE(MakeDoorCloseObstructionRule(nullptr)(facts, ref, none));
    const auto rule = MakeDoorCloseObstructionRule([](std::string_view) { return true; });
    CHECK_FALSE(rule(facts, ref, none)); // closed by default: nothing to block
    InteractionDefaults open;
    open.values["open"] = true;
    CHECK(rule(facts, ref, open)); // authored open, no fact yet: closing it would be blocked
    REQUIRE(facts.Apply(SetFact{TestDoorOpenKey("d"), std::int64_t{1}}).HasValue());
    CHECK_FALSE(rule(facts, ref, open)); // corrupt: the door rules report it, the obstruction rule does not guess
}

TEST_CASE("cooldown, exhausted buttons, repeated requests and clock rules") {
    World w;
    InteractionDefaults twice;
    twice.values["max_presses"] = std::int64_t{2};
    w.Add("btn", ButtonKind(), {1.0, 0.0, 1.6}, 0, 500, twice);
    CHECK(w.system.Interact(w.Req("btn", 1000)).status == InteractStatus::Success);
    auto early = w.system.Interact(w.Req("btn", 1100));
    CHECK(early.status == InteractStatus::Blocked);
    CHECK(early.feedback == InteractionCooldownFeedback());
    CHECK(*w.facts.FindAs<std::int64_t>(ButtonPressesKey("btn")) == 1);
    CHECK(w.system.Interact(w.Req("btn", 1500)).status == InteractStatus::Success); // exactly at the cooldown edge
    const auto exhausted = w.system.Interact(w.Req("btn", 5000));
    CHECK(exhausted.status == InteractStatus::AlreadyInState);
    CHECK(exhausted.feedback == ButtonExhaustedFeedback());
    CHECK(w.system.Interact(w.Req("btn", 10)).status == InteractStatus::InvalidInput); // clock went backwards
    w.system.ResetRuntimeState();
    CHECK(w.system.Interact(w.Req("btn", 10)).status == InteractStatus::AlreadyInState); // bookkeeping forgotten, state still exhausted
}

TEST_CASE("rapid repeated requests on a switch alternate state and journal every change once") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.0, 1.6});
    for (int i = 0; i < 9; ++i) CHECK(w.system.Interact(w.Req("sw", i)).status == InteractStatus::Success);
    CHECK(*w.facts.FindAs<bool>(SwitchOnKey("sw")) == true);
    CHECK(w.facts.Revision() == 9);
}

TEST_CASE("events: attempt, success and state change in order; rejections carry status and feedback") {
    World w;
    w.Add("front", TestDoorKind(), {1.0, 0.0, 1.0});
    w.Add("sw", SwitchKind(), {1.0, 0.5, 1.6});
    REQUIRE(w.system.Interact(w.Req("front")).status == InteractStatus::Success);
    CHECK(w.Types() == std::vector<GameEventType>{GameEventType::InteractionAttempted, GameEventType::InteractionSucceeded, GameEventType::DoorStateChanged});
    CHECK(w.events[2].objectId == "front");
    CHECK(w.events[2].from == 0);
    CHECK(w.events[2].to == 1);
    CHECK(w.events[2].actor == "player.local");
    w.events.clear();
    REQUIRE(w.system.Interact(w.Req("sw")).status == InteractStatus::Success);
    CHECK(w.events.back().type == GameEventType::DeviceStateChanged);
    w.events.clear();
    REQUIRE_FALSE(SetTestDoorLocked(w.facts, "front", true, "t").has_value());
    // door is open and locked now; closing is still allowed (the lock keeps it from OPENING). Lock a closed one instead:
    REQUIRE(w.system.Interact(w.Req("front")).status == InteractStatus::Success);
    w.events.clear();
    const auto locked = w.system.Interact(w.Req("front"));
    CHECK(locked.status == InteractStatus::Locked);
    CHECK(w.Types() == std::vector<GameEventType>{GameEventType::InteractionAttempted, GameEventType::InteractionRejected});
    CHECK(w.events.back().status == InteractStatus::Locked);
    CHECK(w.events.back().feedback == TestDoorLockedFeedback());
    // sequence numbers are gapless across everything
    for (std::size_t i = 1; i < w.events.size(); ++i) CHECK(w.events[i].sequence == w.events[i - 1].sequence + 1);
}

TEST_CASE("a listener that interacts during an event is refused; there is no nested state change") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.0, 1.6});
    std::vector<InteractStatus> nested;
    auto sub = ScopedSubscription(w.bus, w.bus.Subscribe([&](const GameEvent& e) {
        if (e.type == GameEventType::InteractionSucceeded) nested.push_back(w.system.Interact(w.Req("sw")).status);
    }));
    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::Success);
    REQUIRE(nested.size() == 1);
    CHECK(nested[0] == InteractStatus::Blocked);
    CHECK(*w.facts.FindAs<bool>(SwitchOnKey("sw")) == true); // toggled exactly once
    CHECK(w.facts.Revision() == 1);
    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::Success); // the guard was released
}

TEST_CASE("world state problems are reported as InvalidWorldState and change nothing") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.0, 1.6});
    REQUIRE(w.facts.Apply(SetFact{SwitchOnKey("sw"), std::string("on")}).HasValue());
    const auto revision = w.facts.Revision();
    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::InvalidWorldState);
    CHECK(w.facts.Revision() == revision);
}

TEST_CASE("lever positions through the system and events carry the position") {
    World w;
    InteractionDefaults three;
    three.values["positions"] = std::int64_t{3};
    w.Add("lv", LeverKind(), {1.0, 0.0, 1.6}, 0, 0, three);
    for (int i = 0; i < 4; ++i) REQUIRE(w.system.Interact(w.Req("lv", i)).status == InteractStatus::Success);
    CHECK(*w.facts.FindAs<std::int64_t>(LeverPositionKey("lv")) == 1);
    CHECK(w.events.back().from == 0);
    CHECK(w.events.back().to == 1);
}

TEST_CASE("link: a switch unlocks a door; the door then opens; every step is evented and journaled with a cause") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.5, 1.6});
    w.Add("gate", TestDoorKind(), {1.0, -0.5, 1.0});
    REQUIRE_FALSE(SetTestDoorLocked(w.facts, "gate", true, "setup").has_value());
    Link link;
    link.source = "sw";
    link.whenState = 1;
    link.target = "gate";
    link.targetKey = TestDoorLockedKey("gate");
    link.value = false;
    link.otherwise = true; // switching off locks the gate again
    REQUIRE_FALSE(w.system.AddLink(link).has_value());
    CHECK(w.system.Interact(w.Req("gate")).status == InteractStatus::Locked);

    w.events.clear();
    const auto on = w.system.Interact(w.Req("sw"));
    REQUIRE(on.status == InteractStatus::Success);
    CHECK(on.changes.size() == 2);
    CHECK(on.changes[1].cause == "link:sw>gate");
    CHECK(*w.facts.FindAs<bool>(TestDoorLockedKey("gate")) == false);
    CHECK(w.Types() == std::vector<GameEventType>{GameEventType::InteractionAttempted, GameEventType::InteractionSucceeded, GameEventType::DeviceStateChanged,
                                                  GameEventType::LinkedMechanismActivated, GameEventType::DoorStateChanged});
    CHECK(w.events[3].sourceId == "sw");
    CHECK(w.events[3].objectId == "gate");
    CHECK(w.system.Interact(w.Req("gate")).status == InteractStatus::Success);

    CHECK(w.system.Interact(w.Req("sw")).status == InteractStatus::Success); // off -> relocks
    CHECK(*w.facts.FindAs<bool>(TestDoorLockedKey("gate")) == true);
}

TEST_CASE("a link that would not change anything writes no record and no event") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.5, 1.6});
    w.Add("gate", TestDoorKind(), {1.0, -0.5, 1.0});
    Link link{"sw", std::nullopt, "gate", TestDoorLockedKey("gate"), false, std::nullopt};
    REQUIRE_FALSE(w.system.AddLink(link).has_value());
    REQUIRE_FALSE(SetTestDoorLocked(w.facts, "gate", false, "setup").has_value());
    const auto before = w.facts.Revision();
    const auto result = w.system.Interact(w.Req("sw"));
    CHECK(result.changes.size() == 1);
    CHECK(w.facts.Revision() == before + 1);
    for (const auto& e : w.events) CHECK(e.type != GameEventType::LinkedMechanismActivated);
}

TEST_CASE("links chain through objects and a button triggers on every press") {
    World w;
    w.Add("btn", ButtonKind(), {1.0, 0.0, 1.6});
    w.Add("sw", SwitchKind(), {1.0, 0.4, 1.6});
    w.Add("gate", TestDoorKind(), {1.0, -0.4, 1.0});
    REQUIRE_FALSE(SetTestDoorLocked(w.facts, "gate", true, "setup").has_value());
    // every press of the button sets the switch on; the switch being on unlocks the gate
    REQUIRE_FALSE(w.system.AddLink({"btn", std::nullopt, "sw", SwitchOnKey("sw"), true, std::nullopt}).has_value());
    REQUIRE_FALSE(w.system.AddLink({"sw", 1, "gate", TestDoorLockedKey("gate"), false, std::nullopt}).has_value());
    const auto result = w.system.Interact(w.Req("btn"));
    REQUIRE(result.status == InteractStatus::Success);
    CHECK(result.changes.size() == 3);
    CHECK(*w.facts.FindAs<bool>(TestDoorLockedKey("gate")) == false);
}

TEST_CASE("link validation: unknown objects, foreign keys, self links, cycles, duplicates, stateless sources") {
    World w;
    w.Add("a", SwitchKind(), {1.0, 0.0, 1.6});
    w.Add("b", SwitchKind(), {1.0, 0.2, 1.6});
    w.Add("d", TestDoorKind(), {1.0, 0.4, 1.0});
    CHECK(w.system.AddLink({"nope", 1, "d", TestDoorLockedKey("d"), false, std::nullopt})->code == SystemErrorCode::UnknownObject);
    CHECK(w.system.AddLink({"a", 1, "nope", TestDoorLockedKey("d"), false, std::nullopt})->code == SystemErrorCode::UnknownObject);
    CHECK(w.system.AddLink({"a", 1, "d", TestDoorLockedKey("other"), false, std::nullopt})->code == SystemErrorCode::InvalidLink); // writes another object's key
    CHECK(w.system.AddLink({"a", 1, "d", "no key!", false, std::nullopt})->code == SystemErrorCode::InvalidLink);
    CHECK(w.system.AddLink({"a", 1, "a", SwitchOnKey("a"), true, std::nullopt})->code == SystemErrorCode::LinkCycle);
    REQUIRE_FALSE(w.system.AddLink({"a", 1, "b", SwitchOnKey("b"), true, std::nullopt}).has_value());
    CHECK(w.system.AddLink({"a", 0, "b", SwitchOnKey("b"), false, std::nullopt})->code == SystemErrorCode::DuplicateLink);
    CHECK(w.system.AddLink({"b", 1, "a", SwitchOnKey("a"), true, std::nullopt})->code == SystemErrorCode::LinkCycle);
    REQUIRE_FALSE(w.system.AddLink({"b", 1, "d", TestDoorLockedKey("d"), false, std::nullopt}).has_value());
    CHECK(w.system.AddLink({"d", 1, "a", SwitchOnKey("a"), true, std::nullopt})->code == SystemErrorCode::LinkCycle); // a>b>d>a
    CHECK(w.system.LinkCount() == 2);
    // removing an object removes its links
    CHECK(w.system.RemoveInteractable("b"));
    CHECK(w.system.LinkCount() == 0);
}

TEST_CASE("a link whose target fact has another type does nothing and does not corrupt the target") {
    World w;
    w.Add("sw", SwitchKind(), {1.0, 0.0, 1.6});
    w.Add("gate", TestDoorKind(), {1.0, 0.5, 1.0});
    REQUIRE_FALSE(w.system.AddLink({"sw", std::nullopt, "gate", TestDoorLockedKey("gate"), false, std::nullopt}).has_value());
    REQUIRE(w.facts.Apply(SetFact{TestDoorLockedKey("gate"), std::int64_t{7}}).HasValue());
    const auto result = w.system.Interact(w.Req("sw"));
    CHECK(result.status == InteractStatus::Success);
    CHECK(result.changes.size() == 1);
    CHECK(*w.facts.FindAs<std::int64_t>(TestDoorLockedKey("gate")) == 7);
}

TEST_CASE("refusal classification by feedback id") {
    CHECK(ClassifyRefusal(*ContentId::Parse("placeholder:feedback.door.locked")) == InteractStatus::Locked);
    CHECK(ClassifyRefusal(*ContentId::Parse("placeholder:feedback.device.disabled")) == InteractStatus::Disabled);
    CHECK(ClassifyRefusal(*ContentId::Parse("placeholder:feedback.button.exhausted")) == InteractStatus::AlreadyInState);
    CHECK(ClassifyRefusal(*ContentId::Parse("mod:feedback.whatever")) == InteractStatus::Blocked);
    CHECK(ClassifyRefusal(ContentId{}) == InteractStatus::Blocked);
    CHECK(ObjectFactPrefix(TestDoorKind(), "front") == "placeholder.door.front.");
    CHECK(ObjectFactPrefix(TestDoorKind(), "Bad").empty());
    CHECK(ObjectFactPrefix(ContentId{}, "front").empty());
}

TEST_CASE("many interactables: selection stays correct with 2000 objects (smoke, not a benchmark)") {
    World w;
    for (int i = 0; i < 2000; ++i) {
        InteractableDescriptor d;
        d.id = "sw_" + std::to_string(i);
        d.kind = SwitchKind();
        d.centre = {1.0 + (i % 50) * 3.0, (i / 50) * 3.0, 1.6};
        REQUIRE_FALSE(w.system.AddInteractable(std::move(d)).has_value());
    }
    ActorView actor;
    actor.eye = {0.0, 0.0, 1.6};
    CHECK(*w.system.SelectTarget(actor) == "sw_0");
    CHECK(w.system.ReachableTargets(actor).size() == 1);
}

TEST_CASE("broad phase: negative coordinates, cell borders, removal and re-adding keep selection exact; absurd coordinates are rejected") {
    World w;
    // targets around the cell border at x = -4, y = 0 and in negative space; the actor stands at the border
    w.Add("west", SwitchKind(), {-4.5, 0.0, 1.6});
    w.Add("east", SwitchKind(), {-3.5, 0.0, 1.6});
    w.Add("south", SwitchKind(), {-4.0, -1.0, 1.6});
    ActorView actor;
    actor.eye = {-4.0, 0.0, 1.6};
    actor.viewDirection = {-1.0, 0.0, 0.0};
    CHECK(*w.system.SelectTarget(actor) == "west"); // straight ahead
    CHECK(w.system.ReachableTargets(actor).size() == 3);
    actor.viewDirection = {1.0, 0.0, 0.0};
    CHECK(*w.system.SelectTarget(actor) == "east");
    CHECK(w.system.RemoveInteractable("east"));
    CHECK(*w.system.SelectTarget(actor) != "east");
    w.Add("east", SwitchKind(), {-3.5, 0.0, 1.6}); // same id again after removal
    CHECK(*w.system.SelectTarget(actor) == "east");

    // far apart objects never leak into a selection; a big radius is still found from the edge of its reach
    InteractableDescriptor far;
    far.id = "far_away";
    far.kind = SwitchKind();
    far.centre = {5000.0, -7000.0, 1.6};
    REQUIRE_FALSE(w.system.AddInteractable(far).has_value());
    ActorView farActor;
    farActor.eye = {5001.0, -7000.0, 1.6};
    farActor.viewDirection = {-1.0, 0.0, 0.0};
    CHECK(*w.system.SelectTarget(farActor) == "far_away");
    CHECK(w.system.ReachableTargets(actor).size() == 3);

    InteractableDescriptor big;
    big.id = "big";
    big.kind = SwitchKind();
    big.centre = {-4.0, 20.0, 1.6};
    big.radius = 17.0; // surface 3 m from the actor's eye line: within 2.5 m? no -> distance - radius = 3.0 > 2.5
    REQUIRE_FALSE(w.system.AddInteractable(big).has_value());
    actor.viewDirection = {0.0, 1.0, 0.0};
    const auto none = w.system.SelectTarget(actor);
    CHECK((!none.has_value() || *none != "big"));
    w.system.RemoveInteractable("big");
    big.id = "big2";
    big.centre.y = 19.0; // surface 2.0 m away
    REQUIRE_FALSE(w.system.AddInteractable(big).has_value());
    CHECK(*w.system.SelectTarget(actor) == "big2"); // found although its centre is 19 m (several cells) away

    InteractableDescriptor absurd;
    absurd.id = "absurd";
    absurd.kind = SwitchKind();
    absurd.centre = {kMaxWorldCoordinate * 2.0, 0.0, 0.0};
    CHECK(w.system.AddInteractable(absurd)->code == SystemErrorCode::InvalidNumber);
    actor.eye = {1.0e12, 0.0, 0.0};
    CHECK_FALSE(w.system.SelectTarget(actor).has_value()); // an absurd eye position selects nothing and does not overflow the grid keys
}

TEST_CASE("selection through the grid equals a brute-force ranking on a pseudo-random scene") {
    World w;
    std::uint32_t state = 12345;
    const auto next = [&] { state = state * 1664525u + 1013904223u; return static_cast<double>(state >> 8) / static_cast<double>(1u << 24); };
    for (int i = 0; i < 400; ++i) {
        InteractableDescriptor d;
        d.id = "o_" + std::to_string(i);
        d.kind = SwitchKind();
        d.centre = {next() * 40.0 - 20.0, next() * 40.0 - 20.0, 1.0 + next()};
        d.radius = next() * 0.8;
        d.priority = static_cast<int>(next() * 3.0);
        REQUIRE_FALSE(w.system.AddInteractable(d).has_value());
    }
    for (int trial = 0; trial < 60; ++trial) {
        ActorView actor;
        actor.eye = {next() * 40.0 - 20.0, next() * 40.0 - 20.0, 1.6};
        actor.viewDirection = {next() - 0.5, next() - 0.5, 0.0};
        std::vector<std::string> brute;
        // brute force: the same reach test over every object, ordered by the documented key
        struct Row { int priority; double angle; double distance; std::string id; };
        std::vector<Row> rows;
        for (int i = 0; i < 400; ++i) {
            const InteractableDescriptor* d = w.system.Find("o_" + std::to_string(i));
            ReachQuery q;
            q.eye = actor.eye; q.viewDirection = actor.viewDirection; q.targetCentre = d->centre; q.targetRadius = d->radius;
            if (CheckReach(ReachConfig{}, q) != ReachResult::Ok) continue;
            const ReachVec3 to{d->centre.x - actor.eye.x, d->centre.y - actor.eye.y, d->centre.z - actor.eye.z};
            const double dist = std::sqrt(to.x * to.x + to.y * to.y + to.z * to.z);
            const double len = std::sqrt(actor.viewDirection.x * actor.viewDirection.x + actor.viewDirection.y * actor.viewDirection.y);
            const double angle = len < 1e-9 ? 0.0 : std::acos(std::clamp((to.x * actor.viewDirection.x + to.y * actor.viewDirection.y) / (dist * len), -1.0, 1.0));
            rows.push_back({d->priority, angle, dist, d->id});
        }
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
            if (a.priority != b.priority) return a.priority > b.priority;
            if (a.angle != b.angle) return a.angle < b.angle;
            if (a.distance != b.distance) return a.distance < b.distance;
            return a.id < b.id;
        });
        const auto got = w.system.ReachableTargets(actor);
        REQUIRE(got.size() == rows.size());
        for (std::size_t i = 0; i < rows.size(); ++i) CHECK(got[i] == rows[i].id);
    }
}
