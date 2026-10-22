#!/usr/bin/env python3
"""Render Docs/Game/World/24-WORLD700-GIS-Referenz.md from the WORLD_700 GeoPackage (read-only; stdlib only).
Usage: render_w700.py <path.gpkg> <out.md>"""
import json, math, sqlite3, sys
sys.path.insert(0, __import__("os").path.dirname(__file__))
gp, out = sys.argv[1], sys.argv[2]
con = sqlite3.connect("file:%s?mode=ro" % gp, uri=True)
import importlib.util
spec = importlib.util.spec_from_file_location("gm", __import__("os").path.join(__import__("os").path.dirname(__file__), "gpkg_metrics_lib.py"))
gm = importlib.util.module_from_spec(spec); spec.loader.exec_module(gm)

def rows(t):
    cols = [r[1] for r in con.execute('pragma table_info("%s")' % t)]
    for r in con.execute('select * from "%s"' % t):
        d = dict(zip(cols, r))
        try: d["p"] = json.loads(d.get("properties_json") or "{}")
        except Exception: d["p"] = {}
        try: d["m"] = gm.metrics(d["geom"])
        except Exception: d["m"] = ("?", 0, 0, 0, 0, 0)
        yield d
def f1(x): return ("%.2f" % x).replace(".", ",")
def f0(x): return "{:,.0f}".format(x).replace(",", ".")
L = []
w = L.append
w("""# Welt 24: WORLD_700 – GIS-Referenz (generiert)

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
""")
st = list(rows("admin_states"))
w("\n---\n\n## 2. Staaten\n\n| ID | Name | Fläche Polygon (km²) | Landfläche laut Datei (km²) | Binnenwasser (km²) | Meer (km²) | Anteil Spielfläche (%) | Nachbarn |\n|---|---|---:|---:|---:|---:|---:|---|")
for d in st:
    p = d["p"]
    w("| %s | %s | %s | %s | %s | %s | %s | %s |" % (d["feature_id"], d["name"], f1(d["m"][2]), f1(p.get("state_land_area_km2", 0)), f1(p.get("inland_water_area_km2", 0)), f1(p.get("sea_area_km2", 0)), f1(p.get("percentage_of_playable_land", 0)), ", ".join(x.replace("STATE_", "") for x in p.get("neighbors", []))))
w("\nSumme der sieben Staatspolygone (Rechnung): %s km². Die Datei führt die Verwaltungsfläche einschließlich Wasser getrennt (Spalten 4–6)." % f1(sum(d["m"][2] for d in st)))
ur = list(rows("urban_areas"))
w("\n---\n\n## 3. Siedlungen\n\n| ID | Name | Stufe | Staat | Fläche (km²) | Lage X / Y (m, grob) |\n|---|---|---|---|---:|---|")
for d in ur:
    w("| %s | %s | %s | %s | %s | %s / %s |" % (d["feature_id"], d["name"], d["p"].get("urban_tier", ""), (d["state_id"] or "").replace("STATE_", ""), f1(d["m"][2]), f0(d["m"][4]), f0(d["m"][5])))
w("\nStatus aller sieben Einträge in der Datei: `planned_locked_anchor`. Metropolis (State E) hat **%s km²**, Major City A %s km², Major City B %s km²; zusammen %s km² (%s %% der Spielfläche von 690,9 km², Rechnung). Die Datei nennt keine Kleinstädte und Dörfer als Polygone." % (f1(ur[0]["m"][2]), f1(ur[1]["m"][2]), f1(ur[2]["m"][2]), f1(sum(d["m"][2] for d in ur[:3])), f1(100 * sum(d["m"][2] for d in ur[:3]) / 690.903626)))
ap = list(rows("airports")); poi = list(rows("poi"))
w("\n---\n\n## 4. Flughäfen und Punkte von Interesse\n\n### 4.1 Flughafenflächen\n\n| ID | Staat | Fläche (km²) |\n|---|---|---:|")
for d in ap:
    w("| %s | %s | %s |" % (d["feature_id"], (d["state_id"] or "").replace("STATE_", ""), f1(d["m"][2])))
