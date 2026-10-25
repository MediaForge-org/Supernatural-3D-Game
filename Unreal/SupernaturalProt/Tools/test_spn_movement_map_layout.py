"""Checks of the movement test map layout without Unreal (M2-A, Docs/Proposals/0011).

Run from anywhere:  python3 -I Unreal/SupernaturalProt/Tools/test_spn_movement_map_layout.py
(-I is fine: the test puts its own folder on sys.path.)
"""

import math
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import spn_movement_map_layout as L  # noqa: E402

MAX_STEP = 45.0  # default max step height of the character, cm
WALKABLE_ANGLE = 44.8


def aabb(prim):
    """Conservative axis-aligned bounds (min, max) of a primitive, sign-insensitive for rotations."""
    pitch, yaw, roll = (math.radians(v) for v in prim["rot"])
    cp, sp, cy, sy, cr, sr = math.cos(pitch), math.sin(pitch), math.cos(yaw), math.sin(yaw), math.cos(roll), math.sin(roll)
    # R = Rz(yaw) * Ry(pitch) * Rx(roll); only absolute values matter for the extent.
    r = [
        [cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr],
        [sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr],
        [-sp, cp * sr, cp * cr],
    ]
    half = [v / 2.0 for v in prim["size"]]
    if prim["shape"] == "sphere":
        # Half-buried dome: only the cap above the ground (Z > 0) counts.
        radius = half[0]
        cap = prim["center"][2] + radius
        rim = math.sqrt(max(2.0 * radius * cap - cap * cap, 0.0))
        return ([prim["center"][0] - rim, prim["center"][1] - rim, 0.0], [prim["center"][0] + rim, prim["center"][1] + rim, cap])
    extent = [sum(abs(r[i][j]) * half[j] for j in range(3)) for i in range(3)]
    lo = [prim["center"][i] - extent[i] for i in range(3)]
    hi = [prim["center"][i] + extent[i] for i in range(3)]
    return lo, hi


class LayoutTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = L.build_layout()
        cls.prims = {p["label"]: p for p in cls.data["prims"]}

    def test_reproducible(self):
        self.assertEqual(L.to_json(L.build_layout()), L.to_json(L.build_layout()))

    def test_labels_unique_and_budget(self):
        labels = [p["label"] for p in self.data["prims"]]
        self.assertEqual(len(labels), len(set(labels)))
        self.assertLess(len(labels), 700, "actor budget for the test map")

    def test_known_shapes_and_materials(self):
        for p in self.data["prims"]:
            self.assertIn(p["shape"], L.SHAPE_PATHS)
            self.assertIn(p["material"], L.MATERIAL_PATHS)
            self.assertTrue(all(v > 0 for v in p["size"]), p["label"])

    def test_map_is_300_by_300_m(self):
        floor = self.prims["SPN_MV_Floor"]
        self.assertEqual(floor["size"][:2], [30000.0, 30000.0])
        self.assertEqual(floor["center"][2] + floor["size"][2] / 2.0, 0.0, "ground surface at Z = 0")

    def test_everything_but_the_boundary_is_inside_the_square(self):
        for p in self.data["prims"]:
            if p["zone"] in ("boundary",):
                continue
            lo, hi = aabb(p)
            self.assertGreaterEqual(min(lo[0], lo[1]), -L.HALF - 1e-6, p["label"])
            self.assertLessEqual(max(hi[0], hi[1]), L.HALF + 1e-6, p["label"])

    def test_boundary_walls_enclose_the_square(self):
        for name in ("North", "South", "East", "West"):
            p = self.prims["SPN_MV_Boundary_" + name]
            lo, hi = aabb(p)
            self.assertGreaterEqual(hi[2], 1500.0, "wall must be far higher than any jump or hill")
            self.assertLessEqual(lo[2], -100.0 + 1e-6, "wall reaches below the ground surface")
        n, s_ = aabb(self.prims["SPN_MV_Boundary_North"]), aabb(self.prims["SPN_MV_Boundary_South"])
        e, w = aabb(self.prims["SPN_MV_Boundary_East"]), aabb(self.prims["SPN_MV_Boundary_West"])
        self.assertAlmostEqual(n[0][1], L.HALF)
        self.assertAlmostEqual(s_[1][1], -L.HALF)
        self.assertAlmostEqual(e[0][0], L.HALF)
        self.assertAlmostEqual(w[1][0], -L.HALF)
        # Corners are closed: the north/south walls span past the east/west faces.
        self.assertGreaterEqual(n[1][0], e[1][0] - 1e-6)
        self.assertLessEqual(n[0][0], w[0][0] + 1e-6)

    def test_zones_do_not_overlap_and_prims_stay_in_their_zone(self):
        names = list(L.ZONES)
        for i, a in enumerate(names):
            for b in names[i + 1:]:
                ax0, ax1, ay0, ay1 = L.ZONES[a]
                bx0, bx1, by0, by1 = L.ZONES[b]
                self.assertTrue(ax1 <= bx0 or bx1 <= ax0 or ay1 <= by0 or by1 <= ay0, "%s overlaps %s" % (a, b))
        for p in self.data["prims"]:
            zone = p["zone"]
            if zone in ("base", "boundary"):
                continue
            x0, x1, y0, y1 = L.ZONES[zone]
            lo, hi = aabb(p)
            self.assertTrue(lo[0] >= x0 - 1e-6 and hi[0] <= x1 + 1e-6 and lo[1] >= y0 - 1e-6 and hi[1] <= y1 + 1e-6,
                            "%s leaves zone %s: %s %s" % (p["label"], zone, lo, hi))

    def test_player_start_is_on_open_ground(self):
        sx, sy, _ = self.data["player_start"]
        for p in self.data["prims"]:
            if p["zone"] in ("base", "boundary"):
                continue
            lo, hi = aabb(p)
            near = lo[0] - 100.0 <= sx <= hi[0] + 100.0 and lo[1] - 100.0 <= sy <= hi[1] + 100.0
            self.assertFalse(near, "%s stands at the player start" % p["label"])

    def test_obstacle_heights(self):
        for name, depth in (("Thin", 40.0), ("Thick", 200.0)):
            heights = []
            for h in L.OBSTACLE_HEIGHTS:
                p = self.prims["SPN_MV_Obstacle_%s_%03d" % (name, h)]
                lo, hi = aabb(p)
                self.assertAlmostEqual(lo[2], 0.0)
                self.assertAlmostEqual(hi[2], float(h))
                self.assertAlmostEqual(p["size"][0], depth)
                heights.append(hi[2])
            self.assertEqual(heights, sorted(heights))
        self.assertEqual(L.OBSTACLE_HEIGHTS, (20, 30, 40, 50, 60, 80, 100, 120, 150))

    def test_ramps_have_the_stated_angle_and_a_closed_landing(self):
        for angle in L.RAMP_ANGLES:
            p = self.prims["SPN_MV_Ramp_%02d" % angle]
            self.assertAlmostEqual(p["rot"][0], float(angle))
            landing = self.prims["SPN_MV_Ramp_%02d_Landing" % angle]
            self.assertAlmostEqual(landing["size"][2], 1500.0 * math.sin(math.radians(angle)), places=2)
        self.assertLess(max(a for a in L.RAMP_ANGLES if a < WALKABLE_ANGLE), WALKABLE_ANGLE)
        self.assertGreater(max(L.RAMP_ANGLES), WALKABLE_ANGLE, "one ramp must be too steep to walk")

    def test_ramp_top_face_starts_at_the_ground(self):
        for angle in L.RAMP_ANGLES:
            p = self.prims["SPN_MV_Ramp_%02d" % angle]
            a = math.radians(angle)
            t = p["size"][2]
            # Start of the top face: centre minus half length along the slope plus the normal offset.
            x = p["center"][0] - p["size"][0] / 2.0 * math.cos(a) - t / 2.0 * math.sin(a)
            z = p["center"][2] - p["size"][0] / 2.0 * math.sin(a) + t / 2.0 * math.cos(a)
            self.assertAlmostEqual(x, -9000.0, places=2)
            self.assertAlmostEqual(z, 0.0, places=2)

    def test_staircases(self):
        rises = []
        for rise, run in L.STAIRCASES:
            label = "SPN_MV_Stairs_R%02d" % rise
            previous = 0.0
            for i in range(1, L.STAIR_STEPS + 1):
                top = aabb(self.prims["%s_Step_%02d" % (label, i)])[1][2]
                self.assertAlmostEqual(top - previous, float(rise))
                previous = top
            landing = aabb(self.prims[label + "_Landing"])[1][2]
            self.assertAlmostEqual(landing, previous)
            rises.append(rise)
        self.assertTrue(any(r <= MAX_STEP - 5 for r in rises), "at least one climbable staircase")
        self.assertTrue(any(r > MAX_STEP for r in rises), "at least one staircase above the step height")
        self.assertNotIn(45, rises, "no stair exactly on the step-height limit")

    def test_rocks_are_walkable_low_unevenness(self):
        tops = [aabb(p)[1][2] for p in self.data["prims"] if p["zone"] == "rocks"]
        self.assertEqual(len(tops), 120)
        self.assertLessEqual(max(tops), 40.0)
        self.assertGreater(max(tops), 25.0)

    def test_terraces_step_20_cm(self):
        tops = [aabb(self.prims["SPN_MV_Terrace_%d" % i])[1][2] for i in range(1, 7)]
        diffs = [round(b - a, 6) for a, b in zip(tops, tops[1:])]
        self.assertEqual(set(diffs), {20.0})
        self.assertEqual(tops[0], 20.0)

    def test_hills_are_walkable_domes(self):
        for i in (1, 2, 3):
            p = self.prims["SPN_MV_Hill_%d" % i]
            radius = p["size"][0] / 2.0
            cap = p["center"][2] + radius
            self.assertGreater(cap, 100.0)
            slope = math.degrees(math.acos((radius - cap) / radius))
            self.assertLess(slope, WALKABLE_ANGLE, "hill %d rim slope" % i)

    def test_doors(self):
        ids = [d["id"] for d in self.data["doors"]]
        self.assertEqual(len(ids), len(set(ids)))
        self.assertTrue(all(i.startswith("mv_") for i in ids), "ids must not clash with Lvl_SpnDoorTest")
        self.assertGreaterEqual(len(ids), 2)

    def test_house_west_wall_is_cut_for_door_and_window(self):
        # Door and window cuts split the wall into at least: before, between, above/below pieces.
        names = [k for k in self.prims if k.startswith("SPN_MV_House_G_West_")]
        self.assertGreaterEqual(len(names), 4)

    def test_house_floor_levels_line_up(self):
        top_step = aabb(self.prims["SPN_MV_House_Stair_17"])[1][2]
        slab = aabb(self.prims["SPN_MV_House_Slab_North"])[1][2]
        self.assertAlmostEqual(top_step, slab, places=3)
        ground_wall_top = max(aabb(p)[1][2] for k, p in self.prims.items() if k.startswith("SPN_MV_House_G_West"))
        slab_bottom = aabb(self.prims["SPN_MV_House_Slab_Main"])[0][2]
        self.assertAlmostEqual(ground_wall_top, slab_bottom, places=3)

    def test_head_room_in_buildings(self):
        # Capsule is 192 cm tall: every ceiling the player can stand under is at least 220 cm above its floor.
        self.assertGreaterEqual(aabb(self.prims["SPN_MV_Alley_Ceiling"])[0][2], 220.0)
        self.assertGreaterEqual(aabb(self.prims["SPN_MV_House_Roof"])[0][2] - 320.0, 220.0)


if __name__ == "__main__":
    unittest.main(verbosity=2)
