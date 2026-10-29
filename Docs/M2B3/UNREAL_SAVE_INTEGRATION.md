# M2B3.2 — Unreal save subsystem integration (WorldFacts)

> **Update M2B3.3 (2026-10-29):** `player.inventory` is now saved and loaded together with WorldFacts while the inventory subsystem has a catalog; the statements below that inventory is "not saved" / "has no Unreal owner" describe the M2B3.2 state. Sections 3, 5 and 9 below were revised; the full description is in `UNREAL_INVENTORY_INTEGRATION.md`. The map revision guard mentioned in section 9 now exists in the Core (`WorldMapState::Revision()`), but the map is still not integrated.

Status words: **Implemented / Tested / Proposed / Unverified**. Date: 2026-10-29. Base: `da1af14` + the uncommitted M2B3.1 and M2B3.2 changes. Nothing was committed or pushed; the Git stash, the installed engine, existing maps/assets and the Fab arrangement (plugin stays isolated) were not touched.
Master consultation (CLAUDE.md §11): **no new save format and no new persistence rule is introduced** — the existing M2B2 container (`SPN-SAVESLOT 1`) and the existing `world.facts` payload are used verbatim; the only new decisions are Unreal-side policies (slot-id rule, refusal of sections without an owner), listed in §9 as proposals for the developer. Nothing about axis presets or coordinates is touched: the save stores no positions.

## 1. What was implemented

| Item | File | State |
|---|---|---|
| `USpnSaveSubsystem` (UGameInstanceSubsystem) | `Source/SpnCore/Public/SpnSaveSubsystem.h`, `Private/SpnSaveSubsystem.cpp` | Implemented, Tested |
| Private Core-typed state of the world-state subsystem, shared with the save subsystem (friend) | `Private/SpnWorldStateInternal.h` (the `FState` struct moved out of `SpnWorldStateSubsystem.cpp`, no behaviour change) | Implemented |
| Console commands `Spn.SaveGame [slot]`, `Spn.LoadGame [slot]` | `Source/SupernaturalProt/Spn/SpnWorldStateConsoleCommands.cpp` | Implemented, run in a real game session |
| Automation tests `SpnCore.SaveSubsystem.*` (12) | `Source/SpnCore/Private/Tests/SpnSaveSubsystemAutomation.cpp` | Tested |
| Core hardening found by the exception investigation (§6) | `Core/src/SaveStore.cpp` (`Display`) + a Windows unit test | Implemented, Tested |

Core types appear only in private headers/sources (Decision 0006): the public header has Unreal types, two enums and a status struct. The subsystem depends on `USpnWorldStateSubsystem` (`InitializeDependency`).

## 2. Public API (`SpnSaveSubsystem.h`)

```cpp
FSpnSaveStatus SaveSlot(const FString& SlotId);        // game thread
FSpnSaveStatus LoadSlot(const FString& SlotId);        // game thread, two-phase, failure changes nothing
bool           DoesSlotExist(const FString& SlotId) const;
static bool    IsValidSlotId(const FString& SlotId);   // 1..64 chars, first [a-z], rest [a-z0-9_]
FString        GetSaveDirectory() const;               // default <Project>/Saved/SpnSaves
FSpnSaveStatus SetSaveDirectory(const FString& AbsoluteDirectory);  // tests/tools; validated
void           ResetSaveDirectory();
FString        GetSlotFilePath(const FString& SlotId) const;
FSimpleMulticastDelegate OnSaved, OnLoaded;
```

`FSpnSaveStatus { ESpnSaveResult Result; FString Detail; ESpnSaveSource Source (None/Current/Previous); FString CurrentProblem; TArray<FString> PreservedSections; uint64 FactsRevision; FString Path; bool IsOk(); }`.
`ESpnSaveResult`: `Ok, InvalidSlotId, InvalidDirectory, WorldStateUnavailable, NotFound, Io, Corrupt, UnsupportedVersion, Rejected, SectionsNotIntegrated, InconsistentSnapshot, InternalError`. Success and failure are always explicit; `Detail` is for logs/UI, never for logic.

Slot ids use the same rule as the placeholder slots of `USpnWorldStateSubsystem` (no separators, dots or upper case), so a slot id can never traverse out of the save root. A load that succeeds also broadcasts the existing `USpnWorldStateSubsystem::OnWorldStateReplaced`, so door actors etc. refresh exactly as after the legacy `Spn.LoadWorld`.

