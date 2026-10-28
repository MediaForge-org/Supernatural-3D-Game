"""Static checks of the new Unreal C++ sources that cannot be compiled on Fedora (no engine). They catch the mistakes a compiler would
catch first (missing includes, generated-header order, undefined members, missing module dependencies). They do NOT replace a build:
macros, UHT parsing and API signatures are only confirmed by compiling on Windows.

Run from Unreal/SupernaturalProt:  python3 -m unittest Tools.test_spn_unreal_static
"""

import os
import re
import unittest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPN = os.path.join(ROOT, "Source", "SupernaturalProt", "Spn")
BUILD_CS = os.path.join(ROOT, "Source", "SupernaturalProt", "SupernaturalProt.Build.cs")

# Engine header -> module that must be a dependency of the game module (only the ones the new files use).
HEADER_MODULE = {
    "CoreMinimal.h": "Core", "Components/ActorComponent.h": "Engine", "GameFramework/Character.h": "Engine",
    "GameFramework/CharacterMovementComponent.h": "Engine", "Components/CapsuleComponent.h": "Engine", "HAL/IConsoleManager.h": "Core", "Engine/World.h": "Engine",
}


def read(path):
    with open(path, encoding="utf-8") as handle:
        return handle.read()


class DiagnosticsSourceTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.h = read(os.path.join(SPN, "SpnMovementDiagnostics.h"))
        cls.cpp = read(os.path.join(SPN, "SpnMovementDiagnostics.cpp"))
        cls.build = read(BUILD_CS)

    def test_header_structure(self):
        self.assertTrue(self.h.startswith("#pragma once"))
        includes = re.findall(r'#include "([^"]+)"', self.h)
        self.assertEqual(includes[-1], "SpnMovementDiagnostics.generated.h", "the .generated.h include must come last")
        self.assertEqual(self.h.count("GENERATED_BODY()"), 1)
        self.assertEqual(self.h.count("UCLASS("), 1)
        self.assertRegex(self.h, r"class USpnMovementDiagnostics : public UActorComponent")

    def test_cpp_includes_its_header_first_and_what_it_uses(self):
        includes = re.findall(r'#include "([^"]+)"', self.cpp)
        self.assertEqual(includes[0], "Spn/SpnMovementDiagnostics.h")
        for needed in ("GameFramework/Character.h", "GameFramework/CharacterMovementComponent.h", "HAL/IConsoleManager.h", "Engine/World.h", "SupernaturalProt.h"):
            self.assertIn(needed, includes)

    def test_every_declared_member_function_is_defined(self):
        for fn in ("DumpAndClear", "TickComponent", "IsRecording", "Push", "BeginPlay", "EndPlay", "OnCapsuleHit"):
            self.assertRegex(self.cpp, r"USpnMovementDiagnostics::%s\(" % fn, fn + " is declared but not defined")
        self.assertRegex(self.cpp, r"USpnMovementDiagnostics::USpnMovementDiagnostics\(\)")

    def test_engine_headers_belong_to_modules_the_game_module_depends_on(self):
        modules = set(re.findall(r'"(\w+)"', self.build))
        for source in (self.h, self.cpp):
            for header in re.findall(r'#include "([^"]+)"', source):
                module = HEADER_MODULE.get(header)
                if module:
                    self.assertIn(module, modules, "%s needs the %s module" % (header, module))

    def test_braces_and_parentheses_balance(self):
        for source in (self.h, self.cpp):
            stripped = re.sub(r'"(?:\\.|[^"\\])*"', '""', re.sub(r"//[^\n]*|/\*.*?\*/", "", source, flags=re.S))
            self.assertEqual(stripped.count("{"), stripped.count("}"))
            self.assertEqual(stripped.count("("), stripped.count(")"))

    def test_disabled_component_does_no_work_and_logging_is_bounded(self):
        tick = self.cpp[self.cpp.index("void USpnMovementDiagnostics::TickComponent"):]
        self.assertLess(tick.index("IsRecording()"), tick.index("Cast<ACharacter>"), "the early-out must come before any character lookup")
        # config line, hit lines, frame header and frame lines: all only inside DumpAndClear (a dump), never in the tick or the hit callback
        self.assertEqual(self.cpp.count("UE_LOG("), 4)
        dump = self.cpp[self.cpp.index("void USpnMovementDiagnostics::DumpAndClear"):self.cpp.index("void USpnMovementDiagnostics::TickComponent")]
        self.assertEqual(dump.count("UE_LOG("), 4)
        hit = self.cpp[self.cpp.index("void USpnMovementDiagnostics::OnCapsuleHit"):self.cpp.index("void USpnMovementDiagnostics::DumpAndClear")]
        self.assertLess(hit.index("IsRecording()"), hit.index("Cast<ACharacter>"), "a disabled recorder must ignore hits first")
        self.assertIn("MaxHits", hit)                                   # bounded
        begin = self.cpp[self.cpp.index("void USpnMovementDiagnostics::BeginPlay"):self.cpp.index("void USpnMovementDiagnostics::EndPlay")]
        end = self.cpp[self.cpp.index("void USpnMovementDiagnostics::EndPlay"):self.cpp.index("void USpnMovementDiagnostics::OnCapsuleHit")]
        self.assertIn("AddDynamic", begin)
        self.assertIn("RemoveDynamic", end)                             # bound in BeginPlay, unbound in EndPlay
        self.assertRegex(self.h, r"MaxSamples\s*=\s*1200")              # bounded ring buffer
        self.assertIn("ClampMax", self.h)

    def test_ring_buffer_math_matches_a_model(self):
        # The cpp ring: below MaxSamples append, then overwrite Samples[Head] and advance Head; the dump reads (Head + i) % Num.
        max_samples, samples, head = 5, [], 0
        for value in range(12):
            if len(samples) < max_samples:
                samples.append(value)
            else:
                samples[head] = value
                head = (head + 1) % len(samples)
        self.assertEqual([samples[(head + i) % len(samples)] for i in range(len(samples))], [7, 8, 9, 10, 11])
        self.assertIn("Samples[Head] = Sample;", self.cpp)
        self.assertIn("Head = (Head + 1) % Samples.Num();", self.cpp)
        self.assertIn("Samples[(Head + i) % Samples.Num()]", self.cpp)


