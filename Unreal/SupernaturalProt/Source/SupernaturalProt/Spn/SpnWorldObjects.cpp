#include "Spn/SpnWorldObjects.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "Misc/Crc.h"
#include "SpnInventorySubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

namespace SpnWorld
{
	uint64 ContainerIdFor(const FString& StableId)
	{
		return 0x1'0000'0000ull + static_cast<uint64>(FCrc::StrCrc32(*StableId) & 0x7FFFFFFFu);
	}

	FText ItemDisplayName(const FString& DefinitionId)
	{
		return FSpnUiText::Get(FString::Printf(TEXT("item.%s"), *DefinitionId), {}, FText::FromString(DefinitionId));
	}

	FText ErrorSentence(ESpnInventoryError Error)
	{
		switch (Error)
		{
		case ESpnInventoryError::WeightLimitReached: return FSpnUiText::Get(TEXT("ui.world.err.weight"));
		case ESpnInventoryError::VolumeLimitReached: return FSpnUiText::Get(TEXT("ui.world.err.volume"));
		case ESpnInventoryError::StackLimitReached: return FSpnUiText::Get(TEXT("ui.world.err.stacks"));
		case ESpnInventoryError::CategoryNotAllowed: return FSpnUiText::Get(TEXT("ui.world.err.category"));
		case ESpnInventoryError::NoCatalog: return FSpnUiText::Get(TEXT("ui.world.err.nocatalog"));
		case ESpnInventoryError::UnknownDefinition: return FSpnUiText::Get(TEXT("ui.world.err.unknown"));
		default: return FSpnUiText::Get(TEXT("ui.world.err.other"));
		}
	}

	uint64 FindPlayerContainer(const USpnInventorySubsystem& Inventory)
	{
		FSpnContainerView View;
		if (Inventory.GetContainer(PlayerContainerId, View))
		{
			return PlayerContainerId;
		}
		uint64 Lowest = 0;
		for (const uint64 Id : Inventory.ListContainers())
		{
			if (!SpnWorldContainerRegistry().Contains(static_cast<int64>(Id)) && (Lowest == 0 || Id < Lowest))
			{
				Lowest = Id;
			}
		}
		return Lowest;
	}
}

namespace
{
	UStaticMesh* CubeMesh()
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
		return Cube.Succeeded() ? Cube.Object : nullptr;
	}
}

// --- ASpnWorldPickup ------------------------------------------------------------------------------------------------------------------

ASpnWorldPickup::ASpnWorldPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);
	Body->SetRelativeLocation(FVector(0.f, 0.f, 15.f));
	Body->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Body->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	if (UStaticMesh* Mesh = CubeMesh())
	{
		Body->SetStaticMesh(Mesh);
	}
	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetRelativeLocation(FVector(0.f, 0.f, 48.f));
	Label->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(10.f);
	Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpnWorldPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Label->SetText(FText::FromString(FString::Printf(TEXT("PLACEHOLDER: %s x%d"), *DefinitionId, Quantity)));
}

USpnWorldStateSubsystem* ASpnWorldPickup::GetWorldState() const
{
	const UGameInstance* Instance = GetGameInstance();
	return Instance != nullptr ? Instance->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
}

USpnInventorySubsystem* ASpnWorldPickup::GetInventory() const
{
	const UGameInstance* Instance = GetGameInstance();
	return Instance != nullptr ? Instance->GetSubsystem<USpnInventorySubsystem>() : nullptr;
}

FString ASpnWorldPickup::TakenKey() const
{
	return FString::Printf(TEXT("world.pickup.%s.taken"), *StableId);
}

bool ASpnWorldPickup::IsTaken() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	return WorldState != nullptr && WorldState->ReadFlag(TakenKey()).Get(false);
}

void ASpnWorldPickup::BeginPlay()
{
	Super::BeginPlay();
	if (!USpnWorldStateSubsystem::IsValidInteractableId(StableId))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s: StableId '%s' breaks the id rule; the pickup cannot be used"), *GetName(), *StableId);
	}
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		ReplacedHandle = WorldState->OnWorldStateReplaced.AddUObject(this, &ASpnWorldPickup::Refresh);
	}
	Refresh();
}

void ASpnWorldPickup::EndPlay(const EEndPlayReason::Type Reason)
{
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldState->OnWorldStateReplaced.Remove(ReplacedHandle);
	}
	ReplacedHandle.Reset();
	Super::EndPlay(Reason);
}

