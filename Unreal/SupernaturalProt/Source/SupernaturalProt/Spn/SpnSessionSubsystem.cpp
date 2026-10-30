#include "Spn/SpnSessionSubsystem.h"

#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnMapSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "Spn/SpnWorldObjects.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
#include "SupernaturalProt.h"
#include "UObject/UObjectGlobals.h"

namespace
{
	static FAutoConsoleCommandWithWorldAndArgs GEnableFlow(TEXT("Spn.Game.Enable"), TEXT("Enable the Night Build 01 game flow (menus, HUD, session) in the current game."),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>&, UWorld* World)
		{
			if (World != nullptr && World->GetGameInstance() != nullptr)
			{
				if (USpnSessionSubsystem* Session = World->GetGameInstance()->GetSubsystem<USpnSessionSubsystem>())
				{
					Session->EnableGameFlow();
					Session->SetupGameplayWorld(World);
					UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Game.Enable: game flow enabled"));
				}
			}
		}));

	FSpnSaveStatus MakeStatus(ESpnSaveResult Result, const FString& Detail)
	{
		FSpnSaveStatus Status;
		Status.Result = Result;
		Status.Detail = Detail;
		return Status;
	}
}

FString USpnSessionSubsystem::GameplayMapName()
{
	FString Override;
	if (FParse::Value(FCommandLine::Get(), TEXT("SpnGameplayMap="), Override) && !Override.IsEmpty())
	{
		return Override;
	}
	return TEXT("/Game/SpnTest/Lvl_SpnMovementTestV3Interiors");
}

void USpnSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<USpnWorldStateSubsystem>();
	Collection.InitializeDependency<USpnInventorySubsystem>();
	Collection.InitializeDependency<USpnMapSubsystem>();
	Collection.InitializeDependency<USpnSaveSubsystem>();
	Super::Initialize(Collection);
	bGameFlow = FParse::Param(FCommandLine::Get(), TEXT("SpnGameFlow"));
	FString SaveDir;
	if (FParse::Value(FCommandLine::Get(), TEXT("SpnSaveDir="), SaveDir) && !SaveDir.IsEmpty())
	{
		// QA runs keep their saves in their own folder (never the player's real slots)
		if (USpnSaveSubsystem* Save = Collection.InitializeDependency<USpnSaveSubsystem>())
		{
			const FSpnSaveStatus Status = Save->SetSaveDirectory(FPaths::ConvertRelativePathToFull(SaveDir));
			UE_LOG(LogSupernaturalProt, Display, TEXT("Session: save root set by -SpnSaveDir: %s (%s)"), *SaveDir, LexToString(Status.Result));
		}
	}
	PostLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USpnSessionSubsystem::HandlePostLoadMap);
}

void USpnSessionSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLoadHandle);
	Super::Deinitialize();
}

void USpnSessionSubsystem::HandlePostLoadMap(UWorld* World)
{
	if (World == nullptr || World->GetGameInstance() != GetGameInstance() || !World->IsGameWorld())
	{
		return;
	}
	bMenuWorld = World->GetMapName().Contains(TEXT("SpnMainMenu"));
	if (bMenuWorld)
	{
		bGameFlow = true; // loading the menu level is the opt-in
	}
	if (!bGameFlow)
	{
		return;
	}
	if (bMenuWorld)
	{
		// slots are judged against the catalog and the map definition the game will use, so they must be installed before the menu lists slots
		EnsureCatalog();
		EnsureMap();
	}
	else
	{
		SetupGameplayWorld(World);
	}
	OnWorldReady.Broadcast(bMenuWorld);
}

bool USpnSessionSubsystem::EnsureCatalog()
{
	USpnInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<USpnInventorySubsystem>();
	if (Inventory == nullptr)
	{
		return false;
	}
	if (Inventory->HasCatalog())
	{
		return true;
	}
	const FSpnInventoryStatus Status = Inventory->LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath());
	if (!Status.IsOk())
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("Session: the temporary test item catalog could not be loaded: %s (%s)"), LexToString(Status.Error), *Status.Detail);
		return false;
	}
	UE_LOG(LogSupernaturalProt, Display, TEXT("Session: loaded the TEMPORARY test item catalog"));
	return true;
}

void USpnSessionSubsystem::EnsureMap()
{
	USpnMapSubsystem* Map = GetGameInstance()->GetSubsystem<USpnMapSubsystem>();
	if (Map != nullptr && !Map->HasDefinition())
	{
		Map->InstallTestDefinition();
	}
}

