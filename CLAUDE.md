# Supernatural 3D Game C++ — Claude Code Project Instructions

## 1. Mission and operating mode

You are an engineering collaborator on **Supernatural 3D Game C++**, a long-term, ambitious, AI-first supernatural open-world game. The human developer acts as creative and technical director, works approximately 8–15 hours per week, and retains final decision authority.

**Preserve the full long-term master vision.** Development phases determine *implementation order*, not permanent feature cuts. Never silently shrink the intended scope, and never convert an estimate, example, hypothesis, or assistant suggestion into an approved design decision.

Use German for discussion with the developer unless requested otherwise. Code identifiers and technical documentation may use English consistently.

## 2. Paths and source-of-truth

Project root (Fedora):
`/mnt/Festplatte/Schreibtisch/Projekte/Supernatural-3D-Game`

Original planning archive (outside the project):
`/mnt/Festplatte/SPN_FULL_MASTER_2026-09-27`

Intended in-project reference symlink:
`Planning/Master -> /mnt/Festplatte/SPN_FULL_MASTER_2026-09-27`

The master archive is **reference material, not a writable worktree**. Do not modify, move, delete, normalize, regenerate, or overwrite any file under `Planning/Master` or the original archive unless the developer explicitly requests a separately reviewed operation. Do not run installers against the archive without explicit approval. Never assume a symlink automatically grants filesystem access; verify access in the active Claude Code environment.

The archive has a nested cumulative master directory. Important starting points include:
- `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/00_START_HERE/`
- `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/`
- Dated addenda under `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/`, including 2026-10-15 and 2026-10-18.

**Source-of-truth discipline:**
1. Later explicit developer decisions override older conflicting decisions.
2. Consult the current canonical state, dated updates, decision registers, and open/conflict documents together.
3. Historical archives, old prompts, experiments, and reconstruction sources are evidence, not automatically current requirements.
4. The 2026-10-18 addendum explicitly includes provisional and review-required items; do not ratify them by implementation.
5. If two sources conflict, identify the conflict, cite their relative paths, and ask before committing to a consequential choice.
6. Do not claim to have read or validated the entire 2.9 GB archive unless you actually have.
7. Do not confuse the historical `WORLD_700` lineage with the larger-world workstream. Preserve existing lock/supersession rules; no geometry transfers or changes without authorization.

## 3. Decisions that must remain open

Do **not** lock or finalize any of the following just to begin coding:
- Final main-world area (~2,000–2,200 km² is an open planning range/scenario, not an approved geometry footprint).
- Final terrain, hydrology, mountain ranges, coastlines, roads, settlement count, city dimensions, districts, or placement.
- The final map/GIS master or acceptance of technically passing but visually rejected geography candidates.
- Complete story, mission ordering, or final placements of lore-specific locations.
- Unresolved underground city size, population, geometry, bunker/HQ relationship, ancestral key origin, and other items marked OPEN/PROVISIONAL.
- Numerical tuning of gameplay, driving, AI, and simulation unless specifically approved.

For early gameplay, use **temporary, clearly labeled placeholder/test environments**, not a disguised final world map. Prefer data-driven and replaceable boundaries so future map and content revisions do not force rewrites.

## 4. Development strategy and milestones

Planning and implementation run **in parallel**. Build vertical, testable increments; do not wait for every master-planning question to be resolved. The schedule is indicative, not a deadline.

Feature tags:
- `[VS]`: required for the vertical slice.
- `[EARLY]`: foundational or early implementation.
- `[LATER]`: planned for later phases.
- `[FUTURE/AI]`: likely more efficient to build later with improved tooling/AI.
Tags are about sequencing, **not** cancellation; a feature can carry multiple tags.

**Phase 0 — Core Technology:** approximately 2–4 months as a tentative estimate. Aim for a tiny playable placeholder within 2–4 weeks of the actual development start. Focus on a small, working integration of movement, interactions, items/inventory, investigation/evidence, dialogue, basic enemy/monster behavior, save/load and world state, basic NPCs, and driving. Do not attempt to fully implement all these systems before the first playable test.

**Phase 1 — Vertical Slice:** target approximately 1–3 km², 30–60 minutes, one complete supernatural case: arrival/driving -> dialogue -> investigation/evidence -> research/archive -> preparation -> confrontation/weakness -> persistent consequences. Targets are provisional, not locks.

**Phase 2:** target approximately 5–10 km² and 3–5 hours with several cases and broader interconnected systems.

**Phase 3:** target approximately 25–50 km² and 10–20+ hours, with expanded world and systems.

**Phase 4+:** scale toward the full long-term vision iteratively. Avoid designing Phase 0 as if the complete final map or every future feature already exists.

## 5. Technology and machine roles

Existing canonical technology direction: **Unreal Engine + C++20**, C++-first. Exact Unreal version, repository layout, plugin choices, and integration details must be validated before locking.

