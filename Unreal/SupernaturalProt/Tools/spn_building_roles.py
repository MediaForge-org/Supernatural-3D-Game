"""Collision roles, geometric validation and export of generated buildings (M2-A.2 phase 2, layers 4-6 of spn_building_templates.py).

Roles separate what a box IS for: `collision` (blocks the capsule and the camera), `walkable` (floors, slabs, stair treads). `check_building` / `check_layout` is the
one place that turns "this variant is valid" into automatic checks; the tests and the district generator both call it. The export
for an engine-side builder lives in spn_building_export.py.

Pure Python, no Unreal import. Nothing here proves engine behaviour: collision shapes, CharacterMovement stepping and the camera are
Windows tests. Units: cm.
"""

import re

import spn_building_kit as kit
import spn_movement_map_layout as L

CAPSULE_RADIUS = L.CAPSULE_RADIUS
CAPSULE_HALF_HEIGHT = L.CAPSULE_HALF_HEIGHT
STEP_HEIGHT = L.MAX_STEP_HEIGHT
CELL = 10.0
MIN_CLEAR_CEILING = 240.0  # cm, clear height of every floor

WALKABLE_ROLES = ("floor", "slab", "stair")
_ROLE_PATTERNS = (
    (re.compile(r"_Stair_\d+$"), "stair"),
    (re.compile(r"_StairRail\w*$"), "rail"),
    (re.compile(r"_Floor$"), "floor"),
    (re.compile(r"_Slab\d+_\d+$"), "slab"),
    (re.compile(r"_Roof[A-Z]?$|_Gable[SN]_\d+$"), "roof"),
    (re.compile(r"_F\d+_"), "wall"),
)


def role_of(label):
    for pattern, role in _ROLE_PATTERNS:
        if pattern.search(label):
            return role
    return "wall"


def build_standalone(spec, prefix="B"):
    """The building at the origin on its own Layout (the unit the checks work on)."""
    layout = L.Layout()
    kit.build_building(layout, spec, 0.0, 0.0, prefix, "z")
    return layout


def cube_box(p):
    """3D AABB (x0, x1, y0, y1, z0, z1) of an axis-aligned cube primitive, else None."""
    if p["shape"] != "cube":
        return None
    pitch, yaw, roll = p["rot"]
    if abs(pitch) > 1e-6 or abs(roll) > 1e-6 or abs(yaw % 90.0) > 1e-6:
        return None
    cx, cy, cz = p["center"]
    sx, sy, sz = p["size"]
    if round(yaw / 90.0) % 2:
        sx, sy = sy, sx
    return (cx - sx / 2, cx + sx / 2, cy - sy / 2, cy + sy / 2, cz - sz / 2, cz + sz / 2)


def overlap3(a, b, eps=0.01):
    return all(a[i] < b[i + 1] - eps and a[i + 1] > b[i] + eps for i in (0, 2, 4))


