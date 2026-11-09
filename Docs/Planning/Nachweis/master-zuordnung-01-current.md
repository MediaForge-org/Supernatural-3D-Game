# Master-Zuordnung: 01_CURRENT_CANONICAL_STATE

Stand: 08.11.2026. Zweck: Nachweis je Master-Quelldatei, wohin sie in `Docs/` übertragen wird, wie weit die Übertragung reicht und was geprüft wurde.

**Quelle (read-only):** `SPN_FULL_MASTER_2026-09-27/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/` (57 Markdown-Dateien).

**Lesart der Spalten**

- **Übertragung:** `vollständig` = jede Anforderung der Datei ist in `Docs/` mit Zielstelle nachgewiesen; `teilweise` = Kernaussagen übertragen, Details fehlen; `noch nicht` = nicht übertragen; `historisch` = nur Fortschreibung oder Vorstufe, keine eigenen Anforderungen; `überholt` = durch neuere bestätigte Entscheidung ersetzt; `ungeklärt` = Widerspruch oder offener Punkt.
- **Prüfung:** `Inhalt gelesen` = Datei in dieser Sitzung vollständig gelesen; `Kurzlesung` = Datei gelesen, aber als reine Statusnotiz; `Stichprobe` = Zielstellen nur punktuell per Suche belegt, keine Zeilen-für-Zeilen-Gegenprüfung.
- Ein Dateiname, ein Suchtreffer oder ein erfolgreicher Link-Check gilt nicht als Nachweis. Ein `vollständig` setzt eine Zeilen-Gegenprüfung voraus; **keine Zeile dieses Registers erreicht diesen Stand**.

**Gesamtbefund:** Keine Datei ist bisher als `vollständig` nachgewiesen. Die Realm-Angaben, die Weltgrößen-Historie und die Modding-Grundsätze sind in `Docs/` weitgehend vorhanden, aber nicht Zeile für Zeile geprüft. Die Block-10-Sandbox-Anforderungen (CA5 §4) sind nur stichprobenartig belegt.

## A. Inhaltlich tragende Dateien

| Master-Datei | Inhalt (Kern) | Ziel unter Docs | Übertragung | Prüfung |
|---|---|---|---|---|
| `CANONICAL_ADDENDUM_2026-09-04.md` | WORLD_700 geschlossen; Hauptwelt nicht gelockt (1.500/1.650/1.800 km²); Realme als eigene Tracks; Sammelobjekt-Korrektur (1.200–1.500 physisch = alte ~200 km²; ~5.000 gesamt) | `Game/World/02`, `03`, `Game/Spielwelt-und-Untergrund.md` §15 | teilweise | Inhalt gelesen; Stichprobe |
| `CANONICAL_ADDENDUM_2026-09-05.md` (CA5) | Hauptwelt 1.500–1.800 km² (historisch, durch N-01 überholt); WORLD_700 Legacy; Zeitreise ~1600–heute, keine harte Untergrenze, Historie aus Ebenen statt Jahreskarten; Historische Übernatürliche Schicht inkl. WWII-Schicht (original IP); Block 10 Sozialsandbox; Realme Hölle/Fegefeuer/Himmel mit Größen und Kandidaten; Sammelobjekte; Offenes | `Game/World/26` §1–§5, `Game/Spielwelt-und-Untergrund.md` §15, `Game/Spielvision.md`, `Game/Gameplay-Systeme.md`, `Game/World/02` | teilweise | Inhalt gelesen; Stichprobe |
| `CANONICAL_ADDENDUM_2026-09-06.md` | Hauptwelt Standard 1.500–1.800; WORLD_700 nicht DLC; Realme-Ortskandidaten; Realm-Traversal und Persistenz; Zeitreise-Kanon | `Game/World/26` §3, `Game/Spielwelt-und-Untergrund.md` §15, `Game/World/02` | teilweise | Inhalt gelesen; Stichprobe |
| `CANONICAL_UPDATE_2026-09-08.md` | Supersession: 1.500–1.800 Standard; WORLD_700 Legacy; Realm-Größen und Ortslisten nicht gelockt; Realmgröße ≠ Entwicklungsaufwand | `Game/World/26`, `Game/World/02`, `Planning/DECISIONS.md` | teilweise | Inhalt gelesen; Stichprobe |
| `CURRENT_CANON_2026-09-16.md` | Vollständiger Kanon: WORLD_700 geschlossen; WORLD_1500 Gate offen, ~2.000 km² nur weicher Zielwert; Unreal + C++20; Modding Provider-unabhängig, `<GameRoot>/mods/`, Block 5 bestätigt, Block 10 abgeschlossen; Sprachproduktion mit Einwilligung; Produktionsprompts mit Referenzen | `Planning/CANON.md`, `Game/World/02`, `Game/Spielwelt-und-Untergrund.md`, Modding-Dokumente (siehe B) | teilweise | Inhalt gelesen; Stichprobe |
| `MASTER_STATUS_2026-10-13.md` | Produktionsstrategie (KI-first, 8–15 h/Woche, Phasen, Labels `[VS]/[EARLY]/[LATER]/[FUTURE/AI]`); Weltzielwert 2.000–2.200 km² als offener Planungsbereich; NYC/Chicago-Referenzen; Hunter-Bunker mit Tesla (Geheimhaltung) | `Planning/CANON.md`, `Game/Entscheidungsstatus.md`, `Game/World/02`, `Game/Ermittlung/17`, `Game/World/20` | teilweise | Inhalt gelesen; Stichprobe |
| `REALMS/STATUS_2026-09-10.md` | Drei Realme als Minimum; Größenbereiche Hölle 350–500 (Favorit 420), Fegefeuer 500–750 (650), Himmel 250–400 (320) plus Erinnerungsdomänen; Schwerpunkte je Realm; Zeitverhältnisse offen | `Game/Spielwelt-und-Untergrund.md` §15, `Game/World/26` §3, `Game/Kreaturen/07` | teilweise | Inhalt gelesen; Stichprobe |
| `BLOCK_09_STATUS.md` | Block 9 im Wesentlichen akzeptiert: seltene Jägernetzwerke, mehrere Organisationen, persistente Monsterökologie, Individual-Tod endgültig, Territorien/Nester/Migration, intelligente Übernatürliche NPCs; offen: Originalnamen, Realm-Kosmologie/Zeitverhältnis, Tod-Ablauf, Monsteranzahl | `Game/Kreaturen/*`, `Game/World/26`, `Game/Kreaturen/20`, `Game/Kreaturen/21` | teilweise | Kurzlesung |
| `MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md` | Block 5 bestätigt: Steam Workshop nicht Pflicht; Kern hängt von keinem Anbieter ab; `<GameRoot>/mods/` als Standard; `.spnmod` und entpackte Verzeichnisse; Installation getrennt von Aktivierung; Profile und Speicherstand; Zusatzbibliotheken optional | `Game/Gameplay-Systeme.md` (Mods), `Game/NPCs/23-Koop-Host-Speicherstand-und-Mods.md`, `Game/Adult/28-Spezifikation-Speicherung-Mods-Builds.md` (Zuordnung prüfen) | teilweise | Inhalt gelesen; Stichprobe |
| `DISTRIBUTION_AND_OPTIONAL_PACKS/OPTIONAL_CONTENT_PACKS_2026-09-10.md` | Standardinstallation mit komprimierter Stimmversion; optionale Studio-Stimmpakete (48 kHz, nicht 96/192); Sprachpakete einzeln; Ultra/Cinematic-Texturen optional, Größen- und VRAM-Hinweis | `Game/Spielvision.md` (Sprachpakete), `Planning/OPEN_QUESTIONS.md` | teilweise | Inhalt gelesen; Stichprobe |
| `VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10.md` | Freiwillige Stimmreferenzen mit Einwilligung, kommerziellen Rechten, Nutzungsumfang, Aufbewahrung und Löschung; externe Referenzen für Blender-/Asset-Prompts | `Game/Spielvision.md`, `Planning/CANON.md`, `Planning/DECISIONS.md` | teilweise | Inhalt gelesen; Stichprobe. **Produktionsprompt-Referenzen: noch nicht übertragen.** |

