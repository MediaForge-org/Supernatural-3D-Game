#include <doctest.h>

#include <limits>

#include <spn/core/Traversal.h>

using namespace spn::core;

namespace {
const MovementConfig kMove;
const TraversalConfig kTrav;
TraversalMove Classify(double h, double depth = 0.4, double speed = 4.0, double headroom = 10.0, bool climbable = false) {
    return ClassifyTraversal(kMove, kTrav, TraversalObstacle{h, depth, headroom, speed, climbable}).move;
}
} // namespace

TEST_CASE("traversal config defaults are valid and Validate rejects broken ones") {
    CHECK_FALSE(Validate(kTrav).has_value());
    TraversalConfig c = kTrav;
    c.vaultMaxHeight = 2.5;
    CHECK(Validate(c).has_value());
    c = kTrav;
    c.mantleMaxHeight = 5.0;
    CHECK(Validate(c).has_value());
    c = kTrav;
    c.standingHeadroom = 0.5;
    CHECK(Validate(c).has_value());
    c = kTrav;
    c.vaultMinSpeed = std::numeric_limits<double>::quiet_NaN();
    CHECK(Validate(c).has_value());
}

TEST_CASE("the map's obstacle heights fall into step / jump / vault-or-mantle classes") {
    CHECK(Classify(0.0) == TraversalMove::None);
    CHECK(Classify(-1.0) == TraversalMove::None);
    CHECK(Classify(0.20) == TraversalMove::StepUp);
    CHECK(Classify(0.25) == TraversalMove::StepUp);
    CHECK(Classify(0.30) == TraversalMove::Jump);
    CHECK(Classify(0.40) == TraversalMove::Jump);
    CHECK(Classify(0.45) == TraversalMove::Vault);  // at the apex a jump no longer counts; thin wall + run-up
    CHECK(Classify(1.00) == TraversalMove::Vault);
    CHECK(Classify(1.50) == TraversalMove::Blocked); // thin, above vault height, ledge too narrow
}

TEST_CASE("thin walls need a run-up to vault, thick blocks are mantled") {
    CHECK(Classify(1.0, 0.4, 1.0) == TraversalMove::Blocked); // thin, too slow, ledge too narrow to stand on
    CHECK(Classify(1.0, 0.4, 2.0) == TraversalMove::Vault);
    CHECK(Classify(1.0, 2.0, 4.0) == TraversalMove::Mantle); // thick: not a vault
    CHECK(Classify(1.0, 2.0, 0.0) == TraversalMove::Mantle); // standing start is fine for a mantle
    CHECK(Classify(1.5, 2.0) == TraversalMove::Mantle);
    CHECK(Classify(1.9, 2.0) == TraversalMove::Mantle);
    CHECK(Classify(2.0, 2.0) == TraversalMove::Blocked);
}

TEST_CASE("head room, climbable surfaces and bad input") {
    CHECK(Classify(1.0, 2.0, 0.0, 1.0) == TraversalMove::Blocked);       // a low ceiling above the ledge
    CHECK(Classify(2.5, 2.0, 1.0, 10.0, true) == TraversalMove::Climb);
    CHECK(Classify(2.5, 2.0, 1.0, 10.0, false) == TraversalMove::Blocked);
    CHECK(Classify(2.5, 2.0, 5.0, 10.0, true) == TraversalMove::Blocked); // no running into a climb
    CHECK(Classify(3.5, 2.0, 1.0, 10.0, true) == TraversalMove::Blocked);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    CHECK(Classify(nan) == TraversalMove::Blocked);
    CHECK(Classify(1.0, nan) == TraversalMove::Blocked);
    CHECK(Classify(1.0, 0.4, nan) == TraversalMove::Blocked);  // NaN speed counts as 0: no run-up
    CHECK(std::string(ClassifyTraversal(kMove, kTrav, TraversalObstacle{2.0, 2.0, 10.0, 0.0, false}).reason) == "too high");
}

