#pragma once

// NpcMemory: the first building block of the persistent living world (Docs/Proposals/0010, phase L1): stable entity ids, typed events, a BOUNDED
// memory per important NPC, hidden dispositions derived from that memory, deterministic time advancement and a versioned save format.
//
// What it is NOT: no AI, no nemesis/rivalry logic, no faction or schedule simulation, no dialogue, no WorldFacts integration. The store is a
// self-contained record keeper that those systems will read from and write events into. Per the proposal (section 6) high-frequency derived state
// does not go through WorldFacts, and the journal/compaction question is still open, so this store keeps NO global event journal: an event lives on
// only as memories of the NPCs involved, and every memory list is bounded.
//
// Design rules (all from Proposal 0010, section 4 and 7):
//   * Entities are records with stable ids, not actors. Player characters are entities too (the caller picks their ids; they need no NPC record).
//   * Events are the only way state changes; derived values (disposition) are computed from memories and can always be rebuilt.
//   * One world clock: events may not be older than the newest event / AdvanceTo time (monotonic), so replaying the same events gives the same store.
//   * A dead entity takes no actions (it cannot be the actor of a new event) and records no new memories; death is irreversible.
//   * Only plausible observations create memories: the actor and subject of an event, and the witnesses the caller lists - nobody else.
//   * Beliefs may be wrong: every memory has an accuracy that scales its influence (a rumour counts less than first-hand knowledge).
//   * Memory decays with age (half-life by persistence tier). Decay is a pure function of (memory, now), never mutated state, so it is composable
//     and frame-rate independent; AdvanceTo only removes memories whose strength fell below a threshold.
//
// ENGINEERING PROPOSAL (NightBuild02), not master canon. The tiers follow Proposal 0010 L2 (A/B/C); every number in NpcMemoryConfig is a PLACEHOLDER.

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Result.h>
#include <spn/core/StrongId.h>
#include <spn/core/Vec.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

struct EntityTag;
/// Stable id of an NPC or player character; 0 is the null id. Assigned by the caller (authored data / the spawner), never reused.
using EntityId = StrongId<EntityTag, std::uint64_t>;

/// Persistence tier (Docs/Game/Spielwelt-und-Untergrund.md section 13). A: story / hunter / close relationships (maximal memory). B: merchants,
/// neighbours, recurring contacts. C: ordinary residents (light). Tier D (crowds) is not stored individually and is not representable here.
enum class PersistenceTier : std::uint8_t { A = 0, B, C };
inline constexpr std::size_t kPersistenceTierCount = 3;
[[nodiscard]] std::string_view ToString(PersistenceTier tier) noexcept;

/// Appended, never reordered (the order is part of the saved format only through the text names, so reordering would be safe, but keep it stable).
enum class NpcEventKind : std::uint8_t { Encounter = 0, Helped, Rescued, Betrayed, Threatened, Injured, Witnessed, Killed };
inline constexpr std::size_t kNpcEventKindCount = 8;
[[nodiscard]] std::string_view ToString(NpcEventKind kind) noexcept;

/// What a memory contributes to a disposition, per unit of strength (placeholders). Dispositions are hidden values, never shown as bars.
struct KindEffect {
    double trust = 0.0;     ///< signed
    double fear = 0.0;      ///< >= 0
    double hostility = 0.0; ///< >= 0
    double importance = 0.0; ///< 0..1, how long it is worth remembering when the list is full
};

struct NpcMemoryConfig {
    std::array<std::size_t, kPersistenceTierCount> capacity = {64, 16, 4};         ///< memories per NPC
    std::array<double, kPersistenceTierCount> halfLifeSeconds = {864000.0, 172800.0, 21600.0}; ///< 10 days, 2 days, 6 hours of game time
    std::array<KindEffect, kNpcEventKindCount> effects = {{
        {0.02, 0.0, 0.0, 0.1},   // Encounter
        {0.5, 0.0, 0.0, 0.5},    // Helped
        {0.8, 0.0, 0.0, 0.8},    // Rescued
        {-0.7, 0.2, 0.3, 0.9},   // Betrayed
        {-0.2, 0.4, 0.2, 0.5},   // Threatened
        {-0.4, 0.5, 0.5, 0.8},   // Injured
        {0.0, 0.0, 0.0, 0.2},    // Witnessed
        {-0.9, 0.8, 0.9, 1.0},   // Killed
    }};
    double witnessWeight = 0.3;      ///< influence of an event the NPC only witnessed, compared with one that happened to it
    double forgetBelow = 0.02;       ///< AdvanceTo removes memories whose effective strength is below this
    std::size_t maxNpcs = 100'000;   ///< store limit against absurd input (not a design target)
};
[[nodiscard]] std::optional<std::string> Validate(const NpcMemoryConfig& config);

struct NpcIdentity {
    EntityId id;
    PersistenceTier tier = PersistenceTier::C;
    ContentId archetype; ///< optional: free data key (not interpreted here)
    std::string label;   ///< optional display / debug name
    friend bool operator==(const NpcIdentity&, const NpcIdentity&) = default;
};

/// One remembered event. `actor` did something to `subject`; either may be a player entity or an NPC.
struct MemoryRecord {
    std::uint64_t sequence = 0; ///< store-wide event number (monotonic); equal sequences in two NPCs are the same event
    double time = 0.0;          ///< game seconds
    NpcEventKind kind = NpcEventKind::Encounter;
    EntityId actor;
    EntityId subject;
    bool firstHand = true;      ///< seen / experienced vs heard about
    double accuracy = 1.0;      ///< 0..1: how well it is remembered (1 = exactly)
    double intensity = 1.0;     ///< 0..1: how severe / noticeable the event was
    Vec2 location;
    friend bool operator==(const MemoryRecord&, const MemoryRecord&) = default;
};

