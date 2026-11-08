# Entscheidungsregister

Pfade sind relativ zum Repo-Wurzelverzeichnis. „Zeile“ bezieht sich auf den Stand vom 08.11.2026.
Die Spalte **Status** folgt `README.md`. Die Spalte **Quelle** nennt die Stelle, an der die Entscheidung steht.

## A. November 2026 (neueste Entwicklerangaben)

Diese Angaben wurden am 07./08.11.2026 im Gespräch gemacht. Eine ausformulierte Textfassung im Repo existiert noch nicht. Sie gelten nach Vorrangregel 1, bis sie schriftlich gefasst und bestätigt sind (siehe `OPEN_QUESTIONS.md`, Punkt 1).

| ID | Thema | Status | Entscheidung | Quelle | Überschreibt / betrifft |
|---|---|---|---|---|---|
| N-01 | Größe der Hauptwelt | Verbindlich (Textfassung ausstehend) | Die Hauptwelt wird kleiner als bisher geplant. Die endgültige Größe ist offen. | Entwicklerangabe 08.11.2026 | Betrifft `Docs/Game/Entscheidungsstatus.md` Zeilen 66–67, 100 (2.000 bis 2.200 km²) und §6.1. Diese Zahlen gelten nicht mehr als Richtwert, bis die Größe neu festgelegt ist. |
| N-02 | Unterirdische Zivilisation | Verbindlich (bestätigt 08.11.2026) | Die vollständige unterirdische Zivilisation entfällt. Bunker, Höhlen, Geheimlabore und kleinere unterirdische Gebiete bleiben möglich. Alte Zivilisationskonzepte (Erbauer, Umfang 50–100 km², Bevölkerung, unterirdischer Schienenverbund) sind historisch, nicht mehr verbindlich. | Entwicklerangabe 08.11.2026; Bestätigung im Gespräch 08.11.2026 | Überholt: C-42, C-45, W-UG-001 bis W-UG-003, W-UG-008 bis W-UG-012 (Nachträge in den Quelldateien). Master (nur Verweis, unverändert, SY-WELT-001): UC:7–20, US:7–15 und 19–22, UA:5–16, AI:9–21 sind als historisch einzustufen. |
| N-03 | Entwicklungsreihenfolge | Verbindlich (Textfassung ausstehend) | Schrittweise Entwicklung, beginnend mit etwa 30 Minuten spielbarem Inhalt. | Entwicklerangabe 08.11.2026 | Präzisiert `Docs/Game/Entscheidungsstatus.md` Zeile 108 (Phase 1: 30–60 Minuten, 1–3 km²). Die Spanne 30–60 Minuten bleibt als Arbeitsbereich, der Einstieg wird auf ca. 30 Minuten gesetzt. |
| P-01 | Regel für strukturierte Planungsfragen | Verbindlich (Entwicklerangabe 08.11.2026) | Jede Planungsfrage hat eine Freitextoption; Freitext hat Vorrang; Kombinationen zulässig; Konflikte werden nicht eigenmächtig aufgelöst. Gilt nicht für gewöhnliche Implementierungsaufträge. | Entwicklerangabe 08.11.2026 (Q-DELTA-1); `PLANUNGSFRAGEN.md` | Gilt für alle künftigen Planungsfragen |

## B. Bestätigt und gesperrt (aus `Docs/Game/Entscheidungsstatus.md` §2)

Diese Einträge übernehmen die Festlegungen, nicht die Quellenformulierungen. Vollständige Details bleiben in der Quelle.

### B.1 Produkt und Technik

| ID | Thema | Status | Festlegung | Quelle |
|---|---|---|---|---|
| T-01 | Marke | Verbindlich | Originale IP; keine Übernahme bestehender Namen, Figuren, Lore, Symbole. Rechtliche Markenprüfung vor Veröffentlichung. | `Docs/Game/Entscheidungsstatus.md:52` |
| T-02 | Engine und Sprache | Verbindlich | Unreal Engine plus C++20, C++-first, eigene Persistenz- und Weltzustandsarchitektur, keine eigene vollständige Engine, keine stille Migration auf C++23. | `Docs/Game/Entscheidungsstatus.md:53` |
| T-03 | Produktionsstrategie | Verbindlich | KI-gestützte Entwicklung durch einen Entwickler, 8–15 Stunden pro Woche. Volle Langzeitvision bleibt erhalten. Frühe sichtbare spielbare Ergebnisse. Kennzeichnung `[VS]`/`[EARLY]`/`[LATER]`/`[FUTURE/AI]` ist Priorität, keine Streichung. | `Docs/Game/Entscheidungsstatus.md:54` |
| T-04 | Modding | Verbindlich | Anbieterunabhängiger Kern. Lokale `.spnmod`-Dateien und `<GameRoot>/mods/`. Aktivierung pro Profil und Spielstand. Steam Workshop nicht Pflicht. Mods additiv. Offizieller Online-Modus ohne Mods. | `Docs/Game/Entscheidungsstatus.md:55` |
| T-05 | Mehrspieler | Verbindlich (Richtung) | Koop-Kampagne bis 4 Spieler. Einzelspieler offline. Koop mit Mods oder Cheats ohne Transfer in den Online-Fortschritt. Offizieller Online-Modus ohne Mods oder Cheats. | `Docs/Game/Entscheidungsstatus.md:56` |
| T-06 | Persistenz | Verbindlich | Zentraler Weltzustand, stabile IDs, segmentierte, absturzsichere, migrierbare Spielstände. | `Docs/Game/Entscheidungsstatus.md:57` |
| T-07 | Testphilosophie | Verbindlich | Tests sind Teil der Architektur, nicht Schlussphase. | `Docs/Game/Entscheidungsstatus.md:58` |
| T-08 | Distribution | Verbindlich | Zusatzpakete (Stimme, Texturen, Sprachen) wählbar. Standardinstallation nicht aufblähen. | `Docs/Game/Entscheidungsstatus.md:59` |

