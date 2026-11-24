"""The reference forest section of the placeholder world: the pine track, the cabin clearing and the pond shore.

With the Fab content imported (see fab.py) the section is built from scanned assets: Megaplants spruce and beech in four sizes as
instanced Nanite-assembly trees, Megascans lady fern, mossy rock clusters, fallen logs and stumps, plus patches of ground mist that the
lighting scenarios fade in and out. Without it, the procedural placeholders below are used (undergrowth, dead wood, rocks).
Either way the cabin keeps its clutter (firewood, chopping block, fire pit, rain barrel).
Everything is generated from the layout seed, so a rebuild gives the same forest. Runs inside Unreal as part of build_world.py (phase H).
"""

import math
import os
import random

import unreal

import fab
import layout
import meshkit
import nature
import ue_kit as k

CELL = 300.0                # sampling grid of the undergrowth
TREE_STEP = 540.0           # sampling grid of the scanned trees

# Distance (cm) beyond which the wind animation (world position offset) of the scanned vegetation is switched off; 0 = never.
# Beyond it the plant shows its rest pose, Nanite rasterises it on the fast path and its virtual shadow map pages stay cached.
# Measured before it became the default, see Artifacts/VisualProgress/PROGRESS.md. SPN_WPO_SCALE=0 builds without it (for A/B runs).
WPO_DISABLE = {"fern": 2500, "small_tree": 3500, "tree": 6000}
WPO_SCALE = float(os.environ.get("SPN_WPO_SCALE", "0"))


def wpo_distance(kind):
    return int(WPO_DISABLE[kind] * WPO_SCALE)


def rock_mesh(name, seed, mats):
    """Low boulder: a sphere pushed in and out by noise, flat-shaded like broken stone, a third of it below the ground."""
    rng = random.Random(seed)
    rx, ry, rz = 60.0 * rng.uniform(0.85, 1.25), 60.0 * rng.uniform(0.65, 1.0), 60.0 * rng.uniform(0.45, 0.75)
    rows, cols = 8, 13

    def pt(r, c):
        lat, lon = math.pi * (r / rows - 0.5), 2 * math.pi * (c % cols) / cols
        d = (math.cos(lat) * math.cos(lon), math.cos(lat) * math.sin(lon), math.sin(lat))
        bump = 0.70 + 0.50 * layout.noise(d[0] * 1.6 + 10.0 + d[2], d[1] * 1.6 + 10.0 - d[2], seed) + 0.20 * layout.noise(d[0] * 4.0 + 30.0 + d[2] * 3.0, d[1] * 4.0 + 30.0, seed + 1)
        return (d[0] * rx * bump, d[1] * ry * bump, d[2] * rz * bump + rz * 0.3)

    mesh = meshkit.Mesh()
    for r in range(rows):
        for c in range(cols):
            pts = [pt(r, c), pt(r, c + 1), pt(r + 1, c + 1), pt(r + 1, c)]
            if r == 0:
                pts = pts[1:]
            elif r == rows - 1:
                pts = pts[:3]
            centre = [sum(p[i] for p in pts) / len(pts) for i in range(3)]
            shade = 0.82 + 0.18 * layout.noise(centre[0] * 0.05 + 50.0, centre[1] * 0.05 + centre[2] * 0.05 + 50.0, seed + 2)
            mesh.poly("rock", pts, None, [(p[0] + p[2] * 0.6, p[1] + p[2] * 0.6) for p in pts], (shade, 0.25, 0.0, 1.0), hint=(centre[0], centre[1], centre[2] - rz * 0.3))
    return k.static_mesh(mesh, name, mats, nanite=False, collision=True, folder=nature.VEG)


def bark_material():
    texture = k.load(nature.VEG + "/T_Conifer_Bark_01_C")
    return k.instance("MI_LogBark", nature.masters()["bark"], {"UVScale": 1.0 / 110.0}, {"Tint": (0.62, 0.58, 0.54)}, {"BaseColor": texture}, nature.VEG)


