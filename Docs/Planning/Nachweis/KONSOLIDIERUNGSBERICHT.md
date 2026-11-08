# Konsolidierungsbericht (Stand 08.11.2026)

Quellenbasierter Gesamtstand nach den Paketen CONSOLIDATION-01, -02 (Welt, Systeme), -ADULT und -DELTA. Keine neuen Spielentscheidungen, keine Spielcodeänderungen, keine Commits oder Pushes.

## 1. Vollständig dokumentiert

| Bereich | Stand | Quelle |
|---|---|---|
| Welt (WE-…) | 91 vollständig, 17 bereits vorhanden, 1 verworfen | `Nachweis/welt.md` (Zählung per Skript aus der Statusspalte) |
| Systeme (SY-…) | 92 vollständig, 48 bereits vorhanden | `Nachweis/systeme.md` |
| Adult (ADU-…) | 131 belegt in `Docs/Game/Adult/`; Kategorien verbindlich 61, vorläufig 18, offen 26, widersprüchlich 8, historisch 9, verworfen 8, unklar 1 | `Nachweis/adult.md` (Abschnitt G) |
| Tagesdeltas 14.–19.10. | 44 Einzelpunkte, davon die meisten in den Docs belegt | `Nachweis/tagesdeltas.md` |
| Planungsregel | Freitext-Regel als verbindlich (P-01) | `PLANUNGSFRAGEN.md`, `DECISIONS.md` |

Wichtig: „vollständig“ heißt „in den Zieldokumenten belegt und mit Status versehen“. Es heißt nicht, dass die Sache entschieden oder umgesetzt ist.

## 2. Teilweise integriert oder blockiert

- **Teilweise integriert: 0** (in welt und systeme; alle Zeilen sind entweder vollständig, blockiert oder in einem Konflikt).
- **Welt blockiert: 10**: K-20 (WE-GROESSE-007/008, WE-STADT-002/009, WE-WASSER-001), K-28 (WE-UNTERGRUND-006), K-12 (WE-GESELL-001/002), K-26 (WE-REICH-006), Quelle offen (WE-ZEIT-005). Konflikt offen: K-33 (WE-ZEIT-004).
- **Systeme blockiert: 10**: K-27 (SY-ERM-011, Teil), K-22 (SY-KAMPF-011), K-21 und K-26 (SY-KREA-011/012), K-26 (SY-NPC-013), weitere aus dem älteren Stand. (SY-WELT-006 ist vollständig; nur die Stadtgrößen hängen an K-20.) Konflikt offen: K-34 (SY-KREA-027).
- **Gameplay (`Nachweis/gameplay.md`, 222 Zeilen):** 213 „bereits vorhanden“, 7 blockiert, 2 vollständig. Diese Zeilen wurden in den Paketen dieser Sitzungen **nicht** neu geprüft (Restarbeit R1).
- **Adult:** 22 offen, 12 vorläufig, 8 widersprüchlich. Blockiert durch K-23, K-31, K-10 (siehe 3).

## 3. Konflikte, die tatsächlich eine Entwicklerentscheidung brauchen

Quelle: `Docs/Planning/CONFLICTS.md` (Stand-Spalte), abgeglichen mit den Nachweisen.

**Eindeutig Entwicklerentscheidung:**
- K-01: neue Größe der Hauptwelt (N-01 gilt, Zahl fehlt).
- K-10 / K-23 / K-32: Romantik im Basisspiel (blockiert ADU-A-002 bis A-005).
- K-19: Freigabe Phase A WORLD_700 (Agent-Bericht, im Kanon nicht gefunden).
- K-22: Amputation und Gliedmaßen (blockiert SY-KAMPF-011).
- K-24: Geltungsbereich „individueller Tod bleibt endgültig“.
- K-26: Todesmodell des Spielers (blockiert WE-REICH-006, SY-NPC-013).
- K-27: Todesursache wichtiger NPCs (blockiert SY-ERM-011, Teil).
- K-28: Bunker und Hunter-HQ nach N-02 (blockiert WE-UNTERGRUND-006).
- K-29: Gesellschaftsentscheidungen U24–U36 ohne Zivilisation (Geltung, blockiert WE-GESELL-001/002 über K-12).
- K-31: USER-LOCK „Handeln gegen den Willen“ (ADU-E-01, nicht genehmigt).
- K-33: Geografie bei Zeitreisen (Master gegen Docs).
- K-34: DLC-Aussage Lore und Inhalte.

