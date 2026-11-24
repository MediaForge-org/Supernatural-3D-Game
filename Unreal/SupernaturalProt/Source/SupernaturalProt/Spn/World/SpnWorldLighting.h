#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpnWorldLighting.generated.h"

class UMaterialParameterCollection;

/**
 * One reproducible lighting situation of a level (visual build): sun, sky light, fog, exposure, colour grading, lamps and weather look.
 * PLACEHOLDER presentation data authored by the world generator script; not a weather or time-of-day simulation and not canon.
 */
USTRUCT(BlueprintType)
struct FSpnLightingScenario
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting")
	FName Name = NAME_None;

	// --- sun (directional light tagged SpnSun)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sun")
	float SunPitch = -45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sun")
	float SunYaw = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sun")
	float SunLux = 90000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sun")
	FLinearColor SunColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sun")
	float SunSourceAngle = 0.5357f;

	// --- moon (optional second directional light tagged SpnMoon); 0 lux switches it off
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Moon")
	float MoonLux = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Moon")
	FLinearColor MoonColor = FLinearColor(0.62f, 0.74f, 1.f);

	// --- sky light
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sky")
	float SkyLightIntensity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Sky")
	FLinearColor SkyLightColor = FLinearColor::White;

	// --- exponential height fog
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	float FogDensity = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	float FogHeightFalloff = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	FLinearColor FogInscatteringColor = FLinearColor(0.f, 0.f, 0.f);   // black = the sky atmosphere drives the fog colour

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	FLinearColor VolumetricFogAlbedo = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	float VolumetricFogExtinctionScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	float VolumetricFogScatteringDistribution = 0.2f;

	/** Local fog patches (actors tagged SpnMist with a local fog volume): extinction multiplier, 0 = hidden. Each patch keeps its own shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	float MistDensity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Fog")
	FLinearColor MistAlbedo = FLinearColor(0.85f, 0.88f, 0.9f);

	// --- clouds: scalar parameters of the volumetric cloud material (applied to a dynamic instance), and visibility
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Clouds")
	bool bCloudsVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Clouds")
	TMap<FName, float> CloudScalars;

	// --- exposure and colour grading (unbound post process volume tagged SpnPost)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float ExposureCompensation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float ExposureMinEV = -10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float ExposureMaxEV = 20.f;

	/** Exposure range inside rooms: applied to the bounded post process volumes tagged SpnPostInterior (they only override the exposure range). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float InteriorExposureMinEV = -10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float InteriorExposureMaxEV = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float WhiteTemp = 6500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float Saturation = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float Contrast = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	FLinearColor ColorGain = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float BloomIntensity = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|Post")
	float VignetteIntensity = 0.3f;

	// --- world: lamps (light components of actors tagged SpnLamp) and material collection scalars (LampGlow, WindowGlow, Wetness, ...)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|World")
	bool bLampsOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|World")
	TMap<FName, float> CollectionScalars;

	/** Console commands run when the scenario is applied (render settings that belong to the look). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting|World")
	TArray<FString> ConsoleCommands;
};

/**
 * Applies named lighting scenarios to the lights, fog and post process of its level. One per level; found through the tags below.
 * Console: `Spn.Lighting <name>` (no name: lists them), `Spn.Lighting next`.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnWorldLighting : public AActor
{
	GENERATED_BODY()

public:
	ASpnWorldLighting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting")
	TArray<FSpnLightingScenario> Scenarios;

	/** Applied at BeginPlay. None = the first scenario. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting")
	FName InitialScenario = NAME_None;

	/** Collection whose scalars the scenarios set (wetness, lamp glow, window glow). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Lighting")
	TObjectPtr<UMaterialParameterCollection> Collection;

	UFUNCTION(BlueprintCallable, Category = "Spn|Lighting")
	bool ApplyScenario(FName Name);

	UFUNCTION(BlueprintCallable, Category = "Spn|Lighting")
	FName GetCurrentScenario() const { return Current; }

	TArray<FName> GetScenarioNames() const;

	/**
	 * Render resolution of this level with TSR (r.ScreenPercentage), set at BeginPlay and restored at EndPlay, so only the level that holds this
	 * actor (Lvl_SpnWorld) is affected, never the QA test maps. PROVISIONAL standard 90 % (developer decision 2026-11-13); 0 = leave the setting alone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Quality", meta = (ClampMin = "0", ClampMax = "200"))
	float DefaultScreenPercentage = 90.f;

	/** Quality mode: full resolution; must stay available at any time. Chosen in the graphics settings (saved, FSpnPlayerSettings::bWorldQualityMode)
	 *  or for the session with `Spn.Quality quality` (not saved). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Quality", meta = (ClampMin = "0", ClampMax = "200"))
	float QualityScreenPercentage = 100.f;

	/** Switches between the standard (false) and the quality mode (true). Returns the screen percentage now in effect. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Quality")
	float SetQualityMode(bool bQuality);

	UFUNCTION(BlueprintCallable, Category = "Spn|Quality")
	bool IsQualityMode() const { return bQualityMode; }

	/** r.ScreenPercentage as the renderer sees it right now. */
	static float GetScreenPercentage();

	static ASpnWorldLighting* Find(const UWorld* World);

	static const FName SunTag;
	static const FName MoonTag;
	static const FName PostTag;
	static const FName InteriorPostTag;
	static const FName LampTag;
	static const FName MistTag;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void ApplyScreenPercentage(float Percent);
	void HandleSettingsChanged();
	FDelegateHandle SettingsChangedHandle;
	bool bAppliedSetting = false;      // the settings value last applied (Spn.Quality is a session switch and does not change it)

	FName Current = NAME_None;
	bool bQualityMode = false;
	bool bScreenPercentageSet = false;     // this actor changed r.ScreenPercentage and restores it at EndPlay
	float PreviousScreenPercentage = 100.f;
};
