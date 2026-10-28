#include "Spn/SpnMovementDiagnostics.h"

#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HAL/IConsoleManager.h"
#include "SupernaturalProt.h"

namespace
{
TAutoConsoleVariable<int32> CVarMoveDiag(
	TEXT("spn.MoveDiag"), 0,
	TEXT("1 = every USpnMovementDiagnostics component records the last frames and dumps them to the log on landing / stop."),
	ECVF_Default);

constexpr uint8 EventJumpStart = 1;
constexpr uint8 EventLanded = 2;
constexpr uint8 EventStepUp = 4;
constexpr uint8 EventSpeedCollapse = 8;
}

USpnMovementDiagnostics::USpnMovementDiagnostics()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

bool USpnMovementDiagnostics::IsRecording() const
{
	return bEnabled || CVarMoveDiag.GetValueOnGameThread() != 0;
}

void USpnMovementDiagnostics::Push(const FSample& Sample)
{
	if (Samples.Num() < MaxSamples)
	{
		Samples.Add(Sample);
		return;
	}
	Samples[Head] = Sample;
	Head = (Head + 1) % Samples.Num();
}

void USpnMovementDiagnostics::BeginPlay()
{
	Super::BeginPlay();
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character != nullptr && Character->GetCapsuleComponent() != nullptr)
	{
		Character->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &USpnMovementDiagnostics::OnCapsuleHit);
	}
}

void USpnMovementDiagnostics::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character != nullptr && Character->GetCapsuleComponent() != nullptr)
	{
		Character->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &USpnMovementDiagnostics::OnCapsuleHit);
	}
	Super::EndPlay(EndPlayReason);
}

void USpnMovementDiagnostics::OnCapsuleHit(UPrimitiveComponent*, AActor*, UPrimitiveComponent* OtherComponent, FVector, const FHitResult& Hit)
{
	if (!IsRecording() || GetWorld() == nullptr)
	{
		return;
	}
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	const UCharacterMovementComponent* Movement = Character != nullptr ? Character->GetCharacterMovement() : nullptr;
	FHitSample Sample;
	Sample.Time = GetWorld()->GetTimeSeconds();
	Sample.Normal = Hit.ImpactNormal;
	Sample.Impact = Hit.ImpactPoint;
	Sample.Component = OtherComponent != nullptr ? OtherComponent->GetName() : FString(TEXT("none"));
	Sample.bWalkable = Movement != nullptr && Movement->IsWalkable(Hit);
	if (Hits.Num() >= MaxHits)
	{
		Hits.RemoveAt(0, 1, EAllowShrinking::No); // keep the newest hits; MaxHits is small
	}
	Hits.Add(Sample);
}

void USpnMovementDiagnostics::DumpAndClear()
{
	if (Samples.Num() == 0)
	{
		Hits.Reset();
		return;
	}
	if (const ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		const UCharacterMovementComponent* M = Owner->GetCharacterMovement();
		const UCapsuleComponent* Capsule = Owner->GetCapsuleComponent();
		if (M != nullptr && Capsule != nullptr)
		{
			UE_LOG(LogSupernaturalProt, Display,
				TEXT("SpnMoveDiagCfg,maxStep=%.2f,jumpZ=%.1f,gravityZ=%.1f,airControl=%.3f,groundFriction=%.2f,brakingWalking=%.1f,capsuleR=%.1f,capsuleHH=%.1f,walkableZ=%.4f,perchRadius=%.1f,perchExtra=%.1f,flatBase=%d,maxAccel=%.1f"),
				M->MaxStepHeight, M->JumpZVelocity, M->GetGravityZ(), M->AirControl, M->GroundFriction, M->BrakingDecelerationWalking,
				Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(), M->GetWalkableFloorZ(), M->PerchRadiusThreshold,
				M->PerchAdditionalHeight, M->bUseFlatBaseForFloorChecks ? 1 : 0, M->GetMaxAcceleration());
		}
	}
	for (const FHitSample& H : Hits)
	{
		UE_LOG(LogSupernaturalProt, Display, TEXT("SpnMoveDiagHit,%.4f,%.3f,%.3f,%.3f,%.1f,%.1f,%.1f,%d,%s"), H.Time, H.Normal.X, H.Normal.Y, H.Normal.Z,
			H.Impact.X, H.Impact.Y, H.Impact.Z, H.bWalkable ? 1 : 0, *H.Component);
	}
	Hits.Reset();
	UE_LOG(LogSupernaturalProt, Display, TEXT("SpnMoveDiag,time,x,y,z,vx,vy,vz,mode,floorDist,walkable,events"));
	for (int32 i = 0; i < Samples.Num(); ++i)
	{
		const FSample& S = Samples[(Head + i) % Samples.Num()];
		UE_LOG(LogSupernaturalProt, Display, TEXT("SpnMoveDiag,%.4f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%d,%.2f,%d,%d"), S.Time, S.Position.X, S.Position.Y,
			S.Position.Z, S.Velocity.X, S.Velocity.Y, S.Velocity.Z, static_cast<int32>(S.Mode), S.FloorDist, S.bWalkableFloor ? 1 : 0, static_cast<int32>(S.Events));
	}
	Samples.Reset();
	Head = 0;
	bHavePrevious = false;
}

void USpnMovementDiagnostics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bRecording = IsRecording();
	if (!bRecording)
	{
		if (bWasEnabled)
		{
			DumpAndClear();
			bWasEnabled = false;
		}
		return;
	}
	bWasEnabled = true;

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	const UCharacterMovementComponent* Movement = Character != nullptr ? Character->GetCharacterMovement() : nullptr;
	if (Movement == nullptr || GetWorld() == nullptr)
	{
		return;
	}

	FSample Now;
	Now.Time = GetWorld()->GetTimeSeconds();
	Now.Position = Character->GetActorLocation();
	Now.Velocity = Movement->Velocity;
	Now.Mode = static_cast<uint8>(Movement->MovementMode.GetValue());
	Now.FloorDist = Movement->CurrentFloor.FloorDist;
	Now.bWalkableFloor = Movement->CurrentFloor.IsWalkableFloor();

	if (bHavePrevious)
	{
		const bool bWasWalking = Previous.Mode == static_cast<uint8>(MOVE_Walking);
		const bool bIsWalking = Now.Mode == static_cast<uint8>(MOVE_Walking);
		const bool bWasFalling = Previous.Mode == static_cast<uint8>(MOVE_Falling);
		const bool bIsFalling = Now.Mode == static_cast<uint8>(MOVE_Falling);
		if (bWasWalking && bIsFalling && Now.Velocity.Z > 0.f)
		{
			Now.Events |= EventJumpStart;
		}
		if (bWasFalling && bIsWalking)
		{
			Now.Events |= EventLanded;
		}
		if (bWasWalking && bIsWalking && Now.Position.Z - Previous.Position.Z >= StepUpMinDeltaZ)
		{
			Now.Events |= EventStepUp;
		}
		if (bWasFalling && bIsFalling)
		{
			const float PreviousSpeed = Previous.Velocity.Size2D();
			if (PreviousSpeed > 100.f && Now.Velocity.Size2D() < PreviousSpeed * (1.f - SpeedCollapseFraction))
			{
				Now.Events |= EventSpeedCollapse;
			}
		}
	}

	Push(Now);
	Previous = Now;
	bHavePrevious = true;

	if (bDumpOnLanding && (Now.Events & EventLanded) != 0)
	{
		DumpAndClear();
	}
}
