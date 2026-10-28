# Gameplay QA (automatisierte Spieltests mit Videoaufnahme)

Stand 2026-10-28, Unreal Engine 5.8.4, Windows. PLATZHALTER-Testkarte `/Game/SpnTest/Lvl_SpnMovementTestV2`; alle Schwellen sind Platzhalter, keine freigegebenen Zielwerte.

## Start

```powershell
.\Tools\run_gameplay_qa.ps1                    # vollständiger Lauf mit Video (ca. 8 Minuten)
.\Tools\run_gameplay_qa.ps1 -Suite Quick       # schnelle Regression: Spawn, Türen, Reichweite, Save/Load; kleines Fenster, kein Video (ca. 2 Min.)
.\Tools\run_gameplay_qa.ps1 -Suite Movement    # Gangarten, Wenden, Sprünge
.\Tools\run_gameplay_qa.ps1 -Suite Doors       # Türen, Interaktionsreichweite, 9 Gebäude inkl. Treppen
.\Tools\run_gameplay_qa.ps1 -Suite Camera
.\Tools\run_gameplay_qa.ps1 -Suite Performance
.\Tools\run_gameplay_qa.ps1 -Suite Failed      # wiederholt die Testgruppen, die im letzten Lauf FAIL/BLOCKED waren oder nichts gemeldet haben
.\Tools\run_gameplay_qa.ps1 -Suite Full -Regression   # zusätzlich die älteren SpnGame.*/SpnCore.* Regressionstests
```

Weitere Schalter: `-NoBuild`, `-NoVideo`, `-NoClips`, `-Fps 30`, `-Width/-Height`, `-TimeoutMinutes`, `-EnginePath`, `-FfmpegPath`.
Das Skript baut (außer mit `-NoBuild`), prüft das Inventar, startet das Spiel als **eigenen Prozess** (`UnrealEditor.exe -game`, Fenster 1920x1080 bei 0,0), nimmt die Client-Fläche des Fensters mit FFmpeg (gdigrab, NVENC H.264, Rückfall libx264) auf, wartet mit Zeitlimit und erzeugt den Bericht. Es beendet nie fremde Prozesse; läuft ein Editor mit dem Projekt (Live Coding blockiert den Build), bricht es mit einer Meldung ab. Während der Aufnahme darf das Spielfenster nicht verdeckt werden (es wird für die Aufnahme nach oben gelegt).

Ergebnis: `Unreal/SupernaturalProt/Saved/GameplayQA/<Zeitstempel>/` (Pfad des letzten Laufs: `Saved/GameplayQA/latest.txt`)

| Datei | Inhalt |
|---|---|
| `report.md` | Bericht: Unreal-Version, Git-Commit (+ "uncommitted"), Karte, Datum, Zählung, Fehler mit Position/Geschwindigkeit/Videostelle, NOT_IMPLEMENTED, FPS/Framezeiten/Speicher, Pfade |
| `results.json` | dieselben Daten maschinenlesbar |
| `records.jsonl` | eine Zeile je Prüfung, sofort geschrieben (überlebt Absturz/Timeout) |
| `gameplay.mp4` | Aufnahme des echten Spielfensters, Einblendung des laufenden Tests oben links und des letzten Ergebnisses |
| `clips/` | 10-Sekunden-Ausschnitte um jede fehlgeschlagene Prüfung |
| `shots/` | Screenshot je FAIL/BLOCKED (Spielansicht mit Overlay) |
| `automation/index.json` | Unreal-eigener Testbericht, `game.log` Spiellog, `run_info.json` Laufdaten |

Vergleich und Datenprüfung: `spn_qa_report.py` vergleicht jeden Lauf automatisch mit dem neuesten früheren Lauf im selben Ordner (Abschnitt "Compared with the previous run": Regressionen, behoben, weiterhin fehlschlagend, verschwundene und neue Checks; `--compare <Ordner>` / `--no-compare`). Ungültige Statuswerte werden BLOCKED, doppelte Checks, fehlendes Video/Clips/`records.jsonl` stehen unter "Problems with this run's data".

Lauf-Urteil und Traces: `INCOMPLETE` (Exit 2) bei Timeout, fehlenden Suiten oder fehlender `records.jsonl`. `traces.jsonl` enthält Messreihen (Turns, Jump) ohne Status; `spn_trace_tools.py` wertet sie aus (Links/Rechts-Symmetrie der Sprint-Wende, Sprung-Klassifikation).

Nachweisklassen (M2A3): (1) Core-Tests auf Fedora (`ctest`), (2) statische Integrationsprüfungen (`test_spn_unreal_static.py`), (3) braucht Unreal-Build, (4) braucht Unreal-Laufzeit. Eine Zeile aus Klasse 1-2 sagt nie, dass Klasse 3-4 bestanden ist. `env_scenarios` der angereicherten Karte stehen auf `PENDING_UNREAL_ADAPTER`.

