"""Test map v2 (/Game/SpnTest/Lvl_SpnMovementTestV2): the v1 movement test map PLUS a small connected district
(overnight work order M2-A.2, packages 5-11). Pure Python, no Unreal import, deterministic.

v1 (spn_movement_map_layout.build_layout) is NOT changed: the Windows automation tests and the layout consistency tests still
describe /Game/SpnTest/Lvl_SpnMovementTest. v2 reuses every v1 primitive unchanged (same labels, same positions) and adds
new primitives with the prefix SPN_D_ in the free parts of the 300 x 300 m square, so every v1 test station stays testable.

Contents: a ring road with two residential lanes (one with a real curve), a connector to the test zones, kerbs/sidewalks (12 cm,
a natural step-up test), parking lots with bay markings, four building types (cottage, two-storey house, storage hall, workshop)
plus a garage from spn_building_kit, garden walls of 30/45/100 cm, fences, a small park, trees, bushes, boulders and street lamps.

NOT a geographic excerpt of the later open world; implies no decision about roads, settlements or buildings of the real game.
"""

import math

import spn_building_kit as kit
import spn_building_templates as tpl
import spn_movement_map_layout as v1

MAP_V2 = "/Game/SpnTest/Lvl_SpnMovementTestV2"
GENERATOR_VERSION = "m2a2-w2"  # bump when the layout or ownership rules change; written into the SPN_GEN_VER tag of every actor
HALF = v1.HALF
CELL = 200.0
ROAD_W = 800.0
SIDEWALK_W = 200.0
ROAD_TOP = 1.0       # cm
SIDEWALK_TOP = 12.0  # cm: a kerb the character walks up (step height 25 cm)
LOT_TOP = 1.0

# Straight road centre lines (multiples of 200 so road and sidewalk edges fall on the 200 cm grid): (x0, y0, x1, y1).
ROADS = (
    ("West Avenue", -10200.0, -14000.0, -10200.0, 14200.0),
    ("South Street", -10200.0, -14000.0, 14200.0, -14000.0),
    ("East Avenue", 14200.0, -14000.0, 14200.0, 14200.0),
    ("North Street", -10200.0, 14200.0, 14200.0, 14200.0),
    ("Birkenweg", -10200.0, -7000.0, -6200.0, -7000.0),
    ("Storage Road", -6200.0, -7000.0, 6000.0, -7000.0),      # Birkenweg continues east past the test zones
    ("Lagerstrasse", 6000.0, -14000.0, 6000.0, -7000.0),
    ("Lindenweg", -10200.0, 11000.0, -7000.0, 11000.0),
    ("Hofweg", 4600.0, 9600.0, 4600.0, 14200.0),
    ("Kurvenweg lead-in", -10200.0, -10800.0, -8400.0, -10800.0),   # straight part of the curved lane; the arc starts at (-8000, -10800)
    ("Kurvenweg lead-out", -6400.0, -14000.0, -6400.0, -12800.0),   # the arc ends at (-6400, -12400)
)
# Curved lane (centre (-8000, -12400), radius 1600): leaves West Avenue eastwards at y = -10800, bends south, ends at South Street.
CURVE = {"name": "Kurvenweg", "centre": (-8000.0, -12400.0), "radius": 1600.0, "segments": 8, "from_x": -10200.0, "to_y": -14000.0}
TURNAROUNDS = (("Lindenweg end", -7000.0, 11000.0), ("Hofweg end", 4600.0, 9600.0))
TURN_RADIUS = 600.0

LOTS = (  # (name, x0, x1, y0, y1)
    ("Workshop lot", 8600.0, 11000.0, -13200.0, -10600.0),
    ("East lot", 11800.0, 13600.0, 8200.0, 12800.0),
)

ZONE_ROADS = "d_roads"


def _rect_cells(x0, x1, y0, y1):
    ix0, ix1 = int(round(x0 / CELL)), int(round(x1 / CELL))
    iy0, iy1 = int(round(y0 / CELL)), int(round(y1 / CELL))
    return {(i, j) for i in range(ix0, ix1) for j in range(iy0, iy1)}


