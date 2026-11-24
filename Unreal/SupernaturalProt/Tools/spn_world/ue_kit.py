"""Unreal-side helpers of the world generator: textures, materials, static meshes from meshkit data, actors."""

import os

import unreal

import meshkit

ROOT = "/Game/SpnWorld"
TEX = ROOT + "/Textures"
MAT = ROOT + "/Materials"
MESH = ROOT + "/Meshes"
mel = unreal.MaterialEditingLibrary
eal = unreal.EditorAssetLibrary
tools = unreal.AssetToolsHelpers.get_asset_tools()
actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
MP = unreal.MaterialProperty


def log(message):
    unreal.log("SpnWorld: " + str(message))


def fail(message):
    unreal.log_error("SpnWorld: " + str(message))
    raise RuntimeError(message)


def load(path):
    asset = unreal.load_asset(path)
    if asset is None:
        fail("asset not found: " + path)
    return asset


# ---------------------------------------------------------------- textures

def import_textures(folder, force=False):
    """Imports every T_*.tga of the folder into /Game/SpnWorld/Textures with the right compression."""
    tasks = []
    for name in sorted(os.listdir(folder)):
        if not name.lower().endswith(".tga"):
            continue
        asset_name = os.path.splitext(name)[0]
        if eal.does_asset_exist(TEX + "/" + asset_name) and not force:
            continue
        task = unreal.AssetImportTask()
        task.filename = os.path.join(folder, name)
        task.destination_path = TEX
        task.destination_name = asset_name
        task.automated = True
        task.replace_existing = True
        task.save = False
        tasks.append(task)
    if tasks:
        tools.import_asset_tasks(tasks)
    for task in tasks:
        tex = unreal.load_asset(TEX + "/" + task.destination_name)
        if tex is None:
            fail("texture import failed: " + task.filename)
        name = task.destination_name
        if name.endswith("_N"):
            tex.set_editor_property("compression_settings", unreal.TextureCompressionSettings.TC_NORMALMAP)
            tex.set_editor_property("srgb", False)
        elif name.endswith("_ORM") or "Grunge" in name:
            tex.set_editor_property("compression_settings", unreal.TextureCompressionSettings.TC_MASKS)
            tex.set_editor_property("srgb", False)
        tex.set_editor_property("virtual_texture_streaming", False)
        eal.save_loaded_asset(tex)
    log("textures imported: %d" % len(tasks))


def copy_asset(source, name, folder=TEX):
    """Duplicates an engine/plugin asset into the project (so the game does not depend on an editor plugin)."""
    target = folder + "/" + name
    if not eal.does_asset_exist(target):
        if eal.duplicate_asset(source, target) is None:
            fail("could not copy " + source)
        asset = unreal.load_asset(target)
        if isinstance(asset, unreal.Texture2D):
            asset.set_editor_property("virtual_texture_streaming", False)
        eal.save_loaded_asset(asset)
    return unreal.load_asset(target)


# ---------------------------------------------------------------- material graphs

