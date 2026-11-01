# Nächste kleinste sinnvolle Schritte — NightBuild 02

Reihenfolge ist ein Vorschlag; Entscheidungen bleiben beim Entwickler.

1. **Review und Commit** der Änderungen (Vorschlag unten im Abschlussbericht). Danach: MSVC-Build der Core-Tests (WINDOWS_VALIDATION §1).
2. **Entscheidung Höhenmodell** (Brücken über Straßen, Parkhäuser): z in `MapPosition`/Segmenten oder „Ebenen-ID pro Segment“. Das beeinflusst Snapping (KNOWN_ISSUES 2).
3. **Entscheidung WorldForge-Importformat** (`RC-01`) → ein Importer-Testfall, der echte (kleine) Exportdaten in `MapGeometryData` überführt; `MergeGeometryData` für Regionen.
4. **Unreal-Anbindung der Navigation** (kleinster Schnitt): ein Wegpunkt per Karte setzen, `NavigationSession` in `USpnMapSubsystem`, Route als Linienzug in `SpnMapView`. Die synthetische Region als Demo-Level (Daten aus dem Core).
5. **Hierarchisches Routing** (z. B. Contraction Hierarchies oder Region-Überlagerungsgraph), sobald echte Netzgrößen vorliegen; vorher Speicherbudget pro Segment messen.
6. **Connector-Regeln** mit echten Daten abstimmen (Gebäudewände, Zäune, Gelände); Distanzgrenzen aus Spieltest.
7. **NPC-Gedächtnis → erste Spielbarkeit (Phase L2):** ein Zeuge im Vertical Slice, der sich erinnert; dazu `SpnSaveSubsystem` ↔ `world.npcs` und eine Entscheidung zur Journal-/Kompaktierungsfrage (Proposal 0010 §6).
8. **Movement:** die offenen Editor-Probleme von NightBuild 01 mit dem Diagnose-Trace (`spn.MoveTrace`) auf Windows reproduzieren; erst dann Änderungen.
9. **Kartenrendering:** Beschriftung, Icons, Fog-of-War-Maske.
