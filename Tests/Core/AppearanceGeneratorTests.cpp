#include "AppearanceTestCatalog.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <doctest.h>
#include <limits>
#include <set>
#include <spn/core/Appearance.h>
#include <spn/core/AppearanceGenerator.h>
#include <spn/core/DeterministicRandom.h>
#include <string>

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

// Frozen output of generator version 1 for the test catalog (see MakeTestCatalog).
// Never edit to make a failing test pass: a change means the algorithm changed.
// NOLINTNEXTLINE(bugprone-throwing-static-initialization): test data, a failure aborts the run.
const std::string kExpectedSeed12345 = Tabs("SPNAPPEARANCE 1\n"
                                            "body<T>base:body.placeholder_b\n"
                                            "hair<T>base:hair.placeholder_curly\n"
                                            "facial_hair<T>-\n"
                                            "param<T>base:body.height<T>187\n"
                                            "param<T>base:body.hip_width<T>611\n"
                                            "param<T>base:body.leg_length<T>162\n"
                                            "param<T>base:body.mass<T>110\n"
                                            "param<T>base:body.muscle<T>-240\n"
                                            "param<T>base:body.shoulder_width<T>354\n"
                                            "param<T>base:eyes.iris_hue<T>309\n"
                                            "param<T>base:eyes.iris_lightness<T>682\n"
                                            "param<T>base:eyes.iris_saturation<T>436\n"
                                            "param<T>base:face.brow_height<T>366\n"
                                            "param<T>base:face.cheek_fullness<T>-277\n"
                                            "param<T>base:face.chin_length<T>-121\n"
                                            "param<T>base:face.ear_size<T>-200\n"
                                            "param<T>base:face.eye_size<T>-49\n"
                                            "param<T>base:face.eye_spacing<T>-290\n"
                                            "param<T>base:face.head_width<T>-420\n"
                                            "param<T>base:face.jaw_width<T>-122\n"
                                            "param<T>base:face.lip_fullness<T>145\n"
                                            "param<T>base:face.mouth_width<T>19\n"
                                            "param<T>base:face.nose_bridge_height<T>39\n"
                                            "param<T>base:face.nose_width<T>70\n"
                                            "param<T>base:hair.density<T>466\n"
                                            "param<T>base:hair.grey_amount<T>0\n"
                                            "param<T>base:hair.melanin<T>710\n"
                                            "param<T>base:hair.redness<T>19\n"
                                            "param<T>base:skin.age_wear<T>239\n"
                                            "param<T>base:skin.freckles<T>9\n"
                                            "param<T>base:skin.tone<T>651\n"
                                            "param<T>base:skin.undertone<T>-48\n"
                                            "clothing<T>torso<T>base:cloth.placeholder_shirt\n"
                                            "clothing<T>legs<T>base:cloth.placeholder_jeans\n"
                                            "clothing<T>feet<T>base:cloth.placeholder_boots\n"
                                            "clothing<T>outer<T>base:cloth.placeholder_jacket\n"
                                            "end<T>36\n");