## 3. What is actually saved

One file per slot: `<save root>/<slot>/game.spnsave` (+ `game.spnsave.prev`, transient `.tmp` / `.prev.tmp`). Format `SPN-SAVESLOT 1`, produced by the existing `CaptureGameSave`:

| Section | Content | Required |
|---|---|---|
| `world.facts` | `WorldFacts::Serialize()` — the complete fact store *including the journal* (test doors, test pickups, the placeholder inventory counts that live in facts) | yes |
| `slot.meta` | `facts_revision N` — the moment the snapshot was taken | no |

A real two-section example is about 150 bytes. **Not saved:** player position/rotation, actor state (doors are re-derived from facts), `player.inventory`, `world.map`, `world.objects`, any UI state. Nothing is invented for them.

Existing formats stay compatible: the `world.facts` payload is byte-identical to what the legacy `worldfacts.spnfacts` file contains, and the legacy files and commands are untouched (`Saved/SpnSaves/<slot>/worldfacts.spnfacts` vs `game.spnsave` live side by side). There is no migration and no import of legacy files yet.

## 4. Save directory

Default `<Project>/Saved/SpnSaves` (`FPaths::ConvertRelativePathToFull`), the same root as the legacy slots. `SetSaveDirectory` accepts only absolute paths without `.`/`..` segments, without characters Windows forbids in names, and at most 150 characters (so root + slot + `game.spnsave.prev.tmp` stays below the classic MAX_PATH); otherwise it returns `InvalidDirectory` and keeps the old root. The slot directory is created on the first save (`CreateDirectoryTree`); a failure is `InvalidDirectory`. Paths are converted FString → UTF-8 → `std::filesystem::path(u8string)`, so non-ASCII names work (Core tests cover `Spielstände 日本`). The tests use `Saved/SpnTestTemp/<guid>` and delete it afterwards; the real `Saved/SpnSaves` was not used by the automation tests (only by the manual console check in §7, slot `m2b32_check`, deleted again).

## 5. Behaviour and failure handling

Save (`SaveSlot`): validate slot id → find world state → create directory → read the existing main file → capture (`CaptureGameSave`, synchronous, read-only; facts revision compared before/after) → `SaveSlotStore::Save` (temporary file, flush, read-back compare, previous generation kept, atomic publish; on Windows `MoveFileExW(WRITE_THROUGH)`).

| Situation | Result | State of disk / live state |
|---|---|---|
| invalid slot id | `InvalidSlotId` | nothing touched |
| bad / uncreatable directory | `InvalidDirectory` | nothing touched (old root kept for `SetSaveDirectory`) |
| write, flush, read-back, rename fails | `Io` | previous save intact, temp files removed |
| existing save is from a newer build (container, section version, unknown *required* section) | `UnsupportedVersion` | **not overwritten**, file byte-identical |
| existing slot holds `world.objects` / `world.map` / `player.inventory` | `SectionsNotIntegrated` | **not overwritten** — saving WorldFacts only would drop them |
| existing slot holds unknown *optional* sections | saved; ids listed in `PreservedSections` | carried over byte for byte with their version/flags |
| existing main file is corrupt (checksum…) | saved | the corrupt file is not copied over `.prev`; the last good `.prev` stays |
| facts changed during capture | `InconsistentSnapshot` | nothing written |
| any exception | `InternalError` | nothing written (exception caught at the boundary) |

Load (`LoadSlot`) — two phases: **(1)** read main file → container checks (magic, version, section checksums, trailer, order, schema) → `ParseGameSave` builds *new* objects; **(2)** only then the live `WorldFacts` is replaced and listeners are notified. Candidates in order: main file, previous generation.

| Situation | Result | Live state |
|---|---|---|
| invalid slot id | `InvalidSlotId` | unchanged |
| neither file exists | `NotFound` | unchanged, no directory created |
| main file damaged / unreadable / missing but `.prev` valid | `Ok`, `Source = Previous`, `CurrentProblem` says why | replaced by the previous generation (a warning is logged) |
| main file checksum-valid but `world.facts` rejected by its parser, or `slot.meta` disagrees | falls back to `.prev`; if that fails too: `Rejected` | unchanged on failure |
| both generations damaged | `Corrupt` (or `Rejected` / `Io`, the last failure) | unchanged |
| a generation is from a newer build | `UnsupportedVersion`, **no fallback** to the older one | unchanged |
| slot contains `world.objects` / `world.map` / `player.inventory` | `SectionsNotIntegrated` | unchanged (no half-restore) |
| exception | `InternalError` | unchanged |

