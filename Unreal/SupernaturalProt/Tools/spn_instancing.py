"""Instancing plan for the v2 test map (M2-A.2 phase 2, package B3). Pure Python, no Unreal import.

The district primitives (SPN_D_*) are repetitive, static and carry no per-object gameplay, so they can be drawn as instanced static
meshes: one group (= one actor with one instanced component) per (shape, material, 75 m map cell). v1 primitives (SPN_MV_*) stay
individual actors, because the Windows automation tests look them up by label. Door actors, the play-area guard, lights and the
player start stay individual actors too.

This module only PLANS and counts. The actor reduction it reports is a count of generated data, not a performance measurement.
Spawning the instanced components in the editor (spn_map_extras.spawn_instance_group) uses editor Python calls that are UNCONFIRMED
without an engine; the editor script falls back to one actor per primitive when they fail.
"""

CELL = 7500.0


def group_id(shape, material, ix, iy):
    return "SPN_D_ISM_%s_%s_%d_%d" % (shape, material, ix, iy)


def plan_instancing(prims, prefix=("SPN_D_", "SPN_E_")):
    """Split `prims` into (actors, groups). actors: prims that stay individual actors (every prim without the prefix). groups: list of
    {"id", "shape", "material", "cell", "instances": [prim, ...]} sorted by id; instances keep their order in `prims`."""
    actors, groups = [], {}
    for p in prims:
        if not p["label"].startswith(prefix):
            actors.append(p)
            continue
        ix, iy = int((p["center"][0] + 15000.0) // CELL), int((p["center"][1] + 15000.0) // CELL)
        gid = group_id(p["shape"], p["material"], ix, iy)
        group = groups.setdefault(gid, {"id": gid, "shape": p["shape"], "material": p["material"], "cell": [ix, iy], "instances": []})
        group["instances"].append(p)
    return actors, [groups[k] for k in sorted(groups)]


def summarize(data):
    """Actor counts before and after instancing for a layout dict (primitives, doors, lights, guard, player start)."""
    actors, groups = plan_instancing(data["prims"])
    fixed = len(data["doors"]) + len(data.get("lights", [])) + 1 + 1  # doors, lights, guard, player start (sun/sky are not counted)
    before = len(data["prims"]) + fixed
    after = len(actors) + len(groups) + fixed
    return {"primitives": len(data["prims"]), "actors_before": before, "actors_after": after, "groups": len(groups),
            "instanced_primitives": sum(len(g["instances"]) for g in groups), "largest_group": max((len(g["instances"]) for g in groups), default=0)}


def instanced_ids(groups):
    """id() of every primitive dict that sits in a group (to skip them when spawning individual actors)."""
    return {id(p) for g in groups for p in g["instances"]}


def spawn_record(prim, native_size):
    """What the editor must create for one primitive, as plain data: location, rotation (pitch, yaw, roll), scale (wanted size / native
    size of the engine shape), shape, material and the collision intent. The individual-actor path and the instance path both use
    exactly this, so the instanced result can be compared with the individual one in tests."""
    return {
        "shape": prim["shape"], "material": prim["material"], "location": list(prim["center"]), "rotation": list(prim["rot"]),
        "scale": [prim["size"][i] / native_size[i] for i in range(3)], "collision": "block_all",
    }


def group_label(group):
    """Actor label of an instance group (its id)."""
    return group["id"]
