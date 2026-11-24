#include "Spn/SpnNpc.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Spn/SpnTestDoor.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	constexpr float EyeHeightCm = 70.f;
	constexpr float SenseInterval = 0.1f;
	constexpr float WarinessInterval = 0.5f;
	constexpr double SightingMemoryCooldown = 10.0;
	constexpr int32 MaxLogEntries = 256;
	constexpr float RoutineArrivalCm = 90.f;      // "arrived" for the patrol; larger than the path-following acceptance so a stop short of the point still counts
	constexpr float MoveAcceptanceCm = 35.f;
	constexpr float RoutineStuckSeconds = 8.f;   // a patrol leg that makes no progress for this long is given up (next point)

	FLinearColor StateColor(ESpnReactionState State)
	{
		switch (State)
		{
		case ESpnReactionState::Idle: return FLinearColor(0.15f, 0.55f, 0.25f);
		case ESpnReactionState::Investigate: return FLinearColor(0.95f, 0.8f, 0.1f);
		case ESpnReactionState::Alert: return FLinearColor(1.f, 0.4f, 0.05f);
		case ESpnReactionState::Flee: return FLinearColor(0.9f, 0.1f, 0.1f);
		case ESpnReactionState::ReturnToRoutine: return FLinearColor(0.2f, 0.45f, 0.95f);
		}
		return FLinearColor::White;
	}

	float Dist2D(const FVector& A, const FVector& B)
	{
		return FVector::Dist2D(A, B);
	}
}

ASpnNpc::ASpnNpc()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	bUseControllerRotationYaw = false;
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 420.f, 0.f);
	Move->MaxWalkSpeed = WalkSpeedCm;
	Move->bUseRVOAvoidance = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetCapsuleComponent());
	Body->SetRelativeLocation(FVector(0.f, 0.f, -14.f));
	Body->SetRelativeScale3D(FVector(0.55f, 0.55f, 1.45f));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetGenerateOverlapEvents(false);
	if (Cylinder.Succeeded())
	{
		Body->SetStaticMesh(Cylinder.Object);
	}

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetCapsuleComponent());
	Head->SetRelativeLocation(FVector(0.f, 0.f, 76.f));
	Head->SetRelativeScale3D(FVector(0.32f));
	Head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Head->SetGenerateOverlapEvents(false);
	if (Sphere.Succeeded())
	{
		Head->SetStaticMesh(Sphere.Object);
	}

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(GetCapsuleComponent());
	Label->SetRelativeLocation(FVector(0.f, 0.f, 135.f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(16.f);
	Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

USpnNpcMemorySubsystem* ASpnNpc::GetMemory() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	return GameInstance != nullptr ? GameInstance->GetSubsystem<USpnNpcMemorySubsystem>() : nullptr;
}

USpnNoiseSubsystem* ASpnNpc::GetNoise() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	return GameInstance != nullptr ? GameInstance->GetSubsystem<USpnNoiseSubsystem>() : nullptr;
}

APawn* ASpnNpc::ResolveTarget() const
{
	return PerceptionTarget.IsValid() ? PerceptionTarget.Get() : UGameplayStatics::GetPlayerPawn(this, 0);
}

bool ASpnNpc::HasNavigationMesh() const
{
	const UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	return Nav != nullptr && Nav->GetDefaultNavDataInstance() != nullptr;
}

ESpnReactionState ASpnNpc::GetReactionState() const
{
	return Brain.IsValid() ? Brain->GetState() : ESpnReactionState::Idle;
}

uint64 ASpnNpc::GetTransitionCount() const
{
	return Brain.IsValid() ? Brain->GetTransitionCount() : 0;
}

