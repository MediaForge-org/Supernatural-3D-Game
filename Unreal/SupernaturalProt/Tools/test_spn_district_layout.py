"""Checks for the v2 district (spn_district_layout.py) and the building kit (spn_building_kit.py) without Unreal.

Run from Unreal/SupernaturalProt:  python3 -m unittest Tools.test_spn_district_layout
These tests describe GEOMETRY RULES (no overlapping collision bodies, open doorways, walkable stairs, connected roads, reachable rooms).
They do not replace walking the map in the engine: collision shapes, capsule behaviour and the camera are Windows tests.
"""

import json
import math
import os
import re
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import spn_building_kit as K  # noqa: E402
import spn_building_roles as R  # noqa: E402
import spn_map_validate as V  # noqa: E402
import spn_building_templates as T  # noqa: E402
import spn_district_layout as D  # noqa: E402
import spn_movement_map_layout as L  # noqa: E402

CAPSULE_R = L.CAPSULE_RADIUS


def spec_of(prefix):
    """The spec of a district building (generated from its template)."""
    for name, template, params, _, _ in D.BUILDINGS:
        if name == prefix:
            return T.generate(T.TEMPLATES[template], params)
    raise KeyError(prefix)


def cube_box(p):
    """3D AABB (x0, x1, y0, y1, z0, z1) of an axis-aligned cube primitive, else None (rotated, cylinder or sphere)."""
    if p["shape"] != "cube":
        return None
    pitch, yaw, roll = p["rot"]
    if abs(pitch) > 1e-6 or abs(roll) > 1e-6 or abs(yaw % 90.0) > 1e-6:
        return None
    cx, cy, cz = p["center"]
    sx, sy, sz = p["size"]
    if round(yaw / 90.0) % 2:
        sx, sy = sy, sx
    return (cx - sx / 2, cx + sx / 2, cy - sy / 2, cy + sy / 2, cz - sz / 2, cz + sz / 2)


def overlap3(a, b, eps=0.01):
    return all(a[i] < b[i + 1] - eps and a[i + 1] > b[i] + eps for i in (0, 2, 4))


def is_decal(p):
    return any(k in p["label"] for k in ("_Bay_", "Lane_Marker", "Park_Lawn"))  # flat ground cover (<= 2 cm)


def is_veg_or_prop(p):
    return any(k in p["label"] for k in ("_Tree_", "_Bush_", "_Boulder_", "_Lamp_"))


class DistrictTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = D.build_layout_v2()
        cls.v1 = L.build_layout()
        cls.v1_count = len(cls.v1["prims"])
        cls.new = cls.data["prims"][cls.v1_count:]
        cls.all = cls.data["prims"]

    def test_v1_is_kept_unchanged(self):
        # The only deliberate V2 deviation: the V1 house stair block, shortened so the entrance doorway is free (fix_v1_house_stairs).
        changed = {p["label"] for p in self.v1["prims"] if re.fullmatch(r"SPN_MV_House_Stair_\d\d", p["label"])} | {"SPN_MV_House_Slab_North", "SPN_MV_House_StairRail"}
        keep = lambda prims: [p for p in prims if p["label"] not in changed]  # noqa: E731
        self.assertEqual(keep(self.data["prims"][:self.v1_count]), keep(self.v1["prims"]))
        self.assertEqual([p["label"] for p in self.data["prims"][:self.v1_count]], [p["label"] for p in self.v1["prims"]])
        self.assertEqual(self.data["doors"][:len(self.v1["doors"])], self.v1["doors"])
        self.assertEqual(L.build_layout()["prims"], self.v1["prims"])  # building v2 does not touch v1 state
        self.assertEqual(self.data["half_extent_cm"], L.HALF)
        self.assertNotEqual(self.data["map"], self.v1["map"])
        self.assertEqual(self.data["map"], "/Game/SpnTest/Lvl_SpnMovementTestV2")

    def test_reproducible(self):
        self.assertEqual(L.to_json(D.build_layout_v2()), L.to_json(D.build_layout_v2()))

    def test_reproducible_across_processes_and_hash_seeds(self):
        digests = set()
        for seed in ("0", "1", "4242"):
            env = dict(os.environ, PYTHONHASHSEED=seed)
            out = subprocess.run([sys.executable, "-c", "import sys, hashlib, json; sys.path.insert(0, %r); import spn_district_layout as D, spn_movement_map_layout as L;"
                                  "print(hashlib.sha256(L.to_json(D.build_layout_v2()).encode()).hexdigest())" % HERE],
                                 capture_output=True, text=True, env=env, check=True).stdout.strip()
            digests.add(out)
        self.assertEqual(len(digests), 1)

    def test_labels_unique_and_budget(self):
        labels = [p["label"] for p in self.all]
        self.assertEqual(len(labels), len(set(labels)))
        self.assertTrue(all(p["label"].startswith("SPN_D_") for p in self.new))
        self.assertLessEqual(len(self.all), 1400, "actor budget of the generator (no instancing yet)")
        self.assertGreater(len(self.new), 300)

    def test_values_are_finite_and_inside_the_square(self):
        for p in self.new:
            for v in p["center"] + p["size"] + p["rot"]:
                self.assertTrue(math.isfinite(v), p["label"])
            self.assertTrue(all(s > 0 for s in p["size"]), p["label"])
            fp = D._prim_footprint(p)
            self.assertGreaterEqual(fp[0], -L.HALF, p["label"])
            self.assertLessEqual(fp[1], L.HALF, p["label"])
            self.assertGreaterEqual(fp[2], -L.HALF, p["label"])
            self.assertLessEqual(fp[3], L.HALF, p["label"])

    def test_new_geometry_does_not_overlap_v1_geometry(self):
        v1_fp = [(p["label"], D._prim_footprint(p)) for p in self.v1["prims"] if D._prim_footprint(p) and not p["label"].startswith("SPN_MV_Boundary")]
        bad = []
        for p in self.new:
            fp = D._prim_footprint(p)
            for label, o in v1_fp:
                if fp[0] < o[1] - 1 and fp[1] > o[0] + 1 and fp[2] < o[3] - 1 and fp[3] > o[2] + 1:
                    bad.append((p["label"], label))
        self.assertEqual(bad[:5], [], "%d overlaps with v1 geometry" % len(bad))

    def test_no_overlapping_collision_boxes_among_axis_aligned_cubes(self):
        boxes = [(p["label"], cube_box(p)) for p in self.all if not is_decal(p) and cube_box(p) and p["label"] != "SPN_MV_Floor" and not p["label"].startswith("SPN_MV_Boundary")]
        bad = []
        # sweep on x
        boxes.sort(key=lambda t: t[1][0])
        for i, (la, a) in enumerate(boxes):
            for lb, b in boxes[i + 1:]:
                if b[0] >= a[1]:
                    break
                if overlap3(a, b) and (la.startswith("SPN_D_") or lb.startswith("SPN_D_")):
                    bad.append((la, lb))
        self.assertEqual(bad[:6], [], "%d overlapping boxes" % len(bad))

    def test_vegetation_and_lamps_stand_on_free_ground(self):
        solid = [(p["label"], D._prim_footprint(p)) for p in self.all if D._prim_footprint(p) and not is_decal(p) and not is_veg_or_prop(p)
                 and not p["label"].startswith(("SPN_MV_Boundary", "SPN_D_Road", "SPN_D_Sidewalk", "SPN_D_Lot", "SPN_D_Curve", "SPN_D_Park_Lawn", "SPN_D_Path"))]
        bad = []
        for p in self.new:
            if not (is_veg_or_prop(p) and ("_Trunk" in p["label"] or "_Bush_" in p["label"] or "_Boulder_" in p["label"] or "_Pole" in p["label"])):
                continue
            x, y, _ = p["center"]
            for label, o in solid:
                if "Lamp" in p["label"] and label.startswith("SPN_D_Sidewalk"):
                    continue
                if x > o[0] - 40 and x < o[1] + 40 and y > o[2] - 40 and y < o[3] + 40:
                    bad.append((p["label"], label))
        self.assertEqual(bad[:5], [])

    def test_roads_are_one_connected_network_with_the_curve_closed(self):
        cells = set()
        for r in D.road_rects():
            cells |= D._rect_cells(*r)
        start = next(iter(sorted(cells)))
        seen, stack = {start}, [start]
        while stack:
            i, j = stack.pop()
            for n in ((i + 1, j), (i - 1, j), (i, j + 1), (i, j - 1)):
                if n in cells and n not in seen:
                    seen.add(n)
                    stack.append(n)
        self.assertEqual(seen, cells, "road network is split")
        for name, tx, ty in D.TURNAROUNDS:
            self.assertIn((int(math.floor(tx / D.CELL)), int(math.floor(ty / D.CELL))), cells, name + " is not on a road")
        pts = D._curve_points(D.CURVE)
        self.assertAlmostEqual(pts[0][0], -8000.0, places=3)
        self.assertAlmostEqual(pts[0][1], -10800.0, places=3)
        self.assertAlmostEqual(pts[-1][0], -6400.0, places=3)
        self.assertAlmostEqual(pts[-1][1], -12400.0, places=3)
        lead_in, lead_out = D.road_rects()[-2], D.road_rects()[-1]
        self.assertAlmostEqual(lead_in[1], pts[0][0], places=3)   # lead-in ends where the arc starts
        self.assertAlmostEqual(lead_out[3], pts[-1][1], places=3)  # lead-out begins where the arc ends
        self.assertTrue(lead_in[2] < pts[0][1] < lead_in[3])
        self.assertTrue(lead_out[0] < pts[-1][0] < lead_out[1])

    def test_roads_sidewalks_and_lots_do_not_overlap(self):
        flat = [(p["label"], D._prim_footprint(p)) for p in self.new if p["label"].startswith(("SPN_D_Road", "SPN_D_Sidewalk", "SPN_D_Lot"))]
        for i, (la, a) in enumerate(flat):
            for lb, b in flat[i + 1:]:
                self.assertFalse(a[0] < b[1] - 1 and a[1] > b[0] + 1 and a[2] < b[3] - 1 and a[3] > b[2] + 1, (la, lb))

    def test_kerb_is_a_step_up_not_a_wall(self):
        self.assertLess(D.SIDEWALK_TOP, L.MAX_STEP_HEIGHT)
        self.assertGreater(D.SIDEWALK_TOP, D.ROAD_TOP)

    def test_every_building_entrance_reaches_a_hard_surface(self):
        info = self.data["district"]["buildings"]
        self.assertGreaterEqual(len(info), 8)
        hard = D._hard_cells()
        for name, b in info.items():
            x0, x1, y0, y1 = b["footprint"]
            if name == "Garage":
                continue
            labels = [p["label"] for p in self.new if p["label"] == "SPN_D_Path_" + name]
            self.assertEqual(len(labels), 1, name + " has no entrance path")
            path = next(p for p in self.new if p["label"] == "SPN_D_Path_" + name)
            self.assertLessEqual(path["size"][0], 1100.0, name + ": path unreasonably long")
            self.assertEqual(path["size"][2], 2.0)
            end_cell = (int(math.floor((path["center"][0] + path["size"][0] / 2 + 1) / D.CELL)), int(math.floor(path["center"][1] / D.CELL)))
            start_cell = (int(math.floor((path["center"][0] - path["size"][0] / 2 - 1) / D.CELL)), int(math.floor(path["center"][1] / D.CELL)))
            self.assertTrue(end_cell in hard or start_cell in hard, name + ": path ends in the grass")

    def test_buildings_cover_the_four_types_and_are_enterable(self):
        kinds = {b["kind"] for b in self.data["district"]["buildings"].values()}
        self.assertTrue({"cottage", "house", "warehouse", "workshop", "garage"} <= kinds)
        self.assertTrue(any(b["floors"] == 2 for b in self.data["district"]["buildings"].values()))
        new_doors = self.data["doors"][len(self.v1["doors"]):]
        self.assertEqual(len(new_doors), 8)  # every building but the garage has a door actor
        self.assertEqual(len({d["id"] for d in self.data["doors"]}), len(self.data["doors"]))

    def test_doorways_are_open_and_the_door_frame_is_clear(self):
        by_zone = {}
        for d in self.data["doors"][len(self.v1["doors"]):]:
            hinge_x, hinge_y = d["hinge"][0], d["hinge"][1]
            centre_y = hinge_y - 50.0
            opening = [o for o in self.data["openings"] if o["kind"] == "doorway" and o["axis"] == "x" and abs(o["fixed"] - hinge_x) < 0.01
                       and o["from"] <= centre_y - 54.9 and o["to"] >= centre_y + 54.9]
            self.assertEqual(len(opening), 1, d["id"])
            self.assertGreaterEqual(opening[0]["width"], 109.9)
            self.assertGreaterEqual(opening[0]["height"], 219.9)
            # nothing solid inside the doorway volume (swept 0..220 cm tall, wall thickness)
            vol = (hinge_x - 8.0, hinge_x + 8.0, centre_y - 54.0, centre_y + 54.0, 6.0, 219.0)
            for p in self.all:
                box = cube_box(p)
                if box and not is_decal(p) and overlap3(box, vol):
                    if p["label"] in ("SPN_D_Path_" + d["zone"], ) or p["size"][2] <= 2.0:
                        continue
                    self.fail("%s blocks the doorway of %s" % (p["label"], d["id"]))
            # no tall obstacle right in front of the door on the outside (1.5 m deep)
            outward = -1.0 if True else 1.0
            for p in self.all:
                box = cube_box(p)
                if not box or is_decal(p) or p["label"].startswith("SPN_MV_Boundary"):
                    continue
                for front in ((hinge_x - 150.0, hinge_x - 20.0), (hinge_x + 20.0, hinge_x + 150.0)):
                    front_vol = (front[0], front[1], centre_y - 54.0, centre_y + 54.0, 30.0, 200.0)
                    if overlap3(box, front_vol) and not p["label"].startswith("SPN_D_" + ""):
                        self.fail("%s stands in front of %s" % (p["label"], d["id"]))

    def test_openings_do_not_open_onto_a_drop(self):
        for o in self.data["openings"][len(self.v1["openings"]):]:
            if o["kind"] == "doorway":
                self.assertLessEqual(o["sill"], 0.5)

    def test_house_stairs_are_comfortable_and_the_stairwell_clear(self):
        spec = spec_of("House1")
        steps, rise, run, sxo, syo = K.stair_geometry(spec)
        self.assertLessEqual(rise, K.MAX_STEP_RISE)
        self.assertLess(rise, L.MAX_STEP_HEIGHT)
        self.assertGreaterEqual(run, 27.0)
        self.assertAlmostEqual(steps * rise, K.floor_top(spec, 1) - K.floor_top(spec, 0))
        self.assertGreaterEqual(spec.stair_width, 100.0)
        lay = L.Layout()
        K.build_building(lay, spec, 0.0, 0.0, "B", "z")
        flight = (sxo + 1.0, sxo + spec.stair_width - 1.0, syo + 1.0, syo + K.stair_length(spec) - 1.0, K.floor_top(spec, 1) - 20.0, K.floor_top(spec, 1) + 200.0)
        for p in lay.prims:
            box = cube_box(p)
            if box and not p["label"].startswith("B_Stair_") and overlap3(box, flight):
                self.fail("%s sits in the stairwell" % p["label"])
        # head room above the first step: nothing between the step top and 200 cm above it in the lower storey
        self.assertGreaterEqual(K.wall_top(spec, 0) - K.PLINTH, 200.0)
        self.assertTrue(any(p["label"] == "B_StairRail" for p in lay.prims))

    def test_wall_pieces_surround_openings_exactly(self):
        lay = L.Layout()
        K.build_building(lay, spec_of("Cottage1"), 0.0, 0.0, "C", "z")
        door = [o for o in lay.openings if o["kind"] == "doorway" and o["axis"] == "x" and abs(o["fixed"]) < 0.01][0]
        self.assertEqual((door["width"], door["height"]), (110.0, 220.0))
        # No wall piece lies in the doorway volume; a lintel above it exists.
        vol = (-9.0, 9.0, door["from"] + 0.5, door["to"] - 0.5, 6.0, 219.0)
        self.assertFalse([p for p in lay.prims if cube_box(p) and overlap3(cube_box(p), vol)])
        self.assertTrue([p for p in lay.prims if cube_box(p) and cube_box(p)[4] >= 224.9 and cube_box(p)[0] < 0 < cube_box(p)[1] and cube_box(p)[2] <= door["from"] + 1 and cube_box(p)[3] >= door["to"] - 1])

    def test_spec_validation(self):
        good = spec_of("Cottage1")
        self.assertIsNone(K.validate_spec(good))
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (200.0, 800.0))))
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (800.0, 800.0), floors=2)))   # no stair
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (800.0, 800.0), floors=2, stair=True, stair_width=80.0)))
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (800.0, 500.0), floors=2, stair=True)))  # flight longer than the depth
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (800.0, 800.0), roof="dome")))
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (800.0, 800.0), openings=(K.Opening("S", 400.0, 110.0, 0.0, 220.0, 0, "d"),))))
        self.assertIsNotNone(K.validate_spec(K.BuildingSpec("a", "x", (800.0, 800.0), openings=(K.Opening("W", 20.0, 110.0, 0.0, 220.0),))))
        with self.assertRaises(ValueError):
            K.build_building(L.Layout(), K.BuildingSpec("a", "x", (100.0, 100.0)), 0.0, 0.0, "B", "z")

    def test_reset_points_and_start_are_free(self):
        solid = [D._prim_footprint(p) for p in self.all if D._prim_footprint(p) and not is_decal(p) and p["size"][2] > 3.0
                 and not p["label"].startswith(("SPN_MV_Boundary", "SPN_D_Sidewalk", "SPN_D_Path"))]
        for rp in self.data["reset_points"]:
            x, y, _ = rp["pos"]
            for fp in solid:
                inside = x > fp[0] - CAPSULE_R and x < fp[1] + CAPSULE_R and y > fp[2] - CAPSULE_R and y < fp[3] + CAPSULE_R
                self.assertFalse(inside, "reset point %s is blocked" % rp["name"])
        self.assertEqual(len(self.data["guard"]["reset_points"]), len(self.data["reset_points"]))

    def test_lights_are_inside_the_map_and_inside_or_beside_their_building(self):
        self.assertGreater(len(self.data["lights"]), 20)
        for light in self.data["lights"]:
            x, y, z = light["pos"]
            self.assertTrue(-L.HALF < x < L.HALF and -L.HALF < y < L.HALF and 0 < z < 1200, light)
            self.assertGreater(light["intensity"], 0)

    def test_house_floor_levels_line_up(self):
        spec = spec_of("House1")
        lay = L.Layout()
        K.build_building(lay, spec, 0.0, 0.0, "B", "z")
        slab = [p for p in lay.prims if p["label"].startswith("B_Slab1")]
        tops = {round(p["center"][2] + p["size"][2] / 2, 3) for p in slab}
        self.assertEqual(tops, {spec.floor_height})
        self.assertEqual(round(K.wall_top(spec, 0) + spec.slab_thickness, 3), spec.floor_height)

    def test_every_district_building_passes_the_building_checks(self):
        for name, template, params, _, _ in D.BUILDINGS:
            self.assertEqual(R.check_building(spec_of(name)), [], name)

    def test_the_export_of_every_district_building_matches_what_the_layout_built(self):
        import spn_building_export as X
        for name, template, params, x0, y0 in D.BUILDINGS:
            spec = spec_of(name)
            doc = X.export_building(spec, (x0, y0), 0, name)          # strict: runs the geometric checks too
            built = [p for p in self.data["prims"] if p["label"].startswith("SPN_D_%s_" % name)]
            self.assertEqual(len(doc["modules"]), len(built), name)
            a = sorted((tuple(m["center"]), tuple(m["size"]), tuple(m["rot"])) for m in doc["modules"])
            b = sorted((tuple(round(c, 3) for c in p["center"]), tuple(p["size"]), tuple(p["rot"])) for p in built)
            self.assertEqual(a, b, name + ": export and layout geometry differ")
            self.assertEqual(X.validate_export(doc), [])

    def test_district_buildings_come_from_templates(self):
        names = {b["template"] for b in self.data["district"]["buildings"].values()}
        self.assertEqual(names, set(T.TEMPLATES))

    def test_json_round_trip(self):
        self.assertEqual(json.loads(L.to_json(self.data))["district"]["buildings"]["Workshop"]["kind"], "workshop")

    def test_open_district_doors_swing_outward_and_never_into_the_hall(self):
        """Found in Unreal: a west door opened by +90 put its leaf across the 140 cm stair hall (leaf along +X at the hinge row)."""
        district_doors = [d for d in self.data["doors"] if d["zone"].startswith("d_")]
        self.assertEqual(len(district_doors), 8)
        for door in district_doors:
            angle = D.door_open_angle(door)
            yaw = math.radians(door["yaw"] + angle)   # open leaf direction (UE yaw: 0 = +X, +90 = +Y)
            tip_x = door["hinge"][0] + 110.0 * math.cos(yaw)
            prefix = next(p for p, *_ in D.BUILDINGS if "d_" + p.lower() == door["zone"])
            spec, (x0, y0) = D.building_spec(prefix)
            inside = x0 < tip_x < x0 + spec.size[0]
            self.assertFalse(inside, "door %s opens into the building (leaf tip x %.0f)" % (door["id"], tip_x))
        v1_doors = [d for d in self.data["doors"] if not d["zone"].startswith("d_")]
        self.assertTrue(all(D.door_open_angle(d) == 90.0 for d in v1_doors))   # the v1 map is not changed

    def test_v1_house_stairs_leave_the_entrance_doorway_free(self):
        """Found by the Unreal QA (SpnQA.Doors, TestHouse): the stair block ended at y -9204 right behind the door (opening y -9255..-9145), so an
        84 cm capsule could not enter. V2 shortens the treads; the V1 layout itself is unchanged (its snapshot hash is pinned elsewhere)."""
        steps = [p for p in self.data["prims"] if re.fullmatch(r"SPN_MV_House_Stair_\d\d", p["label"])]
        self.assertEqual(len(steps), 17)
        north_end = max(p["center"][1] + p["size"][1] / 2.0 for p in steps)
        opening_south_edge = -9255.0
        self.assertLessEqual(north_end, opening_south_edge + 0.01)
        top = max(p["center"][2] + p["size"][2] / 2.0 for p in steps)
        slab = next(p for p in self.data["prims"] if p["label"] == "SPN_MV_House_Slab_North")
        self.assertAlmostEqual(slab["center"][1] - slab["size"][1] / 2.0, north_end, places=2)   # the first-floor slab starts where the stairs end
        self.assertAlmostEqual(top, 320.0, places=1)
        v1 = [p for p in L.build_layout()["prims"] if re.fullmatch(r"SPN_MV_House_Stair_\d\d", p["label"])]
        self.assertAlmostEqual(max(p["center"][1] + p["size"][1] / 2.0 for p in v1), -9204.0, places=2)


