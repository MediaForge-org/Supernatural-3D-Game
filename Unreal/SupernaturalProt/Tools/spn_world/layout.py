"""Layout of the PLACEHOLDER test world of the visual build (pure Python, no Unreal import).

A small highway town in a wooded valley, about 1.26 km x 1.26 km (1.59 km2). It is a test environment for look, lighting, traversal and
performance: no canon location, no final map, no decision about the game's world (CLAUDE.md section 3). Everything is data in this file so
the layout can be replaced without touching the generators.

Units: centimetres. X east, Y north is only a naming convention here. Roads are smooth curves through hand-placed control points; buildings,
poles, lamps and signs are placed relative to a road (distance along it, side, setback), so they follow its curves.
"""

import math
import random

GRID_SAMPLES = 1009
GRID_QUAD = 125.0
Z_SCALE = 100.0
EXTENT = (GRID_SAMPLES - 1) * GRID_QUAD * 0.5          # 63000 cm
COMPONENTS = 16
QUADS_PER_COMPONENT = 63
SEED = 11
TOWN_CENTRE = (0.0, 0.0)
TOWN_RADIUS = 27000.0
M = 100.0


def catmull(points, step=400.0):
    """Smooth polyline through the control points, sampled about every `step` cm."""
    pts = [points[0]] + list(points) + [points[-1]]
    out = []
    for i in range(1, len(pts) - 2):
        p0, p1, p2, p3 = pts[i - 1], pts[i], pts[i + 1], pts[i + 2]
        n = max(2, int(math.dist(p1, p2) / step))
        for k in range(n):
            t = k / n
            t2, t3 = t * t, t * t * t
            out.append(tuple(0.5 * ((2 * p1[a]) + (-p0[a] + p2[a]) * t + (2 * p0[a] - 5 * p1[a] + 4 * p2[a] - p3[a]) * t2 + (-p0[a] + 3 * p1[a] - 3 * p2[a] + p3[a]) * t3) for a in (0, 1)))
    out.append(tuple(points[-1]))
    return out


class Road:
    """kind: highway | street | gravel | track. half = half width of the driving surface."""

    def __init__(self, name, kind, half, control, shoulder=120.0, blend=800.0, ditch=False):
        self.name, self.kind, self.half, self.shoulder, self.blend, self.ditch = name, kind, half, shoulder, blend, ditch
        self.pts = catmull([(x * M, y * M) for x, y in control])
        self.z = [0.0] * len(self.pts)
        self.s = [0.0]
        for a, b in zip(self.pts, self.pts[1:]):
            self.s.append(self.s[-1] + math.dist(a, b))
        self.length = self.s[-1]

    def at(self, s):
        """(x, y, z, tangent_x, tangent_y) at distance s along the road."""
        s = min(max(s, 0.0), self.length)
        lo, hi = 0, len(self.s) - 1
        while hi - lo > 1:
            mid = (lo + hi) // 2
            if self.s[mid] <= s:
                lo = mid
            else:
                hi = mid
        a, b = self.pts[lo], self.pts[lo + 1]
        seg = self.s[lo + 1] - self.s[lo]
        t = (s - self.s[lo]) / seg if seg > 0 else 0.0
        tx, ty = (b[0] - a[0]) / seg, (b[1] - a[1]) / seg
        return a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, self.z[lo] + (self.z[lo + 1] - self.z[lo]) * t, tx, ty

    def offset(self, s, side):
        """World position `side` cm to the left (positive) or right (negative) of the road at s, and the yaw (degrees) of the tangent."""
        x, y, z, tx, ty = self.at(s)
        return x - ty * side, y + tx * side, z, math.degrees(math.atan2(ty, tx))

    def nearest_s(self, px, py):
        best, best_s = 1e18, 0.0
        for i in range(len(self.pts) - 1):
            a, b = self.pts[i], self.pts[i + 1]
            dx, dy = b[0] - a[0], b[1] - a[1]
            l2 = dx * dx + dy * dy
            t = max(0.0, min(1.0, ((px - a[0]) * dx + (py - a[1]) * dy) / l2)) if l2 > 0 else 0.0
            d = (a[0] + dx * t - px) ** 2 + (a[1] + dy * t - py) ** 2
            if d < best:
                best, best_s = d, self.s[i] + math.sqrt(l2) * t
        return best_s, math.sqrt(best)


