# M2A3: Visuelle Prüfliste der angereicherten Testkarte (Windows, Unreal-Editor)

Gegenstand: `/Game/SpnTest/Lvl_SpnMovementTestV2Env`, erzeugt mit `create_spn_movement_test_map.py --v2 --enrich --instancing --force`. Es ist ein
**Platzhalter-Testlevel**, keine Spielwelt und keine Aussage über die geplante Karte. Alles unten ist **nicht** geprüft: auf Fedora gibt es nur Geometrie-
und Datenvalidierung (`test_spn_env_enrichment.py`). Ergebnis jeder Zeile notieren: OK / Abweichung (Screenshot) / nicht prüfbar.

## Vor dem Start
- [ ] Generator-Log: `python Tools\spn_log_tools.py generator <log>` Exit 0; erwartet 1800 Primitive (1102 V2 + 698), 15 Türen (10 V2 + 5 Tore), instanziert ca. 578 Actors.
- [ ] Map-Check im Editor: 0 Fehler. `Lvl_SpnMovementTest.umap` (V1) und `Lvl_SpnMovementTestV2.umap` unverändert (Hash-Vergleich des Skripts).
- [ ] Materialien/Instancing: keine Standardmaterial-Warnung (`missing usage flag InstancedStaticMeshes` darf nicht erscheinen).

## Sichtprüfung (Luftbild, dann zu Fuß)
- [ ] Straßen: Mittelmarkierungen (181 Striche) nur auf freien Strecken, nicht in Kreuzungen, nicht auf Kurve/Wendehämmern, kein Flackern (Z-Fighting) zur Fahrbahn.
- [ ] Gehwege: Mülleimer (56), Bänke (15), Schilder (50) stehen auf dem Gehweg, nicht auf der Fahrbahn und nicht in Eingängen; Restbreite fühlt sich frei an (>= 100 cm), keine Kamera-Blockade.
- [ ] Briefkästen (8) neben den Hauszugängen, nicht im Weg; Einfahrten (4) als flache Platten neben dem Zugang, keine Stufenkante > 25 cm.
- [ ] Parkplätze: 4 Platzhalter-Autos in den Buchten (Körper + Kabine), plausibel ausgerichtet, nicht über Buchtenlinien.
- [ ] Vegetation: 31 Bodenflecken mit Baum-/Buschgruppen (64 Bäume), Größenvariation sichtbar, keine Bäume in Straßen/Wegen/Eingängen/Teststationen.
- [ ] Zäune/Gehöfte (5): Zaun geschlossen, Lücke nur am Tor (130 cm), Tor (Platzhalter-Tür, 200 cm hoch) schwenkt nach außen, Gerät (Pfosten mit Kopf) steht daneben.
- [ ] Gesamteindruck: Dichte/Leere, Lichter (teilweise überbelichtet?), Auto-Exposure; Anmerkungen als Liste (Gestaltung ist offen, nichts davon ist beschlossen).

## Spielerische Prüfung
- [ ] Alle Routen aus `D.ROUTES` begehbar (Spawn -> Häuser, Hütte, Teststationen), keine neue Sackgasse. Teststationen v1 (Rampen, Treppen, Hindernisse) unverändert und erreichbar.
- [ ] Jedes Tor: verschlossen (E -> "Door (locked)"), Gerät benutzen (E am Pfosten), Tor entsperrt (Prompt "Open door"), öffnen, durchgehen (Kapsel passt, Spiel >= 13 cm), schließen.
- [ ] Hinweis: Schalter/Taster/Hebel haben **keinen** Unreal-Actor. Der Pfosten ist nur Geometrie; Benutzen geht erst nach dem Unreal-Adapter (offen im Handoff).
- [ ] Performance: FPS/Draw Calls gegen die V2-Zahlen des Integrationsberichts (114-125 FPS, 120-230 Draw Calls); Actor-Zahl notieren.
- [ ] Kamera: Zäune, Bänke und Bäume am Gehweg erzeugen kein Pumpen.