## B. Fortschreibungen und Supersession-Dateien

| Master-Datei | Inhalt | Ziel / Bewertung | Übertragung | Prüfung |
|---|---|---|---|---|
| `MASTER_STATUS_2026-09-10.md` | Rekonstruktion Modding 09-09; optionale Pakete; Blender-Referenzworkflow | Fortschreibung von oben | historisch | Kurzlesung |
| `MASTER_STATUS_2026-09-11.md` | Modding Block 5 integriert; Heaven/Hell/Purgatory weiter vorläufig; Archivpolitik | Fortschreibung | historisch | Kurzlesung |
| `MASTER_STATUS_2026-09-17.md` … `2026-09-26.md` (10 Dateien) | Tägliche Fortschreibung „keine neue Entscheidung“; WORLD_700-Technikkarten (09-23) als eingebettete Artefakte; Hinweis, dass Roman-Projekt getrennt ist | Keine eigenen Anforderungen. Die WORLD_700-Artefakte sind nicht in `Docs/` übertragen (Binär-/GIS-Material, siehe C). | historisch | Kurzlesung (alle 10 gelesen) |
| `MASTER_STATUS_2026-09-27.md` | Vollständiger Master; Archivteile „EXTERNAL / NOT EMBEDDED“ | Fortschreibung; Archivstatus in `Planning/SOURCES.md` | historisch | Kurzlesung |
| `PLANNING_BLOCK_STATUS_2026-09-16.md` | Gates: WORLD_700 geschlossen; WORLD_1500 Gate offen; ~2.000 soft; Modding Block 5 bestätigt, Block 10 abgeschlossen; Realm-Bibles offen; Host offen; Prompt-System unvollständig | `Planning/CANON.md`, `Planning/TASKS.md` | teilweise | Kurzlesung |
| `PLANNING_STATUS_2026-09-05.md` | Block 9 akzeptiert; Block 10 (Beziehungen, Begleiter, Hund, Tarnung, Spuren, Forensik, Verletzungen, Inventar, Smartphone, Feiertage, Urbex, Auktionen) akzeptiert; Zeitreise-Anker 1600 | `Game/Spielvision.md`, `Game/Gameplay-Systeme.md`, `Game/World/26` | teilweise | Kurzlesung. **Block-10-Gegenprüfung offen.** |
| `PLANNING_STATUS_2026-09-06.md` | Gleicher Stand wie CA6 | Siehe oben | teilweise | Kurzlesung |
| `PLANNING_STATUS_2026-09-07.md` | Stand 09-06 fortgeschrieben; Artefakte WORLD_700 403 (nicht eingebettet) | Fortschreibung | historisch | Kurzlesung |
| `SOURCE_OF_TRUTH_AND_SUPERSESSION.md` (aktuell) | Supersession-Regeln 1–10 (Entwicklerentscheidungen zuerst; WORLD_700 nicht DLC; Hauptwelt 1.500–1.800 damals; Realme nicht gelockt; keine harte Zeitgrenze; Fiktion ≠ Geschichte) | `Planning/README.md` (Leseregel), `Planning/DECISIONS.md` | teilweise (Regeln als Leseregel in `README.md` übernommen; Einzelfolgen nicht geprüft) | Inhalt gelesen |
| `SOURCE_OF_TRUTH_AND_SUPERSESSION_2026-09-10/11/16/17/18/19/20/21/22/23/24/25/26/27.md` (14 Dateien) | Tagesfortschreibungen der Supersession-Regeln | Siehe aktuelle Fassung; Tagesfassungen sind historisch | historisch | 09-10, 09-11, 09-16, 09-27 gelesen; übrige Kurzlesung. Die Regelinhalte wiederholen sich. |
| `WORLD_1500/STATUS_2026-09-10.md` | WORLD_1500 bleibt Planungs-Track; Generierung blockiert bis Anforderungen und räumliche Zuordnung fertig sind | `Game/World/02`, `Planning/DECISIONS.md` (N-01) | teilweise | Inhalt gelesen |
| `WORLD_700/STATUS_2026-09-10.md` | WORLD_700 geografisch und visuell abgeschlossen; additive Integration; kein DLC | `Game/World/02`, `Game/World/03`, `Game/World/04` | teilweise | Inhalt gelesen |
| `MODDING/MODDING_BLOCK_STATUS_2026-09-11.md` | Block 5 bestätigt; Block 10 abgeschlossen | `Game/Gameplay-Systeme.md` | teilweise | Inhalt gelesen |
| `MODDING/MODDING_ARCHITECTURE_V1_RECONSTRUCTED_2026-09-09.md` | Rekonstruierter Stand 09-09, durch Block 5 ersetzt | Historie; Block-5-Regeln gelten | historisch | Kurzlesung |

