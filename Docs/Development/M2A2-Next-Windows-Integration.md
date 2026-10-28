# M2-A.2: nächste Windows-Integration (Übergabe aus der Fedora-Nacht 2026-10-28)

Startcommit der Nacht: `f59604a`. Alles hier ist auf Fedora geschrieben und **nicht gegen Unreal kompiliert oder gespielt**. Die Windows-Integration ist **nicht abgeschlossen**. `Lvl_SpnMovementTestV2.umap` im Repository ist der Stand vor diesen Änderungen; V1 (`Lvl_SpnMovementTest`) wird nie angefasst.

## 1. Auf Fedora umgesetzt

| Änderung | Dateien | Wirkung auf Unreal |
|---|---|---|
| V2-Generator: Eingangstür des V1-Hauses (`mv_house_door`) schwingt nach außen (`open_angle -90`, neues Feld in den Türdaten) | `Tools/spn_district_layout.py` (`fix_v1_house_door`, `door_open_angle`) | Karte V2 muss **neu erzeugt** werden; sonst keine Wirkung |
| Prüfmodell für Türdurchgänge (Flügel als Hindernis, Kapsel r 42, Engstellenlänge) | `Tools/spn_map_validate.py` (`door_passage`, `door_pinch`) | keine (Python) |
| Regressionstests Tür (alle 10 Türen: Spiel >= 5 cm, Engstelle <= 40 cm; Prüfmodell erkennt die beiden Altfehler) | `Tools/test_spn_district_layout.py` | keine |
| V2-Layout-Hash neu: `4df2a460da9fa6a1e66c12887979a0c4992c5d4b7ad4b05cded24f77e5fd6dab` (V1-Hash unverändert) | `Tools/test_spn_map_tools.py` | keine |
| QA-Datenpunkt `mv_house_door`: Innenpunkt (-520, -9200) statt (-450, -9200) | `Source/.../Tests/SpnQaDoorsAutomation.cpp` | **ein C++-Datenwert**, zu kompilieren |
| Core-Modell der Engine-Reibung erklärt die Sprint-Wende (461 gemessen, 465 Modell; 1,02 s beide) | `Tests/Core/GroundSteeringTests.cpp` | keine (Core-Test) |
| QA-Bericht: Vergleich mit dem Vorlauf, ungültige Status/Duplikate, fehlende Artefakte | `Tools/spn_qa_report.py`, `Tools/test_spn_qa_report.py` | `run_gameplay_qa.ps1` ruft das Skript unverändert auf; der Vergleich läuft automatisch |
| Stub-Check kennt `ClientUpdatePositionAfterServerUpdate` | `Tools/stubcheck/include/CoreMinimal.h` | keine |

**Nicht geändert:** Bewegungskomponente, Sprungphysik, Stufenhöhe, Sprint-/Kurvenparameter, Kapsel. Kein Vault/Mantle.

## 2. Windows-Schritte (in dieser Reihenfolge)

1. Fedora-Änderungen übernehmen (Patch/Branch). Konfliktgefahr: `SpnQaDoorsAutomation.cpp` (eine Zeile), sonst nur Python/Core/Docs.
2. Core-Tests: `cmake --preset clang` / `cmake --build --preset clang` / `ctest --preset clang`. Erwartung: **279 Tests, alle bestanden** (Fedora: gcc, clang, je mit ASan+UBSan).
3. Python-Tests (Python der Engine oder System-Python): im Ordner `Unreal\SupernaturalProt` `python -m unittest discover -s Tools -p "test_*.py"`. Erwartung: **183 Tests, OK**.
4. Editor-Target bauen: `SupernaturalProtEditor Win64 Development`. Es ist nur eine Datei mit Datenänderung (`SpnQaDoorsAutomation.cpp`) neu; Fehler hier wären neu zu untersuchen.
5. **V2-Karte neu erzeugen** (nur V2; ersetzt ausschließlich getaggte `SPN_GEN`-Actors):
   `UnrealEditor-Cmd.exe <uproject> -run=pythonscript "-script=<repo>\Unreal\SupernaturalProt\Tools\create_spn_movement_test_map.py --v2 --instancing --force" -unattended -nullrhi`
   Vorher `Lvl_SpnMovementTestV2.umap` sichern (Kopie). Log prüfen: `python Tools\spn_log_tools.py generator <log>` (Exit 0), Erwartung weiter 1102 Primitive, 10 Türen, 52 Lichter, 553 Actors mit Instancing. Bricht das Skript mit „legacy“ ab, **nicht** `--adopt-legacy` blind setzen: V1 darf nicht betroffen sein (der Pfad ist V2-spezifisch).
