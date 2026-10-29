#include <algorithm>
#include <array>
#include <limits>
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

std::optional<InventoryError> CheckCapacity(const Container& c, const ItemCatalog& catalog) {
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

/// Working copy: containers are copied into `staged_` on first touch, so a failed transaction is dropped without ever having
/// touched the live state, and the cost is proportional to the containers involved, not to the whole inventory.
class InventoryTransaction {
public:
    explicit InventoryTransaction(const InventoryState& base) : base_(base), next_(base.nextInstance_) {}

    std::optional<InventoryError> Run(const InventoryCommand& command) { return std::visit([this](const auto& c) { return Do(c); }, command); }

    void Commit(InventoryState& live) {
        for (auto& [id, container] : staged_) {
            if (container) {
                live.containers_[id] = std::move(*container);
            } else {
                live.containers_.erase(id);
            }
        }
        live.nextInstance_ = next_;
        ++live.revision_;
    }
    std::vector<ItemInstanceId> created;

private:
    Container* Get(ContainerId id) {
        const auto staged = staged_.find(id);
        if (staged != staged_.end()) {
            return staged->second ? &*staged->second : nullptr;
        }
        const auto live = base_.containers_.find(id);
        if (live == base_.containers_.end()) {
            return nullptr;
        }
        return &*staged_.emplace(id, live->second).first->second;
    }
    ItemInstanceId Allocate() {
        const ItemInstanceId id(next_++);
        created.push_back(id);
        return id;
    }
    const ItemCatalog& Catalog() const { return *base_.catalog_; }
    std::size_t ContainerCount() const {
        std::size_t n = base_.containers_.size();
        for (const auto& [id, c] : staged_) {
            const bool wasLive = base_.containers_.contains(id);
            if (c && !wasLive) {
                ++n;
            } else if (!c && wasLive) {
                --n;
            }
        }
        return n;
    }

    /// Adds `quantity` of a stackable definition: fill matching stacks first, then new stacks; `keepId` (non-null) names the id for
    /// the first newly created stack.
    std::optional<InventoryError> PutStackable(Container& c, const ItemDefinition& d, std::uint32_t quantity, ItemInstanceId keepId) {
        std::uint32_t remaining = quantity;
        for (auto& [id, stack] : c.stacks) {
            if (remaining == 0) {
                break;
            }
            if (stack.definition == d.id && stack.quantity < d.maxStack) {
                const std::uint32_t put = std::min(remaining, d.maxStack - stack.quantity);
                stack.quantity += put;
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
            c.stacks.emplace(stack.id, std::move(stack));
            remaining -= put;
        }
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const CreateContainer& cmd) {
        if (cmd.container.IsNull()) {
            return Err(InventoryErrorCode::InvalidId, "container");
        }
        if (Get(cmd.container) != nullptr) {
            return Err(InventoryErrorCode::DuplicateContainer);
        }
        if (ContainerCount() >= kMaxContainers) {
            return Err(InventoryErrorCode::LimitExceeded, "containers");
        }
        Container c;
        c.id = cmd.container;
        c.rules = cmd.rules;
        staged_[cmd.container] = std::move(c);
        return std::nullopt;
    }

    std::optional<InventoryError> Do(const DestroyContainer& cmd) {
        Container* c = Get(cmd.container);
        if (c == nullptr) {
            return Err(InventoryErrorCode::UnknownContainer);
        }
        if (!c->stacks.empty()) {
            return Err(InventoryErrorCode::ContainerNotEmpty);
        }
        staged_[cmd.container] = std::nullopt;
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
            c->stacks.emplace(stack.id, std::move(stack));
        }
        return CheckCapacity(*c, Catalog());
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
        it->second.quantity -= cmd.quantity;
        if (it->second.quantity == 0) {
            c->stacks.erase(it);
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
            if (whole) {
                from->stacks.erase(it);
            } else {
                it->second.quantity -= cmd.quantity;
            }
            // A partial move that does not merge entirely creates one new id per new stack (allocated inside PutStackable).
            if (auto e = PutStackable(*to, *d, cmd.quantity, keep)) {
                return e;
            }
        } else {
            if (to->stacks.size() >= kMaxStacksPerContainer) {
                return Err(InventoryErrorCode::LimitExceeded);
            }
            ItemStack moved = std::move(it->second);
            from->stacks.erase(it);
            to->stacks.emplace(moved.id, std::move(moved));
        }
        return CheckCapacity(*to, Catalog());
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
        it->second.quantity -= cmd.quantity;
        ItemStack split;
        split.id = Allocate();
        split.definition = it->second.definition;
        split.quantity = cmd.quantity;
        c->stacks.emplace(split.id, std::move(split));
        return CheckCapacity(*c, Catalog());
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
        dst->second.quantity += move;
        src->second.quantity -= move;
        if (src->second.quantity == 0) {
            c->stacks.erase(src);
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
        if (c->equipped.erase(cmd.slot) == 0) {
            return Err(InventoryErrorCode::SlotEmpty, cmd.slot.Value());
        }
        return std::nullopt;
    }

    const InventoryState& base_;
    std::uint64_t next_;
    std::map<ContainerId, std::optional<Container>> staged_;
};

Result<TransactionResult, InventoryError> InventoryState::ApplyAll(const std::vector<InventoryCommand>& commands) {
    if (commands.empty()) {
        return TransactionResult{revision_, {}};
    }
    InventoryTransaction tx(*this);
    for (std::size_t i = 0; i < commands.size(); ++i) {
        if (auto error = tx.Run(commands[i])) {
            error->commandIndex = i;
            return *error;
        }
    }
    tx.Commit(*this);
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
    std::uint64_t total = 0;
    return c != nullptr && Total(*c, *catalog_, [](const ItemDefinition& d) { return d.weightGrams; }, total) ? total : (c != nullptr ? std::numeric_limits<std::uint64_t>::max() : 0);
}

std::uint64_t InventoryState::TotalVolumeMilliliters(ContainerId container) const {
    const Container* c = FindContainer(container);
    std::uint64_t total = 0;
    return c != nullptr && Total(*c, *catalog_, [](const ItemDefinition& d) { return d.volumeMilliliters; }, total) ? total : (c != nullptr ? std::numeric_limits<std::uint64_t>::max() : 0);
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
        if (auto e = CheckCapacity(c, *catalog_)) {
            e->detail = "container " + std::to_string(cid.Value()) + " over capacity";
            return e;
        }
    }
    return std::nullopt;
}

} // namespace spn::core