## C. Nicht spielbezogene oder nicht übertragbare Dateien

| Master-Datei | Bewertung |
|---|---|
| `99_…`-Quellen (in dieser Liste nicht enthalten) | Siehe `Planning/SOURCES.md`; privater Inhalt darf nicht übertragen werden. |
| `_SOURCE_ARCHIVE` / WORLD_700-Karten (PNG, SVG, GIS, ZIP) | Binär- und GIS-Material. Nicht in `Docs/`; bleibt im Archiv (siehe `Planning/SOURCES.md`). |
| Hinweis: 2026-09-27-Archivteile | „EXTERNAL / NOT EMBEDDED“ laut Master selbst. Inhalt nicht prüfbar. |

## D. Konflikte und offene Punkte aus dieser Gegenprüfung

1. **Weltgröße.** CA5, CA6, CU8, CC-09-16 und MS-10-13 nennen 1.500–1.800 km² bzw. ~2.000–2.200 km². Die Entwicklerentscheidung N-01 (08.11.2026) hat beides als Verbindlichkeit aufgehoben: Die Weltgröße ist offen. Eintrag: `Planning/DECISIONS.md` und `Planning/CONFLICTS.md`, K-12 / K-20.
2. **Bunker und Tesla.** MS-10-13 setzt Tesla als im Hunter-Bunker lebend und geheim. `Docs/Game/Entscheidungsstatus.md` führt den Bunker als Feldbasis noch als offen (K-28). Beide Aussagen sind getrennt zu behandeln: Die Existenz eines Verstecks ist nicht dasselbe wie seine Funktion als Basis. **Ungeklärt, Entwicklerfrage nötig.**
3. **Zeitdehnung in der Hölle.** CA5 §5 und REALMS: Verhältnis offen. Docs `Spielwelt-und-Untergrund.md` §15 nennt „40:4 bestätigt 22.10.“ (PE-E1022). `Planning/CONFLICTS.md` K-21 führt dies als offen. **Widerspruch bleibt bestehen, nicht eigenmächtig aufgelöst.**
4. **Freie Energie vor 1775.** Master: nicht als reale Geschichte übernommen; nur als fiktive Schicht. Docs: `Game/World/10` und `20` markieren die Passage als historisch. Übertragung vorhanden, Prüfung offen.
5. **Produktionsprompts (Blender, Assets).** Als Richtung übertragen in `Docs/Development/Produktions-Referenzen-und-Prompts.md`. Die Auswahl konkreter Tutorials und das Prompt-System bleiben offen.
6. **Block-10-Sandbox (CA5 §4).** Viele Einzelpunkte (Hund, Tarnung, Spuren, Forensik, Verletzungen, Schlaf/Alkohol/Drogen, Kleidung, Smartphone, Feiertage, Urbex, Auktionen, Beziehungen mit Ehe/Gemeinschaftsbesitz). Stichproben belegen Vorkommen; eine vollständige Gegenprüfung fehlt. Die Zeilen GP-BALLISTIK-002 und GP-KOERPER-002 sind bereits als nicht auffindbar gekennzeichnet (siehe `Planning/Nachweis/gameplay.md`).
7. **Sammelobjekte.** Historische Korrektur (1.200–1.500 physisch = alte ~200 km²) ist in `Docs/Game/Spielvision.md` SV:243 übernommen.

## E. Abschnittsweise Nachweise (Stand 08.11.2026, Sitzung „Inhaltsmigration“)

