# Vertical Slice (SLICE-CASE-01): belegte und offene Abnahmekriterien

- **Stand:** 08.11.2026 (CONSOLIDATION-READINESS)
- **Regel:** Nur Kriterien, die in einer Quelle belegt sind, werden als Kriterium geführt. Was fehlt, steht unter „Offene Spezifikationspunkte“. Keine Zahl wird erfunden.

## A. Belegte Kriterien

| Nr. | Kriterium | Quelle | Status der Quelle |
|---|---|---|---|
| K1 | Ein vollständiger übernatürlicher Fall (Umfang des Slice) | `README.md` Kapitel 3 | 📋 „geplant“ |
| K2 | Dauer etwa 30–60 Minuten | `README.md` Kapitel 3; `MS13` Zeile 10; `ROADMAP.md` | 🧭 vorläufig |
| K3 | Fläche etwa 1–3 km² | `README.md` Kapitel 3; `MS13` Zeile 10; `CLAUDE.md` §4 | 🧭 vorläufig |
| K4 | Ablauf in sieben Schritten: Ankunft, Dialog, Ermittlung, Recherche, Vorbereitung, Konfrontation, Konsequenzen | `README.md` Kapitel 3 (Schritte 1–7) | 📋 |
| K5 | Die Fahrt ist Spielzeit (Gespräche, Radio, Telefon, Schweigen) | `README.md` Kapitel 3, Schritt 1 | 📋 |
| K6 | Aussagen von Zeugen sind Information, keine garantierte Wahrheit | `README.md` Kapitel 3, Schritt 2; `SV:220` | 📋 |
| K7 | Ein Fall ist lösbar, sobald genug richtige Hinweise vorliegen | `README.md` Kapitel 3, Schritt 3; `SV:60` | 📋 |
| K8 | Das Archiv verknüpft Fundstücke, auch mit falschem oder veraltetem Wissen | `README.md` Kapitel 3, Schritt 4; `SV:235`, `SV:238` | 📋 |
| K9 | Die Vorbereitung vor der Jagd zählt | `README.md` Kapitel 3, Schritt 5; `GS:107` | 📋 |
| K10 | Die Konfrontation nutzt die Schwäche; Auflösungsarten sind töten, vertreiben, bannen, fangen, Fluch brechen, retten, zerstören, sichern, verhandeln, überleben, evakuieren, überlisten | `README.md` Kapitel 3, Schritt 6; `SV:200` | 📋 |
| K11 | Das Ergebnis wird im Welt- und Fall-Journal festgehalten und wirkt nach | `README.md` Kapitel 3, Schritt 7; `CLAUDE.md` §4 (persistent consequences) | 📋 |
| K12 | Der Slice ist ein Meilenstein nach Phase 0 und vor den späteren Phasen | `ROADMAP.md` (`P2-VERTICAL-SLICE`) | geplant |

## B. Abgrenzung

- **Nicht** der Slice ist der Prototyp `PROTOTYPE-CASE-01` („Ein Raum, ein Zeuge, ein Beweis, ein Abschluss“, `NAMING_PROPOSAL.md` §4). Dieser ist ein früherer Testfall.
- Der erste spielbare Einstieg (ca. 30 Minuten, N-03) ist ein eigener Schritt `P1-FIRST-PLAYABLE`; die Zuordnung ist laut `ROADMAP.md` noch zu bestätigen.

## C. Offene Spezifikationspunkte (keine Kriterien)

| Nr. | Offener Punkt | Wo offen |
|---|---|---|
| O1 | Akt, Figuren und Fallinhalt des Slice | `TASKS.md` (SLICE-CASE-01), `OPEN_QUESTIONS.md` |
| O2 | Auslöser des Falls (Ereignis zu Beginn) | `SV:136` (❓) |
| O3 | Welche Kreatur, welcher Schauplatz, welche Konfrontationsarten im Slice enthalten sind | nicht festgelegt |
| O4 | Messregel für die Dauer 30–60 min (Mittelwert, Spanne, Testdurchlauf) | nicht festgelegt |
| O5 | Messregel für die Fläche 1–3 km² (welche Karte, welche Abgrenzung) | nicht festgelegt |
| O6 | Akzeptanztest für „vollständiger Fall“ (Ende-zu-Ende-Durchlauf ohne Eingriff, Speichern und Laden) | nicht festgelegt, nur Vorschlag |

## D. Voraussetzungen aus der Aufgabenliste (ohne Kriterium)

`CASE-EVIDENCE`, `CASE-DIALOG`, `NPC-BASIC`, `CREATURE-BASIC`, `VEHICLE-DRIVING` sind alle `geplant`. Ohne sie ist kein Slice-Schritt ausführbar, außer Ankunft (Fahrt) und Speichern/Laden. Die Reihenfolge ist offen (P0-PLAYABLE-PLACEHOLDER).
