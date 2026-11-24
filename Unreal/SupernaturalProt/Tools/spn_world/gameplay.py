"""Gameplay integration, phase 1, of the PLACEHOLDER world (developer approval 2026-11-13): a short playable route at the north edge of the town.

    start on Oak Street at the north edge of the town -> a resident (NPC) on the street -> house oak_n7 (unlocked: key and notebook on the table)
    -> house oak_n6 (locked, the key opens it: a flashlight inside) -> north along Oak Street onto the pine track (second NPC) -> the cabin

Only existing systems are placed: ASpnTestDoor (lock, key, persistence), ASpnTestKey, ASpnWorldPickup (real inventory items), ASpnLightSwitch,
ASpnNpc with NavMeshBoundsVolumes (runtime navigation, DefaultEngine.ini), ASpnPlayAreaGuard, engine BlockingVolumes. Nothing here is a canon
location, story element or design; ids, positions and numbers are PLACEHOLDERS. Called by build_world.py in phase H, after the woodland.

HOUSES is read by buildings.house() to give the two houses a door, an inner wall face, a floor, a ceiling and a few pieces of furniture.
"""

import math

import unreal

import layout
import ue_kit as k

CLS = "/Script/SupernaturalProt."
KEY_ID = "key.spnworld.oak_n6"
HOUSES = {
    "oak_n7": {"door_id": "spnworld.oak_n7_door", "locked": False, "key": ""},
    "oak_n6": {"door_id": "spnworld.oak_n6_door", "locked": True, "key": KEY_ID},
}
NPCS = (2001, "Resident"), (2002, "Walker")
BOUNDARY_INSET = 3000.0      # the walls stand 30 m inside the landscape edge, deep in the edge forest
BOUNDARY_HEIGHT = 60000.0


def _box_volume(cls, cx, cy, cz, sx, sy, sz, label, folder="Gameplay"):
    """Engine volume with the default 200 cm brush scaled to sx x sy x sz cm."""
    actor = k.spawn(cls, cx, cy, cz, 0.0, label, folder=folder)
    actor.set_actor_scale3d(unreal.Vector(sx / 200.0, sy / 200.0, sz / 200.0))
    extent = actor.get_actor_bounds(False)[1]
    if min(extent.x, extent.y, extent.z) < 50.0:
        k.fail("%s has no brush (extent %.0f x %.0f x %.0f)" % (label, extent.x, extent.y, extent.z))
    return actor


def _ground(terrain, x, y):
    return terrain.z(x, y)


def boundary(ctx, route_start):
    """Blocking walls round the playable area, hidden in the edge forest; where the highway leaves the map, a rockfall and fallen trunks
    close the road so that the wall is not met on open asphalt. A play-area guard is the second barrier (returns a pawn that got out)."""
    terrain, roads = ctx["terrain"], ctx["roads"]
    edge = layout.EXTENT - BOUNDARY_INSET
    length = 2 * edge + 400.0
    walls = []
    for name, cx, cy, sx, sy in (("N", 0.0, edge + 100.0, length, 200.0), ("S", 0.0, -edge - 100.0, length, 200.0),
                                 ("E", edge + 100.0, 0.0, 200.0, length), ("W", -edge - 100.0, 0.0, 200.0, length)):
        walls.append(_box_volume(unreal.BlockingVolume, cx, cy, 0.0, sx, sy, BOUNDARY_HEIGHT, "SPN_Boundary_" + name))

    # the highway is the only road that leaves the map: close it with rocks and trunks just inside the wall
    import fab
    rock = fab.static(fab.ROCKS)
    log = fab.static(fab.LOG)
    hwy = roads["hwy"]
    barriers = []
    for end_s, sign in ((0.0, 1.0), (hwy.length, -1.0)):
        # walk inwards along the road to the wall line, then 12 m further
        s = end_s
        while abs(hwy.offset(s, 0.0)[0]) > edge - 1200.0 and 0.0 <= s <= hwy.length:
            s += sign * 200.0
        # a rockfall over the whole width of road and verges (+-15 m): three staggered rows of big boulders, the middle row the highest
        n = 0
        for row, (along, scale, lift) in enumerate(((0.0, 3.0, -30.0), (sign * 260.0, 3.8, -10.0), (sign * 520.0, 2.8, -40.0))):
            for side in range(-1500, 1501, 300):
                jitter = ((side * 7 + row * 13) % 11 - 5) * 25.0
                x, y, _, yaw = hwy.offset(s + along + jitter, side + jitter * 0.6)
                if rock is not None:
                    a = k.place(rock, x, y, _ground(terrain, x, y) + lift, yaw + 47.0 * n, "SPN_Barrier_Rock_%d_%d" % (int(end_s > 0), n), folder="Gameplay")
                    a.set_actor_scale3d(unreal.Vector(scale + 0.3 * (n % 3), scale * 0.9, scale * (1.1 + 0.15 * (n % 2))))
                n += 1
        if log is not None:
            for j, (side, along) in enumerate(((-450.0, -sign * 180.0), (380.0, -sign * 120.0), (0.0, sign * 700.0))):
                x, y, _, yaw = hwy.offset(s + along, side)
                a = k.place(log, x, y, _ground(terrain, x, y) + 35.0, yaw + 78.0 + 17.0 * j, "SPN_Barrier_Log_%d_%d" % (int(end_s > 0), j), folder="Gameplay")
                a.set_actor_scale3d(unreal.Vector(4.5, 4.5, 4.5))
        barriers.append(hwy.offset(s, 0.0)[:2])

    # second barrier: the guard returns a pawn that left the area to the nearest reset point on a road or the route
    guard = k.spawn(unreal.load_class(None, CLS + "SpnPlayAreaGuard"), 0.0, 0.0, 0.0, 0.0, "SPN_PlayAreaGuard", folder="Gameplay")
    guard.set_editor_property("box_extent", unreal.Vector(edge + 800.0, edge + 800.0, BOUNDARY_HEIGHT / 2.0))
    resets = [route_start]
    for s in (3000.0, hwy.length * 0.5, hwy.length - 3000.0):
        x, y, z, _ = hwy.offset(s, -150.0)
        resets.append((x, y, terrain.z(x, y) + 120.0))
    guard.set_editor_property("reset_points", [unreal.Vector(*p) for p in resets])
    guard.set_editor_property("safe_location", unreal.Vector(*route_start))
    k.log("gameplay: boundary walls at +-%.0f cm, highway closed at %s, %d guard reset points (rock %s, log %s)" % (edge, barriers, len(resets), rock is not None, log is not None))
    return {"edge": edge, "barriers": [[round(x, 1), round(y, 1)] for x, y in barriers]}


