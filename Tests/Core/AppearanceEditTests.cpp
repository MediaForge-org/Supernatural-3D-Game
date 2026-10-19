#include "AppearanceTestCatalog.h"

#include <doctest.h>
#include <spn/core/Appearance.h>
#include <spn/core/AppearanceEdit.h>
#include <string>

using namespace spn::core;
using spn::testing::Id;
using spn::testing::MakeProviderCatalog;
using spn::testing::MakeTestCatalog;

namespace {

AppearanceSpec BaseSpec() {
    AppearanceSpec spec;
    spec.bodyProvider = Id("base:body.placeholder_a");
    spec.hairStyle = Id("base:hair.placeholder_short");
    spec.parameters.emplace(Id("base:skin.tone"), 400);
    spec.clothing.emplace(ClothingSlot::Torso, Id("base:cloth.placeholder_shirt"));
    spec.layers.push_back(
        {LayerKind::Tattoo, Id("base:tattoo.placeholder_anchor"), BodyRegion::Torso, 800});
    spec.layers.push_back(
        {LayerKind::Scar, Id("base:scar.placeholder_slash"), BodyRegion::Face, 500});
    spec.layers.push_back(
        {LayerKind::Tattoo, Id("base:tattoo.placeholder_sigil"), BodyRegion::Back, 600});
    return spec;
}

AppearanceSpec Edited(const AppearanceSpec& spec, const AppearanceCatalog& catalog,
                      const AppearanceEdit& edit) {
    auto result = ApplyEdit(spec, catalog, edit);
    REQUIRE_MESSAGE(result.HasValue(), "edit failed: ", Describe(result.Error()));
    return std::move(result).Value();
}

AppearanceError EditError(const AppearanceSpec& spec, const AppearanceCatalog& catalog,
                          const AppearanceEdit& edit) {
    const auto result = ApplyEdit(spec, catalog, edit);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

std::vector<std::string> LayerIds(const AppearanceSpec& spec) {
    std::vector<std::string> ids;
    ids.reserve(spec.layers.size());
    for (const AppearanceLayer& layer : spec.layers) {
        ids.push_back(layer.content.Value());
    }
    return ids;
}

} // namespace

TEST_CASE("Edit: parameters can be set and reset") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceSpec spec =
        Edited(BaseSpec(), catalog, SetParameter{Id("base:face.jaw_width"), -300});
    CHECK(spec.parameters.at(Id("base:face.jaw_width")) == -300);
    spec = Edited(spec, catalog, SetParameter{Id("base:face.jaw_width"), 250});
    CHECK(spec.parameters.at(Id("base:face.jaw_width")) == 250);
    spec = Edited(spec, catalog, ResetParameter{Id("base:face.jaw_width")});
    CHECK_FALSE(spec.parameters.contains(Id("base:face.jaw_width")));
    // Resetting something that is not set is accepted.
    CHECK(Edited(spec, catalog, ResetParameter{Id("base:face.jaw_width")}) == spec);
}

TEST_CASE("Edit: invalid parameter edits are rejected with the validation error") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const AppearanceSpec spec = BaseSpec();
    const AppearanceError range =
        EditError(spec, catalog, SetParameter{Id("base:skin.tone"), 1001});
    CHECK(range.code == AppearanceErrorCode::ParameterOutOfRange);
    CHECK(range.subject == "base:skin.tone");
    CHECK(EditError(spec, catalog, SetParameter{Id("mod_x:nope"), 1}).code ==
          AppearanceErrorCode::UnknownParameter);
}

TEST_CASE("Edit: hair, facial hair and clothing") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    AppearanceSpec spec =
        Edited(BaseSpec(), catalog, SetHairStyle{Id("base:hair.placeholder_long")});
    CHECK(spec.hairStyle == Id("base:hair.placeholder_long"));
    spec = Edited(spec, catalog, SetHairStyle{ContentId{}});
    CHECK(spec.hairStyle.IsEmpty());
    spec = Edited(spec, catalog, SetFacialHairStyle{Id("base:beard.placeholder_full")});
    CHECK(spec.facialHairStyle == Id("base:beard.placeholder_full"));
    spec =
        Edited(spec, catalog, SetClothing{ClothingSlot::Legs, Id("base:cloth.placeholder_jeans")});
    CHECK(spec.clothing.size() == 2);
    spec = Edited(spec, catalog, SetClothing{ClothingSlot::Torso, ContentId{}});
    CHECK_FALSE(spec.clothing.contains(ClothingSlot::Torso));

    CHECK(EditError(spec, catalog, SetHairStyle{Id("base:beard.placeholder_full")}).code ==
          AppearanceErrorCode::ContentKindMismatch);
    CHECK(
        EditError(spec, catalog, SetClothing{ClothingSlot::Feet, Id("base:cloth.placeholder_cap")})
            .code == AppearanceErrorCode::ClothingSlotMismatch);
}

