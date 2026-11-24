#include "Spn/SpnTestDoor.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Pawn.h"
#include "SupernaturalProtCharacter.h"
#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "CollisionQueryParams.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "SpnWorldStateSubsystem.h"
#include "SpnNpcReaction.h"
#include "SupernaturalProt.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
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

	const float NextAngle = FMath::FInterpConstantTo(CurrentAngle, TargetAngle, DeltaSeconds, SwingSpeed);
	if (!FMath::IsNearlyEqual(NextAngle, CurrentAngle))
	{
		// Every frame of a swing runs only the cheap any-hit test. The second test (opener ignored) runs only while the swing is blocked:
		// if the sweep is free without the opener, the opener is the only pawn in the way.
		if (IsSweepBlockedByPawn(CurrentAngle, NextAngle))
		{
			// A pawn stands in the way of the leaf: the door stops against it and waits (it never pushes a pawn away). It resumes as soon as the
			// way is free. If the only pawn in the way is the one who just opened the door, it is asked to take a short step out of the swing.
			const bool bStartsWaiting = !bWaitingForPawn;
			if (bStartsWaiting)
			{
				bWaitingForPawn = true;
				UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): the swing is blocked by a pawn, waiting"), *GetName(), *StableId);
			}
			APawn* const OpenerPawn = IsOpen() ? Opener.Get() : nullptr;
			const bool bOnlyOpener = OpenerPawn != nullptr && !IsSweepBlockedByPawn(CurrentAngle, NextAngle, OpenerPawn);
			if (bOnlyOpener && !bStepAsideAsked)
			{
				bStepAsideAsked = true;
				LetOpenerStepAside(*OpenerPawn);
				if (StepAsideResult == EStepAside::NoRoom)
				{
					SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.door.msg.obstructed")), SpnUi::ENote::Warning);
				}
			}
			else if (bStartsWaiting && !bOnlyOpener)
			{
				SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.door.msg.obstructed")), SpnUi::ENote::Warning);
			}
			return;
		}
	}
	bWaitingForPawn = false;
	SetCurrentAngle(NextAngle);
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
	return !IsBroken() && Fact.Get(bAuthoredLocked);   // a broken lock holds nothing
}

bool ASpnTestDoor::IsClosingObstructed() const
{
	return IsSweepBlockedByPawn(CurrentAngle, 0.f);
}

FVector ASpnTestDoor::GetHingeLocation() const
{
	return Hinge != nullptr && Root != nullptr ? (FTransform(Hinge->GetRelativeLocation()) * Root->GetComponentTransform()).GetLocation() : GetActorLocation();
}

float ASpnTestDoor::GetLeafReach() const
{
	const UStaticMesh* Mesh = Panel != nullptr ? Panel->GetStaticMesh() : nullptr;
	if (Mesh == nullptr)
	{
		return 0.f;
	}
	// farthest corner of the leaf box from the hinge axis, measured in the hinge's frame (the leaf only turns about that axis)
	const FBox Box = Mesh->GetBounds().GetBox();
	const FTransform PanelInHinge(Panel->GetRelativeRotation(), Panel->GetRelativeLocation(), Panel->GetRelativeScale3D());
	float Reach = 0.f;
	for (int32 I = 0; I < 8; ++I)
	{
		const FVector Corner((I & 1) ? Box.Max.X : Box.Min.X, (I & 2) ? Box.Max.Y : Box.Min.Y, (I & 4) ? Box.Max.Z : Box.Min.Z);
		Reach = FMath::Max(Reach, FVector2D(PanelInHinge.TransformPosition(Corner)).Size());
	}
	return Reach * Root->GetComponentScale().X;
}

