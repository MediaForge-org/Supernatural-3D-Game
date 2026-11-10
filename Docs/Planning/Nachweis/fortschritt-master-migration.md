# Fortschrittsübersicht Master-Migration

Stand: 09.11.2026. Diese Datei trennt drei Stufen strikt:

1. **Inventarisiert:** Datei erfasst, Hash berechnet, Duplikatgruppe bestimmt.
2. **Inhaltlich geprüft:** Originaltexte gegen `Docs/` gelesen, Zielstellen belegt.
3. **Migriert und abgeschlossen:** alle eigenständigen Inhalte nachweislich in `Docs/` oder ausdrücklich als historisch, verworfen, extern nicht verfügbar oder offen dokumentiert.

Eine Stufe impliziert die vorherige nicht automatisch. Eine Datei mit nur Stufe 1 gilt **nicht** als geprüft. Unbekannte Werte sind mit „unbekannt“ gekennzeichnet.

Detailregister: `master-zuordnung-01-current.md` (Abschnitte A bis I). Hash-Inventar `04_HISTORY`: `inventar-04-history.csv` (Spalten: Pfad, Typ, Größe, SHA-256, Duplikatgruppe, Anzahl Kopien, Snapshot, logischer Name, Überschrift).

## Gesamtübersicht

| Verzeichnis | Dateien | Inventarisiert | Exakte Duplikate | Eigenständige Inhalte | Inhaltlich geprüft | Migriert und abgeschlossen |
|---|---:|---|---|---|---|---|
| `01_CURRENT_CANONICAL_STATE` | 57 md | ja | unbekannt | unbekannt (nicht abschließend gezählt) | teilweise | **0 vollständig** |
| `02_WORLD_700` | 12 | ja | unbekannt | 4 Textdateien + Binär | Textdateien vollständig | Textinhalte migriert, Binärdateien begründet nicht übertragen |
| `99_CANON_AUDITS` | 16 | ja | unbekannt | Prüfbericht, kein Kanon | vollständig (Bericht) | klassifiziert, Nachweise nicht als Anforderungen übernommen |
| `99_DAILY_DELTAS` | 102 | ja (Metadaten) | unbekannt | Prüfvorlagen, rund 60 Dokumente | 9 kanon-relevante Dokumente plus Register-H-Dokumente | Register H abgeschlossen (inhaltlich relevante Dokumente), Metadaten offen |
| `04_HISTORY` | 402 | **ja (vollständig, SHA-256)** | **400 Dateien in 42 Gruppen; 358 überzählige Kopien** | **44 eindeutige Inhalte** | Inhalte = `01_CURRENT`, siehe unten | Inhalte durch `01_CURRENT` gedeckt; **kein Eigeninhalt** |
| `99_RECONSTRUCTION_SOURCES` | 60 | **ja (vollständig, SHA-256)** | **0** | **60 eindeutige Dateien** | **nicht geprüft** | **nicht migriert**; privater Gesprächsinhalt, Prüfung offen |
| `02_EMBEDDED_BASE_ARCHIVES` | 3.689 | **ja (vollständig, SHA-256)** | **2.756 Dateien in 927 Gruppen; 1.829 überzählige Kopien** | **1.860 eindeutige Inhalte** | Planungsbacklog vollständig gelesen; übrige Ordner **nicht** geprüft | **teilweise** (Backlog-Richtungen übertragen) |
| `03_PROMPT_LIBRARY_HISTORY` (Teil von 02) | 1.807 | ja | in 02 enthalten | in 1.860 enthalten | nicht geprüft | nicht migriert, Entwurf |
| `_SOURCE_ARCHIVE`, `_SOURCE_RESEARCH`, `03`, `06`, `00`, `99_MANIFESTS` | je Verzeichnis | nur Zählung | unbekannt | Binär oder Verweise | nicht geprüft (Verweise in `SOURCES.md`) | klassifiziert, nicht übertragen |

## 04_HISTORY im Detail

