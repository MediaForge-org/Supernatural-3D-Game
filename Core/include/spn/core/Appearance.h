#pragma once

// Appearance data model: WHAT a character looks like, as pure data.
//
// ENGINEERING PROPOSAL, not master canon. Parameter names, ranges, regions and slots
// are placeholders for review (see Docs/Decisions/0005). Boundaries:
//  - This header knows nothing about NPC simulation, WorldFacts, Unreal or meshes.
//  - Rendering adapters (later, Unreal side) read an AppearanceSpec and map parameters
//    and content ids onto a concrete body provider (MetaHuman, modular Blender parts, ...).
//  - NPC simulation refers to a character's appearance only by holding a spec or a seed.

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Result.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

/// Parameter values are fixed-point integers, never floats: they serialize exactly and
/// behave identically on every compiler/platform. Their physical meaning is defined by
/// the rendering adapter, not here. Convention used by the built-in set: morph-like
/// parameters are bipolar [-1000, 1000] with default 0; amount/colour parameters are
/// unipolar [0, 1000].
using ParamValue = std::int32_t;
using ParameterId = ContentId;

inline constexpr std::uint32_t kAppearanceSchemaVersion = 1;
inline constexpr std::size_t kMaxAppearanceLayers = 32;
inline constexpr std::size_t kMaxAppearanceParameters = 512;
inline constexpr ParamValue kMinLayerOpacity = 0;
inline constexpr ParamValue kMaxLayerOpacity = 1000;

// --- Vocabulary ---------------------------------------------------------------

enum class ParameterGroup : std::uint8_t { Face, Body, Skin, Eyes, Hair, Custom };

enum class ClothingSlot : std::uint8_t { Head, Torso, Legs, Feet, Hands, Outer, Accessory };

/// Skin/appearance layers painted over the base. Wounds are runtime injury state, not
/// part of the spec (OPEN: how injury canon maps onto layers).
enum class LayerKind : std::uint8_t { Tattoo, Scar, Birthmark, Makeup, Dirt };

enum class BodyRegion : std::uint8_t {
    Head,
    Face,
    Neck,
    Torso,
    Back,
    LeftArm,
    RightArm,
    LeftHand,
    RightHand,
    LeftLeg,
    RightLeg
};

/// What kind of thing a ContentId refers to.
enum class ContentKind : std::uint8_t { BodyProvider, HairStyle, FacialHairStyle, Clothing, Layer };

inline constexpr std::array kAllClothingSlots = {
    ClothingSlot::Head,  ClothingSlot::Torso, ClothingSlot::Legs,     ClothingSlot::Feet,
    ClothingSlot::Hands, ClothingSlot::Outer, ClothingSlot::Accessory};
inline constexpr std::array kAllLayerKinds = {
    LayerKind::Tattoo, LayerKind::Scar, LayerKind::Birthmark, LayerKind::Makeup, LayerKind::Dirt};
inline constexpr std::array kAllBodyRegions = {
    BodyRegion::Head,      BodyRegion::Face,    BodyRegion::Neck,     BodyRegion::Torso,
    BodyRegion::Back,      BodyRegion::LeftArm, BodyRegion::RightArm, BodyRegion::LeftHand,
    BodyRegion::RightHand, BodyRegion::LeftLeg, BodyRegion::RightLeg};

[[nodiscard]] std::string_view ToString(ParameterGroup value) noexcept;
[[nodiscard]] std::string_view ToString(ClothingSlot value) noexcept;
[[nodiscard]] std::string_view ToString(LayerKind value) noexcept;
[[nodiscard]] std::string_view ToString(BodyRegion value) noexcept;
[[nodiscard]] std::string_view ToString(ContentKind value) noexcept;

// --- Errors -------------------------------------------------------------------

enum class AppearanceErrorCode : std::uint8_t {
    // Validating a spec against a catalog
    UnsupportedSchemaVersion,
    MissingBodyProvider,
    UnknownContent,
    ContentKindMismatch,
    ClothingSlotMismatch,
    LayerKindMismatch,
    UnknownParameter,
    ParameterOutOfRange,
    OpacityOutOfRange,
    DuplicateLayer,
    TooManyLayers,
    // Building a catalog
    InvalidId,
    DuplicateDefinition,
    InvalidParameterRange,
    // Loading saved data
    InvalidHeader,
    UnsupportedVersion,
    TruncatedData,
    TrailingData,
    MalformedLine,
    InvalidToken,
    NonCanonical,
    CountMismatch,
    // Generating
    EmptyCatalog,
    InvalidGenerationOption,
    // Added in the review of Phase 0.4 (appended: existing values stay stable)
    ParameterUnsupportedByProvider,
    ContentIncompatibleWithProvider,
    TooManyParameters,
    UnknownProviderReference,
    UnsupportedGeneratorVersion,
    CatalogChanged,
    IndexOutOfRange,
};

[[nodiscard]] std::string_view ToString(AppearanceErrorCode code) noexcept;

struct AppearanceError {
    AppearanceErrorCode code;
    std::string subject;  // offending id/parameter/token; empty if not applicable
    std::size_t line = 0; // 1-based line when loading saved data; 0 otherwise
    friend bool operator==(const AppearanceError&, const AppearanceError&) = default;
};

/// One-line human-readable description, e.g. "parameter out of range: base:face.jaw_width".
[[nodiscard]] std::string Describe(const AppearanceError& error);

// --- Spec ---------------------------------------------------------------------

struct AppearanceLayer {
    LayerKind kind = LayerKind::Tattoo;
    ContentId content;
    BodyRegion region = BodyRegion::Torso;
    ParamValue opacity = kMaxLayerOpacity; // 0..1000
    friend bool operator==(const AppearanceLayer&, const AppearanceLayer&) = default;
};

