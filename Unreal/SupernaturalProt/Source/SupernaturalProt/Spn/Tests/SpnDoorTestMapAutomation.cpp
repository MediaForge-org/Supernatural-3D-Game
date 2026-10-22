#include "Engine/Engine.h"
#include "Engine/Level.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/WorldSettings.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/FileManager.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Misc/App.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "SpnWorldStateSubsystem.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

#if WITH_DEV_AUTOMATION_TESTS

// Checks for the generated test map /Game/SpnTest/Lvl_SpnDoorTest
// (Unreal/SupernaturalProt/Tools/create_spn_door_test_map.py).

namespace
{
	const TCHAR* const TestMapPackage = TEXT("/Game/SpnTest/Lvl_SpnDoorTest");
	const TCHAR* const TestMapObject = TEXT("/Game/SpnTest/Lvl_SpnDoorTest.Lvl_SpnDoorTest");

	struct FExpectedDoor
	{
		const TCHAR* StableId;
		bool bLocked;
	};
	const FExpectedDoor ExpectedDoors[] = {{TEXT("door_a"), false}, {TEXT("door_b"), false}, {TEXT("door_locked"), true}};

	ASpnTestDoor* FindDoor(const ULevel& Level, const TCHAR* StableId)
	{
		for (AActor* Actor : Level.Actors)
		{
			ASpnTestDoor* Door = Cast<ASpnTestDoor>(Actor);
			if (Door != nullptr && Door->StableId.Equals(StableId, ESearchCase::CaseSensitive))
			{
				return Door;
			}
		}
		return nullptr;
	}

	/** World centre of a door's panel (closed): 50 cm along the actor's X from the hinge, 100 cm up. */
	FVector PanelCentre(const ASpnTestDoor& Door)
	{
		return Door.GetActorTransform().TransformPosition(FVector(50.f, 0.f, 100.f));
	}

	UWorld* FindGameWorld()
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if ((Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE) && Context.World() != nullptr)
			{
				return Context.World();
			}
		}
		return nullptr;
	}
}

#if WITH_EDITOR // actor labels are editor-only
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorTestMapLayoutTest, "SpnGame.TestMap.Layout",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSpnDoorTestMapLayoutTest::RunTest(const FString& Parameters)
{
	const UWorld* Map = LoadObject<UWorld>(nullptr, TestMapObject);
	if (!TestNotNull(TEXT("test map exists"), Map) || !TestNotNull(TEXT("test map has a level"), Map->PersistentLevel.Get()))
	{
		return false;
	}
	const ULevel& Level = *Map->PersistentLevel;

	TArray<const APlayerStart*> PlayerStarts;
	TArray<const ASpnTestDoor*> Doors;
	const AStaticMeshActor* Floor = nullptr;
	for (const AActor* Actor : Level.Actors)
	{
		if (const APlayerStart* Start = Cast<APlayerStart>(Actor))
		{
			PlayerStarts.Add(Start);
		}
		else if (const ASpnTestDoor* Door = Cast<ASpnTestDoor>(Actor))
		{
			Doors.Add(Door);
		}
		else if (const AStaticMeshActor* Mesh = Cast<AStaticMeshActor>(Actor); Mesh != nullptr && Mesh->GetActorLabel() == TEXT("SPN_Floor"))
		{
			Floor = Mesh;
		}
	}

	if (!TestEqual(TEXT("exactly one Player Start"), PlayerStarts.Num(), 1) || !TestNotNull(TEXT("floor present"), Floor))
	{
		return false;
	}
	TestEqual(TEXT("exactly three test doors"), Doors.Num(), 3);

	const FVector Start = PlayerStarts[0]->GetActorLocation();
	const FVector Forward = PlayerStarts[0]->GetActorForwardVector();
	const FBox FloorBox = Floor->GetComponentsBoundingBox(/*bNonColliding*/ true);
	TestTrue(TEXT("floor is solid (collision enabled)"), Floor->GetStaticMeshComponent()->IsCollisionEnabled());
	TestTrue(TEXT("Player Start stands above the floor"),
		FloorBox.IsInsideXY(Start) && Start.Z > FloorBox.Max.Z && Start.Z < FloorBox.Max.Z + 200.f);

	for (const FExpectedDoor& Expected : ExpectedDoors)
	{
		const ASpnTestDoor* Door = FindDoor(Level, Expected.StableId);
		if (!TestNotNull(FString::Printf(TEXT("%s placed"), Expected.StableId), Door))
		{
			continue;
		}
		TestEqual(FString::Printf(TEXT("%s lock"), Expected.StableId), Door->bAuthoredLocked, Expected.bLocked);
		TestFalse(FString::Printf(TEXT("%s authored closed"), Expected.StableId), Door->bAuthoredOpen);
		TestTrue(FString::Printf(TEXT("%s stands on the floor"), Expected.StableId),
			FloorBox.IsInsideXY(Door->GetActorLocation()) && FMath::IsNearlyEqual(Door->GetActorLocation().Z, FloorBox.Max.Z, 1.f));

		// Reachable: a few metres ahead of the Player Start, panel facing it (normal along the start's forward).
		const FVector ToPanel = PanelCentre(*Door) - Start;
		const float Ahead = FVector::DotProduct(ToPanel, Forward);
		TestTrue(FString::Printf(TEXT("%s is 2-8 m ahead of the Player Start"), Expected.StableId), Ahead > 200.f && Ahead < 800.f);
		TestTrue(FString::Printf(TEXT("%s panel faces the Player Start"), Expected.StableId),
			FMath::Abs(FVector::DotProduct(Door->GetActorRightVector(), Forward)) > 0.99f);
	}

	// The map uses the project default game mode, which spawns the third-person character.
	TestNull(TEXT("no game mode override in the test map"), Map->GetWorldSettings()->DefaultGameMode.Get());
	FString GameModePath;
	GConfig->GetString(TEXT("/Script/EngineSettings.GameMapsSettings"), TEXT("GlobalDefaultGameMode"), GameModePath, GEngineIni);
	const UClass* GameModeClass = LoadClass<AGameModeBase>(nullptr, *GameModePath);
	if (TestNotNull(TEXT("project default game mode loads"), GameModeClass))
	{
		const UClass* PawnClass = GameModeClass->GetDefaultObject<AGameModeBase>()->DefaultPawnClass;
		TestTrue(TEXT("default pawn is the third-person character"),
			PawnClass != nullptr && PawnClass->IsChildOf(ASupernaturalProtCharacter::StaticClass()));
	}
	return true;
}
#endif // WITH_EDITOR


