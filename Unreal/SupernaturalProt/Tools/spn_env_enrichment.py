"""Environment enrichment layer for the V2 movement test map (M2A3 workstream K). OPT-IN: `build_layout_v2()` is unchanged (its snapshot hash is
pinned); `enrich(data)` adds props, street furniture, yards with gates, parked placeholder cars, road markings, vegetation clusters and a few
interactive test objects to an already built V2 layout, and switches it to its own level path (ENV_MAP) so the V2 map is never replaced.

Everything is built from the existing engine basic shapes and prototype materials (cube, cylinder, sphere) and is deterministic (seeded LCG).
Every placement is rejection-checked against the whole existing layout, the v1 test zones, door fronts, entrance paths and the sidewalk
clearance; the result can be re-validated with `validate_enrichment(data)` (also used by the tests). Nothing here is visually verified: that
needs the Unreal editor (Docs/Development/M2A3-Environment-Visual-Checklist.md).

Interactive objects (`data["env_objects"]`, `data["env_links"]`) are DATA for the Core interaction/persistence systems (ids, kinds, regions, authored
defaults, links). Doors among them are spawned as ASpnTestDoor by the existing generator; switches, buttons and levers get visible placeholder
geometry only - there is no Unreal actor for them yet (see the M2A3 handoff).
"""

import math

import spn_district_layout as D
import spn_movement_map_layout as L

ENV_MAP = "/Game/SpnTest/Lvl_SpnMovementTestV2Env"
ENV_VERSION = "m2a3-env1"
SEED = 20261028
REGION_CELL = 4000.0               # cm; the persistence region (streaming cell) of an object
SOLID_TOP = 20.0                   # cm: prims higher than this are obstacles for the placement rules
ZONE_MARGIN = 300.0                # cm kept free around every v1 test zone
FRONT_CLEAR = 350.0                # cm kept free in front of every entrance door
SIDEWALK_MIN_CLEAR = 100.0         # cm of sidewalk width left for the capsule (84 cm) beside any furniture
PRIM_BUDGET = 1300                 # cap on added primitives (actor budget; instancing groups the repeated ones)
ID_MAX = 96

_ZONE_PROP = {"bin": "d_env_street", "bench": "d_env_street", "sign": "d_env_street", "mailbox": "d_env_street", "mark": "d_env_marks",
              "drive": "d_env_yards", "fence": "d_env_yards", "car": "d_env_lots", "veg": "d_env_veg", "dev": "d_env_devices"}


# Every field that can hold persistent state needs an authored default (ApplyRecords skips a field without one and reports it as unknown, so the
# state would silently not come back). Per kind: field -> value type (bool / int).
STATE_FIELDS = {
    "placeholder:interactable.door": {"open": bool, "locked": bool, "disabled": bool},
    "placeholder:interactable.switch": {"on": bool, "disabled": bool},
    "placeholder:interactable.button": {"presses": int, "disabled": bool},
    "placeholder:interactable.lever": {"position": int, "disabled": bool},
}
_STATE_DEFAULTS = {bool: False, int: 0}


def with_state_defaults(kind, defaults):
    """The authored defaults plus a default for every state field the kind can persist (false / 0 unless the object says otherwise)."""
    full = {name: _STATE_DEFAULTS[t] for name, t in STATE_FIELDS[kind].items()}
    full.update(defaults)
    return full


def region_of(x, y):
    """Streaming cell id of a world position, e.g. cell_0_m2 (negative indices are written with an m prefix)."""
    def part(v):
        n = int(math.floor(v / REGION_CELL))
        return ("m%d" % -n) if n < 0 else "%d" % n
    return "cell_%s_%s" % (part(x), part(y))


def _rect_free(index, fp, margin=0.0):
    """Exact rectangle test against a D._FootprintIndex: True when no stored footprint comes within `margin` of fp (x0, x1, y0, y1)."""
    for i in index._range(fp[0] - margin, fp[1] + margin):
        for j in index._range(fp[2] - margin, fp[3] + margin):
            for x0, x1, y0, y1 in index.cells.get((i, j), ()):
                if fp[1] + margin > x0 and fp[0] - margin < x1 and fp[3] + margin > y0 and fp[2] - margin < y1:
                    return False
    return True


def _group(label):
    """Parts of one object share a label stem (Bin_003 and Bin_003_Lid are one bin)."""
    for suffix in ("_Lid", "_Plate", "_Post", "_Base", "_Seat", "_Head", "_Box", "_Crown", "_Trunk", "_Body", "_Cabin"):
        if label.endswith(suffix):
            return label[:-len(suffix)]
    return label


