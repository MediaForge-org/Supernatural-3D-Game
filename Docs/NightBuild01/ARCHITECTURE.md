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

No format change: the already-defined optional section `world.map` now has an Unreal owner, and the existing policy applies (a slot with a section this build cannot restore is refused, never half-loaded). `InspectSlot` parses exactly like a load but swaps nothing and reports: loadable / previous generation / catalog mismatch / newer build / not integrated / corrupt / missing. A catalog mismatch is never accepted automatically: the UI asks, and the Core still validates every item. **Not saved (documented limitation):** the player's position and rotation (there is no section for it).

## 6. Decisions left open (deliberately)

Axis preset / origin (CQ-101); co-op pause semantics; final map data source; whether waypoints are limited to one per player; item/inventory UI key bindings for pads (Triangle for the map is a placeholder; no existing mapping was changed); music/effects audio buses (not offered, none exist).