### B.2 Welt

| ID | Thema | Status | Festlegung | Quelle |
|---|---|---|---|---|
| W-01 | Ältere Welt | Verbindlich | Geografisch und optisch abgeschlossen. Makrogeografie gesperrt. Additive Inhalte erlaubt. Kein klassisches DLC. Referenz- und Technik-Master. | `Docs/Game/Entscheidungsstatus.md:65` |
| W-02 | Größere Hauptwelt | Verbindlich, durch N-01 eingeschränkt | Standard-Hauptwelt des Basisspiels. Keine Endgenerierung vor vollständigen Anforderungen und räumlicher Zuordnung. | `Docs/Game/Entscheidungsstatus.md:66` |
| W-03 | Städte | Verbindlich | Vorbild für Stimmung und Vielfalt, keine realen Flächen. Frühere Gesamtstadtfläche von 570 km² abgelehnt. Plausible Nachbarschaften, Hügel, Vegetation, Parks, Flüsse, organische Straßen. | `Docs/Game/Entscheidungsstatus.md:68` |
| W-04 | Reiche | Verbindlich | Himmel, Hölle und Fegefeuer als eigene Open Worlds im Basisspiel. Persistenz auch dort. | `Docs/Game/Entscheidungsstatus.md:69` |
| W-05 | Zeitreise | Verbindlich (Spielsystem) | Großes Spielsystem. Jahresgenau etwa ab 1600 bis zur Gegenwart. Keine harte Untergrenze. | `Docs/Game/Entscheidungsstatus.md:70` |
| W-06 | Geheimhaltung | Verbindlich | Geheimes Hauptquartier wird auf öffentlichen Spielerkarten nicht auffällig beschriftet. | `Docs/Game/Entscheidungsstatus.md:71` |
| W-07 | Karte | Verbindlich | Eine geografisch maßgebliche Karte. Abgeleitete Ansichten. Technisch bestandene Kandidaten sind nicht automatisch akzeptiert. | `Docs/Game/Entscheidungsstatus.md:72` |

### B.3 Spielinhalt

| ID | Thema | Status | Festlegung | Quelle |
|---|---|---|---|---|
| S-01 | Spielgattung | Verbindlich | Open-World-Action-Adventure mit RPG-, Ermittlungs-, Immersive-Sim- und Survival-Horror-Tiefe. Keine sichtbaren Monster- oder Charakterlevel. | `Docs/Game/Entscheidungsstatus.md:78` |
| S-02 | Ton | Verbindlich | Düsterer Mystery-Horror nach dem Vorbild der frühen Phase des Genres. | `Docs/Game/Entscheidungsstatus.md:79` |
| S-03 | Protagonist | Verbindlich | Frei erstellbarer Hauptcharakter, männlich oder weiblich ohne Sperren. Feste Hunter-Crew. | `Docs/Game/Entscheidungsstatus.md:80` |
| S-04 | Spielbeginn | Verbindlich | Ländlich, persönliche Vergangenheit, erste lokale Fälle. Großes Hauptquartier erst nach mehreren Stunden Hauptgeschichte entdeckt. | `Docs/Game/Entscheidungsstatus.md:81` |
| S-05 | Story | Verbindlich | Akte und Kapitel. Stabile Handlungsachse. Hauptmissionen nie dauerhaft verpassbar. Spielerfreiheit ändert vor allem das Wie. | `Docs/Game/Entscheidungsstatus.md:82` |
| S-06 | Fälle | Verbindlich | Mehrwege-Ermittlungen. Frühes korrektes Lösen möglich. Journal. Detektivmodus ohne Hilfe. | `Docs/Game/Entscheidungsstatus.md:83` |
| S-07 | Archiv | Verbindlich | Wissensgraph mit qualitativer Anzeige. Wissensstatus: bestätigt, vermutet, umstritten, widerlegt, historisch. | `Docs/Game/Entscheidungsstatus.md:84` |
| S-08 | Monster | Verbindlich | Keine Respawn-Spawnpunkte. Mehrdimensionale Klassifikation. Schwächen nicht nur als Bonusschaden. | `Docs/Game/Entscheidungsstatus.md:85` |
| S-09 | Living World, NPC-Gedächtnis, Nemesis | Verbindlich (Langzeitvision, kein M1-Feature) | Persistente NPC-Identitäten, Langzeitgedächtnis, Nemesis und Rivalität (auch für intelligente Kreaturen), autonome Fraktionen, persistente Konsequenzen, eigenständige Lebensentwicklung, skalierbare Weltsimulation, Integration ohne widersprüchliche Zustände. Umsetzung, Datenmodell und Zahlen offen. | `Docs/Game/Entscheidungsstatus.md:86`; `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` §1 |
| S-10 | Medaillen | Verbindlich | Bronze, Silber, Gold, Platin, Diamant. Verborgene Kriterien. Replay ändert den Story-Zustand nicht. | `Docs/Game/Entscheidungsstatus.md:87` |
| S-11 | Soziale Medien | Verbindlich | Nie Pflicht für Hauptgeschichte oder normale Nebenfälle. Nur optionale dynamische Weltereignisse. | `Docs/Game/Entscheidungsstatus.md:88` |
| S-12 | Fahren | Verbindlich | Manuelle Kupplung, Abwürgen, freie Gangwahl. Handschalt-Variante für jedes Fahrzeug als Ziel. Frühe gegenüber langfristiger Version offen. | `Docs/Game/Entscheidungsstatus.md:89` |
| S-13 | Rail | Verbindlich (Langzeitziel) | Alle Züge innerhalb der Karte. Facility-Bereiche. Hohe Elektrifizierung. Fahren nur mit Job oder Berechtigung. | `Docs/Game/Entscheidungsstatus.md:91` |
| S-14 | Adult-Bereich | Verbindlich (Rahmen) | Getrenntes optionales Paket mit Zustimmungs- und Altersregeln. Basisspiel nicht explizit. Detailplanung läuft separat. | `Docs/Game/Entscheidungsstatus.md:92`; `Docs/Game/Adult-System.md` |

