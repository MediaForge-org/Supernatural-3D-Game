#include "Spn/SpnInteractionComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/OverlapResult.h"
#include "Components/PrimitiveComponent.h"
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
#include "Spn/UI/SpnUiText.h"
#include "SpnInteractionPick.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/World/SpnDecorDoorSubsystem.h"
#include "SupernaturalProt.h"

USpnInteractionComponent::USpnInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpnInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bForceHeld)
	{
		TickForce(DeltaTime);
	}

	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!bShowPrompt || Pawn == nullptr || !Pawn->IsLocallyControlled())
	{
		CurrentPrompt.Reset();
		return;
	}

	AActor* Target = FindTarget();
	const ISpnInteractable* Interactable = Cast<ISpnInteractable>(Target);
	const FString Verb = Interactable != nullptr ? Interactable->SpnPromptText(Pawn) : FString();
	const FString Force = Interactable != nullptr ? Interactable->SpnForcePromptText(Pawn) : FString();
	// "E / Quadrat - Verb": the key names come from the key properties (the character copies its keys here); the pad name is shown while a gamepad is attached.
	const bool bPad = FSlateApplication::IsInitialized() && FSlateApplication::Get().IsGamepadAttached();
	const FString UseKeys = bPad ? FString::Printf(TEXT("%s / %s"), *PromptKey.GetDisplayName().ToString(), *FSpnUiText::Get(TEXT("ui.interact.pad")).ToString()) : PromptKey.GetDisplayName().ToString();
	const FString ForceKeys = bPad ? FString::Printf(TEXT("%s / %s"), *ForceKey.GetDisplayName().ToString(), *FSpnUiText::Get(TEXT("ui.interact.padforce")).ToString()) : ForceKey.GetDisplayName().ToString();
	CurrentPrompt = Verb.IsEmpty() ? FString() : FString::Printf(TEXT("%s – %s"), *UseKeys, *Verb);
	if (!CurrentPrompt.IsEmpty() && !Force.IsEmpty())
	{
		CurrentPrompt += FString::Printf(TEXT("     %s – %s"), *ForceKeys, *Force);
	}
	if (bForceHeld)
	{
		CurrentPrompt = FString::Printf(TEXT("%s %d%%"), *FSpnUiText::Get(TEXT("ui.door.breaking")).ToString(), FMath::Clamp(FMath::RoundToInt(ForceProgress * 100.f), 0, 100));
	}
	if (SpnUi::SetPrompt(this, CurrentPrompt))
	{
		HidePromptWidget(); // the unified HUD draws the prompt (Night Build 01)
	}
	else if (CurrentPrompt.IsEmpty())
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

	// 1. Everything usable whose collision lies within reach of the eyes.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnInteractionOverlap), /*bTraceComplex*/ false, GetOwner());
	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(Overlaps, Start, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(InteractRange), Params);

	TArray<AActor*> Actors;
	TArray<FSpnPickCandidate> Candidates;
	const FVector View = Direction.GetSafeNormal();
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Actor = Overlap.GetActor();
		UPrimitiveComponent* Component = Overlap.GetComponent();
		if (Actor == nullptr || Component == nullptr || Actors.Contains(Actor) || !Actor->Implements<USpnInteractable>())
		{
			continue;
		}

		FVector Origin, Extent;
		Actor->GetActorBounds(/*bOnlyCollidingComponents*/ true, Origin, Extent);

		// 2. Line of sight to the nearest point of the object: a wall (or any other blocking actor) in between hides it.
		FVector Nearest = Origin;
		Component->GetClosestPointOnCollision(Start, Nearest);
		FHitResult Block;
		FCollisionQueryParams SightParams(SCENE_QUERY_STAT(SpnInteractionSight), /*bTraceComplex*/ false, GetOwner());
		const FVector SightEnd = Nearest + (Start - Nearest).GetSafeNormal() * 2.f;
		const bool bBlocked = World->LineTraceSingleByChannel(Block, Start, SightEnd, TraceChannel, SightParams) && Block.GetActor() != Actor;

		FSpnPickCandidate Candidate;
		Candidate.EyeCm = Start;
		Candidate.ViewDirection = View;
		Candidate.CentreCm = Origin;
		Candidate.RadiusCm = FMath::Min(Extent.GetMax(), MaxTargetRadius);
		Candidate.bLineOfSightClear = !bBlocked;
		Candidate.Priority = Cast<ISpnInteractable>(Actor)->SpnInteractionPriority();
		Actors.Add(Actor);
		Candidates.Add(Candidate);
	}

	// 3. The Core decides: in reach, in view, line of sight, then the most central and closest, ties by priority.
	FSpnPickConfig Config;
	Config.MaxDistanceCm = InteractRange;
	Config.MaxAngleDegrees = MaxViewAngle;
	Config.MaxHeightDifferenceCm = MaxHeightDifference;
	const int32 Index = SpnInteractionPick::Pick(Config, Candidates);
	return Index != INDEX_NONE ? Actors[Index] : nullptr;
}

bool USpnInteractionComponent::TryInteract()
{
	return TryVerb(/*bForce*/ false);
}

bool USpnInteractionComponent::TryForce()
{
	return TryVerb(/*bForce*/ true);
}

