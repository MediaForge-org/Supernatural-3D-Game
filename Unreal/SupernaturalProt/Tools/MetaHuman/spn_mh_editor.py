"""Editor-side helpers for the Protagonist V1 MetaHuman workflow (needs the Unreal Editor with PythonScriptPlugin + MetaHumanCharacter).

Comparison lights/capture actors carry the labels CMP_* and live only in the level that is open when the scripts run.
Findings from UE 5.8.4 (2026-10-31, see Docs/ProtagonistV1/README.md):
  * the preview actor's hair strands may be missing in SceneCapture renders until the groom bindings finished building
    (minutes) and the groom components were re-initialised (refresh_grooms); check every render visually,
  * instance parameters of grooms live on the PREVIEW collection; on_edit_preview_collection() writes them back to the asset.
"""

import math
import os

import unreal

MHS = unreal.get_editor_subsystem(unreal.MetaHumanCharacterEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
PREVIEW_ACTOR_CLASS = "MetaHumanDefaultEditorPipelineActor"


def out_dir(sub="ProtagonistV1"):
    path = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_saved_dir()), sub)
    os.makedirs(path, exist_ok=True)
    return path


def open_for_edit(asset_path):
    ch = unreal.load_asset(asset_path)
    if ch is None:
        raise RuntimeError(f"asset not found: {asset_path}")
    if not MHS.is_object_added_for_editing(ch) and not MHS.try_add_object_to_edit(ch):
        raise RuntimeError(f"cannot open {asset_path} for editing (open in another editor?)")
    return ch


def find(cls, label):
    for a in EAS.get_all_level_actors():
        if isinstance(a, cls) and a.get_actor_label() == label:
            return a
    return None


def preview_actors():
    return [a for a in EAS.get_all_level_actors() if a.get_class().get_name() == PREVIEW_ACTOR_CLASS]


def spawn_preview(ch):
    """Transient preview actor owned by the MetaHuman subsystem (never saved with the level)."""
    for a in preview_actors():
        EAS.destroy_actor(a)
    MHS.assemble_for_preview(ch)
    return MHS.spawn_meta_human_actor(ch)


def refresh_grooms():
    """Re-initialise the preview grooms (toggling cards re-creates their render state). Call in a separate remote call
    after the bindings were built, then capture in yet another call."""
    for a in preview_actors():
        for c in a.get_components_by_class(unreal.GroomComponent):
            c.set_editor_property("use_cards", not c.get_editor_property("use_cards"))


def ensure_lights(key_rot=(-25.0, -100.0), key_intensity=2.2, sky_intensity=2.2, warm=True):
    sun = find(unreal.DirectionalLight, "CMP_Key")
    if not sun:
        sun = EAS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 300))
        sun.set_actor_label("CMP_Key")
    sun.set_actor_rotation(unreal.Rotator(0, key_rot[0], key_rot[1]), False)
    lc = sun.light_component
    lc.set_intensity(key_intensity)
    lc.set_light_color(unreal.LinearColor(1.0, 0.85, 0.7, 1.0) if warm else unreal.LinearColor(1, 1, 1, 1))
    lc.set_editor_property("light_source_angle", 12.0)
    sky = find(unreal.SkyLight, "CMP_Sky")
    if not sky:
        sky = EAS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 300))
        sky.set_actor_label("CMP_Sky")
    cube = unreal.load_asset("/Engine/MapTemplates/Sky/SunsetAmbientCubemap")
    if cube:
        sky.light_component.set_editor_property("source_type", unreal.SkyLightSourceType.SLS_SPECIFIED_CUBEMAP)
        sky.light_component.set_editor_property("cubemap", cube)
    sky.light_component.set_intensity(sky_intensity)
    sky.light_component.recapture_sky()


def _capture_actor(w, h):
    cap = find(unreal.SceneCapture2D, "CMP_Capture")
    if not cap:
        cap = EAS.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 300, 160))
        cap.set_actor_label("CMP_Capture")
    cc = cap.capture_component2d
    rt = cc.get_editor_property("texture_target")
    if not rt or rt.size_x != w or rt.size_y != h:
        rt = unreal.RenderingLibrary.create_render_target2d(cap, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA8_SRGB)
        cc.set_editor_property("texture_target", rt)
    cc.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
    cc.set_editor_property("capture_every_frame", False)
    cc.set_editor_property("capture_on_movement", False)
    return cap, cc, rt


def capture(directory, file_name, cam, fov, w, h, warm=4):
    """cam = [x, y, z, pitch, yaw, roll]. Writes <directory>/<file_name> (PNG)."""
    cap, cc, rt = _capture_actor(w, h)
    cap.set_actor_location_and_rotation(unreal.Vector(*cam[:3]), unreal.Rotator(cam[5], cam[3], cam[4]), False, False)
    cc.set_editor_property("fov_angle", fov)
    for _ in range(warm):
        cc.capture_scene()
    unreal.RenderingLibrary.export_render_target(cap, rt, directory, file_name)
    return os.path.join(directory, file_name)


def track(image_path):
    """MetaHuman face contour tracker on an image file -> {curve: [[x, y], ...]} ({} if no face found)."""
    size, pixels = unreal.PromotedFrameUtils.get_promoted_frame_as_pixel_array_from_disk(image_path)
    res = MHS.track_face_landmarks_from_image(pixels, size.x, size.y)
    if isinstance(res, tuple) and len(res) == 1:
        res = res[0]
    if not res:
        return {}
    return {str(k): [[p.x, p.y] for p in v.tracking_points] for k, v in res.items()}


def landmarks(ch):
    return [(v.x, v.y, v.z) for v in MHS.get_face_landmarks(ch)]


def orbit(target, yaw_deg, pitch_deg, dist):
    y, p = math.radians(yaw_deg), math.radians(pitch_deg)
    loc = (target[0] + dist * math.cos(p) * math.sin(y), target[1] + dist * math.cos(p) * math.cos(y), target[2] + dist * math.sin(p))
    d = (target[0] - loc[0], target[1] - loc[1], target[2] - loc[2])
    return [loc[0], loc[1], loc[2], math.degrees(math.atan2(d[2], math.hypot(d[0], d[1]))), math.degrees(math.atan2(d[1], d[0])), 0.0]
