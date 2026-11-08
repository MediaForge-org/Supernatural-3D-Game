# Roadmap

Stand: 08.11.2026. Quelle der Phasen: `README.md` Abschnitt 27.1 und `CLAUDE.md` §4. Die Zeitangaben sind **Schätzungen, keine Termine**. Die Kennzeichnungen `[VS]`, `[EARLY]`, `[LATER]`, `[FUTURE/AI]` legen die Reihenfolge fest, sie streichen nichts.

## Phasennamen (Vorschlag des Entwicklers, mit der Roadmap abgeglichen)

| Neuer Name | Alter Name (README 27.1) | Ziel | Zielwerte | Status |
|---|---|---|---|---|
| `P0-FOUNDATION` | Phase 0 Core Technology | Kleine Integration der Kernsysteme, ein winziger Platzhalter in 2–4 Wochen | ca. 2–4 Monate | in Arbeit (siehe Abschnitt Phase 0) |
| `P1-FIRST-PLAYABLE` | neu, zwischen Phase 0 und Phase 1 | Erste zusammenhängende spielbare Erfahrung, etwa 30 Minuten | ca. 30 Minuten (N-03) | noch nicht begonnen |
| `P2-VERTICAL-SLICE` | Phase 1 Vertical Slice | Ein vollständiger übernatürlicher Fall (Ablauf in README Kapitel 3) | ca. 1–3 km², 30–60 Minuten | geplant |
| `P3-WORLD-BUILDOUT` | Phase 2 und Phase 3 | Mehrere Fälle, breiter vernetzte Systeme, erweiterte Welt | 5–10 km² und 3–5 Stunden (P2 alt); 25–50 km² und 10–20+ Stunden (P3 alt) | geplant |
| `P4-RELEASE` | Phase 4 („schrittweise Skalierung zur vollen Langzeitvision“), bestätigt als kommerzielle Veröffentlichung auf Steam (Entwicklerangabe 08.11.2026) | Auslieferungsfähiger Umfang | Veröffentlichungstermin, konkreter Releaseumfang und Releasekriterien **offen** | bestätigt als Entwicklungsphase |
| `POST-LAUNCH` | – | Langfristiger Planungsbereich nach der Auslieferung | keine Zusage für Erweiterungen oder Inhalte | bestätigt als Planungsbereich (08.11.2026) |

Abgleich und offene Punkte:
- Der Name `P1-FIRST-PLAYABLE` ist neu, weil die Roadmap bisher keine Stufe zwischen Phase 0 und dem Vertical Slice kennt. Er wird durch N-03 (30 Minuten als erster spielbarer Einstieg) begründet, ist aber eine Umgliederung, die bestätigt werden muss.
- P4-RELEASE umfasst die alte Phase 4 (Skalierung) und die kommerzielle Veröffentlichung auf Steam (Entwicklerangabe 08.11.2026). Termin, Umfang und Releasekriterien sind offen.
- `POST-LAUNCH` ist ein langfristiger Planungsbereich ohne Zusage (Entwicklerangabe 08.11.2026).

## Stand der Phase 0 (`P0-FOUNDATION`)

Quelle: README Abschnitt 27.2 und 27.3.

| Schritt | Inhalt | Status laut Quelle |
|---|---|---|
| 0.1 | C++20, CMake, Ninja, `StrongId`, `Result`, doctest, Presets | abgeschlossen (`Docs/Decisions/0001`) |
| 0.2 | WorldFacts: Store, Journal, Format v1 | abgeschlossen (`Docs/Decisions/0002`) |
| 0.3 | WorldFacts-Golden-Datei und Speicherrichtlinie (`Docs/Decisions/0004`); Interaktions-Inkremente 1–4 (`Docs/Proposals/0004`) | abgeschlossen, manueller Editor-Test durch Entwickler offen |
| 0.4 | Aussehensgrundlage (`Docs/Decisions/0005`) | abgeschlossen laut Entscheidung |
| Windows/Unreal | MSVC-Build, `SpnCore`, Unreal-Prototyp (`Docs/Decisions/0006`) | abgeschlossen, Windows-Build |
| 0.5 | Aufnehmbare Gegenstände, Inventar-Zähler (`Docs/Proposals/0007`) | Automationstests bestanden, manueller Editor-Test offen |
| M1 | Testkarte mit Rampe, Treppe, Wand, Sprint, Kamera (`Docs/Proposals/0003`, `0008`) | **Technical Acceptance PASS** (23.10.2026). Editor-PIE durch Entwickler nicht geprüft. Release `v0.1.0-alpha.1` vorbereitet, nicht veröffentlicht. |
| Weitere Phase-0-Systeme | Gegenstände, Ermittlung, Dialog, einfache NPCs, Gegnerverhalten, Fahren | **Reihenfolge nicht festgelegt** (README 27.3). Dialog, Beweise, Fahren und Mods sind im Code nicht vorhanden. |

Parallel-Stränge (Core-Arbeit aus Nachtläufen, siehe `Milestones/`): M2-A (Bewegung), M2-B1 bis B3 (Karte, Koordinaten, Inventar, Speichern), NightBuild02 (Routing, Navigation, NPC-Gedächtnis), NightBuild03 (Demo-Navigation), NightBuild04 (Platzhalter-Türregeln). Einzelheiten in `TASKS.md`.

## Nächster Schritt

`PROTOTYPE-CASE-01` ist als kleiner technischer Test genehmigt. Er ist **nicht automatisch der nächste Implementierungsauftrag**. Vorher: Editor-Abnahme der Phase-0-Pakete durch den Entwickler, damit die Basis feststeht. Ziel bleibt eine erste zusammenhängende, etwa 30-minütige spielbare Erfahrung (`P1-FIRST-PLAYABLE`).
