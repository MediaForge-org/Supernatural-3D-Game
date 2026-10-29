#include <doctest.h>

#include <spn/core/Devices.h>
#include <spn/core/TestDoor.h>

using namespace spn::core;

namespace {
InteractionRegistry MakeRegistry() {
    InteractionRegistry r;
    REQUIRE_FALSE(RegisterDevices(r).has_value());
    REQUIRE_FALSE(RegisterTestDoor(r).has_value());
    return r;
}
Result<InteractionOutcome, InteractionError> Use(const InteractionRegistry& r, WorldFacts& f, const ContentId& kind, const char* id, const InteractionDefaults& d = {}) {
    return r.Interact(f, InteractableRef{kind, id}, InteractionVerb::Use, d, "test");
}
} // namespace

TEST_CASE("switch toggles, honours authored defaults and the disabled flag") {
    const auto r = MakeRegistry();
    WorldFacts f;
    CHECK(Use(r, f, SwitchKind(), "sw").HasValue());
    CHECK(*f.FindAs<bool>(SwitchOnKey("sw")) == true);
    CHECK(Use(r, f, SwitchKind(), "sw").HasValue());
    CHECK(*f.FindAs<bool>(SwitchOnKey("sw")) == false);

    InteractionDefaults on;
    on.values["on"] = true;
    WorldFacts g;
    CHECK(Use(r, g, SwitchKind(), "sw", on).HasValue());
    CHECK(*g.FindAs<bool>(SwitchOnKey("sw")) == false); // authored on -> toggled off

    REQUIRE(f.Apply(SetFact{SwitchDisabledKey("sw"), true}).HasValue());
    const auto journal = f.Revision();
    const auto refused = Use(r, f, SwitchKind(), "sw");
    REQUIRE_FALSE(refused.HasValue());
    CHECK(refused.Error().code == InteractionErrorCode::Refused);
    CHECK(refused.Error().feedback == DeviceDisabledFeedback());
    CHECK(f.Revision() == journal);
}

TEST_CASE("switch with a corrupt fact reports StateCorrupt and writes nothing") {
    const auto r = MakeRegistry();
    WorldFacts f;
    REQUIRE(f.Apply(SetFact{SwitchOnKey("sw"), std::int64_t{3}}).HasValue());
    const auto result = Use(r, f, SwitchKind(), "sw");
    REQUIRE_FALSE(result.HasValue());
    CHECK(result.Error().code == InteractionErrorCode::StateCorrupt);
    InteractionDefaults bad;
    bad.values["disabled"] = std::string("yes");
    WorldFacts g;
    CHECK(Use(r, g, SwitchKind(), "sw2", bad).Error().code == InteractionErrorCode::StateCorrupt);
}

TEST_CASE("button counts presses, is exhausted at max_presses and respects disabled") {
    const auto r = MakeRegistry();
    WorldFacts f;
    InteractionDefaults twice;
    twice.values["max_presses"] = std::int64_t{2};
    CHECK(Use(r, f, ButtonKind(), "b", twice).HasValue());
    CHECK(Use(r, f, ButtonKind(), "b", twice).HasValue());
    CHECK(*f.FindAs<std::int64_t>(ButtonPressesKey("b")) == 2);
    const auto third = Use(r, f, ButtonKind(), "b", twice);
    REQUIRE_FALSE(third.HasValue());
    CHECK(third.Error().feedback == ButtonExhaustedFeedback());
    CHECK(*f.FindAs<std::int64_t>(ButtonPressesKey("b")) == 2);

    WorldFacts unlimited;
    for (int i = 0; i < 25; ++i) REQUIRE(Use(r, unlimited, ButtonKind(), "b").HasValue());
    CHECK(*unlimited.FindAs<std::int64_t>(ButtonPressesKey("b")) == 25);

    InteractionDefaults negative;
    negative.values["max_presses"] = std::int64_t{-1};
    CHECK(Use(r, unlimited, ButtonKind(), "b", negative).Error().code == InteractionErrorCode::StateCorrupt);
    REQUIRE(unlimited.Apply(SetFact{ButtonPressesKey("c"), std::int64_t{-5}}).HasValue());
    CHECK(Use(r, unlimited, ButtonKind(), "c").Error().code == InteractionErrorCode::StateCorrupt);
    REQUIRE(unlimited.Apply(SetFact{ButtonPressesKey("m"), INT64_MAX}).HasValue());
    CHECK(Use(r, unlimited, ButtonKind(), "m").Error().code == InteractionErrorCode::StateCorrupt); // no overflow
}

