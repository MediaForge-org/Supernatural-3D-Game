#include "Spn/SpnTestPickup.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "SpnWorldStateSubsystem.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	// Authored default names of the core test pickup (spn::core::kTestPickupItemState / kTestPickupAmountState).
	const TCHAR* const ItemState = TEXT("item");
	const TCHAR* const AmountState = TEXT("amount");

	// Engine basic cube (100 cm, centred): scaled to a 40 cm placeholder box standing on the floor.
	const FVector BodyScale(0.4f, 0.4f, 0.4f);
	const FVector BodyOffset(0.f, 0.f, 20.f);
}

ASpnTestPickup::ASpnTestPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);
	Body->SetRelativeLocation(BodyOffset);
	Body->SetRelativeScale3D(BodyScale);
	Body->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Body->SetStaticMesh(CubeMesh.Object);
	}

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetRelativeLocation(FVector(0.f, 0.f, 2.f * BodyOffset.Z + 20.f));
	Label->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(14.f);
	Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpnTestPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Label->SetText(FText::FromString(FString::Printf(TEXT("PLACEHOLDER pickup: %s x%d"), *ItemId, Amount)));
}

void ASpnTestPickup::BeginPlay()
{
	Super::BeginPlay();

	if (!USpnWorldStateSubsystem::IsValidInteractableId(StableId))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s: StableId '%s' breaks the id rule ([a-z][a-z0-9_.]*, at most 96 chars); Use will fail"),
			*GetName(), *StableId);
	}
	if (!USpnWorldStateSubsystem::IsValidItemId(ItemId))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s (%s): ItemId '%s' breaks the item rule ([a-z][a-z0-9_]*, at most 48 chars); Use will fail"),
			*GetName(), *StableId, *ItemId);
	}
	WarnAboutDuplicateIds();

	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldStateReplacedHandle = WorldState->OnWorldStateReplaced.AddUObject(this, &ASpnTestPickup::HandleWorldStateReplaced);
	}
	else
	{
		UE_LOG(LogSupernaturalProt, Warning, TEXT("%s (%s): no world state in %s; the pickup will not react to Use or loads"),
			*GetName(), *StableId, *GetPathNameSafe(GetWorld()));
	}
	RefreshFromWorldState();
}

void ASpnTestPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldState->OnWorldStateReplaced.Remove(WorldStateReplacedHandle);
	}
	WorldStateReplacedHandle.Reset();
	Super::EndPlay(EndPlayReason);
}

FSpnInteractStatus ASpnTestPickup::SpnUse(AActor* User)
{
	USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (WorldState == nullptr)
	{
		FSpnInteractStatus Status;
		Status.Result = ESpnInteractResult::Failed;
		Status.Detail = TEXT("no world state");
		return Status;
	}

	// Single player: every user is the local player. Co-op later maps User to its own holder id.
	const FString Cause = FString::Printf(TEXT("unreal.use:%s"), *GetNameSafe(User));
	const FSpnInteractStatus Status = WorldState->TryInteract(USpnWorldStateSubsystem::TestPickupKind(), StableId,
		ESpnInteractionVerb::Use, AuthoredDefaults(), USpnWorldStateSubsystem::DefaultHolder(), Cause);

	switch (Status.Result)
	{
	case ESpnInteractResult::Changed:
	case ESpnInteractResult::Unchanged:
		RefreshFromWorldState();
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): took %d x %s, now %lld"), *GetName(), *StableId, Amount, *ItemId,
			static_cast<long long>(WorldState->ReadInventoryCount(USpnWorldStateSubsystem::DefaultHolder(), ItemId).Get(0)));
		break;
	case ESpnInteractResult::Refused:
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s) refused: %s"), *GetName(), *StableId, *Status.Feedback);
		break;
	default:
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s (%s): Use failed: %s %s"), *GetName(), *StableId, LexToString(Status.Result), *Status.Detail);
		break;
	}
	return Status;
}

bool ASpnTestPickup::IsTaken() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	const TOptional<bool> Fact = WorldState != nullptr ? WorldState->ReadFlag(USpnWorldStateSubsystem::TestPickupTakenKey(StableId)) : TOptional<bool>();
	return Fact.Get(false);
}

USpnWorldStateSubsystem* ASpnTestPickup::GetWorldState() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	return GameInstance != nullptr ? GameInstance->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
}

FSpnInteractDefaults ASpnTestPickup::AuthoredDefaults() const
{
	FSpnInteractDefaults Defaults;
	Defaults.Strings.Add(FString(ItemState), ItemId);
	Defaults.Ints.Add(FString(AmountState), static_cast<int64>(Amount));
	return Defaults;
}

void ASpnTestPickup::RefreshFromWorldState()
{
	const bool bTaken = IsTaken();
	SetActorHiddenInGame(bTaken);
	SetActorEnableCollision(!bTaken);
}

void ASpnTestPickup::HandleWorldStateReplaced()
{
	RefreshFromWorldState();
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): world state replaced, now %s"), *GetName(), *StableId, IsTaken() ? TEXT("taken") : TEXT("present"));
}

void ASpnTestPickup::WarnAboutDuplicateIds() const
{
	for (TActorIterator<ASpnTestPickup> It(GetWorld()); It; ++It)
	{
		if (*It != this && It->StableId.Equals(StableId, ESearchCase::CaseSensitive))
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s and %s share StableId '%s': they will share one saved state"),
				*GetName(), *It->GetName(), *StableId);
		}
	}
}
