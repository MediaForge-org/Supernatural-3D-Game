# Windows-Übergabe M2-A / M2-A.1: Bewegung, Gangarten, Sprung- und Stufenhöhe, Bewegungs-Testkarte (Stand 2026-10-26)

Technische Arbeitsnotiz, **kein Planungsmaster**. Hintergrund und Entscheidungen: [Proposal 0011](../Proposals/0011-m2a-movement-traversal-and-test-map.md). Geschrieben auf Fedora **ohne Unreal Engine**. **Nichts aus M2-A oder M2-A.1 wurde bisher von Unreal gebaut oder ausgeführt, die Karte `Lvl_SpnMovementTest.umap` existiert nicht. M2-A gilt erst als abgeschlossen, wenn Abschnitt 5 erfüllt ist.**

`<repo>` = Repository-Verzeichnis, `<uproject>` = `<repo>\Unreal\SupernaturalProt\SupernaturalProt.uproject`, `<UE>` = Engine-Verzeichnis (z. B. `C:\Program Files\Unreal Engine\UE_5.8`). Allgemeine Windows-Voraussetzungen (Git LFS, Visual Studio mit C++-Workloads, CMake, kurze Pfade): [Windows-Setup-und-Tests.md](Windows-Setup-und-Tests.md) §1.

## 0. Nachgewiesen und angenommen

| Aussage | Stand |
|---|---|
| Der portable Core (Gangarten, Sprung, Step-up, Bewegungszustände, Hindernisklassen) ist korrekt wie geschrieben | **nachgewiesen** auf Fedora: 215 doctest-Fälle in Clang/GCC, je mit und ohne ASan/UBSan, Warnungen als Fehler |
| Kartenlayout (393 Formen, 2 Türen, 14 Rücksetzpunkte) ist deterministisch und in sich stimmig; Editor-Skript, C++-Testkonstanten und Standardwerte passen zum Layout | **nachgewiesen** auf Fedora: 46 Python-Prüfungen |
| Brücke, Movement-Komponente und Guard sind untereinander konsistent (Header gegen Quelltext) | **nachgewiesen** nur gegen **selbstgeschriebene Engine-Attrappen** (`Tools/stubcheck`); sagt nichts über die echte Engine-API |
| Der Code baut mit Unreal 5.8 | **nicht geprüft** |
| Die Engine-API-Annahmen (Proposal 0011 §4.9) stimmen | **angenommen** |
| Die Karte entsteht wie beschrieben, Engine-Formen haben Kollision, Skalierung stimmt | **angenommen** (Skript lief nur gegen eine Attrappe der Editor-API) |
| Gangarten, Sprung, Step-up, Sprint-Wende, Prediction, Kamera, Animation verhalten sich wie geplant | **angenommen**, kein Unreal-Lauf |
| Windows erkennt das DualSense als Gamepad mit den üblichen Tasten | **angenommen**, nicht geprüft |

## 1. Dateien

Neu seit M2-A (Commit `bd62eb6`, siehe `git show --stat bd62eb6`) bzw. geändert in M2-A.1 (zum Zeitpunkt der Übergabe **noch nicht committet**; Commits macht der Projektinhaber):
- Core: `Core/include/spn/core/Movement.h`, `Core/src/Movement.cpp`, `Tests/Core/MovementTests.cpp`, `Tests/CMakeLists.txt`.
- Brücke: `Unreal/SupernaturalProt/Source/SpnCore/Public/SpnMovement.h`, `.../Private/SpnMovement.cpp`.
- Spiel-Modul: `Source/SupernaturalProt/Spn/SpnCharacterMovementComponent.{h,cpp}`, `SpnPlayAreaGuard.{h,cpp}`, `SupernaturalProtCharacter.{h,cpp}`, `SupernaturalProt.Build.cs` (`SpnCore` jetzt öffentliche Abhängigkeit).
- Tests: `Spn/Tests/SpnMovementAutomation.cpp`, `SpnMovementMapAutomation.cpp`, `SpnMovementTestSupport.h`, `SpnTestMapM1Automation.cpp` (Stufenprüfung gegen 25 cm).
- Karte und Werkzeuge: `Unreal/SupernaturalProt/Tools/spn_movement_map_layout.py`, `create_spn_movement_test_map.py`, `test_spn_movement_map_layout.py`, `stubcheck/` (Attrappen-Prüfung, nur Linux), `prepared/SpnMovementNetworkAutomation.cpp.txt` (nicht im Build).
- Nicht ändern: `Planning/Master`.

