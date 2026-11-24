"""Materials of the placeholder world: one surface master (PBR texture set, tint, macro variation, vertex-colour grime, wetness) with
instances, plus landscape, window, glass, lamp, road marking, water, foliage and grass masters. Built from scratch on every run."""

import unreal

import fab
import ue_kit as k
from ue_kit import Graph, MP, MAT, TEX

MACRO = "/Engine/EngineMaterials/T_Default_MacroVariation"
WATER_N = "/Engine/Functions/Engine_MaterialFunctions02/ExampleContent/Textures/water_n"

# name: (texture set, tile cm, tint, roughness scale, flat colour used before the material phase)
SURFACES = {
    "asphalt": ("Asphalt", 400, (1, 1, 1), 1.0, (0.10, 0.10, 0.11)),
    "asphalt_old": ("AsphaltOld", 520, (1, 1, 1), 1.0, (0.16, 0.16, 0.17)),
    "concrete": ("Concrete", 300, (1, 1, 1), 1.0, (0.45, 0.45, 0.43)),
    "sidewalk": ("Sidewalk", 300, (1, 1, 1), 1.0, (0.48, 0.48, 0.46)),
    "kerb": ("Concrete", 180, (0.86, 0.86, 0.86), 1.0, (0.40, 0.40, 0.39)),
    "plaster": ("Plaster", 250, (0.93, 0.90, 0.82), 1.0, (0.72, 0.69, 0.60)),
    "plaster_grey": ("Plaster", 250, (0.62, 0.64, 0.63), 1.0, (0.42, 0.44, 0.43)),
    "plaster_ochre": ("Plaster", 250, (0.80, 0.62, 0.38), 1.0, (0.55, 0.40, 0.22)),
    "siding_white": ("Siding", 240, (0.95, 0.95, 0.92), 1.0, (0.80, 0.80, 0.76)),
    "siding_cream": ("Siding", 240, (0.92, 0.84, 0.64), 1.0, (0.74, 0.64, 0.42)),
    "siding_blue": ("Siding", 240, (0.50, 0.60, 0.68), 1.0, (0.28, 0.36, 0.44)),
    "siding_sage": ("Siding", 240, (0.58, 0.66, 0.52), 1.0, (0.34, 0.42, 0.30)),
    "siding_yellow": ("Siding", 240, (0.90, 0.78, 0.44), 1.0, (0.72, 0.58, 0.24)),
    "siding_grey": ("Siding", 240, (0.60, 0.60, 0.58), 1.0, (0.38, 0.38, 0.36)),
    "siding_red": ("Siding", 240, (0.56, 0.20, 0.16), 1.0, (0.36, 0.09, 0.07)),
    "siding_brown": ("Siding", 240, (0.45, 0.33, 0.24), 1.0, (0.24, 0.16, 0.10)),
    "trim": ("WallPaint", 200, (0.96, 0.96, 0.93), 0.8, (0.85, 0.85, 0.82)),
    "trim_dark": ("WallPaint", 200, (0.22, 0.25, 0.24), 0.8, (0.10, 0.12, 0.11)),
    "brick": ("Brick", 180, (1, 1, 1), 1.0, (0.33, 0.14, 0.10)),
    "brick_dark": ("Brick", 180, (0.62, 0.58, 0.58), 1.0, (0.20, 0.10, 0.08)),
    "brick_tan": ("Brick", 180, (1.15, 1.05, 0.85), 1.0, (0.42, 0.30, 0.20)),
    "shingles": ("Shingles", 240, (1, 1, 1), 1.0, (0.07, 0.07, 0.07)),
    "shingles_brown": ("Shingles", 240, (1.5, 1.05, 0.8), 1.0, (0.13, 0.08, 0.05)),
    "shingles_green": ("Shingles", 240, (0.85, 1.25, 0.95), 1.0, (0.06, 0.10, 0.07)),
    "corrugated": ("Corrugated", 152, (1, 1, 1), 1.0, (0.36, 0.38, 0.40)),
    "metal_white": ("PaintedMetal", 200, (1.0, 1.0, 0.98), 1.0, (0.75, 0.75, 0.73)),
    "metal_red": ("PaintedMetal", 200, (0.75, 0.10, 0.08), 1.0, (0.50, 0.04, 0.03)),
    "metal_green": ("PaintedMetal", 200, (0.16, 0.36, 0.22), 1.0, (0.06, 0.18, 0.09)),
    "metal_blue": ("PaintedMetal", 200, (0.14, 0.26, 0.50), 1.0, (0.05, 0.10, 0.26)),
    "metal_yellow": ("PaintedMetal", 200, (0.92, 0.70, 0.10), 1.0, (0.70, 0.46, 0.03)),
    "metal_dark": ("PaintedMetal", 200, (0.12, 0.12, 0.13), 1.0, (0.04, 0.04, 0.05)),
    "steel": ("Steel", 150, (1, 1, 1), 1.0, (0.40, 0.40, 0.41)),
    "planks": ("Planks", 200, (1, 1, 1), 1.0, (0.24, 0.19, 0.14)),
    "planks_dark": ("Planks", 200, (0.55, 0.45, 0.38), 1.0, (0.11, 0.08, 0.06)),
    "planks_red": ("Planks", 200, (1.05, 0.38, 0.30), 1.0, (0.30, 0.07, 0.05)),
    "wood_floor": ("WoodFloor", 240, (1, 1, 1), 1.0, (0.30, 0.18, 0.09)),
    "gravel": ("Gravel", 250, (1, 1, 1), 1.0, (0.30, 0.28, 0.25)),
    "dirt": ("Dirt", 300, (0.82, 0.80, 0.76), 1.0, (0.16, 0.12, 0.08)),
    "rock": ("Rock", 400, (1, 1, 1), 1.0, (0.26, 0.25, 0.23)),
    "wall_paint": ("WallPaint", 200, (0.90, 0.87, 0.78), 1.0, (0.70, 0.66, 0.55)),
    "wallpaper": ("Wallpaper", 130, (0.86, 0.80, 0.62), 1.0, (0.55, 0.50, 0.36)),
    "carpet": ("Carpet", 150, (0.50, 0.22, 0.20), 1.0, (0.20, 0.07, 0.06)),
    "linoleum": ("Linoleum", 240, (1, 1, 1), 1.0, (0.35, 0.36, 0.30)),
    "ceiling": ("Ceiling", 200, (1, 1, 1), 1.0, (0.78, 0.78, 0.76)),
}
LAYERS = [("Grass", "GrassGround", 280, (0.16, 0.22, 0.07)), ("Forest", "ForestFloor", 260, (0.13, 0.10, 0.06)), ("Dirt", "Dirt", 330, (0.17, 0.13, 0.09)),
          ("Gravel", "Gravel", 260, (0.30, 0.28, 0.25)), ("Rock", "Rock", 600, (0.26, 0.25, 0.23))]
