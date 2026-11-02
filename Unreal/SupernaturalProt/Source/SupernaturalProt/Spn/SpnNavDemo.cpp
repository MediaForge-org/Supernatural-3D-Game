#include "Spn/SpnNavDemo.h"

#include "Camera/CameraActor.h"
#include "Containers/Ticker.h"
#include "EngineUtils.h"
#include "HAL/FileManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformTime.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/CommandLine.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/UI/SpnMapScreen.h"
#include "Spn/UI/SpnMapView.h"
#include "Spn/UI/SpnUiSubsystem.h"
#include "SupernaturalProt.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpnNavDemo, Log, All);

namespace
{
	constexpr double RoadThicknessCm = 10.0;
	constexpr double RoadTopCm = 3.0;

	double RoadWidthMetres(uint8 Class)
	{
		static const double Widths[] = {14.0, 8.0, 12.0, 10.0, 8.0, 6.0, 4.0, 3.0, 2.5, 1.6};
		return Widths[FMath::Clamp<int32>(Class, 0, 9)];
	}

	FLinearColor RoadColor(uint8 Class)
	{
		static const FLinearColor Colors[] = {
			FLinearColor(0.045f, 0.045f, 0.05f), FLinearColor(0.05f, 0.05f, 0.055f), FLinearColor(0.05f, 0.05f, 0.055f), FLinearColor(0.06f, 0.06f, 0.065f),
			FLinearColor(0.07f, 0.07f, 0.075f), FLinearColor(0.09f, 0.09f, 0.095f), FLinearColor(0.11f, 0.11f, 0.115f), FLinearColor(0.16f, 0.12f, 0.07f),
			FLinearColor(0.09f, 0.075f, 0.05f), FLinearColor(0.30f, 0.27f, 0.24f)};
		return Colors[FMath::Clamp<int32>(Class, 0, 9)];
	}

	double DistanceToPolyline(const FVector2D& P, const TArray<FVector2D>& Pts)
	{
		double Best = 1.0e18;
		for (int32 I = 0; I + 1 < Pts.Num(); ++I)
		{
			const FVector2D A = Pts[I], B = Pts[I + 1];
			const FVector2D AB = B - A;
			const double L2 = AB.SizeSquared();
			const double T = L2 > 1e-9 ? FMath::Clamp(FVector2D::DotProduct(P - A, AB) / L2, 0.0, 1.0) : 0.0;
			Best = FMath::Min(Best, FVector2D::Distance(P, A + AB * T));
		}
		return Best;
	}

	void Bounds(const TArray<FVector2D>& Pts, FVector2D& Min, FVector2D& Max)
	{
		Min = FVector2D(1e18, 1e18);
		Max = FVector2D(-1e18, -1e18);
		for (const FVector2D& P : Pts)
		{
			Min.X = FMath::Min(Min.X, P.X); Min.Y = FMath::Min(Min.Y, P.Y);
			Max.X = FMath::Max(Max.X, P.X); Max.Y = FMath::Max(Max.Y, P.Y);
		}
	}

	FString FormatDistance(double Metres)
	{
		return Metres >= 1000.0 ? FString::Printf(TEXT("%.1f km"), Metres / 1000.0) : FString::Printf(TEXT("%d m"), FMath::RoundToInt(Metres));
	}

	double SmoothStep(double T) { T = FMath::Clamp(T, 0.0, 1.0); return T * T * (3.0 - 2.0 * T); }

	FVector CatmullRom(const FVector& P0, const FVector& P1, const FVector& P2, const FVector& P3, double T)
	{
		const double T2 = T * T, T3 = T2 * T;
		return 0.5 * ((2.0 * P1) + (-P0 + P2) * T + (2.0 * P0 - 5.0 * P1 + 4.0 * P2 - P3) * T2 + (-P0 + 3.0 * P1 - 3.0 * P2 + P3) * T3);
	}

	static FAutoConsoleCommandWithWorldAndArgs GRunDemo(TEXT("Spn.NavDemo.Run"), TEXT("Runs the scripted map & navigation tech demo (needs Lvl_SpnNavDemo)."),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>&, UWorld* World)
		{
			if (World == nullptr) { return; }
			for (TActorIterator<ASpnNavDemo> It(World); It; ++It) { It->StartScript(); return; }
			UE_LOG(LogSpnNavDemo, Warning, TEXT("Spn.NavDemo.Run: no ASpnNavDemo in this level"));
		}));

	static FAutoConsoleCommandWithWorldAndArgs GDrive(TEXT("Spn.NavDemo.Drive"), TEXT("Spn.NavDemo.Drive [metres per second]: demo autopilot along the active route (0 = stop)."),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			if (World == nullptr) { return; }
			const float Speed = Args.Num() > 0 ? FCString::Atof(*Args[0]) : 30.f;
			for (TActorIterator<ASpnNavDemo> It(World); It; ++It)
			{
				if (Speed <= 0.f) { It->StopAutopilot(); } else { It->StartAutopilot(Speed); }
				return;
			}
		}));
}

ASpnNavDemo::ASpnNavDemo()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true; // the map screen pauses the game; the script keeps running
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

APlayerController* ASpnNavDemo::GetPlayer() const
{
	return GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController() : nullptr;
}

USpnUiSubsystem* ASpnNavDemo::GetUi() const
{
	const APlayerController* PC = GetPlayer();
	const ULocalPlayer* LocalPlayer = PC != nullptr ? PC->GetLocalPlayer() : nullptr;
	return LocalPlayer != nullptr ? LocalPlayer->GetSubsystem<USpnUiSubsystem>() : nullptr;
}

