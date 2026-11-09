# Architekturprüfung der Dialog-, Beweis- und Ermittlungsgrundlagen (SPN-DEV-03, 08.11.2026)

Geprüft: `WorldFacts`, `GameEventBus`, `StrongId`/`ContentId`, die neuen Module `Evidence`, `Dialogue`, `InvestigationAssist`, `CasePrototype` (Übernahme). Keine Unreal-Abnahme, keine Persistenz außerhalb von WorldFacts.

## 1. WorldFacts als Zustandsquelle und GameEventBus als Benachrichtigung

**Befund.** Das vorhandene Muster ist bereits richtig: `InteractionSystem` und `ObjectPersistence` schreiben zuerst in WorldFacts und veröffentlichen danach ein Ereignis (`bus_->Publish`, Zeilen 247/284/314 bzw. 345). Der Bus ist also Benachrichtigung, nicht Speicher.

**Grenzen, die beachtet werden müssen.**
- Ereignisse sind nicht persistent. Eine Queue kann voll sein; dann wird ein Ereignis verworfen und gezählt (`DroppedCount`). Ein Listener darf daher keinen Zustand aufbauen, der nur über Ereignisse entsteht. Der Zustand gehört immer in WorldFacts.
- `GameEventType` ist ein geschlossenes Enum mit sieben Werten, das Interaktionen und Persistenz beschreibt. Neue Typen für Beweise oder Dialoge würden dieses Enum ändern.

**Entscheidung (Architektur, kein Spielgesetz).** Die neuen Module veröffentlichen **keine** Ereignisse. Sie schreiben nur WorldFacts. Begründung: Der Fortschritt ist über das Journal von WorldFacts vollständig nachvollziehbar; ein zusätzlicher Ereignisstrom würde ohne Bedarf eine zweite Quelle für Benachrichtigungen schaffen. Wenn Unreal später Journal-UI oder Audio braucht, werden **zusätzliche** Typen (z. B. `EvidenceRegistered`, `DialogueEnded`) vorgeschlagen und dann ergänzt, und nur nach dem Schreiben in WorldFacts veröffentlicht. Vorschlag, nicht umgesetzt.

## 2. Lokale Kennungen gegenüber StrongId und ContentId

| Konvention | Zweck | Verhältnis zu den neuen Kennungen |
|---|---|---|
| `StrongId<Tag, Rep>` (`StrongId.h`) | numerische Laufzeit-Handles (z. B. `EntityId`) | nicht geeignet: Beweise und Dialoge brauchen stabile Namen in Speicherständen und Daten |
| `ContentId` (`ContentId.h`) | namespacete Inhalts-IDs `ns:pfad` (max. 96) | Die lokalen Kennungen sind eine **Teilmenge** der Pfadsegmente (`[a-z][a-z0-9_]*`). Eine spätere Abbildung ist `placeholder:evidence.<id>` ohne Änderung der Kennung. |
| `IsValidLocalId` (neu, `InvestigationIds.h`) | Fakt-Schlüssel ohne Mehrdeutigkeit | keine Punkte, keine Doppelpunkte, max. 48 |

**Kollisionen.**
- Evidenz-IDs sind im Katalog eindeutig (Duplikate werden abgelehnt).
- Dialog-IDs sind innerhalb ihres Dialogs eindeutig; Knoten-IDs und Optionen-IDs sind je Dialog bzw. Knoten geprüft.
- Fakt-Schlüssel kollidieren nicht, weil die Präfixe getrennt sind: `placeholder.evidence.<id>.known|found`, `placeholder.journal.evidence.<id>`, `placeholder.dialogue.<d>.node|done`, `placeholder.assist.preset|<opt>.override`, `npc.<npc>.knows.<topic>`, `placeholder.case.proto.*`.
- **Längenprüfung:** schlimmster Fall je Schlüssel unter den Grenzen (`kMaxFactKeyLength` = 128): Journal 77, NPC-Wissen 107, Dialogzustand 74, Assist 39. Alle innerhalb.
- Ein Risiko blieb: Die Sprecher-ID `player` könnte als NPC-ID vergeben werden. **Behoben** (siehe 3).

