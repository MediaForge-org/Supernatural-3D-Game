#include "AppearanceTestCatalog.h"

#include <array>
#include <doctest.h>
#include <limits>
#include <set>
#include <spn/core/Appearance.h>
#include <string>
#include <string_view>

using namespace spn::core;
using spn::testing::Id;
using spn::testing::MakeProviderCatalog;
using spn::testing::MakeTestCatalog;

namespace {

std::string Tabs(std::string text) {
    for (std::size_t pos = text.find("<T>"); pos != std::string::npos;
         pos = text.find("<T>", pos)) {
        text.replace(pos, 3, "\t");
    }
    return text;
}

/// A valid spec touching every field of the model.
AppearanceSpec MakeSpec() {
    AppearanceSpec spec;
    spec.bodyProvider = Id("base:body.placeholder_a");
    spec.hairStyle = Id("base:hair.placeholder_short");
    spec.parameters.emplace(Id("base:face.jaw_width"), -120);
    spec.parameters.emplace(Id("base:skin.tone"), 640);
    spec.clothing.emplace(ClothingSlot::Torso, Id("base:cloth.placeholder_shirt"));
    spec.clothing.emplace(ClothingSlot::Legs, Id("base:cloth.placeholder_jeans"));
    spec.clothing.emplace(ClothingSlot::Outer, Id("base:cloth.placeholder_jacket"));
    spec.layers.push_back(
        {LayerKind::Tattoo, Id("base:tattoo.placeholder_anchor"), BodyRegion::Torso, 900});
    spec.layers.push_back(
        {LayerKind::Scar, Id("base:scar.placeholder_slash"), BodyRegion::LeftArm, 450});
    return spec;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization): test data, a failure aborts the run.
const std::string kGoldenText =
    Tabs("SPNAPPEARANCE 1\n"
         "body<T>base:body.placeholder_a\n"
         "hair<T>base:hair.placeholder_short\n"
         "facial_hair<T>-\n"
         "param<T>base:face.jaw_width<T>-120\n"
         "param<T>base:skin.tone<T>640\n"
         "clothing<T>torso<T>base:cloth.placeholder_shirt\n"
         "clothing<T>legs<T>base:cloth.placeholder_jeans\n"
         "clothing<T>outer<T>base:cloth.placeholder_jacket\n"
         "layer<T>tattoo<T>base:tattoo.placeholder_anchor<T>torso<T>900\n"
         "layer<T>scar<T>base:scar.placeholder_slash<T>left_arm<T>450\n"
         "end<T>10\n");

/// Unwraps a catalog error that must be present.
AppearanceError Err(const std::optional<AppearanceError>& error) {
    REQUIRE(error.has_value());
    return error.value_or(AppearanceError{AppearanceErrorCode::EmptyCatalog, "", 0});
}

AppearanceError LoadError(std::string_view text) {
    const auto result = DeserializeAppearance(text);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

/// Validation against `catalog` must report exactly this one code (and subject).
void CheckSingleIssueIn(const AppearanceSpec& spec, const AppearanceCatalog& catalog,
                        AppearanceErrorCode code, std::string_view subject) {
    const auto issues = ValidateAppearance(spec, catalog);
    REQUIRE(issues.size() == 1);
    CHECK(issues.front().code == code);
    CHECK(issues.front().subject == subject);
}

void CheckSingleIssue(const AppearanceSpec& spec, AppearanceErrorCode code,
                      std::string_view subject) {
    CheckSingleIssueIn(spec, MakeTestCatalog(), code, subject);
}

} // namespace

// --- Catalog ----------------------------------------------------------------

TEST_CASE("Catalog: the built-in parameter set is valid, unique and covers all groups") {
    const std::vector<ParameterDef> defs = BuiltinParameterDefs();
    CHECK(defs.size() == 29);
    AppearanceCatalog catalog;
    std::set<ParameterGroup> groups;
    for (const ParameterDef& def : defs) {
        CHECK_FALSE(catalog.AddParameter(def).has_value());
        CHECK(def.id.Namespace() == "base");
        CHECK(def.min <= def.defaultValue);
        CHECK(def.defaultValue <= def.max);
        groups.insert(def.group);
    }
    for (const ParameterGroup group :
         {ParameterGroup::Face, ParameterGroup::Body, ParameterGroup::Skin, ParameterGroup::Eyes,
          ParameterGroup::Hair}) {
        CHECK(groups.contains(group));
    }
}

TEST_CASE("Catalog: rejects duplicates, empty ids and bad ranges") {
    AppearanceCatalog catalog;
    const ParameterDef good =
        spn::testing::P(Id("mod_a:face.horn_size"), ParameterGroup::Custom, 0, 100, 10);
    CHECK_FALSE(catalog.AddParameter(good).has_value());

    const AppearanceError duplicate = Err(catalog.AddParameter(good));
    CHECK(duplicate.code == AppearanceErrorCode::DuplicateDefinition);
    CHECK(duplicate.subject == "mod_a:face.horn_size");

    CHECK(Err(catalog.AddParameter(spn::testing::P(ParameterId{}, ParameterGroup::Custom, 0, 1, 0)))
              .code == AppearanceErrorCode::InvalidId);

    CHECK(Err(catalog.AddParameter(
                  spn::testing::P(Id("mod_a:x.inverted"), ParameterGroup::Custom, 10, 0, 5)))
              .code == AppearanceErrorCode::InvalidParameterRange);
    CHECK(Err(catalog.AddParameter(
                  spn::testing::P(Id("mod_a:x.default_low"), ParameterGroup::Custom, 0, 10, -1)))
              .code == AppearanceErrorCode::InvalidParameterRange);
    CHECK(Err(catalog.AddParameter(
                  spn::testing::P(Id("mod_a:x.default_high"), ParameterGroup::Custom, 0, 10, 11)))
              .code == AppearanceErrorCode::InvalidParameterRange);
    CHECK(catalog.Parameters().size() == 1); // failed adds changed nothing

    CHECK_FALSE(catalog.AddContent(spn::testing::C(Id("mod_a:hair.horns"), ContentKind::HairStyle))
                    .has_value());
    CHECK(
        Err(catalog.AddContent(spn::testing::C(Id("mod_a:hair.horns"), ContentKind::BodyProvider)))
            .code == AppearanceErrorCode::DuplicateDefinition);
    CHECK(Err(catalog.AddContent(spn::testing::C(ContentId{}, ContentKind::HairStyle))).code ==
          AppearanceErrorCode::InvalidId);
    const ContentDef* horns = catalog.FindContent(Id("mod_a:hair.horns"));
    REQUIRE(horns != nullptr);
    CHECK(horns->kind == ContentKind::HairStyle);
}

TEST_CASE("Catalog: fingerprint is stable and reacts to every definition change") {
    CHECK(MakeTestCatalog().Fingerprint() == MakeTestCatalog().Fingerprint());
    CHECK(AppearanceCatalog{}.Fingerprint() != MakeTestCatalog().Fingerprint());

    AppearanceCatalog extended = MakeTestCatalog();
    REQUIRE_FALSE(
        extended.AddContent(spn::testing::C(Id("mod_a:hair.horns"), ContentKind::HairStyle))
            .has_value());
    CHECK(extended.Fingerprint() != MakeTestCatalog().Fingerprint());

    AppearanceCatalog widened = MakeTestCatalog();
    REQUIRE_FALSE(widened
                      .AddParameter(spn::testing::P(Id("mod_a:face.horn_size"),
                                                    ParameterGroup::Custom, 0, 100, 0))
                      .has_value());
    AppearanceCatalog widenedMore = MakeTestCatalog();
    REQUIRE_FALSE(widenedMore
                      .AddParameter(spn::testing::P(Id("mod_a:face.horn_size"),
                                                    ParameterGroup::Custom, 0, 200, 0))
                      .has_value());
    CHECK(widened.Fingerprint() != widenedMore.Fingerprint());
}

// --- Validation -------------------------------------------------------------

TEST_CASE("Validate: a consistent spec is valid, and so is a minimal one") {
    CHECK(ValidateAppearance(MakeSpec(), MakeTestCatalog()).empty());

    AppearanceSpec minimal;
    minimal.bodyProvider = Id("base:body.placeholder_b");
    CHECK(ValidateAppearance(minimal, MakeTestCatalog()).empty());
}

TEST_CASE("Validate: body provider") {
    AppearanceSpec spec = MakeSpec();
    spec.bodyProvider = ContentId{};
    CheckSingleIssue(spec, AppearanceErrorCode::MissingBodyProvider, "");
    spec.bodyProvider = Id("mod_missing:body.x");
    CheckSingleIssue(spec, AppearanceErrorCode::UnknownContent, "mod_missing:body.x");
    spec.bodyProvider = Id("base:hair.placeholder_short");
    CheckSingleIssue(spec, AppearanceErrorCode::ContentKindMismatch, "base:hair.placeholder_short");
}

TEST_CASE("Validate: hair and facial hair") {
    AppearanceSpec spec = MakeSpec();
    spec.hairStyle = Id("base:beard.placeholder_full");
    CheckSingleIssue(spec, AppearanceErrorCode::ContentKindMismatch, "base:beard.placeholder_full");
    spec = MakeSpec();
    spec.facialHairStyle = Id("base:hair.placeholder_long");
    CheckSingleIssue(spec, AppearanceErrorCode::ContentKindMismatch, "base:hair.placeholder_long");
    spec = MakeSpec();
    spec.facialHairStyle = Id("base:beard.nope");
    CheckSingleIssue(spec, AppearanceErrorCode::UnknownContent, "base:beard.nope");
}

TEST_CASE("Validate: parameters") {
    AppearanceSpec spec = MakeSpec();
    spec.parameters[Id("base:skin.tone")] = 1001;
    CheckSingleIssue(spec, AppearanceErrorCode::ParameterOutOfRange, "base:skin.tone");
    spec.parameters[Id("base:skin.tone")] = -1;
    CheckSingleIssue(spec, AppearanceErrorCode::ParameterOutOfRange, "base:skin.tone");
    spec.parameters[Id("base:skin.tone")] = 1000; // boundary is valid
    CHECK(ValidateAppearance(spec, MakeTestCatalog()).empty());
    spec.parameters[Id("base:face.jaw_width")] = -1000;
    CHECK(ValidateAppearance(spec, MakeTestCatalog()).empty());
    spec.parameters[Id("base:face.jaw_width")] = -1001;
    CheckSingleIssue(spec, AppearanceErrorCode::ParameterOutOfRange, "base:face.jaw_width");

    spec = MakeSpec();
    spec.parameters.emplace(Id("mod_missing:face.horn_size"), 5);
    CheckSingleIssue(spec, AppearanceErrorCode::UnknownParameter, "mod_missing:face.horn_size");
}

TEST_CASE("Validate: clothing") {
    AppearanceSpec spec = MakeSpec();
    spec.clothing[ClothingSlot::Feet] = Id("base:cloth.placeholder_shirt"); // torso item in feet
    CheckSingleIssue(spec, AppearanceErrorCode::ClothingSlotMismatch,
                     "base:cloth.placeholder_shirt");
    spec = MakeSpec();
    spec.clothing[ClothingSlot::Feet] = Id("base:hair.placeholder_short");
    CheckSingleIssue(spec, AppearanceErrorCode::ContentKindMismatch, "base:hair.placeholder_short");
    spec = MakeSpec();
    spec.clothing[ClothingSlot::Feet] = Id("base:cloth.nope");
    CheckSingleIssue(spec, AppearanceErrorCode::UnknownContent, "base:cloth.nope");
}

TEST_CASE("Validate: layers") {
    AppearanceSpec spec = MakeSpec();
    spec.layers[0].opacity = 1001;
    CheckSingleIssue(spec, AppearanceErrorCode::OpacityOutOfRange,
                     "base:tattoo.placeholder_anchor");
    spec.layers[0].opacity = -1;
    CheckSingleIssue(spec, AppearanceErrorCode::OpacityOutOfRange,
                     "base:tattoo.placeholder_anchor");
    spec.layers[0].opacity = 0; // boundary is valid
    CHECK(ValidateAppearance(spec, MakeTestCatalog()).empty());

    spec = MakeSpec();
    spec.layers[1].kind = LayerKind::Tattoo; // scar asset declared as tattoo
    CheckSingleIssue(spec, AppearanceErrorCode::LayerKindMismatch, "base:scar.placeholder_slash");

    spec = MakeSpec();
    spec.layers.push_back(spec.layers[0]); // exact duplicate
    CheckSingleIssue(spec, AppearanceErrorCode::DuplicateLayer, "base:tattoo.placeholder_anchor");
    spec.layers.back().region = BodyRegion::Back; // same asset elsewhere is fine
    CHECK(ValidateAppearance(spec, MakeTestCatalog()).empty());

    spec = MakeSpec();
    spec.layers[0].content = Id("base:cloth.placeholder_shirt");
    CheckSingleIssue(spec, AppearanceErrorCode::ContentKindMismatch,
                     "base:cloth.placeholder_shirt");
}

TEST_CASE("Validate: layer count limit") {
    AppearanceSpec spec = MakeSpec();
    spec.layers.clear();
    for (std::size_t i = 0; i < kMaxAppearanceLayers; ++i) {
        spec.layers.push_back({LayerKind::Tattoo, Id("base:tattoo.placeholder_anchor"),
                               kAllBodyRegions[i % kAllBodyRegions.size()],
                               static_cast<ParamValue>(i)});
        // Distinct (content, region) pairs are limited by the region count, so vary content.
        spec.layers.back().content = Id(i % 3 == 0 ? "base:tattoo.placeholder_anchor"
                                                   : (i % 3 == 1 ? "base:tattoo.placeholder_sigil"
                                                                 : "base:tattoo.placeholder_band"));
    }
    // 32 layers over 3 contents x 11 regions: at most 33 unique, so duplicates are impossible here.
    CHECK(ValidateAppearance(spec, MakeTestCatalog()).empty());
    spec.layers.push_back({LayerKind::Scar, Id("base:scar.placeholder_burn"), BodyRegion::Head, 1});
    CheckSingleIssue(spec, AppearanceErrorCode::TooManyLayers, "33");
}

TEST_CASE("Validate: schema version and several problems at once") {
    AppearanceSpec spec = MakeSpec();
    spec.schemaVersion = 2;
    CheckSingleIssue(spec, AppearanceErrorCode::UnsupportedSchemaVersion, "2");

    spec = MakeSpec();
    spec.hairStyle = Id("base:hair.nope");
    spec.parameters[Id("base:skin.tone")] = 5000;
    spec.layers[1].opacity = -5;
    const auto issues = ValidateAppearance(spec, MakeTestCatalog());
    REQUIRE(issues.size() == 3); // all reported, in deterministic spec order
    CHECK(issues[0].code == AppearanceErrorCode::UnknownContent);
    CHECK(issues[1].code == AppearanceErrorCode::ParameterOutOfRange);
    CHECK(issues[2].code == AppearanceErrorCode::OpacityOutOfRange);
    CHECK(ValidateAppearance(spec, MakeTestCatalog()) == issues);
}

TEST_CASE("Errors: Describe is readable and every code has a distinct name") {
    CHECK(Describe({AppearanceErrorCode::ParameterOutOfRange, "base:skin.tone", 0}) ==
          "parameter out of range: base:skin.tone");
    CHECK(Describe({AppearanceErrorCode::MalformedLine, "", 7}) == "malformed line (line 7)");
    CHECK(Describe({AppearanceErrorCode::InvalidToken, "x", 3}) == "invalid token: x (line 3)");

    std::set<std::string_view> names;
    for (int value = 0; value <= static_cast<int>(AppearanceErrorCode::IndexOutOfRange); ++value) {
        const std::string_view name = ToString(static_cast<AppearanceErrorCode>(value));
        CHECK(name != "unknown error");
        CHECK(names.insert(name).second);
    }
    CHECK(ToString(static_cast<AppearanceErrorCode>(250)) == "unknown error");
}

TEST_CASE("Vocabulary: enum names are distinct and round trip through the format") {
    std::set<std::string_view> slots;
    for (const ClothingSlot slot : kAllClothingSlots) {
        CHECK(slots.insert(ToString(slot)).second);
        CHECK(ToString(slot) != "unknown");
    }
    std::set<std::string_view> kinds;
    for (const LayerKind kind : kAllLayerKinds) {
        CHECK(kinds.insert(ToString(kind)).second);
    }
    std::set<std::string_view> regions;
    for (const BodyRegion region : kAllBodyRegions) {
        CHECK(regions.insert(ToString(region)).second);
    }
    CHECK(ToString(ParameterGroup::Face) == "face");
    CHECK(ToString(ContentKind::FacialHairStyle) == "facial_hair_style");

    // Every region and layer kind survives serialize -> deserialize.
    AppearanceSpec spec = MakeSpec();
    spec.layers.clear();
    for (const BodyRegion region : kAllBodyRegions) {
        spec.layers.push_back({LayerKind::Dirt, Id("base:dirt.x"), region, 1});
    }
    for (const LayerKind kind : kAllLayerKinds) {
        spec.layers.push_back({kind, Id("base:y.z"), BodyRegion::Head, 2});
    }
    const auto loaded = DeserializeAppearance(SerializeAppearance(spec));
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == spec);
}

