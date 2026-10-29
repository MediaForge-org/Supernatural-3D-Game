#include "SpnMovement.h"

#include <spn/core/GroundSteering.h>
#include <spn/core/Movement.h>

namespace
{
	spn::core::Gait ToCore(ESpnGait Gait)
	{
		const auto Converted = spn::core::GaitFromIndex(static_cast<unsigned>(Gait));
		return Converted.value_or(spn::core::Gait::Run);
	}

	ESpnGait FromCore(spn::core::Gait Gait)
	{
		return static_cast<ESpnGait>(static_cast<uint8>(Gait));
	}

	// Centimetres to metres for the core.
	constexpr double CmToM = 0.01;

	spn::core::MovementConfig ToCore(const FSpnMovementSettings& S)
	{
		spn::core::MovementConfig Config;
		Config.gaits[0] = {S.WalkSpeed * CmToM, S.WalkAcceleration * CmToM};
		Config.gaits[1] = {S.JogSpeed * CmToM, S.JogAcceleration * CmToM};
		Config.gaits[2] = {S.RunSpeed * CmToM, S.RunAcceleration * CmToM};
		Config.gaits[3] = {S.SprintSpeed * CmToM, S.SprintAcceleration * CmToM};
		Config.defaultGait = ToCore(S.DefaultGait);
		Config.brakingDeceleration = S.BrakingDeceleration * CmToM;
		Config.backwardSpeedFactor = S.BackwardSpeedFactor;
		Config.strafeSpeedFactor = S.StrafeSpeedFactor;
		Config.sprintMaxAngleDegrees = S.SprintMaxAngleDegrees;
		Config.sprintSustainAngleDegrees = S.SprintSustainAngleDegrees;
		Config.turnRateDegrees = S.TurnRateDegrees;
		Config.minAnalogSpeed = S.MinAnalogSpeed * CmToM;
		Config.jumpHeight = S.JumpHeight * CmToM;
		Config.maxStepHeight = S.MaxStepHeight * CmToM;
		Config.idleSpeed = S.IdleSpeed * CmToM;
		Config.landingSeconds = S.LandingSeconds;
		return Config;
	}

	spn::core::SteeringConfig ToSteering(const FSpnMovementSettings& S)
	{
		spn::core::SteeringConfig Config;
		Config.maxHeadingRateDegrees = S.MaxHeadingRateDegrees;
		Config.minHeadingRateDegrees = S.MinHeadingRateDegrees;
		Config.fullSpeed = S.SprintSpeed * CmToM;
		Config.speedLossPerRadian = S.SpeedLossPerRadian;
		return Config;
	}

	spn::core::MoveIntent ToIntent(FVector2D Input)
	{
		return spn::core::MoveIntent{Input.X, Input.Y};
	}
}

bool SpnMovement::Validate(const FSpnMovementSettings& Settings, FString& OutError)
{
	std::optional<std::string> Error = spn::core::Validate(ToCore(Settings));
	if (!Error.has_value())
	{
		Error = spn::core::Validate(ToSteering(Settings));
	}
	if (Error.has_value())
	{
		OutError = FString(UTF8_TO_TCHAR(Error->c_str()));
		return false;
	}
	OutError.Reset();
	return true;
}

float SpnMovement::GaitSpeed(const FSpnMovementSettings& Settings, ESpnGait Gait)
{
	return static_cast<float>(spn::core::GaitSpeed(ToCore(Settings), ToCore(Gait)) / CmToM);
}

float SpnMovement::GaitAcceleration(const FSpnMovementSettings& Settings, ESpnGait Gait)
{
	return static_cast<float>(spn::core::GaitAcceleration(ToCore(Settings), ToCore(Gait)) / CmToM);
}

ESpnGait SpnMovement::ResolveGait(const FSpnMovementSettings& Settings, ESpnGait Base, bool bSprintRequested, ESpnGait MaxGait, FVector2D Input, float CurrentSpeedCm)
{
	spn::core::GaitLimits Limits;
	Limits.maxGait = ToCore(MaxGait);
	return FromCore(spn::core::ResolveGait(ToCore(Settings), ToCore(Base), bSprintRequested, Limits, ToIntent(Input), CurrentSpeedCm * CmToM));
}

ESpnGait SpnMovement::ObservedGait(const FSpnMovementSettings& Settings, float HorizontalSpeedCm)
{
	return FromCore(spn::core::ObservedGait(ToCore(Settings), HorizontalSpeedCm * CmToM));
}

ESpnMovementState SpnMovement::ClassifyMotion(const FSpnMovementSettings& Settings, float HorizontalSpeedCm, float VerticalVelocityCm, bool bOnGround, float SecondsSinceLanding)
{
	const spn::core::MotionSample Sample{HorizontalSpeedCm * CmToM, VerticalVelocityCm * CmToM, bOnGround, SecondsSinceLanding};
	return static_cast<ESpnMovementState>(static_cast<uint8>(spn::core::ClassifyMotion(ToCore(Settings), Sample)));
}

float SpnMovement::DirectionSpeedScale(const FSpnMovementSettings& Settings, FVector2D Input)
{
	return static_cast<float>(spn::core::DirectionSpeedScale(ToCore(Settings), ToIntent(Input)));
}

float SpnMovement::JumpVelocityForHeight(float HeightCm, float GravityCm)
{
	return static_cast<float>(spn::core::JumpVelocityForHeight(HeightCm * CmToM, GravityCm * CmToM) / CmToM);
}

float SpnMovement::JumpApexHeight(float LaunchVelocityCm, float GravityCm)
{
	return static_cast<float>(spn::core::JumpApexHeight(LaunchVelocityCm * CmToM, GravityCm * CmToM) / CmToM);
}

ESpnObstacleClass SpnMovement::ClassifyObstacle(const FSpnMovementSettings& Settings, float HeightCm)
{
	switch (spn::core::ClassifyObstacle(ToCore(Settings), HeightCm * CmToM))
	{
	case spn::core::ObstacleClass::StepUp: return ESpnObstacleClass::StepUp;
	case spn::core::ObstacleClass::JumpCandidate: return ESpnObstacleClass::JumpCandidate;
	default: return ESpnObstacleClass::Blocked;
	}
}

FVector2D SpnMovement::SteerVelocity(const FSpnMovementSettings& Settings, FVector2D VelocityCm, FVector2D Direction, float TargetSpeedCm, float AccelerationCm, float DeltaSeconds)
{
	spn::core::SteeringInput Input;
	Input.direction = {Direction.X, Direction.Y};
	Input.targetSpeed = TargetSpeedCm * CmToM;
	Input.acceleration = AccelerationCm * CmToM;
	Input.braking = Settings.BrakingDeceleration * CmToM;
	const spn::core::Vec2 Next = spn::core::SteerVelocity(ToSteering(Settings), {VelocityCm.X * CmToM, VelocityCm.Y * CmToM}, Input, DeltaSeconds);
	return FVector2D(Next.x / CmToM, Next.y / CmToM);
}
