# Nachweis: Adult-Bereich, Anforderungen und Klassifizierung

**Stand:** 08.11.2026
**Bearbeiter:** Claude (Subagent, Modell Haiku 5.5) im Auftrag des Entwicklers. Geschrieben wurde nur diese Datei. Keine Änderungen an `Docs/Game/`, keine Git-Befehle, keine Spielentscheidungen.
**Umfang:** `Docs/Game/Adult/` 01 bis 41 und `README.md` (vollständig gelesen; die großen Matrizen 36 bis 40 und das Register 14 in Abschnitten, jeweils ohne Auslassung). `Docs/Game/Adult-System.md` (vollständig). `Docs/Planning/DECISIONS.md` (S-14), `Docs/Planning/CONFLICTS.md` (K-10, K-23). Gegenprobe: `Docs/Game/Entscheidungsstatus.md` (Z. 92, 190–192), `Docs/Game/Gameplay-Systeme.md` (Z. 220).

**Fundstellen:** `NN:Z` = `Docs/Game/Adult/NN-*.md`, Zeile Z (beim Lesen geprüft). `AS:Z` = `Adult-System.md`. `DEC:Z`, `CON:Z`, `ES:Z`, `GS:Z` = die Projektdateien oben.

**Statusbegriffe:**
- **verbindlich** = Quelle trägt 📋 (Entwicklerbestätigung, *nicht* im Kanon konsolidiert). Das ist keine Kanonisierung.
- **vorläufig** = 🧭 (Vorschlag, Assistentenaussage ohne ausdrückliche Zustimmung oder Kompromiss).
- **offen** = ❓. **widersprüchlich** = Quellen oder Register stehen gegeneinander. **historisch** = 🕰️ (überholt). **verworfen** = ⛔ oder ❌.

**Nummern:** Die internen Konflikte des Adult-Bereichs heißen hier „Adult-K1…K23“ und „Adult-OE-01…86“. Achtung: `CON` K-10 und K-23 meinen **Romantik im Basisspiel**. Adult-K10 (Fragenzahl R15) und Adult-K23 (Reihenfolge der Abschlussschritte) sind andere Themen.

**Grenzen:** Keine Rechtsbewertung. Nichts davon ist implementiert (`01:4`, `AS:4`). Der Matrixstatus in 36 bis 40 ist heuristisch (`41:53`) und wurde nicht neu bewertet.

---

## 0. Befund in Kürze

- Die Detailplanung liegt nur im Interviewprotokoll (Chat 35, Nachrichten 185 bis 812, `13:49–56`). Für die Runden 16 bis 20 gibt es keine konsolidierte Quelle (`13:53`, `41:249`, `34:128`). Die Matrizen zählen 1.903 Fragen (`41:62`, `41:132`).
- Pauschale Zustimmungen wie „passt alles“ sind als 📋 gezählt, aber schwach belegt: `36:383–432` (Runde 9B.3), `40:187–206` (Runde 17E), `39:507` (Missing-Family-Audit). Sie tragen keine Entscheidung.
- Der Sicherheitskern (Alter, Zustimmung, Stopp) ist mehrfach belegt. Die frühen Gegenpositionen des Entwicklers (10. bis 16.09.) sind als historisch markiert (Abschnitt F).
- Die Konflikte mit Projektbezug sind Romantik im Basisspiel (`CON` K-10/K-23, `DEC` S-14) und die Frage „Familie/Kinder“ (Widerspruch zu `34:58`).

---

## A. Allgemeine Beziehungs- und Romantikmechanik

Prüfung gegen `CON` K-10 und K-23 nötig. Nicht entschieden.

| ID | Anforderung | Fundstelle | Status | Blockade / Hinweis |
|---|---|---|---|---|
| ADU-A-001 | Eigenständige Beziehungswerte (Freundschaft, Vertrauen, Respekt, Angst, Sympathie) | 05:27 | verbindlich | Basisspiel-Zuordnung siehe ADU-A-002 |
| ADU-A-002 | Romantische und sexuelle Beziehungen mit erwachsenen NPCs „breit möglich“ im Basisspiel | 05:25–28; 14:320 (E-NP-031); GS:220 | widersprüchlich | `DEC` S-14: Basisspiel „nicht explizit“; `ES:92`. `CON` K-10/K-23 offen. GS:220 führt Romantik als 📋 im Basisspiel |
| ADU-A-003 | Mehrere gleichzeitige Beziehungen, Eifersucht, offene Beziehung, Fremdgehen, Trennung | 05:29, 05:58; 36 (R5 Q10–11) | verbindlich | Teil von ADU-A-002 |
| ADU-A-004 | Zusammenziehen, Heirat, gemeinsamer Besitz | 05:29; 14:321 (E-NP-032) | verbindlich | — |
| ADU-A-005 | Familiengründung und Kinder | 05:29 (niedrige Priorität); 14:321 (❓); 10A Q14 „weiß noch nicht“ (`37:58`); 36 (`9B.3` Q18 „passt alles“) | widersprüchlich | Siehe W-02, Adult-OE-20 |
| ADU-A-006 | Keine sichtbaren Beziehungsbalken; Zustand zeigt sich über Verhalten | 05:31; 36/37 (R10 Q58–59) | verbindlich | — |
| ADU-A-007 | Romantische Pfade verbiegen die Hauptgeschichte nicht; kein Story-Zwang | 05:32; 01:31 | verbindlich | — |
| ADU-A-008 | Präferenzprofil verborgen; Stufen neugierig … harte Grenze; partner- und situationsabhängig | 05:38–47; 21:122 | verbindlich | Zahlenwerte nicht festgelegt |
| ADU-A-009 | Getrennte Werte Attraktion, Vertrauen, Zuneigung, Romantik, Komfort, Vertrautheit; Chemie bidirektional | 05:56; 14:296 (E-NP-007) | verbindlich | — |
| ADU-A-010 | NPCs initiieren, flirten, schlagen Orte vor; spontane Begegnungen ohne Quest-Markierung als Kernziel | 05:77–81; 14:300 | verbindlich | Häufigkeit über Einstellungen steuerbar (05:80) |
| ADU-A-011 | NPC↔NPC-Interaktionen in der Welt; Spieler kann zuschauen oder beitreten | 05:80; 14:146 (E-IA-002) | verbindlich | Beitritt nur mit Zustimmung aller (`08:34`) |
| ADU-A-012 | Präferenz-DNA aus Seed, an globale NPC-ID gekoppelt, nur bei Erzeugung; Archetyp nur als Bias | 05:96–101; 14:301 | verbindlich | Erzeugungs-Richtwerte (500.000 NPCs) sind 🧭 (05:98) |
| ADU-A-013 | Gedächtnis statt Zähler; kein „Sex-Level“; kein Zähler steuert Verhalten | 05:88–89; 14:165 (E-IA-021) | verbindlich | — |
| ADU-A-014 | Partnerwissen kann falsch sein; Kompatibilität entsteht aus vielen Faktoren, nicht aus einem Gesamtwert | 05:90; 14:317 (E-NP-028) | verbindlich | — |
| ADU-A-015 | Ein früherer Widerruf oder eine Grenzverletzung wirkt im Gedächtnis nach | 05:89; 01:151 | verbindlich | — |
| ADU-A-016 | Ein aktuelles Nein bleibt in der Situation Nein; Desinteresse kann sich über Zeit entwickeln | 05:61; 14:298 (E-NP-009, 🕰️ für „Nein ohne Änderungschance“) | verbindlich (Nein bleibt Nein, 05:61 📋); Desinteresse-Entwicklung vorläufig | Die Regel „Nein ohne Änderungschance“ ist verworfen (F-05) |
| ADU-A-017 | Sexuelle und romantische Orientierung getrennt; datengetriebener Zustand | 05:66–68; 14:314 (E-NP-025) | verbindlich | Modellumfang offen (ADU-D-03, W-03) |
| ADU-A-018 | Lebenssimulation: NPCs altern, verändern Tattoos und Erscheinung über Jahre, entfernte NPCs werden vereinfacht weitersimuliert | 05:106–111; 14:319 (E-NP-030), 14:305 | verbindlich | — |
| ADU-A-019 | Privatsphäre und Zeugen werden simuliert; Orte merken sich Vorfälle | 08:23–37; 14:153 (E-IA-009) | verbindlich | Schwellen nicht festgelegt |
| ADU-A-020 | Attraktion wird auf dem tatsächlich erzeugten Körper berechnet | 14:240 (E-KO-005) | verbindlich | — |
| ADU-A-021 | Verabredung führt über Übergang zur Szene; keine Schnitte, die Welt läuft weiter | 03:141; 14:157–158 (E-IA-013/014) | verbindlich | Zeitmaßstab offen (Adult-K15) |

