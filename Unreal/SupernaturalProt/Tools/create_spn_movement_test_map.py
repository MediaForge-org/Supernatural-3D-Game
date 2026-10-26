"""Creates the PLACEHOLDER test map /Game/SpnTest/Lvl_SpnMovementTest (Docs/Proposals/0011, M2-A / M2-A.1).

A 300 x 300 m movement test environment: a speed lane, obstacle rows (20-150 cm), ramps (5-50 degrees), staircases, domed hills,
a terraced hill, a field of low rocks, surface patches, a camera-collision alley, a pillar field, three simple test buildings
(hut, two-storey house with interior stairs, warehouse), rails on every raised edge, four containment walls and a play-area
guard volume with reset points.
It is NOT a geographic excerpt of the later open world. The geometry comes from spn_movement_map_layout.py
(checked without Unreal by test_spn_movement_map_layout.py); this script only spawns it.

Only engine and existing project assets are referenced. No other level is opened for editing or changed.

Run headless from the repository root (Windows), after building the editor target:
    UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
        -script="<abs path>/create_spn_movement_test_map.py" -unattended -nullrhi
An existing map is kept unless "--force" follows the script path inside -script="...".

STATUS: written on a machine without Unreal Engine; never executed. Check the log for "SpnMovementTestMap:".
"""

import os
import sys

import unreal

try:
    _HERE = os.path.dirname(os.path.abspath(__file__))
except NameError:  # some embedded runners do not define __file__
    _HERE = os.path.dirname(os.path.abspath(sys.argv[0])) if sys.argv and sys.argv[0] else os.getcwd()
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import spn_movement_map_layout as layout_module  # noqa: E402

DOOR_CLASS_PATH = "/Script/SupernaturalProt.SpnTestDoor"
GUARD_CLASS_PATH = "/Script/SupernaturalProt.SpnPlayAreaGuard"

_native_sizes = {}
_materials = {}
_meshes = {}
_actor_subsystem = None


def fail(message):
    unreal.log_error("SpnMovementTestMap: " + message)
    raise RuntimeError(message)


def actor_subsystem():
    global _actor_subsystem
    if _actor_subsystem is None:
        _actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    return _actor_subsystem


def spawn(actor_class, location, rotation=(0.0, 0.0, 0.0), label=None):
    actors = actor_subsystem()
    pitch, yaw, roll = rotation
    actor = actors.spawn_actor_from_class(actor_class, unreal.Vector(*location), unreal.Rotator(roll=roll, pitch=pitch, yaw=yaw))
    if actor is None:
        fail("could not spawn " + str(actor_class))
    if label:
        actor.set_actor_label(label)
    return actor


def load_shape(shape):
    if shape in _meshes:
        return _meshes[shape]
    mesh = unreal.load_asset(layout_module.SHAPE_PATHS[shape])
    if mesh is None:
        fail("engine shape not found: " + layout_module.SHAPE_PATHS[shape])
    _meshes[shape] = mesh
    if shape not in _native_sizes:
        # Scale = wanted size / native size, so it does not matter how big the engine shape is.
        extent = mesh.get_bounds().box_extent
        _native_sizes[shape] = (extent.x * 2.0, extent.y * 2.0, extent.z * 2.0)
        unreal.log("SpnMovementTestMap: native size of %s = %s" % (shape, _native_sizes[shape]))
    return mesh


def load_material(key):
    if key not in _materials:
        material = unreal.load_asset(layout_module.MATERIAL_PATHS[key])
        if material is None:
            unreal.log_warning("SpnMovementTestMap: material missing, using the shape default: " + layout_module.MATERIAL_PATHS[key])
        _materials[key] = material
    return _materials[key]


def spawn_prim(prim):
    mesh = load_shape(prim["shape"])
    native = _native_sizes[prim["shape"]]
    actor = spawn(unreal.StaticMeshActor, prim["center"], prim["rot"], prim["label"])
    size = prim["size"]
    actor.set_actor_scale3d(unreal.Vector(size[0] / native[0], size[1] / native[1], size[2] / native[2]))
    component = actor.static_mesh_component
    component.set_static_mesh(mesh)
    material = load_material(prim["material"])
    if material is not None:
        component.set_material(0, material)
    try:
        actor.set_folder_path("SPN_MV/" + prim["zone"])
    except Exception:  # folders are cosmetic
        pass
    return actor


def build_lighting():
    sun = spawn(unreal.DirectionalLight, (0.0, 0.0, 500.0), (-50.0, 30.0, 0.0), "SPN_Sun")
    sun_component = sun.get_component_by_class(unreal.DirectionalLightComponent)
    sun_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    sun_component.set_editor_property("atmosphere_sun_light", True)

    spawn(unreal.SkyAtmosphere, (0.0, 0.0, 0.0), label="SPN_SkyAtmosphere")

    sky_light = spawn(unreal.SkyLight, (0.0, 0.0, 300.0), label="SPN_SkyLight")
    sky_component = sky_light.get_component_by_class(unreal.SkyLightComponent)
    sky_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    sky_component.set_editor_property("real_time_capture", True)


def build_doors(data):
    door_class = unreal.load_class(None, DOOR_CLASS_PATH)
    if door_class is None:
        fail("ASpnTestDoor not found; build SupernaturalProtEditor first")
    for door in data["doors"]:
        actor = spawn(door_class, door["hinge"], (0.0, door["yaw"], 0.0), "SPN_MV_Door_" + door["id"])
        actor.set_editor_property("stable_id", door["id"])
        actor.set_editor_property("authored_locked", False)


def build_guard(data):
    guard_class = unreal.load_class(None, GUARD_CLASS_PATH)
    if guard_class is None:
        fail("ASpnPlayAreaGuard not found; build SupernaturalProtEditor first")
    spec = data["guard"]
    guard = spawn(guard_class, spec["center"], label="SPN_MV_PlayAreaGuard")
    guard.set_editor_property("box_extent", unreal.Vector(*spec["extent"]))
    guard.set_editor_property("safe_location", unreal.Vector(*spec["safe_location"]))
    guard.set_editor_property("reset_points", [unreal.Vector(*point) for point in spec["reset_points"]])


def main():
    data = layout_module.build_layout()
    map_path = data["map"]
    force = "--force" in sys.argv[1:]
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(map_path):
        if not force:
            unreal.log_warning("SpnMovementTestMap: " + map_path + " exists; kept unchanged (pass --force to rebuild it)")
            return
        if not levels.load_level(map_path):
            fail("could not open " + map_path)
        actors = actor_subsystem()
        actors.destroy_actors(actors.get_all_level_actors())
    elif not levels.new_level(map_path, False):
        fail("could not create " + map_path)

    build_lighting()
    spawn(unreal.PlayerStart, tuple(data["player_start"]), label="SPN_PlayerStart")
    for prim in data["prims"]:
        spawn_prim(prim)
    build_doors(data)
    build_guard(data)

    if not levels.save_current_level():
        fail("could not save " + map_path)
    spawned = sum(1 for actor in actor_subsystem().get_all_level_actors() if actor.get_actor_label().startswith("SPN_MV_") and isinstance(actor, unreal.StaticMeshActor))
    if spawned != len(data["prims"]):
        fail("expected %d shapes in the level, found %d (map saved, but incomplete)" % (len(data["prims"]), spawned))
    unreal.log("SpnMovementTestMap: created %s with %d shapes, %d doors, %d reset points" % (map_path, len(data["prims"]), len(data["doors"]), len(data["reset_points"])))


main()
