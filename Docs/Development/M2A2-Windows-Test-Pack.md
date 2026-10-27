# Windows-Testpaket (Phase 3) - alles UNGETESTET gegen eine Engine

Reihenfolge, Erwartungen und Prüfpunkte. Skript: `Tools/windows/run_spn_checks.ps1` (nie ausgeführt). Log-Auswertung: `Tools/spn_log_tools.py` (an synthetischen Logs getestet).

## L1 Build
1. Windows-Stand (NetworkSprint) zuerst committen; Fedora-Änderungen als Patch/Branch holen; Konflikte aus `M2A2-Fedora-To-Windows-Handover.md` §16.
2. Core: `cmake --preset clang` / build / `ctest` -> 276 Tests erwartet. (MSVC: Core-Dateien werden von UBT über `SpnCore.Build.cs` aus `Core/src` kompiliert - neue .cpp (CameraArm, GroundSteering, InteractionReach, JumpModel, Locomotion, Npc, Traversal) werden automatisch aufgenommen.)
3. Editor-Target `SupernaturalProtEditor`. Neue Unreal-Datei: `Spn/SpnMovementDiagnostics.{h,cpp}` (Modul `SupernaturalProt`, nutzt nur Core/CoreUObject/Engine, keine neuen Module).
   Include-/API-Risiken (hier zuerst nachsehen): `UCharacterMovementComponent::CurrentFloor` (public), `FFindFloorResult::IsWalkableFloor()`, `TEnumAsByte::GetValue()`, `TAutoConsoleVariable<int32>::GetValueOnGameThread()`, `UE_LOG(LogSupernaturalProt, ...)` (Header `SupernaturalProt.h`).
   MSVC-Risiken im Core: `std::clamp`/`std::copysign` (`<algorithm>`/`<cmath>`), `friend operator== = default` in `Vec.h`, `[[fallthrough]]` in `Locomotion.cpp`; Warnungen als Fehler je nach UBT-Einstellung.

## L2 Kartengenerierung
- v2-Pfad `/Game/SpnTest/Lvl_SpnMovementTestV2`. Aufruf: `-script="<abs>/create_spn_movement_test_map.py --v2"`; Zusätze `--instancing`, `--force`, `--adopt-legacy`.
- Benötigte Assets: Engine-Shapes (Cube/Cylinder/Sphere), `/Game/LevelPrototyping/Materials/MI_PrototypeGrid_*` und `MI_DefaultColorway` (fehlendes Material nur Warnung), Klassen `/Script/SupernaturalProt.SpnTestDoor` und `.SpnPlayAreaGuard`.
- Ablauf im Skript: Layout-Preflight (Daten, Shapes, Materialien, Klassen) -> Plan (create/keep/abort/rebuild) -> Generierung -> Verifikation der Actor-Zahlen -> erst dann Speichern. Fehler = Level NICHT gespeichert, Editor ohne Speichern schließen.
- Erstlauf OHNE `--instancing` (Fallback-Pfad ist 1:1 Einzelactors), danach mit `--instancing`: Erwartung im Log "N instance groups"; Gruppen die nicht bauen -> Warnung "instancing of ... failed" und Einzelactors (kein Duplikat).
- Wiederholung: zweiter Lauf ohne `--force` ändert nichts ("keep"); mit `--force` ersetzt nur getaggte Actors. Die alte v1-Karte ist "legacy" (keine Tags): `--force` bricht ohne `--adopt-legacy` ab.
- Auswertung: `python Tools\spn_log_tools.py generator <log>` (Exit 0 = gespeichert/unverändert).
- UNBESTÄTIGTE APIs (bei Fehler zuerst prüfen): `actor.tags` setzbar, `actor.add_component_by_class(...)`, `component.add_instance(transform, True)`, `component.get_instance_count()`, `actor.destroy_actor()`, `unreal.PointLightComponent` Properties `intensity`/`attenuation_radius`. Ein bare `unreal.Actor` hat evtl. keine Root-Komponente: dann Instanzposition kontrollieren.

## L3 Gameplay-Tests (je Punkt: bestanden / Messwert / Abweichung notieren)
| Bereich | Test | Erwartung |
|---|---|---|
| Spawn | PlayerStart (0,0,100), Reset-Punkte inkl. v2 (`d_*`) | Spawn frei, Guard setzt zurück |
| Bewegung | Walk/Jog/Run/Sprint auf der Speed-Lane (v1) | 1,8 / 3,5 / 5,5 / 8,0 m/s (+-5 %) |
| Sprint-Wenden | Ringstraße: 45/90/135/180° | Geschwindigkeit pro Frame protokollieren; Vergleich mit Core-Modell (Mindest 45° > 6,5, 90° > 5,5 m/s) - das Modell ist NICHT verdrahtet, Abweichung erwartet |
| Sprung | `spn.MoveDiag 1`; Wände 30/40/45 cm (v1, dünn und dick); Absprungabstand 0,3/0,7/1,1/2,0 m | `spn_log_tools diag`: Vorhersage `BlockedRising` unter dem Mindestabstand, Tempo-Einbruch (Ereignis 8), vertikale Phase; Auto-Stufe um 30 cm: Ereignis 4 + FloorDist |
| Hindernisse | Kerb 12 cm, Gartenmauern 30/45/100 | Kerb ohne Sprung; Klassen wie Core |
| Treppen | Haus 1 / Courthouse: Fuß erreichbar? hoch/runter, Podest, Kopf im Treppenloch? | Fuß im Süden frei, oben 2-m-Podest, Geländer an Seite und Fußende |
| Türen | 8 neue + 2 v1 Aktoren; Schwenk nach innen/außen | Schwenkrichtung (unbestätigt) frei von Wänden; Interaktion `E`; Zustand speichern/laden |
| Innenräume | Büro: Flur, 2-4 Räume je Seite; alle Türen | Kapsel (r 42) passt überall; keine unsichtbaren Wände in Öffnungen |
| Obergeschoss | Treppe -> Podest -> Räume | alle Räume erreichbar |
| Kamera | Matrix in Handover §10 plus Büro-Flur und Treppenhaus | kein Durchsehen, kein Pumpen an Türen |
| Interaktion | Reichweite 2,5 m / 60° (Core `InteractionReach` noch nicht angebunden) | bestehendes M1-Verhalten unverändert |
| Karte | Instancing an/aus: Kollision der Instanzen (Capsule, Kamera), Sichtbarkeit, Schatten | gleiche Ergebnisse |
| Performance | FPS, Draw Calls, Ladezeit, mit/ohne Instancing | erstmals messen (nichts davon ist bekannt) |

## L4 Netzwerk
Die Windows-NetworkSprint-Korrektur (`bSprintRequested`/`BaseGait` über `ClientUpdatePositionAfterServerUpdate()`) bleibt maßgeblich; Fedora liefert KEINE zweite Implementierung. Nach dem Merge: NetworkSprint 20x Wiederholung mit den neuen Gangartwerten; Patchvorschläge (Lenkmodell, Ledge-Assist) erst danach und nur hinter Flags.

## L5 Automatisierung
`run_spn_checks.ps1` (Build, Generator, Verdict, Automation-Aufruf), `spn_log_tools.py generator|diag`. Der Automation-Filter `SPN` ist eine Annahme - an die tatsächlichen Testnamen anpassen.