Jeder Eintrag nennt die Quelle, das Zielabschnitt und den Stand. Die Prüfung erfolgte **punktweise** (jede Regel einzeln gesucht und im Ziel gelesen), nicht Zeile für Zeile über den ganzen Abschnitt. Deshalb gilt die Quelle insgesamt weiterhin als **teilweise**, außer wo ausdrücklich `abgeschlossen` steht.

| Quelle | Abschnitt | Zielstellen | Stand | Ausnahmen / offen |
|---|---|---|---|---|
| CA5 §5 Realme | Größen, Favoriten, Orte, Bewegung, Persistenz | `Game/Spielwelt-und-Untergrund.md` §15; `Game/World/26` §2–§4 | abgeschlossen (punktweise) | Zeitverhältnis Hölle: Docs nennt Bestätigung 22.10. (PE-E1022), K-21 führt es offen. Widerspruch bleibt bestehen. |
| CA5 §5 / CA6 Realme | Zeitreise, Gedächtnisdomänen, Traversal | `Game/World/26` §4–§5, §7 | abgeschlossen (punktweise) | Architektur der Gedächtnisdomänen offen (World/26 §7). |
| CA5 §2 Zeitreise | Jahresauflösung, Schichten, keine harte Untergrenze, Freie-Energie-Passage | `Game/World/26` §5 (ergänzt 08.11.); `Game/World/10` (historisch) | abgeschlossen (punktweise) | Geografie-Konflikt K-33 offen. |
| CA5 §3 Historische Schicht | mehrhundertjährige Geschichte, WWII-Ära, Erschaffene Wesen und Wächter, Original-IP | `Game/Spielvision.md` Ton/Hintergrund (ergänzt 08.11.); `Game/Kreaturen/11` | abgeschlossen (punktweise) | Progenitor-/Erste-Art-Konzept und Institutionsname: offen, Eintrag Q-K32 in `Game/Kreaturen/21`. |
| CA5 §4 Block 10 Begleiter | Mitstreiter, Tod, Hund, Solo | `Game/Spielvision.md` §6.3; `Game/NPCs/19` N-CO-007 bis N-CO-015 | abgeschlossen (punktweise) | Hund-Details sind in NPCs/19 N-CO-014 vollständig, in §6.3 nur als Merkmalsliste. |
| CA5 §4 Block 10 Tarnung, Spuren, Forensik | Stealth, Tracking, Labor | `Game/Gameplay-Systeme.md` §7, §8; `Game/Ermittlung/05`, `11` | abgeschlossen (punktweise) | – |
| CA5 §4 Block 10 Verletzung, Körper, Alltag | Blutverlust, Behandlung, Schlaf, Alkohol, Kleidung, Rehabilitation | `Game/Gameplay-Systeme.md` §8, Zeilen 239–243, 402–403; `Game/Kampfsystem/13` | teilweise | **Amputation/Prothesen:** Gameplay-Systeme nennt sie offen mit Gore-Ergebnis-Option (K-22); `Kampfsystem/27` COMBAT-DEC-VL-012 führt die Master-Vorgabe (nur Story). Der Konflikt K-22 bleibt offen; Statusvermerk in Kampfsystem/27 korrigiert. |
| CA5 §4 Block 10 Smartphone, Feiertage, Urbex, Auktionen | Smartphone-Ökosystem, Funklöcher, Satellit, Kalender, Sandbox-Schichten | `Game/Gameplay-Systeme.md` §11; `Game/Spielvision.md`; `Game/NPCs/07` N-IN-016 | abgeschlossen (punktweise) | – |
| CA5 §4 Block 10 Beziehungen | Freundschaft, Vertrauen, Zusammenleben, Heirat, gemeinsames Eigentum, Familiengenerationen | `Game/NPCs/09` N-BV-009; `Game/Gameplay-Systeme.md` Z. 398; `Planning/DECISIONS.md` C-23 | abgeschlossen (punktweise) | Familiengenerationen: niedrige Priorität, offen (Gameplay-Systeme Z. 398). |
| CA5 §4 Block 10 Sandbox-Prinzip | keine künstlichen Klassensperren | `Game/Ermittlung/07` Z. 31; `Ermittlung/24` D10 | abgeschlossen (punktweise) | – |
| CA5 §4 Block 10 Versorgung | HQ-Vorrat ohne Mikromanagement | `Game/Spielvision.md` Z. 183; `NPCs/19` N-CO-020 | teilweise | Die Regel „keine Mikro-Munitionsverwaltung bei gefülltem HQ“ ist nur als Versorgungsgrundsatz belegt. |
| CA5 §6 Sammelobjekte | 1.200–1.500 physisch = alte ~200 km² | `Game/Spielvision.md` SV:243 | abgeschlossen (punktweise) | Neue Gesamtsumme offen. |
| CA5 §7 Offene Liste | Realm-Namen, Kosmologie, Tod, Zukunftsreise, Familie | `Game/World/26` §7; `Game/Spielwelt-und-Untergrund.md` §15 | abgeschlossen | – |
| Block 9 Status | Säulen (Jägernetz, Ökologie, Todes-Permanenz, Territorien, intelligente Wesen, Artspezifische KI, öffentliche Mehrdeutigkeit) | `Game/Spielvision.md` Z. 97, 273, 287; `Game/Kreaturen/11` | teilweise | Progenitor-Konzept und Institutionsname offen (Q-K32). |
| Modding Block 5 | Anbieterunabhängigkeit, `<GameRoot>/mods/`, .spnmod, Aktivierung pro Profil/Speicherstand, Zusatzbibliotheken, Manifest-Abbildung | `Game/Gameplay-Systeme.md` Z. 345–348 und Abschnitt „Mod-Laden“ (Z. 408–411); `Game/NPCs/23` N-KO-012 | abgeschlossen (punktweise) | Mod-Manifest-Felder sind 💡 Vorschlag (W-08), kein Master-Nachweis. |
| CC-09-16 / VP-09-10 Produktionsprompts | externe Tutorials, Szenenreferenzen | `Docs/Development/Produktions-Referenzen-und-Prompts.md` (neu, 08.11.) | teilweise | Konkrete Tutorials, Promptstruktur und Zuständigkeit sind offen (Entwicklerentscheidung). Die Prompt-Bibliothek ist Entwurf. |
| VP-09-10 Stimmen | Referenzaufnahmen mit Einwilligung | `Game/Spielvision.md` Z. 401–402, 449; `Planning/DECISIONS.md` C-40 | abgeschlossen (punktweise) | – |

