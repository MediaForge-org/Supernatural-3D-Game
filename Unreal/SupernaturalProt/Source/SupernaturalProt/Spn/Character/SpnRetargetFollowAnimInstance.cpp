#include "Spn/Character/SpnRetargetFollowAnimInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "Retargeter/IKRetargeter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpnRetargetFollowAnimInstance)

void FSpnRetargetFollowProxy::Configure(UIKRetargeter* Retargeter, USkeletalMeshComponent* Source, int32 IKLodThreshold)
{
	RetargetNode.IKRetargeterAsset = Retargeter;
	RetargetNode.RetargetFrom = ERetargetSourceMode::CustomSkeletalMeshComponent;
	RetargetNode.SourceMeshComponent = Source;
	RetargetNode.LODThresholdForIK = IKLodThreshold;
	if (FIKRetargetProcessor* Processor = RetargetNode.GetRetargetProcessor())
	{
		Processor->SetNeedsInitialized();
	}
}

void USpnRetargetFollowAnimInstance::Configure(UIKRetargeter* InRetargeter, USkeletalMeshComponent* InSource, int32 IKLodThreshold)
{
	Retargeter = InRetargeter;
	Source = InSource;
	GetProxyOnGameThread<FSpnRetargetFollowProxy>().Configure(InRetargeter, InSource, IKLodThreshold);
}

bool USpnRetargetFollowAnimInstance::IsRetargetReady()
{
	FIKRetargetProcessor* Processor = GetProxyOnGameThread<FSpnRetargetFollowProxy>().RetargetNode.GetRetargetProcessor();
	return Processor != nullptr && Processor->IsInitialized();
}

double USpnRetargetFollowAnimInstance::GetAverageRetargetMs()
{
#if WITH_EDITOR
	return GetProxyOnGameThread<FSpnRetargetFollowProxy>().RetargetNode.GetAverageExecutionTime() * 1000.0;
#else
	return -1.0;
#endif
}
