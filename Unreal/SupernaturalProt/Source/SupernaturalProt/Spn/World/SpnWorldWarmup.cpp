#include "Spn/World/SpnWorldWarmup.h"

#include "ContentStreaming.h"
#if WITH_EDITOR
#include "AssetCompilingManager.h"
#include "ShaderCompiler.h"
#endif
#include "Camera/PlayerCameraManager.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealClient.h"
#include "LandscapeSubsystem.h"
#include "Misc/App.h"
#include "SupernaturalProt.h"

namespace SpnWorldWarmup
{
	FResult WarmUpView(UWorld* World, const FVector& ViewLocation, float TimeLimitSeconds, float FadeSeconds)
	{
		FResult Result;
		if (World == nullptr || !World->IsGameWorld() || !FApp::CanEverRender() || World->GetNetMode() == NM_DedicatedServer)
		{
			return Result;
		}
		Result.bRan = true;
#if WITH_EDITOR
		// editor builds (UnrealEditor.exe -game, Tools\play_spnworld.ps1) prepare meshes, textures and shaders from the derived data cache while
		// the level starts; until they are done the first frames show placeholders. A packaged game has them cooked.
		const double CompileStart = FPlatformTime::Seconds();
		FAssetCompilingManager::Get().FinishAllCompilation();
		if (GShaderCompilingManager != nullptr)
		{
			GShaderCompilingManager->FinishAllCompilation();
		}
		Result.CompileSeconds = FPlatformTime::Seconds() - CompileStart;
#endif
		IStreamingManager& Streaming = IStreamingManager::Get();
		// The view is not rendered yet: tell the streamer where it will be and how large it is on screen (kept for a few seconds, the first
		// frames then take over). AddViewLocation alone would take the screen size of earlier views, and at a level start there are none.
		float ScreenWidth = 1920.f;
		float FovDeg = 90.f;
		if (GEngine != nullptr && GEngine->GameViewport != nullptr && GEngine->GameViewport->Viewport != nullptr)
		{
			ScreenWidth = FMath::Max(640.f, static_cast<float>(GEngine->GameViewport->Viewport->GetSizeXY().X));
		}
		if (const APlayerController* PC = World->GetFirstPlayerController(); PC != nullptr && PC->PlayerCameraManager != nullptr)
		{
			FovDeg = FMath::Clamp(PC->PlayerCameraManager->GetFOVAngle(), 30.f, 120.f);
		}
		const float FovScreenSize = ScreenWidth / FMath::Tan(FMath::DegreesToRadians(FovDeg * 0.5f));
		Streaming.AddViewInformation(ViewLocation, ScreenWidth, FovScreenSize, 1.f, false, 4.f, nullptr, World);

		const double GrassStart = FPlatformTime::Seconds();
		if (ULandscapeSubsystem* Landscape = World->GetSubsystem<ULandscapeSubsystem>())
		{
			FVector Cameras[] = {ViewLocation};
			Landscape->RegenerateGrass(/*bInFlushGrass*/ false, /*bInForceSync*/ true, TArrayView<FVector>(Cameras, 1));
		}
		Result.GrassSeconds = FPlatformTime::Seconds() - GrassStart;

		const double StreamStart = FPlatformTime::Seconds();
		Result.PendingRequests = Streaming.StreamAllResources(FMath::Max(0.1f, TimeLimitSeconds));
		Result.StreamSeconds = FPlatformTime::Seconds() - StreamStart;
		// What stays after the streaming work is not streaming: right after a level start the auto exposure, Lumen and the TSR history settle for
		// about 1-1.5 s (A/B measured: fully loaded textures do not change those frames). A short fade from black covers it, as a load screen would.
		if (APlayerController* PC = World->GetFirstPlayerController(); PC != nullptr && PC->PlayerCameraManager != nullptr && FadeSeconds > 0.f)
		{
			PC->PlayerCameraManager->StartCameraFade(1.f, 0.f, FadeSeconds, FLinearColor::Black, /*bShouldFadeAudio*/ false, /*bHoldWhenFinished*/ false);
		}
		UE_LOG(LogSupernaturalProt, Display, TEXT("WorldWarmup: view (%.0f, %.0f, %.0f): editor compile %.2f s, grass %.2f s, streaming %.2f s, %d requests still pending"),
			ViewLocation.X, ViewLocation.Y, ViewLocation.Z, Result.CompileSeconds, Result.GrassSeconds, Result.StreamSeconds, Result.PendingRequests);
		return Result;
	}
}
