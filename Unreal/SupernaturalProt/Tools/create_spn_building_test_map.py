"""Creates the PLACEHOLDER gameplay test level /Game/SpnTest/Lvl_SpnBuildingTest (NightBuild04): a small enterable building with doors.

Not the final Hunter HQ and not a final building design: engine-cube walls with the project's placeholder materials, project test actors only.

Layout (cm; X east, Y towards the back wall; floor top at Z = 15, ground at Z = 0; walls 20 thick, 280 high; doorways 110 clear):

        y=900 +-------------------------------+--------------------+
              |  STUDY (locked, needs key)    |  STORAGE (locked)  |  back door (breakable, locked) at x=450
              |  pickup, light switch         |  crate, lamp       |
        y=450 +--[study door]-------------+----+--[storage door]----+
              |  FOYER                    |  ARCHIVE (key on a table, light switch)
              |  entrance, light switch   |
        y=0   +--[front door]--------------+--------------------+
                 two porch steps, porch lamp
              x=-600          x=0                        x=600

Access: the front door is unlocked. The study door opens only with the key lying on the archive table (key.study). The storage door and the back
door are locked but breakable (F / DualSense Triangle); the storage connects to the study through an unlocked inner door, so the study can also be
reached without the key by breaking a door open.

Run headless from the repository root (Windows):
    UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
        -script="<abs path>/create_spn_building_test_map.py" -unattended -nullrhi
An existing map is kept unless the environment variable SPN_FORCE=1 is set.
"""

import os

import unreal

MAP_PATH = "/Game/SpnTest/Lvl_SpnBuildingTest"
CUBE = "/Engine/BasicShapes/Cube.Cube"
MAT = "/Game/SpnTest/Materials/"
CLS = "/Script/SupernaturalProt."

FLOOR_TOP = 15.0
WALL_TOP = 295.0
DOOR_HEIGHT = 220.0
THICK = 20.0
GAP = 110.0

M_OUTER = MAT + "MI_Spn_PlasterCream.MI_Spn_PlasterCream"
M_INNER = MAT + "MI_Spn_InteriorWall.MI_Spn_InteriorWall"
M_FLOOR = MAT + "MI_Spn_FloorWood.MI_Spn_FloorWood"
M_CEIL = MAT + "MI_Spn_Ceiling.MI_Spn_Ceiling"
M_GROUND = MAT + "MI_Spn_FloorConcrete.MI_Spn_FloorConcrete"
M_STEP = MAT + "MI_Spn_Stone.MI_Spn_Stone"
M_WOOD = MAT + "MI_Spn_FurnWood.MI_Spn_FurnWood"
M_DARKWOOD = MAT + "MI_Spn_FurnWoodDark.MI_Spn_FurnWoodDark"
M_CRATE = MAT + "MI_Spn_Crate.MI_Spn_Crate"
M_BRASS = MAT + "MI_Spn_Brass.MI_Spn_Brass"
M_DOOR_WOOD = MAT + "MI_Spn_DoorWood.MI_Spn_DoorWood"
M_DOOR_RED = MAT + "MI_Spn_DoorRed.MI_Spn_DoorRed"
M_DOOR_METAL = MAT + "MI_Spn_DoorMetal.MI_Spn_DoorMetal"

_assets = {}
_actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    unreal.log_error("SpnBuildingTestMap: " + message)
    raise RuntimeError(message)


def asset(path):
    if path not in _assets:
        loaded = unreal.load_asset(path)
        if loaded is None:
            fail("asset not found: " + path)
        _assets[path] = loaded
    return _assets[path]


def spawn(actor_class, location, yaw=0.0, label=None):
    actor = _actors.spawn_actor_from_class(actor_class, location, unreal.Rotator(roll=0.0, pitch=0.0, yaw=yaw))
    if actor is None:
        fail("could not spawn " + str(actor_class))
    if label:
        actor.set_actor_label(label)
    return actor


