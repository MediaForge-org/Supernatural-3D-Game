#pragma once

// How a character's looks are STORED: either a full spec (authored characters) or a
// recipe = seed + generation settings + hand-made overrides (generic NPCs). A recipe is
// tiny, so crowds stay cheap in saves and in co-op sync (Docs/Proposals/0003, guardrail 2).
//
// ENGINEERING PROPOSAL, not master canon.

#include <cstdint>
#include <map>
#include <optional>
#include <spn/core/Appearance.h>
#include <spn/core/AppearanceGenerator.h>
#include <spn/core/Result.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace spn::core {

/// Deliberate deviations from what the seed generates. Every field is "unchanged" by
/// default; this is also what a future character editor stores for a generic NPC.
struct AppearanceOverrides {
    std::optional<ContentId> bodyProvider;        // present: replace (must be non-empty)
    std::optional<ContentId> hairStyle;           // present: replace; empty id = no hair
    std::optional<ContentId> facialHairStyle;     // present: replace; empty id = none
    std::map<ParameterId, ParamValue> parameters; // set (also parameters the seed did not draw)
    std::map<ClothingSlot, ContentId> clothing;   // empty id = remove the slot's item
    std::optional<std::vector<AppearanceLayer>> layers; // present: replace ALL layers

    [[nodiscard]] bool IsEmpty() const noexcept {
        return !bodyProvider && !hairStyle && !facialHairStyle && parameters.empty() &&
               clothing.empty() && !layers;
    }
    friend bool operator==(const AppearanceOverrides&, const AppearanceOverrides&) = default;
};

/// Everything needed to reproduce a generated character:
/// (seed, generator version, options, catalog) -> spec, then overrides on top.
struct AppearanceRecipe {
    std::uint64_t seed = 0;
    std::uint32_t generatorVersion = kAppearanceGeneratorVersion;
    /// Catalog fingerprint at creation. If present it must still match when resolving
    /// (see FingerprintPolicy), because content choices depend on what the catalog offers.
    std::optional<std::uint64_t> catalogFingerprint;
    GenerationOptions options;
    AppearanceOverrides overrides;
    friend bool operator==(const AppearanceRecipe&, const AppearanceRecipe&) = default;
};

/// Authored characters hold a spec, generic ones a recipe.
using AppearanceSource = std::variant<AppearanceSpec, AppearanceRecipe>;

enum class FingerprintPolicy : std::uint8_t {
    Require, // a changed catalog is an error (CatalogChanged): the safe default
    Ignore   // regenerate against the current catalog anyway (parts may differ)
};

/// New recipe for `seed` that records the current generator version and catalog fingerprint.
[[nodiscard]] AppearanceRecipe MakeRecipe(const AppearanceCatalog& catalog, std::uint64_t seed,
                                          const GenerationOptions& options = {});

/// generate -> apply overrides -> validate. Errors: UnsupportedGeneratorVersion,
/// CatalogChanged, any generation error, or the first validation issue of the result.
[[nodiscard]] Result<AppearanceSpec, AppearanceError>
ResolveRecipe(const AppearanceCatalog& catalog, const AppearanceRecipe& recipe,
              FingerprintPolicy policy = FingerprintPolicy::Require);

/// A spec is validated and returned; a recipe is resolved.
[[nodiscard]] Result<AppearanceSpec, AppearanceError>
ResolveAppearance(const AppearanceCatalog& catalog, const AppearanceSource& source,
                  FingerprintPolicy policy = FingerprintPolicy::Require);

/// Canonical text form "SPNRECIPE 1", see Docs/Decisions/0005. Strict like the spec format.
[[nodiscard]] std::string SerializeRecipe(const AppearanceRecipe& recipe);
[[nodiscard]] Result<AppearanceRecipe, AppearanceError> DeserializeRecipe(std::string_view text);

/// Writes either form; Deserialize picks the parser from the header line.
[[nodiscard]] std::string SerializeSource(const AppearanceSource& source);
[[nodiscard]] Result<AppearanceSource, AppearanceError> DeserializeSource(std::string_view text);

} // namespace spn::core
