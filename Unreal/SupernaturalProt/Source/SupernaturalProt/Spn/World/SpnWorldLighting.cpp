#include "Spn/World/SpnWorldLighting.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/LightComponent.h"
#include "Components/LocalFogVolumeComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/Engine.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SkyLight.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Engine/GameInstance.h"
#include "Spn/SpnSettingsSubsystem.h"

const FName ASpnWorldLighting::SunTag(TEXT("SpnSun"));
const FName ASpnWorldLighting::MoonTag(TEXT("SpnMoon"));
const FName ASpnWorldLighting::PostTag(TEXT("SpnPost"));
const FName ASpnWorldLighting::InteriorPostTag(TEXT("SpnPostInterior"));
const FName ASpnWorldLighting::LampTag(TEXT("SpnLamp"));
const FName ASpnWorldLighting::MistTag(TEXT("SpnMist"));

ASpnWorldLighting::ASpnWorldLighting()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

ASpnWorldLighting* ASpnWorldLighting::Find(const UWorld* World)
{
	if (World != nullptr)
	{
		for (TActorIterator<ASpnWorldLighting> It(World); It; ++It)
		{
			return *It;
		}
	}
	return nullptr;
}

TArray<FName> ASpnWorldLighting::GetScenarioNames() const
{
	TArray<FName> Names;
	for (const FSpnLightingScenario& S : Scenarios)
	{
		Names.Add(S.Name);
	}
	return Names;
}

void ASpnWorldLighting::BeginPlay()
{
	Super::BeginPlay();
	if (Scenarios.Num() > 0)
	{
		ApplyScenario(InitialScenario.IsNone() ? Scenarios[0].Name : InitialScenario);
	}
	// the player's choice in the graphics settings (standard 90 % / quality 100 %), live while the level runs
	USpnSettingsSubsystem* Settings = GetGameInstance() != nullptr ? GetGameInstance()->GetSubsystem<USpnSettingsSubsystem>() : nullptr;
	bAppliedSetting = Settings != nullptr && Settings->Get().bWorldQualityMode;
	if (Settings != nullptr)
	{
		SettingsChangedHandle = Settings->OnChanged.AddUObject(this, &ASpnWorldLighting::HandleSettingsChanged);
	}
	if (DefaultScreenPercentage > 0.f)     // 0 = this level leaves the render resolution alone, whatever the setting says
	{
		SetQualityMode(bAppliedSetting);
	}
}

void ASpnWorldLighting::HandleSettingsChanged()
{
	const USpnSettingsSubsystem* Settings = GetGameInstance() != nullptr ? GetGameInstance()->GetSubsystem<USpnSettingsSubsystem>() : nullptr;
	if (Settings == nullptr || Settings->Get().bWorldQualityMode == bAppliedSetting)
	{
		return;     // another setting changed: a session choice made with Spn.Quality stays
	}
	bAppliedSetting = Settings->Get().bWorldQualityMode;
	if (DefaultScreenPercentage > 0.f)
	{
		SetQualityMode(bAppliedSetting);
	}
}

void ASpnWorldLighting::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SettingsChangedHandle.IsValid())
	{
		if (USpnSettingsSubsystem* Settings = GetGameInstance() != nullptr ? GetGameInstance()->GetSubsystem<USpnSettingsSubsystem>() : nullptr)
		{
			Settings->OnChanged.Remove(SettingsChangedHandle);
		}
		SettingsChangedHandle.Reset();
	}
	if (bScreenPercentageSet)
	{
		// give the previous value back (another level, the editor, a QA test map); a value typed on the console has a higher priority and stays
		if (IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage")))
		{
			Var->Set(PreviousScreenPercentage, ECVF_SetByCode);
		}
		bScreenPercentageSet = false;
		UE_LOG(LogTemp, Display, TEXT("SpnWorldLighting: r.ScreenPercentage restored to %.0f"), PreviousScreenPercentage);
	}
	Super::EndPlay(EndPlayReason);
}

float ASpnWorldLighting::GetScreenPercentage()
{
	const IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	return Var != nullptr ? Var->GetFloat() : 100.f;
}

void ASpnWorldLighting::ApplyScreenPercentage(float Percent)
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (Var == nullptr || Percent <= 0.f)
	{
		return;
	}
	if (!bScreenPercentageSet)
	{
		PreviousScreenPercentage = Var->GetFloat();
		bScreenPercentageSet = true;
	}
	// SetByCode: above scalability / game user settings, below the console (a developer's `r.ScreenPercentage` and the perf specs still win)
	Var->Set(Percent, ECVF_SetByCode);
	UE_LOG(LogTemp, Display, TEXT("SpnWorldLighting: r.ScreenPercentage %.0f (%s mode, now %.0f)"), Percent, bQualityMode ? TEXT("quality") : TEXT("standard"), Var->GetFloat());
}