TEST_CASE("a move is monotone in height for a fixed ledge") {
    // Never a Vault below a Jump, never a Mantle above Blocked-by-height for the same thick ledge.
    int lastRank = 0;
    for (double h = 0.0; h <= 1.95; h += 0.01) {
        const auto m = Classify(h, 2.0, 4.0);
        int rank = m == TraversalMove::None ? 0 : m == TraversalMove::StepUp ? 1 : m == TraversalMove::Jump ? 2 : 3;
        CHECK(rank >= lastRank);
        lastRank = rank;
    }
}

TEST_CASE("traversal phases run in order and cannot be restarted midway") {
    const TraversalTimings t;
    TraversalState s;
    CHECK(CanStartTraversal(s, TraversalMove::Vault, MovementState::Run));
    CHECK_FALSE(CanStartTraversal(s, TraversalMove::Jump, MovementState::Run));
    CHECK_FALSE(CanStartTraversal(s, TraversalMove::StepUp, MovementState::Run));
    CHECK_FALSE(CanStartTraversal(s, TraversalMove::Mantle, MovementState::Falling));
    CHECK_FALSE(CanStartTraversal(s, TraversalMove::Mantle, MovementState::Landing));
    s = StartTraversal(TraversalMove::Mantle);
    CHECK(s.phase == TraversalPhase::Approach);
    CHECK(TraversalOwnsMovement(s));
    CHECK_FALSE(CanStartTraversal(s, TraversalMove::Vault, MovementState::Run));
    s = AdvanceTraversal(s, t, 0.10);
    CHECK(s.phase == TraversalPhase::Approach);
    s = AdvanceTraversal(s, t, 0.10);
    CHECK(s.phase == TraversalPhase::Active);
    CHECK(TraversalOwnsMovement(s));
    s = AdvanceTraversal(s, t, t.active);
    CHECK(s.phase == TraversalPhase::Recover);
    CHECK_FALSE(TraversalOwnsMovement(s));
    s = AdvanceTraversal(s, t, 10.0); // a long step runs through all remaining phases
    CHECK(s.phase == TraversalPhase::Done);
    CHECK(CanStartTraversal(s, TraversalMove::Vault, MovementState::Jog));
    CHECK(AdvanceTraversal(s, t, 1.0).phase == TraversalPhase::Done);
    CHECK(AdvanceTraversal(TraversalState{}, t, 1.0).phase == TraversalPhase::Idle);
    const auto same = AdvanceTraversal(StartTraversal(TraversalMove::Vault), t, -5.0);
    CHECK(same.phase == TraversalPhase::Approach);
    CHECK(same.phaseSeconds == 0.0);
}

namespace {
TraversalProbe Probe(double h, double depth, double speed) {
    TraversalProbe p;
    p.obstacle = TraversalObstacle{h, depth, 10.0, speed, false};
    return p;
}
TraversalMove Checked(const TraversalProbe& p) { return ClassifyTraversalChecked(kMove, kTrav, TraversalSafety{}, p).move; }
} // namespace

TEST_CASE("safety rules refuse unsafe vaults, mantles and climbs but leave ordinary movement alone") {
    CHECK(Checked(Probe(1.0, 0.4, 4.0)) == TraversalMove::Vault);
    CHECK(Checked(Probe(1.0, 2.0, 0.0)) == TraversalMove::Mantle);
    TraversalProbe p = Probe(1.0, 0.4, 4.0);
    p.pathClear = false;
    CHECK(Checked(p) == TraversalMove::Blocked);                         // a closed wall / blocker in between
    p = Probe(1.0, 0.4, 4.0);
    p.landingClearDistance = 0.5;
    CHECK(Checked(p) == TraversalMove::Blocked);                         // nowhere to land
    p.landingClearDistance = 1.0;
    CHECK(Checked(p) == TraversalMove::Vault);                           // exactly the minimum is fine
    p = Probe(1.0, 2.0, 0.0);
    p.topSurfaceAngleDegrees = 50.0;
    CHECK(Checked(p) == TraversalMove::Blocked);
    p.topSurfaceAngleDegrees = 44.8;
    CHECK(Checked(p) == TraversalMove::Mantle);
    p.capsuleFitsOnTop = false;
    CHECK(Checked(p) == TraversalMove::Blocked);
    p = Probe(1.0, 0.4, 4.0);
    p.approachAngleDegrees = 60.0;
    CHECK(Checked(p) == TraversalMove::Blocked);
    p.approachAngleDegrees = -45.0;
    CHECK(Checked(p) == TraversalMove::Vault);
    p.approachAngleDegrees = std::numeric_limits<double>::quiet_NaN();
    CHECK(Checked(p) == TraversalMove::Blocked);
    // Step and jump are ordinary movement: the probe's safety fields do not matter.
    p = Probe(0.2, 0.4, 4.0);
    p.pathClear = false;
    CHECK(Checked(p) == TraversalMove::StepUp);
    p = Probe(0.35, 0.4, 4.0);
    p.pathClear = false;
    CHECK(Checked(p) == TraversalMove::Jump);
    // Climb keeps its own restrictions plus the safety ones.
    p = Probe(2.5, 2.0, 1.0);
    p.obstacle.climbable = true;
    CHECK(Checked(p) == TraversalMove::Climb);
    p.capsuleFitsOnTop = false;
    CHECK(Checked(p) == TraversalMove::Blocked);
}

