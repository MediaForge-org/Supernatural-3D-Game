# Abhängigkeiten

Stand: 08.11.2026. Pfeil bedeutet „braucht“ (A → B: A braucht B).

## Technische Basis

- `WORLD-MAP-CORE` → `FOUNDATION-COORDINATES` (Kartenkoordinaten brauchen die Einheiten).
- `NAV-ROUTING-CORE` → `WORLD-MAP-CORE` (Routing braucht das Kartenmodell).
- `NAV-DEMO-REGION` → `NAV-ROUTING-CORE`.
- `FOUNDATION-INVENTORY` → `FOUNDATION-SAVE` (Inventar wird gespeichert).
- `PROTOTYPE-PICKUP` → `FOUNDATION-INVENTORY`, `FOUNDATION-SAVE`.
- `PROTOTYPE-DOOR` → `FOUNDATION-WORLDFACTS` (Türzustand ist ein Fakt).
- `PROTOTYPE-TESTMAP` → `PROTOTYPE-MOVEMENT`, `PROTOTYPE-DOOR`, `PROTOTYPE-PICKUP`.
- `SIM-NPC-MEMORY` → `FOUNDATION-WORLDFACTS`, `FOUNDATION-SAVE`.
- `MOD-CORE` → `FOUNDATION-SAVE` (Mod-Zustand pro Spielstand, C-37), `FOUNDATION-UNREAL-CORE`.
- `COOP-HOST-SAVE` → `FOUNDATION-SAVE`, Netzwerkentscheidung (offen).

## Spielinhalt

- `PROTOTYPE-CASE-01` → `PROTOTYPE-PICKUP`, `FOUNDATION-SAVE`, `PROTOTYPE-MOVEMENT` (Abnahme), `P0-ACCEPT-EDITOR`.
- `SLICE-CASE-01` → `PROTOTYPE-CASE-01`, `CASE-EVIDENCE`, `CASE-DIALOG`, `NPC-BASIC`.
- `SIM-NPC-PERSONA` → `CASE-DIALOG`, `SIM-NPC-MEMORY`, `CASE-EVIDENCE`.
- `SIM-NEMESIS` → `CREATURE-BASIC`, `SIM-NPC-PERSONA`.
- `SIM-FACTIONS` → `SIM-TIERED`, `SIM-NPC-PERSONA`.
- `WORLD-REALM-BIBLE-*` → `DEC-WORLD-SIZE` (nur für die räumliche Zuordnung; die Bibel selbst ist unabhängig).
- Weltzustand für Realms und Zeitreise → `FOUNDATION-WORLDFACTS` und `FOUNDATION-SAVE` (zentraler Weltzustand, T-06).

## Entscheidungsabhängigkeiten

- `WORLD-REALM-*` und die endgültige Welt warten auf `DEC-WORLD-SIZE` (N-01) und die räumliche Zuordnung (C-02).
- Untergrund-Inhalte warten auf `DEC-UNDERGROUND-SCOPE` (N-02).
- Adult-Inhalte warten auf die Abgrenzung (K-10).

## Reihenfolge laut Quellen

1. Phase-0-Abnahme durch den Entwickler (`P0-ACCEPT-EDITOR`).
2. `PROTOTYPE-CASE-01` (kleiner technischer Test, genehmigt).
3. `P1-FIRST-PLAYABLE` (etwa 30 Minuten).
4. `P2-VERTICAL-SLICE` (ein Fall).
5. Systeme aus Phase 0 (Dialog, Evidenz, NPC, Gegner, Fahren), Reihenfolge nach Bedarf des Slice.
