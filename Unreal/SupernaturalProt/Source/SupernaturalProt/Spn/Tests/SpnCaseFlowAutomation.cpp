#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Spn/Case/SpnCaseCreature.h"
#include "Spn/Case/SpnCaseSubsystem.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/UI/SpnMenuScreen.h"
#include "Spn/UI/SpnUiSubsystem.h"
#include "SpnNpcReaction.h"
#include "SpnSaveSubsystem.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

#if WITH_DEV_AUTOMATION_TESTS

// The case in the real game flow (NightBuild07): main menu -> "New game" into Lvl_SpnCaseMill -> the arrival autosave -> death -> the death screen
// -> "Load the last checkpoint" -> alive at the autosave with the difficulty's health; and the pause menu refuses saving while the creature hunts.
// Run in a -game process that starts on the main menu:
//   UnrealEditor.exe <uproject> /Game/SpnTest/Lvl_SpnMainMenu -game -SpnGameplayMap=/Game/SpnTest/Lvl_SpnCaseMill
//       -ExecCmds="Automation RunTests SpnCaseQA.Flow" -TestExit="Automation Test Queue Empty"
// Deliberately NOT named SpnQA.* (the gameplay QA runner runs those on its own maps). Every step after a failed precondition does nothing.
// Saves go to a temporary directory that is removed at the end. A rendered run also writes a screenshot of the death screen.

namespace SpnCaseFlowTest
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FState
	{
		bool bReady = false; // the flow reached the case level; later steps do nothing without it
		FString SaveDir;
		FVector Start = FVector::ZeroVector;
	};

	APlayerController* PC()
	{
		UWorld* World = SpnMovementTest::FindPlayWorld();
		return World != nullptr ? World->GetFirstPlayerController() : nullptr;
	}

	USpnUiSubsystem* Ui()
	{
		APlayerController* Controller = PC();
		return Controller != nullptr && Controller->GetLocalPlayer() != nullptr ? Controller->GetLocalPlayer()->GetSubsystem<USpnUiSubsystem>() : nullptr;
	}

	USpnCaseSubsystem* Case()
	{
		UWorld* World = SpnMovementTest::FindPlayWorld();
		return World != nullptr ? World->GetSubsystem<USpnCaseSubsystem>() : nullptr;
	}

	UGameInstance* Instance()
	{
		UWorld* World = SpnMovementTest::FindPlayWorld();
		return World != nullptr ? World->GetGameInstance() : nullptr;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseFlowTest, "SpnCaseQA.Flow.DeathScreenAndRespawnFromTheAutosave", SpnCaseFlowTest::Flags)
