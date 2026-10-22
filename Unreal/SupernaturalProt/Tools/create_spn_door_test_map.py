"""Creates the PLACEHOLDER test map /Game/SpnTest/Lvl_SpnDoorTest (Docs/Proposals/0004, increment 4).

Contents: a 40 x 40 m floor, sun, sky light and atmosphere, one Player Start, three
ASpnTestDoor actors (door_a, door_b, door_locked [locked]) and, since Phase 0.5
(Docs/Proposals/0007), two ASpnTestPickup actors (pickup_a, pickup_b) and, since M1 (Docs/Proposals/0003 section 5), a ramp, a
10-step staircase with landing and a low wall behind the doors (labels SPN_M1_*). Only engine and existing project
assets are referenced; no other level is opened for editing or changed.

Run headless from the repository root (Windows):
    UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
        -script="<abs path>/create_spn_door_test_map.py" -unattended -nullrhi
An existing test map is kept unless "--force" follows the script path inside -script="...".
"""

import sys

import unreal

MAP_PATH = "/Game/SpnTest/Lvl_SpnDoorTest"
DOOR_CLASS_PATH = "/Script/SupernaturalProt.SpnTestDoor"
PICKUP_CLASS_PATH = "/Script/SupernaturalProt.SpnTestPickup"
CUBE_PATH = "/Engine/BasicShapes/Cube.Cube"
FLOOR_MATERIAL_PATH = "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray.MI_PrototypeGrid_Gray"

# Player Start at the origin facing +X; the doors stand 4 m ahead, 3 m apart (left to right).
# Door yaw -90: the panel spans 100 cm towards -Y from the hinge (actor origin), faces the
# player and swings open away from them (+X).
PLAYER_START_LOCATION = unreal.Vector(0.0, 0.0, 100.0)
DOOR_X = 400.0
DOOR_YAW = -90.0
DOORS = [
    # (stable id, panel centre Y, authored locked)
    ("door_a", -300.0, False),
    ("door_b", 0.0, False),
    ("door_locked", 300.0, True),
]
# Two pickups of the same item between the Player Start and the doors (2.5 m ahead, 2 m apart):
# taking both shows the count adding up.
PICKUP_X = 250.0
PICKUPS = [
    # (stable id, Y, item id, amount)
    ("pickup_a", -100.0, "test_item", 1),
    ("pickup_b", 100.0, "test_item", 2),
]


# M1 movement geometry (Docs/Proposals/0003 section 5): PLACEHOLDER test shapes behind the doors,
# side by side at X >= 900 so nothing blocks the doors or pickups. Values are throw-away test values
# chosen inside the default Character Movement limits (max step 45 cm, walkable angle ~44.8 deg,
# jump apex ~127 cm); they are not gameplay numbers.
M1_X = 900.0
RAMP_Y = -800.0
RAMP_LENGTH = 800.0
RAMP_WIDTH = 300.0
RAMP_THICKNESS = 20.0
RAMP_ANGLE = 15.0  # degrees, rising towards +X
STAIRS_Y = 0.0
STAIRS_WIDTH = 300.0
STAIRS_COUNT = 10
STAIRS_RISE = 18.0
STAIRS_RUN = 30.0
STAIRS_LANDING = 300.0
WALL_Y = 800.0
WALL_X = 1300.0
WALL_WIDTH = 600.0
WALL_THICKNESS = 40.0
WALL_HEIGHT = 100.0  # low wall: jumpable


def fail(message):
    unreal.log_error("SpnDoorTestMap: " + message)
    raise RuntimeError(message)


def spawn(actor_class, location, yaw=0.0, label=None):
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actor = actors.spawn_actor_from_class(actor_class, location, unreal.Rotator(roll=0.0, pitch=0.0, yaw=yaw))
    if actor is None:
        fail("could not spawn " + str(actor_class))
    if label:
        actor.set_actor_label(label)
    return actor


def build_floor():
    cube = unreal.load_asset(CUBE_PATH)
    if cube is None:
        fail("engine cube not found: " + CUBE_PATH)
    floor = spawn(unreal.StaticMeshActor, unreal.Vector(0.0, 0.0, -50.0), label="SPN_Floor")  # top face at Z = 0
    floor.set_actor_scale3d(unreal.Vector(40.0, 40.0, 1.0))
    mesh = floor.static_mesh_component
    mesh.set_static_mesh(cube)
    material = unreal.load_asset(FLOOR_MATERIAL_PATH)
    if material is not None:
        mesh.set_material(0, material)


def build_box(label, centre, size, pitch=0.0):
    """A solid engine-cube box; size = full extents (X, Y, Z) in cm, pitch in degrees (+ raises +X)."""
    cube = unreal.load_asset(CUBE_PATH)
    box = spawn(unreal.StaticMeshActor, centre, label=label)
    box.set_actor_scale3d(unreal.Vector(size.x / 100.0, size.y / 100.0, size.z / 100.0))
    if pitch != 0.0:
        box.set_actor_rotation(unreal.Rotator(roll=0.0, pitch=pitch, yaw=0.0), False)
    mesh = box.static_mesh_component
    mesh.set_static_mesh(cube)
    material = unreal.load_asset(FLOOR_MATERIAL_PATH)
    if material is not None:
        mesh.set_material(0, material)
    return box


