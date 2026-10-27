# M2-A.2 Overnight Progress

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