void USpnSessionSubsystem::ResetForNewGame()
{
	UGameInstance* Instance = GetGameInstance();
	if (USpnWorldStateSubsystem* WorldState = Instance->GetSubsystem<USpnWorldStateSubsystem>())
	{
		WorldState->ResetToEmpty();
	}
	if (USpnInventorySubsystem* Inventory = Instance->GetSubsystem<USpnInventorySubsystem>())
	{
		Inventory->ResetInventory();
	}
	if (USpnMapSubsystem* Map = Instance->GetSubsystem<USpnMapSubsystem>())
	{
		Map->InstallTestDefinition(Map->GetFrame()); // a fresh state on the same definition: no markers, nothing discovered
	}
	ActiveSlot.Reset();
}

void USpnSessionSubsystem::SetupGameplayWorld(UWorld* World)
{
	UGameInstance* Instance = GetGameInstance();
	if (Instance == nullptr || World == nullptr)
	{
		return;
	}
	const bool bCatalog = EnsureCatalog();
	EnsureMap();
	const ESpnSessionPending Action = Pending;
	const FString Slot = PendingSlot;
	const bool bAccept = bPendingAccept;
	Pending = ESpnSessionPending::None;

	if (Action == ESpnSessionPending::NewGame)
	{
		ResetForNewGame();
	}
	USpnInventorySubsystem* Inventory = Instance->GetSubsystem<USpnInventorySubsystem>();
	if (bCatalog && Inventory != nullptr && SpnWorld::FindPlayerContainer(*Inventory) == 0)
	{
		const FSpnInventoryStatus Status = Inventory->CreateContainerFromPreset(SpnWorld::PlayerContainerId, TEXT("spn_test:preset.body"));
		if (!Status.IsOk())
		{
			UE_LOG(LogSupernaturalProt, Error, TEXT("Session: the player container could not be created: %s"), LexToString(Status.Error));
		}
	}
	SpawnScenario(World);

	if (Action == ESpnSessionPending::Load)
	{
		USpnSaveSubsystem* Save = Instance->GetSubsystem<USpnSaveSubsystem>();
		FSpnLoadOptions Options;
		Options.bAcceptCatalogMismatch = bAccept;
		FSpnSaveStatus Status = Save != nullptr ? Save->LoadSlot(Slot, Options) : MakeStatus(ESpnSaveResult::WorldStateUnavailable, TEXT("no save subsystem"));
		if (Status.IsOk())
		{
			ActiveSlot = Slot;
		}
		UE_LOG(LogSupernaturalProt, Display, TEXT("Session: pending load of '%s' -> %s %s"), *Slot, LexToString(Status.Result), *Status.Detail);
		OnPendingLoadFinished.Broadcast(Status);
	}
}

void USpnSessionSubsystem::SpawnScenario(UWorld* World)
{
	// The integration scenario lives in one level only; its objects are PLACEHOLDERS (cubes with a label) at coordinates inside Cottage1 and in
	// front of it (Tools/spn_district_layout.py: Cottage1 origin (-9400, -5800), door d_cottage1_door at x = -9400).
	if (!World->GetMapName().Contains(TEXT("V3Interiors")))
	{
		return;
	}
	struct FPickupSpec { const TCHAR* Id; const TCHAR* Def; int32 Qty; FVector Where; };
	const FPickupSpec Pickups[] = {
		{TEXT("nb01.pickup.ration_yard"), TEXT("spn_test:item.ration"), 2, FVector(-10150.f, -5440.f, 5.f)},
		{TEXT("nb01.pickup.flashlight_room2"), TEXT("spn_test:item.flashlight"), 1, FVector(-8840.f, -5170.f, 5.f)},
		{TEXT("nb01.pickup.crate_hall"), TEXT("spn_test:item.crate"), 1, FVector(-9150.f, -5340.f, 5.f)},
	};
	for (const FPickupSpec& Spec : Pickups)
	{
		bool bExists = false;
		for (TActorIterator<ASpnWorldPickup> It(World); It; ++It)
		{
			bExists |= It->StableId == Spec.Id;
		}
		if (bExists)
		{
			continue;
		}
		if (ASpnWorldPickup* Pickup = World->SpawnActorDeferred<ASpnWorldPickup>(ASpnWorldPickup::StaticClass(), FTransform(FRotator::ZeroRotator, Spec.Where)))
		{
			Pickup->StableId = Spec.Id;
			Pickup->DefinitionId = Spec.Def;
			Pickup->Quantity = Spec.Qty;
			Pickup->FinishSpawning(FTransform(FRotator::ZeroRotator, Spec.Where));
		}
	}
	bool bContainerExists = false;
	for (TActorIterator<ASpnWorldContainer> It(World); It; ++It)
	{
		bContainerExists |= It->StableId == TEXT("nb01.container.cupboard");
	}
	if (!bContainerExists)
	{
		const FTransform Where(FRotator::ZeroRotator, FVector(-8700.f, -5740.f, 5.f));
		if (ASpnWorldContainer* Container = World->SpawnActorDeferred<ASpnWorldContainer>(ASpnWorldContainer::StaticClass(), Where))
		{
			Container->StableId = TEXT("nb01.container.cupboard");
			Container->PresetId = TEXT("spn_test:preset.small_box");
			Container->Title = TEXT("Cupboard");
			Container->InitialItems = {{TEXT("spn_test:item.bandage"), 3}, {TEXT("spn_test:item.notebook"), 1}};
			Container->FinishSpawning(Where);
		}
	}
}