def navigation(ctx, route_area):
    """Navigation bounds only where NPCs walk: the north end of Oak Street round the two houses, and the pine track to the cabin."""
    terrain, roads = ctx["terrain"], ctx["roads"]
    x0, y0, x1, y1 = route_area
    zc = terrain.z((x0 + x1) / 2, (y0 + y1) / 2)
    volumes = [_box_volume(unreal.NavMeshBoundsVolume, (x0 + x1) / 2, (y0 + y1) / 2, zc, x1 - x0, y1 - y0, 6000.0, "SPN_Nav_Town")]
    pine = roads["pine"]
    step = 4500.0
    s = 0.0
    i = 0
    while s < pine.length:
        ax, ay, _, _ = pine.offset(s, 0.0)
        bx, by, _, _ = pine.offset(min(s + step, pine.length), 0.0)
        cx, cy = (ax + bx) / 2, (ay + by) / 2
        volumes.append(_box_volume(unreal.NavMeshBoundsVolume, cx, cy, terrain.z(cx, cy), abs(bx - ax) + 2400.0, abs(by - ay) + 2400.0, 6000.0, "SPN_Nav_Track_%d" % i))
        s += step
        i += 1
    # the navigation data itself is created and built by the navigation system when the level starts (DefaultEngine.ini: RuntimeGeneration=Dynamic);
    # spawning a RecastNavMesh actor from the commandlet is not possible (tried 2026-11-13: "could not spawn")
    k.log("gameplay: %d navigation volumes" % len(volumes))
    return len(volumes)


def npc(terrain, npc_id, name, route):
    z = lambda x, y: terrain.z(x, y) + 95.0      # noqa: E731  (capsule centre above the ground)
    x, y = route[0]
    yaw = math.degrees(math.atan2(route[1][1] - y, route[1][0] - x))
    actor = k.spawn(unreal.load_class(None, CLS + "SpnNpc"), x, y, z(x, y), yaw, "SPN_Npc_" + name, folder="Gameplay")
    actor.set_editor_property("npc_id", npc_id)
    actor.set_editor_property("display_name", name)
    actor.set_editor_property("routine_points", [unreal.Vector(px, py, z(px, py)) for px, py in route])
    actor.set_editor_property("routine_wait_seconds", 4.0)
    return actor


