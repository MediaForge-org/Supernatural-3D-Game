#include <algorithm>
#include <array>
#include <limits>
#include <utility>
#include <variant>
#include <vector>
#include <spn/core/Inventory.h>

namespace spn::core {

namespace {

InventoryError Err(InventoryErrorCode code, std::string detail = {}) { return InventoryError{code, 0, std::move(detail)}; }

constexpr std::array<std::string_view, 33> kErrorNames{
    "InvalidDefinition", "DuplicateDefinition", "InvalidId", "UnknownDefinition", "UnknownContainer", "DuplicateContainer", "UnknownItem",
    "InvalidQuantity", "NotStackable", "StackMismatch", "SameContainer", "ContainerNotEmpty", "CategoryNotAllowed", "StackLimitReached",
    "WeightLimitReached", "VolumeLimitReached", "ItemEquipped", "UnknownSlot", "NotEquippable", "AlreadyEquipped", "SlotOccupied", "SlotEmpty",
    "InvalidState", "ArithmeticOverflow", "LimitExceeded", "InvalidHeader", "UnsupportedVersion", "TruncatedData", "TrailingData",
    "MalformedLine", "InvalidToken", "CountMismatch", "Inconsistent"};
static_assert(static_cast<std::size_t>(InventoryErrorCode::Inconsistent) == 32, "update kErrorNames when InventoryErrorCode changes");

bool CheckedAdd(std::uint64_t a, std::uint64_t b, std::uint64_t& out) { return !__builtin_add_overflow(a, b, &out); }
bool CheckedMul(std::uint64_t a, std::uint64_t b, std::uint64_t& out) { return !__builtin_mul_overflow(a, b, &out); }

bool ValidState(const FieldMap& state) {
    if (state.size() > kMaxItemStateFields) {
        return false;
    }
    return std::all_of(state.begin(), state.end(), [](const auto& field) { return IsValidFieldName(field.first); });
}

bool CategoryAllowed(const Container& c, const ItemDefinition& d) {
    return c.rules.allowedCategories.empty() || c.rules.allowedCategories.contains(d.category);
}

bool IsEquipped(const Container& c, ItemInstanceId id) {
    return std::any_of(c.equipped.begin(), c.equipped.end(), [&](const auto& slot) { return slot.second == id; });
}

/// Sum over the stacks of `per(def) * quantity`; false on arithmetic overflow.
template <typename Per> bool Total(const Container& c, const ItemCatalog& catalog, Per per, std::uint64_t& out) {
    out = 0;
    for (const auto& [id, stack] : c.stacks) {
        const ItemDefinition* def = catalog.Find(stack.definition);
        std::uint64_t part = 0;
        if (def == nullptr || !CheckedMul(per(*def), stack.quantity, part) || !CheckedAdd(out, part, out)) {
            return false;
        }
    }
    return true;
}

/// Recomputes the sums from the stacks (O(stacks)). Only CheckInvariants uses it now: it is the independent oracle for the cached totals.
std::optional<InventoryError> CheckCapacityFromScratch(const Container& c, const ItemCatalog& catalog) {
    if (c.rules.maxStacks && c.stacks.size() > *c.rules.maxStacks) {
        return Err(InventoryErrorCode::StackLimitReached);
    }
    std::uint64_t weight = 0;
    if (!Total(c, catalog, [](const ItemDefinition& d) { return d.weightGrams; }, weight)) {
        return Err(InventoryErrorCode::ArithmeticOverflow, "weight");
    }
    if (c.rules.maxWeightGrams && weight > *c.rules.maxWeightGrams) {
        return Err(InventoryErrorCode::WeightLimitReached);
    }
    std::uint64_t volume = 0;
    if (!Total(c, catalog, [](const ItemDefinition& d) { return d.volumeMilliliters; }, volume)) {
        return Err(InventoryErrorCode::ArithmeticOverflow, "volume");
    }
    if (c.rules.maxVolumeMilliliters && volume > *c.rules.maxVolumeMilliliters) {
        return Err(InventoryErrorCode::VolumeLimitReached);
    }
    return std::nullopt;
}

} // namespace

std::string_view ToString(InventoryErrorCode code) noexcept {
    const auto index = static_cast<std::size_t>(code);
    return index < kErrorNames.size() ? kErrorNames[index] : "Unknown";
}

// --- Catalog ------------------------------------------------------------------------------------------------------------------

std::optional<InventoryError> ItemCatalog::Add(ItemDefinition definition) {
    if (definition.id.IsEmpty() || definition.category.IsEmpty() || definition.maxStack == 0 || definition.maxStack > kMaxStackLimit ||
        (!definition.equipSlots.empty() && definition.maxStack != 1) ||
        std::any_of(definition.equipSlots.begin(), definition.equipSlots.end(), [](const ContentId& s) { return s.IsEmpty(); })) {
        return Err(InventoryErrorCode::InvalidDefinition, definition.id.Value());
    }
    const ContentId key = definition.id;
    if (!definitions_.emplace(key, std::move(definition)).second) {
        return Err(InventoryErrorCode::DuplicateDefinition, key.Value());
    }
    return std::nullopt;
}

const ItemDefinition* ItemCatalog::Find(const ContentId& id) const {
    const auto it = definitions_.find(id);
    return it == definitions_.end() ? nullptr : &it->second;
}

// --- Transaction --------------------------------------------------------------------------------------------------------------

bool InventoryState::RecomputeTotals(Container& c, const ItemCatalog& catalog) {
    std::uint64_t weight = 0;
    std::uint64_t volume = 0;
    const bool ok = Total(c, catalog, [](const ItemDefinition& d) { return d.weightGrams; }, weight) &&
                    Total(c, catalog, [](const ItemDefinition& d) { return d.volumeMilliliters; }, volume);
    c.weightGrams = ok ? weight : 0;
    c.volumeMilliliters = ok ? volume : 0;
    return ok;
}

/// Applies commands IN PLACE to the live state and records how to undo every change (an undo log). A failed transaction is rolled back by
/// replaying the log backwards; a successful one just discards it. Cost: proportional to what the commands touch (O(log n) per touched
/// stack), not to the size of the containers; no container is ever copied.
///
/// Undo never allocates: erased and moved map entries are kept as extracted node handles and re-inserted, inserted entries are erased, and
/// changed numbers are written back. The Core does not use exceptions, so there is no unwinding path; an out-of-memory std::bad_alloc inside a
/// command would terminate the process like everywhere else in the Core.
class InventoryTransaction {
public:
    explicit InventoryTransaction(InventoryState& live) : live_(live), nextAtStart_(live.nextInstance_) {}

