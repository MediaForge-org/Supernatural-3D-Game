#include <doctest.h>

#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include <spn/core/GameEvents.h>
#include <spn/core/GameSave.h>
#include <spn/core/NpcReaction.h>

using namespace spn::core;

namespace {

constexpr std::uint64_t kPlayer = 9000;
const EntityId kPlayerId{kPlayer};

NpcPerceptionConfig Perc() { return NpcPerceptionConfig{}; }

// Minimal driver: walks the NPC towards the goal at 1.4 m/s (the engine does this with navigation).
struct Sim {
    NpcReactionBrain brain;
    Vec2 self{0, 0};
    Vec2 home{0, 0};
    double time = 0.0;
    std::vector<NpcReactionState> states; // one entry per transition
    int alarms = 0;
    explicit Sim(NpcReactionConfig c = {}) : brain(c) {}

    ReactionOutput Step(double dt, bool sees = false, Vec2 player = {}, std::optional<HeardNoise> noise = std::nullopt, bool wary = false) {
        ReactionInput in;
        in.dt = dt;
        in.self = self;
        in.home = home;
        in.seesPlayer = sees;
        in.playerPosition = player;
        in.noise = noise;
        in.wary = wary;
        const ReactionOutput out = brain.Update(in);
        time += dt;
        if (out.changed) states.push_back(out.state);
        if (out.raiseAlarm) ++alarms;
        if (out.hasGoal) {
            const double dx = out.goal.x - self.x, dy = out.goal.y - self.y;
            const double d = std::hypot(dx, dy);
            if (d > 1e-9) {
                const double step = std::min(d, 1.4 * dt);
                self.x += dx / d * step;
                self.y += dy / d * step;
            }
        } else if (out.fleeAway) {
            const double dx = self.x - out.threat.x, dy = self.y - out.threat.y;
            const double d = std::max(std::hypot(dx, dy), 1e-9);
            self.x += dx / d * 3.5 * dt;
            self.y += dy / d * 3.5 * dt;
        }
        return out;
    }
    void Run(double seconds, double dt = 1.0 / 60.0) {
        for (double t = 0; t < seconds; t += dt) Step(dt);
    }
};

HeardNoise Suspicious(Vec2 p, double sev = 0.9) { return HeardNoise{p, sev, true}; }
HeardNoise Harmless(Vec2 p) { return HeardNoise{p, 0.3, false}; }

} // namespace

TEST_CASE("noise kinds: door break and impact are suspicious, footsteps and door use are not") {
    CHECK(IsSuspicious(NoiseKind::DoorBreak, 0.1));
    CHECK(IsSuspicious(NoiseKind::Impact, 0.1));
    CHECK_FALSE(IsSuspicious(NoiseKind::Footstep, 0.3));
    CHECK_FALSE(IsSuspicious(NoiseKind::DoorUse, 0.4));
    CHECK(IsSuspicious(NoiseKind::DoorUse, 0.9)); // a very loud harmless sound is suspicious as well
}

TEST_CASE("hearing: range, strength and wall attenuation") {
    const HearingConfig h;
    GameplayNoise n;
    n.position = {0, 0};
    n.range = 20.0;
    n.strength = 1.0;
    n.kind = NoiseKind::DoorBreak;
    CHECK(CanHear(h, n, {19.9, 0}, 0).heard);
    CHECK_FALSE(CanHear(h, n, {20.1, 0}, 0).heard);
    CHECK(CanHear(h, n, {9.9, 0}, 1).heard); // one wall halves the range
    CHECK_FALSE(CanHear(h, n, {10.1, 0}, 1).heard);
    CHECK_FALSE(CanHear(h, n, {5.1, 0}, 2).heard);
    CHECK(CanHear(h, n, {4.9, 0}, 2).heard);
    GameplayNoise silent = n;
    silent.strength = 0.0;
    CHECK_FALSE(CanHear(h, silent, {1, 0}, 0).heard);
    GameplayNoise nan = n;
    nan.position.x = std::numeric_limits<double>::quiet_NaN();
    CHECK_FALSE(CanHear(h, nan, {1, 0}, 0).heard);
    CHECK_FALSE(CanHear(h, n, {1, 0}, -1).heard);
    HearingConfig deaf;
    deaf.sensitivity = 0.0;
    CHECK_FALSE(CanHear(deaf, n, {1, 0}, 0).heard);
}