class DoorSourceTest(unittest.TestCase):
    """Static checks of the M2A3 door changes (ASpnTestDoor::IsClosingObstructed / IsLocked). NOT a compile and NOT a runtime test: the overlap
    query must be verified on Windows (a door closing onto a standing player)."""

    @classmethod
    def setUpClass(cls):
        cls.h = read(os.path.join(SPN, "SpnTestDoor.h"))
        cls.cpp = read(os.path.join(SPN, "SpnTestDoor.cpp"))

    def test_declared_members_are_defined(self):
        for fn in ("IsLocked", "IsClosingObstructed", "SpnUse", "SpnPromptText"):
            self.assertRegex(self.cpp, r"ASpnTestDoor::%s\(" % fn)
        self.assertIn("bool IsClosingObstructed() const;", self.h)

    def test_obstruction_is_checked_before_any_state_change_and_only_when_closing(self):
        use = self.cpp[self.cpp.index("FSpnInteractStatus ASpnTestDoor::SpnUse"):]
        self.assertLess(use.index("IsOpen() && IsClosingObstructed()"), use.index("TryInteract"), "the obstruction check must come before the state write")
        block = use[:use.index("TryInteract")]
        self.assertIn("ESpnInteractResult::Refused", block)
        self.assertIn("placeholder:feedback.door.blocked", block)       # same id as spn::core::TestDoorBlockedFeedback()
        self.assertIn("return Blocked;", block)

    def test_query_uses_object_type_pawn_only_and_ignores_itself(self):
        query = self.cpp[self.cpp.index("bool ASpnTestDoor::IsClosingObstructed"):self.cpp.index("FSpnInteractStatus ASpnTestDoor::SpnUse")]
        self.assertIn("FCollisionObjectQueryParams Pawns(ECC_Pawn)", query)
        self.assertIn("OverlapAnyTestByObjectType", query)
        self.assertIn("this);", query)                                  # query params ignore the door actor
        self.assertIn("CeilToInt", query)                               # sampled, bounded number of queries along the swing
        self.assertNotIn("OverlapMulti", query)                         # no allocation of result arrays in this gameplay path

    def test_prompt_reads_the_lock_from_the_world_state(self):
        prompt = self.cpp[self.cpp.index("FString ASpnTestDoor::SpnPromptText"):self.cpp.index("bool ASpnTestDoor::IsOpen")]
        self.assertIn("IsLocked()", prompt)
        self.assertNotIn("bAuthoredLocked", prompt)
        locked = self.cpp[self.cpp.index("bool ASpnTestDoor::IsLocked"):self.cpp.index("bool ASpnTestDoor::IsClosingObstructed")]
        self.assertIn("TestDoorLockedKey", locked)
        self.assertIn("bAuthoredLocked", locked)                        # the authored value is only the fallback


if __name__ == "__main__":
    unittest.main()