- **Struktur:** 13 Momentaufnahmen des Kanon-Zustands (`PRE_2026-09-10_CURRENT_STATE` bis `PRE_2026-09-27_CURRENT_STATE`) plus 5 Dateien im Wurzelverzeichnis.
- **Exakte Duplikate:** 400 Dateien gehören zu 42 Duplikatgruppen. 358 Kopien sind überzählig. Die 2 übrigen sind Einzeldateien.
- **Eigenständige Inhalte:** 44 unterschiedliche Dateiinhalte.
- **Identität mit dem aktuellen Stand:** Alle 44 eindeutigen Inhalte sind mit der gleichnamigen Datei in `01_CURRENT_CANONICAL_STATE` **byte-identisch** (SHA-256 geprüft). Die 13 übrigen Dateien von `01_CURRENT` (Addenda 10-15 und 10-18, Statusdateien 09-27 und 10-13, Supersession 09-27) kommen in `04_HISTORY` nicht vor.
- **Ähnliche Versionen:** Keine. Für keinen Dateinamen gibt es in `04_HISTORY` unterschiedliche Fassungen. Es gibt also keine historischen Abweichungen, die gesondert zu prüfen wären.
- **Ergebnis:** Die Inhalte von `04_HISTORY` sind vollständig durch `01_CURRENT` abgedeckt. Die Migration hängt daher am Stand von `01_CURRENT`, der noch nicht abgeschlossen ist.

## 99_RECONSTRUCTION_SOURCES

- Inventarisiert: 60 Dateien (59 Markdown, 1 Textdatei), alle eindeutig.
- Enthält laut Master nicht spielbezogene Gesprächsprotokolle, teils persönlich. Dateinamen und Überschriften werden **nicht** in `Docs/` übernommen, weil sie private Themen offenlegen können.
- **Nicht geprüft.** Ob fachliche Spielanforderungen darin stehen, ist unbekannt. Eine datensparsame fachliche Extraktion braucht eine Zulässigkeitsentscheidung des Entwicklers.

## 02_EMBEDDED_BASE_ARCHIVES

- Inventarisiert: 3.689 Dateien (3.673 Markdown, 10 JSON, 5 CSV, 1 Text). Eindeutige Inhalte: 1.860. Überzählige Kopien: 1.829.
- **Heuristische Einordnung der 1.860 eindeutigen Inhalte** (nach Überschriftsmuster, nicht manuell verifiziert):
  - 1.696 technische Arbeitsanweisungen (Aufgaben `SPN-01` bis `SPN-18`, Phase-0-Stil),
  - 58 Prompt-Vorlagen oder -Varianten,
  - 95 sonstige Dokumente,
  - 11 ohne erkennbare Überschrift.
- **Entwurfsstatus:** laut `SOURCES.md` und Master „Entwurf, Neuschreibung nötig“. Nicht als Anforderung zu übernehmen.
- **Geprüft:** Der Planungsbacklog `docs/planning-backlog/` (26 Dateien, 21,5 KB) wurde vollständig gelesen. Die kanonischen Richtungen wurden gegen `Docs/` abgeglichen:
  - Übertragen oder bereits vorhanden: Audio-Mastering (Track 20), Plattformen (21), Begleiter (22), Kreaturen-Rahmen (24), Ereignisse (25), Dialog und Stimmen (26), Charaktererstellung (33), Hunter-Crew und Akte (34), Koop und Online-Trennung (35), Wirtschaft (36), Wildtiere (37, neu ergänzt), Fahrzeuge und Trims (38), Recht und Beweise (39), soziale Medien (40), Körperanpassung (41), Medaillen (42), Verteilung (43), Verletzung, Gore und Darstellung (44, neu ergänzt).
  - Neu als Produktionsanforderung dokumentiert: Blender-Pipeline (Track 29) und Werkzeuganforderungen (23, 27, 28, 30, 31) in `Development/Produktions-Referenzen-und-Prompts.md` §7.
  - Genre-Positionierung (32): Verbot sichtbarer Monsterstufen und Beute-Tretmühle ist in `Spielvision` Z. 57 bereits enthalten.
