#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Spn/SpnTestDoor.h"
#include "SpnSaveSubsystem.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// Door opened by a character that stands in the swing (decision 2026-11-13, Docs/Development/Door-Swing-Analysis-2026-11-13.md):
// the door asks exactly that character for a short step out of the swing when there is room, keeps waiting when there is none, and never
// moves anybody else. The first tests use a headless world like SpnGame.NightBuild05 without movement (decision and step target); the
// SpnGame.Doors.Walked* tests tick a headless world so the character really walks the step with its movement component and collision
// (corridor, dead end, pawn behind, capsule sizes). The rendered check is SpnQA.Doors / SpnQA.HouseJourney / SpnWorldQA.Integration.
//
// Geometry: door at the origin, yaw 0, the closed leaf runs from the hinge along +X (100 cm), the open leaf along +Y (towards the player).

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FStepWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;
		FString SaveRoot;

		FStepWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			SaveRoot = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			GameInstance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(SaveRoot);
			Wall(FVector(0.f, 0.f, -15.f), FVector(3000.f, 3000.f, 30.f));   // floor, top at z 0 (the step needs walkable floor at its target)
		}

		~FStepWorld()
		{
			for (AActor* Actor : Spawned) { if (IsValid(Actor)) { Actor->Destroy(); } }
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(/*bInformEngineOfWorld*/ false);
			GameInstance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*SaveRoot, false, true);
		}

		ASpnTestDoor* Door(const TCHAR* Id)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestDoor* Door = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(FVector::ZeroVector), Params);
			Door->StableId = Id;
			Door->FinishSpawning(FTransform(FVector::ZeroVector));
			Door->DispatchBeginPlay();
			Spawned.Add(Door);
			return Door;
		}

		/** The game's player character (the C++ class is abstract; the blueprint is the concrete one the game uses). Null if missing. */
		ASupernaturalProtCharacter* Player(const FVector& Location)
		{
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class == nullptr)
			{
				return nullptr;
			}
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASupernaturalProtCharacter* C = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(Location), Params);
			if (C != nullptr) { Spawned.Add(C); }
			return C;
		}

		ACharacter* Pawn(const FVector& Location)
		{
			ACharacter* Pawn = World->SpawnActor<ACharacter>(ACharacter::StaticClass(), FTransform(Location));
			Spawned.Add(Pawn);
			return Pawn;
		}

		void Wall(const FVector& Centre, const FVector& Size)
		{
			AActor* Wall = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(Centre));
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Wall, TEXT("Mesh"));
			Wall->SetRootComponent(Mesh);
			Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
			Mesh->SetRelativeScale3D(Size / 100.f);
			Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			Wall->SetActorLocation(Centre);
			Mesh->RegisterComponent();
			Spawned.Add(Wall);
		}

		static void Swing(ASpnTestDoor& Door, float Seconds)
		{
			for (float T = 0.f; T < Seconds; T += 1.f / 60.f) { Door.Tick(1.f / 60.f); }
		}
	};

	// in the arc of the leaf: 78 cm from the hinge, 50 degrees (same spot as SpnGame.NightBuild05.DoorNeverPushesAPawn)
	const FVector InTheSwing(50.f, 60.f, 100.f);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorStepAsideTest, "SpnGame.Doors.OpenerStepsOutOfTheSwing", Flags)