class _Env:
    """Placement context: the layout so far, the avoidance index, counters and the report."""

    def __init__(self, data, seed):
        self.data = data
        self.rng = L._Lcg(seed)
        self.new = L.Layout()
        self.new.lights = []
        self.road_cells, self.sidewalk_cells = D.ground_cells()
        base = data["prims"]
        ground = [p for p in base if _bottom(p) < OVERHEAD_Z]   # roofs and crowns above 2 m do not occupy the ground
        self.solid_fps = D._FootprintIndex(fp for fp in (D._prim_footprint(p) for p in ground if _top(p) > SOLID_TOP) if fp)
        self.all_fps = D._FootprintIndex(fp for fp in (D._prim_footprint(p) for p in ground if p["label"] not in ("SPN_D_Park_Lawn",)) if fp)
        self.zones = [(z[0] - ZONE_MARGIN, z[1] + ZONE_MARGIN, z[2] - ZONE_MARGIN, z[3] + ZONE_MARGIN) for z in L.ZONES.values()]
        self.fronts = [_front_rect(name) for name, *_ in D.BUILDINGS]
        self.fronts = [r for r in self.fronts if r]
        self.paths = [D._prim_footprint(p) for p in base if p["label"].startswith("SPN_D_Path_")]
        self.report = {"added": {}, "skipped": {}}

    def count(self, kind, n=1):
        self.report["added"][kind] = self.report["added"].get(kind, 0) + n

    def skip(self, kind, why):
        self.report["skipped"].setdefault(kind, []).append(why)

    def budget_left(self):
        return PRIM_BUDGET - len(self.new.prims)

    def in_zone(self, fp):
        return any(fp[1] > z[0] and fp[0] < z[1] and fp[3] > z[2] and fp[2] < z[3] for z in self.zones)

    def near_front(self, fp):
        return any(fp[1] > r[0] and fp[0] < r[1] and fp[3] > r[2] and fp[2] < r[3] for r in self.fronts)

    def on_sidewalk(self, x, y):
        return (int(math.floor(x / D.CELL)), int(math.floor(y / D.CELL))) in self.sidewalk_cells

    def on_road(self, x, y):
        return (int(math.floor(x / D.CELL)), int(math.floor(y / D.CELL))) in self.road_cells

    def free_solid(self, fp, margin=0.0):
        """No solid obstacle (prim higher than SOLID_TOP) within `margin` of the footprint."""
        return _rect_free(self.solid_fps, fp, margin)

    def free_all(self, fp, margin=0.0):
        """Nothing at all (roads, sidewalks, paths, lots included) within `margin` of the footprint."""
        return _rect_free(self.all_fps, fp, margin)

    def reserve(self, fp, solid=True):
        self.all_fps.add(fp)
        if solid:
            self.solid_fps.add(fp)


OVERHEAD_Z = 200.0   # cm: anything whose underside is higher than this is passed under (roofs, tree crowns)


def _bottom(p):
    return p["center"][2] - p["size"][2] / 2.0


def _top(p):
    return p["center"][2] + p["size"][2] / 2.0


def _front_rect(name):
    """Rectangle in front of an entrance door that must stay free of furniture (door swing, route front point)."""
    spec, (x0, y0) = D.building_spec(name)
    ent = next((o for o in spec.openings if o.door_id), None)
    if ent is None:
        return None
    y = y0 + ent.centre
    if ent.side == "W":
        return (x0 - FRONT_CLEAR, x0 + 50.0, y - 250.0, y + 250.0)
    return (x0 + spec.size[0] - 50.0, x0 + spec.size[0] + FRONT_CLEAR, y - 250.0, y + 250.0)


# --- props ------------------------------------------------------------------------------------------------------------------------

def _add(env, kind, name, shape, centre, size, rot=(0.0, 0.0, 0.0), material="grid_gray_02"):
    env.new.add("SPN_E_%s" % name, shape, centre, size, _ZONE_PROP[kind], rot=rot, material=material)


def _fp_box(cx, cy, sx, sy):
    return (cx - sx / 2.0, cx + sx / 2.0, cy - sy / 2.0, cy + sy / 2.0)