TEST_CASE("lever advances through its positions and wraps - invalid configuration or state is corrupt") {
    const auto r = MakeRegistry();
    WorldFacts f;
    InteractionDefaults three;
    three.values["positions"] = std::int64_t{3};
    std::vector<std::int64_t> seen;
    for (int i = 0; i < 7; ++i) {
        REQUIRE(Use(r, f, LeverKind(), "lv", three).HasValue());
        seen.push_back(*f.FindAs<std::int64_t>(LeverPositionKey("lv")));
    }
    CHECK(seen == std::vector<std::int64_t>{1, 2, 0, 1, 2, 0, 1});
    // default: two positions = a toggle
    WorldFacts g;
    REQUIRE(Use(r, g, LeverKind(), "lv").HasValue());
    CHECK(*g.FindAs<std::int64_t>(LeverPositionKey("lv")) == 1);
    REQUIRE(Use(r, g, LeverKind(), "lv").HasValue());
    CHECK(*g.FindAs<std::int64_t>(LeverPositionKey("lv")) == 0);
    for (const std::int64_t bad : {std::int64_t{0}, std::int64_t{1}, kMaxLeverPositions + 1, std::int64_t{-4}}) {
        InteractionDefaults d;
        d.values["positions"] = bad;
        CHECK(Use(r, g, LeverKind(), "lv", d).Error().code == InteractionErrorCode::StateCorrupt);
    }
    REQUIRE(g.Apply(SetFact{LeverPositionKey("out"), std::int64_t{2}}).HasValue()); // 2 is outside [0, 2)
    CHECK(Use(r, g, LeverKind(), "out").Error().code == InteractionErrorCode::StateCorrupt);
    REQUIRE(g.Apply(SetFact{LeverDisabledKey("lv"), true}).HasValue());
    CHECK(Use(r, g, LeverKind(), "lv").Error().feedback == DeviceDisabledFeedback());
}

TEST_CASE("door: disabled comes before locked, lock helpers validate, authored state is separate from runtime facts") {
    const auto r = MakeRegistry();
    WorldFacts f;
    CHECK_FALSE(SetTestDoorLocked(f, "d", true, "test").has_value());
    CHECK(Use(r, f, TestDoorKind(), "d").Error().feedback == TestDoorLockedFeedback());
    CHECK_FALSE(SetTestDoorDisabled(f, "d", true, "test").has_value());
    CHECK(Use(r, f, TestDoorKind(), "d").Error().feedback == TestDoorDisabledFeedback());
    CHECK_FALSE(SetTestDoorDisabled(f, "d", false, "test").has_value());
    CHECK_FALSE(SetTestDoorLocked(f, "d", false, "test").has_value());
    CHECK(Use(r, f, TestDoorKind(), "d").HasValue());
    CHECK(*f.FindAs<bool>(TestDoorOpenKey("d")) == true);
    CHECK(SetTestDoorLocked(f, "Bad Id", true, "test").has_value());      // invalid key
    // authored lock is only a default; a runtime unlock wins
    InteractionDefaults locked;
    locked.values["locked"] = true;
    WorldFacts g;
    CHECK(Use(r, g, TestDoorKind(), "d", locked).Error().feedback == TestDoorLockedFeedback());
    CHECK_FALSE(SetTestDoorLocked(g, "d", false, "unlock").has_value());
    CHECK(Use(r, g, TestDoorKind(), "d", locked).HasValue());
}

TEST_CASE("devices cannot be registered twice") {
    InteractionRegistry r;
    REQUIRE_FALSE(RegisterDevices(r).has_value());
    const auto again = RegisterDevices(r);
    REQUIRE(again.has_value());
    CHECK(again->code == InteractionErrorCode::DuplicateKind);
}
