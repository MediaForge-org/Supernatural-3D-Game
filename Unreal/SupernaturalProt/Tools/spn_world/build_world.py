"""Creates the PLACEHOLDER test world /Game/SpnWorld/Lvl_SpnWorld of the visual build (not a final map, see layout.py).

Run from the repository root with rendering enabled (the landscape import needs a GPU):
    UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript
        -script="<abs>/Unreal/SupernaturalProt/Tools/spn_world/build_world.py" -unattended -AllowCommandletRendering
        -EnablePlugins=GeometryScripting,ProceduralVegetationEditor
Needs Tools/SpnWorldTool built and its textures generated (Saved/SpnWorldGen/textures).

SPN_PHASE selects how much of the world is built, cumulatively, so every phase of the visual build can be reproduced and recorded:
    B terrain   C roads   D buildings   E materials (textures)   F vegetation   G lighting and weather   H details   (default: H)
"""

import importlib
import os
import sys
import time

import unreal

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import layout, meshkit, archkit, terrain_gen, ue_kit, fab, materials, nature  # noqa: E402,E401

for module in (layout, meshkit, archkit, terrain_gen, ue_kit, fab, materials, nature):
    importlib.reload(module)
k = ue_kit

PHASE = os.environ.get("SPN_PHASE", "H").upper()
MAP = os.environ.get("SPN_MAP", "/Game/SpnWorld/Lvl_SpnWorld")
GEN = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_saved_dir()), "SpnWorldGen")


def has(letter):
    return PHASE >= letter


# Sun shadow resolution per scenario (virtual shadow maps; +1 halves the texel density). The engine's Epic scalability sets -1.5. Measured on
# the forest track at 1440p (2026-11-13): -1.5 35 fps, -0.5 42, 0 44.5, +1 47; at 100 % crops -1.5 and -0.5 look the same. Soft-light
# scenarios go coarser because their shadows are blurred by the large light source anyway.
SHADOW_RES = "r.Shadow.Virtual.ResolutionLodBiasDirectional %.1f"


def scenario(name, **values):
    s = unreal.SpnLightingScenario()
    s.set_editor_property("name", name)
    for key, value in values.items():
        s.set_editor_property(key, value)
    return s


