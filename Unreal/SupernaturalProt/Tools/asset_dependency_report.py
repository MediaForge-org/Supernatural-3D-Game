"""READ-ONLY: recursive package dependencies of the maps and config assets that builds and tests need (NightBuild06, asset manifest).

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Writes Saved/SpnAssets/asset_dependencies.json:
  {root: {"exists": bool, "packages": {"/Game/..": {"file": "Content/..", "bytes": n}}, "engine_or_plugin": [..], "missing": [..]}}
Only /Game packages are listed with a file (they are the project's own content); engine and plugin packages are listed by name.
Loads no level and saves nothing: it only reads the asset registry.
"""

import json
import os

import unreal

# Maps the headless tests, the QA suites and the game flow open (grep of Source/, Tools/run_gameplay_qa.ps1, Config/), plus the assets the
# config presets point to. Keep in sync with Docs/NightBuild06/ASSET_MANIFEST.md.
ROOTS = [
    "/Game/SpnTest/Lvl_SpnMovementTest",
    "/Game/SpnTest/Lvl_SpnMovementTestV2",
    "/Game/SpnTest/Lvl_SpnMovementTestV2Env",
    "/Game/SpnTest/Lvl_SpnMovementTestV3Interiors",
    "/Game/SpnTest/Lvl_SpnDoorTest",
    "/Game/SpnTest/Lvl_SpnBuildingTest",
    "/Game/SpnTest/Lvl_SpnNpcTest",
    "/Game/SpnTest/Lvl_SpnMainMenu",
    "/Game/SpnTest/Lvl_SpnNavDemo",
    "/Game/SpnTest/Lvl_PoseTest",
    "/Game/ThirdPerson/Lvl_ThirdPerson",
    "/Game/SpnWorld/Lvl_SpnWorld",
    "/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter",
    "/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed",
    "/Game/SpnCharacterPrep/MetaHumanTest/RTG_SpnMannequinToMHTemplate",
    "/Game/SpnCharacterPrep/MetaHumanTest/RTG_SpnMannequinToMHMaleTall",
    "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/Body/SKM_MHC_SpnTestMaleTall_BodyMesh",
    "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/Face/SKM_MHC_SpnTestMaleTall_FaceMesh",
    "/Game/Characters/ProtagonistV1/MHC_ProtagonistV1",
]
OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnAssets", "asset_dependencies.json")
CONTENT = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_content_dir())


def package_file(package):
    """Content/<rel>.uasset|.umap for a /Game package, or None."""
    rel = package[len("/Game/"):]
    for ext in (".umap", ".uasset"):
        path = os.path.join(CONTENT, rel + ext)
        if os.path.isfile(path):
            return "Content/" + rel + ext, os.path.getsize(path)
    return None, 0


def main():
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    registry.search_all_assets(True)
    options = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True,
                                                    include_searchable_names=False, include_soft_management_references=False,
                                                    include_hard_management_references=False)
    report = {}
    for root in ROOTS:
        entry = {"exists": False, "packages": {}, "engine_or_plugin": [], "missing": []}
        report[root] = entry
        file, _ = package_file(root)
        if file is None:
            continue
        entry["exists"] = True
        seen = set()
        todo = [root]
        # one-file-per-actor maps keep their actors in separate packages that the map package itself does not reference
        rel = root[len("/Game/"):]
        for folder in ("/Game/__ExternalActors__/" + rel, "/Game/__ExternalObjects__/" + rel):
            for data in registry.get_assets_by_path(unreal.Name(folder), recursive=True, include_only_on_disk_assets=True) or []:
                todo.append(str(data.package_name))
        while todo:
            package = todo.pop()
            if package in seen:
                continue
            seen.add(package)
            if not package.startswith("/Game/"):
                entry["engine_or_plugin"].append(package)
                continue   # engine / plugin content ships with the engine: not followed further
            file, size = package_file(package)
            if file is None:
                entry["missing"].append(package)
                continue
            entry["packages"][package] = {"file": file, "bytes": size}
            deps = registry.get_dependencies(unreal.Name(package), options) or []
            for dep in deps:
                name = str(dep)
                if name.startswith("/Script/") or name.startswith("/Memory/") or name.startswith("/Temp/"):
                    continue
                todo.append(name)
        entry["engine_or_plugin"] = sorted(set(entry["engine_or_plugin"]))
        entry["missing"] = sorted(set(entry["missing"]))
        unreal.log("SPN deps %s: %d project packages, %d engine/plugin, %d missing" % (root, len(entry["packages"]), len(entry["engine_or_plugin"]), len(entry["missing"])))
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w") as f:
        json.dump(report, f, indent=1, sort_keys=True)
    unreal.log("SPN deps written to %s" % OUT)


main()
