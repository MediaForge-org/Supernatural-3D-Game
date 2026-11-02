#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpnMapSubsystem.h"

#include "SpnNavDemo.generated.h"

class ACameraActor;
class UInstancedStaticMeshComponent;
class UMaterialInstanceDynamic;
class UPointLightComponent;
class UStaticMesh;
class UStaticMeshComponent;
class SWidget;

// Map & navigation tech demo (NightBuild03, ENGINEERING PROPOSAL, not master canon).
//
// ASpnNavDemo installs the Core's SYNTHETIC test region into USpnMapSubsystem and builds a simplified 3D version of exactly that data
// (USpnMapSubsystem::GetGeometryView): ground, water, roads by class, bridges (deck + rails + piers), tunnels (closed tube at ground level - the
// Core has no height model), building boxes with entrances, forest/park/plaza/parking/industrial slabs, trees, a mountain cone. The map screen,
// the minimap and this world therefore show the same data. It is NOT geography, NOT a WorldForge result and NOT a world-size decision.
//
// DEMO ADAPTER (coordinates): the actor uses the frame already installed in the map subsystem (default: ESpnWorldAxes::EastSouthUp, origin 0,
// 1 map metre = 100 cm, map +X = Unreal +X, map +Y (north) = Unreal -Y, height from the Z value passed by the caller). Every map <-> world
// conversion goes through USpnMapSubsystem::MapToUnreal / UnrealToMap; this class has no private axis convention. It is a replaceable demo
// choice, not a global decision and not part of RC-01.
//
// Not game systems: the "demo autopilot" moves the pawn along the Core route at a set speed (there is no vehicle simulation in the project yet);
// the car shown is a placeholder made of boxes.
UCLASS()
class SUPERNATURALPROT_API ASpnNavDemo : public AActor
{
	GENERATED_BODY()

public:
	ASpnNavDemo();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	/** Builds the 3D region from the map subsystem's geometry (idempotent: clears first). Returns the number of instances created. */
	int32 BuildRegion();

	// --- autopilot (demo only) ---
	void StartAutopilot(float MetresPerSecond);
	void StopAutopilot();
	bool IsAutopilotRunning() const { return bAutopilot; }
	double GetAutopilotDistance() const { return AutoProgressM; }

	// --- scripted video run ---
	void StartScript();
	bool IsScriptRunning() const { return bScript; }

	/** Counts per instance layer after BuildRegion (tests, logs). */
	int32 GetInstanceCount() const { return InstanceCount; }
	/** The synthetic region as built: ground cells, roads, bridges, tunnels, buildings (tests). */
	struct FBuildStats
	{
		int32 GroundCells = 0, RoadPieces = 0, Bridges = 0, Tunnels = 0, Buildings = 0, Entrances = 0, Slabs = 0, Trees = 0, Water = 0;
	};
	const FBuildStats& GetBuildStats() const { return Stats; }
	FVector2D GetPlayerStartMetres() const { return PlayerStartMetres; }

private:
	struct FStep
	{
		double Time = 0.0;
		TFunction<void()> Action;
		bool bDone = false;
	};
	struct FCamKey
	{
		double Time = 0.0;
		FVector2D Metres = FVector2D::ZeroVector; // eye position on the map plane
		double HeightM = 100.0;
		FVector2D LookMetres = FVector2D::ZeroVector;
		double LookHeightM = 0.0;
	};

	// building
	UInstancedStaticMeshComponent* GetLayer(const FString& Key, UStaticMesh* Mesh, const FLinearColor& Color, bool bCollision);
	void AddBox(const FString& Key, const FLinearColor& Color, const FVector& CenterCm, const FVector& SizeCm, const FRotator& Rot, bool bCollision = true);
	bool ToWorld(const FVector2D& Metres, double ZCm, FVector& Out) const;
	void BuildGround(const FSpnGeometryView& Geo);
	void BuildAreas(const FSpnGeometryView& Geo);
	void BuildRoads(const FSpnGeometryView& Geo);
	void BuildBuildings(const FSpnGeometryView& Geo);
	void BuildCar();
	void ClearRegion();

