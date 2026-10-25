#pragma once

#include "CoreMinimal.h"
#include "SpnMovement.generated.h"

/**
 * The four on-foot gaits (M2-A, Docs/Proposals/0011). Values are the 2-bit wire values of the
 * movement prediction; do not reorder. Sprint is a temporary request, never a base gait.
 */
UENUM(BlueprintType)
enum class ESpnGait : uint8
{
	Walk = 0,
	Jog = 1,
	Run = 2,
	Sprint = 3
};

UENUM(BlueprintType)
enum class ESpnObstacleClass : uint8
{
	/** Walking up is enough (height <= max step height). */
	StepUp,
	/** Above the step height, within the jump height. */
	Jump,
	/** Above both. No vault or mantle exists yet (Proposal 0009). */
	Blocked
};

/**
 * Configurable movement parameters, in centimetres like the engine. Every value is a PLACEHOLDER
 * TEST VALUE of the M2-A work order, not a final gameplay number. The rules behind them (validation,
 * gait resolution, jump height) live in the engine-independent core (spn/core/Movement.h).
 */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnMovementSettings
{
	GENERATED_BODY()

	/** Top speed per gait, cm/s. Must be strictly ascending Walk < Jog < Run < Sprint. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits", meta = (ClampMin = "1"))
	float WalkSpeed = 140.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits", meta = (ClampMin = "1"))
	float JogSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits", meta = (ClampMin = "1"))
	float RunSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits", meta = (ClampMin = "1"))
	float SprintSpeed = 800.f;

	/** Acceleration per gait while input is held, cm/s^2. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "1"))
	float WalkAcceleration = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "1"))
	float JogAcceleration = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "1"))
	float RunAcceleration = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "1"))
	float SprintAcceleration = 400.f;

	/** Deceleration without input, cm/s^2. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "1"))
	float BrakingDeceleration = 800.f;

	/** How quickly the velocity direction follows the input direction (Character Movement "Ground Friction"). Higher = tighter turns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "0"))
	float TurnFriction = 8.f;

	/** Gait used while no other is chosen. Never Sprint. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits")
	ESpnGait DefaultGait = ESpnGait::Jog;

	/** Speed factor for backward / sideways input. Applies only while the character does not turn towards its movement (aiming, lock-on later). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction", meta = (ClampMin = "0.05", ClampMax = "1"))
	float BackwardSpeedFactor = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction", meta = (ClampMin = "0.05", ClampMax = "1"))
	float StrafeSpeedFactor = 0.85f;

	/** Sprinting needs input within this angle of straight ahead, degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction", meta = (ClampMin = "0", ClampMax = "180"))
	float SprintMaxAngleDegrees = 50.f;

	/** Vertical apex height of an active standing jump, cm. NOT the maximum obstacle height. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "1", ClampMax = "200"))
	float JumpHeight = 45.f;

	/** Share of the ground acceleration available in the air (Character Movement "Air Control"). Small on purpose: no supernatural air control. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0", ClampMax = "1"))
	float AirControl = 0.05f;

	/** Horizontal deceleration while airborne without input, cm/s^2. 0 keeps the take-off momentum (the template's 1500 stopped a jump dead in the air). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0"))
	float AirBrakingDeceleration = 0.f;

	/** Highest ledge walked up without jumping, cm (applied to the Character Movement "Max Step Height"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0", ClampMax = "100"))
	float MaxStepHeight = 45.f;
};

/** Bridge to the engine-independent movement rules (spn/core/Movement.h). Core types stay behind this boundary. */
namespace SpnMovement
{
	/** True when valid; otherwise OutError holds the reason. */
	SPNCORE_API bool Validate(const FSpnMovementSettings& Settings, FString& OutError);

	SPNCORE_API float GaitSpeed(const FSpnMovementSettings& Settings, ESpnGait Gait);
	SPNCORE_API float GaitAcceleration(const FSpnMovementSettings& Settings, ESpnGait Gait);

	/**
	 * The gait actually used: the chosen base gait, or Sprint when requested, allowed by MaxGait and
	 * input pointing forward within SprintMaxAngleDegrees. Input is the move vector in the character's frame
	 * (X = forward, Y = right). Never above MaxGait.
	 */
	SPNCORE_API ESpnGait ResolveGait(const FSpnMovementSettings& Settings, ESpnGait Base, bool bSprintRequested, ESpnGait MaxGait, FVector2D Input);

	/** Speed factor (BackwardSpeedFactor..1) for an input direction (X = forward, Y = right). */
	SPNCORE_API float DirectionSpeedScale(const FSpnMovementSettings& Settings, FVector2D Input);

	/** Launch velocity (cm/s) reaching HeightCm under GravityCm (cm/s^2, positive). 0 for invalid input. */
	SPNCORE_API float JumpVelocityForHeight(float HeightCm, float GravityCm);

	/** Apex height (cm) of a jump with the given launch velocity (cm/s). */
	SPNCORE_API float JumpApexHeight(float LaunchVelocityCm, float GravityCm);

	SPNCORE_API ESpnObstacleClass ClassifyObstacle(const FSpnMovementSettings& Settings, float HeightCm);
}