Videostelle: `epoch` der Prüfung minus Startzeit von FFmpeg (+0,5 s Anlaufzeit); typische Abweichung unter 1 s. Der aktuell laufende Test steht zusätzlich als Text im Bild.

## Status

`PASS` Erfolgskriterium erfüllt; `FAIL` Kriterium verletzt; `BLOCKED` Prüfung konnte nicht zu Ende laufen (steckt fest, Zeitüberschreitung); `NOT_IMPLEMENTED` Funktion existiert nicht (Warnung, nie PASS); `NOT_TESTED` nicht ausgeführt oder nicht automatisierbar (eine Testgruppe ohne jeden Datensatz zählt als NOT_TESTED). Exit-Code des Skripts: 1 bei FAIL/BLOCKED.

## Eingabepfad (wie ein Mensch)

Die `SpnQA.*`-Tests laufen im echten Spiel (`-game`, Renderer an) mit dem echten Spielercharakter. Tasten gehen als Tastaturereignisse durch `APlayerController::InputKey` (W/A/S/D, Shift, Leertaste, V, **E**) und damit durch Enhanced Input, Mapping, `DoInteract`, den Interaktionstrace und `ASpnTestDoor::SpnUse`. Nur das Zielen (Kamerarichtung) wird direkt gesetzt. `-nullrhi` wurde ausprobiert und verliert Tastenereignisse unzuverlässig, deshalb rendern alle Suiten (Quick mit kleinem Fenster ohne Video). Hindernisse der Sprungtests sind zur Laufzeit erzeugte Blöcke, nie gespeichert.

## Türinteraktion (Standardtaste E)

Existiert seit M1: `E` (`DefaultInteractKey` am Charakter) -> `USpnInteractionComponent::TryInteract` (Kugeltrace 250 cm ab den Augen, Radius 15 cm, Blickrichtung = Steuerrotation) -> `ASpnTestDoor::SpnUse` (Weltzustand, Schwenk 180 °/s). Neu in dieser Sitzung: eine Bildschirmzeile **"[E] Open door" / "[E] Close door"** (Debug-Text, PLATZHALTER für ein UI-Widget) über `ISpnInteractable::SpnPromptText`; vorher gab es keinerlei Hinweis, welche Taste Türen bedient. Gemessene Reichweite: 30 bis 200 cm Abstand bei Kamerawinkel +10 bis -20 ° trifft immer, ab 240 cm (Kapselrand) nie; bei -40 ° ab 200 cm nicht (`SpnQA.DoorReach`).

## Neue Tests / Inventar erweitern

* Neue Prüfungen: `IMPLEMENT_SIMPLE_AUTOMATION_TEST(..., "SpnQA.<Gruppe>", ...)` in `Source/SupernaturalProt/Spn/Tests/SpnQa*Automation.cpp`, Ergebnisse mit `SpnQa::Check(...)` (`SpnQaRecorder.h`). Die Gruppe in `run_gameplay_qa.ps1` (`$groups`, `$expected`) eintragen.
* **Inventar:** `Unreal/SupernaturalProt/Tools/qa_inventory.json` listet jedes Gameplay-System mit Klassen, Core-Headern, `Spn.*`-Konsolenbefehlen, `Default*Key`-Belegungen, Tests und einer ausdrücklichen Abdeckungslücke (`gap`). `Tools/check_qa_inventory.py --check` (läuft vor jedem QA-Lauf und als Python-Test) scheitert, wenn eine neue `UCLASS`/`UINTERFACE`, ein neuer Core-Header, Konsolenbefehl oder Tastenbindung fehlt, wenn ein Eintrag weder Test noch Lücke hat, wenn ein genannter Test nicht existiert oder wenn `Docs/Development/Gameplay-QA-Inventory.md` veraltet ist (`check_qa_inventory.py --write` erzeugt es neu). So kann ein neues Feature nicht ohne Test oder dokumentierte Lücke hinzukommen.
* Python-Tests der Werkzeuge: `Tools/test_check_qa_inventory.py`, `Tools/test_spn_qa_report.py` (zusammen mit den übrigen: `python -m unittest discover -p "test_*.py"` im Tools-Ordner, mit der Python-Version der Engine).

## Grenzen

* Core-Tests (`core:`-Einträge, CMake/CTest) laufen auf dem Fedora-Rechner, nicht in diesem Skript.
* Keine automatische Bildbewertung: Optik (Lücken, Beleuchtung) bleibt Sichtprüfung am Video.
* Die Videoaufnahme belastet dieselbe Maschine; die Performance-Zahlen im Bericht sind daher Untergrenzen. Läuft ein zweiter Bildschirm-Capture oder liegt ein Fenster über dem Spiel, ist das Video unbrauchbar (die Tests laufen trotzdem).
* FPS-/Framezeit-Schwellen und Beschleunigungs-/Bremsgrenzen sind Platzhalter.