// --- Serialization ----------------------------------------------------------

TEST_CASE("Serialize: canonical text is stable (golden)") {
    CHECK(SerializeAppearance(MakeSpec()) == kGoldenText);
    CHECK(SerializeAppearance(MakeSpec()) == SerializeAppearance(MakeSpec()));
}

TEST_CASE("Serialize: insertion order does not matter, layer order does") {
    AppearanceSpec a;
    a.bodyProvider = Id("base:body.placeholder_a");
    a.parameters.emplace(Id("base:skin.tone"), 1);
    a.parameters.emplace(Id("base:body.height"), 2);
    a.clothing.emplace(ClothingSlot::Feet, Id("base:cloth.placeholder_boots"));
    a.clothing.emplace(ClothingSlot::Head, Id("base:cloth.placeholder_cap"));

    AppearanceSpec b;
    b.bodyProvider = Id("base:body.placeholder_a");
    b.parameters.emplace(Id("base:body.height"), 2);
    b.parameters.emplace(Id("base:skin.tone"), 1);
    b.clothing.emplace(ClothingSlot::Head, Id("base:cloth.placeholder_cap"));
    b.clothing.emplace(ClothingSlot::Feet, Id("base:cloth.placeholder_boots"));
    CHECK(SerializeAppearance(a) == SerializeAppearance(b));

    const AppearanceSpec c = MakeSpec();
    AppearanceSpec d = MakeSpec();
    std::swap(d.layers[0], d.layers[1]);
    CHECK(SerializeAppearance(c) != SerializeAppearance(d)); // stacking order is data
}

