"""Measurement specs built from the camera runs of make_cine.py (pure Python; run with the engine's Python after make_cine.py).

    python cine_variants.py screen_percentage   -> cine/sp_perf.json (perf) and cine/sp_video.json (video at 1440p)

The same shots, cameras and lighting scenarios are repeated with one console setting changed per variant, so the variants differ in
nothing else. Each scenario block starts with a warm-up shot and ends with the first variant again, to show the drift of the machine.
"""

import copy
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
CINE = os.path.join(HERE, "cine")


def shots_by_name(file_name):
    with open(os.path.join(CINE, file_name)) as f:
        return {s["name"]: s for s in json.load(f)["shots"]}


def variant_spec(base_shots, scenarios, variants, durations, restore):
    """variants: [(label, [console commands])]; durations: {shot name: seconds}."""
    shots = []
    for scenario in scenarios:
        first = base_shots[0]
        warm = copy.deepcopy(first)
        warm.update(name="warm_%s" % scenario, scenario=scenario, duration=5, console=list(restore))
        shots.append(warm)
        for label, commands in list(variants) + [(variants[0][0] + "_again", variants[0][1])]:
            for base in base_shots:
                s = copy.deepcopy(base)
                s.update(name="%s_%s_%s" % (base["name"], scenario, label), scenario=scenario, duration=durations.get(base["name"], base["duration"]),
                         console=list(restore) + list(commands))
                s.pop("stills", None)
                shots.append(s)
    return {"warmup_frames": 240, "cut_warmup_frames": 90, "console": list(restore), "shots": shots}


def screen_percentage():
    tour = shots_by_name("tour.json")
    base = [tour["forest_track"], tour["cabin_pond"], tour["creek"]]
    restore = ["r.ScreenPercentage 100"]
    variants = [("sp100", ["r.ScreenPercentage 100"]), ("sp90", ["r.ScreenPercentage 90"]), ("sp85", ["r.ScreenPercentage 85"])]
    perf = variant_spec(base, ["day", "spn_overcast"], variants, {"forest_track": 20, "cabin_pond": 14, "creek": 14}, restore)
    with open(os.path.join(CINE, "sp_perf.json"), "w") as f:
        json.dump(perf, f, indent=1)
    # video for the visual comparison: only the forest track, once per variant and scenario, no repeat
    video = variant_spec([tour["forest_track"]], ["day", "spn_overcast"], variants, {"forest_track": 10}, restore)
    video["shots"] = [s for s in video["shots"] if not s["name"].startswith("warm_") and not s["name"].endswith("_again")]
    with open(os.path.join(CINE, "sp_video.json"), "w") as f:
        json.dump(video, f, indent=1)
    print("wrote cine/sp_perf.json (%d shots) and cine/sp_video.json (%d shots)" % (len(perf["shots"]), len(video["shots"])))


def quality_modes():
    """The SpnWorld standard (90 %) against the quality mode (100 %), switched only through `Spn.Quality` - the way the game does it.
    No r.ScreenPercentage on the console here: a console value has a higher priority than the level's setting and would hide it."""
    tour = shots_by_name("tour.json")
    base = [tour["forest_track"], tour["cabin_pond"], tour["creek"]]
    restore = ["Spn.Quality standard"]
    variants = [("standard", ["Spn.Quality standard"]), ("quality", ["Spn.Quality quality"])]
    perf = variant_spec(base, ["day", "spn_overcast"], variants, {"forest_track": 20, "cabin_pond": 14, "creek": 14}, restore)
    with open(os.path.join(CINE, "quality_perf.json"), "w") as f:
        json.dump(perf, f, indent=1)
    print("wrote cine/quality_perf.json (%d shots)" % len(perf["shots"]))


if __name__ == "__main__":
    {"screen_percentage": screen_percentage, "quality_modes": quality_modes}[sys.argv[1] if len(sys.argv) > 1 else "screen_percentage"]()