void ASpnNavDemo::Mark(const FString& Name) const
{
	UE_LOG(LogSpnNavDemo, Display, TEXT("SPN_NAVDEMO_MARK %s t=%.2f"), *Name, ScriptTime);
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// building the region
// ---------------------------------------------------------------------------------------------------------------------------------------

bool ASpnNavDemo::ToWorld(const FVector2D& Metres, double ZCm, FVector& Out) const
{
	const USpnMapSubsystem* M = Map.Get();
	return M != nullptr && M->MapToUnreal(Metres, static_cast<float>(ZCm), Out);
}

UInstancedStaticMeshComponent* ASpnNavDemo::GetLayer(const FString& Key, UStaticMesh* Mesh, const FLinearColor& Color, bool bCollision)
{
	if (TObjectPtr<UInstancedStaticMeshComponent>* Found = InstLayers.Find(Key))
	{
		return *Found;
	}
	UInstancedStaticMeshComponent* Layer = NewObject<UInstancedStaticMeshComponent>(this, NAME_None, RF_Transient);
	Layer->SetStaticMesh(Mesh);
	Layer->SetMobility(EComponentMobility::Movable);
	Layer->SetupAttachment(Root);
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	Material->SetVectorParameterValue(TEXT("Color"), Color);
	Layer->SetMaterial(0, Material);
	Layer->SetCollisionProfileName(bCollision ? TEXT("BlockAll") : TEXT("NoCollision"));
	Layer->SetCastShadow(true);
	Layer->RegisterComponent();
	InstLayers.Add(Key, Layer);
	Materials.Add(Key, Material);
	return Layer;
}

void ASpnNavDemo::AddBox(const FString& Key, const FLinearColor& Color, const FVector& CenterCm, const FVector& SizeCm, const FRotator& Rot, bool bCollision)
{
	UInstancedStaticMeshComponent* Layer = GetLayer(Key, CubeMesh, Color, bCollision);
	Layer->AddInstance(FTransform(Rot, CenterCm, SizeCm / 100.0), true);
	++InstanceCount;
}

void ASpnNavDemo::ClearRegion()
{
	for (auto& Pair : InstLayers)
	{
		if (Pair.Value != nullptr) { Pair.Value->DestroyComponent(); }
	}
	InstLayers.Reset();
	Materials.Reset();
	InstanceCount = 0;
	Stats = FBuildStats();
}

void ASpnNavDemo::BuildGround(const FSpnGeometryView& Geo)
{
	const USpnMapSubsystem* M = Map.Get();
	const FSpnMapBounds B = M->GetBounds();
	// cells between all x / y edges of the water bodies: ground where no water lies. Water is a trench (the banks are 3 m high) with the water plane
	// at the bottom, so it is a real obstacle for the pawn.
	TArray<double> Xs{B.Min.X, B.Max.X}, Ys{B.Min.Y, B.Max.Y};
	TArray<TPair<FVector2D, FVector2D>> Waters;
	for (const FSpnGeoArea& A : Geo.Areas)
	{
		if (A.Kind != 0) { continue; }
		FVector2D Min, Max;
		Bounds(A.Outline, Min, Max);
		Waters.Emplace(Min, Max);
		Xs.Add(FMath::Clamp(Min.X, B.Min.X, B.Max.X)); Xs.Add(FMath::Clamp(Max.X, B.Min.X, B.Max.X));
		Ys.Add(FMath::Clamp(Min.Y, B.Min.Y, B.Max.Y)); Ys.Add(FMath::Clamp(Max.Y, B.Min.Y, B.Max.Y));
	}
	Xs.Sort(); Ys.Sort();
	const FLinearColor Grass(0.045f, 0.105f, 0.03f);
	for (int32 I = 0; I + 1 < Xs.Num(); ++I)
	{
		for (int32 J = 0; J + 1 < Ys.Num(); ++J)
		{
			if (Xs[I + 1] - Xs[I] < 0.5 || Ys[J + 1] - Ys[J] < 0.5) { continue; }
			const FVector2D Centre((Xs[I] + Xs[I + 1]) * 0.5, (Ys[J] + Ys[J + 1]) * 0.5);
			bool bWet = false;
			for (const auto& W : Waters)
			{
				if (Centre.X > W.Key.X && Centre.X < W.Value.X && Centre.Y > W.Key.Y && Centre.Y < W.Value.Y) { bWet = true; break; }
			}
			if (bWet) { continue; }
			FVector C0, C1;
			ToWorld(FVector2D(Xs[I], Ys[J]), 0.0, C0);
			ToWorld(FVector2D(Xs[I + 1], Ys[J + 1]), 0.0, C1);
			const FVector Mid = (C0 + C1) * 0.5;
			const FVector Size(FMath::Abs(C1.X - C0.X), FMath::Abs(C1.Y - C0.Y), 300.0);
			AddBox(TEXT("ground"), Grass, FVector(Mid.X, Mid.Y, -150.0), Size, FRotator::ZeroRotator);
			++Stats.GroundCells;
		}
	}
}

void ASpnNavDemo::BuildAreas(const FSpnGeometryView& Geo)
{
	struct FKindStyle { const TCHAR* Key; FLinearColor Color; };
	static const FKindStyle Styles[] = {
		{TEXT("water"), FLinearColor(0.03f, 0.12f, 0.25f)}, {TEXT("island"), FLinearColor(0.085f, 0.15f, 0.06f)}, {TEXT("forest"), FLinearColor(0.035f, 0.10f, 0.04f)},
		{TEXT("park"), FLinearColor(0.10f, 0.22f, 0.08f)}, {TEXT("green"), FLinearColor(0.11f, 0.19f, 0.07f)}, {TEXT("industrial"), FLinearColor(0.20f, 0.19f, 0.17f)},
		{TEXT("parking"), FLinearColor(0.07f, 0.07f, 0.08f)}, {TEXT("plaza"), FLinearColor(0.38f, 0.36f, 0.33f)}, {TEXT("district"), FLinearColor::Transparent},
		{TEXT("mountain"), FLinearColor(0.20f, 0.17f, 0.14f)}};
	UStaticMesh* Cyl = CylinderMesh;
	UStaticMesh* Cone = ConeMesh;
	for (const FSpnGeoArea& A : Geo.Areas)
	{
		if (A.Outline.Num() < 3 || A.Kind >= UE_ARRAY_COUNT(Styles) || A.Kind == 8) { continue; }
		FVector2D Min, Max;
		Bounds(A.Outline, Min, Max);
		FVector C0, C1;
		ToWorld(Min, 0.0, C0);
		ToWorld(Max, 0.0, C1);
		const FVector Mid = (C0 + C1) * 0.5;
		const FVector2D SizeCm(FMath::Abs(C1.X - C0.X), FMath::Abs(C1.Y - C0.Y));
		const FKindStyle& Style = Styles[A.Kind];
		if (A.Kind == 0)
		{
			// water: opaque plane at the bottom of the trench; the pawn cannot cross it
			AddBox(TEXT("water"), Style.Color, FVector(Mid.X, Mid.Y, -310.0), FVector(SizeCm.X, SizeCm.Y, 20.0), FRotator::ZeroRotator);
			++Stats.Water;
		}
		else if (A.Kind == 1)
		{
			AddBox(TEXT("island"), Style.Color, FVector(Mid.X, Mid.Y, -155.0), FVector(SizeCm.X, SizeCm.Y, 310.0), FRotator::ZeroRotator);
			++Stats.Slabs;
		}
		else if (A.Kind == 9)
		{
			const double HeightCm = (A.ElevationMetres > 0.0 ? A.ElevationMetres : 300.0) * 100.0;
			UInstancedStaticMeshComponent* Layer = GetLayer(TEXT("mountain"), Cone, Style.Color, true);
			Layer->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Mid.X, Mid.Y, HeightCm * 0.5), FVector(SizeCm.X / 100.0, SizeCm.Y / 100.0, HeightCm / 100.0)), true);
			++InstanceCount;
			++Stats.Slabs;
		}
		else
		{
			// flat ground cover; every kind sits a little higher than the previous one so overlaps do not flicker
			AddBox(Style.Key, Style.Color, FVector(Mid.X, Mid.Y, 1.0 + 0.1 * A.Kind), FVector(SizeCm.X, SizeCm.Y, 2.0), FRotator::ZeroRotator, false);
			++Stats.Slabs;
			if (A.Kind == 2 || A.Kind == 3)
			{
				// deterministic trees on a jittered grid, kept away from the roads
				FRandomStream Rand(static_cast<int32>(A.Id) * 7919 + 13);
				const double Spacing = A.Kind == 2 ? 22.0 : 55.0;
				for (double X = Min.X + Spacing * 0.5; X < Max.X; X += Spacing)
				{
					for (double Y = Min.Y + Spacing * 0.5; Y < Max.Y; Y += Spacing)
					{
						const FVector2D P(X + Rand.FRandRange(-0.35, 0.35) * Spacing, Y + Rand.FRandRange(-0.35, 0.35) * Spacing);
						bool bNearRoad = false;
						for (const FSpnGeoRoad& R : Geo.Roads)
						{
							if (DistanceToPolyline(P, R.Points) < RoadWidthMetres(R.Class) * 0.5 + 6.0) { bNearRoad = true; break; }
						}
						if (bNearRoad) { continue; }
						const double H = Rand.FRandRange(7.0, 12.0) * 100.0;
						FVector Base;
						ToWorld(P, 0.0, Base);
						UInstancedStaticMeshComponent* Trunks = GetLayer(TEXT("trunk"), Cyl, FLinearColor(0.10f, 0.06f, 0.03f), true);
						Trunks->AddInstance(FTransform(FRotator::ZeroRotator, Base + FVector(0, 0, H * 0.2), FVector(0.45, 0.45, H * 0.4 / 100.0)), true);
						UInstancedStaticMeshComponent* Crowns = GetLayer(TEXT("crown"), Cone, A.Kind == 2 ? FLinearColor(0.02f, 0.10f, 0.03f) : FLinearColor(0.06f, 0.20f, 0.05f), false);
						Crowns->AddInstance(FTransform(FRotator::ZeroRotator, Base + FVector(0, 0, H * 0.4 + H * 0.35), FVector(H * 0.45 / 100.0, H * 0.45 / 100.0, H * 0.7 / 100.0)), true);
						InstanceCount += 2;
						++Stats.Trees;
					}
				}
			}
		}
	}
}

