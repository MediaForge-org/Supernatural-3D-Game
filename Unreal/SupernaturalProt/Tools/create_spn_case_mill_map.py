"""Creates the PLACEHOLDER case level /Game/SpnTest/Lvl_SpnCaseMill (NightBuild07, first case "Die Stille Mühle", Docs/Proposals/0019, night build A).

GREYBOX ONLY: engine cubes with the project's placeholder test materials, project actors only. No final art, no final layout, no lore placement:
the developer makes the real assets later; the area is built around one ORIGIN so it can be moved into the game world as a whole later
(developer, 2026-11-25: "wenn das gut passt dann können wir den bereich einfach ins spiel übernehmen").

Layout (cm relative to ORIGIN; X east, Y north; ground top at Z = 0):

    y=6500  ..... forest (trunks) .............................................
    y=5400  +----[back door]---+-------------------+
            |  STORAGE (NW)     |  GRINDING ROOM (NE)|      MILL (18 x 14 m, roofed, dark)
    y=4700  +--[open]-----------+------[closed door]-+     creature patrols inside and behind the mill
            |  HALL: the bell hangs here, two bottles |
    y=4000  +-------------[front door]----------------+
    y=3600                   autosave "before_mill_night"
            |  road (gravel) .... street lamps ....
    y=900     house          road          house
    y=-600    house          road          house      LIBRARY (enterable, research comes in night build B)
    y=-1800                player start, autosave "arrival"

Run headless from the repository root (Windows), after building SupernaturalProtEditor:
    UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
        -script="<abs path>/create_spn_case_mill_map.py" -unattended -nullrhi
An existing map is kept unless the environment variable SPN_FORCE=1 is set.
Play it: main menu with -SpnGameplayMap=/Game/SpnTest/Lvl_SpnCaseMill (Tools/play_case_mill.ps1).
"""

import os
import random

import unreal

MAP_PATH = "/Game/SpnTest/Lvl_SpnCaseMill"
ORIGIN = (0.0, 0.0)          # move the whole area here (cm)
CUBE = "/Engine/BasicShapes/Cube.Cube"
MAT = "/Game/SpnTest/Materials/"
CLS = "/Script/SupernaturalProt."

WALL_TOP = 295.0
DOOR_HEIGHT = 220.0
THICK = 20.0
GAP = 160.0          # wide doorways: a mill's doors, and the navigation mesh (agent radius 34) needs the room with an open leaf in the gap
LEAF = 150.0
FLOOR = 0.0

M_GROUND = MAT + "MI_Spn_FloorConcrete.MI_Spn_FloorConcrete"
M_ROAD = MAT + "MI_Spn_Stone.MI_Spn_Stone"
M_HOUSE = MAT + "MI_Spn_PlasterGrey.MI_Spn_PlasterGrey"
M_HOUSE2 = MAT + "MI_Spn_PlasterCream.MI_Spn_PlasterCream"
M_ROOF = MAT + "MI_Spn_RoofDark.MI_Spn_RoofDark"
M_MILL = MAT + "MI_Spn_Brick.MI_Spn_Brick"
M_INNER = MAT + "MI_Spn_InteriorWall.MI_Spn_InteriorWall"
M_FLOOR_WOOD = MAT + "MI_Spn_FloorWood.MI_Spn_FloorWood"
M_TRUNK = MAT + "MI_Spn_FurnWoodDark.MI_Spn_FurnWoodDark"
M_WOOD = MAT + "MI_Spn_FurnWood.MI_Spn_FurnWood"
M_CRATE = MAT + "MI_Spn_Crate.MI_Spn_Crate"
M_STEEL = MAT + "MI_Spn_Steel.MI_Spn_Steel"
M_BRASS = MAT + "MI_Spn_Brass.MI_Spn_Brass"
M_DOOR_WOOD = MAT + "MI_Spn_DoorWood.MI_Spn_DoorWood"
M_DOOR_METAL = MAT + "MI_Spn_DoorMetal.MI_Spn_DoorMetal"

_assets = {}
_actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    unreal.log_error("SpnCaseMillMap: " + message)
    raise RuntimeError(message)


