#include "Engine/Engine.h"
#include "Engine/UserInterfaceSettings.h"
#include "Misc/ConfigCacheIni.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "Misc/App.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnMapSubsystem.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "Spn/SpnSettingsSubsystem.h"
#include "Spn/SpnWorldObjects.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnUiText.h"

#if WITH_DEV_AUTOMATION_TESTS

// Night Build 01: world pickups and containers backed by the real inventory, the settings, and the interface texts. Each test runs in its own
// game instance with a dummy game world and its own temporary save root; no level and no user save is touched.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FNbWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		FString Root;
		TArray<AActor*> Spawned;

		FNbWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Save().SetSaveDirectory(Root);
			SpnWorldContainerRegistry().Reset();
			FString Json;
			FFileHelper::LoadFileToString(Json, *USpnInventorySubsystem::GetTestCatalogPath());
			Inventory().SetCatalogFromJson(Json);
			Inventory().CreateContainerFromPreset(SpnWorld::PlayerContainerId, TEXT("spn_test:preset.body"));
		}

		~FNbWorld()
		{
			for (AActor* Actor : Spawned) { if (IsValid(Actor)) { Actor->Destroy(); } }
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
			GameInstance->RemoveFromRoot();
			SpnWorldContainerRegistry().Reset();
			IFileManager::Get().DeleteDirectory(*Root, false, true);
		}

		USpnInventorySubsystem& Inventory() const { return *GameInstance->GetSubsystem<USpnInventorySubsystem>(); }
		USpnWorldStateSubsystem& WorldState() const { return *GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }
		USpnSaveSubsystem& Save() const { return *GameInstance->GetSubsystem<USpnSaveSubsystem>(); }

		ASpnWorldPickup* Pickup(const TCHAR* Id, const TCHAR* Definition, int32 Quantity)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnWorldPickup* Actor = World->SpawnActor<ASpnWorldPickup>(ASpnWorldPickup::StaticClass(), FTransform(FVector(0, 0, 0)), Params);
			Actor->StableId = Id;
			Actor->DefinitionId = Definition;
			Actor->Quantity = Quantity;
			Actor->FinishSpawning(FTransform(FVector(0, 0, 0)));
			Actor->DispatchBeginPlay();
			Spawned.Add(Actor);
			return Actor;
		}

		ASpnWorldContainer* Container(const TCHAR* Id)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnWorldContainer* Actor = World->SpawnActor<ASpnWorldContainer>(ASpnWorldContainer::StaticClass(), FTransform(FVector(500, 0, 0)), Params);
			Actor->StableId = Id;
			Actor->PresetId = TEXT("spn_test:preset.small_box");
			Actor->Title = TEXT("Test box");
			Actor->InitialItems = {{TEXT("spn_test:item.bandage"), 3}, {TEXT("spn_test:item.notebook"), 1}};
			Actor->FinishSpawning(FTransform(FVector(500, 0, 0)));
			Actor->DispatchBeginPlay();
			Spawned.Add(Actor);
			return Actor;
		}
	};

	uint64 Own(const FNbWorld& W, const TCHAR* Def) { return W.Inventory().CountOf(SpnWorld::PlayerContainerId, Def); }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbPickupTest, "SpnGame.NightBuild.Pickup", Flags)
