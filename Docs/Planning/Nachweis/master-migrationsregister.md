# Master-Migrationsregister (zentral)

Stand: 09.11.2026. Quelle: `Planning/Master` (`SPN_FULL_MASTER_2026-09-27`, nur lesend). Dieses Register ist die zentrale Übersicht. Die Detailregister stehen in `master-zuordnung-01-current.md`, `entscheidungsabgleich-2026-08-31.md`, `entscheidungsabgleich-project-decisions.md` und `fortschritt-master-migration.md`.

## Begriffe

- **inventarisiert:** Datei erfasst, SHA-256 berechnet, Duplikatgruppe bestimmt.
- **gelesen:** Inhalt in dieser Arbeit gelesen (vollständig oder als Kurzlesung, siehe Spalte).
- **abgeglichen:** Inhalt gegen konkrete `Docs/`-Zielstellen im Wortlaut verglichen.
- **migriert:** fehlende gültige Anforderungen in `Docs/` übertragen.
- **formal abgeschlossen:** alle relevanten eindeutigen Inhalte verarbeitet oder nachvollziehbar ausgeschlossen.

Duplikate sind byte-identisch (SHA-256 bzw. `cmp`). Ähnliche Fassungen gelten nicht als Duplikat.

## Gesamtzahlen (gemessen, 09.11.2026)

- Dateien im Master-Baum: **4.869**
- Eindeutige Inhalte global (SHA-256, alle 4.869 Dateien vollständig gehasht am 09.11.2026): **2.440**
- Redundante Kopien: **2.429**
- Binärdateien (Bilder, Video, Archive): über 250 in `_SOURCE_ARCHIVE` und 3 MP4 in `_SOURCE_RESEARCH`, plus Kartenbilder in `02_WORLD_700`. Alle Dateien einschließlich der 14 Dateien über 50 MB sind gehasht (`sha256sum`).

## Quellengruppen

