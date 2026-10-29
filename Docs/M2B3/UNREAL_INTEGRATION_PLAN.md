# M2B3.1 — Unreal integration plan for WorldMap, Minimap, Inventory, Navigation and Save

Status words: **Implemented / Tested / Proposed / Unverified**. Everything marked *Proposed* is an engineering proposal for the developer's review, not master canon and not approved. No gameplay content, UI or map geography was invented in this milestone.
Master consultation (CLAUDE.md §11): no new gameplay system, save format or architecture decision is *made* here (a proposal document plus one thin adapter); the master findings relevant to coordinates are in `Docs/M2B2/WORLD_COORDINATE_CONVENTIONS.md` §1 and were not re-searched. Nothing in this plan changes any master-open decision (axis preset, world origin, area, geography).

## 1. What exists today (verified by reading and building)

| Piece | State |
|---|---|
| `SpnCore` module (`Source/SpnCore/SpnCore.Build.cs`) | compiles **every** `Core/src/*.cpp` straight into the Unreal module (`ConditionalAddModuleDirectory`, `NoPCHs`, no unity, C++20). The real editor build of 2026-10-29 compiled all M2B1/M2B2 files (Coordinates, Minimap, Navigation, WorldMap, SpatialGrid, Inventory, InventoryPersistence, SaveSlot, SaveStore, GameSave, …) with Unreal's clang toolchain: **Succeeded** (`logs/unreal-editor-build-1.log`). |
| Pattern for exposing the Core | `USpnWorldStateSubsystem` (UGameInstanceSubsystem): Core types stay behind a private implementation (`TPimplPtr`) and a private include path (Decision 0006); the public API uses Unreal types only; results are small enums + status structs. |
| Save wiring | still the placeholder `worldfacts.spnfacts` file per slot (`Spn.SaveWorld` / `Spn.LoadWorld`); **the new `SaveSlotStore`/`GameSave` are not used by the game**. |
| Map / minimap / inventory / navigation in Unreal | nothing. No data assets, no UI, no subsystem. |
| Coordinates | M2B3.1 adds `SpnWorldFrame` (below). No gameplay code uses it. |

## 2. Delivered in M2B3.1: `SpnWorldFrame` (Implemented, Tested in Unreal)

Files: `Source/SpnCore/Public/SpnWorldFrame.h`, `Private/SpnWorldFrame.cpp`, `Private/Tests/SpnWorldFrameAutomation.cpp`. A free-function adapter (`SpnWorldFrame::ToUnreal / ToWorld / BearingToYawDegrees / YawDegreesToBearing`) that forwards to the Core `Coordinates.h`, takes the axis preset and origin as parameters (no global state, **no preset chosen for the project**), and returns `false` without touching the output for NaN/∞/out-of-range input. Public header has no Core types.

Automation tests `SpnCore.WorldFrame.*` — **5 of 5 pass in the real engine** (`logs/unreal-worldframe-tests.log`):

| Test | What it proves in Unreal |
|---|---|
| `KnownValues` | hand-computed conversions for both presets and an origin offset |
| `RoundTrip` | both presets, points up to ±9 900 km, < 1 nm |
| `RejectsInvalid` | NaN, ∞, > 10 000 km rejected, outputs untouched |
| `EngineDirections` | **engine facts, not assumed:** yaw 0 faces +X, yaw 90 faces +Y; for both presets and the four compass bearings the yaw returned by the adapter makes `FRotator::Vector()` point along the converted compass step; the east/north/up triple product is −1 (the conversion is a mirror, as it must be between a right-handed ENU frame and the left-handed engine). Measured side finding: `FRotationMatrix::TransformVector` is only float-accurate (x = 1.2e-8 for yaw 90), so that one comparison uses 1e-6. |
| `TestMapLocations` | the V3 PlayerStart `(-10550, -4700, 100)` cm and "West Avenue" / "North Street" of the shared district layout, converted under both presets (see §3) |

## 3. Coordinate convention — findings

