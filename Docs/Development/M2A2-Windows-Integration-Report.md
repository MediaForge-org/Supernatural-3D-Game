# M2-A.2 Windows / Unreal Integration: Ergebnisbericht (2026-10-27)

Engine: Unreal Engine 5.8.4 (`C:\Program Files\Unreal Engine\UE_5.8`, CL 58902052), MSVC 14.44, RTX 3090. Alle Zahlen stammen aus echten Läufen
dieser Sitzung; Rohlogs liegen unter `Unreal/SupernaturalProt/Saved/M2A2-Windows/` (nicht im Git). Die 300 x 300 m große Karte ist ein
PLATZHALTER-Entwicklungslevel und ersetzt nicht die geplante Spielwelt. Master-Konsultation (CLAUDE.md §11): `Planning/Master` war nicht
eingehängt; es wurden keine Gameplay-Systementscheidungen getroffen, nur Test-/Generatorfehler behoben und Messungen ergänzt.

## 1. Build
`SupernaturalProtEditor Win64 Development`: **erfolgreich**. Einziger Fehler: `Core/src/JumpModel.cpp` benutzte `std::max` ohne `<algorithm>`
(MSVC C2039); behoben. `SpnMovementDiagnostics` und die übrigen Fedora-Dateien kompilierten ohne Änderung.

## 2. Karte V2 (`/Game/SpnTest/Lvl_SpnMovementTestV2`)
Mit dem vorhandenen Generator erzeugt (`create_spn_movement_test_map.py --v2 --instancing --force`), Layout-Preflight, Ownership-Tag
(`SPN_GEN`) und Verifikation vor dem Speichern funktionieren in Unreal. V1 blieb unangetastet. Ergebnis: 1102 Primitive, 10 Türen, 52 Lichter,
Guard, PlayerStart, 9 Distriktgebäude; mit Instancing 553 Actors (75 Instanzgruppen, 709 Instanzen), ohne 1187.
Die Karte öffnet im Editor mit 0 Map-Check-Fehlern.

## 3. Gefundene und behobene Fehler
| # | Fehler | Beleg | Korrektur |
|---|---|---|---|
| 1 | `std::max` ohne Header | Build | `#include <algorithm>` |
| 2 | Instancing: `add_component_by_class` existiert in der Python-API nicht; der Fallback scheiterte zusätzlich (`actor.destroy_actor()` gibt es nicht) | Generatorlog | `ASpnInstancedGroup` (C++-Actor mit ISM-Root), Löschen über `EditorActorSubsystem` |
| 3 | Instanzen im Spielbetrieb mit Default-Material (Materialflag "Used with Instanced Static Meshes" fehlt) | Log `missing usage flag InstancedStaticMeshes` | Generator setzt das Flag auf `M_PrototypeGrid` und `M_FlatCol` und speichert sie (LFS-Assets geändert!) |
| 4 | **Treppe beider zweigeschossiger Häuser blockiert**: Der geöffnete Türflügel (West-Tür, `OpenAngle +90`) steht quer in der 140 cm breiten Halle (Restspalt 40 cm < Kapsel 84 cm). Das Python-Modell prüfte nur "Schwenk gegen Wände", nicht den offenen Flügel als Hindernis | PIE-Walkthrough: Charakter steckt 160-180 cm vor dem Treppenfuß | Distrikttüren schwingen nach außen (`door_open_angle`: W -90, O +90); Test ergänzt |

Nach der Korrektur erreicht der Walkthrough in PIE: alle 9 Eingänge, den Büroflur, beide Obergeschosse (Z 392 / 412) und den Rückweg.

## 4. Unreal-Automationstests
Stand vor Änderungen (35 Tests SpnGame+SpnCore): 32 bestanden, 3 fehlgeschlagen. **Endstand (40 Tests, instanzierte V2-Karte): 37 bestanden, 3 fehlgeschlagen.** Python-Tools: 166/166. Neu: `SpnGame.MapV2.Spawn`, `SpnGame.MapV2.Walkthrough`
(beide bestehen) sowie Mess-/Hilfstests unter `SpnGame.MapV2Capture.*` (Screenshots, JumpApproachReport, StepLimitReport, PerformanceReport;
sie prüfen nichts, sie messen).

Weiterhin fehlgeschlagen (Anforderungen nicht abgeschwächt):
- **Obstacles**: 30 cm gehend wird überquert (erwartet: blockiert, Stufenlimit 25); mit Sprüngen werden 50 und 60 cm überquert (erwartet: > 45 cm blockiert).
- **StairsAndRamps**: Treppe mit 30 cm Stufenhöhe wird erklommen (erwartet: blockiert).
- **SprintTurn**: 90° rechts: Sprint bleibt, aber Tempo fällt auf 461 von 800 cm/s (erwartet >= 90 %); 90° links: Sprint endet (asymmetrisch zu rechts) und Tempo fällt auf 463.

### Ursachen (gemessen)
- *Gehen über 30 cm*: Der Anstieg passiert im Walking-Modus als Folge vieler kleiner Schritte (1-8 cm je Frame über ca. 30 cm Weg, Diagnose-Event 4), und er hängt
  **nicht** von `MaxStepHeight` ab: Mit Limit 10, 20 und 25 werden 20/25/30 cm überquert, 40 und 45 cm nicht (`StepLimitReport`). Es ist also nicht `StepUp`
  (dessen `DeltaZ > MaxStepHeight`-Prüfung ist im Engine-Quelltext vorhanden), sondern vermutlich das Gleiten der Kapselrundung (Radius 42 cm) über die Kante. Das
  ist ein Engine-/Kapselverhalten; eine Änderung wäre eine Gameplay-Entscheidung (Kapselform, eigener Slide/CanStepUp-Override).
