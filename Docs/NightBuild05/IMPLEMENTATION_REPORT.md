# Implementation report – NightBuild 05

## 1. Bestand (Phase 1 der Aufgabe)

| System | Befund | Entscheidung |
|---|---|---|
| `Core/Npc.h` | Zustandsmaschine (Idle/Moving/Interacting/Alert/Incapacitated), `Perceive` (Sicht/Gehör, **mit** „Nähe sieht alles“, auch durch Wände) | Nicht erweitert; `NpcReaction.h` ergänzt eigene Urteile (`CanSee` ohne Röntgenblick, `CanHear` mit Wanddämpfung). `NpcPerceptionConfig` wird wiederverwendet. |
| `Core/NpcMemory.h` (`NpcMemoryStore`, `world.npcs`) | vollständig, getestet, **aber in Unreal nirgends angebunden** (kein Besitzer, kein Eintrag im Save-Subsystem) | Unreal-Besitzer `USpnNpcMemorySubsystem` + Save-Anbindung ergänzt; Format `SPN-NPCMEM 1` unverändert. |
| `Core/GameEvents.h` (`GameEventBus`) | nur Interaktions-/Türereignisse, in Unreal nicht benutzt | `NoiseEmitted` + Felder ergänzt; Unreal-Kanal `USpnNoiseSubsystem` besitzt einen Bus. |
| `InteractionSystem` / `TestDoor` (NB04) | Türzustände, Force | unverändert genutzt; Tür erweitert (Pawn-Schutz, Haltedauer, Geräusch). |
| `WorldFacts`, `SpnSaveSubsystem` | `world.facts`, Inventar, Karte, Pose | `world.npcs` ergänzt, tolerantes Laden (siehe unten). |
| NPC/AI in Unreal | **keine** (kein NPC-Actor, kein AI-Controller-Einsatz) | `ASpnNpc` neu (Platzhalter). |
| Navigation | `NavigationSystem`-Modul war nicht eingebunden, kein NavMesh in den Testlevels | Modul eingebunden, Laufzeit-NavMesh (`RuntimeGeneration=Dynamic`) + Volumen im NPC-Level. |
| Level | `Lvl_SpnBuildingTest` (NB04) | neues `Lvl_SpnNpcTest` abgeleitet, siehe `Docs/Development/LEVEL_REGISTRY.md`. |

## 2. Zusammenspiel (Ablauf je Frame)

1. Ein Emitter (Tür) ruft `USpnNoiseSubsystem::EmitNoise` → `GameEventBus::Publish(NoiseEmitted)` → Listener → `FSpnNoiseEmitted` → jeder `ASpnNpc::HandleNoise`.
2. `HandleNoise` zählt die Wände zwischen Quelle und Ohr (iterativer Linientrace), ruft `CanHear`. Gehört + verdächtig → `Remember` (Gedächtnis) → Wachsamkeit neu; gehört + harmlos → nur gezählt.
3. `ASpnNpc::Tick`: Sicht (alle 0,1 s, Linientrace Kopf/Brust/Füße) → `NpcReactionBrain::Update(dt, …)` → Ausgabe (Ziel, Gangart, Alarm) → Bewegung (`MoveToLocation`/Fallback), Alarm-Ruf = `Shout`-Geräusch, Erinnerung `Alarmed`.
4. Speichern: `SyncClockForSave` bringt die Uhr des Speichers auf die Spielzeit, `world.npcs` wird (sobald es NPCs gibt oder der Slot den Abschnitt schon hat) geschrieben.
5. Laden: Abschnitt wird geparst (zweiphasig wie alle Domänen); danach `OnReplaced` → jeder NPC registriert sich neu und leitet seinen flüchtigen Startzustand aus dem Gedächtnis ab.

## 3. Save/Load im Detail