## 2. Reihenfolge auf Windows (15 Schritte)

### 1. Repository synchronisieren
```
cd <repo>
git status
git log --oneline -3
git lfs pull
```
Prüfen, dass die M2-A.1-Änderungen im Arbeitsbaum oder in einem Commit des Projektinhabers sind (`git status` zeigt sonst die Dateien aus §1). Nichts zurücksetzen oder bereinigen.

### 2. Projektzustand prüfen
```
type "<UE>\Engine\Build\Build.version"
```
Die Version notieren (Dokumente nennen 5.8.3, der Auftrag 5.8.4). Die `.uproject` hat die GUID-Zuordnung `{DE4DD34C-4D4D-299E-9466-62A2AC1D1153}`; Assets tragen die Kennung 5.6. Bei Warnungen zur Engine-Zuordnung nur nach Rücksprache „Switch Unreal Engine version“ verwenden (ändert die `.uproject`). Python: `python --version`; für die Kartenerzeugung muss das „Python Editor Script Plugin“ der Engine aktiv sein (Setup-Dokument §4a hat die Methode auf Windows erfolgreich benutzt).

### 3. Portable Tests (Core und Python)
```
cmake -S . -B build/windows-msvc
cmake --build build/windows-msvc --config Debug -j 8
ctest --test-dir build/windows-msvc -C Debug --output-on-failure
python -I Unreal\SupernaturalProt\Tools\test_spn_movement_map_layout.py
```
Erwartet: **216** ctest-Tests bestanden (215 doctest-Fälle + `movement_test_map_layout`; auf Fedora zusätzlich `unreal_stub_check`, das es unter Windows nicht gibt), Python-Skript `Ran 46 tests ... OK`. MSVC-Ergebnis ist offen. Weicht die Zahl ab, zuerst die Zahl und die fehlgeschlagenen Tests melden, nichts anpassen.

### 4. Unreal-Projektdateien vorbereiten
Für den Kommandozeilen-Build (Schritt 5) sind keine Visual-Studio-Projektdateien nötig. Für die IDE: Rechtsklick auf `<uproject>` → „Generate Visual Studio project files“. Kein Live-Coding verwenden: neue `UCLASS/USTRUCT/UENUM` brauchen den UnrealHeaderTool-Lauf, also Editor schließen und per `Build.bat` bauen.

### 5. Editor-Build
```
"<UE>\Engine\Build\BatchFiles\Build.bat" SupernaturalProtEditor Win64 Development -Project=<uproject> -WaitMutex
```
Erwartet (Ziel): fehlerfrei; keine neuen Warnungen aus SPN-Code (bekannt und ausgenommen: 7 × C4996 aus `UObject/Class.h`). **Das ist der erste Build dieses Codes: Fehler sind möglich.** Das Log (`<repo>\Unreal\SupernaturalProt\Saved\Logs` bzw. die Konsolenausgabe) vollständig aufheben.

### 6. Kompilierfehler beheben
Erst melden, dann beheben. Wahrscheinlichste Stellen, in dieser Reihenfolge (Annahmen aus Proposal 0011 §4.9):
1. `Spn/SpnCharacterMovementComponent.{h,cpp}`: Signaturen von `FSavedMove_Character::{CanCombineWith, SetMoveFor, PrepMoveFor, Clear, GetCompressedFlags}`, `FNetworkPredictionData_Client_Character::AllocateNewMove`, `GetPredictionData_Client`, `UpdateFromCompressedFlags`, `OnMovementModeChanged`, Namen der Member (`ClientPredictionData`, `MinAnalogWalkSpeed`, `BrakingFriction`, `bUseSeparateBrakingFriction`). Abgleich: `<UE>\Engine\Source\Runtime\Engine\Classes\GameFramework\CharacterMovementComponent.h`.
2. `Spn/Tests/*.cpp`: `AddExpectedError`, `AddInfo`, `OverlapBlockingTestByChannel`, `SCENE_QUERY_STAT`, `TestEqual`-Überladungen (Enums und Bools wurden vermieden).
3. `SpnCore`: `SpnMovement.generated.h` entsteht erst durch den UHT-Lauf; Fehler wie „cannot open include file“ zuerst mit einem vollständigen Build.bat-Lauf klären.
4. `SupernaturalProtCharacter.cpp`: `FKey::IsValid`, Konstruktor mit `FObjectInitializer`.
Jede Korrektur im Bericht festhalten (Datei, Zeile, Ursache). Wird eine Annahme der Liste widerlegt, die Liste in Proposal 0011 §4.9 nachführen.