TEST_CASE("Edit: layers can be added, removed, moved and faded") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const AppearanceSpec start = BaseSpec();
    const AppearanceLayer extra{LayerKind::Scar, Id("base:scar.placeholder_burn"),
                                BodyRegion::LeftArm, 300};

    AppearanceSpec spec = Edited(start, catalog, AddLayer{extra, std::nullopt});
    CHECK(spec.layers.size() == 4);
    CHECK(spec.layers.back() == extra); // default: on top

    spec = Edited(start, catalog, AddLayer{extra, 0});
    CHECK(spec.layers.front() == extra);
    spec = Edited(start, catalog, AddLayer{extra, 3}); // index == size appends
    CHECK(spec.layers.back() == extra);
    CHECK(EditError(start, catalog, AddLayer{extra, 4}).code ==
          AppearanceErrorCode::IndexOutOfRange);

    spec = Edited(start, catalog, RemoveLayer{1});
    CHECK(LayerIds(spec) == std::vector<std::string>{"base:tattoo.placeholder_anchor",
                                                     "base:tattoo.placeholder_sigil"});
    CHECK(EditError(start, catalog, RemoveLayer{3}).code == AppearanceErrorCode::IndexOutOfRange);

    spec = Edited(start, catalog, SetLayerOpacity{2, 0});
    CHECK(spec.layers[2].opacity == 0);
    CHECK(EditError(start, catalog, SetLayerOpacity{2, 1001}).code ==
          AppearanceErrorCode::OpacityOutOfRange);
    CHECK(EditError(start, catalog, SetLayerOpacity{3, 5}).code ==
          AppearanceErrorCode::IndexOutOfRange);

    // Duplicate (same asset, same region) is rejected.
    CHECK(EditError(start, catalog, AddLayer{start.layers[0], std::nullopt}).code ==
          AppearanceErrorCode::DuplicateLayer);
}

TEST_CASE("Edit: MoveLayer moves up, down and to the same place") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const AppearanceSpec start = BaseSpec(); // [anchor, slash, sigil]
    CHECK(LayerIds(Edited(start, catalog, MoveLayer{0, 2})) ==
          std::vector<std::string>{"base:scar.placeholder_slash", "base:tattoo.placeholder_sigil",
                                   "base:tattoo.placeholder_anchor"});
    CHECK(LayerIds(Edited(start, catalog, MoveLayer{2, 0})) ==
          std::vector<std::string>{"base:tattoo.placeholder_sigil",
                                   "base:tattoo.placeholder_anchor",
                                   "base:scar.placeholder_slash"});
    CHECK(LayerIds(Edited(start, catalog, MoveLayer{1, 1})) == LayerIds(start));
    CHECK(LayerIds(Edited(start, catalog, MoveLayer{0, 1})) ==
          std::vector<std::string>{"base:scar.placeholder_slash", "base:tattoo.placeholder_anchor",
                                   "base:tattoo.placeholder_sigil"});
    CHECK(EditError(start, catalog, MoveLayer{3, 0}).code == AppearanceErrorCode::IndexOutOfRange);
    CHECK(EditError(start, catalog, MoveLayer{0, 3}).code == AppearanceErrorCode::IndexOutOfRange);
}

TEST_CASE("Edit: changing the body provider respects provider compatibility") {
    const AppearanceCatalog catalog = MakeProviderCatalog();
    AppearanceSpec spec = BaseSpec();
    // Everything in BaseSpec fits both bodies.
    spec = Edited(spec, catalog, SetBodyProvider{Id("base:body.placeholder_b")});
    CHECK(spec.bodyProvider == Id("base:body.placeholder_b"));

    // Body B supports the horn parameter, body A does not.
    spec = Edited(spec, catalog, SetParameter{Id("mod_a:body.horn_size"), 600});
    CHECK(EditError(spec, catalog, SetBodyProvider{Id("base:body.placeholder_a")}).code ==
          AppearanceErrorCode::ParameterUnsupportedByProvider);
    spec = Edited(spec, catalog, ResetParameter{Id("mod_a:body.horn_size")});
    spec = Edited(spec, catalog, SetBodyProvider{Id("base:body.placeholder_a")});

    // The hat fits body A only.
    spec = Edited(spec, catalog, SetClothing{ClothingSlot::Head, Id("mod_a:cloth.hat_for_a")});
    CHECK(EditError(spec, catalog, SetBodyProvider{Id("base:body.placeholder_b")}).code ==
          AppearanceErrorCode::ContentIncompatibleWithProvider);

    CHECK(EditError(spec, catalog, SetBodyProvider{ContentId{}}).code ==
          AppearanceErrorCode::MissingBodyProvider);
    CHECK(EditError(spec, catalog, SetBodyProvider{Id("base:hair.placeholder_short")}).code ==
          AppearanceErrorCode::ContentKindMismatch);
}

