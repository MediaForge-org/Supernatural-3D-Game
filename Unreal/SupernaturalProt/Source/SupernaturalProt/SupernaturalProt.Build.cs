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
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
