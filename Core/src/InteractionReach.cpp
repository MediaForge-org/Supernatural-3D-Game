#include <spn/core/InteractionReach.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(const ReachVec3& v) noexcept { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }

double Length(const ReachVec3& v) noexcept { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

} // namespace

std::optional<std::string> Validate(const ReachConfig& c) {
    if (!std::isfinite(c.maxDistance) || c.maxDistance <= 0.0 || c.maxDistance > 20.0) return "max distance outside (0, 20] m";
    if (!std::isfinite(c.maxAngleDegrees) || c.maxAngleDegrees <= 0.0 || c.maxAngleDegrees > 180.0) return "max angle outside (0, 180] degrees";
    if (!std::isfinite(c.maxHeightDifference) || c.maxHeightDifference <= 0.0) return "max height difference must be positive";
    return std::nullopt;
}

const char* ReachResultName(ReachResult r) noexcept {
    switch (r) {
    case ReachResult::Ok: return "Ok";
    case ReachResult::TooFar: return "TooFar";
    case ReachResult::OutOfView: return "OutOfView";
    case ReachResult::TooHighOrLow: return "TooHighOrLow";
    case ReachResult::Obstructed: return "Obstructed";
    case ReachResult::InvalidInput: return "InvalidInput";
    }
    return "Unknown";
}

ReachResult CheckReach(const ReachConfig& config, const ReachQuery& q) noexcept {
    // A NaN limit makes every comparison below false, which would read as reachable; an unusable config is invalid input, not a pass.
    if (!std::isfinite(config.maxDistance) || !std::isfinite(config.maxAngleDegrees) || !std::isfinite(config.maxHeightDifference)) return ReachResult::InvalidInput;
    if (!Finite(q.eye) || !Finite(q.viewDirection) || !Finite(q.targetCentre) || !std::isfinite(q.targetRadius) || q.targetRadius < 0.0) return ReachResult::InvalidInput;
    const double viewLength = Length(q.viewDirection);
    if (viewLength < 1e-9) return ReachResult::InvalidInput;
    const ReachVec3 to{q.targetCentre.x - q.eye.x, q.targetCentre.y - q.eye.y, q.targetCentre.z - q.eye.z};
    const double distance = Length(to);
    if (std::abs(to.z) > config.maxHeightDifference + q.targetRadius) return ReachResult::TooHighOrLow;
    if (distance - q.targetRadius > config.maxDistance) return ReachResult::TooFar;
    if (distance > 1e-9) {
        const double cosAngle = std::clamp((to.x * q.viewDirection.x + to.y * q.viewDirection.y + to.z * q.viewDirection.z) / (distance * viewLength), -1.0, 1.0);
        const double angle = std::acos(cosAngle) * 180.0 / kPi;
        const double angular = distance > q.targetRadius ? std::asin(std::min(q.targetRadius / distance, 1.0)) * 180.0 / kPi : 90.0;
        if (angle - angular > config.maxAngleDegrees) return ReachResult::OutOfView;
    }
    if (config.requireLineOfSight && !q.lineOfSightClear) return ReachResult::Obstructed;
    return ReachResult::Ok;
}

bool MayInteract(const ActorInteractState& s) noexcept {
    return !s.incapacitated && !s.traversing && !s.inVehicle && !s.inDialogue;
}

} // namespace spn::core