Hinweis: S-09 und die Gesellschafts-Einträge (`Entscheidungsstatus.md` Zeile 90, Untergrund U24–U36) sind im Quelldokument nur verdichtet. Die Detailregeln müssen in den Folgepaketen aus den Quellen gezogen werden.

## C. Vorläufig (aus `Docs/Game/Entscheidungsstatus.md` §3)

Diese Werte sind Arbeitsstände. Sie gelten nicht als Festlegung.

| ID | Thema | Status | Arbeitsstand | Quelle |
|---|---|---|---|---|
| V-01 | Fläche der Hauptwelt | Vorläufig, durch N-01 überholt | 2.200 km² Festland plus Ostinsel. | `Docs/Game/Entscheidungsstatus.md:100` |
| V-02 | Metropolis | Vorläufig | 110 km², 20 Bezirke, ca. 20 km² Innenstadt, ca. 12 km² Hochhauskern, 3–5 Supertalls. | `Docs/Game/Entscheidungsstatus.md:101` |
| V-03 | Größte Untergrundstadt | Vorläufig, durch N-02 überholt | 50–100 km² Horizontale. | `Docs/Game/Entscheidungsstatus.md:102` |
| V-04 | Reichsgrößen | Vorläufig | Hölle 350–500 km², Fegefeuer 500–750 km², Himmel 250–400 km². | `Docs/Game/Entscheidungsstatus.md:103` |
| V-05 | Hauptgeschichte | Vorläufig | Etwa 80–100 Stunden. | `Docs/Game/Entscheidungsstatus.md:104` |
| V-06 | Zeitskala | Vorläufig (Idee) | Etwa 2 Echtstunden pro Spieltag. | `Docs/Game/Entscheidungsstatus.md:105` |
| V-07 | Tierarten | Vorläufig (Zielbereich) | 220–280 auf der Hauptkarte. | `Docs/Game/Entscheidungsstatus.md:106` |
| V-08 | Kernarten von Monstern | Vorläufig | Grob 20–30 sehr tiefe statt 100 flacher. | `Docs/Game/Entscheidungsstatus.md:107` |
| V-09 | Phasenziele | Vorläufig (Schätzung) | Phase 0: 2–4 Monate. Phase 1: ca. 1–3 km², 30–60 Minuten. Phase 2: ca. 5–10 km², 3–5 Stunden. Phase 3: ca. 25–50 km², 10–20+ Stunden. | `Docs/Game/Entscheidungsstatus.md:108`; `CLAUDE.md` §4 |
| V-10 | Online | Vorläufig (langfristig) | 64–100 Spieler pro große Weltinstanz. | `Docs/Game/Entscheidungsstatus.md:109` |
| V-11 | Basisspiel-Größe | Vorläufig (Richtung) | Etwa 100 GB. | `Docs/Game/Entscheidungsstatus.md:110` |
| V-12 | Rennsport | Vorläufig (Planungsgespräch) | Rallyes mit 10–20 Etappen, Stadtkurse, Schotter-Bergrennen, mindestens 5 Strecken. | `Docs/Game/Entscheidungsstatus.md` §3 (Tabelle) |
| V-13 | Eingabe DualSense | Vorläufig | L1 + L2 als analoge Kupplung. Technische Machbarkeit im Prototyp zu prüfen. | `Docs/Game/Entscheidungsstatus.md` §3 (Tabelle) |