6. Kontrolle der Tür im Editor: Actor `SPN_MV_Door_mv_house_door` hat `Open Angle = -90`; alle anderen V1-Türen/Hütte bleiben +90; Distrikttüren wie bisher (W: -90, O: +90).
7. QA starten: `.\Tools\run_gameplay_qa.ps1 -Suite Doors` (dann `-Suite Quick`, dann `-Suite Full -Regression`).
8. Berichtsvergleich: Der Bericht enthält jetzt „Compared with the previous run“ (Vorlauf = neuester früherer Ordner unter `Saved/GameplayQA`). Prüfen: keine Regressionen, `Doors.TestHouse.walk_through` bei PASS.

## 3. Erwartete Artefakte
`Saved/GameplayQA/<Zeitstempel>/` mit `report.md`, `results.json` (neu: `record_issues`, `artifact_problems`, `comparison`), `records.jsonl`, `gameplay.mp4`, `clips/`, `shots/`, `automation/`. Der Bericht listet jetzt Probleme der Laufdaten (ungültiger Status, doppelte Checks, fehlendes Video/Clips/`records.jsonl`) und im Konsolenlauf `DATA PROBLEM:` / `REGRESSIONS since the previous run`.

## 4. Messplan für die offenen Bewegungsfehler (Engine nötig)
Nichts davon ist entschieden; erst messen, dann entscheiden.

* **Stufen/Kanten (30 cm gehend überquert, `MaxStepHeight` 25):** Mit `spn.MoveDiag 1` + `StepLimitReport` an 30-cm-Kante: zeigt Ereignis 4 (Step-up) wirklich `StepUp`, oder gleitet die Kapsel (Radius 42, runde Unterseite) über die Kante? Hypothese: Kante nur wenige cm über dem tiefsten Kapselpunkt ergibt einen flachen, „begehbaren“ Kontaktnormalenwinkel (Z >= 0,71). Gegenprobe: Kante mit scharfem Quader vs. abgeschrägtem Quader; `PerchRadiusThreshold`/`bUseFlatBaseForFloorChecks`/`WalkableFloorAngle` testweise ändern (**nur zum Messen**, nicht festschreiben). Auffällig war die Abhängigkeit Limit 10/20/25 -> überquert 20/25/30 cm: passt nicht zu einer festen Überhöhung; `StepLimitReport` mit feinerem Raster (je 2 cm) wiederholen.
* **Sprünge 50/60 cm und Spätsprünge:** folgen vermutlich aus derselben Kapsel-/Kantenmechanik („Sprung 45 cm + Einrasten“). Erst nach Klärung der Stufenfrage neu bewerten; Optionen A/B/C bleiben in `M2A2-Patch-Proposals.md` §3.
* **Sprint-Wende:** Erklärung steht (Reibungsformel). Entscheidung nötig: Lenkmodell (`GroundSteering`, Patchvorschlag §1, hinter Flag `bUseSpnSteering = false`) oder die Schwelle (>= 90 %) als Platzhalter anpassen. Offen bleibt die Links/Rechts-Asymmetrie (Sprint endet nur links): Frame-Trace (Geschwindigkeit, Gait, Winkel im Akteur-Frame, Akteur-Yaw) für 10 Wiederholungen je Seite aufnehmen.
* Das Testfeld „local input angle“ in `SpnQA.Turns` ist der Winkel im **Akteur-Frame** nach 30-100 ms (Kapsel hat schon bis 50° gedreht) und keine Abweichung vom 90°-Eingabewinkel; Beschriftung bei Gelegenheit anpassen.

## 5. Annahme-/Ablehnungskriterien

