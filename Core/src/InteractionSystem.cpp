#include <spn/core/InteractionSystem.h>

#include <algorithm>
#include <cmath>
#include <utility>
#include <spn/core/TestDoor.h>
#include <variant>

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

ContentId MustParse(std::string_view text) { return *ContentId::Parse(text); }

bool Finite(const ReachVec3& v) noexcept { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }

std::int64_t CellOf(double v) noexcept { return static_cast<std::int64_t>(std::floor(v / kInteractionGridCell)); }
std::uint64_t CellKey(std::int64_t cx, std::int64_t cy) noexcept { return (static_cast<std::uint64_t>(cx) << 32) ^ (static_cast<std::uint64_t>(cy) & 0xffffffffu); }

std::string_view LastSegment(const ContentId& id) noexcept {
    const std::string_view value = id.Value();
    const std::size_t dot = value.rfind('.');
    const std::size_t colon = value.rfind(':');
    const std::size_t start = dot != std::string_view::npos ? dot + 1 : (colon != std::string_view::npos ? colon + 1 : 0);
    return value.substr(start);
}

std::int64_t AsInt(const std::optional<FactValue>& value) noexcept {
    if (!value) return 0;
    if (const bool* b = std::get_if<bool>(&*value)) return *b ? 1 : 0;
    if (const auto* i = std::get_if<std::int64_t>(&*value)) return *i;
    return 0;
}

bool IsDoor(const ContentId& kind) { return LastSegment(kind) == "door"; }

} // namespace

std::function<bool(const WorldFacts&, const InteractableRef&, const InteractionDefaults&)>
MakeDoorCloseObstructionRule(std::function<bool(std::string_view doorId)> swingObstructed) {
    return [swingObstructed = std::move(swingObstructed)](const WorldFacts& facts, const InteractableRef& ref, const InteractionDefaults& defaults) {
        if (!swingObstructed) return false;
        bool open = false;
        if (const FactValue* fact = facts.Find(TestDoorOpenKey(ref.id))) {
            const bool* v = std::get_if<bool>(fact);
            if (v == nullptr) return false; // corrupt state is reported by the rules, not guessed here
            open = *v;
        } else if (const auto found = defaults.values.find(kTestDoorOpenState); found != defaults.values.end()) {
            if (const bool* v = std::get_if<bool>(&found->second)) open = *v;
        }
        return open && swingObstructed(ref.id);
    };
}

std::string ObjectFactPrefix(const ContentId& kind, std::string_view id) {
    if (kind.IsEmpty() || !IsValidInteractableId(id)) return {};
    return std::string(kind.Namespace()).append(".").append(LastSegment(kind)).append(".").append(id).append(".");
}

std::string_view PrimaryStateField(const ContentId& kind) noexcept {
    const std::string_view segment = LastSegment(kind);
    if (segment == "door") return "open";
    if (segment == "switch") return "on";
    if (segment == "button") return "presses";
    if (segment == "lever") return "position";
    return {};
}

InteractStatus ClassifyRefusal(const ContentId& feedback) noexcept {
    const std::string_view last = LastSegment(feedback);
    if (last == "locked") return InteractStatus::Locked;
    if (last == "disabled") return InteractStatus::Disabled;
    if (last == "exhausted" || last == "already") return InteractStatus::AlreadyInState;
    return InteractStatus::Blocked; // blocked, cooldown and anything unknown: nothing happened, try later
}

const ContentId& InteractionCooldownFeedback() { static const ContentId k = MustParse("placeholder:feedback.interaction.cooldown"); return k; }
const ContentId& InteractionReentrantFeedback() { static const ContentId k = MustParse("placeholder:feedback.interaction.reentrant"); return k; }
const ContentId& InteractionActorStateFeedback() { static const ContentId k = MustParse("placeholder:feedback.interaction.actor_state"); return k; }

std::string_view ToString(SystemErrorCode code) noexcept {
    switch (code) {
    case SystemErrorCode::InvalidId: return "invalid id";
    case SystemErrorCode::DuplicateId: return "duplicate id";
    case SystemErrorCode::UnknownKind: return "unknown kind";
    case SystemErrorCode::InvalidNumber: return "invalid number";
    case SystemErrorCode::UnknownObject: return "unknown object";
    case SystemErrorCode::InvalidLink: return "invalid link";
    case SystemErrorCode::DuplicateLink: return "duplicate link";
    case SystemErrorCode::LinkCycle: return "link cycle";
    }
    return "unknown error";
}