TEST_CASE("sight needs a clear line, a view cone and range") {
    const NpcPerceptionConfig c = Perc();
    CHECK(CanSee(c, {0, 0}, 0.0, {10, 0}, true));
    CHECK_FALSE(CanSee(c, {0, 0}, 0.0, {10, 0}, false)); // wall in between
    CHECK_FALSE(CanSee(c, {0, 0}, 0.0, {0.5, 0}, false)); // even very close: a wall blocks
    CHECK_FALSE(CanSee(c, {0, 0}, 0.0, {-10, 0}, true));  // behind
    CHECK(CanSee(c, {0, 0}, 0.0, {-1.0, 0}, true));       // very close, clear line: noticed regardless of facing
    CHECK_FALSE(CanSee(c, {0, 0}, 0.0, {c.sightRange + 1, 0}, true));
    CHECK(CanSee(c, {0, 0}, 90.0, {0, 10}, true));
}

TEST_CASE("reaction: a harmless noise is ignored when not wary") {
    Sim s;
    for (int i = 0; i < 100; ++i) {
        s.Step(0.1, false, {}, Harmless({5, 0}));
        s.Step(0.1);
    }
    CHECK(s.brain.State() == NpcReactionState::Idle);
    CHECK(s.brain.TransitionCount() == 0);
}

TEST_CASE("reaction: suspicious noise -> Investigate -> search -> ReturnToRoutine -> Idle") {
    Sim s;
    s.Step(0.016, false, {}, Suspicious({10, 0}));
    CHECK(s.brain.State() == NpcReactionState::Investigate);
    s.Run(60.0);
    REQUIRE(s.states.size() == 3);
    CHECK(s.states[0] == NpcReactionState::Investigate);
    CHECK(s.states[1] == NpcReactionState::ReturnToRoutine);
    CHECK(s.states[2] == NpcReactionState::Idle);
    CHECK(std::hypot(s.self.x, s.self.y) <= 1.0); // back home
    CHECK(s.alarms == 0);
}

TEST_CASE("reaction: seeing the player while investigating -> Alert (alarm once) -> Flee when close -> back to Idle") {
    Sim s;
    s.Step(0.016, false, {}, Suspicious({10, 0}));
    s.Run(2.0);
    // the player stands 4 m from the NPC and is visible
    const Vec2 player{s.self.x + 4.0, s.self.y};
    s.Step(0.016, true, player);
    CHECK(s.brain.State() == NpcReactionState::Alert);
    CHECK(s.alarms == 1);
    for (int i = 0; i < 400 && s.brain.State() != NpcReactionState::Flee; ++i) s.Step(0.016, true, player);
    CHECK(s.brain.State() == NpcReactionState::Flee);
    CHECK(s.alarms == 1); // not raised again
    // the NPC runs; once far away and the player out of sight, it calms down
    for (int i = 0; i < 60 * 40 && s.brain.State() != NpcReactionState::Idle; ++i) s.Step(1.0 / 60.0);
    CHECK(s.brain.State() == NpcReactionState::Idle);
}

TEST_CASE("reaction: sighting the player without suspicion does nothing - wary NPCs react at once") {
    Sim a;
    a.Step(0.016, true, {5, 0});
    CHECK(a.brain.State() == NpcReactionState::Idle);
    Sim b;
    b.Step(0.016, true, {5, 0}, std::nullopt, true);
    CHECK(b.brain.State() == NpcReactionState::Alert);
    CHECK(b.alarms == 1);
    // wary + player close: flees after the short wary grace time (0.5 s), not the normal 1.5 s
    b.Step(0.8, true, {3, 0}, std::nullopt, true);
    CHECK(b.brain.State() == NpcReactionState::Flee);
}

TEST_CASE("reaction: a wary NPC investigates even harmless noises") {
    Sim s;
    s.Step(0.016, false, {}, Harmless({6, 0}), true);
    CHECK(s.brain.State() == NpcReactionState::Investigate);
}