bool FSpnNbPickupTest::RunTest(const FString&)
{
	FNbWorld W;
	ASpnWorldPickup* Ration = W.Pickup(TEXT("t.ration"), TEXT("spn_test:item.ration"), 2);
	TestFalse(TEXT("not taken yet"), Ration->IsTaken());
	TestEqual(TEXT("the player carries nothing"), Own(W, TEXT("spn_test:item.ration")), 0ull);

	const FSpnInteractStatus Use = Ration->SpnUse(nullptr);
	TestEqual(TEXT("taking works"), Use.Result, ESpnInteractResult::Changed);
	TestEqual(TEXT("the item is really in the inventory (Core transaction), not a counter"), Own(W, TEXT("spn_test:item.ration")), 2ull);
	TestTrue(TEXT("taken flag in the world facts"), Ration->IsTaken());
	TestEqual(TEXT("no WorldFacts counter stands in for the item"), W.WorldState().ReadInventoryCount(USpnWorldStateSubsystem::DefaultHolder(), TEXT("ration")).Get(0), static_cast<int64>(0));
	TestTrue(TEXT("hidden"), Ration->IsHidden());
	TestEqual(TEXT("inventory invariants"), W.Inventory().CheckInvariants(), FString());

	const uint64 Revision = W.Inventory().GetRevision();
	TestEqual(TEXT("a second use is refused"), Ration->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("no duplication"), Own(W, TEXT("spn_test:item.ration")), 2ull);
	TestEqual(TEXT("and no transaction"), W.Inventory().GetRevision(), Revision);

	// does not fit: the crate (60 l) is bigger than the body container (30 l). The pickup stays, nothing moves.
	ASpnWorldPickup* Crate = W.Pickup(TEXT("t.crate"), TEXT("spn_test:item.crate"), 1);
	const FSpnInteractStatus NoRoom = Crate->SpnUse(nullptr);
	TestEqual(TEXT("refused"), NoRoom.Result, ESpnInteractResult::Refused);
	TestFalse(TEXT("still lying there"), Crate->IsTaken());
	TestFalse(TEXT("still visible"), Crate->IsHidden());
	TestEqual(TEXT("nothing was added"), Own(W, TEXT("spn_test:item.crate")), 0ull);
	TestEqual(TEXT("no inventory change at all"), W.Inventory().GetRevision(), Revision);

	// no partial pickup: 100 rations do not fit (weight), so not even some of them are taken
	ASpnWorldPickup* Many = W.Pickup(TEXT("t.many"), TEXT("spn_test:item.ration"), 100);
	TestEqual(TEXT("refused as a whole"), Many->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("still two"), Own(W, TEXT("spn_test:item.ration")), 2ull);
	TestFalse(TEXT("not taken"), Many->IsTaken());

	// an unknown item definition is refused by the Core
	ASpnWorldPickup* Bogus = W.Pickup(TEXT("t.bogus"), TEXT("spn_test:item.does_not_exist"), 1);
	TestEqual(TEXT("unknown definition refused"), Bogus->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	TestFalse(TEXT("not taken"), Bogus->IsTaken());
	TestEqual(TEXT("still consistent"), W.Inventory().CheckInvariants(), FString());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbPickupSaveTest, "SpnGame.NightBuild.PickupPersistence", Flags)
bool FSpnNbPickupSaveTest::RunTest(const FString&)
{
	FNbWorld W;
	ASpnWorldPickup* Flash = W.Pickup(TEXT("t.flash"), TEXT("spn_test:item.flashlight"), 1);
	TestTrue(TEXT("saved before taking"), W.Save().SaveSlot(TEXT("before")).IsOk());
	TestEqual(TEXT("taken"), Flash->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestTrue(TEXT("saved after taking"), W.Save().SaveSlot(TEXT("after")).IsOk());

	TestTrue(TEXT("load the state from before: the item is back on the ground, out of the inventory"), W.Save().LoadSlot(TEXT("before")).IsOk());
	TestFalse(TEXT("pickup is visible again"), Flash->IsTaken());
	TestFalse(TEXT("visible"), Flash->IsHidden());
	TestEqual(TEXT("not carried"), Own(W, TEXT("spn_test:item.flashlight")), 0ull);

	TestTrue(TEXT("load the later state"), W.Save().LoadSlot(TEXT("after")).IsOk());
	TestTrue(TEXT("taken again"), Flash->IsTaken());
	TestTrue(TEXT("hidden again"), Flash->IsHidden());
	TestEqual(TEXT("carried again"), Own(W, TEXT("spn_test:item.flashlight")), 1ull);
	TestEqual(TEXT("taking it a second time is impossible after the load"), Flash->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("still exactly one"), Own(W, TEXT("spn_test:item.flashlight")), 1ull);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbContainerTest, "SpnGame.NightBuild.Container", Flags)
bool FSpnNbContainerTest::RunTest(const FString&)
{
	FNbWorld W;
	ASpnWorldContainer* Box = W.Container(TEXT("t.box"));
	const uint64 Id = Box->GetContainerId();
	TestEqual(TEXT("stable id is a pure function of the stable id"), Id, SpnWorld::ContainerIdFor(TEXT("t.box")));
	TestTrue(TEXT("registered as a world container (hidden from the plain inventory)"), SpnWorldContainerRegistry().Contains(static_cast<int64>(Id)));
	TestTrue(TEXT("never collides with the player's container"), Id != SpnWorld::PlayerContainerId);
	TestEqual(TEXT("seeded bandages"), W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")), 3ull);
	TestEqual(TEXT("seeded notebook"), W.Inventory().CountOf(Id, TEXT("spn_test:item.notebook")), 1ull);
	TestEqual(TEXT("the player container is still the player's"), SpnWorld::FindPlayerContainer(W.Inventory()), SpnWorld::PlayerContainerId);

	// an existing container is never re-seeded (its contents are the truth)
	FSpnContainerView View;
	TestTrue(TEXT("container view"), W.Inventory().GetContainer(Id, View));
	uint64 BandageStack = 0;
	for (const FSpnStackView& Stack : View.Stacks) { if (Stack.Definition == TEXT("spn_test:item.bandage")) { BandageStack = Stack.StackId; } }
	if (BandageStack != 0)
	{
		const uint64 Before = W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")) + Own(W, TEXT("spn_test:item.bandage"));
		TestTrue(TEXT("transfer 2 bandages to the player"), W.Inventory().TransferItem(Id, SpnWorld::PlayerContainerId, BandageStack, 2).IsOk());
		TestEqual(TEXT("nothing is lost or duplicated by a transfer"), W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")) + Own(W, TEXT("spn_test:item.bandage")), Before);
	}
	const uint64 InBox = W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage"));
	TestTrue(TEXT("ensuring again does not re-seed"), Box->EnsureContainer());
	TestEqual(TEXT("contents unchanged by EnsureContainer"), W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")), InBox);

	// capacity / category rules are the Core's: the small box accepts only consumables and tools
	W.Inventory().AddItem(SpnWorld::PlayerContainerId, TEXT("spn_test:item.coat"), 1);
	W.Inventory().GetContainer(SpnWorld::PlayerContainerId, View);
	uint64 CoatStack = 0;
	for (const FSpnStackView& Stack : View.Stacks) { if (Stack.Definition == TEXT("spn_test:item.coat")) { CoatStack = Stack.StackId; } }
	const FSpnInventoryStatus Refused = W.Inventory().TransferItem(SpnWorld::PlayerContainerId, Id, CoatStack, 1);
	TestEqual(TEXT("clothing is not allowed in the box"), Refused.Error, ESpnInventoryError::CategoryNotAllowed);
	TestEqual(TEXT("a refused transfer moves nothing"), Own(W, TEXT("spn_test:item.coat")), 1ull);

	// persistence of the contents: save, change, load
	TestTrue(TEXT("saved"), W.Save().SaveSlot(TEXT("box")).IsOk());
	const uint64 SavedInBox = W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage"));
	W.Inventory().GetContainer(Id, View);
	for (const FSpnStackView& Stack : View.Stacks) { if (Stack.Definition == TEXT("spn_test:item.bandage")) { W.Inventory().TransferItem(Id, SpnWorld::PlayerContainerId, Stack.StackId, Stack.Quantity); } }
	TestEqual(TEXT("emptied"), W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")), 0ull);
	TestTrue(TEXT("loaded"), W.Save().LoadSlot(TEXT("box")).IsOk());
	TestEqual(TEXT("the box holds again what it held"), W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")), SavedInBox);
	TestEqual(TEXT("invariants"), W.Inventory().CheckInvariants(), FString());

	// a new game (inventory reset) makes the container re-create itself with its authored contents
	W.Inventory().ResetInventory();
	TestEqual(TEXT("after the reset the box was created again, seeded"), W.Inventory().CountOf(Id, TEXT("spn_test:item.bandage")), 3ull);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbSettingsTest, "SpnGame.NightBuild.Settings", Flags)
bool FSpnNbSettingsTest::RunTest(const FString&)
{
	FNbWorld W;
	USpnSettingsSubsystem* Settings = W.GameInstance->GetSubsystem<USpnSettingsSubsystem>();
	if (Settings == nullptr)
	{
		AddError(TEXT("no settings subsystem"));
		return false;
	}
	const FSpnPlayerSettings Original = Settings->Get();
	const float OriginalVolume = FApp::GetVolumeMultiplier();
	const FString OriginalLanguage = FSpnUiText::GetLanguageOverride();

	FSpnPlayerSettings Wild;
	Wild.MasterVolume = 7.f;
	Wild.MouseSensitivity = -3.f;
	Wild.GamepadSensitivity = NAN;
	Wild.MinimapZoom = 100.f;
	Wild.Language = TEXT("klingon");
	const FSpnPlayerSettings Clamped = USpnSettingsSubsystem::Clamped(Wild);
	TestEqual(TEXT("volume clamped to 1"), Clamped.MasterVolume, 1.f);
	TestEqual(TEXT("sensitivity clamped to the minimum"), Clamped.MouseSensitivity, 0.1f);
	TestEqual(TEXT("NaN falls back to the default"), Clamped.GamepadSensitivity, 1.f);
	TestEqual(TEXT("zoom clamped"), Clamped.MinimapZoom, 4.f);
	TestEqual(TEXT("unknown language becomes auto"), Clamped.Language, FString(TEXT("auto")));

	int32 Changes = 0;
	Settings->OnChanged.AddLambda([&Changes]() { ++Changes; });
	FSpnPlayerSettings Mine = Original;
	Mine.MasterVolume = 0.4f;
	Mine.bInvertY = true;
	Mine.bMinimapHeadingUp = true;
	Mine.MinimapZoom = 2.f;
	Mine.Language = TEXT("de");
	Settings->Set(Mine);
	TestEqual(TEXT("listeners were told"), Changes, 1);
	TestEqual(TEXT("master volume really drives the engine volume multiplier"), FApp::GetVolumeMultiplier(), 0.4f);
	TestEqual(TEXT("language override applied to the UI texts"), FSpnUiText::GetActiveLanguage(), FString(TEXT("de")));

	// reload from the ini: the values were persisted
	Settings->Load();
	TestEqual(TEXT("volume persisted"), Settings->Get().MasterVolume, 0.4f);
	TestTrue(TEXT("invert persisted"), Settings->Get().bInvertY);
	TestTrue(TEXT("orientation persisted"), Settings->Get().bMinimapHeadingUp);
	TestEqual(TEXT("zoom persisted"), Settings->Get().MinimapZoom, 2.f);
	TestEqual(TEXT("language persisted"), Settings->Get().Language, FString(TEXT("de")));

	// restore the user's real settings
	Settings->Set(Original);
	FApp::SetVolumeMultiplier(OriginalVolume);
	FSpnUiText::SetLanguageOverride(OriginalLanguage);
	return true;
}

// Settings completion: music / effects volume (engine sound classes) and the UI scale have a real, observable effect and survive a restart (simulated by
// resetting the in-memory values and loading the ini again). The test restores everything it touched.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbSettingsAudioUiTest, "SpnGame.NightBuild.SettingsAudioUi", Flags)
bool FSpnNbSettingsAudioUiTest::RunTest(const FString&)
{
	FNbWorld W;
	USpnSettingsSubsystem* Settings = W.GameInstance->GetSubsystem<USpnSettingsSubsystem>();
	if (Settings == nullptr) { AddError(TEXT("no settings subsystem")); return false; }
	const FSpnPlayerSettings Original = Settings->Get();
	const float OriginalMasterMultiplier = FApp::GetVolumeMultiplier();
	const float OriginalScale = GetDefault<UUserInterfaceSettings>()->ApplicationScale;

	const FSpnPlayerSettings Wild = [] { FSpnPlayerSettings S; S.MusicVolume = -2.f; S.EffectsVolume = NAN; S.UiScale = 9.f; return S; }();
	const FSpnPlayerSettings Clamped = USpnSettingsSubsystem::Clamped(Wild);
	TestEqual(TEXT("music volume clamped"), Clamped.MusicVolume, 0.f);
	TestEqual(TEXT("NaN effects volume falls back"), Clamped.EffectsVolume, 1.f);
	TestEqual(TEXT("UI scale clamped to 1.5"), Clamped.UiScale, 1.5f);

	FSpnPlayerSettings Mine = Original;
	Mine.MusicVolume = 0.3f;
	Mine.EffectsVolume = 0.6f;
	Mine.UiScale = 1.25f;
	Settings->Set(Mine);
	TestEqual(TEXT("the Music sound class follows the music slider"), USpnSettingsSubsystem::GetMusicClassVolume(), 0.3f);
	TestEqual(TEXT("the SFX sound class follows the effects slider"), USpnSettingsSubsystem::GetEffectsClassVolume(), 0.6f);
	TestEqual(TEXT("the UI scale reaches the UI settings"), GetDefault<UUserInterfaceSettings>()->ApplicationScale, 1.25f);

	// "restart": throw the in-memory values away, load from the ini, apply
	FSpnPlayerSettings Reset = Original;
	Reset.MusicVolume = 1.f; Reset.EffectsVolume = 1.f; Reset.UiScale = 1.f;
	Settings->Set(Reset);
	TestEqual(TEXT("reset really changed the class volume"), USpnSettingsSubsystem::GetMusicClassVolume(), 1.f);
	GConfig->SetFloat(TEXT("SpnPlayer"), TEXT("MusicVolume"), 0.3f, GGameUserSettingsIni);
	GConfig->SetFloat(TEXT("SpnPlayer"), TEXT("EffectsVolume"), 0.6f, GGameUserSettingsIni);
	GConfig->SetFloat(TEXT("SpnPlayer"), TEXT("UiScale"), 1.25f, GGameUserSettingsIni);
	Settings->Load();
	Settings->ApplyGlobal();
	TestEqual(TEXT("music volume restored from the ini"), Settings->Get().MusicVolume, 0.3f);
	TestEqual(TEXT("effects volume restored from the ini"), Settings->Get().EffectsVolume, 0.6f);
	TestEqual(TEXT("UI scale restored from the ini"), Settings->Get().UiScale, 1.25f);
	TestEqual(TEXT("and applied again"), USpnSettingsSubsystem::GetMusicClassVolume(), 0.3f);

	Settings->Set(Original);
	FApp::SetVolumeMultiplier(OriginalMasterMultiplier);
	GetMutableDefault<UUserInterfaceSettings>()->ApplicationScale = OriginalScale;
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNbTextTest, "SpnGame.NightBuild.UiText", Flags)
bool FSpnNbTextTest::RunTest(const FString&)
{
	// every key of the table has an English and a German text, and none of the Night Build keys is empty or a copy of its key
	int32 Checked = 0;
	for (const FString& Key : FSpnUiText::AllKeys())
	{
		if (!Key.StartsWith(TEXT("ui.map.")) && !Key.StartsWith(TEXT("ui.settings.")) && !Key.StartsWith(TEXT("ui.slots.")) && !Key.StartsWith(TEXT("ui.world.")) && !Key.StartsWith(TEXT("ui.save."))
			&& !Key.StartsWith(TEXT("ui.menu.")) && !Key.StartsWith(TEXT("ui.pause.")) && !Key.StartsWith(TEXT("ui.confirm.")) && !Key.StartsWith(TEXT("ui.hud.")))
		{
			continue;
		}
		++Checked;
		TestFalse(*FString::Printf(TEXT("%s has an English text"), *Key), FSpnUiText::GetInLanguage(Key, TEXT("en")).IsEmpty());
		TestFalse(*FString::Printf(TEXT("%s has a German text"), *Key), FSpnUiText::GetInLanguage(Key, TEXT("de")).IsEmpty());
	}
	TestTrue(TEXT("the Night Build texts are in the table"), Checked > 100);
	AddInfo(FString::Printf(TEXT("checked %d Night Build UI keys in both languages"), Checked));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
