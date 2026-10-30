#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnMapSubsystem.h"
#include "SpnPlayerPose.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Package.h"

#include <spn/core/SaveSlot.h>

#if WITH_DEV_AUTOMATION_TESTS

// Save completion: the player pose (section player.pose). Subsystem tests without a level; the real-world round trip (character, level, main
// menu) is SpnGame.NightBuild.PoseWorld in the game module. Each test works in its own temporary save root.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Level = TEXT("/Game/SpnTest/Lvl_PoseTest");

	struct FEnv
	{
		UGameInstance* Instance = nullptr;
		FString Root;

		FEnv()
		{
			Instance = NewObject<UGameInstance>(GetTransientPackage());
			Instance->AddToRoot();
			Instance->Init();
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Save().SetSaveDirectory(Root);
			Map().InstallTestDefinition();
		}

		~FEnv()
		{
			Instance->Shutdown();
			Instance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*Root, false, true);
		}

		USpnMapSubsystem& Map() const { return *Instance->GetSubsystem<USpnMapSubsystem>(); }
		USpnSaveSubsystem& Save() const { return *Instance->GetSubsystem<USpnSaveSubsystem>(); }
		USpnWorldStateSubsystem& World() const { return *Instance->GetSubsystem<USpnWorldStateSubsystem>(); }
	};

	FSpnPlayerPose MakePose(const FVector& Cm, double Yaw, double Pitch, bool bCrouched, ESpnWorldAxes Axes = ESpnWorldAxes::EastSouthUp)
	{
		FSpnWorldFrame Frame;
		Frame.Axes = Axes;
		FSpnPlayerPose Pose;
		FSpnPlayerPose::FromUnreal(Level, Frame, Cm, Yaw, Pitch, TEXT("run"), bCrouched, Pose);
		return Pose;
	}

	/** Rewrites the slot's main file with a modified player.pose section (valid checksums: only the CONTENT is bad). */
	bool RewritePose(const FString& File, const std::string& Payload, uint32 Version)
	{
		TArray<uint8> Bytes;
		if (!FFileHelper::LoadFileToArray(Bytes, *File))
		{
			return false;
		}
		auto Slot = spn::core::SaveSlot::Parse(std::string_view(reinterpret_cast<const char*>(Bytes.GetData()), static_cast<size_t>(Bytes.Num())));
		if (!Slot)
		{
			return false;
		}
		spn::core::SaveSection Section;
		Section.id = "player.pose";
		Section.version = Version;
		Section.required = false;
		Section.payload = Payload;
		if (Slot.Value().Put(std::move(Section)))
		{
			return false;
		}
		const std::string Out = Slot.Value().Serialize();
		TArray<uint8> OutBytes;
		OutBytes.Append(reinterpret_cast<const uint8*>(Out.data()), static_cast<int32>(Out.size()));
		return FFileHelper::SaveArrayToFile(OutBytes, *File);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnPlayerPoseFormatTest, "SpnCore.Save.PlayerPoseFormat", Flags)
