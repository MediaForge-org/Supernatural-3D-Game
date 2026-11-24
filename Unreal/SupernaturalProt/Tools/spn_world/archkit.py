"""Architectural building blocks on top of meshkit (pure Python): walls with openings, windows, doors, gable roofs, steps, railings.

Local building frame: origin at the centre of the front edge on the ground, +X along the front, +Y into the lot, +Z up; centimetres.
A Frame describes one wall line: s runs along the wall, t points outwards.
"""

import math

from meshkit import WHITE


class Frame:
    def __init__(self, ox, oy, dx, dy, nx, ny, oz=0.0):
        length = math.hypot(dx, dy) or 1.0
        self.o = (ox, oy, oz)
        self.d = (dx / length, dy / length)
        self.n = (nx, ny)

    def p(self, s, t, z):
        return (self.o[0] + self.d[0] * s + self.n[0] * t, self.o[1] + self.d[1] * s + self.n[1] * t, self.o[2] + z)

    def vec(self, s, t, z):
        return (self.d[0] * s + self.n[0] * t, self.d[1] * s + self.n[1] * t, z)

    def box(self, mesh, key, s0, s1, t0, t1, z0, z1, col=WHITE, skip=""):
        """Box in wall coordinates. skip letters: s S t T z Z (min/max faces)."""
        p = self.p
        faces = (("s", [(s0, t0, z0), (s0, t1, z0), (s0, t1, z1), (s0, t0, z1)], (-1, 0, 0), 1, 2), ("S", [(s1, t0, z0), (s1, t1, z0), (s1, t1, z1), (s1, t0, z1)], (1, 0, 0), 1, 2),
                 ("t", [(s0, t0, z0), (s1, t0, z0), (s1, t0, z1), (s0, t0, z1)], (0, -1, 0), 0, 2), ("T", [(s0, t1, z0), (s1, t1, z0), (s1, t1, z1), (s0, t1, z1)], (0, 1, 0), 0, 2),
                 ("z", [(s0, t0, z0), (s1, t0, z0), (s1, t1, z0), (s0, t1, z0)], (0, 0, -1), 0, 1), ("Z", [(s0, t0, z1), (s1, t0, z1), (s1, t1, z1), (s0, t1, z1)], (0, 0, 1), 0, 1))
        for letter, pts, normal, ua, va in faces:
            if letter in skip:
                continue
            uvs = [(q[ua], -q[va] if va == 2 else q[va]) for q in pts]
            mesh.quad(key, *[p(*q) for q in pts], self.vec(*normal), uvs, col)


def wall(mesh, key, frame, length, z0, z1, openings=(), thick=20.0, inner_key=None, reveal_key=None, u0=0.0, grime=True, reveal=None, base_z=None):
    """Wall face along the frame (outer face at t = 0) with rectangular openings [(s0, s1, zb, zt)]; optional inner face and reveals."""
    base_z = z0 if base_z is None else base_z
    cuts_s = sorted(set([0.0, length] + [v for o in openings for v in (o[0], o[1])]))
    cuts_z = sorted(set([z0, z1] + [v for o in openings for v in (o[2], o[3])] + ([base_z + 90.0] if grime and z0 < base_z + 90.0 < z1 else [])))
    def colour(z):
        g = max(0.0, 1.0 - (z - base_z) / 90.0) if grime else 0.0
        return (1.0, g, 0.5, 1.0)
    for si in range(len(cuts_s) - 1):
        for zi in range(len(cuts_z) - 1):
            sa, sb, za, zb = cuts_s[si], cuts_s[si + 1], cuts_z[zi], cuts_z[zi + 1]
            cs, cz = (sa + sb) / 2, (za + zb) / 2
            if any(o[0] < cs < o[1] and o[2] < cz < o[3] for o in openings):
                continue
            cols = [colour(za), colour(za), colour(zb), colour(zb)]
            mesh.quad(key, frame.p(sa, 0, za), frame.p(sb, 0, za), frame.p(sb, 0, zb), frame.p(sa, 0, zb), frame.vec(0, 1, 0), [(u0 + sa, -za), (u0 + sb, -za), (u0 + sb, -zb), (u0 + sa, -zb)], cols=cols)
            if inner_key:
                mesh.quad(inner_key, frame.p(sa, -thick, za), frame.p(sb, -thick, za), frame.p(sb, -thick, zb), frame.p(sa, -thick, zb), frame.vec(0, -1, 0), [(u0 + sa, -za), (u0 + sb, -za), (u0 + sb, -zb), (u0 + sa, -zb)])
    depth = thick if reveal is None else reveal
    for s0, s1, zb, zt in openings:
        rk = reveal_key or key
        mesh.quad(rk, frame.p(s0, 0, zb), frame.p(s0, -depth, zb), frame.p(s0, -depth, zt), frame.p(s0, 0, zt), frame.vec(1, 0, 0))
        mesh.quad(rk, frame.p(s1, 0, zb), frame.p(s1, -depth, zb), frame.p(s1, -depth, zt), frame.p(s1, 0, zt), frame.vec(-1, 0, 0))
        mesh.quad(rk, frame.p(s0, 0, zt), frame.p(s1, 0, zt), frame.p(s1, -depth, zt), frame.p(s0, -depth, zt), frame.vec(0, 0, -1))
        if zb > z0 + 1.0:
            mesh.quad(rk, frame.p(s0, 0, zb), frame.p(s1, 0, zb), frame.p(s1, -depth, zb), frame.p(s0, -depth, zb), frame.vec(0, 0, 1))


