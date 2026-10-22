# 0006 — Unreal integration of the C++20 core (engineering notes)

Status: engineering notes, **not** master canon. First working integration,
2026-10-19, UE 5.8.3 (launcher build), MSVC 14.51, Windows 11. Reviewable.

## Windows/MSVC build fix
- Cause: doctest v2.4.12 forward-declares `std::basic_ostream` unless
  `DOCTEST_CONFIG_USE_STD_HEADERS` is set. Stringifying `std::string_view`
  instantiates MSVC's `operator<<` (`__msvc_string_view.hpp`) on that incomplete
  type → C2027. libc++ already forces the option; libstdc++ happened to work.
- Fix: `DOCTEST_CONFIG_USE_STD_HEADERS` on the `spn_doctest` interface target
  (tests only; Core flags unchanged).
- `/EHsc` was never missing: CMake's MSVC default `CMAKE_CXX_FLAGS` is
  `/DWIN32 /D_WINDOWS /EHsc`. Passing `-DCMAKE_CXX_FLAGS=/FIostream` replaced
  that default, which is what removed `/EHsc`.
- Windows MAX_PATH: cloning doctest fails in deep build directories (long
  example file names). Keep build dirs short, e.g. `build/<name>` in the repo.

## Integration approach
- Unreal module `SpnCore` (in the Unreal project, `Source/SpnCore/`) compiles
  `Core/src/*.cpp` **directly from this repository** via UBT's
  `ModuleRules.ConditionalAddModuleDirectory`. Nothing is copied.
- Reason: UE uses `/MD` (release CRT) in every configuration and
  `_HAS_EXCEPTIONS=0` in game builds. A CMake-prebuilt `spn_core.lib` (`/EHsc`,
  `_HAS_EXCEPTIONS=1`, debug CRT in Debug) would mix incompatible settings.
  Compiling with UBT gives each UE configuration matching flags automatically.
- Core path: env var `SPN_CORE_ROOT`, else the repository that contains the
  project (`<repo>/Unreal/SupernaturalProt`, layout since commit `cd7d0da`), else a
  sibling checkout `../Supernatural-3D-Game` next to the Unreal project. No
  absolute paths. (2026-10-20: the in-repo case was added; before it, the in-repo
  project failed with a RulesError unless `SPN_CORE_ROOT` was set.)
- Core TUs: no PCH, no unity build (each .cpp stays its own TU, as in CMake).
- Core headers are a *private* include path of `SpnCore`. In modular (editor)
  builds `SpnCore` is a DLL and Core symbols are not exported, so other modules
  use the exported bridge functions (`SPNCORE_API`), not Core types directly.

## World state subsystem (2026-10-20, Proposal 0004 increment 3)
- `USpnWorldStateSubsystem` (game instance subsystem, module `SpnCore`) owns one
  `WorldFacts` store and the `InteractionRegistry` (test door registered). Core types sit
  behind a `TPimplPtr`, so the public header uses Unreal types only (bridge, see open
  point below). `SpnCore` now also depends on `CoreUObject` and `Engine`.
- Slot file (placeholder for M5): `Saved/SpnSaves/<slot>/worldfacts.spnfacts`, WorldFacts
  v1 bytes unchanged. Placeholder slot rule: 1..64 chars, `[a-z][a-z0-9_]*` (no dots: Windows
  drops trailing dots; no separators: no path escape).
- Save: write `<file>.tmp`, full flush, read back and compare, then one atomic rename over
  the old file (`MoveFileExW(REPLACE_EXISTING | WRITE_THROUGH)` on Windows, `rename()` via
  `IPlatformFile::MoveFile` elsewhere). `IFileManager::Move(Replace)` is not used: it deletes
  the target before moving. A failure before the rename leaves the old save untouched.
- Load: missing slot, unreadable file or a `Deserialize` error returns an error status
  (core error text and line) and keeps the current store; only success replaces it and
  broadcasts `OnWorldStateReplaced`.
- Known warnings: `SpnCore` builds without PCH, so each TU including `UObject/Class.h`
  shows 7 C4996 warnings from that UE 5.8.3 header (`UE_INTERNAL`/deprecated overrides in
  `UClass`). The project's shared PCH compiles report the same 7; not from SPN code.

