"""Runs the standalone terrain tool for the layout and loads its result (pure Python; usable inside and outside Unreal).

    python terrain_gen.py <out_dir> [<path to spn_worldtool.exe>]
"""

import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import layout  # noqa: E402


def default_tool():
    repo = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", ".."))
    for sub in ("build/worldtool/Release/spn_worldtool.exe", "build/worldtool/spn_worldtool.exe", "build/worldtool/spn_worldtool"):
        path = os.path.join(repo, sub)
        if os.path.exists(path):
            return path
    raise RuntimeError("spn_worldtool not built: cmake -S Tools/SpnWorldTool -B build/worldtool && cmake --build build/worldtool --config Release")


def main_street(roads):
    return layout.main_street(roads)


def generate(out_dir, tool=None):
    """Returns (roads, lots, terrain, creek polyline with z)."""
    os.makedirs(out_dir, exist_ok=True)
    roads = layout.build_roads()
    lots = layout.build_lots(roads)
    # Paved ground of the lots (drives, paths, parking) comes from the building generators, which only load inside Unreal;
    # outside Unreal the list written by the last world build is reused.
    paved_path = os.path.join(out_dir, "paved.txt")
    try:
        import buildings
        paved = buildings.paved_rects(roads, lots)
        with open(paved_path, "w") as f:
            f.write("\n".join(" ".join("%.2f" % v for v in rect) for rect in paved))
    except ImportError:
        paved = []
        if os.path.exists(paved_path):
            with open(paved_path) as f:
                paved = [tuple(float(v) for v in line.split()) for line in f if line.strip()]
    spec, creek = layout.terrain_spec(roads, lots, paved)
    spec_path = os.path.join(out_dir, "terrain_spec.txt")
    with open(spec_path, "w") as f:
        f.write(spec)
    result = subprocess.run([tool or default_tool(), "terrain", spec_path, out_dir], capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError("terrain tool failed: " + result.stdout + result.stderr)
    terrain = layout.Terrain(out_dir)
    for rd in roads.values():
        rd.z = terrain.heights[("road", rd.name)]
    for lot in lots:
        lot.z = terrain.heights[("pad", lot.name)]
    creek_z = terrain.heights[("creek", "creek")]
    creek3 = [(p[0], p[1], z) for p, z in zip(creek, creek_z)]
    return roads, lots, terrain, creek3


if __name__ == "__main__":
    roads, lots, terrain, creek = generate(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else None)
    print("roads:", {name: "%.0f m, z %.1f..%.1f m" % (rd.length / 100, min(rd.z) / 100, max(rd.z) / 100) for name, rd in roads.items()})
    print("lots:", len(lots), "main street s:", main_street(roads))
    worst = max(max(abs(b - a) / max(1.0, rd.s[i + 1] - rd.s[i]) for i, (a, b) in enumerate(zip(rd.z, rd.z[1:]))) for rd in roads.values())
    print("steepest road grade: %.1f %%" % (worst * 100))
