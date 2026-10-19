#include "AppearanceTestCatalog.h"

#include <doctest.h>
#include <spn/core/Appearance.h>
#include <spn/core/AppearanceRecipe.h>
#include <string>
#include <string_view>

using namespace spn::core;
using spn::testing::Id;
using spn::testing::MakeTestCatalog;

namespace {

std::string Tabs(std::string text) {
    for (std::size_t pos = text.find("<T>"); pos != std::string::npos;
         pos = text.find("<T>", pos)) {
        text.replace(pos, 3, "\t");
    }
    return text;
}

/// A recipe using every kind of override.
AppearanceRecipe MakeFullRecipe() {
    AppearanceRecipe recipe;
    recipe.seed = 42;
    recipe.catalogFingerprint = 1234567890123ULL;
    AppearanceOverrides& ov = recipe.overrides;
    ov.bodyProvider = Id("base:body.placeholder_b");
    ov.hairStyle = ContentId{};
    ov.facialHairStyle = Id("base:beard.placeholder_full");
    ov.parameters.emplace(Id("base:face.jaw_width"), 123);
    ov.parameters.emplace(Id("base:skin.tone"), 777);
    ov.clothing.emplace(ClothingSlot::Torso, ContentId{});
    ov.clothing.emplace(ClothingSlot::Feet, Id("base:cloth.placeholder_boots"));
    ov.layers = std::vector<AppearanceLayer>{
        {LayerKind::Tattoo, Id("base:tattoo.placeholder_band"), BodyRegion::Neck, 650},
        {LayerKind::Scar, Id("base:scar.placeholder_burn"), BodyRegion::Head, 450}};
    return recipe;
}

const std::string& FullRecipeText() {
    static const std::string kText =
        Tabs("SPNRECIPE 1\n"
             "seed<T>42\n"
             "generator<T>1\n"
             "catalog<T>1234567890123\n"
             "options<T>50<T>250<T>100<T>150<T>3<T>2\n"
             "body<T>base:body.placeholder_b\n"
             "hair<T>-\n"
             "facial_hair<T>base:beard.placeholder_full\n"
             "param<T>base:face.jaw_width<T>123\n"
             "param<T>base:skin.tone<T>777\n"
             "clothing<T>torso<T>-\n"
             "clothing<T>feet<T>base:cloth.placeholder_boots\n"
             "layers<T>2\n"
             "layer<T>tattoo<T>base:tattoo.placeholder_band<T>neck<T>650\n"
             "layer<T>scar<T>base:scar.placeholder_burn<T>head<T>450\n"
             "end<T>14\n");
    return kText;
}

AppearanceSpec Resolve(const AppearanceCatalog& catalog, const AppearanceRecipe& recipe,
                       FingerprintPolicy policy = FingerprintPolicy::Require) {
    auto result = ResolveRecipe(catalog, recipe, policy);
    REQUIRE_MESSAGE(result.HasValue(), "resolve failed: ", Describe(result.Error()));
    return std::move(result).Value();
}

AppearanceError ResolveError(const AppearanceCatalog& catalog, const AppearanceRecipe& recipe,
                             FingerprintPolicy policy = FingerprintPolicy::Require) {
    const auto result = ResolveRecipe(catalog, recipe, policy);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

AppearanceError LoadError(std::string_view text) {
    const auto result = DeserializeRecipe(text);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

} // namespace

// --- Resolving ---------------------------------------------------------------

TEST_CASE("Recipe: without overrides it resolves to the generated character") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const AppearanceRecipe recipe = MakeRecipe(catalog, 777);
    CHECK(recipe.generatorVersion == kAppearanceGeneratorVersion);
    REQUIRE(recipe.catalogFingerprint.has_value());
    CHECK(recipe.catalogFingerprint.value_or(0) == catalog.Fingerprint());
    CHECK(recipe.overrides.IsEmpty());

    auto generated = GenerateAppearance(catalog, 777);
    REQUIRE(generated.HasValue());
    CHECK(Resolve(catalog, recipe) == generated.Value());
    CHECK(Resolve(catalog, recipe) == Resolve(catalog, recipe)); // reproducible
}

TEST_CASE("Recipe: generation options are part of the recipe") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceRecipe recipe = MakeRecipe(catalog, 5);
    recipe.options.baldChance = 1000;
    recipe.options.tattooChance = 0;
    recipe.options.scarChance = 0;
    const AppearanceSpec spec = Resolve(catalog, recipe);
    CHECK(spec.hairStyle.IsEmpty());
    CHECK(spec.layers.empty());
}

