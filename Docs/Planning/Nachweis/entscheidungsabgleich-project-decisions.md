# Entscheidungsabgleich: PROJECT_DECISIONS.md

Quelle: `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` (707 Zeilen, 58 Abschnitte).
Stand 09.11.2026. Die Datei nennt sich „living source of truth“; Regel: Punkte mit OPEN sind keine Implementierungsentscheidungen.

**Prüfstufen:**
- **Wortlaut:** die Zeilen des Abschnitts wurden gelesen und die Regeln mit Einschränkungen gegen die Zielstelle verglichen.
- **Stichprobe:** einzelne Regeln wurden gegen die Zielstelle geprüft, der Abschnitt nicht vollständig.
- **Verweis:** der Abschnitt wiederholt einen bereits abgeglichenen Changelog (31.08.); die Abdeckung folgt aus diesem Abgleich.

| Zeilen | Abschnitt | Prüfstufe | Zielstelle in `Docs/` | Befund |
|---|---|---|---|---|
| 1–5 | Produktidentität | Stichprobe | `Game/Spielvision.md` (Markenprüfung) | Klearance und Namensoffenheit belegt |
| 6–11 | Technologie | Stichprobe | `Planning/CANON.md`, `Game/Gameplay-Systeme.md` | Unreal, C++20, kein C++23 belegt |
| 12–30 | Welt | Stichprobe | `Game/World/02`, `03`, `04`, `18`; `Spielwelt-und-Untergrund.md` | Größe 200–300 km² historisch (N-01 offen); PCG, Reiche, DLC offen belegt |
| 31–44 | Geschichte und Fälle | Stichprobe | `Game/Spielvision.md` §5, `Ermittlung/02`, `03` | mehrere Lösungswege, Journal, Referenzweg belegt |
| 45–51 | Ermittlungsmodi | Stichprobe | `Ermittlung/03` | Detektivmodus ohne Führung belegt |
| 52–66 | Archiv, Sammelobjekte, Fortschritt | Stichprobe | `Ermittlung/04`, `Game/Spielvision.md` | 1.200–1.500 historisch; Unbekannt ohne Kategoriename belegt |
| 67–77 | NPCs und KI | Stichprobe | `Game/NPCs/`, `Game/Kreaturen/10` | Gedächtnis, Beziehungen ohne Zahlen, Grounded nie allwissend belegt |
| 78–87 | Weltzustand und Persistenz | Stichprobe | `Game/NPCs/21`, `Game/Gameplay-Systeme.md` | Segmentierung, 20–50 MB, PostgreSQL nur Backend belegt |
| 88–115 | Anatomie, Verletzung, Medizin | Stichprobe | `Game/Kampfsystem/13`, `Gameplay-Systeme.md` §8 | Trauma, Blutung, Schock, Stufen der Behandlung belegt |
| 116–128 | Tod und Scheitern (OPEN) | Stichprobe | `Game/Spielvision.md`, `Kampfsystem/14` | Kein Soulslike, Checkpoint optional, Rettung belegt |
| 129–137 | Schwierigkeit und Konfiguration | Stichprobe | `Game/Spielvision.md`, `Kampfsystem/02` | Presets, Grounded, Zugänglichkeit getrennt belegt |
| 138–161 | Waffen | Stichprobe | `Game/Kampfsystem/10`, `Gameplay-Systeme.md` | Röhrenmagazin, Teilmagazin, Fehlfunktionen belegt |
| 162–194 | Ballistik | Stichprobe | `Game/Kampfsystem/05`, `Gameplay-Systeme.md` §5 | Flugzeit, Wind, Abpraller, Kill-Cam belegt |
| 195–218 | Nahkampf und Ringen | Stichprobe | `Game/Kampfsystem/04`, `Gameplay-Systeme.md` | Ehrenkodex, Umgebungsgegenstände belegt |
| 219–230 | Fahrzeuge | Verweis + Stichprobe | `Game/Fahrzeuge-und-Verkehr/04`, `05`, `28` | Kupplung, Konfiguration, Bedienung belegt |
| 231–236 | Progression (OPEN) | Stichprobe | `Game/Spielvision.md`, `Entscheidungsstatus.md` | abnehmende Erträge, Fortschritt durch Nutzung |
| 237–245 | Modding | Stichprobe | `Game/Gameplay-Systeme.md` (Mods), `Game/Adult-System.md` | Schichten, Lua optional, Vertrauensstufen belegt |
| 246–259 | Wissensplattform | Stichprobe | `Game/Spielvision.md`, `Planning/DECISIONS.md` | Spoilerstufen, Einlösung, DLC-Gating teilweise belegt |
| 260–278 | Testing und Prompt-Governance | Stichprobe + Prozess | `Game/Gameplay-Systeme.md` (Tests), `Development/` | Prompt-Governance ist Prozess der alten Bibliothek, keine Spielanforderung |
| 279–292 | Mehrgegner-Kampf, Improvisation, Kapitulation | Stichprobe | `Game/Kampfsystem/02`, `04`, `07`; `Gameplay-Systeme.md` | Glasflasche, Mehrgegner, Kapitulation belegt |
| 293–328 | Stealth und horror-KI | Stichprobe | `Game/Kreaturen/10`, `Gameplay-Systeme.md` §7 | Unsicherheit, Schall, Türen, Verstecke belegt; Analogschleichen ergänzt |
| 329–340 | Begleiter | Stichprobe | `Game/NPCs/19`, `Spielvision.md` §6.3 | autonome Begleiter, Plan, Befehle belegt |
| 341–357 | Audio | Stichprobe + Ergänzung | `Game/Gameplay-Systeme.md` §14.2; `Development` §9 | Bitraten als Planungsbereich ergänzt |
| 358–378 | Rendering und Plattformen | Stichprobe + Ergänzung | `Game/Gameplay-Systeme.md` §14, `Entscheidungsstatus.md` | Konsolen-Anzeigeoptionen ergänzt |
| 379–388 | Testphilosophie | Stichprobe | `Game/Gameplay-Systeme.md` (Tests), `Kampfsystem/30` | Testarchitektur, Regressionen belegt |
| 389–406 | Prompt-Bibliothek, Scrum | Prozess | `Development/Produktions-Referenzen-und-Prompts.md` §7, §11 | Prozessregeln, keine Spielanforderungen |
| 407–413 | Genre und Progression | Stichprobe | `Game/Spielvision.md` (§1), `Entscheidungsstatus.md` | keine Level, keine Lootfarben belegt |
| 414–442 | Kreaturen-Rahmen | Stichprobe | `Game/Kreaturen/03`, `04`, `02` | Dimensionen, Alpha ohne Beschriftung, Schwächenarten belegt |
| 443–450 | Organische Ereignisse | Stichprobe | `Game/Ermittlung/26`, `09` | Vorlauf, Verzögerung, Folgen belegt |
| 451–456 | Friedliche Wesen, unabhängige Jäger | Stichprobe | `Game/Spielvision.md` §Wesen | Spielerwahl ohne Automatismus belegt |
| 457–465 | Archiv-Epistemologie | Stichprobe | `Game/Ermittlung/04`, `Kreaturen/02` (L12) | Wissen statt Daten, Hypothesen belegt |
| 466–470 | Langfristige Bedrohungen | Stichprobe | `Game/Spielvision.md` §5 | Akt-Bedrohungen belegt |
| 471–500 | Dialog-Architektur | Stichprobe + Ergänzung | `Game/NPCs/20`, `Spielvision.md` §15; `Development` §10 | Dialog-IDs, Stimmen-Bibel, Revisionen ergänzt |
| 501–515 | Dialogvolumen, Stimmstrategie | Stichprobe | `Game/Spielvision.md` §15.2, Stimmen | Volumen, Klon-Strategie, Einwilligung belegt |
| 516–525 | Voice, Gesicht, Körper | Stichprobe + Ergänzung | `Development` §10 | Signalprofile, Lippensynchronisation ergänzt |
| 526–535 | Prompt-Explorer | Stichprobe + Ergänzung | `Development` §7, §10 | Technik (React, Volltext) als Richtung ergänzt |
| 536–547 | Visuelle Referenzen | Stichprobe + Ergänzung | `Development` §7, §10 | verbindlich/Stimmung, Visual-IDs ergänzt |
| 548–565 | Blender-Workflow | Ergänzung | `Development` §11 | MESSEN/LOOK, Modulspezifikation, Iterationsschleife ergänzt |
| 566–583 | Claude-Laufzeittests | Ergänzung | `Development` §11 | Ausführungsschleife, Testkarten, Fehlerreproduktion ergänzt |
| 584–595 | Menschliche Freigabe | Ergänzung | `Development` §11 | Zustände und Abgrenzung ergänzt |
| 596–604 | Vorproduktion und Sequenz | Ergänzung | `Development` §11 | Reihenfolge ergänzt; globale Reihenfolge offen |
| 605–616 | Fortschrittsberichte | informell | nicht übernommen | 22–23 % und 2–3 % sind Schätzungen des Nutzers, keine Projektanforderung |
| 617–622 | Protagonist, Crew, HQ (Changelog-Auszug) | Verweis | `Game/NPCs/19`, `Spielvision.md` | Abgleich im Changelog 31.08. |
| 623–707 | Changelog-Auszug 31.08. (Abschnitte 1–55) | Verweis | siehe `entscheidungsabgleich-2026-08-31.md` | Dublette; nicht erneut geprüft |