    std::optional<InventoryError> Run(const InventoryCommand& command) { return std::visit([this](const auto& c) { return Do(c); }, command); }

    void Commit() {
        ++live_.revision_;
        log_.clear();
    }

    /// Restores the state at construction: containers, stacks, equipment, derived totals, the instance-id counter. The revision was never changed.
    void Rollback() noexcept {
        while (!log_.empty()) {
            std::visit([this](auto& entry) { Undo(entry); }, log_.back());
            log_.pop_back();
        }
        live_.nextInstance_ = nextAtStart_;
    }

    std::vector<ItemInstanceId> created;

private:
    using StackNode = std::map<ItemInstanceId, ItemStack>::node_type;
    using EquipNode = std::map<ContentId, ItemInstanceId>::node_type;
    using ContainerNode = std::map<ContainerId, Container>::node_type;

    struct UQuantity { ContainerId container; ItemInstanceId stack; std::uint32_t old; };
    struct UInsertedStack { ContainerId container; ItemInstanceId stack; };
    struct UErasedStack { ContainerId container; StackNode node; };
    struct UMovedStack { ContainerId from; ContainerId to; ItemInstanceId stack; };
    struct UInsertedEquip { ContainerId container; ContentId slot; };
    struct UErasedEquip { ContainerId container; EquipNode node; };
    struct UTotals { ContainerId container; std::uint64_t weight; std::uint64_t volume; };
    struct UCreatedContainer { ContainerId container; };
    struct UDestroyedContainer { ContainerNode node; };
    using LogEntry = std::variant<UQuantity, UInsertedStack, UErasedStack, UMovedStack, UInsertedEquip, UErasedEquip, UTotals, UCreatedContainer,
                                  UDestroyedContainer>;