**Annehmen**, wenn: Build ohne neue Fehler; Core 279/279; Python 183/183; Karte V2 neu erzeugt mit gleichen Zahlen und ohne Map-Check-Fehler; `SpnQA.Doors`: **alle** Türen PASS inklusive `TestHouse.walk_through`, `TestHouse.close_from_inside`; keine neuen Regressionen im Vergleichsabschnitt; `SpnGame.MapV2.Walkthrough` weiter PASS; V1-Karte unverändert (Dateihash vorher/nachher).

**Ablehnen/zurückstellen**, wenn: `walk_through` weiter BLOCKED (dann `DescribeBlocker`-Detail und Position sichern; die Python-Prüfung gilt dann als unvollständig, Zusatzgeometrie wie Türrahmen/Schwelle suchen), `close_from_inside` FAIL (Reichweite: Innenpunkt weiter an die Tür rücken), oder eine bisher bestehende Prüfung regressiert.

## 6. Bekannte offene Punkte
Obstacles (30 cm gehend), StairsAndRamps (30-cm-Stufen), Sprünge 50/60 und Spätsprünge, SprintTurn 90° (alles unverändert, nur besser erklärt, siehe `M2A2-Overnight-Progress.md`); Links/Rechts-Asymmetrie; visuelle Prüfung der Karte; Git: `.umap`/`.uasset` sind ignoriert (`git add -f`, LFS), `Lvl_SpnMovementTestV2.umap` ändert sich nach Schritt 5.

## 7. Zurückgestellt (Roadmap, nichts davon begonnen)
Inventarsystem (eigener späterer Meilenstein) und interaktive Weltkarte (Darstellung, Interaktion und Gestaltung sind unentschieden und werden hier nicht festgelegt).

## 8. Nachtrag Sitzung 2 (Startcommit `b69eb90`)
* **Ein Aufruf statt vieler Schritte:** `powershell -File Tools\windows\run_m2a2_integration.ps1 -EnginePath "C:\Program Files\Unreal Engine\UE_5.8"` (zuerst mit `-DryRun`). Er sichert die V2-Karte nach `Saved\M2A2-Backup\<Zeit>`, erzeugt nur V2 neu, prüft das Generator-Log, startet `-Suite Doors` (Parameter `-Suite Quick|Movement|Full`), vergleicht den Hash der V1-Karte vorher/nachher und wertet die Traces aus. Core-Erwartung jetzt **280** Tests, Python **198**.
* **Neue Exit-Codes von `spn_qa_report.py` / `run_gameplay_qa.ps1`:** 0 vollständig ohne Fehler, 1 vollständig mit FAIL/BLOCKED, **2 unvollständig** (Timeout, Suite ohne Record, keine `records.jsonl`). Ein nicht-null Exit-Code von Unreal allein ist nur ein Hinweis.
* **Neue Artefakte:** `traces.jsonl` im QA-Ordner (Turns-Traces, Jump-Traces). Auswertung: `python Tools\spn_trace_tools.py <QA-Ordner>` (Exit 1 bei Asymmetrie). Im Bericht: "Mirrored sprint turns".
* **Neuer Build-Risikopunkt (zuerst nachsehen, falls der Build scheitert):** `SpnMovementDiagnostics.{h,cpp}` (Bindung `OnComponentHit` am Kapselcomponent, `EAllowShrinking::No`, `GetWalkableFloorZ`, `PerchRadiusThreshold`, `PerchAdditionalHeight`, `bUseFlatBaseForFloorChecks`, `IsWalkable(Hit)`), `SpnQaRecorder.h` (`SpnQa::Trace`), `SpnQaMovementAutomation.cpp` (Trace-Aufnahme in Turns und Jump, gemischte `TArray<double>`-Initialisierer).
* **Messläufe für die offenen Bewegungsfehler:** In einer Sitzung `spn.MoveDiag 1` setzen, `-Suite Movement` laufen lassen. Dann `python Tools\spn_log_tools.py diag <game.log> --ledge-cm 30 --distance-cm 10` (liefert `config`, `hit_summary`: ein `walkable_corner_contact` ab Kantenhöhe bestätigt die Einrast-Hypothese) und `python Tools\spn_trace_tools.py <QA-Ordner>` (`jumps.<fall>.classification`, `turns.verdict`).
* **Generator:** Das Editor-Skript bricht V2 im Preflight ab, wenn eine Tür nicht passierbar ist oder eine lange Engstelle hat (`validate_door_passages`); V1 ist davon ausgenommen.
