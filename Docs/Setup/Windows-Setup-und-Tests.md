# Windows: Einrichtung, Tests, gepackter Build und Messung (Stand 2026-10-22)

Technische Arbeitsnotiz, **kein Planungsmaster**. Alle Schritte wurden am 2026-10-22 auf dem Entwickler-PC (Windows 11, Ryzen 7 3700X, RTX 3090) ausgeführt, **nicht** an einem sauberen Klon (siehe „Nicht geprüft“). Pfade mit `<repo>` stehen für das Repository-Verzeichnis; es gibt keine festen Pfade im Quellcode.

## 1. Voraussetzungen
- Windows 11, Git und **Git LFS** (`.umap`/`.uasset` sind LFS-Dateien, siehe `.gitattributes`).
- Unreal Engine **5.8.3** (Epic Launcher, Standardpfad `C:\Program Files\Unreal Engine\UE_5.8`).
- Visual Studio 2026 Community mit C++-Workloads (siehe `Unreal/SupernaturalProt/.vsconfig`); gebaut wurde mit MSVC 14.44.
- Für Core-Tests: CMake (liegt im Visual Studio unter `Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin`).
- Kurze Build-Pfade (Windows MAX_PATH), z. B. `<repo>\build\windows-msvc`.

## 2. Klonen
```
git clone <repo> && cd <repo>
git lfs pull
```
Das verschachtelte Verzeichnis `Supernatural-3D-Game/` im Arbeitsbaum gehört nicht zum Projekt und wird nicht verwendet.

## 3. Core-Tests (C++20, ohne Unreal)
```
cmake -S . -B build/windows-msvc
cmake --build build/windows-msvc --config Debug -j 8
ctest --test-dir build/windows-msvc -C Debug --output-on-failure
```
Ergebnis am 2026-10-22: **198 von 198 bestanden** (MSVC, Debug).

## 4. Unreal-Editor bauen
```
"C:\Program Files\Unreal Engine\UE_5.8\Engine\Build\BatchFiles\Build.bat" SupernaturalProtEditor Win64 Development -Project=<repo>\Unreal\SupernaturalProt\SupernaturalProt.uproject -WaitMutex
```
Ergebnis am 2026-10-22: Build erfolgreich. Der Core wird direkt aus `<repo>\Core\src` in das Modul `SpnCore` kompiliert (`SPN_CORE_ROOT` ist optional, siehe `Docs/Decisions/0006-unreal-core-integration.md`).

## 4a. Testkarte neu erzeugen (nur falls nötig)
Die Karte `/Game/SpnTest/Lvl_SpnDoorTest` ist im Repository (LFS). Neu erzeugen, **nur diese Karte**, mit Backup vorher:
```
UnrealEditor-Cmd.exe <repo>\Unreal\SupernaturalProt\SupernaturalProt.uproject -run=pythonscript "-script=<repo>\Unreal\SupernaturalProt\Tools\create_spn_door_test_map.py --force" -unattended -nullrhi
```
Inhalt: Boden, Licht, Player Start, drei Türen, zwei Pickups und die M1-Geometrie (`SPN_M1_Ramp`, `SPN_M1_Stair_01..10`, `SPN_M1_StairLanding`, `SPN_M1_Wall`).

## 5. Unreal-Automationstests
```
UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnCore+SpnGame;Quit" -unattended -nullrhi
```
Ergebnis am 2026-10-22: siehe README §26.3 (alle SPN-Tests bestanden, ohne Renderer). Neu für M1: `SpnGame.TestMap.M1Layout` (Maße der Geometrie) und `SpnGame.TestMap.M1Traversal` (Figur läuft Treppe und Rampe hinauf, wird von der Wand gestoppt, springt über die Wand; Sprint-Belegung; Kamera-Einstellungen). Die Tests laufen mit `-nullrhi`; sie prüfen **nicht** das Aussehen und **nicht**, dass die Kamera in der Wand sichtbar clippt.

## 6. Gepackter Windows-Development-Build
Die Standardkarte (`GameDefaultMap` = Vorlagenkarte) bleibt unverändert; die Testkarte wird per `-map` gecookt und per Startargument geöffnet.
```
RunUAT.bat BuildCookRun -project=<uproject> -noP4 -platform=Win64 -clientconfig=Development -build -cook -stage -pak -archive -archivedirectory=<ausgabe> -map=/Game/SpnTest/Lvl_SpnDoorTest -unattended
```
Start (Testkarte):
```
<ausgabe>\Windows\SupernaturalProt.exe /Game/SpnTest/Lvl_SpnDoorTest -windowed -ResX=1920 -ResY=1080 -log
```
Ohne Kartenargument startet der Build die Vorlagenkarte `Lvl_ThirdPerson`. Beim Cook erscheinen Warnungen zu `FText` aus `FString` („Saving FText … PLACEHOLDER door/pickup“); sie betreffen die Beschriftungen der Testobjekte und sind nicht behoben. Die Ausgabe gehört **nicht** ins Repository.