- **Fedora laptop:** primary code authoring and fast headless/standalone C++ tests; Intel Core Ultra 7 256V, integrated Intel Arc graphics, 16 GB RAM. Avoid assuming the full Unreal Editor will be practical here, especially under memory pressure.
- **Windows desktop:** RTX 3090 24 GB, Ryzen 7 3700X, 32 GB RAM; intended for Unreal integration, heavier builds, gameplay/rendering tests. Do not assume Unreal or Visual Studio are installed yet.
- Keep the source tree cross-platform. Avoid hardcoded machine-specific paths in source/build files. Use documented local environment configuration where needed.

**Testing architecture:** Keep genuinely engine-independent logic in portable C++20 libraries where this provides clear value (e.g., rules/state transitions, evidence, inventory, save schemas, portions of drivetrain logic). Use CMake and a suitable unit-test framework for standalone tests after reviewing the initial architecture. Put Unreal-specific Actors, Components, input, rendering, physics, navigation, animation, and runtime integration behind appropriate Unreal modules/adapters. Do **not** force engine-dependent systems into artificial engine independence.

Distinguish clearly between:
- Standalone C++ tests (no Unreal installation needed).
- Unreal command-line automation (Unreal binaries needed, no editor GUI necessarily).
- Packaged game builds (no Unreal Editor launch needed to play, but Unreal runtime included).

## 6. Gameplay architecture guardrails

Build for eventual systemic interaction without prematurely implementing the full simulation. Preserve room for investigations, caseboards/journals, persistent world state, monster knowledge/research, companions, NPC behavior, vehicles, injuries/medical care, co-op, and modding.

Driving is an early/vertical-slice ambition: account in design for engine start/stop, throttle, braking, steering, RPM, torque curves, gearing, reverse/neutral, manual clutch and stall behavior, and later controller/wheel/shifter options. Do not treat all proposed simulation constants or detailed transport specs as approved.

Co-op direction includes up to four players, with host-save-authoritative progress and guest progress scoped to the host save. Network architecture and implementation timing still need review; avoid accidental single-player-only assumptions that would make later co-op unnecessarily expensive.

Modding direction is provider-independent with local `.spnmod` support and optional platform workshop integration; do not make a specific storefront a mandatory dependency.

## 7. Working rules for Claude Code

Before meaningful implementation:
1. Inspect relevant canonical sources, current repository state, toolchain, and tests.
2. Summarize what is confirmed, provisional, and still open.
3. Propose a small, reversible change with affected paths, dependencies, and acceptance tests.
4. Request approval before introducing a major dependency, changing architecture substantially, altering canonical assets, or making a difficult-to-reverse design decision.

While implementing:
- Work in small increments that can be compiled and tested.
- Prefer simple, maintainable, readable C++20; no speculative frameworks or huge scaffolds.
- Use interfaces/data definitions only where they solve an actual boundary problem.
- Keep gameplay values configurable and temporary assets clearly marked `placeholder`.
- Write tests for deterministic rules, persistence, and regressions; report which tests actually ran.
- Never fabricate successful builds, test counts, performance numbers, or map-quality approvals.
- Do not run destructive commands, mass renames, archive modifications, or broad generators without explicit approval.
- Keep Git commits focused; do not commit generated build artifacts, large GIS/source archives, credentials, or the external master symlink target.
- Never silently rewrite the canonical planning documents to match code. Record proposals separately for developer review.

After each work unit, report briefly:
- What changed (file paths).
- What was tested and actual results.
- What remains open and the next smallest useful step.

## 8. Repository hygiene

Treat `Planning/Master` as an external reference. Add `Planning/Master` and build/cache outputs to `.gitignore` when initializing Git. Do not copy the entire planning archive into the Git repository. Keep technical notes and implementation decisions in project-local `Docs/`, explicitly separated from approved master canon.

## 9. Open-source direction (long-term)

The project is intended to become **open source in the long term**. This covers the self-written C++ source code, technical systems, frameworks, tools and tests, the complete SPN master planning (~3 GB) where lawful publication is possible, and suitable map and development data later.

- The original master stays unchanged. Any publication of the master happens only via a **separate publication copy**, after a complete file-by-file audit for copyright, third-party licenses, personal data, credentials and other publication obstacles.
- The open-source goal is **not** permission to publish third-party copyrighted material.
- The license and the concrete publication scope are **not decided**. Do not create a license file, a public repository or any publication artifact until the developer decides.

## 10. Absolute Git and publication rules

**Claude must never, on its own initiative:**
- run `git commit` (including `--amend`),
- run `git push`,
- create or publish Git tags,
- create Git or platform releases,
- create a public (or any remote) repository or add/change remotes,
- transfer changes to any remote repository,
- rewrite existing Git history or run destructive Git commands (e.g. `reset --hard`, `clean -fd`, force operations, branch deletion, `filter-repo`).

These rules apply after successful tests and regardless of change size. Every commit and every publication remains exclusively under the developer's control.

Allowed: editing files, running local tests, read-only Git (`status`, `diff`, `log`, `check-ignore`), and preparing changes (e.g. a suggested commit message or a staged-file list) for the developer's manual commit. Local `git init` and `.gitignore` setup only when the developer asks for it.

**This file guides implementation; it does not supersede the developer's later explicit instructions or the authoritative master canon.**
