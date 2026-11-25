"""Builds the placeholder template of the 1.5 x 1.5 km test block in the open Blender scene.

Placeholder geometry only (no materials, LODs or real geography). Coordinates are in metres.
Sketch coordinates (x right, y down, 0..1500) are converted to Blender (y up) with fy().

Run inside Blender (MCP execute or `blender -b -P build_testblock.py`). It only removes the
objects of its own collections (TEMPLATE) and the camera/sun it creates, nothing else.
NOT yet verified as a standalone script; the scene was built with the same steps via MCP.
"""
import bpy
import math
import os
import random

random.seed(7)
W = 1500
scene = bpy.context.scene
OUT_BLEND = os.path.join(os.path.dirname(__file__), "..", "..", "Art", "Source", "Testblock", "testblock_template.blend")
TEMPLATE = ["00_Ground", "01_Roads", "02_Buildings", "03_Parks_Water", "04_Trees", "05_Cars", "06_NPCs"]


def fy(y):
    return W - y


def link(obj, coll):
    for col in list(obj.users_collection):
        col.objects.unlink(obj)
    coll.objects.link(obj)
    return obj


def mkcoll(name):
    c = bpy.data.collections.new(name)
    scene.collection.children.link(c)
    return c


def box(name, x, y, w, h, z0, z1, coll, color=None):
    """Box in Blender coordinates, (x, y) = bottom-left corner."""
    bpy.ops.mesh.primitive_cube_add(size=1, location=(x + w / 2, y + h / 2, (z0 + z1) / 2))
    o = bpy.context.active_object
    o.name = name
    o.scale = (w, h, z1 - z0)
    bpy.ops.object.transform_apply(scale=True)
    if color:
        o.color = color
    return link(o, coll)


def rect_s(name, x, y, w, h, z0, z1, coll, color=None):
    """Box in sketch coordinates, (x, y) = top-left corner."""
    return box(name, x, fy(y + h), w, h, z0, z1, coll, color)


def dist_seg(px, py, ax, ay, bx, by):
    dx, dy = bx - ax, by - ay
    l2 = dx * dx + dy * dy or 1e-9
    t = max(0, min(1, ((px - ax) * dx + (py - ay) * dy) / l2))
    return math.hypot(px - (ax + t * dx), py - (ay + t * dy))


def overlaps(a, b, pad=0):
    ax, ay, aw, ah = a
    bx, by, bw, bh = b
    return not (ax + aw + pad < bx or bx + bw + pad < ax or ay + ah + pad < by or by + bh + pad < ay)


# --- clear only this template -------------------------------------------------------------
for name in TEMPLATE:
    c = bpy.data.collections.get(name)
    if c:
        for o in list(c.objects):
            bpy.data.objects.remove(o, do_unlink=True)
        scene.collection.children.unlink(c)
        bpy.data.collections.remove(c)
for n in ["Cam_Top", "Cam_Top_v6", "Sun"]:
    o = bpy.data.objects.get(n)
    if o:
        bpy.data.objects.remove(o, do_unlink=True)

c_ground, c_roads, c_blocks, c_parks, c_trees, c_cars, c_npcs = [mkcoll(n) for n in TEMPLATE]
ROAD = (0.42, 0.41, 0.39, 1)
ROOF = (0.78, 0.45, 0.35, 1)
WALLS = (0.85, 0.74, 0.60, 1)
WATER = (0.45, 0.70, 0.78, 1)
GRASS = (0.70, 0.82, 0.55, 1)

rect_s("Ground", 0, 0, W, W, -0.2, 0, c_ground, (0.88, 0.85, 0.78, 1))

# --- areas from the reference image (sketch coordinates) -----------------------------------
PARK = (0, 0, 510, 480)
PARKING = (1110, 630, 180, 210)
NE_LM = (1170, 15, 285, 240)
E_LM = (1320, 615, 135, 225)
SW_LM = (30, 1275, 375, 195)
LANDMARKS = [NE_LM, E_LM, SW_LM]
PLAYGROUND = (300, 350, 70, 60)
ROUND = (735, 1245, 45)
POND = (110, 380, 45)
CREEK = [(1500, 1140), (1380, 1200), (1320, 1320), (1200, 1395), (1050, 1500)]
BRIDGE_Y = 1245            # southern east-west street crosses the creek here