    // --- undo (noexcept: only erases, node re-insertions and number write-backs; every entry refers to state this transaction created or changed) ---
    Container* Live(ContainerId id) noexcept {
        const auto it = live_.containers_.find(id);
        return it == live_.containers_.end() ? nullptr : &it->second;
    }
    void Undo(UQuantity& e) noexcept { Live(e.container)->stacks.find(e.stack)->second.quantity = e.old; }
    void Undo(UInsertedStack& e) noexcept { Live(e.container)->stacks.erase(e.stack); }
    void Undo(UErasedStack& e) noexcept { Live(e.container)->stacks.insert(std::move(e.node)); }
    void Undo(UMovedStack& e) noexcept { Live(e.from)->stacks.insert(Live(e.to)->stacks.extract(e.stack)); }
    void Undo(UInsertedEquip& e) noexcept { Live(e.container)->equipped.erase(e.slot); }
    void Undo(UErasedEquip& e) noexcept { Live(e.container)->equipped.insert(std::move(e.node)); }
    void Undo(UTotals& e) noexcept {
        Container* c = Live(e.container);
        c->weightGrams = e.weight;
        c->volumeMilliliters = e.volume;
    }
    void Undo(UCreatedContainer& e) noexcept { live_.containers_.erase(e.container); }
    void Undo(UDestroyedContainer& e) noexcept { live_.containers_.insert(std::move(e.node)); }

    // --- logged mutations ---
    Container* Get(ContainerId id) { return Live(id); }

    ItemInstanceId Allocate() {
        const ItemInstanceId id(live_.nextInstance_++);
        created.push_back(id);
        return id;
    }
    const ItemCatalog& Catalog() const { return *live_.catalog_; }

    void SetQuantity(Container& c, ItemStack& stack, std::uint32_t quantity) {
        log_.emplace_back(UQuantity{c.id, stack.id, stack.quantity});
        stack.quantity = quantity;
    }
    void InsertStack(Container& c, ItemStack stack) {
        const ItemInstanceId id = stack.id;
        c.stacks.emplace(id, std::move(stack));
        log_.emplace_back(UInsertedStack{c.id, id});
    }
    void EraseStack(Container& c, std::map<ItemInstanceId, ItemStack>::iterator it) {
        log_.emplace_back(UErasedStack{c.id, c.stacks.extract(it)});
    }
    void MoveStack(Container& from, Container& to, std::map<ItemInstanceId, ItemStack>::iterator it) {
        const ItemInstanceId id = it->first;
        to.stacks.insert(from.stacks.extract(it));
        log_.emplace_back(UMovedStack{from.id, to.id, id});
    }

    /// Adds (sign +1) or removes (sign -1) `quantity` units of `d` to/from the derived totals. On uint64 overflow the totals are left unchanged and
    /// the overflow is remembered for CheckCapacity of THIS command (the transaction then fails and is rolled back, so no overflowed value is kept).
    void AdjustTotals(Container& c, const ItemDefinition& d, std::uint64_t quantity, bool add) {
        std::uint64_t weightPart = 0;
        std::uint64_t volumePart = 0;
        const bool weightOk = CheckedMul(d.weightGrams, quantity, weightPart);
        const bool volumeOk = CheckedMul(d.volumeMilliliters, quantity, volumePart);
        std::uint64_t weight = c.weightGrams;
        std::uint64_t volume = c.volumeMilliliters;
        if (add) {
            weightOverflow_ = weightOverflow_ || !weightOk || !CheckedAdd(weight, weightPart, weight);
            volumeOverflow_ = volumeOverflow_ || !volumeOk || !CheckedAdd(volume, volumePart, volume);
        } else {
            weight -= weightPart; // committed states never overflow, and a stack's contribution is part of the sum, so this cannot wrap
            volume -= volumePart;
        }
        log_.emplace_back(UTotals{c.id, c.weightGrams, c.volumeMilliliters});
        if (!weightOverflow_) c.weightGrams = weight;
        if (!volumeOverflow_) c.volumeMilliliters = volume;
    }

