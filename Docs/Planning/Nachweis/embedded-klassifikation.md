# 02_EMBEDDED_BASE_ARCHIVES: Klassifikation nach Inhalt und Duplikaten (MASTER-MIGRATION-17, 09.11.2026)

**Quelle (nur lesend):** `SPN_FULL_CUMULATIVE_MASTER_2026-09-27/02_EMBEDDED_BASE_ARCHIVES/` (3.689 Dateien; **1.860 verschiedene Inhalte**).

**Methode:** SHA-256 über alle Dateien (`sha256sum`). Jeder verschiedene Inhalt wird einer Kategorie zugeordnet; Kopien zählen nicht doppelt. Die Summe ist 1.860. Die Kategorie stammt aus dem Pfad.

## Zählung nach Kategorie (verschiedene Inhalte)

| Kategorie | Verschiedene Inhalte | Status | Begründung |
|---|---:|---|---|
| Prompt-Entwürfe, Version 0.4/0.5 (`docs/prompts`, 866 Dateien; Kopie in `extracted_v0.5/docs`) | 866 | **Entwurf**, `DRAFT_REWRITE_REQUIRED` | keine Anforderungsquelle; Track-Kontexte (18) gelesen; Einzelprompts nicht Zeile für Zeile |
| Prompt-Entwürfe, Version 0.1 (`SPN_Master_Prompt_Library_v0.1/prompts`, 866 Dateien) | 866 | **historischer Arbeitsauftrag** | älterer Entwurfsstand; keine Anforderung |
| Alte Prompt-Sammlung (`SPN_Prompt_Pack`, 22 Dateien) | 22 | historisch, nicht gelesen | ältere Auszüge der Prompt-Pakete; Inhalte durch die Entwürfe überholt |
| Planungs-Backlog (`docs/planning-backlog`, 26 Dateien) | 26 | **gelesen und übertragen** (MASTER-MIGRATION-Vorläufe) | siehe `master-migrationsregister.md` |
| Prozess-Governance (`docs/governance`, 17 Dateien) | 22 (inkl. Kopien der Altversion) | **ausgeschlossen, begründet** | Prozessregeln der alten Prompt-Pipeline (Scrum-Felder, Token-Vorgaben) |
| Referenz- und Entscheidungsdokumente (`docs/reference`) | 18 | gelesen oder geprüft | `PROJECT_DECISIONS` (58 von 58), `OPEN_DECISIONS` (124 von 124), `CURRENT_COVERAGE` (gelesen), `PROJECT_STATE` (teilweise), `DECISION_CHANGELOG` (abgeschlossen), `ARCHIVE_LINEAGE`, `PACK_VALIDATION`, `PROGRESS_SNAPSHOT` (Prozess, Prozentangaben nicht übernommen), `FILE_MANIFEST_SHA256` (Hashliste, nur Kopf) |
| Vorstufe 04.09. und Startdokumente (`00_START_HERE` 7, `01_CURRENT` alt 9, `06_WORLD_1500_PLANNING` 3, `08_PREVIOUS_SNAPSHOTS` 4, `supernatural_planning_snapshot_2026-09-04` 6) | 25 | teilweise gelesen, siehe unten | Vorstufen des heutigen Kanons; Inhalte überwiegend in `Docs/` eingearbeitet |
| Manifest und Metadaten (`99_MANIFESTS`, `SPLIT_MANIFEST.json`, `README_SPLIT_DOWNLOAD.txt`) | 5 | **ausgeschlossen, begründet** | Prüfsummen und Installationshinweise |
| Sonstige (`03_PROMPT_LIBRARY_HISTORY`, `02_PROMPT_LIBRARY_CURRENT`) | 9 | nicht einzeln geprüft | Governance-Reste und Nebendateien |
| **Summe** | **1.860** | | |

## Duplikate innerhalb von `02_EMBEDDED` und gegenüber den Hauptquellen

