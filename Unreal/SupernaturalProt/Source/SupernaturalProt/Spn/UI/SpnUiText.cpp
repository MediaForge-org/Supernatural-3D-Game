#include "Spn/UI/SpnUiText.h"

#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Text.h"

namespace
{
	struct FEntry
	{
		const TCHAR* Key;
		const TCHAR* En;
		const TCHAR* De;
	};

	// Keys: ui.* = interface; spn.* = message keys of the inventory view-model and the save subsystem; item.* / cat.* / slot.* / kind.* = names of the
	// TEMPORARY test content (Content/SpnTest/Inventory/TestItemCatalog.json); real content will bring its own names (presentation data assets).
	const FEntry GTable[] = {
		// --- screen ---
		{TEXT("ui.inventory.title"), TEXT("INVENTORY"), TEXT("INVENTAR")},
		{TEXT("ui.equipment.title"), TEXT("EQUIPMENT"), TEXT("AUSRÜSTUNG")},
		{TEXT("ui.items.title"), TEXT("ITEMS"), TEXT("GEGENSTÄNDE")},
		{TEXT("ui.details.title"), TEXT("DETAILS"), TEXT("DETAILS")},
		{TEXT("ui.actions.title"), TEXT("ACTIONS"), TEXT("AKTIONEN")},
		{TEXT("ui.capacity.title"), TEXT("CAPACITY"), TEXT("KAPAZITÄT")},
		{TEXT("ui.capacity.stacks"), TEXT("Stacks"), TEXT("Stapel")},
		{TEXT("ui.capacity.weight"), TEXT("Weight"), TEXT("Gewicht")},
		{TEXT("ui.capacity.volume"), TEXT("Volume"), TEXT("Volumen")},
		{TEXT("ui.capacity.unlimited"), TEXT("{0} (no limit)"), TEXT("{0} (unbegrenzt)")},
		{TEXT("ui.empty.items"), TEXT("This container is empty."), TEXT("Dieser Behälter ist leer.")},
		{TEXT("ui.empty.inactive"), TEXT("The inventory is not available."), TEXT("Das Inventar ist nicht verfügbar.")},
		{TEXT("ui.empty.nocontainer"), TEXT("No container."), TEXT("Kein Behälter.")},
		{TEXT("ui.list.window"), TEXT("Showing {0}–{1} of {2}"), TEXT("Zeige {0}–{1} von {2}")},
		{TEXT("ui.details.none"), TEXT("Select an item to see its details."), TEXT("Wähle einen Gegenstand, um Details zu sehen.")},
		{TEXT("ui.details.category"), TEXT("Category"), TEXT("Kategorie")},
		{TEXT("ui.details.quantity"), TEXT("Quantity"), TEXT("Menge")},
		{TEXT("ui.details.stack"), TEXT("Stack"), TEXT("Stapel")},
		{TEXT("ui.details.stack.value"), TEXT("{0} of {1}"), TEXT("{0} von {1}")},
		{TEXT("ui.details.stack.unique"), TEXT("unique item"), TEXT("Einzelstück")},
		{TEXT("ui.details.weight"), TEXT("Weight"), TEXT("Gewicht")},
		{TEXT("ui.details.volume"), TEXT("Volume"), TEXT("Volumen")},
		{TEXT("ui.details.each"), TEXT("{0} each, {1} total"), TEXT("{0} pro Stück, {1} gesamt")},
		{TEXT("ui.details.fits"), TEXT("Fits"), TEXT("Passt in")},
		{TEXT("ui.details.fits.none"), TEXT("cannot be equipped"), TEXT("nicht ausrüstbar")},
		{TEXT("ui.details.condition"), TEXT("Condition"), TEXT("Zustand")},
		{TEXT("ui.details.equipped"), TEXT("Equipped: {0}"), TEXT("Ausgerüstet: {0}")},
		{TEXT("ui.details.testdata"), TEXT("TEST CONTENT"), TEXT("TESTINHALT")},
		{TEXT("ui.slot.free"), TEXT("empty"), TEXT("leer")},
		{TEXT("ui.badge.equipped"), TEXT("EQUIPPED"), TEXT("AUSGERÜSTET")},
		{TEXT("ui.quantity"), TEXT("Quantity"), TEXT("Menge")},
		{TEXT("ui.quantity.value"), TEXT("{0} of {1}"), TEXT("{0} von {1}")},
		{TEXT("ui.actions.none"), TEXT("No action available."), TEXT("Keine Aktion verfügbar.")},
		{TEXT("ui.actions.unavailable"), TEXT("not available: {0}"), TEXT("nicht möglich: {0}")},
		{TEXT("ui.close"), TEXT("Close"), TEXT("Schließen")},
		// --- actions ---
		{TEXT("ui.action.equip"), TEXT("Equip ({0})"), TEXT("Ausrüsten ({0})")},
		{TEXT("ui.action.unequip"), TEXT("Unequip ({0})"), TEXT("Ablegen ({0})")},
		{TEXT("ui.action.transfer"), TEXT("Move to {0}"), TEXT("Verschieben nach {0}")},
		{TEXT("ui.action.split"), TEXT("Split off {0}"), TEXT("{0} abspalten")},
		{TEXT("ui.action.merge"), TEXT("Combine stacks"), TEXT("Stapel zusammenführen")},
		{TEXT("ui.action.discard"), TEXT("Discard"), TEXT("Wegwerfen")},
		// --- discard confirmation ---
		{TEXT("ui.dialog.discard.title"), TEXT("Discard this item?"), TEXT("Gegenstand wegwerfen?")},
		{TEXT("ui.dialog.discard.body"), TEXT("{0} × {1} will be destroyed. This cannot be undone."), TEXT("{0} × {1} wird zerstört. Das lässt sich nicht rückgängig machen.")},
		{TEXT("ui.dialog.confirm"), TEXT("Discard"), TEXT("Wegwerfen")},
		{TEXT("ui.dialog.cancel"), TEXT("Cancel"), TEXT("Abbrechen")},
		// --- input hints ---
		{TEXT("ui.hint.keyboard"), TEXT("Arrows: move   Enter: use   Tab: next container   ←/→: panel   +/−: quantity   Esc / I: close"), TEXT("Pfeile: bewegen   Eingabe: benutzen   Tab: nächster Behälter   ←/→: Bereich   +/−: Menge   Esc / I: schließen")},
		{TEXT("ui.hint.gamepad"), TEXT("D-pad: move   Cross: use   Circle: back   L1/R1: container   L2/R2: quantity   Options: close"), TEXT("Steuerkreuz: bewegen   Kreuz: benutzen   Kreis: zurück   L1/R1: Behälter   L2/R2: Menge   Options: schließen")},
		{TEXT("ui.hint.dialog"), TEXT("Enter / Cross: confirm   Esc / Circle: cancel"), TEXT("Eingabe / Kreuz: bestätigen   Esc / Kreis: abbrechen")},
		// --- view-model: errors ---
		{TEXT("spn.inventory.ok"), TEXT("Done."), TEXT("Erledigt.")},
		{TEXT("spn.inventory.error.no_catalog"), TEXT("The inventory is not available."), TEXT("Das Inventar ist nicht verfügbar.")},
		{TEXT("spn.inventory.error.invalid_argument"), TEXT("That action is not valid."), TEXT("Diese Aktion ist nicht gültig.")},
		{TEXT("spn.inventory.error.catalog_locked"), TEXT("The item definitions cannot change while containers exist."), TEXT("Die Gegenstandsdefinitionen lassen sich nicht ändern, solange Behälter existieren.")},
		{TEXT("spn.inventory.error.internal"), TEXT("Something went wrong. Nothing was changed."), TEXT("Etwas ist schiefgelaufen. Es wurde nichts verändert.")},
		{TEXT("spn.inventory.error.invalid_id"), TEXT("That is not a valid choice."), TEXT("Das ist keine gültige Auswahl.")},
		{TEXT("spn.inventory.error.unknown_definition"), TEXT("Unknown item."), TEXT("Unbekannter Gegenstand.")},
		{TEXT("spn.inventory.error.unknown_container"), TEXT("That container does not exist."), TEXT("Diesen Behälter gibt es nicht.")},
		{TEXT("spn.inventory.error.duplicate_container"), TEXT("That container already exists."), TEXT("Diesen Behälter gibt es bereits.")},
		{TEXT("spn.inventory.error.unknown_item"), TEXT("That item is no longer there."), TEXT("Dieser Gegenstand ist nicht mehr da.")},
		{TEXT("spn.inventory.error.invalid_quantity"), TEXT("That quantity is not possible."), TEXT("Diese Menge ist nicht möglich.")},
		{TEXT("spn.inventory.error.not_stackable"), TEXT("This item cannot be stacked."), TEXT("Dieser Gegenstand lässt sich nicht stapeln.")},
		{TEXT("spn.inventory.error.stack_mismatch"), TEXT("These items cannot be combined."), TEXT("Diese Gegenstände lassen sich nicht zusammenführen.")},
		{TEXT("spn.inventory.error.same_container"), TEXT("It is already there."), TEXT("Es ist bereits dort.")},
		{TEXT("spn.inventory.error.container_not_empty"), TEXT("The container is not empty."), TEXT("Der Behälter ist nicht leer.")},
		{TEXT("spn.inventory.error.category_not_allowed"), TEXT("This kind of item is not allowed here."), TEXT("Diese Art von Gegenstand ist hier nicht erlaubt.")},
		{TEXT("spn.inventory.error.stack_limit"), TEXT("No room: too many different items."), TEXT("Kein Platz: zu viele verschiedene Gegenstände.")},
		{TEXT("spn.inventory.error.weight_limit"), TEXT("Too heavy for this container."), TEXT("Zu schwer für diesen Behälter.")},
		{TEXT("spn.inventory.error.volume_limit"), TEXT("Too bulky for this container."), TEXT("Zu sperrig für diesen Behälter.")},
		{TEXT("spn.inventory.error.item_equipped"), TEXT("Take it off first."), TEXT("Lege es zuerst ab.")},
		{TEXT("spn.inventory.error.unknown_slot"), TEXT("There is no such slot here."), TEXT("Diesen Platz gibt es hier nicht.")},
		{TEXT("spn.inventory.error.not_equippable"), TEXT("This cannot be equipped there."), TEXT("Das lässt sich dort nicht ausrüsten.")},
		{TEXT("spn.inventory.error.already_equipped"), TEXT("Already equipped."), TEXT("Bereits ausgerüstet.")},
		{TEXT("spn.inventory.error.slot_occupied"), TEXT("The slot is occupied."), TEXT("Der Platz ist belegt.")},
		{TEXT("spn.inventory.error.slot_empty"), TEXT("Nothing is equipped there."), TEXT("Dort ist nichts ausgerüstet.")},
		{TEXT("spn.inventory.error.invalid_state"), TEXT("The item data is not valid."), TEXT("Die Gegenstandsdaten sind nicht gültig.")},
		{TEXT("spn.inventory.error.overflow"), TEXT("The numbers are too large."), TEXT("Die Zahlen sind zu groß.")},
		{TEXT("spn.inventory.error.limit_exceeded"), TEXT("A limit was reached."), TEXT("Ein Limit wurde erreicht.")},
		{TEXT("spn.inventory.error.invalid_definition"), TEXT("An item definition is not valid."), TEXT("Eine Gegenstandsdefinition ist nicht gültig.")},
		{TEXT("spn.inventory.error.duplicate_definition"), TEXT("An item definition appears twice."), TEXT("Eine Gegenstandsdefinition kommt doppelt vor.")},
		{TEXT("spn.inventory.error.unknown"), TEXT("Unknown error."), TEXT("Unbekannter Fehler.")},
		// --- view-model: action results and notices ---
		{TEXT("spn.inventory.action.equip"), TEXT("Equipped."), TEXT("Ausgerüstet.")},
		{TEXT("spn.inventory.action.unequip"), TEXT("Unequipped."), TEXT("Abgelegt.")},
		{TEXT("spn.inventory.action.transfer"), TEXT("Moved."), TEXT("Verschoben.")},
		{TEXT("spn.inventory.action.split"), TEXT("Split."), TEXT("Abgespalten.")},
		{TEXT("spn.inventory.action.merge"), TEXT("Combined."), TEXT("Zusammengeführt.")},
		{TEXT("spn.inventory.action.discard"), TEXT("Discarded."), TEXT("Weggeworfen.")},
		{TEXT("spn.inventory.no_default_action"), TEXT("Nothing to do with this item here."), TEXT("Hier gibt es mit diesem Gegenstand nichts zu tun.")},
		{TEXT("spn.inventory.load.migrated_empty"), TEXT("This save is from before inventories existed. The inventory starts empty."), TEXT("Dieser Spielstand stammt aus der Zeit vor dem Inventar. Das Inventar beginnt leer.")},
		{TEXT("spn.inventory.load.catalog_unverified"), TEXT("This save does not record its item definitions; they could not be verified."), TEXT("Dieser Spielstand speichert seine Gegenstandsdefinitionen nicht; sie ließen sich nicht prüfen.")},
		{TEXT("spn.inventory.load.catalog_mismatch"), TEXT("This save was made with different item definitions and was not loaded. Nothing was changed."), TEXT("Dieser Spielstand wurde mit anderen Gegenstandsdefinitionen erstellt und nicht geladen. Es wurde nichts verändert.")},
		{TEXT("spn.inventory.load.catalog_mismatch_accepted"), TEXT("This save was made with different item definitions. Item data may have changed."), TEXT("Dieser Spielstand wurde mit anderen Gegenstandsdefinitionen erstellt. Gegenstandsdaten können sich geändert haben.")},
		{TEXT("spn.save.recovered_previous"), TEXT("The latest save was damaged; the one before it was loaded."), TEXT("Der neueste Spielstand war beschädigt; der davor wurde geladen.")},
		{TEXT("spn.save.load_failed"), TEXT("Loading failed. Nothing was changed."), TEXT("Laden fehlgeschlagen. Es wurde nichts verändert.")},
		{TEXT("spn.save.save_failed"), TEXT("Saving failed. The previous save is unchanged."), TEXT("Speichern fehlgeschlagen. Der vorherige Spielstand ist unverändert.")},
		// --- TEMPORARY TEST CONTENT (spn_test:) ---
		{TEXT("item.spn_test:item.ration.name"), TEXT("Test ration"), TEXT("Test-Ration")},
		{TEXT("item.spn_test:item.ration.desc"), TEXT("Placeholder for a stackable consumable (stack limit 5)."), TEXT("Platzhalter für ein stapelbares Verbrauchsgut (Stapelgrenze 5).")},
		{TEXT("item.spn_test:item.bandage.name"), TEXT("Test bandage"), TEXT("Test-Verband")},
		{TEXT("item.spn_test:item.bandage.desc"), TEXT("Placeholder for a second stackable consumable (stack limit 10)."), TEXT("Platzhalter für ein zweites stapelbares Verbrauchsgut (Stapelgrenze 10).")},
		{TEXT("item.spn_test:item.flashlight.name"), TEXT("Test flashlight"), TEXT("Test-Taschenlampe")},
		{TEXT("item.spn_test:item.flashlight.desc"), TEXT("Placeholder for a unique tool that can be held in the hand."), TEXT("Platzhalter für ein einzelnes Werkzeug, das in der Hand gehalten werden kann.")},
		{TEXT("item.spn_test:item.notebook.name"), TEXT("Test notebook"), TEXT("Test-Notizbuch")},
		{TEXT("item.spn_test:item.notebook.desc"), TEXT("Placeholder for a unique item that cannot be equipped."), TEXT("Platzhalter für einen einzelnen Gegenstand, der sich nicht ausrüsten lässt.")},
		{TEXT("item.spn_test:item.coat.name"), TEXT("Test coat"), TEXT("Test-Mantel")},
		{TEXT("item.spn_test:item.coat.desc"), TEXT("Placeholder for clothing worn on the torso; too bulky for the small box."), TEXT("Platzhalter für Kleidung am Oberkörper; zu sperrig für die kleine Kiste.")},
		{TEXT("item.spn_test:item.crate.name"), TEXT("Test crate"), TEXT("Test-Kiste")},
		{TEXT("item.spn_test:item.crate.desc"), TEXT("Placeholder for a heavy, bulky object that only the trunk can hold."), TEXT("Platzhalter für ein schweres, sperriges Objekt, das nur der Kofferraum fasst.")},
		{TEXT("cat.spn_test:cat.consumable"), TEXT("Consumable"), TEXT("Verbrauchsgut")},
		{TEXT("cat.spn_test:cat.tool"), TEXT("Tool"), TEXT("Werkzeug")},
		{TEXT("cat.spn_test:cat.clothing"), TEXT("Clothing"), TEXT("Kleidung")},
		{TEXT("cat.spn_test:cat.bulky"), TEXT("Bulky"), TEXT("Sperrgut")},
		{TEXT("slot.spn_test:slot.hand"), TEXT("Hand"), TEXT("Hand")},
		{TEXT("slot.spn_test:slot.torso"), TEXT("Torso"), TEXT("Oberkörper")},
		{TEXT("kind.spn_test:container.body"), TEXT("Carried"), TEXT("Am Körper")},
		{TEXT("kind.spn_test:container.trunk"), TEXT("Trunk"), TEXT("Kofferraum")},
		{TEXT("kind.spn_test:container.box"), TEXT("Small box"), TEXT("Kleine Kiste")},
	};

