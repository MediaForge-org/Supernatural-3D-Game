#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/SpnTestPickup.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "SpnWorldStateSubsystem.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"
#include "EngineUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// Plays the interactions of /Game/SpnTest/Lvl_SpnDoorTest like a player (E through Enhanced Input, console commands
// through the player controller): out-of-range E, repeated E, both pickups (test_item = 3), doors, the locked door,
// Spn.Inventory, Spn.SaveWorld and Spn.LoadWorld. In the editor it runs in PIE, with -game in the game world, and in a
// packaged Development build via -ExecCmds="Automation RunTests SpnGame.TestMap.PlayPickups;Quit".
// With -SpnKeepSlot the save slot 'spn_m1_audit' is left on disk (for a second process that loads it); otherwise a
// private slot is used and deleted.

namespace
{
	const TCHAR* const TestMapPackage = TEXT("/Game/SpnTest/Lvl_SpnDoorTest");
	const FVector PickupAway(-1.f, 0.f, 0.f); // stand on the -X side of a pickup

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

	struct FPlay
	{
		ASupernaturalProtCharacter* Character = nullptr;
		APlayerController* Controller = nullptr;
		USpnWorldStateSubsystem* WorldState = nullptr;
		TMap<FString, ASpnTestPickup*> Pickups;
		TMap<FString, ASpnTestDoor*> Doors;

		bool IsReady() const { return Character != nullptr && WorldState != nullptr && Pickups.Num() == 2 && Doors.Num() == 3; }

		int64 Count() const { return WorldState->ReadInventoryCount(USpnWorldStateSubsystem::DefaultHolder(), TEXT("test_item")).Get(-1); }

		/** Stands Distance cm in front of the actor (along -X for pickups, against the panel normal for doors), looks at its centre and presses E. */
		void Press(const AActor& Target, const FVector& Away, float Distance) const
		{
			const FVector Centre = Target.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter();
			const FVector Stand = FVector(Centre.X, Centre.Y, Character->GetActorLocation().Z) + Away * Distance;
			Character->GetCharacterMovement()->StopMovementImmediately();
			Character->SetActorLocation(Stand, false, nullptr, ETeleportType::TeleportPhysics);
			Controller->SetControlRotation((Centre - Character->GetPawnViewLocation()).Rotation());
			UEnhancedInputLocalPlayerSubsystem* Input = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
			Input->InjectInputForAction(Character->GetBoundInteractAction(), FInputActionValue(true), {}, {});
		}