    /// Same checks and the same order as the M2B1 full recomputation (stack count, weight overflow, weight limit, volume overflow, volume limit),
    /// but on the derived totals: O(1).
    std::optional<InventoryError> CheckCapacity(const Container& c) {
        const bool weightOverflow = std::exchange(weightOverflow_, false);
        const bool volumeOverflow = std::exchange(volumeOverflow_, false);
        if (c.rules.maxStacks && c.stacks.size() > *c.rules.maxStacks) {
            return Err(InventoryErrorCode::StackLimitReached);
        }
        if (weightOverflow) {
            return Err(InventoryErrorCode::ArithmeticOverflow, "weight");
        }
        if (c.rules.maxWeightGrams && c.weightGrams > *c.rules.maxWeightGrams) {
            return Err(InventoryErrorCode::WeightLimitReached);
        }
        if (volumeOverflow) {
            return Err(InventoryErrorCode::ArithmeticOverflow, "volume");
        }
        if (c.rules.maxVolumeMilliliters && c.volumeMilliliters > *c.rules.maxVolumeMilliliters) {
            return Err(InventoryErrorCode::VolumeLimitReached);
        }
        return std::nullopt;
    }

    /// Adds `quantity` of a stackable definition: fill matching stacks first, then new stacks; `keepId` (non-null) names the id for
    /// the first newly created stack. Updates the derived totals by `quantity` once everything fitted.
    std::optional<InventoryError> PutStackable(Container& c, const ItemDefinition& d, std::uint32_t quantity, ItemInstanceId keepId) {
        std::uint32_t remaining = quantity;
        for (auto& [id, stack] : c.stacks) {
            if (remaining == 0) {
                break;
            }
            if (stack.definition == d.id && stack.quantity < d.maxStack) {
                const std::uint32_t put = std::min(remaining, d.maxStack - stack.quantity);
                SetQuantity(c, stack, stack.quantity + put);
                remaining -= put;
            }
        }
        const std::uint64_t needed = (static_cast<std::uint64_t>(remaining) + d.maxStack - 1) / d.maxStack;
        if (c.stacks.size() + needed > kMaxStacksPerContainer || (c.rules.maxStacks && c.stacks.size() + needed > *c.rules.maxStacks)) {
            return Err(c.stacks.size() + needed > kMaxStacksPerContainer ? InventoryErrorCode::LimitExceeded : InventoryErrorCode::StackLimitReached);
        }
        while (remaining > 0) {
            const std::uint32_t put = std::min(remaining, d.maxStack);
            ItemStack stack;
            stack.id = keepId.IsNull() ? Allocate() : keepId;
            keepId = ItemInstanceId();
            stack.definition = d.id;
            stack.quantity = put;
            InsertStack(c, std::move(stack));
            remaining -= put;
        }
        AdjustTotals(c, d, quantity, true);
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const CreateContainer& cmd) {
        if (cmd.container.IsNull()) {
            return Err(InventoryErrorCode::InvalidId, "container");
        }
        if (Get(cmd.container) != nullptr) {
            return Err(InventoryErrorCode::DuplicateContainer);
        }
        if (live_.containers_.size() >= kMaxContainers) {
            return Err(InventoryErrorCode::LimitExceeded, "containers");
        }
        Container c;
        c.id = cmd.container;
        c.rules = cmd.rules;
        live_.containers_.emplace(cmd.container, std::move(c));
        log_.emplace_back(UCreatedContainer{cmd.container});
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const DestroyContainer& cmd) {
        const auto it = live_.containers_.find(cmd.container);
        if (it == live_.containers_.end()) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        if (!it->second.stacks.empty()) {
            return Err(InventoryErrorCode::ContainerNotEmpty);
        }
        log_.emplace_back(UDestroyedContainer{live_.containers_.extract(it)});
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const AddItem& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        const ItemDefinition* d = Catalog().Find(cmd.definition);
        if (d == nullptr) {
            return Err(InventoryErrorCode::UnknownDefinition, cmd.definition.Value());
        }
        if (cmd.quantity == 0 || (!d->Stackable() && cmd.quantity != 1)) {
            return Err(InventoryErrorCode::InvalidQuantity);
        }
        if (!CategoryAllowed(*c, *d)) {
            return Err(InventoryErrorCode::CategoryNotAllowed);
        }
        if (!ValidState(cmd.state) || (d->Stackable() && !cmd.state.empty())) {
            return Err(InventoryErrorCode::InvalidState);
        }
        if (d->Stackable()) {
            if (auto e = PutStackable(*c, *d, cmd.quantity, ItemInstanceId())) {
                return e;
            }
        } else {
            if (c->stacks.size() >= kMaxStacksPerContainer) {
                return Err(InventoryErrorCode::LimitExceeded);
            }
            ItemStack stack;
            stack.id = Allocate();
            stack.definition = d->id;
            stack.state = cmd.state;
            InsertStack(*c, std::move(stack));
            AdjustTotals(*c, *d, 1, true);
        }
        return CheckCapacity(*c);
    }

