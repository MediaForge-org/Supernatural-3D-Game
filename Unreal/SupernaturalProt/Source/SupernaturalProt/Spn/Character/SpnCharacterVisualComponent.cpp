#include "Spn/Character/SpnCharacterVisualComponent.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Retargeter/IKRetargeter.h"
#include "Spn/Character/SpnRetargetFollowAnimInstance.h"
#include "SupernaturalProt.h"
#include "SupernaturalProtCharacter.h"

USpnCharacterVisualComponent::USpnCharacterVisualComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// PLACEHOLDER height tests on the mannequin (0013 S3): no asset, only the body height. Not a character choice.
	FSpnBodyPreset Short;
	Short.Id = TEXT("test_160");
	Short.HeightCm = 160.f;
	Presets.Add(Short);
	FSpnBodyPreset Tall;
	Tall.Id = TEXT("test_195");
	Tall.HeightCm = 195.f;
	Presets.Add(Tall);

	// PLACEHOLDER body collision tests (NightBuild06): the same heights with a capsule that follows the body. Not a gameplay decision.
	FSpnBodyPreset ShortFit = Short;
	ShortFit.Id = TEXT("test_160_fit");
	ShortFit.Collision = ESpnBodyCollision::FromHeight;
	Presets.Add(ShortFit);
	FSpnBodyPreset TallFit = Tall;
	TallFit.Id = TEXT("test_195_fit");
	TallFit.Collision = ESpnBodyCollision::FromHeight;
	Presets.Add(TallFit);
}

TArray<FName> USpnCharacterVisualComponent::GetPresetIds() const
{
	TArray<FName> Ids;
	for (const FSpnBodyPreset& P : Presets) { Ids.Add(P.Id); }
	for (const FSpnBodyPreset& P : ConfigPresets) { Ids.AddUnique(P.Id); }
	return Ids;
}

const FSpnBodyPreset* USpnCharacterVisualComponent::FindPreset(FName Id) const
{
	auto Match = [Id](const FSpnBodyPreset& P) { return P.Id == Id; };
	if (const FSpnBodyPreset* Found = Presets.FindByPredicate(Match)) { return Found; }
	return ConfigPresets.FindByPredicate(Match);
}

USkeletalMeshComponent* USpnCharacterVisualComponent::GetShownBody() const
{
	return RetargetBody != nullptr ? RetargetBody.Get() : GetBodyComponent();
}

void USpnCharacterVisualComponent::SetShownOwnerNoSee(bool bNoSee)
{
	if (USkeletalMeshComponent* Shown = GetShownBody()) { Shown->SetOwnerNoSee(bNoSee); }
	for (USkeletalMeshComponent* Follower : FollowerComponents)
	{
		if (IsValid(Follower)) { Follower->SetOwnerNoSee(bNoSee); }
	}
}

void USpnCharacterVisualComponent::ClearRetargetBody()
{
	if (IsValid(RetargetBody)) { RetargetBody->DestroyComponent(); }
	RetargetBody = nullptr;
	if (USkeletalMeshComponent* Body = GetBodyComponent())
	{
		Body->SetVisibility(true, false);
		Body->VisibilityBasedAnimTickOption = OriginalTickOption;
	}
}

USkeletalMeshComponent* USpnCharacterVisualComponent::GetBodyComponent() const
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	return Character != nullptr ? Character->GetMesh() : GetOwner() != nullptr ? GetOwner()->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
}

void USpnCharacterVisualComponent::CaptureOriginal()
{
	if (bOriginalCaptured)
	{
		return;
	}
	if (USkeletalMeshComponent* Body = GetBodyComponent())
	{
		OriginalMesh = Body->GetSkeletalMeshAsset();
		OriginalAnimClass = Body->GetAnimClass();
		OriginalMeshScale = Body->GetRelativeScale3D();
		OriginalTickOption = Body->VisibilityBasedAnimTickOption;
	}
	if (const ASupernaturalProtCharacter* Player = Cast<ASupernaturalProtCharacter>(GetOwner()))
	{
		OriginalCameraBase = Player->GetCameraBase();
	}
	bOriginalCaptured = true;
}

