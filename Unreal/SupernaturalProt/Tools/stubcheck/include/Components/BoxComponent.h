#pragma once
#include "CoreMinimal.h"
enum ECollisionEnabled { QueryOnly };
enum ECollisionChannel { ECC_Pawn };
enum ECollisionResponse { ECR_Ignore, ECR_Overlap };
struct FStubDelegate { template <typename T, typename F> void AddDynamicStub(T*, F) {} };
#define AddDynamic(Object, Function) AddDynamicStub(Object, Function)

class UBoxComponent : public UPrimitiveComponent
{
public:
	FBoxSphereBounds Bounds;
	FStubDelegate OnComponentEndOverlap;
	void SetBoxExtent(const FVector&) {}
	void SetCollisionEnabled(ECollisionEnabled) {}
	void SetCollisionResponseToAllChannels(ECollisionResponse) {}
	void SetCollisionResponseToChannel(ECollisionChannel, ECollisionResponse) {}
	void SetGenerateOverlapEvents(bool) {}
	void SetHiddenInGame(bool) {}
};
