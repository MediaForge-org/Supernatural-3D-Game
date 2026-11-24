#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpnMetaHumanPrepLibrary.generated.h"

/**
 * Editor-only preparation helpers for MetaHuman TEST bodies (Protagonist V1 phase 2, 2026-11-15). Uses only the public, exported API of the
 * engine's MetaHumanCharacterEditor module (UMetaHumanCharacterEditorSubsystem::TryGenerateCharacterAssets): it generates the body / face
 * skeletal meshes of a MetaHuman Character asset LOCALLY from its current state (no auto-rig, no texture download, no cloud) and saves them
 * as ordinary assets under a folder of our choice. In a non-editor build every function reports "editor only".
 * Called from Python (Tools/character_mh_male_setup.py). Nothing here is used by the game at runtime.
 */
UCLASS()
class SUPERNATURALPROT_API USpnMetaHumanPrepLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Generates and saves the meshes (and the materials / textures they reference) of a MetaHuman Character asset into Folder
	 * (e.g. /Game/SpnCharacterPrep/MetaHumanTest/MaleTall). Character must be a UMetaHumanCharacter. Returns a JSON object: saved asset
	 * paths, body measurements (cm) when bMeasurements, and "error" on failure.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|CharacterPrep")
	static FString GenerateMetaHumanMeshes(UObject* Character, const FString& Folder, bool bMeasurements);

	/** Face rig state of a MetaHuman Character ("Unrigged" / "RigPending" / "Rigged", public subsystem API), opened for editing if needed. */
	UFUNCTION(BlueprintCallable, Category = "Spn|CharacterPrep")
	static FString GetMetaHumanRigState(UObject* Character);

	/**
	 * Compares two skeletal meshes vertex by vertex on LOD 0 (render data): vertex count, max / mean position difference (cm), max UV
	 * difference (channel 0) and whether the index buffers are identical. JSON. Used to prove a round trip keeps topology, order and UVs.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|CharacterPrep")
	static FString CompareSkeletalMeshes(UObject* MeshA, UObject* MeshB);

	/**
	 * Order-independent compare on LOD 0: every vertex of B is matched to the vertex of A with the same UV (channel 0, quantised 1e-5; at UV
	 * seams the nearest of the candidates), then the position difference is measured. JSON: matched share, unmatched UVs, max / mean / p99
	 * distance (cm). Proves "same topology + same UVs + same shape" even when the render data orders the vertices differently.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|CharacterPrep")
	static FString CompareSkeletalMeshesByUV(UObject* MeshA, UObject* MeshB);

	/**
	 * Shape compare independent of order AND UVs on LOD 0 section 0 (the head skin): both point sets are moved onto their centroids, then
	 * every vertex of B is matched to its nearest vertex of A (spatial hash). JSON: mean / p95 / max distance (cm) and the RMS radius of
	 * both (scale). Small distances = the same head shape.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|CharacterPrep")
	static FString CompareHeadShapes(UObject* MeshA, UObject* MeshB);
};
