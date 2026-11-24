"""Detail pass of the placeholder world: street lamps and house lights, utility poles with wires, mail boxes, fences, signs, street
furniture, a culvert with guard rails, a small dock, stains. Everything is placed for a reason (along roads, at entrances, behind shops)."""

import math
import random

import unreal

import archkit as a
import layout
import meshkit
import roads as roadkit
import ue_kit as k
from archkit import Frame

SHOP_NAMES = ["GENERAL STORE", "HARDWARE", "BARBER SHOP", "PHARMACY", "BOOKS", "CAFE", "LAUNDROMAT", "ANTIQUES", "BAR & GRILL", "BAKERY", "FEED & SEED", "TAILOR", "RADIO REPAIR"]
LAMP_TAG = "SpnLamp"


def light(cls, x, y, z, candela, radius, colour=(255, 196, 132), shadows=False, pitch=0.0, yaw=0.0, cone=None, label=None):
    actor = k.spawn(cls, x, y, z, yaw, label, pitch=pitch, folder="Lights")
    actor.set_editor_property("tags", [LAMP_TAG])
    c = actor.get_component_by_class(unreal.LightComponent)
    c.set_mobility(unreal.ComponentMobility.MOVABLE)
    c.set_editor_property("intensity_units", unreal.LightUnits.CANDELAS)
    c.set_editor_property("intensity", candela)
    c.set_editor_property("attenuation_radius", radius)
    c.set_editor_property("light_color", unreal.Color(r=colour[0], g=colour[1], b=colour[2], a=255))
    c.set_editor_property("cast_shadows", shadows)
    c.set_editor_property("source_radius", 6.0)
    if cone:
        c.set_editor_property("outer_cone_angle", cone)
        c.set_editor_property("inner_cone_angle", cone * 0.55)
    return actor


def text(value, x, y, z, yaw, size, colour=(235, 228, 205), label=None):
    actor = k.spawn(unreal.TextRenderActor, x, y, z, yaw, label, folder="Signs")
    c = actor.text_render
    c.set_text(value)
    c.set_world_size(size)
    c.set_text_render_color(unreal.Color(r=colour[0], g=colour[1], b=colour[2], a=255))
    c.set_horizontal_alignment(unreal.HorizTextAligment.EHTA_CENTER)
    c.set_vertical_alignment(unreal.VerticalTextAligment.EVRTA_TEXT_CENTER)
    return actor


def instanced(asset, transforms, label, collide=True):
    if not transforms:
        return
    actor = k.spawn(unreal.load_class(None, "/Script/SupernaturalProt.SpnInstancedGroup"), 0, 0, 0, label=label, folder="Props")
    comp = actor.get_editor_property("instances")
    comp.set_static_mesh(asset)
    if not collide:
        comp.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    comp.add_instances(transforms, False, True)


def xf(x, y, z, yaw=0.0, scale=1.0):
    return unreal.Transform(unreal.Vector(x, y, z), unreal.Rotator(roll=0.0, pitch=0.0, yaw=yaw), unreal.Vector(scale, scale, scale))


# ---------------------------------------------------------------- prop meshes (local origin on the ground)

def street_lamp():
    m = meshkit.Mesh()
    m.cyl("steel", 0, 0, 0, 60, 13, 10, segs=10)
    m.cyl("steel", 0, 0, 60, 800, 8.5, 5.5, segs=10)
    m.beam("steel", (0, 0, 790), (170, 0, 820), 7.0)
    m.box("steel", 150, 235, -14, 14, 812, 826)
    m.box("lamp", 160, 228, -11, 11, 808, 812)
    return m


def utility_pole(transformer=False):
    m = meshkit.Mesh()
    m.cyl("planks_dark", 0, 0, -80, 980, 15, 11, segs=10)
    m.box("planks_dark", -8, 8, -125, 125, 890, 904)
    m.beam("steel", (9, -60, 850), (9, 0, 890), 3.0)
    m.beam("steel", (9, 60, 850), (9, 0, 890), 3.0)
    for y in (-110, 0, 110):
        m.cyl("trim", 0, y, 904, 918, 4.5, segs=6)
    if transformer:
        m.cyl("steel", 30, 0, 700, 800, 24, segs=12)
    return m


