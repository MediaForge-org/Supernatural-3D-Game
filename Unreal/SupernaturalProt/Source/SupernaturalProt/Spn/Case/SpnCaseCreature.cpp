#include "Spn/Case/SpnCaseCreature.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Navigation/PathFollowingComponent.h"
#include "Spn/Case/SpnCaseSubsystem.h"
#include "Spn/SpnTestDoor.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

ASpnCreature::ASpnCreature()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	bUseControllerRotationYaw = false;
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 360.f, 0.f);
	Move->MaxWalkSpeed = ProwlSpeedCm;
	Move->bUseRVOAvoidance = false;
	GetCapsuleComponent()->InitCapsuleSize(40.f, 105.f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetCapsuleComponent());
	Body->SetRelativeLocation(FVector(0.f, 0.f, -10.f));
	Body->SetRelativeScale3D(FVector(0.6f, 0.45f, 1.9f));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetGenerateOverlapEvents(false);
	if (Cylinder.Succeeded())
	{
		Body->SetStaticMesh(Cylinder.Object);
	}

	// an elongated, eyeless head: the placeholder says "blind" without any art
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetCapsuleComponent());
	Head->SetRelativeLocation(FVector(18.f, 0.f, 100.f));
	Head->SetRelativeScale3D(FVector(0.55f, 0.3f, 0.3f));
	Head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Head->SetGenerateOverlapEvents(false);
	if (Sphere.Succeeded())
	{
		Head->SetStaticMesh(Sphere.Object);
	}

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(GetCapsuleComponent());
	Label->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(16.f);
	Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpnCreature::BeginPlay()
{
	Super::BeginPlay();
	SpawnCm = GetActorLocation();
	SpawnRotation = GetActorRotation();
	Random.Initialize(static_cast<int32>(GetTypeHash(GetName())));
	if (UMaterialInterface* Base = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
	{
		BodyMaterial = UMaterialInstanceDynamic::Create(Base, this);
		Body->SetMaterial(0, BodyMaterial);
		Head->SetMaterial(0, BodyMaterial);
	}
	Label->SetVisibility(bShowDebugLabel);

	USpnCaseSubsystem* Case = GetWorld() != nullptr ? GetWorld()->GetSubsystem<USpnCaseSubsystem>() : nullptr;
	if (Case != nullptr)
	{
		Case->RegisterCreature(this); // applies the active difficulty
	}
	if (!Brain.IsValid())
	{
		Brain = MakeUnique<FSpnCreatureBrain>(Tuning);
	}
	if (UGameInstance* Instance = GetGameInstance())
	{
		if (USpnNoiseSubsystem* Noise = Instance->GetSubsystem<USpnNoiseSubsystem>())
		{
			NoiseHandle = Noise->OnNoise.AddUObject(this, &ASpnCreature::HandleNoise);
		}
	}
	UpdateVisuals();
	UE_LOG(LogSupernaturalProt, Log, TEXT("Creature %s ready at %s, %d patrol point(s), reaction delay %.1f s, search %.0f s"), *GetName(), *SpawnCm.ToCompactString(),
		PatrolPoints.Num(), Tuning.ReactionDelaySeconds, Tuning.SearchSeconds);
}

void ASpnCreature::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGameInstance* Instance = GetGameInstance())
	{
		if (USpnNoiseSubsystem* Noise = Instance->GetSubsystem<USpnNoiseSubsystem>())
		{
			Noise->OnNoise.Remove(NoiseHandle);
		}
	}
	if (UWorld* World = GetWorld())
	{
		if (USpnCaseSubsystem* Case = World->GetSubsystem<USpnCaseSubsystem>())
		{
			Case->UnregisterCreature(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ASpnCreature::ApplyDifficulty(const FSpnDifficultyProfile& Profile)
{
	Tuning.ReactionDelaySeconds = Profile.CreatureReactionDelay;
	Tuning.SearchSeconds = Profile.CreatureSearchSeconds;
	HearingSensitivity = FMath::Max(0.1f, Profile.CreatureHearingScale);
	FString Error;
	Brain = MakeUnique<FSpnCreatureBrain>(Tuning, &Error);
	if (!Error.IsEmpty())
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("Creature %s: tuning refused (%s), Core defaults used"), *GetName(), *Error);
	}
}

ESpnCreatureState ASpnCreature::GetCreatureState() const
{
	return Brain.IsValid() ? Brain->GetState() : ESpnCreatureState::Patrol;
}

bool ASpnCreature::IsWary() const
{
	return Brain.IsValid() && Brain->IsWary();
}

uint64 ASpnCreature::GetTransitionCount() const
{
	return Brain.IsValid() ? Brain->GetTransitionCount() : 0;
}

APawn* ASpnCreature::ResolvePrey() const
{
	if (PreyOverride.IsValid())
	{
		return PreyOverride.Get();
	}
	const USpnCaseSubsystem* Case = GetWorld() != nullptr ? GetWorld()->GetSubsystem<USpnCaseSubsystem>() : nullptr;
	return Case != nullptr ? Case->GetPlayerPawn() : nullptr;
}

FVector ASpnCreature::GetEarLocation() const
{
	return GetActorLocation() + FVector(0.f, 0.f, 80.f);
}

int32 ASpnCreature::CountWallsBetween(const FVector& From, const FVector& To) const
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return 0;
	}
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnCreatureWalls), /*bTraceComplex*/ false, this);
	if (const APawn* Prey = ResolvePrey())
	{
		Params.AddIgnoredActor(Prey);
	}
	// one blocking actor per trace, then ignored and traced again (as ASpnNpc::CountWallsBetween); an open door is an opening
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
			if (Door->IsOpen())
			{
				continue;
			}
		}
		if (Cast<APawn>(Actor) != nullptr)
		{
			continue; // other creatures / NPCs do not damp sound
		}
		++Walls;
	}
	return Walls;
}