def _merge_cells(cells):
    """Greedy rectangle cover of a cell set (row runs, then merged upwards). Deterministic. Returns (x0, x1, y0, y1) in cm."""
    rows = {}
    for i, j in cells:
        rows.setdefault(j, []).append(i)
    runs = {}  # (i0, i1) -> list of j
    out = []
    open_runs = {}
    for j in sorted(rows):
        xs = sorted(rows[j])
        current = []
        start = prev = xs[0]
        for i in xs[1:]:
            if i != prev + 1:
                current.append((start, prev + 1))
                start = i
            prev = i
        current.append((start, prev + 1))
        new_open = {}
        for run in current:
            if run in open_runs and open_runs[run][1] == j:
                new_open[run] = (open_runs[run][0], j + 1)
            else:
                new_open[run] = (j, j + 1)
        for run, (j0, j1) in open_runs.items():
            if run not in new_open:
                out.append((run, j0, j1))
        open_runs = new_open
    for run, (j0, j1) in open_runs.items():
        out.append((run, j0, j1))
    return sorted((r[0][0] * CELL, r[0][1] * CELL, r[1] * CELL, r[2] * CELL) for r in out)


def road_rects():
    rects = []
    for _, ax, ay, bx, by in ROADS:
        half = ROAD_W / 2.0
        rects.append((min(ax, bx) - half, max(ax, bx) + half, min(ay, by) - half, max(ay, by) + half))
    return rects


def _curve_points(curve):
    cx, cy = curve["centre"]
    r = curve["radius"]
    n = curve["segments"]
    pts = []
    for k in range(n + 1):
        a = math.radians(90.0 - 90.0 * k / n)  # from the top of the circle (heading east) to its right side (heading south)
        pts.append((cx + r * math.cos(a), cy + r * math.sin(a)))
    return pts


def ground_cells():
    """(road_cells, sidewalk_cells) on the 200 cm grid: straight roads plus turnaround discs, and the sidewalk ring around them."""
    road_cells = set()
    for r in road_rects():
        road_cells |= _rect_cells(*r)
    inflated = set()
    for x0, x1, y0, y1 in road_rects():
        inflated |= _rect_cells(x0 - SIDEWALK_W, x1 + SIDEWALK_W, y0 - SIDEWALK_W, y1 + SIDEWALK_W)
    for _, tx, ty in TURNAROUNDS:
        disc = {(i, j) for i in range(int((tx - TURN_RADIUS) / CELL) - 1, int((tx + TURN_RADIUS) / CELL) + 2)
                for j in range(int((ty - TURN_RADIUS) / CELL) - 1, int((ty + TURN_RADIUS) / CELL) + 2)
                if math.hypot((i + 0.5) * CELL - tx, (j + 0.5) * CELL - ty) <= TURN_RADIUS}
        road_cells |= disc
        inflated |= disc
    return road_cells, inflated - road_cells


def build_roads(layout):
    road_cells, sidewalk_cells = ground_cells()
    for n, (x0, x1, y0, y1) in enumerate(_merge_cells(road_cells)):
        layout.add("SPN_D_Road_%02d" % n, "cube", ((x0 + x1) / 2.0, (y0 + y1) / 2.0, ROAD_TOP / 2.0), (x1 - x0, y1 - y0, ROAD_TOP), ZONE_ROADS, material="grid_topdark")
    for n, (x0, x1, y0, y1) in enumerate(_merge_cells(sidewalk_cells)):
        layout.add("SPN_D_Sidewalk_%02d" % n, "cube", ((x0 + x1) / 2.0, (y0 + y1) / 2.0, SIDEWALK_TOP / 2.0), (x1 - x0, y1 - y0, SIDEWALK_TOP), ZONE_ROADS, material="grid_gray")
    return road_cells


def build_curve(layout):
    """Road surface of the curved lane as yaw-rotated chord boxes plus a kerb strip on both sides (rotation: positive yaw turns X towards Y)."""
    pts = _curve_points(CURVE)
    cx, cy = CURVE["centre"]
    for k in range(len(pts) - 1):
        (ax, ay), (bx, by) = pts[k], pts[k + 1]
        length = math.hypot(bx - ax, by - ay)
        yaw = math.degrees(math.atan2(by - ay, bx - ax))
        mx, my = (ax + bx) / 2.0, (ay + by) / 2.0
        # Overlap the neighbours a little (they are the same material and height) so no wedge gap opens on the outer side.
        layout.add("SPN_D_Curve_%d" % k, "cube", (mx, my, ROAD_TOP / 2.0), (length + 40.0, ROAD_W, ROAD_TOP), ZONE_ROADS, rot=(0.0, yaw, 0.0), material="grid_topdark")
        nx, ny = (mid_normal(ax, ay, bx, by))
        for tag, sign in (("In", -1.0), ("Out", 1.0)):
            off = ROAD_W / 2.0 + SIDEWALK_W / 2.0
            layout.add("SPN_D_CurveKerb%s_%d" % (tag, k), "cube", (mx + sign * nx * off, my + sign * ny * off, SIDEWALK_TOP / 2.0),
                       (length + 2.0, SIDEWALK_W, SIDEWALK_TOP), ZONE_ROADS, rot=(0.0, yaw, 0.0), material="grid_gray")


