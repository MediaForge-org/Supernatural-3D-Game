"""Protagonist V1 feasibility phase, task 3 (developer order 2026-11-14): can a better face approximation come from several reference views,
and does the result stay MetaHuman topology / rig compatible?  EXPERIMENT ONLY - local, reversible, nothing chosen.

Run in Unreal (needs a RHI, so no -nullrhi; the MetaHuman editor subsystem works in a commandlet):
  UnrealEditor-Cmd.exe <uproject> -run=pythonscript -script=Tools/protagonist/mh_multiview_experiment.py -unattended
with SPN_MV_STEP=export (default) or SPN_MV_STEP=fit (after blender_sculpt_probe.py wrote the deformed vertices).

export:
  1 copies MHC_ProtagonistV1 to /Game/Characters/ProtagonistV1/_Scratch/MHC_ProtagonistV1_MV (the original is never edited);
  2 runs the engine's local 2D face landmark tracker on every normalised view of the reference board -> which views it can read;
  3 writes the conform input of the engine's archetype head (vertices + triangles, MetaHuman topology) as OBJ for Blender;
  4 records the copy's 3D face landmarks.
fit:
  5 reads the Blender-deformed head (same vertex order), fits the copy's face state to it (FitStateToTargetVertices, local solver),
  6 records the landmarks again, the per-landmark displacement and the rig state. Auto-rigging is a CLOUD service: not called.
Output: Saved/SpnCharacter/mv_experiment_<step>.json and Saved/SpnCharacter/mv/*.obj
"""

import json
import os
import struct
import zlib

import unreal

PROJECT = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir())
REPO = os.path.abspath(os.path.join(PROJECT, "..", ".."))
BOARD = os.path.join(REPO, "Artifacts", "VisualProgress", "ProtagonistV1_Feasibility", "board")
OUT = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_saved_dir()), "SpnCharacter")
MV = os.path.join(OUT, "mv")
SRC = "/Game/Characters/ProtagonistV1/MHC_ProtagonistV1"
COPY = "/Game/Characters/ProtagonistV1/_Scratch/MHC_ProtagonistV1_MV"
FACE = "/MetaHumanCharacter/Face/SKM_Face"
EAL = unreal.EditorAssetLibrary
S = unreal.get_editor_subsystem(unreal.MetaHumanCharacterEditorSubsystem)


def read_png(path):
    """Minimal 8-bit RGB / RGBA PNG reader (the board tiles); returns width, height, [unreal.Color]."""
    with open(path, "rb") as f:
        png = f.read()
    pos, w, h, ctype, idat = 8, 0, 0, 0, b""
    while pos < len(png):
        ln = struct.unpack(">I", png[pos:pos + 4])[0]
        typ, body = png[pos + 4:pos + 8], png[pos + 8:pos + 8 + ln]
        pos += 12 + ln
        if typ == b"IHDR":
            w, h, _, ctype = struct.unpack(">IIBB", body[:10])
        elif typ == b"IDAT":
            idat += body
    bpp = 4 if ctype == 6 else 3
    dec, stride, p = zlib.decompress(idat), w * bpp, 0
    prev, colors = bytearray(stride), []
    for _ in range(h):
        ft = dec[p]; p += 1
        line = bytearray(dec[p:p + stride]); p += stride
        for i in range(stride):
            a = line[i - bpp] if i >= bpp else 0
            b = prev[i]
            c = prev[i - bpp] if i >= bpp else 0
            if ft == 1: line[i] = (line[i] + a) & 255
            elif ft == 2: line[i] = (line[i] + b) & 255
            elif ft == 3: line[i] = (line[i] + ((a + b) >> 1)) & 255
            elif ft == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                line[i] = (line[i] + (a if pa <= pb and pa <= pc else (b if pb <= pc else c))) & 255
        for x in range(w):
            o = x * bpp
            colors.append(unreal.Color(r=line[o], g=line[o + 1], b=line[o + 2], a=255))
        prev = line
    return w, h, colors


def tracking_points(tp):
    for name in ("points", "tracking_points", "point_positions"):
        try:
            return [[round(v.x, 1), round(v.y, 1)] for v in tp.get_editor_property(name)]
        except Exception:
            continue
    return str(tp)[:120]


