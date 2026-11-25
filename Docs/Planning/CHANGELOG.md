# Änderungsprotokoll der Planungskonsolidierung

## Paket MASTER-MIGRATION-19 (10.11.2026): Restliche Tages-Deltas, PROJECT_STATE 83, Blöcke 8–9, Prompt-Archiv, Textpakete

- **A, Tages-Deltas:** alle 9 Status- und Vergleichsberichte gelesen; begründet ausgeschlossen (Entwicklerberichte oder Vergleiche, deren Inhalte in `Docs/` belegt sind). Nachweis `daily-deltas-changelogs.md`.
- **B, PROJECT_STATE 83:** Gesamtphilosophie ergänzt (`Spielvision` §2.1, 🧭). Vorbehalt aufgehoben; Quelle formal abgeschlossen.
- **C, Blöcke 8–9:** alle ~90 Regeln einer Fundstelle in `Docs/` zugeordnet; sieben unsichere Punkte im Wortlaut geprüft. Keine fehlende Regel. Nachweis `embedded-klassifikation.md`.
- **D, Prompt-Archiv:** 866 Inhalte normalisiert: außer Titel und Baustein-Gerüst nur 31 Anforderungsbullets und Track-Kontexte; Version 0.1 und 0.5 nur durch den Entwurfsvermerk verschieden. Zuordnung `prompt-archiv-zuordnung.csv` (527 Spielthemen, 321 technische Aufgaben, 18 Track-Kontexte). Vorschläge als 💡 (Case-Tag-, Waffen-, Ballistik-, Verletzungs-Debugger; Modding-Pflichten; Wiederverletzung).
- **E, Textpakete:** 143 eindeutige Textdateien gescannt; GIS-Methodenregeln (fiktive Web-Projektion, Renderquelle DEM) als 🧭 in `World/24` §12 übernommen; Geometrie begründet ausgeschlossen.
- **Neue Anforderungen (Stufe D/🧭/💡):** Gesamtphilosophie (`Spielvision` §2.1), Wiederverletzung (`Kampfsystem/13`, 💡), Steam-Unabhängigkeit und Entitlements je Produkt (`Spielvision` §16, 🧭), Modding-Pflichten (`Gameplay-Systeme` §15, 💡), Debugger (`Produktions-Referenzen` §12, 💡), GIS-Methodik (`World/24` §12, 🧭).
- **Zählung:** 148 von 2.440 abgeschlossen (6,07 %); 37 begründet ausgeschlossen; 2.255 offen. Korrektur: Offen-Zahl bisher ohne Abzug der ausgeschlossenen Metadaten angegeben.
- Keine Master-Dateien verändert; keine Commits, Pushes oder Tags; kein C++ oder Unreal; `99_RECONSTRUCTION_SOURCES` nicht gelesen. `verify_planning.py`: 167 Referenzen, 0 defekt.

## Paket MASTER-MIGRATION-18 (09.11.2026): PROJECT_STATE, Tages-Deltas, Embedded-Entwürfe, Textpakete

- **PROJECT_STATE_2026-08-31_COMPACT:** alle 85 Abschnitte gelesen und geprüft; formal abgeschlossen mit Vorbehalt (Abschnitt 83 thematisch; `Nachweis/project-state-compact.md`). Neu in `Docs/`: Modding-API-Familien und Manifest (`Gameplay-Systeme` §15), Distribution (Disc, Updates, Blu-ray, `Spielvision` §16), Sekundärphysik (`Gameplay-Systeme` §9), seltene Einzeltiere (`Spielwelt` §9.2a), DLC-Grundsatz (`Spielvision`), Arbeitspaket-Hierarchie und -Felder (`Produktions-Referenzen` §12, 💡). Missionszustände als 💡 Vorschlag (`Spielvision` §6).
- **Tages-Deltas:** alle Entscheidungs-Ledger (16.–19.10.) gegen `Docs/` geprüft; Vorschläge (C++-Notizen, Phase-0.4-Erscheinungsbild) und Berichte (Status, Vergleich, Herkunft) getrennt; Prozessdateien ausgeschlossen (`Nachweis/daily-deltas-changelogs.md`).
- **Embedded:** 1.732 Prompt-Entwürfe in zwei Versionen zu je 866 Titeln, hash-dedupliziert; 31 verschiedene Anforderungsbullets extrahiert und am Text geprüft (Spielregeln belegt, übrige technische Vorgaben); SPN_Prompt_Pack 12 von 22 gelesen, Inhalte gegen Docs geprüft. Die Case-Tag-Idee als 💡 in `Produktions-Referenzen` §12.
- **_SOURCE_ARCHIVE:** 143 Textdateien klassifiziert (WORLD_700-Iterationspakete, technische Geometrie- und Prüfdaten); keine Spielanforderung; Geometrie gesperrt; R05A-Archivierung (03.09.) in `World/19` ergänzt.
- **Zählung:** 125 von 2.440 eindeutigen Inhalten inhaltlich abgeschlossen (5,12 %), 2.315 offen. Zuwachs: 6 CHANGELOGs, 4 Entscheidungs-Ledger, PROJECT_STATE, zwei Spezifikationen vom 17.10. (Kommunikation, Datenschutz).
- Keine Master-Dateien verändert; keine Commits, Pushes oder Tags; kein C++ oder Unreal; `99_RECONSTRUCTION_SOURCES` nicht gelesen. `verify_planning.py`: 167 Referenzen, 0 defekt.

## Paket MASTER-MIGRATION-17 (09.11.2026): 01_CURRENT vollständig, Abschnitte 13 bis 28, Zählkorrektur

- **01_CURRENT_CANONICAL_STATE** (57 von 57 Dateien) vollständig gelesen und gegen `Docs/` geprüft. Nachweis: `master-zuordnung-01-current.md`, Abschnitt J. Keine fehlende Anforderung gefunden; offene Konflikte K-21 (Hölle-Zeitverhältnis) und K-28 (Bunker/HQ) bleiben offen.
- **Zählkorrektur:** Eindeutige Inhalte 2.440 (nicht 2.444), redundante Kopien 2.429 (nicht 2.425). Ursache: 14 Dateien über 50 MB sind nur 10 verschiedene Inhalte; die frühere Zählung hatte sie als 14 Einheiten gezählt. `_SOURCE_ARCHIVE` 323 eindeutige (nicht 327).
- **Messung statt Schätzung:** Inhaltlich abgeschlossen 112 von 2.440 eindeutigen Inhalten (4,59 %; Stand nach Aufnahme der aktuellen Fassungen von PROJECT_DECISIONS und OPEN_DECISIONS); 28 Metadaten- und Prozessdateien begründet ausgeschlossen (nicht als migriert gezählt). Offen 2.328.
- **Kleine Gruppen:** `00_START_HERE` (3) abgeglichen; `03_EXTERNAL_LIBRARY_REGISTRY` (18), `99_MANIFESTS` (3), Wurzeldateien (5), `06` (2) als Metadaten gelesen. Widerspruch dokumentiert: Die Registry führt `R05A_phase_B3_true_highfreq_regional_master_24k.png` als „not embedded“, die Hash-Liste zeigt die Datei aber unter `_SOURCE_ARCHIVE/World History B`. Registry nicht geändert.
- **PROJECT_DECISIONS Abschnitte 13 bis 28** im Wortlaut geprüft (Stand 28 von 58). Ergänzt wurden: Modding ausdrücklich unterstützt und ohne Reverse Engineering (§15), gezieltes Prüfen bei belegtem Versteckverhalten (§7), Nicht-Besitzer-Sicht auf Guides (§16), Path Tracing und Upscaling (§15 Rendering), Entwicklerwerkzeuge, Pflichttests und Prompt-Governance (`Produktions-Referenzen-und-Prompts.md` §12). Belegt ohne Ergänzung: Ballistik, Nahkampf, Fahrzeuge, Fortschritt (OFFEN), Begleiter, Audio, Testphilosophie, Genre, Scrum-Prozess.
- **Konflikt dokumentiert, nicht aufgelöst:** Xbox-artige Controller-Belegung (älteres Konzept) gegen DualSense vom 15.10.2026. Die spätere Entwicklerentscheidung hat Vorrang; das ältere Konzept bleibt als Richtung erhalten (VEH-DEC-ST-004).
- Keine Master-Dateien verändert; keine Commits, Pushes oder Tags; kein C++ oder Unreal. `verify_planning.py`: 167 Referenzen, 0 defekt.
- **PROJECT_DECISIONS** ist mit Abschnitt 58 vollständig im Wortlaut erfasst (Stand 58 von 58). Die Protokolltabelle enthält 58 eindeutige Abschnitte; Abschnitt 8 hat eine überholte Vorfassung, die als Verlauf bleibt.
- **OPEN_DECISIONS** (124 Punkte, 17 Gruppen) je Gruppe gegen Zielstellen geprüft: `Nachweis/entscheidungsabgleich-open-decisions.md`. Neu in `OPEN_QUESTIONS.md`: Q-OD-42 bis Q-OD-53 (Punkte ohne offene Zielstelle). Eine Dublette wurde vor Abschluss entfernt.
- **Ergänzt** in `Gameplay-Systeme.md`: Modding ohne Reverse Engineering (§15), gezieltes Prüfen bei belegtem Versteckverhalten (§7), Path Tracing und Upscaling (Rendering), Sekundärphysik (§9). In `Spielvision.md`: Sicht Nicht-Besitzer (§16), Missionszustände als 💡 Vorschlag (§6). In `Produktions-Referenzen-und-Prompts.md`: §10 Barks, Laden nur bei Bedarf, Explorer-Mindestfunktionen; §11 Blender-Automation, Testpawns, Freigabeaktionen; §12 Tests, Werkzeuge, Prompt-Governance, Arbeitspaket-Schätzungen als 💡.
- **Statusdifferenzen dokumentiert:** Trophäengruppen (`PROJECT_DECISIONS` §47 entschieden, `Entscheidungsstatus` offen); Creator-Wirtschaft (Wunschrichtung, Details offen). Keine davon aufgelöst.
- `PROJECT_STATE_2026-08-31_COMPACT.md` teilweise geprüft; `CURRENT_COVERAGE.md` gelesen (Prozentangaben nicht übernommen).
- **02_EMBEDDED klassifiziert** (1.860 verschiedene Inhalte; 1.732 Prompt-Entwürfe in zwei Versionen, keine Anforderungsquelle). Byte-identische Dubletten belegt: Projektzusammenfassung (01 alt = PROJECT_STATE), Blöcke 1–7 und WORLD_1500-Anforderungen (01 alt = 06). Nachweis `embedded-klassifikation.md`.
- **_SOURCE_ARCHIVE klassifiziert:** 323 verschiedene Inhalte (179 Binär, 128 Text nur dort, Rest Kopien). Binär begründet ausgeschlossen; Text sind WORLD_700-Iterationspakete ohne Anforderungsstatus. Archivierung der R05A-Pilote (03.09.) in `World/19` ergänzt.
- **Abgleich Vorstufe 03.09.:** Börse, Krypto, 12–18 Buchmacher (scharf, halbscharf, weich) und Wettbörse (Back/Lay) sind in `Gameplay-Systeme` §13 belegt (zunächst übersehen, weil der Text deutsche Begriffe nutzt). Offen bleibt das Preis- und Liquiditätsmodell (Q-OD-54). Reihenfolge der Reichsplanung (🧭) in `Spielwelt` §15 ergänzt.
- **99_DAILY_DELTAS:** Die sechs CHANGELOGs (14.–19.10.) sind je Entscheidung gegen `Docs/` geprüft (`Nachweis/daily-deltas-changelogs.md`). Keine fehlende bestätigte Entscheidung gefunden. Canon-, Spec- und Design-Dokumente der Tage bleiben teilweise (Zeile-für-Zeile-Prüfung offen).