**Entscheidung nötig, aber nur als Bestätigung oder Zahlenfrage:** K-21 (Hölle-Zeitdehnung). Die `World/26` führt „40:4 bestätigt 22.10. (PE-E1022)“. `CONFLICTS.md` führt K-21 als OPEN nach Kanon. PE-E1022 steht nicht im Entscheidungsregister. Das ist ein Widerspruch, den der Entwickler klären muss.

**Keine Entwicklerentscheidung, sondern Dokumentations- oder Prüfarbeit:** K-03 (Folge von N-02), K-04 (durch N-03 gelöst), K-06 (Metropolis 110 km², PE-E1022), K-12 (siehe 4), K-16, K-17 (ID-Kollision M1–M6), K-18 (Phase 0.3 doppelt), K-20 (durch N-01 entschieden, Nachweise noch nicht angepasst), K-25 (entschieden, Docs prüfen), K-30 (Phase-0.4-Status, Fakt gegen TASKS prüfen), K-05, K-07, K-08, K-09, K-11, K-13, K-14, K-15.

## 4. Historische oder überholte Aussagen ohne eindeutige Kennzeichnung

Maschinelle Suche in `Docs/Game/` nach Begriffen aus N-01/N-02 (2.200 km², 50–100 km², Untergrundstadt, Erbauer, Schienenverbund, Bevölkerungszahlen). Ergebnis: 39 Treffer ohne Markierung im Umfeld; viele davon sind Chroniken, Versionsgeschichte, Konfliktregister oder schon als Kandidat markiert. **Echte Restarbeit (R3):**

- `Docs/Game/Entscheidungsstatus.md:147`: Klärungsliste „Größe … der größten Untergrundstadt“ (historisch, N-02).
- `Docs/Game/World/03-Massstab-Flaechen-Proportionen.md:43`: Tabelle „2.200 km² Zielfläche“, ohne Hinweis auf N-01/K-20; ebenso :217 und :220.
- `Docs/Game/World/02-Weltvision-und-geografische-Grundstruktur.md:26`: „Die Zahlen (2.200 km² …) sind 🧭“, nach N-01 nicht mehr Richtwert.
- `Docs/Game/World/20-Entscheidungsregister.md:116–117`: W-ST-002/003 (110 km² „für den Kandidaten mit 2.200 km²“).
- `Docs/Game/World/22-Offene-Fragen-Katalog.md:62` (OF-24, Footprint der größten Untergrundstadt) und `:69` (OF-31, Erbauer): historisch nach N-02.
- `Docs/Game/World/10-Untergrundzivilisation.md:123`: 💡-Aussage zu Bahnhöfen zwischen „Stadtkulturen“ (historisch).
- `Docs/Game/Fahrzeuge-und-Verkehr/27-Performance-und-Skalierung.md:121`: VEH-DEC-PF-013 (Kopie, in `Fa28` bereits markiert, hier nicht).
- `Docs/Game/World/28-Nachtragsvorschlag-Spielwelt-und-Untergrund.md:52`: „Untergrund 50–100 km² tentativ“ (Vorschlagsdokument).

## 5. Master-Teile ohne inhaltliche Auswertung

Quelle: Dateizählung im Master (nur gelesen); `Docs/Planning/SOURCES.md` (veraltet, siehe R5).

