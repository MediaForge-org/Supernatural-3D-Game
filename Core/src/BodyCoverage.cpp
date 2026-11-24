#include <spn/core/BodyCoverage.h>

#include <algorithm>
#include <array>

namespace spn::core {

const char* BodyZoneName(BodyZone zone) noexcept {
    static constexpr std::array<const char*, static_cast<std::size_t>(BodyZone::Count)> kNames{
        "head", "face", "neck", "chest", "abdomen", "back", "pelvis", "buttocks",
        "upper_arms", "forearms", "hands", "thighs", "knees", "calves", "feet"};
    const auto i = static_cast<std::size_t>(zone);
    return i < kNames.size() ? kNames[i] : "unknown";
}

const char* WearLayerName(WearLayer layer) noexcept {
    switch (layer) {
    case WearLayer::Underwear: return "underwear";
    case WearLayer::Base: return "base";
    case WearLayer::Outer: return "outer";
    case WearLayer::Accessory: return "accessory";
    }
    return "unknown";
}

CoverageResult ResolveCoverage(const std::vector<WearableCoverage>& worn, const std::vector<BodyDetailDef>& details,
                               const PresentationPolicy& policy) {
    CoverageResult r;
    std::array<ZoneMask, 4> perLayer{};   // opaque coverage per layer, for conflict reports
    for (const WearableCoverage& w : worn) {
        const ZoneMask covers = w.covers & AllZones();
        if ((w.hidesBody & ~covers) != 0) {
            r.issues.push_back({CoverageIssueKind::HidesUncoveredZone, w.item, w.hidesBody & ~covers});
        }
        if (!w.opaque) {
            if (w.hidesBody != 0) {
                r.issues.push_back({CoverageIssueKind::TransparentHides, w.item, w.hidesBody});
            }
            continue;   // sheer: neither covers (for detail gating) nor hides
        }
        auto& layerMask = perLayer[static_cast<std::size_t>(w.layer)];
        if ((layerMask & covers) != 0) {
            r.issues.push_back({CoverageIssueKind::LayerConflict, w.item, layerMask & covers});
        }
        layerMask |= covers;
        r.covered |= covers;
        if (policy.hideCoveredBody) {
            r.hiddenBody |= w.hidesBody & covers;
        }
    }
    r.exposed = AllZones() & ~r.covered;
    for (const BodyDetailDef& d : details) {
        const bool enabled = std::find(policy.enabledDetailCategories.begin(), policy.enabledDetailCategories.end(), d.category) !=
                             policy.enabledDetailCategories.end();
        if (enabled && (r.exposed & Zone(d.zone)) != 0) {
            r.details.push_back(d.id);
        }
    }
    return r;
}

} // namespace spn::core