def build(ctx):
    terrain, roads = ctx["terrain"], ctx["roads"]
    lots = {lot.name: lot for lot in ctx["lots"]}
    for cls in ("SpnTestDoor", "SpnTestKey", "SpnWorldPickup", "SpnNpc", "SpnPlayAreaGuard", "SpnLightSwitch"):
        if unreal.load_class(None, CLS + cls) is None:
            k.fail(cls + " not found; build SupernaturalProtEditor first")
    oak, pine = roads["oak"], roads["pine"]
    house_a, house_b = lots["oak_n7"], lots["oak_n6"]

    # start: on Oak Street at the north edge of the town, about 70 m south of the two houses, facing north
    s_a = oak.nearest_s(house_a.fx, house_a.fy)[0]
    s_b = oak.nearest_s(house_b.fx, house_b.fy)[0]
    s_houses = min(s_a, s_b)
    sx, sy, _, syaw = oak.offset(s_houses - 7000.0, -150.0)
    start = (sx, sy, terrain.z(sx, sy) + 120.0)

    # objects on the tables (buildings.house stores the table top in lot.gameplay); local x along the front, y into the house
    def on_table(lot, dx, lift):
        lx, ly, lz = lot.gameplay["table_top"]
        wx, wy = lot.to_world(lx + dx, ly)
        return wx, wy, lot.z + lz + lift

    x, y, z = on_table(house_a, -35.0, 2.0)
    key = k.spawn(unreal.load_class(None, CLS + "SpnTestKey"), x, y, z, house_a.yaw, "SPN_Key_OakN6", folder="Gameplay")
    key.set_editor_property("key_id", KEY_ID)
    pickups = {}
    for lot, stable, definition, dx in ((house_a, "spnworld.pickup.notebook", "spn_test:item.notebook", 35.0),
                                        (house_b, "spnworld.pickup.flashlight", "spn_test:item.flashlight", 0.0)):
        x, y, z = on_table(lot, dx, 6.0)
        p = k.spawn(unreal.load_class(None, CLS + "SpnWorldPickup"), x, y, z, lot.yaw, "SPN_Pickup_" + stable.split(".")[-1], folder="Gameplay")
        p.set_editor_property("stable_id", stable)
        p.set_editor_property("definition_id", definition)
        p.set_editor_property("quantity", 1)
        pickups[stable] = (x, y, z)

    # NPCs: a resident walking the street between the houses, a walker on the first part of the pine track
    resident_route = [oak.offset(s, side)[:2] for s, side in ((s_houses - 2500.0, 200.0), (s_houses + 1800.0, 200.0), (s_houses + 400.0, -150.0))]
    walker_route = [pine.offset(s, side)[:2] for s, side in ((900.0, 60.0), (5200.0, 60.0), (3000.0, -60.0))]
    npc(terrain, NPCS[0][0], NPCS[0][1], resident_route)
    npc(terrain, NPCS[1][0], NPCS[1][1], walker_route)

    # navigation over the route area of the town (the houses, the street between them and the start)
    xs = [house_a.fx, house_b.fx, sx] + [p[0] for p in resident_route]
    ys = [house_a.fy, house_b.fy, sy] + [p[1] for p in resident_route]
    route_area = (min(xs) - 2500.0, min(ys) - 2500.0, max(xs) + 2500.0, max(ys) + 2500.0)
    nav_volumes = navigation(ctx, route_area)
    edge = boundary(ctx, start)

    # the route for the QA test and the cine walker (world cm; z = terrain)
    def p3(x, y):
        return [round(x, 1), round(y, 1), round(terrain.z(x, y), 1)]

    def house_points(lot):
        c = lambda lx, ly, lz=0.0: [round(v, 1) for v in lot.to_world(lx, ly)] + [round(lot.z + lz, 1)]      # noqa: E731
        door_lx = lot.interior["door"][0] + 50.0          # door centre along the front (hinge + half the leaf)
        fh = lot.interior["door"][2]
        tlx, tly, tlz = lot.gameplay["table_top"]
        # the front garden fence (props.py) has a 150 cm gap on the door axis between the road edge and the fence line: the way in leads through it
        gate_ly = -(lot.setback - lot.road_half) + 60.0
        return {"door_id": HOUSES[lot.name]["door_id"], "locked": HOUSES[lot.name]["locked"], "gate": c(door_lx, gate_ly, 0.0),
                "in_front":c(door_lx, -lot.gameplay["porch_depth"] - 120.0, fh), "porch": c(door_lx, -110.0, fh), "doorway": c(door_lx, 10.0, fh),
                "inside": c(door_lx, 170.0, fh), "table_front": c(tlx, tly - 110.0, fh), "table": c(tlx, tly, tlz),
                "yaw": round(lot.yaw, 2)}

    tx, ty, _, _ = pine.offset(6000.0, 0.0)
    oak_end = oak.offset(oak.length - 300.0, 0.0)
    ctx["gameplay"] = {
        "start": [round(v, 1) for v in start], "start_yaw": round(syaw, 1),
        "npcs": {str(i): name for i, name in NPCS},
        "resident_route": [p3(*q) for q in resident_route], "walker_route": [p3(*q) for q in walker_route],
        "house_a": house_points(house_a), "house_b": house_points(house_b),
        "key_id": KEY_ID, "pickups": {k_: [round(v, 1) for v in xyz] for k_, xyz in pickups.items()},
        "to_track": [p3(oak_end[0], oak_end[1]), p3(tx, ty)],
        "boundary": edge, "nav_volumes": nav_volumes,
        "route_area": [round(v, 1) for v in route_area],
    }
    k.log("gameplay: route start %s, houses %s / %s, key %s" % (start, house_a.name, house_b.name, KEY_ID))
    return ctx["gameplay"]
