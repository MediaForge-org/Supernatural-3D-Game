#include "SpnInventorySubsystem.h"
#include "SpnCoreModule.h"
#include "SpnInventoryInternal.h"

#include "Dom/JsonObject.h"
#include "Engine/GameInstance.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include <spn/core/CatalogFingerprint.h>
#include <spn/core/ContentId.h>

#include <exception>
#include <limits>
#include <set>
#include <string>
#include <variant>

namespace
{
	std::string ToUtf8(const FString& Text)
	{
		const FTCHARToUTF8 Converted(*Text);
		return std::string(Converted.Get(), static_cast<std::size_t>(Converted.Length()));
	}

	FString ToFString(const std::string& Utf8)
	{
		return FString(FUTF8ToTCHAR(Utf8.c_str(), static_cast<int32>(Utf8.size())));
	}

	FString ToFString(std::string_view Utf8)
	{
		return FString(FUTF8ToTCHAR(Utf8.data(), static_cast<int32>(Utf8.size())));
	}

	FSpnInventoryStatus MakeStatus(ESpnInventoryError Error, const FString& Detail, int32 FailedIndex = -1)
	{
		FSpnInventoryStatus Status;
		Status.Error = Error;
		Status.Detail = Detail;
		Status.FailedCommandIndex = FailedIndex;
		return Status;
	}

	bool ParseId(const FString& Text, spn::core::ContentId& Out, FString& OutError, const TCHAR* What)
	{
		const auto Parsed = spn::core::ContentId::Parse(ToUtf8(Text));
		if (!Parsed)
		{
			OutError = FString::Printf(TEXT("%s '%s' is not a valid id (namespace:path.segments, lower case)"), What, *Text);
			return false;
		}
		Out = *Parsed;
		return true;
	}

	ESpnInventoryError ToError(spn::core::InventoryErrorCode Code)
	{
		using spn::core::InventoryErrorCode;
		switch (Code)
		{
		case InventoryErrorCode::InvalidDefinition: return ESpnInventoryError::InvalidDefinition;
		case InventoryErrorCode::DuplicateDefinition: return ESpnInventoryError::DuplicateDefinition;
		case InventoryErrorCode::InvalidId: return ESpnInventoryError::InvalidId;
		case InventoryErrorCode::UnknownDefinition: return ESpnInventoryError::UnknownDefinition;
		case InventoryErrorCode::UnknownContainer: return ESpnInventoryError::UnknownContainer;
		case InventoryErrorCode::DuplicateContainer: return ESpnInventoryError::DuplicateContainer;
		case InventoryErrorCode::UnknownItem: return ESpnInventoryError::UnknownItem;
		case InventoryErrorCode::InvalidQuantity: return ESpnInventoryError::InvalidQuantity;
		case InventoryErrorCode::NotStackable: return ESpnInventoryError::NotStackable;
		case InventoryErrorCode::StackMismatch: return ESpnInventoryError::StackMismatch;
		case InventoryErrorCode::SameContainer: return ESpnInventoryError::SameContainer;
		case InventoryErrorCode::ContainerNotEmpty: return ESpnInventoryError::ContainerNotEmpty;
		case InventoryErrorCode::CategoryNotAllowed: return ESpnInventoryError::CategoryNotAllowed;
		case InventoryErrorCode::StackLimitReached: return ESpnInventoryError::StackLimitReached;
		case InventoryErrorCode::WeightLimitReached: return ESpnInventoryError::WeightLimitReached;
		case InventoryErrorCode::VolumeLimitReached: return ESpnInventoryError::VolumeLimitReached;
		case InventoryErrorCode::ItemEquipped: return ESpnInventoryError::ItemEquipped;
		case InventoryErrorCode::UnknownSlot: return ESpnInventoryError::UnknownSlot;
		case InventoryErrorCode::NotEquippable: return ESpnInventoryError::NotEquippable;
		case InventoryErrorCode::AlreadyEquipped: return ESpnInventoryError::AlreadyEquipped;
		case InventoryErrorCode::SlotOccupied: return ESpnInventoryError::SlotOccupied;
		case InventoryErrorCode::SlotEmpty: return ESpnInventoryError::SlotEmpty;
		case InventoryErrorCode::InvalidState: return ESpnInventoryError::InvalidState;
		case InventoryErrorCode::ArithmeticOverflow: return ESpnInventoryError::ArithmeticOverflow;
		case InventoryErrorCode::LimitExceeded: return ESpnInventoryError::LimitExceeded;
		default: return ESpnInventoryError::InternalError;
		}
	}

	FString DescribeError(const spn::core::InventoryError& Error)
	{
		return FString::Printf(TEXT("%s%s%s"), *ToFString(spn::core::ToString(Error.code)), Error.detail.empty() ? TEXT("") : TEXT(": "), *ToFString(Error.detail));
	}

	// --- Unreal data <-> Core -------------------------------------------------------------------------------------------------------------

	bool ToCoreSet(const TArray<FString>& In, std::set<spn::core::ContentId>& Out, FString& OutError, const TCHAR* What)
	{
		for (const FString& Text : In)
		{
			spn::core::ContentId Id;
			if (!ParseId(Text, Id, OutError, What))
			{
				return false;
			}
			Out.insert(Id);
		}
		return true;
	}