rect_s("Park", *PARK, 0.02, 0.04, c_parks, GRASS)
rect_s("Playground", *PLAYGROUND, 0.04, 0.06, c_parks, (0.85, 0.75, 0.55, 1))
rect_s("Parking", *PARKING, 0.02, 0.04, c_parks, (0.50, 0.49, 0.46, 1))
bpy.ops.mesh.primitive_cylinder_add(radius=POND[2], depth=0.08, location=(POND[0], fy(POND[1]), 0.05))
link(bpy.context.active_object, c_parks).name = "Pond"
bpy.context.active_object.color = WATER


# --- creek: smooth centre line (Catmull-Rom), one continuous strip, clamped to the map edge ----
def catmull(p0, p1, p2, p3, t):
    t2, t3 = t * t, t * t * t
    return tuple(0.5 * (2 * p1[k] + (-p0[k] + p2[k]) * t + (2 * p0[k] - 5 * p1[k] + 4 * p2[k] - p3[k]) * t2
                        + (-p0[k] + 3 * p1[k] - 3 * p2[k] + p3[k]) * t3) for k in (0, 1))


pts = [CREEK[0]] + CREEK + [CREEK[-1]]
path = []
for i in range(1, len(pts) - 2):
    for s in range(40):
        path.append(catmull(pts[i - 1], pts[i], pts[i + 1], pts[i + 2], s / 40))
path.append(CREEK[-1])


def clamp(v):
    return max(0.0, min(float(W), v))


verts, faces = [], []
for k, (x, y) in enumerate(path):
    a = path[max(k - 1, 0)]
    b = path[min(k + 1, len(path) - 1)]
    dx, dy = b[0] - a[0], b[1] - a[1]
    length = math.hypot(dx, dy) or 1
    nx, ny = -dy / length, dx / length
    half = (38 + 6 * math.sin(k * 0.21)) / 2
    verts.append((clamp(x + nx * half), clamp(fy(y + ny * half)), 0.03))
    verts.append((clamp(x - nx * half), clamp(fy(y - ny * half)), 0.03))
for k in range(len(path) - 1):
    b = 2 * k
    faces.append((b, b + 1, b + 3, b + 2))
mesh = bpy.data.meshes.new("Creek_mesh")
mesh.from_pydata(verts, [], faces)
mesh.update()
creek = bpy.data.objects.new("Creek", mesh)
c_parks.objects.link(creek)
creek.color = WATER

# --- roads: full-length lines, junctions closed ---------------------------------------------------
H_LINES = [(0.18 * W, 42), (0.37 * W, 66), (0.70 * W, 42), (0.83 * W, 42)]
V_LINES = [(0.15 * W, 42, 0, 0.83 * W), (0.49 * W, 66, 0, W), (0.66 * W, 42, 0, W)]
for i, (y, w) in enumerate(H_LINES):
    rect_s(f"RoadH_{i}", 0, y - w / 2, W, w, 0, 0.05, c_roads, ROAD)
for i, (x, w, y0, y1) in enumerate(V_LINES):
    rect_s(f"RoadV_{i}", x - w / 2, y0, w, y1 - y0, 0, 0.05, c_roads, ROAD)
bpy.ops.mesh.primitive_cylinder_add(radius=ROUND[2], depth=0.06, location=(ROUND[0], fy(ROUND[1]), 0.03))
link(bpy.context.active_object, c_roads).color = ROAD
bpy.ops.mesh.primitive_cylinder_add(radius=ROUND[2] / 2, depth=0.1, location=(ROUND[0], fy(ROUND[1]), 0.06))
link(bpy.context.active_object, c_parks).color = GRASS

# bridge over the creek at the crossing with the southern street
idx = min(range(len(path)), key=lambda i: abs(path[i][1] - BRIDGE_Y))
xc = path[idx][0]
span0, span1 = xc - 56, xc + 56
box("Bridge_Deck", span0, fy(BRIDGE_Y + 21), span1 - span0, 42, 0.5, 0.7, c_roads, (0.55, 0.52, 0.48, 1))
box("Bridge_RailN", span0, fy(BRIDGE_Y - 21), span1 - span0, 0.6, 0.7, 1.3, c_roads, (0.70, 0.68, 0.62, 1))
box("Bridge_RailS", span0, fy(BRIDGE_Y + 21) + 41.4, span1 - span0, 0.6, 0.7, 1.3, c_roads, (0.70, 0.68, 0.62, 1))


