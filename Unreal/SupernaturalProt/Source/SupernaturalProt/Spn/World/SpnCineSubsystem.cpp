#include "Spn/World/SpnCineSubsystem.h"

#include "AssetCompilingManager.h"
#include "ContentStreaming.h"
#include "Rendering/NaniteStreamingManager.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Dom/JsonObject.h"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformMemory.h"
#include "Misc/App.h"
#include "Misc/CommandLine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "RenderTimer.h"
#include "RHI.h"
#include "RHIStats.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "ShaderCompiler.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/World/SpnWorldLighting.h"
#include "Spn/World/SpnWorldWarmup.h"
#include "SupernaturalProtCharacter.h"
#include "UnrealClient.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpnCine, Display, All);

namespace
{
	FVector ReadVec(const TArray<TSharedPtr<FJsonValue>>& A)
	{
		return FVector(A.Num() > 0 ? A[0]->AsNumber() : 0.0, A.Num() > 1 ? A[1]->AsNumber() : 0.0, A.Num() > 2 ? A[2]->AsNumber() : 0.0);
	}

	FVector CatmullRom(const FVector& P0, const FVector& P1, const FVector& P2, const FVector& P3, double T)
	{
		const double T2 = T * T, T3 = T2 * T;
		return 0.5 * ((2.0 * P1) + (-P0 + P2) * T + (2.0 * P0 - 5.0 * P1 + 4.0 * P2 - P3) * T2 + (-P0 + 3.0 * P1 - 3.0 * P2 + P3) * T3);
	}

	/** F in [0, N-1]: position on the spline through all points. */
	template <typename TGet>
	FVector SplineAt(int32 Num, double F, TGet Get)
	{
		if (Num == 1) { return Get(0); }
		const int32 I = FMath::Clamp(FMath::FloorToInt32(F), 0, Num - 2);
		const double T = FMath::Clamp(F - I, 0.0, 1.0);
		return CatmullRom(Get(FMath::Max(I - 1, 0)), Get(I), Get(I + 1), Get(FMath::Min(I + 2, Num - 1)), T);
	}

	float Percentile(TArray<float>& Sorted, float P)
	{
		return Sorted.Num() == 0 ? 0.f : Sorted[FMath::Clamp(FMath::RoundToInt32(P * (Sorted.Num() - 1)), 0, Sorted.Num() - 1)];
	}
}

