"""Unit tests for asset_manifest.py (plain Python, no Unreal): the origin rules decide the licence column of the manifest, so a wrong match
would silently mark third-party content as own work."""

import json
import os
import tempfile
import unittest

import asset_manifest as am


class ClassifyTest(unittest.TestCase):
    def origin(self, rel):
        return am.classify(rel)[0]

    def test_third_party_folders_are_third_party(self):
        self.assertIn("third party", self.origin("Fab/Megascans/3D/Rock/High/x.uasset"))
        self.assertIn("third party", self.origin("Megaplant_Library/Tree_Norway_Spruce/a.uasset"))

    def test_fab_derived_is_not_plain_own_work(self):
        origin, licence, _ = am.classify("SpnWorld/FabDerived/SM_Fern_A.uasset")
        self.assertIn("derivative of Fab", origin)
        self.assertIn("same restrictions", licence)

    def test_specific_prefix_wins_over_its_parent(self):
        self.assertIn("own data", self.origin("SpnWorld/Data/SpnWorldMap.json"))
        self.assertIn("world generator", self.origin("SpnWorld/Meshes/SM_House.uasset"))
        self.assertIn("by hand", self.origin("Characters/ProtagonistV1/MHC_ProtagonistV1.uasset"))
        self.assertIn("template", self.origin("Characters/Mannequins/Meshes/SKM_Quinn.uasset"))

    def test_protagonist_is_marked_not_regenerable(self):
        self.assertIn("NOT regenerable", am.classify("Characters/ProtagonistV1/x.uasset")[2])

    def test_metahuman_generated_content_is_not_cleared_for_public(self):
        self.assertIn("not cleared", am.classify("SpnCharacterPrep/MetaHumanTest/RTG.uasset")[1])

    def test_unknown_folder_is_reported_as_unknown(self):
        self.assertEqual(self.origin("SomethingNew/a.uasset"), "unclassified")


class NeededByTest(unittest.TestCase):
    def test_roots_are_collected_per_file(self):
        report = {
            "/Game/SpnTest/Lvl_A": {"exists": True, "packages": {"/Game/SpnTest/M": {"file": "Content/SpnTest/M.uasset", "bytes": 1}}},
            "/Game/SpnTest/Lvl_B": {"exists": True, "packages": {"/Game/SpnTest/M": {"file": "Content/SpnTest/M.uasset", "bytes": 1}}},
            "/Game/Missing": {"exists": False, "packages": {}},
        }
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "deps.json")
            with open(path, "w", encoding="utf-8") as fh:
                json.dump(report, fh)
            need = am.needed_by(path)
        self.assertEqual(need["Content/SpnTest/M.uasset"], ["Lvl_A", "Lvl_B"])

    def test_missing_deps_file_gives_no_needs(self):
        self.assertEqual(am.needed_by(os.path.join(tempfile.gettempdir(), "does_not_exist_nb06.json")), {})


if __name__ == "__main__":
    unittest.main()