def _try_street_item(env, kind, idx, x, y, along_x):
    """One piece of street furniture centred at (x, y) on a sidewalk; rejected unless the whole footprint is on the sidewalk, clear of every
    solid, of door fronts, entrance paths and test zones, and leaves SIDEWALK_MIN_CLEAR of walkable width."""
    if kind == "bin":
        size = (50.0, 50.0)
    elif kind == "bench":
        size = (160.0, 50.0) if along_x else (50.0, 160.0)
    else:  # sign
        size = (24.0, 24.0)
    fp = _fp_box(x, y, size[0], size[1])
    corners = ((fp[0], fp[2]), (fp[0], fp[3]), (fp[1], fp[2]), (fp[1], fp[3]))
    if not all(env.on_sidewalk(cx, cy) for cx, cy in corners):
        return False
    if env.in_zone(fp) or env.near_front(fp) or not env.free_solid(fp, 40.0):
        return False
    if any(fp[1] > r[0] - 60.0 and fp[0] < r[1] + 60.0 and fp[3] > r[2] - 60.0 and fp[2] < r[3] + 60.0 for r in env.paths):
        return False
    if env.budget_left() < 4:
        return False
    z = D.SIDEWALK_TOP
    n = "%s_%03d" % (kind.capitalize(), idx)
    if kind == "bin":
        _add(env, "bin", n, "cylinder", (x, y, z + 45.0), (50.0, 50.0, 90.0), material="grid_topdark")
        _add(env, "bin", n + "_Lid", "cylinder", (x, y, z + 93.0), (54.0, 54.0, 6.0), material="grid_gray")
    elif kind == "bench":
        sx, sy = size
        _add(env, "bench", n + "_Seat", "cube", (x, y, z + 42.0), (sx, sy, 8.0))
        _add(env, "bench", n + "_Base", "cube", (x, y, z + 19.0), (sx - 20.0 if along_x else sx, sy if along_x else sy - 20.0, 38.0), material="grid_topdark")
    else:
        _add(env, "sign", n + "_Post", "cylinder", (x, y, z + 120.0), (8.0, 8.0, 240.0), material="grid_topdark")
        _add(env, "sign", n + "_Plate", "cube", (x, y, z + 215.0), (60.0, 4.0, 40.0) if along_x else (4.0, 60.0, 40.0), material="grid_gray")
    env.reserve(fp)
    env.count(kind)
    return True


def _street_furniture(env):
    idx = {"bin": 0, "bench": 0, "sign": 0}
    spacing = {"bin": 3800.0, "bench": 6200.0, "sign": 5000.0}
    phase = {"bin": 900.0, "bench": 2200.0, "sign": 1500.0}
    lateral = {"bin": D.ROAD_W / 2.0 + D.SIDEWALK_W - 50.0, "bench": D.ROAD_W / 2.0 + D.SIDEWALK_W - 55.0, "sign": D.ROAD_W / 2.0 + 25.0}
    for name, ax, ay, bx, by in D.ROADS:
        along_x = abs(bx - ax) >= abs(by - ay)
        lo, hi = (min(ax, bx), max(ax, bx)) if along_x else (min(ay, by), max(ay, by))
        for kind in ("bin", "bench", "sign"):
            t = lo + phase[kind]
            while t < hi:
                for side in (-1.0, 1.0):
                    x, y = (t, ay + side * lateral[kind]) if along_x else (ax + side * lateral[kind], t)
                    if env.budget_left() < 4:
                        break
                    if _try_street_item(env, kind, idx[kind], x, y, along_x):
                        idx[kind] += 1
                    # alternate the sides a little: benches on one side only
                    if kind == "bench":
                        break
                t += spacing[kind] + (env.rng.uniform(-300.0, 300.0))


def _mailboxes(env):
    n = 0
    for p in list(env.data["prims"]):
        if not p["label"].startswith("SPN_D_Path_"):
            continue
        fp = D._prim_footprint(p)
        y = p["center"][1]
        # at the outer end of the path (the end that is not at the building), 100 cm beside the path line, 25 cm into the sidewalk
        for end_x in (fp[0], fp[1]):
            for dy in (300.0, -300.0):
                x = end_x - 40.0 if end_x == fp[0] else end_x + 40.0
                box = _fp_box(x, y + dy, 40.0, 24.0)
                if not (env.on_sidewalk(x, y + dy) and env.free_solid(box, 30.0) and not env.in_zone(box) and not env.near_front(box) and env.budget_left() >= 4):
                    continue
                if not all(env.on_sidewalk(cx, cy) for cx, cy in ((box[0], box[2]), (box[1], box[3]))):
                    continue
                z = D.SIDEWALK_TOP
                label = p["label"].split("SPN_D_Path_")[1]
                _add(env, "mailbox", "Mailbox_%s_Post" % label, "cylinder", (x, y + dy, z + 50.0), (10.0, 10.0, 100.0), material="grid_topdark")
                _add(env, "mailbox", "Mailbox_%s_Box" % label, "cube", (x, y + dy, z + 112.0), (40.0, 24.0, 26.0), material="grid_gray")
                env.reserve(box)
                env.count("mailbox")
                n += 1
                break
            else:
                continue
            break
    return n


