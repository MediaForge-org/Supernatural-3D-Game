#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpnRouteGuideSubsystem.generated.h"

class ASpnTestDoor;
class ASpnTestKey;
class ASpnWorldPickup;

/**
 * TECHNICAL PROTOTYPE of a task guidance for the SpnWorld test route (night build 2026-11-14), not a mission and not a quest system.
 * Active only in Lvl_SpnWorld. The step is DERIVED every 0.25 s from state the game already keeps and saves (key held, door of house B
 * unlocked, flashlight taken, player near the cabin), so it needs no save data and is right after a load:
 *   1 take the key in the open house   2 unlock the locked house   3 take the flashlight   4 follow the pine track to the cabin   -> done
 * It shows the step as an objective line under the minimap (SpnUi::SetObjective) and sets a map waypoint on the step's target through the
 * existing map system (route on the minimap), but never replaces a waypoint the player set. Completion at the cabin: one HUD note.
 */
UCLASS()
class SUPERNATURALPROT_API USpnRouteGuideSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return bActive; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USpnRouteGuideSubsystem, STATGROUP_Tickables); }

	/** 0..3 = steps 1..4, 4 = completed; -1 = inactive or the route objects are missing. */
	int32 GetStep() const { return Step; }
	bool IsCompleted() const { return bCompleted; }
	FText GetObjectiveText() const { return Objective; }
	/** Distance (cm) to the cabin door at which the route counts as finished. */
	static constexpr float CabinRadiusCm = 900.f;

private:
	void FindObjects();
	int32 ComputeStep(const FVector& PlayerLocation) const;
	void ApplyStep(int32 NewStep);
	void UpdateWaypoint(const AActor* Target, const FText& Label);

	bool bActive = false;
	bool bCompleted = false;
	int32 Step = -1;
	float Accumulator = 0.f;
	int64 GuideWaypointId = 0;
	FText Objective;

	TWeakObjectPtr<ASpnTestDoor> DoorA;
	TWeakObjectPtr<ASpnTestDoor> DoorB;
	TWeakObjectPtr<ASpnTestDoor> CabinDoor;
	TWeakObjectPtr<ASpnTestKey> Key;
	TWeakObjectPtr<ASpnWorldPickup> Flashlight;
};