**Fortsetzungspunkt:** Die Block-10-Zeile „Verletzung, Körper, Alltag“ und die Produktions-Zeile sind teilweise. Danach die Block-9-Reste (Q-K32) und die Zeilen-Gegenprüfung von CA5 §4 Block 10 (Volltext), dann die 02_WORLD_700- und 99_CANON_AUDITS-Dateien.

## F. Verzeichnis 02_WORLD_700 (geprüft 08.11.2026)

Quelle: `SPN_FULL_CUMULATIVE_MASTER_2026-09-27/02_WORLD_700/`. Zwölf Dateien: zwei Raster-PNG (rund 105 MB je Iteration 5 und 6), ein Vorschaubild (rund 7,7 MB), vier Textdateien mit Methodik und Status, ein Overlay (SVG), zwei JSON-Manifeste, eine Prüfsummendatei und der Wiederherstellungsstatus.

| Quelle | Zielstelle in `Docs/` | Status | Prüfung |
|---|---|---|---|
| `README_iteration_7_technical_map.md` (Koordinatensystem, Transformation, Zoomstufen, Kennungen, Genauigkeitsgrenzen, Verteilung) | `Game/World/24` §11.1–§11.5 | abgeschlossen (08.11.2026) | Zeile für Zeile gegen Quelle |
| `open_world_map_iteration_6_layer_manifest.json` (13 Gruppen, Anzeige-Hierarchie, Produktionshinweise) | `Game/World/24` §11.10 | abgeschlossen | vollständig gelesen |
| `open_world_map_iteration_6_overlay.svg` | Gruppenstruktur in §11.10; Geometrie nicht übertragen (technische Vektordaten, im Archiv belassen) | abgeschlossen als Binär-/Geometrie-Ausnahme | Struktur geprüft, Koordinaten nicht übernommen |
| `open_world_multiresolution_phase_a/docs/phase_a_methodology.md` | `Game/World/24` §11.6 (Abnahmeschranken, Überlappung, Pilot, kein Upscaling) und `Game/World/17` (Phase-A-Freigabe, K-19) | abgeschlossen | vollständig gelesen |
| `…/docs/README.md` | `Game/World/24` §11.6, §11.8 | abgeschlossen | vollständig gelesen |
| `…/manifests/MAP_RENDER_STYLE_V1.json` | `Game/World/24` §11.7 (Vorschlag, nicht gesperrt) | abgeschlossen | vollständig gelesen |
| `…/manifests/regional_map_manifest.json` (9-Regionen-Vorschlag, Begrenzungen) | Konflikt K-19 dokumentiert (§11.6); Begrenzungen als Referenz im Archiv belassen, nicht übertragen | offener Konflikt mit Originalquelle | Struktur geprüft; Freigabe ungeklärt |
| `…/RECOVERY_STATUS.md` (abgeschnittenes Paket, drei nicht wiederhergestellte Dateien) | `Game/World/24` §11.8 | abgeschlossen | vollständig gelesen |
| `…/previews/regional_segmentation_preview.png` (7,7 MB) | nicht übertragbar (Binär); SHA256 in §11.8 | abgeschlossen als Binär-Ausnahme | Hash belegt |
| `open_world_map_iteration_5_master_12k.png`, `…iteration_6_master_12k.png` (rund 105 MB je) | nicht übertragbar (Binär); Maße in §11.1 | abgeschlossen als Binär-Ausnahme | Maße belegt |
| `…/docs/SHA256SUMS.txt` | Prüfsummen; für die Herkunftsprüfung in §11.8 genannt | abgeschlossen | Hashes gelesen |

**Ergebnis:** Alle Textinhalte sind übertragen oder mit Konfliktvermerk dokumentiert. Die drei Phase-A-Dateien sind nicht wiederherstellbar und werden nicht durch spätere Fassungen ersetzt. Der Widerspruch Phase A (Vorschlag im Master, Freigabe im Register W-TE-008) bleibt offen (K-19).

## G. Verzeichnis 99_CANON_AUDITS (geprüft 08.11.2026)

Quelle: `99_CANON_AUDITS/2026-10-15/`. Es handelt sich um einen Prüfbericht mit Nachweisen, **nicht** um Kanon. Der Bericht selbst bestätigt das („nicht als Canon freigegeben“).

