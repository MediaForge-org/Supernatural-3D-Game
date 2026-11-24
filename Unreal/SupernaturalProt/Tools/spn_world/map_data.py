"""Map data of the PLACEHOLDER world SpnWorld for the in-game map and minimap (night build 2026-11-14).

Pure Python (no Unreal import): reads the same layout the world generator builds from (layout.build_roads / build_lots / CREEK / POND) and
writes Content/SpnWorld/Data/SpnWorldMap.json. Deterministic: same layout -> byte-identical file. Called by build_world.py (phase H) and
runnable alone:  python map_data.py

All coordinates are UNREAL CENTIMETRES (x, y); the game converts them to map metres with its world frame (SpnWorldFrame, EastSouthUp).
This is placeholder test data, not geography and not a map candidate (CLAUDE.md section 3).

Format (version 1):
  bounds_cm   [min_x, min_y, max_x, max_y] of the playable landscape
  nodes       [{id, x, y}]                       road graph nodes (road ends and junctions)
  segments    [{id, from, to, class, name, shape: [[x, y], ...]}]   shape = vertices strictly between the nodes
  buildings   [{id, label, category, footprint: [[x, y] x4], entrance: [x, y]}]
  areas       [{id, kind, label, outline: [[x, y], ...]}]
  lines       [{id, kind, label, points: [[x, y], ...]}]
  regions     [{id, label, min: [x, y], max: [x, y]}]
  pois        [{id, category, label, x, y, visibility}]   visibility: always | known | hidden
"""

import json
import math
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import layout  # noqa: E402

OUT = os.path.normpath(os.path.join(HERE, "..", "..", "Content", "SpnWorld", "Data", "SpnWorldMap.json"))
ROAD_CLASS = {"highway": "secondary", "street": "residential", "gravel": "field_track", "track": "forest_track"}
ROAD_NAME = {"hwy": "Highway (placeholder)", "oak": "Oak Street", "mill": "Mill Road", "farm": "Farm Lane", "elm": "Elm Street",
             "birch": "Birch Street", "pine": "Pine Track"}
JOIN_CM = 600.0
SIMPLIFY_CM = 800.0
# ids the earlier temporary test definition used in this level; kept HIDDEN so map state in older SpnWorld saves still loads (never shown)
LEGACY_IDS = ["spn_test:poi.player_start", "spn_test:poi.birkenweg", "spn_test:poi.storage_road", "spn_test:poi.lagerstrasse",
              "spn_test:poi.kurvenweg", "spn_test:poi.workshop_lot", "spn_test:poi.lindenweg_end", "spn_test:poi.hofweg_end",
              "spn_test:poi.east_lot", "spn_test:region.district"]
LANDMARKS = {"gas": "Gas station", "motel": "Motel", "church": "Church", "cabin": "Cabin", "diner": "Diner", "barn": "Farm",
             "tower": "Water tower", "garage": "Garage", "hall": "Town hall"}


def r(v):
    return round(v, 1)


def seg_intersection(p, q, a, b):
    d1 = (q[0] - p[0], q[1] - p[1])
    d2 = (b[0] - a[0], b[1] - a[1])
    den = d1[0] * d2[1] - d1[1] * d2[0]
    if abs(den) < 1e-9:
        return None
    t = ((a[0] - p[0]) * d2[1] - (a[1] - p[1]) * d2[0]) / den
    u = ((a[0] - p[0]) * d1[1] - (a[1] - p[1]) * d1[0]) / den
    if 0.0 <= t <= 1.0 and 0.0 <= u <= 1.0:
        return (t, u, (p[0] + d1[0] * t, p[1] + d1[1] * t))
    return None


def project(pts, pt):
    """(index of segment, t, point, distance) of the nearest point on the polyline."""
    best = None
    for i in range(len(pts) - 1):
        a, b = pts[i], pts[i + 1]
        dx, dy = b[0] - a[0], b[1] - a[1]
        l2 = dx * dx + dy * dy
        t = max(0.0, min(1.0, ((pt[0] - a[0]) * dx + (pt[1] - a[1]) * dy) / l2)) if l2 > 0 else 0.0
        c = (a[0] + dx * t, a[1] + dy * t)
        d = math.dist(c, pt)
        if best is None or d < best[3]:
            best = (i, t, c, d)
    return best


