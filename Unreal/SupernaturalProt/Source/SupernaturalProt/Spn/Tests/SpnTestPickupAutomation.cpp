#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/Level.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Spn/SpnTestPickup.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for ASpnTestPickup and the inventory bridge (Docs/Proposals/0007).
// Each test runs in its own game instance with a dummy game world; no map asset is needed.

namespace
{
	constexpr EAutomationTestFlags SpnPickupTestFlags =
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

		/** Spawns a pickup, sets it up like a placed one and starts its play. */
		ASpnTestPickup* SpawnPickup(const TCHAR* StableId, const TCHAR* ItemId, int32 Amount, const FVector& Location)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestPickup* Pickup = World->SpawnActor<ASpnTestPickup>(ASpnTestPickup::StaticClass(), FTransform(Location), Params);
			Pickup->StableId = StableId;
			Pickup->ItemId = ItemId;
			Pickup->Amount = Amount;
			Pickup->FinishSpawning(FTransform(Location));
			Pickup->DispatchBeginPlay();
			Spawned.Add(Pickup);
			return Pickup;
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

	int64 CountOf(const USpnWorldStateSubsystem& WorldState, const TCHAR* Item)
	{
		return WorldState.ReadInventoryCount(USpnWorldStateSubsystem::DefaultHolder(), Item).Get(-1);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestPickupTakeTest, "SpnGame.TestPickup.TakeOnce", SpnPickupTestFlags)

bool FSpnTestPickupTakeTest::RunTest(const FString& Parameters)
{
	FScopedGameWorld Game;
	ASpnTestPickup* A = Game.SpawnPickup(TEXT("crate_a"), TEXT("test_item"), 3, FVector::ZeroVector);
	ASpnTestPickup* B = Game.SpawnPickup(TEXT("crate_b"), TEXT("test_item"), 2, FVector(200.f, 0.f, 0.f));
	ASpnTestPickup* C = Game.SpawnPickup(TEXT("crate_c"), TEXT("other_item"), 1, FVector(400.f, 0.f, 0.f));

	TestFalse(TEXT("present at start"), A->IsTaken());
	TestFalse(TEXT("visible at start"), A->IsHidden());
	TestTrue(TEXT("solid at start"), A->GetActorEnableCollision());
	TestEqual(TEXT("empty inventory"), CountOf(Game.WorldState(), TEXT("test_item")), int64{0});

	TestEqual(TEXT("Use takes it"), A->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestTrue(TEXT("taken"), A->IsTaken());
	TestTrue(TEXT("hidden after taking"), A->IsHidden());
	TestFalse(TEXT("no collision after taking"), A->GetActorEnableCollision());
	TestEqual(TEXT("count is the amount"), CountOf(Game.WorldState(), TEXT("test_item")), int64{3});
	TestEqual(TEXT("two journaled changes (taken flag and count)"), Game.WorldState().GetRevision(), uint64{2});

	// Taking twice: refused, nothing written.
	const FSpnInteractStatus Again = A->SpnUse(nullptr);
	TestEqual(TEXT("second Use is refused"), Again.Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("refusal feedback"), Again.Feedback, FString(TEXT("placeholder:feedback.pickup.gone")));
	TestEqual(TEXT("refusal writes nothing"), Game.WorldState().GetRevision(), uint64{2});
	TestEqual(TEXT("count unchanged"), CountOf(Game.WorldState(), TEXT("test_item")), int64{3});

	// Same item adds up, other items stay apart; other instances are unaffected.
	TestFalse(TEXT("other instance still present"), B->IsTaken());
	TestEqual(TEXT("second pickup"), B->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestEqual(TEXT("counts add up"), CountOf(Game.WorldState(), TEXT("test_item")), int64{5});
	TestEqual(TEXT("third pickup, other item"), C->SpnUse(nullptr).Result, ESpnInteractResult::Changed);

	const TArray<FSpnInventoryEntry> List = Game.WorldState().ListInventory(USpnWorldStateSubsystem::DefaultHolder());
	if (TestEqual(TEXT("two items listed"), List.Num(), 2))
	{
		TestEqual(TEXT("ordered by item id: other_item first"), List[0].Item, FString(TEXT("other_item")));
		TestEqual(TEXT("other_item count"), List[0].Count, int64{1});
		TestEqual(TEXT("test_item second"), List[1].Item, FString(TEXT("test_item")));
		TestEqual(TEXT("test_item count"), List[1].Count, int64{5});
	}

	// Invalid ids: Use fails without writing.
	AddExpectedMessagePlain(TEXT("breaks the item rule"), ELogVerbosity::Error, EAutomationExpectedMessageFlags::Contains, 1);
	AddExpectedMessagePlain(TEXT("Use failed: InvalidId"), ELogVerbosity::Error, EAutomationExpectedMessageFlags::Contains, 1);
	const uint64 RevisionBefore = Game.WorldState().GetRevision();
	ASpnTestPickup* BadItem = Game.SpawnPickup(TEXT("crate_d"), TEXT("Bad.Item"), 1, FVector(600.f, 0.f, 0.f));
	TestEqual(TEXT("invalid item is reported"), BadItem->SpnUse(nullptr).Result, ESpnInteractResult::InvalidId);
	TestEqual(TEXT("invalid item writes nothing"), Game.WorldState().GetRevision(), RevisionBefore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestPickupHolderTest, "SpnGame.TestPickup.HoldersAreSeparate", SpnPickupTestFlags)

bool FSpnTestPickupHolderTest::RunTest(const FString& Parameters)
{
	FScopedGameWorld Game;
	USpnWorldStateSubsystem& WorldState = Game.WorldState();

	FSpnInteractDefaults Defaults;
	Defaults.Strings.Add(TEXT("item"), TEXT("test_item"));
	Defaults.Ints.Add(TEXT("amount"), 4);

	const FString Kind = USpnWorldStateSubsystem::TestPickupKind();
	TestEqual(TEXT("coop holder takes crate_a"),
		WorldState.TryInteract(Kind, TEXT("crate_a"), ESpnInteractionVerb::Use, Defaults, TEXT("coop.p2"), TEXT("test")).Result,
		ESpnInteractResult::Changed);
	TestEqual(TEXT("the local player cannot take what is gone"),
		WorldState.TryInteract(Kind, TEXT("crate_a"), ESpnInteractionVerb::Use, Defaults, USpnWorldStateSubsystem::DefaultHolder(), TEXT("test")).Result,
		ESpnInteractResult::Refused);
	TestEqual(TEXT("coop count"), WorldState.ReadInventoryCount(TEXT("coop.p2"), TEXT("test_item")).Get(-1), int64{4});
	TestEqual(TEXT("local count stays 0"), CountOf(WorldState, TEXT("test_item")), int64{0});

	const uint64 Revision = WorldState.GetRevision();
	TestEqual(TEXT("malformed holder is rejected"),
		WorldState.TryInteract(Kind, TEXT("crate_b"), ESpnInteractionVerb::Use, Defaults, TEXT("Bad Holder"), TEXT("test")).Result,
		ESpnInteractResult::InvalidId);
	TestEqual(TEXT("malformed holder writes nothing"), WorldState.GetRevision(), Revision);
	TestFalse(TEXT("unknown count of a malformed id is unset"), WorldState.ReadInventoryCount(TEXT("Bad Holder"), TEXT("test_item")).IsSet());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestPickupLoadTest, "SpnGame.TestPickup.SaveLoadRestoresInventory", SpnPickupTestFlags)

bool FSpnTestPickupLoadTest::RunTest(const FString& Parameters)
{
	const FScopedTestSlot Slot(TEXT("spn_automation_pickup"));

	{
		FScopedGameWorld Game;
		ASpnTestPickup* A = Game.SpawnPickup(TEXT("crate_a"), TEXT("test_item"), 3, FVector::ZeroVector);
		ASpnTestPickup* B = Game.SpawnPickup(TEXT("crate_b"), TEXT("test_item"), 2, FVector(200.f, 0.f, 0.f));
		A->SpnUse(nullptr);
		if (!TestTrue(TEXT("save succeeds"), Game.WorldState().SaveToSlot(Slot.Name).IsOk()))
		{
			return false;
		}

		// Take the second one after saving; loading must bring it back and restore the count.
		B->SpnUse(nullptr);
		TestEqual(TEXT("count before the load"), CountOf(Game.WorldState(), TEXT("test_item")), int64{5});
		TestTrue(TEXT("load succeeds"), Game.WorldState().LoadFromSlot(Slot.Name).IsOk());
		TestTrue(TEXT("first pickup stays taken"), A->IsTaken());
		TestTrue(TEXT("first pickup stays hidden"), A->IsHidden());
		TestFalse(TEXT("second pickup is back"), B->IsTaken());
		TestFalse(TEXT("second pickup is visible again"), B->IsHidden());
		TestTrue(TEXT("second pickup is solid again"), B->GetActorEnableCollision());
		TestEqual(TEXT("count restored"), CountOf(Game.WorldState(), TEXT("test_item")), int64{3});
	}

	{
		// A new session (like restarting PIE) starts empty and shows the saved state after loading.
		FScopedGameWorld Game;
		ASpnTestPickup* A = Game.SpawnPickup(TEXT("crate_a"), TEXT("test_item"), 3, FVector::ZeroVector);
		ASpnTestPickup* B = Game.SpawnPickup(TEXT("crate_b"), TEXT("test_item"), 2, FVector(200.f, 0.f, 0.f));
		TestFalse(TEXT("new session: crate_a present"), A->IsTaken());
		TestEqual(TEXT("new session: empty inventory"), CountOf(Game.WorldState(), TEXT("test_item")), int64{0});

		TestTrue(TEXT("new session: load succeeds"), Game.WorldState().LoadFromSlot(Slot.Name).IsOk());
		TestTrue(TEXT("new session: crate_a taken"), A->IsTaken());
		TestTrue(TEXT("new session: crate_a hidden"), A->IsHidden());
		TestFalse(TEXT("new session: crate_b still present"), B->IsTaken());
		TestEqual(TEXT("new session: count restored"), CountOf(Game.WorldState(), TEXT("test_item")), int64{3});
		TestEqual(TEXT("taken pickup stays refused"), A->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
		TestEqual(TEXT("untouched pickup can still be taken"), B->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
		TestEqual(TEXT("counts add up after the load"), CountOf(Game.WorldState(), TEXT("test_item")), int64{5});
	}
	return true;
}

#if WITH_EDITOR
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestPickupMapTest, "SpnGame.TestMap.Pickups",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

// The generated map /Game/SpnTest/Lvl_SpnDoorTest gets two pickups from create_spn_door_test_map.py
// (pickup_a, pickup_b). A map generated before Phase 0.5 has none: that is reported as a warning
// ("regenerate with --force"), not as a failure, because only the Windows editor can rebuild the .umap.
bool FSpnTestPickupMapTest::RunTest(const FString& Parameters)
{
	const UWorld* Map = LoadObject<UWorld>(nullptr, TEXT("/Game/SpnTest/Lvl_SpnDoorTest.Lvl_SpnDoorTest"));
	if (!TestNotNull(TEXT("test map exists"), Map) || !TestNotNull(TEXT("test map has a level"), Map->PersistentLevel.Get()))
	{
		return false;
	}

	TArray<const ASpnTestPickup*> Pickups;
	for (const AActor* Actor : Map->PersistentLevel->Actors)
	{
		if (const ASpnTestPickup* Pickup = Cast<ASpnTestPickup>(Actor))
		{
			Pickups.Add(Pickup);
		}
	}
	if (Pickups.IsEmpty())
	{
		AddWarning(TEXT("The test map has no pickups yet: regenerate it on Windows with create_spn_door_test_map.py --force (see Docs/Decisions/0006)."));
		return true;
	}

	TestEqual(TEXT("exactly two test pickups"), Pickups.Num(), 2);
	TSet<FString> Ids;
	for (const ASpnTestPickup* Pickup : Pickups)
	{
		Ids.Add(Pickup->StableId);
		TestTrue(FString::Printf(TEXT("%s has a valid stable id"), *Pickup->StableId), USpnWorldStateSubsystem::IsValidInteractableId(Pickup->StableId));
		TestTrue(FString::Printf(TEXT("%s has a valid item id"), *Pickup->StableId), USpnWorldStateSubsystem::IsValidItemId(Pickup->ItemId));
		TestTrue(FString::Printf(TEXT("%s amount is at least 1"), *Pickup->StableId), Pickup->Amount >= 1);
	}
	TestEqual(TEXT("stable ids are unique"), Ids.Num(), Pickups.Num());
	return true;
}
#endif // WITH_EDITOR

#endif
