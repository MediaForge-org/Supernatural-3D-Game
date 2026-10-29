using System;
using System.IO;
using EpicGames.Core;
using UnrealBuildTool;

// Integration layer between Unreal and the engine-independent SPN C++20 core
// (repository "Supernatural-3D-Game"). The core sources are compiled by UBT
// straight from that repository, with this target's compiler, CRT and
// configuration flags, so no prebuilt library has to match Unreal's ABI.
// Nothing from the core is copied into this project.
public class SpnCore : ModuleRules
{
	public SpnCore(ReadOnlyTargetRules Target) : base(Target)
	{
		// Core repository root: SPN_CORE_ROOT, else the repository containing this project
		// (<repo>/Unreal/SupernaturalProt), else a sibling checkout next to this project.
		string CoreRoot = Environment.GetEnvironmentVariable("SPN_CORE_ROOT");
		if (string.IsNullOrEmpty(CoreRoot))
		{
			CoreRoot = Path.Combine(ModuleDirectory, "..", "..", "..", "..");
			if (!File.Exists(Path.Combine(CoreRoot, "Core", "include", "spn", "core", "WorldFacts.h")))
			{
				CoreRoot = Path.Combine(ModuleDirectory, "..", "..", "..", "Supernatural-3D-Game");
			}
		}
		CoreRoot = Path.GetFullPath(CoreRoot);

		string CoreDir = Path.Combine(CoreRoot, "Core");
		if (!File.Exists(Path.Combine(CoreDir, "include", "spn", "core", "WorldFacts.h")))
		{
			throw new BuildException("SpnCore: SPN core not found at '{0}'. Set SPN_CORE_ROOT to the Supernatural-3D-Game repository root.", CoreRoot);
		}

		// Compile every source file under <core>/Core/src as part of this module.
		ConditionalAddModuleDirectory(new DirectoryReference(CoreDir));
		PrivateIncludePaths.Add(Path.Combine(CoreDir, "include"));

		CppStandard = CppStandardVersion.Cpp20;
		// Core sources include no Unreal headers: no shared PCH forced into them,
		// and no unity build, so each core .cpp stays its own translation unit
		// exactly as in the CMake build.
		PCHUsage = PCHUsageMode.NoPCHs;
		bUseUnity = false;

		// CoreUObject/Engine: USpnWorldStateSubsystem (a game instance subsystem).
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
		// Json: the temporary test item catalog (Content/SpnTest/Inventory/TestItemCatalog.json) is read by USpnInventorySubsystem.
		PrivateDependencyModuleNames.Add("Json");
	}
}