void ASpnNpc::BeginPlay()
{
	Super::BeginPlay();

	Brain = MakeUnique<FSpnReactionBrain>();
	SpawnCm = GetActorLocation();
	HomeCm = RoutinePoints.Num() > 0 ? RoutinePoints[0] : SpawnCm;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedCm;
	// the engine cylinder has no colour parameter: the dynamic instance is made from the engine basic-shape material (parameter "Color")
	if (UMaterialInterface* Base = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
	{
		BodyMaterial = UMaterialInstanceDynamic::Create(Base, this);
		Body->SetMaterial(0, BodyMaterial);
		if (UMaterialInstanceDynamic* Skin = UMaterialInstanceDynamic::Create(Base, this))
		{
			Skin->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.78f, 0.6f, 0.48f));
			Head->SetMaterial(0, Skin);
		}
	}
	Label->SetVisibility(bShowDebugLabel);

	if (NpcId <= 1 || NpcId >= (1ll << 62))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("%s: NpcId %lld is not usable (needs 1 < id < 2^62); this NPC will not remember anything"), *GetName(), NpcId);
	}
	if (USpnNpcMemorySubsystem* Memory = GetMemory())
	{
		Memory->RegisterNpc(NpcId, Tier, DisplayName);
		ReplacedHandle = Memory->OnReplaced.AddUObject(this, &ASpnNpc::HandleMemoryReplaced);
	}
	if (USpnNoiseSubsystem* Noise = GetNoise())
	{
		NoiseHandle = Noise->OnNoise.AddUObject(this, &ASpnNpc::HandleNoise);
	}
	RestoreFromMemory();
	UE_LOG(LogSupernaturalProt, Log, TEXT("NPC %s (id %lld) ready at %s, %d routine point(s), navigation mesh: %s, state %s%s"), *DisplayName, NpcId, *SpawnCm.ToCompactString(),
		RoutinePoints.Num(), HasNavigationMesh() ? TEXT("yes") : TEXT("NO (straight-line fallback)"), LexToString(GetReactionState()), Wariness.bWary ? TEXT(", wary") : TEXT(""));
}

void ASpnNpc::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (USpnNpcMemorySubsystem* Memory = GetMemory())
	{
		Memory->OnReplaced.Remove(ReplacedHandle);
	}
	if (USpnNoiseSubsystem* Noise = GetNoise())
	{
		Noise->OnNoise.Remove(NoiseHandle);
	}
	ReplacedHandle.Reset();
	NoiseHandle.Reset();
	Brain.Reset();
	Super::EndPlay(EndPlayReason);
}

void ASpnNpc::HandleMemoryReplaced()
{
	// A load swapped the memory: make sure this NPC exists in it again (an older save may not know it) and re-derive the volatile state.
	if (USpnNpcMemorySubsystem* Memory = GetMemory())
	{
		Memory->RegisterNpc(NpcId, Tier, DisplayName);
	}
	RestoreFromMemory();
}

void ASpnNpc::RestoreFromMemory()
{
	USpnNpcMemorySubsystem* Memory = GetMemory();
	if (!Brain.IsValid() || Memory == nullptr)
	{
		return;
	}
	RefreshWariness();
	ESpnReactionState State;
	FVector TargetCm;
	Memory->GetInitialReaction(NpcId, State, TargetCm);
	const ESpnReactionState Before = Brain->GetState();
	Brain->Restore(State, TargetCm);
	bHasPendingNoise = false;
	bHasMoveGoal = false;
	StopMoving();
	if (State != Before || State != ESpnReactionState::Idle)
	{
		ReactionLog.Add({Memory->GetWorldTime(), Before, State, TEXT("restored from memory")});
		UE_LOG(LogSupernaturalProt, Log, TEXT("NPC %s: reaction restored from memory: %s -> %s (wary %d, recent alarm %d)"), *DisplayName, LexToString(Before), LexToString(State),
			Wariness.bWary, Wariness.bRecentAlarm);
	}
	UpdateVisuals();
}

void ASpnNpc::RefreshWariness()
{
	if (const USpnNpcMemorySubsystem* Memory = GetMemory())
	{
		Wariness = Memory->Assess(NpcId);
	}
	WarinessTimer = 0.f;
}

FVector ASpnNpc::GetEarLocation() const
{
	return GetActorLocation() + FVector(0.f, 0.f, EyeHeightCm);
}

