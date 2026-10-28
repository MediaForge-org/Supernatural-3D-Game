#include <doctest.h>

#include <limits>

#include <spn/core/InteractionReach.h>

using namespace spn::core;

namespace {
const ReachConfig kCfg;
ReachQuery Q(double tx, double ty, double tz, double radius = 0.0, bool los = true) {
    ReachQuery q;
    q.eye = {0.0, 0.0, 1.6};
    q.viewDirection = {1.0, 0.0, 0.0};
    q.targetCentre = {tx, ty, tz};
    q.targetRadius = radius;
    q.lineOfSightClear = los;
    return q;
}
} // namespace

TEST_CASE("reach config validation") {
    CHECK_FALSE(Validate(kCfg).has_value());
    ReachConfig c = kCfg;
    c.maxDistance = 0.0;
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.maxAngleDegrees = 200.0;
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.maxHeightDifference = std::numeric_limits<double>::quiet_NaN();
    CHECK(Validate(c).has_value());
}

TEST_CASE("distance is measured to the surface of the target") {
    CHECK(CheckReach(kCfg, Q(2.0, 0.0, 1.6)) == ReachResult::Ok);
    CHECK(CheckReach(kCfg, Q(2.5, 0.0, 1.6)) == ReachResult::Ok);        // exactly at the limit
    CHECK(CheckReach(kCfg, Q(2.51, 0.0, 1.6)) == ReachResult::TooFar);
    CHECK(CheckReach(kCfg, Q(3.0, 0.0, 1.6, 0.5)) == ReachResult::Ok);   // a 0.5 m radius brings the surface to 2.5 m
    CHECK(CheckReach(kCfg, Q(3.1, 0.0, 1.6, 0.5)) == ReachResult::TooFar);
    CHECK(CheckReach(kCfg, Q(0.0, 0.0, 1.6)) == ReachResult::Ok);        // standing inside the target
}

TEST_CASE("view angle, widened by the angular size of the target") {
    CHECK(CheckReach(kCfg, Q(1.0, 1.0, 1.6)) == ReachResult::Ok);        // 45 degrees
    CHECK(CheckReach(kCfg, Q(1.0, 1.8, 1.6)) == ReachResult::OutOfView); // about 61 degrees
    CHECK(CheckReach(kCfg, Q(1.0, 1.8, 1.6, 0.3)) == ReachResult::Ok);   // the 0.3 m radius covers 17 degrees more
    CHECK(CheckReach(kCfg, Q(-1.0, 0.0, 1.6)) == ReachResult::OutOfView);// behind the actor
    ReachQuery q = Q(1.0, 0.0, 1.6);
    q.viewDirection = {5.0, 0.0, 0.0};                                   // not normalised
    CHECK(CheckReach(kCfg, q) == ReachResult::Ok);
}

TEST_CASE("height difference, line of sight and the order of checks") {
    CHECK(CheckReach(kCfg, Q(1.0, 0.0, 3.5)) == ReachResult::TooHighOrLow);
    CHECK(CheckReach(kCfg, Q(1.0, 0.0, 0.2)) == ReachResult::Ok);         // 1.4 m below is still within 1.5
    CHECK(CheckReach(kCfg, Q(1.0, 0.0, 0.0)) == ReachResult::TooHighOrLow);
    CHECK(CheckReach(kCfg, Q(1.0, 0.0, 1.6, 0.0, false)) == ReachResult::Obstructed);
    ReachConfig open = kCfg;
    open.requireLineOfSight = false;
    CHECK(CheckReach(open, Q(1.0, 0.0, 1.6, 0.0, false)) == ReachResult::Ok);
    CHECK(CheckReach(kCfg, Q(9.0, 0.0, 9.0, 0.0, false)) == ReachResult::TooHighOrLow); // height is judged before distance and sight
    CHECK(CheckReach(kCfg, Q(9.0, 0.0, 1.6, 0.0, false)) == ReachResult::TooFar);        // distance before line of sight
}

TEST_CASE("invalid measurements never read as reachable") {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    CHECK(CheckReach(kCfg, Q(nan, 0.0, 1.6)) == ReachResult::InvalidInput);
    CHECK(CheckReach(kCfg, Q(1.0, 0.0, 1.6, -1.0)) == ReachResult::InvalidInput);
    ReachQuery q = Q(1.0, 0.0, 1.6);
    q.viewDirection = {0.0, 0.0, 0.0};
    CHECK(CheckReach(kCfg, q) == ReachResult::InvalidInput);
    q = Q(1.0, 0.0, 1.6);
    q.eye.x = std::numeric_limits<double>::infinity();
    CHECK(CheckReach(kCfg, q) == ReachResult::InvalidInput);
    CHECK(std::string(ReachResultName(ReachResult::Obstructed)) == "Obstructed");
}

TEST_CASE("a non-finite reach configuration never reads as reachable") {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    for (int field = 0; field < 3; ++field) {
        ReachConfig c;
        (field == 0 ? c.maxDistance : field == 1 ? c.maxAngleDegrees : c.maxHeightDifference) = nan;
        CHECK(CheckReach(c, Q(1.0, 0.0, 1.6)) == ReachResult::InvalidInput);
    }
    CHECK(CheckReach(kCfg, Q(1.0, 0.0, 1.6)) == ReachResult::Ok); // the default config is unaffected
}

TEST_CASE("actor state gates interactions") {
    CHECK(MayInteract(ActorInteractState{}));
    CHECK_FALSE(MayInteract(ActorInteractState{true, false, false, false}));
    CHECK_FALSE(MayInteract(ActorInteractState{false, true, false, false}));
    CHECK_FALSE(MayInteract(ActorInteractState{false, false, true, false}));
    CHECK_FALSE(MayInteract(ActorInteractState{false, false, false, true}));
}
