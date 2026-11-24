#pragma once

#include "CoreMinimal.h"

class UObject;

// Status notifications of the HUD (Night Build 01). One entry point so gameplay code does not depend on the HUD classes.
namespace SpnUi
{
	enum class ENote : uint8 { Info, Good, Warning, Error };

	/** Shows a short message on the local player's HUD (and logs it). Safe to call without a HUD (menu level, tests): then it only logs. */
	SUPERNATURALPROT_API void Notify(const UObject* WorldContext, const FText& Text, ENote Kind = ENote::Info);
}

namespace SpnUi
{
	/** Hands the interaction prompt to the HUD. False if there is no HUD (menu level, flow disabled): the caller then draws its own prompt. */
	SUPERNATURALPROT_API bool SetPrompt(const UObject* WorldContext, const FString& Text);
	/** Objective line under the minimap (technical prototype of the SpnWorld route guide); empty text hides it. False without a HUD. */
	SUPERNATURALPROT_API bool SetObjective(const UObject* WorldContext, const FText& Text);
}