def window(mesh, frame, s0, s1, zb, zt, rnd, glass="window", trim="trim", casing=9.0, sill=True, mullions=(1, 1), inset=7.0, shutters=None):
    """Casing around an opening, sash bars and the pane (UV 0..1 across the pane, vertex colour B = rnd for the night glow)."""
    if casing > 0:
        frame.box(mesh, trim, s0 - casing, s0, 0, 2.5, zb - casing * 0.4, zt + casing)
        frame.box(mesh, trim, s1, s1 + casing, 0, 2.5, zb - casing * 0.4, zt + casing)
        frame.box(mesh, trim, s0, s1, 0, 3.5, zt, zt + casing + 2.0)
    if sill:
        frame.box(mesh, trim, s0 - casing - 3.0, s1 + casing + 3.0, 0, 6.5, zb - 5.0, zb)
    bar = 4.5
    frame.box(mesh, trim, s0, s0 + bar, -inset, -inset + 4.0, zb, zt, skip="t")
    frame.box(mesh, trim, s1 - bar, s1, -inset, -inset + 4.0, zb, zt, skip="t")
    frame.box(mesh, trim, s0 + bar, s1 - bar, -inset, -inset + 4.0, zb, zb + bar, skip="t")
    frame.box(mesh, trim, s0 + bar, s1 - bar, -inset, -inset + 4.0, zt - bar, zt, skip="t")
    for i in range(1, mullions[0] + 1):
        s = s0 + (s1 - s0) * i / (mullions[0] + 1)
        frame.box(mesh, trim, s - 1.6, s + 1.6, -inset, -inset + 3.0, zb + bar, zt - bar, skip="tzZ")
    for i in range(1, mullions[1] + 1):
        z = zb + (zt - zb) * i / (mullions[1] + 1)
        frame.box(mesh, trim, s0 + bar, s1 - bar, -inset, -inset + 3.4, z - 2.0, z + 2.0, skip="tsS")
    col = (1.0, 0.0, rnd, 1.0)
    mesh.quad(glass, frame.p(s0, -inset, zb), frame.p(s1, -inset, zb), frame.p(s1, -inset, zt), frame.p(s0, -inset, zt), frame.vec(0, 1, 0), [(0, 1), (1, 1), (1, 0), (0, 0)], col)
    if glass == "glass":
        mesh.quad(glass, frame.p(s0, -inset - 1.0, zb), frame.p(s1, -inset - 1.0, zb), frame.p(s1, -inset - 1.0, zt), frame.p(s0, -inset - 1.0, zt), frame.vec(0, -1, 0), [(0, 1), (1, 1), (1, 0), (0, 0)], col)
    if shutters:
        width = (s1 - s0) * 0.5
        for a, b in ((s0 - casing - width, s0 - casing), (s1 + casing, s1 + casing + width)):
            frame.box(mesh, shutters, a + 2.0, b - 2.0, 0, 3.0, zb, zt)


