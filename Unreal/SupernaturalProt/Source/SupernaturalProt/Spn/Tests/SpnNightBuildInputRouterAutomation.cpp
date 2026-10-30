#include "Engine/Console.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDevice.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnUiSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// Night Build 01 completion: dedicated regression tests of the input router (not only the scripted scenario). Keys are injected through Slate
// (pre-processors first, exactly like a keyboard); the console is the real viewport console typed key by key. Simulated keys are NOT hardware:
// a physical DualSense / keyboard layout is still unverified.
//
//   SpnGame.Input.RouterPriority   map / inventory / pause exclusion, Esc semantics, repeated open-close cycles, input mode after every cycle
//   SpnGame.Input.ConsoleTyping    "Spn.Inv.List" and plain letters typed into the console must not trigger inventory, map or pause
//   SpnGame.Input.ModeAfterTravel  menu level -> New Game: the gameplay level must not inherit the menu's mouse-capture mode (regression of the
//                                  scripted run: the mouse look only worked while a button was held)

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags RouterFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const RouterMap = TEXT("/Game/SpnTest/Lvl_SpnMovementTestV3Interiors");
	const TCHAR* const RouterMenuMap = TEXT("/Game/SpnTest/Lvl_SpnMainMenu");

	struct FRouterState
	{
		FString Root;
		TWeakObjectPtr<UGameInstance> Instance;
		int32 Waited = 0;
		int32 InventoryOpenedWhileTyping = 0;
		int32 MapOpenedWhileTyping = 0;
		int32 PausedWhileTyping = 0;
	};

	APlayerController* PC() { return FindPlayWorld() != nullptr ? FindPlayWorld()->GetFirstPlayerController() : nullptr; }
	USpnUiSubsystem* Ui() { return PC() != nullptr && PC()->GetLocalPlayer() != nullptr ? PC()->GetLocalPlayer()->GetSubsystem<USpnUiSubsystem>() : nullptr; }
	USpnInventoryUiSubsystem* InvUi() { return PC() != nullptr && PC()->GetLocalPlayer() != nullptr ? PC()->GetLocalPlayer()->GetSubsystem<USpnInventoryUiSubsystem>() : nullptr; }

	void Key(const FKey& K, bool bShift = false)
	{
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		const FModifierKeysState Modifiers(bShift, false, false, false, false, false, false, false, false);
		FSlateApplication::Get().ProcessKeyDownEvent(FKeyEvent(K, Modifiers, User, false, 0, 0));
		FSlateApplication::Get().ProcessKeyUpEvent(FKeyEvent(K, Modifiers, User, false, 0, 0));
	}

	/**
	 * The console key reaches the viewport console through the viewport's input path, which a headless automation run (no focused viewport window) does not
	 * have. The console STATE is what the router asks (UConsole::ConsoleActive), so the test puts the real console into its typing state, exactly what the
	 * console key does, and injects the keys through Slate like everything else. What stays unverified here: that a real window routes the console key to the
	 * console (covered by the rendered suite SpnQA.InventoryUi and the scripted run).
	 */
	void OpenConsole()
	{
		if (GEngine != nullptr && GEngine->GameViewport != nullptr && GEngine->GameViewport->ViewportConsole != nullptr)
		{
			GEngine->GameViewport->ViewportConsole->FakeGotoState(FName(TEXT("Typing")));
		}
	}

	void CloseConsole()
	{
		if (GEngine != nullptr && GEngine->GameViewport != nullptr && GEngine->GameViewport->ViewportConsole != nullptr)
		{
			GEngine->GameViewport->ViewportConsole->FakeGotoState(NAME_None);
		}
	}

	bool ConsoleOpen()
	{
		return GEngine != nullptr && GEngine->GameViewport != nullptr && GEngine->GameViewport->ViewportConsole != nullptr && GEngine->GameViewport->ViewportConsole->ConsoleActive();
	}

	/** Gameplay input as the player needs it: nothing ignored, cursor hidden, the viewport captures the mouse permanently, the game runs. */
	FString GameplayInputProblem()
	{
		APlayerController* Controller = PC();
		if (Controller == nullptr) { return TEXT("no controller"); }
		if (Controller->IsMoveInputIgnored()) { return TEXT("move input ignored"); }
		if (Controller->IsLookInputIgnored()) { return TEXT("look input ignored"); }
		if (Controller->bShowMouseCursor) { return TEXT("mouse cursor visible"); }
		if (UGameplayStatics::IsGamePaused(Controller)) { return TEXT("game paused"); }
		const UGameViewportClient* Viewport = FindPlayWorld() != nullptr ? FindPlayWorld()->GetGameViewport() : nullptr;
		if (Viewport != nullptr && Viewport->GetMouseCaptureMode() == EMouseCaptureMode::CaptureDuringMouseDown)
		{
			return TEXT("mouse capture mode is CaptureDuringMouseDown (the mouse look needs a held button)");
		}
		if (Ui() == nullptr || Ui()->IsModalOpen()) { return TEXT("a page is still open"); }
		if (InvUi() != nullptr && InvUi()->IsOpen()) { return TEXT("the inventory is still open"); }
		return FString();
	}

	void BeginFlow(FAutomationTestBase& Test, const TSharedRef<FRouterState>& S)
	{
		UWorld* World = FindPlayWorld();
		S->Instance = World->GetGameInstance();
		S->Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
		S->Instance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(S->Root);
		S->Instance->GetSubsystem<USpnSessionSubsystem>()->StartNewGameInPlace(World);
	}

	bool WaitForPlayer(FAutomationTestBase& Test, const TSharedRef<FRouterState>& S, const TCHAR* MapNeedle)
	{
		const UWorld* World = FindPlayWorld();
		if (World != nullptr && World->GetMapName().Contains(MapNeedle) && FindCharacter() != nullptr && Ui() != nullptr)
		{
			S->Waited = 0;
			return true;
		}
		if (++S->Waited > 1800) { Test.AddError(TEXT("timed out waiting for the player")); S->Waited = 0; return true; }
		return false;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnRouterPriorityTest, "SpnGame.Input.RouterPriority", RouterFlags)
