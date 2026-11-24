"""Road meshes of the placeholder world: carriageways that follow the terrain profile, the kerbs and pavements of the main street,
painted lines, crossings and stop lines. Side roads end at the edge of the road they join (no overlapping surfaces)."""

import math

import unreal

import layout
import meshkit
import ue_kit as k

PRIORITY = ["hwy", "oak", "mill", "elm", "birch", "farm", "pine"]
SURFACE = {"highway": "asphalt", "street": "asphalt_old", "gravel": "gravel", "track": "track"}   # track: M_SpnTrack (materials.track_master, 2026-11-14)
MAIN_HALF = 560.0
KERB = 15.0
WALK = 290.0


def eff_half(rd, s, main):
    """Half width of the surface at s: the highway widens to parking lanes inside the town."""
    if rd.name != "hwy":
        return rd.half
    t = min(max((s - (main[0] - 1500.0)) / 1500.0, 0.0), 1.0) * min(max(((main[1] + 1500.0) - s) / 1500.0, 0.0), 1.0)
    t = t * t * (3 - 2 * t)
    return rd.half + (MAIN_HALF - rd.half) * t


def samples(rd, s0=None, s1=None, step=300.0):
    s0 = 0.0 if s0 is None else s0
    s1 = rd.length if s1 is None else s1
    count = max(1, int(math.ceil((s1 - s0) / step)))
    out = []
    for i in range(count + 1):
        s = s0 + (s1 - s0) * i / count
        x, y, z, tx, ty = rd.at(s)
        out.append((x, y, z, -ty, tx, s))
    return out


def strip(mesh, key, run, o0, o1, z0, z1, hint_up=1.0, hint_side=0.0, u0=None, u1=None, col=meshkit.WHITE):
    """Ribbon between two lateral offsets (functions of the sample or numbers) with height offsets above the road profile."""
    f = lambda v, smp: v(smp) if callable(v) else v
    for a, b in zip(run, run[1:]):
        pts, uvs = [], []
        for smp, order in ((a, (o0, z0, o1, z1)), (b, (o1, z1, o0, z0))):
            for off, dz in ((order[0], order[1]), (order[2], order[3])):
                o = f(off, smp)
                pts.append((smp[0] + smp[3] * o, smp[1] + smp[4] * o, smp[2] + f(dz, smp)))
                uvs.append((o, smp[5]))
        if u0 is not None:
            uvs = [(u0, a[5]), (u1, a[5]), (u1, b[5]), (u0, b[5])]
        mesh.quad(key, pts[0], pts[1], pts[2], pts[3], None, uvs, col, hint=(a[3] * hint_side, a[4] * hint_side, hint_up))


def covered_ranges(rd, roads, main):
    """Marks samples that lie on a road of higher priority; returns the runs of free samples (with one overlapping sample at each end)."""
    higher = [roads[n] for n in PRIORITY[:PRIORITY.index(rd.name)]]
    all_samples = samples(rd)
    free = []
    for smp in all_samples:
        inside = False
        for other in higher:
            s_other, dist = other.nearest_s(smp[0], smp[1])
            if dist < eff_half(other, s_other, main) - 25.0:
                inside = True
                break
        free.append(not inside)
    runs, current = [], []
    for index, smp in enumerate(all_samples):
        if free[index]:
            if not current and index > 0:
                current.append(all_samples[index - 1])
            current.append(smp)
        elif current:
            current.append(smp)
            runs.append(current)
            current = []
    if len(current) > 1:
        runs.append(current)
    return runs


def junctions_on(rd, roads, main):
    """Where other roads meet this one: [(s on rd, side +1 left / -1 right / 0 both, half width of the other road)]."""
    out = []
    for other in roads.values():
        if other is rd:
            continue
        hits = []
        for end in (0, -1):
            s, dist = rd.nearest_s(*other.pts[end])
            if dist < eff_half(rd, s, main) + 100.0 and 200.0 < s < rd.length - 200.0:
                inner = other.pts[min(12, len(other.pts) - 1)] if end == 0 else other.pts[max(-13, -len(other.pts))]
                x, y, _, tx, ty = rd.at(s)
                hits.append((s, 1 if (tx * (inner[1] - y) - ty * (inner[0] - x)) > 0 else -1))
        if not hits:   # crossing in the middle of the other road
            for i in range(0, len(other.pts), 2):
                s, dist = rd.nearest_s(*other.pts[i])
                if dist < 150.0 and 200.0 < s < rd.length - 200.0:
                    hits.append((s, 0))
                    break
        out.extend((s, side, other.half) for s, side in hits)
    return out


