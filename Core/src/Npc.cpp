#include <spn/core/Npc.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(NpcPoint p) noexcept { return std::isfinite(p.x) && std::isfinite(p.y); }

} // namespace

const char* NpcStateName(NpcState s) noexcept {
    switch (s) {
    case NpcState::Idle: return "Idle";
    case NpcState::Moving: return "Moving";
    case NpcState::Interacting: return "Interacting";
    case NpcState::Alert: return "Alert";
    case NpcState::Incapacitated: return "Incapacitated";
    }
    return "Unknown";
}

NpcState NextNpcState(NpcState current, const NpcEvents& e) noexcept {
    if (current == NpcState::Incapacitated) return e.recovered && !e.incapacitated ? NpcState::Idle : NpcState::Incapacitated;
    if (e.incapacitated) return NpcState::Incapacitated;
    if (current == NpcState::Alert) {
        if (e.threatPerceived || !e.threatGone) return NpcState::Alert;
        return e.hasGoal && !e.goalReached ? NpcState::Moving : NpcState::Idle;
    }
    if (e.threatPerceived) return NpcState::Alert;
    if (current == NpcState::Interacting) return e.interactionFinished ? NpcState::Idle : NpcState::Interacting;
    if (e.interactionRequested) return NpcState::Interacting;
    if (e.hasGoal && !e.goalReached) return NpcState::Moving;
    return NpcState::Idle;
}

bool IsLegalNpcTransition(NpcState from, NpcState to) noexcept {
    if (from == to) return true;
    switch (from) {
    case NpcState::Idle: return to != NpcState::Idle;                       // any other state
    case NpcState::Moving: return true;                                      // a moving NPC may stop to interact, get alert, fall
    case NpcState::Interacting: return to == NpcState::Idle || to == NpcState::Alert || to == NpcState::Incapacitated;
    case NpcState::Alert: return to == NpcState::Idle || to == NpcState::Moving || to == NpcState::Incapacitated;
    case NpcState::Incapacitated: return to == NpcState::Idle;
    }
    return false;
}

std::optional<std::string> Validate(const NpcMoveConfig& c) {
    if (!std::isfinite(c.arrivalRadius) || !std::isfinite(c.slowDownRadius) || !std::isfinite(c.minSpeedFraction)) return "movement values must be finite";
    if (c.arrivalRadius <= 0.0) return "arrival radius must be positive";
    if (c.slowDownRadius <= c.arrivalRadius) return "slow-down radius must exceed the arrival radius";
    if (c.minSpeedFraction <= 0.0 || c.minSpeedFraction > 1.0) return "minimum speed fraction outside (0, 1]";
    return std::nullopt;
}

NpcMoveIntent ComputeMoveIntent(const MovementConfig& movement, const NpcMoveConfig& config, NpcPoint position, NpcPoint goal, Gait gait) noexcept {
    NpcMoveIntent out;
    if (!Finite(position) || !Finite(goal)) return out;
    const double dx = goal.x - position.x, dy = goal.y - position.y;
    const double distance = std::hypot(dx, dy);
    if (distance <= config.arrivalRadius) {
        out.arrived = true;
        return out;
    }
    out.direction = {dx / distance, dy / distance};
    const double speed = GaitSpeed(movement, gait);
    if (distance >= config.slowDownRadius) {
        out.targetSpeed = speed;
    } else {
        const double t = (distance - config.arrivalRadius) / (config.slowDownRadius - config.arrivalRadius);
        out.targetSpeed = speed * (config.minSpeedFraction + (1.0 - config.minSpeedFraction) * t);
    }
    return out;
}

Perception Perceive(const NpcPerceptionConfig& config, NpcPoint position, double facingDegrees, const NpcStimulus& s) noexcept {
    if (!Finite(position) || !Finite(s.position) || !std::isfinite(facingDegrees) || !std::isfinite(s.loudness)) return Perception::None;
    const double dx = s.position.x - position.x, dy = s.position.y - position.y;
    const double distance = std::hypot(dx, dy);
    if (distance <= config.closeSenseRange) return Perception::Seen;
    if (distance <= config.sightRange && s.lineOfSightClear) {
        double diff = std::atan2(dy, dx) * 180.0 / kPi - facingDegrees;
        diff = std::fmod(diff + 180.0, 360.0);
        if (diff < 0.0) diff += 360.0;
        diff -= 180.0;
        if (std::abs(diff) <= config.fieldOfViewDegrees / 2.0) return Perception::Seen;
    }
    if (s.loudness > 0.0 && distance <= config.hearingRange * s.loudness) return Perception::Heard;
    return Perception::None;
}

} // namespace spn::core
