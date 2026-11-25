#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Spn/Character/SpnCharacterVisualComponent.h"
#include "Spn/SpnNpc.h"
#include "Spn/SpnTestDoor.h"
#include "SpnSaveSubsystem.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// Doors and the pawns around them, beyond the generic ACharacter of SpnGame.NightBuild05.DoorNeverPushesAPawn (NightBuild06, Phase E):
//   * a real NPC (ASpnNpc, its own capsule and collision setup) in the swing of a door the PLAYER opens: the door stops at it, never moves
//     it, never asks it to step aside (only the opener is asked), finishes when it leaves, and refuses to close onto it;
//   * the opener with a different body collision profile (test_195_fit, taller capsule): the step out of the swing still clears the leaf.
// Headless; geometry as SpnDoorStepAsideAutomation.cpp (door at the origin, open leaf along +Y).

namespace SpnDoorNpcTest
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const FVector InTheSwing(50.f, 60.f, 100.f);      // 78 cm from the hinge, 50 degrees: inside the arc of the leaf
	const FVector OutsideTheSwing(-300.f, -300.f, 100.f);

	struct FDoorWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;
		FString SaveRoot;

		FDoorWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			SaveRoot = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			GameInstance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(SaveRoot);
			Floor();
		}

		~FDoorWorld()
		{
			for (AActor* Actor : Spawned) { if (IsValid(Actor)) { Actor->Destroy(); } }
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(/*bInformEngineOfWorld*/ false);
			GameInstance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*SaveRoot, false, true);
		}

		void Floor()
		{
			AActor* Slab = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(FVector(0.f, 0.f, -15.f)));
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Slab, TEXT("Mesh"));
			Slab->SetRootComponent(Mesh);
			Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
			Mesh->SetRelativeScale3D(FVector(30.f, 30.f, 0.3f));
			Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			Slab->SetActorLocation(FVector(0.f, 0.f, -15.f));
			Mesh->RegisterComponent();
			Spawned.Add(Slab);
		}

		ASpnTestDoor* Door(const TCHAR* Id)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestDoor* D = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(FVector::ZeroVector), Params);
			D->StableId = Id;
			D->FinishSpawning(FTransform(FVector::ZeroVector));
			D->DispatchBeginPlay();
			Spawned.Add(D);
			return D;
		}

		ASupernaturalProtCharacter* Player(const FVector& Location)
		{
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class == nullptr) { return nullptr; }
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASupernaturalProtCharacter* C = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(Location), Params);
			if (C != nullptr) { Spawned.Add(C); }
			return C;
		}

		ASpnNpc* Npc(const FVector& Location)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASpnNpc* N = World->SpawnActor<ASpnNpc>(ASpnNpc::StaticClass(), FTransform(Location), Params);
			N->NpcId = 9601;   // test id, not a content NPC
			N->FinishSpawning(FTransform(Location));
			N->DispatchBeginPlay();
			Spawned.Add(N);
			return N;
		}

		static void Swing(ASpnTestDoor& D, float Seconds)
		{
			for (float T = 0.f; T < Seconds; T += 1.f / 60.f) { D.Tick(1.f / 60.f); }
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorNpcSwingTest, "SpnGame.Doors.NpcInTheSwingIsNeverPushed", SpnDoorNpcTest::Flags)
bool FSpnDoorNpcSwingTest::RunTest(const FString&)
{
	using namespace SpnDoorNpcTest;
	FDoorWorld W;
	ASpnTestDoor* Door = W.Door(TEXT("npc_door"));
	ASupernaturalProtCharacter* Player = W.Player(OutsideTheSwing);
	if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }
	ASpnNpc* Npc = W.Npc(InTheSwing);
	if (!TestNotNull(TEXT("NPC spawned"), Npc)) { return false; }
	TestTrue(TEXT("the NPC capsule is a pawn for the door's sweep (object type Pawn)"), Npc->GetCapsuleComponent()->GetCollisionObjectType() == ECC_Pawn);
	const FVector NpcBefore = Npc->GetActorLocation();

	TestEqual(TEXT("the player opens the door"), Door->SpnUse(Player).Result, ESpnInteractResult::Changed);
	FDoorWorld::Swing(*Door, 2.f);
	TestTrue(*FString::Printf(TEXT("the leaf stops short of the NPC (%.1f degrees)"), Door->GetCurrentAngle()), Door->GetCurrentAngle() < 45.f && Door->IsWaitingForPawn());
	TestTrue(TEXT("the NPC was not moved"), Npc->GetActorLocation().Equals(NpcBefore, 0.01f));
	TestTrue(TEXT("nobody was asked to step (the opener is not in the swing, the NPC is not the opener)"),
		Door->GetStepAsideResult() == ASpnTestDoor::EStepAside::None && !Player->IsSteppingAside());

	// the NPC walks off: the door finishes the swing it started
	Npc->SetActorLocation(FVector(300.f, -300.f, 100.f));
	FDoorWorld::Swing(*Door, 2.f);
	TestTrue(TEXT("with the NPC gone the door finishes opening"), FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->GetTargetAngle(), 0.5f) && !Door->IsWaitingForPawn());

	// the NPC steps back into the swing: the player cannot close the door onto it
	Npc->SetActorLocation(InTheSwing);
	TestEqual(TEXT("closing onto the NPC is refused"), Door->SpnUse(Player).Result, ESpnInteractResult::Refused);
	TestTrue(TEXT("refused: the door stays open"), Door->IsOpen());
	TestTrue(TEXT("refused: the NPC was not moved"), Npc->GetActorLocation().Equals(InTheSwing, 0.01f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDoorBodyProfileTest, "SpnGame.Doors.OpenerStepClearsTheLeafForEveryBodyProfile", SpnDoorNpcTest::Flags)
bool FSpnDoorBodyProfileTest::RunTest(const FString&)
{
	using namespace SpnDoorNpcTest;
	for (const TCHAR* Preset : {TEXT(""), TEXT("test_160_fit"), TEXT("test_195_fit")})
	{
		FDoorWorld W;
		ASpnTestDoor* Door = W.Door(TEXT("profile_door"));
		ASupernaturalProtCharacter* Player = W.Player(InTheSwing);
		if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }
		const FString Label = *Preset ? FString(Preset) : FString(TEXT("default capsule"));
		if (*Preset)
		{
			TestTrue(Label + TEXT(": preset applies"), Player->GetVisual()->ApplyPreset(Preset));
			TestFalse(Label + TEXT(": capsule accepted"), Player->GetVisual()->WasCollisionRefused());
		}
		const FSpnBodyCollisionProfile C = Player->GetBodyCollision();
		const FVector Before = Player->GetActorLocation();
		Door->SpnUse(Player);
		FDoorWorld::Swing(*Door, 0.5f);
		TestTrue(*FString::Printf(TEXT("%s (%.0f / %.0f): the leaf stops at the opener"), *Label, C.Radius, C.HalfHeight), Door->IsWaitingForPawn());
		TestTrue(Label + TEXT(": the opener is asked to step out of the swing"), Door->GetStepAsideResult() == ASpnTestDoor::EStepAside::Requested);
		const FVector Target = Player->GetStepAsideTarget();
		const float Clear = Door->GetLeafReach() + C.Radius;
		TestTrue(*FString::Printf(TEXT("%s: the step target clears the leaf (%.0f cm from the hinge, needs %.0f)"), *Label, FVector::Dist2D(Target, Door->GetHingeLocation()), Clear),
			FVector::Dist2D(Target, Door->GetHingeLocation()) >= Clear);
		TestTrue(*FString::Printf(TEXT("%s: the step keeps the feet on the floor (target z %.1f, feet at %.1f)"), *Label, Target.Z, Before.Z - C.HalfHeight),
			FMath::Abs((Target.Z - C.HalfHeight) - (Before.Z - C.HalfHeight)) < 5.f);
		Player->SetActorLocation(Target);
		FDoorWorld::Swing(*Door, 1.5f);
		TestTrue(Label + TEXT(": the door finishes opening once the opener stands at the target"),
			FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->GetTargetAngle(), 0.5f) && !Door->IsWaitingForPawn());
	}
	return true;
}

#endif