## Paket MASTER-MIGRATION-15 (09.11.2026): Inventurvalidierung und Abschnitt 12

- Inventur validiert: 4.869 Dateien, 18 überschneidungsfreie Gruppen, 2.444 globale eindeutige Inhalte, 2.425 Kopien. Registerzählungen für 99_CANON_AUDITS und 99_DAILY_DELTAS korrigiert; gruppenübergreifende Duplikate (8+8 nach _SOURCE_ARCHIVE) ergänzt.
- PROJECT_DECISIONS Abschnitt 12 (Gunplay) im Wortlaut geprüft, ohne Ergänzung. Gesamt: 12 von 58.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-14 (09.11.2026): PROJECT_DECISIONS, Abschnitte 8, 10, 11

- Wortlaut geprüft: Abschnitte 8, 10, 11. Insgesamt 11 von 58 Abschnitten.
- Konflikt offen: Checkpoint-Schleife (`Kampfsystem/14` Punkt 4 gegen `Spielvision` §14.2 und `COMBAT-DEC-BW-001`).
- StateTree-Status vereinheitlicht in `Game/Kreaturen/10` §11 (Technikrahmen 📋, Aufteilung 💡).
- Ergänzt: Munitionsanzeige binär (`Game/Kampfsystem/02`).
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-13 (09.11.2026): PROJECT_DECISIONS im Wortlaut (Teil)

- Wortlaut-Protokoll: 9 von 58 Abschnitten geprüft (1–7, 9 vollständig; 8 teilweise).
- Ergänzt: `Game/Spielwelt-und-Untergrund.md` (Fall- und Story-Gebäude handgebaut).
- Konflikt dokumentiert: StateTree-Regel (PROJECT_DECISIONS) gegen 💡-Status in `Game/Kreaturen/10`.
- PROJECT_DECISIONS.md nicht formal abgenommen. Nächster Abschnitt: 10 (Zeile 116, Tod und Scheitern, OPEN).
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-12 (09.11.2026): PROJECT_DECISIONS erfasst

- Neu: `Nachweis/entscheidungsabgleich-project-decisions.md` (58 Abschnitte, Prüfstufe je Abschnitt). Keine Abschnitte formal abgeschlossen.
- Ergänzt: `Development/Produktions-Referenzen-und-Prompts.md` §10 und §11.
- Offen: OPEN_DECISIONS, PROJECT_DECISIONS Zeilen 1–400 im Wortlaut, zwei weitere reference-Dateien.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-10 (09.11.2026): Changelog 24.08., OPEN_DECISIONS, PROJECT_DECISIONS (Teil)

- Changelog 24.08. formal verarbeitet; offen: Pre-1.0-Builds (Q-BUILD-1), Platzhalterstimmen (Q-VOICE-1).
- `OPEN_DECISIONS.md`: 41 Punkte ohne Treffer in `Planning/OPEN_QUESTIONS.md` als Q-OD-01 bis Q-OD-41 übertragen. 83 Treffer nur automatisch abgeglichen.
- `PROJECT_DECISIONS.md`: Zeilen 1–260 stichprobenartig geprüft; Zeilen 260–707 offen.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-09 (09.11.2026): Wortlaut-Prüfung 31.08., Entscheidungen 23./24.08.

- Prüfstufen im Abgleich 31.08. eindeutig: 33 Zeilen „Text geprüft“, 26 Zeilen „Wortlaut“, 1 informell.
- Changelog 31.08. formal verarbeitet; offene Entscheidungen dokumentiert.
- Ergänzt: `Game/Gameplay-Systeme.md` (Kosmetik ohne Spielkraft), `Game/Fahrzeuge-und-Verkehr/04` (Vertrautheit, Luftlenkung, Bergung), `Game/Kreaturen/10` (Monsterreaktion, Ernte), `Game/Kampfsystem/13` (Insassenverletzung), `Game/Kampfsystem/11` (Rauchen), `Game/Ermittlung/27` (NPC-Aufnahmen, Behördenzugriff), `Game/NPCs/04` (Kommunikation ohne Malus), `Game/Ermittlung/14` (Statusvermerk Eskalation).
- Neu: `Docs/Development/Produktions-Referenzen-und-Prompts.md` §9 (Audio-Qualität und Audio-QA, Entscheidungen 23.08.).
- Neu in `Planning/OPEN_QUESTIONS.md`: Q-BUILD-1 (Pre-1.0-Builds), Q-VOICE-1 (Platzhalterstimmen).
- Changelog 24.08.: nur teilweise geprüft. `PROJECT_DECISIONS.md` und `OPEN_DECISIONS.md`: nicht geprüft.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-08 (09.11.2026): Abgleich DECISION_CHANGELOG 08-31

- Neu: `Nachweis/entscheidungsabgleich-2026-08-31.md` (55 Abschnitte, 60 Prüfzeilen; Prüfstufe je Zeile).
- Ergänzt: `Game/Spielvision.md` (Geografie vor Story §5.2; Missionszustände; Online-Richtung und Modus-Grenzen); `Game/Spielwelt-und-Untergrund.md` §9.2a (Kadaverzustände, Aasfresser-Hinweis); `Game/NPCs/14` (Alterung nach Story- und Weltzeit).
- Nicht abgenommen: 26 Stichwort-Zeilen; DECISION_CHANGELOG 08-23/08-24, PROJECT_DECISIONS.md und OPEN_DECISIONS.md noch nicht geprüft.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-07 (09.11.2026): Governance, Prompt-Tracks, Plattform