def _road_markings(env):
    """Dashed centre lines on the straight roads, 15 cm wide, 300 cm long every 600 cm, only where this road is alone (no junction)."""
    rects = D.road_rects()
    n = 0
    for ri, (name, ax, ay, bx, by) in enumerate(D.ROADS):
        along_x = abs(bx - ax) >= abs(by - ay)
        lo, hi = (min(ax, bx), max(ax, bx)) if along_x else (min(ay, by), max(ay, by))
        t = lo + 700.0
        while t < hi - 700.0:
            x, y = (t, ay) if along_x else (ax, t)
            size = (300.0, 15.0) if along_x else (15.0, 300.0)
            fp = _fp_box(x, y, size[0], size[1])
            others = [r for k, r in enumerate(rects) if k != ri]
            alone = not any(fp[1] > r[0] - 100.0 and fp[0] < r[1] + 100.0 and fp[3] > r[2] - 100.0 and fp[2] < r[3] + 100.0 for r in others)
            if alone and env.budget_left() > 60 and not env.in_zone(fp):
                _add(env, "mark", "Dash_%02d_%03d" % (ri, n), "cube", (x, y, D.ROAD_TOP + 0.25), (size[0], size[1], 0.5), material="grid_gray")
                env.count("road_mark")
                n += 1
            t += 600.0


def _driveways(env):
    for p in list(env.data["prims"]):
        if not p["label"].startswith("SPN_D_Path_"):
            continue
        fp = D._prim_footprint(p)
        label = p["label"].split("SPN_D_Path_")[1]
        if label.startswith(("StorageHall", "Workshop", "Office")):
            continue
        for dy in (330.0, -330.0):
            box = (fp[0] + 30.0, fp[1] - 30.0, p["center"][1] + dy - 150.0, p["center"][1] + dy + 150.0)   # 30 cm clear of the wall and the kerb
            if env.free_all(box, 20.0) and not env.in_zone(box) and env.budget_left() > 4:
                env.new.add("SPN_E_Drive_%s" % label, "cube", ((box[0] + box[1]) / 2.0, (box[2] + box[3]) / 2.0, 1.0), (box[1] - box[0], box[3] - box[2], 2.0), "d_env_yards", material="grid_topdark")
                env.reserve(box, solid=False)
                env.count("driveway")
                break
        else:
            env.skip("driveway", label)


def _parked_cars(env):
    """Placeholder cars (body + cabin boxes) in the bays of the lots, away from the lot centre (a route target) and from each other."""
    n = 0
    for name, x0, x1, y0, y1 in D.LOTS:
        cx, cy = (x0 + x1) / 2.0, (y0 + y1) / 2.0
        long_x = (x1 - x0) >= (y1 - y0)
        picks = (1, 3, 6) if long_x else (1, 3)
        for k in picks:
            if long_x:
                x, y = x0 + k * 250.0 + 125.0, y0 + 250.0 + 40.0
                size = (180.0, 440.0)
                rot = (0.0, 0.0, 0.0)
            else:
                x, y = x0 + 250.0 + 40.0, y0 + k * 250.0 + 125.0
                size = (440.0, 180.0)
                rot = (0.0, 0.0, 0.0)
            fp = _fp_box(x, y, size[0], size[1])
            if math.hypot(x - cx, y - cy) < 500.0 or not (x0 <= fp[0] and fp[1] <= x1 and y0 <= fp[2] and fp[3] <= y1) or not env.free_solid(fp, 30.0) or env.budget_left() < 6:
                env.skip("car", "%s bay %d" % (name, k))
                continue
            tag = "%s_%d" % (name.replace(" ", ""), k)
            body = (size[0], size[1], 70.0)
            cabin = (size[0] - 20.0, size[1] * 0.5, 55.0) if long_x else (size[0] * 0.5, size[1] - 20.0, 55.0)
            env.new.add("SPN_E_Car_%s_Body" % tag, "cube", (x, y, LOT_Z + 35.0 + 18.0), body, "d_env_lots", rot=rot, material="grid_gray_02")
            env.new.add("SPN_E_Car_%s_Cabin" % tag, "cube", (x, y - (30.0 if long_x else 0.0), LOT_Z + 70.0 + 18.0 + 27.0), cabin, "d_env_lots", rot=rot, material="grid_gray")
            env.reserve(fp)
            env.count("car")
            n += 1


LOT_Z = D.LOT_TOP


COMPOUNDS = (("compound_a", "button"), ("compound_b", "switch"))


# --- yards, gates and interactive objects ------------------------------------------------------------------------------------------

YARD_CANDIDATES = (  # (building, depth behind the building in cm, device kind); a yard that does not fit is skipped and reported
    ("House1", 800.0, "button"),
    ("Office1", 600.0, "lever"),
    ("Cottage2", 600.0, "switch"),
    ("Cottage1", 600.0, "button"),
)


