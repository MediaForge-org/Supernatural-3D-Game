# 0007 — Phase 0.5: pickup items and a minimal inventory (placeholder slice) — ENGINEERING PROPOSAL

Status: proposal written 2026-10-22; the four questions in section 5 were answered the same day (section 8). **Not master canon.** Core part implemented and tested on Fedora; Unreal part written but **not built or run** (no Unreal on that machine).
Everything here is a **placeholder test slice**, not a design for items, inventory, weight, capacity or loot in the finished game.

Naming: `README.md` §27.2 already uses "Phase 0.4" for the finished appearance foundation (Decision 0005). This slice is therefore called **Phase 0.5**.

## 1. Why this slice
Phase 0 lists the systems "interaction, items/inventory, investigation, dialogue, simple enemy, save/load, NPCs, driving" (`CLAUDE.md` §4) in no fixed order (`README.md` §27.3). Picking up a thing is the smallest next step that

- reuses what exists unchanged: `InteractionRegistry` (verb `Use` only, D1), `WorldFacts` v1, `USpnWorldStateSubsystem`, `USpnInteractionComponent`, the generated test map;
- is playable in Unreal in a minute (walk up, press E, the object disappears, a counter changes, save, load);
- is automatically testable in Core on Fedora first, then in Unreal automation;
- is reused later by evidence, keys, ammunition, loot and shop goods, which all need "an object leaves the world and a count changes".

Larger candidates (enemy AI, driving, dialogue) need many undecided design values and a new framework each; they are not proposed now.

## 2. Master basis (consultation per `CLAUDE.md` §11)
| # | Finding | Status as read | Source |
|---|---|---|---|
| I1 | Body inventory is physically limited; vehicles provide storage; HQ holds very large stocks | accepted direction; **no numbers, no capacity model** | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md`, player-state section; `PLANNING_STATUS_2026-09-05.md` (Block 10 "accepted direction") |
| I2 | Important dropped items can stay physical and become evidence; cleanup prevents unbounded clutter | confirmed (tier 3) | `PD`, line 686 |
| I3 | State lives in the persistent world state with stable ids; major changes do not casually reset | confirmed | Proposal 0004, M1, M2, M4 |
| I4 | Gameplay values stay configurable and open decisions stay open | project rule | `CLAUDE.md` §3, §7 |

Searched, **not found**: an item taxonomy, stacking rules, weight or slot numbers, item ids, pickup rules. Hence none of these is invented below.

## 3. Proposal

### 3.1 Core (Fedora first)
- Interactable kind `placeholder:interactable.pickup`. `Use` takes the object **once**:
  - fact `placeholder.pickup.<id>.taken` (bool, absent = authored `false`);
  - fact `placeholder.inventory.<holder>.<item>.count` (int, `AdjustInt` by the authored amount).
- Authored defaults (placement data): `item` (item id) and `amount` (int, default 1). Item ids reuse the D2 grammar `[a-z][a-z0-9_.]*`.
- Taking is atomic: both commands apply together or none (the registry already guarantees this). A second `Use` on a taken pickup writes nothing and is `Refused` with feedback `placeholder:feedback.pickup.gone`.
- No capacity, weight, stacking or drop rule (I1: unspecified). The count has no upper limit beyond the fact value range.
- Tests (about 12, `Tests/Core/PickupTests.cpp`): take once; second take refused with no journal record; amount and item defaults; two pickups of the same item add up; unknown or invalid item id; wrong fact type gives `StateCorrupt`; determinism of two stores; `StateAt(n)` before and after; save/load round trip byte-identical; unregistered kind in a save (mod case) loads unchanged; key-length limit holds for the longest allowed ids.

### 3.2 Unreal (Windows)
- `ASpnTestPickup` next to `ASpnTestDoor`: engine cube, placeholder label, `StableId`, `ItemId`, `Amount`; hidden and non-colliding when `taken`; follows `OnWorldStateReplaced` like the door.
- Console command `Spn.Inventory` printing the counts; on-screen debug text only (non-Shipping), no UI design.
- Test map: the existing generator script gains two pickups (one stacking item twice, one different item). Automation tests mirror the door tests (use, save, new subsystem, load; map layout).

### 3.3 Reuse
Pickup and counts are facts plus one handler; later systems add kinds and keys without touching the store. A key unlocking a door would use the same counts; that step is **not** part of this slice (section 5, Q3).

## 4. Acceptance criteria
1. Core: new tests plus all existing 181 pass on `fedora-gcc`, `fedora-clang` and both ASan/UBSan presets, no warnings; `WorldFacts`, format v1 and fixtures unchanged; `Core` includes no Unreal headers.
2. Unreal (Windows): new automation tests pass together with the existing 10; manual play: pick up both objects, `Spn.Inventory` shows the counts, `Spn.SaveWorld`, restart PIE, `Spn.LoadWorld`: objects gone, counts restored, untouched pickups still present.
3. One short record in `Docs/Decisions/` (engineering notes), no long game documentation.

## 5. Questions for you (answers needed before the code)
1. **Fact keys and limits:** key shape `placeholder.inventory.<holder>.<item>.count` with prototype limits holder ≤ 24 and item id ≤ 48 characters so the longest key stays within the 128-character fact-key limit (like D2 for doors). Accept?
2. **Who holds the item:** the registry call has no actor identity today. Proposed minimum: an optional `holder` string next to `cause` (single player uses the constant `player.local`), so co-op does not need a rewrite (`CLAUDE.md` §6). Accept this small API addition, or keep a fixed holder for now?
3. **Keys and locks:** whether keys exist, and how they interact with doors, is open (Proposal 0004, Q3). This slice does **not** decide it. Do you want a follow-up (Phase 0.5b) after your lock-model decision?
4. **Capacity:** left out on purpose (I1 gives no model). Confirm that no limit is wanted in this slice.

## 6. Increments (each reversible)
1. Core: `Pickup.h/.cpp`, `PickupTests.cpp`, CMake entries; ends with the results of all four Fedora presets.
2. Unreal: `ASpnTestPickup`, `Spn.Inventory`, automation tests (Windows).
3. Map generator update and manual play test (Windows).
4. Short decision record.

## 7. Non-goals
Weights, slots, stacks beyond a count, item categories, equipment, drops in the world, UI, sounds, loot tables, shops, evidence handling, keys, replication, mod loading, final assets.

## 8. Decisions and implementation status (2026-10-22)
Answers (technical prototype rules, not canon for the later inventory design): Q1 key scheme and limits accepted; Q2 optional holder, default `player.local`; Q3 no keys or locks in this phase; Q4 no capacity, weight or stack limits.

| Point | State |
|---|---|
| Core: `Pickup.h/.cpp`, optional `holder` in `InteractionRegistry::Interact` (new `planForHolder`; door handler and old call sites unchanged) | Done |
| Core tests: `Tests/Core/PickupTests.cpp`, 17 cases | 198 of 198 pass on `fedora-gcc`, `fedora-clang`, `fedora-gcc-asan-ubsan`, `fedora-clang-asan-ubsan`, no warnings |
| Unreal: `ASpnTestPickup`, bridge (`TryInteract` overload with holder and typed defaults, `ReadInventoryCount`, `ListInventory`), `Spn.Inventory`, 4 automation tests, map script with two pickups | Written, **not compiled or run** |
| Test map `.umap` | Not regenerated (needs the Windows editor); `SpnGame.TestMap.Pickups` warns until it is |
| Item ids | Deviation from the proposal: **no dots** in item ids (`[a-z][a-z0-9_]*`), because holder ids may contain dots and `holder.item` would be ambiguous in the key |
