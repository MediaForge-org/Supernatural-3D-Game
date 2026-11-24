"""Protagonist V1 feasibility phase, task 3: the Blender half of the round trip  MetaHuman head -> Blender -> MetaHuman state.

Run with Blender (no add-ons, no downloads):
    blender.exe --background --factory-startup --python Tools/protagonist/blender_sculpt_probe.py -- <mv_dir>

Reads <mv_dir>/archetype_head.obj (written by mh_multiview_experiment.py, MetaHuman conform topology), applies a SMALL, deterministic
"sculpt" that stands in for an artist's edit, and writes <mv_dir>/sculpted_head.obj with the SAME vertex order (that is the contract of
FitStateToTargetVertices). The edit is chosen to be measurable, not to look like the protagonist:
  * nose tip region pushed 4 mm forward (smooth falloff, radius 2.5 cm),
  * face 3 % narrower (scaled towards the mid plane),
Vertex count and order are checked on the way out. Writes <mv_dir>/sculpt_report.json with the axes found and the moved vertices.
"""

import json
import os
import sys

import numpy as np


def read_obj(path):
    verts, faces = [], []
    with open(path, encoding="ascii") as f:
        for line in f:
            if line.startswith("v "):
                verts.append([float(t) for t in line.split()[1:4]])
            elif line.startswith("f "):
                faces.append(line)
    return np.array(verts, dtype=np.float64), faces


def main():
    mv = sys.argv[sys.argv.index("--") + 1]
    v, faces = read_obj(os.path.join(mv, "archetype_head.obj"))
    ext = v.max(axis=0) - v.min(axis=0)
    # axes: the narrowest extent of a head is left-right? no - a head is narrower left-right than front-back only slightly; use the mesh:
    # up = largest extent among the two non-symmetric axes; left-right = the axis around which the mesh is mirror-symmetric (mean ~ 0)
    centre = v.mean(axis=0)
    sym = int(np.argmin(np.abs(centre) / (ext + 1e-9)))          # mirror plane through 0
    others = [a for a in range(3) if a != sym]
    up = others[int(np.argmax(ext[others]))]
    fwd = [a for a in others if a != up][0]
    # forward sign: the nose tip is the most extreme point near the mid plane in the forward axis
    mid = np.abs(v[:, sym]) < 0.01 * ext[sym] + 1e-6
    hi, lo = v[mid, fwd].max(), v[mid, fwd].min()
    sign = 1.0 if abs(hi - centre[fwd]) > abs(lo - centre[fwd]) else -1.0
    nose_idx = int(np.where(mid)[0][np.argmax(sign * v[mid, fwd])])
    unit = 1.0 if ext[up] > 5.0 else 100.0   # cm (UE) or m (DNA)? head height ~ 25 cm
    cm = 1.0 / unit if unit == 1.0 else 0.01
    out = v.copy()
    d = np.linalg.norm(v - v[nose_idx], axis=1)
    radius = 2.5 * (1.0 if ext[up] > 5.0 else 0.01)
    fall = np.clip(1.0 - d / radius, 0.0, 1.0) ** 2
    push = 0.4 * (1.0 if ext[up] > 5.0 else 0.01)     # 4 mm
    out[:, fwd] += sign * push * fall
    out[:, sym] = centre[sym] + (out[:, sym] - centre[sym]) * 0.97
    assert out.shape == v.shape
    with open(os.path.join(mv, "sculpted_head.obj"), "w", encoding="ascii") as f:
        for p in out:
            f.write("v %.5f %.5f %.5f\n" % tuple(p))
        f.writelines(faces)
    moved = np.linalg.norm(out - v, axis=1)
    rep = {"vertices": int(len(v)), "faces": len(faces), "extent": ext.round(3).tolist(), "axes": {"mirror": sym, "up": up, "forward": fwd, "forward_sign": sign},
           "nose_tip_vertex": nose_idx, "nose_tip": v[nose_idx].round(3).tolist(), "units_guess": "cm" if ext[up] > 5.0 else "m",
           "edit": "nose tip +4 mm forward (r 2.5 cm), width x0.97", "max_move": float(moved.max()), "moved_over_1mm": int((moved > (0.1 if ext[up] > 5.0 else 0.001)).sum())}
    with open(os.path.join(mv, "sculpt_report.json"), "w", encoding="utf-8") as f:
        json.dump(rep, f, indent=1)
    print("SPN sculpt probe:", json.dumps(rep))


main()