- **Nicht geprüft:** `docs/governance/`, `docs/prompts/` (Track-Kontexte), `docs/` allgemein, `00_START_HERE`, `01_CURRENT_CANONICAL_STATE` dieses Archivs (9 Dateien, ältere Fassung vom 04.09., nicht identisch mit dem Master), die Snapshot-Ordner und die Manifeste. Der Inhalt dieser Dateien ist **unbekannt** und nicht abschließend bewertet.

## Offene Konflikte (unverändert)

K-19 (Phase A), K-21 (Zeitverhältnis Hölle), K-22 (Amputation), K-28 (Bunker), K-29 (Untergrund nach N-02), K-33 (Zeitreise-Geografie), N-01 (Weltgröße).

## Nächster Schritt

1. `01_CURRENT_CANONICAL_STATE`: Zeilen-Gegenprüfung der Dateien mit Anforderungen (CA5 §4 Volltext, Block 9).
2. `02_EMBEDDED_BASE_ARCHIVES`: `docs/governance/` und `docs/prompts/` auf Spielanforderungen prüfen.
3. `99_RECONSTRUCTION_SOURCES`: nach Zulässigkeitsentscheidung des Entwicklers. Bis dahin keine Übernahme.

## Stand 09.11.2026 (MASTER-MIGRATION-07): 02_EMBEDDED-Governance, Prompts, Track-Kontexte

| Quelle | Inventarisiert | Geprüft | Migriert | Befund |
|---|---|---|---|---|
| `02_…/docs/governance/` (17 Dateien, 0,57 MB) | ja | **Prozessregeln** (Prompt-Qualität, Freigabe-Gate, Scrum, Kontext-Routing u. a.) nur als Klassifikation: keine Spielanforderungen | nicht migriert, Prozessstatus | Gilt für die alte Prompt-Pipeline, nicht für `Docs/` |
| `02_…/docs/prompts/` (866 Dateien, 5 MB, 18 Tracks) | ja | **Track-Kontexte** (18 Dateien) vollständig gelesen und gegen `Docs/` abgeglichen. Einzelne Prompt-Dateien: **nicht** vollständig gelesen | Track-Richtungen übertragen oder bereits vorhanden; Einzelprompts teilweise | Einzelprompts sind Arbeitsanweisungen mit Entwurfsstatus |
| `02_…/docs/reference/DECISION_CHANGELOG_2026-08-31.md` (55 Abschnitte, 37 KB) | ja | **Stichprobe** (rund 20 Abschnitte) gegen `Docs/`; Rest offen | nicht vollständig | Wichtig: enthält Nutzerentscheidungen, daher vorrangig abzugleichen |
| `02_…/docs/reference/DECISION_CHANGELOG_2026-08-23/24.md` | ja | nicht geprüft | offen | Prompt-Qualität, Kampf, Stealth, Begleiter, Audio, Rendering, Test |
| `02_…/docs/reference/PROJECT_DECISIONS.md` (78 KB), `OPEN_DECISIONS.md`, `PROJECT_STATE_*` | ja | nicht geprüft | offen | Zusammenfassende Entscheidungsregister der Bibliothek |
| `02_…/00_START_HERE`, 9 ältere `01_CURRENT`-Dateien (Stand 04.09., nicht identisch mit Master), Manifeste | ja | nicht geprüft | offen | Ältere Fassungen; Inhalte vermutlich in Master-`01_CURRENT` überholt, aber nicht abschließend belegt |