def box(label, x0, x1, y0, y1, z0, z1, material):
    """Solid engine-cube box between the given bounds (cm)."""
    actor = spawn(unreal.StaticMeshActor, unreal.Vector((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), label=label)
    actor.set_actor_scale3d(unreal.Vector((x1 - x0) / 100.0, (y1 - y0) / 100.0, (z1 - z0) / 100.0))
    mesh = actor.static_mesh_component
    mesh.set_static_mesh(asset(CUBE))
    mesh.set_material(0, asset(material))
    return actor


def wall_x(label, y, x0, x1, gaps, material):
    """Wall along X at y, with doorways [(centre x, ...)] (clear width GAP, lintel above DOOR_HEIGHT)."""
    cursor = x0
    for index, centre in enumerate(sorted(gaps)):
        left, right = centre - GAP / 2.0, centre + GAP / 2.0
        box("%s_%d" % (label, index), cursor, left, y - THICK / 2.0, y + THICK / 2.0, FLOOR_TOP, WALL_TOP, material)
        box("%s_lintel_%d" % (label, index), left, right, y - THICK / 2.0, y + THICK / 2.0, FLOOR_TOP + DOOR_HEIGHT, WALL_TOP, material)
        cursor = right
    box("%s_end" % label, cursor, x1, y - THICK / 2.0, y + THICK / 2.0, FLOOR_TOP, WALL_TOP, material)


def wall_y(label, x, y0, y1, gaps, material):
    cursor = y0
    for index, centre in enumerate(sorted(gaps)):
        low, high = centre - GAP / 2.0, centre + GAP / 2.0
        box("%s_%d" % (label, index), x - THICK / 2.0, x + THICK / 2.0, cursor, low, FLOOR_TOP, WALL_TOP, material)
        box("%s_lintel_%d" % (label, index), x - THICK / 2.0, x + THICK / 2.0, low, high, FLOOR_TOP + DOOR_HEIGHT, WALL_TOP, material)
        cursor = high
    box("%s_end" % label, x - THICK / 2.0, x + THICK / 2.0, cursor, y1, FLOOR_TOP, WALL_TOP, material)


def door(stable_id, hinge, yaw, material, locked=False, breakable=False, key="", open_angle=90.0):
    actor = spawn(unreal.load_class(None, CLS + "SpnTestDoor"), unreal.Vector(hinge[0], hinge[1], FLOOR_TOP), yaw=yaw, label="SPN_Door_" + stable_id)
    actor.set_editor_property("stable_id", stable_id)
    actor.set_editor_property("authored_locked", locked)
    actor.set_editor_property("breakable", breakable)
    actor.set_editor_property("required_key_id", key)
    actor.set_editor_property("open_angle", open_angle)
    actor.set_editor_property("leaf_material", asset(material))
    actor.set_editor_property("hardware_material", asset(M_BRASS))
    actor.set_editor_property("show_handle", True)
    actor.set_editor_property("show_hinges", True)
    return actor


def lamp(label, location, intensity=30.0, radius=800.0, color=None):
    actor = spawn(unreal.PointLight, unreal.Vector(*location), label=label)
    component = actor.get_component_by_class(unreal.PointLightComponent)
    component.set_mobility(unreal.ComponentMobility.MOVABLE)
    component.set_editor_property("intensity", intensity)
    component.set_editor_property("attenuation_radius", radius)
    component.set_editor_property("source_radius", 8.0)
    if color is not None:
        component.set_editor_property("light_color", color)
    return actor


def switch(stable_id, location, yaw, lights):
    actor = spawn(unreal.load_class(None, CLS + "SpnLightSwitch"), unreal.Vector(*location), yaw=yaw, label="SPN_Switch_" + stable_id)
    actor.set_editor_property("stable_id", stable_id)
    actor.set_editor_property("lights", lights)
    return actor


def build_environment():
    sun = spawn(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 500.0), label="SPN_Sun")
    sun.set_actor_rotation(unreal.Rotator(roll=0.0, pitch=-14.0, yaw=60.0), False)   # low sun: dusk, the lamps matter
    component = sun.get_component_by_class(unreal.DirectionalLightComponent)
    component.set_mobility(unreal.ComponentMobility.MOVABLE)
    component.set_editor_property("atmosphere_sun_light", True)
    spawn(unreal.SkyAtmosphere, unreal.Vector(0.0, 0.0, 0.0), label="SPN_SkyAtmosphere")
    sky = spawn(unreal.SkyLight, unreal.Vector(0.0, 0.0, 300.0), label="SPN_SkyLight")
    sky_component = sky.get_component_by_class(unreal.SkyLightComponent)
    sky_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    sky_component.set_editor_property("real_time_capture", True)
    box("SPN_Ground", -3000, 3000, -3000, 3000, -50.0, 0.0, M_GROUND)


def build_shell():
    box("SPN_Floor", -600, 600, 0, 900, 0.0, FLOOR_TOP, M_FLOOR)
    box("SPN_Ceiling", -610, 610, -10, 910, WALL_TOP, WALL_TOP + 20.0, M_CEIL)
    # porch: two steps in front of the entrance (7.5 and 15 cm), wider than the door
    box("SPN_PorchStep1", -420, -180, -90, -10, 0.0, 7.5, M_STEP)
    box("SPN_PorchStep2", -420, -180, -40, -10, 0.0, FLOOR_TOP, M_STEP)

    wall_x("SPN_WallSouth", 0, -610, 610, [-300], M_OUTER)
    wall_x("SPN_WallNorth", 900, -610, 610, [450], M_OUTER)
    wall_y("SPN_WallWest", -600, 10, 890, [], M_OUTER)
    wall_y("SPN_WallEast", 600, 10, 890, [], M_OUTER)
    wall_y("SPN_WallFoyerArchive", 0, 10, 440, [225], M_INNER)
    wall_x("SPN_WallMiddle", 450, -590, 590, [-350, 250], M_INNER)
    wall_y("SPN_WallStudyStorage", -100, 460, 890, [675], M_INNER)


