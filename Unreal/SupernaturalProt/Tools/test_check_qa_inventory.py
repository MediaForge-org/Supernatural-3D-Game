"""Tests of check_qa_inventory.py: a new system without an inventory entry, a missing test and an entry without test or gap must fail."""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import check_qa_inventory as qa


def inventory(*entries):
    return {"entries": list(entries)}


class InventoryCheckTest(unittest.TestCase):
    def check(self, inv, classes=(), headers=(), commands=(), keys=(), tests=()):
        return qa.check(inv, set(classes), set(headers), set(commands), set(keys), set(tests), repo=qa.REPO)

    def test_listed_system_with_a_test_passes(self):
        inv = inventory({"id": "a", "classes": ["ASpnX"], "tests": ["SpnQA.X"]})
        self.assertEqual(self.check(inv, classes=["ASpnX"], tests=["SpnQA.X"]), [])

    def test_new_class_without_entry_fails(self):
        inv = inventory({"id": "a", "classes": ["ASpnX"], "tests": ["SpnQA.X"]})
        problems = self.check(inv, classes=["ASpnX", "ASpnNewFeature"], tests=["SpnQA.X"])
        self.assertTrue(any("ASpnNewFeature" in p for p in problems))

    def test_new_console_command_core_header_and_key_fail(self):
        inv = inventory({"id": "a", "tests": ["SpnQA.X"]})
        problems = self.check(inv, headers=["Quest.h"], commands=["Spn.Warp"], keys=["DefaultCrouchKey"], tests=["SpnQA.X"])
        self.assertEqual(len(problems), 3)

    def test_missing_test_and_missing_gap_fail(self):
        inv = inventory({"id": "a", "tests": ["SpnQA.Gone"]}, {"id": "b"})
        problems = self.check(inv, tests=["SpnQA.Other"])
        self.assertTrue(any("SpnQA.Gone" in p for p in problems))
        self.assertTrue(any("neither a test nor a gap" in p for p in problems))

    def test_gap_without_test_is_allowed_but_stale_names_fail(self):
        inv = inventory({"id": "a", "classes": ["ASpnRemoved"], "gap": "not automatable yet"})
        problems = self.check(inv)
        self.assertEqual(len(problems), 1)
        self.assertIn("no longer exists", problems[0])

    def test_real_repository_is_consistent(self):
        inv = qa.load_inventory()
        ignore = tuple(inv.get("ignore_source_dirs", []))
        problems = qa.check(inv, qa.scan_classes(qa.SOURCE, ignore), qa.scan_core_headers(qa.CORE_HEADERS), qa.scan_console_commands(qa.SOURCE, ignore),
                            qa.scan_key_bindings(qa.SOURCE, ignore), qa.scan_tests(qa.SOURCE))
        self.assertEqual(problems, [])


if __name__ == "__main__":
    unittest.main()