// NOLINTNEXTLINE(bugprone-throwing-static-initialization): test data, a failure aborts the run.
const std::string kExpectedSeed0 = Tabs("SPNAPPEARANCE 1\n"
                                        "body<T>base:body.placeholder_b\n"
                                        "hair<T>base:hair.placeholder_curly\n"
                                        "facial_hair<T>base:beard.placeholder_short\n"
                                        "param<T>base:body.height<T>-79\n"
                                        "param<T>base:body.hip_width<T>554\n"
                                        "param<T>base:body.leg_length<T>-84\n"
                                        "param<T>base:body.mass<T>-71\n"
                                        "param<T>base:body.muscle<T>-75\n"
                                        "param<T>base:body.shoulder_width<T>-99\n"
                                        "param<T>base:eyes.iris_hue<T>449\n"
                                        "param<T>base:eyes.iris_lightness<T>436\n"
                                        "param<T>base:eyes.iris_saturation<T>553\n"
                                        "param<T>base:face.brow_height<T>334\n"
                                        "param<T>base:face.cheek_fullness<T>249\n"
                                        "param<T>base:face.chin_length<T>-296\n"
                                        "param<T>base:face.ear_size<T>-91\n"
                                        "param<T>base:face.eye_size<T>29\n"
                                        "param<T>base:face.eye_spacing<T>-79\n"
                                        "param<T>base:face.head_width<T>-349\n"
                                        "param<T>base:face.jaw_width<T>164\n"
                                        "param<T>base:face.lip_fullness<T>48\n"
                                        "param<T>base:face.mouth_width<T>207\n"
                                        "param<T>base:face.nose_bridge_height<T>134\n"
                                        "param<T>base:face.nose_width<T>-83\n"
                                        "param<T>base:hair.density<T>664\n"
                                        "param<T>base:hair.grey_amount<T>430\n"
                                        "param<T>base:hair.melanin<T>293\n"
                                        "param<T>base:hair.redness<T>362\n"
                                        "param<T>base:skin.age_wear<T>4\n"
                                        "param<T>base:skin.freckles<T>512\n"
                                        "param<T>base:skin.tone<T>394\n"
                                        "param<T>base:skin.undertone<T>253\n"
                                        "clothing<T>torso<T>base:cloth.placeholder_shirt\n"
                                        "clothing<T>legs<T>base:cloth.placeholder_jeans\n"
                                        "clothing<T>feet<T>base:cloth.placeholder_boots\n"
                                        "end<T>35\n");

AppearanceSpec Generate(const AppearanceCatalog& catalog, std::uint64_t seed,
                        const GenerationOptions& options = {}) {
    auto result = GenerateAppearance(catalog, seed, options);
    REQUIRE_MESSAGE(result.HasValue(), "generation failed: ", Describe(result.Error()));
    return std::move(result).Value();
}

} // namespace

// --- Random primitives -------------------------------------------------------

TEST_CASE("SplitMix64: matches the published reference sequence") {
    SplitMix64 rng(0);
    CHECK(rng.Next() == 0xe220a8397b1dcdafULL);
    CHECK(rng.Next() == 0x6e789e6aa1b965f4ULL);
    CHECK(rng.Next() == 0x06c45d188009454fULL);
}

TEST_CASE("SplitMix64: bounded draws stay in range and cover it") {
    SplitMix64 rng(42);
    CHECK(rng.NextBelow(0) == 0);
    CHECK(rng.NextBelow(1) == 0);
    std::set<std::uint64_t> seen;
    for (int i = 0; i < 2000; ++i) {
        const std::uint64_t value = rng.NextBelow(7);
        CHECK(value < 7);
        seen.insert(value);
    }
    CHECK(seen.size() == 7);

    for (int i = 0; i < 2000; ++i) {
        const std::int32_t value = rng.NextInRange(-3, 3);
        CHECK(value >= -3);
        CHECK(value <= 3);
    }
    CHECK(rng.NextInRange(5, 5) == 5);
    CHECK(rng.NextInRange(9, 2) == 9); // inverted range returns min

    constexpr auto kMin = std::numeric_limits<std::int32_t>::min();
    constexpr auto kMax = std::numeric_limits<std::int32_t>::max();
    for (int i = 0; i < 100; ++i) {
        const std::int32_t value = rng.NextInRange(kMin, kMax); // full span must not overflow
        CHECK(value >= kMin);
        CHECK(value <= kMax);
    }
}

TEST_CASE("MakeStream: deterministic, and labels and seeds give different streams") {
    CHECK(MakeStream(1, "a").Next() == MakeStream(1, "a").Next());
    CHECK(MakeStream(1, "a").Next() != MakeStream(1, "b").Next());
    CHECK(MakeStream(1, "a").Next() != MakeStream(2, "a").Next());
    static_assert(Fnv1a64("") == 0xcbf29ce484222325ULL);
    static_assert(Fnv1a64("a") == 0xaf63dc4c8601ec8cULL);
}

// --- Generation --------------------------------------------------------------

TEST_CASE("Generate: same seed, same catalog, same bytes") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    for (const std::uint64_t seed : {0ULL, 1ULL, 12345ULL, 0xffffffffffffffffULL}) {
        const AppearanceSpec first = Generate(catalog, seed);
        const AppearanceSpec second = Generate(catalog, seed);
        CHECK(first == second);
        CHECK(SerializeAppearance(first) == SerializeAppearance(second));
        // A fresh but identical catalog gives the same result (no hidden state).
        CHECK(Generate(MakeTestCatalog(), seed) == first);
    }
}

