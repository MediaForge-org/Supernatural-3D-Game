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
    "GameFramework/CharacterMovementComponent.h": "Engine", "HAL/IConsoleManager.h": "Core", "Engine/World.h": "Engine",
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
        for fn in ("DumpAndClear", "TickComponent", "IsRecording", "Push"):
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
            stripped = re.sub(r'"(?:\\.|[^"\\])*"', '""', re.sub(r"//.*|/\*.*?\*/", "", source, flags=re.S))
            self.assertEqual(stripped.count("{"), stripped.count("}"))
            self.assertEqual(stripped.count("("), stripped.count(")"))

    def test_disabled_component_does_no_work_and_logging_is_bounded(self):
        tick = self.cpp[self.cpp.index("void USpnMovementDiagnostics::TickComponent"):]
        self.assertLess(tick.index("IsRecording()"), tick.index("Cast<ACharacter>"), "the early-out must come before any character lookup")
        self.assertEqual(self.cpp.count("UE_LOG("), 2)                  # header line + one line per dumped frame, only inside DumpAndClear
        dump = self.cpp[self.cpp.index("void USpnMovementDiagnostics::DumpAndClear"):self.cpp.index("void USpnMovementDiagnostics::TickComponent")]
        self.assertEqual(dump.count("UE_LOG("), 2)
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


if __name__ == "__main__":
    unittest.main()
