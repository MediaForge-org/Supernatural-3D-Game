#include "SpnPlayerPose.h"

namespace
{
	constexpr int32 MaxLevelLength = 256;

	bool IsNumberText(const FString& Text)
	{
		if (Text.IsEmpty() || Text.Len() > 40)
		{
			return false;
		}
		bool bDigit = false;
		for (int32 I = 0; I < Text.Len(); ++I)
		{
			const TCHAR C = Text[I];
			if (C >= '0' && C <= '9') { bDigit = true; continue; }
			if (C == '-' || C == '+' || C == '.' || C == 'e' || C == 'E') { continue; }
			return false;
		}
		return bDigit;
	}

	bool ParseNumber(const FString& Text, double& Out)
	{
		if (!IsNumberText(Text))
		{
			return false;
		}
		const double Value = FCString::Atod(*Text);
		if (!FMath::IsFinite(Value))
		{
			return false;
		}
		Out = Value;
		return true;
	}

	bool ValidLevel(const FString& Level)
	{
		if (Level.Len() < 2 || Level.Len() > MaxLevelLength || Level[0] != '/')
		{
			return false;
		}
		for (int32 I = 0; I < Level.Len(); ++I)
		{
			const TCHAR C = Level[I];
			const bool bOk = (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z') || (C >= '0' && C <= '9') || C == '/' || C == '_' || C == '-' || C == '.';
			if (!bOk)
			{
				return false;
			}
		}
		return !Level.Contains(TEXT(".."));
	}

	bool ValidGait(const FString& Gait)
	{
		if (Gait.IsEmpty() || Gait.Len() > 16)
		{
			return false;
		}
		for (int32 I = 0; I < Gait.Len(); ++I)
		{
			if (!((Gait[I] >= 'a' && Gait[I] <= 'z') || Gait[I] == '_'))
			{
				return false;
			}
		}
		return true;
	}

	FString Num(double Value) { return FString::Printf(TEXT("%.17g"), Value); }

	bool ParseVector(const FString& Text, FVector& Out)
	{
		TArray<FString> Parts;
		Text.ParseIntoArray(Parts, TEXT(" "), false);
		if (Parts.Num() != 3)
		{
			return false;
		}
		double V[3];
		for (int32 I = 0; I < 3; ++I)
		{
			if (!ParseNumber(Parts[I], V[I]))
			{
				return false;
			}
		}
		Out = FVector(V[0], V[1], V[2]);
		return true;
	}
}

bool FSpnPlayerPose::FromUnreal(const FString& InLevel, const FSpnWorldFrame& InFrame, const FVector& LocationCm, double YawDegrees, double InPitchDeg,
	const FString& InGait, bool bInCrouched, FSpnPlayerPose& Out)
{
	FVector Metres;
	double Bearing = 0.0;
	if (!SpnWorldFrame::ToWorld(InFrame, LocationCm, Metres) || !SpnWorldFrame::YawDegreesToBearing(InFrame.Axes, YawDegrees, Bearing) || !ValidLevel(InLevel)
		|| !ValidGait(InGait) || !FMath::IsFinite(InPitchDeg))
	{
		return false;
	}
	Out.Level = InLevel;
	Out.Frame = InFrame;
	Out.WorldMetres = Metres;
	Out.BearingRad = Bearing;
	Out.PitchDeg = FMath::Clamp(InPitchDeg, -89.0, 89.0);
	Out.Gait = InGait;
	Out.bCrouched = bInCrouched;
	return true;
}

bool FSpnPlayerPose::ToUnreal(FVector& OutLocationCm, double& OutYawDegrees) const
{
	FVector Cm;
	double Yaw = 0.0;
	if (!SpnWorldFrame::ToUnreal(Frame, WorldMetres, Cm) || !SpnWorldFrame::BearingToYawDegrees(Frame.Axes, BearingRad, Yaw))
	{
		return false;
	}
	OutLocationCm = Cm;
	OutYawDegrees = Yaw;
	return true;
}

FString FSpnPlayerPose::Serialize() const
{
	return FString::Printf(TEXT("SPN-PLAYERPOSE 1\nlevel %s\naxes %s\norigin_m %s %s %s\npos_m %s %s %s\nbearing_rad %s\npitch_deg %s\ngait %s\ncrouched %d\n"), *Level,
		Frame.Axes == ESpnWorldAxes::EastSouthUp ? TEXT("east_south_up") : TEXT("north_east_up"), *Num(Frame.OriginMetres.X), *Num(Frame.OriginMetres.Y),
		*Num(Frame.OriginMetres.Z), *Num(WorldMetres.X), *Num(WorldMetres.Y), *Num(WorldMetres.Z), *Num(BearingRad), *Num(PitchDeg), *Gait, bCrouched ? 1 : 0);
}

bool FSpnPlayerPose::Parse(const FString& Payload, FSpnPlayerPose& Out, FString& Error)
{
	TArray<FString> Lines;
	Payload.ParseIntoArray(Lines, TEXT("\n"), false);
	// the payload ends with '\n', so the split leaves one empty last element
	if (Lines.Num() != 10 || !Lines.Last().IsEmpty() || Payload.Contains(TEXT("\r")))
	{
		Error = TEXT("player.pose: wrong number of lines");
		return false;
	}
	if (Lines[0] != TEXT("SPN-PLAYERPOSE 1"))
	{
		Error = TEXT("player.pose: bad header");
		return false;
	}
	auto Field = [&Lines, &Error](int32 Index, const TCHAR* Key, FString& Value)
	{
		const FString Prefix = FString(Key) + TEXT(" ");
		if (!Lines[Index].StartsWith(Prefix, ESearchCase::CaseSensitive))
		{
			Error = FString::Printf(TEXT("player.pose: expected '%s'"), Key);
			return false;
		}
		Value = Lines[Index].Mid(Prefix.Len());
		return true;
	};
	FSpnPlayerPose Pose;
	FString Text;
	if (!Field(1, TEXT("level"), Pose.Level) || !ValidLevel(Pose.Level))
	{
		Error = TEXT("player.pose: invalid level");
		return false;
	}
	if (!Field(2, TEXT("axes"), Text))
	{
		return false;
	}
	if (Text == TEXT("east_south_up")) { Pose.Frame.Axes = ESpnWorldAxes::EastSouthUp; }
	else if (Text == TEXT("north_east_up")) { Pose.Frame.Axes = ESpnWorldAxes::NorthEastUp; }
	else { Error = TEXT("player.pose: unknown axes"); return false; }
	if (!Field(3, TEXT("origin_m"), Text) || !ParseVector(Text, Pose.Frame.OriginMetres))
	{
		Error = TEXT("player.pose: invalid origin_m");
		return false;
	}
	if (!Field(4, TEXT("pos_m"), Text) || !ParseVector(Text, Pose.WorldMetres))
	{
		Error = TEXT("player.pose: invalid pos_m");
		return false;
	}
	if (!Field(5, TEXT("bearing_rad"), Text) || !ParseNumber(Text, Pose.BearingRad) || Pose.BearingRad < 0.0 || Pose.BearingRad > 2.0 * UE_DOUBLE_PI + 1e-9)
	{
		Error = TEXT("player.pose: invalid bearing_rad");
		return false;
	}
	if (!Field(6, TEXT("pitch_deg"), Text) || !ParseNumber(Text, Pose.PitchDeg) || Pose.PitchDeg < -89.0 || Pose.PitchDeg > 89.0)
	{
		Error = TEXT("player.pose: invalid pitch_deg");
		return false;
	}
	if (!Field(7, TEXT("gait"), Pose.Gait) || !ValidGait(Pose.Gait))
	{
		Error = TEXT("player.pose: invalid gait");
		return false;
	}
	if (!Field(8, TEXT("crouched"), Text) || (Text != TEXT("0") && Text != TEXT("1")))
	{
		Error = TEXT("player.pose: invalid crouched");
		return false;
	}
	Pose.bCrouched = Text == TEXT("1");
	// the position must be representable in its own frame, otherwise the pose is useless and must not be half-applied
	FVector Cm;
	double Yaw;
	if (!Pose.ToUnreal(Cm, Yaw))
	{
		Error = TEXT("player.pose: position outside the representable range");
		return false;
	}
	Out = Pose;
	return true;
}
