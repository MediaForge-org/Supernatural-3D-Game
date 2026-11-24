#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "Misc/AutomationTest.h"
#include "Spn/SpnSettingsSubsystem.h"
#include "Spn/World/SpnWorldLighting.h"

#if WITH_DEV_AUTOMATION_TESTS

// Render resolution of SpnWorld (decision 2026-11-13, Docs/Development/Visual-Build-Tools.md): the level's ASpnWorldLighting sets the provisional
// standard 90 % at BeginPlay, `Spn.Quality quality` gives 100 %, and leaving the level gives the previous value back - a level without the actor
// (every QA test map) never sees the change. Headless; the rendered check in Lvl_SpnWorld is SpnWorldQA.Integration.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldScreenPercentageTest, "SpnGame.World.ScreenPercentageOnlyWhileTheWorldLevelRuns", Flags)
bool FSpnWorldScreenPercentageTest::RunTest(const FString&)
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (!TestNotNull(TEXT("r.ScreenPercentage exists"), Var)) { return false; }
	const float Original = Var->GetFloat();

	// a recognisable value "of another level"; if a console value is in force (higher priority) the setting cannot be tested here
	Var->Set(77.f, ECVF_SetByCode);
	if (!FMath::IsNearlyEqual(Var->GetFloat(), 77.f))
	{
		AddWarning(TEXT("r.ScreenPercentage was set on the console in this process; the level setting cannot override it - not tested"));
		return true;
	}

	UGameInstance* GameInstance = NewObject<UGameInstance>(GEngine);
	GameInstance->AddToRoot();
	GameInstance->InitializeStandalone();
	UWorld* World = GameInstance->GetWorld();
	World->InitializeActorsForPlay(FURL());   // actors spawned now are initialized, so Destroy() routes EndPlay like leaving a level does
	USpnSettingsSubsystem* Settings = GameInstance->GetSubsystem<USpnSettingsSubsystem>();
	const FSpnPlayerSettings OriginalSettings = Settings->Get();
	if (OriginalSettings.bWorldQualityMode)
	{
		FSpnPlayerSettings Standard = OriginalSettings;
		Standard.bWorldQualityMode = false;     // this test is about the standard; the saved choice is restored below
		Settings->Set(Standard);
	}

	ASpnWorldLighting* Lighting = World->SpawnActor<ASpnWorldLighting>();
	TestTrue(TEXT("the class default is the provisional standard 90"), FMath::IsNearlyEqual(Lighting->DefaultScreenPercentage, 90.f));
	TestTrue(TEXT("the quality mode is 100"), FMath::IsNearlyEqual(Lighting->QualityScreenPercentage, 100.f));
	TestTrue(TEXT("nothing changes before the level plays"), FMath::IsNearlyEqual(Var->GetFloat(), 77.f));
	Lighting->DispatchBeginPlay();
	TestTrue(*FString::Printf(TEXT("the world level runs at 90 %% (is %.0f)"), Var->GetFloat()), FMath::IsNearlyEqual(Var->GetFloat(), 90.f));
	TestFalse(TEXT("standard mode"), Lighting->IsQualityMode());
	TestTrue(TEXT("quality mode: 100 %"), FMath::IsNearlyEqual(Lighting->SetQualityMode(true), 100.f) && Lighting->IsQualityMode());
	TestTrue(TEXT("back to the standard: 90 %"), FMath::IsNearlyEqual(Lighting->SetQualityMode(false), 90.f));
	Lighting->SetQualityMode(true);
	Lighting->Destroy();   // EndPlay
	TestTrue(*FString::Printf(TEXT("leaving the level restores the previous value, also from the quality mode (is %.0f)"), Var->GetFloat()),
		FMath::IsNearlyEqual(Var->GetFloat(), 77.f));

	// a level with the actor but DefaultScreenPercentage 0 leaves the setting alone
	ASpnWorldLighting* Neutral = World->SpawnActor<ASpnWorldLighting>();
	Neutral->DefaultScreenPercentage = 0.f;
	Neutral->DispatchBeginPlay();
	TestTrue(TEXT("0 = the level does not touch the setting"), FMath::IsNearlyEqual(Var->GetFloat(), 77.f));
	Neutral->Destroy();

	if (OriginalSettings.bWorldQualityMode) { Settings->Set(OriginalSettings); }
	GameInstance->Shutdown();
	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	GameInstance->RemoveFromRoot();
	Var->Set(Original, ECVF_SetByCode);
	return true;
}

