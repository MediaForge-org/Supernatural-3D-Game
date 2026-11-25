# 0019 — First playable case "Die Stille Mühle" (2026-11-25) — PROPOSAL

Status: **proposal for developer review, not master canon.**
- Story content, names, the creature and its weakness are working material for a first playable case. They lock nothing in the master's
  story, lore or creature planning.
- All geometry is greybox placeholder. Weapons, vehicles, characters and environment art are made by the developer later.
- Every system is built so that an asset can be swapped without code changes (data-driven slots).

## 0. Basis

| Source | What it gives | Status |
|---|---|---|
| Developer, chat 2026-11-25 | Choices matter strongly in the finished game, so the case **ends with a choice** | developer decision |
| Developer, chat 2026-11-25 | Player does not know the creature, takes a **pistol** for protection, finds the weakness (bell) by **research in a library** | developer decision |
| Developer, chat 2026-11-25 | Death leads to a **real death screen** and a restart. The **main menu** must be improved ("too superficial") | developer decision |
| Developer, chat 2026-11-25 | Own test map first. If it works well, take the area **over into the game** as a whole | developer decision |
| Developer, chat 2026-11-25 | Tone: **horror**, dark and mysterious. Mood reference: Supernatural season 1 | developer decision |
| Developer, chat 2026-11-25 | Present day. The phone only makes **calls** for now (a full phone in the finished game) | developer decision |
| Developer, chat 2026-11-25 | Dialogue: on-screen text, the player clicks an answer, the conversation continues | developer decision |
| Developer, chat 2026-11-25 | Player body: `mh_male_193` | developer decision |
| Developer, chat 2026-11-25 | The protagonist is a **new hunter** at the start. Rare monsters are identified by research; demons and ghosts often not | developer direction |
| Developer, chat 2026-11-25 | Permadeath will come later as an **option** | developer direction |
| Developer, chat 2026-11-25 | As far as the developer knows, the master defines **no case and no monster** yet | to verify |
| `Planning/Master` | Not reachable on the Windows machine. The repo planning register (merged from `main` on 2026-11-25) was consulted instead (table below) | partly |

**Repo planning register (consulted 2026-11-25)**

| Entry | What it says | Status | Effect on this case |
|---|---|---|---|
| `Docs/Planning/TASKS.md` `SLICE-CASE-01` | Act, characters, case and trigger are **open** | open | This case fills that slot as a proposal only |
| `TASKS.md` `PROTOTYPE-CASE-01`, `CORE-EVIDENCE`, `CORE-DIALOGUE` | Evidence, dialogue and investigation-assist cores exist in `Core` with tests | done (Core) | **Reuse** `Evidence.h`, `Dialogue.h`, `InvestigationAssist.h`; no new parallel systems |
| `DECISIONS.md` EVD-04 / EVD-05 | No automatic case closure. Evidence unlocks dialogues, actions and conclusions; closure needs a **deliberate player action**; wrong conclusions may be possible | binding | The ending is the player's act at the mill. The case file never closes the case by itself |
| `DECISIONS.md` EVD-02 | The journal saves investigation progress | binding | Case file progress lives in WorldFacts |
| `DECISIONS.md` DIF-01 / DIF-02 | Several difficulty levels; number and names open. Assists configurable independently of difficulty | binding (count open) | Difficulty = data list of N profiles; assists separate |
| `DECISIONS.md` S-08 | No respawn spawn points; weaknesses are **not only bonus damage** | binding | The bell stuns (a mechanic), it is not a damage multiplier. The creature does not respawn |
| `DECISIONS.md` C-21 | Individual death stays final; intelligent, species-specific creatures; incomplete knowledge | binding (essentially) | A killed creature stays dead. Knowledge comes from research |
| `Docs/Game/Kreaturen/10` §1 | Perception with uncertainty (no magic player position); four knowledge levels; search phases (pursuit → local → extended → containment → wary), duration by difficulty; three kinds of noise; lighting is part of perception; the AI must be testable with an inspector | 📋 planned | Creature AI follows these rules. Patrol / hear / chase / stun in §3 is the smallest subset |
| `Docs/Game/Kreaturen/10` §1 | A creature that is effectively trapped must not free itself by clever AI; mythology holds | 📋 / 💡 | "Lock it away" is safe against AI escape. A later risk must come from the story (for example a person), not the AI |
| `CONFLICTS.md` K-26 | **Player death model is contradictory** (C-21 vs. checkpoint after death in the combat docs). Blocks combat, investigation and checkpoints | conflict, open | Developer said on 2026-11-25: death → restart, optional permadeath later. **Needs confirmation that this closes K-26** (§7) |

The mood reference means atmosphere only. No characters, names, vehicles or scenes from the series are used.

## 1. The case in short

- **Setting:** a small place at the edge of a forest with an old, closed sawmill.
- **Hook:** two people vanished at night near the mill within three weeks, and the neighbours **heard nothing**. A phone call brings the
  player in.
- **The creature:** stays nameless and half unexplained, deliberately.
  - It is blind and hunts by **sound**.
  - Its presence **swallows sound**: when the world goes quiet, it is near.
  - Shots barely hurt it, but they are loud and draw it straight to the shooter. The pistol feels like protection and is a trap.
  - A loud, high sound (the old mill bell) **stuns** it.
- **Twist:** the miller knew everything. His father did not kill the creature in the past. He locked it away and fed it, and the miller
  continued.
- **Choice at the end:** kill it, or lock it away again. The miller is then reported, spared or confronted. The result is saved
  as world state, so later cases can read it.