- *Sprünge über 50/60 cm*: Sprung (Scheitel 45 cm) plus Anstieg über die Kante nach dem Scheitel; effektiv ca. 45 + 25 cm.
- *Sprint-Wende*: das CMC bremst/beschleunigt beim 90°-Wechsel über die Reibung; die Erwartung (>= 90 % Tempo) setzt das **nicht verdrahtete Lenkmodell**
  (`M2A2-Patch-Proposals.md`) voraus. Die Links/Rechts-Asymmetrie beim Gait ist ungeklärt (nicht weiter untersucht).

## 5. Sprung "fast senkrecht" (Frame-Diagnostik, `spn.MoveDiag`, `JumpApproachReport`)
Beim Absprung 125 cm vor einer Kante gibt es **keine** vertikale Phase: das Tempo bleibt über den ganzen Sprung bei 300-350 cm/s (Jog) bzw. 550 (Run), Anstieg 45 cm.
Das senkrechte Hochsteigen tritt nur auf, wenn die Kante etwa so hoch ist wie der Sprungscheitel:
Jog, Kante 45 cm, Abstand 10 cm: Tempo bricht auf 38 cm/s ein, 31 cm Vorwärtsweg in 0,4 s, Überquerung erst nach 1,03 s; Run, Kante 40 cm, Abstand 10 cm: 44 cm/s, 0,97 s;
Run, Kante 45 cm, Abstand 10 cm: 0 cm/s, 1,20 s. Bei Kanten 30-40 cm mit >= 30 cm Abstand bricht das Tempo nie ein. Das stimmt mit dem Core-Modell
(`JumpModel`, "BlockedRising") überein. Die Sprungphysik wurde **nicht** verändert; die Optionen A/B/C stehen in `M2A2-Patch-Proposals.md` §3 und brauchen deine Entscheidung.

## 6. Bewegungsgeschwindigkeiten
`SpnGame.Movement.Gaits` besteht: 1,8 / 3,5 / 5,5 / 8,0 m/s werden im Spiel verwendet. Sprungscheitel 44,9 cm gemessen.

## 7. Performance (`-game`, 1600x900, RTX 3090, novsync, Instancing an / aus)
| Ansicht | FPS an | FPS aus | Draw Calls an / aus | GPU ms an / aus |
|---|---|---|---|---|
| Luftbild | 114,7 | 112,6 | 230 / 246 | 4,32 / 4,27 |
| Straße West | 124,2 | 121,4 | 134 / 152 | 4,31 / 4,21 |
| Treppenhaus | 124,9 | 122,1 | 120 / 104 | 5,03 / 4,95 |
| Spawn | 119,3 | 117,6 | 135 / 132 | 4,47 / 4,42 |
Actors 553 / 1187. Game Thread ca. 2,6-3,2 ms. Prozessspeicher ca. 3,2 GB (Editor-Build mit Content). Ein Ausreißer von ca. 35 ms (Spawn-Ansicht) in beiden Varianten.
Instancing halbiert die Actors, ändert die FPS auf dieser kleinen Karte aber nicht messbar; die Kollision ist identisch (Trace-Raster: 22 801 Boden- und 91 204 Wandstrahlen, 0 Abweichungen).
Nicht gemessen: Editor-Viewport-Performance (Editor wird in dieser Sitzung im Hintergrund auf 3 FPS gedrosselt), Ladezeit.

## 8. Screenshots (echte Unreal-Spielansicht)
`Unreal/SupernaturalProt/Saved/M2A2-Windows/shots/`: 01_aerial_whole_map, 02_street_district_west, 03_residential_house1_exterior, 04_house1_stair_hall, 05_house1_upper_landing,
06_movement_test_obstacles, 07_player_view_spawn, 08_office_corridor, 09_courthouse_exterior, 10_parking_and_vegetation. Neu erzeugen:
`UnrealEditor.exe <uproject> -game -ExecCmds="Automation RunTests SpnGame.MapV2Capture.Screenshots" -TestExit="Automation Test Queue Empty" -windowed -ResX=1600 -ResY=900`.

## 9. Offene Punkte / manuell zu prüfen
1. Entscheidung zu den drei fehlschlagenden Tests: Kapsel-/Stufenverhalten (Kapselform, Slide-Override), Sprunghilfe (Optionen A/B/C), Lenkmodell.
2. Visuelle Prüfung im Editor (Straßenkreuzungen, Fensterlücken, Vegetation, Lichter wirken teils überbelichtet, Auto-Exposure) und spielerischer Rundgang durch dich.
3. Nicht automatisiert geprüft: Kamera in Türen/Treppenhaus (nur Screenshots), Türinteraktion im Distrikt (nur Öffnen per `SpnUse`), Save/Load der V2-Türen.
4. Git: `.gitignore` enthält `*.umap` und `*.uasset`; `Lvl_SpnMovementTestV2.umap` (und `Lvl_SpnMovementTest.umap`) ist daher **ignoriert** und braucht `git add -f` (LFS-Regel in `.gitattributes` greift). Geänderte LFS-Assets: `Content/LevelPrototyping/Materials/M_FlatCol.uasset`, `M_PrototypeGrid.uasset`.
