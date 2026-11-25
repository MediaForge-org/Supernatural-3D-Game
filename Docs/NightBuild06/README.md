# NightBuild06 – Integration, Character & Gameplay Stabilization (2026-11-24/25)

Developer order of 2026-11-24 (night build 06). **No commit, push or tag. No test was removed, switched off or loosened.**

Unchanged:
- `Planning/Master`, `Docs/Game/Adult`, the reference images;
- the QA test maps (not opened for saving and not regenerated; they were only made versionable, see A).

`Planning/Master` is not reachable on this machine, so no master search ran. Tonight's work is engineering: a collision mechanism, tests,
tools, and a noise emitter that NightBuild05 had already listed as its next step. **No gameplay value was tuned and no design decision
was taken. New behaviour is opt-in, or it changes nothing for a calm NPC.**

Other files in this folder:
- `PROGRESS.md`: running log;
- `ASSET_MANIFEST.md` + `asset_manifest.csv` / `asset_manifest_summary.json`: assets, licences, LFS, backup;
- `ANIMATION_CLIP_REQUIREMENTS.md`: foot sliding cause and the missing clips.

## 1. Starting state

- Branch `checkpoint/2026-11-15`: the developer's 4 commits `0c93987..d7fe160` (not pushed). Clean apart from `Artifacts/VisualProgress/`.
- The door fix (`FindSweepBlockers` → any-hit query) was already done on 2026-11-24 before this order (static test 11/11, Doors 89/0).
- Baseline:
  - Core `ctest` 640/640;
  - Python 230/230;
  - Unreal headless `SpnGame+SpnCore` 186/186 (editor context, like the checkpoint).
  - A first attempt as a `-game` process with a map gave 177/181. Its 4 movement-map tests did not move the character: a launch-mode
    difference, not a regression.

## 2. What was done (status: implemented / tested / visually checked / prepared / not done)

| Phase | Item | Status |
|---|---|---|
| A | Verified same-disk copy of all content (1646 files, 2.94 GB, SHA-256 equal) | done; **real backup blocked**: no second drive |
| A | Asset dependency report + asset manifest (two new read-only tools, 1646 rows, origin / licence / need / recovery / git status) | implemented, tested (8 Python cases) |
| A | `Content/SpnTest/**` versionable via LFS (`.gitignore` exception for this folder only) | implemented, checked (`git check-ignore`, `check-attr`); **not committed** |
| B | Stale QA text `Spawn.npc_vs_player` (NPCs exist since NightBuild05) | fixed: NOT_IMPLEMENTED → NOT_TESTED in V2 |
| B | **Found:** `SpnQA.CharacterBody` was *Fail* at the automation level in every run since 2026-11-14 (engine ensure), hidden behind PASS checks | fixed, run `20261124-234229` reports *Success* |
| B | **Found:** `-SpnQaBodies=a,b,c` ran only the first body | fixed |
| C | Body collision profiles: Core rules + bridge + `ApplyBodyCollision` + presets `Collision = Keep / FromHeight / Explicit` + test presets `test_160_fit`, `test_195_fit`, `mh_male_193_fit` | implemented, tested (Core 8, headless 2 + 1 door test, rendered: Movement, Doors, Building, Traversal, Access with `mh_male_193_fit`) |
| D | Foot sliding measured in five more scenarios on three bodies; cause and exact clip list | measured, documented; **no fix** (needs clips or an anim-graph change, section 7) |
| E | Jump + ledge assist together (new map test, 24 cases) | implemented, tested |
| E | Door vs a real `ASpnNpc`; opener step with three body profiles | implemented, tested (headless) |
| F | Player footstep noises (calm NPCs ignore them, wary NPCs do not) + NPC hearing pre-check | implemented, tested (Core 2, headless 2, rendered Npc 29/0, World 88/0) |
| G | Route performance measured; perf report gets hitch statistics | measured; nothing to fix found on the route (0 frames > 33 ms) |
| H | Modular body: rendered check of the neutral marker on three bodies (shown when exposed, gone under a data-only vest) | implemented, tested, **visually checked** (screenshots) |
| I | Windows Development build (packaged, starts without the editor, world QA 88/88 inside the packaged exe); two game-target build errors fixed | done, tested |

