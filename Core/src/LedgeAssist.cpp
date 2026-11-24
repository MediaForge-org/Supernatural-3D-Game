#include <spn/core/LedgeAssist.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

bool Finite(double v) noexcept { return std::isfinite(v); }

LedgePlan Refuse(const char* reason, double height = 0.0) noexcept {
    LedgePlan plan;
    plan.reason = reason;
    plan.height = height;
    return plan;
}

double EaseOut(double s) noexcept {
    const double c = std::clamp(s, 0.0, 1.0);
    return 1.0 - (1.0 - c) * (1.0 - c);
}

double LiftUp(const LedgePlan& plan, double t) noexcept {
    return plan.liftSeconds > 0.0 ? plan.riseHeight * EaseOut(t / plan.liftSeconds) : plan.riseHeight;
}

double LiftForward(const LedgePlan& plan, double t) noexcept {
    const double free = plan.liftForwardSpeed * std::max(t, 0.0);
    const double limit = plan.faceAxisDistance - LedgeRequiredAxisDistance(plan, LiftUp(plan, t));
    // not clamped to forwardEnd: a fast step keeps its pace until the hips are up (PlanLedgeAssist extends forwardEnd instead)
    return std::max(std::min(free, limit), 0.0);
}

double LiftSeconds(LedgeMove move, double height, double approachSpeed) noexcept {
    switch (move) {
    case LedgeMove::StepOn: return 0.12 + 0.30 * height;
    case LedgeMove::Vault: return height <= 0.40 ? 0.12 + 0.30 * height : 0.16 + 0.30 * height;
    case LedgeMove::Mantle: return (0.16 + 0.30 * height) * (1.0 + 0.5 * (1.0 - std::min(approachSpeed / 3.5, 1.0)));
    case LedgeMove::ClimbUp: return 0.55 + 0.45 * height;
    case LedgeMove::None: break;
    }
    return 0.0;
}

} // namespace

const char* LedgeMoveName(LedgeMove move) noexcept {
    switch (move) {
    case LedgeMove::None: return "None";
    case LedgeMove::StepOn: return "StepOn";
    case LedgeMove::Vault: return "Vault";
    case LedgeMove::Mantle: return "Mantle";
    case LedgeMove::ClimbUp: return "ClimbUp";
    }
    return "Unknown";
}

std::optional<std::string> Validate(const LedgeAssistConfig& c) {
    for (const double v : {c.stepHeight, c.lowMax, c.midMax, c.highMax, c.reachStanding, c.reachMax, c.minGripDepth, c.maxApproachAngle,
                           c.maxTopAngle, c.vaultMaxDepth, c.maxVaultDrop, c.legAllowance, c.clearance, c.minForwardSpeed, c.minOverSpeed}) {
        if (!Finite(v) || v <= 0.0) return "every ledge assist value must be finite and positive";
    }
    for (const double v : {c.reachPerSpeed, c.vaultMinSpeed, c.maxVaultRise, c.overEndMargin}) {
        if (!Finite(v) || v < 0.0) return "ledge assist value must be finite and not negative";
    }
    if (!(c.stepHeight < c.lowMax && c.lowMax < c.midMax && c.midMax < c.highMax)) return "bands must ascend: step < low < mid < high";
    if (c.reachStanding > c.reachMax) return "standing reach above the maximum reach";
    if (c.maxApproachAngle >= 90.0 || c.maxTopAngle >= 90.0) return "angles must stay below 90 degrees";
    return std::nullopt;
}

double LedgeReach(const LedgeAssistConfig& config, double approachSpeed) noexcept {
    const double speed = Finite(approachSpeed) ? std::max(approachSpeed, 0.0) : 0.0;
    return std::min(config.reachStanding + config.reachPerSpeed * speed, config.reachMax);
}

double LedgeSpeedRetention(const LedgeAssistConfig& config, LedgeMove move, double height) noexcept {
    const double span = std::max(config.midMax - config.lowMax, 1e-6);
    const double s = std::clamp((height - config.lowMax) / span, 0.0, 1.0);
    switch (move) {
    case LedgeMove::StepOn: return 0.90;
    case LedgeMove::Vault: return height <= config.lowMax ? 0.90 : 0.85 - 0.25 * s;
    case LedgeMove::Mantle: return 0.80 - 0.50 * s;
    case LedgeMove::ClimbUp:
    case LedgeMove::None: break;
    }
    return 0.0;
}

double LedgeRequiredAxisDistance(const LedgePlan& plan, double up) noexcept {
    const double upperBodyBottom = up + plan.legAllowance;
    if (upperBodyBottom >= plan.height) return -1.0e9; // above the top: nothing to keep clear of
    const double k = upperBodyBottom + plan.radius - plan.height; // hemisphere centre above the edge
    if (k <= 0.0) return plan.radius;
    return std::sqrt(std::max(plan.radius * plan.radius - k * k, 0.0));
}

