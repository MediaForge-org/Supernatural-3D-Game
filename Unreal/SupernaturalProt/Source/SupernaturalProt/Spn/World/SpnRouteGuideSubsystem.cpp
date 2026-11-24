#include "Spn/World/SpnRouteGuideSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "SpnMapSubsystem.h"
#include "Spn/SpnBuildingObjects.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/SpnWorldObjects.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
#include "SupernaturalProt.h"

namespace
{
	// ids written by the world generator (Tools/spn_world/gameplay.py, buildings.py)
	const TCHAR* DoorAId = TEXT("spnworld.oak_n7_door");
	const TCHAR* DoorBId = TEXT("spnworld.oak_n6_door");
	const TCHAR* CabinDoorId = TEXT("cabin_door");
	const TCHAR* KeyId = TEXT("key.spnworld.oak_n6");
	const TCHAR* FlashlightId = TEXT("spnworld.pickup.flashlight");
}

bool USpnRouteGuideSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USpnRouteGuideSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	bActive = InWorld.GetMapName().Contains(TEXT("Lvl_SpnWorld"));
	Step = -1;
	bCompleted = false;
	GuideWaypointId = 0;
}

void USpnRouteGuideSubsystem::FindObjects()
{
	for (TActorIterator<ASpnTestDoor> It(GetWorld()); It; ++It)
	{
		if (It->StableId == DoorAId) { DoorA = *It; }
		else if (It->StableId == DoorBId) { DoorB = *It; }
		else if (It->StableId == CabinDoorId) { CabinDoor = *It; }
	}
	for (TActorIterator<ASpnTestKey> It(GetWorld()); It; ++It)
	{
		if (It->KeyId == KeyId) { Key = *It; }
	}
	for (TActorIterator<ASpnWorldPickup> It(GetWorld()); It; ++It)
	{
		if (It->StableId == FlashlightId) { Flashlight = *It; }
	}
}

int32 USpnRouteGuideSubsystem::ComputeStep(const FVector& PlayerLocation) const
{
	if (!DoorA.IsValid() || !DoorB.IsValid() || !CabinDoor.IsValid() || !Key.IsValid() || !Flashlight.IsValid())
	{
		return -1;
	}
	if (bCompleted) { return 4; }
	if (!Key->IsHeld() && DoorB->IsLocked()) { return 0; }
	if (DoorB->IsLocked()) { return 1; }
	if (!Flashlight->IsTaken()) { return 2; }
	if (FVector::Dist2D(PlayerLocation, CabinDoor->GetActorLocation()) > CabinRadiusCm) { return 3; }
	return 4;
}

void USpnRouteGuideSubsystem::UpdateWaypoint(const AActor* Target, const FText& Label)
{
	USpnMapSubsystem* Map = GetWorld()->GetGameInstance() != nullptr ? GetWorld()->GetGameInstance()->GetSubsystem<USpnMapSubsystem>() : nullptr;
	if (Map == nullptr || !Map->HasDefinition())
	{
		return;
	}
	FSpnMapMarkerView Current;
	const bool bHasWaypoint = Map->GetWaypoint(Current);
	// a guide waypoint may come back from a save (other id): recognised by its label
	bool bGuideLabel = false;
	for (const TCHAR* LabelKey : {TEXT("ui.guide.target1"), TEXT("ui.guide.target2"), TEXT("ui.guide.target3"), TEXT("ui.guide.target4")})
	{
		bGuideLabel |= Current.Label == FSpnUiText::Get(LabelKey).ToString();
	}
	if (bHasWaypoint && Current.Id != GuideWaypointId && !bGuideLabel)
	{
		return; // the player's own waypoint wins
	}
	if (Target == nullptr)
	{
		if (bHasWaypoint) { Map->ClearWaypoint(); }
		GuideWaypointId = 0;
		return;
	}
	FVector2D Metres;
	int64 Id = 0;
	if (Map->UnrealToMap(Target->GetActorLocation(), Metres) && Map->SetWaypointAt(Metres, Label.ToString(), Id) == ESpnMapResult::Ok)
	{
		GuideWaypointId = Id;
	}
}

void USpnRouteGuideSubsystem::ApplyStep(int32 NewStep)
{
	Step = NewStep;
	static const TCHAR* Keys[] = {TEXT("ui.guide.step1"), TEXT("ui.guide.step2"), TEXT("ui.guide.step3"), TEXT("ui.guide.step4"), TEXT("ui.guide.done")};
	static const TCHAR* Labels[] = {TEXT("ui.guide.target1"), TEXT("ui.guide.target2"), TEXT("ui.guide.target3"), TEXT("ui.guide.target4")};
	Objective = NewStep >= 0 ? FSpnUiText::Get(Keys[NewStep]) : FText::GetEmpty();
	SpnUi::SetObjective(this, Objective);
	const AActor* Targets[] = {DoorA.Get(), DoorB.Get(), Flashlight.Get(), CabinDoor.Get()};
	UpdateWaypoint(NewStep >= 0 && NewStep < 4 ? Targets[NewStep] : nullptr, NewStep >= 0 && NewStep < 4 ? FSpnUiText::Get(Labels[NewStep]) : FText::GetEmpty());
	if (NewStep == 4 && !bCompleted)
	{
		bCompleted = true;
		SpnUi::Notify(this, FSpnUiText::Get(TEXT("ui.guide.complete")), SpnUi::ENote::Good);
	}
	UE_LOG(LogSupernaturalProt, Display, TEXT("RouteGuide: step %d (%s)"), NewStep, *Objective.ToString());
}

void USpnRouteGuideSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Accumulator += DeltaTime;
	if (Accumulator < 0.25f)
	{
		return;
	}
	Accumulator = 0.f;
	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	const APawn* Pawn = PC != nullptr ? PC->GetPawn() : nullptr;
	if (Pawn == nullptr)
	{
		return;
	}
	if (!DoorA.IsValid() || !DoorB.IsValid() || !CabinDoor.IsValid() || !Key.IsValid() || !Flashlight.IsValid())
	{
		FindObjects();
	}
	const int32 NewStep = ComputeStep(Pawn->GetActorLocation());
	if (NewStep != Step)
	{
		ApplyStep(NewStep);
	}
	else
	{
		SpnUi::SetObjective(this, Objective);   // the HUD may have been (re)created after the step was set
		// a new game or a load replaces the map state (and with it the guide's waypoint): put it back while no waypoint exists
		const USpnMapSubsystem* Map = GetWorld()->GetGameInstance() != nullptr ? GetWorld()->GetGameInstance()->GetSubsystem<USpnMapSubsystem>() : nullptr;
		FSpnMapMarkerView Current;
		if (Step >= 0 && Step < 4 && Map != nullptr && Map->HasDefinition() && !Map->GetWaypoint(Current))
		{
			GuideWaypointId = 0;
			const AActor* Targets[] = {DoorA.Get(), DoorB.Get(), Flashlight.Get(), CabinDoor.Get()};
			static const TCHAR* Labels[] = {TEXT("ui.guide.target1"), TEXT("ui.guide.target2"), TEXT("ui.guide.target3"), TEXT("ui.guide.target4")};
			UpdateWaypoint(Targets[Step], FSpnUiText::Get(Labels[Step]));
		}
	}
}