void ASpnNavDemo::BuildRoads(const FSpnGeometryView& Geo)
{
	for (const FSpnGeoRoad& Road : Geo.Roads)
	{
		const bool bBridge = Road.Structure == 1;
		const bool bTunnel = Road.Structure == 2 || Road.Structure == 3;
		const double WidthCm = RoadWidthMetres(Road.Class) * 100.0;
		const FLinearColor Colour = bTunnel ? FLinearColor(0.035f, 0.035f, 0.04f) : RoadColor(Road.Class);
		double Travelled = 0.0;
		double Total = 0.0;
		for (int32 I = 0; I + 1 < Road.Points.Num(); ++I) { Total += FVector2D::Distance(Road.Points[I], Road.Points[I + 1]); }
		for (int32 I = 0; I + 1 < Road.Points.Num(); ++I)
		{
			FVector A, B;
			if (!ToWorld(Road.Points[I], 0.0, A) || !ToWorld(Road.Points[I + 1], 0.0, B)) { continue; }
			const FVector Delta = B - A;
			const double LenCm = Delta.Size2D();
			if (LenCm < 1.0) { continue; }
			const FRotator Rot(0.0, FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X)), 0.0);
			const FVector Mid = (A + B) * 0.5;
			const FVector Along = Delta.GetSafeNormal2D();
			const FVector Side(-Along.Y, Along.X, 0.0);
			// the slab overlaps its neighbours a little so bends and junctions leave no gap
			const double Extra = FMath::Min(WidthCm * 0.5, 400.0);
			if (bBridge)
			{
				AddBox(TEXT("deck"), FLinearColor(0.12f, 0.12f, 0.13f), FVector(Mid.X, Mid.Y, RoadTopCm - 40.0), FVector(LenCm + Extra, WidthCm, 80.0), Rot);
				AddBox(TEXT("rail"), FLinearColor(0.55f, 0.55f, 0.58f), Mid + Side * (WidthCm * 0.5 - 10.0) + FVector(0, 0, RoadTopCm + 50.0), FVector(LenCm, 20.0, 100.0), Rot);
				AddBox(TEXT("rail"), FLinearColor(0.55f, 0.55f, 0.58f), Mid - Side * (WidthCm * 0.5 - 10.0) + FVector(0, 0, RoadTopCm + 50.0), FVector(LenCm, 20.0, 100.0), Rot);
				// piers every ~40 m down into the trench (they are only visible where the deck spans water)
				const int32 Piers = FMath::Max(1, FMath::FloorToInt(LenCm / 4000.0));
				for (int32 P = 0; P <= Piers; ++P)
				{
					const FVector At = A + Delta * (static_cast<double>(P) / Piers);
					AddBox(TEXT("pier"), FLinearColor(0.30f, 0.30f, 0.32f), FVector(At.X, At.Y, -160.0), FVector(150.0, FMath::Min(WidthCm * 0.6, 600.0), 320.0), Rot);
				}
			}
			else
			{
				AddBox(TEXT("road_") + FString::FromInt(bTunnel ? 99 : Road.Class), Colour, FVector(Mid.X, Mid.Y, RoadTopCm - RoadThicknessCm * 0.5), FVector(LenCm + Extra, WidthCm, RoadThicknessCm), Rot);
			}
			if (bTunnel)
			{
				// closed tube at ground level (the Core has no height model): side walls and a roof, dark inside, a portal frame at the ends
				const FLinearColor Concrete(0.20f, 0.20f, 0.21f);
				AddBox(TEXT("tunnelwall"), Concrete, Mid + Side * (WidthCm * 0.5 + 40.0) + FVector(0, 0, 280.0), FVector(LenCm + Extra, 80.0, 560.0), Rot);
				AddBox(TEXT("tunnelwall"), Concrete, Mid - Side * (WidthCm * 0.5 + 40.0) + FVector(0, 0, 280.0), FVector(LenCm + Extra, 80.0, 560.0), Rot);
				AddBox(TEXT("tunnelroof"), Concrete, Mid + FVector(0, 0, 600.0), FVector(LenCm + Extra, WidthCm + 240.0, 80.0), Rot);
				if (Travelled < 1.0 || Travelled + LenCm * 0.01 >= Total * 0.999)
				{
					// not needed for correctness; kept cheap
				}
			}
			Travelled += LenCm * 0.01;
			++Stats.RoadPieces;
		}
		if (bBridge) { ++Stats.Bridges; }
		if (bTunnel) { ++Stats.Tunnels; }
	}
}