**Track-Ebene (Prompts 01–18), Ergebnis:**
- 02 Welt: Richtung „dicht, viele begehbare Gebäude“. Die Größe 200–300 km² ist **historisch** und durch N-01 überholt.
- 03 Fälle: keine verborgene Pflichtliste; Detektivmodus ohne automatische Führung (Spielvision).
- 04 Archiv: 1.200–1.500 physische Sammelobjekte sind **historisch** (Korrektur CA5 §6).
- 05 NPCs: Verhalten statt Zahlen (Gameplay-Systeme).
- 06 Anatomie: kein Plot-Armor (Kampfsystem 13).
- 07 Schusswaffen: Fehlfunktionen (Kampfsystem 10 §6), Kill-Cam (Kampfsystem 05), Abpraller (Kampfsystem 05).
- 08 Nahkampf: Kampfsystem 04.
- 10 Schwierigkeit: Grounded mit Hilfen aus; Barrierefreiheit unabhängig (Entscheidungsstatus).
- 11 Persistenz: 20–50 MB Zielbereich; PostgreSQL nur Backend (NPCs/21).
- 12 Modding: Stufen Daten, Blueprint, C++ (Gameplay-Systeme).
- 13 Wissensplattform: **teilweise**. Spoiler-Stufen stehen in Spielvision; die Anforderungen an eine eigene Wissensplattform (DLC-Gating der Vollanleitung, Basiskauf schaltet Basisanleitung frei, spoilerfreie öffentliche Seiten) sind **nicht** als Anforderung übertragen. Offen.
- 14 Assets: Blender-Abdeckung (Development §7), Gore ursachenabhängig (Gameplay-Systeme).
- 16 IP und Recht: Original-IP (Spielvision), Freigabe (Planning).
- 17 Plattform: **ergänzt** (Development §8).
- 18 Entwicklerwerkzeuge: Zustandsprüfung ohne Cheat-Leck; Debug-Anforderungen in Ermittlung 04, 08, 19.

**Insgesamt:** Die 866 Einzelprompts gelten als **Entwürfe** (Master: „DRAFT_REWRITE_REQUIRED“). Ihre fachlichen Aussagen wurden nur über die Track-Kontexte und die Stichprobe erfasst. Eine Einzelprüfung nach Mustern bleibt offen.

## Nachweis der Governance-Entscheidung

Die Prozessregeln in `governance/` (z. B. `PRECODING_GATE`, `HUMAN_APPROVAL_GATE`) setzen eine alte Prompt-Pipeline voraus. Sie werden **nicht** als Projektregel übernommen. Für dieses Projekt gelten `CLAUDE.md` und `Docs/Planning/README.md`.

## Stand 09.11.2026 (MASTER-MIGRATION-08): Direkte Nutzerentscheidungen

| Quelle | Inventarisiert | Geprüft | Migriert | Befund |
|---|---|---|---|---|
| `reference/DECISION_CHANGELOG_2026-08-31.md` (55 Abschnitte) | ja, vollständig gelesen | **Abschnitt für Abschnitt** in `Nachweis/entscheidungsabgleich-2026-08-31.md`: 60 Prüfzeilen, davon 33 mit Textprüfung, 26 nur Stichwort | **nicht abgenommen**: 26 Stichwort-Zeilen offen | Ergänzt: Geografie vor Story, Missionszustände, Online-Maßstab, Verwesung/Aasfresser, Alterung |
| `reference/DECISION_CHANGELOG_2026-08-23.md` | ja | nicht geprüft | nicht | offen |
| `reference/DECISION_CHANGELOG_2026-08-24.md` | ja | nicht geprüft | nicht | offen |
| `reference/PROJECT_DECISIONS.md` (78 KB) | ja | nicht geprüft | nicht | offen |
| `reference/OPEN_DECISIONS.md` (9 KB) | ja | nicht geprüft | nicht | offen |

**Zählung Entscheidungen:** 55 Abschnitte im Changelog 08-31 erfasst. Davon 43 Zeilen bestätigt, 9 ergänzt, 7 teilweise, 1 informell (Fortschrittsschätzung, nicht übernommen). Keine Entscheidung ist als historisch oder widersprüchlich eingestuft; das Ergebnis ist nicht durch Gegenlesen aller Zeilen abgesichert.

## Stand 09.11.2026 (MASTER-MIGRATION-09): Wortlaut-Prüfung 31.08. und 23./24.08.

