// Ledge assist of USpnCharacterMovementComponent (developer decision 2026-11-14): measurement, collision validation and the custom movement
// mode that moves the capsule along the core path (spn/core/LedgeAssist.h). See Docs/Development/Ledge-Assist-2026-11-14.md.

#include "Spn/SpnCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "SupernaturalProt.h"

namespace
{
	constexpr float TopClearance = 3.f;      // cm; same as the core's clearance
	constexpr float ProbeSphere = 5.f;       // cm; radius of the face probe
	constexpr float FaceStep = 15.f;         // cm between the face probes
	constexpr float DepthStep = 10.f;        // cm between the depth samples
	constexpr float DepthMax = 140.f;        // cm the top is followed (beyond the vault limit and a standing capsule)

	bool IsWalkableNormal(const FVector& Normal, float WalkableZ) { return Normal.Z >= WalkableZ - KINDA_SMALL_NUMBER; }
}

FSpnLedgePlan USpnCharacterMovementComponent::EvaluateLedgeAssist(FSpnLedgeProbe* OutProbe) const
{
	FSpnLedgeProbe Probe;
	FVector Dir;
	TArray<UPrimitiveComponent*> Ledge;
	const FSpnLedgePlan Plan = PlanLedge(Probe, Dir, Ledge);
	if (OutProbe != nullptr) { *OutProbe = Probe; }
	return Plan;
}

