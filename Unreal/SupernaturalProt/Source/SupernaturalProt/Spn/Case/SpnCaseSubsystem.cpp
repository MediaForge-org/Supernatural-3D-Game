#include "Spn/Case/SpnCaseSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Spn/Case/SpnCaseCreature.h"
#include "Spn/Case/SpnCaseObjects.h"
#include "Spn/SpnSessionSubsystem.h"
#include "SpnNpcReaction.h"
#include "SupernaturalProt.h"

const TArray<FSpnDifficultyProfile>& USpnCaseSettings::GetProfiles() const
{
	static TArray<FSpnDifficultyProfile> BuiltIn = []()
	{
		// two TEST profiles so both ends are exercised; the developer's real list (about ten levels, names open) goes into the config
		FSpnDifficultyProfile Standard;
		Standard.Id = TEXT("test_standard");
		Standard.DisplayName = TEXT("Test: standard");
		Standard.RespawnHealth = 0.6f;
		Standard.CreatureReactionDelay = 1.5f;
		Standard.CreatureSearchSeconds = 20.f;
		Standard.CreatureHearingScale = 1.f;
		FSpnDifficultyProfile Hard;
		Hard.Id = TEXT("test_hardest");
		Hard.DisplayName = TEXT("Test: hardest");
		Hard.RespawnHealth = 0.1f;
		Hard.CreatureReactionDelay = 0.f;
		Hard.CreatureSearchSeconds = 45.f;
		Hard.CreatureHearingScale = 1.3f;
		return TArray<FSpnDifficultyProfile>{Standard, Hard};
	}();
	FString Error;
	if (Difficulties.Num() > 0 && SpnDifficulty::ValidateList(Difficulties, Error))
	{
		return Difficulties;
	}
	if (Difficulties.Num() > 0)
	{
		static bool bLogged = false;
		if (!bLogged)
		{
			bLogged = true;
			UE_LOG(LogSupernaturalProt, Error, TEXT("Case: the configured difficulty list is invalid (%s); the built-in test profiles are used"), *Error);
		}
	}
	return BuiltIn;
}

bool USpnCaseSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World != nullptr && (World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::GamePreview ||
		World->WorldType == EWorldType::Inactive || World->WorldType == EWorldType::None);
}

void USpnCaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const USpnCaseSettings* Settings = GetDefault<USpnCaseSettings>();
	const TArray<FSpnDifficultyProfile>& Profiles = Settings->GetProfiles();
	FString Wanted = Settings->DefaultDifficulty;
	FParse::Value(FCommandLine::Get(), TEXT("SpnDifficulty="), Wanted);
	DifficultyIndex = FMath::Max(0, SpnDifficulty::Find(Profiles, Wanted));
	Health = Settings->PlayerMaxHealth;
}

const FSpnDifficultyProfile& USpnCaseSubsystem::GetDifficulty() const
{
	const TArray<FSpnDifficultyProfile>& Profiles = GetDefault<USpnCaseSettings>()->GetProfiles();
	return Profiles[FMath::Clamp(DifficultyIndex, 0, Profiles.Num() - 1)];
}

bool USpnCaseSubsystem::SetDifficulty(const FString& Id)
{
	const int32 Index = SpnDifficulty::Find(GetDefault<USpnCaseSettings>()->GetProfiles(), Id);
	if (Index == INDEX_NONE)
	{
		return false;
	}
	DifficultyIndex = Index;
	for (ASpnCreature* Creature : GetCreatures())
	{
		Creature->ApplyDifficulty(GetDifficulty());
	}
	return true;
}

float USpnCaseSubsystem::GetMaxHealth() const
{
	return GetDefault<USpnCaseSettings>()->PlayerMaxHealth;
}

void USpnCaseSubsystem::RegisterCreature(ASpnCreature* Creature)
{
	if (Creature == nullptr)
	{
		return;
	}
	Creatures.AddUnique(Creature);
	Creature->ApplyDifficulty(GetDifficulty());
	Creature->SetFrozen(bPlayerDead);
}

void USpnCaseSubsystem::UnregisterCreature(ASpnCreature* Creature)
{
	Creatures.Remove(Creature);
}

