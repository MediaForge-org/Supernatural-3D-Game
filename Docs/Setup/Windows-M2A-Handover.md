# Windows-Übergabe M2-A: Bewegung, Gangarten, Sprunghöhe, Bewegungs-Testkarte (Stand 2026-10-25)

Technische Arbeitsnotiz, **kein Planungsmaster**. Hintergrund und Entscheidungen: [Proposal 0011](../Proposals/0011-m2a-movement-traversal-and-test-map.md). Geschrieben auf Fedora **ohne Unreal Engine**: Kein Unreal-Code aus diesem Paket wurde bisher kompiliert oder ausgeführt, die Karte `Lvl_SpnMovementTest.umap` existiert noch nicht. `<repo>` steht für das Repository-Verzeichnis, `<uproject>` für `<repo>\Unreal\SupernaturalProt\SupernaturalProt.uproject`, `<UE>` für das Engine-Verzeichnis (z. B. `C:\Program Files\Unreal Engine\UE_5.8`).

## 1. Benötigte Dateien
Neu:
- `Core/include/spn/core/Movement.h`, `Core/src/Movement.cpp`, `Tests/Core/MovementTests.cpp` (portabel; `Core/CMakeLists.txt` und `Tests/Core/CMakeLists.txt` angepasst)
- `Unreal/SupernaturalProt/Source/SpnCore/Public/SpnMovement.h`, `.../Private/SpnMovement.cpp`
- `Unreal/SupernaturalProt/Source/SupernaturalProt/Spn/SpnCharacterMovementComponent.{h,cpp}`, `SpnPlayAreaGuard.{h,cpp}`
- Tests: `Spn/Tests/SpnMovementAutomation.cpp`, `SpnMovementMapAutomation.cpp`, `SpnMovementTestSupport.h`
- Karte: `Unreal/SupernaturalProt/Tools/spn_movement_map_layout.py`, `create_spn_movement_test_map.py`, `test_spn_movement_map_layout.py`
- Vorbereitet, nicht im Build: `Unreal/SupernaturalProt/Tools/prepared/SpnMovementNetworkAutomation.cpp.txt`
Geändert: `SupernaturalProtCharacter.{h,cpp}` (eigene Movement-Komponente, Sprint/Gangwechsel, `SprintSpeed` entfernt), `Spn/Tests/SpnTestMapM1Automation.cpp` (siehe Proposal 0011 §4.2), `Tests/CMakeLists.txt` (Python-Layouttest).
Nicht ändern: `Planning/Master`.

## 2. Voraussetzungen
Wie [Windows-Setup-und-Tests.md](Windows-Setup-und-Tests.md) §1 (Git LFS, Visual Studio mit C++-Workloads, CMake). **Engine-Version prüfen:** das Dokument nennt 5.8.3, der Auftrag 5.8.4. Die `.uproject` hat eine GUID-Zuordnung (`{DE4DD34C-4D4D-299E-9466-62A2AC1D1153}`), Assets tragen die Kennung 5.6. Bei Warnungen zur Engine-Zuordnung: Rechtsklick auf die `.uproject` → „Switch Unreal Engine version", **nur nach Rücksprache** (ändert die `.uproject`). Python für die Kartenerzeugung: dieselbe `-run=pythonscript`-Methode wie in Setup §4a (dort auf Windows erfolgreich benutzt); geht sie nicht, das Plugin „Python Editor Script Plugin" prüfen.

## 3. Build-Schritte
```
cmake -S . -B build/windows-msvc
cmake --build build/windows-msvc --config Debug -j 8
ctest --test-dir build/windows-msvc -C Debug --output-on-failure
```
Erwartet: 207 Core-Testfälle + `movement_test_map_layout` = 208 bestanden (Fedora: so erreicht; MSVC noch offen).
```
"<UE>\Engine\Build\BatchFiles\Build.bat" SupernaturalProtEditor Win64 Development -Project=<uproject> -WaitMutex
```
**Erster Unreal-Build dieses Pakets:** Kompilierfehler sind möglich (siehe §9). Neue UCLASS/USTRUCT/UENUM: UnrealHeaderTool läuft automatisch; ein Live-Coding-Build reicht nicht, Editor schließen und per Build.bat bauen.

## 4. Testkarte erzeugen
```
UnrealEditor-Cmd.exe <uproject> -run=pythonscript "-script=<repo>\Unreal\SupernaturalProt\Tools\create_spn_movement_test_map.py" -unattended -nullrhi
```
Vorhandene Karte wird nicht überschrieben; neu bauen mit `--force` hinter dem Skriptpfad im `-script="..."`-Argument (nur diese Karte). Erwartetes Log: `SpnMovementTestMap: created /Game/SpnTest/Lvl_SpnMovementTest with 319 shapes, 2 doors`, außerdem drei Zeilen „native size of cube/sphere/cylinder". Die Datei entsteht unter `Content/SpnTest/Lvl_SpnMovementTest.umap` (Git LFS, `git lfs ls-files` zeigt sie nach `git add`; **Commit macht der Projektinhaber**).
Vor dem Skript prüfen (ohne Unreal, geht auch auf Windows mit Python): `python -I Unreal\SupernaturalProt\Tools\test_spn_movement_map_layout.py` → 19 Tests OK.
Danach in den Editor, Karte öffnen, optisch prüfen (§7).

