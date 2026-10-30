#include "Spn/UI/SpnUiSubsystem.h"

#include "Engine/Console.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "HAL/FileManager.h"
#include "Misc/CommandLine.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnMapSubsystem.h"
#include "SpnSaveSubsystem.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/UI/SpnHudWidget.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnMapScreen.h"
#include "Spn/UI/SpnUiText.h"
#include "SupernaturalProt.h"

namespace
{
	const double FpsOptions[] = {0.0, 30.0, 60.0, 90.0, 120.0, 144.0, 165.0, 240.0};
	const float ZoomOptions[] = {0.5f, 0.75f, 1.f, 1.5f, 2.f, 3.f, 4.f};

	FText T(const TCHAR* Key) { return FSpnUiText::Get(Key); }
	FText T(const TCHAR* Key, const TArray<FText>& Args) { return FSpnUiText::Get(Key, Args); }

	template <typename TOption, int32 N>
	int32 NearestIndex(const TOption (&Options)[N], double Value)
	{
		int32 Best = 0;
		for (int32 I = 1; I < N; ++I)
		{
			if (FMath::Abs(static_cast<double>(Options[I]) - Value) < FMath::Abs(static_cast<double>(Options[Best]) - Value)) { Best = I; }
		}
		return Best;
	}

	FText Percent(float Value) { return FText::FromString(FString::Printf(TEXT("%d %%"), FMath::RoundToInt(Value * 100.f))); }
	FText Multiplier(float Value) { return FText::FromString(FString::Printf(TEXT("×%.1f"), Value)); }
}

/** Sees every key before the widgets and the game; routes it to the router (which decides whether it is for a modal screen). */
class FSpnUiInputProcessor : public IInputProcessor
{
public:
	explicit FSpnUiInputProcessor(USpnUiSubsystem* InOwner) : Owner(InOwner) {}
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& Event) override { return Owner.IsValid() && Owner->ProcessKeyDown(Event); }
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& Event) override { return Owner.IsValid() && Owner->ProcessAnalog(Event); }
	virtual const TCHAR* GetDebugName() const override { return TEXT("SpnUiRouter"); }

private:
	TWeakObjectPtr<USpnUiSubsystem> Owner;
};

// --- free helpers ---------------------------------------------------------------------------------------------------------------------

namespace SpnUi
{
	static USpnUiSubsystem* FindUi(const UObject* Context)
	{
		const UWorld* World = Context != nullptr ? Context->GetWorld() : nullptr;
		const ULocalPlayer* Player = World != nullptr ? World->GetFirstLocalPlayerFromController() : nullptr;
		return Player != nullptr ? Player->GetSubsystem<USpnUiSubsystem>() : nullptr;
	}

	void Notify(const UObject* WorldContext, const FText& Message, ENote Kind)
	{
		UE_LOG(LogSupernaturalProt, Display, TEXT("HUD note [%d]: %s"), static_cast<int32>(Kind), *Message.ToString());
		if (USpnUiSubsystem* Ui = FindUi(WorldContext))
		{
			Ui->Notify(Message, Kind);
		}
	}

	bool SetPrompt(const UObject* WorldContext, const FString& Prompt)
	{
		USpnUiSubsystem* Ui = FindUi(WorldContext);
		if (Ui == nullptr || Ui->GetHud() == nullptr)
		{
			return false;
		}
		Ui->SetInteractionPrompt(Prompt);
		return true;
	}
}

// --- lifetime -------------------------------------------------------------------------------------------------------------------------

void USpnUiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (FSlateApplication::IsInitialized())
	{
		Processor = MakeShared<FSpnUiInputProcessor>(this);
		FSlateApplication::Get().RegisterInputPreProcessor(Processor, 0);
	}
	if (FParse::Param(FCommandLine::Get(), TEXT("SpnQaKeys")))
	{
		StartQaPoseFeed();
	}
	if (UGameInstance* Instance = GetInstance())
	{
		if (USpnSessionSubsystem* Session = Instance->GetSubsystem<USpnSessionSubsystem>())
		{
			WorldReadyHandle = Session->OnWorldReady.AddUObject(this, &USpnUiSubsystem::HandleWorldReady);
			PendingLoadHandle = Session->OnPendingLoadFinished.AddUObject(this, &USpnUiSubsystem::HandlePendingLoad);
		}
	}
	if (ULocalPlayer* Player = GetLocalPlayer())
	{
		if (USpnInventoryUiSubsystem* Inventory = Player->GetSubsystem<USpnInventoryUiSubsystem>())
		{
			Inventory->OpenBlocker = [this]() { return IsModalOpen(); };
		}
	}
}

void USpnUiSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(QaPoseFeed);
	if (Processor.IsValid() && FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(Processor);
	}
	Processor.Reset();
	if (UGameInstance* Instance = GetInstance())
	{
		if (USpnSessionSubsystem* Session = Instance->GetSubsystem<USpnSessionSubsystem>())
		{
			Session->OnWorldReady.Remove(WorldReadyHandle);
			Session->OnPendingLoadFinished.Remove(PendingLoadHandle);
		}
	}
	if (ULocalPlayer* Player = GetLocalPlayer())
	{
		if (USpnInventoryUiSubsystem* Inventory = Player->GetSubsystem<USpnInventoryUiSubsystem>())
		{
			Inventory->OpenBlocker = nullptr;
		}
	}
	Super::Deinitialize();
}

UGameInstance* USpnUiSubsystem::GetInstance() const
{
	return GetLocalPlayer() != nullptr ? GetLocalPlayer()->GetGameInstance() : nullptr;
}

USpnSessionSubsystem* USpnUiSubsystem::GetSession() const
{
	UGameInstance* Instance = GetInstance();
	return Instance != nullptr ? Instance->GetSubsystem<USpnSessionSubsystem>() : nullptr;
}

APlayerController* USpnUiSubsystem::GetController() const
{
	const ULocalPlayer* Player = GetLocalPlayer();
	return Player != nullptr ? Player->GetPlayerController(Player->GetWorld()) : nullptr;
}

USpnMenuScreen* USpnUiSubsystem::GetMenuScreen(ESpnPage Page) const
{
	const TObjectPtr<USpnMenuScreen>* Found = Menus.Find(Page);
	return Found != nullptr ? Found->Get() : nullptr;
}

void USpnUiSubsystem::HandleWorldReady(bool bMenu)
{
	// the viewport of the old world is gone with it: drop every widget, they are rebuilt for the new controller on demand
	Menus.Empty();
	MapScreen = nullptr;
	Hud = nullptr;
	Stack.Reset();
	bPausedByUi = false;
	bInputModeApplied = false;
	if (bMenu)
	{
		ShowMainMenu();
	}
	else
	{
		EnsureHud();
		ApplyGameplayInput(0);
	}
}

