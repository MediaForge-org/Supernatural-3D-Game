#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpnSkinnedGroup.generated.h"

class UInstancedSkinnedMeshComponent;

/**
 * Holder for one group of identical skinned meshes drawn as instances: the trees of the world generator that come as a skeletal mesh
 * with a Nanite assembly (branches and twigs instanced inside the mesh). Needs "Nanite Foliage" (r.Nanite.Foliage) in the project.
 * Python cannot add components to a bare Actor, so the component is the root of this actor. No tick, no gameplay logic, no collision
 * (the generator places separate trunk colliders).
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class ASpnSkinnedGroup : public AActor
{
	GENERATED_BODY()

public:
	ASpnSkinnedGroup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spn|Instancing")
	TObjectPtr<UInstancedSkinnedMeshComponent> Instances;
};
