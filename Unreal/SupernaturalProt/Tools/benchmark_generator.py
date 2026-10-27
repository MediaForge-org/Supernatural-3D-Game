"""Reproducible Fedora-side measurements of the map and building generators (no Unreal involved).

Run from Unreal/SupernaturalProt:  python3 Tools/benchmark_generator.py [repeats]
Prints wall-clock time (best and median of `repeats`), peak Python memory (tracemalloc, one run), output sizes and the planned
actor reduction. These are GENERATOR numbers on this machine, not Unreal performance: frame rate, load time and draw calls need
an engine run (Windows).
"""

import json
import os
import statistics
import sys
import time
import tracemalloc

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import spn_building_export as X  # noqa: E402
import spn_building_roles as R  # noqa: E402
import spn_building_templates as T  # noqa: E402
import spn_district_layout as D  # noqa: E402
import spn_instancing as I  # noqa: E402
import spn_map_validate as V  # noqa: E402
import spn_movement_map_layout as L  # noqa: E402


def timed(fn, repeats):
    runs = []
    for _ in range(repeats):
        t = time.perf_counter()
        fn()
        runs.append(time.perf_counter() - t)
    return min(runs), statistics.median(runs)


def main():
    repeats = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    best, median = timed(L.build_layout, repeats)
    print("v1 layout:            best %.1f ms, median %.1f ms" % (best * 1e3, median * 1e3))
    best, median = timed(D.build_layout_v2, repeats)
    print("v2 layout:            best %.1f ms, median %.1f ms" % (best * 1e3, median * 1e3))
    data = D.build_layout_v2()
    text = L.to_json(data)
    best, median = timed(lambda: L.to_json(D.build_layout_v2()), repeats)
    print("v2 layout + JSON:     best %.1f ms, median %.1f ms, %d KiB" % (best * 1e3, median * 1e3, len(text) // 1024))
    tracemalloc.start()
    D.build_layout_v2()
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    print("v2 peak Python memory: %.1f MiB" % (peak / 2**20))
    print("v2 primitives: %d (v1 %d), doors %d, lights %d" % (len(data["prims"]), len(L.build_layout()["prims"]), len(data["doors"]), len(data["lights"])))
    print("instancing plan:", json.dumps(I.summarize(data)))
    best, _ = timed(lambda: V.validate_layout_data(data), repeats)
    print("layout data validation: best %.1f ms" % (best * 1e3))
    best, _ = timed(lambda: V.MapGrid(data), max(repeats // 2, 1))
    grid = V.MapGrid(data)
    print("map grid (100 cm, %d cells, %d blocked): best %.1f ms" % (grid.n * grid.n, len(grid.blocked), best * 1e3))
    best, _ = timed(lambda: D.check_routes(data, V.MapGrid(data)), max(repeats // 2, 1))
    print("grid + 13 named routes (one labelling pass; before caching: 13 flood fills ~1.1 s): best %.1f ms" % (best * 1e3))
    best, _ = timed(lambda: I.plan_instancing(data["prims"]), repeats)
    print("instancing plan: best %.2f ms" % (best * 1e3))
    house = T.generate(T.TEMPLATES["house"], T.make_variants(T.TEMPLATES["house"], 1, 3)[0])
    best, _ = timed(lambda: X.export_building(house, strict=False), repeats)
    print("export (house, no checks): best %.1f ms" % (best * 1e3))
    best, _ = timed(lambda: X.export_building(house), max(repeats // 2, 1))
    print("export (house, with geometric checks): best %.1f ms" % (best * 1e3))
    for name, template in sorted(T.TEMPLATES.items()):
        params = T.make_variants(template, 12, 20261027)
        best, _ = timed(lambda: [T.generate(template, p) for p in params], max(repeats // 2, 1))
        specs = [T.generate(template, p) for p in params]
        best_check, _ = timed(lambda: [R.check_building(s) for s in specs], 1)
        print("%-10s 12 variants: generate %.1f ms, full geometric check %.0f ms" % (name, best * 1e3, best_check * 1e3))


if __name__ == "__main__":
    main()
