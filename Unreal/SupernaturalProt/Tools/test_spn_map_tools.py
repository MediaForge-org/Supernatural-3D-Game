"""Tests of the instancing plan and the generator ownership logic (with a fake `unreal` module), without Unreal.

Run from Unreal/SupernaturalProt:  python3 -m unittest Tools.test_spn_map_tools
They test the PLAN and the tag logic only. That the editor accepts the calls (tags, instanced components) is a Windows check.
"""

import hashlib
import os
import sys
import types
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

fake = types.ModuleType("unreal")
fake.Name = str
fake.log_warning = fake.log_error = fake.log = lambda *_: None


class _Rec:
    """Accepts any constructor arguments and remembers them (stand-in for unreal.Vector / Rotator / Transform)."""

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs


class _Actor:
    pass


class _ISM:
    pass


fake.Vector = fake.Rotator = fake.Transform = _Rec
fake.Actor = _Actor
fake.EditorActorSubsystem = type("EditorActorSubsystem", (), {})


class _ActorSubsystem:
    def destroy_actor(self, actor):
        return actor.destroy_actor()


fake.load_class = lambda outer, path: object()  # the SpnInstancedGroup holder class exists
fake.get_editor_subsystem = lambda cls: _ActorSubsystem()
fake.InstancedStaticMeshComponent = _ISM
sys.modules.setdefault("unreal", fake)

import spn_district_layout as D  # noqa: E402
import spn_instancing as I  # noqa: E402
import spn_map_extras as X  # noqa: E402
import spn_movement_map_layout as L  # noqa: E402


class FakeActor:
    def __init__(self, label, tags=()):
        self.label = label
        self.tags = list(tags)
        self.destroyed = False
        self.components = []
        self.refuse_destroy = False

    def get_actor_label(self):
        return self.label

    def get_editor_property(self, name):
        """ASpnInstancedGroup.instances: the holder actor's instanced component."""
        assert name == "instances"
        if not self.components:
            self.components.append(FakeISM(self))
        return self.components[0]

    def destroy_actor(self):
        if self.refuse_destroy:
            return False
        self.destroyed = True
        return True


class FakeISM:
    fail_after = None  # class-level switch: add_instance raises on that many-th call

    def __init__(self, owner):
        self.owner, self.instances, self.mesh, self.material = owner, [], None, None

    def set_static_mesh(self, mesh):
        self.mesh = mesh

    def set_material(self, index, material):
        self.material = material

    def add_instance(self, transform, world_space):
        if FakeISM.fail_after is not None and len(self.instances) >= FakeISM.fail_after:
            raise RuntimeError("add_instance refused")
        self.instances.append(transform)

    def get_instance_count(self):
        return len(self.instances)


class World:
    """A tiny stand-in for a level: spawn() makes tagged fake actors like the generator's spawn() does."""

    def __init__(self, foreign=()):
        self.actors = list(foreign)

    def spawn(self, cls, location, rotation=(0, 0, 0), label=None):
        a = FakeActor(label or "x")
        X.tag_actor(a)
        self.actors.append(a)
        return a

    def generate(self, data, groups=()):
        """What a generator run adds to the level (labels only, as the real run)."""
        self.spawn(None, (0, 0, 0), label="SPN_PlayerStart")
        in_group = {id(p) for g in groups for p in g["instances"]}
        n = 0
        for p in data["prims"]:
            if id(p) not in in_group:
                self.spawn(None, p["center"], label=p["label"])
                n += 1
        for g in groups:
            self.spawn(None, (0, 0, 0), label=g["id"])
        for d in data["doors"]:
            self.spawn(None, d["hinge"], label="SPN_MV_Door_" + d["id"])
        self.spawn(None, (0, 0, 0), label="SPN_MV_PlayAreaGuard")
        for i, _ in enumerate(data.get("lights", [])):
            self.spawn(None, (0, 0, 0), label="SPN_D_Light_%03d" % i)
        return n

    def apply(self, plan):
        self.actors = [a for a in self.actors if a not in plan.remove]


class InstancingTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = D.build_layout_v2()

    def test_every_primitive_is_either_an_actor_or_in_exactly_one_group(self):
        actors, groups = I.plan_instancing(self.data["prims"])
        grouped = [p for g in groups for p in g["instances"]]
        self.assertEqual(len(actors) + len(grouped), len(self.data["prims"]))
        self.assertEqual(len({id(p) for p in actors} | {id(p) for p in grouped}), len(self.data["prims"]))
        self.assertTrue(all(p["label"].startswith("SPN_MV_") for p in actors))      # v1 stays individual: tests look it up by label
        self.assertTrue(all(p["label"].startswith("SPN_D_") for p in grouped))
        self.assertEqual(I.instanced_ids(groups), {id(p) for p in grouped})

    def test_groups_are_homogeneous_sorted_and_unique(self):
        _, groups = I.plan_instancing(self.data["prims"])
        ids = [g["id"] for g in groups]
        self.assertEqual(ids, sorted(ids))
        self.assertEqual(len(ids), len(set(ids)))
        for g in groups:
            for p in g["instances"]:
                self.assertEqual((p["shape"], p["material"]), (g["shape"], g["material"]))
                self.assertEqual([int((p["center"][0] + 15000.0) // I.CELL), int((p["center"][1] + 15000.0) // I.CELL)], g["cell"])

    def test_actor_reduction_is_reported_from_generated_data(self):
        s = I.summarize(self.data)
        self.assertEqual(s["primitives"], len(self.data["prims"]))
        self.assertLess(s["actors_after"], s["actors_before"])
        self.assertGreater(s["actors_before"] - s["actors_after"], 500)
        self.assertEqual(s["instanced_primitives"], s["primitives"] - len(L.build_layout()["prims"]))

    def test_plan_is_deterministic(self):
        def digest():
            _, groups = I.plan_instancing(D.build_layout_v2()["prims"])
            return hashlib.sha256(repr([(g["id"], [p["label"] for p in g["instances"]]) for g in groups]).encode()).hexdigest()
        self.assertEqual(digest(), digest())

    def test_empty_and_v1_only_input(self):
        self.assertEqual(I.plan_instancing([]), ([], []))
        actors, groups = I.plan_instancing(L.build_layout()["prims"])
        self.assertEqual(groups, [])
        self.assertEqual(I.summarize({"prims": [], "doors": [], "lights": []})["largest_group"], 0)


class SnapshotTest(unittest.TestCase):
    """Layout snapshots: a deliberate change of the generators changes these hashes (update them WITH the change, and say so in the
    handover). v1 must never change without the Windows tests being updated, because they are written against that exact layout."""

    V1_SHA256 = "619b792d46dc577ecc9d5a90d997826250543681d8f010b314f55377b450c3ad"
    V2_SHA256 = "980f8dc9309935bdcfe67e98979ecbaef285e3e4c6a1f1ca04a0c6b41b86d3f8"

    def test_v1_layout_snapshot(self):
        self.assertEqual(hashlib.sha256(L.to_json(L.build_layout()).encode()).hexdigest(), self.V1_SHA256)

    def test_v2_layout_snapshot(self):
        self.assertEqual(hashlib.sha256(L.to_json(D.build_layout_v2()).encode()).hexdigest(), self.V2_SHA256)


class EditorLogicTest(unittest.TestCase):
    """The editor script's decisions, run against fake actors: idempotent rebuilds, verification before saving, instance fallback."""

    @classmethod
    def setUpClass(cls):
        cls.data = D.build_layout_v2()
        cls.groups = I.plan_instancing(cls.data["prims"])[1]

    def setUp(self):
        X.set_generator("/Game/SpnTest/Lvl_X", "v-test")
        FakeISM.fail_after = None

    def run_generator(self, world, force, groups=(), adopt=False):
        plan = X.rebuild_plan(world.actors, bool(world.actors), force, adopt)
        if plan.action in ("keep", "abort"):
            return plan, None
        world.apply(plan)
        individual = world.generate(self.data, groups)
        return plan, individual

    def snapshot(self, world):
        return sorted(a.get_actor_label() for a in world.actors if a.get_actor_label().startswith("SPN_"))

    def test_rebuild_is_idempotent_and_keeps_foreign_actors(self):
        foreign = [FakeActor("MyNotes"), FakeActor("SPN_HandMade", ["Hand"]), FakeActor("Landscape")]
        world = World(foreign)
        plan, n = self.run_generator(world, force=False)
        self.assertEqual(plan.action, "keep")                                 # a level with foreign actors counts as existing: nothing changes
        world = World()
        plan, n = self.run_generator(world, force=True)
        self.assertEqual(plan.action, "create")
        first = self.snapshot(world)
        world.actors += foreign
        for _ in range(3):
            plan, n = self.run_generator(world, force=True)
            self.assertEqual(plan.action, "rebuild")
            self.assertEqual(self.snapshot(world), sorted(first + ["SPN_HandMade"]))      # same generated set, plus the hand-made one that was never ours
            self.assertEqual(len([a for a in world.actors if a.get_actor_label() == "MyNotes"]), 1)
            self.assertEqual(len([a for a in world.actors if a.get_actor_label() == "Landscape"]), 1)
            self.assertEqual(X.verify_level(world.actors, X.expected_counts(self.data, [], n)), [])

    def test_a_force_run_never_stacks_duplicates(self):
        world = World()
        self.run_generator(world, force=True)
        count = len(world.actors)
        for _ in range(4):
            self.run_generator(world, force=True)
        self.assertEqual(len(world.actors), count)
        labels = [a.get_actor_label() for a in world.actors]
        self.assertEqual(len(labels), len(set(labels)))

    def test_legacy_levels_abort_untouched_until_adopted(self):
        old = [FakeActor("SPN_MV_Floor"), FakeActor("SPN_MV_Ramp_05"), FakeActor("SPN_Sun"), FakeActor("Hand")]
        plan = X.rebuild_plan(old, True, True, False)
        self.assertEqual((plan.action, plan.remove), ("abort", []))
        plan = X.rebuild_plan(old, True, True, True)
        self.assertEqual(plan.action, "rebuild")
        self.assertEqual([a.get_actor_label() for a in plan.remove], ["SPN_MV_Floor", "SPN_MV_Ramp_05", "SPN_Sun"])
        self.assertEqual(X.rebuild_plan(old, True, False, False).action, "keep")
        self.assertEqual(X.rebuild_plan([], False, True, False).action, "create")

    def test_verify_level_catches_incomplete_duplicated_and_untagged_levels(self):
        world = World()
        n = world.generate(self.data)
        expected = X.expected_counts(self.data, [], n)
        self.assertEqual(X.verify_level(world.actors, expected), [])
        missing = [a for a in world.actors if a.get_actor_label() != "SPN_MV_Door_mv_hut_door"]
        self.assertTrue(any("doors" in p for p in X.verify_level(missing, expected)))
        self.assertTrue(any("duplicate" in p for p in X.verify_level(world.actors + [FakeActor("SPN_MV_Floor", ["SPN_GEN"])], expected)))
        self.assertTrue(any("untagged" in p for p in X.verify_level(world.actors + [FakeActor("SPN_D_Extra")], expected)))
        self.assertTrue(any("shapes" in p for p in X.verify_level([a for a in world.actors if a.get_actor_label() != "SPN_MV_Floor"], expected)))

    def test_instance_groups_are_built_once_without_duplicate_geometry(self):
        world = World()
        group = self.groups[0]
        native = {"cube": (100.0, 100.0, 100.0), "cylinder": (100.0, 100.0, 100.0), "sphere": (100.0, 100.0, 100.0)}
        fallback_calls = []
        made = X.spawn_instance_group(world.spawn, lambda shape: "mesh:" + shape, lambda key: "mat:" + key, native, group, fallback_calls.append)
        self.assertEqual((made, fallback_calls), (0, []))
        actor = world.actors[0]
        comp = actor.components[0]
        self.assertEqual(len(comp.instances), len(group["instances"]))
        self.assertEqual((comp.mesh, comp.material), ("mesh:" + group["shape"], "mat:" + group["material"]))
        self.assertFalse(actor.destroyed)

    def test_a_failing_group_is_removed_before_the_fallback_so_nothing_exists_twice(self):
        world = World()
        group = next(g for g in self.groups if len(g["instances"]) >= 3)
        native = {"cube": (100.0, 100.0, 100.0), "cylinder": (100.0, 100.0, 100.0), "sphere": (100.0, 100.0, 100.0)}
        FakeISM.fail_after = 2
        fallback_calls = []
        made = X.spawn_instance_group(world.spawn, lambda shape: "m", lambda key: "k", native, group, fallback_calls.append)
        self.assertEqual(made, len(group["instances"]))
        self.assertEqual([p["label"] for p in fallback_calls], [p["label"] for p in group["instances"]])   # every prim exactly once, in order
        self.assertTrue(world.actors[0].destroyed)                                                          # the half-built instance actor is gone
        FakeISM.fail_after = None

    def test_a_refused_destroy_stops_the_run(self):
        world = World()
        group = next(g for g in self.groups if len(g["instances"]) >= 3)
        FakeISM.fail_after = 1

        def spawn_refusing(cls, location, rotation=(0, 0, 0), label=None):
            a = world.spawn(cls, location, rotation, label)
            a.refuse_destroy = True
            return a
        with self.assertRaises(RuntimeError):
            X.spawn_instance_group(spawn_refusing, lambda s: "m", lambda k: "k", {"cube": (100.0,) * 3, "cylinder": (100.0,) * 3, "sphere": (100.0,) * 3}, group, lambda p: None)
        FakeISM.fail_after = None

    def test_instanced_and_individual_records_are_identical(self):
        native = {"cube": (100.0, 100.0, 100.0), "cylinder": (100.0, 100.0, 100.0), "sphere": (100.0, 100.0, 100.0)}
        by_label = {p["label"]: p for p in self.data["prims"]}
        seen = 0
        for g in self.groups:
            for p in g["instances"]:
                rec = I.spawn_record(p, native[p["shape"]])
                same = I.spawn_record(by_label[p["label"]], native[p["shape"]])
                self.assertEqual(rec, same)
                self.assertEqual(rec["location"], p["center"])
                self.assertEqual(rec["rotation"], p["rot"])
                self.assertEqual(rec["scale"], [v / 100.0 for v in p["size"]])
                self.assertEqual((rec["shape"], rec["material"], rec["collision"]), (g["shape"], g["material"], "block_all"))
                seen += 1
        self.assertEqual(seen, len(self.data["prims"]) - len(L.build_layout()["prims"]))
        self.assertEqual(I.spawn_record({"shape": "cube", "material": "grid_gray", "center": [1, 2, 3], "rot": [0, 90, 0], "size": [200, 50, 10]}, (200.0, 100.0, 50.0))["scale"], [1.0, 0.5, 0.2])

    def test_script_order_preflight_before_changes_and_verify_before_save(self):
        text = open(os.path.join(HERE, "create_spn_movement_test_map.py"), encoding="utf-8").read()
        main = text[text.index("def main():"):]
        self.assertLess(main.index("preflight(data, grouped)"), main.index("load_level("))
        self.assertLess(main.index("preflight(data, grouped)"), main.index("destroy_actors("))
        self.assertLess(main.index("verify_level("), main.index("save_current_level()"))
        self.assertIn("NOT saved", text)
        pre = text[text.index("def preflight"):text.index("def main():")]
        for needle in ("validate_layout_data", "load_shape", "load_material", "load_class"):
            self.assertIn(needle, pre)


class OwnershipTest(unittest.TestCase):
    def setUp(self):
        X.set_generator("/Game/SpnTest/Lvl_X", "v-test")

    def test_tag_actor_adds_generator_tags_once_and_keeps_existing_ones(self):
        a = FakeActor("SPN_D_Tree_001_Trunk", ["Hand"])
        X.tag_actor(a)
        X.tag_actor(a)
        self.assertEqual(a.tags, ["Hand", "SPN_GEN", "SPN_GEN_MAP:/Game/SpnTest/Lvl_X", "SPN_GEN_VER:v-test"])

    def test_only_tagged_actors_are_owned(self):
        mine = FakeActor("SPN_D_Wall", ["SPN_GEN"])
        hand_named_like_mine = FakeActor("SPN_Notes", [])
        other = FakeActor("MyHouse", [])
        hand_tagged = FakeActor("SPN_Custom", ["Hand"])
        owned, legacy, foreign = X.classify_actors([mine, hand_named_like_mine, other, hand_tagged])
        self.assertEqual(owned, [mine])
        self.assertEqual(legacy, [])
        self.assertEqual(foreign, [hand_named_like_mine, other, hand_tagged])

    def test_untagged_generator_labels_are_legacy_and_adopted_only_on_request(self):
        old = [FakeActor("SPN_MV_Floor"), FakeActor("SPN_D_Road_01"), FakeActor("SPN_Sun"), FakeActor("SPN_PlayerStart")]
        keep = FakeActor("Landscape")
        owned, legacy, foreign = X.classify_actors(old + [keep])
        self.assertEqual((owned, legacy, foreign), ([], old, [keep]))
        owned, legacy, foreign = X.classify_actors(old + [keep], adopt_legacy=True)
        self.assertEqual(owned, old)
        self.assertEqual(foreign, [keep])

    def test_tag_failure_is_not_fatal(self):
        class Broken:
            def get_actor_label(self):
                return "x"
        X.tag_actor(Broken())  # must not raise

    def test_script_uses_the_ownership_rules(self):
        text = open(os.path.join(HERE, "create_spn_movement_test_map.py"), encoding="utf-8").read()
        self.assertTrue("rebuild_plan" in text)
        self.assertTrue("--adopt-legacy" in text)
        self.assertFalse('startswith("SPN_")]' in text)          # the old label-only deletion is gone
        self.assertTrue("NOT saved" in text)
        self.assertLess(text.index("destroy_actors(plan.remove)"), text.index("save_current_level"))

if __name__ == "__main__":
    unittest.main()