- Neu: `Docs/Development/Produktions-Referenzen-und-Prompts.md` §8 (Plattform-Grundlagen Track 17, plattformneutral, Glyphen offen).
- Fortschrittsübersicht um Governance-, Prompt- und Track-Kontext-Stand erweitert. Governance-Prozessregeln bewusst nicht übernommen.
- Track-Kontexte der Prompt-Bibliothek (18 Dateien) gegen `Docs/` abgeglichen. Einzelprompts (866) und Changelogs 08-23/08-24 nicht vollständig geprüft.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-06 (09.11.2026): Inventare, Duplikatprüfung, Fortschrittsübersicht

- Neu: `Nachweis/fortschritt-master-migration.md` (drei Stufen getrennt: inventarisiert, geprüft, migriert).
- Neu: `Nachweis/inventar-04-history.csv` (402 Dateien, SHA-256, Duplikatgruppen).
- `04_HISTORY`: 402 Dateien, 44 eindeutige Inhalte, alle byte-identisch mit `01_CURRENT`. Kein Eigeninhalt.
- `99_RECONSTRUCTION_SOURCES`: 60 Dateien, alle eindeutig. Inventar bewusst nicht in `Docs/` (private Überschriften); nicht geprüft.
- `02_EMBEDDED_BASE_ARCHIVES`: 3.689 Dateien, 1.860 eindeutige Inhalte (heuristische Einordnung). Planungsbacklog (26 Dateien) vollständig gelesen.
- Neu: `Docs/Game/Spielwelt-und-Untergrund.md` §9.2a (Wildtier-Studium, Verarbeitung, Forensik; Track 37).
- Ergänzt: `Docs/Game/Gameplay-Systeme.md` (Minderjährige NPCs als aktuelle Darstellungsrichtung mit offener Prüfung; Darstellungsprofile Voll/Reduziert/Minimal; Track 44).
- Neu: `Docs/Development/Produktions-Referenzen-und-Prompts.md` §7 (Blender-Pipeline Track 29; Werkzeuganforderungen 23, 27, 28, 30, 31).
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-05 (09.11.2026): Register H abgeschlossen

- Neu: `Docs/Game/World/12` §3 um Autonomiegrenzen (U35-Richtung, Berechtigungsmodell 💡, Infrastruktur-Isolation, Ausfall-Szenarien, Verbote).
- Neu: `Docs/Development/Produktions-Referenzen-und-Prompts.md` §6 (KI-3D-Pipeline als Recherche, Herstellerangaben nicht unabhängig geprüft).
- Neu: `Docs/Planning/OPEN_QUESTIONS.md` Q-ASSET-1/2 und Q-PUB-1/2.
- Ergänzt: `Docs/Decisions/0004` §6 (berichtete Risiken Phase 0.3, nicht unabhängig geprüft).
- Register `Nachweis/master-zuordnung-01-current.md` Abschnitt H: alle Delta-Dokumente mit Zielstelle oder Prozessstatus.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-04 (09.11.2026): offene Tagesdeltas übertragen

- Neu: `Docs/Game/World/12` §3 um KI-Anerkennung nach U34 (sechs Dimensionen, offene Fragen, Freitext-Regel), Simulationsgrenzen (bestätigter Rahmen, Architektur 💡, Abnahmebedingungen, Verbotsliste) und Datenschutz/Ausfallsicherheit (bestätigte Grundsätze, Vorschläge, Prüffälle; U37 offen) ergänzt.
- Neu: `Docs/Game/World/17` §6 Abnahme-Gates der Karten-Spezifikation (visuelle Freigabe zuerst, Topologie, Render-Ehrlichkeit, Paketintegrität, Statustransparenz, Nicht-Ziele).
- Register `Nachweis/master-zuordnung-01-current.md` Abschnitt H aktualisiert.
- Keine Master-Dateien verändert; keine Commits.

## Paket MASTER-MIGRATION-02 (08.11.2026): Quellen abschließen, Verzeichnisse 02, 99 und 04 klassifizieren

- Neu ergänzt: `Docs/Game/World/24-WORLD700-GIS-Referenz.md` §11 (OWP-LCS-1-Koordinatenformel, Zoomstufen, Kennungen, Genauigkeitsgrenzen, Verteilung, Phase-A-Abnahmeschranken, Stil-Vorschlag `MAP_RENDER_STYLE_V1` nicht gesperrt, Wiederherstellungsstand, Ebenen-Manifest Iteration 6). Widerspruch Phase A (Master PROPOSED vs. Register APPROVED_LOCKED) vermerkt; K-19 bleibt offen.
- Neu ergänzt: `Docs/Game/World/12` §3 (Kommunikation nach U36: Absicht/Darstellung, kein Pflichtimplantat, offene U37-Liste; 💡/offen markiert).
- Neu: `Docs/Development/Produktions-Referenzen-und-Prompts.md` (aus Paket 1, Abschnitt 2 Richtung; System offen).
- Korrigiert: `Docs/Game/Kampfsystem/27` COMBAT-DEC-VL-012 (Status Master-Vorgabe; K-22 offen).
- Geprüft: CA5 §4 (Verletzung, Körper, Alltag; Versorgung; Sandbox-Prinzip) punktweise belegt; Verletzungsarten und Krankenhausstufe in `Gameplay-Systeme` §8 und `Kampfsystem/13`.
- Geprüft, ohne Änderung: Modding Block 5 (Startsynchronisation und Manifest-Abbildung bereits in `Gameplay-Systeme` Abschnitt „Mod-Laden“). Eine Dublette aus dieser Sitzung wurde entfernt.
- Geprüft: `99_CANON_AUDITS` (Entscheidungsregister, Konflikte, Duplikate per Byte-Vergleich). Keine neuen Anforderungen; Untergrundeinträge durch N-02 historisch.
- Geprüft: neun Tagesdelta-Dokumente mit bestätigten Nutzerentscheidungen (Karten, Bahn, Fahrzeuge, KI, Kommunikation). Übrige Deltas offen (siehe Register H).
- Klassifiziert: `04_HISTORY`, `99_RECONSTRUCTION_SOURCES` (privater Inhalt, nicht übertragen), `02_EMBEDDED_BASE_ARCHIVES`, `_SOURCE_*` (siehe Register I).
- Nicht entschieden: K-19, K-21, K-22, K-28, K-29, K-33, N-01 (Weltgröße).
- Keine Änderungen an Master-Dateien; keine Commits.

## Paket MASTER-MIGRATION-01 (08.11.2026): Inhaltsmigration 01_CURRENT, Abschnitte CA5 §2–§6

- Quelle: `01_CURRENT_CANONICAL_STATE` (57 Dateien, Inventar im Register `Nachweis/master-zuordnung-01-current.md`).
- Neu: `Docs/Planning/Nachweis/master-zuordnung-01-current.md` (Zuordnung je Quelle, Übertragungs- und Prüfstatus, Abschnitt E mit Zielstellen).
- Neu: `Docs/Development/Produktions-Referenzen-und-Prompts.md` (Richtung aus CURRENT_CANON_2026-09-16 und VOICE_PRODUCTION_…; System offen).
- Ergänzt: `Docs/Game/World/26` §5 (historische Ebenen statt Jahreskarten; Freie-Energie-Passage als historisch markiert).
- Ergänzt: `Docs/Game/Spielvision.md` (WWII-Ära und Original-IP-Vorgabe, CA5 §3).
- Geprüft, ohne Änderung: `Docs/Game/Gameplay-Systeme.md` Modding Block 5 (Anbieterunabhängigkeit, Manifest-Abbildung und Startsynchronisation waren bereits im Abschnitt „Mod-Laden“ enthalten).
- Ergänzt: `Docs/Game/Kreaturen/21` Q-K32 (Progenitor-/Erste-Art-Konzept offen; bisher nicht in Docs).
- Korrigiert: `Docs/Game/Kampfsystem/27` COMBAT-DEC-VL-012 war als BESCHLOSSEN geführt, obwohl K-22 (Amputation) offen ist; Statusvermerk entsprechend angepasst.
- Nicht entschieden: K-22 (Amputation), K-21 (Zeitverhältnis Hölle, Docs-Bestätigung vs. Konfliktregister), K-33 (Geografie bei Zeitreisen), K-28 (Bunker als Feldbasis vs. Tesla-Aufenthalt laut MS 10-13), Weltgröße (N-01).
- Keine Änderungen an Master-Dateien; keine Commits.
- Prüfung: `verify_planning.py` 165 Referenzen, 0 defekt.

## Paket 1 (08.11.2026): Grundstruktur und bestätigte Festlegungen

