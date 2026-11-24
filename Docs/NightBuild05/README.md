# NightBuild 05 – Living World Foundations: NPC-Wahrnehmung, Erinnerungen, Konsequenzen

ENGINEERING PROPOSAL / Arbeitsstand, **nicht** Master-Canon. Ausgangscommit `704e54d`. **WorldForge ist kein Teil dieses Meilensteins** (keine Schnittstelle, kein Import, keine Weltgenerierung). Keine Weltgröße, kein Fraktions- oder Nemesis-System.

## Master basis

Konsultationsprotokoll (CLAUDE.md §11): **nicht möglich** – `Planning/Master` ist auf diesem Windows-Rechner nicht eingerichtet (`Tools/master-search.sh` meldet „planning master not available“, wie schon in Night Build 04). Es wurde **nichts** aus dem Master übernommen. Alle Regeln unten (Hörweiten, Zeiten, Wachsamkeit, Alarm, Aufbrechdauer, Gedächtnisgewichte) sind **vorläufige Gameplay-Parameter** (Liste in `IMPLEMENTATION_REPORT.md`, Abschnitt „Vorläufige Regeln“). Projektinterne Grundlage: `Docs/Proposals/0010` (Living World L1/L2, `NpcMemoryStore`), `Docs/NightBuild01/ARCHITECTURE.md`, `Docs/NightBuild04/README.md`.

## Was gebaut wurde (ein Kreislauf)

```
Tür aufbrechen (F halten) ──► Geräusch-Ereignis (Ort, Reichweite, Stärke, Art)
        │                               │
        │                     NPC hört es? (Reichweite, Wände dämpfen)
        │                               │ verdächtig → Erinnerung (NpcMemoryStore)
        ▼                               ▼
 Tür bleibt aufgebrochen        Investigate ─► sieht Spieler (Sichtlinie) ─► Alert + Alarm-Ruf ─► Flee
 (WorldFacts, gespeichert)              │                                              │
                                        └──── ReturnToRoutine ─► Idle ◄────────────────┘
                              Speichern: Türfakten + NPC-Erinnerungen   (Reaktionszustand flüchtig, nicht gespeichert)
                              Laden:     Erinnerungen zurück → NPC ist wachsam → untersucht auch harmlose Geräusche
```

| Teil | Pfad |
|---|---|
| Core: Geräusche, Hör-/Sichturteil, Reaktions-Zustandsmaschine `NpcReactionBrain`, Anbindung an `NpcMemoryStore` (Vorfälle, Wachsamkeit, Objekt-IDs) | `Core/include/spn/core/NpcReaction.h`, `Core/src/NpcReaction.cpp` |
| Core: neues Ereignis `GameEventType::NoiseEmitted` (+ Felder) im vorhandenen `GameEventBus` | `Core/include/spn/core/GameEvents.h`, `Core/src/GameEvents.cpp` |
| Core-Tests (19 neue Fälle) | `Tests/Core/NpcReactionTests.cpp` |
| Unreal-Wrapper (cm): Sinne, `FSpnReactionBrain`, Geräuschkanal `USpnNoiseSubsystem` | `SpnCore/Public/SpnNpcReaction.h`, `Private/SpnNpcReaction.cpp` |
| Unreal: persistenter Speicher `USpnNpcMemorySubsystem` (ein `NpcMemoryStore` je GameInstance) | `SpnCore/Public/SpnNpcMemorySubsystem.h`, `Private/SpnNpcMemory*.{h,cpp}` |
| Save-Anbindung: Abschnitt `world.npcs` (bestehendes Format `SPN-NPCMEM 1`), tolerantes Laden | `SpnCore/Private/SpnSaveSubsystem.cpp`, `Public/SpnSaveSubsystem.h` |
| NPC-Actor `ASpnNpc` (Platzhalterkörper, Routine, Sinne, Bewegung über NavMesh) | `SupernaturalProt/Spn/SpnNpc.{h,cpp}` |
| Tür: schiebt keine Pawns mehr, Aufbrechen braucht Zeit (F halten), Geräusch bei Erfolg | `Spn/SpnTestDoor.*`, `Spn/SpnInteractionComponent.*`, `Spn/SpnInteractable.h`, `SupernaturalProtCharacter.*` |
| Level `Lvl_SpnNpcTest` | `Tools/create_spn_building_test_map.py` (`SPN_VARIANT=npc`), `Docs/Development/LEVEL_REGISTRY.md` |
| Tests | `SpnCore/Private/Tests/SpnNpcMemoryAutomation.cpp` (8), `Spn/Tests/SpnNightBuild05Automation.cpp` (10), `Spn/Tests/SpnQaNpcAutomation.cpp` (`SpnQA.NpcReaction`, gerendert) |
| Runner | `Tools/run_gameplay_qa.ps1 -Suite Npc` |