| Quelle | Inhalt | Zielstelle / Behandlung | Status | Prüfung |
|---|---|---|---|---|
| `00_START_HERE/README.md`, `01_SOURCE_COVERAGE.md` | Abdeckung: 56 vorgefilterte Gespräche von 521; neun Master-Teile nur als Dateinamen gezählt | Lesestatus in `Docs/Planning/SOURCES.md` §0 (überholt, neu gefasst); Abdeckung als offen geführt | abgeschlossen | gelesen |
| `02_DECISION_REGISTER.md` (14 Einträge) | Siehe Tabelle G.1 | Jeder Eintrag einem Docs-Ziel oder Konflikt zugeordnet | abgeschlossen | gegen Docs abgeglichen |
| `03_CONFLICTS_AND_GAPS.md` (10 Punkte) | Siehe Tabelle G.2 | Jeder Punkt als Konflikt, offen oder erledigt dokumentiert | abgeschlossen | gelesen |
| `04_MASTER_INVENTORY.md` | Zählung der neun ZIP-Teile (Mitglieder, MiB) | Keine Inhaltsübertragung; Zählung in `SOURCES.md` §2 | abgeschlossen (Metadaten) | gelesen |
| `05_NEXT_ACTIONS.md` | Arbeitsaufträge (Gespräche filtern, Manifeste prüfen, Vollexport beschaffen, Entscheidungen einholen) | Prozessaufträge, keine Anforderungen. Der Vollexport der 521 Gespräche ist **extern nicht verfügbar**; das wird in `SOURCES.md` vermerkt. | extern referenziert, nicht verfügbar | gelesen |
| `CONSOLIDATION_REVIEW/00_README.md`, `03_CONSOLIDATED_STATUS.md` | Konsolidierter Review-Stand (15.10.) | Abgeglichen mit `Docs/Game/World/20` und `Planning/DECISIONS.md`; Untergrundteile durch N-02 historisch | abgeschlossen | gelesen |
| `CONSOLIDATION_REVIEW/04_CONFLICTS_REVIEW.md` (CRITICAL-01 bis MEDIUM-07) | Siehe Tabelle G.2 | als Konflikt oder historisch geführt | abgeschlossen | gelesen |
| `CONSOLIDATION_REVIEW/02_EVIDENCE_INDEX.md` (35 KB) | Stichwortbasierter Index aus Nutzerbeiträgen, keine Canon-Freigabe | Nachweisindex; nicht übertragen, weil er Gesprächsauszüge enthält | Nachweismaterial, nicht Anforderung | Kopf und Gesprächsliste gelesen |
| `CONSOLIDATION_REVIEW/01_CHRONOLOGICAL_EVIDENCE.jsonl` (499 KB) | Volltext der erfassten Nutzerbeiträge | nicht übertragen; enthält private Gesprächsinhalte und ist Nachweismaterial | Nachweismaterial, nicht Anforderung | nur Struktur geprüft |
| `CONSOLIDATION_REVIEW/REFERENCE_CANON_2026-09-27/*` (3 Dateien) | Kopien von `CURRENT_CANON_2026-09-16`, `MASTER_STATUS_2026-09-27`, `SOURCE_OF_TRUTH_…_2026-09-27` | **Duplikate**: byte-identisch mit den Primärquellen in `01_CURRENT` (cmp geprüft) | historische Wiederholung | Vergleich abgeschlossen |
| `CONSOLIDATION_REVIEW/MANIFEST_SHA256.json`, `SHA256SUMS` | Prüfsummen | Herkunftsnachweis, nicht übertragen | abgeschlossen | gelesen |

**G.1 Entscheidungsregister (02) gegen Docs**

| Thema (Register) | Stand im Register | Docs-Ziel | Bewertung |
|---|---|---|---|
| WORLD_700 Makrogeografie | CANON/LOCKED | `World/02`, `24` | übereinstimmend |
| WORLD_1500 Generierung | BLOCKED | `World/02`, `17`; N-01 | übereinstimmend |
| Große Weltfläche | PROVISIONAL (~2.000 / ~2.200) | `World/02` (2.200 kein Richtwert), N-01 | durch N-01 offen; Register verweist noch auf 2.200 als Szenario |
| Metropole 20–25 km², Städte 8–10 km² | PROVISIONAL | `World/03`, `Spielwelt-und-Untergrund.md` | übereinstimmend (Testwerte) |
| Stadtidentität NYC/Chicago | DIRECTION | `World/03`, `World/06` | übereinstimmend |
| Untergrund-Erbauer (CANON) | historisch | `World/10`, §14 (N-02) | **überholt**: N-02 entfernt die vollständige unterirdische Zivilisation |
| Untergrund-Bevölkerung (OPEN) | OPEN | `World/10` historisch, K-29 | offen |
| Größte Untergrundstadt (OPEN) | OPEN | N-02, K-29 | historisch; Konflikt CRITICAL-01 ist durch N-02 gegenstandslos |
| Familienschlüssel-Ursprung (RESERVED) | RESERVED | `Planning/DECISIONS.md` C-44 | übereinstimmend |
| Bunkerentdeckung über Schlüssel (CANON) | CANON | C-44; `World/20` | übereinstimmend; Bunker als Basis K-28 offen |
| Hunter-Stadt-Beziehung (CANON-Rahmen) | Allianz, Gegenseitigkeit | `World/10` historisch | **überholt** (Untergrund entfällt); Restgeltung für Bunker offen (K-29) |
| Tesla (CANON-Rahmen) | im Bunker verborgen | `Ermittlung/17`, `World/20`, MS-10-13 | übereinstimmend |
| AI-first-Produktion | DIRECTION | `Planning/CANON.md`, `Spielvision` | übereinstimmend |
| Story-Koop bis vier | DIRECTION | `Spielvision` | übereinstimmend |

