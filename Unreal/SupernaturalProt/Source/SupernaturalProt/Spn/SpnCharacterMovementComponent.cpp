#include "Spn/SpnCharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "SupernaturalProt.h"

namespace
{
	// Compressed move flags carrying the gait request (FLAG_Custom_0..3 are free for game use).
	constexpr uint8 FlagSprint = FSavedMove_Character::FLAG_Custom_0;
	constexpr uint8 FlagBaseGaitBit0 = FSavedMove_Character::FLAG_Custom_1;
	constexpr uint8 FlagBaseGaitBit1 = FSavedMove_Character::FLAG_Custom_2;

	ESpnGait SanitizeBaseGait(ESpnGait Gait)
	{
		return Gait == ESpnGait::Sprint ? ESpnGait::Run : Gait;
	}
}

USpnCharacterMovementComponent::USpnCharacterMovementComponent()
{
	// Placeholder: the defaults of FSpnMovementSettings are the M2-A test values. Nothing here ticks.
	BaseGait = SanitizeBaseGait(Settings.DefaultGait);
}

void USpnCharacterMovementComponent::SetBaseGait(ESpnGait NewGait)
{
	BaseGait = SanitizeBaseGait(NewGait);
}

void USpnCharacterMovementComponent::SetSprintRequested(bool bRequested)
{
	bSprintRequested = bRequested;
}

void USpnCharacterMovementComponent::CycleBaseGait()
{
	switch (BaseGait)
	{
	case ESpnGait::Walk: BaseGait = ESpnGait::Jog; break;
	case ESpnGait::Jog: BaseGait = ESpnGait::Run; break;
	default: BaseGait = ESpnGait::Walk; break;
	}
}

void USpnCharacterMovementComponent::SetMovementLimits(ESpnGait NewMaxGait, float NewSpeedScale)
{
	MaxAllowedGait = NewMaxGait;
	SpeedScale = FMath::Clamp(NewSpeedScale, 0.01f, 1.f);
}

FVector2D USpnCharacterMovementComponent::GetLocalInput() const
{
	if (UpdatedComponent == nullptr || Acceleration.IsNearlyZero())
	{
		return FVector2D::ZeroVector;
	}
	const FVector Local = UpdatedComponent->GetComponentQuat().UnrotateVector(Acceleration.GetSafeNormal2D());
	return FVector2D(Local.X, Local.Y);
}

ESpnGait USpnCharacterMovementComponent::GetEffectiveGait() const
{
	// The velocity is part of the predicted state, so the sprint-sustain test (a wider angle once at sprint speed)
	// gives the same answer on the client and on the server.
	return SpnMovement::ResolveGait(Settings, BaseGait, bSprintRequested, MaxAllowedGait, GetLocalInput(), Velocity.Size2D());
}

ESpnGait USpnCharacterMovementComponent::GetObservedGait() const
{
	const float Speed = Velocity.Size2D();
	if (Speed < Settings.IdleSpeed)
	{
		return BaseGait;
	}
	return SpnMovement::ObservedGait(Settings, Speed);
}

float USpnCharacterMovementComponent::GetSecondsSinceLanding() const
{
	const UWorld* World = GetWorld();
	if (World == nullptr || LastLandedTime < 0.0)
	{
		return -1.f;
	}
	return static_cast<float>(World->GetTimeSeconds() - LastLandedTime);
}

ESpnMovementState USpnCharacterMovementComponent::GetMovementState() const
{
	// Anything that is not falling counts as ground contact (walking, nav walking; swimming and flying do not exist yet).
	return SpnMovement::ClassifyMotion(Settings, Velocity.Size2D(), Velocity.Z, !IsFalling(), GetSecondsSinceLanding());
}

void USpnCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (PreviousMovementMode == MOVE_Falling && IsMovingOnGround() && GetWorld() != nullptr)
	{
		LastLandedTime = GetWorld()->GetTimeSeconds();
	}
}

