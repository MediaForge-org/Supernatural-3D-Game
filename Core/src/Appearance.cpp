#include "TextParsing.h"

#include <algorithm>
#include <set>
#include <spn/core/Appearance.h>
#include <spn/core/DeterministicRandom.h>
#include <utility>

namespace spn::core {

namespace {

constexpr std::string_view kMagic = "SPNAPPEARANCE";
constexpr char kTab = '\t';
constexpr std::size_t kMaxSubjectLength = 96;

AppearanceError Fail(AppearanceErrorCode code, std::string_view subject = {},
                     std::size_t line = 0) {
    return AppearanceError{code, std::string(subject.substr(0, kMaxSubjectLength)), line};
}

using detail::ParseCanonicalInteger;
using detail::ParseEnum;
using detail::SplitTabs;

const std::string& IdText(const ContentId& id) {
    static const std::string kNone = "-";
    return id.IsEmpty() ? kNone : id.Value();
}

} // namespace

// --- ToString ---------------------------------------------------------------

std::string_view ToString(ParameterGroup value) noexcept {
    switch (value) {
    case ParameterGroup::Face:
        return "face";
    case ParameterGroup::Body:
        return "body";
    case ParameterGroup::Skin:
        return "skin";
    case ParameterGroup::Eyes:
        return "eyes";
    case ParameterGroup::Hair:
        return "hair";
    case ParameterGroup::Custom:
        return "custom";
    }
    return "unknown";
}

std::string_view ToString(ClothingSlot value) noexcept {
    switch (value) {
    case ClothingSlot::Head:
        return "head";
    case ClothingSlot::Torso:
        return "torso";
    case ClothingSlot::Legs:
        return "legs";
    case ClothingSlot::Feet:
        return "feet";
    case ClothingSlot::Hands:
        return "hands";
    case ClothingSlot::Outer:
        return "outer";
    case ClothingSlot::Accessory:
        return "accessory";
    }
    return "unknown";
}

std::string_view ToString(LayerKind value) noexcept {
    switch (value) {
    case LayerKind::Tattoo:
        return "tattoo";
    case LayerKind::Scar:
        return "scar";
    case LayerKind::Birthmark:
        return "birthmark";
    case LayerKind::Makeup:
        return "makeup";
    case LayerKind::Dirt:
        return "dirt";
    }
    return "unknown";
}

std::string_view ToString(BodyRegion value) noexcept {
    switch (value) {
    case BodyRegion::Head:
        return "head";
    case BodyRegion::Face:
        return "face";
    case BodyRegion::Neck:
        return "neck";
    case BodyRegion::Torso:
        return "torso";
    case BodyRegion::Back:
        return "back";
    case BodyRegion::LeftArm:
        return "left_arm";
    case BodyRegion::RightArm:
        return "right_arm";
    case BodyRegion::LeftHand:
        return "left_hand";
    case BodyRegion::RightHand:
        return "right_hand";
    case BodyRegion::LeftLeg:
        return "left_leg";
    case BodyRegion::RightLeg:
        return "right_leg";
    }
    return "unknown";
}

std::string_view ToString(ContentKind value) noexcept {
    switch (value) {
    case ContentKind::BodyProvider:
        return "body_provider";
    case ContentKind::HairStyle:
        return "hair_style";
    case ContentKind::FacialHairStyle:
        return "facial_hair_style";
    case ContentKind::Clothing:
        return "clothing";
    case ContentKind::Layer:
        return "layer";
    }
    return "unknown";
}

std::string_view ToString(GenerationShape value) noexcept {
    switch (value) {
    case GenerationShape::Bell:
        return "bell";
    case GenerationShape::Uniform:
        return "uniform";
    case GenerationShape::TowardDefault:
        return "toward_default";
    }
    return "unknown";
}

std::string_view ToString(AppearanceErrorCode code) noexcept {
    switch (code) {
    case AppearanceErrorCode::UnsupportedSchemaVersion:
        return "unsupported schema version";
    case AppearanceErrorCode::MissingBodyProvider:
        return "missing body provider";
    case AppearanceErrorCode::UnknownContent:
        return "unknown content";
    case AppearanceErrorCode::ContentKindMismatch:
        return "content kind mismatch";
    case AppearanceErrorCode::ClothingSlotMismatch:
        return "clothing slot mismatch";
    case AppearanceErrorCode::LayerKindMismatch:
        return "layer kind mismatch";
    case AppearanceErrorCode::UnknownParameter:
        return "unknown parameter";
    case AppearanceErrorCode::ParameterOutOfRange:
        return "parameter out of range";
    case AppearanceErrorCode::OpacityOutOfRange:
        return "opacity out of range";
    case AppearanceErrorCode::DuplicateLayer:
        return "duplicate layer";
    case AppearanceErrorCode::TooManyLayers:
        return "too many layers";
    case AppearanceErrorCode::InvalidId:
        return "invalid id";
    case AppearanceErrorCode::DuplicateDefinition:
        return "duplicate definition";
    case AppearanceErrorCode::InvalidParameterRange:
        return "invalid parameter range";
    case AppearanceErrorCode::InvalidHeader:
        return "invalid header";
    case AppearanceErrorCode::UnsupportedVersion:
        return "unsupported version";
    case AppearanceErrorCode::TruncatedData:
        return "truncated data";
    case AppearanceErrorCode::TrailingData:
        return "trailing data";
    case AppearanceErrorCode::MalformedLine:
        return "malformed line";
    case AppearanceErrorCode::InvalidToken:
        return "invalid token";
    case AppearanceErrorCode::NonCanonical:
        return "non-canonical data";
    case AppearanceErrorCode::CountMismatch:
        return "count mismatch";
    case AppearanceErrorCode::EmptyCatalog:
        return "empty catalog";
    case AppearanceErrorCode::InvalidGenerationOption:
        return "invalid generation option";
    case AppearanceErrorCode::ParameterUnsupportedByProvider:
        return "parameter unsupported by body provider";
    case AppearanceErrorCode::ContentIncompatibleWithProvider:
        return "content incompatible with body provider";
    case AppearanceErrorCode::TooManyParameters:
        return "too many parameters";
    case AppearanceErrorCode::UnknownProviderReference:
        return "unknown provider reference";
    case AppearanceErrorCode::UnsupportedGeneratorVersion:
        return "unsupported generator version";
    case AppearanceErrorCode::CatalogChanged:
        return "catalog changed";
    case AppearanceErrorCode::IndexOutOfRange:
        return "index out of range";
    }
    return "unknown error";
}

std::string Describe(const AppearanceError& error) {
    std::string text(ToString(error.code));
    if (!error.subject.empty()) {
        text += ": ";
        text += error.subject;
    }
    if (error.line != 0) {
        text += " (line ";
        text += std::to_string(error.line);
        text += ')';
    }
    return text;
}

// --- Catalog ----------------------------------------------------------------

namespace {

/// Providers lists are sets: sorted and free of duplicates, so equal catalogs fingerprint
/// identically regardless of the order a pack listed them in.
void NormalizeProviders(std::vector<ContentId>& providers) {
    std::ranges::sort(providers);
    providers.erase(std::ranges::unique(providers).begin(), providers.end());
}

} // namespace

std::optional<AppearanceError> AppearanceCatalog::AddParameter(ParameterDef def) {
    if (def.id.IsEmpty()) {
        return Fail(AppearanceErrorCode::InvalidId);
    }
    if (def.min > def.max || def.defaultValue < def.min || def.defaultValue > def.max) {
        return Fail(AppearanceErrorCode::InvalidParameterRange, def.id.Value());
    }
    if (parameters_.contains(def.id)) {
        return Fail(AppearanceErrorCode::DuplicateDefinition, def.id.Value());
    }
    if (parameters_.size() >= kMaxAppearanceParameters) {
        return Fail(AppearanceErrorCode::TooManyParameters, def.id.Value());
    }
    NormalizeProviders(def.providers);
    const ParameterId id = def.id;
    parameters_.emplace(id, std::move(def));
    return std::nullopt;
}

std::optional<AppearanceError> AppearanceCatalog::AddContent(ContentDef def) {
    if (def.id.IsEmpty()) {
        return Fail(AppearanceErrorCode::InvalidId);
    }
    if (content_.contains(def.id)) {
        return Fail(AppearanceErrorCode::DuplicateDefinition, def.id.Value());
    }
    NormalizeProviders(def.providers);
    const ContentId id = def.id;
    content_.emplace(id, std::move(def));
    return std::nullopt;
}

const ParameterDef* AppearanceCatalog::FindParameter(const ParameterId& id) const {
    const auto it = parameters_.find(id);
    return it != parameters_.end() ? &it->second : nullptr;
}

const ContentDef* AppearanceCatalog::FindContent(const ContentId& id) const {
    const auto it = content_.find(id);
    return it != content_.end() ? &it->second : nullptr;
}

bool AppearanceCatalog::Supports(const std::vector<ContentId>& providers,
                                 const ContentId& provider) {
    return providers.empty() || std::ranges::find(providers, provider) != providers.end();
}

std::vector<AppearanceError> AppearanceCatalog::ValidateConsistency() const {
    std::vector<AppearanceError> issues;
    const auto check = [this, &issues](const std::vector<ContentId>& providers) {
        for (const ContentId& provider : providers) {
            const ContentDef* def = FindContent(provider);
            if (def == nullptr || def->kind != ContentKind::BodyProvider) {
                issues.push_back(
                    Fail(AppearanceErrorCode::UnknownProviderReference, provider.Value()));
            }
        }
    };
    for (const auto& [id, def] : parameters_) {
        check(def.providers);
    }
    for (const auto& [id, def] : content_) {
        if (def.kind != ContentKind::BodyProvider) {
            check(def.providers);
        }
    }
    return issues;
}

std::uint64_t AppearanceCatalog::Fingerprint() const {
    const auto providersText = [](const std::vector<ContentId>& providers) {
        std::string text;
        for (const ContentId& provider : providers) {
            text += " @" + provider.Value();
        }
        return text;
    };
    std::string text;
    for (const auto& [id, def] : parameters_) {
        text += "p ";
        text += id.Value();
        text += ' ';
        text += ToString(def.group);
        text += ' ' + std::to_string(def.min) + ' ' + std::to_string(def.max) + ' ' +
                std::to_string(def.defaultValue) + ' ';
        text += ToString(def.shape);
        text += providersText(def.providers);
        text += '\n';
    }
    for (const auto& [id, def] : content_) {
        text += "c ";
        text += id.Value();
        text += ' ';
        text += ToString(def.kind);
        if (def.kind == ContentKind::Clothing) {
            text += ' ';
            text += ToString(def.slot);
        } else if (def.kind == ContentKind::Layer) {
            text += ' ';
            text += ToString(def.layerKind);
        }
        text += providersText(def.providers);
        text += '\n';
    }
    return Fnv1a64(text);
}

std::vector<ParameterDef> BuiltinParameterDefs() {
    struct Row {
        std::string_view id;
        ParameterGroup group;
        ParamValue min;
        ParamValue max;
        ParamValue defaultValue;
        GenerationShape shape;
    };
    constexpr ParamValue kBi = 1000;
    constexpr GenerationShape kBell = GenerationShape::Bell;
    constexpr GenerationShape kLow = GenerationShape::TowardDefault;
    // PLACEHOLDER set for review: names, ranges and shapes are not final.
    constexpr std::array kRows = {
        Row{"base:face.head_width", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.jaw_width", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.chin_length", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.cheek_fullness", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.brow_height", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.eye_spacing", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.eye_size", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.nose_width", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.nose_bridge_height", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.mouth_width", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.lip_fullness", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:face.ear_size", ParameterGroup::Face, -kBi, kBi, 0, kBell},
        Row{"base:body.height", ParameterGroup::Body, -kBi, kBi, 0, kBell},
        Row{"base:body.mass", ParameterGroup::Body, -kBi, kBi, 0, kBell},
        Row{"base:body.muscle", ParameterGroup::Body, -kBi, kBi, 0, kBell},
        Row{"base:body.shoulder_width", ParameterGroup::Body, -kBi, kBi, 0, kBell},
        Row{"base:body.hip_width", ParameterGroup::Body, -kBi, kBi, 0, kBell},
        Row{"base:body.leg_length", ParameterGroup::Body, -kBi, kBi, 0, kBell},
        Row{"base:skin.tone", ParameterGroup::Skin, 0, kBi, 500, kBell},
        Row{"base:skin.undertone", ParameterGroup::Skin, -kBi, kBi, 0, kBell},
        Row{"base:skin.freckles", ParameterGroup::Skin, 0, kBi, 0, kLow},
        Row{"base:skin.age_wear", ParameterGroup::Skin, 0, kBi, 0, kLow},
        Row{"base:eyes.iris_hue", ParameterGroup::Eyes, 0, kBi, 500, GenerationShape::Uniform},
        Row{"base:eyes.iris_lightness", ParameterGroup::Eyes, 0, kBi, 500, kBell},
        Row{"base:eyes.iris_saturation", ParameterGroup::Eyes, 0, kBi, 500, kBell},
        Row{"base:hair.melanin", ParameterGroup::Hair, 0, kBi, 500, GenerationShape::Uniform},
        Row{"base:hair.redness", ParameterGroup::Hair, 0, kBi, 0, kLow},
        Row{"base:hair.grey_amount", ParameterGroup::Hair, 0, kBi, 0, kLow},
        Row{"base:hair.density", ParameterGroup::Hair, 0, kBi, 500, kBell},
    };
    std::vector<ParameterDef> defs;
    defs.reserve(kRows.size());
    for (const Row& row : kRows) {
        // The table above is fixed, so a failing Parse would be a programming error.
        if (auto id = ParameterId::Parse(row.id)) {
            defs.push_back(ParameterDef{
                std::move(*id), row.group, row.min, row.max, row.defaultValue, row.shape, {}});
        }
    }
    return defs;
}

// --- Validation -------------------------------------------------------------

namespace {

using Issue = std::optional<AppearanceError>;

/// Finds `id` and checks its kind. On success `def` is set; otherwise the error is returned.
Issue FindContentOfKind(const AppearanceCatalog& catalog, const ContentId& id, ContentKind kind,
                        const ContentDef*& def) {
    def = catalog.FindContent(id);
    if (def == nullptr) {
        return Fail(AppearanceErrorCode::UnknownContent, id.Value());
    }
    if (def->kind != kind) {
        def = nullptr;
        return Fail(AppearanceErrorCode::ContentKindMismatch, id.Value());
    }
    return std::nullopt;
}

/// `provider` empty or unknown means "no provider check" (reported separately).
Issue CheckProviderFit(const ContentDef& def, const ContentId& provider) {
    if (!provider.IsEmpty() && !AppearanceCatalog::Supports(def.providers, provider)) {
        return Fail(AppearanceErrorCode::ContentIncompatibleWithProvider, def.id.Value());
    }
    return std::nullopt;
}

/// Hair / facial hair / body-independent single reference.
Issue CheckStyle(const AppearanceCatalog& catalog, const ContentId& id, ContentKind kind,
                 const ContentId& provider) {
    const ContentDef* def = nullptr;
    if (Issue issue = FindContentOfKind(catalog, id, kind, def)) {
        return issue;
    }
    return CheckProviderFit(*def, provider);
}

Issue CheckParameter(const AppearanceCatalog& catalog, const ParameterId& id, ParamValue value,
                     const ContentId& provider) {
    const ParameterDef* def = catalog.FindParameter(id);
    if (def == nullptr) {
        return Fail(AppearanceErrorCode::UnknownParameter, id.Value());
    }
    if (value < def->min || value > def->max) {
        return Fail(AppearanceErrorCode::ParameterOutOfRange, id.Value());
    }
    if (!provider.IsEmpty() && !AppearanceCatalog::Supports(def->providers, provider)) {
        return Fail(AppearanceErrorCode::ParameterUnsupportedByProvider, id.Value());
    }
    return std::nullopt;
}

Issue CheckClothing(const AppearanceCatalog& catalog, ClothingSlot slot, const ContentId& item,
                    const ContentId& provider) {
    const ContentDef* def = nullptr;
    if (Issue issue = FindContentOfKind(catalog, item, ContentKind::Clothing, def)) {
        return issue;
    }
    if (def->slot != slot) {
        return Fail(AppearanceErrorCode::ClothingSlotMismatch, item.Value());
    }
    return CheckProviderFit(*def, provider);
}

Issue CheckLayer(const AppearanceCatalog& catalog, const AppearanceLayer& layer,
                 const ContentId& provider) {
    const ContentDef* def = nullptr;
    if (Issue issue = FindContentOfKind(catalog, layer.content, ContentKind::Layer, def)) {
        return issue;
    }
    if (def->layerKind != layer.kind) {
        return Fail(AppearanceErrorCode::LayerKindMismatch, layer.content.Value());
    }
    if (layer.opacity < kMinLayerOpacity || layer.opacity > kMaxLayerOpacity) {
        return Fail(AppearanceErrorCode::OpacityOutOfRange, layer.content.Value());
    }
    return CheckProviderFit(*def, provider);
}

bool SameLayerSlot(const AppearanceLayer& a, const AppearanceLayer& b) {
    return a.kind == b.kind && a.content == b.content && a.region == b.region;
}

/// The body provider the other checks are measured against; empty if it is unusable.
ContentId UsableProvider(const AppearanceSpec& spec, const AppearanceCatalog& catalog) {
    const ContentDef* def = catalog.FindContent(spec.bodyProvider);
    return def != nullptr && def->kind == ContentKind::BodyProvider ? spec.bodyProvider
                                                                    : ContentId{};
}

} // namespace

std::vector<AppearanceError> ValidateAppearance(const AppearanceSpec& spec,
                                                const AppearanceCatalog& catalog) {
    std::vector<AppearanceError> issues;
    const auto add = [&issues](Issue issue) {
        if (issue) {
            issues.push_back(std::move(*issue));
        }
    };

    if (spec.schemaVersion != kAppearanceSchemaVersion) {
        issues.push_back(Fail(AppearanceErrorCode::UnsupportedSchemaVersion,
                              std::to_string(spec.schemaVersion)));
    }

    if (spec.bodyProvider.IsEmpty()) {
        issues.push_back(Fail(AppearanceErrorCode::MissingBodyProvider));
    } else {
        const ContentDef* body = nullptr;
        add(FindContentOfKind(catalog, spec.bodyProvider, ContentKind::BodyProvider, body));
    }
    const ContentId provider = UsableProvider(spec, catalog);

    if (!spec.hairStyle.IsEmpty()) {
        add(CheckStyle(catalog, spec.hairStyle, ContentKind::HairStyle, provider));
    }
    if (!spec.facialHairStyle.IsEmpty()) {
        add(CheckStyle(catalog, spec.facialHairStyle, ContentKind::FacialHairStyle, provider));
    }

    if (spec.parameters.size() > kMaxAppearanceParameters) {
        issues.push_back(
            Fail(AppearanceErrorCode::TooManyParameters, std::to_string(spec.parameters.size())));
    }
    for (const auto& [id, value] : spec.parameters) {
        add(CheckParameter(catalog, id, value, provider));
    }
    for (const auto& [slot, item] : spec.clothing) {
        add(CheckClothing(catalog, slot, item, provider));
    }

    if (spec.layers.size() > kMaxAppearanceLayers) {
        issues.push_back(
            Fail(AppearanceErrorCode::TooManyLayers, std::to_string(spec.layers.size())));
    }
    for (std::size_t i = 0; i < spec.layers.size(); ++i) {
        add(CheckLayer(catalog, spec.layers[i], provider));
        const bool duplicate = std::any_of(
            spec.layers.begin(), spec.layers.begin() + static_cast<std::ptrdiff_t>(i),
            [&](const AppearanceLayer& earlier) { return SameLayerSlot(earlier, spec.layers[i]); });
        if (duplicate) {
            issues.push_back(
                Fail(AppearanceErrorCode::DuplicateLayer, spec.layers[i].content.Value()));
        }
    }
    return issues;
}

std::vector<std::string> ReferencedNamespaces(const AppearanceSpec& spec) {
    std::set<std::string> names;
    const auto note = [&names](const ContentId& id) {
        if (!id.IsEmpty()) {
            names.emplace(id.Namespace());
        }
    };
    note(spec.bodyProvider);
    note(spec.hairStyle);
    note(spec.facialHairStyle);
    for (const auto& entry : spec.parameters) {
        note(entry.first);
    }
    for (const auto& entry : spec.clothing) {
        note(entry.second);
    }
    for (const AppearanceLayer& layer : spec.layers) {
        note(layer.content);
    }
    return {names.begin(), names.end()};
}

Result<AvailableAppearance, AppearanceError>
MakeAvailableAppearance(const AppearanceSpec& spec, const AppearanceCatalog& catalog) {
    if (spec.schemaVersion != kAppearanceSchemaVersion) {
        return Fail(AppearanceErrorCode::UnsupportedSchemaVersion,
                    std::to_string(spec.schemaVersion));
    }
    if (spec.bodyProvider.IsEmpty()) {
        return Fail(AppearanceErrorCode::MissingBodyProvider);
    }
    const ContentDef* body = nullptr;
    if (Issue issue =
            FindContentOfKind(catalog, spec.bodyProvider, ContentKind::BodyProvider, body)) {
        return std::move(*issue);
    }

    AvailableAppearance view;
    view.spec.bodyProvider = spec.bodyProvider;
    const ContentId& provider = spec.bodyProvider;
    const auto keep = [&view](Issue issue) {
        if (issue) {
            view.omitted.push_back(std::move(*issue));
            return false;
        }
        return true;
    };

    if (!spec.hairStyle.IsEmpty() &&
        keep(CheckStyle(catalog, spec.hairStyle, ContentKind::HairStyle, provider))) {
        view.spec.hairStyle = spec.hairStyle;
    }
    if (!spec.facialHairStyle.IsEmpty() &&
        keep(CheckStyle(catalog, spec.facialHairStyle, ContentKind::FacialHairStyle, provider))) {
        view.spec.facialHairStyle = spec.facialHairStyle;
    }
    for (const auto& [id, value] : spec.parameters) {
        if (keep(CheckParameter(catalog, id, value, provider)) &&
            view.spec.parameters.size() < kMaxAppearanceParameters) {
            view.spec.parameters.emplace(id, value);
        }
    }
    for (const auto& [slot, item] : spec.clothing) {
        if (keep(CheckClothing(catalog, slot, item, provider))) {
            view.spec.clothing.emplace(slot, item);
        }
    }
    for (const AppearanceLayer& layer : spec.layers) {
        if (!keep(CheckLayer(catalog, layer, provider))) {
            continue;
        }
        const bool duplicate =
            std::ranges::any_of(view.spec.layers, [&layer](const AppearanceLayer& other) {
                return SameLayerSlot(other, layer);
            });
        if (duplicate) {
            view.omitted.push_back(
                Fail(AppearanceErrorCode::DuplicateLayer, layer.content.Value()));
        } else if (view.spec.layers.size() >= kMaxAppearanceLayers) {
            view.omitted.push_back(Fail(AppearanceErrorCode::TooManyLayers, layer.content.Value()));
        } else {
            view.spec.layers.push_back(layer);
        }
    }
    return view;
}

// --- Serialization ----------------------------------------------------------

std::string SerializeAppearance(const AppearanceSpec& spec) {
    std::string out;
    out += kMagic;
    out += ' ';
    out += std::to_string(kAppearanceSchemaVersion);
    out += '\n';
    const auto line = [&out](std::string_view keyword, const std::string& value) {
        out += keyword;
        out += kTab;
        out += value;
        out += '\n';
    };
    line("body", IdText(spec.bodyProvider));
    line("hair", IdText(spec.hairStyle));
    line("facial_hair", IdText(spec.facialHairStyle));
    for (const auto& [id, value] : spec.parameters) {
        line("param", id.Value() + kTab + std::to_string(value));
    }
    for (const auto& [slot, item] : spec.clothing) {
        line("clothing", std::string(ToString(slot)) + kTab + IdText(item));
    }
    for (const AppearanceLayer& layer : spec.layers) {
        line("layer", std::string(ToString(layer.kind)) + kTab + IdText(layer.content) + kTab +
                          std::string(ToString(layer.region)) + kTab +
                          std::to_string(layer.opacity));
    }
    const std::size_t count =
        3 + spec.parameters.size() + spec.clothing.size() + spec.layers.size();
    line("end", std::to_string(count));
    return out;
}

namespace {

/// Where the loader is in the fixed line order:
/// header, body, hair, facial_hair, param*, clothing*, layer*, end.
enum class Section : std::uint8_t { Params, Clothing, Layers };

std::optional<ContentId> ParseRequiredId(std::string_view token) {
    return ContentId::Parse(token);
}

/// "-" is the empty id; anything else must be a valid id.
std::optional<ContentId> ParseOptionalId(std::string_view token) {
    if (token == "-") {
        return ContentId{};
    }
    return ContentId::Parse(token);
}

} // namespace

Result<AppearanceSpec, AppearanceError> DeserializeAppearance(std::string_view text) {
    AppearanceSpec spec;
    std::size_t position = 0;
    std::size_t lineNumber = 0;
    Section section = Section::Params;
    std::optional<ParameterId> lastParameter;
    std::optional<ClothingSlot> lastSlot;

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
            const std::string prefix = std::string(kMagic) + ' ';
            if (!line.starts_with(prefix)) {
                return Fail(AppearanceErrorCode::InvalidHeader, {}, lineNumber);
            }
            const auto version = ParseCanonicalInteger<ParamValue>(line.substr(prefix.size()));
            if (!version) {
                return Fail(AppearanceErrorCode::InvalidHeader, {}, lineNumber);
            }
            if (std::cmp_not_equal(*version, kAppearanceSchemaVersion)) {
                return Fail(AppearanceErrorCode::UnsupportedVersion, {}, lineNumber);
            }
            continue;
        }

        const std::vector<std::string_view> fields = SplitTabs(line);
        const std::string_view keyword = fields.front();

        // Lines 2-4 are fixed: body, hair, facial_hair.
        if (lineNumber <= 4) {
            constexpr std::array<std::string_view, 3> kFixed = {"body", "hair", "facial_hair"};
            if (keyword != kFixed.at(lineNumber - 2) || fields.size() != 2) {
                return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
            }
            if (lineNumber == 2) {
                const auto id = ParseRequiredId(fields[1]);
                if (!id) {
                    return Fail(AppearanceErrorCode::InvalidToken, fields[1], lineNumber);
                }
                spec.bodyProvider = *id;
            } else {
                const auto id = ParseOptionalId(fields[1]);
                if (!id) {
                    return Fail(AppearanceErrorCode::InvalidToken, fields[1], lineNumber);
                }
                (lineNumber == 3 ? spec.hairStyle : spec.facialHairStyle) = *id;
            }
            continue;
        }

        if (keyword == "end") {
            if (fields.size() != 2) {
                return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
            }
            const auto count = ParseCanonicalInteger<ParamValue>(fields[1]);
            if (!count) {
                return Fail(AppearanceErrorCode::InvalidToken, fields[1], lineNumber);
            }
            if (static_cast<std::size_t>(*count) != lineNumber - 2 || *count < 0) {
                return Fail(AppearanceErrorCode::CountMismatch, {}, lineNumber);
            }
            if (position != text.size()) {
                return Fail(AppearanceErrorCode::TrailingData, {}, lineNumber + 1);
            }
            return spec;
        }

        if (keyword == "param") {
            if (fields.size() != 3) {
                return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
            }
            if (section != Section::Params) {
                return Fail(AppearanceErrorCode::NonCanonical, keyword, lineNumber);
            }
            const auto id = ParseRequiredId(fields[1]);
            const auto value = ParseCanonicalInteger<ParamValue>(fields[2]);
            if (!id || !value) {
                return Fail(AppearanceErrorCode::InvalidToken, !id ? fields[1] : fields[2],
                            lineNumber);
            }
            if (lastParameter && !(*lastParameter < *id)) {
                return Fail(AppearanceErrorCode::NonCanonical, fields[1], lineNumber);
            }
            if (spec.parameters.size() >= kMaxAppearanceParameters) {
                return Fail(AppearanceErrorCode::TooManyParameters, {}, lineNumber);
            }
            lastParameter = id;
            spec.parameters.emplace(*id, *value);
        } else if (keyword == "clothing") {
            if (fields.size() != 3) {
                return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
            }
            if (section == Section::Layers) {
                return Fail(AppearanceErrorCode::NonCanonical, keyword, lineNumber);
            }
            section = Section::Clothing;
            const auto slot = ParseEnum(fields[1], kAllClothingSlots);
            const auto item = ParseRequiredId(fields[2]);
            if (!slot || !item) {
                return Fail(AppearanceErrorCode::InvalidToken, !slot ? fields[1] : fields[2],
                            lineNumber);
            }
            if (lastSlot && !(*lastSlot < *slot)) {
                return Fail(AppearanceErrorCode::NonCanonical, fields[1], lineNumber);
            }
            lastSlot = slot;
            spec.clothing.emplace(*slot, *item);
        } else if (keyword == "layer") {
            if (fields.size() != 5) {
                return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
            }
            section = Section::Layers;
            const auto kind = ParseEnum(fields[1], kAllLayerKinds);
            const auto content = ParseRequiredId(fields[2]);
            const auto region = ParseEnum(fields[3], kAllBodyRegions);
            const auto opacity = ParseCanonicalInteger<ParamValue>(fields[4]);
            if (!kind || !content || !region || !opacity) {
                const std::string_view bad = !kind      ? fields[1]
                                             : !content ? fields[2]
                                             : !region  ? fields[3]
                                                        : fields[4];
                return Fail(AppearanceErrorCode::InvalidToken, bad, lineNumber);
            }
            if (spec.layers.size() >= kMaxAppearanceLayers) {
                return Fail(AppearanceErrorCode::TooManyLayers, {}, lineNumber);
            }
            spec.layers.push_back(AppearanceLayer{*kind, *content, *region, *opacity});
        } else {
            return Fail(AppearanceErrorCode::MalformedLine, {}, lineNumber);
        }
    }
    return Fail(lineNumber == 0 ? AppearanceErrorCode::InvalidHeader
                                : AppearanceErrorCode::TruncatedData,
                {}, lineNumber);
}

} // namespace spn::core
