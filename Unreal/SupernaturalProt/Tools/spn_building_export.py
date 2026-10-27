"""Export of a generated building for an engine-side builder (schema version 2).

Layers in the exported document (keys): "spec" (the building description), "topology" (rooms, openings, connections), "geometry"
(modules with transform in cm), "collision" (per module: role, collision/walkable flags), "materials" (material roles), "slots"
(asset slots and their defaults), "unreal" (units, axes, placement). Version-1 documents (phase 1/2: no "schema_version") had the
top-level keys name, kind, size, floors, origin, rooms, openings, navigation, modules, lights; they are all still present with the same
meaning, so a v1 reader keeps working; v2 only adds keys (plus "slot" in every module).

Coordinates: the layout coordinate system IS the Unreal world system (X east/forward, Y north/right, Z up, centimetres); nothing is
converted. Placement rotates the whole building about its SW corner by a multiple of 90 degrees (Unreal yaw: positive turns +X
towards +Y), applied after the module's own pitch/roll (FRotator composes yaw last: UNCONFIRMED against the engine).

Pure Python, no Unreal import.
"""

import math

import spn_building_kit as kit
import spn_building_roles as R

SCHEMA_VERSION = 2

# Reusable asset slots. "default_mesh" is an engine shape key today; a later asset pass maps a slot to a modular mesh, e.g.
# "outer_wall": "/Game/Assets/Buildings/SM_Wall_300" - the generation and the checks do not change.
ASSET_SLOTS = {
    "outer_wall": {"role": "wall", "material_role": "wall_exterior", "default_mesh": "cube"},
    "inner_wall": {"role": "wall", "material_role": "wall_interior", "default_mesh": "cube"},
    "floor": {"role": "floor", "material_role": "floor", "default_mesh": "cube"},
    "ceiling": {"role": "slab", "material_role": "ceiling", "default_mesh": "cube"},
    "roof": {"role": "roof", "material_role": "roof", "default_mesh": "cube"},
    "window": {"role": "opening", "material_role": "glass_frame", "default_mesh": None},
    "door_frame": {"role": "opening", "material_role": "trim", "default_mesh": None},
    "door_leaf": {"role": "door", "material_role": "door", "default_mesh": None},
    "stair": {"role": "stair", "material_role": "stair", "default_mesh": "cube"},
    "railing": {"role": "rail", "material_role": "railing", "default_mesh": "cube"},
}

# material role -> project material key (spn_movement_map_layout.MATERIAL_PATHS)
MATERIAL_ROLES = {
    "wall_exterior": "grid_gray_02", "wall_interior": "grid_gray_02", "floor": "grid_gray", "ceiling": "grid_gray", "roof": "grid_gray",
    "glass_frame": "grid_topdark", "trim": "grid_topdark", "door": "grid_topdark", "stair": "grid_gray", "railing": "grid_topdark",
}

_ROLE_SLOT = {"floor": "floor", "slab": "floor", "roof": "roof", "stair": "stair", "rail": "railing"}


class ExportError(ValueError):
    """The building or the produced document is not valid; `problems` lists why."""

    def __init__(self, problems):
        super().__init__("; ".join(problems[:5]) + (" ..." if len(problems) > 5 else ""))
        self.problems = list(problems)


def slot_of(label, role):
    if role == "wall":
        return "inner_wall" if "_In" in label.split("_F", 1)[-1] else "outer_wall"
    return _ROLE_SLOT[role]


def _rotate_point(x, y, yaw):
    c, s = {0: (1, 0), 90: (0, 1), 180: (-1, 0), 270: (0, -1)}[yaw]
    return x * c - y * s, x * s + y * c


def _place(center, size, rot, origin, yaw):
    x, y = _rotate_point(center[0], center[1], yaw)
    sx, sy, sz = (size[1], size[0], size[2]) if yaw in (90, 270) else size
    return [round(x + origin[0], 3), round(y + origin[1], 3), center[2]], [sx, sy, sz], [rot[0], round((rot[1] + yaw) % 360.0, 6), rot[2]]


