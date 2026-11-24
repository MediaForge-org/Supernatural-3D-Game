# Visual Build: Werkzeuge für die Platzhalter-Welt

Technische Notiz (kein Kanon, keine Planungsaussage). Die Welt `/Game/SpnWorld/Lvl_SpnWorld` ist ein **Platzhalter-Testgebiet** für Aussehen,
Licht, Begehbarkeit und Performance. Größe, Lage, Straßen und Gebäude sind frei gewählte Testdaten (`spn_world/layout.py`) und legen nichts für
die spätere Spielwelt fest (CLAUDE.md §3).

## Ablauf

```
cmake -S Tools/SpnWorldTool -B build/worldtool
cmake --build build/worldtool --config Release          # Texturen + Gelände, eigenständiges C++20
.\Tools\spn_world_build.ps1 [-Phase B..H] [-Textures]   # erzeugt die Map (Unreal, braucht GPU)
python Unreal/SupernaturalProt/Tools/spn_world/make_cine.py   # Kamerafahrten aus dem Layout (Engine-Python genügt)
.\Tools\spn_visual.ps1 -Spec <cine/*.json> -Map /Game/SpnWorld/Lvl_SpnWorld -Out <mp4> | -Mode stills | -Mode perf
```

`.uasset`/`.umap` sind nicht versioniert (`.gitignore`); die Skripte sind die Quelle, die Map ist ihr Ergebnis. Die `.uproject` bleibt
unverändert: Die Editor-Skripte laufen mit `-EnablePlugins=GeometryScripting,ProceduralVegetationEditor`.

| Teil | Pfad | Aufgabe |
|---|---|---|
| `spn_worldtool textures` | `Tools/SpnWorldTool/textures.cpp` | 29 kachelbare PBR-Sätze (Farbe, Normal, AO/Rauheit/Metall + Höhe), rein prozedural |
| `spn_worldtool terrain` | `Tools/SpnWorldTool/terrain.cpp` | Höhenfeld, an Straßen/Grundstücke/Bach/Teich angepasst; fünf Bodenschichten; Wald- und Straßenmasken |
| Layout | `spn_world/layout.py` | Straßen (Kontrollpunkte), Grundstücke relativ zur Straße, Bach, Teich |
| Netze | `spn_world/meshkit.py`, `archkit.py` | Netzaufbau in reinem Python: Wände mit Öffnungen, Fenster, Dächer, Treppen |
| Generatoren | `spn_world/roads.py`, `buildings.py`, `nature.py`, `props.py`, `materials.py` | Straßen, Gebäude, Vegetation, Details, Materialien |
| Hauptskript | `spn_world/build_world.py` | baut die Map, Phasen kumulativ: B Gelände, C Straßen, D Gebäude, E Materialien, F Vegetation, G Licht, H Details |
| C++ (Spiel) | `Source/SupernaturalProt/Spn/World/` | `USpnCineSubsystem` (Kamerafahrten), `ASpnWorldLighting` (Lichtszenarien), `USpnWorldBuildLibrary` (Landscape-Import, nur Editor) |

## Lichtszenarien

`ASpnWorldLighting` im Level hält benannte Szenarien (`day`, `overcast`, `spn_overcast`, `night`): Sonne, Mond, Himmelslicht, Nebel, Wolken,
Belichtung, Farbkorrektur, Lampen (Aktoren mit Tag `SpnLamp`), Bodennebel und die Materialwerte `Wetness`, `LampGlow`, `WindowGlow`
(`MPC_SpnWorld`). Konsole: `Spn.Lighting day|overcast|spn_overcast|night|next`. Kein Wetter- oder Tageszeitsystem, nur vier reproduzierbare
Zustände.

`spn_overcast`: stark bewölkter, feuchter Spätnachmittag (Bildsprache einer nordamerikanischen Mysteryserie der frühen 2000er): kein direktes
Sonnenlicht (3 800 lx, Quellwinkel 40°), kühles Himmelslicht, Sättigung 0,62, Kontrast 1,12, Weißabgleich 7 600 K, dichter Höhennebel und
Bodennebel. Tageslicht, keine Nacht.

Bodennebel: Local-Fog-Volumes mit Tag `SpnMist` (von `woodland.py` in Senken des Waldabschnitts gesetzt). Jedes trägt seine Grunddichte als
zweiten Tag `Mist=<Extinktion pro Meter>`; das Szenario multipliziert sie mit `MistDensity` (0 = ausgeblendet; `day` 0, `overcast` 0,35,
`spn_overcast` 1,0, `night` 0,5). Die Extinktion gilt **pro Meter**: Werte um 0,01–0,02 sind dezent, 0,3 ergibt eine undurchsichtige Wand.

Innenräume: Ein begrenztes Post-Process-Volume mit Tag `SpnPostInterior` (Priorität 1, Überblendung 80 cm) bekommt je Szenario einen eigenen
Belichtungsbereich (`InteriorExposureMinEV`/`MaxEV`); alles andere bleibt beim globalen Volume. So bleibt die Untergrenze außen unverändert
und der Raum wird trotzdem hell genug belichtet. `buildings.py` setzt das Volume in jeden Raum mit `lot.interior["room"]`.

