# NightBuild 03 — Karte & Navigation erstmals in Unreal (M2 Tech-Demo)

ENGINEERING PROPOSAL / Arbeitsstand, **nicht** Master-Canon. Keine Welt-Größe, kein WorldForge-/RC-01-Vertrag festgelegt.

## Master basis
Master consultation protocol (CLAUDE.md §11): **übersprungen** — der Meilenstein verbindet bereits vorhandene Core-Systeme (NightBuild02) mit Unreal ohne neues Gameplay-System, ohne Save-Format-Änderung und ohne Architekturentscheidung. Grundlage: `Docs/NightBuild02/*`, `Docs/Proposals/0012-map-geometry-road-routing-and-npc-memory-block.md`.

## Was gebaut wurde
| Teil | Pfad |
|---|---|
| `USpnMapSubsystem`: `InstallSyntheticRegion`, `NavigationSession`, `BuildMapRenderFrame`, Sperrungen, Reisemodus, `GetGeometryView` | `Unreal/SupernaturalProt/Source/SpnCore/Public|Private/SpnMapSubsystem.*`, `SpnMapInternal.h` |
| Karte zeichnet Straßen/Gebäude/Wasser/Route (Polygonfüllung, Klassen, Brücke/Tunnel, Sperr-Markierung) | `.../Spn/UI/SpnMapView.cpp` |
| Kartenscreen: Taste `X` Straße am Cursor sperren/öffnen, Taste `T` Fahrzeug/zu Fuß, Routen-/Fehlertexte | `.../Spn/UI/SpnMapScreen.*`, `SpnUiTextNightBuild.inc` |
| Minimap mit Straßenlayer + Route (quadratisch), HUD-Beschriftung | `.../Spn/UI/SpnHudWidget.cpp` |
| 3D-Demo-Region aus denselben Core-Daten, Wegpunkt-Beam, Demo-Autopilot, Skript | `.../Spn/SpnNavDemo.*`, `SpnNavDemoScript.cpp` |
| Level `/Game/SpnTest/Lvl_SpnNavDemo` (nur Licht, PlayerStart, 1 Actor) | `Unreal/SupernaturalProt/Tools/create_spn_nav_demo_map.py` |
| Aufnahme (ddagrab → NVENC) | `Tools/windows/record_nav_demo.ps1` |
| Tests | `.../SpnCore/Private/Tests/SpnMapNavigationAutomation.cpp` |
| MSVC-Fix und Preset | `Core/src/GeometryMath.cpp` (`<array>`), `CMakePresets.json` (`msvc-ninja`) |

**Keine zweite Routing-Implementierung:** Routen, Neuplanung, Sperrungen und Render-Daten kommen ausschließlich aus `PlanRoute` / `NavigationSession` / `BuildMapRenderFrame` des Cores. Die Unreal-Seite konvertiert nur Typen.

## Koordinaten-Adapter (Demo, austauschbar)
Es gibt **keine** neue globale Festlegung. Verwendet wird der bereits vorhandene `FSpnWorldFrame` (Standard): `ESpnWorldAxes::EastSouthUp`, Ursprung 0.

| | |
|---|---|
| Einheit | 1 Kartenmeter = 100 cm |
| Karte → Unreal | Karte +X (Ost) = UE +X, Karte +Y (Nord) = UE −Y (UE +Y = Süd), Z = vom Aufrufer (Boden 0) |
| Ursprung | Karten-(0,0) = UE (0,0,0), `OriginMetres` verschiebt bei Bedarf |
| Rotation/Kurs | Kompasskurs (Rad, im Uhrzeigersinn ab Nord) ↔ UE-Yaw: Nord = −90°, Ost = 0° |
| Höhe | Der Core kennt keine Höhen. Brücken liegen flach auf Geländeniveau, der Tunnel ist eine geschlossene Röhre am Boden, Wasser ist ein 3-m-Graben. |
| Bildschirm | y nach unten; Kartenansicht rechnet über `MinimapTransform` (Core) |

Alle Umrechnungen laufen über `MapToUnreal` / `UnrealToMap`; weder Karte noch Demo-Actor haben eine private Achsenkonvention. Test: `SpnCore.Navigation.CoordinateAdapter`, `SpnCore.Navigation.WorldToMapRoundTrip`. Die Frage NorthEastUp vs. EastSouthUp bleibt für die echte Welt offen.

## Bedienung (Level `Lvl_SpnNavDemo`)
Start: `UnrealEditor.exe <uproject> /Game/SpnTest/Lvl_SpnNavDemo -game` (Spielfluss-HUD wird vom Level aktiviert).
`M` Karte; Ziehen = verschieben, Mausrad = Zoom, **Rechtsklick = Wegpunkt**, `Entf` = Navigation abbrechen, `X` Straße sperren/öffnen, `T` Fahrzeug/zu Fuß, `C` zentrieren.
Konsole: `Spn.NavDemo.Run` (Skript), `Spn.NavDemo.Drive <m/s>` (Demo-Autopilot, 0 = aus).

## Bekannte Einschränkungen
Siehe `KNOWN_ISSUES.md`.
