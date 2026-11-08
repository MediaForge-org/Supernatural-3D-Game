# Welt 17: Weltgenerierung und GIS-Daten

> [!IMPORTANT]
> **Es gibt kein GIS der größeren Welt.** Bestehende GIS-Dateien betreffen nur die gesperrte Referenzwelt WORLD_700. Alles, was für die größere Welt „architektonisch“ klingt, ist 💡 und nicht beschlossen. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (TE, KA).

Teil der Welt-Dokumentation ([Index](README.md)). Unreal-Seite: [Modul 18](18-Unreal-Landscape-World-Partition-und-Streaming.md); Datenbestand: [Modul 24](24-WORLD700-GIS-Referenz.md), [Modul 19](19-Kartenreferenzen-und-Versionsgeschichte.md).

**Inhalt**

1. [Bestehende gegenüber geplanten Daten](#1-bestehende-gegenüber-geplanten-daten)
2. [Koordinatensystem](#2-koordinatensystem)
3. [Geografische Konsistenz: Anforderungen](#3-geografische-konsistenz-anforderungen)
4. [Heightmaps und Gelände](#4-heightmaps-und-gelände)
5. [Reproduzierbarkeit und Versionierung](#5-reproduzierbarkeit-und-versionierung)
6. [Prüfkatalog](#6-prüfkatalog)
7. [Datenschichten](#7-datenschichten)
8. [Mehrfachauflösung und Zoom](#8-mehrfachauflösung-und-zoom)
9. [Geheimnisschicht](#9-geheimnisschicht)
10. [Zeitschichten](#10-zeitschichten)
11. [Prozess: Von der Skizze zur Spielwelt](#11-prozess-von-der-skizze-zur-spielwelt)
12. [Offene Punkte](#12-offene-punkte)

---

## 1. Bestehende gegenüber geplanten Daten

| Gegenstand | Stand | Status |
|---|---|:---:|
| WORLD_700: GeoPackage (495 Features), 12K-Raster, DEMs 10,8 m und 5,4 m, D8-Hydrologie, SVG, QGIS-Projekt (leer 389 B), WKT | vorhanden im Master | 🕰️ (gesperrt) |
| WORLD_700: 11 Regionalkarten (A.2) | beschlossen (`APPROVED_LOCKED`), Regionalmaster nicht erzeugt | 🕰️ |
| Größere Welt: Geometrie, DEM, CRS | **nicht vorhanden** | ❓ |
| Testwelt 2.200 km² (V1.x): 50-m-DEM 1.348 × 680, 18 Regionen, Blueprint-GPKG | nur berichtet, Dateien fehlen | 🕰️ |
| `SPN_SCALE_MASTER_V1` | beschrieben, Rohdaten fehlen | ❓ |
| Projekt-Pipeline | im Chat berichtet (Python-Generatoren, GeoJSON/GPKG, Viewer) | 🕰️ |

---

## 2. Koordinatensystem

- **OWP-LCS-1** (WORLD_700): fiktives lokales Ingenieurkoordinatensystem, kartesisch, Meter, Ursprung Südwest, X Ost, Y Nord, Z oben; Raster-Ursprung oben links; Umrechnung `WorldX = PixelX × 2,7`, `WorldY = (8192 − PixelY) × 2,7` (G7-LCS). GeoJSON-Koordinaten sind Meter, **nicht** Längen-/Breitengrad. WKT: `ENGCRS`, ID OWP:LCS-1.
- Für die größere Welt verlangen die Quellen: ein autoritatives CRS mit dokumentierten Einheiten, Grenzen und Maßstab; Flächen mit geeigneter flächentreuer Methode oder kontrolliertem planarem CRS (D19-CHK Nr. 1, 💡). Welches CRS, Toleranzen, native Rasterauflösung: offen (D19-OQ Nr. 6).
- Der 29.09.-Prototypauftrag arbeitete mit lokalen Kilometerkoordinaten (Ursprung SW, 1-km-Raster); das ist 🕰️.
- **Vorschlag (💡):** Das neue CRS als Fortsetzung von OWP-LCS-1 definieren, mit Weltursprung und Achsen unverändert, um Werkzeuge wiederzuverwenden. Die Übernahme ist **keine Entscheidung**; sie berührt die Regel ‚keine Geometrie-Übertragung‘ nicht, solange kein Objekt kopiert wird.

---

## 3. Geografische Konsistenz: Anforderungen

Alle 📋 aus D18-MAP, D19-REQ, D18-ACC:

1. **Zentrale Datenquelle:** eine geografisch maßgebliche Karte; keine unabhängig erzeugten Küsten, Flüsse, Seen oder Stadtstandorte.
2. **Identische Koordinaten und Maßstäbe** in allen Ausgaben; Labels dürfen variieren, Gelände, Hydrografie, Küste und Siedlungslagen nicht.
3. **Konsistente Straßen-, Fluss- und Gebirgsverläufe** (Verkehr folgt Gelände, Flüsse folgen Tälern).
4. **Reproduzierbarkeit:** Skripte/Pipeline statt Einzelbilder.
5. **Versionierung:** echte SHA-256, README, CHANGELOG, Manifest.
6. **Nachprüfbare Flächenverhältnisse:** 2.200 / 110 / 2.090 / Insel getrennt (W-KA-011).
7. **Einheitliche Ableitung** von Weltkarte und Detailkarten aus einer Geometrie.
8. **Ehrliche Auflösung:** native Größe, Skalierungsfaktor und Herkunft offenlegen.
9. **Statusklassen:** `CANDIDATE`, `VISUAL_NOT_ACCEPTED`, `ACCEPTED_MASTER`, `TECHNICALLY_UNVALIDATED`.

---

## 4. Heightmaps und Gelände

- Gebirge im Norden, natürlich; Reliefberechnung soll Seeabflüsse und Flusstopologie tragen (D19-REQ).
- Das Terrain muss **zuerst** entstehen, Städte daraus abgeleitet (Assistent 45#62A, 💡).
- Die Forschungsreihe V3.1/V1.3 lieferte **kein** akzeptiertes Terrain. Berichtete Erkenntnisse: reine Synthese erzeugte unplausible Höhen (−1.369 m bis 5.032 m, 45#253E); der Vorschlag lautet daher ein handkuratierter, editierbarer Kontrollgraph aus Kämmen, Tälern, Seen, Höhenankern und Abflüssen (45#253E, 💡). Der Entwickler wollte stattdessen sichtbare Gesamtergebnisse (45#203E) und stoppte die Mikropilote (D18-ACC).
- Referenz für WORLD_700: DEM v3 mit 24 Höhenankern, 4 Gipfeln, 1 Pass; Hydrologie D8.
- Gipfelhöhe der neuen Welt ist offen (K4).

**Begriff 8,6 versus 18,9 km (WORLD_700):** Die Vektorkante `ROAD_HILLCLIMB` misst 8,6 km (Planungskante), die Ingenieurstrecke 18,9 km (G7-CLOSE). Ingenieurdatensätze sind **detaillierter** als die Vektorkanten.

---

## 5. Reproduzierbarkeit und Versionierung

- Deterministische Wiederholung wird als Prüfpunkt berichtet (identische Hashes, 45#80E, 45#265E).
- Generator + Daten in einem ZIP je Iteration (30#104E).
- Keine großen Binärassets in Git (CLAUDE.md §10; Projektregel).
- **Vorschlag (💡):** Metadaten, Hashes und Skripte im Repository, Rohrasterdaten außerhalb; Manifest mit SHA-256 je Datei; PCG-Seeds und Versionsmetadaten (Prompt `021_create-pcg-environment-seed-version-metadata`).

---

## 6. Prüfkatalog

Aus D19-CHK (💡, ‚measurable acceptance checklist‘): Flächenbilanz mit Toleranz; Insel überlappt Festland nicht; Seen innerhalb Landmaske, aber subtrahiert; gerichteter Flussgraph; Seenlage gegen die Papierskizze; Geheimnisebene getrennt; Siedlungspolygone statt Symboldurchmesser; alle Exporte aus einer Geometrie (Feature-ID und Koordinaten vergleichen). Erzeugnisse (später): `area_report.json`, `feature_ids.geojson`, `river_graph_report.json`, `georeference.json`, `visual_acceptance.md`, Prüfsummen.

**Zusätzlicher Konsistenzvorschlag (💡):** automatisierte Tests im Repository, die Flächenbilanz, Flussgraph-Zusammenhang, Namens-/ID-Eindeutigkeit und Geheimnisfreiheit aus dem Geometrie-Export prüfen – analog zu den Skripten in `Tools/world-audit/`.

---

## 7. Datenschichten

Beispiel der Referenzwelt (G7-GPKG, G7-LCS): `admin_*`, `urban_areas`, `roads_major/minor`, `rail_lines`, `rail_nodes`, `transit_routes`, `transit_nodes`, `airports`, `motorsport`, `poi`, `hydro_*`, `terrain_*`, `engineering_alignments`, `labels`, `control_anchors`. Jedes Feature hat unveränderliche `feature_id`; Namen dürfen sich ändern; POIs tragen Entdeckungszustand und Spoiler-Attribute; Zoomstufen Z0–Z6 mit Sichtbarkeitsregeln (Label-Hierarchie Ebene 1: Metropolis, Major Cities, große Seen, Hauptgebirge; Ebene 2: Mittelstädte, Stationen, Flughäfen, Motorsport, Hauptflüsse; Ebene 3: Kleinstädte, Dörfer, Nebenseen, lokale POI, Park&Ride; Ebene 4: lokale Straßen).

---

## 8. Mehrfachauflösung und Zoom

WORLD_700: Phase A.2 mit 11 Regionen, 20K/24K, ~0,45–0,50 m/px, Überlappung 5–10 % zur Nahtprüfung; Hochskalieren verboten, tiefe Ebenen direkt aus DEM/Vektor/Landnutzung rendern (G7-PA; 30#194E). Das ist das belegte **Vorbild**; für die größere Welt nicht entschieden.

---

## 9. Geheimnisschicht

Das Hunter-HQ und andere Geheimorte liegen in einer getrennten, nicht öffentlichen Ebene (gilt für Spieler- und Exportkarten; die Beschreibung der HQ-Lage in der Weltdokumentation ist freigegeben, W-KA-017); ein Test prüft, dass kein exportiertes Spielerbild und kein GeoJSON ein „Hunter HQ“ enthält (D19-CHK Nr. 6). WORLD_700-Daten führen das HQ als Punkt mit hoher Priorität; das ist für die neue Karte nicht zu übernehmen.

---

## 10. Zeitschichten

Geografie ist die stabile Schicht: Seen, Berge und Flüsse bleiben beim Zeitreisen (W-RE-006); Siedlungen und Infrastruktur sind zeitversioniert (M-CA0905 §2). Folge (💡): GIS braucht eine Zeitdimension für Siedlungs- und Verkehrslayer, nicht für Terrain und Hydrologie.

---

## 11. Prozess: Von der Skizze zur Spielwelt

Reihenfolge laut Quellen: (1) Anforderungen und Spatial Allocation (M-W15R), (2) visuelle Gesamtkomposition und Abnahme (D18-ACC), (3) Georeferenz und Hydrologie-Prüfung, (4) Detailkarten aus dem Master, (5) erst dann Unreal-Landscape. Die Methode ‚maßstäbliche geografische Grundkarte → Topografie, Wasser, Vegetation, Infrastruktur → visuelle Darstellung‘ stammt vom Assistenten (45#62A, 💡); der Entwickler bewertete die damalige Karte als nicht gut (45#61E, Punkt 6); eine ausdrückliche Zustimmung zu genau dieser Methode ist nicht belegt.

---

## 12. Offene Punkte

CRS und Toleranzen; Quelle der Geometrie (Handzeichnung, Generator, GIS-Digitalisierung der Skizze); Verhältnis zu `SPN_SCALE_MASTER_V1`; Rohdaten der Kandidaten; DEM-Auflösung; Form der Landmaske; Format für Landschaft → Unreal.

---

## Nachtrag Master-Abgleich (08.11.2026)

Ergänzungen aus dem Abgleich mit dem Planungsmaster. Quellen in Klammern: Master-Dateien unter `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/`, Kürzel siehe `Docs/Planning/SOURCES.md`. Blockierte Punkte sind als offen markiert und nicht entschieden (`Docs/Planning/CONFLICTS.md`).

### Ergänzungen aus dem Master-Abgleich (08.11.2026)
- **Flächenbilanz mit Toleranz** (Master: `D19-CHK:8`): Die Akzeptanz verlangt eine Bilanz mit Toleranz. Die Toleranz ist nicht festgelegt und bleibt offen.
- **Phase-A-Freigabe** (Master: `W700-PA:24–25`): Das Manifest ist erst nach Freigabe maßgeblich. Welche Fassung gilt (9 oder 11 Regionen), ist *blockiert (K-19)*.
