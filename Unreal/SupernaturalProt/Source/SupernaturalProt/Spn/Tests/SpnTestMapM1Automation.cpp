#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Engine/Level.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Misc/AutomationTest.h"
#include "SpnMovement.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"

#if WITH_DEV_AUTOMATION_TESTS

// M1 movement geometry of /Game/SpnTest/Lvl_SpnDoorTest (ramp, staircase, low wall; Docs/Proposals/0003 section 5):
// SpnGame.TestMap.M1Layout checks the generated map, SpnGame.TestMap.M1Traversal walks the character over it.
// All shapes are PLACEHOLDER test values, see Tools/create_spn_door_test_map.py.

namespace
{
	const TCHAR* const TestMapPackage = TEXT("/Game/SpnTest/Lvl_SpnDoorTest");
	const TCHAR* const TestMapObject = TEXT("/Game/SpnTest/Lvl_SpnDoorTest.Lvl_SpnDoorTest");

	constexpr int32 StairCount = 10;
	// Values of the script (M1_X, RAMP_Y, STAIRS_Y, WALL_*); the layout test keeps both in step.
	constexpr float M1X = 900.f;
	constexpr float RampY = -800.f;
	constexpr float StairsY = 0.f;
	constexpr float WallX = 1300.f;
	constexpr float WallY = 800.f;
	constexpr float WallThickness = 40.f;
	constexpr float CharacterStandZ = 97.f; // capsule half height 96 on a floor at Z = 0

