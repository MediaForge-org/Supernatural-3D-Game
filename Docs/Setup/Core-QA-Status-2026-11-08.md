# C++-Core: Build- und Teststatus (SPN-DEVELOPMENT-SESSION-01, 08.11.2026)

Dieser Bericht beschreibt, was in dieser Umgebung (Fedora, GCC/Clang, Ninja, CMake 4.3) tatsächlich gebaut und ausgeführt wurde. Unreal-Engine war hier **nicht** verfügbar; keine Unreal-Abnahme ist daraus abgeleitet.

## Ergebnisse (ausgeführt am 08.11.2026)

| Preset | Configure | Build | Tests |
|---|---|---|---|
| `fedora-clang` | PASS | PASS | 608 / 608 |
| `fedora-clang-asan-ubsan` | PASS | PASS | 608 / 608 |
| `fedora-gcc` | PASS | PASS | 608 / 608 |
| `fedora-gcc-asan-ubsan` | PASS | PASS | 608 / 608 |

Reproduzierbar: `SPN_PRESET_PREFIX=fedora- bash Tools/RunAllChecks.sh`. Bericht: `build/qa-report.txt` (gitignored). Exit-Code 0 = alles bestanden, 1 = Fehler.

Die 608 Tests umfassen 599 bestehende und 9 neue (`CasePrototypeTests.cpp`).

## Behobene Fehler in dieser Sitzung

1. **Build-Fehler unter Clang mit `-Werror`** in `Tests/Core/DoorStatesTests.cpp`: Sieben designierte Initialisierer (`Authored{.open = …}`) ließen das Feld `key` aus, Clang meldet `-Wmissing-designated-field-initializers`, `-Werror` macht daraus einen Fehler. Behoben durch `.key = {}` (leerer Standardwert, kein Verhaltensunterschied). MSVC kennt diese Warnung nicht; die Windows-Konfiguration war davon nicht betroffen. **Der MSVC-Build wurde hier nicht neu geprüft.**

## Neu hinzugefügt

- `Core/include/spn/core/CasePrototype.h`, `Core/src/CasePrototype.cpp`: Zustandsmaschine für PROTOTYPE-CASE-01 (Platzhalter; Abschluss nur bei Zeuge UND Beweis; Zustand nur in WorldFacts; idempotent; keine Schreibvorgänge bei Ablehnung).
- `Tests/Core/CasePrototypeTests.cpp`: 9 Tests (Zustandsübergänge, Ablehnungen, Journal, Typfehler, Speichern/Laden).
- `Tools/RunAllChecks.sh`: Schritt-Berichte, Exit-Codes, Berichtsdatei.

## Unreal-Seite (nicht geprüft)

- Module: `SpnCore`, `SupernaturalProt` (zwei `.Build.cs`), 204 Quelldateien, 170 Automatisierungstests statisch gezählt (nicht ausgeführt).
- Vorhandene Testobjekte: `ASpnTestDoor`, `ASpnTestPickup`, `USpnInventorySubsystem`, `USpnWorldStateSubsystem`, `USpnSaveSubsystem`.
- **Technische Schuld:** `SupernaturalProt/Variant_Combat`, `Variant_Platforming`, `Variant_SideScrolling` sind Vorlagenvarianten der Unreal-Vorlage. Sie sind im Build aktiv, aber in der Projektkonfiguration nicht verwendet. Entfernen erst nach Entwicklerfreigabe.
- NPC-Reaktionen im Spiel: nur die Vorlagen-NPC-Klasse `ASideScrollingNPC` ist vorhanden; kein Spielfigur-Actor mit `NpcMemory`.

## Editor-PIE-Abnahme (nächste Schritte, vom Entwickler auszuführen)

Vollständige Anleitung: `Docs/Setup/Windows-Setup-und-Tests.md`, Abschnitt 8 (Türen, Pickups, Speichern/Laden, Bewegung, Kamera). Vorher:

1. Auf dem Windows-Rechner den Stand aus dem Repo holen (ohne `build/`).
2. Core-Tests mit MSVC (Preset `msvc-ninja` bzw. `RunAllChecks.sh` in Windows-Umgebung) ausführen; der Fix aus dieser Sitzung muss dort ebenfalls grün sein.
3. Editor öffnen, `/Game/SpnTest/Lvl_SpnDoorTest` laden, PIE starten und Abschnitt 8 Punkte 1–5 durchgehen.
4. Ergebnis in `P0-ACCEPT-EDITOR` (TASKS) eintragen.

## Offen

- MSVC-Build und Windows-Tests nach dem Fix nicht erneut gelaufen.
- Unreal-Build und PIE in dieser Umgebung nicht möglich.
- Vorlagenvarianten entfernen (Entwicklerfreigabe).
- Keine Performance-Messungen.
