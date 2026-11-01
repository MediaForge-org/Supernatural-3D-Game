# Performance measurements (NightBuild02)

**These are measurements of SYNTHETIC street grids, not of the final world** (about 2,200 km2 is a working scenario; no real data exists yet). Machine: Fedora laptop (Intel Core Ultra 7 256V), clang 22 **Release**, one thread, `spn_core_bench_navigation` (`Tests/Core/BenchNavigation.cpp`, hand-run, not part of CTest). Debug-build numbers are meaningless and were not recorded.

| Grid (nodes) | Segments | Build `MapGeometry::Create` | Corner-to-corner route | Short route (28 states) | Map frame (~250 features) |
|---|---|---|---|---|---|
| 100 x 100 | 19,800 | 14.7 ms | 23.2 ms (39,297 states) | 0.17 ms | 0.18 ms |
| 300 x 300 | 179,400 | 148 ms | 200 ms (357,897 states) | 0.15 ms | 0.17 ms |
| 600 x 600 | 718,800 | 587 ms | 816 ms (1,435,797 states) | 0.16 ms | 0.17 ms |

* Build time is linear in the segment count. **The first run found two real defects** that these numbers confirm as fixed: `MapGeometry::Create` looked up each segment's start node linearly (quadratic: 251 s at 720k segments, now 0.6 s), and every route request scanned all segments once for a layer check (57 ms for a 28-state route, now 0.16 ms and constant).
* A short route and a map frame cost the same at 20k and 720k segments: no per-frame or per-request work depends on the world size.
* Corner-to-corner on a uniform grid is the worst case for a Euclidean-heuristic A\* (all monotone paths tie, so most of the grid is expanded). Real road networks are far from this case, but a continent-scale cross-region route will want hierarchical routing (see NEXT_STEPS). The 720k-segment corner-to-corner search is about 60 km of grid and takes 0.8 s.
* Memory: not measured. Per segment the network stores a polyline copy (`points`, `cumulative`) in addition to the definition; a budget for the full world is **not** established.
* Frame-rate behaviour of steering, measured with the quick test in `GroundSteeringTests.cpp`: speed dip and turn time of a sprint turn differ by less than 1.5 % between 20 and 1000 fps.
