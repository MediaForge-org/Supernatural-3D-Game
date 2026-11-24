#include "Spn/World/SpnSkinnedGroup.h"

#include "Components/InstancedSkinnedMeshComponent.h"
#include "Engine/CollisionProfile.h"

ASpnSkinnedGroup::ASpnSkinnedGroup()
{
	PrimaryActorTick.bCanEverTick = false;

	Instances = CreateDefaultSubobject<UInstancedSkinnedMeshComponent>(TEXT("Instances"));
	Instances->SetMobility(EComponentMobility::Static);
	Instances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	// The trees never move and stand in their reference pose (no animation bank). An instance that animates invalidates its virtual
	// shadow map pages every frame, so the whole forest was re-rendered into the shadow map each frame. A minimum screen size no instance
	// can reach switches instance animation off. Measured 2026-11-13 at 1440p, still camera on the forest track: shadow depths 5.9 -> 1.7 ms,
	// 43.8 -> 50.7 fps (Artifacts/VisualProgress/PROGRESS.md). Wind in the foliage material (world position offset) is unaffected.
	Instances->SetAnimationMinScreenSize(1000.f);
	Instances->ShadowCacheInvalidationBehavior = EShadowCacheInvalidationBehavior::Static;
	SetRootComponent(Instances);
}
