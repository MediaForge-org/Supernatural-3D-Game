#include <spn/core/CameraArm.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

std::optional<std::string> Validate(const CameraArmConfig& c) {
    for (const double v : {c.baseLength, c.sprintExtra, c.airborneExtra, c.minLength, c.collisionMargin, c.recoverSpeed, c.recoverDelay, c.blendSpeed})
        if (!std::isfinite(v)) return "camera values must be finite";
    if (c.minLength <= 0.0) return "min length must be positive";
    if (c.baseLength < c.minLength) return "base length below the minimum";
    if (c.sprintExtra < 0.0 || c.airborneExtra < 0.0 || c.collisionMargin < 0.0 || c.recoverDelay < 0.0) return "extras, margin and delay must not be negative";
    if (c.recoverSpeed <= 0.0 || c.blendSpeed <= 0.0) return "speeds must be positive";
    return std::nullopt;
}

double DesiredArmLength(const CameraArmConfig& c, const CameraArmContext& ctx, std::optional<double> hit) noexcept {
    double length = c.baseLength + (ctx.sprinting ? c.sprintExtra : 0.0) + (ctx.airborne ? c.airborneExtra : 0.0);
    if (hit && std::isfinite(*hit)) length = std::min(length, std::max(*hit, 0.0) - c.collisionMargin);
    return std::max(length, c.minLength);
}

CameraArmState StepCameraArm(const CameraArmConfig& c, CameraArmState s, double target, double dt) noexcept {
    if (!std::isfinite(dt) || dt <= 0.0 || !std::isfinite(target)) return s;
    if (target <= s.length) {
        s.length = target;       // pull in at once: never look through the wall
        s.growTimer = 0.0;
        return s;
    }
    s.growTimer += dt;
    if (s.growTimer >= c.recoverDelay) s.length = std::min(s.length + c.recoverSpeed * dt, target);
    return s;
}

} // namespace spn::core
