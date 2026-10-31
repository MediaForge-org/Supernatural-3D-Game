"""Pure math for fitting a MetaHuman face to a single reference photo (no Unreal, standard library only).

Camera model = Unreal's: rotator (pitch, yaw, roll) in degrees, X forward / Y right / Z up, horizontal FOV, principal point in the
image centre. A camera is [x, y, z, pitch, yaw, roll]. Projection returns (u, v, depth) in pixels (u right, v down).
Tested in test_spn_mh_fit_math.py.
"""

import math


def rot_axes(pitch, yaw, roll):
    """Forward, right and up axes of an Unreal rotator (FRotationMatrix X, Y, Z)."""
    p, y, r = math.radians(pitch), math.radians(yaw), math.radians(roll)
    sp, cp, sy, cy, sr, cr = math.sin(p), math.cos(p), math.sin(y), math.cos(y), math.sin(r), math.cos(r)
    fwd = (cp * cy, cp * sy, sp)
    right = (sr * sp * cy - cr * sy, sr * sp * sy + cr * cy, -sr * cp)
    up = (-(cr * sp * cy + sr * sy), cy * sr - cr * sp * sy, cr * cp)
    return fwd, right, up


def dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def focal_px(width, fov):
    return (width / 2) / math.tan(math.radians(fov / 2))


def project(points, cam, width, height, fov):
    fwd, right, up = rot_axes(cam[3], cam[4], cam[5])
    f = focal_px(width, fov)
    out = []
    for v in points:
        d = (v[0] - cam[0], v[1] - cam[1], v[2] - cam[2])
        x, y, z = dot(d, fwd), dot(d, right), dot(d, up)
        out.append((width / 2 + f * y / x, height / 2 - f * z / x, x))
    return out


def look_at_cam(target, yaw_deg, pitch_deg, dist):
    """Camera on a sphere around target, looking at it. yaw 0 = in front of a face looking along +Y; negative yaw = the
    character's right side (-X)."""
    y, p = math.radians(yaw_deg), math.radians(pitch_deg)
    loc = (target[0] + dist * math.cos(p) * math.sin(y), target[1] + dist * math.cos(p) * math.cos(y), target[2] + dist * math.sin(p))
    d = (target[0] - loc[0], target[1] - loc[1], target[2] - loc[2])
    return [loc[0], loc[1], loc[2], math.degrees(math.atan2(d[2], math.hypot(d[0], d[1]))), math.degrees(math.atan2(d[1], d[0])), 0.0]


def _solve(a, b):
    n = len(b)
    m = [row[:] + [b[i]] for i, row in enumerate(a)]
    for c in range(n):
        piv = max(range(c, n), key=lambda r: abs(m[r][c]))
        m[c], m[piv] = m[piv], m[c]
        if abs(m[c][c]) < 1e-12:
            continue
        for r in range(n):
            if r != c:
                fct = m[r][c] / m[c][c]
                for k in range(c, n + 1):
                    m[r][k] -= fct * m[c][k]
    return [m[i][n] / m[i][i] if abs(m[i][i]) > 1e-12 else 0.0 for i in range(n)]


def _residuals(cam, points, targets, weights, width, height, fov):
    out = []
    for p, t, w in zip(project(points, cam, width, height, fov), targets, weights):
        out += [w * (p[0] - t[0]), w * (p[1] - t[1])]
    return out


def fit_camera(cam, points, targets, weights, width, height, fov, iters=60):
    """Levenberg-Marquardt over the 6 camera parameters. Returns (camera, weighted RMS reprojection error in px)."""
    cam = list(cam)
    lam, step = 1e-2, 0.05
    r = _residuals(cam, points, targets, weights, width, height, fov)
    cost = sum(x * x for x in r)
    for _ in range(iters):
        jac = []
        for k in range(6):
            c2 = cam[:]
            c2[k] += step
            r2 = _residuals(c2, points, targets, weights, width, height, fov)
            jac.append([(a - b) / step for a, b in zip(r2, r)])
        jtj = [[sum(jac[i][m] * jac[j][m] for m in range(len(r))) for j in range(6)] for i in range(6)]
        jtr = [sum(jac[i][m] * r[m] for m in range(len(r))) for i in range(6)]
        improved, dx = False, [0.0] * 6
        for _try in range(10):
            a = [[jtj[i][j] + (lam * jtj[i][i] if i == j else 0.0) for j in range(6)] for i in range(6)]
            dx = _solve(a, [-v for v in jtr])
            c2 = [p + d for p, d in zip(cam, dx)]
            r2 = _residuals(c2, points, targets, weights, width, height, fov)
            cost2 = sum(x * x for x in r2)
            if cost2 < cost:
                cam, r, cost, lam = c2, r2, cost2, max(lam / 3, 1e-6)
                improved = True
                break
            lam *= 4
        if not improved or max(abs(v) for v in dx) < 1e-5:
            break
    return cam, math.sqrt(cost / max(1, len(targets)))


