#include "Spn/Character/SpnAnimInstance.h"

#include "GameFramework/Pawn.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "SupernaturalProtCharacter.h"

void USpnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	RefreshFromOwner();

	// cadence above the fastest authored locomotion clip (see the header); smoothed so a gait change never pops
	if (USkeletalMeshComponent* Mesh = GetSkelMeshComponent(); Mesh != nullptr && bHasSpnMovement)
	{
		const APawn* Pawn = TryGetPawnOwner();
		const USpnCharacterMovementComponent* Movement = Pawn != nullptr ? Pawn->FindComponentByClass<USpnCharacterMovementComponent>() : nullptr;
		const bool bGroundLocomotion = !bSpnInAir && Movement != nullptr && !Movement->IsLedgeAssisting();
		const float Target = bSpnScaleCadenceAboveAuthoredSpeed && bGroundLocomotion && SpnGroundSpeed > SpnLocomotionAuthoredTopSpeed
			? FMath::Min(SpnGroundSpeed / SpnLocomotionAuthoredTopSpeed, SpnMaxCadenceScale) : 1.f;
		SpnCadenceScale = FMath::FInterpTo(SpnCadenceScale, Target, DeltaSeconds, 8.f);
		if (!FMath::IsNearlyEqual(SpnCadenceScale, Target, 0.002f) || !FMath::IsNearlyEqual(Mesh->GlobalAnimRateScale, SpnCadenceScale, 0.002f))
		{
			Mesh->GlobalAnimRateScale = SpnCadenceScale;
		}
	}
}

void USpnAnimInstance::RefreshFromOwner()
{
	const APawn* Pawn = TryGetPawnOwner();
	const USpnCharacterMovementComponent* Movement = Pawn != nullptr ? Pawn->FindComponentByClass<USpnCharacterMovementComponent>() : nullptr;
	bHasSpnMovement = Movement != nullptr;
	if (Movement == nullptr)
	{
		return;     // e.g. the anim blueprint previewed in the editor: keep the defaults
	}
	const FVector Velocity = Movement->Velocity;
	SpnMovementState = Movement->GetMovementState();
	SpnObservedGait = Movement->GetObservedGait();
	SpnGroundSpeed = Velocity.Size2D();
	SpnVerticalSpeed = Velocity.Z;
	bSpnInAir = Movement->IsFalling();
	SpnSecondsSinceLanding = Movement->GetSecondsSinceLanding();
	const ASupernaturalProtCharacter* Character = Cast<ASupernaturalProtCharacter>(Pawn);
	bSpnSteppingAside = Character != nullptr && Character->IsSteppingAside();
}
