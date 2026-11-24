"""Protagonist V1 phase 2, task B (2026-11-15): why do the feet slide?  Read-only diagnosis of the player's locomotion animations.

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file> -unattended -nullrhi
For every clip the player's anim blueprint uses (asset registry, through the blend spaces):
  * length, frames, root motion on/off;
  * the stance speed the clip was authored for: the ball bone's horizontal speed in component space while it is the lower foot and near
    its lowest point (an in-place clip moves the planted foot backwards at exactly the speed the character must move for no sliding);
  * the stride length (distance the planted foot travels per stance) and the cadence.
For every blend space: axes, ranges, samples (clip + coordinates) and the "axis to scale animation" (play-rate scaling by speed).
Writes Saved/SpnCharacter/locomotion_stride.json. Changes nothing.
"""

import json
import math
import os

import unreal

ABP = "/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"
SAVED = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter")
reg = unreal.AssetRegistryHelpers.get_asset_registry()
opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True)


def used_assets():
    seen, todo, out = set(), [ABP], {"AnimSequence": [], "BlendSpace": [], "BlendSpace1D": []}
    while todo:
        p = todo.pop()
        if p in seen or not p.startswith("/Game"):
            continue
        seen.add(p)
        for a in reg.get_assets_by_package_name(p):
            cls = str(a.asset_class_path.asset_name)
            if cls in out:
                out[cls].append(p)
            if cls in ("AnimSequence", "BlendSpace", "BlendSpace1D", "AnimBlueprint", "AnimMontage") or p == ABP:
                todo.extend(str(d) for d in (reg.get_dependencies(p, opts) or []))
    return {k: sorted(set(v)) for k, v in out.items()}


def bone_track(anim, bone, frames, length):
    pts = []
    eval_opts = unreal.AnimPoseEvaluationOptions()
    for i in range(frames):
        t = length * i / max(frames - 1, 1)
        pose = unreal.AnimPoseExtensions.get_anim_pose_at_time(anim, t, eval_opts)
        tr = unreal.AnimPoseExtensions.get_bone_pose(pose, bone, unreal.AnimPoseSpaces.WORLD)
        loc = tr.translation
        pts.append((t, loc.x, loc.y, loc.z))
    return pts


def stance_speed(track_l, track_r):
    """Horizontal speed of the lower foot while it is within 2.5 cm of its lowest height (= planted in an in-place clip)."""
    speeds, strides = [], []
    for track in (track_l, track_r):
        zmin = min(p[3] for p in track)
        run, run_len = [], 0.0
        for a, b in zip(track, track[1:]):
            planted = a[3] < zmin + 2.5 and b[3] < zmin + 2.5
            dt = b[0] - a[0]
            if planted and dt > 0:
                d = math.hypot(b[1] - a[1], b[2] - a[2])
                speeds.append(d / dt)
                run_len += d
            elif run_len > 0:
                strides.append(run_len)
                run_len = 0.0
        if run_len > 0:
            strides.append(run_len)
    speeds.sort()
    med = speeds[len(speeds) // 2] if speeds else None
    return med, (sum(strides) / len(strides) if strides else None), len(speeds)


def main():
    used = used_assets()
    report = {"anim_blueprint": ABP, "clips": [], "blend_spaces": []}
    for path in used["AnimSequence"]:
        anim = unreal.load_asset(path)
        length = anim.get_play_length()
        frames = unreal.AnimationLibrary.get_num_frames(anim)
        row = {"clip": path.split("/")[-1], "length_s": round(length, 3), "frames": frames}
        try:
            row["root_motion"] = bool(anim.get_editor_property("enable_root_motion"))
        except Exception:
            pass
        try:
            row["rate_scale"] = float(anim.get_editor_property("rate_scale"))
        except Exception:
            pass
        try:
            n = max(frames, 2) * 2
            l = bone_track(anim, "ball_l", n, length)
            r = bone_track(anim, "ball_r", n, length)
            spd, stride, samples = stance_speed(l, r)
            row["stance_speed_cm_s"] = round(spd, 1) if spd is not None else None
            row["stride_cm"] = round(stride, 1) if stride is not None else None
            row["stance_samples"] = samples
            root = bone_track(anim, "root", 2, length)
            dist = math.hypot(root[-1][1] - root[0][1], root[-1][2] - root[0][2])
            # authored travel speed of the clip (root motion over one loop); with root motion ignored by the anim blueprint this is the speed
            # the clip "expects" the capsule to move at - any other capsule speed shows as foot sliding
            row["root_speed_cm_s"] = round(dist / length, 1) if length > 0 else None
            row["root_travel_cm"] = round(dist, 1)
        except Exception as e:  # noqa: BLE001
            row["error"] = str(e)[:200]
        report["clips"].append(row)
    for path in used["BlendSpace"] + used["BlendSpace1D"]:
        bs = unreal.load_asset(path)
        row = {"blend_space": path.split("/")[-1]}
        for prop in ("axis_to_scale_animation", "target_weight_interpolation_speed_per_sec", "b_interpolate_using_grid", "blend_parameters"):
            try:
                row[prop] = str(bs.get_editor_property(prop))[:400]
            except Exception:
                pass
        try:
            row["samples"] = [{"anim": str(s.get_editor_property("animation").get_name()), "value": str(s.get_editor_property("sample_value")),
                               "rate": float(s.get_editor_property("rate_scale"))} for s in bs.get_editor_property("sample_data")]
        except Exception as e:  # noqa: BLE001
            row["samples_error"] = str(e)[:200]
        report["blend_spaces"].append(row)
    # retarget op API for the fix (task B)
    try:
        report["add_retarget_op_doc"] = unreal.IKRetargeterController.add_retarget_op.__doc__
    except Exception as e:  # noqa: BLE001
        report["op_doc_error"] = str(e)
    report["op_structs"] = [n for n in dir(unreal) if n.startswith("IKRetarget") and n.endswith("Op")]
    os.makedirs(SAVED, exist_ok=True)
    with open(os.path.join(SAVED, "locomotion_stride.json"), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, default=str)
    unreal.log("SPN stride check: %d clips, %d blend spaces" % (len(report["clips"]), len(report["blend_spaces"])))


main()
