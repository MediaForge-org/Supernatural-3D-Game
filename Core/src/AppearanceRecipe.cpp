#include "TextParsing.h"

#include <spn/core/AppearanceRecipe.h>
#include <utility>

namespace spn::core {

namespace {

using detail::ParseCanonicalInteger;
using detail::ParseEnum;
using detail::SplitTabs;

constexpr std::string_view kMagic = "SPNRECIPE";
constexpr char kTab = '\t';

AppearanceError Fail(AppearanceErrorCode code, std::string_view subject = {},
                     std::size_t line = 0) {
    return AppearanceError{code, std::string(subject.substr(0, 96)), line};
}

const std::string& IdText(const ContentId& id) {
    static const std::string kNone = "-";
    return id.IsEmpty() ? kNone : id.Value();
}

} // namespace

AppearanceRecipe MakeRecipe(const AppearanceCatalog& catalog, std::uint64_t seed,
                            const GenerationOptions& options) {
    AppearanceRecipe recipe;
    recipe.seed = seed;
    recipe.generatorVersion = kAppearanceGeneratorVersion;
    recipe.catalogFingerprint = catalog.Fingerprint();
    recipe.options = options;
    return recipe;
}

Result<AppearanceSpec, AppearanceError> ResolveRecipe(const AppearanceCatalog& catalog,
                                                      const AppearanceRecipe& recipe,
                                                      FingerprintPolicy policy) {
    if (recipe.generatorVersion != kAppearanceGeneratorVersion) {
        return Fail(AppearanceErrorCode::UnsupportedGeneratorVersion,
                    std::to_string(recipe.generatorVersion));
    }
    if (policy == FingerprintPolicy::Require && recipe.catalogFingerprint &&
        *recipe.catalogFingerprint != catalog.Fingerprint()) {
        return Fail(AppearanceErrorCode::CatalogChanged);
    }
    auto generated = GenerateAppearance(catalog, recipe.seed, recipe.options);
    if (!generated) {
        return generated.Error();
    }
    AppearanceSpec spec = std::move(generated).Value();

    const AppearanceOverrides& overrides = recipe.overrides;
    if (overrides.bodyProvider) {
        spec.bodyProvider = *overrides.bodyProvider;
    }
    if (overrides.hairStyle) {
        spec.hairStyle = *overrides.hairStyle;
    }
    if (overrides.facialHairStyle) {
        spec.facialHairStyle = *overrides.facialHairStyle;
    }
    for (const auto& [id, value] : overrides.parameters) {
        spec.parameters.insert_or_assign(id, value);
    }
    for (const auto& [slot, item] : overrides.clothing) {
        if (item.IsEmpty()) {
            spec.clothing.erase(slot);
        } else {
            spec.clothing.insert_or_assign(slot, item);
        }
    }
    if (overrides.layers) {
        spec.layers = *overrides.layers;
    }

    if (auto issues = ValidateAppearance(spec, catalog); !issues.empty()) {
        return std::move(issues.front());
    }
    return spec;
}

Result<AppearanceSpec, AppearanceError> ResolveAppearance(const AppearanceCatalog& catalog,
                                                          const AppearanceSource& source,
                                                          FingerprintPolicy policy) {
    if (const auto* recipe = std::get_if<AppearanceRecipe>(&source)) {
        return ResolveRecipe(catalog, *recipe, policy);
    }
    const auto& spec = std::get<AppearanceSpec>(source);
    if (auto issues = ValidateAppearance(spec, catalog); !issues.empty()) {
        return std::move(issues.front());
    }
    return spec;
}

// --- Serialization ----------------------------------------------------------

std::string SerializeRecipe(const AppearanceRecipe& recipe) {
    std::string out;
    out += kMagic;
    out += " 1\n";
    std::size_t lines = 0;
    const auto line = [&out, &lines](std::string_view keyword, const std::string& value) {
        out += keyword;
        out += kTab;
        out += value;
        out += '\n';
        ++lines;
    };
    const GenerationOptions& o = recipe.options;
    line("seed", std::to_string(recipe.seed));
    line("generator", std::to_string(recipe.generatorVersion));
    line("catalog", recipe.catalogFingerprint ? std::to_string(*recipe.catalogFingerprint) : "-");
    line("options", std::to_string(o.baldChance) + kTab + std::to_string(o.facialHairChance) +
                        kTab + std::to_string(o.tattooChance) + kTab +
                        std::to_string(o.scarChance) + kTab + std::to_string(o.maxTattoos) + kTab +
                        std::to_string(o.maxScars));
    const AppearanceOverrides& ov = recipe.overrides;
    if (ov.bodyProvider) {
        line("body", IdText(*ov.bodyProvider));
    }
    if (ov.hairStyle) {
        line("hair", IdText(*ov.hairStyle));
    }
    if (ov.facialHairStyle) {
        line("facial_hair", IdText(*ov.facialHairStyle));
    }
    for (const auto& [id, value] : ov.parameters) {
        line("param", id.Value() + kTab + std::to_string(value));
    }
    for (const auto& [slot, item] : ov.clothing) {
        line("clothing", std::string(ToString(slot)) + kTab + IdText(item));
    }
    if (ov.layers) {
        line("layers", std::to_string(ov.layers->size()));
        for (const AppearanceLayer& layer : *ov.layers) {
            line("layer", std::string(ToString(layer.kind)) + kTab + IdText(layer.content) + kTab +
                              std::string(ToString(layer.region)) + kTab +
                              std::to_string(layer.opacity));
        }
    }
    out += "end";
    out += kTab;
    out += std::to_string(lines);
    out += '\n';
    return out;
}

namespace {

/// Order of the optional override lines; each may appear at most once, in this order.
enum class Stage : std::uint8_t { Fixed, Body, Hair, FacialHair, Params, Clothing, Layers };

std::optional<ContentId> ParseId(std::string_view token, bool allowNone) {
    if (allowNone && token == "-") {
        return ContentId{};
    }
    return ContentId::Parse(token);
}

} // namespace

Result<AppearanceRecipe, AppearanceError> DeserializeRecipe(std::string_view text) {
    AppearanceRecipe recipe;
    std::size_t position = 0;
    std::size_t lineNumber = 0;
    Stage stage = Stage::Fixed;
    std::optional<ParameterId> lastParameter;
    std::optional<ClothingSlot> lastSlot;
    std::size_t layersLeft = 0; // 'layer' lines still owed by the 'layers' line

    while (position < text.size()) {
        const std::size_t newline = text.find('\n', position);
        ++lineNumber;
        if (newline == std::string_view::npos) {
            return Fail(lineNumber == 1 ? AppearanceErrorCode::InvalidHeader
                                        : AppearanceErrorCode::TruncatedData,
                        {}, lineNumber);
        }
        const std::string_view line = text.substr(position, newline - position);
        position = newline + 1;

        if (lineNumber == 1) {
            if (line != std::string(kMagic) + " 1") {
                const bool sameFamily = line.starts_with(std::string(kMagic) + ' ');
                const auto version =
                    sameFamily ? ParseCanonicalInteger<std::int32_t>(line.substr(kMagic.size() + 1))
                               : std::nullopt;
                return Fail(version ? AppearanceErrorCode::UnsupportedVersion
                                    : AppearanceErrorCode::InvalidHeader,
                            {}, lineNumber);
            }
            continue;
        }

        const std::vector<std::string_view> fields = SplitTabs(line);
        const std::string_view keyword = fields.front();
        const auto malformed = [&] {
            return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
        };
        const auto badToken = [&](std::string_view token) {
            return Fail(AppearanceErrorCode::InvalidToken, token, lineNumber);
        };
        const auto outOfOrder = [&] {
            return Fail(AppearanceErrorCode::NonCanonical, keyword, lineNumber);
        };

        // Lines 2-5: seed, generator, catalog, options.
        if (lineNumber <= 5) {
            constexpr std::array<std::string_view, 4> kFixed = {"seed", "generator", "catalog",
                                                                "options"};
            const std::size_t expectedFields = lineNumber == 5 ? 7 : 2;
            if (keyword != kFixed.at(lineNumber - 2) || fields.size() != expectedFields) {
                return malformed();
            }
            if (lineNumber == 2) {
                const auto seed = ParseCanonicalInteger<std::uint64_t>(fields[1]);
                if (!seed) {
                    return badToken(fields[1]);
                }
                recipe.seed = *seed;
            } else if (lineNumber == 3) {
                const auto version = ParseCanonicalInteger<std::uint32_t>(fields[1]);
                if (!version) {
                    return badToken(fields[1]);
                }
                recipe.generatorVersion = *version;
            } else if (lineNumber == 4) {
                if (fields[1] != "-") {
                    const auto fingerprint = ParseCanonicalInteger<std::uint64_t>(fields[1]);
                    if (!fingerprint) {
                        return badToken(fields[1]);
                    }
                    recipe.catalogFingerprint = fingerprint;
                }
            } else {
                GenerationOptions& o = recipe.options;
                const auto bald = ParseCanonicalInteger<std::int32_t>(fields[1]);
                const auto facial = ParseCanonicalInteger<std::int32_t>(fields[2]);
                const auto tattoo = ParseCanonicalInteger<std::int32_t>(fields[3]);
                const auto scar = ParseCanonicalInteger<std::int32_t>(fields[4]);
                const auto maxTattoos = ParseCanonicalInteger<std::uint32_t>(fields[5]);
                const auto maxScars = ParseCanonicalInteger<std::uint32_t>(fields[6]);
                if (!bald || !facial || !tattoo || !scar || !maxTattoos || !maxScars) {
                    return badToken(line);
                }
                o.baldChance = *bald;
                o.facialHairChance = *facial;
                o.tattooChance = *tattoo;
                o.scarChance = *scar;
                o.maxTattoos = *maxTattoos;
                o.maxScars = *maxScars;
            }
            continue;
        }

        if (keyword == "end") {
            if (fields.size() != 2) {
                return malformed();
            }
            const auto count = ParseCanonicalInteger<std::int64_t>(fields[1]);
            if (!count) {
                return badToken(fields[1]);
            }
            if (layersLeft != 0) {
                return Fail(AppearanceErrorCode::CountMismatch, "layers", lineNumber);
            }
            if (*count != static_cast<std::int64_t>(lineNumber) - 2) {
                return Fail(AppearanceErrorCode::CountMismatch, {}, lineNumber);
            }
            if (position != text.size()) {
                return Fail(AppearanceErrorCode::TrailingData, {}, lineNumber + 1);
            }
            return recipe;
        }

        if (layersLeft != 0 && keyword != "layer") {
            return Fail(AppearanceErrorCode::CountMismatch, "layers", lineNumber);
        }

        if (keyword == "body" || keyword == "hair" || keyword == "facial_hair") {
            if (fields.size() != 2) {
                return malformed();
            }
            const Stage mine = keyword == "body"   ? Stage::Body
                               : keyword == "hair" ? Stage::Hair
                                                   : Stage::FacialHair;
            if (stage >= mine) {
                return outOfOrder();
            }
            stage = mine;
            const auto id = ParseId(fields[1], mine != Stage::Body);
            if (!id) {
                return badToken(fields[1]);
            }
            (mine == Stage::Body   ? recipe.overrides.bodyProvider
             : mine == Stage::Hair ? recipe.overrides.hairStyle
                                   : recipe.overrides.facialHairStyle) = *id;
        } else if (keyword == "param") {
            if (fields.size() != 3) {
                return malformed();
            }
            if (stage > Stage::Params) {
                return outOfOrder();
            }
            stage = Stage::Params;
            const auto id = ParseId(fields[1], false);
            const auto value = ParseCanonicalInteger<ParamValue>(fields[2]);
            if (!id || !value) {
                return badToken(!id ? fields[1] : fields[2]);
            }
            if (lastParameter && !(*lastParameter < *id)) {
                return Fail(AppearanceErrorCode::NonCanonical, fields[1], lineNumber);
            }
            if (recipe.overrides.parameters.size() >= kMaxAppearanceParameters) {
                return Fail(AppearanceErrorCode::TooManyParameters, {}, lineNumber);
            }
            lastParameter = id;
            recipe.overrides.parameters.emplace(*id, *value);
        } else if (keyword == "clothing") {
            if (fields.size() != 3) {
                return malformed();
            }
            if (stage > Stage::Clothing) {
                return outOfOrder();
            }
            stage = Stage::Clothing;
            const auto slot = ParseEnum(fields[1], kAllClothingSlots);
            const auto item = ParseId(fields[2], true);
            if (!slot || !item) {
                return badToken(!slot ? fields[1] : fields[2]);
            }
            if (lastSlot && !(*lastSlot < *slot)) {
                return Fail(AppearanceErrorCode::NonCanonical, fields[1], lineNumber);
            }
            lastSlot = slot;
            recipe.overrides.clothing.emplace(*slot, *item);
        } else if (keyword == "layers") {
            if (fields.size() != 2) {
                return malformed();
            }
            if (stage >= Stage::Layers) {
                return outOfOrder();
            }
            stage = Stage::Layers;
            const auto count = ParseCanonicalInteger<std::size_t>(fields[1]);
            if (!count) {
                return badToken(fields[1]);
            }
            if (*count > kMaxAppearanceLayers) {
                return Fail(AppearanceErrorCode::TooManyLayers, fields[1], lineNumber);
            }
            layersLeft = *count;
            recipe.overrides.layers.emplace();
        } else if (keyword == "layer") {
            if (fields.size() != 5) {
                return malformed();
            }
            if (layersLeft == 0) {
                return Fail(stage == Stage::Layers ? AppearanceErrorCode::CountMismatch
                                                   : AppearanceErrorCode::NonCanonical,
                            "layers", lineNumber);
            }
            --layersLeft;
            const auto kind = ParseEnum(fields[1], kAllLayerKinds);
            const auto content = ParseId(fields[2], false);
            const auto region = ParseEnum(fields[3], kAllBodyRegions);
            const auto opacity = ParseCanonicalInteger<ParamValue>(fields[4]);
            if (!kind || !content || !region || !opacity) {
                return badToken(!kind      ? fields[1]
                                : !content ? fields[2]
                                : !region  ? fields[3]
                                           : fields[4]);
            }
            recipe.overrides.layers->push_back(AppearanceLayer{*kind, *content, *region, *opacity});
        } else {
            return malformed();
        }
    }
    return Fail(lineNumber == 0 ? AppearanceErrorCode::InvalidHeader
                                : AppearanceErrorCode::TruncatedData,
                {}, lineNumber);
}

std::string SerializeSource(const AppearanceSource& source) {
    if (const auto* recipe = std::get_if<AppearanceRecipe>(&source)) {
        return SerializeRecipe(*recipe);
    }
    return SerializeAppearance(std::get<AppearanceSpec>(source));
}

Result<AppearanceSource, AppearanceError> DeserializeSource(std::string_view text) {
    if (text.starts_with("SPNRECIPE")) {
        auto recipe = DeserializeRecipe(text);
        if (!recipe) {
            return recipe.Error();
        }
        return AppearanceSource{std::move(recipe).Value()};
    }
    auto spec = DeserializeAppearance(text);
    if (!spec) {
        return spec.Error();
    }
    return AppearanceSource{std::move(spec).Value()};
}

} // namespace spn::core