def log_mesh(name, length, radius, mats, stubs=2, seed=1):
    """Fallen trunk, modelled standing (smooth sides); the instances are laid down. Origin at the thick end."""
    rng = random.Random(seed)
    mesh = meshkit.Mesh()
    parts = 5
    for i in range(parts):
        z0, z1 = length * i / parts, length * (i + 1) / parts
        r0, r1 = radius * (1.0 - 0.28 * i / parts), radius * (1.0 - 0.28 * (i + 1) / parts)
        mesh.cyl("bark", 0.0, 0.0, z0, z1, r0, r1, segs=12, caps=False)
    ring = lambda r, z: [(math.cos(2 * math.pi * i / 12) * r, math.sin(2 * math.pi * i / 12) * r, z) for i in range(12)]
    mesh.poly("cut", ring(radius, 0.0), (0, 0, -1), [(p[0] * 1.5, p[1] * 1.5) for p in ring(radius, 0.0)])
    mesh.poly("cut", ring(radius * 0.72, length), (0, 0, 1), [(p[0] * 1.5, p[1] * 1.5) for p in ring(radius * 0.72, length)])
    for _ in range(stubs):                                                  # broken branch stubs
        z, a = rng.uniform(0.3, 0.85) * length, rng.uniform(0, 2 * math.pi)
        reach = rng.uniform(25.0, 60.0)
        mesh.beam("bark", (math.cos(a) * radius * 0.5, math.sin(a) * radius * 0.5, z), (math.cos(a) * (radius + reach), math.sin(a) * (radius + reach), z + reach * 0.5), rng.uniform(5.0, 9.0))
    return k.static_mesh(mesh, name, mats, nanite=False, collision=True, folder=nature.VEG)


def stump_mesh(name, mats):
    mesh = meshkit.Mesh()
    mesh.cyl("bark", 0.0, 0.0, -25.0, 8.0, 34.0, 25.0, segs=12, caps=False)        # root flare
    mesh.cyl("bark", 0.0, 0.0, 8.0, 42.0, 25.0, 22.0, segs=12, caps=False)
    top = [(math.cos(2 * math.pi * i / 12) * 22.0, math.sin(2 * math.pi * i / 12) * 22.0, 42.0 + 3.0 * math.sin(i * 1.7)) for i in range(12)]
    for i in range(12):
        tri = [(0.0, 0.0, 43.0), top[i], top[(i + 1) % 12]]
        mesh.poly("cut", tri, None, [(p[0] * 1.5, p[1] * 1.5) for p in tri], hint=(0, 0, 1))
    return k.static_mesh(mesh, name, mats, nanite=False, collision=True, folder=nature.VEG)


def woodpile_mesh(name, mats, rng):
    """Split firewood stacked between two posts; local +Y along the stack, origin at one end on the ground."""
    mesh = meshkit.Mesh()
    length, depth = 300.0, 42.0
    for y in (0.0, length):
        mesh.box("planks_dark", -5.0, 5.0, y - 5.0, y + 5.0, -20.0, 115.0)
    z = 0.0
    row = 0
    while z < 92.0:
        y = 8.0 + (6.0 if row % 2 else 0.0)
        height = rng.uniform(10.0, 14.0)
        while y < length - 18.0:
            width = rng.uniform(10.0, 16.0)
            shade = rng.uniform(0.7, 1.05)
            x0 = -depth / 2 + rng.uniform(-3.0, 3.0)
            mesh.box("planks", x0, x0 + depth + rng.uniform(-4.0, 4.0), y, y + width - 1.2, z, z + height - 0.8, col=(shade, rng.uniform(0.1, 0.5), 0.0, 1.0))
            y += width
        z += height
        row += 1
    return k.static_mesh(mesh, name, mats, nanite=False, collision=True, folder=nature.VEG)


def barrel_mesh(name, mats):
    mesh = meshkit.Mesh()
    mesh.cyl("planks_dark", 0.0, 0.0, 0.0, 45.0, 27.0, 31.0, segs=14)
    mesh.cyl("planks_dark", 0.0, 0.0, 45.0, 90.0, 31.0, 27.0, segs=14)
    for z in (12.0, 43.0, 76.0):
        mesh.cyl("steel", 0.0, 0.0, z, z + 4.0, 31.5 if z == 43.0 else 29.5, segs=14, caps=False)
    return k.static_mesh(mesh, name, mats, nanite=False, collision=True, folder=nature.VEG)


