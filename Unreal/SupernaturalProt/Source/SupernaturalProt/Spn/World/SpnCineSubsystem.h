#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpnCineSubsystem.generated.h"

class ACameraActor;

/**
 * Reproducible camera runs through a level for visual evidence and performance sampling (development tool, not a gameplay system).
 * Active only when the game was started with -SpnCine="<file.json>". The file lists shots (camera paths through key points, or a walk of
 * the player character along way points), each with an optional lighting scenario of ASpnWorldLighting.
 *
 *   mode "video" : fixed time step, one PNG per frame into out_dir (frame_000000.png ...), encoded to MP4 afterwards (Tools/spn_visual.ps1)
 *   mode "stills": a few settled single frames per shot (<shot>_<n>.png)
 *   mode "perf"  : real time, no images; frame times, thread times, draw calls per shot into out_dir/perf.json
 *
 * The run ends the game by itself and writes out_dir/cine_done.json. See Docs/Development/Visual-Build-Tools.md for the file format.
 */
UCLASS()
class SUPERNATURALPROT_API USpnCineSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return bActive; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USpnCineSubsystem, STATGROUP_Tickables); }

	struct FKey { FVector P = FVector::ZeroVector; FVector L = FVector::ZeroVector; };
	struct FShot
	{
		FString Name;
		FName Scenario;
		float Duration = 5.f;
		float Fov = 60.f;
		TArray<FKey> Keys;
		TArray<FVector> Walk;
		TArray<float> InteractAt;
		TArray<float> Stills;
		TArray<FString> Console;
		float WalkPitch = -8.f;
		float WalkInput = 1.f;
		bool bSprint = false;
		bool bShowUi = false;
		bool bLinear = false;
		/** Walk shots: the walker opens a closed door once it is its interaction target and close enough, then stands still for DoorPause seconds. */
		bool bOpenDoors = false;
		float DoorPause = 0.9f;
		float DoorDistance = 170.f;
		/** run SpnWorldWarmup::WarmUpView at the start of the shot (what a level start or a loaded save does in the game) */
		bool bWarmup = false;
	};
	struct FFrameStat
	{
		float Ms, Game, Render, Gpu; int32 Draws, Prims;
		/** streaming state of the frame: texture/mesh mip streamer (required and over-budget MB, assets still wanting mips),
		    Nanite streaming quality scale (1 = full quality, below 1 = the Nanite pool is too small), assets still compiling (editor builds) */
		float TexRequiredMB = 0.f, TexOverMB = 0.f, NaniteScale = 1.f; int32 Wanting = 0, Compiling = 0;
	};

	/** Camera pose of a shot at U in [0,1] (arc-length parameterised Catmull-Rom through the keys). Public for the automation test. */
	static void EvalShot(const FShot& Shot, float U, FVector& OutPos, FRotator& OutRot);
	static bool ParseSpec(const FString& JsonText, TArray<FShot>& OutShots, FString& OutDir, FString& OutMode, int32& OutFps, int32& OutWarmup, int32& OutCutWarmup, TArray<FString>& OutConsole, FString& OutError);

private:
	enum class EPhase : uint8 { Warmup, Cut, Run, Hold, Finish, Done };

	void BeginShot(int32 Index);
	void BeginStill();
	void ApplyPose(float U);
	void PlaceWalker();
	void TickWalker(float DeltaTime, float ShotTime);
	void Capture(const FString& FileName, bool bShowUi);
	void FinishShotStats();
	void WriteDone();
	bool IsEngineBusy() const;
	static void SampleStreaming(FFrameStat& Stat);

	bool bActive = false;
	FString OutDir;
	FString Mode;
	int32 Fps = 30;
	int32 WarmupFrames = 120;
	int32 CutWarmupFrames = 45;
	/** spec "cold": true — do not wait for asset/shader compilation before the first shot (measures what a player sees right after loading) */
	bool bCold = false;
	double StartSeconds = 0.0;
	double WarmupSeconds = 0.0;
	float LastPendingTime = 0.f;
	TArray<FShot> Shots;

	EPhase Phase = EPhase::Warmup;
	int32 ShotIndex = 0;
	int32 StillIndex = 0;
	int32 FramesLeft = 0;
	int32 WarmupCount = 0;
	int32 ShotFrame = 0;
	int32 GlobalFrame = 0;
	int32 WalkIndex = 0;
	int32 NextInteract = 0;
	float ShotTime = 0.f;
	float WalkHoldUntil = 0.f;
	TArray<TWeakObjectPtr<AActor>> OpenedDoors;
	FName CurrentScenario = NAME_None;
	TArray<FFrameStat> Stats;
	TArray<TSharedPtr<class FJsonValue>> ShotReports;

	UPROPERTY(Transient)
	TObjectPtr<ACameraActor> Camera;
};
