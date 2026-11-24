#include "Spn/Character/SpnMetaHumanPrepLibrary.h"

#include "Dom/JsonObject.h"
#include "Engine/SkeletalMesh.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "MetaHumanCharacter.h"
#include "MetaHumanCharacterEditorSubsystem.h"
#include "MetaHumanCharacterGeneratedAssets.h"
#include "UObject/Package.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpnMetaHumanPrepLibrary)

namespace
{
	FString ToJson(const TSharedRef<FJsonObject>& Object)
	{
		FString Out;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
		FJsonSerializer::Serialize(Object, Writer);
		return Out;
	}
}

FString USpnMetaHumanPrepLibrary::GenerateMetaHumanMeshes(UObject* Character, const FString& Folder, bool bMeasurements)
{
	const TSharedRef<FJsonObject> Result = MakeShared<FJsonObject>();
#if WITH_EDITOR
	UMetaHumanCharacter* MetaHuman = Cast<UMetaHumanCharacter>(Character);
	UMetaHumanCharacterEditorSubsystem* Subsystem = GEditor != nullptr ? GEditor->GetEditorSubsystem<UMetaHumanCharacterEditorSubsystem>() : nullptr;
	if (MetaHuman == nullptr || Subsystem == nullptr || !Folder.StartsWith(TEXT("/Game/")))
	{
		Result->SetStringField(TEXT("error"), TEXT("needs a MetaHumanCharacter, the editor subsystem and a /Game/ folder"));
		return ToJson(Result);
	}
	if (!Subsystem->IsObjectAddedForEditing(MetaHuman) && !Subsystem->TryAddObjectToEdit(MetaHuman))
	{
		Result->SetStringField(TEXT("error"), TEXT("could not open the character for editing"));
		return ToJson(Result);
	}

	FMetaHumanCharacterGeneratedAssetOptions Options;
	Options.bGenerateFaceMesh = true;
	Options.bGenerateBodyMesh = true;
	Options.bGenerateBodyMeasurements = bMeasurements;
	FMetaHumanCharacterGeneratedAssets Generated;
	if (!Subsystem->TryGenerateCharacterAssets(MetaHuman, GetTransientPackage(), Options, Generated))
	{
		Result->SetStringField(TEXT("error"), TEXT("TryGenerateCharacterAssets failed (see LogMetaHumanCharacterEditor)"));
		return ToJson(Result);
	}

	// every generated object goes into its own package under Folder/<subfolder>/<name>, as the engine's own unpack step does
	TArray<UPackage*> Packages;
	TArray<TSharedPtr<FJsonValue>> Saved;
	for (const FMetaHumanGeneratedAssetMetadata& Meta : Generated.Metadata)
	{
		UObject* Object = Meta.Object;
		if (Object == nullptr || Meta.PreferredName.IsEmpty())
		{
			continue;
		}
		const FString PackageName = (Meta.PreferredSubfolderPath.IsEmpty() ? Folder : Folder / Meta.PreferredSubfolderPath) / Meta.PreferredName;
		UPackage* Package = CreatePackage(*PackageName);
		Package->FullyLoad();
		// a re-run replaces the asset of the previous run: move the old object out of the way (no redirector, nothing references it but us)
		if (UObject* Existing = StaticFindObject(UObject::StaticClass(), Package, *Meta.PreferredName))
		{
			Existing->ClearFlags(RF_Public | RF_Standalone);
			Existing->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors | REN_NonTransactional);
		}
		Object->Rename(*Meta.PreferredName, Package, REN_DontCreateRedirectors | REN_NonTransactional);
		Object->ClearFlags(RF_Transient);
		Object->SetFlags(RF_Public | RF_Standalone);
		FAssetRegistryModule::AssetCreated(Object);
		Package->MarkPackageDirty();
		Packages.Add(Package);
		Saved.Add(MakeShared<FJsonValueString>(Object->GetPathName()));
	}
	const bool bSaved = UEditorLoadingAndSavingUtils::SavePackages(Packages, /*bOnlyDirty*/ false);
	Result->SetBoolField(TEXT("saved_ok"), bSaved);
	Result->SetArrayField(TEXT("assets"), Saved);
	Result->SetStringField(TEXT("body_mesh"), Generated.BodyMesh != nullptr ? Generated.BodyMesh->GetPathName() : FString());
	Result->SetStringField(TEXT("face_mesh"), Generated.FaceMesh != nullptr ? Generated.FaceMesh->GetPathName() : FString());
	const TSharedRef<FJsonObject> Measures = MakeShared<FJsonObject>();
	for (const TPair<FString, float>& Pair : Generated.BodyMeasurements) { Measures->SetNumberField(Pair.Key, Pair.Value); }
	Result->SetObjectField(TEXT("body_measurements"), Measures);