# colour multipliers of the textured landscape layers (night build 2026-11-14, visual pass): the scanned dirt read as pale sand along every road
# and under the lawn; PLACEHOLDER look values
LAYER_TINT = {"Grass": (0.64, 0.76, 0.46), "Dirt": (0.56, 0.52, 0.46), "Gravel": (0.62, 0.61, 0.57), "Forest": (0.80, 0.77, 0.70)}
TRACK_TINT = (0.46, 0.50, 0.50)      # MI_Spn_dirt when the scan is used (TRACK_USES_SCAN)
TRACK_USES_SCAN = False


def tex(name):
    return k.load(TEX + "/T_" + name)


def collection():
    path = MAT + "/MPC_SpnWorld"
    if k.eal.does_asset_exist(path):
        return unreal.load_asset(path)
    mpc = k.tools.create_asset("MPC_SpnWorld", MAT, unreal.MaterialParameterCollection, unreal.MaterialParameterCollectionFactoryNew())
    params = []
    for name, default in (("Wetness", 0.0), ("LampGlow", 0.0), ("WindowGlow", 0.0)):
        p = unreal.CollectionScalarParameter()
        p.set_editor_property("parameter_name", name)
        p.set_editor_property("default_value", default)
        params.append(p)
    mpc.set_editor_property("scalar_parameters", params)
    k.eal.save_loaded_asset(mpc)
    return mpc