void ASpnTestDoor::LetOpenerStepAside(APawn& Pawn)
{
	// Out of the swing for good: radially away from the hinge to just beyond the leaf's reach plus the pawn's radius. Anything farther from
	// the hinge than the reach can never be touched by the leaf, whatever the remaining angle is.
	ASupernaturalProtCharacter* Character = Cast<ASupernaturalProtCharacter>(&Pawn);
	const UCapsuleComponent* Capsule = Character != nullptr ? Character->GetCapsuleComponent() : nullptr;
	if (Character == nullptr || Capsule == nullptr)
	{
		StepAsideResult = EStepAside::NoRoom;
		return;
	}
	const FVector Hinge3 = GetHingeLocation();
	const FVector From = Character->GetActorLocation();
	const FVector P = FVector(From.X - Hinge3.X, From.Y - Hinge3.Y, 0.f);    // the opener relative to the hinge axis
	const float Radius = Capsule->GetScaledCapsuleRadius();
	const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	const float Need = GetLeafReach() + Radius + StepAsideMargin;

	// The opener's side of the door plane (the leaf swings towards it). Steps never cross the plane: that would lead through the leaf.
	FVector Side = GetActorRightVector().GetSafeNormal2D();
	if (FVector::DotProduct(P, Side) < 0.f) { Side = -Side; }
	const FVector Radial = P.SizeSquared() > 1.f ? P.GetSafeNormal() : Side;

	// Candidate directions: radial (the shortest way out of the reach), straight back from the door plane (along a corridor), and the
	// directions in between and around them in 15 degree steps. For each, the shortest step that ends beyond the reach.
	// Angle around the hinge, 0 = the closed leaf, growing towards the opener's side. The leaf stands between 0 and the opener's angle, and the
	// door is ignored by the sweep below (it touches the opener), so a step must never turn towards the leaf: that could lead through it.
	const FVector LeafClosed = GetActorForwardVector().GetSafeNormal2D();
	const auto AngleOf = [&](const FVector& V) { return FMath::Atan2(FVector::DotProduct(V, Side), FVector::DotProduct(V, LeafClosed)); };
	const float FromAngle = AngleOf(P);
	struct FCandidate { FVector Dir; float Length; };
	TArray<FCandidate> Candidates;
	const auto Consider = [&](const FVector& Dir)
	{
		const FVector D = Dir.GetSafeNormal2D();
		if (D.IsNearlyZero() || FVector::DotProduct(D, Side) < 0.05f) { return; }
		for (const FCandidate& C : Candidates) { if (FVector::DotProduct(C.Dir, D) > 0.999f) { return; } }
		// |P + t D| = Need  ->  t = -(P.D) + sqrt((P.D)^2 - |P|^2 + Need^2)   (P is inside the reach, so the root is real and positive)
		const float PD = FVector::DotProduct(P, D);
		const float Disc = PD * PD - P.SizeSquared() + Need * Need;
		if (Disc < 0.f) { return; }
		const float Length = FMath::Max(-PD + FMath::Sqrt(Disc), 10.f);
		if (AngleOf(P + D * Length) < FromAngle - FMath::DegreesToRadians(0.5f)) { return; }   // towards the leaf
		Candidates.Add({D, Length});
	};
	for (int32 Deg = 0; Deg <= 90; Deg += 15)
	{
		Consider(Radial.RotateAngleAxis(static_cast<float>(Deg), FVector::UpVector));
		Consider(Radial.RotateAngleAxis(static_cast<float>(-Deg), FVector::UpVector));
		Consider(Side.RotateAngleAxis(static_cast<float>(Deg), FVector::UpVector));
		Consider(Side.RotateAngleAxis(static_cast<float>(-Deg), FVector::UpVector));
	}
	Candidates.Sort([](const FCandidate& A, const FCandidate& B) { return A.Length < B.Length; });

	// The first (shortest) candidate that is a short, natural step, lets the whole capsule pass (walls, furniture and other pawns block it;
	// the door itself is ignored, the step leads away from it) and ends on walkable floor at about the same height (no step off a porch or stair).
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnDoorStepAside), /*bTraceComplex*/ false, Character);
	Params.AddIgnoredActor(this);
	const FCollisionShape Shape = FCollisionShape::MakeCapsule(Radius - 1.f, HalfHeight - 2.f);
	const float MaxDrop = 30.f;
	int32 Tried = 0;
	FString LastBlock;
	for (const FCandidate& C : Candidates)
	{
		if (C.Length > StepAsideMaxLength)
		{
			if (LastBlock.IsEmpty()) { LastBlock = FString::Printf(TEXT("%.0f cm needed"), C.Length); }
			continue;
		}
		++Tried;
		const FVector To = From + C.Dir * C.Length;
		FHitResult Hit;
		if (GetWorld()->SweepSingleByChannel(Hit, From, To, FQuat::Identity, ECC_Pawn, Shape, Params))
		{
			LastBlock = FString::Printf(TEXT("blocked by %s"), Hit.GetActor() ? *Hit.GetActor()->GetName() : TEXT("?"));
			continue;
		}
		FHitResult Floor;
		const FVector Feet = To - FVector(0.f, 0.f, HalfHeight);
		if (!GetWorld()->LineTraceSingleByChannel(Floor, To, Feet - FVector(0.f, 0.f, MaxDrop), ECC_Visibility, Params) || Floor.ImpactNormal.Z < 0.7f)
		{
			LastBlock = TEXT("no walkable floor at the target");
			continue;
		}
		if (!Character->StartStepAside(To))
		{
			break;
		}
		StepAsideResult = EStepAside::Requested;
		LastStepAsideTarget = To;
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): the opener stands in the swing, steps back %.0f cm (%.0f deg from straight back, candidate %d of %d)"),
			*GetName(), *StableId, C.Length, FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(FVector::DotProduct(C.Dir, Side), -1.f, 1.f))), Tried, Candidates.Num());
		return;
	}
	StepAsideResult = EStepAside::NoRoom;
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s): the opener stands in the swing and has no room to step back (%d direction(s) tried, last: %s), the door waits"),
		*GetName(), *StableId, Tried, LastBlock.IsEmpty() ? TEXT("-") : *LastBlock);
}