bool FSpnPlayerPoseFormatTest::RunTest(const FString&)
{
	for (const ESpnWorldAxes Axes : {ESpnWorldAxes::EastSouthUp, ESpnWorldAxes::NorthEastUp})
	{
		const FVector Cm(-10550.123456789, -4700.987654321, 187.5);
		const FSpnPlayerPose Pose = MakePose(Cm, -133.25, -12.5, true, Axes);
		FSpnPlayerPose Back;
		FString Error;
		TestTrue(TEXT("a serialized pose parses"), FSpnPlayerPose::Parse(Pose.Serialize(), Back, Error));
		TestEqual(TEXT("level"), Back.Level, Pose.Level);
		TestEqual(TEXT("gait"), Back.Gait, FString(TEXT("run")));
		TestTrue(TEXT("crouched"), Back.bCrouched);
		TestEqual(TEXT("pitch"), Back.PitchDeg, -12.5, 1e-12);
		FVector BackCm;
		double BackYaw = 0.0;
		TestTrue(TEXT("converts back"), Back.ToUnreal(BackCm, BackYaw));
		TestTrue(TEXT("the position survives the round trip (canonical metres, exact)"), BackCm.Equals(Cm, 1e-6));
		TestEqual(TEXT("the heading survives"), BackYaw, -133.25, 1e-9);
		TestEqual(TEXT("serialising again gives the same bytes"), Back.Serialize(), Pose.Serialize());
	}

	// the canonical position is metres east/north (not raw Unreal centimetres): (1000 cm, -2000 cm) with EastSouthUp is 10 m east, 20 m north
	const FSpnPlayerPose Canonical = MakePose(FVector(1000.0, -2000.0, 100.0), 0.0, 0.0, false);
	TestTrue(TEXT("x is east in metres"), FMath::IsNearlyEqual(Canonical.WorldMetres.X, 10.0, 1e-9));
	TestTrue(TEXT("y is north in metres"), FMath::IsNearlyEqual(Canonical.WorldMetres.Y, 20.0, 1e-9));

	// strictness: nothing half-valid is accepted and Out stays untouched
	const FString Good = Canonical.Serialize();
	const TArray<TPair<FString, FString>> Bad = {
		{TEXT("SPN-PLAYERPOSE 1"), TEXT("SPN-PLAYERPOSE 2")},
		{TEXT("axes east_south_up"), TEXT("axes sideways")},
		{TEXT("pos_m 10 20 1"), TEXT("pos_m 10 20")},
		{TEXT("pos_m 10 20 1"), TEXT("pos_m 10 nan 1")},
		{TEXT("pos_m 10 20 1"), TEXT("pos_m 10 20 1e999")},
		{TEXT("crouched 0"), TEXT("crouched 2")},
		{TEXT("pitch_deg 0"), TEXT("pitch_deg 120")},
		{TEXT("gait run"), TEXT("gait RUN!")},
		{TEXT("level /Game/SpnTest/Lvl_PoseTest"), TEXT("level ../escape")},
		{TEXT("\n"), TEXT("\r\n")},
	};
	FSpnPlayerPose Untouched;
	Untouched.Level = TEXT("/Game/Untouched");
	for (const TPair<FString, FString>& Case : Bad)
	{
		FString Mutated = Good;
		if (Case.Key == TEXT("\n"))
		{
			Mutated = Good.Replace(TEXT("\n"), TEXT("\r\n"));
		}
		else
		{
			TestTrue(*FString::Printf(TEXT("test case applies: %s"), *Case.Key), Mutated.Contains(Case.Key));
			Mutated = Mutated.Replace(*Case.Key, *Case.Value);
		}
		FString Error;
		TestFalse(*FString::Printf(TEXT("rejected: %s"), *Case.Value), FSpnPlayerPose::Parse(Mutated, Untouched, Error));
		TestFalse(TEXT("with a reason"), Error.IsEmpty());
		TestEqual(TEXT("and Out is untouched"), Untouched.Level, FString(TEXT("/Game/Untouched")));
	}
	FString Error;
	TestFalse(TEXT("truncated"), FSpnPlayerPose::Parse(Good.Left(Good.Len() - 12), Untouched, Error));
	TestFalse(TEXT("empty"), FSpnPlayerPose::Parse(FString(), Untouched, Error));
	FSpnPlayerPose Out;
	TestFalse(TEXT("an unrepresentable position cannot be built"), FSpnPlayerPose::FromUnreal(Level, FSpnWorldFrame(), FVector(NAN, 0, 0), 0, 0, TEXT("jog"), false, Out));
	TestFalse(TEXT("a bad level cannot be built"), FSpnPlayerPose::FromUnreal(TEXT("no spaces please"), FSpnWorldFrame(), FVector::ZeroVector, 0, 0, TEXT("jog"), false, Out));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnPlayerPoseSaveTest, "SpnCore.Save.PlayerPose", Flags)