void USpnUiSubsystem::ApplyGameplayInput(int32 Attempt)
{
	// A level change keeps the viewport and its mouse-capture mode: the menu level left it on "capture only while a button is held", so the mouse
	// look of the gameplay level would only turn the camera with a button down. Every gameplay world therefore starts in game-only input with a
	// hidden cursor and the look / move input enabled, whatever the previous level did.
	APlayerController* Controller = GetController();
	if (Controller == nullptr || Controller->GetPawn() == nullptr)
	{
		if (Attempt < 50 && GetLocalPlayer() != nullptr)
		{
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this, Attempt](float) { ApplyGameplayInput(Attempt + 1); return false; }), 0.1f);
		}
		return;
	}
	if (Stack.Num() > 0 || bPausedByUi)
	{
		return; // a page is open: it owns the input mode and restores it when it closes
	}
	Controller->SetIgnoreMoveInput(false);
	Controller->SetIgnoreLookInput(false);
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->FlushPressedKeys();
}

void USpnUiSubsystem::HandlePendingLoad(const FSpnSaveStatus& Status)
{
	// the HUD does not exist yet (the world is still being set up): the note is shown as soon as it does
	PendingNoteText = Status.IsOk() ? T(TEXT("ui.save.loaded")) : DescribeSaveResult(Status);
	PendingNoteKind = Status.IsOk() ? SpnUi::ENote::Good : SpnUi::ENote::Error;
	bHasPendingNote = true;
}

// --- HUD ------------------------------------------------------------------------------------------------------------------------------

void USpnUiSubsystem::EnsureHud()
{
	APlayerController* Controller = GetController();
	USpnSessionSubsystem* Session = GetSession();
	if (Controller == nullptr || Session == nullptr || !Session->IsGameFlowEnabled() || Session->IsMenuWorld())
	{
		return;
	}
	if (Hud == nullptr || Hud->GetOwningPlayer() != Controller)
	{
		Hud = CreateWidget<USpnHudWidget>(Controller, USpnHudWidget::StaticClass());
	}
	if (Hud != nullptr && !Hud->IsInViewport())
	{
		Hud->AddToViewport(10);
		Hud->SetUsingGamepad(bUsingGamepad);
		Hud->SetSuppressed(IsModalOpen());
	}
	if (Hud != nullptr && bHasPendingNote)
	{
		bHasPendingNote = false;
		Hud->PushNotification(PendingNoteText, PendingNoteKind);
	}
}

void USpnUiSubsystem::Notify(const FText& Text, SpnUi::ENote Kind)
{
	EnsureHud();
	if (Hud != nullptr)
	{
		Hud->PushNotification(Text, Kind);
	}
}

void USpnUiSubsystem::SetInteractionPrompt(const FString& Text)
{
	if (Hud != nullptr)
	{
		Hud->SetPrompt(IsModalOpen() ? FString() : Text);
	}
}

// --- input ----------------------------------------------------------------------------------------------------------------------------

bool USpnUiSubsystem::IsConsoleActive() const
{
	const UWorld* World = GetLocalPlayer() != nullptr ? GetLocalPlayer()->GetWorld() : nullptr;
	const UGameViewportClient* Viewport = World != nullptr ? World->GetGameViewport() : nullptr;
	return Viewport != nullptr && Viewport->ViewportConsole != nullptr && Viewport->ViewportConsole->ConsoleActive();
}

bool USpnUiSubsystem::IsTextEntryFocused() const
{
	if (!FSlateApplication::IsInitialized())
	{
		return false;
	}
	const TSharedPtr<SWidget> Focused = FSlateApplication::Get().GetKeyboardFocusedWidget();
	if (!Focused.IsValid())
	{
		return false;
	}
	const FString Type = Focused->GetType().ToString();
	return Type.Contains(TEXT("EditableText")) || Type.Contains(TEXT("InputBox"));
}

bool USpnUiSubsystem::ProcessKeyDown(const FKeyEvent& Event)
{
	if (!bRoutingEnabled)
	{
		return false;
	}
	USpnSessionSubsystem* Session = GetSession();
	if (Session == nullptr || !Session->IsGameFlowEnabled())
	{
		return false;
	}
	const FKey Key = Event.GetKey();
	// 1. the console and text fields own the keyboard; the console key itself is never touched
	if (IsConsoleActive() || IsTextEntryFocused() || GetDefault<UInputSettings>()->ConsoleKeys.Contains(Key))
	{
		return false;
	}
	if (!Event.IsRepeat() && HandleQaKey(Key))
	{
		return true;
	}
	const bool bPad = Key.IsGamepadKey();
	if (bPad != bUsingGamepad)
	{
		bUsingGamepad = bPad;
		if (Hud != nullptr) { Hud->SetUsingGamepad(bPad); }
		if (MapScreen != nullptr) { MapScreen->SetUsingGamepad(bPad); }
	}
	// 2. the inventory has its own router while it is open
	if (const ULocalPlayer* Player = GetLocalPlayer())
	{
		if (const USpnInventoryUiSubsystem* Inventory = Player->GetSubsystem<USpnInventoryUiSubsystem>())
		{
			if (Inventory->IsOpen())
			{
				return false;
			}
		}
	}
	// 3. a modal page of this router: the top one always consumes (modal)
	if (Stack.Num() > 0)
	{
		const ESpnPage Top = Stack.Last();
		if (Top == ESpnPage::Map)
		{
			if (MapScreen != nullptr) { MapScreen->HandleKey(Key, Event.IsRepeat(), Event.IsShiftDown()); }
			return true;
		}
		if (Top == ESpnPage::Pause && Key == EKeys::Gamepad_Special_Right && !Event.IsRepeat())
		{
			Back(); // Options toggles the pause menu
			return true;
		}
		if (USpnMenuScreen* Menu = GetMenuScreen(Top))
		{
			Menu->HandleKey(Key, Event.IsRepeat());
		}
		return true;
	}
	// 4. gameplay
	if (Session->IsMenuWorld() || Event.IsRepeat())
	{
		return false;
	}
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		return OpenPause();
	}
	if (Key == EKeys::M || Key == EKeys::Gamepad_FaceButton_Top)
	{
		return OpenMap();
	}
	return false;
}

bool USpnUiSubsystem::ProcessAnalog(const FAnalogInputEvent& Event)
{
	if (!bRoutingEnabled || Stack.Num() == 0)
	{
		return false;
	}
	const FKey Key = Event.GetKey();
	if (FMath::Abs(Event.GetAnalogValue()) > 0.3f && Key.IsGamepadKey() && !bUsingGamepad)
	{
		bUsingGamepad = true;
		if (Hud != nullptr) { Hud->SetUsingGamepad(true); }
		if (MapScreen != nullptr) { MapScreen->SetUsingGamepad(true); }
	}
	const ESpnPage Top = Stack.Last();
	if (Top == ESpnPage::Map)
	{
		if (MapScreen != nullptr) { MapScreen->HandleAnalog(Key, Event.GetAnalogValue()); }
	}
	else if (USpnMenuScreen* Menu = GetMenuScreen(Top))
	{
		Menu->HandleAnalog(Key, Event.GetAnalogValue());
	}
	return false;
}

// --- input mode and pause ---------------------------------------------------------------------------------------------------------------

void USpnUiSubsystem::ApplyInputMode()
{
	APlayerController* Controller = GetController();
	if (Controller == nullptr || bInputModeApplied)
	{
		return;
	}
	bInputModeApplied = true;
	bHadShowMouseCursor = Controller->bShowMouseCursor;
	Controller->FlushPressedKeys(); // a held movement key must not keep running behind the screen
	Controller->SetShowMouseCursor(true);
	FInputModeGameAndUI Mode; // the pre-processor routes keys; the mouse is free for clicking
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Mode.SetHideCursorDuringCapture(false);
	Controller->SetInputMode(Mode);
	Controller->SetIgnoreMoveInput(true);
	Controller->SetIgnoreLookInput(true);
}

