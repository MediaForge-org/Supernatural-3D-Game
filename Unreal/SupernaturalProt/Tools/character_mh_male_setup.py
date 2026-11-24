"""Protagonist V1 phase 2, task A (developer order 2026-11-15): a LOCAL tall male MetaHuman body as an optional TEST preset.

Run headless WITH a RHI (the MetaHuman editor subsystem needs one; no -nullrhi):
  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file> -unattended

Steps (all local, reversible, gitignored output under /Game/SpnCharacterPrep/MetaHumanTest/MaleTall):
  1 copy /Game/Characters/ProtagonistV1/MHC_ProtagonistV1 -> MHC_SpnTestMaleTall (the original is never edited; its face is only the
    unfinished 2026-10-31 preview, so the test body is NOT a likeness and NOT a character choice)
  2 body: the engine's local fixed-compatibility body DNA m_tal_nrw.dna ("male, tall, normal weight") via ImportBodyWholeRig;
    SPN_MALE_BODY=parametric instead keeps the parametric body and only sets the height constraint (SPN_MALE_HEIGHT, default 193)
  3 meshes: USpnMetaHumanPrepLibrary.generate_meta_human_meshes (public engine API TryGenerateCharacterAssets; no rig, no cloud)
  4 rig + retargeter for that body: character_mh_test_setup.py with SPN_MH_TAG=MHMaleTall
Writes Saved/SpnCharacter/mh_male_setup.json (import result, body measurements, generated assets, heights).
"""

import json
import os
import runpy

import unreal

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = "/Game/Characters/ProtagonistV1/MHC_ProtagonistV1"
FOLDER = "/Game/SpnCharacterPrep/MetaHumanTest/MaleTall"
COPY = FOLDER + "/MHC_SpnTestMaleTall"
ENGINE = unreal.Paths.convert_relative_path_to_full(unreal.Paths.engine_dir())
BODY_DNA = os.path.join(ENGINE, "Plugins", "MetaHuman", "MetaHumanCharacter", "Content", "Optional", "Body", "FixedCompatibility",
                        os.environ.get("SPN_MALE_DNA", "m_tal_nrw.dna"))
SAVED = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_saved_dir()), "SpnCharacter")
EAL = unreal.EditorAssetLibrary
S = unreal.get_editor_subsystem(unreal.MetaHumanCharacterEditorSubsystem)


def constraints(ch):
    out = {}
    try:
        for c in S.get_body_constraints(ch):
            out[str(c.get_editor_property("name"))] = round(float(c.get_editor_property("target_measurement")), 1)
    except Exception as e:  # noqa: BLE001
        out["error"] = str(e)[:200]
    return out


def main():
    report = {"status": "TEST BODY ONLY (2026-11-15): local engine content, not a character choice, no cloud"}
    ch = unreal.load_asset(COPY) if EAL.does_asset_exist(COPY) else EAL.duplicate_asset(SRC, COPY)
    EAL.save_loaded_asset(ch, only_if_is_dirty=False)
    report["edit"] = bool(S.try_add_object_to_edit(ch))
    report["constraints_before"] = constraints(ch)

    mode = os.environ.get("SPN_MALE_BODY", "dna")
    if mode == "dna":
        report["body_dna"] = BODY_DNA
        report["body_dna_exists"] = os.path.exists(BODY_DNA)
        try:
            report["import_body_whole_rig"] = str(S.import_body_whole_rig(ch, BODY_DNA, ""))
        except Exception as e:  # noqa: BLE001
            report["import_error"] = str(e)[:400]
    else:
        height = float(os.environ.get("SPN_MALE_HEIGHT", "193"))
        # Python hands out copies of array elements: build the new list explicitly
        cons = []
        for c in S.get_body_constraints(ch):
            c = c.copy()
            if str(c.get_editor_property("name")) == "Height":
                c.set_editor_property("is_active", True)
                c.set_editor_property("target_measurement", height)
            cons.append(c)
        report["active_constraints"] = [str(c.get_editor_property("name")) for c in cons if c.get_editor_property("is_active")]
        S.set_body_constraints(ch, cons)
        report["height_constraint"] = height
    S.commit_body_state(ch)
    report["constraints_after"] = constraints(ch)
    EAL.save_loaded_asset(ch, only_if_is_dirty=False)

    raw = unreal.SpnMetaHumanPrepLibrary.generate_meta_human_meshes(ch, FOLDER, True)
    gen = json.loads(raw) if raw.startswith("{") else {"raw": raw}
    report["generated"] = gen
    body = gen.get("body_mesh", "")
    face = gen.get("face_mesh", "")
    if body and face:
        os.environ["SPN_MH_BODY"] = body.split(".")[0]
        os.environ["SPN_MH_FACE"] = face.split(".")[0]
        os.environ["SPN_MH_TAG"] = "MHMaleTall"
        runpy.run_path(os.path.join(HERE, "character_mh_test_setup.py"), run_name="__main__")
        report["retarget_setup"] = "Saved/SpnCharacter/mh_test_setup_MHMaleTall.json"
        for key, path in (("body", body), ("face", face)):
            mesh = unreal.load_asset(path)
            b = mesh.get_bounds() if mesh else None
            if b:
                report[key + "_bounds_cm"] = {"min_z": round(b.origin.z - b.box_extent.z, 1), "max_z": round(b.origin.z + b.box_extent.z, 1)}
    os.makedirs(SAVED, exist_ok=True)
    with open(os.path.join(SAVED, "mh_male_setup.json"), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN MH male setup: body %s face %s" % (body, face))


main()