def build_doors():
    door("front_door", (-355, 0), 0.0, M_DOOR_WOOD)                                                     # unlocked
    door("archive_door", (0, 170), 90.0, M_DOOR_WOOD)                                                   # unlocked
    door("study_door", (-405, 450), 0.0, M_DOOR_RED, locked=True, key="key.study")                      # key only
    door("storage_door", (195, 450), 0.0, M_DOOR_METAL, locked=True, breakable=True)                     # break it
    door("back_door", (395, 900), 0.0, M_DOOR_METAL, locked=True, breakable=True, open_angle=-90.0)     # second access, opens inwards
    door("inner_door", (-100, 620), 90.0, M_DOOR_WOOD)                                                  # storage <-> study, unlocked


def build_furniture_and_objects(lights):
    # archive: a table with the key, a low bench near the door (a narrow passage)
    box("SPN_Table", 240, 360, 60, 120, FLOOR_TOP, FLOOR_TOP + 75.0, M_DARKWOOD)
    key = spawn(unreal.load_class(None, CLS + "SpnTestKey"), unreal.Vector(300, 90, FLOOR_TOP + 75.0 + 2.0), label="SPN_Key_Study")
    key.set_editor_property("key_id", "key.study")
    box("SPN_Bench", 30, 70, 300, 420, FLOOR_TOP, FLOOR_TOP + 45.0, M_WOOD)         # 45 cm: still a step the character can mount
    # foyer: a shelf narrowing the way to the archive door (about 95 cm of free passage)
    box("SPN_Shelf", -590, -560, 150, 330, FLOOR_TOP, FLOOR_TOP + 190.0, M_WOOD)
    box("SPN_Rug", -430, -170, 120, 330, FLOOR_TOP, FLOOR_TOP + 1.0, M_CRATE)
    # storage: crates, one of them a 40 cm step
    box("SPN_Crate1", 520, 590, 800, 880, FLOOR_TOP, FLOOR_TOP + 80.0, M_CRATE)
    box("SPN_Crate2", 330, 410, 800, 880, FLOOR_TOP, FLOOR_TOP + 40.0, M_CRATE)
    box("SPN_Crate3", 60, 150, 470, 530, FLOOR_TOP, FLOOR_TOP + 60.0, M_CRATE)
    # study: a desk and the pickup on it
    box("SPN_Desk", -520, -400, 780, 860, FLOOR_TOP, FLOOR_TOP + 75.0, M_DARKWOOD)
    pickup = spawn(unreal.load_class(None, CLS + "SpnTestPickup"), unreal.Vector(-460, 820, FLOOR_TOP + 75.0 + 8.0), label="SPN_Pickup_StudyNote")
    pickup.set_editor_property("stable_id", "study_note")
    pickup.set_editor_property("item_id", "test_item")
    pickup.set_editor_property("amount", 1)

    # light switches next to the doors (thin axis towards the wall)
    switch("foyer_light", (-230, 12, 140), 90.0, [lights["foyer"]])
    switch("archive_light", (12, 300, 140), 0.0, [lights["archive"]])
    switch("study_light", (-250, 463, 140), 90.0, [lights["study"]])


def build_lights():
    warm = unreal.Color(r=255, g=209, b=148, a=255)
    lights = {
        "foyer": lamp("SPN_Lamp_Foyer", (-300, 225, 250), color=warm),
        "archive": lamp("SPN_Lamp_Archive", (300, 225, 250), color=warm),
        "study": lamp("SPN_Lamp_Study", (-350, 675, 250), color=warm),
        "storage": lamp("SPN_Lamp_Storage", (250, 675, 250), color=unreal.Color(r=217, g=230, b=255, a=255)),
    }
    # outside: porch lamp over the entrance and a lamp at the back door (always on)
    lamp("SPN_Lamp_Porch", (-300, -70, 270), intensity=40.0, radius=900.0, color=warm)
    lamp("SPN_Lamp_Back", (450, 960, 270), intensity=40.0, radius=900.0, color=warm)
    box("SPN_PorchFixture", -306, -294, -76, -64, 262, 278, M_BRASS)
    box("SPN_BackFixture", 444, 456, 954, 966, 262, 278, M_BRASS)
    return lights


def main():
    force = os.environ.get("SPN_FORCE") == "1"
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        if not force:
            unreal.log_warning("SpnBuildingTestMap: " + MAP_PATH + " exists; kept unchanged (set SPN_FORCE=1 to rebuild it)")
            return
        if not levels.load_level(MAP_PATH):
            fail("could not open " + MAP_PATH)
        _actors.destroy_actors(_actors.get_all_level_actors())
    elif not levels.new_level(MAP_PATH, False):
        fail("could not create " + MAP_PATH)

    for cls in ("SpnTestDoor", "SpnTestKey", "SpnLightSwitch", "SpnTestPickup"):
        if unreal.load_class(None, CLS + cls) is None:
            fail(cls + " not found; build SupernaturalProtEditor first")

    build_environment()
    build_shell()
    build_doors()
    lights = build_lights()
    build_furniture_and_objects(lights)
    spawn(unreal.PlayerStart, unreal.Vector(-300, -600, 100), yaw=90.0, label="SPN_PlayerStart")   # in front of the entrance, facing the building

    if not levels.save_current_level():
        fail("could not save " + MAP_PATH)
    unreal.log("SpnBuildingTestMap: created " + MAP_PATH)


main()