struct InteractionSystem::Candidate {
    const InteractableDescriptor* object;
    double angle;
    double distance;
};

InteractionSystem::InteractionSystem(const InteractionRegistry& registry, WorldFacts& facts, GameEventBus* bus, ReachConfig reach)
    : registry_(registry), facts_(facts), bus_(bus), reach_(reach) {}

std::optional<SystemError> InteractionSystem::AddInteractable(InteractableDescriptor d) {
    if (!IsValidInteractableId(d.id)) return SystemError{SystemErrorCode::InvalidId, d.id};
    if (!registry_.IsRegistered(d.kind)) return SystemError{SystemErrorCode::UnknownKind, d.kind.Value()};
    if (!Finite(d.centre) || !std::isfinite(d.radius) || d.radius < 0.0 || d.cooldownMs < 0 || std::abs(d.centre.x) > kMaxWorldCoordinate ||
        std::abs(d.centre.y) > kMaxWorldCoordinate || d.radius > kMaxWorldCoordinate) {
        return SystemError{SystemErrorCode::InvalidNumber, d.id};
    }
    if (objects_.contains(d.id)) return SystemError{SystemErrorCode::DuplicateId, d.id};
    std::string id = d.id;
    const auto inserted = objects_.emplace(std::move(id), std::move(d)).first;
    const InteractableDescriptor& stored = inserted->second;
    grid_[CellKey(CellOf(stored.centre.x), CellOf(stored.centre.y))].push_back(&stored);
    maxRadius_ = std::max(maxRadius_, stored.radius);
    return std::nullopt;
}

bool InteractionSystem::RemoveInteractable(std::string_view id) {
    const auto found = objects_.find(id);
    if (found == objects_.end()) return false;
    std::erase_if(links_, [&](const Link& l) { return l.source == id || l.target == id; });
    {
        auto cell = grid_.find(CellKey(CellOf(found->second.centre.x), CellOf(found->second.centre.y)));
        if (cell != grid_.end()) {
            std::erase(cell->second, &found->second);
            if (cell->second.empty()) grid_.erase(cell);
        }
    }
    lastUseMs_.erase(std::string(id));
    objects_.erase(found);
    return true;
}

bool InteractionSystem::SetEnabled(std::string_view id, bool enabled) {
    const auto found = objects_.find(id);
    if (found == objects_.end()) return false;
    found->second.enabled = enabled;
    return true;
}

const InteractableDescriptor* InteractionSystem::Find(std::string_view id) const {
    const auto found = objects_.find(id);
    return found == objects_.end() ? nullptr : &found->second;
}

void InteractionSystem::SetBlockRule(const ContentId& kind, BlockRule rule) {
    if (!rule) {
        blockRules_.erase(kind);
    } else {
        blockRules_[kind] = std::move(rule);
    }
}

std::vector<InteractionSystem::Candidate> InteractionSystem::Rank(const ActorView& actor) const {
    std::vector<Candidate> result;
    if (!MayInteract(actor.state) || !Finite(actor.eye) || !Finite(actor.viewDirection)) return result;
    const double viewLength = std::sqrt(actor.viewDirection.x * actor.viewDirection.x + actor.viewDirection.y * actor.viewDirection.y + actor.viewDirection.z * actor.viewDirection.z);
    if (viewLength < 1e-9) return result;
    // Broad phase: only the grid cells within reach (+ the biggest target radius) of the eye are looked at, never all objects.
    if (std::abs(actor.eye.x) > kMaxWorldCoordinate || std::abs(actor.eye.y) > kMaxWorldCoordinate) return result;
    const double span = reach_.maxDistance + maxRadius_;
    const std::int64_t x0 = CellOf(actor.eye.x - span), x1 = CellOf(actor.eye.x + span), y0 = CellOf(actor.eye.y - span), y1 = CellOf(actor.eye.y + span);
    if ((x1 - x0 + 1) * (y1 - y0 + 1) > 4096) { // absurd reach configuration: fall back to a full scan instead of walking millions of cells
        return RankScan(actor, viewLength);
    }
    std::vector<const InteractableDescriptor*> near;
    for (std::int64_t cx = x0; cx <= x1; ++cx) {
        for (std::int64_t cy = y0; cy <= y1; ++cy) {
            const auto cell = grid_.find(CellKey(cx, cy));
            if (cell != grid_.end()) near.insert(near.end(), cell->second.begin(), cell->second.end());
        }
    }
    return RankCandidates(actor, viewLength, near);
}