bool FSpnDoorStepAsideTest::RunTest(const FString&)
{
	FStepWorld W;
	ASpnTestDoor* Door = W.Door(TEXT("step_door"));
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }
	const FVector Before = Player->GetActorLocation();

	TestEqual(TEXT("the player opens the door"), Door->SpnUse(Player).Result, ESpnInteractResult::Changed);
	TestTrue(TEXT("the door remembers who opened it"), Door->GetOpener() == Player);
	FStepWorld::Swing(*Door, 0.5f);
	TestTrue(TEXT("the leaf stops at the player first (physical stop kept)"), Door->IsWaitingForPawn() && Door->GetCurrentAngle() < 45.f);
	TestTrue(TEXT("the door asked the player to step back"), Door->GetStepAsideResult() == ASpnTestDoor::EStepAside::Requested);
	TestTrue(TEXT("the player is stepping aside"), Player->IsSteppingAside());
	TestTrue(TEXT("nothing was teleported"), Player->GetActorLocation().Equals(Before, 0.01f));
	const FVector Target = Player->GetStepAsideTarget();
	const float Clear = Door->GetLeafReach() + Player->GetCapsuleComponent()->GetScaledCapsuleRadius();
	TestTrue(*FString::Printf(TEXT("the step target is out of the swing (%.0f cm from the hinge, needs %.0f)"), FVector::Dist2D(Target, Door->GetHingeLocation()), Clear),
		FVector::Dist2D(Target, Door->GetHingeLocation()) >= Clear);
	TestTrue(*FString::Printf(TEXT("the step is short (%.0f cm)"), FVector::Dist2D(Target, Before)), FVector::Dist2D(Target, Before) <= Door->StepAsideMaxLength);
	TestEqual(TEXT("one step"), Player->GetStepAsideCount(), 1);

	// once the player stands at the target (the walk itself is what the rendered QA checks), the door opens fully
	Player->SetActorLocation(Target);
	FStepWorld::Swing(*Door, 1.5f);
	TestTrue(TEXT("the door finishes opening"), FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->GetTargetAngle(), 0.5f) && !Door->IsWaitingForPawn());
	TestTrue(TEXT("the door is open in the world state"), Door->IsOpen());
	TestEqual(TEXT("still one step (asked once per swing)"), Player->GetStepAsideCount(), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorNoRoomTest, "SpnGame.Doors.NoRoomToStepBackKeepsTheDoorBlocked", Flags)
bool FSpnDoorNoRoomTest::RunTest(const FString&)
{
	FStepWorld W;
	ASpnTestDoor* Door = W.Door(TEXT("tight_door"));
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }
	// a wall right behind the player, across the way out of the swing
	W.Wall(FVector(110.f, 145.f, 100.f), FVector(320.f, 30.f, 300.f));
	const FVector Before = Player->GetActorLocation();

	Door->SpnUse(Player);
	FStepWorld::Swing(*Door, 1.5f);
	TestTrue(TEXT("no room: no step was asked"), Door->GetStepAsideResult() == ASpnTestDoor::EStepAside::NoRoom);
	TestFalse(TEXT("the player is not moving"), Player->IsSteppingAside());
	TestTrue(TEXT("the player was not moved (not into the wall either)"), Player->GetActorLocation().Equals(Before, 0.01f));
	TestTrue(TEXT("the door stays blocked against the player"), Door->IsWaitingForPawn() && Door->GetCurrentAngle() < 45.f);
	TestTrue(TEXT("the door state is open (it finishes when the way is free)"), Door->IsOpen());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorOtherPawnTest, "SpnGame.Doors.OnlyTheOpenerIsAskedToStepAside", Flags)
bool FSpnDoorOtherPawnTest::RunTest(const FString&)
{
	FStepWorld W;
	ASpnTestDoor* Door = W.Door(TEXT("other_door"));
	ASupernaturalProtCharacter* Player = W.Player(FVector(-300.f, -300.f, 100.f));    // opens it from outside the swing
	if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }
	ACharacter* Other = W.Pawn(InTheSwing);                                             // somebody else stands in it
	const FVector OtherBefore = Other->GetActorLocation();

	Door->SpnUse(Player);
	FStepWorld::Swing(*Door, 1.5f);
	TestTrue(TEXT("nobody was asked to step"), Door->GetStepAsideResult() == ASpnTestDoor::EStepAside::None);
	TestFalse(TEXT("the opener does not move"), Player->IsSteppingAside());
	TestTrue(TEXT("the other pawn was not moved"), Other->GetActorLocation().Equals(OtherBefore, 0.01f));
	TestTrue(TEXT("the door waits for the other pawn as before"), Door->IsWaitingForPawn());

	// a door opened by a script (no user) never asks anybody either
	FStepWorld W2;
	ASpnTestDoor* Scripted = W2.Door(TEXT("script_door"));
	ASupernaturalProtCharacter* Standing = W2.Player(InTheSwing);
	if (!TestNotNull(TEXT("second player character spawned"), Standing)) { return false; }
	Scripted->SpnUse(nullptr);
	FStepWorld::Swing(*Scripted, 1.f);
	TestTrue(TEXT("scripted open: no step"), Scripted->GetStepAsideResult() == ASpnTestDoor::EStepAside::None && !Standing->IsSteppingAside());
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------
// Walked steps: a ticking headless world. The character has no controller; bRunPhysicsWithNoController lets its movement component run, so the
// step is the same movement input, collision and floor logic as in play.

