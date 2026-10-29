#pragma once

// Inventory domain: data-driven item definitions, unique item stacks, containers with capacity rules, equipment slots and
// ALL-OR-NOTHING transactions. Portable and deterministic: the same commands on the same state always give the same state and the
// same ids, so commands can be replicated (co-op), replayed and tested.
//
//   ItemCatalog       validated definitions (what an item IS: category, stack limit, optional weight/volume, equip slots).
//   InventoryState    containers and the stacks in them. Only InventoryCommands change it, through Apply (one command) or
//                     ApplyAll (a list that either fully succeeds or leaves the state untouched).
//
// Model
//   * Every ItemStack has a unique ItemInstanceId. Non-stackable items (maxStack == 1) are unique instances with their own `state`
//     fields; stackable items are fungible quantities (no per-item state), so stacks of one definition merge freely.
//   * A stack lives in exactly one container. Transfers keep the id when a whole stack moves into a container without merging.
//   * Equipping marks a stack of the same container as occupying a named slot. Equipped stacks cannot be removed, split or
//     transferred until they are unequipped.
//   * Capacity: optional limits on the number of stacks, total weight and total volume per container, optional category whitelist.
//     Weight/volume are opaque integers (grams / millilitres by convention); definitions default to 0 = no contribution. The Core
//     contains NO item data and NO balance values; limits are supplied by content.
//
// Not covered here (extension points, see Docs/M2B1/INVENTORY_FOUNDATION.md): durability, attachments, ammunition, trading,
// crafting, weapons. `ItemStack::state` (named integer/bool/string fields) is the extension point for per-instance data.
//
// ENGINEERING FOUNDATION (M2B1), not master canon. Master basis: "body inventory is physically limited; vehicles provide
// storage; HQ holds very large stocks" (accepted direction, no numbers). The older placeholder inventory counts in Pickup.h are
// untouched; migrating them is a separate, documented step.

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <spn/core/ContentId.h>
#include <spn/core/ObjectPersistence.h>
#include <spn/core/Result.h>
#include <spn/core/StrongId.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace spn::core {

struct ContainerTag;
struct ItemInstanceTag;
/// Id of a container (player body, vehicle trunk, NPC, house, ...). Chosen by the caller (stable, 0 = null).
using ContainerId = StrongId<ContainerTag, std::uint64_t>;
/// Id of one item stack/instance. Allocated by InventoryState (monotonic, never reused, saved).
using ItemInstanceId = StrongId<ItemInstanceTag, std::uint64_t>;

inline constexpr std::uint32_t kMaxStackLimit = 1'000'000; ///< sanity bound for definitions, not a design target
inline constexpr std::size_t kMaxItemStateFields = 16;
inline constexpr std::size_t kMaxContainers = 1'000'000;
inline constexpr std::size_t kMaxStacksPerContainer = 1'000'000;

struct ItemDefinition {
    ContentId id;       ///< required, unique in the catalog
    ContentId category; ///< required (data-driven: any valid ContentId)
    std::uint32_t maxStack = 1; ///< 1 = non-stackable; > 1 = stackable up to this many per stack
    std::uint64_t weightGrams = 0;   ///< per single item; 0 = no contribution
    std::uint64_t volumeMilliliters = 0;
    std::set<ContentId> equipSlots; ///< slots this item can occupy; non-empty requires maxStack == 1
    [[nodiscard]] bool Stackable() const noexcept { return maxStack > 1; }
    friend bool operator==(const ItemDefinition&, const ItemDefinition&) = default;
};