def on_road(x, y, pad=8):
    return any(abs(y - yy) < w / 2 + pad for yy, w in H_LINES) or any(abs(x - xx) < w / 2 + pad for xx, w, _, _ in V_LINES)


# --- houses: walled box, south door, closed roof -------------------------------------------------
footprints = []
WALL, DOOR = 0.25, 1.0


def house(name, x, y, w, h, height):
    box(name + "_floor", x, y, w, h, 0, 0.1, c_blocks, (0.80, 0.72, 0.60, 1))
    mid = x + w / 2
    box(name + "_S1", x, y, mid - DOOR / 2 - x, WALL, 0, height, c_blocks, WALLS)
    box(name + "_S2", mid + DOOR / 2, y, x + w - (mid + DOOR / 2), WALL, 0, height, c_blocks, WALLS)
    box(name + "_lintel", mid - DOOR / 2, y, DOOR, WALL, 2.2, height, c_blocks, WALLS)
    box(name + "_N", x, y + h - WALL, w, WALL, 0, height, c_blocks, WALLS)
    box(name + "_W", x, y + WALL, WALL, h - 2 * WALL, 0, height, c_blocks, WALLS)
    box(name + "_E", x + w - WALL, y + WALL, WALL, h - 2 * WALL, 0, height, c_blocks, WALLS)
    box(name + "_roof", x, y, w, h, height, height + 0.2, c_blocks, ROOF)
    footprints.append((x, y, w, h))


def near_creek(x, y, pad):
    return any(dist_seg(x, y, *CREEK[i], *CREEK[i + 1]) < pad for i in range(len(CREEK) - 1))


xs = [0] + sorted(x for x, _, _, _ in V_LINES) + [W]
ys = [0] + sorted(y for y, _ in H_LINES) + [W]
MARGIN = 40
n_houses = 0
for ci in range(len(xs) - 1):
    for cj in range(len(ys) - 1):
        x0, x1 = xs[ci] + MARGIN, xs[ci + 1] - MARGIN
        y0, y1 = ys[cj] + MARGIN, ys[cj + 1] - MARGIN
        if x1 - x0 < 40 or y1 - y0 < 40:
            continue
        y = y0
        while y + 40 <= y1:
            x = x0
            while x + 38 <= x1:
                w = min(44 + random.random() * 14, x1 - x)
                h = min(40 + random.random() * 6, y1 - y)
                if w < 38:
                    break
                area = (x, y, w, h)
                blocked = any(overlaps(area, r, 8) for r in [PARK, PARKING, *LANDMARKS])
                in_round = abs(x + w / 2 - ROUND[0]) < ROUND[2] + 60 and abs(y + h / 2 - ROUND[1]) < ROUND[2] + 60
                if blocked or in_round or near_creek(x + w / 2, y + h / 2, 70):
                    x += w + 8
                    continue
                house(f"House_{n_houses:03d}", x, fy(y + h), w, h, 3.2)
                n_houses += 1
                x += w + 8
            y += 54

for k, (lx, ly, lw, lh) in enumerate(LANDMARKS):
    house(f"Landmark_{k}", lx, fy(ly + lh), lw, lh, 6.0)


# --- trees: stem plus crown, off roads, houses, creek, roundabout and parking --------------------
def in_house(x, y):
    by = fy(y)
    return any(fx - 2 <= x <= fx + fw + 2 and fyy - 2 <= by <= fyy + fh + 2 for fx, fyy, fw, fh in footprints)