namespace
{
	/** With a real renderer (not -nullrhi), saves the game view to Saved/Screenshots for a visual check. */
	void RequestScreenshotIfRendering(const TCHAR* Name)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(FPaths::ScreenShotDir() / FString(Name) + TEXT(".png"), /*bShowUI*/ false, /*bAddFilenameSuffix*/ false);
		}
	}

	/** How far the door's panel is actually swung (world rotation of the mesh), in degrees. */
	float PanelOpening(const ASpnTestDoor& Door)
	{
		TInlineComponentArray<UStaticMeshComponent*> Meshes(&Door);
		return Meshes.Num() > 0 ? FRotator::NormalizeAxis(Meshes[0]->GetComponentRotation().Yaw - Door.GetActorRotation().Yaw) : TNumericLimits<float>::Max();
	}
}

// Plays the map like the manual test: in the editor it runs in PIE (AutomationOpenMap starts a
// play session), in a game process (-game) in the game world:
//   UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.TestMap.PlayDoors;Quit" -unattended -nullrhi
//   UnrealEditor-Cmd.exe <uproject> /Game/SpnTest/Lvl_SpnDoorTest -game -nullrhi -unattended
//       -ExecCmds="Automation RunTests SpnGame.TestMap.PlayDoors;Quit"
// Sequence: open door_a and door_b with E, Spn.SaveWorld, close both, Spn.LoadWorld -> both shown open.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorTestMapPlayTest, "SpnGame.TestMap.PlayDoors",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnDoorTestMapPlayTest::RunTest(const FString& Parameters)
{
	const FString Slot = TEXT("spn_automation_test_map");
	const auto DeleteSlot = [Slot] { IFileManager::Get().DeleteDirectory(*FPaths::GetPath(USpnWorldStateSubsystem::GetSlotFilePath(Slot)), false, true); };
	DeleteSlot();

	AutomationOpenMap(TestMapPackage);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	struct FPlayState
	{
		ASupernaturalProtCharacter* Character = nullptr;
		APlayerController* Controller = nullptr;
		TMap<FString, ASpnTestDoor*> Doors;

		bool IsReady() const { return Doors.Num() == 3 && Character != nullptr; }
		ASpnTestDoor& Door(const TCHAR* StableId) const { return *Doors.FindChecked(StableId); }

		/** Puts the character 1.5 m in front of the panel where it is now (closed or swung open), looking at it, and presses Interact. */
		void UseDoor(const TCHAR* StableId) const
		{
			const ASpnTestDoor& Target = Door(StableId);
			const FVector Panel = Target.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter();
			const FVector Facing = Target.GetActorRightVector(); // panel normal, pointing away from the player side
			Character->SetActorLocation(FVector(Panel.X, Panel.Y, Character->GetActorLocation().Z) - Facing * 150.f, false, nullptr, ETeleportType::TeleportPhysics);
			Controller->SetControlRotation(Facing.Rotation());
			UEnhancedInputLocalPlayerSubsystem* Input = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
			Input->InjectInputForAction(Character->GetBoundInteractAction(), FInputActionValue(true), {}, {});
		}

		/** Runs a console command the way the in-game console does (through the player controller). */
		void Console(const FString& Command) const { Controller->ConsoleCommand(Command, /*bWriteToLog*/ true); }
	};
	const TSharedRef<FPlayState> State = MakeShared<FPlayState>();

	// Checks the world state, the door's presentation value and the panel's real rotation.
	auto ExpectShown = [this, State](const TCHAR* StableId, bool bOpen, const FString& When)
	{
		const ASpnTestDoor& Door = State->Door(StableId);
		const float Expected = bOpen ? Door.OpenAngle : 0.f;
		TestEqual(FString::Printf(TEXT("%s: %s open in the world state"), *When, StableId), Door.IsOpen(), bOpen);
		TestEqual(FString::Printf(TEXT("%s: %s presentation angle"), *When, StableId), Door.GetCurrentAngle(), Expected);
		TestEqual(FString::Printf(TEXT("%s: %s panel rotation"), *When, StableId), PanelOpening(Door), Expected, 1.f);
	};

	auto Step = [this, State](TFunction<void()> Body)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([State, Body]
		{
			if (State->IsReady())
			{
				Body();
			}
			return true;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	};

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, State]
	{
		UWorld* World = FindGameWorld();
		if (!TestNotNull(TEXT("game world running"), World))
		{
			return true;
		}
		TestTrue(TEXT("test map is loaded"), World->GetOutermost()->GetName().EndsWith(TEXT("Lvl_SpnDoorTest")));
		AddInfo(FString::Printf(TEXT("Playing in %s world %s"), World->WorldType == EWorldType::PIE ? TEXT("PIE") : TEXT("game"), *World->GetPathName()));
		State->Controller = World->GetFirstPlayerController();
		if (State->Controller != nullptr)
		{
			SpnTestPlay::ResetWorldState(*State->Controller); // the automation PIE sessions share one game instance
		}
		State->Character = State->Controller != nullptr ? Cast<ASupernaturalProtCharacter>(State->Controller->GetPawn()) : nullptr;
		if (!TestNotNull(TEXT("third-person character spawned and possessed"), State->Character))
		{
			return true;
		}
		TestTrue(TEXT("character is BP_ThirdPersonCharacter"), State->Character->GetClass()->GetName().StartsWith(TEXT("BP_ThirdPersonCharacter")));
		TestTrue(TEXT("character stands on the floor"), FMath::Abs(State->Character->GetActorLocation().Z - 97.f) < 20.f);

		UInputAction* Interact = State->Character->GetBoundInteractAction();
		UEnhancedInputLocalPlayerSubsystem* Input = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(State->Controller->GetLocalPlayer());
		if (!TestTrue(TEXT("E is mapped to Interact"), Interact != nullptr && Input != nullptr && Input->QueryKeysMappedToAction(Interact).Contains(EKeys::E)))
		{
			return true;
		}

		for (const FExpectedDoor& Expected : ExpectedDoors)
		{
			ASpnTestDoor* Door = FindDoor(*World->PersistentLevel, Expected.StableId);
			if (TestNotNull(FString::Printf(TEXT("%s in the game world"), Expected.StableId), Door))
			{
				State->Doors.Add(Expected.StableId, Door);
			}
		}
		return true;
	}));

	Step([State, ExpectShown]
	{
		for (const FExpectedDoor& Expected : ExpectedDoors)
		{
			ExpectShown(Expected.StableId, false, TEXT("start"));
		}
		State->UseDoor(TEXT("door_a"));
	});
	Step([State] { State->UseDoor(TEXT("door_b")); });
	Step([State] { State->UseDoor(TEXT("door_locked")); });
	Step([State, ExpectShown, Slot]
	{
		ExpectShown(TEXT("door_a"), true, TEXT("after E"));
		ExpectShown(TEXT("door_b"), true, TEXT("after E"));
		ExpectShown(TEXT("door_locked"), false, TEXT("after E on the locked door"));
		State->Console(FString::Printf(TEXT("Spn.SaveWorld %s"), *Slot));
		State->UseDoor(TEXT("door_a")); // close after saving
	});
	Step([State] { State->UseDoor(TEXT("door_b")); });
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { RequestScreenshotIfRendering(TEXT("SpnDoorTest_1_closed_before_load")); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, State, ExpectShown, Slot]
	{
		if (State->IsReady())
		{
			TestTrue(TEXT("save file written"), FPaths::FileExists(USpnWorldStateSubsystem::GetSlotFilePath(Slot)));
			ExpectShown(TEXT("door_a"), false, TEXT("closed after saving"));
			ExpectShown(TEXT("door_b"), false, TEXT("closed after saving"));
			State->Console(FString::Printf(TEXT("Spn.LoadWorld %s"), *Slot));
			ExpectShown(TEXT("door_a"), true, TEXT("right after loading"));
			ExpectShown(TEXT("door_b"), true, TEXT("right after loading"));
		}
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([State, ExpectShown, DeleteSlot]
	{
		if (State->IsReady())
		{
			// Still shown open some frames later (nothing swings it back).
			ExpectShown(TEXT("door_a"), true, TEXT("0.5 s after loading"));
			ExpectShown(TEXT("door_b"), true, TEXT("0.5 s after loading"));
			ExpectShown(TEXT("door_locked"), false, TEXT("0.5 s after loading"));
		}
		DeleteSlot();
		RequestScreenshotIfRendering(TEXT("SpnDoorTest_2_after_load"));
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f)); // let the screenshot be taken
	return true;
}

#endif