| Gruppe | Dateien | Eindeutig | Relevanz Spielplanung | inventarisiert | gelesen | abgeglichen | migriert | formal abgeschlossen | Zielbereich in `Docs/` | Noch nötige Arbeit |
|---|---:|---:|---|---|---|---|---|---|---|---|
| `01_CURRENT_CANONICAL_STATE` | 57 | 57 | hoch (aktueller Kanon) | ja | ja, 57 von 57 vollständig gelesen | ja, 57 von 57 Zielstellen geprüft (Abschnitt J) | ja (Anforderungen nachgewiesen) | **ja** | `Game/Spielwelt-und-Untergrund.md`, `World/*`, `Spielvision.md`, `Gameplay-Systeme.md`, `Planning/CANON.md` | Zeilen-Gegenprüfung CA5 §4 (Volltext), Block 9, Statusreihen |
| `04_HISTORY` | 402 | 44 | keine eigenen Inhalte | ja | ja (Hashes) | ja: alle 44 identisch mit `01_CURRENT` | durch `01_CURRENT` gedeckt | **nein** (folgt 01) | wie 01_CURRENT | keine eigene Arbeit; Abschluss folgt 01_CURRENT |
| `02_EMBEDDED_BASE_ARCHIVES` | 3.689 | 1.860 (866 Prompt-Titel in zwei fast identischen Versionen; 848 Prompt-Titel + 18 Track-Kontexte; 128 Nicht-Entwurf-Inhalte) | gemischt | ja (`Nachweis/embedded-klassifikation.md`) | ja: Blöcke 8–9 (Fundstellen), SPN_Prompt_Pack (22, vollständig), Prompt-Archiv (Text geprüft, Themen-Abgleich) | ja: Blöcke 8–9 vollständig zugeordnet; Prompt-Archiv in `Nachweis/prompt-archiv-zuordnung.csv` | teilweise | **nein** | `Game/*`, `Development/*` | Prompt-Archiv: je Titel einzelne Zielstelle (Entwurf, keine Anforderungsquelle); Vorstufen-Startdokumente 00 (7) und 06 (3) Wort für Wort |
| – davon `02_PROMPT_LIBRARY_CURRENT/docs/prompts/` (Einzelprompts) | 866 | nicht getrennt gezählt | niedrig (Entwürfe) | ja | nur Track-Kontexte (18) | Track-Ebene | Track-Richtungen | nein | `Development/*` | Einzelprüfung nach Mustern |
| – davon `planning-backlog/` | 26 | 26 | mittel | ja | vollständig (21,5 KB) | ja | ja (Wildtiere, Verletzung, Plattform, Blender, Werkzeuge) | **ja** (vollständig gelesen, übertragen) | `Spielwelt` §9.2a, `Gameplay-Systeme`, `Development` §7, §8 | – |
| – davon `docs/governance/` | 17 | 17 | keine (Prozess) | ja | nicht gelesen | – | nicht übertragen | ausgeschlossen (Prozessregeln der alten Pipeline) | – | Klassifikation dokumentiert |
| – davon `docs/reference/` Entscheidungsquellen | 9 | 9 | hoch | ja | teilweise | teilweise | teilweise | siehe unten | `Game/*` | siehe unten |
| – davon `00_START_HERE`, alte `01_CURRENT` (9), Snapshots, `06`, `99_MANIFESTS` | ca. 30 | ca. 30 | niedrig bis mittel | ja | nicht gelesen | – | – | nein | – | Stichprobe der alten 01_CURRENT-Dateien |
| `99_DAILY_DELTAS` | 102 | 102 (intern; 5 Kopien identisch mit `01_CURRENT`) | hoch (Prüfvorlagen, teils bestätigte Nutzerentscheidungen) | ja | ja: alle CHANGELOGs, Ledger, Status- und Vergleichsberichte, Kern-Spezifikationen | ja (`Nachweis/daily-deltas-changelogs.md`) | ja | **teilweise** (Entscheidungen, Ledger, Berichte vollständig; Asset-Register und Install-/Prüfdateien begründet ausgeschlossen) | `World/*`, `Development/*`, `Planning/OPEN_QUESTIONS.md` | keine inhaltlichen Restposten; nur Asset-Inventare und Prozessdateien |
| `99_CANON_AUDITS` | 16 | 16 (intern; 3 Kopien identisch mit `01_CURRENT`) | mittel (Prüfbericht, kein Kanon) | ja | ja | ja (Entscheidungsregister, Konflikte) | nicht nötig (Nachweise) | **ja** (als Prüfbericht eingeordnet) | `Nachweis/master-zuordnung-01-current.md` G | – |
| `02_WORLD_700` | 12 | 12 | mittel (Referenzwelt) | ja | Textdateien vollständig | ja | ja (`World/24` §11) | **ja** (Textinhalte), Binärdateien begründet ausgeschlossen | `World/24` §11, `World/17`, `World/19` | Phase-A-Konflikt K-19 |
| `99_RECONSTRUCTION_SOURCES` | 60 | 60 | unbekannt (private Gespräche) | ja (Metadaten) | **nein** | **nein** | **nein** | **nein, gesperrt** | – | Nutzerentscheidung zur Zulässigkeit einer datensparsamen Extraktion |
| `03_EXTERNAL_LIBRARY_REGISTRY` | 18 | 18 | externe Verweise | ja (Metadaten) | ja, 18 von 18 (Verfügbarkeitsregister) | ja: Widerspruch R05A-Pfad dokumentiert (siehe unten) | – | Metadaten, begründet ausgeschlossen | `SOURCES.md` | Registerpflege: R05A-Eintrag korrigieren, nur mit Entwicklerfreigabe |
| `06_NEW_REFERENCES_2026-09-10` | 2 | 2 | niedrig | ja | ja (`REFERENCE_STATUS.md`; PNG 3,1 MB binär) | ja: kein Kanonstatus | – | Text erledigt; PNG begründet ausgeschlossen (Referenzbild, kein Kanon) | – | Zuordnung des Bildes zum Meridian-Reich offen (kein Kanon)
| `00_START_HERE` | 3 | 3 | niedrig | ja | ja, 3 von 3 | ja: alle Aussagen in `01_CURRENT`, `Planning/CANON.md` | ja | **ja** | `Planning/CANON.md` | – |
| `99_MANIFESTS` | 3 | 3 | keine (Prüfsummen) | ja | ja (Metadaten, 3 von 3) | – | – | Metadaten, begründet ausgeschlossen | – | – |
| `_SOURCE_ARCHIVE` | 497 | 323 (179 Binär nur dort, 128 Text nur dort, 16 Kopien) | niedrig bis mittel (WORLD_700-Iterationspakete) | ja | ja: alle 143 eindeutigen Textdateien inhaltlich gescannt | Methodenregeln in `World/24` §12; Iterations-Historie `World/19` | Binär 179: begründet ausgeschlossen. Text 128: klassifiziert, Methodenregeln übernommen, Geometrie und Messwerte begründet ausgeschlossen | – | **nein** | nur bei Freigabe zeilenweise |
| `_SOURCE_RESEARCH` | 3 | 3 (MP4) | unbekannt | ja | nicht lesbar als Text | – | – | **extern/Binär** | `SOURCES.md` | Entwickler-Entscheidung, ob Inhalte benötigt werden |
| Wurzeldateien (`PARTS.json`, `PARTS_SHA256.txt`, `README_INDEX.md`, `x.txt`, `_MERGE_LOG.txt`) | 5 | 5 | keine | ja | ja: `README_INDEX`, `PARTS`, Prüfsummen gelesen; `x.txt` = Verzeichnisbaum (5.078 Zeilen), `_MERGE_LOG.txt` = Installationsprotokoll (7.755 Zeilen) | – | – | Metadaten, begründet ausgeschlossen | – | Part 07–09 (`PARTS.json`) nicht im Master vorhanden |