bool ASpnTestDoor::IsSweepBlockedByPawn(float FromAngle, float ToAngle, const APawn* IgnoredPawn) const
{
	const UWorld* World = GetWorld();
	const UStaticMesh* Mesh = Panel != nullptr ? Panel->GetStaticMesh() : nullptr;
	if (World == nullptr || Mesh == nullptr || Hinge == nullptr || Root == nullptr)
	{
		return false;
	}
	const FVector HalfExtent = Mesh->GetBounds().BoxExtent * Panel->GetRelativeScale3D().GetAbs();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnDoorSweep), /*bTraceComplex*/ false, this);
	if (IgnoredPawn != nullptr)
	{
		Params.AddIgnoredActor(IgnoredPawn);
	}
	const FCollisionObjectQueryParams Pawns(ECC_Pawn);
	const float StepDegrees = 4.f;
	const int32 Steps = FMath::Max(1, FMath::CeilToInt(FMath::Abs(ToAngle - FromAngle) / StepDegrees));
	for (int32 Step = 0; Step <= Steps; ++Step)
	{
		// from the leaf position at FromAngle to the one at ToAngle, sampled so a thin pawn cannot slip between two samples
		const float Angle = FMath::Lerp(FromAngle, ToAngle, static_cast<float>(Step) / static_cast<float>(Steps));
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
		ReportOutcome(Blocked);
		return Blocked;
	}
	return RunVerb(ESpnInteractionVerb::Use, User);
}

FSpnInteractStatus ASpnTestDoor::SpnForce(AActor* User)
{
	return RunVerb(ESpnInteractionVerb::Force, User);
}