## Gameplay-Test in der Welt (`SpnWorldQA.Integration`)

`Tools\run_gameplay_qa.ps1 -Suite World` spielt die erzeugte Welt mit echten Eingaben (Spawn, Gehen/Sprinten auf Straße, Waldweg und Hang,
Kollision mit Baum, Fels und Hüttenwand, Hüttentür, Innenraum und seine Belichtung, vier Lichtszenarien, Kamera-/Fuß-Clipping) und nimmt
das Spielfenster als Video auf. Die Prüfpunkte schreibt der Weltgenerator bei jedem Aufbau nach `Saved/SpnWorldGen/qa_points.json`
(`Tools/spn_world/qa_points.py`); fehlt ein Punkt, meldet der Test den Teil als NOT_TESTED. Code:
`Source/SupernaturalProt/Spn/Tests/SpnQaSpnWorldAutomation.cpp`.

Messreihen mit einer geänderten Einstellung bei sonst identischen Fahrten erzeugt `Tools/spn_world/cine_variants.py` (z. B.
`screen_percentage` → `cine/sp_perf.json`, `cine/sp_video.json`; `quality_modes` → `cine/quality_perf.json`).

## Gameplay-Strecke (Phase 1, 2026-11-13)

`Tools/spn_world/gameplay.py` (Phase H, nach dem Wald) setzt die Spielstrecke, NPCs, Navigationsvolumen und die Weltgrenze; `buildings.py`
gibt den Häusern aus `gameplay.HOUSES` Tür und Innenraum; `qa_points.py` schreibt die Streckenpunkte für `SpnWorldQA.Route`. Spielen:
`Tools\play_spnworld.ps1`. Details, Tests, Messwerte und offene Punkte: `Docs/Development/SpnWorld-Gameplay-Phase1.md`.

## Renderauflösung (TSR) – vorläufiger Standard 90 % (Entwicklerentscheidung 2026-11-13)

| | Standard (PROVISIONAL) | Qualitätsmodus |
|---|---|---|
| `r.ScreenPercentage` | 90 | 100 |
| Umschalten | `Spn.Quality standard` | `Spn.Quality quality` (jederzeit, auch `high`/`100`) |
| Kantenglättung | TSR (`r.AntiAliasingMethod 4`) | TSR |

- Gesetzt von `ASpnWorldLighting` (`DefaultScreenPercentage` 90, `QualityScreenPercentage` 100; der Generator schreibt beide explizit in
  `build_world.py`) bei BeginPlay, zurückgesetzt auf den vorherigen Wert bei EndPlay. Der Actor existiert nur in `Lvl_SpnWorld` (geprüft: 1 von
  17 `.umap`), die QA-Testkarten sind also unberührt. `DefaultScreenPercentage = 0` = Pegel fasst die Einstellung nicht an.
- Priorität `SetByCode`: über Skalierbarkeit und Spieleinstellungen, **unter** der Konsole. Ein von Hand eingegebenes `r.ScreenPercentage`
  (oder eine Messspezifikation mit diesem Befehl) gewinnt und bleibt bis zum Prozessende. Deshalb setzen die Standard-Kamerafahrten von
  `make_cine.py` kein `r.ScreenPercentage` mehr; nur die Messmatrizen (`forest_profile`, `sp_perf`, `sp_video`) setzen es ausdrücklich.
- 85 % ist **nicht** Standard (Entscheidung).
- **Einstellungsmenü (seit 2026-11-13):** Grafik → „Welt-Renderqualität“: Standard (90 %) / Qualität (100 %), Links/Rechts wie jede
  Einstellungszeile. Gespeichert als `WorldQualityMode` in `[SpnPlayer]` der GameUserSettings.ini (`FSpnPlayerSettings::bWorldQualityMode`),
  sofort wirksam: `ASpnWorldLighting` liest die Wahl bei BeginPlay und folgt Änderungen live. In Ebenen ohne den Actor zeigt die Zeile
  „Wirkt in der offenen Welt (SpnWorld)“ und ändert dort nichts. `Spn.Quality` bleibt eine reine Sitzungs-Umschaltung zum Entwickeln
  (wird nicht gespeichert; eine andere Menüänderung setzt sie nicht zurück). Tests: `SpnGame.World.QualityModeFollowsTheGraphicsSetting`
  (headless) und `graphics_menu_world_quality` in `SpnWorldQA.Integration` (echtes Menü, Pfeiltaste rechts → 100 %, erneut → 90 %,
  danach ursprüngliche Wahl wiederhergestellt).
- Tests: `SpnGame.World.ScreenPercentageOnlyWhileTheWorldLevelRuns` (headless: 90 bei BeginPlay, 100/90 per Umschalter, Rückgabe des
  vorherigen Werts bei EndPlay auch aus dem Qualitätsmodus, 0 = unberührt) und der Abschnitt „render resolution (TSR)“ in
  `SpnWorldQA.Integration` (90 → 100 → 90 in der laufenden Welt, TSR aktiv).