class Graph:
    """Small wrapper around MaterialEditingLibrary: creates nodes and wires them."""

    def __init__(self, name, folder=MAT, **props):
        path = folder + "/" + name
        if eal.does_asset_exist(path):
            eal.delete_asset(path)
        self.mat = tools.create_asset(name, folder, unreal.Material, unreal.MaterialFactoryNew())
        for key, value in props.items():
            self.mat.set_editor_property(key, value)
        self.y = 0

    def node(self, cls, **props):
        self.y += 120
        n = mel.create_material_expression(self.mat, getattr(unreal, "MaterialExpression" + cls), -600, self.y)
        for key, value in props.items():
            n.set_editor_property(key, value)
        return n

    @staticmethod
    def _src(value):
        return value if isinstance(value, tuple) else (value, "")

    def link(self, src, dst, pin):
        node, out = self._src(src)
        if not mel.connect_material_expressions(node, out, dst, pin):
            fail("material link failed: %s.%s -> %s.%s" % (node.get_name(), out, dst.get_name(), pin))

    def const(self, value):
        if isinstance(value, (int, float)):
            return self.node("Constant", r=float(value))
        return self.node("Constant3Vector", constant=unreal.LinearColor(value[0], value[1], value[2], 1.0))

    def _bin(self, cls, a, b):
        n = self.node(cls)
        for pin, value in (("A", a), ("B", b)):
            if isinstance(value, (int, float)):
                n.set_editor_property("const_" + pin.lower(), float(value))
            else:
                self.link(value, n, pin)
        return n

    def mul(self, a, b):
        return self._bin("Multiply", a, b)

    def add(self, a, b):
        return self._bin("Add", a, b)

    def sub(self, a, b):
        return self._bin("Subtract", a, b)

    def div(self, a, b):
        return self._bin("Divide", a, b)

    def lerp(self, a, b, alpha):
        n = self.node("LinearInterpolate")
        for pin, value in (("A", a), ("B", b), ("Alpha", alpha)):
            if isinstance(value, (int, float)):
                n.set_editor_property("const_" + pin.lower(), float(value))
            else:
                self.link(value, n, pin)
        return n

    def one(self, cls, value, pin=""):
        n = self.node(cls)
        self.link(value, n, pin)
        return n

    def mask(self, value, channels):
        n = self.node("ComponentMask", r="r" in channels, g="g" in channels, b="b" in channels, a="a" in channels)
        self.link(value, n, "")
        return n

    def scalar(self, name, default, group="Spn"):
        return self.node("ScalarParameter", parameter_name=name, default_value=float(default), group=group)

    def vector(self, name, default, group="Spn"):
        return self.node("VectorParameter", parameter_name=name, default_value=unreal.LinearColor(default[0], default[1], default[2], default[3] if len(default) > 3 else 1.0), group=group)

    def tex(self, name, texture, uv=None, kind="color", param=True):
        sampler = {"color": unreal.MaterialSamplerType.SAMPLERTYPE_COLOR, "normal": unreal.MaterialSamplerType.SAMPLERTYPE_NORMAL,
                   "linear": unreal.MaterialSamplerType.SAMPLERTYPE_LINEAR_COLOR, "masks": unreal.MaterialSamplerType.SAMPLERTYPE_MASKS}[kind]
        if param:
            n = self.node("TextureSampleParameter2D", parameter_name=name, texture=texture, sampler_type=sampler)
        else:
            n = self.node("TextureSample", texture=texture, sampler_type=sampler)
        n.set_editor_property("sampler_source", unreal.SamplerSourceMode.SSM_WRAP_WORLD_GROUP_SETTINGS)
        if uv is not None:
            self.link(uv, n, "UVs")
        return n

    def collection(self, collection, name):
        return self.node("CollectionParameter", collection=collection, parameter_name=name)

    def out(self, value, prop):
        node, pin = self._src(value)
        if not mel.connect_material_property(node, pin, prop):
            fail("material output failed: " + str(prop))

    def finish(self):
        mel.layout_material_expressions(self.mat)
        mel.recompile_material(self.mat)
        eal.save_loaded_asset(self.mat)
        return self.mat