FSpnInteractStatus ASpnTestDoor::RunVerb(ESpnInteractionVerb Verb, AActor* User)
{
	USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (WorldState == nullptr)
	{
		FSpnInteractStatus Status;
		Status.Result = ESpnInteractResult::Failed;
		Status.Detail = TEXT("no world state");
		return Status;
	}

	const FString Cause = FString::Printf(TEXT("unreal.%s:%s"), Verb == ESpnInteractionVerb::Force ? TEXT("force") : TEXT("use"), *GetNameSafe(User));
	const FSpnInteractStatus Status = WorldState->TryInteract(USpnWorldStateSubsystem::TestDoorKind(), StableId, Verb,
		AuthoredDefaults(), USpnWorldStateSubsystem::DefaultHolder(), Cause);

	switch (Status.Result)
	{
	case ESpnInteractResult::Changed:
	case ESpnInteractResult::Unchanged:
		if (Status.Result == ESpnInteractResult::Changed)
		{
			// a new swing: remember who started it (only a pawn that opened it may be asked to step out of the swing)
			Opener = IsOpen() ? Cast<APawn>(User) : nullptr;
			bStepAsideAsked = false;
			StepAsideResult = EStepAside::None;
		}
		RefreshFromWorldState(/*bAnimate*/ true);
		if (Status.Result == ESpnInteractResult::Changed)
		{
			EmitDoorNoise(Verb, Status, User);
		}
		break;
	case ESpnInteractResult::Refused:
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s (%s) refused: %s"), *GetName(), *StableId, *Status.Feedback);
		break;
	default:
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s (%s): %s failed: %s %s"), *GetName(), *StableId,
			Verb == ESpnInteractionVerb::Force ? TEXT("Force") : TEXT("Use"), LexToString(Status.Result), *Status.Detail);
		break;
	}
	ReportOutcome(Status);
	return Status;
}

void ASpnTestDoor::ReportOutcome(const FSpnInteractStatus& Status) const
{
	// Player-facing message for the feedback ids; the HUD shows it (and logs it). Silent without a HUD.
	const TCHAR* Key = nullptr;
	SpnUi::ENote Kind = SpnUi::ENote::Warning;
	if (Status.Feedback == TEXT("placeholder:feedback.door.locked")) { Key = TEXT("ui.door.msg.locked"); }
	else if (Status.Feedback == TEXT("placeholder:feedback.door.unlocked")) { Key = TEXT("ui.door.msg.unlocked"); Kind = SpnUi::ENote::Good; }
	else if (Status.Feedback == TEXT("placeholder:feedback.door.broken") && Status.Result == ESpnInteractResult::Changed) { Key = TEXT("ui.door.msg.broken"); Kind = SpnUi::ENote::Good; }
	else if (Status.Feedback == TEXT("placeholder:feedback.door.disabled")) { Key = TEXT("ui.door.msg.blocked"); }
	else if (Status.Feedback == TEXT("placeholder:feedback.door.unbreakable")) { Key = TEXT("ui.door.msg.unbreakable"); }
	else if (Status.Feedback == TEXT("placeholder:feedback.door.blocked")) { Key = TEXT("ui.door.msg.obstructed"); }
	if (Key != nullptr)
	{
		SpnUi::Notify(this, FSpnUiText::Get(Key), Kind);
	}
}

FString ASpnTestDoor::SpnPromptText(const AActor* User) const
{
	if (IsBlocked())
	{
		return FSpnUiText::Get(TEXT("ui.door.blocked")).ToString();
	}
	if (IsOpen())
	{
		return FSpnUiText::Get(TEXT("ui.door.close")).ToString();
	}
	if (IsLocked())
	{
		return FSpnUiText::Get(HasRequiredKey() ? TEXT("ui.door.unlock") : TEXT("ui.door.locked")).ToString();
	}
	return FSpnUiText::Get(TEXT("ui.door.open")).ToString();
}

FString ASpnTestDoor::SpnForcePromptText(const AActor* User) const
{
	// Offered only where Force can succeed: a closed, locked, breakable, not blocked door.
	return bBreakable && !IsBlocked() && !IsOpen() && IsLocked() ? FSpnUiText::Get(TEXT("ui.door.force")).ToString() : FString();
}

