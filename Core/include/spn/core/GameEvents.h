#pragma once

// GameEventBus: a small, deterministic, instance-owned event dispatcher for gameplay events (no global singleton).
//
// Semantics (the contract the tests pin down):
//  * Listeners are called in subscription order, once per event, on the thread of Publish.
//  * Publish while an event is being dispatched does not recurse: the event is queued and delivered, in publish order, after the
//    current event's listeners have all run (the outermost Publish drains the queue). The queue is bounded; an overflowing event is
//    dropped and counted, never delivered half-way.
//  * Unsubscribe during dispatch takes effect immediately for later events AND for the remaining listeners of the current event.
//  * A listener subscribed during dispatch first sees the next event, not the current one.
//  * The bus never owns listener captures beyond the std::function; a listener that captures a dead object must unsubscribe first
//    (ScopedSubscription does that automatically).
//
// ENGINEERING FOUNDATION for the M2A3 interaction/persistence work, not master canon.

#include <cstddef>
#include <cstdint>
#include <functional>
#include <deque>
#include <spn/core/ContentId.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

enum class GameEventType : std::uint8_t {
    InteractionAttempted,
    InteractionSucceeded,
    InteractionRejected,
    DoorStateChanged,
    DeviceStateChanged,
    LinkedMechanismActivated,
    PersistentStateRestored,
    NoiseEmitted, ///< NightBuild05: a gameplay noise (position, range, strength) that NPC perception may hear
};

inline constexpr std::size_t kGameEventTypeCount = 8;

[[nodiscard]] std::string_view ToString(GameEventType type) noexcept;

/// Outcome classes of an interaction request (also carried by rejected events).
enum class InteractStatus : std::uint8_t {
    Success,
    OutOfRange,
    InvalidTarget,
    Disabled,
    Locked,
    Blocked,
    AlreadyInState,
    InvalidWorldState,
    InvalidInput,
};

[[nodiscard]] std::string_view ToString(InteractStatus status) noexcept;

/// Explicit payload. Not every field is meaningful for every type; unused ones stay at their defaults.
struct GameEvent {
    GameEventType type = GameEventType::InteractionAttempted;
    std::uint64_t sequence = 0;   ///< assigned by the bus, 1-based, gapless per bus
    std::string objectId;         ///< the object concerned (target / changed object / link target)
    ContentId kind;               ///< its kind, empty if unknown
    std::string actor;            ///< who acted; empty for state changes caused by links or restores
    InteractStatus status = InteractStatus::Success; ///< InteractionSucceeded / Rejected
    ContentId feedback;           ///< UI/audio reason, empty = none
    std::int64_t from = 0;        ///< state value before (bool as 0/1, lever position, press count)
    std::int64_t to = 0;          ///< state value after
    std::string sourceId;         ///< LinkedMechanismActivated: the object whose change triggered the link
    std::uint64_t count = 0;      ///< PersistentStateRestored: number of objects restored
    double noiseX = 0.0;          ///< NoiseEmitted: position in metres
    double noiseY = 0.0;
    double noiseRange = 0.0;      ///< NoiseEmitted: audible radius in metres (before attenuation)
    double noiseStrength = 0.0;   ///< NoiseEmitted: 0..1
};

class GameEventBus {
public:
    using Listener = std::function<void(const GameEvent&)>;
    using Handle = std::uint64_t;
    static constexpr Handle kInvalidHandle = 0;
    static constexpr std::size_t kMaxQueuedEvents = 256;

    /// Empty function -> kInvalidHandle. `only` empty = all types; otherwise just those types.
    [[nodiscard]] Handle Subscribe(Listener listener, std::vector<GameEventType> only = {});
    /// False for an unknown or already removed handle. Safe during dispatch.
    bool Unsubscribe(Handle handle);
    /// Delivers (or queues, during dispatch) the event. Returns false if the queue was full and the event was dropped.
    bool Publish(GameEvent event);

    [[nodiscard]] std::size_t ListenerCount() const noexcept;
    [[nodiscard]] std::uint64_t PublishedCount() const noexcept { return sequence_; }
    [[nodiscard]] std::uint64_t DroppedCount() const noexcept { return dropped_; }
    [[nodiscard]] bool IsDispatching() const noexcept { return dispatching_; }

private:
    struct Entry {
        Handle handle;
        Listener listener;
        std::uint16_t mask; // bit per GameEventType, 0x7f = all
        bool active;
    };
    void Dispatch(const GameEvent& event);

    std::deque<Entry> entries_; // deque: subscribing during dispatch keeps references to running entries valid
    std::deque<GameEvent> queue_;
    Handle nextHandle_ = 1;
    std::uint64_t sequence_ = 0;
    std::uint64_t dropped_ = 0;
    bool dispatching_ = false;
};

/// Unsubscribes on destruction, so a listener never outlives the object it captures. Movable, not copyable.
class ScopedSubscription {
public:
    ScopedSubscription() = default;
    ScopedSubscription(GameEventBus& bus, GameEventBus::Handle handle) noexcept : bus_(&bus), handle_(handle) {}
    ScopedSubscription(const ScopedSubscription&) = delete;
    ScopedSubscription& operator=(const ScopedSubscription&) = delete;
    ScopedSubscription(ScopedSubscription&& other) noexcept : bus_(other.bus_), handle_(other.handle_) { other.bus_ = nullptr; }
    ScopedSubscription& operator=(ScopedSubscription&& other) noexcept {
        if (this != &other) {
            Reset();
            bus_ = other.bus_;
            handle_ = other.handle_;
            other.bus_ = nullptr;
        }
        return *this;
    }
    ~ScopedSubscription() { Reset(); }
    void Reset() noexcept {
        if (bus_ != nullptr) {
            bus_->Unsubscribe(handle_);
            bus_ = nullptr;
        }
    }

private:
    GameEventBus* bus_ = nullptr;
    GameEventBus::Handle handle_ = GameEventBus::kInvalidHandle;
};

} // namespace spn::core