def macro_factor(g, low=0.80, high=1.14, scale=1.0 / 2600.0):
    world = g.mask(g.node("WorldPosition"), "rg")
    sample = g.tex("Macro", k.load(MACRO), g.mul(world, scale), "color", param=False)
    big = g.tex("Macro2", k.load(MACRO), g.mul(world, scale * 0.13), "color", param=False)
    return g.lerp(low, high, g.mul(g.add((sample, "R"), (big, "G")), 0.5))


def surface_master(mpc):
    g = Graph("M_SpnSurface")
    uv = g.div(g.node("TextureCoordinate"), g.scalar("Tiling", 250.0))
    c = g.tex("BaseColor", tex("Concrete_C"), uv, "color")
    n = g.tex("Normal", tex("Concrete_N"), uv, "normal")
    o = g.tex("ORM", tex("Concrete_ORM"), uv, "masks")
    vc = g.node("VertexColor")
    base = g.mul(g.mul((c, "RGB"), g.vector("Tint", (1, 1, 1))), macro_factor(g))
    base = g.mul(base, g.mask(vc, "r"))
    grime = g.one("Saturate", g.mul(g.mask(vc, "g"), g.scalar("GrimeAmount", 0.6)))
    base = g.lerp(base, g.mul(base, g.vector("GrimeColor", (0.42, 0.36, 0.28))), grime)
    wet = g.mul(g.collection(mpc, "Wetness"), g.scalar("WetResponse", 0.6))
    base = g.lerp(base, g.mul(base, 0.68), wet)
    rough = g.mul((o, "G"), g.scalar("RoughScale", 1.0))
    rough = g.lerp(rough, g.mul(rough, 0.28), wet)
    g.out(base, MP.MP_BASE_COLOR)
    g.out(rough, MP.MP_ROUGHNESS)
    g.out(g.mul((o, "B"), g.scalar("MetalScale", 1.0)), MP.MP_METALLIC)
    g.out((o, "R"), MP.MP_AMBIENT_OCCLUSION)
    g.out(g.lerp(g.const((0, 0, 1)), (n, "RGB"), g.scalar("NormalStrength", 1.0)), MP.MP_NORMAL)
    return g.finish()


