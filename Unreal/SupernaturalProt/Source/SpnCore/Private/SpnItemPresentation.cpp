#include "SpnItemPresentation.h"

const FSpnItemPresentationEntry* USpnItemPresentationSet::Find(const FString& ItemId) const
{
	for (const FSpnItemPresentationEntry& Entry : Entries)
	{
		if (Entry.ItemId == ItemId)
		{
			return &Entry;
		}
	}
	return nullptr;
}

FSpnPresentationReport USpnItemPresentationSet::ValidateAgainstCatalog(const TArray<FString>& CatalogItemIds) const
{
	FSpnPresentationReport Report;
	TSet<FString> Seen;
	for (const FSpnItemPresentationEntry& Entry : Entries)
	{
		if (Seen.Contains(Entry.ItemId))
		{
			Report.DuplicateEntries.AddUnique(Entry.ItemId);
			continue;
		}
		Seen.Add(Entry.ItemId);
		if (!CatalogItemIds.Contains(Entry.ItemId))
		{
			Report.OrphanedEntries.Add(Entry.ItemId);
		}
	}
	for (const FString& Id : CatalogItemIds)
	{
		if (!Seen.Contains(Id))
		{
			Report.ItemsWithoutPresentation.Add(Id);
		}
	}
	return Report;
}

FPrimaryAssetType USpnItemPresentationSet::GetAssetType()
{
	return FPrimaryAssetType(TEXT("SpnItemPresentation"));
}

FPrimaryAssetId USpnItemPresentationSet::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GetAssetType(), GetFName());
}
