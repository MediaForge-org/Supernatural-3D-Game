#pragma once

// Shared fixture: a small catalog with clearly labelled placeholder content.
#include <doctest.h>
#include <spn/core/Appearance.h>
#include <string_view>
#include <utility>

namespace spn::testing {

inline spn::core::ContentId Id(std::string_view text) {
    auto id = spn::core::ContentId::Parse(text);
    REQUIRE_MESSAGE(id.has_value(), "invalid test id: ", text);
    return id.value_or(spn::core::ContentId{});
}

/// Helpers that spell out every field (the defs have more fields than most tests care about).
inline spn::core::ParameterDef P(spn::core::ParameterId id, spn::core::ParameterGroup group,
                                 spn::core::ParamValue min, spn::core::ParamValue max,
                                 spn::core::ParamValue defaultValue) {
    return spn::core::ParameterDef{
        std::move(id), group, min, max, defaultValue, spn::core::GenerationShape::Bell, {}};
}

inline spn::core::ContentDef C(spn::core::ContentId id, spn::core::ContentKind kind) {
    return spn::core::ContentDef{
        std::move(id), kind, spn::core::ClothingSlot::Torso, spn::core::LayerKind::Tattoo, {}};
}

inline void AddContent(spn::core::AppearanceCatalog& catalog, std::string_view id,
                       spn::core::ContentKind kind,
                       spn::core::ClothingSlot slot = spn::core::ClothingSlot::Torso,
                       spn::core::LayerKind layerKind = spn::core::LayerKind::Tattoo) {
    const auto error = catalog.AddContent({Id(id), kind, slot, layerKind, {}});
    REQUIRE_FALSE(error.has_value());
}

inline spn::core::AppearanceCatalog MakeTestCatalog() {
    using namespace spn::core;
    AppearanceCatalog catalog;
    for (const ParameterDef& def : BuiltinParameterDefs()) {
        REQUIRE_FALSE(catalog.AddParameter(def).has_value());
    }
    AddContent(catalog, "base:body.placeholder_a", ContentKind::BodyProvider);
    AddContent(catalog, "base:body.placeholder_b", ContentKind::BodyProvider);
    AddContent(catalog, "base:hair.placeholder_short", ContentKind::HairStyle);
    AddContent(catalog, "base:hair.placeholder_long", ContentKind::HairStyle);
    AddContent(catalog, "base:hair.placeholder_curly", ContentKind::HairStyle);
    AddContent(catalog, "base:beard.placeholder_short", ContentKind::FacialHairStyle);
    AddContent(catalog, "base:beard.placeholder_full", ContentKind::FacialHairStyle);
    AddContent(catalog, "base:cloth.placeholder_shirt", ContentKind::Clothing, ClothingSlot::Torso);
    AddContent(catalog, "base:cloth.placeholder_jacket", ContentKind::Clothing,
               ClothingSlot::Outer);
    AddContent(catalog, "base:cloth.placeholder_jeans", ContentKind::Clothing, ClothingSlot::Legs);
    AddContent(catalog, "base:cloth.placeholder_boots", ContentKind::Clothing, ClothingSlot::Feet);
    AddContent(catalog, "base:cloth.placeholder_cap", ContentKind::Clothing, ClothingSlot::Head);
    AddContent(catalog, "base:tattoo.placeholder_anchor", ContentKind::Layer, ClothingSlot::Torso,
               LayerKind::Tattoo);
    AddContent(catalog, "base:tattoo.placeholder_sigil", ContentKind::Layer, ClothingSlot::Torso,
               LayerKind::Tattoo);
    AddContent(catalog, "base:tattoo.placeholder_band", ContentKind::Layer, ClothingSlot::Torso,
               LayerKind::Tattoo);
    AddContent(catalog, "base:scar.placeholder_slash", ContentKind::Layer, ClothingSlot::Torso,
               LayerKind::Scar);
    AddContent(catalog, "base:scar.placeholder_burn", ContentKind::Layer, ClothingSlot::Torso,
               LayerKind::Scar);
    return catalog;
}

/// MakeTestCatalog plus provider-specific content: a parameter that only body B supports and
/// a hat that only fits body A (everything else fits both).
inline spn::core::AppearanceCatalog MakeProviderCatalog() {
    using namespace spn::core;
    AppearanceCatalog catalog = MakeTestCatalog();
    ParameterDef horns = P(Id("mod_a:body.horn_size"), ParameterGroup::Body, 0, 1000, 0);
    horns.providers = {Id("base:body.placeholder_b")};
    REQUIRE_FALSE(catalog.AddParameter(horns).has_value());
    ContentDef hat = C(Id("mod_a:cloth.hat_for_a"), ContentKind::Clothing);
    hat.slot = ClothingSlot::Head;
    hat.providers = {Id("base:body.placeholder_a")};
    REQUIRE_FALSE(catalog.AddContent(hat).has_value());
    return catalog;
}

} // namespace spn::testing
