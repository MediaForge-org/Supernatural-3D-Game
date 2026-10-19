#pragma once

#include "CoreMinimal.h"

namespace SpnCore
{
	/**
	 * PLACEHOLDER integration check, not gameplay code: creates a fresh
	 * spn::core::WorldFacts store, sets one test fact, reads it back and logs
	 * the result to LogSpnCore. Returns true if the value read back matches.
	 */
	SPNCORE_API bool RunWorldFactsSmokeTest();
}
