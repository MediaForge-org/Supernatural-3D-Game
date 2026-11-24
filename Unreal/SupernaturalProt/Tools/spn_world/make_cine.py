"""Writes the camera run specifications of the placeholder world (pure Python): cine/tour.json and cine/showcase.json.

    python make_cine.py        (needs the terrain output in Saved/SpnWorldGen/terrain; run after the world build)

The shots follow the roads and circle the landmarks of layout.py, with heights taken from the generated terrain, so the same file works
for every build phase and the videos of two phases can be compared shot by shot.
"""

import json
import math
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import layout  # noqa: E402
import terrain_gen  # noqa: E402

GEN = os.path.abspath(os.path.join(HERE, "..", "..", "Saved", "SpnWorldGen", "terrain"))


def main():
    roads = layout.build_roads()
    lots = {lot.name: lot for lot in layout.build_lots(roads)}
    terrain = layout.Terrain(GEN)
    for rd in roads.values():
        rd.z = terrain.heights[("road", rd.name)]
    for lot in lots.values():
        lot.z = terrain.heights[("pad", lot.name)]
    main0, main1 = terrain_gen.main_street(roads)

    def follow(road, s0, s1, height=180.0, side=0.0, ahead=2500.0, keys=7, look_drop=40.0):
        rd = roads[road]
        out = []
        for i in range(keys):
            s = s0 + (s1 - s0) * i / (keys - 1)
            x, y, z, _ = rd.offset(s, side)
            direction = 1.0 if s1 >= s0 else -1.0
            lx, ly, lz, _ = rd.offset(min(max(s + direction * ahead, 0.0), rd.length), side * 0.3)
            out.append({"p": [x, y, max(z, terrain.z(x, y)) + height], "l": [lx, ly, lz + height - look_drop]})
        return out

    def arc(cx, cy, radius, a0, a1, height, look_height=200.0, keys=6, radius1=None, height1=None):
        out = []
        base = terrain.z(cx, cy)
        for i in range(keys):
            t = i / (keys - 1)
            a = math.radians(a0 + (a1 - a0) * t)
            r = radius + ((radius1 or radius) - radius) * t
            h = height + ((height1 or height) - height) * t
            x, y = cx + math.cos(a) * r, cy + math.sin(a) * r
            out.append({"p": [x, y, max(base, terrain.z(x, y)) + h], "l": [cx, cy, base + look_height]})
        return out

    def lot_arc(name, radius, a0, a1, height, look_height=250.0, **kw):
        lot = lots[name]
        return arc(lot.x, lot.y, radius, lot.yaw + a0, lot.yaw + a1, height, look_height, **kw)

    def walk(road, s0, s1, side, step=900.0):
        rd = roads[road]
        count = max(2, int(abs(s1 - s0) / step) + 1)
        pts = []
        for i in range(count):
            x, y, z, _ = rd.offset(s0 + (s1 - s0) * i / (count - 1), side)
            pts.append([x, y, z + 100.0])
        return pts

    oak_main = roads["oak"].nearest_s(-6000.0, 0.0)[0]
    tour = [
        {"name": "overview", "duration": 12, "fov": 50, "keys": arc(0, 0, 52000, 215, 300, 17000, 0, keys=7, radius1=40000, height1=12000)},
        {"name": "highway_west", "duration": 11, "fov": 62, "keys": follow("hwy", 14000, main0 - 3000, 220, -160, 4000)},
        {"name": "main_street", "duration": 12, "fov": 62, "keys": follow("hwy", main0 - 1500, main1 - 4000, 175, -150, 3000)},
        {"name": "gas_station", "duration": 8, "fov": 58, "keys": lot_arc("gas", 3400, -150, -40, 260, 280)},
        {"name": "oak_street", "duration": 10, "fov": 62, "keys": follow("oak", oak_main + 1500, oak_main + 17000, 175, -120, 2500)},
        {"name": "house_close", "duration": 7, "fov": 55, "keys": lot_arc("oak_n1", 1900, -135, -45, 190, 220)},
        {"name": "motel", "duration": 9, "fov": 58, "keys": lot_arc("motel", 4300, -140, -50, 320, 250)},
        {"name": "church", "duration": 7, "fov": 55, "keys": lot_arc("church", 3600, -125, -55, 260, 600)},
        {"name": "forest_track", "duration": 10, "fov": 65, "keys": follow("pine", 1500, roads["pine"].length - 2500, 170, 0, 2200)},
        {"name": "cabin_pond", "duration": 8, "fov": 58, "keys": arc(*layout.POND_VIEW, 420, 0)},
        {"name": "creek", "duration": 7, "fov": 60, "keys": arc(*layout.CREEK_VIEW, 380, 0)},
        {"name": "farm", "duration": 8, "fov": 58, "keys": lot_arc("barn", 4200, -130, -40, 330, 350)},
        {"name": "player_walk", "duration": 12, "fov": 60, "walk": walk("hwy", main0 + 9000, main0 + 14500, -640.0), "walk_pitch": -5},
    ]
    cabin = lots["cabin"]
    cabin_path = [list(cabin.to_world(lx, ly)) + [cabin.z + 100.0] for lx, ly in ((60.0, -1100.0), (0.0, -520.0), (0.0, -150.0), (0.0, 140.0), (-90.0, 300.0))]
    # the walker opens the closed cabin door itself (position based: "open_doors"), waits for the swing and walks in
    cabin_walk = {"name": "cabin_walk", "duration": 12, "fov": 60, "walk": cabin_path, "walk_pitch": -4, "open_doors": True}
    cabin_arc = {"name": "cabin", "duration": 8, "fov": 58, "keys": lot_arc("cabin", 1700, -130, -60, 200, 180)}

    def inside(lx, ly, h):
        x, y = cabin.to_world(lx, ly)
        return [x, y, cabin.z + 50.0 + h]      # 50 cm: floor height of the cabin (buildings.cabin)
    cabin_interior = {"name": "cabin_interior", "duration": 7, "fov": 72, "stills": [0.0, 0.5, 1.0],
                      "keys": [{"p": inside(-330.0 + 27.0 * i, 110.0 + 13.0 * i, 165.0 - 1.5 * i), "l": inside(150.0 + 17.0 * i, 420.0 - 20.0 * i, 85.0)} for i in range(4)]}
    for shot in tour:
        shot.setdefault("stills", [0.0, 0.5, 1.0])
        shot.setdefault("scenario", "day")
    by_name = {shot["name"]: shot for shot in tour}

    def variant(name, scenario, duration=None):
        shot = dict(by_name[name]) if name in by_name else dict(cabin_arc)
        shot["name"] = "%s_%s" % (name, scenario)
        shot["scenario"] = scenario
        if duration:
            shot["duration"] = duration
        return shot

    def write(file_name, shots):
        with open(os.path.join(HERE, "cine", file_name), "w") as f:
            # no forced r.ScreenPercentage: the level's own standard applies (SpnWorld 90 %, ASpnWorldLighting; `Spn.Quality quality` = 100 %)
            json.dump({"warmup_frames": 240, "cut_warmup_frames": 60, "console": [], "shots": shots}, f, indent=1)
        print("wrote cine/%s: %d shots, %.0f s" % (file_name, len(shots), sum(s["duration"] for s in shots)))

    write("lighting.json", [variant(name, scenario, 6) for scenario in ("day", "overcast", "spn_overcast", "night") for name in ("main_street", "gas_station", "house_close", "motel", "cabin_pond")])
    write("gameplay.json", [by_name["player_walk"], dict(cabin_walk, scenario="day")])
    # the reference forest section in the series look: down the pine track, open the cabin door, look around inside
    pine_len = roads["pine"].length
    forest_walk = {"name": "forest_walk", "duration": 16, "fov": 60, "walk": walk("pine", pine_len - 9500.0, pine_len - 1900.0, 110.0), "walk_pitch": -6}
    write("gameplay_fab.json", [dict(forest_walk, scenario="spn_overcast"), dict(cabin_walk, scenario="spn_overcast"), dict(cabin_interior, scenario="spn_overcast")])
    # performance before/after the gameplay integration (2026-11-13): the same three runs, one of them over the gameplay route at the north end
    # of Oak Street (houses oak_n6 / oak_n7, the NPCs) towards the pine track
    oak = roads["oak"]
    route_s0 = oak.nearest_s(-6600.0, 15000.0)[0]
    write("route_perf.json", [dict(by_name["main_street"], duration=15), dict(by_name["forest_track"], duration=15),
                              {"name": "oak_north_route", "duration": 15, "fov": 62, "scenario": "day", "keys": follow("oak", route_s0, oak.length - 600.0, 175, 0, 2500)}])
    write("moody.json",[variant(name, "spn_overcast", 6) for name in ("forest_track", "cabin_pond", "creek", "cabin", "main_street", "house_close")])
    write("interior.json", [dict(cabin_interior, name="cabin_interior_" + scenario, scenario=scenario) for scenario in ("day", "overcast", "night")])
    write("showcase.json", tour[:-1] + [dict(cabin_arc, scenario="day"), dict(cabin_walk, scenario="day"), dict(cabin_interior, scenario="day"), by_name["player_walk"]]
          + [variant(name, "overcast", 7) for name in ("highway_west", "main_street", "oak_street", "creek")]
          + [variant(name, "spn_overcast", 7) for name in ("forest_track", "cabin_pond", "cabin")]
          + [variant(name, "night", 7) for name in ("main_street", "gas_station", "motel", "house_close", "cabin")])
    # Performance probe of the weakest shot (-Mode perf): the same run again and again, each time with one part of the renderer switched off.
    # Every variant first restores the defaults, so the difference to "base" is the cost of that part. A measurement, not a quality setting.
    restore = ["ShowFlag.SkeletalMeshes 1", "r.Shadow.Virtual.Enable 1", "r.DynamicGlobalIlluminationMethod 1", "r.ReflectionMethod 1", "r.Lumen.HardwareRayTracing 1", "ShowFlag.InstancedGrass 1",
               "ShowFlag.InstancedStaticMeshes 1", "r.VolumetricFog 1", "r.VolumetricCloud 1", "r.ScreenPercentage 100", "r.Nanite.MaxPixelsPerEdge 1", "r.AntiAliasingMethod 4"]
    probes = [("base", []), ("no_shadows", ["r.Shadow.Virtual.Enable 0"]), ("no_lumen_gi", ["r.DynamicGlobalIlluminationMethod 0"]), ("no_lumen_reflections", ["r.ReflectionMethod 0"]),
              ("lumen_software_tracing", ["r.Lumen.HardwareRayTracing 0"]), ("no_grass", ["ShowFlag.InstancedGrass 0"]), ("no_trees_shrubs", ["ShowFlag.InstancedStaticMeshes 0"]), ("no_megaplants", ["ShowFlag.SkeletalMeshes 0"]),
              ("no_volumetric_fog", ["r.VolumetricFog 0"]), ("no_clouds", ["r.VolumetricCloud 0"]), ("screen_50", ["r.ScreenPercentage 50"]),
              ("nanite_edge_2", ["r.Nanite.MaxPixelsPerEdge 2"]), ("taa_instead_of_tsr", ["r.AntiAliasingMethod 2"]), ("base_again", [])]
    write("forest_profile.json", [dict(by_name["forest_track"], name="forest_" + name, console=restore + commands) for name, commands in probes])
    spec = {"warmup_frames": 240, "cut_warmup_frames": 60, "console": [], "shots": tour}
    os.makedirs(os.path.join(HERE, "cine"), exist_ok=True)
    with open(os.path.join(HERE, "cine", "tour.json"), "w") as f:
        json.dump(spec, f, indent=1)
    print("wrote cine/tour.json with %d shots, %.0f s" % (len(tour), sum(s["duration"] for s in tour)))


if __name__ == "__main__":
    main()
