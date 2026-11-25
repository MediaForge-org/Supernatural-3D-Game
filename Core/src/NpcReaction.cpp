#include <spn/core/NpcReaction.h>

#include <algorithm>
#include <cmath>
#include <limits>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(Vec2 v) noexcept { return std::isfinite(v.x) && std::isfinite(v.y); }
double Dist(Vec2 a, Vec2 b) noexcept { return std::hypot(a.x - b.x, a.y - b.y); }

constexpr std::uint64_t kObjectBit = 1ull << 62;
constexpr std::uint64_t kUnknownActorValue = 1ull << 63;

} // namespace

// ------------------------------------------------------------------ noises and senses

std::string_view ToString(NoiseKind kind) noexcept {
    switch (kind) {
    case NoiseKind::Footstep: return "footstep";
    case NoiseKind::DoorUse: return "door_use";
    case NoiseKind::DoorBreak: return "door_break";
    case NoiseKind::Impact: return "impact";
    case NoiseKind::Shout: return "shout";
    }
    return "unknown";
}

bool IsSuspicious(NoiseKind kind, double strength, double suspiciousStrength) noexcept {
    if (kind == NoiseKind::DoorBreak || kind == NoiseKind::Impact || kind == NoiseKind::Shout) return true;
    return std::isfinite(strength) && strength >= suspiciousStrength;
}

FootstepNoise FootstepNoiseFor(double horizontalSpeedCmS, bool crouched, const FootstepNoiseConfig& config) noexcept {
    FootstepNoise out;
    if (!std::isfinite(horizontalSpeedCmS) || horizontalSpeedCmS < config.minSpeed || config.sprintSpeed <= config.walkSpeed) return out;
    const double t = std::clamp((horizontalSpeedCmS - config.walkSpeed) / (config.sprintSpeed - config.walkSpeed), 0.0, 1.0);
    const double factor = crouched ? config.crouchFactor : 1.0;
    out.range = (config.quietRange + t * (config.loudRange - config.quietRange)) * factor;
    out.strength = (config.quietStrength + t * (config.loudStrength - config.quietStrength)) * factor;
    out.audible = out.range > 0.0 && out.strength > 0.0;
    return out;
}

HearingResult CanHear(const HearingConfig& config, const GameplayNoise& noise, Vec2 listener, int walls) noexcept {
    HearingResult r;
    if (!Finite(noise.position) || !Finite(listener) || !std::isfinite(noise.range) || !std::isfinite(noise.strength)) return r;
    if (noise.range <= 0.0 || noise.strength <= 0.0 || walls < 0) return r;
    if (!std::isfinite(config.sensitivity) || config.sensitivity <= 0.0) return r;
    if (!std::isfinite(config.wallAttenuation) || config.wallAttenuation < 0.0 || config.wallAttenuation > 1.0) return r;
    r.distance = Dist(noise.position, listener);
    r.effectiveRange = noise.range * config.sensitivity * std::pow(config.wallAttenuation, walls);
    r.heard = r.distance <= r.effectiveRange;
    return r;
}

bool CanSee(const NpcPerceptionConfig& config, Vec2 eye, double facingDegrees, Vec2 target, bool lineClear) noexcept {
    if (!lineClear || !Finite(eye) || !Finite(target) || !std::isfinite(facingDegrees)) return false;
    const double dx = target.x - eye.x, dy = target.y - eye.y;
    const double distance = std::hypot(dx, dy);
    if (distance > config.sightRange) return false;
    if (distance <= config.closeSenseRange) return true; // very close WITH a clear line: seen regardless of facing
    double diff = std::atan2(dy, dx) * 180.0 / kPi - facingDegrees;
    diff = std::fmod(diff + 180.0, 360.0);
    if (diff < 0.0) diff += 360.0;
    diff -= 180.0;
    return std::abs(diff) <= config.fieldOfViewDegrees / 2.0;
}

// ------------------------------------------------------------------ reaction state machine

std::string_view ToString(NpcReactionState state) noexcept {
    switch (state) {
    case NpcReactionState::Idle: return "Idle";
    case NpcReactionState::Investigate: return "Investigate";
    case NpcReactionState::Alert: return "Alert";
    case NpcReactionState::Flee: return "Flee";
    case NpcReactionState::ReturnToRoutine: return "ReturnToRoutine";
    }
    return "Unknown";
}

std::optional<std::string> Validate(const NpcReactionConfig& c) {
    const double values[] = {c.minDwell, c.lookSeconds, c.investigateTimeout, c.alertHoldSeconds, c.fleeTriggerDistance, c.fleeTriggerSeconds, c.fleeTriggerSecondsWary,
                             c.fleeSeconds, c.safeDistance, c.returnTimeout, c.arrivalRadius, c.sameNoiseRadius, c.noiseReactionCooldown};
    for (const double v : values) {
        if (!std::isfinite(v) || v < 0.0) return "NpcReactionConfig: every value must be finite and >= 0";
    }
    if (c.arrivalRadius <= 0.0) return "NpcReactionConfig: arrivalRadius must be > 0";
    if (c.safeDistance <= c.fleeTriggerDistance) return "NpcReactionConfig: safeDistance must exceed fleeTriggerDistance (otherwise flee and alert would flap)";
    if (c.investigateTimeout <= 0.0 || c.returnTimeout <= 0.0 || c.fleeSeconds <= 0.0) return "NpcReactionConfig: timeouts must be > 0";
    return std::nullopt;
}

