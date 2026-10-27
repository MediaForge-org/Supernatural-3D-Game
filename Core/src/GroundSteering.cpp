#include <spn/core/GroundSteering.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(double v) noexcept { return std::isfinite(v); }

} // namespace

double Speed(Vec2 v) noexcept { return std::hypot(v.x, v.y); }

std::optional<std::string> Validate(const SteeringConfig& c) {
    if (!Finite(c.maxHeadingRateDegrees) || !Finite(c.minHeadingRateDegrees) || !Finite(c.fullSpeed) || !Finite(c.speedLossPerRadian))
        return "steering values must be finite";
    if (c.minHeadingRateDegrees <= 0.0 || c.minHeadingRateDegrees > c.maxHeadingRateDegrees) return "heading rates: 0 < min <= max";
    if (c.maxHeadingRateDegrees > 3600.0) return "heading rate above 3600 deg/s";
    if (c.fullSpeed <= 0.0) return "full speed must be positive";
    if (c.speedLossPerRadian < 0.0 || c.speedLossPerRadian > 0.5) return "speed loss per radian outside 0..0.5";
    return std::nullopt;
}

Vec2 SteerVelocity(const SteeringConfig& config, Vec2 velocity, const SteeringInput& input, double dt) noexcept {
    if (!Finite(dt) || dt <= 0.0 || !Finite(velocity.x) || !Finite(velocity.y)) return velocity;
    double speed = Speed(velocity);
    const double length = Finite(input.direction.x) && Finite(input.direction.y) ? Speed(input.direction) : 0.0;
    const double braking = Finite(input.braking) ? std::max(input.braking, 0.0) : 0.0;
    const double accel = Finite(input.acceleration) ? std::max(input.acceleration, 0.0) : 0.0;
    const double target = Finite(input.targetSpeed) ? std::max(input.targetSpeed, 0.0) : 0.0;

    if (length < 1e-9) {  // no input: brake along the current heading
        if (speed < 1e-9) return {0.0, 0.0};
        const double slowed = std::max(speed - braking * dt, 0.0);
        return {velocity.x / speed * slowed, velocity.y / speed * slowed};
    }
    const double dirX = input.direction.x / length;
    const double dirY = input.direction.y / length;
    double heading = speed > 1e-9 ? std::atan2(velocity.y, velocity.x) : std::atan2(dirY, dirX);
    const double wanted = std::atan2(dirY, dirX);

    if (speed > 1e-9) {
        double error = wanted - heading;
        while (error > kPi) error -= 2.0 * kPi;
        while (error < -kPi) error += 2.0 * kPi;
        const double t = std::min(speed / config.fullSpeed, 1.0);
        const double rate = (config.maxHeadingRateDegrees + (config.minHeadingRateDegrees - config.maxHeadingRateDegrees) * t) * kPi / 180.0;
        const double turn = std::copysign(std::min(std::abs(error), rate * dt), error);
        heading += turn;
        speed *= std::max(1.0 - config.speedLossPerRadian * std::abs(turn), 0.0);
    }
    if (speed < target) speed = std::min(speed + accel * dt, target);
    else if (speed > target) speed = std::max(speed - braking * dt, target);
    return {std::cos(heading) * speed, std::sin(heading) * speed};
}

} // namespace spn::core
