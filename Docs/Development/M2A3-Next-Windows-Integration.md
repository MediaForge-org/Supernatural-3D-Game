# M2-A.3: nächste Windows-Integration (aus der Fedora-Sitzung 2026-10-28/29, Startcommit `b69eb90`)

Alles unten wurde auf Fedora geschrieben. **Nichts ist gegen Unreal kompiliert oder gespielt.** Kein Commit, kein Push; die M2A2-Änderungen der Vornacht lagen
unverändert im Arbeitsbaum.

## 1. Was gebaut wurde (Core, auf Fedora getestet)
| System | Dateien | Kern |
|---|---|---|
| Ereignisbus | `Core/.../GameEvents.{h,cpp}` | geordnete Listener, Filter, Unsubscribe/Subscribe im Dispatch, Warteschlange statt Rekursion (begrenzt, Überlauf gezählt), `ScopedSubscription` |
| Interaktion V2 | `InteractionSystem.{h,cpp}` | Zielauswahl (Priorität, Winkel, Distanz, Id; Raster-Vorauswahl), `InteractStatus` (Success/OutOfRange/InvalidTarget/Disabled/Locked/Blocked/AlreadyInState/InvalidWorldState/InvalidInput), Cooldowns, Wiedereintrittsschutz, Block-Regeln je Kind, Links (Schalter -> Türschloss) mit Zyklen-/Duplikat-/Fremdschlüsselprüfung |
| Geräte | `Devices.{h,cpp}` | Schalter, Taster (Zähler, `max_presses`), Hebel (2-16 Positionen); disabled; Zustand nur in `WorldFacts` |
| Tür V2 | `TestDoor.{h,cpp}`, `InteractionSystem` | `disabled`, `SetTestDoorLocked/Disabled`, `MakeDoorCloseObstructionRule` (Schließen blockiert, Öffnen nie) |
| Persistenz | `ObjectPersistence.{h,cpp}` | nur geänderter Zustand je Objekt, Katalog (Konfiguration getrennt vom Laufzeitzustand), Format `SPN-OBJECTS 1`, Dirty-Tracking, Regionen laden/entladen, Waisen, atomares Schreiben mit `.bak` |
| Traversal | `Traversal.{h,cpp}` | `MoveToStart`: nichts startet ohne passende Taste (Sprung = Sprungtaste, Vault/Mantle/Climb = künftige Traversal-Taste) |
| Umgebung | `Tools/spn_env_enrichment.py` | opt-in Anreicherung, eigenes Level `Lvl_SpnMovementTestV2Env`; Fixture `Tests/Core/Data/env_scenarios.txt` wird von `EnvScenarioTests.cpp` gegen den Core ausgeführt |

## 2. Unreal-Änderungen (NICHT kompiliert, zuerst bauen)
- `Spn/SpnTestDoor.{h,cpp}`: `IsLocked()` (Prompt aus dem Weltzustand statt aus `bAuthoredLocked`), `IsClosingObstructed()` (Pawn-Overlap entlang des Schließwegs, alle 10°, `OverlapAnyTestByObjectType(ECC_Pawn)`), `SpnUse` verweigert das Schließen mit `placeholder:feedback.door.blocked`, ohne Zustandsänderung.
  Risiken beim Bauen: `UStaticMesh::GetBounds()`, `FCollisionObjectQueryParams(ECC_Pawn)`, `UWorld::OverlapAnyTestByObjectType`, `SCENE_QUERY_STAT`.
  **Runtime-Prüfung nötig:** Tür schließt nicht auf einem im Türrahmen stehenden Spieler; Öffnen bleibt möglich; keine Fehlauslösung durch Wände (nur Pawn-Objekttyp wird geprüft).
- Aus der Vornacht (M2A2, weiter ungebaut): `SpnMovementDiagnostics`, `SpnQaRecorder.h` (`Trace`), `SpnQaMovementAutomation.cpp`.
- Es gibt **keine** Unreal-Klassen für Schalter/Taster/Hebel, keine Anbindung von `InteractionSystem`/`ObjectPersistence` an `USpnInteractionComponent`/`USpnWorldStateSubsystem`.

## 3. Reihenfolge auf Windows
1. `powershell -File Tools\windows\run_m2a3_integration.ps1 -DryRun` (Befehle prüfen), dann ohne `-DryRun` (Parameter `-SkipBuild`, `-SkipMap`, `-SkipQa`, `-Suite`, `-Yes`, `-EnginePath`). Er führt aus: Python-Tests, Core-Tests, Editor-Build, Backup + Neuerzeugung **nur** der Datei `Lvl_SpnMovementTestV2Env`, Generator-Verdikt, `run_gameplay_qa.ps1`, Hash-Vergleich V1/V2, Trace-Analyse; schreibt `Saved\M2A3-Integration\<Zeit>\summary.json` mit Fehlerklassen PREREQ / TEST / BUILD / GENERATE / RUNTIME / INTEGRITY.
2. Erwartungen: Python **219** Tests OK, Core **354** Tests, Build ohne Fehler (sonst die Risiken in Abschnitt 2), Generatorlog 1800 Primitive.
3. Visuelle und spielerische Prüfung: `M2A3-Environment-Visual-Checklist.md`.
4. QA-Exit-Codes unverändert: 0 vollständig und grün, 1 vollständig mit Fehlern, 2 unvollständig.

## 4. Nicht erledigt / Backlog
- Unreal-Adapter für Schalter/Taster/Hebel (`ASpnTestSwitch` o. ä.), Anbindung des Interaktionssystems V2 an den Komponentenpfad, Anbindung der Objekt-Persistenz an den Weltzustand-Subsystem-Speicherpfad und Regionen-Streaming.
- Unreal-Automationstests für Türhindernis, Geräte, Szenarien (`env_scenarios` haben Status `PENDING_UNREAL_ADAPTER`).
- Offene Bewegungsthemen der M2A2 (Sprint-Asymmetrie, Kantenverhalten) unverändert.
- Inventar, interaktive Weltkarte: nicht begonnen.