**Ergebnis:** Alle 58 Abschnitte sind erfasst. Die Dublette ab Zeile 617 ist dem Changelog-Abgleich zugeordnet. Stichprobe bedeutet ausdrücklich **keine** Vollständigkeit: Die Abschnitte 1–400 sind noch nicht im Wortlaut abgeglichen (Aufgabe 2 des Auftrags).

**Offen aus diesem Abgleich:**
- Zeilen 1–400: Wortlaut-Prüfung aller Abschnitte. Die Stichproben sind keine Abnahme.
- Abschnitt „Progressionsschätzung“ (605–616) ist informell; eine Entscheidung dazu ist nicht erforderlich.
- Keine Entscheidung wurde eigenmächtig getroffen; OPEN-Punkte bleiben OPEN.

## Wortlaut-Protokoll (MASTER-MIGRATION-13, 09.11.2026)

Diese Tabelle ist die einzige Quelle für die Prüfstufe „Wortlaut geprüft“. Ein Stichworttreffer zählt nicht.

| Nr. | Zeilen | Abschnitt | Prüfstufe | Zielstellen (im Wortlaut gelesen) | Ergänzung / Befund |
|---|---|---|---|---|---|
| 1 | 6–11 | Produktidentität | Wortlaut geprüft | `Game/Spielvision.md` §2; `Game/Entscheidungsstatus.md` Z. 52, 137 | Originale Marke, rechtliche Prüfung, Titel offen belegt |
| 2 | 12–18 | Technologie | Wortlaut geprüft | `Game/Gameplay-Systeme.md` Z. 372–375; `Planning/CANON.md` Z. 10; `Game/World/18` Z. 31 | Unreal und C++20, kein stilles C++23 |
| 3 | 19–30 | Welt | Wortlaut geprüft | `Game/Spielwelt-und-Untergrund.md` §2, §4, §15, Z. 226, 319–320; `Game/World/02` Z. 104; `Game/Spielvision.md` Z. 61 | Ergänzt: Fall- und Story-Gebäude handgebaut. Größe 200–300 historisch (N-01 offen) |
| 4 | 31–44 | Story und Fälle | Wortlaut geprüft | `Game/Spielvision.md` Z. 113–115, 119, 203, 205; `Game/Ermittlung/02` §6 | Auflösungsarten, Journal, Referenzweg belegt |
| 5 | 45–51 | Ermittlungsmodi | Wortlaut geprüft | `Game/Ermittlung/03` Z. 26–27, 47, 52–66 | Hilfen unabhängig, Detektivmodus ohne Führung belegt |
| 6 | 52–66 | Archiv, Fortschritt | Wortlaut geprüft | `Game/Ermittlung/04` Z. 60, 111, 126; `Game/Spielvision.md` Z. 243–245 | Kartenhierarchie, qualitative Anzeige, Monsterstufen belegt |
| 7 | 67–77 | NPCs und KI | Wortlaut geprüft, **Konflikt** | `Game/NPCs/03`, `Game/Gameplay-Systeme.md` Z. 208; `Game/Kreaturen/10` Z. 209, 227 | StateTree und eigene C++-Systeme: PROJECT_DECISIONS als Regel, `Kreaturen/10` als 💡 Vorschlag. Konflikt dokumentiert, nicht aufgelöst |
| 8 | 78–87 | Weltzustand und Persistenz | (Vorfassung MIGRATION-13, überholt) Stichprobe | `Game/NPCs/21`; `Game/World/18`; `Game/Gameplay-Systeme.md` | Kernregeln belegt; Segmentierung und Absturzsicherheit im Wortlaut nicht vollständig gelesen |
| 9 | 88–115 | Anatomie, Verletzung, Medizin | Wortlaut geprüft | `Game/Gameplay-Systeme.md` Z. 80–86, 91; `Game/Kampfsystem/13` | Trauma, Blutung, Schmerz, Behandlungsstufen, Heilungsstufen belegt |