w("\n### 4.2 Punkte (POI)\n\n| ID | Name | Kategorie | Staat | X / Y (m) | Anzeigepriorität |\n|---|---|---|---|---|---:|")
for d in poi:
    w("| %s | %s | %s | %s | %s / %s | %s |" % (d["feature_id"], d["name"], d["class"], (d["state_id"] or "").replace("STATE_", ""), f0(d["m"][4]), f0(d["m"][5]), d["p"].get("display_priority", "")))
w("\nAlle POI führen ein Zustandsschema `unknown / discovered / visited / completed / locked / hidden` und sind als Entdeckungs-Zustände vorbereitet; das Hunter HQ trägt mit dem Flughafen Metropolis International die höchste Anzeigepriorität (90) – für die öffentliche Spielerkarte widerspricht das der späteren Geheimhaltungsregel ([W-KA-008](20-Entscheidungsregister.md#3-karte-und-abnahme-ka)); es ist ein Datenbefund der gesperrten Welt, keine Entscheidung für die neue.")
ms = list(rows("motorsport"))
w("\n---\n\n## 5. Motorsportstandorte\n\n| ID | Name | Staat | X / Y (m) | Priorität |\n|---|---|---|---|---:|")
for d in ms:
    w("| %s | %s | %s | %s / %s | %s |" % (d["feature_id"], d["name"], (d["state_id"] or "").replace("STATE_", ""), f0(d["m"][4]), f0(d["m"][5]), d["p"].get("display_priority", "")))
w("\nEs sind nur **Punkte**; Streckenverläufe, Längen und Zuschauerbereiche liegen nicht vor (Detailkarten „Track Detail“ wurden in `G7-CLOSE` ausdrücklich als nachgelagert geführt).")
lk = list(rows("hydro_lakes")); wl = list(rows("hydro_wetlands")); rv = list(rows("hydro_rivers")); dm = list(rows("hydro_dams")); sm = list(rows("hydro_streams"))
w("\n---\n\n## 6. Gewässer\n\n| ID | Name | Typ | Spiegel (m) | Fläche (km²) | Zuflüsse | Abfluss |\n|---|---|---|---:|---:|---|---|")
for d in lk:
    p = d["p"]
    w("| %s | %s | %s | %s | %s | %s | %s |" % (d["feature_id"], d["name"], p.get("lake_type", ""), p.get("surface_elevation_m", ""), f1(d["m"][2]), ", ".join(p.get("primary_inflows", [])) or "–", p.get("primary_outflow") or "–"))
for d in wl: w("\n**Feuchtgebiet:** %s, %s km², Hydroperiode `%s`." % (d["name"], f1(d["m"][2]), d["p"].get("hydroperiod", "")))
w("\n| Fluss | Von | Nach | Länge (km) | Einzugsgebiete |\n|---|---|---|---:|---|")
for d in rv:
    p = d["p"]; w("| %s | %s | %s | %s | %s → %s |" % (d["feature_id"], p.get("from_node"), p.get("to_node"), f1(d["m"][3]), p.get("upstream_basin"), p.get("downstream_basin")))
