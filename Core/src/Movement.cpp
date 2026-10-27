#include <spn/core/Movement.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(double value) noexcept {
    return std::isfinite(value);
}

// An out-of-range enum (e.g. a corrupt wire value cast to Gait) must never index past the array.
std::size_t GaitIndex(Gait gait) noexcept {
    const auto index = static_cast<std::size_t>(gait);
    return index < kGaitCount ? index : kGaitCount - 1;
}

Gait SaneGait(Gait gait) noexcept {
    return static_cast<std::size_t>(gait) < kGaitCount ? gait : Gait::Sprint;
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
    if (!Finite(config.sprintSustainAngleDegrees) || config.sprintSustainAngleDegrees < config.sprintMaxAngleDegrees ||
        config.sprintSustainAngleDegrees > 180.0) {
        return std::string("sprint sustain angle must be in [sprint angle, 180] degrees");
    }
    if (!Finite(config.turnRateDegrees) || config.turnRateDegrees <= 0.0 || config.turnRateDegrees > 3600.0) {
        return std::string("turn rate must be in (0, 3600] degrees per second");
    }
    if (!Finite(config.minAnalogSpeed) || config.minAnalogSpeed < 0.0 || config.minAnalogSpeed > config.gaits[0].speed) {
        return std::string("minimum analog speed must be in [0, walk speed]");
    }
    if (!Finite(config.idleSpeed) || config.idleSpeed < 0.0 || config.idleSpeed >= config.gaits[0].speed) {
        return std::string("idle speed must be in [0, walk speed)");
    }
    if (!Finite(config.landingSeconds) || config.landingSeconds < 0.0 || config.landingSeconds > 2.0) {
        return std::string("landing time must be in [0, 2] seconds");
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
    return config.gaits[GaitIndex(gait)].speed;
}

double GaitAcceleration(const MovementConfig& config, Gait gait) noexcept {
    return config.gaits[GaitIndex(gait)].acceleration;
}

Gait ObservedGait(const MovementConfig& config, double horizontalSpeed) noexcept {
    const double speed = (Finite(horizontalSpeed) && horizontalSpeed > 0.0) ? horizontalSpeed : 0.0;
    Gait best = Gait::Walk;
    double bestDelta = std::abs(config.gaits[0].speed - speed);
    for (std::size_t i = 1; i < kGaitCount; ++i) {
        const double delta = std::abs(config.gaits[i].speed - speed);
        if (delta < bestDelta) { // strict: an exact tie keeps the lower gait
            bestDelta = delta;
            best = static_cast<Gait>(i);
        }
    }
    return best;
}

const char* MovementStateName(MovementState state) noexcept {
    switch (state) {
    case MovementState::Idle: return "Idle";
    case MovementState::Walk: return "Walk";
    case MovementState::Jog: return "Jog";
    case MovementState::Run: return "Run";
    case MovementState::Sprint: return "Sprint";
    case MovementState::Jumping: return "Jumping";
    case MovementState::Falling: return "Falling";
    case MovementState::Landing: return "Landing";
    }
    return "Unknown";
}

MovementState ClassifyMotion(const MovementConfig& config, const MotionSample& sample) noexcept {
    if (!sample.onGround) {
        return sample.verticalVelocity > 0.0 ? MovementState::Jumping : MovementState::Falling;
    }
    if (Finite(sample.secondsSinceLanding) && sample.secondsSinceLanding >= 0.0 &&
        sample.secondsSinceLanding < config.landingSeconds) {
        return MovementState::Landing;
    }
    if (!(sample.horizontalSpeed >= config.idleSpeed)) { // also true for NaN
        return MovementState::Idle;
    }
    switch (ObservedGait(config, sample.horizontalSpeed)) {
    case Gait::Walk: return MovementState::Walk;
    case Gait::Jog: return MovementState::Jog;
    case Gait::Run: return MovementState::Run;
    case Gait::Sprint: return MovementState::Sprint;
    }
    return MovementState::Idle;
}

Gait ResolveGait(const MovementConfig& config, Gait base, bool sprintRequested, const GaitLimits& limits,
                 MoveIntent intent, double currentSpeed) noexcept {
    // Corrupt enum values (a bad wire value cast to Gait) are treated like Sprint, as GaitSpeed does: a base of Sprint counts as Run
    // and a ceiling of Sprint restricts nothing. The result is therefore always a valid gait.
    const Gait baseGait = SaneGait(base);
    const Gait ceiling = SaneGait(limits.maxGait);
    const Gait walking = std::min(baseGait == Gait::Sprint ? Gait::Run : baseGait, ceiling);
    if (!sprintRequested || ceiling != Gait::Sprint) {
        return walking;
    }

    const double magnitude = std::hypot(intent.forward, intent.right);
    if (magnitude > 1e-6) {
        // Angle between the input direction and straight ahead.
        const double angle = std::atan2(std::abs(intent.right), intent.forward) * 180.0 / kPi;
        const bool alreadySprinting = ObservedGait(config, currentSpeed) == Gait::Sprint;
        const double allowed = alreadySprinting ? config.sprintSustainAngleDegrees : config.sprintMaxAngleDegrees;
        if (angle <= allowed) {
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
    const Gait capped = std::min(SaneGait(gait), SaneGait(limits.maxGait));
    // The documented range of the scale is (0, 1]; NaN counts as "no restriction", values outside are clamped.
    const double scale = std::isnan(limits.speedScale) ? 1.0 : std::clamp(limits.speedScale, 0.0, 1.0);
    return GaitSpeed(config, capped) * scale;
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
    case ObstacleClass::JumpCandidate: return "JumpCandidate";
    case ObstacleClass::Blocked: return "Blocked";
    }
    return "Unknown";
}

ObstacleClass ClassifyObstacle(const MovementConfig& config, double height) noexcept {
    if (!Finite(height)) {
        return ObstacleClass::Blocked; // NaN and +-inf: never claim an obstacle can be passed
    }
    const double h = std::max(height, 0.0);
    if (h <= config.maxStepHeight) {
        return ObstacleClass::StepUp;
    }
    // Strictly below the apex: the apex is reached for an instant only.
    if (h < config.jumpHeight) {
        return ObstacleClass::JumpCandidate;
    }
    return ObstacleClass::Blocked;
}

} // namespace spn::core