void ASpnCreature::HandleNoise(const FSpnNoise& Noise)
{
	if (bFrozen || !Brain.IsValid() || Noise.Sequence == LastNoiseSequence || Noise.SourceId == GetName())
	{
		return;
	}
	LastNoiseSequence = Noise.Sequence;
	const float Scale = HearingSensitivity * (Brain->IsWary() ? Brain->GetHearingScale() : 1.f);
	if (FVector::Dist2D(Noise.LocationCm, GetActorLocation()) > Noise.RangeCm * Scale)
	{
		++NoisesNotHeard;
		return;
	}
	const int32 Walls = CountWallsBetween(Noise.LocationCm + FVector(0.f, 0.f, 50.f), GetEarLocation());
	float Distance = 0.f, Effective = 0.f;
	if (!SpnNpcSense::CanHear(Noise, GetActorLocation(), Walls, Scale, WallAttenuation, Distance, Effective))
	{
		++NoisesNotHeard;
		return;
	}
	++NoisesHeard;
	// the loudest noise of the tick wins
	if (!bHasPendingNoise || Noise.Strength > PendingStrength)
	{
		bHasPendingNoise = true;
		PendingNoiseCm = Noise.LocationCm;
		PendingStrength = Noise.Strength;
		PendingDistanceCm = Distance;
	}
	UE_LOG(LogSupernaturalProt, Verbose, TEXT("Creature %s heard %s %.1f m away (%d wall(s))"), *GetName(), LexToString(Noise.Kind), Distance / 100.f, Walls);
}

bool ASpnCreature::StunByBell()
{
	if (!Brain.IsValid())
	{
		return false;
	}
	const ESpnCreatureState Before = Brain->GetState();
	Brain->Stun();
	StopMoving();
	if (Before != ESpnCreatureState::Stunned)
	{
		Log.Add({GetWorld() != nullptr ? GetWorld()->GetTimeSeconds() : 0.0, Before, ESpnCreatureState::Stunned});
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("Creature %s stunned by the bell (was %s)"), *GetName(), LexToString(Before));
	UpdateVisuals();
	return true;
}

void ASpnCreature::ResetToSpawn()
{
	StopMoving();
	SetActorLocationAndRotation(SpawnCm, SpawnRotation, false, nullptr, ETeleportType::TeleportPhysics);
	GetCharacterMovement()->StopMovementImmediately();
	if (Brain.IsValid())
	{
		Brain->Reset();
	}
	bHasPendingNoise = false;
	bListening = false;
	bHasSearchPoint = false;
	PatrolIndex = 0;
	PatrolWait = 0.f;
	UpdateVisuals();
}

void ASpnCreature::SetFrozen(bool bInFrozen)
{
	bFrozen = bInFrozen;
	if (bFrozen)
	{
		StopMoving();
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void ASpnCreature::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bFrozen || !Brain.IsValid())
	{
		bHasPendingNoise = false;
		return;
	}
	if (!bNotedLevelStart)
	{
		// the first creature tick is at the level start: where the player stands now is the respawn point when no save exists
		if (USpnCaseSubsystem* Case = GetWorld()->GetSubsystem<USpnCaseSubsystem>())
		{
			Case->CaptureStartIfNeeded();
			bNotedLevelStart = Case->HasLevelStart();
		}
	}
	FSpnCreatureInput In;
	In.DeltaSeconds = DeltaSeconds;
	In.SelfCm = GetActorLocation();
	if (bHasPendingNoise)
	{
		In.bHasNoise = true;
		In.NoiseCm = PendingNoiseCm;
		In.NoiseStrength = PendingStrength;
		In.NoiseDistanceCm = PendingDistanceCm;
		bHasPendingNoise = false;
	}
	if (const APawn* Prey = ResolvePrey())
	{
		const float Dist = FVector::Dist2D(Prey->GetActorLocation(), GetActorLocation());
		const float DZ = FMath::Abs(Prey->GetActorLocation().Z - GetActorLocation().Z);
		if (DZ < 200.f) // another floor is not "touching"
		{
			In.bHasPrey = true;
			In.PreyCm = Prey->GetActorLocation();
			In.PreyDistanceCm = Dist;
		}
	}
	const FSpnCreatureOutput Out = Brain->Update(In);
	Apply(Out, DeltaSeconds);
}

