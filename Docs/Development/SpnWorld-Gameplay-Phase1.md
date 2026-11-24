# SpnWorld — gameplay integration, phase 1 (2026-11-13)

Developer order of 2026-11-13: a small, coherent, reproducibly testable gameplay route in the existing 1260 × 1260 m placeholder world. No map
enlargement, no rebuild of the town, no new complex systems, existing systems only. Status: **implemented and tested**; open points in section 7.

Everything here is PLACEHOLDER engineering (ids, positions, furniture, numbers). No location, character or story element is canon.
Master consultation (CLAUDE.md §11): `Planning/Master` is not reachable on this Windows machine; no master search was run. Nothing in this
work decides an open master question (world size, town layout, story, NPC design); the route only places existing test systems.

## 1. What was added

| Part | Implementation | Source |
|---|---|---|
| World boundary | four engine `BlockingVolume` walls 30 m inside the landscape edge (±600 m), deep in the edge forest; the highway — the only road that leaves the map — is closed about 14 m before the wall by a rockfall (three staggered rows of scanned boulders over road and verges, three fallen trunks) at both ends; `ASpnPlayAreaGuard` round the whole area as the second barrier (returns a pawn that got out to the nearest of four reset points) | `Tools/spn_world/gameplay.py` `boundary()` |
| Navigation | 7 `NavMeshBoundsVolume`s: one over the route area at the north end of Oak Street (the two houses, the street, the start), six along the pine track to the cabin. The mesh is built at level start (`RuntimeGeneration=Dynamic`, as on the NPC test map). No navigation elsewhere | `navigation()` |
| NPCs | two `ASpnNpc` (existing class, ids 2001 "Resident", 2002 "Walker") with patrol routines: the street between the houses; the first 50 m of the pine track | `npc()` |
| Enterable houses | `oak_n7` (unlocked) and `oak_n6` (locked, key `key.spnworld.oak_n6`): real `ASpnTestDoor`, inner wall faces, floor, ceiling, table, stools, shelf, rug, ceiling lamp with `ASpnLightSwitch`, interior exposure volume. All other houses unchanged (the random sequence of the house generator is preserved) | `buildings.py` `house_interior()`, `gameplay.HOUSES` |
| Key and items | `ASpnTestKey` and an `ASpnWorldPickup` notebook on the table of `oak_n7`; an `ASpnWorldPickup` flashlight in `oak_n6`. The pickups are real inventory items; "examining" is the existing inventory screen (name and description) | `gameplay.build()` |
| Start | the level's `PlayerStart` is the route start on Oak Street, 70 m south of the houses, facing north | `build_world.py` |
| Playing it | `Tools\play_spnworld.ps1`: main menu → New game opens SpnWorld (existing option `-SpnGameplayMap=`); save/load through the pause menu | new script, no new game code |

Route: start → the Resident on the street → house A (E: door; E: key; E: notebook) → house B (E unlocks with the key, E opens; E: flashlight)
→ north on Oak Street onto the pine track (the Walker) → the cabin (door, light switch, as before).

## 2. Defects found and fixed on the way

| Defect | Cause | Fix |
|---|---|---|
| NPC patrol legs "made no progress for 8 s" although the NPC walked | `RoutineStuckSeconds` was the whole leg time, not time without progress: every leg longer than 12 m was given up. The test maps only have shorter legs | `ASpnNpc::RunRoutine`: the timer restarts whenever the NPC got 25 cm closer; counters `GetPatrolLegsCompleted/GivenUp`. After the fix: 12 legs completed, 0 given up per NPC, no warning |
| World pickups showed the raw id (`spn_test:item.notebook`) in prompt and HUD note | `SpnWorld::ItemDisplayName` looked up `item.<id>`; the text table has `item.<id>.name` | looks up `.name` first; now "Test notebook" |
| `SpnWorldQA.Integration` cabin checks failed after the houses got room volumes | the test took the last interior volume of the level | takes the one nearest to the cabin door |
| First boundary check passed without reaching the wall | a tree stopped the player 9.6 m before it | the generator picks a start 7 m before the wall on a line free of trunks and rocks; the check requires "at the wall and stopped" |
| First rockfall was five small rocks that could be walked round | — | three rows over ±15 m, about three to four times the scan's size |

Not possible: spawning a `RecastNavMesh` actor from the build commandlet ("could not spawn"); not needed, the navigation system creates the
data at level start.

## 3. Tests

`Tools\run_gameplay_qa.ps1 -Suite World` runs `SpnWorldQA.Integration` (29 checks, unchanged scope) and the new `SpnWorldQA.Route`
(`Spn/Tests/SpnQaSpnWorldRouteAutomation.cpp`, 37 checks + 1 reported gap). The route is played with key events (W, E) like a player; nothing
sets a door, item or NPC state directly. Points come from `Saved/SpnWorldGen/qa_points.json` ("gameplay"), written by every world build.