- `extracted_v0.5/docs` (924 Dateien) ist vollständig identisch mit `02_PROMPT_LIBRARY_CURRENT/docs` (Kopie).
- `03_PROMPT_LIBRARY_HISTORY/docs/prompts` (866) ist identisch mit `02_PROMPT_LIBRARY_CURRENT/docs/prompts` (866).
- `01_CURRENT_CANONICAL_STATE/SPN_Master_Project_State_2026-08-31_BASELINE.md` ist **byte-identisch** mit `02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_STATE_2026-08-31_COMPACT.md` (`cmp`).
- `01_CURRENT_CANONICAL_STATE/WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT.md` ist **byte-identisch** mit `06_WORLD_1500_PLANNING/WORLD_1500_RELEVANT_BLOCKS_1_TO_7.md` (`cmp`).
- `01_CURRENT_CANONICAL_STATE/WORLD_1500_CURRENT_REQUIREMENTS.md` ist **byte-identisch** mit `06_WORLD_1500_PLANNING/WORLD_1500_REQUIREMENTS_CURRENT.md` (`cmp`).

## Vorstufen 04.09.: Prüfbefund

- **`LATE_ADDITIONS_AND_CORRECTIONS_2026-09-04.md`** (Detektiv-Gameplay, Reusable Core, Bowling, Themen- und Wasserparks, Größenspanne, Sammelobjekt-Korrektur, Realismus): belegt in `Game/Ermittlung/07`, `Game/Gameplay-Systeme.md` §Simulationskern, `Game/Spielwelt-und-Untergrund.md` (Bowling, Parks), `Game/Spielvision.md` §Sammelobjekte, `Game/World/03` und `Game/Entscheidungsstatus.md` (Größe, N-01 historisch).
- **`OPEN_DECISIONS_AND_NEXT_WORK.md`**: offene Punkte (Bevölkerung WORLD_1500, Stadtanzahl, Küste/See, Zeitskala, Stadien, Park-Anzahl, Spielhallen, räumliche Zuordnung, Launch-Umfang) belegt in `Game/Entscheidungsstatus.md` §3 und §5, `Game/World/08`.
- **`REALM_DESIGN_PHASE_CURRENT.md`**: Reihenfolge der Reichsplanung. **Ergänzt** als 🧭 in `Game/Spielwelt-und-Untergrund.md` §15 (Weltbibel vor Karte).
- **`WORLD_1500_CURRENT_REQUIREMENTS.md`** (Megacity 70–73 km², zwei Großstädte 20–25 km², sieben Makroregionen, Flughafen außerhalb, Final-Map-Gate): Größenwerte **historisch** durch N-01 (08.11.2026); Makrostruktur und Gate belegt in `Game/World/06`, `Game/World/03`, `Game/Spielwelt-und-Untergrund.md` §1 und §2.
- **`WORLD_700_CURRENT_STATUS.md`** (Hülle 733,8 km², Spielfläche 690,9 km², OWP-LCS-1, 12K-Master, elf Regionen, gesperrte Makrogeografie, additive Inhalte): belegt in `Game/World/24` §11 und `Game/World/02`.
- **`MASTER_BLOCK_INDEX_30.md`** (30 Planungsblöcke): Planungsstruktur, keine Anforderung. Die Pflicht zur vollständigen räumlichen Zuordnung vor der Kartenerzeugung ist in `Game/World/08` und im Final-Map-Gate (`World/17` §6) belegt.
- **`WORLD_DESIGN_BLOCKS_8_TO_9_CURRENT.md`** (8.970 Bytes, rund 70 Einzelregeln): **Stichprobe** von rund 25 Regeln an den Zielstellen geprüft; die übrigen Regeln sind nur über Themen-Suche abgedeckt und **nicht** zeilenweise bestätigt. Belegt in `Game/NPCs/*`, `Game/Ermittlung/09, 12, 14, 16, 17`, `Game/Kreaturen/08, 09, 11, 12, 16, 21`, `Game/Spielvision.md`, `Game/Spielwelt-und-Untergrund.md`, `Game/Gameplay-Systeme.md`. Die Wirtschaftsregeln (Börse, Krypto, Wetten) stehen in `Gameplay-Systeme` §13; die offene Frage zu Preis- und Liquiditätsmodell ist Q-OD-54. Ergebnis der Stichprobe: keine fehlende Regel außer Börse und Wetten (ergänzt). Monster-Kernzahl, Realmzeit und Auslösendes Ereignis bleiben offen.
- **`WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT.md`** (12.802 Bytes, Blöcke 1–7 plus Nachträge): **Stichprobe** von rund 25 Regeln an den Zielstellen (Gesellschaft `World/06`, `12`; Wirtschaft und Wetten `Gameplay-Systeme` §13; Gebäude und Immobilien `Spielwelt` §12; Fahrzeuge `Gameplay-Systeme` §6; Natur `Spielwelt` §8; Sport und Freizeit `Spielwelt` §10; Freizeitparks `Spielwelt` §10). Keine fehlende Regel gefunden. Nicht zeilenweise bestätigt.
- **`08_PREVIOUS_SNAPSHOTS/WORLD_DESIGN_MASTER_SNAPSHOT.md`** (10.571 Bytes): **Vorfassung**; alle 131 nichtleeren Zeilen sind im Blocks-1–7-Dokument enthalten (Zeilenmengenvergleich). Keine eigene Prüfung nötig.
- **`SPN_Master_Project_State_…_BASELINE`** (= `PROJECT_STATE_2026-08-31_COMPACT.md`, byte-identisch): **Teilprüfung** (Abschnitte 68, 72, 74, 79 zeilengenau geprüft: Sprung, Luftsteuerung, Ego-Perspektive, Fotomodus, Detektiv-Regeln belegt; **DLC-Grundsatz neu** als 🧭 in `Spielvision` §Trophäen). Die übrigen 81 Abschnitte sind thematisch durch die Entscheidungsnachweise abgedeckt, aber nicht zeilenweise bestätigt.
- **`00_START_HERE`** (7 Dateien, gelesen): `CURRENT_SUPERSESSION_2026-09-03.md` ist die wichtigste Vorstufe. Welt- und Systemaussagen (WORLD_700 geschlossen, WORLD_1500 Gate, Planungsmethode, neue Systeme) sind belegt; **Lücke geschlossen:** Börse, Aktien, Krypto, 12–18 Buchmacher (Sharp/Semi-Sharp/Soft) und Wettbörse mit Back/Lay standen nicht in `Docs/`. Ergänzt in `Game/Spielwelt-und-Untergrund.md` §11.3 (📋), Preis- und Liquiditätsmodell als **Q-OD-54** offen. `SOURCE_OF_TRUTH_AND_MEMORY_RULE.md` und `DAILY_FULL_ARCHIVE_POLICY*.md` sind Prozessregeln (Archivpflicht, Splitting), sinngemäß in `Planning/README.md` belegt; `KNOWN_EXTERNAL_FILE_LIBRARY_ITEMS.md` (SVG V2 nicht eingebettet) ist im Register als extern geführt.