Keine zweite Implementierung: Speicher = vorhandener `NpcMemoryStore`; Ereignisse = vorhandener `GameEventBus`; Speichern = vorhandenes `world.npcs`-Format und `SaveSlot`-Container; Türzustände = vorhandene WorldFacts; Interaktion = vorhandene `USpnInteractionComponent`.

## Unreal AI Perception: geprüft, bewusst nicht eingesetzt

Geprüft wurde `UAIPerceptionComponent` (Sight/Hearing). **Nicht verwendet**, aus diesen Gründen:

1. Das Hörmodell soll Wände **dämpfen** (Reichweite halbiert sich je Wand). `UAISense_Hearing` kennt nur „Reichweite“ oder optional „Sichtlinie ja/nein“ (`bUseLoSHearing`), keine abgestufte Dämpfung.
2. Das Wahrnehmungsurteil ist so **reine C++20-Core-Logik** (`CanHear`, `CanSee`) und mit Core-Tests ohne Unreal prüfbar; die Engine liefert nur Messwerte (Wandzahl, Sichtlinie per Linientrace).
3. AI Perception verwaltet eigene Stimuli-Zeitstempel/Alterung; die Erinnerung soll aber **einzig** im `NpcMemoryStore` liegen (gespeichert, begrenzt, mit Spielzeit).
4. Geräusche laufen über den vorhandenen `GameEventBus` (`NoiseEmitted`), damit Audio, Gameplay und spätere Systeme dieselbe Quelle nutzen.

**Genutzt** werden von Unreal: `AAIController::MoveToLocation` und das **NavMesh** (Laufen, Fluchtpunkte über `GetRandomReachablePointInRadius`), `LineTrace` (Sicht, Wandzählung). Ein späterer Wechsel zu AI Perception bliebe möglich, weil die Entscheidung im Core liegt.

## Verhalten im Detail

