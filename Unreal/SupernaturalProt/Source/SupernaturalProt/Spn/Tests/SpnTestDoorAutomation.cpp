#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for ASpnTestDoor and USpnInteractionComponent (Docs/Proposals/0004, increment 4).
// Each test runs in its own game instance with a dummy game world; no map asset is needed.

namespace
{
	constexpr EAutomationTestFlags SpnDoorTestFlags =
		EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	/** A standalone game instance with its own game world, torn down at scope end. */
	struct FScopedGameWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;

		FScopedGameWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
		}

		~FScopedGameWorld()
		{
			for (AActor* Actor : Spawned)
			{
				if (IsValid(Actor))
				{
					Actor->Destroy();
				}
			}
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(/*bInformEngineOfWorld*/ false);
			GameInstance->RemoveFromRoot();
		}

		USpnWorldStateSubsystem& WorldState() const { return *GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }

		/** Spawns a door, sets it up like a placed one and starts its play. */
		ASpnTestDoor* SpawnDoor(const TCHAR* StableId, const FVector& Location, bool bAuthoredOpen = false, bool bAuthoredLocked = false)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestDoor* Door = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(Location), Params);
			Door->StableId = StableId;
			Door->bAuthoredOpen = bAuthoredOpen;
			Door->bAuthoredLocked = bAuthoredLocked;
			Door->FinishSpawning(FTransform(Location));
			Door->DispatchBeginPlay();
			Spawned.Add(Door);
			return Door;
		}

		/** An actor with an interaction component, standing at Location and facing along Rotation. */
		USpnInteractionComponent* SpawnInteractor(const FVector& Location, const FRotator& Rotation)
		{
			AActor* Owner = World->SpawnActor<AActor>();
			USceneComponent* Root = NewObject<USceneComponent>(Owner, TEXT("Root"));
			Owner->SetRootComponent(Root);
			Root->RegisterComponent();
			Owner->SetActorLocationAndRotation(Location, Rotation);
			USpnInteractionComponent* Interaction = NewObject<USpnInteractionComponent>(Owner, TEXT("Interaction"));
			Interaction->RegisterComponent();
			Spawned.Add(Owner);
			return Interaction;
		}

		TArray<AActor*> Spawned;
	};

	/** Deletes the test slot's directory before and after a test. */
	struct FScopedTestSlot
	{
		FString Name;

		explicit FScopedTestSlot(const TCHAR* InName) : Name(InName) { Delete(); }
		~FScopedTestSlot() { Delete(); }

		void Delete() const
		{
			IFileManager::Get().DeleteDirectory(*FPaths::GetPath(USpnWorldStateSubsystem::GetSlotFilePath(Name)), false, true);
		}
	};

	/** Ticks the door until its swing ends (bounded). */
	void FinishSwing(ASpnTestDoor& Door)
	{
		for (int32 Step = 0; Step < 100 && Door.IsActorTickEnabled(); ++Step)
		{
			Door.Tick(0.05f);
		}
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestDoorUseTest, "SpnGame.TestDoor.UseAndSwing", SpnDoorTestFlags)

