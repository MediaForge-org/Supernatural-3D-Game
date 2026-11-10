# Welt 19: Kartenreferenzen und Versionsgeschichte

> [!IMPORTANT]
> **Keine Karte der größeren Welt ist akzeptiert.** Dieses Modul führt alle belegten Kartenversionen mit ihrem Status, ihrer Herkunft und dem, was von ihnen im Master tatsächlich vorliegt. Berichtete Prüfzahlen stammen aus Statusberichten anderer Chats und sind **nicht überprüft**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md) (KA); GIS: [Modul 17](17-Weltgenerierung-und-GIS-Daten.md), [Modul 24](24-WORLD700-GIS-Referenz.md).

**Inhalt**

1. [Zwei Kartenlinien](#1-zwei-kartenlinien)
2. [Linie A: WORLD_700](#2-linie-a-world_700)
3. [Linie B: größere Welt](#3-linie-b-größere-welt)
4. [Terrain-Forschungsreihe V3.1](#4-terrain-forschungsreihe-v31)
5. [Welt-Blueprint-Reihe V1.x](#5-welt-blueprint-reihe-v1x)
6. [Bildkandidaten V3 bis V7](#6-bildkandidaten-v3-bis-v7)
7. [Referenzbilder und Skizzen](#7-referenzbilder-und-skizzen)
8. [Was im Master tatsächlich liegt](#8-was-im-master-tatsächlich-liegt)
9. [Offene Abnahmeaufgaben](#9-offene-abnahmeaufgaben)

---

## 1. Zwei Kartenlinien

| Linie | Status | Rohdaten im Master |
|---|---|---|
| **A: WORLD_700** (Referenzwelt, 733,8 km² Hülle, 690,9 km² Spielfläche) | 🕰️ / 📋 gesperrt (`WORLD_MASTER_LOCK_1`); Iteration 8.3 `PASS_WITH_REVIEW_FLAGS` | **ja** (12K-PNG, GeoPackage, DEM, Hydrologie, SVG, Prüfberichte) |
| **B: größere Welt** (2.200-km²-Kandidat) | 🧭 keine Karte akzeptiert, Geografie nicht gesperrt | **nein** (nur Anforderungen und berichtete Zwischenstände) |

Beide Linien dürfen nicht vermischt werden ([W-GE-004](20-Entscheidungsregister.md#2-weltgröße-und-grundstruktur-ge)).

---

## 2. Linie A: WORLD_700

Quellen: Chat 30 (01.–02.09.), `G7-LCS`, `G7-PA`, `G7-CLOSE`, `G7-V3`.

| Stufe | Datum | Inhalt | Beleg |
|---|---|---|---|
| Frühe Konzepte | 31.08. | 200–620 km²; mehrere Gebirgsketten und Flusssysteme; Metropole als Fall-Line-Stadt am Hauptfluss, Major B als Küstenhafen, HQ als altes Anwesen am See | 23#788E (eingefügt); 23#790E |
| Spatial Spec V3 | 31.08.–01.09. | 650 km², 7 Staaten, 2 Gebirgsketten, 4 Flusssysteme, Küste klein (2,9 %), 33 Siedlungen (Metropolis 35 km²) | G7-V3 |
| 12K-Master | 01.09. | 12.288 × 8.192 px, ~101–105 MB, Iterationen 4–6 mit „POSITION LOCK“ | 30#34E; 30#48E; 30#62E; 30#72E |
| Georeferenz | 01.09. | Iteration 7: OWP-LCS-1, 2,7 m/px, GCPs, SVG-Overlay, Zoomstufen Z0–Z6 | G7-LCS; 30#72E |
| Premium-SVG | 02.09. | echte mathematische SVG ohne Rasterbilder; Wunsch des Entwicklers nach Vektor | 30#138E; 30#149E; 30#156E |
| Mehrfachauflösung | 02.09. | Phase A (9 Regionen) → A.1 (11 Regionen) → **A.2 vom Entwickler als `APPROVED_LOCKED` bestätigt**, 11 Regionen, 0 m² unbedeckt | 30#167E; 30#176E; 30#185E; 30#194E |
| R05A-Pilot | 02.09. | Metropolis-Region als 24K-Test, visueller Stilpass B.2 | 30#209E |
| Iteration 8.x | 02.09. | Südwest-Revision (Damm, historisches Zentrum, Steinbruch), Hydrologie, Gebirgs-DEM, Straßen-/Bahn-/Hillclimb-/Gondelabschluss | 30#124E; G7-CLOSE |
| Abschluss | 03.09. | Entwickler: 700 km² ‚abgeschlossen bis auf Ergänzungen‘ | 23#1019E |
| Archivierung | 03.09. | R05A-Pilote B1–B3 (24K) archiviert; der Entwickler beendet vorerst weitere visuelle Detailiteration (Vorstufe, MASTER-MIGRATION-17) | `02_EMBEDDED/00_START_HERE/CURRENT_SUPERSESSION_2026-09-03.md` |

**Gesicherte Zahlen der Phase A.2** (vom Entwickler als autoritativ bestätigt, 30#194E): 11 Regionen, 0 m² Unbedecktes, 0 m² außerhalb, Weltumhüllung 733,83542784 km², R04 = 20K / 0,493179 m/px, R05A = 24K / 0,447597 m/px, R05B = 24K / 0,448000 m/px, R09 = 24K / 0,499577 m/px, Produktionsredundanz ≈ 13,1679 %.

**Lücken in `G7-PA`:** Das Paket Phase A war abgeschnitten; `region_table.csv`, `regional_boundaries_overlay.svg` und `storage_estimates.json` konnten nicht wiederhergestellt werden (`RECOVERY_STATUS.md`). Spätere A.1/A.2-Dateien liegen an anderer Stelle (`World Current/…/phase_a2_closure/`), werden aber ausdrücklich **nicht** als Ersatz der Phase-A-Originale ausgegeben.

---

## 3. Linie B: größere Welt

| Stufe | Datum | Inhalt | Beleg |
|---|---|---|---|
| Ablehnung der Vorstufe | 27.09.–01.10. | „Fortnite-Map“, Insel, zu viele Städte, zu viel Gebirge, Rechteckform, fehlende Maßstabseinheit/Legende | 35#876E–35#947E |
| Prototypauftrag | 29.09. | 66 × 34 km technische Makrokarte mit Koordinatenraster; Städte, Seen, Küste in Kilometerangaben | 30#232E (eingefügt) |
| Veredelungsaufträge | 29.09.–01.10. | Beibehalten der Makroverteilung, ländliche Straßen −30–40 %, größere Wildnislücken; dann „keine Rechteckform“ | 30#237E; 30#241E; 30#245E; 30#249E; 30#255E; 30#259E; 30#265E |
| Maßstab-Rechnung | 01.10. | 105 × 60 km Layout auf ~2.200 km² skaliert (×0,591): 62,0 × 35,5 km | 35#926A |
| Größenvergleich | 04.10. | 25 Kartengrößen | 45#60A |
| Städtevision | 09.10. | New York/Chicago/San Francisco/Boston als Gefühl | 45#58E; 48#39E |
| Terrainreihe V3.1 | 10.–11.10. | siehe Abschnitt 4 | 45#80E–45#205E |
| Blueprint-Reihe V1.x | 11.–13.10. | siehe Abschnitt 5 | 45#210E–45#274E; 54#104E |
| Entwicklerskizze | 13.10. | Papier „extrem grob“ | 45#277E; 45#279E |
| Bildkandidaten V3–V7 | 14.–19.10. | siehe Abschnitt 6 | D18-REV; D19-REQ |

**Aufträge in Chat 30 (29.09.–01.10.)** nennen als zu bewahrende Anker: Metropolis, Major City A, Major City B, Lake Alder, Lake Meridian, Meridian River, Greywater Wetlands, Coastal Town, Harbor Isle, Hunter HQ, drei Flughäfen und ‚sechs Motorsportsysteme‘ (30#265E). Diese Anker sind die **Namensbasis** des Kandidaten; ihre Lagen sind nur im Prototypauftrag (29.09.) in Kilometern angegeben (Metropolis ~X 34 / Y 17, Major City A ~10,5 / 21, Major City B ~55 / 20, Lake Alder ~24 / 26, Lake Meridian ~35–37 / 20–22, Greywater-Feuchtgebiet X 42–52 / Y 6–13, Küstenstadt ~59–61 / 6–8, Harbor Isle ~63–65 / 4–6, Hunter HQ ~28–31 / 11–14; 30#232E). Diese Koordinaten sind 🕰️: sie gehören zu einer Karte, die verworfen wurde.

---

**Ergänzung (Chat 54, 09.10.):** Die ersten Berichte des Generator-Chats nennen eine „Variante 1/25“ mit 2.200 km² (Rechteck 67,4 × 34 km, 96,00 % Abdeckung, Raster 50 m) und eine V2 mit höchstem Gipfel 4.600 m und 128 von 131 bestandenen Tests (W-KA-016). Es sind Berichte des Assistenten ohne Dateiprüfung. Die Hülle 67 × 34 km geht auf die Entscheidung vom 04.10. zurück, 95–97 % des Rechtecks als Spielfläche zu nutzen (W-GE-035).

## 4. Terrain-Forschungsreihe V3.1

Berichte in Chat 45 (eingefügte Assistentenberichte; **nicht überprüft**).

| Version | Datum | Berichteter Inhalt | Beleg |
|---|---|---|---|
| V2-Baseline | 10.10. | 128 von 131 Tests bestanden; drei bekannte Fehler: Lake Alder → Meridian ohne durchgängige Verbindung, 28.781 bergauf gerichtete D8-Kanten, 12 von 21 Hauptverbindungen mit Steigungsproblemen | 45#80E |
| V3.1 | 10.10. | 53/57 bestanden; Alder → Meridian → Meer funktioniert; Straßen 6 von 21 nicht freigegeben; Bahn und Gebirgsrealismus unzureichend | 45#80E |
| V3.1c–V3.1i | 10.10. | zwei Terrain-Kandidaten, Watershed-/Shoreline-Pilot, Real-DEM-Gate; Zwischenstände 34/41, 33 PASS/9 FAIL, `NOT_ACCEPTED` | 45#87E; 45#90E; 45#107E; 45#111E; 45#115E; 45#119E |
| V3.1j–V3.1s | 10.–11.10. | Einlesen echter USGS-Referenzdaten (Glacier National Park, Great Smoky Mountains), Priority-Flood, 1-m-Höhendaten, Hydrographie | 45#136E bis 45#199E |
| V3.1t | 11.10. | 2×2-km-Prototyp, 15 Talabschnitte, 116/121 unabhängige Tests | 45#205E |

Ergebnis laut Chat: **kein akzeptierter Terrain-Kandidat**. Der Entwickler fragte am 11.10. ‚wann gibt es wirklich eine Map, bisjetzt sind es ja nur teile und nichts sichtbares‘ (45#203E). Die Forschungsreihe wurde danach von der Welt-Blueprint-Reihe getrennt weitergeführt (45#216E).

---

## 5. Welt-Blueprint-Reihe V1.x

| Version | Datum | Berichteter Inhalt | Beleg |
|---|---|---|---|
| V1.1 | 11.10. | Atlas und C++-3D-Slice: Spielwelt 2.200 km², Metropolis 22,5, Glenhaven 9, Ashford 9, Lake Alder 25, Meridian 5, Südreservoir 7 km²; 23 Verkehrskorridore (2 PASS, 19 NEEDS_ENGINEERING, 2 FAIL); 2,5D-Software-Renderer, 1,96 km² erkundbar | 45#216E |
| V1.2 | 11.10. | perspektivischer OpenGL-Prototyp; Entwickler: ‚nur paar häuser mehr‘, nicht brauchbar | 45#220E; 45#227E |
| Wunsch riesige Karte | 11.10. | ‚8k bisschen klein‘, riesige Karte, Vektorgrafik für Speichereffizienz und Zoom | 45#231E; 45#233E; 45#237E; 45#239E |
| V1.3a | 11.10. | Vektor-First-Hybrid, 4.096 × 2.066 px, 170 Kacheln, 94 Master-Geometrien, 671 Vektorfeatures | 45#241E; 54#104E |
| V1.3b–V1.3d | 12.10. | zwei Terrain-Ansätze, Architekturvergleich (L korrigiert, W ‚watershed-first‘): `NEITHER`; 50-m-Raster 1.348 × 680 | 45#245E; 45#249E; 45#253E |
| V1.3e | 12.10. | editierbarer Blueprint, 18 Landschaftsregionen, gerichteter Entwässerungsgraph, 10 GIS-Layer | 45#257E |
| V1.3e.1 / .2 | 13.10. | zusätzliche Nebenkämme, 419 Landform-Polygone, 1.819 km² neu klassifiziert (Kartenklassen, kein neues Gelände) | 45#261E; 45#265E |
| V1.3e.3 | 13.10. | 3 Regionen überarbeitet: Nordgebirge/West Crown/North Divide (75,19 km²), Alder ↔ Meridian (18,29 km²), Greywater/Südost (52,67 km²); final 763 Mosaik-Polygone, 146,14 km² Klassifikationsänderung | 45#274E; 54#133E |

**Entwicklerreaktion 13.10.:** ‚ja ich merke es, es gibt nur wenige städte und sonst nichts, das meer ist auch vielleicht bisschen wenig‘ (45#272E); danach die Papierskizze (45#277E) und die Neuordnung (Meer im Osten, Gebirge im Norden). Der Blueprint V1.3e.x ist damit **als Grundlage der Geografie überholt**, seine technischen Daten bleiben als Quelle.

**Status:** alle V1.x sind `NOT_ACCEPTED` („Geografische Freigabe: Nicht bestanden“, „Vollständige Spielwelt spielbar: Nein“, 45#216E; 45#257E).

---

## 6. Bildkandidaten V3 bis V7

Quelle: `D18-REV`, `D19-REQ`. Zählung: Bildversionen des Oktober, nicht identisch mit der Terrain-Reihe V3.1.

| Kandidat | Berichteter Befund | Status |
|---|---|:---:|
| Früher Atlas | als „8K“ gemeldet, laut Bericht nativ 2048 × 1033; GIS/DEM nicht neu erzeugt | 🕰️ nicht akzeptiert |
| GIS-/Illustrationspaket | 8192 × 4132 GIS-PNG plus separate 1536 × 1024 Illustration mit widersprüchlicher Küste | 🕰️ `VISUAL_NOT_ACCEPTED` |
| V3 | Paket 13/13 Hashes, pixelgleiche Ausschnitte; „8K“ aus 1766 × 891 hochskaliert; Küste zu zersplittert, Metropolis zu klein/kreisförmig, sichtbares Hunter-HQ-Label | Zwischenreferenz |
| V4 | Wunsch nach ruhigerer Ostküste, besserer Metropolis und Wildnis bei satellitenähnlichem Stil | nicht bestätigt |
| V5 | zu lebhaft, künstlich, spielzeughaft, schwache Geländetextur | **abgelehnt** |
| V6 | Kandidatenpaket, Original 1766 × 891; Greywater-Wasserproblem und vorläufige Insel eingeräumt | Kandidat, keine Abnahme |
| V7 | Stilreferenz (19.10.) | Stilreferenz, **kein** Geografie-Master |
| `SPN_SCALE_MASTER_V1` | als mathematisch begrenzter Geometrieprototyp (GeoJSON/GeoPackage, 8.000 × 4.500) beschrieben; Rohdaten im Lauf nicht verfügbar | ❓ nicht prüfbar |

**Merksatz der Quelle:** Prüfsummen, Ausschnitte und Hochskalierungen sind keine Abnahme (D18-REV).

---

## 7. Referenzbilder und Skizzen

- **Papierskizze (13.10.)** des Entwicklers: Gebirge im Norden, Metropolis zentral, A und B west/östlich, Meer rechts mit Insel. Nach `D19-REQ` hat sie für die **Anordnung** Vorrang. Datei nicht im Master.
- `06_NEW_REFERENCES_2026-09-10/Karte_des_Meridian-Reichs.png` (3,1 MB): ein Referenzbild mit dem Namen „Meridian-Reich“; `REFERENCE_STATUS.md` nennt es nur als Referenz. Der Inhalt wurde in dieser Etappe nicht bildlich ausgewertet.
- `02_WORLD_700/…/open_world_map_iteration_6_master_12k.png` und `…_5_…`: 105 MB große Rasterkarten der Referenzwelt; nicht bildlich ausgewertet.
- Aus Papier und V7 sind im Master keine Maßangaben überliefert; die Maße stammen aus Text ([Modul 03](03-Massstab-Flaechen-Proportionen.md)).

---

## 8. Was im Master tatsächlich liegt

| Artefakt | Vorhanden | Prüfung dieser Etappe |
|---|:---:|---|
| WORLD_700 GeoPackage (495 Features) | ja | gelesen, Flächen/Längen berechnet ([Modul 24](24-WORLD700-GIS-Referenz.md)) |
| WORLD_700 12K-PNG, SVG, DEMs (TIFF) | ja (große Binärdateien) | nicht geöffnet; nur Metadaten gelesen |
| WORLD_700 Validierungs-JSONs (v4, Scorecard, Closure) | ja | gelesen (Closure, Flächenstatistik, Ingenieurdateien) |
| Phase A (9 Regionen), Phase A2 (11 Regionen) Manifeste | ja (Phase A teilweise) | Manifest gelesen |
| Terrain-/Blueprint-Pakete V3.1–V1.3e.3 (ZIPs) | **nein** | nur Berichte |
| Bildkandidaten V3–V7, `SPN_SCALE_MASTER_V1` | **nein** | nur Berichte |
| Papierskizze 13.10. | **nein** | – |
| Hunter-HQ-freie öffentliche Karte | **nein** | – |
| Dokumentation der HQ-Lage (Weltübersicht) | **ja, freigegeben** (22.10., `PE-E1022`, W-KA-017) | gilt nicht für Spieler-/Exportkarten (W-KA-008) |

---

## 9. Offene Abnahmeaufgaben

Aus `D18-OQ` und `D19-OQ` (unverändert übernommen, nicht beantwortet):

1. Welcher Vollwelt-Raster-Master ist verbindlich? Bisher keiner.
2. Kann der Export echte Details bei ~7680 × 4320 enthalten statt Hochskalierung?
3. Georeferenz: Grenzen, Projektion, Weltmaßstab, Ankerpunkte, Herkunftsnachweis.
4. Endgültige Hydrografie: Lake Alder/Meridian/South Reservoir, Abflüsse, Greywater Bay, Küstenlinie, Form und Name der Ostinsel.
5. Wie übersetzen sich die bestätigten 110 km² (Stadtgebiet einschließlich Vororten), 20 Bezirke, 20/12-km²-Kern und 700–1.000-m-Landmarken in Spieldichte, vertikalen Maßstab und Leistung?
6. Positionen von Glenhaven und Ashford, Hafenlayout Greywater ohne Kanäle.
7. Geheimnisebenen (Hunter HQ u. a.) für Spieler- und Exportkarten; die Lage des HQ darf in der Weltdokumentation stehen (W-KA-017), die exakte Position im Kandidaten ist ❓ (OF-69).
8. Welche der ~25 Größen-Presets werden unterstützt, nach welchen Regeln skaliert?
9. Kann der Fluss-Graph Alder – Metropolis durchgängig sein, ohne durch Glenhaven/Downtown zu führen?
10. Welches CRS, welche Toleranzen, welche native Rasterauflösung?
