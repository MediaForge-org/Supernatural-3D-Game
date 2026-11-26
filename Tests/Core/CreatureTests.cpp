#include <doctest.h>

#include <cmath>
#include <limits>
#include <vector>

#include <spn/core/Creature.h>
#include <spn/core/Difficulty.h>

using namespace spn::core;

namespace creature_tests {

CreatureInput Tick(Vec2 self, double dt = 0.1)
{
    CreatureInput in;
    in.dt = dt;
    in.self = self;
    return in;
}

CreatureInput Heard(Vec2 self, Vec2 at, double strength, double distance, double dt = 0.1)
{
    CreatureInput in = Tick(self, dt);
    in.noise = CreatureNoise{at, strength, distance};
    return in;
}

/// Runs the brain without input for `seconds`, standing at `self`.
CreatureOutput Idle(CreatureBrain& brain, Vec2 self, double seconds)
{
    CreatureOutput out;
    for (double t = 0.0; t < seconds; t += 0.1) {
        out = brain.Update(Tick(self));
    }
    return out;
}

} // namespace creature_tests

using namespace creature_tests;

TEST_CASE("Creature: the default config is valid and broken configs are refused")
{
    CHECK_FALSE(Validate(CreatureConfig{}).has_value());
    CreatureConfig bad;
    bad.catchRange = 3.0; // larger than the contact range
    CHECK(Validate(bad).has_value());
    bad = {};
    bad.stunSeconds = std::numeric_limits<double>::quiet_NaN();
    CHECK(Validate(bad).has_value());
    bad = {};
    bad.closeHearDistance = 40.0;
    CHECK(Validate(bad).has_value());
}

TEST_CASE("Creature: without any input it patrols forever")
{
    CreatureBrain brain;
    const CreatureOutput out = Idle(brain, {}, 120.0);
    CHECK(out.state == CreatureState::Patrol);
    CHECK_FALSE(out.hasGoal);
    CHECK(brain.TransitionCount() == 0);
}

TEST_CASE("Creature: a quiet noise is investigated, then it goes back to the patrol")
{
    CreatureBrain brain;
    CreatureOutput out = brain.Update(Heard({}, {6, 0}, 0.2, 6.0));
    CHECK(out.state == CreatureState::Investigate);
    CHECK(out.hasGoal);
    CHECK(out.goal == Vec2{6, 0});
    CHECK(out.pace == CreaturePace::Prowl);
    out = Idle(brain, {6, 0}, brain.Config().investigateSeconds + 0.5);
    CHECK(out.state == CreatureState::Patrol);
}

TEST_CASE("Creature: a loud noise starts a pursuit only after the reaction delay (the head start)")
{
    CreatureConfig config;
    config.reactionDelay = 2.0;
    CreatureBrain brain(config);
    CreatureOutput out = brain.Update(Heard({}, {15, 0}, 1.0, 15.0));
    CHECK(out.state == CreatureState::Patrol);
    CHECK(out.listening);
    CHECK(out.pace == CreaturePace::Still);
    out = Idle(brain, {}, 1.5);
    CHECK(out.state == CreatureState::Patrol);
    out = Idle(brain, {}, 0.7);
    CHECK(out.state == CreatureState::Chase);
    CHECK(out.goal == Vec2{15, 0});
    CHECK(out.pace == CreaturePace::Hunt);
}

TEST_CASE("Creature: with no reaction delay a shot starts the pursuit on the next tick")
{
    CreatureConfig config;
    config.reactionDelay = 0.0;
    CreatureBrain brain(config);
    brain.Update(Heard({}, {10, 0}, 1.0, 10.0));
    const CreatureOutput out = brain.Update(Tick({}));
    CHECK(out.state == CreatureState::Chase);
}

TEST_CASE("Creature: a loud noise far away is only investigated")
{
    CreatureBrain brain;
    const CreatureOutput out = brain.Update(Heard({}, {40, 0}, 1.0, 40.0));
    CHECK(out.state == CreatureState::Investigate);
    CHECK_FALSE(out.listening);
}