def export_building(spec, origin=(0.0, 0.0), yaw=0, building_id="building", strict=True):
    """The v2 document of `spec` placed at `origin` (cm) and turned by `yaw` (0/90/180/270 degrees about its SW corner). With
    strict=True (default) a building that fails the geometric checks raises ExportError; the produced document is always validated."""
    if yaw not in (0, 90, 180, 270):
        raise ExportError(["yaw must be 0, 90, 180 or 270 degrees"])
    if strict:
        codes = R.check_codes(spec)
        if codes:
            raise ExportError(["%s: %s" % c for c in codes])
    layout = R.build_standalone(spec)
    modules = []
    for p in layout.prims:
        role = R.role_of(p["label"])
        slot = slot_of(p["label"], role)
        center, size, rot = _place(p["center"], p["size"], p["rot"], origin, yaw)
        m = {"label": p["label"], "role": role, "slot": slot, "collision": True, "walkable": role in R.WALKABLE_ROLES,
             "mesh": ASSET_SLOTS[slot]["default_mesh"], "material": MATERIAL_ROLES[ASSET_SLOTS[slot]["material_role"]],
             "material_role": ASSET_SLOTS[slot]["material_role"], "center": center, "size": size, "rot": rot}
        if role == "slab":
            m["also_ceiling_of_floor"] = int(p["label"].split("_Slab")[1].split("_")[0]) - 1
        modules.append(m)
    doors, windows = [], []
    for d in layout.doors:
        hinge = _place([d["hinge"][0], d["hinge"][1], 0.0], [0, 0, 0], [0, 0, 0], origin, yaw)[0]
        doors.append({"id": d["id"], "slots": {"frame": "door_frame", "leaf": "door_leaf"}, "hinge": hinge, "yaw": round((d["yaw"] + yaw) % 360.0, 6)})
    for o in spec.openings:
        if o.door_id or o.sill <= 0.0:
            continue
        sx, sy = spec.size
        local = {"W": (0.0, o.centre), "E": (sx, o.centre), "S": (o.centre, 0.0), "N": (o.centre, sy)}[o.side]
        z0 = kit.floor_top(spec, o.floor) + o.sill
        center, _, _ = _place([local[0], local[1], z0 + (o.head - o.sill) / 2.0], [1, 1, 1], [0, 0, 0], origin, yaw)
        windows.append({"slot": "window", "side": o.side, "floor": o.floor, "center": center, "width": o.width, "height": o.head - o.sill, "sill": o.sill})
    rooms = [{"name": r.name, "floor": r.floor, "rect": [r.x0, r.x1, r.y0, r.y1]} for r in spec.rooms]
    openings = [{"side": o.side, "centre": o.centre, "width": o.width, "sill": o.sill, "head": o.head, "floor": o.floor, "door_id": o.door_id} for o in spec.openings]
    nav = R.navigation_rects(spec)
    graph = R.room_graph(spec)
    doc = {
        "schema_version": SCHEMA_VERSION, "id": building_id,
        "spec": {"name": spec.name, "kind": spec.kind, "size": list(spec.size), "floors": spec.floors, "floor_height": spec.floor_height,
                 "wall_thickness": spec.wall_thickness, "roof": spec.roof, "stair": spec.stair, "stair_side": spec.stair_side if spec.stair else None,
                 "stair_landing": spec.stair_landing},
        "topology": {"rooms": rooms, "openings": openings, "edges": [list(e) for e in graph["edges"]], "nodes": graph["nodes"]},
        "geometry": {"count": len(modules)},
        "collision": {"roles": sorted({m["role"] for m in modules}), "all_collide": True},
        "materials": dict(MATERIAL_ROLES),
        "slots": {k: dict(v) for k, v in ASSET_SLOTS.items()},
        "unreal": {"units": "cm", "axes": "X east/forward, Y north/right, Z up (same as the layout)", "origin": [origin[0], origin[1]], "yaw_deg": yaw, "doors": doors, "windows": windows},
        # --- version-1 keys, unchanged meaning ---
        "name": spec.name, "kind": spec.kind, "size": list(spec.size), "floors": spec.floors, "origin": [origin[0], origin[1]],
        "rooms": rooms, "openings": openings, "navigation": nav, "modules": modules, "lights": [list(l) for l in spec.lights],
    }
    problems = validate_export(doc)
    if problems:
        raise ExportError(problems)
    return doc


def validate_export(doc):
    """Problems of a v2 document as short strings (empty = valid): schema, finite positive transforms, known roles and slots, unique
    labels, counts, yaw, rooms/navigation inside the footprint, doors that refer to existing openings."""
    problems = []
    if doc.get("schema_version") != SCHEMA_VERSION:
        return ["schema_version must be %d" % SCHEMA_VERSION]
    modules = doc.get("modules") or []
    if not modules:
        problems.append("no modules")
    if doc["geometry"]["count"] != len(modules):
        problems.append("geometry.count does not match the modules")
    seen = set()
    for m in modules:
        label = m.get("label", "?")
        if label in seen:
            problems.append("duplicate module label " + label)
        seen.add(label)
        if m.get("slot") not in ASSET_SLOTS or m.get("role") not in set(R.WALKABLE_ROLES) | {"wall", "roof", "rail"}:
            problems.append("unknown role/slot in " + label)
        for key in ("center", "size", "rot"):
            v = m.get(key)
            if not v or len(v) != 3 or not all(isinstance(x, (int, float)) and math.isfinite(x) for x in v):
                problems.append("%s: %s is not three finite numbers" % (label, key))
        if m.get("size") and not all(x > 0 for x in m["size"]):
            problems.append("%s: non-positive size" % label)
        if m.get("material") not in MATERIAL_ROLES.values():
            problems.append("%s: unknown material" % label)
    yaw = doc["unreal"]["yaw_deg"]
    if yaw not in (0, 90, 180, 270):
        problems.append("yaw is not a multiple of 90")
    sx, sy = doc["size"]
    for r in doc["rooms"]:
        x0, x1, y0, y1 = r["rect"]
        if not (0 <= x0 < x1 <= sx and 0 <= y0 < y1 <= sy):
            problems.append("room %s outside the footprint" % r["name"])
    for n in doc["navigation"]:
        x0, x1, y0, y1 = n["rect"]
        if not (0 <= x0 < x1 <= sx and 0 <= y0 < y1 <= sy):
            problems.append("navigation rectangle %s outside the footprint" % n["name"])
    ids = {o["door_id"] for o in doc["openings"] if o["door_id"]}
    for d in doc["unreal"]["doors"]:
        if d["id"] not in ids:
            problems.append("door %s has no opening" % d["id"])
        if not all(math.isfinite(x) for x in d["hinge"]):
            problems.append("door %s hinge not finite" % d["id"])
    if len(ids) != len(doc["unreal"]["doors"]):
        problems.append("door count differs from the entrance openings")
    return problems
