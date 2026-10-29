# M2B1 — Inventory foundation

Status: engineering notes/proposals, **not master canon**. Header `Inventory.h`; sources `Inventory.cpp`, `InventoryPersistence.cpp`; tests `Tests/Core/InventoryTests.cpp`. Master basis: only B1 ("body inventory is physically limited; vehicles provide storage; HQ holds very large stocks"), accepted direction without numbers — see `GAME_SYSTEMS_ARCHITECTURE.md` §2. **The Core contains no item data and no balance values.**

## 1. Implemented

| Requirement | Where |
|---|---|
| Item definitions (data-driven) | `ItemDefinition`, `ItemCatalog::Add` (validated) |
| Categories | `ContentId category` on definitions; `ContainerRules::allowedCategories` whitelist |
| Stackable / non-stackable | `maxStack` (1 = unique instance, >1 = stackable) |
| Unique item instances | `ItemStack` with unique `ItemInstanceId` and per-instance `state` (`FieldMap`, non-stackable only) |
| Containers | `Container`, `ContainerRules`, `CreateContainer` / `DestroyContainer` (empty only) |
| Transfers | `TransferItem` (keeps the id on a whole move; merges into matching stacks) |
| Stack split / merge | `SplitStack`, `MergeStacks` |
| Equip / unequip | `EquipItem`, `UnequipSlot`; slots are `ContentId`s offered by the container and allowed by the definition |
| Capacity constraints | optional max stacks, max weight (g), max volume (ml) per container; overflow-safe arithmetic |
| Transactional changes | `InventoryState::Apply` / `ApplyAll` — all or nothing |
| Validation of invalid operations | 33 `InventoryErrorCode`s, failing command index reported |
| Deterministic transitions | ordered containers, monotonic ids, no randomness, no clock |
| Persistence | `Serialize` / `Deserialize` (`SPN-INVENTORY 1`) |

