# 0009 — Traversal: active jump, human jump strength, height-matched movement over low obstacles — DIRECTION (NOT IMPLEMENTED)

Status: written 2026-10-22 after the M1 manual test. The **design direction** below was stated by the developer on 2026-10-22; the numbers and the implementation are **open**. Not master canon. Nothing in this document is implemented; M1 contains no vault or mantle system.

## 1. Observation (M1 manual test)
Crossing the 1 m test wall works with the existing jump (`SPN_M1_Wall`, `SpnGame.TestMap.M1Traversal` passes), but there is no matching vault/mantle animation or movement: the character simply jumps over it.

## 2. Design direction (developer, 2026-10-22)
1. **Jumping must be triggered actively** by the player (a jump input). It is never an automatic reaction to an obstacle.
2. **Jump strength stays realistically human.** The values must not be raised to clear obstacles.
3. For **low obstacles** the character should later perform a movement that **fits the obstacle height** (for example a step-up, a vault or a mantle) instead of an unnecessarily high jump.
4. Implemented only in a later **traversal milestone**; M1 does not get a vault/mantle system.

## 3. Current state (read from code, 2026-10-22)
- `SupernaturalProtCharacter.cpp`: the jump is bound to `JumpAction` (`Started` -> `Jump`, `Completed` -> `StopJumping`). There is no automatic jump. Sprint and interact are held/pressed inputs too.
- `JumpZVelocity` is 500 cm/s in the C++ constructor (template value). With the engine default gravity of 980 cm/s^2 that is an apex of about 127 cm, which is **higher than a realistic standing human jump**. The Blueprint `BP_ThirdPersonCharacter` might override it (binary asset, not inspected). The M1 wall height of 100 cm was chosen so that this template jump clears it.
- No jump-hold variation (`JumpMaxHoldTime` is 0), no mantle, no obstacle-height detection.
- Consequence: the M1 test wall is **only crossable because the template jump is stronger than the direction above allows**. When the jump strength is lowered, the wall needs to be lowered or replaced by a traversal move; `M1Traversal` (wall jump case) will then have to change. That is an expected follow-up, not a defect.

### 3a. Additions from the second M1 audit (2026-10-22)
- **Further developer decisions (2026-10-22):** the planned jump key on the DualSense is cross (x); animation and actual movement must match; injuries and exhaustion may influence traversal later; a 50 cm wall must not cause an exaggerated jump.
- **Verified values (read from the Blueprint class `BP_ThirdPersonCharacter_C`, identical to the C++ constructor):** `MaxWalkSpeed` 500, sprint 800 (SPN placeholder), `JumpZVelocity` 500, gravity scale 1, air control 0.35, max step height 45, walkable floor angle 44.8 degrees. Apex about 127 cm (calculated; the M1 wall of 100 cm is crossed in `M1Traversal`).
- **Bindings (`IMC_Default`):** jump = Space and `Gamepad_FaceButton_Bottom` (the DualSense cross is that button on the Windows gamepad layout), so the planned key is already bound. Sprint (Shift) and interact (E) have no gamepad binding yet.
- **Needed later:** a much lower jump velocity (a number to decide), the M1 wall and the `M1Traversal` wall-jump case must then be lowered/changed, a gamepad binding for sprint and interact, obstacle height classification (section 4). The M1 camera pop at the low wall was fixed in the third audit by rate-limiting the arm's growth (Proposal 0008 section 12); a traversal milestone should re-check the camera during vault/mantle moves.

## 4. Proposal for the later traversal milestone (PROPOSAL, nothing decided)
- A small obstacle probe (height and depth by traces in front of the character) decides between: walk (step height), jump, vault (low, thin obstacle), mantle (higher edge, hands on top).
- The move chosen depends on measured height; thresholds and the human jump strength are configuration values to be decided with you, not constants.
- Animation: needs vault/mantle assets (not available yet); a placeholder could be a scripted capsule move without animation.
- Tests: deterministic ones for the height classification (portable logic), plus map tests with several wall heights.
- Co-op and persistence: no world-state impact; movement stays local/authoritative per player (see `CLAUDE.md` section 6).

## 5. Open questions
1. What jump apex counts as "realistic human" for this game (a number, or "to be tuned by feel")?
2. Which obstacle heights get which move (step, vault, mantle)?
3. Are assets for these moves planned (own, purchased, generated)?
