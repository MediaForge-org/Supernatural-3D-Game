#include "Spn/SpnInteractionComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
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
	// "E - Verb": the key name comes from PromptKey (the character copies its interact key here), so a gamepad key shows its own name later.
	CurrentPrompt = Verb.IsEmpty() ? FString() : FString::Printf(TEXT("%s \u2013 %s"), *PromptKey.GetDisplayName().ToString(), *Verb);
	if (CurrentPrompt.IsEmpty())
	{
		HidePromptWidget();
	}
	else
	{
		ShowPromptWidget(CurrentPrompt);
	}
}

void USpnInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HidePromptWidget();
	Super::EndPlay(EndPlayReason);
}

void USpnInteractionComponent::ShowPromptWidget(const FString& Text)
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr || !FSlateApplication::IsInitialized())
	{
		return;
	}
	if (!PromptWidget.IsValid())
	{
		// A dark translucent plate with a bold white text, centred horizontally, in the lower third: not over the crosshair/centre of the view.
		PromptWidget = SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(0.f, 0.f, 0.f, 190.f))
			.Visibility(EVisibility::HitTestInvisible)
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.62f))
				.Padding(FMargin(22.f, 9.f))
				[
					SAssignNew(PromptTextBlock, STextBlock)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 30))
					.ColorAndOpacity(FLinearColor::White)
					.ShadowOffset(FVector2D(1.f, 1.f))
					.ShadowColorAndOpacity(FLinearColor::Black)
				]
			];
		GEngine->GameViewport->AddViewportWidgetContent(PromptWidget.ToSharedRef(), 50);
	}
	PromptTextBlock->SetText(FText::FromString(Text));
}

void USpnInteractionComponent::HidePromptWidget()
{
	if (PromptWidget.IsValid())
	{
		if (GEngine != nullptr && GEngine->GameViewport != nullptr)
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(PromptWidget.ToSharedRef());
		}
		PromptWidget.Reset();
		PromptTextBlock.Reset();
	}
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
