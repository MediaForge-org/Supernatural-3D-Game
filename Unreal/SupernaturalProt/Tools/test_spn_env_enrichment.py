"""Tests of the opt-in environment enrichment layer (spn_env_enrichment.py): determinism, placement rules, validation, and that the plain V2 and V1
layouts are untouched. Geometry rules only: nothing here says how the map looks (needs the Unreal editor)."""
import json
import os
import re
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import spn_district_layout as D  # noqa: E402
import spn_env_enrichment as E  # noqa: E402
import spn_instancing as I  # noqa: E402
import spn_map_validate as V  # noqa: E402
import spn_movement_map_layout as L  # noqa: E402

V1_SHA = "619b792d46dc577ecc9d5a90d997826250543681d8f010b314f55377b450c3ad"
V2_SHA = "4df2a460da9fa6a1e66c12887979a0c4992c5d4b7ad4b05cded24f77e5fd6dab"


def sha(data):
    import hashlib
    return hashlib.sha256(L.to_json(data).encode()).hexdigest()


class EnrichmentTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plain = D.build_layout_v2()
        cls.data = E.build_layout_v2_enriched()
        cls.base_count = len(cls.plain["prims"])

    def test_plain_v1_and_v2_are_untouched_by_the_opt_in_layer(self):
        self.assertEqual(sha(L.build_layout()), V1_SHA)
        self.assertEqual(sha(D.build_layout_v2()), V2_SHA)
        self.assertNotIn("env_objects", self.plain)
        self.assertEqual(self.plain["map"], D.MAP_V2)

    def test_enriched_layout_has_its_own_level_path_and_keeps_every_base_primitive(self):
        self.assertEqual(self.data["map"], E.ENV_MAP)
        self.assertNotEqual(self.data["map"], D.MAP_V2)
        self.assertNotEqual(self.data["map"], L.build_layout()["map"])
        self.assertEqual(self.data["prims"][:self.base_count], self.plain["prims"])      # the base is a prefix, byte for byte
        self.assertEqual(self.data["doors"][:len(self.plain["doors"])], self.plain["doors"])
        self.assertTrue(self.data["generator_version"].endswith("+" + E.ENV_VERSION))

    def test_deterministic_across_runs_and_seeds_differ(self):
        again = E.build_layout_v2_enriched()
        self.assertEqual(L.to_json(again), L.to_json(self.data))
        other = E.build_layout_v2_enriched(seed=7)
        self.assertNotEqual(L.to_json(other), L.to_json(self.data))
        self.assertEqual(E.validate_enrichment(other), [])             # any seed gives a valid layout

    def test_deterministic_in_other_processes(self):
        import subprocess
        digests = set()
        for seed in ("0", "5"):
            env = dict(os.environ, PYTHONHASHSEED=seed)
            out = subprocess.run([sys.executable, "-B", "-c", "import sys, hashlib; sys.path.insert(0, %r); import spn_env_enrichment as E, spn_movement_map_layout as L;"
                                  "print(hashlib.sha256(L.to_json(E.build_layout_v2_enriched()).encode()).hexdigest())" % os.path.dirname(os.path.abspath(__file__))],
                                 capture_output=True, text=True, env=env, check=True).stdout.strip()
            digests.add(out)
        self.assertEqual(len(digests), 1)

    def test_validation_passes_and_covers_the_things_it_claims(self):
        self.assertEqual(E.validate_enrichment(self.data), [])
        report = self.data["env_report"]
        for kind in ("bin", "bench", "sign", "mailbox", "road_mark", "car", "tree", "yard", "device", "driveway"):
            self.assertGreater(report["added"].get(kind, 0), 0, kind)
        self.assertGreaterEqual(report["added"]["yard"], 2)             # at least two gate scenarios exist
        self.assertLessEqual(report["added_prims"], E.PRIM_BUDGET)
        self.assertEqual(report["added_prims"], len(self.data["prims"]) - self.base_count)

    def test_new_primitives_are_labelled_zoned_and_unique(self):
        new = self.data["prims"][self.base_count:]
        labels = [p["label"] for p in new]
        self.assertEqual(len(labels), len(set(labels)))
        self.assertTrue(all(l.startswith("SPN_E_") for l in labels))
        self.assertTrue(all(p["zone"].startswith("d_env_") for p in new))
        self.assertTrue(all(p["shape"] in ("cube", "cylinder", "sphere") and p["material"] in V.KNOWN_MATERIALS for p in new))
        self.assertEqual(V.validate_layout_data(self.data), [])

    def test_no_new_solid_stands_in_a_v1_zone_a_road_or_an_entrance_front(self):
        zones = list(L.ZONES.values())
        fronts = [r for r in (E._front_rect(n) for n, *_ in D.BUILDINGS) if r]
        road_cells, _ = D.ground_cells()
        for p in self.data["prims"][self.base_count:]:
            if E._top(p) <= E.SOLID_TOP:
                continue
            fp = D._prim_footprint(p)
            self.assertFalse(any(fp[1] > z[0] and fp[0] < z[1] and fp[3] > z[2] and fp[2] < z[3] for z in zones), p["label"])
            self.assertFalse(any(fp[1] > f[0] and fp[0] < f[1] and fp[3] > f[2] and fp[2] < f[3] for f in fronts), p["label"])
            cx, cy = p["center"][0], p["center"][1]
            self.assertNotIn((int(cx // D.CELL), int(cy // D.CELL)), road_cells, "%s stands on the road surface" % p["label"])

    def test_sidewalk_furniture_leaves_room_for_the_capsule(self):
        """Every bin, bench and sign is on the sidewalk and leaves at least SIDEWALK_MIN_CLEAR of its 200 cm width free."""
        _, sidewalk = D.ground_cells()
        furniture = [p for p in self.data["prims"][self.base_count:] if re.match(r"SPN_E_(Bin|Bench|Sign)_\d+$", p["label"]) or re.match(r"SPN_E_(Bin|Bench|Sign)_\d+_(Seat|Base|Post)$", p["label"])]
        self.assertGreater(len(furniture), 100)
        for p in furniture:
            fp = D._prim_footprint(p)
            width = min(fp[1] - fp[0], fp[3] - fp[2])
            self.assertLessEqual(width, D.SIDEWALK_W - E.SIDEWALK_MIN_CLEAR, p["label"])
            self.assertIn((int(p["center"][0] // D.CELL), int(p["center"][1] // D.CELL)), sidewalk, p["label"])

    def test_all_named_routes_still_reachable_and_door_passages_still_valid(self):
        grid = V.MapGrid(self.data)
        for name, ok, detail in D.check_routes(self.data, grid):
            self.assertTrue(ok, "%s: %s" % (name, detail))
        gates = [d for d in self.data["doors"] if d["id"].startswith("env_")]
        self.assertGreaterEqual(len(gates), 2)
        for gate in gates:
            passage = V.door_passage(self.data, gate, D.door_open_angle(gate), D.door_inward_sign(gate))
            self.assertGreaterEqual(passage["slack"], 10.0, gate["id"])
            self.assertLessEqual(V.door_pinch(self.data, gate, D.door_open_angle(gate), D.door_inward_sign(gate))["length"], 40.0, gate["id"])

    def test_interactive_objects_links_and_scenarios_are_consistent(self):
        objects = {o["id"]: o for o in self.data["env_objects"]}
        self.assertEqual(len(objects), len(self.data["env_objects"]))
        door_ids = {d["id"] for d in self.data["doors"]}
        grammar = re.compile(r"[a-z][a-z0-9_.]*$")
        for o in objects.values():
            self.assertRegex(o["id"], grammar)
            self.assertLessEqual(len(o["id"]), 96)
            self.assertRegex(o["region"], r"cell_(m?\d+)_(m?\d+)$")
            self.assertEqual(o["region"], E.region_of(o["centre_cm"][0], o["centre_cm"][1]))
            if o["kind"].endswith(".door"):
                self.assertIn(o["id"], door_ids)
                self.assertTrue(o["defaults"]["locked"])                 # gates start locked: the device is the way in
        for link in self.data["env_links"]:
            self.assertIn(link["source"], objects)
            target = objects[link["target"]]
            self.assertTrue(link["target_key"].startswith("placeholder.door.%s." % target["id"]))
            self.assertFalse(objects[link["source"]]["kind"].endswith(".door"))
        scen = {s["id"]: s for s in self.data["env_scenarios"]}
        self.assertEqual(len(scen), len(self.data["env_scenarios"]))
        for s in scen.values():
            self.assertEqual(s["status"], "PENDING_UNREAL_ADAPTER")      # never claims to have run
            self.assertEqual(s["steps"][0]["expect"], "Locked")
            for step in s["steps"]:
                self.assertIn(step["target"], objects)

    def test_every_state_field_has_an_authored_default_and_the_fixture_matches_the_data(self):
        for o in self.data["env_objects"]:
            for name, t in E.STATE_FIELDS[o["kind"]].items():
                self.assertIs(type(o["defaults"][name]), t, "%s.%s" % (o["id"], name))
        committed = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "Tests", "Core", "Data", "env_scenarios.txt")
        with open(committed, encoding="utf-8", newline="") as handle:
            self.assertEqual(handle.read(), E.core_fixture(self.data), "Tests/Core/Data/env_scenarios.txt is stale: run spn_env_enrichment.py --write-fixture")
        broken = json.loads(L.to_json(self.data))
        del next(o for o in broken["env_objects"] if o["kind"].endswith("button"))["defaults"]["presses"]
        self.assertTrue(any("would not be restored" in p for p in E.validate_enrichment(broken)))

    def test_region_ids(self):
        self.assertEqual(E.region_of(0.0, 0.0), "cell_0_0")
        self.assertEqual(E.region_of(-1.0, -1.0), "cell_m1_m1")
        self.assertEqual(E.region_of(3999.9, 4000.0), "cell_0_1")
        self.assertEqual(E.region_of(-4000.0, -4000.1), "cell_m1_m2")

    def test_enrich_refuses_anything_but_the_plain_v2_layout_once(self):
        with self.assertRaises(ValueError):
            E.enrich(L.build_layout())
        with self.assertRaises(ValueError):
            E.enrich(self.data)

    def test_instancing_keeps_the_actor_count_well_below_the_primitive_count(self):
        summary = I.summarize(self.data)
        self.assertLess(summary["actors_after"], summary["actors_before"] * 0.55)
        self.assertGreater(summary["instanced_primitives"], self.base_count * 0.6)
        env_only = [g for g in I.plan_instancing(self.data["prims"])[1] if any(i["label"].startswith("SPN_E_") for i in g["instances"])]
        self.assertGreater(len(env_only), 5)                                 # the new props really are grouped, not left as single actors
        plain = I.summarize(D.build_layout_v2())
        self.assertEqual(plain["actors_after"], 532)                          # the plain V2 numbers of the Windows report did not change

    def test_validation_catches_each_kind_of_violation(self):
        base = json.loads(L.to_json(self.data))
        def broken(mutate):
            copy = json.loads(L.to_json(self.data))
            mutate(copy)
            return E.validate_enrichment(copy)
        # a solid prop dropped in the middle of the plaza test zone
        def into_zone(d):
            d["prims"].append({"label": "SPN_E_Bad_Zone", "shape": "cube", "center": [0.0, 0.0, 50.0], "size": [100.0, 100.0, 100.0], "rot": [0, 0, 0], "material": "grid_gray", "zone": "d_env_street"})
        self.assertTrue(any("v1 test zone" in p for p in broken(into_zone)))
        # a wall across a road that cuts a named route
        def wall(d):
            d["prims"].append({"label": "SPN_E_Bad_Wall", "shape": "cube", "center": [-8950.0, -5450.0, 150.0], "size": [400.0, 60.0, 300.0], "rot": [0, 0, 0], "material": "grid_gray", "zone": "d_env_street"})
        self.assertTrue(any("overlaps an existing solid" in p for p in broken(wall)))
        # missing prefix, broken object id, link to a foreign key
        self.assertTrue(any("SPN_E_" in p for p in broken(lambda d: d["prims"].append({**d["prims"][-1], "label": "Other_Label"}))))
        def bad_id(d):
            d["env_objects"][0]["id"] = "Bad Id"
        self.assertTrue(any("invalid or duplicate" in p for p in broken(bad_id)))
        def foreign(d):
            d["env_links"][0]["target_key"] = "placeholder.door.someone_else.locked"
        self.assertTrue(any("another object" in p for p in broken(foreign)))
        def region(d):
            d["env_objects"][0]["region"] = "cell_99_99"
        self.assertTrue(any("region" in p for p in broken(region)))
        def budget(d):
            for k in range(E.PRIM_BUDGET):
                d["prims"].append({"label": "SPN_E_Pad_%d" % k, "shape": "cube", "center": [-14000.0 + (k % 100), 14000.0, 0.5], "size": [1.0, 1.0, 1.0], "rot": [0, 0, 0], "material": "grid_gray", "zone": "d_env_street"})
        self.assertTrue(any("budget" in p for p in broken(budget)))
        self.assertEqual(E.validate_enrichment(base), [])


class EditorPipelineTest(unittest.TestCase):
    """The editor script's generation logic on the enriched layout, against the fake world of test_spn_map_tools (no Unreal)."""

    def test_enriched_level_generates_verifies_and_rebuilds_without_duplicates(self):
        import test_spn_map_tools as M
        data = E.build_layout_v2_enriched()
        groups = I.plan_instancing(data["prims"])[1]
        M.X.set_generator(data["map"], data["generator_version"])
        world = M.World()
        plan = M.X.rebuild_plan(world.actors, False, True, False)
        self.assertEqual(plan.action, "create")
        individual = world.generate(data, groups)
        self.assertEqual(M.X.verify_level(world.actors, M.X.expected_counts(data, groups, individual)), [])
        labels = [a.get_actor_label() for a in world.actors]
        self.assertEqual(len(labels), len(set(labels)))
        count = len(world.actors)
        for _ in range(2):
            plan = M.X.rebuild_plan(world.actors, True, True, False)
            self.assertEqual(plan.action, "rebuild")
            world.apply(plan)
            individual = world.generate(data, groups)
        self.assertEqual(len(world.actors), count)
        self.assertEqual(M.X.verify_level(world.actors, M.X.expected_counts(data, groups, individual)), [])

    def test_script_wiring_is_opt_in_and_names_the_separate_level(self):
        text = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "create_spn_movement_test_map.py"), encoding="utf-8").read()
        self.assertIn('"--enrich" in args', text)
        self.assertIn("--enrich needs --v2", text)
        self.assertLess(text.index("spn_env_enrichment.enrich(data)"), text.index('map_path = data["map"]'))      # the level path is read AFTER the enrichment switched it
        self.assertIn('data.get("generator_version", district_module.GENERATOR_VERSION)', text)
        self.assertTrue(E.ENV_MAP.endswith("Lvl_SpnMovementTestV2Env"))


if __name__ == "__main__":
    unittest.main()