**G.2 Konflikte (03, 04)**

| Punkt | Behandlung in Docs |
|---|---|
| 1. Kartenmaßstab 2.000 vs 2.200 | N-01: Weltgröße offen |
| 2. Stadtflächen 350+100+120 | verworfen (MS-10-13); in `World/03` belegt |
| 3. WORLD_700 vs WORLD_1500 | getrennte Linien (`Spielwelt-und-Untergrund.md` §2) |
| 4. Untergrundbevölkerung und -fläche | historisch (N-02), K-29 offen |
| 5. Familienschlüssel-Ursprung | C-44 reserviert, keine Auflösung erfunden |
| 6. Hunter-Zugang vs feindliche Gesellschaft | historisch (N-02) |
| 7. Gesprächsexport 56 von 521 | nicht vollständig nachweisbar; vermerkt in `SOURCES.md` |
| 8. Große Master-Assets | nur Dateinamen geprüft; vermerkt |
| 9. Keine automatische Canon-Übernahme | eingehalten; keine Änderung an `01_CURRENT` |
| 10. Datumsunsicherheit | vermerkt; keine Daten erfunden |
| CRITICAL-01 bis MEDIUM-07 | entsprechend G.1 und oben; MEDIUM-06 (Produktion vs. technische Locks) und MEDIUM-07 (Schlüssel) beachtet |

**Ergebnis G:** Alle Nachweise und Entscheidungsstände sind eingeordnet. Keine neue Anforderung war zu übertragen; die Untergrund-Einträge sind durch N-02 historisch geworden, ihre Restgeltung bleibt K-29 vorbehalten.

## H. Verzeichnis 99_DAILY_DELTAS (teilweise geprüft, 08.11.2026)

Quelle: `99_DAILY_DELTAS/2026-10-14` bis `2026-10-19`, 102 Dateien (rund 484 KB). Laut Master sind die Tagesdeltas **Prüfvorlagen** („REVIEW REQUIRED — not a verified canonical master merge“). Sie gelten nicht als Kanon; ihre bestätigten Nutzerentscheidungen sind dort einzeln benannt.

**Inhaltlich geprüft und übertragen (vollständig gelesen, Ziel belegt):**

| Delta-Datei | Inhalt | Zielstelle | Status |
|---|---|---|---|
| 2026-10-18 `docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md` | Meer nur im Osten, Gebirge im Norden, weniger Wasser, Seen, Metropole 110 km² (20 Bezirke, 20 km² Downtown mit 12 km² Kern, 3–5 Supertalls), Greywater als einzige Hafenstadt, Hunter-HQ nicht auf öffentlicher Karte, Status VISUAL_NOT_ACCEPTED | `Game/World/02`, `03`, `06`, `05`, `19`, `20` | übertragen; Zeile-für-Zeile-Vergleich offen (Zahlen und Gruppen belegt) |
| 2026-10-19 `docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md` | 2.200 km² als provisorisches Vorab-Ziel; Flächenbilanz; Insel 80–100 km²; Seen; Flachkarte; Hauptfluss; Abnahmestand | `Game/World/02`, `03`, `17`, `19` | übertragen; **Hinweis:** die Weltgröße ist seit N-01 offen; das 2.200-km²-Ziel gilt nur als Vorab-Ziel |
| 2026-10-15 `docs/specs/RAILWAY_SYSTEM_2026-10-15.md` | Anlagenanzahl (Güterknoten 3–4, Terminals 8–10, Depots 4–6, Abstellanlagen 18–26), Elektrifizierung, Dichten (S-Bahn 3–5 Min.), Engineering-Zerlegung (💡) | `Game/World/09` §Schienen | übertragen |
| 2026-10-15 `docs/specs/ROAD_VEHICLES_2026-10-15.md` | Stufen früh/lang, DualSense-Belegung, Handschalt-Variante je Fahrzeug, ungeklärter Schadensentwurf | `Game/Fahrzeuge-und-Verkehr/05`, `03` (VEH-DEC-KL-006), `Game/Gameplay-Systeme.md` Z. 158, `Fahrzeuge-und-Verkehr/28` | übertragen (L2-Doppelbelegung als Prüfplan in `Fahrzeuge-und-Verkehr/05` §3 und VEH-DEC-ST-003 bereits vermerkt) |
| 2026-10-16 `docs/canon/AI_DECISIONS_33_34.md` | U33 verschiedene Entwicklungsstufen, U34 individuelle Anerkennung | `Game/World/12` §3 | übertragen |
| 2026-10-17 `docs/canon/UNDERGROUND_AI_AND_COMMUNICATION_U35_U36.md` | U35 unterschiedliche KI-Regeln; U36 Kommunikationsrichtung | `Game/World/12` §3 | übertragen |
| 2026-10-17 `docs/specs/COMMUNICATION_MODALITIES.md` | Kommunikationsmodalitäten, Architektur (💡), Sicherheitsvorschläge (💡), offene U37-Liste | `Game/World/12` §3 (ergänzt 08.11.2026) | **neu ergänzt** |
| 2026-10-15 `docs/canon/UNDERGROUND_SOCIETY_DECISIONS_24-32.md` | U24–U32 Gesellschaft | `Game/World/12` §1–§2, markiert als historisch (N-02) | übertragen; Geltung offen (K-29) |
| 2026-10-14 `docs/canon/HUNTER_BUNKER_AND_FAMILY_KEY.md`, `UNDERGROUND_CITIES_DECISIONS.md`, `docs/open_questions/…` | Bunker, Tesla, Familienschlüssel, Untergrundstädte | `Planning/DECISIONS.md` C-44, C-47; `Game/World/10`, `11` (historisch N-02); `Game/World/20` | übertragen |

