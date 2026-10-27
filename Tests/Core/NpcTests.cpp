#include <doctest.h>

#include <limits>

#include <spn/core/Npc.h>

using namespace spn::core;

namespace {
const MovementConfig kMove;
const NpcMoveConfig kNav;
const NpcPerceptionConfig kPerc;
NpcState Step(NpcState s, NpcEvents e) { return NextNpcState(s, e); }
} // namespace

TEST_CASE("goal drives Idle <-> Moving") {
    NpcEvents e;
    CHECK(Step(NpcState::Idle, e) == NpcState::Idle);
    e.hasGoal = true;
    CHECK(Step(NpcState::Idle, e) == NpcState::Moving);
    CHECK(Step(NpcState::Moving, e) == NpcState::Moving);
    e.goalReached = true;
    CHECK(Step(NpcState::Moving, e) == NpcState::Idle);
}

TEST_CASE("interaction runs to completion unless a threat interrupts it") {
    NpcEvents e;
    e.interactionRequested = true;
    CHECK(Step(NpcState::Idle, e) == NpcState::Interacting);
    CHECK(Step(NpcState::Moving, e) == NpcState::Interacting);
    e = NpcEvents{};
    CHECK(Step(NpcState::Interacting, e) == NpcState::Interacting);
    e.interactionFinished = true;
    CHECK(Step(NpcState::Interacting, e) == NpcState::Idle);
    e = NpcEvents{};
    e.threatPerceived = true;
    CHECK(Step(NpcState::Interacting, e) == NpcState::Alert);
}

TEST_CASE("alert persists until the threat is gone, then resumes the goal") {
    NpcEvents e;
    e.threatPerceived = true;
    CHECK(Step(NpcState::Idle, e) == NpcState::Alert);
    e = NpcEvents{};
    CHECK(Step(NpcState::Alert, e) == NpcState::Alert);           // no threat now, but not calm yet
    e.threatGone = true;
    CHECK(Step(NpcState::Alert, e) == NpcState::Idle);
    e.hasGoal = true;
    CHECK(Step(NpcState::Alert, e) == NpcState::Moving);
    e.threatPerceived = true;
    CHECK(Step(NpcState::Alert, e) == NpcState::Alert);
}

TEST_CASE("incapacitated beats everything and only recovery leaves it") {
    NpcEvents e;
    e.incapacitated = true;
    for (const NpcState s : {NpcState::Idle, NpcState::Moving, NpcState::Interacting, NpcState::Alert, NpcState::Incapacitated})
        CHECK(Step(s, e) == NpcState::Incapacitated);
    e = NpcEvents{};
    e.hasGoal = true;
    e.threatPerceived = true;
    CHECK(Step(NpcState::Incapacitated, e) == NpcState::Incapacitated);
    e.recovered = true;
    CHECK(Step(NpcState::Incapacitated, e) == NpcState::Idle);
    e.incapacitated = true;                                         // recovered and hit again in the same tick
    CHECK(Step(NpcState::Incapacitated, e) == NpcState::Incapacitated);
}

TEST_CASE("every state the transition function produces is a legal transition") {
    // Exhaustive over the 8 boolean inputs (256 combinations) and the 5 states.
    for (unsigned s = 0; s < kNpcStateCount; ++s) {
        for (unsigned bits = 0; bits < 256; ++bits) {
            NpcEvents e;
            e.hasGoal = bits & 1;
            e.goalReached = bits & 2;
            e.interactionRequested = bits & 4;
            e.interactionFinished = bits & 8;
            e.threatPerceived = bits & 16;
            e.threatGone = bits & 32;
            e.incapacitated = bits & 64;
            e.recovered = bits & 128;
            const NpcState from = static_cast<NpcState>(s);
            const NpcState to = NextNpcState(from, e);
            CHECK(IsLegalNpcTransition(from, to));
        }
    }
    CHECK_FALSE(IsLegalNpcTransition(NpcState::Incapacitated, NpcState::Moving));
    CHECK_FALSE(IsLegalNpcTransition(NpcState::Interacting, NpcState::Moving));
    CHECK(IsLegalNpcTransition(NpcState::Moving, NpcState::Moving));
    CHECK(std::string(NpcStateName(NpcState::Alert)) == "Alert");
}

