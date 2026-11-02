# NightBuild 04 — Interaktion, Türen, Gebäude, persistente Weltzustände

ENGINEERING PROPOSAL / Arbeitsstand, **nicht** Master-Canon. Keine Weltgröße, kein RC-01-/WorldForge-Vertrag, kein Hunter-HQ.

## Master basis
Master consultation protocol (CLAUDE.md §11): **nicht möglich** — `Planning/Master` ist auf diesem Windows-Rechner nicht eingerichtet (`Tools/master-search.sh` meldet „planning master not available"). Es wurde nichts aus dem Master übernommen. Alle Regeln unten (Schlüssel, Aufbrechen, blockiert) sind **Platzhalter-Regeln** mit `placeholder.*`-Fakten-Schlüsseln; Umbenennen später ist ein Save-Kompatibilitätsereignis (`Docs/Decisions/0004`). Grundlage: `Docs/Proposals/0004` (Interaktion/Testtür), `0007` (Holder/Inventar), `Docs/NightBuild01/ARCHITECTURE.md`.

## Was gebaut wurde
| Teil | Pfad |
|---|---|
| Core: Verb `Force`; Tür-Zustände `Closed/Open/Locked/Blocked/Broken`, Schlüssel, Aufbrechen (`TestDoor`); Verb-Prüfung in `InteractionRegistry::Interact`; Kandidatenwahl `PickInteractionCandidate` | `Core/include/spn/core/{Interaction,TestDoor,InteractionReach}.h`, `Core/src/…` |
| Core-Tests (12 neue Fälle) | `Tests/Core/DoorStatesTests.cpp` (+ angepasst: `InteractionTests.cpp`) |
| Unreal-Wrapper der Kandidatenwahl | `SpnCore/Public/SpnInteractionPick.h`, `Private/SpnInteractionPick.cpp` |
| Welt-Zustand: `Force`, Fakten-Schlüssel (`broken`, `disabled`, `key…held`), Schalter-Typ registriert | `SpnCore/…/SpnWorldStateSubsystem.*` |
| Interaktionskomponente: Überlappung → Sichtlinie → Core-Wahl, zweiter Prompt, `TryForce` | `Spn/SpnInteractionComponent.*`, `SpnInteractable.h` |
| Tür: gesperrt/Schlüssel/blockiert/aufbrechbar, Texte, Rückmeldung im HUD | `Spn/SpnTestDoor.*`, `UI/SpnUiTextNightBuild.inc` |
| Neue Objekte: `ASpnTestKey`, `ASpnLightSwitch` | `Spn/SpnBuildingObjects.*` |
| Eingabe: E + DualSense Quadrat (Interagieren), F + DualSense Dreieck (Aufbrechen) | `SupernaturalProtCharacter.*` |
| Testgebäude-Level | `Tools/create_spn_building_test_map.py` → `Content/SpnTest/Lvl_SpnBuildingTest.umap` |
| Tests | `Spn/Tests/SpnNightBuild04Automation.cpp` (11), `SpnQaBuildingAutomation.cpp` (`SpnQA.HouseJourney`, gerendert) |
| Runner | `Tools/run_gameplay_qa.ps1 -Suite Building` |

Keine zweite Interaktions-/Tür-/Save-Implementierung: alles läuft über `InteractionRegistry`, `WorldFacts` und `USpnWorldStateSubsystem` (Save/Load: `SaveToSlot`/`LoadFromSlot`, gleiches WorldFacts-v1-Format, **keine Format-Änderung**).

## Tür-Zustandsmodell (Platzhalter)
Fakten: `placeholder.door.<id>.{open,locked,disabled,broken}`, Schlüssel `placeholder.key.<keyid>.held`. Authored: `open`, `locked`, `disabled` (= „blockiert"), `breakable`, `key`.

| Zustand | Bedeutung |
|---|---|
| Geschlossen / Offen | `open`; **Use** schaltet um (Schließen nie durch einen Pawn im Schwenkbereich) |
| Verriegelt | `locked` und geschlossen: **Use** wird abgelehnt — außer der passende Schlüssel ist im Besitz: dann **entriegelt** Use (Tür bleibt zu, nächstes Use öffnet) |
| Blockiert | `disabled`: Use und Force werden abgelehnt, nichts wird geschrieben |
| Aufgebrochen | **Force** bei geschlossener, verriegelter, `breakable` Tür: `broken=true`, `locked=false`, `open=true` — dauerhaft, gespeichert; Schloss kommt nicht wieder; Tür lässt sich weiter öffnen/schließen (hängt sichtbar schief) |
| Nicht aufbrechbar | Force auf `breakable=false`, offener oder unverriegelter Tür: abgelehnt, keine Änderung |

Jedes Verb, das eine Art nicht anbietet, wird von `InteractionRegistry::Interact` abgelehnt (`placeholder:feedback.verb.unsupported`) — ein Schalter hat kein Force.

## Interaktion
Überlappung (Reichweite 250 cm) → Sichtlinie zum nächsten Punkt des Objekts (Wände blockieren) → `spn::core::PickInteractionCandidate` (Reichweite, Blickwinkel 35°, Höhe, dann zentralster/nächster, Gleichstand per `SpnInteractionPriority`). Prompt im HUD: `E / Quadrat – Tür öffnen` (Pad-Name nur bei angeschlossenem Gamepad) und, falls Force möglich, zusätzlich `F / Dreieck – Tür aufbrechen`. Es gibt in dieser Spielfigur **keine Angriffs-Aktion**; Interagieren/Force haben eigene Tasten, ein Test hält die Tastensätze disjunkt zu Sprint/Sprung.

## Testgebäude `Lvl_SpnBuildingTest`
12 × 9 m, Foyer / Archiv / Arbeitszimmer / Lager, Vordertreppe (2 Stufen), 6 Türen, Schlüssel auf dem Archivtisch, 3 Lichtschalter, Notiz (Pickup) im Arbeitszimmer, Innen- und Außenlampen (Dämmerung), Engstellen (Regal, Bank), Kisten (40–80 cm). Zugang: Vordertür offen; Arbeitszimmer per Schlüssel **oder** über Lager (Hintertür/Lagertür aufbrechen → Innentür). Reines Test-/Platzhalter-Gebäude (Engine-Würfel + Projekt-Platzhaltermaterialien).

## Tests und Validierung
Siehe `QA_RESULTS.md` und `KNOWN_ISSUES.md`.
