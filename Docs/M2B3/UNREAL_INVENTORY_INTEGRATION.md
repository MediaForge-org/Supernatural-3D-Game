# M2B3.3 — Unreal inventory subsystem, test catalog and save integration

> **Update M2B3.4 (2026-10-29):** the inventory subsystem gained a view-model, a catalog fingerprint (`slot.catalog`, a new optional section) and explicit migration/compatibility statuses; see `INVENTORY_VIEW_MODEL.md`. `LoadSlot` now takes `FSpnLoadOptions`, `ESpnSaveResult` has `CatalogMismatch`, and `FSpnSaveStatus` reports `InventoryOutcome`, `CatalogCheck` and both fingerprints.

Status words: **Implemented / Tested / Proposed / Unverified**. Date: 2026-10-29. Base `da1af14` plus the uncommitted M2B3.1–M2B3.3 changes. Nothing was committed or pushed; the Git stash, the installed engine, existing maps/assets, controller mappings and the Fab arrangement (plugin stays isolated) were not touched.
Master consultation (CLAUDE.md §11): **no new inventory rules, item data, save format or balance value was invented.** The Core inventory (M2B1/M2B2) and the `player.inventory` section (`SPN-INVENTORY 1`) are used as they are; the only master finding on inventories remains "body inventory is physically limited, vehicles provide storage" (`Docs/M2B1/GAME_SYSTEMS_ARCHITECTURE.md` §2, no numbers). Every item and number in the test catalog is arbitrary test data. Unreal-side policies that are decisions of ours are marked **Proposed**.

## 1. What was delivered

| Item | Where | State |
|---|---|---|
| `USpnInventorySubsystem` (UGameInstanceSubsystem) over the real Core `ItemCatalog` + `InventoryState` | `Source/SpnCore/Public/SpnInventorySubsystem.h`, `SpnInventoryTypes.h`, `Private/SpnInventorySubsystem.cpp`, `Private/SpnInventoryInternal.h` | Implemented, Tested |
| Temporary data-driven test catalog (6 items, 3 container presets), JSON | `Content/SpnTest/Inventory/TestItemCatalog.json` | Implemented, Tested |
| `player.inventory` in `USpnSaveSubsystem` (facts + inventory in one slot, multi-system safe load) | `Private/SpnSaveSubsystem.cpp`, `Public/SpnSaveSubsystem.h` | Implemented, Tested |
| Developer commands `Spn.Inv.*` (11) | `Source/SupernaturalProt/Spn/SpnInventoryConsoleCommands.cpp` | Implemented, run in a real game session |
| `WorldMapState::Revision()` + snapshot guard in `CaptureGameSave` | `Core/include/spn/core/WorldMap.h`, `Core/src/WorldMap.cpp`, `GameSave.cpp` | Implemented, Tested |
| Automation tests `SpnCore.Inventory.*` (17) | `Private/Tests/SpnInventorySubsystemAutomation.cpp` | Tested |
| Core test: map revision | `Tests/Core/MapTests.cpp` | Tested |

No inventory UI, no pickup/drop actors, no item content, no controller mapping, no map or minimap UI was created.

## 2. Unreal API

Header `SpnInventorySubsystem.h` (+ value types in `SpnInventoryTypes.h`). Core types appear only in private headers (Decision 0006). Game thread only.

```cpp
// catalog
FSpnInventoryStatus SetCatalog(const FSpnItemCatalogData&);          // validated by the Core; installs an EMPTY inventory; refused (CatalogLocked) while containers exist
FSpnInventoryStatus SetCatalogFromJson(const FString&);  FSpnInventoryStatus LoadCatalogFromFile(const FString& AbsolutePath);
static FString GetTestCatalogPath();   bool HasCatalog() const;   const FSpnItemCatalogData& GetCatalogData() const;
bool FindDefinition(Id, FSpnItemDefinitionData&) const;   bool FindContainerPreset(Id, FSpnContainerRulesData&) const;
void ResetInventory();   void ClearCatalog();

// transactions (all-or-nothing; one revision per call)
FSpnInventoryStatus Apply(const TArray<FSpnInventoryCommand>&);      // atomic batch
CreateContainer / CreateContainerFromPreset / DestroyContainer / AddItem / RemoveItem / TransferItem / SplitStack / MergeStacks / Equip / Unequip   // single-command conveniences
const FSpnInventoryStatus& GetLastFailure() const;

// read-only
uint64 GetRevision();  TArray<uint64> ListContainers();  bool GetContainer(Id, FSpnContainerView&);  uint64 CountOf(Container, DefId);
uint64 LocateStack(StackId);  uint64 GetNextStackId();  FString CheckInvariants();  FString Describe();

FSimpleMulticastDelegate OnInventoryChanged, OnInventoryReplaced;
```

