#include "Spn/SpnMovementDiagnostics.h"

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

void USpnMovementDiagnostics::DumpAndClear()
{
	if (Samples.Num() == 0)
	{
		return;
	}
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