void ASpnWorldPickup::Refresh()
{
	const bool bTaken = IsTaken();
	SetActorHiddenInGame(bTaken);
	SetActorEnableCollision(!bTaken);
}

FString ASpnWorldPickup::SpnPromptText(const AActor* User) const
{
	if (IsTaken())
	{
		return FString();
	}
	return FSpnUiText::Get(TEXT("ui.world.prompt.take"), {SpnWorld::ItemDisplayName(DefinitionId), FText::AsNumber(Quantity)}).ToString();
}

FSpnInteractStatus ASpnWorldPickup::SpnUse(AActor* User)
{
	FSpnInteractStatus Status;
	Status.Result = ESpnInteractResult::Failed;
	USpnInventorySubsystem* Inventory = GetInventory();
	USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (Inventory == nullptr || WorldState == nullptr || !Inventory->HasCatalog())
	{
		Status.Detail = TEXT("inventory or world state unavailable");
		SpnUi::Notify(this, SpnWorld::ErrorSentence(ESpnInventoryError::NoCatalog), SpnUi::ENote::Warning);
		return Status;
	}
	if (IsTaken())
	{
		Status.Result = ESpnInteractResult::Refused;
		Status.Feedback = TEXT("spn:feedback.pickup.taken");
		return Status;
	}
	const uint64 Container = SpnWorld::FindPlayerContainer(*Inventory);
	if (Container == 0)
	{
		Status.Detail = TEXT("no player container");
		SpnUi::Notify(this, SpnWorld::ErrorSentence(ESpnInventoryError::UnknownContainer), SpnUi::ENote::Warning);
		return Status;
	}

	const TArray<FSpnInventoryCommand> Commands{FSpnInventoryCommand::AddItem(Container, DefinitionId, static_cast<uint32>(FMath::Max(1, Quantity)))};
	// 1) dry run: the Core says whether it fits (capacity, weight, volume, category). Nothing changes if not.
	const FSpnInventoryStatus Probe = Inventory->ProbeCommands(Commands);
	if (!Probe.IsOk())
	{
		Status.Result = ESpnInteractResult::Refused;
		Status.Feedback = TEXT("spn:feedback.pickup.no_room");
		Status.Detail = LexToString(Probe.Error);
		SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.world.cannot_take"), {SpnWorld::ItemDisplayName(DefinitionId), SpnWorld::ErrorSentence(Probe.Error)}), SpnUi::ENote::Warning);
		return Status;
	}
	// 2) mark taken (reversible), 3) move the item; if the move fails after all, the mark is undone: no loss, no duplication
	const FString Cause = FString::Printf(TEXT("unreal.pickup:%s"), *GetNameSafe(User));
	if (!WorldState->SetFlag(TakenKey(), true, Cause))
	{
		Status.Detail = TEXT("cannot record the pickup");
		return Status;
	}
	const FSpnInventoryStatus Applied = Inventory->Apply(Commands);
	if (!Applied.IsOk())
	{
		WorldState->SetFlag(TakenKey(), false, TEXT("unreal.pickup:rollback"));
		Status.Result = ESpnInteractResult::Refused;
		Status.Detail = LexToString(Applied.Error);
		SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.world.cannot_take"), {SpnWorld::ItemDisplayName(DefinitionId), SpnWorld::ErrorSentence(Applied.Error)}), SpnUi::ENote::Warning);
		return Status;
	}
	Refresh();
	Status.Result = ESpnInteractResult::Changed;
	Status.Feedback = TEXT("spn:feedback.pickup.taken_now");
	SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.world.picked_up"), {SpnWorld::ItemDisplayName(DefinitionId), FText::AsNumber(Quantity)}), SpnUi::ENote::Good);
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): took %d x %s into container %llu (inventory revision %llu)"), *GetName(), *StableId, Quantity, *DefinitionId,
		static_cast<unsigned long long>(Container), static_cast<unsigned long long>(Applied.Revision));
	return Status;
}

// --- ASpnWorldContainer ---------------------------------------------------------------------------------------------------------------

ASpnWorldContainer::ASpnWorldContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);
	Body->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	Body->SetRelativeScale3D(FVector(0.8f, 0.5f, 0.5f));
	Body->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	if (UStaticMesh* Mesh = CubeMesh())
	{
		Body->SetStaticMesh(Mesh);
	}
	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetRelativeLocation(FVector(0.f, 0.f, 62.f));
	Label->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(10.f);
	Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpnWorldContainer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Label->SetText(FText::FromString(FString::Printf(TEXT("PLACEHOLDER: %s"), *Title)));
}

