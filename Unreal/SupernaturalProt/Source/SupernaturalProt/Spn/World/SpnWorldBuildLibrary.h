#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpnWorldBuildLibrary.generated.h"

class ALandscape;
class UMaterialInterface;

/**
 * Editor-only helpers for the world generator scripts (Unreal/SupernaturalProt/Tools/spn_world/): operations the Python API does not offer.
 * Every function does nothing and returns null outside the editor. Not gameplay code.
 */
UCLASS()
class SUPERNATURALPROT_API USpnWorldBuildLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a landscape in the current editor world from raw files written by the terrain tool.
	 * HeightFile: little-endian uint16, row-major, (ComponentsX*QuadsPerComponent+1) x (ComponentsY*QuadsPerComponent+1) samples; 32768 = actor Z.
	 * LayerFiles: one uint8 weight file per layer name, same resolution; the weights of a sample should add up to 255.
	 * Centre: world position of the middle of the landscape. Scale: cm per quad (X, Y) and Z scale (100 = 256 m of range below and above).
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|World Build", meta = (WorldContext = "WorldContextObject"))
	static ALandscape* CreateLandscapeFromRaw(UObject* WorldContextObject, const FString& HeightFile, int32 ComponentsX, int32 ComponentsY, int32 SectionsPerComponent,
		int32 QuadsPerSection, FVector Centre, FVector Scale, UMaterialInterface* Material, const TArray<FName>& LayerNames, const TArray<FString>& LayerFiles,
		const FString& LayerInfoPath);
};