## D. Überholt

| ID | Thema | Status | Frühere Festlegung | Ersetzt durch | Quelle |
|---|---|---|---|---|---|
| X-01 | Weltgröße 1.500–1.800 km² | Überholt | Standardwelt 1.500–1.800 km² (Favorit 1.650). | W-02, V-01 | `Docs/Game/Entscheidungsstatus.md` §6.1 |
| X-02 | Weltgröße 200–300 km² | Überholt | Bis Ende August 2026. | W-02 | `Docs/Game/Entscheidungsstatus.md` §6.1 |
| X-03 | Metropolis 70–73 km² | Überholt | September 2026. | V-02 | `Docs/Game/Entscheidungsstatus.md` §6.2 |
| X-04 | Gesamtstadtfläche 570 km² | Verworfen | Als Entscheidung abgelehnt. | W-03 | `Docs/Game/Entscheidungsstatus.md:68` |

## E. Vision und Offenes

Nicht als Entscheidungen erfasst. Siehe `OPEN_QUESTIONS.md`.

## F. Kanon aus `01_CURRENT_CANONICAL_STATE` (Paket 2, gelesen am 08.11.2026)

Alle 57 kanonischen Dateien wurden gelesen (Liste und Hashes in `SOURCES.md` Abschnitt 2). Der Kanon-Pfad lautet unten `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/`. Er löst über den Symlink `Planning/Master` auf.

P = `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/`

### F.1 Welt und Größe

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-01 | WORLD_700 | Verbindlich (gesperrt) | Geografisch und visuell abgeschlossen. Makrogeografie gesperrt. Ergänzungen nur additiv. Keine klassische DLC-Karte. Referenz-, Test- und Technik-Master. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION_2026-09-27.md:5`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` (Abschnitt „WORLD_700 — role clarified“) | 27.09.2026 |
| C-02 | WORLD_1500 / größere Hauptwelt: Endgenerierung | Verbindlich (blockiert) | Keine finale Generierung vor vollständigen Anforderungen und räumlicher Zuordnung. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION_2026-09-27.md:6`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CURRENT_CANON_2026-09-16.md:6` | 27.09.2026 |
| C-03 | Weltgröße | Vorläufig (ersetzt durch N-01) | 1.500–1.800 km² als Standard (05./06./08.09.), ca. 1.650 km² als Favorit. Ab 16.09. ca. 2.000 km² als weiches Ziel. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:3`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:4`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CURRENT_CANON_2026-09-16.md:6` | Überholt durch N-01 |
| C-04 | Metropolis-Tests | Vorläufig | Bei 2.200 km²: Metropole ca. 20–25 km², größere Städte ca. 8–10 km². Flächenwachstum unterlinear. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:11` | 15.10.2026 |
| C-05 | Städte-Vorbild | Verbindlich | Vorbild für Charakter und Vielfalt: New York, Chicago, San Francisco, Boston. Keine realen Flächen. Ursprünglich abgeleitete 570 km² als Entscheidung abgelehnt. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:10`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md:14` | 15.10.2026 |
| C-06 | Realm-Größen | Vorläufig, Vermutungen | Hölle 350–500 km² (Favorit 420). Purgatorium 500–750 km² (650). Himmel 250–400 km² (320) plus Erinnerungsdomänen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:25–27`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/REALMS/STATUS_2026-09-10.md:3` | Sept. 2026, nicht gesperrt |
| C-07 | Realm-Regeln | Vorläufig | Basisspiel-Minimum: Hölle, Purgatorium, Himmel. Jedes Reich mit eigener Realm Bible, räumlicher Zuordnung, Traversal, Persistenz. Größe ist nicht gleich Entwicklungsaufwand. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:58`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_UPDATE_2026-09-08.md:26` | Sept. 2026 |
| C-08 | Hölle | Vorläufig | Vertikal, dicht, labyrinthisch, innenraumreich. Machtstrukturen, Gefängnisse, Ruinen. Zeitdilatation möglich, Verhältnis offen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:30` | Sept. 2026 |
| C-09 | Purgatorium | Vorläufig | Wildnis, Überleben, Monster-Ökologie. Territorien, Beutetiere, Wanderung, Rudel. Stärkster Kandidat für Ökologie-Simulation. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:44` | Sept. 2026 |
| C-10 | Himmel | Vorläufig | Nicht einfach Wolkenwelt. Gemeinsamer offener Raum plus persönliche Erinnerungsdomänen. Torsystem und besondere Übergangsregeln. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:58` | Sept. 2026 |
| C-11 | Reise zwischen Reichen | Vorläufig | Unterscheidet sich deutlich vom Fahren in der materiellen Welt. Persistenz: geöffnete Tore, entfernte Artefakte, Veränderungen bleiben. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:72–74` | Sept. 2026 |