def build_roads():
    r = {}
    r["hwy"] = Road("hwy", "highway", 370.0, [(-630, 95), (-520, 80), (-400, 50), (-290, 10), (-180, -5), (-60, 0), (60, 0), (160, 8), (250, 40), (340, 95), (440, 150), (540, 185), (630, 200)],
                    shoulder=170.0, blend=900.0, ditch=True)
    r["oak"] = Road("oak", "street", 300.0, [(-48, -240), (-55, -150), (-60, -70), (-60, 0), (-62, 70), (-70, 140), (-85, 210), (-95, 270)], shoulder=90.0, blend=600.0)
    r["mill"] = Road("mill", "street", 300.0, [(70, 0), (72, -70), (80, -140), (100, -200)], shoulder=90.0, blend=600.0)
    r["farm"] = Road("farm", "gravel", 210.0, [(100, -200), (140, -250), (200, -285), (250, -300), (292, -302)], shoulder=60.0, blend=600.0)
    r["elm"] = Road("elm", "street", 290.0, [(-70, 140), (0, 150), (60, 150), (118, 138), (148, 100), (152, 55), (150, 7)], shoulder=90.0, blend=600.0)
    r["birch"] = Road("birch", "street", 280.0, [(-55, -150), (-110, -160), (-170, -150), (-215, -165)], shoulder=90.0, blend=600.0)
    r["pine"] = Road("pine", "track", 150.0, [(-95, 270), (-130, 330), (-190, 380), (-250, 410), (-292, 432)], shoulder=40.0, blend=500.0)
    return r


def main_street(roads):
    """The part of the highway inside the town (kerbs, pavements, street lamps): start and end distance along "hwy", cm."""
    hwy = roads["hwy"]
    return hwy.nearest_s(-15500.0, -300.0)[0], hwy.nearest_s(16500.0, 900.0)[0]

CREEK = [(330, 630), (350, 500), (390, 380), (410, 260), (402, 132), (380, 20), (350, -100), (360, -220), (420, -330), (470, -450), (520, -630)]
POND = {"name": "pond", "x": -25200.0, "y": 48800.0, "radius": 2600.0, "depth": 170.0}
CLEARINGS = [(-30000.0, 45500.0, 5200.0), (27500.0, -32500.0, 8000.0), (-12500.0, 19500.0, 5000.0), (-20500.0, 15500.0, 2600.0), (45000.0, 30000.0, 6000.0), (-42000.0, -12000.0, 7000.0)]


class Lot:
    """One building site. Placed relative to a road: s along it, side +1 left / -1 right, setback from the road centre line to the front."""

    def __init__(self, name, kind, road, s, side, setback, width, depth, yaw_jitter=0.0, **params):
        self.name, self.kind, self.road, self.s, self.side, self.setback = name, kind, road, s, side, setback
        self.width, self.depth, self.yaw_jitter, self.params = width, depth, yaw_jitter, params
        self.x = self.y = self.z = self.yaw = 0.0
        self.anchor = (0.0, 0.0)

    def resolve(self, roads):
        rd = roads[self.road]
        fx, fy, _, tangent = rd.offset(self.s, self.side * self.setback)
        ax, ay, _, _ = rd.offset(self.s, self.side * (rd.half + 60.0))
        # building frame: origin = centre of the front edge, local +X along the front, local +Y from the front into the lot (away from the road).
        # local Y = (-sin yaw, cos yaw) must equal side * left normal of the road, hence:
        self.yaw = tangent + (0.0 if self.side > 0 else 180.0) + self.yaw_jitter
        rad = math.radians(self.yaw)
        self.fx, self.fy = fx, fy
        self.x = fx - math.sin(rad) * self.depth * 0.5
        self.y = fy + math.cos(rad) * self.depth * 0.5
        self.anchor = (ax, ay)

    def to_world(self, lx, ly):
        """Local building coordinates (x along the front, y into the lot, origin = centre of the front edge) to world."""
        rad = math.radians(self.yaw)
        c, s = math.cos(rad), math.sin(rad)
        return self.fx + lx * c - ly * s, self.fy + lx * s + ly * c