namespace
{
	struct FWalkWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;
		FString SaveRoot;
		float MaxFrameMove = 0.f;    // largest horizontal move of the player in one frame (a teleport would show here)
		ACharacter* Watched = nullptr;

		explicit FWalkWorld(bool bFloor = true, float FloorMaxY = 1500.f)
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			SaveRoot = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			GameInstance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(SaveRoot);
			World->InitializeActorsForPlay(FURL());
			World->GetWorldSettings()->NotifyBeginPlay();     // what the game state does: actors spawned from now on begin play at once
			if (bFloor)
			{
				const float MinY = -1500.f;
				Box(FVector(0.f, (MinY + FloorMaxY) * 0.5f, -15.f), FVector(3000.f, FloorMaxY - MinY, 30.f));
			}
		}

		~FWalkWorld()
		{
			// a world that has begun play must end play before it is cleaned up (else world subsystems, e.g. the Water plugin's, keep stale state and
			// a later garbage collection crashes: seen in SpnGame.Input.ConsoleTyping right after these tests)
			World->BeginTearingDown();
			for (AActor* Actor : Spawned) { if (IsValid(Actor)) { Actor->Destroy(); } }
			World->EndPlay(EEndPlayReason::Quit);
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
			GameInstance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*SaveRoot, false, true);
		}

		AActor* Box(const FVector& Centre, const FVector& Size)
		{
			AActor* Actor = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(Centre));
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Actor, TEXT("Mesh"));
			Actor->SetRootComponent(Mesh);
			Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
			Mesh->SetRelativeScale3D(Size / 100.f);
			Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			Actor->SetActorLocation(Centre);
			Mesh->RegisterComponent();
			Spawned.Add(Actor);
			return Actor;
		}

		/** Corridor 140 cm wide (inner faces at x -20 and x 120) running from the door wall along +Y. */
		void Corridor()
		{
			Box(FVector(-35.f, 210.f, 150.f), FVector(30.f, 400.f, 300.f));
			Box(FVector(135.f, 210.f, 150.f), FVector(30.f, 400.f, 300.f));
		}

		ASpnTestDoor* Door(const TCHAR* Id)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestDoor* Door = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(FVector::ZeroVector), Params);
			Door->StableId = Id;
			Door->FinishSpawning(FTransform(FVector::ZeroVector));
			Spawned.Add(Door);
			return Door;
		}

		ASupernaturalProtCharacter* Player(const FVector& Location, float Radius = 0.f)
		{
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class == nullptr) { return nullptr; }
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASupernaturalProtCharacter* C = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(Location), Params);
			if (C == nullptr) { return nullptr; }
			if (Radius > 0.f) { C->GetCapsuleComponent()->SetCapsuleRadius(Radius); }
			// without a controller the character is never possessed: it keeps MOVE_None unless the movement is started here
			C->GetCharacterMovement()->bRunPhysicsWithNoController = true;
			C->GetCharacterMovement()->SetDefaultMovementMode();
			Spawned.Add(C);
			Watched = C;
			return C;
		}

		ACharacter* Pawn(const FVector& Location)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ACharacter* P = World->SpawnActor<ACharacter>(ACharacter::StaticClass(), FTransform(Location), Params);
			Spawned.Add(P);
			return P;
		}

		void Run(float Seconds)
		{
			for (float T = 0.f; T < Seconds; T += 1.f / 60.f)
			{
				const FVector Before = Watched != nullptr ? Watched->GetActorLocation() : FVector::ZeroVector;
				// every tick function runs at most once per engine frame (GFrameCounter); this loop simulates many frames inside one engine frame
				++GFrameCounter;
				World->Tick(LEVELTICK_All, 1.f / 60.f);
				if (Watched != nullptr) { MaxFrameMove = FMath::Max(MaxFrameMove, static_cast<float>(FVector::Dist2D(Before, Watched->GetActorLocation()))); }
			}
		}

		/** True if the player's capsule overlaps any blocking geometry (a wall it was pushed into). */
		bool PlayerInsideSomething(const ACharacter& C) const
		{
			FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnStepOverlap), false, &C);
			const UCapsuleComponent* Capsule = C.GetCapsuleComponent();
			return World->OverlapAnyTestByChannel(C.GetActorLocation(), FQuat::Identity, ECC_Pawn,
				FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius() - 2.f, Capsule->GetScaledCapsuleHalfHeight() - 4.f), Params);
		}
	};

	/** Opens the door as the player, lets the world run and reports what the step did. */
	struct FStepOutcome { ASpnTestDoor::EStepAside Result; FVector Start, End, Target; float Moved; bool bDoorFullyOpen; bool bDoorWaiting; float Angle = 0.f; float WorldTime = 0.f; };

	FStepOutcome OpenAndWalk(FWalkWorld& W, ASpnTestDoor& Door, ASupernaturalProtCharacter& Player, float Seconds = 2.5f)
	{
		W.Run(0.5f);    // settle on the floor
		FStepOutcome O;
		O.Start = Player.GetActorLocation();
		W.MaxFrameMove = 0.f;
		Door.SpnUse(&Player);
		W.Run(Seconds);
		O.Result = Door.GetStepAsideResult();
		O.End = Player.GetActorLocation();
		O.Target = Door.GetLastStepAsideTarget();
		O.Moved = FVector::Dist2D(O.Start, O.End);
		O.bDoorFullyOpen = FMath::IsNearlyEqual(Door.GetCurrentAngle(), Door.GetTargetAngle(), 0.5f);
		O.bDoorWaiting = Door.IsWaitingForPawn();
		O.Angle = Door.GetCurrentAngle();
		O.WorldTime = W.World->GetTimeSeconds();
		return O;
	}

	/** Arrived: at the target or a little beyond it in the step's direction (the character brakes after the input ends: 6-9 cm measured). */
	bool Arrived(const FStepOutcome& O)
	{
		const FVector Dir = (O.Target - O.Start).GetSafeNormal2D();
		const float Along = FVector::DotProduct(FVector(O.End.X - O.Target.X, O.End.Y - O.Target.Y, 0.f), Dir);
		return FVector::Dist2D(O.End, O.Target) <= 12.f && Along >= -6.f;
	}

	FString Describe(const FStepOutcome& O)
	{
		return FString::Printf(TEXT("result %d, start %.0f,%.0f,%.0f, end %.0f,%.0f,%.0f, target %.0f,%.0f, moved %.1f cm, door angle %.1f open %d waiting %d, world time %.2f"),
			static_cast<int32>(O.Result), O.Start.X, O.Start.Y, O.Start.Z, O.End.X, O.End.Y, O.End.Z, O.Target.X, O.Target.Y, O.Moved, O.Angle, O.bDoorFullyOpen, O.bDoorWaiting, O.WorldTime);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorWalkedOpenTest, "SpnGame.Doors.WalkedStepInTheOpen", Flags)