---

## B. Optionale explizite Inhalte, Alters- und Zustimmungsregeln (Adult-Paket)

| ID | Anforderung | Fundstelle | Status | Blockade / Hinweis |
|---|---|---|---|---|
| ADU-B-001 | Basisspiel nicht explizit: Intimität angedeutet (Blocking, Kamera, Audio, Reaktion); Nacktheit konfigurierbar | 02:37; 04:45; 14:109 (E-PA-009) | verbindlich | Darstellungsprofile offen (Adult-OE-61) |
| ADU-B-002 | Explizite Darstellung nur im separaten Paket, nicht in Konsolen-Cooks, kein „Paket installiert“-Check im Kern | 02:40–41; 14:102–104; 28:61 | verbindlich | — |
| ADU-B-003 | Kein globaler Intensitätsschalter; Paket an/aus; Hardcore = Preset über mehrere Dimensionen | 02:67; 14:111–112 | verbindlich | — |
| ADU-B-004 | Inhaltsprofile Standard, Fortgeschritten, Hochintensität; Zuschnitt, Gesamtgröße, Presets | 03:70–79; 02:68; 14:122 | offen | Adult-OE-49, 50 |
| ADU-B-005 | Mindestalter 18 Jahre 0 Tage; unbekanntes Alter = gesperrt; Prüfung läuft vor allem anderen | 01:33, 01:41–44; 14:64–65 | verbindlich | Build-Validator vorläufig (ADU-B-009) |
| ADU-B-006 | Altersnachweis in der Welt; gespeichert wird nur der Status „verifiziert“, kein Ausweisinhalt | 01:44; 14:66 (verbindlich), 14:67 (E-SK-012, vorläufig) | verbindlich / vorläufig | Speicherregel vorläufig |
| ADU-B-007 | Alter aus Geburtsdatum, nie aus Aussehen, Körper oder Stimme; Alter und Vorlieben getrennt | 01:43, 01:46; 14:68 | verbindlich | — |
| ADU-B-008 | Unter 18 vollständig ausgeschlossen (kein Menüeintrag, keine Anzeige, kein Zugang) | 01:42; 14:91 (E-SK-036, Kinder als Weltbewohner, außerhalb Adult) | verbindlich | Kinder-Darstellung gehört nicht zu Adult (Verweis, W-10) |
| ADU-B-009 | Altersregel als Build-Validator; sehr junge Erwachsene müssen visuell eindeutig erwachsen wirken | 01:51–52; 14:86 (E-SK-031) | vorläufig | Rechtlich begründet, ungeprüft (`01:49`) |
| ADU-B-010 | Fail closed: bei Unwissen, Fehler oder Timeout nicht starten bzw. sicher beenden | 01:33 (Regel 6); 20:32 | verbindlich | — |
| ADU-B-011 | Zustimmung ist kein Wahrheitswert, sondern ein Datensatz mit Geltungsbereich | 01:60–72; 14:56 (E-SK-001); 14:82 (E-SK-027) | verbindlich (Prinzip) / vorläufig (Scope) | Dauerrahmen offen (Adult-OE-05) |
| ADU-B-012 | Keine Dauer-Popups; Zustimmung gilt in der laufenden Situation, nicht am nächsten Tag | 01:81; 14:80 (E-SK-025, vorläufig) | vorläufig | Dialogwerte offen |
| ADU-B-013 | Entscheidungsfähigkeit (Capacity Gate): leichte bis mittlere Beeinträchtigung zustimmungsfähig; Bewusstlosigkeit und offensichtliche Entscheidungsunfähigkeit beenden | 01:117–118; 14:70 (E-SK-015), 14:71 (E-SK-016, vorläufig) | verbindlich | Schwellen offen (Adult-OE-04) |
| ADU-B-014 | Bewusstlosigkeit wird kurz verzögert erkannt, dann sofortiger diegetischer Abbruch | 01:119–120; 14:72–73 | verbindlich | Zeitwert offen (Adult-OE-77, AK-ZU-9 „festzulegen“, 30:38) |
| ADU-B-015 | Schweigen, fehlende Ablehnung oder Stöhnen ist keine Zustimmung; Sitzungsrahmen mit Mehrsignal-Auswertung | 01:100–102; 14:77 (E-SK-022) | verbindlich | Gewichtung offen (Adult-K1, OE-02) |
| ADU-B-016 | Stopp-Signal beendet sofort; keine Neuinterpretation; weiche Signale sind kein Rückzug | 01:148–151; 14:74–76 | verbindlich | Ablaufdetails 🧭 (20:234–242) |
| ADU-B-017 | Widerruf jederzeit; danach sicherer Übergang, Ausstieg, Aufräumen | 01:150; 20:236–241 | verbindlich / vorläufig | — |
| ADU-B-018 | Beziehung, Story, KI-Ziele, Gruppen-, Admin- oder Party-Rechte, Preis und Vertrag ersetzen Zustimmung nie | 01:31, 01:56; 14:58 (E-SK-003) | verbindlich | — |
| ADU-B-019 | Erregung, gelernte Präferenz und Schweigen sind keine Zustimmung | 14:85 (E-SK-030); 20:176–182 | vorläufig | — |
| ADU-B-020 | Versehentlicher Kontakt oder eine Handlung außerhalb des Rahmens ist keine Zustimmung zur neuen Handlung | 14:89 (E-SK-034); 20:213 (ZU-LS-05) | vorläufig | — |
| ADU-B-021 | Kein stellvertretendes Einverständnis durch Gruppe oder Server-Admin | 01:32; 02:145 | verbindlich | — |
| ADU-B-022 | Jeder Beteiligte stimmt einzeln zu; keine Mehrheits- oder Gruppenentscheidung | 02:142–145 | vorläufig | Netzwerkteil 🧭 (02:142); Gäste offen (Adult-OE-06) |
| ADU-B-023 | Atem- und Halskontrolle bis Bewusstlosigkeit: Gesundheitssystem aktiv, keine Anleitung, keine „optimale“ Mechanik | 01:122; 06:80; 14:278 (E-GE-008) | vorläufig | Grundsatz 📋 (`01:122`), Ausgestaltung 🧭. Sicherheitsrelevant (Adult-K2-Umfeld) |
| ADU-B-024 | Intensität und Schmerz können einvernehmlich gewollt sein; das ist keine Zustimmungsrücknahme | 01:149; 14:75 (E-SK-020) | verbindlich | Welche Inhalte: ADU-E-09 |
| ADU-B-025 | Körperliche Grenzreaktionen (Würgen, Erbrechen, Tränen) als physiologische Zustände; starke Darstellung als Content-Toggle | 23:160–171; 14:255 (E-KO-020); 14:421 (E-ST-011) | verbindlich (Toggle) | Detailtiefe offen (Adult-OE-60) |
| ADU-B-026 | Verletzungssystem bleibt aktiv bei gewolltem Risiko; keine Obergrenze außer physikalisch; die Figur entscheidet | 06:77–80; 14:271 (E-GE-001), 14:276 (E-GE-006) | verbindlich | Heilungs- und Risikozahlen offen (06:82) |
| ADU-B-027 | Intime Anatomie: kontinuierliche Parameter, Erweiterung im selben Editor, nur für Erwachsene mit Paket | 07:37–39; 14:250 (E-KO-015) | verbindlich | Parameterlisten offen (07:113) |
| ADU-B-028 | Tattoo-Editor mit Nacktheit im Hauptspiel, nicht im Paket | 07:46; 17:80; 14:362 (E-TA-015) | verbindlich | Plattform-Deskriptoren offen (Adult-K17, OE-34) |
| ADU-B-029 | Sexuelle Handlungen mit Bewusstlosen oder Schlafenden als aktive Teilnehmer sind ausgeschlossen | AS:124; 09:76 | verbindlich | Einvernehmlicher Schlafzustand offen (ADU-E-04) |
| ADU-B-030 | Dienstleistung: nicht jede Person bietet jede Leistung; „bezahlt ≠ alles erlaubt“; harte Grenzen bleiben | 08:66–70; 26:103–108; 24:210–214 | verbindlich | Preise und Vertragsmodell offen (OE-18, OE-75) |
| ADU-B-031 | Dienstleistung ist Vertragskontext, keine eigene Handlungskategorie | 03:155; 14:382–383 (E-WE-001/002) | verbindlich | — |
| ADU-B-032 | Langzeit-Begleitvertrag: freie NPC-Person, kann kündigen, kein Eigentum, kein Verkauf | 08:81–91; 26:113; 14:313 (E-NP-024) | vorläufig | Kündigungsmodell offen (Adult-K6, OE-17); Preisbeispiel 250 EUR nicht beschlossen (26:175) |