FSpnLedgePlan USpnCharacterMovementComponent::PlanLedge(FSpnLedgeProbe& Probe, FVector& Dir, TArray<UPrimitiveComponent*>& Ledge) const
{
	Probe = FSpnLedgeProbe();
	Ledge.Reset();
	FSpnLedgePlan None;
	None.Reason = TEXT("no character");
	const UWorld* World = GetWorld();
	const UCapsuleComponent* Capsule = CharacterOwner != nullptr ? CharacterOwner->GetCapsuleComponent() : nullptr;
	if (World == nullptr || Capsule == nullptr || UpdatedComponent == nullptr)
	{
		return None;
	}
	const float R = Capsule->GetScaledCapsuleRadius();
	const float HH = Capsule->GetScaledCapsuleHalfHeight();
	const FVector Loc = UpdatedComponent->GetComponentLocation();
	// heights are measured from the floor the character stands on (the capsule hovers a few cm above it)
	const float FeetZ = Loc.Z - HH - (CurrentFloor.bBlockingHit ? CurrentFloor.FloorDist : 0.f);

	// Travel direction: where the player steers (the stick / keys), else where the character moves, else where it faces. The input wins over
	// the velocity: pushed diagonally against a wall the velocity runs along the wall, but the player means the wall.
	Dir = Acceleration.GetSafeNormal2D();
	if (Dir.IsNearlyZero())
	{
		Dir = Velocity.Size2D() > 50.f ? Velocity.GetSafeNormal2D() : FVector::ZeroVector;
	}
	if (Dir.IsNearlyZero())
	{
		Dir = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	}
	Probe.ApproachSpeed = FMath::Max(static_cast<float>(FVector::DotProduct(Velocity, Dir)), 0.f);
	const float Reach = SpnLedgeAssist::Reach(LedgeAssist, Probe.ApproachSpeed);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnLedgeProbe), false, CharacterOwner);
	FCollisionResponseParams Response;
	InitCollisionParams(Params, Response);
	const ECollisionChannel Channel = UpdatedComponent->GetCollisionObjectType();

	// 1. The face: small sphere sweeps forward from the capsule front at heights through the three bands.
	FHitResult Face;
	float FaceDistance = TNumericLimits<float>::Max();
	for (float H = MaxStepHeight + 2.f; H <= LedgeAssist.HighBandMaxCm + 1.f; H += FaceStep)
	{
		const FVector Start = FVector(Loc.X, Loc.Y, FeetZ + H) + Dir * (R - ProbeSphere - 2.f);
		const FVector End = FVector(Loc.X, Loc.Y, FeetZ + H) + Dir * (R + Reach + 2.f);
		FHitResult Hit;
		if (World->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, Channel, FCollisionShape::MakeSphere(ProbeSphere), Params, Response) && !Hit.bStartPenetrating
			&& FMath::Abs(Hit.ImpactNormal.Z) < 0.35f)
		{
			const float Distance = static_cast<float>(FVector::DotProduct(Hit.ImpactPoint - Loc, Dir)) - R;
			if (Distance < FaceDistance)
			{
				FaceDistance = Distance;
				Face = Hit;
			}
		}
	}
	if (Face.GetComponent() == nullptr)
	{
		None.Reason = TEXT("nothing in front");
		return None;
	}
	Probe.bFaceFound = true;
	Probe.FaceDistance = FaceDistance;
	const FVector Inward = -Face.ImpactNormal.GetSafeNormal2D();
	Probe.ApproachAngle = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(static_cast<float>(FVector::DotProduct(Inward, Dir)), -1.f, 1.f)));
	UPrimitiveComponent* FaceComponent = Face.GetComponent();
	Probe.bSurfaceAllowed = FaceComponent->CanCharacterStepUp(CharacterOwner) && !FaceComponent->IsSimulatingPhysics() && Cast<APawn>(Face.GetActor()) == nullptr
		&& FaceComponent->GetComponentVelocity().SizeSquared() < 1.f;
	Ledge.Add(FaceComponent);

	// Points along the travel direction from the face (the face plane met head-on along Dir).
	const FVector FacePoint = FVector(Loc.X, Loc.Y, FeetZ) + Dir * (R + FaceDistance);
	auto Along = [&](float D) { return FacePoint + Dir * D; };

	// 2. The top: nothing may continue above the highest band right behind the edge (a higher wall, an overhang) ...
	{
		const float Z = FeetZ + LedgeAssist.HighBandMaxCm + 25.f;
		FHitResult Above;
		const FVector Start = FVector(Loc.X, Loc.Y, Z);
		if (World->LineTraceSingleByChannel(Above, Start, FVector(Along(LedgeAssist.MinGripDepthCm).X, Along(LedgeAssist.MinGripDepthCm).Y, Z), Channel, Params, Response))
		{
			Probe.bTopFound = false;
			FSpnLedgePlan Plan = SpnLedgeAssist::Plan(LedgeAssist, MaxStepHeight, Probe, R);
			return Plan;
		}
	}
	// ... and a surface found from above just behind the edge.
	auto TopAt = [&](float D, float FromZ, float ToZ, FHitResult& Out) -> bool
	{
		const FVector P = Along(D);
		return World->LineTraceSingleByChannel(Out, FVector(P.X, P.Y, FromZ), FVector(P.X, P.Y, ToZ), Channel, Params, Response) && !Out.bStartPenetrating;
	};
	FHitResult Top;
	if (!TopAt(6.f, FeetZ + LedgeAssist.HighBandMaxCm + 20.f, FeetZ + MaxStepHeight - 5.f, Top))
	{
		Probe.bTopFound = false;
		return SpnLedgeAssist::Plan(LedgeAssist, MaxStepHeight, Probe, R);
	}
	Probe.bTopFound = true;
	Probe.Height = static_cast<float>(Top.ImpactPoint.Z) - FeetZ;
	Probe.TopAngle = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(static_cast<float>(Top.ImpactNormal.Z), -1.f, 1.f)));
	if (Top.GetComponent() != nullptr && Top.GetComponent() != FaceComponent)
	{
		Ledge.Add(Top.GetComponent());
	}
	const float TopZ = static_cast<float>(Top.ImpactPoint.Z);

	// 3. Depth: follow the top while it stays at the same height and walkable.
	float Depth = 6.f;
	for (float D = 6.f + DepthStep; D <= DepthMax; D += DepthStep)
	{
		FHitResult Sample;
		if (!TopAt(D, TopZ + 30.f, TopZ - 15.f, Sample) || FMath::Abs(Sample.ImpactPoint.Z - TopZ) > MaxStepHeight * 0.5f || !IsWalkableNormal(Sample.ImpactNormal, GetWalkableFloorZ()))
		{
			break;
		}
		Depth = D;
	}
	Probe.Depth = Depth + (Depth >= DepthMax - DepthStep ? 0.f : DepthStep * 0.5f);

	// 4. Room to rise in place (a ceiling over the character).
	const FCollisionShape Shape = FCollisionShape::MakeCapsule(R, HH);
	auto SweepFree = [&](const FVector& From, const FVector& To, bool bIgnoreLedge) -> bool
	{
		FCollisionQueryParams P = Params;
		if (bIgnoreLedge) { P.AddIgnoredComponents(Ledge); }
		FHitResult Hit;
		return !World->SweepSingleByChannel(Hit, From, To, UpdatedComponent->GetComponentQuat(), Channel, Shape, P, Response);
	};
	const float Rise = Probe.Height + TopClearance;
	const FVector Base(Loc.X, Loc.Y, FeetZ + HH); // the capsule centre standing exactly on the floor; the path counts from here
	Probe.bLiftClear = SweepFree(Loc, Base + FVector(0.f, 0.f, Rise), /*bIgnoreLedge*/ true);

	// 5. Behind a thin obstacle: a walkable floor not far below or above the start floor.
	if (Probe.Depth <= LedgeAssist.VaultMaxDepthCm)
	{
		FHitResult Floor;
		if (TopAt(Probe.Depth + R + 15.f, TopZ + 20.f, FeetZ - 150.f, Floor) && IsWalkableNormal(Floor.ImpactNormal, GetWalkableFloorZ()))
		{
			Probe.bLandingFound = true;
			Probe.LandingRise = static_cast<float>(Floor.ImpactPoint.Z) - FeetZ;
			Probe.bLandingClear = true; // validated below against the planned path
		}
	}

	// 6. Plan optimistically, then validate the planned path with capsule sweeps and re-plan without what failed (at most three rounds).
	FSpnLedgePlan Plan;
	for (int32 Round = 0; Round < 3; ++Round)
	{
		Plan = SpnLedgeAssist::Plan(LedgeAssist, MaxStepHeight, Probe, R);
		if (Plan.Move == ESpnLedgeMove::None)
		{
			return Plan;
		}
		const FVector Lifted = Base + FVector(0.f, 0.f, Plan.RiseHeight);
		const FVector EndPose = Lifted + Dir * Plan.ForwardEnd;
		// across at top height: the obstacle itself counts again (the capsule bottom is above it)
		const bool bCorridor = SweepFree(Lifted, EndPose, /*bIgnoreLedge*/ false);
		if (Plan.bEndsOnTop)
		{
			// standing on top at the end pose: free, and a walkable floor right below it
			FHitResult Under;
			const FVector Feet = EndPose - FVector(0.f, 0.f, HH);
			const bool bFloor = World->LineTraceSingleByChannel(Under, Feet + FVector(0.f, 0.f, 5.f), Feet - FVector(0.f, 0.f, TopClearance + MaxStepHeight * 0.5f), Channel, Params, Response)
				&& IsWalkableNormal(Under.ImpactNormal, GetWalkableFloorZ());
			if (bCorridor && bFloor)
			{
				return Plan;
			}
			Probe.bTopClear = false;
		}
		else
		{
			// the landing pose behind the obstacle must be free as well
			const FVector Landing = FVector(EndPose.X, EndPose.Y, FeetZ + Probe.LandingRise + HH + 2.f);
			FCollisionQueryParams P = Params;
			const bool bLandingFree = !World->OverlapBlockingTestByChannel(Landing, UpdatedComponent->GetComponentQuat(), Channel, Shape, P, Response);
			if (bCorridor && bLandingFree)
			{
				return Plan;
			}
			Probe.bLandingClear = false;
		}
	}
	return Plan.Move == ESpnLedgeMove::None ? Plan : SpnLedgeAssist::Plan(LedgeAssist, MaxStepHeight, Probe, R);
}

