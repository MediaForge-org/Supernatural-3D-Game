#include <spn/core/JumpModel.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {
bool Positive(double v) noexcept { return std::isfinite(v) && v > 0.0; }
} // namespace

double JumpHeightAt(double v0, double g, double t) noexcept {
    if (!std::isfinite(v0) || !std::isfinite(g) || !std::isfinite(t)) return 0.0;
    return v0 * t - 0.5 * g * t * t;
}

JumpWindow ComputeJumpWindow(double v0, double g, double h, double speed) noexcept {
    JumpWindow w;
    if (!Positive(v0) || !Positive(g) || !Positive(speed) || !std::isfinite(h)) return w;
    if (h <= 0.0) return w;
    const double disc = v0 * v0 - 2.0 * g * h;
    if (disc <= 1e-9) return w;  // apex at (or within rounding of) the ledge height: the feet only touch it for an instant
    const double root = std::sqrt(disc);
    w.reachable = true;
    w.riseTime = (v0 - root) / g;
    w.fallTime = (v0 + root) / g;
    w.minDistance = speed * w.riseTime;
    w.maxDistance = speed * w.fallTime;
    return w;
}

const char* JumpOutcomeName(JumpOutcome o) noexcept {
    switch (o) {
    case JumpOutcome::Clears: return "Clears";
    case JumpOutcome::BlockedRising: return "BlockedRising";
    case JumpOutcome::FallsShort: return "FallsShort";
    case JumpOutcome::TooHigh: return "TooHigh";
    }
    return "Unknown";
}

JumpOutcome EvaluateJump(const JumpAttempt& a) noexcept {
    const JumpWindow w = ComputeJumpWindow(a.launchVelocity, a.gravity, a.ledgeHeight, a.speed);
    if (!w.reachable) return JumpOutcome::TooHigh;
    if (!std::isfinite(a.distanceToFace) || a.distanceToFace < w.minDistance) return JumpOutcome::BlockedRising;
    if (a.distanceToFace > w.maxDistance) return JumpOutcome::FallsShort;
    return JumpOutcome::Clears;
}

double VerticalClimbSeconds(const JumpAttempt& a) noexcept {
    if (EvaluateJump(a) != JumpOutcome::BlockedRising) return 0.0;
    const JumpWindow w = ComputeJumpWindow(a.launchVelocity, a.gravity, a.ledgeHeight, a.speed);
    const double tFace = std::isfinite(a.distanceToFace) && a.distanceToFace > 0.0 ? a.distanceToFace / a.speed : 0.0;
    return w.riseTime - tFace;
}

FaceJumpResult SimulateFaceJump(const FaceJumpParams& p) noexcept {
    FaceJumpResult r;
    const JumpAttempt& a = p.attempt;
    r.outcome = EvaluateJump(a);
    const JumpWindow w = ComputeJumpWindow(a.launchVelocity, a.gravity, a.ledgeHeight, a.speed);
    if (!w.reachable) return r;
    r.timeFeetAboveLedge = w.riseTime;
    r.airTimeLeft = w.fallTime - w.riseTime;
    if (r.outcome != JumpOutcome::BlockedRising) {
        r.onLedge = r.outcome == JumpOutcome::Clears;
        return r;
    }
    if (!Positive(p.airAcceleration) && !p.keepSpeed) return r;
    r.travelNeeded = std::max(p.overhang, 0.0);
    if (p.keepSpeed) {
        // Ledge assist: the speed survives the contact and is travelled in the remaining air time at the take-off speed.
        r.travelAchieved = a.speed * r.airTimeLeft;
    } else {
        // Stopped at the face, then accelerated by the air control for the time the feet stay above the ledge top.
        r.travelAchieved = 0.5 * p.airAcceleration * r.airTimeLeft * r.airTimeLeft;
    }
    r.onLedge = r.travelAchieved >= r.travelNeeded;
    return r;
}

} // namespace spn::core
