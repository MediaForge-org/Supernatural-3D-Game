"""0013 preparation (night build 2026-11-14): ONE reproducible acceptance check for a future player body (no body chosen, nothing downloaded).

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Environment (optional):
  SPN_BODY       skeletal mesh path of the candidate body   (default: the template SKM_Manny_Simple, a stand-in that proves the chain)
  SPN_FOLLOWERS  comma separated follower meshes (clothing / head / hair slots, leader pose)   (default: SKM_Quinn_Simple as a dummy slot)
Steps, each recorded in Saved/SpnCharacter/candidate_check.json:
  1 skeleton contract   character_skeleton_contract.py on the body -> DIRECT / RETARGET / RETARGET + BONE MAPPING
  2 followers           every follower bone must exist in the body skeleton (leader pose copies by bone name); same skeleton asset?
  3 animation base      the clips the player's anim blueprint (ABP_Unarmed, parent USpnAnimInstance) really uses, from the asset registry;
                        batch-retargeted to the body with RTG_SpnMannequin_Template into /Game/SpnCharacterPrep/Candidate (only when the body
                        uses the mannequin skeleton; otherwise the report names the missing IK_<Body> step); frame counts must match
  4 proportions/camera  height, eye height (head bone), leg / arm ratio against the contract, the camera pivot shift the visual component would
                        apply (CameraHeightFactor 0.9) and the capsule the gameplay keeps (never changed by a body)
Verdict READY / NEEDS RETARGET RIG / BLOCKED. Creates assets only in /Game/SpnCharacterPrep/Candidate (gitignored); changes nothing else.
"""

import json
import os
import runpy

import unreal

HERE = os.path.dirname(os.path.abspath(__file__))
BODY = os.environ.get("SPN_BODY", "/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple")
FOLLOWERS = [f.strip() for f in os.environ.get("SPN_FOLLOWERS", "/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple").split(",") if f.strip()]
ABP = "/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"
RTG = "/Game/SpnCharacterPrep/RTG_SpnMannequin_Template"
SOURCE_MESH = "/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple"
OUT_FOLDER = "/Game/SpnCharacterPrep/Candidate"
SAVED = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter")
OUT = os.path.join(SAVED, "candidate_check.json")
EAL = unreal.EditorAssetLibrary