void USpnUiSubsystem::RestoreGameInput()
{
	APlayerController* Controller = GetController();
	if (Controller != nullptr && bInputModeApplied)
	{
		Controller->SetIgnoreMoveInput(false);
		Controller->SetIgnoreLookInput(false);
		Controller->SetShowMouseCursor(bHadShowMouseCursor);
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->FlushPressedKeys();
	}
	bInputModeApplied = false;
}

void USpnUiSubsystem::SetPaused(bool bPaused)
{
	USpnSessionSubsystem* Session = GetSession();
	if (Session == nullptr || Session->IsMenuWorld() || bPausedByUi == bPaused)
	{
		return;
	}
	if (UWorld* World = GetLocalPlayer() != nullptr ? GetLocalPlayer()->GetWorld() : nullptr)
	{
		UGameplayStatics::SetGamePaused(World, bPaused);
		bPausedByUi = bPaused;
	}
}

// --- page stack -------------------------------------------------------------------------------------------------------------------------

USpnMenuScreen* USpnUiSubsystem::GetOrCreateMenu(ESpnPage Page)
{
	if (USpnMenuScreen* Existing = GetMenuScreen(Page))
	{
		return Existing;
	}
	APlayerController* Controller = GetController();
	if (Controller == nullptr)
	{
		return nullptr;
	}
	USpnMenuScreen* Menu = CreateWidget<USpnMenuScreen>(Controller, USpnMenuScreen::StaticClass());
	Menu->SetStyle(Page == ESpnPage::MainMenu ? ESpnMenuStyle::MainMenu : ESpnMenuStyle::Panel);
	Menu->SetBackdropOpacity(Page == ESpnPage::MainMenu ? 0.38f : 0.74f);
	Menu->OnActivate.BindLambda([this, Page](FName Id, bool bSecondary) { OnActivate(Page, Id, bSecondary); });
	Menu->OnAdjust.BindLambda([this, Page](FName Id, int32 Delta) { OnAdjust(Page, Id, Delta); });
	Menu->OnBack.BindLambda([this]() { Back(); });
	Menus.Add(Page, Menu);
	return Menu;
}

void USpnUiSubsystem::RefreshPage(ESpnPage Page)
{
	switch (Page)
	{
	case ESpnPage::MainMenu: FillMainMenu(); break;
	case ESpnPage::Pause: FillPause(); break;
	case ESpnPage::Settings: FillSettings(); break;
	case ESpnPage::Slots: FillSlots(); break;
	case ESpnPage::Confirm: FillConfirm(); break;
	default: break;
	}
}

void USpnUiSubsystem::ShowTop()
{
	if (Stack.IsEmpty())
	{
		return;
	}
	const ESpnPage Top = Stack.Last();
	UUserWidget* Widget = Top == ESpnPage::Map ? static_cast<UUserWidget*>(MapScreen) : static_cast<UUserWidget*>(GetMenuScreen(Top));
	if (Widget != nullptr && !Widget->IsInViewport())
	{
		Widget->AddToViewport(200 + Stack.Num() * 10);
	}
}

void USpnUiSubsystem::Push(ESpnPage Page)
{
	// the page below disappears while the new one is up
	if (!Stack.IsEmpty())
	{
		const ESpnPage Below = Stack.Last();
		UUserWidget* Widget = Below == ESpnPage::Map ? static_cast<UUserWidget*>(MapScreen) : static_cast<UUserWidget*>(GetMenuScreen(Below));
		if (Widget != nullptr)
		{
			Widget->RemoveFromParent();
		}
	}
	Stack.Add(Page);
	ApplyInputMode();
	if (Page != ESpnPage::MainMenu)
	{
		SetPaused(true);
	}
	if (Hud != nullptr)
	{
		Hud->SetSuppressed(true);
	}
	if (Page != ESpnPage::Map)
	{
		RefreshPage(Page);
	}
	ShowTop();
}

void USpnUiSubsystem::Pop()
{
	if (Stack.IsEmpty())
	{
		return;
	}
	const ESpnPage Top = Stack.Pop();
	UUserWidget* Widget = Top == ESpnPage::Map ? static_cast<UUserWidget*>(MapScreen) : static_cast<UUserWidget*>(GetMenuScreen(Top));
	if (Widget != nullptr)
	{
		Widget->RemoveFromParent();
	}
	if (Stack.IsEmpty())
	{
		RestoreGameInput();
		SetPaused(false);
		if (Hud != nullptr) { Hud->SetSuppressed(false); }
		return;
	}
	if (Stack.Last() != ESpnPage::Map)
	{
		RefreshPage(Stack.Last());
	}
	ShowTop();
}

void USpnUiSubsystem::Back()
{
	if (Stack.IsEmpty() || Stack.Last() == ESpnPage::MainMenu)
	{
		return; // the main menu is the base of the stack and cannot be closed
	}
	Pop();
}

void USpnUiSubsystem::CloseAll()
{
	while (!Stack.IsEmpty() && Stack.Last() != ESpnPage::MainMenu)
	{
		Pop();
	}
}

void USpnUiSubsystem::ShowMainMenu()
{
	USpnMenuScreen* Menu = GetOrCreateMenu(ESpnPage::MainMenu);
	if (Menu == nullptr)
	{
		return;
	}
	Stack.Reset();
	bInputModeApplied = false;
	Push(ESpnPage::MainMenu);
}

bool USpnUiSubsystem::OpenPause()
{
	USpnSessionSubsystem* Session = GetSession();
	if (Session == nullptr || !Session->IsGameFlowEnabled() || Session->IsMenuWorld() || !Stack.IsEmpty() || GetController() == nullptr)
	{
		return false;
	}
	if (GetOrCreateMenu(ESpnPage::Pause) == nullptr)
	{
		return false;
	}
	EnsureHud();
	Push(ESpnPage::Pause);
	if (USpnMenuScreen* PauseMenu = GetMenuScreen(ESpnPage::Pause))
	{
		PauseMenu->Select(PauseMenu->FindIndex(TEXT("resume"))); // a fresh pause always starts on Resume, never on the last used entry
	}
	return true;
}

bool USpnUiSubsystem::OpenMap()
{
	USpnSessionSubsystem* Session = GetSession();
	UGameInstance* Instance = GetInstance();
	USpnMapSubsystem* Map = Instance != nullptr ? Instance->GetSubsystem<USpnMapSubsystem>() : nullptr;
	APlayerController* Controller = GetController();
	if (Session == nullptr || !Session->IsGameFlowEnabled() || Session->IsMenuWorld() || !Stack.IsEmpty() || Controller == nullptr || Map == nullptr || !Map->HasDefinition())
	{
		return false;
	}
	if (MapScreen == nullptr || MapScreen->GetOwningPlayer() != Controller)
	{
		MapScreen = CreateWidget<USpnMapScreen>(Controller, USpnMapScreen::StaticClass());
		MapScreen->OnCloseRequested.BindLambda([this]() { Back(); });
	}
	MapScreen->Setup(Map);
	MapScreen->SetUsingGamepad(bUsingGamepad);
	EnsureHud();
	Push(ESpnPage::Map);
	MapScreen->OnShown();
	return true;
}

