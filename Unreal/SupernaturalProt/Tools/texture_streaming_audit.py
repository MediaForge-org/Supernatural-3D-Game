"""READ-ONLY: streaming settings of every texture the generated world uses (night build 2026-11-14, texture pop-in diagnosis).

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Walks the package dependencies of /Game/SpnWorld/Lvl_SpnWorld (hard and soft, recursively) and writes
Saved/SpnWorldGen/texture_streaming_audit.json: per texture virtual-texture streaming, NeverStream, LOD group, size, mip generation,
LOD bias, compression; totals per folder. Saves nothing.
"""

import json
import os
from collections import Counter, defaultdict

import unreal

LEVEL = "/Game/SpnWorld/Lvl_SpnWorld"
OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnWorldGen", "texture_streaming_audit.json")


def deps_of(reg, package):
    opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True,
                                                 include_searchable_names=False, include_soft_management_references=False,
                                                 include_hard_management_references=False)
    return reg.get_dependencies(package, opts) or []


def main():
    reg = unreal.AssetRegistryHelpers.get_asset_registry()
    seen, todo = set(), [LEVEL]
    while todo:
        p = todo.pop()
        if p in seen or not str(p).startswith("/Game"):
            continue
        seen.add(p)
        todo.extend(str(d) for d in deps_of(reg, p))
    textures = []
    for p in sorted(seen):
        for a in reg.get_assets_by_package_name(p):
            cls = str(a.asset_class_path.asset_name)
            if cls not in ("Texture2D", "TextureCube", "Texture2DArray"):
                continue
            t = unreal.EditorAssetLibrary.load_asset(str(a.package_name) + "." + str(a.asset_name))
            if t is None:
                continue
            def g(name, default=None):
                try:
                    return t.get_editor_property(name)
                except Exception:
                    return default
            w = g("blueprint_get_size_x") if False else None
            try:
                w, h = t.blueprint_get_size_x(), t.blueprint_get_size_y()
            except Exception:
                w = h = 0
            textures.append({
                "path": str(a.package_name), "class": cls, "w": w, "h": h,
                "vt": bool(g("virtual_texture_streaming", False)), "never_stream": bool(g("never_stream", False)),
                "lod_group": str(g("lod_group")), "mip_gen": str(g("mip_gen_settings")), "lod_bias": g("lod_bias", 0),
                "compression": str(g("compression_settings")), "max_size": g("max_texture_size", 0),
            })
    by_folder = defaultdict(Counter)
    for t in textures:
        folder = "/".join(t["path"].split("/")[:3])
        c = by_folder[folder]
        c["textures"] += 1
        c["vt"] += t["vt"]
        c["never_stream"] += t["never_stream"]
        c["no_mips"] += "NOMIPMAPS" in t["mip_gen"].upper()
        c["4k_or_more"] += max(t["w"], t["h"]) >= 4096
        c["lod_bias_nonzero"] += 1 if t["lod_bias"] else 0
    report = {"level": LEVEL, "packages": len(seen), "textures": len(textures),
              "by_folder": {k: dict(v) for k, v in sorted(by_folder.items())},
              "lod_groups": dict(Counter(t["lod_group"] for t in textures)), "list": textures}
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1)
    unreal.log("texture_streaming_audit: %d textures in %d packages -> %s" % (len(textures), len(seen), OUT))


main()