float ASpnWorldLighting::SetQualityMode(bool bQuality)
{
	bQualityMode = bQuality;
	const float Percent = bQuality ? QualityScreenPercentage : DefaultScreenPercentage;
	if (Percent > 0.f)
	{
		ApplyScreenPercentage(Percent);
	}
	return GetScreenPercentage();
}

bool ASpnWorldLighting::ApplyScenario(FName Name)
{
	const FSpnLightingScenario* S = Scenarios.FindByPredicate([Name](const FSpnLightingScenario& X) { return X.Name == Name; });
	UWorld* World = GetWorld();
	if (S == nullptr || World == nullptr)
	{
		return false;
	}

	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		UDirectionalLightComponent* Light = Cast<UDirectionalLightComponent>(It->GetLightComponent());
		if (Light == nullptr)
		{
			continue;
		}
		if (It->ActorHasTag(MoonTag))
		{
			Light->SetIntensity(S->MoonLux);
			Light->SetLightColor(S->MoonColor);
			Light->SetVisibility(S->MoonLux > 0.f);
		}
		else
		{
			It->SetActorRotation(FRotator(S->SunPitch, S->SunYaw, 0.f));
			Light->SetIntensity(S->SunLux);
			Light->SetLightColor(S->SunColor);
			Light->SetLightSourceAngle(S->SunSourceAngle);
			Light->SetVisibility(S->SunLux > 0.f);
		}
	}

	for (TActorIterator<ASkyLight> It(World); It; ++It)
	{
		if (USkyLightComponent* Sky = It->GetLightComponent())
		{
			Sky->SetIntensity(S->SkyLightIntensity);
			Sky->SetLightColor(S->SkyLightColor);
			Sky->RecaptureSky();
		}
	}

	for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
	{
		if (UExponentialHeightFogComponent* Fog = It->GetComponent())
		{
			Fog->SetFogDensity(S->FogDensity);
			Fog->SetFogHeightFalloff(S->FogHeightFalloff);
			Fog->SetFogInscatteringColor(S->FogInscatteringColor);
			Fog->SetVolumetricFogAlbedo(S->VolumetricFogAlbedo.ToFColor(true));
			Fog->SetVolumetricFogExtinctionScale(S->VolumetricFogExtinctionScale);
			Fog->SetVolumetricFogScatteringDistribution(S->VolumetricFogScatteringDistribution);
		}
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (UVolumetricCloudComponent* Cloud = It->FindComponentByClass<UVolumetricCloudComponent>())
		{
			Cloud->SetVisibility(S->bCloudsVisible);
			if (S->CloudScalars.Num() > 0 && Cloud->GetMaterial() != nullptr)
			{
				UMaterialInstanceDynamic* Dynamic = Cast<UMaterialInstanceDynamic>(Cloud->GetMaterial());
				if (Dynamic == nullptr)
				{
					Dynamic = UMaterialInstanceDynamic::Create(Cloud->GetMaterial(), this);
					Cloud->SetMaterial(Dynamic);
				}
				for (const TPair<FName, float>& P : S->CloudScalars)
				{
					Dynamic->SetScalarParameterValue(P.Key, P.Value);
				}
			}
		}
		if (It->ActorHasTag(LampTag))
		{
			TInlineComponentArray<ULightComponent*> Lights(*It);
			for (ULightComponent* Light : Lights)
			{
				Light->SetVisibility(S->bLampsOn);
			}
		}
		if (It->ActorHasTag(MistTag))
		{
			// the generator stores each patch's own extinction in its first tag after SpnMist ("Mist=<value>"), so scenarios only scale it
			float Base = 1.f;
			for (const FName& Tag : It->Tags)
			{
				const FString Text = Tag.ToString();
				if (Text.StartsWith(TEXT("Mist=")))
				{
					Base = FCString::Atof(*Text.RightChop(5));
				}
			}
			TInlineComponentArray<ULocalFogVolumeComponent*> Patches(*It);
			for (ULocalFogVolumeComponent* Patch : Patches)
			{
				Patch->SetVisibility(S->MistDensity > 0.f);
				Patch->SetRadialFogExtinction(Base * S->MistDensity);
				Patch->SetHeightFogExtinction(Base * S->MistDensity);
				Patch->SetFogAlbedo(S->MistAlbedo);
			}
		}
	}

	for (TActorIterator<APostProcessVolume> It(World); It; ++It)
	{
		if (It->ActorHasTag(InteriorPostTag))
		{
			// a room: only its own exposure range, everything else stays with the unbound volume
			It->Settings.bOverride_AutoExposureMinBrightness = true; It->Settings.AutoExposureMinBrightness = S->InteriorExposureMinEV;
			It->Settings.bOverride_AutoExposureMaxBrightness = true; It->Settings.AutoExposureMaxBrightness = S->InteriorExposureMaxEV;
			continue;
		}
		if (!It->ActorHasTag(PostTag))
		{
			continue;
		}
		FPostProcessSettings& P = It->Settings;
		P.bOverride_AutoExposureBias = true; P.AutoExposureBias = S->ExposureCompensation;
		P.bOverride_AutoExposureMinBrightness = true; P.AutoExposureMinBrightness = S->ExposureMinEV;
		P.bOverride_AutoExposureMaxBrightness = true; P.AutoExposureMaxBrightness = S->ExposureMaxEV;
		P.bOverride_WhiteTemp = true; P.WhiteTemp = S->WhiteTemp;
		P.bOverride_ColorSaturation = true; P.ColorSaturation = FVector4(S->Saturation, S->Saturation, S->Saturation, 1.0);
		P.bOverride_ColorContrast = true; P.ColorContrast = FVector4(S->Contrast, S->Contrast, S->Contrast, 1.0);
		P.bOverride_ColorGain = true; P.ColorGain = FVector4(S->ColorGain.R, S->ColorGain.G, S->ColorGain.B, 1.0);
		P.bOverride_BloomIntensity = true; P.BloomIntensity = S->BloomIntensity;
		P.bOverride_VignetteIntensity = true; P.VignetteIntensity = S->VignetteIntensity;
	}

	if (Collection != nullptr)
	{
		for (const TPair<FName, float>& P : S->CollectionScalars)
		{
			UKismetMaterialLibrary::SetScalarParameterValue(this, Collection, P.Key, P.Value);
		}
	}

	if (GEngine != nullptr)
	{
		for (const FString& Command : S->ConsoleCommands)
		{
			GEngine->Exec(World, *Command);
		}
	}

	Current = Name;
	UE_LOG(LogTemp, Display, TEXT("SpnWorldLighting: scenario '%s' applied"), *Name.ToString());
	return true;
}

