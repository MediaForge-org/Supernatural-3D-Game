#include <doctest.h>

#include <spn/core/BodyCollision.h>

#include <algorithm>
#include <limits>

using namespace spn::core;

namespace {
bool Has(const BodyCollisionResult& r, BodyCollisionIssue issue) {
    return std::find(r.issues.begin(), r.issues.end(), issue) != r.issues.end();
}
} // namespace

TEST_CASE("body collision: Keep is today's capsule for every body height") {
    for (float height : {0.f, 160.f, 180.f, 193.f, 230.f}) {
        const BodyCollisionResult r = ResolveBodyCollision({BodyCollisionMode::Keep, height, {}});
        CHECK(r.profile.radiusCm == doctest::Approx(35.f));
        CHECK(r.profile.halfHeightCm == doctest::Approx(90.f));
        CHECK(r.profile.crouchedHalfHeightCm == doctest::Approx(40.f));
        CHECK_FALSE(r.differsFromDefault);
        CHECK(r.halfHeightDeltaCm == doctest::Approx(0.f));
        CHECK(r.issues.empty());
    }
}

TEST_CASE("body collision: Keep still reports how far the head sticks out (0017: 193 cm body in a 180 cm capsule)") {
    const BodyCollisionResult r = ResolveBodyCollision({BodyCollisionMode::Keep, 193.3f, {}});
    CHECK(r.headAboveCapsuleCm == doctest::Approx(13.3f));
    CHECK(ResolveBodyCollision({BodyCollisionMode::Keep, 0.f, {}}).headAboveCapsuleCm == doctest::Approx(0.f));
}

TEST_CASE("body collision: FromHeight covers the head with the clearance, keeps the radius, scales the crouch") {
    const BodyCollisionResult tall = ResolveBodyCollision({BodyCollisionMode::FromHeight, 193.3f, {}});
    CHECK(tall.profile.halfHeightCm == doctest::Approx(97.65f));
    CHECK(tall.profile.radiusCm == doctest::Approx(35.f));
    CHECK(tall.profile.crouchedHalfHeightCm == doctest::Approx(40.f * 97.65f / 90.f));
    CHECK(tall.halfHeightDeltaCm == doctest::Approx(7.65f));
    CHECK(tall.headAboveCapsuleCm == doctest::Approx(-2.f));   // the capsule top is 2 cm above the head
    CHECK(tall.differsFromDefault);
    CHECK(tall.issues.empty());

    const BodyCollisionResult shorter = ResolveBodyCollision({BodyCollisionMode::FromHeight, 160.f, {}});
    CHECK(shorter.profile.halfHeightCm == doctest::Approx(81.f));
    CHECK(shorter.halfHeightDeltaCm == doctest::Approx(-9.f));   // a shorter body gets a shorter capsule, not only taller ones
    CHECK(shorter.profile.crouchedHalfHeightCm < 40.f);
}

TEST_CASE("body collision: FromHeight without a height falls back to the default and says so") {
    const BodyCollisionResult r = ResolveBodyCollision({BodyCollisionMode::FromHeight, 0.f, {}});
    CHECK(r.profile.halfHeightCm == doctest::Approx(90.f));
    CHECK_FALSE(r.differsFromDefault);
    CHECK(Has(r, BodyCollisionIssue::NoBodyHeight));
    CHECK(Has(ResolveBodyCollision({BodyCollisionMode::FromHeight, std::numeric_limits<float>::quiet_NaN(), {}}), BodyCollisionIssue::NoBodyHeight));
}

TEST_CASE("body collision: heights outside the limits are clamped and reported") {
    const BodyCollisionResult giant = ResolveBodyCollision({BodyCollisionMode::FromHeight, 400.f, {}});
    CHECK(giant.profile.halfHeightCm == doctest::Approx(125.f));
    CHECK(Has(giant, BodyCollisionIssue::HalfHeightClamped));
    const BodyCollisionResult tiny = ResolveBodyCollision({BodyCollisionMode::FromHeight, 50.f, {}});
    CHECK(tiny.profile.halfHeightCm == doctest::Approx(60.f));
    CHECK(Has(tiny, BodyCollisionIssue::HalfHeightClamped));
}