### 7. Testkarte erzeugen
```
UnrealEditor-Cmd.exe <uproject> -run=pythonscript "-script=<repo>\Unreal\SupernaturalProt\Tools\create_spn_movement_test_map.py" -unattended -nullrhi
```
Eine vorhandene Karte bleibt unverändert; neu bauen mit `--force` hinter dem Skriptpfad innerhalb von `-script="..."` (nur diese Karte). Erwartetes Log (Filter `SpnMovementTestMap:`): drei Zeilen „native size of cube/sphere/cylinder“ und `created /Game/SpnTest/Lvl_SpnMovementTest with 393 shapes, 2 doors, 14 reset points`. Meldet das Skript „expected 393 shapes in the level, found N“, ist die Karte unvollständig gespeichert: Log lesen (Material oder Mesh fehlt?).

### 8. Testkarte speichern und prüfen
Das Skript speichert selbst: `<repo>\Unreal\SupernaturalProt\Content\SpnTest\Lvl_SpnMovementTest.umap` muss existieren (Git LFS; **Commit macht der Projektinhaber**). Im Editor öffnen und ansehen: Outliner-Ordner `SPN_MV/<zone>`; Bereiche A bis H (Proposal 0011 §3.5): Plaza, Bahn bei X = −125 m, Rampen X ≈ −90 m, Treppen, Hindernisreihen X = 30/50 m, Hügel, Terrassen, Felsfeld, Gasse, Pfeilerwald, Hütte, Haus, Lagerhalle, vier Außenwände. Prüfen: ein Player Start bei (0, 0, 100), Geländer an Rampen, Treppen, Podesten, Treppenhaus und Ladekante, **keine offene Kante**, Kollision der Kugeln und Zylinder (Player Collision im Viewport). Sieht etwas falsch skaliert aus, melden; das Skript misst die Engine-Formen selbst.

### 9. Unreal-Automationstests
Alle SPN-Tests:
```
UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnCore+SpnGame;Quit" -unattended -nullrhi -ReportExportPath="<repo>\Saved\AutomationReport"
```
Einzeln (jeweils `Automation RunTests <Name>`): ohne Karte `SpnGame.Movement.Settings`, `.CharacterDefaults`, `.PredictionFlags`; mit Karte `.MapLayout`, `.Jump`, `.JumpMoving`, `.Gaits`, `.SprintTurn`, `.StepJumpIndependence`, `.Obstacles`, `.StairsAndRamps`, `.Boundary`, `.AlleyCamera`, `.CameraClearance`; außerdem `SpnGame.TestMap.M1Layout`, `.M1Traversal`, `.M1Camera` (Letzteres mit Renderer, siehe Setup-Dokument §10). Erwartet: 14 Movement-Tests + die 19 bisherigen. Fehlt die Karte, scheitern die Kartentests mit einer Meldung.
**Ergebnisse ohne Wertung:** Die Tests `Obstacles` und `StairsAndRamps` melden Grenzfälle als `MEASURED (not asserted): ...` im Log (25, 30, 40, 45 cm Hindernis und Treppe mit 25 cm Stufe, jeweils „crossed“ oder „blocked“). Diese Zeilen **vollständig in den Bericht übernehmen**: sie entscheiden über `MaxStepHeight` (Proposal 0011 §4.1). Ein „Fehler“ in den übrigen Zeilen ist ein Befund, keine Toleranzfrage: nicht die Toleranz lockern, sondern erst melden.

