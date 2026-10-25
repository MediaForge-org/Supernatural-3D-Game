#include <spn/core/Movement.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(double value) noexcept {
    return std::isfinite(value);
}

} // namespace

const char* GaitName(Gait gait) noexcept {
    switch (gait) {
    case Gait::Walk: return "Walk";
    case Gait::Jog: return "Jog";
    case Gait::Run: return "Run";
    case Gait::Sprint: return "Sprint";
    }
    return "Unknown";
}

std::optional<Gait> GaitFromIndex(unsigned index) noexcept {
    if (index >= kGaitCount) {
        return std::nullopt;
    }
    return static_cast<Gait>(index);
}

std::optional<std::string> Validate(const MovementConfig& config) {
    for (std::size_t i = 0; i < kGaitCount; ++i) {
        const GaitSettings& g = config.gaits[i];
        if (!Finite(g.speed) || g.speed <= 0.0) {
            return std::string("gait speed must be positive: ") + GaitName(static_cast<Gait>(i));
        }
        if (!Finite(g.acceleration) || g.acceleration <= 0.0) {
            return std::string("gait acceleration must be positive: ") + GaitName(static_cast<Gait>(i));
        }
        if (i > 0 && g.speed <= config.gaits[i - 1].speed) {
            return std::string("gait speeds must be strictly ascending at ") + GaitName(static_cast<Gait>(i));
        }
    }
    if (static_cast<std::size_t>(config.defaultGait) >= kGaitCount) {
        return std::string("default gait out of range");
    }
    if (config.defaultGait == Gait::Sprint) {
        return std::string("default gait must not be Sprint (sprint is a temporary request)");
    }
    if (!Finite(config.brakingDeceleration) || config.brakingDeceleration <= 0.0) {
        return std::string("braking deceleration must be positive");
    }
    if (!Finite(config.backwardSpeedFactor) || config.backwardSpeedFactor <= 0.0 || config.backwardSpeedFactor > 1.0) {
        return std::string("backward speed factor must be in (0, 1]");
    }
    if (!Finite(config.strafeSpeedFactor) || config.strafeSpeedFactor <= 0.0 || config.strafeSpeedFactor > 1.0) {
        return std::string("strafe speed factor must be in (0, 1]");
    }
    if (!Finite(config.sprintMaxAngleDegrees) || config.sprintMaxAngleDegrees < 0.0 || config.sprintMaxAngleDegrees > 180.0) {
        return std::string("sprint angle must be in [0, 180] degrees");
    }
    if (!Finite(config.jumpHeight) || config.jumpHeight <= 0.0 || config.jumpHeight > 2.0) {
        return std::string("jump height must be in (0, 2] m");
    }
    if (!Finite(config.maxStepHeight) || config.maxStepHeight < 0.0 || config.maxStepHeight > 1.0) {
        return std::string("max step height must be in [0, 1] m");
    }
    return std::nullopt;
}

double GaitSpeed(const MovementConfig& config, Gait gait) noexcept {
    return config.gaits[static_cast<std::size_t>(gait)].speed;
}

double GaitAcceleration(const MovementConfig& config, Gait gait) noexcept {
    return config.gaits[static_cast<std::size_t>(gait)].acceleration;
}

Gait ResolveGait(const MovementConfig& config, Gait base, bool sprintRequested, const GaitLimits& limits,
                 MoveIntent intent) noexcept {
    const Gait walking = std::min(base == Gait::Sprint ? Gait::Run : base, limits.maxGait);
    if (!sprintRequested || limits.maxGait != Gait::Sprint) {
        return walking;
    }

    const double magnitude = std::hypot(intent.forward, intent.right);
    if (magnitude > 1e-6 && intent.forward > 0.0) {
        // Angle between the input direction and straight ahead.
        const double angle = std::atan2(std::abs(intent.right), intent.forward) * 180.0 / kPi;
        if (angle <= config.sprintMaxAngleDegrees) {
            return Gait::Sprint;
        }
    }
    return walking;
}

double DirectionSpeedScale(const MovementConfig& config, MoveIntent intent) noexcept {
    const double magnitude = std::hypot(intent.forward, intent.right);
    if (magnitude <= 1e-6) {
        return 1.0;
    }
    const double fwd = intent.forward / magnitude;
    const double side = std::abs(intent.right) / magnitude;
    // Forward half: blend from 1 (straight ahead) to the strafe factor (sideways) by the side share.
    // Backward half: blend from the strafe factor (sideways) to the backward factor (straight back).
    if (fwd >= 0.0) {
        return 1.0 + (config.strafeSpeedFactor - 1.0) * side;
    }
    const double back = -fwd;
    return config.strafeSpeedFactor + (config.backwardSpeedFactor - config.strafeSpeedFactor) * back;
}

double EffectiveSpeed(const MovementConfig& config, Gait gait, const GaitLimits& limits) noexcept {
    const Gait capped = std::min(gait, limits.maxGait);
    return GaitSpeed(config, capped) * limits.speedScale;
}

double JumpVelocityForHeight(double jumpHeight, double gravity) noexcept {
    if (!(jumpHeight > 0.0) || !(gravity > 0.0) || !Finite(jumpHeight) || !Finite(gravity)) {
        return 0.0;
    }
    return std::sqrt(2.0 * gravity * jumpHeight);
}

double JumpApexHeight(double launchVelocity, double gravity) noexcept {
    if (!(gravity > 0.0) || !Finite(gravity) || !Finite(launchVelocity) || launchVelocity <= 0.0) {
        return 0.0;
    }
    return launchVelocity * launchVelocity / (2.0 * gravity);
}

const char* ObstacleClassName(ObstacleClass c) noexcept {
    switch (c) {
    case ObstacleClass::StepUp: return "StepUp";
    case ObstacleClass::Jump: return "Jump";
    case ObstacleClass::Blocked: return "Blocked";
    }
    return "Unknown";
}

ObstacleClass ClassifyObstacle(const MovementConfig& config, double height) noexcept {
    const double h = std::max(height, 0.0);
    if (h <= config.maxStepHeight) {
        return ObstacleClass::StepUp;
    }
    if (h <= config.jumpHeight) {
        return ObstacleClass::Jump;
    }
    return ObstacleClass::Blocked;
}

} // namespace spn::core
