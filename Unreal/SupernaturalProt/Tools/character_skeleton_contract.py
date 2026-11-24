"""0013 retarget preparation: the reference skeleton contract and a checker for candidate bodies. READ-ONLY (changes no asset).

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Optional: environment variable SPN_CANDIDATES = comma separated skeletal mesh paths to check (default: the two template mannequins).
Writes Saved/SpnCharacter/skeleton_contract.json (the reference) and Saved/SpnCharacter/skeleton_check.json (one verdict per candidate).

The contract is an ENGINEERING PROPOSAL derived from the UE5 template mannequin (SK_Mannequin), which all existing animations use. It is not a
decision about the final body or pipeline (0013 section 6 stays open). A candidate that fails it is not rejected: it needs an IK Retargeter.
"""

import json
import math
import os

import unreal

REFERENCE_MESH = "/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple"
DEFAULT_CANDIDATES = ["/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple", "/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple"]
OUT_DIR = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter")

# bones every body must have under these names to use the existing animation sets without retargeting (humanoid core of SK_Mannequin)
REQUIRED = ["root", "pelvis", "spine_01", "spine_02", "spine_03", "spine_04", "spine_05", "neck_01", "neck_02", "head",
            "clavicle_l", "upperarm_l", "lowerarm_l", "hand_l", "clavicle_r", "upperarm_r", "lowerarm_r", "hand_r",
            "thigh_l", "calf_l", "foot_l", "ball_l", "thigh_r", "calf_r", "foot_r", "ball_r"]
# used by the template's foot IK control rig and by weapon/prop animations; without them those parts need adapting
RECOMMENDED = ["ik_foot_root", "ik_foot_l", "ik_foot_r", "ik_hand_root", "ik_hand_gun", "ik_hand_l", "ik_hand_r"]
# core retarget chains: exactly as the engine's auto-generated mannequin definition (IK_SpnMannequin, character_retarget_prep.py) names them; the
# 20 finger / metacarpal chains of that definition are optional for a body without finger bones
CHAINS = {"Spine": ("spine_01", "spine_05"), "Neck": ("neck_01", "neck_02"), "Head": ("head", "head"),
          "LeftClavicle": ("clavicle_l", "clavicle_l"), "LeftArm": ("upperarm_l", "hand_l"),
          "RightClavicle": ("clavicle_r", "clavicle_r"), "RightArm": ("upperarm_r", "hand_r"),
          "LeftLeg": ("thigh_l", "foot_l"), "LeftFoot": ("ball_l", "ball_l"),
          "RightLeg": ("thigh_r", "foot_r"), "RightFoot": ("ball_r", "ball_r")}


def world_positions(mesh):
    pose = mesh.skeleton.get_reference_pose()
    names = [str(n) for n in unreal.AnimPoseExtensions.get_bone_names(pose)]
    out = {}
    for n in names:
        t = unreal.AnimPoseExtensions.get_ref_bone_pose(pose, n, unreal.AnimPoseSpaces.WORLD)
        out[n] = t.translation
    return names, out


def dist(a, b):
    return math.sqrt((a.x - b.x) ** 2 + (a.y - b.y) ** 2 + (a.z - b.z) ** 2)