int32 ASpnNpc::CountWallsBetween(const FVector& From, const FVector& To, const FString& IgnoreDoorObjectId, TArray<FString>* OutObstacles) const
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return 0;
	}
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnNpcWalls), /*bTraceComplex*/ false, this);
	if (const APawn* Player = ResolveTarget())
	{
		Params.AddIgnoredActor(Player);
	}
	// A multi-trace stops at the first BLOCKING hit, so the walls are counted one trace at a time: every blocking actor is counted once, then ignored,
	// and the trace continues from the same start (at most 16 obstacles). The door that made the noise is the source, not an obstacle; an open
	// door is an opening.
	int32 Walls = 0;
	for (int32 Step = 0; Step < 16; ++Step)
	{
		FHitResult Hit;
		if (!World->LineTraceSingleByChannel(Hit, From, To, ECC_Visibility, Params) || Hit.GetActor() == nullptr)
		{
			break;
		}
		const AActor* Actor = Hit.GetActor();
		Params.AddIgnoredActor(Actor);
		if (const ASpnTestDoor* Door = Cast<ASpnTestDoor>(Actor))
		{
			if (!IgnoreDoorObjectId.IsEmpty() && IgnoreDoorObjectId == FString::Printf(TEXT("placeholder.door.%s"), *Door->StableId))
			{
				continue;
			}
			// An OPEN door is an opening, not a wall: its leaf stands in the room, and a sound line that only grazes it is not damped by a wall.
			// Counting it made hearing depend on a few cm of the listener's/speaker's position (SpnQA.NpcReaction neighbour checks, 2026-11-13).
			// A closed door between two rooms still counts as one wall.
			if (Door->IsOpen())
			{
				continue;
			}
		}
		++Walls;
		if (OutObstacles != nullptr)
		{
			OutObstacles->Add(FString::Printf(TEXT("%s [%s] at %.0f,%.0f,%.0f"), *Actor->GetActorNameOrLabel(), Hit.GetComponent() ? *Hit.GetComponent()->GetName() : TEXT("?"),
				Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z));
		}
	}
	return Walls;
}

void ASpnNpc::HandleNoise(const FSpnNoise& Noise)
{
	if (!Brain.IsValid() || Noise.SourceId == GetName() || Noise.Sequence == LastHandledNoiseSequence)
	{
		return; // own shout, or the same event twice
	}
	LastHandledNoiseSequence = Noise.Sequence;

	TArray<FString> Obstacles;
	const int32 Walls = CountWallsBetween(Noise.LocationCm, GetEarLocation(), Noise.ObjectId, &Obstacles);
	float Distance = 0.f, EffectiveRange = 0.f;
	if (!SpnNpcSense::CanHear(Noise, GetActorLocation(), Walls, HearingSensitivity, WallAttenuation, Distance, EffectiveRange))
	{
		++NoisesNotHeard;
		UE_LOG(LogSupernaturalProt, Log, TEXT("NPC %s: did not hear %s at %.1f m (%d wall(s), range %.1f m) from %.0f,%.0f,%.0f; obstacles: %s"), *DisplayName, LexToString(Noise.Kind),
			Distance / 100.f, Walls, EffectiveRange / 100.f, Noise.LocationCm.X, Noise.LocationCm.Y, Noise.LocationCm.Z, *FString::Join(Obstacles, TEXT("; ")));
		return;
	}
	++NoisesHeard;
	const bool bSuspicious = SpnNpcSense::IsSuspicious(Noise);
	if (!bSuspicious && !Wariness.bWary)
	{
		++NoisesIgnored; // harmless and the NPC has no reason to care: the brain would ignore it too, and nothing is worth remembering
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("NPC %s: heard %s %.1f m away (%d wall(s)), %s"), *DisplayName, LexToString(Noise.Kind), Distance / 100.f, Walls,
		bSuspicious ? TEXT("suspicious") : (Wariness.bWary ? TEXT("harmless, but wary") : TEXT("harmless")));

	if (bSuspicious)
	{
		if (USpnNpcMemorySubsystem* Memory = GetMemory())
		{
			FSpnIncident Incident;
			Incident.Kind = Noise.Kind == ESpnNoiseKind::DoorBreak ? ESpnIncidentKind::HeardDoorBroken : ESpnIncidentKind::HeardSuspicious;
			Incident.LocationCm = Noise.LocationCm;
			Incident.Severity = Noise.Strength;
			Incident.ObjectId = Noise.ObjectId;
			Incident.Accuracy = FMath::Clamp(1.f - 0.5f * Distance / FMath::Max(EffectiveRange, 1.f), 0.4f, 1.f) * (Walls > 0 ? 0.85f : 1.f);
			Memory->Remember(NpcId, Incident);
			RefreshWariness();
		}
	}
	if (bSuspicious || Wariness.bWary)
	{
		if (!bHasPendingNoise || bSuspicious)
		{
			PendingNoise = Noise;
			bHasPendingNoise = true;
		}
	}
}