void USpnUiSubsystem::OpenSettings()
{
	if (GetOrCreateMenu(ESpnPage::Settings) != nullptr)
	{
		Push(ESpnPage::Settings);
	}
}

void USpnUiSubsystem::OpenSlots(ESpnSlotMode Mode)
{
	if (GetOrCreateMenu(ESpnPage::Slots) != nullptr)
	{
		SlotMode = Mode;
		Push(ESpnPage::Slots);
	}
}

void USpnUiSubsystem::ShowConfirm(const FText& Title, const FText& Body, const FText& YesLabel, TFunction<void()> OnYes)
{
	if (GetOrCreateMenu(ESpnPage::Confirm) != nullptr)
	{
		ConfirmTitle = Title;
		ConfirmBody = Body;
		ConfirmYes = YesLabel;
		ConfirmAction = MoveTemp(OnYes);
		Push(ESpnPage::Confirm);
	}
}

// --- page content ---------------------------------------------------------------------------------------------------------------------

FText USpnUiSubsystem::SlotTitle(const FString& SlotId) const
{
	if (SlotId.StartsWith(TEXT("save_")) && SlotId.Len() > 5 && SlotId.Mid(5).IsNumeric())
	{
		return T(TEXT("ui.slots.save_n"), {FText::FromString(SlotId.Mid(5))});
	}
	return FText::FromString(SlotId);
}

FText USpnUiSubsystem::DescribeSaveResult(const FSpnSaveStatus& Status) const
{
	const TCHAR* Key = TEXT("ui.save.err.other");
	switch (Status.Result)
	{
	case ESpnSaveResult::NotFound: Key = TEXT("ui.save.err.notfound"); break;
	case ESpnSaveResult::Corrupt: Key = TEXT("ui.save.err.corrupt"); break;
	case ESpnSaveResult::UnsupportedVersion: Key = TEXT("ui.save.err.newer"); break;
	case ESpnSaveResult::CatalogMismatch: Key = TEXT("ui.save.err.catalog"); break;
	case ESpnSaveResult::SectionsNotIntegrated: Key = TEXT("ui.save.err.sections"); break;
	case ESpnSaveResult::Io: Key = TEXT("ui.save.err.io"); break;
	case ESpnSaveResult::InvalidSlotId: Key = TEXT("ui.save.err.slotid"); break;
	case ESpnSaveResult::Busy: Key = TEXT("ui.save.err.busy"); break;
	case ESpnSaveResult::InconsistentSnapshot: Key = TEXT("ui.save.err.inconsistent"); break;
	case ESpnSaveResult::Rejected: Key = TEXT("ui.save.err.rejected"); break;
	case ESpnSaveResult::Ok: return T(TEXT("ui.save.ok"));
	default: break;
	}
	return T(Key);
}

void USpnUiSubsystem::FillMainMenu()
{
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::MainMenu);
	USpnSessionSubsystem* Session = GetSession();
	const USpnSaveSubsystem* Save = GetInstance() != nullptr ? GetInstance()->GetSubsystem<USpnSaveSubsystem>() : nullptr;
	if (Menu == nullptr)
	{
		return;
	}
	const FString Latest = Session != nullptr ? Session->FindContinueSlot() : FString();
	const bool bAnySlot = Save != nullptr && !Save->ListSlots().IsEmpty();

	TArray<FSpnMenuItem> Items;
	FSpnMenuItem Continue;
	Continue.Id = TEXT("continue");
	Continue.Title = T(TEXT("ui.menu.continue"));
	Continue.bEnabled = !Latest.IsEmpty();
	Continue.Subtitle = Latest.IsEmpty() ? T(TEXT("ui.menu.continue.none")) : SlotTitle(Latest);
	Items.Add(Continue);
	FSpnMenuItem NewGame; NewGame.Id = TEXT("new"); NewGame.Title = T(TEXT("ui.menu.new")); Items.Add(NewGame);
	FSpnMenuItem Load; Load.Id = TEXT("load"); Load.Title = T(TEXT("ui.menu.load")); Load.bEnabled = bAnySlot;
	Load.Subtitle = bAnySlot ? FText::GetEmpty() : T(TEXT("ui.menu.load.none")); Items.Add(Load);
	FSpnMenuItem Settings; Settings.Id = TEXT("settings"); Settings.Title = T(TEXT("ui.menu.settings")); Items.Add(Settings);
	FSpnMenuItem Quit; Quit.Id = TEXT("quit"); Quit.Title = T(TEXT("ui.menu.quit")); Items.Add(Quit);

	Menu->SetTexts(T(TEXT("ui.menu.title")), T(TEXT("ui.menu.tagline")), T(TEXT("ui.menu.footer")));
	Menu->SetItems(Items);
}

void USpnUiSubsystem::FillPause()
{
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::Pause);
	if (Menu == nullptr)
	{
		return;
	}
	TArray<FSpnMenuItem> Items;
	auto Add = [&Items](const TCHAR* Id, const TCHAR* Key) { FSpnMenuItem I; I.Id = Id; I.Title = T(Key); Items.Add(I); };
	Add(TEXT("resume"), TEXT("ui.pause.resume"));
	Add(TEXT("save"), TEXT("ui.pause.save"));
	Add(TEXT("load"), TEXT("ui.pause.load"));
	Add(TEXT("settings"), TEXT("ui.menu.settings"));
	Add(TEXT("mainmenu"), TEXT("ui.pause.mainmenu"));
	Add(TEXT("quit"), TEXT("ui.menu.quit"));
	Menu->SetTexts(T(TEXT("ui.pause.title")), FText::GetEmpty(), T(TEXT("ui.pause.footer")));
	Menu->SetItems(Items);
}

void USpnUiSubsystem::FillConfirm()
{
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::Confirm);
	if (Menu == nullptr)
	{
		return;
	}
	TArray<FSpnMenuItem> Items;
	FSpnMenuItem No; No.Id = TEXT("no"); No.Title = T(TEXT("ui.confirm.no")); Items.Add(No);
	FSpnMenuItem Yes; Yes.Id = TEXT("yes"); Yes.Title = ConfirmYes; Yes.SubtitleColor = SpnUi::Warning; Items.Add(Yes);
	Menu->SetTexts(ConfirmTitle, ConfirmBody, T(TEXT("ui.confirm.footer")));
	Menu->SetItems(Items, TEXT("no")); // the safe answer is the default
}

FString USpnUiSubsystem::NextSlotId() const
{
	const USpnSaveSubsystem* Save = GetInstance() != nullptr ? GetInstance()->GetSubsystem<USpnSaveSubsystem>() : nullptr;
	for (int32 I = 1; I <= 99 && Save != nullptr; ++I)
	{
		const FString Id = FString::Printf(TEXT("save_%02d"), I);
		if (!Save->DoesSlotExist(Id)) { return Id; }
	}
	return FString();
}

