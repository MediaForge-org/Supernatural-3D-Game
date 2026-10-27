"""Modular building kit for the PLACEHOLDER movement test map v2 (overnight work order M2-A.2, packages 6-8).

Pure Python, no Unreal import. A building is described as data (BuildingSpec) and turned into boxes, door actors,
openings and lights on a spn_movement_map_layout.Layout. Deterministic: the same spec and position give the same geometry.

Conventions (same as spn_movement_map_layout.py): centimetres, X east, Y north, Z up, ground surface Z = 0. A building is placed by
its south-west corner (x0, y0) of the wall centre lines; offsets in a spec are measured from that corner.
Sides: W (x = x0), E (x = x1), S (y = y0), N (y = y1). Door ACTORS (ASpnTestDoor) exist only for W/E walls, because the
actor hinges in a wall plane x = const; every other opening is an empty gap (window, doorway, garage gate).

Collision rules the kit keeps (checked by test_spn_district_layout.py):
- walls are built in pieces AROUND their openings, so an open doorway has no invisible collision;
- outer wall pieces do not overlap each other (W/E walls run over the corners, S/N walls stop at the W/E wall faces);
- a staircase runs against the W or E wall face with its foot facing the free hall to the south and its top ending in an upper
  landing in front of the N wall; the stairwell hole covers the whole flight (head room) and its open edges (side, foot end) have rails;
- steps are at most MAX_STEP_RISE high (below the character's 25 cm step height) and RUN deep.

Nothing here is a statement about the real game's architecture; all sizes are throw-away test values.
"""

import math
from dataclasses import dataclass

PLINTH = 5.0          # ground slab thickness above the terrain, cm
MAX_STEP_RISE = 18.5  # cm, comfortable indoor stairs; the character steps up 25 cm
STEP_RUN = 28.0       # cm
DOOR_WIDTH = 110.0    # doorway width for door actors and open doorways
DOOR_HEIGHT = 220.0
RAIL_HEIGHT = 100.0
ROOF_PITCH_DEGREES = 25.0
ROOF_OVERHANG = 30.0
ROOF_THICKNESS = 12.0


@dataclass(frozen=True)
class Opening:
    """A gap in an outer wall. `centre` is the offset along the wall from the SW corner; sill/head are heights above the floor of
    `floor` (0 = ground floor). `door_id` set = a door actor sits in it (W/E walls only)."""
    side: str
    centre: float
    width: float
    sill: float
    head: float
    floor: int = 0
    door_id: str = ""


@dataclass(frozen=True)
class InnerWall:
    """Interior wall. axis 'x': plane x = x0 + pos running along Y from offset a0 to a1 (wall FACES, from the SW corner);
    axis 'y': plane y = y0 + pos running along X. `doorways` are (centre, width) pairs along the running direction."""
    axis: str
    pos: float
    a0: float
    a1: float
    doorways: tuple = ()
    floor: int = 0


@dataclass(frozen=True)
class Room:
    name: str
    x0: float
    x1: float
    y0: float
    y1: float
    floor: int = 0

    @property
    def centre(self):
        return ((self.x0 + self.x1) / 2.0, (self.y0 + self.y1) / 2.0)


@dataclass(frozen=True)
class BuildingSpec:
    name: str
    kind: str                      # cottage | house | warehouse | workshop | garage
    size: tuple                    # (sx, sy): distance between the wall centre lines
    floors: int = 1
    floor_height: float = 300.0    # top of one floor slab to the top of the next
    wall_thickness: float = 20.0
    slab_thickness: float = 20.0
    openings: tuple = ()
    inner_walls: tuple = ()
    stair: bool = False            # one flight from floor 0 to 1 against the S and W walls (needs floors >= 2)
    stair_width: float = 120.0
    roof: str = "flat"             # flat | gable (ridge along Y)
    lights: tuple = ()             # (x offset, y offset, floor)
    rooms: tuple = ()              # Room(name, x0, x1, y0, y1, floor): clear interior rectangles (offsets from the SW corner), for plans/tests/export
    stair_landing: bool = False    # a flat landing (stair_width long) in the middle of the flight
    stair_side: str = "W"          # the flight runs against this outer wall (W or E)
    stair_top_depth: float = 200.0 # upper-floor landing north of the stairwell, measured from the stairwell end to the N wall centre line