def free_cells(layout, spec, floor, x_range, y_range, radius=CAPSULE_RADIUS, step=CELL):
    """{(i, j): bool} walkable cells of one floor for a capsule of `radius`: blocked when a box that reaches above the step height and
    below head height (relative to the floor) is within `radius` of the cell centre. Stair treads higher than the step height block."""
    z0 = kit.floor_top(spec, floor)
    solids = []
    for p in layout.prims:
        box = cube_box(p)
        if not box:
            continue
        role = role_of(p["label"])
        if role in ("floor", "slab", "roof"):
            continue
        if box[4] < z0 + 2.0 * CAPSULE_HALF_HEIGHT and box[5] > z0 + STEP_HEIGHT:
            solids.append(box)
    nx, ny = int((x_range[1] - x_range[0]) / step), int((y_range[1] - y_range[0]) / step)
    free = {(i, j): True for i in range(nx) for j in range(ny)}
    for b in solids:  # block the cells whose centre lies strictly inside the solid grown by the capsule radius
        i0 = max(int((b[0] - radius - x_range[0]) // step) - 1, 0)
        i1 = min(int((b[1] + radius - x_range[0]) // step) + 2, nx)
        j0 = max(int((b[2] - radius - y_range[0]) // step) - 1, 0)
        j1 = min(int((b[3] + radius - y_range[0]) // step) + 2, ny)
        for i in range(i0, i1):
            x = x_range[0] + (i + 0.5) * step
            if not b[0] - radius < x < b[1] + radius:
                continue
            for j in range(j0, j1):
                y = y_range[0] + (j + 0.5) * step
                if b[2] - radius < y < b[3] + radius:
                    free[(i, j)] = False
    return free


def reach(free, start):
    seen, stack = {start}, [start]
    while stack:
        i, j = stack.pop()
        for n in ((i + 1, j), (i - 1, j), (i, j + 1), (i, j - 1)):
            if free.get(n) and n not in seen:
                seen.add(n)
                stack.append(n)
    return seen


def entrance(spec):
    """The entrance opening: the door-actor opening, else the widest W/E opening on the ground floor."""
    doors = [o for o in spec.openings if o.door_id]
    if doors:
        return doors[0]
    gates = [o for o in spec.openings if o.side in ("W", "E") and o.floor == 0 and o.sill == 0.0]
    return max(gates, key=lambda o: o.width) if gates else None


# Error codes (stable, machine readable). The message after the code keeps the human wording.
CODES = ("spec", "overlap", "ceiling", "no_floor", "floating", "wall_gap", "doorway_blocked", "doorway_small", "swing_blocked",
         "window_conflict", "opening_overlap", "wall_sliver", "no_entrance", "entrance_blocked", "room_unreachable", "stair_comfort",
         "stairwell_blocked", "stair_foot_blocked", "stair_top_blocked", "upper_room_unreachable", "graph_mismatch", "dead_room")


def _expected_walls(spec):
    """{wall label: (length, height)} for every outer and inner wall the kit builds (labels as in spn_building_kit.build_building)."""
    t = spec.wall_thickness
    sx, sy = spec.size
    out = {}
    for f in range(spec.floors):
        h = kit.wall_top(spec, f) - kit.floor_top(spec, f)
        out["B_F%d_W" % f] = out["B_F%d_E" % f] = (sy + t, h)
        out["B_F%d_S" % f] = out["B_F%d_N" % f] = (sx - t, h)
    for i, w in enumerate(spec.inner_walls):
        h = kit.wall_top(spec, w.floor) - kit.floor_top(spec, w.floor)
        out["B_F%d_In%d" % (w.floor, i)] = (w.a1 - w.a0, h)
    return out


def _touch(a, b, eps=0.6):
    return all(a[i] <= b[i + 1] + eps and a[i + 1] >= b[i] - eps for i in (0, 2, 4))


def _check_support(boxes):
    """Every box must be connected (touching or overlapping, 0.6 cm tolerance) to the ground through other boxes."""
    n = len(boxes)
    grounded = [i for i, (_, b) in enumerate(boxes) if b[4] <= 0.6]
    seen, stack = set(grounded), list(grounded)
    order = sorted(range(n), key=lambda i: boxes[i][1][0])
    while stack:
        i = stack.pop()
        for j in range(n):
            if j not in seen and _touch(boxes[i][1], boxes[j][1]):
                seen.add(j)
                stack.append(j)
    return [boxes[i][0] for i in range(n) if i not in seen]


def room_graph(spec):
    """Rooms and their connections as {"nodes": [...], "edges": [(a, b, kind)]}. Nodes are "outside" and "name@floor". Edges: entrance
    (outside <-> room behind an entrance opening), doorway (inner-wall doorway between the rooms on its two sides), stair (rooms at
    the foot and at the top of the flight). A doorway whose sides do not both lie in a room is reported as an edge to "?"."""
    t = spec.wall_thickness
    sx, sy = spec.size

    def room_at(x, y, floor):
        for r in spec.rooms:
            if r.floor == floor and r.x0 - 1.0 <= x <= r.x1 + 1.0 and r.y0 - 1.0 <= y <= r.y1 + 1.0:
                return "%s@%d" % (r.name, r.floor)
        return "?"

    edges = []
    for o in spec.openings:
        if o.side not in ("W", "E") or o.sill > 0.0 or o.width < 2 * CAPSULE_RADIUS:
            continue
        x = t / 2 + 30.0 if o.side == "W" else sx - t / 2 - 30.0
        edges.append(("outside", room_at(x, o.centre, o.floor), "entrance"))
    for w in spec.inner_walls:
        for c, width in w.doorways:
            if w.axis == "x":
                a, b = room_at(w.pos - t / 2 - 30.0, c, w.floor), room_at(w.pos + t / 2 + 30.0, c, w.floor)
            else:
                a, b = room_at(c, w.pos - t / 2 - 30.0, w.floor), room_at(c, w.pos + t / 2 + 30.0, w.floor)
            edges.append((a, b, "doorway"))
    if spec.stair:
        _, _, _, sxo, syo = kit.stair_geometry(spec)
        cx = sxo + spec.stair_width / 2
        edges.append((room_at(cx, syo - 40.0, 0), room_at(cx, syo + kit.stair_length(spec) + 60.0, 1), "stair"))
    nodes = ["outside"] + ["%s@%d" % (r.name, r.floor) for r in spec.rooms]
    return {"nodes": nodes, "edges": edges}


def graph_reachable(graph, start="outside"):
    adj = {}
    for a, b, _ in graph["edges"]:
        adj.setdefault(a, set()).add(b)
        adj.setdefault(b, set()).add(a)
    seen, stack = {start}, [start]
    while stack:
        for n in adj.get(stack.pop(), ()):
            if n not in seen:
                seen.add(n)
                stack.append(n)
    return seen


def check_layout(spec, layout, radius=CAPSULE_RADIUS, check_swing=True):
    """[(code, detail)] for a built `layout` of `spec`. Separate from build so that tests can mutate the geometry."""
    problems = []
    add = lambda code, detail: problems.append((code, detail))  # noqa: E731
    boxes = [(p["label"], cube_box(p)) for p in layout.prims if cube_box(p)]
    t = spec.wall_thickness

    # 1. overlapping collision boxes (3D, 0.01 cm tolerance); sorted sweep over x
    ordered = sorted(boxes, key=lambda lb: lb[1][0])
    for i, (la, a) in enumerate(ordered):
        for lb, b in ordered[i + 1:]:
            if b[0] >= a[1]:
                break
            if overlap3(a, b):
                add("overlap", "%s / %s" % (la, lb))

    # 2. floating geometry
    for label in _check_support(boxes):
        add("floating", label)

    # 3. wall pieces + openings must exactly tile every wall (no gaps, no double coverage)
    pieces = {}
    for label, b in boxes:
        m = re.match(r"^(B_F\d+_(?:[WESN]|In\d+))_\d\d$", label)
        if m:
            run = (b[2], b[3]) if (b[1] - b[0]) < (b[3] - b[2]) and b[1] - b[0] <= t + 0.01 else (b[0], b[1])
            length = run[1] - run[0]
            pieces[m.group(1)] = pieces.get(m.group(1), 0.0) + length * (b[5] - b[4])
    openings = {}
    for o in layout.openings:
        openings[o["wall"]] = openings.get(o["wall"], 0.0) + o["width"] * o["height"]
    for wall, (length, height) in _expected_walls(spec).items():
        total = pieces.get(wall, 0.0) + openings.get(wall, 0.0)
        if abs(total - length * height) > 1.0:
            add("wall_gap", "%s covers %.0f of %.0f cm2" % (wall, total, length * height))

    # 4. ceilings and floors
    for f in range(spec.floors):
        ceiling = kit.wall_top(spec, f) - kit.floor_top(spec, f)
        if ceiling < MIN_CLEAR_CEILING:
            add("ceiling", "ceiling of floor %d only %.0f cm" % (f, ceiling))
    surfaces = [b for label, b in boxes if role_of(label) in ("floor", "slab")]
    for r in spec.rooms:
        cx, cy = r.centre
        top = kit.floor_top(spec, r.floor)
        if not any(b[0] <= cx <= b[1] and b[2] <= cy <= b[3] and abs(b[5] - top) < 0.5 for b in surfaces):
            add("no_floor", "no floor under room %s (floor %d)" % (r.name, r.floor))

    # 5. openings: no overlap, no window on a partition, no sliver of wall between neighbours
    by_wall = {}
    for o in spec.openings:
        by_wall.setdefault((o.side, o.floor), []).append(o)
    sx, sy = spec.size
    for (side, floor), items in by_wall.items():
        items.sort(key=lambda o: o.centre)
        length = sy if side in ("W", "E") else sx
        for a, b in zip(items, items[1:]):
            gap = (b.centre - b.width / 2) - (a.centre + a.width / 2)
            if gap < 0.0:
                add("opening_overlap", "%s floor %d: openings at %.0f and %.0f overlap" % (side, floor, a.centre, b.centre))
            elif gap < 15.0:
                add("wall_sliver", "%s floor %d: %.0f cm of wall between openings" % (side, floor, gap))
        for o in items:
            if o.centre - o.width / 2 < t or o.centre + o.width / 2 > length - t:
                add("wall_sliver", "%s floor %d: opening at %.0f touches a corner" % (side, floor, o.centre))
            for w in spec.inner_walls:
                if w.floor != floor:
                    continue
                # an inner wall meets an outer wall when it runs along the other axis and reaches that wall's face
                meets = (w.axis == "y" and side in ("W", "E") and (w.a0 <= t if side == "W" else w.a1 >= sx - t)) or \
                        (w.axis == "x" and side in ("S", "N") and (w.a0 <= t if side == "S" else w.a1 >= sy - t))
                if meets and abs(w.pos - o.centre) < o.width / 2 + t / 2 + 5.0:
                    add("window_conflict", "%s floor %d: opening at %.0f meets a partition at %.0f" % (side, floor, o.centre, w.pos))

    # 6. doorways free of collision and big enough for the capsule
    for o in layout.openings:
        if o["kind"] != "doorway":
            continue
        floor = int(re.search(r"_F(\d+)_", o["wall"]).group(1))
        z0 = kit.floor_top(spec, floor) + o["sill"]
        vol = ((o["fixed"] - 8, o["fixed"] + 8, o["from"] + 0.5, o["to"] - 0.5, z0 + 1.0, z0 + o["height"] - 1.0) if o["axis"] == "x"
               else (o["from"] + 0.5, o["to"] - 0.5, o["fixed"] - 8, o["fixed"] + 8, z0 + 1.0, z0 + o["height"] - 1.0))
        for label, b in boxes:
            if role_of(label) in ("wall", "stair") and overlap3(b, vol):
                add("doorway_blocked", "doorway of %s blocked by %s" % (o["wall"], label))
        if o["width"] < 2.0 * radius + 4.0 or o["height"] < 2.0 * CAPSULE_HALF_HEIGHT + 4.0:
            add("doorway_small", "doorway of %s too small for the capsule" % o["wall"])

    if check_swing:
        for d in layout.doors:
            hx, hy = d["hinge"][0], d["hinge"][1]
            for sign in (-1.0, 1.0):  # either swing direction (which one the actor uses is UNCONFIRMED)
                x_lo, x_hi = sorted((hx + sign * 10.0, hx + sign * 100.0))
                zone = (x_lo, x_hi, hy - 100.0, hy, 30.0, 200.0)
                for label, b in boxes:
                    if role_of(label) in ("wall", "stair") and overlap3(b, zone):
                        add("swing_blocked", "door %s swing area blocked by %s" % (d["id"], label))

    # 7. reachability for a capsule of `radius`
    ent = entrance(spec)
    if ent is None:
        add("no_entrance", "no entrance")
        return problems
    east = ent.side == "E"
    x_range = (-300.0, sx + 300.0)
    y_range = (-100.0, sy + 100.0)
    free0 = free_cells(layout, spec, 0, x_range, y_range, radius)
    start = (int(((sx + 150.0 if east else -150.0) - x_range[0]) / CELL), int((ent.centre - y_range[0]) / CELL))
    if not free0.get(start):
        add("entrance_blocked", "entrance blocked outside")
        return problems
    seen = reach(free0, start)
    for r in spec.rooms:
        if r.floor == 0:
            cx, cy = r.centre
            if (int((cx - x_range[0]) / CELL), int((cy - y_range[0]) / CELL)) not in seen:
                add("room_unreachable", "room %s unreachable" % r.name)

    if spec.stair:
        steps, rise, run, sxo, syo = kit.stair_geometry(spec)
        if rise > kit.MAX_STEP_RISE or rise >= STEP_HEIGHT or run < 27.0 or spec.stair_width < 2 * radius + 16.0:
            add("stair_comfort", "stair not comfortable (rise %.1f run %.1f width %.0f)" % (rise, run, spec.stair_width))
        foot = (int((sxo + spec.stair_width / 2 - x_range[0]) / CELL), int((syo - 50.0 - y_range[0]) / CELL))
        if foot not in seen:
            add("stair_foot_blocked", "the foot of the stair cannot be reached from the entrance")
        flight = (sxo + 1.0, sxo + spec.stair_width - 1.0, syo + 1.0, syo + kit.stair_length(spec) - 1.0, kit.floor_top(spec, 1) - 20.0, kit.floor_top(spec, 1) + 200.0)
        for label, b in boxes:
            if role_of(label) != "stair" and overlap3(b, flight):
                add("stairwell_blocked", "stairwell blocked by " + label)
        xr, yr = (-50.0, sx + 50.0), (-50.0, sy + 50.0)
        free1 = free_cells(layout, spec, 1, xr, yr, radius)
        arrival = (int((sxo + spec.stair_width / 2 - xr[0]) / CELL), int((syo + kit.stair_length(spec) + 60.0 - yr[0]) / CELL))
        if not free1.get(arrival):
            add("stair_top_blocked", "stair top blocked")
        else:
            seen1 = reach(free1, arrival)
            for r in spec.rooms:
                if r.floor == 1:
                    cx, cy = r.centre
                    if (int((cx - xr[0]) / CELL), int((cy - yr[0]) / CELL)) not in seen1:
                        add("upper_room_unreachable", "upper room %s unreachable" % r.name)

    # 8. the room graph must agree with the flood fill and have no loose rooms
    graph = room_graph(spec)
    linked = graph_reachable(graph)
    for node in graph["nodes"]:
        if node not in linked:
            add("dead_room", "room %s has no connection from the entrance" % node)
    if any("?" in (a, b) for a, b, _ in graph["edges"]):
        add("graph_mismatch", "a doorway does not connect two rooms")
    return problems


def check_building(spec, radius=CAPSULE_RADIUS, check_swing=True):
    """All geometric problems of one building as strings "code: detail" (empty list = valid)."""
    return ["%s: %s" % (c, d) for c, d in check_codes(spec, radius, check_swing)]


def check_codes(spec, radius=CAPSULE_RADIUS, check_swing=True):
    bad = kit.validate_spec(spec)
    if bad:
        return [("spec", bad)]
    return check_layout(spec, build_standalone(spec), radius, check_swing)


def building_report(building_id, template, seed, spec):
    """Compact machine-readable validation result of one building (JSON-able)."""
    codes = check_codes(spec)
    return {
        "id": building_id, "template": template, "seed": seed, "kind": spec.kind,
        "rooms": len(spec.rooms), "floors": spec.floors,
        "doors": sum(1 for o in spec.openings if o.door_id), "doorways": sum(len(w.doorways) for w in spec.inner_walls) + sum(1 for o in spec.openings if o.door_id),
        "windows": sum(1 for o in spec.openings if o.sill > 0.0), "stairs": 1 if spec.stair else 0,
        "status": "ok" if not codes else "invalid", "errors": sorted({c for c, _ in codes}),
        "error_count": len(codes),
    }


def navigation_rects(spec):
    """Walkable rectangles (offsets from the SW corner) per floor: every room plus the stair footprint; the rest inside the walls is blocked."""
    rects = [{"floor": r.floor, "name": r.name, "rect": [r.x0, r.x1, r.y0, r.y1]} for r in spec.rooms]
    if spec.stair:
        _, _, _, sxo, syo = kit.stair_geometry(spec)
        rects.append({"floor": 0, "name": "stair", "rect": [sxo, sxo + spec.stair_width, syo, syo + kit.stair_length(spec)], "sloped": True})
    return rects