		void Console(const FString& Command) const { Controller->ConsoleCommand(Command, /*bWriteToLog*/ true); }
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestMapM1PlayPickupsTest, "SpnGame.TestMap.PlayPickups",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnTestMapM1PlayPickupsTest::RunTest(const FString& Parameters)
{
	const bool bKeep = FParse::Param(FCommandLine::Get(), TEXT("SpnKeepSlot"));
	const FString Slot = bKeep ? TEXT("spn_m1_audit") : TEXT("spn_automation_play_pickups");
	const auto DeleteSlot = [Slot, bKeep] { if (!bKeep) { IFileManager::Get().DeleteDirectory(*FPaths::GetPath(USpnWorldStateSubsystem::GetSlotFilePath(Slot)), false, true); } };
	DeleteSlot();

	AutomationOpenMap(TestMapPackage);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	const TSharedRef<FPlay> Play = MakeShared<FPlay>();

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Play]
	{
		UWorld* World = FindPlayWorld();
		if (!TestNotNull(TEXT("play world"), World))
		{
			return true;
		}
		Play->Controller = World->GetFirstPlayerController();
		Play->Character = Play->Controller != nullptr ? Cast<ASupernaturalProtCharacter>(Play->Controller->GetPawn()) : nullptr;
		Play->WorldState = World->GetGameInstance() != nullptr ? World->GetGameInstance()->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
		for (TActorIterator<ASpnTestPickup> It(World); It; ++It)
		{
			Play->Pickups.Add(It->StableId, *It);
		}
		for (TActorIterator<ASpnTestDoor> It(World); It; ++It)
		{
			Play->Doors.Add(It->StableId, *It);
		}
		TestTrue(TEXT("character, world state, 2 pickups and 3 doors found"), Play->IsReady());
		if (Play->Controller != nullptr)
		{
			TestTrue(TEXT("blank state written"), SpnTestPlay::ResetWorldState(*Play->Controller));
		}
		return true;
	}));

	const auto Step = [Play](TFunction<void()> Body)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Play, Body]
		{
			if (Play->IsReady())
			{
				Body();
			}
			return true;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	};

	Step([this, Play]
	{
		TestEqual(TEXT("fresh state: no test_item"), Play->Count(), int64{0});
		TestFalse(TEXT("fresh state: pickup_a present"), Play->Pickups[TEXT("pickup_a")]->IsTaken());
		TestFalse(TEXT("fresh state: pickup_b present"), Play->Pickups[TEXT("pickup_b")]->IsTaken());
		TestFalse(TEXT("fresh state: door_a closed"), Play->Doors[TEXT("door_a")]->IsOpen());
		TestFalse(TEXT("fresh state: door_b closed"), Play->Doors[TEXT("door_b")]->IsOpen());
		Play->Press(*Play->Pickups[TEXT("pickup_a")], PickupAway, 450.f); // out of reach (range 250)
	});
	Step([this, Play]
	{
		TestFalse(TEXT("out of range: pickup_a is not taken"), Play->Pickups[TEXT("pickup_a")]->IsTaken());
		TestEqual(TEXT("out of range: inventory unchanged"), Play->Count(), int64{0});
		Play->Press(*Play->Pickups[TEXT("pickup_a")], PickupAway, 150.f);
	});
	Step([this, Play]
	{
		TestTrue(TEXT("pickup_a taken"), Play->Pickups[TEXT("pickup_a")]->IsTaken());
		TestEqual(TEXT("pickup_a gives 1"), Play->Count(), int64{1});
		// Pressing E repeatedly at the same spot must not give anything again.
		for (int32 Repeat = 0; Repeat < 3; ++Repeat)
		{
			Play->Press(*Play->Pickups[TEXT("pickup_a")], PickupAway, 150.f);
		}
	});
	Step([this, Play]
	{
		TestEqual(TEXT("repeated E: still 1"), Play->Count(), int64{1});
		Play->Press(*Play->Pickups[TEXT("pickup_b")], PickupAway, 150.f);
	});
	Step([this, Play]
	{
		TestTrue(TEXT("pickup_b taken"), Play->Pickups[TEXT("pickup_b")]->IsTaken());
		TestEqual(TEXT("pickup_b gives 2: test_item = 3"), Play->Count(), int64{3});
		Play->Press(*Play->Pickups[TEXT("pickup_b")], PickupAway, 150.f);
	});
	Step([this, Play]
	{
		TestEqual(TEXT("E on a taken pickup: still 3"), Play->Count(), int64{3});
		// Doors: open door_a and door_b; the locked door must refuse.
		Play->Press(*Play->Doors[TEXT("door_a")], -Play->Doors[TEXT("door_a")]->GetActorRightVector(), 150.f);
	});
	Step([Play]
	{
		Play->Press(*Play->Doors[TEXT("door_b")], -Play->Doors[TEXT("door_b")]->GetActorRightVector(), 150.f);
	});
	Step([Play]
	{
		Play->Press(*Play->Doors[TEXT("door_locked")], -Play->Doors[TEXT("door_locked")]->GetActorRightVector(), 150.f);
	});
	Step([this, Play, Slot]
	{
		TestTrue(TEXT("door_a open"), Play->Doors[TEXT("door_a")]->IsOpen());
		TestTrue(TEXT("door_b open"), Play->Doors[TEXT("door_b")]->IsOpen());
		TestFalse(TEXT("locked door stays closed"), Play->Doors[TEXT("door_locked")]->IsOpen());
		Play->Console(TEXT("Spn.Inventory"));
		Play->Console(FString::Printf(TEXT("Spn.SaveWorld %s"), *Slot));
		// Change the live state after saving: close door_a (E on the swung-open panel at its current place).
		Play->Press(*Play->Doors[TEXT("door_a")], -Play->Doors[TEXT("door_a")]->GetActorRightVector(), 150.f);
	});
	Step([this, Play, Slot]
	{
		TestFalse(TEXT("door_a closed again after saving"), Play->Doors[TEXT("door_a")]->IsOpen());
		Play->Console(FString::Printf(TEXT("Spn.LoadWorld %s"), *Slot));
		TestTrue(TEXT("load: door_a open again"), Play->Doors[TEXT("door_a")]->IsOpen());
		TestTrue(TEXT("load: door_b open"), Play->Doors[TEXT("door_b")]->IsOpen());
		TestFalse(TEXT("load: locked door closed"), Play->Doors[TEXT("door_locked")]->IsOpen());
		TestTrue(TEXT("load: pickup_a still taken"), Play->Pickups[TEXT("pickup_a")]->IsTaken() && Play->Pickups[TEXT("pickup_a")]->IsHidden());
		TestTrue(TEXT("load: pickup_b still taken"), Play->Pickups[TEXT("pickup_b")]->IsTaken() && Play->Pickups[TEXT("pickup_b")]->IsHidden());
		TestEqual(TEXT("load: test_item = 3"), Play->Count(), int64{3});
		Play->Console(TEXT("Spn.Inventory"));
	});
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([DeleteSlot] { DeleteSlot(); return true; }));
	return true;
}

#endif
