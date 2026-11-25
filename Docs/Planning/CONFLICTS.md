# Konfliktliste

Stand: 08.11.2026. Jeder Konflikt nennt beide Quellen. **Keine Entscheidung, welche Version gilt, ist hier getroffen**, außer wo der Entwickler ausdrücklich entschieden hat (Vorrangregel 1 in `README.md`).

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-01 | Größe der Hauptwelt | `Docs/Game/Entscheidungsstatus.md:66–67, 100` (2.000–2.200 km²) | Entwicklerangabe 08.11.2026 (DECISIONS N-01: kleiner, Größe offen) | Entwickler hat entschieden, Zahlen fehlen | N-01 gilt. Alte Zahlen als überholt markieren, sobald die neue Größe feststeht. |
| K-02 | Unterirdische Zivilisation | `Docs/Game/Spielwelt-und-Untergrund.md:346–349` (📋 gesperrt: „verborgene, unabhängige Zivilisation … mehrere abgeschlossene Städte unter der Erde“) | Entwicklerangabe 08.11.2026 (N-02: keine vollständige unterirdische Zivilisation) | Entwickler hat entschieden. Die gesperrte Textstelle widerspricht dem | N-02 gilt. Die 📋-Stelle muss als überholt gekennzeichnet werden. **Offen:** welche Teile bleiben (Untergrund-Einrichtung, Ausnahmen). |
| K-03 | Größte Untergrundstadt | `Docs/Game/Entscheidungsstatus.md:102` und §6.4 (50–100 km², vorläufig) | N-02 | Folge aus K-02 | Nicht mehr als Richtwert verwenden. |
| K-04 | Phase 1 Umfang | `Docs/Game/Entscheidungsstatus.md:108` und `CLAUDE.md` §4 (30–60 Minuten, 1–3 km²) | Entwicklerangabe 08.11.2026 (N-03: Einstieg ca. 30 Minuten) | Unterschiedliche Angaben derselben Phase | N-03 als Einstieg, 30–60 als Spanne. Fläche von 1–3 km² bleibt als Arbeitsbereich. |
| K-05 | Untergrund-Einrichtung und Hauptquartier | `Docs/Game/Entscheidungsstatus.md` §6.3 | Mehrere Dokumente | Nicht belastbar dokumentiert | Nicht zusammenführen. Offen lassen. |
| K-06 | Metropolis-Fläche | `Docs/Game/Entscheidungsstatus.md:101` (110 km²) | §6.2 (20–25 km² Testszenario, 70–73 km² älter) | Verschiedene Planungsstände | 110 km² als Arbeitsmaß. Keine Konstante. |
| K-07 | Engine-Aussagen | `Docs/Game/Entscheidungsstatus.md:53` (Unreal, C++20) | `Docs/Game/Gameplay-Systeme.md` §17 (nicht gelesen) | Nicht geprüft | In Paket 2 prüfen. |
| K-08 | Living World Umfang | `Docs/Game/Entscheidungsstatus.md:86` (verbindlich, Langzeit) | `Docs/Proposals/0010` §10 (Phasen L0–L5, nicht M1) | Konsistent, aber Umsetzungsreihenfolge offen | Kein Konflikt. Im Register als Langzeitvision geführt. |
| K-09 | Weltgrößen-Beispiele | `Docs/Game/Entscheidungsstatus.md:127` (Rechenbeispiele, z. B. 30 km² / 600.000) | Keine | Keine Festlegung | Als Rechenbeispiel markiert, nicht als Wert übernehmen. |

## Noch nicht geprüfte mögliche Konflikte

Diese Punkte sind nicht bewertet, weil die Quellen noch nicht gelesen wurden:
- `Docs/Decisions/0002`–`0006` (Engineering) gegen Spielentscheidungen in `Docs/Game/`.
- `01_CURRENT_CANONICAL_STATE` gegen `Docs/Game/Entscheidungsstatus.md`. Der Entscheidungsstatus beruft sich auf dieselben Quellen, die Übereinstimmung ist nicht geprüft.
- `99_DAILY_DELTAS` gegen `01_CURRENT_CANONICAL_STATE`.

