#include <spn/core/Creature.h>

#include <algorithm>
#include <cmath>

namespace spn::core {

namespace {
bool Finite(double v) noexcept { return std::isfinite(v); }
double Dist(Vec2 a, Vec2 b) noexcept { return std::hypot(a.x - b.x, a.y - b.y); }
} // namespace

std::string_view ToString(CreatureState state) noexcept
{
    switch (state) {
    case CreatureState::Patrol: return "Patrol";
    case CreatureState::Investigate: return "Investigate";
    case CreatureState::Chase: return "Chase";
    case CreatureState::Search: return "Search";
    case CreatureState::Stunned: return "Stunned";
    }
    return "Unknown";
}

std::string_view ToString(CreaturePace pace) noexcept
{
    switch (pace) {
    case CreaturePace::Still: return "Still";
    case CreaturePace::Prowl: return "Prowl";
    case CreaturePace::Hunt: return "Hunt";
    }
    return "Unknown";
}

std::optional<std::string> Validate(const CreatureConfig& c)
{
    const double all[] = {c.chaseStrength, c.chaseHearDistance, c.closeHearDistance, c.contactRange, c.catchRange, c.reactionDelay,
                          c.chaseLoseSeconds, c.chaseGiveUpSeconds, c.searchSeconds, c.searchRadiusStart, c.searchRadiusGrowth, c.searchRadiusMax,
                          c.investigateSeconds, c.stunSeconds, c.waryHearingScale, c.warySeconds, c.arrivalRadius};
    for (double v : all) {
        if (!Finite(v) || v < 0.0) {
            return std::string("every creature value must be finite and >= 0");
        }
    }
    if (c.chaseStrength > 1.0) return std::string("chaseStrength must be <= 1");
    if (c.catchRange <= 0.0 || c.catchRange > c.contactRange) return std::string("catchRange must be > 0 and <= contactRange");
    if (c.closeHearDistance > c.chaseHearDistance) return std::string("closeHearDistance must be <= chaseHearDistance");
    if (c.searchRadiusStart > c.searchRadiusMax) return std::string("searchRadiusStart must be <= searchRadiusMax");
    if (c.waryHearingScale < 1.0) return std::string("waryHearingScale must be >= 1");
    if (c.chaseGiveUpSeconds < c.chaseLoseSeconds) return std::string("chaseGiveUpSeconds must be >= chaseLoseSeconds");
    if (c.chaseLoseSeconds <= 0.0 || c.stunSeconds <= 0.0 || c.arrivalRadius <= 0.0) return std::string("chaseLoseSeconds, stunSeconds and arrivalRadius must be > 0");
    return std::nullopt;
}

void CreatureBrain::Enter(CreatureState next, CreatureOutput& out)
{
    if (next == state_) {
        return;
    }
    out.previous = state_;
    out.changed = true;
    state_ = next;
    timeInState_ = 0.0;
    ++transitions_;
    if (next == CreatureState::Chase) {
        sinceContact_ = 0.0;
        caughtThisChase_ = false;
    }
    if (next != CreatureState::Patrol && next != CreatureState::Investigate) {
        listenLeft_ = -1.0;
    }
}

bool CreatureBrain::Stun()
{
    const bool fresh = state_ != CreatureState::Stunned;
    CreatureOutput ignored;
    Enter(CreatureState::Stunned, ignored);
    timeInState_ = 0.0;
    listenLeft_ = -1.0;
    return fresh;
}

void CreatureBrain::Reset()
{
    state_ = CreatureState::Patrol;
    timeInState_ = 0.0;
    sinceContact_ = 0.0;
    listenLeft_ = -1.0;
    waryLeft_ = 0.0;
    caughtThisChase_ = false;
    target_ = {};
    pendingTarget_ = {};
}

CreatureOutput CreatureBrain::Update(const CreatureInput& in)
{
    CreatureOutput out;
    out.previous = state_;
    const double dt = Finite(in.dt) && in.dt > 0.0 ? in.dt : 0.0;
    timeInState_ += dt;
    waryLeft_ = std::max(0.0, waryLeft_ - dt);

    std::optional<CreatureNoise> noise = in.noise;
    if (noise && (!Finite(noise->position.x) || !Finite(noise->position.y) || !Finite(noise->strength) || !Finite(noise->distance))) {
        noise.reset();
    }
    std::optional<double> prey = in.preyDistance;
    if (prey && (!Finite(*prey) || *prey < 0.0)) {
        prey.reset();
    }
    const bool contact = prey && *prey <= config_.contactRange;

    if (state_ == CreatureState::Stunned) {
        if (timeInState_ >= config_.stunSeconds) {
            target_ = in.self; // it comes to where it stood; the prey may be anywhere
            Enter(CreatureState::Search, out);
        }
    } else {
        // what counts as finding the prey: a loud noise not too far, any noise very close while already on the hunt, or contact
        const bool hunting = state_ != CreatureState::Patrol;
        const bool preyNoise = noise && ((noise->strength >= config_.chaseStrength && noise->distance <= config_.chaseHearDistance) ||
                                         (hunting && noise->distance <= config_.closeHearDistance));
        if (state_ == CreatureState::Chase) {
            if (preyNoise || contact) {
                target_ = contact ? in.preyPosition : noise->position;
                sinceContact_ = 0.0;
            } else {
                sinceContact_ += dt;
                if (noise) {
                    target_ = noise->position; // a far noise during a pursuit still pulls it
                }
            }
            if (prey && *prey <= config_.catchRange && !caughtThisChase_) {
                out.caught = true;
                caughtThisChase_ = true;
            }
            const bool reached = Dist(in.self, target_) <= config_.arrivalRadius;
            if ((sinceContact_ >= config_.chaseLoseSeconds && reached) || sinceContact_ >= config_.chaseGiveUpSeconds) {
                Enter(CreatureState::Search, out);
            }
        } else if (contact) {
            target_ = in.preyPosition; // touched: no listening, no head start
            Enter(CreatureState::Chase, out);
        } else if (preyNoise && listenLeft_ < 0.0) {
            pendingTarget_ = noise->position;
            listenLeft_ = config_.reactionDelay;
        } else if (preyNoise) {
            pendingTarget_ = noise->position; // it keeps listening, the target follows the sound
        } else if (noise && state_ != CreatureState::Chase) {
            target_ = noise->position;
            Enter(CreatureState::Investigate, out);
            timeInState_ = 0.0; // a new noise restarts the investigation
        }

        if (state_ != CreatureState::Chase && listenLeft_ >= 0.0) {
            listenLeft_ -= dt;
            if (listenLeft_ < 0.0) {
                target_ = pendingTarget_;
                Enter(CreatureState::Chase, out);
            }
        }

        if (state_ == CreatureState::Investigate && listenLeft_ < 0.0 && timeInState_ >= config_.investigateSeconds) {
            Enter(CreatureState::Patrol, out);
        } else if (state_ == CreatureState::Search && listenLeft_ < 0.0 && timeInState_ >= config_.searchSeconds) {
            waryLeft_ = config_.warySeconds;
            Enter(CreatureState::Patrol, out);
        }
    }

    out.state = state_;
    out.listening = listenLeft_ >= 0.0;
    switch (state_) {
    case CreatureState::Patrol:
        out.hasGoal = false;
        out.pace = out.listening ? CreaturePace::Still : CreaturePace::Prowl;
        break;
    case CreatureState::Investigate:
        out.hasGoal = true;
        out.goal = target_;
        out.pace = out.listening ? CreaturePace::Still : CreaturePace::Prowl;
        break;
    case CreatureState::Chase:
        out.hasGoal = true;
        out.goal = target_;
        out.pace = CreaturePace::Hunt;
        break;
    case CreatureState::Search:
        out.hasGoal = true;
        out.goal = target_;
        out.searchRadius = std::min(config_.searchRadiusMax, config_.searchRadiusStart + config_.searchRadiusGrowth * timeInState_);
        out.pace = out.listening ? CreaturePace::Still : CreaturePace::Prowl;
        break;
    case CreatureState::Stunned:
        out.hasGoal = false;
        out.pace = CreaturePace::Still;
        break;
    }
    return out;
}

bool BellReaches(double distance, double bellRange, int walls) noexcept
{
    if (!Finite(distance) || !Finite(bellRange) || distance < 0.0 || bellRange <= 0.0 || walls < 0) {
        return false;
    }
    double range = bellRange;
    for (int i = 0; i < walls && i < 16; ++i) {
        range *= 0.5;
    }
    return distance <= range;
}

bool SaveAllowed(const std::vector<CreatureState>& creatures) noexcept
{
    return std::none_of(creatures.begin(), creatures.end(), [](CreatureState s) { return s == CreatureState::Chase; });
}

} // namespace spn::core