void ASpnNavDemo::BuildBuildings(const FSpnGeometryView& Geo)
{
	for (const FSpnGeoBuilding& B : Geo.Buildings)
	{
		if (B.Footprint.Num() < 3) { continue; }
		FVector2D Min, Max;
		Bounds(B.Footprint, Min, Max);
		FVector C0, C1;
		ToWorld(Min, 0.0, C0);
		ToWorld(Max, 0.0, C1);
		const FVector Mid = (C0 + C1) * 0.5;
		double HeightM = B.HeightMetres;
		FLinearColor Colour(0.46f, 0.36f, 0.28f);
		if (B.Category.Contains(TEXT("warehouse"))) { if (HeightM <= 0.0) { HeightM = 9.0; } Colour = FLinearColor(0.34f, 0.36f, 0.38f); }
		else if (B.Category.Contains(TEXT("shop"))) { if (HeightM <= 0.0) { HeightM = 5.0; } Colour = FLinearColor(0.50f, 0.28f, 0.22f); }
		else { if (HeightM <= 0.0) { HeightM = 6.5; } }
		AddBox(TEXT("building"), Colour, FVector(Mid.X, Mid.Y, HeightM * 50.0), FVector(FMath::Abs(C1.X - C0.X), FMath::Abs(C1.Y - C0.Y), HeightM * 100.0), FRotator::ZeroRotator);
		AddBox(TEXT("roof"), FLinearColor(0.10f, 0.05f, 0.04f), FVector(Mid.X, Mid.Y, HeightM * 100.0 + 15.0), FVector(FMath::Abs(C1.X - C0.X) + 40.0, FMath::Abs(C1.Y - C0.Y) + 40.0, 30.0), FRotator::ZeroRotator, false);
		++Stats.Buildings;
		for (const FVector2D& E : B.Entrances)
		{
			FVector At;
			if (!ToWorld(E, 0.0, At)) { continue; }
			AddBox(TEXT("entrance"), FLinearColor(0.95f, 0.75f, 0.15f), FVector(At.X, At.Y, 110.0), FVector(150.0, 150.0, 220.0), FRotator::ZeroRotator, false);
			++Stats.Entrances;
		}
	}
}

void ASpnNavDemo::BuildCar()
{
	if (CarRoot != nullptr) { return; }
	CarRoot = NewObject<USceneComponent>(this, NAME_None, RF_Transient);
	CarRoot->SetupAttachment(Root);
	CarRoot->RegisterComponent();
	auto Part = [this](UStaticMesh* Mesh, const FLinearColor& Colour, const FVector& Loc, const FVector& ScaleCm, const FRotator& Rot)
	{
		UStaticMeshComponent* C = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
		C->SetStaticMesh(Mesh);
		C->SetupAttachment(CarRoot);
		UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		Mat->SetVectorParameterValue(TEXT("Color"), Colour);
		C->SetMaterial(0, Mat);
		C->SetCollisionProfileName(TEXT("NoCollision"));
		C->SetRelativeLocationAndRotation(Loc, Rot);
		C->SetRelativeScale3D(ScaleCm / 100.0);
		C->RegisterComponent();
		C->SetVisibility(false);
		CarParts.Add(C);
	};
	// placeholder car (boxes): body, cabin, four wheels; local +X is forward, the origin is on the road surface
	Part(CubeMesh, FLinearColor(0.55f, 0.04f, 0.03f), FVector(0, 0, 70), FVector(450, 190, 70), FRotator::ZeroRotator);
	Part(CubeMesh, FLinearColor(0.05f, 0.07f, 0.10f), FVector(-30, 0, 125), FVector(230, 170, 55), FRotator::ZeroRotator);
	for (const double X : {135.0, -135.0})
	{
		for (const double Y : {95.0, -95.0})
		{
			Part(CylinderMesh, FLinearColor(0.01f, 0.01f, 0.01f), FVector(X, Y, 36), FVector(72, 72, 26), FRotator(0, 0, 90));
		}
	}
}

