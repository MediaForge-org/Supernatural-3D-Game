#include "Spn/UI/SpnInventoryUiSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Console.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "SpnInventorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "Spn/UI/SpnInventoryScreen.h"
#include "Spn/UI/SpnUiText.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpnInventoryUi, Log, All);

/** Pre-processor: sees every key before the widgets and the game. Closed: only the open keys. Open: everything the screen handles. */
class FSpnInventoryInputProcessor : public IInputProcessor
{
public:
	explicit FSpnInventoryInputProcessor(USpnInventoryUiSubsystem* InOwner) : Owner(InOwner) {}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& Event) override
	{
		return Owner.IsValid() && Owner->ProcessKeyDown(Event);
	}

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& Event) override
	{
		return Owner.IsValid() && Owner->ProcessAnalog(Event);
	}

	virtual const TCHAR* GetDebugName() const override { return TEXT("SpnInventoryInput"); }

private:
	TWeakObjectPtr<USpnInventoryUiSubsystem> Owner;
};

void USpnInventoryUiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (const USpnInventoryUiSettings* Settings = GetDefault<USpnInventoryUiSettings>())
	{
		if (!Settings->LanguageOverride.IsEmpty())
		{
			FSpnUiText::SetLanguageOverride(Settings->LanguageOverride);
		}
	}
	if (FSlateApplication::IsInitialized())
	{
		Processor = MakeShared<FSpnInventoryInputProcessor>(this);
		FSlateApplication::Get().RegisterInputPreProcessor(Processor, 0);
	}
}

void USpnInventoryUiSubsystem::Deinitialize()
{
	if (bOpen)
	{
		CloseInventory();
	}
	if (Processor.IsValid() && FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(Processor);
	}
	Processor.Reset();
	if (Screen != nullptr)
	{
		Screen->Teardown();
		Screen = nullptr;
	}
	Super::Deinitialize();
}

APlayerController* USpnInventoryUiSubsystem::GetController() const
{
	const ULocalPlayer* Player = GetLocalPlayer();
	return Player != nullptr ? Player->GetPlayerController(Player->GetWorld()) : nullptr;
}

bool USpnInventoryUiSubsystem::IsConsoleActive() const
{
	const UWorld* World = GetWorld();
	const UGameViewportClient* Viewport = World != nullptr ? World->GetGameViewport() : nullptr;
	return Viewport != nullptr && Viewport->ViewportConsole != nullptr && Viewport->ViewportConsole->ConsoleActive();
}

bool USpnInventoryUiSubsystem::OpenInventory()
{
	if (bOpen)
	{
		return true;
	}
	APlayerController* Controller = GetController();
	UGameInstance* GameInstance = GetLocalPlayer() != nullptr ? GetLocalPlayer()->GetGameInstance() : nullptr;
	if (Controller == nullptr || GameInstance == nullptr)
	{
		return false;
	}
	USpnInventorySubsystem* Inventory = GameInstance->GetSubsystem<USpnInventorySubsystem>();
	if (Inventory == nullptr)
	{
		return false;
	}
	if (Screen == nullptr || Screen->GetOwningPlayer() != Controller)
	{
		if (Screen != nullptr)
		{
			Screen->Teardown();
			Screen->RemoveFromParent();
		}
		Screen = CreateWidget<USpnInventoryScreen>(Controller, USpnInventoryScreen::StaticClass());
		Screen->OnCloseRequested.BindUObject(this, &USpnInventoryUiSubsystem::CloseInventory);
	}
	// first bound container = the player's own (lowest id) unless a QA setup says otherwise
	Screen->Setup(Inventory, GameInstance->GetSubsystem<USpnSaveSubsystem>(), 0);
	Screen->AddToViewport(100);

	bHadShowMouseCursor = Controller->bShowMouseCursor;
	Controller->FlushPressedKeys(); // a held movement key must not keep running while the screen is open
	Controller->SetShowMouseCursor(true);
	FInputModeGameAndUI Mode; // the pre-processor routes keys; the mouse is free for clicking; gameplay actions are blocked by IgnoreLookInput/MoveInput below
	Mode.SetWidgetToFocus(Screen->TakeWidget());
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Mode.SetHideCursorDuringCapture(false);
	Controller->SetInputMode(Mode);
	Controller->SetIgnoreMoveInput(true);
	Controller->SetIgnoreLookInput(true);

	bOpen = true;
	Screen->OnShown();
	UE_LOG(LogSpnInventoryUi, Log, TEXT("Inventory screen opened"));
	return true;
}

void USpnInventoryUiSubsystem::CloseInventory()
{
	if (!bOpen)
	{
		return;
	}
	bOpen = false;
	if (Screen != nullptr)
	{
		Screen->RemoveFromParent();
		Screen->Teardown();
	}
	if (APlayerController* Controller = GetController())
	{
		Controller->SetIgnoreMoveInput(false);
		Controller->SetIgnoreLookInput(false);
		Controller->SetShowMouseCursor(bHadShowMouseCursor);
		FInputModeGameOnly Mode;
		Controller->SetInputMode(Mode);
		Controller->FlushPressedKeys();
	}
	UE_LOG(LogSpnInventoryUi, Log, TEXT("Inventory screen closed"));
}

void USpnInventoryUiSubsystem::ToggleInventory()
{
	if (bOpen) { CloseInventory(); } else { OpenInventory(); }
}

bool USpnInventoryUiSubsystem::IsTextEntryFocused() const
{
	if (!FSlateApplication::IsInitialized())
	{
		return false;
	}
	// Editor Output Log command box, viewport console and any other text field: focus sits on the inner editable text
	const TSharedPtr<SWidget> Focused = FSlateApplication::Get().GetKeyboardFocusedWidget();
	if (!Focused.IsValid())
	{
		return false;
	}
	const FString Type = Focused->GetType().ToString();
	return Type.Contains(TEXT("EditableText")) || Type.Contains(TEXT("InputBox"));
}

bool USpnInventoryUiSubsystem::ProcessKeyDown(const FKeyEvent& Event)
{
	if (!bRoutingEnabled || IsConsoleActive() || IsTextEntryFocused())
	{
		return false;
	}
	const USpnInventoryUiSettings* Settings = GetDefault<USpnInventoryUiSettings>();
	const FKey Key = Event.GetKey();
	if (GetDefault<UInputSettings>()->ConsoleKeys.Contains(Key))
	{
		return false; // the console key always reaches the console, also while the inventory is open
	}
	if (!bOpen)
	{
		if (!Event.IsRepeat() && (Key == Settings->OpenKey || Key == Settings->OpenGamepadKey))
		{
			// only while a game world with a controller is running and no other UI owns the keyboard
			if (GetController() != nullptr && OpenInventory())
			{
				return true;
			}
		}
		return false;
	}
	if (Screen == nullptr)
	{
		return false;
	}
	if (!Event.IsRepeat() && (Key == Settings->OpenKey || Key == Settings->OpenGamepadKey))
	{
		CloseInventory();
		return true;
	}
	Screen->HandleKey(Key, Event.IsShiftDown(), Event.IsRepeat());
	return true; // while open the screen is modal: nothing reaches the gameplay (mouse keys are not routed through here)
}

bool USpnInventoryUiSubsystem::ProcessAnalog(const FAnalogInputEvent& Event)
{
	if (!bOpen || !bRoutingEnabled || Screen == nullptr)
	{
		return false;
	}
	Screen->HandleAnalog(Event.GetKey(), Event.GetAnalogValue());
	return false; // the value may still reach other listeners; movement is blocked by IgnoreMoveInput
}