## 3. Implemented functions (with files)

### 3.1 Body collision profile (Phase C)

Core `Core/include/spn/core/BodyCollision.h`, `Core/src/BodyCollision.cpp`:
- `ResolveBodyCollision`:
  - Keep = today's capsule;
  - FromHeight: half height = (body height + 2 cm) / 2, radius unchanged, crouch scaled;
  - Explicit: clamped to 120–250 cm / radius 20–60;
  - reports how far the head sticks out.
- `FitPassage`: capsule standing, crouched, visible head.

Bridge `Source/SpnCore/Public/SpnBodyCollision.h`, `Private/SpnBodyCollision.cpp`.

`ASupernaturalProtCharacter::ApplyBodyCollision`:
- sets the capsule and the crouched half height;
- the feet stay where they are; mesh offset, `CacheInitialMeshOffset`, `BaseEyeHeight` and the camera base move by the centre shift, so
  everything the player sees stays where it was;
- refuses (nothing changed) without room, while crouched, during a ledge move or a step aside.

`USpnCharacterVisualComponent`:
- presets get `Collision` and `ExplicitCollision`;
- new `CollisionHeadClearanceCm`, `GetLastCollision()` and `WasCollisionRefused()`.

`Config/DefaultGame.ini`: test preset `mh_male_193_fit`.

**Defaults unchanged** (35 / 90). Whether the protagonist uses it stays the developer's decision (0017 "Collision capsule").

### 3.2 Footstep noises (Phase F)

- Core `FootstepNoiseFor` in `NpcReaction.h/.cpp`: walk 4 m / 0.15 … sprint 18 m / 0.6, halved crouched, placeholders. Always below the
  suspicious strength of 0.75.
- `ASupernaturalProtCharacter::TickFootstepNoise`:
  - one `Footstep` noise per 150 cm on the ground;
  - authority only;
  - switch `bEmitFootstepNoise`.
- `ASpnNpc::HandleNoise` skips the wall traces when a noise is beyond its free range. The verdict is the same as `CanHear`, because walls
  only shorten the range.

### 3.3 QA and tooling

- `SpnQaCharacterBodyAutomation.cpp`: ensure fix, comma fix, the modular-marker check and five gait scenarios.
- `SpnQaMovementAutomation.cpp`: text and status of `Spawn.npc_vs_player`.
- `SpnCore.Build.cs` (`bEnableExceptions`) and `SpnMovementNetworkAutomation.cpp` (editor includes inside the guard): game-target build fixes found by packaging.
- `SpnCineSubsystem.cpp/.h`: perf report with `frames_over_33ms`, `frames_over_50ms` and `worst_frames`.
- New tools:
  - `Unreal/SupernaturalProt/Tools/asset_dependency_report.py`;
  - `asset_manifest.py`;
  - `test_asset_manifest.py`;
  - `Tools/package_dev_build.ps1`.
- `qa_inventory.json` and the generated `Docs/Development/Gameplay-QA-Inventory.md`: new tests and gap texts (0 problems).

### 3.4 New tests

| Test | Kind | What |
|---|---|---|
| `Tests/Core/BodyCollisionTests.cpp` (8) | Core | Keep / FromHeight / Explicit, limits, crouch, passage fit |
| `NpcReactionTests.cpp` +2 | Core | footstep loudness, never suspicious, hearing range |
| `SpnGame.Character.BodyCollisionFollowsTheBodyOnlyWhenAsked` | headless | feet, mesh, camera and eye stay, restore exact |
| `SpnGame.Character.BodyCollisionNeedsRoomAndAStandingCharacter` | headless | ceiling refuses growth, shrinking always fits, crouched refuses |
| `SpnGame.Movement.JumpWithLedgeAssist` | headless map | 12 obstacles × press in reach / early |
| `SpnGame.Doors.NpcInTheSwingIsNeverPushed` | headless | real `ASpnNpc` in the swing |
| `SpnGame.Doors.OpenerStepClearsTheLeafForEveryBodyProfile` | headless | default, `test_160_fit`, `test_195_fit` |
| `SpnGame.Npc.PlayerFootstepsAreGameplayNoises` | headless | emission, loudness, crouch, air, switch |
| `SpnGame.Npc.FootstepsMatterOnlyToAWaryNpc` | headless | calm ignores, out of range not heard, wary does not ignore |
| `CharacterBody.<body>_modular_marker` + 5 gait scenarios per body | rendered QA | section 6 |
| `test_asset_manifest.py` (8) | Python | origin rules, needed-by |