bool ASpnNpc::ComputeSeesPlayer(FVector& OutPlayerCm) const
{
	const UWorld* World = GetWorld();
	const APawn* Player = ResolveTarget();
	if (World == nullptr || Player == nullptr)
	{
		return false;
	}
	const FVector Eye = GetActorLocation() + FVector(0.f, 0.f, EyeHeightCm);
	const float Yaw = GetActorRotation().Yaw;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnNpcSight), /*bTraceComplex*/ false, this);
	Params.AddIgnoredActor(Player);
	// head, chest, feet: any clear line is enough (a table between the NPC and the player's legs does not hide the player)
	for (const float Offset : {55.f, 0.f, -55.f})
	{
		const FVector Target = Player->GetActorLocation() + FVector(0.f, 0.f, Offset);
		FHitResult Hit;
		const bool bClear = !World->LineTraceSingleByChannel(Hit, Eye, Target, ECC_Visibility, Params);
		if (SpnNpcSense::CanSee(Eye, Yaw, Target, bClear, SightRangeCm, FieldOfViewDegrees, CloseSenseRangeCm))
		{
			OutPlayerCm = Player->GetActorLocation();
			return true;
		}
	}
	return false;
}

void ASpnNpc::SenseNow()
{
	SenseTimer = 0.f;
	FVector Player;
	const bool bSees = ComputeSeesPlayer(Player);
	if (bSees)
	{
		LastPlayerCm = Player;
		if (!bSeesPlayer)
		{
			RecordSighting();
		}
	}
	bSeesPlayer = bSees;
}

void ASpnNpc::RecordSighting()
{
	USpnNpcMemorySubsystem* Memory = GetMemory();
	if (Memory == nullptr || Memory->GetWorldTime() - LastSightingRecordTime < SightingMemoryCooldown)
	{
		return;
	}
	LastSightingRecordTime = Memory->GetWorldTime();
	FSpnIncident Incident;
	Incident.Kind = ESpnIncidentKind::SawPlayer;
	Incident.LocationCm = LastPlayerCm;
	Incident.Severity = 0.3f;
	Memory->Remember(NpcId, Incident);
	RefreshWariness();
}

void ASpnNpc::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	USpnNpcMemorySubsystem* Memory = GetMemory();
	if (!Brain.IsValid() || Memory == nullptr)
	{
		return;
	}

	SenseTimer += DeltaSeconds;
	if (SenseTimer >= SenseInterval)
	{
		SenseNow();
	}
	WarinessTimer += DeltaSeconds;
	if (WarinessTimer >= WarinessInterval)
	{
		RefreshWariness();
	}

	FSpnReactionInput In;
	In.DeltaSeconds = DeltaSeconds;
	In.SelfCm = GetActorLocation();
	In.HomeCm = HomeCm;
	In.bSeesPlayer = bSeesPlayer;
	In.PlayerCm = LastPlayerCm;
	In.bWary = Wariness.bWary;
	if (bHasPendingNoise)
	{
		In.bHasNoise = true;
		In.NoiseCm = PendingNoise.LocationCm;
		In.NoiseSeverity = PendingNoise.Strength;
		In.bNoiseSuspicious = SpnNpcSense::IsSuspicious(PendingNoise);
	}
	const FSpnReactionOutput Out = Brain->Update(In);
	const bool bNoiseUsed = In.bHasNoise;
	bHasPendingNoise = false;

	if (Out.bChanged)
	{
		FString Why = Out.bRaiseAlarm ? TEXT("sees the player: alarm") : (bNoiseUsed ? FString::Printf(TEXT("noise: %s"), LexToString(PendingNoise.Kind)) : (bSeesPlayer ? TEXT("sees the player") : TEXT("timer / arrival")));
		ReactionLog.Add({Memory->GetWorldTime(), Out.Previous, Out.State, Why});
		if (ReactionLog.Num() > MaxLogEntries)
		{
			ReactionLog.RemoveAt(0, ReactionLog.Num() - MaxLogEntries);
		}
		UE_LOG(LogSupernaturalProt, Log, TEXT("NPC %s: %s -> %s (%s)"), *DisplayName, LexToString(Out.Previous), LexToString(Out.State), *Why);
		bHasMoveGoal = false;
		FleeRepickTimer = 0.f;
		if (Out.State == ESpnReactionState::Idle)
		{
			bRoutineMoving = false;
			RoutineWait = 0.5f;
		}
	}
	if (Out.bRaiseAlarm)
	{
		++AlarmsRaised;
		FSpnIncident Incident;
		Incident.Kind = ESpnIncidentKind::Alarmed;
		Incident.LocationCm = LastPlayerCm;
		Incident.Severity = 0.8f;
		Memory->Remember(NpcId, Incident);
		RefreshWariness();
		if (USpnNoiseSubsystem* Noise = GetNoise())
		{
			FSpnNoise Shout;
			Shout.LocationCm = GetActorLocation() + FVector(0.f, 0.f, EyeHeightCm);
			Shout.RangeCm = 2500.f;
			Shout.Strength = 0.9f;
			Shout.Kind = ESpnNoiseKind::Shout;
			Shout.SourceId = GetName();
			Noise->EmitNoise(Shout);
		}
	}

	Apply(Out, DeltaSeconds);
	UpdateVisuals();
}