### 10. Bewegung manuell prüfen (PIE, Karte `Lvl_SpnMovementTest`)
| Prüfung | Erwartet (Testwerte) |
|---|---|
| WASD, halten **Shift** = Sprint, **V** = Walk → Jog → Run | Eingeschwungen Walk 140, Jog 300, Run 500, Sprint 800 cm/s (±6 %); Standard-Gang Jog |
| Sprung (Leertaste), Taste halten | etwa 45 cm, Flugzeit etwa 0,6 s, kein zweiter Sprung, kein Dauerhüpfen |
| Sprung im Stand, im Jog, im Sprint | gleiche Höhe; Bewegung bleibt erhalten, kein Geschwindigkeitsgewinn in der Luft |
| Sprint, dann scharfe Wende (90°) | Sprint bleibt, Tempo bricht nicht ein; Umkehr (> 100°) beendet den Sprint |
| Bahn bei X = −125 m (Start −120 m) | Markierungen alle 10 m: Sprint nach etwa 4,5 s bei etwa 30 m Strecke |
| Hindernisreihe X = 30 m (dünn) und 50 m (dick) | 20 cm gehen hoch; 25 cm Grenzfall; 30 bis 40 cm nur mit Sprung (Sprung **kann** reichen); ab 45 cm unüberwindbar; ab 50 cm sicher blockiert |
| Treppen Y = 40 bis 70 m (rise 15 bis 50 cm) | 15, 18, 22 cm gehen; 25 cm Grenzfall; 30, 40, 50 cm blockieren |
| Rampen 5° bis 40° / 50° | bis 40° begehbar, 50° nicht; Geländer halten die Figur auf der Rampe |
| Hügel, Terrassen (20 cm), Felsfeld | begehbar, ohne Hängenbleiben an den Felskanten (alle unter 25 cm) |
| Gebäude: Hütte, Haus (Tür `E`, 17 Stufen, Geländer am Treppenhaus), Lagerhalle (Ladekante 100 cm nur über die Rampe) | Türöffnungen passierbar (110 cm breit), Fenster nicht passierbar |
| Rand: gegen alle vier Außenwände sprinten und springen | kein Durchkommen. Das Zurücksetzen über die Wand, weit hinter die Ostwand und unter den Boden prüft `SpnGame.Movement.Boundary` (Figur kommt an den nächsten der 14 Rücksetzpunkte zurück); manuell ist es nur über eigenen Testcode oder den Editor erreichbar |
Sprünge und Treppen beobachten: Gleitende Füße und Sprunganimation notieren (Animation, §3).

### 11. Kamera prüfen
Gasse (X −15 bis 6 m, Y 35 bis 75 m; niedrige Decke 230 cm in der Mitte), Hütte, Haus (Halle, mitten auf der Treppe), Pfeilerwald (X −55 bis −37 m, Y −63 bis −47 m), Treppenfeld; dabei den Blick schnell herumreißen. Erwartet: die Kamera sitzt nie in Geometrie, der Arm verkürzt sofort und wächst höchstens mit 300 cm/s zurück, die Figur wird bei weniger als 70 cm Kameraabstand ausgeblendet. Kamera-Code wurde in M2-A.1 **nicht** geändert; Auffälligkeiten melden, nicht sofort ändern.

### 12. Controller testen
Xbox-kompatibel und DualSense, jeweils: linker Stick halb/voll ausgelenkt (halbe/volle Geschwindigkeit, stufenlos), rechter Stick = Blick, **A/Kreuz = Sprung**, **L3 = Sprint** (Platzhalter, nicht entschieden), Interaktion und Gangwechsel: **keine Pad-Belegung** (Kampfsystem 06). Beim DualSense zuerst prüfen, ob Unreal das Gerät als Gamepad liefert (Input-Debug des Editors, z. B. Konsole `showdebug enhancedinput`; der genaue Befehl ist nicht geprüft); Steam Input oder Treiber können die Tasten umlegen. Auffälligkeiten (Deadzone, Drift, Stick-Klick unbequem) im Bericht notieren. Konflikte mit dem Kampfsystem: Proposal 0011 §4.8.