## 5. Editor-Werkzeuge
Python-Skript (oben), Output Log (Filter `SpnMovementTestMap`), Outliner-Ordner `SPN_MV/<zone>`, Automation-Fenster (Session Frontend → Automation, Filter `SpnGame.Movement`), Konsole im PIE.

## 6. Testablauf
1. Core- und Layouttests (§3), Editor bauen (§3).
2. Karte erzeugen (§4).
3. Automation (alle SPN-Tests; erwartet nach Karte: 19 bestehende + 3 + 7 neue, ohne Netzwerktest):
```
UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnCore+SpnGame;Quit" -unattended -nullrhi
```
Einzeln: `SpnGame.Movement.Settings`, `.CharacterDefaults`, `.PredictionFlags` (keine Karte nötig), `.MapLayout`, `.Jump`, `.Gaits`, `.Obstacles`, `.StairsAndRamps`, `.Boundary`, `.AlleyCamera`, `SpnGame.TestMap.M1Traversal`. Fehlt die Karte, scheitern die Kartentests mit einer Meldung (kein stilles Bestehen).
4. Manuell (PIE, Karte `Lvl_SpnMovementTest`):
   - WASD + **Shift (halten) = Sprint**, **V = Grundgang Walk → Jog → Run**, Leertaste = Sprung; Gamepad: Kreuz/A = Sprung, L3 = Sprint. Analogstick halb ausgelenkt = halbe Geschwindigkeit.
   - Sprung: etwa 45 cm; Leertaste halten ergibt keinen zweiten Sprung.
   - Hindernisreihe bei X = 3000/5000: 20 bis 40 cm laufen hoch, ab 50 cm blockiert (erwartet, Proposal 0011 §4.1).
   - Rampen bei X = −9000 (bis 40° begehbar, 50° nicht), Treppen bei Y = 4000 bis 6500 (Stufen 15 bis 40 cm gehen, 50 cm nicht), Hügel, Terrassenhügel, Felsfeld, Gasse (Y 4000 bis 7000, niedrige Decke in der Mitte: Kamera zoomt ein).
   - Gebäude: Hütte, Haus (Tür `E`, Treppe nach oben), Lagerhalle (Rampe zur 1-m-Ladekante).
   - Rand: gegen die Außenwand laufen und springen; über die Wand ist kein Weg.
5. Netzwerk (manuell, **nicht automatisiert geprüft**): Play → Net Mode „Play As Listen Server", Anzahl Spieler 2. Im Client-Fenster Konsole: `p.NetShowCorrections 1`, danach 5 s sprinten (Shift + vorwärts), Gänge wechseln, springen, gegen Wand laufen. Mit Latenz: `NetEmulation.PktLag 100`, `NetEmulation.PktLoss 3`. Beobachten: Log-Kategorie `LogNetPlayerMovement` (Korrekturen) und rote/grüne Korrekturlinien. Erwartet: keine dauerhaften Korrekturen beim Sprinten, kleine bei Verzögerung.

## 7. Erwartete Ergebnisse
- Sprunghöhe 45 ± 7 cm (Messung Kapselmitte), ein Absprung bei gehaltener Taste.
- Eingeschwungene Geschwindigkeit je Gang: Walk 140, Jog 300, Run 500, Sprint 800 cm/s (± 6 %); Halbstick-Jog ≈ 150.
- Beschleunigung ohne Sprünge (≤ 1,25 × a·dt je Frame), Bremsen monoton, Stillstand in etwa v/8 m/s² (+ Toleranz).
- Hindernisse: 20/30/40 cm überwunden, 50 bis 150 cm blockiert, auch mit Sprungversuchen; kein übertriebener Sprung.
- Treppen mit Stufenhöhe 15 bis 40 cm erklommen, 50 cm nicht; Rampen bis 40° erklommen, 50° nicht.
- Außenwände halten (alle vier Seiten); die Wächter-Box setzt eine über die Wandoberkante verschobene Figur zurück.
- Kamera in der Gasse: Kamera unter 230 cm, Arm verkürzt.