void USpnUiSubsystem::FillSlots()
{
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::Slots);
	const USpnSaveSubsystem* Save = GetInstance() != nullptr ? GetInstance()->GetSubsystem<USpnSaveSubsystem>() : nullptr;
	if (Menu == nullptr || Save == nullptr)
	{
		return;
	}
	SlotItems.Reset();
	TArray<FSpnMenuItem> Items;
	if (SlotMode == ESpnSlotMode::Save)
	{
		FSpnMenuItem New;
		New.Id = TEXT("new_slot");
		New.Title = T(TEXT("ui.slots.new"));
		const FString Next = NextSlotId();
		New.bEnabled = !Next.IsEmpty();
		New.Subtitle = Next.IsEmpty() ? T(TEXT("ui.slots.full")) : T(TEXT("ui.slots.new.hint"), {SlotTitle(Next)});
		Items.Add(New);
	}
	const TArray<FString> Slots = Save->ListSlots();
	for (const FString& Slot : Slots)
	{
		const USpnSaveSubsystem::FSpnSlotInfo Info = Save->InspectSlot(Slot);
		FSpnMenuItem Item;
		Item.Id = FName(*FString::Printf(TEXT("slot:%s"), *Slot));
		Item.Title = SlotTitle(Slot);
		Item.Value = Info.Modified.GetTicks() > 0 ? FText::FromString(Info.Modified.ToString(TEXT("%Y-%m-%d %H:%M"))) : FText::GetEmpty();
		const bool bLoadable = Info.Result == ESpnSaveResult::Ok || Info.Result == ESpnSaveResult::CatalogMismatch;
		switch (Info.Result)
		{
		case ESpnSaveResult::Ok:
			Item.Subtitle = T(TEXT("ui.slots.summary"), {FText::AsNumber(static_cast<int64>(Info.FactsRevision)), T(Info.bHasInventory ? TEXT("ui.slots.yes") : TEXT("ui.slots.no")),
				FText::AsNumber(Info.MapMarkers)});
			if (Info.bFromPrevious)
			{
				Item.Subtitle = T(TEXT("ui.slots.previous"));
				Item.SubtitleColor = SpnUi::Warning;
			}
			break;
		case ESpnSaveResult::CatalogMismatch:
			Item.Subtitle = T(TEXT("ui.slots.catalog"));
			Item.SubtitleColor = SpnUi::Warning;
			break;
		default:
			Item.Subtitle = FText::FromString(FString::Printf(TEXT("%s %s"), *DescribeSaveResult(FSpnSaveStatus{Info.Result}).ToString(), *Info.Detail.Left(120)));
			Item.SubtitleColor = SpnUi::Error;
			break;
		}
		// loading needs a loadable slot; saving may overwrite any slot (the user is asked first), a damaged one included
		Item.bEnabled = SlotMode == ESpnSlotMode::Save ? Info.Result != ESpnSaveResult::InvalidSlotId : bLoadable;
		SlotItems.Add(Item.Id, Slot);
		Items.Add(Item);
	}
	if (Slots.IsEmpty())
	{
		FSpnMenuItem Empty;
		Empty.Id = TEXT("empty");
		Empty.Title = T(TEXT("ui.slots.empty"));
		Empty.bHeader = true;
		Items.Add(Empty);
	}
	Menu->SetTexts(T(SlotMode == ESpnSlotMode::Save ? TEXT("ui.slots.title.save") : TEXT("ui.slots.title.load")), FText::GetEmpty(), T(TEXT("ui.slots.footer")));
	Menu->SetItems(Items);
}

void USpnUiSubsystem::FillSettings()
{
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::Settings);
	UGameInstance* Instance = GetInstance();
	USpnSettingsSubsystem* Settings = Instance != nullptr ? Instance->GetSubsystem<USpnSettingsSubsystem>() : nullptr;
	if (Menu == nullptr || Settings == nullptr)
	{
		return;
	}
	const FSpnPlayerSettings& S = Settings->Get();
	const bool bDisplay = USpnSettingsSubsystem::CanChangeDisplay();
	TArray<FSpnMenuItem> Items;
	auto Header = [&Items](const TCHAR* Key) { FSpnMenuItem I; I.Id = FName(Key); I.Title = T(Key); I.bHeader = true; Items.Add(I); };
	auto Row = [&Items](const TCHAR* Id, const TCHAR* TitleKey, const FText& Value, bool bEnabled = true) -> FSpnMenuItem&
	{
		FSpnMenuItem I; I.Id = Id; I.Title = T(TitleKey); I.Value = Value; I.bAdjustable = true; I.bEnabled = bEnabled;
		if (!bEnabled) { I.Subtitle = T(TEXT("ui.settings.unavailable")); }
		return Items.Add_GetRef(I);
	};
	auto OnOff = [](bool b) { return T(b ? TEXT("ui.settings.on") : TEXT("ui.settings.off")); };

	Header(TEXT("ui.settings.graphics"));
	const EWindowMode::Type Mode = USpnSettingsSubsystem::GetDisplayMode();
	Row(TEXT("display"), TEXT("ui.settings.display"), T(Mode == EWindowMode::Fullscreen ? TEXT("ui.settings.mode.fullscreen") : Mode == EWindowMode::WindowedFullscreen ? TEXT("ui.settings.mode.borderless") : TEXT("ui.settings.mode.windowed")), bDisplay);
	const FIntPoint Resolution = USpnSettingsSubsystem::GetResolution();
	Row(TEXT("resolution"), TEXT("ui.settings.resolution"), FText::FromString(FString::Printf(TEXT("%d × %d"), Resolution.X, Resolution.Y)), bDisplay);
	static const TCHAR* Quality[] = {TEXT("ui.settings.q.low"), TEXT("ui.settings.q.medium"), TEXT("ui.settings.q.high"), TEXT("ui.settings.q.epic"), TEXT("ui.settings.q.cinematic")};
	const int32 Level = USpnSettingsSubsystem::GetScalabilityLevel();
	Row(TEXT("quality"), TEXT("ui.settings.quality"), T(Level >= 0 && Level <= 4 ? Quality[Level] : TEXT("ui.settings.q.custom")), bDisplay);
	Row(TEXT("vsync"), TEXT("ui.settings.vsync"), OnOff(USpnSettingsSubsystem::IsVSync()), bDisplay);
	const float Fps = USpnSettingsSubsystem::GetFrameRateLimit();
	Row(TEXT("fps"), TEXT("ui.settings.fps"), Fps <= 0.f ? T(TEXT("ui.settings.unlimited")) : FText::AsNumber(FMath::RoundToInt(Fps)), bDisplay);

	Header(TEXT("ui.settings.audio"));
	Row(TEXT("volume"), TEXT("ui.settings.volume"), Percent(S.MasterVolume));
	Row(TEXT("music"), TEXT("ui.settings.music"), Percent(S.MusicVolume)).Subtitle = T(TEXT("ui.settings.audio_note"));
	Row(TEXT("effects"), TEXT("ui.settings.effects"), Percent(S.EffectsVolume)).Subtitle = T(TEXT("ui.settings.audio_note"));

	Header(TEXT("ui.settings.input"));
	Row(TEXT("mouse"), TEXT("ui.settings.mouse"), Multiplier(S.MouseSensitivity));
	Row(TEXT("pad"), TEXT("ui.settings.pad"), Multiplier(S.GamepadSensitivity));
	Row(TEXT("invert"), TEXT("ui.settings.invert"), OnOff(S.bInvertY));

	Header(TEXT("ui.settings.interface"));
	Row(TEXT("minimap"), TEXT("ui.settings.minimap"), OnOff(S.bMinimapVisible));
	Row(TEXT("orientation"), TEXT("ui.settings.orientation"), T(S.bMinimapHeadingUp ? TEXT("ui.settings.heading_up") : TEXT("ui.settings.north_up")));
	Row(TEXT("zoom"), TEXT("ui.settings.zoom"), Multiplier(S.MinimapZoom));
	Row(TEXT("smoothing"), TEXT("ui.settings.smoothing"), OnOff(S.bCameraSmoothing));
	Row(TEXT("uiscale"), TEXT("ui.settings.uiscale"), Percent(S.UiScale));
	Row(TEXT("language"), TEXT("ui.settings.language"), S.Language == TEXT("de") ? FText::FromString(TEXT("Deutsch")) : S.Language == TEXT("en") ? FText::FromString(TEXT("English")) : T(TEXT("ui.settings.auto")));

	Menu->SetTexts(T(TEXT("ui.settings.title")), FText::GetEmpty(), T(TEXT("ui.settings.footer")));
	Menu->SetItems(Items);
}