def build(ctx):
    roads, terrain, mats = ctx["roads"], ctx["terrain"], ctx["mats"]
    main = layout.main_street(roads)
    detail = ctx["has"]("C")
    marks = meshkit.Mesh()

    for name in PRIORITY:
        rd = roads[name]
        mesh = meshkit.Mesh()
        key = SURFACE[rd.kind]
        half = lambda smp, rd=rd: eff_half(rd, smp[5], main)
        crown = lambda smp, rd=rd: 3.0 - eff_half(rd, smp[5], main) * 0.012
        for run in covered_ranges(rd, roads, main):
            strip(mesh, key, run, lambda s: -half(s), 0.0, crown, 3.0)
            strip(mesh, key, run, 0.0, half, 3.0, crown)
            for sign in (-1.0, 1.0):   # edge skirts into the ground
                strip(mesh, key, run, lambda s, sign=sign: sign * half(s), lambda s, sign=sign: sign * (half(s) + 45.0), crown, lambda s: crown(s) - 40.0, hint_up=0.6, hint_side=sign)
            if rd.kind == "track":
                continue
        if mesh.triangle_count():
            asset = k.static_mesh(mesh, "SM_Road_" + name, mats)
            k.place(asset, label="SPN_Road_" + name, folder="Roads")

    # ---- kerbs and pavements of the main street
    hwy = roads["hwy"]
    gaps = junctions_on(hwy, roads, main)
    walk = meshkit.Mesh()
    for side in (-1.0, 1.0):
        cuts = sorted((s - other_half - 380.0, s + other_half + 380.0) for s, js, other_half in gaps if js == 0 or js == side)
        cursor = main[0]
        ranges = []
        for a, b in cuts:
            if b < main[0] or a > main[1]:
                continue
            if a > cursor + 300.0:
                ranges.append((cursor, a))
            cursor = max(cursor, b)
        if cursor < main[1] - 300.0:
            ranges.append((cursor, main[1]))
        for a, b in ranges:
            run = samples(hwy, a, b, 250.0)
            h = lambda smp: eff_half(hwy, smp[5], main)
            edge = lambda smp, o=0.0: side * (h(smp) + o)
            base = lambda smp: 3.0 - h(smp) * 0.012
            strip(walk, "kerb", run, lambda s: edge(s), lambda s: edge(s, 1.0), base, lambda s: base(s) + KERB, hint_up=0.2, hint_side=-side)
            strip(walk, "kerb", run, lambda s: edge(s, 1.0), lambda s: edge(s, 16.0), lambda s: base(s) + KERB, lambda s: base(s) + KERB + 0.3)
            strip(walk, "sidewalk", run, lambda s: edge(s, 16.0), lambda s: edge(s, WALK), lambda s: base(s) + KERB + 0.3, lambda s: base(s) + KERB + 4.0)
            strip(walk, "concrete", run, lambda s: edge(s, WALK), lambda s: edge(s, WALK + 30.0), lambda s: base(s) + KERB + 4.0, lambda s: base(s) - 60.0, hint_up=0.5, hint_side=side)
            for smp in (run[0], run[-1]):   # sloped ends instead of a step
                direction = -1.0 if smp is run[0] else 1.0
                tx, ty = smp[4], -smp[3]
                p = lambda o, dz, along=0.0: (smp[0] + smp[3] * side * (h(smp) + o) + tx * along * direction, smp[1] + smp[4] * side * (h(smp) + o) + ty * along * direction, smp[2] + base(smp) + dz)
                walk.quad("sidewalk", p(0.0, KERB), p(WALK, KERB + 4.0), p(WALK, -2.0, 160.0), p(0.0, -2.0, 160.0), None, None, hint=(tx * direction, ty * direction, 1.0))
    if walk.triangle_count():
        k.place(k.static_mesh(walk, "SM_MainStreet_Pavement", mats), label="SPN_MainStreet_Pavement", folder="Roads")
    ctx["pavement_gaps"] = gaps

    # ---- painted lines
    lift = lambda smp: 3.9 - abs(0.0) * 0.012
    oak_cross = [s for s, js, _ in gaps if js == 0]
    def line(rd, s0, s1, offset, width, key, dash=None):
        if s1 - s0 < 100.0:
            return
        if dash:
            s = s0
            while s < s1:
                line(rd, s, min(s + dash[0], s1), offset, width, key)
                s += dash[0] + dash[1]
            return
        run = samples(rd, s0, s1, 300.0)
        z = lambda smp: 4.0 - abs(offset) * 0.012
        strip(marks, key, run, offset - width / 2, offset + width / 2, z, z)

    segments, cursor = [], 0.0
    for s in sorted(oak_cross):
        segments.append((cursor, s - 900.0))
        cursor = s + 900.0
    segments.append((cursor, hwy.length))
    for a, b in segments:
        line(hwy, a, b, -11.0, 10.0, "mark_yellow")
        line(hwy, a, b, 11.0, 10.0, "mark_yellow")
        for side in (-1.0, 1.0):
            line(hwy, a, min(b, main[0] - 1500.0), side * (hwy.half - 22.0), 11.0, "mark_white")
            line(hwy, max(a, main[1] + 1500.0), b, side * (hwy.half - 22.0), 11.0, "mark_white")
            line(hwy, max(a, main[0]), min(b, main[1]), side * 345.0, 10.0, "mark_white", dash=(300.0, 450.0))
    # zebra crossings beside the cross street, stop lines on the side roads
    for s in oak_cross:
        for offset in (-760.0, 760.0):
            stripe = -MAIN_HALF + 60.0
            while stripe < MAIN_HALF - 60.0:
                run = samples(hwy, s + offset - 150.0, s + offset + 150.0, 300.0)
                strip(marks, "mark_white", run, stripe, stripe + 45.0, 4.0 - abs(stripe) * 0.012, 4.0 - abs(stripe + 45.0) * 0.012)
                stripe += 95.0
    for name in PRIORITY[1:]:
        rd = roads[name]
        if rd.kind != "street":
            continue
        for run in covered_ranges(rd, roads, main):
            for smp, direction in ((run[0], 1.0), (run[-1], -1.0)):
                if smp[5] < 50.0 or smp[5] > rd.length - 50.0:
                    continue   # a free end, not a junction
                s = smp[5] + direction * 420.0
                near = samples(rd, s, s + direction * 40.0, 40.0)
                lane = (-rd.half + 20.0, -10.0) if direction < 0 else (10.0, rd.half - 20.0)
                strip(marks, "mark_white", sorted(near, key=lambda q: q[5]), lane[0], lane[1], 4.0, 4.0)
    if marks.triangle_count():
        k.place(k.static_mesh(marks, "SM_Road_Markings", mats, nanite=False, collision=False), label="SPN_Road_Markings", folder="Roads", collide=False, shadow=False)
