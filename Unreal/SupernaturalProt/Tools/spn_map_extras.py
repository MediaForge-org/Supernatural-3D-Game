"""Editor-side helpers for the test map generators that the layout consistency test does not scan (they need the Unreal Python API).

Ownership: every actor made by create_spn_movement_test_map.py is tagged SPN_GEN (+ SPN_GEN_MAP:<map>, SPN_GEN_VER:<version>), and
only tagged actors are ever deleted by --force. The tag logic is plain Python on `actor.tags` / `actor.get_actor_label()` and is unit
tested with fake actors (test_spn_map_tools.py).

STATUS: written on a machine without Unreal Engine; never executed in the editor. UNCONFIRMED engine API assumptions:
AActor.Tags reachable as `actor.tags` (list of unreal.Name) and settable; PointLightComponent properties `intensity` and
`attenuation_radius`; adding an InstancedStaticMeshComponent to a spawned actor and `add_instance(transform)`.
"""

import collections

import unreal

import spn_instancing

TAG_OWNED = "SPN_GEN"
GENERATED_PREFIXES = ("SPN_MV_", "SPN_D_", "SPN_Sun", "SPN_Sky", "SPN_PlayerStart")
_state = {"map": "", "version": ""}


def set_generator(map_path, version):
    _state["map"], _state["version"] = map_path, version


def generator_tags():
    return [TAG_OWNED, "SPN_GEN_MAP:" + _state["map"], "SPN_GEN_VER:" + _state["version"]]


def tag_actor(actor):
    """Mark an actor as made by this generator. Never raises (a missing tag only makes --force stricter)."""
    try:
        existing = [str(t) for t in actor.tags]
        actor.tags = [unreal.Name(t) for t in existing + [t for t in generator_tags() if t not in existing]]
    except Exception as error:  # noqa: BLE001 - tag API unconfirmed
        unreal.log_warning("SpnMovementTestMap: could not tag %s: %s" % (actor.get_actor_label(), error))


def classify_actors(actors, adopt_legacy=False):
    """(owned, legacy, foreign): owned = tagged SPN_GEN (plus, with adopt_legacy, untagged SPN_MV_/SPN_D_/SPN_ generator labels);
    legacy = untagged actors that look generated (SPN_MV_/SPN_D_ prefix); foreign = everything else. Pure, no deletion."""
    owned, legacy, foreign = [], [], []
    for actor in actors:
        tags = [str(t) for t in actor.tags]
        label = actor.get_actor_label()
        if TAG_OWNED in tags:
            owned.append(actor)
        elif label.startswith(GENERATED_PREFIXES):
            legacy.append(actor)
            if adopt_legacy:
                owned.append(actor)
        else:
            foreign.append(actor)
    return owned, legacy, foreign


def build_lights(spawn, data):
    """One stationary point light per entry of data["lights"] (interior ceiling lights and street lamps)."""
    for i, light in enumerate(data.get("lights", [])):
        actor = spawn(unreal.PointLight, light["pos"], label="SPN_D_Light_%03d" % i)
        component = actor.get_component_by_class(unreal.PointLightComponent)
        component.set_editor_property("intensity", light["intensity"])
        component.set_editor_property("attenuation_radius", light["radius"])
        component.set_mobility(unreal.ComponentMobility.STATIONARY)


RebuildPlan = collections.namedtuple("RebuildPlan", "action remove reason")


def rebuild_plan(actors, level_exists, force, adopt_legacy=False):
    """Decide what a run does before it touches anything. Returns a RebuildPlan(action, remove, reason):
    create (no level yet), keep (level exists, no --force: nothing changes), abort (legacy actors without a tag and no --adopt-legacy),
    rebuild (remove exactly `remove` = the owned actors, then generate). Foreign actors are never in `remove`."""
    if not level_exists:
        return RebuildPlan("create", [], "no level yet")
    if not force:
        return RebuildPlan("keep", [], "level exists, pass --force to rebuild")
    owned, legacy, foreign = classify_actors(actors, adopt_legacy)
    if legacy and not adopt_legacy:
        return RebuildPlan("abort", [], "%d SPN_ actors without generator tag (older generator version); use --adopt-legacy" % len(legacy))
    return RebuildPlan("rebuild", owned, "%d generated actors replaced, %d other actors kept" % (len(owned), len(foreign)))


def expected_counts(data, groups, individual):
    """What a correct run leaves in the level: shape actors made individually, instance-group actors, doors, lights, guard, player start."""
    return {"shapes": individual, "groups": len(groups), "doors": len(data["doors"]), "lights": len(data.get("lights", [])), "guard": 1, "player_start": 1}


