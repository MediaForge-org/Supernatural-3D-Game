#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnMapSubsystem.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/SpnWorldObjects.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// Night Build 01 completion: the player pose through REAL level loads. The character walks (real movement input), the game saves, the character
// walks elsewhere, the game loads in place, and the position / rotation / gait / inventory / waypoint / world facts must be back. Then the whole thing
// again through the main menu level: menu world -> LoadGame -> level travel -> the pose is applied once the pawn exists.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags PoseFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const GameplayMap = TEXT("/Game/SpnTest/Lvl_SpnMovementTestV3Interiors");
	const TCHAR* const MenuMap = TEXT("/Game/SpnTest/Lvl_SpnMainMenu");

	struct FPoseState
	{
		FString Root;
		FVector SavedLocation = FVector::ZeroVector;
		float SavedYaw = 0.f;
		float SavedPitch = 0.f;
		int64 SavedWaypointId = 0;
		bool bHaveSaved = false;
		int32 Waited = 0;
		TWeakObjectPtr<UGameInstance> Instance;
	};

	USpnSessionSubsystem* Session(const TSharedRef<FPoseState>& S) { return S->Instance.IsValid() ? S->Instance->GetSubsystem<USpnSessionSubsystem>() : nullptr; }

	/** Waits (bounded) for a possessed character in a world whose map name contains Needle, and for the session to have applied its pending pose. */
	bool WaitForGameplay(FAutomationTestBase& Test, const TSharedRef<FPoseState>& S, const TCHAR* Needle)
	{
		const UWorld* World = FindPlayWorld();
		USpnSessionSubsystem* Sess = Session(S);
		const bool bReady = World != nullptr && World->GetMapName().Contains(Needle) && FindCharacter() != nullptr && Sess != nullptr && !Sess->GetPendingPose().IsSet();
		if (bReady)
		{
			S->Waited = 0;
			return true;
		}
		if (++S->Waited > 1800)
		{
			Test.AddError(FString::Printf(TEXT("timed out waiting for the gameplay world (%s)"), Needle));
			S->Waited = 0;
			return true;
		}
		return false;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbPoseWorldTest, "SpnGame.NightBuild.PoseWorld", PoseFlags)