def build_lots(roads):
    """Hand-placed sites. s values are metres along the named road (see build_roads for the directions)."""
    rng = random.Random(SEED)
    lots = []

    def add(name, kind, road, s_m, side, setback_m, width_m, depth_m, jitter=0.0, **params):
        lot = Lot(name, kind, road, s_m * M, side, setback_m * M, width_m * M, depth_m * M, jitter, **params)
        lot.resolve(roads)
        lots.append(lot)
        return lot

    hwy = roads["hwy"]
    # --- main street (highway through the town): row buildings close to the pavement
    oak_s = hwy.nearest_s(-6000.0, 0.0)[0] / M
    mill_s = hwy.nearest_s(7000.0, 0.0)[0] / M
    front = 9.2
    shops_n = [("store_n1", 11.5, 2, "brick"), ("store_n2", 9.0, 1, "plaster"), ("store_n3", 12.5, 2, "brick"), ("store_n4", 8.5, 2, "siding"), ("store_n5", 13.0, 1, "brick"), ("store_n6", 10.0, 2, "plaster"), ("store_n7", 11.0, 1, "brick")]
    cursor = oak_s + 13.0
    for index, (name, width, floors, wall) in enumerate(shops_n):
        gap = 3.2 if index in (3,) else 0.25
        cursor += gap
        add(name, "shop", "hwy", cursor + width / 2, +1, front, width, 13.0 + rng.uniform(-1.5, 2.5), 0.0, floors=floors, wall=wall, seed=100 + index)
        cursor += width
    shops_s = [("store_s1", 12.0, 1, "brick"), ("store_s2", 10.0, 2, "brick"), ("store_s3", 9.5, 2, "plaster"), ("store_s4", 13.5, 1, "siding"), ("store_s5", 10.5, 2, "brick"), ("store_s6", 12.0, 2, "brick")]
    cursor = oak_s + 14.0
    for index, (name, width, floors, wall) in enumerate(shops_s):
        gap = 4.0 if index == 2 else 0.25
        cursor += gap
        add(name, "shop", "hwy", cursor + width / 2, -1, front, width, 12.0 + rng.uniform(-1.0, 3.0), 0.0, floors=floors, wall=wall, seed=200 + index)
        cursor += width
    add("diner", "diner", "hwy", oak_s - 38.0, -1, 17.0, 20.0, 9.0, 0.0)
    add("garage", "garage", "hwy", oak_s - 42.0, +1, 16.0, 18.0, 11.0, 0.0)
    add("office", "shop", "hwy", mill_s + 24.0, -1, 10.5, 15.0, 12.0, 0.0, floors=1, wall="brick", seed=300, civic=True)
    add("hall", "shop", "hwy", mill_s + 22.0, +1, 10.5, 14.0, 13.0, 0.0, floors=2, wall="brick", seed=301)
    # --- landmarks
    add("gas", "gas", "hwy", hwy.nearest_s(-24000.0, 300.0)[0] / M, -1, 9.5, 36.0, 30.0, 0.0)
    add("motel", "motel", "hwy", hwy.nearest_s(23500.0, 3500.0)[0] / M, +1, 13.0, 52.0, 36.0, 0.0)
    add("church", "church", "oak", roads["oak"].nearest_s(-8000.0, 19500.0)[0] / M, +1, 15.0, 13.0, 24.0, 0.0)
    add("cabin", "cabin", "pine", roads["pine"].length / M - 4.0, -1, 9.0, 9.0, 7.0, -18.0)
    add("barn", "barn", "farm", roads["farm"].length / M - 12.0, -1, 16.0, 12.0, 18.0, 8.0)
    add("farmhouse", "house", "farm", roads["farm"].length / M - 46.0, +1, 13.0, 11.0, 9.0, -6.0, floors=2, seed=77, porch=True, garage=False)
    add("tower", "watertower", "oak", roads["oak"].nearest_s(-7000.0, 14000.0)[0] / M + 16.0, +1, 70.0, 9.0, 9.0, 0.0)
    # --- houses: irregular setbacks and small rotations along the residential streets
    def houses(prefix, road, entries):
        for index, (s_m, side, setback, floors, garage) in enumerate(entries):
            width, depth = rng.choice([(10.0, 8.0), (11.5, 8.5), (9.0, 9.5), (12.5, 8.0), (10.5, 9.0)])
            add("%s%d" % (prefix, index + 1), "house", road, s_m, side, setback, width, depth, rng.uniform(-7.0, 7.0), floors=floors, garage=garage, porch=rng.random() < 0.75, seed=rng.randrange(10000))

    oak_main = roads["oak"].nearest_s(-6000.0, 0.0)[0] / M
    houses("oak_s", "oak", [(oak_main - 62, +1, 13.0, 1, True), (oak_main - 64, -1, 14.5, 2, False), (oak_main - 118, -1, 12.0, 1, True), (oak_main - 150, +1, 13.5, 2, True), (oak_main - 196, -1, 15.0, 1, False), (oak_main - 212, +1, 12.5, 1, True)])
    houses("oak_n", "oak", [(oak_main + 50, -1, 13.0, 2, True), (oak_main + 56, +1, 14.0, 1, False), (oak_main + 92, -1, 12.5, 1, True), (oak_main + 104, +1, 15.5, 2, True), (oak_main + 176, -1, 14.0, 2, False), (oak_main + 236, -1, 16.0, 1, True), (oak_main + 250, +1, 14.0, 1, False)])
    houses("elm", "elm", [(34, +1, 13.5, 1, True), (44, -1, 12.5, 2, False), (82, +1, 14.5, 2, True), (104, -1, 13.0, 1, True), (132, +1, 13.0, 1, False), (176, +1, 14.0, 2, True), (236, -1, 13.0, 1, True)])
    houses("birch", "birch", [(34, +1, 12.5, 1, True), (46, -1, 13.5, 2, False), (84, +1, 14.0, 2, True), (104, -1, 12.5, 1, True), (140, +1, 13.0, 1, False), (156, -1, 14.5, 1, True)])
    houses("mill", "mill", [(62, +1, 13.0, 1, True), (76, -1, 14.0, 2, True), (128, +1, 13.5, 1, False), (150, -1, 12.5, 1, True)])
    return lots


