# Quellenindex

Stand: 08.11.2026. Dieser Index sagt, was gelesen wurde und was nicht. Er ersetzt nicht die Quellen.

## 0. Aktualisierung 08.11.2026 (CONSOLIDATION-FINAL)

Die Lesestatus in Abschnitt 1 und 2 unten stammen aus früheren Paketen. Maßgeblich ist diese Aktualisierung. Die alten Zeilen bleiben zur Nachvollziehbarkeit stehen, sind aber überholt, wo sie abweichen.

Geprüft in den Paketen WELT, SYSTEME, ADULT, DELTA und FINAL (Stichprobe und Zielstellen, nicht vollständig):

| Quelle | Umfang | Lesestatus (aktuell) | Grundlage |
|---|---|---|---|
| `CLAUDE.md` | 161 Zeilen | vollständig | Projektregeln, Verweise auf `Docs/Planning/` geprüft |
| `Docs/Planning/` Hauptdateien (README, DECISIONS, CONFLICTS, OPEN_QUESTIONS, TASKS, CHANGELOG, PLANUNGSFRAGEN) | alle | vollständig | Bearbeitet; `verify_planning.py` |
| `Docs/Planning/Konsolidierung/B-welt.md`, `C-systeme.md`, `A-gameplay-spielvision.md` | je 213, 292, 304 Zeilen | vollständig (Zeilen-IDs gezählt) | Quellen der Nachweise |
| `Docs/Planning/Nachweis/welt.md`, `systeme.md`, `adult.md`, `deltas.md`, `gameplay.md` | je vollständig | Status gezählt; `welt`, `systeme`, `adult` bearbeitet; `deltas`, `gameplay` nicht inhaltlich bearbeitet | |
| `Docs/Game/Entscheidungsstatus.md` | 261 Zeilen | §2, §3, §6.4 gelesen; §6.5 bis §7 nicht | Teilweise |
| `Docs/Game/Spielwelt-und-Untergrund.md` | 441 Zeilen | Abschnitte 4, 7, 9.2, 14.1, 15 und Stichproben | Teilweise |
| `Docs/Game/Gameplay-Systeme.md`, `Spielvision.md` | 388 / 426 Zeilen | nur Zeilen 166–167, 217, 220 (Gameplay); Spielvision nicht | Überwiegend nicht gelesen |
| `Docs/Game/Adult/` (41 Dateien) und `Adult-System.md` | | alle 312 Quellzeilen des Adult-Nachweises maschinell geprüft; Register 14 und Matrizen in Auszügen | Teilweise |
| `Docs/Game/Ermittlung/` | | Register 20 (Zeilen 67–80, 98–163, 235–240); Testkatalog 23 (Abschnitte 4, 6, 9); Modul 04 §5, 14 §2, 17 Zeile 47 | Teilweise |
| `Docs/Game/Kreaturen/` | | 02 (57), 04 (246), 06 (76), 20 (51, 171–180), 21 (74) | Teilweise |
| `Docs/Game/NPCs/` | | 10, 22 (309), 26 (BH, SI) | Teilweise |
| `Docs/Game/Fahrzeuge-und-Verkehr/` | | 02 (59, 101–106), 03 (25), 04 (118), 05 (106), 06, 07 (4, 31), 08, 27 (121), 28 (GP, KL, PF, Zählung), 29 (245) | Teilweise |
| `Docs/Game/Kampfsystem/` | | nur Nachweis-Stellen (Testkatalog 30 nicht) | Kaum |
| `Docs/Game/World/` (01–28) | | Abschnitte der Nachweis-Zielstellen gelesen, Chroniken und Versionsgeschichte (19) nur punktuell | Teilweise |
| `Docs/Proposals/`, `Docs/Decisions/`, `Docs/NightBuild*/`, `Docs/M2*` | | nicht gelesen | Nicht gelesen |
| Master `99_DAILY_DELTAS/` | 102 Dateien | alle sechs `CHANGELOG.md` vollständig; `docs/`, `tests/`, `install.sh`, Manifeste, Assets nicht; nicht ausgeführt | Teilweise (siehe `Nachweis/tagesdeltas.md`) |
| Master `01_CURRENT_CANONICAL_STATE/` | 57 | nur zitierte Addenda und Statusdateien (siehe `DECISIONS.md` Abschnitt F, Paket 2) | Teilweise |
| Master `99_CANON_AUDITS/` | 16 | nicht gelesen | Nicht gelesen |
| Master `99_RECONSTRUCTION_SOURCES/` | 60 (23 MB) | nicht gelesen | Nicht gelesen |
| Master `02_WORLD_700/` | 12 (209 MB) | nur Lock-Dokument (`W700-PA`) zitiert | Kaum |
| Master `02_EMBEDDED_BASE_ARCHIVES/` | 3.689 | nicht gelesen | Nicht gelesen |
| Master `_SOURCE_ARCHIVE/` (1,8 GB), `_SOURCE_RESEARCH/` (849 MB) | 497 / 3 | nicht gelesen; nicht ins Repo | Nicht gelesen |
| Master `04_HISTORY/` (402), `03_EXTERNAL_LIBRARY_REGISTRY/`, `06_NEW_REFERENCES_2026-09-10/`, `99_MANIFESTS/`, `00_START_HERE/` | | nicht gelesen | Nicht gelesen |