**Stand:** 9 von 58 Abschnitten im Wortlaut geprüft (Abschnitte 1 bis 7 und 9 vollständig, Abschnitt 8 nur Kernpunkte). Offen: Abschnitte 10 bis 58 (Zeilen 116–707 in den 58-Abschnitt-Zählung).

### Fortschreibung MASTER-MIGRATION-14 (09.11.2026)

| Nr. | Zeilen | Abschnitt | Prüfstufe | Zielstellen (im Wortlaut gelesen) | Ergänzung / Befund |
|---|---|---|---|---|---|
| 8 | 78–87 | Weltzustand und Persistenz | Wortlaut geprüft | `Game/NPCs/21` N-PE-001 bis N-PE-021 | Segmentierung, Absturzsicherheit, Migration, 20–50 MB, PostgreSQL nur Backend, unbegrenztes Journal belegt |
| 10 | 116–128 | Tod und Scheitern (OPEN) | Wortlaut geprüft, **Konflikt** | `Game/Spielvision.md` §14.2; `Game/Kampfsystem/14` Punkte 2–6; `COMBAT-DEC-BW-001` bis `-010` | Richtung belegt (Hybrid, kein GTA-Reset, kein Soulslike, Permadeath optional). **Konflikt:** `Kampfsystem/14` Punkt 4 gibt eine spätere Entwicklerantwort wieder (Rückkehr zum letzten Checkpoint mit minimaler Gesundheit). `Spielvision` §14.2 führt die Schleife als offen, `BW-001` als beschlossen. Nicht aufgelöst. |
| 11 | 129–137 | Schwierigkeit und Konfiguration | Wortlaut geprüft | `Game/Kampfsystem/02` GP-012 bis GP-015, GP-023; `Game/Ermittlung/08` | Ergänzt: Munitionsanzeige binär, ohne Anzeige physische Kontrolle. Anatomie bleibt in allen Stufen gleich (GP-014) belegt |