### F.2 Zeitreise

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-12 | Zeitreise als Spielsystem | Verbindlich | Großes geplantes System, kein Set fester Epochenkarten. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:20` | Sept. 2026 |
| C-13 | Jahresgenauigkeit | Verbindlich (Anker) | Im voll unterstützten Zeitraum jedes einzelne Jahr. Anker etwa 1600 bis zur Gegenwart. Früher möglich, mit geringerer Detailtiefe. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:21`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:79` | Sept. 2026 |
| C-14 | Monat, Tag, Uhrzeit | Vorläufig (Standard) | Beim Jahreswechsel bleiben Monat, Tag und Uhrzeit erhalten. Keine eigene Zielwahl im ersten Umfang. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:22` | Sept. 2026 |
| C-15 | Historischer Weltzustand | Verbindlich (Prinzip) | Rekonstruktion aus zeitversionierten Schichten und Änderungsereignissen, nicht eine Karte pro Jahr. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:23` | Sept. 2026 |
| C-16 | Vorwärtsreise | Offen | Bewusst offen. Spielerhandlungen können den Kausalzustand ändern. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:24` | Sept. 2026 |
| C-17 | Verzweigte Zeitlinien | Vision | Später und komplexer, keine Startbedingung. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:25` | Sept. 2026 |
| C-18 | Untergrenze | Verbindlich | Keine harte kosmologische Untergrenze für Reisen in die Vergangenheit. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:78` | Sept. 2026 |
| C-19 | Historische Behauptungen | Verbindlich | Reale Behauptungen (etwa „freie Energie“ vor 1775) gelten nicht als Fakten. Ähnliche Ideen dürfen als fiktive Geschichte des Spiels verwendet werden. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md:81`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION.md:10` | Sept. 2026 |

### F.3 Übernatürliche Schicht und Monster

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-20 | Historische übernatürliche Schicht | Verbindlich (Rahmen) | Mehrhundertjährige verborgene Geschichte. Religiöse Gemeinschaften, Jägerfamilien, Okkultisten, Militärprogramme. WWII-Okkultgeschichte in eigener IP. Künstliche Wächter möglich. Inspiration aus dem Genre, aber original. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:30` | Sept. 2026 |
| C-21 | Block 9: akzeptierte Säulen | Verbindlich (im Wesentlichen) | Seltenes Jägernetz. Mehrere Organisationen. Öffentliche übernatürliche Ambiguität. Persistente Monster-Ökologie. Individueller Tod bleibt endgültig. Territorien, Nester, Migration, Fortpflanzung. Intelligente Kreaturen. Artspezifische KI. Systemische dynamische Fälle. Forensik. Unvollständiges Wissen. Varianten und Mutationen. Rituale ohne generisches Mana. Verfluchte Objekte. Spukimmobilien. Kulte. Übernatürliche Hotspots. Besessenheit, Flüche, Infektion möglich. Realistische Verletzungen und Gore. Qualitätsfokus beim Monsterkatalog. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/BLOCK_09_STATUS.md:5` | Sept. 2026 |
| C-22 | Block 9: offen | Offen | Namen der Institution nach Men-of-Letters-Vorbild, Urform-Konzepte, exakte Kosmologie, Zeitverhältnisse der Reiche, Todesablauf, Gesamtzahl der Monster. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/BLOCK_09_STATUS.md:7` | Sept. 2026 |

### F.4 Lebenssimulation und Sandbox (Block 10)

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-23 | Beziehungssystem | Verbindlich (Richtung) | Dynamische Freundschaft, Vertrauen, Respekt, Furcht, Sympathie. Romantische und sexuelle Beziehungen mit erwachsenen NPCs breit möglich, wo Interesse, Zustimmung und Charakterlogik passen. Mehrere gleichzeitige Beziehungen. Eifersucht, Konflikte, Geheimhaltung, Trennungen. Zusammenleben, Heirat, gemeinsames Eigentum. Familiengenerationen niedrige Priorität. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:38–41` | Sept. 2026 |
| C-24 | Wichtige Charaktere und generische NPCs | Verbindlich | Wichtige Figuren mit eigener Tiefe. Generische NPCs über systemische Beziehungsinhalte. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:42` | Sept. 2026 |
| C-25 | Begleiter | Verbindlich (Richtung) | Unterschiedliche Fähigkeiten (Kampf, Medizin, Forschung, Technik, Spurensuche). Begleiter können dauerhaft sterben, außer Story-kritische Figuren im Hauptquartier. Story-Missionen können Begleiter verlangen. Solo bleibt außerhalb der Story spielbar. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:43–45` | Sept. 2026 |
| C-26 | Hund als Begleiter | Verbindlich (Richtung) | Rasse, Persönlichkeit, Training, Gehorsam, Rückruf, Suche, Witterung, Verletzung, Altern, Tierarzt. Impfungen optional. Spuren über die Umgebung, kein Zielpfeil. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:46` | Sept. 2026 |
| C-27 | Heimlichkeit | Verbindlich (Richtung) | Licht, Schatten, Sicht, Schall, Oberflächen, Wetter, Ablenkung, Verstecken, Überwachung, Alarme, Hausfriedensbruch, Verkleidungen, Ausweise, Verhalten, Wiedererkennung. Keine sofortige perfekte Tarnung. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:47` | Sept. 2026 |
| C-28 | Spurensuche | Verbindlich (Richtung) | Fußspuren, Blut, Reifen, Schleifspuren, Vegetation, Haare, Federn, Witterung. Wetter und Schnee verschlechtern die Spur. Manipulierte Spuren möglich. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:48` | Sept. 2026 |
| C-29 | Forensik | Verbindlich (Richtung) | Fingerabdrücke, DNA, Ballistik, Schuhabdrücke, Fasern, Toxikologie, Todeszeitraum, Obduktion, Tatort, Labore mit Bearbeitungszeit. Mehrdeutigkeit und übernatürliche Anomalien. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:49` | Sept. 2026 |
| C-30 | Verletzungen | Verbindlich (Richtung) | Blutverlust, Brüche, Gehirnerschütterung, innere Verletzungen, Verbrennungen, Vergiftung, Kälte, Hitze, Dehydrierung, selektive Infektion. Krankenhaus und Operationen möglich. Keine volle Rehabilitationssimulation. Kein Amputations-Automatismus. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:50` | Sept. 2026 |
| C-31 | Lebensbedürfnisse | Verbindlich (Richtung) | Schlaf mit Folgen, ohne Dauer-Verwaltung. Essen optional. Alkohol beeinflusst Reaktion und Fahren. Drogen mit eigenen Wirkungen, Abhängigkeit bei extremem Konsum. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:51` | Sept. 2026 |
| C-32 | Kleidung und Inventar | Verbindlich (Richtung) | Kleidung mit Wetter-, Schutz- und Lagerfunktion, nass, schmutzig, blutig, beschädigt. Körperinventar begrenzt, Fahrzeuge bieten Stauraum, Hauptquartier große Vorräte. Keine Munitionsverwaltung ohne Sinn. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:52–53` | Sept. 2026 |
| C-33 | Smartphone und Netz | Verbindlich (Richtung) | Navigation, Verkehr, Wetter, Banking, Märkte, Wetten, Nachrichten, Messaging, Kamera, Mitfahrdienste, Lieferung, Shops, Immobilien, Fahrzeuge, Marktplatz, Jobs. Funklöcher zählen. Satellitenkommunikation als Spezialoption. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:54` | Sept. 2026 |
| C-34 | Kalender und Freizeit | Verbindlich (Richtung) | Feiertage, Tourismus, saisonale Ereignisse, Urbex, Metalldetektor, Geocaching, Flohmärkte, Nachlassverkäufe, Auktionen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:55` | Sept. 2026 |
| C-35 | Keine Klassensperren | Verbindlich (Prinzip) | Der Spieler darf viele Systeme und Lebensstile gleichzeitig verfolgen. Keine künstlichen Sperren, nur weil der Charakter Jäger ist. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md:56` | Sept. 2026 |

