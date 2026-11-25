# Vorschlag: Phasen- und Arbeitspaket-System

Status: **Vorschlag, nicht angewendet.** Entscheidung durch den Entwickler steht aus (siehe `OPEN_QUESTIONS.md`, Punkt 17).

Stand der Bestandsaufnahme: 08.11.2026. Quellen sind die README-Roadmap (Abschnitt 27), `CLAUDE.md` §4, `Docs/Proposals/`, `Docs/Decisions/`, die Ordner `Docs/M2A*`, `Docs/M2B*`, `Docs/NightBuild01`–`04`.

## 1. Was es heute schon gibt

| Schema | Beispiele | Bedeutung | Problem |
|---|---|---|---|
| Phasen | Phase 0 (mit Schritten 0.1 bis 0.5), Phase 1 bis 3, 4+ | Entwicklungsstufen gemäß CLAUDE.md §4 und README §27.1 | „Phase 0.3“ wird für zwei Dinge verwendet (README §27.2 vermerkt das selbst). |
| Meilensteine | M1 (erste sichtbare Testkarte, Proposal 0003/0008, Technical Acceptance PASS am 23.10.2026); M2-A (Bewegung, Proposals 0011 und `Docs/M2A*`); M2-B (Kernsysteme, `Docs/M2B1`–`M2B3`) | Arbeitspakete mit Ergebnissen | Die Schreibweise schwankt (M2A, M2-A, M2A4.1). |
| Sprints | NightBuild01 bis NightBuild04 | Autonome Arbeitsläufe, kein Thema | Keine Aussage über das Ziel. |
| Proposals | 0003 bis 0012 | Entwurfsdokumente pro Thema | Nummern sind historisch gewachsen. |
| Entscheidungen | 0001 bis 0006 | Technische Entscheidungen | Nicht mit Spielentscheidungen zu verwechseln. |
| Anforderungs-IDs | `W-GE-`, `W-UG-`, `CK-01`, `M1` bis `M6` (Weltzustand) | Fachliche Register in `Docs/Game/` | **Kollision:** `M1` bis `M6` bedeuten in der README (Abschnitt Weltzustand, Kanon-Tier 3) etwas anderes als der Meilenstein M1. |

Fazit: Die bestehenden IDs sind nicht falsch, aber sie vermischen Phase, Thema und Sprint. Eine neue Schicht muss sie übernehmen, nicht ersetzen.

## 2. Drei Ebenen, klar getrennt

1. **Phase** (zeitliche Reihenfolge): `P0-CORE`, `P1-VSLICE`, `P2-SLICES`, `P3-EXPANSION`, `P4-LONGTERM`. Entspricht README §27.1. Eine Phase hat keine Akzeptanzkriterien, sie ist ein Ziel.
2. **Thema / Domäne** (wofür): kurze Großbuchstaben-Kürzel ohne Nummern, z. B. `FOUNDATION`, `PROTOTYPE`, `MOVEMENT`, `WORLD`, `NAV`, `SIM`, `CASE`, `COMBAT`, `CREATURE`, `VEHICLE`, `UI`, `COOP`, `MOD`, `PLATFORM`, `DATA`, `QA`.
3. **Arbeitspaket** (was genau): `<THEMA>-<SLUG>`, z. B. `FOUNDATION-SAVE`, `PROTOTYPE-MOVEMENT`, `SIM-NPC-MEMORY`, `COOP-HOST-SAVE`. Der Slug beschreibt den Inhalt, nicht eine Nummer. Er ändert sich nie. Wird ein Paket verworfen, bleibt seine ID mit Status „verworfen“ erhalten.

Ein Paket trägt zusätzlich Felder `phase` und `milestone_alias` (für Altbezeichnungen wie M2-A). Die Phase ist eine Eigenschaft, kein Teil der ID. So bleibt die ID stabil, auch wenn ein Paket später in eine andere Phase rutscht.

## 3. Bestehende Bezeichnungen behalten

Alte Namen bleiben als **Alias** erhalten (Tabelle in `TASKS.md`, Spalte `milestone_alias`). Vorschlag für die Zuordnung:

