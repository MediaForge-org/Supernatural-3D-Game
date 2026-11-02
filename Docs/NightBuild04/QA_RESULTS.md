# NightBuild 04 — QA-Ergebnisse (Windows 11, UE 5.8.4, RTX 3090, 2026-11-02)

| Ebene | Befehl | Ergebnis |
|---|---|---|
| Core (MSVC) | `ctest --preset msvc-ninja` | 600/600 (NB03: 588; +12 `DoorStatesTests`, 2 angepasst) |
| Unreal-Automation (-nullrhi) | `Automation RunTests SpnCore+SpnGame+SpnDiag` | 154/154 (NB03: 143; +11 `SpnGame.NightBuild04.*`) |
| Gameplay-QA vor den Änderungen (Phase 0) | `run_gameplay_qa.ps1 -Suite Full` | 219 PASS, 2 FAIL (`Jump.late_jump_ledge_40_run/45_jog`, bekannt M2A2), 3 NOT_IMPLEMENTED, 1 NOT_TESTED (Level-bedingt) |
| Gameplay-QA `SpnQA.HouseJourney` (gerendert, echte Tasten) | `-Suite Building` bei 30 / 60 / 144 fps | je 42/42 PASS |
| Voller `SpnQA`-Lauf nach den Änderungen | `-Suite Full` | 251 PASS, 12 FAIL: 2 bekannte Sprung-Fails + 10 × `Doors.*.prompt_shown` (Regression: Türtext jetzt lokalisiert, alter Test erwartete festen deutschen Text) → Test an `FSpnUiText` angepasst, danach `-Suite Doors` 89/89, `Traversal` 22/22, `MovementDemo` 27/27 |

Hinweis: Der volle `Full`-Lauf wurde nach der Test-Korrektur nicht komplett wiederholt, nur die betroffenen Suiten (Doors, Traversal, MovementDemo, HouseJourney).
Video des Rundgangs (nicht in Git): `Unreal/SupernaturalProt/Saved/GameplayQA/20261102-005451/gameplay.mp4` (1920x1080, 60 fps).
