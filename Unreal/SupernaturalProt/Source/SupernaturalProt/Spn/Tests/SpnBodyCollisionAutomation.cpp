#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Misc/AutomationTest.h"
#include "Spn/Character/SpnCharacterVisualComponent.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// Body collision profiles (NightBuild06, Core BodyCollision.h): a preset with Collision = FromHeight gives the character a capsule that fits
// its body, without moving anything the player sees or the floor contact: same feet, same mesh position, same camera and eye height as the
// same body with the default capsule. Default presets keep the capsule (gameplay unchanged). Headless.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FCollisionWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		ASupernaturalProtCharacter* Player = nullptr;

		FCollisionWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class != nullptr)
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				Player = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(FVector(0.f, 0.f, 200.f)), Params);
			}
		}

		/** A blocking 1 m cube whose bottom face is at BottomZ, centred over the player. */
		AStaticMeshActor* SpawnCeiling(float BottomZ) const
		{
			UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
			if (Cube == nullptr) { return nullptr; }
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FTransform(FVector(0.f, 0.f, BottomZ + 50.f)), Params);
			Block->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			Block->GetStaticMeshComponent()->SetStaticMesh(Cube);
			Block->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("BlockAll"));
			return Block;
		}

		~FCollisionWorld()
		{
			if (IsValid(Player)) { Player->Destroy(); }
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
			GameInstance->RemoveFromRoot();
		}
	};

	struct FPose
	{
		float FeetZ, MeshZ, CameraZ, EyeZ, Radius, Half, Crouched;
	};

	FPose Measure(const ASupernaturalProtCharacter& P)
	{
		const FSpnBodyCollisionProfile C = P.GetBodyCollision();
		const float CentreZ = static_cast<float>(P.GetActorLocation().Z);
		return {CentreZ - C.HalfHeight, static_cast<float>(P.GetMesh()->GetComponentLocation().Z), static_cast<float>(P.GetCameraBoom()->GetComponentLocation().Z),
			CentreZ + P.BaseEyeHeight, C.Radius, C.HalfHeight, C.CrouchedHalfHeight};
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnBodyCollisionFitTest, "SpnGame.Character.BodyCollisionFollowsTheBodyOnlyWhenAsked", Flags)
bool FSpnBodyCollisionFitTest::RunTest(const FString&)
{
	FCollisionWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USpnCharacterVisualComponent* Visual = W.Player->GetVisual();
	if (!TestNotNull(TEXT("visual component"), Visual)) { return false; }

	const FPose Start = Measure(*W.Player);
	const FVector MeshRelative = W.Player->GetMesh()->GetRelativeLocation();
	const FVector CameraBase = W.Player->GetCameraBase();
	AddInfo(FString::Printf(TEXT("default capsule %.1f / %.1f, crouched %.1f, feet z %.1f, eye %.1f"), Start.Radius, Start.Half, Start.Crouched, Start.FeetZ, W.Player->BaseEyeHeight));

	for (const float Height : {160.f, 195.f})
	{
		const FName Plain(*FString::Printf(TEXT("test_%.0f"), Height));
		const FName Fit(*FString::Printf(TEXT("test_%.0f_fit"), Height));

		// the plain preset keeps the capsule (today's gameplay)
		TestTrue(*FString::Printf(TEXT("%s applies"), *Plain.ToString()), Visual->ApplyPreset(Plain));
		const FPose PlainPose = Measure(*W.Player);
		TestTrue(*FString::Printf(TEXT("%s: capsule unchanged (%.1f / %.1f)"), *Plain.ToString(), PlainPose.Radius, PlainPose.Half),
			FMath::IsNearlyEqual(PlainPose.Radius, Start.Radius) && FMath::IsNearlyEqual(PlainPose.Half, Start.Half) && FMath::IsNearlyEqual(PlainPose.Crouched, Start.Crouched));

		// the fit preset: capsule from the body height, everything the player sees stays where the plain preset put it
		TestTrue(*FString::Printf(TEXT("%s applies"), *Fit.ToString()), Visual->ApplyPreset(Fit));
		TestFalse(*FString::Printf(TEXT("%s: the character accepted the capsule"), *Fit.ToString()), Visual->WasCollisionRefused());
		const FPose FitPose = Measure(*W.Player);
		const float ExpectedHalf = (Visual->GetBodyHeightCm() + Visual->CollisionHeadClearanceCm) * 0.5f;
		AddInfo(FString::Printf(TEXT("%s: body %.1f cm -> capsule %.1f / %.1f, crouched %.1f; head above capsule %.1f cm (plain preset: %.1f)"), *Fit.ToString(),
			Visual->GetBodyHeightCm(), FitPose.Radius, FitPose.Half, FitPose.Crouched, Visual->GetLastCollision().HeadAboveCapsule, Visual->GetBodyHeightCm() - 2.f * Start.Half));
		TestTrue(*FString::Printf(TEXT("%s: half height %.2f = (body + clearance) / 2 = %.2f"), *Fit.ToString(), FitPose.Half, ExpectedHalf), FMath::IsNearlyEqual(FitPose.Half, ExpectedHalf, 0.05f));
		TestTrue(*FString::Printf(TEXT("%s: radius stays %.1f"), *Fit.ToString(), Start.Radius), FMath::IsNearlyEqual(FitPose.Radius, Start.Radius));
		TestTrue(*FString::Printf(TEXT("%s: crouch scales with the half height (%.2f)"), *Fit.ToString(), FitPose.Crouched),
			FMath::IsNearlyEqual(FitPose.Crouched, Start.Crouched * FitPose.Half / Start.Half, 0.05f));
		TestTrue(*FString::Printf(TEXT("%s: the head is inside the capsule (%.1f cm above it)"), *Fit.ToString(), Visual->GetLastCollision().HeadAboveCapsule),
			Visual->GetLastCollision().HeadAboveCapsule <= 0.f);
		TestTrue(*FString::Printf(TEXT("%s: feet stay (%.2f vs %.2f)"), *Fit.ToString(), FitPose.FeetZ, PlainPose.FeetZ), FMath::IsNearlyEqual(FitPose.FeetZ, PlainPose.FeetZ, 0.01f));
		TestTrue(*FString::Printf(TEXT("%s: mesh stays on the capsule bottom (%.2f vs %.2f)"), *Fit.ToString(), FitPose.MeshZ, PlainPose.MeshZ), FMath::IsNearlyEqual(FitPose.MeshZ, PlainPose.MeshZ, 0.01f));
		TestTrue(*FString::Printf(TEXT("%s: camera height above the floor unchanged (%.2f vs %.2f)"), *Fit.ToString(), FitPose.CameraZ, PlainPose.CameraZ), FMath::IsNearlyEqual(FitPose.CameraZ, PlainPose.CameraZ, 0.01f));
		TestTrue(*FString::Printf(TEXT("%s: eye height above the floor unchanged (%.2f vs %.2f)"), *Fit.ToString(), FitPose.EyeZ, PlainPose.EyeZ), FMath::IsNearlyEqual(FitPose.EyeZ, PlainPose.EyeZ, 0.01f));
	}

	// back to the blueprint: exactly the spawned values
	TestTrue(TEXT("none restores the blueprint"), Visual->ApplyPreset(NAME_None));
	const FPose End = Measure(*W.Player);
	TestTrue(TEXT("restored capsule"), FMath::IsNearlyEqual(End.Radius, Start.Radius) && FMath::IsNearlyEqual(End.Half, Start.Half) && FMath::IsNearlyEqual(End.Crouched, Start.Crouched));
	TestTrue(*FString::Printf(TEXT("restored feet (%.2f vs %.2f)"), End.FeetZ, Start.FeetZ), FMath::IsNearlyEqual(End.FeetZ, Start.FeetZ, 0.01f));
	TestTrue(TEXT("restored mesh offset"), W.Player->GetMesh()->GetRelativeLocation().Equals(MeshRelative, 0.01));
	TestTrue(TEXT("restored camera base"), W.Player->GetCameraBase().Equals(CameraBase, 0.01));
	TestTrue(*FString::Printf(TEXT("restored eye height (%.2f vs %.2f)"), End.EyeZ, Start.EyeZ), FMath::IsNearlyEqual(End.EyeZ, Start.EyeZ, 0.01f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnBodyCollisionRoomTest, "SpnGame.Character.BodyCollisionNeedsRoomAndAStandingCharacter", Flags)
bool FSpnBodyCollisionRoomTest::RunTest(const FString&)
{
	FCollisionWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USpnCharacterVisualComponent* Visual = W.Player->GetVisual();
	const FSpnBodyCollisionProfile Default = W.Player->GetBodyCollision();
	const float CapsuleTop = static_cast<float>(W.Player->GetActorLocation().Z) + Default.HalfHeight;

	// a ceiling 3 cm above today's capsule: the 195 cm body needs about 8.5 cm more, so the capsule must not grow into it
	AStaticMeshActor* Ceiling = W.SpawnCeiling(CapsuleTop + 3.f);
	if (!TestNotNull(TEXT("ceiling spawned"), Ceiling)) { return false; }
	TestTrue(TEXT("test_195 (plain) applies under the ceiling"), Visual->ApplyPreset(TEXT("test_195")));
	const float PlainCameraZ = static_cast<float>(W.Player->GetCameraBoom()->GetComponentLocation().Z);
	TestTrue(TEXT("test_195_fit: the presentation applies"), Visual->ApplyPreset(TEXT("test_195_fit")));
	TestTrue(TEXT("test_195_fit: the larger capsule is refused under the ceiling"), Visual->WasCollisionRefused());
	TestTrue(TEXT("refused: the capsule is unchanged"), W.Player->GetBodyCollision().Equals(Default));
	TestTrue(TEXT("refused: the camera is where the plain preset puts it"),
		FMath::IsNearlyEqual(static_cast<float>(W.Player->GetCameraBoom()->GetComponentLocation().Z), PlainCameraZ, 0.01f));

	// shrinking always fits, even under the ceiling
	TestTrue(TEXT("test_160_fit applies under the ceiling"), Visual->ApplyPreset(TEXT("test_160_fit")));
	TestFalse(TEXT("test_160_fit: a smaller capsule is never refused for room"), Visual->WasCollisionRefused());
	TestTrue(TEXT("test_160_fit: capsule smaller than the default"), W.Player->GetBodyCollision().HalfHeight < Default.HalfHeight);

	// without the ceiling the large one fits
	Ceiling->Destroy();
	TestTrue(TEXT("test_195_fit applies without the ceiling"), Visual->ApplyPreset(TEXT("test_195_fit")));
	TestFalse(TEXT("test_195_fit: accepted without the ceiling"), Visual->WasCollisionRefused());
	TestTrue(TEXT("test_195_fit: capsule taller than the default"), W.Player->GetBodyCollision().HalfHeight > Default.HalfHeight);

	// a crouched character never changes its standing capsule (the crouch code owns the capsule then)
	const FSpnBodyCollisionProfile Before = W.Player->GetBodyCollision();
	W.Player->bIsCrouched = true;
	FSpnBodyCollisionProfile Other = Before;
	Other.HalfHeight = Before.HalfHeight + 5.f;
	TestFalse(TEXT("crouched: ApplyBodyCollision refuses"), W.Player->ApplyBodyCollision(Other));
	TestTrue(TEXT("crouched: capsule unchanged"), W.Player->GetBodyCollision().Equals(Before));
	W.Player->bIsCrouched = false;
	TestTrue(TEXT("standing again: none restores the default capsule"), Visual->ApplyPreset(NAME_None) && W.Player->GetBodyCollision().Equals(Default));
	return true;
}

#endif
