#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"

#include "SpnBuildingObjects.generated.h"

class UStaticMeshComponent;
class ULightComponent;
class USpnWorldStateSubsystem;

/**
 * PLACEHOLDER key (NightBuild04): taking it sets the world fact `placeholder.key.<KeyId>.held`, which a door with the same RequiredKeyId
 * reads. The key keeps no state of its own: held (fact) = hidden and not usable, so it stays gone after a save/load, and a loaded save
 * without the fact puts it back. A key item in the inventory can set the same fact later.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnTestKey : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnTestKey();

	/** Key id ([a-z][a-z0-9_.]*): the same text as ASpnTestDoor::RequiredKeyId. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Key")
	FString KeyId = TEXT("key.test");

	/** Name shown in the pick-up message. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Key")
	FText DisplayName = FText::FromString(TEXT("Schlüssel"));

	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

	bool IsHeld() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	USpnWorldStateSubsystem* GetWorldState() const;
	void Refresh();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	FDelegateHandle ReplacedHandle;
};

/**
 * PLACEHOLDER wall switch for lights (NightBuild04): a placeholder.switch device of the Core. The switch state lives only in WorldFacts
 * (placeholder.switch.<StableId>.on, absent = bAuthoredOn); the lights it controls are re-read after every use and every load.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnLightSwitch : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnLightSwitch();

	UPROPERTY(EditAnywhere, Category = "Spn|Light Switch")
	FString StableId = TEXT("light_switch");

	UPROPERTY(EditAnywhere, Category = "Spn|Light Switch")
	bool bAuthoredOn = true;

	/** Actors whose light components follow the switch. */
	UPROPERTY(EditAnywhere, Category = "Spn|Light Switch")
	TArray<TObjectPtr<AActor>> Lights;

	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

	bool IsOn() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	USpnWorldStateSubsystem* GetWorldState() const;
	void Refresh();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Plate;

	FDelegateHandle ReplacedHandle;
};