void USpnCharacterMovementComponent::ApplySettings()
{
	FString Error;
	if (!SpnMovement::Validate(Settings, Error))
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("'%s': invalid movement settings, keeping the previous values: %s"), *GetNameSafe(GetOwner()), *Error);
		return;
	}

	// Values the engine reads directly. Speed and acceleration of walking come from the overrides below;
	// MaxWalkSpeed / MaxAcceleration are kept equal to the default gait for code that reads them.
	MaxWalkSpeed = SpnMovement::GaitSpeed(Settings, Settings.DefaultGait);
	MaxAcceleration = SpnMovement::GaitAcceleration(Settings, Settings.DefaultGait);
	BrakingDecelerationWalking = Settings.BrakingDeceleration;
	// Constant-deceleration stopping: the braking friction would add an exponential term on top.
	bUseSeparateBrakingFriction = true;
	BrakingFriction = 0.f;
	GroundFriction = Settings.TurnFriction;
	RotationRate = FRotator(0.f, Settings.TurnRateDegrees, 0.f);
	MinAnalogWalkSpeed = Settings.MinAnalogSpeed;
	// Independent of the jump height: the step height only decides which ledges are walked up, JumpZVelocity only the apex.
	MaxStepHeight = Settings.MaxStepHeight;
	AirControl = Settings.AirControl;
	BrakingDecelerationFalling = Settings.AirBrakingDeceleration;
	RefreshJumpVelocity();
}

void USpnCharacterMovementComponent::RefreshJumpVelocity()
{
	if (GetWorld() == nullptr)
	{
		return; // no world, no gravity: class default objects and detached components keep their JumpZVelocity
	}
	// GetGravityZ is negative (downwards) and already includes GravityScale.
	const float Gravity = -GetGravityZ();
	const float LaunchVelocity = SpnMovement::JumpVelocityForHeight(Settings.JumpHeight, Gravity);
	if (LaunchVelocity > 0.f)
	{
		JumpZVelocity = LaunchVelocity;
	}
	else
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("'%s': cannot derive a jump velocity (jump height %.1f cm, gravity %.1f cm/s^2); keeping JumpZVelocity %.1f"),
			*GetNameSafe(GetOwner()), Settings.JumpHeight, Gravity, JumpZVelocity);
	}
}

void USpnCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	// After Blueprint overrides were loaded, so Settings win over a stale JumpZVelocity / MaxWalkSpeed in an asset.
	BaseGait = SanitizeBaseGait(Settings.DefaultGait);
	ApplySettings();
}

void USpnCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	// The physics volume (gravity) is known now.
	RefreshJumpVelocity();
}

#if WITH_EDITOR
void USpnCharacterMovementComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(USpnCharacterMovementComponent, Settings) && GetWorld() != nullptr)
	{
		ApplySettings();
	}
}
#endif

float USpnCharacterMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		{
			if (IsCrouching())
			{
				return MaxWalkSpeedCrouched;
			}
			float Speed = SpnMovement::GaitSpeed(Settings, GetEffectiveGait()) * SpeedScale;
			if (!bOrientRotationToMovement)
			{
				// The character keeps facing one way (aiming, lock-on): going backwards or sideways is slower.
				Speed *= SpnMovement::DirectionSpeedScale(Settings, GetLocalInput());
			}
			return Speed;
		}
	case MOVE_Falling:
		{
			// Keep the take-off momentum in the air: never a limit below the current speed (the base class would
			// brake a sprint down to MaxWalkSpeed at once), and no gain beyond the gait's top speed.
			const float GaitLimit = SpnMovement::GaitSpeed(Settings, GetEffectiveGait()) * SpeedScale;
			return FMath::Max(GaitLimit, Velocity.Size2D());
		}
	default:
		return Super::GetMaxSpeed();
	}
}