## Ergebnis

- **Verschiedene Inhalte in `02_EMBEDDED`:** 1.860. Davon sind **1.732 Prompt-Entwürfe** (zwei Versionen zu je 866) und damit keine Anforderungsquelle.
- **Nicht-Entwurf-Inhalte:** 128 (Backlog, Governance, Referenzen, Vorstufen, Manifeste, Sonstige).
- **Ausgeschlossen mit Begründung:** Governance (Prozess), Manifeste (Prüfsummen), überholte Prompt-Pakete.
- **Noch offen:** Vorstufen-Blockdokumente 8–9 und Weltdesign-Snapshot (Zeile für Zeile), `SPN_Prompt_Pack` (22), Prompt-Entwürfe nach Muster und Track (1.732 Inhalte; Track-Kontexte gelesen).
- **Neu übernommen:** Reihenfolge der Reichsplanung (🧭, `Spielwelt` §15); Q-OD-54 (Wett-Preismodell). Alle übrigen geprüften Vorstufenaussagen waren bereits in `Docs/` belegt oder sind durch N-01 bzw. N-02 überholt.

## Phase 3 (MASTER-MIGRATION-18): Prompt-Entwürfe, hash-dedupliziert

- **Verschiedene Prompt-Inhalte:** 1.732 (zwei Versionen: `02_PROMPT_LIBRARY_CURRENT/docs/prompts` und `03_PROMPT_LIBRARY_HISTORY/…v0.1/prompts`). Jeder der **866 Titel** liegt in beiden Versionen vor. Doppelte Kopien (`extracted_v0.5`, `03/docs/prompts`) sind Duplikate.
- **Aufbau:** Jeder Entwurf ist ein Arbeitspaket mit Titel, Status, Ziel, Kontext und „Mandatory requirements“. Die Entwürfe tragen den Vermerk „DRAFT ONLY“.
- **Anforderungsbullets:** automatisch aus den Abschnitten „Mandatory requirements“ extrahiert: **31 verschiedene Zeilen**, alle als Engineering-Regeln oder Spielregeln geprüft. Alle spielbezogenen Regeln sind in `Docs/` belegt (verborgene Beziehungswerte, getrennte Sammelobjekte, Kategorie „Unbekannt“, Detektivmodus ohne Hilfen, Grounded ohne versteckte Hilfen, kein globaler Munitionspool, seltene Querschläger, gleiche Verletzungsregeln, kontextsensitive Steuerung, Grounded-Standard für Abpraller,). Die übrigen sind technische Vorgaben (Architektur, Tests, Versionierung, Plattform-Neutralität), die nicht Spielanforderungen sind.
- **Titel-Klassifikation (grob, Titelmuster, nicht Einzelprüfung):** rund 596 Spielthemen (Feature, Zustand, Regel), rund 270 technische Aufgaben (Werkzeug, Konvention, Schema, Test). Der Begriff „Create …“ ist Umsetzungsauftrag, keine Nutzerentscheidung.
- **Stichprobe von 70 zufällig gezogenen Titeln:** Spielthemen sind in `Docs/` belegt. Feinzustände (z. B. Rückenkontrolle im Bodenkampf, Handgelenkssteuerung) sind nur durch die allgemeinen Nahkampf-Regeln (NK-008 bis NK-011) gedeckt; Umsetzungsdetail. Die Titel „Fall-Wiederholungs-Tags“ und „Case-Repetition-Tag-Tool“ sind als 💡 in `Produktions-Referenzen` §12 aufgenommen.
- **Keine Übernahme** als verbindliche Entscheidung. Prompts sind historische Arbeitsmaterialien.
- **Status:** klassifiziert; Titel-Abgleich auf Stichprobe; nicht formal abgeschlossen.