void ASpnNpc::Apply(const FSpnReactionOutput& Out, float DeltaSeconds)
{
	UCharacterMovementComponent* Move = GetCharacterMovement();
	switch (Out.State)
	{
	case ESpnReactionState::Idle:
		Move->MaxWalkSpeed = WalkSpeedCm;
		RunRoutine(DeltaSeconds);
		break;
	case ESpnReactionState::Investigate:
		MoveTowards(Out.GoalCm, false);
		break;
	case ESpnReactionState::ReturnToRoutine:
		MoveTowards(HomeCm, false);
		break;
	case ESpnReactionState::Alert:
	{
		StopMoving();
		const FVector To = Out.ThreatCm - GetActorLocation();
		if (!To.IsNearlyZero())
		{
			const FRotator Want(0.f, To.Rotation().Yaw, 0.f);
			SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), Want, DeltaSeconds, 360.f));
		}
		break;
	}
	case ESpnReactionState::Flee:
	{
		FleeRepickTimer -= DeltaSeconds;
		if (FleeRepickTimer <= 0.f || Dist2D(GetActorLocation(), FleeGoalCm) < 120.f)
		{
			FleeGoalCm = PickFleePoint(Out.ThreatCm);
			FleeRepickTimer = 2.f;
			bHasMoveGoal = false;
		}
		MoveTowards(FleeGoalCm, true);
		break;
	}
	}
}

void ASpnNpc::StopMoving()
{
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		if (AI->GetMoveStatus() != EPathFollowingStatus::Idle)
		{
			AI->StopMovement();
		}
	}
	bHasMoveGoal = false;
}

void ASpnNpc::MoveTowards(const FVector& GoalCm, bool bSprint)
{
	GetCharacterMovement()->MaxWalkSpeed = bSprint ? FleeSpeedCm : WalkSpeedCm;
	AAIController* AI = Cast<AAIController>(GetController());
	if (AI == nullptr)
	{
		return;
	}
	if (Dist2D(GetActorLocation(), GoalCm) <= RoutineArrivalCm * 0.7f)
	{
		return;
	}
	const bool bNewGoal = !bHasMoveGoal || Dist2D(GoalCm, MoveGoalCm) > 60.f;
	const bool bIdleAgain = !bFallbackSteering && AI->GetMoveStatus() == EPathFollowingStatus::Idle;
	if (bNewGoal || bIdleAgain)
	{
		MoveGoalCm = GoalCm;
		bHasMoveGoal = true;
		bFallbackSteering = false;
		const EPathFollowingRequestResult::Type Result = AI->MoveToLocation(GoalCm, MoveAcceptanceCm, /*bStopOnOverlap*/ false, /*bUsePathfinding*/ true,
			/*bProjectDestinationToNavigation*/ true, /*bCanStrafe*/ false, nullptr, /*bAllowPartialPath*/ true);
		if (Result == EPathFollowingRequestResult::Failed)
		{
			bFallbackSteering = true;
			UE_LOG(LogSupernaturalProt, Verbose, TEXT("NPC %s: no navigation path to %s, steering straight at it"), *DisplayName, *GoalCm.ToCompactString());
		}
	}
	if (bFallbackSteering)
	{
		const FVector To = (GoalCm - GetActorLocation()).GetSafeNormal2D();
		AddMovementInput(To, 1.f);
	}
}