// The graphics settings choice (FSpnPlayerSettings::bWorldQualityMode) drives the world level: read at BeginPlay, applied live on change; a session
// switch (Spn.Quality / SetQualityMode) is not undone by an unrelated settings change. The test restores the user's settings.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldQualitySettingTest, "SpnGame.World.QualityModeFollowsTheGraphicsSetting", Flags)
bool FSpnWorldQualitySettingTest::RunTest(const FString&)
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (!TestNotNull(TEXT("r.ScreenPercentage exists"), Var)) { return false; }
	const float OriginalPercent = Var->GetFloat();
	Var->Set(77.f, ECVF_SetByCode);
	if (!FMath::IsNearlyEqual(Var->GetFloat(), 77.f))
	{
		AddWarning(TEXT("r.ScreenPercentage was set on the console in this process - not tested"));
		return true;
	}

	UGameInstance* GameInstance = NewObject<UGameInstance>(GEngine);
	GameInstance->AddToRoot();
	GameInstance->InitializeStandalone();
	UWorld* World = GameInstance->GetWorld();
	World->InitializeActorsForPlay(FURL());
	USpnSettingsSubsystem* Settings = GameInstance->GetSubsystem<USpnSettingsSubsystem>();
	const FSpnPlayerSettings Original = Settings->Get();

	FSpnPlayerSettings S = Original;
	S.bWorldQualityMode = true;
	Settings->Set(S);
	ASpnWorldLighting* Lighting = World->SpawnActor<ASpnWorldLighting>();
	Lighting->DispatchBeginPlay();
	TestTrue(*FString::Printf(TEXT("saved choice 'quality' -> the world starts at 100 %% (is %.0f)"), Var->GetFloat()), FMath::IsNearlyEqual(Var->GetFloat(), 100.f) && Lighting->IsQualityMode());

	S.bWorldQualityMode = false;
	Settings->Set(S);
	TestTrue(*FString::Printf(TEXT("switched to 'standard' in the menu -> 90 %% at once (is %.0f)"), Var->GetFloat()), FMath::IsNearlyEqual(Var->GetFloat(), 90.f) && !Lighting->IsQualityMode());

	Lighting->SetQualityMode(true);     // session switch, as Spn.Quality quality
	S.MinimapZoom = Original.MinimapZoom > 1.5f ? 1.f : 2.f;
	Settings->Set(S);
	TestTrue(TEXT("an unrelated settings change keeps the session choice (100 %)"), FMath::IsNearlyEqual(Var->GetFloat(), 100.f));
	TestFalse(TEXT("the session switch did not change the saved choice"), Settings->Get().bWorldQualityMode);

	Lighting->Destroy();
	TestTrue(*FString::Printf(TEXT("leaving the level restores the previous value (is %.0f)"), Var->GetFloat()), FMath::IsNearlyEqual(Var->GetFloat(), 77.f));
	S.bWorldQualityMode = true;
	Settings->Set(S);
	TestTrue(TEXT("without the world level the setting changes nothing"), FMath::IsNearlyEqual(Var->GetFloat(), 77.f));

	Settings->Set(Original);           // the user's own settings back
	GameInstance->Shutdown();
	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	GameInstance->RemoveFromRoot();
	Var->Set(OriginalPercent, ECVF_SetByCode);
	return true;
}

#endif
