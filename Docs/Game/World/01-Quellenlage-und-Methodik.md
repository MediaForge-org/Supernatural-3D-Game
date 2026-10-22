# Welt 01: Quellenlage und Methodik

> [!IMPORTANT]
> **Nichts davon ist implementiert, und keine Karte ist freigegeben.** Dieses Modul sagt, **woher** die Welt-Dokumentation stammt, was die Quellen können und was nicht, und nach welchen Regeln Aussagen einen Status bekommen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)). Das Register steht in [Modul 20](20-Entscheidungsregister.md), die Abdeckungsmatrix in [Modul 23](23-Quellenabdeckung-und-Auditbericht.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Zitierschlüssel](#2-zitierschlüssel)
3. [Methodik und Statusregeln](#3-methodik-und-statusregeln)
4. [Chronologie der Quellen](#4-chronologie-der-quellen)
5. [Gewichtung der Quellen](#5-gewichtung-der-quellen)
6. [Die Lücke nach dem 19. Oktober](#6-die-lücke-nach-dem-19-oktober)
7. [Eigenheiten des Chat-Materials](#7-eigenheiten-des-chat-materials)

---

## 1. Befund in Kürze

1. **Die Welt-Planung liegt in drei Schichten:** (a) kurze **Kanon-Dokumente** (September-Nachträge, Oktober-Nachträge), (b) **Tages-Deltas** vom 14. bis 19. Oktober 2026 mit Entwicklerentscheidungen zu Karte, Untergrund, Bahn und Straßenfahrzeugen, (c) **Chat-Transkripte** (Rekonstruktionspaket) mit den ursprünglichen Antworten des Entwicklers.
2. **Der Master trägt „2026-09-27“ im Namen, enthält aber Dokumente bis zum 19. Oktober 2026.** Die Oktober-Entscheidungen zur 2.200-km²-Karte (18./19.10.) sind **vorhanden** (`D18-MAP`, `D19-REQ`). Was **danach** (20.–22.10.) entschieden wurde, enthält der Master nicht ([Abschnitt 6](#6-die-lücke-nach-dem-19-oktober)); Ausnahme sind drei Primärentscheidungen des Projektinhabers vom 22.10.2026 ([Abschnitt 2.5](#25-primärentscheidungen-des-projektinhabers)).
3. **Es gibt genau ein vollständiges GIS-Paket**, und zwar für die **gesperrte Referenzwelt WORLD_700** (GeoPackage mit 495 Features, DEM, Hydrologie). Für die **größere Welt** (2.200-km²-Kandidat) enthält der Master **keine Geometrie**, nur Anforderungen und berichtete Kartenversuche. Die Rohdateien der Kandidaten V3–V6 und der Welt-Blueprints V1.x fehlen.
4. **Die Größen wechseln stark** (Metropolis von 10,5 bis 110 km², je nach Datum und Weltgröße). Jede Zahl trägt in diesem Ordner ihr Datum und ihre Weltgröße; ein Vermischen wird in [Modul 03](03-Massstab-Flaechen-Proportionen.md) und [Modul 21](21-Konflikte-und-Widersprueche.md) ausdrücklich verhindert.
5. **Für Fahrzeuge, Rennsport und Verkehrssimulation** gibt es bestätigte Richtungen (Block 5/7, Straßenfahrzeug- und Bahn-Delta vom 15.10.), aber **keine Fahrzeugliste, keine Konstanten, kein Schadensmodell**.
6. **Für den Untergrund** gibt es bestätigte Lore und Gesellschaftsentscheidungen U24–U36, eine vorläufige Größenspanne (50–100 km²), und viele offene Punkte (U37, Verhältnis Bunker/HQ, Netz zur Oberfläche).

---

## 2. Zitierschlüssel

Alle Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation bleibt ohne Zugriff darauf lesbar; die Schlüssel dienen nur der Nachprüfbarkeit.

### 2.1 Chats

Alle Chats liegen unter `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/`. Schlüssel `<Chat>#<Nachricht><Rolle>`, zum Beispiel `45#61E`; `E` = Rolle „user“ im Transkript (Entwickler), `A` = Rolle „assistant“.

| Schlüssel | Chat | Zeitraum | Nachrichten | Inhalt für die Welt |
|---|---|---|---:|---|
| `23#` | SPN Full Master ZIP | 18.08.–05.09. | 1.109 | Frühe Weltplanung: Kartengröße, Stadtgrößen, WORLD_700/1500, Blocks 1–9 |
| `30#` | Open World Karte erstellt | 01.09.–01.10. | 271 | Kartenaufträge (isometrisch, WORLD_700-Iterationen, 66×34-km-Auftrag, Form-Aufträge) |
| `35#` | Projektwissen nutzen | 06.09.–04.10. | 951 | Stadtgrößen 09.09., Kartenkritik 27.09.–04.10., Motorsport |
| `43#` | Bunkergröße und Ausstattung | 27.09. | 5 | Bunkerreferenz der Serie |
| `45#` | Planung Mapgenerierung | 04.10.–13.10. | 284 | 25 Kartengrößen, Städtevision, V3.1–V1.3e-Berichte, Skizze, Stadtgrößenfrage |
| `48#` | Planung ZIP Downloads | 05.10.–13.10. | 297 | Tesla-/Bunker-Entscheidungen |
| `54#` | Kartenversion präsentieren | 09.10.–13.10. | 137 | V1.3a/V1.3e.3-Aufträge |

Dateinamen: `23_SPN_Full_Master_ZIP_6a83a2dc.md`, `30_Open_World_Karte_erstellt_6a961cea.md`, `35_Projektwissen_nutzen_6a9cb579.md`, `43_Supernatural_Bunkergr_e_und_Ausstattung_6a…`, `45_Planung_Mapgenerierung_6ac29b58.md`, `48_Planung_ZIP_Downloads_6ac427a5.md`, `54_Kartenversion_pr_sentieren_6ac94b77.md`.

### 2.2 Kanon- und Planungsdokumente

| Schlüssel | Datei (relativ zum kumulativen Master) |
|---|---|
| `M-CA0905` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` |
| `M-CA0906` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` |
| `M-CU0908` | `01_CURRENT_CANONICAL_STATE/CANONICAL_UPDATE_2026-09-08.md` |
| `M-W1500` / `M-W700` | `01_CURRENT_CANONICAL_STATE/WORLD_1500/STATUS_2026-09-10.md` / `…/WORLD_700/STATUS_2026-09-10.md` |
| `M-MS1013` | `01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md` |
| `M-AD1015` / `M-DR1015` | `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md` / `…/02_DECISION_REGISTER.md` |
| `M-AD1018` | `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/01_CONFIRMED_AND_PROVISIONAL.md` |
| `M-WB17` / `M-WB89` | `02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT.md` / `…_8_TO_9_CURRENT.md` |
| `M-W15R` | `02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/WORLD_1500_CURRENT_REQUIREMENTS.md` |
| `M-LATE` | `02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/LATE_ADDITIONS_AND_CORRECTIONS_2026-09-04.md` |
| `M-PD` / `M-OD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` / `…/OPEN_DECISIONS.md` |
| `M-B38` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/planning-backlog/38-vehicle-physics-trims-racing.md` |

### 2.3 Tages-Deltas (14.–19.10.2026)

Alle unter `99_DAILY_DELTAS/2026-10-NN/SPN_DAILY_DELTA_2026-10-NN/docs/…`. Sie sind **Review-Pakete**: Entscheidungen ‚wie berichtet‘, Vorschläge und Belege; sie machen nichts allein zum Kanon (siehe [Abschnitt 5](#5-gewichtung-der-quellen)).

| Schlüssel | Datei | Inhalt |
|---|---|---|
| `D14-UC` | `2026-10-14/…/docs/canon/UNDERGROUND_CITIES_DECISIONS.md` | Untergrundstädte |
| `D14-HB` | `…/docs/canon/HUNTER_BUNKER_AND_FAMILY_KEY.md` | Bunker, Tesla, Schlüssel |
| `D14-PS` | `…/docs/design/POPULATION_AND_SPATIAL_SCENARIOS.md` | Flächen-/Bevölkerungsrechnung |
| `D14-NS` | `…/docs/design/UNDERGROUND_NETWORK_AND_SECURITY.md` | Netz und Sicherheit (Entwurf) |
| `D14-OQ` / `D14-CL` | `…/docs/open_questions/OPEN_QUESTIONS_2026-10-14.md` / `2026-10-14/…/CHANGELOG.md` | offene Fragen, Änderungen |
| `D15-RW` | `2026-10-15/…/docs/specs/RAILWAY_SYSTEM_2026-10-15.md` | Bahnsystem |
| `D15-RV` | `…/docs/specs/ROAD_VEHICLES_2026-10-15.md` | Straßenfahrzeuge |
| `D15-U` / `D15-OQ` | `…/docs/canon/UNDERGROUND_SOCIETY_DECISIONS_24-32.md` / `…/open_questions/OPEN_QUESTIONS_2026-10-15.md` | U24–U32, offene Fragen |
| `D16-AI` / `D16-SIM` / `D16-REC` | `2026-10-16/…/docs/canon/AI_DECISIONS_33_34.md` / `…/specs/AI_SIMULATION_BOUNDARIES.md` / `…/specs/AI_RECOGNITION_AND_CONFLICTS.md` | U33/U34 und Entwürfe |
| `D17-U` / `D17-AUT` / `D17-COM` / `D17-PRI` / `D17-OQ` | `2026-10-17/…/docs/canon/UNDERGROUND_AI_AND_COMMUNICATION_U35_U36.md` / `…/specs/AI_AUTONOMY_BOUNDARIES.md` / `…/specs/COMMUNICATION_MODALITIES.md` / `…/specs/PRIVACY_SECURITY_AND_FAILURE_REQUIREMENTS.md` / `…/open_questions/OPEN_QUESTIONS_2026-10-17.md` | U35/U36, U37 offen |
| `D18-MAP` / `D18-ACC` / `D18-REV` / `D18-OQ` / `D18-CMP` / `D18-PRV` | `2026-10-18/…/docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md` / `…/specs/MASTER_MAP_ACCEPTANCE_SPEC.md` / `…/status/MAP_ITERATION_REVIEW.md` / `…/open_questions/OPEN_QUESTIONS_2026-10-18.md` / `…/comparison/PRIOR_CANON_DIFF.md` / `…/provenance/SCOPE_AND_EVIDENCE.md` | Kartenentscheidungen |
| `D19-REQ` / `D19-CHK` / `D19-OQ` / `D19-CMP` | `2026-10-19/…/docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md` / `…/specs/MAP_ACCEPTANCE_CHECKS_2026-10-19.md` / `…/open_questions/OPEN_QUESTIONS_2026-10-19.md` / `…/comparison/DELTA_VS_PRIOR_CANON_2026-10-19.md` | Kartenanforderungen 19.10. |

### 2.4 WORLD_700-Technikpakete

| Schlüssel | Datei | Inhalt |
|---|---|---|
| `G7-GPKG` | `_SOURCE_ARCHIVE/World Current/04_WORLD_700_CURRENT/vectors/open_world_master_iteration_8_3.gpkg` | GeoPackage (495 Features) |
| `G7-CLOSE` | `_SOURCE_ARCHIVE/World Current/04_WORLD_700_CURRENT/docs/README_ITERATION_8_3.md`, `validation/world_master_closure_decision_v4.json`, `packages/…/admin/area_statistics_locked.json`, `…/mountain/*.json`, `…/roads/*.json`, `…/southwest/*.json` | Abschluss Iteration 8.3 |
| `G7-LCS` | `02_WORLD_700/AVAILABLE_TECHNICAL_MAP_ARTIFACTS_2026-09-23/README_iteration_7_technical_map.md` | Koordinatensystem OWP-LCS-1 |
| `G7-PA` | `02_WORLD_700/AVAILABLE_TECHNICAL_MAP_ARTIFACTS_2026-09-27/open_world_multiresolution_phase_a/…` | Phase A (neun Regionen, teilweise wiederhergestellt) |
| `G7-V3` | `_SOURCE_ARCHIVE/World History Loose Early/05_WORLD_700_HISTORY/loose_files/Weltplanung_V3_Spatial_Spec.md` | frühe 650-km²-Spezifikation |

---

### 2.5 Primärentscheidungen des Projektinhabers

| Schlüssel | Quelle |
|---|---|
| `PE-E1022` | Mitteilung ‚Kanon-Update: Metropolis, Höllenzeit und Hunter HQ‘ des Projektinhabers vom 22.10.2026 (direkt im Arbeitsauftrag). Drei verbindliche Entscheidungen, **neue Primärentscheidungen und kein Bestandteil des Masters**; sie haben Vorrang vor älteren widersprüchlichen Planungsständen. |

Behauptet wird nicht, dass diese Aussagen schon in älteren Master-Quellen standen. Die Entscheidungen betreffen: (1) Metropolis = 110 km² zusammenhängendes Stadtgebiet einschließlich Vororten, (2) Höllenzeit 40 Jahre Hölle ≈ 4 Monate Erde, (3) die geografische Lage des Hunter HQ darf in der allgemeinen Weltdokumentation stehen.

---

## 3. Methodik und Statusregeln

1. **Inventar per Skript.** `Tools/world-audit/inventory.py` zählt je Datei Themenbegriffe in 4.864 Master-Dateien (nur Textdateien unter 3 MB); `split_chats.py` zerlegt Chat-Transkripte in Nachrichten; `gpkg_summary.py` und `gpkg_metrics.py` lesen das GeoPackage nur lesend und berechnen Flächen und Längen aus der Geometrie. **Eine Stichwortsuche ersetzt keine Lektüre:** jede zitierte Stelle wurde im Kontext gelesen.
2. **Statusvergabe.**
   - 📋 nur, wenn die Quelle selbst ‚confirmed / user-specified / user-approved / LOCKED / CANON‘ sagt oder der Entwickler die Aussage im Chat ausdrücklich so formuliert.
   - 🧭 bei ‚provisional / working figure / scenario / candidate / test‘.
   - 💡 für Vorschläge des Assistenten oder Engineering-Notizen, auch wenn der Entwickler mit „ja“ antwortete, ohne die Zahl oder Struktur selbst zu bestätigen. Ein knappes „ja passt“ auf eine **Liste** mit vielen Punkten bestätigt die Richtung, nicht jede Zahl; das Register hält diese Fälle als 💡 oder 🧭.
   - 🕰️ wenn eine spätere ausdrückliche Entscheidung die Aussage ersetzt hat; die Aussage bleibt als Geschichte stehen.
   - ⚠️ wenn zwei gleichrangige Quellen sich widersprechen und keine Ersetzung belegt ist.
   - ❓ wenn die Frage gestellt, aber nicht beantwortet wurde.
3. **Spätere ausdrückliche Entwicklerentscheidungen schlagen ältere** (CLAUDE.md §2). Ein neueres **Dokument** schlägt ein älteres nicht automatisch (Abschnitt 5).
4. **Berichtete Ergebnisse sind keine Ergebnisse.** Statusberichte aus Kartenläufen („128 von 131 Tests bestanden“, „20/20 PASS“) stehen in den Chats als Berichte von Assistenten in anderen Chats. Sie werden als **berichtet** geführt und nirgends als Prüfergebnis dieser Dokumentation ausgegeben.
5. **Zitate.** „…“ = wörtlich (maschinell gegen die Quelle geprüft, [Modul 23](23-Quellenabdeckung-und-Auditbericht.md)); ‚…‘ = Paraphrase, gekürzt oder Begriff.
6. **Keine neuen Entscheidungen.** Technische Vorschläge erscheinen nur mit 💡 und dem Hinweis, dass sie nicht beschlossen sind.
7. **Sprache.** Deutsch; Bezeichner und Dateiformate bleiben englisch.

---

## 4. Chronologie der Quellen

| Datum | Ereignis | Quelle |
|---|---|---|
| 18.–27.08. | Erste Weltgedanken: eine einzigartige, generierte Welt; Orte dürfen nicht doppelt sein; mindestens fünf Rennstrecken | 23#33E; 23#46E; 23#511E |
| 31.08. | Kartengröße 200–300 km² reicht nicht; Stadtgrößen 20–40 km²; Küste klein; Festland; Gebirge 2.000–5.000 m (widersprüchlich) | 23#697E–23#823E |
| 01.–03.09. | WORLD_700-Iterationen (12K-Master, Locks, 11 Regionen); 1.500-km²-Karte mit 70-km²-Megacity beschlossen; Blocks 1–7 beantwortet | 30#1E–30#209E; 23#881E; 23#907E; 23#1009E–23#1039E |
| 04.–10.09. | Karte größer (1.500–1.800 km²), WORLD_700 kein DLC, Blocks 8–9, Reiche, Zeitreise | 23#1067E; M-CA0906; M-CU0908 |
| 09.09. | ~2.000 km² für die Hauptstory; Metropolis 70–80 km²; Städte höchstens 12–18 % | 35#87E; 35#89E; 35#93E |
| 16.09. | Stadtrennen, Rallyes mit 10–20 Etappen | 35#562E; 35#566E |
| 27.09.–01.10. | Karten-Kritik: kein Insel-/Fortnite-Look, Städte zu groß, Rechteckform falsch, zu viel Gebirge | 35#876E–35#947E; 30#227E–30#265E |
| 04.10. | Größen-Tests bis 1 Mio. km²; Lake Alder auf 25 km² | 45#8E; 45#22E; 45#24E |
| 09.10. | Städtevision (New York/Chicago/San Francisco/Boston als Gefühl); 570 km² verworfen | 45#58E–45#61E; 48#39E |
| 10.–13.10. | Karten-/Terrain-Berichte V3.1–V1.3e.3; Skizze; Stadtgrößen-Frage (70/110/160) | 45#80E–45#284A; 54#104E |
| 13.10. | Planungsstand: 2.000–2.200 km², Metropolis 20–25 test | M-MS1013 |
| 14.–17.10. | Untergrund-Lore, U24–U36, Bahn, Straßenfahrzeuge | D14–D17 |
| 15.10. | Untergrundstadt 50–100 km² vom Entwickler bekräftigt | M-AD1015 |
| 18.10. | 2.200-km²-Kandidat, Metropolis 110 km², 20 Bezirke, Ostmeer, Nordgebirge | D18-MAP; M-AD1018 |
| 19.10. | Flächenbilanz (2.200 / 110 / 2.090 / Insel 80–100), 25 Größenvarianten, flacher Atlas | D19-REQ |
| 20.–22.10. | **nicht im Master** (Ausnahme: die drei Primärentscheidungen vom 22.10.) | – |
| 22.10. | Primärentscheidungen: Metropolis 110 km² = Stadtgebiet einschließlich Vororten, Höllenzeit, HQ-Lage dokumentierbar | PE-E1022 |

---

## 5. Gewichtung der Quellen

| Schicht | Beispiele | Gewicht |
|---|---|---|
| Kanon-Nachträge | M-CA0905, M-CA0906, M-AD1015, M-AD1018 | Hohe Kandidaten; M-AD1018 sagt selbst, dass Annahme ungelöster Punkte noch geprüft werden muss |
| Entwicklerantworten in Chats | 23#881E, 35#87E, 45#8E, 45#61E | Primärbeleg, wenn die Antwort die Zahl selbst nennt |
| Tages-Deltas | D14–D19 | Belege und Vorschläge; Entscheidungen ‚wie berichtet‘ aus den Chats des jeweiligen Tages; Ablage ohne Rohnachrichten |
| Eingefügte Fremdberichte | 45#216E, 45#253E, 54#… | Berichtet, nicht überprüft |
| Auftragstexte | 30#232E (Prototyp 29.09.), 30#255E (Form) | Arbeitsaufträge für Kartenläufe; Zahlen darin sind Planungsvorgaben des Laufs, nicht dauerhafte Weltentscheidungen |
| Vorschläge des Assistenten | 45#60A, 45#282A, 35#926A | 💡 |
| Prompt-Bibliothek | `LIB/prompts/…` | alles Entwürfe (`DRAFT_REWRITE_REQUIRED`), keine Anforderungen |

---

## 6. Die Lücke nach dem 19. Oktober

Der Master endet für die Weltkarte beim **Tages-Delta vom 19.10.2026**. Heute ist der 22.10.2026. Daher gilt:

- Entscheidungen vom **20.–22.10.** (mit Ausnahme der drei Primärentscheidungen `PE-E1022` vom 22.10.; zum Beispiel eine Abnahme eines Kartenkandidaten, eine Antwort auf U37 oder die Größe der Untergrundstadt) **fehlen**; sie sind nirgends in diesem Ordner berücksichtigt.
- Die Karte vom 19.10. („V7“, Papierskizze, `SPN_SCALE_MASTER_V1`, 8.000×4.500 px mit GeoJSON/GeoPackage) wird in `D19-REQ` beschrieben, **die Dateien selbst fehlen** („raw files were not available“).
- Das Dokument [Modul 19](19-Kartenreferenzen-und-Versionsgeschichte.md) führt alle genannten Versionen mit ihrem Status; kein Kandidat wird als akzeptiert geführt.
- Der Projektinhaber sollte die fehlenden Tages-Deltas ab dem 20.10. nachreichen, bevor die Weltgeometrie angefasst wird.

---

## 7. Eigenheiten des Chat-Materials

- Viele Nachrichten mit Rolle „user“ sind **eingefügte Texte aus anderen Chats**: Statusberichte (45#216E, 45#241E bis 45#265E), Aufträge an Bildgeneratoren (30#232E), Terminalausgaben (35#853E, 45#172E). Sie sind **keine** Äußerungen des Entwicklers und tragen im Register den Vermerk „eingefügt“.
- Antworten des Entwicklers sind oft knapp („ja genau“, „passt“). Der Bezug ergibt sich aus der vorangehenden Assistentennachricht; das Register nennt dann beide Nachrichten.
- Bilder und Anhänge erscheinen nur als „[Nichttext-Anhang oder strukturierter Inhalt]“; die **Skizze vom 13.10.** und alle Kartenbilder sind daher nicht auswertbar.
- Rechtschreibung in Entwicklerzitaten wird nicht korrigiert.
