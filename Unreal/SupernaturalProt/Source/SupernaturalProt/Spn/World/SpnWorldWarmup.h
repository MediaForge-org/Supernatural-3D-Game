#pragma once

#include "CoreMinimal.h"

class UWorld;

/**
 * Prepares what the player is about to see before the first frame after a level start, a loaded save or another jump of the view
 * (night build 2026-11-14, texture/vegetation pop-in in SpnWorld).
 *
 * Measured cause of the pop-in (Docs/Development/Streaming-PopIn-2026-11-14.md): the streaming pools are not short (textures need at most
 * ~400 of 1000 MB, the virtual-texture pools are almost empty, Nanite keeps full quality). What the player saw were the first ~1-2 s after a
 * jump of the view: landscape grass is created component by component around the view (one per frame) and the texture streamer starts from
 * the resident low mips. This runs once at such a jump: it builds the grass around the location synchronously and streams the textures the
 * view needs, with a time limit. It does not disable streaming and does not force all top mips.
 */
namespace SpnWorldWarmup
{
	struct FResult
	{
		bool bRan = false;
		double CompileSeconds = 0.0;   // editor builds only: waiting for meshes/textures/shaders still being prepared
		double GrassSeconds = 0.0;
		double StreamSeconds = 0.0;
		int32 PendingRequests = 0;   // streaming requests still in flight when the time limit ran out
	};

	/** Blocking: call only behind a load or a cut (level start, loaded save). Does nothing without rendering (headless tests, servers). */
	/** FadeSeconds > 0: the player camera fades in from black over that time afterwards (covers exposure/Lumen settling, see the .cpp). */
	SUPERNATURALPROT_API FResult WarmUpView(UWorld* World, const FVector& ViewLocation, float TimeLimitSeconds = 3.f, float FadeSeconds = 0.f);
}
