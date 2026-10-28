#include "Spn/SpnTestDoor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "SpnWorldStateSubsystem.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	// Authored default names of the core test door (spn::core::kTestDoorOpenState / kTestDoorLockedState).
	const TCHAR* const OpenState = TEXT("open");
	const TCHAR* const LockedState = TEXT("locked");

	// Engine basic cube (100 cm, centred): scaled to a 100 x 10 x 200 cm placeholder panel.
	const FVector PanelScale(1.f, 0.1f, 2.f);
	const FVector PanelOffset(50.f, 0.f, 100.f); // hinge edge at the actor origin, bottom on the floor
}

ASpnTestDoor::ASpnTestDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false; // ticks only while swinging

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
	Hinge->SetupAttachment(Root);

	Panel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Panel"));
	Panel->SetupAttachment(Hinge);
	Panel->SetRelativeLocation(PanelOffset);
	Panel->SetRelativeScale3D(PanelScale);
	Panel->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Panel->SetStaticMesh(CubeMesh.Object);
	}

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetRelativeLocation(FVector(PanelOffset.X, 0.f, 2.f * PanelOffset.Z + 25.f));
	Label->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(18.f);
	Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpnTestDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Label->SetText(FText::FromString(FString::Printf(TEXT("PLACEHOLDER door: %s%s"), *StableId, bAuthoredLocked ? TEXT(" (locked)") : TEXT(""))));

	// Editor preview of the authored state; BeginPlay replaces it with the world state.
	TargetAngle = bAuthoredOpen ? OpenAngle : 0.f;
	SetCurrentAngle(TargetAngle);
}

void ASpnTestDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!USpnWorldStateSubsystem::IsValidInteractableId(StableId))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s: StableId '%s' breaks the id rule ([a-z][a-z0-9_.]*, at most 96 chars); Use will fail"),
			*GetName(), *StableId);
	}
	WarnAboutDuplicateIds();

	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldStateReplacedHandle = WorldState->OnWorldStateReplaced.AddUObject(this, &ASpnTestDoor::HandleWorldStateReplaced);
		UE_LOG(LogSupernaturalProt, Verbose, TEXT("%s (%s): following the world state of %s"), *GetName(), *StableId, *GetNameSafe(GetGameInstance()));
	}
	else
	{
		UE_LOG(LogSupernaturalProt, Warning, TEXT("%s (%s): no world state in %s; the door will not react to Use or loads"),
			*GetName(), *StableId, *GetPathNameSafe(GetWorld()));
	}
	RefreshFromWorldState(/*bAnimate*/ false);
}

void ASpnTestDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (USpnWorldStateSubsystem* WorldState = GetWorldState())
	{
		WorldState->OnWorldStateReplaced.Remove(WorldStateReplacedHandle);
	}
	WorldStateReplacedHandle.Reset();
	Super::EndPlay(EndPlayReason);
}

void ASpnTestDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetCurrentAngle(FMath::FInterpConstantTo(CurrentAngle, TargetAngle, DeltaSeconds, SwingSpeed));
	if (FMath::IsNearlyEqual(CurrentAngle, TargetAngle))
	{
		SetCurrentAngle(TargetAngle);
		SetActorTickEnabled(false);
	}
}

FSpnInteractStatus ASpnTestDoor::SpnUse(AActor* User)
{
	USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (WorldState == nullptr)
	{
		FSpnInteractStatus Status;
		Status.Result = ESpnInteractResult::Failed;
		Status.Detail = TEXT("no world state");
		return Status;
	}

	const FString Cause = FString::Printf(TEXT("unreal.use:%s"), *GetNameSafe(User));
	const FSpnInteractStatus Status = WorldState->TryInteract(USpnWorldStateSubsystem::TestDoorKind(), StableId,
		ESpnInteractionVerb::Use, AuthoredDefaults(), Cause);

	switch (Status.Result)
	{
	case ESpnInteractResult::Changed:
	case ESpnInteractResult::Unchanged:
		RefreshFromWorldState(/*bAnimate*/ true);
		break;
	case ESpnInteractResult::Refused:
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s) refused: %s"), *GetName(), *StableId, *Status.Feedback);
#if !UE_BUILD_SHIPPING
		if (GEngine != nullptr)
		{
			// Debug text only; player-facing feedback (UI, audio) maps the feedback id later.
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("%s: %s"), *StableId, *Status.Feedback));
		}
#endif
		break;
	default:
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s (%s): Use failed: %s %s"), *GetName(), *StableId, LexToString(Status.Result), *Status.Detail);
		break;
	}
	return Status;
}

FString ASpnTestDoor::SpnPromptText(const AActor* User) const
{
	if (IsOpen())
	{
		return TEXT("Close door");
	}
	return bAuthoredLocked ? TEXT("Door (locked)") : TEXT("Open door");
}

bool ASpnTestDoor::IsOpen() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	const TOptional<bool> Fact = WorldState != nullptr ? WorldState->ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(StableId)) : TOptional<bool>();
	return Fact.Get(bAuthoredOpen);
}

USpnWorldStateSubsystem* ASpnTestDoor::GetWorldState() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	return GameInstance != nullptr ? GameInstance->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
}

TMap<FString, bool> ASpnTestDoor::AuthoredDefaults() const
{
	return {{OpenState, bAuthoredOpen}, {LockedState, bAuthoredLocked}};
}

void ASpnTestDoor::RefreshFromWorldState(bool bAnimate)
{
	TargetAngle = IsOpen() ? OpenAngle : 0.f;
	if (bAnimate)
	{
		SetActorTickEnabled(!FMath::IsNearlyEqual(CurrentAngle, TargetAngle));
	}
	else
	{
		SetCurrentAngle(TargetAngle);
		SetActorTickEnabled(false);
	}
}

void ASpnTestDoor::HandleWorldStateReplaced()
{
	// A load shows the saved state at once, without a visible swing.
	RefreshFromWorldState(/*bAnimate*/ false);
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): world state replaced, now shown %s"), *GetName(), *StableId, IsOpen() ? TEXT("open") : TEXT("closed"));
}

void ASpnTestDoor::SetCurrentAngle(float Angle)
{
	CurrentAngle = Angle;
	Hinge->SetRelativeRotation(FRotator(0.f, CurrentAngle, 0.f));
}

void ASpnTestDoor::WarnAboutDuplicateIds() const
{
	for (TActorIterator<ASpnTestDoor> It(GetWorld()); It; ++It)
	{
		if (*It != this && It->StableId.Equals(StableId, ESearchCase::CaseSensitive))
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s and %s share StableId '%s': they will share one saved state"),
				*GetName(), *It->GetName(), *StableId);
		}
	}
}