w("\nBäche: %d Features, zusammen %s km (Rechnung). Dämme: %s." % (len(sm), f1(sum(d["m"][3] for d in sm)), ", ".join("%s (%s)" % (d["name"], d["feature_id"]) for d in dm)))
w("\nWasserkette in den Daten: **Lake Alder → Fluss RIVER_ALDER_OUT → Lake Meridian → RIVER_MERIDIAN → Greywater-Auslass/-Feuchtgebiet → Küste**; das Südwest-Talreservoir (`LAKE_SOUTHWEST_01`) gehört zu einem eigenen Südbecken. Dieselben Namen und eine ähnliche Kette tauchen im Prototypauftrag vom 29.09. wieder auf (`30#232E`); die Kette gilt **nicht** automatisch für den Kandidaten vom 18./19.10.")
rd = list(rows("roads_major")) + list(rows("roads_minor"))
w("\n---\n\n## 7. Straßen\n\n| ID | Klasse | Von → Nach | Länge (km) | Spuren (ca.) | Brücke | Tunnel | Zugang |\n|---|---|---|---:|---:|:---:|:---:|---|")
for d in rd:
    p = d["p"]
    w("| %s | %s | %s → %s | %s | %s | %s | %s | %s |" % (d["feature_id"], p.get("road_class", d["class"]), p.get("from_node", "–"), p.get("to_node", "–"), f1(d["m"][3]), p.get("approximate_lanes", "–"), "ja" if p.get("bridge") else "nein", "ja" if p.get("tunnel") else "nein", p.get("access_type", "–")))
w("\nSumme Hauptnetz (11 Kanten): %s km; Nebenstraßen (%d Kanten): %s km (Rechnung). Die Interstate-Kette I1 (West → Major A → Metropolis → Major B → Ost-Tor) ist das Rückgrat." % (f1(sum(d["m"][3] for d in list(rows("roads_major")))), len(list(rows("roads_minor"))), f1(sum(d["m"][3] for d in list(rows("roads_minor"))))))
rl = list(rows("rail_lines")); tr = list(rows("transit_routes"))
w("\n---\n\n## 8. Schiene und Nahverkehr\n\n### 8.1 Bahnstrecken\n\n| ID | Klasse | Von → Nach | Länge (km) | Elektrifizierung |\n|---|---|---|---:|---|")
for d in rl:
    p = d["p"]; w("| %s | %s | %s → %s | %s | %s |" % (d["feature_id"], p.get("rail_class"), p.get("from_node"), p.get("to_node"), f1(d["m"][3]), p.get("electrification")))
w("\nSumme: %s km (Rechnung). Spurzahl: `%s`; Elektrifizierung in allen Kanten ‚geplant, unspezifiziert‘." % (f1(sum(d["m"][3] for d in rl)), rl[0]["p"].get("track_count")))
w("\n### 8.2 Nahverkehrs- und Buskorridore\n\n| ID | Klasse | Rolle | Länge (km) |\n|---|---|---|---:|")
for d in tr:
    p = d["p"]; w("| %s | %s | %s | %s |" % (d["feature_id"], p.get("transit_class"), p.get("service_role"), f1(d["m"][3])))
w("\nIn WORLD_700 gibt es **drei U-Bahn-Linien** (Nord–Süd-Hauptlinie, West–Ost-Hauptlinie, diagonale Tangentiallinie), einen Straßenbahn-Kernring und vier Buskorridore. Das ist **kleiner** als die Metropolis-Planung der größeren Welt (8–10 U-Bahn-Linien, 7–9 S-Bahn-Linien, [W-VK-006](20-Entscheidungsregister.md#8-straßen-schiene-verkehrsinfrastruktur-vk)).")
w("""
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
""")
w("\n---\n\n## 10. Layerübersicht des GeoPackage\n\n| Layer | Features |\n|---|---:|")
tot = 0
for (t,) in con.execute("select table_name from gpkg_contents order by table_name"):
    n = con.execute('select count(*) from "%s"' % t).fetchone()[0]; tot += n
    w("| %s | %d |" % (t, n))
w("| **Summe aller Tabellen** | **%d** |" % tot)
w("\nDie Summe aller Tabellenzeilen kann von den ‚495 Features‘ in `G7-CLOSE` abweichen, weil dort Hilfstabellen (zum Beispiel Review-Flags) anders gezählt sein können; die Abweichung wird nicht aufgelöst.")
open(out, "w", encoding="utf8").write("\n".join(L) + "\n")
print("written", len(L))