| Quelle | Prüfstufe | Migration | Befund |
|---|---|---|---|
| `DECISION_CHANGELOG_2026-08-31.md` (55 Abschnitte, 60 Prüfzeilen) | alle Zeilen im Wortlaut geprüft (33 Zeilen „Text geprüft“, 26 Zeilen „Wortlaut“, 1 informell) | 13 Lücken ergänzt: Geografie vor Story, Missionszustände, Online-Richtung, Verwesung, Alterung, Kosmetik ohne Spielkraft, Monsterreaktion, Insassenverletzung, Luftlenkung, Vertrautheit, Rauchen, digitale Beweise, NPC-Kommunikation | formal verarbeitet. Offene Entscheidungen sind dokumentiert: Abschnitt 39 (Anzeige und Minimap), 52 (Freigabeprüfung), 53 (Stufennamen), 16 (Vertrieb), 23 (Aktivierung Combat Focus), 3 (Umzugsgründe), 7 (Zustandsschema), 34 (App-Umfang) |
| `DECISION_CHANGELOG_2026-08-23.md` (7 Themen) | Wortlaut geprüft für Kampf, Stealth, Begleiter, Audio, Rendering, Test | Audio-Qualität und Audio-QA ergänzt (`Development` §9) | formal verarbeitet, mit offener Entscheidung zu Pre-1.0-Builds (Q-BUILD-1) |
| `DECISION_CHANGELOG_2026-08-24.md` (16 Punkte) | Stichprobe: Kreaturen, Archiv, Dialog, Stimme, Prompt-Werkzeuge, Testorchestrator, Freigabe-Gate | Prompt-Werkzeuge in `Development` §7 | nicht formal verarbeitet. Offen: Pre-1.0-Builds (Q-BUILD-1), Platzhalterstimmen (Q-VOICE-1). Restliche Punkte nicht zeilenweise gegengelesen |
| `PROJECT_DECISIONS.md` (78 KB) | nicht geprüft | – | offen |
| `OPEN_DECISIONS.md` (9 KB) | nicht geprüft | – | offen |

**Zählung Entscheidungsquellen (formal verarbeitet):** 2 von 4 (Changelog 31.08. und 23.08.). Der Changelog 24.08. ist nur teilweise geprüft.

## Stand 09.11.2026 (MASTER-MIGRATION-10): Changelog 24.08., OPEN_DECISIONS, PROJECT_DECISIONS (Zeilen 1–260)

| Quelle | Prüfstufe | Migration | Befund |
|---|---|---|---|
| `DECISION_CHANGELOG_2026-08-24.md` (16 Punkte) | Wortlaut der relevanten Zielstellen geprüft: Kreaturen-Dimensionen (`Kreaturen/03`), friedliche Wesen (`Spielvision`), Wissensstatus und Unbekanntes (`Ermittlung/04`), Dialog-Volumen und Stimmen (`Spielvision`), Prompt-Werkzeuge (`Development` §7) | keine neuen Lücken außer Pre-1.0-Builds (Q-BUILD-1) und Platzhalterstimmen (Q-VOICE-1) | formal verarbeitet, offene Punkte dokumentiert |
| `OPEN_DECISIONS.md` (124 Punkte, 9 Gruppen) | automatischer Stichwortabgleich: 83 Punkte mit Treffern (**nicht** einzeln gegengelesen), 41 Punkte ohne Treffer | 41 Punkte als Q-OD-01 bis Q-OD-41 in `Planning/OPEN_QUESTIONS.md` übertragen | **teilweise**. Die 83 Treffer brauchen eine Einzelprüfung |
| `PROJECT_DECISIONS.md` (707 Zeilen, 58 Abschnitte) | Zeilen 1–260 stichprobenartig gegen `Docs/` (rund 70 Regeln); Abschnitte ab Zeile 260 nicht geprüft | keine neuen Lücken in den geprüften Regeln gefunden | **teilweise** (Zeilen 1–260); offen ab Zeile 260 |

