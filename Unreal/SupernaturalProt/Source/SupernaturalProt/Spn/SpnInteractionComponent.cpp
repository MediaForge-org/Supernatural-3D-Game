#include "Spn/SpnInteractionComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"
#include "SupernaturalProt.h"

USpnInteractionComponent::USpnInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpnInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!bShowPrompt || Pawn == nullptr || !Pawn->IsLocallyControlled())
	{
		CurrentPrompt.Reset();
		return;
	}

	const ISpnInteractable* Interactable = Cast<ISpnInteractable>(FindTarget());
	const FString Verb = Interactable != nullptr ? Interactable->SpnPromptText(Pawn) : FString();
	CurrentPrompt = Verb.IsEmpty() ? FString() : FString::Printf(TEXT("[%s] %s"), *PromptKey.GetDisplayName().ToString(), *Verb);
#if !UE_BUILD_SHIPPING
	if (GEngine != nullptr && !CurrentPrompt.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(static_cast<uint64>(0x5350'4E50), 0.f, FColor::White, CurrentPrompt, false, FVector2D(2.f, 2.f));
	}
#endif
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