TEST_CASE("Recipe: overrides replace exactly what they name") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceRecipe recipe = MakeFullRecipe();
    const AppearanceSpec base =
        Resolve(catalog, MakeRecipe(catalog, 42), FingerprintPolicy::Ignore);

    const AppearanceSpec spec = Resolve(catalog, recipe, FingerprintPolicy::Ignore);
    CHECK(spec.bodyProvider == Id("base:body.placeholder_b"));
    CHECK(spec.hairStyle.IsEmpty());
    CHECK(spec.facialHairStyle == Id("base:beard.placeholder_full"));
    CHECK(spec.parameters.at(Id("base:face.jaw_width")) == 123);
    CHECK(spec.parameters.at(Id("base:skin.tone")) == 777);
    CHECK_FALSE(spec.clothing.contains(ClothingSlot::Torso));
    CHECK(spec.clothing.at(ClothingSlot::Feet) == Id("base:cloth.placeholder_boots"));
    REQUIRE(spec.layers.size() == 2);
    CHECK(spec.layers[0].content == Id("base:tattoo.placeholder_band"));

    // Untouched parameters keep the seed's values.
    CHECK(spec.parameters.at(Id("base:body.height")) == base.parameters.at(Id("base:body.height")));
    CHECK(spec.parameters.size() == base.parameters.size());

    // An empty-but-present layer list removes all generated layers.
    recipe.overrides.layers = std::vector<AppearanceLayer>{};
    CHECK(Resolve(catalog, recipe, FingerprintPolicy::Ignore).layers.empty());
}

TEST_CASE("Recipe: invalid overrides are reported by validation") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceRecipe recipe = MakeRecipe(catalog, 1);
    recipe.overrides.parameters.emplace(Id("base:skin.tone"), 4000);
    const AppearanceError error = ResolveError(catalog, recipe);
    CHECK(error.code == AppearanceErrorCode::ParameterOutOfRange);
    CHECK(error.subject == "base:skin.tone");

    recipe = MakeRecipe(catalog, 1);
    recipe.overrides.hairStyle = Id("mod_gone:hair.x");
    CHECK(ResolveError(catalog, recipe).code == AppearanceErrorCode::UnknownContent);

    recipe = MakeRecipe(catalog, 1);
    recipe.overrides.bodyProvider = ContentId{};
    CHECK(ResolveError(catalog, recipe).code == AppearanceErrorCode::MissingBodyProvider);
}

TEST_CASE("Recipe: generator version and catalog changes are detected") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceRecipe recipe = MakeRecipe(catalog, 9);

    recipe.generatorVersion = kAppearanceGeneratorVersion + 1;
    const AppearanceError version = ResolveError(catalog, recipe);
    CHECK(version.code == AppearanceErrorCode::UnsupportedGeneratorVersion);
    CHECK(version.subject == std::to_string(kAppearanceGeneratorVersion + 1));
    // Not even Ignore helps: a different algorithm cannot be reproduced.
    CHECK(ResolveError(catalog, recipe, FingerprintPolicy::Ignore).code ==
          AppearanceErrorCode::UnsupportedGeneratorVersion);

    // The catalog gains a mod hair style after the NPC was created.
    recipe = MakeRecipe(catalog, 9);
    AppearanceCatalog extended = MakeTestCatalog();
    REQUIRE_FALSE(
        extended.AddContent(spn::testing::C(Id("mod_a:hair.horns"), ContentKind::HairStyle))
            .has_value());
    CHECK(ResolveError(extended, recipe).code == AppearanceErrorCode::CatalogChanged);
    CHECK(Resolve(extended, recipe, FingerprintPolicy::Ignore).bodyProvider.Value().size() > 0);

    // A recipe without a recorded fingerprint always resolves.
    recipe.catalogFingerprint.reset();
    CHECK(ResolveAppearance(extended, AppearanceSource{recipe}).HasValue());
}

