"""Test points of the SpnWorld gameplay integration test (SpnQA.SpnWorld), written with every world build.

    Saved/SpnWorldGen/qa_points.json

The test reads them at run time, so it follows the generated world instead of carrying coordinates of its own. All values in cm,
world space; z is the terrain height (the test drops the character from above the ground). Pure Python; called by build_world.py.
"""

import json
import math
import os

import layout


def _segment_distance(px, py, ax, ay, bx, by):
    dx, dy = bx - ax, by - ay
    t = max(0.0, min(1.0, ((px - ax) * dx + (py - ay) * dy) / max(dx * dx + dy * dy, 1e-6)))
    return math.hypot(px - (ax + t * dx), py - (ay + t * dy))


def _free_path(ax, ay, bx, by, obstacles, clearance, skip=None):
    for i, (ox, oy, r) in enumerate(obstacles):
        if i == skip:
            continue
        if _segment_distance(ox, oy, ax, ay, bx, by) < r + clearance:
            return False
    return True


def build(ctx, out_dir):
    roads, lots, terrain = ctx["roads"], ctx["lots"], ctx["terrain"]
    wood = ctx.get("woodland", {"trees": [], "rocks": []})
    trees = list(wood["trees"])
    rocks = [(x, y, 90.0 * s) for x, y, s in wood["rocks"]]       # a scanned cluster is about 3 m x 1.9 m: 90 cm keeps clear of it
    obstacles = trees + rocks
    z = terrain.z

    def p(x, y):
        return [round(x, 1), round(y, 1), round(z(x, y), 1)]

    points = {"map": "/Game/SpnWorld/Lvl_SpnWorld"}

    # 1 asphalt: a 40 m stretch of the main street, on the carriageway (the pavement has benches, bins and lamp posts)
    hwy = roads["hwy"]
    main0, _ = layout.main_street(roads)
    a = hwy.offset(main0 + 3000.0, -250.0)
    b = hwy.offset(main0 + 7000.0, -250.0)
    points["street"] = [p(a[0], a[1]), p(b[0], b[1])]

    # 2 forest track: the last 45 m of the pine track before the cabin
    pine = roads["pine"]
    a = pine.offset(pine.length - 7000.0, 0.0)
    b = pine.offset(pine.length - 2500.0, 0.0)
    points["forest_track"] = [p(a[0], a[1]), p(b[0], b[1])]

    # 3 uneven ground: from the track 12-18 m into the forest, onto a slope with a free line of walk
    best = None
    for s in range(int(pine.length - 9000), int(pine.length - 2000), 500):
        for side in (1.0, -1.0):
            for dist in (1200.0, 1500.0, 1800.0):
                sx, sy, _, _ = pine.offset(s, 0.0)
                ex, ey, _, _ = pine.offset(s, side * dist)
                slope = terrain.slope(ex, ey)
                rise = abs(z(ex, ey) - z(sx, sy))
                if 0.12 <= slope <= 0.45 and rise >= 100.0 and _free_path(sx, sy, ex, ey, obstacles, 120.0):
                    score = rise
                    if best is None or score > best[0]:
                        best = (score, p(sx, sy), p(ex, ey), slope, rise)
    if best:
        points["uneven"] = {"from": best[1], "to": best[2], "slope": round(best[3], 2), "rise_cm": round(best[4])}

    # 4 a tree: a big trunk 5-14 m from the track, reachable in a straight free line from the track
    best = None
    for i, (tx, ty, r) in enumerate(trees):
        if r < 22.0:
            continue
        s, d = pine.nearest_s(tx, ty)
        if not (500.0 <= d <= 1400.0) or s > pine.length - 1500.0:
            continue
        sx, sy, _, _ = pine.offset(s, 0.0)
        if _free_path(sx, sy, tx, ty, obstacles, 110.0, skip=i) and terrain.slope(tx, ty) < 0.4:
            if best is None or r > best[0]:
                best = (r, p(sx, sy), p(tx, ty))
    if best:
        points["tree"] = {"from": best[1], "centre": best[2], "radius": round(best[0], 1)}

    # 5 a rock: the biggest scanned cluster within 26 m of the track that can be walked at in a straight free line (from the track side,
    #   else from any of eight directions); clusters closer to each other than their size count as one obstacle field and are skipped
    best = None
    for i, (rx, ry, r) in enumerate(rocks):
        s, d = pine.nearest_s(rx, ry)
        if d > 2600.0 or r < 55.0:
            continue
        tx, ty = pine.offset(s, 0.0)[0], pine.offset(s, 0.0)[1]
        directions = [math.atan2(ty - ry, tx - rx)] + [k * math.pi / 4.0 for k in range(8)]
        for a in directions:
            sx, sy = rx + math.cos(a) * (r + 500.0), ry + math.sin(a) * (r + 500.0)
            if terrain.slope(sx, sy) > 0.45:
                continue
            if _free_path(sx, sy, rx + math.cos(a) * r, ry + math.sin(a) * r, obstacles, 70.0, skip=len(trees) + i):
                if best is None or r > best[0]:
                    best = (r, p(sx, sy), p(rx, ry))
                break
    if best:
        points["rock"] = {"from": best[1], "centre": best[2], "keep_out": round(best[0] * 0.5, 1)}

    # 6-8 the cabin: side wall, door, inside, outside (cabin local frame: +x along the front, +y into the cabin; see buildings.cabin)
    cabin = next(lot for lot in lots if lot.kind == "cabin")
    c = lambda lx, ly: p(*cabin.to_world(lx, ly))           # noqa: E731
    points["cabin"] = {
        "wall_from": c(900.0, 160.0), "wall_target": c(0.0, 160.0), "wall_plane": c(410.0, 160.0),
        "door_id": "cabin_door", "in_front": c(0.0, -190.0), "doorway": c(0.0, 10.0), "inside": c(-60.0, 160.0), "outside": c(0.0, -420.0),
        # deep in the room, round the table and stools, so that the third-person camera is inside the room too (buildings.cabin furniture)
        "room_a": c(-230.0, 200.0), "room_b": c(-230.0, 430.0),
        # in front of the table on the door axis: the door opens inwards and its leaf stands along the left jamb, so the way in and out
        # goes straight through the doorway to here (50 cm beside the leaf) and only then sideways
        "centre": c(0.0, 200.0),
        "yaw": round(cabin.yaw, 2),
    }
    # 9 the gameplay route (gameplay.py): start, NPC routines, the two houses, key and pickups, the way to the track, the world boundary
    if "gameplay" in ctx:
        g = dict(ctx["gameplay"])
        edge = g["boundary"]["edge"]
        hwy = roads["hwy"]
        # boundary tests: in the edge forest north of the town (walk 7 m north into the wall on a line free of trunks and rocks, so the wall
        # itself stops the player, not a tree), and the highway barrier in the west
        best = None
        for x in range(-30000, 30001, 500):
            if terrain.slope(x, edge - 400.0) < 0.35 and _free_path(x, edge - 700.0, x, edge + 300.0, obstacles, 80.0):
                if best is None or abs(x) < abs(best):
                    best = x
        if best is not None:
            g["boundary_walk"] = {"from": p(best, edge - 700.0), "to": p(best, edge + 800.0), "wall_y": round(edge, 1)}
        wx, wy = g["boundary"]["barriers"][0]
        g["barrier_walk"] = {"from": p(*hwy.offset(hwy.nearest_s(wx, wy)[0] + 3500.0, -150.0)[:2]), "to": p(wx, wy)}
        g["outside"] = p(0.0, layout.EXTENT - 500.0)      # beyond the wall (guard test: the pawn is put back)
        points["gameplay"] = g
    path = os.path.join(out_dir, "qa_points.json")
    with open(path, "w") as f:
        json.dump(points, f, indent=1)
    return path, sorted(points)