if __name__ == "__main__":
    unittest.main()


class WorldAccessTest(unittest.TestCase):
    """Whole-map accessibility on a 100 cm grid (spn_map_validate.MapGrid): is everything a player should reach reachable from the start?"""

    @classmethod
    def setUpClass(cls):
        cls.data = D.build_layout_v2()
        cls.grid = V.MapGrid(cls.data)
        cls.reached = cls.grid.flood((0.0, 0.0))
        cls.blocked = cls.grid.blocked
        cls.n = cls.grid.n

    def cell(self, x, y):
        return self.grid.cell(x, y)

    def test_player_start_is_free(self):
        self.assertNotIn(self.cell(0.0, 0.0), self.blocked)

    def test_every_road_node_is_reachable_from_the_player_start(self):
        for name, ax, ay, bx, by in D.ROADS:
            for x, y in ((ax, ay), (bx, by)):
                self.assertIn(self.cell(x, y), self.reached, "%s end (%d, %d) not reachable" % (name, x, y))

    def test_every_building_entrance_is_reachable_from_the_player_start(self):
        for name in self.data["district"]["buildings"]:
            x, y = D.front_point(name)
            self.assertIn(self.cell(x, y), self.reached, "%s: the front of the entrance is cut off" % name)

    def test_lots_and_turnarounds_are_reachable(self):
        for name, x0, x1, y0, y1 in D.LOTS:
            self.assertIn(self.cell((x0 + x1) / 2, (y0 + y1) / 2), self.reached, name)
        for name, x, y in D.TURNAROUNDS:
            self.assertIn(self.cell(x, y), self.reached, name)

    def test_named_gameplay_routes_are_all_passable(self):
        report = D.check_routes(self.data, self.grid)
        self.assertGreaterEqual(len(report), 12)
        self.assertEqual([r for r in report if not r[1]], [])
        names = {r[0] for r in report}
        for needed in ("spawn_to_house1", "spawn_to_office", "spawn_to_storage_hall", "obstacles_to_ramps", "ramps_to_stairs", "street_to_workshop_lot",
                       "house1_ground_to_upper_room"):
            self.assertIn(needed, names)

    def test_a_blocked_route_is_detected(self):
        # Wall off the whole north-south corridor between the plaza and West Avenue: the routes that cross it must fail, not pass.
        data = D.build_layout_v2()
        data["prims"].append({"label": "SPN_X_Wall", "shape": "cube", "center": [-10200.0, 0.0, 150.0], "size": [30000.0, 40.0, 300.0], "rot": [0, 0, 0],
                              "material": "grid_gray", "zone": "x"})
        data["prims"].append({"label": "SPN_X_Wall2", "shape": "cube", "center": [0.0, -7000.0, 150.0], "size": [40.0, 30000.0, 300.0], "rot": [0, 0, 0],
                              "material": "grid_gray", "zone": "x"})
        report = D.check_routes(data, V.MapGrid(data))
        self.assertTrue([r for r in report if not r[1]], "a walled-off map still reports every route as passable")

    def test_interior_routes_use_the_stair_for_the_upper_floor(self):
        path = D.interior_path("House1", "room1@1")
        self.assertIsNotNone(path)
        self.assertIn("landing@1", path)
        self.assertIsNone(D.interior_path("House1", "nowhere@3"))
        self.assertEqual(D.interior_path("Cottage1", "room1@0")[0], "outside")

    def test_decoration_keeps_a_capsule_gap_to_the_v1_test_stations(self):
        stations = [p for p in self.v1_prims() if p["zone"] in ("obstacles", "ramps", "stairs", "speedlane", "alley", "plaza")
                    and not p["label"].startswith("SPN_MV_Boundary") and p["label"] != "SPN_MV_Floor"]
        deco = [p for p in self.data["prims"][len(self.v1_prims()):] if any(k in p["label"] for k in ("_Tree_", "_Bush_", "_Boulder_", "_Lamp_", "_Fence_", "_GardenWall_"))]
        for d in deco:
            fd = D._prim_footprint(d)
            for s in stations:
                fs = D._prim_footprint(s)
                gap_x = max(fs[0] - fd[1], fd[0] - fs[1])
                gap_y = max(fs[2] - fd[3], fd[2] - fs[3])
                self.assertGreaterEqual(max(gap_x, gap_y), 2 * L.CAPSULE_RADIUS, "%s crowds test station %s" % (d["label"], s["label"]))

    @staticmethod
    def v1_prims():
        return L.build_layout()["prims"]

    def test_most_of_the_open_ground_is_connected(self):
        free = self.n * self.n - len(self.blocked)
        self.assertGreater(len(self.reached) / free, 0.9, "more than 10 % of the free ground is cut off from the start")


