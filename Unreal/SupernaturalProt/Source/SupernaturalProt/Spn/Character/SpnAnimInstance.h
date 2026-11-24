#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SpnMovement.h"
#include "SpnAnimInstance.generated.h"

/**
 * C++ base of every player/NPC anim blueprint (Docs/Proposals/0013 step S1). It only READS the gameplay side - the movement state
 * interface of USpnCharacterMovementComponent and a few character flags - and never drives movement, so a new body or anim blueprint
 * cannot break gameplay. The variables are prefixed Spn so they never collide with the template's own blueprint variables.
 *
 * Status: engineering placeholder for the replaceable character presentation; no animation design, no final character.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPERNATURALPROT_API USpnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** Idle / Walk / Jog / Run / Sprint / Jumping / Falling / Landing (USpnCharacterMovementComponent::GetMovementState). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	ESpnMovementState SpnMovementState = ESpnMovementState::Idle;

	/** The gait judged from the horizontal speed (works on every proxy). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	ESpnGait SpnObservedGait = ESpnGait::Walk;

	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	float SpnGroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	float SpnVerticalSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	bool bSpnInAir = false;

	/** Seconds since the last touch-down; negative while none happened. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	float SpnSecondsSinceLanding = -1.f;

	/** The player is taking the automatic step out of a door's swing. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation")
	bool bSpnSteppingAside = false;

	/**
	 * Locomotion cadence above the fastest authored clip (2026-11-15, Docs/Proposals/0017 task B). The locomotion clips are authored at
	 * 300 (walk) and 600 cm/s (jog) and the blend space ends at 600, so at sprint speed (800) the feet kept the 600 cadence and slid by the
	 * difference. Above LocomotionAuthoredTopSpeed the whole mesh plays faster by Speed / LocomotionAuthoredTopSpeed (capped), only on the
	 * ground and never during the ledge assist. PLACEHOLDER values (measured with Tools/locomotion_stride_check.py, not tuned by design).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Animation|Locomotion")
	bool bSpnScaleCadenceAboveAuthoredSpeed = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Animation|Locomotion", meta = (ClampMin = "100"))
	float SpnLocomotionAuthoredTopSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Animation|Locomotion", meta = (ClampMin = "1", ClampMax = "3"))
	float SpnMaxCadenceScale = 1.6f;

	/** Current play-rate factor applied to the mesh (1 = authored speed). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Animation|Locomotion")
	float SpnCadenceScale = 1.f;

	/** Reads the owner once (also called every animation update). Public for tests and for anim blueprints that need it early. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Animation")
	void RefreshFromOwner();

	/** True once an owner with a USpnCharacterMovementComponent was found. */
	UFUNCTION(BlueprintPure, Category = "Spn|Animation")
	bool HasSpnMovement() const { return bHasSpnMovement; }

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	bool bHasSpnMovement = false;
};