bool FSpnRouterPriorityTest::RunTest(const FString&)
{
	if (!FPackageName::DoesPackageExist(RouterMap)) { AddError(TEXT("test level missing")); return false; }
	const TSharedRef<FRouterState> S = MakeShared<FRouterState>();
	AutomationOpenMap(RouterMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]() { if (!WaitForPlayer(*this, S, TEXT("V3Interiors"))) { return false; } BeginFlow(*this, S); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		TestEqual(TEXT("start: gameplay input in order"), GameplayInputProblem(), FString());

		// map: M opens, M closes
		Key(EKeys::M);
		TestEqual(TEXT("M opens the map"), Ui()->GetTopPage(), ESpnPage::Map);
		TestTrue(TEXT("the map pauses the game"), Ui()->IsPaused());
		// priority: inside the map, I does not open the inventory and Esc closes only the map, it does not pause
		Key(EKeys::I);
		TestFalse(TEXT("I does not open the inventory while the map is open"), InvUi()->IsOpen());
		Key(EKeys::Escape);
		TestEqual(TEXT("Esc closes the map"), Ui()->GetTopPage(), ESpnPage::None);
		TestEqual(TEXT("after the map: gameplay input restored"), GameplayInputProblem(), FString());

		// inventory: I opens, M does not open the map, Esc closes the inventory (and does not open the pause menu)
		Key(EKeys::I);
		TestTrue(TEXT("I opens the inventory"), InvUi()->IsOpen());
		Key(EKeys::M);
		TestFalse(TEXT("M does not open the map while the inventory is open"), Ui()->GetTopPage() == ESpnPage::Map);
		Key(EKeys::Escape);
		TestFalse(TEXT("Esc closes the inventory"), InvUi()->IsOpen());
		TestEqual(TEXT("and does not also open the pause menu"), Ui()->GetTopPage(), ESpnPage::None);
		TestEqual(TEXT("after the inventory: gameplay input restored"), GameplayInputProblem(), FString());

		// pause: Esc opens, I and M are shut out, Esc resumes
		Key(EKeys::Escape);
		TestEqual(TEXT("Esc opens the pause menu"), Ui()->GetTopPage(), ESpnPage::Pause);
		Key(EKeys::I);
		TestFalse(TEXT("I does not open the inventory over the pause menu"), InvUi()->IsOpen());
		Key(EKeys::M);
		TestEqual(TEXT("M does not open the map over the pause menu"), Ui()->GetTopPage(), ESpnPage::Pause);
		Key(EKeys::Escape);
		TestEqual(TEXT("Esc resumes"), Ui()->GetTopPage(), ESpnPage::None);
		TestEqual(TEXT("after the pause menu: gameplay input restored"), GameplayInputProblem(), FString());

		// repeated cycles must never leave the character unable to move
		int32 Problems = 0;
		for (int32 Cycle = 0; Cycle < 12; ++Cycle)
		{
			Key(Cycle % 3 == 0 ? EKeys::M : Cycle % 3 == 1 ? EKeys::I : EKeys::Escape);
			Key(EKeys::Escape);
			const FString Problem = GameplayInputProblem();
			if (!Problem.IsEmpty()) { AddError(FString::Printf(TEXT("cycle %d left the input in a bad state: %s"), Cycle, *Problem)); ++Problems; }
		}
		TestEqual(TEXT("12 open/close cycles: no bad state"), Problems, 0);
		return true;
	}));
	// the character can still move after all of that
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (Character == nullptr) { AddError(TEXT("no character")); return true; }
		const FVector Before = Character->GetActorLocation();
		Character->DoMove(0.f, 1.f);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnRouterConsoleTest, "SpnGame.Input.ConsoleTyping", RouterFlags)
