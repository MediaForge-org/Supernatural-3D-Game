# Known issues and limits — NightBuild 02

## Karte / Navigation
1. **Nichts davon läuft unter Unreal.** Es gibt keine Anbindung an `SpnMapSubsystem` / `SpnMapScreen` / `SpnMapView` / `SpnHudWidget`; kein Zeichencode. (3)/(5)
2. **Keine Höhe im Modell.** `MapPosition` hat kein z. Eine Straße unter einer Brücke und die Brücke selbst sind beim Einrasten nur über „gleiche Distanz → Oberflächenstraße zuerst“ unterscheidbar; ein Spieler direkt unter einer Brücke kann auf die falsche Ebene einrasten. Echte Mehrebenen-Straßen (Parkhäuser, Auffahrtsschleifen) brauchen eine Höhenentscheidung.
3. **Keine Ebenenwechsel im Routing.** Eine Route bleibt auf einer `MapLayer`; Gebäude-Innenräume, Untergrund, Treppen (`SegmentKind::LayerTransition`) werden vom Routing nicht erzeugt.
4. **Connectoren ignorieren Gebäude und Gelände** (nur Wasser blockiert). Ein Zugang kann durch eine Hauswand führen. Gelände/Steigung geht nicht in die Kosten ein. Alle Connector-Grenzen (100 m, 5 m Slack, 10 m Nass-Toleranz) sind unabgestimmte Platzhalter.
5. **Kein Zeit-/Geschwindigkeitsmodell.** Kosten sind Meter (optional Klassenfaktor ≥ 1). Keine ETA, kein Verkehr, keine Fahrzeugtypen, keine Straßenzustände außer Sperrung.
6. **Abbiegeverbote**: nur Von-Segment → Nach-Segment an einem Knoten (kein „Via-Weg“-Verbot über mehrere Segmente, keine zeitabhängigen Verbote). U-Turns nur an Sackgassen.
7. **Skalierung**: A* mit euklidischer Heuristik expandiert auf gleichförmigen Rastern fast alles (0,8 s für 60 km Raster-Diagonale). Für echte Fernrouten über das ganze Hauptspiel ist hierarchisches Routing nötig. Kein Speicherbudget ermittelt. Spatial-Index-Zellgröße ist ein Abstimmungsparameter.
8. **Regionen-Streaming** ist nur als `MergeGeometryData` auf Datenebene vorhanden; es gibt kein Laden/Entladen von Paketen zur Laufzeit und keinen Cache.
9. **Rendering-Daten ohne Beschriftung/Icons**: keine Textplatzierung, keine Fog-of-War-Maske, Entdeckung nur auf Regionsebene.
10. **Keine Persistenz der aktiven Navigation** (nur der Wegpunkt-Marker wird bereits gespeichert).
11. Die Importschnittstelle zu WorldForge (`RC-01`) ist nicht definiert; die Datentypen sind ein Vorschlag.

## Living World
12. Kein globales Event-Journal; ein Ereignis lebt nur in den Erinnerungen der Beteiligten. Welche Historie behalten wird, ist laut Proposal 0010 offen.
13. Dispositionen betrachten nur, was ein NPC dem *Akteur* zuschreibt; es gibt keine Gruppen-/Fraktionsdispositionen, keinen Gerüchte-Austausch zwischen NPCs (`Tell`), keine Persönlichkeitsmerkmale.
14. Alle Zahlen (Kapazitäten, Halbwertszeiten, Effekte, Wichtigkeiten) sind Platzhalter ohne Spieltest.
15. `world.npcs` ist in `CaptureGameSave`/`ParseGameSave` verdrahtet, aber von keinem Unreal-Subsystem benutzt. `slot.meta` enthält keine NPC-Revision; ein geladener Slot kann also nicht auf Konsistenz Fakten ↔ NPC-Gedächtnis geprüft werden (analog zur Karte).

## Movement
16. Die Unreal-seitigen Probleme (Treppen/Türen/Kamera/Engstellen) wurden nicht untersucht, weil sie ohne Engine nicht reproduzierbar sind.

## Testumgebung
17. Der Stub-Check beweist nur, dass die Projektdateien zueinander und zu *meinem* Bild der Engine passen. Die neu ergänzten Stub-Signaturen (`FindFloor`, `StepUp`, …) sind nicht gegen UE 5.8.4 geprüft.
18. Der Benchmark `spn_core_bench_navigation` ist nicht in CTest (Zeitmessung).