---

## C. Produktion, Recht, Vertrieb, Plattformen

| ID | Anforderung | Fundstelle | Status | Blockade / Hinweis |
|---|---|---|---|---|
| ADU-C-001 | Ziel: öffentliche Veröffentlichung des Pakets; privat nur als Rückfallplan | 10:35; 14:474 (E-PR-013); 40:411 | verbindlich | Frühere Aussage „kein Pflichtziel“ (40:411) ist mit der Spätaussage zu prüfen (W-08) |
| ADU-C-002 | Vertriebsweg, Plattform- und Altersfreigabe nicht entschieden | 10:37; 14:108 (E-PA-008) | offen | Adult-OE-31, 38 |
| ADU-C-003 | Rechtslage Österreich und Deutschland (Pornographiegesetz, §207a, Jugendmedienschutz) | 10:50–52; 17:52–53 | offen | Ungeprüft (`10:4`, `17:4`). Rechtsberatung nötig (Adult-OE-32) |
| ADU-C-004 | Dreifach-Tor: Gesetz ∩ Plattformregel ∩ Zahlungsdienstleister | 10:49; 14:492 (E-RC-004) | offen | Ungeprüft (🧭) |
| ADU-C-005 | Deklaration unerreichbarer Adult-Inhalte im Build | 10:48; 17:51 | offen | Ungeprüft; Plattformpolitik offen |
| ADU-C-006 | Eigener Entwickler-Mod darf nicht als Fan-Mod getarnt werden | 17:139–143; 14:105 (E-PA-005, 🧭) | verworfen (Täuschung) | Siehe F-03 (E-RC-003, ⛔) |
| ADU-C-007 | Keine Live-generative KI im fertigen Spiel; vorab erzeugte Assets sind zulässig | 08:99; 10:63; 14:496 (E-RC-008) | verbindlich | Herkunftsregister je Asset 🧭 (14:497) |
| ADU-C-008 | KI-Nutzung: Wunsch „eher geheimhalten“ gegen wahrheitsgemäße Deklaration | 17:65; 14:495 (E-RC-007) | widersprüchlich | Adult-K7, OE-33 |
| ADU-C-009 | Unzensiertes lokales Modell („Qwen“) als Lückenfinder und Katalog- oder Testdaten-Helfer | 10:64; 33:152 („Qwen“); 14:453 (E-SP-009, 🧭); 39:243 | offen | Keine Ergebnisse im Master (`13:52`); **NICHT GENEHMIGT** (Entwickler: „jetzt nicht“, `40:66`). Herkunftsfrage für explizite Inhalte (Adult-OE-81) |
| ADU-C-010 | Extremistische Symbolik auf Tattoos | 17:85; 19 (K16); 14:499 (E-RC-011, 🧭) | offen | Rechtlich ungeprüft; **NICHT GENEHMIGT** als Anforderung (Adult-OE-22); Wunsch nicht übernommen |
| ADU-C-011 | Marken und IP: Titel „Supernatural“ geschützt, reale Fahrzeugnamen vermeiden | 10:55; 14:493 (E-RC-005, 🧭) | offen | Projektthema außerhalb Adult |
| ADU-C-012 | Gore-/Altersprofil: Erwachsene und Monster stark, Kinder ohne expliziten Gore | 14:91–92 (E-SK-036/037); 17:143 (`1060`) | verbindlich (außerhalb Adult) | Entwickler: „aus Plattformgründen“; Widerspruch W-10 |
| ADU-C-013 | Streamer-Filter und Namen der Nacktheitsstufen | 14:422 (E-ST-012); 34:71 (OE-35) | offen | Plattformregeln |
| ADU-C-014 | Produktionsumfang V1 klein: Basisinteraktionen, wenige Positionen, grundlegende Reaktionen, eine Testlocation | 39:R16R (`39:626`); 40:402; 14:473 (E-PR-012) | verbindlich | Umfang nach Audits zu bestätigen |
| ADU-C-015 | Qualitätstore: Core-Gate, Performance-Gate, QA-Gate, Visual-Gate | 14:473 (E-PR-012); 30:145–158 | verbindlich | Schwellwerte fehlen (Abschnitt Akzeptanz) |
| ADU-C-016 | Registry-Freeze erst nach Missing-Family-Audit und Qwen-Audit | 14:470 (E-PR-009, 📋 Reihenfolge); 34:126 (OE-82) | verbindlich (Reihenfolge) / offen (Audits) | Audits nicht durchgeführt (`34:128–130`); Qwen-Audit betrifft ADU-C-009, nicht genehmigt |
| ADU-C-017 | Vor jedem großen Teilsystem eine kurze technische Spezifikation | 02:124; 40:413 | verbindlich | — |