TEST_CASE("Deserialize: round trip restores the spec exactly") {
    const auto loaded = DeserializeAppearance(kGoldenText);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == MakeSpec());
    CHECK(SerializeAppearance(loaded.Value()) == kGoldenText);

    const AppearanceSpec minimal = [] {
        AppearanceSpec spec;
        spec.bodyProvider = Id("base:body.placeholder_a");
        return spec;
    }();
    const auto again = DeserializeAppearance(SerializeAppearance(minimal));
    REQUIRE(again.HasValue());
    CHECK(again.Value() == minimal);
}

TEST_CASE("Deserialize: extreme and negative values survive") {
    AppearanceSpec spec = MakeSpec();
    spec.parameters[Id("mod_a:x.max")] = std::numeric_limits<ParamValue>::max();
    spec.parameters[Id("mod_a:x.min")] = std::numeric_limits<ParamValue>::min();
    spec.parameters[Id("mod_a:x.zero")] = 0;
    const auto loaded = DeserializeAppearance(SerializeAppearance(spec));
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == spec);
    // Loading is syntax only: the catalog decides whether those values are acceptable.
    CHECK_FALSE(ValidateAppearance(loaded.Value(), MakeTestCatalog()).empty());
}

TEST_CASE("Deserialize: loading does not need the referenced content to exist") {
    // A save that references a missing mod loads fine; validation reports the problem.
    const auto loaded = DeserializeAppearance(kGoldenText);
    REQUIRE(loaded.HasValue());
    const auto issues = ValidateAppearance(loaded.Value(), AppearanceCatalog{});
    CHECK_FALSE(issues.empty());
    CHECK(issues.front().code == AppearanceErrorCode::UnknownContent);
}

