"""Buildings of the placeholder world, generated as static meshes: houses, main-street shops and a few landmarks (filling station, motel,
church, cabin, barn, water tower, diner, workshop). Proportions follow common North American small-town construction (2.8 m storeys,
clapboard houses with porches, brick shop fronts). Placeholder architecture: no building here is a canon location."""

import json
import math
import os
import random
import zlib

import unreal

import archkit as a
import gameplay
import meshkit
import ue_kit as k
from archkit import Frame

STOREY = 285.0


def frames(x0, x1, y0, y1, oz=0.0):
    return {"front": Frame(x0, y0, 1, 0, 0, -1, oz), "back": Frame(x0, y1, 1, 0, 0, 1, oz), "left": Frame(x0, y0, 0, 1, -1, 0, oz), "right": Frame(x1, y0, 0, 1, 1, 0, oz)}


def bays(length, count, width, margin=0.0):
    span = (length - 2 * margin) / count
    return [(margin + span * i + (span - width) / 2, margin + span * i + (span + width) / 2) for i in range(count)]


PAVED = []      # (world x, world y, half x, half y, yaw) of every draped ground surface, see paved_rects()


def drape(mesh, key, lot, terrain, x0, x1, y0, y1, step=160.0, lift=5.0):
    """Ground surface in local lot coordinates that follows the terrain (parking areas, drives, paths)."""
    PAVED.append(tuple(lot.to_world((x0 + x1) / 2, (y0 + y1) / 2)) + (abs(x1 - x0) / 2, abs(y1 - y0) / 2, lot.yaw))
    nx, ny = max(1, int(math.ceil((x1 - x0) / step))), max(1, int(math.ceil((y1 - y0) / step)))
    def pt(i, j):
        lx, ly = x0 + (x1 - x0) * i / nx, y0 + (y1 - y0) * j / ny
        wx, wy = lot.to_world(lx, ly)
        return (lx, ly, terrain.z(wx, wy) + lift - lot.z)
    for i in range(nx):
        for j in range(ny):
            p = [pt(i, j), pt(i + 1, j), pt(i + 1, j + 1), pt(i, j + 1)]
            mesh.quad(key, *p, None, [(q[0], q[1]) for q in p], hint=(0, 0, 1))


def road_reach(lot):
    """Local y of the road edge in front of the lot (negative: towards the road)."""
    return -(lot.setback - lot.road_half) + 10.0


# ---------------------------------------------------------------- house

HOUSE_WALLS = ["siding_white", "siding_cream", "siding_blue", "siding_sage", "siding_yellow", "siding_grey", "siding_white", "siding_brown"]
ROOFS = ["shingles", "shingles", "shingles_brown", "shingles_green"]
DOORS = ["metal_red", "metal_green", "metal_blue", "planks_dark", "trim_dark", "metal_white"]


def house_interior(m, lot, x0, x1, W, D, fh, ds0, ds1, spec):
    """Ground-floor room of an enterable house of the gameplay route (gameplay.HOUSES): floor, ceiling, a table in the back half (the route's
    key and items lie on it), a shelf, a rug and two stools; the door is a real ASpnTestDoor (lot.interior, placed by build()).
    PLACEHOLDER furnishing; the room keeps a free way from the door to the front of the table."""
    zc = fh + STOREY
    m.box("wood_floor", x0 + 20, x1 - 20, 20.0, D - 20.0, fh - 14.0, fh, skip="xXyY")
    m.quad("plaster", (x0 + 20, 20.0, zc), (x1 - 20, 20.0, zc), (x1 - 20, D - 20.0, zc), (x0 + 20, D - 20.0, zc), (0, 0, -1))
    ty = D * 0.62
    m.box("carpet", -170.0, 170.0, ty - 150.0, ty + 110.0, fh, fh + 1.2, skip="z")
    m.box("planks_dark", -70.0, 70.0, ty - 40.0, ty + 40.0, fh + 70.0, fh + 75.0)                 # table, top at fh + 75
    for tx, tyy in ((-62, ty - 32), (62, ty - 32), (-62, ty + 32), (62, ty + 32)):
        m.box("planks_dark", tx - 4, tx + 4, tyy - 4, tyy + 4, fh, fh + 70.0, skip="zZ")
    for cx in (-125.0, 125.0):
        m.cyl("planks", cx, ty, fh, fh + 45.0, 17.0, segs=10)
    m.box("planks_dark", x1 - 60.0, x1 - 22.0, D * 0.35, D * 0.35 + 140.0, fh, fh + 185.0)           # shelf on the right wall
    for z in (45.0, 100.0, 150.0):
        m.box("planks", x1 - 62.0, x1 - 22.0, D * 0.35, D * 0.35 + 140.0, fh + z, fh + z + 4.0)
    m.box("lamp", -9.0, 9.0, D / 2 - 9.0, D / 2 + 9.0, zc - 60.0, zc - 40.0)                       # ceiling lamp
    m.box("metal_dark", -1.0, 1.0, D / 2 - 1.0, D / 2 + 1.0, zc - 40.0, zc)
    lot.interior = {"door": (x0 + ds0, 10.0, fh, spec["door_id"], "planks_dark"), "lamp": (0.0, D / 2, zc - 75.0),
                    "switch": (x0 + ds1 + 35.0, 21.5, fh + 125.0), "room": (x0 + 20.0, x1 - 20.0, 20.0, D - 20.0, fh, zc),
                    "door_opts": {"authored_locked": spec["locked"], "required_key_id": spec["key"]}}
    lot.gameplay = {"table_top": (0.0, ty, fh + 75.0), "porch_depth": 110.0}