## 4. Architecture decisions (engineering; for review)

1. **The capsule follows the body only by opt-in, per preset, and the decision stays open.**
   - Rules in the Core: deterministic, testable on Fedora.
   - Applied in one place on the character, so no system needs a 193-cm special case.
   - It works for any height, male or female profiles alike: 160 cm shrinks, 195 cm grows.
   - Ledge assist and door step-aside already read the capsule at run time and needed no change. Tests: 58/0 Movement incl. ledge, Doors
     89/0, Traversal 22/0, Access 15/0, Building 42/0 with the 193-cm fit capsule.
2. **The visible pose does not move when the capsule changes** (feet, mesh, camera, eye height). Only collision changes, so a profile
   change never pops on screen. It is tested exactly against the same body with the default capsule.
3. **Footsteps are never suspicious on their own.** Making calm NPCs react to walking would be a gameplay change. The existing rule (wary
   NPCs care about harmless noises) gives stealth pressure without that. The loudness values are placeholders.
4. **Third-party and MetaHuman content stays out of git.** Only own, generated test content (`SpnTest`, 4 MB) becomes versionable.

## 5. Measurements (before / after)

### 5.1 Body collision (headless, measured)

| | Capsule radius / half height | Crouched | Head above the capsule top |
|---|---|---|---|
| default (blueprint) | 35 / 90 | 40 | – |
| test_160 → test_160_fit | 35 / 90 → 35 / 81 | 40 → 36 | −20 → −2 cm |
| test_195 → test_195_fit | 35 / 90 → 35 / 98.5 | 40 → 43.8 | +15 → −2 cm |
| mh_male_193 → mh_male_193_fit | 35 / 90 → 35 / 97.5 (config `HeightCm=193`) | 40 → 43.3 | +13 → −2 cm |

Feet, mesh, camera and eye height are identical between plain and fit (tolerance 0.01 cm).

### 5.2 Foot sliding

Median speed of a planted foot in cm/s. Run `20261124-234229`, 1920 × 1080, same lane and input as in 0017.

| Scenario | Placeholder | MH male 193 | MH male 193 fit | 0017 (placeholder, 2026-11-15) |
|---|---|---|---|---|
| walk 180 | 3.8 | 4.8 | 4.7 | 3.8 |
| jog 350 | 31.6 | 30.5 | 30.5 | 31.4 |
| run 540 | 49.2 | 51.5 | 52.5 | 49.9 |
| sprint 740 | 51.4 | 47.7 | 55.1 | 51.1 |
| start / stop walk | 10.2 | 10.1 | 10.0 | 10.3 |
| 90° turn jog | 31.1 | 29.5 | 30.0 | 30.9 |
| **new** walk → jog | 25.5 | 24.8 | 24.8 | – |
| **new** run → sprint | 88.0 | 98.4 | 90.5 | – |
| **new** braking from a run | 59.7 | 59.1 | 59.1 | – |
| **new** jog backwards (S) | 31.8 | 30.5 | 30.7 | – |
| **new** jog sideways (D) | 31.6 | 30.5 | 30.5 | – |

- Ground penetration (lowest ball height above the floor): −1.1 … +1.1 cm on all bodies.
- Mean speeds are identical across bodies (gameplay is unchanged by the body; the check is asserted).
- The fit capsule does not change the animation; the differences are run-to-run noise.
- Worst cases: the **run → sprint transition** and **braking**. Both have no clips (section 7).

### 5.3 Route performance (2560 × 1440, 90 % TSR, `cine/route_perf.json`)