**Geändert bzw. neu:**
- `Docs/Planning/README.md` (neu): Einstieg, Statusklassen, Vorrang, Regeln für Sitzungen.
- `Docs/Planning/DECISIONS.md` (neu): Register mit N-01 bis N-03 (November), T-01 bis T-08, W-01 bis W-07, S-01 bis S-14, V-01 bis V-13, X-01 bis X-04.
- `Docs/Planning/SOURCES.md` (neu): Quellenindex mit Lesestatus.
- `Docs/Planning/CONFLICTS.md` (neu): K-01 bis K-09 plus Liste der noch nicht bewerteten Konflikte.
- `Docs/Planning/OPEN_QUESTIONS.md` (neu): offene Fragen aus `Entscheidungsstatus.md` §5, ergänzt um die November-Fragen.
- `Docs/Planning/progress.json` (neu, vom Prüfskript erzeugt): Paketstatus und nächste Pakete.
- `Tools/planning/verify_planning.py` (neu): prüft alle Quellverweise auf Existenz und Zeilenbereich.

**Prüfung:**
- `verify_planning.py`: 75 Verweise geprüft, 0 defekt.
- Stichprobe von 6 Verweisen mit Inhaltsvergleich: alle stimmen.
- Das Skript prüft nicht, ob der Inhalt jeder Zeile vollständig übernommen ist. Das geschieht durch Lesen in den Folgepaketen.

**Nicht geändert:** Alle Originaldokumente in `Docs/Game/`, `Docs/Proposals/`, `Docs/Decisions/` und der Master. Keine Commits.

**Noch nicht erledigt in Paket 1:** Verweis in `CLAUDE.md` (siehe unten).

## Offene Pakete

- Paket 2: `Docs/Game/Entscheidungsstatus.md` Abschnitte 6.5–7 lesen und übernehmen.
- Paket 3: `Docs/Game/Gameplay-Systeme.md`, Abschnitte 2–18.
- Paket 4: `Docs/Game/Spielvision.md`, Abschnitte 1–17.
- Paket 5: `Docs/Game/Spielwelt-und-Untergrund.md`, vollständig.
- Paket 6: `01_CURRENT_CANONICAL_STATE` (57 Dateien) aus dem Master. Duplikate per Hash, nur Kanon-Kopie lesen.
- Paket 7: `Docs/Game/Adult/`, `Ermittlung/`, `Kampfsystem/`, `Kreaturen/`, `NPCs/`, `World/`, `Fahrzeuge-und-Verkehr/`.
- Paket 8: `Docs/Decisions/`, `Docs/Proposals/` (Konfliktprüfung gegen Spielentscheidungen).
- Paket 9: `99_DAILY_DELTAS` und `04_HISTORY` nur nach Bedarf, als Verweis, nicht vollständig übernehmen.

## Paket 2 (08.11.2026): Kanon und Entscheidungsstatus

**Gelesen:**
- `Docs/Game/Entscheidungsstatus.md` vollständig (einschließlich §6.5 bis §7).
- Alle 57 kanonischen Dateien aus `01_CURRENT_CANONICAL_STATE` (Pfade und Hashes in `SOURCES.md`). Keine Duplikate mit identischem Hash innerhalb dieses Ordners.

**Geändert:**
- `Docs/Planning/DECISIONS.md`: neuer Abschnitt F mit C-01 bis C-57 (Kanon-Entscheidungen mit Quelle und Zeile).
- `Docs/Planning/CONFLICTS.md`: K-10 bis K-16.
- `Docs/Planning/OPEN_QUESTIONS.md`: Abschnitt „0.3 Blockierend für Untergrund-Inhalte“ und Fragen 3 bis 16.
- `Docs/Planning/SOURCES.md`: Kanon-Zeile auf „vollständig“.
- `Tools/planning/verify_planning.py`: erkennt jetzt auch `Planning/` und `Tools/` als Verweisziele.

**Prüfung:**
- `verify_planning.py`: 146 Verweise, 0 defekt.
- Zeilenbezüge: Fehler gefunden (einige Zeilen waren geschätzt) und korrigiert, 21 Stellen. Danach Stichprobe von 10 Verweisen mit Inhaltsvergleich: alle stimmen.

**Ergebnis zu den Docs/Game-Dateien:** Die Stichwortsuche zeigt, dass die Themen des Kanons in `Docs/Game` bereits sehr breit vorkommen (z. B. Zeitreise 44 Dateien, Begleiter 78, Stealth 33, Forensik 38, Smartphone 32). Ob die Details vollständig übernommen sind, ist damit **nicht** bewiesen. Der Abgleich Zeile für Zeile steht noch aus.

**Noch nicht erledigt in Paket 2 (wird in Paket 3 nachgeholt):**
- Direkte Ergänzungen in den thematischen `Docs/Game`-Dateien (z. B. Purgatorium-Ökologie, Himmel-Erinnerungsdomänen, Realm-Persistenz). Die Stichwortsuche findet „Fegefeuer“ und „Hölle“, aber für Purgatorium auf Englisch und die Persistenzregel für Reiche keine Treffer.
- Markierung der betroffenen Stellen in `Docs/Game/Spielwelt-und-Untergrund.md` (Zeile 346–349) und `Entscheidungsstatus.md` (Zeile 102). Das darf erst nach Bestätigung der N-02-Formulierung geschehen (Vorrang: Entwickler, aber Textfassung fehlt).
- Die restlichen 221 Dateien in `Docs/Game` (Schritte 3 bis 5 aus dem Auftrag).
- `Docs/Game/World/20-Entscheidungsregister.md` existiert bereits als Register im Repo. Abgleich mit `DECISIONS.md` steht aus.

**Blockierend:** K-12 (N-02 gegen die Untergrund-Lore). Die übrigen Pakete laufen weiter.

## Offene Pakete

- Paket 3: Abgleich `Docs/Game/World/20-Entscheidungsregister.md` mit `DECISIONS.md`. Dann die Datei-für-Datei-Prüfung von `Docs/Game/World/`.
- Paket 4: `Docs/Game/Gameplay-Systeme.md` und `Spielvision.md`, Abschnitte und Details.
- Paket 5: `Docs/Game/Spielwelt-und-Untergrund.md` (nach Bestätigung von N-02) und `Docs/Game/Ermittlung/`.
- Paket 6: `Docs/Game/Adult/`, `Kampfsystem/`, `Kreaturen/`, `NPCs/`, `Fahrzeuge-und-Verkehr/`.
- Paket 7: Tagesdeltas (`99_DAILY_DELTAS`) gezielt, nur soweit Kanon-Addenda sie referenzieren.
- Paket 8: `04_HISTORY` nach Bedarf, nicht vollständig.

## Paket 3 (08.11.2026): bestehendes Weltregister

**Gelesen:** `Docs/Game/World/20-Entscheidungsregister.md`, Abschnitte 1 (Lesehilfe), 2 (GE), 9 (UG). Abschnitte 3 bis 8 und 10 bis 18 nicht gelesen.

**Geändert:** `Docs/Planning/DECISIONS.md` Abschnitt G mit der Zuordnung der Kennungen zu N-01 und N-02. Das Register in `Docs/Game/World/` ist unverändert.

**Ergebnis:**
- Das Register enthält zwei Einträge im direkten Widerspruch zu N-02: W-UG-001 (eigenständige Untergrundzivilisation) und W-UG-002 (Erbauer). Beide sind als 📋 gekennzeichnet. Vor einer Änderung ist die Textfassung von N-02 zu bestätigen (K-12).
- Die Bunker-Einträge W-UG-014 bis W-UG-020 hängen von derselben Frage ab.
- Die Weltgrößen-Einträge W-GE-006 bis W-GE-014 sind bereits historisch markiert (🕰️ oder 🧭). Die Zuordnung zu N-01 ist dokumentiert.

**Prüfung:** `verify_planning.py` zu Paket 3 ausführen (siehe unten).

**Noch offen:** Abschnitte 3 bis 8 und 10 bis 18 des Weltregisters.

## Projektsteuerung (08.11.2026): Bestandsaufnahme und Vorschlag

**Gelesen (nur Überschriften und Roadmap):** README Abschnitt 27 (Phasenmodell, Schritte, nächste Schritte), `CLAUDE.md` §4, Dateinamen in `Docs/Proposals`, `Docs/Decisions`, `Docs/Development`, `Docs/M2A*`, `Docs/M2B*`, `Docs/NightBuild01`–`04`.

**Geändert:**
- `Docs/Planning/NAMING_PROPOSAL.md` (neu): Vorschlag, nicht angewendet.
- `Docs/Planning/OPEN_QUESTIONS.md`: Abschnitt 17.
- `Docs/Planning/CONFLICTS.md`: K-17 (M1–M6 doppelt), K-18 (Phase 0.3 doppelt).

**Nicht geändert:** `TASKS.md`, `ROADMAP.md`, `CANON.md`, `DEPENDENCIES.md` und `Milestones/` existieren noch nicht. Sie werden erst angelegt, wenn das Namenssystem bestätigt ist.