def house(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D = lot.width, lot.depth
    floors = lot.params.get("floors", 1)
    wall_key, roof_key, door_key = rng.choice(HOUSE_WALLS), rng.choice(ROOFS), rng.choice(DOORS)
    shutters = rng.choice([None, None, "trim_dark", "metal_green", "metal_blue"])
    fh = rng.choice([45.0, 55.0, 70.0])
    x0, x1 = -W / 2, W / 2
    zt = fh + floors * STOREY
    pitch = rng.choice([0.5, 0.58, 0.67, 0.75])
    m.box("concrete", x0 + 2, x1 - 2, 2, D - 2, -80.0, fh, skip="zZ")
    f = frames(x0, x1, 0.0, D)
    n_front = 3 if W < 1080 else 4
    door_bay = rng.choice([1, n_front - 2]) if n_front > 3 else 1
    front_bays = bays(W, n_front, 100.0, 40.0)
    win_w, win_h, sill = rng.choice([(95.0, 140.0, 88.0), (105.0, 150.0, 80.0), (88.0, 132.0, 95.0)])
    openings = {"front": [], "back": [], "left": [], "right": []}
    windows = []
    for level in range(floors):
        zb = fh + level * STOREY + sill
        for index, (s0, s1) in enumerate(bays(W, n_front, win_w, 40.0)):
            if level == 0 and index == door_bay:
                continue
            if level == 0 and rng.random() < 0.25:   # a wider living-room window
                s0, s1 = s0 - 35.0, s1 + 35.0
            openings["front"].append((s0, s1, zb, zb + win_h)); windows.append(("front", s0, s1, zb, zb + win_h))
        for side, length, count in (("left", D, 2), ("right", D, 2), ("back", W, n_front)):
            for s0, s1 in bays(length, count, win_w * (0.9 if side != "back" else 1.0), 50.0):
                if rng.random() < 0.82:
                    openings[side].append((s0, s1, zb, zb + win_h)); windows.append((side, s0, s1, zb, zb + win_h))
    ds0, ds1 = front_bays[door_bay]
    openings["front"].append((ds0, ds1, fh, fh + 208.0))
    enterable = gameplay.HOUSES.get(lot.name)        # gameplay route (gameplay.py): a real door and a simple room on the ground floor
    u = 0.0
    for side, length in (("front", W), ("right", D), ("back", W), ("left", D)):
        a.wall(m, wall_key, f[side], length, fh, zt, openings[side], reveal=10.0, reveal_key="trim", u0=u, base_z=fh,
               inner_key="plaster" if enterable else None)
        f[side].box(m, "trim", 0, length, 0, 2.2, fh, fh + 16.0, skip="t")                       # skirt board
        f[side].box(m, "trim", 0, length, 0, 2.0, zt - 20.0, zt, skip="t")                       # frieze board
        for s in (0.0, length - 9.0):
            f[side].box(m, "trim", s, s + 9.0, 0, 2.4, fh + 16.0, zt - 20.0, skip="tzZ")          # corner boards
        u += length
    for side, s0, s1, zb, ztop in windows:
        a.window(m, f[side], s0, s1, zb, ztop, rng.random(), shutters=shutters if side == "front" else None, mullions=rng.choice([(1, 1), (1, 1), (0, 1), (2, 1)]))
    glazed, door_rnd = rng.random() < 0.5, rng.random()      # drawn in every case, so the random sequence (and the other houses) stay the same
    if enterable:
        house_interior(m, lot, x0, x1, W, D, fh, ds0, ds1, enterable)
    else:
        a.door(m, f["front"], ds0, ds1, fh, fh + 208.0, door_key, glazed=glazed, rnd=door_rnd)
    # roof
    zr = a.gable_roof(m, x0, x1, 0.0, D, zt, pitch, key=roof_key)
    for x, sign in ((x0, -1.0), (x1, 1.0)):
        a.gable_end(m, wall_key, x, 0.0, D, zt, pitch, sign)
    # chimney
    if rng.random() < 0.7:
        cx = rng.choice([x0 + 130.0, x1 - 130.0, 0.0]); cy = D / 2 + rng.choice([-60.0, 90.0])
        m.box("brick", cx - 32, cx + 32, cy - 32, cy + 32, zt + 20.0, zr + 75.0, skip="z")
        m.box("concrete", cx - 37, cx + 37, cy - 37, cy + 37, zr + 75.0, zr + 83.0)
    # porch or stoop
    front = f["front"]
    dc = (ds0 + ds1) / 2
    if lot.params.get("porch", True):
        pd = rng.choice([180.0, 210.0, 240.0])
        full = rng.random() < 0.5
        p0, p1 = (6.0, W - 6.0) if full else (max(6.0, dc - 220.0), min(W - 6.0, dc + 220.0))
        front.box(m, "planks", p0, p1, 0, pd, fh - 12.0, fh - 2.0)
        front.box(m, "trim", p0, p1, pd - 2.0, pd, 0.0, fh - 12.0, skip="zZ")
        for s in (p0, p1 - 2.0):
            front.box(m, "trim", s, s + 2.0, 0, pd, 0.0, fh - 12.0, skip="zZ")
        posts = [p0 + 8.0, p1 - 8.0] + ([dc - 75.0, dc + 75.0] if p1 - p0 > 500 else [])
        roof_z = fh + 252.0
        for s in posts:
            front.box(m, "trim", s - 6.5, s + 6.5, pd - 16.0, pd - 3.0, fh - 2.0, roof_z)
        front.box(m, "trim", p0, p1, pd - 18.0, pd - 2.0, roof_z, roof_z + 18.0)
        # lean-to roof: built in wall coordinates of the front (s along, t outwards), mapped by hand
        sub = meshkit.Mesh()
        a.gable_roof(sub, p0 - 20.0, p1 + 20.0, 0.0, pd + 5.0, roof_z + 18.0, 0.22, eave=30.0, rake=10.0, key=roof_key, half=True)
        # the sub roof rises towards +b; the porch roof must rise towards the wall (b = 0), so mirror it across the front line
        for key_, part in sub.parts.items():
            dst = m.part(key_); base = len(dst.v)
            for v, nrm in zip(part.v, part.n):
                dst.v.append((x0 + v[0], -(pd + 5.0) + v[1], v[2])); dst.n.append(nrm)
            dst.uv.extend(part.uv); dst.c.extend(part.c); dst.t.extend((t0 + base, t1 + base, t2 + base) for t0, t1, t2 in part.t)
        sorted_posts = sorted(posts)
        for s0, s1 in zip(sorted_posts, sorted_posts[1:]):
            if not (s0 < dc < s1):
                a.railing(m, "trim", front, s0 + 6.5, s1 - 6.5, pd - 9.0, fh - 2.0)
        if not full or True:
            for s, t0 in ((p0 + 4.0, 0.0), (p1 - 4.0, 0.0)):
                a.railing(m, "trim", Frame(*front.p(s, 0, 0)[:2], 0, -1, 1, 0), 10.0, pd - 16.0, 0.0, fh - 2.0)
        run = a.steps(m, "planks", front, dc - 70.0, dc + 70.0, fh - 2.0, t0=pd)
        path_start = pd + run
        if enterable:
            lot.gameplay["porch_depth"] = pd + run
    else:
        front.box(m, "concrete", dc - 80.0, dc + 80.0, 0, 110.0, -30.0, fh - 2.0, skip="z")
        run = a.steps(m, "concrete", front, dc - 80.0, dc + 80.0, fh - 2.0, t0=110.0)
        path_start = 110.0 + run
        front.box(m, "trim", dc - 85.0, dc + 85.0, 0, 70.0, fh + 228.0, fh + 236.0)             # door hood
    m.box("lamp", x0 + ds1 + 16.0, x0 + ds1 + 28.0, -9.0, 0.0, fh + 175.0, fh + 195.0, skip="Y")
    lot.lamp = (x0 + ds1 + 22.0, -22.0, fh + 180.0)
    # garage
    drive_x = None
    if lot.params.get("garage"):
        side = rng.choice([-1.0, 1.0])
        gw, gd, gset = 360.0, 620.0, rng.choice([60.0, 140.0])
        g0 = x1 if side > 0 else x0 - gw
        g1 = g0 + gw
        gf = frames(g0, g1, gset, gset + gd)
        gz = 262.0
        m.box("concrete", g0 + 2, g1 - 2, gset + 2, gset + gd - 2, -80.0, 12.0, skip="zZ")
        a.wall(m, wall_key, gf["front"], gw, 12.0, gz, [(45.0, gw - 45.0, 12.0, 222.0)], reveal=14.0, reveal_key="trim", base_z=12.0)
        gf["front"].box(m, "metal_white", 45.0, gw - 45.0, -12.0, -8.0, 12.0, 222.0, skip="t")
        for i in range(1, 4):
            gf["front"].box(m, "trim_dark", 45.0, gw - 45.0, -8.0, -7.4, 12.0 + i * 52.0, 13.5 + i * 52.0, skip="tsS")
        for s in ("back", "left" if side < 0 else "right"):
            a.wall(m, wall_key, gf[s], gw if s == "back" else gd, 12.0, gz, [], base_z=12.0)
        a.gable_roof(m, gset, gset + gd, g0, g1, gz, 0.42, key=roof_key, axis="y", eave=35.0)
        a.gable_end(m, wall_key, gset, g0, g1, gz, 0.42, -1.0, axis="y")
        a.gable_end(m, wall_key, gset + gd, g0, g1, gz, 0.42, 1.0, axis="y", vent=False)
        drive_x = (g0 + g1) / 2
        drape(m, "concrete", lot, terrain, drive_x - 150.0, drive_x + 150.0, road_reach(lot), gset)
    # path from the steps to the road
    drape(m, "concrete", lot, terrain, x0 + dc - 55.0, x0 + dc + 55.0, road_reach(lot), -path_start, step=140.0)
    lot.mail = (x0 + dc + 110.0, road_reach(lot) + 60.0)
    return m


# ---------------------------------------------------------------- shop (main street row building)

def shop(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D = lot.width, lot.depth
    floors = lot.params.get("floors", 1)
    wall_key = {"brick": rng.choice(["brick", "brick", "brick_dark", "brick_tan"]), "plaster": rng.choice(["plaster", "plaster_grey", "plaster_ochre"]), "siding": rng.choice(["siding_white", "siding_sage", "siding_blue"])}[lot.params.get("wall", "brick")]
    side_key = "brick_dark" if "brick" in wall_key else wall_key
    accent = rng.choice(["metal_green", "metal_red", "metal_blue", "metal_dark", "trim_dark", "metal_yellow"])
    x0, x1 = -W / 2, W / 2
    g = 370.0 + rng.choice([0.0, 20.0, 45.0])
    zt = g + (floors - 1) * 300.0 + rng.choice([70.0, 95.0, 120.0])
    f = frames(x0, x1, 0.0, D)
    front = f["front"]
    # shop front: plinth, big panes, a recessed door, sign band
    n = 3 if W < 1150 else 4
    door_bay = rng.randrange(n)
    span = (W - 60.0) / n
    openings, panes = [], []
    for i in range(n):
        s0, s1 = 30.0 + span * i + 22.0, 30.0 + span * (i + 1) - 22.0
        if i == door_bay:
            openings.append((s0 + 20.0, s1 - 20.0, 8.0, 262.0)); door_span = (s0 + 20.0, s1 - 20.0)
        else:
            openings.append((s0, s1, 62.0, 262.0)); panes.append((s0, s1))
    upper = []
    if floors > 1:
        for s0, s1 in bays(W, 3 if W < 1150 else 4, 95.0, 50.0):
            upper.append((s0, s1, g + 85.0, g + 235.0))
    a.wall(m, wall_key, front, W, 0.0, zt, openings + upper, reveal=16.0, reveal_key="trim_dark", base_z=0.0)
    for s0, s1 in panes:
        a.window(m, front, s0, s1, 62.0, 262.0, 0.5 + rng.random() * 0.5, glass="shopwindow", trim="trim_dark", casing=0.0, sill=False, mullions=(1 if s1 - s0 > 260 else 0, 0), inset=12.0)
        front.box(m, "concrete", s0 - 6.0, s1 + 6.0, 0, 5.0, 54.0, 62.0)
    ds0, ds1 = door_span
    front.box(m, "trim_dark", ds0, ds1, -70.0, -64.0, 8.0, 262.0, skip="t")
    a.door(m, Frame(*front.p(0, -64.0, 0)[:2], 1, 0, 0, -1), (ds0 + ds1) / 2 - 50.0, (ds0 + ds1) / 2 + 50.0, 8.0, 215.0, "trim_dark", trim="trim_dark", inset=0.0, glazed=True, rnd=rng.random(), casing=0.0)
    front.box(m, "concrete", ds0, ds1, -64.0, 0.0, 0.0, 8.0)
    for s0, s1, zb, ztop in upper:
        a.window(m, front, s0, s1, zb, ztop, rng.random(), trim="trim", casing=0.0, sill=False)
        front.box(m, "concrete", s0 - 12.0, s1 + 12.0, 0, 5.0, ztop, ztop + 22.0)
        front.box(m, "concrete", s0 - 8.0, s1 + 8.0, 0, 7.0, zb - 9.0, zb)
    front.box(m, accent, 12.0, W - 12.0, 0, 9.0, 276.0, 348.0)                                  # sign band
    lot.sign = (0.0, -11.0, 312.0, W - 60.0)
    if rng.random() < 0.55:                                                                    # awning
        aw0, aw1 = 30.0, W - 30.0
        col = rng.choice(["metal_red", "metal_green", "metal_blue", "metal_dark"])
        m.quad(col, front.p(aw0, 0, 272.0), front.p(aw1, 0, 272.0), front.p(aw1, 130.0, 226.0), front.p(aw0, 130.0, 226.0), None, None, hint=(0, -0.3, 1))
        m.quad(col, front.p(aw0, 0, 270.0), front.p(aw1, 0, 270.0), front.p(aw1, 130.0, 224.0), front.p(aw0, 130.0, 224.0), None, None, hint=(0, 0.3, -1))
        m.quad(col, front.p(aw0, 130.0, 226.0), front.p(aw1, 130.0, 226.0), front.p(aw1, 130.0, 206.0), front.p(aw0, 130.0, 206.0), front.vec(0, 1, 0))
    # cornice and parapet
    front.box(m, "concrete", -6.0, W + 6.0, 0, 14.0, zt - 34.0, zt - 20.0)
    front.box(m, "concrete", -10.0, W + 10.0, 0, 22.0, zt - 20.0, zt - 6.0)
    for side, length in (("left", D), ("right", D)):
        a.wall(m, side_key, f[side], length, 0.0, zt - 25.0, [], base_z=0.0)
    back_open = [(W * 0.5 - 50.0, W * 0.5 + 50.0, 5.0, 210.0)] + ([(W * 0.2, W * 0.2 + 80.0, g + 100.0, g + 210.0)] if floors > 1 else [])
    a.wall(m, side_key, f["back"], W, 0.0, zt - 25.0, back_open, reveal=14.0, base_z=0.0)
    a.door(m, f["back"], W * 0.5 - 50.0, W * 0.5 + 50.0, 5.0, 210.0, "metal_dark", trim="trim_dark", casing=0.0)
    if floors > 1:
        a.window(m, f["back"], W * 0.2, W * 0.2 + 80.0, g + 100.0, g + 210.0, rng.random(), casing=0.0)
    drape(m, "sidewalk", lot, terrain, x0 - 15.0, x1 + 15.0, -110.0, 6.0, step=150.0, lift=9.0)
    roof_z = zt - 80.0
    m.quad("gravel", (x0, 0, roof_z), (x1, 0, roof_z), (x1, D, roof_z), (x0, D, roof_z), (0, 0, 1))
    for fr, length, top in ((f["front"], W, zt), (f["left"], D, zt - 25.0), (f["right"], D, zt - 25.0), (f["back"], W, zt - 25.0)):   # parapet inside faces and caps
        fr.box(m, "concrete", 0, length, -22.0, 2.0, top, top + 7.0)
        m.quad(side_key, fr.p(0, -20.0, roof_z), fr.p(length, -20.0, roof_z), fr.p(length, -20.0, top), fr.p(0, -20.0, top), fr.vec(0, -1, 0))
    return m


# ---------------------------------------------------------------- landmarks

def simple_block(m, x0, x1, y0, y1, z0, z1, key, openings=None, **kw):
    f = frames(x0, x1, y0, y1)
    openings = openings or {}
    for side, length in (("front", x1 - x0), ("right", y1 - y0), ("back", x1 - x0), ("left", y1 - y0)):
        a.wall(m, key, f[side], length, z0, z1, openings.get(side, []), base_z=z0, **kw)
    return f


def gas(lot, terrain, rng):
    """Filling station: forecourt, canopy on four columns with two pump islands, kiosk, price sign."""
    m = meshkit.Mesh()
    W, D = lot.width, lot.depth
    drape(m, "asphalt_old", lot, terrain, -W / 2, W / 2, road_reach(lot), D * 0.72, step=200.0)
    cy = 900.0
    m.box("concrete", -700.0, 700.0, cy - 420.0, cy + 420.0, -20.0, 7.0, skip="z")
    top = 470.0
    for x in (-520.0, 520.0):
        for y in (cy - 250.0, cy + 250.0):
            m.box("metal_white", x - 16, x + 16, y - 16, y + 16, 7.0, top)
            m.box("metal_dark", x - 20, x + 20, y - 20, y + 20, 7.0, 60.0)
    m.box("metal_white", -780.0, 780.0, cy - 470.0, cy + 470.0, top, top + 22.0, skip="xXyY")
    for fr, length in ((Frame(-780.0, cy - 470.0, 1, 0, 0, -1), 1560.0), (Frame(-780.0, cy + 470.0, 1, 0, 0, 1), 1560.0), (Frame(-780.0, cy - 470.0, 0, 1, -1, 0), 940.0), (Frame(780.0, cy - 470.0, 0, 1, 1, 0), 940.0)):
        fr.box(m, "metal_white", 0, length, 0, 4.0, top - 6.0, top + 70.0, skip="t")
        fr.box(m, "metal_red", 0, length, 4.0, 5.0, top + 18.0, top + 46.0, skip="t")
    for x in (-250.0, 250.0):                                                                  # lights under the canopy
        for y in (cy - 200.0, cy + 200.0):
            m.box("lamp_cold", x - 45, x + 45, y - 20, y + 20, top - 5.0, top - 0.5)
    lot.lights = [(x, cy, top - 30.0) for x in (-300.0, 300.0)]
    for x in (-330.0, 330.0):                                                                  # pump islands
        m.box("kerb", x - 60, x + 60, cy - 260.0, cy + 260.0, 7.0, 22.0, skip="z")
        for y in (cy - 120.0, cy + 120.0):
            m.box("metal_white", x - 32, x + 32, y - 24, y + 24, 22.0, 150.0, skip="z")
            m.box("metal_red", x - 34, x + 34, y - 26, y + 26, 150.0, 190.0)
            m.box("metal_dark", x - 33, x + 33, y - 15, y + 15, 95.0, 135.0)
            m.box("steel", x - 30, x + 30, y - 25.5, y + 25.5, 22.0, 34.0)
        for y in (cy - 300.0, cy + 300.0):                                                     # bollards
            m.cyl("metal_yellow", x - 45, y, 7.0, 95.0, 6.0, segs=8)
            m.cyl("metal_yellow", x + 45, y, 7.0, 95.0, 6.0, segs=8)
    # kiosk
    k0, k1, ky0, ky1, kz = -520.0, 420.0, D * 0.72 - 640.0, D * 0.72, 330.0
    front_open = [(60.0, 330.0, 70.0, 250.0), (380.0, 500.0, 5.0, 222.0), (550.0, 880.0, 70.0, 250.0)]
    f = simple_block(m, k0, k1, ky0, ky1, 0.0, kz, "plaster", {"front": front_open}, reveal=14.0, reveal_key="trim_dark")
    for s0, s1, zb, zt in (front_open[0], front_open[2]):
        a.window(m, f["front"], s0, s1, zb, zt, 0.9, glass="shopwindow", trim="trim_dark", casing=0.0, sill=False, mullions=(2, 0), inset=10.0)
    a.door(m, f["front"], 380.0, 500.0, 5.0, 222.0, "trim_dark", trim="trim_dark", glazed=True, rnd=0.9, casing=0.0)
    f["front"].box(m, "metal_red", 0, k1 - k0, 0, 8.0, kz - 62.0, kz - 8.0)
    m.box("metal_white", k0 - 40, k1 + 40, ky0 - 110.0, ky1 + 30, kz, kz + 18.0)
    m.box("concrete", k0 - 30, k1 + 30, ky0 - 130.0, ky0, -20.0, 12.0, skip="z")
    m.box("metal_white", k0 + 70, k0 + 230, ky0 - 75.0, ky0 - 5.0, 12.0, 190.0)               # ice chest
    m.cyl("metal_white", k1 + 160.0, ky0 + 300.0, 40.0, 190.0, 55.0, segs=14)                  # propane tank on feet
    lot.sign = ((k0 + k1) / 2, ky0 - 9.0, kz - 35.0, 500.0)
    # price sign at the road
    sx, sy = W / 2 - 250.0, road_reach(lot) + 260.0
    m.box("steel", sx - 11, sx + 11, sy - 11, sy + 11, -30.0, 640.0)
    m.box("metal_white", sx - 130, sx + 130, sy - 16, sy + 16, 400.0, 660.0)
    m.box("metal_red", sx - 122, sx + 122, sy - 17.5, sy + 17.5, 560.0, 645.0)
    m.box("lamp_cold", sx - 110, sx + 110, sy - 17.5, sy + 17.5, 420.0, 540.0)
    lot.pole_sign = (sx, sy - 19.0, 602.0)
    return m


def motel(lot, terrain, rng):
    """L-shaped single-storey motel: a row of rooms behind a covered walkway, an office at the corner, parking in front."""
    m = meshkit.Mesh()
    W, D = lot.width, lot.depth
    drape(m, "asphalt_old", lot, terrain, -W / 2, W / 2, road_reach(lot), D - 760.0, step=220.0)
    room_w, room_d, h, fh = 400.0, 700.0, 275.0, 16.0
    lot.doors, lot.lights, lot.numbers = [], [], []
    def wing(ox, oy, count, along_x, office_first):
        length = count * room_w
        if along_x:
            x0, x1, y0, y1 = ox, ox + length, oy, oy + room_d
            front = Frame(x0, y0, 1, 0, 0, -1)
        else:
            x0, x1, y0, y1 = ox, ox + room_d, oy, oy + length
            front = Frame(x1, y0, 0, 1, 1, 0)
        m.box("concrete", x0 - 2, x1 + 2, y0 - 2, y1 + 2, -60.0, fh, skip="z")
        openings, units = [], []
        for i in range(count):
            s = i * room_w
            if office_first and i == 0:
                openings += [(s + 40.0, s + 240.0, fh + 60.0, fh + 225.0), (s + 275.0, s + 375.0, fh, fh + 208.0)]
                units.append(("office", s + 40.0, s + 240.0, s + 275.0, s + 375.0))
            else:
                openings += [(s + 40.0, s + 140.0, fh, fh + 208.0), (s + 185.0, s + 360.0, fh + 85.0, fh + 215.0)]
                units.append(("room", s + 185.0, s + 360.0, s + 40.0, s + 140.0))
        a.wall(m, "plaster_ochre" if not office_first or True else "plaster", front, length, fh, fh + h, openings, reveal=12.0, reveal_key="trim", base_z=fh)
        f = frames(x0, x1, y0, y1)
        for side in ("front", "back", "left", "right"):
            fr = f[side]
            if abs(fr.n[0] - front.n[0]) < 0.01 and abs(fr.n[1] - front.n[1]) < 0.01:
                continue
            ln = (x1 - x0) if side in ("front", "back") else (y1 - y0)
            back_windows = [(s0, s1, fh + 150.0, fh + 210.0) for s0, s1 in bays(ln, max(1, int(ln / room_w)), 70.0)] if ln > room_d + 10 else []
            a.wall(m, "plaster_ochre", fr, ln, fh, fh + h, back_windows, reveal=12.0, base_z=fh)
            for s0, s1, zb, zt in back_windows:
                a.window(m, fr, s0, s1, zb, zt, rng.random(), casing=0.0, sill=False, mullions=(0, 0))
        for kind, w0, w1, d0, d1 in units:
            a.window(m, front, w0, w1, fh + (60.0 if kind == "office" else 85.0), fh + (225.0 if kind == "office" else 215.0), rng.random(), glass="shopwindow" if kind == "office" else "window", mullions=(1, 0))
            a.door(m, front, d0, d1, fh, fh + 208.0, "metal_green" if kind == "room" else "trim_dark", glazed=kind == "office", rnd=0.95)
            px, py, _ = front.p((d0 + d1) / 2, 30.0, 0)
            lot.numbers.append((px, py, fh + 170.0, front))
            front.box(m, "lamp", d1 + 14.0, d1 + 26.0, 0, 9.0, fh + 180.0, fh + 198.0, skip="t")
            lot.lights.append(front.p(d1 + 20.0, 60.0, fh + 185.0))
        # covered walkway
        wd = 190.0
        front.box(m, "concrete", -2.0, length + 2.0, 0, wd, -30.0, fh - 1.0, skip="z")
        for i in range(count + 1):
            s = min(max(i * room_w, 8.0), length - 8.0)
            front.box(m, "trim", s - 6.0, s + 6.0, wd - 16.0, wd - 4.0, fh - 1.0, fh + 232.0)
        front.box(m, "trim", 0.0, length, wd - 18.0, wd - 2.0, fh + 232.0, fh + 250.0)
        # roof: gable over the wing with the front eave reaching over the walkway
        if along_x:
            a.gable_roof(m, x0, x1, y0 - wd + 20.0, y1, fh + h, 0.30, eave=40.0, key="shingles_brown")
            for x, sign in ((x0, -1.0), (x1, 1.0)):
                a.gable_end(m, "plaster_ochre", x, y0 - wd + 20.0, y1, fh + h, 0.30, sign, vent=False)
        else:
            a.gable_roof(m, y0, y1, x0, x1 + wd - 20.0, fh + h, 0.30, eave=40.0, key="shingles_brown", axis="y")
            for y, sign in ((y0, -1.0), (y1, 1.0)):
                a.gable_end(m, "plaster_ochre", y, x0, x1 + wd - 20.0, fh + h, 0.30, sign, axis="y", vent=False)
    wing(-W / 2 + 700.0, D - 720.0, 9, True, False)
    wing(-W / 2, D - 720.0 - 4 * room_w + 0.0, 4, False, True)
    # sign at the road
    sx, sy = W / 2 - 300.0, road_reach(lot) + 280.0
    m.box("steel", sx - 12, sx + 12, sy - 12, sy + 12, -30.0, 560.0)
    m.box("metal_blue", sx - 170, sx + 170, sy - 18, sy + 18, 560.0, 760.0)
    m.box("neon_red", sx - 150, sx + 150, sy - 19.5, sy + 19.5, 572.0, 580.0)
    m.box("neon_red", sx - 150, sx + 150, sy - 19.5, sy + 19.5, 740.0, 748.0)
    lot.pole_sign = (sx, sy - 21.0, 665.0)
    for i in range(9):                                                                         # parking bay lines
        x = -W / 2 + 700.0 + i * room_w
        drape(m, "mark_white", lot, terrain, x - 5.0, x + 5.0, D - 720.0 - 190.0 - 520.0, D - 720.0 - 190.0 - 30.0, step=260.0, lift=6.2)
    return m


def church(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D = 900.0, 1700.0
    fh, zt, pitch = 60.0, 560.0, 1.0
    x0, x1, y0 = -W / 2, W / 2, 320.0
    f = frames(x0, x1, y0, y0 + D)
    m.box("concrete", x0 - 4, x1 + 4, y0 - 4, y0 + D + 4, -80.0, fh, skip="z")
    side_open = [(s0, s1, fh + 110.0, fh + 400.0) for s0, s1 in bays(D, 5, 90.0, 60.0)]
    for side, length, op in (("front", W, []), ("right", D, side_open), ("back", W, [(W / 2 - 60, W / 2 + 60, fh + 200.0, fh + 420.0)]), ("left", D, side_open)):
        a.wall(m, "siding_white", f[side], length, fh, zt, op, reveal=10.0, reveal_key="trim", base_z=fh)
        for s0, s1, zb, ztop in op:
            a.window(m, f[side], s0, s1, zb, ztop, rng.random() * 0.4, mullions=(1, 3))
    zr = a.gable_roof(m, y0, y0 + D, x0, x1, zt, pitch, key="shingles", axis="y", eave=35.0)
    a.gable_end(m, "siding_white", y0, x0, x1, zt, pitch, -1.0, axis="y")
    a.gable_end(m, "siding_white", y0 + D, x0, x1, zt, pitch, 1.0, axis="y")
    # tower with the entrance, belfry and spire
    t = 190.0
    tf = frames(-t, t, 0.0, y0 + 40.0)
    tz = 1250.0
    m.box("concrete", -t - 4, t + 4, -4.0, y0, -80.0, fh, skip="z")
    a.wall(m, "siding_white", tf["front"], 2 * t, fh, tz, [(t - 75.0, t + 75.0, fh, fh + 250.0), (t - 40.0, t + 40.0, 900.0, 1100.0)], reveal=12.0, reveal_key="trim", base_z=fh)
    a.door(m, tf["front"], t - 75.0, t + 75.0, fh, fh + 250.0, "planks_dark", inset=10.0)
    for side in ("left", "right"):
        a.wall(m, "siding_white", tf[side], y0 + 40.0, fh, tz, [(140.0, 220.0, 900.0, 1100.0)], reveal=12.0, base_z=fh)
        tf[side].box(m, "trim_dark", 140.0, 220.0, -14.0, -12.0, 900.0, 1100.0, skip="t")
    a.wall(m, "siding_white", tf["back"], 2 * t, zt, tz, [], grime=False)
    tf["front"].box(m, "trim_dark", t - 40.0, t + 40.0, -14.0, -12.0, 900.0, 1100.0, skip="t")
    m.box("trim", -t - 14, t + 14, -14.0, y0 + 54.0, tz, tz + 16.0)
    apex = (0.0, (y0 + 40.0) / 2, tz + 16.0 + 620.0)
    corners = [(-t - 6, -6.0, tz + 16.0), (t + 6, -6.0, tz + 16.0), (t + 6, y0 + 46.0, tz + 16.0), (-t - 6, y0 + 46.0, tz + 16.0)]
    for i in range(4):
        p, q = corners[i], corners[(i + 1) % 4]
        mid = ((p[0] + q[0]) / 2 - apex[0], (p[1] + q[1]) / 2 - apex[1], 0.4)
        m.poly("shingles", [p, q, apex], None, [(0, 700), (2 * t, 700), (t, 0)], hint=mid)
    m.box("steel", -3, 3, apex[1] - 3, apex[1] + 3, apex[2] - 20.0, apex[2] + 130.0)
    m.box("steel", -30, 30, apex[1] - 3, apex[1] + 3, apex[2] + 70.0, apex[2] + 76.0)
    run = a.steps(m, "concrete", tf["front"], t - 120.0, t + 120.0, fh, t0=0.0)
    drape(m, "concrete", lot, terrain, -70.0, 70.0, road_reach(lot), -run, step=150.0)
    lot.lamp = (120.0, -30.0, fh + 260.0)
    # graveyard beside the church: rows of weathered stones (kept as part of the building mesh)
    for row in range(4):
        for col in range(7):
            if rng.random() < 0.2:
                continue
            gx, gy = x1 + 380.0 + col * 190.0 + rng.uniform(-25, 25), y0 + 200.0 + row * 330.0 + rng.uniform(-30, 30)
            wx, wy = lot.to_world(gx, gy)
            gz = terrain.z(wx, wy) - lot.z
            sub = meshkit.Mesh()
            w, hgt = rng.uniform(22, 34), rng.uniform(55, 105)
            sub.box("rock", -w, w, -6.0, 6.0, -25.0, hgt)
            if rng.random() < 0.5:
                sub.box("rock", -w * 0.7, w * 0.7, -6.0, 6.0, hgt, hgt + 10.0)
            m.add(sub, gx, gy, gz, rng.uniform(-9, 9), rename={"rock": "concrete" if rng.random() < 0.5 else "rock"})
    lot.fence = (x1 + 250.0, y0 + 60.0, x1 + 250.0 + 7 * 190.0 + 150.0, y0 + 4 * 330.0 + 100.0)
    return m


def cabin(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D, fh, zt = 820.0, 620.0, 50.0, 300.0
    x0, x1 = -W / 2, W / 2
    f = frames(x0, x1, 0.0, D)
    for x in (x0 + 20, 0.0, x1 - 20):
        for y in (20.0, D - 20.0):
            m.box("rock", x - 22, x + 22, y - 22, y + 22, -60.0, fh)
    op = {"front": [(120.0, 230.0, fh + 95.0, fh + 205.0), (360.0, 460.0, fh, fh + 205.0), (590.0, 700.0, fh + 95.0, fh + 205.0)], "left": [(250.0, 370.0, fh + 95.0, fh + 205.0)], "right": [], "back": [(350.0, 470.0, fh + 95.0, fh + 205.0)]}
    for side, length in (("front", W), ("right", D), ("back", W), ("left", D)):
        a.wall(m, "planks_dark", f[side], length, fh, zt, op[side], thick=20.0, inner_key="planks", reveal_key="planks", base_z=fh)
        for s0, s1, zb, ztop in op[side]:
            if ztop - zb < 150:
                a.window(m, f[side], s0, s1, zb, ztop, rng.random(), glass="glass", trim="planks", inset=9.0)
    # the one building with an interior: a single room behind a real door (ASpnTestDoor), with a switchable lamp
    m.box("wood_floor", x0 + 20, x1 - 20, 20.0, D - 20.0, fh - 14.0, fh, skip="xXyY")
    m.quad("planks", (x0 + 20, 20.0, zt), (x1 - 20, 20.0, zt), (x1 - 20, D - 20.0, zt), (x0 + 20, D - 20.0, zt), (0, 0, -1))
    m.box("carpet", -150.0, 150.0, 200.0, 420.0, fh, fh + 1.2, skip="z")                        # rug
    m.box("planks", -70.0, 70.0, 250.0, 350.0, fh + 70.0, fh + 75.0)                            # table
    for tx, ty in ((-62, 258), (62, 258), (-62, 342), (62, 342)):
        m.box("planks", tx - 4, tx + 4, ty - 4, ty + 4, fh, fh + 70.0, skip="zZ")
    for cx in (-120.0, 120.0):                                                                 # two stools
        m.cyl("planks_dark", cx, 300.0, fh, fh + 45.0, 17.0, segs=10)
    m.box("planks_dark", x0 + 24, x0 + 120, D - 230.0, D - 24.0, fh, fh + 38.0)                 # cot with a blanket
    m.box("carpet", x0 + 26, x0 + 118, D - 228.0, D - 26.0, fh + 38.0, fh + 50.0)
    m.box("planks_dark", x0 + 200, x0 + 330, D - 55.0, D - 22.0, fh, fh + 190.0)                # shelf
    for z in (40.0, 95.0, 150.0):
        m.box("planks", x0 + 198, x0 + 332, D - 60.0, D - 22.0, fh + z, fh + z + 4.0)
    m.box("rock", x1 - 70, x1 - 20, D / 2 - 90, D / 2 + 90, fh, fh + 150.0)                     # fireplace
    m.box("trim_dark", x1 - 72, x1 - 68, D / 2 - 50, D / 2 + 50, fh + 5.0, fh + 95.0)
    m.box("planks", x1 - 82, x1 - 20, D / 2 - 100, D / 2 + 100, fh + 150.0, fh + 158.0)
    m.cyl("metal_dark", x1 - 180.0, 90.0, fh, fh + 70.0, 28.0, segs=12)                         # stove
    m.cyl("metal_dark", x1 - 180.0, 90.0, fh + 70.0, zt, 7.0, segs=8, caps=False)
    m.box("lamp", -9.0, 9.0, 291.0, 309.0, zt - 60.0, zt - 40.0)
    m.box("metal_dark", -1.0, 1.0, 299.0, 301.0, zt - 40.0, zt)
    lot.interior = {"door": (x0 + 360.0, 10.0, fh, "cabin_door", "planks"), "lamp": (0.0, 300.0, zt - 75.0), "switch": (x0 + 490.0, 21.5, fh + 125.0),
                    "room": (x0 + 20.0, x1 - 20.0, 20.0, D - 20.0, fh, zt)}    # x0, x1, y0, y1, z0, z1 of the room (inner wall faces): its own exposure range
    # (y0 was 40 until 2026-11-13: a third-person camera pressed against the inside of the front wall stood in the 20 cm gap and got the
    # outdoor exposure; found by SpnWorldQA.Integration)
    zr = a.gable_roof(m, x0, x1, -230.0, D, zt, 0.62, key="shingles_green", trim="planks", eave=40.0)
    for x, sign in ((x0, -1.0), (x1, 1.0)):
        a.gable_end(m, "planks_dark", x, -230.0, D, zt, 0.62, sign, vent=False)
    f["front"].box(m, "planks", 0, W, 0, 230.0, fh - 12.0, fh - 2.0)
    for s in (8.0, W / 2 - 110.0, W / 2 + 110.0, W - 8.0):
        f["front"].box(m, "planks", s - 7, s + 7, 208.0, 222.0, -30.0, zt - 20.0)
    a.railing(m, "planks", f["front"], 15.0, W / 2 - 117.0, 215.0, fh - 2.0)
    a.railing(m, "planks", f["front"], W / 2 + 117.0, W - 15.0, 215.0, fh - 2.0)
    a.steps(m, "planks", f["front"], W / 2 - 100.0, W / 2 + 100.0, fh - 2.0, t0=230.0)
    drape(m, "dirt", lot, terrain, -80.0, 80.0, road_reach(lot), -300.0, step=120.0, lift=3.0)     # trodden path from the track to the steps
    m.box("rock", x1 - 10, x1 + 90, D / 2 - 70, D / 2 + 70, -60.0, zr + 90.0)                  # stone chimney
    m.box("lamp", x0 + 470.0, x0 + 482.0, -9.0, 0.0, fh + 180.0, fh + 198.0, skip="Y")
    lot.lamp = (x0 + 476.0, -24.0, fh + 185.0)
    m.box("planks", x0 - 95.0, x0 - 20.0, 60.0, 330.0, 0.0, 95.0)                              # wood pile
    return m


def barn(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D, zt = 1100.0, 1700.0, 520.0
    x0, x1 = -W / 2, W / 2
    f = frames(x0, x1, 0.0, D)
    m.box("concrete", x0 - 3, x1 + 3, -3.0, D + 3, -60.0, 30.0, skip="z")
    op = {"front": [(W / 2 - 190.0, W / 2 + 190.0, 30.0, 400.0)], "back": [(W / 2 - 150.0, W / 2 + 150.0, 30.0, 360.0)], "left": [(s0, s1, 150.0, 240.0) for s0, s1 in bays(D, 4, 90.0)], "right": [(s0, s1, 150.0, 240.0) for s0, s1 in bays(D, 4, 90.0)]}
    for side, length in (("front", W), ("right", D), ("back", W), ("left", D)):
        a.wall(m, "planks_red", f[side], length, 30.0, zt, op[side], reveal=10.0, reveal_key="trim", base_z=30.0)
        for s in (0.0, length - 12.0):
            f[side].box(m, "trim", s, s + 12.0, 0, 2.5, 30.0, zt, skip="tzZ")
        for s0, s1, zb, ztop in op[side]:
            if ztop - zb < 120:
                a.window(m, f[side], s0, s1, zb, ztop, 0.1, mullions=(1, 0))
    for side, half_w, top in (("front", 190.0, 400.0), ("back", 150.0, 360.0)):                # big doors with cross bracing
        fr = f[side]
        for s0, s1 in ((W / 2 - half_w, W / 2 - 2.0), (W / 2 + 2.0, W / 2 + half_w)):
            fr.box(m, "planks_red", s0, s1, -8.0, -3.0, 30.0, top, skip="t")
            for za, zb_ in ((30.0, 44.0), (top - 14.0, top), ((30.0 + top) / 2 - 7.0, (30.0 + top) / 2 + 7.0)):
                fr.box(m, "trim", s0, s1, -3.0, -1.0, za, zb_, skip="t")
            for s in (s0, s1 - 14.0):
                fr.box(m, "trim", s, s + 14.0, -3.0, -1.0, 30.0, top, skip="t")
    zr = a.gable_roof(m, 0.0, D, x0, x1, zt, 0.85, key="corrugated", axis="y", eave=45.0, rake=45.0)
    a.gable_end(m, "planks_red", 0.0, x0, x1, zt, 0.85, -1.0, axis="y")
    a.gable_end(m, "planks_red", D, x0, x1, zt, 0.85, 1.0, axis="y")
    # silo and a lean-to shed
    sx, sy = x1 + 420.0, 420.0
    m.cyl("corrugated", sx, sy, -40.0, 1150.0, 230.0, segs=24, caps=False)
    m.cyl("steel", sx, sy, 1150.0, 1290.0, 236.0, 20.0, segs=24, caps=False)
    sh = frames(x0 - 520.0, x0, 300.0, 1100.0)
    for side, length in (("left", 800.0), ("back", 520.0)):
        a.wall(m, "corrugated", sh[side], length, 0.0, 260.0, [], grime=False)
    for y in (300.0, 700.0, 1100.0):
        m.box("planks", x0 - 520.0, x0 - 508.0, y - 6, y + 6, 0.0, 260.0)
    m.quad("corrugated", (x0 - 560.0, 280.0, 250.0), (x0, 280.0, 340.0), (x0, 1120.0, 340.0), (x0 - 560.0, 1120.0, 250.0), None, [(0, 0), (570, 0), (570, 840), (0, 840)], hint=(0, 0, 1))
    m.quad("corrugated", (x0 - 560.0, 280.0, 247.0), (x0, 280.0, 337.0), (x0, 1120.0, 337.0), (x0 - 560.0, 1120.0, 247.0), None, None, hint=(0, 0, -1))
    drape(m, "gravel", lot, terrain, -350.0, 350.0, road_reach(lot), 0.0, step=220.0)
    lot.lamp = (0.0, -30.0, 430.0)
    return m


def watertower(lot, terrain, rng):
    m = meshkit.Mesh()
    legs, radius, base, tank = 6, 420.0, 1900.0, 620.0
    pts = [(math.cos(2 * math.pi * i / legs) * radius, math.sin(2 * math.pi * i / legs) * radius) for i in range(legs)]
    for i, (x, y) in enumerate(pts):
        m.box("concrete", x * 1.25 - 45, x * 1.25 + 45, y * 1.25 - 45, y * 1.25 + 45, -80.0, 25.0)
        m.beam("steel", (x * 1.25, y * 1.25, 25.0), (x * 0.92, y * 0.92, base), 24.0)
        nx, ny = pts[(i + 1) % legs]
        for level in (0.33, 0.66, 1.0):
            sa, sb = 1.25 - 0.33 * level, 1.25 - 0.33 * level
            m.beam("steel", (x * sa, y * sa, 25.0 + (base - 25.0) * level), (nx * sb, ny * sb, 25.0 + (base - 25.0) * level), 10.0)
        m.beam("steel", (x * 1.25, y * 1.25, 25.0), (nx * 1.14, ny * 1.14, 25.0 + (base - 25.0) * 0.33), 6.0)
        m.beam("steel", (x * 1.14, y * 1.14, 25.0 + (base - 25.0) * 0.33), (nx * 1.03, ny * 1.03, 25.0 + (base - 25.0) * 0.66), 6.0)
    m.cyl("metal_white", 0.0, 0.0, base - 60.0, base, 250.0, 470.0, segs=28)
    m.cyl("metal_white", 0.0, 0.0, base, base + tank, 470.0, segs=28, caps=False)
    m.cyl("metal_white", 0.0, 0.0, base + tank, base + tank + 190.0, 480.0, 12.0, segs=28, caps=False)
    m.cyl("steel", 0.0, 0.0, base + 40.0, base + 52.0, 520.0, segs=28)                          # catwalk
    m.cyl("steel", 0.0, 0.0, 0.0, base - 60.0, 45.0, segs=12, caps=False)                      # riser pipe
    lot.sign = (0.0, -472.0, base + tank * 0.5, 520.0)
    return m


def diner(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D, fh, zt = 1900.0, 760.0, 40.0, 350.0
    x0, x1 = -W / 2, W / 2
    drape(m, "asphalt_old", lot, terrain, -lot.width / 2 - 300.0, lot.width / 2 + 300.0, road_reach(lot), -160.0, step=220.0)
    m.box("concrete", x0 - 4, x1 + 4, -4.0, D + 4, -60.0, fh, skip="z")
    wins = [(s0, s1, fh + 80.0, fh + 215.0) for s0, s1 in bays(W, 8, 185.0, 30.0)]
    door = wins.pop(4)
    front_open = wins + [(door[0] + 35.0, door[1] - 35.0, fh, fh + 215.0)]
    f = simple_block(m, x0, x1, 0.0, D, fh, zt, "metal_white", {"front": front_open, "left": [(200.0, 520.0, fh + 80.0, fh + 215.0)], "right": [(200.0, 520.0, fh + 80.0, fh + 215.0)]}, reveal=10.0, reveal_key="steel")
    for s0, s1, zb, ztop in wins:
        a.window(m, f["front"], s0, s1, zb, ztop, 0.6 + rng.random() * 0.4, glass="shopwindow", trim="steel", casing=0.0, sill=False, mullions=(0, 0))
    for side in ("left", "right"):
        a.window(m, f[side], 200.0, 520.0, fh + 80.0, fh + 215.0, 0.9, glass="shopwindow", trim="steel", casing=0.0, sill=False, mullions=(1, 0))
    a.door(m, f["front"], door[0] + 35.0, door[1] - 35.0, fh, fh + 215.0, "steel", trim="steel", glazed=True, rnd=0.95, casing=0.0)
    for fr, length in ((f["front"], W), (f["left"], D), (f["right"], D)):
        fr.box(m, "metal_red", 0, length, 0, 3.0, fh + 30.0, fh + 62.0, skip="t")
        fr.box(m, "steel", 0, length, 0, 4.0, fh + 228.0, fh + 240.0, skip="t")
        fr.box(m, "metal_red", 0, length, 0, 3.0, zt - 40.0, zt - 12.0, skip="t")
    m.box("metal_white", x0 - 35, x1 + 35, -55.0, D + 20, zt, zt + 16.0)
    m.box("steel", x0 - 38, x1 + 38, -58.0, D + 23, zt + 16.0, zt + 22.0, skip="z")
    a.steps(m, "concrete", f["front"], (door[0] + door[1]) / 2 - 90.0, (door[0] + door[1]) / 2 + 90.0, fh, t0=0.0)
    m.box("metal_red", -320.0, 320.0, 120.0, 150.0, zt + 22.0, zt + 150.0)                     # roof sign board
    m.box("neon_red", -300.0, 300.0, 117.0, 120.0, zt + 34.0, zt + 40.0)
    lot.sign = (0.0, 116.0, zt + 92.0, 520.0)
    lot.lights = [(-500.0, -80.0, zt - 30.0), (500.0, -80.0, zt - 30.0)]
    return m


def garage(lot, terrain, rng):
    m = meshkit.Mesh()
    W, D, zt = 1700.0, 1000.0, 470.0
    x0, x1 = -W / 2, W / 2
    drape(m, "concrete", lot, terrain, x0 - 200.0, x1 + 200.0, road_reach(lot), 0.0, step=220.0)
    front_open = [(90.0, 470.0, 4.0, 350.0), (560.0, 940.0, 4.0, 350.0), (1090.0, 1190.0, 4.0, 212.0), (1260.0, 1560.0, 95.0, 230.0)]
    f = simple_block(m, x0, x1, 0.0, D, 0.0, zt, "plaster_grey", {"front": front_open}, reveal=18.0, reveal_key="concrete")
    for s0, s1, zb, ztop in front_open[:2]:
        f["front"].box(m, "corrugated", s0, s1, -14.0, -10.0, zb, ztop, skip="t")
    a.door(m, f["front"], 1090.0, 1190.0, 4.0, 212.0, "metal_blue", trim="trim_dark", casing=0.0)
    a.window(m, f["front"], 1260.0, 1560.0, 95.0, 230.0, 0.8, glass="shopwindow", trim="trim_dark", casing=0.0, sill=False, mullions=(1, 0))
    f["front"].box(m, "metal_blue", 30.0, W - 30.0, 0, 6.0, 380.0, 440.0, skip="t")
    lot.sign = (0.0, -8.0, 410.0, 700.0)
    m.quad("gravel", (x0, 0, zt - 60.0), (x1, 0, zt - 60.0), (x1, D, zt - 60.0), (x0, D, zt - 60.0), (0, 0, 1))
    for fr, length in ((f["front"], W), (f["left"], D), (f["right"], D), (f["back"], W)):
        fr.box(m, "concrete", 0, length, -20.0, 2.0, zt, zt + 7.0)
        m.quad("plaster_grey", fr.p(0, -20.0, zt - 60.0), fr.p(length, -20.0, zt - 60.0), fr.p(length, -20.0, zt), fr.p(0, -20.0, zt), fr.vec(0, -1, 0))
    for i in range(4):                                                                         # stacked tyres beside the wall
        m.cyl("trim_dark", x1 + 90.0, 150.0, i * 22.0, i * 22.0 + 20.0, 33.0, segs=12)
    lot.lights = [(-560.0, -60.0, 400.0), (-90.0, -60.0, 400.0)]
    return m


GENERATORS = {"house": house, "shop": shop, "gas": gas, "motel": motel, "church": church, "cabin": cabin, "barn": barn, "watertower": watertower, "diner": diner, "garage": garage}


def generate(lot, roads, terrain):
    lot.road_half = roads[lot.road].half if lot.road != "hwy" else 560.0 if lot.kind in ("shop", "diner", "garage") else roads["hwy"].half
    if lot.kind == "shop":
        lot.road_half = 560.0 + 290.0      # the pavement lies between the road and the shop front
    rng = random.Random(lot.params.get("seed", zlib.crc32(lot.name.encode()) & 0xFFFF))
    return GENERATORS[lot.kind](lot, terrain, rng)


def paved_rects(roads, lots):
    """Drives, paths and parking areas of all lots as world rectangles, for the terrain layers (no grass below them).

    Runs the generators once on flat ground: the plan position of these surfaces does not depend on the terrain."""
    class Flat:
        def z(self, x, y):
            return 0.0
    PAVED.clear()
    for lot in lots:
        lot.z = 0.0             # overwritten with the real height once the terrain exists
        generate(lot, roads, Flat())
    rects = list(PAVED)
    PAVED.clear()
    return rects


def build(ctx):
    roads, terrain, mats = ctx["roads"], ctx["terrain"], ctx["mats"]
    if "shopwindow" not in mats:
        mats["shopwindow"] = k.instance("MI_Spn_shopwindow", mats["window"], {"CurtainAmount": 0.0, "LitThreshold": 0.35, "GlowStrength": 0.9})
    triangles = 0
    decor_doors = []
    for lot in ctx["lots"]:
        mesh = generate(lot, roads, terrain)
        triangles += mesh.triangle_count()
        for index, mark in enumerate(getattr(mesh, "door_marks", [])):
            cx, cy, cz = mark["centre"]
            wx, wy = lot.to_world(cx, cy)
            ox, oy = lot.to_world(0.0, 0.0)
            nx, ny = lot.to_world(mark["normal"][0], mark["normal"][1])
            decor_doors.append({"id": "%s_door%d" % (lot.name, index), "x": round(wx, 1), "y": round(wy, 1), "z": round(lot.z + cz, 1),
                                "half_width": round(mark["half_width"], 1), "height": round(mark["height"], 1),
                                "nx": round(nx - ox, 4), "ny": round(ny - oy, 4)})
        asset = k.static_mesh(mesh, "SM_Bld_" + lot.name, mats)
        k.place(asset, lot.fx, lot.fy, lot.z, lot.yaw, "SPN_Bld_" + lot.name, folder="Buildings")
        interior = getattr(lot, "interior", None)
        if interior:
            cls = "/Script/SupernaturalProt."
            lx, ly, lz, stable_id, leaf = interior["door"]
            wx, wy = lot.to_world(lx, ly)
            door = k.spawn(unreal.load_class(None, cls + "SpnTestDoor"), wx, wy, lot.z + lz, lot.yaw, "SPN_Door_" + stable_id, folder="Buildings")
            for key, value in (("stable_id", stable_id), ("leaf_material", mats[leaf]), ("hardware_material", mats["steel"]), ("show_handle", True), ("show_hinges", True), ("leaf_thickness", 6.0)):
                door.set_editor_property(key, value)
            for key, value in interior.get("door_opts", {}).items():       # gameplay route: lock and key (gameplay.HOUSES)
                door.set_editor_property(key, value)
            if ctx["has"]("G"):
                # the room meters its own exposure (ASpnWorldLighting sets the range per scenario): the daylight range outside leaves it black
                rx0, rx1, ry0, ry1, rz0, rz1 = interior["room"]
                wx, wy = lot.to_world((rx0 + rx1) / 2, (ry0 + ry1) / 2)
                room = k.spawn(unreal.PostProcessVolume, wx, wy, lot.z + (rz0 + rz1) / 2, lot.yaw, "SPN_PostInterior_" + lot.name, folder="Buildings")
                room.set_actor_scale3d(unreal.Vector((rx1 - rx0) / 200.0, (ry1 - ry0) / 200.0, (rz1 - rz0) / 200.0))   # the default brush is a 200 cm cube
                room.set_editor_property("tags", ["SpnPostInterior"])
                room.set_editor_property("priority", 1.0)
                room.set_editor_property("blend_radius", 80.0)
                settings = room.get_editor_property("settings")
                for key, value in (("override_auto_exposure_min_brightness", True), ("auto_exposure_min_brightness", 7.0), ("override_auto_exposure_max_brightness", True), ("auto_exposure_max_brightness", 11.0)):
                    settings.set_editor_property(key, value)
                room.set_editor_property("settings", settings)
                extent = room.get_actor_bounds(False)[1]
                k.log("interior volume %s: half extent %.0f x %.0f x %.0f cm" % (lot.name, extent.x, extent.y, extent.z))
                if min(extent.x, extent.y, extent.z) < 50.0:
                    k.fail("interior post process volume of %s has no brush" % lot.name)
            lx, ly, lz = interior["lamp"]
            wx, wy = lot.to_world(lx, ly)
            lamp = k.spawn(unreal.PointLight, wx, wy, lot.z + lz, label="SPN_Lamp_" + lot.name, folder="Lights")
            c = lamp.get_component_by_class(unreal.PointLightComponent)
            c.set_mobility(unreal.ComponentMobility.MOVABLE)
            c.set_editor_property("intensity_units", unreal.LightUnits.CANDELAS)
            c.set_editor_property("intensity", 400.0)
            c.set_editor_property("attenuation_radius", 900.0)
            c.set_editor_property("light_color", unreal.Color(r=255, g=190, b=120, a=255))
            lx, ly, lz = interior["switch"]
            wx, wy = lot.to_world(lx, ly)
            switch = k.spawn(unreal.load_class(None, cls + "SpnLightSwitch"), wx, wy, lot.z + lz, lot.yaw + 90.0, "SPN_Switch_" + lot.name, folder="Buildings")
            switch.set_editor_property("stable_id", lot.name + "_light")
            switch.set_editor_property("lights", [lamp])
    k.log("buildings: %d, %d triangles" % (len(ctx["lots"]), triangles))
    write_decor_doors(decor_doors)


DECOR_DOORS = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "Content", "SpnWorld", "Data", "SpnWorldDecorDoors.json"))


def write_decor_doors(doors, path=DECOR_DOORS):
    """Doors that are part of a building mesh (no ASpnTestDoor): where the game answers an attempt to open them. Deterministic (sorted ids).
    Unreal centimetres; normal = the facade's outward direction. PLACEHOLDER data of the generated test world."""
    doors = sorted(doors, key=lambda d: d["id"])
    data = {"version": 1, "placeholder": True, "source": "Tools/spn_world/buildings.py (archkit.door)", "doors": doors}
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(json.dumps(data, indent=1, sort_keys=True) + "\n")
    k.log("decorative doors: %d -> %s" % (len(doors), path))