bool USpnCharacterMovementComponent::DoJump(bool bReplayingMoves, float DeltaTime)
{
	if (LedgeAssist.bEnabled && !bLedgeActive && IsMovingOnGround() && CharacterOwner != nullptr)
	{
		FSpnLedgeProbe Probe;
		FVector Dir;
		TArray<UPrimitiveComponent*> Ledge;
		const FSpnLedgePlan Plan = PlanLedge(Probe, Dir, Ledge);
		LastLedge = FSpnLedgeRecord();
		LastLedge.Move = Plan.Move;
		LastLedge.Reason = Plan.Reason;
		LastLedge.bFaceInReach = Probe.bFaceFound;
		LastLedge.Height = Probe.Height;
		LastLedge.Depth = Probe.Depth;
		LastLedge.FaceDistance = Probe.FaceDistance;
		LastLedge.ApproachSpeed = Probe.ApproachSpeed;
		LastLedge.PlannedSeconds = Plan.TotalSeconds;
		LastLedge.ExitSpeed = Plan.ExitSpeed;
		if (Plan.Move != ESpnLedgeMove::None)
		{
			StartLedgeAssist(Plan, Dir, Ledge);
			return true;
		}
		if (Probe.bFaceFound)
		{
			UE_LOG(LogSupernaturalProt, Log, TEXT("LedgeAssist: %s presses jump %.0f cm before an obstacle (top %.0f cm): no assisted move (%s), ordinary jump"),
				*GetNameSafe(CharacterOwner), Probe.FaceDistance, Probe.Height, *Plan.Reason);
		}
	}
	return Super::DoJump(bReplayingMoves, DeltaTime);
}

