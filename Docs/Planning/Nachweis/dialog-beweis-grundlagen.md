# Dialog-, Beweis- und Ermittlungsgrundlagen (SPN-DEV-02, 08.11.2026)

- **Entscheidungen:** `Docs/Planning/DECISIONS.md`, Abschnitt J (DLG-01..04, EVD-01..04, DIF-01..03, NPCA-01..03). Quellenstatus: Entwicklerangabe 08.11.2026.
- **Konflikt K-35:** entschieden und geschlossen (SPN-DEV-03, EVD-05). Der Core enthält keine automatische Abschlusslogik; `Ermittlung/23` T-F02 ist umformuliert.
- **Umfang:** rein C++20-Core, keine Unreal-Abhängigkeit, keine Laufzeit-LLM (DLG-04).

## Architekturentscheidungen

1. **Ein Zustandsspeicher.** Alle Zustände liegen in `WorldFacts` (Journal, Speichern/Laden, Golden-Format vorhanden). Kein zweiter Zustandsspeicher, kein zweites Ereignissystem. `GameEventBus` wird **nicht** verwendet, weil die Übergänge schon durch den Fakt-Journal nachvollziehbar sind; ein Ereignisbus würde einen parallelen Zustandsfluss erzeugen.
2. **Lokale Kennungen** (`IsValidLocalId`, `[a-z][a-z0-9_]*`, max. 48). Keine Punkte und keine Doppelpunkte, damit sie in Fakt-Schlüsseln eindeutig bleiben. Gleiche Grammatik wie Pickup-Items. `ContentId` (mit `:`) wurde nicht gewählt, weil es in Fakt-Schlüsseln nicht erlaubt ist.
3. **Keine Kopplung an NpcMemory.** Dialogbedingungen lesen nur Fakten. NPC-Wissen wird als Fakten `npc.<npc>.knows.<topic>` über Effekte gesetzt. NPCA-03: nichts wird automatisch erzeugt.
4. **Integration mit CasePrototype über Schlüssel.** `AdoptInvestigationProgress` liest `placeholder.journal.evidence.<id>` und `placeholder.dialogue.<id>.done`. Kein Header-Import der anderen Module. Die Platzhalter-Abschlussregel bleibt unverändert (Zeuge UND Beweis, manuelles `CloseCaseProto`).
5. **Atomare Auswahl.** `DialogueChoose` schreibt auf eine Kopie der Fakten und übernimmt sie nur bei vollständigem Erfolg. Testfall: fehlschlagender Effekt lässt Speicher und Knoten unverändert.

## Module

| Modul | Dateien | Aufgabe | Tests |
|---|---|---|---|
| Kennungen | `InvestigationIds.h` | gemeinsame Grammatik | (über die anderen Tests) |
| Beweis | `Evidence.h`, `Evidence.cpp` | bekannt / gefunden / registriert; Registrierung erst nach Sammeln; keine Doppelregistrierung | 9 |
| Dialog | `Dialogue.h`, `Dialogue.cpp` | Knoten, Optionen, Bedingungen (wahr, fehlt, gleich), Effekte, Übergänge, Start/Ende | 13 |
| Ermittlungshilfe | `InvestigationAssist.h`, `InvestigationAssist.cpp` | Presets als Daten, Überschreibungen getrennt, Hilfen ohne Wissensänderung | 10 |
| Integration | `CasePrototype.h/.cpp` (`AdoptInvestigationProgress`) | Übernahme echten Fortschritts in die Platzhalterflags | 5 (Integrationsdatei) + Prototyp 9 |

Neue Tests: 37. Gesamt: 645 je Preset (vorher 608).

## Was bewusst nicht implementiert ist

- Schwierigkeitsgrade mit Namen und Werten (DIF-01: Anzahl und Namen offen). Presets sind Daten des Aufrufers; der Core kennt keine Namen.
- Dynamische Dialogzeilen, freie Texteingabe (DLG-01: Umfang offen; DLG-04: keine LLM-Abhängigkeit). Erweiterungspunkt: Textschlüssel eines Knotens oder einer Option könnten zur Laufzeit von einer Quelle aufgelöst werden; Graph und Zustandsschlüssel bleiben gleich.
- Monsterbezogene Beweise und Untersuchungen (EVD-03). Kategorien sind als Platzhalter-Kennung im Katalog vorhanden, ohne Logik.
- Abschlussbedingungen des Falls und Fallinhalt: fallspezifisch (EVD-05); SLICE-CASE-01 O1–O6.

## Offene Design- und Architekturfragen

1. Anzahl und Benennung der Schwierigkeitsgrade (DIF-01).
2. Welche Hilfen es gibt; die drei Platzhalter-Optionen (`found_marker`, `known_hint`, `dialogue_hint`) sind keine Designentscheidung.
3. Umfang der freien Texteingabe im Dialog (DLG-01) und wie sie in den Graphen eingebunden wird.
4. Beweiskategorien über Monster hinaus (EVD-03).
6. Ob Wissen eines NPC aus `NpcMemory` in Dialogbedingungen einfließen soll (Schnittstelle fehlt bewusst).

## Was für die Unreal-Integration fehlt

- Ein Unreal-Actor für den Zeugen (Gesprächsstart, Auswahl-UI, Gesprächsende an `DialogueChoose`).
- Ein Beweisobjekt in der Welt, das `FindEvidence` aufruft und die Sammelinteraktion mit `CollectEvidence` verbindet (Pickup-Muster existiert: `ASpnTestPickup`).
- Ein Journal-UI, das `RegisteredEvidence` anzeigt.
- Anbindung der Ermittlungshilfe an die Anzeige (Marker, Hinweise); die Werte sind nur Daten.
- Persistenz: alles liegt bereits in WorldFacts, die Unreal-Speicherung (`USpnSaveSubsystem`) muss nur eingebunden werden.
- PIE-Abnahme (Editor) erst nach `P0-ACCEPT-EDITOR`.

## Nächster konkreter Schritt

Unreal-Actor für den Zeugen und das Beweisobjekt, dazu eine Testkarte mit einem Raum (Blockout). Architekturprüfung und Voraussetzungen: `architektur-pruefung-dev03.md`.
