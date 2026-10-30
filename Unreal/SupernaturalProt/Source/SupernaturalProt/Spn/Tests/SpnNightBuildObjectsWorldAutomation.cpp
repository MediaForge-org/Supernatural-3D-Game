#include "EngineUtils.h"
#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/SpnWorldObjects.h"
#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// Night Build 01 completion: persistent world objects through a REAL level recreation. Pickups and the container are taken / changed with the
// objects' own interaction entry point (SpnUse) and the Core transfer, the game saves, returns to the main menu (the level and every actor are
// destroyed), loads again (the level is rebuilt and the actors are created anew) and the recreated actors must show the saved state. Failed
// transactions (a bulky item that does not fit, a duplicate identity) must change nothing.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags ObjFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const ObjGameplayMap = TEXT("/Game/SpnTest/Lvl_SpnMovementTestV3Interiors");

	struct FObjState
	{
		FString Root;
		TWeakObjectPtr<UGameInstance> Instance;
		int32 Waited = 0;
	};

	template <typename T>
	T* FindById(UWorld* World, const TCHAR* Id)
	{
		for (TActorIterator<T> It(World); It; ++It)
		{
			if (It->StableId == Id && !It->HasIdentityConflict())
			{
				return *It;
			}
		}
		return nullptr;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbObjectsWorldTest, "SpnGame.NightBuild.ObjectsWorld", ObjFlags)