TEST_CASE("reaction: repeated identical noises do not restart or flap") {
    Sim s;
    for (int i = 0; i < 300; ++i) s.Step(0.05, false, {}, Suspicious({10, 0}));
    // one Investigate; no ping-pong. The noise keeps refreshing the target, the episode timeout ends it, a later noise may start a new episode.
    CHECK(s.brain.TransitionCount() <= 6);
}

TEST_CASE("reaction: without new events every state settles in Idle in bounded time") {
    for (int start = 0; start < static_cast<int>(kNpcReactionStateCount); ++start) {
        Sim s;
        s.self = {30, 0};
        s.brain.Restore(static_cast<NpcReactionState>(start), {20, 0});
        s.Run(300.0);
        CHECK(s.brain.State() == NpcReactionState::Idle);
        CHECK(s.brain.TransitionCount() <= 4);
    }
}

TEST_CASE("reaction: lost sight of the player in Alert -> Investigate the last known place, not endless Alert") {
    Sim s;
    s.brain.Restore(NpcReactionState::Alert, {});
    s.Step(0.016, true, {12, 0}); // visible but far: stays alert
    CHECK(s.brain.State() == NpcReactionState::Alert);
    s.Run(10.0);
    CHECK(s.states.size() >= 1);
    CHECK(s.states.front() == NpcReactionState::Investigate);
    s.Run(60.0);
    CHECK(s.brain.State() == NpcReactionState::Idle);
}

TEST_CASE("reaction: the state sequence does not depend on the frame rate") {
    auto run = [](double dt) {
        Sim s;
        s.Step(dt, false, {}, Suspicious({10, 0}));
        s.Run(3.0, dt);
        const Vec2 player{s.self.x + 4.0, s.self.y};
        for (double t = 0; t < 4.0; t += dt) s.Step(dt, true, player);
        s.Run(60.0, dt);
        return s.states;
    };
    const auto a = run(1.0 / 30.0);
    const auto b = run(1.0 / 60.0);
    const auto c = run(1.0 / 144.0);
    CHECK(a == b);
    CHECK(b == c);
    REQUIRE(a.size() >= 4);
    CHECK(a[0] == NpcReactionState::Investigate);
    CHECK(a[1] == NpcReactionState::Alert);
    CHECK(a[2] == NpcReactionState::Flee);
}

TEST_CASE("reaction: invalid input is ignored, config is validated") {
    Sim s;
    ReactionInput in;
    in.dt = std::numeric_limits<double>::quiet_NaN();
    in.noise = Suspicious({1, 1});
    CHECK(s.brain.Update(in).state == NpcReactionState::Idle);
    CHECK_FALSE(Validate(NpcReactionConfig{}).has_value());
    NpcReactionConfig bad;
    bad.safeDistance = bad.fleeTriggerDistance;
    CHECK(Validate(bad).has_value());
    bad = NpcReactionConfig{};
    bad.arrivalRadius = 0.0;
    CHECK(Validate(bad).has_value());
}

TEST_CASE("object entity ids are stable, marked and disjoint from NPC ids") {
    const EntityId a = ObjectEntityId("door.back");
    CHECK(a == ObjectEntityId("door.back"));
    CHECK(a != ObjectEntityId("door.front"));
    CHECK(IsObjectEntity(a));
    CHECK_FALSE(IsObjectEntity(EntityId(42)));
    CHECK_FALSE(IsObjectEntity(UnknownActor()));
    CHECK(a.Value() >= (1ull << 62));
}

namespace {
NpcMemoryStore Store() {
    NpcMemoryStore s;
    NpcIdentity a;
    a.id = EntityId(101);
    a.tier = PersistenceTier::B;
    a.label = "caretaker";
    REQUIRE_FALSE(s.AddNpc(a).has_value());
    NpcIdentity b;
    b.id = EntityId(102);
    b.tier = PersistenceTier::C;
    b.label = "neighbour";
    REQUIRE_FALSE(s.AddNpc(b).has_value());
    return s;
}
Incident DoorBroken(double time, double severity = 0.9) {
    Incident i;
    i.kind = IncidentKind::HeardDoorBroken;
    i.time = time;
    i.where = {12, 3};
    i.severity = severity;
    i.object = ObjectEntityId("placeholder.door.back");
    i.accuracy = 0.8;
    return i;
}
} // namespace