def track_master(mpc):
    """Earth track material (developer decision 2026-11-14: keep the earth, add variation with existing textures only).

    The road ribbon's U coordinate is the lateral offset in cm from the centre line (roads.strip), so the material knows where it is across the
    track: two compacted wheel ruts with a little gravel showing, needle litter on the hump between them and towards the verges, and litter
    patches broken up by the engine's macro-variation texture in world space. Textures: the generated Dirt, ForestFloor and Gravel sets that
    the world already uses. No new assets, no geometry, no vegetation. PLACEHOLDER look values (all parameters)."""
    g = Graph("M_SpnTrack")
    coord = g.node("TextureCoordinate")
    lateral = g.one("Abs", g.mask(coord, "r"))
    sets = {}
    for name, tiling in (("Dirt", 300.0), ("ForestFloor", 260.0), ("Gravel", 220.0)):
        uv = g.div(coord, g.scalar(name + "Tiling", tiling))
        sets[name] = (g.tex(name + "C", tex(name + "_C"), uv, "color", param=False), g.tex(name + "N", tex(name + "_N"), uv, "normal", param=False),
                      g.tex(name + "ORM", tex(name + "_ORM"), uv, "masks", param=False))
    world = g.mask(g.node("WorldPosition"), "rg")
    small = g.tex("PatchSmall", k.load(MACRO), g.mul(world, 1.0 / 900.0), "color", param=False)
    large = g.tex("PatchLarge", k.load(MACRO), g.mul(world, 1.0 / 3100.0), "color", param=False)
    noise = g.mul(g.add((small, "R"), (large, "G")), 0.5)
    # wheel ruts at +-RutOffset (linear falloff over RutWidth)
    rut = g.sub(1.0, g.one("Saturate", g.div(g.one("Abs", g.sub(lateral, g.scalar("RutOffset", 70.0))), g.scalar("RutWidth", 28.0))))
    # litter: towards the verges, on the hump between the ruts, and in patches anywhere
    edge = g.one("Saturate", g.div(g.sub(lateral, g.scalar("EdgeStart", 95.0)), g.scalar("EdgeFade", 55.0)))
    hump = g.mul(g.sub(1.0, g.one("Saturate", g.div(lateral, g.scalar("HumpWidth", 32.0)))), g.scalar("HumpLitter", 0.7))
    patches = g.mul(g.sub(noise, g.scalar("PatchThreshold", 0.52)), g.scalar("PatchContrast", 2.6))
    litter = g.one("Saturate", g.add(g._bin("Max", edge, hump), patches))
    litter = g.mul(litter, g.sub(1.0, g.mul(rut, 0.85)))
    gravel = g.mul(g.one("Saturate", g.mul(rut, g.sub(g.mul(noise, 1.8), 0.45))), g.scalar("GravelAmount", 0.55))

    def blend(index, channels=None):
        d, f, gr = sets["Dirt"][index], sets["ForestFloor"][index], sets["Gravel"][index]
        pick = (lambda n: (n, channels)) if channels else (lambda n: n)
        return g.lerp(g.lerp(pick(d), pick(f), litter), pick(gr), gravel)

    vc = g.node("VertexColor")
    base = g.mul(g.mul(blend(0, "RGB"), g.vector("Tint", (0.82, 0.80, 0.76))), macro_factor(g, 0.86, 1.10))
    base = g.mul(base, g.lerp(1.0, g.scalar("RutDarken", 0.84), rut))
    base = g.mul(base, g.mask(vc, "r"))
    wet = g.mul(g.collection(mpc, "Wetness"), g.scalar("WetResponse", 0.7))
    base = g.lerp(base, g.mul(base, 0.66), wet)
    orm = blend(2)
    rough = g.mul(g.mul(g.mask(orm, "g"), g.lerp(1.0, 0.86, rut)), g.scalar("RoughScale", 1.0))
    rough = g.lerp(rough, g.mul(rough, 0.3), wet)
    g.out(base, MP.MP_BASE_COLOR)
    g.out(rough, MP.MP_ROUGHNESS)
    g.out(g.const(0.0), MP.MP_METALLIC)
    g.out(g.mask(orm, "r"), MP.MP_AMBIENT_OCCLUSION)
    g.out(blend(1, "RGB"), MP.MP_NORMAL)
    return g.finish()


def flat_master():
    g = Graph("M_SpnFlat")
    vc = g.node("VertexColor")
    g.out(g.mul(g.vector("Color", (0.5, 0.5, 0.5)), g.mask(vc, "r")), MP.MP_BASE_COLOR)
    g.out(g.scalar("Roughness", 0.85), MP.MP_ROUGHNESS)
    return g.finish()