bool FSpnDoorWalkedOpenTest::RunTest(const FString&)
{
	FWalkWorld W;
	ASpnTestDoor* Door = W.Door(TEXT("walk_open"));
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player"), Player)) { return false; }
	const FStepOutcome O = OpenAndWalk(W, *Door, *Player);
	AddInfo(Describe(O));
	TestTrue(TEXT("a step was requested"), O.Result == ASpnTestDoor::EStepAside::Requested);
	TestTrue(TEXT("the character really walked to the target (arrived, braking overrun <= 12 cm)"), Arrived(O));
	TestFalse(TEXT("the step has ended"), Player->IsSteppingAside());
	TestTrue(*FString::Printf(TEXT("walked, not teleported (largest move in one frame %.1f cm)"), W.MaxFrameMove), W.MaxFrameMove < 8.f);
	TestTrue(*FString::Printf(TEXT("short step (%.0f cm <= %.0f)"), O.Moved, Door->StepAsideMaxLength), O.Moved <= Door->StepAsideMaxLength + 4.f);
	TestTrue(TEXT("the door then opened fully"), O.bDoorFullyOpen && !O.bDoorWaiting && Door->IsOpen());
	TestFalse(TEXT("the character is not inside any geometry"), W.PlayerInsideSomething(*Player));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorWalkedCorridorTest, "SpnGame.Doors.WalkedStepInANarrowCorridor", Flags)