TEST_CASE("memory: a heard door break makes only the witness wary and remembers where") {
    NpcMemoryStore s = Store();
    REQUIRE_FALSE(RememberIncident(s, EntityId(101), kPlayerId, DoorBroken(10.0)).has_value());
    const NpcWariness w1 = AssessWariness(s, EntityId(101), kPlayerId, 11.0);
    CHECK(w1.wary);
    CHECK(w1.brokeInRemembered);
    CHECK_FALSE(w1.playerSeenRemembered); // heard it, did not see who
    REQUIRE(w1.lastIncident.has_value());
    CHECK(w1.lastIncident->x == doctest::Approx(12.0));
    CHECK(w1.lastIncidentTime == doctest::Approx(10.0));
    CHECK_FALSE(w1.recentAlarm);
    const NpcWariness w2 = AssessWariness(s, EntityId(102), kPlayerId, 11.0); // did not hear
    CHECK_FALSE(w2.wary);
    CHECK(w2.suspicion == 0.0);
    CHECK(AssessWariness(s, EntityId(999), kPlayerId, 11.0).suspicion == 0.0); // unknown NPC: neutral, no crash
}

TEST_CASE("memory: seeing the player alone is not suspicious, an alarm is - resume window decides the restored state") {
    NpcMemoryStore s = Store();
    Incident seen;
    seen.kind = IncidentKind::SawPlayer;
    seen.time = 5.0;
    seen.severity = 0.3;
    REQUIRE_FALSE(RememberIncident(s, EntityId(101), kPlayerId, seen).has_value());
    NpcWariness w = AssessWariness(s, EntityId(101), kPlayerId, 6.0);
    CHECK(w.playerSeenRemembered);
    CHECK_FALSE(w.wary);
    Incident alarm;
    alarm.kind = IncidentKind::Alarmed;
    alarm.time = 20.0;
    alarm.where = {7, 7};
    alarm.severity = 0.8;
    REQUIRE_FALSE(RememberIncident(s, EntityId(101), kPlayerId, alarm).has_value());
    w = AssessWariness(s, EntityId(101), kPlayerId, 30.0);
    CHECK(w.wary);
    CHECK(w.recentAlarm);
    const RestoredReaction r = InitialStateFromMemory(w);
    CHECK(r.state == NpcReactionState::Investigate);
    CHECK(r.target.x == doctest::Approx(7.0));
    // much later the alarm is no longer "going on", but the NPC still remembers (the memory only fades slowly)
    w = AssessWariness(s, EntityId(101), kPlayerId, 20.0 + 500.0);
    CHECK_FALSE(w.recentAlarm);
    CHECK(InitialStateFromMemory(w).state == NpcReactionState::Idle);
}

TEST_CASE("memory: repeated incidents stay bounded, timestamps never run backwards") {
    NpcMemoryStore s = Store();
    for (int i = 0; i < 200; ++i) REQUIRE_FALSE(RememberIncident(s, EntityId(102), kPlayerId, DoorBroken(1.0 + i * 0.5)).has_value());
    CHECK(s.Memories(EntityId(102))->size() <= s.Config().capacity[static_cast<std::size_t>(PersistenceTier::C)]);
    // an incident stamped before the store clock is clamped to it (never rejected, never out of order)
    REQUIRE_FALSE(RememberIncident(s, EntityId(102), kPlayerId, DoorBroken(0.0)).has_value());
    const auto& m = *s.Memories(EntityId(102));
    for (std::size_t i = 1; i < m.size(); ++i) CHECK(m[i].time >= m[i - 1].time);
    // an unregistered NPC is an error, not a crash
    CHECK(RememberIncident(s, EntityId(555), kPlayerId, DoorBroken(200.0)).has_value());
}

