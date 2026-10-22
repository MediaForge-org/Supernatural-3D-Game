# Welt 24: WORLD_700 – GIS-Referenz (generiert)

> [!IMPORTANT]
> **Gesperrte Referenzwelt, nicht die größere Hauptwelt.** Dieses Modul beschreibt das **vorhandene** GeoPackage von WORLD_700 (Iteration 8.3, `PASS_WITH_REVIEW_FLAGS`). Die Tabellen sind **generiert** (`Tools/world-audit/render_w700.py`), die Flächen und Längen **aus der Geometrie berechnet**, nicht aus Berichten übernommen. Keine Zahl gilt für den 2.200-km²-Kandidaten ([W-GE-004](20-Entscheidungsregister.md#2-weltgröße-und-grundstruktur-ge)).

Teil der Welt-Dokumentation ([Index](README.md)). Kontext: [Modul 03 §10](03-Massstab-Flaechen-Proportionen.md#10-world_700-als-maßstabsreferenz), [Modul 17](17-Weltgenerierung-und-GIS-Daten.md).

**Inhalt**

1. [Datei und Methode](#1-datei-und-methode)
2. [Staaten](#2-staaten)
3. [Siedlungen](#3-siedlungen)
4. [Flughäfen und Punkte von Interesse](#4-flughäfen-und-punkte-von-interesse)
5. [Motorsportstandorte](#5-motorsportstandorte)
6. [Gewässer](#6-gewässer)
7. [Straßen](#7-straßen)
8. [Schiene und Nahverkehr](#8-schiene-und-nahverkehr)
9. [Gelände und Technikdaten](#9-gelände-und-technikdaten)
10. [Layerübersicht des GeoPackage](#10-layerübersicht-des-geopackage)

---

## 1. Datei und Methode

- **Datei:** `G7-GPKG` (`vectors/open_world_master_iteration_8_3.gpkg`, GeoPackage-Schema 1.3, 495 Features, stabile `feature_id`, laut `G7-CLOSE` Integritätsprüfung „ok“).
- **Koordinatensystem:** OWP-LCS-1 (fiktiv, kartesisch, Meter; Ursprung Südwest, X nach Osten, Y nach Norden). **Kein** realer geodätischer Bezug.
- **Berechnung:** Flächen per Shoelace-Formel auf der planaren Geometrie (Löcher abgezogen), Längen als Polylinienlänge, Schwerpunkt als Mittel der Stützpunkte (grobe Lage, **kein** Flächenschwerpunkt). Abweichungen gegenüber den Rasterstatistiken in `G7-CLOSE` sind möglich (Raster 10,8 m vs. Vektorgeometrie) und werden nicht ausgeglichen.
- **Status der Daten:** Stadt-, Flughafen- und Staatspolygone sind laut Datei ‚Planungseinhüllende, keine Katastergrenzen‘ (`alignment_status: planning_envelope_not_cadastral_boundary`). Wasserflächen sind aus der Rasterkarte segmentiert (`VISUAL_MASTER_WATER_SEGMENTATION`).


---

## 2. Staaten

| ID | Name | Fläche Polygon (km²) | Landfläche laut Datei (km²) | Binnenwasser (km²) | Meer (km²) | Anteil Spielfläche (%) | Nachbarn |
|---|---|---:|---:|---:|---:|---:|---|
| STATE_A | State A | 98,71 | 96,04 | 2,83 | 0,00 | 13,90 | B, D |
| STATE_B | State B | 75,01 | 68,36 | 6,73 | 0,00 | 9,89 | A, C, E |
| STATE_C | State C | 127,69 | 125,17 | 2,52 | 0,00 | 18,12 | B, E, G |
| STATE_D | State D | 79,53 | 79,62 | 0,00 | 0,00 | 11,52 | A, E, F |
| STATE_E | State E | 68,45 | 68,12 | 0,28 | 0,00 | 9,86 | B, C, D, F, G |
| STATE_F | State F | 125,75 | 125,16 | 0,53 | 0,00 | 18,12 | D, E, G |
| STATE_G | State G | 158,69 | 128,43 | 0,00 | 30,05 | 18,59 | C, E, F |

Summe der sieben Staatspolygone (Rechnung): 733,84 km². Die Datei führt die Verwaltungsfläche einschließlich Wasser getrennt (Spalten 4–6).

---

## 3. Siedlungen

| ID | Name | Stufe | Staat | Fläche (km²) | Lage X / Y (m, grob) |
|---|---|---|---|---:|---|
| URBAN_METROPOLIS | Metropolis | metropolis | E | 29,98 | 19.401 / 11.696 |
| URBAN_MAJOR_A | Major City A | major_city | A | 9,97 | 5.305 / 16.448 |
| URBAN_MAJOR_B | Major City B | major_city | C | 9,32 | 28.363 / 14.693 |
| URBAN_MID_A | Mid City A | mid_city | B | 1,87 | 11.857 / 14.261 |
| URBAN_MID_B | Mid City B | mid_city | C | 2,07 | 26.790 / 13.019 |
| URBAN_MID_C | Mid City C | mid_city | E | 2,07 | 11.859 / 11.102 |
| URBAN_MID_D | Mid City D | mid_city | G | 2,01 | 24.035 / 10.184 |

Status aller sieben Einträge in der Datei: `planned_locked_anchor`. Metropolis (State E) hat **29,98 km²**, Major City A 9,97 km², Major City B 9,32 km²; zusammen 49,27 km² (7,13 % der Spielfläche von 690,9 km², Rechnung). Die Datei nennt keine Kleinstädte und Dörfer als Polygone.

---

## 4. Flughäfen und Punkte von Interesse

### 4.1 Flughafenflächen

| ID | Staat | Fläche (km²) |
|---|---|---:|
| AIRPORT_WEST_REGIONAL_BOUNDARY | A | 2,39 |
| AIRPORT_METROPOLIS_INTL_BOUNDARY | E | 6,35 |
| AIRPORT_EAST_REGIONAL_BOUNDARY | C | 2,75 |

### 4.2 Punkte (POI)

| ID | Name | Kategorie | Staat | X / Y (m) | Anzeigepriorität |
|---|---|---|---|---|---:|
| POI_HUNTER_HQ | Hunter HQ | hunter_hq | E | 13.500 / 9.509 | 90 |
| AIRPORT_WEST_REGIONAL | West Regional Airport | airport | A | 2.835 / 17.798 | 82 |
| AIRPORT_METROPOLIS_INTL | Metropolis International Airport | airport | E | 17.307 / 7.862 | 90 |
| AIRPORT_EAST_REGIONAL | East Regional Airport | airport | C | 30.780 / 14.234 | 82 |
| POI_GONDOLA_VALLEY | Gondola Valley Station | gondola | C | 20.790 / 17.663 | 58 |
| POI_GONDOLA_SUMMIT | Summit Station | gondola | C | 26.838 / 21.227 | 76 |
| POI_OLD_MERIDIAN_DAM | Old Meridian Dam | dam | E | 13.095 / 8.348 | 72 |
| POI_COASTAL_HARBOR | Coastal Town Harbor | harbor | G | 27.945 / 4.163 | 65 |
| POI_HARBOR_ISLE_DOCK | Harbor Isle Dock | harbor | G | 30.726 / 2.273 | 52 |

Alle POI führen ein Zustandsschema `unknown / discovered / visited / completed / locked / hidden` und sind als Entdeckungs-Zustände vorbereitet; das Hunter HQ trägt mit dem Flughafen Metropolis International die höchste Anzeigepriorität (90) – für die öffentliche Spielerkarte widerspricht das der späteren Geheimhaltungsregel ([W-KA-008](20-Entscheidungsregister.md#3-karte-und-abnahme-ka)); es ist ein Datenbefund der gesperrten Welt, keine Entscheidung für die neue.

---

## 5. Motorsportstandorte

| ID | Name | Staat | X / Y (m) | Priorität |
|---|---|---|---|---:|
| MOTORSPORT_F1 | F1 Circuit | D | 5.940 / 10.778 | 86 |
| MOTORSPORT_ROAD_CIRCUIT | Road Circuit | E | 8.100 / 6.188 | 70 |
| MOTORSPORT_RALLY_PARK | Rally Park | A | 7.830 / 20.498 | 70 |
| MOTORSPORT_MOUNTAIN_HILLCLIMB | Mountain Hillclimb | C | 19.440 / 18.068 | 82 |
| MOTORSPORT_DRAG_STRIP | Drag Strip | G | 21.330 / 6.593 | 68 |
| MOTORSPORT_TEST_PARK | Motorsport Test Park | G | 27.405 / 8.483 | 68 |

Es sind nur **Punkte**; Streckenverläufe, Längen und Zuschauerbereiche liegen nicht vor (Detailkarten „Track Detail“ wurden in `G7-CLOSE` ausdrücklich als nachgelagert geführt).

---

## 6. Gewässer

| ID | Name | Typ | Spiegel (m) | Fläche (km²) | Zuflüsse | Abfluss |
|---|---|---|---:|---:|---|---|
| LAKE_ALDER | Lake Alder | natural_lake | 412.0 | 6,33 | STREAM_ALDER_NW, STREAM_ALDER_N | RIVER_ALDER_OUT |
| LAKE_MERIDIAN | Lake Meridian | natural_lake | 286.0 | 4,86 | RIVER_ALDER_OUT, STREAM_MERIDIAN_N | RIVER_MERIDIAN |
| LAKE_EAGLE | Eagle Lake | mountain_lake | 1380.0 | 0,46 | STREAM_EAGLE_HEAD | STREAM_EAGLE_OUT |
| LAKE_SUMMIT | Summit Lake | mountain_lake | 1860.0 | 0,76 | STREAM_SUMMIT_HEAD | STREAM_SUMMIT_OUT |
| LAKE_SOUTHWEST_01 | Southwest Valley Reservoir | reservoir | 236.0 | 2,12 | STREAM_SW_NORTH_01, STREAM_SW_NORTHEAST_02, STREAM_SW_WEST_03, STREAM_SW_RIDGE_04 | RIVER_SW_RESERVOIR_OUT |
| LAKE_SOUTH_02 | Pine Hill Lake | natural_lake | 214.0 | 0,46 | – | – |

**Feuchtgebiet:** Greywater Wetlands, 8,65 km², Hydroperiode `seasonal_and_perennial_mosaic`.

| Fluss | Von | Nach | Länge (km) | Einzugsgebiete |
|---|---|---|---:|---|
| RIVER_ALDER_OUT | LAKE_ALDER | LAKE_MERIDIAN | 6,81 | BASIN_ALDER → BASIN_MERIDIAN |
| RIVER_MERIDIAN | LAKE_MERIDIAN | GREYWATER_OUTLET | 10,36 | BASIN_MERIDIAN → BASIN_GREYWATER |

Bäche: 12 Features, zusammen 47,71 km (Rechnung). Dämme: Old Meridian Dam (DAM_OLD_MERIDIAN), Southwest Valley Dam (DAM_SOUTHWEST_LOCAL).

Wasserkette in den Daten: **Lake Alder → Fluss RIVER_ALDER_OUT → Lake Meridian → RIVER_MERIDIAN → Greywater-Auslass/-Feuchtgebiet → Küste**; das Südwest-Talreservoir (`LAKE_SOUTHWEST_01`) gehört zu einem eigenen Südbecken. Dieselben Namen und eine ähnliche Kette tauchen im Prototypauftrag vom 29.09. wieder auf (`30#232E`); die Kette gilt **nicht** automatisch für den Kandidaten vom 18./19.10.

---

## 7. Straßen

| ID | Klasse | Von → Nach | Länge (km) | Spuren (ca.) | Brücke | Tunnel | Zugang |
|---|---|---|---:|---:|:---:|:---:|---|
| ROAD_I1_WEST | ROAD_INTERSTATE | RN_WEST_GATE → RN_MAJOR_A | 5,29 | 4 | nein | nein | public |
| ROAD_I1_CENTRAL | ROAD_INTERSTATE | RN_MAJOR_A → RN_MET_WEST | 13,14 | 4 | ja | nein | public |
| ROAD_I1_METROPOLIS | ROAD_INTERSTATE | RN_MET_WEST → RN_MET_EAST | 6,70 | 6 | ja | nein | public |
| ROAD_I1_EAST | ROAD_INTERSTATE | RN_MET_EAST → RN_MAJOR_B | 5,99 | 4 | ja | nein | public |
| ROAD_I1_EAST_GATE | ROAD_INTERSTATE | RN_MAJOR_B → RN_EAST_GATE | 5,37 | 4 | nein | nein | public |
| ROAD_G1_SOUTH | ROAD_HIGHWAY | RN_MET_CENTRAL → RN_SOUTH_JUNCTION | 5,69 | 4 | ja | nein | public |
| ROAD_G1_COAST | ROAD_HIGHWAY | RN_SOUTH_JUNCTION → RN_COASTAL | 7,49 | 4 | ja | nein | public |
| ROAD_AIRPORT_CONNECTOR | ROAD_PRIMARY | RN_MET_CENTRAL → RN_AIRPORT | 4,42 | 4 | ja | nein | public |
| ROAD_MOUNTAIN_ACCESS | ROAD_MOUNTAIN | RN_MAJOR_B → RN_MOUNTAIN | 6,10 | 2 | ja | ja | public |
| ROAD_F1_ACCESS | ROAD_PRIMARY | RN_MID_C → RN_F1 | 5,91 | 2 | nein | nein | event_public |
| ROAD_HILLCLIMB | ROAD_MOUNTAIN | RN_HILLCLIMB → RN_MOUNTAIN | 8,62 | 2 | nein | nein | event_seasonal |
| ROAD_ROAD_CIRCUIT_ACCESS | ROAD_SECONDARY | RN_MID_C → RN_ROAD_CIRCUIT | 6,18 | 2 | nein | nein | event_public |
| ROAD_HUNTER_ACCESS | ROAD_RURAL | RN_MET_WEST → RN_HUNTER | 3,21 | 2 | ja | nein | restricted_destination |
| ROAD_DAM_SERVICE | ROAD_SERVICE | RN_HUNTER → RN_DAM | 1,23 | 1 | ja | nein | authorized_service |
| ROAD_DRAG_ACCESS | ROAD_SERVICE | RN_SOUTH_JUNCTION → RN_DRAG | 1,00 | 2 | nein | nein | industrial_event |
| ROAD_TEST_PARK_ACCESS | ROAD_SERVICE | RN_COASTAL → RN_TEST | 3,92 | 2 | nein | nein | private_controlled |
| ROAD_RALLY_ACCESS | ROAD_FOREST | RN_MAJOR_A → RN_RALLY | 5,20 | 2 | nein | nein | event_seasonal |
| ROAD_SW_HISTORIC_MAIN | ROAD_PRIMARY | RN_SW_CIRCUIT_SERVICE → RN_SW_HISTORIC_CORE | 2,19 | – | nein | nein | public |
| ROAD_SW_DAM_ACCESS | ROAD_RURAL | RN_SW_HISTORIC_CORE → RN_SW_DAM | 4,02 | – | nein | nein | public |
| ROAD_SW_QUARRY_ACCESS | ROAD_SERVICE | RN_SW_HISTORIC_CORE → RN_SW_QUARRY | 4,86 | – | nein | nein | service_restricted |
| ROAD_SW_LOGGING_ACCESS | ROAD_FOREST | RN_SW_QUARRY → RN_SW_LOGGING | 1,64 | – | nein | nein | service_restricted |
| ROAD_SW_FARM_LINK | ROAD_RURAL | RN_SW_HISTORIC_CORE → RN_SW_FARM | 1,99 | – | nein | nein | public |

Summe Hauptnetz (11 Kanten): 74,73 km; Nebenstraßen (11 Kanten): 35,46 km (Rechnung). Die Interstate-Kette I1 (West → Major A → Metropolis → Major B → Ost-Tor) ist das Rückgrat.

---

## 8. Schiene und Nahverkehr

### 8.1 Bahnstrecken

| ID | Klasse | Von → Nach | Länge (km) | Elektrifizierung |
|---|---|---|---:|---|
| RAIL_IC_A_MET | RAIL_INTERCITY | STATION_MAJOR_A → STATION_MET_CENTRAL | 16,50 | planned_unspecified |
| RAIL_IC_MET_B | RAIL_INTERCITY | STATION_MET_CENTRAL → STATION_MAJOR_B | 9,69 | planned_unspecified |
| RAIL_REG_MET_MIDC | RAIL_REGIONAL | STATION_MET_CENTRAL → STATION_MID_C | 7,60 | planned_unspecified |
| RAIL_REG_MET_MIDB | RAIL_REGIONAL | STATION_MET_CENTRAL → STATION_MID_B | 5,34 | planned_unspecified |
| RAIL_REG_MET_COAST | RAIL_REGIONAL | STATION_MET_CENTRAL → STATION_COASTAL | 13,40 | planned_unspecified |
| RAIL_COMMUTER_AIRPORT | RAIL_COMMUTER | STATION_MET_CENTRAL → STATION_AIRPORT | 4,69 | planned_unspecified |
| RAIL_FREIGHT_CITY_AIRPORT | RAIL_FREIGHT | FREIGHT_METROPOLIS → FREIGHT_AIRPORT_CARGO | 2,60 | planned_unspecified |

Summe: 59,83 km (Rechnung). Spurzahl: `conceptual`; Elektrifizierung in allen Kanten ‚geplant, unspezifiziert‘.

### 8.2 Nahverkehrs- und Buskorridore

| ID | Klasse | Rolle | Länge (km) |
|---|---|---|---:|
| METRO_LINE_1 | RAIL_METRO | north_south_trunk | 6,32 |
| METRO_LINE_2 | RAIL_METRO | west_east_trunk | 6,66 |
| METRO_LINE_3 | RAIL_METRO | diagonal_tangential | 6,52 |
| TRAM_CORE_LOOP | RAIL_TRAM | historic_core_lakefront | 9,05 |
| BUS_IC_A_MET | INTERCITY_BUS_CORRIDOR | world_master_corridor | 16,46 |
| BUS_IC_MET_B | INTERCITY_BUS_CORRIDOR | world_master_corridor | 9,70 |
| BUS_IC_MET_COAST | INTERCITY_BUS_CORRIDOR | world_master_corridor | 11,41 |
| BUS_REG_A_WEST | REGIONAL_BUS_CORRIDOR | world_master_corridor | 4,67 |

In WORLD_700 gibt es **drei U-Bahn-Linien** (Nord–Süd-Hauptlinie, West–Ost-Hauptlinie, diagonale Tangentiallinie), einen Straßenbahn-Kernring und vier Buskorridore. Das ist **kleiner** als die Metropolis-Planung der größeren Welt (8–10 U-Bahn-Linien, 7–9 S-Bahn-Linien, [W-VK-006](20-Entscheidungsregister.md#8-straßen-schiene-verkehrsinfrastruktur-vk)).

---

## 9. Gelände und Technikdaten

| Datensatz | Inhalt | Quelle |
|---|---|---|
| Planungs-DEM | `terrain/preproduction_dem_v3.tif`, 10,8 m/Probe; 730.982 geänderte Zellen gegenüber der Vorversion | `G7-CLOSE` |
| Regional-DEMs | Südwest und Gebirge je 5,4 m | `G7-CLOSE` |
| Hydrologie | D8-Fließrichtung, Akkumulation, sechs benannte Einzugsgebietsklassen mit Konfidenz | `G7-CLOSE` |
| Höhenpunkte | 24 Anker, 4 Gipfel, 1 Pass, 12 Strukturkandidaten, 210 Höhenlinien | `G7-GPKG` |
| Gondelbahn | zwei Abschnitte (Stationen 1.323–1.750 m und 1.750–2.180 m Höhe; 3,59 km / 3,44 km horizontal; 11,9 % / 12,5 % mittlere Steigung) | `G7-CLOSE` |
| Hillclimb | 18,9 km, 1.430 m Höhengewinn, ø 7,56 % Gefälle, 17 Kehren, Mindestradius 330 m, Planungsbreite 8 m, Start 620 m, Ziel 2.050 m | `G7-CLOSE` |
| Bergzufahrt | Lösung ‚langer Talanstieg + Pass + Kehren + kurzer Tunnel‘, max. Ziel-Gefälle 8 % | `G7-CLOSE` |
| Ost-Tor der I1 | kurzer Tunnel plus Viadukt, max. Ziel-Gefälle 6 % | `G7-CLOSE` |
| Südwest-Damm | Normalstau 236,0 m, Kronenhöhe 244,0 m, Hochwasserentlastung 239,5 m, Freibord 8,0 / 3,5 m; nur Planungsplan, Speicherkurve und Bemessungshochwasser nicht modelliert | `G7-CLOSE` |

**Review-Markierungen** (`PASS_WITH_REVIEW_FLAGS`): Die D8-Konfidenz in Gebirgs- und Küstenbecken liegt unter Ingenieur-Niveau. Auf spätere Detailmaßstäbe übertragen wurden: Südwest-Straßenhöhenführung, Speicherkurve und Entlastungsdimensionierung, Tunnel/Viadukt am Ost-Tor, Bergzugangsbauwerke, Hillclimb-Sicherheit/Entwässerung, Gondelstützen, Bahntunnel/Viadukte/Erdarbeiten und Stadtblockstruktur. Empfohlen als erste Detailkarte: Mountain Regional.


---

## 10. Layerübersicht des GeoPackage

| Layer | Features |
|---|---:|
| admin_playable_land | 1 |
| admin_sea | 1 |
| admin_states | 7 |
| airport_runways | 3 |
| airport_terminals | 3 |
| airports | 3 |
| control_anchors | 22 |
| engineering_alignments | 16 |
| gondola_sections | 2 |
| hydro_cascades | 1 |
| hydro_d8_outlets | 6 |
| hydro_dams | 2 |
| hydro_lakes | 6 |
| hydro_rivers | 2 |
| hydro_streams | 12 |
| hydro_watersheds | 7 |
| hydro_wetlands | 1 |
| interchanges | 7 |
| labels | 36 |
| motorsport | 6 |
| park_ride | 4 |
| poi | 9 |
| rail_lines | 7 |
| rail_nodes | 9 |
| road_nodes | 26 |
| roads_hillclimb_detail | 1 |
| roads_major | 11 |
| roads_minor | 11 |
| sw_phase_b_review_flags | 6 |
| terrain_contours | 210 |
| terrain_elevation_anchors | 24 |
| terrain_passes | 1 |
| terrain_peaks | 4 |
| terrain_structure_candidates | 12 |
| transit_nodes | 7 |
| transit_routes | 8 |
| urban_areas | 7 |
| **Summe aller Tabellen** | **501** |

Die Summe aller Tabellenzeilen kann von den ‚495 Features‘ in `G7-CLOSE` abweichen, weil dort Hilfstabellen (zum Beispiel Review-Flags) anders gezählt sein können; die Abweichung wird nicht aufgelöst.
