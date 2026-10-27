#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SpnMovementDiagnostics.generated.h"

/**
 * Opt-in movement recorder for the jump / step-up investigation (overnight work order M2-A.2, package D2). Attach it to a character;
 * it does nothing (no tick work, no log) until enabled with the property bEnabled or the console variable spn.MoveDiag 1.
 * While enabled it keeps the last MaxSamples frames in a ring buffer and writes ONE compact CSV block to the log (category
 * LogSupernaturalProt, prefix "SpnMoveDiag,") when it is stopped or, with bDumpOnLanding, on every landing. No permanent logging.
 *
 * Columns: time, x, y, z, vx, vy, vz, mode, floorDist, walkableFloor, events. events is a bit set: 1 jump start, 2 landed,
 * 4 step-up (z gain in one walking frame above StepUpMinDeltaZ), 8 horizontal speed collapse while airborne (the face of a ledge
 * took the speed: compare with Core JumpModel, BlockedRising).
 *
 * STATUS: written without an Unreal build. It only uses long-standing character/movement members; compile it first on Windows.
 */
UCLASS(ClassGroup = (Spn), meta = (BlueprintSpawnableComponent))
class USpnMovementDiagnostics : public UActorComponent
{
	GENERATED_BODY()

public:
	USpnMovementDiagnostics();

	/** Record while true (also on while the console variable spn.MoveDiag is not 0). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Diagnostics")
	bool bEnabled = false;

	/** Ring buffer size in frames (about 20 s at 60 fps). */
	UPROPERTY(EditAnywhere, Category = "Spn|Diagnostics", meta = (ClampMin = "60", ClampMax = "20000"))
	int32 MaxSamples = 1200;

	/** Dump the buffer to the log whenever the character lands. */
	UPROPERTY(EditAnywhere, Category = "Spn|Diagnostics")
	bool bDumpOnLanding = true;

	/** A walking frame that gains at least this much height (cm) counts as a step-up event. */
	UPROPERTY(EditAnywhere, Category = "Spn|Diagnostics", meta = (ClampMin = "0.1"))
	float StepUpMinDeltaZ = 1.f;

	/** In the air: a frame that loses more than this fraction of the horizontal speed counts as an event 8. */
	UPROPERTY(EditAnywhere, Category = "Spn|Diagnostics", meta = (ClampMin = "0.1", ClampMax = "1"))
	float SpeedCollapseFraction = 0.4f;

	/** Writes the buffered frames to the log and clears the buffer. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Diagnostics")
	void DumpAndClear();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	struct FSample
	{
		double Time = 0.0;
		FVector Position = FVector::ZeroVector;
		FVector Velocity = FVector::ZeroVector;
		uint8 Mode = 0;
		float FloorDist = 0.f;
		bool bWalkableFloor = false;
		uint8 Events = 0;
	};

	TArray<FSample> Samples;
	int32 Head = 0;
	bool bHavePrevious = false;
	FSample Previous;
	bool bWasEnabled = false;

	bool IsRecording() const;
	void Push(const FSample& Sample);
};