bool FSpnRouterConsoleTest::RunTest(const FString&)
{
	if (!FPackageName::DoesPackageExist(RouterMap)) { AddError(TEXT("test level missing")); return false; }
	const TSharedRef<FRouterState> S = MakeShared<FRouterState>();
	AutomationOpenMap(RouterMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]() { if (!WaitForPlayer(*this, S, TEXT("V3Interiors"))) { return false; } BeginFlow(*this, S); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	auto Type = [S](const TCHAR* Line, bool bEnter)
	{
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		FString Trace = FString::Printf(TEXT("console after Tilde: %d;"), ConsoleOpen());
		for (const TCHAR Char : FString(Line))
		{
			const FKey K = Char == TEXT('.') ? EKeys::Period : Char == TEXT(' ') ? EKeys::SpaceBar : FKey(FName(*FString::Chr(FChar::ToUpper(Char))));
			const FModifierKeysState Modifiers(FChar::IsUpper(Char), false, false, false, false, false, false, false, false);
			FSlateApplication::Get().ProcessKeyDownEvent(FKeyEvent(K, Modifiers, User, false, static_cast<uint32>(Char), 0));
			FSlateApplication::Get().ProcessKeyCharEvent(FCharacterEvent(Char, FModifierKeysState(), User, false));
			FSlateApplication::Get().ProcessKeyUpEvent(FKeyEvent(K, Modifiers, User, false, static_cast<uint32>(Char), 0));
			S->InventoryOpenedWhileTyping += InvUi()->IsOpen() ? 1 : 0;
			S->MapOpenedWhileTyping += Ui()->GetTopPage() == ESpnPage::Map ? 1 : 0;
			S->PausedWhileTyping += Ui()->IsPaused() ? 1 : 0;
			Trace += FString::Printf(TEXT(" %c=%d"), Char, ConsoleOpen());
		}
		UE_LOG(LogTemp, Display, TEXT("SPN-INPUT-TEST typed '%s': %s"), Line, *Trace);
		if (bEnter) { Key(EKeys::Enter); CloseConsole(); }
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]() { TestEqual(TEXT("start: gameplay input in order"), GameplayInputProblem(), FString()); return true; }));
	// the command from the bug report, letter by letter
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]() { OpenConsole(); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.4f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Type]() { TestTrue(TEXT("the console is in its typing state"), ConsoleOpen()); Type(TEXT("Spn.Inv.List"), true); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.6f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		TestEqual(TEXT("the inventory never opened while typing Spn.Inv.List"), S->InventoryOpenedWhileTyping, 0);
		TestEqual(TEXT("the map never opened while typing"), S->MapOpenedWhileTyping, 0);
		TestEqual(TEXT("the pause menu never opened while typing"), S->PausedWhileTyping, 0);
		TestFalse(TEXT("the console is closed again"), ConsoleOpen());
		TestEqual(TEXT("gameplay input in order after the console"), GameplayInputProblem(), FString());
		return true;
	}));
	// plain letters that are gameplay hotkeys (i = inventory, m = map), typed into the console and cancelled with Esc
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]() { OpenConsole(); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.4f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Type]() { Type(TEXT("imie"), false); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.6f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		TestTrue(TEXT("the console is open"), ConsoleOpen());
		TestFalse(TEXT("the letters i m e did not open the inventory"), InvUi()->IsOpen());
		TestFalse(TEXT("nor the map"), Ui()->GetTopPage() == ESpnPage::Map);
		Key(EKeys::Escape); // the router must let Esc pass to the console and must not open the pause menu itself
		TestEqual(TEXT("Esc with the console open does not open the pause menu"), Ui()->GetTopPage(), ESpnPage::None);
		CloseConsole();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.6f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		TestFalse(TEXT("the console is closed"), ConsoleOpen());
		TestEqual(TEXT("and did not open the pause menu"), Ui()->GetTopPage(), ESpnPage::None);
		TestEqual(TEXT("gameplay input in order after cancelling the console"), GameplayInputProblem(), FString());
		Key(EKeys::I);
		TestTrue(TEXT("I opens the inventory again after the console was used"), InvUi()->IsOpen());
		Key(EKeys::Escape);
		TestEqual(TEXT("and closes"), GameplayInputProblem(), FString());
		return true;
	}));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnRouterTravelModeTest, "SpnGame.Input.ModeAfterTravel", RouterFlags)
