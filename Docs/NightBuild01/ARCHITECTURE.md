# Night Build 01 — Architecture

Status words: **Implemented / Tested / Proposed / Unverified**. Everything here is an engineering proposal for review, **not master canon**. No world geography, no final map extent and no WorldForge data was created or changed. Master consultation (CLAUDE.md §11): no master requirement for map UI, minimap, navigation or menus exists (see `Docs/M2B1/GAME_SYSTEMS_ARCHITECTURE.md`, "Master basis"); nothing was found that contradicts this work; nothing here ratifies an OPEN item.

## 1. Layers

```
Core (portable C++20, unchanged in this build)      WorldMapState, MinimapFrame, Navigation, GameSave, InventoryState, WorldFacts
        │ compiled into
SpnCore  (Unreal module, no UI)                      USpnMapSubsystem        NEW   owns the ONE WorldMapState (map, minimap, compass, waypoint)
                                                     USpnSaveSubsystem       EXT   world.map section; ListSlots(); InspectSlot() (read-only dry run)
                                                     USpnWorldStateSubsystem EXT   SetFlag(), ResetToEmpty()
                                                     USpnInventorySubsystem  —     unchanged; pickups/containers use its transactions
SupernaturalProt (game module, UI and world)         USpnSessionSubsystem    NEW   game flow: new game / continue / load / main menu, world setup
                                                     USpnSettingsSubsystem   NEW   player settings (+ thin wrappers over UGameUserSettings)
                                                     USpnUiSubsystem         NEW   screen router + HUD host + the single input pre-processor
                                                     USpnMenuScreen / USpnMapScreen / USpnHudWidget / SSpnMapView (UMG + Slate)
                                                     ASpnWorldPickup / ASpnWorldContainer    NEW   inventory-backed world objects
                                                     ASpnMainMenuGameMode    NEW
```

**Single source of truth.** The full map, the minimap and the compass all read `USpnMapSubsystem`; none of them keeps a marker, a discovery state or the waypoint. The active navigation destination *is* the player waypoint marker (at most one, replaces the previous one), so it is saved and restored by the map section without a second store.