def window_master(mpc):
    """Opaque 'glass' of buildings without an interior: dark, glossy, drawn curtains; some windows glow at night (vertex colour B)."""
    g = Graph("M_SpnWindow")
    uv = g.node("TextureCoordinate")
    u = g.mask(uv, "r")
    vc = g.node("VertexColor")
    rnd = g.mask(vc, "b")
    side = g.one("Abs", g.sub(u, 0.5))
    opening = g.add(0.10, g.mul(rnd, 0.30))
    curtain = g.mul(g.one("Saturate", g.mul(g.sub(side, opening), 60.0)), g.scalar("CurtainAmount", 1.0))
    folds = g.add(0.8, g.mul(g.one("Sine", g.mul(u, 150.0)), 0.2))
    cloth = g.mul(g.mul(g.vector("CurtainColor", (0.50, 0.46, 0.38)), folds), g.add(0.6, g.mul(rnd, 0.5)))
    base = g.lerp(g.const((0.012, 0.015, 0.018)), g.mul(cloth, 0.35), curtain)
    lit = g.one("Saturate", g.mul(g.sub(rnd, g.scalar("LitThreshold", 0.55)), 50.0))
    glow = g.mul(g.mul(g.lerp(g.const((1.0, 0.62, 0.30)), g.mul(cloth, 1.6), curtain), g.mul(lit, g.collection(mpc, "WindowGlow"))), g.scalar("GlowStrength", 3.2))
    g.out(base, MP.MP_BASE_COLOR)
    g.out(g.const(0.06), MP.MP_ROUGHNESS)
    g.out(glow, MP.MP_EMISSIVE_COLOR)
    return g.finish()


def glass_master():
    g = Graph("M_SpnGlass", blend_mode=unreal.BlendMode.BLEND_TRANSLUCENT, two_sided=True, translucency_lighting_mode=unreal.TranslucencyLightingMode.TLM_SURFACE)
    g.out(g.const((0.03, 0.04, 0.04)), MP.MP_BASE_COLOR)
    g.out(g.const(0.05), MP.MP_ROUGHNESS)
    g.out(g.scalar("Opacity", 0.22), MP.MP_OPACITY)
    return g.finish()


def lamp_master(mpc):
    g = Graph("M_SpnLamp")
    glow = g.mul(g.mul(g.vector("Color", (1.0, 0.78, 0.52)), g.scalar("Strength", 60.0)), g.add(g.scalar("AlwaysOn", 0.0), g.collection(mpc, "LampGlow")))
    g.out(g.const((0.6, 0.6, 0.58)), MP.MP_BASE_COLOR)
    g.out(g.const(0.3), MP.MP_ROUGHNESS)
    g.out(glow, MP.MP_EMISSIVE_COLOR)
    return g.finish()


def marking_master(textured):
    g = Graph("M_SpnMarking", blend_mode=unreal.BlendMode.BLEND_MASKED)
    g.out(g.vector("Color", (0.80, 0.80, 0.76)), MP.MP_BASE_COLOR)
    g.out(g.const(0.55), MP.MP_ROUGHNESS)
    if textured:
        world = g.mask(g.node("WorldPosition"), "rg")
        wear = g.tex("Wear", tex("Grunge_C"), g.mul(world, 1.0 / 350.0), "masks", param=False)
        g.out(g.sub(1.0, g.mul((wear, "B"), g.scalar("Wear", 0.75))), MP.MP_OPACITY_MASK)
    return g.finish()


def water_master():
    g = Graph("M_SpnWater")
    world = g.mask(g.node("WorldPosition"), "rg")
    time = g.node("Time")
    n1 = g.tex("N1", k.load(WATER_N), g.add(g.mul(world, 1.0 / 420.0), g.mul(time, 0.012)), "normal", param=False)
    n2 = g.tex("N2", k.load(WATER_N), g.sub(g.mul(world, 1.0 / 170.0), g.mul(time, 0.02)), "normal", param=False)
    normal = g.lerp(g.const((0, 0, 1)), g.add((n1, "RGB"), (n2, "RGB")), 0.22)
    g.out(g.vector("Color", (0.012, 0.022, 0.018)), MP.MP_BASE_COLOR)
    g.out(g.const(0.04), MP.MP_ROUGHNESS)
    g.out(g.const(0.6), MP.MP_SPECULAR)
    g.out(g.one("Normalize", normal, "VectorInput"), MP.MP_NORMAL)
    return g.finish()