void USpnUiSubsystem::OnAdjust(ESpnPage Page, FName Id, int32 Delta)
{
	if (Page != ESpnPage::Settings)
	{
		return;
	}
	UGameInstance* Instance = GetInstance();
	USpnSettingsSubsystem* Settings = Instance != nullptr ? Instance->GetSubsystem<USpnSettingsSubsystem>() : nullptr;
	if (Settings == nullptr)
	{
		return;
	}
	FSpnPlayerSettings S = Settings->Get();
	bool bPlayerSetting = true;
	if (Id == TEXT("display"))
	{
		bPlayerSetting = false;
		const EWindowMode::Type Order[] = {EWindowMode::Windowed, EWindowMode::WindowedFullscreen, EWindowMode::Fullscreen};
		int32 Index = 0;
		for (int32 I = 0; I < 3; ++I) { if (Order[I] == USpnSettingsSubsystem::GetDisplayMode()) { Index = I; } }
		USpnSettingsSubsystem::SetDisplayMode(Order[(Index + Delta + 3) % 3]);
	}
	else if (Id == TEXT("resolution"))
	{
		bPlayerSetting = false;
		const TArray<FIntPoint> List = USpnSettingsSubsystem::GetSupportedResolutions();
		int32 Index = List.IndexOfByKey(USpnSettingsSubsystem::GetResolution());
		Index = FMath::Clamp((Index == INDEX_NONE ? 0 : Index) + Delta, 0, List.Num() - 1);
		USpnSettingsSubsystem::SetResolution(List[Index]);
	}
	else if (Id == TEXT("quality"))
	{
		bPlayerSetting = false;
		const int32 Level = USpnSettingsSubsystem::GetScalabilityLevel();
		USpnSettingsSubsystem::SetScalabilityLevel(FMath::Clamp((Level < 0 ? 2 : Level) + Delta, 0, 4));
	}
	else if (Id == TEXT("vsync")) { bPlayerSetting = false; USpnSettingsSubsystem::SetVSync(!USpnSettingsSubsystem::IsVSync()); }
	else if (Id == TEXT("fps"))
	{
		bPlayerSetting = false;
		const int32 Index = FMath::Clamp(NearestIndex(FpsOptions, USpnSettingsSubsystem::GetFrameRateLimit()) + Delta, 0, UE_ARRAY_COUNT(FpsOptions) - 1);
		USpnSettingsSubsystem::SetFrameRateLimit(static_cast<float>(FpsOptions[Index]));
	}
	else if (Id == TEXT("volume")) { S.MasterVolume = FMath::Clamp(FMath::RoundToFloat(S.MasterVolume * 10.f + Delta) / 10.f, 0.f, 1.f); }
	else if (Id == TEXT("music")) { S.MusicVolume = FMath::Clamp(FMath::RoundToFloat(S.MusicVolume * 10.f + Delta) / 10.f, 0.f, 1.f); }
	else if (Id == TEXT("effects")) { S.EffectsVolume = FMath::Clamp(FMath::RoundToFloat(S.EffectsVolume * 10.f + Delta) / 10.f, 0.f, 1.f); }
	else if (Id == TEXT("uiscale")) { S.UiScale = FMath::Clamp(FMath::RoundToFloat(S.UiScale * 20.f + Delta) / 20.f, 0.75f, 1.5f); }
	else if (Id == TEXT("mouse")) { S.MouseSensitivity = FMath::Clamp(FMath::RoundToFloat(S.MouseSensitivity * 10.f + Delta) / 10.f, 0.1f, 4.f); }
	else if (Id == TEXT("pad")) { S.GamepadSensitivity = FMath::Clamp(FMath::RoundToFloat(S.GamepadSensitivity * 10.f + Delta) / 10.f, 0.1f, 4.f); }
	else if (Id == TEXT("invert")) { S.bInvertY = !S.bInvertY; }
	else if (Id == TEXT("minimap")) { S.bMinimapVisible = !S.bMinimapVisible; }
	else if (Id == TEXT("orientation")) { S.bMinimapHeadingUp = !S.bMinimapHeadingUp; }
	else if (Id == TEXT("zoom")) { S.MinimapZoom = ZoomOptions[FMath::Clamp(NearestIndex(ZoomOptions, S.MinimapZoom) + Delta, 0, UE_ARRAY_COUNT(ZoomOptions) - 1)]; }
	else if (Id == TEXT("smoothing")) { S.bCameraSmoothing = !S.bCameraSmoothing; }
	else if (Id == TEXT("language"))
	{
		const TCHAR* Order[] = {TEXT("auto"), TEXT("en"), TEXT("de")};
		int32 Index = 0;
		for (int32 I = 0; I < 3; ++I) { if (S.Language == Order[I]) { Index = I; } }
		S.Language = Order[(Index + Delta + 3) % 3];
	}
	else
	{
		return;
	}
	if (bPlayerSetting)
	{
		Settings->Set(S);
	}
	// the language changes every text of the open pages
	for (const ESpnPage Open : Stack)
	{
		if (Open != ESpnPage::Map) { RefreshPage(Open); }
	}
	if (Hud != nullptr) { Hud->SetUsingGamepad(bUsingGamepad); }
}

void USpnUiSubsystem::DoSaveToSlot(const FString& SlotId)
{
	USpnSessionSubsystem* Session = GetSession();
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::Slots);
	if (Session == nullptr)
	{
		return;
	}
	const FSpnSaveStatus Status = Session->SaveGame(SlotId);
	if (Status.IsOk())
	{
		if (Menu != nullptr) { Menu->SetMessage(T(TEXT("ui.save.saved"), {SlotTitle(SlotId)}), SpnUi::Good); }
		Notify(T(TEXT("ui.save.saved"), {SlotTitle(SlotId)}), SpnUi::ENote::Good);
		FillSlots();
		if (Menu != nullptr) { Menu->SetMessage(T(TEXT("ui.save.saved"), {SlotTitle(SlotId)}), SpnUi::Good); }
	}
	else
	{
		const FText Message = T(TEXT("ui.save.failed"), {SlotTitle(SlotId), DescribeSaveResult(Status)});
		if (Menu != nullptr) { Menu->SetMessage(Message, SpnUi::Error); }
		Notify(Message, SpnUi::ENote::Error);
		UE_LOG(LogSupernaturalProt, Warning, TEXT("UI: save to '%s' failed: %s %s"), *SlotId, LexToString(Status.Result), *Status.Detail);
	}
}

