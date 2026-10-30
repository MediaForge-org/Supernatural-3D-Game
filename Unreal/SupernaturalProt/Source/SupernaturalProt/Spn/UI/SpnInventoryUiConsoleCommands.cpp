#include "SupernaturalProt.h"
#include "Spn/UI/SpnInventoryScreen.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnUiText.h"

#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"

// DEVELOPER commands of the inventory screen (M2B3.5). The normal way to open it is the configured key (I / gamepad Create); these exist for QA
// and for sessions where the keyboard is not available.

#if !UE_BUILD_SHIPPING

namespace
{
	USpnInventoryUiSubsystem* FindUi(const UWorld* World, const TCHAR* Command)
	{
		const UGameInstance* GameInstance = World != nullptr ? World->GetGameInstance() : nullptr;
		const ULocalPlayer* Player = GameInstance != nullptr ? GameInstance->GetFirstGamePlayer() : nullptr;
		USpnInventoryUiSubsystem* Ui = Player != nullptr ? Player->GetSubsystem<USpnInventoryUiSubsystem>() : nullptr;
		if (Ui == nullptr)
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s FAILED: no local player; run it while playing (PIE or game)"), Command);
		}
		return Ui;
	}

	void UiInventory(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventoryUiSubsystem* Ui = FindUi(World, TEXT("Spn.UI.Inventory"));
		if (Ui == nullptr)
		{
			return;
		}
		const FString Mode = Args.IsValidIndex(0) ? Args[0].ToLower() : FString(TEXT("toggle"));
		if (Mode == TEXT("open")) { Ui->OpenInventory(); }
		else if (Mode == TEXT("close")) { Ui->CloseInventory(); }
		else { Ui->ToggleInventory(); }
		UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.UI.Inventory: %s"), Ui->IsOpen() ? TEXT("open") : TEXT("closed"));
	}

	void UiLanguage(const TArray<FString>& Args)
	{
		const FString Language = Args.IsValidIndex(0) ? Args[0].ToLower() : FString();
		if (Language == TEXT("auto") || Language.IsEmpty())
		{
			FSpnUiText::SetLanguageOverride(FString());
		}
		else
		{
			FSpnUiText::SetLanguageOverride(Language);
		}
		UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.UI.Language: active language is now '%s' (override '%s')"), *FSpnUiText::GetActiveLanguage(), *FSpnUiText::GetLanguageOverride());
	}

	FAutoConsoleCommandWithWorldAndArgs UiInventoryCommand(TEXT("Spn.UI.Inventory"), TEXT("DEV: open | close | toggle the graphical inventory screen"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&UiInventory));
	FAutoConsoleCommand UiLanguageCommand(TEXT("Spn.UI.Language"), TEXT("DEV: en | de | auto - language of the inventory screen texts"), FConsoleCommandWithArgsDelegate::CreateStatic(&UiLanguage));
}

#endif