def foliage_master(name, wind=0.0):
    """Leaves and needles: masked, two-sided foliage shading. Colour + alpha from one texture."""
    g = Graph(name, blend_mode=unreal.BlendMode.BLEND_MASKED, two_sided=True, shading_model=unreal.MaterialShadingModel.MSM_TWO_SIDED_FOLIAGE)
    g.mat.set_editor_property("opacity_mask_clip_value", 0.35)
    c = g.tex("ColorAlpha", tex("GrassCard_C"), None, "color")
    world = g.node("WorldPosition")
    patch = g.tex("Macro", k.load(MACRO), g.mul(g.mask(world, "rg"), 1.0 / 5200.0), "color", param=False)
    base = g.mul(g.mul((c, "RGB"), g.vector("Tint", (1, 1, 1))), g.lerp(0.72, 1.18, (patch, "R")))
    g.out(base, MP.MP_BASE_COLOR)
    g.out((c, "A"), MP.MP_OPACITY_MASK)
    g.out(g.scalar("Roughness", 0.62), MP.MP_ROUGHNESS)
    g.out(g.mul(base, g.vector("Subsurface", (0.55, 0.65, 0.25))), MP.MP_SUBSURFACE_COLOR)
    g.out(g.const(0.25), MP.MP_SPECULAR)
    for usage in (unreal.MaterialUsage.MATUSAGE_INSTANCED_STATIC_MESHES, unreal.MaterialUsage.MATUSAGE_NANITE):
        mel_ok = k.mel.set_material_usage(g.mat, usage)
    return g.finish()


def bark_master():
    g = Graph("M_SpnBark")
    uv = g.mul(g.node("TextureCoordinate"), g.scalar("UVScale", 1.0))
    c = g.tex("BaseColor", tex("Planks_C"), uv, "color")
    g.out(g.mul((c, "RGB"), g.vector("Tint", (1, 1, 1))), MP.MP_BASE_COLOR)
    g.out(g.const(0.88), MP.MP_ROUGHNESS)
    for usage in (unreal.MaterialUsage.MATUSAGE_INSTANCED_STATIC_MESHES, unreal.MaterialUsage.MATUSAGE_NANITE):
        k.mel.set_material_usage(g.mat, usage)
    return g.finish()


def landscape_master(textured, grass_types=None):
    """Ground: the five painted layers, each a world-aligned texture set with a second, larger tiling mixed in against repetition."""
    g = Graph("M_SpnLandscape")
    world = g.mask(g.node("WorldPosition"), "rg")
    colour = normal = orm = None
    weights = {}
    for name, texture, tile, flat in LAYERS:
        w = g.node("LandscapeLayerSample", parameter_name=name)
        weights[name] = w
        if textured:
            scanned = fab.floor(name)             # Megascans surface when the developer imported one for this layer
            if scanned:
                t_c, t_n, t_orm, tile = scanned
                mix = 0.30                        # scans repeat less visibly: less of the large-scale sample
            else:
                t_c, t_n, t_orm, mix = tex(texture + "_C"), tex(texture + "_N"), tex(texture + "_ORM"), 0.45
            uv = g.mul(world, 1.0 / tile)
            near = g.tex(name + "C", t_c, uv, "color", param=False)
            far = g.tex(name + "Cfar", t_c, g.mul(world, 0.23 / tile), "color", param=False)
            c = g.mul(g.lerp((near, "RGB"), (far, "RGB"), mix), w)
            if name in LAYER_TINT:                # night build 2026-11-14: less sand-coloured verges and lawn ground (darker, earthier)
                c = g.mul(c, g.const(LAYER_TINT[name]))
            n = g.mul((g.tex(name + "N", t_n, uv, "normal", param=False), "RGB"), w)
            o = g.mul((g.tex(name + "O", t_orm, uv, "masks", param=False), "RGB"), w)
            colour = c if colour is None else g.add(colour, c)
            normal = n if normal is None else g.add(normal, n)
            orm = o if orm is None else g.add(orm, o)
        else:
            c = g.mul(g.const(flat), w)
            colour = c if colour is None else g.add(colour, c)
    g.out(g.mul(colour, macro_factor(g, 0.72, 1.2, 1.0 / 5200.0)), MP.MP_BASE_COLOR)
    if textured:
        g.out(g.one("Normalize", normal, "VectorInput"), MP.MP_NORMAL)
        g.out(g.mask(orm, "g"), MP.MP_ROUGHNESS)
        g.out(g.mask(orm, "r"), MP.MP_AMBIENT_OCCLUSION)
    else:
        g.out(g.const(0.9), MP.MP_ROUGHNESS)
    if grass_types:
        out = g.node("LandscapeGrassOutput")
        inputs = []
        for layer, grass_type in grass_types:
            entry = unreal.GrassInput()
            entry.set_editor_property("name", layer + "_" + grass_type.get_name())
            entry.set_editor_property("grass_type", grass_type)
            inputs.append(entry)
        out.set_editor_property("grass_types", inputs)
        for layer, grass_type in grass_types:
            g.link(weights[layer], out, layer + "_" + grass_type.get_name())
    return g.finish()