def fire_ground_mesh(name, mats):
    mesh = meshkit.Mesh()
    ring = lambda r, z: [(math.cos(2 * math.pi * i / 14) * r, math.sin(2 * math.pi * i / 14) * r, z) for i in range(14)]
    mesh.poly("dirt", ring(95.0, 2.0), (0, 0, 1), None, (0.75, 0.6, 0.0, 1.0))
    mesh.poly("dirt", ring(48.0, 3.0), (0, 0, 1), None, (0.22, 0.9, 0.0, 1.0))        # ash and charcoal
    for a, tilt in ((0.4, 9.0), (2.1, 6.0), (3.9, 11.0)):                              # charred remains
        mesh.beam("planks_dark", (math.cos(a) * 30.0, math.sin(a) * 30.0, 5.0), (-math.cos(a) * 26.0, -math.sin(a) * 26.0, tilt), 7.0, col=(0.25, 0.9, 0.0, 1.0))
    return k.static_mesh(mesh, name, mats, nanite=False, collision=False, folder=nature.VEG)


def instanced(asset, transforms, label, collide, shadow=True, cull=0, wpo=0):
    """cull: distance in cm beyond which the instances are not drawn (0 = always); small things only, they are invisible that far away.
    wpo: distance in cm beyond which their wind animation stops (0 = never)."""
    if not transforms:
        return
    actor = k.spawn(unreal.load_class(None, "/Script/SupernaturalProt.SpnInstancedGroup"), 0, 0, 0, label=label, folder="Nature/woodland")
    comp = actor.get_editor_property("instances")
    comp.set_static_mesh(asset)
    if cull:
        comp.set_cull_distances(int(cull * 0.8), int(cull))
    if wpo:
        comp.set_editor_property("world_position_offset_disable_distance", wpo)
    if not collide:
        comp.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    if not shadow:
        comp.set_cast_shadow(False)
    comp.add_instances(transforms, False, True)


def skinned(mesh, transforms, label, cull=0, wpo=0):
    """One instanced group of a Megaplants tree (skeletal mesh with a Nanite assembly), reference pose, no collision."""
    if not transforms:
        return
    actor = k.spawn(unreal.load_class(None, "/Script/SupernaturalProt.SpnSkinnedGroup"), 0, 0, 0, label=label, folder="Nature/woodland")
    comp = actor.get_editor_property("instances")
    comp.set_skinned_asset_and_update(mesh, True)
    if wpo:
        comp.set_editor_property("world_position_offset_disable_distance", wpo)
    if cull:
        comp.set_editor_property("instance_start_cull_distance", int(cull * 0.8))
        comp.set_editor_property("instance_end_cull_distance", int(cull))
    comp.add_instances(transforms, [0] * len(transforms), False, True)


def mist(rng, section, terrain, free, count=16):
    """Patches of ground mist (local fog volumes) in hollows of the section. Hidden until a scenario gives them a density."""
    x0, x1, y0, y1 = section.box
    placed = 0
    tries = 0
    while placed < count and tries < 4000:
        tries += 1
        x, y = rng.uniform(x0, x1), rng.uniform(y0, y1)
        if not section(x, y) or not free(x, y, 200.0):
            continue
        z = terrain.z(x, y)
        around = sum(terrain.z(x + dx, y + dy) for dx, dy in ((1500, 0), (-1500, 0), (0, 1500), (0, -1500))) / 4.0
        if z > around + 60.0 and rng.random() < 0.8:                  # mist gathers in hollows and on low ground
            continue
        base = rng.uniform(0.010, 0.022)        # extinction per metre: 30 m of mist dims the view by about a third to a half
        actor = k.spawn(unreal.LocalFogVolume, x, y, z + 50.0, rng.uniform(0, 360), "SPN_Mist_%02d" % placed, folder="Nature/woodland")
        actor.set_actor_scale3d(unreal.Vector(rng.uniform(22, 42), rng.uniform(18, 34), rng.uniform(3.5, 6.0)))
        actor.set_editor_property("tags", ["SpnMist", "Mist=%.2f" % base])
        comp = actor.get_component_by_class(unreal.LocalFogVolumeComponent)
        comp.set_editor_property("radial_fog_extinction", base)
        comp.set_editor_property("height_fog_extinction", base)
        placed += 1
    return placed