### F.5 Mods und Verteilung

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-36 | Mod-Host | Verbindlich (Block 5) | Steam Workshop nicht Pflicht. Mod-Kern hängt nicht an einem Anbieter. Standard `<GameRoot>/mods/`. Installierte und aktivierte Zustände sind getrennt. Zusätzliche Bibliotheken optional. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md:3` | 11.09.2026 |
| C-37 | Mod-Laden (rekonstruiert) | Verbindlich, aber rekonstruiert | Fehlende kritische Mods blockieren das Laden. Ausnahme nur nach automatischem Backup und Risikohinweis. Aktivierung pro Spielstand. In-Session-Deaktivierung logisch. Mod-Zustand bleibt für spätere Aktivierung erhalten. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_RECONSTRUCTED_2026-09-09.md:5` | Rekonstruiert am 09.09.2026 aus bestätigten Angaben |
| C-38 | Block 10 Modding | Abgeschlossen | Der Stand ist als abgeschlossen vermerkt. Der Originalartefakt-Text ist nicht vorhanden. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MODDING/MODDING_BLOCK_STATUS_2026-09-11.md:5` | 11.09.2026 |
| C-39 | Optionale Pakete | Verbindlich | Standardinstallation mit komprimierter hochwertiger Stimme. Große verlustfreie Stimmenpakete separat. Verlustfrei mit 48 kHz, nicht 96/192. Sprachpakete einzeln wählbar. Ultra- und Cinematic-Texturpakete optional. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/DISTRIBUTION_AND_OPTIONAL_PACKS/OPTIONAL_CONTENT_PACKS_2026-09-10.md:5` | 10.09.2026 |
| C-40 | Stimmproduktion | Verbindlich | Freiwillige Freunde oder Familie liefern saubere Referenzaufnahmen für TTS. Einwilligung, kommerzielle Rechte, Umfang und Löschregeln sind Pflicht. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10.md:3` | 10.09.2026 |
| C-41 | Produktions-Referenzen | Verbindlich | Blender- und Umgebungs-Prompts nutzen externe Tutorials und visuelle Referenzen. Konkrete Szenenbeschreibungen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10.md:6` | 10.09.2026 |