def build_m1_geometry():
    import math

    # Ramp: a slab whose top face starts at the floor at X = M1_X and rises towards +X.
    angle = math.radians(RAMP_ANGLE)
    half = RAMP_LENGTH / 2.0
    ramp_centre = unreal.Vector(
        M1_X + half * math.cos(angle) + RAMP_THICKNESS / 2.0 * math.sin(angle),
        RAMP_Y,
        half * math.sin(angle) - RAMP_THICKNESS / 2.0 * math.cos(angle),
    )
    build_box("SPN_M1_Ramp", ramp_centre, unreal.Vector(RAMP_LENGTH, RAMP_WIDTH, RAMP_THICKNESS), pitch=RAMP_ANGLE)

    # Stairs: each step is a solid block from the floor up to its tread, then a landing.
    for i in range(1, STAIRS_COUNT + 1):
        height = i * STAIRS_RISE
        build_box(
            "SPN_M1_Stair_%02d" % i,
            unreal.Vector(M1_X + (i - 0.5) * STAIRS_RUN, STAIRS_Y, height / 2.0),
            unreal.Vector(STAIRS_RUN, STAIRS_WIDTH, height),
        )
    top = STAIRS_COUNT * STAIRS_RISE
    build_box(
        "SPN_M1_StairLanding",
        unreal.Vector(M1_X + STAIRS_COUNT * STAIRS_RUN + STAIRS_LANDING / 2.0, STAIRS_Y, top / 2.0),
        unreal.Vector(STAIRS_LANDING, STAIRS_WIDTH, top),
    )

    # Low wall across the way (+X): blocks walking, can be jumped.
    build_box(
        "SPN_M1_Wall",
        unreal.Vector(WALL_X, WALL_Y, WALL_HEIGHT / 2.0),
        unreal.Vector(WALL_THICKNESS, WALL_WIDTH, WALL_HEIGHT),
    )


def build_lighting():
    sun = spawn(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 500.0), label="SPN_Sun")
    sun.set_actor_rotation(unreal.Rotator(roll=0.0, pitch=-50.0, yaw=30.0), False)
    sun_component = sun.get_component_by_class(unreal.DirectionalLightComponent)
    sun_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    sun_component.set_editor_property("atmosphere_sun_light", True)

    spawn(unreal.SkyAtmosphere, unreal.Vector(0.0, 0.0, 0.0), label="SPN_SkyAtmosphere")

    sky_light = spawn(unreal.SkyLight, unreal.Vector(0.0, 0.0, 300.0), label="SPN_SkyLight")
    sky_component = sky_light.get_component_by_class(unreal.SkyLightComponent)
    sky_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    sky_component.set_editor_property("real_time_capture", True)


def build_doors():
    door_class = unreal.load_class(None, DOOR_CLASS_PATH)
    if door_class is None:
        fail("ASpnTestDoor not found; build SupernaturalProtEditor first")
    for stable_id, centre_y, locked in DOORS:
        hinge_y = centre_y + 50.0  # yaw -90: the panel runs from the hinge towards -Y
        door = spawn(door_class, unreal.Vector(DOOR_X, hinge_y, 0.0), yaw=DOOR_YAW, label="SPN_TestDoor_" + stable_id)
        door.set_editor_property("stable_id", stable_id)
        door.set_editor_property("authored_locked", locked)


def build_pickups():
    pickup_class = unreal.load_class(None, PICKUP_CLASS_PATH)
    if pickup_class is None:
        fail("ASpnTestPickup not found; build SupernaturalProtEditor first")
    for stable_id, y, item_id, amount in PICKUPS:
        pickup = spawn(pickup_class, unreal.Vector(PICKUP_X, y, 0.0), label="SPN_TestPickup_" + stable_id)
        pickup.set_editor_property("stable_id", stable_id)
        pickup.set_editor_property("item_id", item_id)
        pickup.set_editor_property("amount", amount)


def main():
    force = "--force" in sys.argv[1:]
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        if not force:
            unreal.log_warning("SpnDoorTestMap: " + MAP_PATH + " exists; kept unchanged (pass --force to rebuild it)")
            return
        # Rebuild in place: open this test map only and clear its actors.
        if not levels.load_level(MAP_PATH):
            fail("could not open " + MAP_PATH)
        actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        actors.destroy_actors(actors.get_all_level_actors())
    elif not levels.new_level(MAP_PATH, False):
        fail("could not create " + MAP_PATH)

    build_floor()
    build_lighting()
    spawn(unreal.PlayerStart, PLAYER_START_LOCATION, label="SPN_PlayerStart")
    build_doors()
    build_pickups()
    build_m1_geometry()

    if not levels.save_current_level():
        fail("could not save " + MAP_PATH)
    unreal.log("SpnDoorTestMap: created " + MAP_PATH)


main()