**StateTree-Statuskonflikt (Aufgabe 3):** Die Regel „StateTree organisiert Zustände, eigenes C++ übernimmt Wahrnehmung, Gedächtnis, Kampf und Sonderregeln“ ist in `PROJECT_DECISIONS` („NPCs and AI“) verankert. Keine spätere Nutzerentscheidung im Changelog 23.08. bis 31.08. oder in den Entscheidungsregistern ersetzt sie. Die Statuskennzeichnung in `Game/Kreaturen/10` §11 ist vereinheitlicht: Technikrahmen 📋, nur die Aufteilung der Teile 💡. `Game/Gameplay-Systeme.md` Z. 208 ist konsistent (📋).

**Stand nach MASTER-MIGRATION-14:** 11 von 58 Abschnitten im Wortlaut geprüft (1–11). Erster offener Abschnitt: **Nummer 12**, Gunplay / Waffen, Zeile 138.

### Fortschreibung MASTER-MIGRATION-15 (09.11.2026)

| Nr. | Zeilen | Abschnitt | Prüfstufe | Zielstellen (im Wortlaut gelesen) | Ergänzung / Befund |
|---|---|---|---|---|---|
| 12 | 138–161 | Gunplay und Waffen | Wortlaut geprüft | `Game/Kampfsystem/05` FK-001 bis FK-009, FK-012; `Game/Kampfsystem/09` WA-001 bis WA-007; `Game/Kampfsystem/10` MU-006 bis MU-022 | Magazinzustände, Patronenlager, Notnachladen, Röhrenmagazin, Störungen, Wartung, Trageort belegt. Ohne Ergänzung |

**Stand:** 12 von 58 Abschnitten im Wortlaut geprüft (Abschnitte 1–12). Nächster Abschnitt: **13 („Ballistics“, Zeile 162)**.

### Fortschreibung MASTER-MIGRATION-17 (09.11.2026)

| Nr. | Zeilen | Abschnitt | Prüfstufe | Zielstellen (im Wortlaut gelesen) | Ergänzung / Befund |
|---|---|---|---|---|---|
| 13 | 162–194 | Ballistik | Wortlaut geprüft | `Game/Kampfsystem/05` FK-013 bis FK-028, Abschnitte 4.1 bis 4.4, 7 | Alle 28 Regelgruppen belegt: physikalische Flugbahn ohne künstliche Maximalreichweite, Projektildefinition, Durchdringung nach Material und Schichten, Fahrzeug- und Glasstrukturen, Abpraller nach Grounded-Voreinstellung, kosmetische Splitter, Wind, Vorhalten, Höhe, Winkel, Visierhöhe, Cant, Transschall, Kein Coriolis, Anbauteile, Nullpunkt, Schalldämpfer, Reichweiten 100 m und 500–1000 m, Schießstand und Training, Entfernungsmesser, Kill-Cam. Ohne Ergänzung |

**Stand:** 13 von 58 Abschnitten im Wortlaut geprüft (Abschnitte 1–13). Nächster Abschnitt: **14 („Melee / grappling“, Zeile 195)**.
| 14 | 195–218 | Nahkampf und Ringen | Wortlaut geprüft | `Game/Kampfsystem/04` COMBAT-DEC-NK-001 bis NK-049; `Game/Kampfsystem/07` DF-001, DF-006; `Game/Kampfsystem/02` GP-017 und Tabelle Zeilen 165–168 (Soulslike, UFC-Klon, universeller Block) | Tiefe, Clinch, Takedowns, Scrambles, Aufgabegriffe mit Abklopfen oder Bewusstlosigkeit, Fight-Club-Ehrenkodex, Monster nach Anatomie, Finisher nur bei Handlungsunfähigkeit, kontextabhängige Steuerung belegt. Ohne Ergänzung |
| 15 | 219–232 | Fahrzeuge | Wortlaut geprüft, **Konflikt (Vorrang beachtet)** | `Game/Fahrzeuge-und-Verkehr/04` VEH-DEC-PH-001, PH-006; `05` VEH-DEC-ST-001 bis ST-006; `Game/Gameplay-Systeme.md` §6.3; `Game/Fahrzeuge-und-Verkehr/02` GP-001; `Game/World/20` W-GE-026 | Manuelles Getriebe, freie Gangwahl (auch 5→2), Abwürgen, Motorbremse, Kofferraum als mobile Basis, Fahrzeugwahl und Kalibrierung der Kartengröße belegt. **Konflikt:** Die ältere Xbox-artige Belegung (RT, LT, A, X, B, Y, LB) ist durch die spätere DualSense-Belegung vom 15.10.2026 präzisiert, nicht aufgehoben (VEH-DEC-ST-004). Keine stille Ersetzung. Ohne weitere Ergänzung |
| 16 | 233–237 | Fortschritt (OFFEN) | Wortlaut geprüft | `Game/Spielvision.md` Zeile 323; `Game/Kampfsystem/11` COMBAT-DEC-AU-005 | Wachstum durch Nutzung und Training, starke abnehmende Erträge gegen Ausnutzung, Gesamtdesign offen. Ohne Ergänzung |
| 17 | 237–245 | Modding | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §15 (Ebenen, Lua optional, Least-Privilege, Browser, Prüfung und Signatur, Sicherheitsgrenze) | **Ergänzt:** „Modding ist ausdrücklich unterstützt und soll kein Reverse Engineering verlangen“ fehlte und wurde in §15 als 📋 aufgenommen. Übrige Punkte belegt |