float USpnCharacterVisualComponent::GetReferenceHeightCm() const
{
	if (ReferenceHeightCm > 0.f)
	{
		return ReferenceHeightCm;
	}
	const USkeletalMeshComponent* Body = GetBodyComponent();
	const USkeletalMesh* Mesh = Body != nullptr ? Body->GetSkeletalMeshAsset() : nullptr;
	// the imported bounds of a standing body: feet to the top of the head, in the mesh's own units (cm)
	return Mesh != nullptr ? static_cast<float>(Mesh->GetImportedBounds().BoxExtent.Z * 2.0) : 180.f;
}

float USpnCharacterVisualComponent::GetBodyHeightCm() const
{
	return RetargetHeightCm > 0.f ? RetargetHeightCm : GetReferenceHeightCm() * CurrentScale;
}

void USpnCharacterVisualComponent::ClearFollowers()
{
	for (USkeletalMeshComponent* Follower : FollowerComponents)
	{
		if (IsValid(Follower)) { Follower->DestroyComponent(); }
	}
	FollowerComponents.Reset();
}

void USpnCharacterVisualComponent::BeginPlay()
{
	Super::BeginPlay();
	CaptureOriginal();
	FString FromCommandLine;
	if (FParse::Value(FCommandLine::Get(), TEXT("SpnBody="), FromCommandLine) && Cast<APawn>(GetOwner()) != nullptr && Cast<APawn>(GetOwner())->IsPlayerControlled())
	{
		ApplyPreset(FName(*FromCommandLine));
	}
	else if (!InitialPreset.IsNone())
	{
		ApplyPreset(InitialPreset);
	}
}

