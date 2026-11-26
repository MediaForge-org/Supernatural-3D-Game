#include "Spn/Case/SpnCaseObjects.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Spn/Case/SpnCaseSubsystem.h"
#include "SpnNpcReaction.h"
#include "SupernaturalProt.h"
#include "UObject/ConstructorHelpers.h"

namespace SpnCaseObjectsLocal
{
	/** Constructor-only: an engine basic shape. */
	UStaticMesh* Shape(const TCHAR* Path)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(Path);
		return Finder.Succeeded() ? Finder.Object : nullptr;
	}

	USpnCaseSubsystem* CaseOf(const AActor* Actor)
	{
		return Actor != nullptr && Actor->GetWorld() != nullptr ? Actor->GetWorld()->GetSubsystem<USpnCaseSubsystem>() : nullptr;
	}

	FSpnInteractStatus Status(ESpnInteractResult Result, const TCHAR* Feedback)
	{
		FSpnInteractStatus S;
		S.Result = Result;
		S.Feedback = Feedback;
		return S;
	}
}

// ------------------------------------------------------------------ bell

ASpnBell::ASpnBell()
{
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	RootComponent = Frame;
	Frame->SetStaticMesh(SpnCaseObjectsLocal::Shape(TEXT("/Engine/BasicShapes/Cube.Cube")));
	Frame->SetRelativeScale3D(FVector(0.2f, 1.2f, 0.2f));
	Frame->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	Bell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell"));
	Bell->SetupAttachment(Frame);
	Bell->SetStaticMesh(SpnCaseObjectsLocal::Shape(TEXT("/Engine/BasicShapes/Cone.Cone")));
	Bell->SetRelativeLocation(FVector(0.f, 0.f, -260.f)); // in the frame's scaled space: about 50 cm below the beam
	Bell->SetRelativeScale3D(FVector(3.f, 0.5f, 3.f));
}

FSpnInteractStatus ASpnBell::SpnUse(AActor* User)
{
	const double Now = GetWorld() != nullptr ? GetWorld()->GetTimeSeconds() : 0.0;
	if (Now - LastRingTime < CooldownSeconds)
	{
		return SpnCaseObjectsLocal::Status(ESpnInteractResult::Unchanged, TEXT("placeholder:feedback.bell.still_ringing"));
	}
	USpnCaseSubsystem* Case = SpnCaseObjectsLocal::CaseOf(this);
	if (Case == nullptr)
	{
		return SpnCaseObjectsLocal::Status(ESpnInteractResult::Failed, TEXT(""));
	}
	LastRingTime = Now;
	++RingCount;
	LastStunned = Case->RingBell(GetActorLocation(), User);
	return SpnCaseObjectsLocal::Status(ESpnInteractResult::Changed, TEXT("placeholder:feedback.bell.rung"));
}

FString ASpnBell::SpnPromptText(const AActor* User) const
{
	return TEXT("Glocke läuten");
}

// ------------------------------------------------------------------ throwable pickup

ASpnThrowablePickup::ASpnThrowablePickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetStaticMesh(SpnCaseObjectsLocal::Shape(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));
	Mesh->SetRelativeScale3D(FVector(0.08f, 0.08f, 0.28f)); // bottle-sized
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

FSpnInteractStatus ASpnThrowablePickup::SpnUse(AActor* User)
{
	USpnCaseSubsystem* Case = SpnCaseObjectsLocal::CaseOf(this);
	if (Case == nullptr)
	{
		return SpnCaseObjectsLocal::Status(ESpnInteractResult::Failed, TEXT(""));
	}
	if (!Case->AddThrowable())
	{
		return SpnCaseObjectsLocal::Status(ESpnInteractResult::Refused, TEXT("placeholder:feedback.throwable.hands_full"));
	}
	Destroy();
	return SpnCaseObjectsLocal::Status(ESpnInteractResult::Changed, TEXT("placeholder:feedback.throwable.taken"));
}

FString ASpnThrowablePickup::SpnPromptText(const AActor* User) const
{
	return TEXT("Flasche nehmen (G: werfen)");
}

// ------------------------------------------------------------------ thrown object

ASpnThrownObject::ASpnThrownObject()
{
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->InitSphereRadius(6.f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->OnComponentHit.AddDynamic(this, &ASpnThrownObject::OnHit);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetStaticMesh(SpnCaseObjectsLocal::Shape(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));
	Mesh->SetRelativeScale3D(FVector(0.08f, 0.08f, 0.28f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = Collision;
	Movement->bAutoActivate = false;
	Movement->bShouldBounce = true;
	Movement->Bounciness = 0.2f;
	Movement->Friction = 0.6f;
	Movement->ProjectileGravityScale = 1.f;
	InitialLifeSpan = 20.f;
}

void ASpnThrownObject::Launch(const FVector& VelocityCm)
{
	if (APawn* Thrower = GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(Thrower, true);
	}
	Movement->Velocity = VelocityCm;
	Movement->Activate(true);
}

void ASpnThrownObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bLanded)
	{
		return;
	}
	bLanded = true;
	ImpactCm = Hit.ImpactPoint;
	const USpnCaseSettings* Settings = GetDefault<USpnCaseSettings>();
	if (UGameInstance* Instance = GetGameInstance())
	{
		if (USpnNoiseSubsystem* Noise = Instance->GetSubsystem<USpnNoiseSubsystem>())
		{
			FSpnNoise N;
			N.LocationCm = ImpactCm;
			N.RangeCm = Settings->ThrowNoiseRangeCm;
			N.Strength = Settings->ThrowNoiseStrength;
			N.Kind = ESpnNoiseKind::Impact;
			N.SourceId = TEXT("placeholder.case.thrown");
			Noise->EmitNoise(N);
		}
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("Case: thrown object landed at %s"), *ImpactCm.ToCompactString());
	SetLifeSpan(6.f);
}

// ------------------------------------------------------------------ autosave point

ASpnAutosavePoint::ASpnAutosavePoint()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Trigger"));
	Box->SetGenerateOverlapEvents(true);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASpnAutosavePoint::OnOverlap);
	Box->SetBoxExtent(Extent);
}

void ASpnAutosavePoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Box->SetBoxExtent(Extent);
}

void ASpnAutosavePoint::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateUObject(this, &ASpnAutosavePoint::CheckStartInside), 1.0f, false);
}

void ASpnAutosavePoint::CheckStartInside()
{
	USpnCaseSubsystem* Case = SpnCaseObjectsLocal::CaseOf(this);
	APawn* Player = Case != nullptr ? Case->GetPlayerPawn() : nullptr;
	if (Player != nullptr && Box->IsOverlappingActor(Player))
	{
		Case->RequestAutosave(PointId);
	}
}

void ASpnAutosavePoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USpnCaseSubsystem* Case = SpnCaseObjectsLocal::CaseOf(this);
	if (Case != nullptr && OtherActor != nullptr && OtherActor == Case->GetPlayerPawn())
	{
		Case->RequestAutosave(PointId);
	}
}
