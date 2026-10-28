#include "Spn/SpnTestDoor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "CollisionQueryParams.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
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

	// Handle bars (cube scaled to 4 x 6 x 16 cm) near the free edge of the leaf, one per face (the leaf is 10 cm thick).
	const FVector HandleScale(0.04f, 0.06f, 0.16f);
	const float HandleInset = 10.f;   // distance of the handle from the free edge of the leaf
	const float HandleHeight = 100.f;
	const FVector HingeScale(0.04f, 0.04f, 0.14f);   // cylinder 100 cm wide, 200 cm high at scale 1: barrel 4 cm wide, 28 cm high
	const float HingeHeights[] = {25.f, 100.f, 175.f};
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

	const auto MakeHandle = [this](const TCHAR* Name, const FVector& Offset)
	{
		UStaticMeshComponent* Handle = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Handle->SetupAttachment(Hinge);
		Handle->SetRelativeLocation(Offset);
		Handle->SetRelativeScale3D(HandleScale);
		Handle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Handle->SetVisibility(false);
		Handle->SetGenerateOverlapEvents(false);
		return Handle;
	};
	HandleFront = MakeHandle(TEXT("HandleFront"), FVector::ZeroVector);
	HandleBack = MakeHandle(TEXT("HandleBack"), FVector::ZeroVector);
	if (CubeMesh.Succeeded())
	{
		HandleFront->SetStaticMesh(CubeMesh.Object);
		HandleBack->SetStaticMesh(CubeMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(HingeHeights); ++Index)
	{
		UStaticMeshComponent* Barrel = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("HingeBarrel%d"), Index));
		Barrel->SetupAttachment(Hinge);
		Barrel->SetRelativeLocation(FVector(0.f, 0.f, HingeHeights[Index]));
		Barrel->SetRelativeScale3D(HingeScale);
		Barrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Barrel->SetGenerateOverlapEvents(false);
		Barrel->SetVisibility(false);
		if (CylinderMesh.Succeeded())
		{
			Barrel->SetStaticMesh(CylinderMesh.Object);
		}
		HingeBarrels.Add(Barrel);
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

	if (LeafMaterial != nullptr)
	{
		Panel->SetMaterial(0, LeafMaterial);
	}
	// Leaf geometry: the hinge edge stays at the actor origin, width and thickness follow the properties.
	Panel->SetRelativeScale3D(FVector(LeafWidth / 100.f, LeafThickness / 100.f, PanelScale.Z));
	Panel->SetRelativeLocation(FVector(LeafWidth / 2.f, 0.f, PanelOffset.Z));
	Label->SetVisibility(LeafMaterial == nullptr);   // the "PLACEHOLDER door" text only on unstyled doors
	HandleFront->SetRelativeLocation(FVector(LeafWidth - HandleInset, -(LeafThickness / 2.f + 3.f), HandleHeight));
	HandleBack->SetRelativeLocation(FVector(LeafWidth - HandleInset, LeafThickness / 2.f + 3.f, HandleHeight));
	for (UStaticMeshComponent* Handle : {HandleFront.Get(), HandleBack.Get()})
	{
		Handle->SetVisibility(bShowHandle);
		if (HardwareMaterial != nullptr) { Handle->SetMaterial(0, HardwareMaterial); }
	}
	for (UStaticMeshComponent* Barrel : HingeBarrels)
	{
		Barrel->SetVisibility(bShowHinges);
		if (HardwareMaterial != nullptr) { Barrel->SetMaterial(0, HardwareMaterial); }
	}

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

bool ASpnTestDoor::IsLocked() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	const TOptional<bool> Fact = WorldState != nullptr ? WorldState->ReadFlag(USpnWorldStateSubsystem::TestDoorLockedKey(StableId)) : TOptional<bool>();
	return Fact.Get(bAuthoredLocked);
}

bool ASpnTestDoor::IsClosingObstructed() const
{
	const UWorld* World = GetWorld();
	const UStaticMesh* Mesh = Panel != nullptr ? Panel->GetStaticMesh() : nullptr;
	if (World == nullptr || Mesh == nullptr || Hinge == nullptr || Root == nullptr)
	{
		return false;
	}
	const FVector HalfExtent = Mesh->GetBounds().BoxExtent * Panel->GetRelativeScale3D().GetAbs();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnDoorClosing), /*bTraceComplex*/ false, this);
	const FCollisionObjectQueryParams Pawns(ECC_Pawn);
	const float StepDegrees = 10.f;
	const int32 Steps = FMath::Max(1, FMath::CeilToInt(FMath::Abs(CurrentAngle) / StepDegrees));
	for (int32 Step = 0; Step <= Steps; ++Step)
	{
		// From the current leaf position (Step 0) to the closed one (angle 0).
		const float Angle = CurrentAngle * (1.f - static_cast<float>(Step) / static_cast<float>(Steps));
		const FTransform HingeWorld = FTransform(FRotator(0.f, Angle, 0.f), Hinge->GetRelativeLocation()) * Root->GetComponentTransform();
		const FTransform PanelWorld = FTransform(Panel->GetRelativeRotation(), Panel->GetRelativeLocation()) * HingeWorld;
		if (World->OverlapAnyTestByObjectType(PanelWorld.GetLocation(), PanelWorld.GetRotation(), Pawns, FCollisionShape::MakeBox(HalfExtent), Params))
		{
			return true;
		}
	}
	return false;
}

FSpnInteractStatus ASpnTestDoor::SpnUse(AActor* User)
{
	if (IsOpen() && IsClosingObstructed())
	{
		// A closing door must never be pushed into a pawn: refuse, change no state. Feedback id = spn::core::TestDoorBlockedFeedback().
		FSpnInteractStatus Blocked;
		Blocked.Result = ESpnInteractResult::Refused;
		Blocked.Feedback = TEXT("placeholder:feedback.door.blocked");
		Blocked.Detail = TEXT("a pawn is in the swing of the door");
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): closing refused, a pawn is in the way"), *GetName(), *StableId);
		return Blocked;
	}
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
		return TEXT("T\u00fcr schlie\u00dfen");
	}
	return IsLocked() ? TEXT("T\u00fcr (verschlossen)") : TEXT("T\u00fcr \u00f6ffnen");
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
