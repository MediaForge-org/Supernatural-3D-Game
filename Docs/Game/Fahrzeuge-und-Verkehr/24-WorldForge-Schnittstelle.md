# Fahrzeuge und Verkehr 24: WorldForge-Schnittstelle

> [!IMPORTANT]
> **Es gibt keinen beschlossenen Austauschvertrag zwischen WorldForge und dem Spiel für Straßen.** WorldForge (`SPN_WorldForge`, Nachbarprojekt außerhalb dieses Repositories) hat bisher nur synthetisches Terrain implementiert (M0.1); der Unreal-Import ist dort als noch zu belegen geführt. Für Straßen existiert ein **M2-Vorvertrag** (`RC-01`), der Graph, Kanten, Querungen und Kreuzungen beschreibt, aber keine Fahrspur-Verbindungen, Geschwindigkeitsbegrenzungen, Schilder, Signale, Parkplätze oder Tankstellen. Dieses Modul stellt gegenüber, was dort steht und was das Spiel braucht, und nennt Kandidatenfelder als **Vorschlag**, nicht als Format. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [Welt 17](../World/17-Weltgenerierung-und-GIS-Daten.md), [Modul 10](10-Strassennetz-und-Strassentypen.md), [Modul 11](11-Kreuzungen-Signale-und-Schilder.md).

**Inhalt**