	bool ToCoreRules(const FSpnContainerRulesData& In, spn::core::ContainerRules& Out, FString& OutError)
	{
		if (!In.Kind.IsEmpty() && !ParseId(In.Kind, Out.kind, OutError, TEXT("container kind")))
		{
			return false;
		}
		const auto Limit = [&OutError](int64 Value, uint64 Max, const TCHAR* What, auto& OutOptional) -> bool
		{
			if (Value < 0)
			{
				return true; // unlimited
			}
			if (static_cast<uint64>(Value) > Max)
			{
				OutError = FString::Printf(TEXT("%s %lld is too large"), What, Value);
				return false;
			}
			OutOptional = static_cast<typename std::remove_reference_t<decltype(OutOptional)>::value_type>(Value);
			return true;
		};
		return Limit(In.MaxStacks, std::numeric_limits<uint32>::max(), TEXT("max stacks"), Out.maxStacks)
			&& Limit(In.MaxWeightGrams, std::numeric_limits<int64>::max(), TEXT("max weight"), Out.maxWeightGrams)
			&& Limit(In.MaxVolumeMilliliters, std::numeric_limits<int64>::max(), TEXT("max volume"), Out.maxVolumeMilliliters)
			&& ToCoreSet(In.AllowedCategories, Out.allowedCategories, OutError, TEXT("allowed category"))
			&& ToCoreSet(In.EquipSlots, Out.equipSlots, OutError, TEXT("container slot"));
	}

	FSpnContainerRulesData FromCoreRules(const spn::core::ContainerRules& In)
	{
		FSpnContainerRulesData Out;
		Out.Kind = ToFString(In.kind.Value());
		Out.MaxStacks = In.maxStacks ? static_cast<int64>(*In.maxStacks) : -1;
		Out.MaxWeightGrams = In.maxWeightGrams ? static_cast<int64>(*In.maxWeightGrams) : -1;
		Out.MaxVolumeMilliliters = In.maxVolumeMilliliters ? static_cast<int64>(*In.maxVolumeMilliliters) : -1;
		for (const spn::core::ContentId& Id : In.allowedCategories)
		{
			Out.AllowedCategories.Add(ToFString(Id.Value()));
		}
		for (const spn::core::ContentId& Id : In.equipSlots)
		{
			Out.EquipSlots.Add(ToFString(Id.Value()));
		}
		return Out;
	}

	bool ToCoreState(const TArray<FSpnItemStateField>& In, spn::core::FieldMap& Out, FString& OutError)
	{
		for (const FSpnItemStateField& Field : In)
		{
			const std::string Name = ToUtf8(Field.Name);
			if (Out.contains(Name))
			{
				OutError = FString::Printf(TEXT("state field '%s' appears twice"), *Field.Name);
				return false;
			}
			switch (Field.Type)
			{
			case ESpnStateValueType::Bool: Out.emplace(Name, spn::core::FactValue(Field.BoolValue)); break;
			case ESpnStateValueType::Int: Out.emplace(Name, spn::core::FactValue(static_cast<std::int64_t>(Field.IntValue))); break;
			case ESpnStateValueType::String: Out.emplace(Name, spn::core::FactValue(ToUtf8(Field.StringValue))); break;
			}
		}
		return true;
	}

	TArray<FSpnItemStateField> FromCoreState(const spn::core::FieldMap& In)
	{
		TArray<FSpnItemStateField> Out;
		for (const auto& Entry : In)
		{
			const FString Name = ToFString(Entry.first);
			if (const bool* B = std::get_if<bool>(&Entry.second))
			{
				Out.Add(FSpnItemStateField::MakeBool(Name, *B));
			}
			else if (const std::int64_t* I = std::get_if<std::int64_t>(&Entry.second))
			{
				Out.Add(FSpnItemStateField::MakeInt(Name, *I));
			}
			else
			{
				Out.Add(FSpnItemStateField::MakeString(Name, ToFString(std::get<std::string>(Entry.second))));
			}
		}
		return Out;
	}

	bool ToCoreDefinition(const FSpnItemDefinitionData& In, spn::core::ItemDefinition& Out, FString& OutError)
	{
		if (!ParseId(In.Id, Out.id, OutError, TEXT("item id")) || !ParseId(In.Category, Out.category, OutError, TEXT("item category")))
		{
			return false;
		}
		if (In.MaxStack < 1)
		{
			OutError = FString::Printf(TEXT("item '%s': max stack %d is below 1"), *In.Id, In.MaxStack);
			return false;
		}
		Out.maxStack = static_cast<std::uint32_t>(In.MaxStack);
		Out.weightGrams = In.WeightGrams;
		Out.volumeMilliliters = In.VolumeMilliliters;
		return ToCoreSet(In.EquipSlots, Out.equipSlots, OutError, TEXT("equip slot"));
	}