TArray<ASpnCreature*> USpnCaseSubsystem::GetCreatures() const
{
	TArray<ASpnCreature*> Out;
	for (const TWeakObjectPtr<ASpnCreature>& C : Creatures)
	{
		if (C.IsValid())
		{
			Out.Add(C.Get());
		}
	}
	return Out;
}

bool USpnCaseSubsystem::IsSaveBlocked() const
{
	TArray<ESpnCreatureState> States;
	for (const ASpnCreature* C : GetCreatures())
	{
		States.Add(C->GetCreatureState());
	}
	return !SpnCreatureRules::SaveAllowed(States);
}

APawn* USpnCaseSubsystem::GetPlayerPawn() const
{
	if (PlayerOverride.IsValid())
	{
		return PlayerOverride.Get();
	}
	return GetWorld() != nullptr ? UGameplayStatics::GetPlayerPawn(GetWorld(), 0) : nullptr;
}

void USpnCaseSubsystem::CaptureStartIfNeeded()
{
	if (bHasStart)
	{
		return;
	}
	if (const APawn* Pawn = GetPlayerPawn())
	{
		PlayerStart = Pawn->GetActorTransform();
		bHasStart = true;
	}
}

void USpnCaseSubsystem::SetPlayerFrozen(bool bFrozen)
{
	APawn* Pawn = GetPlayerPawn();
	if (Pawn == nullptr)
	{
		return;
	}
	if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
	{
		PC->SetIgnoreMoveInput(bFrozen);
		PC->SetIgnoreLookInput(bFrozen);
	}
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
	}
}

void USpnCaseSubsystem::DamagePlayer(float Amount, const FString& Cause)
{
	if (bPlayerDead || !FMath::IsFinite(Amount) || Amount <= 0.f)
	{
		return;
	}
	CaptureStartIfNeeded();
	Health = FMath::Max(0.f, Health - Amount);
	if (Health > 0.f)
	{
		return;
	}
	bPlayerDead = true;
	++DeathCount;
	LastDeathCause = Cause;
	SetPlayerFrozen(true);
	for (ASpnCreature* Creature : GetCreatures())
	{
		Creature->SetFrozen(true);
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("Case: the player died (%s), death %d"), *Cause, DeathCount);
	OnPlayerDied.Broadcast(Cause);
}

bool USpnCaseSubsystem::Respawn()
{
	if (!bPlayerDead)
	{
		return false;
	}
	UGameInstance* Instance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr;
	USpnSessionSubsystem* Session = Instance != nullptr ? Instance->GetSubsystem<USpnSessionSubsystem>() : nullptr;
	bool bLoaded = false;
	if (Session != nullptr && Session->IsGameFlowEnabled() && !Session->IsMenuWorld())
	{
		const FString Slot = Session->FindContinueSlot();
		if (!Slot.IsEmpty())
		{
			const FSpnSaveStatus Status = Session->LoadGame(Slot);
			bLoaded = Status.IsOk();
			UE_LOG(LogSupernaturalProt, Log, TEXT("Case: respawn from save '%s': %s %s"), *Slot, LexToString(Status.Result), *Status.Detail);
		}
	}
	if (!bLoaded)
	{
		CaptureStartIfNeeded();
		if (APawn* Pawn = GetPlayerPawn(); Pawn != nullptr && bHasStart)
		{
			Pawn->SetActorLocationAndRotation(PlayerStart.GetLocation(), PlayerStart.Rotator(), false, nullptr, ETeleportType::TeleportPhysics);
			if (AController* Controller = Pawn->GetController())
			{
				Controller->SetControlRotation(PlayerStart.Rotator());
			}
		}
		UE_LOG(LogSupernaturalProt, Log, TEXT("Case: respawn at the level start (no save, or no game flow)"));
	}
	for (ASpnCreature* Creature : GetCreatures())
	{
		Creature->ResetToSpawn();
		Creature->SetFrozen(false);
	}
	Health = SpnDifficulty::RespawnHealth(GetDifficulty(), GetMaxHealth());
	bPlayerDead = false;
	SetPlayerFrozen(false);
	UE_LOG(LogSupernaturalProt, Log, TEXT("Case: respawned with %.0f / %.0f health (difficulty %s)"), Health, GetMaxHealth(), *GetDifficulty().Id);
	OnPlayerRespawned.Broadcast();
	return true;
}

