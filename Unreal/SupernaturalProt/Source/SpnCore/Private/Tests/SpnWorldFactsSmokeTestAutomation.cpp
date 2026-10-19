#include "Misc/AutomationTest.h"
#include "SpnWorldFactsSmokeTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldFactsSmokeTest, "SpnCore.WorldFacts.SetAndRead",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnWorldFactsSmokeTest::RunTest(const FString& Parameters)
{
	return TestTrue(TEXT("WorldFacts set/read through the SPN core"), SpnCore::RunWorldFactsSmokeTest());
}

#endif
