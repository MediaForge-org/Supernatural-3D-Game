# M2-A.2 Overnight Progress

## Sitzung 2026-10-28 (Fedora, autonom, keine Commits, kein Push)
Startcommit `f59604a`. Umgebung: Fedora 44, Python 3.14.8, CMake 4.3.0, GCC 16.2.1 / Clang; Windows/Unreal 5.8.4 nicht verfügbar. `Planning/Master` nicht eingehängt (CLAUDE.md §11: keine Master-Anforderung übernommen; nur Test-/Generator-/Werkzeugfehler behoben, keine Gameplay-Entscheidung).

**Baseline (vor Änderungen, Fedora):** Python 177/177. CTest `fedora-gcc`: 276 von 277 bestanden; `unreal_stub_check` schlug fehl (Stub kannte `ClientUpdatePositionAfterServerUpdate` nicht, seit der Windows-NetworkSprint-Korrektur).

| # | Arbeit | Ergebnis |
|---|---|---|
| 1 | Stub-Check repariert (`stubcheck/include/CoreMinimal.h`) | CTest wieder grün; Defekt war der Stub, nicht der Spielcode |
| 2 | Tür TestHouse (`Doors.TestHouse.walk_through`) | Siehe unten: Ursache = Flügel nach innen + Treppenblock (Engstelle 100 cm breit, 108 cm lang). Korrektur im V2-Generator, Regressionstests, QA-Datenpunkt angepasst |
| 3 | Sprint-Wende 800 -> 461 | Messung validiert und Ursache modelliert (siehe unten); keine Bewegungsparameter geändert |
| 4 | Sprünge 50/60 cm, Spätsprünge | Quelltext-/Kinematikanalyse, **Engine-Ursache offen** (Windows-Messplan im Handoff); nichts geändert |
| 5 | QA-Werkzeug `spn_qa_report.py` | Vorlauf-Vergleich (Regressionen/behoben/verschwunden), ungültige Status/Duplikate, fehlende Artefakte; 4 neue Tests |

### 2 Tür TestHouse
* Gemessen (Windows): 59 cm zwischen Treppenblock (Stufe 17 bei y -9204, 28-cm-Tritt) und Türpfosten. Der **offene Flügel** (10 cm dick, Angel am Nordpfosten) verengt den Durchgang zusätzlich: effektiv 49 cm gegen die 84-cm-Kapsel.
* Der Generator enthielt bereits die 25-cm-Tritt-Korrektur (Treppe endet bündig mit der Südkante der Öffnung y -9255). Sie reicht **allein nicht robust**: Der Durchgang bleibt 100 cm breit (Öffnung 110 minus Flügeldicke 10 = 16 cm Spiel gesamt), und weil der Flügel nach innen (+90) neben dem Treppenblock steht, muss die Kapsel diese Linie 108 cm weit halten (alle anderen 9 Türen: 14 cm = Wanddicke).
* Neues Prüfmodell in `spn_map_validate.py`: `door_passage` (breitester Pfad, 4-cm-Raster, Flügel als Hindernis, Kapselradius 42) und `door_pinch` (Länge der Engstelle). Das Modell reproduziert beide Fehlerzustände (28-cm-Tritt: Spiel -19 cm; Flügel innen: 108 cm lang).
* Korrektur (klein, V2 only): `fix_v1_house_door` setzt `open_angle = -90` für `mv_house_door` (Flügel schwingt nach außen, wie die Distrikttüren). V1-Daten/-Hash unverändert. Neuer V2-Hash `4df2a460...`. Keine Kollisions-Sonderfälle, Treppenproportionen (17 x 18,5 cm Steigung, 25 cm Tritt) und Grundriss unverändert.
* QA-Test: Innenpunkt `mv_house_door` von x -450 auf -520 (Flügel liegt jetzt 100 cm außen; sonst wäre er für `close_from_inside` knapp außerhalb der 250-cm-Reichweite). Auf Unreal ungetestet.