	UWorld* FindPlayWorld()
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
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestMapM1LayoutTest, "SpnGame.TestMap.M1Layout",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSpnTestMapM1LayoutTest::RunTest(const FString& Parameters)
{
	const UWorld* Map = LoadObject<UWorld>(nullptr, TestMapObject);
	if (!TestNotNull(TEXT("test map exists"), Map) || !TestNotNull(TEXT("test map has a level"), Map->PersistentLevel.Get()))
	{
		return false;
	}

	TMap<FString, const AStaticMeshActor*> Meshes;
	float LastDoorX = 0.f;
	for (const AActor* Actor : Map->PersistentLevel->Actors)
	{
		if (const AStaticMeshActor* Mesh = Cast<AStaticMeshActor>(Actor))
		{
			Meshes.Add(Mesh->GetActorLabel(), Mesh);
		}
		else if (Cast<ASpnTestDoor>(Actor) != nullptr)
		{
			LastDoorX = FMath::Max(LastDoorX, Actor->GetActorLocation().X);
		}
	}

	const AStaticMeshActor* const* Floor = Meshes.Find(TEXT("SPN_Floor"));
	if (!TestNotNull(TEXT("floor present"), Floor != nullptr ? *Floor : nullptr))
	{
		return false;
	}
	const FBox FloorBox = (*Floor)->GetComponentsBoundingBox(true);

	const auto CheckSolidBehindDoors = [&](const FString& Label, const AStaticMeshActor* Mesh)
	{
		const FBox Box = Mesh->GetComponentsBoundingBox(true);
		TestTrue(Label + TEXT(" has collision"), Mesh->GetStaticMeshComponent()->IsCollisionEnabled());
		TestTrue(Label + TEXT(" lies behind the doors"), Box.Min.X > LastDoorX + 100.f);
		TestTrue(Label + TEXT(" lies on the floor"), FloorBox.IsInsideXY(Box.GetCenter()) && Box.Min.Z > -30.f); // the pitched ramp slab dips a few cm into the floor
		return Box;
	};

	// Ramp: walkable slope (default walkable floor angle is about 44.8 degrees), rising towards +X.
	if (const AStaticMeshActor* const* Ramp = Meshes.Find(TEXT("SPN_M1_Ramp")); TestNotNull(TEXT("ramp placed"), Ramp != nullptr ? *Ramp : nullptr))
	{
		const FBox Box = CheckSolidBehindDoors(TEXT("ramp"), *Ramp);
		const float Pitch = (*Ramp)->GetActorRotation().Pitch;
		TestTrue(TEXT("ramp slope is 5-40 degrees (walkable)"), Pitch > 5.f && Pitch < 40.f);
		TestTrue(TEXT("ramp rises at least 1.5 m"), Box.Max.Z > 150.f);
		TestTrue(TEXT("ramp starts at the M1 line"), FMath::Abs(Box.Min.X - M1X) < 20.f);
	}

	// Stairs: every tread at most the walkable step height above the previous one (25 cm since M2-A.1); one landing.
	float PreviousTop = 0.f;
	for (int32 Index = 1; Index <= StairCount; ++Index)
	{
		const FString Label = FString::Printf(TEXT("SPN_M1_Stair_%02d"), Index);
		const AStaticMeshActor* const* Step = Meshes.Find(Label);
		if (!TestNotNull(Label + TEXT(" placed"), Step != nullptr ? *Step : nullptr))
		{
			break;
		}
		const FBox Box = CheckSolidBehindDoors(Label, *Step);
		const float Rise = Box.Max.Z - PreviousTop;
		TestTrue(FString::Printf(TEXT("%s rises more than 5 cm and at most the max step height (%.0f cm) above the previous tread"), *Label, FSpnMovementSettings().MaxStepHeight),
			Rise > 5.f && Rise <= FSpnMovementSettings().MaxStepHeight);
		PreviousTop = Box.Max.Z;
	}
	if (const AStaticMeshActor* const* Landing = Meshes.Find(TEXT("SPN_M1_StairLanding")); TestNotNull(TEXT("stair landing placed"), Landing != nullptr ? *Landing : nullptr))
	{
		const FBox Box = CheckSolidBehindDoors(TEXT("landing"), *Landing);
		TestTrue(TEXT("landing is level with the top tread"), FMath::Abs(Box.Max.Z - PreviousTop) < 1.f);
	}

	// Wall: solid and high enough to block (since M2-A also against the 45 cm human jump; the template jump of 127 cm used to clear it).
	if (const AStaticMeshActor* const* Wall = Meshes.Find(TEXT("SPN_M1_Wall")); TestNotNull(TEXT("wall placed"), Wall != nullptr ? *Wall : nullptr))
	{
		const FBox Box = CheckSolidBehindDoors(TEXT("wall"), *Wall);
		TestTrue(TEXT("wall is 50-120 cm high"), Box.Max.Z > 50.f && Box.Max.Z < 120.f);
	}
	return true;
}
#endif // WITH_EDITOR


// Walks the character with DoMove over the three shapes in the running map (PIE in the editor, or -game):
//   UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.TestMap.M1Traversal;Quit" -unattended -nullrhi
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestMapM1TraversalTest, "SpnGame.TestMap.M1Traversal",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnTestMapM1TraversalTest::RunTest(const FString& Parameters)
{
	AutomationOpenMap(TestMapPackage);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	struct FWalk
	{
		ASupernaturalProtCharacter* Character = nullptr;
		double StartTime = -1.0;
		bool bFinished = false;
		bool bReachedGoal = false;
		FVector End = FVector::ZeroVector;
	};
	const TSharedRef<FWalk> State = MakeShared<FWalk>();

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, State]
	{
		const UWorld* World = FindPlayWorld();
		const APlayerController* Controller = World != nullptr ? World->GetFirstPlayerController() : nullptr;
		State->Character = Controller != nullptr ? Cast<ASupernaturalProtCharacter>(Controller->GetPawn()) : nullptr;
		if (TestNotNull(TEXT("third-person character possessed"), State->Character))
		{
			// Camera: spring arm with collision test, so the camera cannot pass through walls.
			const USpringArmComponent* Boom = State->Character->GetCameraBoom();
			TestTrue(TEXT("camera boom does a collision test"), Boom != nullptr && Boom->bDoCollisionTest);
			TestTrue(TEXT("camera boom follows the control rotation"), Boom != nullptr && Boom->bUsePawnControlRotation);
			TestNotNull(TEXT("follow camera exists"), State->Character->GetFollowCamera());
			TestTrue(TEXT("character jumps"), State->Character->GetCharacterMovement()->JumpZVelocity > 0.f);

			// Sprint: Shift is mapped; DoSprintStart / DoSprintEnd set and clear the sprint request on the movement component
			// (M2-A: the speed itself comes from the gait rule, not from MaxWalkSpeed; SpnGame.Movement.Gaits checks the speeds).
			const UInputAction* Sprint = State->Character->GetBoundSprintAction();
			const UEnhancedInputLocalPlayerSubsystem* Input = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
			TestTrue(TEXT("Left Shift is mapped to sprint"), Sprint != nullptr && Input != nullptr && Input->QueryKeysMappedToAction(Sprint).Contains(EKeys::LeftShift));
			USpnCharacterMovementComponent* GaitMovement = State->Character->GetSpnMovement();
			if (TestNotNull(TEXT("character uses USpnCharacterMovementComponent"), GaitMovement))
			{
				TestFalse(TEXT("not sprinting at the start"), GaitMovement->IsSprintRequested());
				State->Character->DoSprintStart();
				TestTrue(TEXT("sprint is requested after DoSprintStart"), GaitMovement->IsSprintRequested());
				State->Character->DoSprintEnd();
				TestFalse(TEXT("sprint request cleared after DoSprintEnd"), GaitMovement->IsSprintRequested());
			}
		}
		return true;
	}));

	// Teleports to Start facing +X, holds "forward" until Goal is true or MaxSeconds pass, then runs Check.
	auto Walk = [this, State](const FVector& Start, float MaxSeconds, TFunction<bool(const FVector&)> Goal, TFunction<void(const FVector&, bool)> Check, float Yaw = 0.f)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([State, Start, Yaw]
		{
			State->bFinished = false;
			State->bReachedGoal = false;
			State->StartTime = -1.0;
			if (State->Character != nullptr)
			{
				State->Character->GetCharacterMovement()->StopMovementImmediately();
				State->Character->SetActorLocation(Start, false, nullptr, ETeleportType::TeleportPhysics);
				State->Character->GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));
			}
			return true;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([State, MaxSeconds, Goal]
		{
			if (State->Character == nullptr)
			{
				return true;
			}
			const double Now = State->Character->GetWorld()->GetTimeSeconds();
			if (State->StartTime < 0.0)
			{
				State->StartTime = Now;
			}
			State->End = State->Character->GetActorLocation();
			if (Goal(State->End))
			{
				State->bReachedGoal = true;
			}
			if (State->bReachedGoal || Now - State->StartTime >= MaxSeconds)
			{
				State->bFinished = true;
				return true;
			}
			State->Character->DoMove(0.f, 1.f);
			return false;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([State, Check]
		{
			if (State->Character != nullptr)
			{
				Check(State->End, State->bReachedGoal);
			}
			return true;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	};

	// Stairs: walk up the ten treads (180 cm) without jumping.
	const float StairTopZ = CharacterStandZ + StairCount * 18.f;
	Walk(FVector(M1X - 150.f, StairsY, CharacterStandZ), 5.f,
		[StairTopZ](const FVector& Location) { return Location.Z > StairTopZ - 15.f; },
		[this, StairTopZ](const FVector& End, bool bReached)
		{
			TestTrue(FString::Printf(TEXT("walked up the stairs (end Z %.0f, needed %.0f)"), End.Z, StairTopZ - 15.f), bReached);
		});

	// Ramp: walk up a 15 degree slope without jumping.
	Walk(FVector(M1X - 150.f, RampY, CharacterStandZ), 6.f,
		[](const FVector& Location) { return Location.Z > CharacterStandZ + 150.f; },
		[this](const FVector& End, bool bReached)
		{
			TestTrue(FString::Printf(TEXT("walked up the ramp (end Z %.0f)"), End.Z), bReached);
		});

	// Wall: walking into it must stop in front of it.
	const float WallFace = WallX - WallThickness / 2.f;
	Walk(FVector(WallFace - 300.f, WallY, CharacterStandZ), 2.f, [](const FVector&) { return false; },
		[this, WallFace](const FVector& End, bool)
		{
			TestTrue(FString::Printf(TEXT("walked up to the wall (end X %.0f)"), End.X), End.X > WallFace - 120.f);
			TestTrue(FString::Printf(TEXT("the wall blocks the character (end X %.0f, wall face %.0f)"), End.X, WallFace), End.X < WallFace);
			TestTrue(TEXT("still on the floor in front of the wall"), FMath::Abs(End.Z - CharacterStandZ) < 20.f);
		});

	// Wall, jumping: since M2-A the jump is a human 45 cm jump (Proposal 0009 section 3), so the 1 m wall can no longer be
	// jumped over. This used to assert the opposite (the template's 127 cm jump); the change is the expected follow-up.
	Walk(FVector(WallFace - 300.f, WallY, CharacterStandZ), 4.f,
		[State, WallFace](const FVector& Location)
		{
			if (Location.X > WallFace - 150.f)
			{
				State->Character->Jump();
			}
			return Location.X > WallX + WallThickness / 2.f + 60.f;
		},
		[this, WallFace](const FVector& End, bool bReached)
		{
			TestFalse(FString::Printf(TEXT("the 1 m wall is not crossed by jumping (end X %.0f)"), End.X), bReached);
			TestTrue(FString::Printf(TEXT("still in front of the wall (end X %.0f)"), End.X), End.X < WallFace);
		});

	// Wall, diagonal: walking into the wall at 45 degrees must slide along it, not stick.
	// Movement investigation after Night Build 01: both properties are checked at the moment the slide has been shown (Y past WallY + 100), not after a
	// fixed 3.5 s. The fixed time only worked while the ground steering slowed the slide down by a feedback loop with the wall (the defect fixed in
	// USpnCharacterMovementComponent::CalcVelocity); at the engine's slide speed (about 350 * cos 45 = 247 cm/s) the character reaches the end of this 6 m
	// wall within 3.5 s and walks round it, which is correct and not what this check is about. Nothing is relaxed: still stopped by the wall, still sliding.
	Walk(FVector(WallFace - 300.f, WallY - 280.f, CharacterStandZ), 3.5f, [](const FVector& Location) { return Location.Y > WallY + 100.f; },
		[this, WallFace](const FVector& End, bool bReached)
		{
			TestTrue(FString::Printf(TEXT("diagonal: slid along the wall (end Y %.0f, started at %.0f)"), End.Y, WallY - 280.f), bReached && End.Y > WallY + 100.f);
			TestTrue(FString::Printf(TEXT("diagonal: stopped by the wall while sliding (end X %.0f, face %.0f)"), End.X, WallFace), End.X < WallFace);
		},
		45.f);
	return true;
}

#endif