TEST_CASE("Source: a spec is validated, a recipe is resolved") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    auto generated = GenerateAppearance(catalog, 3);
    REQUIRE(generated.HasValue());

    const AppearanceSource spec{generated.Value()};
    const auto resolvedSpec = ResolveAppearance(catalog, spec);
    REQUIRE(resolvedSpec.HasValue());
    CHECK(resolvedSpec.Value() == generated.Value());

    const AppearanceSource recipe{MakeRecipe(catalog, 3)};
    const auto resolvedRecipe = ResolveAppearance(catalog, recipe);
    REQUIRE(resolvedRecipe.HasValue());
    CHECK(resolvedRecipe.Value() == generated.Value());

    AppearanceSpec broken = generated.Value();
    broken.bodyProvider = Id("mod_gone:body.x");
    CHECK(ResolveAppearance(catalog, AppearanceSource{broken}).Error().code ==
          AppearanceErrorCode::UnknownContent);
}

TEST_CASE("Recipe: a crowd of recipes is far smaller than the full specs") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    std::size_t recipeBytes = 0;
    std::size_t specBytes = 0;
    for (std::uint64_t seed = 0; seed < 50; ++seed) {
        recipeBytes += SerializeRecipe(MakeRecipe(catalog, seed)).size();
        specBytes += SerializeAppearance(Resolve(catalog, MakeRecipe(catalog, seed))).size();
    }
    CHECK(recipeBytes * 4 < specBytes);
}

// --- Serialization -------------------------------------------------------------

TEST_CASE("Recipe: canonical text is stable (golden)") {
    CHECK(SerializeRecipe(MakeFullRecipe()) == FullRecipeText());

    const AppearanceRecipe minimal;
    CHECK(SerializeRecipe(minimal) == Tabs("SPNRECIPE 1\nseed<T>0\ngenerator<T>1\ncatalog<T>-\n"
                                           "options<T>50<T>250<T>100<T>150<T>3<T>2\nend<T>4\n"));
}

TEST_CASE("Recipe: round trip restores the recipe exactly") {
    const auto loaded = DeserializeRecipe(FullRecipeText());
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == MakeFullRecipe());
    CHECK(SerializeRecipe(loaded.Value()) == FullRecipeText());

    const AppearanceRecipe minimal;
    const auto again = DeserializeRecipe(SerializeRecipe(minimal));
    REQUIRE(again.HasValue());
    CHECK(again.Value() == minimal);

    // Large seeds, and a present-but-empty layer list, survive.
    AppearanceRecipe edge;
    edge.seed = UINT64_MAX;
    edge.catalogFingerprint = UINT64_MAX;
    edge.overrides.layers = std::vector<AppearanceLayer>{};
    const auto edgeLoaded = DeserializeRecipe(SerializeRecipe(edge));
    REQUIRE(edgeLoaded.HasValue());
    CHECK(edgeLoaded.Value() == edge);
}

TEST_CASE("Recipe: a resolved character is identical after save and load") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceRecipe recipe = MakeRecipe(catalog, 2024);
    recipe.overrides.parameters.emplace(Id("base:body.height"), 300);
    recipe.overrides.clothing.emplace(ClothingSlot::Head, Id("base:cloth.placeholder_cap"));
    const auto loaded = DeserializeRecipe(SerializeRecipe(recipe));
    REQUIRE(loaded.HasValue());
    CHECK(Resolve(catalog, loaded.Value()) == Resolve(catalog, recipe));
}

TEST_CASE("Recipe: rejects bad headers, versions and truncation") {
    CHECK(LoadError("").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNRECIPE").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNRECIPE 1").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNRECIPE 2\n") ==
          AppearanceError{AppearanceErrorCode::UnsupportedVersion, "", 1});
    CHECK(LoadError("SPNRECIPE x\n").code == AppearanceErrorCode::InvalidHeader);
    CHECK(LoadError("SPNAPPEARANCE 1\n").code == AppearanceErrorCode::InvalidHeader);

    const std::string& text = FullRecipeText();
    for (std::size_t length = 0; length < text.size(); ++length) {
        CHECK_FALSE(DeserializeRecipe(std::string_view(text).substr(0, length)).HasValue());
    }
    CHECK(LoadError(text + "x\n") == AppearanceError{AppearanceErrorCode::TrailingData, "", 17});
}