---

## D. Technische Architektur (Pakete, Speicherung, Mods)

| ID | Anforderung | Fundstelle | Status | Blockade / Hinweis |
|---|---|---|---|---|
| ADU-D-001 | Adult als optional ladbares Modul; Hauptspiel ist ohne Adult baubar; eigene C++-Module | 02:92–94; 14:128 (E-PA-028) | verbindlich | — |
| ADU-D-002 | Ein gemeinsamer Human Core; Adult ergänzt nur; gleiche öffentliche Schnittstellen wie Drittanbieter-Mods | 02:24–28; 14:134 (E-PA-034), 14:196 (E-SO-005) | verbindlich | — |
| ADU-D-003 | Adult-Daten separat gespeichert, verknüpft über stabile IDs; Basisspielstand lädt ohne Paket | 02:102–107; 14:129–130 (E-PA-029/030) | verbindlich | — |
| ADU-D-004 | Verwaiste Referenzen werden markiert, nicht gelöscht; Hot Enable/Disable | 02:107, 02:109; 14:130 | verbindlich | — |
| ADU-D-005 | Schema-Version und Migration | 02:108 (ohne Zeichen); 14:448 (E-SP-004) | unklar | Kein Statuszeichen in 02:108 |
| ADU-D-006 | Feature Flags, Crash-Isolation, Mod-Sandbox, Versionsprüfung | 02:110, 02:121; 14:130 (E-PA-030) | verbindlich | — |
| ADU-D-007 | Builds: Basis, intern, intern mit Adult, später Distribution | 02:75–82; 14:131 (E-PA-031) | verbindlich | Distributions-Build offen |
| ADU-D-008 | Basisbuild enthält keinen Adult-Content, auch nicht versteckt | 02:45; 14:132 (E-PA-032, 🧭) | vorläufig | Plattform-Aussage ungeprüft |
| ADU-D-009 | Offizielles Online ohne Mods; Paket dort nicht nutzbar; Koop mit Freunden und Mods möglich | 02:42–43; 14:106–107 | verbindlich | Gast-Altersprüfung offen (Adult-K21, OE-06) |
| ADU-D-010 | Nur Host aktiviert Adult; Serverautorität über Sitzung und Zustimmung | 14:449 (E-SP-005, 📋); 28:154 | verbindlich (Host-Aktivierung) / vorläufig (Serverautorität) | Netzwerk offen (02:152) |
| ADU-D-011 | Engine-neutraler Kern in C++20, Unreal nur als Adapter | 02:160; 14:125 (E-PA-025) | verbindlich | Unreal-Version offen (Adult-OE-40, CLAUDE §5) |
| ADU-D-012 | Stabile, versionsfreie IDs (`namespace:kind:local-id`), Schema-Version, Alias-Tabelle | 21:47–53; 14:451 (E-SP-007, 🧭) | vorläufig | — |
| ADU-D-013 | Präferenz- und Kategorien-Registry, data-driven; keine geschlossenen Enums | 21:55–71 (🧭); 14:172–173 (E-IA-029/030) | vorläufig (Struktur) / offen (Freeze) | Registry-Freeze offen (ADU-C-016) |
| ADU-D-014 | Sicherheitskern (Alter, Zustimmung, Blockregeln, Serverautorität) nicht moddbar | 01:171; 02:132–134; 14:59 (E-SK-004) | verbindlich | Liste moddbarer Teile vorläufig (02:136) |
| ADU-D-015 | Mods: lokal `<GameRoot>/mods/`, provider-unabhängig, offline-first, kein unbeschränkter Internetzugriff | 14:136 (E-PA-036); 28:119–123 | verbindlich | Workshop-Frage offen (14:108) |
| ADU-D-016 | Content-Rating und Adult-Flag in Mod-Metadaten; Nutzerfilter | 14:137 (E-PA-037); 28:130 | verbindlich | Prüfablauf offen (01:173) |
| ADU-D-017 | Speicherung nur Ergebnisse und Zustände, keine Frames, Kamera oder IK; Delta gegenüber generiertem Ausgangszustand | 21:38; 14:330 (E-LE-002) | verbindlich | — |
| ADU-D-018 | Speichergröße 50 bis 150 MB: nur Orientierung, nicht gemessen | 14:331 (E-LE-003, 🧭); 30:54 | offen | Budget festzulegen (Adult-OE-73); Zahl 50–150 MB nur 🧭 (14:331, Vorschlag) |
| ADU-D-019 | Determinismus: Gameplay- und Replay-Zustand deterministisch; kosmetische Physik darf abweichen | 02:114; 14:445 (E-SP-001) | verbindlich | — |
| ADU-D-020 | Ereignisgesteuert statt Tick; Simulations-LOD; Budgets je Subsystem vorab | 02:120; 14:473 (E-PR-012) | verbindlich (Grundsatz) / offen (Zahlen) | Budgetzahlen fehlen (`30:66`, OE-76) |
| ADU-D-021 | Telemetrie nur für Funktion, Fehler, Abstürze; keine individuellen Verläufe | 02:122; 28:166 | verbindlich | — |
| ADU-D-022 | Mods registrieren Reaktionen und Präferenzen über Registry, ändern aber Reaktionsgraph und Regelkern nicht | 28:131; 23:144; 14:450 (E-SP-006) | vorläufig | Offen: Zugriff auf Reaktionsgraph (28:248) |
| ADU-D-023 | Replay ist kein Savegame; deterministische Kernspur; Adult-Replays gehören zum Adult-Speicher | 27:196–202; 28:113; 14:437 (E-RP-008) | verbindlich | Replay-Format offen |
| ADU-D-024 | Standalone-C++-Tests ohne Unreal; Unreal-Automation getrennt | 29:365–371; CLAUDE §5 | verbindlich (Architektur) | Testframework offen (Adult-OE-41) |
| ADU-D-025 | Speicherformat, Backend und Verschlüsselung nicht entschieden | 21:280–282; 34:82–83 | offen | Adult-OE-42, 43 |
| ADU-D-026 | Konfigurationsschichten: Global, Mod-Profil, Charakter, Speicherstand, pro NPC; Overrides nie destruktiv | 21:227; 14:452 (E-SP-008, 🧭) | vorläufig | — |
| ADU-D-027 | Build-Validator: Basisbuild-Scan auf Adult-Assets, Verweise und Symbole | 28:75; 29:330; 14:132 | vorläufig | Akzeptanz in 30:135 |
| ADU-D-028 | Interaktionsengine: Freeform statt Positionsmenü; Pose-Templates nur als Hilfen; Kette Intent → Kontakt → IK → Balance → Mikrovariation | 03:112–121; 22:57–59; 14:199 (E-SO-008, 🧭) | verbindlich (Freeform) / vorläufig (Kette) | Startposition offen (14:200, E-SO-009) |