def build_sky(mats):
    full = has("G")
    sun = k.spawn(unreal.DirectionalLight, 0, 0, 5000, label="SPN_Sun", folder="Sky")
    sun.set_editor_property("tags", ["SpnSun"])
    c = sun.get_component_by_class(unreal.DirectionalLightComponent)
    c.set_mobility(unreal.ComponentMobility.MOVABLE)
    c.set_editor_property("atmosphere_sun_light", True)
    c.set_editor_property("intensity", 90000.0 if full else 60000.0)
    sun.set_actor_rotation(unreal.Rotator(roll=0.0, pitch=-48.0, yaw=215.0), False)
    k.spawn(unreal.SkyAtmosphere, label="SPN_SkyAtmosphere", folder="Sky")
    sky = k.spawn(unreal.SkyLight, 0, 0, 6000, label="SPN_SkyLight", folder="Sky")
    sc = sky.get_component_by_class(unreal.SkyLightComponent)
    sc.set_mobility(unreal.ComponentMobility.MOVABLE)
    sc.set_editor_property("real_time_capture", True)
    post = k.spawn(unreal.PostProcessVolume, label="SPN_Post", folder="Sky")
    post.set_editor_property("unbound", True)
    post.set_editor_property("tags", ["SpnPost"])
    settings = post.get_editor_property("settings")
    for key, value in (("override_auto_exposure_min_brightness", True), ("auto_exposure_min_brightness", 12.5), ("override_auto_exposure_max_brightness", True), ("auto_exposure_max_brightness", 14.5)):
        settings.set_editor_property(key, value)
    post.set_editor_property("settings", settings)
    if not full:
        return
    moon = k.spawn(unreal.DirectionalLight, 0, 0, 5200, label="SPN_Moon", folder="Sky")
    moon.set_editor_property("tags", ["SpnMoon"])
    mc = moon.get_component_by_class(unreal.DirectionalLightComponent)
    mc.set_mobility(unreal.ComponentMobility.MOVABLE)
    mc.set_editor_property("atmosphere_sun_light", True)
    mc.set_editor_property("atmosphere_sun_light_index", 1)
    mc.set_editor_property("intensity", 0.0)
    moon.set_actor_rotation(unreal.Rotator(roll=0.0, pitch=-38.0, yaw=70.0), False)
    fog = k.spawn(unreal.ExponentialHeightFog, 0, 0, 0, label="SPN_Fog", folder="Sky")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    fc.set_editor_property("enable_volumetric_fog", True)
    fc.set_editor_property("fog_density", 0.012)
    cloud = k.spawn(unreal.VolumetricCloud, label="SPN_Clouds", folder="Sky")
    cloud.get_component_by_class(unreal.VolumetricCloudComponent).set_editor_property("material", k.load("/Engine/EngineSky/VolumetricClouds/m_SimpleVolumetricCloud_Inst"))

    lighting = k.spawn(unreal.load_class(None, "/Script/SupernaturalProt.SpnWorldLighting"), label="SPN_WorldLighting", folder="Sky")
    lighting.set_editor_property("collection", mats["_mpc"])
    white = unreal.LinearColor(1, 1, 1, 1)
    lighting.set_editor_property("scenarios", [
        scenario("day", sun_pitch=-46.0, sun_yaw=215.0, sun_lux=95000.0, sun_color=unreal.LinearColor(1.0, 0.96, 0.90, 1), sky_light_intensity=1.0,
                 fog_density=0.010, fog_height_falloff=0.12, volumetric_fog_extinction_scale=0.6, cloud_scalars={"Cloud_GlobalCoverage": -0.12, "Cloud_GlobalDensity": 0.008},
                 exposure_compensation=-0.2, exposure_min_ev=11.4, exposure_max_ev=14.6, interior_exposure_min_ev=6.0, interior_exposure_max_ev=9.4, white_temp=6400.0, saturation=0.96, contrast=1.04,
                 bloom_intensity=0.35, vignette_intensity=0.25, lamps_on=False, collection_scalars={"Wetness": 0.0, "LampGlow": 0.0, "WindowGlow": 0.0},
                 console_commands=[SHADOW_RES % -0.5]),
        scenario("overcast", sun_pitch=-52.0, sun_yaw=200.0, sun_lux=9000.0, sun_color=unreal.LinearColor(0.86, 0.90, 0.95, 1), sun_source_angle=18.0, sky_light_intensity=1.25,
                 sky_light_color=unreal.LinearColor(0.86, 0.90, 0.96, 1), fog_density=0.045, fog_height_falloff=0.06, volumetric_fog_extinction_scale=1.4,
                 volumetric_fog_albedo=unreal.LinearColor(0.82, 0.86, 0.90, 1), cloud_scalars={"Cloud_GlobalCoverage": 0.62, "Cloud_GlobalDensity": 0.012},
                 exposure_compensation=-0.4, exposure_min_ev=9.6, exposure_max_ev=12.4, interior_exposure_min_ev=6.0, interior_exposure_max_ev=9.6, white_temp=7000.0, saturation=0.82, contrast=1.0,
                 color_gain=unreal.LinearColor(0.96, 0.99, 1.03, 1), bloom_intensity=0.3, vignette_intensity=0.35, lamps_on=False, mist_density=0.35,
                 collection_scalars={"Wetness": 1.0, "LampGlow": 0.0, "WindowGlow": 0.25}, console_commands=[SHADOW_RES % 0.0]),
        # Look of a damp, heavily clouded late afternoon in the Pacific north-west (reference: the TV series the game is modelled on):
        # no direct sun, cold flat light, muted greens, crushed but readable shadows, mist lying in the forest. Daylight, not night.
        scenario("spn_overcast", sun_pitch=-34.0, sun_yaw=235.0, sun_lux=3800.0, sun_color=unreal.LinearColor(0.78, 0.84, 0.92, 1), sun_source_angle=40.0,
                 sky_light_intensity=1.15, sky_light_color=unreal.LinearColor(0.74, 0.82, 0.90, 1), fog_density=0.040, fog_height_falloff=0.11,
                 volumetric_fog_extinction_scale=1.25, volumetric_fog_albedo=unreal.LinearColor(0.76, 0.81, 0.84, 1), volumetric_fog_scattering_distribution=0.35,
                 cloud_scalars={"Cloud_GlobalCoverage": 0.78, "Cloud_GlobalDensity": 0.016},
                 exposure_compensation=0.0, exposure_min_ev=7.4, exposure_max_ev=10.8, interior_exposure_min_ev=5.0, interior_exposure_max_ev=8.2,
                 white_temp=7600.0, saturation=0.68, contrast=1.10, color_gain=unreal.LinearColor(0.94, 1.0, 1.02, 1), bloom_intensity=0.22,
                 vignette_intensity=0.5, lamps_on=False, mist_density=1.0, mist_albedo=unreal.LinearColor(0.78, 0.82, 0.85, 1),
                 collection_scalars={"Wetness": 0.75, "LampGlow": 0.0, "WindowGlow": 0.35}, console_commands=[SHADOW_RES % 1.0]),
        scenario("night", sun_pitch=24.0, sun_yaw=250.0, sun_lux=0.0, moon_lux=0.22, moon_color=unreal.LinearColor(0.55, 0.68, 1.0, 1), sky_light_intensity=1.0,
                 sky_light_color=unreal.LinearColor(0.60, 0.72, 1.0, 1), fog_density=0.030, fog_height_falloff=0.10, volumetric_fog_extinction_scale=1.0,
                 volumetric_fog_albedo=white, cloud_scalars={"Cloud_GlobalCoverage": 0.05, "Cloud_GlobalDensity": 0.008},
                 exposure_compensation=0.0, exposure_min_ev=0.6, exposure_max_ev=2.6, interior_exposure_min_ev=0.6, interior_exposure_max_ev=6.0, white_temp=6000.0, saturation=0.9, contrast=1.06,
                 bloom_intensity=0.6, vignette_intensity=0.4, lamps_on=True, mist_density=0.5, mist_albedo=unreal.LinearColor(0.5, 0.56, 0.66, 1), collection_scalars={"Wetness": 0.35, "LampGlow": 1.0, "WindowGlow": 1.0},
                 console_commands=[SHADOW_RES % 0.5]),
    ])
    lighting.set_editor_property("initial_scenario", "day")
    # render resolution with TSR: PROVISIONAL standard 90 %, quality mode 100 % (`Spn.Quality`); only this level, never the QA test maps
    lighting.set_editor_property("default_screen_percentage", 90.0)
    lighting.set_editor_property("quality_screen_percentage", 100.0)


