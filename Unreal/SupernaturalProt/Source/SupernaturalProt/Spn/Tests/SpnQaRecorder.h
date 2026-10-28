#pragma once

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "InputKeyEventArgs.h"
#include "Misc/App.h"
#include "Misc/AutomationTest.h"
#include "Misc/CommandLine.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "SupernaturalProtCharacter.h"
#include "UnrealClient.h"

#if WITH_DEV_AUTOMATION_TESTS

// Gameplay QA recorder (Tools/run_gameplay_qa.ps1, Docs/Development/Gameplay-QA.md).
// Every check of an SpnQA.* test is written as ONE JSON line to <QaDir>/records.jsonl at once (a crash or timeout keeps what was
// measured so far), shown as an overlay line in the game view (so the video shows which test is running) and, on a failure, logged
// with position, velocity and a screenshot. The PowerShell wrapper turns records.jsonl into results.json / report.md and maps the
// wall-clock `epoch` of each record to a video offset. Statuses: PASS, FAIL, BLOCKED, NOT_IMPLEMENTED, NOT_TESTED.

namespace SpnQa
{
	enum class EStatus : uint8 { Pass, Fail, Blocked, NotImplemented, NotTested };

	inline const TCHAR* ToString(EStatus Status)
	{
		switch (Status)
		{
		case EStatus::Pass: return TEXT("PASS");
		case EStatus::Fail: return TEXT("FAIL");
		case EStatus::Blocked: return TEXT("BLOCKED");
		case EStatus::NotImplemented: return TEXT("NOT_IMPLEMENTED");
		default: return TEXT("NOT_TESTED");
		}
	}

	/** Output directory of this run: -SpnQaOut=<dir> (set by the wrapper), else Saved/GameplayQA/adhoc. */
	inline FString QaDir()
	{
		FString Dir;
		if (!FParse::Value(FCommandLine::Get(), TEXT("SpnQaOut="), Dir) || Dir.IsEmpty())
		{
			Dir = FPaths::ProjectSavedDir() / TEXT("GameplayQA/adhoc");
		}
		Dir.ReplaceInline(TEXT("\""), TEXT(""));
		return FPaths::ConvertRelativePathToFull(Dir);
	}

	inline double NowEpoch() { return (FDateTime::UtcNow() - FDateTime(1970, 1, 1)).GetTotalSeconds(); }

	inline FString JsonEscape(const FString& In)
	{
		FString Out;
		for (const TCHAR C : In)
		{
			switch (C)
			{
			case '"': Out += TEXT("\\\""); break;
			case '\\': Out += TEXT("\\\\"); break;
			case '\n': Out += TEXT("\\n"); break;
			case '\r': break;
			case '\t': Out += TEXT(" "); break;
			default: Out.AppendChar(C); break;
			}
		}
		return Out;
	}