bool USpnCaseSubsystem::RequestAutosave(FName PointId)
{
	CaptureStartIfNeeded();
	if (bPlayerDead || AutosavedPoints.Contains(PointId) || IsSaveBlocked())
	{
		return false;
	}
	UGameInstance* Instance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr;
	USpnSessionSubsystem* Session = Instance != nullptr ? Instance->GetSubsystem<USpnSessionSubsystem>() : nullptr;
	if (Session == nullptr || !Session->IsGameFlowEnabled() || Session->IsMenuWorld())
	{
		return false;
	}
	const FSpnSaveStatus Status = Session->SaveGame(AutosaveSlot());
	if (!Status.IsOk())
	{
		UE_LOG(LogSupernaturalProt, Warning, TEXT("Case: autosave at '%s' failed: %s %s"), *PointId.ToString(), LexToString(Status.Result), *Status.Detail);
		return false;
	}
	AutosavedPoints.Add(PointId);
	++AutosaveCount;
	UE_LOG(LogSupernaturalProt, Log, TEXT("Case: autosave at '%s'"), *PointId.ToString());
	return true;
}

int32 USpnCaseSubsystem::RingBell(const FVector& LocationCm, AActor* Ringer)
{
	const USpnCaseSettings* Settings = GetDefault<USpnCaseSettings>();
	if (UGameInstance* Instance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (USpnNoiseSubsystem* Noise = Instance->GetSubsystem<USpnNoiseSubsystem>())
		{
			FSpnNoise N;
			N.LocationCm = LocationCm;
			N.RangeCm = Settings->BellRangeCm * 3.f; // heard much farther than it stuns
			N.Strength = 1.f;
			N.Kind = ESpnNoiseKind::Impact;
			N.SourceId = TEXT("placeholder.case.bell");
			Noise->EmitNoise(N);
		}
	}
	int32 Stunned = 0;
	for (ASpnCreature* Creature : GetCreatures())
	{
		const float Distance = FVector::Dist(LocationCm, Creature->GetActorLocation());
		const int32 Walls = Creature->CountWallsBetween(LocationCm, Creature->GetEarLocation());
		if (SpnCreatureRules::BellReaches(Distance, Settings->BellRangeCm, Walls) && Creature->StunByBell())
		{
			++Stunned;
		}
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("Case: bell rung by %s, %d creature(s) stunned"), Ringer != nullptr ? *Ringer->GetName() : TEXT("?"), Stunned);
	return Stunned;
}

bool USpnCaseSubsystem::AddThrowable()
{
	if (Throwables >= GetDefault<USpnCaseSettings>()->MaxThrowables)
	{
		return false;
	}
	++Throwables;
	return true;
}

ASpnThrownObject* USpnCaseSubsystem::Throw(APawn* Thrower)
{
	if (Thrower == nullptr || Throwables <= 0 || bPlayerDead || GetWorld() == nullptr)
	{
		return nullptr;
	}
	FVector Eye;
	FRotator View;
	Thrower->GetActorEyesViewPoint(Eye, View);
	if (const AController* Controller = Thrower->GetController())
	{
		View = Controller->GetControlRotation();
	}
	View.Pitch = FMath::Clamp(View.Pitch + 8.f, -60.f, 70.f); // a slight lob
	const FVector Start = Thrower->GetActorLocation() + FVector(0.f, 0.f, 60.f) + Thrower->GetActorForwardVector() * 60.f;
	FActorSpawnParameters Params;
	Params.Owner = Thrower;
	Params.Instigator = Thrower;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASpnThrownObject* Thrown = GetWorld()->SpawnActor<ASpnThrownObject>(ASpnThrownObject::StaticClass(), FTransform(View, Start), Params);
	if (Thrown == nullptr)
	{
		return nullptr;
	}
	Thrown->Launch(View.Vector() * GetDefault<USpnCaseSettings>()->ThrowSpeedCm);
	--Throwables;
	return Thrown;
}