def asset(path):
    if path not in _assets:
        loaded = unreal.load_asset(path)
        if loaded is None:
            fail("asset not found: " + path)
        _assets[path] = loaded
    return _assets[path]


def at(x, y, z=0.0):
    return unreal.Vector(ORIGIN[0] + x, ORIGIN[1] + y, z)


def spawn(actor_class, location, yaw=0.0, label=None):
    actor = _actors.spawn_actor_from_class(actor_class, location, unreal.Rotator(roll=0.0, pitch=0.0, yaw=yaw))
    if actor is None:
        fail("could not spawn " + str(actor_class))
    if label:
        actor.set_actor_label(label)
    return actor


def box(label, x0, x1, y0, y1, z0, z1, material):
    actor = spawn(unreal.StaticMeshActor, at((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), label=label)
    actor.set_actor_scale3d(unreal.Vector((x1 - x0) / 100.0, (y1 - y0) / 100.0, (z1 - z0) / 100.0))
    mesh = actor.static_mesh_component
    mesh.set_static_mesh(asset(CUBE))
    mesh.set_material(0, asset(material))
    return actor


def wall_x(label, y, x0, x1, gaps, material, top=WALL_TOP):
    cursor = x0
    for index, centre in enumerate(sorted(gaps)):
        left, right = centre - GAP / 2.0, centre + GAP / 2.0
        box("%s_%d" % (label, index), cursor, left, y - THICK / 2.0, y + THICK / 2.0, FLOOR, top, material)
        box("%s_lintel_%d" % (label, index), left, right, y - THICK / 2.0, y + THICK / 2.0, FLOOR + DOOR_HEIGHT, top, material)
        cursor = right
    box("%s_end" % label, cursor, x1, y - THICK / 2.0, y + THICK / 2.0, FLOOR, top, material)


def wall_y(label, x, y0, y1, gaps, material, top=WALL_TOP):
    cursor = y0
    for index, centre in enumerate(sorted(gaps)):
        low, high = centre - GAP / 2.0, centre + GAP / 2.0
        box("%s_%d" % (label, index), x - THICK / 2.0, x + THICK / 2.0, cursor, low, FLOOR, top, material)
        box("%s_lintel_%d" % (label, index), x - THICK / 2.0, x + THICK / 2.0, low, high, FLOOR + DOOR_HEIGHT, top, material)
        cursor = high
    box("%s_end" % label, x - THICK / 2.0, x + THICK / 2.0, cursor, y1, FLOOR, top, material)


def door(stable_id, hinge, yaw, material, open_=False, open_angle=90.0):
    actor = spawn(unreal.load_class(None, CLS + "SpnTestDoor"), at(hinge[0], hinge[1], FLOOR), yaw=yaw, label="SPN_Door_" + stable_id)
    actor.set_editor_property("stable_id", stable_id)
    actor.set_editor_property("open_angle", open_angle)
    actor.set_editor_property("leaf_width", LEAF)
    actor.set_editor_property("leaf_material", asset(material))
    actor.set_editor_property("hardware_material", asset(M_BRASS))
    actor.set_editor_property("show_handle", True)
    actor.set_editor_property("show_hinges", True)
    if open_:
        actor.set_editor_property("authored_open", True)
    return actor


def lamp(label, location, intensity=20.0, radius=900.0, color=None):
    actor = spawn(unreal.PointLight, at(*location), label=label)
    component = actor.get_component_by_class(unreal.PointLightComponent)
    component.set_editor_property("intensity", intensity)
    component.set_editor_property("attenuation_radius", radius)
    component.set_editor_property("source_radius", 8.0)
    if color is not None:
        component.set_editor_property("light_color", color)
    return actor


def build_night():
    # a low, cold moon and almost no sky light: night. Fog for depth. Exposure is clamped so the eye does not adapt the night away.
    moon = spawn(unreal.DirectionalLight, at(0.0, 0.0, 800.0), label="SPN_Moon")
    moon.set_actor_rotation(unreal.Rotator(roll=0.0, pitch=-32.0, yaw=-140.0), False)
    mc = moon.get_component_by_class(unreal.DirectionalLightComponent)
    mc.set_mobility(unreal.ComponentMobility.MOVABLE)
    mc.set_editor_property("intensity", 0.35)
    mc.set_editor_property("light_color", unreal.Color(r=150, g=170, b=255, a=255))
    mc.set_editor_property("atmosphere_sun_light", False)   # the moon must not light the sky like a sun: the sky stays black
    spawn(unreal.SkyAtmosphere, at(0.0, 0.0, 0.0), label="SPN_SkyAtmosphere")
    sky = spawn(unreal.SkyLight, at(0.0, 0.0, 400.0), label="SPN_SkyLight")
    sc = sky.get_component_by_class(unreal.SkyLightComponent)
    sc.set_mobility(unreal.ComponentMobility.MOVABLE)
    sc.set_editor_property("real_time_capture", True)
    sc.set_editor_property("intensity", 0.25)
    fog = spawn(unreal.ExponentialHeightFog, at(0.0, 0.0, 0.0), label="SPN_Fog")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    fc.set_editor_property("fog_density", 0.035)
    fc.set_editor_property("fog_inscattering_luminance", unreal.LinearColor(0.02, 0.025, 0.04, 1.0))
    ppv = spawn(unreal.PostProcessVolume, at(0.0, 0.0, 0.0), label="SPN_NightExposure")
    ppv.set_editor_property("unbound", True)
    settings = ppv.get_editor_property("settings")
    settings.set_editor_property("override_auto_exposure_min_brightness", True)
    settings.set_editor_property("auto_exposure_min_brightness", -1.0)
    settings.set_editor_property("override_auto_exposure_max_brightness", True)
    settings.set_editor_property("auto_exposure_max_brightness", -1.0)
    settings.set_editor_property("override_motion_blur_amount", True)      # greybox QA shots after teleports stay sharp
    settings.set_editor_property("motion_blur_amount", 0.0)
    ppv.set_editor_property("settings", settings)
    box("SPN_Ground", -6000, 6000, -3000, 9000, -50.0, 0.0, M_GROUND)


def house(label, x, y, w, d, material):
    box(label, x - w / 2, x + w / 2, y - d / 2, y + d / 2, 0.0, 520.0, material)
    box(label + "_Roof", x - w / 2 - 40, x + w / 2 + 40, y - d / 2 - 40, y + d / 2 + 40, 520.0, 560.0, M_ROOF)
    # a faint window light: someone is home
    box(label + "_Window", x - 60, x + 60, y - d / 2 - 3, y - d / 2 - 1, 180.0, 300.0, M_BRASS)


def build_village():
    warm = unreal.Color(r=255, g=196, b=128, a=255)
    box("SPN_Road", -250, 250, -2400, 3800, 0.0, 2.0, M_ROAD)
    house("SPN_House_SW", -1500, -600, 900, 700, M_HOUSE)
    house("SPN_House_SE", 1500, -600, 800, 800, M_HOUSE2)
    house("SPN_House_NW", -1500, 1100, 800, 700, M_HOUSE2)
    house("SPN_House_NE", 1500, 1100, 900, 800, M_HOUSE)
    for i, y in enumerate((-1600, -400, 800, 2000, 3200)):
        x = 330 if i % 2 == 0 else -330
        box("SPN_StreetLampPole_%d" % i, x - 8, x + 8, y - 8, y + 8, 0.0, 420.0, M_STEEL)
        lamp("SPN_StreetLamp_%d" % i, (x, y, 400.0), intensity=12.0 if i < 4 else 4.0, radius=1100.0, color=warm)   # the last one, near the mill, flickers later


def build_library():
    # enterable room 10 x 8 m east of the road; the research (night build B) happens here
    x0, x1, y0, y1 = 2400, 3400, -400, 400
    box("SPN_Library_Floor", x0, x1, y0, y1, 0.0, 2.0, M_FLOOR_WOOD)
    box("SPN_Library_Roof", x0 - 20, x1 + 20, y0 - 20, y1 + 20, WALL_TOP, WALL_TOP + 20, M_ROOF)
    wall_y("SPN_Library_WallW", x0, y0 - 10, y1 + 10, [0], M_HOUSE2)
    wall_y("SPN_Library_WallE", x1, y0 - 10, y1 + 10, [], M_HOUSE2)
    wall_x("SPN_Library_WallS", y0, x0, x1, [], M_HOUSE2)
    wall_x("SPN_Library_WallN", y1, x0, x1, [], M_HOUSE2)
    door("library_door", (x0, -80), 90.0, M_DOOR_WOOD)
    for i in range(4):
        box("SPN_Library_Shelf_%d" % i, x0 + 150 + i * 220, x0 + 230 + i * 220, y1 - 60, y1 - 20, 0.0, 220.0, M_WOOD)
    box("SPN_Library_Table", 2800, 3000, -120, 40, 0.0, 78.0, M_WOOD)
    lamp("SPN_Library_Lamp", (2900, -40, 250.0), intensity=8.0, radius=700.0, color=unreal.Color(r=255, g=214, b=160, a=255))
    box("SPN_Library_Sign", x0 - 14, x0 - 12, 120, 300, 230.0, 270.0, M_BRASS)


def build_forest():
    rng = random.Random(20261125)   # deterministic: the same forest every build
    placed = 0
    for _ in range(900):
        x = rng.uniform(-5800, 5800)
        y = rng.uniform(2300, 8800)
        if -1100 < x < 1100 and 3700 < y < 5800:
            continue   # the mill and its yard
        if -400 < x < 400 and y < 3900:
            continue   # the road
        r = rng.uniform(18, 34)
        h = rng.uniform(900, 1500)
        box("SPN_Trunk_%03d" % placed, x - r, x + r, y - r, y + r, 0.0, h, M_TRUNK)
        placed += 1
        if placed >= 260:
            break


def build_mill():
    x0, x1, y0, ym, y1 = -900, 900, 4000, 4700, 5400
    box("SPN_Mill_Floor", x0, x1, y0, y1, 0.0, 2.0, M_FLOOR_WOOD)
    box("SPN_Mill_Roof", x0 - 30, x1 + 30, y0 - 30, y1 + 30, 480.0, 520.0, M_ROOF)
    top = 480.0
    wall_x("SPN_Mill_WallS", y0, x0, x1, [0], M_MILL, top)
    wall_x("SPN_Mill_WallN", y1, x0, x1, [-600], M_MILL, top)
    wall_y("SPN_Mill_WallW", x0, y0 - 10, y1 + 10, [], M_MILL, top)
    wall_y("SPN_Mill_WallE", x1, y0 - 10, y1 + 10, [], M_MILL, top)
    wall_x("SPN_Mill_WallMid", ym, x0 + 10, x1 - 10, [-500, 500], M_INNER, top)
    wall_y("SPN_Mill_WallRooms", 0, ym + 10, y1 - 10, [5050], M_INNER, top)
    door("mill_front", (-80, y0), 0.0, M_DOOR_WOOD)
    door("mill_hall_west", (-580, ym), 0.0, M_DOOR_WOOD, open_=True)     # the creature's way between the hall and the storage
    door("mill_hall_east", (420, ym), 0.0, M_DOOR_METAL)                 # closed: hiding / slamming
    door("mill_rooms", (0, 4970), 90.0, M_DOOR_WOOD, open_=True)       # open: the creature patrols through it (it opens no doors)
    door("mill_back", (-680, y1), 0.0, M_DOOR_WOOD, open_=True, open_angle=-90.0)   # open: its round leads out behind the mill
    # hall: the old machinery (blocks to sneak around), the bell under a beam
    box("SPN_Mill_Gear", 250, 650, 4200, 4500, 0.0, 180.0, M_STEEL)
    box("SPN_Mill_Sacks", -700, -350, 4120, 4300, 0.0, 110.0, M_CRATE)
    box("SPN_Mill_Beam", -300, 300, 4440, 4460, 330.0, 350.0, M_WOOD)
    bell = spawn(unreal.load_class(None, CLS + "SpnBell"), at(0.0, 4450.0, 320.0), label="SPN_Bell")
    # storage (NW) and grinding room (NE): crates and a millstone
    box("SPN_Mill_CrateA", -850, -650, 5150, 5350, 0.0, 120.0, M_CRATE)
    box("SPN_Mill_CrateB", -420, -250, 4800, 4950, 0.0, 90.0, M_CRATE)
    box("SPN_Mill_Millstone", 300, 600, 5000, 5300, 0.0, 70.0, M_STEEL)
    # one weak, cold lamp in the hall; the rooms are dark (the flashlight comes in night build C)
    lamp("SPN_Mill_HallLamp", (-600.0, 4100.0, 300.0), intensity=3.0, radius=900.0, color=unreal.Color(r=190, g=205, b=255, a=255))
    return bell


def throwable(label, x, y, z=2.0):
    actor = spawn(unreal.load_class(None, CLS + "SpnThrowablePickup"), at(x, y, z + 14.0), label=label)
    return actor


def autosave(point_id, x, y, extent):
    actor = spawn(unreal.load_class(None, CLS + "SpnAutosavePoint"), at(x, y, 150.0), label="SPN_Autosave_" + point_id)
    actor.set_editor_property("point_id", unreal.Name(point_id))
    actor.set_editor_property("extent", unreal.Vector(*extent))
    return actor


def build_gameplay():
    # bottles: three in the village, two in the mill hall
    throwable("SPN_Bottle_Village1", -900, -900)
    throwable("SPN_Bottle_Village2", 900, 300)
    throwable("SPN_Bottle_Village3", -380, 2600)
    throwable("SPN_Bottle_Mill1", -500, 4350)
    throwable("SPN_Bottle_Mill2", 700, 4100)
    autosave("arrival", 0, -1800, (400.0, 400.0, 200.0))
    autosave("before_mill_night", 0, 3600, (600.0, 200.0, 200.0))
    # the creature: inside the mill, out of the back door and around the back
    # every point on free floor: not on the millstone (300..600 / 5000..5300) or in the gear block (250..650 / 4200..4500)
    creature = spawn(unreal.load_class(None, CLS + "SpnCreature"), at(750.0, 4850.0, 110.0), yaw=180.0, label="SPN_Creature")
    route = [(750, 4850), (-500, 5100), (-500, 4450), (0, 4250), (780, 4350), (-500, 4450), (-600, 5650), (600, 5650), (-500, 5100)]
    creature.set_editor_property("patrol_points", [at(p[0], p[1], 110.0) for p in route])
    # navigation over the whole area (the mesh is generated when the level starts, see DefaultEngine.ini)
    volume = spawn(unreal.NavMeshBoundsVolume, at(0.0, 3000.0, 200.0), label="SPN_NavVolume")
    volume.set_actor_scale3d(unreal.Vector(60.0, 65.0, 6.0))
    spawn(unreal.PlayerStart, at(0.0, -1800.0, 100.0), yaw=90.0, label="SPN_PlayerStart")


def main():
    force = os.environ.get("SPN_FORCE") == "1"
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        if not force:
            unreal.log_warning("SpnCaseMillMap: " + MAP_PATH + " exists; kept unchanged (set SPN_FORCE=1 to rebuild it)")
            return
        if not levels.load_level(MAP_PATH):
            fail("could not open " + MAP_PATH)
        _actors.destroy_actors(_actors.get_all_level_actors())
    elif not levels.new_level(MAP_PATH, False):
        fail("could not create " + MAP_PATH)

    for cls in ("SpnTestDoor", "SpnCreature", "SpnBell", "SpnThrowablePickup", "SpnAutosavePoint"):
        if unreal.load_class(None, CLS + cls) is None:
            fail(cls + " not found; build SupernaturalProtEditor first")

    build_night()
    build_village()
    build_library()
    build_forest()
    build_mill()
    build_gameplay()

    if not levels.save_current_level():
        fail("could not save " + MAP_PATH)
    unreal.log("SpnCaseMillMap: created " + MAP_PATH)


main()