## Paket 2 (08.11.2026): neue Konflikte aus dem Kanon

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-10 | Romantik mit erwachsenen NPCs | Kanon Block 10: breit verfügbar, wo Zustimmung und Logik passen (`DECISIONS.md` C-23) | Entscheidungsstatus §2.3, S-14: Adult-Bereich als getrenntes optionales Paket | Die Abgrenzung zwischen Basisspiel und Adult-Paket ist nicht geklärt | Offen. Keine Auflösung ohne Entwicklerentscheidung. |
| K-11 | Weltgröße im Verlauf | 1.500–1.800 km² als Standard (Kanon 05./06./08.09.2026, `DECISIONS.md` C-03) | ca. 2.000 km² weiches Ziel (Kanon 16.09.2026) und 2.000–2.200 km² (Entscheidungsstatus §6.1) | Historische Abfolge. Die späteren Stände sind weicher formuliert. | N-01 hat beides überholt. Zahlen nicht als Richtwert verwenden. |
| K-12 (teilweise gelöst 08.11.2026) | Unterirdische Zivilisation | Kanon-Lore: Erbauer und Nachfahren, 50–100 km² (C-42, C-45, Stand 15.10.2026) | N-02: keine vollständige unterirdische Zivilisation (08.11.2026) | Entwicklerangabe ist neuer. Die Kanon-Einträge müssen als betroffen markiert werden. | N-02 gilt nach Vorrangregel 1: keine vollständige unterirdische Zivilisation; Bunker, Höhlen, Labore bleiben möglich. Offen bleibt die Geltung der Gesellschaftsinhalte U24–U36 nach N-02 (siehe K-29, blockiert WE-GESELL-001/002). Textfassung und Umfang sind zu bestätigen (`OPEN_QUESTIONS.md` 0.3). |
| K-13 | Namen der Reiche | Kanon: Purgatory (Englisch) | `Docs/Game`: Fegefeuer (Deutsch, 34 Dateien) | Dieselbe Sache, unterschiedliche Namen | Kein inhaltlicher Konflikt. Im Glossar zusammenführen (Folgepaket). |
| K-14 | Mod-Laden nach Fehlern | Kanon C-37 (rekonstruiert aus bestätigten Angaben vom 09.09.2026) | Kein Originaltext vorhanden | Rekonstruktion, nicht Originalquelle | Als „rekonstruiert“ führen. Vor Umsetzung bestätigen lassen. |
| K-15 | Eisenbahn und Straßenverkehr | Engineering-Vorschläge im Tagesdelta (C-49), Prüfeingaben | `Docs/Game/Entscheidungsstatus.md` §4, dieselbe Einordnung | Konsistent | Kein Konflikt. Zahlen bleiben Prüfeingaben. |
| K-16 | Hunter-HQ-Beschriftung auf Karten | Entscheidungsstatus §6.5: Qualitätsmangel | Kartenanforderung 18./19.10.2026 (W-06) | Konsistent | Kein Konflikt. Als Fehler der Kartenbilder führen. |

## Projektsteuerung (08.11.2026)

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-17 | ID M1 bis M6 doppelt belegt | Meilenstein M1 (Proposal 0003, 0008) | Weltzustandsregeln M1–M6 in der README (Kanon-Tier 3) | Bestehende Kollision | Vorschlag `W-REQ-M1`–`W-REQ-M6` in `NAMING_PROPOSAL.md`. Keine Umbenennung ohne Zustimmung. |
| K-18 | „Phase 0.3“ doppelt belegt | WorldFacts-Format (`Docs/Decisions/0004`) | Interaktions-Inkremente (`Docs/Proposals/0004`) | Vom README bereits vermerkt | Alias-Tabelle trennt beide (`FOUNDATION-WORLDFACTS`, `PROTOTYPE-DOOR`). |


## Konsolidierung Welt (08.11.2026, Agent-Bericht `Konsolidierung/B-welt.md`, nicht unabhängig geprüft)

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-19 | Freigabe Phase A WORLD_700 | `Docs/Game/World/` (Register W-TE-008): 11 Regionen APPROVED_LOCKED | Master-Phase-A-Dokumentation: „PROPOSED / AWAITING REGION APPROVAL“ (Agent-Angabe, Quellen 02_WORLD_700 und 99_RECONSTRUCTION_SOURCES) | Nicht im Kanon gefunden; im Agent-Bericht geprüft, von mir noch nicht nachvollzogen | Entwickler bestätigt, welche Fassung gilt (9 oder 11 Regionen). |
| K-20 (entschieden 08.11.2026) | Gilt das 2.200-km²-Preset noch? | `Docs/Game/World/` W-GE-013 (🧭, Arbeitsstand 18.–19.10.) | N-01 (08.11.2026: Größe offen) | Blockiert Karten-, Metropolis- und Städteanforderungen (WE-GROESSE-006, WE-STADT-002, WE-STADT-009) | Entschieden (N-01/K-20, 08.11.2026): 2.200 km² ist kein Richtwert mehr; die neue Größe ist offen. Keine Festlegung auf 2.200 km². Die Größenfestlegung bleibt Entwicklerentscheidung. |
| K-21 | Hölle-Zeitdehnung | Kanon: OPEN (C-08) | Agent-Bericht: im Master OPEN (PE-E1022) | Konsistent | Kein Konflikt. Offen lassen. |