| Area | Checks (run `20261113-222429`: 66 PASS, 0 FAIL, 1 NOT_IMPLEMENTED) |
|---|---|
| New game | started through the session (catalog, empty player container, game flow); on the ground at the player start |
| Repeated generation | every door / key / pickup / NPC id exactly once, 1 guard, 4 boundary volumes, 7 navigation volumes — after five complete world builds with the gameplay module on 2026-11-13 (a sixth aborted on the RecastNavMesh attempt, section 2) (the build deletes and recreates the level; counts from the level itself: 3 doors, 1 key, 2 pickups, 2 NPCs, 3 switches) |
| NPCs | both on the ground and on the navigation mesh (no fallback steering), walking; patrol legs completed; the Resident sees the player; the player cannot walk through it (centre distance = sum of the capsules); no alarm, no flight for a passer-by |
| House A | prompt, door opens with E, inside the room volume, key prompt, key taken, notebook in the inventory with name and description |
| House B | locked, prompt "unlock" with the key held, E unlocks, E opens, flashlight taken, back outside |
| Way on | Oak Street north to the pine track point |
| Save / load | saved through the session; leave to the main menu; Load from there returns to SpnWorld: pose (0 cm off), both doors open and unlocked with the leaf at 90°, key held and hidden, both pickups taken and hidden, inventory counts, NPC memory count (4 = 4), NPCs back on the navigation mesh, the loaded door still works |
| Boundary | wall in the edge forest: stopped at the wall face while pushing (y 59 965 of 60 000); rockfall: stopped 24 m before the wall; guard: a pawn placed outside is returned |
| New game from the menu | main menu → New game opens SpnWorld fresh: at the route start, house B locked again, key not held, empty inventory |
| Minimap | **NOT_IMPLEMENTED**, see section 6 |

The door step-aside is used on the route as everywhere (`SpnTestDoor`), no special case.

Acceptance after the last build (2026-11-13, all with the final code):

| Check | Result |
|---|---|
| Core (ctest, build/msvc Release) | 621/621 |
| all headless `SpnGame.*` | 95/95, no crash |
| QA `World` (twice) | 66 PASS / 0 FAIL / 1 NOT_IMPLEMENTED (minimap) each |
| QA `Full` (`20261113-224436`) | 290 PASS / 2 FAIL / 0 BLOCKED — the two known `Jump.late_jump_ledge_*` checks (ledge assist missing), unchanged |
| QA `Doors` / `Building` / `Npc` | 89/0, 42/0, 29/0 |
| `git diff --check`, protected folders, QA test maps | clean / untouched / untouched (only content change remains `ABP_Unarmed.uasset` from 0013 S1) |

## 4. Performance before / after (2560 × 1440, standard 90 % TSR, `cine/route_perf.json`, same three runs)

| Shot | before fps / 1 % low | after fps / 1 % low | GPU ms before → after | game thread ms before → after |
|---|---|---|---|---|
| main_street | 64.7 / 50.6 | 64.2 / 53.5 | 13.8 → 13.9 | 3.9 → 4.3 |
| forest_track | 53.1 / 47.8 | 52.6 / 46.7 | 17.5 → 17.7 | 3.6 → 4.0 |
| oak_north_route (the route) | 58.6 / 51.5 | 57.9 / 51.7 | 15.6 → 15.8 | 3.5 → 4.0 |

Cost of the integration: 0.5–0.7 fps (about 0.4 ms game thread: two NPCs, runtime navigation; 0.1–0.2 ms GPU). One run each; the
run-to-run drift measured earlier on this machine is about 0.5 fps, so the difference is at the edge of the measurement.

## 5. Video

`Artifacts/VisualProgress/20_SpnWorld_Gameplay_Route.mp4` (154 s, 188 MB, gitignored): the recorded QA run `20261113-215306`, six labelled parts
(new game and NPC, house A, house B, way to the track and save, boundary, main menu and load); walking parts at double speed. The QA
overlay text is visible. Recorded before the patrol fix and before the "New game from the menu" check were added.

## 6. Minimap in SpnWorld

**Update 2026-11-14 (night build): implemented without a save-format change.**
- `Tools/spn_world/map_data.py` writes `Content/SpnWorld/Data/SpnWorldMap.json` from the layout. It is deterministic (byte-identical
  across builds) and is called in phase H. Contents: 15 road segments split at junctions, 54 building footprints with entrances, the pond
  as a water area, the creek as a stream, POIs, and the regions valley and town.
- `USpnMapSubsystem::InstallGeneratedRegion` builds the Core definition, the geometry and a `NavigationSession` from it (road routing).
- `USpnSessionSubsystem::EnsureMap` installs it for `Lvl_SpnWorld`; every other level keeps the test definition.
- The map save section only stores state (discovered ids, markers in metres), so the format is unchanged. The ids of the old test
  definition are in the file as HIDDEN POIs, so SpnWorld saves made before still load.
- `SpnWorldQA.Route` `minimap_shows_spnworld` is now a PASS/FAIL check (bounds 1260 m, player inside, no old test POIs visible, a road
  route to the cabin) and `map_screen_opens_in_spnworld` opens the map screen.
