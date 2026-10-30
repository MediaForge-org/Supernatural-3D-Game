#pragma once

#include "CoreMinimal.h"

// User-facing text of the SPN user interface (M2B3.5). English and German, stable keys, English fallback.
//
// WHY NOT LOCTEXT / STRING TABLE ASSETS: the project builds its content from code and text files; a localisation target needs a gather
// run and committed .po/.archive files that cannot be reviewed from here. This table is the same idea in plain C++: every user-facing
// string has a stable key, a German and an English text; the key is the identifier a later string-table migration would reuse (nothing in
// widget logic contains a user-facing literal). The inventory view-model's message keys (spn.inventory.error.*, spn.inventory.load.*, ...)
// are looked up here too, so the German texts of those messages live in one place.
//
// LOOKUP: current language -> English -> the fallback text the caller passes (e.g. the view-model's English text) -> the key itself.
// LANGUAGE: "auto" follows the engine's current culture (de, de-DE, ... -> German, everything else -> English); SetLanguageOverride("de"|"en")
// forces one (console command Spn.UI.Language, tests).

class SUPERNATURALPROT_API FSpnUiText
{
public:
	/** The text for the key in the active language. Args replace {0}, {1}, ... */
	static FText Get(const FString& Key, const TArray<FText>& Args = {}, const FText& Fallback = FText::GetEmpty());
	static FText Get(const TCHAR* Key) { return Get(FString(Key)); }

	/** True if the table knows the key (in English). */
	static bool Has(const FString& Key);
	/** Text of the key in a specific language ("en" or "de"); empty if unknown. Used by tests to compare both languages. */
	static FString GetInLanguage(const FString& Key, const FString& Language);

	/** "auto", "en" or "de". */
	static void SetLanguageOverride(const FString& Language);
	static FString GetLanguageOverride();
	/** "en" or "de": what Get() currently uses. */
	static FString GetActiveLanguage();

	/** All keys (for tests: every key has both languages). */
	static TArray<FString> AllKeys();

	// --- display formatting of gameplay numbers (presentation only; the Core keeps grams and millilitres) ---
	static FText FormatWeight(int64 Grams);
	static FText FormatVolume(int64 Milliliters);
	/** "3 / 8", or "3 (no limit)". */
	static FText FormatMeter(int64 Used, int64 Max, bool bWeight, bool bVolume);
};
