#include "Spn/SpnInteractionComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"
#include "SupernaturalProt.h"

USpnInteractionComponent::USpnInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AActor* USpnInteractionComponent::FindTarget() const
{
	const AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		return nullptr;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	return FindTargetAlong(EyeLocation, EyeRotation.Vector());
}

AActor* USpnInteractionComponent::FindTargetAlong(const FVector& Start, const FVector& Direction) const
{
	const UWorld* World = GetWorld();
	if (World == nullptr || Direction.IsNearlyZero())
	{
		return nullptr;
	}

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnInteractionTrace), /*bTraceComplex*/ false, GetOwner());
	FHitResult Hit;
	const FVector End = Start + Direction.GetSafeNormal() * InteractRange;
	if (!World->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(TraceRadius), Params))
	{
		return nullptr;
	}

	AActor* HitActor = Hit.GetActor();
	return HitActor != nullptr && HitActor->Implements<USpnInteractable>() ? HitActor : nullptr;
}

bool USpnInteractionComponent::TryInteract()
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr || !Owner->HasAuthority())
	{
		// Co-op later routes the request to the host; nothing is replicated yet.
		UE_LOG(LogSupernaturalProt, Verbose, TEXT("Interaction ignored: not the authority"));
		return false;
	}

	AActor* Target = FindTarget();
	ISpnInteractable* Interactable = Cast<ISpnInteractable>(Target);
	if (Interactable == nullptr)
	{
		UE_LOG(LogSupernaturalProt, Verbose, TEXT("Interaction: nothing to use in range"));
		return false;
	}

	const FSpnInteractStatus Status = Interactable->SpnUse(Owner);
	UE_LOG(LogSupernaturalProt, Log, TEXT("Interaction: %s used %s -> %s %s"), *GetNameSafe(Owner), *GetNameSafe(Target),
		LexToString(Status.Result), *Status.Feedback);
	return true;
}