int32 ASpnNavDemo::BuildRegion()
{
	USpnMapSubsystem* M = Map.Get();
	if (M == nullptr || !M->HasRoadNetwork())
	{
		return 0;
	}
	ClearRegion();
	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	ConeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
	BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (CubeMesh == nullptr || CylinderMesh == nullptr || ConeMesh == nullptr || BaseMaterial == nullptr)
	{
		UE_LOG(LogSpnNavDemo, Error, TEXT("NavDemo: engine basic shapes are missing"));
		return 0;
	}
	const FSpnGeometryView Geo = M->GetGeometryView();
	if (!Geo.bValid) { return 0; }
	BuildGround(Geo);
	BuildAreas(Geo);
	BuildRoads(Geo);
	BuildBuildings(Geo);
	BuildCar();
	// waypoint beam: a tall thin pillar and a diamond on top, with a light at its foot
	BeamMesh = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
	BeamMesh->SetStaticMesh(CylinderMesh);
	BeamMesh->SetupAttachment(Root);
	{
		UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.0f, 0.45f, 0.05f));
		BeamMesh->SetMaterial(0, Mat);
	}
	BeamMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BeamMesh->SetCastShadow(false);
	BeamMesh->SetRelativeScale3D(FVector(0.7, 0.7, 90.0));
	BeamMesh->RegisterComponent();
	BeamMesh->SetVisibility(false);
	BeamTop = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
	BeamTop->SetStaticMesh(CubeMesh);
	BeamTop->SetupAttachment(Root);
	BeamTop->SetMaterial(0, BeamMesh->GetMaterial(0));
	BeamTop->SetCollisionProfileName(TEXT("NoCollision"));
	BeamTop->SetCastShadow(false);
	BeamTop->SetRelativeScale3D(FVector(5.0, 5.0, 5.0));
	BeamTop->RegisterComponent();
	BeamTop->SetVisibility(false);
	BeamLight = NewObject<UPointLightComponent>(this, NAME_None, RF_Transient);
	BeamLight->SetupAttachment(Root);
	BeamLight->SetLightColor(FLinearColor(1.0f, 0.5f, 0.1f));
	BeamLight->SetIntensity(80000.f);
	BeamLight->SetAttenuationRadius(2500.f);
	BeamLight->SetCastShadows(false);
	BeamLight->RegisterComponent();
	BeamLight->SetVisibility(false);
	BeamRevision = ~0ull;
	bBuilt = true;
	UE_LOG(LogSpnNavDemo, Display, TEXT("NavDemo: built the synthetic region: %d instances (ground cells %d, road pieces %d, bridges %d, tunnels %d, buildings %d, entrances %d, slabs %d, trees %d, water %d)"),
		InstanceCount, Stats.GroundCells, Stats.RoadPieces, Stats.Bridges, Stats.Tunnels, Stats.Buildings, Stats.Entrances, Stats.Slabs, Stats.Trees, Stats.Water);
	return InstanceCount;
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// lifecycle
// ---------------------------------------------------------------------------------------------------------------------------------------

void ASpnNavDemo::BeginPlay()
{
	Super::BeginPlay();
	UGameInstance* Instance = GetGameInstance();
	USpnMapSubsystem* M = Instance != nullptr ? Instance->GetSubsystem<USpnMapSubsystem>() : nullptr;
	if (M == nullptr)
	{
		return;
	}
	Map = M;
	if (!M->IsSyntheticRegion())
	{
		M->InstallSyntheticRegion();
	}
	BuildRegion();
	M->GetSyntheticPosition(TEXT("westStreet"), PlayerStartMetres);

	// the game flow (HUD, minimap, map screen) is opt-in; this level opts in
	if (USpnSessionSubsystem* Session = Instance->GetSubsystem<USpnSessionSubsystem>())
	{
		Session->EnableGameFlow();
		FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this, Session](float)
		{
			Session->SetupGameplayWorld(GetWorld());
			return false;
		}), 0.5f);
	}
	if (const APlayerController* PC = GetPlayer(); PC != nullptr && PC->GetPawn() != nullptr)
	{
		AutoZCm = static_cast<float>(PC->GetPawn()->GetActorLocation().Z);
	}

	if (FParse::Param(FCommandLine::Get(), TEXT("SpnNavDemoRun")))
	{
		FParse::Value(FCommandLine::Get(), TEXT("SpnNavDemoFlag="), FlagPath);
		FParse::Value(FCommandLine::Get(), TEXT("SpnNavDemoReport="), ReportPath);
		bWaitForFlag = !FlagPath.IsEmpty();
		if (!bWaitForFlag) { StartScript(); }
		else { UE_LOG(LogSpnNavDemo, Display, TEXT("NavDemo: waiting for the start flag %s"), *FlagPath); }
	}
}

void ASpnNavDemo::EndPlay(const EEndPlayReason::Type Reason)
{
	if (OverlayWidget.IsValid() && GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(OverlayWidget.ToSharedRef());
	}
	OverlayWidget.Reset();
	Super::EndPlay(Reason);
}

void ASpnNavDemo::Tick(float Dt)
{
	Super::Tick(Dt);
	if (bWaitForFlag && IFileManager::Get().FileExists(*FlagPath))
	{
		bWaitForFlag = false;
		StartScript();
	}
	UpdateWaypointBeam();
	TickAutopilot(Dt);
	if (bScript) { TickScript(Dt); }
	TickMapAnim(Dt);
	if (bCameraActive) { TickCamera(); }
}