void USpnUiSubsystem::DoLoadSlot(const FString& SlotId, bool bAcceptMismatch)
{
	USpnSessionSubsystem* Session = GetSession();
	if (Session == nullptr)
	{
		return;
	}
	const bool bFromMenu = Session->IsMenuWorld();
	const FSpnSaveStatus Status = Session->LoadGame(SlotId, bAcceptMismatch);
	if (Status.Result == ESpnSaveResult::CatalogMismatch && !bAcceptMismatch)
	{
		// never accepted silently: the player decides, the inventory is validated against the current definitions either way
		ShowConfirm(T(TEXT("ui.save.catalog.title")), T(TEXT("ui.save.catalog.body")), T(TEXT("ui.save.catalog.yes")), [this, SlotId]() { DoLoadSlot(SlotId, true); });
		return;
	}
	USpnMenuScreen* Menu = GetMenuScreen(ESpnPage::Slots);
	if (!Status.IsOk())
	{
		const FText Message = T(TEXT("ui.save.loadfailed"), {SlotTitle(SlotId), DescribeSaveResult(Status)});
		if (Menu != nullptr) { Menu->SetMessage(Message, SpnUi::Error); }
		UE_LOG(LogSupernaturalProt, Warning, TEXT("UI: load of '%s' failed: %s %s"), *SlotId, LexToString(Status.Result), *Status.Detail);
		return;
	}
	if (bFromMenu)
	{
		if (Menu != nullptr) { Menu->SetMessage(T(TEXT("ui.save.loading")), SpnUi::Good); }
		return; // the level change is under way
	}
	CloseAll();
	Notify(Status.bMapRestored || Status.bInventoryRestored ? T(TEXT("ui.save.loaded")) : T(TEXT("ui.save.loaded.old")), SpnUi::ENote::Good);
}

void USpnUiSubsystem::OnActivate(ESpnPage Page, FName Id, bool bSecondary)
{
	USpnSessionSubsystem* Session = GetSession();
	if (Session == nullptr)
	{
		return;
	}
	switch (Page)
	{
	case ESpnPage::MainMenu:
		if (Id == TEXT("continue"))
		{
			const FString Latest = Session->FindContinueSlot();
			if (!Latest.IsEmpty()) { DoLoadSlot(Latest, false); }
		}
		else if (Id == TEXT("new")) { Session->StartNewGame(); }
		else if (Id == TEXT("load")) { OpenSlots(ESpnSlotMode::Load); }
		else if (Id == TEXT("settings")) { OpenSettings(); }
		else if (Id == TEXT("quit")) { Session->QuitGame(); }
		break;
	case ESpnPage::Pause:
		if (Id == TEXT("resume")) { Back(); }
		else if (Id == TEXT("save")) { OpenSlots(ESpnSlotMode::Save); }
		else if (Id == TEXT("load")) { OpenSlots(ESpnSlotMode::Load); }
		else if (Id == TEXT("settings")) { OpenSettings(); }
		else if (Id == TEXT("mainmenu"))
		{
			ShowConfirm(T(TEXT("ui.confirm.mainmenu.title")), T(TEXT("ui.confirm.mainmenu.body")), T(TEXT("ui.confirm.mainmenu.yes")), [Session]() { Session->ReturnToMainMenu(); });
		}
		else if (Id == TEXT("quit"))
		{
			ShowConfirm(T(TEXT("ui.confirm.quit.title")), T(TEXT("ui.confirm.quit.body")), T(TEXT("ui.confirm.quit.yes")), [Session]() { Session->QuitGame(); });
		}
		break;
	case ESpnPage::Slots:
	{
		if (Id == TEXT("new_slot"))
		{
			const FString Next = NextSlotId();
			if (!Next.IsEmpty()) { DoSaveToSlot(Next); }
			break;
		}
		const FString* Slot = SlotItems.Find(Id);
		if (Slot == nullptr) { break; }
		if (SlotMode == ESpnSlotMode::Save)
		{
			const FString Chosen = *Slot;
			ShowConfirm(T(TEXT("ui.confirm.overwrite.title")), T(TEXT("ui.confirm.overwrite.body"), {SlotTitle(Chosen)}), T(TEXT("ui.confirm.overwrite.yes")),
				[this, Chosen]() { DoSaveToSlot(Chosen); });
		}
		else if (Session->IsMenuWorld())
		{
			DoLoadSlot(*Slot, false);
		}
		else
		{
			const FString Chosen = *Slot;
			ShowConfirm(T(TEXT("ui.confirm.load.title")), T(TEXT("ui.confirm.load.body"), {SlotTitle(Chosen)}), T(TEXT("ui.confirm.load.yes")),
				[this, Chosen]() { DoLoadSlot(Chosen, false); });
		}
		break;
	}
	case ESpnPage::Confirm:
	{
		TFunction<void()> Action = Id == TEXT("yes") ? ConfirmAction : TFunction<void()>();
		Back(); // close the dialog first: the action may travel to another level or push another page
		if (Action) { Action(); }
		break;
	}
	default:
		break;
	}
}

// --- QA hooks (only with -SpnQaKeys on the command line) -----------------------------------------------------------------------------------

#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Misc/CommandLine.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/SpnWorldObjects.h"

namespace
{
	struct FQaStation { const TCHAR* Name; FVector Where; FVector Target; };
	// Stations of the Night Build 01 integration run in Lvl_SpnMovementTestV3Interiors (UE cm, floor z = 4): the character stands 1-2.5 m from the object
	// and looks at it; everything after that (E, M, I, menus, saving) is real input. EastSouthUp frame: yaw 0 = east, +Y = south.
	const FQaStation Stations[] = {
		{TEXT("yard_pickup"), FVector(-10280.f, -5440.f, 102.f), FVector(-10150.f, -5440.f, 20.f)},
		{TEXT("cottage_door"), FVector(-9600.f, -5440.f, 102.f), FVector(-9400.f, -5440.f, 110.f)},
		{TEXT("room2_flashlight"), FVector(-8840.f, -5290.f, 106.f), FVector(-8840.f, -5170.f, 20.f)},
		{TEXT("hall_crate"), FVector(-9150.f, -5440.f, 106.f), FVector(-9150.f, -5340.f, 20.f)},
		{TEXT("room1_cupboard"), FVector(-8700.f, -5640.f, 106.f), FVector(-8700.f, -5740.f, 30.f)},
	};
}