enum class InventoryErrorCode : std::uint8_t {
    // Catalog
    InvalidDefinition, DuplicateDefinition,
    // Commands
    InvalidId, UnknownDefinition, UnknownContainer, DuplicateContainer, UnknownItem, InvalidQuantity, NotStackable, StackMismatch,
    SameContainer, ContainerNotEmpty, CategoryNotAllowed, StackLimitReached, WeightLimitReached, VolumeLimitReached,
    ItemEquipped, UnknownSlot, NotEquippable, AlreadyEquipped, SlotOccupied, SlotEmpty, InvalidState, ArithmeticOverflow, LimitExceeded,
    // Persistence
    InvalidHeader, UnsupportedVersion, TruncatedData, TrailingData, MalformedLine, InvalidToken, CountMismatch, Inconsistent,
};
[[nodiscard]] std::string_view ToString(InventoryErrorCode code) noexcept;

struct InventoryError {
    InventoryErrorCode code;
    std::size_t commandIndex = 0; ///< ApplyAll: index of the failing command; persistence: 1-based line
    std::string detail;
    friend bool operator==(const InventoryError&, const InventoryError&) = default;
};

class ItemCatalog {
public:
    /// Validates: non-empty id and category, unique id, 1 <= maxStack <= kMaxStackLimit, equipSlots only for maxStack == 1.
    [[nodiscard]] std::optional<InventoryError> Add(ItemDefinition definition);
    [[nodiscard]] const ItemDefinition* Find(const ContentId& id) const;
    [[nodiscard]] std::size_t Size() const noexcept { return definitions_.size(); }
    [[nodiscard]] const std::map<ContentId, ItemDefinition>& All() const noexcept { return definitions_; }

private:
    std::map<ContentId, ItemDefinition> definitions_;
};

struct ContainerRules {
    ContentId kind;                          ///< data-driven label (base:container.body, ...); no built-in meaning
    std::optional<std::uint32_t> maxStacks;  ///< unset = unlimited
    std::optional<std::uint64_t> maxWeightGrams;
    std::optional<std::uint64_t> maxVolumeMilliliters;
    std::set<ContentId> allowedCategories;   ///< empty = every category
    std::set<ContentId> equipSlots;          ///< slots this container offers
    friend bool operator==(const ContainerRules&, const ContainerRules&) = default;
};

struct ItemStack {
    ItemInstanceId id;
    ContentId definition;
    std::uint32_t quantity = 1;
    FieldMap state; ///< named per-instance fields; only for non-stackable items (IsValidFieldName names)
    friend bool operator==(const ItemStack&, const ItemStack&) = default;
};

struct Container {
    ContainerId id;
    ContainerRules rules;
    std::map<ItemInstanceId, ItemStack> stacks; ///< ordered by id: deterministic iteration
    std::map<ContentId, ItemInstanceId> equipped; ///< slot -> stack
    friend bool operator==(const Container&, const Container&) = default;
};

// --- Commands -----------------------------------------------------------------------------------------------------------------

/// Creates an empty container. The id must be non-null and unused.
struct CreateContainer {
    ContainerId container;
    ContainerRules rules;
};
/// Removes an EMPTY container (no stacks).
struct DestroyContainer {
    ContainerId container;
};
/// Adds `quantity` items. Stackable: fills existing non-full stacks of the definition first, then creates new stacks. Non-stackable:
/// quantity must be 1; `state` becomes the instance state (must be empty for stackable items).
struct AddItem {
    ContainerId container;
    ContentId definition;
    std::uint32_t quantity = 1;
    FieldMap state;
};
/// Removes `quantity` items from a stack (the whole stack when quantity == stack size).
struct RemoveItem {
    ContainerId container;
    ItemInstanceId item;
    std::uint32_t quantity = 1;
};
/// Moves `quantity` of a stack to another container. Stackable items fill matching stacks there first; the remainder becomes a
/// new stack (keeping the id when the whole source stack was moved).
struct TransferItem {
    ContainerId from;
    ContainerId to;
    ItemInstanceId item;
    std::uint32_t quantity = 1;
};
/// Splits `quantity` off a stackable stack into a new stack of the same container (0 < quantity < stack size).
struct SplitStack {
    ContainerId container;
    ItemInstanceId item;
    std::uint32_t quantity = 1;
};
/// Moves `quantity` from `source` into `target` (same container, same definition, both stackable) up to the target's stack limit;
/// MergeStacks with quantity 0 means "as much as fits". The source disappears when empty. Fails if nothing can move.
struct MergeStacks {
    ContainerId container;
    ItemInstanceId source;
    ItemInstanceId target;
    std::uint32_t quantity = 0;
};
struct EquipItem {
    ContainerId container;
    ItemInstanceId item;
    ContentId slot;
};
struct UnequipSlot {
    ContainerId container;
    ContentId slot;
};