**Erweiterbarkeit und Unreal.** Die Kennungen sind Text, serialisierbar und diff-freundlich; die Katalogs- und Graphdaten sind immutable und können aus Daten geladen werden. Für die Unreal-Integration bleibt die Abbildung auf `ContentId` (Dateiformat, Assets) eine Aufgabe, keine Änderung der Schlüssel.

## 3. Atomare Effekte, Save/Load, ungültige Übergänge, CasePrototype-Übernahme

**Gefundene Risiken und Behebung** (jeweils mit Regressionstest):
1. **Dialogeffekte konnten fremde Zustände überschreiben** (z. B. `placeholder.journal.evidence.*`, `placeholder.dialogue.*.done`). Das hätte EVD-01 und die Gesprächslogik umgangen. **Behoben:** Die Namensräume `placeholder.journal.`, `placeholder.dialogue.`, `placeholder.evidence.`, `placeholder.assist.`, `placeholder.case.` sind für Effekte gesperrt (`DialogueDef::Create` lehnt sie ab). Test: `Regression: a dialogue effect cannot write another module's own state`.
2. **Sprecher-ID `player` als NPC-ID möglich.** **Behoben:** `player` ist als NPC-ID reserviert. Test: `...the NPC id 'player' is reserved`.
3. **Übernahme in CasePrototype prüfte die IDs nicht.** Ungültige IDs hätten beliebige Schlüssel gelesen. **Behoben:** neuer Fehlercode `InvalidId`. Test: `Integration: invalid ids are refused...`.

**Geprüft ohne Befund:**
- Atomarität: Eine Auswahl schreibt auf eine Kopie und übernimmt sie nur bei Erfolg. Test für einen einzelnen und für mehrere Effekte, bei denen der erste Effekt verworfen wird.
- Save/Load mitten im Gespräch und nach dem Abschluss.
- Ungültige Übergänge: Auswahl ohne Start, nach Ende, unbekannte oder nicht verfügbare Option, beschädigter Zustand (falscher Typ, fehlender Knoten).
- Evidenz: Sammeln vor Fund abgelehnt; keine Doppelregistrierung; Registrierung nur über `CollectEvidence`.

**Übernahme in CasePrototype.** `AdoptInvestigationProgress` liest nur Fakt-Schlüssel und setzt die Platzhalterflags. Die Abschlussregel bleibt manuell (EVD-05). Test: Ready ohne Auto-Abschluss, Resolved erst auf Anforderung.

## 4. Regressionstests
- 3 Dialog-Regressionen (Namensräume, `player`, Atomarität bei mehreren Effekten).
- 3 Integrationstests (Freischaltung erst nach Sammeln, ungültige IDs, Ende-zu-Ende-Ablauf mit zwei Save/Load-Zyklen).

## 5. Technische Korrekturen
Die drei Befunde aus Abschnitt 3 sind die einzigen Codeänderungen dieser Prüfung. Keine Refactorings.

## Verbleibende Voraussetzungen für die Unreal-Integration
- Actor für den Zeugen und Auswahl-UI, die `DialogueView` anzeigt und `DialogueChoose` aufruft.
- Beweisobjekt, das `FindEvidence` und über die Sammelinteraktion `CollectEvidence` aufruft (Muster: `ASpnTestPickup`).
- Journal-UI, die `RegisteredEvidence` liest.
- Anbindung der Ermittlungshilfe an Marker und Hinweise (Werte kommen aus den Presets als Daten).
- Persistenz: nur Einbindung in `USpnSaveSubsystem`; WorldFacts-Format ist vorhanden.
- Editor-PIE-Abnahme nach `P0-ACCEPT-EDITOR`. Keine Unreal-Abnahme ist hier behauptet.