Korrekturen gegenüber den alten Zählungen unten: `99_DAILY_DELTAS/` enthält 102 Dateien (nicht 71), `99_CANON_AUDITS/` 16 (nicht 13), `02_WORLD_700/` 12 (nicht 4).

Nicht geprüft und nicht behauptet: Der Gesamtbestand des 2,9-GB-Masters wurde nicht ausgewertet. Keine Validierung der Master-Aussagen über die zitierten Stellen hinaus.

Legende für **Lesestatus**: *vollständig* / *teilweise* (Zeilenbereich angegeben) / *Überschriften* / *Suchtreffer* / *nicht gelesen*.

### 0a. Inhaltsmigration 08.11.2026 (MASTER-MIGRATION-01)

Geprüft abschnittsweise (Zielstellen je Regel, nicht Zeile für Zeile): `CANONICAL_ADDENDUM_2026-09-05.md` §2–§6; `BLOCK_09_STATUS.md`; `MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md`; `CURRENT_CANON_2026-09-16.md` (Produktionsprompts); `VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10.md`. Vollständiger Stand je Quelle: `Docs/Planning/Nachweis/master-zuordnung-01-current.md`, Abschnitt E.

Nicht eingebettet, im Master als „EXTERNAL / NOT EMBEDDED“ geführt: 2026-09-27-Archivteile. Prompt-Bibliothek (`02_EMBEDDED_BASE_ARCHIVES`) bleibt Entwurf.

### 0b. Inhaltsmigration MASTER-MIGRATION-02 (08.11.2026)

- `02_WORLD_700`: Textdateien vollständig geprüft, Ziel `Game/World/24` §11. Binär-PNG nicht übertragen. Phase-A-Dateien `region_table.csv`, `regional_boundaries_overlay.svg`, `storage_estimates.json` nicht wiederherstellbar.
- `99_CANON_AUDITS`: Prüfbericht, kein Kanon; Duplikate `REFERENCE_CANON_2026-09-27` byte-identisch mit `01_CURRENT`.
- `99_DAILY_DELTAS`: neun Dokumente mit bestätigten Nutzerentscheidungen geprüft; übrige rund 60 Dateien offen.
- `04_HISTORY`, `99_RECONSTRUCTION_SOURCES`, `02_EMBEDDED_BASE_ARCHIVES`, `_SOURCE_*`: klassifiziert, nicht übertragen. Begründung in Register I.
- Gesprächsexport: 56 von 521 Gesprächen im Master-Nachweis; der Vollexport ist extern nicht verfügbar.

## 1. Repo: `Docs/` (Planungsdokumente; Lesestatus **überholt**, siehe Abschnitt 0)

| Quelle | Umfang | Lesestatus | Anmerkung |
|---|---|---|---|
| `CLAUDE.md` | 161 Zeilen | vollständig | Projektregeln. Wird in Paket 1 um einen Verweis auf `Docs/Planning/` ergänzt. |
| `Docs/Game/Entscheidungsstatus.md` | 261 Zeilen | teilweise (Zeilen 1–200 und Abschnitt 6.1–6.4) | §2 und §3 vollständig übernommen. §6.5–6.9, §7 offen. |
| `Docs/Game/Gameplay-Systeme.md` | 388 Zeilen | Überschriften | Abschnitte 2–18 offen. |
| `Docs/Game/Spielvision.md` | 426 Zeilen | Überschriften | Abschnitte 1–17 offen. |
| `Docs/Game/Spielwelt-und-Untergrund.md` | 441 Zeilen | Teile (Zeilen 118, 346–349, 372) | Vollständig offen. |
| `Docs/Game/Adult-System.md` | 149 Zeilen | nicht gelesen | Verweis auf Unterordner `Docs/Game/Adult/` (41 Dateien). |
| `Docs/Game/Adult/` | 41 Dateien | nicht gelesen | Detailplanung des optionalen Pakets. |
| `Docs/Game/Ermittlung/` | Dateien 01–15 und weitere | nicht gelesen | Ermittlungssystem. |
| `Docs/Game/Fahrzeuge-und-Verkehr/` | Unterordner | nicht gelesen | |
| `Docs/Game/Kampfsystem/`, `Docs/Game/Kreaturen/`, `Docs/Game/NPCs/`, `Docs/Game/World/` | Unterordner | nicht gelesen | |
| `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` | 96 Zeilen | vollständig | Quelle für S-09. |
| `Docs/Proposals/0003`, `0004`, `0007`, `0008`, `0009`, `0011` | je Datei | nicht gelesen | Engineering-Vorschläge, keine Planung im Sinne dieses Registers. |
| `Docs/Decisions/0001`–`0006` | 5 Dateien | nicht gelesen | Engineering-Entscheidungen. Relevant für Konflikte mit Spielentscheidungen. |
| `Docs/MasterIndex.md` | Index | Auszug (Überschriften und Suchhinweise) | Einstieg in den Master. |
| `Docs/NightBuild01`–`04/` | 35 Dateien | nicht gelesen | Berichte, keine Spielentscheidungen. |
| `Docs/M2A4`, `M2B1`–`M2B3`, `Setup`, `Development`, `ProtagonistV1` | Unterordner | nicht gelesen | Engineering und Setup. |