TEST_CASE("abort, failure, exit state and the cooldown against double triggering") {
    const TraversalTimings t;
    // Abort works only before the move itself.
    TraversalState s = StartTraversal(TraversalMove::Vault);
    TraversalState aborted = AbortTraversal(s, t);
    CHECK(aborted.phase == TraversalPhase::Done);
    CHECK(aborted.result == TraversalResult::Aborted);
    CHECK(ExitMovementState(aborted, true) == MovementState::Landing);
    CHECK(ExitMovementState(aborted, false) == MovementState::Falling);
    s = AdvanceTraversal(s, t, t.approach);
    REQUIRE(s.phase == TraversalPhase::Active);
    CHECK(AbortTraversal(s, t).phase == TraversalPhase::Active);        // no teleport out of an active move
    CHECK(FailTraversal(StartTraversal(TraversalMove::Vault)).phase == TraversalPhase::Approach); // not active yet: unchanged
    // Failure while active: controlled release through Recover.
    const TraversalState failed = FailTraversal(s);
    CHECK(failed.phase == TraversalPhase::Recover);
    CHECK(failed.result == TraversalResult::Failed);
    CHECK_FALSE(TraversalOwnsMovement(failed));
    TraversalState done = AdvanceTraversal(failed, t, 1.0);
    CHECK(done.phase == TraversalPhase::Done);
    CHECK(done.result == TraversalResult::Failed);                       // stays Failed, not turned into Success
    CHECK(ExitMovementState(done, false) == MovementState::Falling);
    CHECK(ExitMovementState(done, true) == MovementState::Landing);
    // Success: vault lands, mantle stands.
    TraversalState vault = AdvanceTraversal(StartTraversal(TraversalMove::Vault), t, 5.0);
    CHECK(vault.result == TraversalResult::Success);
    CHECK(ExitMovementState(vault, true) == MovementState::Landing);
    TraversalState mantle = AdvanceTraversal(StartTraversal(TraversalMove::Mantle), t, 5.0);
    CHECK(ExitMovementState(mantle, true) == MovementState::Idle);
    CHECK(ExitMovementState(mantle, false) == MovementState::Falling);
    // An unfinished move never claims a ground state.
    CHECK(ExitMovementState(StartTraversal(TraversalMove::Mantle), true) == MovementState::Falling);
    // Cooldown: right after Done a new move is refused, later accepted.
    const TraversalState fresh = AdvanceTraversal(StartTraversal(TraversalMove::Vault), t, t.approach + t.active + t.recover + 0.1);
    CHECK(fresh.phase == TraversalPhase::Done);
    CHECK(fresh.cooldown == doctest::Approx(t.cooldown - 0.1));
    CHECK_FALSE(CanStartTraversal(fresh, TraversalMove::Vault, MovementState::Run));
    const TraversalState later = AdvanceTraversal(fresh, t, 0.25);
    CHECK(later.cooldown == 0.0);
    CHECK(CanStartTraversal(later, TraversalMove::Vault, MovementState::Run));
    CHECK(AdvanceTraversal(aborted, t, -1.0).cooldown == aborted.cooldown);  // negative time changes nothing
}

