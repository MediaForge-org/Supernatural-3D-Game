# Kanon: aktueller gültiger Projektstand

Stand: 08.11.2026. Diese Datei ist kurz gehalten. Die Begründung und die Quellen stehen in `DECISIONS.md` (über die IDs). Bei Widerspruch gilt `DECISIONS.md`, dort steht der Status.

Vorrang: spätere ausdrückliche Entwicklerentscheidung > kanonische Statusdokumente > Vorschläge (siehe `README.md`).

## Verbindlich (bestätigt)

**Produkt und Technik** (`DECISIONS.md` B.1, F.7)
- Unreal Engine (Projekt-Hinweis: 5.8.4) mit C++20, C++-first, eigene Persistenz- und Weltzustandsarchitektur (T-02, C-51).
- Anbieterunabhängiger Mod-Kern, lokale `.spnmod`-Dateien, Standardordner `<GameRoot>/mods/`, Aktivierung pro Profil und Spielstand. Steam Workshop nicht Pflicht (T-04, C-36).
- Koop-Kampagne bis 4 Spieler (Richtung, T-05, C-52). Netzwerk-Autorität offen.
- Zentraler Weltzustand, stabile IDs, segmentierte, absturzsichere, migrierbare Spielstände (T-06; Weltzustandsregeln `W-REQ-M1` bis `W-REQ-M6`, README Abschnitt Weltzustand).
- Tests sind Teil der Architektur (T-07).
- Produktionsstrategie: KI-first, Solo, 8–15 Stunden pro Woche. Langfristige Vision bleibt erhalten, frühe spielbare Ergebnisse haben Vorrang (C-53).

**Welt**
- WORLD_700: geografisch und visuell abgeschlossen, Makrogeografie gesperrt, Ergänzungen nur additiv, kein DLC (W-01, C-01).
- Größere Hauptwelt: keine Endgenerierung vor Anforderungen und räumlicher Zuordnung (W-02, C-02).
- Größe: **offen** (N-01). Historische Zahlen (1.500–1.800 km², 2.000 km², 2.200 km²) sind nicht mehr Richtwert.
- Städte: Vorbild für Charakter und Vielfalt, keine realen Flächen (W-03, C-05).
- Reiche Hölle, Purgatorium (Fegefeuer), Himmel: eigene Open Worlds im Basisspiel, mit Persistenz (W-04, C-06 bis C-11). Größen sind Planungsbereiche.
- Zeitreise: jedes Jahr ab etwa 1600 bis zur Gegenwart voll unterstützt, keine harte Untergrenze, historische Weltzustände aus Schichten rekonstruiert (W-05, C-12 bis C-19).
- **Untergrund (N-02, bestätigt am 08.11.2026):** Die vollständige unterirdische Zivilisation entfällt. Bunker, Höhlen, Geheimlabore und kleinere unterirdische Gebiete bleiben möglich. Die alten Zivilisationskonzepte (Erbauer, 50–100 km², Bevölkerung, unterirdischer Schienenverbund) sind historisch und nicht mehr verbindlich.

**Spielinhalt**
- Open-World-Action-Adventure mit RPG-, Ermittlungs-, Immersive-Sim- und Survival-Horror-Tiefe (S-01).
- Düsterer Mystery-Horror (S-02). Frei erstellbarer Protagonist, feste Hunter-Crew (S-03).
- Spielbeginn ländlich, erste lokale Fälle (S-04). Hauptgeschichte in Akte und Kapitel, Hauptmissionen nie dauerhaft verpassbar (S-05).
- Fälle mit mehreren Lösungswegen, Journal, Detektivmodus ohne Hilfe (S-06). Archiv als Wissensgraph (S-07).
- Monster ohne Respawn-Spawnpunkte, Schwächen nicht nur als Bonusschaden (S-08). Individueller Tod bleibt endgültig (C-21).
- Living World, NPC-Gedächtnis, Nemesis, Fraktionen: Langzeitvision, kein M1-Feature (S-09, Proposal 0010).
- Lebenssimulation und Sandbox (Block 10): Beziehungen, Begleiter, Hund, Heimlichkeit, Spurensuche, Forensik, Verletzungen, Lebensbedürfnisse, Inventar, Smartphone, Kalender und Freizeit, keine Klassensperren (C-23 bis C-35).
- Fahren mit manueller Kupplung und freier Gangwahl als Ziel (S-12).
- Medaillen mit verborgenen Kriterien, Replay ändert den Story-Zustand nicht (S-10).
- Soziale Medien nie Pflicht (S-11). Adult-Bereich als getrenntes optionales Paket (S-14, offen: K-10).

**Verteilung**
- Optionale Pakete für Stimme und Texturen, Standardinstallation nicht aufblähen (T-08, C-39).
- Stimmproduktion mit freiwilligen Referenzaufnahmen und schriftlicher Einwilligung (C-40).

## Vorläufig (Arbeitsstand, keine Festlegung)

- Metropole 20–25 km², größere Städte 8–10 km² (C-04).
- Phasenziele: Phase 1 ca. 1–3 km² und 30–60 Minuten, N-03 setzt den ersten spielbaren Einstieg auf etwa 30 Minuten (V-09, N-03).
- Hauptgeschichte etwa 80–100 Stunden (V-05).
- Weitere Zielwerte in `DECISIONS.md` Abschnitt C.

## Offen

Siehe `OPEN_QUESTIONS.md`. Wichtigste Punkte: Weltgröße (N-01), Realm-Zeitdilatation, Himmel-Erinnerungsdomänen, Todesablauf, Abgrenzung Romantik/Adult (K-10), Mod-Laden nach Fehlern (K-14), Koop-Netzwerk, Namensgebung der Untergrund-Institutionen (eigene IP).

## Verworfen

- Gesamtstadtfläche 570 km² (X-04).
- Frühere Weltgrößen 200–300 km² und 1.500–1.800 km² als Richtwert (X-01, X-02).
- Unterirdische Zivilisation mit mehreren Millionen Bewohnern und 50–100 km² Hauptstadt (N-02, C-42, C-45). Historisch erhalten.
- Steam Workshop als Pflichtanbieter (C-36).