#else
	Result->SetStringField(TEXT("error"), TEXT("editor only"));
#endif
	return ToJson(Result);
}

FString USpnMetaHumanPrepLibrary::GetMetaHumanRigState(UObject* Character)
{
#if WITH_EDITOR
	UMetaHumanCharacter* MetaHuman = Cast<UMetaHumanCharacter>(Character);
	UMetaHumanCharacterEditorSubsystem* Subsystem = GEditor != nullptr ? GEditor->GetEditorSubsystem<UMetaHumanCharacterEditorSubsystem>() : nullptr;
	if (MetaHuman == nullptr || Subsystem == nullptr) { return TEXT("invalid"); }
	if (!Subsystem->IsObjectAddedForEditing(MetaHuman) && !Subsystem->TryAddObjectToEdit(MetaHuman)) { return TEXT("not editable"); }
	switch (Subsystem->GetRiggingState(MetaHuman))
	{
	case EMetaHumanCharacterRigState::Rigged: return TEXT("Rigged");
	case EMetaHumanCharacterRigState::RigPending: return TEXT("RigPending");
	default: return TEXT("Unrigged");
	}
#else
	return TEXT("editor only");
#endif
}

FString USpnMetaHumanPrepLibrary::CompareSkeletalMeshes(UObject* MeshA, UObject* MeshB)
{
	const TSharedRef<FJsonObject> Result = MakeShared<FJsonObject>();
	USkeletalMesh* A = Cast<USkeletalMesh>(MeshA);
	USkeletalMesh* B = Cast<USkeletalMesh>(MeshB);
	const FSkeletalMeshRenderData* RA = A != nullptr ? A->GetResourceForRendering() : nullptr;
	const FSkeletalMeshRenderData* RB = B != nullptr ? B->GetResourceForRendering() : nullptr;
	if (RA == nullptr || RB == nullptr || RA->LODRenderData.IsEmpty() || RB->LODRenderData.IsEmpty())
	{
		Result->SetStringField(TEXT("error"), TEXT("needs two skeletal meshes with render data"));
		return ToJson(Result);
	}
	const FSkeletalMeshLODRenderData& LA = RA->LODRenderData[0];
	const FSkeletalMeshLODRenderData& LB = RB->LODRenderData[0];
	const uint32 NA = LA.StaticVertexBuffers.PositionVertexBuffer.GetNumVertices();
	const uint32 NB = LB.StaticVertexBuffers.PositionVertexBuffer.GetNumVertices();
	Result->SetNumberField(TEXT("vertices_a"), NA);
	Result->SetNumberField(TEXT("vertices_b"), NB);
	Result->SetNumberField(TEXT("sections_a"), LA.RenderSections.Num());
	Result->SetNumberField(TEXT("sections_b"), LB.RenderSections.Num());
	if (NA == NB)
	{
		double MaxD = 0.0, SumD = 0.0, MaxUv = 0.0;
		const bool bUv = LA.StaticVertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords() > 0 && LB.StaticVertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords() > 0;
		for (uint32 i = 0; i < NA; ++i)
		{
			const double D = FVector3f::Dist(LA.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i), LB.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i));
			MaxD = FMath::Max(MaxD, D);
			SumD += D;
			if (bUv)
			{
				const FVector2f Ua = LA.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
				const FVector2f Ub = LB.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
				MaxUv = FMath::Max(MaxUv, static_cast<double>(FVector2f::Distance(Ua, Ub)));
			}
		}
		Result->SetNumberField(TEXT("max_position_diff_cm"), MaxD);
		Result->SetNumberField(TEXT("mean_position_diff_cm"), NA > 0 ? SumD / NA : 0.0);
		Result->SetNumberField(TEXT("max_uv0_diff"), bUv ? MaxUv : -1.0);
	}
	TArray<uint32> IA, IB;
	LA.MultiSizeIndexContainer.GetIndexBuffer(IA);
	LB.MultiSizeIndexContainer.GetIndexBuffer(IB);
	Result->SetNumberField(TEXT("indices_a"), IA.Num());
	Result->SetNumberField(TEXT("indices_b"), IB.Num());
	Result->SetBoolField(TEXT("index_buffers_identical"), IA == IB);
	return ToJson(Result);
}

namespace
{
	/** Vertex positions of LOD 0 render section 0 (for a MetaHuman face mesh: the head skin). */
	TArray<FVector3f> HeadSection(USkeletalMesh* Mesh)
	{
		TArray<FVector3f> Out;
		const FSkeletalMeshRenderData* R = Mesh != nullptr ? Mesh->GetResourceForRendering() : nullptr;
		if (R == nullptr || R->LODRenderData.IsEmpty() || R->LODRenderData[0].RenderSections.IsEmpty()) { return Out; }
		const FSkeletalMeshLODRenderData& L = R->LODRenderData[0];
		const FSkelMeshRenderSection& S = L.RenderSections[0];
		for (uint32 i = 0; i < S.NumVertices; ++i) { Out.Add(L.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(S.BaseVertexIndex + i)); }
		return Out;
	}
}

