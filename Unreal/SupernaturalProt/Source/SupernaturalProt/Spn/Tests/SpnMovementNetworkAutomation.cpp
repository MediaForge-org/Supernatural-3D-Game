// Enabled on Windows (UE 5.8.4, 2026-10-26): compiled unchanged from the prepared text file. It was unstable (about 37% failures)
// because a server correction replayed older saved moves and left their sprint request behind (fixed in
// USpnCharacterMovementComponent::ClientUpdatePositionAfterServerUpdate; 20/20 afterwards). See Docs/Setup/Windows-M2A-Handover.md.
// Loopback only: no packet lag or loss, one client.
// Originally written on Fedora without Unreal Engine (Proposal 0011).
//
// Automated client/server sprint check: a listen server plus one client in one editor process (PIE), the client sprints
// on the speed lane (x = -125 m) for 5 s, then turns 90 degrees at full speed for 1.5 s; the server's copy of the client's pawn is
// compared with the client's own throughout (position error, speed kept).
//
// To enable on Windows (see Docs/Setup/Windows-M2A-Handover.md, step 13):
//   1. Copy this file to Source/SupernaturalProt/Spn/Tests/SpnMovementNetworkAutomation.cpp (drop ".txt").
//   2. In SupernaturalProt.Build.cs add, for editor builds only:
//        if (Target.bBuildEditor) { PrivateDependencyModuleNames.Add("UnrealEd"); }
//   3. Build the editor target, run:  Automation RunTests SpnGame.Movement.NetworkSprint
// Expect to fix API details (the PIE session API changes between engine versions). Until then use the manual procedure
// in the handover document (p.NetShowCorrections, NetEmulation).

#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS && WITH_EDITOR