| Fall | Verhalten | Test |
|---|---|---|
| Slot mit `world.npcs` | wiederhergestellt, Uhr = gespeicherte Spielzeit, Status `bNpcMemoryRestored` | `SpnCore.NpcMemory.SaveLoadRoundTrip`, `SpnQA.NpcReaction` |
| Slot **ohne** `world.npcs` (alter Slot / noch kein NPC) | lädt; NPC-Gedächtnis wird **geleert und gemeldet** (`bNpcMemoryResetToEmpty`), Türfakten bleiben | `…SlotWithoutNpcSection` |
| `world.npcs` beschädigt (Prüfsumme ok, Inhalt abgelehnt; oder abgeschnitten) | **Rest des Slots lädt**, NPC-Gedächtnis leer, `bNpcMemoryDiscarded` + Grund, Warnung im Log; `InspectSlot` nennt den Slot weiter ladbar. Ein Container-Prüfsummenfehler bleibt ein Fehler des ganzen Slots (unverändert). | `…DamagedNpcSection` |
| `world.npcs` mit neuerer Version | Laden und Speichern werden **verweigert** (`UnsupportedVersion`), nichts wird überschrieben | `…DamagedNpcSection` |
| Mehrfaches Speichern/Laden | 12 Zyklen, nichts verloren/doppelt, Zustand identisch | `…RepeatedSaveLoadCycles` |
| Speichern **während Alert/Flee** | gespeichert werden Erinnerungen (kein Zustandsname im Text), nach dem Laden `Investigate` zum letzten Vorfallsort (Alarm < 120 s) | `SaveDuringAlarmAndLoad`, `SpnQA.NpcReaction` |
| Wiederholte Ereignisse | begrenzt je Tier (A/B/C: 64/16/4) | `…BoundedRepeatedEvents` |

Warum nicht den ganzen Slot ablehnen, wenn nur NPC-Daten schlecht sind? Das NPC-Gedächtnis ist eine **eigene, optionale Domäne**; ein Rollback auf die vorherige Generation würde Spielfortschritt verlieren, der nicht beschädigt ist. Der Verlust ist begrenzt, sichtbar (Status, Log) und wird beim nächsten Speichern durch den lebenden Zustand ersetzt.

## 4. Vorläufige Regeln (keine Kanon-Entscheidung)

Alle folgenden Werte sind **Platzhalter-Gameplay-Parameter**, sitzen in `NpcReactionConfig`, `HearingConfig`, `WarinessConfig`, `NpcMemoryConfig` (Core) bzw. als `UPROPERTY` an `ASpnNpc`/`ASpnTestDoor` und wurden **nicht** als Spielkanon beschlossen:

| Bereich | Wert |
|---|---|
| Hörweite Türbruch / Tür benutzen / NPC-Ruf | 20 m (Stärke 1,0) / 6 m (0,35) / 25 m (0,9) |
| Wanddämpfung | Reichweite ×0,5 je Wand |
| „Laut genug, um verdächtig zu sein“ | Stärke ≥ 0,75 (auch bei harmloser Art) |
| Sicht | 18 m, Sichtkegel 120°, kein Röntgenblick, Ziel: Kopf/Brust/Füße |
| Verweildauer / Suchen / Untersuchungs-Timeout | 0,75 s / 4 s / 25 s |
| Alert halten / Flee-Auslöser (Distanz, Zeit) | 6 s / 6 m, 1,5 s (wachsam: 0,5 s) |
| Flee Mindestdauer / sicherer Abstand / Rückweg-Timeout | 10 s / 15 m / 40 s |
| Geräusch-Zusammenfassung / Reaktions-Cooldown | 3 m / 1 s |
| Wachsam ab Verdacht | 0,3 (Summe Gewichte × verblasste Stärke: Türbruch 1,0; Geräusch 0,7; Alarm 1,0; Spieler gesehen 0,15) |
| Alarm gilt „läuft noch“ | 120 Spielsekunden (Wiederaufnahme nach dem Laden) |
| Gedächtnis | 64/16/4 Einträge (A/B/C), Halbwertszeit 10 Tage/2 Tage/6 h Spielzeit |
| Aufbrechdauer | 1,6 s Halten (`BreakSeconds`) |
| NPC-Gehen / Flucht | 1,5 m/s / 4,3 m/s |
| Patrouille | Wartezeit 3 s je Punkt |

## 5. Bekannte Abweichung von der Aufgabe

* **„Sichtkontakt … Wahrnehmung durch massive Hindernisse“**: Sicht ist strikt blockiert; Hören ist **gedämpft**, nicht blockiert (Wände halbieren die Reichweite). Das ist die gewählte Modellierung („sofern das jeweilige Wahrnehmungsmodell dies erfordert“) und im README begründet.
* **Unreal AI Perception** wurde geprüft und nicht benutzt (Begründung im README).
