"""Validation of layout data (v1 or v2) before anything is spawned, and whole-map reachability on a 100 cm grid.

Pure Python, no Unreal import. `validate_layout_data` is the generator's pre-flight (nothing in the level is touched when it fails);
`MapGrid` answers "can a capsule walk from A to B" for the named gameplay routes. The grid is conservative: ramps, stairs, hills and
anything higher than the step height are treated as blocked, low things (kerbs, paths, lots, lawn, low rocks) as walkable.
"""

import math

import spn_movement_map_layout as L

CELL = 100.0
KNOWN_SHAPES = set(L.SHAPE_PATHS)
KNOWN_MATERIALS = set(L.MATERIAL_PATHS)


def _finite(values):
    return all(isinstance(v, (int, float)) and math.isfinite(v) for v in values)


def validate_layout_data(data):
    """Problems of a layout dict as short strings (empty = valid): finite numbers, positive sizes, unique labels, known shapes and
    materials, doors with unique ids, reset points and the player start inside the square, guard consistent."""
    problems = []
    labels = set()
    half = data.get("half_extent_cm", L.HALF)
    for p in data.get("prims", []):
        label = p.get("label", "?")
        if label in labels:
            problems.append("duplicate label " + label)
        labels.add(label)
        if p.get("shape") not in KNOWN_SHAPES:
            problems.append("%s: unknown shape %r" % (label, p.get("shape")))
        if p.get("material") not in KNOWN_MATERIALS:
            problems.append("%s: unknown material %r" % (label, p.get("material")))
        for key in ("center", "size", "rot"):
            v = p.get(key)
            if not v or len(v) != 3 or not _finite(v):
                problems.append("%s: %s is not three finite numbers" % (label, key))
        if p.get("size") and _finite(p["size"]) and min(p["size"]) <= 0:
            problems.append("%s: non-positive size" % label)
    ids = set()
    for d in data.get("doors", []):
        if d["id"] in ids:
            problems.append("duplicate door id " + d["id"])
        ids.add(d["id"])
        if not _finite(d["hinge"]) or not _finite([d["yaw"]]):
            problems.append("door %s: not finite" % d["id"])
    for r in data.get("reset_points", []):
        x, y, _ = r["pos"]
        if not (-half < x < half and -half < y < half):
            problems.append("reset point %s outside the square" % r["name"])
    x, y, _ = data["player_start"]
    if not (-half < x < half and -half < y < half):
        problems.append("player start outside the square")
    for light in data.get("lights", []):
        if not _finite(light["pos"]) or light["intensity"] <= 0 or light["radius"] <= 0:
            problems.append("bad light " + str(light["pos"]))
    guard = data.get("guard")
    if guard and len(guard["reset_points"]) != len(data.get("reset_points", [])):
        problems.append("guard reset points differ from the reset points")
    return problems


def footprint(p):
    """Conservative (x0, x1, y0, y1) on the ground plane, or None for the ground itself."""
    import spn_district_layout as D
    return D._prim_footprint(p)