def door(mesh, frame, s0, s1, zb, zt, key="planks_dark", trim="trim", inset=8.0, glazed=False, rnd=0.5, casing=9.0):
    """A closed door leaf as geometry (buildings without an interior).

    Every such door is also recorded on the mesh (mesh.door_marks: leaf centre, half width, height, outward normal, all in mesh
    coordinates) so the game can answer an attempt to open it (developer decision 2026-11-14: feedback on interaction, no permanent marker)."""
    marks = getattr(mesh, "door_marks", None)
    if marks is None:
        marks = mesh.door_marks = []
    marks.append({"centre": frame.p((s0 + s1) / 2, -inset, (zb + zt) / 2), "half_width": (s1 - s0) / 2, "height": zt - zb, "normal": frame.n})
    if casing > 0:
        frame.box(mesh, trim, s0 - casing, s0, 0, 2.5, zb, zt + casing)
        frame.box(mesh, trim, s1, s1 + casing, 0, 2.5, zb, zt + casing)
        frame.box(mesh, trim, s0, s1, 0, 3.5, zt, zt + casing + 2.0)
    frame.box(mesh, key, s0, s1, -inset - 4.0, -inset, zb, zt, skip="t")
    if glazed:
        m = (s1 - s0) * 0.16
        frame.box(mesh, trim, s0 + m - 3, s1 - m + 3, -inset, -inset + 1.5, zb + (zt - zb) * 0.5 - 3, zt - m + 3, skip="t")
        mesh.quad("window", frame.p(s0 + m, -inset + 1.7, zb + (zt - zb) * 0.5), frame.p(s1 - m, -inset + 1.7, zb + (zt - zb) * 0.5), frame.p(s1 - m, -inset + 1.7, zt - m), frame.p(s0 + m, -inset + 1.7, zt - m),
                  frame.vec(0, 1, 0), [(0.4, 1), (0.6, 1), (0.6, 0), (0.4, 0)], (1.0, 0.0, rnd, 1.0))
    else:
        for za, zc in ((zb + 18, zb + (zt - zb) * 0.42), (zb + (zt - zb) * 0.48, zt - 18)):
            frame.box(mesh, key, s0 + 14, s1 - 14, -inset, -inset + 1.5, za, zc, skip="t")
    frame.box(mesh, "steel", s1 - 13, s1 - 7, -inset, -inset + 6.0, zb + 96, zb + 102, skip="t")


def gable_roof(mesh, a0, a1, b0, b1, zt, pitch, eave=45.0, rake=30.0, key="shingles", trim="trim", axis="x", thick=14.0, half=False):
    """Gable roof over the rectangle; the ridge runs along `axis` (a = along the ridge, b = across). Returns the ridge height.
    half = True: a single slope rising from b0 to b1 (shed / lean-to roof)."""
    def P(a, b, z):
        return (a, b, z) if axis == "x" else (b, a, z)
    def V(a, b, z):
        return (a, b, z) if axis == "x" else (b, a, z)
    mid = b1 if half else (b0 + b1) / 2
    rise = (mid - b0) * pitch
    zr = zt + rise
    ze = zt - eave * pitch
    slope_len = math.hypot(mid - b0 + eave, rise + eave * pitch)
    A0, A1 = a0 - rake, a1 + rake
    sides = [(b0 - eave, mid, -1.0)] if half else [(b0 - eave, mid, -1.0), (b1 + eave, mid, 1.0)]
    for be, bm, sign in sides:
        up = V(0, sign * pitch, 1.0)
        uv = [(A0, slope_len), (A1, slope_len), (A1, 0), (A0, 0)]
        mesh.quad(key, P(A0, be, ze), P(A1, be, ze), P(A1, bm, zr), P(A0, bm, zr), None, uv, hint=up)
        mesh.quad(trim, P(A0, be, ze - thick), P(A1, be, ze - thick), P(A1, bm, zr - thick), P(A0, bm, zr - thick), None, None, (0.8, 0.0, 0.5, 1.0), hint=V(0, 0, -1.0))
        mesh.quad(trim, P(A0, be, ze - thick), P(A1, be, ze - thick), P(A1, be, ze), P(A0, be, ze), V(0, sign, 0))                 # eave fascia
        for a, asign in ((A0, -1.0), (A1, 1.0)):                                                                                  # rake boards
            mesh.quad(trim, P(a, be, ze - thick), P(a, bm, zr - thick), P(a, bm, zr), P(a, be, ze), V(asign, 0, 0))
    if half:
        mesh.quad(trim, P(A0, mid, zr - thick), P(A1, mid, zr - thick), P(A1, mid, zr), P(A0, mid, zr), V(0, 1.0, 0))
    else:   # ridge cap
        w = 12.0
        mesh.quad(trim if key == "corrugated" else key, P(A0, mid - w, zr - w * pitch + 2.5), P(A1, mid - w, zr - w * pitch + 2.5), P(A1, mid, zr + 2.5), P(A0, mid, zr + 2.5), None, None, hint=V(0, -pitch, 1.0))
        mesh.quad(trim if key == "corrugated" else key, P(A0, mid + w, zr - w * pitch + 2.5), P(A1, mid + w, zr - w * pitch + 2.5), P(A1, mid, zr + 2.5), P(A0, mid, zr + 2.5), None, None, hint=V(0, pitch, 1.0))
    return zr