TEST_CASE("Generate: the result is a complete, valid spec") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    for (std::uint64_t seed = 0; seed < 1500; ++seed) {
        const AppearanceSpec spec = Generate(catalog, seed);
        CHECK(ValidateAppearance(spec, catalog).empty());
        CHECK(spec.parameters.size() == catalog.Parameters().size());
        CHECK(spec.layers.size() <= kMaxAppearanceLayers);
        CHECK(spec.clothing.contains(ClothingSlot::Torso));
        CHECK(spec.clothing.contains(ClothingSlot::Legs));
        CHECK(spec.clothing.contains(ClothingSlot::Feet));
        // Round trip through the save format.
        const auto loaded = DeserializeAppearance(SerializeAppearance(spec));
        REQUIRE(loaded.HasValue());
        CHECK(loaded.Value() == spec);
    }
}

TEST_CASE("Generate: different seeds give varied characters") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    std::set<std::string> distinct;
    std::set<ParamValue> jawValues;
    for (std::uint64_t seed = 0; seed < 200; ++seed) {
        const AppearanceSpec spec = Generate(catalog, seed);
        distinct.insert(SerializeAppearance(spec));
        jawValues.insert(spec.parameters.at(Id("base:face.jaw_width")));
    }
    CHECK(distinct.size() == 200);
    CHECK(jawValues.size() > 100);
}

TEST_CASE("Generate: parameter values are bell-shaped around the range centre") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    constexpr int kSamples = 2000;
    std::int64_t sum = 0;
    int nearCentre = 0;
    for (std::uint64_t seed = 0; seed < kSamples; ++seed) {
        const ParamValue value = Generate(catalog, seed).parameters.at(Id("base:face.jaw_width"));
        sum += value;
        nearCentre += (value > -333 && value < 333) ? 1 : 0;
    }
    CHECK(std::abs(sum / kSamples) < 60); // mean near 0 for [-1000, 1000]
    CHECK(nearCentre > kSamples * 2 / 3); // a uniform draw would give only 1/3
}

TEST_CASE("Generate: extending the catalog never changes existing parameter values") {
    AppearanceCatalog extended = MakeTestCatalog();
    REQUIRE_FALSE(extended
                      .AddParameter(spn::testing::P(Id("mod_a:face.horn_size"),
                                                    ParameterGroup::Custom, 0, 100, 0))
                      .has_value());
    REQUIRE_FALSE(extended
                      .AddParameter(spn::testing::P(Id("aaa:first.in_order"),
                                                    ParameterGroup::Custom, 0, 9, 0))
                      .has_value());
    const AppearanceCatalog base = MakeTestCatalog();
    for (std::uint64_t seed = 0; seed < 100; ++seed) {
        const AppearanceSpec before = Generate(base, seed);
        const AppearanceSpec after = Generate(extended, seed);
        REQUIRE(after.parameters.size() == before.parameters.size() + 2);
        for (const auto& [id, value] : before.parameters) {
            CHECK(after.parameters.at(id) == value);
        }
        CHECK(ValidateAppearance(after, extended).empty());
    }
}

TEST_CASE("Generate: optional content may be absent from the catalog") {
    AppearanceCatalog bare;
    REQUIRE_FALSE(bare.AddContent(spn::testing::C(Id("base:body.only"), ContentKind::BodyProvider))
                      .has_value());
    const AppearanceSpec spec = Generate(bare, 7);
    CHECK(spec.bodyProvider == Id("base:body.only"));
    CHECK(spec.hairStyle.IsEmpty());
    CHECK(spec.facialHairStyle.IsEmpty());
    CHECK(spec.parameters.empty());
    CHECK(spec.clothing.empty());
    CHECK(spec.layers.empty());
}