* **Sicht:** Reichweite, Sichtkegel, **klare Sichtlinie** (Kopf/Brust/Füße des Ziels). Ohne klare Linie wird nichts gesehen, auch nicht aus nächster Nähe (kein Röntgenblick).
* **Gehör:** Geräusch = Ort, Reichweite, Stärke 0..1, Art (`Footstep`, `DoorUse`, `DoorBreak`, `Impact`, `Shout`). Wirksame Reichweite = Reichweite × Empfindlichkeit × 0,5^Wände. Wände werden einzeln gezählt (der Multi-Trace der Engine hört beim ersten Block auf, deshalb wird iterativ getraced).
* **Harmlos vs. verdächtig:** `DoorBreak`, `Impact`, `Shout` sind verdächtig, `Footstep`/`DoorUse` nicht (ab Stärke 0,75 auch sie). Harmlose Geräusche werden gehört, aber ignoriert und **nicht** gemerkt – es sei denn, der NPC ist wachsam.
* **Zustände:** Idle → Investigate → Alert → Flee → ReturnToRoutine → Idle. Prioritäten: Flee > Alert > Investigate > Return > Idle; Eskalation sofort, Beruhigung erst nach Mindestverweildauer; jede Phase hat eine Zeitbegrenzung; ohne neues Ereignis landet jeder Zustand in begrenzter Zeit in Idle (Core-Test). Gleiche Geräusche innerhalb von 1 s und 3 m werden zusammengefasst.
* **Gedächtnis:** Gehörter Türbruch → `Witnessed` (Täter **unbekannt**, Subjekt = Tür-Objekt-ID), Spieler gesehen → `Encounter`, Alarm → `Threatened`. Wachsamkeit = Summe der (mit der Zeit verblassenden) Gewichte; ab 0,3 „wachsam“. Wachsame NPCs untersuchen auch harmlose Geräusche und eskalieren schneller (0,5 s statt 1,5 s bis Flee). **Kein** Nemesis: keine Rivalität, kein Wachstum, kein Fraktionszustand.
* **Flüchtig vs. dauerhaft:** gespeichert werden Identitäten, Erinnerungen, Uhr. **Nicht** gespeichert: Reaktionszustand, Timer, Ziele, Position. Nach Laden/Levelstart wird der Startzustand aus der Erinnerung abgeleitet: war ein Alarm in den letzten 120 Spielsekunden, beginnt der NPC mit **Investigate** zum letzten Vorfallsort, sonst **Idle**.
* **Speicher begrenzt:** je Tier A/B/C 64/16/4 Erinnerungen, Verfall mit Halbwertszeit (10 Tage/2 Tage/6 Stunden Spielzeit), bis 100 000 NPCs (Konfiguration des Cores, Platzhalter).
* **Stabile IDs:** `NpcId` je NPC (Level-Daten, 1 < id < 2^62, nie wiederverwenden); Spieler = Entität 1; Türen/Objekte = FNV-1a-Hash der Objekt-ID mit reserviertem Bit; „unbekannter Täter“ = reservierte ID.

## Türen (Verbesserungen)

* **Kein Wegschieben mehr:** Beim Öffnen und Schließen wird das Türblatt in 4°-Schritten gegen Pawns geprüft; steht ein Pawn im Weg, **wartet die Tür** (Meldung „Jemand steht im Weg“) und schwingt weiter, sobald der Weg frei ist. Das Schließen auf einen Pawn wird weiterhin abgelehnt.
* **Aufbrechen braucht Zeit:** `F` (Gamepad Dreieck) **gedrückt halten**, `BreakSeconds` = 1,6 s (Platzhalter). Fortschritt im HUD (`Tür wird aufgebrochen… 62 %`), die Tür rüttelt. Loslassen, Wegschauen oder Wegbewegen **bricht ab**, ohne Zustandsänderung und ohne Geräusch. Ziele ohne Dauer (z. B. nicht aufbrechbare Türen) antworten sofort wie zuvor.
* **Geräusch:** Erst bei Erfolg: `DoorBreak`, Reichweite 20 m, Stärke 1,0. Öffnen/Schließen: `DoorUse`, 6 m, Stärke 0,35 (harmlos). Echte Audiodateien gibt es nicht; das Gameplay-Geräusch funktioniert ohne.
* **Persistenz** unverändert über die WorldFacts (`placeholder.door.<id>.*`); der Format bleibt `WorldFacts v1`.

## Bekannte Korrektur zu Night Build 04

Der Bericht von Night Build 04 behauptete, `SpnQA.HouseJourney` habe das Level „neu geöffnet (frische Actors)“. Das war **falsch**: `AutomationOpenMap` tut nichts, wenn die Karte schon geladen ist (nur ein `LoadMap` im Log). Der Test hat den Zustand korrekt geleert und wieder geladen, aber dieselben Actors benutzt. `SpnQA.NpcReaction` lädt das Level jetzt wirklich neu (`open`) und prüft, dass die Actors neu sind. `HouseJourney` wurde dafür **nicht** geändert (siehe `KNOWN_ISSUES.md`).

Tests und Ergebnisse: `TEST_RESULTS.md`. Umsetzung im Detail und vorläufige Regeln: `IMPLEMENTATION_REPORT.md`. Offene Punkte: `KNOWN_ISSUES.md`.