def mirror_pairs(landmarks, midline_cm=0.6):
    """Index of the left/right counterpart of every landmark (None for landmarks on the midline x ~ 0)."""
    pairs = {}
    for i, a in enumerate(landmarks):
        if abs(a[0]) < midline_cm:
            pairs[i] = None
            continue
        pairs[i] = min((j for j in range(len(landmarks)) if j != i),
                       key=lambda j: (landmarks[j][0] + a[0]) ** 2 + (landmarks[j][1] - a[1]) ** 2 + (landmarks[j][2] - a[2]) ** 2)
    return pairs


def curve_extremes(curves):
    """Eye and mouth corners (image space) from MetaHuman contour-tracker curves: leftmost/rightmost point per feature."""
    out = {}
    for side in ("l", "r"):
        pts = curves.get(f"crv_eyelid_upper_{side}", []) + curves.get(f"crv_eyelid_lower_{side}", [])
        if pts:
            out[f"eye_{side}_minx"] = min(pts, key=lambda p: p[0])
            out[f"eye_{side}_maxx"] = max(pts, key=lambda p: p[0])
    lips = [p for k, v in curves.items() if k.startswith("crv_lip_") and "outer" in k for p in v]
    if lips:
        out["mouth_minx"] = min(lips, key=lambda p: p[0])
        out["mouth_maxx"] = max(lips, key=lambda p: p[0])
    return out


def curve_offset(landmark_px, rendered, reference, radius=25.0, nearest=3):
    """Mean (dx, dy) from the rendered contour points nearest to a projected landmark to the same-index reference points.
    rendered/reference: {curve name: [[x, y], ...]} with equal point counts per curve (the tracker's fixed layout)."""
    cands = []
    for name, pts in rendered.items():
        ref = reference.get(name)
        if not ref or len(ref) != len(pts):
            continue
        for k, p in enumerate(pts):
            d = math.hypot(p[0] - landmark_px[0], p[1] - landmark_px[1])
            cands.append((d, ref[k][0] - p[0], ref[k][1] - p[1]))
    cands.sort()
    near = [c for c in cands[:nearest] if c[0] < radius]
    if not near:
        return None
    return sum(c[1] for c in near) / len(near), sum(c[2] for c in near) / len(near)


def image_to_world_deltas(deltas_px, depths, cam, width, fov, alpha):
    """2D pixel residuals -> 3D displacements in the camera image plane (cm), scaled by alpha."""
    _, right, up = rot_axes(cam[3], cam[4], cam[5])
    f = focal_px(width, fov)
    out = {}
    for i, (dx, dy) in deltas_px.items():
        s = depths[i] / f * alpha
        out[i] = tuple(s * (dx * right[k] - dy * up[k]) for k in range(3))
    return out


def symmetric_deltas(raw, pairs, own_share=0.3):
    """Remove the mean (a global shift is ambiguous with the camera), zero midline x, and blend each side with the mirrored
    other side (own_share keeps some asymmetry); a one-sided delta is mirrored to the other side."""
    if not raw:
        return {}
    mean = tuple(sum(d[k] for d in raw.values()) / len(raw) for k in range(3))
    raw = {i: tuple(d[k] - mean[k] for k in range(3)) for i, d in raw.items()}
    out = {}
    for i, d in raw.items():
        j = pairs.get(i)
        if j is None:
            out[i] = (0.0, d[1], d[2])
        elif j in raw:
            m = (-raw[j][0], raw[j][1], raw[j][2])
            out[i] = tuple((1 - own_share) * (d[k] + m[k]) / 2 + own_share * d[k] for k in range(3))
        else:
            out[i] = d
            out[j] = (-d[0], d[1], d[2])
    return out