---

## E. Offen oder widersprüchlich

| ID | Thema | Fundstelle | Status | Hinweis |
|---|---|---|---|---|
| ADU-E-01 | Handeln gegen den Willen im Admin-Build („USER-LOCK“) | 01:158–163; 09:58–62; 11:22; 14:522 (E-XX-013) | verworfen (⛔), nicht zurückgezogen | **NICHT GENEHMIGT** (⛔); `CONFLICTS` K-31 offen. Adult-K2, OE-01: zurückziehen, Testpfad ohne Handlungsmechanik oder aufrechterhalten; Rechtsprüfung nötig |
| ADU-E-02 | Spielerbegangene Sexualstraftaten als Weltereignis | 09:38–48; 14:394 (E-WE-013, 🧭); 11:23 | vorläufig / offen | **NICHT GENEHMIGT** (keine Darstellung freigegeben). Adult-K3, OE-15; Auswirkung auf Altersfreigabe und Plattform |
| ADU-E-03 | Orientierung: Setting „keine LGBTQ-Inhalte“ gegen datengetriebenes Modell; Entwickler spricht auch von „Spektrum“ | 05:70–71; 14:314; 19:113–123 (K5) | widersprüchlich | Adult-K5, OE-10. Diskriminierungsrisiko ist Entwicklerentscheidung; die abwertende Äußerung (`38:152`) ist nicht übernommen |
| ADU-E-04 | Einvernehmlicher Schlafzustand | 01:123; AS:124; 11:29 (K9); 34:37 (OE-03) | widersprüchlich | Strengere Regel gilt bis zur Entscheidung |
| ADU-E-05 | Gewichtung der Live-Signale, Capacity-Schwellen, Erkennungsfenster | 01:104; 20:216; 34:36–38 (OE-02, OE-04) | offen | Prototyp-Frage; AK-ZU-14 „festzulegen“ (`30:43`) |
| ADU-E-06 | Gespielte Nichteinwilligung in Produktionen (Rollenspiel) | 03:156; 19:94–100 (K4); 11:24; 14:518 (E-XX-009) | offen | Nicht freigegeben; Abgrenzung messbar machen (Adult-K4, OE-16) |
| ADU-E-07 | Welche extremen einvernehmlichen Inhalte ausgeliefert werden; Detailtiefe von Grenzreaktionen | 06:61; 09:90–93; 23:170; 34:99 (OE-60) | offen | Inhalts-, Rechts- und Plattformentscheidung |
| ADU-E-08 | Familiengründung und Kinder im Adult-Kontext | 05:29; 14:321; 36 (9B.3 Q18); 34:58 (OE-20) | widersprüchlich | Siehe W-02 |
| ADU-E-09 | Zeitmaßstab (30 Minuten Realzeit ≠ 12 Spielstunden) | 14:339 (E-LE-011, 🧭); 19:159–163 (K15); 34:52 (OE-14) | offen | Nicht bestätigt (`19:159–163`) |
| ADU-E-10 | Entwicklungszeitpunkt von Adult | 39:620 („sehr früh“); 39:626 („Ladebildschirm zuerst“); 34:88 (OE-48) | widersprüchlich | Adult-K19; Spätaussage nach `39:626` |
| ADU-E-11 | Abschlussstatus „Adult fertig“ gegen offene Audits | 37:83; 14:476 (E-PR-015, 🧭); 17:119 | widersprüchlich | Adult-K22/K23 aufgelöst: Planung „im Wesentlichen abgeschlossen“, Audits offen (`35:812`) |
| ADU-E-12 | Zuwanderung in der Bevölkerung („keine Immigranten“) | 14:303 (E-NP-014, ❓); 19:147–149 (K12); 34:50 | offen | Nicht entschieden; Entwicklerantwort „Ok“ ohne Klärung (`19:149`) |
| ADU-E-13 | Religion und Beschneidung (99,9 % christlich) | 14:252 (E-KO-017); 19:155–157 (K14) | offen | Setting-Entscheidung |
| ADU-E-14 | Mehrspieler-Gäste: Zustimmung und Alter | 02:152; 28:160; 34:40 (OE-06) | offen | Adult-K21 |
| ADU-E-15 | Dauerrahmen über mehrere Tage | 01:81; 20:325; 34:39 (OE-05) | offen | Quelle sagt: neue Begegnung = neue Berechnung |
| ADU-E-16 | Regurgitation und Vomiting als eigene Zustände | 39:390 (Q257); 14:258 (E-KO-023) | offen | Entwicklerantwort „was genau?“ (`39:390`) |
| ADU-E-17 | Pauschal bestätigte Blöcke ohne Einzelprüfung | 36:383–432; 40:187–206; 39:507 | offen | Siehe W-11 |
| ADU-E-18 | Vorbehalt zur Quellenlage: Runden 16–20 nicht konsolidiert; Datei `USER_REQUESTED_BUT_NOT_CANONICALIZED.md` nicht auffindbar | 13:52–53; 41:249–250; 14:533 (E-XX-024) | offen | Kanonisierung ausstehend |

