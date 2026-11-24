"""Protagonist V1 feasibility phase, task 1 (developer order 2026-11-14): a consistent reference board of the head views.

Run with Blender's own Python (numpy is bundled; nothing is installed):
    blender.exe --background --factory-startup --python Tools/protagonist/reference_board.py -- <out_dir>

Reads the three ORIGINAL sheets in References/Characters/Protagonist read-only and never writes there. For every usable head view it
  * crops the panel,
  * scales it so the eye line -> chin distance is the same (200 px) and puts the eye line at the same height,
  * neutralises the grading: grey-world white balance + a 1..99 % luminance stretch (the sheets have strong dusk light and orange
    grading; this does NOT recover albedo, it only makes the views comparable),
  * writes tile_<id>_color.png and tile_<id>_gray.png plus landmarks.json (the hand-placed points used, +-3..5 px on the sheet).
The board layout and the labels are drawn afterwards by compose_board.ps1 (System.Drawing).

Landmarks are hand-placed estimates (2026-11-14) on the 1536 x 1024 sheets: eye = midpoint between the pupils (front / 3/4) or the visible
eye (profile), chin = lowest point of the chin under the beard. They are the measurement basis of the conflict list in the report.
"""

import json
import os
import sys

import bpy
import numpy as np

REPO = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
REF = os.path.join(REPO, "References", "Characters", "Protagonist")
SHEETS = {
    "key": "protagonist_v1_reference.png",
    "face": "protagonist_v1_face_sheet.png",
    "body": "protagonist_v1_fullbody_sheet.png",
}
# id, sheet, panel crop (x0, y0, x1, y1), eye (x, y), chin y, view, role
VIEWS = [
    # eye y corrected from 230 to 210 on 2026-11-14: the engine's landmark tracker found the eye line 20 sheet px higher than the hand estimate
    ("key_34", "key", (380, 40, 900, 520), (607, 210), 365, "3/4 to his left (key art)", "identity master"),
    ("fs_front", "face", (0, 0, 238, 400), (112, 160), 292, "front", "geometry"),
    ("fb_front", "body", (0, 0, 196, 240), (112, 102), 182, "front", "geometry"),
    ("fb_front_neutral", "body", (200, 0, 390, 240), (290, 105), 184, "front, neutral expression", "geometry"),
    ("fs_34_left", "face", (243, 0, 474, 400), (321, 152), 275, "3/4 to his right", "geometry"),
    ("fb_34_left", "body", (392, 0, 584, 240), (475, 103), 180, "3/4 to his right", "geometry"),
    ("fs_34_right", "face", (703, 0, 918, 400), (853, 141), 255, "3/4 to his left, head raised", "geometry (tilted)"),
    ("fb_34_right", "body", (588, 0, 782, 240), (705, 100), 175, "3/4 to his left", "geometry"),
    ("fs_profile_l", "face", (476, 0, 700, 400), (510, 150), 265, "profile, facing image-left", "geometry (depth)"),
    ("fs_profile_r", "face", (921, 0, 1128, 400), (1095, 142), 257, "profile, facing image-right", "geometry (depth)"),
    ("fb_profile_a", "body", (786, 0, 966, 240), (932, 112), 172, "profile 'LEFT' (faces image-right)", "geometry (depth)"),
    ("fb_profile_b", "body", (970, 0, 1160, 240), (1120, 107), 172, "profile 'RIGHT' (ALSO faces image-right)", "geometry (depth)"),
]
TILE_W, TILE_H = 420, 560
EYE_Y, EYE_TO_CHIN = 230, 200


def load(path):
    img = bpy.data.images.load(path, check_existing=True)
    w, h = img.size
    px = np.array(img.pixels[:], dtype=np.float32).reshape(h, w, 4)[::-1, :, :3]   # top-down, RGB, linear float
    return px


def save(arr, path):
    h, w = arr.shape[:2]
    img = bpy.data.images.new(os.path.basename(path), width=w, height=h, alpha=False)
    rgba = np.ones((h, w, 4), dtype=np.float32)
    rgba[:, :, :3] = np.clip(arr, 0.0, 1.0)
    img.pixels[:] = rgba[::-1].ravel()
    img.filepath_raw = path
    img.file_format = "PNG"
    img.save()
    bpy.data.images.remove(img)


