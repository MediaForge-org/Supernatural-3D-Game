#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpnNpcMemorySubsystem.h"
#include "SpnNpcReaction.h"

#include "SpnNpc.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;
class UMaterialInstanceDynamic;
class USpnNoiseSubsystem;

/** One reaction transition for tests / the debug log. */
struct FSpnReactionLogEntry
{
	double WorldTime = 0.0;
	ESpnReactionState From = ESpnReactionState::Idle;
	ESpnReactionState To = ESpnReactionState::Idle;
	FString Why;
};

/**
 * PLACEHOLDER NPC (NightBuild05): one resident that goes about a routine, hears gameplay noises, sees the player with a line-of-sight check,
 * reacts (Idle -> Investigate -> Alert -> Flee -> ReturnToRoutine, all decisions by the Core NpcReactionBrain) and REMEMBERS (USpnNpcMemorySubsystem).
 *
 * Not a character design: engine-shape body, a floating debug label, no animation, no dialogue, no schedule beyond a patrol between authored points.
 * Navigation uses the engine's navigation mesh through AAIController::MoveToLocation; if the level has none (or no path exists) the NPC steers straight
 * at the goal and the log says so.
 *
 * Persistent: the memories (stable NpcId -> world.npcs in the save). Volatile: the reaction state, timers and goals; after a load or a level start the
 * state is derived from the memory (Investigate towards the last incident while an alarm is recent, otherwise Idle). Positions are not saved.
 * All numbers are placeholder gameplay parameters, not master canon.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnNpc : public ACharacter
{
	GENERATED_BODY()

public:
	ASpnNpc();

	/** Stable id in the NPC memory (1 < id < 2^62, unique, never reused). Keys the saved memories: changing it later orphans them. */
	UPROPERTY(EditAnywhere, Category = "Spn|NPC")
	int64 NpcId = 1001;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC")
	FString DisplayName = TEXT("Caretaker");

	UPROPERTY(EditAnywhere, Category = "Spn|NPC")
	ESpnNpcTier Tier = ESpnNpcTier::B;

	/** Patrol points (world cm). The NPC walks them in order and waits RoutineWaitSeconds at each; point 0 is also "home" for ReturnToRoutine. Empty = stands still at its spawn point. */
	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Routine")
	TArray<FVector> RoutinePoints;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Routine", meta = (ClampMin = "0"))
	float RoutineWaitSeconds = 3.f;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Senses", meta = (ClampMin = "0"))
	float SightRangeCm = 1800.f;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Senses", meta = (ClampMin = "1", ClampMax = "360"))
	float FieldOfViewDegrees = 120.f;

	/** Within this distance and with a clear line the player is noticed whatever the facing. */
	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Senses", meta = (ClampMin = "0"))
	float CloseSenseRangeCm = 150.f;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Senses", meta = (ClampMin = "0.1"))
	float HearingSensitivity = 1.f;

	/** Range factor per solid wall between a noise and the NPC. */
	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Senses", meta = (ClampMin = "0", ClampMax = "1"))
	float WallAttenuation = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Movement", meta = (ClampMin = "10"))
	float WalkSpeedCm = 150.f;

	UPROPERTY(EditAnywhere, Category = "Spn|NPC|Movement", meta = (ClampMin = "10"))
	float FleeSpeedCm = 430.f;

	/** Floating label with name and state (a development visualisation, not game UI). */
	UPROPERTY(EditAnywhere, Category = "Spn|NPC")
	bool bShowDebugLabel = true;

	// --- read access for tests, QA and tools
	ESpnReactionState GetReactionState() const;
	const TArray<FSpnReactionLogEntry>& GetReactionLog() const { return ReactionLog; }
	uint64 GetTransitionCount() const;
	bool SeesPlayer() const { return bSeesPlayer; }
	bool IsWary() const { return Wariness.bWary; }
	const FSpnWariness& GetWariness() const { return Wariness; }
	int32 GetNoisesHeard() const { return NoisesHeard; }
	int32 GetNoisesIgnored() const { return NoisesIgnored; }
	int32 GetNoisesNotHeard() const { return NoisesNotHeard; }
	int32 GetAlarmsRaised() const { return AlarmsRaised; }
	/** Patrol legs that reached their routine point / were given up after 8 s without progress (tests, diagnostics). */
	int32 GetPatrolLegsCompleted() const { return PatrolLegsCompleted; }
	int32 GetPatrolLegsGivenUp() const { return PatrolLegsGivenUp; }
	FVector GetHomeCm() const { return HomeCm; }
	bool IsUsingFallbackSteering() const { return bFallbackSteering; }
	bool HasNavigationMesh() const;

	/** The pawn this NPC watches for. Default: the first player pawn. Tests and later NPC-vs-NPC perception set it explicitly. */
	void SetPerceptionTarget(APawn* Target) { PerceptionTarget = Target; }
	APawn* ResolveTarget() const;

	/** Forces the sense update now (tests). */
	void SenseNow();

	/**
	 * Solid obstacles between a noise and a point (the hearing model's wall count). The door that made the noise is not counted.
	 * OutObstacles (optional) receives the names of the counted actors, for the log and diagnostics.
	 */
	int32 CountWallsBetween(const FVector& From, const FVector& To, const FString& IgnoreDoorObjectId, TArray<FString>* OutObstacles = nullptr) const;

	/** Where this NPC hears from (actor location + eye height). */
	FVector GetEarLocation() const;
	/** Re-reads the memory and puts the volatile state where the memory says (what a load does). */
	void RestoreFromMemory();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void HandleNoise(const FSpnNoise& Noise);
	void HandleMemoryReplaced();
	bool ComputeSeesPlayer(FVector& OutPlayerCm) const;
	void Apply(const FSpnReactionOutput& Out, float DeltaSeconds);
	void MoveTowards(const FVector& GoalCm, bool bSprint);
	void StopMoving();
	FVector PickFleePoint(const FVector& ThreatCm) const;
	void RunRoutine(float DeltaSeconds);
	void UpdateVisuals();
	void RefreshWariness();
	void RecordSighting();

	USpnNpcMemorySubsystem* GetMemory() const;
	USpnNoiseSubsystem* GetNoise() const;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Head;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTextRenderComponent> Label;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> BodyMaterial;

	TUniquePtr<FSpnReactionBrain> Brain;
	TArray<FSpnReactionLogEntry> ReactionLog;
	FSpnWariness Wariness;
	FDelegateHandle NoiseHandle;
	FDelegateHandle ReplacedHandle;

	// pending noise (the strongest one heard since the last brain update)
	bool bHasPendingNoise = false;
	FSpnNoise PendingNoise;
	uint64 LastHandledNoiseSequence = 0;

	TWeakObjectPtr<APawn> PerceptionTarget;
	bool bSeesPlayer = false;
	FVector LastPlayerCm = FVector::ZeroVector;
	float SenseTimer = 0.f;
	float WarinessTimer = 0.f;
	double LastSightingRecordTime = -1000.0;
	bool bSightingRecordedThisContact = false;

	FVector HomeCm = FVector::ZeroVector;
	FVector SpawnCm = FVector::ZeroVector;
	FVector MoveGoalCm = FVector::ZeroVector;
	bool bHasMoveGoal = false;
	bool bFallbackSteering = false;
	float FleeRepickTimer = 0.f;
	FVector FleeGoalCm = FVector::ZeroVector;

	int32 RoutineIndex = 0;
	float RoutineWait = 0.f;
	float RoutineLegTime = 0.f;                                      // seconds without progress on the current patrol leg
	float RoutineBestDistance = TNumericLimits<float>::Max();       // closest the NPC got to the leg's goal so far
	int32 PatrolLegsCompleted = 0;
	int32 PatrolLegsGivenUp = 0;
	bool bRoutineMoving = false;

	int32 NoisesHeard = 0;
	int32 NoisesIgnored = 0;
	int32 NoisesNotHeard = 0;
	int32 AlarmsRaised = 0;
	ESpnReactionState LastShownState = ESpnReactionState::Idle;
	bool bLabelShown = false;
};