def terrain_spec(roads, lots, paved=()):
    """Text input of `spn_worldtool terrain` (see Tools/SpnWorldTool/terrain.cpp)."""
    lines = ["grid %d %g %g" % (GRID_SAMPLES, GRID_QUAD, Z_SCALE), "seed %d" % SEED, "town %g %g %g" % (TOWN_CENTRE[0], TOWN_CENTRE[1], TOWN_RADIUS)]
    hwy = roads["hwy"]
    main0, main1 = main_street(roads)
    main_pts = [p for p, s in zip(hwy.pts, hwy.s) if main0 - 1200.0 <= s <= main1 + 1200.0]
    lines.append("road mainpad %g %g %g 0 %d %s" % (900.0, 100.0, 700.0, len(main_pts), " ".join("%.1f %.1f" % p for p in main_pts)))
    for rd in roads.values():
        lines.append("road %s %g %g %g %d %d %s" % (rd.name, rd.half, rd.shoulder, rd.blend, 1 if rd.ditch else 0, len(rd.pts), " ".join("%.1f %.1f" % p for p in rd.pts)))
    for lot in lots:
        margin = 250.0 if lot.kind in ("shop",) else 450.0
        lines.append("pad %s %.1f %.1f %.1f %.1f %.2f %g %.1f %.1f %g" % (lot.name, lot.x, lot.y, lot.width / 2 + margin, lot.depth / 2 + margin, lot.yaw, 1500.0 if lot.kind == "cabin" else 700.0, lot.anchor[0], lot.anchor[1], 14.0))
    for rect in paved:      # drives, paths and parking areas: (x, y, half x, half y, yaw)
        lines.append("paved %.1f %.1f %.1f %.1f %.2f" % tuple(rect))
    creek = catmull([(x * M, y * M) for x, y in CREEK], 500.0)
    lines.append("creek creek %g %g %d %s" % (170.0, 150.0, len(creek), " ".join("%.1f %.1f" % p for p in creek)))
    lines.append("pond %s %g %g %g %g" % (POND["name"], POND["x"], POND["y"], POND["radius"], POND["depth"]))
    for cx, cy, radius in CLEARINGS:
        lines.append("clear %g %g %g" % (cx, cy, radius))
    return "\n".join(lines) + "\n", creek


CREEK_VIEW = (38500.0, 4500.0, 2800.0, 150.0, 230.0)    # camera arc of the creek shot (centre x, y, radius, from/to degrees): kept free of trees
POND_VIEW = (-25200.0, 48800.0, 3300.0, 200.0, 290.0)    # camera arc of the pond shot (POND centre), same rule
CAMERA_ARCS = (CREEK_VIEW, POND_VIEW)

SECTION_TRACK = 6500.0      # reference forest section: cm to each side of the pine track,
SECTION_CABIN = 8000.0      # cm around the cabin,
SECTION_POND = 6500.0       # cm around the pond centre


