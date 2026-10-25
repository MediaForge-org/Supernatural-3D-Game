#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpnPlayAreaGuard.generated.h"

class UBoxComponent;

/**
 * Safety net for a bounded test area (M2-A, Docs/Proposals/0011): a box volume around the playable space.
 * A pawn that leaves it (over a wall, through a gap, down a hole) is put back at SafeLocation on the
 * authority, with its velocity cleared. The map's walls and terrain are the first barrier; this is the
 * second, so the boundary does not depend on decorative geometry alone.
 *
 * No tick: it reacts to the end-overlap event only. PLACEHOLDER for test maps, not a world-boundary design.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class ASpnPlayAreaGuard : public AActor
{
	GENERATED_BODY()

public:
	ASpnPlayAreaGuard();

	/** Where a pawn that left the area is put back (world space). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Play Area")
	FVector SafeLocation = FVector(0.0, 0.0, 120.0);

	/** Half extents of the guarded box in cm (the actor location is its centre). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Play Area", meta = (ClampMin = "100"))
	FVector BoxExtent = FVector(15000.0, 15000.0, 5000.0);

	/** Puts the pawn back at SafeLocation (authority only). Returns false on clients or for a null pawn. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Play Area")
	bool ReturnToSafeLocation(APawn* Pawn);

	/** How many pawns were put back since the level started (diagnostics, tests). */
	int32 GetReturnCount() const { return ReturnCount; }

	/** True when the pawn's location is inside the guarded box. */
	bool IsInside(const AActor* Actor) const;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* Volume;

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	int32 ReturnCount = 0;
};