float USpnCharacterMovementComponent::GetMaxAcceleration() const
{
	if (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking)
	{
		return SpnMovement::GaitAcceleration(Settings, GetEffectiveGait());
	}
	return Super::GetMaxAcceleration();
}

FNetworkPredictionData_Client* USpnCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		// Same pattern as the engine: allocate once, keep it in the mutable base pointer.
		USpnCharacterMovementComponent* MutableThis = const_cast<USpnCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_SpnCharacter(*this);
	}
	return ClientPredictionData;
}

void USpnCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	// Server side: the client's request. Untrusted input: the value is range-limited here, and the
	// gait actually used is resolved from the server's own limits in GetEffectiveGait().
	bSprintRequested = (Flags & FlagSprint) != 0;
	const uint8 Encoded = static_cast<uint8>(((Flags & FlagBaseGaitBit0) != 0 ? 1 : 0) | ((Flags & FlagBaseGaitBit1) != 0 ? 2 : 0));
	BaseGait = SanitizeBaseGait(static_cast<ESpnGait>(Encoded));
}

bool USpnCharacterMovementComponent::ClientUpdatePositionAfterServerUpdate()
{
	// PrepMoveFor writes the request of each replayed move into the live state. The engine restores its own live inputs
	// (bWantsToCrouch, bPressedJump) after the replay; the SPN requests need the same treatment, otherwise a correction that
	// arrives right after a key press leaves the older request behind and the press is lost (found with NetworkSprint).
	const bool bRealSprintRequested = bSprintRequested;
	const ESpnGait RealBaseGait = BaseGait;
	const bool bResult = Super::ClientUpdatePositionAfterServerUpdate();
	bSprintRequested = bRealSprintRequested;
	BaseGait = RealBaseGait;
	return bResult;
}

FSavedMove_SpnCharacter::FSavedMove_SpnCharacter()
	: SavedBaseGait(static_cast<uint8>(ESpnGait::Jog))
	, bSavedSprintRequested(false)
{
}

void FSavedMove_SpnCharacter::Clear()
{
	Super::Clear();
	SavedBaseGait = static_cast<uint8>(ESpnGait::Jog);
	bSavedSprintRequested = false;
}

uint8 FSavedMove_SpnCharacter::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if (bSavedSprintRequested)
	{
		Result |= FlagSprint;
	}
	if ((SavedBaseGait & 1) != 0)
	{
		Result |= FlagBaseGaitBit0;
	}
	if ((SavedBaseGait & 2) != 0)
	{
		Result |= FlagBaseGaitBit1;
	}
	return Result;
}

bool FSavedMove_SpnCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	// Moves with a different gait request must not be merged: the merged move would carry only one of them.
	const FSavedMove_SpnCharacter* Other = static_cast<const FSavedMove_SpnCharacter*>(NewMove.Get());
	if (SavedBaseGait != Other->SavedBaseGait || bSavedSprintRequested != Other->bSavedSprintRequested)
	{
		return false;
	}
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void FSavedMove_SpnCharacter::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	if (const USpnCharacterMovementComponent* Movement = Cast<USpnCharacterMovementComponent>(Character->GetCharacterMovement()))
	{
		SavedBaseGait = static_cast<uint8>(Movement->BaseGait);
		bSavedSprintRequested = Movement->bSprintRequested;
	}
}

void FSavedMove_SpnCharacter::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	// Replaying after a correction: use the gait request this move was made with.
	if (USpnCharacterMovementComponent* Movement = Cast<USpnCharacterMovementComponent>(Character->GetCharacterMovement()))
	{
		Movement->BaseGait = static_cast<ESpnGait>(SavedBaseGait);
		Movement->bSprintRequested = bSavedSprintRequested;
	}
}

FNetworkPredictionData_Client_SpnCharacter::FNetworkPredictionData_Client_SpnCharacter(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_SpnCharacter::AllocateNewMove()
{
	return MakeShared<FSavedMove_SpnCharacter>();
}
