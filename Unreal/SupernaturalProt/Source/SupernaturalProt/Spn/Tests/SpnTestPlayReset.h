#pragma once

#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// Automation runs several PIE sessions in one editor process and they share one game instance, so the world state of
// one play test is still there in the next one (seen 2026-10-22: doors left open by SpnGame.TestMap.PlayDoors).
// Real PIE restarts and new game processes start fresh. Play tests call this first, so they do not depend on the order.

namespace SpnTestPlay
{
	/** Replaces the live world state by an empty one through the normal load path (a slot holding an empty WorldFacts v1 file). */
	inline bool ResetWorldState(APlayerController& Controller)
	{
		const FString Blank = TEXT("spn_automation_blank_state");
		const FString File = USpnWorldStateSubsystem::GetSlotFilePath(Blank);
		const bool bWritten = FFileHelper::SaveStringToFile(TEXT("SPNFACTS 1\nend\t0\n"), *File, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		Controller.ConsoleCommand(FString::Printf(TEXT("Spn.LoadWorld %s"), *Blank), /*bWriteToLog*/ true);
		IFileManager::Get().DeleteDirectory(*FPaths::GetPath(File), false, true);
		return bWritten;
	}
}

#endif
