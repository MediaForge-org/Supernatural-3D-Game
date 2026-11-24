"""Protagonist V1 phase 2, task C: the Blender leg of the head round trip, WITHOUT any edit.

    blender.exe --background --factory-startup --python Tools/protagonist/blender_fbx_passthrough.py -- <in.fbx> <out.fbx> <report.json>

Imports the head FBX exported by Unreal, exports it again with settings that keep the mesh as it is (no triangulation change, no
modifiers, armature kept, no leaf bones), then re-imports the written file into a clean scene and compares it with the first import:
object / vertex / face / loop counts, vertex positions in order, UV coordinates per loop and the vertex groups (skin weights' bones).
This proves (or disproves) that Blender keeps topology, vertex order and UVs - the precondition of every sculpt workflow on a MetaHuman head.
"""

import json
import sys

import bpy


def load(path):
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bpy.ops.import_scene.fbx(filepath=path, use_custom_normals=True, ignore_leaf_bones=False, automatic_bone_orientation=False)
    meshes = sorted([o for o in bpy.context.scene.objects if o.type == "MESH"], key=lambda o: o.name)
    data = []
    for o in meshes:
        m = o.data
        uv = m.uv_layers.active
        data.append({
            "name": o.name,
            "verts": [tuple(round(c, 5) for c in v.co) for v in m.vertices],
            "faces": len(m.polygons),
            "loops": len(m.loops),
            "uv": [tuple(round(c, 6) for c in d.uv) for d in uv.data] if uv else [],
            "uv_layers": len(m.uv_layers),
            "groups": sorted(g.name for g in o.vertex_groups),
            "materials": len(m.materials),
        })
    return data


def main():
    args = sys.argv[sys.argv.index("--") + 1:]
    src, dst, rep_path = args[0], args[1], args[2]
    first = load(src)
    # Unreal's FBX export writes every LOD as its own mesh object; the template import wants the LOD 0 head only. Dropping the other
    # LOD objects is no edit of the head itself.
    lod0 = [o for o in bpy.context.scene.objects if o.type == "MESH" and o.name.endswith("_LOD0")]
    if lod0:
        for o in [o for o in bpy.context.scene.objects if o.type == "MESH" and o not in lod0]:
            bpy.data.objects.remove(o, do_unlink=True)
        first = [m for m in first if m["name"].endswith("_LOD0")]
    bpy.ops.export_scene.fbx(filepath=dst, use_selection=False, apply_scale_options="FBX_SCALE_NONE", use_mesh_modifiers=False,
                             mesh_smooth_type="OFF", use_triangles=False, add_leaf_bones=False, bake_anim=False, use_custom_props=False)
    second = load(dst)
    rows = []
    for a, b in zip(first, second):
        same_v = a["verts"] == b["verts"]
        max_d = 0.0
        if len(a["verts"]) == len(b["verts"]):
            for p, q in zip(a["verts"], b["verts"]):
                max_d = max(max_d, max(abs(p[i] - q[i]) for i in range(3)))
        max_uv = 0.0
        if len(a["uv"]) == len(b["uv"]):
            for p, q in zip(a["uv"], b["uv"]):
                max_uv = max(max_uv, abs(p[0] - q[0]), abs(p[1] - q[1]))
        rows.append({"object": a["name"], "verts": [len(a["verts"]), len(b["verts"])], "faces": [a["faces"], b["faces"]], "loops": [a["loops"], b["loops"]],
                     "vertex_order_identical": same_v, "max_position_diff": max_d, "uv_layers": [a["uv_layers"], b["uv_layers"]],
                     "max_uv_diff": max_uv if len(a["uv"]) == len(b["uv"]) else None, "vertex_groups_identical": a["groups"] == b["groups"],
                     "vertex_groups": len(a["groups"]), "materials": [a["materials"], b["materials"]]})
    report = {"in": src, "out": dst, "objects": [len(first), len(second)], "meshes": rows, "blender": bpy.app.version_string}
    with open(rep_path, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1)
    print("SPN fbx passthrough:", json.dumps({k: report[k] for k in ("objects", "blender")}), [(r["object"], r["verts"], r["vertex_order_identical"], r["max_uv_diff"]) for r in rows])


main()