**Offen:** N-02 (blockiert die Untergrund-Inhalte, K-12), Namenssystem (Abschnitt 17).

## Nachweis Gameplay (08.11.2026)

Datei: `Docs/Planning/Nachweis/gameplay.md` (222 Zeilen, je Anforderung eine Zeile).

- Status: vollständig integriert 1, bereits vorhanden 213, teilweise 2, blockiert 6, verworfen 0, noch nicht integriert 0.
- Blockiert: GP-GORE-001 (K-22), GP-BEZIEHUNG-001 (K-23/K-10), SV-HQ-002 und SV-HQ-003 (K-28), SV-TOD-003 (K-26), SV-TOD-005 (K-24).
- Teilweise: GP-BEZIEHUNG-002, GP-MODDING-006.
- Korrektur: Rund 20 Anforderungen waren in der Ausgangsliste als „fehlt“ markiert, der Inhalt steht aber in GS/SV. Die Lücke-Spalte bezieht sich auf den Master.
- Offen in Docs/Game: 15 Korrekturen (nicht umgesetzt, Liste im Nachweis).
- **Einschränkung:** Master-Stellen wurden nur punktuell geprüft (CA5, B9, CA6, MB5). Übrige Master-Angaben sind als „–“ markiert. „Bereits vorhanden“ bedeutet: im Docs-Text nachgeprüft, nicht im Master.

## Nachweis Systeme (08.11.2026)

Datei: `Docs/Planning/Nachweis/systeme.md` (151 Zeilen).

- Status: bereits vorhanden 48, vollständig integriert 81, teilweise 14, blockiert 5, noch nicht integriert 3, verworfen 0.
- Maschinelle Prüfung: 1.447 zitierte Fundstellen auf die genannte ID geprüft, 0 Abweichungen. Master nur für 12 Zeilen mit konkretem Zitat geprüft.
- Blockaden: K-26 (SY-ERM-019, SY-KAMPF-005, SY-KAMPF-006), K-27 (SY-KAMPF-026), K-28 (SY-WELT-002).
- Noch nicht integriert: SY-WELT-001, SY-WELT-004, SY-WELT-005.
- Umgesetzt: N-02-Markierung „historisch“ in NPCs/26 (2 Zeilen: N-SI-024 und eine N-FA-Zeile). In Fahrzeuge-und-Verkehr/28 gab es keine passende Zeile, dort nichts geändert. Text unverändert.
- Noch offen: Verweise in NPCs/10, 15, 16, 18, 22 (Umwidmung auf historisch, SY-WELT-005).

## Nachweis Adult (08.11.2026)

Datei: `Docs/Planning/Nachweis/adult.md` (263 Zeilen). Alle 41 Dateien gelesen.
- Klassen: A 21 (Beziehung/Romantik, gesperrt bis K-23), B 32 (optionale explizite Inhalte), C 17 (Produktion/Recht/Vertrieb), D 28 (Architektur), E 18 (offen/widersprüchlich), F 15 (historisch/verworfen). Insgesamt 131 Tabellenzeilen.
- 11 Widersprüche, 18 offene Entscheidungen für den Entwickler.
- Nichts in `Docs/Game/Adult/` geändert.

## Nachweis Welt (08.11.2026)

Datei: `Docs/Planning/Nachweis/welt.md`. Alle 120 WE-IDs geprüft.
- Status: vollständig integriert 74, vorhanden 17, teilweise 18, blockiert 9, verworfen 1, noch nicht integriert 1.
- Umgesetzt: Weltgröße in `Entscheidungsstatus.md` §2.2 als überholt markiert.
- Offen: N-02-Markierung in `World/09` §7 und `World/10` §1, §7; N-01-Text fehlt in Docs/Game (WE-GROESSE-001); Master-Nachtrag für PE-E1022 und N-01 bis N-03 fehlt im Master.

## Korrekturen Welt (08.11.2026)

- N-02 in `World/09` §7 (Unterirdisches Netz) und `World/10` §1 (Überblick) und §7 (Größe und Bevölkerung) als „historisch (N-02)“ markiert. Der Text bleibt unverändert.
- Noch offen: N-01 (Weltgröße offen) in den Welt-Dokumenten als Hinweis, WE-GROESSE-001. Keine neue Größe festgelegt.
- Noch offen: Die 34 teilweise integrierten Anforderungen, die vier noch nicht integrierten, die 131 Adult-Anforderungen als Einordnung und die Tagesdeltas.

## CONSOLIDATION-01: Offene Welt- und NPC-Korrekturen (08.11.2026)

**Geprüft (Quelle und Ziel gelesen):** Spielwelt §4 und Tabelle Weltgrößen, `World/20` W-GE-013, `Entscheidungsstatus.md` §2.2, `Fahrzeuge-und-Verkehr/01` Zeile 202, `Ermittlung/20` E-GH-005, `NPCs/26` N-BH-018, `NPCs/10` (Zeilen 228, 255), `NPCs/15` (288), `NPCs/16` (392), `NPCs/18` (280), `NPCs/22` (309).

**Geänderte Dateien:**
- `Docs/Game/Spielwelt-und-Untergrund.md`: 2.200-km²-Preset und Festland-Zeile als überholt (K-20 / N-01) markiert; Metropolis-Bezug markiert.
- `Docs/Game/World/20-Entscheidungsregister.md`: W-GE-013 als überholt markiert.
- `Docs/Game/Entscheidungsstatus.md`: §2.2 Weltgröße als überholt markiert (früher).
- `Docs/Game/World/09`, `World/10`: N-02 (früher).
- `Docs/Game/NPCs/10, 15, 16, 18, 22`: Untergrund-Punkte als historisch (N-02) markiert; Restfrage Bunker und Höhlen bleibt bei Q-N25 offen.
- `Docs/Game/NPCs/26`: N-BH-018 Zusatz (Labore bleiben möglich).
- `Docs/Game/Ermittlung/20`: E-GH-005 Zusatz (Labore bleiben möglich).
- `Docs/Game/Fahrzeuge-und-Verkehr/01`: Zeile 202 als historisch (N-02).

**Nachweise aktualisiert:** WE-GROESSE-001 (vollständig), SY-WELT-004 (vollständig), SY-WELT-005 (vollständig), SY-WELT-001 (teilweise).

**Keine neue Weltgröße festgelegt.** Keine Akzeptanzkriterien erfunden.

**Offen (nicht Teil dieses Pakets):** Die übrigen Teilweise-Anforderungen, Adult-Einordnung, Tagesdeltas.

## NIGHT-CONSOLIDATION-02, Teilpaket 1 (08.11.2026)

- GP-MODDING-006: Backup- und Risikoregel in `Docs/Game/Gameplay-Systeme.md` (Abschnitt Mods) eingearbeitet. Rekonstruiert, Bestätigung K-14 offen. Status vollständig integriert.
- GP-BEZIEHUNG-002: blockiert durch K-23 (der fehlende Teil betrifft Romantik).
- Restliche teilweise Anforderungen: welt.md (17 offen), systeme.md (14 offen).

## CONSOLIDATION-02: Weltanforderungen abschließen (08.11.2026)

**Geprüft:** Die teilweise integrierten Anforderungen aus `Docs/Planning/Nachweis/welt.md`. Die Tabelle nennt 18 Zeilen mit diesem Status (der Auftrag sprach von 17). Je Anforderung wurden Quelle und Ziel gelesen. Keine Spielentscheidung, keine Code- und Git-Änderung.

**Ergebnis:** 16 Anforderungen vollständig integriert (WE-GROESSE-009, WE-STADT-012, WE-STADT-014, WE-NATUR-007, WE-WASSER-002, WE-UNTERGRUND-001, -002, -003, WE-WIRT-001, WE-MOTOR-004, WE-NPC-004, WE-TECH-007, WE-REICH-009, WE-REICH-010, WE-AKT-004, WE-HQ-002). WE-ZEIT-004 als Konflikt K-33 offen. WE-ZEIT-005 blockiert, weil die Quelle „Paket 11, Frage 129“ im Register nicht nachweisbar ist.

