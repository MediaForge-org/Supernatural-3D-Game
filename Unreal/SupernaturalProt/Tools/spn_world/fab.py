"""Fab / Quixel assets the developer added to the project (Fab window, "Add to Project"). Not in git: see
Docs/Development/Fab-Asset-Sources.md for listing, licence and the import step.

Every function degrades gracefully: when an asset is missing the generator keeps its procedural placeholder, so the world still builds
on a machine without the Fab content. Nothing under /Game/Fab or /Game/Megaplant_Library is created, renamed or deleted here; the only
change to those assets is the material usage flags the editor itself would set when they are placed (Nanite, instanced meshes).
"""

import unreal

import ue_kit as k

MEGA = "/Game/Megaplant_Library"
MS = "/Game/Fab/Megascans"
DERIVED = k.ROOT + "/FabDerived"          # our own copies (Nanite fern meshes); generated, gitignored like all .uasset

SPRUCE = {v: MEGA + "/Tree_Norway_Spruce/Tree_Norway_Spruce_01/Tree_Norway_Spruce_01_" + v for v in "ABCD"}
BEECH = {v: MEGA + "/Tree_European_Beech/Tree_European_Beech_01/Tree_European_Beech_01_" + v for v in "ABCD"}
FERN = {v: MS + "/Plants/Lady_Fern_wdvlditia/High/wdvlditia_tier_1/StaticMeshes/SM_wdvlditia_Var" + v for v in "ABCDEFGHI"}
STUMP = MS + "/3D/Damaged_Tree_Stump_rcrwm/High/rcrwm_tier_1/StaticMeshes/rcrwm_tier_1"
ROCKS = MS + "/3D/Mossy_Rock_Cluster_regsH/High/regsH_tier_1/StaticMeshes/regsH_tier_1"
LOG = MS + "/3D/Mossy_Wooden_Log_rhfdj/High/rhfdj_tier_1/StaticMeshes/rhfdj_tier_1"
MOSSY_FLOOR = MS + "/Surfaces/Mossy_Forest_Floor_wjvpahk/High/wjvpahk_tier_1/Textures/T_wjvpahk_4K_"
ROCKY_FLOOR = MS + "/Surfaces/Rocky_Forest_Floor_vl0lecrlw/High/vl0lecrlw_tier_1/Textures/T_vl0lecrlw_4K_"

# landscape layer -> (texture prefix, tile size in cm = the scanned area)
FLOORS = {"Forest": (MOSSY_FLOOR, 200.0), "Dirt": (ROCKY_FLOOR, 250.0), "Rock": (ROCKY_FLOOR, 320.0)}   # steep cuts: needles, roots and
                                                                                                            # stones instead of smooth grey rock

# Megaplants: trunk collider radius per variant at scale 1 (0 = too thin to block the player)
TRUNK = {"spruce_A": 32.0, "spruce_B": 27.0, "spruce_C": 18.0, "spruce_D": 0.0, "beech_A": 30.0, "beech_B": 24.0, "beech_C": 0.0, "beech_D": 0.0}

_cache = {}
_reported = set()


def _exists(path):
    return k.eal.does_asset_exist(path)


def asset(path):
    """The asset, or None (logged once) when the developer has not imported it."""
    if path not in _cache:
        _cache[path] = unreal.load_asset(path) if _exists(path) else None
        if _cache[path] is None and path not in _reported:
            _reported.add(path)
            k.log("fab: missing %s (procedural placeholder stays)" % path)
    return _cache[path]


def trees():
    """{'spruce_A': SkeletalMesh, ...} of the imported Megaplants; empty when none are there or Nanite Foliage is off."""
    found = {}
    for kind, table in (("spruce", SPRUCE), ("beech", BEECH)):
        for variant, path in table.items():
            mesh = asset(path)
            if mesh is not None:
                found["%s_%s" % (kind, variant)] = mesh
    return found


def ensure_usage(material, usages):
    """Sets missing usage flags on a Fab master material and saves it (the editor does the same on first placement).
    Without them a Nanite or instanced mesh renders with the default material in a -game run."""
    base = material
    while isinstance(base, unreal.MaterialInstance):
        base = base.get_editor_property("parent")
    if base is None:
        return
    changed = []
    for prop, usage in usages:
        if not base.get_editor_property(prop):
            k.mel.set_material_usage(base, usage)
            changed.append(prop)
    if changed:
        k.eal.save_loaded_asset(base)
        k.log("fab: usage %s set on %s" % (", ".join(changed), base.get_path_name()))


NANITE_ISM = (("used_with_nanite", unreal.MaterialUsage.MATUSAGE_NANITE), ("used_with_instanced_static_meshes", unreal.MaterialUsage.MATUSAGE_INSTANCED_STATIC_MESHES))


def static(path):
    """A Fab static mesh ready for instancing (usage flags checked), or None."""
    mesh = asset(path)
    if mesh is not None:
        for slot in mesh.static_materials:
            if slot.material_interface is not None:
                ensure_usage(slot.material_interface, NANITE_ISM)
    return mesh


def ferns():
    """Nanite copies of the Lady Fern variants in DERIVED (the originals have LODs and a billboard, no Nanite). Reused once made."""
    result = {}
    for variant, path in FERN.items():
        source = static(path)
        if source is None:
            continue
        target = DERIVED + "/SM_Fern_" + variant
        copy = unreal.load_asset(target) if _exists(target) else k.eal.duplicate_asset(path, target)
        if copy is None:
            k.log("fab: could not copy %s" % path)
            continue
        settings = copy.get_editor_property("nanite_settings")
        if not settings.enabled:                       # new copy, or one left without Nanite by an interrupted build
            settings.set_editor_property("enabled", True)
            settings.set_editor_property("shape_preservation", unreal.NaniteShapePreservation.PRESERVE_AREA)
            copy.set_editor_property("nanite_settings", settings)     # set_editor_property runs PostEditChange: the mesh is rebuilt
            if not copy.get_editor_property("nanite_settings").enabled:
                k.fail("fab: Nanite could not be enabled on " + target)
            k.eal.save_loaded_asset(copy)
        result[variant] = copy
    if result:
        k.log("fab: %d fern variants (Nanite copies in %s)" % (len(result), DERIVED))
    return result


def floor(layer):
    """(color, normal, orm textures, tile cm) of the scanned surface for a landscape layer, or None."""
    entry = FLOORS.get(layer)
    if entry is None:
        return None
    prefix, tile = entry
    textures = [asset(prefix + suffix) for suffix in ("B", "N", "ORM")]
    if any(t is None for t in textures):
        return None
    return textures[0], textures[1], textures[2], tile
