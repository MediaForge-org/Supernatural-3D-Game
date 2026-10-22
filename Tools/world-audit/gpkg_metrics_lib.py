"""Library part of gpkg_metrics.py (WKB reader and metrics)."""
"""Area / length metrics per feature of the WORLD_700 GeoPackage (read-only, stdlib only).
GeoPackage geometry = header + WKB; coordinates are metres in OWP-LCS-1 (planar), so
area = shoelace in m^2, length = Euclidean polyline length.
Usage: gpkg_metrics.py <path.gpkg> table [table ...]   -> TSV: table, feature_id, name, class, state, n, area_km2, length_km, cx_m, cy_m"""
import sqlite3, struct, sys, json, math

def read_wkb(b, o=0):
    bo = b[o]; e = "<" if bo == 1 else ">"; t = struct.unpack_from(e + "I", b, o + 1)[0] % 1000; o += 5
    if t == 1:
        x, y = struct.unpack_from(e + "dd", b, o); return ("P", [(x, y)]), o + 16
    if t == 2:
        n = struct.unpack_from(e + "I", b, o)[0]; o += 4
        pts = [struct.unpack_from(e + "dd", b, o + 16 * i) for i in range(n)]; return ("L", [pts]), o + 16 * n
    if t == 3:
        nr = struct.unpack_from(e + "I", b, o)[0]; o += 4; rings = []
        for _ in range(nr):
            n = struct.unpack_from(e + "I", b, o)[0]; o += 4
            rings.append([struct.unpack_from(e + "dd", b, o + 16 * i) for i in range(n)]); o += 16 * n
        return ("A", [rings]), o
    if t in (4, 5, 6, 7):
        n = struct.unpack_from(e + "I", b, o)[0]; o += 4; kind = None; parts = []
        for _ in range(n):
            (k, p), o = read_wkb(b, o); kind = k; parts += p
        return (kind, parts), o
    raise ValueError(t)

def ring_area(r): return sum(r[i][0] * r[i + 1][1] - r[i + 1][0] * r[i][1] for i in range(len(r) - 1)) / 2

def metrics(blob):
    flags = blob[3]; env = {0: 0, 1: 32, 2: 48, 3: 48, 4: 64}[(flags >> 1) & 7]
    (k, parts), _ = read_wkb(blob, 8 + env)
    area = length = 0.0; pts = []
    for p in parts:
        if k == "A":
            area += abs(ring_area(p[0])) - sum(abs(ring_area(h)) for h in p[1:]); pts += p[0]
        elif k == "L":
            length += sum(math.dist(p[i], p[i + 1]) for i in range(len(p) - 1)); pts += p
        else: pts += [p] if isinstance(p, tuple) else p
    xs = [q[0] for q in pts]; ys = [q[1] for q in pts]
    return k, len(parts), area / 1e6, length / 1e3, sum(xs) / len(xs), sum(ys) / len(ys)