- Open, NOT tested: a SpnWorld save inspected from a menu that targets another level (started without `-SpnGameplayMap`) is judged
  against the test definition. Its SpnWorld ids are then unknown, so the save subsystem should refuse it as damaged or mismatched.
  Expected from the code, not observed.
- **Tested on 2026-11-14 (afternoon)** with `SpnCore.Map.OldSpnWorldSaveWithGeneratedMap` (headless):
  - No SpnWorld save from before the generated map exists on this machine. Every SpnWorld save was written later, and the older saves
    from 30.10. belong to the test maps.
  - So the test writes the old save the way the build of 2026-11-13 wrote it in SpnWorld: the test definition installed, a waypoint on
    one of its POIs, discovery from walking.
  - The current build (the generated map) loads it: the waypoint is back at the same place and the old discovery is kept. The old POIs
    stay hidden; none is shown on the SpnWorld map.
  - The reverse case is confirmed. A current SpnWorld save judged against the test definition is refused as a whole and changes
    nothing (`SectionRejected: world.map: UnknownEntry spnworld:poi.cabin`).
  - The menu then reports "die gespeicherten Daten wurden als ungültig abgelehnt" ("the saved data was rejected as invalid"), not
    "belongs to another map". A clearer message would be a small UI change (proposal; no format change).

Original analysis (2026-11-13):

Measured by the test: the map subsystem holds the Core TEST definition ("test district", bounds −200..200 m = 400 × 400 m); at the save
point the player is at (−124, −322) m, outside it. The minimap and the map screen therefore show the test district, not SpnWorld.

What an integration needs (not started; `SpnMapSubsystem.h` says a real definition "arrives through SetDefinition-style loading later"):
1. a map definition of the generated world (bounds 1260 × 1260 m, the seven roads, 54 building footprints, creek, pond) written by the
   world generator from the same `layout.py` data it builds from;
2. a way to install it for this level (today only `InstallTestDefinition` / `InstallSyntheticRegion` exist) and the world frame
   (Unreal cm ↔ map metres) for SpnWorld;
3. then markers / discovery / waypoint routing would work with the existing map features.
This touches the map data format, which is a persistence format (CLAUDE.md §11: master consultation first) — a developer decision.

## 6a. Doors that are part of a building facade (developer decision 2026-11-14, item 2)

Decision: no permanent markers on all decorative doors; check a light, context-dependent answer when the player tries one; interactive
doors must stay clearly recognisable.

- The generator now records every door it builds into a mesh (`archkit.door`). That is 76 doors on 2026-11-14: houses, shops, motel,
  church, diner, garage and others. The barn's big doors are not drawn with `archkit.door` and are not included.
- They are written to `Content/SpnWorld/Data/SpnWorldDecorDoors.json`: leaf centre, half width, height and outward direction.
  Deterministic, written by `buildings.build`.
- `USpnDecorDoorSubsystem` (Lvl_SpnWorld only) loads them. When Interact (E / Square) or Force (F / Square held) finds nothing usable,
  `USpnInteractionComponent` asks whether the view meets such a door. The test is the door plane hit within 20 cm of the leaf, or the
  door within 30° of the view, inside the interaction range, with a clear line of sight.
- The answer is one short HUD note: "Diese Tür lässt sich nicht öffnen." (Interact) or "Diese Tür gibt nicht nach." (Force). It is
  not repeated within 1.5 s at the same door. There is no prompt and no marker; looking at such a door shows nothing.
- Interactive doors keep their prompt ("E – Tür öffnen", "Aufschließen", "Tür (verschlossen)").
- Tested in `SpnWorldQA.RouteControls`:
  - a facade door (the church door) has no prompt;
  - E and F there are answered with that door's id;
  - the door of house A still shows its prompt.
- Also: `Content/SpnWorld/Data` is now staged for packaged builds (`DefaultGame.ini`). This covers the map data from the night build as
  well; no packaged build has been made.

## 7. Open / not done

- Minimap (section 6).
- NPCs are the existing placeholder (cylinder + sphere, no animation); they do not open doors and have no navigation outside the two areas.
  NPC positions are not saved (existing limit); their memory is.
- Only two houses and the cabin are enterable; all other buildings are closed shells.
- The boundary walls are invisible; only the highway ends are dressed (rockfall). In the forest the wall is met between trees — tested at
  one place in the north, not along the whole perimeter. Steep terrain at the edge is not additionally blocked.
- The rockfall is a visual placeholder (bright scanned rocks on a clean road, no debris, no signs).
- Items cannot be "examined" in the world; only the inventory screen shows name and description (existing behaviour).
- `SpnWorldQA.Route` inherits the already loaded level from `SpnWorldQA.Integration` in a `-Suite World` run (it resets the world state and
  starts a new game through the session; the later menu → Load and menu → New game are real level changes).
- `Tools\play_spnworld.ps1` itself was not started by hand; its path (main menu, `-SpnGameplayMap=`) is what the last test check exercises.
- No human playtest.