bool FSpnTestDoorUseTest::RunTest(const FString& Parameters)
{
	FScopedGameWorld Game;
	ASpnTestDoor* Door = Game.SpawnDoor(TEXT("door_a"), FVector::ZeroVector);

	TestFalse(TEXT("closed by default"), Door->IsOpen());
	TestEqual(TEXT("closed door shows angle 0"), Door->GetCurrentAngle(), 0.f);
	TestFalse(TEXT("closed door does not tick"), Door->IsActorTickEnabled());

	TestEqual(TEXT("Use opens"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestTrue(TEXT("world state says open"),
		Game.WorldState().ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(TEXT("door_a"))) == TOptional<bool>(true));
	TestTrue(TEXT("door reads open"), Door->IsOpen());
	TestEqual(TEXT("swing target is the open angle"), Door->GetTargetAngle(), Door->OpenAngle);
	TestTrue(TEXT("door swings (ticks)"), Door->IsActorTickEnabled());

	Door->Tick(0.25f); // 180 deg/s
	TestEqual(TEXT("swing is gradual"), Door->GetCurrentAngle(), 45.f);
	FinishSwing(*Door);
	TestEqual(TEXT("swing ends open"), Door->GetCurrentAngle(), Door->OpenAngle);
	TestFalse(TEXT("tick stops after the swing"), Door->IsActorTickEnabled());

	TestEqual(TEXT("Use again closes"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	FinishSwing(*Door);
	TestFalse(TEXT("door reads closed"), Door->IsOpen());
	TestEqual(TEXT("swing ends closed"), Door->GetCurrentAngle(), 0.f);
	TestEqual(TEXT("two journaled changes"), Game.WorldState().GetRevision(), uint64{2});

	// Authored open: shown open at start, Use closes it.
	ASpnTestDoor* OpenDoor = Game.SpawnDoor(TEXT("door_b"), FVector(300.f, 0.f, 0.f), /*bAuthoredOpen*/ true);
	TestTrue(TEXT("authored open door reads open"), OpenDoor->IsOpen());
	TestEqual(TEXT("authored open door starts at the open angle"), OpenDoor->GetCurrentAngle(), OpenDoor->OpenAngle);
	TestEqual(TEXT("Use closes the authored open door"), OpenDoor->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestFalse(TEXT("authored open door now closed"), OpenDoor->IsOpen());

	// Locked: refused, nothing written, nothing moves.
	ASpnTestDoor* LockedDoor = Game.SpawnDoor(TEXT("door_c"), FVector(600.f, 0.f, 0.f), false, /*bAuthoredLocked*/ true);
	const uint64 RevisionBefore = Game.WorldState().GetRevision();
	const FSpnInteractStatus Refused = LockedDoor->SpnUse(nullptr);
	TestEqual(TEXT("locked door refuses"), Refused.Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("refusal feedback"), Refused.Feedback, FString(TEXT("placeholder:feedback.door.locked")));
	TestEqual(TEXT("refusal writes nothing"), Game.WorldState().GetRevision(), RevisionBefore);
	TestFalse(TEXT("locked door does not swing"), LockedDoor->IsActorTickEnabled());
	TestFalse(TEXT("other doors unaffected"), Door->IsOpen());

	// Invalid id: Use fails without writing.
	AddExpectedMessagePlain(TEXT("breaks the id rule"), ELogVerbosity::Error, EAutomationExpectedMessageFlags::Contains, 1);
	AddExpectedMessagePlain(TEXT("Use failed: InvalidId"), ELogVerbosity::Error, EAutomationExpectedMessageFlags::Contains, 1);
	ASpnTestDoor* BadDoor = Game.SpawnDoor(TEXT("Bad Door"), FVector(900.f, 0.f, 0.f));
	TestEqual(TEXT("invalid id is reported"), BadDoor->SpnUse(nullptr).Result, ESpnInteractResult::InvalidId);
	TestEqual(TEXT("invalid id writes nothing"), Game.WorldState().GetRevision(), RevisionBefore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestDoorLoadTest, "SpnGame.TestDoor.LoadShowsSavedState", SpnDoorTestFlags)

bool FSpnTestDoorLoadTest::RunTest(const FString& Parameters)
{
	const FScopedTestSlot Slot(TEXT("spn_automation_door"));

	{
		FScopedGameWorld Game;
		ASpnTestDoor* DoorA = Game.SpawnDoor(TEXT("door_a"), FVector::ZeroVector);
		Game.SpawnDoor(TEXT("door_b"), FVector(300.f, 0.f, 0.f));
		DoorA->SpnUse(nullptr); // door_a open, door_b untouched
		if (!TestTrue(TEXT("save succeeds"), Game.WorldState().SaveToSlot(Slot.Name).IsOk()))
		{
			return false;
		}

		// Close door_a after saving; loading must show it open again, at once.
		DoorA->SpnUse(nullptr);
		FinishSwing(*DoorA);
		TestEqual(TEXT("door_a closed before the load"), DoorA->GetCurrentAngle(), 0.f);
		TestTrue(TEXT("load succeeds"), Game.WorldState().LoadFromSlot(Slot.Name).IsOk());
		TestTrue(TEXT("door_a reads open after the load"), DoorA->IsOpen());
		TestEqual(TEXT("door_a shown open without a swing"), DoorA->GetCurrentAngle(), DoorA->OpenAngle);
		TestFalse(TEXT("no swing after the load"), DoorA->IsActorTickEnabled());
	}

	{
		// A new session (like restarting PIE) starts closed and shows the saved state after loading.
		FScopedGameWorld Game;
		ASpnTestDoor* DoorA = Game.SpawnDoor(TEXT("door_a"), FVector::ZeroVector);
		ASpnTestDoor* DoorB = Game.SpawnDoor(TEXT("door_b"), FVector(300.f, 0.f, 0.f));
		TestFalse(TEXT("new session: door_a closed"), DoorA->IsOpen());

		TestTrue(TEXT("new session: load succeeds"), Game.WorldState().LoadFromSlot(Slot.Name).IsOk());
		TestEqual(TEXT("new session: door_a shown open"), DoorA->GetCurrentAngle(), DoorA->OpenAngle);
		TestEqual(TEXT("new session: door_b still closed"), DoorB->GetCurrentAngle(), 0.f);

		// A failed load leaves both the state and the doors as they are.
		const FScopedTestSlot Missing(TEXT("spn_automation_door_missing"));
		AddExpectedMessagePlain(TEXT("NOT loaded: no save in slot"), ELogVerbosity::Warning, EAutomationExpectedMessageFlags::Contains, 1);
		TestEqual(TEXT("missing slot"), Game.WorldState().LoadFromSlot(Missing.Name).Result, ESpnSlotResult::SlotNotFound);
		TestEqual(TEXT("door_a still shown open"), DoorA->GetCurrentAngle(), DoorA->OpenAngle);

		// A door spawned after the load reads the loaded state in BeginPlay (and warns about the shared id).
		AddExpectedMessagePlain(TEXT("share StableId 'door_a'"), ELogVerbosity::Warning, EAutomationExpectedMessageFlags::Contains, 1);
		ASpnTestDoor* LateDoorA = Game.SpawnDoor(TEXT("door_a"), FVector(0.f, 600.f, 0.f));
		TestEqual(TEXT("late door shows the loaded state"), LateDoorA->GetCurrentAngle(), LateDoorA->OpenAngle);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInteractionTraceTest, "SpnGame.Interaction.TraceAndUse", SpnDoorTestFlags)

bool FSpnInteractionTraceTest::RunTest(const FString& Parameters)
{
	FScopedGameWorld Game;
	// Closed panel: x 0..100, y -5..5, z 0..200 around the actor origin.
	ASpnTestDoor* Door = Game.SpawnDoor(TEXT("door_a"), FVector::ZeroVector);

	const FVector Eye(50.f, -150.f, 150.f);
	USpnInteractionComponent* Interaction = Game.SpawnInteractor(Eye, FRotator(0.f, 90.f, 0.f)); // facing +Y, at the door

	TestTrue(TEXT("door in front is found"), Interaction->FindTargetAlong(Eye, FVector(0.f, 1.f, 0.f)) == Door);
	TestNull(TEXT("looking away finds nothing"), Interaction->FindTargetAlong(Eye, FVector(0.f, -1.f, 0.f)));
	TestNull(TEXT("out of range finds nothing"),
		Interaction->FindTargetAlong(FVector(50.f, -150.f - Interaction->InteractRange, 150.f), FVector(0.f, 1.f, 0.f)));

	TestTrue(TEXT("owner's view finds the door"), Interaction->FindTarget() == Door);
	TestTrue(TEXT("TryInteract uses the door"), Interaction->TryInteract());
	TestTrue(TEXT("door opened through the component"), Door->IsOpen());
	return true;
}

#endif