def instance(name, parent, scalars=None, vectors=None, textures=None, folder=MAT):
    path = folder + "/" + name
    if eal.does_asset_exist(path):
        eal.delete_asset(path)
    mi = tools.create_asset(name, folder, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    mel.set_material_instance_parent(mi, parent)
    for key, value in (scalars or {}).items():
        mel.set_material_instance_scalar_parameter_value(mi, key, float(value))
    for key, value in (vectors or {}).items():
        mel.set_material_instance_vector_parameter_value(mi, key, unreal.LinearColor(value[0], value[1], value[2], 1.0))
    for key, value in (textures or {}).items():
        mel.set_material_instance_texture_parameter_value(mi, key, value)
    mel.update_material_instance(mi)
    eal.save_loaded_asset(mi)
    return mi


# ---------------------------------------------------------------- meshes

def calibrate_winding():
    """Finds out which triangle order DynamicMesh treats as outward-facing and sets meshkit.WIND."""
    meshkit.WIND = 1
    test = meshkit.Mesh()
    test.box("m", 0, 100, 0, 100, 0, 100)
    dyn = to_dynamic(test, ["m"])
    volume = unreal.GeometryScript_MeshQueries.get_mesh_volume_area(dyn)[1]
    meshkit.WIND = 1 if volume > 0 else -1
    log("winding calibration: volume %.0f -> WIND %d" % (volume, meshkit.WIND))


def to_dynamic(mesh, keys):
    dyn = unreal.DynamicMesh()
    unreal.GeometryScript_Materials.enable_material_i_ds(dyn)
    for index, key in enumerate(keys):
        part = mesh.parts.get(key)
        if part is None or not part.t:
            continue
        buffers = unreal.GeometryScriptSimpleMeshBuffers()
        buffers.vertices = [unreal.Vector(v[0], v[1], v[2]) for v in part.v]
        buffers.normals = [unreal.Vector(n[0], n[1], n[2]) for n in part.n]
        buffers.uv0 = [unreal.Vector2D(uv[0], uv[1]) for uv in part.uv]
        buffers.vertex_colors = [unreal.LinearColor(c[0], c[1], c[2], c[3]) for c in part.c]
        buffers.triangles = [unreal.IntVector(t[0], t[1], t[2]) for t in part.t]
        unreal.GeometryScript_MeshEdits.append_buffers_to_mesh(dyn, buffers, index)
    return dyn


def static_mesh(mesh, name, materials, nanite=True, collision=True, folder=MESH):
    """Creates (or replaces) a StaticMesh asset from a meshkit Mesh. materials: {part key: material}."""
    keys = [k for k in mesh.parts if mesh.parts[k].t]
    for key in keys:
        if key not in materials:
            fail("mesh %s: no material for part '%s'" % (name, key))
    dyn = to_dynamic(mesh, keys)
    unreal.GeometryScript_Normals.compute_tangents(dyn, unreal.GeometryScriptTangentsOptions())
    path = folder + "/" + name
    if eal.does_asset_exist(path):
        eal.delete_asset(path)
    options = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
    options.enable_recompute_normals = False
    options.enable_recompute_tangents = False
    options.enable_nanite = nanite
    options.enable_collision = False
    asset, outcome = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(dyn, path, options)
    if asset is None:
        fail("could not create mesh " + path)
    asset.set_editor_property("static_materials", [unreal.StaticMaterial(material_interface=materials[k], material_slot_name=k) for k in keys])
    body = asset.get_editor_property("body_setup")
    if body is not None:
        body.set_editor_property("collision_trace_flag", unreal.CollisionTraceFlag.CTF_USE_COMPLEX_AS_SIMPLE if collision else unreal.CollisionTraceFlag.CTF_USE_DEFAULT)
    eal.save_loaded_asset(asset)
    return asset


def spawn(cls, x=0.0, y=0.0, z=0.0, yaw=0.0, label=None, pitch=0.0, folder=None):
    actor = actors.spawn_actor_from_class(cls, unreal.Vector(x, y, z), unreal.Rotator(roll=0.0, pitch=pitch, yaw=yaw))
    if actor is None:
        fail("could not spawn " + str(cls))
    if label:
        actor.set_actor_label(label)
    if folder:
        actor.set_folder_path(folder)
    return actor


def place(asset, x=0.0, y=0.0, z=0.0, yaw=0.0, label=None, folder=None, collide=True, shadow=True):
    actor = spawn(unreal.StaticMeshActor, x, y, z, yaw, label, folder=folder)
    component = actor.static_mesh_component
    component.set_static_mesh(asset)
    if not collide:
        component.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    if not shadow:
        component.set_cast_shadow(False)
    return actor