## Test door and E interaction (2026-10-20, Proposal 0004 increment 4)
All PLACEHOLDER: no final animation, UI, input or save design is implied.
- Game module `SupernaturalProt`, folder `Source/SupernaturalProt/Spn/`:
  - `ISpnInteractable`: one function, `SpnUse(User)` (D1: only Use). Call on the authority only.
  - `USpnInteractionComponent`: sphere trace (radius 15 cm, range 250 cm, Visibility) from the
    owner's eyes along its view rotation; uses the first `ISpnInteractable` hit. Reusable on any actor.
  - `ASpnTestDoor`: engine cube panel (100 x 10 x 200 cm) on a hinge at the actor origin, a
    "PLACEHOLDER door" label, `StableId`, `bAuthoredOpen`, `bAuthoredLocked`, `OpenAngle`,
    `SwingSpeed`. It keeps **no state of its own**: open/closed is read from
    `USpnWorldStateSubsystem` after every use (swings) and after every load (jumps, no swing).
    BeginPlay warns about an invalid or duplicate `StableId`.
  - `Spn.SaveWorld [slot]` / `Spn.LoadWorld [slot]` (default slot `debug`): debug console
    commands, not in Shipping builds.
- `ASupernaturalProtCharacter` (the C++ parent of `BP_ThirdPersonCharacter`, via the
  `TP_ThirdPersonCharacter` class redirect): an `Interaction` component, `InteractAction`
  (optional asset) and `DefaultInteractKey` (E). While `InteractAction` is empty, a transient
  Enhanced Input action and mapping context for E are created in code (priority 1), so no
  input asset had to change. Assigning an asset later is optional; it must then be mapped
  in an Input Mapping Context by hand. No gamepad binding yet.
- A refused Use (locked door) logs the feedback id and shows it as on-screen debug text
  (non-Shipping only); player-facing feedback maps the id later.

### Test map (generated)
`/Game/SpnTest/Lvl_SpnDoorTest` is generated by `Unreal/SupernaturalProt/Tools/create_spn_door_test_map.py`
(Unreal Editor Python; the Python Editor Script Plugin is already active in this project, nothing
was installed). Contents: 40 x 40 m floor (engine cube, `MI_PrototypeGrid_Gray`), movable sun,
sky light and atmosphere, one Player Start at the origin facing +X, and three doors 4 m ahead,
3 m apart, panels facing the start and swinging away from it: `door_a` (left), `door_b`,
`door_locked` (right, `bAuthoredLocked`). No game mode override: the project default
(`BP_ThirdPersonGameMode`) spawns `BP_ThirdPersonCharacter`. No other level is touched.
```
UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
    -script="<abs path>/Unreal/SupernaturalProt/Tools/create_spn_door_test_map.py" -unattended -nullrhi
```
An existing map is kept; `-script="<path> --force"` rebuilds it in place (all its actors are replaced).
The `.umap` matches `*.umap` in `.gitignore` (like `Lvl_ThirdPerson.umap`, which was force-added)
and is an LFS file per `.gitattributes`.

### Manual test (Unreal Editor, Windows)
1. Open `/Game/SpnTest/Lvl_SpnDoorTest` (Content Browser > SpnTest).
2. (Doors are already placed; ids and lock are under Details > Spn > Test Door.)
3. Play (PIE). Look at a door, press **E**: it swings open; E again closes it. The locked door
   stays shut and shows `door_locked: placeholder:feedback.door.locked`.