bool FSpnNbPoseWorldTest::RunTest(const FString&)
{
	if (!FPackageName::DoesPackageExist(GameplayMap) || !FPackageName::DoesPackageExist(MenuMap))
	{
		AddError(TEXT("the test levels Lvl_SpnMovementTestV3Interiors / Lvl_SpnMainMenu are missing (they are generated, not committed)"));
		return false;
	}
	const TSharedRef<FPoseState> S = MakeShared<FPoseState>();
	S->Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));

	AutomationOpenMap(GameplayMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// 1. bring the flow up, as Spn.Game.Enable does, with an isolated save root
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		UWorld* World = FindPlayWorld();
		if (World == nullptr || FindCharacter() == nullptr)
		{
			if (++S->Waited > 1800) { AddError(TEXT("the gameplay level never produced a character")); S->Waited = 0; return true; }
			return false;
		}
		S->Waited = 0;
		S->Instance = World->GetGameInstance();
		USpnSessionSubsystem* Sess = Session(S);
		USpnSaveSubsystem* Save = S->Instance->GetSubsystem<USpnSaveSubsystem>();
		if (Sess == nullptr || Save == nullptr)
		{
			AddError(TEXT("session / save subsystem missing"));
			return true;
		}
		Save->SetSaveDirectory(S->Root);
		Sess->EnableGameFlow();
		Sess->StartNewGameInPlace(World); // resets the facts / inventory / map and installs the scenario objects, like a new game would
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));

	// helper lambdas for the sequence
	auto Walk = [this](float Seconds, float Forward)
	{
		const TSharedRef<double> Start = MakeShared<double>(-1.0);
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Start, Seconds, Forward]()
		{
			ASupernaturalProtCharacter* Character = FindCharacter();
			if (Character == nullptr) { AddError(TEXT("no character while walking")); return true; }
			const double Now = Character->GetWorld()->GetTimeSeconds();
			if (*Start < 0.0) { *Start = Now; }
			Character->DoMove(0.f, Forward);
			return Now - *Start >= Seconds;
		}));
	};
	auto Inv = [S]() { return S->Instance->GetSubsystem<USpnInventorySubsystem>(); };
	auto Rations = [Inv]() { return Inv()->CountOf(SpnWorld::FindPlayerContainer(*Inv()), TEXT("spn_test:item.ration")); };

	// 2. walk (real movement), turn, set the state that has to survive, save
	Walk(1.2f, 1.f);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S, Inv]()
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (Character == nullptr) { AddError(TEXT("no character")); return true; }
		Character->GetController()->SetControlRotation(FRotator(-14.f, 37.f, 0.f));
		Character->GetSpnMovement()->SetBaseGait(ESpnGait::Walk);
		return true;
	}));
	Walk(0.6f, 1.f);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S, Inv]()
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (Character == nullptr) { AddError(TEXT("no character")); return true; }
		S->SavedLocation = Character->GetActorLocation();
		const FRotator View = Character->GetController()->GetControlRotation();
		S->SavedYaw = FRotator::NormalizeAxis(View.Yaw);
		S->SavedPitch = FRotator::NormalizeAxis(View.Pitch);
		USpnInventorySubsystem* Inventory = Inv();
		const uint64 Player = SpnWorld::FindPlayerContainer(*Inventory);
		TestTrue(TEXT("player container exists"), Player != 0);
		TestTrue(TEXT("two rations into the inventory"), Inventory->AddItem(Player, TEXT("spn_test:item.ration"), 2).IsOk());
		USpnMapSubsystem* Map = S->Instance->GetSubsystem<USpnMapSubsystem>();
		TestEqual(TEXT("waypoint set"), Map->SetWaypointAt(FVector2D(50.0, 40.0), TEXT("Pose test"), S->SavedWaypointId), ESpnMapResult::Ok);
		const FSpnSaveStatus Saved = Session(S)->SaveGame(TEXT("pose_world"));
		TestTrue(TEXT("saved"), Saved.IsOk());
		TestTrue(TEXT("with a pose"), Saved.bPoseInSlot);
		AddInfo(FString::Printf(TEXT("saved pose: loc (%.1f, %.1f, %.1f) yaw %.1f pitch %.1f"), S->SavedLocation.X, S->SavedLocation.Y, S->SavedLocation.Z, S->SavedYaw, S->SavedPitch));
		return true;
	}));

	// 3. play on: walk away backwards, change everything, then load IN PLACE
	Walk(1.5f, -1.f);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S, Inv]()
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (Character == nullptr) { return true; }
		TestTrue(TEXT("the character really moved away"), FVector::Dist(Character->GetActorLocation(), S->SavedLocation) > 100.0);
		Character->GetController()->SetControlRotation(FRotator(5.f, 200.f, 0.f));
		Character->GetSpnMovement()->SetBaseGait(ESpnGait::Run);
		Inv()->AddItem(SpnWorld::FindPlayerContainer(*Inv()), TEXT("spn_test:item.ration"), 3);
		S->Instance->GetSubsystem<USpnMapSubsystem>()->ClearWaypoint();
		const FSpnSaveStatus Loaded = Session(S)->LoadGame(TEXT("pose_world"));
		TestTrue(TEXT("in-place load ok"), Loaded.IsOk());
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	auto Verify = [this, S, Rations](const TCHAR* When)
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (Character == nullptr) { AddError(FString::Printf(TEXT("%s: no character"), When)); return; }
		const FString Tag(When);
		TestTrue(*(Tag + TEXT(": position survives save/load (within 5 cm)")), FVector::Dist(Character->GetActorLocation(), S->SavedLocation) < 5.0);
		const FRotator View = Character->GetController()->GetControlRotation();
		TestTrue(*(Tag + TEXT(": yaw survives")), FMath::Abs(FRotator::NormalizeAxis(View.Yaw - S->SavedYaw)) < 0.5);
		TestTrue(*(Tag + TEXT(": pitch survives")), FMath::Abs(FRotator::NormalizeAxis(View.Pitch - S->SavedPitch)) < 0.5);
		TestEqual(*(Tag + TEXT(": gait restored")), static_cast<int32>(Character->GetSpnMovement()->GetBaseGait()), static_cast<int32>(ESpnGait::Walk));
		TestEqual(*(Tag + TEXT(": inventory (rations)")), Rations(), 2ull);
		FSpnMapMarkerView Waypoint;
		TestTrue(*(Tag + TEXT(": waypoint survives")), S->Instance->GetSubsystem<USpnMapSubsystem>()->GetWaypoint(Waypoint) && Waypoint.Id == S->SavedWaypointId);
		{
			FString Ids;
			for (const uint64 Id : S->Instance->GetSubsystem<USpnInventorySubsystem>()->ListContainers()) { Ids += FString::Printf(TEXT("%llu%s "), Id, SpnWorldContainerRegistry().Contains(static_cast<int64>(Id)) ? TEXT("(world)") : TEXT("")); }
			AddInfo(FString::Printf(TEXT("%s: containers: %s player=%llu"), When, *Ids, SpnWorld::FindPlayerContainer(*S->Instance->GetSubsystem<USpnInventorySubsystem>())));
		}
		AddInfo(FString::Printf(TEXT("%s: inventory: %s"), When, *S->Instance->GetSubsystem<USpnInventorySubsystem>()->Describe().Replace(LINE_TERMINATOR, TEXT(" | "))));
		AddInfo(FString::Printf(TEXT("%s: loc (%.1f, %.1f, %.1f) saved (%.1f, %.1f, %.1f)"), When, Character->GetActorLocation().X, Character->GetActorLocation().Y,
			Character->GetActorLocation().Z, S->SavedLocation.X, S->SavedLocation.Y, S->SavedLocation.Z));
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Verify]() { Verify(TEXT("in-place load")); return true; }));

	// 4. back to the main menu, change the state, then load from the menu: level travel, pose applied after the pawn exists
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]()
	{
		if (USpnSessionSubsystem* Sess = Session(S)) { Sess->ReturnToMainMenu(); } else { AddError(TEXT("no session")); }
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S, Inv]()
	{
		USpnSessionSubsystem* Sess = Session(S);
		if (Sess == nullptr || !Sess->IsMenuWorld())
		{
			if (++S->Waited > 1200) { AddError(TEXT("never reached the main menu level")); S->Waited = 0; return true; }
			return false;
		}
		S->Waited = 0;
		Inv()->AddItem(SpnWorld::FindPlayerContainer(*Inv()), TEXT("spn_test:item.ration"), 4);
		S->Instance->GetSubsystem<USpnMapSubsystem>()->ClearWaypoint();
		const FSpnSaveStatus Loaded = Sess->LoadGame(TEXT("pose_world"));
		TestTrue(TEXT("load from the main menu accepted"), Loaded.IsOk());
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, S]() { return WaitForGameplay(*this, S, TEXT("V3Interiors")); }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Verify]() { Verify(TEXT("load after the main menu")); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([S]()
	{
		IFileManager::Get().DeleteDirectory(*S->Root, false, true);
		return true;
	}));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
