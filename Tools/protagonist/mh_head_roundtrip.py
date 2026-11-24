"""Protagonist V1 phase 2, task C (developer order 2026-11-15): isolated export / import prototype for an UNCHANGED MetaHuman head.

Only documented, Python-exposed engine interfaces are used:
  export  MetaHumanCharacterExportBlueprintLibrary.export_dna (head .dna file), .export_dcc (DCC package), AssetExportTask (FBX of the head mesh)
  import  MetaHumanCharacterEditorSubsystem.import_from_template (MetaHuman-topology mesh, vertices matched by UVs)
          MetaHumanCharacterEditorSubsystem.import_from_face_dna (DNA file, whole rig)
  edit    MetaHumanCharacterEditorSubsystem.translate_face_landmarks (proof that the result is still editable as a MetaHuman)
plus the project's editor helper USpnMetaHumanPrepLibrary (mesh generation via the public TryGenerateCharacterAssets, rig state, mesh compare).

Run in Unreal with a RHI (no -nullrhi):  UnrealEditor-Cmd.exe <uproject> -run=pythonscript -script=<this file> -unattended
  SPN_RT_STEP=export   source -> files in Saved/SpnCharacter/headrt
  (Blender: Tools/protagonist/blender_fbx_passthrough.py head_original.fbx -> head_blender.fbx, unchanged)
  SPN_RT_STEP=import   three targets: A template import of the generated head mesh, B template import of the Blender round-tripped FBX,
                       C DNA import; each compared with the source (landmarks, generated face mesh vertex by vertex, rig state, edit test)
Everything lands in /Game/SpnCharacterPrep/MetaHumanTest/HeadRoundTrip (gitignored). No cloud, no auto-rig, no download.
"""

import json
import os

import unreal

SOURCE = "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/MHC_SpnTestMaleTall"
SOURCE_FACE = "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall/Face/SKM_MHC_SpnTestMaleTall_FaceMesh"
FOLDER = "/Game/SpnCharacterPrep/MetaHumanTest/HeadRoundTrip"
SAVED = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_saved_dir()), "SpnCharacter")
RT = os.path.join(SAVED, "headrt")
EAL = unreal.EditorAssetLibrary
S = unreal.get_editor_subsystem(unreal.MetaHumanCharacterEditorSubsystem)
LIB = unreal.SpnMetaHumanPrepLibrary
EXP = unreal.MetaHumanCharacterExportBlueprintLibrary


def landmarks(ch):
    return [(v.x, v.y, v.z) for v in S.get_face_landmarks(ch)]


def lm_diff(a, b):
    if len(a) != len(b) or not a:
        return {"error": "landmark counts %d / %d" % (len(a), len(b))}
    d = [((p[0] - q[0]) ** 2 + (p[1] - q[1]) ** 2 + (p[2] - q[2]) ** 2) ** 0.5 for p, q in zip(a, b)]
    return {"max_cm": round(max(d), 4), "mean_cm": round(sum(d) / len(d), 4), "count": len(d)}


def files_under(path):
    out = []
    for root, _, files in os.walk(path):
        for f in files:
            p = os.path.join(root, f)
            out.append({"file": os.path.relpath(p, path), "kb": round(os.path.getsize(p) / 1024.0, 1)})
    return out


def export_step(report):
    src = unreal.load_asset(SOURCE)
    S.try_add_object_to_edit(src)
    report["source_rig_state"] = LIB.get_meta_human_rig_state(src)
    report["source_landmarks"] = landmarks(src)
    os.makedirs(RT, exist_ok=True)
    p = unreal.MetaHumanDNAExportParams()
    p.set_editor_property("external_path", os.path.join(RT, "dna"))
    p.set_editor_property("project_path", "")
    p.set_editor_property("dna_body", False)
    try:
        EXP.export_dna(src, p)
    except Exception as e:  # noqa: BLE001
        report["export_dna_error"] = str(e)[:300]
    report["dna_files"] = files_under(os.path.join(RT, "dna"))
    d = unreal.MetaHumanDCCExportParams()
    d.set_editor_property("external_path", os.path.join(RT, "dcc"))
    try:
        EXP.export_dcc(src, d)
    except Exception as e:  # noqa: BLE001
        report["export_dcc_error"] = str(e)[:300]
    report["dcc_files"] = files_under(os.path.join(RT, "dcc"))[:60]
    # FBX of the (unchanged) generated head mesh, the hand-over format for Blender
    task = unreal.AssetExportTask()
    task.set_editor_property("object", unreal.load_asset(SOURCE_FACE))
    task.set_editor_property("filename", os.path.join(RT, "head_original.fbx"))
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_identical", True)
    task.set_editor_property("prompt", False)
    report["fbx_export_ok"] = bool(unreal.Exporter.run_asset_export_task(task))
    report["fbx_kb"] = round(os.path.getsize(os.path.join(RT, "head_original.fbx")) / 1024.0, 1) if os.path.exists(os.path.join(RT, "head_original.fbx")) else None