bool FSpnCaseFlowTest::RunTest(const FString&)
{
	using namespace SpnCaseFlowTest;
	TSharedRef<FState> S = MakeShared<FState>();
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.f)); // the main menu
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]
	{
		UGameInstance* GI = Instance();
		USpnSessionSubsystem* Session = GI != nullptr ? GI->GetSubsystem<USpnSessionSubsystem>() : nullptr;
		if (Session == nullptr || !Session->IsMenuWorld())
		{
			AddError(TEXT("start this test on the main menu with -SpnGameplayMap=/Game/SpnTest/Lvl_SpnCaseMill"));
			return true;
		}
		S->SaveDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
		GI->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(S->SaveDir);
		Session->StartNewGame();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(6.f)); // the level, the navigation mesh, the arrival autosave
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]
	{
		USpnCaseSubsystem* C = Case();
		ASupernaturalProtCharacter* Player = SpnMovementTest::FindCharacter();
		UGameInstance* GI = Instance();
		if (C == nullptr || Player == nullptr || GI == nullptr)
		{
			AddError(TEXT("no case level / player after New game"));
			return true;
		}
		S->bReady = true;
		S->Start = Player->GetActorLocation();
		TestEqual(TEXT("the case level runs"), FPaths::GetBaseFilename(Player->GetWorld()->GetMapName()).Contains(TEXT("Lvl_SpnCaseMill")), true);
		TestTrue(*FString::Printf(TEXT("the arrival autosave was written (%d)"), C->GetAutosaveCount()), C->GetAutosaveCount() >= 1);
		TestTrue(TEXT("the autosave slot exists"), GI->GetSubsystem<USpnSaveSubsystem>()->DoesSlotExist(USpnCaseSubsystem::AutosaveSlot()));
		Player->SetActorLocation(S->Start + FVector(0.f, 1200.f, 0.f)); // walked on a bit
		C->DamagePlayer(10000.f, TEXT("creature"));
		TestTrue(TEXT("the player is dead"), C->IsPlayerDead());
		TestTrue(TEXT("the death screen is up"), Ui() != nullptr && Ui()->GetTopPage() == ESpnPage::Death);
		USpnMenuScreen* Menu = Ui() != nullptr ? Ui()->GetMenuScreen(ESpnPage::Death) : nullptr;
		TestTrue(TEXT("it offers the last checkpoint first"), Menu != nullptr && Menu->GetSelectedId() == FName(TEXT("reload")));
		TestTrue(TEXT("and the main menu"), Menu != nullptr && Menu->FindIndex(TEXT("mainmenu")) != INDEX_NONE);
		if (Ui() != nullptr) { Ui()->Back(); }
		TestTrue(TEXT("Back does not close the death screen"), Ui() != nullptr && Ui()->GetTopPage() == ESpnPage::Death);
		FScreenshotRequest::RequestScreenshot(FPaths::ProjectSavedDir() / TEXT("SpnCase") / TEXT("shots") / TEXT("CaseMill_05_death_screen.png"), true, false);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]
	{
		if (!S->bReady) { return true; }
		USpnMenuScreen* Menu = Ui() != nullptr ? Ui()->GetMenuScreen(ESpnPage::Death) : nullptr;
		if (Menu == nullptr) { AddError(TEXT("no death screen")); return true; }
		Menu->Select(Menu->FindIndex(TEXT("reload")));
		Menu->Activate(false);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]
	{
		if (!S->bReady) { return true; }
		USpnCaseSubsystem* C = Case();
		ASupernaturalProtCharacter* Player = SpnMovementTest::FindCharacter();
		if (C == nullptr || Player == nullptr) { AddError(TEXT("no case after the reload")); S->bReady = false; return true; }
		TestFalse(TEXT("alive again"), C->IsPlayerDead());
		TestTrue(TEXT("the death screen is gone"), Ui() != nullptr && Ui()->GetTopPage() == ESpnPage::None);
		TestTrue(*FString::Printf(TEXT("back at the autosave (%.0f cm from it)"), FVector::Dist2D(Player->GetActorLocation(), S->Start)), FVector::Dist2D(Player->GetActorLocation(), S->Start) < 150.f);
		const float Expected = C->GetMaxHealth() * C->GetDifficulty().RespawnHealth;
		TestTrue(*FString::Printf(TEXT("health %.0f = the difficulty's respawn health %.0f"), C->GetHealth(), Expected), FMath::IsNearlyEqual(C->GetHealth(), Expected, 0.5f));

		// make the creature hunt: a loud noise right next to it
		TArray<ASpnCreature*> Creatures = C->GetCreatures();
		if (Creatures.Num() == 0) { AddError(TEXT("no creature")); S->bReady = false; return true; }
		FSpnNoise N;
		N.LocationCm = Creatures[0]->GetActorLocation() + FVector(300.f, 0.f, 0.f);
		N.RangeCm = 3000.f;
		N.Strength = 1.f;
		N.Kind = ESpnNoiseKind::Impact;
		N.SourceId = TEXT("test");
		Instance()->GetSubsystem<USpnNoiseSubsystem>()->EmitNoise(N);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]
	{
		if (!S->bReady) { return true; }
		USpnCaseSubsystem* C = Case();
		if (C == nullptr || Ui() == nullptr || C->GetCreatures().Num() == 0) { AddError(TEXT("no case / UI / creature")); return true; }
		TestTrue(*FString::Printf(TEXT("the creature hunts (%s), saving is blocked"), LexToString(C->GetCreatures()[0]->GetCreatureState())), C->IsSaveBlocked());
		TestTrue(TEXT("the pause menu opens"), Ui()->OpenPause());
		USpnMenuScreen* Pause = Ui()->GetMenuScreen(ESpnPage::Pause);
		const int32 SaveIndex = Pause != nullptr ? Pause->FindIndex(TEXT("save")) : INDEX_NONE;
		TestTrue(TEXT("its Save entry is disabled while hunted"), SaveIndex != INDEX_NONE && !Pause->IsItemEnabled(SaveIndex));
		FScreenshotRequest::RequestScreenshot(FPaths::ProjectSavedDir() / TEXT("SpnCase") / TEXT("shots") / TEXT("CaseMill_06_pause_save_blocked.png"), true, false);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([S]
	{
		if (USpnUiSubsystem* U = Ui()) { U->CloseAll(); }
		if (UGameInstance* GI = Instance())
		{
			GI->GetSubsystem<USpnSaveSubsystem>()->ResetSaveDirectory();
		}
		if (!S->SaveDir.IsEmpty()) { IFileManager::Get().DeleteDirectory(*S->SaveDir, false, true); }
		return true;
	}));
	return true;
}

#endif
