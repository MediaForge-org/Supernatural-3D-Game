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