void USpnUiSubsystem::StartQaPoseFeed()
{
	FString Dir;
	if (!FParse::Value(FCommandLine::Get(), TEXT("SpnQaDir="), Dir) || Dir.IsEmpty())
	{
		Dir = FPaths::ProjectSavedDir() / TEXT("NightBuild01");
	}
	const FString File = FPaths::ConvertRelativePathToFull(Dir) / TEXT("pose.txt");
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(File), true);
	QaPoseFeed = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this, File](float)
	{
		const APlayerController* Controller = GetController();
		const APawn* Pawn = Controller != nullptr ? Controller->GetPawn() : nullptr;
		FString Text;
		if (Pawn != nullptr)
		{
			const FVector P = Pawn->GetActorLocation();
			const FVector Eye = Pawn->GetPawnViewLocation();
			const FRotator R = Controller->GetControlRotation();
			Text = FString::Printf(TEXT("%.1f %.1f %.1f %.2f %.2f %.1f %.1f %.1f %d %d %d %d %d"), P.X, P.Y, P.Z, FRotator::NormalizeAxis(R.Yaw), FRotator::NormalizeAxis(R.Pitch), Eye.X, Eye.Y,
				Eye.Z, Stack.Num(), bPausedByUi ? 1 : 0, Controller->bShowMouseCursor ? 1 : 0, Controller->IsLookInputIgnored() ? 1 : 0, Controller->IsMoveInputIgnored() ? 1 : 0);
		}
		else
		{
			Text = TEXT("none");
		}
		FFileHelper::SaveStringToFile(Text, *File, FFileHelper::EEncodingOptions::ForceAnsi);
		return true;
	}), 0.03f);
}

bool USpnUiSubsystem::HandleQaKey(const FKey& Key)
{
	static const bool bQa = FParse::Param(FCommandLine::Get(), TEXT("SpnQaKeys"));
	if (!bQa || (Key != EKeys::F5 && Key != EKeys::F9))
	{
		return false;
	}
	if (Key == EKeys::F9)
	{
		UE_LOG(LogSupernaturalProt, Display, TEXT("NB01-STATE %s"), *BuildStateDump());
		return true;
	}
	APlayerController* Controller = GetController();
	ACharacter* Character = Controller != nullptr ? Cast<ACharacter>(Controller->GetPawn()) : nullptr;
	if (Character == nullptr)
	{
		return true;
	}
	const FQaStation& Station = Stations[QaStation % UE_ARRAY_COUNT(Stations)];
	Character->SetActorLocation(Station.Where, false, nullptr, ETeleportType::TeleportPhysics);
	// look straight at the object (what a player does: the interaction ray follows the camera)
	const FVector Eye = Character->GetPawnViewLocation();
	Controller->SetControlRotation((Station.Target - Eye).Rotation());
	if (Character->GetCharacterMovement() != nullptr) { Character->GetCharacterMovement()->StopMovementImmediately(); }
	UE_LOG(LogSupernaturalProt, Display, TEXT("NB01-QA teleported to station %d '%s'"), QaStation % UE_ARRAY_COUNT(Stations), Station.Name);
	++QaStation;
	return true;
}

FString USpnUiSubsystem::BuildStateDump() const
{
	UGameInstance* Instance = GetInstance();
	const UWorld* World = GetLocalPlayer() != nullptr ? GetLocalPlayer()->GetWorld() : nullptr;
	if (Instance == nullptr)
	{
		return TEXT("no game instance");
	}
	const USpnInventorySubsystem* Inventory = Instance->GetSubsystem<USpnInventorySubsystem>();
	const USpnMapSubsystem* Map = Instance->GetSubsystem<USpnMapSubsystem>();
	const USpnWorldStateSubsystem* WorldState = Instance->GetSubsystem<USpnWorldStateSubsystem>();
	FString Out;
	static const TCHAR* const Items[] = {TEXT("ration"), TEXT("bandage"), TEXT("flashlight"), TEXT("notebook"), TEXT("crate"), TEXT("coat")};
	if (Inventory != nullptr && Inventory->HasCatalog())
	{
		Out += TEXT("player{");
		for (const TCHAR* Item : Items)
		{
			Out += FString::Printf(TEXT("%s=%llu "), Item, static_cast<unsigned long long>(Inventory->CountOf(SpnWorld::PlayerContainerId, FString::Printf(TEXT("spn_test:item.%s"), Item))));
		}
		FSpnContainerView View;
		if (Inventory->GetContainer(SpnWorld::PlayerContainerId, View))
		{
			for (const FSpnStackView& Item : View.Stacks) { if (!Item.EquippedSlot.IsEmpty()) { Out += FString::Printf(TEXT("equipped:%s@%s "), *Item.Definition, *Item.EquippedSlot); } }
		}
		Out += FString::Printf(TEXT("} cupboard{bandage=%llu notebook=%llu} rev=%llu "),
			static_cast<unsigned long long>(Inventory->CountOf(SpnWorld::ContainerIdFor(TEXT("nb01.container.cupboard")), TEXT("spn_test:item.bandage"))),
			static_cast<unsigned long long>(Inventory->CountOf(SpnWorld::ContainerIdFor(TEXT("nb01.container.cupboard")), TEXT("spn_test:item.notebook"))),
			static_cast<unsigned long long>(Inventory->GetRevision()));
	}
	if (WorldState != nullptr)
	{
		Out += FString::Printf(TEXT("taken{yard=%d flashlight=%d crate=%d} "), WorldState->ReadFlag(TEXT("world.pickup.nb01.pickup.ration_yard.taken")).Get(false),
			WorldState->ReadFlag(TEXT("world.pickup.nb01.pickup.flashlight_room2.taken")).Get(false), WorldState->ReadFlag(TEXT("world.pickup.nb01.pickup.crate_hall.taken")).Get(false));
	}
	if (World != nullptr)
	{
		for (TActorIterator<ASpnTestDoor> It(World); It; ++It)
		{
			if (It->StableId == TEXT("d_cottage1_door")) { Out += FString::Printf(TEXT("door_cottage1_open=%d "), It->IsOpen()); }
		}
		const APlayerController* Controller = GetController();
		if (Controller != nullptr && Controller->GetPawn() != nullptr)
		{
			const FVector P = Controller->GetPawn()->GetActorLocation();
			Out += FString::Printf(TEXT("pos=(%.0f,%.0f,%.0f) "), P.X, P.Y, P.Z);
			if (Map != nullptr && Map->HasDefinition())
			{
				const FSpnNavigationReading R = Map->GetNavigation(P, Controller->GetControlRotation().Yaw);
				Out += R.bActive ? FString::Printf(TEXT("waypoint=\"%s\" dist=%.1f bearing=%.0f arrived=%d "), *R.Label, R.DistanceMetres, FMath::RadiansToDegrees(R.BearingRadians), R.bArrived) : FString(TEXT("waypoint=none "));
				Out += FString::Printf(TEXT("markers=%d "), Map->GetMarkers().Num());
			}
		}
	}
	const USpnInventoryUiSubsystem* InventoryUi = GetLocalPlayer() != nullptr ? GetLocalPlayer()->GetSubsystem<USpnInventoryUiSubsystem>() : nullptr;
	Out += FString::Printf(TEXT("ui{stack=%d top=%d paused=%d inventory=%d container_view=%d gamepad=%d} activeSlot=%s"), Stack.Num(), static_cast<int32>(GetTopPage()), bPausedByUi,
		InventoryUi != nullptr && InventoryUi->IsOpen(), InventoryUi != nullptr && InventoryUi->IsContainerView(), bUsingGamepad, GetSession() != nullptr ? *GetSession()->GetActiveSlot() : TEXT(""));
	return Out;
}
