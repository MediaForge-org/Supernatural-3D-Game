#include <doctest.h>

#include <spn/core/BodyCoverage.h>

using namespace spn::core;

namespace {
ContentId Id(const char* text) { return *ContentId::Parse(text); }

WearableCoverage Wear(const char* id, WearLayer layer, ZoneMask covers, ZoneMask hides, bool opaque = true) {
    return {Id(id), layer, covers, hides, opaque};
}

const ZoneMask kTrunk = Zone(BodyZone::Chest) | Zone(BodyZone::Abdomen) | Zone(BodyZone::Back);
const ZoneMask kHips = Zone(BodyZone::Pelvis) | Zone(BodyZone::Buttocks);
} // namespace

TEST_CASE("body coverage: nothing worn exposes every zone and hides no body") {
    const CoverageResult r = ResolveCoverage({}, {}, {});
    CHECK(r.covered == 0);
    CHECK(r.hiddenBody == 0);
    CHECK(r.exposed == AllZones());
    CHECK(r.details.empty());
    CHECK(r.issues.empty());
}

TEST_CASE("body coverage: layers add up, body is hidden only where a wearer says so") {
    const std::vector<WearableCoverage> worn{
        Wear("base:shorts_test", WearLayer::Underwear, kHips, kHips),
        Wear("base:shirt_test", WearLayer::Base, kTrunk | Zone(BodyZone::UpperArms), kTrunk),   // loose sleeves: arms covered, not hidden
        Wear("base:jacket_test", WearLayer::Outer, kTrunk | Zone(BodyZone::UpperArms) | Zone(BodyZone::Forearms), 0),
    };
    const CoverageResult r = ResolveCoverage(worn, {}, {});
    CHECK(r.covered == (kHips | kTrunk | Zone(BodyZone::UpperArms) | Zone(BodyZone::Forearms)));
    CHECK(r.hiddenBody == (kHips | kTrunk));
    CHECK((r.exposed & Zone(BodyZone::Hands)) != 0);
    CHECK(r.issues.empty());
    // order independent
    const std::vector<WearableCoverage> reversed(worn.rbegin(), worn.rend());
    const CoverageResult r2 = ResolveCoverage(reversed, {}, {});
    CHECK(r2.covered == r.covered);
    CHECK(r2.hiddenBody == r.hiddenBody);
}

TEST_CASE("body coverage: sheer items cover nothing and may not hide body geometry") {
    const CoverageResult r = ResolveCoverage({Wear("base:mesh_top_test", WearLayer::Base, kTrunk, kTrunk, /*opaque*/ false)}, {}, {});
    CHECK(r.covered == 0);
    CHECK(r.hiddenBody == 0);
    REQUIRE(r.issues.size() == 1);
    CHECK(r.issues[0].kind == CoverageIssueKind::TransparentHides);
}

TEST_CASE("body coverage: hiding outside the covered zones is ignored and reported") {
    const CoverageResult r = ResolveCoverage({Wear("base:gloves_test", WearLayer::Accessory, Zone(BodyZone::Hands), Zone(BodyZone::Hands) | Zone(BodyZone::Forearms))}, {}, {});
    CHECK(r.hiddenBody == Zone(BodyZone::Hands));
    REQUIRE(r.issues.size() == 1);
    CHECK(r.issues[0].kind == CoverageIssueKind::HidesUncoveredZone);
    CHECK(r.issues[0].zones == Zone(BodyZone::Forearms));
}

TEST_CASE("body coverage: two items on the same layer over the same zone are a reported conflict") {
    const CoverageResult r = ResolveCoverage({Wear("base:a_test", WearLayer::Base, kTrunk, 0), Wear("base:b_test", WearLayer::Base, Zone(BodyZone::Chest), 0)}, {}, {});
    REQUIRE(r.issues.size() == 1);
    CHECK(r.issues[0].kind == CoverageIssueKind::LayerConflict);
    CHECK(r.issues[0].item == Id("base:b_test"));
}

TEST_CASE("body coverage: optional detail geometry needs an enabled category AND an exposed zone") {
    const std::vector<BodyDetailDef> details{
        {Id("base:scar_relief_test"), Id("base:relief"), BodyZone::Chest},
        {Id("pack_x:detail_test"), Id("pack_x:detail"), BodyZone::Pelvis},
    };
    // default policy: nothing optional at all
    CHECK(ResolveCoverage({}, details, {}).details.empty());
    PresentationPolicy policy;
    policy.enabledDetailCategories = {Id("pack_x:detail")};
    // enabled + exposed
    CoverageResult r = ResolveCoverage({}, details, policy);
    REQUIRE(r.details.size() == 1);
    CHECK(r.details[0] == Id("pack_x:detail_test"));
    // enabled but covered by any opaque layer (underwear is enough): not created, so a dressed character never pays for it
    r = ResolveCoverage({Wear("base:shorts_test", WearLayer::Underwear, kHips, 0)}, details, policy);
    CHECK(r.details.empty());
    // a sheer item does not count as covering
    r = ResolveCoverage({Wear("base:sheer_test", WearLayer::Underwear, kHips, 0, false)}, details, policy);
    CHECK(r.details.size() == 1);
}

TEST_CASE("body coverage: hideCoveredBody=false keeps all body geometry") {
    PresentationPolicy policy;
    policy.hideCoveredBody = false;
    const CoverageResult r = ResolveCoverage({Wear("base:shirt_test", WearLayer::Base, kTrunk, kTrunk)}, {}, policy);
    CHECK(r.covered == kTrunk);
    CHECK(r.hiddenBody == 0);
}

TEST_CASE("body coverage: zone and layer names") {
    CHECK(std::string(BodyZoneName(BodyZone::Pelvis)) == "pelvis");
    CHECK(std::string(BodyZoneName(BodyZone::Feet)) == "feet");
    CHECK(std::string(WearLayerName(WearLayer::Underwear)) == "underwear");
    CHECK(AllZones() == 0x7FFFu);
}