bool USpnInteractionComponent::BeginForce()
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr || !Owner->HasAuthority())
	{
		return false;
	}
	AbortForce();
	AActor* Target = FindTarget();
	ISpnInteractable* Interactable = Cast<ISpnInteractable>(Target);
	if (Interactable == nullptr)
	{
		AnswerDecorDoor(/*bForce*/ true);
		return false;
	}
	const float Duration = Interactable->SpnForceDuration();
	// Only a Force that is offered right now takes time; a refused one (not breakable, not locked ...) answers at once with its message.
	if (Duration <= 0.f || Interactable->SpnForcePromptText(Owner).IsEmpty())
	{
		return TryVerb(/*bForce*/ true);
	}
	ForceTarget = Target;
	ForceDuration = Duration;
	ForceElapsed = 0.f;
	ForceProgress = 0.f;
	bForceHeld = true;
	UE_LOG(LogSupernaturalProt, Log, TEXT("Interaction: %s starts forcing %s (%.1f s)"), *GetNameSafe(Owner), *GetNameSafe(Target), Duration);
	return true;
}

void USpnInteractionComponent::EndForce()
{
	if (bForceHeld)
	{
		UE_LOG(LogSupernaturalProt, Log, TEXT("Interaction: forcing %s cancelled at %.0f%%"), *GetNameSafe(ForceTarget.Get()), ForceProgress * 100.f);
	}
	AbortForce();
}

void USpnInteractionComponent::AbortForce()
{
	if (ISpnInteractable* Interactable = Cast<ISpnInteractable>(ForceTarget.Get()))
	{
		Interactable->SpnForceStrain(0.f);
	}
	bForceHeld = false;
	ForceTarget.Reset();
	ForceElapsed = 0.f;
	ForceProgress = 0.f;
}

void USpnInteractionComponent::TickForce(float DeltaTime)
{
	AActor* Owner = GetOwner();
	AActor* Target = FindTarget();
	ISpnInteractable* Interactable = Cast<ISpnInteractable>(Target);
	// The effort stops when the player looks away, steps out of reach, or the action is no longer possible (someone else opened the door ...).
	if (Interactable == nullptr || Target != ForceTarget.Get() || Interactable->SpnForcePromptText(Owner).IsEmpty())
	{
		UE_LOG(LogSupernaturalProt, Log, TEXT("Interaction: forcing %s interrupted at %.0f%%"), *GetNameSafe(ForceTarget.Get()), ForceProgress * 100.f);
		AbortForce();
		return;
	}
	ForceElapsed += DeltaTime;
	ForceProgress = FMath::Clamp(ForceElapsed / FMath::Max(ForceDuration, KINDA_SMALL_NUMBER), 0.f, 1.f);
	Interactable->SpnForceStrain(ForceProgress);
	if (ForceElapsed >= ForceDuration)
	{
		AbortForce(); // ends the effort first (strain off), then commits
		TryVerb(/*bForce*/ true);
	}
}

bool USpnInteractionComponent::HasForceTarget() const
{
	const AActor* Target = FindTarget();
	const ISpnInteractable* Interactable = Cast<ISpnInteractable>(Target);
	return Interactable != nullptr && !Interactable->SpnForcePromptText(GetOwner()).IsEmpty();
}

void USpnInteractionComponent::AnswerDecorDoor(bool bForce)
{
	const UWorld* World = GetWorld();
	const USpnDecorDoorSubsystem* Doors = World != nullptr ? World->GetSubsystem<USpnDecorDoorSubsystem>() : nullptr;
	if (Doors == nullptr || Doors->GetDoorCount() == 0 || GetOwner() == nullptr)
	{
		return;
	}
	FVector Eye;
	FRotator View;
	GetOwner()->GetActorEyesViewPoint(Eye, View);
	const USpnDecorDoorSubsystem::FDoor* Door = Doors->FindDoorInView(Eye, View.Vector(), InteractRange);
	if (Door == nullptr)
	{
		return;
	}
	// one short note per attempt, not repeated while the key is hammered
	const double Now = World->GetTimeSeconds();
	const bool bRepeat = Door->Id == LastDecorDoorId && Now - LastDecorDoorTime < 1.5;
	LastDecorDoorId = Door->Id;
	LastDecorDoorTime = Now;
	UE_LOG(LogSupernaturalProt, Log, TEXT("Interaction: %s tried the facade door %s (%s): it does not open"), *GetNameSafe(GetOwner()), *Door->Id, bForce ? TEXT("force") : TEXT("use"));
	if (!bRepeat)
	{
		SpnUi::Notify(this, FSpnUiText::Get(bForce ? TEXT("ui.door.decor.force") : TEXT("ui.door.decor.use")));
	}
}

bool USpnInteractionComponent::TryVerb(bool bForce)
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
		AnswerDecorDoor(bForce);
		return false;
	}

	const FSpnInteractStatus Status = bForce ? Interactable->SpnForce(Owner) : Interactable->SpnUse(Owner);
	UE_LOG(LogSupernaturalProt, Log, TEXT("Interaction: %s %s %s -> %s %s"), *GetNameSafe(Owner), bForce ? TEXT("forced") : TEXT("used"),
		*GetNameSafe(Target), LexToString(Status.Result), *Status.Feedback);
	return true;
}