TEST_CASE("Recipe: rejects malformed, misordered and non-canonical lines") {
    const std::string head = Tabs(
        "SPNRECIPE 1\nseed<T>1\ngenerator<T>1\ncatalog<T>-\noptions<T>1<T>2<T>3<T>4<T>5<T>6\n");
    const auto code = [&](const std::string& body, int count) {
        return LoadError(head + Tabs(body) + "end\t" + std::to_string(count) + '\n').code;
    };

    CHECK(DeserializeRecipe(head + "end\t4\n").HasValue());
    CHECK(code("bogus\n", 5) == AppearanceErrorCode::MalformedLine);
    CHECK(code("hair<T>-<T>x\n", 5) == AppearanceErrorCode::MalformedLine);
    CHECK(code("param<T>base:skin.tone\n", 5) == AppearanceErrorCode::MalformedLine);

    // Fixed lines in the wrong order or with bad values.
    CHECK(LoadError(
              "SPNRECIPE 1\ngenerator\t1\nseed\t1\ncatalog\t-\noptions\t1\t2\t3\t4\t5\t6\nend\t4\n")
              .code == AppearanceErrorCode::MalformedLine);
    CHECK(
        LoadError(
            "SPNRECIPE 1\nseed\t-1\ngenerator\t1\ncatalog\t-\noptions\t1\t2\t3\t4\t5\t6\nend\t4\n")
            .code == AppearanceErrorCode::InvalidToken);
    CHECK(
        LoadError(
            "SPNRECIPE 1\nseed\t01\ngenerator\t1\ncatalog\t-\noptions\t1\t2\t3\t4\t5\t6\nend\t4\n")
            .code == AppearanceErrorCode::InvalidToken);
    CHECK(LoadError(
              "SPNRECIPE 1\nseed\t1\ngenerator\t1\ncatalog\tx\noptions\t1\t2\t3\t4\t5\t6\nend\t4\n")
              .code == AppearanceErrorCode::InvalidToken);
    CHECK(
        LoadError(
            "SPNRECIPE 1\nseed\t1\ngenerator\t1\ncatalog\t-\noptions\t1\t2\t3\t4\t5\t-6\nend\t4\n")
            .code == AppearanceErrorCode::InvalidToken);
    CHECK(LoadError("SPNRECIPE 1\nseed\t18446744073709551616\ngenerator\t1\ncatalog\t-\n"
                    "options\t1\t2\t3\t4\t5\t6\nend\t4\n")
              .code == AppearanceErrorCode::InvalidToken);

    // Override lines: order, duplicates, tokens.
    CHECK(code("hair<T>-\nbody<T>base:body.placeholder_a\n", 6) ==
          AppearanceErrorCode::NonCanonical);
    CHECK(code("hair<T>-\nhair<T>-\n", 6) == AppearanceErrorCode::NonCanonical);
    CHECK(code("body<T>-\n", 5) == AppearanceErrorCode::InvalidToken);
    CHECK(code("hair<T>BAD\n", 5) == AppearanceErrorCode::InvalidToken);
    CHECK(code("param<T>base:skin.tone<T>1\nhair<T>-\n", 6) == AppearanceErrorCode::NonCanonical);
    CHECK(code("param<T>base:skin.tone<T>1\nparam<T>base:skin.tone<T>2\n", 6) ==
          AppearanceErrorCode::NonCanonical);
    CHECK(code("param<T>base:skin.tone<T>1\nparam<T>base:body.height<T>2\n", 6) ==
          AppearanceErrorCode::NonCanonical);
    CHECK(code("param<T>base:skin.tone<T>007\n", 5) == AppearanceErrorCode::InvalidToken);
    CHECK(code("clothing<T>legs<T>-\nclothing<T>torso<T>-\n", 6) ==
          AppearanceErrorCode::NonCanonical);
    CHECK(code("clothing<T>neck<T>-\n", 5) == AppearanceErrorCode::InvalidToken);
    CHECK(code("clothing<T>torso<T>-\nparam<T>base:skin.tone<T>1\n", 6) ==
          AppearanceErrorCode::NonCanonical);
}