def floor_top(spec, floor):
    return PLINTH if floor == 0 else floor * spec.floor_height


def wall_top(spec, floor):
    """Underside of the slab above, or the ceiling height of the top floor."""
    if floor < spec.floors - 1:
        return (floor + 1) * spec.floor_height - spec.slab_thickness
    return floor_top(spec, floor) + spec.floor_height - spec.slab_thickness - (PLINTH if floor == 0 else 0.0)


def stair_geometry(spec):
    """(steps, rise, run, x_offset of the west edge, y_offset of the first riser) of the staircase, offsets from the SW corner.
    The flight rises towards +Y: its foot (the first riser, at y_offset) faces the free hall to the south, its top ends in the upper
    landing in front of the N wall. A flight that started at the S wall would have its foot walled in."""
    rise_total = floor_top(spec, 1) - floor_top(spec, 0)
    steps = int(math.ceil(rise_total / MAX_STEP_RISE))
    x_offset = spec.wall_thickness / 2.0 if spec.stair_side == "W" else spec.size[0] - spec.wall_thickness / 2.0 - spec.stair_width
    steps_run_length = steps * STEP_RUN + (spec.stair_width if spec.stair_landing else 0.0)
    y_offset = spec.size[1] - spec.stair_top_depth - steps_run_length
    return steps, rise_total / steps, STEP_RUN, x_offset, y_offset


def stair_length(spec):
    """Length of the whole stairwell along Y: all runs plus the landing."""
    steps, _, run, _, _ = stair_geometry(spec)
    return steps * run + (spec.stair_width if spec.stair_landing else 0.0)


def stair_steps_layout(spec):
    """[(y offset of the step's first edge from the first riser, run, top height above the lower floor)] for every tread, landing included
    (a landing is one tread of length stair_width)."""
    steps, rise, run, _, _ = stair_geometry(spec)
    n1 = steps // 2 if spec.stair_landing else steps
    out, y = [], 0.0
    for i in range(1, n1 + 1):
        out.append((y, run, i * rise))
        y += run
    if spec.stair_landing:
        out.append((y, spec.stair_width, n1 * rise))
        y += spec.stair_width
        for i in range(1, steps - n1 + 1):
            out.append((y, run, (n1 + i) * rise))
            y += run
    return out


def validate_spec(spec):
    """nullopt-style: returns None when valid, else a short reason."""
    sx, sy = spec.size
    if sx < 300.0 or sy < 300.0:
        return "footprint below 3 x 3 m"
    if spec.floors < 1 or spec.floors > 3:
        return "floors must be 1..3"
    if spec.floor_height < 250.0:
        return "floor height below 2.5 m"
    if spec.roof not in ("flat", "gable"):
        return "unknown roof"
    if spec.stair:
        if spec.floors < 2:
            return "a stair needs two floors"
        if stair_geometry(spec)[4] < spec.wall_thickness + 250.0:
            return "stair flight does not fit the depth (needs 2.5 m of free hall south of the foot)"
        if spec.stair_top_depth < 190.0:
            return "upper landing shallower than 1.9 m"
        if spec.stair_side not in ("W", "E"):
            return "stair side must be W or E"
        if spec.stair_width < 100.0:
            return "stair narrower than the capsule needs"
    elif spec.floors > 1:
        return "a multi-storey building needs a stair"
    for o in spec.openings:
        if o.side not in ("W", "E", "S", "N"):
            return "bad opening side"
        if o.door_id and o.side not in ("W", "E"):
            return "door actors only in W/E walls"
        length = sy if o.side in ("W", "E") else sx
        if o.centre - o.width / 2.0 <= spec.wall_thickness or o.centre + o.width / 2.0 >= length - spec.wall_thickness:
            return "opening too close to a corner"
        if not 0 <= o.floor < spec.floors:
            return "opening on a missing floor"
    return None