### 13. Netzwerkverhalten prüfen
Manuell: Play → Net Mode „Play As Listen Server“, Anzahl Spieler 2. Im Client-Fenster Konsole:
```
p.NetShowCorrections 1
NetEmulation.PktLag 100
NetEmulation.PktLoss 3
```
Dann 5 s sprinten (Shift + vorwärts), Gänge mit V wechseln, springen, scharf wenden, gegen eine Wand laufen. Beobachten: Log `LogNetPlayerMovement` und die Korrekturlinien. Erwartet: keine dauerhaften Korrekturen beim Sprint und bei der Wende; kleine, seltene Korrekturen mit künstlicher Verzögerung sind normal. Zusätzlich vergleichen: Zeigt die Remote-Figur auf dem anderen Fenster die richtige Gangart (`GetObservedGait`, `GetMovementState`)? Zustand „Landing“ auf Remote-Figuren ist **unverifiziert** (Proposal 0011 §4.5).
Automatisiert (vorbereitet, nicht im Build): `Tools/prepared/SpnMovementNetworkAutomation.cpp.txt` nach `Source/SupernaturalProt/Spn/Tests/SpnMovementNetworkAutomation.cpp` kopieren; in `SupernaturalProt.Build.cs` für Editor-Builds `PrivateDependencyModuleNames.Add("UnrealEd")` ergänzen; bauen; `Automation RunTests SpnGame.Movement.NetworkSprint`. Die PIE-Session-API ändert sich zwischen Versionen: Anpassungen sind zu erwarten und zu melden. **Nicht nachgewiesen:** die Korrektheit der Prediction, die Reaktion auf Paketverlust, mehr als ein Client, `SetMovementLimits` (nicht repliziert), Laufzeitänderungen von `Settings` (nicht repliziert).

### 14. Performance messen
Keine Aussage ohne Messung; vorhandene Zahlen gibt es nicht. Messmethode wie [Windows-Setup-und-Tests.md](Windows-Setup-und-Tests.md) §7, aber auf der Bewegungskarte, gepackt oder im Standalone:
```
UnrealEditor.exe <uproject> /Game/SpnTest/Lvl_SpnMovementTest -game -windowed -ResX=1920 -ResY=1080 -log -novsync -ExecCmds="t.MaxFPS 0,stat unit,CsvProfile Frames=4000"
```
Drei Szenen getrennt aufnehmen: (a) Stand am Player Start, (b) Sprint über die Bahn, (c) Blick über die Karte vom Podest der 40°-Rampe. Zahlen mit Hardware, Konfiguration (Editor/Development) und Szene notieren; die ersten 600 Frames sind Aufwärmphase. Beobachten: Draw Calls (393 Formen, nicht instanziert), Game-Thread-Zeit der Bewegung (`stat game`). Erst bei messbarem Problem optimieren (Optionen: Proposal 0011 §4.10).

### 15. Ergebnisse dokumentieren
Bericht (neues Dokument oder Abschnitt in Proposal 0011): Engine-Version; Build-Ergebnis mit allen Korrekturen; Kartenerzeugung (Log); Testergebnisse mit Zahlen und allen `MEASURED`-Zeilen; manuelle Prüfungen aus 10 bis 12 mit Befund; Netzwerk (Methode, Latenz, Ergebnis); Performance-Zahlen; offene Punkte. Nichts als „bestanden“ führen, was nicht lief.

## 3. Animation (nicht erledigt, Editor-Arbeit)
Vorhanden sind `ABP_Unarmed` (Blendspace `BS_Idle_Walk_Run`, Walk-/Jog-Sets in acht Richtungen, `MM_Jump`, `MM_Fall_Loop`, `MM_Land`). Es gibt keine eigenen Run-/Sprint-Clips; der Wertebereich der Geschwindigkeitsachse ist aus den Binärdateien nicht lesbar. **Nicht geprüft:** Füße gleiten bei 140/300/500/800 cm/s? Passt die Sprunganimation zur neuen Flugzeit (vorher ≈ 1,02 s, jetzt ≈ 0,61 s)?
Editor-Aufgaben: (1) im `ABP_Unarmed` die Achse des Blendspace an 0 bis 800 cm/s anpassen oder Run-/Sprint-Clips ergänzen (Sprint-Clips fehlen, nichts aus Fremdquellen laden); (2) die Zustandsübergänge Idle/Jump/Fall/Land an `GetMovementState()` (Idle, Walk, Jog, Run, Sprint, Jumping, Falling, Landing) hängen statt an eigener Logik; (3) Abspielraten von Jump-Start, Fall und Land an die Flugzeit anpassen; (4) im Event Graph `GetObservedGait`/`GetSecondsSinceLanding` des Movement-Components lesen (BlueprintPure). Eine zweite Zustandsmaschine ist nicht vorgesehen.