def _yard(env, building, depth, device_kind):
    """Tries the yard at its nominal depth, then at 75 % and 55 % (never below 350 cm); skips it as a whole when none fits."""
    reason = "no depth tried"
    for d in (depth, depth * 0.75, depth * 0.55):
        if d < 350.0:
            break
        for margin in (250.0, 150.0, 60.0):   # how far the fence reaches beyond the building along its side
            reason = _try_yard(env, building, d, device_kind, margin)
            if reason is None:
                return
            if reason.startswith("not a west"):
                env.skip("yard", "%s: %s" % (building, reason))
                return
    env.skip("yard", "%s: %s" % (building, reason))


def _try_yard(env, building, depth, device_kind, margin=250.0):
    spec, (ox, oy) = D.building_spec(building)
    entrance = next((o for o in spec.openings if o.door_id), None)
    if entrance is None or entrance.side not in ("W", "E"):
        return "not a west or east entrance"
    sign = 1.0 if entrance.side == "W" else -1.0          # the yard lies BEHIND the building: east of a west entrance, west of an east one
    bx0, bx1 = ox, ox + spec.size[0]
    if sign > 0:
        x0, x1 = bx1 + 100.0, bx1 + depth
    else:
        x0, x1 = bx0 - depth, bx0 - 100.0
    return _enclosure(env, building.lower(), x0, x1, oy - margin, oy + spec.size[1] + margin, sign, device_kind)


def _enclosure(env, name, x0, x1, y0, y1, sign, device_kind):
    """A fence rectangle with a gate (an ASpnTestDoor) on its far side (east for sign > 0) and a device outside the gate that unlocks it. The whole
    thing is placed or nothing is: boards, gate swing and device must be free. Returns None on success, else the reason."""
    far = x1 if sign > 0 else x0
    building = name
    gate_y = (y0 + y1) / 2.0
    gap_side = "E" if sign > 0 else "W"
    tmp = L.Layout()
    D._fence(tmp, "Yard_" + name, x0, x1, y0, y1, gaps=((gap_side, gate_y - 65.0, gate_y + 65.0),), height=110.0, thickness=8.0)
    fps = [D._prim_footprint(p) for p in tmp.prims]
    post_x, post_y = far + sign * 90.0, gate_y + 110.0
    dev_fp = _fp_box(post_x, post_y, 60.0, 60.0)
    gate_fp = (far - 10.0, far + 110.0, gate_y - 60.0, gate_y + 60.0) if sign > 0 else (far - 110.0, far + 10.0, gate_y - 60.0, gate_y + 60.0)
    if len(tmp.prims) + 6 > env.budget_left():
        return "budget"
    for fp in fps + [dev_fp]:
        if not env.free_all(fp, 15.0) or env.in_zone(fp) or env.near_front(fp):
            return "blocked"
    if not env.free_solid(gate_fp, 10.0):
        return "gate blocked"
    for p in tmp.prims:
        p["label"] = p["label"].replace("SPN_D_Fence_", "SPN_E_Fence_")
        p["zone"] = "d_env_yards"
        env.new.prims.append(p)
    for fp in fps:
        env.reserve(fp)
    env.count("yard")
    env.count("fence_board", len(tmp.prims))
    gate_id = "env_%s_gate" % name
    env.new.door(gate_id, far, gate_y, "d_env_yards")
    env.data["doors"].append(env.new.doors[-1])
    env.data["doors"][-1]["inward"] = -sign   # a pedestrian enters the enclosure against the direction of the gate-to-yard axis
    env.data["doors"][-1]["open_angle"] = 90.0 if sign > 0 else -90.0   # outward: away from the yard, the leaf stands on the device side only 50 cm off the gate line
    env.objects.append({"id": gate_id, "kind": "placeholder:interactable.door", "region": region_of(far, gate_y), "centre_cm": [far, gate_y, 100.0],
                        "radius_cm": 60.0, "priority": 1, "cooldown_ms": 0,
                        "defaults": with_state_defaults("placeholder:interactable.door", {"locked": True})})
    env.reserve(gate_fp, solid=False)
    dev_id = "env_%s_%s" % (name, device_kind)
    env.new.add("SPN_E_Dev_%s_Post" % dev_id, "cylinder", (post_x, post_y, 55.0), (12.0, 12.0, 110.0), "d_env_devices", material="grid_topdark")
    env.new.add("SPN_E_Dev_%s_Head" % dev_id, "cube", (post_x, post_y, 122.0), (34.0, 14.0, 40.0), "d_env_devices", material="colorway")
    env.reserve(dev_fp)
    env.count("device")
    defaults = {"positions": 3} if device_kind == "lever" else ({"max_presses": 0} if device_kind == "button" else {})   # configuration; state fields get theirs below
    env.objects.append({"id": dev_id, "kind": "placeholder:interactable." + device_kind, "region": region_of(post_x, post_y), "centre_cm": [post_x, post_y, 120.0],
                        "radius_cm": 30.0, "priority": 2, "cooldown_ms": 400 if device_kind == "button" else 0,
                        "defaults": with_state_defaults("placeholder:interactable." + device_kind, defaults)})
    when = None if device_kind == "button" else (2 if device_kind == "lever" else 1)
    env.links.append({"source": dev_id, "when_state": when, "target": gate_id, "target_key": "placeholder.door.%s.locked" % gate_id, "value": False,
                      "otherwise": None if device_kind == "button" else True})
    steps = [{"do": "interact", "target": gate_id, "expect": "Locked"}, {"do": "interact", "target": dev_id, "expect": "Success"}]
    if device_kind == "lever":
        steps.append({"do": "interact", "target": dev_id, "expect": "Success"})   # positions 0 -> 1 -> 2: the second press reaches the trigger state
    steps.append({"do": "interact", "target": gate_id, "expect": "Success"})
    env.scenarios.append({"id": "%s_unlocks_gate_%s" % (device_kind, name), "title": "%s outside the %s enclosure unlocks its gate" % (device_kind, name),
                          "objects": [dev_id, gate_id], "steps": steps, "status": "PENDING_UNREAL_ADAPTER"})
    return None