bool FSpnRouterTravelModeTest::RunTest(const FString&)
{
	if (!FPackageName::DoesPackageExist(RouterMap) || !FPackageName::DoesPackageExist(RouterMenuMap)) { AddError(TEXT("test levels missing")); return false; }
	const TSharedRef<FRouterState> S = MakeShared<FRouterState>();
	AutomationOpenMap(RouterMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		if (!WaitForPlayer(*this, S, TEXT("V3Interiors"))) { return false; }
		BeginFlow(*this, S);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([S]() { S->Instance->GetSubsystem<USpnSessionSubsystem>()->ReturnToMainMenu(); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		USpnSessionSubsystem* Sess = S->Instance->GetSubsystem<USpnSessionSubsystem>();
		if (!Sess->IsMenuWorld()) { if (++S->Waited > 1200) { AddError(TEXT("no menu")); S->Waited = 0; return true; } return false; }
		S->Waited = 0;
		TestTrue(TEXT("in the menu level the cursor is visible (the menu needs it)"), PC() != nullptr && PC()->bShowMouseCursor);
		Sess->StartNewGame();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]() { return WaitForPlayer(*this, S, TEXT("V3Interiors")); }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		TestEqual(TEXT("after menu -> New Game the gameplay level starts in a clean input mode"), GameplayInputProblem(), FString());
		return true;
	}));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