TEST_CASE("move intent heads for the goal, slows down near it and stops inside the arrival radius") {
    CHECK_FALSE(Validate(kNav).has_value());
    const auto far = ComputeMoveIntent(kMove, kNav, {0.0, 0.0}, {10.0, 0.0}, Gait::Jog);
    CHECK(far.direction.x == doctest::Approx(1.0));
    CHECK(far.direction.y == doctest::Approx(0.0));
    CHECK(far.targetSpeed == doctest::Approx(3.5));
    CHECK_FALSE(far.arrived);
    const auto mid = ComputeMoveIntent(kMove, kNav, {0.0, 0.0}, {0.0, 1.75}, Gait::Jog);   // half way through the slow-down band
    CHECK(mid.direction.y == doctest::Approx(1.0));
    CHECK(mid.targetSpeed == doctest::Approx(3.5 * (0.3 + 0.7 * 0.5)));
    const auto edge = ComputeMoveIntent(kMove, kNav, {0.0, 0.0}, {0.5001, 0.0}, Gait::Run);
    CHECK(edge.targetSpeed == doctest::Approx(5.5 * 0.3).epsilon(0.01));
    const auto here = ComputeMoveIntent(kMove, kNav, {0.0, 0.0}, {0.5, 0.0}, Gait::Run);
    CHECK(here.arrived);
    CHECK(here.targetSpeed == 0.0);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const auto bad = ComputeMoveIntent(kMove, kNav, {nan, 0.0}, {1.0, 0.0}, Gait::Run);
    CHECK_FALSE(bad.arrived);
    CHECK(bad.targetSpeed == 0.0);
    NpcMoveConfig broken = kNav;
    broken.slowDownRadius = 0.2;
    CHECK(Validate(broken).has_value());
    broken = kNav;
    broken.minSpeedFraction = 0.0;
    CHECK(Validate(broken).has_value());
}

TEST_CASE("perception: view cone, range, line of sight, hearing and the close range") {
    const NpcPoint npc{0.0, 0.0};
    NpcStimulus s{{10.0, 0.0}, 0.0, true};
    CHECK(Perceive(kPerc, npc, 0.0, s) == Perception::Seen);
    CHECK(Perceive(kPerc, npc, 180.0, s) == Perception::None);          // looking away, silent
    CHECK(Perceive(kPerc, npc, 54.0, s) == Perception::Seen);           // 54 < 55 half angle
    CHECK(Perceive(kPerc, npc, 56.0, s) == Perception::None);
    CHECK(Perceive(kPerc, npc, -170.0, NpcStimulus{{10.0, 0.0}, 0.0, true}) == Perception::None);
    CHECK(Perceive(kPerc, npc, 350.0, s) == Perception::Seen);          // 350 wraps to -10
    s.lineOfSightClear = false;
    CHECK(Perceive(kPerc, npc, 0.0, s) == Perception::None);            // a wall in between
    s.position = {25.0, 0.0};
    s.lineOfSightClear = true;
    CHECK(Perceive(kPerc, npc, 0.0, s) == Perception::None);            // beyond sight range
    // Hearing: loud stimuli behind a wall or behind the NPC.
    NpcStimulus loud{{6.0, 0.0}, 1.0, false};
    CHECK(Perceive(kPerc, npc, 180.0, loud) == Perception::Heard);
    loud.position = {9.0, 0.0};
    CHECK(Perceive(kPerc, npc, 180.0, loud) == Perception::None);       // beyond 8 m * 1.0
    loud.loudness = 2.0;
    CHECK(Perceive(kPerc, npc, 180.0, loud) == Perception::Heard);      // 16 m for a loud noise
    // Seen wins over Heard; very close is always noticed.
    CHECK(Perceive(kPerc, npc, 0.0, NpcStimulus{{5.0, 0.0}, 2.0, true}) == Perception::Seen);
    CHECK(Perceive(kPerc, npc, 180.0, NpcStimulus{{1.0, 0.0}, 0.0, false}) == Perception::Seen);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    CHECK(Perceive(kPerc, npc, nan, s) == Perception::None);
}