**Geänderte Dateien:**
- `Docs/Game/Entscheidungsstatus.md`: N-03 bei Phase 1 (§3); größte Untergrundstadt in §6.4 als historisch (N-02).
- `Docs/Game/Spielwelt-und-Untergrund.md`: Naturräume nach N-02 erlaubt (§14.1, nicht historisch); K-33 bei Geografie (§7); Hunter-HQ-Lage als Hypothese (§4).
- `Docs/Game/World/20-Entscheidungsregister.md`: W-TE-014 (N-03); W-UG-011 (Naturräume erlaubt); W-ST-035 (Hypothese); W-RE-006 (K-33).
- `Docs/Game/World/02`, `04` (§8 Artenzahl und Studium), `05` (§1 W-WA-007, §5 Toleranzen offen), `06` (§8 Tattoos, Stripclub-Zuordnung, Rechenzentren), `08` (§7.1 Mengen 09.09. historisch), `11` (§2 Städte historisch), `12` (§1 historisch, §6 optionale Systeme), `15` (§5 Wunsch Bergstrecke, OF-58), `16` (§7 Save-Größe K13), `18` (§1 IP-neutral, §9 Save-Größe K13), `26` (§3 Leviathane, §4 DLC, §5 Konflikt K-33 und Kausalaussage als offen).
- `Docs/Planning/CONFLICTS.md`: K-33 neu (Geografie bei Zeitreisen, Master gegen Docs).
- `Docs/Planning/Nachweis/welt.md`: Status und Details der 18 Zeilen, Zählung (91 / 17 / 0 / 10 / 1 / 1), Korrekturliste.

**Korrekturen am Nachweis:** Mehrere Befunde in `welt.md` waren veraltet oder falsch. World/10 §1, §7 und World/09 §7 waren bereits als historisch markiert. Die Studium-Regel steht in SW §9.2, nicht „nirgends“. World/12 §6 enthielt Börse und Krypto schon. Die Zählung in der Zusammenfassung (74 / 1 „noch nicht integriert“) stimmte nicht mit den Zeilen überein.

**Offen (Korrekturliste 1, 2, 5, 6, 7, 20, 23–27):** gehört zu anderen Anforderungen oder braucht eine Entwicklerentscheidung. Dazu gehören K-33 (Geografie bei Zeitreisen) und die Quelle für die Kausalaussage in World/26 §5 (WE-ZEIT-005).

**Prüfung:** `python3 Tools/planning/verify_planning.py`: 153 Referenzen, 0 defekt.

## CONSOLIDATION-02, Teil 2: Systemanforderungen (08.11.2026)

**Geprüft:** Die teilweise integrierten Anforderungen aus `Docs/Planning/Nachweis/systeme.md`. Die Tabelle nennt 15 Zeilen (Auftrag: 14; eine Zeile trug einen Zusatztext und wurde deshalb separat gezählt). Quelle und Ziel je Anforderung gelesen. Keine Spielentscheidung, keine Code- und Git-Änderung.

**Ergebnis:** 9 vollständig integriert (SY-ERM-007, SY-ERM-016, SY-NPC-011, SY-NPC-023, SY-NPC-027, SY-FAHR-027, SY-FAHR-028, SY-WELT-001, SY-WELT-006). 5 blockiert: SY-ERM-011 (Teil, K-27), SY-KAMPF-011 (K-22), SY-KREA-011 (Zeitverhältnis, K-21), SY-KREA-012 (K-21, K-26), SY-NPC-013 (K-26). 1 Konflikt offen: SY-KREA-027 (K-34, neu).

**Geänderte Dateien:**
- `Docs/Game/Ermittlung/23-Testkatalog-und-Abnahmekriterien.md`: T-B16 (qualitative Wissensanzeige), T-Z13 (Zeugen wissen nur Gesehenes; keine garantierte Wahrheit).
- `Docs/Game/Fahrzeuge-und-Verkehr/28-Entscheidungsregister.md`: VEH-DEC-GP-020 („Fahren ist kein Zwang“, Fa02 P10; zuerst als GP-014 vergeben, Kollision mit Datenprofil-Entscheidung, korrigiert); Verweis KL-015 auf World/09 §3 (Schienen-Betriebsregeln); N-01-Hinweis bei PF-013.
- `Docs/Game/NPCs/26-Entscheidungsregister.md`: N-BH-004 (kein Sternesystem, K-E8 offen); N-SI-016 (Zahl hängt an N-01).
- `Docs/Planning/DECISIONS.md`: N-02, Master-Historie UC/US/UA/AI als Verweis (Master unverändert).
- `Docs/Planning/CONFLICTS.md`: K-34 (DLC-Aussage Lore und Inhalte).
- `Docs/Planning/Nachweis/systeme.md`: Status und Details der 15 Zeilen, Zählung (92 / 48 / 0 / 10 / 1), Korrekturabschnitt.

**Korrekturen am Nachweis:** Mehrere Befunde waren falsch. „Unbekannt“ steht in Er20 E-AR-012 und ist durch T-B13 getestet. Die N-02-Notiz in E-GH-005 war vorhanden. Die Untergrund-Markierungen in NPCs/10 und NPCs/22 waren gesetzt. Wiederbelebung steht in Kreaturen/02, /04, /06. Die Zeitverhältnis-Frage ist K-21 (nicht W-04). Die frühere Zählung summierte sich nicht auf 151.

**Offen:** K-21 (Zeitverhältnis Hölle), K-22 (Gliedmaßen-Gore), K-26 (Todesmodell), K-27 (Todesursache wichtiger NPCs), K-34 (DLC). Diese Punkte brauchen eine Entwicklerentscheidung.

**Prüfung:** `python3 Tools/planning/verify_planning.py`: 155 Referenzen, 0 defekt.

## CONSOLIDATION-ADULT: Adult- und Beziehungssysteme (08.11.2026)

**Geprüft:** 131 Anforderungen aus `Docs/Planning/Nachweis/adult.md` (Abschnitte A bis F). Maschinelle Prüfung aller 312 Zitierungen (Datei, Zeile, Kennung), Abgleich der Statuszeichen in den Quellzeilen, Handprüfung der Auffälligkeiten.

**Ergebnis:** Alle 131 Anforderungen sind in `Docs/Game/Adult/` oder `Docs/Game/Adult-System.md` belegt. Keine neue Übernahme in die Entscheidungsregister, weil keine dieser Anforderungen dort als bestätigte Entscheidung gilt. Konflikte und offene Entscheidungen bleiben offen: K-23 (Romantik im Basisspiel), K-10, K-31 (USER-LOCK). Nicht genehmigt und markiert: ADU-C-009 (Qwen), ADU-C-010 (Symbolik), ADU-E-01, ADU-E-02.

**Korrekturen:** 6 falsche oder leere Zeilenangaben (ADU-B-019, D-006, D-008, E-01, E-17, F-04) und 5 Statuszuordnungen (ADU-A-016, C-016, D-010, D-013, D-018) im Nachweis.

**Geänderte Dateien:** `Docs/Planning/Nachweis/adult.md` (Korrekturen, Abschnitt G), dieser Changelog-Eintrag. Keine Änderung an `Docs/Game/`.

## Prüfung der neuen Kennungen

| Kennung | Ergebnis | Bedeutung |
|---|---|---|
| K-33 | frei vor Eintrag; keine Kollision | Konflikt: Geografie bei Zeitreisen (Docs stabil, Master zählt Geografie zu versionierten Schichten). Offen. |
| K-34 | frei vor Eintrag; keine Kollision | Konflikt: DLC-Aussage (Lore und Inhalte) gegen „kein klassisches DLC“ (ältere Welt). Offen. |
| T-B16 | frei; keine Kollision | Test: Wissensanzeige ist qualitativ, nie Prozentzahl (E-AR-013). |
| T-Z13 | frei; keine Kollision | Test: NPCs wissen nur Gesehenes oder Gehörtes; Zeugenaussagen nicht garantiert wahr (E-ZE-001/002). |
| VEH-DEC-GP-014 | **Kollision** (bereits belegt: Datenprofile je Region, Fa02:101, Fa28:60) | Nicht verwendet. Der Eintrag „Fahren ist kein Zwang“ wurde als **VEH-DEC-GP-020** neu vergeben (frei). |

## CONSOLIDATION-DELTA: Übernahme der Tagesdeltas 14.–19.10.2026 (08.11.2026)

**Geprüft:** die sechs `CHANGELOG.md` vollständig; `docs/`-Dateien über Changelogs und Stichproben; Abgleich mit `Docs/Game/`, `TASKS.md`, `DECISIONS.md` (N-01, N-02). Nicht ausgeführt: `install.sh`, `tests/`, Manifeste; keine Änderung am Master-Archiv.

**Ergebnis:** Register `Docs/Planning/Nachweis/tagesdeltas.md` mit 44 Einzelpunkten. Die meisten sind bereits in den Docs belegt (U24–U36, Schiene, DualSense-Belegung, 2D-Atlas, Siedlungshierarchie, Hydrographie, Metropolis 110 km²). Überholt durch N-01/N-02: Bevölkerung und Stadtkonzept der Untergrundstadt (14.10.), 2.200 km² als Richtwert (18.10./19.10. teilweise). Nicht übernommen: Assistenten-Vorschläge, Tripo-Recherche, 30-km²-Beispiel. Offen: U37, V06, Untergrund-Zahlen, Zugnetz, Kartenabnahme.