void ASpnNavDemo::UpdateWaypointBeam()
{
	USpnMapSubsystem* M = Map.Get();
	if (M == nullptr || BeamMesh == nullptr) { return; }
	FSpnMapMarkerView Waypoint;
	const bool bHas = M->GetWaypoint(Waypoint);
	if (!bHas)
	{
		if (BeamMesh->IsVisible()) { BeamMesh->SetVisibility(false); BeamTop->SetVisibility(false); BeamLight->SetVisibility(false); }
		BeamMarkerId = 0;
		return;
	}
	FVector At;
	if (!M->MapToUnreal(Waypoint.Metres, 0.f, At)) { return; }
	if (!BeamMesh->IsVisible()) { BeamMesh->SetVisibility(true); BeamTop->SetVisibility(true); BeamLight->SetVisibility(true); }
	BeamMesh->SetWorldLocation(At + FVector(0, 0, 4500.0));
	BeamTop->SetWorldLocation(At + FVector(0, 0, 9200.0));
	BeamTop->SetWorldRotation(FRotator(45.0, GetWorld()->GetTimeSeconds() * 60.0, 45.0));
	BeamLight->SetWorldLocation(At + FVector(0, 0, 400.0));
	BeamMarkerId = Waypoint.Id;
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// autopilot (demo only)
// ---------------------------------------------------------------------------------------------------------------------------------------

void ASpnNavDemo::StartAutopilot(float MetresPerSecond)
{
	AutoMaxSpeed = FMath::Max(MetresPerSecond, 1.f);
	bAutopilot = true;
	AutoGeneration = ~0ull;
	bAutoYawInit = false;
	AutoSpeed = FMath::Min(AutoSpeed, AutoMaxSpeed);
	// chase camera: the character's boom is sized for a walking figure; pull it back and up so the car and the road ahead are in view
	if (APlayerController* PC = GetPlayer(); PC != nullptr && PC->GetPawn() != nullptr)
	{
		if (USpringArmComponent* Arm = PC->GetPawn()->FindComponentByClass<USpringArmComponent>())
		{
			if (!bArmSaved) { SavedArmLength = Arm->TargetArmLength; SavedArmOffset = Arm->SocketOffset; bArmSaved = true; }
			Arm->TargetArmLength = 1150.f;
			Arm->SocketOffset = FVector(0.f, 0.f, 260.f);
		}
	}
	UE_LOG(LogSpnNavDemo, Display, TEXT("NavDemo: autopilot on at %.1f m/s (demo: moves the pawn along the Core route, no vehicle simulation)"), AutoMaxSpeed);
}

void ASpnNavDemo::StopAutopilot()
{
	bAutopilot = false;
	AutoSpeed = 0.f;
	APlayerController* PC = GetPlayer();
	if (APawn* Pawn = PC != nullptr ? PC->GetPawn() : nullptr)
	{
		Pawn->SetActorHiddenInGame(false);
		if (bArmSaved)
		{
			if (USpringArmComponent* Arm = Pawn->FindComponentByClass<USpringArmComponent>()) { Arm->TargetArmLength = SavedArmLength; Arm->SocketOffset = SavedArmOffset; }
		}
	}
	for (UStaticMeshComponent* Part : CarParts) { if (Part != nullptr) { Part->SetVisibility(false); } }
	bCarVisible = false;
}

void ASpnNavDemo::TickAutopilot(float Dt)
{
	USpnMapSubsystem* M = Map.Get();
	APlayerController* PC = GetPlayer();
	APawn* Pawn = PC != nullptr ? PC->GetPawn() : nullptr;
	if (!bAutopilot || M == nullptr || Pawn == nullptr || Dt <= 0.f) { return; }
	FSpnRouteView Route;
	const FSpnNavigationReading Reading = M->GetNavigation(Pawn->GetActorLocation(), 0.f);
	if (!M->GetRoute(Route) || Reading.Status == ESpnNavStatus::Arrived || Reading.Status == ESpnNavStatus::NoRoute)
	{
		AutoSpeed = FMath::FInterpConstantTo(AutoSpeed, 0.f, Dt, 40.f);
		if (Reading.Status == ESpnNavStatus::Arrived && AutoSpeed < 0.5f)
		{
			UE_LOG(LogSpnNavDemo, Display, TEXT("NavDemo: autopilot arrived"));
			bAutopilot = false;
		}
		return;
	}
	if (Route.Generation != AutoGeneration)
	{
		AutoGeneration = Route.Generation;
		AutoPoints = Route.Points;
		AutoCumulative.Reset();
		double Sum = 0.0;
		AutoCumulative.Add(0.0);
		for (int32 I = 1; I < AutoPoints.Num(); ++I) { Sum += FVector2D::Distance(AutoPoints[I - 1], AutoPoints[I]); AutoCumulative.Add(Sum); }
		AutoProgressM = 0.0; // a new route always starts at the vehicle
		UE_LOG(LogSpnNavDemo, Display, TEXT("NavDemo: following a route of %.1f m (generation %llu)"), Route.LengthMetres, Route.Generation);
	}
	if (AutoPoints.Num() < 2) { return; }
	const double Total = AutoCumulative.Last();
	auto PointAt = [&](double S)
	{
		S = FMath::Clamp(S, 0.0, Total);
		int32 I = 1;
		while (I < AutoCumulative.Num() - 1 && AutoCumulative[I] < S) { ++I; }
		const double Seg = AutoCumulative[I] - AutoCumulative[I - 1];
		const double T = Seg > 1e-9 ? (S - AutoCumulative[I - 1]) / Seg : 0.0;
		return FMath::Lerp(AutoPoints[I - 1], AutoPoints[I], T);
	};
	auto HeadingAt = [&](double S)
	{
		const FVector2D A = PointAt(S), B = PointAt(S + 3.0);
		return FMath::Atan2(B.X - A.X, B.Y - A.Y); // compass bearing
	};
	// slow down for bends: the heading change over the next 35 m decides the target speed
	const double Turn = FMath::Abs(FMath::UnwindRadians(HeadingAt(AutoProgressM + 35.0) - HeadingAt(AutoProgressM)));
	const float Target = FMath::Lerp(AutoMaxSpeed, FMath::Min(AutoMaxSpeed, 13.f), static_cast<float>(FMath::Clamp(Turn / 1.0, 0.0, 1.0)));
	AutoSpeed = FMath::FInterpConstantTo(AutoSpeed, Target, Dt, AutoSpeed < Target ? 18.f : 45.f);
	AutoProgressM = FMath::Min(AutoProgressM + AutoSpeed * Dt, Total);

	const FVector2D Here = PointAt(AutoProgressM);
	FVector Location;
	if (!M->MapToUnreal(Here, AutoZCm, Location)) { return; }
	const FVector Ahead3 = [&]
	{
		FVector V;
		M->MapToUnreal(PointAt(AutoProgressM + 6.0), AutoZCm, V);
		return V;
	}();
	const FVector Dir = (Ahead3 - Location).GetSafeNormal2D();
	if (!Dir.IsNearlyZero())
	{
		const float WantedYaw = FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));
		AutoYawDeg = bAutoYawInit ? FMath::FixedTurn(AutoYawDeg, WantedYaw, 220.f * Dt) : WantedYaw;
		bAutoYawInit = true;
	}
	Pawn->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
	Pawn->SetActorRotation(FRotator(0.f, AutoYawDeg, 0.f), ETeleportType::TeleportPhysics);
	if (!bCameraActive)
	{
		PC->SetControlRotation(FRotator(-20.f, AutoYawDeg, 0.f));
	}
	Pawn->SetActorHiddenInGame(true);
	if (CarRoot != nullptr)
	{
		CarRoot->SetWorldLocationAndRotation(Location - FVector(0, 0, AutoZCm - 3.0), FRotator(0.f, AutoYawDeg, 0.f));
		if (!bCarVisible)
		{
			for (UStaticMeshComponent* Part : CarParts) { if (Part != nullptr) { Part->SetVisibility(true); } }
			bCarVisible = true;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// cinematic camera
// ---------------------------------------------------------------------------------------------------------------------------------------

void ASpnNavDemo::SetCameraPath(const TArray<FCamKey>& Keys)
{
	CamKeys = Keys;
}

void ASpnNavDemo::UseCamera(bool bOn)
{
	APlayerController* PC = GetPlayer();
	if (PC == nullptr) { return; }
	if (bOn)
	{
		if (DemoCamera == nullptr)
		{
			FActorSpawnParameters Params;
			Params.ObjectFlags |= RF_Transient;
			DemoCamera = GetWorld()->SpawnActor<ACameraActor>(FVector(0, 0, 5000), FRotator::ZeroRotator, Params);
			DemoCamera->GetCameraComponent()->SetFieldOfView(68.f);
		}
		PC->SetViewTarget(DemoCamera);
		bCameraActive = true;
	}
	else
	{
		bCameraActive = false;
		if (PC->GetPawn() != nullptr) { PC->SetViewTarget(PC->GetPawn()); }
	}
}

void ASpnNavDemo::TickCamera()
{
	if (DemoCamera == nullptr || CamKeys.Num() < 2) { return; }
	const double T = ScriptTime;
	int32 I = 0;
	while (I + 2 < CamKeys.Num() && CamKeys[I + 1].Time <= T) { ++I; }
	const FCamKey& K1 = CamKeys[I];
	const FCamKey& K2 = CamKeys[I + 1];
	const FCamKey& K0 = CamKeys[FMath::Max(I - 1, 0)];
	const FCamKey& K3 = CamKeys[FMath::Min(I + 2, CamKeys.Num() - 1)];
	const double Alpha = FMath::Clamp((T - K1.Time) / FMath::Max(K2.Time - K1.Time, 1e-3), 0.0, 1.0);
	auto Eye = [this](const FCamKey& K) { FVector V = FVector::ZeroVector; ToWorld(K.Metres, K.HeightM * 100.0, V); return V; };
	auto Look = [this](const FCamKey& K) { FVector V = FVector::ZeroVector; ToWorld(K.LookMetres, K.LookHeightM * 100.0, V); return V; };
	const FVector Pos = CatmullRom(Eye(K0), Eye(K1), Eye(K2), Eye(K3), Alpha);
	const FVector Target = CatmullRom(Look(K0), Look(K1), Look(K2), Look(K3), Alpha);
	DemoCamera->SetActorLocationAndRotation(Pos, (Target - Pos).Rotation());
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// overlay (captions, title cards)
// ---------------------------------------------------------------------------------------------------------------------------------------

void ASpnNavDemo::EnsureOverlay()
{
	if (OverlayWidget.IsValid() || GEngine == nullptr || GEngine->GameViewport == nullptr) { return; }
	auto Bold = [](int32 Size) { return FCoreStyle::GetDefaultFontStyle("Bold", Size); };
	auto Regular = [](int32 Size) { return FCoreStyle::GetDefaultFontStyle("Regular", Size); };
	const FSlateBrush* White = FCoreStyle::Get().GetBrush("WhiteBrush");
	TSharedRef<SOverlay> Overlay = SNew(SOverlay).Visibility(EVisibility::HitTestInvisible);

	// lower third caption
	Overlay->AddSlot().HAlign(HAlign_Left).VAlign(VAlign_Bottom).Padding(FMargin(70.f, 0.f, 0.f, 70.f))
	[
		SNew(SBorder).BorderImage(White).Padding(FMargin(30.f, 16.f))
		.BorderBackgroundColor_Lambda([this] { return FLinearColor(0.015f, 0.02f, 0.03f, 0.80f * CaptionAlpha); })
		[
			SNew(SBox)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock).Font(Bold(36)).Text_Lambda([this] { return FText::FromString(CaptionHead); })
					.ColorAndOpacity_Lambda([this] { return FLinearColor(1.f, 0.62f, 0.22f, CaptionAlpha); })
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 6.f, 0.f, 0.f)
				[
					SNew(STextBlock).Font(Regular(24)).WrapTextAt(1500.f).AutoWrapText(true).Text_Lambda([this] { return FText::FromString(CaptionBody); })
					.ColorAndOpacity_Lambda([this] { return FLinearColor(0.93f, 0.95f, 0.97f, CaptionAlpha); })
				]
			]
		]
	];

	// title card
	Overlay->AddSlot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
	[
		SNew(SBorder).BorderImage(White).Padding(FMargin(80.f))
		.BorderBackgroundColor_Lambda([this] { return FLinearColor(0.01f, 0.012f, 0.018f, 0.86f * TitleAlpha); })
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
			[
				SNew(STextBlock).Font(Bold(64)).Justification(ETextJustify::Center).AutoWrapText(true).Text_Lambda([this] { return FText::FromString(TitleA); })
				.ColorAndOpacity_Lambda([this] { return FLinearColor(1.f, 0.62f, 0.22f, TitleAlpha); })
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0.f, 22.f, 0.f, 0.f)
			[
				SNew(STextBlock).Font(Regular(30)).Justification(ETextJustify::Center).AutoWrapText(true).Text_Lambda([this] { return FText::FromString(TitleB); })
				.ColorAndOpacity_Lambda([this] { return FLinearColor(0.93f, 0.95f, 0.97f, TitleAlpha); })
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0.f, 22.f, 0.f, 0.f)
			[
				SNew(STextBlock).Font(Regular(23)).Justification(ETextJustify::Center).AutoWrapText(true).Text_Lambda([this] { return FText::FromString(TitleC); })
				.ColorAndOpacity_Lambda([this] { return FLinearColor(0.72f, 0.78f, 0.84f, TitleAlpha); })
			]
		]
	];

	// permanent badge (top left): this is test data, not the game world
	Overlay->AddSlot().HAlign(HAlign_Left).VAlign(VAlign_Top).Padding(FMargin(36.f, 28.f))
	[
		SNew(SBorder).BorderImage(White).Padding(FMargin(14.f, 6.f)).BorderBackgroundColor(FLinearColor(0.01f, 0.015f, 0.02f, 0.55f))
		[
			SNew(STextBlock).Font(Regular(17)).ColorAndOpacity(FLinearColor(0.80f, 0.85f, 0.9f, 0.95f))
			.Text(FText::FromString(TEXT("TECH DEMO  |  Night Build 03  |  Platzhalterdaten")))
		]
	];
	OverlayWidget = Overlay;
	GEngine->GameViewport->AddViewportWidgetContent(Overlay, 100);
}