TEST_CASE("Deserialize: rejects bad headers and versions") {
    CHECK(LoadError("") == AppearanceError{AppearanceErrorCode::InvalidHeader, "", 0});
    CHECK(LoadError("hello\n").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNAPPEARANCE 1").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNAPPEARANCE x\n").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNAPPEARANCE 01\n").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("spnappearance 1\n").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNAPPEARANCE 2\n") ==
          AppearanceError{AppearanceErrorCode::UnsupportedVersion, "", 1});
    CHECK(LoadError("SPNAPPEARANCE 0\n").code == AppearanceErrorCode::UnsupportedVersion);
    CHECK(LoadError("SPNAPPEARANCE 1\r\n").code == AppearanceErrorCode::InvalidHeader);
}

TEST_CASE("Deserialize: rejects truncation, count errors and trailing data") {
    for (std::size_t length = 0; length < kGoldenText.size(); ++length) {
        CHECK_FALSE(
            DeserializeAppearance(std::string_view(kGoldenText).substr(0, length)).HasValue());
    }
    std::string wrongCount = kGoldenText;
    wrongCount.replace(wrongCount.rfind("10"), 2, "9");
    CHECK(LoadError(wrongCount).code == AppearanceErrorCode::CountMismatch);
    CHECK(LoadError(kGoldenText + "x\n") ==
          AppearanceError{AppearanceErrorCode::TrailingData, "", 13});
    CHECK(LoadError(kGoldenText + kGoldenText).code == AppearanceErrorCode::TrailingData);
}