### 3 Sprint-Wende
* Das 41-44-Grad-Rätsel: `SpnQA.Turns` misst den Beschleunigungswinkel im **Akteur-Frame** 30-100 ms nach dem Tastendruck. Die Kapsel dreht mit 500 °/s und hat dann bis zu 50° gedreht: gemessen wird der **Restwinkel**, nicht der Eingabewinkel (der war 90°). Messung gültig, Beschriftung irreführend.
* Core-Modell der Engine-Formel (`CalcVelocity`: `v -= (v - dir*|v|) * min(dt*Friction,1)`, Friction 8): Der Betrag wird dabei **kumulativ** kleiner (nicht nur auf 0,707). Modell: Minimum 4,65 m/s (58 %), 95 % nach 1,02 s. **Gemessen: 461-464 cm/s, ca. 1,02 s.** Das Verlieren der Sprint-Gangart (Sustain-Winkel gilt nur über 6,75 m/s) kostet nur ca. 0,2 m/s; Ursache ist die Reibungsformel. Test `the engine's friction blend alone explains ...` in `GroundSteeringTests.cpp`.
* Links/Rechts-Asymmetrie (Sprint endet nur links) ist vom Modell **nicht** erklärt (Modell ist spiegelsymmetrisch) und bleibt offen; Vermutung Messzeitpunkt/Rotationsdetail, nur mit Frame-Trace auf Windows klärbar.
* Konsequenz: Die Anforderung >= 90 % ist mit der Engine-Reibung nicht erreichbar; sie setzt das nicht verdrahtete Lenkmodell (`GroundSteering`, Patchvorschlag §1) voraus. Das ist eine Gameplay-Entscheidung (Momentum vs. Responsivität). Nichts geändert.

### 4 Sprünge (offen, Engine-Ursache)
* Reine Kinematik (Apex 45 cm) kann Überqueren von 50/60 cm nicht erklären; dazu passt: Die Kapsel hat eine **runde Unterseite** (Radius 42). Ragt eine Kante nur wenige cm über den tiefsten Kapselpunkt (Apex/Stufenhöhe), ist der Kontaktnormalenwinkel flach genug für "begehbar" (Z >= 0,71 gilt bis ca. 12 cm Überhöhung) und die Kapsel rastet ein und steigt weiter. Das erklärt "Sprung + Anstieg ca. 45 + 25 cm" und das Gehen über 25-30-cm-Kanten; die gemessene Abhängigkeit vom `MaxStepHeight` (Limit 10 -> 20 cm überquert, 20 -> 25, 25 -> 30) passt nicht sauber zu einer festen Überhöhung, daher **Hypothese, nicht bestätigt**.
* Prüfplan und Entscheidungsoptionen im Handoff-Dokument; Optionen A/B/C aus `M2A2-Patch-Proposals.md` §3 unverändert offen. Kein automatisches Klettern/Mantling eingebaut.

### Dateien
`Tests/Core/GroundSteeringTests.cpp`, `Unreal/SupernaturalProt/Tools/{spn_district_layout,spn_map_validate,spn_qa_report}.py`, `test_spn_district_layout.py`, `test_spn_map_tools.py`, `test_spn_qa_report.py`, `stubcheck/include/CoreMinimal.h`, `Source/.../Tests/SpnQaDoorsAutomation.cpp` (Datenpunkt), Docs (diese Datei, `M2A2-Next-Windows-Integration.md`, Patch-Proposals-Nachtrag).

### Endstand Fedora (siehe auch Handoff-Dokument)
CTest `fedora-gcc`, `fedora-clang`, `fedora-gcc-asan-ubsan`, `fedora-clang-asan-ubsan`: je 279/279. Python 183/183 (`unittest discover Tools "test_*.py"`). `RunToolChecks.sh` und `check_qa_inventory.py --check` ohne Fehler. `git diff --check` sauber.

### Nächster Schritt
Windows: `M2A2-Next-Windows-Integration.md` abarbeiten (V2-Karte neu erzeugen, Build, `SpnQA.Doors`, Messläufe Sprung/Stufe).

---

# Frühere Sitzungen (Stand vor dieser Nacht)

Zweite Sitzung (Phase 2) auf Fedora. Keine Commits. Zählung: CTest 262/262, Python 106/106 (Stand Ende dieser Sitzung).

| Bereich | Stand |
|---|---|
| A Gebäude | Implementiert und getestet: 6 Templates (cottage, house, office, warehouse, workshop, garage), Raumzahl 2-4, Flur-Grundriss, Treppenabsatz, Räume als Daten, Rollen/Validierung/Export getrennt (`spn_building_roles.py`). 144 Varianten (6 Templates x 3 Seeds x 8) bestehen die Prüfungen. Unreal-Test ausstehend. |
| B Karte v2 | Implementiert und getestet (Daten); Editor-Skript nie ausgeführt. Ownership-Tags (`SPN_GEN`), `--adopt-legacy`, kein Speichern bei Fehler, `--instancing` (Plan 1156 -> 531 Actors, ISM-API unbestätigt, Fallback je Prim). Snapshot-Hashes v1/v2. Welt-Erreichbarkeit per 100-cm-Flood-Fill. |
| C Gestaltung | Teilweise: Büro hinzugefügt, Abstand zu v1-Teststationen geprüft; Straßen/Kurve unverändert aus Phase 1. |
| D Sprung | Core `JumpModel` (Kinematik) + Tests; `USpnMovementDiagnostics` (neu, nicht kompiliert); Hypothese und Optionen in `M2A2-Patch-Proposals.md`. Keine Korrektur angewendet. |
| E Traversal | Core vertieft: Sicherheitsregeln (`ClassifyTraversalChecked`), Abbruch/Fehler/Exit-State, Cooldown. Keine Bewegung/Animation. |
| F Movement | Steering-Tests 0/45/90/135/180, Start, Sprintabbruch; Patchvorschlag, nicht angewendet. |
| G Animation/Kamera | Locomotion unverändert; Core `CameraArm` (Regeln + Tests); keine Unreal-Kamera geändert. |
| H Interaktion | Core `InteractionReach` (Reichweite/Blickwinkel/Sicht/Akteurzustand) + Tests; M1-Registry unverändert. |
| I NPC | Core `Npc` (Zustände, Bewegungsabsicht, Wahrnehmung) + Tests, exhaustive Übergangsprüfung. |
| K Performance | Generator: v2 18 ms (vorher 56), Gebäudeprüfung ca. 3,5x schneller; C++-Micro-Benchmarks (`spn_core_bench`, Debug). |