def main():
    started = time.time()
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    k.log("phase %s -> %s" % (PHASE, MAP))
    k.calibrate_winding()
    k.import_textures(os.path.join(GEN, "textures"), force=os.environ.get("SPN_FORCE_TEX") == "1")
    roads, lots, terrain, creek = terrain_gen.generate(os.path.join(GEN, "terrain"))

    grass_types = None
    if has("F"):
        grass_types = nature.grass_types()
    mats = materials.build(has("E"), grass_types)

    if k.eal.does_asset_exist(MAP):
        if not (k.eal.does_asset_exist("/Game/SpnWorld/_Scratch") and levels.load_level("/Game/SpnWorld/_Scratch")):
            levels.new_level("/Game/SpnWorld/_Scratch", False)
        if not k.eal.delete_asset(MAP):
            k.fail("could not delete the old " + MAP)
    if not levels.new_level(MAP, False):
        k.fail("could not create " + MAP)
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()

    folder = os.path.join(GEN, "terrain")
    names = [layer[0] for layer in materials.LAYERS]
    landscape = unreal.SpnWorldBuildLibrary.create_landscape_from_raw(world, os.path.join(folder, "height.r16"), layout.COMPONENTS, layout.COMPONENTS, 1, layout.QUADS_PER_COMPONENT,
        unreal.Vector(0, 0, 0), unreal.Vector(layout.GRID_QUAD, layout.GRID_QUAD, layout.Z_SCALE), mats["landscape"], names,
        [os.path.join(folder, "layer_%s.r8" % n) for n in names], "/Game/SpnWorld/Landscape")
    if landscape is None:
        k.fail("landscape creation failed")
    build_sky(mats)

    ctx = {"roads": roads, "lots": lots, "terrain": terrain, "creek": creek, "mats": mats, "has": has, "landscape": landscape}
    nature.water(ctx)
    for letter, module_name in (("C", "roads"), ("D", "buildings"), ("F", "nature"), ("H", "props"), ("H", "woodland")):
        if has(letter) and os.path.exists(os.path.join(HERE, module_name + ".py")):
            module = importlib.import_module(module_name)
            importlib.reload(module)
            module.build(ctx)
            k.log("%s done (%.0f s)" % (module_name, time.time() - started))

    route = None
    if has("H"):
        # gameplay integration phase 1 (2026-11-13): the playable route at the north edge of the town, NPCs, navigation, world boundary
        import gameplay
        importlib.reload(gameplay)
        route = gameplay.build(ctx)
        k.log("gameplay done (%.0f s)" % (time.time() - started))
        import qa_points
        importlib.reload(qa_points)
        path, keys = qa_points.build(ctx, GEN)
        k.log("qa points %s -> %s" % (keys, path))
        # map data for the in-game map and minimap (night build 2026-11-14), from the same layout; deterministic
        import map_data
        importlib.reload(map_data)
        map_path, map_info = map_data.write()
        k.log("map data -> %s (%d roads, %d buildings)" % (map_path, len(map_info["segments"]), len(map_info["buildings"])))

    if route is not None:
        # a new game starts at the beginning of the route, at the north edge of the town
        sx, sy, sz = route["start"]
        start = k.spawn(unreal.PlayerStart, sx, sy, sz, route["start_yaw"], "SPN_PlayerStart")
    else:
        hwy = roads["hwy"]
        sx, sy, sz, yaw = hwy.offset(terrain_gen.main_street(roads)[0] + 3000.0, -620.0)
        start = k.spawn(unreal.PlayerStart, sx, sy, sz + 120.0, yaw, "SPN_PlayerStart")
    if not levels.save_current_level():
        k.fail("could not save " + MAP)
    k.eal.save_directory("/Game/SpnWorld", only_if_is_dirty=True, recursive=True)
    k.log("created %s, phase %s, %.0f s" % (MAP, PHASE, time.time() - started))


main()