| Shot | 2026-11-14 (fps / 1 % low) | 2026-11-24 (fps / 1 % low) | frames > 33 ms | worst frame |
|---|---|---|---|---|
| main_street | 64.4 / 54.3 | 70.1 / 58.7 | 0 | 20.0 ms (render thread) |
| forest_track | 52.6 / 46.8 | 56.8 / 49.9 | 0 | 21.8 ms (render thread) |
| oak_north_route | 58.0 / 52.0 | 62.6 / 54.4 | 0 | 26.2 ms (render thread) |

No change tonight touches rendering. The higher fps are **not** claimed as an improvement; they come from the state of the machine (driver,
caches). No hitch above 33 ms was found on the route, so there was no evidenced performance problem to fix.

### 5.4 Ledge assist with jump (new test, measured)

| Obstacle (thin, 40 cm deep, jog) | Press in reach (30 cm) | Press early (150 cm) |
|---|---|---|
| 20, 25 cm | walked over, no move | walked over |
| 30, 40 cm | Vault (low step-over), crossed | jump, crossed |
| 45–120 cm | Vault, crossed | ordinary jump, **not crossed** (assist starts only in reach; no catch in the air) |
| 150 cm | refused "high band: no room to stand on top" | not crossed |

The highest capsule centre always stayed within the obstacle height + 25 cm, or the jump apex.

## 6. Test results

### 6.1 After all changes (2026-11-24/25)

| Test | Result | Baseline (checkpoint 2026-11-15) |
|---|---|---|
| Core `ctest` (MSVC Release) | **650 / 650** (incl. the test-name guard) | 640 |
| Python unit tests (12 files) | **238 / 238** | 230 (11 files) |
| Unreal headless `SpnGame+SpnCore` (editor context) | **193 / 193** | 186 |
| Editor build `SupernaturalProtEditor` | succeeded | – |
| Game build (packaging, full compile) | succeeded, **0 compiler warnings** | never built before |
| QA `Full` (run `20261125-002551`) | **PASS 360, FAIL 0**, NOT_IMPLEMENTED 2, NOT_TESTED 2; every automation test *Success* | 342 / 0 / 3 / 1 |
| Packaged build: `SpnWorldQA` inside the packaged exe | **88 / 88**, 3 automation tests *Success* | – |

The four non-PASS entries of `Full` are known and unchanged in substance:
- `SaveLoad.player_position_saved` and `SaveLoad.save_ui_and_autosave`: the console path; the game-flow path is covered by
  `SpnWorldQA.Route`;
- `SaveLoad.inventory_save_load`: covered elsewhere;
- `Spawn.npc_vs_player`: now correctly NOT_TESTED instead of NOT_IMPLEMENTED.


### 6.2 Rendered QA runs tonight

| Suite | Body | Run | Result |
|---|---|---|---|
| Movement (Movement, Turns, Jump, LedgeAssist, Locomotion) | mh_male_193_fit | 20261124-230913 | 58 / 0 |
| Doors (Doors, DoorReach, Buildings) | mh_male_193_fit | series night | 89 / 0 |
| Building (HouseJourney) | mh_male_193_fit | series night | 42 / 0 |
| Traversal (V3Interiors) | mh_male_193_fit | series night | 22 / 0 |
| Access (V3Interiors) | mh_male_193_fit | series night | 15 / 0 |
| Npc | default | series night | 29 / 0 |
| World | default | series night | 88 / 0 |
| Character | none, mh_male_193, mh_male_193_fit | 20261124-234229 | 54 / 0; automation state *Success* (was *Fail* since 2026-11-14) |
| Route perf (`spn_visual -Mode perf`) | default | route_perf_nb06 | section 5.3 |

### 6.3 Not runnable / not run

| Check | Why |
|---|---|
| A real backup on a second medium | no second drive in this machine |
| Fresh-checkout test (clone + `git lfs pull`) | the versionable test maps are not committed yet (commit E) |
| DualSense on hardware, Fedora build, visual judgement of animation quality | not available / not claimed |
| `Planning/Master` consultation | the link is missing on this machine |

## 7. Known defects and limits (open)

1. **Foot sliding** at jog / run (30–50 cm/s) and in transitions (run → sprint ~90, braking ~60 cm/s).
   - Cause: there are no clips at the gameplay speeds and no start / stop / pivot clips.
   - Fix: clips or stride warping (`ANIMATION_CLIP_REQUIREMENTS.md`). Decision needed.
