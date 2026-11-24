# Level-Übersicht (Level Registry)

Stand: Night Build 05 (2026-11-02). Alle `SpnTest`-Levels sind **temporäre Platzhalter-/Testlevels**, keine Spielwelt und keine Aussage über Weltgröße, Gelände oder Siedlungen (CLAUDE.md §3). Sie werden von Skripten unter `Unreal/SupernaturalProt/Tools/` erzeugt und dürfen jederzeit neu erzeugt oder ersetzt werden; die `.umap`-Dateien sind generierte Ausgaben dieser Skripte.

| Level (`/Game/SpnTest/…`) | Zweck | Erzeugt von | Seit | Genutzt von |
|---|---|---|---|---|
| `Lvl_SpnMainMenu` | Hintergrund des Hauptmenüs (Nachtszene aus Engine-Primitiven, Menükamera) | `create_spn_main_menu_map.py` | Night Build 01 | Hauptmenü |
| `Lvl_SpnDoorTest` | 40 × 40 m Boden, drei Testtüren, zwei Aufnahme-Objekte, Rampe/Treppe/Mauer | `create_spn_door_test_map.py` | Proposal 0004 / M1 | `SpnGame.TestMap.*`, Tür-/Pickup-Tests |
| `Lvl_SpnMovementTest` | 300 × 300 m Bewegungstestumgebung (Speed-Lane, Hindernisse, Rampen, Treppen, drei Testgebäude) | `create_spn_movement_test_map.py` | M2-A | Bewegungs-QA (V1) |
| `Lvl_SpnMovementTestV2` | V2-Generator-Ausgabe (Instancing, Gebäude) – **Standardkarte der `SpnQA`-Läufe** (`-Map`) | `create_spn_movement_test_map.py --v2` | M2-A3 | `SpnQA.Movement/Jump/Doors/…`, `SpnGame.MapV2.*` |
| `Lvl_SpnMovementTestV2Env` | V2 plus Umgebungsanreicherung | `… --v2 --enrich --instancing` | M2-A3 | Sichtprüfung der Umgebung |
| `Lvl_SpnMovementTestV3Interiors` | V3 mit begehbaren Innenräumen | Generator in diesem Repository nicht eindeutig zugeordnet (nicht nachgeprüft) | M2-A4 / M2B3 | `-Map …V3Interiors` für `SpnQA.Doors`, `BuildingAccess`, `ContinuousTraversal` (laut `Docs/M2B3/INVENTORY_VIEW_MODEL.md`) |
| `Lvl_SpnNavDemo` | Karten-/Navigations-Techdemo (synthetische Region, Straßenrouting) | `create_spn_nav_demo_map.py` | Night Build 03 | Navigations-Demo (Night Build 03) |
| `Lvl_SpnBuildingTest` | begehbares Testgebäude 12 × 9 m: Foyer/Archiv/Arbeitszimmer/Lager, sechs Türen (verriegelt, Schlüssel, aufbrechbar), Lichtschalter, Treppenstufen | `create_spn_building_test_map.py` | Night Build 04 | `SpnQA.HouseJourney` (Suite `Building`) |
| **`Lvl_SpnNpcTest`** | **Night Build 05:** dasselbe Gebäude **ohne Dach** (Übersichtskameras sehen hinein), Lagertür entriegelt und offen, zwei NPCs (Hausmeister im Archiv, Nachbar im Foyer), Navigationsvolumen, zwei Kameras, Spieler startet hinter dem Gebäude | `SPN_VARIANT=npc SPN_FORCE=1 … create_spn_building_test_map.py` | Night Build 05 | `SpnQA.NpcReaction` (Suite `Npc`) |

## Visual Build: `/Game/SpnWorld/Lvl_SpnWorld`

| Level | Zweck | Erzeugt von | Seit | Genutzt von |
|---|---|---|---|---|
| **`/Game/SpnWorld/Lvl_SpnWorld`** | **Platzhalter-Testgebiet** 1,26 × 1,26 km (1,59 km²): Landscape, Ort an einer Landstraße, Wald, Bach, Teich, begehbare Hütte, drei Lichtszenarien. Keine finale Karte, kein Kanonort. | `Tools/spn_world_build.ps1` (`spn_world/build_world.py`) | Visual Build (2026-11-11) | `Tools/spn_visual.ps1` (Kamerafahrten, Performance), siehe `Visual-Build-Tools.md` |

Die `SpnTest`-Levels oben bleiben unverändert: Ihre Koordinaten tragen die `SpnQA`-Tests. `/Game/SpnWorld/_Scratch` ist ein leeres Hilfslevel des Generators.

Weitere Levels im Projekt, **nicht** von diesem Projekt erzeugt: `ThirdPerson/Lvl_ThirdPerson` (Projektstartkarte der Unreal-Vorlage), `Variant_Combat`, `Variant_Platforming`, `Variant_SideScrolling` (Vorlagen-Varianten, ungenutzt), `Characters/ProtagonistV1/Comparison/LV_ProtagonistV1_Compare` und `_Scratch/LV_Probe` (Protagonisten-Prototyp).

## Night Build 05: Levelwahl und Begründung

Es wurde **ein neues Level** (`Lvl_SpnNpcTest`) aus dem **bestehenden Generator** des Gebäudetests abgeleitet (Umgebungsvariable `SPN_VARIANT=npc`), statt ein zweites Gebäude zu erfinden oder `Lvl_SpnBuildingTest` zu verändern:

* `Lvl_SpnBuildingTest` bleibt unverändert die Referenz für die Türtests von Night Build 04 (`SpnQA.HouseJourney`, `SpnGame.NightBuild04.*`).
* Die NPC-Variante braucht ein offenes Dach (nachvollziehbare Kameraperspektiven für die Aufnahme), einen für NPCs begehbaren Weg (entriegelte, offene Lagertür) und ein Navigationsvolumen. Das sind Eigenschaften des Szenarios, nicht des Gebäudes.
* Das Level enthält keine Weltgeometrie, keine Karte und keine WorldForge-Anbindung.

Neu erzeugen (Windows, aus dem Repository-Stamm):

```
$env:SPN_VARIANT='npc'; $env:SPN_FORCE='1'
UnrealEditor-Cmd.exe Unreal/SupernaturalProt/SupernaturalProt.uproject -run=pythonscript -script="<abs>/Unreal/SupernaturalProt/Tools/create_spn_building_test_map.py" -unattended -nullrhi
```

Navigation: Das Level liefert **kein gebackenes** NavMesh. `Config/DefaultEngine.ini` (`[/Script/NavigationSystem.RecastNavMesh] RuntimeGeneration=Dynamic`) lässt es beim Levelstart erzeugen; es folgt auch den beweglichen Türen. Die QA wartet nach dem Laden 2,5 s. Fehlt das Mesh, steuern NPCs geradlinig zum Ziel (`ASpnNpc::IsUsingFallbackSteering`).
