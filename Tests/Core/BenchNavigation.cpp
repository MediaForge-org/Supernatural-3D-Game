// Hand-run micro-benchmark for routing and map frame building on SYNTHETIC data (not a benchmark of the final world).
// Usage: spn_core_bench_navigation   (prints milliseconds; numbers depend on the machine and build type - run a Release build for meaningful values)
#include <chrono>
#include <cstdio>
#include <spn/core/MapRender.h>
#include <spn/core/Routing.h>
#include <spn/core/SyntheticMapRegion.h>

using namespace spn::core;
using Clock = std::chrono::steady_clock;

namespace {
double Ms(Clock::time_point a, Clock::time_point b) { return std::chrono::duration<double, std::milli>(b - a).count(); }
}

int main() {
    // street grid n x n
    for (const int n : {100, 300, 600}) {
        const MapLayerId layer(1);
        MapLayerDef l;
        l.id = layer;
        l.key = *ContentId::Parse("synthetic:layer.bench");
        l.bounds = MapRect{{-10000, -10000}, {200000, 200000}};
        auto def = WorldMapDefinition::Create({l}, {}, {});
        MapGeometryData d;
        const auto id = [&](int r, int c) { return static_cast<std::uint64_t>(r * n + c + 1); };
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) { d.nodes.push_back(RoadNodeDef{RoadNodeId(id(r, c)), MapPosition{layer, Vec2{c * 50.0, r * 50.0}}}); }
        }
        std::uint64_t seg = 1;
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                for (const auto& [dr, dc] : {std::pair{0, 1}, std::pair{1, 0}}) {
                    if (r + dr >= n || c + dc >= n) { continue; }
                    RoadSegmentDef s;
                    s.id = RoadSegmentId(seg++);
                    s.from = RoadNodeId(id(r, c));
                    s.to = RoadNodeId(id(r + dr, c + dc));
                    d.segments.push_back(s);
                }
            }
        }
        const auto t0 = Clock::now();
        auto geometry = MapGeometry::Create(def.Value(), std::move(d), 500.0);
        const auto t1 = Clock::now();
        if (!geometry) { std::puts("geometry failed"); return 1; }
        RouteOptions o;
        o.mode = TravelMode::Driving;
        Destination dest;
        dest.position = MapPosition{layer, Vec2{(n - 1) * 50.0, (n - 1) * 50.0}};
        const auto t2 = Clock::now();
        const auto plan = PlanRoute(geometry.Value(), MapPosition{layer, Vec2{0, 0}}, dest, o);
        const auto t3 = Clock::now();
        Destination shortDest;
        shortDest.position = MapPosition{layer, Vec2{(n / 2) * 50.0 + 150.0, (n / 2) * 50.0 + 100.0}};
        const auto t4 = Clock::now();
        const auto shortPlan = PlanRoute(geometry.Value(), MapPosition{layer, Vec2{(n / 2) * 50.0, (n / 2) * 50.0}}, shortDest, o);
        const auto t5 = Clock::now();
        MinimapConfig vp;
        vp.shape = MinimapShape::Rectangle;
        vp.halfWidthPx = 256;
        vp.halfHeightPx = 256;
        MinimapView view;
        view.player = MapPosition{layer, Vec2{(n / 2) * 50.0, (n / 2) * 50.0}};
        const auto t6 = Clock::now();
        std::size_t emitted = 0;
        for (int i = 0; i < 100; ++i) {
            view.player.xy.x += 3.0;
            const auto frame = BuildMapRenderFrame(geometry.Value(), nullptr, view, vp, {});
            emitted += frame.HasValue() ? frame.Value().stats.emitted : 0;
        }
        const auto t7 = Clock::now();
        std::printf("grid %dx%d (%zu segments): build %.1f ms | corner-to-corner %.1f ms (%zu states, %.0f m) | short route %.3f ms (%zu states) | map frame %.3f ms avg (%zu features avg)\n", n, n,
                    geometry.Value().Roads().Segments().size(), Ms(t0, t1), Ms(t2, t3), plan.HasValue() ? plan.Value().stats.statesExpanded : 0,
                    plan.HasValue() ? plan.Value().lengthMetres : 0.0, Ms(t4, t5), shortPlan.HasValue() ? shortPlan.Value().stats.statesExpanded : 0, Ms(t6, t7) / 100.0,
                    emitted / 100);
    }
    return 0;
}