bool USpnCineSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool USpnCineSubsystem::ParseSpec(const FString& JsonText, TArray<FShot>& OutShots, FString& OutDir, FString& OutMode, int32& OutFps, int32& OutWarmup, int32& OutCutWarmup, TArray<FString>& OutConsole, FString& OutError)
{
	TSharedPtr<FJsonObject> Root;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonText), Root) || !Root.IsValid())
	{
		OutError = TEXT("not valid JSON");
		return false;
	}
	OutDir = Root->GetStringField(TEXT("out_dir"));
	OutMode = Root->HasField(TEXT("mode")) ? Root->GetStringField(TEXT("mode")) : TEXT("video");
	OutFps = Root->HasField(TEXT("fps")) ? Root->GetIntegerField(TEXT("fps")) : 30;
	OutWarmup = Root->HasField(TEXT("warmup_frames")) ? Root->GetIntegerField(TEXT("warmup_frames")) : 120;
	OutCutWarmup = Root->HasField(TEXT("cut_warmup_frames")) ? Root->GetIntegerField(TEXT("cut_warmup_frames")) : 45;
	const TArray<TSharedPtr<FJsonValue>>* Array = nullptr;
	if (Root->TryGetArrayField(TEXT("console"), Array))
	{
		for (const TSharedPtr<FJsonValue>& V : *Array) { OutConsole.Add(V->AsString()); }
	}
	if (!Root->TryGetArrayField(TEXT("shots"), Array) || Array->Num() == 0)
	{
		OutError = TEXT("no shots");
		return false;
	}
	for (const TSharedPtr<FJsonValue>& V : *Array)
	{
		const TSharedPtr<FJsonObject> O = V->AsObject();
		if (!O.IsValid()) { OutError = TEXT("shot is not an object"); return false; }
		FShot S;
		S.Name = O->GetStringField(TEXT("name"));
		FString Scenario;
		if (O->TryGetStringField(TEXT("scenario"), Scenario)) { S.Scenario = FName(*Scenario); }
		double Number = 0.0;
		if (O->TryGetNumberField(TEXT("duration"), Number)) { S.Duration = static_cast<float>(Number); }
		if (O->TryGetNumberField(TEXT("fov"), Number)) { S.Fov = static_cast<float>(Number); }
		if (O->TryGetNumberField(TEXT("walk_pitch"), Number)) { S.WalkPitch = static_cast<float>(Number); }
		if (O->TryGetNumberField(TEXT("walk_input"), Number)) { S.WalkInput = static_cast<float>(Number); }
		O->TryGetBoolField(TEXT("sprint"), S.bSprint);
		O->TryGetBoolField(TEXT("ui"), S.bShowUi);
		O->TryGetBoolField(TEXT("linear"), S.bLinear);
		O->TryGetBoolField(TEXT("open_doors"), S.bOpenDoors);
		O->TryGetBoolField(TEXT("warmup"), S.bWarmup);
		if (O->TryGetNumberField(TEXT("door_pause"), Number)) { S.DoorPause = static_cast<float>(Number); }
		if (O->TryGetNumberField(TEXT("door_distance"), Number)) { S.DoorDistance = static_cast<float>(Number); }
		const TArray<TSharedPtr<FJsonValue>>* Sub = nullptr;
		if (O->TryGetArrayField(TEXT("keys"), Sub))
		{
			for (const TSharedPtr<FJsonValue>& K : *Sub)
			{
				const TSharedPtr<FJsonObject> KO = K->AsObject();
				if (!KO.IsValid() || !KO->HasField(TEXT("p")) || !KO->HasField(TEXT("l"))) { OutError = S.Name + TEXT(": key needs p and l"); return false; }
				FKey Key;
				Key.P = ReadVec(KO->GetArrayField(TEXT("p")));
				Key.L = ReadVec(KO->GetArrayField(TEXT("l")));
				S.Keys.Add(Key);
			}
		}
		if (O->TryGetArrayField(TEXT("walk"), Sub))
		{
			for (const TSharedPtr<FJsonValue>& W : *Sub) { S.Walk.Add(ReadVec(W->AsArray())); }
		}
		if (O->TryGetArrayField(TEXT("interact_at"), Sub))
		{
			for (const TSharedPtr<FJsonValue>& W : *Sub) { S.InteractAt.Add(static_cast<float>(W->AsNumber())); }
		}
		if (O->TryGetArrayField(TEXT("stills"), Sub))
		{
			for (const TSharedPtr<FJsonValue>& W : *Sub) { S.Stills.Add(static_cast<float>(W->AsNumber())); }
		}
		if (O->TryGetArrayField(TEXT("console"), Sub))
		{
			for (const TSharedPtr<FJsonValue>& W : *Sub) { S.Console.Add(W->AsString()); }
		}
		if (S.Keys.Num() == 0 && S.Walk.Num() < 2) { OutError = S.Name + TEXT(": needs keys or a walk of two points"); return false; }
		if (S.Stills.Num() == 0) { S.Stills = {0.f, 0.5f, 1.f}; }
		OutShots.Add(MoveTemp(S));
	}
	return true;
}

void USpnCineSubsystem::EvalShot(const FShot& Shot, float U, FVector& OutPos, FRotator& OutRot)
{
	const int32 Num = Shot.Keys.Num();
	if (Num == 0) { return; }
	U = FMath::Clamp(U, 0.f, 1.f);
	if (!Shot.bLinear) { U = FMath::InterpEaseInOut(0.f, 1.f, U, 1.6f); }
	double F = 0.0;
	if (Num > 1)
	{
		// arc-length table: equal U steps cover equal distances, so the camera speed does not depend on the key spacing
		constexpr int32 Samples = 96;
		double Lengths[Samples + 1];
		Lengths[0] = 0.0;
		FVector Prev = Shot.Keys[0].P;
		for (int32 I = 1; I <= Samples; ++I)
		{
			const FVector P = SplineAt(Num, static_cast<double>(I) / Samples * (Num - 1), [&Shot](int32 K) { return Shot.Keys[K].P; });
			Lengths[I] = Lengths[I - 1] + FVector::Dist(Prev, P);
			Prev = P;
		}
		const double Wanted = U * Lengths[Samples];
		int32 I = 1;
		while (I < Samples && Lengths[I] < Wanted) { ++I; }
		const double Span = Lengths[I] - Lengths[I - 1];
		const double Local = Span > KINDA_SMALL_NUMBER ? (Wanted - Lengths[I - 1]) / Span : 0.0;
		F = Lengths[Samples] > KINDA_SMALL_NUMBER ? (I - 1 + Local) / Samples * (Num - 1) : U * (Num - 1);
	}
	OutPos = SplineAt(Num, F, [&Shot](int32 K) { return Shot.Keys[K].P; });
	const FVector Look = SplineAt(Num, F, [&Shot](int32 K) { return Shot.Keys[K].L; });
	OutRot = (Look - OutPos).Rotation();
	OutRot.Roll = 0.f;
}

void USpnCineSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	FString SpecPath;
	if (!FParse::Value(FCommandLine::Get(), TEXT("SpnCine="), SpecPath))
	{
		return;
	}
	FString Text, Error;
	TArray<FString> Console;
	if (!FFileHelper::LoadFileToString(Text, *SpecPath) || !ParseSpec(Text, Shots, OutDir, Mode, Fps, WarmupFrames, CutWarmupFrames, Console, Error))
	{
		UE_LOG(LogSpnCine, Error, TEXT("SpnCine: cannot use '%s': %s"), *SpecPath, *Error);
		FPlatformMisc::RequestExitWithStatus(false, 3);
		return;
	}
	IFileManager::Get().MakeDirectory(*OutDir, true);
	Fps = FMath::Clamp(Fps, 1, 240);
	{
		TSharedPtr<FJsonObject> Root;
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), Root) && Root.IsValid())
		{
			Root->TryGetBoolField(TEXT("cold"), bCold);
		}
	}
	StartSeconds = FPlatformTime::Seconds();
	if (Mode != TEXT("perf"))
	{
		FApp::SetUseFixedTimeStep(true);
		FApp::SetFixedDeltaTime(1.0 / Fps);
	}
	for (const FString& Command : Console)
	{
		GEngine->Exec(&InWorld, *Command);
	}
	Camera = InWorld.SpawnActor<ACameraActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (Camera != nullptr)
	{
		Camera->GetCameraComponent()->bConstrainAspectRatio = false;
	}
	bActive = Camera != nullptr;
	Phase = EPhase::Warmup;
	WarmupCount = 0;
	UE_LOG(LogSpnCine, Display, TEXT("SpnCine: %d shots, mode %s, %d fps, out %s"), Shots.Num(), *Mode, Fps, *OutDir);
}

bool USpnCineSubsystem::IsEngineBusy() const
{
	if (bCold) { return false; }   // a cold run shows what is still loading or compiling
	return FAssetCompilingManager::Get().GetNumRemainingAssets() > 0 || (GShaderCompilingManager != nullptr && GShaderCompilingManager->IsCompiling());
}

void USpnCineSubsystem::SampleStreaming(FFrameStat& Stat)
{
	constexpr float MB = 1024.f * 1024.f;
	if (IStreamingManager::Get().IsRenderAssetStreamingEnabled(EStreamableRenderAssetType::None))
	{
		const IRenderAssetStreamingManager& Streamer = IStreamingManager::Get().GetRenderAssetStreamingManager();
		Stat.TexRequiredMB = static_cast<float>(Streamer.GetRequiredPoolSize()) / MB;
		Stat.TexOverMB = static_cast<float>(Streamer.GetMemoryOverBudget()) / MB;
		Stat.Wanting = Streamer.GetNumWantingResources();
	}
	Stat.NaniteScale = Nanite::GStreamingManager.GetQualityScaleFactor();
	Stat.Compiling = FAssetCompilingManager::Get().GetNumRemainingAssets() + (GShaderCompilingManager != nullptr ? GShaderCompilingManager->GetNumRemainingJobs() : 0);
}

void USpnCineSubsystem::ApplyPose(float U)
{
	const FShot& Shot = Shots[ShotIndex];
	if (Shot.Keys.Num() == 0 || Camera == nullptr) { return; }
	FVector Pos; FRotator Rot;
	EvalShot(Shot, U, Pos, Rot);
	Camera->SetActorLocationAndRotation(Pos, Rot);
	Camera->GetCameraComponent()->SetFieldOfView(Shot.Fov);
}

