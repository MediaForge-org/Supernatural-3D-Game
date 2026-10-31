"""Apply skin, eyes, body and groom settings from a JSON config to a MetaHuman Character asset (UE 5.8 Python API).

Hair, eyebrows, beard and mustache are wardrobe items in their own slots, so they stay exchangeable and are never baked
into the face. Body values are raw MetaHuman body-constraint measurements (cm / model units) and are PLACEHOLDERS.
  spn_mh_apply_appearance.py <asset> <config.json>
"""

import json
import os
import sys

import unreal

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spn_mh_editor as E  # noqa: E402

GROOM_DIR = "/MetaHumanCharacter/Optional/Grooms/Bindings"


def select_groom(ch, slot, folder, name):
    col = E.MHS.get_preview_collection(ch)
    wi = unreal.load_asset(f"{GROOM_DIR}/{folder}/WI_{name}")
    if wi is None:
        raise RuntimeError(f"wardrobe item not found: {folder}/WI_{name}")
    keys = list(col.get_item_keys_for_wardrobe_item(wi))
    key = keys[0] if keys else col.try_add_item_from_wardrobe_item(slot, wi)
    col.default_instance.set_single_slot_selection(slot, key)
    return key


def main(argv):
    asset, cfg = argv[0], json.load(open(argv[1], encoding="utf-8"))
    ch = E.open_for_edit(asset)

    skin = ch.skin_settings
    for k, v in cfg.get("skin", {}).items():
        setattr(skin.skin, k, v)
    for k, v in cfg.get("freckles", {}).items():
        setattr(skin.freckles, k, v)
    for region, vals in cfg.get("accents", {}).items():
        acc = getattr(skin.accents, region)
        for k, v in vals.items():
            setattr(acc, k, v)
        setattr(skin.accents, region, acc)
    E.MHS.commit_skin_settings(ch, skin)

    eyes = ch.eyes_settings
    for side in ("eye_left", "eye_right"):
        e = getattr(eyes, side)
        for k, v in cfg.get("iris", {}).items():
            setattr(e.iris, k, v)
        setattr(eyes, side, e)
    E.MHS.commit_eyes_settings(ch, eyes)

    if cfg.get("body"):
        cons = E.MHS.get_body_constraints(ch)
        for c in cons:
            if str(c.name) in cfg["body"]:
                c.is_active = True
                c.target_measurement = float(cfg["body"][str(c.name)])
        E.MHS.set_body_constraints(ch, list(cons))
        E.MHS.commit_body_state(ch)

    keys = {slot: select_groom(ch, slot, folder, name) for slot, (folder, name) in cfg.get("grooms", {}).items()}
    E.MHS.on_edit_preview_collection(ch)
    E.MHS.assemble_for_preview(ch)
    inst = E.MHS.get_preview_collection(ch).default_instance
    for slot, params in cfg.get("groom_params", {}).items():
        if slot not in keys:
            continue
        plist = inst.get_instance_parameters(item_path=unreal.MetaHumanPaletteItemPath(item_key=keys[slot]))
        for pname, val in params.items():
            p = next((p for p in plist if str(p.name) == pname), None)
            if p is None or not p.set_float(value=val):
                print(f"[spn_mh_apply_appearance] WARNING: {slot}.{pname} not set (available: {[str(q.name) for q in plist]})")
    E.MHS.on_edit_preview_collection(ch)
    E.MHS.assemble_for_preview(ch)
    unreal.EditorAssetLibrary.save_asset(asset, only_if_is_dirty=False)
    print(f"[spn_mh_apply_appearance] applied {argv[1]} to {asset}")


if __name__ == "__main__":
    main(sys.argv[1:])
