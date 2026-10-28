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
"--v2" builds /Game/SpnTest/Lvl_SpnMovementTestV2 (v1 plus the district from spn_district_layout.py: roads, enterable buildings,
vegetation, lamps and interior lights) instead; the v1 map is never touched by it.
"--force" removes ONLY actors this script created: actors carrying the tag SPN_GEN (set by spawn()). Actors without the tag stay,
including hand-placed ones that happen to be named SPN_*. A map made by an older generator version (SPN_MV_/SPN_D_ labels, no tag)
makes --force stop without deleting anything; add --adopt-legacy to treat those labels as generated.
"--instancing" (with --v2) draws the district primitives as instanced static mesh groups (fewer actors); the editor calls this needs
are UNCONFIRMED without an engine, so a group that cannot be built falls back to one actor per primitive.
If generation fails half-way the level is NOT saved; close it without saving.

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
import spn_district_layout as district_module  # noqa: E402
import spn_env_enrichment  # noqa: E402
import spn_instancing  # noqa: E402
import spn_map_extras  # noqa: E402
import spn_map_validate  # noqa: E402

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
    spn_map_extras.tag_actor(actor)
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


def load_material(key, for_instancing=False):
    if key not in _materials:
        material = unreal.load_asset(layout_module.MATERIAL_PATHS[key])
        if material is None:
            unreal.log_warning("SpnMovementTestMap: material missing, using the shape default: " + layout_module.MATERIAL_PATHS[key])
        elif for_instancing:
            spn_map_extras.ensure_instancing_usage(material)
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
        actor.set_editor_property("open_angle", district_module.door_open_angle(door))


def build_guard(data):
    guard_class = unreal.load_class(None, GUARD_CLASS_PATH)
    if guard_class is None:
        fail("ASpnPlayAreaGuard not found; build SupernaturalProtEditor first")
    spec = data["guard"]
    guard = spawn(guard_class, spec["center"], label="SPN_MV_PlayAreaGuard")
    guard.set_editor_property("box_extent", unreal.Vector(*spec["extent"]))
    guard.set_editor_property("safe_location", unreal.Vector(*spec["safe_location"]))
    guard.set_editor_property("reset_points", [unreal.Vector(*point) for point in spec["reset_points"]])


def preflight(data, groups):
    """Everything that can be checked before the level is touched: layout data, engine shapes, classes, materials. Raises on the first
    hard problem; a missing material only warns (the shape default is used)."""
    problems = spn_map_validate.validate_layout_data(data)
    if not problems and "district" in data:   # V2 only; the v1 data keeps its known tight house door
        problems = spn_map_validate.validate_door_passages(data)
    if problems:
        fail("layout data invalid, nothing was changed: " + "; ".join(problems[:5]))
    for shape in sorted({p["shape"] for p in data["prims"]}):
        load_shape(shape)
    for material in sorted({p["material"] for p in data["prims"]}):
        load_material(material, for_instancing=bool(groups))
    for path in (DOOR_CLASS_PATH, GUARD_CLASS_PATH) + ((spn_map_extras.INSTANCE_HOLDER_CLASS_PATH,) if groups else ()):
        if unreal.load_class(None, path) is None:
            fail("class not found, build the editor target first: " + path)
    if data.get("lights") and not hasattr(unreal, "PointLight"):
        fail("unreal.PointLight is not available")


def main():
    args = sys.argv[1:]
    v2 = "--v2" in args
    data = district_module.build_layout_v2() if v2 else layout_module.build_layout()
    if "--enrich" in args:
        # Opt-in M2A3 environment layer: its own level path (Lvl_SpnMovementTestV2Env), the plain V2 level is never touched by it.
        if not v2:
            fail("--enrich needs --v2")
        spn_env_enrichment.enrich(data)
        problems = spn_env_enrichment.validate_enrichment(data)
        if problems:
            fail("enrichment invalid, nothing was changed: " + "; ".join(problems[:5]))
    map_path = data["map"]
    force = "--force" in args
    adopt_legacy = "--adopt-legacy" in args
    instancing = "--instancing" in args and v2
    spn_map_extras.set_generator(map_path, data.get("generator_version", district_module.GENERATOR_VERSION))
    grouped = spn_instancing.plan_instancing(data["prims"])[1] if instancing else []
    preflight(data, grouped)

    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    exists = unreal.EditorAssetLibrary.does_asset_exist(map_path)
    if exists and force and not levels.load_level(map_path):
        fail("could not open " + map_path)
    actors = actor_subsystem()
    plan = spn_map_extras.rebuild_plan(actors.get_all_level_actors() if exists and force else [], exists, force, adopt_legacy)
    unreal.log("SpnMovementTestMap: %s: %s" % (plan.action, plan.reason))
    if plan.action == "keep":
        unreal.log_warning("SpnMovementTestMap: " + map_path + " exists; kept unchanged (pass --force to rebuild it)")
        return
    if plan.action == "abort":
        fail(plan.reason + ". Nothing was deleted or changed.")
    if plan.action == "create":
        if not levels.new_level(map_path, False):
            fail("could not create " + map_path)
    else:
        actors.destroy_actors(plan.remove)

    try:
        build_lighting()
        spawn(unreal.PlayerStart, tuple(data["player_start"]), label="SPN_PlayerStart")
        in_group = spn_instancing.instanced_ids(grouped)
        individual = 0
        for prim in data["prims"]:
            if id(prim) not in in_group:
                spawn_prim(prim)
                individual += 1
        for group in grouped:
            individual += spn_map_extras.spawn_instance_group(spawn, load_shape, load_material, _native_sizes, group, spawn_prim)
        build_doors(data)
        build_guard(data)
        spn_map_extras.build_lights(spawn, data)
        # Verify BEFORE saving: a level that does not match the layout is never written to disk.
        expected = spn_map_extras.expected_counts(data, [g for g in grouped if _group_actor_exists(g)], individual)
        problems = spn_map_extras.verify_level(actors.get_all_level_actors(), expected)
        if problems:
            fail("the generated level does not match the layout, NOT saved: " + "; ".join(problems[:6]))
    except Exception as error:  # the level is left UNSAVED so the file on disk stays as it was
        unreal.log_error("SpnMovementTestMap: generation failed, the level was NOT saved: %s" % error)
        raise

    if not levels.save_current_level():
        fail("could not save " + map_path)
    unreal.log("SpnMovementTestMap: created %s with %d shape actors for %d primitives (%d instance groups), %d doors, %d reset points"
               % (map_path, individual, len(data["prims"]), len(grouped), len(data["doors"]), len(data["reset_points"])))


def _group_actor_exists(group):
    """A group counts as instanced when its actor exists (the fallback destroys it)."""
    return any(a.get_actor_label() == spn_instancing.group_label(group) for a in actor_subsystem().get_all_level_actors())


main()