TEST_CASE("Generate: options steer the optional parts") {
    const AppearanceCatalog catalog = MakeTestCatalog();

    GenerationOptions none;
    none.baldChance = 1000;
    none.facialHairChance = 0;
    none.tattooChance = 0;
    none.scarChance = 0;
    GenerationOptions all;
    all.baldChance = 0;
    all.facialHairChance = 1000;
    all.tattooChance = 1000;
    all.scarChance = 1000;
    all.maxTattoos = 3;
    all.maxScars = 2;

    std::size_t maxTattoos = 0;
    for (std::uint64_t seed = 0; seed < 300; ++seed) {
        const AppearanceSpec sparse = Generate(catalog, seed, none);
        CHECK(sparse.hairStyle.IsEmpty());
        CHECK(sparse.facialHairStyle.IsEmpty());
        CHECK(sparse.layers.empty());

        const AppearanceSpec full = Generate(catalog, seed, all);
        CHECK_FALSE(full.hairStyle.IsEmpty());
        CHECK_FALSE(full.facialHairStyle.IsEmpty());
        std::size_t tattoos = 0;
        std::size_t scars = 0;
        for (const AppearanceLayer& layer : full.layers) {
            tattoos += layer.kind == LayerKind::Tattoo ? 1 : 0;
            scars += layer.kind == LayerKind::Scar ? 1 : 0;
            CHECK(layer.opacity >= (layer.kind == LayerKind::Tattoo ? 700 : 400));
        }
        CHECK(tattoos >= 1);
        CHECK(tattoos <= 3);
        CHECK(scars >= 1);
        CHECK(scars <= 2);
        maxTattoos = std::max(maxTattoos, tattoos);
        CHECK(ValidateAppearance(full, catalog).empty());
    }
    CHECK(maxTattoos == 3);
}

TEST_CASE("Generate: invalid options and empty catalogs are reported") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const auto fails = [&](const GenerationOptions& options, std::string_view subject) {
        const auto result = GenerateAppearance(catalog, 1, options);
        REQUIRE_FALSE(result.HasValue());
        CHECK(result.Error().code == AppearanceErrorCode::InvalidGenerationOption);
        CHECK(result.Error().subject == subject);
    };
    GenerationOptions options;
    options.baldChance = -1;
    fails(options, "baldChance");
    options = {};
    options.facialHairChance = 1001;
    fails(options, "facialHairChance");
    options = {};
    options.tattooChance = 5000;
    fails(options, "tattooChance");
    options = {};
    options.scarChance = -50;
    fails(options, "scarChance");
    options = {};
    options.maxTattoos = static_cast<std::uint32_t>(kMaxAppearanceLayers) + 1;
    fails(options, "maxLayers");

    const auto empty = GenerateAppearance(AppearanceCatalog{}, 1);
    REQUIRE_FALSE(empty.HasValue());
    CHECK(empty.Error().code == AppearanceErrorCode::EmptyCatalog);
    CHECK(Describe(empty.Error()) == "empty catalog: body_provider");

    // Zero counts are legal and simply produce no layers.
    options = {};
    options.maxTattoos = 0;
    options.maxScars = 0;
    options.tattooChance = 1000;
    options.scarChance = 1000;
    CHECK(Generate(catalog, 3, options).layers.empty());
}

TEST_CASE("Generate: the algorithm is frozen per generator version (golden)") {
    static_assert(kAppearanceGeneratorVersion == 1,
                  "Generator version changed: add a golden for the new version and decide "
                  "what happens to characters generated by version 1.");
    const AppearanceCatalog catalog = MakeTestCatalog();
    CHECK(SerializeAppearance(Generate(catalog, 12345)) == kExpectedSeed12345);
    CHECK(SerializeAppearance(Generate(catalog, 0)) == kExpectedSeed0);
}

