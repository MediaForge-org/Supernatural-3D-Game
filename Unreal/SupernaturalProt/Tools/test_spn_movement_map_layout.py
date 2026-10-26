"""Checks of the movement test map layout without Unreal (M2-A / M2-A.1, Docs/Proposals/0011).

Run from anywhere:  python3 -I Unreal/SupernaturalProt/Tools/test_spn_movement_map_layout.py
(-I is fine: the test puts its own folder on sys.path.)

Besides the geometry itself the tests compare the layout with the code that uses it, because a mismatch there only shows up
on Windows: the editor script (keys and properties it uses), the C++ automation tests (their copies of the layout constants),
the C++ defaults (step height and jump height) and the portable core (the same defaults).
"""

import ast
import hashlib
import json
import math
import os
import re
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import spn_movement_map_layout as L  # noqa: E402

PROJECT = os.path.abspath(os.path.join(HERE, ".."))
REPO = os.path.abspath(os.path.join(PROJECT, "..", ".."))
SOURCE = os.path.join(PROJECT, "Source", "SupernaturalProt")
MAP_TESTS = os.path.join(SOURCE, "Spn", "Tests", "SpnMovementMapAutomation.cpp")
SUPPORT = os.path.join(SOURCE, "Spn", "Tests", "SpnMovementTestSupport.h")
SETTINGS_HEADER = os.path.join(PROJECT, "Source", "SpnCore", "Public", "SpnMovement.h")
CORE_HEADER = os.path.join(REPO, "Core", "include", "spn", "core", "Movement.h")
CREATE_SCRIPT = os.path.join(HERE, "create_spn_movement_test_map.py")

WALKABLE_ANGLE = 44.8
JUMP_TOLERANCE_FOR_CLASSES = 3.0


def read(path):
    with open(path, encoding="utf-8") as handle:
        return handle.read()


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


def parse_cpp_constants(text):
    """Scalar `constexpr float|int32 Name = value;` and array `constexpr float Name[] = {...};` of a C++ file."""
    scalars = {}
    arrays = {}
    for match in re.finditer(r"constexpr\s+(?:float|int32)\s+(\w+)\s*=\s*(-?[0-9.]+)f?\s*;", text):
        scalars[match.group(1)] = float(match.group(2))
    for match in re.finditer(r"constexpr\s+float\s+(\w+)\[\]\s*=\s*\{([^}]*)\}\s*;", text):
        arrays[match.group(1)] = [float(v.strip().rstrip("f")) for v in match.group(2).split(",") if v.strip()]
    return scalars, arrays


class LayoutTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = L.build_layout()
        cls.prims = {p["label"]: p for p in cls.data["prims"]}
        cls.step = L.MAX_STEP_HEIGHT
        cls.jump = L.JUMP_HEIGHT

    # --- reproducibility and identity -------------------------------------------------------------------------------

    def test_reproducible(self):
        self.assertEqual(L.to_json(L.build_layout()), L.to_json(L.build_layout()))

    def test_reproducible_across_processes_and_hash_seeds(self):
        digests = set()
        for seed in ("0", "1", "12345"):
            env = dict(os.environ, PYTHONHASHSEED=seed)
            out = subprocess.run([sys.executable, "-I", "-c",
                                  "import sys; sys.path.insert(0, %r); import spn_movement_map_layout as L; import hashlib; "
                                  "print(hashlib.sha256(L.to_json(L.build_layout()).encode()).hexdigest())" % HERE],
                                 capture_output=True, text=True, env=env, check=True)
            digests.add(out.stdout.strip())
        self.assertEqual(len(digests), 1, "the layout must not depend on the process (hash seed, dict order)")
        self.assertEqual(digests.pop(), hashlib.sha256(L.to_json(self.data).encode()).hexdigest())

    def test_labels_unique_and_budget(self):
        labels = [p["label"] for p in self.data["prims"]]
        self.assertEqual(len(labels), len(set(labels)), "object ids (actor labels) must be unique")
        self.assertLess(len(labels), 500, "actor budget for the test map")
        self.assertTrue(all(label.startswith("SPN_MV_") for label in labels), "all labels carry the map prefix")

    def test_no_duplicate_geometry(self):
        seen = {}
        for p in self.data["prims"]:
            key = (p["shape"], tuple(p["center"]), tuple(p["size"]), tuple(p["rot"]))
            self.assertNotIn(key, seen, "%s duplicates %s" % (p["label"], seen.get(key)))
            seen[key] = p["label"]

    def test_values_are_finite_and_valid(self):
        for p in self.data["prims"]:
            for value in p["center"] + p["size"] + p["rot"]:
                self.assertTrue(math.isfinite(value), p["label"])
            self.assertTrue(all(v > 0 for v in p["size"]), p["label"])
            self.assertEqual(len(p["center"]), 3)
            self.assertEqual(len(p["rot"]), 3)
            self.assertIn(p["shape"], L.SHAPE_PATHS)
            self.assertIn(p["material"], L.MATERIAL_PATHS)
            self.assertIn(p["zone"], list(L.ZONES) + ["base", "boundary"], p["label"])

    # --- size, boundary, areas ---------------------------------------------------------------------------------------

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
            self.assertGreaterEqual(p["size"][0] if name in ("East", "West") else p["size"][1], 500.0, "walls are thick: no tunnelling")
        n, s_ = aabb(self.prims["SPN_MV_Boundary_North"]), aabb(self.prims["SPN_MV_Boundary_South"])
        e, w = aabb(self.prims["SPN_MV_Boundary_East"]), aabb(self.prims["SPN_MV_Boundary_West"])
        self.assertAlmostEqual(n[0][1], L.HALF)
        self.assertAlmostEqual(s_[1][1], -L.HALF)
        self.assertAlmostEqual(e[0][0], L.HALF)
        self.assertAlmostEqual(w[1][0], -L.HALF)
        # Corners are closed: the north/south walls span past the east/west faces.
        self.assertGreaterEqual(n[1][0], e[1][0] - 1e-6)
        self.assertLessEqual(n[0][0], w[0][0] + 1e-6)

    def test_floor_has_no_gaps(self):
        # One continuous slab under the whole square: nothing to fall through, no edge to fall off.
        floor_prims = [p for p in self.data["prims"] if p["label"] == "SPN_MV_Floor"]
        self.assertEqual(len(floor_prims), 1)
        lo, hi = aabb(floor_prims[0])
        self.assertLessEqual(lo[0], -L.HALF)
        self.assertGreaterEqual(hi[0], L.HALF)
        self.assertLessEqual(lo[1], -L.HALF)
        self.assertGreaterEqual(hi[1], L.HALF)

    def test_zones_do_not_overlap_and_prims_stay_in_their_zone(self):
        names = list(L.ZONES)
        for i, a in enumerate(names):
            for b in names[i + 1:]:
                ax0, ax1, ay0, ay1 = L.ZONES[a]
                bx0, bx1, by0, by1 = L.ZONES[b]
                self.assertTrue(ax1 <= bx0 or bx1 <= ax0 or ay1 <= by0 or by1 <= ay0, "%s overlaps %s" % (a, b))
        for name, (x0, x1, y0, y1) in L.ZONES.items():
            self.assertLess(x0, x1)
            self.assertLess(y0, y1)
            self.assertGreaterEqual(min(x0, y0), -L.HALF, name)
            self.assertLessEqual(max(x1, y1), L.HALF, name)
        for p in self.data["prims"]:
            zone = p["zone"]
            if zone in ("base", "boundary"):
                continue
            x0, x1, y0, y1 = L.ZONES[zone]
            lo, hi = aabb(p)
            self.assertTrue(lo[0] >= x0 - 1e-6 and hi[0] <= x1 + 1e-6 and lo[1] >= y0 - 1e-6 and hi[1] <= y1 + 1e-6,
                            "%s leaves zone %s: %s %s" % (p["label"], zone, lo, hi))

    def test_areas_a_to_h_cover_every_zone_once(self):
        self.assertEqual(sorted(L.AREAS), ["A_flat", "B_slopes", "C_stairs", "D_obstacles", "E_uneven", "F_camera", "G_buildings", "H_boundary"])
        assigned = [zone for zones in L.AREAS.values() for zone in zones]
        self.assertEqual(len(assigned), len(set(assigned)), "a zone belongs to one area")
        self.assertEqual(sorted(set(assigned) - {"boundary"}), sorted(L.ZONES))
        self.assertEqual(self.data["areas"], {k: list(v) for k, v in L.AREAS.items()})

    def test_every_area_has_content(self):
        counts = {}
        for p in self.data["prims"]:
            counts[p["zone"]] = counts.get(p["zone"], 0) + 1
        for area, zones in L.AREAS.items():
            if area == "A_flat":
                self.assertGreater(counts.get("speedlane", 0), 0)  # the plaza itself is open ground on purpose
                continue
            self.assertTrue(any(counts.get(zone, 0) > 0 for zone in zones), area)

    # --- spawn, reset points, guard ---------------------------------------------------------------------------------

    def free_of_geometry(self, x, y, z):
        """True when the character capsule at (x, y, z) overlaps none of the shapes (conservative boxes)."""
        r, h = L.CAPSULE_RADIUS, L.CAPSULE_HALF_HEIGHT
        for p in self.data["prims"]:
            if p["zone"] in ("base", "boundary"):
                continue
            lo, hi = aabb(p)
            if lo[0] < x + r and hi[0] > x - r and lo[1] < y + r and hi[1] > y - r and lo[2] < z + h and hi[2] > z - h:
                return False
        return True

    def test_player_start_is_on_open_ground(self):
        sx, sy, sz = self.data["player_start"]
        self.assertTrue(self.free_of_geometry(sx, sy, sz))
        for p in self.data["prims"]:
            if p["zone"] in ("base", "boundary"):
                continue
            lo, hi = aabb(p)
            near = lo[0] - 100.0 <= sx <= hi[0] + 100.0 and lo[1] - 100.0 <= sy <= hi[1] + 100.0
            self.assertFalse(near, "%s stands at the player start" % p["label"])
        self.assertGreater(sz, L.CAPSULE_HALF_HEIGHT, "spawn above the floor, not inside it")
        self.assertLess(sz, L.CAPSULE_HALF_HEIGHT + 30.0, "spawn close to the floor")

    def test_reset_points_are_valid(self):
        points = self.data["reset_points"]
        names = [p["name"] for p in points]
        self.assertEqual(len(names), len(set(names)))
        self.assertGreaterEqual(len(points), 14)
        for point in points:
            x, y, z = point["pos"]
            self.assertLess(abs(x), L.HALF - 1500.0, point["name"] + " is well inside the walls")
            self.assertLess(abs(y), L.HALF - 1500.0, point["name"])
            self.assertTrue(self.free_of_geometry(x, y, z), "reset point %s is inside geometry" % point["name"])
            self.assertGreater(z, L.CAPSULE_HALF_HEIGHT)
        # Every area is reachable from a reset point: one next to each zone of the areas A..G.
        for zones in L.AREAS.values():
            for zone in zones:
                if zone != "boundary":
                    self.assertIn(zone, names, "no reset point at zone %s" % zone)

    def test_guard_covers_the_square_and_lists_the_reset_points(self):
        guard = self.data["guard"]
        cx, cy, cz = guard["center"]
        ex, ey, ez = guard["extent"]
        self.assertGreaterEqual(ex, L.HALF)
        self.assertGreaterEqual(ey, L.HALF)
        self.assertLessEqual(cz - ez, -1000.0, "reaches far below the floor")
        self.assertGreaterEqual(cz + ez, L.WALL_HEIGHT, "reaches above the wall tops")
        self.assertEqual(guard["reset_points"], [p["pos"] for p in self.data["reset_points"]])
        self.assertEqual(guard["safe_location"], list(L.PLAYER_START))
        # The safe location is itself on open ground.
        self.assertTrue(self.free_of_geometry(*guard["safe_location"]))

    # --- obstacles --------------------------------------------------------------------------------------------------

    def test_obstacle_heights(self):
        for name, depth in (("Thin", L.THIN_DEPTH), ("Thick", L.THICK_DEPTH)):
            heights = []
            for h in L.OBSTACLE_HEIGHTS:
                p = self.prims["SPN_MV_Obstacle_%s_%03d" % (name, h)]
                lo, hi = aabb(p)
                self.assertAlmostEqual(lo[2], 0.0)
                self.assertAlmostEqual(hi[2], float(h))
                self.assertAlmostEqual(p["size"][0], depth)
                heights.append(hi[2])
            self.assertEqual(heights, sorted(heights))
        required = {20, 25, 30, 40, 45, 50, 75, 100, 150}  # work order M2-A.1 section 13
        self.assertTrue(required <= set(L.OBSTACLE_HEIGHTS))
        self.assertTrue({60, 80, 120} <= set(L.OBSTACLE_HEIGHTS), "the M2-A heights stay")
        self.assertEqual(list(L.OBSTACLE_HEIGHTS), sorted(set(L.OBSTACLE_HEIGHTS)))

    def test_obstacle_heights_cover_every_class_and_boundary(self):
        heights = L.OBSTACLE_HEIGHTS
        self.assertTrue(any(h < self.step for h in heights), "below the step height")
        self.assertIn(self.step, heights, "exactly the step height")
        self.assertTrue(any(self.step < h < self.jump for h in heights), "jump candidates between step height and apex")
        self.assertIn(self.jump, heights, "exactly the jump apex")
        self.assertTrue(any(h > self.jump + JUMP_TOLERANCE_FOR_CLASSES for h in heights), "clearly above the apex")
        self.assertNotEqual(self.step, self.jump, "step height and jump height are independent values")

    def test_obstacles_are_spaced_and_inside_their_zone(self):
        x0, x1, y0, y1 = L.ZONES["obstacles"]
        previous = None
        for i, h in enumerate(L.OBSTACLE_HEIGHTS):
            y = L.obstacle_y(i)
            self.assertAlmostEqual(self.prims["SPN_MV_Obstacle_Thin_%03d" % h]["center"][1], y)
            self.assertAlmostEqual(self.prims["SPN_MV_Obstacle_Thick_%03d" % h]["center"][1], y)
            if previous is not None:
                self.assertGreaterEqual(y - previous - 500.0, 2 * L.CAPSULE_RADIUS + 10.0, "room for the character between two obstacles")
            previous = y
            self.assertGreaterEqual(y - 250.0, y0)
            self.assertLessEqual(y + 250.0, y1)

    # --- ramps and stairs -------------------------------------------------------------------------------------------

    def test_ramps_have_the_stated_angle_and_a_closed_landing(self):
        for angle in L.RAMP_ANGLES:
            p = self.prims["SPN_MV_Ramp_%02d" % angle]
            self.assertAlmostEqual(p["rot"][0], float(angle))
            landing = self.prims["SPN_MV_Ramp_%02d_Landing" % angle]
            self.assertAlmostEqual(landing["size"][2], L.RAMP_LENGTH * math.sin(math.radians(angle)), places=2)
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
            self.assertAlmostEqual(x, L.RAMP_START_X, places=2)
            self.assertAlmostEqual(z, 0.0, places=2)

    def test_ramps_have_rails_on_slope_and_landing(self):
        for i, angle in enumerate(L.RAMP_ANGLES):
            label = "SPN_MV_Ramp_%02d" % angle
            landing_top = aabb(self.prims[label + "_Landing"])[1][2]
            for tag in ("Rail_W", "Rail_E"):
                rail = self.prims["%s_%s" % (label, tag)]
                self.assertAlmostEqual(rail["rot"][0], float(angle), msg="rail follows the slope")
                self.assertAlmostEqual(rail["size"][2], L.RAIL_HEIGHT)
                self.assertAlmostEqual(abs(rail["center"][1] - L.ramp_y(i)), 200.0 - L.RAIL_THICKNESS / 2.0)
            for tag in ("RailW", "RailE", "EndRail"):
                rail = self.prims["%s_Landing_%s" % (label, tag)]
                self.assertAlmostEqual(aabb(rail)[0][2], landing_top, delta=0.01, msg="rail stands on the landing")
                self.assertAlmostEqual(aabb(rail)[1][2], landing_top + L.RAIL_HEIGHT, delta=0.01)
            # Rails are higher than anything the character can climb or jump (step + jump apex), so no hop gets over them.
            self.assertGreater(L.RAIL_HEIGHT, self.step + self.jump + 30.0)

    def test_staircases(self):
        rises = []
        for i, (rise, run) in enumerate(L.STAIRCASES):
            label = "SPN_MV_Stairs_R%02d" % rise
            previous = 0.0
            for k in range(1, L.STAIR_STEPS + 1):
                step = self.prims["%s_Step_%02d" % (label, k)]
                top = aabb(step)[1][2]
                self.assertAlmostEqual(top - previous, float(rise))
                self.assertAlmostEqual(step["size"][0], float(run))
                self.assertGreaterEqual(step["size"][1], 100.0, "stairs are wide enough for the capsule and the camera")
                self.assertAlmostEqual(step["center"][1], L.stair_y(i))
                previous = top
            landing = aabb(self.prims[label + "_Landing"])[1][2]
            self.assertAlmostEqual(landing, previous)
            rises.append(rise)
        self.assertEqual(rises, sorted(set(rises)))
        self.assertTrue(any(r < self.step - 2 for r in rises), "at least one climbable staircase")
        self.assertIn(int(self.step), rises, "a staircase exactly at the step height")
        self.assertTrue(any(r > self.step + 2 for r in rises), "at least one staircase above the step height")

    def test_staircases_have_side_walls_and_end_rails(self):
        for rise, _ in L.STAIRCASES:
            label = "SPN_MV_Stairs_R%02d" % rise
            top = aabb(self.prims[label + "_Landing"])[1][2]
            for tag in ("Side_W", "Side_E"):
                self.assertAlmostEqual(aabb(self.prims["%s_%s" % (label, tag)])[1][2], top + L.RAIL_HEIGHT)
            end = self.prims[label + "_EndRail"]
            self.assertAlmostEqual(aabb(end)[0][2], top)
            self.assertAlmostEqual(aabb(end)[1][2], top + L.RAIL_HEIGHT)
            # The side walls stand outside the clear width of the stairs.
            west, east = self.prims[label + "_Side_W"], self.prims[label + "_Side_E"]
            clear = east["center"][1] - west["center"][1] - L.RAIL_THICKNESS
            self.assertGreaterEqual(clear, 280.0)

    def test_house_stairs_are_comfortable(self):
        steps = [self.prims["SPN_MV_House_Stair_%02d" % i] for i in range(1, 18)]
        tops = [aabb(s)[1][2] for s in steps]
        rises = [b - a for a, b in zip([5.0] + tops, tops)]
        for rise in rises:
            self.assertAlmostEqual(rise, rises[0], delta=0.01)
            self.assertLessEqual(rise, self.step - 2.0, "interior stairs are walkable without jumping")
            self.assertGreaterEqual(rise, 15.0)
        run = steps[0]["size"][1]
        self.assertGreaterEqual(run, 25.0)
        self.assertAlmostEqual(2 * rises[0] + run, 63.0, delta=10.0, msg="Blondel's rule: 2 rise + run is about 63 cm")
        self.assertGreaterEqual(steps[0]["size"][0], 100.0, "stairs wide enough for the capsule")

    # --- uneven ground, hills, terraces ------------------------------------------------------------------------------

    def test_rocks_are_walkable_low_unevenness(self):
        tops = [aabb(p)[1][2] for p in self.data["prims"] if p["zone"] == "rocks"]
        self.assertEqual(len(tops), 80)
        self.assertLessEqual(max(tops), self.step - 1.0, "every rock edge is a step, never a wall")
        self.assertGreater(max(tops), 12.0)
        self.assertGreater(min(tops), 0.0)

    def test_terraces_step_20_cm(self):
        tops = [aabb(self.prims["SPN_MV_Terrace_%d" % i])[1][2] for i in range(1, 7)]
        diffs = [round(b - a, 6) for a, b in zip(tops, tops[1:])]
        self.assertEqual(set(diffs), {20.0})
        self.assertEqual(tops[0], 20.0)
        self.assertLess(20.0, self.step, "terrace steps are walkable")

    def test_hills_are_walkable_domes(self):
        for i in (1, 2, 3):
            p = self.prims["SPN_MV_Hill_%d" % i]
            radius = p["size"][0] / 2.0
            cap = p["center"][2] + radius
            self.assertGreater(cap, 100.0)
            slope = math.degrees(math.acos((radius - cap) / radius))
            self.assertLess(slope, WALKABLE_ANGLE, "hill %d rim slope" % i)

    # --- speed lane --------------------------------------------------------------------------------------------------

    def test_speed_lane_is_free_and_long_enough(self):
        x0, x1, y0, y1 = L.ZONES["speedlane"]
        self.assertTrue(x0 <= L.LANE_X - 300.0 and L.LANE_X + 300.0 <= x1)
        self.assertGreaterEqual(y1 - y0, 20000.0, "a lane of at least 200 m")
        run = 8.0 * 100.0 * 5.0  # a 5 s sprint at 8 m/s, cm
        self.assertGreaterEqual(y1 - L.LANE_START_Y, run * 2.0)
        self.assertTrue(self.free_of_geometry(L.LANE_X, L.LANE_START_Y, 100.0))
        markers = [p for p in self.data["prims"] if p["zone"] == "speedlane"]
        self.assertEqual(len(markers), L.LANE_MARKERS)
        for p in markers:
            self.assertLessEqual(aabb(p)[1][2], 1.0 + 1e-6, "markers are flat: no obstacle in the lane")

    # --- buildings, doors, windows ---------------------------------------------------------------------------------

    def test_doors(self):
        ids = [d["id"] for d in self.data["doors"]]
        self.assertEqual(len(ids), len(set(ids)))
        self.assertTrue(all(i.startswith("mv_") for i in ids), "ids must not clash with Lvl_SpnDoorTest")
        self.assertGreaterEqual(len(ids), 2)

    def test_doors_sit_in_doorways(self):
        doorways = [o for o in self.data["openings"] if o["kind"] == "doorway" and o["axis"] == "x"]
        for door in self.data["doors"]:
            hx, hy, _ = door["hinge"]
            fits = [o for o in doorways if abs(o["fixed"] - hx) < 1e-6 and o["from"] <= hy - 100.0 + 1e-6 and hy <= o["to"] + 1e-6]
            self.assertTrue(fits, "door %s (hinge %s) is not inside a doorway" % (door["id"], door["hinge"]))

    def test_openings(self):
        self.assertGreaterEqual(len(self.data["openings"]), 10)
        for o in self.data["openings"]:
            if o["kind"] == "doorway":
                self.assertGreaterEqual(o["width"], 100.0, o["wall"] + " doorway width")
                self.assertGreaterEqual(o["height"], 192.0 + 20.0, o["wall"] + " doorway height: capsule plus clearance")
                self.assertLessEqual(o["sill"], 0.5, "a doorway starts at the floor")
            else:
                # A window must not be passable: the sill is above anything a step or a jump gets over.
                self.assertGreater(o["sill"], self.jump + 30.0, o["wall"] + " window sill")
                self.assertLessEqual(o["width"], 250.0)

    def test_no_doorway_opens_onto_a_drop(self):
        # The warehouse floor is 100 cm above the ground: its only doorway is the one the dock ramp leads to.
        doorways = [o for o in self.data["openings"] if o["zone"] == "warehouse" and o["kind"] == "doorway"]
        self.assertEqual(len(doorways), 1)
        self.assertEqual(doorways[0]["fixed"], 3000.0)
        self.assertIn("SPN_MV_Warehouse_DockRamp_Rail_W", self.prims)
        self.assertIn("SPN_MV_Warehouse_DockRamp_Rail_E", self.prims)
        # No doorway in the first-floor outer walls of the house.
        for o in self.data["openings"]:
            if o["wall"].startswith("SPN_MV_House_F_") and o["wall"] != "SPN_MV_House_F_Divider":
                self.assertEqual(o["kind"], "window")

    def test_house_west_wall_is_cut_for_door_and_window(self):
        names = [k for k in self.prims if k.startswith("SPN_MV_House_G_West_")]
        self.assertGreaterEqual(len(names), 4)

    def test_house_floor_levels_line_up(self):
        top_step = aabb(self.prims["SPN_MV_House_Stair_17"])[1][2]
        slab = aabb(self.prims["SPN_MV_House_Slab_North"])[1][2]
        self.assertAlmostEqual(top_step, slab, places=3)
        ground_wall_top = max(aabb(p)[1][2] for k, p in self.prims.items() if k.startswith("SPN_MV_House_G_West"))
        slab_bottom = aabb(self.prims["SPN_MV_House_Slab_Main"])[0][2]
        self.assertAlmostEqual(ground_wall_top, slab_bottom, places=3)

    def test_stairwell_has_a_rail(self):
        rail = self.prims["SPN_MV_House_StairRail"]
        slab = aabb(self.prims["SPN_MV_House_Slab_Main"])[1][2]
        self.assertAlmostEqual(aabb(rail)[0][2], slab)
        self.assertGreaterEqual(aabb(rail)[1][2] - slab, 100.0)

    def test_head_room_in_buildings(self):
        # Capsule is 192 cm tall: every ceiling the player can stand under is at least 220 cm above its floor.
        self.assertGreaterEqual(aabb(self.prims["SPN_MV_Alley_Ceiling"])[0][2], 220.0)
        self.assertGreaterEqual(aabb(self.prims["SPN_MV_House_Roof"])[0][2] - 320.0, 220.0)
        self.assertGreaterEqual(aabb(self.prims["SPN_MV_Hut_Roof"])[0][2] - 5.0, 220.0)

    def test_camera_spaces_exist(self):
        pillars = [p for p in self.data["prims"] if p["zone"] == "pillars"]
        self.assertEqual(len(pillars), 25)
        xs = sorted({p["center"][0] for p in pillars})
        gap = xs[1] - xs[0] - pillars[0]["size"][0]
        self.assertGreaterEqual(gap, 250.0, "the character and the camera arm fit between the pillars")
        # Alley: 150 cm between the inner faces (narrow passage), with a low ceiling section.
        west, east = self.prims["SPN_MV_Alley_West_01"], self.prims["SPN_MV_Alley_East_01"]
        inner = east["center"][0] - west["center"][0] - west["size"][0]
        self.assertAlmostEqual(inner, 150.0)