| 18 | 246–259 | Offizielle Wissensplattform | Wortlaut geprüft | `Game/Spielvision.md` §16 (Spoilerstufen, KI-Assistent über offizielle Schnittstelle, Freischaltcode, DLC-Besitz, offline); `Game/Ermittlung/08` und `20` E-MO-014 | **Ergänzt:** „Nicht-Besitzer sehen nur öffentliche, spoilerfreie Produktseiten, Trailer und Funktionsbeschreibungen“ in `Spielvision.md` §16. Preis offen belassen |
| 19 | 260–267 | Tests, Werkzeuge, Qualität | Wortlaut geprüft | `Game/Gameplay-Systeme.md` Zeile 381 (Testarchitektur), Zeile 202 (KI-Inspektor); `Development/Produktions-Referenzen-und-Prompts.md` §12 | **Ergänzt:** Entwicklerwerkzeuge (Fall-Debugger, Weltzustands-Inspektor, Performance- und Spawn-Werkzeuge) und Pflichttests in §12 als 📋 Richtung; Werkzeugumfang offen |
| 20 | 268–278 | Prompt-Bibliothek, Governance | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §12 | **Ergänzt:** Pflichtfelder eines Umsetzungsprompts, Vollständigkeit vor Anzahl, Geltungsbereich, Entwurfsstatus des Masters. Keine Freigabe des Prompt-Systems |
| 21 | 279–292 | Mehrgegner-Kampf, Improvisation, Kapitulation | Wortlaut geprüft | `Game/Kampfsystem/04` COMBAT-DEC-NK-018 bis NK-024; `NK-019` (keine dauerhafte Betäubungs-Sperre) | Belegt. Ohne Ergänzung |

| 22 | 293–328 | Tarnung, Wahrnehmung, Horror-KI | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §7 (Wahrnehmung mit Unsicherheit, Gehör, Sicht, Taktikvorrat, Zusammenspiel, Anzeigen), `Game/Kreaturen/10` Zeile 41 (drei Geräuscharten) und 66 (Licht, Raytracing) | **Ergänzt:** gezieltes Prüfen bei gesehenem oder belegtem Versteckverhalten, sonst nur Verdacht (als 📋 in §7). Übrige Punkte belegt, auch Türen (Zeile 433), Schwarmverbot und artspezifische Kommunikation |

| 23 | 329–340 | Begleiter, Planung, Befehle | Wortlaut geprüft | `Game/Spielvision.md` §6.2 (Zwei-Jäger-Kern, Zeile 166), §6.3 (Planung, Befehle, Abweichung, Fehler ohne Sabotage, Eigenfeuer, Hunde); `Game/NPCs/19`, `26` N-CO-002 | Belegt. Ohne Ergänzung |
| 24 | 341–357 | Audio, Raumakustik, Qualität | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §Audio (Zeilen 334–335); `Development/Produktions-Referenzen-und-Prompts.md` §9 (Masterformate, Bitraten als Planungsbereiche, Blindtests, Horrorgeräusche, Raum-/Portalmodell, Streaming, QA) | Belegt. Bitraten bleiben Planungsbereiche ohne Codec-Zusage. Ohne Ergänzung |

| 25 | 358–378 | Rendering, Grafikeinstellungen, Plattformziele | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §Rendering (4K primär, 8K optional, PS5 30/60, kein 40-FPS-Modus, Xbox und Series S, Konsolen-Anzeigeoptionen ohne CVars, Budgets, offene Profilierung); Zeile 327 | **Ergänzt:** Path Tracing als optionaler High-End-Pfad und Upscaling (TSR, DLSS, FSR, XeSS) für preiswertere Hardware, Zielgarantien nur nach Profilierung. Zahlenwerte bleiben offen |
| 26 | 379–388 | Testphilosophie | Wortlaut geprüft | `Game/Gameplay-Systeme.md` Zeile 383 (Testarchitektur, Risikosysteme, Seeds und Replays, Regressionen, manuelle QA); Zeile 202 (KI-Inspektor) | Belegt. Ohne Ergänzung |