void ASpnCreature::Apply(const FSpnCreatureOutput& Out, float DeltaSeconds)
{
	bListening = Out.bListening;
	if (Out.bChanged)
	{
		Log.Add({GetWorld()->GetTimeSeconds(), Out.Previous, Out.State});
		UE_LOG(LogSupernaturalProt, Log, TEXT("Creature %s: %s -> %s"), *GetName(), LexToString(Out.Previous), LexToString(Out.State));
		bHasSearchPoint = false;
	}
	if (Out.bCaught)
	{
		++Catches;
		UE_LOG(LogSupernaturalProt, Log, TEXT("Creature %s caught its prey"), *GetName());
		if (USpnCaseSubsystem* Case = GetWorld()->GetSubsystem<USpnCaseSubsystem>())
		{
			Case->DamagePlayer(CatchDamage, TEXT("creature"));
		}
	}
	GetCharacterMovement()->MaxWalkSpeed = Out.Pace == ESpnCreaturePace::Hunt ? HuntSpeedCm : ProwlSpeedCm;

	if (Out.Pace == ESpnCreaturePace::Still)
	{
		StopMoving(); // listening, or stunned
	}
	else if (!Out.bHasGoal)
	{
		RunPatrol(DeltaSeconds);
	}
	else if (Out.State == ESpnCreatureState::Search && Out.SearchRadiusCm > 0.f)
	{
		if (!bHasSearchPoint || FVector::Dist2D(GetActorLocation(), SearchPointCm) < 120.f)
		{
			SearchPointCm = PickSearchPoint(Out.GoalCm, Out.SearchRadiusCm);
			bHasSearchPoint = true;
		}
		LastGoalCm = SearchPointCm;
		MoveTowards(SearchPointCm);
	}
	else
	{
		LastGoalCm = FVector(Out.GoalCm.X, Out.GoalCm.Y, GetActorLocation().Z);
		MoveTowards(LastGoalCm);
	}
	if (Out.bChanged)
	{
		UpdateVisuals();
	}
}

FVector ASpnCreature::PickSearchPoint(const FVector& Centre, float Radius)
{
	const float Angle = Random.FRandRange(0.f, 2.f * PI);
	const float R = Random.FRandRange(0.3f, 1.f) * Radius;
	return FVector(Centre.X + FMath::Cos(Angle) * R, Centre.Y + FMath::Sin(Angle) * R, GetActorLocation().Z);
}

void ASpnCreature::RunPatrol(float DeltaSeconds)
{
	if (PatrolPoints.Num() == 0)
	{
		LastGoalCm = SpawnCm;
		MoveTowards(SpawnCm);
		return;
	}
	PatrolIndex = PatrolIndex % PatrolPoints.Num();
	const FVector Point = PatrolPoints[PatrolIndex];
	LastGoalCm = Point;
	if (FVector::Dist2D(GetActorLocation(), Point) <= 120.f)
	{
		StopMoving();
		PatrolWait += DeltaSeconds;
		if (PatrolWait >= PatrolWaitSeconds)
		{
			PatrolWait = 0.f;
			PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
		}
		return;
	}
	MoveTowards(Point);
}

void ASpnCreature::StopMoving()
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

void ASpnCreature::MoveTowards(const FVector& GoalCm)
{
	if (FVector::Dist2D(GetActorLocation(), GoalCm) <= 60.f)
	{
		return;
	}
	AAIController* AI = Cast<AAIController>(GetController());
	const bool bNewGoal = !bHasMoveGoal || FVector::Dist2D(GoalCm, MoveGoalCm) > 60.f;
	if (AI != nullptr && (bNewGoal || (!bFallbackSteering && AI->GetMoveStatus() == EPathFollowingStatus::Idle)))
	{
		MoveGoalCm = GoalCm;
		bHasMoveGoal = true;
		bFallbackSteering = AI->MoveToLocation(GoalCm, 40.f, false, true, true, false, nullptr, true) == EPathFollowingRequestResult::Failed;
	}
	else if (AI == nullptr)
	{
		bFallbackSteering = true;
		bHasMoveGoal = true;
		MoveGoalCm = GoalCm;
	}
	if (bFallbackSteering)
	{
		AddMovementInput((GoalCm - GetActorLocation()).GetSafeNormal2D(), 1.f);
	}
}

void ASpnCreature::UpdateVisuals()
{
	const ESpnCreatureState State = GetCreatureState();
	if (BodyMaterial != nullptr)
	{
		FLinearColor Color(0.06f, 0.06f, 0.07f); // near black
		if (State == ESpnCreatureState::Chase) { Color = FLinearColor(0.22f, 0.02f, 0.02f); }
		else if (State == ESpnCreatureState::Stunned) { Color = FLinearColor(0.25f, 0.25f, 0.3f); }
		BodyMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	}
	Label->SetText(FText::FromString(FString::Printf(TEXT("[placeholder creature] %s"), LexToString(State))));
}
