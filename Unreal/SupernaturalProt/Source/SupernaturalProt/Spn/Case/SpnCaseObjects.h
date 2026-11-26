#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"
#include "SpnCaseObjects.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

// Placeholder objects of the first case (NightBuild07, Docs/Proposals/0019). Engine shapes only; the developer replaces them with real assets
// later (the actors stay, the meshes change). Nothing here is saved yet: a picked bottle comes back after a load (KNOWN ISSUE in the report).

/** The mill bell: Use rings it (loud noise, stuns creatures in range; USpnCaseSubsystem::RingBell). */
UCLASS()
class SUPERNATURALPROT_API ASpnBell : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnBell();

	UPROPERTY(EditAnywhere, Category = "Spn|Bell", meta = (ClampMin = "0"))
	float CooldownSeconds = 3.f;

	int32 GetRingCount() const { return RingCount; }
	int32 GetLastStunned() const { return LastStunned; }

	//~ ISpnInteractable
	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Frame;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Bell;

	double LastRingTime = -1000.0;
	int32 RingCount = 0;
	int32 LastStunned = 0;
};

/** Something to throw (a bottle): Use picks it up (USpnCaseSubsystem::AddThrowable). */
UCLASS()
class SUPERNATURALPROT_API ASpnThrowablePickup : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnThrowablePickup();

	//~ ISpnInteractable
	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
};

/** A thrown object in flight: on its first hit it makes an Impact noise there (a lure), then it lies still and disappears after a while. */
UCLASS()
class SUPERNATURALPROT_API ASpnThrownObject : public AActor
{
	GENERATED_BODY()

public:
	ASpnThrownObject();

	void Launch(const FVector& VelocityCm);
	bool HasLanded() const { return bLanded; }
	FVector GetImpactCm() const { return ImpactCm; }

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> Movement;

	bool bLanded = false;
	FVector ImpactCm = FVector::ZeroVector;
};

/** Entering the box writes the autosave once (USpnCaseSubsystem::RequestAutosave; refused during a pursuit). */
UCLASS()
class SUPERNATURALPROT_API ASpnAutosavePoint : public AActor
{
	GENERATED_BODY()

public:
	ASpnAutosavePoint();

	/** Each id saves once per session ("arrival", "after_research", "before_mill_night" ...). */
	UPROPERTY(EditAnywhere, Category = "Spn|Autosave")
	FName PointId = TEXT("arrival");

	UPROPERTY(EditAnywhere, Category = "Spn|Autosave")
	FVector Extent = FVector(200.f, 200.f, 150.f);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

private:
	/** A player who starts the level inside the box gets no begin-overlap: checked once shortly after the start. */
	void CheckStartInside();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> Box;
};
