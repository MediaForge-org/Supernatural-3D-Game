"""0013 retarget PREPARATION (not S4/S5, no character chosen): the source side of a future retarget and a dry run of the tool chain.

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Creates ONLY new assets in /Game/SpnCharacterPrep (placeholder tooling, never an existing asset):
  IK_SpnMannequin            IK Rig of the template mannequin (auto-generated retarget chains + full-body IK), the SOURCE rig for every later body
  RTG_SpnMannequin_Template  IK Retargeter mannequin -> mannequin with the default op stack; a later spike sets its TARGET rig to the new body
  DryRun/                    three locomotion clips retargeted Quinn -> Manny with RTG_SpnMannequin_Template (proves the batch path works here)
Writes Saved/SpnCharacter/retarget_prep.json. Re-running reuses the rig/retargeter and overwrites the dry-run clips.
"""

import json
import os

import unreal

FOLDER = "/Game/SpnCharacterPrep"
SOURCE_MESH = "/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple"
TARGET_MESH = "/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple"
CLIPS = ["/Game/Characters/Mannequins/Anims/Unarmed/MM_Idle",
         "/Game/Characters/Mannequins/Anims/Unarmed/Walk/MF_Unarmed_Walk_Fwd",
         "/Game/Characters/Mannequins/Anims/Unarmed/Jog/MF_Unarmed_Jog_Fwd"]
OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter", "retarget_prep.json")
TOOLS = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary


def load_or_create(name, cls, factory):
    path = "%s/%s" % (FOLDER, name)
    if EAL.does_asset_exist(path):
        return unreal.load_asset(path), False
    return TOOLS.create_asset(name, FOLDER, cls, factory), True


def set_first(struct, names, value, report):
    for n in names:
        try:
            struct.set_editor_property(n, value)
            report.append(n)
            return True
        except Exception:
            continue
    return False


def main():
    report = {"status": "PREPARATION ONLY (0013); no body chosen, S4/S5 not started"}
    source_mesh = unreal.load_asset(SOURCE_MESH)
    target_mesh = unreal.load_asset(TARGET_MESH)

    # --- source IK rig
    rig, created = load_or_create("IK_SpnMannequin", unreal.IKRigDefinition, unreal.IKRigDefinitionFactory())
    rc = unreal.IKRigController.get_controller(rig)
    rc.set_skeletal_mesh(source_mesh)
    report["ik_rig"] = {"asset": rig.get_path_name(), "created": created,
                        "auto_retarget_definition": bool(rc.apply_auto_generated_retarget_definition()),
                        "auto_fbik": bool(rc.apply_auto_fbik()),
                        "retarget_root": str(rc.get_retarget_root()),
                        "chains": [{"name": str(c.chain_name), "start": str(rc.get_retarget_chain_start_bone(c.chain_name)),
                                    "end": str(rc.get_retarget_chain_end_bone(c.chain_name)), "goal": str(rc.get_retarget_chain_goal(c.chain_name))}
                                   for c in rc.get_retarget_chains()],
                        "goals": [str(g.goal_name) for g in rc.get_all_goals()],
                        "solvers": rc.get_num_solvers()}
    EAL.save_loaded_asset(rig, only_if_is_dirty=False)

    # --- retargeter template (mannequin -> mannequin; a spike only swaps the target rig and its preview mesh)
    rtg, created = load_or_create("RTG_SpnMannequin_Template", unreal.IKRetargeter, unreal.IKRetargetFactory())
    tc = unreal.IKRetargeterController.get_controller(rtg)
    tc.set_ik_rig(unreal.RetargetSourceOrTarget.SOURCE, rig)
    tc.set_ik_rig(unreal.RetargetSourceOrTarget.TARGET, rig)
    tc.set_preview_mesh(unreal.RetargetSourceOrTarget.SOURCE, source_mesh)
    tc.set_preview_mesh(unreal.RetargetSourceOrTarget.TARGET, target_mesh)
    tc.add_default_ops()
    try:
        tc.assign_ik_rig_to_all_ops(unreal.RetargetSourceOrTarget.TARGET, rig)
    except Exception as e:      # signature differs between engine versions; the op stack still uses the asset's target rig
        report["assign_ik_rig_to_all_ops"] = str(e)
    tc.auto_map_chains(unreal.AutoMapChainType.EXACT, True)
    report["retargeter"] = {"asset": rtg.get_path_name(), "created": created, "ops": [str(tc.get_op_name(i)) for i in range(tc.get_num_retarget_ops())],
                            "retarget_pose": str(tc.get_current_retarget_pose_name(unreal.RetargetSourceOrTarget.TARGET))}
    EAL.save_loaded_asset(rtg, only_if_is_dirty=False)

    # --- dry run of the batch path into the prep folder
    inputs = unreal.IKRetargetBatchOperationInputs()
    fields = []
    data = [unreal.EditorAssetLibrary.find_asset_data(p) for p in CLIPS]
    ok = all([set_first(inputs, ["assets_to_retarget"], data, fields),
              set_first(inputs, ["source_mesh"], source_mesh, fields),
              set_first(inputs, ["target_mesh"], target_mesh, fields),
              set_first(inputs, ["ik_retarget_asset", "retarget_asset", "retargeter"], rtg, fields)])
    set_first(inputs, ["suffix"], "_SpnDryRun", fields)
    set_first(inputs, ["target_path", "output_path"], FOLDER + "/DryRun", fields)
    set_first(inputs, ["use_source_path"], False, fields)
    set_first(inputs, ["include_referenced_assets"], False, fields)
    set_first(inputs, ["overwrite_existing_files"], True, fields)
    report["batch_inputs_set"] = fields
    if not ok:
        report["dry_run"] = {"error": "IKRetargetBatchOperationInputs fields not found", "doc": (unreal.IKRetargetBatchOperationInputs.__doc__ or "")[:2000]}
    else:
        made = unreal.IKRetargetBatchOperation.run_batch_retarget(inputs)
        clips = []
        for d in made:
            anim = unreal.load_asset(str(d.package_name))
            if not isinstance(anim, unreal.AnimSequence):
                continue
            src = unreal.load_asset(next((c for c in CLIPS if os.path.basename(c) in str(d.package_name)), ""))
            clips.append({"asset": str(d.package_name), "skeleton": anim.get_editor_property("skeleton").get_path_name(),
                          "frames": unreal.AnimationLibrary.get_num_frames(anim),
                          "source_frames": unreal.AnimationLibrary.get_num_frames(src) if src else None,
                          "length_s": round(unreal.AnimationLibrary.get_sequence_length(anim), 3),
                          "source_length_s": round(unreal.AnimationLibrary.get_sequence_length(src), 3) if src else None,
                          "in_prep_folder": str(d.package_name).startswith(FOLDER)})
            EAL.save_loaded_asset(anim, only_if_is_dirty=False)
        report["dry_run"] = {"created": len(clips), "clips": clips}
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN retarget prep: %s" % json.dumps({"rig": report["ik_rig"]["auto_retarget_definition"], "chains": len(report["ik_rig"]["chains"]),
                                                    "ops": report["retargeter"]["ops"], "dry_run": report["dry_run"].get("created", report["dry_run"].get("error"))}))


main()