TEST_CASE("Recipe: layer blocks must announce and deliver the right number of lines") {
    const std::string head = Tabs(
        "SPNRECIPE 1\nseed<T>1\ngenerator<T>1\ncatalog<T>-\noptions<T>1<T>2<T>3<T>4<T>5<T>6\n");
    const std::string layer = Tabs("layer<T>tattoo<T>base:tattoo.placeholder_band<T>neck<T>650\n");
    const auto load = [&](const std::string& body, int count) {
        return head + body + "end\t" + std::to_string(count) + '\n';
    };

    CHECK(DeserializeRecipe(load("layers\t1\n" + layer, 6)).HasValue());
    CHECK(DeserializeRecipe(load("layers\t0\n", 5)).HasValue());
    CHECK(LoadError(load("layers\t2\n" + layer, 6)).code == AppearanceErrorCode::CountMismatch);
    CHECK(LoadError(load("layers\t1\n" + layer + layer, 7)).code ==
          AppearanceErrorCode::CountMismatch);
    CHECK(LoadError(load(layer, 5)).code == AppearanceErrorCode::NonCanonical);
    CHECK(LoadError(load("layers\t1\nhair\t-\n" + layer, 7)).code ==
          AppearanceErrorCode::CountMismatch);
    CHECK(LoadError(load("layers\t1\n" + layer + "layers\t0\n", 7)).code ==
          AppearanceErrorCode::NonCanonical);
    CHECK(LoadError(load("layers\t33\n", 5)).code == AppearanceErrorCode::TooManyLayers);
    CHECK(LoadError(load("layers\tx\n", 5)).code == AppearanceErrorCode::InvalidToken);
    CHECK(LoadError(load("layers\t1\nlayer\tstamp\tbase:tattoo.placeholder_band\tneck\t1\n", 6))
              .code == AppearanceErrorCode::InvalidToken);
}

TEST_CASE("Recipe: wrong line counts are rejected") {
    std::string text = FullRecipeText();
    text.replace(text.rfind("14"), 2, "13");
    CHECK(LoadError(text).code == AppearanceErrorCode::CountMismatch);
}

TEST_CASE("Recipe: any single-byte corruption is rejected or re-serializes identically") {
    const std::string& text = FullRecipeText();
    constexpr std::string_view kReplacements = "X\t\n:-0.";
    std::size_t rejected = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        for (const char replacement : kReplacements) {
            if (text[i] == replacement) {
                continue;
            }
            std::string mutated = text;
            mutated[i] = replacement;
            const auto result = DeserializeRecipe(mutated);
            if (result.HasValue()) {
                CHECK(SerializeRecipe(result.Value()) == mutated);
            } else {
                ++rejected;
            }
        }
    }
    CHECK(rejected > 0);
}

TEST_CASE("Recipe: limits protect the loader") {
    std::string text = Tabs("SPNRECIPE 1\nseed<T>1\ngenerator<T>1\ncatalog<T>-\n"
                            "options<T>1<T>2<T>3<T>4<T>5<T>6\n");
    for (std::size_t i = 0; i <= kMaxAppearanceParameters; ++i) {
        std::string suffix = std::to_string(i);
        for (char& ch : suffix) {
            ch = static_cast<char>('a' + (ch - '0')); // digits -> letters keeps ids valid
        }
        text += "param\tx:p" + std::string(4 - std::min<std::size_t>(4, suffix.size()), 'a') +
                suffix + "\t1\n";
    }
    text += "end\t" + std::to_string(kMaxAppearanceParameters + 1 + 4) + "\n";
    const AppearanceError error = LoadError(text);
    CHECK(error.code == AppearanceErrorCode::TooManyParameters);
}

// --- Either form ------------------------------------------------------------------

TEST_CASE("Source: serialization picks the format by header") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const AppearanceSource recipe{MakeFullRecipe()};
    auto generated = GenerateAppearance(catalog, 11);
    REQUIRE(generated.HasValue());
    const AppearanceSource spec{generated.Value()};

    const auto loadedRecipe = DeserializeSource(SerializeSource(recipe));
    REQUIRE(loadedRecipe.HasValue());
    CHECK(loadedRecipe.Value() == recipe);
    CHECK(std::holds_alternative<AppearanceRecipe>(loadedRecipe.Value()));

    const auto loadedSpec = DeserializeSource(SerializeSource(spec));
    REQUIRE(loadedSpec.HasValue());
    CHECK(loadedSpec.Value() == spec);
    CHECK(std::holds_alternative<AppearanceSpec>(loadedSpec.Value()));

    CHECK_FALSE(DeserializeSource("").HasValue());
    CHECK_FALSE(DeserializeSource("garbage\n").HasValue());
    CHECK(DeserializeSource("SPNRECIPE 2\n").Error().code ==
          AppearanceErrorCode::UnsupportedVersion);
}
