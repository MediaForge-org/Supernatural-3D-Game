"""Iterative landmark fit of a MetaHuman Character asset to ONE reference photo (UE 5.8 MetaHuman Creator Python API).

There is no photo-to-MetaHuman function in UE 5.8. This script approximates the face by analysis-by-synthesis:
  1. fit a camera (Levenberg-Marquardt) so the character's 3D face landmarks project onto 2D targets in the reference:
     hand-annotated points (annotations JSON, low-confidence ones weighted 0.5) + eye/mouth corners from the tracker,
  2. render the character with that camera, run the MetaHuman contour tracker on render and reference, and take the
     per-landmark offset between same-index contour points (eyes, lips, nasolabial folds),
  3. turn the 2D residuals into 3D image-plane displacements, symmetrise them, translate_face_landmarks + commit_face_state,
  4. repeat; the best iteration (score = hypot(annotation RMS, contour RMS)) is restored at the end.
A single view cannot recover depth (eye depth, brow ridge, nose projection seen from the side): those stay manual.
Coefficients before/after are written next to the renders so a run can be undone with set_face_model_coefficients.

Run inside the editor (remote execution or Tools > Execute Python Script with args):
  spn_mh_fit_face.py <asset> <reference.png> <annotations.json> [iterations=6] [alpha=0.7] [fov=20] [tag=fit]
The character must not be open in the MetaHuman Creator window at the same time.
"""

import json
import math
import os
import sys

import unreal

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spn_mh_editor as E  # noqa: E402
import spn_mh_fit_math as M  # noqa: E402

W, H = 1536, 1024
# MetaHuman 5.8 face landmark indices (character space, verified on renders 2026-10-31; see Docs/ProtagonistV1/README.md)
CORNERS = {22: "eye_r_minx", 33: "eye_r_maxx", 19: "eye_l_minx", 18: "eye_l_maxx", 48: "mouth_minx", 32: "mouth_maxx"}
CURVE_GROUPS = (
    ([22, 33, 44, 51, 52, 53], ("crv_eyelid_upper_r", "crv_eyelid_lower_r")),
    ([18, 19, 20, 35, 36, 37], ("crv_eyelid_upper_l", "crv_eyelid_lower_l")),
    ([5, 47, 31, 48, 32, 49, 34, 50, 4, 9, 16], ("crv_lip_",)),
    ([11, 17], ("crv_nasolabial_r", "crv_nasolabial_l")),
)


def main(argv):
    asset, ref_path, ann_path = argv[0], argv[1], argv[2]
    iters = int(argv[3]) if len(argv) > 3 else 6
    alpha = float(argv[4]) if len(argv) > 4 else 0.7
    fov = float(argv[5]) if len(argv) > 5 else 20.0
    tag = argv[6] if len(argv) > 6 else "fit"
    out = E.out_dir()
    ann_doc = json.load(open(ann_path, encoding="utf-8"))
    ann = {int(k): v for k, v in ann_doc["points"].items()}
    low = set(int(k) for k in ann_doc.get("low_confidence", []))

    ref_curves = E.track(ref_path)
    if not ref_curves:
        raise RuntimeError("tracker found no face in the reference image")
    ref_ext = M.curve_extremes(ref_curves)
    ch = E.open_for_edit(asset)
    E.ensure_lights()
    E.spawn_preview(ch)
    json.dump(list(E.MHS.get_face_model_coefficients(ch)), open(os.path.join(out, f"coeffs_{tag}_before.json"), "w"))

    idx = list(ann) + list(CORNERS)
    tgt = [ann[i] for i in ann] + [ref_ext[CORNERS[i]] for i in CORNERS]
    wts = [0.5 if i in low else 1.0 for i in ann] + [1.0] * len(CORNERS)
    cam, best, log = None, None, []
    for it in range(iters + 1):
        lm = E.landmarks(ch)
        if cam is None:
            cen = tuple(sum(p[k] for p in lm) / len(lm) for k in range(3))
            fx = M.focal_px(W, fov)
            cands = []
            for yaw in (-50, -35, -20, 20, 35, 50):
                c0 = M.look_at_cam((0, cen[1] + 4, cen[2]), yaw, -5, 120 * 30.0 / fov)
                ax, ay = sum(t[0] for t in tgt) / len(tgt), sum(t[1] for t in tgt) / len(tgt)
                c0[4] += math.degrees(math.atan((ax - W / 2) / fx))
                c0[3] -= math.degrees(math.atan((ay - H / 2) / fx))
                cands.append(M.fit_camera(c0, [lm[i] for i in idx], tgt, wts, W, H, fov))
            cam = min(cands, key=lambda c: c[1])[0]
        cam, rms_ann = M.fit_camera(cam, [lm[i] for i in idx], tgt, wts, W, H, fov)

        img = E.capture(out, f"{tag}_it{it}.png", cam, fov, W, H)
        rendered = E.track(img)
        proj = M.project(lm, cam, W, H, fov)
        targets = {i: (t, 0.5 if i in low else 1.0) for i, t in zip(idx, tgt)}
        errs = []
        for lms, prefixes in CURVE_GROUPS:
            r = {k: v for k, v in rendered.items() if k.startswith(prefixes) and "philtrum" not in k}
            f = {k: v for k, v in ref_curves.items() if k.startswith(prefixes) and "philtrum" not in k}
            for i in lms:
                off = M.curve_offset(proj[i][:2], r, f)
                if off is None:
                    continue
                errs.append(math.hypot(*off))
                if i not in targets or i in CORNERS:
                    targets[i] = ((proj[i][0] + off[0], proj[i][1] + off[1]), 1.0)
        rms_curve = math.sqrt(sum(e * e for e in errs) / max(1, len(errs)))
        score = math.hypot(rms_ann, rms_curve)
        if best is None or score < best[0]:
            best = (score, it, list(E.MHS.get_face_model_coefficients(ch)), list(cam))
        log.append({"it": it, "rms_annotations_px": rms_ann, "rms_contours_px": rms_curve, "score": score, "curves_tracked": len(rendered)})
        print(f"[spn_mh_fit_face] it {it}: annotations {rms_ann:.2f}px contours {rms_curve:.2f}px score {score:.2f}")
        if it == iters:
            break

        deltas_px = {i: (t[0] - proj[i][0], t[1] - proj[i][1]) for i, (t, _) in targets.items()}
        weights = {i: w for i, (_, w) in targets.items()}
        world = M.image_to_world_deltas(deltas_px, {i: proj[i][2] for i in targets}, cam, W, fov, alpha)
        world = {i: tuple(weights[i] * c for c in d) for i, d in world.items()}
        final = M.symmetric_deltas(world, M.mirror_pairs(lm))
        ids = sorted(final)
        E.MHS.translate_face_landmarks(ch, ids, [unreal.Vector(*final[i]) for i in ids])
        E.MHS.commit_face_state(ch)

    score, best_it, coeffs, best_cam = best
    if best_it != it:
        E.MHS.set_face_model_coefficients(ch, coeffs)
        E.MHS.commit_face_state(ch)
    json.dump(coeffs, open(os.path.join(out, f"coeffs_{tag}_best.json"), "w"))
    json.dump({"asset": asset, "fov": fov, "cam": best_cam, "best_iteration": best_it, "log": log},
              open(os.path.join(out, f"fit_{tag}.json"), "w"), indent=1)
    unreal.EditorAssetLibrary.save_asset(asset, only_if_is_dirty=False)
    print(f"[spn_mh_fit_face] kept iteration {best_it} (score {score:.2f}); results in {out}")


if __name__ == "__main__":
    main(sys.argv[1:])