def _compounds(env, specs):
    """Free-standing fenced compounds (storage or utility yards) on free ground: the first free 700 x 500 cm spot of a deterministic scan order,
    so the gate scenarios exist even where the district leaves no room behind a building."""
    cells = [(x, y) for x in range(-9000, 13000, 800) for y in range(-12500, 12800, 800)]
    for i in range(len(cells) - 1, 0, -1):   # seeded shuffle
        j = int(env.rng.uniform(0.0, i + 0.999))
        cells[i], cells[j] = cells[j], cells[i]
    for name, kind in specs:
        for x, y in cells:
            if _enclosure(env, name, float(x), float(x) + 700.0, float(y), float(y) + 500.0, 1.0, kind) is None:
                break
        else:
            env.skip("yard", "%s: no free spot" % name)


# --- vegetation clusters ---------------------------------------------------------------------------------------------------------

def _clusters(env, count=34):
    placed_trees = 0
    xlo, xhi, ylo, yhi = -9800.0, 13800.0, -13300.0, 13300.0
    for c in range(count * 12):
        if len([1 for p in env.new.prims if p["label"].startswith("SPN_E_Cluster")]) >= count * 6 or env.budget_left() < 40 or c >= count * 12:
            break
        cx, cy = env.rng.uniform(xlo, xhi), env.rng.uniform(ylo, yhi)
        cluster = _fp_box(cx, cy, 900.0, 900.0)
        if env.in_zone(cluster) or not env.free_all(cluster, 100.0):
            continue
        idx = c
        # ground variation: a flat colour-way patch, then 3-6 trees and bushes of varying size around the centre
        env.new.add("SPN_E_Cluster_%03d_Ground" % idx, "cylinder", (cx, cy, 1.0), (700.0 + env.rng.uniform(0.0, 300.0), 700.0, 2.0), "d_env_veg", material="colorway")
        env.reserve(cluster, solid=False)
        env.count("ground_patch")
        for k in range(int(3 + env.rng.uniform(0.0, 4.0))):
            tree = bool(k % 3)
            h, crown = env.rng.uniform(250.0, 420.0), env.rng.uniform(220.0, 360.0)
            d = env.rng.uniform(70.0, 130.0)
            r = crown / 2.0 if tree else d / 2.0          # the footprint that is really built (crown / bush diameter)
            for _ in range(8):
                x, y = cx + env.rng.uniform(-330.0, 330.0), cy + env.rng.uniform(-330.0, 330.0)
                fp = _fp_box(x, y, 2 * r, 2 * r)
                if not env.free_solid(fp, 30.0):
                    continue
                if tree:
                    env.new.add("SPN_E_Cluster_%03d_Tree%d_Trunk" % (idx, k), "cylinder", (x, y, h / 2.0), (34.0, 34.0, h), "d_env_veg", material="grid_topdark")
                    env.new.add("SPN_E_Cluster_%03d_Tree%d_Crown" % (idx, k), "sphere", (x, y, h + crown * 0.25), (crown, crown, crown), "d_env_veg", material="colorway")
                    placed_trees += 1
                else:
                    env.new.add("SPN_E_Cluster_%03d_Bush%d" % (idx, k), "sphere", (x, y, d * 0.1), (d, d, d), "d_env_veg", material="colorway")
                env.reserve(fp)
                break
    env.count("tree", placed_trees)


# --- assembly -----------------------------------------------------------------------------------------------------------------------

