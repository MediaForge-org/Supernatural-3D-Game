#include "Spn/SpnSettingsSubsystem.h"

#include "Engine/Engine.h"
#include "HAL/IConsoleManager.h"
#include "Misc/App.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/CommandLine.h"
#include "RHI.h"
#include "Spn/UI/SpnUiText.h"

namespace
{
	const TCHAR* const Section = TEXT("SpnPlayer");

	UGameUserSettings* Gus()
	{
		return GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
	}
}

FSpnPlayerSettings USpnSettingsSubsystem::Clamped(const FSpnPlayerSettings& In)
{
	FSpnPlayerSettings Out = In;
	Out.MasterVolume = FMath::Clamp(FMath::IsFinite(In.MasterVolume) ? In.MasterVolume : 1.f, 0.f, 1.f);
	Out.MouseSensitivity = FMath::Clamp(FMath::IsFinite(In.MouseSensitivity) ? In.MouseSensitivity : 1.f, 0.1f, 4.f);
	Out.GamepadSensitivity = FMath::Clamp(FMath::IsFinite(In.GamepadSensitivity) ? In.GamepadSensitivity : 1.f, 0.1f, 4.f);
	Out.MinimapZoom = FMath::Clamp(FMath::IsFinite(In.MinimapZoom) ? In.MinimapZoom : 1.f, 0.5f, 4.f);
	if (Out.Language != TEXT("en") && Out.Language != TEXT("de"))
	{
		Out.Language = TEXT("auto");
	}
	return Out;
}

void USpnSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Load();
	ApplyGlobal();
}

void USpnSettingsSubsystem::Load()
{
	FSpnPlayerSettings Loaded;
	if (GConfig != nullptr)
	{
		GConfig->GetFloat(Section, TEXT("MasterVolume"), Loaded.MasterVolume, GGameUserSettingsIni);
		GConfig->GetFloat(Section, TEXT("MouseSensitivity"), Loaded.MouseSensitivity, GGameUserSettingsIni);
		GConfig->GetFloat(Section, TEXT("GamepadSensitivity"), Loaded.GamepadSensitivity, GGameUserSettingsIni);
		GConfig->GetBool(Section, TEXT("InvertY"), Loaded.bInvertY, GGameUserSettingsIni);
		GConfig->GetBool(Section, TEXT("MinimapVisible"), Loaded.bMinimapVisible, GGameUserSettingsIni);
		GConfig->GetBool(Section, TEXT("MinimapHeadingUp"), Loaded.bMinimapHeadingUp, GGameUserSettingsIni);
		GConfig->GetFloat(Section, TEXT("MinimapZoom"), Loaded.MinimapZoom, GGameUserSettingsIni);
		GConfig->GetBool(Section, TEXT("CameraSmoothing"), Loaded.bCameraSmoothing, GGameUserSettingsIni);
		GConfig->GetString(Section, TEXT("Language"), Loaded.Language, GGameUserSettingsIni);
	}
	Settings = Clamped(Loaded);
}

void USpnSettingsSubsystem::SaveToDisk() const
{
	if (GConfig == nullptr)
	{
		return;
	}
	GConfig->SetFloat(Section, TEXT("MasterVolume"), Settings.MasterVolume, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("MouseSensitivity"), Settings.MouseSensitivity, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("GamepadSensitivity"), Settings.GamepadSensitivity, GGameUserSettingsIni);
	GConfig->SetBool(Section, TEXT("InvertY"), Settings.bInvertY, GGameUserSettingsIni);
	GConfig->SetBool(Section, TEXT("MinimapVisible"), Settings.bMinimapVisible, GGameUserSettingsIni);
	GConfig->SetBool(Section, TEXT("MinimapHeadingUp"), Settings.bMinimapHeadingUp, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("MinimapZoom"), Settings.MinimapZoom, GGameUserSettingsIni);
	GConfig->SetBool(Section, TEXT("CameraSmoothing"), Settings.bCameraSmoothing, GGameUserSettingsIni);
	GConfig->SetString(Section, TEXT("Language"), *Settings.Language, GGameUserSettingsIni);
	GConfig->Flush(false, GGameUserSettingsIni);
}