void USpnCineSubsystem::PlaceWalker()
{
	const FShot& Shot = Shots[ShotIndex];
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ACharacter* Walker = PC != nullptr ? Cast<ACharacter>(PC->GetPawn()) : nullptr;
	if (Walker == nullptr) { return; }
	FVector Start = Shot.Walk[0];
	FHitResult Hit;
	FCollisionQueryParams Params(FName(TEXT("SpnCineGround")), false, Walker);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start + FVector(0, 0, 400), Start - FVector(0, 0, 5000), ECC_Visibility, Params))
	{
		Start.Z = Hit.ImpactPoint.Z;
	}
	Start.Z += Walker->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 4.f;
	const float Yaw = (Shot.Walk[1] - Shot.Walk[0]).Rotation().Yaw;
	Walker->SetActorLocationAndRotation(Start, FRotator(0.f, Yaw, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
	Walker->GetCharacterMovement()->StopMovementImmediately();
	PC->SetControlRotation(FRotator(Shot.WalkPitch, Yaw, 0.f));
	if (ASupernaturalProtCharacter* Spn = Cast<ASupernaturalProtCharacter>(Walker))
	{
		Spn->ResetStairCamera();
	}
	WalkIndex = 1;
	NextInteract = 0;
	WalkHoldUntil = 0.f;
	OpenedDoors.Reset();
}

void USpnCineSubsystem::TickWalker(float DeltaTime, float Time)
{
	const FShot& Shot = Shots[ShotIndex];
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ACharacter* Walker = PC != nullptr ? Cast<ACharacter>(PC->GetPawn()) : nullptr;
	if (Walker == nullptr) { return; }
	ASupernaturalProtCharacter* Spn = Cast<ASupernaturalProtCharacter>(Walker);
	if (Spn != nullptr && NextInteract < Shot.InteractAt.Num() && Time >= Shot.InteractAt[NextInteract])
	{
		const USpnInteractionComponent* Interaction = Spn->FindComponentByClass<USpnInteractionComponent>();
		UE_LOG(LogSpnCine, Display, TEXT("SpnCine: interact at %.2f s, target %s"), Time, *GetNameSafe(Interaction != nullptr ? Interaction->FindTarget() : nullptr));
		Spn->DoInteract();
		++NextInteract;
	}
	if (Spn != nullptr && Shot.bOpenDoors && Time >= WalkHoldUntil)
	{
		// position based instead of timed: the walker presses Interact when a closed door is what it looks at from close by, then lets it swing
		const USpnInteractionComponent* Interaction = Spn->FindComponentByClass<USpnInteractionComponent>();
		ASpnTestDoor* Door = Interaction != nullptr ? Cast<ASpnTestDoor>(Interaction->FindTarget()) : nullptr;
		if (Door != nullptr && !Door->IsOpen() && !OpenedDoors.Contains(Door))
		{
			FVector Origin, Extent;
			Door->GetActorBounds(/*bOnlyCollidingComponents*/ true, Origin, Extent);
			if (FVector::Dist2D(Walker->GetActorLocation(), Origin) <= Shot.DoorDistance)
			{
				Spn->DoInteract();
				OpenedDoors.Add(Door);
				WalkHoldUntil = Time + Shot.DoorPause;
				UE_LOG(LogSpnCine, Display, TEXT("SpnCine: walker used door %s at %.2f s, now %s"), *Door->GetName(), Time, Door->IsOpen() ? TEXT("open") : TEXT("still closed"));
			}
		}
	}
	if (Time < WalkHoldUntil)
	{
		return;   // stands in front of the door while it swings (the leaf never pushes a pawn, it would wait)
	}
	while (WalkIndex < Shot.Walk.Num() && FVector::Dist2D(Walker->GetActorLocation(), Shot.Walk[WalkIndex]) < 70.f)
	{
		++WalkIndex;
	}
	if (WalkIndex >= Shot.Walk.Num()) { return; }
	const FVector Dir = (Shot.Walk[WalkIndex] - Walker->GetActorLocation()).GetSafeNormal2D();
	Walker->AddMovementInput(Dir, Shot.WalkInput);
	const FRotator Wanted(Shot.WalkPitch, Dir.Rotation().Yaw, 0.f);
	PC->SetControlRotation(FMath::RInterpTo(PC->GetControlRotation(), Wanted, DeltaTime, 2.2f));
}

void USpnCineSubsystem::BeginShot(int32 Index)
{
	ShotIndex = Index;
	const FShot& Shot = Shots[ShotIndex];
	UWorld* World = GetWorld();
	APlayerController* PC = World->GetFirstPlayerController();
	FramesLeft = CutWarmupFrames;
	if (!Shot.Scenario.IsNone() && Shot.Scenario != CurrentScenario)
	{
		if (ASpnWorldLighting* Lighting = ASpnWorldLighting::Find(World))
		{
			Lighting->ApplyScenario(Shot.Scenario);
			FramesLeft += 120;   // sky light capture, Lumen and exposure settle after a change of light
		}
		CurrentScenario = Shot.Scenario;
	}
	for (const FString& Command : Shot.Console)
	{
		GEngine->Exec(World, *Command);
	}
	const bool bWalk = Shot.Walk.Num() >= 2;
	if (PC != nullptr)
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->SetActorHiddenInGame(!bWalk);
			if (ASupernaturalProtCharacter* Spn = Cast<ASupernaturalProtCharacter>(Pawn))
			{
				if (bWalk && Shot.bSprint) { Spn->DoSprintStart(); } else { Spn->DoSprintEnd(); }
			}
		}
		if (bWalk)
		{
			PlaceWalker();
			PC->SetViewTarget(PC->GetPawn());
		}
		else
		{
			PC->SetViewTarget(Camera);
		}
	}
	ShotFrame = 0;
	ShotTime = 0.f;
	StillIndex = 0;
	Stats.Reset();
	LastPendingTime = 0.f;
	ApplyPose(Mode == TEXT("stills") ? Shot.Stills[0] : 0.f);
	if (Shot.bWarmup)
	{
		const APawn* Pawn = PC != nullptr ? PC->GetPawn() : nullptr;
		SpnWorldWarmup::WarmUpView(World, bWalk && Pawn != nullptr ? Pawn->GetActorLocation() : Camera->GetActorLocation());
	}
	Phase = EPhase::Cut;
	UE_LOG(LogSpnCine, Display, TEXT("SpnCine: shot %d/%d '%s' (frame %d)"), ShotIndex + 1, Shots.Num(), *Shot.Name, GlobalFrame);
}