## 2. Master-Archiv (extern, nicht im Repo; Lesestatus **überholt**, siehe Abschnitt 0)

Pfad: `/mnt/Festplatte/SPN_FULL_MASTER_2026-09-27/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/`
Identische Kopie in `/mnt/Festplatte/SPN_MASTER_FOR_WINDOWS.zip` (Wurzelordner `Master/`).
Verkürzte Kopie `/mnt/Festplatte/SPN_FULL_MASTER_2026-09-27 (Kopie)` (599 MB, 4713 Dateien, unvollständig).

Inventar (Stand 08.11.2026): 4869 Dateien, 2,9 GB, 490 Ordner. Hash-Liste außerhalb des Repos unter dem Sitzungs-Scratchpad (`master_sha256.txt`, `master_sizes.txt`).

| Bereich | Dateien | Lesestatus | Anmerkung |
|---|---|---|---|
| `01_CURRENT_CANONICAL_STATE/` | 57 (md) | vollständig (Paket 2, 08.11.2026) | Alle Dateien gelesen oder über Überschriften und Statuszeilen geprüft. Übernommen als `DECISIONS.md` Abschnitt F. |
| `00_START_HERE/` | 1 md | nicht gelesen | Einstieg. |
| `99_DAILY_DELTAS/` | 71 md | nicht gelesen (nur über Kanon-Addenda referenziert) | Vorschläge und berichtete Ergebnisse, keine Canon. Folgepaket. |
| `99_CANON_AUDITS/` | 13 md | nicht gelesen | |
| `99_RECONSTRUCTION_SOURCES/` | 59 md, 23 MB | nicht gelesen | Quellenmaterial. |
| `04_HISTORY/` | 402 md | nicht gelesen | Historisch. Nur bei Bedarf. |
| `02_WORLD_700/` | 4 md, 209 MB (GIS, PNG) | nicht gelesen | Historische Welt, Geografie gesperrt. Nicht ins Repo. |
| `02_EMBEDDED_BASE_ARCHIVES/` | 3673 md, 33 MB | nicht gelesen | Hauptsächlich Prompt-Bibliothek, Status „Entwurf, Neuschreibung nötig“. |
| `_SOURCE_ARCHIVE/` | 497 Dateien, 1,8 GB | nicht gelesen | PNG, JSON, GeoJSON, SVG, 66 md. Nicht ins Repo. |
| `_SOURCE_RESEARCH/` | 3 Dateien (MP4), 849 MB | nicht gelesen | Videos. Nicht ins Repo. |
| `06_NEW_REFERENCES_2026-09-10/` | 2 Dateien | nicht gelesen | |
| `03_EXTERNAL_LIBRARY_REGISTRY/`, `99_MANIFESTS/` | wenige | nicht gelesen | Register und Manifeste. |

Duplikate: 1096 Hash-Gruppen mit gleichem Inhalt. Eine Kanon-Kopie pro Gruppe wird in Paket 2 festgelegt.

## 3. Außerhalb des Repos, nicht geprüft

- `/mnt/Festplatte/Schreibtisch/`, `/mnt/Festplatte/Dokumente/`, `/mnt/Festplatte/Downloads/`, `/mnt/Festplatte/Mods/`, `/mnt/Festplatte/Digi4School/`: auf Planungsmaterial nicht geprüft.
- Gesprächsverlauf: Entscheidungen aus Gesprächen mit dem Entwickler sind nur dann erfasst, wenn sie im Repo oder im Master stehen. Die November-Angaben sind als solche gekennzeichnet (siehe `DECISIONS.md` A).
