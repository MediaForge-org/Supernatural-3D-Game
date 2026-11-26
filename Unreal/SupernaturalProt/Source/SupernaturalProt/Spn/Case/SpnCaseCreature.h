#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpnCreature.h"
#include "SpnNpcReaction.h"
#include "SpnCaseCreature.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;
class UMaterialInstanceDynamic;
struct FSpnDifficultyProfile;

/** One state change for tests and the debug log. */
struct FSpnCreatureLogEntry
{
	double WorldTime = 0.0;
	ESpnCreatureState From = ESpnCreatureState::Patrol;
	ESpnCreatureState To = ESpnCreatureState::Patrol;
};

/**
 * PLACEHOLDER creature of the first case (NightBuild07, Docs/Proposals/0019): blind, hunts by sound, stunned by the mill bell. All decisions are
 * made by the Core CreatureBrain (Core/include/spn/core/Creature.h); this actor only senses (gameplay noises through USpnNoiseSubsystem, walls
 * counted like ASpnNpc does, the prey's distance for contact and catch) and moves (AIController::MoveToLocation, straight-line fallback without
 * a navigation mesh). It makes no footstep noise of its own.
 *
 * Not a creature design: an engine-shape body (dark, tall), a debug label, no animation, no sound. The developer makes the real creature later.
 * Volatile: nothing is saved; after a death / load it goes back to its spawn and its patrol.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnCreature : public ACharacter
{
	GENERATED_BODY()

public:
	ASpnCreature();

	/** Patrol points (world cm), walked in order with a short wait. Empty: it waits at its spawn. */
	UPROPERTY(EditAnywhere, Category = "Spn|Creature")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Spn|Creature", meta = (ClampMin = "0"))
	float PatrolWaitSeconds = 4.f;

	UPROPERTY(EditAnywhere, Category = "Spn|Creature", meta = (ClampMin = "10"))
	float ProwlSpeedCm = 180.f;

	/** Pursuit speed: below the player's sprint (800) and above the run, so sprinting away works but is loud. */
	UPROPERTY(EditAnywhere, Category = "Spn|Creature", meta = (ClampMin = "10"))
	float HuntSpeedCm = 560.f;

	UPROPERTY(EditAnywhere, Category = "Spn|Creature", meta = (ClampMin = "0.1"))
	float HearingSensitivity = 1.f;

	/** Range factor per solid wall (closed door) between a noise and the creature. */
	UPROPERTY(EditAnywhere, Category = "Spn|Creature", meta = (ClampMin = "0", ClampMax = "1"))
	float WallAttenuation = 0.5f;

	/** Damage of a catch; the default kills (no injury model yet). */
	UPROPERTY(EditAnywhere, Category = "Spn|Creature", meta = (ClampMin = "0"))
	float CatchDamage = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Spn|Creature")
	bool bShowDebugLabel = true;

	ESpnCreatureState GetCreatureState() const;
	bool IsListening() const { return bListening; }
	bool IsWary() const;
	uint64 GetTransitionCount() const;
	const TArray<FSpnCreatureLogEntry>& GetLog() const { return Log; }
	int32 GetNoisesHeard() const { return NoisesHeard; }
	int32 GetNoisesNotHeard() const { return NoisesNotHeard; }
	int32 GetCatches() const { return Catches; }
	FVector GetGoalCm() const { return LastGoalCm; }
	FVector GetSpawnCm() const { return SpawnCm; }

	/** The bell (USpnCaseSubsystem::RingBell). */
	bool StunByBell();
	/** Back to the spawn and the patrol (after a death / load). */
	void ResetToSpawn();
	/** Frozen: no sensing, no decisions, no movement (while the player is dead). */
	void SetFrozen(bool bFrozen);
	/** The values of a difficulty profile (reaction delay, search time, hearing). Rebuilds the brain (state back to Patrol). */
	void ApplyDifficulty(const FSpnDifficultyProfile& Profile);

	/** Closed obstacles between two points (closed doors and walls count, open doors do not). */
	int32 CountWallsBetween(const FVector& From, const FVector& To) const;
	FVector GetEarLocation() const;

	/** The pawn it hunts. Default: the case subsystem's player. Tests set it. */
	void SetPrey(APawn* Prey) { PreyOverride = Prey; }
	APawn* ResolvePrey() const;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void HandleNoise(const FSpnNoise& Noise);
	void Apply(const FSpnCreatureOutput& Out, float DeltaSeconds);
	void MoveTowards(const FVector& GoalCm);
	void StopMoving();
	void RunPatrol(float DeltaSeconds);
	void UpdateVisuals();
	FVector PickSearchPoint(const FVector& Centre, float Radius);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Head;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTextRenderComponent> Label;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> BodyMaterial;

	TUniquePtr<FSpnCreatureBrain> Brain;
	FSpnCreatureTuning Tuning;
	TWeakObjectPtr<APawn> PreyOverride;
	FDelegateHandle NoiseHandle;

	bool bHasPendingNoise = false;
	FVector PendingNoiseCm = FVector::ZeroVector;
	float PendingStrength = 0.f;
	float PendingDistanceCm = 0.f;

	bool bFrozen = false;
	bool bNotedLevelStart = false;
	bool bListening = false;
	FVector SpawnCm = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FVector LastGoalCm = FVector::ZeroVector;
	FVector MoveGoalCm = FVector::ZeroVector;
	bool bHasMoveGoal = false;
	bool bFallbackSteering = false;
	FVector SearchPointCm = FVector::ZeroVector;
	bool bHasSearchPoint = false;
	int32 PatrolIndex = 0;
	float PatrolWait = 0.f;
	FRandomStream Random;
	TArray<FSpnCreatureLogEntry> Log;
	int32 NoisesHeard = 0;
	int32 NoisesNotHeard = 0;
	int32 Catches = 0;
	uint64 LastNoiseSequence = 0;
};
