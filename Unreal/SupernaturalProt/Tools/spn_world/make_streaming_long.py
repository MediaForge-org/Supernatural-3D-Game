"""Writes cine/streaming_long.json: streaming during LONGER continuous movement (developer request 2026-11-14, item 5).

Shots (lighting "day", like the other perf specs):
  long_sprint   the player character sprints without a cut from the route start up Oak Street, along the whole pine track to the cabin and
                back to the start (about 730 m, town -> forest -> town)
  long_flight   one continuous camera flight at vehicle speed (about 15 m/s) over the main street, Oak Street, the north end and the pine track

Run with any Python 3 (no Unreal needed):  python make_streaming_long.py
Then:  Tools\\spn_visual.ps1 -Spec <cine/streaming_long.json> -Map /Game/SpnWorld/Lvl_SpnWorld -Mode perf -Width 2560 -Height 1440
Points: the route start and the way to the track from Saved/SpnWorldGen/qa_points.json (written by every world build), the pine track's layout
points (layout.py, metres -> cm), the cabin's front; camera keys from showcase.json / route_perf.json. PLACEHOLDER measurement run.
"""
import json
import math
import os

HERE = os.path.dirname(os.path.abspath(__file__))
CINE = os.path.join(HERE, "cine")
QA = os.path.normpath(os.path.join(HERE, "..", "..", "Saved", "SpnWorldGen", "qa_points.json"))
PINE_M = [(-130, 330), (-190, 380), (-250, 410)]   # layout.py "pine" (metres), between the track entry and the cabin


def load(name):
    with open(os.path.join(CINE, name), encoding="utf-8") as f:
        return json.load(f)


def shot(spec, name):
    for s in spec["shots"]:
        if s["name"] == name:
            return s
    raise KeyError(name)


def length(points):
    return sum(math.dist(a[:2], b[:2]) for a, b in zip(points, points[1:]))


def main():
    with open(QA, encoding="utf-8") as f:
        qa = json.load(f)
    g = qa["gameplay"]
    out = [g["start"]] + g["to_track"][:1] + [[x * 100.0, y * 100.0, 0.0] for x, y in PINE_M] + [qa["cabin"]["outside"]]
    path = out + list(reversed(out[:-1]))
    sprint_seconds = length(path) / 800.0 + 12.0   # sprint speed 8 m/s plus the starts and the turn
    showcase, route = load("showcase.json"), load("route_perf.json")
    keys = []
    for spec, name in ((showcase, "main_street"), (showcase, "oak_street"), (route, "oak_north_route"), (route, "forest_track")):
        keys += shot(spec, name)["keys"]
    flight_len = length([k["p"] for k in keys])
    shots = [
        {"name": "long_sprint", "duration": round(sprint_seconds, 1), "fov": 70, "scenario": "day", "sprint": True, "walk_pitch": -6,
         "keys": [{"p": path[0], "l": path[1]}], "walk": path},
        {"name": "long_flight", "duration": round(flight_len / 1500.0, 1), "fov": 70, "scenario": "day", "keys": keys},
    ]
    spec = {"warmup_frames": 240, "cut_warmup_frames": 60, "console": [], "shots": shots}
    with open(os.path.join(CINE, "streaming_long.json"), "w", encoding="utf-8") as f:
        json.dump(spec, f, indent=1)
    print("long_sprint %.0f m in %.0f s, long_flight %.0f m in %.0f s" % (length(path) / 100.0, sprint_seconds, flight_len / 100.0, flight_len / 1500.0))


if __name__ == "__main__":
    main()
