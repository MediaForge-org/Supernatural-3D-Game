#pragma once

// Body coverage (character architecture, developer order 2026-11-15; Docs/Proposals/0018): which body zones the worn items cover, which body
// geometry may be hidden under them, and which OPTIONAL body detail geometry may be shown. Pure rules, no engine code, no persistence:
// nothing here is saved (the save format of the appearance is AppearanceSpec, Docs/Decisions/0005, unchanged).
//
// Architecture rules this file enforces (Docs/Game/Adult/02 "ein gemeinsamer Human Core", 07 "kein zweiter Charaktergenerator"):
//   * the body is one mesh without clothing; every wearable is its own mesh on its own layer (Underwear < Base < Outer < Accessory) and is
//     never merged into the body - so a wearable here is only DATA (what it covers / hides), never geometry;
//   * body geometry is hidden ONLY where an opaque wearable explicitly says it hides it (no hidden body without a wearer of the zone);
//   * optional detail geometry (any kind: scars with relief, piercings, anatomical detail, prosthetics ...) is generic content in a zone.
//     It is created only when the presentation policy allows details of its CATEGORY and its zone is not covered by an opaque wearable -
//     so a dressed character never pays for it. The core knows no content pack and asks no "is pack X installed" question: categories
//     are plain ids registered by whatever pack provides them; the policy (settings / content gate) is decided outside the core.
// All zone names and numbers are PLACEHOLDERS (CLAUDE.md section 3) for review, not a final anatomy model.

#include <cstdint>
#include <string>
#include <vector>

#include <spn/core/ContentId.h>

namespace spn::core {

/// Body zones for coverage and hiding. Finer than BodyRegion (Appearance.h, used for layers); a body mesh that wants per-zone hiding is
/// split (or masked) along these zones. Bit index = enum value.
enum class BodyZone : std::uint8_t {
    Head, Face, Neck, Chest, Abdomen, Back, Pelvis, Buttocks,
    UpperArms, Forearms, Hands, Thighs, Knees, Calves, Feet,
    Count
};
using ZoneMask = std::uint32_t;

[[nodiscard]] constexpr ZoneMask Zone(BodyZone z) noexcept { return ZoneMask{1} << static_cast<unsigned>(z); }
[[nodiscard]] constexpr ZoneMask AllZones() noexcept { return (ZoneMask{1} << static_cast<unsigned>(BodyZone::Count)) - 1; }
[[nodiscard]] const char* BodyZoneName(BodyZone zone) noexcept;

/// Stacking layer of a wearable, inner to outer. Exactly one wearable per (slot, layer) is the editor's rule, not this file's.
enum class WearLayer : std::uint8_t { Underwear, Base, Outer, Accessory };
[[nodiscard]] const char* WearLayerName(WearLayer layer) noexcept;

/// What one worn item does to the body. `hidesBody` must lie inside `covers` (you cannot hide what you do not cover).
struct WearableCoverage {
    ContentId item;
    WearLayer layer = WearLayer::Base;
    ZoneMask covers = 0;       ///< zones the item visually covers
    ZoneMask hidesBody = 0;    ///< zones whose BODY geometry may be removed under it (tight, opaque, no gaps); subset of covers
    bool opaque = true;        ///< false: sheer / mesh / torn - covers nothing for detail gating and hides nothing
};

/// Optional detail geometry for one zone. `category` is a free id ("base:scar_relief", "pack_x:...") the policy enables or not.
struct BodyDetailDef {
    ContentId id;
    ContentId category;
    BodyZone zone = BodyZone::Chest;
};

/// Decided outside the core (settings, platform build, installed packs); the core only applies it.
struct PresentationPolicy {
    std::vector<ContentId> enabledDetailCategories;   ///< empty: no optional detail geometry at all (the default)
    bool hideCoveredBody = true;                      ///< false: never remove body geometry (debug / sheer clothing tests)
};

enum class CoverageIssueKind : std::uint8_t {
    HidesUncoveredZone,     ///< hidesBody has zones outside covers (ignored for those zones)
    TransparentHides,       ///< a non-opaque item claims to hide body geometry (ignored)
    LayerConflict,          ///< two items on the same layer cover the same zone (both kept; the editor should prevent it)
};

struct CoverageIssue {
    CoverageIssueKind kind;
    ContentId item;
    ZoneMask zones = 0;
};

struct CoverageResult {
    ZoneMask covered = 0;          ///< zones covered by at least one opaque wearable
    ZoneMask hiddenBody = 0;       ///< zones whose body geometry is removed
    ZoneMask exposed = 0;          ///< AllZones() & ~covered
    std::vector<ContentId> details; ///< detail ids to create, in definition order (only enabled categories, only exposed zones)
    std::vector<CoverageIssue> issues;
};

/// Applies the rules above. Deterministic; the order of `worn` does not change the result (issues are reported in input order).
[[nodiscard]] CoverageResult ResolveCoverage(const std::vector<WearableCoverage>& worn, const std::vector<BodyDetailDef>& details,
                                             const PresentationPolicy& policy);

} // namespace spn::core