1. **Canonical (Core):** metres, double, x east, y north, z up. Unchanged.
2. **Engine facts verified:** UE yaw 0 = +X, yaw 90 = +Y (also observed in the real gameplay trace: a character facing control yaw 90 walks towards increasing Y).
3. **The authored test levels use a third, unnamed convention.** `Tools/spn_building_kit.py` and `spn_district_layout.py` declare "centimetres, X east, Y north, Z up", i.e. they treat UE +Y as *north* (`North Street` at Y = +14 200, `South Street` at Y = −14 000). In a left-handed engine a map with X = east and Y = north is a **mirror image** of the real compass layout. Neither Core preset reproduces those labels:
   * `EastSouthUp`: UE +Y is south, so "North Street" is 142 m **south** of the origin and the PlayerStart is 47 m **north**.
   * `NorthEastUp`: UE +X is north; "West Avenue" (X = −10 200) becomes 102 m **south**.
   The test levels are explicitly placeholders, so this is harmless today; it matters the moment the minimap or compass is drawn on them (a compass would point the wrong way relative to the street names).
4. **Which preset fits the Unreal project?** Proposed (not decided): **`EastSouthUp`** as the working preset for test-level integration, because it is the only one that keeps the authored "X = east" and the existing yaw-0-faces-east habit of the demo/QA code; the authored "north/south" street names are then just names. `NorthEastUp` gives "forward at yaw 0 = north", which is the natural choice for a game that points the player north at spawn, but it would turn every authored X into north. **The real decision belongs to the developer together with the WorldForge frame statement (`CQ-101`).** The adapter takes it as data, so deciding later changes a configuration value, not code.
5. **Unresolved:** WorldForge handedness / datum / origin; whether World Partition + origin rebasing are used (affects `OriginMetres`); heading convention of authored actors (all current QA code uses yaw only); how the compass/minimap is rotated on screen (Core `MinimapTransform` uses bearing, clockwise from north).
6. **No project-wide change was made.** No level, config or source file outside the new adapter uses a preset.

## 4. Proposed integration architecture (Proposed, Unverified)

Principle: the Core remains the single implementation; Unreal code is a thin owner/adapter. One subsystem per bounded state, all game-thread only (the Core is not thread-safe and nothing here needs threads).

```
                   ┌─────────────────────────────── SpnCore (Unreal module) ───────────────────────────────┐
 GameInstance  ──► │ USpnSaveSubsystem      owns SaveSlotStore + SaveSchema; (de)serialises via GameSave    │
                   │ USpnInventorySubsystem owns InventoryState + ItemCatalog; applies command batches      │
 World         ──► │ USpnMapSubsystem       owns WorldMapState (+ definition); builds MinimapFrame per tick │
                   │ USpnNavigationSubsystem owns destinations/route queries over the map definition        │
                   │ SpnWorldFrame          pure functions: Unreal cm ↔ canonical metres                    │
                   └───────────────────────────────────────────────────────────────────────────────────────┘
        Core types behind TPimplPtr + private includes (Decision 0006); public API = Unreal types + small enums/status structs
```

| Subsystem | Lifetime | Owns | Public API (Unreal types only) | Notes |
|---|---|---|---|---|
| `USpnSaveSubsystem` | GameInstance | `SaveSlotStore`, `SaveSchema` | `Save(SlotName)`, `Load(SlotName)`, `FSpnSaveStatus` (error code + detail), `GetSlotDirectory()` | path from `FPaths::ProjectSavedDir()/SpnSaves/<slot>` converted with the wide `TCHAR*` (UE `TCHAR` is `wchar_t` on Windows: non-ASCII safe); collects sections from the other subsystems through a small registration interface `ISpnSaveContributor { SectionId, Capture(), Apply() }` so the save code does not depend on the domains; retry/"save failed" notification on `Io` (finding S4). Migration from the placeholder `worldfacts.spnfacts` is a separate, documented save-compatibility event. |
| `USpnInventorySubsystem` | GameInstance | `InventoryState`, `ItemCatalog` | `Apply(Holder, Commands)` → `FSpnInventoryResult`, read queries (`GetStacks`, totals) | commands are the Core value types wrapped as `USTRUCT`s or built through helper functions; the catalog comes from data (none exists: tests build a tiny one). Authority model is OPEN (co-op): keep the subsystem call-only-from-host in mind. |
| `USpnMapSubsystem` | World | `WorldMapState`, definition | `SetDefinition(...)`, `Discover(...)`, `AddMarker(...)`, `BuildMinimapFrame(PawnLocation, Yaw, View)` → `FSpnMinimapFrame` | uses `SpnWorldFrame` for every Unreal↔canonical conversion; definition arrives from WorldForge/data assets later; until then only test fixtures. Frames are plain structs for a future UI. |
| `USpnNavigationSubsystem` | World | navigation graph/destinations | `SetDestination`, `QueryRoute` | depends on the map definition; layer transitions are Core concepts, not Unreal volumes. |

