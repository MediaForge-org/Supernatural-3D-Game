"""Tests of the parametric building system (templates, kit, roles/validation, export) without Unreal.

Run from Unreal/SupernaturalProt:  python3 -m unittest Tools.test_spn_building_system
Geometry rules only; engine collision, CharacterMovement stepping and door swing in the engine are Windows tests.
"""

import copy
import dataclasses
import hashlib
import itertools
import json
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import spn_building_kit as K  # noqa: E402
import spn_building_export as X  # noqa: E402
import spn_building_roles as R  # noqa: E402
import spn_building_templates as T  # noqa: E402

SEEDS = (20261027, 7, 99)
PER_SEED = 8


def all_variants():
    for name, template in sorted(T.TEMPLATES.items()):
        for seed in SEEDS:
            for params in T.make_variants(template, PER_SEED, seed):
                yield name, template, params


class VariantSweepTest(unittest.TestCase):
    def test_every_template_yields_valid_variants_for_every_seed(self):
        for name, template in T.TEMPLATES.items():
            for seed in SEEDS:
                variants = T.make_variants(template, PER_SEED, seed)
                self.assertEqual(len(variants), PER_SEED, "%s seed %d" % (name, seed))
                self.assertEqual(variants, T.make_variants(template, PER_SEED, seed))  # deterministic

    def test_variants_actually_differ(self):
        for name, template in T.TEMPLATES.items():
            variants = T.make_variants(template, 12, SEEDS[0])
            keys = {(v.width, v.depth, v.door_side, v.roof, v.rooms, v.hall_width, v.seed_split, v.stair_landing) for v in variants}
            self.assertEqual(len(keys), 12, name)
        self.assertGreater(len({v.rooms for v in T.make_variants(T.TEMPLATES["cottage"], 24, 5)}), 1)
        self.assertGreater(len({v.rooms for v in T.make_variants(T.TEMPLATES["office"], 24, 5)}), 2)
        self.assertTrue(any(v.stair_landing for v in T.make_variants(T.TEMPLATES["house"], 24, 5)))
        self.assertTrue(any(not v.stair_landing for v in T.make_variants(T.TEMPLATES["house"], 24, 5)))

    def test_all_variants_pass_the_geometric_checks(self):
        count = 0
        for name, template, params in all_variants():
            problems = R.check_building(T.generate(template, params))
            self.assertEqual(problems, [], "%s %r" % (name, params))
            count += 1
        self.assertEqual(count, 6 * len(SEEDS) * PER_SEED)

    def test_rooms_have_realistic_sizes_and_are_inside_the_walls(self):
        for name, template, params in all_variants():
            spec = T.generate(template, params)
            self.assertTrue(spec.rooms, name)
            for r in spec.rooms:
                lo, hi = sorted((r.x1 - r.x0, r.y1 - r.y0))
                self.assertGreaterEqual(lo, T.MIN_HALL - 0.5)
                if r.name not in ("hall", "hall2", "landing", "corridor"):
                    self.assertGreaterEqual(lo, T.MIN_ROOM - 0.5, "%s %s" % (name, r.name))
                self.assertGreaterEqual(r.x0, 9.5)
                self.assertLessEqual(r.x1, spec.size[0] - 9.5)
                self.assertLessEqual(r.y1, spec.size[1] - 9.5)
            # no two rooms of one floor overlap
            for f in range(spec.floors):
                rs = [r for r in spec.rooms if r.floor == f]
                for i, a in enumerate(rs):
                    for b in rs[i + 1:]:
                        self.assertFalse(a.x0 < b.x1 - 0.5 and a.x1 > b.x0 + 0.5 and a.y0 < b.y1 - 0.5 and a.y1 > b.y0 + 0.5, "%s: %s overlaps %s" % (name, a.name, b.name))

    def test_landing_stairs_are_valid(self):
        t = T.TEMPLATES["house"]
        landing = [v for v in T.make_variants(t, 30, 11) if v.stair_landing]
        self.assertTrue(landing)
        for params in landing:
            spec = T.generate(t, params)
            self.assertEqual(R.check_building(spec), [])
            treads = K.stair_steps_layout(spec)
            steps, rise, run, _, _ = K.stair_geometry(spec)
            self.assertEqual(len(treads), steps + 1)
            self.assertAlmostEqual(treads[-1][0] + treads[-1][1], K.stair_length(spec))
            self.assertAlmostEqual(max(t[2] for t in treads), K.floor_top(spec, 1) - K.floor_top(spec, 0))
            flat = [t for t in treads if t[1] == spec.stair_width]
            self.assertEqual(len(flat), 1)
            tops = [t[2] for t in treads]
            self.assertEqual(tops, sorted(tops))
            for a, b in zip(tops, tops[1:]):
                self.assertLessEqual(b - a, rise + 1e-6)


class NegativeCheckTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.house = T.generate(T.TEMPLATES["house"], T.Params(1100.0, 1100.0, 2, "W", "gable", 300.0, 160.0, 350.0, "d_h", 0.5))
        cls.cottage = T.generate(T.TEMPLATES["cottage"], T.Params(900.0, 700.0, 1, "W", "gable", 300.0, 160.0, 350.0, "d_c", 0.5))
        cls.office = T.generate(T.TEMPLATES["office"], T.Params(1600.0, 1000.0, 1, "W", "flat", 300.0, 160.0, 350.0, "d_o", 0.5, 3))

    def test_the_reference_buildings_are_valid(self):
        for spec in (self.house, self.cottage, self.office):
            self.assertEqual(R.check_building(spec), [])

    def test_room_without_a_doorway_is_unreachable(self):
        walls = list(self.cottage.inner_walls)
        i = next(i for i, w in enumerate(walls) if w.axis == "x")
        walls[i] = dataclasses.replace(walls[i], doorways=())
        problems = R.check_building(dataclasses.replace(self.cottage, inner_walls=tuple(walls)))
        self.assertTrue(any("unreachable" in p for p in problems), problems)

    def test_office_without_corridor_doorways_is_unreachable(self):
        walls = [dataclasses.replace(w, doorways=()) if w.axis == "y" else w for w in self.office.inner_walls]
        problems = R.check_building(dataclasses.replace(self.office, inner_walls=tuple(walls)))
        self.assertTrue(any("unreachable" in p for p in problems), problems)

    def test_too_narrow_doorway_is_reported(self):
        walls = list(self.cottage.inner_walls)
        i = next(i for i, w in enumerate(walls) if w.axis == "x")
        walls[i] = dataclasses.replace(walls[i], doorways=tuple((c, 70.0) for c, _ in walls[i].doorways))
        problems = R.check_building(dataclasses.replace(self.cottage, inner_walls=tuple(walls)))
        self.assertTrue(any("too small" in p or "unreachable" in p for p in problems), problems)

    def test_low_ceiling_is_reported(self):
        self.assertTrue(any("ceiling" in p for p in R.check_building(dataclasses.replace(self.cottage, floor_height=260.0))))

    def test_capsule_size_boundary(self):
        # Doorways are 110 cm: a 48 cm capsule radius passes the whole reference set, 56 does not.
        for spec in (self.house, self.cottage, self.office):
            self.assertEqual(R.check_building(spec, radius=48.0), [])
            self.assertNotEqual(R.check_building(spec, radius=56.0), [])

    def test_door_swing_blocked_by_a_wall_next_to_the_door(self):
        # An interior wall 40 cm inside the entrance door's wall blocks the swing of the door actor (door actors are in W/E walls).
        walls = list(self.cottage.inner_walls) + [K.InnerWall("x", 50.0, 10.0, 690.0)]
        problems = R.check_building(dataclasses.replace(self.cottage, inner_walls=tuple(walls)))
        self.assertTrue(any("swing" in p or "blocked" in p or "unreachable" in p for p in problems), problems)

    def test_spec_rules(self):
        self.assertIsNone(K.validate_spec(self.house))
        self.assertIsNotNone(K.validate_spec(dataclasses.replace(self.house, stair=False)))
        self.assertIsNotNone(K.validate_spec(dataclasses.replace(self.house, size=(1100.0, 600.0), stair_landing=True)))
        self.assertIsNotNone(K.validate_spec(dataclasses.replace(self.house, floors=4)))
        self.assertIsNotNone(K.validate_spec(dataclasses.replace(self.house, roof="dome")))

    def test_template_parameters_are_validated(self):
        t = T.TEMPLATES["cottage"]
        good = T.make_variants(t, 1, 1)[0]
        for bad in (dataclasses.replace(good, width=5000.0), dataclasses.replace(good, floors=2), dataclasses.replace(good, roof="dome"),
                    dataclasses.replace(good, door_side="N"), dataclasses.replace(good, hall_width=100.0), dataclasses.replace(good, rooms=9),
                    dataclasses.replace(good, stair_landing=True), dataclasses.replace(good, floor_height=250.0)):
            with self.assertRaises(ValueError):
                T.generate(t, bad)
        # geometry that cannot be made walkable is refused, not repaired
        with self.assertRaises(ValueError):
            T.generate(T.TEMPLATES["house"], T.Params(1000.0, 800.0, 2, "W", "gable", 320.0, 160.0, 350.0, "d", 0.5, 2, True))
        self.assertEqual(T.window_centres(700.0, 350.0, [350.0]), [])
        self.assertTrue(all(120 <= c <= 580 for c in T.window_centres(700.0, 300.0, [])))


