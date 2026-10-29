// Hand-run benchmark of inventory transactions (M2B2). Not registered with CTest. Identical workload before and after an implementation
// change: run the same source against both and compare the output.
//
//   spn_core_bench_inventory_tx [repetitions] [seed]
//
// Measures per (stack count, scenario): latency of ApplyAll (steady_clock around each call; mean, median, p99, max), heap allocations
// per call and peak extra heap during one call (malloc_usable_size, exact for this process), and heap bytes per stack of the state.
// Each cell is repeated `repetitions` times (default 7) on a fresh copy of the state; the table shows the median of the per-repetition
// medians plus the min..max range across repetitions, so noise is visible. Build in Release.
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <malloc.h>
#include <new>
#include <spn/core/ContentId.h>
#include <spn/core/Inventory.h>
#include <string>
#include <vector>

using namespace spn::core;

// --- allocation accounting --------------------------------------------------------------------------------------------------------------
namespace {
std::size_t g_allocs = 0, g_live = 0, g_peak = 0;
} // namespace
void* operator new(std::size_t n) {
    void* p = std::malloc(n);
    if (p == nullptr) throw std::bad_alloc();
    ++g_allocs;
    g_live += malloc_usable_size(p);
    if (g_live > g_peak) g_peak = g_live;
    return p;
}
void operator delete(void* p) noexcept {
    if (p == nullptr) return;
    g_live -= malloc_usable_size(p);
    std::free(p);
}
void operator delete(void* p, std::size_t) noexcept { operator delete(p); }

namespace {

using Clock = std::chrono::steady_clock;

ContentId Cid(const std::string& s) { return *ContentId::Parse(s); }

struct World {
    ItemCatalog catalog;
    std::vector<ContentId> stackables, uniques;
    World() {
        for (int i = 0; i < 30; ++i) {
            ItemDefinition d;
            d.id = Cid("bench:item.s" + std::to_string(i));
            d.category = Cid("bench:cat.c" + std::to_string(i % 5));
            d.maxStack = 10;
            d.weightGrams = 10 + static_cast<std::uint64_t>(i);
            d.volumeMilliliters = 5;
            stackables.push_back(d.id);
            if (catalog.Add(d)) std::abort();
        }
        for (int i = 0; i < 10; ++i) {
            ItemDefinition d;
            d.id = Cid("bench:item.u" + std::to_string(i));
            d.category = Cid("bench:cat.c" + std::to_string(i % 5));
            d.weightGrams = 500;
            d.equipSlots = {Cid("bench:slot.hand")};
            uniques.push_back(d.id);
            if (catalog.Add(d)) std::abort();
        }
    }
};

constexpr ContainerId kBig{1}, kSmall{2};

// A big container with exactly `stacks` stacks (3 of 4 full stackable stacks, 1 of 4 unique items) and a small one with a few stacks.
InventoryState Build(const World& w, int stacks, std::vector<ItemInstanceId>& stackableIds) {
    InventoryState s(w.catalog);
    ContainerRules rules; // unlimited: the workload must not depend on a capacity value
    rules.equipSlots = {Cid("bench:slot.hand")};
    std::vector<InventoryCommand> cmds{CreateContainer{kBig, rules}, CreateContainer{kSmall, rules}};
    for (int i = 0; i < stacks; ++i) {
        if (i % 4 == 3) cmds.push_back(AddItem{kBig, w.uniques[static_cast<std::size_t>(i) % w.uniques.size()], 1, {}});
        else cmds.push_back(AddItem{kBig, w.stackables[static_cast<std::size_t>(i) % w.stackables.size()], 10, {}});
    }
    for (int i = 0; i < 5; ++i) cmds.push_back(AddItem{kSmall, w.stackables[static_cast<std::size_t>(i)], 3, {}});
    const auto r = s.ApplyAll(cmds);
    if (!r) std::abort();
    for (const auto& [id, stack] : s.FindContainer(kBig)->stacks)
        if (w.catalog.Find(stack.definition)->Stackable()) stackableIds.push_back(id);
    return s;
}

struct Sample {
    double ns;
    std::size_t allocs, peakExtra;
};

enum class Scenario { AddRemoveUnique, TransferRoundTrip, FailingRollback, SplitMerge };
const char* Name(Scenario s) {
    switch (s) {
    case Scenario::AddRemoveUnique: return "add unique + remove it (2 tx)";
    case Scenario::TransferRoundTrip: return "transfer out + back (2 tx)";
    case Scenario::FailingRollback: return "failing 2-cmd tx (rollback)";
    case Scenario::SplitMerge: return "split + merge (1 tx, 2 cmds)";
    }
    return "";
}

template <typename F> Sample Measure(F&& f) {
    const std::size_t a0 = g_allocs, live0 = g_live;
    g_peak = g_live;
    const auto t0 = Clock::now();
    f();
    const auto t1 = Clock::now();
    return {std::chrono::duration<double, std::nano>(t1 - t0).count(), g_allocs - a0, g_peak - live0};
}

void RunOnce(Scenario sc, const World& w, InventoryState& s, const std::vector<ItemInstanceId>& ids, int iterations, std::vector<Sample>& out) {
    const ItemInstanceId target = ids[ids.size() / 2];
    const ContentId unique = w.uniques[0];
    for (int i = 0; i < iterations; ++i) {
        switch (sc) {
        case Scenario::AddRemoveUnique: {
            ItemInstanceId created;
            out.push_back(Measure([&] {
                const auto r = s.ApplyAll({AddItem{kBig, unique, 1, {}}});
                if (!r) std::abort();
                created = r.Value().createdStacks.front();
            }));
            out.push_back(Measure([&] {
                if (!s.ApplyAll({RemoveItem{kBig, created, 1}})) std::abort();
            }));
            break;
        }
        case Scenario::TransferRoundTrip: {
            out.push_back(Measure([&] {
                if (!s.ApplyAll({TransferItem{kBig, kSmall, target, 1}})) std::abort();
            }));
            // find the moved unit's destination stack: the small container merged it into a stack of the same definition (or created one)
            const ItemStack* moved = nullptr;
            for (const auto& [id, st] : s.FindContainer(kSmall)->stacks)
                if (st.definition == s.FindStack(kBig, target)->definition) { moved = &st; break; }
            if (moved == nullptr) std::abort();
            const ItemInstanceId back = moved->id;
            out.push_back(Measure([&] {
                if (!s.ApplyAll({TransferItem{kSmall, kBig, back, 1}})) std::abort();
            }));
            break;
        }
        case Scenario::FailingRollback:
            out.push_back(Measure([&] {
                // first command succeeds (mutates the staged copy), second names an unknown container: the transaction must change nothing
                const auto r = s.ApplyAll({AddItem{kBig, unique, 1, {}}, RemoveItem{ContainerId{999}, target, 1}});
                if (r) std::abort();
            }));
            break;
        case Scenario::SplitMerge: {
            out.push_back(Measure([&] {
                // split one unit off the full stack, merge it back: net zero; the new stack id is not known in advance, so merge by next id
                const std::uint64_t nextId = s.NextInstanceId().Value();
                const auto r = s.ApplyAll({SplitStack{kBig, target, 1}, MergeStacks{kBig, ItemInstanceId(nextId), target, 0}});
                if (!r) std::abort();
            }));
            break;
        }
        }
    }
}

double Pct(std::vector<double> v, double p) {
    std::sort(v.begin(), v.end());
    return v[std::min(v.size() - 1, static_cast<std::size_t>(p * static_cast<double>(v.size())))];
}
double Median(std::vector<double> v) {
    std::sort(v.begin(), v.end());
    const std::size_t n = v.size();
    return n % 2 == 1 ? v[n / 2] : (v[n / 2 - 1] + v[n / 2]) / 2.0; // true median: the two-transaction scenarios are bimodal
}

} // namespace