void NpcReactionBrain::Enter(NpcReactionState next, ReactionOutput& out) {
    if (next == state_) return;
    state_ = next;
    timeInState_ = 0.0;
    ++transitions_;
    out.changed = true;
    switch (next) {
    case NpcReactionState::Idle:
        episodeTime_ = 0.0;
        suspicious_ = false;
        alarmRaised_ = false;
        closeTime_ = 0.0;
        lookTimer_ = 0.0;
        break;
    case NpcReactionState::Investigate:
        lookTimer_ = 0.0;
        suspicious_ = true; // investigating always means something was off: sighting the player now escalates
        break;
    case NpcReactionState::Alert:
        closeTime_ = 0.0;
        if (!alarmRaised_) {
            alarmRaised_ = true;
            out.raiseAlarm = true;
        }
        break;
    case NpcReactionState::Flee:
    case NpcReactionState::ReturnToRoutine:
        break;
    }
}

void NpcReactionBrain::Restore(NpcReactionState state, Vec2 target) {
    state_ = state;
    target_ = target;
    timeInState_ = 0.0;
    episodeTime_ = 0.0;
    sinceSeen_ = 1e9;
    closeTime_ = 0.0;
    lookTimer_ = 0.0;
    sinceNoiseReaction_ = 1e9;
    suspicious_ = state != NpcReactionState::Idle;
    alarmRaised_ = state == NpcReactionState::Alert || state == NpcReactionState::Flee;
}

ReactionOutput NpcReactionBrain::Update(const ReactionInput& in) {
    ReactionOutput out;
    out.previous = state_;
    const bool inputOk = Finite(in.self) && Finite(in.home) && std::isfinite(in.dt) && in.dt >= 0.0 && (!in.seesPlayer || Finite(in.playerPosition));
    const double dt = inputOk ? in.dt : 0.0;

    timeInState_ += dt;
    sinceNoiseReaction_ += dt;
    if (state_ != NpcReactionState::Idle) episodeTime_ += dt;

    const bool sees = inputOk && in.seesPlayer;
    if (sees) {
        sinceSeen_ = 0.0;
        lastPlayer_ = in.playerPosition;
    } else {
        sinceSeen_ += dt;
    }
    const double playerDistance = sees ? Dist(in.self, in.playerPosition) : std::numeric_limits<double>::infinity();
    const bool dwellOk = timeInState_ >= config_.minDwell;

    // A noise is only acted on if the cooldown since the last noise reaction has passed (older/duplicate ones are simply dropped).
    const bool noiseUsable = inputOk && in.noise.has_value() && Finite(in.noise->position) && sinceNoiseReaction_ >= config_.noiseReactionCooldown;
    const bool noiseSuspicious = noiseUsable && in.noise->suspicious;
    const bool noiseCauses = noiseUsable && (in.noise->suspicious || in.wary);

    switch (state_) {
    case NpcReactionState::Idle:
        if (sees && in.wary) {
            suspicious_ = true;
            Enter(NpcReactionState::Alert, out);
        } else if (noiseCauses) {
            target_ = in.noise->position;
            sinceNoiseReaction_ = 0.0;
            episodeTime_ = 0.0;
            Enter(NpcReactionState::Investigate, out);
        }
        break;

    case NpcReactionState::Investigate:
        if (sees && (suspicious_ || in.wary)) {
            Enter(NpcReactionState::Alert, out);
            break;
        }
        if (noiseSuspicious) {
            sinceNoiseReaction_ = 0.0;
            if (Dist(in.noise->position, target_) > config_.sameNoiseRadius) {
                target_ = in.noise->position;
                lookTimer_ = 0.0; // new place to search; the episode timeout still runs
            }
        }
        if (Dist(in.self, target_) <= config_.arrivalRadius) lookTimer_ += dt;
        if (dwellOk && (lookTimer_ >= config_.lookSeconds || episodeTime_ >= config_.investigateTimeout)) Enter(NpcReactionState::ReturnToRoutine, out);
        break;

    case NpcReactionState::Alert: {
        const bool close = sees && playerDistance <= config_.fleeTriggerDistance;
        closeTime_ = close ? closeTime_ + dt : 0.0;
        if (close && closeTime_ >= (in.wary ? config_.fleeTriggerSecondsWary : config_.fleeTriggerSeconds)) {
            Enter(NpcReactionState::Flee, out);
        } else if (!sees && dwellOk && sinceSeen_ >= config_.alertHoldSeconds) {
            target_ = lastPlayer_; // lost sight: go and look where the player was last seen
            lookTimer_ = 0.0;
            episodeTime_ = 0.0;
            Enter(NpcReactionState::Investigate, out);
        }
        break;
    }

    case NpcReactionState::Flee:
        if (dwellOk && timeInState_ >= config_.fleeSeconds && (!sees || playerDistance >= config_.safeDistance)) Enter(NpcReactionState::ReturnToRoutine, out);
        break;

    case NpcReactionState::ReturnToRoutine:
        if (sees && (suspicious_ || in.wary)) {
            Enter(NpcReactionState::Alert, out);
        } else if (noiseCauses) {
            target_ = in.noise->position;
            sinceNoiseReaction_ = 0.0;
            episodeTime_ = 0.0;
            Enter(NpcReactionState::Investigate, out);
        } else if ((dwellOk && Dist(in.self, in.home) <= config_.arrivalRadius) || timeInState_ >= config_.returnTimeout) {
            Enter(NpcReactionState::Idle, out);
        }
        break;
    }

    out.state = state_;
    switch (state_) {
    case NpcReactionState::Idle:
        break;
    case NpcReactionState::Investigate:
        out.hasGoal = true;
        out.goal = target_;
        out.gait = Gait::Walk;
        break;
    case NpcReactionState::Alert:
        out.facePlayer = true;
        out.threat = lastPlayer_;
        break;
    case NpcReactionState::Flee:
        out.fleeAway = true;
        out.threat = lastPlayer_;
        out.gait = Gait::Sprint;
        break;
    case NpcReactionState::ReturnToRoutine:
        out.hasGoal = true;
        out.goal = in.home;
        out.gait = Gait::Walk;
        break;
    }
    return out;
}