bool FSpnPlayerPoseSaveTest::RunTest(const FString&)
{
	FEnv Env;
	USpnSaveSubsystem& Save = Env.Save();
	int64 Id = 0;
	Env.Map().SetWaypointAt(FVector2D(12.5, -33.25), TEXT("Pose waypoint"), Id);
	Env.World().SetFlag(TEXT("pose.marker"), true, TEXT("test"));

	const FSpnPlayerPose Live = MakePose(FVector(-10550.0, -4700.0, 100.0), 77.5, -20.0, false);
	Save.PoseProvider = [&Live](FSpnPlayerPose& Out) { Out = Live; return true; };
	const FSpnSaveStatus Saved = Save.SaveSlot(TEXT("slot_pose"));
	TestTrue(TEXT("saved"), Saved.IsOk());
	TestTrue(TEXT("the pose was written"), Saved.bPoseInSlot && Saved.Pose.IsSet());
	const USpnSaveSubsystem::FSpnSlotInfo Info = Save.InspectSlot(TEXT("slot_pose"));
	TestTrue(TEXT("inspect sees the pose"), Info.bHasPose && Info.PoseLevel == Level);

	// the world moves on; the load brings position, rotation, waypoint and facts back and tells the listener exactly once
	Env.Map().ClearWaypoint();
	Env.World().SetFlag(TEXT("pose.marker"), false, TEXT("test"));
	TArray<FSpnPlayerPose> Received;
	Save.OnPoseLoaded.AddLambda([&Received](const FSpnPlayerPose& Pose) { Received.Add(Pose); });
	const FSpnSaveStatus Loaded = Save.LoadSlot(TEXT("slot_pose"));
	TestTrue(TEXT("loaded"), Loaded.IsOk());
	TestTrue(TEXT("the pose is reported"), Loaded.bPoseInSlot);
	if (TestEqual(TEXT("one pose notification"), Received.Num(), 1))
	{
		FVector Cm;
		double Yaw = 0.0;
		TestTrue(TEXT("convertible"), Received[0].ToUnreal(Cm, Yaw));
		TestTrue(TEXT("position survives save/load"), Cm.Equals(FVector(-10550.0, -4700.0, 100.0), 1e-6));
		TestEqual(TEXT("rotation (yaw) survives save/load"), Yaw, 77.5, 1e-9);
		TestEqual(TEXT("rotation (pitch) survives save/load"), Received[0].PitchDeg, -20.0, 1e-12);
		TestEqual(TEXT("movement state: gait"), Received[0].Gait, FString(TEXT("run")));
		TestEqual(TEXT("level"), Received[0].Level, FString(Level));
	}
	FSpnMapMarkerView Waypoint;
	TestTrue(TEXT("the waypoint survives"), Env.Map().GetWaypoint(Waypoint) && Waypoint.Label == TEXT("Pose waypoint"));
	TestTrue(TEXT("the facts survive"), Env.World().ReadFlag(TEXT("pose.marker")).Get(false));

	// saving again without a live player drops the old pose: a stale position never comes back
	Save.PoseProvider = [](FSpnPlayerPose&) { return false; };
	TestTrue(TEXT("saved without a player"), Save.SaveSlot(TEXT("slot_pose")).IsOk());
	TestFalse(TEXT("the old pose is gone from the slot"), Save.InspectSlot(TEXT("slot_pose")).bHasPose);

	// legacy slot (no pose at all): loads completely, reports it, and nobody is told to move the player
	Received.Reset();
	const FSpnSaveStatus Legacy = Save.LoadSlot(TEXT("slot_pose"));
	TestTrue(TEXT("legacy slot loads"), Legacy.IsOk());
	TestFalse(TEXT("and reports that it has no pose"), Legacy.bPoseInSlot);
	TestEqual(TEXT("no placement is requested"), Received.Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnPlayerPoseCorruptTest, "SpnCore.Save.PlayerPoseCorruptIsAtomic", Flags)
bool FSpnPlayerPoseCorruptTest::RunTest(const FString&)
{
	FEnv Env;
	USpnSaveSubsystem& Save = Env.Save();
	int64 Id = 0;
	Env.Map().SetWaypointAt(FVector2D(5, 5), TEXT("Saved"), Id);
	Env.World().SetFlag(TEXT("pose.marker"), true, TEXT("test"));
	const FSpnPlayerPose Live = MakePose(FVector(100.0, 200.0, 100.0), 10.0, 0.0, false);
	Save.PoseProvider = [&Live](FSpnPlayerPose& Out) { Out = Live; return true; };
	TestTrue(TEXT("saved"), Save.SaveSlot(TEXT("slot_bad")).IsOk());

	// the live state differs from the save, so a (wrong) partial load would be visible
	Env.Map().SetWaypointAt(FVector2D(-70, -70), TEXT("Live"), Id);
	Env.World().SetFlag(TEXT("pose.marker"), false, TEXT("test"));
	const uint64 Revision = Env.World().GetRevision();
	int32 Notifications = 0;
	Save.OnPoseLoaded.AddLambda([&Notifications](const FSpnPlayerPose&) { ++Notifications; });
	Save.OnLoaded.AddLambda([&Notifications]() { ++Notifications; });

	// valid checksums, bad content: the Core accepts the container, the pose parser must stop the whole load
	TestTrue(TEXT("rewrite with a broken pose"), RewritePose(Save.GetSlotFilePath(TEXT("slot_bad")), "SPN-PLAYERPOSE 1\nlevel /x\n", 1));
	const FSpnSaveStatus Bad = Save.LoadSlot(TEXT("slot_bad"));
	TestFalse(TEXT("the load fails"), Bad.IsOk());
	TestEqual(TEXT("as rejected content"), Bad.Result, ESpnSaveResult::Rejected);
	TestEqual(TEXT("facts untouched (revision)"), Env.World().GetRevision(), Revision);
	TestFalse(TEXT("facts untouched (value)"), Env.World().ReadFlag(TEXT("pose.marker")).Get(true));
	FSpnMapMarkerView Waypoint;
	TestTrue(TEXT("map untouched: the live waypoint is still there"), Env.Map().GetWaypoint(Waypoint) && Waypoint.Label == TEXT("Live"));
	TestEqual(TEXT("nobody was notified"), Notifications, 0);
	TestFalse(TEXT("inspect does not call the slot loadable"), Save.InspectSlot(TEXT("slot_bad")).Result == ESpnSaveResult::Ok);

	// a pose written by a newer build is never ignored and never overwritten
	TestTrue(TEXT("rewrite with a newer pose version"), RewritePose(Save.GetSlotFilePath(TEXT("slot_bad")), std::string(TCHAR_TO_UTF8(*Live.Serialize())), 2));
	TestEqual(TEXT("newer pose: refused"), Save.LoadSlot(TEXT("slot_bad")).Result, ESpnSaveResult::UnsupportedVersion);
	TestEqual(TEXT("and not overwritten by a save"), Save.SaveSlot(TEXT("slot_bad")).Result, ESpnSaveResult::UnsupportedVersion);
	TestEqual(TEXT("still nothing changed"), Env.World().GetRevision(), Revision);

	// the previous generation is the fallback when only the newest file has the broken pose
	TestTrue(TEXT("good save into another slot"), Save.SaveSlot(TEXT("slot_fb")).IsOk());
	TestTrue(TEXT("and a second generation"), Save.SaveSlot(TEXT("slot_fb")).IsOk());
	TestTrue(TEXT("break the newest"), RewritePose(Save.GetSlotFilePath(TEXT("slot_fb")), "garbage\n", 1));
	const FSpnSaveStatus Fallback = Save.LoadSlot(TEXT("slot_fb"));
	TestTrue(TEXT("falls back to the previous generation"), Fallback.IsOk() && Fallback.Source == ESpnSaveSource::Previous);
	TestTrue(TEXT("with its valid pose"), Fallback.bPoseInSlot);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