2. **No ledge animation** (unchanged since 2026-11-14): the capsule moves correctly, the mesh plays the jump pose.
3. **An early jump press in front of a 45–120 cm obstacle is a plain jump that does not get over.** This is by design (the assist starts
   only in reach); a "catch in the air" would be a design change.
4. Body collision and footsteps are **not replicated** (co-op prediction is open, as for the ledge assist).
5. The neutral detail marker is placed for the mannequin's bone axes; on the MetaHuman body it sits at the belly. Only a placement value.
6. The headless movement-map tests need the editor context (`UnrealEditor-Cmd`). A `-game` launch fails 4 of them (character does not
   move); that is a launch-mode limit, not a regression.
7. `Lvl_SpnMovementTest` layout constant `StandZ = 97` assumes a half height of 96, while the blueprint capsule is 90. Characters start 7 cm
   above the floor and drop. Harmless; noted, not changed (test layout).

## 8. Assets, licences, backup

See `ASSET_MANIFEST.md`.

- 2.73 GB of content exist only locally:
  - Megaplant 1.13 GB (Fab licence);
  - Fab 0.70 GB;
  - SpnWorld 0.55 GB (own, generated);
  - SpnCharacterPrep 0.38 GB (MetaHuman-generated);
  - ProtagonistV1 21 MB (**not regenerable**).
- Versionable now: `Content/SpnTest/**` (39 files, 4 MB, own work). Third-party content stays out of git.
- **Backup:** a verified copy is at `C:\SpnBackup\NB06_2026-11-24` (same disk; SHA-256 list included). A real backup on a second
  medium is blocked: no second drive exists.

## 9. Packaging (Phase I)

**Result: a Windows Development build exists, starts without the editor and passes the world QA in packaged form.**

- Command: `Tools/package_dev_build.ps1` (new). BuildCookRun Win64 Development, pak.
- Maps: `Lvl_SpnMainMenu`, `Lvl_SpnWorld`, `Lvl_SpnMovementTestV3Interiors`, `Lvl_ThirdPerson`. No QA map was removed or changed.
- Output: `Unreal/SupernaturalProt/Saved/Packages/Dev_20261124-235223/Windows/`
  - 2.55 GB;
  - starter `Start_SpnPrototype.bat` (main menu; "New game" opens SpnWorld).
- Time: 11.9 min (build, cook, stage, pak).

**First attempt failed.** The game target had never been built before; two errors existed only outside the editor:

1. **C4530 in `SpnSaveSubsystem.cpp` / `SpnInventorySubsystem.cpp`**:
   - cause: the core reports `std::filesystem` / standard-library failures as exceptions and the bridges catch them; editor targets force
     exceptions on, game targets do not;
   - fix: `bEnableExceptions = true` in `SpnCore.Build.cs`, this module only.
2. **`Editor.h` not found** in `SpnMovementNetworkAutomation.cpp`:
   - cause: editor-only includes stood before the file's own `WITH_EDITOR` guard;
   - fix: moved inside the guard.

After the fixes:
- the editor build is still clean;
- the full game-target build has **0 compiler warnings** (log `Saved/Logs/package_dev_20261124-235223.log`);
- the cook has only FText warnings ("initialized from FString") for placeholder door names in `V3Interiors`, which is harmless.

**Packaged smoke test:** the world QA ran **inside the packaged executable** (`SupernaturalProt.exe ... -ExecCmds="Automation RunTests
SpnWorldQA"`).
- Before the run, `Saved/SpnWorldGen/qa_points.json` (generator output, test data) was copied into the build's `Saved` folder.
- Results: `SpnWorldQA.Integration`, `.Route` and `.RouteControls` all *Success*, **88 / 88 checks PASS**, exit code 0 (controlled quit).

What that covers in the packaged build:

