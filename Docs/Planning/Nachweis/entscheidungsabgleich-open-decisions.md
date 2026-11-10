# Entscheidungsabgleich: OPEN_DECISIONS.md (MASTER-MIGRATION-17, 09.11.2026)

**Quelle (nur lesend):** `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/OPEN_DECISIONS.md` (Stand 31.08.2026, 124 Punkte in 17 Gruppen; Zählung aus der Datei: 9+6+8+8+6+7+6+7+5+8+9+5+10+10+9+7+4 = 124).

**Prüfregel:** Jeder Punkt ist ein Platzhalter „Exact ... / Final ...“ und darf nicht als entschieden erscheinen. Geprüft wird, ob der Punkt in `Docs/` **offen** geführt wird (Zielstelle gelesen) oder als Frage in `Planning/OPEN_QUESTIONS.md` steht (Q-OD-Nummer). Ein Stichworttreffer allein gilt nicht als Nachweis; für jede Gruppe ist die Zielstelle genannt.

**Zielstellen:**
- `Game/Entscheidungsstatus.md` §5.1 bis §5.10 („Offene Entscheidungen nach Themen“)
- `Planning/OPEN_QUESTIONS.md`, Abschnitt mit Q-OD-01 bis Q-OD-53
- Fachmodule (Zeilen je Gruppe unten)

## Ergebnis je Gruppe

| Gruppe | Punkte | Offen geführt in | Q-OD | Befund |
|---|---:|---|---|---|
| Produkt, Welt, Story | 9 | `Entscheidungsstatus` §5.1 (Titel, Fläche, Akte, Crew, Nebenfälle, HQ, Tod, Kalender) | Q-OD-01, Q-OD-02 (historisch, N-01) | vollständig offen |
| Fortschritt, Körper, Überleben | 6 | §5.3; Kalender §5.1 | Q-OD-03 bis Q-OD-06 | vollständig offen |
| Tiere, Jagd, Monster | 8 | §5.4 (Artenliste, Studium, Individuen, Wirtschaft, Alterung, Taxonomie, Hypothesen) | Q-OD-07 | vollständig offen |
| Fahrzeuge, Rennen | 8 | §5.5; `Fahrzeuge-und-Verkehr/04` W-FZ-014 (Obergrenze der Vertrautheit offen) | Q-OD-08, Q-OD-09, **Q-OD-53** (neu: Abschleppen, Winde, Bergung) | eine Lücke geschlossen |
| Kampf, Verletzung, Gore | 6 | §5.6 (Combat Focus, Parameter, Gore-Schwellen, Minderjährige, Nahkampftiefe) | — | vollständig offen |
| Recht, Beweise | 7 | §5.7 (Katalog, Festnahme, Zuständigkeit, Beweismodelle, CCTV, K9, Durchsuchung, Polizeikarte) | — | vollständig offen |
| Wirtschaft, Bargeld, Läden | 6 | §5.7 (Balance, Bargeld, Lager, Pfand, Crafting, Preise); §5.8 (Echtgeld, Schutz vor Pay-to-Win) | Q-OD-10, Q-OD-11 | vollständig offen |
| Smartphone, Medien, Weltereignisse | 7 | §5.7 (Netzwerk, Generator, Verhältnis handgeschrieben/generiert, Beweise, Apps); `NPCs/04` §7 (Kommunikationsstil, Entwurf) | — | vollständig offen |
| Medaillen, Erfolge | 5 | §5.8 (Gewichtung, Anzeige, Trophäenzahl, Gruppen, Platin) | Q-OD-12, Q-OD-13 | **Konflikt:** die Quelle nennt die Trophäengruppen als entschieden; `Entscheidungsstatus` führt sie als offen. Nicht aufgelöst |
| Online, Backend | 8 | §5.8 (Slots, Spielerzahl, Backend, Matchmaking, Feindseligkeit, Geschäfte, Sprachchat, Anti-Cheat) | Q-OD-14 | vollständig offen |
| Creator, Modding | 9 | §5.8 (Umsatz, Förderfonds, Creator-Codes, Auszahlung, kuratierter Weg, Hosting, Lua, Signierung) | Q-OD-15 bis Q-OD-17 | vollständig offen (Creator-Ideen als 🧭 Wunsch in `Spielvision` Zeilen 366–372) |
| Adult, Romanze, Interaktion | 5 | §5.10 und `Adult/11` (Taxonomie, Kameraprofile, Nacktheitsstufen, Betriebe, Paketzuschnitt) | Q-OD-18, Q-OD-19 | vollständig offen |
| Rendering, Leistung, Speicher | 10 | §5.9 (Hardware, Auflösung, RT, Path Tracing, 8K, PS5 Pro, Series S, Upscaling-Matrix, Installationsgröße, Sprachpakete) | Q-OD-20 bis Q-OD-26 | vollständig offen |
| Audio, Stimme | 10 | §5.9 (Codecs, Raum-Architektur, Speicher-Budgets, Aufnahme, Dialogzahlen, Datenbank, Besetzung, Klon-Anbieter, Verhältnis, Synchronsprachen) | Q-OD-27 bis Q-OD-33 | vollständig offen |
| Prompt-Explorer, DCC, Produktion | 9 | `Produktions-Referenzen` §10 (Technik „mögliche Technik“, nicht festgelegt) | Q-OD-34 bis Q-OD-37; **Q-OD-42 bis Q-OD-45** (neu: Visual-Formate, Review-Auslösung, Blender-Mechanismus, Austauschformate) | vier Lücken geschlossen |
| Testorchestrierung, Freigabe | 7 | `Produktions-Referenzen` §11 (Freigabe, Zustände) | Q-OD-38 bis Q-OD-40; **Q-OD-46 bis Q-OD-49** (neu: Framework-Mix, CI-Pool, Szenarienzahl, Review-Rollen) | vier Lücken geschlossen |
| Scrum, Produktion | 4 | `Produktions-Referenzen` §11 (Reihenfolge, Fortschrittsmessung) | Q-OD-41; **Q-OD-50 bis Q-OD-52** (neu: Sprintdauer, Zerlegung, Paketzahl) | drei Lücken geschlossen |

## Zusammenfassung

- Insgesamt 124 Punkte geprüft; alle 124 sind offen geführt (in `Entscheidungsstatus` §5, in einem Fachmodul oder als Q-OD).
- **Neu in `OPEN_QUESTIONS.md` aufgenommen:** Q-OD-42 bis Q-OD-53 (12 Fragen), weil diese Punkte in `Docs/` nicht als offen geführt waren.
- Eine Dublette (Kalender und Story-Zeit) wurde vor dem Abschluss entfernt: Sie steht bereits in `Entscheidungsstatus` §5.1.
- **Offener Konflikt, nicht aufgelöst:** Trophäengruppen (Quelle entschieden, `Entscheidungsstatus` offen).
- Die 83 bisher nur automatisch abgeglichenen Punkte sind damit in Gruppen mit Zielstelle geprüft. Es bleibt kein Punkt ohne Zuordnung.

## Verweise

- Register: `master-migrationsregister.md`
- Fragen: `Planning/OPEN_QUESTIONS.md` (Q-OD-42 bis Q-OD-53)
- Keine Änderung an der Quelle; keine Entscheidung getroffen.