TEST_CASE("Deserialize: rejects malformed and out-of-order lines") {
    const std::string head = "SPNAPPEARANCE 1\n";
    const std::string fixed = Tabs("body<T>base:body.placeholder_a\nhair<T>-\nfacial_hair<T>-\n");

    CHECK(LoadError(head + Tabs("hair<T>-\n")).code == AppearanceErrorCode::MalformedLine);
    CHECK(LoadError(head + Tabs("body<T>a:b<T>c\n")).code == AppearanceErrorCode::MalformedLine);
    CHECK(LoadError(head + fixed + "bogus\n").code == AppearanceErrorCode::MalformedLine);
    CHECK(LoadError(head + fixed + Tabs("param<T>base:skin.tone\n")).code ==
          AppearanceErrorCode::MalformedLine);
    CHECK(LoadError(head + fixed + Tabs("layer<T>tattoo<T>a:b<T>torso\n")).code ==
          AppearanceErrorCode::MalformedLine);

    // Sections must come in order: param*, clothing*, layer*.
    const std::string clothing = Tabs("clothing<T>torso<T>base:cloth.placeholder_shirt\n");
    const std::string layer = Tabs("layer<T>tattoo<T>base:tattoo.placeholder_anchor<T>torso<T>5\n");
    const std::string param = Tabs("param<T>base:skin.tone<T>1\n");
    CHECK(LoadError(head + fixed + clothing + param).code == AppearanceErrorCode::NonCanonical);
    CHECK(LoadError(head + fixed + layer + clothing).code == AppearanceErrorCode::NonCanonical);
    CHECK(LoadError(head + fixed + layer + param).code == AppearanceErrorCode::NonCanonical);
}

TEST_CASE("Deserialize: rejects invalid and non-canonical tokens") {
    const std::string head = "SPNAPPEARANCE 1\n";
    const std::string fixed = Tabs("body<T>base:body.placeholder_a\nhair<T>-\nfacial_hair<T>-\n");
    const auto withLine = [&](const std::string& line, int count = 4) {
        return head + fixed + Tabs(line) + "end\t" + std::to_string(count) + '\n';
    };
    const auto code = [&](const std::string& line) { return LoadError(withLine(line)).code; };
    const auto invalid = AppearanceErrorCode::InvalidToken;

    CHECK(DeserializeAppearance(withLine("param<T>base:skin.tone<T>5\n")).HasValue());
    CHECK(code("param<T>BAD<T>5\n") == invalid);
    CHECK(code("param<T>base:skin.tone<T>05\n") == invalid);
    CHECK(code("param<T>base:skin.tone<T>+5\n") == invalid);
    CHECK(code("param<T>base:skin.tone<T>-0\n") == invalid);
    CHECK(code("param<T>base:skin.tone<T>\n") == invalid);
    CHECK(code("param<T>base:skin.tone<T>2147483648\n") == invalid);
    CHECK(code("param<T>-<T>5\n") == invalid);
    CHECK(code("clothing<T>Torso<T>base:cloth.placeholder_shirt\n") == invalid);
    CHECK(code("clothing<T>neck<T>base:cloth.placeholder_shirt\n") == invalid);
    CHECK(code("clothing<T>torso<T>-\n") == invalid);
    CHECK(code("layer<T>tattoo<T>base:tattoo.placeholder_anchor<T>torso<T>x\n") == invalid);
    CHECK(code("layer<T>stamp<T>base:tattoo.placeholder_anchor<T>torso<T>1\n") == invalid);
    CHECK(code("layer<T>tattoo<T>base:tattoo.placeholder_anchor<T>belly<T>1\n") == invalid);
    CHECK(code("layer<T>tattoo<T>-<T>torso<T>1\n") == invalid);
    CHECK(LoadError(head + Tabs("body<T>-\nhair<T>-\nfacial_hair<T>-\nend<T>3\n")).code == invalid);
    CHECK(LoadError(head + Tabs("body<T>base:body.placeholder_a\nhair<T>Base:x\n")).code ==
          invalid);

    // Duplicate or unsorted entries have exactly one canonical spelling.
    const std::string twoParams = Tabs("param<T>base:skin.tone<T>1\nparam<T>base:skin.tone<T>2\n");
    CHECK(LoadError(head + fixed + twoParams + "end\t5\n").code ==
          AppearanceErrorCode::NonCanonical);
    const std::string unsorted = Tabs("param<T>base:skin.tone<T>1\nparam<T>base:body.height<T>2\n");
    CHECK(LoadError(head + fixed + unsorted + "end\t5\n").code ==
          AppearanceErrorCode::NonCanonical);
    const std::string unsortedClothing = Tabs("clothing<T>legs<T>a:b\nclothing<T>torso<T>a:b\n");
    CHECK(LoadError(head + fixed + unsortedClothing + "end\t5\n").code ==
          AppearanceErrorCode::NonCanonical);
}

