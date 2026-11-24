"""0013 (Decision 3): read-only report of the skeleton / animation / retargeting dependencies of the player character presentation.

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Writes Saved/SpnCharacter/dependency_report.json. Changes no asset.
"""

import json
import os

import unreal

REG = unreal.AssetRegistryHelpers.get_asset_registry()
OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter", "dependency_report.json")

ROOTS = [
    "/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter",
    "/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed",
    "/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple",
    "/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple",
    "/Game/Characters/Mannequins/Meshes/SK_Mannequin",
]


def deps(package, referencers=False):
    opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True)
    found = REG.get_referencers(package, opts) if referencers else REG.get_dependencies(package, opts)
    return sorted(str(p) for p in (found or []) if str(p).startswith("/Game"))


def assets_of_class(class_name):
    out = []
    for data in REG.get_assets_by_class(unreal.TopLevelAssetPath("/Script/" + class_name[0], class_name[1]), True):
        out.append(str(data.package_name))
    return sorted(out)


def main():
    report = {"roots": {}, "skeletons": {}, "anim_sequences_by_skeleton": {}, "retargeting": {}, "metahuman": {}}
    for root in ROOTS:
        report["roots"][root] = {"dependencies": deps(root), "referencers": deps(root, True)}

    # which skeleton each skeletal mesh and anim asset uses
    for cls in (("Engine", "SkeletalMesh"), ("Engine", "AnimSequence"), ("Engine", "BlendSpace"), ("Engine", "AnimMontage"), ("Engine", "AnimBlueprint")):
        for data in REG.get_assets_by_class(unreal.TopLevelAssetPath("/Script/" + cls[0], cls[1]), True):
            path = str(data.package_name)
            if not path.startswith("/Game"):
                continue
            skeleton = data.get_tag_value("Skeleton") or data.get_tag_value("TargetSkeleton") or ""
            key = str(skeleton).split("'")[1] if "'" in str(skeleton) else str(skeleton)
            report["anim_sequences_by_skeleton"].setdefault(key or "(no skeleton tag)", {}).setdefault(cls[1], []).append(path)
    for key, kinds in report["anim_sequences_by_skeleton"].items():
        report["skeletons"][key] = {kind: len(paths) for kind, paths in kinds.items()}

    report["retargeting"] = {
        "ik_rigs": assets_of_class(("IKRig", "IKRigDefinition")),
        "ik_retargeters": assets_of_class(("IKRig", "IKRetargeter")),
        "control_rigs": assets_of_class(("ControlRigDeveloper", "ControlRigBlueprint")),
        "physics_assets": assets_of_class(("Engine", "PhysicsAsset")),
    }
    report["metahuman"] = {
        "character_assets": [str(d.package_name) for d in REG.get_assets_by_path("/Game", True) if "MHC_" in str(d.asset_name)],
    }
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w") as f:
        json.dump(report, f, indent=1)
    unreal.log("SPN deps: wrote %s" % OUT)
    unreal.log("SPN deps: skeletons %s" % json.dumps(report["skeletons"]))
    unreal.log("SPN deps: retargeting %s" % json.dumps({k: len(v) for k, v in report["retargeting"].items()}))


main()
