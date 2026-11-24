#include "SpnLedgeAssist.h"

#include <spn/core/LedgeAssist.h>

namespace
{
	constexpr double CmToM = 0.01;
	constexpr double MToCm = 100.0;

	spn::core::LedgeAssistConfig ToCore(const FSpnLedgeAssistSettings& S, float MaxStepHeightCm)
	{
		spn::core::LedgeAssistConfig C;
		C.stepHeight = MaxStepHeightCm * CmToM;
		C.lowMax = S.LowBandMaxCm * CmToM;
		C.midMax = S.MidBandMaxCm * CmToM;
		C.highMax = S.HighBandMaxCm * CmToM;
		C.reachStanding = S.ReachStandingCm * CmToM;
		C.reachPerSpeed = S.ReachPerSpeedSeconds;
		C.reachMax = S.ReachMaxCm * CmToM;
		C.minGripDepth = S.MinGripDepthCm * CmToM;
		C.maxApproachAngle = S.MaxApproachAngleDegrees;
		C.vaultMaxDepth = S.VaultMaxDepthCm * CmToM;
		C.vaultMinSpeed = S.VaultMinSpeed * CmToM;
		C.legAllowance = S.LegAllowanceCm * CmToM;
		return C;
	}

	// CoreData layout (internal to this file): the fields of spn::core::LedgePlan in metres / seconds.
	enum ECoreField : int32 { FMove, EndsOnTop, Height, Depth, Radius, FaceAxis, Speed, Lift, Over, LiftFwd, OverSpd, Exit, Rise, FwdEnd, Leg, Count };

	spn::core::LedgePlan FromData(const TArray<double>& D)
	{
		spn::core::LedgePlan P;
		if (D.Num() != Count)
		{
			return P;
		}
		P.move = static_cast<spn::core::LedgeMove>(static_cast<uint8>(D[FMove]));
		P.endsOnTop = D[EndsOnTop] > 0.5;
		P.height = D[Height];
		P.depth = D[Depth];
		P.radius = D[Radius];
		P.faceAxisDistance = D[FaceAxis];
		P.approachSpeed = D[Speed];
		P.liftSeconds = D[Lift];
		P.overSeconds = D[Over];
		P.liftForwardSpeed = D[LiftFwd];
		P.overSpeed = D[OverSpd];
		P.exitSpeed = D[Exit];
		P.riseHeight = D[Rise];
		P.forwardEnd = D[FwdEnd];
		P.legAllowance = D[Leg];
		return P;
	}
}

namespace SpnLedgeAssist
{
	FSpnLedgePlan Plan(const FSpnLedgeAssistSettings& Settings, float MaxStepHeightCm, const FSpnLedgeProbe& Probe, float CapsuleRadiusCm)
	{
		spn::core::LedgeProbe P;
		P.faceFound = Probe.bFaceFound;
		P.faceDistance = Probe.FaceDistance * CmToM;
		P.approachAngle = Probe.ApproachAngle;
		P.approachSpeed = Probe.ApproachSpeed * CmToM;
		P.surfaceAllowed = Probe.bSurfaceAllowed;
		P.topFound = Probe.bTopFound;
		P.height = Probe.Height * CmToM;
		P.topAngle = Probe.TopAngle;
		P.depth = Probe.Depth * CmToM;
		P.liftClear = Probe.bLiftClear;
		P.topClear = Probe.bTopClear;
		P.landingFound = Probe.bLandingFound;
		P.landingRise = Probe.LandingRise * CmToM;
		P.landingClear = Probe.bLandingClear;

		const spn::core::LedgePlan C = spn::core::PlanLedgeAssist(ToCore(Settings, MaxStepHeightCm), P, CapsuleRadiusCm * CmToM);
		FSpnLedgePlan Out;
		Out.Move = static_cast<ESpnLedgeMove>(static_cast<uint8>(C.move));
		Out.Reason = UTF8_TO_TCHAR(C.reason);
		Out.Height = static_cast<float>(C.height * MToCm);
		Out.Depth = static_cast<float>(C.depth * MToCm);
		Out.ApproachSpeed = static_cast<float>(C.approachSpeed * MToCm);
		Out.ExitSpeed = static_cast<float>(C.exitSpeed * MToCm);
		Out.ForwardEnd = static_cast<float>(C.forwardEnd * MToCm);
		Out.RiseHeight = static_cast<float>(C.riseHeight * MToCm);
		Out.LiftSeconds = static_cast<float>(C.liftSeconds);
		Out.TotalSeconds = static_cast<float>(C.TotalSeconds());
		Out.bEndsOnTop = C.endsOnTop;
		Out.CoreData.SetNumZeroed(Count);
		Out.CoreData[FMove] = static_cast<double>(static_cast<uint8>(C.move));
		Out.CoreData[EndsOnTop] = C.endsOnTop ? 1.0 : 0.0;
		Out.CoreData[Height] = C.height;
		Out.CoreData[Depth] = C.depth;
		Out.CoreData[Radius] = C.radius;
		Out.CoreData[FaceAxis] = C.faceAxisDistance;
		Out.CoreData[Speed] = C.approachSpeed;
		Out.CoreData[Lift] = C.liftSeconds;
		Out.CoreData[Over] = C.overSeconds;
		Out.CoreData[LiftFwd] = C.liftForwardSpeed;
		Out.CoreData[OverSpd] = C.overSpeed;
		Out.CoreData[Exit] = C.exitSpeed;
		Out.CoreData[Rise] = C.riseHeight;
		Out.CoreData[FwdEnd] = C.forwardEnd;
		Out.CoreData[Leg] = C.legAllowance;
		return Out;
	}

	float Reach(const FSpnLedgeAssistSettings& Settings, float ApproachSpeed)
	{
		return static_cast<float>(spn::core::LedgeReach(ToCore(Settings, 25.f), ApproachSpeed * CmToM) * MToCm);
	}

	FVector2D Sample(const FSpnLedgePlan& Plan, float Seconds, bool& bOutDone)
	{
		const spn::core::LedgePathPoint Point = spn::core::SampleLedgePath(FromData(Plan.CoreData), Seconds);
		bOutDone = Point.done;
		return FVector2D(Point.forward * MToCm, Point.up * MToCm);
	}

	const TCHAR* MoveName(ESpnLedgeMove Move)
	{
		switch (Move)
		{
		case ESpnLedgeMove::StepOn: return TEXT("StepOn");
		case ESpnLedgeMove::Vault: return TEXT("Vault");
		case ESpnLedgeMove::Mantle: return TEXT("Mantle");
		case ESpnLedgeMove::ClimbUp: return TEXT("ClimbUp");
		default: return TEXT("None");
		}
	}

	FString Validate(const FSpnLedgeAssistSettings& Settings, float MaxStepHeightCm)
	{
		const std::optional<std::string> Error = spn::core::Validate(ToCore(Settings, MaxStepHeightCm));
		return Error.has_value() ? FString(UTF8_TO_TCHAR(Error->c_str())) : FString();
	}
}