---

## F. Verworfen oder historisch

| ID | Inhalt | Fundstelle | Status | Hinweis |
|---|---|---|---|---|
| ADU-F-01 | Rahmenentwurf 28.08.: Presentation Modes, FadeToBlack, PreferenceLevel, UE-Typen im Kern, Online-Anfrage-Flow, Intensitätsschalter, 21 Phasen | 35:83–97; 12:23; 17:123 | historisch | Vorlage nur für Zustimmung, Zustandsautomat, Fallback (35:104–117) |
| ADU-F-02 | Frühe Zustimmungsaussagen (10./11.09.): Nicken genügt bei Betrunkenheit; „ein Nein ist oft nur Spiel“; Frauen „tun nur so“ | 01:127–136; 09:64–66; 14:510, 14:514 (E-XX-001/005) | historisch | Ersetzt durch Capacity Gate und Sitzungsrahmen |
| ADU-F-03 | Verworfen: Belohnung für Rettung mit Sex; Fan-Mod-Tarnung; codierte Umschreibung; Links zu Videos als Vorlage | 09:41; 14:491, 14:512, 14:529, 14:531–532 (E-RC-003, E-XX-003/020/022/023) | verworfen | — |
| ADU-F-04 | Schweigen oder Stöhnen heißt weitermachen (16.09.) | 01:100; 14:519 (E-XX-010) | historisch | Ersetzt durch Sitzungsrahmen (14:77) |
| ADU-F-05 | „Nein ohne Änderungschance“ und „Stopp-Logik greift bei Bewusstlosigkeit nicht“ | 05:61; 14:298; 14:515 (E-XX-006) | historisch / verworfen | Ersetzt durch Abbruch (01:118–120) |
| ADU-F-06 | Kündigung des Begleitvertrags nie („99,99999 %“) | 14:516 (E-XX-007) | historisch | Kompromiss ADU-B-032 |
| ADU-F-07 | STI-Simulation in der Basisversion | 06:102; 14:274 (E-GE-004) | verworfen | Architektur bleibt erweiterbar |
| ADU-F-08 | Tattoo-Editor als Adult-Paket (frühe Doku-Fassung) | 07:46; 18:74 | historisch | Korrigiert (ADU-B-028) |
| ADU-F-09 | Hardcore als Katalogklasse, globaler Intensitätsschalter, Online-Adult mit Anfrage-Flow | 02:67; 35:84–85 | historisch | — |
| ADU-F-10 | „Jungfrau“ als Preis- oder Shop-Kategorie; abwertende Verallgemeinerung über Performerinnen | 26:89; 14:527 (E-XX-018) | verworfen | — |
| ADU-F-11 | Gewalt gegen Menschen wegen Orientierung als Weltregel; „konservative Welt“ hardcoden | 09:82; 14:528 (E-XX-019); 24:200 | verworfen | Entwickler: Welt soll logisch denken (14:528) |
| ADU-F-12 | Begriff „Sklavin“ für Begleitung | 24:217 (NP-DV-08) | verworfen | — |
| ADU-F-13 | Interner Entwurf mit versionsabhängigen IDs im engine-neutralen Kern | 02:98 | historisch | Nicht übernommen |
| ADU-F-14 | Qwen-Entwurf: Hard-Limit-Umgehung über Story-Marker | 14:454 (E-SP-010) | verworfen | Vom Assistenten als unzulässig korrigiert (E-SK-028) |
| ADU-F-15 | Überholte Zahlen: 58, 175 oder 246 Abschnitte im Rahmenentwurf | 12:23; 13:83; 35:149 | historisch | Siehe W-09 |

---

## Widersprüche

