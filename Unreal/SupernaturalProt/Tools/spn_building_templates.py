"""Parametric building templates and the variant generator (M2-A.2 phase 2).

Layers (kept apart on purpose; each can be replaced without touching the others):
  1. DESCRIPTION  Template (a building TYPE as data) + Params (one concrete variant).
  2. PLAN         generate() -> spn_building_kit.BuildingSpec: rooms, interior walls, doorways, openings, stair, roof, lights.
  3. GEOMETRY     spn_building_kit.build_building(): boxes (walls built AROUND openings).
  4. COLLISION    spn_building_roles.py: role of every box (collision/visual/navigation) and the validation checks.
  5. ASSETS       spn_building_roles.ASSET_KEYS: role -> asset key; today every key resolves to an engine cube.
  6. EXPORT       spn_building_roles.export_building(): plain JSON-able description for an Unreal-side builder.

Layouts: dwelling (entrance hall/stair strip + 2..3 rooms), corridor (office: central corridor, rooms on both sides), hall (storage),
workshop (bay + office), garage. A new type is a new Template entry. `make_variants` draws valid variants deterministically.

Pure Python, no Unreal import. Sizes are placeholder test values, not architecture decisions of the real game. Units: cm.
"""

from dataclasses import dataclass

import spn_building_kit as kit
import spn_movement_map_layout as v1

WALL = 20.0
MIN_ROOM = 250.0      # smallest clear room side, cm
MIN_HALL = 130.0      # smallest hall / corridor width (capsule diameter is 84)
MIN_CEILING = 240.0   # clear height, cm
WINDOW_WIDTH = 120.0
WINDOW_SILL = 95.0
WINDOW_HEAD = 215.0
WINDOW_MARGIN = 60.0  # clear wall between a window and a door / partition / corner
DOOR_CLEAR = 55.0 + 20.0  # half doorway + margin to a wall face


@dataclass(frozen=True)
class Template:
    name: str
    kind: str
    layout: str                 # dwelling | corridor | hall | workshop | garage
    width: tuple                # (min, max) X size of the wall centre-line rectangle, cm, in steps of `step`
    depth: tuple                # (min, max) Y size
    floors: tuple               # allowed floor counts
    door_sides: tuple           # allowed entrance sides (W/E)
    roofs: tuple                # allowed roofs
    floor_heights: tuple        # allowed floor heights (slab top to slab top)
    window_spacing: tuple       # (min, max) distance between window centres
    hall_width: tuple = (160.0, 160.0)   # dwelling: entrance hall / stair strip; corridor: corridor clear width
    rooms: tuple = (2, 2)       # dwelling: rooms beside the hall; corridor: rooms per side (south side; north has rooms or rooms-1)
    landing: bool = False       # stair may have a mid-flight landing
    gate_width: float = 0.0
    step: float = 50.0


TEMPLATES = {
    "cottage": Template("cottage", "cottage", "dwelling", (800.0, 1000.0), (600.0, 1000.0), (1,), ("W", "E"), ("gable",), (300.0,), (300.0, 400.0), (150.0, 250.0), rooms=(2, 3)),
    "house": Template("house", "house", "dwelling", (1000.0, 1300.0), (900.0, 1200.0), (2,), ("W", "E"), ("gable", "flat"), (300.0, 320.0), (300.0, 400.0), rooms=(2, 3), landing=True),
    "office": Template("office", "office", "corridor", (1100.0, 2200.0), (900.0, 1300.0), (1,), ("W", "E"), ("flat", "gable"), (300.0, 320.0), (300.0, 450.0), (140.0, 220.0), rooms=(2, 4)),
    "warehouse": Template("warehouse", "warehouse", "hall", (1500.0, 2400.0), (1000.0, 1400.0), (1,), ("W",), ("flat",), (500.0, 520.0, 600.0), (400.0, 600.0), gate_width=360.0),
    "workshop": Template("workshop", "workshop", "workshop", (1300.0, 1800.0), (800.0, 1100.0), (1,), ("W",), ("flat", "gable"), (300.0, 350.0), (300.0, 400.0), gate_width=320.0),
    "garage": Template("garage", "garage", "garage", (550.0, 700.0), (500.0, 600.0), (1,), ("W", "E"), ("flat",), (300.0,), (300.0, 300.0), gate_width=300.0),
}


