"""READ-ONLY: counts the placed gameplay actors (project classes and a few engine kinds) per level, to compare the world with the test maps.

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Writes Saved/SpnWorldGen/level_content.json. Loads each level in the editor world; saves nothing.
"""

import json
import os
from collections import Counter

import unreal

LEVELS = ["/Game/SpnWorld/Lvl_SpnWorld", "/Game/SpnTest/Lvl_SpnMovementTestV2", "/Game/SpnTest/Lvl_SpnMovementTestV3Interiors",
          "/Game/SpnTest/Lvl_SpnNpcTest"]
OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnWorldGen", "level_content.json")
ENGINE_KINDS = {"PlayerStart", "NavMeshBoundsVolume", "RecastNavMesh", "PostProcessVolume", "CameraActor", "PointLight", "SpotLight"}


def main():
    les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    report = {}
    for level in LEVELS:
        if not unreal.EditorAssetLibrary.does_asset_exist(level):
            report[level] = {"error": "missing"}
            continue
        les.load_level(level)
        counts = Counter()
        for actor in eas.get_all_level_actors():
            cls = actor.get_class()
            name = cls.get_name()
            path = cls.get_path_name()
            if path.startswith("/Script/SupernaturalProt") or "/Game/" in path or name in ENGINE_KINDS:
                counts[name] += 1
        report[level] = dict(sorted(counts.items()))
        unreal.log("SPN content %s: %s" % (level, dict(sorted(counts.items()))))
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w") as f:
        json.dump(report, f, indent=1)


main()
