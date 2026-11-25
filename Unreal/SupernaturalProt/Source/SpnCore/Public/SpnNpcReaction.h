#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SpnNpcReaction.generated.h"

// Unreal wrappers of the Core NPC reaction model (Core/include/spn/core/NpcReaction.h, NightBuild05): gameplay noises, hearing and sight verdicts,
// the reaction state machine and the noise channel. Core types stay private (Decision 0006); this interface is Unreal types in CENTIMETRES.
// ENGINEERING PROPOSAL, not master canon: every number is a placeholder gameplay parameter.

UENUM()
enum class ESpnReactionState : uint8
{
	Idle,
	Investigate,
	Alert,
	Flee,
	ReturnToRoutine,
};
SPNCORE_API const TCHAR* LexToString(ESpnReactionState State);

UENUM()
enum class ESpnNoiseKind : uint8
{
	Footstep,
	DoorUse,
	DoorBreak,
	Impact,
	/** An NPC raising the alarm. */
	Shout,
};
SPNCORE_API const TCHAR* LexToString(ESpnNoiseKind Kind);

/** One gameplay noise: where, how far it carries, how strong (0..1) and what it is. Independent of any audio asset. */
struct FSpnNoise
{
	FVector LocationCm = FVector::ZeroVector;
	/** Audible radius in cm at listener sensitivity 1 and no walls. */
	float RangeCm = 0.f;
	/** 0..1: loudness / alarm; also the severity remembered by listeners. */
	float Strength = 0.f;
	ESpnNoiseKind Kind = ESpnNoiseKind::Footstep;
	/** The world object concerned (a door's StableId), empty if none. */
	FString ObjectId;
	/** Who caused it; informational only, NPCs do not learn it from the noise. */
	FString SourceId;
	/** Emitter-side sequence number assigned by USpnNoiseSubsystem (1-based), 0 before emission. */
	uint64 Sequence = 0;
};

namespace SpnNpcSense
{
	/** DoorBreak, Impact and Shout are suspicious; very loud harmless noises as well. */
	SPNCORE_API bool IsSuspicious(const FSpnNoise& Noise);

	/** Footstep loudness for a horizontal speed (spn::core::FootstepNoiseFor, placeholder bands; never suspicious on its own). False: silent. */
	SPNCORE_API bool FootstepNoise(float HorizontalSpeedCmS, bool bCrouched, float& OutRangeCm, float& OutStrength);

	/** Hearing verdict. `Walls` = number of solid obstacles between source and listener (the caller counts them); each halves the range by WallAttenuation. */
	SPNCORE_API bool CanHear(const FSpnNoise& Noise, const FVector& ListenerCm, int32 Walls, float Sensitivity, float WallAttenuation, float& OutDistanceCm, float& OutEffectiveRangeCm);

	/** Sight verdict: range, view cone and a CLEAR line (no exception when very close). FacingYawDegrees is the actor yaw. */
	SPNCORE_API bool CanSee(const FVector& EyeCm, float FacingYawDegrees, const FVector& TargetCm, bool bLineClear, float SightRangeCm, float FieldOfViewDegrees, float CloseRangeCm);
}

struct FSpnReactionInput
{
	float DeltaSeconds = 0.f;
	FVector SelfCm = FVector::ZeroVector;
	FVector HomeCm = FVector::ZeroVector;
	bool bSeesPlayer = false;
	FVector PlayerCm = FVector::ZeroVector;
	bool bHasNoise = false;
	FVector NoiseCm = FVector::ZeroVector;
	float NoiseSeverity = 0.f;
	bool bNoiseSuspicious = false;
	bool bWary = false;
};

struct FSpnReactionOutput
{
	ESpnReactionState State = ESpnReactionState::Idle;
	ESpnReactionState Previous = ESpnReactionState::Idle;
	bool bChanged = false;
	bool bRaiseAlarm = false;
	bool bHasGoal = false;
	FVector GoalCm = FVector::ZeroVector;
	bool bSprint = false;
	bool bFacePlayer = false;
	bool bFleeAway = false;
	FVector ThreatCm = FVector::ZeroVector;
};

/** The reaction state machine of ONE NPC (volatile: never saved). */
class SPNCORE_API FSpnReactionBrain
{
public:
	FSpnReactionBrain();
	~FSpnReactionBrain();
	FSpnReactionBrain(const FSpnReactionBrain&) = delete;
	FSpnReactionBrain& operator=(const FSpnReactionBrain&) = delete;

	FSpnReactionOutput Update(const FSpnReactionInput& Input);
	ESpnReactionState GetState() const;
	float GetTimeInState() const;
	uint64 GetTransitionCount() const;
	/** Sets the state without a transition (after a load); timers restart. */
	void Restore(ESpnReactionState State, const FVector& TargetCm);

private:
	struct FState;
	TPimplPtr<FState> State;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FSpnNoiseEmitted, const FSpnNoise&);

/**
 * The noise channel of the game instance. Emitters (doors, later footsteps, NPC shouts) call EmitNoise; listeners (NPCs) bind OnNoise. The noise is
 * published as GameEventType::NoiseEmitted on the Core GameEventBus (sequence numbers, re-entrancy-safe queue) and delivered from its listener.
 * A noise is an instantaneous event: it is not saved; what NPCs made of it (memories) is.
 */
UCLASS()
class SPNCORE_API USpnNoiseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USpnNoiseSubsystem();
	virtual ~USpnNoiseSubsystem() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Publishes the noise (sanitised: non-finite or non-positive range/strength is dropped). Returns its sequence number, 0 if dropped. */
	uint64 EmitNoise(FSpnNoise Noise);

	uint64 GetEmittedCount() const { return EmittedCount; }
	uint64 GetDroppedCount() const { return DroppedCount; }
	/** The most recent delivered noise (for tests and debugging). */
	const FSpnNoise& GetLastNoise() const { return LastNoise; }

	FSpnNoiseEmitted OnNoise;

private:
	struct FState;
	TPimplPtr<FState> State;
	uint64 EmittedCount = 0;
	uint64 DroppedCount = 0;
	FSpnNoise LastNoise;
};