| Order item | Evidence (check ids) |
|---|---|
| Start without the editor | packaged `SupernaturalProt.exe` |
| Main menu | `new_game_from_the_main_menu_opens_spnworld_fresh`, `load_from_the_main_menu_returns_to_the_world` |
| Load the intended world | the same; staged data JSONs: `decor_doors_loaded` (76 facade doors from the staged content) |
| Player movement | the walked route (both halves) |
| HUD / UI | `map_screen_opens_in_spnworld`, `minimap_shows_spnworld`, prompts `E – Open door`, `touchpad_toggles_the_map` |
| Interactions | `house_a_door_opens_with_e`, key / notebook / flashlight pickups, `decor_door_answers_interact` |
| Save / load | `saved_in_the_world`, `load_restores_the_player_position` (0 cm), doors, items, `load_restores_the_npc_memory` |
| Controlled quit | `-TestExit` → exit code 0 |

Screenshots of the packaged run (day, night, overcast, interior, door) are in the scratch folder of this session. They were looked at:
the world renders with its vegetation and light.

Notes:
- The packaged build **contains Fab / Megaplant content**: it is for local testing only and must not be shared publicly.
- Not done: a manual playthrough with keyboard / controller by a person, and a Shipping configuration.


## 10. Open risks

- **Data loss** (high): the only full copy of `ProtagonistV1` and the world content is on one disk.
- **Licence:** a careless `git add -f Content/...` would commit Fab / MetaHuman content. The `.gitignore` exception is limited to
  `SpnTest/`.
- **Capsule decision:** until the developer decides, the 193-cm test body keeps the 180-cm capsule. Doors below about 195 cm visibly
  clip the head.
- **Co-op:** body collision, footsteps and ledge moves are authority/single-player; replication is a later task.

## 11. Recommended next steps (smallest first)

1. Copy `C:\SpnBackup\NB06_2026-11-24` to an external drive (check it with `SHA256SUMS_Content.txt`).
2. Review and commit (plan below), commit E included. Then a fresh clone + `git lfs pull` + headless run on the second machine.
3. Decide the protagonist capsule: keep 180 cm (design doors ≥ 205 cm), or use the body profile (`Collision=FromHeight`) for V1.
4. Decide the locomotion path: stride warping now (no asset) and / or clips (list in `ANIMATION_CLIP_REQUIREMENTS.md`).
5. Footstep loudness and surfaces (gravel, carpet): tuning decision; the mechanism is in place.

## 12. Git commit plan (for the developer; nothing was committed)

Run on branch `checkpoint/2026-11-15` (or a new branch). After each `git add`:
- check `git diff --cached --stat`;
- for commit E, also check `git lfs status`.

```powershell
cd C:\Projekte\Supernatural-3D-Game

# --- Commit A: Core rules (body collision, footstep noise) + Core tests
git add -- Core Tests/Core
git diff --cached --stat
git commit -m "feat(core): body collision profiles and footstep noise loudness (NightBuild06)"

# --- Commit B: Unreal code + config (body collision, footsteps, NPC hearing pre-check, perf hitch report, QA fixes and new tests)
git add -- Unreal/SupernaturalProt/Source Unreal/SupernaturalProt/Config
git diff --cached --stat
git commit -m "feat(unreal): body collision profiles, player footstep noises, door/NPC and jump/ledge tests, QA fixes (NightBuild06)"

# --- Commit C: tools + QA inventory (asset manifest, dependency report, packaging script)
git add -- Tools Unreal/SupernaturalProt/Tools Docs/Development/Gameplay-QA-Inventory.md
git diff --cached --stat
git commit -m "chore(tools): asset manifest and dependency report, dev packaging script, QA inventory (NightBuild06)"

# --- Commit D: docs (this report, manifest, clip requirements)
git add -- Docs/NightBuild06
git diff --cached --stat
git commit -m "docs: NightBuild06 report, asset manifest, animation clip requirements"

# --- Commit E (optional, decision): version the QA test maps and their material instances in LFS
git add -- .gitignore Unreal/SupernaturalProt/Content/SpnTest
git lfs status                     # all 39 .umap / .uasset must be listed as LFS objects
git diff --cached --stat
git commit -m "test(content): version the QA test maps and SpnTest materials in LFS"

git log --oneline -6
git status --short                 # expected: only Artifacts/VisualProgress/ left
```

