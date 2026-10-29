#include "SpnWorldFrame.h"

#include <spn/core/Coordinates.h>

namespace
{
	spn::core::UnrealAxes ToCore(ESpnWorldAxes Axes)
	{
		return Axes == ESpnWorldAxes::NorthEastUp ? spn::core::UnrealAxes::NorthEastUp : spn::core::UnrealAxes::EastSouthUp;
	}

	spn::core::UnrealFrame ToCore(const FSpnWorldFrame& Frame)
	{
		spn::core::UnrealFrame Out;
		Out.axes = ToCore(Frame.Axes);
		Out.originMetres = spn::core::WorldPoint{Frame.OriginMetres.X, Frame.OriginMetres.Y, Frame.OriginMetres.Z};
		return Out;
	}
}

bool SpnWorldFrame::ToUnreal(const FSpnWorldFrame& Frame, const FVector& WorldMetres, FVector& OutUnrealCm)
{
	const auto Result = spn::core::WorldToUnreal(ToCore(Frame), spn::core::WorldPoint{WorldMetres.X, WorldMetres.Y, WorldMetres.Z});
	if (!Result.HasValue())
	{
		return false;
	}
	OutUnrealCm = FVector(Result.Value().x, Result.Value().y, Result.Value().z);
	return true;
}

bool SpnWorldFrame::ToWorld(const FSpnWorldFrame& Frame, const FVector& UnrealCm, FVector& OutWorldMetres)
{
	const auto Result = spn::core::UnrealToWorld(ToCore(Frame), spn::core::UnrealPoint{UnrealCm.X, UnrealCm.Y, UnrealCm.Z});
	if (!Result.HasValue())
	{
		return false;
	}
	OutWorldMetres = FVector(Result.Value().x, Result.Value().y, Result.Value().z);
	return true;
}

bool SpnWorldFrame::BearingToYawDegrees(ESpnWorldAxes Axes, double BearingRadians, double& OutYawDegrees)
{
	const auto Result = spn::core::BearingToUnrealYawDegrees(ToCore(Axes), BearingRadians);
	if (!Result.HasValue())
	{
		return false;
	}
	OutYawDegrees = Result.Value();
	return true;
}

bool SpnWorldFrame::YawDegreesToBearing(ESpnWorldAxes Axes, double YawDegrees, double& OutBearingRadians)
{
	const auto Result = spn::core::UnrealYawDegreesToBearing(ToCore(Axes), YawDegrees);
	if (!Result.HasValue())
	{
		return false;
	}
	OutBearingRadians = Result.Value();
	return true;
}