**Neue Fragen in `OPEN_QUESTIONS.md`:** Q-DELTA-1 (Freitext-Regel fehlt in Planung), Q-DELTA-2 (2.200-km²-Messung nur für Kandidat), Q-DELTA-3 (HQ auf Vorschaubildern).

**Geänderte Dateien:** `Docs/Planning/Nachweis/tagesdeltas.md` (neu), `Docs/Planning/OPEN_QUESTIONS.md` (Abschnitt angehängt), dieser Changelog. Keine Änderung an `Docs/Game/` oder am Master.

**Prüfung:** `python3 Tools/planning/verify_planning.py`: 156 Referenzen, 0 defekt.

## CONSOLIDATION-REPORT: Freitext-Regel und Gesamtstand (08.11.2026)

- **Regel dokumentiert:** `Docs/Planning/PLANUNGSFRAGEN.md` (Freitextoption Pflicht, Freitext hat Vorrang, Kombinationen zulässig, keine eigenmächtige Konfliktauflösung). Eintrag P-01 in `DECISIONS.md`. Verweis in `README.md`. Q-DELTA-1 in `OPEN_QUESTIONS.md` als beantwortet markiert.
- **Gesamtstand:** `Docs/Planning/Nachweis/KONSOLIDIERUNGSBERICHT.md` mit Quellen, 14 Konflikten nach Entscheidungsbedarf, 39 maschinell gefundenen Stellen (8 echte Restarbeiten), Master-Lücken und 11 Restarbeiten.
- **Keine Änderungen** an `Docs/Game/`, `CLAUDE.md` oder am Master. Keine Commits, keine Pushes.
- **Prüfung:** `verify_planning.py`: 156 Referenzen, 0 defekt.

## CONSOLIDATION-FINAL-CLEANUP: Restarbeiten aus KONSOLIDIERUNGSBERICHT (08.11.2026)

**Erledigt (ohne neue Spielentscheidungen):**
- **R3:** Acht überholte Aussagen in `Docs/Game/` als historisch oder überholt markiert, Inhalt erhalten: `Entscheidungsstatus` (Untergrundstadt-Klärungsliste), `World/02` (2.200 km² kein Richtwert), `World/03` (Tabelle und Punkte 1 und 4 als Kandidatenmaß), `World/10` (Bahnhöfe, N-02), `World/20` (W-ST-002, N-01/K-20), `World/22` (OF-24, OF-31, N-02), `Fahrzeuge-und-Verkehr/27` (PF-013, N-01/K-20), `World/28` (Untergrundwerte, N-02).
- **R2 / Konsistenz K-12 und K-20:** `CONFLICTS.md` K-12 von „gelöst“ auf „teilweise gelöst“ (N-02 gilt; Geltung U24–U36 bleibt offen, K-29). K-20 Vorgehen auf „entschieden (N-01/K-20): 2.200 km² kein Richtwert; neue Größe offen“. Welt-Nachweis: fünf Zeilen von „blockiert (K-20)“ auf „blockiert durch N-01“, zwei Zeilen von „blockiert (K-12)“ auf „blockiert (K-29; N-02 gilt)“. Systeme: SY-WELT-006 entsprechend.
- **R5:** `SOURCES.md` mit datierter Aktualisierung (Abschnitt 0); alte Lesestatus bleiben als Historie. Falsche Zählungen der Master-Ordner korrigiert.
- **R6:** `TASKS.md`: fünf Statuswerte mit Zusatz auf das Vokabular gebracht (`abgeschlossen`), Scope in die Bemerkung verschoben; `DEC-UNDERGROUND-SCOPE` auf `bereit`. Neuer Abschnitt „Konsolidierung“ mit CONS-Kennungen (12 Einträge, stabile IDs).
- **R1 (teilweise):** `Nachweis/gameplay.md`: Zitierungen maschinell geprüft (233 Zitierungen, eine korrigiert: GP-BALLISTIK-001), Zählung korrigiert (213 / 7 / 2; die frühere Tabelle stimmte nicht), blockierte Zeilen gegen K-IDs geprüft. Nicht geprüft: inhaltliche Gegenprüfung aller 222 Zeilen.
- **CLAUDE.md:** Verweis auf `Docs/Planning/PLANUNGSFRAGEN.md` und Regel P-01 ergänzt (Projektanweisung, mit Freigabe).
- **Fortschritt:** `progress.json` aktualisiert (161 Referenzen, keine defekten, Restarbeiten getrennt nach entscheidungsabhängig und offen).

**Nicht erledigt, entscheidungsabhängig (übersprungen):** K-21 (Hölle-Zeitdehnung, PE-E1022-Widerspruch), K-29 (Geltung U24–U36), K-10/K-23/K-31, K-22, K-24, K-26, K-27, K-28, K-33, K-34, Q-DELTA-2/3, Größenfestlegung (N-01), Gameplay-Korrekturliste (15 Punkte mit Entwicklerfragen), Master-Rest (R9).

**Prüfung:** `python3 Tools/planning/verify_planning.py`: 161 Referenzen, 0 defekt.
**Keine** Änderungen an Spielcode, keine Commits, keine Pushes, keine Löschung von Anforderungen, keine Änderung historischer Quellen (Master, Chroniken, Versionsgeschichte).

## CONS-GAMEPLAY, Paket 1 (08.11.2026)

- Inhaltliche Gegenprüfung der Anforderungen 1–25 (GP-ERKUNDUNG bis GP-GORE-004) gegen Quelle und Ziel. Ergebnis in `Docs/Planning/Nachweis/gameplay-inhaltspruefung.md`: 24 übereinstimmend, 1 Abweichung der Ausgangsliste (GP-INTERAKTION-003, „Ruhe“), 1 blockiert (GP-GORE-001, K-22). Keine Dokumentkorrektur nötig.
- Prüfung: `python3 Tools/planning/verify_planning.py`.

## CONS-GAMEPLAY: Inhaltliche Gegenprüfung Gameplay (08.11.2026)

- **Geprüft:** 222 von 222 Anforderungen (GP und SV) gegen Ausgangstext, Quellzeile in `Gameplay-Systeme.md` bzw. `Spielvision.md` und Zielstelle. Ergebnisse je ID in `Docs/Planning/Nachweis/gameplay-inhaltspruefung.md`.
- **Ergebnis:** 215 übereinstimmend, 7 blockiert (K-22, K-23/K-10, K-24, K-26, K-28), 7 Ausgangsliste ungenau (Dokument korrekt).
- **Korrigiert:** `Gameplay-Systeme.md` GS:255 Querverweis auf die Forensikliste (SV:219); GS:349 Feldliste als Vorschlag (W-08); GS:86 Widerspruch W-10 vermerkt. `Spielvision.md` SV:184 Bunker als Feldbasis offen (K-28); SV:264 Formulierung auf Master-Wortlaut (`BLOCK_09_STATUS.md:5`); SV:352 K-25 vermerkt; SV:397 Zeilenzahlen vorläufig (W-05); SV:435 Reichsgrößen als Planungsbereich (N-01).
- **Register:** `gameplay.md` Zählung korrigiert (213 / 7 / 2); Zitat GS:116–118 korrigiert; Korrekturliste mit Status (9 erledigt, 3 Entwicklerentscheidungen).
- **Hinweis:** Während der Prüfung hat ein Zeileneinschub in GS (Forensik-Verweis) die Zeilennummern nach GS:255 verschoben; er wurde zurückgenommen und als Anhang an GS:255 umgesetzt. Die Zeilennummern entsprechen wieder dem Stand vor der Prüfung.
- **Nicht geöffnet:** Die Master-Stellen (CA5, CA6, MR9 usw.) wurden nicht erneut inhaltlich gegen die Dokumente geprüft; das ist der nächste Schritt.
- **Prüfung:** `python3 Tools/planning/verify_planning.py`.

## NIGHT-AUDIT-03: Master-Gegenprüfung, Prüfgrad, Trockenlauf, Scope-Audit (08.11.2026)