std::vector<InteractionSystem::Candidate> InteractionSystem::RankScan(const ActorView& actor, double viewLength) const {
    std::vector<const InteractableDescriptor*> all;
    all.reserve(objects_.size());
    for (const auto& [id, object] : objects_) all.push_back(&object);
    return RankCandidates(actor, viewLength, all);
}

std::vector<InteractionSystem::Candidate> InteractionSystem::RankCandidates(const ActorView& actor, double viewLength, const std::vector<const InteractableDescriptor*>& pool) const {
    std::vector<Candidate> result;
    for (const InteractableDescriptor* objectPtr : pool) {
        const InteractableDescriptor& object = *objectPtr;
        ReachQuery q;
        q.eye = actor.eye;
        q.viewDirection = actor.viewDirection;
        q.targetCentre = object.centre;
        q.targetRadius = object.radius;
        q.lineOfSightClear = !reach_.requireLineOfSight || !lineOfSight_ || lineOfSight_(object);
        if (CheckReach(reach_, q) != ReachResult::Ok) continue;
        const ReachVec3 to{object.centre.x - actor.eye.x, object.centre.y - actor.eye.y, object.centre.z - actor.eye.z};
        const double distance = std::sqrt(to.x * to.x + to.y * to.y + to.z * to.z);
        double angle = 0.0;
        if (distance > 1e-9) {
            const double c = std::clamp((to.x * actor.viewDirection.x + to.y * actor.viewDirection.y + to.z * actor.viewDirection.z) / (distance * viewLength), -1.0, 1.0);
            angle = std::acos(c) * 180.0 / kPi;
        }
        result.push_back(Candidate{&object, angle, distance});
    }
    std::sort(result.begin(), result.end(), [](const Candidate& a, const Candidate& b) {
        if (a.object->priority != b.object->priority) return a.object->priority > b.object->priority;
        if (a.angle != b.angle) return a.angle < b.angle;
        if (a.distance != b.distance) return a.distance < b.distance;
        return a.object->id < b.object->id;
    });
    return result;
}

std::optional<std::string> InteractionSystem::SelectTarget(const ActorView& actor) const {
    const auto ranked = Rank(actor);
    if (ranked.empty()) return std::nullopt;
    return ranked.front().object->id;
}

std::vector<std::string> InteractionSystem::ReachableTargets(const ActorView& actor) const {
    std::vector<std::string> ids;
    for (const Candidate& c : Rank(actor)) ids.push_back(c.object->id);
    return ids;
}

namespace {

struct BusyGuard {
    explicit BusyGuard(bool& flag) : flag_(flag) { flag_ = true; }
    ~BusyGuard() { flag_ = false; }
    bool& flag_;
};

} // namespace

void InteractionSystem::PublishChange(const ChangeRecord& change, const InteractableDescriptor& object, const std::string& actor) {
    if (bus_ == nullptr) return;
    GameEvent e;
    e.type = IsDoor(object.kind) ? GameEventType::DoorStateChanged : GameEventType::DeviceStateChanged;
    e.objectId = object.id;
    e.kind = object.kind;
    e.actor = actor;
    e.from = AsInt(change.previous);
    e.to = AsInt(change.current);
    bus_->Publish(std::move(e));
}