LedgePlan PlanLedgeAssist(const LedgeAssistConfig& config, const LedgeProbe& p, double capsuleRadius) noexcept {
    for (const double v : {p.faceDistance, p.approachAngle, p.approachSpeed, p.height, p.topAngle, p.depth, p.landingRise, capsuleRadius}) {
        if (!Finite(v)) return Refuse("invalid measurement");
    }
    if (capsuleRadius <= 0.0) return Refuse("invalid capsule");
    if (!p.faceFound) return Refuse("nothing in front");
    const double speed = std::max(p.approachSpeed, 0.0);
    if (p.faceDistance < -0.02 || p.faceDistance > LedgeReach(config, speed)) return Refuse("obstacle not within reach: ordinary jump");
    if (std::abs(p.approachAngle) > config.maxApproachAngle) return Refuse("approach too oblique");
    if (!p.surfaceAllowed) return Refuse("not a surface to climb (pawn or moving object)");
    if (!p.topFound) return Refuse("no reachable top (higher than the highest band, or an overhang)");
    const double h = p.height;
    if (h <= config.stepHeight) return Refuse("low enough to walk up", h);
    if (h > config.highMax) return Refuse("higher than the highest band", h);
    if (p.topAngle > config.maxTopAngle) return Refuse("top not walkable", h);
    if (p.depth < config.minGripDepth) return Refuse("edge too narrow to grip", h);
    if (!p.liftClear) return Refuse("no room to rise (ceiling)", h);

    const bool canStand = p.topClear && p.depth >= capsuleRadius + config.overEndMargin;
    const bool thin = p.depth <= config.vaultMaxDepth;
    const bool landingOk = p.landingFound && p.landingClear && p.landingRise >= -config.maxVaultDrop && p.landingRise <= config.maxVaultRise;

    LedgeMove move = LedgeMove::None;
    const char* reason = "";
    if (h <= config.lowMax) {
        if (canStand) { move = LedgeMove::StepOn; reason = "low band: step onto it"; }
        else if (thin && landingOk) { move = LedgeMove::Vault; reason = "low band: step over the thin obstacle"; }
        else return Refuse("low band: no room on top and no free landing behind", h);
    } else if (h <= config.midMax) {
        if (thin && landingOk && speed >= config.vaultMinSpeed) { move = LedgeMove::Vault; reason = "middle band: thin, run-up, free landing"; }
        else if (canStand) { move = LedgeMove::Mantle; reason = "middle band: onto the ledge"; }
        else if (thin && landingOk) return Refuse("middle band: a vault needs a run-up and the top is too narrow to stand on", h);
        else return Refuse("middle band: no room on top and no free landing behind", h);
    } else {
        if (canStand) { move = LedgeMove::ClimbUp; reason = "high band: climb up"; }
        else return Refuse("high band: no room to stand on top", h);
    }

    LedgePlan plan;
    plan.move = move;
    plan.reason = reason;
    plan.height = h;
    plan.depth = p.depth;
    plan.radius = capsuleRadius;
    plan.faceAxisDistance = std::max(p.faceDistance, 0.0) + capsuleRadius;
    plan.approachSpeed = speed;
    plan.legAllowance = config.legAllowance;
    plan.riseHeight = h + config.clearance;
    plan.endsOnTop = move != LedgeMove::Vault;
    const double keep = LedgeSpeedRetention(config, move, h);
    plan.exitSpeed = keep * speed;
    plan.liftForwardSpeed = std::max(keep * speed, config.minForwardSpeed);
    plan.overSpeed = std::max(keep * speed, config.minOverSpeed);
    plan.liftSeconds = LiftSeconds(move, h, speed);
    plan.forwardEnd = plan.endsOnTop ? plan.faceAxisDistance + capsuleRadius + config.overEndMargin
                                     : plan.faceAxisDistance + p.depth + capsuleRadius + config.overEndMargin;
    const double atTop = LiftForward(plan, plan.liftSeconds);
    plan.forwardEnd = std::max(plan.forwardEnd, atTop);
    plan.overSeconds = (plan.forwardEnd - atTop) / plan.overSpeed;
    return plan;
}

LedgePathPoint SampleLedgePath(const LedgePlan& plan, double seconds) noexcept {
    LedgePathPoint point;
    if (plan.move == LedgeMove::None) {
        point.done = true;
        return point;
    }
    const double t = Finite(seconds) ? std::max(seconds, 0.0) : 0.0;
    if (t < plan.liftSeconds || (t == plan.liftSeconds && plan.overSeconds > 0.0)) {
        point.up = LiftUp(plan, t);
        point.forward = LiftForward(plan, t);
        return point;
    }
    const double atTop = LiftForward(plan, plan.liftSeconds);
    point.up = plan.riseHeight;
    point.forward = std::min(atTop + plan.overSpeed * (t - plan.liftSeconds), plan.forwardEnd);
    point.done = t >= plan.TotalSeconds();
    if (point.done) point.forward = plan.forwardEnd;
    return point;
}

} // namespace spn::core