def codes_of(spec, layout=None, **kw):
    layout = layout if layout is not None else R.build_standalone(spec)
    return {c for c, _ in R.check_layout(spec, layout, **kw)}


def mutated(spec, edit):
    """A fresh layout of `spec` after `edit(layout)` changed its primitives (the spec itself stays valid)."""
    layout = R.build_standalone(spec)
    edit(layout)
    return layout


def crate(layout, x, y, z0, sx, sy, sz, label="Crate"):
    layout.add("B_" + label, "cube", (x, y, z0 + sz / 2.0), (sx, sy, sz), "z")


class MutationTest(unittest.TestCase):
    """The checks must notice real defects: every mutation of a valid building has to be reported with the expected code."""

    @classmethod
    def setUpClass(cls):
        cls.house = T.generate(T.TEMPLATES["house"], T.Params(1100.0, 1100.0, 2, "W", "gable", 300.0, 160.0, 350.0, "d_h", 0.5))
        cls.cottage = T.generate(T.TEMPLATES["cottage"], T.Params(900.0, 700.0, 1, "W", "gable", 300.0, 160.0, 350.0, "d_c", 0.5))
        cls.office = T.generate(T.TEMPLATES["office"], T.Params(1600.0, 1000.0, 1, "W", "flat", 300.0, 160.0, 350.0, "d_o", 0.5, 3))

    def test_unmutated_buildings_have_no_findings(self):
        for spec in (self.house, self.cottage, self.office):
            self.assertEqual(codes_of(spec), set())

    def test_missing_wall_piece_is_a_gap(self):
        def edit(layout):
            i = next(i for i, p in enumerate(layout.prims) if p["label"].startswith("B_F0_S_"))
            del layout.prims[i]
        self.assertIn("wall_gap", codes_of(self.cottage, mutated(self.cottage, edit)))

    def test_duplicated_wall_piece_overlaps(self):
        def edit(layout):
            layout.prims.append(copy.deepcopy(next(p for p in layout.prims if p["label"].startswith("B_F0_N_"))))
        self.assertIn("overlap", codes_of(self.cottage, mutated(self.cottage, edit)))

    def test_floating_box_is_found(self):
        self.assertIn("floating", codes_of(self.cottage, mutated(self.cottage, lambda l: crate(l, 650.0, 120.0, 150.0, 30.0, 30.0, 30.0))))

    def test_missing_floor_slab_is_found(self):
        def edit(layout):
            layout.prims[:] = [p for p in layout.prims if not p["label"].endswith("_Floor")]
        codes = codes_of(self.cottage, mutated(self.cottage, edit))
        self.assertIn("no_floor", codes)
        self.assertIn("floating", codes)

    def test_missing_upper_slab_is_found(self):
        def edit(layout):
            layout.prims[:] = [p for p in layout.prims if "_Slab1_" not in p["label"]]
        self.assertIn("no_floor", codes_of(self.house, mutated(self.house, edit)))

    def test_blocked_doorway_and_unreachable_room(self):
        door = next(o for o in self.cottage.openings if o.door_id)
        def edit(layout):
            crate(layout, 0.0, door.centre, 6.0, 16.0, 100.0, 200.0, "DoorBlock")
        codes = codes_of(self.cottage, mutated(self.cottage, edit))
        self.assertTrue({"doorway_blocked", "overlap"} & codes, codes)
        self.assertIn("room_unreachable", codes)

    def test_crate_across_the_hall_cuts_off_rooms(self):
        def edit(layout):
            crate(layout, 80.0, 450.0, 5.0, 120.0, 250.0, 120.0, "HallCrate")
        self.assertTrue({"room_unreachable", "entrance_blocked"} & codes_of(self.cottage, mutated(self.cottage, edit)))

    def test_box_in_the_stairwell_and_at_the_stair_foot(self):
        _, _, _, sxo, syo = K.stair_geometry(self.house)
        L_ = K.stair_length(self.house)
        c = codes_of(self.house, mutated(self.house, lambda l: crate(l, sxo + 60.0, syo + L_ / 2.0, 330.0, 60.0, 60.0, 80.0, "WellBlock")))
        self.assertTrue({"stairwell_blocked", "floating", "overlap"} & c, c)
        c = codes_of(self.house, mutated(self.house, lambda l: crate(l, sxo + 60.0, syo - 50.0, 5.0, 120.0, 60.0, 120.0, "FootBlock")))
        self.assertIn("stair_foot_blocked", c)

    def test_low_ceiling_and_small_doorway_specs(self):
        self.assertIn("ceiling", codes_of(dataclasses.replace(self.cottage, floor_height=260.0)))
        walls = [dataclasses.replace(w, doorways=tuple((c, 70.0) for c, _ in w.doorways)) for w in self.cottage.inner_walls]
        self.assertIn("doorway_small", codes_of(dataclasses.replace(self.cottage, inner_walls=tuple(walls))))

    def test_window_on_a_partition_overlapping_and_sliver_openings(self):
        part = next(w for w in self.office.inner_walls if w.axis == "x" and w.a0 <= 10.0)
        bad = K.Opening("S", part.pos, 120.0, 95.0, 215.0)
        self.assertIn("window_conflict", codes_of(dataclasses.replace(self.office, openings=self.office.openings + (bad,))))
        a = next(o for o in self.cottage.openings if o.side == "S")
        overlap = dataclasses.replace(a, centre=a.centre + 60.0)
        self.assertIn("opening_overlap", codes_of(dataclasses.replace(self.cottage, openings=self.cottage.openings + (overlap,))))
        sliver = dataclasses.replace(a, centre=a.centre + a.width + 5.0)
        self.assertIn("wall_sliver", codes_of(dataclasses.replace(self.cottage, openings=self.cottage.openings + (sliver,))))

    def test_door_swing_and_graph_findings(self):
        walls = list(self.cottage.inner_walls) + [K.InnerWall("x", 50.0, 10.0, 690.0)]
        self.assertTrue({"swing_blocked", "doorway_blocked", "room_unreachable", "dead_room"} & codes_of(dataclasses.replace(self.cottage, inner_walls=tuple(walls))))
        walls = [dataclasses.replace(w, doorways=()) if w.axis == "y" else w for w in self.office.inner_walls]
        codes = codes_of(dataclasses.replace(self.office, inner_walls=tuple(walls)))
        self.assertIn("dead_room", codes)
        self.assertIn("room_unreachable", codes)

    def test_graph_agrees_with_the_flood_fill_on_the_reference_set(self):
        for spec in (self.house, self.cottage, self.office):
            g = R.room_graph(spec)
            self.assertEqual(R.graph_reachable(g), set(g["nodes"]))
            self.assertTrue(all("?" not in (a, b) for a, b, _ in g["edges"]))
        kinds = {k for _, _, k in R.room_graph(self.house)["edges"]}
        self.assertEqual(kinds, {"entrance", "doorway", "stair"})