// ------------------------------------------------------------------ memory

EntityId ObjectEntityId(std::string_view objectId) noexcept {
    std::uint64_t h = 1469598103934665603ull;
    for (const char c : objectId) {
        h ^= static_cast<unsigned char>(c);
        h *= 1099511628211ull;
    }
    return EntityId((h & (kObjectBit - 1ull)) | kObjectBit);
}

bool IsObjectEntity(EntityId id) noexcept { return (id.Value() & kObjectBit) != 0 && (id.Value() & kUnknownActorValue) == 0; }

EntityId UnknownActor() noexcept { return EntityId(kUnknownActorValue); }

std::optional<NpcMemoryError> RememberIncident(NpcMemoryStore& store, EntityId npc, EntityId player, const Incident& incident) {
    NpcEventInput e;
    e.time = std::max(incident.time, store.Clock()); // events may not be older than the store clock
    e.location = incident.where;
    e.intensity = std::clamp(std::isfinite(incident.severity) ? incident.severity : 0.0, 0.0, 1.0);
    const double accuracy = std::clamp(std::isfinite(incident.accuracy) ? incident.accuracy : 0.0, 0.0, 1.0);
    switch (incident.kind) {
    case IncidentKind::HeardSuspicious:
        e.kind = NpcEventKind::Witnessed;
        e.actor = UnknownActor();
        break;
    case IncidentKind::HeardDoorBroken:
        e.kind = NpcEventKind::Witnessed;
        e.actor = UnknownActor();
        e.subject = incident.object;
        break;
    case IncidentKind::SawPlayer:
        e.kind = NpcEventKind::Encounter;
        e.actor = player;
        break;
    case IncidentKind::Alarmed:
        e.kind = NpcEventKind::Threatened;
        e.actor = player;
        break;
    }
    e.witnesses.push_back(Witness{npc, true, accuracy});
    auto r = store.RecordEvent(e);
    if (!r) return r.Error();
    return std::nullopt;
}

NpcWariness AssessWariness(const NpcMemoryStore& store, EntityId npc, EntityId player, double now, const WarinessConfig& config) {
    NpcWariness w;
    const auto* memories = store.Memories(npc);
    if (memories == nullptr) return w;
    double sum = 0.0;
    for (const MemoryRecord& m : *memories) {
        double weight = 0.0;
        bool incident = false;
        switch (m.kind) {
        case NpcEventKind::Witnessed:
            weight = IsObjectEntity(m.subject) ? 1.0 : 0.7;
            incident = true;
            if (IsObjectEntity(m.subject)) w.brokeInRemembered = true;
            break;
        case NpcEventKind::Threatened:
            if (m.actor == player) {
                weight = 1.0;
                incident = true;
                if (now - m.time <= config.resumeWindow) w.recentAlarm = true;
            }
            break;
        case NpcEventKind::Encounter:
            if (m.actor == player) {
                weight = 0.15;
                w.playerSeenRemembered = true;
            }
            break;
        default:
            break;
        }
        if (weight <= 0.0) continue;
        sum += weight * store.EffectiveStrength(npc, m, now);
        if (incident && (!w.lastIncident || m.time >= w.lastIncidentTime)) {
            w.lastIncident = m.location;
            w.lastIncidentTime = m.time;
        }
    }
    w.suspicion = std::min(1.0, sum);
    w.wary = w.suspicion >= config.waryAt;
    return w;
}

RestoredReaction InitialStateFromMemory(const NpcWariness& wariness) {
    RestoredReaction r;
    if (wariness.recentAlarm && wariness.lastIncident) {
        r.state = NpcReactionState::Investigate;
        r.target = *wariness.lastIncident;
    }
    return r;
}

} // namespace spn::core
