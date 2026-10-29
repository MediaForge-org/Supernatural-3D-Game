# M2B2 — Inventory Transaction Performance

Status words: see `WORLD_COORDINATE_CONVENTIONS.md`.
Code: `Core/src/Inventory.cpp` (`InventoryTransaction`), `Core/include/spn/core/Inventory.h` (`Container::weightGrams/volumeMilliliters`), `Core/src/InventoryPersistence.cpp`.
Tests: `Tests/Core/InventoryTxTests.cpp` (new), `InventoryTests.cpp` (M2B1, **unchanged**), golden `Tests/Core/Data/inventory_tx_golden.txt`.
Benchmark: `Tests/Core/BenchInventoryTx.cpp` → `spn_core_bench_inventory_tx` (hand-run, Release).

## 1. Starting point (M2B1)

`ApplyAll` copied each touched container on first touch (`std::map<ContainerId, std::optional<Container>> staged_`), ran the commands on the copies, and moved them into the live state on success. After every `AddItem`, `TransferItem` (destination) and `SplitStack` it ran `CheckCapacity`, which recomputed the weight and volume of **all** stacks.

## 2. Investigation — measured, not assumed

Ad-hoc measurement in a Release build, one container with **10,000 stacks** (single run each, indicative only):

| piece | cost | complexity |
|---|---|---|
| copy the container + free the copy (what staging does per touched container, also when the transaction later fails) | ≈ 600 µs, ≈ 10,000 allocations (one map node per stack) | O(n) time and allocations |
| one weight sum over the stacks (`Total`) | ≈ 640 µs | O(n) × catalog lookup (`std::map<ContentId,…>` string compares) |
| `CheckCapacity` = weight sum + volume sum | ≈ 1.3 ms | O(n) × 2 |
| `CheckInvariants` (diagnostic) | ≈ 4 ms | O(n) |
| unknown-container transaction (no copy) | 0.05 µs | O(log C) |

Consequences:

* **An undo log alone would have been the wrong fix.** It removes the copy (≈ 1/3 of an `AddItem`) but leaves the two full sums (≈ 2/3). Both costs had to go: no copy, and no recomputation.
* Commit was already O(1) (a move of the staged map); rollback was O(n) (destroying the copy). Lookup inside a container is O(log n) (`std::map`). The remaining linear piece is the search for a **non-full stack of the same definition** when adding stackable items (`PutStackable` scans the container): see section 6.

## 3. Implemented design

1. **In-place execution with an undo log** (`InventoryTransaction`). Every mutation is recorded *before* it happens: quantity change (old value), inserted stack (id), erased stack (extracted **node handle**), moved stack (from/to/id), inserted/erased equipment (slot / node handle), created/destroyed container (id / node handle), derived totals (old values). Rollback replays the log backwards. Undo never allocates: entries only erase, re-insert extracted nodes, or write numbers back. The instance-id counter is restored; the revision is only incremented on commit.
2. **Derived totals** `Container::weightGrams`, `volumeMilliliters`, updated incrementally with overflow-checked arithmetic (`AdjustTotals`), so `CheckCapacity` is O(1). Overflow keeps the exact M2B1 semantics: the totals are left unchanged, a flag is remembered for the current command, and `CheckCapacity` reports `ArithmeticOverflow` at the same position in the same check order (stack count → weight overflow → weight limit → volume overflow → volume limit). Committed states never contain an overflowed sum. `TotalWeightGrams/TotalVolumeMilliliters` now return the cached value in O(1).
3. **Independent oracle.** `CheckInvariants` still recomputes everything from the stacks (the old O(n) path, renamed `CheckCapacityFromScratch`) and additionally compares the result with the cached totals (`Inconsistent`: "cached weight/volume totals differ"). `Deserialize` recomputes the totals; they are **not** part of `SPN-INVENTORY 1` (format unchanged).
4. Public API and behaviour: unchanged signatures (`Apply`, `ApplyAll`, queries). `Container` gained two public fields documented as derived/read-only; `Container::operator==` includes them (they are a pure function of the stacks, so equal states stay equal).

No exceptions: the Core is exception-free by decision (`Docs/Decisions/0001`), so there is no unwinding path. An out-of-memory `std::bad_alloc` inside a command would terminate the process, as everywhere else in the Core. The staged-copy version would have left the live state untouched in that situation; this is a (theoretical) behavioural difference and is stated here rather than hidden.

## 4. Correctness evidence — **Tested**