**Formal verarbeitete Entscheidungsquellen:** 3 von 4 (Changelog 31.08., Changelog 23.08., Changelog 24.08.). Offen: `OPEN_DECISIONS.md` (Einzelprüfung der Treffer), `PROJECT_DECISIONS.md` (Zeilen 260–707).

**Nachtrag 09.11.2026 (PROJECT_DECISIONS, Zeilen 260–400):** Testing, Prompt-Governance, Mehrgegner-Kampf, Stealth und KI, Begleiter, Audio, Rendering und Testphilosophie stichprobenartig gegen `Docs/` geprüft (rund 60 Regeln). Ergänzt: Bitraten-Planungsbereiche (`Development` §9), Konsolen-Anzeigeoptionen, Türgeräusche und leise Bewegung (`Game/Gameplay-Systeme.md`). Offen: Zeilen 400–707. Die Prompt-Governance-Regeln (Zeilen 268–278, 389–406) sind Prozessvorgaben der alten Bibliothek und keine Spielanforderungen.

## Stand 09.11.2026 (MASTER-MIGRATION-12): PROJECT_DECISIONS vollständig erfasst

- `Nachweis/entscheidungsabgleich-project-decisions.md`: alle 58 Abschnitte (707 Zeilen) erfasst, jeweils mit Prüfstufe und Zielstelle.
- Wortlaut-geprüft: **nicht**. Die Zeilen 1–400 wurden nur stichprobenartig abgeglichen. Zeilen 617–707 sind Dublette des Changelogs 31.08.
- Ergänzt in `Development/Produktions-Referenzen-und-Prompts.md` §10 (Dialog-Architektur, Stimmen-Bibel, Explorer-Technik, visuelle Referenzen) und §11 (Blender-Regeln, Laufzeit-Testschleife, Freigabe-Zustände, Vorproduktions-Reihenfolge, Fortschrittsmessung).
- Formal abgeschlossen: keine der Abschnittsquellen dieses Eintrags.
- Offen: OPEN_DECISIONS (83 Stichworttreffer), PROJECT_DECISIONS Zeilen 1–400 (Wortlaut), `PROJECT_STATE_2026-08-31_COMPACT.md`, `CURRENT_COVERAGE.md`.

**Nachtrag 09.11.2026 (MASTER-MIGRATION-13, PROJECT_DECISIONS):** Wortlaut-Protokoll im Nachweis `entscheidungsabgleich-project-decisions.md`. 9 von 58 Abschnitten im Wortlaut geprüft (Abschnitte 1–7 und 9 vollständig, Abschnitt 8 nur Kernpunkte). Ergänzt: Fall- und Story-Gebäude handgebaut (`Game/Spielwelt-und-Untergrund.md`). Konflikt dokumentiert: StateTree-Regel (PROJECT_DECISIONS) gegen 💡-Status in `Game/Kreaturen/10`. PROJECT_DECISIONS.md formal **nicht** abgenommen.

**Nachtrag 09.11.2026 (MASTER-MIGRATION-14/15):** Zentrales Gesamtregister `master-migrationsregister.md` angelegt (4.869 Dateien, 2.444 eindeutige Inhalte, 2.425 redundante Kopien; Status je Quellengruppe). Wortlaut-Protokoll `PROJECT_DECISIONS`: 11 von 58 Abschnitten. Nächster Schritt: Abschnitt 12, Zeile 138.

**Nachtrag 09.11.2026 (MASTER-MIGRATION-17):** `01_CURRENT_CANONICAL_STATE` vollständig gelesen und abgeglichen (57 von 57, Abschnitt J in `master-zuordnung-01-current.md`). Zählung neu gemessen: 2.440 eindeutige Inhalte, 2.429 Kopien (frühere Angaben 2.444 / 2.425 korrigiert). Inhaltlich abgeschlossen: 110 von 2.440 (4,51 %). `PROJECT_DECISIONS.md` im Wortlaut: 28 von 58 Abschnitte (1–28). Nächster Abschnitt: 29 (Zeile 414).