### SPN_Prompt_Pack (22 Dateien, 2026-08-22)

- Gelesen: `README.md`, `99_Open/Offene_Entscheidungen.md`, `00_Foundation` (3), `01_World` (3), `02_Narrative_Investigation` (4).
- Inhalte gegen Docs geprüft: Vision und Grundregeln (Third Person, Grounded, Roadtrip), Technik (Unreal, C++20, StateTree, MassEntity-Option, Reiche als eigene Maps), IP-Leitplanken, Welt (Gebäudeklassen, Begehbarkeit), Persistenz (20–50 MB, PostgreSQL nur Backend), Fahrzeuge (Kupplung, freie Gangwahl, Controller), Hauptstory (80–100 h, Monster-of-the-Week), Detektivmodus, Hunter Archive („Unbekannt“), NPCs (Zweier-Startteam). Die Entwürfe sind älter; Docs ist aktueller.
- **Offene Liste (`99_Open`):** alle Punkte bleiben offen (Fast Travel, Schnellreise in `Entscheidungsstatus` §5.2; Achievements; Lua; Finale Hardware-Presets).
- **Nicht gelesen:** `03_Combat_Survival` (6), `04_Modding_Ecosystem` (3), `05_Quality` (1). Ihre Themen sind durch Kampfsystem-, Modding- und Testmodule belegt, Wortlaut nicht geprüft.

## Phase 4 (MASTER-MIGRATION-18): `_SOURCE_ARCHIVE`, Textpakete der WORLD_700-Linie

- **143 Textdateien** insgesamt (15 davon Kopien aus `02_EMBEDDED`/`02_WORLD_700`); **128** nur dort. Dateitypen: 56 JSON-Berichte und -Spezifikationen, 28 README/Berichte, 25 Prüfsummen, 15 Markdown, 10 SVG-Dateien, 7 CSV, 2 TXT.
- **README-Inhalte (Stichprobe):** Iteration 8.1 (DEM-Plausibilität, Damm, Hillclimb-Maße, Gondel), Phase B (PASS_WITH_REVIEW_FLAGS, Stable-ID-Erhalt, DEM v2.1), R05A-Pilot. Das sind **technische Geometrie- und Prüfdaten**. Die Geometrie der WORLD_700-Linie ist gesperrt; die Verweise stehen in `World/19` (Versionstabelle) und `World/24` (GIS-Referenz).
- **Ergebnis:** Keine eigenständige Spielanforderung gefunden. Ausgeschlossen mit Begründung: Prüfsummen, JSON-Berichte, SVG-Geometrie. Die Versionsentscheidung zur R05A-Archivierung (03.09.) ist in `World/19` ergänzt.
- **Status:** klassifiziert, Texte nicht zeilenweise geprüft. Formal abgeschlossen nur nach Freigabe, falls Geometrie oder Daten wieder gebraucht werden.