Unknown optional sections do not prevent loading; they are kept on disk (and carried by the next save).

## 6. Unreal exception compatibility — investigation

Facts from the real build (`Intermediate/Build/Win64/x64/UnrealEditor/Development/SpnCore/SpnCore.Shared.rsp`): the `SpnCore` module is compiled with **`/EHsc`**, `PLATFORM_EXCEPTIONS_DISABLED=0`, **`/GR-`** (no RTTI), `/std:c++20`, `/W4`, with Unreal's `/we4456…` set. So C++ exceptions are *enabled* in this module (the Core compiles and links), but Unreal's own code (outside this module) is not exception-safe: an exception that leaves the module would unwind through engine frames.

What can throw in the Core: a grep finds no `throw`/`try`/`catch` in `Core/src` or the headers; the Core reports errors as values. The standard library can throw. **Reproduced (MSVC 19.51):** `std::filesystem::path::u8string()` / `generic_u8string()` and `string()` throw `std::system_error` for a path that contains an unpaired UTF-16 surrogate. `SaveStore.cpp` built error messages with `generic_u8string()` (M2B3.1), so a save to such a path threw out of `SaveSlotStore::Save`/`StdSaveFileSystem` instead of returning `Io`. This is an actual compatibility defect.

Fixes (no error handling disabled or hidden):
1. `Core/src/SaveStore.cpp`: `Display()` is guarded and returns `"<path that is not valid Unicode>"`; the Windows volume description uses it as well. The failure is still reported as `SaveError{Io, …}`. New Windows unit test "a path with an unpaired UTF-16 surrogate is reported as an error and never throws" — **mutation check: with the guard removed the test fails** (the call throws), with it it passes.
2. `USpnSaveSubsystem::SaveSlot` and `LoadSlot` wrap all Core work in `try { … } catch (const std::exception&) / catch (...)` and return `InternalError` with the message; a throwing load leaves the live state unchanged because the swap is the last step. The exception is logged, never swallowed silently.
3. RTTI is off: the Core was checked to not use `dynamic_cast`/`typeid` (it compiled in Unreal, which would have failed otherwise); `std::variant`/`std::visit` do not need RTTI.

Not covered: `std::bad_alloc` on a huge allocation is caught by the same boundary (not provoked in a test); exceptions thrown by *Unreal* code inside the callbacks (`OnWorldStateReplaced` listeners) are the listeners' responsibility.

## 7. Tests and results

| Check | Result |
|---|---|
| Unreal Editor build (Fab isolated, engine untouched) | **Succeeded**; one failure on the way was my own test code (a global named `Slot` collided with engine headers, C4459) |
| `SpnCore.SaveSubsystem.*` (12 tests, `logs/unreal-savesubsystem-tests-4.log`) | **12 of 12 pass** |
| Console check in a real game session on the V2 map (`logs/unreal-console-commands.log`) | `Spn.SaveGame` twice OK (second keeps the first as `.prev`), `Spn.LoadGame` OK, missing slot → `NotFound`, `Bad/Slot` → `InvalidSlotId`; both generations 149 bytes |
| MSVC 19.51 Debug / Release, clang-cl 22.1.3 Debug (fresh directories, `/W4 /WX`) | 80/80 steps, 0 warnings, **481 of 481 CTest** each |
| Python (`python3.15 -I -m unittest discover -s Tools -p "test_*.py"`) | see §8 |
| QA inventory gate | see §8 |
| Existing Unreal regression | see §8 |

The 12 automation tests, with what each proves:

