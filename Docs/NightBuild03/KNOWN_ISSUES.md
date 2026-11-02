# Known issues and limits — NightBuild 03

1. **Platzhalter-/Testdaten.** Die Region ist `synthetic:` (kein WorldForge, keine Geografie, keine Weltgröße). Alle Zahlen (Breiten, Höhen, Geschwindigkeiten) sind unabgestimmte Demo-Werte.
2. **Keine Höhen im Core.** Brücke = flaches Deck auf Bodenhöhe mit Geländer und Pfeilern über dem Wassergraben; Tunnel = geschlossene Röhre am Boden; kein Rampen-/Mehrebenenmodell. Höhenmodell bleibt Entwicklerentscheidung (NightBuild02 KNOWN_ISSUES 2/3).
3. **Kein Fahrzeug.** Der „Demo-Autopilot“ setzt den Pawn entlang der Core-Route (Teleport pro Frame, Kurvenbremsung); das Auto sind Platzhalterboxen. Keine Fahrphysik, kein Lenken durch den Spieler. Der Spieler kann zu Fuß gehen (`Spn.NavDemo.Drive 0`), die Laufgeschwindigkeit ist die des vorhandenen Characters.
4. **3D-Gebäude/Flächen sind Bounding-Boxen** der Core-Grundrisse (korrekt für die achsparallelen Testdaten; echte Polygone würden approximiert).
5. **Karte:** Beschriftung, Straßennamen, Einbahn-Pfeile, Fog-of-War-Maske fehlen. Minimap wird bei Straßendaten quadratisch statt rund (der Core klippt nur Rechteck-/Kreis-Viewport; Slate hat keinen Kreis-Clip).
6. **Aktive Navigation wird nicht gespeichert** (nur der Wegpunkt-Marker). Nach dem Laden plant der nächste `UpdateNavigation` die Route neu.
7. **Neuplanzeit** nutzt Echtzeit (`FPlatformTime`) + `AdvanceNavigationClock` (Tests/Skript); pausiert die Karte das Spiel, läuft die Abkühlzeit weiter.
8. **Minimap-Zoom** der Straßenansicht ist ein Demo-Wert (0,8 m/px bei Zoom 1).
9. **Kamera im Film:** Die Verfolgerkamera des Characters ist nicht für ein Auto abgestimmt (Auto wirkt nah). Ein Spring-Arm-Override ist eingebaut, im Film sichtbar nur eingeschränkt wirksam.
10. **`SpnQA.*`** (visuelle/interaktive Läufe über `Tools/run_gameplay_qa.ps1`) wurden nicht ausgeführt.
11. Vorbestehend: Warnung `EKeys::Virtual_Accept` (deprecated) in `SpnInventoryScreen.cpp`; 17 × „Condition failed“-Log beim Editorstart vor den Tests (auch vor NightBuild03 vorhanden).