## Phase 3, Nachtrag: Titel-Durchgang (MASTER-MIGRATION-18)

- **Einträge:** 866 eindeutige Titel = **848 Prompt-Titel** + **18 Track-Kontexte** (Titel = Trackname). Das passt zur Angabe „848 legacy prompt files“ in `PACK_VALIDATION.md`. Jeder Titel liegt in zwei Versionen vor (v0.5 und v0.1), daher 1.732 Inhalte.
- **Gelesen:** alle 866 Titel im Wortlaut (Titel-Liste), in Themen geordnet.
- **Einordnung:**
  - **Spielthemen** (Umsetzungsaufträge zu Figuren, Zuständen, Regeln): Kampf (Anatomie, Waffen, Ballistik, Nahkampf, Schwierigkeit), Ermittlung (Fälle, Beweise, Archiv), NPC-Gedächtnis, Fahrzeuge, Persistenz, Karte und Welt, Modding-Schnittstellen, Hilfs- und Wissens-Stufen. Ihre Inhalte sind in den Docs-Modulen dieser Themen belegt (Track-Ebene, Stichprobe von 70 Titeln in der Vorstufe).
  - **Technische Aufgaben** (Konventionen, Schemata, Werkzeuge, Debugger, Tests, Asset-Pipelines, Plattform-Abstraktionen, Rechts-Gates): keine Spielanforderung; Umsetzungsdetail.
- **Nicht belegt:** Jeder Titel ist nicht einzeln in den Docs-Zielstellen belegt. Die Zuordnung ist Themen-Ebene.
- **Status:** klassifiziert, nicht formal abgeschlossen. Die 1.732 Inhalte zählen nicht zu den abgeschlossenen Inhalten.
- **Neu übernommen aus Stichproben:** 💡 Wiederverletzung (`Kampfsystem/13`), 💡 Case-Tag-Werkzeug, 💡 Waffen-/Ballistik-/Verletzungs-Debugger, 💡 Modding-Pflichten (Beispielmods, Dokumentation, Packaging), 🧭 Steam-Verknüpfung nicht Pflicht, Entitlements je Produkt, 🧭 Creator-Inhalte-Kategorien.

## Arbeitspaket C (MASTER-MIGRATION-19): Blöcke 8 und 9, Wort-Zuordnung

Quelle: `01_CURRENT_CANONICAL_STATE/WORLD_DESIGN_BLOCKS_8_TO_9_CURRENT.md` (`02_EMBEDDED`, Vorstufe 04.09.). Die rund 90 Regeln von Block 8 und 9 sind je einer Fundstelle in `Docs/` zugeordnet (Tabelle unten). Für die unsicheren Punkte (Ethnie als Ursache, landesweite Fahndung, Narben und Behinderung, Kulte und Infiltration, Hunter-Aufträge, Tod in Realmen, Dauerfolgen) wurde der Wortlaut der Fundstelle gelesen. Bei den übrigen Punkten bestätigt die Fundstelle den Inhalt über die Themenbezeichnung; ein Wort-für-Wort-Vergleich jeder Formulierung wurde nicht durchgeführt.

