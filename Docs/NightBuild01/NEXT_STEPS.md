# Night Build 01 — next steps (smallest useful first)

## After the completion pass (2026-10-30)

Done since the list below was written: player pose in the save (step 3), router tests (step 8), most of the inventory UI package (step 5), music/effects/UI-scale settings foundation (part of step 6), the teleport-free journey. Still open, in suggested order:

1. **Decide:** integrate Core `ObjectPersistence` or keep world.facts + inventory as the single copy (`COMPLETION_REPORT.md` section 5); confirm `player.pose` against the master (not reachable from the Windows machine); axis preset `CQ-101`.
2. **Movement escalation** (`OPUS_ESCALATION.md`): the five open cases, starting with the per-frame recorder; decide the 30 cm rule and whether a mantle assist is wanted.
3. **Hardware pass:** DualSense and a second keyboard layout against the router, the menus, the map and the inventory (gamepad shortcuts Y / L3 / R3 for sort / view / category are new).
4. **Real audio:** put the first sounds on the Music / SFX classes (or choose another architecture) so the two sliders do something; then re-test persistence across a real restart.
5. **Inventory UI:** redesigned equipment panel, real icons from `USpnItemPresentationSet`, responsive layouts for 16:10 and ultrawide, configurable grid columns.
6. **Real navigation / map data** (unchanged, see below).

---

Proposals for the developer's review; none of them is approved by being listed.

1. **Decide the open points of `ARCHITECTURE.md` §6** — the ones that block real content: axis preset / origin (`CQ-101`), where the real map definition comes from (WorldForge file vs data asset), one waypoint or several, pause semantics in co-op.
2. **Movement 30 cm question** (`OPUS_ESCALATION.md`): log the per-frame lift series with `spn.MoveDiag 1`, then either fix the slide path or decide the intended limit (e.g. `MaxStepHeight` 20 cm). Then investigate `late_jump_ledge_45_jog`, `late_jump_ledge_40_run` and the intermittent `ledge_80` case with the same recorder.
3. **Save the player's position** (new optional section, e.g. `player.pose`, with the level id): the only reason *Continue* is not a full "continue where I stood". Needs a save-format decision (CLAUDE.md §11).
4. **Integrate `ObjectPersistence`** (Core already has it) for world objects, so containers and pickups come from saved records instead of the scenario spawner; then delete the hard-coded scenario in `USpnSessionSubsystem::SpawnScenario`.
5. **Inventory UI package M:** grid/list toggle, sorting, category filter, icons from `USpnItemPresentationSet` where they exist, improved equipment layout, responsive layout check at 16:10 and ultrawide.
6. **Audio buses** (master / music / effects `USoundClass` or Submix) so the settings can offer music and effects sliders with a real effect.
7. **Hardware pass with a DualSense:** menus, map (stick pan, triggers zoom), HUD hints; confirm Triangle for the map does not collide with the combat design (`Kampfsystem 06`), or choose another key.
8. **Router unit tests:** key-by-key priority (console, text, inventory, pages) with injected `FKeyEvent`s in a created local player; today the behaviour is verified only through the scripted run.
9. **Real navigation:** when a road graph/navmesh provider exists, produce `Route{source = External}` through `spn::core::Route` and show the polyline on the map (the Core already validates and measures routes).
10. **Map art:** tile streaming of a baked or procedural background per region once WorldForge delivers data; the view already asks only for its own rectangle.