## 8. Bekannte Einschränkungen
- Nichts hiervon lief in Unreal. Toleranzen sind Schätzwerte.
- Netzwerk: Gangwahl und Sprint laufen über die komprimierten Move-Flags (Standardmuster), **nicht** bewiesen; `SetMovementLimits` ist nicht repliziert; `SpnGame.Movement.NetworkSprint` ist nur als Textdatei vorbereitet (Kopf der Datei beschreibt Aktivierung).
- Animation: vorhanden sind `ABP_Unarmed` (Blendspace `BS_Idle_Walk_Run`, Walk-/Jog-Sets in acht Richtungen, `MM_Jump`, `MM_Fall_Loop`, `MM_Land`); es gibt keine eigenen Run-/Sprint-Clips, und der Wertebereich der Geschwindigkeitsachse des Blendspaces ist aus den Binärdateien nicht lesbar. **Nicht geprüft:** ob die Beine bei 140/300/500/800 cm/s zur Bewegung passen (Füße gleiten?) und ob die Sprunganimation zur neuen Flugzeit passt (vorher ≈ 1,02 s, jetzt ≈ 0,61 s bei 45 cm). Editor-Schritte: im `ABP_Unarmed` die Blendspace-Achsenwerte an 0 bis 800 cm/s anpassen, Jump-Start/Fall/Land-Abspielraten prüfen, ggf. `GetObservedGait()` als Variable einführen. Das ist Binärarbeit und nicht auf Fedora möglich.
- Rückwärts-/Seitwärtsfaktoren wirken erst mit einem Modus, in dem sich die Figur nicht zur Bewegung dreht.
- Oberflächen: nur optische Materialien; es gibt keine Physical Materials.
- Sphären-Hügel setzen voraus, dass `/Engine/BasicShapes/Sphere` eine Kugelkollision hat (sonst Kollision im Editor prüfen).
- `RefreshJumpVelocity()` muss nach Laufzeit-Änderung der Schwerkraft aufgerufen werden.

## 9. Fehlersuche
- **Kompilierfehler in `SpnCharacterMovementComponent`:** am ehesten Signaturen von `FSavedMove_Character::{CanCombineWith, SetMoveFor, PrepMoveFor}`, `GetPredictionData_Client`, `UpdateFromCompressedFlags`, `GetMaxSpeed/GetMaxAcceleration` in 5.8; mit der Engine-Header-Datei `GameFramework/CharacterMovementComponent.h` abgleichen.
- **Kompilierfehler in `Spn/Tests/*`:** `TestEqual` auf Enums wurde vermieden; `EAutomationTestFlags` wird wie in den M1-Tests verwendet.
- **UHT-Fehler zu `FSpnMovementSettings`:** `SpnMovement.generated.h` braucht den Build per Build.bat.
- **Figur steht/läuft nicht:** Output Log nach `invalid movement settings` durchsuchen (ApplySettings meldet das und behält die alten Werte).
- **Sprunghöhe falsch:** Log nach `cannot derive a jump velocity`; ein Blueprint-Wert für `JumpZVelocity` wird beim Start überschrieben, Quelle der Wahrheit ist `Settings.JumpHeight`.
- **Karte fehlt/Skript startet nicht:** Python-Plugin, Log nach `SpnMovementTestMap:`; ggf. `-stdout -FullStdOutLogOutput` anhängen.
- **Karte: Kugeln oder Zylinder falsch skaliert:** das Skript misst die Mesh-Größe selbst (Logzeilen „native size").
- **Kartentests „does not exist yet":** Karte nicht erzeugt (§4).
- **Doppelte Tür-IDs:** Türen der Karte heißen `mv_hut_door`, `mv_house_door` (Welt-Zustand ist je Spiel gemeinsam, Namen bewusst anders als in `Lvl_SpnDoorTest`).

## 10. Abnahmekriterien M2-A (vorgeschlagen)
1. Core + Layout: 208/208 (MSVC).
2. Editor-Build fehlerfrei; keine neuen Warnungen aus SPN-Code (bekannte 7 C4996 aus `UObject/Class.h` ausgenommen).
3. Karte erzeugt (Log wie §4), `.umap` im Repository-Arbeitsbaum.
4. Alle `SpnGame.Movement.*`-Tests und `SpnGame.TestMap.*` bestanden (Zahl im Bericht nennen).
5. Manueller PIE-Durchlauf §6.4 ohne Blocker; Spielgefühl der Gangarten bewertet (Zahlen sind Testwerte).
6. Netzwerk §6.5: keine dauerhaften Korrekturen beim Sprint (oder dokumentierte Fehler).
7. Animationsbefund (§8) mit konkreten Editor-Aufgaben notiert.
8. Offene Entscheidungen aus Proposal 0011 §6 beantwortet oder bewusst vertagt.

## Verifiziert auf Fedora (2026-10-25)
Siehe Abschlussbericht und Proposal 0011 §3.6: Core 208/208 in vier Presets (Clang/GCC, je mit und ohne ASan/UBSan), Python-Layouttest 19/19, Kartenskript gegen eine Attrappe der Editor-API ausgeführt (prüft nur die Skriptlogik: 326 gespawnte Akteure).