Prepared for (data fields or extension points, **no mechanics**): weight/volume (`weightGrams`, `volumeMilliliters`, default 0 = no contribution), durability/attachments/ammunition (`ItemStack::state`), clothing (equip slots; the master's "storage utility" would be a container created per worn item — not implemented), medical supplies (categories), vehicle storage / housing / NPC inventories (any `ContainerId` with its own rules and `kind`), trading (a `TransferItem` between two containers is the primitive), multiplayer (commands are value types; `Revision()` increments per committed transaction).

Not implemented, deliberately: combat, weapons, complete crafting, durability decay, trading UI/prices, item data.

## 2. Public API (essentials)

```cpp
ItemCatalog catalog;  catalog.Add(ItemDefinition{...});                 // std::optional<InventoryError>
InventoryState inv(catalog);                                            // catalog must outlive the state
auto r = inv.Apply(AddItem{bag, coinId, 25, {}});                       // Result<TransactionResult, InventoryError>
auto t = inv.ApplyAll({CreateContainer{...}, AddItem{...}, TransferItem{...}});   // one transaction
// Commands: CreateContainer DestroyContainer AddItem RemoveItem TransferItem SplitStack MergeStacks EquipItem UnequipSlot
inv.FindContainer(id), FindStack(c, i), LocateStack(i), CountOf(c, def), TotalWeightGrams(c), TotalVolumeMilliliters(c)
inv.CheckInvariants();  inv.Serialize();  InventoryState::Deserialize(text, catalog);
```
`TransactionResult` returns the new revision and the ids allocated, in order.

## 3. Semantics worth knowing

- `AddItem` of a stackable fills existing non-full stacks of the definition (ascending id) first, then creates stacks of `maxStack`. Non-stackable: quantity must be 1.
- `TransferItem` of a whole stack keeps its id unless it merges away completely; a partial transfer allocates ids for newly created stacks.
- Equipped stacks cannot be removed, split or transferred until unequipped. Swapping needs `UnequipSlot` + `EquipItem` in one `ApplyAll`. Equippable definitions must be non-stackable.
- Capacity is checked after each mutating command on the touched container; a violation fails the transaction (`StackLimitReached`, `WeightLimitReached`, `VolumeLimitReached`, `ArithmeticOverflow` — never a wrapped total).
- A transaction works on copies of only the containers it touches and commits them together with the id counter and revision; on failure nothing (state, ids, revision) changes.

## 4. Invariants (`CheckInvariants`, checked after every step of a 4,000-step randomized test and on every load)

Container id = map key, non-null; stack id = map key, non-null, `< NextInstanceId()`, **unique across all containers**; definition exists; `1 ≤ quantity ≤ maxStack`; non-stackable has quantity 1, stackable has empty state; state names valid and ≤ 16 fields; categories allowed; every equipped slot is offered by the container, holds a stack of the same container whose definition allows that slot, and no stack occupies two slots; stack/weight/volume limits respected.

## 5. Persistence — `SPN-INVENTORY 1`

```
SPN-INVENTORY 1
next <nextInstanceId>     revision <n>     containers <N>
container <id> <kind|-> <maxStacks|-> <maxWeight|-> <maxVolume|-> <categories,|-> <slots,|-> <stackCount> <equippedCount>
stack <id> <definition> <quantity> <fieldCount>
field <name> <b|i|s> <value>
equip <slot> <stackId>
end
```
Canonical and strict (ascending ids, canonical integers, counts match, lists strictly ascending, unknown definitions rejected, full `CheckInvariants` on load, higher version rejected, never partial). The catalog is configuration and not saved; loading against a catalog that lacks a saved definition fails with `Inconsistent` (it does not silently drop items — an orphan-keeping policy like `ObjectPersistence`'s is a possible follow-up). Frozen golden text is in the tests.

Relation to existing code: `Pickup.h` still owns the placeholder `placeholder.inventory.<holder>.<item>.count` facts; the two are independent. Migration is a deliberate, documented save-compatibility step (`Docs/Decisions/0004`) and has not been done.

## 6. Test results

`InventoryTests.cpp`: catalog validation; add (fill/new stacks, unique state, invalid cases); remove; split; merge; transfer (id retention, merging, partial); capacity (stacks, weight, volume, exact fit, overflow); category whitelist; equip/unequip rules; container lifecycle; transactions (commit, rollback with index, staged visibility, create+destroy in one transaction, empty list, determinism of two replays); a 4,000-step randomized property test (invariants after every step, item conservation for non-creating commands, rejected commands leave the state equal to its snapshot; more than 300 accepted and more than 300 rejected commands, so both paths are exercised); persistence (round trip, empty, corrupt/inconsistent/newer data, stack in two containers, golden). Counts: `GAME_SYSTEMS_ARCHITECTURE.md` §8.

## 7. Performance (measured)

Release/GCC 16.2.1, Intel Core Ultra 7 256V, single run, no variance statistics, stack counts are of **one** container whose size is held constant during the measurement:

| Operation | 10 stacks | 100 | 1,000 | 10,000 |
|---|---|---|---|---|
| `AddItem` + `RemoveItem` (2 transactions) | 1.7 µs | 18.5 µs | 195 µs | 2.3 ms |
| 3-command transaction add/transfer/remove | 1.5 µs | 13.1 µs | 130 µs | 1.5 ms |
| failing 2-command transaction (rollback) | 1.1 µs | 10.7 µs | 120 µs | 1.5 ms |

`Serialize` 1,000 stacks 0.06 ms (29 KiB); `Deserialize` + invariants 0.58 ms. Memory per stack was not measured separately.

**Honest finding:** transaction cost grows **linearly with the size of the touched container** (≈ 130–150 ns per stack): the container is copied into the working set and capacity is recomputed. That is fine for body-sized inventories (tens of stacks, microseconds) and is **not acceptable** for containers with thousands of *distinct* stacks (e.g. a hoarded HQ with unique items). Stackable stock stays cheap because fungible items collapse into few stacks. The fix is known and contained (apply in place with an undo log, keep running weight/volume totals) and is the first recommended hardening step before such containers exist; it would not change the public API.

> **Update (M2B2):** the linear cost described here was removed. Transactions now run in place with an undo log and O(1) capacity checks on cached per-container totals; the measured before/after comparison, the preserved-behaviour evidence and the one remaining O(n) piece (searching a non-full stack when adding stackable items) are in `Docs/M2B2/INVENTORY_PERFORMANCE.md`. The tables above remain the M2B1 measurements of the old mechanism.

## 8. Known limitations

- Linear transaction cost (above). Single-threaded, no locking.
- `AddItem` of unique items one at a time (quantity must be 1).
- No automatic "best slot" choice, no drop-to-world, no ownership/legal status of items (evidence/law systems are separate designs), no item level state migration if a definition changes between versions.
- Weight/volume units (g / ml) are a convention of this API, not a master decision.

## 9. Unreal integration requirements

- A subsystem owning `InventoryState` and the `ItemCatalog`, loaded from data assets (item data does not exist yet and is a content/design task).
- Pickup/drop/world-item actors translating to `AddItem`/`RemoveItem`; placeholder pickups migrate here or stay separate (decision needed).
- UI for containers/equipment; replication of commands (co-op: host-authoritative, guests scoped to the host save — design pending).
- Save wiring (`GAME_SYSTEMS_ARCHITECTURE.md` §5). No Unreal test exists.

## 10. WorldForge integration

Not applicable, except that vehicle/housing containers need stable ids tied to authored placements.