## Konsolidierung Gameplay und Spielvision (08.11.2026, Agent-Bericht `Konsolidierung/A-gameplay-spielvision.md`, Stichprobe offen)

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-22 | Amputation und Gliedmaßen | `Docs/Game/Gameplay-Systeme.md` Zeile 91: Verlust von Fingern, Händen, Gliedmaßen als Gore-Ergebnis | Kanon C-30 / `CANONICAL_ADDENDUM_2026-09-05.md:50`: keine Amputation außer über authored Story | Widerspruch | Entwickler entscheidet: nur Story-gebunden, oder physikalisches Ergebnis ohne Automatismus. |
| K-23 | Romantik im Basisspiel | Kanon C-23 / `CANONICAL_ADDENDUM_2026-09-05.md:39`; `Gameplay-Systeme.md` Zeile 220 (Beziehungs-Sandbox im Basisspiel) | `DECISIONS.md` S-14 (Adult-Paket getrennt, Basisspiel „nicht explizit“) | Identisch mit K-10 | Entwickler entscheidet. Blockiert Beziehungssystem und Adult-Paket. |
| K-24 | Geltungsbereich „individueller Tod bleibt endgültig“ | Kanon: `BLOCK_09_STATUS.md:5`, `CANONICAL_ADDENDUM_2026-09-05.md:44` (Companions endgültig) | `Docs/Game/Spielvision.md` Zeilen 380, 384: Permadeath nur optional, kein folgenloser Reset für den Spieler | Unklar, ob der Spieler eingeschlossen ist | Entwickler klärt. Dann Spielvision §14 als Richtung oder Festlegung kennzeichnen. |
| K-25 (entschieden 08.11.2026: Host-Persistenz bestätigt) | Koop-Autorität | `Docs/Game/Spielvision.md` Zeile 352 und `CLAUDE.md` §6: Host-Speicherstand ist autoritativ | Kanon: Netzwerk-Autorität offen (C-52, `MASTER_STATUS_2026-10-13.md:11`) | Docs führen als gesetzt, Kanon als offen | Entweder als Richtung im Kanon festhalten oder in Docs als offen führen. Blockiert Koop-Persistenz. |


## Konsolidierung Systemthemen (08.11.2026, Agent-Bericht `Konsolidierung/C-systeme.md`, Stichprobe offen)

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-26 | Todesmodell des Spielers | Kanon C-21: individueller Tod bleibt endgültig; Docs Kampfsystem BW-001/BW-012: Checkpoint nach echtem Tod (🧭) | Ermittlung E-QA-003 (Vorschlag) | Widersprüchlich; Quelle nennt den Konflikt selbst (BW-028) | Entwickler entscheidet Todesmodell. Blockiert Kampf, Ermittlung, Checkpoints. |
| K-27 | Todesursache wichtiger NPCs | `Docs/Game/Ermittlung` (E-ZE-007) und `Kreaturen/` (PO-020, ⚠️): wichtige NPCs sterben nur durch den Spieler | `Docs/Game/NPCs` (LT-006, LT-033, LB-017): unabhängige Lebensentwicklung und Fraktionskämpfe | Widerspruch | Entwickler entscheidet, welche Todesursachen für wichtige NPCs zulässig sind. |
| K-28 | Bunker und Hunter-HQ | CLAUDE.md §3: Pflichtklärung; Story-Variablen unvollständig (Docs `Kreaturen`, SY-WELT-002) | N-02: Bunker, Höhlen und Labore bleiben möglich | Offen, nach N-02 neu zu beantworten | Entwickler legt die Beziehung fest. Blockiert die Untergrund-Story. |


## Tagesdeltas (08.11.2026, Bericht `Nachweis/deltas.md`)

| ID | Thema | Quelle A | Quelle B | Stand | Vorgehen |
|---|---|---|---|---|---|
| K-29 | Gesellschaftsentscheidungen U24–U36 | Tagesdelta 2026-10-15, `docs/canon/UNDERGROUND_SOCIETY_DECISIONS_24-32.md`; `Docs/Game/Entscheidungsstatus.md:90`, `Spielwelt-und-Untergrund.md:362–366` (verbindlich) | N-02: vollständige Untergrundzivilisation entfällt | U24–U36 setzen die gestrichene Zivilisation voraus. Der Entwickler hat dazu nichts entschieden. | Entwickler entscheidet, welche U-Entscheidungen bestehen bleiben. Nicht eigenmächtig streichen. |
| K-30 | Status Phase 0.4 | `TASKS.md` (FOUNDATION-APPEARANCE: abgeschlossen) | Tagesdelta: widersprüchlicher Status | Widerspruch | Quelle prüfen, dann angleichen. |


