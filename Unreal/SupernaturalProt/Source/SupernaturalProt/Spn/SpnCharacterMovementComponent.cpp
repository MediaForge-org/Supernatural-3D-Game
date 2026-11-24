#include "Spn/SpnCharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "HAL/IConsoleManager.h"
#include "SupernaturalProt.h"

namespace
{
	TAutoConsoleVariable<int32> CVarSpnMoveTrace(TEXT("spn.MoveTrace"), 0,
		TEXT("Diagnostics: record one line per walking/falling engine event (MoveAlongFloor, StepUp, AdjustFloorHeight, SlideAlongSurface, PhysFalling) of SPN characters. 0 = off."));

	FString HitText(const FHitResult& Hit)
	{
		return FString::Printf(TEXT("hit t=%.3f n=(%.2f,%.2f,%.2f) in=(%.2f,%.2f,%.2f) ip=(%.1f,%.1f,%.1f) pen=%d"), Hit.Time, Hit.Normal.X, Hit.Normal.Y, Hit.Normal.Z,
			Hit.ImpactNormal.X, Hit.ImpactNormal.Y, Hit.ImpactNormal.Z, Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z, Hit.bStartPenetrating ? 1 : 0);
	}

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
	// Anything that is not falling counts as ground contact (walking, nav walking; swimming and flying do not exist yet). A ledge assist move is
	// off the ground (it reports Jumping while it rises).
	return SpnMovement::ClassifyMotion(Settings, Velocity.Size2D(), Velocity.Z, !IsFalling() && !bLedgeActive, GetSecondsSinceLanding());
}

void USpnCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (bLedgeActive && !(MovementMode == MOVE_Custom && CustomMovementMode == CustomModeLedgeAssist))
	{
		// something else changed the mode while the move ran (teleport, load, a script): release the move cleanly
		FinishLedgeAssist(false, TEXT("movement mode changed from outside"));
	}
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
	// Independent of the jump height: the step height only decides which ledges are walked up, JumpZVelocity only the apex.
	MaxStepHeight = Settings.MaxStepHeight;
	PerchRadiusThreshold = Settings.PerchRadiusThreshold;
	bUseFlatBaseForFloorChecks = Settings.bUseFlatBaseForFloorChecks;
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

void USpnCharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	// the wall contact of the previous move is used (and consumed) by this velocity update only
	const bool bWallContact = bPendingWallContact;
	const FVector2D WallNormal = PendingWallNormal;
	bPendingWallContact = false;

	// Only the plain walking case: root motion, AI requested velocity, crouching, strafing (no orientation to movement) and every other mode keep the engine path.
	if (!Settings.bUseSteering || MovementMode != MOVE_Walking || !bOrientRotationToMovement || bHasRequestedVelocity || HasAnimRootMotion()
		|| CurrentRootMotion.HasOverrideVelocity() || IsCrouching() || DeltaTime < MIN_TICK_TIME)
	{
		Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
		return;
	}

	// Depends only on Velocity, Acceleration, DeltaTime and the wall contact of the previous move, so a client replay after a correction gives the same result.
	FVector2D Direction = FVector2D(Acceleration.X, Acceleration.Y).GetSafeNormal();
	float TargetSpeed = 0.f;
	if (!Direction.IsNearlyZero())
	{
		const float Analog = ComputeAnalogInputModifier();
		TargetSpeed = FMath::Max(MinAnalogWalkSpeed, GetMaxSpeed() * Analog);
	}
	if (bWallContact && !Direction.IsNearlyZero())
	{
		const float Into = FVector2D::DotProduct(Direction, WallNormal);
		if (Into < 0.f)
		{
			// Pressed into a wall: only the part of the wanted direction that runs along the wall can move the character (the engine's own walking does
			// the same through its slide). Straight into the wall leaves nothing: the steering brakes instead of turning a slide into sideways speed.
			const FVector2D Along = Direction - Into * WallNormal;
			TargetSpeed *= Along.Size();
			Direction = Along.Size() > UE_KINDA_SMALL_NUMBER ? Along / Along.Size() : FVector2D::ZeroVector;
		}
	}
	const FVector2D Next = SpnMovement::SteerVelocity(Settings, FVector2D(Velocity.X, Velocity.Y), Direction, TargetSpeed, GetMaxAcceleration(), DeltaTime);
	Velocity.X = Next.X;
	Velocity.Y = Next.Y;
}

