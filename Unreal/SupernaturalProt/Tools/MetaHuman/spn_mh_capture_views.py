"""Comparison renders of the spawned MetaHuman preview actor: reference-matched view (camera from spn_mh_fit_face) plus
front, three-quarter and side views. Writes PNGs to <project>/Saved/ProtagonistV1/.
  spn_mh_capture_views.py spawn <asset>            # step 1: open for edit + spawn preview actor
  spn_mh_capture_views.py refresh                  # step 2 (after the groom bindings finished, see spn_mh_editor)
  spn_mh_capture_views.py capture <prefix> <fit.json> [views=match,front,q34,side]
Cameras stay >= ~3 m away with a narrow FOV (perspective close to a portrait lens).
"""

import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spn_mh_editor as E  # noqa: E402

VIEW_YAW = {"front": 0.0, "q34": -40.0, "side": -90.0, "q34_left": 40.0}


def main(argv):
    if argv[0] == "spawn":
        ch = E.open_for_edit(argv[1])
        E.ensure_lights()
        E.spawn_preview(ch)
        print("[spn_mh_capture_views] spawned; wait for the groom bindings, then run 'refresh'")
        return
    if argv[0] == "refresh":
        E.refresh_grooms()
        return
    prefix, fit = argv[1], json.load(open(argv[2], encoding="utf-8"))
    views = argv[3].split(",") if len(argv) > 3 else ["match", "front", "q34", "side"]
    out = E.out_dir()
    actors = E.preview_actors()
    if not actors:
        raise RuntimeError("no preview actor; run 'spawn' first")
    head = actors[0].get_components_by_class(E.unreal.SkeletalMeshComponent)
    face = next((c for c in head if c.get_name() == "Face"), None)
    target = face.get_socket_location("head") if face else E.unreal.Vector(0, 0, 160)
    target = (target.x, target.y + 6.0, target.z + 8.0)
    for v in views:
        if v == "match":
            E.ensure_lights((-25.0, -100.0), 2.2, 2.2, warm=True)
            E.capture(out, f"{prefix}_match.png", fit["cam"], fit["fov"], 1536, 1024)
        else:
            E.ensure_lights((-30.0, -110.0), 2.0, 2.0, warm=False)
            E.capture(out, f"{prefix}_{v}.png", E.orbit(target, VIEW_YAW[v], 0.0, 350.0), 6.0, 1024, 1024)
        print(f"[spn_mh_capture_views] {prefix}_{v}.png")


if __name__ == "__main__":
    main(sys.argv[1:])