bool ASpnTestDoor::IsBroken() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	const TOptional<bool> Fact = WorldState != nullptr ? WorldState->ReadFlag(USpnWorldStateSubsystem::TestDoorBrokenKey(StableId)) : TOptional<bool>();
	return Fact.Get(false);
}

bool ASpnTestDoor::IsBlocked() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	const TOptional<bool> Fact = WorldState != nullptr ? WorldState->ReadFlag(USpnWorldStateSubsystem::TestDoorDisabledKey(StableId)) : TOptional<bool>();
	return Fact.Get(bAuthoredBlocked);
}

bool ASpnTestDoor::HasRequiredKey() const
{
	const USpnWorldStateSubsystem* WorldState = GetWorldState();
	if (RequiredKeyId.IsEmpty() || WorldState == nullptr)
	{
		return false;
	}
	return WorldState->ReadFlag(USpnWorldStateSubsystem::TestKeyHeldKey(RequiredKeyId)).Get(false);
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

FSpnInteractDefaults ASpnTestDoor::AuthoredDefaults() const
{
	FSpnInteractDefaults Defaults;
	Defaults.Bools = {{OpenState, bAuthoredOpen}, {LockedState, bAuthoredLocked}, {TEXT("disabled"), bAuthoredBlocked}, {TEXT("breakable"), bBreakable}};
	if (!RequiredKeyId.IsEmpty())
	{
		Defaults.Strings.Add(TEXT("key"), RequiredKeyId);
	}
	return Defaults;
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
	// A broken door hangs off its hinge: the free edge sags (placeholder look, cosmetic only).
	Hinge->SetRelativeRotation(FRotator(IsBroken() ? -7.f : 0.f, CurrentAngle + StrainJitter, 0.f));
}

float ASpnTestDoor::SpnForceDuration() const
{
	return BreakSeconds;
}

void ASpnTestDoor::SpnForceStrain(float Progress01)
{
	// The door shudders while it is being forced: a small yaw jitter that grows with the progress (cosmetic; the state does not change until the end).
	const float Phase = GetWorld() != nullptr ? GetWorld()->GetTimeSeconds() * 50.f : 0.f;
	StrainJitter = Progress01 > 0.f ? FMath::Sin(Phase) * (0.6f + 1.8f * Progress01) : 0.f;
	SetCurrentAngle(CurrentAngle);
}

void ASpnTestDoor::EmitDoorNoise(ESpnInteractionVerb Verb, const FSpnInteractStatus& Status, const AActor* User) const
{
	const UGameInstance* GameInstance = GetGameInstance();
	USpnNoiseSubsystem* Noise = GameInstance != nullptr ? GameInstance->GetSubsystem<USpnNoiseSubsystem>() : nullptr;
	if (Noise == nullptr)
	{
		return;
	}
	FSpnNoise N;
	N.LocationCm = Panel != nullptr ? Panel->GetComponentLocation() : GetActorLocation();
	N.ObjectId = FString::Printf(TEXT("placeholder.door.%s"), *StableId);
	N.SourceId = GetNameSafe(User);
	if (Verb == ESpnInteractionVerb::Force)
	{
		// Breaking a door open is loud and suspicious (placeholder values: 20 m, full strength)
		N.Kind = ESpnNoiseKind::DoorBreak;
		N.RangeCm = DoorBreakNoiseRangeCm;
		N.Strength = 1.f;
	}
	else if (Status.Feedback != TEXT("placeholder:feedback.door.unlocked"))
	{
		// opening or closing a door: audible close by, harmless (placeholder values: 6 m, weak)
		N.Kind = ESpnNoiseKind::DoorUse;
		N.RangeCm = DoorUseNoiseRangeCm;
		N.Strength = 0.35f;
	}
	else
	{
		return;
	}
	Noise->EmitNoise(N);
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