**Noch nicht im Detail geprüft (Status: offen):**

- 2026-10-16: `specs/AI_RECOGNITION_AND_CONFLICTS.md` (Vorschlag, unverbindlich), `specs/AI_SIMULATION_BOUNDARIES.md` (Engineering-Interpretation), `docs/integration/MANUAL_REVIEW_GUIDE.md`, `docs/comparison/DIFF_AGAINST_PRIOR_DELTAS.md`.
- 2026-10-17: `specs/AI_AUTONOMY_BOUNDARIES.md` (U35 Richtung bestätigt, Mechanismen 💡), `specs/PRIVACY_SECURITY_AND_FAILURE_REQUIREMENTS.md`, `design/PROPOSED_CPLUSPLUS_IMPLEMENTATION_NOTES.md` (Nicht-Kanon-Vorschlag, C++-Bezug), `integration/MERGE_REVIEW.md`.
- 2026-10-18: `design/CHARACTER_CREATOR_AND_RELEASE_POLICY.md`, `specs/MASTER_MAP_ACCEPTANCE_SPEC.md` (Anforderungen mit Vorschlägen), `status/CPP_PHASE_0_PROGRESS.md`, `status/MAP_ITERATION_REVIEW.md`, `open_questions/OPEN_QUESTIONS_2026-10-18.md`.
- 2026-10-19: `specs/MAP_ACCEPTANCE_CHECKS_2026-10-19.md` (Prüfliste; Teile in `Game/World/17` D19-CHK), `specs/WORLDFACTS_PHASE_0_3_2026-10-19.md` (C++-Implementierungsbericht, kein Spielinhalt), `design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md`, `design/AI_3D_PIPELINE_WATCH_2026-10-19.md` (Recherche), `docs/status/CPP_GIT_STATUS_2026-10-19.md`, `docs/open_questions/OPEN_QUESTIONS_2026-10-19.md`.
- Alle `assets/`-Inventare, `CHANGELOG.md`- und `README.md`-Dateien sowie `comparison/`-Dateien: nur Metadaten; Inhalte werden über die Originaldeltas oben nachgewiesen.
- `assets/prior_canon_refs/…` (2026-10-18): **Duplikate** von `01_CURRENT`-Dateien. Vergleich mit Primärquelle durch Dateinamen belegt; Byte-Vergleich für `CURRENT_CANON_2026-09-16`, `MASTER_STATUS_2026-09-27`, `SOURCE_OF_TRUTH_…_2026-09-27` in Abschnitt G durchgeführt. Für die übrigen Duplikate (`MASTER_STATUS_2026-10-13`, `CURRENT…ADDENDUM`, `02_DECISION_REGISTER`) ist der Byte-Vergleich offen.

**Ergebnis H:** Neun bestätigte Teilbereiche sind übertragen oder neu ergänzt. Rund 60 Dateien sind Prüf-, Status- und Vorschlagsdokumente; sie bleiben offen und dürfen nicht als bestätigte Anforderungen übernommen werden.

## I. Verzeichnisse 04_HISTORY, 99_RECONSTRUCTION_SOURCES und weitere (nur Klassifikation, 08.11.2026)

| Verzeichnis | Umfang | Einordnung | Behandlung |
|---|---|---|---|
| `04_HISTORY` | 402 Markdown-Dateien, rund 1,7 MB | Historische Fassungen älterer Planungsstände | nicht übertragen; laut Master „historisch“; neuere Fassungen in `01_CURRENT` sind maßgeblich |
| `99_RECONSTRUCTION_SOURCES` | 59 Markdown-Dateien (60 Dateien), rund 23 MB | Rekonstruktionen mit nicht spielbezogenen Gesprächsprotokollen, teils persönlich | **nicht** übertragen; enthält privaten Inhalt. Nur spielrelevante Entscheidungen, die bereits in `Docs/` stehen, zählen. |
| `02_EMBEDDED_BASE_ARCHIVES` | 3.673 Dateien (Prompt-Bibliothek) | Entwürfe (`DRAFT_REWRITE_REQUIRED`) | nicht übertragen; keine Anforderungsquelle |
| `_SOURCE_ARCHIVE`, `_SOURCE_RESEARCH` | 497 Dateien (überwiegend Binär), 3 MP4 | Rohmaterial | nicht übertragen; Archivstatus in `SOURCES.md` |
| `03_EXTERNAL_LIBRARY_REGISTRY`, `06_NEW_REFERENCES`, `00_START_HERE` | 18, 2 und 3 Dateien | Verweise und Registrierung externer Quellen | Verweise in `SOURCES.md`; keine Anforderungen |
| `99_MANIFESTS` | 3 Dateien ohne Markdown | Prüfsummen | nicht übertragen |

**Ergebnis I:** Diese Verzeichnisse sind klassifiziert. Es gibt darin keine Anforderungen, die noch fehlen, soweit die Stichproben reichen. Eine vollständige Prüfung von `04_HISTORY` (402 Dateien) und `99_RECONSTRUCTION_SOURCES` (59 Dateien) steht aus. Sie ist für die Spielplanung nachrangig, weil die Fassungen in `01_CURRENT` maßgeblich sind.