def road_graph(roads):
    names = sorted(roads)
    # polylines with float "position along" keys so junction points can be inserted: {name: [(key, (x, y))]}
    lines = {n: [(float(i), (p[0], p[1])) for i, p in enumerate(roads[n].pts)] for n in names}
    cuts = {n: set() for n in names}

    def add_cut(name, seg_index, t, point):
        key = seg_index + t
        lines[name].append((key, point))
        cuts[name].add(key)
        return key

    for ai, an in enumerate(names):
        for bn in names[ai + 1:]:
            pa = [p for _, p in sorted(lines[an])]
            pb = [p for _, p in sorted(lines[bn])]
            found = False
            # crossings of the two polylines
            for i in range(len(pa) - 1):
                for j in range(len(pb) - 1):
                    hit = seg_intersection(pa[i], pa[i + 1], pb[j], pb[j + 1])
                    if hit:
                        add_cut(an, i, hit[0], hit[2])
                        add_cut(bn, j, hit[1], hit[2])
                        found = True
            if found:
                continue
            # T-junctions: a road end close to the other road
            for end_name, other, pe, po in ((an, bn, pa, pb), (bn, an, pb, pa)):
                for end_index in (0, len(pe) - 1):
                    i, t, c, d = project(po, pe[end_index])
                    if d <= JOIN_CM:
                        add_cut(other, i, t, c)
                        # move the road end onto the junction point
                        lines[end_name] = [(k, c if k == float(end_index) else p) for k, p in lines[end_name]]
                        cuts[end_name].add(float(end_index))
    nodes, node_ids, segments = [], {}, []

    def node_for(p):
        key = (round(p[0] / 50.0), round(p[1] / 50.0))
        if key not in node_ids:
            node_ids[key] = len(nodes) + 1
            nodes.append({"id": node_ids[key], "x": r(p[0]), "y": r(p[1])})
        return node_ids[key]

    for n in names:
        pts = sorted(lines[n])
        keys = [k for k, _ in pts]
        cut_set = cuts[n] | {keys[0], keys[-1]}
        start = 0
        for idx in range(1, len(pts)):
            if keys[idx] in cut_set:
                piece = [p for _, p in pts[start:idx + 1]]
                shape, last = [], piece[0]
                for p in piece[1:-1]:
                    if math.dist(p, last) >= SIMPLIFY_CM:
                        shape.append([r(p[0]), r(p[1])])
                        last = p
                a, b = node_for(piece[0]), node_for(piece[-1])
                if a != b and math.dist(piece[0], piece[-1]) > 1.0:
                    segments.append({"id": len(segments) + 1, "from": a, "to": b, "class": ROAD_CLASS[roads[n].kind],
                                     "name": ROAD_NAME.get(n, n), "shape": shape})
                start = idx
    return nodes, segments


def building(lot, index):
    w = lot.width * 0.5
    ring = [lot.to_world(-w, 0.0), lot.to_world(w, 0.0), lot.to_world(w, lot.depth), lot.to_world(-w, lot.depth)]
    # counter-clockwise in map metres (y north = -Unreal y): order is not interpreted, a simple polygon is enough
    return {"id": index, "label": LANDMARKS.get(lot.name, ""), "category": "spnworld:building." + lot.kind,
            "footprint": [[r(x), r(y)] for x, y in ring], "entrance": [r(c) for c in lot.to_world(0.0, 0.0)]}


def build_data():
    roads = layout.build_roads()
    lots = layout.build_lots(roads)
    nodes, segments = road_graph(roads)
    buildings = [building(lot, i + 1) for i, lot in enumerate(lots)]
    pond = layout.POND
    outline = [[r(pond["x"] + pond["radius"] * math.cos(a * math.pi / 12)), r(pond["y"] + pond["radius"] * math.sin(a * math.pi / 12))] for a in range(24)]
    creek = [[r(x * layout.M), r(y * layout.M)] for x, y in layout.CREEK]
    e = layout.EXTENT
    pois = [{"id": "spnworld:poi.route_start", "category": "spnworld:marker.landmark", "label": "Oak Street (start)", "x": -7348.7, "y": 16531.3, "visibility": "always"},
            {"id": "spnworld:poi.pond", "category": "spnworld:marker.landmark", "label": "Pond", "x": r(pond["x"]), "y": r(pond["y"]), "visibility": "known"}]
    for lot in lots:
        if lot.name in LANDMARKS:
            pois.append({"id": "spnworld:poi." + lot.name, "category": "spnworld:marker.building", "label": LANDMARKS[lot.name],
                         "x": r(lot.x), "y": r(lot.y), "visibility": "always" if lot.name == "cabin" else "known"})
    for legacy in LEGACY_IDS:
        pois.append({"id": legacy, "category": "spn_test:marker.legacy", "label": "legacy (hidden)", "x": 0.0, "y": 0.0, "visibility": "hidden"})
    pois.sort(key=lambda p: p["id"])
    tr = layout.TOWN_RADIUS
    return {
        "version": 1, "placeholder": True, "source": "Tools/spn_world/map_data.py (layout.py, seed %d)" % layout.SEED,
        "bounds_cm": [-e, -e, e, e],
        "nodes": nodes, "segments": segments, "buildings": buildings,
        "areas": [{"id": 1, "kind": "water", "label": "Pond", "outline": outline}],
        "lines": [{"id": 1, "kind": "stream", "label": "Creek", "points": creek}],
        "regions": [{"id": "spnworld:region.valley", "label": "Valley (placeholder)", "min": [-e, -e], "max": [e, e]},
                    {"id": "spnworld:region.town", "label": "Town (placeholder)", "min": [-tr, -tr], "max": [tr, tr]}],
        "pois": pois,
    }


def write(path=OUT):
    data = build_data()
    os.makedirs(os.path.dirname(path), exist_ok=True)
    text = json.dumps(data, indent=1, sort_keys=True) + "\n"
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    return path, data


if __name__ == "__main__":
    p, d = write()
    print("%s: %d nodes, %d segments, %d buildings, %d pois" % (p, len(d["nodes"]), len(d["segments"]), len(d["buildings"]), len(d["pois"])))
