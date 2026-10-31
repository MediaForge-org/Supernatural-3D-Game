"""Import the accepted reference image and build the PLACEHOLDER comparison level for Protagonist V1.

Creates (never overwrites existing assets of other names; re-running only rebuilds actors tagged SPN_PROTAGONIST_V1_COMPARE):
  /Game/Characters/ProtagonistV1/Reference/T_ProtagonistV1_Reference      imported copy of the reference image
  /Game/Characters/ProtagonistV1/Reference/M_ProtagonistV1_ReferenceBoard unlit material showing it
  /Game/Characters/ProtagonistV1/Comparison/LV_ProtagonistV1_Compare      lights, reference board, cine cameras
The MetaHuman itself is not saved in the level (its preview actor is transient); open the character in MetaHuman Creator
or run spn_mh_capture_views.py spawn while this level is open.
  spn_mh_setup_compare.py <reference.png> <fit.json>
"""

import json
import math
import sys

import unreal

ROOT = "/Game/Characters/ProtagonistV1"
REF_DIR = ROOT + "/Reference"
LEVEL = ROOT + "/Comparison/LV_ProtagonistV1_Compare"
TAG = "SPN_PROTAGONIST_V1_COMPARE"
EAL = unreal.EditorAssetLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)


def reference_texture(ref_file):
    path = REF_DIR + "/T_ProtagonistV1_Reference"
    if not EAL.does_asset_exist(path):
        task = unreal.AssetImportTask()
        task.filename = ref_file
        task.destination_path = REF_DIR
        task.destination_name = "T_ProtagonistV1_Reference"
        task.automated = True
        task.save = True
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    tex = unreal.load_asset(path)
    tex.set_editor_property("mip_gen_settings", unreal.TextureMipGenSettings.TMGS_NO_MIPMAPS)
    tex.set_editor_property("lod_group", unreal.TextureGroup.TEXTUREGROUP_UI)
    EAL.save_asset(path)
    return tex


def board_material(tex):
    path = REF_DIR + "/M_ProtagonistV1_ReferenceBoard"
    if not EAL.does_asset_exist(path):
        mel = unreal.MaterialEditingLibrary
        mat = unreal.AssetToolsHelpers.get_asset_tools().create_asset("M_ProtagonistV1_ReferenceBoard", REF_DIR, unreal.Material, unreal.MaterialFactoryNew())
        mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_UNLIT)
        mat.set_editor_property("two_sided", True)
        ts = mel.create_material_expression(mat, unreal.MaterialExpressionTextureSample, -300, 0)
        ts.set_editor_property("texture", tex)
        mel.connect_material_property(ts, "RGB", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
        mel.recompile_material(mat)
        EAL.save_asset(path)
    return unreal.load_asset(path)


def spawn(cls, label, loc, rot=unreal.Rotator(0, 0, 0)):
    a = EAS.spawn_actor_from_class(cls, loc, rot)
    a.set_actor_label(label)
    a.tags = [TAG]
    return a


def cine_camera(label, cam, fov):
    a = spawn(unreal.CineCameraActor, label, unreal.Vector(*cam[:3]), unreal.Rotator(cam[5], cam[3], cam[4]))
    cc = a.get_cine_camera_component()
    cc.set_editor_property("current_focal_length", cc.filmback.sensor_width / (2 * math.tan(math.radians(fov / 2))))
    return a


def main(argv):
    ref_file, fit = argv[0], json.load(open(argv[1], encoding="utf-8"))
    mat = board_material(reference_texture(ref_file))
    if EAL.does_asset_exist(LEVEL):
        LES.load_level(LEVEL)
        for a in EAS.get_all_level_actors():
            if TAG in [str(t) for t in a.tags]:
                EAS.destroy_actor(a)
    else:
        LES.new_level(LEVEL)

    key = spawn(unreal.DirectionalLight, "Light_Key", unreal.Vector(0, 0, 400), unreal.Rotator(0, -25, -100))
    key.light_component.set_intensity(2.2)
    key.light_component.set_light_color(unreal.LinearColor(1.0, 0.85, 0.7, 1.0))
    key.light_component.set_editor_property("light_source_angle", 12.0)
    sky = spawn(unreal.SkyLight, "Light_Sky", unreal.Vector(0, 0, 400))
    sky.light_component.set_editor_property("source_type", unreal.SkyLightSourceType.SLS_SPECIFIED_CUBEMAP)
    sky.light_component.set_editor_property("cubemap", unreal.load_asset("/Engine/MapTemplates/Sky/SunsetAmbientCubemap"))
    sky.light_component.set_intensity(2.2)

    # 1536 x 1024 image on a 150 x 100 cm plane beside the character, facing the front camera (+Y)
    board = spawn(unreal.StaticMeshActor, "ReferenceBoard", unreal.Vector(-130, 0, 150), unreal.Rotator(90, 0, 180))
    board.static_mesh_component.set_static_mesh(unreal.load_asset("/Engine/BasicShapes/Plane"))
    board.set_actor_scale3d(unreal.Vector(1.5, 1.0, 1.0))
    board.static_mesh_component.set_material(0, mat)
    board.static_mesh_component.set_editor_property("cast_shadow", False)

    cine_camera("Cam_ReferenceMatch", fit["cam"], fit["fov"])
    target = (0.0, 6.0, 165.0)
    for label, yaw in (("Cam_Front", 0.0), ("Cam_ThreeQuarter", -40.0), ("Cam_Side", -90.0)):
        y = math.radians(yaw)
        loc = (target[0] + 350 * math.sin(y), target[1] + 350 * math.cos(y), target[2])
        cine_camera(label, [loc[0], loc[1], loc[2], 0.0, math.degrees(math.atan2(target[1] - loc[1], target[0] - loc[0])), 0.0], 6.0)
    note = spawn(unreal.TextRenderActor, "Note_Placeholder", unreal.Vector(-130, 0, 92), unreal.Rotator(0, 0, 90))
    note.text_render.set_editor_property("text", "PLACEHOLDER - Protagonist V1 comparison level (not a game map)")
    note.text_render.set_editor_property("world_size", 4.0)
    LES.save_current_level()
    print(f"[spn_mh_setup_compare] {LEVEL} ready")


if __name__ == "__main__":
    main(sys.argv[1:])
