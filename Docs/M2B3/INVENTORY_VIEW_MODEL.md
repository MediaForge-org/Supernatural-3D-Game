# M2B3.4 — Inventory view-model, catalog fingerprint and UI preparation

Status words: **Implemented / Tested / Proposed / Unverified**. Date: 2026-10-29. Base `da1af14` plus the uncommitted M2B3.1–M2B3.4 changes. Nothing was committed or pushed; the Git stash, the installed engine, existing maps/assets, controller mappings and the Fab arrangement (plugin stays isolated) were not touched.
Master consultation (CLAUDE.md §11): this milestone adds a presentation layer and one save-format extension; it introduces **no inventory rule, item, name, icon, balance value or UI design**. The only master finding on inventories remains "body inventory is physically limited, vehicles provide storage" (`Docs/M2B1/GAME_SYSTEMS_ARCHITECTURE.md` §2). Everything marked *Proposed* is an engineering proposal for the developer, not canon. No widget exists; nothing here binds an input.

## 1. What was delivered

| Item | Where | State |
|---|---|---|
| `USpnInventoryViewModel` — UMG-ready projection of the real inventory (panels, rows, capacity, equipment, selection, actions, navigation, messages) | `Source/SpnCore/Public/SpnInventoryViewModel.h`, `Private/SpnInventoryViewModel.cpp` | Implemented, Tested |
| Dry run `InventoryState::Probe` (Core) + `USpnInventorySubsystem::ProbeCommands` — learns what is possible without re-implementing rules | `Core/src/Inventory.cpp`, `SpnInventorySubsystem.cpp` | Implemented, Tested |
| Change notification with the touched containers (`OnInventoryChangedDetailed`), reset/replace notification | `SpnInventorySubsystem.h/.cpp` | Implemented, Tested |
| Catalog fingerprint `fp1:` (SHA-256 of a canonical text), `slot.catalog` save section | `Core/include/spn/core/CatalogFingerprint.h`, `Core/src/CatalogFingerprint.cpp`, `GameSave.h/.cpp` | Implemented, Tested |
| Compatibility policies in `USpnSaveSubsystem` (migration status, catalog check, explicit options, delegates, last status) | `SpnSaveSubsystem.h/.cpp` | Implemented, Tested |
| Optional presentation data asset `USpnItemPresentationSet` (icon, mesh, material, sounds, montage, names) | `SpnItemPresentation.h/.cpp` | Implemented (class), Tested (in memory); **no asset file exists** |
| Developer command `Spn.Inv.View` (prints what a screen would show) | `SpnInventoryConsoleCommands.cpp` | Implemented, run in a real session |
| Tests: Core 11 new cases; Unreal `SpnCore.InventoryViewModel.*` (12) and `SpnCore.InventoryCompat.*` (6) | `Tests/Core/CatalogFingerprintTests.cpp`, `GameSaveTests.cpp`, `Private/Tests/SpnInventoryViewModelAutomation.cpp` | Tested |

## 2. Architecture

```
 Core (portable)                    Unreal module SpnCore                                   UMG (later)
 ───────────────                    ─────────────────────                                   ───────────
 ItemCatalog ──fingerprint──┐
 InventoryState ─Probe()───┐│       USpnInventorySubsystem  ── OnInventoryChangedDetailed ──┐
 CaptureGameSave/Parse ───┐││         owns catalog + state   ── OnInventoryReplaced ────────┤
                          │││         USpnItemPresentationSet (optional, names/icons)       ▼
                          ││└──────►  ProbeCommands / Apply                          USpnInventoryViewModel ──► widgets
                          │└───────►  USpnSaveSubsystem  ── OnLoadFinished ─────────────►  (panels, rows, selection,
                          └────────►    slot.catalog fingerprint, migration status          actions, navigation, messages)
```