def mailbox():
    m = meshkit.Mesh()
    m.box("planks", -5, 5, -5, 5, -20, 105)
    m.box("metal_dark", -10, 10, -24, 24, 105, 128)
    m.box("metal_red", 10, 12, 4, 8, 118, 140)
    return m


def bench():
    m = meshkit.Mesh()
    for x in (-70, 70):
        m.box("metal_dark", x - 3, x + 3, -24, 24, 0, 43)
        m.box("metal_dark", x - 3, x + 3, 18, 24, 43, 86)
    for i in range(4):
        m.box("planks", -85, 85, -24 + i * 11, -15 + i * 11, 43, 47)
    for i in range(3):
        m.box("planks", -85, 85, 19, 23, 54 + i * 12, 63 + i * 12)
    return m


def trash_can():
    m = meshkit.Mesh()
    m.cyl("metal_green", 0, 0, 0, 82, 26, 29, segs=14)
    m.cyl("metal_dark", 0, 0, 82, 90, 30, 22, segs=14)
    return m


def hydrant():
    m = meshkit.Mesh()
    m.cyl("metal_red", 0, 0, 0, 8, 14, segs=10)
    m.cyl("metal_red", 0, 0, 8, 58, 9.5, segs=10)
    m.cyl("metal_red", 0, 0, 58, 70, 11, 4, segs=10)
    m.box("metal_red", -16, 16, -4.5, 4.5, 40, 49)
    return m


def dumpster():
    m = meshkit.Mesh()
    m.box("metal_green", -95, 95, -55, 55, 12, 120)
    m.box("metal_dark", -98, 98, -58, 58, 120, 127)
    for x in (-80, 80):
        for y in (-45, 45):
            m.cyl("metal_dark", x, y, 0, 12, 6, segs=8)
    return m


def stop_sign():
    m = meshkit.Mesh()
    m.cyl("steel", 0, 0, -30, 250, 3.2, segs=8)
    ring = [(-4.2, math.cos(math.pi / 8 + i * math.pi / 4) * 38, 215 + math.sin(math.pi / 8 + i * math.pi / 4) * 38) for i in range(8)]
    m.poly("metal_red", ring, (-1, 0, 0))
    m.poly("steel", [(-3.4, y, z) for _, y, z in ring], (1, 0, 0))
    return m


def sign_board(width=90.0, height=110.0):
    m = meshkit.Mesh()
    m.cyl("steel", 0, 0, -30, 260, 3.2, segs=8)
    m.box("metal_white", -5.0, -3.5, -width / 2, width / 2, 260 - height, 260)
    return m


def parking_block():
    m = meshkit.Mesh()
    m.box("concrete", -90, 90, -9, 9, 0, 13)
    return m


def picket_run(mesh, lot, terrain, lx0, ly0, lx1, ly1, key="trim"):
    """Picket fence between two local points, following the ground."""
    length = math.hypot(lx1 - lx0, ly1 - ly0)
    if length < 60:
        return
    count = max(1, int(length / 240.0))
    def ground(t):
        lx, ly = lx0 + (lx1 - lx0) * t, ly0 + (ly1 - ly0) * t
        wx, wy = lot.to_world(lx, ly)
        return lx, ly, terrain.z(wx, wy) - lot.z
    for i in range(count + 1):
        x, y, z = ground(i / count)
        mesh.box(key, x - 5, x + 5, y - 5, y + 5, z - 20, z + 105)
    for i in range(count):
        p, q = ground(i / count), ground((i + 1) / count)
        for h in (28.0, 78.0):
            mesh.beam(key, (p[0], p[1], p[2] + h), (q[0], q[1], q[2] + h), 3.5, 8.0)
    pickets = max(1, int(length / 15.0))
    dx, dy = (lx1 - lx0) / length, (ly1 - ly0) / length
    for i in range(pickets):
        x, y, z = ground((i + 0.5) / pickets)
        fr = Frame(x, y, dx, dy, -dy, dx, z)
        fr.box(mesh, key, -4.0, 4.0, 3.0, 5.0, 8.0, 96.0, skip="z")