@dataclass(frozen=True)
class Params:
    width: float
    depth: float
    floors: int = 1
    door_side: str = "W"
    roof: str = "gable"
    floor_height: float = 300.0
    hall_width: float = 160.0
    window_spacing: float = 350.0
    door_id: str = ""
    seed_split: float = 0.5     # 0.35..0.65: varies the room proportions
    rooms: int = 2
    stair_landing: bool = False


def _snap(v, step=10.0):
    return round(v / step) * step


def window_centres(length, spacing, avoid):
    """Window centres along a wall of `length`: a regular rhythm, none closer than WINDOW_MARGIN (+ half window) to a corner or any
    `avoid` position (door, partition junction)."""
    out = []
    n = max(int(length // spacing), 1)
    pitch = length / n
    for k in range(n):
        c = _snap((k + 0.5) * pitch)
        if c - WINDOW_WIDTH / 2 < WALL + WINDOW_MARGIN or c + WINDOW_WIDTH / 2 > length - WALL - WINDOW_MARGIN:
            continue
        if any(abs(c - a) < WINDOW_WIDTH / 2 + WINDOW_MARGIN + 55.0 for a in avoid):
            continue
        out.append(c)
    return out


def validate_params(t, p):
    """None when the parameters are inside the template, else a short reason. Geometry-dependent failures raise in generate()."""
    if not t.width[0] <= p.width <= t.width[1] or not t.depth[0] <= p.depth <= t.depth[1]:
        return "footprint outside the template range"
    if p.floors not in t.floors:
        return "floor count not allowed"
    if p.door_side not in t.door_sides or p.roof not in t.roofs or p.floor_height not in t.floor_heights:
        return "door side, roof or floor height not allowed by the template"
    if p.floor_height - 20.0 < MIN_CEILING:
        return "ceiling lower than 2.4 m"
    if p.stair_landing and not t.landing:
        return "this template has no landing stair"
    if t.layout in ("dwelling", "corridor"):
        if not t.hall_width[0] <= p.hall_width <= t.hall_width[1]:
            return "hall width outside the template range"
        if p.hall_width < MIN_HALL:
            return "hall narrower than the capsule needs"
        if not t.rooms[0] <= p.rooms <= t.rooms[1]:
            return "room count outside the template range"
        if t.layout == "dwelling":
            if not rooms_fit(p.depth, p.rooms):
                return "depth too small for %d rooms" % p.rooms
            if p.width - p.hall_width - WALL < MIN_ROOM:
                return "rooms beside the hall too narrow"
            if p.floors > 1:
                probe = kit.BuildingSpec("x", "x", (p.width, p.depth), floors=p.floors, floor_height=p.floor_height, stair=True,
                                         stair_landing=p.stair_landing, stair_side=p.door_side)
                if kit.validate_spec(probe):
                    return "stair does not fit the depth"
                if _entrance_y(probe, kit.stair_geometry(probe)[4]) is None:
                    return "no room for the entrance beside the stair"
        else:  # corridor
            if not rooms_fit(p.width, p.rooms):
                return "width too small for %d rooms" % p.rooms
            if p.depth < 2 * (MIN_ROOM + WALL) + p.hall_width + WALL:
                return "depth too small for rooms on both sides of the corridor"
    return None


def _proportions(n, seed_split):
    """n deterministic, varied positive weights (sum 1)."""
    w = [1.0 + ((seed_split * 7.0 + i * 0.37) % 1.0) * 0.6 for i in range(n)]
    total = sum(w)
    return [x / total for x in w]


def rooms_fit(outer_length, n):
    """True when n rooms of at least MIN_ROOM clear size fit between the outer wall faces of a wall-centre-line length `outer_length`
    (outer walls and n-1 partitions are WALL thick)."""
    return outer_length - WALL - (n - 1) * WALL >= n * MIN_ROOM - 1e-9


def _cuts(lo, hi, n, seed_split):
    """Partition plane positions between n rooms inside [lo, hi] (outer wall FACES). Every room gets MIN_ROOM first; the remaining
    length is shared by the proportions (rounded down to 10 cm, the last room takes the rest), so no room is ever below MIN_ROOM."""
    clear = hi - lo - (n - 1) * WALL
    extra = clear - n * MIN_ROOM
    if extra < -1e-9:
        raise ValueError("%d rooms of %d cm do not fit into %.0f cm" % (n, MIN_ROOM, hi - lo))
    frac, out, pos = _proportions(n, seed_split), [], lo
    for f in frac[:-1]:
        size = MIN_ROOM + (extra * f) // 10.0 * 10.0
        pos += size
        out.append(pos + WALL / 2)
        pos += WALL
    return out


def _spans(lo, hi, planes):
    """Clear (face to face) spans of the rooms between the outer faces lo/hi and the partition planes."""
    edges = [lo] + list(planes) + [hi]
    return [(edges[i] + (WALL / 2 if i else 0.0), edges[i + 1] - (WALL / 2 if i + 1 < len(edges) - 1 else 0.0)) for i in range(len(edges) - 1)]


def _doorway_in(span, blocked_below=0.0, width=kit.DOOR_WIDTH, blocked_above=1e9):
    """Centre of a doorway inside the clear span, preferably the middle, at least 20 cm from the span ends and inside
    [blocked_below, blocked_above]; None when it does not fit."""
    lo = max(span[0], blocked_below) + width / 2 + 20.0
    hi = min(span[1], blocked_above) - width / 2 - 20.0
    if hi < lo:
        return None
    mid = _snap((span[0] + span[1]) / 2.0)
    return min(max(mid, lo), hi)


def _entrance_y(spec, stair_y0):
    """Y of the entrance door in the hall: the middle of the free hall south of the stair foot (`stair_y0`; the whole depth without a
    stair). None when the doorway plus margins does not fit."""
    top = spec.size[1] - WALL - 40.0 if not spec.stair else stair_y0 - 40.0
    lo = WALL + 60.0
    y = _snap((lo + top) / 2.0)
    if y - kit.DOOR_WIDTH / 2 < WALL + 40.0 or y + kit.DOOR_WIDTH / 2 > top + 20.0:
        return None
    if spec.stair and stair_y0 - 30.0 < MIN_ROOM:
        return None  # the free hall must be a usable room
    return y


def generate(template, p):
    """Template + parameters -> BuildingSpec. Raises ValueError on invalid parameters or geometry that cannot be made walkable."""
    problem = validate_params(template, p)
    if problem:
        raise ValueError("%s: %s" % (template.name, problem))
    door_id = p.door_id or ("d_" + template.name)
    sx, sy = p.width, p.depth
    openings, inner, lights, rooms = [], [], [], []
    multi = p.floors > 1

    def door(side, centre):
        return kit.Opening(side, centre, kit.DOOR_WIDTH, 0.0, kit.DOOR_HEIGHT, 0, door_id)

    def windows(side, avoid, floor=0, high=False):
        length = sy if side in ("W", "E") else sx
        for c in window_centres(length, p.window_spacing, avoid):
            openings.append(kit.Opening(side, c, 200.0, 300.0, 440.0, floor) if high else kit.Opening(side, c, WINDOW_WIDTH, WINDOW_SILL, WINDOW_HEAD, floor))

    def room(name, x0, x1, y0, y1, floor=0, narrow=False, landing=False):
        """Register a clear room rectangle; `narrow` rooms (hall, corridor) only need to be wide enough for the capsule on their short
        side, a stair `landing` on both sides."""
        lo, hi = sorted((x1 - x0, y1 - y0))
        if (lo < MIN_HALL - 0.5 or hi < (MIN_HALL if landing else MIN_ROOM) - 0.5) if narrow else (lo < MIN_ROOM - 0.5):
            raise ValueError("%s: room %s too small (%.0f x %.0f)" % (template.name, name, x1 - x0, y1 - y0))
        rooms.append(kit.Room(name, x0, x1, y0, y1, floor))

    if template.layout == "dwelling":
        hw = p.hall_width
        hall_on_west = p.door_side == "W"  # the hall (and a stair, which runs against the entrance-side wall) is on the entrance side
        px = hw if hall_on_west else sx - hw
        rx0, rx1 = (px + WALL / 2, sx - WALL / 2) if hall_on_west else (WALL / 2, px - WALL / 2)
        if rx1 - rx0 < MIN_ROOM:
            raise ValueError("%s: rooms beside the hall too narrow" % template.name)
        planes = _cuts(WALL / 2, sy - WALL / 2, p.rooms, p.seed_split)
        spans = _spans(WALL / 2, sy - WALL / 2, planes)
        probe = kit.BuildingSpec("x", "x", (sx, sy), floors=p.floors, floor_height=p.floor_height, stair=multi, stair_landing=p.stair_landing,
                                 stair_side=p.door_side)
        stair_y0 = kit.stair_geometry(probe)[4] if multi else sy
        stair_y1 = stair_y0 + kit.stair_length(probe) if multi else sy
        door_y = _entrance_y(probe, stair_y0)
        if door_y is None:
            raise ValueError("%s: no room for the entrance beside the stair" % template.name)
        openings.append(door(p.door_side, door_y))
        hall_free_top = stair_y0 - 20.0 if multi else sy - WALL / 2
        hall_x = (WALL / 2, px - WALL / 2) if hall_on_west else (px + WALL / 2, sx - WALL / 2)
        for f in range(p.floors):
            hall_doorways = []
            for span in spans:
                if f == 0 or not multi:
                    c = _doorway_in(span, blocked_above=hall_free_top)
                    if c is not None:
                        hall_doorways.append((c, kit.DOOR_WIDTH))
                else:  # upper floor: from the landing in front of the N wall, and from the hall part south of the stairwell
                    for kw in ({"blocked_below": stair_y1 + 20.0}, {"blocked_above": hall_free_top}):
                        c = _doorway_in(span, **kw)
                        if c is not None and all(abs(c - d[0]) > kit.DOOR_WIDTH + 20.0 for d in hall_doorways):
                            hall_doorways.append((c, kit.DOOR_WIDTH))
            if not hall_doorways:
                raise ValueError("%s: no room can be entered from the hall" % template.name)
            inner.append(kit.InnerWall("x", px, WALL / 2, sy - WALL / 2, doorways=tuple(sorted(hall_doorways)), floor=f))
            xmid = _snap((rx0 + rx1) / 2.0)
            for plane in planes:
                inner.append(kit.InnerWall("y", plane, rx0, rx1, doorways=((xmid, kit.DOOR_WIDTH),), floor=f))
            for i, span in enumerate(spans):
                room("room%d" % (i + 1), rx0, rx1, span[0], span[1], f)
                lights.append((_snap((rx0 + rx1) / 2.0), _snap((span[0] + span[1]) / 2.0), f))
            hall_cx = _snap((hall_x[0] + hall_x[1]) / 2.0)
            if not multi:
                room("hall", hall_x[0], hall_x[1], WALL / 2, sy - WALL / 2, f, narrow=True)
                lights.append((hall_cx, _snap(sy / 2.0), f))
            else:
                if hall_free_top - WALL / 2 >= MIN_ROOM:
                    room("hall" if f == 0 else "hall2", hall_x[0], hall_x[1], WALL / 2, hall_free_top, f, narrow=True)
                    lights.append((hall_cx, _snap((WALL / 2 + hall_free_top) / 2.0), f))
                if f == 1:
                    room("landing", hall_x[0], hall_x[1], stair_y1 + WALL / 2, sy - WALL / 2, f, narrow=True, landing=True)
                    lights.append((hall_cx, _snap((stair_y1 + sy) / 2.0), f))
            for side in ("S", "N"):
                windows(side, [px], f)
            for side in ("W", "E"):
                avoid = list(planes)
                if f == 0 and side == p.door_side:
                    avoid.append(door_y)
                if multi and side == p.door_side:
                    avoid += [stair_y0, stair_y1, (stair_y0 + stair_y1) / 2.0]
                windows(side, avoid, f)
    elif template.layout == "corridor":
        cw = p.hall_width
        yc0 = _snap((sy - cw - WALL) / 2.0)
        yc1 = yc0 + cw + WALL
        if yc0 - WALL < MIN_ROOM or sy - yc1 - WALL < MIN_ROOM:
            raise ValueError("%s: rooms beside the corridor too shallow" % template.name)
        openings.append(door(p.door_side, _snap((yc0 + yc1) / 2.0)))
        far = "E" if p.door_side == "W" else "W"
        n_s, n_n = p.rooms, max(p.rooms - 1, 1)
        for tag, n, y_lo, y_hi, wall_y, outer in (("s", n_s, WALL / 2, yc0 - WALL / 2, yc0, "S"), ("n", n_n, yc1 + WALL / 2, sy - WALL / 2, yc1, "N")):
            planes = _cuts(WALL / 2, sx - WALL / 2, n, p.seed_split + (0.1 if tag == "n" else 0.0))
            spans = _spans(WALL / 2, sx - WALL / 2, planes)
            doorways = []
            for i, span in enumerate(spans):
                room("%s%d" % (tag, i + 1), span[0], span[1], y_lo, y_hi, 0)
                c = _doorway_in(span)
                if c is None:
                    raise ValueError("%s: room too narrow for a doorway" % template.name)
                doorways.append((c, kit.DOOR_WIDTH))
                lights.append((_snap((span[0] + span[1]) / 2.0), _snap((y_lo + y_hi) / 2.0), 0))
            inner.append(kit.InnerWall("y", wall_y, WALL / 2, sx - WALL / 2, doorways=tuple(doorways)))
            for plane in planes:
                inner.append(kit.InnerWall("x", plane, y_lo, y_hi))
            windows(outer, planes, 0)
        room("corridor", WALL / 2, sx - WALL / 2, yc0 + WALL / 2, yc1 - WALL / 2, 0, narrow=True)
        lights.append((_snap(sx / 2.0), _snap((yc0 + yc1) / 2.0), 0))
        windows(far, [_snap((yc0 + yc1) / 2.0)], 0)
        # corridor walls were registered as one y wall per side; their doorway lists hold the room entrances
    elif template.layout == "hall":
        openings.append(door(p.door_side, 300.0))
        openings.append(kit.Opening("E", _snap(sy / 2), template.gate_width, 0.0, 330.0, 0))
        for side in ("S", "N"):
            windows(side, [], 0, high=True)
        n = max(int(sx // 450.0), 1)
        lights += [(_snap((k + 0.5) * sx / n), _snap(sy / 2), 0) for k in range(n)]
        room("hall", WALL / 2, sx - WALL / 2, WALL / 2, sy - WALL / 2)
    elif template.layout == "workshop":
        office = 400.0
        openings.append(door(p.door_side, 200.0))
        openings.append(kit.Opening("E", _snap(sy / 2), template.gate_width, 0.0, 260.0, 0))
        inner.append(kit.InnerWall("x", office, WALL / 2, sy - WALL / 2, doorways=((_snap(sy / 2), kit.DOOR_WIDTH),)))
        for side in ("S", "N"):
            windows(side, [office], 0)
        lights += [(_snap(office / 2), _snap(sy / 2), 0), (_snap(office + (sx - office) / 2), _snap(sy / 2), 0)]
        room("office", WALL / 2, office - WALL / 2, WALL / 2, sy - WALL / 2)
        room("bay", office + WALL / 2, sx - WALL / 2, WALL / 2, sy - WALL / 2)
    elif template.layout == "garage":
        openings.append(kit.Opening(p.door_side, _snap(sy / 2), template.gate_width, 0.0, 240.0, 0))
        openings.append(kit.Opening("N", _snap(sx / 2), 100.0, WINDOW_SILL, WINDOW_HEAD, 0))
        lights.append((_snap(sx / 2), _snap(sy / 2), 0))
        room("garage", WALL / 2, sx - WALL / 2, WALL / 2, sy - WALL / 2)
    else:
        raise ValueError("unknown layout " + template.layout)

    spec = kit.BuildingSpec(
        name=template.name, kind=template.kind, size=(sx, sy), floors=p.floors, floor_height=p.floor_height, roof=p.roof,
        openings=tuple(openings), inner_walls=tuple(inner), stair=multi, stair_width=120.0, lights=tuple(lights),
        rooms=tuple(rooms), stair_landing=p.stair_landing, stair_side=p.door_side if multi else "W")
    problem = kit.validate_spec(spec)
    if problem:
        raise ValueError("%s: %s" % (template.name, problem))
    return spec


def make_variants(template, count, seed):
    """`count` different valid Params from the template, deterministic for (template, seed). Invalid draws are skipped, never repaired."""
    rng = v1._Lcg(seed)
    out, tries = [], 0
    while len(out) < count and tries < count * 80:
        tries += 1

        def pick(options):
            return options[int(rng.next() * len(options)) % len(options)]

        def span(lo_hi):
            lo, hi = lo_hi
            n = int((hi - lo) // template.step)
            return lo + template.step * (int(rng.next() * (n + 1)) % (n + 1)) if hi > lo else lo

        p = Params(width=span(template.width), depth=span(template.depth), floors=pick(template.floors), door_side=pick(template.door_sides),
                   roof=pick(template.roofs), floor_height=pick(template.floor_heights),
                   hall_width=_snap(template.hall_width[0] + (template.hall_width[1] - template.hall_width[0]) * rng.next(), 10.0),
                   window_spacing=_snap(template.window_spacing[0] + (template.window_spacing[1] - template.window_spacing[0]) * rng.next(), 10.0),
                   door_id="d_%s_%d" % (template.name, len(out)), seed_split=round(0.35 + 0.3 * rng.next(), 2),
                   rooms=pick(tuple(range(template.rooms[0], template.rooms[1] + 1))),
                   stair_landing=bool(template.landing and rng.next() < 0.5))
        if validate_params(template, p) is None and p not in out:
            try:
                generate(template, p)
            except ValueError:
                continue
            out.append(p)
    return out