* **Identifiers:** definitions, categories, equipment slots, container kinds and presets are the Core's `ContentId` strings (`namespace:path.segments`, lower case, ≤ 96 chars). Containers and stacks are `uint64` (0 = none). Stack ids are allocated by the Core, monotonic, **never reused**, saved.
* **`FSpnInventoryCommand`** is one command (factories `AddItem`, `TransferItem`, …); **`FSpnInventoryStatus`** = `{Error, FailedCommandIndex, Detail, Revision, CreatedStacks}`; `ESpnInventoryError` mirrors the Core's command errors one-to-one (`StackLimitReached`, `WeightLimitReached`, `VolumeLimitReached`, `CategoryNotAllowed`, `ItemEquipped`, `SlotOccupied`, …) plus `NoCatalog`, `InvalidArgument`, `CatalogLocked`, `InternalError`.
* **Views:** `FSpnContainerView` (rules, stacks by id, weight, volume), `FSpnStackView` (id, definition, quantity, typed state fields, equipped slot) are copies; there are no references into Core memory.
* **Capacity** (stack count, weight, volume, category whitelist), **equipment slots** (offered by the container, allowed by the definition), **atomic transfers** and **unique instance state** (typed bool/int/string fields, ≤ 16) are the Core's rules; nothing is re-implemented in Unreal.
* **Inactive until a catalog is loaded:** every mutation returns `NoCatalog`; the save subsystem ignores the inventory.
* **Lifetime:** `Initialize` does nothing but log; `Deinitialize` clears the delegates and destroys the state before the catalog it points to. Separate game instances (PIE, tests) have separate inventories.

### Developer commands (non-shipping builds)

`Spn.Inv.Setup` (loads the test catalog and creates containers 1 = body, 2 = trunk, 3 = small box), `Spn.Inv.Catalog`, `Spn.Inv.List`, `Spn.Inv.Create <id> <preset>`, `Spn.Inv.Add <container> <item> [qty]`, `Spn.Inv.Remove <container> <stack> [qty]`, `Spn.Inv.Transfer <from> <to> <stack> [qty]`, `Spn.Inv.Equip <container> <stack> <slot>`, `Spn.Inv.Unequip <container> <slot>`, `Spn.Inv.Last` (shows the last failed transaction), `Spn.Inv.Reset`. Ids may be abbreviated (`ration` = `spn_test:item.ration`). Output goes to the log and the on-screen debug line only; failures print the Core error, the failing command index and "nothing changed". Together with `Spn.SaveGame` / `Spn.LoadGame` this is the whole interface — no widget, no input binding.

**Real session evidence** (`logs/m2b33-session-smoke.log`, V2 map, `-game`): catalog installed ("TEMPORARY TEST DATA, not game content"); `Spn.Inv.Add 1 ration 7` → stacks 1 and 2 (5 + 2); `Add 1 flashlight` → stack 3; `Equip 1 3 hand` OK; `Equip 1 3 torso` → `NotEquippable`; `Transfer 1 3 2 4` → `InvalidQuantity`; `Add 3 coat` into the small box → `CategoryNotAllowed`; `Spn.Inv.Last` prints the last failure; every failure left the revision at 6.

## 3. Test item catalog (TEMPORARY TEST DATA)

`Content/SpnTest/Inventory/TestItemCatalog.json`, schema 1, `"test_data": true`, description in the file says so; every id is in the `spn_test:` namespace, the loader logs "TEMPORARY TEST DATA, not game content". None of this is Supernatural game content and none of the numbers is a balance value.

