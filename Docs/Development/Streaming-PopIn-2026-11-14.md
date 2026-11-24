# SpnWorld — texture / vegetation pop-in (night build 2026-11-14)

Observation from the developer's play test: surfaces and details partly become sharp late. Measured on the RTX 3090, 2560 × 1440,
standard 90 % TSR, editor game binary (`UnrealEditor.exe -game`, the way `Tools\play_spnworld.ps1` runs it), lighting "day".

## 1. Tools added for the diagnosis

- `USpnCineSubsystem` perf mode now samples the streaming state every frame and writes per shot: `tex_required_max_mb`,
  `tex_over_budget_max_mb` / `_share` (texture/mesh mip streamer), `nanite_quality_min` (Nanite streaming quality scale, below 1 = the Nanite
  pool is too small), `compiling_max` (assets/shaders still compiling, editor builds), and per run `tex_pool_mb`, `tex_max_ever_required_mb`,
  `nanite_pool_mb`, `warmup_s`. Spec option `"cold": true` skips the wait for compilation (what a player sees right after loading); shot
  option `"warmup": true` runs the new `SpnWorldWarmup::WarmUpView` at the cut.
- `Tools/spn_world/make_streaming_probe.py` → `cine/streaming_probe.json` (+ `_cold`): seven identical runs — town centre, Oak Street,
  four fast turns in the town, a sprint of the player character from Oak Street onto the pine track, cabin and pond, the pine track flown
  from the forest to the town (forest → town transition), four fast turns in the forest.
- `Unreal/SupernaturalProt/Tools/texture_streaming_audit.py` (read-only): streaming settings of every texture the level references.

## 2. Findings (each system separately)

| System | Measurement | Verdict |
|---|---|---|
| Texture mip streaming (pool) | pool 1000 MB (engine scalability default); required at most **394 MB** (forest → town), never over budget in any shot, cold or warm | not the cause |
| Texture streaming settings | 106 textures: none NeverStream, none without mips, no LOD bias; 24 of 29 Fab textures and 10 Megaplant textures are 4K | correct |
| Steady-state mip choice | stills after settling, default vs `r.Streaming.FullyLoadUsedTextures=1`: SSIM 0.97–0.998, ffmpeg blur metric equal (difference = foliage motion/TAA noise) | the streamer asks for the right mips |
| Virtual texture streaming | only Megaplant textures are VT (10); the three physical pools (190 MB) had at most **57 of 7225** tiles allocated | not the cause |
| Nanite streaming | quality scale stayed **1.0** in all shots (512 MB pool) | not the cause |
| Mesh LOD / editor compilation | "Waiting on static mesh … being ready" at level load; at most 6 assets still compiling in the first 0.6 s of the cold run | editor-only, small |
| **Landscape grass (vegetation)** | after a level start or any jump of the view the ground cover (ferns, grass, weeds) is **missing in the first frame** and appears over ~1–2 s; the grass is created around the view component by component (`grass.MaxCreatePerFrame` 1) | **cause 1** |
| Exposure / Lumen / TSR settling | the first frame in the town is flat, over-bright and lacks fine road detail; it does not change with fully loaded textures nor with unlimited component registration (A/B) | **cause 2, not streaming** |
| Moving / fast turns | sprint and 4 turns in 6 s: no over-budget, no Nanite scaling; p99 18–27 ms | no streaming deficit measured |

## 3. Fix

`Spn/World/SpnWorldWarmup.{h,cpp}` — `SpnWorldWarmup::WarmUpView(World, Location, TimeLimit, FadeSeconds)`, called once by the session
(`USpnSessionSubsystem`) when a new game starts (player / player start) and after a saved pose was applied (the view jumped):
1. editor builds only: finish pending mesh/texture/shader compilation (packaged builds have them cooked);
2. tell the texture streamer the coming view (location, screen size, field of view; `AddViewInformation`, kept 4 s);
3. build the landscape grass around the location synchronously (`ULandscapeSubsystem::RegenerateGrass(false, true, {Location})`);
4. `StreamAllResources` with a 3 s limit;
5. fade the player camera in from black over 1.2 s (covers exposure/Lumen/TSR settling, as a load screen would).

No streaming is disabled, no top mips are forced, no pool is enlarged (none is short), nothing depends on a 24 GB card. Normal play after
the first second is unchanged. Measured cost: compile 0.00–0.03 s, grass 0.02–0.03 s, streaming 0.00–0.37 s per call.

Tried and removed again (no measurable effect, not kept): lifting `r.Streaming.NumStaticComponentsProcessedPerFrame` during the warm-up.

## 4. Before / after (identical cold runs, first image ≈ 12 frames after the cut, SSIM against the settled image)

| Place | before: image 0 / 1 / 2 | after: image 0 / 1 / 2 |
|---|---|---|
| Pine track → town (forest) | 0.728 / 0.894 / 0.936 | **0.916** / 0.933 / 0.935 |
| Town centre (level start) | 0.743 / 0.964 / 0.981 | 0.761 / 0.923 / 0.977 (exposure/Lumen; covered by the fade in the game) |
| Cabin and pond | 0.654 / 0.746 / 0.778 | **0.723** / 0.776 / 0.783 |

(The cabin reference differs in water/cloud motion, so its absolute SSIM stays lower; the change between runs is what counts.)
Video: `Artifacts/VisualProgress/21_Streaming_BeforeAfter.mp4` (stills of both runs side by side, one per second).

## 5. Open

- The fade and the warm-up in the real game flow (main menu → New game / Load) are exercised by `SpnWorldQA.Route`; the visual effect there
  is not recorded separately.
- Teleports by `ASpnPlayAreaGuard` (returning a player who left the area) do not call the warm-up (rare; could be added).
- Grass at the cull edge (40–70 m) fades in by design (start/end cull distances); not changed.
- A packaged (cooked) build was not tested; there the editor compilation step does nothing.

## 6. Longer continuous movement (2026-11-14 afternoon, developer request item 5)

Spec `cine/streaming_long.json` (written by `Tools/spn_world/make_streaming_long.py`), run `streaming_long` at 2560 × 1440, 90 % TSR, day:

| Shot | What | Frames | avg fps | p99 ms | worst frame ms | textures required (max) | over budget | Nanite quality (min) | frames with streaming pending |
|---|---|---|---|---|---|---|---|---|---|
| long_sprint | the character sprints without a cut: route start → Oak Street → the whole pine track → cabin → back (733 m, 104 s) | 5 979 | 57.7 | 20.4 | 44.8 | 376 MB of 1000 | never | 1.0 | 0.2 % |
| long_flight | one camera flight at about 15 m/s (vehicle speed) over main street, Oak Street, the north end and the pine track (1 017 m, 68 s) | 4 101 | 60.5 | 20.7 | 46.5 | 394 MB of 1000 | never | 1.0 | 0 % |

Results:
- The pools never run short on a long run either: textures at most 394 of 1000 MB, Nanite quality 1.0 throughout.
- The few pending frames of the sprint come at about 61 s. In the same shot the shader compiler worked (up to 22 jobs): this run uses
  the editor binaries in `-game` mode, which compile missing shader permutations at runtime. A packaged build ships precompiled
  shaders, so this is a test-setup effect, not texture streaming.
- One frame of about 45 ms per shot. It was not traced further: the perf output keeps no per-frame list, so the count of frames above
  33 ms is not known.
- Not checked by eye: the perf mode records no images, so this run measures the streaming systems only. Visual pop-in during long
  movement is not covered by this run.