| Alt | Vorgeschlagene ID | Phase |
|---|---|---|
| M1 (Testkarte, Proposal 0003/0008) | `PROTOTYPE-TESTMAP` | P0 (abgeschlossen) |
| M2-A (Bewegung, Proposal 0011) | `PROTOTYPE-MOVEMENT` | P0 (implementiert, nicht im Editor abgenommen) |
| M2-B1 (Karte und Minimap-Daten) | `WORLD-MAP-CORE` | P0 |
| M2-B2 (Koordinaten, Persistenz) | `FOUNDATION-COORDINATES` | P0 |
| M2-B3 (Inventar, Speichern, Weltzustand) | `FOUNDATION-SAVE`, `FOUNDATION-INVENTORY` | P0 |
| Phase 0.3 (WorldFacts, Tür) | `FOUNDATION-WORLDFACTS`, `PROTOTYPE-DOOR` | P0 |
| Phase 0.5 (Pickups) | `PROTOTYPE-PICKUP` | P0 |
| NightBuild01 bis 04 | keine eigene ID; die Pakete darin erhalten die passenden Domänen-IDs | – |
| Living World, Proposal 0010 (L1 bis L5) | `SIM-NPC-MEMORY` (L1), `SIM-NPC-PERSONA` (L2), `SIM-NEMESIS` (L3), `SIM-TIERED` (L4), `SIM-FACTIONS` (L5) | P2 bis P4 |

Die M1-bis-M6-Weltzustandsregeln der README bekommen keine Arbeitspaket-ID. Sie sind Anforderungen und heißen weiter `W-REQ-M1` bis `W-REQ-M6`, damit keine Kollision mehr entsteht. (Umbenennung nur in `Docs/Planning/`, Quellen bleiben unverändert.)

## 4. Der erste kleine spielbare Prototyp

Vorhanden: Testkarte mit Türen, Pickups, Speichern und Laden, Bewegung mit Rampe, Treppe und Wand (M1 abgenommen; Editor-Spieltest durch den Entwickler steht offen).

Vorschlag für den nächsten Prototyp, ein einziges Paket: **`PROTOTYPE-CASE-01`**: „Ein Raum, ein Zeuge, ein Beweis, ein Abschluss“.
- Abhängig von: `PROTOTYPE-PICKUP` (vorhanden), `FOUNDATION-SAVE` (vorhanden), `PROTOTYPE-MOVEMENT` (vorhanden).
- Neu: ein Zeuge mit zwei Gesprächszeilen (Platzhalter-Text), ein Beweisgegenstand, ein Journal-Eintrag, eine Abschlussbedingung.
- Ausgeschlossen: echtes Dialogsystem, Forensik, Monster, Fahren, Story-Inhalte. Alles als Platzhalter markiert.
- Akzeptanz: Ein Durchgang vom Start bis zum Abschluss im Editor, Speichern und Laden zwischen den Schritten, Core-Tests grün, Unreal-Automationstest grün, manuelle Abnahme durch den Entwickler.

Begründung: Es ist der kleinste Schritt, der Ermittlung, Gesprächsstruktur und Weltzustand gleichzeitig prüft (Phase 0 laut README §27.1 nennt genau diese Systeme). Dafür muss kein neues Gameplay-Thema eingeführt werden.

## 5. Was ich nicht getan habe

- Keine bestehende ID umbenannt oder umgedeutet.
- Keine Arbeitspakete angelegt.
- Keine Spielentscheidung getroffen.
- Die Reihenfolge der Phase-0-Systeme (README §27.3: „Reihenfolge nicht festgelegt“) bleibt offen.

## 6. Entscheidungen, die ich brauche

1. Zustimmung zum Dreiebenen-System (Phase, Thema, Paket) mit stabilen Slugs.
2. Zustimmung zur Alias-Tabelle in Abschnitt 3, oder Korrekturen.
3. Zustimmung zum Vorschlag `PROTOTYPE-CASE-01` als nächster Prototyp.
4. Soll die M1-bis-M6-Kollision durch `W-REQ-M1` bis `W-REQ-M6` aufgelöst werden?