- **`01_CURRENT_CANONICAL_STATE/` (57 Dateien):** nur die zitierten Addenda und Statusdateien (CANONICAL_ADDENDUM 09-04/05/06, CURRENT_CANON 09-16, MASTER_STATUS 10-13, ADDENDA 10-15, BLOCK_09). Der Rest ist nicht ausgewertet.
- **`99_DAILY_DELTAS/` (102 Dateien):** alle sechs `CHANGELOG.md` gelesen; `docs/`, `tests/`, `install.sh`, Manifeste und Assets nicht.
- **`02_EMBEDDED_BASE_ARCHIVES/` (3.689 Dateien, 33 MB), `_SOURCE_ARCHIVE/` (497, 1,8 GB), `_SOURCE_RESEARCH/` (3, 849 MB), `99_RECONSTRUCTION_SOURCES/` (60, 23 MB), `04_HISTORY/` (402), `99_CANON_AUDITS/` (16), `99_MANIFESTS/` (3), `06_NEW_REFERENCES_2026-09-10/` (2), `03_EXTERNAL_LIBRARY_REGISTRY/` (18), `00_START_HERE/` (3, nur Namen gesehen): nicht ausgewertet.**
- **`02_WORLD_700/` (12 Dateien, 209 MB):** teilweise über `W700-PA` (WORLD_MASTER_LOCK_1) zitiert; kein vollständiger Abgleich.
- Die Nachweise markieren 32 Stellen als „nicht gepr.“ und 3 als „nicht geprüft“ (welt.md). Das ist die Grenze der heutigen Auswertung.

## 6. Ist die Aufgabensteuerung zuverlässig nutzbar?

**Bedingt.**

Positiv:
- `CLAUDE.md` §2 verweist auf `Docs/Planning/README.md`, `CANON.md`, `ROADMAP.md`, `TASKS.md`, `DEPENDENCIES.md`, `Milestones/`. Alle Dateien existieren, `verify_planning.py` meldet 156 Referenzen ohne Bruch.
- `TASKS.md` hat stabile IDs, eine Quellspalte und ein Vokabular (`geplant`, `bereit`, `in Arbeit`, `blockiert`, `abgeschlossen`, `verworfen`). 45 Einträge.

Mängel:
- **Fünf Status außerhalb des Vokabulars:** `WORLD-MAP-CORE`, `NAV-ROUTING-CORE`, `SIM-NPC-MEMORY` („abgeschlossen (Core)“), `NAV-DEMO-REGION` („(Demo)“), `PROTOTYPE-DOORKEY-RULES` („(Platzhalter)“). Dazu `DEC-UNDERGROUND-SCOPE` mit „bereit (Bestätigung … ausstehend)“.
- **Die Konsolidierungsarbeit ist nicht im Register.** Die Pakete WELT, SYSTEME, ADULT und DELTA stehen nur in `CHANGELOG.md`. Es gibt keinen Aufgabeneintrag für die 14 offenen Konflikte und für die Restarbeiten.
- **Blockierungen sind nicht verlinkt.** Die blockierten Zeilen in den Nachweisen verweisen auf K-IDs, nicht auf `TASKS.md`-IDs.
- **`CLAUDE.md` nennt die neue Planungsregel nicht.** Ich habe `CLAUDE.md` nicht geändert (Projektanweisung). Vorschlag: einen Verweis auf `Docs/Planning/PLANUNGSFRAGEN.md` ergänzen, falls der Entwickler das will.

## 7. Konkrete Restarbeiten