FString USpnMetaHumanPrepLibrary::CompareHeadShapes(UObject* MeshA, UObject* MeshB)
{
	const TSharedRef<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<FVector3f> A = HeadSection(Cast<USkeletalMesh>(MeshA));
	TArray<FVector3f> B = HeadSection(Cast<USkeletalMesh>(MeshB));
	if (A.IsEmpty() || B.IsEmpty())
	{
		Result->SetStringField(TEXT("error"), TEXT("needs two skeletal meshes with a LOD 0 section 0"));
		return ToJson(Result);
	}
	auto Centre = [](TArray<FVector3f>& P)
	{
		FVector3f C = FVector3f::ZeroVector;
		for (const FVector3f& V : P) { C += V; }
		C /= static_cast<float>(P.Num());
		double R2 = 0.0;
		for (FVector3f& V : P) { V -= C; R2 += V.SizeSquared(); }
		return FMath::Sqrt(R2 / P.Num());
	};
	const double RadiusA = Centre(A), RadiusB = Centre(B);
	constexpr float Cell = 1.0f;
	TMultiMap<FIntVector, int32> Grid;
	auto CellOf = [](const FVector3f& V) { return FIntVector(FMath::FloorToInt(V.X / Cell), FMath::FloorToInt(V.Y / Cell), FMath::FloorToInt(V.Z / Cell)); };
	for (int32 i = 0; i < A.Num(); ++i) { Grid.Add(CellOf(A[i]), i); }
	// second pass: B scaled to A's RMS radius (shape only, size removed)
	TArray<FVector3f> BScaled = B;
	const float ScaleToA = RadiusB > 1.0e-3 ? static_cast<float>(RadiusA / RadiusB) : 1.f;
	for (FVector3f& V : BScaled) { V *= ScaleToA; }
	TArray<int32> Found;
	auto Nearest = [&](const TArray<FVector3f>& Points, double& OutMean, double& OutP95, double& OutMax)
	{
		TArray<double> D;
		D.Reserve(Points.Num());
		for (const FVector3f& P : Points)
		{
			const FIntVector C = CellOf(P);
			double Best = TNumericLimits<double>::Max();
			for (int32 Ring = 1; Ring <= 6 && Best > (Ring - 1) * Cell; ++Ring)
			{
				for (int32 X = -Ring; X <= Ring; ++X) for (int32 Y = -Ring; Y <= Ring; ++Y) for (int32 Z = -Ring; Z <= Ring; ++Z)
				{
					if (Ring > 1 && FMath::Max3(FMath::Abs(X), FMath::Abs(Y), FMath::Abs(Z)) != Ring) { continue; }
					Found.Reset();
					Grid.MultiFind(C + FIntVector(X, Y, Z), Found);
					for (const int32 i : Found) { Best = FMath::Min(Best, static_cast<double>(FVector3f::Dist(P, A[i]))); }
				}
			}
			D.Add(Best < TNumericLimits<double>::Max() ? Best : 6.0 * Cell);
		}
		D.Sort();
		double S = 0.0;
		for (const double V : D) { S += V; }
		OutMean = S / D.Num();
		OutP95 = D[FMath::Min(D.Num() - 1, static_cast<int32>(D.Num() * 0.95))];
		OutMax = D.Last();
	};
	double SMean = 0.0, SP95 = 0.0, SMax = 0.0;
	Nearest(BScaled, SMean, SP95, SMax);
	Result->SetNumberField(TEXT("scale_b_to_a"), ScaleToA);
	Result->SetNumberField(TEXT("size_normalised_mean_cm"), SMean);
	Result->SetNumberField(TEXT("size_normalised_p95_cm"), SP95);
	Result->SetNumberField(TEXT("size_normalised_max_cm"), SMax);

	TArray<double> Dist;
	Dist.Reserve(B.Num());
	for (const FVector3f& P : B)
	{
		const FIntVector C = CellOf(P);
		double Best = TNumericLimits<double>::Max();
		for (int32 Ring = 1; Ring <= 6 && Best > (Ring - 1) * Cell; ++Ring)   // grow the search until the best match is inside the searched cube
		{
			for (int32 X = -Ring; X <= Ring; ++X) for (int32 Y = -Ring; Y <= Ring; ++Y) for (int32 Z = -Ring; Z <= Ring; ++Z)
			{
				if (FMath::Max3(FMath::Abs(X), FMath::Abs(Y), FMath::Abs(Z)) != Ring && Ring > 1) { continue; }
				Found.Reset();
				Grid.MultiFind(C + FIntVector(X, Y, Z), Found);
				for (const int32 i : Found) { Best = FMath::Min(Best, static_cast<double>(FVector3f::Dist(P, A[i]))); }
			}
		}
		Dist.Add(Best < TNumericLimits<double>::Max() ? Best : 6.0 * Cell);
	}
	Dist.Sort();
	double Sum = 0.0;
	for (const double D : Dist) { Sum += D; }
	Result->SetNumberField(TEXT("head_vertices_a"), A.Num());
	Result->SetNumberField(TEXT("head_vertices_b"), B.Num());
	Result->SetNumberField(TEXT("rms_radius_a_cm"), RadiusA);
	Result->SetNumberField(TEXT("rms_radius_b_cm"), RadiusB);
	Result->SetNumberField(TEXT("mean_cm"), Sum / Dist.Num());
	Result->SetNumberField(TEXT("p95_cm"), Dist[FMath::Min(Dist.Num() - 1, static_cast<int32>(Dist.Num() * 0.95))]);
	Result->SetNumberField(TEXT("max_cm"), Dist.Last());
	return ToJson(Result);
}