	/** Shows the current section (and the last results) over the game view; the key keeps one line per slot. */
	inline void Overlay(int32 Slot, const FString& Text, const FColor& Color, float Seconds = 6.f)
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(Slot, Seconds, Color, Text, /*bNewerOnTop*/ false, FVector2D(2.2f, 2.2f));
		}
	}

	inline void Section(const FString& Name)
	{
		UE_LOG(LogTemp, Display, TEXT("SPNQA SECTION %s"), *Name);
		Overlay(7001, FString::Printf(TEXT("QA: %s"), *Name), FColor::Cyan, 600.f);
	}

	inline void Screenshot(const FString& Id)
	{
		if (FApp::CanEverRender())
		{
			FString Safe = Id;
			for (TCHAR& C : Safe) { if (!FChar::IsAlnum(C) && C != '_' && C != '-') { C = '_'; } }
			FScreenshotRequest::RequestScreenshot(QaDir() / TEXT("shots") / Safe + TEXT(".png"), /*bShowUI*/ true, /*bAddFilenameSuffix*/ false);
		}
	}

	/**
	 * Appends one measurement trace (NOT a check, never a status) as a JSON line to <QaDir>/traces.jsonl:
	 * {"suite","id","epoch","columns":[...],"rows":[[...],...]} . tools/spn_trace_tools.py analyses it (mirrored turns, jump approach).
	 * Keep traces short (a few dozen rows around the event); this is not a per-frame log.
	 */
	inline void Trace(const FString& Suite, const FString& Id, const TArray<FString>& Columns, const TArray<TArray<double>>& Rows)
	{
		FString Cols;
		for (int32 i = 0; i < Columns.Num(); ++i) { Cols += (i > 0 ? TEXT(",\"") : TEXT("\"")) + JsonEscape(Columns[i]) + TEXT("\""); }
		FString Body;
		for (int32 r = 0; r < Rows.Num(); ++r)
		{
			Body += r > 0 ? TEXT(",[") : TEXT("[");
			for (int32 c = 0; c < Rows[r].Num(); ++c) { Body += FString::Printf(TEXT("%s%.3f"), c > 0 ? TEXT(",") : TEXT(""), Rows[r][c]); }
			Body += TEXT("]");
		}
		const FString Line = FString::Printf(TEXT("{\"suite\":\"%s\",\"id\":\"%s\",\"epoch\":%.3f,\"columns\":[%s],\"rows\":[%s]}\n"),
			*JsonEscape(Suite), *JsonEscape(Id), NowEpoch(), *Cols, *Body);
		IFileManager::Get().MakeDirectory(*QaDir(), true);
		FFileHelper::SaveStringToFile(Line, *(QaDir() / TEXT("traces.jsonl")), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM, &IFileManager::Get(), FILEWRITE_Append);
	}

	/**
	 * Records one check. FAIL and BLOCKED are errors of the automation test (so the exit code and the Unreal report show them);
	 * NOT_IMPLEMENTED and NOT_TESTED are warnings/infos and never count as PASS.
	 */
	inline void Check(FAutomationTestBase& Test, const ASupernaturalProtCharacter* Character, const FString& Suite, const FString& Id,
		EStatus Status, const FString& Detail)
	{
		const FVector Pos = Character != nullptr ? Character->GetActorLocation() : FVector::ZeroVector;
		const FVector Vel = Character != nullptr ? Character->GetVelocity() : FVector::ZeroVector;
		const double Epoch = NowEpoch();
		const UWorld* World = Character != nullptr ? Character->GetWorld() : nullptr;
		const FString Line = FString::Printf(
			TEXT("{\"suite\":\"%s\",\"id\":\"%s\",\"status\":\"%s\",\"detail\":\"%s\",\"epoch\":%.3f,\"game_time\":%.3f,"
				"\"pos\":[%.1f,%.1f,%.1f],\"vel\":[%.1f,%.1f,%.1f],\"speed2d\":%.1f}\n"),
			*JsonEscape(Suite), *JsonEscape(Id), ToString(Status), *JsonEscape(Detail), Epoch, World != nullptr ? World->GetTimeSeconds() : 0.0,
			Pos.X, Pos.Y, Pos.Z, Vel.X, Vel.Y, Vel.Z, Vel.Size2D());
		IFileManager::Get().MakeDirectory(*QaDir(), true);
		FFileHelper::SaveStringToFile(Line, *(QaDir() / TEXT("records.jsonl")), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM, &IFileManager::Get(), FILEWRITE_Append);

		const FString Message = FString::Printf(TEXT("[%s] %s: %s"), ToString(Status), *Id, *Detail);
		UE_LOG(LogTemp, Display, TEXT("SPNQA %s | pos (%.0f, %.0f, %.0f) vel (%.0f, %.0f, %.0f)"), *Message, Pos.X, Pos.Y, Pos.Z, Vel.X, Vel.Y, Vel.Z);
		Overlay(7002, Message, Status == EStatus::Pass ? FColor::Green : FColor::Red, 8.f);
		switch (Status)
		{
		case EStatus::Pass:
			Test.AddInfo(Message);
			break;
		case EStatus::Fail:
		case EStatus::Blocked:
			Test.AddError(Message);
			Screenshot(Suite + TEXT("_") + Id);
			break;
		default:
			Test.AddWarning(Message);
			break;
		}
	}

	/** Presses or releases a key through the player controller, the same entry point as a keyboard event of the viewport. */
	inline void Key(APlayerController& Controller, const FKey& Key, bool bDown)
	{
		Controller.InputKey(FInputKeyEventArgs::CreateSimulated(Key, bDown ? IE_Pressed : IE_Released, bDown ? 1.f : 0.f));
	}

	/** Keys a scenario holds; only sends the edge, ReleaseAll leaves nothing pressed behind. */
	struct FHeldKeys
	{
		TSet<FKey> Down;

		void Set(ACharacter& Character, const FKey& Key, bool bDown)
		{
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			if (PC != nullptr && bDown != Down.Contains(Key))
			{
				SpnQa::Key(*PC, Key, bDown);
				if (bDown) { Down.Add(Key); } else { Down.Remove(Key); }
			}
		}

		void ReleaseAll(ACharacter& Character)
		{
			const TSet<FKey> Copy = Down;
			for (const FKey& K : Copy) { Set(Character, K, false); }
		}
	};

	inline void FaceYaw(ACharacter& Character, float Yaw)
	{
		Character.GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));
		Character.SetActorRotation(FRotator(0.f, Yaw, 0.f));
	}
}

#endif
