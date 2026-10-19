#include <algorithm>
#include <spn/core/AppearanceGenerator.h>
#include <spn/core/DeterministicRandom.h>
#include <string>
#include <utility>
#include <vector>

namespace spn::core {

namespace {

constexpr std::int32_t kChanceScale = 1000;

/// Content ids of one kind in catalog (id) order that fit `provider` (empty provider: no
/// restriction) and the predicate.
template <typename Predicate>
std::vector<ContentId> Candidates(const AppearanceCatalog& catalog, ContentKind kind,
                                  const ContentId& provider, Predicate matches) {
    std::vector<ContentId> ids;
    for (const auto& [id, def] : catalog.Content()) {
        const bool fits =
            provider.IsEmpty() || AppearanceCatalog::Supports(def.providers, provider);
        if (def.kind == kind && fits && matches(def)) {
            ids.push_back(id);
        }
    }
    return ids;
}

const ContentId& Pick(const std::vector<ContentId>& candidates, SplitMix64& rng) {
    return candidates[static_cast<std::size_t>(rng.NextBelow(candidates.size()))];
}

bool Chance(SplitMix64& rng, std::int32_t perMille) {
    return rng.NextInRange(0, kChanceScale - 1) < perMille;
}

/// Clustered at the default; the far end of the range is rare. Integer-only.
ParamValue DrawTowardDefault(const ParameterDef& def, SplitMix64& rng) {
    const std::int64_t up = std::int64_t{def.max} - def.defaultValue;
    const std::int64_t down = std::int64_t{def.defaultValue} - def.min;
    const bool coin = rng.NextBelow(2) == 0; // always drawn, so the stream layout is fixed
    const std::int64_t u = rng.NextInRange(0, kChanceScale);
    const std::int64_t t =
        u * u * u / (std::int64_t{kChanceScale} * kChanceScale); // 0..1000, cubic skew to 0
    const bool goUp = up > 0 && (down == 0 || coin);
    const std::int64_t extent = goUp ? up : down;
    const std::int64_t offset = extent * t / kChanceScale;
    return static_cast<ParamValue>(goUp ? def.defaultValue + offset : def.defaultValue - offset);
}

/// Per the parameter's GenerationShape (a hint from the catalog, not a restriction).
ParamValue DrawParameter(const ParameterDef& def, std::uint64_t seed) {
    SplitMix64 rng = MakeStream(seed, "param:" + def.id.Value());
    switch (def.shape) {
    case GenerationShape::Uniform:
        return rng.NextInRange(def.min, def.max);
    case GenerationShape::TowardDefault:
        return DrawTowardDefault(def, rng);
    case GenerationShape::Bell:
        break;
    }
    // Mean of three uniform draws over [min, max]: bell-shaped around the range centre.
    const std::int64_t sum = std::int64_t{rng.NextInRange(def.min, def.max)} +
                             rng.NextInRange(def.min, def.max) + rng.NextInRange(def.min, def.max);
    const std::int64_t offset = sum - 3 * std::int64_t{def.min}; // >= 0
    return static_cast<ParamValue>(std::int64_t{def.min} + offset / 3);
}

bool ValidChance(std::int32_t value) {
    return value >= 0 && value <= kChanceScale;
}

/// Regions where a layer of this kind is generated (placeholder choice).
std::vector<BodyRegion> RegionsFor(LayerKind kind) {
    if (kind == LayerKind::Tattoo) {
        return {BodyRegion::Neck,     BodyRegion::Torso,    BodyRegion::Back,
                BodyRegion::LeftArm,  BodyRegion::RightArm, BodyRegion::LeftHand,
                BodyRegion::RightHand};
    }
    return {kAllBodyRegions.begin(), kAllBodyRegions.end()};
}

/// Appends 1..maxCount layers of `kind` (if `chance` hits) without exact duplicates.
void AddLayers(AppearanceSpec& spec, const AppearanceCatalog& catalog, LayerKind kind,
               std::int32_t chance, std::uint32_t maxCount, std::int32_t minOpacity,
               std::uint64_t seed) {
    const std::string name(ToString(kind));
    const auto candidates =
        Candidates(catalog, ContentKind::Layer, spec.bodyProvider,
                   [kind](const ContentDef& def) { return def.layerKind == kind; });
    if (candidates.empty() || maxCount == 0) {
        return;
    }
    SplitMix64 countRng = MakeStream(seed, "layer.count:" + name);
    if (!Chance(countRng, chance)) {
        return;
    }
    const auto count = 1 + countRng.NextBelow(maxCount);
    const std::vector<BodyRegion> regions = RegionsFor(kind);
    for (std::uint64_t i = 0; i < count; ++i) {
        SplitMix64 rng = MakeStream(seed, "layer:" + name + ':' + std::to_string(i));
        AppearanceLayer layer;
        layer.kind = kind;
        layer.content = Pick(candidates, rng);
        layer.region = regions[static_cast<std::size_t>(rng.NextBelow(regions.size()))];
        layer.opacity = rng.NextInRange(minOpacity, kMaxLayerOpacity);
        const bool duplicate = std::ranges::any_of(spec.layers, [&layer](const AppearanceLayer& l) {
            return l.kind == layer.kind && l.content == layer.content && l.region == layer.region;
        });
        if (!duplicate && spec.layers.size() < kMaxAppearanceLayers) {
            spec.layers.push_back(std::move(layer));
        }
    }
}

/// Chance that an NPC wears something in this slot (placeholder).
std::int32_t ClothingChance(ClothingSlot slot) {
    switch (slot) {
    case ClothingSlot::Torso:
    case ClothingSlot::Legs:
    case ClothingSlot::Feet:
        return kChanceScale;
    case ClothingSlot::Head:
    case ClothingSlot::Hands:
    case ClothingSlot::Outer:
    case ClothingSlot::Accessory:
        return 250;
    }
    return 0;
}

} // namespace

Result<AppearanceSpec, AppearanceError> GenerateAppearance(const AppearanceCatalog& catalog,
                                                           std::uint64_t seed,
                                                           const GenerationOptions& options) {
    const auto badOption = [](std::string_view name) {
        return AppearanceError{AppearanceErrorCode::InvalidGenerationOption, std::string(name), 0};
    };
    if (!ValidChance(options.baldChance)) {
        return badOption("baldChance");
    }
    if (!ValidChance(options.facialHairChance)) {
        return badOption("facialHairChance");
    }
    if (!ValidChance(options.tattooChance)) {
        return badOption("tattooChance");
    }
    if (!ValidChance(options.scarChance)) {
        return badOption("scarChance");
    }
    if (options.maxTattoos > kMaxAppearanceLayers || options.maxScars > kMaxAppearanceLayers) {
        return badOption("maxLayers");
    }

    const auto always = [](const ContentDef&) { return true; };
    const auto bodies = Candidates(catalog, ContentKind::BodyProvider, ContentId{}, always);
    if (bodies.empty()) {
        return AppearanceError{AppearanceErrorCode::EmptyCatalog, "body_provider", 0};
    }

    AppearanceSpec spec;
    {
        SplitMix64 rng = MakeStream(seed, "body_provider");
        spec.bodyProvider = Pick(bodies, rng);
    }
    for (const auto& [id, def] : catalog.Parameters()) {
        if (AppearanceCatalog::Supports(def.providers, spec.bodyProvider)) {
            spec.parameters.emplace(id, DrawParameter(def, seed));
        }
    }

    const auto hairStyles = Candidates(catalog, ContentKind::HairStyle, spec.bodyProvider, always);
    if (!hairStyles.empty()) {
        SplitMix64 rng = MakeStream(seed, "hair_style");
        if (!Chance(rng, options.baldChance)) {
            spec.hairStyle = Pick(hairStyles, rng);
        }
    }
    const auto facialHairStyles =
        Candidates(catalog, ContentKind::FacialHairStyle, spec.bodyProvider, always);
    if (!facialHairStyles.empty()) {
        SplitMix64 rng = MakeStream(seed, "facial_hair_style");
        if (Chance(rng, options.facialHairChance)) {
            spec.facialHairStyle = Pick(facialHairStyles, rng);
        }
    }

    for (const ClothingSlot slot : kAllClothingSlots) {
        const auto items = Candidates(catalog, ContentKind::Clothing, spec.bodyProvider,
                                      [slot](const ContentDef& def) { return def.slot == slot; });
        if (items.empty()) {
            continue;
        }
        SplitMix64 rng = MakeStream(seed, "clothing:" + std::string(ToString(slot)));
        if (Chance(rng, ClothingChance(slot))) {
            spec.clothing.emplace(slot, Pick(items, rng));
        }
    }

    AddLayers(spec, catalog, LayerKind::Tattoo, options.tattooChance, options.maxTattoos, 700,
              seed);
    AddLayers(spec, catalog, LayerKind::Scar, options.scarChance, options.maxScars, 400, seed);

    if (const auto issues = ValidateAppearance(spec, catalog); !issues.empty()) {
        return issues.front(); // cannot happen for a consistent catalog; never return bad data
    }
    return spec;
}

} // namespace spn::core
