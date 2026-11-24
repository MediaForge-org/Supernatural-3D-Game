// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SupernaturalProt : ModuleRules
{
	public SupernaturalProt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"DeveloperSettings",
			"SlateCore",
			"Slate"
		});

		// SpnCore: integration layer to the engine-independent SPN C++20 core. Public, because
		// Spn/SpnCharacterMovementComponent.h (a public header of this module) includes SpnMovement.h.
		PublicDependencyModuleNames.AddRange(new string[] { "SpnCore" });

		// Frame statistics (GGameThreadTime, draw calls, GPU time) of the map performance report test.
		PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore", "RHI" });

		// Visual build tools (Spn/World): camera run specifications are JSON files; the landscape import helper of the world generator
		// (editor-only code paths, see SpnWorldBuildLibrary.cpp).
		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "Landscape" });

		// Character presentation: a test body driven by runtime IK retargeting from the gameplay mesh (Spn/Character, Proposal 0014 C2).
		// IKRig is an engine plugin that is enabled by default.
		PrivateDependencyModuleNames.Add("IKRig");

		PublicIncludePaths.AddRange(new string[] {
			"SupernaturalProt",
			"SupernaturalProt/Variant_Platforming",
			"SupernaturalProt/Variant_Platforming/Animation",
			"SupernaturalProt/Variant_Combat",
			"SupernaturalProt/Variant_Combat/AI",
			"SupernaturalProt/Variant_Combat/Animation",
			"SupernaturalProt/Variant_Combat/Gameplay",
			"SupernaturalProt/Variant_Combat/Interfaces",
			"SupernaturalProt/Variant_Combat/UI",
			"SupernaturalProt/Variant_SideScrolling",
			"SupernaturalProt/Variant_SideScrolling/AI",
			"SupernaturalProt/Variant_SideScrolling/Gameplay",
			"SupernaturalProt/Variant_SideScrolling/Interfaces",
			"SupernaturalProt/Variant_SideScrolling/UI"
		});

		// Editor-only: the PIE-based network automation test (Spn/Tests/SpnMovementNetworkAutomation.cpp) starts a play session.
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
			// 2026-11-15 Protagonist V1 phase 2: USpnMetaHumanPrepLibrary (editor-only code path) generates MetaHuman TEST meshes locally through
			// the public API of the engine's MetaHumanCharacter plugin (already enabled in the .uproject). No runtime dependency.
			PrivateDependencyModuleNames.AddRange(new string[] { "MetaHumanCharacter", "MetaHumanCharacterEditor", "MetaHumanCharacterPalette" });
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