class MapGrid:
    def __init__(self, data, radius=L.CAPSULE_RADIUS):
        self.half = data.get("half_extent_cm", L.HALF)
        self.n = int(2 * self.half / CELL)
        self.blocked = set()
        for p in data["prims"]:
            if p["label"] == "SPN_MV_Floor" or p["label"].startswith("SPN_MV_Boundary"):
                continue
            top = p["center"][2] + p["size"][2] / 2.0
            if p["rot"][0] != 0.0 or p["rot"][2] != 0.0:
                top = max(top, p["center"][2] + max(p["size"]) / 2.0)
            if top <= L.MAX_STEP_HEIGHT:
                continue
            fp = footprint(p)
            if fp is None:
                continue
            for i in range(max(int((fp[0] - radius + self.half) // CELL), 0), min(int((fp[1] + radius + self.half) // CELL) + 1, self.n)):
                for j in range(max(int((fp[2] - radius + self.half) // CELL), 0), min(int((fp[3] + radius + self.half) // CELL) + 1, self.n)):
                    cx, cy = -self.half + (i + 0.5) * CELL, -self.half + (j + 0.5) * CELL
                    if fp[0] - radius < cx < fp[1] + radius and fp[2] - radius < cy < fp[3] + radius:
                        self.blocked.add((i, j))

    def cell(self, x, y):
        return (int((x + self.half) // CELL), int((y + self.half) // CELL))

    def flood(self, start_xy):
        start = self.cell(*start_xy)
        if start in self.blocked:
            return set()
        seen, stack = {start}, [start]
        while stack:
            i, j = stack.pop()
            for nb in ((i + 1, j), (i - 1, j), (i, j + 1), (i, j - 1)):
                if 0 <= nb[0] < self.n and 0 <= nb[1] < self.n and nb not in self.blocked and nb not in seen:
                    seen.add(nb)
                    stack.append(nb)
        return seen

    def _labels(self):
        """Connected components of the free cells (4-neighbourhood), computed once: {cell: component id}."""
        if getattr(self, "_label_map", None) is None:
            labels, next_id = {}, 0
            for i in range(self.n):
                for j in range(self.n):
                    if (i, j) in self.blocked or (i, j) in labels:
                        continue
                    labels[(i, j)] = next_id
                    stack = [(i, j)]
                    while stack:
                        a, b = stack.pop()
                        for nb in ((a + 1, b), (a - 1, b), (a, b + 1), (a, b - 1)):
                            if 0 <= nb[0] < self.n and 0 <= nb[1] < self.n and nb not in self.blocked and nb not in labels:
                                labels[nb] = next_id
                                stack.append(nb)
                    next_id += 1
            self._label_map = labels
        return self._label_map

    def reachable(self, a_xy, b_xy):
        """True when both points are free cells of the same connected component (one labelling pass serves any number of queries)."""
        labels = self._labels()
        a, b = labels.get(self.cell(*a_xy)), labels.get(self.cell(*b_xy))
        return a is not None and a == b


# --- door passage clearance ------------------------------------------------------------------------------------------------------------

DOOR_LEAF_LENGTH = 100.0      # ASpnTestDoor panel: scale (1, 0.1, 2) of a 100 cm cube
DOOR_LEAF_THICKNESS = 10.0
PASSAGE_STEP_HEIGHT = 30.0    # obstacles lower than this are walked over (the QA saw 25-30 cm being crossed); placeholder value
PASSAGE_BODY_TOP = 170.0      # obstacles above this do not touch the capsule
PASSAGE_REACH = (250.0, 110.0)   # start 250 cm outside the wall plane; the pinch scan runs to 110 cm inside (SpnQA.Doors walks to 110 cm behind the door)
PASSAGE_GOAL_DEPTH = 80.0     # the capsule counts as through when its centre is 80 cm behind the wall plane (radius 42 + half wall 10 + margin)
PASSAGE_MARGIN = 150.0        # extra room around start, goal and door
PASSAGE_CELL = 4.0


def _rect_distance(x, y, rect):
    dx = max(rect[0] - x, 0.0, x - rect[1])
    dy = max(rect[2] - y, 0.0, y - rect[3])
    return math.hypot(dx, dy)


def _passage_obstacles(data, door, open_angle, x_min, x_max, y_min, y_max):
    """[(footprint, label)] of everything a walking capsule can touch in the given window, including the open door leaf."""
    wx, hy = door["hinge"][0], door["hinge"][1]
    rects = []
    for p in data["prims"]:
        fp = footprint(p)
        if fp is None:
            continue
        z_lo, z_hi = p["center"][2] - p["size"][2] / 2.0, p["center"][2] + p["size"][2] / 2.0
        if z_hi <= PASSAGE_STEP_HEIGHT or z_lo >= PASSAGE_BODY_TOP:
            continue
        if fp[1] < x_min or fp[0] > x_max or fp[3] < y_min or fp[2] > y_max:
            continue
        rects.append((fp, p["label"]))
    yaw = math.radians(door["yaw"] + open_angle)
    tip = (wx + DOOR_LEAF_LENGTH * math.cos(yaw), hy + DOOR_LEAF_LENGTH * math.sin(yaw))
    half = DOOR_LEAF_THICKNESS / 2.0
    rects.append(((min(wx, tip[0]) - half, max(wx, tip[0]) + half, min(hy, tip[1]) - half, max(hy, tip[1]) + half), "door leaf"))
    return rects


def door_pinch(data, door, open_angle, inward=1.0, tight=2.0 * L.CAPSULE_RADIUS + 20.0, step=2.0):
    """Length (cm along the walking direction) over which the free corridor on the door row is narrower than `tight` (default capsule width + 20 cm).

    door_passage() says whether the capsule can get through; this says for how long it has to hold a line within a few cm. A leaf swinging into a
    hall next to a stair block makes a 100 cm long pinch, an outward leaf only the wall thickness. Returns {"length": cm, "min_gap": cm}."""
    wx, hy = door["hinge"][0], door["hinge"][1]
    row = hy - DOOR_LEAF_LENGTH / 2.0
    xa, xb = sorted((wx - inward * PASSAGE_REACH[0], wx + inward * PASSAGE_REACH[1]))
    rects = _passage_obstacles(data, door, open_angle, xa, xb, row - 400.0, row + 400.0)
    length, min_gap = 0.0, 1e9
    x = xa
    while x <= xb:
        south, north = row - 400.0, row + 400.0
        blocked = False
        for fp, _ in rects:
            if fp[0] < x < fp[1]:
                if fp[2] < row < fp[3]:
                    blocked = True
                elif fp[3] <= row:
                    south = max(south, fp[3])
                else:
                    north = min(north, fp[2])
        gap = 0.0 if blocked else north - south
        min_gap = min(min_gap, gap)
        if gap < tight:
            length += step
        x += step
    return {"length": length, "min_gap": min_gap}


def door_passage(data, door, open_angle, inward=1.0):
    """How comfortably a capsule gets through an open door in a wall plane x = const (Layout.door), from 250 cm outside to 80 cm inside the wall plane.

    Obstacles: every primitive that overlaps the capsule's height band (PASSAGE_STEP_HEIGHT .. PASSAGE_BODY_TOP) by its bounding footprint, and the
    OPEN LEAF (found in Unreal: the leaf of an inward swinging door and the stair block behind it left less than the capsule width).
    `inward` is +1 when the building lies at +X of the wall, -1 otherwise. The search is a bottleneck path on a 4 cm grid: the result is the best
    achievable minimum distance between the capsule centre and any obstacle. Returns
    {"clearance": that distance (cm, 0 when blocked), "slack": clearance - capsule radius (cm per side; < 0 means the capsule cannot pass),
     "limiting": label of the obstacle nearest to the best path's narrowest point}."""
    import heapq
    wx, hy = door["hinge"][0], door["hinge"][1]
    row = hy - DOOR_LEAF_LENGTH / 2.0
    start = (wx - inward * PASSAGE_REACH[0], row)
    goal_x = wx + inward * PASSAGE_GOAL_DEPTH
    x_min, x_max = min(start[0], goal_x) - PASSAGE_MARGIN, max(start[0], goal_x) + PASSAGE_MARGIN
    y_min, y_max = row - PASSAGE_MARGIN - 100.0, row + PASSAGE_MARGIN + 100.0
    rects = _passage_obstacles(data, door, open_angle, x_min, x_max, y_min, y_max)
    nx, ny = int((x_max - x_min) / PASSAGE_CELL) + 1, int((y_max - y_min) / PASSAGE_CELL) + 1
    clear = {}
    def clearance(i, j):
        key = (i, j)
        if key not in clear:
            x, y = x_min + i * PASSAGE_CELL, y_min + j * PASSAGE_CELL
            best = (1e9, None)
            for fp, label in rects:
                d = _rect_distance(x, y, fp)
                if d < best[0]:
                    best = (d, label)
            clear[key] = best
        return clear[key]
    si, sj = round((start[0] - x_min) / PASSAGE_CELL), round((start[1] - y_min) / PASSAGE_CELL)
    gi = round((goal_x - x_min) / PASSAGE_CELL)
    best = {(si, sj): clearance(si, sj)[0]}
    parent = {}
    heap = [(-best[(si, sj)], si, sj)]
    while heap:
        neg, i, j = heapq.heappop(heap)
        width = -neg
        if width < best.get((i, j), -1.0):
            continue
        for di, dj in ((1, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (1, -1), (-1, 1), (-1, -1)):
            a, b = i + di, j + dj
            if not (0 <= a < nx and 0 <= b < ny):
                continue
            w = min(width, clearance(a, b)[0])
            if w > best.get((a, b), -1.0):
                best[(a, b)] = w
                parent[(a, b)] = (i, j)
                heapq.heappush(heap, (-w, a, b))
    end = max((c for c in best if c[0] == gi), key=lambda c: best[c], default=None)
    value = best[end] if end else 0.0
    limiting, narrowest = None, 1e9
    cell = end
    while cell is not None:   # the obstacle nearest to the narrowest point of the best path
        d, label = clearance(*cell)
        if d < narrowest:
            narrowest, limiting = d, label
        cell = parent.get(cell)
    return {"clearance": value, "slack": value - L.CAPSULE_RADIUS, "limiting": limiting}