void USpnCineSubsystem::Capture(const FString& FileName, bool bShowUi)
{
	FScreenshotRequest::RequestScreenshot(OutDir / FileName, bShowUi, /*bAddFilenameSuffix*/ false);
}

void USpnCineSubsystem::FinishShotStats()
{
	const FShot& Shot = Shots[ShotIndex];
	TSharedRef<FJsonObject> O = MakeShared<FJsonObject>();
	O->SetStringField(TEXT("name"), Shot.Name);
	O->SetStringField(TEXT("scenario"), CurrentScenario.ToString());
	O->SetNumberField(TEXT("first_frame"), GlobalFrame - ShotFrame);
	O->SetNumberField(TEXT("frames"), ShotFrame);
	if (Mode == TEXT("perf") && Stats.Num() > 0)
	{
		TArray<float> Ms;
		double SumMs = 0, Game = 0, Render = 0, Gpu = 0, Draws = 0, Prims = 0;
		for (const FFrameStat& S : Stats)
		{
			Ms.Add(S.Ms); SumMs += S.Ms; Game += S.Game; Render += S.Render; Gpu += S.Gpu; Draws += S.Draws; Prims += S.Prims;
		}
		float ReqMax = 0.f, OverMax = 0.f, NaniteMin = 1.f; int32 OverFrames = 0, WantMax = 0, CompileMax = 0, PendingFrames = 0;
		for (const FFrameStat& S : Stats)
		{
			ReqMax = FMath::Max(ReqMax, S.TexRequiredMB); OverMax = FMath::Max(OverMax, S.TexOverMB); NaniteMin = FMath::Min(NaniteMin, S.NaniteScale);
			OverFrames += S.TexOverMB > 0.f ? 1 : 0; WantMax = FMath::Max(WantMax, S.Wanting); CompileMax = FMath::Max(CompileMax, S.Compiling);
			PendingFrames += (S.Wanting > 0 || S.Compiling > 0) ? 1 : 0;
		}
		Ms.Sort();
		const double N = Stats.Num();
		O->SetNumberField(TEXT("samples"), N);
		O->SetNumberField(TEXT("avg_fps"), SumMs > 0 ? 1000.0 * N / SumMs : 0.0);
		O->SetNumberField(TEXT("avg_ms"), SumMs / N);
		O->SetNumberField(TEXT("p50_ms"), Percentile(Ms, 0.50f));
		O->SetNumberField(TEXT("p95_ms"), Percentile(Ms, 0.95f));
		O->SetNumberField(TEXT("p99_ms"), Percentile(Ms, 0.99f));
		O->SetNumberField(TEXT("max_ms"), Ms.Last());
		O->SetNumberField(TEXT("game_ms"), Game / N);
		O->SetNumberField(TEXT("render_ms"), Render / N);
		O->SetNumberField(TEXT("gpu_ms"), Gpu / N);
		O->SetNumberField(TEXT("draw_calls"), Draws / N);
		O->SetNumberField(TEXT("primitives"), Prims / N);
		O->SetNumberField(TEXT("tex_required_max_mb"), ReqMax);
		O->SetNumberField(TEXT("tex_over_budget_max_mb"), OverMax);
		O->SetNumberField(TEXT("tex_over_budget_share"), OverFrames / N);
		O->SetNumberField(TEXT("streaming_wanting_max"), WantMax);
		O->SetNumberField(TEXT("streaming_pending_share"), PendingFrames / N);
		O->SetNumberField(TEXT("streaming_settled_after_s"), LastPendingTime);
		O->SetNumberField(TEXT("compiling_max"), CompileMax);
		O->SetNumberField(TEXT("nanite_quality_min"), NaniteMin);
	}
	ShotReports.Add(MakeShared<FJsonValueObject>(O));
}

