# Intermittent `NpcReaction.neighbour_*` checks — cause and fix (2026-11-13)

Order of the developer (2026-11-13): find the real cause from reproducible NPC positions, sound propagation, wall attenuation and timing; fix
proven defects without weakening the tests. Status: **fixed, verified.** The rules and numbers of the hearing model stay PLACEHOLDER
(NightBuild05); nothing here is canon.

## Symptom

`SpnQA.NpcReaction.neighbour_hears_the_alarm_shout_and_investigates` failed in some `Full` runs (e.g. `Saved/GameplayQA/20261113-040839`,
`-151821`) and passed in others and in every standalone `Npc` run. `neighbour_memory_is_restored_too` failed only as its consequence (a
neighbour that heard nothing remembers nothing).

## Evidence

The game log of every run contains the hearing decision of the Neighbour for the Caretaker's alarm shout (range 25 m, wall factor 0.5):

| Run | Neighbour line | Result |
|---|---|---|
| `20261113-040839`, `-151821` (FAIL) | `did not hear Shout at 8.7 m (2 wall(s), range 6.2 m)` | FAIL |
| `20261113-043417`, `-131449`, `-153300` (PASS) | `heard Shout 8.8 m away (1 wall(s))` | PASS |

The distance is the same; only the wall count differs. The Caretaker raised the alarm at (358, 627/628) in the failing runs and at
(363–366, 634–640) in the passing ones (all 14 recorded positions: y 627–664), depending on where its walk stopped when it saw the player.

A probe on the real map reproduces it without timing: `SpnDiag.Npc.AlarmShoutWallProbe` (`-game` on `Lvl_SpnNpcTest`) counts the walls
from a 2 cm grid of shout positions around those points (x 330–400, y 600–670, shout height = actor + 70 cm) to the Neighbour's ear:

| Before the fix | positions |
|---|---|
| 1 wall: `SPN_WallMiddle_1` | 693 |
| 2 walls: **`SPN_Door_storage_door [Panel]`** + `SPN_WallMiddle_1` | 495 |
| 3 walls: storage door + `SPN_WallMiddle_1` + `SPN_WallFoyerArchive_end` | 108 |

Recorded (358, 627): 2 walls, the storage door's panel hit at (194, 550). Recorded (364, 637): 1 wall.

## Cause

`ASpnNpc::CountWallsBetween` counted every blocking actor on the straight line, including **door leaves that stand open**. The storage door is
authored open (`create_spn_building_test_map.py`: "residents walk through it"); its leaf stands in the room, and the shout line passes within
centimetres of its edge. Whether it grazes the leaf depends on a few centimetres of the Caretaker's stop position, and one extra "wall" halves
the range (12.5 m → 6.2 m) below the 8.7 m distance. An open door is an opening, not a sound barrier: this was a modelling defect, not a test
problem.

## Fix

`ASpnNpc::CountWallsBetween`: an `ASpnTestDoor` that `IsOpen()` is not counted. A closed door still counts as one wall; the door that made the
noise is still never counted. The hearing log line now also names the counted obstacles and the noise position.

After the fix the probe gives 1 wall at 1181 grid positions and 2 real walls (`SPN_WallMiddle_1` + `SPN_WallFoyerArchive_end`) at 115 positions
(x 360–400, y 600–618) — a region none of the 14 recorded alarm positions reached. All three recorded positions: 1 wall. The break-in noise
paths are unchanged (Caretaker 7.0 m / 1 wall, Neighbour 10.9 m / 2 real walls), so `neighbour_behind_more_walls_does_not_hear_it` keeps its
meaning. No test was changed or relaxed.

## Tests

| Test | Result |
|---|---|
| `SpnGame.Npc.OpenDoorLeafIsNotAWall` (new, headless: line through a closed leaf = 1 wall, through an open leaf = 0 although the leaf blocks the trace physically, source door never counted) | PASS |
| `SpnDiag.Npc.AlarmShoutWallProbe` (map probe; asserts the open storage door is never counted) | PASS |
| `SpnGame.NightBuild05.*` (10 tests) | 10/10 PASS |
| `run_gameplay_qa.ps1 -Suite Npc` three times (`20261113-172456`, `-172707`, `-172918`) | 29/0 each; Neighbour heard the shout at 8.9 / 8.8 / 8.7 m through 1 wall |

## Remaining limits (not changed)

- The model counts straight-line obstacles; sound does not travel around corners or through open doorways as paths. Positions behind two
  real walls (the region above) would still not hear the shout — that is the model, not flakiness.
- A door leaf that is swinging counts by its state (`IsOpen`), not by its angle.