def new_target(name):
    path = FOLDER + "/" + name
    if EAL.does_asset_exist(path):
        EAL.delete_asset(path)
    ch = None
    try:
        ch = unreal.AssetToolsHelpers.get_asset_tools().create_asset(name, FOLDER, unreal.MetaHumanCharacter, unreal.MetaHumanCharacterFactoryNew())
    except Exception:  # noqa: BLE001
        ch = None
    how = "new MetaHuman Character (factory default face)"
    if ch is None:
        ch = EAL.duplicate_asset("/MetaHumanCharacter/Presets/" + "Ada", path) if False else EAL.duplicate_asset(SOURCE, path)
        how = "copy of the source with its face pushed away first"
    S.try_add_object_to_edit(ch)
    if how.startswith("copy"):
        # make the face clearly different, so a successful import must move it back
        n = len(S.get_face_landmarks(ch))
        S.translate_face_landmarks(ch, list(range(0, n, 3)), [unreal.Vector(0.0, 0.6, 0.3)] * len(range(0, n, 3)))
        S.commit_face_state(ch)
    return ch, how


def check_target(report_key, ch, how, source_lm, report):
    r = {"start": how}
    r["landmarks_vs_source_before_import"] = report.get("_before", {})
    after = landmarks(ch)
    r["landmarks_vs_source"] = lm_diff(after, source_lm)
    r["rig_state"] = LIB.get_meta_human_rig_state(ch)
    # editability: move the nose-tip landmark 3 mm forward through the MetaHuman editor API, read it back, then undo it
    try:
        n = 64 if len(after) > 64 else 0
        before = after[n]
        S.translate_face_landmarks(ch, [n], [unreal.Vector(0.0, -0.3, 0.0)])
        moved = landmarks(ch)[n]
        r["edit_test"] = {"landmark": n, "moved_cm": round(((moved[0] - before[0]) ** 2 + (moved[1] - before[1]) ** 2 + (moved[2] - before[2]) ** 2) ** 0.5, 3)}
        S.translate_face_landmarks(ch, [n], [unreal.Vector(0.0, 0.3, 0.0)])
    except Exception as e:  # noqa: BLE001
        r["edit_test_error"] = str(e)[:300]
    S.commit_face_state(ch)
    EAL.save_loaded_asset(ch, only_if_is_dirty=False)
    gen = json.loads(LIB.generate_meta_human_meshes(ch, FOLDER + "/" + report_key, False))
    r["generated"] = {k: gen.get(k) for k in ("face_mesh", "saved_ok", "error")}
    if gen.get("face_mesh"):
        mesh = unreal.load_asset(gen["face_mesh"])
        r["face_mesh_vs_source_in_order"] = json.loads(LIB.compare_skeletal_meshes(unreal.load_asset(SOURCE_FACE), mesh))
        r["face_mesh_vs_source_by_uv"] = json.loads(LIB.compare_skeletal_meshes_by_uv(unreal.load_asset(SOURCE_FACE), mesh))
        r["head_shape_vs_source"] = json.loads(LIB.compare_head_shapes(unreal.load_asset(SOURCE_FACE), mesh))
    r["landmark_counts"] = [len(after), len(source_lm)]
    report[report_key] = r


def import_fbx_skeletal(fbx, name):
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", fbx)
    task.set_editor_property("destination_path", FOLDER)
    task.set_editor_property("destination_name", name)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", True)
    ui = unreal.FbxImportUI()
    ui.set_editor_property("import_mesh", True)
    ui.set_editor_property("import_as_skeletal", True)
    ui.set_editor_property("import_animations", False)
    ui.set_editor_property("import_materials", False)
    ui.set_editor_property("import_textures", False)
    ui.set_editor_property("skeleton", unreal.load_asset(SOURCE_FACE).get_editor_property("skeleton"))
    task.set_editor_property("options", ui)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    paths = list(task.get_editor_property("imported_object_paths"))
    meshes = [p for p in paths if isinstance(unreal.load_asset(p), unreal.SkeletalMesh)]
    return meshes[0] if meshes else None, paths