struct Witness {
    EntityId npc;
    bool firstHand = true;
    double accuracy = 1.0;
};

struct NpcEventInput {
    double time = 0.0;
    NpcEventKind kind = NpcEventKind::Encounter;
    EntityId actor;   ///< required
    EntityId subject; ///< optional (null = no direct counterpart)
    Vec2 location;
    double intensity = 1.0;
    std::vector<Witness> witnesses; ///< additional observers; the actor and the subject are recorded automatically if they are living NPCs
};

/// Hidden values derived from memory. Range: trust [-1, 1], fear and hostility [0, 1].
struct Disposition {
    double trust = 0.0;
    double fear = 0.0;
    double hostility = 0.0;
    std::size_t memories = 0; ///< how many memories contributed
};

enum class NpcMemoryErrorCode : std::uint8_t {
    InvalidId, DuplicateNpc, UnknownNpc, TooManyNpcs, InvalidTime, StaleEvent, InvalidValue, ActorDead, InvalidWitness,
    // persistence
    InvalidHeader, UnsupportedVersion, TruncatedData, TrailingData, MalformedLine, InvalidToken, CountMismatch, InvalidOrder, Io,
};
[[nodiscard]] std::string_view ToString(NpcMemoryErrorCode code) noexcept;
struct NpcMemoryError {
    NpcMemoryErrorCode code;
    std::size_t line = 0; ///< 1-based line in saved data, 0 = n/a
    friend bool operator==(const NpcMemoryError&, const NpcMemoryError&) = default;
};

class NpcMemoryStore {
public:
    explicit NpcMemoryStore(NpcMemoryConfig config = {});

    [[nodiscard]] const NpcMemoryConfig& Config() const noexcept { return config_; }

    // --- identities ---
    /// Registers an NPC. InvalidId for the null id, DuplicateNpc if known, TooManyNpcs at the limit.
    [[nodiscard]] std::optional<NpcMemoryError> AddNpc(NpcIdentity identity);
    [[nodiscard]] bool Knows(EntityId id) const { return npcs_.contains(id.Value()); }
    [[nodiscard]] const NpcIdentity* Find(EntityId id) const;
    [[nodiscard]] bool IsAlive(EntityId id) const; ///< true for unknown ids (player entities are never "dead" here)
    [[nodiscard]] std::size_t NpcCount() const noexcept { return npcs_.size(); }
    /// Ids ascending.
    [[nodiscard]] std::vector<EntityId> NpcIds() const;

    // --- events ---
    /// Records an event in the memory of the actor, the subject and the witnesses (living, known NPCs; unknown ids such as players are skipped; each
    /// NPC at most once). Rules: actor required and alive (ActorDead), time finite >= 0 and >= the newest time seen (StaleEvent), intensity and
    /// accuracies in [0, 1], witnesses must be known NPCs (InvalidWitness). A Killed event marks the subject dead (after its memory is recorded).
    /// Returns the event's sequence number.
    [[nodiscard]] Result<std::uint64_t, NpcMemoryError> RecordEvent(const NpcEventInput& event);

    // --- queries ---
    /// Memories of an NPC in sequence order (oldest first); nullptr if unknown.
    [[nodiscard]] const std::vector<MemoryRecord>* Memories(EntityId npc) const;
    /// Strength in (0, 1] of a memory `now`: importance-free freshness x accuracy x intensity, halved every half-life of the NPC's tier.
    [[nodiscard]] double EffectiveStrength(EntityId npc, const MemoryRecord& memory, double now) const;
    /// What `npc` remembers `towards` having done, summed over its memories and clamped. UnknownNpc if the NPC is not registered.
    [[nodiscard]] Result<Disposition, NpcMemoryError> DispositionOf(EntityId npc, EntityId towards, double now) const;

    // --- time ---
    /// Moves the world clock to `now` (never backwards) and removes memories whose effective strength fell below Config().forgetBelow. Idempotent
    /// and composable: AdvanceTo(a) then AdvanceTo(b) equals AdvanceTo(b). Returns how many memories were forgotten. Non-finite `now` does nothing.
    std::size_t AdvanceTo(double now);
    [[nodiscard]] double Clock() const noexcept { return clock_; }

    // --- persistence ---
    [[nodiscard]] std::string Serialize() const;
    /// Strict: canonical spelling only, ascending ids, monotonic sequences and times, consistent counts. Never partially succeeds.
    [[nodiscard]] static Result<NpcMemoryStore, NpcMemoryError> Deserialize(std::string_view text, NpcMemoryConfig config = {});
    [[nodiscard]] bool SameSavedState(const NpcMemoryStore& other) const;
    /// Number of successful mutations since creation / deserialization (not saved), like WorldFacts::Revision().
    [[nodiscard]] std::uint64_t Revision() const noexcept { return revision_; }
    [[nodiscard]] std::size_t TotalMemories() const;

private:
    struct Entry {
        NpcIdentity identity;
        bool alive = true;
        std::vector<MemoryRecord> memories;
    };
    void Remember(Entry& entry, const MemoryRecord& record);
    [[nodiscard]] double Retention(const Entry& entry, const MemoryRecord& m, double now) const;

    NpcMemoryConfig config_;
    std::map<std::uint64_t, Entry> npcs_;
    std::uint64_t nextSequence_ = 1;
    double clock_ = 0.0;
    std::uint64_t revision_ = 0;
};

} // namespace spn::core
