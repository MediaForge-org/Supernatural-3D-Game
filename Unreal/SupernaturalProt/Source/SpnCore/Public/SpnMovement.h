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

/**
 * What the character is doing, derived from physical quantities only (M2-A.1). There is no stored state machine:
 * the same velocity, floor contact and landing time always give the same state, on every client. Animation
 * blueprints read this instead of keeping a second state machine. Values follow spn::core::MovementState.
 */
UENUM(BlueprintType)
enum class ESpnMovementState : uint8
{
	Idle = 0,
	Walk,
	Jog,
	Run,
	Sprint,
	/** In the air and moving up. */
	Jumping,
	/** In the air and not moving up (includes the apex and walking off a ledge). */
	Falling,
	/** On the ground, shortly after touching down (SpnMovementSettings::LandingSeconds). */
	Landing
};

/** A rule of thumb about obstacle HEIGHT only. A JumpCandidate is not guaranteed to be passable. */
UENUM(BlueprintType)
enum class ESpnObstacleClass : uint8
{
	/** Walking up is enough (height <= max step height), given suitable collision geometry. */
	StepUp,
	/** Above the step height and below the jump apex: a jump MAY get over it. Run-up, timing and collision decide. */
	JumpCandidate,
	/** At or above the jump apex and above the step height. No vault or mantle exists yet (Proposal 0009). */
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
	float WalkSpeed = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits", meta = (ClampMin = "1"))
	float JogSpeed = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaits", meta = (ClampMin = "1"))
	float RunSpeed = 550.f;

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

	/** Starting a sprint needs input within this angle of straight ahead, degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction", meta = (ClampMin = "0", ClampMax = "180"))
	float SprintMaxAngleDegrees = 50.f;

	/**
	 * While already at sprint speed the sprint is kept up to this wider angle (not below SprintMaxAngleDegrees).
	 * Without it, a sharp turn drops the sprint until the character has turned towards the new direction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction", meta = (ClampMin = "0", ClampMax = "180"))
	float SprintSustainAngleDegrees = 100.f;

	/** How fast the character turns towards its movement direction, degrees/s (Character Movement "Rotation Rate" Yaw). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction", meta = (ClampMin = "1", ClampMax = "3600"))
	float TurnRateDegrees = 500.f;

	/** Speed of the smallest analog deflection that still moves the character, cm/s (Character Movement "Min Analog Walk Speed"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration", meta = (ClampMin = "0"))
	float MinAnalogSpeed = 20.f;

	/** Vertical apex height of an active standing jump, cm. NOT the maximum obstacle height. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "1", ClampMax = "200"))
	float JumpHeight = 45.f;

	/** Share of the ground acceleration available in the air (Character Movement "Air Control"). Small on purpose: no supernatural air control. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0", ClampMax = "1"))
	float AirControl = 0.05f;

	/** Horizontal deceleration while airborne without input, cm/s^2. 0 keeps the take-off momentum (the template's 1500 stopped a jump dead in the air). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0"))
	float AirBrakingDeceleration = 0.f;

	/**
	 * Highest ledge walked up without jumping, cm (applied to the Character Movement "Max Step Height"). Independent of
	 * JumpHeight. What can really be climbed also depends on the collision shapes (a step is only taken where the capsule fits).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0", ClampMax = "100"))
	float MaxStepHeight = 25.f;

	/**
	 * Ground steering (M2A4.1): the velocity vector bends towards the input direction at a limited rate and loses speed in proportion to the angle
	 * turned (Core "SteerVelocity"), instead of the engine's friction blend, which shrinks the vector cumulatively (800 -> 462 cm/s in a 90 degree
	 * sprint turn, measured). Applies to walking with OrientRotationToMovement only; off = engine friction (TurnFriction).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	bool bUseSteering = true;

	/** Heading change rate of the velocity at low speed and at SprintSpeed, degrees/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering", meta = (ClampMin = "1", ClampMax = "3600"))
	float MaxHeadingRateDegrees = 720.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering", meta = (ClampMin = "1", ClampMax = "3600"))
	float MinHeadingRateDegrees = 300.f;

	/** Fraction of the speed lost per radian of heading change (0.15: a 90 degree turn costs about 23 %). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering", meta = (ClampMin = "0", ClampMax = "1"))
	float SpeedLossPerRadian = 0.15f;

	/**
	 * Character Movement "Perch Radius Threshold", cm: the capsule does not rest on an edge whose contact lies further than (radius - this) from its
	 * axis. 0 = the engine default (the rounded capsule bottom rolls over any ledge corner it can touch and a jump then clears ledges up to about
	 * apex + 25 cm). 14 cm is the value measured in M2A4.1 (Jump suite, ledges 30..100 cm, 12 and 16 cm gave identical results): ledges up to the jump
	 * apex stay jumpable, 60 cm and higher stay blocked.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMin = "0", ClampMax = "42"))
	float PerchRadiusThreshold = 14.f;

	/**
	 * While walking, do not let a slide along a non-walkable surface (a ledge corner) lift the capsule more than the move intended (the engine does this only
	 * while falling). M2A4.1: measured without effect on the 30 cm walking cases; kept as parity with the falling rule, candidate for removal.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	bool bPreventGroundSlopeBoosting = true;

	/**
	 * Character Movement "Use Flat Base For Floor Checks". Off: the step-up test measures the step height against the ROUNDED capsule bottom, so a
	 * "25 cm" step limit really allows about 30 cm (the capsule slides over the corner). On: floor checks use the flat base, the limit is the limit.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	bool bUseFlatBaseForFloorChecks = true;

	/**
	 * Movement investigation after Night Build 01 (Docs/NightBuild01/MOVEMENT_INVESTIGATION_REPORT.md): while walking, a floor found by the floor check
	 * that would lift the capsule by more than MaxStepHeight is not accepted. Measured root cause of the 30 cm cases: StepUp correctly refuses the ledge,
	 * but the engine's floor sweep (flat base = a box turned 45 degrees whose corners reach the full capsule radius, restarted 48.6 cm above the capsule
	 * bottom after a penetrating first try) lands on the ledge TOP while the capsule is blocked at its face; the floor is reported walkable at -27.9 cm and
	 * AdjustFloorHeight lifts the capsule by 30 cm in one frame, bypassing the step limit. With this rule the floor is looked up again with the narrower
	 * perch probe (radius minus PerchRadiusThreshold); only that floor is used. Not a tuning value: it makes MaxStepHeight the limit it claims to be.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	bool bLimitFloorRiseToStepHeight = true;

	/** Below this horizontal speed the character counts as standing, cm/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (ClampMin = "0"))
	float IdleSpeed = 5.f;

	/** How long the Landing state lasts after touching down, seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (ClampMin = "0", ClampMax = "2"))
	float LandingSeconds = 0.25f;
};

/** Bridge to the engine-independent movement rules (spn/core/Movement.h). Core types stay behind this boundary. */
namespace SpnMovement
{
	/** True when valid; otherwise OutError holds the reason. */
	SPNCORE_API bool Validate(const FSpnMovementSettings& Settings, FString& OutError);