def main():
    report = {"status": "PREPARATION ONLY (0013): no body chosen, nothing downloaded", "body": BODY, "followers": FOLLOWERS}
    body = unreal.load_asset(BODY)
    if body is None:
        report["verdict"] = "BLOCKED"
        report["detail"] = "body mesh not found"
        return finish(report)

    # 1 contract (the contract script writes skeleton_check.json for SPN_CANDIDATES)
    os.environ["SPN_CANDIDATES"] = BODY
    contract = runpy.run_path(os.path.join(HERE, "character_skeleton_contract.py"), run_name="__main__")
    try:
        with open(os.path.join(SAVED, "skeleton_check.json"), encoding="utf-8") as f:
            report["contract"] = json.load(f)[0]
    except Exception as e:
        report["contract"] = {"error": str(e)}
    verdict = str(report["contract"].get("verdict", "ERROR"))

    # 2 followers: bone names must exist in the body (leader pose maps by name)
    describe = contract.get("describe")
    body_info = describe(BODY) if describe else {}
    body_bones = set(body_info.get("parents", {}).keys())
    rows = []
    for path in FOLLOWERS:
        mesh = unreal.load_asset(path)
        if mesh is None:
            rows.append({"mesh": path, "ok": False, "detail": "missing"})
            continue
        info = describe(path) if describe else {}
        bones = set(info.get("parents", {}).keys())
        missing = sorted(bones - body_bones) if body_bones else ["(body bones unknown)"]
        same = mesh.get_editor_property("skeleton") == body.get_editor_property("skeleton")
        rows.append({"mesh": path, "ok": not missing, "same_skeleton_asset": same, "bones": len(bones), "bones_missing_in_body": missing[:20]})
    report["followers_check"] = rows

    # 3 animation base: clips the player's anim blueprint uses (asset registry, recursive through blend spaces)
    reg = unreal.AssetRegistryHelpers.get_asset_registry()
    opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True)
    seen, todo, clips = set(), [ABP], []
    while todo:
        p = todo.pop()
        if p in seen or not p.startswith("/Game"):
            continue
        seen.add(p)
        for a in reg.get_assets_by_package_name(p):
            cls = str(a.asset_class_path.asset_name)
            if cls == "AnimSequence":
                clips.append(p)
            if cls in ("AnimSequence", "BlendSpace", "BlendSpace1D", "AnimBlueprint", "AnimMontage") or p == ABP:
                todo.extend(str(d) for d in (reg.get_dependencies(p, opts) or []))
    clips = sorted(set(clips))
    abp = unreal.load_asset(ABP)
    parent = None
    try:
        parent = abp.get_editor_property("parent_class").get_name()
    except Exception:
        pass
    report["animation_base"] = {"anim_blueprint": ABP, "parent_class": parent, "clips_used": clips}
    same_skeleton = body.get_editor_property("skeleton") == unreal.load_asset(SOURCE_MESH).get_editor_property("skeleton")
    target_rig_path = os.environ.get("SPN_TARGET_IKRIG", "")
    if not same_skeleton and not target_rig_path:
        report["animation_base"]["retarget"] = "skipped: the body has its own skeleton -> create IK_<Body> (0013 section 9 step 3) first, or pass SPN_TARGET_IKRIG"
        report["verdict"] = "NEEDS RETARGET RIG" if verdict.startswith("RETARGET") else "BLOCKED"
    else:
        rtg = unreal.load_asset(RTG)
        if rtg is not None and not same_skeleton:
            # 2026-11-14: a body on its own skeleton with an existing IK rig (e.g. a local engine template): a copy of the template retargeter with
            # that rig as TARGET, chains mapped by name (fuzzy), bones auto-aligned. Only in the candidate folder; the template stays untouched.
            target_rig = unreal.load_asset(target_rig_path)
            copy_path = OUT_FOLDER + "/RTG_SpnCandidate"
            # re-runs reuse the copy (deleting a loaded asset fails silently in a commandlet; 2026-11-15) and set it up again below
            if target_rig is None:
                rtg = None
            else:
                rtg = unreal.load_asset(copy_path) if EAL.does_asset_exist(copy_path) else EAL.duplicate_asset(RTG, copy_path)
            if rtg is not None:
                tc = unreal.IKRetargeterController.get_controller(rtg)
                tc.set_ik_rig(unreal.RetargetSourceOrTarget.TARGET, target_rig)
                try:
                    tc.set_preview_mesh(unreal.RetargetSourceOrTarget.TARGET, body)
                except Exception:
                    pass
                tc.auto_map_chains(unreal.AutoMapChainType.FUZZY, True)
                try:
                    tc.auto_align_all_bones(unreal.RetargetSourceOrTarget.TARGET)
                except Exception as e:  # noqa: BLE001
                    report["animation_base"]["align_note"] = str(e)
                report["animation_base"]["target_ik_rig"] = target_rig_path
                report["animation_base"]["retargeter_copy"] = copy_path
                EAL.save_loaded_asset(rtg, only_if_is_dirty=False)
            else:
                report["animation_base"]["retarget"] = "skipped: SPN_TARGET_IKRIG %s not loadable" % target_rig_path
        if rtg is None:
            report["animation_base"]["retarget"] = "skipped: run character_retarget_prep.py first (creates RTG_SpnMannequin_Template)"
            report["verdict"] = "BLOCKED"
        else:
            inputs = unreal.IKRetargetBatchOperationInputs()
            inputs.set_editor_property("assets_to_retarget", [EAL.find_asset_data(c) for c in clips])
            inputs.set_editor_property("source_mesh", unreal.load_asset(SOURCE_MESH))
            inputs.set_editor_property("target_mesh", body)
            for name in ("ik_retarget_asset", "retarget_asset", "retargeter"):
                try:
                    inputs.set_editor_property(name, rtg)
                    break
                except Exception:
                    continue
            for name, value in (("suffix", "_SpnCandidate"), ("target_path", OUT_FOLDER), ("use_source_path", False),
                                ("include_referenced_assets", False), ("overwrite_existing_files", True)):
                try:
                    inputs.set_editor_property(name, value)
                except Exception:
                    pass
            made = unreal.IKRetargetBatchOperation.run_batch_retarget(inputs)
            out = []
            for d in made:
                anim = unreal.load_asset(str(d.package_name))
                if not isinstance(anim, unreal.AnimSequence):
                    continue
                src_path = next((c for c in clips if os.path.basename(c) + "_SpnCandidate" == os.path.basename(str(d.package_name))), None)
                src = unreal.load_asset(src_path) if src_path else None
                frames = unreal.AnimationLibrary.get_num_frames(anim)
                src_frames = unreal.AnimationLibrary.get_num_frames(src) if src else None
                out.append({"asset": str(d.package_name), "frames": frames, "source_frames": src_frames, "match": src_frames == frames,
                            "in_candidate_folder": str(d.package_name).startswith(OUT_FOLDER)})
                EAL.save_loaded_asset(anim, only_if_is_dirty=False)
            report["animation_base"]["retargeted"] = out
            all_ok = len(out) == len(clips) and all(o["match"] and o["in_candidate_folder"] for o in out)
            if all_ok and verdict.startswith("DIRECT") and all(r["ok"] for r in rows):
                report["verdict"] = "READY"
            elif all_ok and not same_skeleton:
                # every clip retargeted with the frame count intact; quality (foot sliding, hands, poses) still needs a look in the editor
                report["verdict"] = "RETARGETS (visual review needed)"
            else:
                report["verdict"] = "BLOCKED"

    # 4 proportions and camera
    height = body_info.get("height_cm")
    head = None
    try:
        _, pos = contract["world_positions"](body)
        head = round(pos["head"].z - min(pos["ball_l"].z, pos["foot_l"].z), 1)
    except Exception:
        pass
    props = body_info.get("proportions") or {}
    report["proportions"] = {"height_cm": height, "head_height_cm": head, "leg_over_height": props.get("leg_over_height"), "arm_over_height": props.get("arm_over_height"),
                             "contract_reference_cm": 180.2,
                             "camera_pivot_shift_cm": round((height - 180.2) * 0.9, 1) if isinstance(height, (int, float)) else None,
                             "capsule": "unchanged by design (USpnCharacterVisualComponent never touches capsule, step or jump height)"}
    finish(report)


def finish(report):
    os.makedirs(SAVED, exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN candidate check: %s -> %s" % (report.get("body"), report.get("verdict")))


main()
