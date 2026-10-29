// Hand-run micro-benchmarks for the M2B1 systems (map, minimap, inventory). Not registered with CTest.
// Build target: spn_core_bench_m2b1. Numbers depend on the build type (the presets are Debug) and the machine; they say nothing
// about Unreal frame cost. Workload sizes are synthetic stress sizes, NOT predictions of the real world's marker or item counts.

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <vector>

#if defined(__GLIBC__)
#include <malloc.h>
#endif

#include <spn/core/Inventory.h>
#include <spn/core/Minimap.h>
#include <spn/core/SpatialGrid.h>
#include <spn/core/WorldMap.h>

using namespace spn::core;
using Clock = std::chrono::steady_clock;

namespace {

struct Rng {
    std::uint64_t s;
    std::uint64_t Next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 11U;
    }
    double Uniform(double lo, double hi) { return lo + (hi - lo) * (static_cast<double>(Next() % 1'000'000ULL) / 999'999.0); }
};

ContentId Cid(const char* text) { return *ContentId::Parse(text); }

template <class F> void Bench(const char* name, int n, F&& f) {
    volatile double sink = 0.0;
    const auto t0 = Clock::now();
    for (int i = 0; i < n; ++i) {
        sink = sink + f(i);
    }
    const double ns = std::chrono::duration<double, std::nano>(Clock::now() - t0).count() / n;
    std::printf("%-58s %12.1f ns/op (%d ops)\n", name, ns, n);
}

std::size_t HeapInUse() {
#if defined(__GLIBC__) && defined(__GLIBC_MINOR__) && (__GLIBC__ > 2 || __GLIBC_MINOR__ >= 33)
    const struct mallinfo2 info = mallinfo2();
    return info.uordblks;
#else
    return 0;
#endif
}

} // namespace

int main() {
    constexpr double kHalf = 25000.0; // a 50 km x 50 km square = 2,500 km^2: the order of magnitude of the planning range, NOT a map
    const double baseline = static_cast<double>(HeapInUse());

    // --- Spatial grid ------------------------------------------------------------------------------------------------------
    for (const std::size_t count : {10'000UL, 100'000UL, 1'000'000UL}) {
        SpatialGrid grid(256.0);
        std::vector<Vec2> points;
        points.reserve(count);
        Rng rng{1};
        const double before = static_cast<double>(HeapInUse());
        for (std::size_t k = 0; k < count; ++k) {
            const Vec2 p{rng.Uniform(-kHalf, kHalf), rng.Uniform(-kHalf, kHalf)};
            points.push_back(p);
            grid.Insert(k, p);
        }
        const double used = static_cast<double>(HeapInUse()) - before - static_cast<double>(points.capacity() * sizeof(Vec2));
        std::printf("\n[SpatialGrid] %zu points over 2,500 km^2, heap for the grid ~ %.1f MiB (%s)\n", count, used / 1048576.0,
                    HeapInUse() == 0 ? "mallinfo2 unavailable" : "glibc mallinfo2");
        std::vector<std::uint64_t> out;
        Rng q{9};
        char name[96];
        std::snprintf(name, sizeof name, "  QueryRadius r=150 m (minimap-sized), %zu pts", count);
        Bench(name, 20000, [&](int) {
            out.clear();
            grid.QueryRadius({q.Uniform(-kHalf, kHalf), q.Uniform(-kHalf, kHalf)}, 150.0, out);
            return static_cast<double>(out.size());
        });
        std::snprintf(name, sizeof name, "  QueryRadius r=2000 m, %zu pts", count);
        Bench(name, 2000, [&](int) {
            out.clear();
            grid.QueryRadius({q.Uniform(-kHalf, kHalf), q.Uniform(-kHalf, kHalf)}, 2000.0, out);
            return static_cast<double>(out.size());
        });
        std::snprintf(name, sizeof name, "  brute-force scan r=150 m (comparison), %zu pts", count);
        Bench(name, count >= 1'000'000 ? 20 : 200, [&](int) {
            const Vec2 c{q.Uniform(-kHalf, kHalf), q.Uniform(-kHalf, kHalf)};
            std::size_t hits = 0;
            for (const Vec2 p : points) {
                const double dx = p.x - c.x;
                const double dy = p.y - c.y;
                hits += dx * dx + dy * dy <= 150.0 * 150.0 ? 1U : 0U;
            }
            return static_cast<double>(hits);
        });
    }

    // --- Map state ---------------------------------------------------------------------------------------------------------
    MapLayerDef surface;
    surface.id = MapLayerId(1);
    surface.key = Cid("bench:layer.surface");
    surface.bounds = MapRect{{-kHalf, -kHalf}, {kHalf, kHalf}};
    std::vector<PointOfInterestDef> pois;
    Rng prng{3};
    for (int i = 0; i < 20'000; ++i) {
        PointOfInterestDef p;
        p.id = *ContentId::Parse("bench:poi.p" + std::to_string(i));
        p.category = Cid(i % 3 == 0 ? "bench:cat.a" : i % 3 == 1 ? "bench:cat.b" : "bench:cat.c");
        p.position = MapPosition{MapLayerId(1), Vec2{prng.Uniform(-kHalf, kHalf), prng.Uniform(-kHalf, kHalf)}};
        p.visibility = MapVisibility::Always;
        pois.push_back(std::move(p));
    }
    auto def = WorldMapDefinition::Create({surface}, {}, std::move(pois));
    if (!def.HasValue()) {
        std::printf("definition failed\n");
        return 1;
    }
    auto shared = std::make_shared<const WorldMapDefinition>(std::move(def).Value());
    WorldMapState map(shared);
    Rng mrng{5};
    const double beforeMarkers = static_cast<double>(HeapInUse());
    const auto t0 = Clock::now();
    for (int i = 0; i < 100'000; ++i) {
        MarkerSpec s;
        s.position = MapPosition{MapLayerId(1), Vec2{mrng.Uniform(-kHalf, kHalf), mrng.Uniform(-kHalf, kHalf)}};
        s.category = Cid(i % 2 == 0 ? "bench:cat.a" : "bench:cat.b");
        s.label = "marker";
        (void)map.AddMarker(s);
    }
    std::printf("\n[WorldMapState] 20,000 POIs + 100,000 markers: AddMarker %.1f ns/op, heap for markers ~ %.1f MiB (%s)\n",
                std::chrono::duration<double, std::nano>(Clock::now() - t0).count() / 100'000.0,
                (static_cast<double>(HeapInUse()) - beforeMarkers) / 1048576.0, HeapInUse() == 0 ? "mallinfo2 unavailable" : "glibc mallinfo2");
    Rng qrng{11};
    MapFilter onlyA;
    onlyA.categories = {Cid("bench:cat.a")};
    Bench("  QueryRadius r=150 m, no filter", 20000, [&](int) {
        const auto r = map.QueryRadius(MapPosition{MapLayerId(1), {qrng.Uniform(-kHalf, kHalf), qrng.Uniform(-kHalf, kHalf)}}, 150.0);
        return static_cast<double>(r.Value().size());
    });
    Bench("  QueryRadius r=150 m, category filter", 20000, [&](int) {
        const auto r = map.QueryRadius(MapPosition{MapLayerId(1), {qrng.Uniform(-kHalf, kHalf), qrng.Uniform(-kHalf, kHalf)}}, 150.0, onlyA);
        return static_cast<double>(r.Value().size());
    });
    Bench("  QueryRadius r=5000 m (dense: ~3000 hits), no filter", 200, [&](int) {
        const auto r = map.QueryRadius(MapPosition{MapLayerId(1), {qrng.Uniform(-kHalf, kHalf), qrng.Uniform(-kHalf, kHalf)}}, 5000.0);
        return static_cast<double>(r.Value().size());
    });
    Bench("  AddPlayerWaypoint + RemoveMarker", 100000, [&](int i) {
        const auto id = map.AddPlayerWaypoint(MapPosition{MapLayerId(1), {static_cast<double>(i % 1000), 0.0}});
        (void)map.RemoveMarker(id.Value());
        return 1.0;
    });
    {
        const auto t1 = Clock::now();
        const std::string text = map.Serialize();
        const double serMs = std::chrono::duration<double, std::milli>(Clock::now() - t1).count();
        const auto t2 = Clock::now();
        auto loaded = WorldMapState::Deserialize(text, shared);
        const double desMs = std::chrono::duration<double, std::milli>(Clock::now() - t2).count();
        std::printf("  Serialize 100,000 markers: %.1f ms (%.1f MiB text); Deserialize + validate + reindex: %.1f ms (%s)\n", serMs,
                    static_cast<double>(text.size()) / 1048576.0, desMs, loaded.HasValue() ? "ok" : "FAILED");
    }

    // --- Minimap -----------------------------------------------------------------------------------------------------------
    MinimapConfig circle;
    circle.halfWidthPx = 128.0;
    circle.offscreenIndicatorRangeMetres = 500.0;
    circle.indicatorInsetPx = 8.0;
    Rng vrng{13};
    Bench("[Minimap] BuildMinimapFrame circle 128px @1 m/px, HeadingUp, +indicators", 20000, [&](int) {
        const MinimapView view{MapPosition{MapLayerId(1), {vrng.Uniform(-kHalf, kHalf), vrng.Uniform(-kHalf, kHalf)}}, vrng.Uniform(0, 6.28), 1.0,
                               MinimapRotation::HeadingUp};
        const auto frame = BuildMinimapFrame(map, view, circle);
        return static_cast<double>(frame.Value().markers.size() + frame.Value().indicators.size());
    });
    const MinimapTransform transform = MinimapTransform::Create(circle, MinimapView{MapPosition{MapLayerId(1), {1000, 1000}}, 1.0, 1.0, MinimapRotation::HeadingUp}).Value();
    Bench("[Minimap] WorldToScreen (single point)", 5'000'000, [&](int i) { return transform.WorldToScreen({1000.0 + i % 97, 1000.0 - i % 89}).x; });

    // --- Inventory ---------------------------------------------------------------------------------------------------------
    ItemCatalog catalog;
    ItemDefinition coin;
    coin.id = Cid("bench:item.coin");
    coin.category = Cid("bench:cat.currency");
    coin.maxStack = 100;
    ItemDefinition tool;
    tool.id = Cid("bench:item.tool");
    tool.category = Cid("bench:cat.tool");
    tool.weightGrams = 10;
    (void)catalog.Add(coin);
    (void)catalog.Add(tool);
    // Containers keep a FIXED size during each measurement: every timed transaction nets to zero (add, move, remove), so the
    // cost shown is the cost at that container size, not at a size that grows while measuring.
    for (const int stacks : {10, 100, 1000, 10000}) {
        InventoryState inv(catalog);
        (void)inv.Apply(CreateContainer{ContainerId(1), ContainerRules{}});
        (void)inv.Apply(CreateContainer{ContainerId(2), ContainerRules{}});
        for (int i = 0; i < stacks; ++i) {
            (void)inv.Apply(AddItem{ContainerId(1), tool.id, 1, {}});
        }
        char name[112];
        const int ops = stacks >= 10000 ? 300 : 2000;
        std::snprintf(name, sizeof name, "[Inventory] AddItem + RemoveItem (2 transactions per op), %d stacks", stacks);
        Bench(name, ops, [&](int) {
            const ItemInstanceId id = inv.NextInstanceId();
            const auto added = inv.Apply(AddItem{ContainerId(1), tool.id, 1, {}});
            const auto removed = inv.Apply(RemoveItem{ContainerId(1), id, 1});
            return added.HasValue() && removed.HasValue() ? 0.5 : 0.0; // two single-command transactions per op
        });
        std::snprintf(name, sizeof name, "[Inventory] 3-command transaction add/transfer/remove, %d stacks", stacks);
        Bench(name, ops, [&](int) {
            const ItemInstanceId id = inv.NextInstanceId();
            const auto r = inv.ApplyAll({AddItem{ContainerId(1), tool.id, 1, {}}, TransferItem{ContainerId(1), ContainerId(2), id, 1},
                                         RemoveItem{ContainerId(2), id, 1}});
            return r.HasValue() ? 1.0 : 0.0;
        });
        std::snprintf(name, sizeof name, "[Inventory] failing 2-command transaction (rollback), %d stacks", stacks);
        Bench(name, ops, [&](int) {
            const auto r = inv.ApplyAll({AddItem{ContainerId(1), tool.id, 1, {}}, RemoveItem{ContainerId(1), ItemInstanceId(999'999'999), 1}});
            return r.HasValue() ? 1.0 : 0.0;
        });
    }
    {
        InventoryState inv(catalog);
        (void)inv.Apply(CreateContainer{ContainerId(1), ContainerRules{}});
        for (int i = 0; i < 1000; ++i) {
            (void)inv.Apply(AddItem{ContainerId(1), tool.id, 1, {}});
        }
        const auto t3 = Clock::now();
        const std::string text = inv.Serialize();
        const double ser = std::chrono::duration<double, std::milli>(Clock::now() - t3).count();
        const auto t4 = Clock::now();
        auto loaded = InventoryState::Deserialize(text, catalog);
        std::printf("[Inventory] 1,000 stacks: Serialize %.2f ms (%.1f KiB), Deserialize + invariants %.2f ms (%s)\n", ser,
                    static_cast<double>(text.size()) / 1024.0, std::chrono::duration<double, std::milli>(Clock::now() - t4).count(),
                    loaded.HasValue() ? "ok" : "FAILED");
    }
    (void)baseline;
    return 0;
}