1. [Stand von WorldForge](#1-stand-von-worldforge)
2. [Was `RC-01` vorsieht](#2-was-rc-01-vorsieht)
3. [Bedarf des Spiels gegenüber `RC-01`](#3-bedarf-des-spiels-gegenüber-rc-01)
4. [Kandidatenfelder](#4-kandidatenfelder)
5. [Wer liefert die Fahrspur-Ebene](#5-wer-liefert-die-fahrspur-ebene)
6. [Stabile IDs, Überlagerung und Neuimport](#6-stabile-ids-überlagerung-und-neuimport)
7. [Validierung auf Spielseite](#7-validierung-auf-spielseite)
8. [Platzhalter für die frühen Phasen](#8-platzhalter-für-die-frühen-phasen)
9. [Register](#9-register)
10. [Offene Punkte](#10-offene-punkte)

---

## 1. Stand von WorldForge

| Aussage | Quelle |
|---|---|
| WorldForge ist ein eigenständiger Rust-Weltgenerator. M0.1 umfasst eine deterministische synthetische 2 × 2 km Terrainregion, Heightmaps, Manifeste, Ressourcenprüfung und Snapshot-Aktivierung. Der echte Unreal-Import bleibt offen und ist `UNVERIFIED`. | `WF-RD` (README des Projekts) |
| M2 (‚Road & Infrastructure Generation‘) ist als Ziel beschrieben: hierarchische Straßengraphen, Trassenoptimierung, 3D-Fahrbahnkörper, Kreuzungen, Brücken, Tunnel, Inselversorgung, Regression gegen gerade Straßen, Strecken ins Meer und unerschlossene Regionen. | `WF-PL` |
| M3 erzeugt Stadt-, Quartiers- und Parzellenstrukturen **mit Straßenzugängen** und eine Asset-Bedarfsliste; Schilder, Markierungen und Laternen kommen aus dem Asset-Katalog in M4. | `WF-PL`; `WF-RG` |
| Die lokal installierte Unreal-Version ist noch nicht erhoben. | `WF-UE` |
| Die Weltgröße von etwa 2.200 km² ist dort wie hier ein Arbeitsszenario. | `WF-UE` (Reimport-Ziel) |

WorldForge ist ein Nachbarprojekt mit eigener Governance. Seine Dokumente binden dieses Spiel nicht, und umgekehrt entscheidet dieses Modul nichts für WorldForge.

---

## 2. Was `RC-01` vorsieht

`WF-RC01` (‚M2-Vorvertrag, nicht schon in M0.4 als vollständigen Straßengenerator interpretieren‘):

| Objekt | Felder (Auszug aus der Quelle) |
|---|---|
| `SettlementNode` | node_id, type, centroid_m, population/demand_index (nur Profilwert), required_service_class, connectivity_status |
| `RoadNode` | stable_id, world_xy_m, world_z_m (optional), junction_kind, allowed_road_levels, connected_edge_ids |
| `RoadEdge` | stable_id, start_node_id, end_node_id, road_class, level, semantics (urban/interurban/service), route_intent_id, horizontal_centerline_m, vertical_profile_m, width_profile_m, lanes, shoulder_type, design_constraints, grade_profile, curvature_profile, terrain_modifiers, spans, source_version |
| `Crossing` | stable_id, crossing_kind (`at_grade`, `bridge`, `tunnel`, `underpass`, `ferry`, `causeway`), traversed_feature_stable_id, clearance_m, abutment/foundation geometry, asset_requirement_ids, validated_status |
| `Intersection` | stable_id, attached_edge_ids, junction_type, turn_constraints, vertical_grade_relations, mesh/spline output_ref |

Weitere Zusagen: Validatoren (Konnektivität, keine Geisteranschlüsse, Wasserüberschreitung braucht ein Crossing, konfigurierbare Zahlenprofile statt globaler Konstanten), Regressionsfälle, und ‚Mesh/Spline-Export referenziert dieselben RoadEdgeIDs, CrossingIDs und Höhenkoordinaten‘ (`WF-RC01`). Der Straßengraph ist von der metrischen 3D-Trasse getrennt (`WF-RG`); `DOMAIN-03` nennt zusätzlich Richtung und Kapazitätsannahme der Kanten sowie die Owner-Region (`WF-D03`).

---

## 3. Bedarf des Spiels gegenüber `RC-01`

Der Auftrag nennt Kandidaten für die Schnittstelle: Straßentyp, Spuren, Richtung, Geschwindigkeit, Kreuzungen, Schilder, Navigationsverknüpfungen, Parkflächen, Brücken und Tunnel. Der Abgleich:

| Bedarf | In `RC-01` / WorldForge | Befund |
|---|---|---|
| Straßentyp | `road_class`, `level`, `semantics` | vorhanden |
| Fahrstreifen | `lanes` (Struktur nicht beschrieben) | Feld da, Semantik offen |
| Richtung, Einbahn | Richtung/Kapazitätsannahme der Kanten (`WF-D03`) | teilweise, Einbahn je Spur offen |
| Geschwindigkeit | Klassenprofile mit Geschwindigkeitsregeln (`WF-RG`) als Entwurfswerte | **Entwurfsgeschwindigkeit ist kein Tempolimit**; Limit fehlt |
| Kreuzungen | `Intersection` mit `junction_type`, `turn_constraints` | vorhanden |
| Abbiegebeziehungen auf Spurebene | – | fehlt |
| Schilder, Signale | Asset-Katalog M4, nur Proxy-Meshes | **keine Regelsemantik** |
| Navigationsverknüpfungen | Graph über `connected_edge_ids`; Spline-Samples im Export (`WF-EX`) | Kantenebene vorhanden, Spurebene fehlt |
| Parkflächen | – (M3: Parzellen, Straßenzugänge) | fehlt |
| Tankstellen, Rastplätze, Depots | – | fehlt |
| Brücken, Tunnel | `Crossing`, `spans`, Portale und Clearance (`WF-RG`) | vorhanden |
| Belag | Materialien und Asset-Referenzen | teilweise |
| Steigung, Kurvenradius | `grade_profile`, `curvature_profile` | vorhanden |
| Stabile IDs | deterministische IDs (`WF-C09`) | vorhanden |
| Einheiten | Meter; Achsenkonvention offen (`WF-DC01`) | vorhanden / offen |
| Verkehrsnachfrage | `demand_index` als Profilwert | vorhanden |

Daraus folgt: Die **Kantenebene** (Topologie, Geometrie, Klasse, Querungen) ist abgedeckt oder geplant. Die **Fahrspur- und Regelebene** (Spurverbindungen, Limits, Schilder, Signale, Parkplätze, Dienstpunkte) fehlt und ist Spielinhalt oder ein Wunsch an WorldForge.

---

## 4. Kandidatenfelder

> **💡 VORSCHLAG.** Kandidaten, keine Formate. Jeder Kandidat steht unter der Frage in Abschnitt 5, wer ihn liefert.

| Kandidat | Inhalt | Verweis |
|---|---|---|
| `lane_semantics[]` je Kante | Anzahl, Richtung, Zweck (Fahrstreifen, Bus, Rad, Parken, Standstreifen) | Module 10, 17 |
| `lane_connections[]` je Kreuzung | Spurverbindungen und Konfliktbereiche | Modul 11 |
| `speed_hint` / Regelprofil-Verweis | Entwurfsgeschwindigkeit der Karte, Limit aus Spielprofil | Modul 10, 19 |
| `functional_role` je Kante | Zubringer, Industriezufahrt, Rennstrecken-Zufahrt, Einsatzweg | Module 12, 16 |
| `ParkingArea` | Art, Kapazität, Zufahrtskanten | Modul 12 |
| `ServicePoint` | Tankstelle, Ladepark, Rastplatz mit Anschlusskante | Modul 09 |
| `Depot` | Heimat für Verkehrsrollen, Anschlusskante | Modul 12 |
| `RegionMembership` | Zugehörigkeit zu Region und Bundesstaat (Kennzeichen, Polizeiregeln) | Module 18, 19 |

Feste Anforderung an jeden Kandidaten: stabile ID nach `WF-C09`, Meter als Einheit, Verweis auf `RoadEdge`/`Intersection` statt Kopie von Geometrie.

---

## 5. Wer liefert die Fahrspur-Ebene

> **💡 VORSCHLAG.** Drei Wege, keiner gewählt. Die Entscheidung berührt beide Projekte (`CQ-99`).

| Weg | Inhalt | Vorteil | Nachteil |
|---|---|---|---|
| A: WorldForge liefert | Der Straßengenerator erzeugt auch Spurverbindungen, Parkflächen und Dienstpunkte | eine Wahrheit, konsistent mit der Geometrie | erweitert den Auftrag von M2/M3, bindet Spielregeln an den Generator |
| B: Spiel leitet ab | Das Spiel berechnet Spurebene und Dienstpunkte aus Kanten, Klassen und Kreuzungen | WorldForge bleibt schlank, Regeln liegen beim Spiel | Ableitung kann der Geometrie widersprechen, doppelte Pflege |
| C: Überlagerung | WorldForge liefert die Kantenebene, das Spiel pflegt eine **Überlagerung** (Overlay) von Spielseiten-Daten, die auf stabile IDs verweisen | klare Trennung, kompatibel mit dem Layer-Prinzip von WorldForge (geschützte manuelle Änderungen) | Verweise können beim Neuimport ungültig werden (Abschnitt 6) |
| F: Freitext | Eigene Beschreibung | – | Inhalt hat Vorrang vor A bis C |

Die Module 10 und 11 setzen den Weg C voraus (`IntersectionRules`, Straßenprofil), ohne ihn zu entscheiden.

---

## 6. Stabile IDs, Überlagerung und Neuimport

- WorldForge vergibt deterministische stabile IDs (Namensraum Welt/Feature/Region/Schlüssel) und verspricht, dass der Unreal-Reimport nur eigene Actors aktualisiert und fremde, manuell erstellte nicht löscht (`WF-C09`, `WF-UE`).
- Eine Straßenverlegung invalidiert abhängige Parzellen und Assets (`WF-C09`: ‚Straßenverlegung berührt Haus‘). Dasselbe gilt für die Spielseite: Verweise des Overlays auf eine verlegte oder entfernte Kante werden ungültig.
- Ein Rebuild eines Korridors soll lokale Straßen-, Terrain- und Parzellenbereiche invalidieren, nicht die ganze Welt (`WF-D03`).

> **💡 VORSCHLAG.** Das Spiel führt eine **Netzversion** im Weltzustand (`road_state`, Modul 22) und prüft beim Laden und beim Neuimport, ob alle Verweise des Overlays und des Spielstands (Baustellen, Sperrungen, Unfallstellen, abgestellte Fahrzeuge) auf vorhandene IDs zeigen. Ungültige Verweise werden gemeldet und nach einer festgelegten Regel migriert, nie still entfernt (`CQ-105`).

---

## 7. Validierung auf Spielseite

> **💡 VORSCHLAG.** `RC-01` validiert Graphen für das Netz. Das Spiel braucht zusätzlich Prüfungen aus der **Fahrzeugsicht** (Details und IDs in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md)):

- **Befahrbarkeit:** Jede Kante, die als befahrbar markiert ist, ist für mindestens eine Verkehrsrolle erreichbar und verlassbar (Abbiegebeschränkungen und Einbahnen berücksichtigt).
- **Erreichbarkeit der Dienste:** Parkflächen, Tankstellen, Depots und Werkstätten haben eine erreichbare Anschlusskante.
- **Fahrzeitkalibrierung:** Die Fahrzeiten zwischen Zielen entsprechen der Absicht (die Weltgröße wird an Fahrzeiten mitkalibriert, `W-GE-026`).
- **Fahrzeugfähigkeit:** Steigung, Kurvenradius und Querungen sind mit den Fahrzeugklassen vereinbar, die die Straßenklasse nutzen dürfen (`W-FZ-009`).
- **Überlagerung:** Alle Verweise des Overlays zeigen auf vorhandene Karten-IDs.
- **Regressionsfälle der Karte** (gerade Straßen, Strecken ins Meer, unerschlossene Regionen; `WF-D03`) werden als Navigationsprüfung mitgeführt: Die Verkehrs-KI darf keine Route ins Meer planen.

---

## 8. Platzhalter für die frühen Phasen

`CLAUDE.md` §3 verlangt für frühe Phasen temporäre, klar gekennzeichnete Platzhalterumgebungen und austauschbare Grenzen, und §2 untersagt, Geometrie der Referenzwelt ohne Genehmigung zu übertragen.

> **💡 VORSCHLAG.** Für Testkarten und den Vertical Slice wird ein **kleiner, von Hand erstellter Straßengraph** im Kandidatenformat gepflegt (Kanten, Kreuzungen, Overlay-Daten), gekennzeichnet als `placeholder`. Er kann später durch einen WorldForge-Export mit demselben Schnittstellenzuschnitt ersetzt werden. Daten der Referenzwelt `WORLD_700` (zum Beispiel `roads_major`, `engineering_alignments` in [Welt 24](../World/24-WORLD700-GIS-Referenz.md)) sind **Referenz**, keine Testbasis ohne ausdrückliche Genehmigung (`CQ-104`).

---

## 9. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-WF-001 | WorldForge ist ein eigenständiger Rust-Weltgenerator; implementiert ist M0.1 (synthetisches Terrain); der Unreal-Import ist `UNVERIFIED` | 🧩 ABGELEITET | `WF-RD`; `WF-UE` | WF | Stand 24.10.2026; kann sich ändern |
| VEH-DEC-WF-002 | `RC-01` ist ein M2-Vorvertrag mit `SettlementNode`, `RoadNode`, `RoadEdge`, `Crossing`, `Intersection`, nicht als vollständiger Straßengenerator zu lesen | 🧭 RICHTUNG | `WF-RC01` | WF SN | – |
| VEH-DEC-WF-003 | Straßengraph (Topologie) und metrische 3D-Trasse sind getrennt; Straßen verbinden Orte nach Bedarf; Validatoren und Regressionsfälle sind vorgesehen | 🧭 RICHTUNG | `WF-RG`; `WF-D03` | WF SN | – |
| VEH-DEC-WF-004 | Mesh- und Spline-Export referenziert dieselben RoadEdge-, Crossing-IDs und Höhenkoordinaten | 🧭 RICHTUNG | `WF-RC01`; `WF-EX` | WF UE | Import ungeprüft |
| VEH-DEC-WF-005 | Stabile IDs sind deterministisch abgeleitet; der Reimport verändert nur WorldForge-eigene Actors | 🧭 RICHTUNG | `WF-C09`; `WF-UE` | WF UE PER | – |
| VEH-DEC-WF-006 | WorldForge rechnet in Metern; die Achsenkonvention ist nicht festgelegt | 🧭 RICHTUNG | `WF-DC01` | WF UE | `CQ-101` |
| VEH-DEC-WF-007 | `RC-01` enthält keine Fahrspur-Verbindungen, Geschwindigkeitsbegrenzungen, Schilder und Signale mit Regelsemantik, Parkflächen, Tankstellen, Rastplätze oder Depots | 🧩 ABGELEITET | `WF-RC01`; `WF-RG` | WF SN KR | Bedarf: Abschnitt 3 |
| VEH-DEC-WF-008 | Vorhanden oder geplant sind Straßenklasse, Spuren (Feld), Querungsarten, Abbiegebeschränkungen, Steigungs- und Krümmungsprofil, Owner-Region, Nachfrage-Index | 🧩 ABGELEITET | `WF-RC01`; `WF-D03` | WF SN | – |
| VEH-DEC-WF-009 | Kandidatenfelder für das Spiel: lane_semantics, lane_connections, speed_hint, functional_role, ParkingArea, ServicePoint, Depot, RegionMembership | 💡 VORSCHLAG | Ableitung aus Abschnitt 3 | WF SN | Format offen: `CQ-100` |
| VEH-DEC-WF-010 | Drei Wege für die Fahrspur-Ebene: A WorldForge liefert, B Spiel leitet ab, C Überlagerung mit Verweis auf stabile IDs, F Freitext; keiner gewählt | 💡 VORSCHLAG | – | WF SN | `CQ-99` |
| VEH-DEC-WF-011 | Das Spiel führt eine Netzversion im Weltzustand und prüft beim Laden und Neuimport alle Verweise auf Karten-IDs | 💡 VORSCHLAG | Ableitung aus `WF-C09`, `WF-D03` | PER WF | `CQ-105` |
| VEH-DEC-WF-012 | Das Spiel prüft aus der Fahrzeugsicht: Befahrbarkeit, Erreichbarkeit der Dienste, Fahrzeitkalibrierung, Fahrzeugfähigkeit, Overlay-Verweise und die Regressionsfälle der Karte | 💡 VORSCHLAG | Ableitung aus `WF-RC01`, `WF-D03`, `W-GE-026` | WF TST | Tests: Modul 31 |
| VEH-DEC-WF-013 | Für frühe Phasen wird ein kleiner, von Hand erstellter Platzhalter-Straßengraph im Kandidatenformat gepflegt und später durch einen WorldForge-Export ersetzt | 💡 VORSCHLAG | Ableitung aus `CLAUDE.md` §3 | WF TST | – |
| VEH-DEC-WF-014 | Daten der Referenzwelt WORLD_700 sind Referenz; ihre Geometrie wird ohne Genehmigung nicht übertragen | 📋 BESCHLOSSEN | `CLAUDE.md` §2 Punkt 7; [Welt 24](../World/24-WORLD700-GIS-Referenz.md) | WF | Nutzung als Testbasis: `CQ-104` |
| VEH-DEC-WF-015 | Wer die Fahrspur-Ebene, Parkflächen und Dienstpunkte liefert, ist nicht entschieden | ❓ OFFEN | – | WF | `CQ-99`, `CQ-102` |
| VEH-DEC-WF-016 | Austauschformat, Versionierung und Verhältnis zu Geschwindigkeits- und Regelprofilen sind nicht festgelegt | ❓ OFFEN | – | WF | `CQ-100`, `CQ-103` |

---

## 10. Offene Punkte

| Frage | Verweis |
|---|---|
| Wer liefert Fahrspur-Ebene (A, B, C oder Freitext) | `CQ-99` |
| Austauschformat und Versionierung | `CQ-100` |
| Achsenkonvention und Einheit im Spiel | `CQ-101` |
| Parkflächen, Tankstellen, Depots: Generator oder Spiel | `CQ-102` |
| Geschwindigkeits- und Regelprofile: Karte oder Spiel | `CQ-103` |
| WORLD_700 als Testbasis | `CQ-104` |
| Umgang mit ungültigen Verweisen beim Neuimport | `CQ-105` |