| Test | Proves |
|---|---|
| `Initialize` | subsystem + dependency exist per game instance, default root, slot-id rule (10 invalid ids incl. `../x`, `a:b`, 65 chars) |
| `InvalidSlotsAndDirectories` | invalid ids refused for save and load, nothing created; 9 bad directories rejected and the old one kept; uncreatable directory → `InvalidDirectory`; a lone surrogate in the directory gives a result, not an exception |
| `SaveCreatesFile` | file exists with exactly `world.facts` + `slot.meta` (`facts_revision 2`), no invented sections, no temp files, live state untouched |
| `WorldFactsRoundTrip` | save, change, load: door states, revision, listener and `OnLoaded` once; facts **byte-identical** (journal included) to the saved ones; the restored registry keeps working; a fresh game instance loads the same file |
| `RepeatedCycles` | 25 save→diverge→load cycles; exactly the current and the previous file remain, no temp files |
| `MissingSave` | `NotFound`, state unchanged, no listener call, no directory created |
| `CorruptSave` | empty, random text, truncated, last byte removed, flipped bit, trailing byte → `Corrupt`; live facts byte-identical afterwards, no listener call |
| `UnsupportedVersions` | newer section version, unknown required section, newer container version → `UnsupportedVersion`, **no rollback to the valid previous generation**, saving over them refused and the file untouched |
| `RejectedContent` | garbage facts behind a valid checksum and a wrong `slot.meta` → `Rejected`, state unchanged |
| `PreviousGenerationRecovery` | damaged main file, missing main file, checksum-valid-but-rejected main file → previous generation (`Source = Previous`, reason given, state = generation 1); both damaged → `Corrupt`, unchanged; a new save repairs the slot |
| `UnknownSectionsArePreserved` | a slot with two unknown optional sections loads, is saved again, and both survive byte for byte with version/flags; reported in `PreservedSections` |
| `SectionsWithoutOwnerAreNeverDropped` | for `player.inventory`, `world.map`, `world.objects`: load refused, save refused, file and live state untouched |

Honesty notes: the tests do not provoke `InconsistentSnapshot` or `InternalError` (no safe way to trigger them from outside; the Core tests cover the capture guard). Failure injection is by file content, not by a failing disk (the Core fault-injection tests cover that). The pre-existing `SpnCore.WorldState.*` tests are unchanged (their results are in §8).

## 8. Regression and environment results

All runs 2026-10-29, Fab isolated, engine untouched, logs in `Docs/M2B3/logs/` (`m2b32-*`).

| Check | Command | Result |
|---|---|---|
| MSVC 19.51 Core, fresh Debug (final state) | `winbuild.cmd` wrapper = vcvars64 → CMake/Ninja `/W4 /WX` → CTest | 80/80 steps, 0 warnings, **481 of 481 CTest** (11.0 s); Release 481/481 (4.2 s) and clang-cl 22.1.3 Debug 481/481 (8.1 s) were run one change earlier with identical sources apart from the restored mutation check |
| Unreal Editor build | `Build.bat SupernaturalProtEditor Win64 Development` | **Succeeded** (`unreal-editor-build-m2b32-*.log`) |
| New Unreal tests | `Automation RunTests SpnCore.SaveSubsystem` | **12 of 12 pass** |
| V2 `Full -Regression` (`20261029-175429`) | `run_gameplay_qa.ps1 -Suite Full -Regression -NoVideo` | QA **PASS 166, FAIL 2, BLOCKED 0, NOT_IMPLEMENTED 3, NOT_TESTED 1** — identical to before M2B3.2; automation **66 tests, 63 success, 3 failed** (`SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps`, `SpnQA.Jump` = the 2 late jumps) |
| V3 Doors / Access / Traversal | `-Suite … -Map …V3Interiors` | **89/89**, **15/15**, **22/22** |
| V3 MovementDemo (`175135`) | `-Suite MovementDemo` | **26 PASS, 1 FAIL** — the known flaky `ledge_80_blocks_even_with_a_jump` (see below) |
| Python | `python3.15 -I -m unittest discover -s Tools -p "test_*.py"` | **219 tests OK** |
| QA inventory gate | `check_qa_inventory.py --check` | 23 systems, **0 problems** (the gate first rejected the new class and the two console commands until they were listed — as designed) |

Automation count: 66 test rows = 63 `Success` + 3 `Fail` (before M2B3.2: 54 = 51 + 3; the difference is the 12 new tests). The 12 new `SpnCore.SaveSubsystem.*` tests, the 5 `SpnCore.WorldFrame.*` tests and the 4 pre-existing `SpnCore.WorldState.*` tests (`Initialize`, `SaveAndLoadSlot`, `SlotNames`, `FailedLoadKeepsState`) are all among the successes — the legacy save path is unchanged and still passes.