Interfaces to prepare in the **next** milestone (declare, don't fill): `FSpnSaveStatus`, `ISpnSaveContributor`, `FSpnInventoryResult`, `FSpnMinimapFrame`, `FSpnMapDefinitionSource` (how a definition is loaded: data asset vs. generated file), plus the subsystem class skeletons registered but empty-functional.

Design rules (Proposed):
1. **No Core logic is duplicated** in Unreal; if a rule is missing, it goes to the Core with a test first.
2. **Core types never appear in public headers**; add `SPNCORE_API` only on subsystem classes and free functions.
3. **No Unreal headers in Core `.cpp` files.** One Core file includes `windows.h` (`SaveStore.cpp`, inside `#if defined(_WIN32)`); it compiles in the Unreal build today because those TUs include no Unreal header. Keep it that way; if a Core file ever needs both, wrap with `Windows/AllowWindowsPlatformTypes.h` as `SpnWorldStateSubsystem.cpp` does.
4. Game-thread only; asynchronous saving (copy the `SaveSlot` snapshot, write on a worker) is a later optimisation that the value-type `SaveSlot` already allows.
5. Test seam: each subsystem gets an Automation test that runs without a level (like the existing `SpnCore.WorldFacts.*`).

## 5. Risks and unverified assumptions

| # | Risk / assumption | Status |
|---|---|---|
| R1 | Axis preset undecided; test levels are mirror-labelled (§3) | open decision |
| R2 | UBT compiles all Core `.cpp` into one module with `PCHUsage = NoPCHs`; a Core file that is not self-contained would break the Unreal build only | observed fine today |
| R3 | Core uses C++20 features; the UE toolchain accepted them (`CppStandard = Cpp20`) | verified by the build |
| R4 | Exceptions: a grep finds no `throw`/`try`/`catch` in `Core/src` or the headers; the Core is `Result`-based. The standard library can still throw (`bad_alloc`, `std::filesystem` path conversion); `SpnCore.Build.cs` does not enable exceptions, and the effective compiler flags were not inspected, so what a throw would do inside Unreal is **Unverified** |
| R5 | Save directory on OneDrive/network folders: `MoveFileExW` can fail or be slow; retry logic missing (S4) | open |
| R6 | Power-loss durability of the save publication | **Unverified** (`SAVE_WINDOWS_VALIDATION.md`) |
| R7 | Co-op/host authority for inventory and map commands | OPEN in the master |
| R8 | WorldForge export frame/format | no data; no code depends on it |
| R9 | PIE multiple worlds: `USpnMapSubsystem` as a World subsystem is created per PIE world; shared state must not leak between them | to verify when written |
| R10 | The existing placeholder pickup/door persistence uses WorldFacts keys; moving inventory to `InventoryState` is a save-compatibility event (Decision 0004) | open |

## 6. Recommended order

1. **Decide with the developer** (no code): axis preset + origin policy (CQ-101), and whether `SpnWorldFrame`'s `EastSouthUp` stays the temporary default for test levels.
2. `USpnSaveSubsystem` skeleton + `ISpnSaveContributor`, wired to the *existing* WorldFacts as its first contributor (`world.facts` section), with an automation test for save → load → corrupt → previous generation and the `Io` retry. This is the lowest-risk path to put the new store into the game and exercises everything verified in `SAVE_WINDOWS_VALIDATION.md`.
3. `USpnInventorySubsystem` over a test-built catalog; migrate the placeholder pickup counts behind a documented compatibility step.
4. `USpnMapSubsystem` + `FSpnMinimapFrame` on a test definition; automation test comparing frames with hand-computed values for a pawn at the known test-map locations (uses `SpnWorldFrame`).
5. `USpnNavigationSubsystem`.
6. Only then UI (a debug overlay first), data-asset import for definitions, and WorldForge import.
