#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpnDecorDoorSubsystem.generated.h"

/**
 * Doors that are part of a building mesh in the generated world (no ASpnTestDoor behind them). Developer decision 2026-11-14: no permanent
 * marker on these doors; the game answers an ATTEMPT to use one ("This door does not open.") so the player knows it is not a bug.
 *
 * Data: Content/SpnWorld/Data/SpnWorldDecorDoors.json, written by the world generator (Tools/spn_world/buildings.py, archkit.door). Loaded only
 * in Lvl_SpnWorld; other levels have none. PLACEHOLDER test-world data.
 */
UCLASS()
class USpnDecorDoorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	struct FDoor
	{
		FString Id;
		FVector Centre = FVector::ZeroVector;   // centre of the leaf, cm
		FVector Normal = FVector::ForwardVector; // outward (towards the street side), horizontal
		float HalfWidth = 50.f;
		float Height = 210.f;
	};

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/** The decorative door the view from Eye along Direction points at within Range (cm), with a clear line of sight; nullptr if none. */
	const FDoor* FindDoorInView(const FVector& Eye, const FVector& Direction, float Range) const;

	int32 GetDoorCount() const { return Doors.Num(); }
	const TArray<FDoor>& GetDoors() const { return Doors; }

	static FString GetDataPath();

	/** Loads the data file (also used by tests). Returns the number of doors read; 0 and OutError on failure. */
	int32 Load(const FString& Path, FString* OutError = nullptr);

private:
	TArray<FDoor> Doors;
};