def reference_section(roads, lots):
    """Predicate (x, y) -> True inside the reference forest section (pine track, cabin clearing, pond shore), the part of the
    placeholder world finished with scanned assets first. Pure geometry; the same in every build."""
    pine = roads["pine"]
    cabin = next(lot for lot in lots if lot.kind == "cabin")
    xs = [p[0] for p in pine.pts] + [cabin.x, POND["x"]]
    ys = [p[1] for p in pine.pts] + [cabin.y, POND["y"]]
    reach = max(SECTION_TRACK, SECTION_CABIN, SECTION_POND)
    box = (min(xs) - reach, max(xs) + reach, min(ys) - reach, max(ys) + reach)

    def inside(x, y):
        if not (box[0] <= x <= box[1] and box[2] <= y <= box[3]):
            return False
        return (math.hypot(x - cabin.x, y - cabin.y) < SECTION_CABIN or math.hypot(x - POND["x"], y - POND["y"]) < SECTION_POND
                or pine.nearest_s(x, y)[1] < SECTION_TRACK)
    inside.box = box
    return inside


def _hash01(i, j, seed):
    h = (i * 374761393 + j * 668265263 + seed * 1442695041) & 0xFFFFFFFF
    h = ((h ^ (h >> 13)) * 1274126177) & 0xFFFFFFFF
    return ((h ^ (h >> 16)) & 0xFFFF) / 65535.0


def noise(x, y, seed=0):
    """Smooth value noise in 0..1 with a feature size of 1 (deterministic; used to break up even distributions)."""
    i, j = int(math.floor(x)), int(math.floor(y))
    fx, fy = x - i, y - j
    fx, fy = fx * fx * (3 - 2 * fx), fy * fy * (3 - 2 * fy)
    a = _hash01(i, j, seed) + (_hash01(i + 1, j, seed) - _hash01(i, j, seed)) * fx
    b = _hash01(i, j + 1, seed) + (_hash01(i + 1, j + 1, seed) - _hash01(i, j + 1, seed)) * fx
    return a + (b - a) * fy


class Terrain:
    """Reads the output of the terrain tool: heights for placement, forest and road-distance masks."""

    def __init__(self, folder):
        import array
        import os
        self.n = GRID_SAMPLES
        self.h = array.array("H")
        with open(os.path.join(folder, "height.r16"), "rb") as f:
            self.h.frombytes(f.read())
        self.forest = open(os.path.join(folder, "forest.r8"), "rb").read()
        self.roaddist = open(os.path.join(folder, "roaddist.r8"), "rb").read()
        self.layers = {name: open(os.path.join(folder, "layer_%s.r8" % name), "rb").read() for name in ("Grass", "Forest", "Dirt", "Gravel", "Rock")}
        self.heights = {}
        for line in open(os.path.join(folder, "heights.txt")):
            parts = line.split()
            if parts[0] in ("road", "creek"):
                self.heights[(parts[0], parts[1])] = [float(v) for v in parts[3:]]
            elif parts[0] in ("pad", "pond"):
                self.heights[(parts[0], parts[1])] = float(parts[2])

    def _grid(self, x, y):
        fx = min(max((x + EXTENT) / GRID_QUAD, 0.0), self.n - 1.001)
        fy = min(max((y + EXTENT) / GRID_QUAD, 0.0), self.n - 1.001)
        return fx, fy

    def z(self, x, y):
        fx, fy = self._grid(x, y)
        ix, iy = int(fx), int(fy)
        tx, ty = fx - ix, fy - iy
        k = iy * self.n + ix
        h = self.h
        a = h[k] + (h[k + 1] - h[k]) * tx
        b = h[k + self.n] + (h[k + self.n + 1] - h[k + self.n]) * tx
        return ((a + (b - a) * ty) - 32768.0) * Z_SCALE / 128.0

    def _byte(self, data, x, y):
        fx, fy = self._grid(x, y)
        return data[int(fy + 0.5) * self.n + int(fx + 0.5)]

    def forest_at(self, x, y):
        return self._byte(self.forest, x, y) / 255.0

    def road_edge(self, x, y):
        """Distance beyond the edge of the nearest road in cm (0 on a road, 6375 = far away)."""
        return self._byte(self.roaddist, x, y) * 25.0

    def layer(self, name, x, y):
        return self._byte(self.layers[name], x, y) / 255.0

    def slope(self, x, y, d=200.0):
        return math.hypot(self.z(x + d, y) - self.z(x - d, y), self.z(x, y + d) - self.z(x, y - d)) / (2 * d)
