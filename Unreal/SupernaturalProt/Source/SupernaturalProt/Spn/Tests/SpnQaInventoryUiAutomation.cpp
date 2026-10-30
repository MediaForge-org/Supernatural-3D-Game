#include "Engine/Console.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/GameInstance.h"
#include "Misc/OutputDeviceRedirector.h"
#include "Widgets/Input/SEditableText.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "SpnInventorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "Spn/UI/SpnInventoryScreen.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnUiText.h"
#include "Widgets/SWindow.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.InventoryUi (M2B3.5): the graphical inventory in a real, rendered gameplay session. Keys reach the screen through FSlateApplication
// (the same path as the keyboard: pre-processor of USpnInventoryUiSubsystem), the mouse through Slate pointer events at the real widget position.
// The gamepad is SIMULATED with gamepad key events (no physical DualSense is required or claimed). Screenshots show the UI (shots/InventoryUi_*.png).
// The test data are the temporary spn_test: items; nothing here is game content.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags UiQaFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("InventoryUi");
	constexpr int64 Body = 1;
	constexpr int64 Trunk = 2;
	constexpr int64 Box = 3;
	const FString Ration = TEXT("spn_test:item.ration");
	const FString Bandage = TEXT("spn_test:item.bandage");
	const FString Flashlight = TEXT("spn_test:item.flashlight");
	const FString Notebook = TEXT("spn_test:item.notebook");
	const FString Coat = TEXT("spn_test:item.coat");

	struct FCtx
	{
		FAutomationTestBase* Test = nullptr;
		TWeakObjectPtr<ASupernaturalProtCharacter> Character;
		int32 Checks = 0, Failed = 0;
		int32 OpenedWhileTyping = 0; // console hotfix: how often the inventory was open right after a typed key

		UWorld* World() const { return Character.IsValid() ? Character->GetWorld() : nullptr; }
		APlayerController* PC() const { return Character.IsValid() ? Cast<APlayerController>(Character->GetController()) : nullptr; }
		UGameInstance* GI() const { return World() != nullptr ? World()->GetGameInstance() : nullptr; }
		USpnInventorySubsystem* Inv() const { return GI() != nullptr ? GI()->GetSubsystem<USpnInventorySubsystem>() : nullptr; }
		USpnSaveSubsystem* Save() const { return GI() != nullptr ? GI()->GetSubsystem<USpnSaveSubsystem>() : nullptr; }
		USpnInventoryUiSubsystem* Ui() const { return PC() != nullptr && PC()->GetLocalPlayer() != nullptr ? PC()->GetLocalPlayer()->GetSubsystem<USpnInventoryUiSubsystem>() : nullptr; }
		USpnInventoryScreen* Screen() const { return Ui() != nullptr ? Ui()->GetScreen() : nullptr; }

		void Check(const FString& Id, bool bOk, const FString& Detail)
		{
			++Checks;
			Failed += bOk ? 0 : 1;
			SpnQa::Check(*Test, Character.Get(), Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Detail);
		}
	};

	/** A key press/release through Slate: pre-processors first, then widgets, exactly like a keyboard or pad event. */
	void SlateKey(const FKey& Key, bool bShift = false)
	{
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		const FModifierKeysState Modifiers(bShift, false, false, false, false, false, false, false, false);
		FSlateApplication::Get().ProcessKeyDownEvent(FKeyEvent(Key, Modifiers, User, false, 0, 0));
		FSlateApplication::Get().ProcessKeyUpEvent(FKeyEvent(Key, Modifiers, User, false, 0, 0));
	}

	/** Clicks the centre of a widget with a real Slate pointer event. Returns false if the widget has no geometry (not laid out). */
	bool SlateClick(USpnInventoryScreen& Screen, UWidget* Widget, FKey Button, bool bDouble = false)
	{
		if (Widget == nullptr)
		{
			return false;
		}
		const FGeometry Geometry = Widget->GetCachedGeometry();
		if (Geometry.GetAbsoluteSize().IsNearlyZero())
		{
			return false;
		}
		const FVector2D Center = Geometry.GetAbsolutePosition() + Geometry.GetAbsoluteSize() * 0.5f;
		TSharedPtr<SWindow> Window = FSlateApplication::Get().FindWidgetWindow(Screen.TakeWidget());
		if (!Window.IsValid())
		{
			return false;
		}
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		FSlateApplication::Get().SetCursorPos(Center);
		const TSet<FKey> NoButtons;
		const TSet<FKey> Pressed = {Button};
		FSlateApplication::Get().ProcessMouseMoveEvent(FPointerEvent(User, 0, Center, Center, NoButtons, FKey(), 0.f, FModifierKeysState()));
		FSlateApplication::Get().ProcessMouseButtonDownEvent(Window->GetNativeWindow(), FPointerEvent(User, 0, Center, Center, Pressed, Button, 0.f, FModifierKeysState()));
		FSlateApplication::Get().ProcessMouseButtonUpEvent(FPointerEvent(User, 0, Center, Center, NoButtons, Button, 0.f, FModifierKeysState()));
		if (bDouble)
		{
			FSlateApplication::Get().ProcessMouseButtonDoubleClickEvent(Window->GetNativeWindow(), FPointerEvent(User, 0, Center, Center, Pressed, Button, 0.f, FModifierKeysState()));
			FSlateApplication::Get().ProcessMouseButtonUpEvent(FPointerEvent(User, 0, Center, Center, NoButtons, Button, 0.f, FModifierKeysState()));
		}
		return true;
	}

	/** Mirrors the log into a string so the QA can prove that a console command really executed. */
	class FQaLogCapture : public FOutputDevice
	{
	public:
		FQaLogCapture() { GLog->AddOutputDevice(this); }
		virtual ~FQaLogCapture() override { GLog->RemoveOutputDevice(this); }
		virtual void Serialize(const TCHAR* Text, ELogVerbosity::Type, const FName&) override { Text_ += Text; Text_ += TEXT("\n"); }
		FString Text_;
	};
	TUniquePtr<FQaLogCapture> GCapture;
	void CaptureBegin(FCtx& C) { GCapture = MakeUnique<FQaLogCapture>(); C.OpenedWhileTyping = 0; }
	FString CaptureText(FCtx&) { const FString Text = GCapture.IsValid() ? GCapture->Text_ : FString(); GCapture.Reset(); return Text; }

	bool IsConsoleOpen()
	{
		return GEngine != nullptr && GEngine->GameViewport != nullptr && GEngine->GameViewport->ViewportConsole != nullptr && GEngine->GameViewport->ViewportConsole->ConsoleActive();
	}

	void SlateChar(TCHAR Char)
	{
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		FSlateApplication::Get().ProcessKeyCharEvent(FCharacterEvent(Char, FModifierKeysState(), User, false));
	}

	/** Opens the real viewport console with its key, types the line key by key (key down, char, key up) and optionally confirms with Enter. */
	void TypeLine(FCtx& C, const TCHAR* Line, bool bEnter)
	{
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		SlateKey(EKeys::Tilde);
		for (const TCHAR Char : FString(Line))
		{
			const FKey Key = Char == TEXT('.') ? EKeys::Period : Char == TEXT(' ') ? EKeys::SpaceBar : FKey(FName(*FString::Chr(FChar::ToUpper(Char))));
			const bool bShift = FChar::IsUpper(Char);
			const FModifierKeysState Modifiers(bShift, false, false, false, false, false, false, false, false);
			FSlateApplication::Get().ProcessKeyDownEvent(FKeyEvent(Key, Modifiers, User, false, static_cast<uint32>(Char), 0));
			SlateChar(Char);
			FSlateApplication::Get().ProcessKeyUpEvent(FKeyEvent(Key, Modifiers, User, false, static_cast<uint32>(Char), 0));
			if (C.Ui() != nullptr && C.Ui()->IsOpen()) { ++C.OpenedWhileTyping; }
		}
		if (bEnter)
		{
			SlateKey(EKeys::Enter);
			if (C.Ui() != nullptr && C.Ui()->IsOpen()) { ++C.OpenedWhileTyping; }
		}
	}

	int32 StackCountIn(FCtx& C, int64 Container)
	{
		FSpnContainerView View;
		return C.Inv() != nullptr && C.Inv()->GetContainer(static_cast<uint64>(Container), View) ? View.Stacks.Num() : -1;
	}

	bool IsEquipped(FCtx& C, const FString& Definition)
	{
		FSpnContainerView View;
		if (C.Inv() == nullptr || !C.Inv()->GetContainer(Body, View)) { return false; }
		for (const FSpnStackView& Stack : View.Stacks) { if (Stack.Definition == Definition && !Stack.EquippedSlot.IsEmpty()) { return true; } }
		return false;
	}

	int32 RowIndex(USpnInventoryScreen& Screen, const TCHAR* Title)
	{
		for (int32 I = 0; I < Screen.GetVisibleRowCount(); ++I) { if (Screen.GetRowTitle(I) == Title) { return I; } }
		return INDEX_NONE;
	}

	struct FStep
	{
		double Delay;
		FString Name;
		TFunction<void(FCtx&)> Run;
	};

	TArray<FStep> BuildSteps()
	{
		TArray<FStep> Steps;
		const auto Add = [&Steps](double Delay, const TCHAR* Name, TFunction<void(FCtx&)> Run) { Steps.Add({Delay, Name, MoveTemp(Run)}); };
		// the QA overlay lines (section, check result) would cover the screen: clear them, the evidence is the UI itself
		const auto Shot = [](const TCHAR* Id)
		{
			if (GEngine != nullptr) { GEngine->ClearOnScreenDebugMessages(); }
			SpnQa::Screenshot(FString(Suite) + TEXT("_") + Id);
		};

		Add(0.5, TEXT("prepare data"), [](FCtx& C)
		{
			SpnQa::Section(TEXT("Inventory UI: prepare (spn_test: catalog, 3 containers)"));
			FSpnUiText::SetLanguageOverride(TEXT("en"));
			USpnInventorySubsystem* Inv = C.Inv();
			C.Check("subsystems_exist", Inv != nullptr && C.Save() != nullptr && C.Ui() != nullptr, TEXT("inventory, save and UI subsystem of the local player exist"));
			if (Inv == nullptr) { return; }
			C.Save()->SetSaveDirectory(FPaths::ConvertRelativePathToFull(SpnQa::QaDir() / TEXT("saves")));
			Inv->ResetInventory();
			const bool bOk = Inv->LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath()).IsOk()
				&& Inv->CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")).IsOk() && Inv->CreateContainerFromPreset(Trunk, TEXT("spn_test:preset.trunk")).IsOk()
				&& Inv->CreateContainerFromPreset(Box, TEXT("spn_test:preset.small_box")).IsOk()
				&& Inv->AddItem(Body, Ration, 7).IsOk() && Inv->AddItem(Body, Flashlight).IsOk() && Inv->AddItem(Body, Bandage, 4).IsOk()
				&& Inv->AddItem(Body, Coat).IsOk() && Inv->AddItem(Body, Notebook).IsOk() && Inv->AddItem(Trunk, Bandage, 2).IsOk();
			C.Check("test_inventory_initialised", bOk, FString::Printf(TEXT("body %d stacks, trunk %d, box %d"), StackCountIn(C, Body), StackCountIn(C, Trunk), StackCountIn(C, Box)));
			C.Check("ui_closed_at_start", C.Ui() != nullptr && !C.Ui()->IsOpen(), TEXT("the screen is not open before the key is pressed"));
		});

		Add(1.0, TEXT("gameplay keys are not consumed while closed"), [](FCtx& C)
		{
			USpnInventoryUiSubsystem* Ui = C.Ui();
			if (Ui == nullptr) { return; }
			const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
			bool bAnyConsumed = false;
			// interact (E), jump (Space / pad Cross), dodge (pad Circle), attack (pad Square), heavy attack (pad Triangle), movement (W)
			for (const FKey& Key : {EKeys::E, EKeys::SpaceBar, EKeys::W, EKeys::LeftShift, EKeys::V, EKeys::Gamepad_FaceButton_Bottom, EKeys::Gamepad_FaceButton_Right, EKeys::Gamepad_FaceButton_Left, EKeys::Gamepad_FaceButton_Top, EKeys::Gamepad_LeftTriggerAxis})
			{
				bAnyConsumed |= Ui->ProcessKeyDown(FKeyEvent(Key, FModifierKeysState(), User, false, 0, 0));
			}
			C.Check("closed_ui_does_not_steal_gameplay_input", !bAnyConsumed && !Ui->IsOpen(), TEXT("E, Space, W, Shift, V and the DualSense face buttons / trigger are not consumed while the screen is closed"));
		});

		Add(0.8, TEXT("open with I"), [Shot](FCtx& C)
		{
			SpnQa::Section(TEXT("Inventory UI: open with the keyboard (I)"));
			SlateKey(EKeys::I);
			USpnInventoryUiSubsystem* Ui = C.Ui();
			C.Check("keyboard_opens_the_screen", Ui != nullptr && Ui->IsOpen() && Ui->GetScreen() != nullptr && Ui->GetScreen()->IsInViewport(), TEXT("pressing I through Slate opened the screen and put it in the viewport"));
			C.Check("gameplay_movement_blocked_while_open", C.PC() != nullptr && C.PC()->IsMoveInputIgnored() && C.PC()->bShowMouseCursor, TEXT("move input ignored, mouse cursor visible"));
			if (USpnInventoryScreen* Screen = C.Screen())
			{
				C.Check("shows_real_core_data", Screen->GetVisibleRowCount() == StackCountIn(C, Body) && Screen->GetVisibleTabCount() == 3, FString::Printf(TEXT("%d rows for %d Core stacks, %d tabs"), Screen->GetVisibleRowCount(), StackCountIn(C, Body), Screen->GetVisibleTabCount()));
			}
		});
		Add(0.8, TEXT("screenshot opened"), [Shot](FCtx&) { Shot(TEXT("01_open")); });

		Add(0.8, TEXT("navigate and inspect"), [Shot](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: keyboard navigation and details"));
			const FString First = Screen->GetDetailsTitle();
			SlateKey(EKeys::Down);
			SlateKey(EKeys::Down);
			C.Check("keyboard_moves_selection_and_details", Screen->GetDetailsTitle() != First && Screen->IsRowSelected(2), FString::Printf(TEXT("details '%s' -> '%s'"), *First, *Screen->GetDetailsTitle()));
			C.Check("details_show_weight_and_volume", Screen->GetDetailsText().Contains(TEXT("Weight")) && Screen->GetDetailsText().Contains(TEXT("Volume")), Screen->GetDetailsText().Replace(TEXT("\n"), TEXT(" | ")));
		});
		Add(0.8, TEXT("screenshot details"), [Shot](FCtx&) { Shot(TEXT("02_details")); });

		Add(0.6, TEXT("equip the flashlight with the keyboard"), [Shot](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: equip with the keyboard"));
			int32 Guard = 0;
			while (Screen->GetDetailsTitle() != TEXT("Test flashlight") && ++Guard < 20) { SlateKey(EKeys::Down); }
			SlateKey(EKeys::Right); // actions
			for (int32 I = 0; I < Screen->GetActionCount(); ++I)
			{
				if (Screen->GetActionTitle(I).StartsWith(TEXT("Equip"))) { while (Screen->GetFocusedActionIndex() < I) { SlateKey(EKeys::Down); } }
			}
			SlateKey(EKeys::Enter);
			C.Check("equip_goes_through_the_core", IsEquipped(C, Flashlight), TEXT("the Core shows the flashlight equipped after Enter on the Equip action"));
			SlateKey(EKeys::Left);
		});
		Add(0.8, TEXT("screenshot equipped"), [Shot](FCtx& C)
		{
			if (USpnInventoryScreen* Screen = C.Screen())
			{
				bool bShown = false;
				for (int32 I = 0; I < Screen->GetSlotCount(); ++I) { bShown |= Screen->GetSlotSubtitle(I) == TEXT("Test flashlight"); }
				const int32 Row = RowIndex(*Screen, TEXT("Test flashlight"));
				C.Check("equipment_panel_and_badge_update", bShown && Row != INDEX_NONE && !Screen->GetRowBadge(Row).IsEmpty(), TEXT("slot shows the flashlight and its row carries the EQUIPPED badge"));
			}
			Shot(TEXT("03_equipped"));
		});

		Add(0.6, TEXT("rejected action"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr || C.Inv() == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: a rejected action explains why and changes nothing"));
			// fill the small box so a transfer into it is refused by the Core
			C.Inv()->AddItem(Box, Notebook); C.Inv()->AddItem(Box, Notebook); C.Inv()->AddItem(Box, Notebook);
			int32 Guard = 0;
			while (Screen->GetDetailsTitle() != TEXT("Test notebook") && ++Guard < 20) { SlateKey(EKeys::Down); }
			const uint64 Revision = C.Inv()->GetRevision();
			SlateKey(EKeys::Right);
			bool bReason = false;
			for (int32 I = 0; I < Screen->GetActionCount(); ++I)
			{
				if (!Screen->IsActionEnabled(I) && Screen->GetActionTitle(I).Contains(TEXT("Small box")))
				{
					bReason = Screen->GetActionSubtitle(I).StartsWith(TEXT("not available:"));
					while (Screen->GetFocusedActionIndex() < I) { SlateKey(EKeys::Down); }
					SlateKey(EKeys::Enter);
				}
			}
			C.Check("unavailable_action_shows_reason", bReason, TEXT("the transfer into the full small box is listed disabled with the Core's reason"));
			C.Check("rejected_action_changes_nothing", C.Inv()->GetRevision() == Revision && Screen->GetMessageText().StartsWith(TEXT("not available:")), FString::Printf(TEXT("revision %llu unchanged, message '%s'"), Revision, *Screen->GetMessageText()));
			SlateKey(EKeys::Left);
		});
		Add(0.8, TEXT("screenshot rejected"), [Shot](FCtx&) { Shot(TEXT("04_rejected")); });

		Add(0.6, TEXT("switch container and transfer"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: container switching and transfer to the trunk"));
			SlateKey(EKeys::Tab);
			C.Check("tab_switches_to_the_trunk", Screen->GetActiveContainer() == Trunk && Screen->GetRowTitle(0) == TEXT("Test bandage"), FString::Printf(TEXT("active container %lld, first row '%s'"), Screen->GetActiveContainer(), *Screen->GetRowTitle(0)));
			SlateKey(EKeys::Tab, true);
			C.Check("shift_tab_returns_to_the_body", Screen->GetActiveContainer() == Body, FString::Printf(TEXT("active container %lld"), Screen->GetActiveContainer()));
			int32 Guard = 0;
			while (Screen->GetDetailsTitle() != TEXT("Test bandage") && ++Guard < 20) { SlateKey(EKeys::Down); }
			const int32 TrunkBefore = StackCountIn(C, Trunk);
			uint32 TrunkQuantityBefore = 0;
			{
				FSpnContainerView View;
				if (C.Inv()->GetContainer(Trunk, View)) { for (const FSpnStackView& Stack : View.Stacks) { TrunkQuantityBefore += Stack.Quantity; } }
			}
			SlateKey(EKeys::Right);
			for (int32 I = 0; I < Screen->GetActionCount(); ++I)
			{
				if (Screen->IsActionEnabled(I) && Screen->GetActionTitle(I).StartsWith(TEXT("Move to")))
				{
					while (Screen->GetFocusedActionIndex() < I) { SlateKey(EKeys::Down); }
					SlateKey(EKeys::Add); // quantity + (when the action takes one)
					SlateKey(EKeys::Enter);
					break;
				}
			}
			uint32 TrunkQuantityAfter = 0;
			{
				FSpnContainerView View;
				if (C.Inv()->GetContainer(Trunk, View)) { for (const FSpnStackView& Stack : View.Stacks) { TrunkQuantityAfter += Stack.Quantity; } }
			}
			C.Check("transfer_moves_items_in_the_core", TrunkQuantityAfter > TrunkQuantityBefore, FString::Printf(TEXT("trunk quantity %u -> %u, stacks %d -> %d"), TrunkQuantityBefore, TrunkQuantityAfter, TrunkBefore, StackCountIn(C, Trunk)));
			SlateKey(EKeys::Left);
		});
		Add(0.8, TEXT("screenshot transfer"), [Shot](FCtx&) { Shot(TEXT("05_transferred")); });

		Add(0.6, TEXT("mouse"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: mouse selection and right click"));
			const int32 Row = RowIndex(*Screen, TEXT("Test ration"));
			UWidget* Widget = Screen->GetRowWidget(Row);
			const bool bClicked = SlateClick(*Screen, Widget, EKeys::LeftMouseButton);
			C.Check("mouse_click_selects_a_row", bClicked && Row != INDEX_NONE && Screen->GetDetailsTitle() == TEXT("Test ration"), FString::Printf(TEXT("clicked=%d, details '%s'"), bClicked ? 1 : 0, *Screen->GetDetailsTitle()));
			const bool bRight = SlateClick(*Screen, Widget, EKeys::RightMouseButton);
			C.Check("right_click_opens_the_actions", bRight && Screen->GetZone() == USpnInventoryScreen::EZone::Actions, FString::Printf(TEXT("zone %d"), static_cast<int32>(Screen->GetZone())));
			SlateKey(EKeys::Left);
		});
		Add(0.8, TEXT("screenshot mouse"), [Shot](FCtx&) { Shot(TEXT("06_mouse")); });

		Add(0.6, TEXT("discard confirmation"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: discard asks for confirmation"));
			const int32 Before = StackCountIn(C, Body);
			SlateKey(EKeys::Delete);
			C.Check("discard_opens_a_confirmation", Screen->IsDialogOpen() && StackCountIn(C, Body) == Before, Screen->GetDialogText());
		});
		Add(0.8, TEXT("screenshot dialog"), [Shot](FCtx&) { Shot(TEXT("07_discard_dialog")); });
		Add(0.4, TEXT("cancel discard"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			const int32 Before = StackCountIn(C, Body);
			SlateKey(EKeys::Escape);
			C.Check("cancel_discards_nothing", !Screen->IsDialogOpen() && StackCountIn(C, Body) == Before, FString::Printf(TEXT("%d stacks"), StackCountIn(C, Body)));
		});

		Add(0.6, TEXT("simulated gamepad"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SpnQa::Section(TEXT("Inventory UI: gamepad navigation (SIMULATED gamepad key events)"));
			const int64 Before = Screen->GetViewModel()->GetSelectedStackId(); // compared by stack id: two stacks can share a name (ration 5 + 2)
			SlateKey(EKeys::Gamepad_DPad_Down);
			C.Check("dpad_moves_the_selection", Screen->GetViewModel()->GetSelectedStackId() != Before && Screen->IsUsingGamepad(), FString::Printf(TEXT("stack %lld -> %lld ('%s')"), Before, Screen->GetViewModel()->GetSelectedStackId(), *Screen->GetDetailsTitle()));
			C.Check("hints_switch_to_the_controller", Screen->GetHintText() == FSpnUiText::Get(TEXT("ui.hint.gamepad")).ToString(), Screen->GetHintText());
			SlateKey(EKeys::Gamepad_DPad_Right);
			C.Check("dpad_right_enters_the_actions", Screen->GetZone() == USpnInventoryScreen::EZone::Actions && Screen->GetActionCount() > 0, FString::Printf(TEXT("%d actions"), Screen->GetActionCount()));
			SlateKey(EKeys::Gamepad_FaceButton_Right); // Circle = back
			C.Check("circle_goes_back", Screen->GetZone() == USpnInventoryScreen::EZone::Items, TEXT("Circle returned from the actions to the items without closing"));
			SlateKey(EKeys::Gamepad_RightShoulder);
			C.Check("r1_switches_the_container", Screen->GetActiveContainer() == Trunk, FString::Printf(TEXT("active container %lld"), Screen->GetActiveContainer()));
			SlateKey(EKeys::Gamepad_LeftShoulder);
		});
		Add(0.8, TEXT("screenshot gamepad"), [Shot](FCtx&) { Shot(TEXT("08_gamepad_hints")); });

		Add(0.6, TEXT("close"), [](FCtx& C)
		{
			SpnQa::Section(TEXT("Inventory UI: close and restore the gameplay input state"));
			SlateKey(EKeys::Escape);
			USpnInventoryUiSubsystem* Ui = C.Ui();
			C.Check("escape_closes_the_screen", Ui != nullptr && !Ui->IsOpen(), TEXT("Escape at the top level closed the screen"));
			C.Check("input_state_restored", C.PC() != nullptr && !C.PC()->IsMoveInputIgnored() && !C.PC()->IsLookInputIgnored() && !C.PC()->bShowMouseCursor, TEXT("move and look input active again, mouse cursor hidden again"));
		});

		Add(0.6, TEXT("save"), [](FCtx& C)
		{
			SpnQa::Section(TEXT("Inventory UI: save, change the state, load, reopen"));
			USpnSaveSubsystem* Save = C.Save();
			if (Save == nullptr) { return; }
			const FSpnSaveStatus Status = Save->SaveSlot(TEXT("qa_inventory_ui"));
			C.Check("save_succeeds", Status.IsOk(), TEXT("slot qa_inventory_ui written"));
			C.Inv()->ResetInventory();
			C.Check("state_changed_after_save", StackCountIn(C, Body) < 0, TEXT("the inventory was reset after saving"));
		});
		Add(0.8, TEXT("open while empty"), [](FCtx& C)
		{
			SlateKey(EKeys::I);
			USpnInventoryScreen* Screen = C.Screen();
			C.Check("empty_inventory_is_shown_empty", Screen != nullptr && Screen->GetVisibleRowCount() == 0 && !Screen->GetEmptyText().IsEmpty(), Screen != nullptr ? Screen->GetEmptyText() : FString(TEXT("no screen")));
		});
		Add(0.8, TEXT("screenshot empty"), [Shot](FCtx&) { Shot(TEXT("09_empty_after_reset")); });
		Add(0.5, TEXT("load while open"), [](FCtx& C)
		{
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr || C.Save() == nullptr) { return; }
			const FSpnSaveStatus Status = C.Save()->LoadSlot(TEXT("qa_inventory_ui"));
			C.Check("load_succeeds", Status.IsOk(), TEXT("slot qa_inventory_ui loaded"));
			C.Check("open_screen_refreshes_after_load", Screen->GetVisibleRowCount() == StackCountIn(C, Body) && Screen->GetVisibleRowCount() > 0 && IsEquipped(C, Flashlight), FString::Printf(TEXT("%d rows, %d Core stacks, flashlight equipped=%d"), Screen->GetVisibleRowCount(), StackCountIn(C, Body), IsEquipped(C, Flashlight) ? 1 : 0));
		});
		Add(0.8, TEXT("screenshot restored"), [Shot](FCtx&) { Shot(TEXT("10_restored_after_load")); });
		Add(0.5, TEXT("reopen"), [](FCtx& C)
		{
			SlateKey(EKeys::I);
			SlateKey(EKeys::I); // closed, then opened again
			USpnInventoryScreen* Screen = C.Screen();
			C.Check("reopen_shows_current_state", C.Ui() != nullptr && C.Ui()->IsOpen() && Screen != nullptr && Screen->GetVisibleRowCount() == StackCountIn(C, Body), TEXT("closing and reopening shows the loaded state"));
		});

		Add(0.6, TEXT("german"), [](FCtx& C)
		{
			SpnQa::Section(TEXT("Inventory UI: German texts"));
			USpnInventoryScreen* Screen = C.Screen();
			if (Screen == nullptr) { return; }
			SlateKey(EKeys::I); // close
			FSpnUiText::SetLanguageOverride(TEXT("de"));
			SlateKey(EKeys::I); // open again
			C.Check("german_texts", Screen->GetHeaderTitle() == FSpnUiText::GetInLanguage(TEXT("ui.inventory.title"), TEXT("de")) && Screen->GetHeaderTitle() != TEXT("INVENTORY"), Screen->GetHeaderTitle());
		});
		Add(0.8, TEXT("screenshot german"), [Shot](FCtx&) { Shot(TEXT("11_german")); });
		// ---- M2B3.5 hotfix: typing in a console / text field must never reach the inventory hotkey ----
		Add(0.6, TEXT("text field focus"), [Shot](FCtx& C)
		{
			SpnQa::Section(TEXT("Console hotfix: a focused text field gets the keys, not the inventory hotkey"));
			USpnInventoryUiSubsystem* Ui = C.Ui();
			if (Ui == nullptr || GEngine == nullptr || GEngine->GameViewport == nullptr) { return; }
			if (Ui->IsOpen()) { SlateKey(EKeys::I); }
			TSharedRef<SEditableText> Field = SNew(SEditableText).HintText(FText::FromString(TEXT("QA text field")));
			GEngine->GameViewport->AddViewportWidgetContent(Field, 1000);
			FSlateApplication::Get().SetKeyboardFocus(Field, EFocusCause::SetDirectly);
			C.Check("text_field_reports_text_entry", Ui->IsTextEntryFocused(), TEXT("IsTextEntryFocused() is true with an editable text widget focused"));
			SlateKey(EKeys::I);
			C.Check("hotkey_ignored_in_text_field", !Ui->IsOpen(), TEXT("pressing I while a text field has focus did not open the inventory"));
			GEngine->GameViewport->RemoveViewportWidgetContent(Field);
			FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::SetDirectly);
			FSlateApplication::Get().SetAllUserFocusToGameViewport();
			SlateKey(EKeys::I);
			C.Check("hotkey_works_again_after_field_removed", Ui->IsOpen(), TEXT("after the field is gone, I opens the inventory again"));
			SlateKey(EKeys::I);
			C.Check("closed_again", !Ui->IsOpen(), TEXT("and closes it again"));
		});

		Add(0.6, TEXT("console: setup"), [Shot](FCtx& C)
		{
			SpnQa::Section(TEXT("Console hotfix: real viewport console, command typed character by character"));
			if (C.Inv() != nullptr) { C.Inv()->ResetInventory(); }
			CaptureBegin(C);
			TypeLine(C, TEXT("Spn.Inv.Setup"), true);
		});
		Add(0.8, TEXT("console: setup result"), [Shot](FCtx& C)
		{
			const FString Log = CaptureText(C);
			FSpnContainerView View;
			C.Check("console_setup_ran", Log.Contains(TEXT("Spn.Inv.Setup:")) && C.Inv() != nullptr && C.Inv()->GetContainer(Body, View), Log.Left(160));
			C.Check("console_setup_did_not_open_inventory", !C.Ui()->IsOpen() && C.OpenedWhileTyping == 0, FString::Printf(TEXT("inventory opened while typing: %d times"), C.OpenedWhileTyping));
			C.Check("console_closed_after_enter", !IsConsoleOpen(), TEXT("the console closed after Enter"));
			C.Check("gameplay_input_not_stuck", C.PC() != nullptr && !C.PC()->IsMoveInputIgnored() && !C.PC()->IsLookInputIgnored(), TEXT("move and look input not ignored after the console closed"));
		});
		Add(0.6, TEXT("console: list"), [](FCtx& C)
		{
			CaptureBegin(C);
			TypeLine(C, TEXT("Spn.Inv.List"), true);
		});
		Add(0.8, TEXT("console: list result"), [Shot](FCtx& C)
		{
			const FString Log = CaptureText(C);
			C.Check("console_list_ran", Log.Contains(TEXT("Spn.Inv.List:")), Log.Left(160));
			C.Check("console_list_did_not_open_inventory", !C.Ui()->IsOpen() && C.OpenedWhileTyping == 0, FString::Printf(TEXT("inventory opened while typing: %d times"), C.OpenedWhileTyping));
		});
		Add(0.6, TEXT("console: ui command"), [](FCtx& C)
		{
			CaptureBegin(C);
			TypeLine(C, TEXT("Spn.UI.Inventory open"), true);
		});
		Add(0.8, TEXT("console: ui command result"), [Shot](FCtx& C)
		{
			C.Check("console_ui_command_opens_on_request", C.Ui()->IsOpen(), TEXT("Spn.UI.Inventory open typed in the console opened the inventory exactly once, after Enter"));
			C.Check("no_open_while_typing_ui_command", C.OpenedWhileTyping == 1, FString::Printf(TEXT("open events during typing+execution: %d (only the command itself)"), C.OpenedWhileTyping));
		});
		Add(0.8, TEXT("screenshot console command"), [Shot](FCtx&) { Shot(TEXT("12_console_command_opened_inventory")); });
		Add(0.6, TEXT("console while inventory open"), [](FCtx& C)
		{
			SpnQa::Section(TEXT("Console hotfix: console while the inventory is open, Escape behaviour"));
			CaptureBegin(C);
			TypeLine(C, TEXT("Spn.Inv.List"), true);
		});
		Add(0.8, TEXT("console while open result"), [](FCtx& C)
		{
			const FString Log = CaptureText(C);
			C.Check("console_works_while_inventory_open", Log.Contains(TEXT("Spn.Inv.List:")) && C.Ui()->IsOpen(), Log.Left(160));
			C.Check("console_key_reaches_console_when_open", !IsConsoleOpen(), TEXT("the console opened (tilde was not swallowed by the screen), the command ran, the console closed; the inventory stays open"));
			SlateKey(EKeys::Escape);
			C.Check("escape_closes_inventory_after_console", !C.Ui()->IsOpen() && C.PC() != nullptr && !C.PC()->IsMoveInputIgnored(), TEXT("Escape with the console closed closes the inventory and restores input"));
		});
		Add(0.6, TEXT("console escape"), [](FCtx& C)
		{
			SlateKey(EKeys::Tilde);
			const bool bOpened = IsConsoleOpen();
			SlateKey(EKeys::Escape);
			C.Check("escape_closes_only_the_console", bOpened && !IsConsoleOpen() && !C.Ui()->IsOpen(), TEXT("tilde opened the console, Escape closed it, the inventory did not open"));
			// repeat open/close several times
			bool bAllOk = true;
			for (int32 I = 0; I < 5; ++I)
			{
				SlateKey(EKeys::Tilde); bAllOk &= IsConsoleOpen();
				SlateKey(EKeys::Escape); bAllOk &= !IsConsoleOpen() && !C.Ui()->IsOpen();
			}
			C.Check("console_repeated_open_close", bAllOk && C.PC() != nullptr && !C.PC()->IsMoveInputIgnored(), TEXT("5 open (console key) / close (Escape) cycles: state consistent, input not stuck"));
		});

		Add(0.5, TEXT("finish"), [](FCtx& C)
		{
			FSpnUiText::SetLanguageOverride(FString());
			if (C.Ui() != nullptr && C.Ui()->IsOpen()) { SlateKey(EKeys::Escape); }
			C.Check("closed_at_the_end", C.Ui() != nullptr && !C.Ui()->IsOpen() && C.PC() != nullptr && !C.PC()->IsMoveInputIgnored(), TEXT("the session ends with the screen closed and gameplay input restored"));
		});
		return Steps;
	}

	class FUiQaCommand : public IAutomationLatentCommand
	{
	public:
		FUiQaCommand(FAutomationTestBase& InTest) : Test(InTest), Steps(BuildSteps()) { Ctx.Test = &InTest; }

		virtual bool Update() override
		{
			if (!Ctx.Character.IsValid())
			{
				Ctx.Character = FindCharacter();
				if (!Ctx.Character.IsValid())
				{
					if (++WaitFrames > 900) { Test.AddError(TEXT("no possessed character after about 15 s in the map")); return true; }
					return false;
				}
				StepStart = FPlatformTime::Seconds();
				return false;
			}
			if (Index >= Steps.Num())
			{
				SpnQa::Check(Test, Ctx.Character.Get(), Suite, TEXT("run"), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps ran, %d checks, %d failed"), Steps.Num(), Ctx.Checks, Ctx.Failed));
				return true;
			}
			if (FPlatformTime::Seconds() - StepStart < Steps[Index].Delay)
			{
				return false;
			}
			UE_LOG(LogTemp, Display, TEXT("SPNQA InventoryUi step %d/%d: %s"), Index + 1, Steps.Num(), *Steps[Index].Name);
			Steps[Index].Run(Ctx);
			++Index;
			StepStart = FPlatformTime::Seconds();
			return false;
		}

	private:
		FAutomationTestBase& Test;
		TArray<FStep> Steps;
		FCtx Ctx;
		int32 Index = 0;
		int32 WaitFrames = 0;
		double StepStart = 0.0;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaInventoryUiTest, "SpnQA.InventoryUi", UiQaFlags)

bool FSpnQaInventoryUiTest::RunTest(const FString&)
{
	const FString Map = SpnQa::QaMapPath();
	if (!FPackageName::DoesPackageExist(Map)) { AddError(*FString::Printf(TEXT("%s does not exist"), *Map)); return false; }
	AutomationOpenMap(Map);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FUiQaCommand(*this));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	return true;
}

#endif