| 27 | 389–406 | Prompt-Bibliothek, Scrum, Umsetzungsorchestrierung | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §7 (Track 23, Scrum-Felder nicht übernommen), §11 (wasserfallartige Vorplanung, danach Scrum-Sprints), §12 (Governance) | **Ergänzt** in §12: einmalige Definition kanonischer Information, Abhängigkeitsgraph als Reihenfolge, Definition of Done mit Pflichttests, Ausführungs-Prompt je Arbeitspaket nach Planung. Kein Widerspruch: Scrum als Prozess ist in §11 belegt; die Track-23-Felder bleiben außerhalb von `Docs/` |
| 28 | 407–413 | Genre und Fortschrittsidentität | Wortlaut geprüft | `Game/Spielvision.md` Zeile 57 (vermiedene Level, Rarität, Skalierung, Schadenszahlen); `Game/Kampfsystem/02` COMBAT-DEC-GP-003, GP-004; `Game/Kreaturen/02` (Tabelle Zeile 50) | Belegt. Fortschritt bleibt OFFEN, wie in der Quelle. Ohne Ergänzung |

| 29 | 414–442 | Kreaturen- und Übernatürlichen-Rahmen | Wortlaut geprüft | `Game/Kreaturen/03` (Dimensionen, Existenzform und Herkunft getrennt, Intelligenz, Antriebe, Temperament, Alter ohne Stärkeverdopplung, Alpha ohne Elite-Klasse, handgeschriebene Arten, Archiv Art/Individuum); `04` (Wirkungstypen der Schwäche, abgestufte Resistenz, Anatomie, Bein-Regel, Regeneration in Teilregeln, Narben); `08` (Sozialstrukturen, Entstehung, Lebensphasen, Aktivitätsbedingungen, Jungwesen); `09`, `10` (Gedächtnis nur plausibel beobachtet) | Belegt. Ohne Ergänzung |

| 30 | 443–450 | Organische Weltereignisse | Wortlaut geprüft | `Game/Ermittlung/07` Zeile 108 (Zeit-, Wetter-, Mond-Fälle ohne MMO-Spawn, schwache Signale); `Game/Ermittlung/09` Signal, Lead, L6 (keine Marker); `Game/Spielvision.md` Zeile 230 (Folgen bei Ignorieren); `Game/Kreaturen/15` Zeile 28 | Belegt. Ohne Ergänzung |
| 31 | 451–456 | Friedliche Wesen, eigenständige Jäger | Wortlaut geprüft | `Game/Kreaturen/09` §3 (Verschonen, kein zwingender Feind, Handlung anderer Jäger); `Game/Kreaturen/11` Zeile 42 | Belegt. Ohne Ergänzung |
| 32 | 457–465 | Epistemologie des Hunter-Archivs | Wortlaut geprüft | `Game/Ermittlung/04` Zeile 44 (entdecktes Wissen, kein Debugger); `Game/Kreaturen/14` Zeilen 33, 36 (interne Daten nicht gezeigt, Begleiter nicht allwissend); `Game/Spielvision.md` Zeile 241 (Wissensstatus) | Belegt. Ohne Ergänzung |
| 33 | 466–471 | Langfristige Story-Bedrohungen | Wortlaut geprüft | `Game/Spielvision.md` Zeile 114 (langlaufende Handlungsstränge, nicht Missionsgegner); `Game/Kreaturen/03` Zeile 160 | Belegt. Ohne Ergänzung |

| 34 | 472–488 | Dialogarchitektur | Wortlaut geprüft | `Game/Spielvision.md` §15.1 (Dialog-ID, Gesprächsarten, Unterbrechung, Gehen und Fahren, Roadtrip, Rang, NPC untereinander); `Development/Produktions-Referenzen-und-Prompts.md` §10 (Dialog-ID mit Revision, Stimme versioniert, Stimmenbibel) | **Ergänzt** in §10: Barks für wichtige Figuren, Laden nur relevanter Blöcke, Schreiben und Spieltest vor der Aufnahme |
| 35 | 489–496 | Dialogumfang und Skalierung | Wortlaut geprüft | `Game/Spielvision.md` §15.1 Zeile 403 (Hunderttausende Zeilen, 10.000–50.000 für Nebenfiguren, Millionen Datensätze; Zahlen offen) | Belegt. Veraltete Assets erkennbar (`Produktions-Referenzen` §10, Zeile 1). Ohne weitere Ergänzung |
| 36 | 497–515 | Stimmenstrategie | Wortlaut geprüft | `Game/Spielvision.md` §15.2 (Platzhalterstimmen, Klon und Sprache-zu-Sprache, Zustimmung, Rechte, keine Nachahmung bestehender Schauspieler, Vorabversionen mit ausgewiesenen synthetischen Stimmen, Anbieter offen); `Produktions-Referenzen` §10 (keine Modelle im Spiel) | Belegt. **Teilweise:** Die Quelle sagt, Klon-TTS sei für den Umfang gegenüber Sprache-zu-Sprache vorzuziehen; die Docs nennen beides als Mix. Keine Entscheidung getroffen, die Gewichtung bleibt offen |

