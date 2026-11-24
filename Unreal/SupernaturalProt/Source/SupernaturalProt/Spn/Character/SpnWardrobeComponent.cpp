#include "Spn/Character/SpnWardrobeComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Spn/Character/SpnCharacterVisualComponent.h"
#include "SupernaturalProt.h"

USpnWardrobeComponent::USpnWardrobeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;   // event driven: nothing runs per frame
}

void USpnWardrobeComponent::OnRegister()
{
	Super::OnRegister();
	// bound at registration (not BeginPlay), so a body change re-attaches the wardrobe in every world, also before play begins
	if (USpnCharacterVisualComponent* Visual = GetOwner() != nullptr ? GetOwner()->FindComponentByClass<USpnCharacterVisualComponent>() : nullptr)
	{
		if (!PresentationHandle.IsValid()) { PresentationHandle = Visual->OnPresentationChanged.AddUObject(this, &USpnWardrobeComponent::Reapply); }
	}
}

void USpnWardrobeComponent::OnUnregister()
{
	if (USpnCharacterVisualComponent* Visual = GetOwner() != nullptr ? GetOwner()->FindComponentByClass<USpnCharacterVisualComponent>() : nullptr)
	{
		Visual->OnPresentationChanged.Remove(PresentationHandle);
	}
	PresentationHandle.Reset();
	Super::OnUnregister();
}

void USpnWardrobeComponent::BeginPlay()
{
	Super::BeginPlay();
	Reapply();
}

void USpnWardrobeComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	Clear();
	Super::EndPlay(Reason);
}

USkeletalMeshComponent* USpnWardrobeComponent::GetShownBody() const
{
	const AActor* Owner = GetOwner();
	if (Owner == nullptr) { return nullptr; }
	if (const USpnCharacterVisualComponent* Visual = Owner->FindComponentByClass<USpnCharacterVisualComponent>())
	{
		return Visual->GetShownBody();
	}
	return Owner->FindComponentByClass<USkeletalMeshComponent>();
}

void USpnWardrobeComponent::SetWorn(const TArray<FName>& Items)
{
	Worn = Items;
	Reapply();
}

void USpnWardrobeComponent::SetPolicy(const FSpnPresentationPolicy& InPolicy)
{
	Policy = InPolicy;
	Reapply();
}

void USpnWardrobeComponent::Clear()
{
	for (USkeletalMeshComponent* C : WearableComponents) { if (IsValid(C)) { C->DestroyComponent(); } }
	for (UPrimitiveComponent* C : DetailComponents) { if (IsValid(C)) { C->DestroyComponent(); } }
	WearableComponents.Reset();
	DetailComponents.Reset();
	if (IsValid(ZoneHiddenBody)) { ApplyZoneHiding(ZoneHiddenBody, 0); }
	ZoneHiddenBody = nullptr;
	HiddenSections = 0;
}

int32 USpnWardrobeComponent::ApplyZoneHiding(USkeletalMeshComponent* Body, int32 HiddenMask)
{
	// body geometry per zone = material slots named SpnZone_<zone> (a body split along the zones); every LOD's sections of that slot
	USkeletalMesh* Mesh = Body != nullptr ? Body->GetSkeletalMeshAsset() : nullptr;
	const FSkeletalMeshRenderData* Render = Mesh != nullptr ? Mesh->GetResourceForRendering() : nullptr;
	if (Render == nullptr) { return 0; }
	int32 Hidden = 0;
	const TArray<FSkeletalMaterial>& Slots = Mesh->GetMaterials();
	for (int32 Slot = 0; Slot < Slots.Num(); ++Slot)
	{
		const FString SlotName = Slots[Slot].MaterialSlotName.ToString();
		if (!SlotName.StartsWith(TEXT("SpnZone_"))) { continue; }
		bool bHide = false;
		for (int32 Z = 0; Z < SpnBodyCoverage::ZoneCount; ++Z)
		{
			if (SlotName.Equals(FString(TEXT("SpnZone_")) + SpnBodyCoverage::ZoneName(static_cast<ESpnBodyZone>(Z)), ESearchCase::IgnoreCase))
			{
				bHide = (HiddenMask & SpnBodyCoverage::ZoneBit(static_cast<ESpnBodyZone>(Z))) != 0;
			}
		}
		for (int32 Lod = 0; Lod < Render->LODRenderData.Num(); ++Lod)
		{
			const TArray<FSkelMeshRenderSection>& Sections = Render->LODRenderData[Lod].RenderSections;
			for (int32 S = 0; S < Sections.Num(); ++S)
			{
				if (Sections[S].MaterialIndex == Slot)
				{
					Body->ShowMaterialSection(Slot, S, !bHide, Lod);
					Hidden += bHide ? 1 : 0;
				}
			}
		}
	}
	return Hidden;
}

