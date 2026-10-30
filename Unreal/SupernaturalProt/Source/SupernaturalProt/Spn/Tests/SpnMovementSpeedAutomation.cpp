#include "Misc/App.h"
#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// M2A4.2 speed audit: does diagonal input or a steeper staircase make the character faster? Every case is a real run in
// /Game/SpnTest/Lvl_SpnMovementTest. The speed is measured over a window defined by the DISTANCE travelled along the movement
// direction (not by time), so a run that is delayed by step-ups is not hidden. Reported per case ("SPEEDAUDIT:" info lines):
//   horizontal speed  = window distance along the movement direction / window time   (what a player perceives on the map)
//   3D speed          = 3D displacement between window start and end / window time
//   path speed        = sum of the 3D frame deltas inside the window / window time    (includes vertical wiggle of step-ups)
//   input magnitude   = |character acceleration| / max acceleration at its peak (must never exceed 1)
// The levels of the layout come from Tools/spn_movement_map_layout.py (same constants as SpnMovementMapAutomation.cpp).

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags SpeedFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	constexpr float StairStartX = -9000.f;
	constexpr float StairFirstY = 4000.f;
	constexpr float StairSpacing = 500.f;
	constexpr int32 StairSteps = 10;
	constexpr float RampStartX = -9000.f;
	constexpr float RampSpacing = 700.f;
	const float StairRises[] = {15.f, 18.f, 22.f, 25.f};
	const float StairRuns[] = {35.f, 30.f, 28.f, 27.f};
	const float RampAngles[] = {5.f, 10.f, 15.f, 20.f, 30.f, 40.f, 50.f};

	struct FResult
	{
		FString Name;
		bool bDone = false;
		float HorizontalSpeed = 0.f;   // cm/s along the movement direction
		float Speed3D = 0.f;
		float PathSpeed = 0.f;
		float MaxInput = 0.f;
		float WindowSeconds = 0.f;
		float MeanDelta = 0.f;
		float ZGain = 0.f;
		int32 ZReversals = 0;      // frames in which the vertical movement changes direction (up -> down or down -> up)
		float MaxFrameDz = 0.f;    // biggest vertical jump of the capsule in one frame, cm
		FString Mode;
	};

	struct FSpec
	{
		FString Name;
		FVector Start;
		float Yaw = 0.f;
		float Right = 0.f, Forward = 1.f;
		ESpnGait Gait = ESpnGait::Jog;
		bool bSprint = false;
		float S0 = 600.f, S1 = 2100.f;   // window along the movement direction, cm from the start
		float Fps = 0.f;                 // 0 = the engine frame time of the run
		bool bPreVelocity = false;       // start at the full speed of the gait (stairs/ramps: no acceleration distance in the map)
	};

	struct FRunState
	{
		bool bStarted = false, bEnded = false;
		FVector P0 = FVector::ZeroVector, P1 = FVector::ZeroVector;
		double T0 = 0.0, T1 = 0.0;
		double Path = 0.0;
		FVector Last = FVector::ZeroVector;
		float MaxInput = 0.f;
		int32 Frames = 0;
		FString Mode;
		bool bHaveStart = false;
		int32 ZReversals = 0;
		float MaxFrameDz = 0.f;
		float LastDz = 0.f;
		FVector StartPos = FVector::ZeroVector;
	};

	TMap<FString, FResult>& Results()
	{
		static TMap<FString, FResult> Map;
		return Map;
	}

	void AddCase(FAutomationTestBase& Test, const FSpec& Spec)
	{
		const TSharedRef<FRunState> State = MakeShared<FRunState>();
		FResult& Placeholder = Results().FindOrAdd(Spec.Name);
		Placeholder = FResult();
		Placeholder.Name = Spec.Name;
		const FVector2D Input(Spec.Right, Spec.Forward);
		const FVector2D LocalDir = Input.GetSafeNormal();
		const FVector MoveDir = FRotator(0.f, Spec.Yaw, 0.f).RotateVector(FVector(LocalDir.Y, LocalDir.X, 0.f)); // DoMove(Right, Forward): forward = X, right = Y
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(Test, Spec.Start, Spec.Yaw, 14.f,
			[Spec, MoveDir](ASupernaturalProtCharacter& Character)
			{
				USpnCharacterMovementComponent* Mov = Character.GetSpnMovement();
				Mov->SetBaseGait(Spec.Gait);
				if (Spec.bSprint) { Character.DoSprintStart(); } else { Character.DoSprintEnd(); }
				if (Spec.Fps > 0.f)
				{
					FApp::SetUseFixedTimeStep(true);
					FApp::SetFixedDeltaTime(1.0 / Spec.Fps);
				}
				if (Spec.bPreVelocity)
				{
					const ESpnGait Gait = Spec.bSprint ? ESpnGait::Sprint : Spec.Gait;
					Mov->Velocity = MoveDir * SpnMovement::GaitSpeed(Mov->Settings, Gait);
				}
			},
			[Spec, State, MoveDir](ASupernaturalProtCharacter& Character, float Delta, double Elapsed)
			{
				Character.DoMove(Spec.Right, Spec.Forward);
				USpnCharacterMovementComponent* Mov = Character.GetSpnMovement();
				const FVector Pos = Character.GetActorLocation();
				const float InputMagnitude = Mov->GetMaxAcceleration() > 0.f ? Mov->GetCurrentAcceleration().Size() / Mov->GetMaxAcceleration() : 0.f;
				if (!State->bHaveStart) { State->StartPos = Pos; State->bHaveStart = true; }
				const float Along = FVector::DotProduct(Pos - State->StartPos, MoveDir);
				if (!State->bStarted && Along >= Spec.S0)
				{
					State->bStarted = true;
					State->P0 = Pos; State->T0 = Elapsed; State->Last = Pos;
				}
				else if (State->bStarted && !State->bEnded)
				{
					const float Dz = Pos.Z - State->Last.Z;
					if (FMath::Abs(Dz) > 0.05f && FMath::Abs(State->LastDz) > 0.05f && FMath::Sign(Dz) != FMath::Sign(State->LastDz)) { ++State->ZReversals; }
					if (FMath::Abs(Dz) > 0.05f) { State->LastDz = Dz; }
					State->MaxFrameDz = FMath::Max(State->MaxFrameDz, FMath::Abs(Dz));
					State->Path += FVector::Dist(Pos, State->Last);
					State->Last = Pos;
					State->MaxInput = FMath::Max(State->MaxInput, InputMagnitude);
					++State->Frames;
					State->Mode = Mov->GetMovementName();
					if (Along >= Spec.S1)
					{
						State->bEnded = true;
						State->P1 = Pos; State->T1 = Elapsed;
					}
				}
				return State->bEnded;
			},
			[Spec, State, MoveDir](ASupernaturalProtCharacter&, bool)
			{
				if (Spec.Fps > 0.f)
				{
					FApp::SetUseFixedTimeStep(false);
				}
				FResult& R = Results().FindOrAdd(Spec.Name);
				R.Name = Spec.Name;
				R.bDone = State->bEnded;
				if (State->bEnded)
				{
					R.WindowSeconds = static_cast<float>(State->T1 - State->T0);
					R.HorizontalSpeed = R.WindowSeconds > 0.f ? FVector::DotProduct(State->P1 - State->P0, MoveDir) / R.WindowSeconds : 0.f;
					R.Speed3D = R.WindowSeconds > 0.f ? FVector::Dist(State->P1, State->P0) / R.WindowSeconds : 0.f;
					R.PathSpeed = R.WindowSeconds > 0.f ? State->Path / R.WindowSeconds : 0.f;
					R.MaxInput = State->MaxInput;
					R.ZGain = State->P1.Z - State->P0.Z;
				R.ZReversals = State->ZReversals;
				R.MaxFrameDz = State->MaxFrameDz;
					R.MeanDelta = State->Frames > 0 ? R.WindowSeconds / State->Frames : 0.f;
					R.Mode = State->Mode;
				}
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}

	struct FDir { const TCHAR* Name; float Right, Forward; };
	const FDir Dirs[] = {{TEXT("W"), 0.f, 1.f}, {TEXT("W+A"), -1.f, 1.f}, {TEXT("W+D"), 1.f, 1.f}, {TEXT("S"), 0.f, -1.f}, {TEXT("S+A"), -1.f, -1.f}, {TEXT("A"), -1.f, 0.f}};

	FString Describe(const FResult& R)
	{
		return FString::Printf(TEXT("SPEEDAUDIT: %-34s done=%d  horiz %7.1f cm/s  3D %7.1f cm/s  path %7.1f cm/s  maxInput %.3f  window %.3f s  dz %6.1f cm  dt %.4f s  zReversals %d  maxFrameDz %.2f cm  mode %s"),
			*R.Name, R.bDone ? 1 : 0, R.HorizontalSpeed, R.Speed3D, R.PathSpeed, R.MaxInput, R.WindowSeconds, R.ZGain, R.MeanDelta, R.ZReversals, R.MaxFrameDz, *R.Mode);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementSpeedAuditTest, "SpnGame.Movement.SpeedAudit", SpeedFlags)

bool FSpnMovementSpeedAuditTest::RunTest(const FString&)
{
	if (!RequireMap(*this)) { return false; }
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	Results().Reset();

	const FVector Plaza(0.f, 0.f, StandZ);
	// level ground: cardinal, diagonal, backward, strafe; walk (Jog) and sprint
	for (const bool bSprint : {false, true})
	{
		for (const FDir& D : Dirs)
		{
			FSpec S;
			S.Name = FString::Printf(TEXT("level %s %s"), bSprint ? TEXT("sprint") : TEXT("jog"), D.Name);
			S.Start = Plaza; S.Right = D.Right; S.Forward = D.Forward; S.bSprint = bSprint;
			AddCase(*this, S);
		}
	}
	// frame-rate independence
	for (const float Fps : {30.f, 60.f, 120.f})
	{
		FSpec S; S.Name = FString::Printf(TEXT("level jog W @%.0ffps"), Fps); S.Start = Plaza; S.Fps = Fps; AddCase(*this, S);
		FSpec D; D.Name = FString::Printf(TEXT("level jog W+D @%.0ffps"), Fps); D.Start = Plaza; D.Right = 1.f; D.Fps = Fps; AddCase(*this, D);
	}

	// stairs: up (jog / sprint / diagonal) and down, at 4 walkable rises
	for (int32 I = 0; I < UE_ARRAY_COUNT(StairRises); ++I)
	{
		const float Y = StairFirstY + I * StairSpacing;
		const float Rise = StairRises[I], Run = StairRuns[I];
		const float TopZ = StandZ + Rise * StairSteps;
		for (const bool bSprint : {false, true})
		{
			FSpec S;
			S.Name = FString::Printf(TEXT("stairs up R%.0f %s W"), Rise, bSprint ? TEXT("sprint") : TEXT("jog"));
			S.Start = FVector(StairStartX - 200.f, Y, StandZ); S.bSprint = bSprint; S.bPreVelocity = true; S.S0 = 200.f + 2.f * Run; S.S1 = 200.f + (StairSteps - 1) * Run;
			AddCase(*this, S);
		}
		FSpec Down;
		Down.Name = FString::Printf(TEXT("stairs down R%.0f jog"), Rise);
		Down.Start = FVector(StairStartX + StairSteps * Run + 150.f, Y, TopZ + 2.f); Down.Yaw = 180.f; Down.bPreVelocity = true; Down.S0 = 150.f + 2.f * Run; Down.S1 = 150.f + (StairSteps - 1) * Run;
		AddCase(*this, Down);
		for (const float Fps : {30.f, 120.f})
		{
			FSpec F;
			F.Name = FString::Printf(TEXT("stairs up R%.0f jog W @%.0ffps"), Rise, Fps);
			F.Start = FVector(StairStartX - 200.f, Y, StandZ); F.bPreVelocity = true; F.Fps = Fps; F.S0 = 200.f + 2.f * Run; F.S1 = 200.f + (StairSteps - 1) * Run;
			AddCase(*this, F);
		}
	}
	// ramps with slopes comparable to the staircases (the lane layout: Tools/spn_movement_map_layout.py)
	for (int32 I = 0; I < UE_ARRAY_COUNT(RampAngles); ++I)
	{
		const float Angle = RampAngles[I];
		if (Angle > 40.f) { continue; }
		FSpec S;
		S.Name = FString::Printf(TEXT("ramp up %.0f deg jog W"), Angle);
		S.Start = FVector(RampStartX - 200.f, (I - (UE_ARRAY_COUNT(RampAngles) - 1) * 0.5f) * RampSpacing, StandZ); S.bPreVelocity = true; S.S0 = 300.f; S.S1 = 1000.f;
		AddCase(*this, S);
	}

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		for (const TPair<FString, FResult>& Pair : Results())
		{
			AddInfo(Describe(Pair.Value));
		}
		const auto H = [](const TCHAR* Name) { const FResult* R = Results().Find(Name); return R != nullptr && R->bDone ? R->HorizontalSpeed : -1.f; };
		const auto Done = [](const TCHAR* Name) { const FResult* R = Results().Find(Name); return R != nullptr && R->bDone; };

		for (const TPair<FString, FResult>& Pair : Results())
		{
			TestTrue(Pair.Key + TEXT(": measurement window reached"), Pair.Value.bDone);
			TestTrue(Pair.Key + TEXT(": input magnitude never above 1"), Pair.Value.MaxInput <= 1.001f);
		}
		for (const bool bSprint : {false, true})
		{
			const FString Tag = bSprint ? TEXT("sprint") : TEXT("jog");
			const float Forward = H(*FString::Printf(TEXT("level %s W"), *Tag));
			if (Forward <= 0.f) { continue; }
			for (const FDir& D : Dirs)
			{
				const FString Name = FString::Printf(TEXT("level %s %s"), *Tag, D.Name);
				const float Speed = H(*Name);
				if (Speed < 0.f) { continue; }
				// a diagonal may be slower (sprint is steered), it must never be faster than the cardinal move
				TestTrue(FString::Printf(TEXT("%s: %.1f cm/s is not faster than forward %.1f cm/s (+3%%)"), *Name, Speed, Forward), Speed <= Forward * 1.03f);
				if (!bSprint) { TestTrue(FString::Printf(TEXT("%s: %.1f cm/s equals forward %.1f cm/s (+-4%%)"), *Name, Speed, Forward), FMath::IsNearlyEqual(Speed, Forward, Forward * 0.04f)); }
			}
		}
		// intended jog speed from the settings
		if (Done(TEXT("level jog W")))
		{
			if (const ASupernaturalProtCharacter* Character = FindCharacter())
			{
				const float Intended = SpnMovement::GaitSpeed(Character->GetSpnMovement()->Settings, ESpnGait::Jog);
				TestTrue(FString::Printf(TEXT("level jog W %.1f cm/s matches the intended gait speed %.1f cm/s (+-5%%)"), H(TEXT("level jog W")), Intended), FMath::IsNearlyEqual(H(TEXT("level jog W")), Intended, Intended * 0.05f));
			}
		}
		// frame-rate independence
		for (const TCHAR* Prefix : {TEXT("level jog W"), TEXT("level jog W+D")})
		{
			const float A = H(*FString::Printf(TEXT("%s @30fps"), Prefix)), B = H(*FString::Printf(TEXT("%s @120fps"), Prefix));
			if (A > 0.f && B > 0.f) { TestTrue(FString::Printf(TEXT("%s: 30 fps %.1f vs 120 fps %.1f cm/s (+-3%%)"), Prefix, A, B), FMath::IsNearlyEqual(A, B, B * 0.03f)); }
		}
		// stairs: horizontal speed must not exceed the level speed of the same gait, and must not rise with the steepness
		const float LevelJog = H(TEXT("level jog W")), LevelSprint = H(TEXT("level sprint W"));
		for (int32 I = 0; I < UE_ARRAY_COUNT(StairRises); ++I)
		{
			const float Rise = StairRises[I];
			const float Up = H(*FString::Printf(TEXT("stairs up R%.0f jog W"), Rise));
			const float Down = H(*FString::Printf(TEXT("stairs down R%.0f jog"), Rise));
			const float UpSprint = H(*FString::Printf(TEXT("stairs up R%.0f sprint W"), Rise));
			if (Up > 0.f && LevelJog > 0.f) { TestTrue(FString::Printf(TEXT("stairs up R%.0f jog: %.1f cm/s not faster than level %.1f cm/s (+3%%)"), Rise, Up, LevelJog), Up <= LevelJog * 1.03f); }
			if (Down > 0.f && LevelJog > 0.f) { TestTrue(FString::Printf(TEXT("stairs down R%.0f jog: %.1f cm/s not faster than level %.1f cm/s (+3%%)"), Rise, Down, LevelJog), Down <= LevelJog * 1.03f); }
			if (UpSprint > 0.f && LevelSprint > 0.f) { TestTrue(FString::Printf(TEXT("stairs up R%.0f sprint: %.1f cm/s not faster than level %.1f cm/s (+3%%)"), Rise, UpSprint, LevelSprint), UpSprint <= LevelSprint * 1.03f); }
			const float Low = H(*FString::Printf(TEXT("stairs up R%.0f jog W @30fps"), Rise)), High = H(*FString::Printf(TEXT("stairs up R%.0f jog W @120fps"), Rise));
			if (Low > 0.f && High > 0.f)
			{
				TestTrue(FString::Printf(TEXT("stairs R%.0f: 30 fps %.1f cm/s is not faster than 120 fps %.1f cm/s (+3%%)"), Rise, Low, High), Low <= High * 1.03f);
				// R25 is exactly the step limit: at 30 fps a step-up is sometimes retried, which costs time (measured: -7%). Slower, never faster.
				if (Rise < 25.f) { TestTrue(FString::Printf(TEXT("stairs R%.0f: 30 fps %.1f vs 120 fps %.1f cm/s (+-5%%)"), Rise, Low, High), FMath::IsNearlyEqual(Low, High, High * 0.05f)); }
			}
		}
		// the steepness must not change the horizontal speed: spread over the walkable staircases <= 3%
		float Min = TNumericLimits<float>::Max(), Max = 0.f;
		for (const float Rise : StairRises)
		{
			const float Up = H(*FString::Printf(TEXT("stairs up R%.0f jog W"), Rise));
			if (Up > 0.f) { Min = FMath::Min(Min, Up); Max = FMath::Max(Max, Up); }
		}
		if (Max > 0.f) { TestTrue(FString::Printf(TEXT("stairs up jog: horizontal speed spread across the rises %.1f..%.1f cm/s is within 3%%"), Min, Max), Max <= Min * 1.03f); }
		return true;
	}));
	return true;
}

#endif
