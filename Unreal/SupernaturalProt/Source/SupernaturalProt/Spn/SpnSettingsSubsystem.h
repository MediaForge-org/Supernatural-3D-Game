#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SpnSettingsSubsystem.generated.h"

// Player settings (Night Build 01). ENGINEERING PROPOSAL (what is exposed, defaults), not master canon.
//
// Two storage owners, no duplication:
//   * graphics (display mode, resolution, scalability preset, VSync, frame-rate limit) belong to the engine's UGameUserSettings and are only
//     read and written through it (GameUserSettings.ini); this class does not copy them.
//   * everything else is stored in the same ini under [SpnPlayer] and applied here.
// Only settings with an implemented effect exist. AUDIO FOUNDATION: the master volume drives the engine's global volume multiplier; music and effects
// volume drive the volume of the engine's own sound classes /Engine/EngineSounds/Music and /Engine/EngineSounds/SFX (in memory only; nothing is saved to
// the engine content). Every sound that is assigned to one of those classes follows the slider. THE PROJECT HAS NO SOUNDS YET: until content uses the
// classes the two sliders have nothing audible to change, and the settings page says so. Nothing here is gameplay tuning.

USTRUCT()
struct FSpnPlayerSettings
{
	GENERATED_BODY()

	/** 0..1, applied through FApp::SetVolumeMultiplier (all audio). */
	UPROPERTY() float MasterVolume = 1.f;
	/** 0..1, volume of the Music sound class and of the SFX sound class (see the foundation note above). */
	UPROPERTY() float MusicVolume = 1.f;
	UPROPERTY() float EffectsVolume = 1.f;
	/** Scale of the whole game UI (UUserInterfaceSettings::ApplicationScale), 0.75 .. 1.5. */
	UPROPERTY() float UiScale = 1.f;
	/** Multipliers on the look input (1 = the project default). */
	UPROPERTY() float MouseSensitivity = 1.f;
	UPROPERTY() float GamepadSensitivity = 1.f;
	UPROPERTY() bool bInvertY = false;
	UPROPERTY() bool bMinimapVisible = true;
	UPROPERTY() bool bMinimapHeadingUp = false;
	/** Minimap zoom, 0.5 .. 4 (Core limits 0.25 .. 8). */
	UPROPERTY() float MinimapZoom = 1.f;
	/** Vertical camera smoothing while climbing stairs (visual camera only). */
	UPROPERTY() bool bCameraSmoothing = true;
	/** "auto", "en" or "de" (FSpnUiText). */
	UPROPERTY() FString Language = TEXT("auto");
};

UCLASS()
class SUPERNATURALPROT_API USpnSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FSpnPlayerSettings& Get() const { return Settings; }
	/** Validates (clamps), applies (volume, language), stores to the ini and broadcasts. */
	void Set(const FSpnPlayerSettings& NewSettings);
	void Load();
	void SaveToDisk() const;
	/** The volume currently set on the engine sound class "Music" / "SFX" (-1 if the class cannot be loaded). For tests and diagnostics. */
	static float GetMusicClassVolume();
	static float GetEffectsClassVolume();
	/** Applies the settings that have a global effect (volume, language, UI scale). */
	void ApplyGlobal() const;
	static FSpnPlayerSettings Clamped(const FSpnPlayerSettings& In);

	// --- graphics: thin, validated wrappers over UGameUserSettings (apply + save at once) ---
	static EWindowMode::Type GetDisplayMode();
	static void SetDisplayMode(EWindowMode::Type Mode);
	static FIntPoint GetResolution();
	/** Supported fullscreen resolutions, ascending, unique. Never empty (falls back to the current one). */
	static TArray<FIntPoint> GetSupportedResolutions();
	static void SetResolution(const FIntPoint& Resolution);
	/** 0 Low .. 3 Epic, 4 Cinematic, -1 custom (individual scalability groups differ). */
	static int32 GetScalabilityLevel();
	static void SetScalabilityLevel(int32 Level);
	static bool IsVSync();
	static void SetVSync(bool bEnabled);
	/** 0 = unlimited. */
	static float GetFrameRateLimit();
	static void SetFrameRateLimit(float Fps);
	/** Whether a setting that changes the window makes sense in this run (not in a commandlet / dedicated server). */
	static bool CanChangeDisplay();

	DECLARE_MULTICAST_DELEGATE(FSpnSettingsChanged);
	FSpnSettingsChanged OnChanged;

private:
	static void ApplyAndSave();
	FSpnPlayerSettings Settings;
};