bool USpnCharacterVisualComponent::ApplyPreset(FName Id)
{
	USkeletalMeshComponent* Body = GetBodyComponent();
	if (Body == nullptr)
	{
		return false;
	}
	const FSpnBodyPreset* Preset = Id.IsNone() ? nullptr : FindPreset(Id);
	if (!Id.IsNone() && Preset == nullptr)
	{
		UE_LOG(LogSupernaturalProt, Warning, TEXT("%s: unknown body preset '%s'; the presentation stays '%s'"), *GetNameSafe(GetOwner()), *Id.ToString(), *Active.ToString());
		return false;
	}
	CaptureOriginal();
	ClearFollowers();
	ClearRetargetBody();
	MissingAssets = 0;

	// retargeted body on another skeleton: the gameplay mesh keeps its mesh, anim blueprint and scale, but is hidden and keeps evaluating
	USkeletalMesh* RetargetMesh = nullptr;
	UIKRetargeter* Retargeter = nullptr;
	if (Preset != nullptr && !Preset->RetargetMesh.IsNull())
	{
		RetargetMesh = Preset->RetargetMesh.LoadSynchronous();
		Retargeter = Preset->Retargeter.LoadSynchronous();
		if (RetargetMesh == nullptr || Retargeter == nullptr)
		{
			MissingAssets += (RetargetMesh == nullptr ? 1 : 0) + (Retargeter == nullptr ? 1 : 0);
			UE_LOG(LogSupernaturalProt, Warning, TEXT("Body preset '%s': retarget body %s or retargeter %s is missing - the mannequin is shown"),
				*Id.ToString(), *Preset->RetargetMesh.ToString(), *Preset->Retargeter.ToString());
			RetargetMesh = nullptr;
		}
	}
	const bool bRetarget = RetargetMesh != nullptr;

	// body mesh and anim class: the preset's if it has one and it loads, else the blueprint's (fallback, reported)
	USkeletalMesh* Mesh = OriginalMesh;
	if (Preset != nullptr && !bRetarget && !Preset->BodyMesh.IsNull())
	{
		if (USkeletalMesh* Loaded = Preset->BodyMesh.LoadSynchronous()) { Mesh = Loaded; }
		else { ++MissingAssets; UE_LOG(LogSupernaturalProt, Warning, TEXT("Body preset '%s': body mesh %s is missing - the mannequin is used"), *Id.ToString(), *Preset->BodyMesh.ToString()); }
	}
	TSubclassOf<UAnimInstance> AnimClass = OriginalAnimClass;
	if (Preset != nullptr && !bRetarget && !Preset->AnimClass.IsNull())
	{
		if (UClass* Loaded = Preset->AnimClass.LoadSynchronous()) { AnimClass = Loaded; }
		else { ++MissingAssets; UE_LOG(LogSupernaturalProt, Warning, TEXT("Body preset '%s': anim class %s is missing - the blueprint's is used"), *Id.ToString(), *Preset->AnimClass.ToString()); }
	}
	if (Body->GetSkeletalMeshAsset() != Mesh) { Body->SetSkeletalMeshAsset(Mesh); }
	if (Body->GetAnimClass() != AnimClass) { Body->SetAnimInstanceClass(AnimClass); }

	// height: scale the body around its origin (the feet, at the capsule bottom), the capsule stays as it is
	Body->SetRelativeScale3D(OriginalMeshScale);
	const float Reference = GetReferenceHeightCm();
	CurrentScale = Preset != nullptr && !bRetarget && Preset->HeightCm > 0.f && Reference > 1.f ? Preset->HeightCm / Reference : 1.f;
	Body->SetRelativeScale3D(OriginalMeshScale * CurrentScale);

	if (bRetarget)
	{
		// the gameplay mesh must keep its bones up to date while hidden: the retargeter reads its component-space pose every frame
		Body->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		Body->SetVisibility(false, false);

		RetargetBody = NewObject<USkeletalMeshComponent>(GetOwner(), MakeUniqueObjectName(GetOwner(), USkeletalMeshComponent::StaticClass(), TEXT("SpnRetargetBody")), RF_Transient);
		RetargetBody->SetSkeletalMeshAsset(RetargetMesh);
		RetargetBody->SetupAttachment(Body);   // same origin as the gameplay mesh: the feet at the capsule bottom
		RetargetBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// its own LOD from its own screen size: synced to the hidden parent it stayed at LOD 0 at every distance (measured 2026-11-14)
		RetargetBody->bSyncAttachParentLOD = false;
		RetargetBody->SetAnimInstanceClass(USpnRetargetFollowAnimInstance::StaticClass());
		RetargetBody->PrimaryComponentTick.AddPrerequisite(Body, Body->PrimaryComponentTick);   // read the pose after it was evaluated
		RetargetBody->RegisterComponent();
		RetargetBody->SetOwnerNoSee(Body->bOwnerNoSee);
		if (USpnRetargetFollowAnimInstance* Follow = Cast<USpnRetargetFollowAnimInstance>(RetargetBody->GetAnimInstance()))
		{
			Follow->Configure(Retargeter, Body, Preset->RetargetIKLodThreshold);
		}
		else
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("Body preset '%s': the retarget anim instance did not start"), *Id.ToString());
		}
	}
	USkeletalMeshComponent* Leader = bRetarget ? RetargetBody.Get() : Body;

	// followers copy the shown body's pose (clothing slots, head, hair)
	if (Preset != nullptr)
	{
		for (const TSoftObjectPtr<USkeletalMesh>& Soft : Preset->Followers)
		{
			USkeletalMesh* FollowerMesh = Soft.LoadSynchronous();
			if (FollowerMesh == nullptr)
			{
				++MissingAssets;
				UE_LOG(LogSupernaturalProt, Warning, TEXT("Body preset '%s': follower %s is missing - left out"), *Id.ToString(), *Soft.ToString());
				continue;
			}
			USkeletalMeshComponent* Follower = NewObject<USkeletalMeshComponent>(GetOwner(), NAME_None, RF_Transient);
			Follower->SetSkeletalMeshAsset(FollowerMesh);
			Follower->SetupAttachment(Leader);
			Follower->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Follower->RegisterComponent();
			Follower->SetLeaderPoseComponent(Leader);
			Follower->SetOwnerNoSee(Leader->bOwnerNoSee);
			FollowerComponents.Add(Follower);
		}
	}

	// camera pivot follows the head height; goes through the character so the stair smoothing keeps the new base
	RetargetHeightCm = bRetarget && Preset->HeightCm > 0.f ? Preset->HeightCm : 0.f;
	if (ASupernaturalProtCharacter* Player = Cast<ASupernaturalProtCharacter>(GetOwner()))
	{
		// gameplay collision of the body (Keep unless the preset opts in); a refusal keeps the old capsule and the presentation goes on
		const ESpnBodyCollision Mode = Preset != nullptr ? Preset->Collision : ESpnBodyCollision::Keep;
		LastCollision = SpnBodyCollision::Resolve(Mode, GetBodyHeightCm(), Preset != nullptr ? Preset->ExplicitCollision : FSpnBodyCollisionProfile(),
			Player->GetDefaultBodyCollision(), CollisionHeadClearanceCm);
		bCollisionRefused = !Player->ApplyBodyCollision(LastCollision.Profile);
		if (bCollisionRefused || LastCollision.Issues.Num() > 0)
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s: body preset '%s' collision %s%s"), *GetNameSafe(GetOwner()), *Id.ToString(),
				bCollisionRefused ? TEXT("refused by the character (old capsule kept)") : TEXT("applied"),
				LastCollision.Issues.Num() > 0 ? *(TEXT(", issues: ") + FString::Join(LastCollision.Issues, TEXT(", "))) : TEXT(""));
		}

		// a retargeted body is never scaled; its measured height only moves the camera pivot like a scaled body of that height would.
		// The capsule centre shift is taken out: the camera base is relative to the capsule centre, its height above the floor must not change.
		const float DeltaZ = RetargetHeightCm > 0.f ? (RetargetHeightCm - Reference) * CameraHeightFactor : (CurrentScale - 1.f) * Reference * CameraHeightFactor;
		Player->SetCameraBase(OriginalCameraBase + FVector(0.f, 0.f, DeltaZ - Player->GetCapsuleCentreShift()));
	}

	Active = Preset != nullptr ? Id : NAME_None;
	OnPresentationChanged.Broadcast();
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s: body preset '%s' - height %.0f cm (reference %.0f, scale %.3f), %s, %d follower(s), %d missing asset(s)"),
		*GetNameSafe(GetOwner()), *Active.ToString(), GetBodyHeightCm(), Reference, CurrentScale,
		bRetarget ? *FString::Printf(TEXT("retargeted body %s"), *GetNameSafe(RetargetMesh)) : TEXT("gameplay mesh shown"), FollowerComponents.Num(), MissingAssets);
	return true;
}

static FAutoConsoleCommandWithWorldAndArgs GSpnBodyCommand(
	TEXT("Spn.Body"),
	TEXT("Spn.Body <preset>|none : body presentation of the player character (PLACEHOLDER presets test_160, test_195). No argument: lists them."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		for (TActorIterator<ASupernaturalProtCharacter> It(World); It; ++It)
		{
			USpnCharacterVisualComponent* Visual = It->FindComponentByClass<USpnCharacterVisualComponent>();
			if (Visual == nullptr || !It->IsPlayerControlled())
			{
				continue;
			}
			if (Args.Num() == 0)
			{
				for (const FName& Id : Visual->GetPresetIds())
				{
					UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Body: %s%s"), *Id.ToString(), Id == Visual->GetActivePreset() ? TEXT(" (active)") : TEXT(""));
				}
				UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Body: active '%s', %.0f cm"), *Visual->GetActivePreset().ToString(), Visual->GetBodyHeightCm());
				return;
			}
			Visual->ApplyPreset(Args[0].Equals(TEXT("none"), ESearchCase::IgnoreCase) ? FName() : FName(*Args[0]));
			return;
		}
		UE_LOG(LogSupernaturalProt, Warning, TEXT("Spn.Body: no player character with a visual component"));
	}));
