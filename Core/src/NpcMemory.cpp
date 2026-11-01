#include <algorithm>
#include <cmath>
#include <spn/core/NpcMemory.h>

#include "TextParsing.h"

namespace spn::core {

namespace {
constexpr std::string_view kMagic = "SPN-NPCMEM";
constexpr std::size_t kAbsoluteMaxMemoriesPerNpc = 4096; ///< parser limit; the configured capacity is applied when the next memory is added
constexpr std::size_t kMaxLabelBytes = 256;

constexpr std::array<PersistenceTier, kPersistenceTierCount> kAllTiers = {PersistenceTier::A, PersistenceTier::B, PersistenceTier::C};
constexpr std::array<NpcEventKind, kNpcEventKindCount> kAllKinds = {NpcEventKind::Encounter, NpcEventKind::Helped, NpcEventKind::Rescued, NpcEventKind::Betrayed,
                                                                      NpcEventKind::Threatened, NpcEventKind::Injured, NpcEventKind::Witnessed, NpcEventKind::Killed};

bool Unit(double v) { return std::isfinite(v) && v >= 0.0 && v <= 1.0; }
NpcMemoryError Err(NpcMemoryErrorCode c, std::size_t line = 0) { return NpcMemoryError{c, line}; }
} // namespace

std::string_view ToString(PersistenceTier tier) noexcept {
    switch (tier) {
    case PersistenceTier::A: return "A";
    case PersistenceTier::B: return "B";
    case PersistenceTier::C: return "C";
    }
    return "?";
}

std::string_view ToString(NpcEventKind kind) noexcept {
    switch (kind) {
    case NpcEventKind::Encounter: return "encounter";
    case NpcEventKind::Helped: return "helped";
    case NpcEventKind::Rescued: return "rescued";
    case NpcEventKind::Betrayed: return "betrayed";
    case NpcEventKind::Threatened: return "threatened";
    case NpcEventKind::Injured: return "injured";
    case NpcEventKind::Witnessed: return "witnessed";
    case NpcEventKind::Killed: return "killed";
    }
    return "?";
}

std::string_view ToString(NpcMemoryErrorCode code) noexcept {
    switch (code) {
    case NpcMemoryErrorCode::InvalidId: return "InvalidId";
    case NpcMemoryErrorCode::DuplicateNpc: return "DuplicateNpc";
    case NpcMemoryErrorCode::UnknownNpc: return "UnknownNpc";
    case NpcMemoryErrorCode::TooManyNpcs: return "TooManyNpcs";
    case NpcMemoryErrorCode::InvalidTime: return "InvalidTime";
    case NpcMemoryErrorCode::StaleEvent: return "StaleEvent";
    case NpcMemoryErrorCode::InvalidValue: return "InvalidValue";
    case NpcMemoryErrorCode::ActorDead: return "ActorDead";
    case NpcMemoryErrorCode::InvalidWitness: return "InvalidWitness";
    case NpcMemoryErrorCode::InvalidHeader: return "InvalidHeader";
    case NpcMemoryErrorCode::UnsupportedVersion: return "UnsupportedVersion";
    case NpcMemoryErrorCode::TruncatedData: return "TruncatedData";
    case NpcMemoryErrorCode::TrailingData: return "TrailingData";
    case NpcMemoryErrorCode::MalformedLine: return "MalformedLine";
    case NpcMemoryErrorCode::InvalidToken: return "InvalidToken";
    case NpcMemoryErrorCode::CountMismatch: return "CountMismatch";
    case NpcMemoryErrorCode::InvalidOrder: return "InvalidOrder";
    case NpcMemoryErrorCode::Io: return "Io";
    }
    return "Unknown";
}

std::optional<std::string> Validate(const NpcMemoryConfig& c) {
    for (std::size_t i = 0; i < kPersistenceTierCount; ++i) {
        if (c.capacity[i] == 0 || c.capacity[i] > kAbsoluteMaxMemoriesPerNpc) return "capacity must be in 1.." + std::to_string(kAbsoluteMaxMemoriesPerNpc);
        if (!std::isfinite(c.halfLifeSeconds[i]) || c.halfLifeSeconds[i] <= 0.0) return "half-life must be positive and finite";
    }
    for (const KindEffect& e : c.effects) {
        if (!std::isfinite(e.trust) || !std::isfinite(e.fear) || !std::isfinite(e.hostility) || !Unit(e.importance) || e.fear < 0.0 || e.hostility < 0.0 ||
            std::abs(e.trust) > 1.0 || e.fear > 1.0 || e.hostility > 1.0) {
            return "effect values out of range";
        }
    }
    if (!Unit(c.witnessWeight)) return "witness weight must be in 0..1";
    if (!std::isfinite(c.forgetBelow) || c.forgetBelow < 0.0 || c.forgetBelow >= 1.0) return "forgetBelow must be in 0..1";
    if (c.maxNpcs == 0) return "maxNpcs must be positive";
    return std::nullopt;
}

NpcMemoryStore::NpcMemoryStore(NpcMemoryConfig config) : config_(config) {}

std::optional<NpcMemoryError> NpcMemoryStore::AddNpc(NpcIdentity identity) {
    if (identity.id.IsNull()) return Err(NpcMemoryErrorCode::InvalidId);
    if (npcs_.contains(identity.id.Value())) return Err(NpcMemoryErrorCode::DuplicateNpc);
    if (npcs_.size() >= config_.maxNpcs) return Err(NpcMemoryErrorCode::TooManyNpcs);
    if (identity.label.size() > kMaxLabelBytes) return Err(NpcMemoryErrorCode::InvalidValue);
    const std::uint64_t key = identity.id.Value(); // read before the identity is moved from
    npcs_[key] = Entry{std::move(identity), true, {}};
    ++revision_;
    return std::nullopt;
}

const NpcIdentity* NpcMemoryStore::Find(EntityId id) const {
    const auto it = npcs_.find(id.Value());
    return it == npcs_.end() ? nullptr : &it->second.identity;
}

bool NpcMemoryStore::IsAlive(EntityId id) const {
    const auto it = npcs_.find(id.Value());
    return it == npcs_.end() || it->second.alive;
}

std::vector<EntityId> NpcMemoryStore::NpcIds() const {
    std::vector<EntityId> out;
    out.reserve(npcs_.size());
    for (const auto& [id, entry] : npcs_) out.push_back(EntityId(id));
    return out;
}

double NpcMemoryStore::EffectiveStrength(EntityId npc, const MemoryRecord& m, double now) const {
    const auto it = npcs_.find(npc.Value());
    const std::size_t tier = it == npcs_.end() ? kPersistenceTierCount - 1 : static_cast<std::size_t>(it->second.identity.tier);
    const double age = std::max(now - m.time, 0.0);
    const double freshness = std::exp2(-age / config_.halfLifeSeconds[tier]);
    return freshness * m.accuracy * m.intensity;
}

// Retention decides what is kept when the list is full: effective strength weighted by how worth remembering the kind is.
double NpcMemoryStore::Retention(const Entry& entry, const MemoryRecord& m, double now) const {
    return EffectiveStrength(entry.identity.id, m, now) * (0.25 + 0.75 * config_.effects[static_cast<std::size_t>(m.kind)].importance);
}

void NpcMemoryStore::Remember(Entry& entry, const MemoryRecord& record) {
    const std::size_t capacity = config_.capacity[static_cast<std::size_t>(entry.identity.tier)];
    entry.memories.push_back(record);
    while (entry.memories.size() > capacity) {
        // evict the least worth keeping; ties: the older sequence goes first (deterministic)
        std::size_t worst = 0;
        double worstScore = Retention(entry, entry.memories[0], clock_);
        for (std::size_t i = 1; i < entry.memories.size(); ++i) {
            const double score = Retention(entry, entry.memories[i], clock_);
            if (score < worstScore) {
                worstScore = score;
                worst = i;
            }
        }
        entry.memories.erase(entry.memories.begin() + static_cast<std::ptrdiff_t>(worst));
    }
}

Result<std::uint64_t, NpcMemoryError> NpcMemoryStore::RecordEvent(const NpcEventInput& e) {
    if (e.actor.IsNull()) return Err(NpcMemoryErrorCode::InvalidId);
    if (!std::isfinite(e.time) || e.time < 0.0) return Err(NpcMemoryErrorCode::InvalidTime);
    if (e.time < clock_) return Err(NpcMemoryErrorCode::StaleEvent);
    if (!Unit(e.intensity) || !std::isfinite(e.location.x) || !std::isfinite(e.location.y) || static_cast<std::size_t>(e.kind) >= kNpcEventKindCount) {
        return Err(NpcMemoryErrorCode::InvalidValue);
    }
    if (!IsAlive(e.actor)) return Err(NpcMemoryErrorCode::ActorDead);
    for (const Witness& w : e.witnesses) {
        if (!Knows(w.npc)) return Err(NpcMemoryErrorCode::InvalidWitness);
        if (!Unit(w.accuracy)) return Err(NpcMemoryErrorCode::InvalidValue);
    }

    // who remembers: actor, subject, witnesses - each once, in id order (deterministic), living NPCs only
    std::map<std::uint64_t, Witness> observers;
    for (const Witness& w : e.witnesses) observers.emplace(w.npc.Value(), w);
    for (const EntityId involved : {e.actor, e.subject}) {
        if (!involved.IsNull() && Knows(involved)) observers[involved.Value()] = Witness{involved, true, 1.0};
    }

    MemoryRecord record;
    record.sequence = nextSequence_;
    record.time = e.time;
    record.kind = e.kind;
    record.actor = e.actor;
    record.subject = e.subject;
    record.intensity = e.intensity;
    record.location = e.location;
    clock_ = e.time;
    for (const auto& [id, w] : observers) {
        Entry& entry = npcs_.at(id);
        if (!entry.alive) continue;
        record.firstHand = w.firstHand;
        record.accuracy = w.accuracy;
        Remember(entry, record);
    }
    if (e.kind == NpcEventKind::Killed && !e.subject.IsNull()) {
        const auto it = npcs_.find(e.subject.Value());
        if (it != npcs_.end()) it->second.alive = false;
    }
    ++nextSequence_;
    ++revision_;
    return record.sequence;
}

const std::vector<MemoryRecord>* NpcMemoryStore::Memories(EntityId npc) const {
    const auto it = npcs_.find(npc.Value());
    return it == npcs_.end() ? nullptr : &it->second.memories;
}

Result<Disposition, NpcMemoryError> NpcMemoryStore::DispositionOf(EntityId npc, EntityId towards, double now) const {
    const auto it = npcs_.find(npc.Value());
    if (it == npcs_.end()) return Err(NpcMemoryErrorCode::UnknownNpc);
    if (!std::isfinite(now)) return Err(NpcMemoryErrorCode::InvalidTime);
    Disposition d;
    for (const MemoryRecord& m : it->second.memories) {
        if (m.actor != towards) continue; // what the NPC remembers `towards` having DONE
        const KindEffect& effect = config_.effects[static_cast<std::size_t>(m.kind)];
        const bool happenedToIt = m.subject == npc;
        const double weight = (happenedToIt ? 1.0 : config_.witnessWeight) * EffectiveStrength(npc, m, now);
        d.trust += effect.trust * weight;
        d.fear += effect.fear * weight;
        d.hostility += effect.hostility * weight;
        ++d.memories;
    }
    d.trust = std::clamp(d.trust, -1.0, 1.0);
    d.fear = std::clamp(d.fear, 0.0, 1.0);
    d.hostility = std::clamp(d.hostility, 0.0, 1.0);
    return d;
}

std::size_t NpcMemoryStore::AdvanceTo(double now) {
    if (!std::isfinite(now) || now < clock_) return 0;
    clock_ = now;
    std::size_t forgotten = 0;
    for (auto& [id, entry] : npcs_) {
        const auto before = entry.memories.size();
        std::erase_if(entry.memories, [&](const MemoryRecord& m) { return EffectiveStrength(entry.identity.id, m, now) < config_.forgetBelow; });
        forgotten += before - entry.memories.size();
    }
    if (forgotten > 0) ++revision_;
    return forgotten;
}

std::size_t NpcMemoryStore::TotalMemories() const {
    std::size_t n = 0;
    for (const auto& [id, entry] : npcs_) n += entry.memories.size();
    return n;
}

bool NpcMemoryStore::SameSavedState(const NpcMemoryStore& other) const { return Serialize() == other.Serialize(); }

// --- persistence ---------------------------------------------------------------------------------------------------------------------

std::string NpcMemoryStore::Serialize() const {
    std::string out;
    out.append(kMagic).append(" 1\n");
    out.append("clock ").append(detail::FormatDouble(clock_)).append("\n");
    out.append("nextseq ").append(std::to_string(nextSequence_)).append("\n");
    out.append("npcs ").append(std::to_string(npcs_.size())).append("\n");
    for (const auto& [id, entry] : npcs_) {
        out.append("n ").append(std::to_string(id)).append(" ").append(ToString(entry.identity.tier)).append(entry.alive ? " 1 " : " 0 ");
        out.append(entry.identity.archetype.IsEmpty() ? "-" : entry.identity.archetype.Value()).append(" ").append(detail::PercentEncode(entry.identity.label)).append(" ");
        out.append(std::to_string(entry.memories.size())).append("\n");
        for (const MemoryRecord& m : entry.memories) {
            out.append("m ").append(std::to_string(m.sequence)).append(" ").append(detail::FormatDouble(m.time)).append(" ").append(ToString(m.kind)).append(" ");
            out.append(std::to_string(m.actor.Value())).append(" ").append(std::to_string(m.subject.Value())).append(m.firstHand ? " 1 " : " 0 ");
            out.append(detail::FormatDouble(m.accuracy)).append(" ").append(detail::FormatDouble(m.intensity)).append(" ");
            out.append(detail::FormatDouble(m.location.x)).append(" ").append(detail::FormatDouble(m.location.y)).append("\n");
        }
    }
    out.append("end\n");
    return out;
}

namespace {

struct LineReader {
    std::string_view rest;
    std::size_t line = 0;
    bool Next(std::string_view& out) {
        if (rest.empty()) return false;
        const std::size_t nl = rest.find('\n');
        if (nl == std::string_view::npos) return false; // every line must end with '\n'
        out = rest.substr(0, nl);
        rest.remove_prefix(nl + 1);
        ++line;
        return true;
    }
};

std::vector<std::string_view> Words(std::string_view line) {
    std::vector<std::string_view> w;
    std::size_t start = 0;
    while (true) {
        const std::size_t sp = line.find(' ', start);
        if (sp == std::string_view::npos) { w.push_back(line.substr(start)); return w; }
        w.push_back(line.substr(start, sp - start));
        start = sp + 1;
    }
}

std::optional<bool> Flag(std::string_view s) {
    if (s == "1") return true;
    if (s == "0") return false;
    return std::nullopt;
}

} // namespace

Result<NpcMemoryStore, NpcMemoryError> NpcMemoryStore::Deserialize(std::string_view text, NpcMemoryConfig config) {
    NpcMemoryStore store(config);
    LineReader in{text};
    std::string_view line;
    if (!in.Next(line)) return Err(NpcMemoryErrorCode::TruncatedData, in.line);
    {
        const auto w = Words(line);
        if (w.size() != 2 || w[0] != kMagic) return Err(NpcMemoryErrorCode::InvalidHeader, in.line);
        if (w[1] != "1") return Err(NpcMemoryErrorCode::UnsupportedVersion, in.line);
    }
    const auto keyed = [&](std::string_view key, std::string_view& value) -> std::optional<NpcMemoryError> {
        if (!in.Next(line)) return Err(NpcMemoryErrorCode::TruncatedData, in.line);
        const auto w = Words(line);
        if (w.size() != 2 || w[0] != key) return Err(NpcMemoryErrorCode::MalformedLine, in.line);
        value = w[1];
        return std::nullopt;
    };
    std::string_view value;
    if (const auto e = keyed("clock", value)) return *e;
    const auto clock = detail::ParseCanonicalDouble(value);
    if (!clock || *clock < 0.0) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
    if (const auto e = keyed("nextseq", value)) return *e;
    const auto nextSeq = detail::ParseCanonicalInteger<std::uint64_t>(value);
    if (!nextSeq || *nextSeq == 0) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
    if (const auto e = keyed("npcs", value)) return *e;
    const auto npcCount = detail::ParseCanonicalInteger<std::uint64_t>(value);
    if (!npcCount || *npcCount > store.config_.maxNpcs) return Err(NpcMemoryErrorCode::InvalidToken, in.line);

    store.clock_ = *clock;
    store.nextSequence_ = *nextSeq;
    std::uint64_t lastId = 0;
    std::uint64_t lastSeqAll = 0; // memories of different NPCs may repeat a sequence (one event), so only track the bound
    (void)lastSeqAll;
    for (std::uint64_t n = 0; n < *npcCount; ++n) {
        if (!in.Next(line)) return Err(NpcMemoryErrorCode::TruncatedData, in.line);
        const auto w = Words(line);
        if (w.size() != 7 || w[0] != "n") return Err(NpcMemoryErrorCode::MalformedLine, in.line);
        const auto id = detail::ParseCanonicalInteger<std::uint64_t>(w[1]);
        const auto tier = detail::ParseEnum(w[2], kAllTiers);
        const auto alive = Flag(w[3]);
        const auto count = detail::ParseCanonicalInteger<std::uint64_t>(w[6]);
        if (!id || *id == 0 || !tier || !alive || !count || *count > kAbsoluteMaxMemoriesPerNpc) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
        if (*id <= lastId) return Err(NpcMemoryErrorCode::InvalidOrder, in.line);
        lastId = *id;
        NpcIdentity identity;
        identity.id = EntityId(*id);
        identity.tier = *tier;
        if (w[4] != "-") {
            auto archetype = ContentId::Parse(w[4]);
            if (!archetype) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
            identity.archetype = *archetype;
        }
        auto label = detail::PercentDecode(w[5]);
        if (!label || label->size() > kMaxLabelBytes) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
        identity.label = std::move(*label);
        Entry entry{std::move(identity), *alive, {}};
        std::uint64_t lastSeq = 0;
        double lastTime = 0.0;
        for (std::uint64_t k = 0; k < *count; ++k) {
            if (!in.Next(line)) return Err(NpcMemoryErrorCode::TruncatedData, in.line);
            const auto m = Words(line);
            if (m.size() != 11 || m[0] != "m") return Err(NpcMemoryErrorCode::MalformedLine, in.line);
            const auto seq = detail::ParseCanonicalInteger<std::uint64_t>(m[1]);
            const auto time = detail::ParseCanonicalDouble(m[2]);
            const auto kind = detail::ParseEnum(m[3], kAllKinds);
            const auto actor = detail::ParseCanonicalInteger<std::uint64_t>(m[4]);
            const auto subject = detail::ParseCanonicalInteger<std::uint64_t>(m[5]);
            const auto firstHand = Flag(m[6]);
            const auto accuracy = detail::ParseCanonicalDouble(m[7]);
            const auto intensity = detail::ParseCanonicalDouble(m[8]);
            const auto x = detail::ParseCanonicalDouble(m[9]);
            const auto y = detail::ParseCanonicalDouble(m[10]);
            if (!seq || !time || !kind || !actor || !subject || !firstHand || !accuracy || !intensity || !x || !y) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
            if (*actor == 0 || !Unit(*accuracy) || !Unit(*intensity) || *time < 0.0 || *time > *clock || *seq == 0 || *seq >= *nextSeq) return Err(NpcMemoryErrorCode::InvalidToken, in.line);
            if (*seq <= lastSeq || *time < lastTime) return Err(NpcMemoryErrorCode::InvalidOrder, in.line);
            lastSeq = *seq;
            lastTime = *time;
            entry.memories.emplace_back(MemoryRecord{*seq, *time, *kind, EntityId(*actor), EntityId(*subject), *firstHand, *accuracy, *intensity, Vec2{*x, *y}});
        }
        store.npcs_.emplace(*id, std::move(entry));
    }
    if (!in.Next(line)) return Err(NpcMemoryErrorCode::TruncatedData, in.line);
    if (line != "end") return Err(NpcMemoryErrorCode::MalformedLine, in.line);
    if (!in.rest.empty()) return Err(NpcMemoryErrorCode::TrailingData, in.line + 1);
    return store;
}

} // namespace spn::core
