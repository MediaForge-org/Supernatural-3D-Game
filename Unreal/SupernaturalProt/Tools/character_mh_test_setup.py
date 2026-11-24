"""Protagonist V1 feasibility phase (developer order 2026-11-14, task 2): the MetaHuman TEMPLATE body as a switchable test preset.

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>

Creates ONLY /Game/SpnCharacterPrep/MetaHumanTest/IK_SpnMHTemplate (a copy of the engine's MetaHuman IK rig with the mannequin's leg / foot
chain layout) and RTG_SpnMannequinToMHTemplate (gitignored, local): a copy of RTG_SpnMannequin_Template with that rig as TARGET, chains mapped by
name (fuzzy), target bones auto-aligned to the source retarget pose, feet snapped to the ground. Nothing else changes;
the template retargeter, the mannequin and the engine content stay untouched. The preset `mh_template` (Config/DefaultGame.ini) points to it.

Writes Saved/SpnCharacter/mh_test_setup.json: chain mapping, bone / LOD / material / texture facts of the template body and face.
"""

import json
import os

import unreal

TEMPLATE_RTG = "/Game/SpnCharacterPrep/RTG_SpnMannequin_Template"
TARGET_RIG = "/MetaHumanCharacter/Animation/Retargeting/IK_MH_IKRig"
# 2026-11-15: other test bodies reuse this setup through the environment (Tools/character_mh_male_setup.py):
#   SPN_MH_BODY / SPN_MH_FACE = mesh paths, SPN_MH_TAG = name suffix of the generated rig / retargeter (default MHTemplate)
BODY = os.environ.get("SPN_MH_BODY", "/MetaHumanCharacter/Body/IdentityTemplate/SKM_Body")
FACE = os.environ.get("SPN_MH_FACE", "/MetaHumanCharacter/Face/SKM_Face")
TAG = os.environ.get("SPN_MH_TAG", "MHTemplate")
OUT_RTG = "/Game/SpnCharacterPrep/MetaHumanTest/RTG_SpnMannequinTo%s" % TAG
OUT_RIG = "/Game/SpnCharacterPrep/MetaHumanTest/IK_Spn%s" % TAG
SAVED = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter")
EAL = unreal.EditorAssetLibrary


def mesh_facts(path):
    mesh = unreal.load_asset(path)
    if mesh is None:
        return {"path": path, "missing": True}
    facts = {"path": path}
    try:
        skel = mesh.get_editor_property("skeleton")
        facts["skeleton"] = skel.get_path_name() if skel else None
    except Exception as e:  # noqa: BLE001
        facts["skeleton_error"] = str(e)
    sub = unreal.get_editor_subsystem(unreal.SkeletalMeshEditorSubsystem)
    try:
        facts["lods"] = sub.get_lod_count(mesh)
    except Exception:
        facts["lods"] = None
    tris = []
    for lod in range(facts["lods"] or 0):
        try:
            tris.append(sub.get_num_verts(mesh, lod))
        except Exception:
            break
    facts["verts_per_lod"] = tris
    # why a mesh would never leave LOD 0: screen sizes, LOD settings asset, minimum LOD, Nanite
    try:
        facts["lod_screen_sizes"] = [str(i.get_editor_property("screen_size")) for i in mesh.get_editor_property("lod_info")]
    except Exception as e:  # noqa: BLE001
        facts["lod_screen_sizes_error"] = str(e)
    for prop in ("lod_settings", "min_lod", "nanite_settings", "disable_below_min_lod_stripping", "support_ray_tracing"):
        try:
            facts[prop] = str(mesh.get_editor_property(prop))
        except Exception as e:  # noqa: BLE001
            facts[prop + "_error"] = str(e)
    try:
        facts["materials"] = [str(m.get_editor_property("material_interface").get_path_name()) if m.get_editor_property("material_interface") else None
                              for m in mesh.get_editor_property("materials")]
    except Exception as e:  # noqa: BLE001
        facts["materials_error"] = str(e)
    b = mesh.get_bounds()
    facts["bounds_cm"] = [round(b.box_extent.x * 2, 1), round(b.box_extent.y * 2, 1), round(b.box_extent.z * 2, 1)]
    facts["bounds_origin_z"] = round(b.origin.z, 1)
    # textures referenced by the materials (asset registry, recursive one level through material instances)
    reg = unreal.AssetRegistryHelpers.get_asset_registry()
    opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True)
    seen, todo, textures = set(), [path.rsplit(".", 1)[0]], {}
    depth = {todo[0]: 0}
    while todo:
        p = todo.pop()
        if p in seen:
            continue
        seen.add(p)
        for a in reg.get_assets_by_package_name(p):
            cls = str(a.asset_class_path.asset_name)
            if cls.startswith("Texture"):
                tex = unreal.load_asset(p)
                try:
                    textures[p] = {"size": [tex.blueprint_get_size_x(), tex.blueprint_get_size_y()], "class": cls}
                except Exception:
                    textures[p] = {"class": cls}
        if depth[p] < 4:
            for d in reg.get_dependencies(p, opts) or []:
                d = str(d)
                if d.startswith("/Script") or d.startswith("/Engine"):
                    continue
                if d not in depth:
                    depth[d] = depth[p] + 1
                todo.append(d)
    facts["textures"] = textures
    return facts