void USpnSettingsSubsystem::ApplyGlobal() const
{
	FApp::SetVolumeMultiplier(Settings.MasterVolume);
	FSpnUiText::SetLanguageOverride(Settings.Language);
}

void USpnSettingsSubsystem::Set(const FSpnPlayerSettings& NewSettings)
{
	Settings = Clamped(NewSettings);
	ApplyGlobal();
	SaveToDisk();
	OnChanged.Broadcast();
}

bool USpnSettingsSubsystem::CanChangeDisplay()
{
	return Gus() != nullptr && !IsRunningCommandlet() && !IsRunningDedicatedServer() && FApp::CanEverRender();
}

void USpnSettingsSubsystem::ApplyAndSave()
{
	if (UGameUserSettings* const Settings = Gus())
	{
		Settings->ApplySettings(false);
		Settings->SaveSettings();
	}
}

EWindowMode::Type USpnSettingsSubsystem::GetDisplayMode()
{
	UGameUserSettings* const S = Gus();
	return S != nullptr ? S->GetFullscreenMode() : EWindowMode::Windowed;
}

void USpnSettingsSubsystem::SetDisplayMode(EWindowMode::Type Mode)
{
	if (UGameUserSettings* const S = Gus())
	{
		S->SetFullscreenMode(Mode);
		ApplyAndSave();
	}
}

FIntPoint USpnSettingsSubsystem::GetResolution()
{
	UGameUserSettings* const S = Gus();
	return S != nullptr ? S->GetScreenResolution() : FIntPoint(1280, 720);
}

TArray<FIntPoint> USpnSettingsSubsystem::GetSupportedResolutions()
{
	TArray<FIntPoint> Out;
	FScreenResolutionArray Resolutions;
	if (RHIGetAvailableResolutions(Resolutions, true))
	{
		for (const FScreenResolutionRHI& R : Resolutions)
		{
			if (R.Width >= 1024 && R.Height >= 576)
			{
				Out.AddUnique(FIntPoint(R.Width, R.Height));
			}
		}
	}
	Out.AddUnique(GetResolution());
	Out.Sort([](const FIntPoint& A, const FIntPoint& B) { return A.X != B.X ? A.X < B.X : A.Y < B.Y; });
	return Out;
}

void USpnSettingsSubsystem::SetResolution(const FIntPoint& Resolution)
{
	if (UGameUserSettings* const S = Gus())
	{
		S->SetScreenResolution(Resolution);
		ApplyAndSave();
	}
}

int32 USpnSettingsSubsystem::GetScalabilityLevel()
{
	UGameUserSettings* const S = Gus();
	return S != nullptr ? S->GetOverallScalabilityLevel() : -1;
}

void USpnSettingsSubsystem::SetScalabilityLevel(int32 Level)
{
	if (UGameUserSettings* const S = Gus())
	{
		S->SetOverallScalabilityLevel(FMath::Clamp(Level, 0, 4));
		ApplyAndSave();
	}
}

bool USpnSettingsSubsystem::IsVSync()
{
	UGameUserSettings* const S = Gus();
	return S != nullptr && S->IsVSyncEnabled();
}

void USpnSettingsSubsystem::SetVSync(bool bEnabled)
{
	if (UGameUserSettings* const S = Gus())
	{
		S->SetVSyncEnabled(bEnabled);
		ApplyAndSave();
	}
}

float USpnSettingsSubsystem::GetFrameRateLimit()
{
	UGameUserSettings* const S = Gus();
	return S != nullptr ? S->GetFrameRateLimit() : 0.f;
}

void USpnSettingsSubsystem::SetFrameRateLimit(float Fps)
{
	if (UGameUserSettings* const S = Gus())
	{
		S->SetFrameRateLimit(FMath::Max(0.f, Fps));
		ApplyAndSave();
	}
}