bool FSpnNbObjectsWorldTest::RunTest(const FString&)
{
	if (!FPackageName::DoesPackageExist(ObjGameplayMap))
	{
		AddError(TEXT("Lvl_SpnMovementTestV3Interiors is missing (generated, not committed)"));
		return false;
	}
	const TSharedRef<FObjState> S = MakeShared<FObjState>();
	S->Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
	AutomationOpenMap(ObjGameplayMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		UWorld* World = FindPlayWorld();
		if (World == nullptr || FindCharacter() == nullptr)
		{
			if (++S->Waited > 1800) { AddError(TEXT("no character")); S->Waited = 0; return true; }
			return false;
		}
		S->Waited = 0;
		S->Instance = World->GetGameInstance();
		S->Instance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(S->Root);
		S->Instance->GetSubsystem<USpnSessionSubsystem>()->StartNewGameInPlace(World);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));

	// play: failed pickup (does not fit), successful pickup, container transfer, failed transfer, duplicate identity
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		UWorld* World = FindPlayWorld();
		ASupernaturalProtCharacter* Character = FindCharacter();
		USpnInventorySubsystem* Inv = S->Instance->GetSubsystem<USpnInventorySubsystem>();
		const uint64 Player = SpnWorld::FindPlayerContainer(*Inv);
		ASpnWorldPickup* Ration = FindById<ASpnWorldPickup>(World, TEXT("nb01.pickup.ration_yard"));
		ASpnWorldPickup* Crate = FindById<ASpnWorldPickup>(World, TEXT("nb01.pickup.crate_hall"));
		ASpnWorldContainer* Cupboard = FindById<ASpnWorldContainer>(World, TEXT("nb01.container.cupboard"));
		if (!TestNotNull(TEXT("scenario ration"), Ration) || !TestNotNull(TEXT("scenario crate"), Crate) || !TestNotNull(TEXT("scenario cupboard"), Cupboard))
		{
			return true;
		}
		TestEqual(TEXT("nothing carried at the start"), Inv->CountOf(Player, TEXT("spn_test:item.ration")), 0ull);

		const uint64 Revision = Inv->GetRevision();
		TestEqual(TEXT("the bulky crate is refused"), Crate->SpnUse(Character).Result, ESpnInteractResult::Refused);
		TestFalse(TEXT("and stays in the world"), Crate->IsTaken() || Crate->IsHidden());
		TestEqual(TEXT("no inventory change"), Inv->GetRevision(), Revision);

		TestEqual(TEXT("the ration is taken"), Ration->SpnUse(Character).Result, ESpnInteractResult::Changed);
		TestEqual(TEXT("really in the Core inventory"), Inv->CountOf(Player, TEXT("spn_test:item.ration")), 2ull);
		TestTrue(TEXT("hidden and flagged"), Ration->IsTaken() && Ration->IsHidden());
		TestEqual(TEXT("a second use does not duplicate"), Ration->SpnUse(Character).Result, ESpnInteractResult::Refused);
		TestEqual(TEXT("still two"), Inv->CountOf(Player, TEXT("spn_test:item.ration")), 2ull);

		FSpnContainerView View;
		TestTrue(TEXT("cupboard container exists"), Inv->GetContainer(Cupboard->GetContainerId(), View));
		uint64 BandageStack = 0;
		for (const FSpnStackView& Stack : View.Stacks)
		{
			if (Stack.Definition == TEXT("spn_test:item.bandage")) { BandageStack = Stack.StackId; }
		}
		TestTrue(TEXT("seeded with bandages"), BandageStack != 0);
		TestTrue(TEXT("transfer ok"), Inv->TransferItem(Cupboard->GetContainerId(), Player, BandageStack, 1).IsOk());
		TestEqual(TEXT("player 1 bandage"), Inv->CountOf(Player, TEXT("spn_test:item.bandage")), 1ull);
		TestEqual(TEXT("cupboard 2 bandages"), Inv->CountOf(Cupboard->GetContainerId(), TEXT("spn_test:item.bandage")), 2ull);
		const uint64 Revision2 = Inv->GetRevision();
		TestFalse(TEXT("over-asking is refused"), Inv->TransferItem(Cupboard->GetContainerId(), Player, BandageStack, 99).IsOk());
		TestEqual(TEXT("and nothing moved"), Inv->GetRevision(), Revision2);

		AddExpectedError(TEXT("another pickup already uses the StableId"), EAutomationExpectedErrorFlags::Contains, 1);
		AddExpectedError(TEXT("another container already uses the StableId"), EAutomationExpectedErrorFlags::Contains, 1);
		ASpnWorldPickup* Twin = World->SpawnActorDeferred<ASpnWorldPickup>(ASpnWorldPickup::StaticClass(), FTransform(FVector(0, 0, 500)));
		Twin->StableId = TEXT("nb01.pickup.flashlight_room2");
		Twin->DefinitionId = TEXT("spn_test:item.flashlight");
		Twin->FinishSpawning(FTransform(FVector(0, 0, 500)));
		TestTrue(TEXT("the twin pickup lost the identity race"), Twin->HasIdentityConflict());
		TestEqual(TEXT("and cannot be used"), Twin->SpnUse(Character).Result, ESpnInteractResult::Failed);
		TestEqual(TEXT("no flashlight appeared"), Inv->CountOf(Player, TEXT("spn_test:item.flashlight")), 0ull);
		Twin->Destroy();
		ASpnWorldContainer* Twin2 = World->SpawnActorDeferred<ASpnWorldContainer>(ASpnWorldContainer::StaticClass(), FTransform(FVector(0, 0, 500)));
		Twin2->StableId = TEXT("nb01.container.cupboard");
		Twin2->FinishSpawning(FTransform(FVector(0, 0, 500)));
		TestTrue(TEXT("the twin container lost the identity race"), Twin2->HasIdentityConflict());
		TestEqual(TEXT("the cupboard keeps its contents"), Inv->CountOf(Cupboard->GetContainerId(), TEXT("spn_test:item.bandage")), 2ull);
		Twin2->Destroy();
		TestEqual(TEXT("inventory invariants"), Inv->CheckInvariants(), FString());

		TestTrue(TEXT("saved"), S->Instance->GetSubsystem<USpnSessionSubsystem>()->SaveGame(TEXT("objects_world")).IsOk());
		return true;
	}));

	// main menu: the level and all actors are gone; then load -> level rebuilt, actors created anew
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([S]() { S->Instance->GetSubsystem<USpnSessionSubsystem>()->ReturnToMainMenu(); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		USpnSessionSubsystem* Sess = S->Instance->GetSubsystem<USpnSessionSubsystem>();
		if (!Sess->IsMenuWorld())
		{
			if (++S->Waited > 1200) { AddError(TEXT("never reached the menu")); S->Waited = 0; return true; }
			return false;
		}
		S->Waited = 0;
		UWorld* Menu = FindPlayWorld();
		int32 Objects = 0;
		for (TActorIterator<ASpnWorldPickup> It(Menu); It; ++It) { ++Objects; }
		for (TActorIterator<ASpnWorldContainer> It(Menu); It; ++It) { ++Objects; }
		TestEqual(TEXT("in the menu level no world object exists"), Objects, 0);
		TestTrue(TEXT("load from the menu"), Sess->LoadGame(TEXT("objects_world")).IsOk());
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		const UWorld* World = FindPlayWorld();
		USpnSessionSubsystem* Sess = S->Instance->GetSubsystem<USpnSessionSubsystem>();
		const bool bReady = World != nullptr && World->GetMapName().Contains(TEXT("V3Interiors")) && FindCharacter() != nullptr && !Sess->GetPendingPose().IsSet();
		if (!bReady)
		{
			if (++S->Waited > 1800) { AddError(TEXT("gameplay level did not come back")); S->Waited = 0; return true; }
			return false;
		}
		S->Waited = 0;
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		UWorld* World = FindPlayWorld();
		ASupernaturalProtCharacter* Character = FindCharacter();
		USpnInventorySubsystem* Inv = S->Instance->GetSubsystem<USpnInventorySubsystem>();
		const uint64 Player = SpnWorld::FindPlayerContainer(*Inv);
		ASpnWorldPickup* Ration = FindById<ASpnWorldPickup>(World, TEXT("nb01.pickup.ration_yard"));
		ASpnWorldPickup* Flashlight = FindById<ASpnWorldPickup>(World, TEXT("nb01.pickup.flashlight_room2"));
		ASpnWorldPickup* Crate = FindById<ASpnWorldPickup>(World, TEXT("nb01.pickup.crate_hall"));
		ASpnWorldContainer* Cupboard = FindById<ASpnWorldContainer>(World, TEXT("nb01.container.cupboard"));
		if (!TestNotNull(TEXT("recreated ration"), Ration) || !TestNotNull(TEXT("recreated flashlight"), Flashlight) || !TestNotNull(TEXT("recreated crate"), Crate)
			|| !TestNotNull(TEXT("recreated cupboard"), Cupboard))
		{
			return true;
		}
		TestTrue(TEXT("the recreated ration pickup is already taken (loading does not respawn it)"), Ration->IsTaken() && Ration->IsHidden());
		TestEqual(TEXT("it cannot be taken again"), Ration->SpnUse(Character).Result, ESpnInteractResult::Refused);
		TestEqual(TEXT("rations: exactly the two that were taken"), Inv->CountOf(Player, TEXT("spn_test:item.ration")), 2ull);
		TestFalse(TEXT("the untouched flashlight is still in the world"), Flashlight->IsTaken() || Flashlight->IsHidden());
		TestFalse(TEXT("the crate that did not fit is still in the world"), Crate->IsTaken() || Crate->IsHidden());
		TestEqual(TEXT("player bandages"), Inv->CountOf(Player, TEXT("spn_test:item.bandage")), 1ull);
		TestEqual(TEXT("cupboard bandages (stable container identity)"), Inv->CountOf(Cupboard->GetContainerId(), TEXT("spn_test:item.bandage")), 2ull);
		TestEqual(TEXT("the notebook is still in the cupboard (no re-seeding)"), Inv->CountOf(Cupboard->GetContainerId(), TEXT("spn_test:item.notebook")), 1ull);
		TestEqual(TEXT("bandages exist exactly three times in total"),
			Inv->CountOf(Player, TEXT("spn_test:item.bandage")) + Inv->CountOf(Cupboard->GetContainerId(), TEXT("spn_test:item.bandage")), 3ull);
		TestEqual(TEXT("inventory invariants"), Inv->CheckInvariants(), FString());
		IFileManager::Get().DeleteDirectory(*S->Root, false, true);
		return true;
	}));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