	bool ToCoreCommand(const FSpnInventoryCommand& In, spn::core::InventoryCommand& Out, FString& OutError)
	{
		using namespace spn::core;
		const auto Container = [&](std::uint64_t Id, const TCHAR* What, ContainerId& Result) -> bool
		{
			if (Id == 0)
			{
				OutError = FString::Printf(TEXT("%s id 0 is the null id"), What);
				return false;
			}
			Result = ContainerId(Id);
			return true;
		};
		const auto Stack = [&](std::uint64_t Id, ItemInstanceId& Result) -> bool
		{
			if (Id == 0)
			{
				OutError = TEXT("item stack id 0 is the null id");
				return false;
			}
			Result = ItemInstanceId(Id);
			return true;
		};
		ContainerId C, C2;
		ItemInstanceId S, S2;
		switch (In.Type)
		{
		case ESpnInventoryCommandType::CreateContainer:
		{
			CreateContainer Command;
			if (!Container(In.Container, TEXT("container"), Command.container) || !ToCoreRules(In.Rules, Command.rules, OutError))
			{
				return false;
			}
			Out = std::move(Command);
			return true;
		}
		case ESpnInventoryCommandType::DestroyContainer:
			if (!Container(In.Container, TEXT("container"), C))
			{
				return false;
			}
			Out = DestroyContainer{C};
			return true;
		case ESpnInventoryCommandType::AddItem:
		{
			AddItem Command;
			if (!Container(In.Container, TEXT("container"), Command.container) || !ParseId(In.Definition, Command.definition, OutError, TEXT("item definition"))
				|| !ToCoreState(In.State, Command.state, OutError))
			{
				return false;
			}
			Command.quantity = In.Quantity;
			Out = std::move(Command);
			return true;
		}
		case ESpnInventoryCommandType::RemoveItem:
			if (!Container(In.Container, TEXT("container"), C) || !Stack(In.Item, S))
			{
				return false;
			}
			Out = RemoveItem{C, S, In.Quantity};
			return true;
		case ESpnInventoryCommandType::TransferItem:
			if (!Container(In.Container, TEXT("source container"), C) || !Container(In.OtherContainer, TEXT("target container"), C2) || !Stack(In.Item, S))
			{
				return false;
			}
			Out = TransferItem{C, C2, S, In.Quantity};
			return true;
		case ESpnInventoryCommandType::SplitStack:
			if (!Container(In.Container, TEXT("container"), C) || !Stack(In.Item, S))
			{
				return false;
			}
			Out = SplitStack{C, S, In.Quantity};
			return true;
		case ESpnInventoryCommandType::MergeStacks:
			if (!Container(In.Container, TEXT("container"), C) || !Stack(In.Item, S) || !Stack(In.OtherItem, S2))
			{
				return false;
			}
			Out = MergeStacks{C, S, S2, In.Quantity};
			return true;
		case ESpnInventoryCommandType::EquipItem:
		{
			ContentId Slot;
			if (!Container(In.Container, TEXT("container"), C) || !Stack(In.Item, S) || !ParseId(In.Slot, Slot, OutError, TEXT("equipment slot")))
			{
				return false;
			}
			Out = EquipItem{C, S, Slot};
			return true;
		}
		case ESpnInventoryCommandType::UnequipSlot:
		{
			ContentId Slot;
			if (!Container(In.Container, TEXT("container"), C) || !ParseId(In.Slot, Slot, OutError, TEXT("equipment slot")))
			{
				return false;
			}
			Out = UnequipSlot{C, Slot};
			return true;
		}
		}
		OutError = TEXT("unknown command type");
		return false;
	}

	// --- JSON -------------------------------------------------------------------------------------------------------------------------

	bool GetString(const TSharedPtr<FJsonObject>& Object, const TCHAR* Field, FString& Out, FString& OutError, const FString& Where, bool bRequired = true)
	{
		if (!Object->HasField(Field))
		{
			if (bRequired)
			{
				OutError = FString::Printf(TEXT("%s: missing \"%s\""), *Where, Field);
			}
			return !bRequired;
		}
		if (!Object->TryGetStringField(Field, Out))
		{
			OutError = FString::Printf(TEXT("%s: \"%s\" must be a string"), *Where, Field);
			return false;
		}
		return true;
	}

	/** A non-negative whole number field; `Missing` is set when the field is absent. */
	bool GetNumber(const TSharedPtr<FJsonObject>& Object, const TCHAR* Field, int64& Out, bool& bMissing, FString& OutError, const FString& Where)
	{
		bMissing = !Object->HasField(Field);
		if (bMissing)
		{
			return true;
		}
		double Value = 0.0;
		if (!Object->TryGetNumberField(Field, Value) || Value < 0.0 || Value != FMath::FloorToDouble(Value) || Value > 9.0e15)
		{
			OutError = FString::Printf(TEXT("%s: \"%s\" must be a non-negative whole number"), *Where, Field);
			return false;
		}
		Out = static_cast<int64>(Value);
		return true;
	}

	bool GetStringArray(const TSharedPtr<FJsonObject>& Object, const TCHAR* Field, TArray<FString>& Out, FString& OutError, const FString& Where)
	{
		if (!Object->HasField(Field))
		{
			return true;
		}
		const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
		if (!Object->TryGetArrayField(Field, Values))
		{
			OutError = FString::Printf(TEXT("%s: \"%s\" must be an array of strings"), *Where, Field);
			return false;
		}
		for (const TSharedPtr<FJsonValue>& Value : *Values)
		{
			FString Text;
			if (!Value.IsValid() || !Value->TryGetString(Text))
			{
				OutError = FString::Printf(TEXT("%s: \"%s\" must be an array of strings"), *Where, Field);
				return false;
			}
			Out.Add(Text);
		}
		return true;
	}

	bool ParseCatalogJson(const FString& JsonText, FSpnItemCatalogData& Out, FString& OutError)
	{
		TSharedPtr<FJsonObject> Root;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonText);
		if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
		{
			OutError = TEXT("not valid JSON");
			return false;
		}
		double Schema = 0.0;
		if (!Root->TryGetNumberField(TEXT("schema"), Schema) || Schema != 1.0)
		{
			OutError = TEXT("\"schema\" must be 1");
			return false;
		}
		Root->TryGetBoolField(TEXT("test_data"), Out.bIsTestData);
		Root->TryGetStringField(TEXT("description"), Out.Description);