    std::optional<InventoryError> Do(const RemoveItem& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        const auto it = c->stacks.find(cmd.item);
        if (it == c->stacks.end()) {
            return Err(InventoryErrorCode::UnknownItem);
        }
        if (cmd.quantity == 0 || cmd.quantity > it->second.quantity) {
            return Err(InventoryErrorCode::InvalidQuantity);
        }
        if (IsEquipped(*c, cmd.item)) {
            return Err(InventoryErrorCode::ItemEquipped);
        }
        const ItemDefinition* d = Catalog().Find(it->second.definition);
        if (d == nullptr) { // cannot happen in a valid state (CheckInvariants); refuse rather than corrupt the totals
            return Err(InventoryErrorCode::UnknownDefinition, it->second.definition.Value());
        }
        AdjustTotals(*c, *d, cmd.quantity, false);
        if (cmd.quantity == it->second.quantity) {
            EraseStack(*c, it);
        } else {
            SetQuantity(*c, it->second, it->second.quantity - cmd.quantity);
        }
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const TransferItem& cmd) {
        if (cmd.from == cmd.to) {
            return Err(InventoryErrorCode::SameContainer);
        }
        Container* from = Get(cmd.from);
        Container* to = Get(cmd.to);
        if (from == nullptr || to == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        const auto it = from->stacks.find(cmd.item);
        if (it == from->stacks.end()) {
            return Err(InventoryErrorCode::UnknownItem);
        }
        if (cmd.quantity == 0 || cmd.quantity > it->second.quantity) {
            return Err(InventoryErrorCode::InvalidQuantity);
        }
        if (IsEquipped(*from, cmd.item)) {
            return Err(InventoryErrorCode::ItemEquipped);
        }
        const ItemDefinition* d = Catalog().Find(it->second.definition);
        if (d == nullptr) {
            return Err(InventoryErrorCode::UnknownDefinition, it->second.definition.Value());
        }
        if (!CategoryAllowed(*to, *d)) {
            return Err(InventoryErrorCode::CategoryNotAllowed);
        }
        if (d->Stackable()) {
            const bool whole = cmd.quantity == it->second.quantity;
            const ItemInstanceId keep = whole ? it->first : ItemInstanceId();
            AdjustTotals(*from, *d, cmd.quantity, false);
            if (whole) {
                EraseStack(*from, it);
            } else {
                SetQuantity(*from, it->second, it->second.quantity - cmd.quantity);
            }
            // A partial move that does not merge entirely creates one new id per new stack (allocated inside PutStackable).
            if (auto e = PutStackable(*to, *d, cmd.quantity, keep)) {
                return e;
            }
        } else {
            if (to->stacks.size() >= kMaxStacksPerContainer) {
                return Err(InventoryErrorCode::LimitExceeded);
            }
            AdjustTotals(*from, *d, 1, false);
            MoveStack(*from, *to, it);
            AdjustTotals(*to, *d, 1, true);
        }
        return CheckCapacity(*to);
    }

    std::optional<InventoryError> Do(const SplitStack& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        const auto it = c->stacks.find(cmd.item);
        if (it == c->stacks.end()) {
            return Err(InventoryErrorCode::UnknownItem);
        }
        const ItemDefinition* d = Catalog().Find(it->second.definition);
        if (d == nullptr || !d->Stackable()) {
            return Err(InventoryErrorCode::NotStackable);
        }
        if (cmd.quantity == 0 || cmd.quantity >= it->second.quantity) {
            return Err(InventoryErrorCode::InvalidQuantity);
        }
        if (c->stacks.size() >= kMaxStacksPerContainer) {
            return Err(InventoryErrorCode::LimitExceeded);
        }
        SetQuantity(*c, it->second, it->second.quantity - cmd.quantity);
        ItemStack split;
        split.id = Allocate();
        split.definition = it->second.definition;
        split.quantity = cmd.quantity;
        InsertStack(*c, std::move(split)); // the units only move between stacks: the totals do not change
        return CheckCapacity(*c);
    }

    std::optional<InventoryError> Do(const MergeStacks& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        const auto src = c->stacks.find(cmd.source);
        const auto dst = c->stacks.find(cmd.target);
        if (src == c->stacks.end() || dst == c->stacks.end()) {
            return Err(InventoryErrorCode::UnknownItem);
        }
        if (cmd.source == cmd.target) {
            return Err(InventoryErrorCode::InvalidId, "source and target are the same stack");
        }
        if (src->second.definition != dst->second.definition) {
            return Err(InventoryErrorCode::StackMismatch);
        }
        const ItemDefinition* d = Catalog().Find(src->second.definition);
        if (d == nullptr || !d->Stackable()) {
            return Err(InventoryErrorCode::NotStackable);
        }
        if (cmd.quantity > src->second.quantity) {
            return Err(InventoryErrorCode::InvalidQuantity);
        }
        const std::uint32_t want = cmd.quantity == 0 ? src->second.quantity : cmd.quantity;
        const std::uint32_t move = std::min(want, d->maxStack - dst->second.quantity);
        if (move == 0) {
            return Err(InventoryErrorCode::StackLimitReached);
        }
        SetQuantity(*c, dst->second, dst->second.quantity + move);
        if (move == src->second.quantity) {
            EraseStack(*c, src);
        } else {
            SetQuantity(*c, src->second, src->second.quantity - move);
        }
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const EquipItem& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        const auto it = c->stacks.find(cmd.item);
        if (it == c->stacks.end()) {
            return Err(InventoryErrorCode::UnknownItem);
        }
        if (!c->rules.equipSlots.contains(cmd.slot)) {
            return Err(InventoryErrorCode::UnknownSlot, cmd.slot.Value());
        }
        const ItemDefinition* d = Catalog().Find(it->second.definition);
        if (d == nullptr || !d->equipSlots.contains(cmd.slot)) {
            return Err(InventoryErrorCode::NotEquippable);
        }
        if (IsEquipped(*c, cmd.item)) {
            return Err(InventoryErrorCode::AlreadyEquipped);
        }
        if (c->equipped.contains(cmd.slot)) {
            return Err(InventoryErrorCode::SlotOccupied, cmd.slot.Value());
        }
        c->equipped.emplace(cmd.slot, cmd.item);
        log_.emplace_back(UInsertedEquip{c->id, cmd.slot});
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const UnequipSlot& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        if (!c->rules.equipSlots.contains(cmd.slot)) {
            return Err(InventoryErrorCode::UnknownSlot, cmd.slot.Value());
        }
        auto node = c->equipped.extract(cmd.slot);
        if (node.empty()) {
            return Err(InventoryErrorCode::SlotEmpty, cmd.slot.Value());
        }
        log_.emplace_back(UErasedEquip{c->id, std::move(node)});
        return std::nullopt;
    }

    InventoryState& live_;
    const std::uint64_t nextAtStart_;
    std::vector<LogEntry> log_;
    bool weightOverflow_ = false;
    bool volumeOverflow_ = false;
};

Result<TransactionResult, InventoryError> InventoryState::ApplyAll(const std::vector<InventoryCommand>& commands) {
    if (commands.empty()) {
        return TransactionResult{revision_, {}};
    }
    InventoryTransaction tx(*this);
    for (std::size_t i = 0; i < commands.size(); ++i) {
        if (auto error = tx.Run(commands[i])) {
            tx.Rollback();
            error->commandIndex = i;
            return *error;
        }
    }
    tx.Commit();
    return TransactionResult{revision_, std::move(tx.created)};
}

Result<TransactionResult, InventoryError> InventoryState::Apply(const InventoryCommand& command) { return ApplyAll({command}); }

// --- Queries ------------------------------------------------------------------------------------------------------------------

const Container* InventoryState::FindContainer(ContainerId id) const {
    const auto it = containers_.find(id);
    return it == containers_.end() ? nullptr : &it->second;
}

const ItemStack* InventoryState::FindStack(ContainerId container, ItemInstanceId item) const {
    const Container* c = FindContainer(container);
    if (c == nullptr) {
        return nullptr;
    }
    const auto it = c->stacks.find(item);
    return it == c->stacks.end() ? nullptr : &it->second;
}

ContainerId InventoryState::LocateStack(ItemInstanceId item) const {
    for (const auto& [id, c] : containers_) {
        if (c.stacks.contains(item)) {
            return id;
        }
    }
    return ContainerId();
}

std::uint64_t InventoryState::CountOf(ContainerId container, const ContentId& definition) const {
    const Container* c = FindContainer(container);
    std::uint64_t total = 0;
    if (c != nullptr) {
        for (const auto& [id, stack] : c->stacks) {
            total += stack.definition == definition ? stack.quantity : 0U;
        }
    }
    return total;
}

std::uint64_t InventoryState::TotalWeightGrams(ContainerId container) const {
    const Container* c = FindContainer(container);
    return c != nullptr ? c->weightGrams : 0;
}

std::uint64_t InventoryState::TotalVolumeMilliliters(ContainerId container) const {
    const Container* c = FindContainer(container);
    return c != nullptr ? c->volumeMilliliters : 0;
}

std::optional<InventoryError> InventoryState::CheckInvariants() const {
    std::set<ItemInstanceId> seen;
    for (const auto& [cid, c] : containers_) {
        const auto bad = [&](std::string what) { return Err(InventoryErrorCode::Inconsistent, "container " + std::to_string(cid.Value()) + ": " + std::move(what)); };
        if (cid.IsNull() || c.id != cid) {
            return bad("id mismatch");
        }
        for (const auto& [sid, s] : c.stacks) {
            const ItemDefinition* d = catalog_->Find(s.definition);
            if (sid.IsNull() || s.id != sid || sid.Value() >= nextInstance_) {
                return bad("stack id " + std::to_string(sid.Value()));
            }
            if (!seen.insert(sid).second) {
                return bad("stack id used twice: " + std::to_string(sid.Value()));
            }
            if (d == nullptr) {
                return bad("unknown definition " + s.definition.Value());
            }
            if (s.quantity == 0 || s.quantity > d->maxStack || !ValidState(s.state) || (d->Stackable() && !s.state.empty())) {
                return bad("stack " + std::to_string(sid.Value()) + " quantity/state");
            }
            if (!CategoryAllowed(c, *d)) {
                return bad("category not allowed for stack " + std::to_string(sid.Value()));
            }
        }
        std::set<ItemInstanceId> equippedIds;
        for (const auto& [slot, sid] : c.equipped) {
            const auto it = c.stacks.find(sid);
            if (!c.rules.equipSlots.contains(slot) || it == c.stacks.end() || !equippedIds.insert(sid).second) {
                return bad("equipment slot " + slot.Value());
            }
            const ItemDefinition* d = catalog_->Find(it->second.definition);
            if (d == nullptr || !d->equipSlots.contains(slot)) {
                return bad("item cannot occupy slot " + slot.Value());
            }
        }
        if (auto e = CheckCapacityFromScratch(c, *catalog_)) {
            e->detail = "container " + std::to_string(cid.Value()) + " over capacity";
            return e;
        }
        std::uint64_t weight = 0;
        std::uint64_t volume = 0; // summed from the stacks, independent of the cached values
        if (!Total(c, *catalog_, [](const ItemDefinition& d) { return d.weightGrams; }, weight) ||
            !Total(c, *catalog_, [](const ItemDefinition& d) { return d.volumeMilliliters; }, volume) || weight != c.weightGrams ||
            volume != c.volumeMilliliters) {
            return bad("cached weight/volume totals differ from the stacks");
        }
    }
    return std::nullopt;
}

} // namespace spn::core
