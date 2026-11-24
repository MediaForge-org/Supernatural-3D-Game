#include "Spn/World/SpnWorldBuildLibrary.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"

#if WITH_EDITOR
#include "Landscape.h"
#include "LandscapeInfo.h"
#include "LandscapeLayerInfoObject.h"
#include "LandscapeProxy.h"
#include "LandscapeUtils.h"
#endif

ALandscape* USpnWorldBuildLibrary::CreateLandscapeFromRaw(UObject* WorldContextObject, const FString& HeightFile, int32 ComponentsX, int32 ComponentsY,
	int32 SectionsPerComponent, int32 QuadsPerSection, FVector Centre, FVector Scale, UMaterialInterface* Material, const TArray<FName>& LayerNames,
	const TArray<FString>& LayerFiles, const FString& LayerInfoPath)
{
#if WITH_EDITOR
	UWorld* World = GEngine != nullptr ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (World == nullptr || ComponentsX < 1 || ComponentsY < 1 || LayerNames.Num() != LayerFiles.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("SpnWorldBuild: bad arguments"));
		return nullptr;
	}
	const int32 QuadsPerComponent = SectionsPerComponent * QuadsPerSection;
	const int32 SizeX = ComponentsX * QuadsPerComponent + 1;
	const int32 SizeY = ComponentsY * QuadsPerComponent + 1;

	TArray<uint8> Bytes;
	if (!FFileHelper::LoadFileToArray(Bytes, *HeightFile) || Bytes.Num() != SizeX * SizeY * 2)
	{
		UE_LOG(LogTemp, Error, TEXT("SpnWorldBuild: height file '%s' has %d bytes, expected %d"), *HeightFile, Bytes.Num(), SizeX * SizeY * 2);
		return nullptr;
	}
	TArray<uint16> Heights;
	Heights.SetNumUninitialized(SizeX * SizeY);
	FMemory::Memcpy(Heights.GetData(), Bytes.GetData(), Bytes.Num());

	TArray<FLandscapeImportLayerInfo> ImportLayers;
	for (int32 I = 0; I < LayerNames.Num(); ++I)
	{
		FLandscapeImportLayerInfo Layer(LayerNames[I]);
		if (!FFileHelper::LoadFileToArray(Layer.LayerData, *LayerFiles[I]) || Layer.LayerData.Num() != SizeX * SizeY)
		{
			UE_LOG(LogTemp, Error, TEXT("SpnWorldBuild: layer file '%s' has %d bytes, expected %d"), *LayerFiles[I], Layer.LayerData.Num(), SizeX * SizeY);
			return nullptr;
		}
		Layer.LayerInfo = UE::Landscape::CreateTargetLayerInfo(LayerNames[I], LayerInfoPath, LayerNames[I].ToString() + TEXT("_LayerInfo"));
		ImportLayers.Add(MoveTemp(Layer));
	}

	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;
	HeightDataPerLayers.Add(FGuid(), MoveTemp(Heights));
	MaterialLayerDataPerLayers.Add(FGuid(), ImportLayers);

	const FVector Offset = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, Scale).TransformVector(FVector(-ComponentsX * QuadsPerComponent / 2.0, -ComponentsY * QuadsPerComponent / 2.0, 0.0));
	ALandscape* Landscape = World->SpawnActor<ALandscape>(Centre + Offset, FRotator::ZeroRotator);
	if (Landscape == nullptr)
	{
		return nullptr;
	}
	Landscape->LandscapeMaterial = Material;
	Landscape->SetActorRelativeScale3D(Scale);
	Landscape->StaticLightingLOD = 0;
	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX - 1, SizeY - 1, SectionsPerComponent, QuadsPerSection, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers,
		ELandscapeImportAlphamapType::Additive, TArrayView<const FLandscapeLayer>());

	ULandscapeInfo* Info = Landscape->GetLandscapeInfo();
	if (Info != nullptr)
	{
		Info->UpdateLayerInfoMap(Landscape);
		for (const FLandscapeImportLayerInfo& Layer : ImportLayers)
		{
			if (Layer.LayerInfo != nullptr)
			{
				Landscape->AddTargetLayer(Layer.LayerName, FLandscapeTargetLayerSettings(Layer.LayerInfo));
				const int32 Index = Info->GetLayerInfoIndex(Layer.LayerName);
				if (Index != INDEX_NONE)
				{
					Info->Layers[Index].LayerInfoObj = Layer.LayerInfo;
				}
			}
		}
	}
	Landscape->SetActorLabel(TEXT("SPN_Landscape"));
	Landscape->ForceUpdateLayersContent(/*bIntermediateRender*/ false);
	Landscape->PostEditChange();
	Landscape->MarkPackageDirty();
	UE_LOG(LogTemp, Display, TEXT("SpnWorldBuild: landscape %dx%d samples, %d layers"), SizeX, SizeY, ImportLayers.Num());
	return Landscape;
#else
	return nullptr;
#endif
}