		const TArray<TSharedPtr<FJsonValue>>* Items = nullptr;
		if (!Root->TryGetArrayField(TEXT("items"), Items))
		{
			OutError = TEXT("\"items\" must be an array");
			return false;
		}
		for (int32 Index = 0; Index < Items->Num(); ++Index)
		{
			const FString Where = FString::Printf(TEXT("items[%d]"), Index);
			const TSharedPtr<FJsonObject>* ObjectPtr = nullptr;
			if (!(*Items)[Index].IsValid() || !(*Items)[Index]->TryGetObject(ObjectPtr))
			{
				OutError = Where + TEXT(" must be an object");
				return false;
			}
			const TSharedPtr<FJsonObject>& Object = *ObjectPtr;
			FSpnItemDefinitionData Item;
			int64 Number = 0;
			bool bMissing = false;
			if (!GetString(Object, TEXT("id"), Item.Id, OutError, Where) || !GetString(Object, TEXT("category"), Item.Category, OutError, Where))
			{
				return false;
			}
			if (!GetNumber(Object, TEXT("max_stack"), Number, bMissing, OutError, Where))
			{
				return false;
			}
			Item.MaxStack = bMissing ? 1 : static_cast<int32>(FMath::Min<int64>(Number, std::numeric_limits<int32>::max()));
			if (!GetNumber(Object, TEXT("weight_g"), Number, bMissing, OutError, Where))
			{
				return false;
			}
			Item.WeightGrams = bMissing ? 0 : static_cast<uint64>(Number);
			if (!GetNumber(Object, TEXT("volume_ml"), Number, bMissing, OutError, Where))
			{
				return false;
			}
			Item.VolumeMilliliters = bMissing ? 0 : static_cast<uint64>(Number);
			if (!GetStringArray(Object, TEXT("equip_slots"), Item.EquipSlots, OutError, Where))
			{
				return false;
			}
			Out.Items.Add(MoveTemp(Item));
		}

		const TArray<TSharedPtr<FJsonValue>>* Presets = nullptr;
		if (Root->HasField(TEXT("container_presets")))
		{
			if (!Root->TryGetArrayField(TEXT("container_presets"), Presets))
			{
				OutError = TEXT("\"container_presets\" must be an array");
				return false;
			}
			for (int32 Index = 0; Index < Presets->Num(); ++Index)
			{
				const FString Where = FString::Printf(TEXT("container_presets[%d]"), Index);
				const TSharedPtr<FJsonObject>* ObjectPtr = nullptr;
				if (!(*Presets)[Index].IsValid() || !(*Presets)[Index]->TryGetObject(ObjectPtr))
				{
					OutError = Where + TEXT(" must be an object");
					return false;
				}
				const TSharedPtr<FJsonObject>& Object = *ObjectPtr;
				FSpnContainerPresetData Preset;
				int64 Number = 0;
				bool bMissing = false;
				if (!GetString(Object, TEXT("id"), Preset.Id, OutError, Where) || !GetString(Object, TEXT("kind"), Preset.Rules.Kind, OutError, Where, /*bRequired*/ false))
				{
					return false;
				}
				if (!GetNumber(Object, TEXT("max_stacks"), Number, bMissing, OutError, Where))
				{
					return false;
				}
				Preset.Rules.MaxStacks = bMissing ? -1 : Number;
				if (!GetNumber(Object, TEXT("max_weight_g"), Number, bMissing, OutError, Where))
				{
					return false;
				}
				Preset.Rules.MaxWeightGrams = bMissing ? -1 : Number;
				if (!GetNumber(Object, TEXT("max_volume_ml"), Number, bMissing, OutError, Where))
				{
					return false;
				}
				Preset.Rules.MaxVolumeMilliliters = bMissing ? -1 : Number;
				if (!GetStringArray(Object, TEXT("allowed_categories"), Preset.Rules.AllowedCategories, OutError, Where)
					|| !GetStringArray(Object, TEXT("equip_slots"), Preset.Rules.EquipSlots, OutError, Where))
				{
					return false;
				}
				Out.ContainerPresets.Add(MoveTemp(Preset));
			}
		}
		return true;
	}
}