void ASpnNavDemo::Caption(const FString& Headline, const FString& Detail, double Seconds)
{
	EnsureOverlay();
	CaptionHead = Headline;
	CaptionBody = Detail;
	CaptionTarget = 1.f;
	CaptionUntil = FPlatformTime::Seconds() + Seconds;
}

void ASpnNavDemo::HideCaption() { CaptionTarget = 0.f; CaptionUntil = 0.0; }

void ASpnNavDemo::Title(const FString& Line1, const FString& Line2, const FString& Line3, double Seconds)
{
	EnsureOverlay();
	TitleA = Line1; TitleB = Line2; TitleC = Line3;
	TitleTarget = 1.f;
	TitleUntil = Seconds > 0.0 ? FPlatformTime::Seconds() + Seconds : 0.0;
}

void ASpnNavDemo::HideTitle() { TitleTarget = 0.f; TitleUntil = 0.0; }

// ---------------------------------------------------------------------------------------------------------------------------------------
// map screen choreography
// ---------------------------------------------------------------------------------------------------------------------------------------

void ASpnNavDemo::AnimateMap(const FVector2D& Center, double MetresPerPixel, double Seconds)
{
	UGameInstance* Instance = GetGameInstance();
	USpnUiSubsystem* Ui = Instance != nullptr ? GetUi() : nullptr;
	USpnMapScreen* Screen = Ui != nullptr ? Ui->GetMapScreen() : nullptr;
	SSpnMapView* View = Screen != nullptr && Screen->GetMapView() != nullptr ? Screen->GetMapView()->GetView() : nullptr;
	if (View == nullptr) { return; }
	MapAnim.bActive = true;
	MapAnim.T = 0.0;
	MapAnim.Duration = FMath::Max(Seconds, 0.05);
	MapAnim.FromCenter = View->GetCenterMetres();
	MapAnim.FromMpp = View->GetMetresPerPixel();
	MapAnim.ToCenter = Center;
	MapAnim.ToMpp = MetresPerPixel;
}