def sample(src, xs, ys):
    """Bilinear sampling; outside the source -> dark grey."""
    h, w = src.shape[:2]
    x0 = np.floor(xs).astype(int); y0 = np.floor(ys).astype(int)
    fx = (xs - x0)[..., None]; fy = (ys - y0)[..., None]
    inside = (x0 >= 0) & (y0 >= 0) & (x0 < w - 1) & (y0 < h - 1)
    x0c = np.clip(x0, 0, w - 2); y0c = np.clip(y0, 0, h - 2)
    a = src[y0c, x0c]; b = src[y0c, x0c + 1]; c = src[y0c + 1, x0c]; d = src[y0c + 1, x0c + 1]
    out = (a * (1 - fx) + b * fx) * (1 - fy) + (c * (1 - fx) + d * fx) * fy
    out[~inside] = 0.08
    return out


def neutralise(tile, mask):
    """Grey-world white balance and a luminance stretch, measured on the face region only (mask)."""
    region = tile[mask]
    means = region.mean(axis=0) + 1e-6
    balanced = tile * (means.mean() / means)
    lum = balanced @ np.array([0.2126, 0.7152, 0.0722], dtype=np.float32)
    lo, hi = np.percentile(lum[mask], [1, 99])
    scale = 1.0 / max(hi - lo, 1e-3)
    out = np.clip((balanced - lo) * scale, 0, 1)
    gray = np.clip((lum - lo) * scale, 0, 1)
    return out, np.repeat(gray[..., None], 3, axis=2)


def main():
    out_dir = sys.argv[sys.argv.index("--") + 1] if "--" in sys.argv else os.path.join(REPO, "Artifacts", "VisualProgress", "ProtagonistV1_Feasibility", "board")
    os.makedirs(out_dir, exist_ok=True)
    sheets = {k: load(os.path.join(REF, v)) for k, v in SHEETS.items()}
    meta = []
    for vid, sheet, crop, eye, chin_y, view, role in VIEWS:
        src = sheets[sheet]
        x0, y0, x1, y1 = crop
        panel = np.zeros_like(src); panel[:] = 0.08
        panel[y0:y1, x0:x1] = src[y0:y1, x0:x1]   # never sample a neighbouring panel
        scale = EYE_TO_CHIN / float(chin_y - eye[1])
        ys, xs = np.mgrid[0:TILE_H, 0:TILE_W].astype(np.float32)
        sx = eye[0] + (xs - TILE_W / 2) / scale
        sy = eye[1] + (ys - EYE_Y) / scale
        tile = sample(panel, sx, sy)
        mask = np.zeros((TILE_H, TILE_W), dtype=bool)
        mask[EYE_Y - 90:EYE_Y + EYE_TO_CHIN, TILE_W // 2 - 110:TILE_W // 2 + 110] = True
        color, gray = neutralise(tile, mask)
        # guide lines: eye line, nose base (~ 45 % eye->chin), mouth (~ 70 %), chin
        for frac, col in ((0.0, (0.1, 1.0, 0.1)), (0.45, (1.0, 0.8, 0.1)), (0.7, (1.0, 0.8, 0.1)), (1.0, (0.1, 1.0, 0.1))):
            y = int(EYE_Y + frac * EYE_TO_CHIN)
            for arr in (color, gray):
                arr[y, ::3] = col
        save(color, os.path.join(out_dir, "tile_%s_color.png" % vid))
        save(gray, os.path.join(out_dir, "tile_%s_gray.png" % vid))
        meta.append({"id": vid, "sheet": SHEETS[sheet], "crop": crop, "eye": eye, "chin_y": chin_y, "eye_to_chin_px_on_sheet": chin_y - eye[1],
                     "scale": round(scale, 3), "view": view, "role": role})
    with open(os.path.join(out_dir, "landmarks.json"), "w", encoding="utf-8") as f:
        json.dump({"status": "hand-placed estimates, +-3..5 px on the sheet; originals untouched", "views": meta}, f, indent=1)
    print("SPN reference board: %d views -> %s" % (len(meta), out_dir))


main()