using InventoryCommand = std::variant<CreateContainer, DestroyContainer, AddItem, RemoveItem, TransferItem, SplitStack, MergeStacks, EquipItem, UnequipSlot>;

struct TransactionResult {
    std::uint64_t revision = 0;               ///< InventoryState::Revision() after the commit
    std::vector<ItemInstanceId> createdStacks; ///< ids allocated by the transaction, in allocation order
};

// --- State --------------------------------------------------------------------------------------------------------------------

class InventoryState {
public:
    /// The catalog must outlive the state (it is configuration, not state).
    explicit InventoryState(const ItemCatalog& catalog) : catalog_(&catalog) {}

    /// One command as a transaction.
    Result<TransactionResult, InventoryError> Apply(const InventoryCommand& command);
    /// All commands in order as ONE transaction: if any fails the state, the id counter and the revision stay exactly as they were
    /// and the error carries the failing command's index. An empty list is a successful no-op (revision unchanged).
    Result<TransactionResult, InventoryError> ApplyAll(const std::vector<InventoryCommand>& commands);

    [[nodiscard]] const Container* FindContainer(ContainerId id) const;
    [[nodiscard]] const ItemStack* FindStack(ContainerId container, ItemInstanceId item) const;
    /// Container that currently holds the stack, or null id.
    [[nodiscard]] ContainerId LocateStack(ItemInstanceId item) const;
    [[nodiscard]] const std::map<ContainerId, Container>& Containers() const noexcept { return containers_; }
    [[nodiscard]] std::uint64_t Revision() const noexcept { return revision_; }
    [[nodiscard]] ItemInstanceId NextInstanceId() const noexcept { return ItemInstanceId(nextInstance_); }

    /// Total quantity of a definition in a container (0 for an unknown container).
    [[nodiscard]] std::uint64_t CountOf(ContainerId container, const ContentId& definition) const;
    /// Total weight/volume of a container, saturating at UINT64_MAX (0 for an unknown container).
    [[nodiscard]] std::uint64_t TotalWeightGrams(ContainerId container) const;
    [[nodiscard]] std::uint64_t TotalVolumeMilliliters(ContainerId container) const;

    /// Checks every invariant (see Docs/M2B1/INVENTORY_FOUNDATION.md) and returns the first violation, for tests and loaders.
    [[nodiscard]] std::optional<InventoryError> CheckInvariants() const;

    /// Deterministic text "SPN-INVENTORY 1" (containers, rules, stacks, equipment, counters, revision).
    [[nodiscard]] std::string Serialize() const;
    /// Validates everything against the catalog and all invariants. Never partially succeeds.
    [[nodiscard]] static Result<InventoryState, InventoryError> Deserialize(std::string_view text, const ItemCatalog& catalog);

    friend bool operator==(const InventoryState& a, const InventoryState& b) {
        return a.catalog_ == b.catalog_ && a.nextInstance_ == b.nextInstance_ && a.revision_ == b.revision_ && a.containers_ == b.containers_;
    }

private:
    friend class InventoryTransaction;
    const ItemCatalog* catalog_;
    std::uint64_t nextInstance_ = 1;
    std::uint64_t revision_ = 0;
    std::map<ContainerId, Container> containers_;
};

} // namespace spn::core