void USpnWardrobeComponent::Reapply()
{
	Clear();
	USkeletalMeshComponent* Body = GetShownBody();
	if (Body == nullptr) { return; }

	TArray<FSpnWearableCoverage> Coverage;
	TArray<const FSpnWearableDef*> WornDefs;
	TArray<FString> Unknown;
	for (const FName& Id : Worn)
	{
		const FSpnWearableDef* Def = Wearables.FindByPredicate([Id](const FSpnWearableDef& W) { return W.Coverage.Item == Id; });
		if (Def == nullptr) { Unknown.Add(Id.ToString()); continue; }
		Coverage.Add(Def->Coverage);
		WornDefs.Add(Def);
	}
	TArray<FSpnBodyDetailDef> DetailDefs;
	for (const FSpnBodyDetailItem& D : Details) { DetailDefs.Add(D.Def); }
	LastResult = SpnBodyCoverage::Resolve(Coverage, DetailDefs, Policy);
	for (const FString& U : Unknown) { LastResult.Issues.Add(FString::Printf(TEXT("unknown wearable '%s' (ignored)"), *U)); }

	// wearables: each its own component on the shown body
	for (const FSpnWearableDef* Def : WornDefs)
	{
		USkeletalMesh* Mesh = Def->Mesh.IsNull() ? nullptr : Def->Mesh.LoadSynchronous();
		if (Mesh == nullptr) { continue; }   // data-only or missing: coverage still counts, nothing drawn
		USkeletalMeshComponent* C = NewObject<USkeletalMeshComponent>(GetOwner(), NAME_None, RF_Transient);
		C->SetSkeletalMeshAsset(Mesh);
		C->SetupAttachment(Body);
		C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		C->RegisterComponent();
		C->SetLeaderPoseComponent(Body);
		C->SetOwnerNoSee(Body->bOwnerNoSee);
		WearableComponents.Add(C);
	}

	HiddenSections = ApplyZoneHiding(Body, LastResult.HiddenBody);
	ZoneHiddenBody = Body;

	// details: only what the rules returned; nothing else exists
	for (const FName& Id : LastResult.Details)
	{
		const FSpnBodyDetailItem* Item = Details.FindByPredicate([Id](const FSpnBodyDetailItem& D) { return D.Def.Id == Id; });
		if (Item == nullptr) { continue; }
		UPrimitiveComponent* Made = nullptr;
		if (USkeletalMesh* Skel = Item->SkeletalMesh.IsNull() ? nullptr : Item->SkeletalMesh.LoadSynchronous())
		{
			USkeletalMeshComponent* C = NewObject<USkeletalMeshComponent>(GetOwner(), NAME_None, RF_Transient);
			C->SetSkeletalMeshAsset(Skel);
			C->SetupAttachment(Body);
			C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			C->RegisterComponent();
			C->SetLeaderPoseComponent(Body);
			Made = C;
		}
		else if (UStaticMesh* Static = Item->StaticMesh.IsNull() ? nullptr : Item->StaticMesh.LoadSynchronous())
		{
			UStaticMeshComponent* C = NewObject<UStaticMeshComponent>(GetOwner(), NAME_None, RF_Transient);
			C->SetStaticMesh(Static);
			C->SetupAttachment(Body, Body->DoesSocketExist(Item->Bone) ? Item->Bone : NAME_None);
			C->SetRelativeTransform(Item->Offset);
			C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			C->SetCastShadow(false);
			C->RegisterComponent();
			Made = C;
		}
		if (Made != nullptr)
		{
			Made->SetCachedMaxDrawDistance(Item->MaxDrawDistance);
			Made->SetOwnerNoSee(Body->bOwnerNoSee);
			DetailComponents.Add(Made);
		}
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s: wardrobe - %d worn (%d drawn), covered 0x%x, hidden body 0x%x (%d section(s)), %d detail(s), %d issue(s)"),
		*GetNameSafe(GetOwner()), Worn.Num(), WearableComponents.Num(), LastResult.Covered, LastResult.HiddenBody, HiddenSections, DetailComponents.Num(), LastResult.Issues.Num());
}
