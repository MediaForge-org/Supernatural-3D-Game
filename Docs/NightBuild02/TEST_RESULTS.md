# Test results — NightBuild 02

Alle Zahlen stammen aus tatsächlichen Läufen auf Fedora (Intel Core Ultra 7 256V). Build-Verzeichnisse unter `/home/maxim/spn-build/` (das Repository liegt auf NTFS; `/tmp` ist tmpfs mit wenig Platz).

| Lauf | Konfiguration | Ergebnis |
|---|---|---|
| Baseline vor Änderungen | clang 22 Debug, `-Werror` | 492 CTest-Einträge: **491 bestanden, 1 fehlgeschlagen** (`unreal_stub_check`) — wie angekündigt |
| Nach Paket A | clang Debug | 492/492 bestanden |
| **Endstand** | clang 22 Debug, `-Werror`, `ctest` | **587/587 bestanden, 0 fehlgeschlagen** (584 Doctest-Fälle + `movement_test_map_layout` + `unreal_stub_check` + `spn_core_test_names`) |
| **Endstand** | clang 22 Debug + ASan + UBSan (`-fno-sanitize-recover=undefined`) | **587/587 bestanden** |
| **Endstand** | GCC Debug, `-Werror`, Doctest-Binary direkt | **584/584 Fälle, 1 560 284 Assertions bestanden**; `ctest` nicht separat gefahren (die 3 Zusatzeinträge sind compilerunabhängig und liefen unter clang) |
| Benchmarks | clang Release | `spn_core_bench_navigation` ausgeführt (siehe `MapNavigation/PERFORMANCE.md`); vorhandene Benchmarks (`Bench`, `BenchM2B1`) **nicht** ausgeführt |
| clang-tidy | nur neue `Core/src/*.cpp` | ausgeführt; 2 echte Befunde behoben, Rest Stil |

Neu hinzugekommen: **95 Doctest-Fälle** (584 − 489 vorher; die Teilsummen sind gezählt und ergeben genau 95):
* `MapGeometryTests.cpp` 17 (Geometrie, BoxGrid, Testregion, Zugang, Snapping, Wasser-Connectoren, Validierung, Merge)
* `RoutingTests.cpp` 28 (Szenarien, Fehlerfälle, Determinismus, große Koordinaten, Regionen-Merge, Skalierung)
* `NavigationSessionTests.cpp` 11
* `MapRenderTests.cpp` 11
* `NpcMemoryTests.cpp` 20
* `NpcSaveTests.cpp` 5
* `GroundSteeringTests.cpp` +3 (Framerate, Extremwerte, Fuzz)

## Was dabei nicht ausgeführt wurde
* **Nicht ausführbar:** Unreal-Build, Unreal-Automationstests (`SpnMapSubsystemAutomation`, `SpnNightBuild*Automation` …), MSVC, Windows-Savesystem-Prüfung, Python-/Windows-Tools unter `Tools/windows`.
* **Übersprungen:** `SaveKillTest`/`SaveProbe` als manuelle Programme; `RunAllChecks.sh` (vier Presets) — stattdessen eigener clang-/GCC-/ASan-Lauf in separaten Verzeichnissen.
* **Unter Windows unbestätigt:** alles, was Unreal betrifft, sowie die Haltbarkeit (fsync/MoveFileEx) des Savesystems.