| Nr. | Widerspruch | Quellen | Hinweis |
|---|---|---|---|
| W-01 | Basisspiel-Romantik: `Adult` 📋 gegen `DEC` S-14 und `ES:92` | 05:28; 14:320; GS:220 gegen DEC S-14; ES:92; CON K-10/K-23 | Entwicklerentscheidung nötig |
| W-02 | Familie/Kinder: 📋 „passt alles“ (36) gegen „offen, niedrige Priorität“ (34:58, 14:321) | 36:383–432 (Q18); 34:58; 05:29 | Schwache Pauschalzustimmung |
| W-03 | Orientierungs-Setting: „keine LGBTQ-Inhalte“ (📋) gegen „theoretisch mit jedem“ (05:61) und ungeklärtes Modell | 05:61, 05:70–71; 19:113–123 | Adult-K5 |
| W-04 | Einvernehmlicher Schlafzustand (🧭) gegen „Schlafende nicht aktiv“ (AS:124, 📋) | 01:123; AS:124; 11:29 | Adult-K9 |
| W-05 | Zustimmungsdauer: Bestätigung in der Situation, Ausdruck „passt alles“ für Nachfolgefragen | 01:81; 14:80; 36 (R10 Q5) | Adult-K1 |
| W-06 | Entwicklungszeitpunkt Adult: „sehr früh“ (39:620) gegen „nach Ladebildschirm und Karte“ (39:626) | 39:620, 39:626; 34:88 | Adult-K19 |
| W-07 | Status „Adult fertig“ (37:83) gegen „Audit offen“ (35:812) | 37:83; 17:119 | Adult-K22 aufgelöst, Audit offen |
| W-08 | Veröffentlichung: Ziel „öffentlich“ (14:474, 40:411 „will ich schon“) gegen früher „privat“ (35:717) | 10:35; 40:411; 14:474 | Späte Aussage gilt; bestätigen |
| W-09 | Abschnittszahl Rahmenentwurf: 58, 175 oder 246 | 12:23; 13:83; 35:149 | Quellen-internes Problem |
| W-10 | Gore und Kinder: „bei Kindern kein Gore“ (14:92) gegen „eingeschränktes Gore-Profil“ (14:91) | 14:91–92; 17:143 (`1060`) | Außerhalb Adult, Verweis |
| W-11 | Matrix-Status: pauschale „passt alles“-Antworten als 📋 gezählt, Register stützt sich nicht darauf | 36:383–432; 40:187–206; 41:53 | Gültigkeit offen |

---

## Offene Entscheidungen für den Entwickler

Keine dieser Entscheidungen ist getroffen. Optionen sind aus den Quellen übernommen (Adult-OE-Katalog, `34`).

| Nr. | Entscheidung | Optionen (aus Quellen) | Blockiert |
|---|---|---|---|
| D-01 | Adult-K2 / OE-01: Handeln gegen den Willen (USER-LOCK) | zurückziehen; reiner Konsequenz-Testpfad; aufrechterhalten (dann außerhalb dieser Dokumentation) | ADU-E-01, Testumfang (`29:377`) |
| D-02 | `CON` K-10/K-23 und `DEC` S-14: Romantik und sexuelle Beziehungen im Basisspiel | Umfang festlegen oder Rahmen bestätigen | ADU-A-002 bis A-005 |
| D-03 | Adult-K5 / OE-10 und OE-11: Orientierungsmodell und Spielerfigur-Konstellationen | neutrales Modell mit Datenvorgabe; Vorgabe nur für Hauptwelt; weitere Vorgabe | ADU-A-017, ADU-E-03 |
| D-04 | Adult-OE-20: Familiensystem | niedrige Priorität/offen; Umfang | ADU-A-005, ADU-E-08 |
| D-05 | Adult-K1 / OE-02, OE-04, OE-77: Live-Signale und Schwellen | Prototyp-Ermittlung oder feste Werte | ADU-B-012 bis B-015 |
| D-06 | Adult-K9 / OE-03: einvernehmlicher Schlafzustand | nein (strengere Regel) oder vereinbartes Schlafspiel | ADU-B-029, ADU-E-04 |
| D-07 | Adult-K3 / OE-15: Sexualstraftaten durch Spielerfiguren | nicht darstellen; nur Konsequenzebene; anderes | ADU-E-02, Altersfreigabe |
| D-08 | Adult-K4 / OE-16: gespielte Nichteinwilligung in Produktionen | nicht aufnehmen; als Rollenspiel mit Spezifikation und Rechtsprüfung | ADU-E-06 |
| D-09 | Adult-OE-60, 61, 22; Adult-K16, K17; OE-34: Inhalte, Darstellungsprofile, Symbolik | Auslieferungsumfang; Profile; Symbolik-Regel | ADU-B-025, ADU-C-010, ADU-E-07 |
| D-10 | Adult-K18 / OE-30 bis 32, 38: Veröffentlichung, Vertrieb, Altersfreigabe, Rechtsberatung | öffentlich als Ziel, privat als Rückfall; Vertriebsweg | ADU-C-001 bis C-005 |
| D-11 | Adult-K7 / OE-33, OE-81: KI-Offenlegung und Rolle von Qwen | wahrheitsgemäße Deklaration; Herkunftsregister; Qwen-Rolle | ADU-C-008, C-009 |
| D-12 | Adult-K6 / OE-17: Begleitvertrag und Kündigung | Kompromiss beibehalten oder ändern; Altersnachweisverfahren | ADU-B-032, ADU-E-15 |
| D-13 | Adult-K15 / OE-14: Zeitmaßstab | nahe 1:1; anderer Faktor; variabel | ADU-E-09, Heilung, Schwangerschaft |
| D-14 | Adult-K19 / OE-48: Zeitpunkt von Adult | „sehr früh“ oder „nach Ladebildschirm und Karte“ | ADU-E-10, Phasenplanung |
| D-15 | Adult-K12, K14 / OE-12, 13: Zuwanderung, Religion, Beschneidung | Setting-Daten festlegen | ADU-E-12, E-13 |
| D-16 | Adult-K21 / OE-06: Gäste im privaten Koop | Host-Verantwortung; Gast-Opt-in; Altersnachweis | ADU-D-009, ADU-E-14 |
| D-17 | Adult-OE-05: Dauerrahmen | nein; ja mit Ablauf; pro Beziehungsform | ADU-B-011, ADU-E-15 |
| D-18 | Adult-OE-80 bis 86: Missing-Family-Audit, Qwen-Audit, Registry-Freeze | Durchführung und Reihenfolge | ADU-C-016, ADU-D-013 |

---

## Akzeptanzkriterien, fehlend oder nicht festgelegt

