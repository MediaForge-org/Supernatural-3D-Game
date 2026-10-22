# Welt 05: Gewässer und Hydrologie

> [!IMPORTANT]
> **Keine Hydrografie des Kandidaten ist freigegeben.** Genaue Ufer, Lagen und Abflüsse bleiben dem akzeptierten Kartenmaster überlassen (D18-MAP). Die Hydrologie der Referenzwelt WORLD_700 ist als Vorbild für Datenstruktur und Prüfregeln dokumentiert, **nicht** als Layout der neuen Welt. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md) (WA); GIS-Daten der Referenzwelt: [Modul 24](24-WORLD700-GIS-Referenz.md).

**Inhalt**

1. [Anforderungen an das Wassersystem](#1-anforderungen-an-das-wassersystem)
2. [Die benannten Gewässer](#2-die-benannten-gewässer)
3. [Der Hauptfluss](#3-der-hauptfluss)
4. [Küste, Bucht und Hafen](#4-küste-bucht-und-hafen)
5. [Hydrologische Regeln und Prüfungen](#5-hydrologische-regeln-und-prüfungen)
6. [Wasser und Gameplay](#6-wasser-und-gameplay)
7. [Vorbild WORLD_700: Datenmodell der Hydrologie](#7-vorbild-world_700-datenmodell-der-hydrologie)
8. [Terrain-Forschung zur Hydrologie](#8-terrain-forschung-zur-hydrologie)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Anforderungen an das Wassersystem

| Anforderung | Status | Quelle |
|---|:---:|---|
| Flüsse, Kanäle, Sumpfarme und zersplittertes Wasser **deutlich reduzieren**; wenige zusammenhängende natürliche Entwässerungssysteme, glaubhafte Seeabflüsse | 📋 | D18-MAP |
| Wasser dominiert das bewohnbare Land nicht; nicht alle Siedlungen werden Wasserstädte | 📋 | D18-MAP |
| Benannte Seen: Lake Alder (mit Alder Isle), Lake Meridian, South Reservoir; dazu Greywater Bay | 🧭 | D18-MAP; D19-REQ |
| Ruhigere, organisch regelmäßige Ostküste; eine große Insel | 📋 | D18-MAP; D19-REQ |
| Natürliche Seeabläufe und durchgängige Flusstopologie | 📋 | D19-REQ |
| Wasser folgt dem Gelände; Flüsse mäandrieren in flachem Gelände, bilden Auen, beeinflussen Straßen und Siedlungen | 💡 | 30#232E (Auftragstext) |
| Wasserflächen sind nicht Teil des Trockenlands und werden nicht doppelt gezählt | 📋 | D19-REQ; D19-CHK |
| Seen sind Aktivitätsräume (Tauchen, Surfen, Angeln, Wassersport) und deshalb nicht zu klein | 📋 | 45#8E; 23#779E |

---

## 2. Die benannten Gewässer

| Name | Kandidat 18./19.10. | Frühere Hinweise | WORLD_700 (gemessen) |
|---|---|---|---|
| **Lake Alder** | ca. 25 km² mit Alder Isle; Binnensee im nördlichen/westlichen Seensystem | 04.10.: „auf 25km²“ gewünscht, damit man Tauchen, Surfen, Angeln machen kann (45#8E); 29.09.: 15–22 km² (max ~25) im Auftrag; 01.10.: ‚Seen sollen etwas größer sein‘ (35#935E) | 6,3 km², 412 m ü. NN |
| **Lake Meridian** | ca. 5 km² | 29.09.: 5–8 km² (max 10) im Auftrag; Metropolis darf einen Teil des Ufers erreichen | 4,9 km², 286 m |
| **South Reservoir** | ca. 7 km², weiter im Südosten als in kritisierten Entwürfen | – | Südwest-Talreservoir 2,1 km², 236 m (Stausee mit kleinem Damm) |
| **Greywater Bay** | benannt, Form offen | – | kleine Bucht (V3-Entwurf) |
| **Alder Isle** | Insel im Lake Alder | – | – |
| Eagle Lake, Summit Lake, Pine Hill Lake | **nicht** für den Kandidaten genannt | – | Bergseen 0,5–0,8 km², Pine Hill 0,5 km² |
| Greywater-Feuchtgebiet | reduziert; „Greywater“ ist **Hafenstadt**, nicht Sumpf | 29.09.: 50–80 km² | 8,7 km² |

Quellen: D18-MAP, D19-REQ, 45#8E, 30#232E, G7-GPKG. Die Spalten gehören zu **verschiedenen** Welten und Zeitpunkten.

**Zusammenhang der Namen:** In WORLD_700 gehören Alder → Meridian → Greywater zu **einem** Becken (Fluss `RIVER_ALDER_OUT` und `RIVER_MERIDIAN`). Der Kandidat verlangt wieder eine Verbindung Alder – Metropolis, fügt aber ausdrücklich die Bedingung hinzu, dass der Fluss weder durch Glenhaven noch durch die Innenstadt führt.

**Rechnung:** Die drei benannten Seen des Kandidaten haben zusammen etwa 37 km², das sind 1,7 % des trockenen Festlands (37 / 2.200; nicht gleich dem Anteil an der Gesamtkarte). Der Prototypauftrag vom 29.09. wollte Seen und Flüsse bei ~5 % der Fläche (30#232E); das lässt sich mit der Oktober-Forderung nach **weniger** Wasser nicht ohne Weiteres vereinbaren.

---

## 3. Der Hauptfluss

Maßgeblich (D19-REQ, D19-CHK):

- Er verläuft **vom westlichen/nördlichen Seensystem** an den **nördlichen äußeren Bezirken von Metropolis** vorbei zur **Ostküste**.
- Die Strecke zwischen **Lake Alder und Metropolis** darf **nicht abrupt enden**.
- Er geht **nicht** durch **Glenhaven**, **nicht** durch die **Innenstadt** von Metropolis.
- Für Metropolis gilt: ein dominanter Fluss durch bzw. entlang eines äußeren Bezirks; Inseln, Arme und Brücken begrenzt (D18-MAP, W-ST-007).
- Prüfung: gerichteter Flussgraph mit Fließrichtung, Seeabflüssen und Küstensenke; zu markieren sind freie Enden, unerwartete Gabelungen und unplausible Kreuzungen (D19-CHK Nr. 4).

**Name:** In den Aufträgen heißt der Fluss ‚Meridian River‘ (30#245E, 30#265E, Auftragstexte); `D18-MAP` und `D19-REQ` nennen ihn nicht beim Namen. Der Name ist daher 🧭.

**Konflikt mit früherer Idee:** Der Assistent hatte am 09.10. einen ‚breiten Fluss tatsächlich durch die Innenstadt‘ vorgeschlagen (45#62A, 💡). Die Oktober-Vorgabe schließt das aus (W-ST-007).

---

## 4. Küste, Bucht und Hafen

- Ostküste ruhig; keine Labyrinthe aus Mündungen und Inseln (W-WA-005).
- **Greywater** als einzige klar maritime Hafenstadt: glaubwürdiger Hafen, nicht Venedig, keine Kanäle, nicht in überflutetes Sumpfland gebettet (W-ST-015).
- Eine Küstenstadt am Meer (31.08.: ‚nur eine kleinere Küstenstadt am Meer‘, 23#821E) ist historische Planung; Greywater erfüllt diese Rolle.
- Marinas und Bootsstationen sind Teil von Block 4/5: Boote werden am Marina gelagert, Tauchen und Erkunden per Boot (M-WB17).
- Hafen- und Frachtinfrastruktur: Häfen, Fischerei, Meeresfracht gehören zur Wirtschaft (M-WB17 Block 2); Lage und Größe offen.

---

## 5. Hydrologische Regeln und Prüfungen

Aus D19-CHK (Prüfvorschlag, 💡; keine genehmigte Methode):

1. Ein autoritatives Koordinatensystem mit Einheiten, Grenzen und Maßstab; Flächen in km² mit geeigneter flächentreuer Methode oder kontrolliertem planarem CRS.
2. Trockenes Festland = 2.200 km²; Metropolis-Polygon = 110 km² innerhalb des Festlands; `Festland − Metropolis` = 2.090 km²; Insel getrennt; Seen und Reservoir **abziehen**; Toleranz vorher definieren.
3. Insel und Festland überlappen nicht; Küste und Inselabstand prüfen; Seen liegen in der Landmaske, aber werden abgezogen.
4. Fluss als verbundener gerichteter Graph.
5. Namen und Lagen der Seen gegen das Papier prüfen; Abstände bindend erst nach akzeptierter Geometrie.
6. Geheimnisebene (kein „Hunter HQ“ in öffentlichen Spieler-/Exportkarten; die Weltdokumentation darf die Lage beschreiben, W-KA-017).
7. Siedlungsflächen als Polygone; Symboldurchmesser ≠ Fläche.
8. Alle Regional-/Detailexporte aus **derselben** Master-Geometrie; Feature-IDs und Koordinaten vergleichen (Küstendrift verhindern).

Visuelle Prüfungen: Fluss sichtbar durchgängig, nicht durch Downtown/Glenhaven, Greywater an der Küste, Inselsilhouette erhalten.

---

## 6. Wasser und Gameplay

- **Unterwasserwelt:** sehr ausgeprägt, mit vielen Tieren, Schwärmen, Vegetation, Felsen, Wracks, Höhlen, Tiefzonen, überfluteten Ruinen (23#1035E Nr. 72; M-WB17).
- **Sportarten:** Tauchen, Schnorcheln, Jetski, Speedboote, Segeln, Yachting, Wasserski, Wakeboard, Surfen, Klippen-/Brückenspringen, Kajak, Rafting (M-WB17 Block 7).
- **Angeln:** tiefe Angelmechanik und Turniere (M-WB17).
- **Hochwasser, Eis, Strömung** sind als Wetterereignisse genannt (Überschwemmungen, M-WB17 Block 6); eine Strömungs- oder Wassersimulation ist **nicht** spezifiziert.
- **Staudämme:** In WORLD_700 gibt es zwei Dämme (Old Meridian Dam am Meridian-Fluss, Southwest Valley Dam); ob die größere Welt Dämme enthält, ist ❓.
- Mehr Küste oder größere Seen sind laut `M-W15R` eine **offene Gestaltungsoption**, weil die Unterwasserwelt ein Hauptlayer ist.

---

## 7. Vorbild WORLD_700: Datenmodell der Hydrologie

Das GeoPackage (`G7-GPKG`) trennt: `hydro_lakes` (6), `hydro_rivers` (2 Hauptflüsse), `hydro_streams` (12), `hydro_wetlands` (1), `hydro_dams` (2), `hydro_watersheds` (7), `hydro_d8_outlets` (6), `hydro_cascades` (1). Wichtige Eigenschaften je Feature:

- Seen: Typ (`natural_lake`, `mountain_lake`, `reservoir`), Spiegelhöhe, Becken-ID, Zu- und Abflüsse, Flag `flat_surface_validated`.
- Flüsse: `from_node`, `to_node`, Fließrichtung ‚vom ersten zum letzten Stützpunkt‘, Ober- und Unterbecken.
- D8: Fließrichtung, Akkumulation, sechs benannte Einzugsgebietsklassen mit Konfidenz; Konfidenz in Gebirgs- und Küstenbecken unter Ingenieur-Niveau (G7-CLOSE).
- Damm: Normalstau, Höchstplanungsspiegel, Hochwasserentlastung, Kronenhöhe, Freibord (nur Planungsplan; Speicherkurve und Bemessungshochwasser nicht modelliert).

Dieses Schema ist ein **belegtes Beispiel**, wie die Projekt-Pipeline Hydrologie führt. Für die größere Welt ist es ein Vorschlag (💡) und nicht als Format beschlossen.

---

## 8. Terrain-Forschung zur Hydrologie

Berichtete Ergebnisse der V3.1-Reihe (45#80E–45#205E, **nicht überprüft**):

- Ausgangsfehler V2: Lake Alder → Meridian ohne durchgängige Abflussverbindung; 28.781 bergauf gerichtete D8-Kanten.
- V3.1: Verbindung Alder → Meridian → Meer funktioniert, vollständiger azyklischer D8-Graph, deterministische Wiederholung.
- Reihe bis V3.1t: Priority-Flood, USGS-Referenzen (Glacier, Smokies), Talnetz mit 15 Abschnitten; Ergebnis `NOT_ACCEPTED` für Terrain, 116/121 unabhängige Tests im letzten Schritt.
- V1.3d: zwei Gebirgs-Ansätze verglichen, **keiner** übernommen; Seespiegel-Konflikt Lake Alder 460 m vs. 459 m, ‚beide bleiben getrennt dokumentiert‘ (45#253E).
- V1.3e: gerichteter Entwässerungsgraph mit ‚Soll-Flüssen‘; geplante Verbindungen nicht durchgehend vom ursprünglichen DEM bestätigt (45#257E).
- V1.3e.3: ‚24 Hydrologie-Verbindungen weiterhin ungeprüft‘ (45#274E).

Der Entwickler hat die Forschungsreihe am 11.10. mit ‚wann gibt es wirklich eine map … nur Teile und nichts Sichtbares‘ (45#203E) kritisiert; `D18-ACC` legt deshalb fest, **keine** weitere lange D8/DEM-Mikropilotforschung als Ersatz für eine überzeugende Gesamtkarte zu betreiben (W-KA-009).

---

## 9. Offene Punkte

1. Lage, Form und Abfluss der drei Seen; Zugehörigkeit des Südreservoirs zu einem Becken.
2. Form von Greywater Bay und Küste; Name und Form der Insel.
3. Flussverlauf in Zahlen (Länge, Gefälle, Breite); Brücken über den Hauptfluss in Metropolis.
4. Ob es Dämme, Kanäle (für Handel/Industrie) oder Hochwasserschutz gibt.
5. Abstimmung der Seen-Größen mit der Forderung nach weniger Wasser.
6. Unterwasserwelt: Tiefenprofil, Wracks, Höhlen.
7. Quellen und Mündung der unterirdischen Flüsse ([Modul 11](11-Untergrund-Architektur-und-Infrastruktur.md)).
