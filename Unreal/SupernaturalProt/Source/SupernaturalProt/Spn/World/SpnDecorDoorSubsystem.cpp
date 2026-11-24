#include "Spn/World/SpnDecorDoorSubsystem.h"

#include "Dom/JsonObject.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "SupernaturalProt.h"

namespace
{
	constexpr float EdgeTolerance = 20.f;   // cm around the leaf that still counts as "at the door" (aiming is forgiving, like the interaction pick)
	constexpr float MaxViewAngleDegrees = 30.f;
}

bool USpnDecorDoorSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World != nullptr && World->IsGameWorld();
}

FString USpnDecorDoorSubsystem::GetDataPath()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("SpnWorld/Data/SpnWorldDecorDoors.json"));
}

void USpnDecorDoorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	Doors.Reset();
	if (!InWorld.GetMapName().EndsWith(TEXT("Lvl_SpnWorld")))
	{
		return;
	}
	FString Error;
	const int32 Count = Load(GetDataPath(), &Error);
	UE_LOG(LogSupernaturalProt, Log, TEXT("DecorDoors: %d doors of building facades loaded%s"), Count, Error.IsEmpty() ? TEXT("") : *FString::Printf(TEXT(" (%s)"), *Error));
}

int32 USpnDecorDoorSubsystem::Load(const FString& Path, FString* OutError)
{
	Doors.Reset();
	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *Path))
	{
		if (OutError != nullptr) { *OutError = FString::Printf(TEXT("cannot read %s"), *Path); }
		return 0;
	}
	TSharedPtr<FJsonObject> Root;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), Root) || !Root.IsValid())
	{
		if (OutError != nullptr) { *OutError = TEXT("not valid JSON"); }
		return 0;
	}
	const TArray<TSharedPtr<FJsonValue>>* List = nullptr;
	if (Root->GetIntegerField(TEXT("version")) != 1 || !Root->TryGetArrayField(TEXT("doors"), List))
	{
		if (OutError != nullptr) { *OutError = TEXT("unknown version or no door list"); }
		return 0;
	}
	for (const TSharedPtr<FJsonValue>& Value : *List)
	{
		const TSharedPtr<FJsonObject> Object = Value.IsValid() ? Value->AsObject() : nullptr;
		if (!Object.IsValid())
		{
			continue;
		}
		FDoor Door;
		Door.Id = Object->GetStringField(TEXT("id"));
		Door.Centre = FVector(Object->GetNumberField(TEXT("x")), Object->GetNumberField(TEXT("y")), Object->GetNumberField(TEXT("z")));
		Door.Normal = FVector(Object->GetNumberField(TEXT("nx")), Object->GetNumberField(TEXT("ny")), 0.0).GetSafeNormal();
		Door.HalfWidth = static_cast<float>(Object->GetNumberField(TEXT("half_width")));
		Door.Height = static_cast<float>(Object->GetNumberField(TEXT("height")));
		if (!Door.Normal.IsNearlyZero() && Door.HalfWidth > 0.f && Door.Height > 0.f)
		{
			Doors.Add(MoveTemp(Door));
		}
	}
	return Doors.Num();
}

const USpnDecorDoorSubsystem::FDoor* USpnDecorDoorSubsystem::FindDoorInView(const FVector& Eye, const FVector& Direction, float Range) const
{
	const UWorld* World = GetWorld();
	const FVector View = Direction.GetSafeNormal();
	if (World == nullptr || View.IsNearlyZero() || Doors.Num() == 0)
	{
		return nullptr;
	}
	const FDoor* Best = nullptr;
	float BestScore = TNumericLimits<float>::Max();
	for (const FDoor& Door : Doors)
	{
		const FVector ToEye = Eye - Door.Centre;
		const float Outside = static_cast<float>(FVector::DotProduct(ToEye, Door.Normal));
		if (Outside <= 0.f || ToEye.Size() > Range + Door.HalfWidth)
		{
			continue; // behind the facade, or too far
		}
		// where the view meets the door plane, or (looking slightly off) how far the view points away from the leaf's centre
		const float Facing = static_cast<float>(FVector::DotProduct(View, Door.Normal));
		bool bHit = false;
		FVector Point = Door.Centre;
		if (Facing < -0.2f)
		{
			const float T = Outside / -Facing;
			Point = Eye + View * T;
			const FVector Tangent(-Door.Normal.Y, Door.Normal.X, 0.f);
			const float Lateral = FMath::Abs(static_cast<float>(FVector::DotProduct(Point - Door.Centre, Tangent)));
			const float Vertical = FMath::Abs(static_cast<float>(Point.Z - Door.Centre.Z));
			bHit = T <= Range && Lateral <= Door.HalfWidth + EdgeTolerance && Vertical <= Door.Height * 0.5f + EdgeTolerance;
		}
		const float Angle = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(static_cast<float>(FVector::DotProduct(View, (-ToEye).GetSafeNormal())), -1.f, 1.f)));
		if (!bHit && !(Angle <= MaxViewAngleDegrees && ToEye.Size() <= Range))
		{
			continue;
		}
		if (!bHit)
		{
			Point = Door.Centre;
		}
		// nothing between the eyes and the door (a fence, a car, another wall); the facade itself sits at the plane, so stop short of it
		FHitResult Block;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnDecorDoorSight), false);
		if (const APawn* Pawn = World->GetFirstPlayerController() != nullptr ? World->GetFirstPlayerController()->GetPawn() : nullptr)
		{
			Params.AddIgnoredActor(Pawn);
		}
		const FVector Stop = Point + Door.Normal * 15.f;
		if (World->LineTraceSingleByChannel(Block, Eye, Stop, ECC_Visibility, Params))
		{
			continue;
		}
		const float Score = Angle + static_cast<float>(ToEye.Size()) * 0.01f;
		if (Score < BestScore)
		{
			BestScore = Score;
			Best = &Door;
		}
	}
	return Best;
}