TEST_CASE("Generate: generation shapes steer the distribution of a parameter") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    constexpr int kSamples = 3000;
    std::int64_t freckleSum = 0;
    int freckleHigh = 0;
    ParamValue freckleMax = 0;
    std::array<int, 3> irisThirds{};
    for (std::uint64_t seed = 0; seed < kSamples; ++seed) {
        const AppearanceSpec spec = Generate(catalog, seed);
        const ParamValue freckles = spec.parameters.at(Id("base:skin.freckles"));
        CHECK(freckles >= 0);
        CHECK(freckles <= 1000);
        freckleSum += freckles;
        freckleHigh += freckles > 700 ? 1 : 0;
        freckleMax = std::max(freckleMax, freckles);
        ++irisThirds.at(static_cast<std::size_t>(
            std::min(2, spec.parameters.at(Id("base:eyes.iris_hue")) / 334)));
    }
    // TowardDefault: clustered at the default 0 but the far end is reachable.
    CHECK(freckleSum / kSamples < 300);
    CHECK(freckleHigh > 0);
    CHECK(freckleHigh < kSamples / 6);
    CHECK(freckleMax > 900);
    // Uniform: each third of the range gets roughly a third of the draws.
    for (const int count : irisThirds) {
        CHECK(count > kSamples / 4);
        CHECK(count < kSamples * 5 / 12);
    }
}

TEST_CASE(
    "Generate: TowardDefault works for defaults inside, at either end of, and without a range") {
    const auto drawn = [](const ParameterDef& def) {
        AppearanceCatalog catalog;
        REQUIRE_FALSE(
            catalog.AddContent(spn::testing::C(Id("base:body.only"), ContentKind::BodyProvider))
                .has_value());
        REQUIRE_FALSE(catalog.AddParameter(def).has_value());
        std::set<ParamValue> values;
        for (std::uint64_t seed = 0; seed < 400; ++seed) {
            const ParamValue value = Generate(catalog, seed).parameters.at(def.id);
            CHECK(value >= def.min);
            CHECK(value <= def.max);
            values.insert(value);
        }
        return values;
    };
    ParameterDef inside =
        spn::testing::P(Id("mod_a:x.inside"), ParameterGroup::Custom, -500, 500, 100);
    inside.shape = GenerationShape::TowardDefault;
    const auto insideValues = drawn(inside);
    CHECK(*insideValues.begin() < 100); // both sides are used
    CHECK(*insideValues.rbegin() > 100);

    ParameterDef top = spn::testing::P(Id("mod_a:x.top"), ParameterGroup::Custom, 0, 50, 50);
    top.shape = GenerationShape::TowardDefault;
    CHECK(*drawn(top).rbegin() == 50);

    ParameterDef fixed = spn::testing::P(Id("mod_a:x.fixed"), ParameterGroup::Custom, 7, 7, 7);
    fixed.shape = GenerationShape::TowardDefault;
    CHECK(drawn(fixed) == std::set<ParamValue>{7});
    fixed.shape = GenerationShape::Uniform;
    fixed.id = Id("mod_a:x.fixed_uniform");
    CHECK(drawn(fixed) == std::set<ParamValue>{7});
}

TEST_CASE("Generate: only parameters and content that fit the body provider are generated") {
    const AppearanceCatalog catalog = MakeProviderCatalog();
    std::size_t bodyA = 0;
    std::size_t bodyB = 0;
    std::size_t hatsOnA = 0;
    for (std::uint64_t seed = 0; seed < 600; ++seed) {
        const AppearanceSpec spec = Generate(catalog, seed);
        CHECK(ValidateAppearance(spec, catalog).empty());
        const bool isA = spec.bodyProvider == Id("base:body.placeholder_a");
        (isA ? bodyA : bodyB) += 1;
        // The horn parameter exists only for body B.
        CHECK(spec.parameters.contains(Id("mod_a:body.horn_size")) == !isA);
        const auto head = spec.clothing.find(ClothingSlot::Head);
        if (head != spec.clothing.end() && head->second == Id("mod_a:cloth.hat_for_a")) {
            CHECK(isA);
            ++hatsOnA;
        }
    }
    CHECK(bodyA > 100);
    CHECK(bodyB > 100);
    CHECK(hatsOnA > 0);
}

TEST_CASE("Generate: unrelated parameters of a provider-restricted catalog stay stable") {
    // Restricting/adding parameters for one body must not shift values of shared ones.
    const AppearanceCatalog plain = MakeTestCatalog();
    const AppearanceCatalog restricted = MakeProviderCatalog();
    for (std::uint64_t seed = 0; seed < 100; ++seed) {
        const AppearanceSpec a = Generate(plain, seed);
        const AppearanceSpec b = Generate(restricted, seed);
        for (const auto& [id, value] : a.parameters) {
            CHECK(b.parameters.at(id) == value);
        }
    }
}
