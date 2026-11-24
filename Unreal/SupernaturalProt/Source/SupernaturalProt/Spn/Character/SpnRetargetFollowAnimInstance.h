#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "AnimNodes/AnimNode_RetargetPoseFromMesh.h"
#include "SpnRetargetFollowAnimInstance.generated.h"

class UIKRetargeter;

/**
 * Proxy that runs exactly one "Retarget Pose From Mesh" node as its root (no anim blueprint, no graph). Plain struct, not reflected: the node's
 * retargeter asset is kept alive by USpnRetargetFollowAnimInstance::Retargeter.
 */
struct FSpnRetargetFollowProxy : public FAnimInstanceProxy
{
	explicit FSpnRetargetFollowProxy(UAnimInstance* InAnimInstance) : FAnimInstanceProxy(InAnimInstance) {}

	/** Game thread only, before or between updates. Re-initialises the retarget processor when an asset changes. */
	void Configure(UIKRetargeter* Retargeter, USkeletalMeshComponent* Source, int32 IKLodThreshold);

	FAnimNode_RetargetPoseFromMesh RetargetNode;

protected:
	virtual FAnimNode_Base* GetCustomRootNode() override { return &RetargetNode; }
	virtual void GetCustomNodes(TArray<FAnimNode_Base*>& OutNodes) override { OutNodes.Add(&RetargetNode); }
};

/**
 * Drives a presentation body from the hidden gameplay mesh by runtime IK retargeting (Docs/Proposals/0014 step C2). The gameplay mesh keeps
 * its anim blueprint (ABP_Unarmed: locomotion, jump, foot IK, ledge assist); this instance only copies its pose onto another skeleton every
 * frame. Native on purpose: no anim blueprint asset has to be authored for the test body, and the node settings are visible in code.
 */
UCLASS(Transient, NotBlueprintable)
class SUPERNATURALPROT_API USpnRetargetFollowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** Source = the animated gameplay mesh (must tick before this one). IKLodThreshold: IK pass runs up to this LOD, -1 = always. */
	void Configure(UIKRetargeter* InRetargeter, USkeletalMeshComponent* InSource, int32 IKLodThreshold = -1);

	/** True once the retarget processor initialised with the current assets (false = the body shows its reference pose). */
	bool IsRetargetReady();

	/** Average cost of the retarget node in ms (editor builds only: the engine measures it there; -1 in a packaged game). */
	double GetAverageRetargetMs();

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return new FSpnRetargetFollowProxy(this); }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override { delete InProxy; }

private:
	UPROPERTY(Transient)
	TObjectPtr<UIKRetargeter> Retargeter;

	TWeakObjectPtr<USkeletalMeshComponent> Source;
};