* **M2B1 inventory tests run unchanged**: all 30 pass (capacity, equipment, rollback, determinism, persistence goldens, 4,000-step property test).
* **Behaviour preserved against the old implementation.** `Tests/Core/Data/inventory_tx_golden.txt` is a 3,000-line transcript recorded with the **M2B1 staged-copy code before any change** (seeded random batches of 1–4 commands over 3–5 containers with limits, equipment, unique and stackable items, a weight-overflow item, create/destroy). Per batch it holds: ok/error code and failing command index, CRC-32 of the full serialized state, revision, next instance id. The new implementation reproduces it **byte for byte** (606 accepted batches, 2,394 rejected, 336 of those rejected *after earlier commands had changed the state*, i.e. real multi-step rollbacks; the test requires every command type to be exercised successfully at least 8 times).
* **Rollback is exact**: after every rejected batch `state == snapshot` (all fields, counters, revision) and the serialization is identical; the failing command alone fails the same way on the state its predecessors produced; an accepted batch equals the same commands applied one by one (except the revision).
* Targeted rollback test: eight batches that each change a *different* kind of thing (quantities and new stacks, erased stacks and equipment, moved unique items, split/merge, created-then-destroyed containers, equip toggles, weight overflow) and then fail; the state afterwards is identical, instance ids continue where they were.
* Instance ids are never reused, also not after rolled-back batches. Cached totals equal the recomputed sums after every one of 1,500 random double-command batches.
* **Mutation checks** (done by hand, restored afterwards): disabling the undo of erased equipment → 3 tests fail; not restoring the cached totals → 7 fail; not restoring the id counter → 5 fail. The tests do detect broken rollbacks.
* Sanitizers: see `VALIDATION.md`.

## 5. Benchmark — before vs after, identical workload

Method: the **same** `BenchInventoryTx.cpp` compiled against (a) the M2B1 sources taken from `git show HEAD` and (b) the new sources, `g++ -O3 -DNDEBUG -march=native -std=c++20`, GCC on Fedora 44, Intel Core Ultra 7 256V (the machine had other load: load average ≈ 2 of 8 during the runs). State: one container with N stacks (3 of 4 full stackable stacks of 30 definitions, 1 of 4 unique items; 10 more unique definitions) plus a small container. Each cell = **7 repetitions** on a fresh copy of the state (60–1,500 iterations each), the whole table run **twice**. Latency is per `ApplyAll` call measured with `steady_clock` (so timer overhead of ≈ 20–30 ns is inside the sub-µs numbers); allocations and peak extra heap are exact counts from replacing `operator new/delete` (`malloc_usable_size`). Scenarios with "2 tx" contribute two samples per iteration (their median mixes both calls).

Median per call in µs (run 1 / run 2); p99 and allocations from run 1.

| stacks | scenario | before median | after median | before p99 | after p99 | allocs/call before → after | peak extra heap before → after |
|---|---|---|---|---|---|---|---|
| 10 | add unique + remove it (2 tx) | 1.98 / 1.81 | 0.27 / 0.25 | 2.78 | 0.37 | 14 → 4 | 2.0 KB → 0.4 KB |
| 10 | transfer out + back (2 tx) | 2.14 / 2.10 | 0.34 / 0.29 | 3.04 | 0.42 | 22 → 5 | 3.1 KB → 0.6 KB |
| 10 | failing 2-cmd tx (rollback) | 2.04 / 1.91 | 0.35 / 0.30 | 2.56 | 0.43 | 15 → 5 | 2.2 KB → 0.7 KB |
| 10 | split + merge (1 tx, 2 cmds) | 2.13 / 2.10 | 0.35 / 0.31 | 2.89 | 0.45 | 15 → 6 | 2.2 KB → 0.8 KB |
| 100 | add unique + remove it (2 tx) | 17.98 / 17.00 | 0.26 / 0.23 | 32.33 | 0.35 | 104 → 4 | 14.0 KB → 0.4 KB |
| 100 | transfer out + back (2 tx) | 18.32 / 18.48 | 0.66 / 0.72 | 30.63 | 0.76 | 112 → 5 | 15.1 KB → 0.6 KB |
| 100 | failing 2-cmd tx (rollback) | 22.62 / 24.39 | 0.38 / 0.41 | 29.41 | 0.47 | 105 → 5 | 14.2 KB → 0.7 KB |
| 100 | split + merge (1 tx, 2 cmds) | 24.51 / 24.93 | 0.41 / 0.41 | 32.34 | 0.52 | 105 → 6 | 14.2 KB → 0.8 KB |
| 1,000 | add unique + remove it (2 tx) | 167.54 / 167.44 | 0.29 / 0.29 | 325.45 | 0.49 | 1004 → 4 | 133.5 KB → 0.4 KB |
| 1,000 | transfer out + back (2 tx) | 172.26 / 162.01 | 3.16 / 3.37 | 317.27 | 6.60 | 1012 → 5 | 134.6 KB → 0.6 KB |
| 1,000 | failing 2-cmd tx (rollback) | 261.54 / 263.47 | 0.45 / 0.49 | 328.43 | 0.55 | 1005 → 5 | 133.7 KB → 0.7 KB |
| 1,000 | split + merge (1 tx, 2 cmds) | 270.74 / 269.92 | 0.48 / 0.51 | 341.57 | 0.65 | 1005 → 6 | 133.7 KB → 0.8 KB |
| 10,000 | add unique + remove it (2 tx) | 1688 / 1680 | 0.29 / 0.32 | 3011 | 0.94 | 10004 → 4 | 1328.8 KB → 0.4 KB |
| 10,000 | transfer out + back (2 tx) | 1776 / 1743 | 39.96 / 42.15 | 3251 | 98.38 | 10012 → 5 | 1329.9 KB → 0.6 KB |
| 10,000 | failing 2-cmd tx (rollback) | 2821 / 2858 | 0.40 / 0.43 | 3036 | 3.00 | 10005 → 5 | 1329.0 KB → 0.7 KB |
| 10,000 | split + merge (1 tx, 2 cmds) | 2879 / 2928 | 0.42 / 0.48 | 3131 | 2.56 | 10005 → 6 | 1329.0 KB → 0.8 KB |

