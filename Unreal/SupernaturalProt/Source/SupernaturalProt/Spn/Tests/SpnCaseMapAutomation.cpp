#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Spn/Case/SpnCaseCreature.h"
#include "Spn/Case/SpnCaseObjects.h"
#include "Spn/Case/SpnCaseSubsystem.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

#if WITH_DEV_AUTOMATION_TESTS

// The greybox case level /Game/SpnTest/Lvl_SpnCaseMill (NightBuild07, Tools/create_spn_case_mill_map.py): its content, the navigation the creature
// needs, the player's way to the mill, and (in a rendered run) screenshots for the developer. Run it in a -game process with the map:
//   UnrealEditor.exe <uproject> /Game/SpnTest/Lvl_SpnCaseMill -game -ExecCmds="Automation RunTests SpnCaseQA.Mill" -TestExit="Automation Test Queue Empty"
// Not a playtest: nothing here judges whether the case is fun or scary. Deliberately NOT named SpnQA.*: the gameplay QA runner
// (Tools/run_gameplay_qa.ps1) runs every SpnQA.* test on its own maps and expects SpnQa records.

namespace SpnCaseMapTest
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* MapPath = TEXT("/Game/SpnTest/Lvl_SpnCaseMill");

	FString ShotDir() { return FPaths::ProjectSavedDir() / TEXT("SpnCase") / TEXT("shots"); }

	bool PathExists(UWorld* World, const FVector& From, const FVector& To, float& OutLength)
	{
		UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(World, From, To);
		OutLength = Path != nullptr ? Path->GetPathLength() : 0.f;
		return Path != nullptr && Path->IsValid() && !Path->IsPartial();
	}

	/** Puts the player at a place with a view. */
	void Place(const FVector& Location, float Yaw, float Pitch)
	{
		ASupernaturalProtCharacter* Character = SpnMovementTest::FindCharacter();
		if (Character == nullptr) { return; }
		Character->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
		Character->GetCharacterMovement()->StopMovementImmediately();
		if (APlayerController* PC = Cast<APlayerController>(Character->GetController())) { PC->SetControlRotation(FRotator(Pitch, Yaw, 0.f)); }
	}

	/** Requests a screenshot (only rendered runs write a file); a moment after Place, so the camera has settled. */
	void Capture(const TCHAR* Name)
	{
		FScreenshotRequest::RequestScreenshot(ShotDir() / (FString(TEXT("CaseMill_")) + Name + TEXT(".png")), /*bShowUI*/ true, /*bAddFilenameSuffix*/ false);
	}

	void Shot(FAutomationTestBase& Test, const TCHAR* Name, const FVector& Location, float Yaw, float Pitch)
	{
		const FString Id = Name;
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Location, Yaw, Pitch] { Place(Location, Yaw, Pitch); return true; }));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.8f));
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Id] { Capture(*Id); return true; }));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseMillMapTest, "SpnCaseQA.Mill.ContentNavigationAndShots", SpnCaseMapTest::Flags)
bool FSpnCaseMillMapTest::RunTest(const FString&)
{
	using namespace SpnCaseMapTest;
	if (!FPackageName::DoesPackageExist(MapPath))
	{
		AddError(TEXT("Lvl_SpnCaseMill is missing: run Tools/create_spn_case_mill_map.py"));
		return false;
	}
	AutomationOpenMap(MapPath);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));
	// the navigation mesh is built when the level starts (runtime generation, the whole area): wait until it is done, at most 60 s
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Started = FPlatformTime::Seconds()]
	{
		const UWorld* World = SpnMovementTest::FindPlayWorld();
		UNavigationSystemV1* Nav = World != nullptr ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(const_cast<UWorld*>(World)) : nullptr;
		const bool bBuilding = Nav != nullptr && Nav->IsNavigationBuildInProgress();
		if (!bBuilding || FPlatformTime::Seconds() - Started > 60.0)
		{
			UE_LOG(LogTemp, Display, TEXT("SpnCaseQA.Mill: navigation %s after %.1f s"), bBuilding ? TEXT("STILL BUILDING") : TEXT("built"), FPlatformTime::Seconds() - Started);
			return true;
		}
		return false;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		UWorld* World = SpnMovementTest::FindPlayWorld();
		ASupernaturalProtCharacter* Character = SpnMovementTest::FindCharacter();
		if (World == nullptr || Character == nullptr)
		{
			AddError(TEXT("no play world / player character"));
			return true;
		}
		int32 Creatures = 0, Bells = 0, Bottles = 0, Autosaves = 0, Doors = 0;
		ASpnCreature* Creature = nullptr;
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (ASpnCreature* C = Cast<ASpnCreature>(*It)) { ++Creatures; Creature = C; }
			Bells += It->IsA<ASpnBell>() ? 1 : 0;
			Bottles += It->IsA<ASpnThrowablePickup>() ? 1 : 0;
			Autosaves += It->IsA<ASpnAutosavePoint>() ? 1 : 0;
			Doors += It->IsA<ASpnTestDoor>() ? 1 : 0;
		}
		TestEqual(TEXT("one creature"), Creatures, 1);
		TestEqual(TEXT("one bell"), Bells, 1);
		TestEqual(TEXT("five bottles"), Bottles, 5);
		TestEqual(TEXT("two autosave points"), Autosaves, 2);
		TestEqual(TEXT("six doors (library + five in the mill)"), Doors, 6);
		USpnCaseSubsystem* Case = World->GetSubsystem<USpnCaseSubsystem>();
		TestTrue(TEXT("the case subsystem knows the creature"), Case != nullptr && Case->GetCreatures().Num() == 1);
		TestFalse(TEXT("nobody is hunted at the start: saving allowed"), Case == nullptr || Case->IsSaveBlocked());
		TestEqual(TEXT("full health at the start"), Case != nullptr ? Case->GetHealth() : 0.f, Case != nullptr ? Case->GetMaxHealth() : -1.f);

		const UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
		TestTrue(TEXT("a navigation mesh exists"), Nav != nullptr && Nav->GetDefaultNavDataInstance() != nullptr);
		float Length = 0.f;
		const FVector Start = Character->GetActorLocation();
		TestTrue(*FString::Printf(TEXT("the player can walk from the start to the mill (%.0f m)"), Length / 100.f), PathExists(World, Start, FVector(0.f, 3800.f, 100.f), Length));
		TestTrue(*FString::Printf(TEXT("and to the library door (%.0f m)"), Length / 100.f), PathExists(World, Start, FVector(2300.f, 0.f, 100.f), Length));
		if (Creature != nullptr)
		{
			TestTrue(*FString::Printf(TEXT("the creature has a patrol (%d points)"), Creature->PatrolPoints.Num()), Creature->PatrolPoints.Num() >= 4);
			int32 Reachable = 0;
			for (const FVector& P : Creature->PatrolPoints)
			{
				float L = 0.f;
				const bool bOk = PathExists(World, Creature->GetActorLocation(), P, L);
				Reachable += bOk ? 1 : 0;
				if (!bOk) { AddInfo(FString::Printf(TEXT("patrol point %s: no full path from %s"), *P.ToCompactString(), *Creature->GetActorLocation().ToCompactString())); }
			}
			TestEqual(TEXT("every patrol point is reachable for the creature (doors on its round stand open)"), Reachable, Creature->PatrolPoints.Num());
			TestTrue(TEXT("the creature is far from the player start"), FVector::Dist2D(Creature->GetActorLocation(), Start) > 4000.f);
		}
		Character->bEmitFootstepNoise = false; // the shots below teleport the player; no noise for the creature
		return true;
	}));
	Shot(*this, TEXT("01_start"), FVector(0.f, -1800.f, 100.f), 90.f, -4.f);
	Shot(*this, TEXT("02_road_to_mill"), FVector(0.f, 2400.f, 100.f), 90.f, 2.f);
	Shot(*this, TEXT("03_mill_hall"), FVector(-150.f, 4120.f, 100.f), 75.f, 10.f);
	Shot(*this, TEXT("04_library"), FVector(2600.f, -250.f, 100.f), 40.f, -2.f);
	return true;
}

#endif