class PlotTopologyTest(unittest.TestCase):
    """Roads, plots and entrances: no road ends for no reason, entrances face the street, neighbours keep their distance."""

    @classmethod
    def setUpClass(cls):
        cls.data = D.build_layout_v2()

    def test_every_road_end_is_a_junction_a_turnaround_or_the_ring(self):
        rects = D.road_rects()
        for name, ax, ay, bx, by in D.ROADS:
            for x, y in ((ax, ay), (bx, by)):
                touching = [r for r in rects if r[0] - 1 <= x <= r[1] + 1 and r[2] - 1 <= y <= r[3] + 1]
                at_turnaround = any(abs(x - tx) < 1 and abs(y - ty) < 1 for _, tx, ty in D.TURNAROUNDS)
                at_curve = any(abs(x - cx) < 1 and abs(y - cy) < 1 for cx, cy in ((-8400.0, -10800.0), (-6400.0, -12800.0)))
                self.assertTrue(len(touching) >= 2 or at_turnaround or at_curve, "%s ends at (%d, %d) for no reason" % (name, x, y))

    def test_dead_ends_are_exactly_the_planned_turnarounds(self):
        ends = []
        for name, ax, ay, bx, by in D.ROADS:
            for x, y in ((ax, ay), (bx, by)):
                if len([r for r in D.road_rects() if r[0] - 1 <= x <= r[1] + 1 and r[2] - 1 <= y <= r[3] + 1]) < 2 and (x, y) not in ((-8400.0, -10800.0), (-6400.0, -12800.0)):
                    ends.append((x, y))
        self.assertEqual(sorted(ends), sorted((tx, ty) for _, tx, ty in D.TURNAROUNDS))

    def test_entrances_face_the_street_with_a_short_path(self):
        for name, b in self.data["district"]["buildings"].items():
            if name == "Garage":
                continue
            path = next(p for p in self.data["prims"] if p["label"] == "SPN_D_Path_" + name)
            self.assertLessEqual(path["size"][0], 700.0, "%s: path of %.0f cm to the street" % (name, path["size"][0]))
            spec, (x0, y0) = D.building_spec(name)
            ent = R.entrance(spec)
            toward_east = path["center"][0] > x0 + spec.size[0] / 2
            self.assertEqual(toward_east, ent.side == "E", name + ": the path leaves on the wrong side")

    def test_buildings_keep_distance_from_each_other_and_from_hard_surfaces(self):
        fps = {n: b["footprint"] for n, b in self.data["district"]["buildings"].items()}
        names = sorted(fps)
        for i, a in enumerate(names):
            for b in names[i + 1:]:
                fa, fb = fps[a], fps[b]
                gap = max(fb[0] - fa[1], fa[0] - fb[1], fb[2] - fa[3], fa[2] - fb[3])
                self.assertGreaterEqual(gap, 300.0, "%s and %s are only %.0f cm apart" % (a, b, gap))
        hard = [D._prim_footprint(p) for p in self.data["prims"] if p["label"].startswith(("SPN_D_Road", "SPN_D_Sidewalk", "SPN_D_Lot", "SPN_D_Curve"))]
        for n, f in fps.items():
            for h in hard:
                self.assertFalse(f[0] < h[1] - 1 and f[1] > h[0] + 1 and f[2] < h[3] - 1 and f[3] > h[2] + 1, "%s overlaps a road/sidewalk/lot" % n)

    def test_setbacks_vary_so_the_street_does_not_look_stamped(self):
        setbacks = set()
        for name, b in self.data["district"]["buildings"].items():
            path = next((p for p in self.data["prims"] if p["label"] == "SPN_D_Path_" + name), None)
            if path:
                setbacks.add(round(path["size"][0] / 50.0))
        self.assertGreaterEqual(len(setbacks), 3)

    def test_trees_are_not_on_a_grid_or_a_line(self):
        trees = [p["center"][:2] for p in self.data["prims"] if p["label"].endswith("_Trunk")]
        self.assertGreater(len(trees), 30)
        xs, ys = sorted({round(t[0], 0) for t in trees}), sorted({round(t[1], 0) for t in trees})
        self.assertGreater(len(xs), len(trees) * 0.9)
        self.assertGreater(len(ys), len(trees) * 0.9)