def mid_normal(ax, ay, bx, by):
    """Unit normal pointing away from the circle centre (outwards)."""
    cx, cy = CURVE["centre"]
    mx, my = (ax + bx) / 2.0, (ay + by) / 2.0
    d = math.hypot(mx - cx, my - cy)
    return (mx - cx) / d, (my - cy) / d


def build_lots(layout):
    for name, x0, x1, y0, y1 in LOTS:
        tag = name.replace(" ", "")
        layout.add("SPN_D_Lot_%s" % tag, "cube", ((x0 + x1) / 2.0, (y0 + y1) / 2.0, LOT_TOP / 2.0), (x1 - x0, y1 - y0, LOT_TOP), "d_lots", material="grid_topdark")
        # Bay lines every 250 cm along the long side, 5 m deep, painted 0.5 cm above the surface.
        long_x = (x1 - x0) >= (y1 - y0)
        if long_x:
            for k in range(int((x1 - x0) // 250.0) + 1):
                x = x0 + k * 250.0
                if x1 - x < 10.0:
                    break
                layout.add("SPN_D_Bay_%s_%02d" % (tag, k), "cube", (x, y0 + 250.0, LOT_TOP + 0.5), (12.0, 500.0, 1.0), "d_lots", material="grid_gray")
        else:
            for k in range(int((y1 - y0) // 250.0) + 1):
                y = y0 + k * 250.0
                if y1 - y < 10.0:
                    break
                layout.add("SPN_D_Bay_%s_%02d" % (tag, k), "cube", (x0 + 250.0, y, LOT_TOP + 0.5), (500.0, 12.0, 1.0), "d_lots", material="grid_gray")


# --- buildings ------------------------------------------------------------------------------------------------------------------

# (prefix, template, Params, x0, y0): every building is a template instance, not a hand-built model.
P = tpl.Params
BUILDINGS = (
    ("Cottage1", "cottage", P(900.0, 700.0, 1, "W", "gable", 300.0, 160.0, 350.0, "d_cottage1_door", 0.5), -9400.0, -5800.0),
    ("House1", "house", P(1100.0, 1100.0, 2, "W", "gable", 300.0, 160.0, 350.0, "d_house1_door", 0.5), -9400.0, 8000.0),
    ("Cottage2", "cottage", P(800.0, 650.0, 1, "W", "gable", 300.0, 200.0, 320.0, "d_cottage2_door", 0.45), -9400.0, 12000.0),
    ("StorageHall", "warehouse", P(1800.0, 1100.0, 1, "W", "flat", 520.0, 160.0, 450.0, "d_storage_door", 0.5), -9300.0, -13000.0),
    ("Workshop", "workshop", P(1500.0, 900.0, 1, "W", "flat", 300.0, 160.0, 350.0, "d_workshop_door", 0.5), 6700.0, -12400.0),
    ("Garage", "garage", P(600.0, 550.0, 1, "W", "flat", 300.0, 160.0, 300.0, "", 0.5), 11300.0, -12800.0),
    ("CourtHouse", "house", P(1100.0, 1100.0, 2, "W", "flat", 320.0, 160.0, 380.0, "d_courthouse_door", 0.55), 5300.0, 10800.0),
    ("Office1", "office", P(1500.0, 1000.0, 1, "E", "flat", 300.0, 160.0, 380.0, "d_office_door", 0.5, 3), 11900.0, 3000.0),
    ("Cottage3", "cottage", P(900.0, 700.0, 1, "E", "gable", 300.0, 180.0, 350.0, "d_cottage3_door", 0.5), 2400.0, 10800.0),
)


def build_buildings(layout):
    """Returns {prefix: {"spec", "origin", "footprint", "entrance"}}."""
    info = {}
    for prefix, template, params, x0, y0 in BUILDINGS:
        spec = tpl.generate(tpl.TEMPLATES[template], params)
        zone = "d_" + prefix.lower()
        fp = kit.build_building(layout, spec, x0, y0, "SPN_D_" + prefix, zone)
        entrance = next((o for o in spec.openings if o.door_id), None) or next((o for o in spec.openings if o.side in ("W", "E") and o.width >= 300.0), None)
        info[prefix] = {"spec": spec, "origin": (x0, y0), "footprint": fp, "entrance": entrance, "template": template}
        if entrance is not None and prefix != "Garage":
            _entrance_path(layout, prefix, spec, x0, y0, entrance, fp)
    return info


def _entrance_path(layout, prefix, spec, x0, y0, entrance, fp):
    """A 140 cm wide, 2 cm high path from the doorway to the nearest road sidewalk / lot (straight along X)."""
    y = y0 + entrance.centre
    sx = spec.size[0]
    if entrance.side == "W":
        edge = x0 - spec.wall_thickness / 2.0
        target = _nearest_hard_surface_x(edge, y, -1.0)
    else:
        edge = x0 + sx + spec.wall_thickness / 2.0
        target = _nearest_hard_surface_x(edge, y, 1.0)
    a, b = sorted((edge, target))
    if b - a < 5.0:
        return
    layout.add("SPN_D_Path_%s" % prefix, "cube", ((a + b) / 2.0, y, 1.0), (b - a, 140.0, 2.0), "d_paths", material="grid_gray")


def _hard_cells():
    cells = set()
    for r in road_rects():
        cells |= _rect_cells(r[0] - SIDEWALK_W, r[1] + SIDEWALK_W, r[2] - SIDEWALK_W, r[3] + SIDEWALK_W)
    for _, tx, ty in TURNAROUNDS:
        cells |= _rect_cells(tx - TURN_RADIUS - SIDEWALK_W, tx + TURN_RADIUS + SIDEWALK_W, ty - TURN_RADIUS - SIDEWALK_W, ty + TURN_RADIUS + SIDEWALK_W)
    for _, x0, x1, y0, y1 in LOTS:
        cells |= _rect_cells(x0, x1, y0, y1)
    return cells


_HARD = None


def _nearest_hard_surface_x(x, y, direction):
    """First x (searching in `direction` from x) where the 200 cm grid cell at y belongs to a road, sidewalk or lot; fallback: 300 cm."""
    global _HARD
    if _HARD is None:
        _HARD = _hard_cells()
    j = int(math.floor(y / CELL))
    xx = x
    for _ in range(100):
        xx += direction * 20.0
        i = int(math.floor(xx / CELL))
        if (i, j) in _HARD:
            return (i + 1) * CELL if direction < 0 else i * CELL  # the cell edge nearest to the building, so the path never overlaps a kerb
    return x + direction * 300.0


# --- walls, fences, lamps, test obstacles ----------------------------------------------------------------------------------------

def build_garden_walls(layout, info):
    """Garden/yard walls of defined heights (obstacle stations in a believable setting): 30/45/100 cm low walls and fences with gates."""
    # Cottage1 yard (south of Birkenweg, west of the pillar field): three low walls of different height with 3 m gaps.
    for k, h in enumerate((30.0, 45.0, 100.0)):
        layout.add("SPN_D_GardenWall_%03d" % h, "cube", (-8000.0, -5200.0 + k * 700.0, h / 2.0), (500.0, 25.0, h), "d_yards", material="grid_gray_02")
    # Hedge-height fence around the storage yard with a gate gap on the west side.
    _fence(layout, "StorageYard", -9500.0, -7300.0, -13300.0, -11800.0, gaps=(("W", -12850.0, -12550.0),))
    door_y = info["Cottage3"]["origin"][1] + info["Cottage3"]["entrance"].centre
    _fence(layout, "CourtYard", 2200.0, 3900.0, 10400.0, 12600.0, gaps=(("E", door_y - 100.0, door_y + 100.0),))


def _fence(layout, name, x0, x1, y0, y1, gaps=(), height=120.0, thickness=8.0):
    """Four fence boards as boxes with optional gaps (side, a0, a1) along the side's running axis."""
    sides = {"W": ("x", x0, y0, y1), "E": ("x", x1, y0, y1), "S": ("y", y0, x0, x1), "N": ("y", y1, x0, x1)}
    for side, (axis, fixed, a0, a1) in sides.items():
        # W/E boards run over the corners, S/N boards stop at their faces: no overlapping boxes.
        a0, a1 = (a0 - thickness / 2.0, a1 + thickness / 2.0) if side in ("W", "E") else (a0 + thickness / 2.0, a1 - thickness / 2.0)
        cuts = sorted((g0, g1, 0.0, height) for s, g0, g1 in gaps if s == side)
        layout.wall("SPN_D_Fence_%s_%s" % (name, side), "d_yards", axis, fixed, a0, a1, 0.0, height, thickness, openings=cuts, material="grid_topdark")


def build_lamps(layout, count_per_road=3):
    """Street lamps (pole + head) along the ring road at fixed spacing, on the sidewalk outer edge, each with a light entry."""
    lights = getattr(layout, "lights", None)
    if lights is None:
        layout.lights = lights = []
    spots = []
    for x in (-6000.0, 0.0, 6000.0, 12000.0):
        spots.append((x, -14000.0 - ROAD_W / 2.0 - SIDEWALK_W / 2.0))
    for y in (-8000.0, -2000.0, 4000.0, 10000.0):
        spots.append((14200.0 + ROAD_W / 2.0 + SIDEWALK_W / 2.0, y))
    for x in (-6000.0, 0.0, 6000.0, 12000.0):
        spots.append((x, 14200.0 + ROAD_W / 2.0 + SIDEWALK_W / 2.0))
    for y in (-10000.0, -4000.0, 2000.0, 8000.0):
        spots.append((-10200.0 - ROAD_W / 2.0 - SIDEWALK_W / 2.0, y))
    for i, (x, y) in enumerate(spots):
        layout.add("SPN_D_Lamp_%02d_Pole" % i, "cylinder", (x, y, SIDEWALK_TOP + 225.0), (16.0, 16.0, 450.0), "d_props", material="grid_topdark")
        layout.add("SPN_D_Lamp_%02d_Head" % i, "sphere", (x, y, SIDEWALK_TOP + 465.0), (40.0, 40.0, 40.0), "d_props", material="grid_gray")
        lights.append({"pos": [x, y, SIDEWALK_TOP + 440.0], "intensity": 4000.0, "radius": 900.0, "zone": "d_props"})


# --- vegetation (deterministic, placed on free ground only) ------------------------------------------------------------------

def _prim_footprint(p):
    """Conservative axis-aligned (x0, x1, y0, y1) of a primitive on the ground plane; None for prims that are the ground itself."""
    if p["label"] == "SPN_MV_Floor":
        return None
    cx, cy, _ = p["center"]
    sx, sy, sz = p["size"]
    pitch, yaw, roll = p["rot"]
    if p["shape"] == "sphere":
        # Half-buried dome: the visible footprint radius from the cap height above the ground (centre z is cap - R).
        r = sx / 2.0
        cap = p["center"][2] + r
        rr = math.sqrt(max(r * r - (r - cap) ** 2, 0.0)) if cap < r else r
        return (cx - rr, cx + rr, cy - rr, cy + rr)
    if p["shape"] == "cylinder":
        r = max(sx, sy) / 2.0
        return (cx - r, cx + r, cy - r, cy + r)
    ex = abs(sx * math.cos(math.radians(pitch))) + abs(sz * math.sin(math.radians(pitch)))
    ey = sy + abs(sz * math.sin(math.radians(roll)))
    c, s = abs(math.cos(math.radians(yaw))), abs(math.sin(math.radians(yaw)))
    hx = (ex * c + ey * s) / 2.0
    hy = (ex * s + ey * c) / 2.0
    return (cx - hx, cx + hx, cy - hy, cy + hy)


class _FootprintIndex:
    """Footprints in a uniform grid (1000 cm buckets): the same yes/no answer as scanning every footprint, without the full scan."""

    BUCKET = 1000.0

    def __init__(self, footprints=()):
        self.cells = {}
        for fp in footprints:
            self.add(fp)

    def _range(self, lo, hi):
        return range(int(math.floor(lo / self.BUCKET)), int(math.floor(hi / self.BUCKET)) + 1)

    def add(self, fp):
        for i in self._range(fp[0], fp[1]):
            for j in self._range(fp[2], fp[3]):
                self.cells.setdefault((i, j), []).append(fp)

    append = add

    def free(self, x, y, radius):
        for i in self._range(x - radius, x + radius):
            for j in self._range(y - radius, y + radius):
                for x0, x1, y0, y1 in self.cells.get((i, j), ()):
                    if x + radius > x0 and x - radius < x1 and y + radius > y0 and y - radius < y1:
                        return False
        return True


def build_vegetation(layout, existing_prims):
    """Trees, bushes and boulders in green areas, rejection-sampled against every existing footprint; a park patch with a path and benches."""
    footprints = _FootprintIndex(fp for fp in (_prim_footprint(p) for p in existing_prims + layout.prims) if fp)
    # Green patch (park) - flat, 2 cm, colour-way material.
    park = (2300.0, 6300.0, 4300.0, 8600.0)
    layout.add("SPN_D_Park_Lawn", "cube", ((park[0] + park[1]) / 2.0, (park[2] + park[3]) / 2.0, 1.0), (park[1] - park[0], park[3] - park[2], 2.0), "d_park", material="colorway")
    layout.add("SPN_D_Park_Path", "cube", (4300.0, (park[2] + park[3]) / 2.0, 4.0), (160.0, park[3] - park[2], 4.0), "d_park", material="grid_gray")
    footprints.append((4300.0 - 100.0, 4300.0 + 100.0, park[2], park[3]))
    for k, y in enumerate((5200.0, 6600.0, 7900.0)):
        layout.add("SPN_D_Park_Bench_%d" % k, "cube", (4300.0 + 140.0, y, 22.0), (45.0, 160.0, 45.0), "d_park", material="grid_gray_02")
        footprints.append((4300.0 + 100.0, 4300.0 + 180.0, y - 100.0, y + 100.0))
    rng = v1._Lcg(20261027)
    areas = (  # (name, x0, x1, y0, y1, trees, bushes, rocks)
        ("park", 2400.0, 6200.0, 4400.0, 8400.0, 14, 6, 0),
        ("northwest", -9300.0, -6200.0, 7700.0, 13300.0, 16, 10, 2),
        ("southwest", -9300.0, -5800.0, -6200.0, -3200.0, 6, 8, 3),
        ("southeast", 6600.0, 13300.0, -9600.0, -8400.0, 8, 2, 5),
        ("east", 11800.0, 13400.0, 0.0, 7600.0, 10, 6, 4),
        ("north", 700.0, 4000.0, 11200.0, 13200.0, 8, 4, 0),
    )
    count = 0
    for name, x0, x1, y0, y1, trees, bushes, rocks in areas:
        for kind, n, radius in (("Tree", trees, 220.0), ("Bush", bushes, 90.0), ("Boulder", rocks, 120.0)):
            placed = 0
            for _ in range(n * 40):
                if placed >= n:
                    break
                x, y = rng.uniform(x0, x1), rng.uniform(y0, y1)
                x, y = round(x, 1), round(y, 1)
                if not footprints.free(x, y, radius + 60.0):
                    continue
                placed += 1
                count += 1
                footprints.append((x - radius, x + radius, y - radius, y + radius))
                zone = "d_veg_" + name
                if kind == "Tree":
                    h = rng.uniform(280.0, 380.0)
                    crown = rng.uniform(260.0, 340.0)
                    layout.add("SPN_D_Tree_%03d_Trunk" % count, "cylinder", (x, y, h / 2.0), (36.0, 36.0, h), zone, material="grid_topdark")
                    layout.add("SPN_D_Tree_%03d_Crown" % count, "sphere", (x, y, h + crown * 0.25), (crown, crown, crown), zone, material="colorway")
                elif kind == "Bush":
                    d = rng.uniform(70.0, 120.0)
                    layout.add("SPN_D_Bush_%03d" % count, "sphere", (x, y, d * 0.1), (d, d, d), zone, material="colorway")
                else:
                    d = rng.uniform(80.0, 160.0)
                    layout.add("SPN_D_Boulder_%03d" % count, "sphere", (x, y, d * 0.1), (d, d * 0.8, d * 0.7), zone, material="grid_gray_02")


# --- gameplay routes ------------------------------------------------------------------------------------------------------------

# (name, from, to). Points: ("start",), ("xy", x, y), ("reset", name), ("lot", name), ("front", building) = the cell 150 cm in front of the
# entrance, ("interior", building, room@floor) = a room reached through the building's room graph (outside -> entrance -> doorways/stair).
ROUTES = (
    ("spawn_to_cottage1", ("start",), ("front", "Cottage1")),
    ("spawn_to_house1", ("start",), ("front", "House1")),
    ("spawn_to_office", ("start",), ("front", "Office1")),
    ("spawn_to_storage_hall", ("start",), ("front", "StorageHall")),
    ("spawn_to_workshop", ("start",), ("front", "Workshop")),
    ("spawn_to_court_house", ("start",), ("front", "CourtHouse")),
    ("cottage1_to_obstacles", ("front", "Cottage1"), ("reset", "obstacles")),
    ("obstacles_to_ramps", ("reset", "obstacles"), ("reset", "ramps")),
    ("ramps_to_stairs", ("reset", "ramps"), ("reset", "stairs")),
    ("street_to_workshop_lot", ("xy", -10200.0, -14000.0), ("lot", "Workshop lot")),
    ("street_to_east_lot", ("xy", 14200.0, 0.0), ("lot", "East lot")),
    ("house1_ground_to_upper_room", ("interior", "House1", "room1@0"), ("interior", "House1", "room1@1")),
    ("office_entrance_to_last_room", ("interior", "Office1", "outside"), ("interior", "Office1", "n1@0")),
)


def building_spec(name):
    for prefix, template, params, x0, y0 in BUILDINGS:
        if prefix == name:
            return tpl.generate(tpl.TEMPLATES[template], params), (x0, y0)
    raise KeyError(name)


def door_open_angle(door):
    """Hinge yaw change when the door actor opens (ASpnTestDoor::OpenAngle, +90 rotates the leaf from -Y towards +X). District doors swing
    OUTWARD: an open leaf must never stand in the entrance hall (the stair halls of the two-storey houses are only 140 cm wide; found in
    Unreal: the +90 swing of a west door blocked the hall and the stair foot). West entrance -> -90, east entrance -> +90. Doors that do not
    belong to a district building (the v1 hut) keep the actor default +90; a door that carries its own "open_angle" (the v2 copy of the v1
    house, see fix_v1_house_door) uses that."""
    if "open_angle" in door:
        return float(door["open_angle"])
    import spn_building_roles as roles
    for prefix, template, params, _, _ in BUILDINGS:
        if door["zone"] == "d_" + prefix.lower():
            spec, _ = building_spec(prefix)
            return -90.0 if roles.entrance(spec).side == "W" else 90.0
    return 90.0


def door_inward_sign(door):
    """+1 when the building lies at +X of the door's wall plane (west entrances and the v1 buildings), -1 for east entrances."""
    import spn_building_roles as roles
    if "inward" in door:   # doors that are not a building entrance (enrichment gates) say it themselves
        return float(door["inward"])
    for prefix, template, params, _, _ in BUILDINGS:
        if door["zone"] == "d_" + prefix.lower():
            spec, _ = building_spec(prefix)
            return 1.0 if roles.entrance(spec).side == "W" else -1.0
    return 1.0


def front_point(name):
    """World point 150 cm outside the entrance of a district building."""
    import spn_building_roles as roles
    spec, (x0, y0) = building_spec(name)
    ent = roles.entrance(spec)
    return (x0 + spec.size[0] + 150.0 if ent.side == "E" else x0 - 150.0, y0 + ent.centre)


def interior_path(name, target):
    """Node list from "outside" to `target` ("room@floor") through the building's room graph, or None when not connected."""
    import spn_building_roles as roles
    spec, _ = building_spec(name)
    graph = roles.room_graph(spec)
    adj = {}
    for a, b, _ in graph["edges"]:
        adj.setdefault(a, []).append(b)
        adj.setdefault(b, []).append(a)
    prev, queue = {"outside": None}, ["outside"]
    while queue:
        n = queue.pop(0)
        for m in sorted(adj.get(n, ())):
            if m not in prev:
                prev[m] = n
                queue.append(m)
    if target not in prev:
        return None
    path, n = [], target
    while n is not None:
        path.append(n)
        n = prev[n]
    return path[::-1]


def _point(data, spec):
    kind = spec[0]
    if kind == "start":
        return tuple(data["player_start"][:2])
    if kind == "xy":
        return (spec[1], spec[2])
    if kind == "reset":
        return tuple(next(r["pos"] for r in data["reset_points"] if r["name"] == spec[1])[:2])
    if kind == "lot":
        _, x0, x1, y0, y1 = next(l for l in LOTS if l[0] == spec[1])
        return ((x0 + x1) / 2.0, (y0 + y1) / 2.0)
    if kind == "front":
        return front_point(spec[1])
    raise KeyError(kind)


def check_routes(data, grid):
    """[(route name, ok, detail)] for every route in ROUTES. Outdoor routes use the 100 cm map grid; interior routes the room graph."""
    out = []
    for name, a, b in ROUTES:
        if a[0] == "interior" or b[0] == "interior":
            target = b[2]
            path = interior_path(b[1], target)
            via_stair = path is not None and any(n.endswith("@1") for n in path) and target.endswith("@1")
            ok = path is not None and (not target.endswith("@1") or via_stair)
            out.append((name, ok, " > ".join(path) if path else "no path"))
        else:
            pa, pb = _point(data, a), _point(data, b)
            out.append((name, grid.reachable(pa, pb), "%s -> %s" % (pa, pb)))
    return out


# --- assembly ---------------------------------------------------------------------------------------------------------------------

V1_HOUSE_STAIR_RUN = 25.0


def fix_v1_house_stairs(prims):
    """V1 house (zone "house"): the 17-step stair block ran along +Y up to y = -9204 right behind the entrance door (opening y -9255 .. -9145),
    leaving 59 cm beside it for an 84 cm capsule; found in the Unreal QA run (SpnQA.Doors, capsule sweep hit SPN_MV_House_Stair_17 behind
    the open door). V2 only: a 25 cm tread (V1 has 28) ends the block at y = -9255, flush with the south edge of the opening. The V1 layout
    data and its snapshot hash stay untouched."""
    y0, y1, steps = -9700.0, -8700.0, 17
    by_label = {p["label"]: p for p in prims}
    for i in range(1, steps + 1):
        stair = by_label["SPN_MV_House_Stair_%02d" % i]
        stair["center"][1] = round(y0 + 20.0 + (i - 0.5) * V1_HOUSE_STAIR_RUN, 3)
        stair["size"][1] = V1_HOUSE_STAIR_RUN
    hole_y1 = y0 + 20.0 + steps * V1_HOUSE_STAIR_RUN
    slab = by_label["SPN_MV_House_Slab_North"]
    slab["center"][1] = round((hole_y1 + y1) / 2.0, 3)
    slab["size"][1] = round(y1 - hole_y1, 3)
    rail = by_label["SPN_MV_House_StairRail"]
    rail["center"][1] = round((y0 + hole_y1) / 2.0, 3)
    rail["size"][1] = round(hole_y1 - y0, 3)


def fix_v1_house_door(doors):
    """V1 house entrance (V2 only): with the leaf swinging +90 into the hall it stands along the north jamb for 100 cm right next to the
    stair block, a 100 cm wide, 100 cm long pinch for an 84 cm capsule (spn_map_validate.door_pinch: 108 cm; every other door has 14 cm, the wall
    thickness). The leaf swings outward instead, like the district doors. The V1 door data and its snapshot hash stay untouched."""
    for door in doors:
        if door["id"] == "mv_house_door":
            door["open_angle"] = -90.0


def build_layout_v2():
    """v1 data + the district. All new primitives are in `d_*` zones; v1 primitives, doors, openings and reset points are kept as they are."""
    data = v1.build_layout()
    fix_v1_house_stairs(data["prims"])
    fix_v1_house_door(data["doors"])
    district = v1.Layout()
    district.lights = []
    build_roads(district)
    build_curve(district)
    build_lots(district)
    info = build_buildings(district)
    build_garden_walls(district, info)
    build_lamps(district)
    build_vegetation(district, data["prims"])
    data["map"] = MAP_V2
    data["prims"] = data["prims"] + district.prims
    data["doors"] = data["doors"] + district.doors
    data["openings"] = data["openings"] + district.openings
    data["lights"] = district.lights
    data["generator_version"] = GENERATOR_VERSION
    data["reset_points"] = data["reset_points"] + [
        {"name": "d_westavenue", "pos": [-10200.0, -2000.0, v1.PLAYER_START[2]]},
        {"name": "d_southstreet", "pos": [0.0, -14000.0, v1.PLAYER_START[2]]},
        {"name": "d_eastavenue", "pos": [14200.0, 0.0, v1.PLAYER_START[2]]},
        {"name": "d_hofweg", "pos": [4600.0, 12000.0, v1.PLAYER_START[2]]},
    ]
    data["guard"]["reset_points"] = [p["pos"] for p in data["reset_points"]]
    data["district"] = {
        "buildings": {k: {"origin": list(v["origin"]), "footprint": [round(c, 3) for c in v["footprint"]], "kind": v["spec"].kind,
                          "floors": v["spec"].floors, "template": v["template"]} for k, v in sorted(info.items())},
        "roads": [list(r) for r in ROADS],
        "turnarounds": [list(t) for t in TURNAROUNDS],
        "lots": [list(l) for l in LOTS],
    }
    return data


if __name__ == "__main__":
    import sys

    result = build_layout_v2()
    if len(sys.argv) > 1:
        with open(sys.argv[1], "w", encoding="utf-8") as handle:
            handle.write(v1.to_json(result) + "\n")
    print("%d primitives (%d new), %d doors, %d lights" % (len(result["prims"]), len(result["prims"]) - len(v1.build_layout()["prims"]), len(result["doors"]), len(result["lights"])))
