#include <doctest.h>

#include <limits>

#include <spn/core/CameraArm.h>

using namespace spn::core;

namespace {
const CameraArmConfig kCfg;
}

TEST_CASE("camera arm config validation") {
    CHECK_FALSE(Validate(kCfg).has_value());
    CameraArmConfig c = kCfg;
    c.minLength = 0.0;
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.baseLength = 0.3;
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.recoverSpeed = 0.0;
    CHECK(Validate(c).has_value());
    c = kCfg;
    c.collisionMargin = std::numeric_limits<double>::quiet_NaN();
    CHECK(Validate(c).has_value());
}

TEST_CASE("desired length: free, sprinting, in a tight room, against a wall") {
    CHECK(DesiredArmLength(kCfg, {}, std::nullopt) == doctest::Approx(4.0));
    CHECK(DesiredArmLength(kCfg, {true, false}, std::nullopt) == doctest::Approx(4.5));
    CameraArmConfig air = kCfg;
    air.airborneExtra = 0.3;
    CHECK(DesiredArmLength(air, {true, true}, std::nullopt) == doctest::Approx(4.8));
    CHECK(DesiredArmLength(kCfg, {}, 2.0) == doctest::Approx(1.85));       // wall at 2 m: margin kept
    CHECK(DesiredArmLength(kCfg, {}, 10.0) == doctest::Approx(4.0));       // far wall: no effect
    CHECK(DesiredArmLength(kCfg, {true, false}, 4.3) == doctest::Approx(4.15));
    CHECK(DesiredArmLength(kCfg, {}, 0.2) == doctest::Approx(0.6));        // never below the minimum
    CHECK(DesiredArmLength(kCfg, {}, -1.0) == doctest::Approx(0.6));
    CHECK(DesiredArmLength(kCfg, {}, std::numeric_limits<double>::quiet_NaN()) == doctest::Approx(4.0)); // a broken probe is ignored
}

TEST_CASE("the arm shortens at once and grows back only after the delay, at the recover speed") {
    CameraArmState s{4.0, 0.0};
    s = StepCameraArm(kCfg, s, 1.0, 1.0 / 60.0);
    CHECK(s.length == doctest::Approx(1.0));                                 // immediate
    // Target clear again: nothing for 0.25 s, then +2.5 m/s.
    double t = 0.0;
    while (t < 0.24) { s = StepCameraArm(kCfg, s, 4.0, 0.01); t += 0.01; }
    CHECK(s.length == doctest::Approx(1.0));
    s = StepCameraArm(kCfg, s, 4.0, 0.02);
    CHECK(s.length > 1.0);
    for (int i = 0; i < 600 && s.length < 4.0; ++i) s = StepCameraArm(kCfg, s, 4.0, 1.0 / 60.0);
    CHECK(s.length == doctest::Approx(4.0));
}

TEST_CASE("doorway flicker does not make the camera pump") {
    // The probe alternates between a wall at 1.5 m and clear every frame: the arm must stay short, not oscillate.
    CameraArmState s{1.35, 0.0};
    double maxLength = 0.0;
    for (int i = 0; i < 120; ++i) {
        const double target = (i % 2) ? 4.0 : 1.35;
        s = StepCameraArm(kCfg, s, target, 1.0 / 60.0);
        maxLength = std::max(maxLength, s.length);
    }
    CHECK(maxLength == doctest::Approx(1.35));
}

TEST_CASE("sprint toggling and bad steps") {
    CameraArmState s{4.0, 0.0};
    s = StepCameraArm(kCfg, s, 4.5, 0.1);                                   // sprint starts: a longer target, delayed growth
    CHECK(s.length == doctest::Approx(4.0));
    const CameraArmState same = StepCameraArm(kCfg, s, 1.0, std::numeric_limits<double>::quiet_NaN());
    CHECK(same.length == s.length);
    CHECK(StepCameraArm(kCfg, s, 1.0, 0.0).length == s.length);
    CHECK(StepCameraArm(kCfg, s, std::numeric_limits<double>::infinity(), 0.1).length == s.length);
    CHECK(StepCameraArm(kCfg, s, std::numeric_limits<double>::quiet_NaN(), 0.1).length == s.length);
}