void ASpnNavDemo::TickMapAnim(float)
{
	if (!MapAnim.bActive) { return; }
	UGameInstance* Instance = GetGameInstance();
	USpnUiSubsystem* Ui = Instance != nullptr ? GetUi() : nullptr;
	USpnMapScreen* Screen = Ui != nullptr ? Ui->GetMapScreen() : nullptr;
	SSpnMapView* View = Screen != nullptr && Screen->GetMapView() != nullptr ? Screen->GetMapView()->GetView() : nullptr;
	static double LastWall = 0.0;
	const double Now = FPlatformTime::Seconds();
	const double Dt = LastWall > 0.0 ? FMath::Min(Now - LastWall, 0.1) : 0.016;
	LastWall = Now;
	if (View == nullptr) { MapAnim.bActive = false; return; }
	MapAnim.T += Dt;
	const double A = SmoothStep(MapAnim.T / MapAnim.Duration);
	// log-interpolated zoom keeps the feel even
	const double Mpp = FMath::Exp(FMath::Lerp(FMath::Loge(MapAnim.FromMpp), FMath::Loge(MapAnim.ToMpp), A));
	View->SetViewMetres(FMath::Lerp(MapAnim.FromCenter, MapAnim.ToCenter, A), Mpp);
	if (MapAnim.T >= MapAnim.Duration) { MapAnim.bActive = false; }
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// script
// ---------------------------------------------------------------------------------------------------------------------------------------

void ASpnNavDemo::StartScript()
{
	if (bScript || Map.Get() == nullptr) { return; }
	EnsureOverlay();
	Steps.Reset();
	BuildScript();
	ScriptTime = 0.0;
	bScript = true;
	UE_LOG(LogSpnNavDemo, Display, TEXT("SPN_NAVDEMO_START %s"), *FDateTime::UtcNow().ToIso8601());
}

void ASpnNavDemo::TickScript(float)
{
	static double LastWall = 0.0;
	const double Now = FPlatformTime::Seconds();
	if (LastWall <= 0.0 || !bScript) { LastWall = Now; }
	const double Dt = FMath::Min(Now - LastWall, 0.2);
	LastWall = Now;
	ScriptTime += Dt;
	for (int32 I = 0; I < Steps.Num(); ++I) // index loop: an action may append further steps
	{
		if (!Steps[I].bDone && ScriptTime >= Steps[I].Time)
		{
			Steps[I].bDone = true;
			TFunction<void()> Action = Steps[I].Action;
			Action();
		}
	}
	// fades follow the wall clock (the game is paused while the map is open)
	if (CaptionUntil > 0.0 && Now > CaptionUntil) { CaptionTarget = 0.f; CaptionUntil = 0.0; }
	if (TitleUntil > 0.0 && Now > TitleUntil) { TitleTarget = 0.f; TitleUntil = 0.0; }
	CaptionAlpha = FMath::FInterpConstantTo(CaptionAlpha, CaptionTarget, static_cast<float>(Dt), 3.f);
	TitleAlpha = FMath::FInterpConstantTo(TitleAlpha, TitleTarget, static_cast<float>(Dt), 1.6f);
}