4. Open `door_a`, open the console (default key `` ` ``; on German layouts often `^` or `Ö`,
   see Project Settings > Input > Console Keys) and run `Spn.SaveWorld`. Close `door_a`,
   run `Spn.LoadWorld`: `door_a` is open again at once. Stop and restart PIE: all doors
   start closed; `Spn.LoadWorld` opens `door_a` again, `door_b` stays closed.
   `Spn.LoadWorld missing` reports `SlotNotFound` and changes nothing.
5. The save is plain text: `Saved/SpnSaves/debug/worldfacts.spnfacts` (WorldFacts v1).

Automated: `UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnCore+SpnGame;Quit" -unattended -nullrhi`
(tests `SpnGame.TestDoor.*` and `SpnGame.Interaction.TraceAndUse` spawn doors in a dummy game world;
`SpnGame.TestMap.Layout` checks the generated map: one Player Start above the floor, the three
ids, the lock, distances and facing, and that the default game mode's pawn is the third-person character).
`SpnGame.TestMap.PlayDoors` replays the manual test (E on door_a and door_b, `Spn.SaveWorld`,
close both, `Spn.LoadWorld`; commands go through `PlayerController::ConsoleCommand` like the
in-game console) and checks the world state, the door's angle and the panel's real rotation,
right after the load and 0.5 s later. In the editor it runs in **PIE** (`AutomationOpenMap`
starts a play session); without `-nullrhi` it also saves `Saved/Screenshots/WindowsEditor/SpnDoorTest_1_closed_before_load.png`
and `SpnDoorTest_2_after_load.png`.
Console pitfall (2026-10-20): on German layouts the console key `^` can end up in the command
(`Spn.SaveWorld^` → "Command not recognized"), so nothing is saved and a later `Spn.LoadWorld`
fails with `SlotNotFound`. Check the log: every save/load prints `Spn.SaveWorld OK` /
`Spn.LoadWorld OK` or `... FAILED` with slot, world (PIE/Game) and game instance; each door
logs `world state replaced, now shown open|closed`.
Playing the map as a game (no editor world; injects the Interact action through Enhanced Input):
`UnrealEditor-Cmd.exe <uproject> /Game/SpnTest/Lvl_SpnDoorTest -game -nullrhi -unattended -ExecCmds="Automation RunTests SpnGame.TestMap.PlayDoors;Quit"`
(from Git Bash prefix `MSYS_NO_PATHCONV=1`, or the `/Game/...` argument is rewritten).

## Pickups and inventory (2026-10-22, Proposal 0007) — NOT BUILT OR RUN
Written on the Fedora machine without Unreal; the Core side is tested (198 of 198 in four presets), the Unreal side needs a Windows build first.
- `USpnWorldStateSubsystem`: registers the test pickup; new `TryInteract(Kind, Id, Verb, FSpnInteractDefaults, Holder, Cause)` (the old overload delegates with `DefaultHolder()` = `player.local`); `ReadInventoryCount`, `ListInventory`, `IsValidHolderId`, `IsValidItemId`, `TestPickupKind`, `TestPickupTakenKey`. `FSpnInteractDefaults` carries bool, int and string defaults.
- `ASpnTestPickup` (`Spn/SpnTestPickup.*`): cube with a PLACEHOLDER label, `StableId`, `ItemId`, `Amount`; follows the fact `placeholder.pickup.<id>.taken` (hidden, no collision when taken), re-read after Use and after every load. Single player: the user is always `player.local`.
- `Spn.Inventory [holder]`: prints the counts to the log and as on-screen debug text (non-Shipping).
- Automation tests (`Spn/Tests/SpnTestPickupAutomation.cpp`): `SpnGame.TestPickup.TakeOnce`, `.HoldersAreSeparate`, `.SaveLoadRestoresInventory`, `SpnGame.TestMap.Pickups`.
- Map: `create_spn_door_test_map.py` now also places `pickup_a` (test_item x1, Y -100) and `pickup_b` (test_item x2, Y +100) at X 250. The committed `.umap` is unchanged until regenerated on Windows with `--force`.
- Manual test after regenerating: walk up to a box, press **E** (it disappears), `Spn.Inventory` shows `test_item=1`, take the other (`=3`), `Spn.SaveWorld`, restart PIE, `Spn.LoadWorld`: both gone, `Spn.Inventory` shows 3; before loading, the boxes are back.

## Open (needs developer decision)
- Direct use of Core types from gameplay modules: either keep a bridge layer,
  or add an opt-in export macro to Core headers (empty in CMake). Not decided.
- ~~The Unreal project is not under version control yet~~ Since commit `cd7d0da` the project lives in this repository (`Unreal/SupernaturalProt`).
- Linux UE build of `SpnCore` untested. The Linux CMake build with the doctest change has been run: on 2026-10-22 all four Fedora presets pass 181 of 181 tests without warnings.