void USpnSessionSubsystem::StartNewGame()
{
	bGameFlow = true;
	Pending = ESpnSessionPending::NewGame;
	PendingSlot.Reset();
	if (UWorld* World = GetGameInstance()->GetWorld())
	{
		UGameplayStatics::OpenLevel(World, FName(*GameplayMapName()));
	}
}

FSpnSaveStatus USpnSessionSubsystem::LoadGame(const FString& Slot, bool bAcceptCatalogMismatch)
{
	bGameFlow = true;
	UGameInstance* Instance = GetGameInstance();
	USpnSaveSubsystem* Save = Instance->GetSubsystem<USpnSaveSubsystem>();
	if (Save == nullptr)
	{
		return MakeStatus(ESpnSaveResult::WorldStateUnavailable, TEXT("no save subsystem"));
	}
	EnsureCatalog(); // a slot is judged against the catalog the game will use
	EnsureMap();
	FSpnLoadOptions Options;
	Options.bAcceptCatalogMismatch = bAcceptCatalogMismatch;
	const USpnSaveSubsystem::FSpnSlotInfo Info = Save->InspectSlot(Slot, Options);
	if (Info.Result != ESpnSaveResult::Ok)
	{
		return MakeStatus(Info.Result, Info.Detail); // nothing was touched
	}
	if (bMenuWorld)
	{
		Pending = ESpnSessionPending::Load;
		PendingSlot = Slot;
		bPendingAccept = bAcceptCatalogMismatch;
		if (UWorld* World = Instance->GetWorld())
		{
			UGameplayStatics::OpenLevel(World, FName(*GameplayMapName()));
		}
		return MakeStatus(ESpnSaveResult::Ok, TEXT("loading"));
	}
	const FSpnSaveStatus Status = Save->LoadSlot(Slot, Options);
	if (Status.IsOk())
	{
		ActiveSlot = Slot;
	}
	return Status;
}

FSpnSaveStatus USpnSessionSubsystem::SaveGame(const FString& Slot)
{
	USpnSaveSubsystem* Save = GetGameInstance()->GetSubsystem<USpnSaveSubsystem>();
	if (Save == nullptr)
	{
		return MakeStatus(ESpnSaveResult::WorldStateUnavailable, TEXT("no save subsystem"));
	}
	const FSpnSaveStatus Status = Save->SaveSlot(Slot);
	if (Status.IsOk())
	{
		ActiveSlot = Slot;
	}
	return Status;
}

void USpnSessionSubsystem::ReturnToMainMenu()
{
	if (UWorld* World = GetGameInstance()->GetWorld())
	{
		UGameplayStatics::SetGamePaused(World, false);
		UGameplayStatics::OpenLevel(World, FName(MainMenuMapName()));
	}
}

void USpnSessionSubsystem::QuitGame()
{
	UWorld* World = GetGameInstance()->GetWorld();
	APlayerController* Controller = World != nullptr ? World->GetFirstPlayerController() : nullptr;
	UKismetSystemLibrary::QuitGame(World, Controller, EQuitPreference::Quit, false);
}

FString USpnSessionSubsystem::FindContinueSlot() const
{
	const USpnSaveSubsystem* Save = GetGameInstance()->GetSubsystem<USpnSaveSubsystem>();
	if (Save == nullptr)
	{
		return FString();
	}
	for (const FString& Slot : Save->ListSlots())
	{
		if (Save->InspectSlot(Slot).Result == ESpnSaveResult::Ok)
		{
			return Slot;
		}
	}
	return FString();
}