def build(ctx):
    terrain, roads, lots = ctx["terrain"], ctx["roads"], ctx["lots"]
    mats = dict(ctx["mats"])
    mats["bark"] = bark_material()
    mats["cut"] = mats["planks"]
    rng = random.Random(layout.SEED * 13 + 5)
    cabin = next(lot for lot in lots if lot.kind == "cabin")
    pond = layout.POND
    section = layout.reference_section(roads, lots)

    trees = fab.trees()
    ferns = fab.ferns()
    rock_s, log_s, stump_s = fab.static(fab.ROCKS), fab.static(fab.LOG), fab.static(fab.STUMP)
    k.log("woodland: scanned trees %d, ferns %d, rock %s, log %s, stump %s" % (len(trees), len(ferns), rock_s is not None, log_s is not None, stump_s is not None))

    def smooth(a, b, v):
        t = min(max((v - a) / (b - a), 0.0), 1.0)
        return t * t * (3 - 2 * t)

    def cabin_local(x, y):
        dx, dy = x - cabin.fx, y - cabin.fy
        c, s = math.cos(math.radians(cabin.yaw)), math.sin(math.radians(cabin.yaw))
        return dx * c + dy * s, -dx * s + dy * c

    def free(x, y, margin=0.0):
        if terrain.road_edge(x, y) < 170.0 + margin:
            return False
        if math.hypot(x - pond["x"], y - pond["y"]) < pond["radius"] * 1.02 + margin:
            return False
        lx, ly = cabin_local(x, y)
        return not (abs(lx) < 500.0 + margin and -420.0 - margin < ly < 700.0 + margin) and not (abs(lx) < 110.0 + margin and ly < 0.0 and ly > -1000.0)

    groups = {}
    tree_groups = {}
    colliders = []
    placed_trees = []           # (x, y, trunk radius) of every tree with a collider: for the SpnWorld QA points (qa_points.py)

    def add(kind, x, y, scale, sink=0.0, yaw=None, pitch=0.0, roll=0.0, squash=1.0, stretch=(1.0, 1.0)):
        z = terrain.z(x, y) - sink
        groups.setdefault(kind, []).append(unreal.Transform(unreal.Vector(x, y, z), unreal.Rotator(roll=roll, pitch=pitch, yaw=rng.uniform(0, 360) if yaw is None else yaw),
                                                            unreal.Vector(scale * stretch[0], scale * stretch[1], scale * squash)))

    def add_rock(x, y, scale, sink, tilt=20.0):
        """One scanned rock cluster, stretched and tilted differently every time so the single scan does not repeat visibly."""
        add("scan_rock", x, y, scale, sink=sink, pitch=rng.uniform(-tilt, tilt), roll=rng.uniform(-tilt, tilt), squash=rng.uniform(0.6, 1.25),
            stretch=(rng.uniform(0.75, 1.35), rng.uniform(0.75, 1.35)))

    camera_path = [(ax + math.cos(math.radians(a)) * ar, ay + math.sin(math.radians(a)) * ar)
                   for ax, ay, ar, a0, a1 in layout.CAMERA_ARCS for a in range(int(a0), int(a1) + 1, 3)]
    cx, cy, cr, _, _ = layout.CREEK_VIEW

    def add_tree(kind, x, y, scale):
        """A scanned tree, slightly sunk and leaning, with an invisible trunk collider for the bigger ones."""
        if min(math.hypot(x - px, y - py) for px, py in camera_path) < (500.0 if kind.endswith("_D") else 900.0):   # the creek and pond
            return                                                                                                # shots fly here
        if not kind.endswith("_D") and math.hypot(x - cx, y - cy) < cr + 400.0:     # ... and looks across this open bank
            return
        z = terrain.z(x, y) - 18.0 * scale
        tree_groups.setdefault(kind, []).append(unreal.Transform(unreal.Vector(x, y, z), unreal.Rotator(roll=rng.uniform(-2.5, 2.5), pitch=rng.uniform(-2.5, 2.5), yaw=rng.uniform(0, 360)),
                                                                 unreal.Vector(scale, scale, scale * rng.uniform(0.94, 1.06))))
        radius = fab.TRUNK.get(kind, 0.0) * scale
        if radius > 0.0:
            colliders.append(unreal.Transform(unreal.Vector(x, y, z + 200.0), unreal.Rotator(), unreal.Vector(radius / 50.0, radius / 50.0, 4.0)))
            placed_trees.append((x, y, radius))

    def lay_log(kind, x, y, length, radius, yaw=None):
        """Lays a procedural log (modelled upright) on the ground, following the slope under it."""
        yaw = rng.uniform(0, 360) if yaw is None else yaw
        dx, dy = -math.cos(math.radians(yaw)), -math.sin(math.radians(yaw))
        z0, z1 = terrain.z(x, y), terrain.z(x + dx * length, y + dy * length)
        grade = math.degrees(math.atan2(z1 - z0, length))
        groups.setdefault(kind, []).append(unreal.Transform(unreal.Vector(x, y, z0 + radius * 0.62), unreal.Rotator(roll=0.0, pitch=90.0 - grade, yaw=yaw), unreal.Vector(1, 1, 1)))

    def lay_scan_log(x, y, scale, yaw=None):
        """Lays the scanned log (modelled lying along X, centred) on the ground, following the slope along it."""
        yaw = rng.uniform(0, 360) if yaw is None else yaw
        half = 73.0 * scale
        dx, dy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        z0, z1 = terrain.z(x - dx * half, y - dy * half), terrain.z(x + dx * half, y + dy * half)
        grade = math.degrees(math.atan2(z1 - z0, 2 * half))
        groups.setdefault("scan_log", []).append(unreal.Transform(unreal.Vector(x, y, (z0 + z1) / 2 - 4.0 * scale), unreal.Rotator(roll=rng.uniform(-8, 8), pitch=grade, yaw=yaw),
                                                                  unreal.Vector(scale, scale * rng.uniform(0.9, 1.15), scale * rng.uniform(0.9, 1.15))))

    fern_kinds = [("fern_" + v, w) for v, w in (("A", 3), ("E", 3), ("F", 3), ("H", 2), ("G", 2), ("I", 2), ("B", 1), ("C", 1), ("D", 1)) if v in ferns]

    def pick_fern():
        total = sum(w for _, w in fern_kinds)
        r = rng.uniform(0, total)
        for kind, w in fern_kinds:
            r -= w
            if r <= 0:
                return kind
        return fern_kinds[-1][0]

    x_lo, x_hi, y_lo, y_hi = section.box
    x_lo, x_hi = max(x_lo, -layout.EXTENT + 400.0), min(x_hi, layout.EXTENT - 400.0)
    y_lo, y_hi = max(y_lo, -layout.EXTENT + 400.0), min(y_hi, layout.EXTENT - 400.0)

    # ---- scanned trees: stands and gaps, mixed ages, spruce inside the forest and beech at its edges
    if trees:
        for i in range(int((x_hi - x_lo) / TREE_STEP)):
            for j in range(int((y_hi - y_lo) / TREE_STEP)):
                x, y = x_lo + (i + rng.random()) * TREE_STEP, y_lo + (j + rng.random()) * TREE_STEP
                if not section(x, y):
                    continue
                f = terrain.forest_at(x, y)
                stand = layout.noise(x / 5200.0, y / 5200.0, 11)
                if f < 0.08 or rng.random() > f ** 0.8 * (0.30 + 1.10 * stand):
                    continue
                if terrain.road_edge(x, y) < 520.0 or terrain.slope(x, y) > 0.8 or not free(x, y, 250.0):
                    continue
                beech = rng.random() < (0.45 if f < 0.55 else 0.16)
                age = rng.random()
                variant = "A" if age < 0.14 else "B" if age < 0.58 else "C" if age < 0.84 else "D"
                kind = ("beech_" if beech else "spruce_") + variant
                if kind not in trees:
                    kind = next(iter(trees))
                add_tree(kind, x, y, rng.uniform(0.86, 1.12))
        # young growth where light reaches the ground: along the track and at the clearing edges
        for i in range(int((x_hi - x_lo) / 700.0)):
            for j in range(int((y_hi - y_lo) / 700.0)):
                x, y = x_lo + (i + rng.random()) * 700.0, y_lo + (j + rng.random()) * 700.0
                if not section(x, y) or not free(x, y, 120.0) or terrain.forest_at(x, y) < 0.1:
                    continue
                light = smooth(1800.0, 500.0, terrain.road_edge(x, y))
                if rng.random() < 0.10 + 0.35 * light:
                    kind = "spruce_D" if rng.random() < 0.6 else "beech_D"
                    if kind in trees:
                        add_tree(kind, x, y, rng.uniform(0.32, 0.75))

    # ---- undergrowth, dead wood and rocks
    for i in range(int((x_hi - x_lo) / CELL)):
        for j in range(int((y_hi - y_lo) / CELL)):
            x, y = x_lo + (i + rng.random()) * CELL, y_lo + (j + rng.random()) * CELL
            if not section(x, y) or not free(x, y):
                continue
            wooded = max(terrain.forest_at(x, y), terrain.layer("Forest", x, y))
            slope = terrain.slope(x, y, 150.0)
            near_road = terrain.road_edge(x, y)
            clump = layout.noise(x / 1300.0, y / 1300.0, 21)
            thicket = layout.noise(x / 2600.0, y / 2600.0, 22)
            light = smooth(1600.0, 500.0, near_road)                       # more light along the track: thicker undergrowth
            verge = smooth(1300.0, 350.0, near_road) if near_road > 220.0 else 0.0   # fern verges where the camera and the player look
            if (wooded > 0.05 or verge > 0.2) and rng.random() < 0.62 * smooth(0.38, 0.68, clump) * (0.6 + 0.4 * wooded) + 0.5 * verge * smooth(0.25, 0.55, clump):
                for _ in range(rng.choice([1, 2, 2, 3])):
                    fx, fy = x + rng.uniform(-110, 110), y + rng.uniform(-110, 110)
                    if not free(fx, fy):
                        continue
                    if fern_kinds:
                        add(pick_fern(), fx, fy, rng.uniform(0.75, 1.3), sink=4.0, pitch=rng.uniform(-5, 5), roll=rng.uniform(-5, 5))
                    else:
                        add("fern", fx, fy, rng.uniform(1.3, 3.0), sink=3.0, pitch=rng.uniform(-6, 6), roll=rng.uniform(-6, 6))
            if not trees:                                                   # placeholder shrubs and young trees (the scanned ones are above)
                if wooded > 0.05 and free(x, y, 120.0) and rng.random() < (0.030 + 0.075 * light) * smooth(0.40, 0.65, thicket):
                    add("shrub", x, y, rng.uniform(0.40, 1.0), sink=6.0)
                if wooded > 0.15 and free(x, y, 150.0) and rng.random() < 0.030 * (1.0 + light):
                    add("conifer" if rng.random() < 0.75 else "deciduous", x, y, rng.uniform(0.14, 0.48), sink=3.0)
            near_cabin = math.hypot(x - cabin.x, y - cabin.y) < 2600.0
            embankment = smooth(0.35, 0.6, slope) * smooth(900.0, 300.0, near_road)     # the steep sides cut for the track
            if rock_s is not None and near_cabin and slope > 0.5 and free(x, y, 80.0) and rng.random() < 0.55:
                # the cut in the slope behind the cabin: broken rock instead of a smooth bank
                add_rock(x, y, rng.uniform(0.8, 1.9), sink=rng.uniform(25.0, 45.0), tilt=25.0)
            elif rng.random() < 0.010 + 0.11 * smooth(0.30, 0.62, slope) + 0.18 * embankment:     # rocks gather on the banks
                big = rng.random() < 0.07
                scale = rng.uniform(1.6, 2.8) if big else rng.uniform(0.3, 1.2)
                if free(x, y, 60.0 * scale):
                    if rock_s is not None:
                        add_rock(x, y, scale * 0.42, sink=(6.0 + 10.0 * embankment) * scale)
                    else:
                        add("rock%d" % rng.randrange(3), x, y, scale, sink=14.0 * scale, pitch=rng.uniform(-12, 12), roll=rng.uniform(-12, 12), squash=rng.uniform(0.7, 1.15))
            if wooded > 0.2 and slope < 0.45 and free(x, y, 500.0):
                r = rng.random()
                if r < 0.0075:
                    if log_s is not None:
                        lay_scan_log(x, y, rng.uniform(1.8, 3.2))
                    else:
                        lay_log("log%d" % rng.randrange(2), x, y, 520.0, 20.0)
                elif r < 0.016:
                    if stump_s is not None:
                        add("scan_stump", x, y, rng.uniform(1.3, 2.4), sink=5.0)
                    else:
                        add("stump", x, y, rng.uniform(0.7, 1.25), sink=2.0)

    # ---- the cabin clearing: things people leave around
    def at_cabin(lx, ly):
        return cabin.to_world(lx, ly)

    fx, fy = at_cabin(-760.0, -330.0)                                       # fire pit in front of the cabin, off the path
    k.place(fire_ground_mesh("SM_FirePit", mats), fx, fy, terrain.z(fx, fy), 0.0, "SPN_FirePit", folder="Nature/woodland", collide=False, shadow=False)
    for i in range(11):
        a = 2 * math.pi * i / 11 + rng.uniform(-0.1, 0.1)
        if rock_s is not None:
            add("scan_rock", fx + math.cos(a) * 66.0, fy + math.sin(a) * 66.0, rng.uniform(0.09, 0.13), sink=1.0)
        else:
            add("rock%d" % (i % 3), fx + math.cos(a) * 62.0, fy + math.sin(a) * 62.0, rng.uniform(0.2, 0.3), sink=3.0)
    for a in (cabin.yaw + 20.0, cabin.yaw + 150.0):                         # two logs to sit on
        sx, sy = fx + math.cos(math.radians(a + 90.0)) * 170.0, fy + math.sin(math.radians(a + 90.0)) * 170.0
        if log_s is not None:
            lay_scan_log(sx, sy, 1.75, yaw=a)
        else:
            ex, ey = sx + math.cos(math.radians(a)) * 130.0, sy + math.sin(math.radians(a)) * 130.0
            lay_log("seat", ex, ey, 260.0, 24.0, yaw=a)
    wx, wy = at_cabin(-470.0, 150.0)                                        # firewood along the left wall
    k.place(woodpile_mesh("SM_Woodpile", mats, rng), wx, wy, cabin.z, cabin.yaw, "SPN_Woodpile", folder="Nature/woodland")
    bx, by = at_cabin(-620.0, 40.0)                                         # chopping block with a few split pieces
    if stump_s is not None:
        add("scan_stump", bx, by, 1.45, sink=8.0)
    else:
        add("stump", bx, by, 1.15, sink=1.0)
    for _ in range(5):
        lay_log("billet", bx + rng.uniform(-110, 110), by + rng.uniform(-110, 110), 40.0, 7.0)
    rx, ry = at_cabin(445.0, -45.0)                                         # rain barrel at the corner
    k.place(barrel_mesh("SM_RainBarrel", mats), rx, ry, cabin.z, cabin.yaw, "SPN_RainBarrel", folder="Nature/woodland")
    if fern_kinds:
        # ferns hugging the walls (side and back), where the mower never reaches; not on the path and not on the porch
        for _ in range(60):
            side = rng.random()
            if side < 0.4:
                lx, ly = rng.uniform(-420.0, 420.0), rng.uniform(640.0, 760.0)                       # behind the back wall
            elif side < 0.7:
                lx, ly = -rng.uniform(430.0, 540.0), rng.uniform(260.0, 620.0)                      # left wall (the woodpile is further front)
            else:
                lx, ly = rng.uniform(430.0, 540.0), rng.uniform(400.0, 620.0)                       # right wall, behind the chimney
            wx, wy = at_cabin(lx, ly)
            if terrain.road_edge(wx, wy) > 200.0:
                add(pick_fern(), wx, wy, rng.uniform(0.6, 1.1), sink=4.0, pitch=rng.uniform(-6, 6), roll=rng.uniform(-6, 6))
    if rock_s is not None:
        for lx, ly in ((-430.0, -250.0), (-380.0, -265.0), (430.0, -255.0)):                    # stones at the porch corners
            wx, wy = at_cabin(lx + rng.uniform(-15, 15), ly + rng.uniform(-15, 15))
            add_rock(wx, wy, rng.uniform(0.16, 0.26), sink=4.0, tilt=12.0)

    # ---- instancing
    assets = {"billet": log_mesh("SM_Billet", 40.0, 7.0, mats, 0, 4)}
    if rock_s is not None:
        assets["scan_rock"] = rock_s
    else:
        for i in range(3):
            assets["rock%d" % i] = rock_mesh("SM_Rock%d" % i, 40 + i, mats)
    if log_s is not None:
        assets["scan_log"] = log_s
    else:
        assets.update({"log0": log_mesh("SM_Log0", 520.0, 20.0, mats, 3, 1), "log1": log_mesh("SM_Log1", 520.0, 20.0, mats, 1, 2), "seat": log_mesh("SM_LogSeat", 260.0, 24.0, mats, 0, 3)})
    assets["scan_stump" if stump_s is not None else "stump"] = stump_s if stump_s is not None else stump_mesh("SM_Stump", mats)
    for v, mesh in ferns.items():
        assets["fern_" + v] = mesh
    if not ferns:
        assets["fern"] = nature.plant_mesh("plant")
    if not trees:
        assets.update({"shrub": nature.plant_mesh("shrub"), "conifer": nature.plant_mesh("conifer"), "deciduous": nature.plant_mesh("deciduous")})
    soft = ("fern", "shrub", "conifer", "deciduous", "billet")
    for kind, transforms in sorted(groups.items()):
        cull = 9000 if kind.startswith("fern") else 16000 if kind in ("scan_stump", "billet") else 0
        instanced(assets[kind], transforms, "SPN_Woodland_" + kind, collide=not kind.startswith(soft), cull=cull, wpo=wpo_distance("fern") if kind.startswith("fern") else 0)
    for kind, transforms in sorted(tree_groups.items()):
        skinned(trees[kind], transforms, "SPN_Woodland_" + kind, cull={"D": 12000, "C": 22000}.get(kind[-1], 0),
                wpo=wpo_distance("small_tree" if kind[-1] in "CD" else "tree"))
    if colliders:
        actor = k.spawn(unreal.load_class(None, "/Script/SupernaturalProt.SpnInstancedGroup"), 0, 0, 0, label="SPN_Woodland_TrunkCollision", folder="Nature/collision")
        comp = actor.get_editor_property("instances")
        comp.set_static_mesh(k.load("/Engine/BasicShapes/Cylinder"))
        comp.set_visibility(False)
        comp.set_cast_shadow(False)
        comp.add_instances(colliders, False, True)
    patches = mist(rng, section, terrain, free)
    rock_scale = 1.0 if rock_s is not None else 2.4      # extent of one instance at scale 1, relative to a scan cluster (about 3 m)
    ctx["woodland"] = {"trees": placed_trees,
                       "rocks": [(t.translation.x, t.translation.y, t.scale3d.x * rock_scale) for kind, ts in groups.items() if kind in ("scan_rock", "rock0", "rock1", "rock2") for t in ts]}
    counts = {kind: len(t) for kind, t in sorted(groups.items())}
    counts.update({kind: len(t) for kind, t in sorted(tree_groups.items())})
    k.log("woodland: %s, %d trunk colliders, %d mist patches" % (counts, len(colliders), patches))
