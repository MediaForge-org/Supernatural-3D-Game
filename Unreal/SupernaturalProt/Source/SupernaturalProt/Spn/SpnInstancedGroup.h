#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpnInstancedGroup.generated.h"

class UInstancedStaticMeshComponent;

/**
 * Holder for one group of identical static-mesh primitives drawn as instances (map generator, --instancing).
 * Python cannot add components to a bare Actor, so the instanced component is the root of this actor.
 * PLACEHOLDER for generated test maps; no tick, no gameplay logic.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class ASpnInstancedGroup : public AActor
{
	GENERATED_BODY()

public:
	ASpnInstancedGroup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spn|Instancing")
	TObjectPtr<UInstancedStaticMeshComponent> Instances;
};