TEST_CASE("Deserialize: errors carry line numbers and offending tokens") {
    std::string broken = kGoldenText;
    broken.replace(broken.find("640"), 3, "6x0");
    const AppearanceError error = LoadError(broken);
    CHECK(error.code == AppearanceErrorCode::InvalidToken);
    CHECK(error.line == 6);
    CHECK(error.subject == "6x0");
    CHECK(Describe(error) == "invalid token: 6x0 (line 6)");
}

TEST_CASE("Deserialize: any single-byte corruption is rejected or re-serializes identically") {
    constexpr std::string_view kReplacements = "X\t\n:-0.";
    std::size_t rejected = 0;
    for (std::size_t i = 0; i < kGoldenText.size(); ++i) {
        for (const char replacement : kReplacements) {
            if (kGoldenText[i] == replacement) {
                continue;
            }
            std::string mutated = kGoldenText;
            mutated[i] = replacement;
            const auto result = DeserializeAppearance(mutated);
            if (result.HasValue()) {
                CHECK(SerializeAppearance(result.Value()) == mutated);
            } else {
                ++rejected;
            }
        }
    }
    CHECK(rejected > 0);
}

// --- Body-provider compatibility -------------------------------------------------

TEST_CASE("Providers: parameters and content can be restricted to body providers") {
    const AppearanceCatalog catalog = MakeProviderCatalog();
    CHECK(catalog.ValidateConsistency().empty());

    AppearanceSpec spec = MakeSpec(); // body A
    spec.parameters.emplace(Id("mod_a:body.horn_size"), 10);
    CheckSingleIssueIn(spec, catalog, AppearanceErrorCode::ParameterUnsupportedByProvider,
                       "mod_a:body.horn_size");
    spec.bodyProvider = Id("base:body.placeholder_b");
    CHECK(ValidateAppearance(spec, catalog).empty());

    spec = MakeSpec();
    spec.clothing.emplace(ClothingSlot::Head, Id("mod_a:cloth.hat_for_a"));
    CHECK(ValidateAppearance(spec, catalog).empty());
    spec.bodyProvider = Id("base:body.placeholder_b");
    CheckSingleIssueIn(spec, catalog, AppearanceErrorCode::ContentIncompatibleWithProvider,
                       "mod_a:cloth.hat_for_a");

    // Hair and layers are restricted the same way.
    AppearanceCatalog restricted = MakeTestCatalog();
    ContentDef hair = spn::testing::C(Id("mod_a:hair.only_b"), ContentKind::HairStyle);
    hair.providers = {Id("base:body.placeholder_b")};
    ContentDef scar = spn::testing::C(Id("mod_a:scar.only_b"), ContentKind::Layer);
    scar.layerKind = LayerKind::Scar;
    scar.providers = {Id("base:body.placeholder_b")};
    REQUIRE_FALSE(restricted.AddContent(hair).has_value());
    REQUIRE_FALSE(restricted.AddContent(scar).has_value());
    spec = MakeSpec();
    spec.hairStyle = Id("mod_a:hair.only_b");
    spec.layers[1].content = Id("mod_a:scar.only_b");
    const auto issues = ValidateAppearance(spec, restricted);
    REQUIRE(issues.size() == 2);
    CHECK(issues[0].code == AppearanceErrorCode::ContentIncompatibleWithProvider);
    CHECK(issues[1].code == AppearanceErrorCode::ContentIncompatibleWithProvider);
}

TEST_CASE("Providers: an unusable body provider reports only itself") {
    const AppearanceCatalog catalog = MakeProviderCatalog();
    AppearanceSpec spec = MakeSpec();
    spec.parameters.emplace(Id("mod_a:body.horn_size"), 10);
    spec.bodyProvider = Id("mod_gone:body.x");
    // Fit cannot be judged against a missing body: no follow-up noise.
    CheckSingleIssueIn(spec, catalog, AppearanceErrorCode::UnknownContent, "mod_gone:body.x");
}