## Phase 3 (laufend, Fedora, keine Commits)
- Master-Konsultation (CLAUDE.md §11): `Planning/Master` ist auf dieser Maschine nicht eingehängt (Verzeichnis leer, `master-search.sh` ohne Treffer). Nicht möglich -> nur Platzhalter-/Testumgebung und Regelkerne geändert; keine Master-Anforderung übernommen.
- ECHTER FEHLER GEFUNDEN UND BEHOBEN: Die Treppe der zweigeschossigen Häuser (Phase 2) war am Fuß eingemauert (Fuß an der Südwand, Halle nur 20 cm breiter als die Treppe); der alte Flood-Fill ignorierte die Stufen. Treppe jetzt mit freiem Fuß im Süden und 2-m-Podest im Norden; neue Prüfung `stair_foot_blocked`, Obergeschoss-Podest/-Flur als Räume.
- Gebäudeprüfung neu: strukturierte Fehlercodes, Support-/Schwebe-Prüfung, Wand-Flächenbilanz (keine Lücken), Fensterkonflikte/Slivers, Raumverbindungsgraph, `building_report`. `validate_params` ist das einzige Machbarkeits-Gate (akzeptiert => generiert und besteht alle Prüfungen; Eckfall-Sweep als Test). Räume haben jetzt per Konstruktion >= 2,5 m.
- Export v2 (`spn_building_export.py`): Schema-Version, Slots, Materialrollen, Yaw-Platzierung 0/90/180/270, Validierung, v1-Schlüssel erhalten.
- Karte: `spn_map_validate.py` (Layout-Preflight + Erreichbarkeitsgitter), 13 benannte Routen, Plot-/Sackgassen-Tests; Birkenweg-Wendehammer mitten in der Straße entfernt.
- Editor-Skript: Preflight vor jeder Änderung, `rebuild_plan` (create/keep/abort/rebuild), Verifikation VOR dem Speichern, Instanz-Fallback zerstört halbfertigen Actor vor dem Fallback (keine doppelte Geometrie). Alles gegen Fake-`unreal` getestet.
- JumpModel: `SimulateFaceJump` (Kantenkontakt, Engine-Antwort vs Ledge-Assist) + Empfindlichkeitstest; Bewertung der Optionen A/B/C in `M2A2-Patch-Proposals.md` §3. Ergebnis: Modell reicht nicht, um "springt abrupt auf das Hindernis" von "fällt zurück" zu trennen; entscheidend ist der von der Engine geforderte Vorwärtsweg nach der Kante (Messung auf Windows).
- Core: `Vec.h` (eine Vektordefinition statt drei), **Property-Tests fanden zwei echte Fehler** (`ResolveGait` gab bei korruptem Enum ein ungültiges Gait zurück; `EffectiveSpeed` mit NaN-Skalierung lieferte NaN) -> behoben. Traversal: Breite/Seitenfreiheit/Landebreite/bewegtes Hindernis/ungültiger Untergrund.
- Tools: `spn_log_tools.py` (Generator-Verdikt, Sprung-Auswertung), `test_spn_unreal_static.py` (statischer Check der Diagnose-Komponente), `Tools/RunToolChecks.sh`, `Tools/windows/run_spn_checks.ps1` (UNGETESTET), `Docs/Development/M2A2-Windows-Test-Pack.md`.
- Sanitizer: Clang ASan+UBSan und GCC: alle Tests grün.
- Performance: v2-Layout 19 ms; Gebäudeprüfung 3-4x schneller (Phase 2); Routen-Prüfung 1,1 s -> 0,14 s (Komponenten-Labeling statt 13 Floods); Export 1,3 ms (ohne) / 40 ms (mit Prüfungen).