bool FSpnDoorWalkedCorridorTest::RunTest(const FString&)
{
	FWalkWorld W;
	W.Corridor();
	ASpnTestDoor* Door = W.Door(TEXT("walk_corridor"));
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player"), Player)) { return false; }
	const float Radius = Player->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FStepOutcome O = OpenAndWalk(W, *Door, *Player);
	AddInfo(Describe(O));
	// the plain radial step would end at about x = 94 + radius > 120: into the right wall. The door must find the way along the corridor.
	TestTrue(TEXT("a step along the corridor was found"), O.Result == ASpnTestDoor::EStepAside::Requested);
	TestTrue(TEXT("the target stays inside the corridor"), O.Target.X - Radius >= -20.f - 0.5f && O.Target.X + Radius <= 120.f + 0.5f);
	TestTrue(TEXT("the character walked there (arrived, braking overrun <= 12 cm)"), Arrived(O));
	TestTrue(TEXT("and stays inside the corridor after braking"), O.End.X - Radius >= -20.f - 0.5f && O.End.X + Radius <= 120.f + 0.5f);
	TestTrue(*FString::Printf(TEXT("walked, not teleported (%.1f cm/frame)"), W.MaxFrameMove), W.MaxFrameMove < 8.f);
	TestTrue(TEXT("beyond the leaf's reach"), FVector::Dist2D(O.End, Door->GetHingeLocation()) >= Door->GetLeafReach() + Radius);
	TestTrue(TEXT("the door opened fully"), O.bDoorFullyOpen && !O.bDoorWaiting);
	TestFalse(TEXT("not inside a wall"), W.PlayerInsideSomething(*Player));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorWalkedDeadEndTest, "SpnGame.Doors.WalkedDeadEndKeepsTheDoorBlocked", Flags)
bool FSpnDoorWalkedDeadEndTest::RunTest(const FString&)
{
	FWalkWorld W;
	W.Corridor();
	W.Box(FVector(50.f, 165.f, 150.f), FVector(140.f, 30.f, 300.f));   // the corridor ends 150 cm behind the door: no way out of the reach
	ASpnTestDoor* Door = W.Door(TEXT("walk_dead_end"));
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player"), Player)) { return false; }
	const FStepOutcome O = OpenAndWalk(W, *Door, *Player);
	AddInfo(Describe(O));
	TestTrue(TEXT("no room: no step"), O.Result == ASpnTestDoor::EStepAside::NoRoom);
	TestTrue(*FString::Printf(TEXT("the character did not move (%.1f cm)"), O.Moved), O.Moved < 1.f);
	TestTrue(TEXT("the door stays blocked against the character (physical stop kept)"), O.bDoorWaiting && Door->GetCurrentAngle() < 45.f);
	TestTrue(TEXT("the door's state is open (it finishes when the way is free)"), Door->IsOpen());
	TestFalse(TEXT("not pushed into a wall"), W.PlayerInsideSomething(*Player));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorWalkedPawnBehindTest, "SpnGame.Doors.WalkedPawnBehindBlocksTheStep", Flags)