- Schwellwerte mit „festzulegen“ in `30`: AK-ZU-8 und -9 (`30:37–38`), AK-ZU-14 (`30:43`), AK-DM-6 (`30:54`), AK-IE-6 (`30:66`), AK-TA-3 und -7 (`30:101`, `30:105`), AK-ST-4 (`30:126`), AK-NP-5 (`30:91`), AK-SM-8 (`30:142`).
- Offen bleibt AK-KR-8 (`30:81`), AK-NP-7 (`30:93`), AK-WE-5 und -6 (`30:116–117`), AK-ST-7 (`30:129`) und die Gesamtabnahme (`30:156`, Stufe F).
- Die Testfälle in `29` sind ein Entwurf; nichts davon wurde ausgeführt (`29:4`). Das Testframework ist offen (`29:373`).
- Die Toleranz für Verteilungstests fehlt (`29:257`, `30:105`).
- Barrierefreiheit ist ohne Kriterien (`30:129`, `27:267`).

## Prüfhinweise zur Dokumentation selbst

- `41:249–258`: Abschnitt „Durchgeführte Prüfungen“ enthält nur „wird nach den Prüfläufen ergänzt“.
- `20:54`: Der Anker `#k2-user-lock-vom-2009` ist ein Schreibfehler (gemeint ist 20.09.). Gleiches Muster in `24:181`, `34:35`.
- `13:191`: Der Hinweis „Abschnitt XX“ verweist auf einen nicht vorhandenen Abschnitt.
- Die Matrizen 36 bis 40 nennen Register-Bezüge nur namentlich; die Modulzuordnung gilt auf Paketebene (`36:17`).
- Die Projekt-Konfliktliste `CON` nummeriert anders (K-10, K-23 = Romantik). Keine Umnummerierung vorgenommen.

---

## G. Konsolidierungsstand (CONSOLIDATION-ADULT, 08.11.2026)

**Vorgehen.** (1) Alle 312 Zitierungen mit Zeilennummern wurden maschinell gegen die Zieldateien geprüft: Datei vorhanden, Zeile innerhalb der Datei, erste Zeile nicht leer, genannte Kennungen (`E-…`) stehen in der zitierten Zeile oder im Umfeld. (2) Die Statuszeichen (📋, 🧭, ❓, 🕰️, ⛔) in den zitierten Zeilen wurden mit dem Nachweis-Status verglichen. (3) Auffälligkeiten wurden von Hand gelesen. Keine Änderung an `Docs/Game/`, keine Spielentscheidung, keine Commits.

**Ergebnis der Zählung (131 Anforderungen).** verbindlich 61, vorläufig 18, offen 26, widersprüchlich 8, historisch 9, verworfen 8, unklar 1. Mehrfach-Status (z. B. „verbindlich / vorläufig“) sind der ersten Kategorie zugeordnet, wie im Nachweis angegeben.

**Korrigierte Zitierungen (fünf leere oder falsche Zeilen).**
- ADU-B-019: `20:172` → `20:176–182` (Zeile 176 ist E-SK-030).
- ADU-D-006: `02:117–121` → `02:110, 02:121` (Feature Flags, Ausfallsicherheit).
- ADU-D-008: `02:62` → `02:45` (🧭 Vorschlag gegen versteckten Adult-Content im Basisbuild; `02:62` betraf Paketversionen).
- ADU-E-01: `01:157–163` → `01:158–163` (Zeile 157 leer).
- ADU-E-17 (und W-11, Widersprüche): `40:181–206` → `40:187–206`.
- ADU-F-04: `01:103` → `01:100` (Zeile 103 leer; die Aussage „Schweigen allein ist keine Zustimmung“ steht in 01:100).

**Korrigierte Statuszuordnung (Quelle trägt ein anderes Zeichen).**
- ADU-A-016: „vorläufig“ → „verbindlich (Nein bleibt Nein, 05:61 📋); Desinteresse-Entwicklung vorläufig“.
- ADU-C-016: „offen“ → „verbindlich (Reihenfolge, 14:470 📋) / offen (Audits)“.
- ADU-D-010: „vorläufig“ → „verbindlich (Host-Aktivierung, 14:449 📋) / vorläufig (Serverautorität)“.
- ADU-D-013: „verbindlich / offen“ → „vorläufig (Struktur, 21:55 🧭) / offen (Freeze)“; die Quelle ist als 🧭 gekennzeichnet.
- ADU-D-018: Hinweis ergänzt, Zahlen 50–150 MB nur 🧭 (14:331).

**Nicht genehmigte Inhalte, ausdrücklich markiert.** ADU-C-009 (Qwen, „jetzt nicht“, 40:66), ADU-C-010 (extremistische Symbolik), ADU-E-01 (Handeln gegen den Willen, ⛔, nicht zurückgezogen), ADU-E-02 (Sexualstraftaten durch Spielerfiguren). ADU-E-06 und ADU-E-07 trugen die Markierung „nicht freigegeben“ bzw. „offen“ bereits. Keine dieser Positionen ist übernommen.

**Offen, weil eine Entwicklerentscheidung nötig ist (unverändert).**
- K-23 (Romantik im Basisspiel, bleibt offen): ADU-A-002 bis A-005, Widerspruch W-01.
- K-31 (USER-LOCK „Handeln gegen den Willen“, bleibt offen): ADU-E-01.
- K-10 (Romantik, Basisspiel): ADU-A-002.
- Weitere Entscheidungen: Abschnitt „Offene Entscheidungen“ (D-01 bis D-18).

**Kategorien nach Vorgabe.** Allgemeine Beziehungen und Romantik: Abschnitt A. Optionale explizite Inhalte und Altersregeln: Abschnitt B. Technische Architektur und Modulschnittstellen: Abschnitt D. Produktion, Recht und Vertrieb: Abschnitt C. Offene, widersprüchliche und historische Konzepte: Abschnitte E und F.

**Ergebnis der Prüfung.** Die 131 Anforderungen sind in den Zieldateien unter `Docs/Game/Adult/` und `Docs/Game/Adult-System.md` belegt. Eine Übernahme in die Planungsregister (`DECISIONS.md`) ist nicht erfolgt, weil keine der Anforderungen dort als Entscheidung geführt werden darf, solange K-23 und K-31 offen sind. Die Nachweise selbst sind der Stand.