def _wall_pieces_for_side(layout, spec, side, label, zone, x0, y0, floor, material):
    sx, sy = spec.size
    t = spec.wall_thickness
    z0 = floor_top(spec, floor)
    height = wall_top(spec, floor) - z0
    openings = []
    for o in spec.openings:
        if o.side == side and o.floor == floor:
            openings.append(o)
    if side in ("W", "E"):
        fixed = x0 if side == "W" else x0 + sx
        a0, a1 = y0 - t / 2.0, y0 + sy + t / 2.0
        base = y0
        axis = "x"
    else:
        fixed = y0 if side == "S" else y0 + sy
        a0, a1 = x0 + t / 2.0, x0 + sx - t / 2.0
        base = x0
        axis = "y"
    cuts = [(base + o.centre - o.width / 2.0, base + o.centre + o.width / 2.0, z0 + o.sill, z0 + o.head) for o in openings]
    layout.wall(label, zone, axis, fixed, a0, a1, z0, height, t, openings=cuts, material=material)


def _slab_with_hole(layout, label, zone, fx0, fx1, fy0, fy1, z_top, thickness, hole):
    """Slab over the footprint rectangle with an optional rectangular hole (hx0, hx1, hy0, hy1); up to four pieces."""
    zc = z_top - thickness / 2.0
    if hole is None:
        pieces = [(fx0, fx1, fy0, fy1)]
    else:
        hx0, hx1, hy0, hy1 = hole
        pieces = [(fx0, hx0, fy0, fy1), (hx1, fx1, fy0, fy1), (hx0, hx1, fy0, hy0), (hx0, hx1, hy1, fy1)]
    index = 0
    for px0, px1, py0, py1 in pieces:
        if px1 - px0 < 0.5 or py1 - py0 < 0.5:
            continue
        index += 1
        layout.add("%s_%d" % (label, index), "cube", ((px0 + px1) / 2.0, (py0 + py1) / 2.0, zc), (px1 - px0, py1 - py0, thickness), zone)


def _roof(layout, spec, prefix, zone, x0, y0):
    sx, sy = spec.size
    t = spec.wall_thickness
    z_wall = wall_top(spec, spec.floors - 1)
    cx, cy = x0 + sx / 2.0, y0 + sy / 2.0
    if spec.roof == "flat":
        layout.add(prefix + "_Roof", "cube", (cx, cy, z_wall + 10.0), (sx + t + 2 * ROOF_OVERHANG, sy + t + 2 * ROOF_OVERHANG, 20.0), zone)
        return
    a = math.radians(ROOF_PITCH_DEGREES)
    half = sx / 2.0 + t / 2.0 + ROOF_OVERHANG
    slope_len = half / math.cos(a)
    run_y = sy + t + 2 * ROOF_OVERHANG
    rise = half * math.tan(a)
    for tag, sign in (("A", 1.0), ("B", -1.0)):
        # sign +1: the west half, rising towards +X (positive pitch raises the +X end); -1: the east half.
        ex = cx - sign * half  # eave x
        mx, mz = (ex + cx) / 2.0, z_wall + rise / 2.0
        nx, nz = -sign * math.sin(a), math.cos(a)  # slab normal (up and outwards-in)
        layout.add("%s_Roof%s" % (prefix, tag), "cube", (mx - nx * ROOF_THICKNESS / 2.0, cy, mz - nz * ROOF_THICKNESS / 2.0),
                   (slope_len, run_y, ROOF_THICKNESS), zone, rot=(sign * ROOF_PITCH_DEGREES, 0.0, 0.0))
    # Gable ends: stepped boxes under the roof line, narrower than the roof underside at their top edge.
    levels = 4
    inner_rise = (sx / 2.0) * math.tan(a)
    for end, y in (("S", y0), ("N", y0 + sy)):
        for k in range(levels):
            width = sx * (1.0 - (k + 1.0) / levels)
            if width < 20.0:
                continue
            h = inner_rise / levels
            layout.add("%s_Gable%s_%d" % (prefix, end, k), "cube", (cx, y, z_wall + (k + 0.5) * h), (width, t, h), zone)