| 37 | 516–525 | Stimme, Gesicht, Körper | Wortlaut geprüft | `Game/Spielvision.md` §15.1 bis 15.3 (Gesicht und Körper, Automatisierung und Handarbeit); `Produktions-Referenzen` §10 (Signalprofile, Lippensynchronisation aus finaler Aufnahme) | Belegt. Ohne Ergänzung |
| 38 | 526–535 | Prompt-/Spec-Explorer vor dem Codieren | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §10 (Dokumentbaum als Wahrheit, Markdown, Technik, Skalierung, Reihenfolge) | **Ergänzt:** Mindestfunktionen und erwünschte Funktionen als Liste in §10. Keine Festlegung der Technik |
| 39 | 536–547 | Visuelle Referenzen | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §10 (Visual-ID, verbindlich gegen Stimmung, Versionen, Filter); Zeile 133 (generierte Konzeptbilder nie automatisch übernommen) | Belegt. Ohne Ergänzung |

| 40 | 548–565 | Blender und Umgebungsbau | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §11 (MESSEN und LOOK, Ablauf Grundriss bis Optimierung, Greybox, Modulspezifikation, Teilaufgaben, Lichtzustände, Iterationsschleife, Video als Anregung) | **Ergänzt:** `bpy`- oder MCP-Steuerung nur mit Validierung, `.blend` allein nie fertig. Übrige Punkte belegt |
| 41 | 566–583 | Claude-Ausführung und Laufzeittests | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §11 (Standardablauf, gestartete Sitzung, deterministische Testkarten, innere KI-Zustände, Seed und Replay, SPN Test Orchestrator, keine erfundene Designentscheidung) | **Ergänzt:** Testpawns und Bots, Gebäudetests, Screenshot-Tests als maschinelle Ergänzung |
| 42 | 584–595 | Menschliche Freigabe | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §11 (Freigabe, Zustände, `TECHNICALLY_VALIDATED` ≠ `APPROVED`); §10 Explorer | **Ergänzt:** Aktionen Freigeben, Änderungen anfordern, Ablehnen im Explorer (§11) |
| 43 | 596–604 | Vorproduktion und Umsetzungsreihenfolge | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §11 (wasserfallartig, dann Scrum; Reihenfolge noch nicht festgelegt; Kette über DAG); §10 (Explorer vor dem Sprint) | Belegt. Ohne Ergänzung |
| 44 | 605–616 | Fortschrittsberichte | Wortlaut geprüft | `Development/Produktions-Referenzen-und-Prompts.md` §11 (Fortschrittsmessung: zwei getrennte Näherungswerte, nur bei echter Arbeit steigend, Prompt zählt erst nach Qualitätsgate) | Belegt. Ohne Ergänzung |

| 45 | 617–622 | Protagonist, Crew, Hauptquartier | Wortlaut geprüft | `Game/Spielvision.md` §6 (frei erstellbarer Hauptcharakter, Hunter-Crew, Feldbasen, HQ als Lebensmittelpunkt); `Game/Entscheidungsstatus.md` Zeile 80 | Belegt. Ohne Ergänzung |
| 46 | 623–629 | Akte, Kapitel, Story-Reaktivität | Wortlaut geprüft | `Game/Spielvision.md` Zeilen 113–118 (Akte, Spine, Hauptmissionen nie verpassbar, Nebenmissionen normalerweise nicht); `Game/NPCs/19` N-CO-017 (persönliche Anfragen entfallen bei Tod oder dauerhaftem Weggang) | Belegt. Ohne Ergänzung |
| 47 | 630–635 | Missionsbewertungen und Trophäen | Wortlaut geprüft, **Statusdifferenz** | `Game/Spielvision.md` §12.1–12.3 (fünf Stufen, verborgene Kriterien, 3 bis 20+ Kriterien, alternative Wege, Platin offline); `Game/Entscheidungsstatus.md` Zeile 178 | **Differenz:** Die Quelle nennt getrennte Trophäengruppen für Online, Grounded, Permadeath und DLC als Entscheidung. `Entscheidungsstatus` führt diese Gruppen als **offen**. Nicht aufgelöst; Entwicklerfrage. |
| 48 | 636–641 | Mehrspieler-Trennung | Wortlaut geprüft | `Game/Spielvision.md` Modustabelle (Einzelspieler, Koop 2–4 ohne Transfer, Offizielles Online ohne Mods und Cheats), Zeile 360 (64–100 Spieler als Ziel) | Belegt. Die Zahl der Online-Slots bleibt offen (Quelle und Docs gleich) |
| 49 | 642–647 | Online-Wirtschaft, Creator | Wortlaut geprüft | `Game/Spielvision.md` Zeilen 366–372 (kein Pay-to-Win, optionale Echtgeld-Beschleunigung diskutiert, Kreativmarktplatz als Wunschrichtung, kein freies Auszahlen) | Belegt. Die Creator-Ideen stehen als 🧭 Wunsch; Aufteilung, Preise und Auszahlung bleiben offen |
| 50 | 648–652 | Optionaler PC-Inhalt, Interaktion | Wortlaut geprüft | `Game/Adult/02` Zeile 41 (nicht im Basis-Build und nicht in Konsolen-Cooks); `Game/Adult/17` Zeilen 138–159 (PC-Paket, generische Mod-Schnittstellen); `Game/Adult/14` E-SK-019 (Stopp-Signal beendet sofort) | Belegt über die Adult-Module. Ohne Ergänzung im Hauptdokument |