TEST_CASE("Providers: catalog consistency finds dangling references") {
    AppearanceCatalog catalog = MakeTestCatalog();
    ParameterDef param = spn::testing::P(Id("mod_a:x.p"), ParameterGroup::Custom, 0, 1, 0);
    param.providers = {Id("mod_gone:body.x")};
    ContentDef cloth = spn::testing::C(Id("mod_a:cloth.x"), ContentKind::Clothing);
    cloth.providers = {Id("base:hair.placeholder_short")}; // exists, but not a body provider
    REQUIRE_FALSE(catalog.AddParameter(param).has_value());
    REQUIRE_FALSE(catalog.AddContent(cloth).has_value());
    const auto issues = catalog.ValidateConsistency();
    REQUIRE(issues.size() == 2);
    CHECK(issues[0].code == AppearanceErrorCode::UnknownProviderReference);
    CHECK(issues[0].subject == "mod_gone:body.x");
    CHECK(issues[1].subject == "base:hair.placeholder_short");
}

TEST_CASE("Providers: lists are sets and affect the fingerprint") {
    const auto fingerprint = [](std::vector<ContentId> providers) {
        AppearanceCatalog catalog = MakeTestCatalog();
        ContentDef cloth = spn::testing::C(Id("mod_a:cloth.x"), ContentKind::Clothing);
        cloth.providers = std::move(providers);
        REQUIRE_FALSE(catalog.AddContent(cloth).has_value());
        return catalog.Fingerprint();
    };
    const ContentId a = Id("base:body.placeholder_a");
    const ContentId b = Id("base:body.placeholder_b");
    CHECK(fingerprint({a, b}) == fingerprint({b, a, a}));
    CHECK(fingerprint({a}) != fingerprint({b}));
    CHECK(fingerprint({}) != fingerprint({a}));

    CHECK(AppearanceCatalog::Supports({}, a));
    CHECK(AppearanceCatalog::Supports({a}, a));
    CHECK_FALSE(AppearanceCatalog::Supports({a}, b));
}

TEST_CASE("Catalog: the number of parameters is bounded") {
    AppearanceCatalog catalog;
    for (std::size_t i = 0; i < kMaxAppearanceParameters; ++i) {
        std::string name = "x:p";
        for (std::size_t n = i + 1; n > 0; n /= 10) {
            name.push_back(static_cast<char>('a' + n % 10));
        }
        REQUIRE_FALSE(
            catalog.AddParameter(spn::testing::P(Id(name), ParameterGroup::Custom, 0, 1, 0))
                .has_value());
    }
    const auto error =
        catalog.AddParameter(spn::testing::P(Id("x:overflow"), ParameterGroup::Custom, 0, 1, 0));
    CHECK(error.value_or(AppearanceError{AppearanceErrorCode::EmptyCatalog, "", 0}).code ==
          AppearanceErrorCode::TooManyParameters);
}

// --- Availability (disabled / missing mods) ---------------------------------------

TEST_CASE("Availability: a fully available spec comes back unchanged") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const auto view = MakeAvailableAppearance(MakeSpec(), catalog);
    REQUIRE(view.HasValue());
    CHECK(view.Value().spec == MakeSpec());
    CHECK(view.Value().omitted.empty());
}

TEST_CASE("Availability: parts from a missing mod are left out, never deleted") {
    AppearanceSpec spec = MakeSpec();
    spec.hairStyle = Id("mod_gone:hair.horns");
    spec.facialHairStyle = Id("mod_gone:beard.x");
    spec.parameters.emplace(Id("mod_gone:face.horn_size"), 5);
    spec.parameters[Id("base:skin.tone")] = 5000; // out of range for the catalog
    spec.clothing.emplace(ClothingSlot::Head, Id("mod_gone:cloth.crown"));
    spec.layers.push_back({LayerKind::Tattoo, Id("mod_gone:tattoo.x"), BodyRegion::Back, 500});
    spec.layers.push_back(spec.layers[0]); // duplicate
    const AppearanceSpec original = spec;
    const AppearanceCatalog catalog = MakeTestCatalog();

    const auto view = MakeAvailableAppearance(spec, catalog);
    REQUIRE(view.HasValue());
    CHECK(spec == original); // the stored spec is untouched

    const AppearanceSpec& shown = view.Value().spec;
    CHECK(shown.bodyProvider == spec.bodyProvider);
    CHECK(shown.hairStyle.IsEmpty());
    CHECK(shown.facialHairStyle.IsEmpty());
    CHECK_FALSE(shown.parameters.contains(Id("mod_gone:face.horn_size")));
    CHECK_FALSE(shown.parameters.contains(Id("base:skin.tone")));
    CHECK(shown.parameters.contains(Id("base:face.jaw_width")));
    CHECK_FALSE(shown.clothing.contains(ClothingSlot::Head));
    CHECK(shown.clothing.size() == 3);
    CHECK(shown.layers.size() == 2);
    CHECK(ValidateAppearance(shown, catalog).empty());

    REQUIRE(view.Value().omitted.size() == 7);
    CHECK(view.Value().omitted[0].code == AppearanceErrorCode::UnknownContent); // hair
    CHECK(view.Value().omitted[0].subject == "mod_gone:hair.horns");
}

