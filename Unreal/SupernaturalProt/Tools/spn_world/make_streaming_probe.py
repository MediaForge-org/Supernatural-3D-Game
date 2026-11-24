"""Writes cine/streaming_probe.json: identical camera runs for the texture/Nanite streaming diagnosis (Night build 2026-11-14).

Shots (all lighting "day", like the other perf specs):
  town_center      main street flight (showcase keys)
  oak_street       Oak Street flight (showcase keys)
  turns_town       standing on the main street, four full turns in 6 s (fast camera rotation)
  sprint_forest    the player character sprints from the north end of Oak Street onto the pine track (walk shot)
  cabin_pond       cabin and pond flight (showcase keys)
  forest_to_town   the pine track flown backwards from the deep forest to the north end of Oak Street (forest -> town)
  turns_forest     standing on the pine track, four full turns in 6 s

Run with any Python 3 (no Unreal needed):  python make_streaming_probe.py
Then:  Tools\\spn_visual.ps1 -Spec <cine/streaming_probe.json> -Map /Game/SpnWorld/Lvl_SpnWorld -Mode perf -Width 2560 -Height 1440
A cold variant (no wait for compiling at start) is written as cine/streaming_probe_cold.json.
"""
import json
import math
import os

HERE = os.path.dirname(os.path.abspath(__file__))
CINE = os.path.join(HERE, "cine")


def load(name):
    with open(os.path.join(CINE, name), encoding="utf-8") as f:
        return json.load(f)


def shot(spec, name):
    for s in spec["shots"]:
        if s["name"] == name:
            return s
    raise KeyError(name)


def turns(name, centre, eye_z, seconds=6.0, laps=4, steps=48):
    keys = []
    for i in range(steps + 1):
        a = 2.0 * math.pi * laps * i / steps
        keys.append({"p": [centre[0], centre[1], eye_z],
                     "l": [centre[0] + 1000.0 * math.cos(a), centre[1] + 1000.0 * math.sin(a), eye_z - 40.0]})
    return {"name": name, "duration": seconds, "fov": 70, "linear": True, "scenario": "day", "keys": keys}


def main():
    showcase = load("showcase.json")
    route = load("route_perf.json")
    shots = []
    for name, dur in (("main_street", 12), ("oak_street", 10)):
        s = dict(shot(showcase, name))
        s.pop("walk", None)
        s.update({"name": "town_center" if name == "main_street" else name, "duration": dur, "scenario": "day"})
        shots.append(s)
    ms = shot(showcase, "main_street")["keys"][3]["p"]
    shots.append(turns("turns_town", ms, ms[2] + 60.0))
    # pine track walk: from the north end of Oak Street along the route's way points (qa_points "to_track") and the forest track keys
    ft = shot(route, "forest_track")["keys"]
    walk = [[-9451.8, 26703.9, -249.5], [-10185.8, 28333.8, -150.0]] + [k["p"] for k in ft[1:5]]
    shots.append({"name": "sprint_forest", "duration": 14, "fov": 70, "scenario": "day", "sprint": True, "walk_pitch": -6,
                  "keys": [{"p": walk[0], "l": walk[1]}], "walk": walk})
    s = dict(shot(showcase, "cabin_pond"))
    s.pop("walk", None)
    s.update({"duration": 10, "scenario": "day"})
    shots.append(s)
    back = [{"p": k["p"], "l": [2 * k["p"][i] - k["l"][i] for i in range(3)]} for k in reversed(ft)]
    oak = [{"p": k["p"], "l": [2 * k["p"][i] - k["l"][i] for i in range(3)]} for k in reversed(shot(route, "oak_north_route")["keys"])]
    shots.append({"name": "forest_to_town", "duration": 20, "fov": 65, "scenario": "day", "keys": back + oak})
    mid = ft[3]["p"]
    shots.append(turns("turns_forest", mid, mid[2] + 40.0))
    spec = {"warmup_frames": 240, "cut_warmup_frames": 60, "console": [], "shots": shots}
    with open(os.path.join(CINE, "streaming_probe.json"), "w", encoding="utf-8") as f:
        json.dump(spec, f, indent=1)
    cold = dict(spec, cold=True, warmup_frames=2, cut_warmup_frames=2)
    with open(os.path.join(CINE, "streaming_probe_cold.json"), "w", encoding="utf-8") as f:
        json.dump(cold, f, indent=1)
    print("shots:", [s["name"] for s in shots])


if __name__ == "__main__":
    main()