	// per tick
	void UpdateWaypointBeam();
	void TickAutopilot(float Dt);
	void TickScript(float Dt);
	void TickCamera();

	// script helpers
	void BuildScript();
	void Caption(const FString& Headline, const FString& Detail, double Seconds);
	void HideCaption();
	void Title(const FString& Line1, const FString& Line2, const FString& Line3, double Seconds);
	void HideTitle();
	void EnsureOverlay();
	void UseCamera(bool bOn);
	void SetCameraPath(const TArray<FCamKey>& Keys);
	APlayerController* GetPlayer() const;
	class USpnUiSubsystem* GetUi() const;
	void Mark(const FString& Name) const;

	UPROPERTY(Transient) TObjectPtr<USceneComponent> Root;
	UPROPERTY(Transient) TMap<FString, TObjectPtr<UInstancedStaticMeshComponent>> InstLayers;
	UPROPERTY(Transient) TMap<FString, TObjectPtr<UMaterialInstanceDynamic>> Materials;
	UPROPERTY(Transient) TObjectPtr<UStaticMesh> CubeMesh;
	UPROPERTY(Transient) TObjectPtr<UStaticMesh> CylinderMesh;
	UPROPERTY(Transient) TObjectPtr<UStaticMesh> ConeMesh;
	UPROPERTY(Transient) TObjectPtr<UMaterialInterface> BaseMaterial;
	UPROPERTY(Transient) TObjectPtr<UStaticMeshComponent> BeamMesh;
	UPROPERTY(Transient) TObjectPtr<UStaticMeshComponent> BeamTop;
	UPROPERTY(Transient) TObjectPtr<UPointLightComponent> BeamLight;
	UPROPERTY(Transient) TObjectPtr<USceneComponent> CarRoot;
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> CarParts;
	UPROPERTY(Transient) TObjectPtr<ACameraActor> DemoCamera;

	FBuildStats Stats;
	int32 InstanceCount = 0;
	FVector2D PlayerStartMetres = FVector2D::ZeroVector;
	TWeakObjectPtr<USpnMapSubsystem> Map;
	bool bBuilt = false;
	int64 BeamMarkerId = 0;
	uint64 BeamRevision = ~0ull;

	// autopilot
	bool bAutopilot = false;
	float AutoMaxSpeed = 40.f;
	float AutoSpeed = 0.f;
	double AutoProgressM = 0.0;
	uint64 AutoGeneration = ~0ull;
	TArray<FVector2D> AutoPoints;
	TArray<double> AutoCumulative;
	float AutoYawDeg = 0.f;
	bool bAutoYawInit = false;
	float AutoZCm = 100.f;
	bool bCarVisible = false;
	bool bArmSaved = false;
	float SavedArmLength = 400.f;
	FVector SavedArmOffset = FVector::ZeroVector;

	// script
	bool bScript = false;
	double ScriptTime = 0.0;
	TArray<FStep> Steps;
	TArray<FCamKey> CamKeys;
	bool bCameraActive = false;
	bool bWaitForFlag = false;
	FString FlagPath;
	FString ReportPath;
	double RouteInitialM = 0.0;
	double StraightInitialM = 0.0;
	double LengthBefore = 0.0;
	double LengthAfter = 0.0;
	FString OutroDetail;

	// overlay
	TSharedPtr<SWidget> OverlayWidget;
	FString CaptionHead, CaptionBody, TitleA, TitleB, TitleC;
	float CaptionAlpha = 0.f, CaptionTarget = 0.f, TitleAlpha = 0.f, TitleTarget = 0.f;
	double CaptionUntil = 0.0, TitleUntil = 0.0;
	double OverlayClock = 0.0;

	// map-screen choreography
	struct FMapAnim
	{
		bool bActive = false;
		double T = 0.0, Duration = 1.0;
		FVector2D FromCenter, ToCenter;
		double FromMpp = 1.0, ToMpp = 1.0;
	} MapAnim;
	void AnimateMap(const FVector2D& Center, double MetresPerPixel, double Seconds);
	void TickMapAnim(float Dt);
};