/// Complete description of one character's looks. Plain value type; absent parameters
/// mean "catalog default". Containers are ordered, so equal specs serialize identically.
struct AppearanceSpec {
    std::uint32_t schemaVersion = kAppearanceSchemaVersion;
    ContentId bodyProvider;    // required; pipeline-agnostic (not tied to MetaHuman)
    ContentId hairStyle;       // empty: none
    ContentId facialHairStyle; // empty: none
    std::map<ParameterId, ParamValue> parameters;
    std::map<ClothingSlot, ContentId> clothing; // at most one item per slot by construction
    std::vector<AppearanceLayer> layers;        // order = stacking order, bottom first
    friend bool operator==(const AppearanceSpec&, const AppearanceSpec&) = default;
};

// --- Catalog (what exists; extendable by mods) ----------------------------------

/// How NPC generation distributes values of a parameter (a generation hint only; it
/// never restricts what a spec may contain).
enum class GenerationShape : std::uint8_t {
    Bell,         // mean of three uniform draws over [min, max]
    Uniform,      // uniform over [min, max]
    TowardDefault // clustered at the default, rare towards the far end (e.g. freckles, grey hair)
};

[[nodiscard]] std::string_view ToString(GenerationShape value) noexcept;

struct ParameterDef {
    ParameterId id;
    ParameterGroup group = ParameterGroup::Custom;
    ParamValue min = 0;
    ParamValue max = 1000;
    ParamValue defaultValue = 0;
    GenerationShape shape = GenerationShape::Bell;
    std::vector<ContentId> providers; // empty = all body providers
};

struct ContentDef {
    ContentId id;
    ContentKind kind = ContentKind::BodyProvider;
    ClothingSlot slot = ClothingSlot::Torso; // used only when kind == Clothing
    LayerKind layerKind = LayerKind::Tattoo; // used only when kind == Layer
    std::vector<ContentId> providers;        // empty = all body providers; ignored for providers
};

/// Which parameters a body provider supports and which parts fit it is data on the
/// parameter/content side (`providers`): empty = works with every body provider, otherwise
/// only with the listed ones. A mod adding clothing for one body provider just lists it.

/// Registry of the parameters and content a spec may refer to. The base game fills it,
/// mods add entries under their own namespace. Duplicate ids are rejected, never
/// silently replaced (override policy is OPEN).
class AppearanceCatalog {
public:
    [[nodiscard]] std::optional<AppearanceError> AddParameter(ParameterDef def);
    [[nodiscard]] std::optional<AppearanceError> AddContent(ContentDef def);

    [[nodiscard]] const ParameterDef* FindParameter(const ParameterId& id) const;
    [[nodiscard]] const ContentDef* FindContent(const ContentId& id) const;

    [[nodiscard]] const std::map<ParameterId, ParameterDef>& Parameters() const noexcept {
        return parameters_;
    }
    [[nodiscard]] const std::map<ContentId, ContentDef>& Content() const noexcept {
        return content_;
    }

    /// Checks cross-references between definitions (every `providers` entry must be a
    /// known body provider). Run once after all packs/mods have been added.
    [[nodiscard]] std::vector<AppearanceError> ValidateConsistency() const;

    /// True if the parameter / content works with `provider` (see `providers`).
    [[nodiscard]] static bool Supports(const std::vector<ContentId>& providers,
                                       const ContentId& provider);

    /// Hash over all definitions. Lets a save notice that the catalog (mods!) changed
    /// since a seed-generated character was first created.
    [[nodiscard]] std::uint64_t Fingerprint() const;

private:
    std::map<ParameterId, ParameterDef> parameters_;
    std::map<ContentId, ContentDef> content_;
};

/// The proposed base parameter set (face, body, skin, eyes, hair). Placeholder names and
/// ranges for review; contains no content (parts come from data/mods).
[[nodiscard]] std::vector<ParameterDef> BuiltinParameterDefs();

// --- Validation and persistence -------------------------------------------------

/// Checks a spec against a catalog and reports every problem, in a deterministic order
/// (empty = valid). Structural identity of ids is guaranteed by ContentId itself.
[[nodiscard]] std::vector<AppearanceError> ValidateAppearance(const AppearanceSpec& spec,
                                                              const AppearanceCatalog& catalog);

/// Sorted namespaces ("base", "mod_x", ...) the spec refers to. The mod layer uses this to
/// decide which mods a character needs (canon: critical missing mods block save loading).
[[nodiscard]] std::vector<std::string> ReferencedNamespaces(const AppearanceSpec& spec);

/// A spec with every part removed that the current catalog cannot provide (disabled or
/// missing mod, wrong body provider, out-of-range value). The stored spec is never
/// modified: the omitted references stay in the save and come back once the mod is
/// enabled again (canon: disabled mods keep their preserved state). `omitted` lists what
/// was left out. Fails only if the body provider itself is unusable, because choosing a
/// fallback body is a policy decision of the caller (OPEN).
struct AvailableAppearance {
    AppearanceSpec spec;
    std::vector<AppearanceError> omitted;
};
[[nodiscard]] Result<AvailableAppearance, AppearanceError>
MakeAvailableAppearance(const AppearanceSpec& spec, const AppearanceCatalog& catalog);

/// Canonical text form ("SPNAPPEARANCE 1"), see Docs/Decisions/0005.
[[nodiscard]] std::string SerializeAppearance(const AppearanceSpec& spec);

/// Strict, canonical-only parser. Checks syntax and canonical form only; whether the
/// referenced content exists (e.g. a mod is missing) is a separate ValidateAppearance
/// question, so "corrupt file" and "missing mod" stay distinguishable.
[[nodiscard]] Result<AppearanceSpec, AppearanceError> DeserializeAppearance(std::string_view text);

} // namespace spn::core