**Core stays the implementation.** No inventory, map, minimap or save rule was re-implemented in Unreal. Pickups use `ProbeCommands` (the Core's dry run) and `Apply` (the Core transaction); the minimap frame is `spn::core::BuildMinimapFrame`; distance and arrival are `DestinationFromMarker` / `MakeStraightLineRoute` / `HasArrived`. `Core/` was not modified.

## 2. Map, minimap, navigation

* **Data:** `USpnMapSubsystem::InstallTestDefinition()` builds a PLACEHOLDER definition (one 400 m layer, one region, nine POIs) positioned from the real test level coordinates (`Tools/spn_district_layout.py`) through `SpnWorldFrame`. It is flagged `IsTestDefinition()` and every screen says "TEST AREA". It is not the 2,200 km² world and not geography.
* **Coordinate preset:** `EastSouthUp` (the preset that keeps the authored "X = east") is a *parameter* of the subsystem (`FSpnWorldFrame`), documented as the working preset for the test levels; the WorldForge/Unreal decision (`CQ-101`) is still open and changing it is a data change (reinstall the definition).
* **Tiled/streamed worlds:** every view queries only its own rectangle (`QueryVisible`, `BuildMinimapFrame` → spatial index). The grid draws a bounded number of lines at any zoom (spacing grows with `metres/pixel`); no per-world allocation exists. **Unverified at scale:** only the 400 m test area was exercised; the Core's own benchmarks (`Tests/Core/BenchM2B1.cpp`) are the only large-scale evidence.
* **Navigation is a straight line.** `FSpnNavigationReading::bStraightLine` is always true; the UI says "direct line - no road routing". There is no pathfinding, no road graph and no obstacle awareness.
* **Discovery:** `ObservePlayer` marks POIs within 60 m *Known* and within 12 m *Visited*, and the region *Visited* (cheap early-out below 2 m of movement).
* **Minimap update strategy:** rebuilt only when the player moved > 30 cm, turned > 1°, the map revision/zoom/orientation changed, or every 0.5 s; the compass is updated every frame (no allocation beyond a few text draws).

## 3. Interface and input

`USpnUiSubsystem` registers **one** Slate input pre-processor and decides per key, in this order:

1. console active, text field focused, or a console key → **nobody** (the key is never touched);
2. the inventory screen is open → not this router (the inventory has its own pre-processor; unchanged semantics);
3. a modal page of this router is open (main menu, pause, settings, slots, confirmation, map) → the top page, always consumed;
4. gameplay → `Esc`/Options = pause, `M`/Triangle = map, everything else goes to the game.

The inventory refuses to open while a modal page is up (`OpenBlocker`). Pause and map pause the game (single player; **co-op needs a different rule — open decision**), the inventory does not (unchanged). Input mode is saved on the first page and restored when the last page closes (`FlushPressedKeys`, ignore-move/look reset, cursor restored). The whole flow is **opt-in** (`USpnSessionSubsystem::IsGameFlowEnabled`: main-menu level, `-SpnGameFlow`, `Spn.Game.Enable`), so existing levels and automation tests run exactly as before.

Widgets are pure code-built UMG (`UUserWidget` + `WidgetTree`) like the inventory screen; the map and minimap are a Slate leaf widget (`SSpnMapView`) wrapped in a `UWidget` because UMG has no drawing primitives for them. Colours/metrics come from the existing `SpnUi` palette. Texts are in the existing key table (`FSpnUiText`, English + German, 114 new keys in `SpnUiTextNightBuild.inc`, checked by `SpnGame.NightBuild.UiText`).

## 4. World objects

* `ASpnWorldPickup`: dry run (`ProbeCommands`) → `SetFlag(taken)` → `Apply(AddItem)`; if the apply fails the flag is undone. All-or-nothing (the Core transaction is all-or-nothing, so there is **no partial pickup**). Taken state = bool fact `world.pickup.<StableId>.taken`.
* `ASpnWorldContainer`: stable container id = `0x1_0000_0000 + CRC32(StableId)`; created (with its authored items, in one transaction) only if it does not exist; an existing/restored container is never re-seeded; re-created after a new game. Using it opens the inventory screen as a *container view* (tabs: the player's container + this one; world containers are hidden from the plain inventory). Transfers are the normal inventory actions = Core transfers (capacity, weight, volume and category rules are the Core's).
* **Line of sight / range:** the existing interaction sweep (250 cm, sphere r = 15 cm) hits the first blocking actor, so a wall between player and object blocks the interaction by construction.
* **What is *not* persisted:** actors (position, lid state) — `ObjectPersistence` is not integrated in Unreal. Only contents (`player.inventory`) and the taken flag (`world.facts`) persist. A container actor exists because its level places it.

## 5. Save

No change of the container format. One new OPTIONAL section, `player.pose` (version 1), written and read by the Unreal layer (`USpnSaveSubsystem`, `FSpnPlayerPose` in `SpnPlayerPose.h`); the Core sees it as an unknown optional section, so every older reader keeps loading the slot and older builds carry the section over. `world.map` has its Unreal owner since the first Night Build, and the existing policy applies (a slot with a section this build cannot restore is refused, never half-loaded). `InspectSlot` parses exactly like a load but swaps nothing.

**`player.pose`** (strict text payload, `SPN-PLAYERPOSE 1`): level package path, the world frame (axes preset + origin), position in the CANONICAL frame (metres, x east, y north, z up, `%.17g`), compass bearing, pitch, gait, crouch. The position goes through `SpnWorldFrame` (the same adapter the map uses); no second coordinate system exists, and a later decision about the axis preset (CQ-101) is a frame parameter, not a save migration.

* **Capture:** `PoseProvider` (bound by `USpnSessionSubsystem`) fills the live pose at save time. If there is no player (menu world, no pawn) the slot is written WITHOUT a pose and an older pose of that slot is dropped, never carried over.
* **Load, phase 1:** the pose is parsed together with facts / inventory / map into new objects. A present but unreadable pose rejects the whole generation (`Rejected`; the previous generation is tried), nothing live changes. A pose written by a newer build (`version > 1`) refuses the load and the overwrite (`UnsupportedVersion`).
* **Load, phase 2:** after the state swap the subsystem broadcasts `OnPoseLoaded`; the session applies the pose once the player pawn of the right level exists (a level travel may still be in progress; a bounded retry ticker waits for the pawn). Placement: right level, representable position, walkable ground within 4 m below (line trace), `TeleportTo` without penetration; otherwise the player stays at the level start and the HUD says so (`ui.pose.invalid` / `ui.pose.level_missing`). **Never a silent default.**
* **Level choice:** `LoadGame` from the menu (or in place when the saved level differs) travels to the saved level when its package exists; otherwise to the test level with the notice above.
* **Legacy slots** (no `player.pose`) load completely; the player starts at the level start and the HUD says `ui.pose.legacy`. `FSpnSaveStatus::bPoseInSlot` tells the caller.
* **Not saved:** velocity, jump/fall state, animation state, the camera boom, stamina etc.; there are none that matter for a rest position.

## 5a. World objects and persistence

Pickups and containers keep their STATE where it already lives and is saved: the taken flag as a bool fact in `world.facts`, the contents in the inventory section (container id = fixed offset + CRC32 of the StableId). The actors are recreated from the level / the scenario spawner and bring themselves in line with the loaded state (`OnWorldStateReplaced` / `OnInventoryReplaced`). Identity guard: an actor whose StableId (for containers: or derived container id) is already used by a live actor of the world disables itself and logs an error, so two actors can never share one flag or one inventory container.

The Core `ObjectPersistence` layer (catalog + `RegionStateCache` + `world.objects`) is **not integrated**. Reason: it stores the state of catalog objects a second time (the facts already hold it), and its added value (state of unloaded regions, orphans) needs region streaming, which does not exist. Integrating it now would create two copies of the same state that must be kept consistent. This is a persistence-format decision for the developer (see `COMPLETION_REPORT.md`, open decisions).

## 5b. Input mode

A level change keeps the viewport. The menu level leaves the mouse-capture mode on "capture only while a button is held"; the gameplay level used to inherit it, so the mouse look worked only with a button down (found by the scripted run, reproduced by `SpnGame.Input.ModeAfterTravel`). `USpnUiSubsystem::ApplyGameplayInput` now starts every gameplay world in game-only input with a hidden cursor and look/move enabled (waits for the pawn, does nothing while a page is open).

## 5c. Settings

Persisted in `GameUserSettings.ini` `[SpnPlayer]`. New: music volume and effects volume (volume of the engine sound classes `/Engine/EngineSounds/Music` and `/SFX`, in memory only) and UI scale (`UUserInterfaceSettings::ApplicationScale`). The project has no sounds yet; the settings page says so next to both sliders.

## 6. Decisions left open (deliberately)

Axis preset / origin (CQ-101); co-op pause semantics; final map data source; whether waypoints are limited to one per player; item/inventory UI key bindings for pads (Triangle for the map is a placeholder; no existing mapping was changed); music/effects audio buses (not offered, none exist).
