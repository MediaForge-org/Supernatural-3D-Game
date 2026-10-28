#include <doctest.h>

#include <spn/core/GameEvents.h>
#include <string>
#include <vector>

using namespace spn::core;

namespace {
GameEvent Ev(GameEventType type, std::string id = "x") {
    GameEvent e;
    e.type = type;
    e.objectId = std::move(id);
    return e;
}
} // namespace

TEST_CASE("listeners run in subscription order, filters apply, sequence numbers are gapless") {
    GameEventBus bus;
    std::vector<std::string> log;
    const auto a = bus.Subscribe([&](const GameEvent& e) { log.push_back("a" + e.objectId); });
    const auto b = bus.Subscribe([&](const GameEvent& e) { log.push_back("b" + e.objectId); }, {GameEventType::DoorStateChanged});
    const auto c = bus.Subscribe([&](const GameEvent& e) { log.push_back("c" + std::to_string(e.sequence)); });
    CHECK(a != GameEventBus::kInvalidHandle);
    CHECK(b != a);
    CHECK(c != b);
    CHECK(bus.Publish(Ev(GameEventType::InteractionAttempted, "1")));
    CHECK(bus.Publish(Ev(GameEventType::DoorStateChanged, "2")));
    CHECK(log == std::vector<std::string>{"a1", "c1", "a2", "b2", "c2"});
    CHECK(bus.PublishedCount() == 2);
    CHECK(bus.ListenerCount() == 3);
}

TEST_CASE("invalid subscriptions and events are rejected without effect") {
    GameEventBus bus;
    CHECK(bus.Subscribe(nullptr) == GameEventBus::kInvalidHandle);
    CHECK(bus.Subscribe([](const GameEvent&) {}, {static_cast<GameEventType>(99)}) == GameEventBus::kInvalidHandle);
    CHECK(bus.ListenerCount() == 0);
    CHECK_FALSE(bus.Unsubscribe(12345));
    CHECK_FALSE(bus.Publish(Ev(static_cast<GameEventType>(200))));
    CHECK(bus.PublishedCount() == 0);
    // a valid and an out-of-range type together: the valid one works
    int hits = 0;
    CHECK(bus.Subscribe([&](const GameEvent&) { ++hits; }, {GameEventType::DoorStateChanged, static_cast<GameEventType>(99)}) != GameEventBus::kInvalidHandle);
    CHECK(bus.Publish(Ev(GameEventType::DoorStateChanged)));
    CHECK(hits == 1);
}

TEST_CASE("unsubscribe is immediate, also inside dispatch, and a handle works once") {
    GameEventBus bus;
    std::vector<std::string> log;
    GameEventBus::Handle second = 0;
    const auto first = bus.Subscribe([&](const GameEvent&) { log.push_back("first"); CHECK(bus.Unsubscribe(second)); });
    second = bus.Subscribe([&](const GameEvent&) { log.push_back("second"); });
    bus.Publish(Ev(GameEventType::InteractionAttempted));
    CHECK(log == std::vector<std::string>{"first"}); // removed before its turn in the same dispatch
    CHECK_FALSE(bus.Unsubscribe(second));
    CHECK(bus.ListenerCount() == 1);
    CHECK(bus.Unsubscribe(first));
    CHECK(bus.ListenerCount() == 0);
}

TEST_CASE("a listener may unsubscribe itself and subscribe a new one; the new one sees only later events") {
    GameEventBus bus;
    int oldHits = 0, newHits = 0;
    GameEventBus::Handle self = 0;
    self = bus.Subscribe([&](const GameEvent&) {
        ++oldHits;
        bus.Unsubscribe(self);
        (void)bus.Subscribe([&](const GameEvent&) { ++newHits; });
    });
    bus.Publish(Ev(GameEventType::InteractionAttempted));
    CHECK(oldHits == 1);
    CHECK(newHits == 0);
    bus.Publish(Ev(GameEventType::InteractionAttempted));
    CHECK(oldHits == 1);
    CHECK(newHits == 1);
}

TEST_CASE("nested publish is queued and delivered after the current event, in order, without recursion") {
    GameEventBus bus;
    std::vector<std::string> log;
    int depth = 0, maxDepth = 0;
    (void)bus.Subscribe([&](const GameEvent& e) {
        ++depth;
        maxDepth = std::max(maxDepth, depth);
        log.push_back("A:" + e.objectId);
        if (e.objectId == "1") {
            CHECK(bus.IsDispatching());
            CHECK(bus.Publish(Ev(GameEventType::InteractionAttempted, "2")));
            CHECK(bus.Publish(Ev(GameEventType::InteractionAttempted, "3")));
        }
        --depth;
    });
    (void)bus.Subscribe([&](const GameEvent& e) { log.push_back("B:" + e.objectId); });
    bus.Publish(Ev(GameEventType::InteractionAttempted, "1"));
    CHECK(log == std::vector<std::string>{"A:1", "B:1", "A:2", "B:2", "A:3", "B:3"});
    CHECK(maxDepth == 1);
    CHECK_FALSE(bus.IsDispatching());
}

TEST_CASE("an event storm is bounded: overflow is dropped and counted, the bus stays usable") {
    GameEventBus bus;
    std::uint64_t seen = 0;
    (void)bus.Subscribe([&](const GameEvent& e) {
        ++seen;
        if (e.objectId == "root") {
            std::uint64_t accepted = 0;
            for (std::size_t i = 0; i < GameEventBus::kMaxQueuedEvents + 50; ++i) accepted += bus.Publish(Ev(GameEventType::InteractionAttempted, "child")) ? 1u : 0u;
            CHECK(accepted == GameEventBus::kMaxQueuedEvents);
        }
    });
    bus.Publish(Ev(GameEventType::InteractionAttempted, "root"));
    CHECK(seen == 1 + GameEventBus::kMaxQueuedEvents);
    CHECK(bus.DroppedCount() == 50);
    seen = 0;
    CHECK(bus.Publish(Ev(GameEventType::InteractionAttempted, "later")));
    CHECK(seen == 1);
}

TEST_CASE("ScopedSubscription unsubscribes on destruction and on reset, and is movable") {
    GameEventBus bus;
    int hits = 0;
    {
        ScopedSubscription scoped(bus, bus.Subscribe([&](const GameEvent&) { ++hits; }));
        ScopedSubscription moved = std::move(scoped);
        bus.Publish(Ev(GameEventType::InteractionAttempted));
        CHECK(hits == 1);
        CHECK(bus.ListenerCount() == 1);
    }
    CHECK(bus.ListenerCount() == 0);
    bus.Publish(Ev(GameEventType::InteractionAttempted));
    CHECK(hits == 1);
    ScopedSubscription again(bus, bus.Subscribe([&](const GameEvent&) { ++hits; }));
    again.Reset();
    again.Reset(); // twice is harmless
    CHECK(bus.ListenerCount() == 0);
}

TEST_CASE("names exist for every event type and status") {
    for (std::size_t i = 0; i < kGameEventTypeCount; ++i) CHECK(ToString(static_cast<GameEventType>(i)) != "unknown event");
    for (int i = 0; i <= static_cast<int>(InteractStatus::InvalidInput); ++i) CHECK(ToString(static_cast<InteractStatus>(i)) != "unknown status");
}