| Regel (Kurzform) | Zielstelle in `Docs/` | Befund |
|---|---|---|
| Tages- und Wochenabläufe, Wetter, Feiertage | `Spielwelt` §13 (NPC-Leben) | belegt |
| Wohnung, Obdachlosigkeit | `Spielwelt` §13 | belegt |
| Arbeitsstätten, Geschäfte, Casinos, Skiresorts | `Spielwelt` §13, `NPCs/13` | belegt |
| Familie, Freunde, Kollegen, Beziehungen wandeln sich | `NPCs/09`, `Spielwelt` §13 | belegt |
| Alterung; Generationswechsel als späte Priorität | `NPCs/14` (Alterung); Familiengenerationen offen (`OPEN_QUESTIONS`) | belegt; Generationswechsel offen |
| Umzug, Jobwechsel, Entlassung, Beförderung | `NPCs/14` (Umzug, Beruf) | belegt |
| Persistenzstufen A bis D | `Spielwelt` §13, `NPCs/03` | belegt |
| Gleiche Personen bleiben, bis sie sterben | `NPCs/03`, `Spielvision` §6 | belegt |
| Gedächtnis nur durch Beobachtung oder Kommunikation; Gerüchte verzerrt | `Spielwelt` §13 (Gedächtnis), `NPCs/05` | belegt |
| Wiedererkennung; Kleidung und Verkleidung wirken | `Gameplay-Systeme` §7 (Tarnung), `Ermittlung/13` | belegt |
| Mit fast jedem NPC sprechbar; NPCs untereinander | `Spielvision` §15.1 | belegt |
| Erwachsenen-Rauchen, Alkohol, Drogen | `Gameplay-Systeme` §9, `Kampfsystem/11` | belegt |
| Kein sichtbarer Ruf-Balken | `Spielwelt` §13, `NPCs/08` | belegt |
| Kriminalität unabhängig vom Spieler | `Spielwelt` §13 (Kriminalität) | belegt |
| Ursache: soziale Lage, Netzwerke, Polizeiarbeit; ethnische Zugehörigkeit nicht mechanische Ursache | `Spielwelt` §13 (Kriminalität, Zeile 339) | belegt |
| Zeuge, Schweigegeld, Einschüchterung | `Ermittlung/05` (Zeile 120) | belegt |
| Organisierte Kriminalität ohne Verherrlichung | `Ermittlung/06` (Zeile 58, Richtung) | belegt |
| Spieler kann landesweit gesucht werden | `Ermittlung/14` P9 | belegt |
| Fahndung bleibt bis zur glaubwürdigen Auflösung; kein Timer | `Ermittlung/14` (Abklingen, Zeile 234) | belegt |
| Festnahme ist kein automatisches Game Over | `Ermittlung/16`, `Gameplay-Systeme` §10 | belegt |
| Feuerwehr, Rettung, Suche simuliert | `Spielwelt` §13 (Notdienste) | belegt |
| Vorfälle verursachen dauerhafte Schäden; NPC-Crews reparieren | `Spielwelt` §13 | belegt |
| Menschenmengen bei Stadien, Festivals, Nachtleben | `Spielwelt` §13 (Menschenmengen) | belegt |
| Detailstufen und Rekonstruktion beim Betreten | `Spielwelt` §13 | belegt |
| Fahranfänger würgen Schaltwagen ab; Wetter ändert Verhalten | `Spielwelt` (Verkehr) und `Fahrzeuge/04` | belegt |
| Dauerhafte Verletzungen: Narben, Behinderung, Gliedmaßen | `NPCs/03` (Dauerfolgen: Narben, verlorene Gliedmaßen) | belegt |
| Tod permanent; wichtige NPCs geschützt vor Zufall | `Spielvision` §6 (Tod), `Spielvision` §12 | belegt |
| Gesellschaftlicher Zustand bleibt erhalten (Haushalte, Betriebe) | `Entscheidungsstatus` §5.2 (Haushalte), `Spielwelt` §13 | belegt |
| Übernatürliche Vorfälle ändern Routinen und Gerüchte | `Spielvision` §6, §11 | belegt |
| Realismus und innere Konsistenz als höchste Priorität | `Spielvision` §2.1 | belegt |
| Hunter selten (einmal oder zweimal im Leben) | `Spielvision` (Jäger sind selten) | belegt |
| Lockeres Jägernetzwerk; Depots; Forum und Datenbanken | `Spielvision` (Jägernetzwerk) | belegt |
| Mehrere Jäger- und Okkult-Organisationen; wissenszentrierte Organisation nach Funktion, Original-IP | `Spielvision` (Organisationen), `Ermittlung/17` | belegt |
| Kleine Geheim- und Forschungseinheiten kennen Teile der Wahrheit | `Ermittlung/17` | belegt |
| Übernatürliches öffentlich unbewiesen; Internet voller Fälschungen | `Ermittlung/27` | belegt |
| Kein universelles magisches Gedächtnis-Wipe | `Spielvision` §4 (Welt erinnert sich) | belegt |
| Monster sind keine Spawnpunkte; einzelne Tiere weg, starke Individuen bleiben | `Spielvision` (Monster) | belegt |
| Territorien, Nester, Rudel, Wanderung, Wiederbesiedlung | `Kreaturen/08` | belegt |
| Zerstörte Nester bleiben zerstört | `Kreaturen/08` (Nester) | belegt |
| Lebensraum, Beute, Jahreszeit, Rivalen beeinflussen | `Kreaturen/08` | belegt |
| Übernatürliche Wesen unter menschlichen Identitäten | `Spielvision` (Identitäten), `Kreaturen/09` | belegt |
| Moral variiert; Übernatürliches nicht automatisch böse | `Kreaturen/09` | belegt |
| Verschonen, Handel, Hilfe | `Kreaturen/09` §3 | belegt |
| Artspezifische Kampf-KI; Anpassung; Ausweichen | `Kreaturen/10`, `Kampfsystem/18` | belegt |
| Sehr seltene Feinde gefährden sichere Orte; Verlegung | `Kreaturen/15`, `Ermittlung/09` (Unterschlupf) | belegt |
| Dynamische Hunter-Aufträge, nicht geskriptet | `Ermittlung/07` (Zeile 105), `Ermittlung/09` | belegt |
| Hauptmissionen handgefertigt | `Ermittlung/09` (Vorschlag) und `Spielvision` §6 | belegt |
| Fehlidentifikation und Fehlalarme | `Kreaturen/14`, `Ermittlung/27` | belegt |
| Gefälschte Beweise und Tarnung durch Menschen | `Ermittlung/15` (Zeile 149), `Ermittlung/09` | belegt |
| Spieler startet mit wenig Wissen; auslösendes Ereignis offen | `Entscheidungsstatus` §5.1 | belegt |
| Archiv-Wissen verdient; alte Lore kann falsch sein | `Ermittlung/04` | belegt |
| Regionale Varianten, Mutationen, Hierarchien | `Kreaturen/03` (Individuen), `Spielwelt` §9 | belegt |
| Rituale brauchen Vorbereitung, kein Mana; Spieler bleibt Jäger | `Spielvision` (Rituale), `Kreaturen/12` | belegt |
| Verfluchte Objekte persistent, selten; HQ-Vault | `Kreaturen/12` (Zeile 147) | belegt |
| Spukhäuser mit persistenter Geschichte | `Kreaturen/16` §2 | belegt |
| Kulte: verwachsen, rekrutieren, unterwandern | `Kreaturen/11` (Zeile 40, 141) | belegt |
| Geografie: historische Brennpunkte, keine Ley-Linien-Karte | `Spielvision` (Geografie) | belegt |
| Hell, Fegefeuer, Himmel als eigene Open Worlds | `Spielwelt` §15 | belegt |
| Zugang selten, nicht per Schnellreise | `Spielwelt` §15 | belegt |
| Reichsveränderungen bleiben | `Spielwelt` §15 | belegt |
| Zeitdehnung der Hölle; Verhältnis offen | `Spielwelt` §15 und K-21 (Konflikt) | belegt; Konflikt dokumentiert |
| Tod in Realmen: kein Standard-Game-Over | `Kampfsystem/14` (offen), `Spielvision` §14 | belegt |
| Besessenheit, Flüche, Infektion selten mit Folgen | `Spielvision` (Übernatürliches) | belegt |
| Erwachsene: sehr drastische Gewalt; Minderjährige ohne explizite Zerstückelung | `Gameplay-Systeme` (Gore, Minderjährige) | belegt |
| Monsterkader: 20–30 tiefe Kernarten | `Entscheidungsstatus` §3 | belegt |
| Einzelne Individuen mit Alter, Stärke, Verhalten | `Kreaturen/03` | belegt |
| Benannte Einzelwesen mit eigener Geschichte | `Kreaturen/03` (Individuen) | belegt |
| Urform-Wesen mit Original-Terminologie | `Kreaturen/21` (offen, Block 9) | belegt; Namen offen |
| Monster als persistente Wesen, keine Mob-Begegnungen | `Spielvision` (Monster) | belegt |