const TCHAR* LexToString(ESpnInventoryError Error)
{
	switch (Error)
	{
	case ESpnInventoryError::None: return TEXT("None");
	case ESpnInventoryError::NoCatalog: return TEXT("NoCatalog");
	case ESpnInventoryError::InvalidArgument: return TEXT("InvalidArgument");
	case ESpnInventoryError::CatalogLocked: return TEXT("CatalogLocked");
	case ESpnInventoryError::InternalError: return TEXT("InternalError");
	case ESpnInventoryError::InvalidId: return TEXT("InvalidId");
	case ESpnInventoryError::UnknownDefinition: return TEXT("UnknownDefinition");
	case ESpnInventoryError::UnknownContainer: return TEXT("UnknownContainer");
	case ESpnInventoryError::DuplicateContainer: return TEXT("DuplicateContainer");
	case ESpnInventoryError::UnknownItem: return TEXT("UnknownItem");
	case ESpnInventoryError::InvalidQuantity: return TEXT("InvalidQuantity");
	case ESpnInventoryError::NotStackable: return TEXT("NotStackable");
	case ESpnInventoryError::StackMismatch: return TEXT("StackMismatch");
	case ESpnInventoryError::SameContainer: return TEXT("SameContainer");
	case ESpnInventoryError::ContainerNotEmpty: return TEXT("ContainerNotEmpty");
	case ESpnInventoryError::CategoryNotAllowed: return TEXT("CategoryNotAllowed");
	case ESpnInventoryError::StackLimitReached: return TEXT("StackLimitReached");
	case ESpnInventoryError::WeightLimitReached: return TEXT("WeightLimitReached");
	case ESpnInventoryError::VolumeLimitReached: return TEXT("VolumeLimitReached");
	case ESpnInventoryError::ItemEquipped: return TEXT("ItemEquipped");
	case ESpnInventoryError::UnknownSlot: return TEXT("UnknownSlot");
	case ESpnInventoryError::NotEquippable: return TEXT("NotEquippable");
	case ESpnInventoryError::AlreadyEquipped: return TEXT("AlreadyEquipped");
	case ESpnInventoryError::SlotOccupied: return TEXT("SlotOccupied");
	case ESpnInventoryError::SlotEmpty: return TEXT("SlotEmpty");
	case ESpnInventoryError::InvalidState: return TEXT("InvalidState");
	case ESpnInventoryError::ArithmeticOverflow: return TEXT("ArithmeticOverflow");
	case ESpnInventoryError::LimitExceeded: return TEXT("LimitExceeded");
	case ESpnInventoryError::InvalidDefinition: return TEXT("InvalidDefinition");
	case ESpnInventoryError::DuplicateDefinition: return TEXT("DuplicateDefinition");
	}
	return TEXT("Unknown");
}

USpnInventorySubsystem::USpnInventorySubsystem()
	: State(MakePimpl<FState>())
{
}

USpnInventorySubsystem::~USpnInventorySubsystem() = default;

void USpnInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogSpnCore, Log, TEXT("Inventory [%s]: subsystem initialized (inactive until a catalog is loaded)"), *GetNameSafe(GetGameInstance()));
}

void USpnInventorySubsystem::Deinitialize()
{
	OnInventoryChanged.Clear();
	OnInventoryReplaced.Clear();
	State->Inventory.reset(); // before the catalog it points to
	State->Catalog.reset();
	Super::Deinitialize();
}

FString USpnInventorySubsystem::GetTestCatalogPath()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("SpnTest/Inventory/TestItemCatalog.json"));
}

bool USpnInventorySubsystem::HasCatalog() const
{
	return State->Catalog != nullptr && State->Inventory.has_value();
}

const FSpnItemCatalogData& USpnInventorySubsystem::GetCatalogData() const
{
	return State->CatalogData;
}

bool USpnInventorySubsystem::FindDefinition(const FString& DefinitionId, FSpnItemDefinitionData& OutDefinition) const
{
	for (const FSpnItemDefinitionData& Item : State->CatalogData.Items)
	{
		if (Item.Id == DefinitionId)
		{
			OutDefinition = Item;
			return true;
		}
	}
	return false;
}

bool USpnInventorySubsystem::FindContainerPreset(const FString& PresetId, FSpnContainerRulesData& OutRules) const
{
	for (const FSpnContainerPresetData& Preset : State->CatalogData.ContainerPresets)
	{
		if (Preset.Id == PresetId)
		{
			OutRules = Preset.Rules;
			return true;
		}
	}
	return false;
}

FSpnInventoryStatus USpnInventorySubsystem::SetCatalog(const FSpnItemCatalogData& Catalog)
{
	const FString Owner = GetNameSafe(GetGameInstance());
	const auto Reject = [this, &Owner](ESpnInventoryError Error, const FString& Detail)
	{
		FSpnInventoryStatus Status = MakeStatus(Error, Detail);
		Status.Revision = GetRevision();
		State->LastFailure = Status;
		UE_LOG(LogSpnCore, Warning, TEXT("Inventory [%s]: catalog NOT installed: %s: %s"), *Owner, LexToString(Error), *Detail);
		return Status;
	};
	try
	{
		if (State->Inventory && !State->Inventory->Containers().empty())
		{
			return Reject(ESpnInventoryError::CatalogLocked, TEXT("containers exist; call ResetInventory first"));
		}
		auto Core = std::make_unique<spn::core::ItemCatalog>();
		FString Error;
		for (const FSpnItemDefinitionData& Item : Catalog.Items)
		{
			spn::core::ItemDefinition Definition;
			if (!ToCoreDefinition(Item, Definition, Error))
			{
				return Reject(ESpnInventoryError::InvalidArgument, Error);
			}
			if (const auto CoreError = Core->Add(std::move(Definition)))
			{
				return Reject(ToError(CoreError->code), FString::Printf(TEXT("item '%s': %s"), *Item.Id, *DescribeError(*CoreError)));
			}
		}
		std::set<spn::core::ContentId> PresetIds;
		for (const FSpnContainerPresetData& Preset : Catalog.ContainerPresets)
		{
			spn::core::ContentId Id;
			spn::core::ContainerRules Rules;
			if (!ParseId(Preset.Id, Id, Error, TEXT("preset id")) || !ToCoreRules(Preset.Rules, Rules, Error))
			{
				return Reject(ESpnInventoryError::InvalidArgument, Error);
			}
			if (!PresetIds.insert(Id).second)
			{
				return Reject(ESpnInventoryError::InvalidArgument, FString::Printf(TEXT("preset '%s' appears twice"), *Preset.Id));
			}
		}
		State->Inventory.reset(); // the old state points to the old catalog
		State->Catalog = std::move(Core);
		State->CatalogData = Catalog;
		State->Inventory.emplace(*State->Catalog);
		State->LastFailure = FSpnInventoryStatus();
		UE_LOG(LogSpnCore, Log, TEXT("Inventory [%s]: catalog installed: %d item definition(s), %d container preset(s)%s"), *Owner, Catalog.Items.Num(),
			Catalog.ContainerPresets.Num(), Catalog.bIsTestData ? TEXT(" -- TEMPORARY TEST DATA, not game content") : TEXT(""));
		BroadcastReplaced();
		FSpnInventoryStatus Status;
		Status.Revision = 0;
		return Status;
	}
	catch (const std::exception& Exception)
	{
		return Reject(ESpnInventoryError::InternalError, FString::Printf(TEXT("exception: %s"), *ToFString(std::string(Exception.what()))));
	}
	catch (...)
	{
		return Reject(ESpnInventoryError::InternalError, TEXT("unknown exception"));
	}
}