void InteractionSystem::RunLinks(const InteractableDescriptor& source, const ChangeRecord& change, std::vector<ChangeRecord>& out, int depth) {
    if (depth > kMaxLinkDepth) return;
    const SetFact* set = std::get_if<SetFact>(&change.command);
    const std::string prefix = ObjectFactPrefix(source.kind, source.id);
    const std::string_view field = PrimaryStateField(source.kind);
    if (set == nullptr || field.empty() || set->key != prefix + std::string(field)) return;
    const std::int64_t state = AsInt(change.current);
    // copy: applying a link may be observed by listeners, which must not invalidate the iteration
    if (links_.empty()) return;
    const std::vector<Link> links = links_;
    for (const Link& link : links) {
        if (link.source != source.id) continue;
        const FactValue* chosen = nullptr;
        if (!link.whenState || *link.whenState == state) {
            chosen = &link.value;
        } else if (link.otherwise) {
            chosen = &*link.otherwise;
        }
        if (chosen == nullptr) continue;
        const FactValue* current = facts_.Find(link.targetKey);
        if (current != nullptr && *current == *chosen) continue; // already in the requested state: no record, no event
        auto applied = facts_.Apply(SetFact{link.targetKey, *chosen}, "link:" + link.source + ">" + link.target);
        if (!applied) continue; // e.g. a type mismatch with existing state: the link does nothing rather than corrupt the target
        const ChangeRecord record = applied.Value();
        out.push_back(record);
        const InteractableDescriptor* target = Find(link.target);
        if (bus_ != nullptr) {
            GameEvent linked;
            linked.type = GameEventType::LinkedMechanismActivated;
            linked.objectId = link.target;
            linked.sourceId = link.source;
            linked.kind = target != nullptr ? target->kind : ContentId{};
            linked.from = AsInt(record.previous);
            linked.to = AsInt(record.current);
            bus_->Publish(std::move(linked));
        }
        if (target != nullptr) {
            PublishChange(record, *target, {});
            RunLinks(*target, record, out, depth + 1);
        }
    }
}

