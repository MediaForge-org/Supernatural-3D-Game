"""Tiny mesh builder for the world generator (pure Python, no Unreal import).

A Mesh is a set of parts, one per material key. Every face gets its own vertices (hard edges), a normal, UVs in centimetres (materials
divide by their tile size) and a vertex colour: R = occlusion multiplier, G = ground grime, B = random per element, A = 1.
"""

import math

WHITE = (1.0, 1.0, 1.0, 1.0)
WIND = 1          # set by the Unreal side after testing which triangle order faces outwards


class Part:
    def __init__(self):
        self.v, self.n, self.uv, self.c, self.t = [], [], [], [], []


def _sub(a, b):
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2])


def _cross(a, b):
    return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


def _dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def _norm(a):
    length = math.sqrt(_dot(a, a)) or 1.0
    return (a[0] / length, a[1] / length, a[2] / length)


class Mesh:
    def __init__(self):
        self.parts = {}

    def part(self, key):
        if key not in self.parts:
            self.parts[key] = Part()
        return self.parts[key]

    def triangle_count(self):
        return sum(len(p.t) for p in self.parts.values())

    def poly(self, key, pts, normal=None, uvs=None, col=WHITE, cols=None, hint=None):
        """Convex polygon (3+ points around its perimeter). normal = outside direction; the winding is fixed to match it.
        hint: rough outside direction; the true face normal is used, turned to the side the hint points to."""
        geo = _cross(_sub(pts[1], pts[0]), _sub(pts[-1], pts[0]))
        if normal is None:
            normal = _norm(geo)
            if hint is not None and _dot(normal, hint) < 0:
                normal = (-normal[0], -normal[1], -normal[2])
        order = list(range(len(pts)))
        if _dot(geo, normal) < 0:
            order.reverse()
        if WIND < 0:
            order.reverse()
        p = self.part(key)
        base = len(p.v)
        for i in order:
            p.v.append(pts[i])
            p.n.append(normal)
            p.uv.append(uvs[i] if uvs else (pts[i][0], pts[i][1]))
            p.c.append(cols[i] if cols else col)
        for i in range(1, len(pts) - 1):
            p.t.append((base, base + i, base + i + 1))

    def quad(self, key, a, b, c, d, normal=None, uvs=None, col=WHITE, cols=None, hint=None):
        self.poly(key, [a, b, c, d], normal, uvs, col, cols, hint)

    def box(self, key, x0, x1, y0, y1, z0, z1, col=WHITE, skip="", top_key=None, uv_off=(0.0, 0.0)):
        """Axis-aligned box. skip: letters of faces to leave out (x X y Y z Z = min/max of an axis)."""
        u, w = uv_off
        if "x" not in skip:
            self.quad(key, (x0, y0, z0), (x0, y1, z0), (x0, y1, z1), (x0, y0, z1), (-1, 0, 0), [(y0 + u, -z0 + w), (y1 + u, -z0 + w), (y1 + u, -z1 + w), (y0 + u, -z1 + w)], col)
        if "X" not in skip:
            self.quad(key, (x1, y0, z0), (x1, y1, z0), (x1, y1, z1), (x1, y0, z1), (1, 0, 0), [(y0 + u, -z0 + w), (y1 + u, -z0 + w), (y1 + u, -z1 + w), (y0 + u, -z1 + w)], col)
        if "y" not in skip:
            self.quad(key, (x0, y0, z0), (x1, y0, z0), (x1, y0, z1), (x0, y0, z1), (0, -1, 0), [(x0 + u, -z0 + w), (x1 + u, -z0 + w), (x1 + u, -z1 + w), (x0 + u, -z1 + w)], col)
        if "Y" not in skip:
            self.quad(key, (x0, y1, z0), (x1, y1, z0), (x1, y1, z1), (x0, y1, z1), (0, 1, 0), [(x0 + u, -z0 + w), (x1 + u, -z0 + w), (x1 + u, -z1 + w), (x0 + u, -z1 + w)], col)
        if "z" not in skip:
            self.quad(key, (x0, y0, z0), (x1, y0, z0), (x1, y1, z0), (x0, y1, z0), (0, 0, -1), [(x0, y0), (x1, y0), (x1, y1), (x0, y1)], col)
        if "Z" not in skip:
            self.quad(top_key or key, (x0, y0, z1), (x1, y0, z1), (x1, y1, z1), (x0, y1, z1), (0, 0, 1), [(x0, y0), (x1, y0), (x1, y1), (x0, y1)], col)

    def cyl(self, key, x, y, z0, z1, r0, r1=None, segs=12, col=WHITE, caps=True):
        """Vertical cylinder or cone frustum (smooth sides)."""
        r1 = r0 if r1 is None else r1
        p = self.part(key)
        height = z1 - z0
        slope = (r0 - r1) / height if height else 0.0
        for i in range(segs):
            a0, a1 = 2 * math.pi * i / segs, 2 * math.pi * (i + 1) / segs
            pts = [(x + math.cos(a0) * r0, y + math.sin(a0) * r0, z0), (x + math.cos(a1) * r0, y + math.sin(a1) * r0, z0),
                   (x + math.cos(a1) * r1, y + math.sin(a1) * r1, z1), (x + math.cos(a0) * r1, y + math.sin(a0) * r1, z1)]
            ns = [_norm((math.cos(a), math.sin(a), slope)) for a in (a0, a1, a1, a0)]
            circ = 2 * math.pi * max(r0, r1)
            uvs = [(circ * i / segs, -z0), (circ * (i + 1) / segs, -z0), (circ * (i + 1) / segs, -z1), (circ * i / segs, -z1)]
            mid = _norm((math.cos((a0 + a1) / 2), math.sin((a0 + a1) / 2), slope))
            geo = _cross(_sub(pts[1], pts[0]), _sub(pts[3], pts[0]))
            order = [0, 1, 2, 3]
            if _dot(geo, mid) < 0:
                order.reverse()
            if WIND < 0:
                order.reverse()
            base = len(p.v)
            for k in order:
                p.v.append(pts[k]); p.n.append(ns[k]); p.uv.append(uvs[k]); p.c.append(col)
            p.t.append((base, base + 1, base + 2)); p.t.append((base, base + 2, base + 3))
        if caps:
            ring = lambda r, z: [(x + math.cos(2 * math.pi * i / segs) * r, y + math.sin(2 * math.pi * i / segs) * r, z) for i in range(segs)]
            if r1 > 0:
                self.poly(key, ring(r1, z1), (0, 0, 1), None, col)
            if r0 > 0:
                self.poly(key, ring(r0, z0), (0, 0, -1), None, col)

    def beam(self, key, a, b, width, height=None, col=WHITE):
        """Box-section beam from point a to point b (any direction)."""
        height = width if height is None else height
        axis = _norm(_sub(b, a))
        ref = (0, 0, 1) if abs(axis[2]) < 0.95 else (1, 0, 0)
        side = _norm(_cross(axis, ref))
        up = _norm(_cross(side, axis))
        hw, hh = width / 2, height / 2
        length = math.sqrt(_dot(_sub(b, a), _sub(b, a)))
        corner = lambda p, s, u: (p[0] + side[0] * s * hw + up[0] * u * hh, p[1] + side[1] * s * hw + up[1] * u * hh, p[2] + side[2] * s * hw + up[2] * u * hh)
        for (s0, u0, s1, u1, n) in ((-1, -1, 1, -1, tuple(-c for c in up)), (1, -1, 1, 1, side), (1, 1, -1, 1, up), (-1, 1, -1, -1, tuple(-c for c in side))):
            self.quad(key, corner(a, s0, u0), corner(a, s1, u1), corner(b, s1, u1), corner(b, s0, u0), n, [(0, 0), (width, 0), (width, length), (0, length)], col)
        self.quad(key, corner(a, -1, -1), corner(a, 1, -1), corner(a, 1, 1), corner(a, -1, 1), tuple(-c for c in axis), None, col)
        self.quad(key, corner(b, -1, -1), corner(b, 1, -1), corner(b, 1, 1), corner(b, -1, 1), axis, None, col)

    def add(self, other, x=0.0, y=0.0, z=0.0, yaw=0.0, rename=None):
        """Merges another mesh, rotated about Z (degrees) and moved."""
        c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        for key, src in other.parts.items():
            dst = self.part(rename.get(key, key) if rename else key)
            base = len(dst.v)
            dst.v.extend((x + v[0] * c - v[1] * s, y + v[0] * s + v[1] * c, z + v[2]) for v in src.v)
            dst.n.extend((n[0] * c - n[1] * s, n[0] * s + n[1] * c, n[2]) for n in src.n)
            dst.uv.extend(src.uv)
            dst.c.extend(src.c)
            dst.t.extend((a + base, b + base, cc + base) for a, b, cc in src.t)