TEST_CASE("edge cases around the obstacle: corner, wall beside, narrow landing, moving obstacle, bad ground") {
    // A vault candidate that is fine in every respect...
    TraversalProbe ok = Probe(1.0, 0.4, 4.0);
    REQUIRE(Checked(ok) == TraversalMove::Vault);
    TraversalProbe p = ok;
    p.obstacleWidth = 0.5;                                    // only the corner of an obstacle is in front: a post, not a wall
    CHECK(Checked(p) == TraversalMove::Blocked);
    p.obstacleWidth = 0.84;                                   // exactly the capsule diameter is accepted
    CHECK(Checked(p) == TraversalMove::Vault);
    p = ok;
    p.sideClearance = 0.05;                                   // a wall flush with the ledge: the legs cannot swing across
    CHECK(Checked(p) == TraversalMove::Blocked);
    p.sideClearance = 0.15;
    CHECK(Checked(p) == TraversalMove::Vault);
    p = ok;
    p.landingWidth = 0.6;                                     // narrow landing strip
    CHECK(Checked(p) == TraversalMove::Blocked);
    p = ok;
    p.obstacleSpeed = 2.0;                                    // a vehicle driving by
    CHECK(Checked(p) == TraversalMove::Blocked);
    p.obstacleSpeed = 0.1;
    CHECK(Checked(p) == TraversalMove::Vault);                // within the tolerance of a swaying door
    p = ok;
    p.groundValid = false;                                    // water, lava, a steep roof behind the wall
    CHECK(Checked(p) == TraversalMove::Blocked);
    p = ok;
    p.obstacleSpeed = std::numeric_limits<double>::quiet_NaN();
    CHECK(Checked(p) == TraversalMove::Blocked);
    // A mantle does not need the side clearance, but it does need the width and a valid top.
    TraversalProbe m = Probe(1.0, 2.0, 0.0);
    REQUIRE(Checked(m) == TraversalMove::Mantle);
    m.sideClearance = 0.0;
    CHECK(Checked(m) == TraversalMove::Mantle);
    m.groundValid = false;
    CHECK(Checked(m) == TraversalMove::Blocked);
    // Ordinary movement is not restricted by any of this.
    TraversalProbe low = Probe(0.2, 0.4, 4.0);
    low.obstacleWidth = 0.1;
    low.groundValid = false;
    low.obstacleSpeed = 5.0;
    CHECK(Checked(low) == TraversalMove::StepUp);
}

TEST_CASE("abort during a traversal: approach can be cancelled, an active move cannot, a failed move releases and cools down") {
    const TraversalTimings t;
    TraversalState s = StartTraversal(TraversalMove::Mantle);
    s = AdvanceTraversal(s, t, 0.05);
    const TraversalState cancelled = AbortTraversal(s, t);
    CHECK(cancelled.result == TraversalResult::Aborted);
    CHECK_FALSE(TraversalOwnsMovement(cancelled));
    CHECK_FALSE(CanStartTraversal(cancelled, TraversalMove::Mantle, MovementState::Run));    // cooldown
    s = AdvanceTraversal(s, t, t.approach);
    REQUIRE(s.phase == TraversalPhase::Active);
    CHECK(AbortTraversal(s, t).phase == TraversalPhase::Active);
    const TraversalState failed = FailTraversal(s);
    CHECK(failed.phase == TraversalPhase::Recover);
    TraversalState released = AdvanceTraversal(failed, t, t.recover + 0.01);
    CHECK(released.phase == TraversalPhase::Done);
    CHECK(released.result == TraversalResult::Failed);
    CHECK(ExitMovementState(released, false) == MovementState::Falling);
    CHECK(released.cooldown > 0.0);
    released = AdvanceTraversal(released, t, 1.0);
    CHECK(CanStartTraversal(released, TraversalMove::Vault, MovementState::Jog));
}