**Ergebnis C:** Keine Regel aus Block 8 oder 9 fehlt in `Docs/` (Fundstellennachweis). Die Punkte, die offen sind (Generationswechsel, Realmzeit, Tod in Realmen, Urform-Namen, Monsterkader), sind dort als offen geführt. Der Block ist damit vollständig zugeordnet.

## Arbeitspaket D (MASTER-MIGRATION-19): Prompt-Archiv

**Befund zur Textstruktur:** Die 866 Prompt-Inhalte der Version 0.5 wurden normalisiert (Titel durch Platzhalter ersetzt). Danach bleiben nur 13 verschiedene Zeilen außerhalb des Bausteintexts übrig: die 31 Anforderungsbullets und die Track-Kontexte. Kein Prompt enthält Fachinhalt außerhalb seines Titels und der Bausteine. Die Version 0.1 unterscheidet sich von der Version 0.5 nur durch den Entwurfsvermerk und Pfadnamen (Ähnlichkeit im Mittel 0,92; kein Inhalt fehlt). Die 866 Titel sind daher die eigentlichen Inhalte. Das Entwurfsstatus-Feld (`DRAFT ONLY`) gilt für alle.

**Zuordnung:** `Docs/Planning/Nachweis/prompt-archiv-zuordnung.csv` (866 Zeilen: Kennung, Track, Titel, Klasse, Zielstellen in `Docs/`, Themenbereich).