	const TMap<FString, const FEntry*>& Index()
	{
		static const TMap<FString, const FEntry*> Map = []
		{
			TMap<FString, const FEntry*> Out;
			for (const FEntry& Entry : GTable)
			{
				Out.Add(Entry.Key, &Entry);
			}
			return Out;
		}();
		return Map;
	}

	FString GOverride = TEXT("auto");
}

FString FSpnUiText::GetActiveLanguage()
{
	if (GOverride == TEXT("de") || GOverride == TEXT("en"))
	{
		return GOverride;
	}
	const FString Culture = FInternationalization::Get().GetCurrentCulture()->GetName();
	return Culture.StartsWith(TEXT("de")) ? TEXT("de") : TEXT("en");
}

void FSpnUiText::SetLanguageOverride(const FString& Language)
{
	GOverride = Language == TEXT("de") || Language == TEXT("en") ? Language : FString(TEXT("auto"));
}

FString FSpnUiText::GetLanguageOverride()
{
	return GOverride;
}

bool FSpnUiText::Has(const FString& Key)
{
	return Index().Contains(Key);
}

FString FSpnUiText::GetInLanguage(const FString& Key, const FString& Language)
{
	const FEntry* const* Entry = Index().Find(Key);
	if (Entry == nullptr)
	{
		return FString();
	}
	return Language == TEXT("de") ? FString((*Entry)->De) : FString((*Entry)->En);
}