TEST_CASE("Creature: while investigating, any noise close by is taken for the prey")
{
    CreatureConfig config;
    config.reactionDelay = 0.0;
    CreatureBrain brain(config);
    brain.Update(Heard({}, {12, 0}, 0.2, 12.0));
    REQUIRE(brain.State() == CreatureState::Investigate);
    brain.Update(Heard({}, {5, 0}, 0.15, 5.0)); // a quiet footstep, but close
    const CreatureOutput out = brain.Update(Tick({}));
    CHECK(out.state == CreatureState::Chase);
}

TEST_CASE("Creature: contact needs no sound and gives no head start")
{
    CreatureConfig config;
    config.reactionDelay = 5.0;
    CreatureBrain brain(config);
    CreatureInput in = Tick({});
    in.preyDistance = 1.2;
    in.preyPosition = {1.2, 0};
    const CreatureOutput out = brain.Update(in);
    CHECK(out.state == CreatureState::Chase);
    CHECK(out.goal == Vec2{1.2, 0});
}

TEST_CASE("Creature: the catch is reported exactly once per pursuit")
{
    CreatureBrain brain;
    CreatureInput in = Tick({});
    in.preyDistance = 0.8;
    in.preyPosition = {0.8, 0};
    brain.Update(in); // contact: chase
    CreatureOutput out = brain.Update(in);
    CHECK(out.caught);
    out = brain.Update(in);
    CHECK_FALSE(out.caught);
}

TEST_CASE("Creature: a quiet player is lost, the search radius grows and the creature ends wary")
{
    CreatureConfig config;
    config.reactionDelay = 0.0;
    config.searchSeconds = 10.0;
    CreatureBrain brain(config);
    brain.Update(Heard({}, {3, 0}, 1.0, 3.0));
    brain.Update(Tick({}));
    REQUIRE(brain.State() == CreatureState::Chase);
    // it reaches the last noise and hears nothing more
    CreatureOutput out = Idle(brain, {3, 0}, config.chaseLoseSeconds + 0.3);
    CHECK(out.state == CreatureState::Search);
    const double early = out.searchRadius;
    out = Idle(brain, {3, 0}, 5.0);
    CHECK(out.state == CreatureState::Search);
    CHECK(out.searchRadius > early);
    CHECK(out.searchRadius <= config.searchRadiusMax);
    out = Idle(brain, {3, 0}, 5.5);
    CHECK(out.state == CreatureState::Patrol);
    CHECK(brain.Wary());
    CHECK(brain.HearingScale() == doctest::Approx(config.waryHearingScale));
    Idle(brain, {3, 0}, config.warySeconds + 1.0);
    CHECK_FALSE(brain.Wary());
}

TEST_CASE("Creature: a pursuit blocked short of the last noise lasts until the give-up time, then it searches")
{
    CreatureConfig config;
    config.reactionDelay = 0.0;
    CreatureBrain brain(config);
    brain.Update(Heard({}, {20, 0}, 1.0, 20.0));
    brain.Update(Tick({}));
    CreatureOutput out = Idle(brain, {}, config.chaseLoseSeconds + 1.0); // stuck far from the target (a closed door)
    CHECK(out.state == CreatureState::Chase);
    out = Idle(brain, {}, config.chaseGiveUpSeconds);
    CHECK(out.state == CreatureState::Search);
}

TEST_CASE("Creature: the bell stuns from any state, and afterwards it searches where it stood")
{
    CreatureConfig config;
    config.reactionDelay = 0.0;
    CreatureBrain brain(config);
    brain.Update(Heard({}, {3, 0}, 1.0, 3.0));
    brain.Update(Tick({}));
    REQUIRE(brain.State() == CreatureState::Chase);
    CHECK(brain.Stun());
    CHECK_FALSE(brain.Stun()); // already stunned, the timer restarts
    CreatureInput in = Heard({2, 0}, {2.5, 0}, 1.0, 0.5);
    in.preyDistance = 0.5; // even contact does nothing while stunned
    in.preyPosition = {2.5, 0};
    CreatureOutput out = brain.Update(in);
    CHECK(out.state == CreatureState::Stunned);
    CHECK_FALSE(out.caught);
    CHECK(out.pace == CreaturePace::Still);
    out = Idle(brain, {2, 0}, config.stunSeconds + 0.2);
    CHECK(out.state == CreatureState::Search);
    CHECK(out.goal == Vec2{2, 0});
}