void USpnCineSubsystem::WriteDone()
{
	TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
	Root->SetStringField(TEXT("mode"), Mode);
	Root->SetNumberField(TEXT("fps"), Fps);
	Root->SetNumberField(TEXT("frames"), GlobalFrame);
	Root->SetStringField(TEXT("map"), GetWorld()->GetMapName());
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		FVector2D Size;
		GEngine->GameViewport->GetViewportSize(Size);
		Root->SetNumberField(TEXT("width"), Size.X);
		Root->SetNumberField(TEXT("height"), Size.Y);
	}
	const FPlatformMemoryStats Memory = FPlatformMemory::GetStats();
	Root->SetNumberField(TEXT("ram_used_mb"), Memory.UsedPhysical / (1024.0 * 1024.0));
	Root->SetNumberField(TEXT("ram_peak_mb"), Memory.PeakUsedPhysical / (1024.0 * 1024.0));
	FTextureMemoryStats Texture;
	RHIGetTextureMemoryStats(Texture);
	Root->SetNumberField(TEXT("vram_dedicated_mb"), Texture.DedicatedVideoMemory / (1024.0 * 1024.0));
	Root->SetNumberField(TEXT("texture_streaming_mb"), Texture.StreamingMemorySize / (1024.0 * 1024.0));
	Root->SetNumberField(TEXT("texture_nonstreaming_mb"), Texture.NonStreamingMemorySize / (1024.0 * 1024.0));
	if (IStreamingManager::Get().IsRenderAssetStreamingEnabled(EStreamableRenderAssetType::None))
	{
		const IRenderAssetStreamingManager& Streamer = IStreamingManager::Get().GetRenderAssetStreamingManager();
		Root->SetNumberField(TEXT("tex_pool_mb"), Streamer.GetPoolSize() / (1024.0 * 1024.0));
		Root->SetNumberField(TEXT("tex_max_ever_required_mb"), Streamer.GetMaxEverRequired() / (1024.0 * 1024.0));
	}
	if (const IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite.Streaming.StreamingPoolSize")))
	{
		Root->SetNumberField(TEXT("nanite_pool_mb"), Var->GetInt());
	}
	Root->SetBoolField(TEXT("cold"), bCold);
	Root->SetNumberField(TEXT("warmup_s"), WarmupSeconds);
	Root->SetArrayField(TEXT("shots"), ShotReports);
	FString Text;
	FJsonSerializer::Serialize(Root, TJsonWriterFactory<>::Create(&Text));
	FFileHelper::SaveStringToFile(Text, *(OutDir / (Mode == TEXT("perf") ? TEXT("perf.json") : TEXT("cine_done.json"))));
}

void USpnCineSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bActive || Shots.Num() == 0) { return; }
	const bool bPerf = Mode == TEXT("perf");
	const bool bStills = Mode == TEXT("stills");

	switch (Phase)
	{
	case EPhase::Warmup:
	{
		if (WarmupCount == 0)
		{
			// first pose and light already during the warm-up, so streaming, Lumen and the exposure settle on what the first shot shows
			BeginShot(0);
			Phase = EPhase::Warmup;
		}
		++WarmupCount;
		const bool bLongEnough = WarmupCount >= WarmupFrames;
		if ((bLongEnough && !IsEngineBusy()) || WarmupCount > 20000)
		{
			WarmupSeconds = FPlatformTime::Seconds() - StartSeconds;
			FramesLeft = CutWarmupFrames;
			Phase = EPhase::Cut;
		}
		break;
	}
	case EPhase::Cut:
	{
		if (--FramesLeft <= 0 && !IsEngineBusy())
		{
			Phase = EPhase::Run;
		}
		break;
	}
	case EPhase::Run:
	{
		const FShot& Shot = Shots[ShotIndex];
		const bool bWalk = Shot.Walk.Num() >= 2;
		if (bStills)
		{
			Capture(FString::Printf(TEXT("%s_%d.png"), *Shot.Name, StillIndex), Shot.bShowUi);
			++GlobalFrame; ++ShotFrame; ++StillIndex;
			Phase = EPhase::Hold;   // the image is taken at the end of this frame: do not move the camera before the next tick
			break;
		}
		const int32 TotalFrames = FMath::Max(1, FMath::RoundToInt32(Shot.Duration * Fps));
		const float Time = bPerf ? ShotTime : static_cast<float>(ShotFrame) / Fps;
		if (bWalk) { TickWalker(DeltaTime, Time); }
		ApplyPose(Shot.Duration > 0.f ? Time / Shot.Duration : 0.f);
		if (bPerf)
		{
			if (ShotFrame >= (bCold ? 0 : 20))   // the first frames after a cut still load (a cold run keeps them: they are what it measures)
			{
				FFrameStat Stat{DeltaTime * 1000.f, FPlatformTime::ToMilliseconds(GGameThreadTime), FPlatformTime::ToMilliseconds(GRenderThreadTime),
					FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0)), GNumDrawCallsRHI[0], GNumPrimitivesDrawnRHI[0]};
				SampleStreaming(Stat);
				if (Stat.Wanting > 0 || Stat.Compiling > 0) { LastPendingTime = ShotTime; }
				Stats.Add(Stat);
			}
			ShotTime += DeltaTime;
			++ShotFrame; ++GlobalFrame;
			if (ShotTime >= Shot.Duration) { Phase = EPhase::Hold; }
		}
		else
		{
			Capture(FString::Printf(TEXT("frame_%06d.png"), GlobalFrame), Shot.bShowUi);
			++ShotFrame; ++GlobalFrame;
			if (ShotFrame >= TotalFrames) { Phase = EPhase::Hold; }
		}
		break;
	}
	case EPhase::Hold:
	{
		const FShot& Shot = Shots[ShotIndex];
		if (bStills && StillIndex < Shot.Stills.Num() && Shot.Walk.Num() < 2)
		{
			ApplyPose(Shot.Stills[StillIndex]);
			FramesLeft = CutWarmupFrames;
			Phase = EPhase::Cut;
			break;
		}
		FinishShotStats();
		if (ShotIndex + 1 < Shots.Num())
		{
			BeginShot(ShotIndex + 1);
		}
		else
		{
			FramesLeft = 40;   // let the last image be written
			Phase = EPhase::Finish;
		}
		break;
	}
	case EPhase::Finish:
	{
		if (--FramesLeft <= 0)
		{
			WriteDone();
			UE_LOG(LogSpnCine, Display, TEXT("SpnCine: finished, %d frames"), GlobalFrame);
			Phase = EPhase::Done;
			bActive = false;
			FPlatformMisc::RequestExit(false);
		}
		break;
	}
	default:
		break;
	}
}