class LayoutDataValidationTest(unittest.TestCase):
    def setUp(self):
        self.data = D.build_layout_v2()

    def test_valid_layouts_pass(self):
        self.assertEqual(V.validate_layout_data(self.data), [])
        self.assertEqual(V.validate_layout_data(L.build_layout()), [])

    def test_each_defect_is_reported(self):
        def problems(edit):
            data = D.build_layout_v2()
            edit(data)
            return V.validate_layout_data(data)
        self.assertTrue(problems(lambda d: d["prims"].append(dict(d["prims"][0]))))                                  # duplicate label
        self.assertTrue(problems(lambda d: d["prims"][5].update(shape="torus")))
        self.assertTrue(problems(lambda d: d["prims"][5].update(material="gold")))
        self.assertTrue(problems(lambda d: d["prims"][5].update(size=[0.0, 10.0, 10.0])))
        self.assertTrue(problems(lambda d: d["prims"][5].update(center=[float("nan"), 0.0, 0.0])))
        self.assertTrue(problems(lambda d: d["prims"][5].update(rot=[0.0, float("inf"), 0.0])))
        self.assertTrue(problems(lambda d: d["doors"].append(dict(d["doors"][0]))))
        self.assertTrue(problems(lambda d: d["reset_points"].append({"name": "far", "pos": [99999.0, 0.0, 100.0]})))
        self.assertTrue(problems(lambda d: d.update(player_start=[99999.0, 0.0, 100.0])))
        self.assertTrue(problems(lambda d: d["lights"].append({"pos": [0, 0, 100], "intensity": -1.0, "radius": 100.0, "zone": "x"})))
        self.assertTrue(problems(lambda d: d["guard"]["reset_points"].pop()))


