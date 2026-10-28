"""Editor-side check of /Game/SpnTest/Lvl_SpnMovementTestV2 (M2-A.2 Windows integration). Run:
    UnrealEditor-Cmd.exe <uproject> -run=pythonscript "-script=<abs>/verify_spn_map_v2.py <out.json>" -unattended -nosplash
Loads the map, counts actors by class / instances, and samples collision with line traces (a downward grid and four horizontal
rays per cell). The JSON can be diffed between an instanced and a non-instanced build of the map. Changes nothing, saves nothing."""

import json
import sys
from collections import Counter

import unreal

MAP = "/Game/SpnTest/Lvl_SpnMovementTestV2"
HALF = 15000.0
STEP = 200.0


def trace(world, start, end):
    result = unreal.SystemLibrary.line_trace_single(
        world, unreal.Vector(*start), unreal.Vector(*end), unreal.TraceTypeQuery.TRACE_TYPE_QUERY1, False, [],
        unreal.DrawDebugTrace.NONE, True)
    if result is None:  # the binding returns nothing when the ray hits nothing
        return None
    if isinstance(result, tuple):
        result = result[1] if len(result) > 1 and not isinstance(result[0], unreal.HitResult) else result[0]
    point = result.impact_point
    return [round(point.x, 1), round(point.y, 1), round(point.z, 1)]


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else None
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if not levels.load_level(MAP):
        raise RuntimeError("could not open " + MAP)
    world = unreal.EditorLevelLibrary.get_editor_world()
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
    classes = Counter(a.get_class().get_name() for a in actors)
    instances = 0
    for a in actors:
        if a.get_class().get_name() == "SpnInstancedGroup":
            instances += a.get_editor_property("instances").get_instance_count()
    result = {"actors": len(actors), "classes": dict(classes), "instances": instances, "ground": [], "walls": []}
    x = -HALF
    while x <= HALF:
        y = -HALF
        while y <= HALF:
            result["ground"].append([x, y, trace(world, (x, y, 3000.0), (x, y, -200.0))])
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                result["walls"].append([x, y, dx, dy, trace(world, (x, y, 100.0), (x + dx * STEP, y + dy * STEP, 100.0))])
            y += STEP
        x += STEP
    unreal.log("SpnMapVerify: actors=%d instances=%d classes=%s" % (len(actors), instances, dict(classes)))
    if out:
        with open(out, "w") as handle:
            json.dump(result, handle)


main()