FText FSpnUiText::Get(const FString& Key, const TArray<FText>& Args, const FText& Fallback)
{
	FString Pattern = GetInLanguage(Key, GetActiveLanguage());
	if (Pattern.IsEmpty())
	{
		Pattern = GetInLanguage(Key, TEXT("en"));
	}
	if (Pattern.IsEmpty())
	{
		if (!Fallback.IsEmpty())
		{
			return Fallback;
		}
		return FText::FromString(Key);
	}
	if (Args.IsEmpty())
	{
		return FText::FromString(Pattern);
	}
	FFormatOrderedArguments Ordered;
	for (const FText& Arg : Args)
	{
		Ordered.Add(Arg);
	}
	return FText::Format(FText::FromString(Pattern), Ordered);
}

TArray<FString> FSpnUiText::AllKeys()
{
	TArray<FString> Keys;
	for (const FEntry& Entry : GTable)
	{
		Keys.Add(Entry.Key);
	}
	return Keys;
}

FText FSpnUiText::FormatWeight(int64 Grams)
{
	if (Grams >= 1000)
	{
		const double Kg = static_cast<double>(Grams) / 1000.0;
		return FText::FromString(FString::Printf(TEXT("%.2f kg"), Kg).Replace(TEXT("."), GetActiveLanguage() == TEXT("de") ? TEXT(",") : TEXT(".")));
	}
	return FText::FromString(FString::Printf(TEXT("%lld g"), Grams));
}

FText FSpnUiText::FormatVolume(int64 Milliliters)
{
	if (Milliliters >= 1000)
	{
		const double Liters = static_cast<double>(Milliliters) / 1000.0;
		return FText::FromString(FString::Printf(TEXT("%.2f l"), Liters).Replace(TEXT("."), GetActiveLanguage() == TEXT("de") ? TEXT(",") : TEXT(".")));
	}
	return FText::FromString(FString::Printf(TEXT("%lld ml"), Milliliters));
}

FText FSpnUiText::FormatMeter(int64 Used, int64 Max, bool bWeight, bool bVolume)
{
	const auto One = [bWeight, bVolume](int64 Value) { return bWeight ? FormatWeight(Value) : (bVolume ? FormatVolume(Value) : FText::AsNumber(Value)); };
	if (Max < 0)
	{
		return Get(TEXT("ui.capacity.unlimited"), {One(Used)});
	}
	return FText::FromString(FString::Printf(TEXT("%s / %s"), *One(Used).ToString(), *One(Max).ToString()));
}