**Known movement failures, preserved and unchanged (M2A4 track, not touched):**
* `Jump.late_jump_ledge_45_jog`, `Jump.late_jump_ledge_40_run` (→ `SpnQA.Jump`)
* `SpnGame.Movement.Obstacles`, `SpnGame.Movement.StairsAndRamps` (30 cm)
* `MovementDemo.ledge_80_blocks_even_with_a_jump` is **still flaky**: it passed 3 of 3 runs after the M2A4.1 hardening, and failed in the run above. The trace shows the same mechanism as before and shows that the widening of the test wall from 4 m to 12 m did not remove it: after the jump the character stands at the wall face, then **slides sideways along the face at ~130 cm/s** (this time east, 630 cm, previously west) although the input is straight into the wall, walks round the end of the wall and keeps going (stopped 4 489 cm beyond the face). The wider wall only moved the end further away. This is a movement/collision defect, not a save problem, and it needs its own investigation (M2A4.2): it must not be hidden by making the test wall even wider.

## 9. Snapshot consistency — what is and is not guaranteed

Investigated in `Core/src/GameSave.cpp` and the headers:

| Domain | Revision / guard in `CaptureGameSave` | Status |
|---|---|---|
| WorldFacts | `Revision()` = journal size; compared before and after serialisation; every change is journaled, so any mutation during capture is detected | **guarded** — this is the only boundary integrated and tested here |
| Inventory | `InventoryState::Revision()`; compared and recorded in `slot.meta` (`inventory_revision`) | guarded in the Core, **no Unreal owner** |
| Map | `WorldMapState` has **no revision**; discovery and markers can change without any counter | **not guarded** |
| Objects | `world.objects` = `RegionStateCache::Snapshot(facts, catalog)`: derived from the facts at capture time plus the cached state of regions that are not loaded; the cache has no revision of its own | partially covered through the facts revision; the not-loaded part is not |

What the WorldFacts boundary really guarantees: capture is synchronous on the game thread, never mutates, serialises the whole store in one call, and compares the revision before and after. It protects against a mutation by a callback that runs *during* the capture; it does not protect against another thread (the Core is single-threaded; the subsystem asserts `IsInGameThread()`), and it says nothing about actors outside WorldFacts (positions, physics). The restored state is all-or-nothing for the facts. **No cross-system snapshot consistency is claimed**; a save of facts + inventory + map taken in one call would be consistent only if all three were captured synchronously on the game thread, which is true for the call but is not *checked* for the map.

**Requirements for adding the other domains** (each is a follow-up work item, none started):

1. *Inventory*: an Unreal owner (`USpnInventorySubsystem`) holding `InventoryState` and the `ItemCatalog`; a catalog source (data assets — none exist); `ISpnSaveContributor`-style hook or direct sources in `GameSaveSources`; `GameLoadContext.itemCatalog` provided at load; the placeholder pickup counts that live in facts need a documented migration (Decision 0004); drop the `player.inventory` refusal only together with load support and a test that a slot with inventory restores facts *and* inventory atomically.
2. *Map*: first add a **revision counter to `WorldMapState`** (Core change + tests) and include it in the capture guard and `slot.meta`; an owner subsystem; a `WorldMapDefinition` source for `GameLoadContext.mapDefinition` (WorldForge/test fixture); definition identity should be stored so a save is not applied to another definition.
3. *Objects*: a `WorldObjectCatalog` and a `RegionStateCache` owner (region streaming); decide which side wins when the facts and the cache disagree; test the not-loaded region path.
4. For all: the two-phase load must parse *all* sections before swapping *any* of them (the Core already does this in `ParseGameSave`); the save must refuse to write while any owner is mid-transaction; async saving only on a copied `SaveSlot` snapshot.

## 10. Remaining limitations and open decisions (Proposals)

* Only WorldFacts is saved. Positions, actors, inventory, map, objects are not.
* The legacy `Spn.SaveWorld` / `Spn.LoadWorld` remain unchanged and write a *different* file; nothing is migrated or unified, so the two systems can hold different states of the same slot name. The legacy commands should be removed only after the developer accepts the new path.
* Policy decisions made here, for the developer to confirm or change: (a) slot-id rule = the placeholder rule; (b) sections without an owner make a slot read-only for this build (refuse instead of drop); (c) `UnsupportedVersion` never falls back to the older generation.
* No retry on a locked file (`Io`) and no UI; `Spn.SaveGame` logs the status.
* Power-loss durability is unverified (see `SAVE_WINDOWS_VALIDATION.md`); the directory flush is a no-op on Windows.
* Co-op: host authority for saves is not modelled; a guest must never call `SaveSlot`.
* `OnWorldStateReplaced` listeners run synchronously during `LoadSlot`; a listener that mutates the facts changes the state *after* a successful load (no protection).