| 51 | 653–658 | Körper, Rückstoß, Combat Focus | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §9 (Anpassungsmodell, Morphing), Zeile 114 (Rückstoß physisch, Combat Focus statt Zeitlupe) | **Ergänzt:** gemeinsame Sekundärphysik für Bewegung, Rückstoß, Fahrzeuge und Kleidung in §9 (📋) |
| 52 | 659–668 | Wildtiere, Studium, Ernte | Wortlaut geprüft | `Game/Spielwelt-und-Untergrund.md` §9 (Studier-Aktion, Verwesung, Aasfresser, Kadaver, Materialqualität ohne Sterne); `Game/Entscheidungsstatus.md` (220–280 Tierarten als Zielbereich); `Game/Kreaturen/14` (Forschung wie bei Tieren) | Belegt. Ohne Ergänzung |
| 53 | 669–679 | Fahrzeuge, Rennen | Wortlaut geprüft | `Game/Fahrzeuge-und-Verkehr/04` (Schaltgetriebe mit Kupplung und Abwürgen als Standard, Fahrdynamik); `Game/Gameplay-Systeme.md` §6 (DCT, sequenziell, Rallye als Varianten, Gelände), Zeile 177 (Abschleppen, Seilwinde); `Fahrzeuge/09` (Energie und Laden); Zeile 150 (Fahrzeugbasis) | Belegt. Die fünf Rennstrecken: `Game/World/15` Zeile 52 (mindestens fünf) |
| 54 | 680–688 | Bargeld, Recht, Beweise | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §10 (Bargeld, Beweise, Festnahme, K9, Pfandhäuser, fallende Waffen als Beweis); `Game/Ermittlung/14` (Festnahme statt Tod) | Belegt. Ohne Ergänzung |
| 55 | 689–697 | Smartphone, soziale Medien, Weltereignisse | Wortlaut geprüft | `Game/Gameplay-Systeme.md` §11 (diegetisch, endloser Feed mit Fake-Inhalten, NPC-Kommunikation, digitale Beweise); `Game/Ermittlung/26` N21 (Weltereignisse keine Nebenmissionen); Zeile 150 (Begleitgespräche) | Belegt. Ohne Ergänzung |
| 56 | 698–701 | Gore und Altersdarstellung | Wortlaut geprüft | `Game/Gameplay-Systeme.md` Zeile 91 (Erwachsenen-Gore) und Minderjährigen-Eintrag (keine Belohnung, keine Unverwundbarkeit, Prüfung ausstehend); `Game/Entscheidungsstatus.md` Zeile 168 | Belegt. Die endgültigen Grenzen bleiben offen (Plattform-, Rechts- und Altersprüfung) |
| 57 | 702–705 | Physische Veröffentlichung, Größe | Wortlaut geprüft | `Game/Spielvision.md` Vertrieb (Disc als Wunsch, Installation ohne Konto, rund 100 GB); `Game/Entscheidungsstatus.md` (Basisspiel-Größe) | Belegt. Ohne Ergänzung |
| 58 | 706–707 | Fortschrittsangabe (Stand 'Reset') | Wortlaut geprüft, **nicht übernommen** | Kein Zielabschnitt. Die Quelle nennt ca. 22–23 % Gesamtplanung und 2–3 % Claude-Produktionsplan als groben Stand | **Nicht als Prozentwert übernommen:** keine nachvollziehbare Berechnungsgrundlage in der Quelle; die Docs führen Fortschritt nur als Zählwerte (`master-migrationsregister.md`) |

**Stand:** 58 von 58 Abschnitten im Wortlaut geprüft (Abschnitte 1–58). Die Quelle ist damit durchgegangen. **Offene Konflikte bleiben dokumentiert:** Checkpoint-Schleife nach Tod (Abschnitt 10), Trophäengruppen (Abschnitt 47: Quelle entschieden, `Entscheidungsstatus` offen), Creator-Wirtschaft (Abschnitt 49: Wunschrichtung, Details offen), Xbox- und DualSense-Belegung (Abschnitt 15, Vorrang beachtet).