TEST_CASE("Availability: provider mismatches and layer limits are handled") {
    const AppearanceCatalog catalog = MakeProviderCatalog();
    AppearanceSpec spec = MakeSpec();
    spec.bodyProvider = Id("base:body.placeholder_b");
    spec.clothing.emplace(ClothingSlot::Head, Id("mod_a:cloth.hat_for_a"));
    spec.parameters.emplace(Id("mod_a:body.horn_size"), 10); // supported by B
    const auto view = MakeAvailableAppearance(spec, catalog);
    REQUIRE(view.HasValue());
    REQUIRE(view.Value().omitted.size() == 1);
    CHECK(view.Value().omitted[0].code == AppearanceErrorCode::ContentIncompatibleWithProvider);
    CHECK(view.Value().spec.parameters.contains(Id("mod_a:body.horn_size")));

    AppearanceSpec many = MakeSpec();
    many.layers.clear();
    for (std::size_t i = 0; i < kMaxAppearanceLayers + 3; ++i) {
        many.layers.push_back(
            {LayerKind::Dirt, Id("base:tattoo.placeholder_anchor"), BodyRegion::Torso, 1});
        many.layers.back().kind = LayerKind::Tattoo;
        many.layers.back().region = kAllBodyRegions[i % kAllBodyRegions.size()];
        many.layers.back().content =
            Id(i % 2 == 0 ? "base:tattoo.placeholder_sigil" : "base:tattoo.placeholder_band");
    }
    const auto capped = MakeAvailableAppearance(many, MakeTestCatalog());
    REQUIRE(capped.HasValue());
    CHECK(capped.Value().spec.layers.size() <= kMaxAppearanceLayers);
    CHECK(ValidateAppearance(capped.Value().spec, MakeTestCatalog()).empty());
}

TEST_CASE("Availability: an unusable body provider is an error, not a guess") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceSpec spec = MakeSpec();
    spec.bodyProvider = Id("mod_gone:body.x");
    auto view = MakeAvailableAppearance(spec, catalog);
    REQUIRE_FALSE(view.HasValue());
    CHECK(view.Error().code == AppearanceErrorCode::UnknownContent);

    spec.bodyProvider = ContentId{};
    CHECK(MakeAvailableAppearance(spec, catalog).Error().code ==
          AppearanceErrorCode::MissingBodyProvider);
    spec.bodyProvider = Id("base:hair.placeholder_short");
    CHECK(MakeAvailableAppearance(spec, catalog).Error().code ==
          AppearanceErrorCode::ContentKindMismatch);
    spec = MakeSpec();
    spec.schemaVersion = 9;
    CHECK(MakeAvailableAppearance(spec, catalog).Error().code ==
          AppearanceErrorCode::UnsupportedSchemaVersion);
}

TEST_CASE("Namespaces: the mods a character needs are listed") {
    AppearanceSpec spec = MakeSpec();
    CHECK(ReferencedNamespaces(spec) == std::vector<std::string>{"base"});
    spec.hairStyle = Id("mod_b:hair.x");
    spec.parameters.emplace(Id("mod_a:face.horn"), 1);
    spec.clothing.emplace(ClothingSlot::Head, Id("mod_a:cloth.crown"));
    spec.layers.push_back({LayerKind::Tattoo, Id("mod_c:tattoo.x"), BodyRegion::Back, 1});
    CHECK(ReferencedNamespaces(spec) ==
          std::vector<std::string>{"base", "mod_a", "mod_b", "mod_c"});
    CHECK(ReferencedNamespaces(AppearanceSpec{}).empty());
}

TEST_CASE("Deserialize: entry counts are limited before memory is spent") {
    const std::string head =
        "SPNAPPEARANCE 1\nbody\tbase:body.placeholder_a\nhair\t-\nfacial_hair\t-\n";
    std::string layers = head;
    for (std::size_t i = 0; i <= kMaxAppearanceLayers; ++i) {
        layers += "layer\ttattoo\tbase:tattoo.placeholder_anchor\ttorso\t1\n";
    }
    layers += "end\t" + std::to_string(3 + kMaxAppearanceLayers + 1) + "\n";
    CHECK(LoadError(layers).code == AppearanceErrorCode::TooManyLayers);
    CHECK(LoadError(layers).line == 4 + kMaxAppearanceLayers + 1);

    std::string params = head;
    for (std::size_t i = 0; i <= kMaxAppearanceParameters; ++i) {
        std::string name = "x:p";
        for (std::size_t n = i + 1; n > 0; n /= 10) {
            name.push_back(static_cast<char>('a' + n % 10));
        }
        params += "param\t" + name + "\t1\n";
    }
    // Ids are not in ascending order here, so either error is a correct rejection; what
    // matters is that the loader never accepts or allocates unboundedly.
    CHECK_FALSE(DeserializeAppearance(params + "end\t0\n").HasValue());
}

TEST_CASE("Vocabulary: generation shapes have names") {
    CHECK(ToString(GenerationShape::Bell) == "bell");
    CHECK(ToString(GenerationShape::Uniform) == "uniform");
    CHECK(ToString(GenerationShape::TowardDefault) == "toward_default");
}