FSpnInventoryStatus USpnInventorySubsystem::SetCatalogFromJson(const FString& JsonText)
{
	FSpnItemCatalogData Data;
	FString Error;
	if (!ParseCatalogJson(JsonText, Data, Error))
	{
		FSpnInventoryStatus Status = MakeStatus(ESpnInventoryError::InvalidArgument, Error);
		State->LastFailure = Status;
		UE_LOG(LogSpnCore, Warning, TEXT("Inventory [%s]: catalog NOT installed: %s"), *GetNameSafe(GetGameInstance()), *Error);
		return Status;
	}
	return SetCatalog(Data);
}

FSpnInventoryStatus USpnInventorySubsystem::LoadCatalogFromFile(const FString& AbsolutePath)
{
	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *AbsolutePath, FFileHelper::EHashOptions::None, FILEREAD_Silent))
	{
		FSpnInventoryStatus Status = MakeStatus(ESpnInventoryError::InvalidArgument, FString::Printf(TEXT("cannot read '%s'"), *AbsolutePath));
		State->LastFailure = Status;
		UE_LOG(LogSpnCore, Warning, TEXT("Inventory [%s]: catalog NOT installed: %s"), *GetNameSafe(GetGameInstance()), *Status.Detail);
		return Status;
	}
	return SetCatalogFromJson(Text);
}

void USpnInventorySubsystem::ResetInventoryInternal()
{
	if (State->Catalog)
	{
		State->Inventory.reset();
		State->Inventory.emplace(*State->Catalog);
	}
}

void USpnInventorySubsystem::BroadcastReplaced()
{
	FSpnInventoryChange Change;
	Change.Revision = GetRevision();
	Change.bReplacedAll = true;
	OnInventoryReplaced.Broadcast();
	OnInventoryChangedDetailed.Broadcast(Change);
}

void USpnInventorySubsystem::ResetInventory()
{
	ResetInventoryInternal();
	BroadcastReplaced();
}

void USpnInventorySubsystem::ClearCatalog()
{
	State->Inventory.reset();
	State->Catalog.reset();
	State->CatalogData = FSpnItemCatalogData();
	BroadcastReplaced();
}

FString USpnInventorySubsystem::GetCatalogFingerprint() const
{
	return State->Catalog ? ToFString(spn::core::CatalogFingerprint(*State->Catalog)) : FString();
}

void USpnInventorySubsystem::SetPresentationSet(USpnItemPresentationSet* InSet)
{
	PresentationSet = InSet;
	BroadcastReplaced(); // names and icons changed: views refresh (the gameplay state did not change)
}

const FSpnItemPresentationEntry* USpnInventorySubsystem::FindPresentation(const FString& DefinitionId) const
{
	return PresentationSet != nullptr ? PresentationSet->Find(DefinitionId) : nullptr;
}

FSpnPresentationReport USpnInventorySubsystem::ValidatePresentation() const
{
	FSpnPresentationReport Report;
	if (PresentationSet != nullptr)
	{
		TArray<FString> Ids;
		for (const FSpnItemDefinitionData& Item : State->CatalogData.Items)
		{
			Ids.Add(Item.Id);
		}
		Report = PresentationSet->ValidateAgainstCatalog(Ids);
	}
	return Report;
}

FSpnInventoryStatus USpnInventorySubsystem::ProbeCommands(const TArray<FSpnInventoryCommand>& Commands)
{
	FSpnInventoryStatus Status;
	Status.Revision = GetRevision();
	if (!HasCatalog())
	{
		Status.Error = ESpnInventoryError::NoCatalog;
		Status.Detail = TEXT("no item catalog is loaded");
		return Status;
	}
	try
	{
		std::vector<spn::core::InventoryCommand> Translated;
		for (int32 Index = 0; Index < Commands.Num(); ++Index)
		{
			spn::core::InventoryCommand Command;
			FString Error;
			if (!ToCoreCommand(Commands[Index], Command, Error))
			{
				Status.Error = ESpnInventoryError::InvalidArgument;
				Status.Detail = Error;
				Status.FailedCommandIndex = Index;
				return Status;
			}
			Translated.push_back(std::move(Command));
		}
		if (const auto Error = State->Inventory->Probe(Translated))
		{
			Status.Error = ToError(Error->code);
			Status.Detail = DescribeError(*Error);
			Status.FailedCommandIndex = static_cast<int32>(Error->commandIndex);
		}
	}
	catch (...)
	{
		Status.Error = ESpnInventoryError::InternalError;
		Status.Detail = TEXT("exception while probing");
	}
	return Status;
}