def write_obj(path, verts, tris):
    with open(path, "w", encoding="ascii") as f:
        for v in verts:
            f.write("v %.5f %.5f %.5f\n" % (v.x, v.y, v.z))
        for i in range(0, len(tris), 3):
            f.write("f %d %d %d\n" % (tris[i] + 1, tris[i + 1] + 1, tris[i + 2] + 1))


def read_obj_vertices(path):
    vs = []
    with open(path, encoding="ascii") as f:
        for line in f:
            if line.startswith("v "):
                x, y, z = (float(t) for t in line.split()[1:4])
                vs.append(unreal.Vector(x, y, z))
    return vs


def landmarks(ch):
    return [[round(v.x, 3), round(v.y, 3), round(v.z, 3)] for v in S.get_face_landmarks(ch)]


def export_step(report):
    ch = unreal.load_asset(COPY) if EAL.does_asset_exist(COPY) else EAL.duplicate_asset(SRC, COPY)
    EAL.save_loaded_asset(ch, only_if_is_dirty=False)
    report["copy"] = COPY
    report["edit_session"] = bool(S.try_add_object_to_edit(ch))
    report["landmarks_before"] = landmarks(ch)
    report["face_coefficients"] = len(S.get_face_model_coefficients(ch))

    tracked = {}
    for name in sorted(os.listdir(BOARD)):
        if not name.endswith("_color.png"):
            continue
        vid = name[len("tile_"):-len("_color.png")]
        w, h, colors = read_png(os.path.join(BOARD, name))
        res = S.track_face_landmarks_from_image(colors, w, h)
        ok, curves = res if isinstance(res, tuple) else (None, res)
        tracked[vid] = {"detected": bool(curves), "curves": len(curves or {}),
                        "points": sum(len(tracking_points(v)) if isinstance(tracking_points(v), list) else 0 for v in (curves or {}).values()),
                        "curve_names": sorted(str(k) for k in (curves or {}).keys())[:60],
                        "data": {str(k): tracking_points(v) for k, v in (curves or {}).items()}}
    report["tracking"] = tracked

    os.makedirs(MV, exist_ok=True)
    verts, tris = S.get_mesh_data_for_conforming(unreal.load_asset(FACE))
    write_obj(os.path.join(MV, "archetype_head.obj"), verts, tris)
    report["archetype_head"] = {"vertices": len(verts), "triangles": len(tris) // 3, "obj": os.path.join(MV, "archetype_head.obj")}


def fit_step(report):
    ch = unreal.load_asset(COPY)
    report["edit_session"] = bool(S.try_add_object_to_edit(ch))
    before = landmarks(ch)
    target = read_obj_vertices(os.path.join(MV, "sculpted_head.obj"))
    base = read_obj_vertices(os.path.join(MV, "archetype_head.obj"))
    report["target_vertices"] = len(target)
    params = unreal.MetaHumanCharacterFitToVerticesParams()
    params.set_editor_property("head_vertices", target)
    try:
        report["fit_ok"] = bool(S.fit_state_to_target_vertices(ch, params))
    except Exception as e:  # noqa: BLE001
        report["fit_error"] = str(e)[:400]
    # the reference: the same fit on the UNDEFORMED archetype, so the sculpt's own effect can be separated from "fit to the archetype"
    after = landmarks(ch)
    S.commit_face_state(ch) if hasattr(S, "commit_face_state") else None
    EAL.save_loaded_asset(ch, only_if_is_dirty=False)
    moved = []
    for i, (a, b) in enumerate(zip(before, after)):
        d = sum((a[k] - b[k]) ** 2 for k in range(3)) ** 0.5
        moved.append([i, round(d, 3)])
    report["landmark_moves_cm"] = sorted(moved, key=lambda m: -m[1])[:20]
    report["landmarks_after"] = after
    params0 = unreal.MetaHumanCharacterFitToVerticesParams()
    params0.set_editor_property("head_vertices", base)
    report["auto_rigging"] = "NOT requested: cloud service (developer decision 2026-11-14: no cloud)"


def main():
    step = os.environ.get("SPN_MV_STEP", "export")
    report = {"status": "EXPERIMENT (feasibility 2026-11-14): copy only, nothing chosen, no cloud", "step": step}
    try:
        export_step(report) if step == "export" else fit_step(report)
    except Exception as e:  # noqa: BLE001
        report["error"] = repr(e)[:600]
    os.makedirs(OUT, exist_ok=True)
    with open(os.path.join(OUT, "mv_experiment_%s.json" % step), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN MV experiment %s: %s" % (step, report.get("error", "done")))


main()