def verify_level(actors, expected):
    """Problems (strings) of the actors in the level against `expected` (see expected_counts); empty = consistent. Checks the counts per
    label family, duplicate SPN_ labels (a rerun that did not clean up) and generated actors without the generator tag."""
    labels = [a.get_actor_label() for a in actors]
    families = {"doors": 0, "lights": 0, "groups": 0, "guard": 0, "player_start": 0, "shapes": 0}
    problems = []
    for actor, label in zip(actors, labels):
        if not label.startswith(GENERATED_PREFIXES):
            continue  # hand-made actors, even when named SPN_*, are not ours to judge
        if TAG_OWNED not in [str(t) for t in actor.tags]:
            problems.append("untagged generated actor " + label)
        if label.startswith("SPN_MV_Door_"):
            families["doors"] += 1
        elif label.startswith("SPN_D_Light_"):
            families["lights"] += 1
        elif label.startswith("SPN_D_ISM_"):
            families["groups"] += 1
        elif label == "SPN_MV_PlayAreaGuard":
            families["guard"] += 1
        elif label == "SPN_PlayerStart":
            families["player_start"] += 1
        elif label.startswith(("SPN_MV_", "SPN_D_")):
            families["shapes"] += 1
    seen = set()
    for label in labels:
        if label.startswith(GENERATED_PREFIXES) and label in seen:
            problems.append("duplicate label " + label)
        seen.add(label)
    for key, want in expected.items():
        if families[key] != want:
            problems.append("%s: expected %d, found %d" % (key, want, families[key]))
    return problems


INSTANCE_HOLDER_CLASS_PATH = "/Script/SupernaturalProt.SpnInstancedGroup"


def ensure_instancing_usage(material):
    """Instanced draws need the material flag "Used with Instanced Static Meshes" on the BASE material; without it a packaged or -game
    build shows the default material on every instance (found in Unreal: the LevelPrototyping materials lack it). Sets the flag, recompiles
    and saves the base material once; a no-op when it is already set. Touches only the material assets the generator itself uses."""
    base = material.get_base_material() if hasattr(material, "get_base_material") else material
    if base is None or base.get_editor_property("used_with_instanced_static_meshes"):
        return
    base.set_editor_property("used_with_instanced_static_meshes", True)
    unreal.MaterialEditingLibrary.recompile_material(base)
    if not unreal.EditorAssetLibrary.save_loaded_asset(base):
        raise RuntimeError("could not save the material flag on " + base.get_path_name())
    unreal.log("SpnMovementTestMap: set 'used with instanced static meshes' on " + base.get_path_name())


def _destroy(actor):
    """Remove a half-built instance actor; raises when the engine refuses (then the run must stop and not save)."""
    if not unreal.get_editor_subsystem(unreal.EditorActorSubsystem).destroy_actor(actor):
        raise RuntimeError("could not remove the partially built instance actor %s" % actor.get_actor_label())


def spawn_instance_group(spawn, load_shape, load_material, native_sizes, group, fallback_spawn_prim):
    """Draw one instance group as an InstancedStaticMeshComponent on a new actor. Returns the number of INDIVIDUAL shape actors made
    (0 when the group was instanced, len(instances) when the fallback spawned one actor per primitive). On ANY failure the partly built
    instance actor is destroyed BEFORE the fallback runs, so the geometry never exists twice. UNCONFIRMED editor API."""
    actor = None
    try:
        mesh = load_shape(group["shape"])
        native = native_sizes[group["shape"]]
        holder_class = unreal.load_class(None, INSTANCE_HOLDER_CLASS_PATH)
        if holder_class is None:
            raise RuntimeError("class not found, build the editor target first: " + INSTANCE_HOLDER_CLASS_PATH)
        actor = spawn(holder_class, (0.0, 0.0, 0.0), label=group["id"])
        component = actor.get_editor_property("instances")
        component.set_static_mesh(mesh)
        material = load_material(group["material"])
        if material is not None:
            component.set_material(0, material)
        for prim in group["instances"]:
            record = spn_instancing.spawn_record(prim, native)
            transform = unreal.Transform(
                location=unreal.Vector(*record["location"]),
                rotation=unreal.Rotator(roll=record["rotation"][2], pitch=record["rotation"][0], yaw=record["rotation"][1]),
                scale=unreal.Vector(*record["scale"]))
            component.add_instance(transform, True)
        if component.get_instance_count() != len(group["instances"]):
            raise RuntimeError("instance count %d, expected %d" % (component.get_instance_count(), len(group["instances"])))
        return 0
    except Exception as error:  # noqa: BLE001 - editor API unconfirmed: fall back, never lose geometry, never duplicate it
        unreal.log_warning("SpnMovementTestMap: instancing of %s failed (%s); spawning %d individual actors" % (group["id"], error, len(group["instances"])))
        if actor is not None:
            _destroy(actor)
        for prim in group["instances"]:
            fallback_spawn_prim(prim)
        return len(group["instances"])