int main(int argc, char** argv) {
    const int reps = argc > 1 ? std::max(1, std::atoi(argv[1])) : 7;
    World w;
    std::printf("inventory transaction benchmark: %d repetitions per cell. Latency/allocs are per ApplyAll call (a scenario with 2 tx contributes 2 samples per iteration).\ntimes in microseconds: median of the per-repetition medians [min..max over repetitions]\n", reps);
    std::printf("%-8s %-34s %12s %22s %10s %10s %9s %11s\n", "stacks", "scenario", "median us", "[min..max of medians]", "mean us", "p99 us", "allocs", "peak extra");
    for (const int stacks : {10, 100, 1000, 10000}) {
        std::vector<ItemInstanceId> ids;
        const InventoryState base = Build(w, stacks, ids);
        if (base.CheckInvariants()) std::abort();
        for (const Scenario sc : {Scenario::AddRemoveUnique, Scenario::TransferRoundTrip, Scenario::FailingRollback, Scenario::SplitMerge}) {
            const int iterations = stacks >= 10000 ? 60 : stacks >= 1000 ? 300 : 1500;
            std::vector<double> medians, p99s, means;
            std::vector<double> allocs, peaks;
            for (int rep = 0; rep < reps; ++rep) {
                InventoryState s = base; // fresh copy per repetition (not measured)
                std::vector<Sample> samples;
                samples.reserve(static_cast<std::size_t>(iterations) * 2);
                RunOnce(sc, w, s, ids, iterations, samples);
                std::vector<double> ns;
                for (const auto& x : samples) {
                    ns.push_back(x.ns);
                    allocs.push_back(static_cast<double>(x.allocs));
                    peaks.push_back(static_cast<double>(x.peakExtra));
                }
                medians.push_back(Median(ns) / 1000.0);
                p99s.push_back(Pct(ns, 0.99) / 1000.0);
                double sum = 0;
                for (const double v : ns) sum += v;
                means.push_back(sum / static_cast<double>(ns.size()) / 1000.0);
                if (s.CheckInvariants()) std::abort();
            }
            const auto [mn, mx] = std::minmax_element(medians.begin(), medians.end());
            char range[48];
            std::snprintf(range, sizeof range, "[%.2f..%.2f]", *mn, *mx);
            std::printf("%-8d %-34s %12.2f %22s %10.2f %10.2f %9.0f %9.1f KB\n", stacks, Name(sc), Median(medians), range, Median(means), Median(p99s),
                        Median(allocs), Median(peaks) / 1024.0);
        }
    }
    // memory of the state itself
    std::printf("\nheap bytes of an InventoryState (copy of the state above, malloc_usable_size):\n");
    for (const int stacks : {10, 100, 1000, 10000}) {
        std::vector<ItemInstanceId> ids;
        const InventoryState base = Build(w, stacks, ids);
        const std::size_t before = g_live;
        { auto* copy = new InventoryState(base); const std::size_t bytes = g_live - before; std::printf("  %6d stacks: %9zu bytes (%.1f bytes per stack)\n", stacks, bytes, static_cast<double>(bytes) / stacks); delete copy; }
    }
    return 0;
}