### F.6 Untergrund, Bunker, Lore (kanonisch, aber betroffen von N-02)

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-42 | Unterirdische Zivilisation | Kanon-Lore, betroffen von N-02 | Uralte Erbauer gründeten die verborgenen Städte. Heute meist Nachfahren, wenige Gründer leben stark eingeschränkt weiter. Gegenseitiger Schutz mit den Jägern. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:24` | 15.10.2026 |
| C-43 | Tesla im Bunker | Kanon-Lore | Tesla lebt im gesicherten Jägerbunker und forscht dort. Bunkerbewohner kennen seine Identität, Außenstehende nicht. Zugang auf einen sehr kleinen geprüften Kreis begrenzt. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md:23`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:25` | 13.10. und 15.10.2026 |
| C-44 | Familienschlüssel | Kanon, Ursprung reserviert | Der Schlüssel führt zur Bunkerentdeckung. Ursprung, erster Besitzer und Verbindung zu den Erbauern bleiben für eine große Story-Wendung reserviert. Nicht ausfüllen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:26` | 15.10.2026 |
| C-45 | Größte Untergrundstadt | Vorläufig, explizit bekräftigt | 50–100 km² horizontal. Keine finale Festlegung, kein Geometrie-Lock. Bevölkerung mehrere Hunderttausend bis ca. 1 Mio., ebenfalls vorläufig. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/02_DECISION_REGISTER.md:16`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:27–28` | 15.10.2026 |
| C-46 | Bunker oder Hauptquartier | Offen | Ob der unterirdische Bunker und das oberirdische Hauptquartier identisch, getrennt oder verbunden sind, ist ungeklärt. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:29` | 15.10.2026 |
| C-47 | Gesellschaft im Untergrund (U24–U32) | Verbindlich (Konsens aus Tagesdelta) | Zugehörigkeit durch Vertrauen, nicht automatische Aufnahme. Vielfältige Familienmodelle. Interessengeleitetes Lernen ohne Pflichtfächer. Begrenzte Regeneration und Verjüngung. Lange Lebensspannen. Freiwillige Genetik und Organersatz. Infrastruktur-KI und Roboter. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/01_CONFIRMED_AND_PROVISIONAL.md:14–23` | 18.10.2026 |
| C-48 | KI und Kommunikation (U33–U36) | Verbindlich (Beschreibung), keine Rechtsentscheidung | KI-Entwicklungsstufen ohne automatischen Bewusstseinsnachweis. Anerkennung individuell. Aufsicht je nach KI-Typ und Aufgabe. Mehrere Kommunikationsarten, Neuro-Schnittstellen nur freiwillig. Privatsphäre zentral. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/01_CONFIRMED_AND_PROVISIONAL.md:25–31` | 18.10.2026 |
| C-49 | Eisenbahn und Straßenfahrzeuge (Details) | Vorschlag, nicht bestätigt | Detaillierte Infrastruktur, Signalisierung, Fahrpläne, Fahrzeugphysik. Prüfeingaben, keine bestätigten Zahlen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/03_ENGINEERING_PROPOSALS.md:9` | 18.10.2026 |
| C-50 | Engineering-Vorschläge zu KI und Netz | Vorschlag, nicht bestätigt | Geräte, Projektionen und Schnittstellen als austauschbare Frontends mit Einwilligung. Keine Annahme von Implantaten oder Oberflächen-Internet. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/03_ENGINEERING_PROPOSALS.md:4–7` | 18.10.2026 |

### F.7 Technik, Produktion, Mehrspieler, Entdeckungen

| ID | Thema | Status | Festlegung | Quelle | Stand |
|---|---|---|---|---|---|
| C-51 | Technik | Verbindlich (Basis) | Unreal Engine und C++20, C++-first, engine-neutrale Simulationsgrenzen, wo sinnvoll. Eigene Persistenz- und Weltzustandsarchitektur. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CURRENT_CANON_2026-09-16.md:8` | 16.09.2026 |
| C-52 | Koop | Richtung | Story-Koop bis 4 Spieler. Netzwerk-Autorität, Synchronisation und Produktionsreihenfolge offen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md:11`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/03_OPEN_QUESTIONS.md:7` | 13./15.10.2026 |
| C-53 | Produktionsstrategie | Verbindlich | KI-first, Solo, 8–15 Stunden pro Woche. Langfristige Vision bleibt. Frühe spielbare Ergebnisse. Tags sind Prioritäten, keine Streichungen. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md:6–8` | 13.10.2026 |
| C-54 | Phasen | Vorläufig | Phase 0: 2–4 Monate, Platzhalter in 2–4 Wochen. Phase 1: 1–3 km², 30–60 Minuten, eine vollständige Spielschleife. Keine technischen Sperren. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md:9–10`; `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md:19` | 13./15.10.2026 |
| C-55 | Entdeckungen und Sammelobjekte | Vorläufig, offen | Alte Basis: 1.200–1.500 physische Sammelobjekte für ca. 200 km². Gesamt ca. 5.000 Entdeckungsobjekte. Neu je Kategorie nach räumlicher Zuordnung. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-04.md:20–23` | 04.09.2026 |
| C-56 | Hauptwelt-Entwicklungsstand | Verbindlich (Ausgangslage) | Nur Ausgangslage. Keine Endgenerierung. | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-04.md:8–9` | 04.09.2026 |
| C-57 | Stand der Planung | Information | Mehrere Statusdateien bestätigen nur den Stand ohne neue Entscheidung (Sep. 2026). | `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-09-17.md` bis `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-09-27.md` | 17.–27.09.2026 |

