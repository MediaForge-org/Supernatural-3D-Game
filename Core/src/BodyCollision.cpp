#include <spn/core/BodyCollision.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

bool Finite(float v) noexcept { return std::isfinite(v); }

float ClampReport(float value, float lo, float hi, BodyCollisionIssue issue, std::vector<BodyCollisionIssue>& issues) {
    const float clamped = std::clamp(Finite(value) ? value : lo, lo, hi);
    if (clamped != value) {
        issues.push_back(issue);
    }
    return clamped;
}

} // namespace

BodyCollisionResult ResolveBodyCollision(const BodyCollisionRequest& request, const BodyCollisionRules& rules) {
    BodyCollisionResult result;
    BodyCollisionProfile p{rules.defaultRadiusCm, rules.defaultHalfHeightCm, rules.defaultCrouchedHalfHeightCm};
    const bool knownHeight = Finite(request.bodyHeightCm) && request.bodyHeightCm > 0.f;

    switch (request.mode) {
    case BodyCollisionMode::Keep:
        break;
    case BodyCollisionMode::FromHeight:
        if (!knownHeight) {
            result.issues.push_back(BodyCollisionIssue::NoBodyHeight);
            break;
        }
        p.halfHeightCm = ClampReport((request.bodyHeightCm + rules.headClearanceCm) * 0.5f, rules.minHalfHeightCm, rules.maxHalfHeightCm,
                                     BodyCollisionIssue::HalfHeightClamped, result.issues);
        // crouching keeps the default's proportion (40 / 90): a taller body crouches into a taller capsule
        p.crouchedHalfHeightCm = rules.defaultHalfHeightCm > 0.f
                                     ? rules.defaultCrouchedHalfHeightCm * p.halfHeightCm / rules.defaultHalfHeightCm
                                     : rules.defaultCrouchedHalfHeightCm;
        break;
    case BodyCollisionMode::Explicit:
        p.radiusCm = ClampReport(request.explicitProfile.radiusCm, rules.minRadiusCm, rules.maxRadiusCm, BodyCollisionIssue::RadiusClamped,
                                 result.issues);
        p.halfHeightCm = ClampReport(request.explicitProfile.halfHeightCm, rules.minHalfHeightCm, rules.maxHalfHeightCm,
                                     BodyCollisionIssue::HalfHeightClamped, result.issues);
        p.crouchedHalfHeightCm = request.explicitProfile.crouchedHalfHeightCm;
        break;
    }

    // a capsule's half height can never be below its radius (it would be a sphere); the crouched one never above the standing one
    if (p.halfHeightCm < p.radiusCm) {
        p.halfHeightCm = p.radiusCm;
        result.issues.push_back(BodyCollisionIssue::HalfHeightClamped);
    }
    if (!Finite(p.crouchedHalfHeightCm) || p.crouchedHalfHeightCm < p.radiusCm || p.crouchedHalfHeightCm > p.halfHeightCm) {
        p.crouchedHalfHeightCm = std::clamp(Finite(p.crouchedHalfHeightCm) ? p.crouchedHalfHeightCm : p.radiusCm, p.radiusCm, p.halfHeightCm);
        result.issues.push_back(BodyCollisionIssue::CrouchClamped);
    }

    result.profile = p;
    result.halfHeightDeltaCm = p.halfHeightCm - rules.defaultHalfHeightCm;
    result.headAboveCapsuleCm = knownHeight ? request.bodyHeightCm - 2.f * p.halfHeightCm : 0.f;
    result.differsFromDefault = p.radiusCm != rules.defaultRadiusCm || p.halfHeightCm != rules.defaultHalfHeightCm ||
                                p.crouchedHalfHeightCm != rules.defaultCrouchedHalfHeightCm;
    return result;
}

PassageFit FitPassage(const BodyCollisionProfile& profile, float bodyHeightCm, float clearHeightCm, float clearWidthCm) noexcept {
    PassageFit fit;
    const bool knownHeight = Finite(bodyHeightCm) && bodyHeightCm > 0.f;
    const bool widthOk = 2.f * profile.radiusCm <= clearWidthCm;
    fit.capsuleFitsStanding = widthOk && 2.f * profile.halfHeightCm <= clearHeightCm;
    fit.capsuleFitsCrouched = widthOk && 2.f * profile.crouchedHalfHeightCm <= clearHeightCm;
    fit.headClearsStanding = !knownHeight || bodyHeightCm <= clearHeightCm;
    fit.headroomCm = clearHeightCm - std::max(knownHeight ? bodyHeightCm : 0.f, 2.f * profile.halfHeightCm);
    return fit;
}

const char* BodyCollisionIssueName(BodyCollisionIssue issue) noexcept {
    switch (issue) {
    case BodyCollisionIssue::NoBodyHeight: return "no_body_height";
    case BodyCollisionIssue::HalfHeightClamped: return "half_height_clamped";
    case BodyCollisionIssue::RadiusClamped: return "radius_clamped";
    case BodyCollisionIssue::CrouchClamped: return "crouch_clamped";
    }
    return "unknown";
}

} // namespace spn::core