| Id | Purpose | Stack | Weight | Volume | Slots |
|---|---|---|---|---|---|
| `spn_test:item.ration` | stackable consumable | 5 | 200 g | 300 ml | – |
| `spn_test:item.bandage` | stackable consumable (second stackable) | 10 | 50 g | 80 ml | – |
| `spn_test:item.flashlight` | non-stackable, unique state, equippable | 1 | 400 g | 250 ml | `spn_test:slot.hand` |
| `spn_test:item.notebook` | non-stackable, unique state, not equippable | 1 | 150 g | 120 ml | – |
| `spn_test:item.coat` | non-stackable, equippable to a different slot, category not allowed in the small box | 1 | 1 200 g | 4 000 ml | `spn_test:slot.torso` |
| `spn_test:item.crate` | heavy and bulky, category not allowed in the small box | 1 | 15 000 g | 60 000 ml | – |

| Preset | Kind | Stacks | Weight | Volume | Categories | Slots |
|---|---|---|---|---|---|---|
| `spn_test:preset.body` | `spn_test:container.body` | 8 | 20 000 g | 30 000 ml | any | hand, torso |
| `spn_test:preset.small_box` | `spn_test:container.box` | 3 | 1 000 g | 1 500 ml | consumable, tool | none |
| `spn_test:preset.trunk` | `spn_test:container.trunk` | 20 | 80 000 g | 200 000 ml | any | none |

**Format choice (Proposed):** a JSON file read with Unreal's `Json` module (new private dependency of `SpnCore`) rather than a `UDataAsset`, because the repository authors content through text and Python tools, a `.uasset` cannot be written or reviewed in a diff, and the catalog must also be buildable from tests. The Unreal value types (`FSpnItemCatalogData`) are exactly what a `UDataAsset` wrapper would hold, so adding `USpnItemCatalogAsset` later only adds a loader. **Limitation:** `Content/**/*.json` is not a cooked asset; a packaged build must add `Content/SpnTest/Inventory` to "additional non-asset directories to copy" (or ship the data as an asset) — not done, not needed for editor and `-game` runs. The Core contains no item; tests that need exact limits (`weight_box` 100 g, `volume_box` 100 ml, `free`) install their own tiny catalog through `SetCatalog`.

## 4. Save integration

`USpnSaveSubsystem` (see `UNREAL_SAVE_INTEGRATION.md` for the base design) now treats the inventory as an integrated domain **while the inventory subsystem has a catalog**.