## 7. Frame-Time-Messung (Renderer, RTX 3090)
```
SupernaturalProt.exe /Game/SpnTest/Lvl_SpnDoorTest -windowed -ResX=1920 -ResY=1080 -log -novsync -ExecCmds="t.MaxFPS 0,CsvProfile Frames=4000"
```
Das CSV liegt unter `<ausgabe>\Windows\SupernaturalProt\Saved\Profiling\CSV\`; die ersten 600 Frames gelten als Aufwärmphase. Die Figur steht dabei am Player Start (keine Bewegung). Ergebnisse und Einschränkungen: README §26.5.

## 8. Manueller PIE-Test (steht aus — vom Entwickler auszuführen)
Öffne `/Game/SpnTest/Lvl_SpnDoorTest`, Play (PIE):
1. Türen: `E` auf `door_a`/`door_b` öffnet/schließt; `door_locked` lehnt ab.
2. Pickups: `E` auf beide Kisten; `Spn.Inventory` zeigt `test_item=3`.
3. `Spn.SaveWorld`, PIE neu starten, `Spn.LoadWorld`: Türen und Pickups im gespeicherten Zustand.
4. Bewegung: Treppe und Rampe hinauf und hinunter, mit **Shift** schneller laufen, gegen die Wand laufen, darüberspringen.
5. Kamera: mit dem Rücken zur Wand die Kamera drehen; sie darf nicht durch die Wand oder in die Wand hinein zeigen.
Dieser Test ist **nicht** durchgeführt; er wird nirgends als bestanden geführt.

## 9. Ergebnisse des M1-Audits (2026-10-22)
- Core: 198/198, Unreal-Automation: 17/17 (auch in einer sauberen Arbeitskopie ohne `.git`, `build`, `Intermediate`, `Saved`, `Binaries`, `DerivedDataCache` und ohne das verschachtelte Repository; Editor-Neubau etwa 4 min).
- Zweites Packaging in einen neuen Ordner: erfolgreich, dieselben 48 Dateien, nicht bit-identisch.
- Start des Pakets: mit Argument `/Game/SpnTest/Lvl_SpnDoorTest` lädt die M1-Karte, ohne Argument die Vorlagenkarte `Lvl_ThirdPerson` (Standardkarte bewusst unverändert).
- Warnungen: beim Cook nur `FText`-Hinweise zu den Platzhalter-Beschriftungen; zur Laufzeit keine `Warning:`/`Error:`-Zeilen. `aqProf.dll`, `VtuneApi*.dll`, `WinPixGpuCapturer.dll` „Failed to load“ sind optionale Profiler-DLLs und harmlos.
- Konsolenbefehle im Spiel: `Spn.SaveWorld`, `Spn.LoadWorld`, `Spn.Inventory` (jeweils vollständig tippen).

## 10. Zweiter Audit (2026-10-22): Tests, die auch im Paket laufen
- Neue Tests: `SpnGame.TestMap.M1Camera` (echte Kollisionsabfragen der Kamera, Screenshots mit Renderer), `SpnGame.TestMap.PlayPickups` (Reichweite, wiederholtes E, `test_item = 3`, Türen, `Spn.Inventory`, Speichern/Laden), `SpnGame.TestPickup.FailedLoadKeepsInventory`. Gesamt: 19/19.
- Kamera mit Renderer und Screenshots (liegen in `Saved/Screenshots/WindowsEditor/SpnCam_*.png`):
  `UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.TestMap.M1Camera;Quit" -unattended -nosplash -windowed -ResX=1280 -ResY=720`
- Spieltest im **gepackten** Build, Speicherstand bleibt liegen (`spn_m1_audit`):
  `SupernaturalProt.exe /Game/SpnTest/Lvl_SpnDoorTest -ExecCmds="Automation RunTests SpnGame.TestMap.PlayPickups;Quit" -SpnKeepSlot -unattended -nullrhi -log`
- **Neuer Prozess**: Speicherstand laden und Inventar prüfen:
  `SupernaturalProt.exe /Game/SpnTest/Lvl_SpnDoorTest -ExecCmds="Spn.Inventory,Spn.LoadWorld spn_m1_audit,Spn.Inventory,Quit" -log -windowed`
  Erwartet im Log: erst `empty, revision 0`, dann `Spn.LoadWorld OK ... revision 6` und `test_item=3`.
- Fehlender oder beschädigter Slot: `Spn.LoadWorld <slot>` meldet `SlotNotFound` bzw. `CorruptData` und ändert nichts.
- Hinweis: Die Automation startet mehrere PIE-Sitzungen in einem Editor-Prozess mit **derselben** Game-Instanz; Spieltests setzen den Weltzustand deshalb zuerst zurück (`Spn/Tests/SpnTestPlayReset.h`). Ein echter Neustart (PIE oder Prozess) beginnt leer.

## 11. Kamera-Glättung (2026-10-22, dritter Audit)
- Die Kamera-Armlänge wird bei Hindernissen sofort verkürzt (Kollision hat Vorrang) und wächst danach höchstens mit `CameraArmRecoverySpeed` (300 cm/s, Platzhalter, am Charakter einstellbar; 0 = sofort) zurück.
- `SpnGame.TestMap.M1Camera` prüft Kollisionsfreiheit, das Ausblenden des Meshs bei sehr kurzem Abstand und das weiche Zurückwachsen. Kontrollläufe (nur zur Beweisführung): `-SpnCameraNoSmoothing` stellt das alte Verhalten her (Test schlägt bei „no pop“ fehl); `-SpnCameraNegativeControl` schaltet die Kollision aus (Test schlägt fehl).
- Letztes gepacktes Paket: `C:\Projekte\_SPN_Builds\M1_final` (außerhalb des Repositorys). Die Befehle aus Abschnitt 10 gelten dort unverändert.

## Nicht geprüft
- Bau aus einem echten **`git clone` mit `git lfs pull`** (die M1-Änderungen sind noch nicht committet; geprüft wurde nur eine Kopie des Arbeitsbaums).
- Packaging-Konfigurationen außer Development; Shipping-Build; Linux-Build.
- Manuelle Kamera-Qualität, Spielgefühl und Sprint-Geschwindigkeit (800 cm/s ist ein Platzhalterwert).