## Entscheidungsquellen (Teil `02_EMBEDDED/docs/reference`)

| Quelle | Stand |
|---|---|
| `DECISION_CHANGELOG_2026-08-31.md` | **formal abgeschlossen** (55 Abschnitte, 60 Prüfzeilen im Wortlaut) |
| `DECISION_CHANGELOG_2026-08-23.md` | **formal abgeschlossen** |
| `DECISION_CHANGELOG_2026-08-24.md` | **formal abgeschlossen** (offene Punkte Q-BUILD-1, Q-VOICE-1) |
| `OPEN_DECISIONS.md` (124 Punkte, 17 Gruppen) | **geprüft (124 von 124 offen geführt)**: Zielstellen je Gruppe in `Nachweis/entscheidungsabgleich-open-decisions.md`; 12 neue Q-OD (Q-OD-42 bis Q-OD-53) für Punkte ohne offene Zielstelle; 1 Konflikt (Trophäengruppen) |
| `PROJECT_DECISIONS.md` (58 Abschnitte) | **geprüft, 58 von 58 im Wortlaut**; Konflikte bleiben dokumentiert (Checkpoint nach Tod, Trophäengruppen, Creator-Wirtschaft). Formal geschlossen mit offenen Konflikten (MASTER-MIGRATION-17) |
| `PROJECT_STATE_2026-08-31_COMPACT.md` (85 Abschnitte, 619 Zeilen) | **formal abgeschlossen** (alle 85 Abschnitte geprüft; Abschnitt 83 nach Ergänzung in `Spielvision` §2.1 vollständig belegt) | – | `Nachweis/project-state-compact.md` |
| `CURRENT_COVERAGE.md` (42 Zeilen) | **gelesen**; Abdeckungsübersicht ohne eigene Anforderungen; die Prozentangaben (22–23 %, 2–3 %) nicht übernommen (keine Berechnungsgrundlage) |
| `ARCHIVE_LINEAGE.md`, `PACK_VALIDATION.md`, `FILE_MANIFEST_SHA256.md`, `PROGRESS_SNAPSHOT_*` | nicht gelesen; Prozess- oder Prüfsummendateien |

## Inventurvalidierung (09.11.2026)

- Gruppenzuordnung: 18 Gruppen, überschneidungsfrei, Summe 4.869 Dateien (Abweichung: keine).
- Gruppenintern eindeutig: 02_EMBEDDED 1.860 von 3.689; _SOURCE_ARCHIVE 323 von 497; 04_HISTORY 44 von 402; übrige Gruppen intern eindeutig.
- Gruppenübergreifende Duplikate (identische Inhalte in zwei Gruppen): `04_HISTORY` ↔ `01_CURRENT` (44 Paare), `99_DAILY_DELTAS` ↔ `01_CURRENT` (5), `99_CANON_AUDITS` ↔ `01_CURRENT` (3), `02_EMBEDDED` ↔ `_SOURCE_ARCHIVE` (8), `02_WORLD_700` ↔ `_SOURCE_ARCHIVE` (8).
- Globale eindeutige Inhalte: **2.440**; redundante Kopien: **2.429** (4.869 − 2.440). Korrektur gegenüber der früheren Angabe 2.444/2.425: Die 14 Dateien über 50 MB wurden früher als 14 eigene Einheiten gezählt; sie haben aber nur 10 verschiedene Inhalte (`sha256sum`). Die Differenz von 4 ist damit erklärt.

## Byte-identische Duplikate (nachgewiesen)

- `04_HISTORY` (402 Dateien) enthält 44 eindeutige Inhalte, alle identisch mit `01_CURRENT_CANONICAL_STATE`.
- `99_DAILY_DELTAS/…/assets/prior_canon_refs/` (5 Dateien) identisch mit `01_CURRENT_CANONICAL_STATE` (`cmp`).
- `99_CANON_AUDITS/…/REFERENCE_CANON_2026-09-27/` (3 Dateien) identisch mit `01_CURRENT_CANONICAL_STATE` (`cmp`).
- `_SOURCE_ARCHIVE` enthält 8 Kopien aus `02_EMBEDDED_BASE_ARCHIVES` und 8 aus `02_WORLD_700` (identische Hashes, keine eigenen Inhalte).

## Historisch überholt (dokumentiert)