USpnInventorySubsystem* ASpnWorldContainer::GetInventory() const
{
	const UGameInstance* Instance = GetGameInstance();
	return Instance != nullptr ? Instance->GetSubsystem<USpnInventorySubsystem>() : nullptr;
}

uint64 ASpnWorldContainer::GetContainerId() const
{
	return SpnWorld::ContainerIdFor(StableId);
}

bool ASpnWorldContainer::EnsureContainer()
{
	USpnInventorySubsystem* Inventory = GetInventory();
	if (Inventory == nullptr || !Inventory->HasCatalog())
	{
		return false;
	}
	FSpnContainerView Existing;
	if (Inventory->GetContainer(GetContainerId(), Existing))
	{
		return true; // restored from a save or created before: its contents are the truth, never re-seeded
	}
	FSpnContainerRulesData Rules;
	if (!Inventory->FindContainerPreset(PresetId, Rules))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s (%s): container preset '%s' is not in the catalog"), *GetName(), *StableId, *PresetId);
		return false;
	}
	TArray<FSpnInventoryCommand> Commands{FSpnInventoryCommand::CreateContainer(GetContainerId(), Rules)};
	for (const FSpnContainerSeed& Seed : InitialItems)
	{
		Commands.Add(FSpnInventoryCommand::AddItem(GetContainerId(), Seed.DefinitionId, static_cast<uint32>(FMath::Max(1, Seed.Quantity))));
	}
	const FSpnInventoryStatus Status = Inventory->Apply(Commands); // one transaction: the container with all its items, or nothing
	if (!Status.IsOk())
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s (%s): creating the container failed: %s (%s)"), *GetName(), *StableId, LexToString(Status.Error), *Status.Detail);
		return false;
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): created container %llu with %d initial item(s)"), *GetName(), *StableId,
		static_cast<unsigned long long>(GetContainerId()), InitialItems.Num());
	return true;
}

void ASpnWorldContainer::BeginPlay()
{
	Super::BeginPlay();
	if (!USpnWorldStateSubsystem::IsValidInteractableId(StableId))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s: StableId '%s' breaks the id rule; the container cannot be used"), *GetName(), *StableId);
		return;
	}
	SpnWorldContainerRegistry().Add(static_cast<int64>(GetContainerId()));
	if (USpnInventorySubsystem* Inventory = GetInventory())
	{
		// a load or a new game replaces the whole inventory: make sure the container exists again (a loaded one keeps its saved contents)
		ReplacedHandle = Inventory->OnInventoryReplaced.AddWeakLambda(this, [this]() { EnsureContainer(); });
	}
	EnsureContainer();
}

void ASpnWorldContainer::EndPlay(const EEndPlayReason::Type Reason)
{
	if (USpnInventorySubsystem* Inventory = GetInventory())
	{
		Inventory->OnInventoryReplaced.Remove(ReplacedHandle);
	}
	ReplacedHandle.Reset();
	SpnWorldContainerRegistry().Remove(static_cast<int64>(GetContainerId()));
	Super::EndPlay(Reason);
}

FString ASpnWorldContainer::SpnPromptText(const AActor* User) const
{
	return FSpnUiText::Get(TEXT("ui.world.prompt.open"), {FText::FromString(Title)}).ToString();
}

FSpnInteractStatus ASpnWorldContainer::SpnUse(AActor* User)
{
	FSpnInteractStatus Status;
	Status.Result = ESpnInteractResult::Failed;
	if (!EnsureContainer())
	{
		Status.Detail = TEXT("container unavailable");
		SpnUi::Notify(this, SpnWorld::ErrorSentence(ESpnInventoryError::NoCatalog), SpnUi::ENote::Warning);
		return Status;
	}
	const APawn* Pawn = Cast<APawn>(User);
	const APlayerController* Controller = Pawn != nullptr ? Cast<APlayerController>(Pawn->GetController()) : nullptr;
	USpnInventoryUiSubsystem* Ui = Controller != nullptr && Controller->GetLocalPlayer() != nullptr ? Controller->GetLocalPlayer()->GetSubsystem<USpnInventoryUiSubsystem>() : nullptr;
	if (Ui == nullptr || !Ui->OpenContainer(static_cast<int64>(GetContainerId())))
	{
		Status.Detail = TEXT("no local inventory screen");
		return Status;
	}
	Status.Result = ESpnInteractResult::Unchanged; // opening changes no state
	Status.Feedback = TEXT("spn:feedback.container.opened");
	return Status;
}