def enrich(data, seed=SEED):
    """Adds the enrichment layer to a V2 layout dict IN PLACE and returns the report. Refuses anything that is not the plain V2 layout."""
    if data.get("map") != D.MAP_V2 or "district" not in data or "env_objects" in data:
        raise ValueError("enrich() needs the plain V2 layout (once)")
    env = _Env(data, seed)
    env.objects, env.links, env.scenarios = [], [], []
    _road_markings(env)
    _street_furniture(env)
    _mailboxes(env)
    _driveways(env)
    _parked_cars(env)
    for building, depth, kind in YARD_CANDIDATES:
        _yard(env, building, depth, kind)
    _compounds(env, COMPOUNDS)
    _clusters(env)
    data["prims"] = data["prims"] + env.new.prims
    data["map"] = ENV_MAP
    data["generator_version"] = D.GENERATOR_VERSION + "+" + ENV_VERSION
    data["env_objects"] = sorted(env.objects, key=lambda o: o["id"])
    data["env_links"] = env.links
    data["env_scenarios"] = env.scenarios
    data["env_report"] = {"version": ENV_VERSION, "seed": seed, "added_prims": len(env.new.prims), "added": dict(sorted(env.report["added"].items())),
                          "skipped": {k: sorted(v) for k, v in sorted(env.report["skipped"].items())}}
    return data["env_report"]


def build_layout_v2_enriched(seed=SEED):
    data = D.build_layout_v2()
    enrich(data, seed)
    return data


# --- validation ---------------------------------------------------------------------------------------------------------------------

def _valid_id(text):
    return bool(text) and len(text) <= ID_MAX and text[0].islower() and all(c.islower() or c.isdigit() or c in "_." for c in text)


def validate_enrichment(data):
    """Problems of an enriched layout as short strings (empty = valid): budget, unique labels, ids/regions/links of the interactive objects,
    solid props never overlapping another solid, nothing inside a v1 test zone, door fronts or the road surface, and every named route still
    reachable (the same pass the plain V2 map has to pass)."""
    import spn_map_validate as V
    problems = list(V.validate_layout_data(data))
    base_count = len(D.build_layout_v2()["prims"])
    new = data["prims"][base_count:]
    if len(new) > PRIM_BUDGET:
        problems.append("%d added primitives exceed the budget %d" % (len(new), PRIM_BUDGET))
    env_labels = [p["label"] for p in new]
    if any(not l.startswith("SPN_E_") for l in env_labels):
        problems.append("added primitive without the SPN_E_ prefix")
    ids = set()
    kinds = {"placeholder:interactable.door", "placeholder:interactable.switch", "placeholder:interactable.button", "placeholder:interactable.lever"}
    for o in data.get("env_objects", []):
        if not _valid_id(o["id"]) or o["id"] in ids:
            problems.append("object id invalid or duplicate: %s" % o["id"])
        ids.add(o["id"])
        if o["kind"] not in kinds:
            problems.append("%s: unknown kind %s" % (o["id"], o["kind"]))
        if o["region"] != region_of(o["centre_cm"][0], o["centre_cm"][1]):
            problems.append("%s: region does not match its position" % o["id"])
    for o in data.get("env_objects", []):
        for name, t in STATE_FIELDS.get(o["kind"], {}).items():
            if name not in o["defaults"] or type(o["defaults"][name]) is not t:
                problems.append("%s: state field %s has no authored default of type %s (it would not be restored)" % (o["id"], name, t.__name__))
    for l in data.get("env_links", []):
        if l["source"] not in ids or l["target"] not in ids or l["source"] == l["target"]:
            problems.append("link %s>%s: unknown object or self link" % (l["source"], l["target"]))
        elif not l["target_key"].startswith("placeholder.%s.%s." % (next(o["kind"] for o in data["env_objects"] if o["id"] == l["target"]).rsplit(".", 1)[1], l["target"])):
            problems.append("link %s>%s writes a key of another object" % (l["source"], l["target"]))
    door_ids = {d["id"] for d in data["doors"]}
    for o in data.get("env_objects", []):
        if o["kind"].endswith(".door") and o["id"] not in door_ids:
            problems.append("door object %s has no door actor entry" % o["id"])
    # geometric rules
    solids = [(p, D._prim_footprint(p)) for p in new if _top(p) > SOLID_TOP and D._prim_footprint(p)]
    zones = list(L.ZONES.values())
    fronts = [r for r in (_front_rect(name) for name, *_ in D.BUILDINGS) if r]
    base_solids = D._FootprintIndex(fp for fp in (D._prim_footprint(p) for p in data["prims"][:base_count] if _top(p) > SOLID_TOP and _bottom(p) < OVERHEAD_Z) if fp)
    groups = {}
    for p, fp in solids:
        groups.setdefault(_group(p["label"]), []).append((p, fp))
    own = {}   # group -> union footprint of all its parts
    for g, parts in groups.items():
        for p, fp in parts:
            if not _rect_free(base_solids, (fp[0] + 1.0, fp[1] - 1.0, fp[2] + 1.0, fp[3] - 1.0)):
                problems.append("%s overlaps an existing solid" % p["label"])
            if any(fp[1] > z[0] and fp[0] < z[1] and fp[3] > z[2] and fp[2] < z[3] for z in zones):
                problems.append("%s stands in a v1 test zone" % p["label"])
            if any(fp[1] > f[0] and fp[0] < f[1] and fp[3] > f[2] and fp[2] < f[3] for f in fronts):
                problems.append("%s blocks an entrance front" % p["label"])
        own[g] = (min(fp[0] for _, fp in parts), max(fp[1] for _, fp in parts), min(fp[2] for _, fp in parts), max(fp[3] for _, fp in parts))
    keys = sorted(g for g in own if "Fence_" not in g and "Cluster" not in g)   # fence boards touch at corners by design; cluster trees are checked per tree below
    index = D._FootprintIndex()
    names = {}
    for g in keys:
        fp = own[g]
        shrunk = (fp[0] + 1.0, fp[1] - 1.0, fp[2] + 1.0, fp[3] - 1.0)
        if not _rect_free(index, shrunk):
            problems.append("%s overlaps another new object" % g)
        index.add(fp)
    tree_index = D._FootprintIndex()
    for g in sorted(own):
        if "Cluster" in g and "_Ground" not in g:
            fp = own[g]
            if not _rect_free(tree_index, (fp[0] + 1.0, fp[1] - 1.0, fp[2] + 1.0, fp[3] - 1.0)):
                problems.append("%s overlaps another tree or bush" % g)
            tree_index.add(fp)
    gates = [d for d in data["doors"] if d["id"].startswith("env_")]
    problems += ["gate %s" % p for p in V.validate_door_passages({"prims": data["prims"], "doors": gates})]
    grid = V.MapGrid(data)
    for name, ok, detail in D.check_routes(data, grid):
        if not ok:
            problems.append("route %s unreachable after the enrichment (%s)" % (name, detail))
    return problems