| Klasse | Anzahl | Umgang |
|---|---:|---|
| Spielthema / Umsetzungsdetail | 527 | Fachinhalt ist in den Zielstellen belegt (Themenebene je Track). Titel selbst sind Umsetzungsaufträge, keine Entscheidungen. |
| Technische Aufgabe (Konvention, Schema, Werkzeug, Test, Plattform, Recht) | 321 | Keine Spielanforderung; Umsetzungsdetail. Architektur-Richtlinien stehen in `Produktions-Referenzen` §8, §12. |
| Track-Kontext (Entwurfsrahmen) | 18 | Rahmen, keine Anforderung. |

**Anforderungsbullets (31):** alle gegen `Docs/` geprüft. Spielregeln sind belegt; die übrigen sind technische Vorgaben (Architektur, Tests, Versionierung, Plattform-Neutralität, Umgang mit Repository). Keine neue Spielregel nötig.

**Historische Vorschläge nicht als verbindlich übernommen:** Case-Tag-Werkzeug, Waffen-/Ballistik-/Verletzungs-Debugger, Modding-Pflichten (als 💡 in `Produktions-Referenzen` §12 und `Gameplay-Systeme` §15). Wiederverletzung (💡 in `Kampfsystem/13`).

**Status D:** Der Text aller 866 Inhalte ist geprüft (Normalisierung). Der Abgleich der 527 Spielthemen ist Themenebene, nicht Titel für Titel gegen einzelne Sätze. Die Prompts zählen deshalb nicht als abgeschlossene Inhalte.

## Arbeitspaket E (MASTER-MIGRATION-19): Textpakete der `_SOURCE_ARCHIVE`

- Alle 143 eindeutigen Textdateien inhaltlich nach verbindlichen Formulierungen gescannt (`muss`, `Entscheidung`, `bestätigt`, `locked` u. a.).
- Die Treffer sind Methodenregeln der Geometrie-Pipeline (`locked` als Geometriesperre, Phase-A-Abnahme, Pilot-Entscheidungen, Upscaling-Verbot, Stable IDs, POI-Zustände, Web-Projektion). Keine Spielanforderung.
- **Übernommen:** Web-Projektion (fiktiv, isoliert), Renderquelle für tiefe Stufen (DEM und Vektor, nicht PNG) als 🧭 in `World/24` §12. Stabile IDs und POI-Zustände sind bereits in `World/24` belegt; Upscaling-Verbot in `World/19` und `World/17` §6.
- **Nicht übernommen:** alle Geometrie-, Mess- und Validierungswerte (gesperrte Makrogeografie, Prüfberichte, Hashes).
- **Status E:** klassifiziert und inhaltlich gescannt; Methodenregeln übernommen; formal nicht zeilenweise abgeschlossen.