// editor-only headers inside the guard (NightBuild06): the packaged game target has no Editor.h
#include "Editor.h"
#include "Settings/LevelEditorPlaySettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementNetworkSprintTest, "SpnGame.Movement.NetworkSprint",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSpnMovementNetworkSprintTest::RunTest(const FString& Parameters)
{
	using namespace SpnMovementTest;
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());

	struct FNet
	{
		ASupernaturalProtCharacter* ClientPawn = nullptr;
		ASupernaturalProtCharacter* ServerCopy = nullptr;
		double Start = -1.0;
		float WorstError = 0.f;
		float ClientSpeed = 0.f;
		float ServerSpeed = 0.f;
		FString Trace;
		int32 Frame = 0;
	};
	const TSharedRef<FNet> Net = MakeShared<FNet>();

	// One listen server and one client in this process.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		ULevelEditorPlaySettings* Settings = GetMutableDefault<ULevelEditorPlaySettings>(); // in memory only, never saved
		Settings->SetPlayNetMode(EPlayNetMode::PIE_ListenServer);
		Settings->SetPlayNumberOfClients(2);
		Settings->SetRunUnderOneProcess(true);
		FRequestPlaySessionParams Params;
		Params.WorldType = EPlaySessionWorldType::PlayInEditor;
		GEditor->RequestPlaySession(Params);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(6.f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Net]
	{
		UWorld* ServerWorld = nullptr;
		UWorld* ClientWorld = nullptr;
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType != EWorldType::PIE || Context.World() == nullptr)
			{
				continue;
			}
			(Context.World()->GetNetMode() == NM_Client ? ClientWorld : ServerWorld) = Context.World();
		}
		if (!TestNotNull(TEXT("server world"), ServerWorld) || !TestNotNull(TEXT("client world"), ClientWorld))
		{
			return true;
		}
		const APlayerController* ClientController = ClientWorld->GetFirstPlayerController();
		Net->ClientPawn = ClientController != nullptr ? Cast<ASupernaturalProtCharacter>(ClientController->GetPawn()) : nullptr;
		for (FConstPlayerControllerIterator It = ServerWorld->GetPlayerControllerIterator(); It; ++It)
		{
			if (It->IsValid() && !(*It)->IsLocalController()) // the remote player's controller on the server
			{
				Net->ServerCopy = Cast<ASupernaturalProtCharacter>((*It)->GetPawn());
			}
		}
		if (Net->ClientPawn != nullptr && Net->ServerCopy != nullptr)
		{
			AddInfo(FString::Printf(TEXT("DIAG pawns: client %s world %s role %d local %d ctrl %s; server copy %s world %s role %d local %d; same object %d"),
				*Net->ClientPawn->GetName(), *Net->ClientPawn->GetWorld()->GetName(), static_cast<int32>(Net->ClientPawn->GetLocalRole()), Net->ClientPawn->IsLocallyControlled() ? 1 : 0, *GetNameSafe(Net->ClientPawn->GetController()),
				*Net->ServerCopy->GetName(), *Net->ServerCopy->GetWorld()->GetName(), static_cast<int32>(Net->ServerCopy->GetLocalRole()), Net->ServerCopy->IsLocallyControlled() ? 1 : 0, Net->ClientPawn == Net->ServerCopy ? 1 : 0));
		}
		TestNotNull(TEXT("client pawn"), Net->ClientPawn);
		TestNotNull(TEXT("server copy of the client pawn"), Net->ServerCopy);
		return true;
	}));

	// Teleporting is done on the server (authority); then the client sprints for 5 s.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Net]
	{
		if (Net->ServerCopy != nullptr && Net->ClientPawn != nullptr)
		{
			// The speed lane of Lvl_SpnMovementTest (free strip along +Y, Tools/spn_movement_map_layout.py).
			Net->ServerCopy->SetActorLocation(FVector(-12500.f, -12000.f, StandZ), false, nullptr, ETeleportType::TeleportPhysics);
			Net->ClientPawn->GetController()->SetControlRotation(FRotator(0.f, 90.f, 0.f));
			Net->ClientPawn->DoSprintStart();
			AddInfo(FString::Printf(TEXT("DIAG after DoSprintStart: requested %d, world time %.2f"), Net->ClientPawn->GetSpnMovement()->IsSprintRequested() ? 1 : 0, Net->ClientPawn->GetWorld()->GetTimeSeconds()));
		}
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Net]
	{
		if (Net->ClientPawn == nullptr || Net->ServerCopy == nullptr)
		{
			return true;
		}
		const double Now = Net->ClientPawn->GetWorld()->GetTimeSeconds();
		if (Net->Start < 0.0)
		{
			Net->Start = Now;
		}
		// After 5 s of straight sprint the input turns 90 degrees to the side for 1.5 s: the sprint is kept (sustain angle), and the
		// server must agree with the client about it (the velocity is predicted state, so the same rule gives the same answer).
		const bool bTurning = Now - Net->Start > 5.0;
		Net->ClientPawn->DoMove(bTurning ? 1.f : 0.f, bTurning ? 0.f : 1.f);
		Net->WorstError = FMath::Max(Net->WorstError, static_cast<float>(FVector::Dist2D(Net->ClientPawn->GetActorLocation(), Net->ServerCopy->GetActorLocation())));
		Net->ClientSpeed = Net->ClientPawn->GetVelocity().Size2D();
		Net->ServerSpeed = Net->ServerCopy->GetVelocity().Size2D();
		if (++Net->Frame % 15 == 0)
		{
			Net->Trace += FString::Printf(TEXT("[t=%.2f c=%.0f/%d r%d s=%.0f/%d r%d] "), Now - Net->Start, Net->ClientSpeed, static_cast<int32>(Net->ClientPawn->GetSpnMovement()->GetEffectiveGait()), Net->ClientPawn->GetSpnMovement()->IsSprintRequested() ? 1 : 0, Net->ServerSpeed, static_cast<int32>(Net->ServerCopy->GetSpnMovement()->GetEffectiveGait()), Net->ServerCopy->GetSpnMovement()->IsSprintRequested() ? 1 : 0);
		}
		return Now - Net->Start > 6.5;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Net]
	{
		AddInfo(TEXT("MEASURED trace (time, client speed/gait, server speed/gait): ") + Net->Trace);
		// Expected: both sides reach the sprint speed; the pawns never drift far apart (smoothing/latency allowance).
		TestTrue(FString::Printf(TEXT("client keeps the sprint through the turn (%.0f cm/s)"), Net->ClientSpeed), Net->ClientSpeed > 700.f);
		TestTrue(FString::Printf(TEXT("server copy keeps the sprint through the turn (%.0f cm/s)"), Net->ServerSpeed), Net->ServerSpeed > 700.f);
		TestTrue(FString::Printf(TEXT("client and server positions stay within 150 cm (worst %.0f)"), Net->WorstError), Net->WorstError < 150.f);
		GEditor->RequestEndPlayMap();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	return true;
}

#endif