def build_building(layout, spec, x0, y0, prefix, zone, material="grid_gray_02"):
    """Add one building with its SW wall-centre corner at (x0, y0). Returns the footprint (xmin, xmax, ymin, ymax) incl. roof overhang."""
    problem = validate_spec(spec)
    if problem:
        raise ValueError("%s: %s" % (spec.name, problem))
    sx, sy = spec.size
    t = spec.wall_thickness
    fx0, fx1, fy0, fy1 = x0 - t / 2.0, x0 + sx + t / 2.0, y0 - t / 2.0, y0 + sy + t / 2.0
    layout.add(prefix + "_Floor", "cube", ((fx0 + fx1) / 2.0, (fy0 + fy1) / 2.0, PLINTH / 2.0), (fx1 - fx0, fy1 - fy0, PLINTH), zone)

    hole = None
    if spec.stair:
        steps, rise, run, sxo, syo = stair_geometry(spec)
        hole = (x0 + sxo, x0 + sxo + spec.stair_width, y0 + syo, y0 + syo + stair_length(spec))

    for f in range(spec.floors):
        if f >= 1:
            _slab_with_hole(layout, "%s_Slab%d" % (prefix, f), zone, fx0, fx1, fy0, fy1, floor_top(spec, f), spec.slab_thickness,
                            hole if f == 1 else None)
        for side in ("W", "E", "S", "N"):
            _wall_pieces_for_side(layout, spec, side, "%s_F%d_%s" % (prefix, f, side), zone, x0, y0, f, material)
        z0 = floor_top(spec, f)
        height = wall_top(spec, f) - z0
        for i, w in enumerate(spec.inner_walls):
            if w.floor != f:
                continue
            cuts = []
            for c, width in w.doorways:
                base = y0 if w.axis == "x" else x0
                cuts.append((base + c - width / 2.0, base + c + width / 2.0, z0, z0 + DOOR_HEIGHT))
            if w.axis == "x":
                layout.wall("%s_F%d_In%d" % (prefix, f, i), zone, "x", x0 + w.pos, y0 + w.a0, y0 + w.a1, z0, height, t, openings=cuts, material=material)
            else:
                layout.wall("%s_F%d_In%d" % (prefix, f, i), zone, "y", y0 + w.pos, x0 + w.a0, x0 + w.a1, z0, height, t, openings=cuts, material=material)

    for o in spec.openings:
        if o.door_id:
            fixed = x0 if o.side == "W" else x0 + sx
            layout.door(o.door_id, fixed, y0 + o.centre, zone)

    if spec.stair:
        steps, rise, run, sxo, syo = stair_geometry(spec)
        for i, (y_off, length, top) in enumerate(stair_steps_layout(spec), 1):
            layout.add("%s_Stair_%02d" % (prefix, i), "cube", (x0 + sxo + spec.stair_width / 2.0, y0 + syo + y_off + length / 2.0, PLINTH + top / 2.0),
                       (spec.stair_width, length, top), zone)
        hx0, hx1, hy0, hy1 = hole
        # Rail on the open east edge of the stairwell at the upper floor.
        rail_x = hx1 + 5.0 if spec.stair_side == "W" else hx0 - 5.0
        layout.rail_box(prefix + "_StairRail", zone, (rail_x, (hy0 + hy1) / 2.0, floor_top(spec, 1) + RAIL_HEIGHT / 2.0),
                        (10.0, hy1 - hy0, RAIL_HEIGHT))
        # The foot end of the stairwell is an open edge on the upper floor too.
        layout.rail_box(prefix + "_StairRailS", zone, ((hx0 + hx1) / 2.0, hy0 - 5.0, floor_top(spec, 1) + RAIL_HEIGHT / 2.0),
                        (hx1 - hx0, 10.0, RAIL_HEIGHT))

    _roof(layout, spec, prefix, zone, x0, y0)

    for lx, ly, lf in spec.lights:
        lights = getattr(layout, "lights", None)
        if lights is None:
            layout.lights = lights = []
        lights.append({"pos": [round(x0 + lx, 3), round(y0 + ly, 3), round(floor_top(spec, lf) + spec.floor_height - spec.slab_thickness - 30.0, 3)],
                       "intensity": 2000.0, "radius": 600.0, "zone": zone})

    over = ROOF_OVERHANG if spec.roof in ("flat", "gable") else 0.0
    return (fx0 - over, fx1 + over, fy0 - over, fy1 + over)