## 2. Flow (target 30–45 min)

1. **Call → arrival** at dusk (on foot; car later).
2. **Talks** (click dialogue):
   - the landlady: times and places;
   - the miller: dismissive;
   - a teenager: "saw the thing", nobody believes him.
3. **Clues:**
   - scratches on a door, *on the inside*;
   - the victim's diary: "it comes when it goes quiet";
   - a dead animal in the forest;
   - the closed mill.
4. **Library research:** a newspaper note from decades ago (people vanished, then the mill was closed) and an old account of the bell.
   The clues are combined in the case file. Result: blind, hunts by sound, bell stuns it.
5. **Preparation:** throwable lures (bottles), flashlight, pistol (with the knowledge of what shots do).
6. **Night in the mill:**
   - sneak past the creature on patrol;
   - lure it with throws;
   - if detected, a chase through narrow corridors; slamming doors buys seconds;
   - ring the bell while it is near → stunned.
7. **Choice + consequences** (saved).

## 3. Systems: what exists, what is missing

| System | State | Core (portable C++20) | Unreal |
|---|---|---|---|
| Footstep / gameplay noise, NPC hearing and suspicion | ✅ (NightBuild06) | `NpcReaction` | `SpnNpc`, character |
| Doors (open / close / slam) | ✅ | — | `SpnTestDoor` |
| Pickups, inventory, save/load slots | ✅ | inventory, `GameSave` | `SpnSessionSubsystem` |
| **Case file: clues → conclusions** | Core ✅ (`Evidence`, `InvestigationAssist`), Unreal ❌ | extend the existing cores only where this case needs it | journal screen, evidence objects |
| **Click dialogue** | Core ✅ (`Dialogue`), Unreal ❌ | reuse as is | dialogue screen |
| **World facts for choices** | partly (`WorldFacts`) | facts set by dialogue and ending | — |
| **Throwables as noise sources** | ❌ | noise loudness rule (reuse `FootstepNoise` pattern) | throw + impact noise |
| **Pistol** (ammo, reload, loudness; little damage to this creature) | ❌ | weapon rules (ammo, noise, damage vs. creature resistances) | placeholder shape, hit trace |
| **Creature AI:** patrol, hear, investigate, chase, stun, lose track | ❌ | state machine (pure rules, testable) | movement, NavMesh, senses |
| **Silence effect** near the creature | ❌ | loudness falloff rule | audio ducking (placeholder) |
| **Player death → death screen → reload / menu** | ❌ | — | death flow, UI |
| **Main menu, more atmosphere** | basic | — | menu map, UI |
| **Phone call** (scripted, text) | ❌ | — | call UI |
| **Flashlight** | ❌ | — | light on the player |
| Case map (village, forest edge, mill, library) | ❌ | — | greybox map, data-driven so it can move into the world |

## 4. Saving (proposal; decision needed)

Today there are free save slots and "Continue". Proposal for this case:
- **Manual save** anywhere, **except** while the creature is hunting the player (chase state). A save mid-chase would trap the player.
- **Autosave** at the chapter points: after arrival, after the research, before the night in the mill, and **directly after the final
  choice**.
- **Death** loads the newest save (auto or manual). The death screen offers "load last save" or "main menu".
- **Old saves** stay loadable, so a player can go back before the choice. Strict "a choice is final" is the job of the later optional
  permadeath / ironman mode, not of this case.

## 5. Order of work (each step playable and tested)

1. **Night build A — the core loop in greybox:**
   - case map greybox;
   - creature (patrol, hear, chase, stun by bell);
   - throwables;
   - player death, death screen and reload.
2. **Night build B — the investigation:**
   - click dialogue;
   - case file with clues and conclusions;
   - library research;
   - phone call.
3. **Night build C — weapon, ending, frame:**
   - pistol (ammo, reload, noise);
   - ending choice with saved consequences;
   - autosaves;
   - main menu rework;
   - flashlight;
   - silence effect.
4. Then **play it through together** and tune. All values stay temporary until the developer approves them.

## 6. Tests

- **Core (ctest):**
  - creature state transitions: patrol → investigate → chase → lose track / stunned;
  - noise of throws and shots;
  - case-file conclusions from clues;
  - dialogue conditions and effects on facts;
  - the ending writes the right facts;
  - save-blocked-while-chased rule.
- **Unreal automation:**
  - the creature hears a sprinting player but not a crouching one at the same distance;
  - a shot draws it;
  - the bell stuns it in range only;
  - death → death screen → reload restores state;
  - the ending choice survives save/load;
  - a slammed door blocks the chase for a moment.
- **QA suite `Case`:** a full scripted run through the case (with screenshots for the developer).

## 7. Open for the developer

1. ~~Saving model (§4)~~ **accepted** by the developer on 2026-11-25 ("genau so wie ich es will").
2. ~~Working title~~ **kept for now** ("passt mal so").
3. ~~Shots and instant death~~ **by difficulty** (developer 2026-11-25): maybe instant death on the hardest level, not fixed. Many levels
   planned (example names: Story … Meisterhaft, not fixed). Built as data profiles; two test profiles only.
4. ~~K-26 (player death model)~~ **decided for the player** on 2026-11-26 (`Docs/Planning/DECISIONS.md` TOD-01..03, MED-01): death →
   checkpoint (newest autosave) or load a save; permadeath optional with variants (mission, chapter, whole game). Still open: TOD-04,
   the state after the reset (minimal health as in `COMBAT-DEC-BW-012`, or none).
5. After Fedora: check `Planning/Master` (§0) before anything here becomes more than a proposal.