FSpnInventoryStatus USpnInventorySubsystem::Apply(const TArray<FSpnInventoryCommand>& Commands)
{
	check(IsInGameThread());
	const FString Owner = GetNameSafe(GetGameInstance());
	const auto Reject = [this, &Owner](FSpnInventoryStatus Status)
	{
		Status.Revision = GetRevision();
		State->LastFailure = Status;
		UE_LOG(LogSpnCore, Warning, TEXT("Inventory [%s]: transaction rejected: %s%s: %s (state unchanged, revision %llu)"), *Owner, LexToString(Status.Error),
			Status.FailedCommandIndex >= 0 ? *FString::Printf(TEXT(" at command %d"), Status.FailedCommandIndex) : TEXT(""), *Status.Detail,
			static_cast<unsigned long long>(Status.Revision));
		return Status;
	};
	if (!HasCatalog())
	{
		return Reject(MakeStatus(ESpnInventoryError::NoCatalog, TEXT("no item catalog is loaded")));
	}
	try
	{
		std::vector<spn::core::InventoryCommand> Translated;
		Translated.reserve(static_cast<std::size_t>(Commands.Num()));
		for (int32 Index = 0; Index < Commands.Num(); ++Index)
		{
			spn::core::InventoryCommand Command;
			FString Error;
			if (!ToCoreCommand(Commands[Index], Command, Error))
			{
				return Reject(MakeStatus(ESpnInventoryError::InvalidArgument, Error, Index));
			}
			Translated.push_back(std::move(Command));
		}
		auto Result = State->Inventory->ApplyAll(Translated);
		if (!Result)
		{
			return Reject(MakeStatus(ToError(Result.Error().code), DescribeError(Result.Error()), static_cast<int32>(Result.Error().commandIndex)));
		}
		FSpnInventoryStatus Status;
		Status.Revision = Result.Value().revision;
		for (const spn::core::ItemInstanceId Id : Result.Value().createdStacks)
		{
			Status.CreatedStacks.Add(Id.Value());
		}
		if (!Commands.IsEmpty())
		{
			FSpnInventoryChange Change;
			Change.Revision = Status.Revision;
			for (const FSpnInventoryCommand& Command : Commands)
			{
				Change.TouchedContainers.AddUnique(Command.Container);
				if (Command.Type == ESpnInventoryCommandType::TransferItem)
				{
					Change.TouchedContainers.AddUnique(Command.OtherContainer);
				}
			}
			OnInventoryChanged.Broadcast();
			OnInventoryChangedDetailed.Broadcast(Change);
		}
		return Status;
	}
	catch (const std::exception& Exception)
	{
		return Reject(MakeStatus(ESpnInventoryError::InternalError, FString::Printf(TEXT("exception: %s"), *ToFString(std::string(Exception.what())))));
	}
	catch (...)
	{
		return Reject(MakeStatus(ESpnInventoryError::InternalError, TEXT("unknown exception")));
	}
}

FSpnInventoryStatus USpnInventorySubsystem::CreateContainer(uint64 ContainerId, const FSpnContainerRulesData& Rules)
{
	return Apply({FSpnInventoryCommand::CreateContainer(ContainerId, Rules)});
}

FSpnInventoryStatus USpnInventorySubsystem::CreateContainerFromPreset(uint64 ContainerId, const FString& PresetId)
{
	if (!HasCatalog())
	{
		return Apply({FSpnInventoryCommand::CreateContainer(ContainerId, FSpnContainerRulesData())}); // reports NoCatalog in the usual way
	}
	FSpnContainerRulesData Rules;
	if (!FindContainerPreset(PresetId, Rules))
	{
		FSpnInventoryStatus Status = MakeStatus(ESpnInventoryError::InvalidArgument, FString::Printf(TEXT("unknown container preset '%s'"), *PresetId), 0);
		Status.Revision = GetRevision();
		State->LastFailure = Status;
		UE_LOG(LogSpnCore, Warning, TEXT("Inventory [%s]: %s"), *GetNameSafe(GetGameInstance()), *Status.Detail);
		return Status;
	}
	return CreateContainer(ContainerId, Rules);
}

FSpnInventoryStatus USpnInventorySubsystem::DestroyContainer(uint64 ContainerId)
{
	return Apply({FSpnInventoryCommand::DestroyContainer(ContainerId)});
}

FSpnInventoryStatus USpnInventorySubsystem::AddItem(uint64 ContainerId, const FString& DefinitionId, uint32 Quantity, const TArray<FSpnItemStateField>& StateFields)
{
	return Apply({FSpnInventoryCommand::AddItem(ContainerId, DefinitionId, Quantity, StateFields)});
}

FSpnInventoryStatus USpnInventorySubsystem::RemoveItem(uint64 ContainerId, uint64 StackId, uint32 Quantity)
{
	return Apply({FSpnInventoryCommand::RemoveItem(ContainerId, StackId, Quantity)});
}

FSpnInventoryStatus USpnInventorySubsystem::TransferItem(uint64 FromContainer, uint64 ToContainer, uint64 StackId, uint32 Quantity)
{
	return Apply({FSpnInventoryCommand::TransferItem(FromContainer, ToContainer, StackId, Quantity)});
}