InteractionResult InteractionSystem::Interact(const InteractionRequest& request) {
    InteractionResult result;
    result.targetId = request.targetId;
    if (busy_) {
        // A listener called Interact while this system was executing one: refuse instead of nesting state changes.
        result.status = InteractStatus::Blocked;
        result.feedback = InteractionReentrantFeedback();
        result.detail = "reentrant interaction";
        return result;
    }
    BusyGuard guard(busy_);

    const auto publish = [&](GameEventType type, InteractStatus status, const ContentId& feedback, const InteractableDescriptor* object) {
        if (bus_ == nullptr) return;
        GameEvent e;
        e.type = type;
        e.status = status;
        e.feedback = feedback;
        e.objectId = request.targetId;
        e.actor = request.actor.actor;
        if (object != nullptr) e.kind = object->kind;
        bus_->Publish(std::move(e));
    };
    const auto reject = [&](InteractStatus status, ContentId feedback, std::string detail, const InteractableDescriptor* object) -> InteractionResult {
        result.status = status;
        result.feedback = std::move(feedback);
        result.detail = std::move(detail);
        publish(GameEventType::InteractionRejected, status, result.feedback, object);
        return result;
    };

    // --- input
    if (!IsValidHolderId(request.actor.actor) || !IsValidInteractableId(request.targetId) || request.nowMs < 0 || !Finite(request.actor.eye) ||
        !Finite(request.actor.viewDirection)) {
        return reject(InteractStatus::InvalidInput, {}, "invalid actor, target id, clock or view", nullptr);
    }
    if (static_cast<std::size_t>(request.verb) >= kAllInteractionVerbs.size()) {
        return reject(InteractStatus::InvalidInput, {}, "unknown verb", nullptr);
    }
    publish(GameEventType::InteractionAttempted, InteractStatus::Success, {}, Find(request.targetId));

    const InteractableDescriptor* object = Find(request.targetId);
    if (object == nullptr) return reject(InteractStatus::InvalidTarget, {}, "no such interactable", nullptr);
    if (!object->enabled) return reject(InteractStatus::Disabled, {}, "interactable disabled", object);

    // --- actor and reach
    if (!MayInteract(request.actor.state)) return reject(InteractStatus::Blocked, InteractionActorStateFeedback(), "actor cannot interact now", object);
    if (request.requireReach) {
        ReachQuery q;
        q.eye = request.actor.eye;
        q.viewDirection = request.actor.viewDirection;
        q.targetCentre = object->centre;
        q.targetRadius = object->radius;
        q.lineOfSightClear = !reach_.requireLineOfSight || !lineOfSight_ || lineOfSight_(*object);
        switch (CheckReach(reach_, q)) {
        case ReachResult::Ok: break;
        case ReachResult::Obstructed: return reject(InteractStatus::Blocked, {}, "line of sight blocked", object);
        case ReachResult::InvalidInput: return reject(InteractStatus::InvalidInput, {}, "invalid reach input", object);
        default: return reject(InteractStatus::OutOfRange, {}, "target not reachable", object);
        }
    }

    // --- cooldown (runtime-only bookkeeping)
    const auto last = lastUseMs_.find(object->id);
    if (last != lastUseMs_.end()) {
        if (request.nowMs < last->second) return reject(InteractStatus::InvalidInput, {}, "clock went backwards", object);
        if (request.nowMs - last->second < object->cooldownMs) return reject(InteractStatus::Blocked, InteractionCooldownFeedback(), "cooldown", object);
    }

    // --- engine-side blocking (e.g. an actor inside the door's swing)
    const InteractableRef ref{object->kind, object->id};
    if (const auto rule = blockRules_.find(object->kind); rule != blockRules_.end() && rule->second(facts_, ref, object->defaults)) {
        return reject(InteractStatus::Blocked, TestDoorBlockedFeedback(), "blocked by an obstruction", object);
    }

    // --- rules of the kind
    auto outcome = registry_.Interact(facts_, ref, request.verb, object->defaults, "interact:" + request.actor.actor, request.actor.actor);
    if (!outcome) {
        const InteractionError& error = outcome.Error();
        switch (error.code) {
        case InteractionErrorCode::Refused: return reject(ClassifyRefusal(error.feedback), error.feedback, "refused by the rules", object);
        case InteractionErrorCode::StateCorrupt:
        case InteractionErrorCode::Fact: return reject(InteractStatus::InvalidWorldState, {}, "world state invalid: " + error.subject, object);
        case InteractionErrorCode::InvalidId: return reject(InteractStatus::InvalidInput, {}, "invalid id", object);
        default: return reject(InteractStatus::InvalidTarget, {}, "no rules for this kind", object);
        }
    }
    InteractionOutcome& done = outcome.Value();
    if (done.changes.empty()) {
        return reject(InteractStatus::AlreadyInState, done.feedback, "nothing changed", object);
    }
    lastUseMs_[object->id] = request.nowMs;
    result.status = InteractStatus::Success;
    result.feedback = done.feedback;
    result.changes = done.changes;
    publish(GameEventType::InteractionSucceeded, InteractStatus::Success, result.feedback, object);
    for (const ChangeRecord& change : done.changes) {
        PublishChange(change, *object, request.actor.actor);
        RunLinks(*object, change, result.changes, 0);
    }
    return result;
}

bool InteractionSystem::Reaches(std::string_view from, std::string_view to) const {
    if (from == to) return true;
    for (const Link& l : links_) {
        if (l.source == from && Reaches(l.target, to)) return true;
    }
    return false;
}

std::optional<SystemError> InteractionSystem::AddLink(Link link) {
    const InteractableDescriptor* source = Find(link.source);
    const InteractableDescriptor* target = Find(link.target);
    if (source == nullptr) return SystemError{SystemErrorCode::UnknownObject, link.source};
    if (target == nullptr) return SystemError{SystemErrorCode::UnknownObject, link.target};
    if (link.source == link.target) return SystemError{SystemErrorCode::LinkCycle, link.source};
    if (PrimaryStateField(source->kind).empty()) return SystemError{SystemErrorCode::InvalidLink, link.source}; // the source has no state to react to
    const std::string prefix = ObjectFactPrefix(target->kind, target->id);
    if (!IsValidFactKey(link.targetKey) || link.targetKey.rfind(prefix, 0) != 0) return SystemError{SystemErrorCode::InvalidLink, link.targetKey};
    for (const Link& other : links_) {
        if (other.source == link.source && other.target == link.target && other.targetKey == link.targetKey) {
            return SystemError{SystemErrorCode::DuplicateLink, link.source + ">" + link.target};
        }
    }
    if (Reaches(link.target, link.source)) return SystemError{SystemErrorCode::LinkCycle, link.source + ">" + link.target};
    links_.push_back(std::move(link));
    return std::nullopt;
}

} // namespace spn::core
