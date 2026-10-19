#include "SpnWorldFactsSmokeTest.h"
#include "SpnCoreModule.h"

#include <cstdint>
#include <spn/core/WorldFacts.h>
#include <string_view>

namespace
{
	FString ToFString(std::string_view Text)
	{
		return FString::ConstructFromPtrSize(reinterpret_cast<const UTF8CHAR*>(Text.data()), static_cast<int32>(Text.size()));
	}
}

bool SpnCore::RunWorldFactsSmokeTest()
{
	constexpr std::string_view Key = "placeholder.integration.smoke_test";
	constexpr std::int64_t Expected = 42;

	spn::core::WorldFacts Facts;
	const auto Applied = Facts.Apply(spn::core::SetFact{std::string(Key), Expected}, "unreal.integration.smoke_test");
	if (!Applied)
	{
		UE_LOG(LogSpnCore, Error, TEXT("WorldFacts smoke test FAILED: Apply returned %s"),
			*ToFString(spn::core::ToString(Applied.Error().code)));
		return false;
	}

	const std::int64_t* Value = Facts.FindAs<std::int64_t>(Key);
	if (Value == nullptr || *Value != Expected)
	{
		UE_LOG(LogSpnCore, Error, TEXT("WorldFacts smoke test FAILED: fact '%s' missing or wrong value"), *ToFString(Key));
		return false;
	}

	UE_LOG(LogSpnCore, Log, TEXT("WorldFacts smoke test OK: %s = %lld (revision %llu, journal sequence %llu)"),
		*ToFString(Key), static_cast<long long>(*Value),
		static_cast<unsigned long long>(Facts.Revision()),
		static_cast<unsigned long long>(Applied.Value().sequence));
	return true;
}