* **One source of truth.** Gameplay values live in the catalog (JSON today), item state in the Core `InventoryState`. The view-model holds *copies for display*, rebuilt from the subsystem; the presentation asset holds only look/sound/text. The fingerprint hashes the gameplay fields only, so editing a presentation asset cannot invalidate a save and no value can exist in two places.
* **No business logic in the view-model.** Whether an action is possible is decided by dry-running the real command in the Core (`ProbeCommands` → `InventoryState::Probe`, which uses the transaction's undo log: nothing is copied, revision and ids are untouched). Executing an action is an ordinary `Apply`. The only decisions the view-model makes itself are *which actions to list* (e.g. equip slots = container slots ∩ definition slots) — a menu-content decision, not a rule.
* **No fake data.** An inactive inventory yields no panels; an empty container yields an empty panel.
* **Ownership.** A view-model is a plain `UObject` created by the screen that needs it (`NewObject` + `Initialize(Inventory, Save)` + `SetBoundContainers(...)`); it unbinds in `Shutdown()`/`BeginDestroy`. Several can exist at once (HUD, inventory screen, trade screen) with different bindings.

## 3. Public interface (UMG contract)

All ids exposed to Blueprints are `int64` (Blueprints have no `uint64`; real ids are small positive numbers). Types are `USTRUCT(BlueprintType)` with read-only properties; functions are `UFUNCTION(BlueprintCallable/BlueprintPure)`; every notification exists as a dynamic (Blueprint) and a native (C++/test) delegate.

| Need | API |
|---|---|
| Bind to containers | `Initialize(Inventory, Save)`, `SetBoundContainers({Body, OtherContainer})` — first = primary |
| Grid / list | `GetPanels()`, `GetPanel(Id, Out)` → `FSpnContainerPanel { Rows[], TotalStackCount, StackMeter, WeightMeter, VolumeMeter, Slots[], Kind, Title }`; `FSpnItemRow { StackId, ContainerId, DefinitionId, Category, DisplayName, Description, SortGroup, Icon (soft), bHasPresentation, Quantity, MaxStack, bStackable, Unit/Total weight & volume, bEquippable, bEquipped, EquippedSlot, State[] }`; `SetSortMode(Default/Name/Category/TotalWeight/TotalVolume/Quantity, descending)`, `SetCategoryFilter(id)` |
| Item details | the selected row (`GetSelectedRow`) carries everything; `Description`/`Icon` come from the presentation set |
| Equipment panel | `Panel.Slots[]` = `FSpnEquipmentSlotView { SlotId, DisplayName, EquippedStackId, EquippedDefinitionId, EquippedName, bOccupied }`; equipment zone navigation (below) |
| Container transfer screen | `SetBoundContainers({Player, Other})`; actions `Transfer` per other bound container; selection follows a transferred stack |
| Capacity display | `FSpnCapacityMeter { Kind, Used, Max (<0 = unlimited), Fraction (0..1, −1 unlimited), bUnlimited, bFull }` for stacks, weight, volume |
| Selection | `SelectStack(Container, Stack)`, `ClearSelection()`, `GetSelectedStackId/ContainerId`, `OnSelectionChanged` |
| Actions | `GetAvailableActions()` → `FSpnActionOption { Action (Equip/Unequip/Transfer/Split/Merge/Discard), Label, TargetContainerId, TargetStackId, Slot, DefaultQuantity, MaxQuantity, bAvailable, UnavailableReason (ESpnInventoryError), UnavailableText }`; `ExecuteAction(FSpnActionRequest)`; `ExecuteDefaultAction()` (the confirm button: equip / unequip) |
| Errors and notices | `GetLastMessage()` / `OnMessage` → `FSpnInventoryMessage { Key (stable, localisable), Text (English fallback), Severity, Error, CommandIndex }`; `SpnMakeInventoryErrorMessage(Error)` for all 30 error codes (keys `spn.inventory.error.*`); save/load notices `spn.inventory.load.*`, `spn.save.*` |
| Refresh | `OnChanged`, `OnContainerChanged(id)`, `GetVersion()` (monotonic, poll-friendly) |

### Keyboard, mouse and controller

The view-model knows **no input device and binds no key** (the DualSense gameplay mapping — X jump, Circle dodge, Square interact/attack, Triangle heavy attack, keyboard E — is untouched). A widget translates its own input into these calls:

| Intent | Keyboard / D-pad / stick | Mouse | Call |
|---|---|---|---|
| move focus | arrows, D-pad, stick (after a dead-zone/repeat in the widget) | hover → `SelectStack` | `Navigate(Up/Down/Left/Right/Next/Previous/PageUp/PageDown/First/Last)` |
| switch container | Tab / shoulder buttons | click on the other panel | `Navigate(NextSection/PreviousSection)`, or `SelectStack` |
| confirm | Enter / bottom face button | double-click | `ExecuteDefaultAction()` or an option from `GetAvailableActions()` |
| open the action menu | context key | right-click | `GetAvailableActions()` |
| grid layout | – | – | `SetGridColumns(n)` (1 = list). In a grid Left/Right move within a row and Up/Down by a row, landing on the last item of a short last row; in a list Left/Right change the section |
| equipment slots | – | – | `SetNavigationZone(Equipment)`; Up/Down/Left/Right/Next/Previous move through the primary container's slots, an occupied slot selects its item |
| wrap-around | – | – | `SetWrapNavigation(true)` |

`Navigate` returns whether the selection changed (a widget can play a "blocked" sound on `false`). Selection survives refreshes: a selected stack that moved to another bound container is **followed**; a removed one moves the selection to the row that took its place; the last row removed clears it; a reset clears it; a selection filtered out jumps to the visible neighbour.

## 4. Update mechanism

* The subsystem broadcasts `OnInventoryChangedDetailed(FSpnInventoryChange{Revision, TouchedContainers, bReplacedAll})` after every successful non-empty transaction — touched = every container named by the commands (source *and* target of a transfer, created and destroyed containers) — and `OnInventoryReplaced` + a `bReplacedAll` change for a load, `ResetInventory`, `SetCatalog`, `ClearCatalog` and `SetPresentationSet`.
* The view-model rebuilds **only the bound panels that were touched** (`RebuildCount` proves it in the tests): a change in an unshown container costs one list lookup and no copy; a failed or empty transaction costs nothing (no notification at all).
* Within a touched panel the rebuild is O(stacks of that container): one `GetContainer` copy plus row conversion. There is no row-level diff (see §8 for the measured cost and the mitigation).
* Save/load: `USpnSaveSubsystem` swaps facts **and** inventory first and only then notifies, so the view-model always rebuilds from the complete loaded state; its `OnLoadFinished` delegate (success *or* failure) feeds notices to the player (`spn.inventory.load.migrated_empty`, `…catalog_unverified`, `…catalog_mismatch`, `…catalog_mismatch_accepted`, `spn.save.recovered_previous`, `spn.save.load_failed`). A failed load changes nothing, so the view is not refreshed — only the message is posted.

## 5. Save compatibility policies

| Rule (confirmed by the project owner) | Implementation | Test |
|---|---|---|
| Missing `player.inventory` in a valid older save = empty inventory **with an explicit migration status** | `ESpnInventoryLoadOutcome::MigratedEmptyNoSection` (always, when the inventory is active and the slot has none); `bInventoryResetToEmpty` additionally says live content was discarded; the view-model posts `spn.inventory.load.migrated_empty` | `LegacySavesAndMigrationStatus`, `CorruptIsNeverMissing` (contrast case) |
| Corrupt inventory data is never treated as missing | a present section is parsed by the Core; any failure is `Rejected` (and the previous generation is tried as a whole slot); the contrast test removes the section (→ migration) versus damaging it (→ rejection, state byte-identical) | `CorruptIsNeverMissing` (empty, garbage, damaged catalog section), M2B3.3 `CorruptInventoryPayload` |
| Unsupported newer sections are rejected | newer `slot.catalog`/`player.inventory`/`slot.meta`/`world.facts` version, an `fp2` fingerprint, an unknown required section → `UnsupportedVersion`, **no fallback to an older generation, no overwrite by a save** | `NewerAndUnintegratedSectionsBlock`, Core `catalog check: damaged or inconsistent …` |
| Loading an older save never overwrites its file | load never writes; verified by file bytes, the file's modification time and the absence of a `.prev` after loading a facts-only save and a fingerprint-less inventory save; only an explicit `SaveSlot` writes (and then adds the fingerprint) | `LegacySavesAndMigrationStatus` |
| Unknown optional sections are preserved | carried over byte for byte on save (`PreservedSections`); `slot.catalog` is rewritten exactly once, never duplicated | `NewerAndUnintegratedSectionsBlock` (last block), M2B3.3 `UnknownAndUnsupportedSections` |
| Unintegrated required sections block unsafe loading | unknown required section → `UnsupportedVersion`; `world.map` / `world.objects` → `SectionsNotIntegrated`; `player.inventory` without a catalog → `SectionsNotIntegrated` | same + M2B3.3 tests |

## 6. Catalog fingerprint

### Format (version 1)

```
canonical text   SPN-CATALOG-FP 1\n
                 def <id> <category> <maxStack> <weightGrams> <volumeMilliliters> <slot,slot,...|->\n   (one line per definition, ascending id; slots ascending)
fingerprint      "fp1:" + SHA-256(canonical text) as 64 lowercase hex digits
```

* **Gameplay-relevant only:** id, category, max stack, weight, volume, equipment slots. **Not hashed:** names, descriptions, icons, meshes, materials, sounds, montages (they live in the presentation asset and never reach the Core), container presets (a saved container stores its own rules), the JSON formatting, key order, whitespace, the description text and the `test_data` flag. The input is the parsed, validated `ItemCatalog` — formatting cannot matter by construction.
* **Platform stable:** integers and ASCII only; no floating point, locale, endianness or path dependence. SHA-256 is implemented in the Core (no OS crypto), checked against the FIPS 180-4 / NIST vectors (`""`, `"abc"`, the 448-bit message, one million `a`) and padding boundaries on MSVC and clang-cl. The frozen fingerprint of a three-item test catalog is asserted in a Core test and the frozen value of the real temporary test catalog (`fp1:483b2b138550f106f0aa2208737fa82ed0952debedb1cb6ed1b73be0d0c1a452`) in an Unreal test; **both were recomputed independently with .NET's SHA-256** over the canonical text (the second from the JSON with a PowerShell script). Fedora/Linux was not run in this milestone (**Unverified** there, but nothing in the code is platform specific).
* **Sensitivity (tested):** weight, volume, stack limit, category, equipment slot (replaced or removed), item id, definition added or removed — each gives a different fingerprint, pairwise distinct; a digit shifted between neighbouring fields cannot collide (`1 20` vs `12 0`).
* **Independence (tested):** other whitespace, other key order, other item order, other presets, another description, `test_data` flag, a presentation set — same fingerprint.

### Where it is stored (backward-compatible extension)

A **new optional section `slot.catalog`** (version 1): `catalog_fingerprint fp1:<hex>\ninventory_revision <n>\n`, written next to `player.inventory` whenever the inventory is saved. It is *not* a new key in `slot.meta`, because `slot.meta` has a strict grammar that M2B3.3 builds reject, whereas **an unknown optional section is ignored and carried over by every older build** (tested: an older schema validates the slot and lists `slot.catalog` as unread). The `inventory_revision` binds the fingerprint to the inventory it describes: if an older build re-saves the inventory and carries the old `slot.catalog` along, the revision no longer matches and the load treats it as *stale = legacy* instead of trusting it.

### Load policy

| Slot | Result |
|---|---|
| inventory + matching fingerprint | `CatalogCheck::Match`, loaded |
| inventory without `slot.catalog` (saved by M2B3.3) — **legacy policy** | loaded after the Core validated it against the current catalog (unknown items, over-full stacks, broken invariants are rejected); `LegacyNoFingerprint`, flagged and reported to the player as "could not be verified"; the next explicit save adds the fingerprint |
| inventory + fingerprint of another inventory revision | `StaleFingerprint`, handled like legacy |
| inventory + **different** fingerprint | **refused**: `CatalogMismatch`, both fingerprints in the detail, live state untouched (byte-identical facts and inventory), **no fallback to an older generation and never a reset**; message `spn.inventory.load.catalog_mismatch` |
| … and the caller explicitly sets `FSpnLoadOptions::bAcceptCatalogMismatch` | loaded as `MismatchAccepted` (warning message) **only if the inventory still validates against the current catalog** — same content, new definitions (e.g. a new weight); an inventory that no longer fits is rejected as usual |
| malformed `slot.catalog`, or one without an inventory | `Rejected` / inconsistent — never read as "no fingerprint" |
| newer `slot.catalog` version or an `fp2` fingerprint | `UnsupportedVersion`; a save refuses to overwrite it too |

The opt-in is the only way past a mismatch; the game decides when to offer it (e.g. a confirmation dialog). It is not a migration.

### How future catalog migrations will work (design only — **no migration is implemented**)

1. A migration is a pure Core function `old fingerprint → new fingerprint` operating on the parsed `InventoryState` text (rename an item id, split a definition, convert state fields), registered in a table keyed by the *pair* of fingerprints (or by an explicit catalog version once the catalog carries one).
2. On a mismatch the loader looks up a chain from the stored fingerprint to the current one. No chain → the load stays refused (today's behaviour). A chain → the inventory text is migrated *in memory*, validated against the current catalog and swapped in like any load; the status reports `Migrated` with the chain; the file on disk is not touched until an explicit save.
3. Every migration needs a frozen "before" slot, a frozen "after" expectation and a test that the loader rejects when one step is missing — **no migration will be added without those test cases**, as required.
4. The fingerprint algorithm itself is versioned by its prefix: `fp2:` would be introduced beside `fp1:`; this build rejects `fp2` as newer (tested).

## 7. Presentation data-asset layer

`USpnItemPresentationSet : UPrimaryDataAsset` (primary asset type `SpnItemPresentation`) holds `FSpnItemPresentationEntry { ItemId, DisplayName, Description, SortGroup, Icon, Mesh, Material, PickupSound, UseSound, UseMontage }`; every asset reference is a **soft** pointer (nothing loads until a widget asks). `USpnInventorySubsystem::SetPresentationSet` installs one; the view-model uses it for names, descriptions, icons and sort groups; meshes, materials, sounds and montages are carried for the later systems (world items, equip animations) and not used yet.

* **No duplicate gameplay values:** the entry has no weight, volume, stack, category or slot field, so there is nothing to keep in sync. `ValidatePresentation()` reports *orphans* (entry for an id the catalog lacks), *duplicates* (first wins) and *items without presentation* (fallback: id as name, no icon, `bHasPresentation=false`). The catalog always wins.
* **No production assets needed:** without a set (or an entry) the UI gets the id as text — the same code path; the tests run with an in-memory set and with none.
* **Packaged builds — Unverified (no packaged build was made):** (a) the presentation set is a primary data asset: add the `AssetManagerSettings` entry shown in `SpnItemPresentation.h` so the Asset Manager cooks the sets and what they reference; (b) soft references to textures/meshes/sounds are cooked through that set or via Directories to Always Cook; (c) the **catalog JSON is not a cooked asset**: either add `/Game/SpnTest/Inventory` (later the production catalog folder) to *Additional Non-Asset Directories to Copy*, or — recommended before shipping — convert the JSON to a cooked `UDataAsset` at build time with a small tool, keeping the JSON as the portable source and the asset as a generated artifact (never edited by hand), and have `LoadCatalogFromFile` fall back to it. That conversion is not done.
* **Why not make the data asset the gameplay source:** the Core must stay engine-independent and testable without Unreal; the JSON is readable by Fedora tests, WorldForge and mod tooling; a `.uasset` is not diffable.

## 8. Tests and results

| Check | Command | Result |
|---|---|---|
| MSVC 19.51 Core, fresh Debug | CMake/Ninja `/W4 /WX`, CTest (see `WINDOWS_CORE_VALIDATION.md` for the wrapper) | 82/82 steps, 0 warnings, **493 of 493** (10.9 s) |
| MSVC Release / clang-cl 22.1.3 Debug | same | **493 of 493** each (4.3 s / 8.3 s) |
| Unreal Editor build | `Build.bat SupernaturalProtEditor Win64 Development` | **Succeeded** |
| `SpnCore.*` automation (all 57) | `Automation RunTests SpnCore` | **57 of 57 pass**: 18 new (12 `InventoryViewModel`, 6 `InventoryCompat`) + 17 `Inventory` + 12 `SaveSubsystem` + 5 `WorldFrame` + 5 legacy |
| V2 / V3 regression, Python, QA gate | see §9 | see §9 |

New Core cases (11): SHA-256 vectors; canonical text + frozen fingerprint; order independence + sensitivity to nine changes; syntax helpers; `Probe` equals `ApplyAll` without changing anything (8 command lists, ids not consumed); `slot.catalog` capture/verify; legacy; stale; mismatch refused / accepted / still validated / item missing; eight malformed or inconsistent catalog sections + newer versions; backward compatibility with the older schema.

New Unreal tests (what each proves):

| Test | Proves |
|---|---|
| `ReflectsTheRealInventory` | inactive → no panels; empty containers → empty panels; rows equal the Core's stacks (quantity, unit/total weight and volume, equipment, state); meters and slots; title = kind id; reset removes panels |
| `PresentationIsOptionalAndSeparate` | names/description/sort group/soft icon from the set, fallback without; orphan/duplicate/missing report; fingerprint and weights unchanged; saves load without a set |
| `MutationsUpdateThePanels` | after add, add unique, split, merge, equip, unequip, transfer, remove, batch: the panel equals the Core; the version advances; failed transaction → no rebuild, no version change |
| `OnlyTouchedBoundContainersAreRebuilt` | an unshown container costs no rebuild and no notification; transfers rebuild only the bound side; reset refreshes |
| `SelectionSurvivesRefreshes` | followed across containers, neighbour after removal, cleared when empty/reset |
| `ActionsComeFromTheCoreNotFromTheUi` | options probed against the real inventory (revision and failure memory untouched), unavailable options carry the Core's reason, execute equips/splits, refused requests are reported with the Core's error |
| `ConfirmEquipsAndUnequips` | the confirm action |
| `NavigationForKeyboardMouseAndController` | list, grid (short last row), sections, paging, wrap, empty selection, equipment zone |
| `SortAndFilter` | six sort modes with stable tie-break, descending, category filter, capacity unaffected by the filter |
| `ErrorMessages` | every `ESpnInventoryError` has a unique key and a text |
| `SaveLoadRefreshesTheView` | load rebuilds the view, restores equipment and selection; failed load changes nothing and posts a message |
| `Performance` | see §10 |
| `CatalogFingerprint` | stable across whitespace/key/item/preset order, sensitive to six gameplay changes, insensitive to presets |
| `FingerprintInTheSave` | the section, its payload, `slot.meta` unchanged, `Match`, no section without an inventory |
| `LegacySavesAndMigrationStatus` | legacy loads flagged; facts-only save → explicit migration to empty; files untouched (bytes, timestamp, no `.prev`) |
| `CatalogMismatchNeverResetsSilently` | refusal, byte-identical state, no fallback, explicit accept keeps content with the new weights, accept does not skip validation |
| `CorruptIsNeverMissing` | damaged/empty inventory and damaged catalog sections are rejected; missing section is a migration |
| `NewerAndUnintegratedSectionsBlock` | newer versions and `fp2` refuse load and save, file untouched; map blocks; unknown optional preserved |

## 9. Regression

All runs 2026-10-29, Fab isolated, engine untouched, logs `Docs/M2B3/logs/m2b34-*`.

| Check | Command | Result |
|---|---|---|
| Real Unreal Editor build | `Build.bat SupernaturalProtEditor Win64 Development` | **Succeeded** (my own mistakes on the way, all fixed: `dynamic_cast` to an incomplete `UTexture2D` in a test, `[[nodiscard]]` ignored, dynamic delegates have no `AddLambda` → native counterparts added) |
| Core, MSVC 19.51 Debug / Release, clang-cl 22.1.3 Debug | CMake/Ninja `/W4 /WX` + CTest | **493 of 493** each, 0 warnings. clang-cl found a real defect GCC/Clang on Fedora would also flag: `-Wmissing-field-initializers` for the new `LoadedGame` fields (fixed) |
| `SpnCore.*` automation | `Automation RunTests SpnCore` | **57 of 57** (`m2b34-spncore-all-tests.log`) |
| V2 `Full -Regression` (`20261029-200534`) | `run_gameplay_qa.ps1 -Suite Full -Regression -NoVideo` | QA **PASS 166, FAIL 2, BLOCKED 0, NOT_IMPLEMENTED 3, NOT_TESTED 1**; automation **101 tests, 98 success, 3 failed** — all 57 `SpnCore.*` tests and the legacy `WorldState.*` tests are among the successes |
| V3 Doors / Access / Traversal | `-Suite … -Map …V3Interiors` | **89/89**, **15/15**, **22/22** |
| V3 MovementDemo | `-Suite MovementDemo` | **26 PASS, 1 FAIL**: the known flaky `ledge_80_blocks_even_with_a_jump` (passed in 5 of the 7 runs since the test hardening, on unchanged movement code) |
| Python | `python3.15 -I -m unittest discover -s Tools -p "test_*.py"` | **219 tests OK** |
| QA inventory gate | `check_qa_inventory.py --check` | 23 systems, **0 problems** (it demanded the three new classes, the new header and the new command until they were listed) |
| Real gameplay session | `-game` on the V2 map: `Spn.Inv.Setup`, adds, `Equip`, `Spn.Inv.View`, `Spn.SaveGame`, `Spn.Inv.Reset`, `Spn.LoadGame`, `Spn.Inv.View` (`m2b34-session-view.log`) | the view shows 3 panels with rows, meters (`weight 3000 / 20000 (15%)`), equipment, a selection and its actions with the Core's verdicts (`Move to box: NOT available: Too heavy for this container`); after save → reset → load the printed view is **identical** |

**Known movement failures, unchanged and unrelated (M2A4 track):** `Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run` (→ `SpnQA.Jump`), `SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps`; plus the flaky `MovementDemo.ledge_80_blocks_even_with_a_jump` (sideways slide along the wall face). They are tracked as separate issues; nothing in this milestone touches movement.

**Test changes that were necessary and are not weakenings:** the M2B3.3 test `FactsAndInventoryTogether` asserted that a combined slot has 3 sections; with the new `slot.catalog` it has 4 (the new section is the feature). A second behaviour change: `ResetInventory`/`ClearCatalog`/`SetCatalog` now broadcast `OnInventoryReplaced` (the view must refresh); the save subsystem uses an internal reset and notifies once after all swaps, so loads still notify once.

## 10. Performance (measured)

Development editor build, Windows 11, Ryzen 7 3700X, one run of `SpnCore.InventoryViewModel.Performance`, each cell the mean of 5 repetitions, unique items in one container (`logs/m2b34-vm-tests-2.log`). Order of magnitude only — not a benchmark of a shipping build.

| Stacks in the container | raw `GetContainer` (public API copy) | view-model **full build** (opening a screen) | `AddItem` while the container is **shown** | `AddItem` into a small shown container | `AddItem` into a container **not shown** | `AddItem` with no view-model |
|---|---|---|---|---|---|---|
| 10 | 0.003 ms | 0.027 ms | 0.013 ms | 0.006 ms | 0.002 ms | 0.002 ms |
| 100 | 0.015 ms | 0.074 ms | 0.068 ms | 0.005 ms | 0.002 ms | 0.002 ms |
| 1 000 | 0.184 ms | 0.519 ms | 0.528 ms | 0.005 ms | 0.003 ms | 0.002 ms |
| 10 000 | 1.839 ms | 5.238 ms | 6.778 ms | 0.010 ms | 0.003 ms | 0.002 ms |

Reading: the bookkeeping cost for a container that is **not** on screen is the same as having no view-model (≈ 2–3 µs). For a shown container the cost per transaction is O(its stacks): ≈ 0.07 ms at 100 stacks, ≈ 0.5 ms at 1 000, ≈ 7 ms at 10 000 (half a frame at 60 fps). A body inventory has tens of stacks → ≪ 0.1 ms. The view-model's own cost is about 2–3× the raw copy of the container (strings, rows). **Significant overhead exists only for containers with thousands of stacks that are shown while many transactions run** — mitigations, none needed today: apply related changes in one `Apply` (one refresh), keep huge storage (HQ, trunk) unbound until opened, or add a row-level diff keyed by stack id (the `FSpnInventoryChange` could then carry created/changed/removed stack ids; not done because the measured cost does not justify the complexity). Memory: the view-model holds one row array per bound container (≈ the container's stack count × the row size, a few hundred bytes), freed on `Shutdown`.

## 11. Remaining work for the graphical inventory

1. **Decide:** how the catalog reaches packaged builds (§7), where presentation sets live and who authors them, container id scheme, whether an accept-mismatch dialog exists, the language/tone of the texts (the English fallbacks are developer wording; keys are stable).
2. **First widgets** (UMG, bound to a view-model, no logic): list/grid of rows with a selection highlight, details panel, capacity bars, equipment panel, action menu from `GetAvailableActions()`, message line from `OnMessage`. Use `GetVersion()` or the delegates; load icons asynchronously from `Row.Icon`.
3. **Input:** a new Enhanced Input context for UI navigation that calls `Navigate`/`ExecuteDefaultAction`; it must not touch the gameplay mapping. Dead-zone, key repeat and focus visuals are widget concerns.
4. **Localisation:** gather the `LOCTEXT` namespace `SpnInventory`, translate (German first), replace the container-kind title by localised names.
5. **Quantity UI:** a stepper for split/transfer using `DefaultQuantity`/`MaxQuantity`.
6. **Drag and drop** maps to `ExecuteAction(Transfer/Merge)`; the view-model already reports availability.
7. **Row diff** only if profiling shows a need (§10).
8. **World items** (pickup/drop actors, the migration of the placeholder pickup counts) remain a separate, undecided task.
9. **Tests with real widgets** (automation with a headless UMG) once widgets exist; until then the view-model tests are the contract.

## 12. Known limitations

* No widget, no presentation asset file, no input binding, no packaged-build verification, no Fedora run of the new Core code.
* The English texts are fallbacks; titles are container kind ids.
* `ExecuteAction` reports success/failure but a multi-step UI flow (confirmation, quantity prompt) is the widget's job.
* The view-model is single-threaded (game thread) like the subsystem.
* Selection follows a stack only among *bound* containers; a stack moved to an unbound one clears the selection.
* A catalog mismatch can be accepted for a whole load, not per item; there is no migration (by design, §6).
* The M2A4 movement failures remain (§9) and are unrelated.
