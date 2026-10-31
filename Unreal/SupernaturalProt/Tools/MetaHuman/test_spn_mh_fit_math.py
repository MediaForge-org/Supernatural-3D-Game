"""Tests of spn_mh_fit_math.py on synthetic data (no Unreal)."""
import math
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spn_mh_fit_math as M  # noqa: E402

W, H, FOV = 1536, 1024, 20.0
# small symmetric "face": eye corners, nose, mouth corners, chin, ears (cm, face looking along +Y)
FACE = [(-4.7, 9.8, 160.7), (4.7, 9.8, 160.7), (-1.7, 10.5, 160.5), (1.7, 10.5, 160.5), (0.0, 14.1, 157.2),
        (-2.5, 11.0, 153.5), (2.5, 11.0, 153.5), (0.0, 11.8, 150.2), (-8.9, 0.4, 161.4), (8.9, 0.4, 161.4)]


class Projection(unittest.TestCase):
    def test_axes_match_unreal_conventions(self):
        fwd, right, up = M.rot_axes(0, 0, 0)
        self.assertEqual((fwd, right, up), ((1, 0, 0), (0, 1, 0), (0, 0, 1)))
        fwd, right, _ = M.rot_axes(0, -90, 0)
        self.assertAlmostEqual(fwd[1], -1.0)
        self.assertAlmostEqual(right[0], 1.0)

    def test_point_on_axis_projects_to_centre(self):
        cam = M.look_at_cam((0, 0, 160), 0, 0, 300)
        u, v, depth = M.project([(0, 0, 160)], cam, W, H, FOV)[0]
        self.assertAlmostEqual(u, W / 2, places=6)
        self.assertAlmostEqual(v, H / 2, places=6)
        self.assertAlmostEqual(depth, 300, places=6)

    def test_negative_yaw_sees_the_right_side(self):
        cam = M.look_at_cam((0, 6, 158), -35, 0, 300)
        pr = M.project(FACE, cam, W, H, FOV)
        # nose tip points to image right, the character's right ear (index 8) is on the image left
        self.assertGreater(pr[4][0], pr[7][0] - 1)
        self.assertLess(pr[8][0], pr[0][0])


class CameraFit(unittest.TestCase):
    def test_recovers_known_camera(self):
        truth = M.look_at_cam((0, 6, 158), -30, -6, 320)
        truth[5] = 4.0
        targets = [p[:2] for p in M.project(FACE, truth, W, H, FOV)]
        start = M.look_at_cam((0, 6, 158), -45, 0, 280)
        cam, rms = M.fit_camera(start, FACE, targets, [1.0] * len(FACE), W, H, FOV, iters=200)
        self.assertLess(rms, 0.05)
        for a, b in zip(cam, truth):
            self.assertAlmostEqual(a, b, delta=0.5)


class Correspondences(unittest.TestCase):
    def test_mirror_pairs(self):
        pairs = M.mirror_pairs(FACE)
        self.assertEqual(pairs[0], 1)
        self.assertEqual(pairs[1], 0)
        self.assertIsNone(pairs[4])
        self.assertEqual(pairs[8], 9)

    def test_curve_extremes(self):
        curves = {"crv_eyelid_upper_l": [[10, 5], [20, 2], [30, 5]], "crv_eyelid_lower_l": [[10, 5], [20, 8], [31, 6]],
                  "crv_lip_upper_outer_l": [[50, 50], [60, 48]], "crv_lip_lower_outer_r": [[40, 51], [50, 55]]}
        e = M.curve_extremes(curves)
        self.assertEqual(e["eye_l_minx"], [10, 5])
        self.assertEqual(e["eye_l_maxx"], [31, 6])
        self.assertEqual(e["mouth_minx"], [40, 51])
        self.assertEqual(e["mouth_maxx"], [60, 48])
        self.assertNotIn("eye_r_minx", e)

    def test_curve_offset_uses_same_index_points(self):
        rendered = {"c": [[0, 0], [10, 0], [20, 0]]}
        reference = {"c": [[2, 1], [12, 1], [22, 1]], "other": [[0, 0]]}
        self.assertEqual(M.curve_offset((10, 0), rendered, reference), (2.0, 1.0))
        self.assertIsNone(M.curve_offset((500, 500), rendered, reference))

    def test_curve_offset_ignores_mismatched_layouts(self):
        self.assertIsNone(M.curve_offset((0, 0), {"c": [[0, 0], [1, 1]]}, {"c": [[0, 0]]}))


class Deltas(unittest.TestCase):
    def test_image_plane_deltas(self):
        cam = M.look_at_cam((0, 0, 160), 0, 0, 300)  # looks along -Y, image right = +X
        d = M.image_to_world_deltas({0: (10.0, 0.0), 1: (0.0, -10.0)}, {0: 300.0, 1: 300.0}, cam, W, FOV, 1.0)
        s = 300.0 / M.focal_px(W, FOV) * 10.0
        self.assertAlmostEqual(d[0][0], s, places=6)
        self.assertAlmostEqual(d[1][2], s, places=6)

    def test_symmetric_deltas_remove_mean_and_mirror(self):
        pairs = {0: 1, 1: 0, 2: None, 3: 4, 4: 3}
        raw = {0: (1.0, 0.0, 0.0), 1: (1.0, 0.0, 0.0), 2: (0.5, 1.0, 0.0), 3: (0.0, 0.0, 0.3)}
        out = M.symmetric_deltas(raw, pairs)
        mean_y = sum(d[1] for d in raw.values()) / len(raw)
        self.assertEqual(out[2][0], 0.0)                         # midline keeps x = 0
        self.assertAlmostEqual(out[2][1], 1.0 - mean_y)          # mean removed
        self.assertIn(4, out)                                    # one-sided delta mirrored
        self.assertAlmostEqual(out[4][0], -out[3][0])
        # both sides moved +x (an asymmetric shift): after removing the mean x (0.625) each keeps 0.375; the mirrored blend
        # cancels the shared part, only own_share * 0.375 remains
        self.assertAlmostEqual(out[0][0], 0.3 * 0.375)
        self.assertAlmostEqual(out[1][0], 0.3 * 0.375)

    def test_empty(self):
        self.assertEqual(M.symmetric_deltas({}, {}), {})


if __name__ == "__main__":
    unittest.main()