## G. Bestehendes Register `Docs/Game/World/20-Entscheidungsregister.md` (Paket 3)

Das Register im Repo ist das detaillierte Weltregister mit eigenen Kennungen (`W-GE-`, `W-UG-`, usw.). Es wird hier nicht kopiert. Dieser Abschnitt sagt nur, welche Kennungen durch die Entscheidungen N-01 und N-02 berührt sind. Das Register selbst wurde **nicht verändert**, bis N-02 bestätigt ist (siehe `OPEN_QUESTIONS.md` 0.3).

| Kennungen im Register | Betroffen durch | Hinweis |
|---|---|---|
| W-GE-001 bis W-GE-005 | Bestätigt (WORLD_700 gesperrt, größere Welt blockiert) | Keine Änderung durch N-01. |
| W-GE-006 bis W-GE-014 | N-01 (Größe der Hauptwelt) | Historische Stände (🕰️) und Kandidat 2.200 km² (🧭). Nach N-01 nicht mehr als Richtwert verwenden. |
| W-GE-015 bis W-GE-016 | Bestätigt (größenunabhängige Bauweise, Festland) | Keine Änderung. |
| W-UG-001, W-UG-002, W-UG-003 | N-02 | Eigenständige Untergrundzivilisation und Erbauer. Als 📋 gekennzeichnet, im Widerspruch zu N-02. |
| W-UG-004 bis W-UG-012 | N-02 (teilweise) | Schienennetz, Abschottung, Elite, größte Stadt 50–100 km², Bevölkerung, Naturräume, weitere Anlagen. |
| W-UG-013 | Keine Betroffenheit | Fiktive freie Energie als Fiktion. |
| W-UG-014 bis W-UG-019 | N-02 (Frage nach dem Umfang) | Bunker und Tesla. Der Bunker ist von N-02 nicht ausdrücklich betroffen, aber die Frage nach der Gesellschaft darum schon. |
| W-UG-020 | N-02 | Entwicklerwunsch: Bunker als echter Hub. Nicht ändern, bevor der Umfang geklärt ist. |
| W-UG-021 bis W-UG-024 | Keine Betroffenheit | Skizzen und Lage. |
| W-UG-025 | N-02 (teilweise) | Geheime Forschungskomplexe, „von sehr klein bis riesig“. |
| W-UG-026 | Keine Betroffenheit | Eigene Planung des Bunkers. |

Abschnitte des Registers, die in Paket 3 noch nicht gelesen wurden: 3 bis 8 und 10 bis 18 (Karte, Städte, Natur, Gewässer, Verkehr, Gesellschaft, Wirtschaft, Fahrzeuge, Fahrphysik, Motorsport, NPC-Verkehr, Technik, Reiche und Zeit, Kennzahlen). Sie folgen in den nächsten Paketen.


## H. Phasen (Entwicklerangabe 08.11.2026)

| ID | Thema | Status | Festlegung |
|---|---|---|---|
| PH-01 | P4-RELEASE | Verbindlich als Entwicklungsphase | Ziel ist die kommerzielle Veröffentlichung auf Steam. Termin, Releaseumfang und Releasekriterien offen. |
| PH-02 | POST-LAUNCH | Verbindlich als Planungsbereich | Langfristiger Bereich. Keine Zusage für bestimmte Erweiterungen oder Inhalte. |


## I. Entwicklerentscheidungen 08.11.2026 (Konsolidierung)

| ID | Thema | Status | Festlegung |
|---|---|---|---|
| N-02 | Unterirdische Zivilisation | Verbindlich (bestätigt) | Keine vollständige unterirdische Zivilisation. Einzelne Bunker, Höhlen, Geheimlabore und kleinere Untergrundbereiche bleiben möglich. Alte Zivilisationskonzepte sind historisch. |
| K-20 | 2.200-km²-Preset | Entschieden | Die früheren 2.200 km² sind überholt. Die endgültige Weltgröße ist offen (N-01). Alte Geografie bleibt als historische Referenz erhalten. |
| K-25 | Koop-Persistenzmodell | Verbindlich (bestätigt) | Welt und Fortschritt gehören zum Host-Speicherstand. Gastcharaktere sind an diesen Host-Save gebunden. |
| PH-01 | P4-RELEASE | Verbindlich | Entwicklungsphase: kommerzielle Veröffentlichung auf Steam. Termin, Umfang, Releasekriterien offen. |
| PH-02 | POST-LAUNCH | Verbindlich | Langfristiger Planungsbereich, keine Zusage. |

**Weiter offen (nicht entschieden):** K-19, K-22, K-23, K-24, K-26, K-27, K-28.