| Nr. | Arbeit | Bearbeiter | Entscheidung nötig |
|---|---|---|---|
| R1 | Gameplay-Nachweis (222 Zeilen) gegen `Docs/Game/Gameplay-Systeme.md` und `Spielvision.md` prüfen | Claude | nein |
| R2 | K-20 ist entschieden: die fünf daran gebundenen Welt-Zeilen (WE-GROESSE-007/008, WE-STADT-002/009, WE-WASSER-001) von „blockiert (K-20)“ auf „blockiert durch N-01 (Größe offen)“ umstellen; SY-WELT-006 entsprechend | Claude | nein |
| R3 | Historische Stellen aus Abschnitt 4 kennzeichnen (8 Stellen) | Claude | nein (Kennzeichnung, keine Festlegung) |
| R4 | K-12 vereinheitlichen: CONFLICTS sagt „gelöst 08.11.“, die Nachweise blockieren weiter. Entweder Nachweise umstellen oder K-12 wieder öffnen | Entwickler | ja (Bestätigung) |
| R5 | `Docs/Planning/SOURCES.md` aktualisieren (Adult, Ermittlung, Welt-Module und Deltas sind gelesen; Abschnittsangaben veraltet) | Claude | nein |
| R6 | `TASKS.md`: fünf Status auf das Vokabular bringen; Konsolidierungs-Aufgaben und Blockaden als Einträge ergänzen | Claude | nein |
| R7 | K-21 klären: PE-E1022 („40:4 bestätigt 22.10.“) in `DECISIONS.md` eintragen oder K-21 offen lassen | Entwickler | ja |
| R8 | Entscheidungen der Abschnitte 3 (eindeutig) einholen; Fragen als Freitextfragen mit Optionsliste formulieren (PLANUNGSFRAGEN.md) | Entwickler | ja |
| R9 | Master-Restteile (Abschnitt 5) nur nach Freigabe auswerten; große Archive (`_SOURCE_ARCHIVE`, `_SOURCE_RESEARCH`) bleiben außen vor | Entwickler | ja (Umfang) |
| R10 | `CLAUDE.md` um Verweis auf `PLANUNGSFRAGEN.md` ergänzen (nur mit Freigabe) | Entwickler | ja |
| R11 | Vorschaubilder 19.10. prüfen, ob „Hunter HQ“ sichtbar ist (W-KA-017) | Entwickler | nein (Prüfung) |

## 8. Hinweis zu den Zählungen

Die Zählungen in den Nachweisen waren in mehreren Fällen nicht konsistent (Welt, Systeme). Die heutigen Werte stammen aus einer Skriptzählung der Statusspalte. Die Gesamtzahl der Anforderungen (120 + 151 + 222 + 131) bleibt, die Verteilung wurde angepasst.

## 9. Status nach CONSOLIDATION-FINAL-CLEANUP (08.11.2026)

| R | Status | Ergebnis |
|---|---|---|
| R1 | teilweise | Zitierungen maschinell geprüft (233; GP-BALLISTIK-001 korrigiert), Zählung korrigiert (213 / 7 / 2). Inhaltliche Gegenprüfung aller 222 Zeilen steht aus (`CONS-GAMEPLAY`). |
| R2 | erledigt | K-12 und K-20 in `CONFLICTS.md` und in den Nachweisen konsistent (siehe `CHANGELOG.md`). Keine neue Größe, keine Konfliktentscheidung. |
| R3 | erledigt | Acht Stellen in `Docs/Game/` als überholt oder historisch markiert, Inhalt erhalten. |
| R4 | übersprungen | Entwicklerentscheidung (Bestätigung K-12 oder Wiedereröffnung). |
| R5 | erledigt | `SOURCES.md` mit datierter Aktualisierung (Abschnitt 0). |
| R6 | erledigt | Fünf Statuswerte in `TASKS.md` normalisiert; Konsolidierungsaufgaben `CONS-*`. |
| R7 | übersprungen | Entwicklerentscheidung (PE-E1022 in DECISIONS oder K-21 offen). |
| R8 | übersprungen | Entscheidungen des Entwicklers; die Fragen bleiben als Freitextfragen offen. |
| R9 | übersprungen | Umfang des Master-Rests braucht Freigabe. |
| R10 | erledigt | Verweis in `CLAUDE.md` auf `PLANUNGSFRAGEN.md` und P-01 (mit Freigabe). |
| R11 | übersprungen | Prüfung der Vorschaubilder durch den Entwickler. |