TEST_CASE("memory: wariness survives a serialisation round trip and a game-save round trip") {
    NpcMemoryStore s = Store();
    REQUIRE_FALSE(RememberIncident(s, EntityId(101), kPlayerId, DoorBroken(10.0)).has_value());
    Incident alarm;
    alarm.kind = IncidentKind::Alarmed;
    alarm.time = 12.0;
    alarm.where = {2, 2};
    alarm.severity = 0.8;
    REQUIRE_FALSE(RememberIncident(s, EntityId(101), kPlayerId, alarm).has_value());
    s.AdvanceTo(13.0);

    auto back = NpcMemoryStore::Deserialize(s.Serialize());
    REQUIRE(back.HasValue());
    CHECK(back.Value().SameSavedState(s));
    const NpcWariness a = AssessWariness(s, EntityId(101), kPlayerId, 14.0);
    const NpcWariness b = AssessWariness(back.Value(), EntityId(101), kPlayerId, 14.0);
    CHECK(a.suspicion == doctest::Approx(b.suspicion));
    CHECK(b.recentAlarm);
    CHECK(b.brokeInRemembered);

    WorldFacts facts;
    GameSaveSources src;
    src.facts = &facts;
    src.npcMemory = &s;
    auto slot = CaptureGameSave(src);
    REQUIRE(slot.HasValue());
    auto parsed = ParseGameSave(slot.Value(), GameLoadContext{});
    REQUIRE(parsed.HasValue());
    REQUIRE(parsed.Value().npcMemory.has_value());
    CHECK(parsed.Value().npcMemory->SameSavedState(s));
}

TEST_CASE("memory: damaged NPC data is rejected as a whole, never half-loaded") {
    NpcMemoryStore s = Store();
    REQUIRE_FALSE(RememberIncident(s, EntityId(101), kPlayerId, DoorBroken(10.0)).has_value());
    std::string text = s.Serialize();
    CHECK_FALSE(NpcMemoryStore::Deserialize(text.substr(0, text.size() / 2)).HasValue());
    CHECK_FALSE(NpcMemoryStore::Deserialize("").HasValue());
    CHECK_FALSE(NpcMemoryStore::Deserialize("garbage").HasValue());
}

TEST_CASE("multiple NPCs react independently to the same noise (hearing differs by walls and distance)") {
    NpcMemoryStore store = Store();
    GameplayNoise n;
    n.position = {0, 0};
    n.range = 18.0;
    n.strength = 1.0;
    n.kind = NoiseKind::DoorBreak;
    const HearingConfig h;
    const Vec2 pos[3] = {{6, 0}, {14, 0}, {30, 0}};
    const int walls[3] = {1, 0, 0};
    bool heard[3];
    for (int i = 0; i < 3; ++i) heard[i] = CanHear(h, n, pos[i], walls[i]).heard;
    CHECK(heard[0]);   // close, one wall (range 9)
    CHECK(heard[1]);   // open line, 14 m
    CHECK_FALSE(heard[2]);
    Sim a, b;
    a.self = pos[0];
    b.self = pos[1];
    for (int i = 0; i < 3; ++i) {
        if (!heard[i]) continue;
        Incident inc = DoorBroken(1.0);
        REQUIRE_FALSE(RememberIncident(store, EntityId(i == 0 ? 101 : 102), kPlayerId, inc).has_value());
    }
    a.Step(0.016, false, {}, Suspicious(n.position));
    b.Step(0.016, false, {}, Suspicious(n.position));
    CHECK(a.brain.State() == NpcReactionState::Investigate);
    CHECK(b.brain.State() == NpcReactionState::Investigate);
    a.Run(20.0);
    b.Run(20.0);
    CHECK(a.brain.TransitionCount() >= 1);
    CHECK(AssessWariness(store, EntityId(101), kPlayerId, 2.0).wary);
    CHECK(AssessWariness(store, EntityId(102), kPlayerId, 2.0).wary);
}

TEST_CASE("game event bus carries gameplay noises") {
    GameEventBus bus;
    GameEvent seen;
    int n = 0;
    ScopedSubscription sub(bus, bus.Subscribe([&](const GameEvent& e) { seen = e; ++n; }, {GameEventType::NoiseEmitted}));
    GameEvent e;
    e.type = GameEventType::NoiseEmitted;
    e.objectId = "placeholder.door.back";
    e.noiseX = 3.0;
    e.noiseY = 4.0;
    e.noiseRange = 18.0;
    e.noiseStrength = 1.0;
    CHECK(bus.Publish(e));
    CHECK(n == 1);
    CHECK(seen.noiseRange == 18.0);
    CHECK(ToString(GameEventType::NoiseEmitted) == "noise emitted");
}