FString USpnMetaHumanPrepLibrary::CompareSkeletalMeshesByUV(UObject* MeshA, UObject* MeshB)
{
	const TSharedRef<FJsonObject> Result = MakeShared<FJsonObject>();
	USkeletalMesh* A = Cast<USkeletalMesh>(MeshA);
	USkeletalMesh* B = Cast<USkeletalMesh>(MeshB);
	const FSkeletalMeshRenderData* RA = A != nullptr ? A->GetResourceForRendering() : nullptr;
	const FSkeletalMeshRenderData* RB = B != nullptr ? B->GetResourceForRendering() : nullptr;
	if (RA == nullptr || RB == nullptr || RA->LODRenderData.IsEmpty() || RB->LODRenderData.IsEmpty())
	{
		Result->SetStringField(TEXT("error"), TEXT("needs two skeletal meshes with render data"));
		return ToJson(Result);
	}
	const FStaticMeshVertexBuffers& VA = RA->LODRenderData[0].StaticVertexBuffers;
	const FStaticMeshVertexBuffers& VB = RB->LODRenderData[0].StaticVertexBuffers;
	if (VA.StaticMeshVertexBuffer.GetNumTexCoords() == 0 || VB.StaticMeshVertexBuffer.GetNumTexCoords() == 0)
	{
		Result->SetStringField(TEXT("error"), TEXT("no UVs"));
		return ToJson(Result);
	}
	auto Key = [](const FVector2f& Uv) { return TPair<int64, int64>(FMath::RoundToInt64(Uv.X * 1.0e5), FMath::RoundToInt64(Uv.Y * 1.0e5)); };
	TMultiMap<TPair<int64, int64>, uint32> ByUv;
	const uint32 NA = VA.PositionVertexBuffer.GetNumVertices(), NB = VB.PositionVertexBuffer.GetNumVertices();
	for (uint32 i = 0; i < NA; ++i) { ByUv.Add(Key(VA.StaticMeshVertexBuffer.GetVertexUV(i, 0)), i); }
	TArray<double> Dist;
	Dist.Reserve(NB);
	uint32 Unmatched = 0;
	TArray<uint32> Candidates;
	for (uint32 j = 0; j < NB; ++j)
	{
		Candidates.Reset();
		ByUv.MultiFind(Key(VB.StaticMeshVertexBuffer.GetVertexUV(j, 0)), Candidates);
		if (Candidates.IsEmpty()) { ++Unmatched; continue; }
		const FVector3f P = VB.PositionVertexBuffer.VertexPosition(j);
		double Best = TNumericLimits<double>::Max();
		for (const uint32 i : Candidates) { Best = FMath::Min(Best, static_cast<double>(FVector3f::Dist(P, VA.PositionVertexBuffer.VertexPosition(i)))); }
		Dist.Add(Best);
	}
	Dist.Sort();
	double Sum = 0.0;
	for (const double D : Dist) { Sum += D; }
	Result->SetNumberField(TEXT("vertices_a"), NA);
	Result->SetNumberField(TEXT("vertices_b"), NB);
	Result->SetNumberField(TEXT("matched_share"), NB > 0 ? static_cast<double>(Dist.Num()) / NB : 0.0);
	Result->SetNumberField(TEXT("unmatched_uvs"), Unmatched);
	if (!Dist.IsEmpty())
	{
		Result->SetNumberField(TEXT("max_cm"), Dist.Last());
		Result->SetNumberField(TEXT("mean_cm"), Sum / Dist.Num());
		Result->SetNumberField(TEXT("p99_cm"), Dist[FMath::Min(Dist.Num() - 1, static_cast<int32>(Dist.Num() * 0.99))]);
	}
	return ToJson(Result);
}
