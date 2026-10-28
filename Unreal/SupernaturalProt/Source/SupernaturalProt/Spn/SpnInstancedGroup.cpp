#include "Spn/SpnInstancedGroup.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

ASpnInstancedGroup::ASpnInstancedGroup()
{
	PrimaryActorTick.bCanEverTick = false;

	Instances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instances"));
	Instances->SetMobility(EComponentMobility::Static);
	Instances->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	SetRootComponent(Instances);
}