FSpnInventoryStatus USpnInventorySubsystem::SplitStack(uint64 ContainerId, uint64 StackId, uint32 Quantity)
{
	return Apply({FSpnInventoryCommand::SplitStack(ContainerId, StackId, Quantity)});
}

FSpnInventoryStatus USpnInventorySubsystem::MergeStacks(uint64 ContainerId, uint64 SourceStack, uint64 TargetStack, uint32 Quantity)
{
	return Apply({FSpnInventoryCommand::MergeStacks(ContainerId, SourceStack, TargetStack, Quantity)});
}

FSpnInventoryStatus USpnInventorySubsystem::Equip(uint64 ContainerId, uint64 StackId, const FString& Slot)
{
	return Apply({FSpnInventoryCommand::EquipItem(ContainerId, StackId, Slot)});
}

FSpnInventoryStatus USpnInventorySubsystem::Unequip(uint64 ContainerId, const FString& Slot)
{
	return Apply({FSpnInventoryCommand::UnequipSlot(ContainerId, Slot)});
}

const FSpnInventoryStatus& USpnInventorySubsystem::GetLastFailure() const
{
	return State->LastFailure;
}

uint64 USpnInventorySubsystem::GetRevision() const
{
	return State->Inventory ? State->Inventory->Revision() : 0;
}

TArray<uint64> USpnInventorySubsystem::ListContainers() const
{
	TArray<uint64> Ids;
	if (State->Inventory)
	{
		for (const auto& Entry : State->Inventory->Containers())
		{
			Ids.Add(Entry.first.Value());
		}
	}
	return Ids;
}

bool USpnInventorySubsystem::GetContainer(uint64 ContainerId, FSpnContainerView& OutView) const
{
	if (!State->Inventory)
	{
		return false;
	}
	const spn::core::Container* Container = State->Inventory->FindContainer(spn::core::ContainerId(ContainerId));
	if (Container == nullptr)
	{
		return false;
	}
	FSpnContainerView View;
	View.ContainerId = ContainerId;
	View.Rules = FromCoreRules(Container->rules);
	View.WeightGrams = Container->weightGrams;
	View.VolumeMilliliters = Container->volumeMilliliters;
	for (const auto& Entry : Container->stacks)
	{
		FSpnStackView Stack;
		Stack.StackId = Entry.first.Value();
		Stack.Definition = ToFString(Entry.second.definition.Value());
		Stack.Quantity = Entry.second.quantity;
		Stack.State = FromCoreState(Entry.second.state);
		for (const auto& Slot : Container->equipped)
		{
			if (Slot.second == Entry.first)
			{
				Stack.EquippedSlot = ToFString(Slot.first.Value());
			}
		}
		View.Stacks.Add(MoveTemp(Stack));
	}
	OutView = MoveTemp(View);
	return true;
}

uint64 USpnInventorySubsystem::CountOf(uint64 ContainerId, const FString& DefinitionId) const
{
	const auto Id = spn::core::ContentId::Parse(ToUtf8(DefinitionId));
	return State->Inventory && Id ? State->Inventory->CountOf(spn::core::ContainerId(ContainerId), *Id) : 0;
}

uint64 USpnInventorySubsystem::LocateStack(uint64 StackId) const
{
	return State->Inventory ? State->Inventory->LocateStack(spn::core::ItemInstanceId(StackId)).Value() : 0;
}

uint64 USpnInventorySubsystem::GetNextStackId() const
{
	return State->Inventory ? State->Inventory->NextInstanceId().Value() : 1;
}

FString USpnInventorySubsystem::CheckInvariants() const
{
	if (!State->Inventory)
	{
		return FString();
	}
	const auto Error = State->Inventory->CheckInvariants();
	return Error ? DescribeError(*Error) : FString();
}

FString USpnInventorySubsystem::Describe() const
{
	if (!HasCatalog())
	{
		return TEXT("inventory inactive (no catalog)");
	}
	FString Text = FString::Printf(TEXT("revision %llu, next stack id %llu, %d container(s)"), static_cast<unsigned long long>(GetRevision()),
		static_cast<unsigned long long>(GetNextStackId()), ListContainers().Num());
	for (const uint64 Id : ListContainers())
	{
		FSpnContainerView View;
		GetContainer(Id, View);
		Text += FString::Printf(TEXT("\n  container %llu [%s] %d stack(s), %llu g, %llu ml"), static_cast<unsigned long long>(Id), *View.Rules.Kind, View.Stacks.Num(),
			static_cast<unsigned long long>(View.WeightGrams), static_cast<unsigned long long>(View.VolumeMilliliters));
		for (const FSpnStackView& Stack : View.Stacks)
		{
			Text += FString::Printf(TEXT("\n    stack %llu: %u x %s%s"), static_cast<unsigned long long>(Stack.StackId), Stack.Quantity, *Stack.Definition,
				Stack.EquippedSlot.IsEmpty() ? TEXT("") : *FString::Printf(TEXT("  [equipped: %s]"), *Stack.EquippedSlot));
			for (const FSpnItemStateField& Field : Stack.State)
			{
				Text += FString::Printf(TEXT(" %s=%s"), *Field.Name,
					Field.Type == ESpnStateValueType::Bool ? (Field.BoolValue ? TEXT("true") : TEXT("false"))
					: Field.Type == ESpnStateValueType::Int ? *FString::Printf(TEXT("%lld"), Field.IntValue) : *Field.StringValue);
			}
		}
	}
	return Text;
}