def build(textured, grass_types=None):
    """Returns {name: material}. textured = False gives the untextured look of the phases before the material phase."""
    mpc = collection()
    mats = {"_mpc": mpc}
    flat = flat_master()
    if textured:
        master = surface_master(mpc)
        for name, (texture, tile, tint, rough, _) in SURFACES.items():
            mats[name] = k.instance("MI_Spn_" + name, master, {"Tiling": tile, "RoughScale": rough}, {"Tint": tint},
                                    {"BaseColor": tex(texture + "_C"), "Normal": tex(texture + "_N"), "ORM": tex(texture + "_ORM")})
        # night build 2026-11-14: the scanned "Dirt" floor is a ROCKY scan; on the pine track it read as pink flagstones. Tracks and trodden paths use the
        # generated earth texture again (the terrain beside them keeps the scan). Set TRACK_USES_SCAN = True to return to the scan.
        scanned = fab.floor("Dirt") if TRACK_USES_SCAN else None
        if scanned:
            t_c, t_n, t_orm, tile = scanned
            mats["dirt"] = k.instance("MI_Spn_dirt", master, {"Tiling": tile, "RoughScale": 1.0}, {"Tint": TRACK_TINT}, {"BaseColor": t_c, "Normal": t_n, "ORM": t_orm})
        # 2026-11-14 (developer decision 4): the pine track keeps the earth and gets ruts, litter and patches (track_master); roads.SURFACE["track"]
        mats["track"] = mats["dirt"] if TRACK_USES_SCAN else track_master(mpc)
    else:
        for name, entry in SURFACES.items():
            mats[name] = k.instance("MI_Spn_" + name, flat, {}, {"Color": entry[4]})
        mats["track"] = mats["dirt"]
    mats["window"] = window_master(mpc)
    mats["glass"] = glass_master()
    lamp = lamp_master(mpc)
    mats["lamp"] = k.instance("MI_Spn_lamp", lamp, {"Strength": 220.0}, {"Color": (1.0, 0.74, 0.46)})
    mats["lamp_cold"] = k.instance("MI_Spn_lamp_cold", lamp, {"Strength": 260.0}, {"Color": (0.78, 0.90, 1.0)})
    mats["neon_red"] = k.instance("MI_Spn_neon_red", lamp, {"Strength": 90.0, "AlwaysOn": 0.04}, {"Color": (1.0, 0.10, 0.06)})
    mats["neon_blue"] = k.instance("MI_Spn_neon_blue", lamp, {"Strength": 90.0, "AlwaysOn": 0.04}, {"Color": (0.15, 0.45, 1.0)})
    marking = marking_master(textured)
    mats["mark_white"] = k.instance("MI_Spn_mark_white", marking, {}, {"Color": (0.78, 0.78, 0.74)})
    mats["mark_yellow"] = k.instance("MI_Spn_mark_yellow", marking, {}, {"Color": (0.80, 0.58, 0.08)})
    mats["water"] = water_master()
    mats["landscape"] = landscape_master(textured, grass_types)
    return mats