FVector USpnCharacterMovementComponent::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	FVector Result = Super::ComputeSlideVector(Delta, Time, Normal, Hit);
	// The engine prevents "slope boosting" (sliding UP along a non-walkable surface) only while FALLING (CharacterMovementComponent.cpp, ComputeSlideVector).
	// This applies the same rule on the ground. M2A4.1 measured NO effect of it on the 30 cm walking cases (they are still crossed, see the regression report):
	// it is kept only as parity with the falling rule and is the first candidate for removal when the 30 cm question is resolved.
	if (Settings.bPreventGroundSlopeBoosting && IsMovingOnGround())
	{
		Result = HandleSlopeBoosting(Result, Delta, Time, Normal, Hit);
	}
	return Result;
}

bool USpnCharacterMovementComponent::IsMoveTraceEnabled()
{
	return CVarSpnMoveTrace.GetValueOnGameThread() != 0;
}

TArray<FString> USpnCharacterMovementComponent::ConsumeMoveTrace()
{
	TArray<FString> Out = MoveTemp(MoveTraceLines);
	MoveTraceLines.Reset();
	return Out;
}

void USpnCharacterMovementComponent::AddMoveTrace(FString&& Line)
{
	if (MoveTraceLines.Num() < 20000)
	{
		const UWorld* World = GetWorld();
		MoveTraceLines.Add(FString::Printf(TEXT("%.4f %s"), World != nullptr ? World->GetTimeSeconds() : 0.0, *Line));
	}
}

void USpnCharacterMovementComponent::MoveAlongFloor(const FVector& InVelocity, float DeltaSeconds, FStepDownResult* OutStepDownResult)
{
	if (!IsMoveTraceEnabled())
	{
		Super::MoveAlongFloor(InVelocity, DeltaSeconds, OutStepDownResult);
		return;
	}
	const FVector Before = UpdatedComponent->GetComponentLocation();
	AddMoveTrace(FString::Printf(TEXT("MAF begin z=%.2f x=%.2f v=%.1f dt=%.4f floor{walk=%d dist=%.2f line=%d ln=%.2f n=(%.2f,%.2f,%.2f)}"), Before.Z, Before.X, InVelocity.Size2D(), DeltaSeconds,
		CurrentFloor.IsWalkableFloor() ? 1 : 0, CurrentFloor.FloorDist, CurrentFloor.bLineTrace ? 1 : 0, CurrentFloor.LineDist, CurrentFloor.HitResult.ImpactNormal.X,
		CurrentFloor.HitResult.ImpactNormal.Y, CurrentFloor.HitResult.ImpactNormal.Z));
	Super::MoveAlongFloor(InVelocity, DeltaSeconds, OutStepDownResult);
	const FVector After = UpdatedComponent->GetComponentLocation();
	AddMoveTrace(FString::Printf(TEXT("MAF end z=%.2f x=%.2f dz=%.2f mode=%d stepdownfloor=%d"), After.Z, After.X, After.Z - Before.Z, static_cast<int32>(MovementMode),
		OutStepDownResult != nullptr && OutStepDownResult->bComputedFloor ? 1 : 0));
}

bool USpnCharacterMovementComponent::StepUp(const FVector& GravDir, const FVector& Delta, const FHitResult& Hit, FStepDownResult* OutStepDownResult)
{
	if (!IsMoveTraceEnabled())
	{
		return Super::StepUp(GravDir, Delta, Hit, OutStepDownResult);
	}
	const FVector Before = UpdatedComponent->GetComponentLocation();
	const bool bResult = Super::StepUp(GravDir, Delta, Hit, OutStepDownResult);
	const FVector After = UpdatedComponent->GetComponentLocation();
	AddMoveTrace(FString::Printf(TEXT("STEPUP %s z %.2f -> %.2f x %.2f -> %.2f %s"), bResult ? TEXT("OK") : TEXT("REFUSED"), Before.Z, After.Z, Before.X, After.X, *HitText(Hit)));
	return bResult;
}

void USpnCharacterMovementComponent::AdjustFloorHeight()
{
	if (!IsMoveTraceEnabled())
	{
		Super::AdjustFloorHeight();
		return;
	}
	const FVector Before = UpdatedComponent->GetComponentLocation();
	const FFindFloorResult Floor = CurrentFloor;
	Super::AdjustFloorHeight();
	const FVector After = UpdatedComponent->GetComponentLocation();
	AddMoveTrace(FString::Printf(TEXT("ADJUST z %.2f -> %.2f (dz %.2f) floor{walk=%d dist=%.2f line=%d ln=%.2f %s}"), Before.Z, After.Z, After.Z - Before.Z, Floor.IsWalkableFloor() ? 1 : 0,
		Floor.FloorDist, Floor.bLineTrace ? 1 : 0, Floor.LineDist, *HitText(Floor.HitResult)));
}