## Adult-Bereich (08.11.2026, Bericht `Nachweis/adult.md`)

**Namensregel:** Die Konflikte K10 und K23 *innerhalb* des Adult-Bereichs (`Docs/Game/Adult/11-…`) sind **nicht** dieselben wie die Projektkonflikte K-10 und K-23. Im Adult-Bereich bleiben sie unverändert; Verweise immer mit Präfix (z. B. „Adult-K10“).

| ID | Thema | Stand | Vorgehen |
|---|---|---|---|
| K-31 | USER-LOCK „Handeln gegen den Willen“ | Im Adult-Bereich als nicht freigegeben markiert, nicht zurückgezogen | Entwickler entscheidet. Nicht eigenmächtig einordnen. |
| K-32 | Romantik im Basisspiel (Klasse A des Adult-Berichts) | Gleicher Kern wie K-10 / K-23 | Blockiert Klasse A. Siehe K-23. |
| K-33 | Geografie bei Zeitreisen (Stabilität) | `Docs/Game/World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md` §5, `Docs/Game/Spielwelt-und-Untergrund.md` §7, `Docs/Game/World/20-Entscheidungsregister.md` W-RE-006 (📋 „Geografie ist die stabile Schicht“, Entwicklerzitat 35#41E Frage 125) | Master `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` §2 (zählt Geografie zu den zeitversionierten Schichten) | Widerspruch, keine Entscheidung. Quelle für Entwicklerzitat: Chat 35. Welt-Nachweis WE-ZEIT-004 (Konflikt 5 aus `B-welt.md`). | Entwickler entscheidet, welche Fassung gilt. Bis dahin beide Aussagen mit Konfliktvermerk führen; nicht eigenmächtig auflösen. |
| K-34 | DLC-Aussage (Lore und Inhalte) | `Docs/Game/Kreaturen/20-Entscheidungsregister.md` Zeile 51 (K-GR-013): „Weitere Monster, Organisationen, Ebenen und Lore über DLC“ (📋) | `Docs/Game/Entscheidungsstatus.md` Zeile 65 und DECISIONS W-01: „kein klassisches DLC“ (bezogen auf die ältere Welt WORLD_700) | Unklar, ob sich „kein klassisches DLC“ nur auf die Karte oder auch auf Inhalte bezieht. Nachweis SY-KREA-027 (W-05 in `C-systeme.md`). | Entwickler klärt: Inhalte über DLC ja oder nein, Karte ja oder nein. Bis dahin beide Aussagen mit Konfliktvermerk führen. |
| K-35 | Automatische Fallauflösung (SPN-DEV-02, EVD-04) | Entwicklerangabe 08.11.2026: keine automatische Auflösung von Fällen (`DECISIONS.md` J, EVD-04) | `Docs/Game/Ermittlung/23-Testkatalog-und-Abnahmekriterien.md` T-F02 (ursprünglich: „Fall wird nach wenigen Hinweisen abgeschlossen“); `Docs/Game/Spielvision.md` Zeile 60 | **Entschieden (SPN-DEV-03, 08.11.2026), Konflikt geschlossen.** Ermittlungsfall wird nicht automatisch abgeschlossen; Beweise und Erkenntnisse schalten Dialoge, Aktionen und Schlussfolgerungen frei; der Abschluss ist eine bewusste Spielerhandlung (EVD-05). T-F02 umformuliert, Historie erhalten. | Erledigt: T-F02 umformuliert; SV:60 klargestellt; Core ohne automatische Abschlusslogik. |


## Welt-Nachweis (08.11.2026, Bericht `Nachweis/welt.md`)

- Status: vollständig integriert 74, vorhanden 17, teilweise 18, blockiert 9, verworfen 1 (WE-GROESSE-006), noch nicht integriert 1 (WE-GROESSE-001).
- Korrigiert: Weltgröße in `Entscheidungsstatus.md` §2.2 als überholt markiert (K-20).
- **Noch offen (nicht geändert):** `World/10` (§1, §7) und `World/09` (§7) sind nicht als N-02-historisch markiert.
- **Namensregel:** Die Konflikte K4, K6, K7, K8, K11, K13 aus dem Welt-Register haben keine Projekt-K-ID. K8 ist nicht mit K-08 (Living World) identisch.