- Weltgrößen 200–300 km², 1.500–1.800 km², 2.000–2.200 km² als Richtwert: durch N-01 offen.
- Unterirdische Zivilisation (Erbauer, 50–100 km², Bevölkerung, Schienenverbund): durch N-02 gestrichen; Bunker und kleinere Bereiche bleiben.
- Sammelobjekte 1.200–1.500 als physische Gesamtzahl: historisch (CA5 §6).
- 2.200-km²-Vorab-Ziel (Karte 18./19.10.): nur als Vorab-Ziel.

## Nicht relevante Prozessdokumente (ausgeschlossen)

- `02_…/docs/governance/` (17 Dateien): Prozessregeln der alten Prompt-Pipeline.
- Scrum-, Token- und Pipeline-Vorgaben aus `planning-backlog/23` und Governance: nicht übernommen.

## Binärdateien und externe Referenzen

- Kartenbilder der Iterationen 5 und 6 (je rund 105 MB), Vorschaubild Phase A (7,7 MB): begründet nicht übertragen; Maße und Hashes in `World/24` §11.
- `_SOURCE_ARCHIVE` (1,9 GB), `_SOURCE_RESEARCH` (890 MB): extern, nicht als Text prüfbar.

## Nicht lesbare oder fehlende Quellen

- Phase-A-Dateien `region_table.csv`, `regional_boundaries_overlay.svg`, `storage_estimates.json`: im Archiv nicht wiederherstellbar (`RECOVERY_STATUS.md`).
- 2026-09-27-Archivteile: laut Master „EXTERNAL / NOT EMBEDDED“.
- Gesprächsexport: 56 von 521 Gesprächen; der Vollexport ist extern nicht verfügbar.

## Offene Konflikte

K-19 (Phase A), K-21 (Zeitverhältnis Hölle), K-22 (Amputation), K-28 (Bunker), K-29 (Untergrund nach N-02), K-33 (Zeitreise-Geografie), N-01 (Weltgröße), Checkpoint-Schleife (`Kampfsystem/14` Punkt 4 gegen `Spielvision` §14.2 und `COMBAT-DEC-BW-001`), Polizeieskalation (Changelog 31.08. Abschnitt 39 gegen `Ermittlung/14` 💡).

## Fortschritt der Gesamtmigration (belastbar gemessen)

- Formal bearbeitete Entscheidungsquellen: **5 von 5** benannten Quellen (Changelog 31.08., 23.08., 24.08., `PROJECT_DECISIONS.md` mit 58 von 58 Abschnitten, `OPEN_DECISIONS.md` mit 124 von 124 Punkten offen geführt). Offen bleiben Konflikte und Fragen, die nur der Entwickler entscheiden kann.
- Wortlaut-geprüfte Abschnitte in `PROJECT_DECISIONS`: **58 von 58** (100 %, gezählt aus den Protokollzeilen des Nachweises).
- Planungsbacklog: vollständig.
- `01_CURRENT`: **formal abgeschlossen** (57 von 57 Dateien gelesen und abgeglichen, Nachweis `master-zuordnung-01-current.md` Abschnitt J; offene Konflikte K-21 und K-28 bleiben im Register).
- Formal abgeschlossene Quellengruppen (vollständig geprüft): `01_CURRENT` (57 Dateien), `04_HISTORY` (durch 01 gedeckt), `99_CANON_AUDITS` (als Prüfbericht), `02_WORLD_700` (Textdateien), Planungs-Backlog (26 Dateien).
- Eindeutige Inhalte (von 2.440) mit abgeschlossener Inhaltsprüfung: **148 = 6,07 %** (Messung 10.11.2026). Gegenüber 125 neu: SPN_Prompt_Pack (22 Dateien, vollständig gelesen und je Datei gegen Zielstellen geprüft) und die Blöcke 8–9 (1). Begründet ausgeschlossen: **37** (28 Metadaten- und Prozessdateien plus 9 Status- und Vergleichsberichte der Tages-Deltas, `Nachweis/daily-deltas-changelogs.md` Arbeitspaket A). Offen: **2.255** (2.440 − 148 − 37). Nicht gezählt: Prompt-Archiv (866 Inhalte, Text geprüft, Abgleich themenbezogen), `_SOURCE_ARCHIVE`-Textpakete (gescannt, Methodenregeln übernommen).

## Nächster Arbeitsschritt

1. Prompt-Archiv (866 Inhalte): je Titel die Zielstelle in `prompt-archiv-zuordnung.csv` am Text prüfen; Stichprobe von 50 Titeln Wort für Wort.
2. Vorstufen-Startdokumente `00_START_HERE` (02, 7 Dateien) und `06_WORLD_1500_PLANNING` (3) Wort für Wort gegen `World/12`, `World/06`.
3. `_SOURCE_ARCHIVE`: Textpakete zeilenweise nur bei Freigabe; `99_RECONSTRUCTION_SOURCES` bleibt gesperrt bis zur ausdrücklichen Nutzerfreigabe.