## 4. Bekannte Einschränkungen
- Nichts hiervon lief in Unreal. Toleranzen der Tests sind Schätzwerte; Grenzfälle werden gemeldet statt bewertet (Schritt 9).
- Netzwerk: Gangwahl und Sprint laufen über die komprimierten Move-Flags (Standardmuster), **nicht** bewiesen. Der neue Sprint-Halt in Wenden liest nur die Geschwindigkeit (vorhersagter Zustand), ohne neues Flag. Ein möglicher Ein-Frame-Effekt im Analog-Modifikator ist in Proposal 0011 §4.4 beschrieben.
- Rückwärts-/Seitwärtsfaktoren wirken erst mit einem Modus, in dem sich die Figur nicht zur Bewegung dreht.
- Oberflächen: nur optische Materialien; es gibt keine Physical Materials.
- Halbkugel-Hügel setzen voraus, dass `/Engine/BasicShapes/Sphere` eine Kugelkollision hat.
- `RefreshJumpVelocity()` muss nach Laufzeit-Änderung der Schwerkraft aufgerufen werden.
- Tastenbelegung: V (Gangwechsel) und L3 (Sprint am Pad) sind Platzhalter, keine Projektentscheidung.

## 5. Abnahmekriterien M2-A / M2-A.1 (alle erforderlich)
1. Core: 216/216 (MSVC); Python-Layouttest 46/46.
2. Editor-Build erfolgreich, ohne neue Warnungen aus SPN-Code.
3. Karte erzeugt (Log wie Schritt 7), `.umap` im Arbeitsbaum.
4. Alle `SpnGame.Movement.*`- und `SpnGame.TestMap.*`-Tests ausgeführt; Zahl der bestandenen und fehlgeschlagenen im Bericht; alle `MEASURED`-Zeilen übernommen.
5. Manueller PIE-Durchlauf (Schritte 10 bis 12) ohne Blocker; Spielgefühl der Gangarten bewertet (Zahlen sind Testwerte).
6. Netzwerk (Schritt 13): keine dauerhaften Korrekturen beim Sprint, oder dokumentierte Fehler.
7. Animationsbefund (§3) mit konkreten Editor-Aufgaben notiert.
8. Offene Entscheidungen aus Proposal 0011 §8 beantwortet oder bewusst vertagt.

## 6. Fehlersuche
- **UHT-Fehler zu `FSpnMovementSettings`:** `SpnMovement.generated.h` braucht den Build per Build.bat.
- **Figur steht oder läuft nicht:** Output Log nach `invalid movement settings` durchsuchen (`ApplySettings` meldet das und behält die alten Werte).
- **Sprunghöhe falsch:** Log nach `cannot derive a jump velocity`; ein Blueprint-Wert für `JumpZVelocity` wird beim Start überschrieben, Quelle der Wahrheit ist `Settings.JumpHeight`. Stufenhöhe: `Settings.MaxStepHeight` (ein Blueprint-Wert für `MaxStepHeight` wird ebenfalls überschrieben).
- **Karte fehlt oder Skript startet nicht:** Python-Plugin, Log nach `SpnMovementTestMap:`; ggf. `-stdout -FullStdOutLogOutput` anhängen.
- **Kartentests „does not exist yet“:** Karte nicht erzeugt (Schritt 7). **`MapLayout` meldet falsche Anzahl:** Layout geändert, ohne `ExpectedPrimitiveCount` nachzuführen (der Python-Test hätte es gemeldet).
- **Doppelte Tür-IDs:** Türen der Karte heißen `mv_hut_door`, `mv_house_door`, bewusst anders als in `Lvl_SpnDoorTest`.
- **Test „Camera inside geometry“:** Kamerapose und Frame aus dem Log notieren, Kamera nicht blind ändern.

## Verifiziert auf Fedora (2026-10-26)
Core und Python wie Abschnitt 0. Alle vier Presets (fedora-clang, fedora-gcc, fedora-clang-asan-ubsan, fedora-gcc-asan-ubsan): 217/217 ctest-Tests. Kartenskript gegen eine Attrappe der Editor-API: 393 Formen, 2 Türen, 14 Rücksetzpunkte. Kein Unreal-Build, kein Unreal-Test.