Noise, computed from the printed tables: within a run the spread between the fastest and slowest repetition median was up to **30 %** of the median in the before cells and up to **58 %** in the after cells (sub-microsecond cells are the noisiest: a few tens of ns of timer and cache effects are a large fraction); the medians of run 1 and run 2 differed by at most **9 %** (before) and **17 %** (after). The speed-up factors (≈ 7× at 10 stacks, ≈ 70× at 100, ≈ 600× at 1,000, ≈ 6,000× at 10,000 for add/remove) are far outside that noise; the exact after-numbers are not. The before numbers match the M2B1 report (≈ 1.5 µs / 130 µs / 1.5 ms per transaction for 10 / 1,000 / 10,000 stacks).

**Reading the result.** Transaction cost no longer depends on container size for add, remove, split, merge, rollback (0.25–0.5 µs at 10 … 10,000 stacks, about 4–6 allocations). Memory overhead per transaction fell from a full container copy (≈ 133 bytes per stack) to a few hundred bytes. State memory: +16 bytes per container (two `uint64`); 10 stacks 2,880 → 2,912 bytes, 10,000 stacks 1,361,520 → 1,361,552 bytes (≈ 136 bytes per stack, unchanged).

Caveats: one machine, GCC only, `-march=native`, a synthetic workload (stack mix is not game data), timer overhead included, other load on the machine. Treat ratios as indicative; do not quote absolute microseconds as a promise for the game.

## 6. What is still O(n) — and why it was not changed

* `TransferItem`/`AddItem` of a **stackable** item scans the destination for a non-full stack of the same definition (`PutStackable`). Measured: "transfer out + back" at 10,000 stacks ≈ 40 µs (the return trip scans the 10,000-stack container), 3 µs at 1,000, 0.7 µs at 100. Fixing it needs a secondary index (definition → non-full stacks) that must also be undone, serialized-free and invariant-checked. **Not justified by the measurement** for the sizes tested (40 µs is far below a frame); it is the first thing to do if containers with tens of thousands of distinct stacks appear. This is a *deliberate non-change*, not an oversight.
* `CheckInvariants`, `Serialize`, `Deserialize`, `CountOf`, `LocateStack` (O(containers × log stacks)): diagnostic or load/save-time paths, unchanged.
* Capacity limits are checked on the destination container only (as in M2B1).

## 7. Known limitations / open items

* Out-of-memory behaviour differs in theory (section 3).
* Benchmarks are Linux-only (`malloc_usable_size`, `<malloc.h>`); the library change itself is portable.
* A hypothetical future command that increases totals without a following `CheckCapacity` would have to call it (or clear the overflow flag) — the invariant "every increasing command ends in `CheckCapacity`" is documented in the code and covered by the oracle in `CheckInvariants`.
* Replication/undo of inventory over the network (co-op) is not designed here.

## 8. Requirements

**Windows / Unreal:** none for this change (pure Core). Compile with MSVC/clang-cl: `Inventory.cpp` already used `__builtin_*_overflow` (GCC/Clang); that pre-existing portability point is unchanged and unverified on MSVC.
**WorldForge:** none.
