"""Modular body architecture (2026-11-15, Docs/Proposals/0018), task 1: READ-ONLY inventory of the human bodies that exist locally.

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file> -unattended -nullrhi
For each body / face mesh: LODs, vertices per LOD, material slots, sections, bone count and the bones per body zone (by name), skin-weight
coverage of the pelvis / chest zone (which bones carry those vertices), and for every material the parameter names that look like
underwear / clothing / masks (to see whether "underwear" is geometry or only painted into the skin material). Also lists the MetaHuman
wardrobe / outfit assets of the local plugin content. Writes Saved/SpnCharacter/body_inventory.json. Changes nothing.
"""

import json
import os

import unreal

MESHES = [
    "/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple",
    "/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple",
    "/MetaHumanCharacter/Body/IdentityTemplate/SKM_Body",
    "/MetaHumanCharacter/Face/SKM_Face",
    "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/Body/SKM_MHC_SpnTestMaleTall_BodyMesh",
    "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/Face/SKM_MHC_SpnTestMaleTall_FaceMesh",
]
ZONE_HINTS = {
    "pelvis/hips": ("pelvis", "hip"), "spine/chest": ("spine", "breast", "pec", "chest"), "neck/head": ("neck", "head"),
    "arms": ("clavicle", "upperarm", "lowerarm", "elbow", "shoulder"), "hands": ("hand", "thumb", "index", "middle", "ring", "pinky", "wrist"),
    "legs": ("thigh", "calf", "knee"), "feet": ("foot", "ball", "toe", "ankle"), "face (facial rig)": ("FACIAL", "eye", "jaw", "teeth", "tongue"),
}
MATERIAL_HINTS = ("underwear", "garment", "cloth", "mask", "region", "groin", "brief", "bra", "nude", "hair", "tattoo", "dirt", "wet", "scar", "freckle", "makeup")


def bone_names(mesh):
    skel = mesh.get_editor_property("skeleton")
    try:   # the reference pose of the skeleton lists every bone (AnimPose scripting API)
        pose = unreal.AnimPoseExtensions.get_reference_pose(skel)
        return [str(n) for n in unreal.AnimPoseExtensions.get_bone_names(pose)]
    except Exception:  # noqa: BLE001
        pass
    names = []
    try:
        ref = unreal.SkeletalMeshEditorSubsystem  # noqa: F841
        names = [str(n) for n in unreal.EditorSkeletalMeshLibrary.get_bone_names(mesh)] if hasattr(unreal, "EditorSkeletalMeshLibrary") else []
    except Exception:
        names = []
    if not names:
        try:
            i = 0
            while True:
                n = skel.get_bone_name(i) if hasattr(skel, "get_bone_name") else None
                if n is None or str(n) == "None":
                    break
                names.append(str(n))
                i += 1
        except Exception:
            pass
    return names


def material_params(mi):
    out = {}
    try:
        if isinstance(mi, unreal.MaterialInstance):
            for kind, getter in (("scalar", "get_scalar_parameter_names"), ("vector", "get_vector_parameter_names"), ("texture", "get_texture_parameter_names")):
                try:
                    names = [str(n) for n in getattr(unreal.MaterialEditingLibrary, getter)(mi)]
                except Exception:
                    names = []
                out[kind] = names
    except Exception as e:  # noqa: BLE001
        out["error"] = str(e)[:120]
    return out


def describe(path):
    mesh = unreal.load_asset(path)
    if mesh is None:
        return {"path": path, "missing": True}
    sub = unreal.get_editor_subsystem(unreal.SkeletalMeshEditorSubsystem)
    lods = sub.get_lod_count(mesh)
    row = {"path": path, "lods": lods, "verts_per_lod": [sub.get_num_verts(mesh, i) for i in range(lods)]}
    slots = []
    for m in mesh.get_editor_property("materials"):
        mi = m.get_editor_property("material_interface")
        params = material_params(mi) if mi else {}
        hits = sorted({p for v in params.values() if isinstance(v, list) for p in v if any(h in p.lower() for h in MATERIAL_HINTS)})
        clothing = sorted({p for v in params.values() if isinstance(v, list) for p in v
                           if any(h in p.lower() for h in ("underwear", "garment", "cloth", "hidemask", "brief", "bra", "region"))})
        slots.append({"slot": str(m.get_editor_property("material_slot_name")), "material": mi.get_path_name() if mi else None,
                      "param_count": sum(len(v) for v in params.values() if isinstance(v, list)), "clothing_and_hide_params": clothing,
                      "relevant_params": hits[:80]})
    row["material_slots"] = slots
    names = bone_names(mesh)
    row["bones"] = len(names)
    zones = {}
    for zone, hints in ZONE_HINTS.items():
        zones[zone] = len([n for n in names if any(h.lower() in n.lower() for h in hints)])
    row["bones_by_zone"] = zones
    row["pelvis_bones"] = [n for n in names if "pelvis" in n.lower() or "hip" in n.lower()][:30]
    row["unexpected_or_special_bones"] = [n for n in names if any(k in n.lower() for k in ("breast", "butt", "glute", "belly", "genit", "jiggle", "spring"))][:40]
    b = mesh.get_bounds()
    row["height_cm"] = round(b.box_extent.z * 2, 1)
    return row


def main():
    report = {"status": "READ-ONLY inventory (2026-11-15)", "meshes": [describe(p) for p in MESHES]}
    reg = unreal.AssetRegistryHelpers.get_asset_registry()
    wardrobe = []
    for root in ("/MetaHumanCharacter", "/MetaHuman"):
        try:
            for a in reg.get_assets_by_path(root, recursive=True):
                cls = str(a.asset_class_path.asset_name)
                name = str(a.asset_name)
                if any(k in cls for k in ("Wardrobe", "Outfit", "Groom", "Cloth", "Collection", "Palette")) or any(k in name.lower() for k in ("garment", "underwear", "outfit")):
                    wardrobe.append({"asset": str(a.package_name), "class": cls})
        except Exception as e:  # noqa: BLE001
            wardrobe.append({"error": str(e)[:120]})
    report["metahuman_wardrobe_assets"] = wardrobe[:120]
    report["metahuman_wardrobe_count"] = len(wardrobe)
    out = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter", "body_inventory.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN body inventory: %d meshes, %d wardrobe assets" % (len(report["meshes"]), len(wardrobe)))


main()