def main():
    report = {"status": "TEST PRESET ONLY (feasibility phase 2026-11-14): engine template body, nothing chosen, nothing downloaded"}
    template = unreal.load_asset(TEMPLATE_RTG)
    target_rig = unreal.load_asset(TARGET_RIG)
    body = unreal.load_asset(BODY)
    if template is None or target_rig is None or body is None:
        report["error"] = "missing: template %s, target rig %s, body %s" % (template is not None, target_rig is not None, body is not None)
        return finish(report)

    # The engine's MetaHuman IK rig ends the leg chains at the ball and puts the foot IK goals on ball_l / ball_r; our mannequin rig ends the
    # legs at the ankle (foot_l) with its own Foot chain and goals on foot_l. Mapped as they are, the target ball is driven to the source
    # ankle: tiptoe feet sunk into the floor (measured 2026-11-14: planted ball -5.8 cm). A local copy of the target rig with the mannequin's
    # chain layout fixes the mismatch; the engine rig stays untouched.
    # re-runs reuse the local copies (deleting a loaded asset can fail in a commandlet) and set them up again from scratch
    target_rig = unreal.load_asset(OUT_RIG) if EAL.does_asset_exist(OUT_RIG) else EAL.duplicate_asset(TARGET_RIG, OUT_RIG)
    rc = unreal.IKRigController.get_controller(target_rig)
    try:
        rc.set_skeletal_mesh(body)   # the rig's preview mesh = this body (its proportions define the retarget pose alignment)
    except Exception as e:  # noqa: BLE001
        report["rig_mesh_note"] = str(e)
    rig_notes = []
    existing = [str(c.get_editor_property("chain_name")) for c in rc.get_retarget_chains()]
    for side, s in (("Left", "l"), ("Right", "r")):
        ok_end = rc.set_retarget_chain_end_bone(side + "Leg", "foot_" + s)
        ok_goal = rc.set_goal_bone(side + "FootIK", "foot_" + s)
        new_chain = (side + "Foot (exists)") if side + "Foot" in existing else rc.add_retarget_chain(side + "Foot", "ball_" + s, "ball_" + s, "")
        rig_notes.append("%s: leg ends at foot_%s %s, goal on foot_%s %s, foot chain %s" % (side, s, ok_end, s, ok_goal, new_chain))
    report["target_rig_changes"] = rig_notes
    EAL.save_loaded_asset(target_rig, only_if_is_dirty=False)

    rtg = unreal.load_asset(OUT_RTG) if EAL.does_asset_exist(OUT_RTG) else EAL.duplicate_asset(TEMPLATE_RTG, OUT_RTG)
    tc = unreal.IKRetargeterController.get_controller(rtg)
    tc.set_ik_rig(unreal.RetargetSourceOrTarget.TARGET, target_rig)
    try:
        tc.set_preview_mesh(unreal.RetargetSourceOrTarget.TARGET, body)
    except Exception as e:  # noqa: BLE001
        report["preview_mesh_note"] = str(e)
    # every op of the stack has its own target rig (UE 5.6+); the copy of the template still pointed its ops at the mannequin rig
    try:
        tc.assign_ik_rig_to_all_ops(unreal.RetargetSourceOrTarget.TARGET, target_rig)
        report["ops_target_rig"] = OUT_RIG
    except Exception as e:  # noqa: BLE001
        report["ops_target_rig_error"] = str(e)
    tc.auto_map_chains(unreal.AutoMapChainType.FUZZY, True)
    try:
        tc.auto_align_all_bones(unreal.RetargetSourceOrTarget.TARGET)
    except Exception as e:  # noqa: BLE001
        report["align_note"] = str(e)
    # the retarget pose's feet on the ground (the template body's reference pose need not stand at z = 0)
    try:
        tc.snap_bone_to_ground("ball_l", unreal.RetargetSourceOrTarget.TARGET)
        report["snap_to_ground"] = "ball_l"
    except Exception as e:  # noqa: BLE001
        report["snap_note"] = str(e)

    # Foot sliding (measured 2026-11-14, diagnosed 2026-11-15): the template's op stack has NO "Retarget IK Goals" op, so the leg IK goals
    # just follow the FK chains, whose stride scales with the body's leg length while the capsule (gameplay) speed does not:
    # sliding ~ speed x (1 - target/source leg ratio) - measured 21.6 cm/s walking for the 160 cm body = 180 x 0.12.
    # The cause is removed by retargeting the leg goals and blending their HORIZONTAL position fully to the source (mannequin) foot, whose
    # planted foot does not slide relative to the capsule; the vertical stays scaled to the body. (The per-chain "ik" settings used on
    # 2026-11-14 are the deprecated pre-5.6 struct and had no effect.)  SPN_MH_LEG_BLEND=0 keeps the old behaviour for a comparison run.
    leg_blend = float(os.environ.get("SPN_MH_LEG_BLEND", "1.0"))
    ops = [str(tc.get_op_name(i)) for i in range(tc.get_num_retarget_ops())]
    report["ops_before"] = ops
    if leg_blend > 0.0:
        try:
            # "Blend To Source" (UE 5.6+ op) as a child of "Run IK Rig", like the engine's own RTG_MH_IKRig puts its goal ops
            name = "Spn Leg Goals"
            idx = tc.get_index_of_op_by_name(name) if name in ops else tc.add_retarget_op("/Script/IKRig.IKRetargetBlendToSourceOp")
            if idx < 0:
                raise RuntimeError("add_retarget_op returned %d" % idx)
            tc.set_op_name(name, idx)
            tc.set_parent_op_by_name(name, "Run IK Rig")
            run_ik = tc.get_index_of_op_by_name("Run IK Rig")
            idx = tc.get_index_of_op_by_name(name)
            if 0 <= run_ik < idx:
                tc.move_retarget_op_in_stack(idx, run_ik)
            tc.assign_ik_rig_to_all_ops(unreal.RetargetSourceOrTarget.TARGET, target_rig)
            idx = tc.get_index_of_op_by_name(name)
            tc.run_op_initial_setup(idx)
            opc = tc.get_op_controller(idx)
            settings = opc.get_settings()
            chains = list(settings.get_editor_property("chains"))
            names = [str(c.get_editor_property("target_chain_name")) for c in chains]
            for leg in ("LeftLeg", "RightLeg"):
                if leg not in names:
                    c = unreal.IKRetargetBlendToSourceChainSettings()
                    c.set_editor_property("target_chain_name", leg)
                    chains.append(c)
            done = []
            for i, c in enumerate(chains):
                c = c.copy()
                is_leg = str(c.get_editor_property("target_chain_name")) in ("LeftLeg", "RightLeg")
                c.set_editor_property("blend_to_source", leg_blend if is_leg else 0.0)
                if is_leg:
                    c.set_editor_property("translation_alpha", 1.0)
                    c.set_editor_property("rotation_alpha", 0.0)
                    c.set_editor_property("translation_per_axis_alpha", unreal.Vector(1.0, 1.0, 0.0))
                    done.append(str(c.get_editor_property("target_chain_name")))
                chains[i] = c
            settings.set_editor_property("chains", chains)
            settings.set_editor_property("alpha", 1.0)
            try:
                settings.set_editor_property("debug_draw", False)
            except Exception:  # noqa: BLE001
                pass
            opc.set_settings(settings)
            report["leg_goals"] = {"op_index": idx, "parent": str(tc.get_parent_op_by_name(name)), "blended": done,
                                   "settings": opc.get_settings().export_text()[:1500]}
        except Exception as e:  # noqa: BLE001
            report["leg_goals_error"] = repr(e)[:400]
    report["ops_after"] = [str(tc.get_op_name(i)) for i in range(tc.get_num_retarget_ops())]

    # the chain mapping as the retargeter has it now
    mapping = []
    try:
        src_rig = tc.get_ik_rig(unreal.RetargetSourceOrTarget.SOURCE)
        src_ctrl = unreal.IKRigController.get_controller(src_rig)
        tgt_ctrl = unreal.IKRigController.get_controller(target_rig)
        report["source_rig"] = src_rig.get_path_name()
        report["source_chains"] = [str(c.get_editor_property("chain_name")) for c in src_ctrl.get_retarget_chains()]
        report["target_chains"] = [str(c.get_editor_property("chain_name")) for c in tgt_ctrl.get_retarget_chains()]
        for t in report["target_chains"]:
            try:
                mapping.append({"target": t, "source": str(tc.get_source_chain(t))})
            except Exception as e:  # noqa: BLE001
                mapping.append({"target": t, "error": str(e)})
    except Exception as e:  # noqa: BLE001
        report["mapping_error"] = str(e)
    report["chain_mapping"] = mapping
    EAL.save_loaded_asset(rtg, only_if_is_dirty=False)
    report["retargeter"] = OUT_RTG
    report["body"] = mesh_facts(BODY)
    report["face"] = mesh_facts(FACE)
    report["mannequin"] = mesh_facts("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple")
    finish(report)


def finish(report):
    os.makedirs(SAVED, exist_ok=True)
    name = "mh_test_setup.json" if TAG == "MHTemplate" else "mh_test_setup_%s.json" % TAG
    with open(os.path.join(SAVED, name), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN MH test setup: %s" % report.get("retargeter", report.get("error")))


main()
