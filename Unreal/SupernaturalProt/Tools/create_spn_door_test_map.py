"""Creates the PLACEHOLDER test map /Game/SpnTest/Lvl_SpnDoorTest (Docs/Proposals/0004, increment 4).

Contents: a 40 x 40 m floor, sun, sky light and atmosphere, one Player Start and three
ASpnTestDoor actors (door_a, door_b, door_locked [locked]). Only engine and existing project
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

    if not levels.save_current_level():
        fail("could not save " + MAP_PATH)
    unreal.log("SpnDoorTestMap: created " + MAP_PATH)


main()
