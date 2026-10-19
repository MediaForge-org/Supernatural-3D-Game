#pragma once

// Deterministic generation of an AppearanceSpec from a seed.
//
// ENGINEERING PROPOSAL, not master canon. Distributions, chances and limits are
// untuned placeholders. The generator only reads an AppearanceCatalog; it knows
// nothing about NPC simulation, WorldFacts or rendering.
//
// Reproducibility contract: the result is a pure function of
//   (seed, kAppearanceGeneratorVersion, GenerationOptions, catalog contents).
//  - Every parameter has its own random stream keyed by its id, so adding parameters
//    (e.g. from a mod) never changes the values of existing ones.
//  - Choices among content (hair, clothing, layers) index into the id-sorted list of
//    matching catalog entries, so they change if matching content is added or removed.
//    Callers that need a character to stay identical across catalog changes must store
//    the generated spec (or compare AppearanceCatalog::Fingerprint()).
//  - The body provider is drawn first; only parameters and content that fit it (see
//    `providers` in the catalog definitions) are generated.
//  - Changing the algorithm requires bumping kAppearanceGeneratorVersion once the
//    generator has been used for saved data. (Version 1 was revised during the Phase 0.4
//    review, before any consumer existed: parameter shapes and provider filtering.)

#include <cstdint>
#include <spn/core/Appearance.h>
#include <spn/core/Result.h>

namespace spn::core {

inline constexpr std::uint32_t kAppearanceGeneratorVersion = 1;

/// Chances are per mille (0..1000). All values are placeholders.
struct GenerationOptions {
    std::int32_t baldChance = 50;
    std::int32_t facialHairChance = 250;
    std::int32_t tattooChance = 100;
    std::int32_t scarChance = 150;
    std::uint32_t maxTattoos = 3; // when tattoos are drawn: 1..maxTattoos
    std::uint32_t maxScars = 2;   // when scars are drawn: 1..maxScars
    friend bool operator==(const GenerationOptions&, const GenerationOptions&) = default;
};

/// Generates a complete spec (all catalog parameters explicit) that passes
/// ValidateAppearance against the same catalog. Errors: EmptyCatalog if the catalog has
/// no body provider, InvalidGenerationOption for a chance outside 0..1000 or a count
/// above kMaxAppearanceLayers.
[[nodiscard]] Result<AppearanceSpec, AppearanceError>
GenerateAppearance(const AppearanceCatalog& catalog, std::uint64_t seed,
                   const GenerationOptions& options = {});

} // namespace spn::core