TEST_CASE("body collision: Explicit is clamped, a crouch below the radius or above the standing height is corrected") {
    const BodyCollisionResult r = ResolveBodyCollision({BodyCollisionMode::Explicit, 0.f, {80.f, 100.f, 20.f}});
    CHECK(r.profile.radiusCm == doctest::Approx(60.f));
    CHECK(Has(r, BodyCollisionIssue::RadiusClamped));
    CHECK(r.profile.crouchedHalfHeightCm == doctest::Approx(60.f));   // never below the radius
    CHECK(Has(r, BodyCollisionIssue::CrouchClamped));

    const BodyCollisionResult high = ResolveBodyCollision({BodyCollisionMode::Explicit, 0.f, {30.f, 95.f, 120.f}});
    CHECK(high.profile.crouchedHalfHeightCm == doctest::Approx(95.f));
    CHECK(Has(high, BodyCollisionIssue::CrouchClamped));

    const BodyCollisionResult ok = ResolveBodyCollision({BodyCollisionMode::Explicit, 175.f, {32.f, 88.f, 42.f}});
    CHECK(ok.issues.empty());
    CHECK(ok.profile.radiusCm == doctest::Approx(32.f));
    CHECK(ok.profile.halfHeightCm == doctest::Approx(88.f));
    CHECK(ok.profile.crouchedHalfHeightCm == doctest::Approx(42.f));
}

TEST_CASE("body collision: custom rules change the defaults (data-driven, no hard-coded body)") {
    BodyCollisionRules rules;
    rules.defaultRadiusCm = 30.f;
    rules.defaultHalfHeightCm = 85.f;
    rules.defaultCrouchedHalfHeightCm = 45.f;
    rules.headClearanceCm = 4.f;
    const BodyCollisionResult keep = ResolveBodyCollision({BodyCollisionMode::Keep, 165.f, {}}, rules);
    CHECK(keep.profile.radiusCm == doctest::Approx(30.f));
    CHECK(keep.profile.halfHeightCm == doctest::Approx(85.f));
    const BodyCollisionResult fit = ResolveBodyCollision({BodyCollisionMode::FromHeight, 166.f, {}}, rules);
    CHECK(fit.profile.halfHeightCm == doctest::Approx(85.f));
    CHECK_FALSE(fit.differsFromDefault);
}

TEST_CASE("passage fit: capsule, crouch and visible head are checked separately") {
    const BodyCollisionProfile keep{35.f, 90.f, 40.f};
    // 193 cm body, 180 cm capsule, 190 cm door: the capsule passes, the head visibly clips through the frame (0017 open point)
    const PassageFit door190 = FitPassage(keep, 193.3f, 190.f, 90.f);
    CHECK(door190.capsuleFitsStanding);
    CHECK_FALSE(door190.headClearsStanding);
    CHECK(door190.headroomCm == doctest::Approx(-3.3f));

    const BodyCollisionProfile tall = ResolveBodyCollision({BodyCollisionMode::FromHeight, 193.3f, {}}).profile;
    const PassageFit tallDoor190 = FitPassage(tall, 193.3f, 190.f, 90.f);
    CHECK_FALSE(tallDoor190.capsuleFitsStanding);   // with a fitted capsule the 190 cm door blocks standing...
    CHECK(tallDoor190.capsuleFitsCrouched);         // ...but not crouching
    CHECK(FitPassage(tall, 193.3f, 205.f, 90.f).capsuleFitsStanding);

    CHECK_FALSE(FitPassage(keep, 180.f, 220.f, 60.f).capsuleFitsStanding);   // 70 cm wide capsule, 60 cm opening
    CHECK(FitPassage(keep, 0.f, 185.f, 80.f).headClearsStanding);            // unknown body height: only the capsule counts
}