def build(ctx):
    roads, terrain, mats, lots = ctx["roads"], ctx["terrain"], ctx["mats"], ctx["lots"]
    rng = random.Random(layout.SEED * 13 + 5)
    main = layout.main_street(roads)
    hwy = roads["hwy"]
    mesh_asset = lambda name, mesh, **kw: k.static_mesh(mesh, "SM_Prop_" + name, mats, folder=k.MESH + "/Props", **kw)

    # ---- street lamps along the main street, alternating sides, clear of the side streets
    lamp_asset = mesh_asset("StreetLamp", street_lamp())
    gaps = ctx.get("pavement_gaps", [])
    lamps, s, side = [], main[0] + 600.0, 1.0
    while s < main[1] - 300.0:
        if not any(abs(s - gs) < half + 600.0 for gs, _, half in gaps):
            x, y, z, yaw = hwy.offset(s, side * (roadkit.eff_half(hwy, s, main) + 55.0))
            face = yaw + (-90.0 if side > 0 else 90.0)
            lamps.append(xf(x, y, z + 15.0, face))
            hx, hy = x + math.cos(math.radians(face)) * 195.0, y + math.sin(math.radians(face)) * 195.0
            light(unreal.SpotLight, hx, hy, z + 15.0 + 800.0, 1500.0, 2600.0, shadows=True, pitch=-90.0, cone=62.0, label="SPN_StreetLight")
        s += 1700.0
        side = -side
    instanced(lamp_asset, lamps, "SPN_StreetLamps")

    # ---- utility poles and wires along the highway outside the main street, and along Oak Street
    pole_asset, pole_t_asset = mesh_asset("UtilityPole", utility_pole()), mesh_asset("UtilityPoleT", utility_pole(True))
    wires = meshkit.Mesh()
    poles, poles_t = [], []
    def pole_line(rd, s0, s1, side, spacing=4300.0):
        previous = None
        s = s0
        index = 0
        while s <= s1:
            x, y, _, yaw = rd.offset(s, side)
            z = terrain.z(x, y)
            (poles_t if index % 4 == 2 else poles).append(xf(x, y, z, yaw))
            tops = []
            for off in (-110.0, 0.0, 110.0):
                rad = math.radians(yaw + 90.0)
                tops.append((x + math.cos(rad) * off, y + math.sin(rad) * off, z + 918.0))
            if previous:
                for p, q in zip(previous, tops):
                    last = p
                    for i in range(1, 9):
                        t = i / 8.0
                        cur = (p[0] + (q[0] - p[0]) * t, p[1] + (q[1] - p[1]) * t, p[2] + (q[2] - p[2]) * t - 4.0 * t * (1 - t) * 70.0)
                        wires.beam("trim_dark", last, cur, 2.2)
                        last = cur
            previous = tops
            s += spacing
            index += 1
    pole_line(hwy, 800.0, main[0] - 800.0, -(hwy.half + 620.0))
    pole_line(hwy, main[1] + 1500.0, hwy.length - 800.0, -(hwy.half + 620.0))
    oak_main = roads["oak"].nearest_s(-6000.0, 0.0)[0]
    pole_line(roads["oak"], oak_main + 2500.0, roads["oak"].length - 400.0, roads["oak"].half + 330.0, 3800.0)
    pole_line(roads["oak"], 600.0, oak_main - 2500.0, roads["oak"].half + 330.0, 3800.0)
    instanced(pole_asset, poles, "SPN_UtilityPoles")
    instanced(pole_t_asset, poles_t, "SPN_UtilityPolesT")
    if wires.triangle_count():
        k.place(k.static_mesh(wires, "SM_Prop_Wires", mats, collision=False, folder=k.MESH + "/Props"), label="SPN_Wires", folder="Props", collide=False)

    # ---- per building: lights, mail boxes, fences, signs, bins
    mail, bins, blocks = [], [], []
    names = SHOP_NAMES[:]
    rng.shuffle(names)
    for lot in lots:
        world = lambda lx, ly, lz=0.0: lot.to_world(lx, ly) + (lot.z + lz,)
        if getattr(lot, "lamp", None):
            x, y, z = world(*lot.lamp)
            light(unreal.PointLight, x, y, z, 16.0, 750.0, label="SPN_PorchLight_" + lot.name)
        for lx, ly, lz in getattr(lot, "lights", []):
            x, y, z = world(lx, ly, lz)
            cold = lot.kind in ("gas", "garage")
            light(unreal.SpotLight if cold else unreal.PointLight, x, y, z, 1100.0 if cold else 18.0, 1800.0 if cold else 650.0, colour=(210, 232, 255) if cold else (255, 196, 132), shadows=cold, pitch=-90.0 if cold else 0.0, cone=70.0 if cold else None, label="SPN_Light_" + lot.name)
        if lot.kind == "house":
            lx, ly = lot.mail
            x, y = lot.to_world(lx, ly)
            mail.append(xf(x, y, terrain.z(x, y), lot.yaw + rng.uniform(-8, 8)))
            if rng.random() < 0.4:   # front garden fence with a gap at the path
                fence = meshkit.Mesh()
                fy = roadkit and (-(lot.setback - lot.road_half) + 170.0)
                gap0, gap1 = lx - 110.0 - 75.0, lx - 110.0 + 75.0
                x0, x1 = -lot.width / 2 - 250.0, lot.width / 2 + 250.0
                picket_run(fence, lot, terrain, x0, fy, gap0, fy)
                if not lot.params.get("garage"):
                    picket_run(fence, lot, terrain, gap1, fy, x1, fy)
                picket_run(fence, lot, terrain, x0, fy, x0, 300.0)
                if fence.triangle_count():
                    k.place(k.static_mesh(fence, "SM_Fence_" + lot.name, mats, folder=k.MESH + "/Props"), lot.fx, lot.fy, lot.z, lot.yaw, "SPN_Fence_" + lot.name, folder="Props")
        if lot.kind == "shop":
            sx, sy, sz, width = lot.sign
            x, y, z = world(sx, sy, sz)
            label = "TOWN HALL" if lot.name == "hall" else "POST OFFICE" if lot.name == "office" else names.pop() if names else "STORE"
            text(label, x, y, z, lot.yaw - 90.0, min(46.0, width / max(4, len(label)) * 1.5))
            x, y = lot.to_world(rng.uniform(-lot.width * 0.3, lot.width * 0.3), lot.depth + 130.0)
            bins.append(xf(x, y, terrain.z(x, y), lot.yaw + rng.uniform(-10, 10)))
        if lot.kind in ("gas", "diner", "garage"):
            sx, sy, sz, width = lot.sign
            x, y, z = world(sx, sy, sz)
            text({"gas": "GAS & MART", "diner": "DINER", "garage": "AUTO REPAIR"}[lot.kind], x, y, z, lot.yaw - 90.0, 46.0 if lot.kind != "diner" else 90.0)
        if getattr(lot, "pole_sign", None):
            x, y, z = world(*lot.pole_sign)
            text({"gas": "GAS", "motel": "MOTEL"}[lot.kind], x, y, z, lot.yaw - 90.0, 95.0 if lot.kind == "motel" else 70.0, colour=(250, 240, 220))
        if lot.kind == "motel":
            for index, (px, py, pz, _) in enumerate(lot.numbers):
                pass
            for i in range(9):
                x, y = lot.to_world(-lot.width / 2 + 700.0 + (i + 0.5) * 400.0, lot.depth - 720.0 - 190.0 - 70.0)
                blocks.append(xf(x, y, terrain.z(x, y) + 5.0, lot.yaw))
        if lot.kind == "church" and getattr(lot, "fence", None):
            fence = meshkit.Mesh()
            fx0, fy0, fx1, fy1 = lot.fence
            for p, q in (((fx0, fy0), (fx1, fy0)), ((fx1, fy0), (fx1, fy1)), ((fx1, fy1), (fx0, fy1))):
                picket_run(fence, lot, terrain, p[0], p[1], q[0], q[1], key="metal_dark")
            k.place(k.static_mesh(fence, "SM_Fence_graveyard", mats, folder=k.MESH + "/Props"), lot.fx, lot.fy, lot.z, lot.yaw, "SPN_Fence_graveyard", folder="Props")
    instanced(mesh_asset("Mailbox", mailbox()), mail, "SPN_Mailboxes")
    instanced(mesh_asset("Dumpster", dumpster()), bins, "SPN_Dumpsters")
    instanced(mesh_asset("ParkingBlock", parking_block()), blocks, "SPN_ParkingBlocks")

    # ---- street furniture on the main street pavements
    benches, cans, hydrants = [], [], []
    s = main[0] + 1500.0
    index = 0
    while s < main[1] - 800.0:
        side = 1.0 if index % 2 == 0 else -1.0
        if not any(abs(s - gs) < half + 700.0 for gs, _, half in gaps):
            off = roadkit.eff_half(hwy, s, main) + 250.0
            x, y, z, yaw = hwy.offset(s, side * off)
            face = yaw + (180.0 if side > 0 else 0.0)
            benches.append(xf(x, y, z + 19.0, face))
            x, y, z, _ = hwy.offset(s + 180.0, side * off)
            cans.append(xf(x, y, z + 19.0, rng.uniform(0, 360)))
            x, y, z, _ = hwy.offset(s + 900.0, -side * (roadkit.eff_half(hwy, s, main) + 60.0))
            hydrants.append(xf(x, y, z + 19.0, rng.uniform(0, 360)))
        s += 3300.0
        index += 1
    instanced(mesh_asset("Bench", bench()), benches, "SPN_Benches")
    instanced(mesh_asset("TrashCan", trash_can()), cans, "SPN_TrashCans")
    instanced(mesh_asset("Hydrant", hydrant()), hydrants, "SPN_Hydrants")

    # ---- stop signs where the side streets meet a bigger road; speed limit signs at the town entrances
    stops, boards = [], []
    for name in roadkit.PRIORITY[1:]:
        rd = roads[name]
        if rd.kind != "street":
            continue
        for run in roadkit.covered_ranges(rd, roads, main):
            for smp, direction in ((run[0], 1.0), (run[-1], -1.0)):
                if smp[5] < 50.0 or smp[5] > rd.length - 50.0:
                    continue
                s = smp[5] + direction * 520.0
                side = -(rd.half + 130.0) if direction < 0 else (rd.half + 130.0)
                x, y, z, yaw = rd.offset(s, side)
                face = yaw + (0.0 if direction > 0 else 180.0)
                stops.append(xf(x, y, terrain.z(x, y), face))
                rad = math.radians(face)
                text("STOP", x - math.cos(rad) * 5.0, y - math.sin(rad) * 5.0, terrain.z(x, y) + 215.0, face + 180.0, 24.0, colour=(240, 240, 235))
    for s, facing in ((main[0] - 5000.0, 180.0), (main[1] + 5000.0, 0.0)):
        side = -(hwy.half + 330.0) if facing == 180.0 else (hwy.half + 330.0)
        x, y, _, yaw = hwy.offset(s, side)
        z = terrain.z(x, y)
        boards.append(xf(x, y, z, yaw + facing + 180.0))
        rad = math.radians(yaw + facing + 180.0)
        text("SPEED\nLIMIT\n25", x - math.cos(rad) * 6.0, y - math.sin(rad) * 6.0, z + 205.0, yaw + facing, 20.0, colour=(20, 20, 20))
    instanced(mesh_asset("StopSign", stop_sign()), stops, "SPN_StopSigns")
    instanced(mesh_asset("SignBoard", sign_board()), boards, "SPN_SignBoards")

    # ---- the creek under the highway: culvert head walls and guard rails
    creek = ctx["creek"]
    cross = min(creek, key=lambda p: hwy.nearest_s(p[0], p[1])[1])
    cs = hwy.nearest_s(cross[0], cross[1])[0]
    culvert = meshkit.Mesh()
    for side in (-1.0, 1.0):
        run = roadkit.samples(hwy, cs - 1500.0, cs + 1500.0, 300.0)
        off = side * (hwy.half + 95.0)
        for smp in run:
            px, py = smp[0] + smp[3] * off, smp[1] + smp[4] * off
            culvert.box("steel", px - 5, px + 5, py - 5, py + 5, smp[2] - 40.0, smp[2] + 72.0)
        for p, q in zip(run, run[1:]):
            pa = (p[0] + p[3] * (off - side * 7.0), p[1] + p[4] * (off - side * 7.0), p[2] + 55.0)
            qa = (q[0] + q[3] * (off - side * 7.0), q[1] + q[4] * (off - side * 7.0), q[2] + 55.0)
            culvert.beam("steel", pa, qa, 4.0, 30.0)
        x, y, z, yaw = hwy.offset(cs, side * (hwy.half + 520.0))
        sub = meshkit.Mesh()
        sub.box("concrete", -30, 30, -330, 330, -420, -60)
        sub.box("trim_dark", (-32 if side < 0 else 28), (-28 if side < 0 else 32), -110, 110, -400, -190)
        culvert.add(sub, x, y, z, yaw)
    k.place(k.static_mesh(culvert, "SM_Prop_Culvert", mats, folder=k.MESH + "/Props"), label="SPN_Culvert", folder="Props")

    # ---- a small dock at the pond
    pond = layout.POND
    cabin = next(l for l in lots if l.kind == "cabin")
    ang = math.atan2(cabin.y - pond["y"], cabin.x - pond["x"])
    dock = meshkit.Mesh()
    pz = terrain.heights[("pond", pond["name"])]
    dock.box("planks", -90, 90, 0, 900, 40, 50)
    for y in (40, 320, 600, 860):
        for x in (-80, 80):
            dock.cyl("planks_dark", x, y, -260, 62, 9, segs=8)
    dx, dy = pond["x"] + math.cos(ang) * pond["radius"] * 1.12, pond["y"] + math.sin(ang) * pond["radius"] * 1.12
    k.place(k.static_mesh(dock, "SM_Prop_Dock", mats, folder=k.MESH + "/Props"), dx, dy, pz, math.degrees(ang) + 90.0, "SPN_Dock", folder="Props")

    # ---- stains on parking areas and at the pumps
    decal_mat = stain_material()
    for lot in lots:
        if lot.kind not in ("gas", "motel", "diner", "garage"):
            continue
        for _ in range(9 if lot.kind != "motel" else 14):
            lx, ly = rng.uniform(-lot.width * 0.42, lot.width * 0.42), rng.uniform(-(lot.setback - lot.road_half) + 150.0, lot.depth * 0.45)
            x, y = lot.to_world(lx, ly)
            actor = k.spawn(unreal.DecalActor, x, y, terrain.z(x, y) + 10.0, rng.uniform(0, 360), pitch=-90.0, folder="Decals")
            actor.set_decal_material(decal_mat)
            size = rng.uniform(60.0, 190.0)
            actor.decal.set_editor_property("decal_size", unreal.Vector(60.0, size, size * rng.uniform(0.6, 1.4)))
    k.log("props done")


def stain_material():
    import materials
    g = k.Graph("M_SpnStain", material_domain=unreal.MaterialDomain.MD_DEFERRED_DECAL, blend_mode=unreal.BlendMode.BLEND_TRANSLUCENT)
    tex = g.tex("Stain", materials.tex("DecalStain_C"), None, "color", param=False)
    g.out(g.const((0.02, 0.018, 0.015)), k.MP.MP_BASE_COLOR)
    g.out(g.const(0.35), k.MP.MP_ROUGHNESS)
    g.out(g.mul((tex, "A"), 0.75), k.MP.MP_OPACITY)
    return g.finish()