def gable_end(mesh, key, a, b0, b1, zt, pitch, outward, axis="x", u0=0.0, vent=True, trim="trim"):
    """Triangular wall under a gable at ridge position a (outward = +1 / -1 along the ridge axis)."""
    def P(aa, b, z):
        return (aa, b, z) if axis == "x" else (b, aa, z)
    mid = (b0 + b1) / 2
    zr = zt + (mid - b0) * pitch
    normal = (outward, 0, 0) if axis == "x" else (0, outward, 0)
    mesh.poly(key, [P(a, b0, zt), P(a, b1, zt), P(a, mid, zr)], normal, [(u0 + b0, -zt), (u0 + b1, -zt), (u0 + mid, -zr)], (0.9, 0.0, 0.5, 1.0))
    if vent and zr - zt > 120.0:
        zc = zt + (zr - zt) * 0.42
        pts = [P(a + outward * 1.5, mid - 22, zc - 26), P(a + outward * 1.5, mid + 22, zc - 26), P(a + outward * 1.5, mid + 22, zc + 26), P(a + outward * 1.5, mid - 22, zc + 26)]
        mesh.quad("trim_dark", *pts, normal)
        pts = [P(a + outward * 1.0, mid - 27, zc - 31), P(a + outward * 1.0, mid + 27, zc - 31), P(a + outward * 1.0, mid + 27, zc + 31), P(a + outward * 1.0, mid - 27, zc + 31)]
        mesh.quad(trim, *pts, normal)


def steps(mesh, key, frame, s0, s1, top, count=None, tread=28.0, t0=0.0):
    """Steps going down outwards from the wall line (t increases away from it), ending at ground level z = 0."""
    count = count or max(1, int(round(top / 17.0)))
    rise = top / count
    for i in range(count):
        frame.box(mesh, key, s0, s1, t0 + i * tread, t0 + (i + 1) * tread, -30.0, top - i * rise - (0.0 if i == 0 else 0.0), skip="z")
    return count * tread


def railing(mesh, key, frame, s0, s1, t, z0, height=92.0, post=None):
    """Balustrade along s at offset t."""
    frame.box(mesh, key, s0, s1, t - 3.5, t + 3.5, z0 + height - 5.0, z0 + height)
    frame.box(mesh, key, s0, s1, t - 2.5, t + 2.5, z0 + 9.0, z0 + 14.0)
    count = max(1, int((s1 - s0) / 13.0))
    for i in range(1, count):
        s = s0 + (s1 - s0) * i / count
        frame.box(mesh, key, s - 1.7, s + 1.7, t - 1.7, t + 1.7, z0 + 14.0, z0 + height - 5.0, skip="zZ")