- **P1 Master:** 222 Gameplay-Anforderungen gegen 11 Masterdateien (CA5, CA6, CU8, B9, MB5, MR9, MS13, A15, OP, VP, CC) klassifiziert: 28 bestätigt, 52 teilweise, 116 nur konsolidiert, 19 nur Delta (Tier 2), 3 vom Master als offen markiert, 2 widersprüchlich (K-22, K-23), 2 Master nicht geöffnet. Ergebnis: `Nachweis/master-gegenpruefung-gameplay.md`.
- **Korrigiert:** `Spielvision.md` SV:243 ergänzt um das alte ~200-km²-Konzept (CA5:79). Die Vorprüfung hatte das Zitat für nicht belegt gehalten; das gilt nur für das Dokument, nicht für den Master.
- **Zitatfehler im Register erkannt, nicht geändert:** GP-BALLISTIK-002 (CA5:49 ist Forensik), GP-KOERPER-002 (CA5:51 ist Schlaf/Alkohol). Im Prüfprotokoll als „nur konsolidiert“ geführt.
- **P2 Prüfgrad:** `Nachweis/pruefgrad.md` (Stufen S0–S3, je Register). Inhaltliche Einzelprüfung der Welt-, Systeme- und Adult-Anforderungen steht aus; Adult: 6 Stichproben gegen Quelle geprüft.
- **P3 Trockenlauf:** `Nachweis/trockenlauf-aufgaben.md` für PROTOTYPE-DOOR, PROTOTYPE-TESTMAP, FOUNDATION-WORLDFACTS, P0-PLAYABLE-PLACEHOLDER, DEC-UNDERGROUND-SCOPE. Testzählungen statisch, nicht ausgeführt. Befund: PROTOTYPE-TESTMAP „abgeschlossen“ bei offener Editor-PIE (nicht geändert).
- **P4 Scope:** `Nachweis/scope-audit.md`. Core ohne Unreal-Includes (verifiziert); Vertical Slice ohne Abnahmekriterien; Restrisiko Master-Größen und U24–U36 in Entscheidungsstatus §2.3.
- **TASKS:** CONS-GAMEPLAY-MASTER auf „in Arbeit“; QA-PRUEFGRAD (in Arbeit), QA-TROCKENLAUF und QA-SCOPE (abgeschlossen) neu.
- **Keine** Spielentscheidungen, keine Spielcodeänderungen, keine Commits oder Pushes, keine Löschung historischer Quellen.

## CONSOLIDATION-READINESS: Entwicklungsvorbereitung (08.11.2026)

1. **Leseregel** in `README.md` (Regeln für Sitzungen) und `CLAUDE.md`: neuere bestätigte Entscheidungen und Supersession-Vermerke haben Vorrang vor älteren Masterquellen; historische Masterangaben sind keine aktuellen Anforderungen.
2. **Zitate korrigiert:** GP-BALLISTIK-002 und GP-KOERPER-002. Die ursprüngliche Masterquelle ist nicht auffindbar (Suche in `01_CURRENT_CANONICAL_STATE`: „Durchdringung“ und „Muskelgedächtnis“ ohne Treffer). Die Docs-Stelle ist die einzige Quelle. Vermerk in `gameplay.md`, `gameplay-inhaltspruefung.md`, `master-gegenpruefung-gameplay.md`.
3. **~200 km²** in `Spielvision.md` SV:243 ist als historisch, „keine aktuelle Weltgröße“ gekennzeichnet; die heutige Weltgröße ist offen (N-01).
4. **U24–U36:** Hinweis „Geltung nach N-02 offen (K-29)“ in `Entscheidungsstatus.md` §2.3 und in `World/12`. Keine Inhaltsentscheidung.
5. **PROTOTYPE-TESTMAP:** Titel und Bemerkung auf technische Abnahme begrenzt (PASS 23.10.2026, Entwicklerbericht, Proposal 0008 §13). Status bleibt „abgeschlossen“ für den technischen Umfang; die manuelle Editor-PIE-Prüfung ist offen und liegt bei `P0-ACCEPT-EDITOR`.
6. **SLICE-CASE-01:** belegte Abnahmekriterien K1–K12 in `Nachweis/vertical-slice-kriterien.md` (Quellen je Kriterium); offene Spezifikationspunkte O1–O6 ausgewiesen. Keine Zahl erfunden.
7. **Entwicklungsbereitschaft:** `Nachweis/entwicklungsbereitschaft.md` (sofort / Designentscheidung / PIE-Prüfung / kleinster Meilenstein `PROTOTYPE-CASE-01` als Vorschlag).

Keine Spielentscheidungen, keine Spielcodeänderungen, keine Commits oder Pushes. Prüfung: `verify_planning.py`, 161 Referenzen, 0 defekt.

## SPN-DEVELOPMENT-SESSION-01: Technische Integration und Prototyp-Grundlage (08.11.2026)

- **Bestandsaufnahme:** Core, Tests, Build-Presets und Unreal-Quelltext statisch geprüft. Ergebnisse in `Docs/Setup/Core-QA-Status-2026-11-08.md`.
- **Build-Fehler behoben:** `Tests/Core/DoorStatesTests.cpp` (sieben designierte Initialisierer ohne Feld `key`; Clang `-Werror`). Fix: `.key = {}`.
- **Neu:** `Core/include/spn/core/CasePrototype.h` und `Core/src/CasePrototype.cpp` (Platzhalter-Zustandsmaschine für PROTOTYPE-CASE-01, nur WorldFacts) mit `Tests/Core/CasePrototypeTests.cpp` (9 Tests). In `Core/CMakeLists.txt` und `Tests/Core/CMakeLists.txt` eingetragen.
- **QA:** `Tools/RunAllChecks.sh` schreibt Schritt-Berichte nach `build/qa-report.txt` und liefert Exit-Code 0 oder 1.
- **Ergebnis:** Alle vier Linux-Presets bestehen (configure, build, 608/608 Tests; Clang, GCC, mit ASan/UBSan).
- **TASKS:** PROTOTYPE-CASE-01 auf `in Arbeit`; neue Aufgabe `TECH-VARIANT-CLEANUP` (geplant, nur mit Freigabe).
- **Nicht geprüft:** Unreal-Build und Editor-PIE (Unreal nicht verfügbar), MSVC nach dem Fix.

## SPN-DEV-02: Dialog-, Beweis- und Ermittlungsgrundlagen (08.11.2026)

- **Entscheidungen** in `DECISIONS.md` Abschnitt J (DLG-01..04, EVD-01..04, DIF-01..03, NPCA-01..03), Quellenstatus Entwicklerangabe 08.11.2026.
- **Konflikt K-35:** `Ermittlung/23` T-F02 (automatischer Abschluss) gegen EVD-04 (keine automatische Fallauflösung). Nicht aufgelöst.
- **Neu im Core:** `InvestigationIds.h`, `Evidence.h/.cpp` (bekannt/gefunden/registriert, Registrierung erst nach Sammeln), `Dialogue.h/.cpp` (Knoten, Optionen, Bedingungen, Effekte, atomare Auswahl), `InvestigationAssist.h/.cpp` (Presets als Daten, Überschreibungen getrennt), `CasePrototype::AdoptInvestigationProgress` (Integration über Fakt-Schlüssel).
- **Tests:** 37 neue (Evidence 9, Dialogue 13, Assist 10, Integration 5) plus 9 bestehende CasePrototype-Tests. Gesamt 645 je Preset.
- **Ergebnis:** `fedora-clang`, `fedora-clang-asan-ubsan`, `fedora-gcc`, `fedora-gcc-asan-ubsan`: je 645/645 bestanden.
- **Statische Analyse:** `clang-tidy` (bugprone, performance, clang-analyzer) auf den neuen Quellen: keine Befunde außer Stilhinweisen zu leicht vertauschbaren String-Parametern und einem Fehlalarm (`Dialogue.cpp`, Zugriff hinter `has_value()`). Nicht geändert.
- **Behebung:** toter Parameter in `Dialogue.cpp` entfernt.
- **Nicht implementiert:** Schwierigkeitsnamen (DIF-01), Freitext (DLG-01), Monsterbeweise (EVD-03), Unreal-Anbindung.

## SPN-DEV-03: Ermittlungsabschluss und Architekturprüfung (08.11.2026)

- **K-35 entschieden und geschlossen:** Ermittlungsfall wird nicht automatisch abgeschlossen. Neue Entscheidung EVD-05 (`DECISIONS.md` J). EVD-04 verweist darauf. `Ermittlung/23` T-F02 umformuliert, ursprünglicher Wortlaut als Historie erhalten. `Spielvision.md` Zeile 60 mit Klarstellung versehen. `CONFLICTS.md` K-35 als entschieden markiert.
- **Architekturprüfung:** `Docs/Planning/Nachweis/architektur-pruefung-dev03.md`. WorldFacts bleibt Zustandsquelle; neue Module veröffentlichen keine Ereignisse; lokale Kennungen sind kollisionsfrei und unter den Schlüssellängen.
- **Codeänderungen (Befunde der Prüfung):** (1) Dialogeffekte dürfen fremde Zustandsnamensräume nicht schreiben (`placeholder.journal.`, `.dialogue.`, `.evidence.`, `.assist.`, `.case.`). (2) NPC-ID `player` reserviert. (3) `AdoptInvestigationProgress` prüft Kennungen (neuer Fehlercode `InvalidId`).
- **Tests:** 6 Regressionstests neu (Dialog 3, Integration 3), inkl. Ende-zu-Ende-Ablauf mit Save/Load. Gesamt 651 je Preset.
- **Nicht behauptet:** Unreal-Abnahme.