Unterschied (gemessen, 2560×1440, RTX 3090, `cine/quality_perf.json`, Umschaltung nur über `Spn.Quality`, 2026-11-13; fps Mittel / 1 %-Tief
= 1000 / p99):

| Einstellung | forest_track | cabin_pond | creek |
|---|---|---|---|
| day, Standard 90 % | 53.6 / 47.6 | 62.3 / 55.2 | 56.6 / 50.5 |
| day, Qualität 100 % | 46.5 / 42.2 | 55.5 / 49.0 | 50.2 / 44.0 |
| spn_overcast, Standard 90 % | 50.6 / 45.4 | 58.2 / 50.9 | 55.0 / 48.4 |
| spn_overcast, Qualität 100 % | 43.9 / 40.4 | 50.5 / 43.8 | 47.7 / 43.0 |

Wiederholung des Standards am Ende jedes Blocks: day 53.6 / 62.0 / 56.4 fps, spn_overcast 50.9 / 58.3 / 55.0 fps (Drift ≤ 0.5 fps).
Die 60 fps werden im Wald auch mit 90 % nicht erreicht (nur cabin_pond/day). Optisch: Vergleichsvideo
`Artifacts/VisualProgress/14_TSR_ScreenPercentage_Compare.mp4` (100/90/85 nebeneinander, aus der Nachtarbeit).

## Kamerafahrten (`-SpnCine=<datei.json>`)

```json
{ "warmup_frames": 240, "cut_warmup_frames": 60, "console": ["..."],
  "shots": [ { "name": "main_street", "scenario": "day", "duration": 12, "fov": 62,
               "keys": [ { "p": [x, y, z], "l": [x, y, z] } ],          // Kamerapunkte und Blickziele (Catmull-Rom, gleichmäßige Geschwindigkeit)
               "walk": [[x, y, z]], "interact_at": [3.0], "stills": [0, 0.5, 1] } ] }   // oder: Spielfigur läuft Wegpunkte ab (echte Spielkamera)
```

Läufer und Türen: `"open_doors": true` lässt die Spielfigur eine geschlossene Tür (`ASpnTestDoor`) selbst öffnen, sobald sie ihr Interaktionsziel
ist und höchstens `door_distance` cm (Vorgabe 170) entfernt liegt; danach bleibt sie `door_pause` s (Vorgabe 0,9) stehen, bis das Blatt
aufgeschwungen ist. Das ist ortsabhängig und deshalb robuster als `interact_at` (Zeitpunkte in Sekunden): Ein zu früher Zeitpunkt liegt außerhalb
der Reichweite von 250 cm und bewirkt nichts. Beide Wege schreiben Ziel und Ergebnis ins Log (`SpnCine: walker used door ...`).

Je Einstellung optional `"console": ["..."]`: Konsolenbefehle beim Schnitt, z. B. für Messreihen (`cine/forest_profile.json`).

`out_dir`, `mode`, `fps` setzt `spn_visual.ps1`. Modi: `video` (fester Zeitschritt, ein PNG je Bild, danach H.264, höchstens 480 MB, vollständige
Dekodierprüfung), `stills`, `perf` (Echtzeit, `perf.json` mit Bildzeiten, Thread-Zeiten, Draw Calls je Einstellung).

Bekannt: Bis zum nächsten Neubau der Spiel-Binärdateien verschmiert das Bild unmittelbar vor einem Kamerasprung (Korrektur `EPhase::Hold`
liegt im Quelltext). `spn_visual.ps1` gleicht das aus (Standbilder doppelt angefordert, gültig sind die geraden Indizes; im Video wird das
letzte Bild jeder Einstellung durch das vorletzte ersetzt).

## Herkunft der Inhalte

- Texturen der Oberflächen und Graskarten: von `spn_worldtool` berechnet, keine Fremdquellen.
- Bäume, Strauch, Bodenpflanze samt Rinden- und Laubtexturen: Beispielinhalte des Engine-Plugins *Procedural Vegetation Editor* (Epic Games),
  beim Bau nach `/Game/SpnWorld/Vegetation` kopiert bzw. in statische Nanite-Netze umgewandelt. Nutzung in Unreal-Projekten nach Engine-Lizenz;
  **nicht** außerhalb davon weitergebbar (für das Open-Source-Ziel relevant, CLAUDE.md §9).
- Makro-Variation, Wasser-Normalmap, Wolkenmaterial: Engine-Inhalte.
- Referenz-Waldabschnitt (Waldweg, Hütte, Teich) und die Waldboden-Schichten der ganzen Welt: Quixel Megascans / Megaplants von Fab, vom
  Entwickler importiert, nicht im Repository. Liste, Pfade, Lizenzhinweise und Rückfallverhalten: `Docs/Development/Fab-Asset-Sources.md`.
  Bäume aus Megaplants sind Skinned Meshes mit Nanite-Assembly; sie stehen in `ASpnSkinnedGroup` (instanzierte Skinned-Mesh-Komponente) und
  brauchen `r.Nanite.Foliage=True` (experimentell).