FVector ASpnNpc::PickFleePoint(const FVector& ThreatCm) const
{
	TArray<FVector> Candidates;
	Candidates.Add(HomeCm);
	Candidates.Add(SpawnCm);
	Candidates.Append(RoutinePoints);
	if (UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		for (int32 Index = 0; Index < 8; ++Index)
		{
			FNavLocation Point;
			if (Nav->GetRandomReachablePointInRadius(GetActorLocation(), 2000.f, Point))
			{
				Candidates.Add(Point.Location);
			}
		}
	}
	FVector Best = GetActorLocation() + (GetActorLocation() - ThreatCm).GetSafeNormal2D() * 800.f;
	float BestScore = Dist2D(Best, ThreatCm) - 100000.f; // the straight-away point is the last resort
	for (const FVector& Candidate : Candidates)
	{
		if (Dist2D(Candidate, GetActorLocation()) < 300.f)
		{
			continue;
		}
		const float Score = Dist2D(Candidate, ThreatCm);
		if (Score > BestScore)
		{
			BestScore = Score;
			Best = Candidate;
		}
	}
	return Best;
}

void ASpnNpc::RunRoutine(float DeltaSeconds)
{
	if (RoutinePoints.Num() == 0)
	{
		return;
	}
	if (bRoutineMoving)
	{
		const FVector& Goal = RoutinePoints[RoutineIndex % RoutinePoints.Num()];
		// RoutineLegTime counts the time WITHOUT progress: it restarts whenever the NPC got 25 cm closer than ever on this leg. (Until
		// 2026-11-13 it was the whole leg time, so every leg longer than 8 s x walk speed = 12 m was given up as "no progress" while the
		// NPC was walking; the test maps only had shorter legs. Found on the 40 m legs of SpnWorld.)
		const float GoalDistance = Dist2D(GetActorLocation(), Goal);
		if (GoalDistance < RoutineBestDistance - 25.f)
		{
			RoutineBestDistance = GoalDistance;
			RoutineLegTime = 0.f;
		}
		RoutineLegTime += DeltaSeconds;
		if (GoalDistance <= RoutineArrivalCm || RoutineLegTime > RoutineStuckSeconds)
		{
			++(RoutineLegTime > RoutineStuckSeconds ? PatrolLegsGivenUp : PatrolLegsCompleted);
			if (RoutineLegTime > RoutineStuckSeconds)
			{
				UE_LOG(LogSupernaturalProt, Warning, TEXT("NPC %s: patrol leg to %s made no progress for %.0f s, moving on"), *DisplayName, *Goal.ToCompactString(), RoutineLegTime);
			}
			RoutineLegTime = 0.f;
			bRoutineMoving = false;
			RoutineWait = RoutineWaitSeconds;
			RoutineIndex = (RoutineIndex + 1) % RoutinePoints.Num();
			StopMoving();
		}
		else
		{
			MoveTowards(Goal, false);
		}
		return;
	}
	RoutineWait -= DeltaSeconds;
	if (RoutineWait <= 0.f)
	{
		RoutineLegTime = 0.f;
		RoutineBestDistance = TNumericLimits<float>::Max();
		bRoutineMoving = true;
	}
}

void ASpnNpc::UpdateVisuals()
{
	const ESpnReactionState State = GetReactionState();
	if (BodyMaterial != nullptr)
	{
		BodyMaterial->SetVectorParameterValue(TEXT("Color"), StateColor(State));
	}
	if (Label != nullptr && bShowDebugLabel)
	{
		if (!bLabelShown || State != LastShownState)
		{
			LastShownState = State;
			bLabelShown = true;
			Label->SetText(FText::FromString(FString::Printf(TEXT("%s\n%s%s"), *DisplayName, LexToString(State), Wariness.bWary ? TEXT(" *") : TEXT(""))));
			Label->SetTextRenderColor(StateColor(State).ToFColor(true));
		}
		if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (const APlayerCameraManager* Camera = PC->PlayerCameraManager)
			{
				const FVector ToCamera = Camera->GetCameraLocation() - Label->GetComponentLocation();
				Label->SetWorldRotation(FRotator(0.f, ToCamera.Rotation().Yaw, 0.f));
			}
		}
	}
}