	SPNCORE_API float GaitSpeed(const FSpnMovementSettings& Settings, ESpnGait Gait);
	SPNCORE_API float GaitAcceleration(const FSpnMovementSettings& Settings, ESpnGait Gait);

	/**
	 * The gait actually used: the chosen base gait, or Sprint when requested, allowed by MaxGait and input within
	 * SprintMaxAngleDegrees of straight ahead (SprintSustainAngleDegrees once CurrentSpeedCm is at sprint speed).
	 * Input is the move vector in the character's frame (X = forward, Y = right). Never above MaxGait.
	 */
	SPNCORE_API ESpnGait ResolveGait(const FSpnMovementSettings& Settings, ESpnGait Base, bool bSprintRequested, ESpnGait MaxGait, FVector2D Input, float CurrentSpeedCm = 0.f);

	/** The gait whose top speed is nearest to the horizontal speed (cm/s). For display and animation. */
	SPNCORE_API ESpnGait ObservedGait(const FSpnMovementSettings& Settings, float HorizontalSpeedCm);

	/** Movement state from physical quantities (cm/s; SecondsSinceLanding < 0 means no recent landing). */
	SPNCORE_API ESpnMovementState ClassifyMotion(const FSpnMovementSettings& Settings, float HorizontalSpeedCm, float VerticalVelocityCm, bool bOnGround, float SecondsSinceLanding);

	/** Speed factor (BackwardSpeedFactor..1) for an input direction (X = forward, Y = right). */
	SPNCORE_API float DirectionSpeedScale(const FSpnMovementSettings& Settings, FVector2D Input);

	/** Launch velocity (cm/s) reaching HeightCm under GravityCm (cm/s^2, positive). 0 for invalid input. */
	SPNCORE_API float JumpVelocityForHeight(float HeightCm, float GravityCm);

	/** Apex height (cm) of a jump with the given launch velocity (cm/s). */
	SPNCORE_API float JumpApexHeight(float LaunchVelocityCm, float GravityCm);

	SPNCORE_API ESpnObstacleClass ClassifyObstacle(const FSpnMovementSettings& Settings, float HeightCm);

	/**
	 * One steering step on the ground (Core SteerVelocity). VelocityCm and the result are horizontal velocities in cm/s; Direction is the wanted heading
	 * (zero = no input, the character brakes with BrakingDeceleration); TargetSpeedCm and AccelerationCm come from the gait.
	 */
	SPNCORE_API FVector2D SteerVelocity(const FSpnMovementSettings& Settings, FVector2D VelocityCm, FVector2D Direction, float TargetSpeedCm, float AccelerationCm, float DeltaSeconds);
}
