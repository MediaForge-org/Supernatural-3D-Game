#include "Spn/SpnBuildingObjects.h"

#include "Components/LightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
#include "SpnWorldStateSubsystem.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

// --- ASpnTestKey --------------------------------------------------------------------------------------------------

ASpnTestKey::ASpnTestKey()
{
	PrimaryActorTick.bCanEverTick = false;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Body->SetStaticMesh(CubeMesh.Object);
	}
	Body->SetRelativeScale3D(FVector(0.16f, 0.05f, 0.03f));   // 16 x 5 x 3 cm placeholder key
	Body->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Body->SetCanEverAffectNavigation(false);
}

USpnWorldStateSubsystem* ASpnTestKey::GetWorldState() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	return GameInstance != nullptr ? GameInstance->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
}

bool ASpnTestKey::IsHeld() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	return WorldState != nullptr && WorldState->ReadFlag(USpnWorldStateSubsystem::TestKeyHeldKey(KeyId)).Get(false);
}

void ASpnTestKey::BeginPlay()
{
	Super::BeginPlay();
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		ReplacedHandle = WorldState->OnWorldStateReplaced.AddUObject(this, &ASpnTestKey::Refresh);
	}
	Refresh();
}

void ASpnTestKey::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldState->OnWorldStateReplaced.Remove(ReplacedHandle);
	}
	ReplacedHandle.Reset();
	Super::EndPlay(EndPlayReason);
}

void ASpnTestKey::Refresh()
{
	const bool bHeld = IsHeld();
	SetActorHiddenInGame(bHeld);
	SetActorEnableCollision(!bHeld);
}

FSpnInteractStatus ASpnTestKey::SpnUse(AActor* User)
{
	FSpnInteractStatus Status;
	USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (WorldState == nullptr || !USpnWorldStateSubsystem::IsValidInteractableId(KeyId))
	{
		Status.Result = ESpnInteractResult::Failed;
		Status.Detail = TEXT("no world state or invalid key id");
		return Status;
	}
	if (IsHeld())
	{
		Status.Result = ESpnInteractResult::Unchanged;
		return Status;
	}
	if (!WorldState->SetFlag(USpnWorldStateSubsystem::TestKeyHeldKey(KeyId), true, FString::Printf(TEXT("unreal.key:%s"), *GetNameSafe(User))))
	{
		Status.Result = ESpnInteractResult::Failed;
		Status.Detail = TEXT("fact write failed");
		return Status;
	}
	Status.Result = ESpnInteractResult::Changed;
	Refresh();
	SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.key.msg.taken"), {DisplayName}), SpnUi::ENote::Good);
	return Status;
}

FString ASpnTestKey::SpnPromptText(const AActor* User) const
{
	return IsHeld() ? FString() : FString::Printf(TEXT("%s nehmen"), *DisplayName.ToString());
}

// --- ASpnLightSwitch ----------------------------------------------------------------------------------------------

ASpnLightSwitch::ASpnLightSwitch()
{
	PrimaryActorTick.bCanEverTick = false;
	Plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate"));
	SetRootComponent(Plate);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Plate->SetStaticMesh(CubeMesh.Object);
	}
	Plate->SetRelativeScale3D(FVector(0.03f, 0.08f, 0.12f));   // 3 x 8 x 12 cm wall plate
	Plate->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Plate->SetCanEverAffectNavigation(false);
}

USpnWorldStateSubsystem* ASpnLightSwitch::GetWorldState() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	return GameInstance != nullptr ? GameInstance->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
}

bool ASpnLightSwitch::IsOn() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	const TOptional<bool> Fact = WorldState != nullptr ? WorldState->ReadFlag(USpnWorldStateSubsystem::SwitchOnKey(StableId)) : TOptional<bool>();
	return Fact.Get(bAuthoredOn);
}

void ASpnLightSwitch::BeginPlay()
{
	Super::BeginPlay();
	if (!USpnWorldStateSubsystem::IsValidInteractableId(StableId))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s: StableId '%s' breaks the id rule; Use will fail"), *GetName(), *StableId);
	}
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		ReplacedHandle = WorldState->OnWorldStateReplaced.AddUObject(this, &ASpnLightSwitch::Refresh);
	}
	Refresh();
}

void ASpnLightSwitch::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldState->OnWorldStateReplaced.Remove(ReplacedHandle);
	}
	ReplacedHandle.Reset();
	Super::EndPlay(EndPlayReason);
}

void ASpnLightSwitch::Refresh()
{
	const bool bOn = IsOn();
	for (AActor* Actor : Lights)
	{
		if (Actor == nullptr)
		{
			continue;
		}
		TArray<ULightComponent*> Components;
		Actor->GetComponents<ULightComponent>(Components);
		for (ULightComponent* Light : Components)
		{
			Light->SetVisibility(bOn);
		}
	}
}

FSpnInteractStatus ASpnLightSwitch::SpnUse(AActor* User)
{
	USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (WorldState == nullptr)
	{
		FSpnInteractStatus Status;
		Status.Result = ESpnInteractResult::Failed;
		Status.Detail = TEXT("no world state");
		return Status;
	}
	FSpnInteractDefaults Defaults;
	Defaults.Bools.Add(TEXT("on"), bAuthoredOn);
	const FSpnInteractStatus Status = WorldState->TryInteract(USpnWorldStateSubsystem::SwitchKind(), StableId, ESpnInteractionVerb::Use,
		Defaults, USpnWorldStateSubsystem::DefaultHolder(), FString::Printf(TEXT("unreal.use:%s"), *GetNameSafe(User)));
	if (Status.Result == ESpnInteractResult::Changed || Status.Result == ESpnInteractResult::Unchanged)
	{
		Refresh();
	}
	return Status;
}

FString ASpnLightSwitch::SpnPromptText(const AActor* User) const
{
	return IsOn() ? TEXT("Licht ausschalten") : TEXT("Licht einschalten");
}