static FAutoConsoleCommandWithWorldAndArgs GSpnLightingCommand(
	TEXT("Spn.Lighting"),
	TEXT("Spn.Lighting <scenario>|next : applies a lighting scenario of the level's ASpnWorldLighting (no argument: lists them)."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		ASpnWorldLighting* Lighting = ASpnWorldLighting::Find(World);
		if (Lighting == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spn.Lighting: this level has no ASpnWorldLighting"));
			return;
		}
		const TArray<FName> Names = Lighting->GetScenarioNames();
		if (Args.Num() == 0 || Names.Num() == 0)
		{
			for (const FName& N : Names)
			{
				UE_LOG(LogTemp, Display, TEXT("Spn.Lighting: %s%s"), *N.ToString(), N == Lighting->GetCurrentScenario() ? TEXT(" (current)") : TEXT(""));
			}
			return;
		}
		FName Wanted(*Args[0]);
		if (Args[0].Equals(TEXT("next"), ESearchCase::IgnoreCase))
		{
			Wanted = Names[(Names.IndexOfByKey(Lighting->GetCurrentScenario()) + 1) % Names.Num()];
		}
		if (!Lighting->ApplyScenario(Wanted))
		{
			UE_LOG(LogTemp, Warning, TEXT("Spn.Lighting: unknown scenario '%s'"), *Args[0]);
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs GSpnQualityCommand(
	TEXT("Spn.Quality"),
	TEXT("Spn.Quality standard|quality : render resolution of the level's ASpnWorldLighting (SpnWorld: standard 90 %, quality 100 %, TSR). No argument: shows the mode."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		ASpnWorldLighting* Lighting = ASpnWorldLighting::Find(World);
		if (Lighting == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spn.Quality: this level has no ASpnWorldLighting; r.ScreenPercentage stays %.0f"), ASpnWorldLighting::GetScreenPercentage());
			return;
		}
		if (Args.Num() > 0)
		{
			const FString Mode = Args[0].ToLower();
			if (Mode == TEXT("quality") || Mode == TEXT("high") || Mode == TEXT("100"))
			{
				Lighting->SetQualityMode(true);
			}
			else if (Mode == TEXT("standard") || Mode == TEXT("default") || Mode == TEXT("90"))
			{
				Lighting->SetQualityMode(false);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Spn.Quality: unknown mode '%s' (standard | quality)"), *Args[0]);
			}
		}
		UE_LOG(LogTemp, Display, TEXT("Spn.Quality: %s, r.ScreenPercentage %.0f"), Lighting->IsQualityMode() ? TEXT("quality") : TEXT("standard"), ASpnWorldLighting::GetScreenPercentage());
	}));