TEST_CASE("Edit: a failed edit leaves the input untouched") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    const AppearanceSpec spec = BaseSpec();
    const std::string before = SerializeAppearance(spec);
    CHECK_FALSE(ApplyEdit(spec, catalog, SetParameter{Id("base:skin.tone"), -5}).HasValue());
    CHECK_FALSE(ApplyEdit(spec, catalog, RemoveLayer{99}).HasValue());
    CHECK(SerializeAppearance(spec) == before);
}

TEST_CASE("Editor: creation requires a valid spec") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    CHECK(AppearanceEditor::Create(catalog, BaseSpec()).HasValue());

    AppearanceSpec broken = BaseSpec();
    broken.hairStyle = Id("mod_missing:hair.x");
    const auto editor = AppearanceEditor::Create(catalog, broken);
    REQUIRE_FALSE(editor.HasValue());
    CHECK(editor.Error().code == AppearanceErrorCode::UnknownContent);
}

TEST_CASE("Editor: undo and redo walk the history") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    auto created = AppearanceEditor::Create(catalog, BaseSpec());
    REQUIRE(created.HasValue());
    AppearanceEditor editor = std::move(created).Value();
    CHECK_FALSE(editor.CanUndo());
    CHECK_FALSE(editor.CanRedo());
    CHECK_FALSE(editor.Undo());
    CHECK_FALSE(editor.Redo());

    const AppearanceSpec s0 = editor.Spec();
    CHECK_FALSE(editor.Apply(SetParameter{Id("base:skin.tone"), 100}).has_value());
    const AppearanceSpec s1 = editor.Spec();
    CHECK_FALSE(editor.Apply(SetHairStyle{ContentId{}}).has_value());
    const AppearanceSpec s2 = editor.Spec();
    CHECK(s0 != s1);
    CHECK(s1 != s2);

    REQUIRE(editor.Undo());
    CHECK(editor.Spec() == s1);
    REQUIRE(editor.Undo());
    CHECK(editor.Spec() == s0);
    CHECK_FALSE(editor.CanUndo());
    REQUIRE(editor.Redo());
    REQUIRE(editor.Redo());
    CHECK(editor.Spec() == s2);
    CHECK_FALSE(editor.CanRedo());

    // A new edit after undo discards the redo branch.
    REQUIRE(editor.Undo());
    CHECK_FALSE(editor.Apply(SetParameter{Id("base:skin.tone"), 900}).has_value());
    CHECK_FALSE(editor.CanRedo());
}

TEST_CASE("Editor: failed and no-op edits leave the history alone") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    auto created = AppearanceEditor::Create(catalog, BaseSpec());
    REQUIRE(created.HasValue());
    AppearanceEditor editor = std::move(created).Value();

    const auto error = editor.Apply(SetParameter{Id("base:skin.tone"), 5000});
    CHECK(error.value_or(AppearanceError{AppearanceErrorCode::EmptyCatalog, "", 0}).code ==
          AppearanceErrorCode::ParameterOutOfRange);
    CHECK(editor.Spec() == BaseSpec());
    CHECK_FALSE(editor.CanUndo());

    CHECK_FALSE(editor.Apply(SetParameter{Id("base:skin.tone"), 400}).has_value()); // same value
    CHECK_FALSE(editor.CanUndo());
}

TEST_CASE("Editor: history is capped") {
    const AppearanceCatalog catalog = MakeTestCatalog();
    auto created = AppearanceEditor::Create(catalog, BaseSpec());
    REQUIRE(created.HasValue());
    AppearanceEditor editor = std::move(created).Value();
    const std::size_t edits = AppearanceEditor::kMaxHistory + 50;
    for (std::size_t i = 0; i < edits; ++i) {
        REQUIRE_FALSE(editor.Apply(SetParameter{Id("base:skin.tone"), static_cast<ParamValue>(i)})
                          .has_value());
    }
    std::size_t undone = 0;
    while (editor.Undo()) {
        ++undone;
    }
    CHECK(undone == AppearanceEditor::kMaxHistory);
}