class CornerSweepTest(unittest.TestCase):
    """validate_params is the single feasibility gate: whatever it accepts must generate AND pass every geometric check; whatever it
    rejects must be refused by generate() too. Corners of every template range (sizes, doors, roofs, heights, rooms, landing)."""

    def corners(self, t):
        mid = lambda r: float((r[0] + r[1]) / 2 // t.step * t.step)  # noqa: E731
        sizes_w = sorted({t.width[0], t.width[1], mid(t.width)})
        sizes_d = sorted({t.depth[0], t.depth[1], mid(t.depth)})
        for w, d, fl, side, roof, fh, hw, rooms, land in itertools.product(
                sizes_w, sizes_d, t.floors, t.door_sides, t.roofs, t.floor_heights, sorted({t.hall_width[0], t.hall_width[1]}),
                range(t.rooms[0], t.rooms[1] + 1), (False, True)):
            yield T.Params(w, d, fl, side, roof, fh, hw, t.window_spacing[0], "d_corner", 0.5, rooms, land)

    def test_gate_and_generator_agree_and_accepted_corners_are_valid(self):
        accepted = rejected = checked = 0
        for name, t in T.TEMPLATES.items():
            for i, p in enumerate(self.corners(t)):
                reason = T.validate_params(t, p)
                if reason:
                    rejected += 1
                    with self.assertRaises(ValueError):
                        T.generate(t, p)
                    continue
                accepted += 1
                spec = T.generate(t, p)  # must not raise
                if i % 5 == 0:
                    checked += 1
                    self.assertEqual(R.check_building(spec), [], "%s %r" % (name, p))
        self.assertGreater(accepted, 600)
        self.assertGreater(rejected, 50)
        self.assertGreater(checked, 100)

    def test_every_accepted_corner_reports_ok(self):
        for name, t in T.TEMPLATES.items():
            for p in list(self.corners(t))[::37]:
                if T.validate_params(t, p) is None:
                    rep = R.building_report("c", name, 0, T.generate(t, p))
                    self.assertEqual((rep["status"], rep["errors"]), ("ok", []), "%s %r" % (name, p))

    def test_report_is_compact_and_machine_readable(self):
        t = T.TEMPLATES["house"]
        spec = T.generate(t, T.make_variants(t, 1, 3)[0])
        rep = R.building_report("house-3", "house", 3, spec)
        self.assertEqual(set(rep), {"id", "template", "seed", "kind", "rooms", "floors", "doors", "doorways", "windows", "stairs", "status", "errors", "error_count"})
        self.assertEqual((rep["floors"], rep["stairs"], rep["doors"], rep["status"]), (2, 1, 1, "ok"))
        self.assertLess(len(json.dumps(rep)), 400)
        bad = R.building_report("x", "house", 3, dataclasses.replace(spec, floor_height=260.0, inner_walls=()))
        self.assertEqual(bad["status"], "invalid")
        self.assertTrue(set(bad["errors"]) <= set(R.CODES))

    def test_variation_is_real_not_just_size(self):
        t = T.TEMPLATES["house"]
        vs = [T.generate(t, p) for p in T.make_variants(t, 24, 9)]
        self.assertEqual({s.stair_side for s in vs}, {"W", "E"})
        self.assertGreater(len({len([r for r in s.rooms if r.floor == 0]) for s in vs}), 1)
        self.assertGreater(len({tuple(w.pos for w in s.inner_walls if w.axis == "y") for s in vs}), 6)
        self.assertGreater(len({sum(1 for o in s.openings if o.sill > 0) for s in vs}), 2)


class ExportTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        t = T.TEMPLATES["house"]
        cls.house = T.generate(t, T.make_variants(t, 1, 5)[0])
        cls.office = T.generate(T.TEMPLATES["office"], T.make_variants(T.TEMPLATES["office"], 1, 5)[0])

    def test_export_is_json_stable_and_complete(self):
        a = X.export_building(self.house, (100.0, 200.0), 0, "h")
        b = X.export_building(self.house, (100.0, 200.0), 0, "h")
        self.assertEqual(json.dumps(a, sort_keys=True), json.dumps(b, sort_keys=True))
        lay = R.build_standalone(self.house)
        self.assertEqual(len(a["modules"]), len(lay.prims))
        self.assertEqual(a["schema_version"], X.SCHEMA_VERSION)
        self.assertTrue({"wall", "floor", "slab", "stair", "roof", "rail"} <= {m["role"] for m in a["modules"]})
        self.assertEqual({m["slot"] for m in a["modules"]} - set(X.ASSET_SLOTS), set())
        self.assertTrue({"outer_wall", "inner_wall", "floor", "roof", "stair", "railing"} <= {m["slot"] for m in a["modules"]})
        self.assertEqual(len(a["topology"]["rooms"]), len(self.house.rooms))
        self.assertTrue({r.name for r in self.house.rooms} <= {n["name"] for n in a["navigation"]} and "stair" in {n["name"] for n in a["navigation"]})
        self.assertEqual(a["modules"][0]["center"][0], lay.prims[0]["center"][0] + 100.0)
        self.assertEqual(set(a["slots"]), {"outer_wall", "inner_wall", "floor", "ceiling", "roof", "window", "door_frame", "door_leaf", "stair", "railing"})

    def test_version_1_readers_still_find_their_keys(self):
        doc = X.export_building(self.house)
        for key in ("name", "kind", "size", "floors", "origin", "rooms", "openings", "navigation", "modules", "lights"):
            self.assertIn(key, doc)
        for m in doc["modules"]:
            for key in ("label", "role", "collision", "walkable", "mesh", "material", "center", "size", "rot"):
                self.assertIn(key, m)
        self.assertTrue(all(m["walkable"] == (m["role"] in R.WALKABLE_ROLES) for m in doc["modules"]))

    def test_units_are_centimetres_and_slabs_know_their_ceiling(self):
        doc = X.export_building(self.house)
        walls = [m for m in doc["modules"] if m["role"] == "wall"]
        self.assertEqual(min(min(m["size"]) for m in walls), 20.0)       # the wall thickness, in cm
        self.assertEqual(doc["unreal"]["units"], "cm")
        slab = [m for m in doc["modules"] if m["role"] == "slab"]
        self.assertTrue(slab and all(m["also_ceiling_of_floor"] == 0 for m in slab))
        self.assertEqual(doc["spec"]["floor_height"], self.house.floor_height)

    def test_placement_rotation_is_rigid_and_composes(self):
        base = X.export_building(self.office, (0.0, 0.0), 0)
        for yaw in (90, 180, 270):
            turned = X.export_building(self.office, (500.0, -300.0), yaw)
            self.assertEqual(len(turned["modules"]), len(base["modules"]))
            for a, b in zip(base["modules"], turned["modules"]):
                x, y = X._rotate_point(a["center"][0], a["center"][1], yaw)
                self.assertAlmostEqual(b["center"][0], x + 500.0, places=2)
                self.assertAlmostEqual(b["center"][1], y - 300.0, places=2)
                self.assertEqual(b["center"][2], a["center"][2])
                self.assertEqual(sorted(b["size"][:2]), sorted(a["size"][:2]))
                self.assertEqual(b["size"][2], a["size"][2])
                if yaw in (90, 270):
                    self.assertEqual(b["size"][:2], [a["size"][1], a["size"][0]])
            # the footprint box turns with it
            xs = [m["center"][0] for m in turned["modules"]]
            self.assertAlmostEqual(max(xs) - min(xs), max(m["center"][1] for m in base["modules"]) - min(m["center"][1] for m in base["modules"]) if yaw in (90, 270)
                                   else max(m["center"][0] for m in base["modules"]) - min(m["center"][0] for m in base["modules"]), delta=1.0)
        # four quarter turns about the origin return the original points
        x, y = 123.0, -45.0
        for _ in range(4):
            x, y = X._rotate_point(x, y, 90)
        self.assertEqual((round(x, 6), round(y, 6)), (123.0, -45.0))

    def test_rotated_doors_and_pitched_roofs_keep_their_meaning(self):
        base = X.export_building(self.house)["unreal"]["doors"][0]
        for yaw in (90, 180, 270):
            d = X.export_building(self.house, (0.0, 0.0), yaw)["unreal"]["doors"][0]
            self.assertAlmostEqual(d["yaw"], (base["yaw"] + yaw) % 360.0)
            x, y = X._rotate_point(base["hinge"][0], base["hinge"][1], yaw)
            self.assertAlmostEqual(d["hinge"][0], x, places=2)
            self.assertAlmostEqual(d["hinge"][1], y, places=2)
        gable = next(s for s in (T.generate(T.TEMPLATES["cottage"], p) for p in T.make_variants(T.TEMPLATES["cottage"], 6, 2)) if s.roof == "gable")
        roof = [m for m in X.export_building(gable, (0.0, 0.0), 90)["modules"] if m["label"].endswith("_RoofA")][0]
        self.assertEqual(roof["rot"][1], 90.0)          # yaw added after the pitch
        self.assertNotEqual(roof["rot"][0], 0.0)

    def test_export_refuses_invalid_input(self):
        with self.assertRaises(X.ExportError):
            X.export_building(self.house, yaw=45)
        broken = dataclasses.replace(self.house, floor_height=260.0)
        with self.assertRaises(X.ExportError) as caught:
            X.export_building(broken)
        self.assertTrue(any("ceiling" in p for p in caught.exception.problems))
        # strict=False skips the geometry checks but still validates the document
        self.assertEqual(X.export_building(self.house, strict=False)["schema_version"], 2)

    def test_validate_export_finds_corrupt_documents(self):
        good = X.export_building(self.house)
        self.assertEqual(X.validate_export(good), [])
        def broken(edit):
            doc = copy.deepcopy(good)
            edit(doc)
            return X.validate_export(doc)
        self.assertTrue(broken(lambda d: d["modules"][0].update(size=[0.0, 10.0, 10.0])))
        self.assertTrue(broken(lambda d: d["modules"][1].update(center=[float("nan"), 0.0, 0.0])))
        self.assertTrue(broken(lambda d: d["modules"][2].update(slot="chimney")))
        self.assertTrue(broken(lambda d: d["modules"][3].update(label=d["modules"][2]["label"])))
        self.assertTrue(broken(lambda d: d["modules"][4].update(material="gold")))
        self.assertTrue(broken(lambda d: d["modules"].pop()))
        self.assertTrue(broken(lambda d: d["unreal"].update(yaw_deg=45)))
        self.assertTrue(broken(lambda d: d["rooms"][0].update(rect=[0.0, 99999.0, 0.0, 10.0])))
        self.assertTrue(broken(lambda d: d["unreal"]["doors"][0].update(id="ghost")))
        self.assertTrue(broken(lambda d: d.update(schema_version=1)))
        self.assertTrue(broken(lambda d: d["unreal"]["doors"].clear()))

    def test_roles_cover_every_label_pattern(self):
        self.assertEqual(R.role_of("B_Stair_03"), "stair")
        self.assertEqual(R.role_of("B_StairRail"), "rail")
        self.assertEqual(R.role_of("B_StairRailS"), "rail")
        self.assertEqual(R.role_of("B_Floor"), "floor")
        self.assertEqual(R.role_of("B_Slab1_2"), "slab")
        self.assertEqual(R.role_of("B_RoofA"), "roof")
        self.assertEqual(R.role_of("B_GableS_1"), "roof")
        self.assertEqual(R.role_of("B_F0_W_01"), "wall")
        self.assertEqual(X.slot_of("B_F1_In2_03", "wall"), "inner_wall")
        self.assertEqual(X.slot_of("B_F1_N_03", "wall"), "outer_wall")

    GOLDEN = "da326d565a994794bae6d99d7bdb0c68bddd5cb0efa7481a1602a5c47e93f4aa"

    def test_golden_hash_of_the_reference_variants(self):
        # Snapshot of the generator output: a deliberate change of templates/kit/export changes this hash (update GOLDEN with the change).
        digest = hashlib.sha256()
        for name, template in sorted(T.TEMPLATES.items()):
            for params in T.make_variants(template, 4, 20261027):
                digest.update(json.dumps(X.export_building(T.generate(template, params), strict=False), sort_keys=True).encode())
        self.assertEqual(digest.hexdigest(), self.GOLDEN)

    def test_golden_hash_is_the_same_in_other_processes_and_hash_seeds(self):
        import subprocess
        here = os.path.dirname(os.path.abspath(__file__))
        code = ("import sys, json, hashlib; sys.path.insert(0, %r); import spn_building_templates as T, spn_building_export as X; d = hashlib.sha256()\n"
                "for n, t in sorted(T.TEMPLATES.items()):\n"
                "    for p in T.make_variants(t, 4, 20261027): d.update(json.dumps(X.export_building(T.generate(t, p), strict=False), sort_keys=True).encode())\n"
                "print(d.hexdigest())" % here)
        for seed in ("0", "123456"):
            out = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True, check=True, env=dict(os.environ, PYTHONHASHSEED=seed)).stdout.strip()
            self.assertEqual(out, self.GOLDEN)


if __name__ == "__main__":
    unittest.main()
