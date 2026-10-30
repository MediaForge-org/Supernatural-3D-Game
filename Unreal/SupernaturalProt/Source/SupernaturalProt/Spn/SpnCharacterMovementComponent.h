#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpnMovement.h"
#include "SpnCharacterMovementComponent.generated.h"

/**
 * On-foot movement with four gaits and a configurable jump height (M2-A, Docs/Proposals/0011).
 *
 * Gaits are part of the client prediction: the chosen base gait and the sprint request travel in the
 * compressed move flags (FLAG_Custom_0..2), so the server and the owning client run the same speed
 * rule for every move and replay saved moves with the gait they were made with. Speed and acceleration
 * come from GetMaxSpeed / GetMaxAcceleration, never from changing MaxWalkSpeed at runtime. The server
 * resolves the gait itself from its own limits and the move's input; a client can only ask.
 *
 * No tick of its own. All numbers are PLACEHOLDER TEST VALUES (FSpnMovementSettings).
 * NOT YET COMPILED OR RUN IN UNREAL (written on Fedora without an engine); see Docs/Setup/Windows-M2A-Handover.md.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class USpnCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USpnCharacterMovementComponent();

	/** Tunable movement numbers; applied at InitializeComponent and by ApplySettings(). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Movement")
	FSpnMovementSettings Settings;

	//~ Requests (call on the owning client; the server gets them through the move flags, or call on the authority for AI)

	/** Gait for normal movement. Walk, Jog or Run; Sprint is treated as Run. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Movement")
	void SetBaseGait(ESpnGait NewGait);

	UFUNCTION(BlueprintCallable, Category = "Spn|Movement")
	void SetSprintRequested(bool bRequested);

	/** Next base gait: Walk -> Jog -> Run -> Walk. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Movement")
	void CycleBaseGait();

	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	ESpnGait GetBaseGait() const { return BaseGait; }

	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	bool IsSprintRequested() const { return bSprintRequested; }

	//~ Limits (prepared for exhaustion, injury and load; nothing sets them in M2-A)

	/**
	 * Ceiling for the gait and a speed multiplier in (0, 1]. NOT replicated yet: whoever sets it must set it
	 * identically on the server and the owning client (from replicated game state), or moves get corrected.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|Movement")
	void SetMovementLimits(ESpnGait NewMaxGait, float NewSpeedScale);

	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	ESpnGait GetMaxAllowedGait() const { return MaxAllowedGait; }

	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	float GetSpeedScale() const { return SpeedScale; }

	//~ Queries

	/** The gait in use right now: requested gait resolved against limits and the current input direction. Authority and owning client. */
	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	ESpnGait GetEffectiveGait() const;

	/** The gait a character is moving at, judged from its horizontal speed alone. Works on every proxy (animation). */
	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	ESpnGait GetObservedGait() const;

	/**
	 * Idle, Walk, Jog, Run, Sprint, Jumping, Falling or Landing, from velocity, floor contact and the time of the last touch-down.
	 * Computed on demand, nothing is stored or replicated, so it works on every proxy. The interface for animation blueprints
	 * (read this instead of keeping a second state machine). Landing needs the movement-mode change to reach the machine
	 * (see OnMovementModeChanged); on simulated proxies that is not verified in Unreal yet.
	 */
	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	ESpnMovementState GetMovementState() const;

	/** Seconds since the character last touched down after a fall or jump; negative if it has not (yet). */
	UFUNCTION(BlueprintPure, Category = "Spn|Movement")
	float GetSecondsSinceLanding() const;

	/** Re-applies Settings to the Character Movement values it drives (braking, friction, step height, jump). Logs an error and keeps the old values if Settings are invalid. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Movement")
	void ApplySettings();

	/** Sets JumpZVelocity so a jump reaches Settings.JumpHeight under the current gravity. Call after changing gravity or GravityScale. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Movement")
	void RefreshJumpVelocity();

	//~ UCharacterMovementComponent
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
	/** Ground steering of the SPN settings (Core SteerVelocity) instead of the engine's friction blend; see FSpnMovementSettings::bUseSteering. */
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

	/** Adds the engine's slope-boosting guard to walking (see FSpnMovementSettings::bPreventGroundSlopeBoosting). */
	virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	/** Replaying saved moves after a server correction must not leave the gait request of the last replayed move behind. */
	virtual bool ClientUpdatePositionAfterServerUpdate() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	// --- movement trace (diagnostics only): console variable spn.MoveTrace 1. Off by default; off costs one cvar read per call. ---
	/** True while spn.MoveTrace is not 0. */
	static bool IsMoveTraceEnabled();
	/** The recorded trace lines since the last call (one line per engine event), and clears them. */
	TArray<FString> ConsumeMoveTrace();
	virtual void MoveAlongFloor(const FVector& InVelocity, float DeltaSeconds, FStepDownResult* OutStepDownResult = NULL) override;
	virtual bool StepUp(const FVector& GravDir, const FVector& Delta, const FHitResult& Hit, FStepDownResult* OutStepDownResult = NULL) override;
	virtual void AdjustFloorHeight() override;
	virtual float SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact) override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
	/** Applies FSpnMovementSettings::bLimitFloorRiseToStepHeight after the engine's floor check. */
	virtual void FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult = NULL) const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	friend class FSavedMove_SpnCharacter;

protected:
	/** Chosen gait for normal movement (never Sprint). Predicted: travels in the move flags. */
	ESpnGait BaseGait = ESpnGait::Jog;

	/** Sprint held. Predicted: travels in the move flags. */
	bool bSprintRequested = false;

	/**
	 * Wall contact of the last ground move (set when a walking move slid along a non-walkable surface, consumed by the next CalcVelocity). The steering
	 * then only follows the part of the input that runs along that wall: pressed straight into a wall the character stops instead of building up
	 * sideways speed (measured: 133 cm/s along the wall after a 30 cm/s nudge, the intermittent ledge_80 failure). Derived from the move's own collision,
	 * so a replayed move recomputes it.
	 */
	FVector2D PendingWallNormal = FVector2D::ZeroVector;
	bool bPendingWallContact = false;

	/** Movement trace lines (only filled while spn.MoveTrace is on; bounded). */
	TArray<FString> MoveTraceLines;
	void AddMoveTrace(FString&& Line);

	ESpnGait MaxAllowedGait = ESpnGait::Sprint;
	float SpeedScale = 1.f;

	/** World time of the last touch-down (from the movement-mode change falling -> ground); negative if none yet. Not replicated. */
	double LastLandedTime = -1.0;

	/** Input direction in the character's frame (X forward, Y right, unit length or zero), from the current Acceleration. */
	FVector2D GetLocalInput() const;
};

/** One recorded move with the gait request it was made with, for replay after a server correction. */
class FSavedMove_SpnCharacter : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;

	FSavedMove_SpnCharacter();

	uint8 SavedBaseGait;
	uint8 bSavedSprintRequested : 1;

	virtual void Clear() override;
	virtual uint8 GetCompressedFlags() const override;
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
	virtual void PrepMoveFor(ACharacter* Character) override;
};

class FNetworkPredictionData_Client_SpnCharacter : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	explicit FNetworkPredictionData_Client_SpnCharacter(const UCharacterMovementComponent& ClientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};