| Case | Save | Load |
|---|---|---|
| inventory active | writes `world.facts`, `player.inventory` (the Core's `SPN-INVENTORY 1`, version 1) and `slot.meta` (`facts_revision`, `inventory_revision`) in **one** container | parses both, swaps both, notifies |
| inventory inactive (no catalog) | writes `world.facts` + `slot.meta` as in M2B3.2 | slot has no inventory → loads as before |
| slot without `player.inventory`, inventory active | – | loads (WorldFacts-only saves stay compatible); the inventory is **reset to empty** and `bInventoryResetToEmpty` is set (the slot describes the whole state; reset is reported, never silent). **Proposed policy.** |
| slot with `player.inventory`, inventory inactive | refused (`SectionsNotIntegrated`), file untouched | refused, state untouched |
| `world.map` / `world.objects` present | refused | refused |
| unknown optional section | carried over byte for byte (`PreservedSections`) | ignored and kept |
| a **newer version** of a known section (`player.inventory`, `slot.meta`, …) | refused (`UnsupportedVersion`), file untouched | refused, no fallback, state untouched |

Why the last row exists: the container tolerates a newer *optional* section and hands it to "unread". Without a guard a `player.inventory` version 2 would have looked like a missing inventory on load (reset to empty = silent data loss) and would have been replaced on save (silent downgrade). Found while designing the missing-section case, fixed with `NewerKnownSection`, covered by a test.

### How the confirmed save policies are enforced

| Policy | Enforcement | Test |
|---|---|---|
| Never silently discard unknown or unintegrated sections | unknown optional: carried over; unintegrated / newer known: refuse | `UnknownAndUnsupportedSections`, `MissingInventorySection` |
| Refuse unsafe partial loading | any section the build cannot restore blocks the whole load | same |
| Never silently downgrade newer saves | newer container/section/required section and newer known optional sections: no fallback, no overwrite | `UnknownAndUnsupportedSections`, M2B3.2 `UnsupportedVersions` |
| Preserve legacy save functionality | `Spn.SaveWorld` / `Spn.LoadWorld` and `worldfacts.spnfacts` untouched | M2B3.2/legacy `SpnCore.WorldState.*` still pass |
| Never report success when publication fails | `Store.Save` result is returned; a failed temp write, read-back, rename → `Io` | M2B3.2 + Core fault-injection tests |
| Callbacks cannot cause inconsistent multi-system state during load | all swaps first, callbacks after; re-entrant save/load refused with `Busy` | `FactsAndInventoryTogether`, `CallbacksCannotReenterSaveOrLoad` |

### Cross-subsystem load consistency — what is actually guaranteed

Load is **parse-all, then swap-all, then notify**: (1) `ParseGameSave` builds new `WorldFacts` and a new `InventoryState` (validated against the live catalog, all invariants, `slot.meta` revisions); any failure aborts with nothing changed, and falls back to the previous generation as a whole slot; (2) the two swaps are moves of ordered maps and call nobody; (3) only then `OnWorldStateReplaced`, `OnInventoryReplaced`, `OnLoaded` run. Consequences, all tested: a failed load changes neither subsystem (byte-identical fingerprints of facts + inventory before and after, for corrupt payload, foreign revision, unknown definition, flipped bit); every listener sees the complete loaded state, whichever runs first; a listener cannot start a save or load in the middle (`Busy`).

**Claimed:** no code can observe a state where one domain is loaded and the other is not (single game thread, no callback or yield between the swaps); on disk the slot is one atomically published file. **Not claimed:** exception-atomicity of the swap itself (moves of `std::map` do not throw; not provoked), atomicity across a *crash* between the in-memory swaps (nothing is persisted there), or consistency with state outside these two domains (actors, positions, the map, the legacy `worldfacts.spnfacts`). A mutation test confirms the tests bite: with the inventory swap removed from `LoadSlot`, 5 of the 17 tests fail.

## 5. Transaction guarantees

* `Apply` translates **all** commands first; a malformed argument (zero id, bad id grammar, duplicate state field name, bad rules) is rejected with `InvalidArgument` and the index before the Core sees anything.
* The Core then runs the list as **one transaction**: on the first failing command containers, stacks, id counter (`GetNextStackId`) and revision are exactly as before; `FailedCommandIndex` names the command; no listener runs; the status carries the unchanged revision. Tested with batches whose first commands succeed (add, transfer, create container) and whose last command fails, comparing the full `Describe()` before and after, plus `CheckInvariants()`.
* An empty list is a successful no-op (no revision, no notification).
* `GetLastFailure()` keeps the most recent rejection (also for catalog errors) for debug output.
* Equipped items cannot be removed or transferred; swapping equipment is `[Unequip, Equip]` in one batch and rolls back as a pair.
* Exceptions: every Core call in the subsystem is wrapped; an exception becomes `InternalError` with nothing changed (not provoked in a test, see `UNREAL_SAVE_INTEGRATION.md` §6 for the investigation).

## 6. WorldMap revision (preparation for the map save integration)

`WorldMapState` had no revision, so the snapshot guard of `CaptureGameSave` could not cover it. Smallest safe change: a `std::uint64_t revision_` incremented by every **successful** mutation — `AddMarker`, `RemoveMarker` (and therefore `PruneExpired`), `MoveMarker`, `SetMarkerVisible` (only when the value changes), `Discover` that raises a state (and therefore `DiscoverPoisAround` / `DiscoverRegionsAt`). Failures, no-ops and queries do not change it. It is **not serialized** (`SPN-MAPSTATE 1` is byte-identical, a golden test still passes), is 0 after `Deserialize`, and is ignored by `SameSavedState`. Temporary markers count as mutations (conservative: the guard may flag a change that would not alter the saved bytes; it never misses one).

* Test `WorldMapState revision: every successful mutation increments it, failures and no-ops do not, it is not saved` (every mutator, the failure and no-op cases, queries, and the serialization/Deserialize/SameSavedState behaviour).
* `CaptureGameSave` now also compares the map revision before and after (like facts and inventory). **Not covered by a test:** none of the three guards can be triggered from outside (the capture takes const references and runs synchronously), so the guard code itself is verified by review and by the revision tests, not by a provoked mismatch.
* **For the future map integration:** (1) add `USpnMapSubsystem` owning the state and the definition; (2) pass `Sources.map`; (3) record the map revision in `slot.meta` — that is a **format change** (`slot.meta` grammar accepts only the two known keys), so it needs a decision and a version bump; until then a *loaded* slot cannot be checked for map-vs-facts consistency, only the capture is guarded; (4) provide `GameLoadContext.mapDefinition`; (5) lift the `world.map` refusal in the same change.

## 7. Test results

| Check | Command | Result |
|---|---|---|
| MSVC 19.51 Core, fresh Debug | `winbuild.cmd` (vcvars64 → CMake/Ninja `/W4 /WX` → CTest) | 80/80, 0 warnings, **482 of 482** CTest (10.6 s) |
| MSVC Release / clang-cl 22.1.3 Debug | same | **482 of 482** each (4.2 s / 8.2 s), 0 warnings |
| Unreal Editor build | `Build.bat SupernaturalProtEditor Win64 Development` | **Succeeded** (own mistakes on the way: missing `Engine/GameInstance.h` include, a self-referencing `TArray::Add` in a test — both fixed) |
| New Unreal tests `SpnCore.Inventory.*` | `Automation RunTests SpnCore.Inventory` | **17 of 17 pass** (`logs/m2b33-inventory-tests-3.log`) |
| V2 `Full -Regression` (`20261029-185147`) | `run_gameplay_qa.ps1 -Suite Full -Regression -NoVideo` | QA **PASS 166, FAIL 2, BLOCKED 0, NOT_IMPLEMENTED 3, NOT_TESTED 1**; automation **83 tests, 80 success, 3 failed**; all 17 inventory, 12 save-subsystem, 5 world-frame and 4 legacy world-state tests are among the successes |
| V3 Doors / Access / Traversal / MovementDemo | `-Suite … -Map …V3Interiors` | **89/89**, **15/15**, **22/22**, **27/27** |
| Python | `python3.15 -I -m unittest discover -s Tools -p "test_*.py"` | **219 tests OK** |
| QA inventory gate | `check_qa_inventory.py --check` | 23 systems, **0 problems** (it rejected the new class and 11 commands until they were listed — as designed) |
| Mutation check | inventory swap removed from `LoadSlot` | 5 tests fail (`FactsAndInventoryTogether`, `SaveLoadRoundTrip`, `RepeatedSaveLoadCycles`, `PreviousGenerationWithInventory`, `CallbacksCannotReenterSaveOrLoad`); source restored and re-verified |

The 17 inventory tests: `Initialize`, `CatalogRegistration`, `AddRemoveAndStacks`, `UniqueInstancesAndState`, `CapacityLimits` (stack, weight-only, volume-only, category, exact limits), `Equipment`, `TransfersAndRollback`, `InvalidOperations` (13 invalid cases, each leaves revision and state unchanged), `Notifications`, `SaveLoadRoundTrip`, `FactsAndInventoryTogether`, `CorruptInventoryPayload`, `MissingInventorySection`, `UnknownAndUnsupportedSections`, `PreviousGenerationWithInventory`, `RepeatedSaveLoadCycles` (20 cycles, both domains byte-identical each time), `CallbacksCannotReenterSaveOrLoad`. All use a fresh game instance and a temporary save root under `Saved/SpnTestTemp`; real saves are not touched.

Two expectations were corrected while writing the tests, after reading the Core: a unique item with quantity 2 returns `InvalidQuantity` (not `NotStackable`), and merging a stack with itself returns `InvalidId` (not `StackMismatch`). Both follow the documented Core behaviour; no assertion about existing code was weakened.

**Known failures, unchanged by this milestone** (M2A4 movement track): `Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run` (→ `SpnQA.Jump`), `SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps`. The flaky `MovementDemo.ledge_80_blocks_even_with_a_jump` passed in this run (27/27) but has failed in earlier runs on the same code (sideways slide along the wall face, open, see `UNREAL_SAVE_INTEGRATION.md` §8).

## 8. Performance considerations

Measured on this machine (Windows 11, Ryzen 7 3700X, MSVC 19.51 Release, `spn_core_bench_inventory_tx`, 7 repetitions, median of medians; `logs/m2b33-bench-inventory-tx-msvc-release.log`): a transaction costs **0.7–1.4 µs** at 10–10 000 stacks for add+remove, rollback and split+merge (the M2B2 undo-log design: no copy of the container), and **31 µs** for a transfer round trip with 10 000 stacks in one container (3.7 µs at 1 000). An `InventoryState` holds about **210 bytes per stack** of heap (449 at 10 stacks). A body inventory has tens of stacks.

What the Unreal layer adds, **not measured** (no Unreal-level benchmark was run): translation of each command (a few `ContentId` parses and small allocations), the `FString` ↔ UTF-8 conversions, and the by-value views of `GetContainer` / `Describe` (O(stacks) copies — fine for debug output and UI refresh on change, not for per-frame polling of huge containers). Guidance: read through `OnInventoryChanged` and cache a view; never poll `GetContainer` in Tick for large containers; batch related changes in one `Apply`. A save serializes the inventory in one pass (Core: 1 000 stacks ≈ 0.06 ms to serialize, 0.6 ms to parse and verify, measured on Fedora in M2B1) and the whole slot is written with a read-back verification, so saving belongs on a button/event, not on a timer. Nothing here runs per frame.

## 9. Known limitations and open decisions

* **No UI, no world items.** Pickups/drops, the placeholder pickup counts in `WorldFacts` (`Pickup.h`) and the new inventory are two independent systems; migrating the placeholders is a save-compatibility event (Decision 0004) that was not done.
* **No item content.** The six test items are placeholders; real definitions are a design task.
* **Catalog format and packaging:** JSON in `Content`, not yet a cooked asset, no hot-reload; the catalog cannot be replaced while containers exist; catalog identity is not stored in the save (a save written against catalog A and loaded with catalog B is rejected only if A's items are unknown to B — different numbers for the same item id are accepted silently). A catalog version/hash in `slot.meta` would close that gap (format change, undecided).
* **Reset-on-missing policy** (WorldFacts-only slot loaded while the inventory is active resets the inventory) and **refusal of newer known sections** are our proposals; confirm or change.
* **Map and objects are still not integrated;** slots holding them are refused. The map revision is prepared but not stored in the slot.
* **Authority/co-op:** all calls are local; there is no host-authority model, no replication of commands. Guests must never call `SaveSlot`/`Apply` once co-op exists.
* **Re-entrancy:** a listener may mutate the inventory in `OnInventoryChanged` (it is called after the transaction is complete); nothing prevents unbounded recursion by a careless listener.
* **No stack/container id allocation policy:** container ids are chosen by the caller (the debug commands use 1–3); an id scheme for body/vehicle/house containers is open (stable ids tied to authored placements, `INVENTORY_FOUNDATION.md` §10).
* Power-loss durability of the save is unverified (`SAVE_WINDOWS_VALIDATION.md`).

## 10. Next steps for the graphical inventory UI

1. **Decide** the data path (JSON → data asset; catalog version in `slot.meta`), the id scheme for containers, and whether the placeholder pickups migrate.
2. **View-model layer** (no widgets yet): a `USpnInventoryViewModel` that listens to `OnInventoryChanged` / `OnInventoryReplaced`, caches `FSpnContainerView`s and exposes sorted, filterable stack lists, equipment slot map, capacity bars (weight/volume/stack counts from the views) and the last failure as a localisable message key — unit-testable without UMG.
3. **Command surface:** UI intents map 1:1 to `FSpnInventoryCommand` batches (drag = `TransferItem`, split = `SplitStack`, equip swap = `[Unequip, Equip]`); the UI never mutates views.
4. **Failure presentation:** map `ESpnInventoryError` to message keys (the placeholder-feedback convention `placeholder:feedback.*`), show `FailedCommandIndex` only in debug builds.
5. **Input:** new bindings need a decision; the established DualSense/keyboard mapping (X jump, Circle dodge, Square interact/attack, Triangle heavy attack, E) must not be touched.
6. **Persistence of UI state** (open tab, sort order) is a separate, non-gameplay section if wanted.
7. Tests: automation tests on the view model with the temporary catalog, then a minimal debug UMG panel behind a console command, only after steps 1–4.
