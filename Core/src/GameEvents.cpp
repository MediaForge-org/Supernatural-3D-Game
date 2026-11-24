#include <spn/core/GameEvents.h>

#include <algorithm>
#include <utility>

namespace spn::core {

std::string_view ToString(GameEventType type) noexcept {
    switch (type) {
    case GameEventType::InteractionAttempted: return "interaction attempted";
    case GameEventType::InteractionSucceeded: return "interaction succeeded";
    case GameEventType::InteractionRejected: return "interaction rejected";
    case GameEventType::DoorStateChanged: return "door state changed";
    case GameEventType::DeviceStateChanged: return "device state changed";
    case GameEventType::LinkedMechanismActivated: return "linked mechanism activated";
    case GameEventType::PersistentStateRestored: return "persistent state restored";
    case GameEventType::NoiseEmitted: return "noise emitted";
    }
    return "unknown event";
}

std::string_view ToString(InteractStatus status) noexcept {
    switch (status) {
    case InteractStatus::Success: return "success";
    case InteractStatus::OutOfRange: return "out of range";
    case InteractStatus::InvalidTarget: return "invalid target";
    case InteractStatus::Disabled: return "disabled";
    case InteractStatus::Locked: return "locked";
    case InteractStatus::Blocked: return "blocked";
    case InteractStatus::AlreadyInState: return "already in state";
    case InteractStatus::InvalidWorldState: return "invalid world state";
    case InteractStatus::InvalidInput: return "invalid input";
    }
    return "unknown status";
}

GameEventBus::Handle GameEventBus::Subscribe(Listener listener, std::vector<GameEventType> only) {
    if (!listener) {
        return kInvalidHandle;
    }
    std::uint16_t mask = 0;
    for (const GameEventType type : only) {
        if (static_cast<std::size_t>(type) < kGameEventTypeCount) {
            mask = static_cast<std::uint16_t>(mask | (1u << static_cast<unsigned>(type)));
        }
    }
    if (only.empty()) {
        mask = static_cast<std::uint16_t>((1u << kGameEventTypeCount) - 1u);
    } else if (mask == 0) {
        return kInvalidHandle; // only out-of-range types were given: it could never fire
    }
    const Handle handle = nextHandle_++;
    entries_.push_back(Entry{handle, std::move(listener), mask, true});
    return handle;
}

bool GameEventBus::Unsubscribe(Handle handle) {
    for (Entry& entry : entries_) {
        if (entry.handle == handle && entry.active) {
            entry.active = false;
            if (!dispatching_) {
                std::erase_if(entries_, [](const Entry& e) { return !e.active; });
            }
            return true;
        }
    }
    return false;
}

std::size_t GameEventBus::ListenerCount() const noexcept {
    return static_cast<std::size_t>(std::count_if(entries_.begin(), entries_.end(), [](const Entry& e) { return e.active; }));
}

bool GameEventBus::Publish(GameEvent event) {
    if (static_cast<std::size_t>(event.type) >= kGameEventTypeCount) {
        return false; // a corrupt type could not be masked safely
    }
    if (dispatching_) {
        if (queue_.size() >= kMaxQueuedEvents) {
            ++dropped_;
            return false;
        }
        event.sequence = ++sequence_;
        queue_.push_back(std::move(event));
        return true;
    }
    event.sequence = ++sequence_;
    dispatching_ = true;
    queue_.push_back(std::move(event));
    while (!queue_.empty()) {
        const GameEvent current = std::move(queue_.front());
        queue_.pop_front();
        Dispatch(current);
    }
    dispatching_ = false;
    std::erase_if(entries_, [](const Entry& e) { return !e.active; });
    return true;
}

void GameEventBus::Dispatch(const GameEvent& event) {
    const std::uint16_t bit = static_cast<std::uint16_t>(1u << static_cast<unsigned>(event.type));
    // Index loop: listeners may subscribe (push_back) during the call; they join after this event. Entries are not erased while dispatching.
    const std::size_t count = entries_.size();
    for (std::size_t i = 0; i < count; ++i) {
        if (!entries_[i].active || (entries_[i].mask & bit) == 0) {
            continue;
        }
        // No copy: an entry is only deactivated while dispatching (its function stays alive until the dispatch ends) and a deque keeps the
        // reference valid when a listener subscribes.
        entries_[i].listener(event);
    }
}

} // namespace spn::core