TEST_CASE("Creature: Reset puts it back on its patrol, not wary")
{
    CreatureConfig config;
    config.reactionDelay = 0.0;
    CreatureBrain brain(config);
    brain.Update(Heard({}, {3, 0}, 1.0, 3.0));
    brain.Update(Tick({}));
    brain.Reset();
    CHECK(brain.State() == CreatureState::Patrol);
    CHECK_FALSE(brain.Wary());
    CHECK(brain.Update(Tick({})).state == CreatureState::Patrol);
}

TEST_CASE("Creature: broken input is ignored")
{
    CreatureBrain brain;
    CreatureInput in = Heard({}, {std::numeric_limits<double>::infinity(), 0}, 1.0, 3.0);
    in.preyDistance = std::numeric_limits<double>::quiet_NaN();
    in.dt = -1.0;
    const CreatureOutput out = brain.Update(in);
    CHECK(out.state == CreatureState::Patrol);
    CHECK_FALSE(out.listening);
}

TEST_CASE("Creature: the bell reaches by distance, walls halve the range")
{
    CHECK(BellReaches(10.0, 15.0, 0));
    CHECK_FALSE(BellReaches(10.0, 15.0, 1));
    CHECK(BellReaches(7.0, 15.0, 1));
    CHECK_FALSE(BellReaches(std::numeric_limits<double>::quiet_NaN(), 15.0, 0));
    CHECK_FALSE(BellReaches(1.0, 0.0, 0));
}

TEST_CASE("Creature: saving is refused only while a creature hunts")
{
    CHECK(SaveAllowed({}));
    CHECK(SaveAllowed({CreatureState::Patrol, CreatureState::Search, CreatureState::Stunned, CreatureState::Investigate}));
    CHECK_FALSE(SaveAllowed({CreatureState::Patrol, CreatureState::Chase}));
}

TEST_CASE("Difficulty: any number of profiles, ids unique and valid")
{
    std::vector<DifficultyProfile> list(10);
    for (std::size_t i = 0; i < list.size(); ++i) {
        list[i].id = "level_" + std::to_string(i);
        list[i].respawnHealth = 1.0 - 0.09 * static_cast<double>(i);
    }
    CHECK_FALSE(ValidateDifficultyList(list).has_value());
    CHECK(FindDifficulty(list, "level_7") == std::optional<std::size_t>(7));
    CHECK_FALSE(FindDifficulty(list, "story").has_value());
    list[3].id = "level_2";
    CHECK(ValidateDifficultyList(list).has_value());
    CHECK(ValidateDifficultyList({}).has_value());
}

TEST_CASE("Difficulty: profile values are range checked")
{
    DifficultyProfile p;
    p.id = "Hard";
    CHECK(Validate(p).has_value()); // upper case is not an id
    p.id = "hard";
    CHECK_FALSE(Validate(p).has_value());
    p.respawnHealth = 0.0;
    CHECK(Validate(p).has_value());
    p.respawnHealth = 0.1;
    p.creatureHearingScale = 9.0;
    CHECK(Validate(p).has_value());
}

TEST_CASE("Difficulty: respawn health is the profile's fraction and never zero")
{
    DifficultyProfile hardest;
    hardest.id = "hardest";
    hardest.respawnHealth = 0.1;
    CHECK(RespawnHealth(hardest, 100.0) == doctest::Approx(10.0));
    hardest.respawnHealth = 0.0; // invalid, but still survivable
    CHECK(RespawnHealth(hardest, 100.0) > 0.0);
    CHECK(RespawnHealth(hardest, std::numeric_limits<double>::infinity()) == 0.0);
}