# --- Core fixture ---------------------------------------------------------------------------------------------------------------------

def core_fixture(data):
    """Plain-text export of the interactive objects, links and scenarios for the C++ Core test (Tests/Core/EnvScenarioTests.cpp), which runs every
    scenario against the real interaction system. One record per line, tokens separated by one space, no escaping needed (ids and numbers only):
      object <id> <kind> <region> <x_cm> <y_cm> <z_cm> <radius_cm> <priority> <cooldown_ms>
      default <id> <name> <b|i> <value>
      link <source> <when|-> <target> <key> <b|i> <value> <otherwise_b|i value|->
      step <scenario> <target> <expect>
    The committed copy is Tests/Core/Data/env_scenarios.txt; a Python test fails when it differs from this output."""
    lines = ["# generated by spn_env_enrichment.core_fixture(); do not edit. Regenerate: python3 Unreal/SupernaturalProt/Tools/spn_env_enrichment.py --write-fixture Tests/Core/Data/env_scenarios.txt"]

    def value(v):
        return ("b %d" % (1 if v else 0)) if isinstance(v, bool) else ("i %d" % v)
    for o in data["env_objects"]:
        lines.append("object %s %s %s %g %g %g %g %d %d" % (o["id"], o["kind"], o["region"], o["centre_cm"][0], o["centre_cm"][1], o["centre_cm"][2], o["radius_cm"], o["priority"], o["cooldown_ms"]))
        for name, v in sorted(o["defaults"].items()):
            lines.append("default %s %s %s" % (o["id"], name, value(v)))
    for l in data["env_links"]:
        other = "-" if l["otherwise"] is None else value(l["otherwise"])
        lines.append("link %s %s %s %s %s %s" % (l["source"], "-" if l["when_state"] is None else l["when_state"], l["target"], l["target_key"], value(l["value"]), other))
    for sc in data["env_scenarios"]:
        for st in sc["steps"]:
            lines.append("step %s %s %s" % (sc["id"], st["target"], st["expect"]))
    return "\n".join(lines) + "\n"


if __name__ == "__main__":
    import sys
    if len(sys.argv) == 3 and sys.argv[1] == "--write-fixture":
        with open(sys.argv[2], "w", encoding="utf-8", newline="\n") as handle:
            handle.write(core_fixture(build_layout_v2_enriched()))
        print("wrote " + sys.argv[2])
    else:
        report = build_layout_v2_enriched()["env_report"]
        print(report)
