"""Creates the TEMPORARY main-menu level /Game/SpnTest/Lvl_SpnMainMenu (Night Build 01, work package E).

A small night scene built only from engine primitives (cubes), one dim moon light, fog and a few warm point lights, plus a camera tagged
SpnMenuCamera. It is a PLACEHOLDER backdrop: no production asset, no vehicle model, no claim about the game world. The level uses the game mode
ASpnMainMenuGameMode (no pawn); the menu widgets are created by USpnUiSubsystem when the level has loaded.

Run headless from the repository root (Windows):
    UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
        -script="<abs path>/create_spn_main_menu_map.py" -unattended -nullrhi
An existing menu map is kept unless "--force" follows the script path inside -script="...".
"""

import sys

import unreal

MAP_PATH = "/Game/SpnTest/Lvl_SpnMainMenu"
GAME_MODE_PATH = "/Script/SupernaturalProt.SpnMainMenuGameMode"
CUBE_PATH = "/Engine/BasicShapes/Cube.Cube"
GROUND_MATERIAL_PATH = "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray.MI_PrototypeGrid_Gray"

# (label, centre x, y, size x, y, z) - dark silhouettes of a few buildings along a road, right of the camera axis
BUILDINGS = [
    ("SPN_Menu_House_A", 1500.0, 500.0, 600.0, 500.0, 420.0),
    ("SPN_Menu_House_B", 2300.0, 900.0, 800.0, 600.0, 640.0),
    ("SPN_Menu_House_C", 3100.0, 300.0, 700.0, 500.0, 380.0),
    ("SPN_Menu_Barn", 2000.0, -700.0, 900.0, 600.0, 520.0),
    ("SPN_Menu_Wall", 900.0, 1200.0, 1600.0, 40.0, 160.0),
]
# warm window lights in front of the houses: (x, y, z)
WINDOW_LIGHTS = [(1190.0, 500.0, 200.0), (1890.0, 900.0, 260.0), (2740.0, 300.0, 180.0), (1700.0, -420.0, 240.0)]


def fail(message):
    unreal.log_error("SpnMainMenuMap: " + message)
    raise RuntimeError(message)


def spawn(actor_class, location, rotation=None, label=None):
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actor = actors.spawn_actor_from_class(actor_class, location, rotation or unreal.Rotator(0.0, 0.0, 0.0))
    if actor is None:
        fail("could not spawn " + str(actor_class))
    if label:
        actor.set_actor_label(label)
    return actor


def box(label, centre, size, dark=True):
    cube = unreal.load_asset(CUBE_PATH)
    actor = spawn(unreal.StaticMeshActor, centre, label=label)
    actor.set_actor_scale3d(unreal.Vector(size[0] / 100.0, size[1] / 100.0, size[2] / 100.0))
    mesh = actor.static_mesh_component
    mesh.set_static_mesh(cube)
    material = unreal.load_asset(GROUND_MATERIAL_PATH)
    if material is not None:
        mesh.set_material(0, material)
    return actor


def build():
    # ground (top face at Z = 0) and a road strip
    box("SPN_Menu_Ground", unreal.Vector(1500.0, 0.0, -50.0), (20000.0, 20000.0, 100.0))
    box("SPN_Menu_Road", unreal.Vector(1500.0, 0.0, 1.0), (6000.0, 600.0, 4.0))
    for label, x, y, sx, sy, sz in BUILDINGS:
        box(label, unreal.Vector(x, y, sz / 2.0), (sx, sy, sz))

    # moon: a dim, cold directional light; no sky atmosphere (black sky)
    moon = spawn(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 800.0), unreal.Rotator(roll=0.0, pitch=-24.0, yaw=210.0), label="SPN_Menu_Moon")
    moon_component = moon.get_component_by_class(unreal.DirectionalLightComponent)
    moon_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    moon_component.set_editor_property("intensity", 1.6)
    moon_component.set_editor_property("light_color", unreal.Color(150, 175, 255, 255))

    sky = spawn(unreal.SkyLight, unreal.Vector(0.0, 0.0, 400.0), label="SPN_Menu_SkyLight")
    sky_component = sky.get_component_by_class(unreal.SkyLightComponent)
    sky_component.set_mobility(unreal.ComponentMobility.MOVABLE)
    sky_component.set_editor_property("intensity", 0.35)
    sky_component.set_editor_property("source_type", unreal.SkyLightSourceType.SLS_CAPTURED_SCENE)
    sky_component.set_editor_property("real_time_capture", False)

    fog = spawn(unreal.ExponentialHeightFog, unreal.Vector(0.0, 0.0, 0.0), label="SPN_Menu_Fog")
    fog_component = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    fog_component.set_editor_property("fog_density", 0.012)
    fog_component.set_editor_property("fog_inscattering_luminance", unreal.LinearColor(0.02, 0.03, 0.05, 1.0))

    for i, (x, y, z) in enumerate(WINDOW_LIGHTS):
        light = spawn(unreal.PointLight, unreal.Vector(x, y, z), label="SPN_Menu_Window_%d" % i)
        component = light.get_component_by_class(unreal.PointLightComponent)
        component.set_mobility(unreal.ComponentMobility.MOVABLE)
        component.set_editor_property("intensity", 2600.0)
        component.set_editor_property("attenuation_radius", 700.0)
        component.set_editor_property("light_color", unreal.Color(255, 180, 100, 255))

    # exposure: an unbound volume that keeps the scene dark instead of auto-exposing the night to daylight
    volume = spawn(unreal.PostProcessVolume, unreal.Vector(0.0, 0.0, 0.0), label="SPN_Menu_PostProcess")
    volume.set_editor_property("unbound", True)
    settings = volume.get_editor_property("settings")
    settings.set_editor_property("override_auto_exposure_method", True)
    settings.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_MANUAL)
    settings.set_editor_property("override_auto_exposure_bias", True)
    settings.set_editor_property("auto_exposure_bias", 6.0)
    volume.set_editor_property("settings", settings)

    # the camera the menu looks through; yawed to the left so the scene sits right of the menu column
    camera = spawn(unreal.CameraActor, unreal.Vector(-700.0, 0.0, 180.0), unreal.Rotator(roll=0.0, pitch=4.0, yaw=-14.0), label="SPN_Menu_Camera")
    camera.tags = [unreal.Name("SpnMenuCamera")]
    camera.get_component_by_class(unreal.CameraComponent).set_editor_property("field_of_view", 62.0)

    world = unreal.EditorLevelLibrary.get_editor_world()
    game_mode = unreal.load_class(None, GAME_MODE_PATH)
    if game_mode is None:
        fail("ASpnMainMenuGameMode not found; build SupernaturalProtEditor first")
    world.get_world_settings().set_editor_property("default_game_mode", game_mode)


def main():
    force = "--force" in sys.argv[1:]
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        if not force:
            unreal.log_warning("SpnMainMenuMap: " + MAP_PATH + " exists; kept unchanged (pass --force to rebuild it)")
            return
        if not levels.load_level(MAP_PATH):
            fail("could not open " + MAP_PATH)
        actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        actors.destroy_actors(actors.get_all_level_actors())
    elif not levels.new_level(MAP_PATH, False):
        fail("could not create " + MAP_PATH)
    build()
    if not levels.save_current_level():
        fail("could not save " + MAP_PATH)
    unreal.log("SpnMainMenuMap: created " + MAP_PATH)


main()