void USpnCharacterMovementComponent::StartLedgeAssist(const FSpnLedgePlan& Plan, const FVector& Dir, const TArray<UPrimitiveComponent*>& Ledge)
{
	ActiveLedge = Plan;
	// the path's "up" counts from the floor (the plan's heights do), not from the hovering capsule
	LedgeStart = UpdatedComponent->GetComponentLocation() - FVector(0.f, 0.f, CurrentFloor.bBlockingHit ? CurrentFloor.FloorDist : 0.f);
	LedgeDir = Dir;
	LedgeElapsed = 0.f;
	bLedgeActive = true;
	++LedgeMoveCount;
	LastLedge.bRunning = true;
	// The planted leg passes beside the edge while the body rises (validated above: the upper body never enters the obstacle); the obstacle is
	// collision again as soon as the capsule bottom is above its top.
	if (UPrimitiveComponent* Primitive = UpdatedPrimitive)
	{
		for (UPrimitiveComponent* Component : Ledge)
		{
			Primitive->IgnoreComponentWhenMoving(Component, true);
			LedgeIgnored.Add(Component);
		}
	}
	UE_LOG(LogSupernaturalProt, Log, TEXT("LedgeAssist: %s %s (%s): height %.0f cm, depth %.0f cm, face %.0f cm ahead, approach %.0f cm/s -> %.2f s, exit %.0f cm/s"),
		*GetNameSafe(CharacterOwner), SpnLedgeAssist::MoveName(Plan.Move), *Plan.Reason, Plan.Height, Plan.Depth, LastLedge.FaceDistance, Plan.ApproachSpeed, Plan.TotalSeconds, Plan.ExitSpeed);
	SetMovementMode(MOVE_Custom, CustomModeLedgeAssist);
}

void USpnCharacterMovementComponent::StopIgnoringLedge()
{
	if (UPrimitiveComponent* Primitive = UpdatedPrimitive)
	{
		for (const TWeakObjectPtr<UPrimitiveComponent>& Component : LedgeIgnored)
		{
			if (Component.IsValid())
			{
				Primitive->IgnoreComponentWhenMoving(Component.Get(), false);
			}
		}
	}
	LedgeIgnored.Reset();
}

void USpnCharacterMovementComponent::FinishLedgeAssist(bool bCompleted, const TCHAR* Why)
{
	if (!bLedgeActive)
	{
		return;
	}
	bLedgeActive = false;
	StopIgnoringLedge();
	LastLedge.bRunning = false;
	LastLedge.bCompleted = bCompleted;
	LastLedge.ActualSeconds = LedgeElapsed;
	LastLedge.EndReason = Why;
	UE_LOG(LogSupernaturalProt, Log, TEXT("LedgeAssist: %s %s %s after %.2f s (%s)"), *GetNameSafe(CharacterOwner), SpnLedgeAssist::MoveName(ActiveLedge.Move),
		bCompleted ? TEXT("completed") : TEXT("released"), LedgeElapsed, Why);
}

void USpnCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	if (CustomMovementMode != CustomModeLedgeAssist || !bLedgeActive)
	{
		Super::PhysCustom(DeltaTime, Iterations);
		if (CustomMovementMode == CustomModeLedgeAssist)
		{
			SetMovementMode(MOVE_Falling); // a stale mode (no move running): hand back to the normal physics
		}
		return;
	}
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}
	LedgeElapsed += DeltaTime;
	bool bDone = false;
	const FVector2D Path = SpnLedgeAssist::Sample(ActiveLedge, LedgeElapsed, bDone);
	const FVector Target = LedgeStart + LedgeDir * Path.X + FVector(0.f, 0.f, Path.Y);
	const FVector Old = UpdatedComponent->GetComponentLocation();
	if (LedgeIgnored.Num() > 0 && Old.Z - LedgeStart.Z >= ActiveLedge.Height + 1.f)
	{
		StopIgnoringLedge(); // above the top now: the obstacle collides again
	}
	const FVector Delta = Target - Old;
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, LedgeDir.Rotation().Quaternion(), true, Hit);
	const FVector Moved = UpdatedComponent->GetComponentLocation() - Old;
	Velocity = Moved / DeltaTime;

	if (Hit.IsValidBlockingHit() && Moved.Size() < 0.5f * Delta.Size() && Delta.Size() > 1.f)
	{
		// Something not in the validated path (a moving object, a pawn): release into a fall at the current place, never push through.
		FinishLedgeAssist(false, TEXT("blocked on the way"));
		Velocity = FVector(Velocity.X, Velocity.Y, FMath::Min(static_cast<float>(Velocity.Z), 0.f)) * 0.5f;
		SetMovementMode(MOVE_Falling);
		return;
	}
	if (!bDone)
	{
		return;
	}
	const bool bOnTop = ActiveLedge.bEndsOnTop;
	FinishLedgeAssist(true, bOnTop ? TEXT("standing on top") : TEXT("across, landing behind"));
	Velocity = LedgeDir * ActiveLedge.ExitSpeed;
	if (bOnTop)
	{
		FFindFloorResult Floor;
		FindFloor(UpdatedComponent->GetComponentLocation(), Floor, false);
		if (Floor.IsWalkableFloor())
		{
			SetMovementMode(MOVE_Walking);
			return;
		}
	}
	SetMovementMode(MOVE_Falling);
}
