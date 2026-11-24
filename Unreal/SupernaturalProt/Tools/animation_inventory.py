"""Animation inventory for the ledge-assist animation plan (developer decision 2026-11-14): which EXISTING clips could be the starting point for
StepOn, Vault, Mantle and ClimbUp. Read-only: loads assets, measures them, writes Saved/SpnCharacter/animation_inventory.json. Creates,
changes and saves nothing.

Run headless:
  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file> [-EnablePlugins=MoverExamples]
(-EnablePlugins only mounts the engine's example content for this run; the project file is not changed.)

Per AnimSequence: skeleton, length, frames, root motion flag, root displacement (start -> end), pelvis height range and its rise, the highest
point of each foot and hand above the start pelvis height (a hand or foot raised high = a candidate for a hand plant / step onto something).
"""

import json
import os

import unreal

ROOTS = ["/Game/Characters", "/Game/Variant_Combat", "/Game/Variant_Platforming", "/Game/Variant_SideScrolling", "/MoverExamples"]
SAVED = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter")
OUT = os.path.join(SAVED, "animation_inventory.json")
BONES = ["pelvis", "foot_l", "foot_r", "hand_l", "hand_r"]
SAMPLES = 24


def pose_at(anim, t):
    options = unreal.AnimPoseEvaluationOptions()
    return unreal.AnimPoseExtensions.get_anim_pose_at_time(anim, t, options)


def bone_z(pose, bone):
    """Height of a bone in the component (world) space of the evaluated pose."""
    return unreal.AnimPoseExtensions.get_bone_pose(pose, bone, unreal.AnimPoseSpaces.WORLD).translation

def measure(anim):
    length = anim.get_play_length()
    info = {"path": anim.get_path_name().split(".")[0], "skeleton": anim.get_editor_property("skeleton").get_path_name().split(".")[0],
            "length_s": round(length, 3), "frames": unreal.AnimationLibrary.get_num_frames(anim),
            "root_motion": bool(anim.get_editor_property("enable_root_motion"))}
    try:
        r0 = unreal.AnimationLibrary.get_bone_pose_for_time(anim, "root", 0.0, False).translation
        r1 = unreal.AnimationLibrary.get_bone_pose_for_time(anim, "root", length, False).translation
        info["root_delta_cm"] = [round(r1.x - r0.x, 1), round(r1.y - r0.y, 1), round(r1.z - r0.z, 1)]
    except Exception as e:  # noqa: BLE001
        info["root_delta_cm"] = None
        info["root_error"] = str(e)
    try:
        heights = {b: [] for b in BONES}
        for i in range(SAMPLES + 1):
            pose = pose_at(anim, length * i / SAMPLES)
            for b in BONES:
                heights[b].append(bone_z(pose, b).z)
        p0 = heights["pelvis"][0]
        info["pelvis_z_cm"] = [round(min(heights["pelvis"]), 1), round(max(heights["pelvis"]), 1)]
        info["pelvis_rise_cm"] = round(max(heights["pelvis"]) - p0, 1)
        info["highest_above_start_pelvis_cm"] = {b: round(max(heights[b]) - p0, 1) for b in BONES if b != "pelvis"}
    except Exception as e:  # noqa: BLE001
        info["bone_error"] = str(e)
    return info


def main():
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    registry.scan_paths_synchronous(ROOTS, True)
    report = {"status": "READ-ONLY inventory (2026-11-14): nothing created or changed", "clips": [], "montages": [], "blendspaces": [], "rigs": []}
    for root in ROOTS:
        assets = registry.get_assets_by_path(root, recursive=True)
        for data in assets:
            cls = str(data.asset_class_path.asset_name)
            path = str(data.package_name)
            if cls == "AnimSequence":
                anim = unreal.load_asset(path)
                if anim is not None:
                    report["clips"].append(measure(anim))
            elif cls == "AnimMontage":
                report["montages"].append(path)
            elif cls in ("BlendSpace", "BlendSpace1D", "AimOffsetBlendSpace", "AimOffsetBlendSpace1D"):
                report["blendspaces"].append(path)
            elif cls in ("IKRigDefinition", "IKRetargeter", "ControlRigBlueprint", "PoseAsset", "Skeleton"):
                report["rigs"].append({"class": cls, "path": path})
    os.makedirs(SAVED, exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1)
    unreal.log("animation inventory: %d clips, %d montages -> %s" % (len(report["clips"]), len(report["montages"]), OUT))


main()
