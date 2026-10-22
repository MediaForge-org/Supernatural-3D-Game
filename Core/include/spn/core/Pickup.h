#pragma once

// Test pickup: the second interactable kind and a minimal per-holder inventory count,
// both PLACEHOLDERS to prove reuse of the interaction system and WorldFacts
// (Docs/Proposals/0007). Not a design for items, inventory, capacity, weight or stacking:
// those are open decisions. No keys or locks.
//
// State lives only in WorldFacts:
//   placeholder.pickup.<id>.taken                bool  absent = authored default "taken" (else false)
//   placeholder.inventory.<holder>.<item>.count  int   absent = 0
// The placeholder. prefix marks the keys as non-final; renaming them later is a
// save-compatibility event (Docs/Decisions/0004), not a silent rename.
//
// Item ids use [a-z][a-z0-9_]* (no dots, at most 48 chars): holder ids may contain dots,
// so a dot in an item id would make "holder.item" ambiguous in the key.

#include <cstddef>
#include <cstdint>
#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

/// Prototype limit so the longest inventory key stays within kMaxFactKeyLength:
/// 22 ("placeholder.inventory.") + 24 holder + 1 + 48 item + 6 (".count") = 101.
inline constexpr std::size_t kMaxItemIdLength = 48;

/// 1..kMaxItemIdLength chars, first [a-z], rest [a-z0-9_].
[[nodiscard]] bool IsValidItemId(std::string_view item) noexcept;

/// placeholder:interactable.pickup
[[nodiscard]] const ContentId& TestPickupKind();
/// placeholder:feedback.pickup.gone, reported with Refused when Use hits a taken pickup.
[[nodiscard]] const ContentId& TestPickupGoneFeedback();

/// Names of the authored defaults in InteractionDefaults::values.
inline constexpr std::string_view kTestPickupItemState = "item";     // string, required
inline constexpr std::string_view kTestPickupAmountState = "amount"; // int >= 1, default 1
inline constexpr std::string_view kTestPickupTakenState = "taken";   // bool, default false

[[nodiscard]] std::string TestPickupTakenKey(std::string_view id);
[[nodiscard]] std::string InventoryCountKey(std::string_view holder, std::string_view item);

/// Handler: Use takes the pickup once (sets `.taken`, adds `amount` to the holder's count of
/// `item`), atomically. A taken pickup is Refused and writes nothing. A bad authored item
/// gives InvalidId; a wrong fact or default type (or amount < 1) gives StateCorrupt.
[[nodiscard]] InteractionHandler MakeTestPickupHandler();

/// Registers MakeTestPickupHandler() under TestPickupKind().
[[nodiscard]] std::optional<InteractionError> RegisterTestPickup(InteractionRegistry& registry);

/// Count of `item` held by `holder`; 0 if none. InvalidId for a bad holder or item,
/// StateCorrupt if the fact has another type.
[[nodiscard]] Result<std::int64_t, InteractionError>
InventoryCount(const WorldFacts& facts, std::string_view holder, std::string_view item);

struct InventoryEntry {
    std::string item;
    std::int64_t count = 0;
    friend bool operator==(const InventoryEntry&, const InventoryEntry&) = default;
};

/// All items of `holder` with a count fact, ordered by item id. Other holders (including
/// ones whose id extends this holder's id) never appear. StateCorrupt on a wrongly typed count.
[[nodiscard]] Result<std::vector<InventoryEntry>, InteractionError>
ListInventory(const WorldFacts& facts, std::string_view holder);

} // namespace spn::core
