#include <spn/core/Traversal.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {

bool Finite(double v) noexcept { return std::isfinite(v); }

double Positive(double v) noexcept { return Finite(v) && v > 0.0 ? v : 0.0; }

double PhaseLength(const TraversalTimings& t, TraversalPhase phase) noexcept {
    switch (phase) {
    case TraversalPhase::Approach: return t.approach;
    case TraversalPhase::Active: return t.active;
    case TraversalPhase::Recover: return t.recover;
    default: return 0.0;
    }
}

} // namespace

const char* TraversalMoveName(TraversalMove move) noexcept {
    switch (move) {
    case TraversalMove::None: return "None";
    case TraversalMove::StepUp: return "StepUp";
    case TraversalMove::Jump: return "Jump";
    case TraversalMove::Vault: return "Vault";
    case TraversalMove::Mantle: return "Mantle";
    case TraversalMove::Climb: return "Climb";
    case TraversalMove::Blocked: return "Blocked";
    }
    return "Unknown";
}

std::optional<std::string> Validate(const TraversalConfig& c) {
    for (const double v : {c.vaultMaxHeight, c.vaultMaxDepth, c.vaultMinSpeed, c.mantleMaxHeight, c.mantleMinLedgeDepth,
                           c.standingHeadroom, c.climbMaxHeight, c.climbMaxApproachSpeed}) {
        if (!Finite(v) || v <= 0.0) return "every traversal value must be finite and positive";
    }
    if (c.vaultMaxHeight >= c.mantleMaxHeight) return "vault height must stay below the mantle height";
    if (c.mantleMaxHeight >= c.climbMaxHeight) return "mantle height must stay below the climb height";
    if (c.standingHeadroom < 1.0 || c.standingHeadroom > 3.0) return "standing headroom outside 1..3 m";
    return std::nullopt;
}

TraversalDecision ClassifyTraversal(const MovementConfig& movement, const TraversalConfig& config, const TraversalObstacle& o) noexcept {
    if (std::isnan(o.height) || std::isnan(o.depth) || std::isnan(o.headroom)) return {TraversalMove::Blocked, "invalid measurement"};
    const double height = std::max(o.height, 0.0);
    if (height <= 0.0) return {TraversalMove::None, "nothing in the way"};
    switch (ClassifyObstacle(movement, height)) {
    case ObstacleClass::StepUp: return {TraversalMove::StepUp, "within the step height"};
    case ObstacleClass::JumpCandidate: return {TraversalMove::Jump, "below the jump apex"};
    case ObstacleClass::Blocked: break;
    }
    const double depth = Positive(o.depth);
    const double speed = Positive(o.approachSpeed);
    if (height <= config.vaultMaxHeight && depth <= config.vaultMaxDepth && speed >= config.vaultMinSpeed)
        return {TraversalMove::Vault, "thin and low enough, with a run-up"};
    if (height <= config.mantleMaxHeight && depth >= config.mantleMinLedgeDepth && o.headroom >= config.standingHeadroom)
        return {TraversalMove::Mantle, "a ledge to stand on"};
    if (o.climbable && height <= config.climbMaxHeight && speed <= config.climbMaxApproachSpeed && o.headroom >= config.standingHeadroom)
        return {TraversalMove::Climb, "climbable surface"};
    if (height <= config.mantleMaxHeight && depth < config.mantleMinLedgeDepth) return {TraversalMove::Blocked, "ledge too narrow and no vault run-up"};
    if (o.headroom < config.standingHeadroom) return {TraversalMove::Blocked, "no head room above"};
    return {TraversalMove::Blocked, "too high"};
}

bool CanStartTraversal(const TraversalState& state, TraversalMove move, MovementState current) noexcept {
    if (move != TraversalMove::Vault && move != TraversalMove::Mantle && move != TraversalMove::Climb) return false;
    if (state.phase != TraversalPhase::Idle && state.phase != TraversalPhase::Done) return false;
    if (state.cooldown > 0.0) return false;
    return current != MovementState::Falling && current != MovementState::Landing;
}

TraversalState StartTraversal(TraversalMove move) noexcept {
    return TraversalState{move, TraversalPhase::Approach, 0.0};
}