def describe(mesh_path):
    mesh = unreal.load_asset(mesh_path)
    if mesh is None or not isinstance(mesh, unreal.SkeletalMesh):
        return {"mesh": mesh_path, "error": "not a skeletal mesh / not found"}
    names, pos = world_positions(mesh)
    bounds = mesh.get_imported_bounds()
    info = {"mesh": mesh_path, "skeleton": mesh.skeleton.get_path_name(), "bone_count": len(names),
            "height_cm": round(bounds.box_extent.z * 2.0, 1),
            "parents": {n: str(mesh.get_bone_parent(n)) for n in names},
            "sockets": [str(mesh.get_socket_by_index(i).socket_name) for i in range(mesh.num_sockets())]}
    has = set(names)
    info["missing_required"] = [b for b in REQUIRED if b not in has]
    info["missing_recommended"] = [b for b in RECOMMENDED if b not in has]
    if not info["missing_required"]:
        head, pelvis, foot = pos["head"], pos["pelvis"], pos["foot_l"]
        info["upright_z"] = head.z > pelvis.z > foot.z
        h = max(head.z - min(pos["ball_l"].z, foot.z), 1.0)
        info["proportions"] = {
            "leg_over_height": round((dist(pos["thigh_l"], pos["calf_l"]) + dist(pos["calf_l"], pos["foot_l"])) / h, 3),
            "arm_over_height": round((dist(pos["upperarm_l"], pos["lowerarm_l"]) + dist(pos["lowerarm_l"], pos["hand_l"])) / h, 3),
            "pelvis_over_height": round((pelvis.z - foot.z) / h, 3)}
        # reference pose type: angle of the upper arm below the horizontal (T-pose ~0, A-pose ~35-50 degrees)
        a, b = pos["upperarm_l"], pos["lowerarm_l"]
        horiz = math.sqrt((b.x - a.x) ** 2 + (b.y - a.y) ** 2)
        info["ref_pose_arm_angle_deg"] = round(math.degrees(math.atan2(a.z - b.z, max(horiz, 1e-3))), 1)
        # facing: the toes point from the ankle towards ball_l; the template mannequin faces +Y in mesh space
        f = pos["ball_l"]
        info["toes_direction_xy"] = [round(f.x - foot.x, 1), round(f.y - foot.y, 1)]
    return info


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    reference = describe(REFERENCE_MESH)
    contract = {"status": "ENGINEERING PROPOSAL (0013), not a decision", "reference": reference, "required": REQUIRED,
                "recommended": RECOMMENDED, "chains": CHAINS, "units": "cm, Z up"}
    with open(os.path.join(OUT_DIR, "skeleton_contract.json"), "w") as f:
        json.dump(contract, f, indent=1, default=str)

    candidates = [c.strip() for c in os.environ.get("SPN_CANDIDATES", "").split(",") if c.strip()] or DEFAULT_CANDIDATES
    verdicts = []
    for path in candidates:
        d = describe(path)
        if "error" in d:
            verdicts.append({"mesh": path, "verdict": "ERROR", "detail": d["error"]})
            continue
        same_skeleton = d["skeleton"] == reference["skeleton"]
        problems = []
        if d["missing_required"]:
            problems.append("missing required bones: %s" % ", ".join(d["missing_required"]))
        if d.get("upright_z") is False:
            problems.append("not Z-up (head above pelvis above feet fails)")
        if d.get("ref_pose_arm_angle_deg") is not None and abs(d["ref_pose_arm_angle_deg"] - reference["ref_pose_arm_angle_deg"]) > 15.0:
            problems.append("reference pose differs from the mannequin's (arm %.0f vs %.0f deg): a retarget pose is needed"
                            % (d["ref_pose_arm_angle_deg"], reference["ref_pose_arm_angle_deg"]))
        verdict = "DIRECT (same skeleton)" if same_skeleton and not problems else ("RETARGET" if not d["missing_required"] else "RETARGET + BONE MAPPING")
        verdicts.append({"mesh": path, "verdict": verdict, "same_skeleton": same_skeleton, "height_cm": d["height_cm"],
                         "problems": problems, "missing_recommended": d["missing_recommended"], "proportions": d.get("proportions"),
                         "ref_pose_arm_angle_deg": d.get("ref_pose_arm_angle_deg")})
        unreal.log("SPN contract: %s -> %s %s" % (path, verdict, "; ".join(problems)))
    with open(os.path.join(OUT_DIR, "skeleton_check.json"), "w") as f:
        json.dump(verdicts, f, indent=1, default=str)
    unreal.log("SPN contract: reference %s, %d bones, %.1f cm, arm %.1f deg, proportions %s" % (
        reference["skeleton"], reference["bone_count"], reference["height_cm"], reference.get("ref_pose_arm_angle_deg", -1), reference.get("proportions")))


main()