bool FSpnDoorWalkedPawnBehindTest::RunTest(const FString&)
{
	FWalkWorld W;
	W.Corridor();
	ASpnTestDoor* Door = W.Door(TEXT("walk_pawn_behind"));
	ACharacter* Other = W.Pawn(FVector(50.f, 140.f, 100.f));   // somebody stands right behind the player in the corridor
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player"), Player)) { return false; }
	W.Run(0.3f);
	const FVector OtherBefore = Other->GetActorLocation();
	const FStepOutcome O = OpenAndWalk(W, *Door, *Player);
	AddInfo(Describe(O));
	TestTrue(TEXT("the pawn behind leaves no room: no step"), O.Result == ASpnTestDoor::EStepAside::NoRoom);
	TestTrue(TEXT("the character did not move"), O.Moved < 1.f);
	TestTrue(*FString::Printf(TEXT("the other pawn was not pushed (%.1f cm)"), FVector::Dist2D(OtherBefore, Other->GetActorLocation())), FVector::Dist2D(OtherBefore, Other->GetActorLocation()) < 1.f);
	TestTrue(TEXT("the door waits"), O.bDoorWaiting);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorWalkedLedgeTest, "SpnGame.Doors.WalkedLedgeBehindStopsTheStep", Flags)
bool FSpnDoorWalkedLedgeTest::RunTest(const FString&)
{
	FWalkWorld W(/*bFloor*/ true, /*FloorMaxY*/ 100.f);   // a porch: the floor ends 100 cm behind the door, a drop behind it
	ASpnTestDoor* Door = W.Door(TEXT("walk_ledge"));
	ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
	if (!TestNotNull(TEXT("player"), Player)) { return false; }
	const FStepOutcome O = OpenAndWalk(W, *Door, *Player);
	AddInfo(Describe(O));
	TestTrue(TEXT("no step off the edge"), O.Result == ASpnTestDoor::EStepAside::NoRoom);
	TestTrue(TEXT("the character stays on the porch"), O.Moved < 1.f && Player->GetCharacterMovement()->IsMovingOnGround());
	TestTrue(TEXT("the door waits"), O.bDoorWaiting);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorWalkedSizesTest, "SpnGame.Doors.WalkedStepForDifferentCharacterSizes", Flags)
bool FSpnDoorWalkedSizesTest::RunTest(const FString&)
{
	// capsule radius in cm -> expected: a step that clears the reach by radius + margin, or none when that needs more than StepAsideMaxLength
	for (const float Radius : {25.f, 35.f, 50.f, 60.f})
	{
		FWalkWorld W;
		ASpnTestDoor* Door = W.Door(*FString::Printf(TEXT("walk_size_%.0f"), Radius));
		ASupernaturalProtCharacter* Player = W.Player(InTheSwing, Radius);
		if (!TestNotNull(TEXT("player"), Player)) { return false; }
		const float Need = Door->GetLeafReach() + Radius + Door->StepAsideMargin;
		const float Shortest = Need - FVector::Dist2D(InTheSwing, Door->GetHingeLocation());
		const FStepOutcome O = OpenAndWalk(W, *Door, *Player);
		AddInfo(FString::Printf(TEXT("radius %.0f: needs %.0f cm from the hinge, shortest step %.0f cm; %s"), Radius, Need, Shortest, *Describe(O)));
		if (Shortest <= Door->StepAsideMaxLength)
		{
			TestTrue(*FString::Printf(TEXT("radius %.0f: steps"), Radius), O.Result == ASpnTestDoor::EStepAside::Requested);
			TestTrue(*FString::Printf(TEXT("radius %.0f: arrived"), Radius), Arrived(O));
			TestTrue(*FString::Printf(TEXT("radius %.0f: ends out of the reach with the margin"), Radius), FVector::Dist2D(O.End, Door->GetHingeLocation()) >= Need - 6.f);
			TestTrue(*FString::Printf(TEXT("radius %.0f: door fully open"), Radius), O.bDoorFullyOpen);
			TestTrue(*FString::Printf(TEXT("radius %.0f: walked (%.1f cm/frame)"), Radius, W.MaxFrameMove), W.MaxFrameMove < 8.f);
		}
		else
		{
			TestTrue(*FString::Printf(TEXT("radius %.0f: %.0f cm > %.0f: no step, the door waits"), Radius, Shortest, Door->StepAsideMaxLength),
				O.Result == ASpnTestDoor::EStepAside::NoRoom && O.Moved < 1.f && O.bDoorWaiting);
		}
	}
	return true;
}

#endif