float USpnCharacterMovementComponent::SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact)
{
	if (IsMovingOnGround() && Hit.bBlockingHit && !IsWalkable(Hit))
	{
		const FVector2D Wall = FVector2D(Hit.Normal.X, Hit.Normal.Y).GetSafeNormal();
		if (!Wall.IsNearlyZero())
		{
			PendingWallNormal = Wall;
			bPendingWallContact = true;
		}
	}
	if (!IsMoveTraceEnabled())
	{
		return Super::SlideAlongSurface(Delta, Time, Normal, Hit, bHandleImpact);
	}
	const FVector Before = UpdatedComponent->GetComponentLocation();
	const FString In = HitText(Hit);
	const float Result = Super::SlideAlongSurface(Delta, Time, Normal, Hit, bHandleImpact);
	const FVector After = UpdatedComponent->GetComponentLocation();
	AddMoveTrace(FString::Printf(TEXT("SLIDE mode=%d z %.2f -> %.2f (dz %.2f) x %.2f -> %.2f N=(%.2f,%.2f,%.2f) %s"), static_cast<int32>(MovementMode), Before.Z, After.Z, After.Z - Before.Z,
		Before.X, After.X, Normal.X, Normal.Y, Normal.Z, *In));
	return Result;
}

void USpnCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	if (!IsMoveTraceEnabled())
	{
		Super::PhysFalling(deltaTime, Iterations);
		return;
	}
	const FVector Before = UpdatedComponent->GetComponentLocation();
	const FVector V = Velocity;
	Super::PhysFalling(deltaTime, Iterations);
	const FVector After = UpdatedComponent->GetComponentLocation();
	AddMoveTrace(FString::Printf(TEXT("FALL z %.2f -> %.2f x %.2f -> %.2f vz %.1f -> %.1f vxy %.1f -> %.1f modeAfter=%d"), Before.Z, After.Z, Before.X, After.X, V.Z, Velocity.Z, V.Size2D(), Velocity.Size2D(),
		static_cast<int32>(MovementMode)));
}

void USpnCharacterMovementComponent::FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult) const
{
	Super::FindFloor(CapsuleLocation, OutFloorResult, bCanUseCachedLocation, DownwardSweepResult);
	if (!Settings.bLimitFloorRiseToStepHeight || !IsMovingOnGround() || !OutFloorResult.IsWalkableFloor())
	{
		return;
	}
	// How far AdjustFloorHeight would lift the capsule to stand on this floor. A legitimate step is taken by StepUp, which moves the capsule up first,
	// so its floor is found at the normal hover distance; a rise above the step limit here is a floor the step rule never approved.
	const float AvgFloorDist = (MIN_FLOOR_DIST + MAX_FLOOR_DIST) * 0.5f;
	const float Rise = AvgFloorDist - OutFloorResult.GetDistanceToFloor();
	if (Rise <= MaxStepHeight)
	{
		return;
	}
	const float TraceDist = FMath::Max(MAX_FLOOR_DIST, MaxStepHeight + MAX_FLOOR_DIST + UE_KINDA_SMALL_NUMBER);
	FFindFloorResult Narrow;
	ComputeFloorDist(CapsuleLocation, TraceDist, TraceDist, Narrow, GetValidPerchRadius());
	const bool bNarrowOk = Narrow.IsWalkableFloor() && AvgFloorDist - Narrow.GetDistanceToFloor() <= MaxStepHeight;
	if (IsMoveTraceEnabled())
	{
		USpnCharacterMovementComponent* MutableThis = const_cast<USpnCharacterMovementComponent*>(this);
		MutableThis->AddMoveTrace(FString::Printf(TEXT("FLOORLIMIT rise %.2f > step %.1f at %s: narrow probe r=%.1f walk=%d dist=%.2f -> %s"), Rise, MaxStepHeight, *OutFloorResult.HitResult.ImpactPoint.ToString(),
			GetValidPerchRadius(), Narrow.IsWalkableFloor() ? 1 : 0, Narrow.GetDistanceToFloor(), bNarrowOk ? TEXT("use narrow floor") : TEXT("no walkable floor")));
	}
	if (bNarrowOk)
	{
		OutFloorResult = Narrow;
	}
	else
	{
		OutFloorResult.bWalkableFloor = false; // nothing walkable within the step limit under the capsule: the normal fall / ledge logic decides
	}
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