class ConsistencyTest(unittest.TestCase):
    """The layout against the code that spawns it, tests it and the defaults it is built around."""

    @classmethod
    def setUpClass(cls):
        cls.data = L.build_layout()
        cls.scalars, cls.arrays = parse_cpp_constants(read(MAP_TESTS))
        cls.support_scalars, _ = parse_cpp_constants(read(SUPPORT))

    def test_cpp_test_constants_match_the_layout(self):
        s, a = self.scalars, self.arrays
        self.assertEqual(a["ObstacleHeights"], [float(h) for h in L.OBSTACLE_HEIGHTS])
        self.assertEqual(a["RampAngles"], [float(v) for v in L.RAMP_ANGLES])
        self.assertEqual(a["StairRises"], [float(rise) for rise, _ in L.STAIRCASES])
        self.assertEqual(s["StairSteps"], L.STAIR_STEPS)
        self.assertEqual(s["ObstacleSpacing"], L.OBSTACLE_SPACING)
        self.assertEqual(s["ThinObstacleX"], L.THIN_X)
        self.assertEqual(s["ThinObstacleDepth"], L.THIN_DEPTH)
        self.assertEqual(s["RampStartX"], L.RAMP_START_X)
        self.assertEqual(s["RampLength"], L.RAMP_LENGTH)
        self.assertEqual(s["RampSpacing"], L.RAMP_SPACING)
        self.assertEqual(s["StairStartX"], L.STAIR_START_X)
        self.assertEqual(s["StairFirstY"], L.STAIR_FIRST_Y)
        self.assertEqual(s["StairSpacing"], L.STAIR_SPACING)
        self.assertEqual(s["LaneX"], L.LANE_X)
        self.assertEqual(s["LaneStartY"], L.LANE_START_Y)
        self.assertEqual(s["ExpectedPrimitiveCount"], len(self.data["prims"]))
        self.assertLessEqual(s["MinimumResetPoints"], len(self.data["reset_points"]))
        self.assertEqual(s["ExpectedStepHeight"], L.MAX_STEP_HEIGHT)
        self.assertEqual(s["ExpectedJumpHeight"], L.JUMP_HEIGHT)
        self.assertEqual(s["WalkableAngleLimit"], 44.8)

    def test_cpp_support_constants_match_the_layout(self):
        self.assertEqual(self.support_scalars["MapHalfExtent"], L.HALF)
        self.assertEqual(self.support_scalars["StandZ"], L.CAPSULE_HALF_HEIGHT + 1.0)
        self.assertEqual(self.data["player_start"][2], 100.0)

    def test_cpp_defaults_match_the_layout_and_the_core(self):
        header = read(SETTINGS_HEADER)
        core = read(CORE_HEADER)
        step_cm = float(re.search(r"float MaxStepHeight = ([0-9.]+)f;", header).group(1))
        jump_cm = float(re.search(r"float JumpHeight = ([0-9.]+)f;", header).group(1))
        self.assertEqual(step_cm, L.MAX_STEP_HEIGHT)
        self.assertEqual(jump_cm, L.JUMP_HEIGHT)
        core_step = float(re.search(r"double maxStepHeight = ([0-9.]+);", core).group(1))
        core_jump = float(re.search(r"double jumpHeight = ([0-9.]+);", core).group(1))
        self.assertAlmostEqual(core_step * 100.0, step_cm)
        self.assertAlmostEqual(core_jump * 100.0, jump_cm)
        self.assertLess(step_cm, jump_cm, "the normal step is lower than the jump apex")

    def test_editor_script_uses_only_existing_layout_names(self):
        tree = ast.parse(read(CREATE_SCRIPT))
        used = {node.attr for node in ast.walk(tree)
                if isinstance(node, ast.Attribute) and isinstance(node.value, ast.Name) and node.value.id == "layout_module"}
        self.assertTrue(used, "the script reads the layout through layout_module")
        for name in used:
            self.assertTrue(hasattr(L, name), "layout_module.%s does not exist" % name)

    def test_editor_script_keys_exist_in_the_layout_data(self):
        tree = ast.parse(read(CREATE_SCRIPT))
        keys = {node.slice.value for node in ast.walk(tree)
                if isinstance(node, ast.Subscript) and isinstance(node.slice, ast.Constant) and isinstance(node.slice.value, str)}
        available = set(self.data) | set(self.data["guard"]) | set(self.data["prims"][0]) | set(self.data["doors"][0])
        for key in keys:
            self.assertIn(key, available, "the editor script reads layout key %r that the layout does not provide" % key)
        for key in ("prims", "doors", "guard", "player_start", "map", "reset_points", "center", "extent", "safe_location", "hinge", "yaw"):
            self.assertIn(key, keys, "the editor script no longer reads %r" % key)

    @staticmethod
    def _normalise(name):
        name = re.sub(r"^b(?=[A-Z])", "", name)  # bAuthoredLocked -> AuthoredLocked
        return name.replace("_", "").lower()

    def test_editor_script_properties_exist_in_the_cpp_classes(self):
        tree = ast.parse(read(CREATE_SCRIPT))
        props = {node.args[0].value for node in ast.walk(tree)
                 if isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute) and node.func.attr == "set_editor_property"
                 and node.args and isinstance(node.args[0], ast.Constant)}
        headers = read(os.path.join(SOURCE, "Spn", "SpnTestDoor.h")) + read(os.path.join(SOURCE, "Spn", "SpnPlayAreaGuard.h"))
        declared = set()
        lines = headers.splitlines()
        for number, line in enumerate(lines):
            if "UPROPERTY(" in line and number + 1 < len(lines):
                declaration = lines[number + 1].split("=")[0].rstrip("; \t")  # "FVector BoxExtent = ..." -> "FVector BoxExtent"
                declared.add(self._normalise(declaration.split()[-1]))
        for prop in props - {"atmosphere_sun_light", "real_time_capture"}:  # engine properties of the sun and sky light
            self.assertIn(self._normalise(prop), declared, "set_editor_property(%r) matches no UPROPERTY of the door or guard" % prop)
        for needed in ("stable_id", "authored_locked", "box_extent", "safe_location", "reset_points"):
            self.assertIn(needed, props)

    def test_editor_script_class_paths_exist(self):
        script = read(CREATE_SCRIPT)
        for path in re.findall(r'"/Script/SupernaturalProt\.(\w+)"', script):
            header = read(os.path.join(SOURCE, "Spn", path + ".h"))
            self.assertRegex(header, r"class\s+(?:\w+_API\s+)?A%s\b" % path)

    def test_materials_and_shapes_are_engine_or_template_paths(self):
        for path in list(L.MATERIAL_PATHS.values()) + list(L.SHAPE_PATHS.values()):
            self.assertTrue(path.startswith("/Engine/BasicShapes/") or path.startswith("/Game/LevelPrototyping/"), path)

    def test_json_round_trip(self):
        text = L.to_json(self.data)
        self.assertEqual(json.loads(text), self.data)


if __name__ == "__main__":
    unittest.main(verbosity=2)