placed, attempts = 0, 0
while placed < 170 and attempts < 8000:
    attempts += 1
    if random.random() < 0.55:
        x, y = random.random() * PARK[2], random.random() * PARK[3]
    else:
        x, y = random.random() * W, random.random() * W
    if (on_road(x, y, 10) or in_house(x, y) or near_creek(x, y, 50) or overlaps((x - 3, y - 3, 6, 6), PARKING)
            or overlaps((x - 3, y - 3, 6, 6), PLAYGROUND) or math.hypot(x - ROUND[0], y - ROUND[1]) < ROUND[2] + 12
            or math.hypot(x - POND[0], y - POND[1]) < POND[2] + 12):
        continue
    r = 4.5 + random.random() * 3
    bpy.ops.mesh.primitive_cylinder_add(radius=0.3, depth=3, location=(x, fy(y), 1.5))
    link(bpy.context.active_object, c_trees).color = (0.40, 0.28, 0.18, 1)
    if random.random() < 0.35:
        bpy.ops.mesh.primitive_cone_add(vertices=12, radius1=r, depth=r * 2.6, location=(x, fy(y), 3 + r * 1.3))
        color = (0.22, 0.42, 0.25, 1)
    else:
        bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=2, radius=r, location=(x, fy(y), 3 + r * 0.9))
        color = (0.36, 0.58, 0.32, 1) if random.random() < 0.6 else (0.52, 0.66, 0.36, 1)
    link(bpy.context.active_object, c_trees).color = color
    placed += 1

# --- cars: two parking rows, and cars along the streets -----------------------------------------
CAR_COLORS = [(0.20, 0.38, 0.55, 1), (0.66, 0.27, 0.23, 1), (0.85, 0.84, 0.80, 1), (0.18, 0.18, 0.18, 1)]
cars = 0
px, py, pw, ph = PARKING
for row in [py + 16, py + ph - 30]:
    for k in range(6):
        if random.random() < 0.8:
            o = box(f"Car_{cars:02d}", px + 14 + k * 27 - 2.25, fy(row + 6), 4.5, 1.8, 0, 1.4, c_cars, random.choice(CAR_COLORS))
            o.rotation_euler = (0, 0, math.radians(90))
            cars += 1
for _ in range(18):
    if random.random() < 0.5:
        yy, w = random.choice(H_LINES)
        box(f"Car_{cars:02d}", random.random() * W - 2.25, fy(yy + w / 2 + 4.5), 4.5, 1.8, 0, 1.4, c_cars, random.choice(CAR_COLORS))
    else:
        xx, w, _, _ = random.choice(V_LINES)
        o = box(f"Car_{cars:02d}", xx + w / 2 + 4.5 - 2.25, fy(random.random() * W), 4.5, 1.8, 0, 1.4, c_cars, random.choice(CAR_COLORS))
        o.rotation_euler = (0, 0, math.radians(90))
    cars += 1

# --- NPCs: sidewalks and park -----------------------------------------------------------------
for i in range(40):
    if random.random() < 0.4:
        x, y = random.random() * PARK[2], random.random() * PARK[3]
    elif random.random() < 0.5:
        yy, w = random.choice(H_LINES)
        x, y = random.random() * W, yy + w / 2 + 3
    else:
        xx, w, _, _ = random.choice(V_LINES)
        x, y = xx + w / 2 + 3, random.random() * W
    if in_house(x, y) or near_creek(x, y, 40):
        continue
    bpy.ops.mesh.primitive_cylinder_add(radius=0.35, depth=1.75, location=(x, fy(y), 0.9))
    link(bpy.context.active_object, c_npcs).color = (0.90, 0.60, 0.20, 1)

# --- top camera and sun -------------------------------------------------------------------------
bpy.ops.object.camera_add(location=(750, 750, 2000))
cam = link(bpy.context.active_object, scene.collection)
cam.name = "Cam_Top"
cam.data.type = "ORTHO"
cam.data.ortho_scale = 1560
cam.data.clip_start = 1
cam.data.clip_end = 5000
scene.camera = cam
sun = bpy.data.objects.new("Sun", bpy.data.lights.new("Sun", "SUN"))
scene.collection.objects.link(sun)
sun.rotation_euler = (math.radians(50), 0, math.radians(35))

os.makedirs(os.path.dirname(os.path.abspath(OUT_BLEND)), exist_ok=True)
bpy.ops.wm.save_as_mainfile(filepath=os.path.abspath(OUT_BLEND))
print("houses", n_houses, "trees", placed, "cars", cars, "objects", len(scene.objects))