TraversalState AdvanceTraversal(const TraversalState& state, const TraversalTimings& timings, double dt) noexcept {
    if (state.phase == TraversalPhase::Idle || state.phase == TraversalPhase::Done) {
        TraversalState cooled = state;
        cooled.cooldown = std::max(state.cooldown - Positive(dt), 0.0);
        return cooled;
    }
    TraversalState next = state;
    double left = Positive(dt);
    while (left > 0.0 && next.phase != TraversalPhase::Done) {
        const double length = PhaseLength(timings, next.phase);
        const double room = length - next.phaseSeconds;
        if (left < room) {
            next.phaseSeconds += left;
            return next;
        }
        left -= std::max(room, 0.0);
        next.phaseSeconds = 0.0;
        next.phase = static_cast<TraversalPhase>(static_cast<std::uint8_t>(next.phase) + 1);
        if (next.phase == TraversalPhase::Done) {
            if (next.result == TraversalResult::None) next.result = TraversalResult::Success;
            next.cooldown = timings.cooldown;
            next.cooldown = std::max(next.cooldown - left, 0.0);
            left = 0.0;
        }
    }
    return next;
}

TraversalState AbortTraversal(const TraversalState& state, const TraversalTimings& timings) noexcept {
    if (state.phase != TraversalPhase::Approach) return state;
    TraversalState next = state;
    next.phase = TraversalPhase::Done;
    next.phaseSeconds = 0.0;
    next.result = TraversalResult::Aborted;
    next.cooldown = timings.cooldown;
    return next;
}

TraversalState FailTraversal(const TraversalState& state) noexcept {
    if (state.phase != TraversalPhase::Active) return state;
    TraversalState next = state;
    next.phase = TraversalPhase::Recover;
    next.phaseSeconds = 0.0;
    next.result = TraversalResult::Failed;
    return next;
}

MovementState ExitMovementState(const TraversalState& state, bool groundBelow) noexcept {
    if (state.phase != TraversalPhase::Done) return MovementState::Falling;
    if (!groundBelow) return MovementState::Falling;
    if (state.result == TraversalResult::Success && (state.move == TraversalMove::Mantle || state.move == TraversalMove::Climb)) return MovementState::Idle;
    return MovementState::Landing;
}

TraversalDecision ClassifyTraversalChecked(const MovementConfig& movement, const TraversalConfig& config, const TraversalSafety& safety,
                                           const TraversalProbe& probe) noexcept {
    const TraversalDecision base = ClassifyTraversal(movement, config, probe.obstacle);
    if (base.move != TraversalMove::Vault && base.move != TraversalMove::Mantle && base.move != TraversalMove::Climb) return base;
    if (!probe.pathClear) return {TraversalMove::Blocked, "path to the obstacle is not clear"};
    if (!Finite(probe.obstacleSpeed) || probe.obstacleSpeed > safety.maxObstacleSpeed) return {TraversalMove::Blocked, "the obstacle is moving"};
    if (!probe.groundValid) return {TraversalMove::Blocked, "the surface behind or on top is not walkable"};
    if (!Finite(probe.obstacleWidth) || probe.obstacleWidth < safety.capsuleDiameter) return {TraversalMove::Blocked, "obstacle narrower than the capsule (walk around it)"};
    if (!Finite(probe.landingWidth) || probe.landingWidth < safety.capsuleDiameter) return {TraversalMove::Blocked, "landing narrower than the capsule"};
    if (!Finite(probe.approachAngleDegrees) || std::abs(probe.approachAngleDegrees) > safety.maxApproachAngle) return {TraversalMove::Blocked, "approach too oblique"};
    if (base.move == TraversalMove::Vault) {
        if (!Finite(probe.landingClearDistance) || probe.landingClearDistance < safety.minVaultLanding) return {TraversalMove::Blocked, "no free landing behind the obstacle"};
        if (!Finite(probe.sideClearance) || probe.sideClearance < safety.minSideClearance) return {TraversalMove::Blocked, "no room beside the obstacle for a vault"};
        return base;
    }
    if (!Finite(probe.topSurfaceAngleDegrees) || probe.topSurfaceAngleDegrees > safety.maxTopSurfaceAngle) return {TraversalMove::Blocked, "top surface too steep"};
    if (!probe.capsuleFitsOnTop) return {TraversalMove::Blocked, "no room for the capsule on top"};
    return base;
}

bool TraversalOwnsMovement(const TraversalState& state) noexcept {
    return state.phase == TraversalPhase::Approach || state.phase == TraversalPhase::Active;
}

} // namespace spn::core