def import_step(report):
    src_report = json.load(open(os.path.join(SAVED, "head_roundtrip_export.json"), encoding="utf-8"))
    source_lm = [tuple(p) for p in src_report["source_landmarks"]]
    params = unreal.ImportFromTemplateParams()
    params.set_editor_property("match_vertices_by_u_vs", True)
    # SPN_RT_ALIGN = NONE / TRANSLATION / ROTATION_TRANSLATION / SCALING_TRANSLATION / SCALING_ROTATION_TRANSLATION (engine default)
    align = os.environ.get("SPN_RT_ALIGN", "")
    if align:
        params.set_editor_property("alignment_options", getattr(unreal.MetaHumanAlignmentOptions, align))
        report["alignment"] = align
    if os.environ.get("SPN_RT_ONLY_A") == "1":
        ch, how = new_target("MHC_RT_A_Template_" + (align or "default"))
        report["A_import"] = str(S.import_from_template(ch, unreal.load_asset(SOURCE_FACE), None, None, None, params))
        check_target("A_" + (align or "default"), ch, how, source_lm, report)
        report.pop("_before", None)
        return

    # A: the generated head mesh itself (MetaHuman topology) as the template
    ch, how = new_target("MHC_RT_A_Template")
    report["_before"] = lm_diff(landmarks(ch), source_lm)
    # the starting face BEFORE the import, measured the same way (so "after" can be judged against it)
    pre = json.loads(LIB.generate_meta_human_meshes(ch, FOLDER + "/A_before", False))
    if pre.get("face_mesh"):
        report["A_start_face_vs_source_by_uv"] = json.loads(LIB.compare_skeletal_meshes_by_uv(unreal.load_asset(SOURCE_FACE), unreal.load_asset(pre["face_mesh"])))
        report["A_start_head_shape_vs_source"] = json.loads(LIB.compare_head_shapes(unreal.load_asset(SOURCE_FACE), unreal.load_asset(pre["face_mesh"])))
    report["A_import"] = str(S.import_from_template(ch, unreal.load_asset(SOURCE_FACE), None, None, None, params))
    check_target("A_template", ch, how, source_lm, report)

    # B: the same head after FBX export -> Blender import / export (no edit) -> FBX import
    fbx = os.path.join(RT, "head_blender.fbx")
    if os.path.exists(fbx):
        mesh, paths = import_fbx_skeletal(fbx, "SKM_HeadBlenderPassthrough")
        report["B_fbx_import"] = {"mesh": mesh, "paths": paths}
        if mesh:
            report["B_mesh_vs_source"] = json.loads(LIB.compare_skeletal_meshes(unreal.load_asset(SOURCE_FACE), unreal.load_asset(mesh)))
            ch, how = new_target("MHC_RT_B_Blender")
            report["_before"] = lm_diff(landmarks(ch), source_lm)
            report["B_import"] = str(S.import_from_template(ch, unreal.load_asset(mesh), None, None, None, params))
            check_target("B_blender", ch, how, source_lm, report)
            a_mesh = (report.get("A_template", {}).get("generated") or {}).get("face_mesh")
            b_mesh = (report.get("B_blender", {}).get("generated") or {}).get("face_mesh")
            if a_mesh and b_mesh:
                report["A_vs_B_in_order"] = json.loads(LIB.compare_skeletal_meshes(unreal.load_asset(a_mesh), unreal.load_asset(b_mesh)))
    else:
        report["B_skipped"] = "head_blender.fbx missing (run the Blender pass-through first)"

    # C: the exported head DNA, whole rig
    dna = [f["file"] for f in src_report.get("dna_files", []) if f["file"].lower().endswith(".dna")]
    if dna:
        ch, how = new_target("MHC_RT_C_Dna")
        report["_before"] = lm_diff(landmarks(ch), source_lm)
        p = unreal.ImportFromDNAParams()
        p.set_editor_property("import_whole_rig", True)
        report["C_dna_file"] = dna[0]
        report["C_import"] = str(S.import_from_face_dna(ch, os.path.join(RT, "dna", dna[0]), p))
        check_target("C_dna", ch, how, source_lm, report)
    else:
        report["C_skipped"] = "no .dna file was exported"
    report.pop("_before", None)


def main():
    step = os.environ.get("SPN_RT_STEP", "export")
    report = {"status": "PROTOTYPE (2026-11-15): unchanged head, local, nothing chosen, no cloud", "step": step}
    try:
        export_step(report) if step == "export" else import_step(report)
    except Exception as e:  # noqa: BLE001
        report["error"] = repr(e)[:800]
    os.makedirs(SAVED, exist_ok=True)
    with open(os.path.join(SAVED, "head_roundtrip_%s.json" % step), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN head round trip %s: %s" % (step, report.get("error", "done")))
    # the FBX export of a skeletal mesh needs a rendered mesh object (it asserts in a commandlet): the export step runs in the editor GUI
    # (UnrealEditor.exe <uproject> -ExecutePythonScript=<this file>) and closes it again when SPN_RT_QUIT=1
    if os.environ.get("SPN_RT_QUIT") == "1":
        unreal.SystemLibrary.quit_editor()


main()
