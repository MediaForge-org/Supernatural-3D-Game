# Adult 14: Entscheidungsregister

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Register verzeichnet Entscheidungen, Vorschläge und Widersprüche der Planungsquellen mit **Status und Herkunft**. Es trifft **keine** Entscheidungen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Quellenschlüssel: [Modul 13, Abschnitt 1](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Die erzählte Reihenfolge steht in [Modul 18](18-Chronologie-Detail.md), die Detailprotokolle in den Modulen [15](15-Interviewprotokoll-Runden-1-bis-10.md) bis [17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md), Widersprüche in [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md).

**Inhalt**

1. [Lesehilfe](#1-lesehilfe)
2. [SK: Sicherheitskern, Alter, Zustimmung](#2-sk-sicherheitskern-alter-zustimmung)
3. [PA: Pakete, Builds, Architektur](#3-pa-pakete-builds-architektur)
4. [IA: Interaktion, Taxonomie, Ablauf](#4-ia-interaktion-taxonomie-ablauf)
5. [SO: Freeform-Engine, Solver, Kontakt](#5-so-freeform-engine-solver-kontakt)
6. [KO: Körper, Reaktionen, Physiologie](#6-ko-körper-reaktionen-physiologie)
7. [GE: Gesundheit, Verletzung, Fortpflanzung](#7-ge-gesundheit-verletzung-fortpflanzung)
8. [NP: NPC, Präferenzen, Beziehungen](#8-np-npc-präferenzen-beziehungen)
9. [LE: Gedächtnis, Leben, Simulation, Speicher](#9-le-gedächtnis-leben-simulation-speicher)
10. [TA: Tattoos, Piercings, Körpermodifikation](#10-ta-tattoos-piercings-körpermodifikation)
11. [WE: Welt, Orte, Branche, Wirtschaft](#11-we-welt-orte-branche-wirtschaft)
12. [ST: Steuerung, Oberfläche, VR, Kamera, Audio](#12-st-steuerung-oberfläche-vr-kamera-audio)
13. [RP: Replay, Aufnahme, Streaming](#13-rp-replay-aufnahme-streaming)
14. [SP: Speicherstand, Modding, Builds, Technik](#14-sp-speicherstand-modding-builds-technik)
15. [PR: Produktion, Qualität, Reihenfolge](#15-pr-produktion-qualität-reihenfolge)
16. [RC: Recht, Plattform, KI, Vertrieb](#16-rc-recht-plattform-ki-vertrieb)
17. [XX: Nicht freigegeben, historisch, überholt](#17-xx-nicht-freigegeben-historisch-überholt)
18. [Zählung](#18-zählung)

---

## 1. Lesehilfe

| Spalte | Bedeutung |
|---|---|
| ID | Fachkürzel plus laufende Nummer. IDs sind stabil; sie werden nie neu vergeben. |
| Datum | Tag der Aussage (2026) |
| Quelle | Nachrichten- oder Abschnittsnummer ([Zitierschlüssel](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel)). `E` hinter der Nummer = **Entwickleraussage**, `A` = **Assistentenvorschlag**, `K` = konsolidierter Kanon |
| Aussage | die Entscheidung oder der Vorschlag in neutraler Formulierung |
| Status | 📋 vom Entwickler bestätigt (nicht konsolidiert, außer `K`) · 🧭 Vorschlag oder vorläufig · ❓ offen · 🕰️ überholt · ⛔ nicht freigegeben |
| Vermerk | Einordnung, Verweise |

**Regeln der Einordnung**

- Eine Entwickleraussage gilt als 📋, wenn er auf einen Vorschlag mit „ja“ antwortete (auch pauschal, z. B. „1–25 ja“) oder selbst eine Festlegung formulierte.
- Eine pauschale Zustimmung auf einen Block, in dem später ein Teil zurückgenommen oder korrigiert wurde, gilt nur für den **nicht korrigierten Teil**.
- Antwortete der Entwickler nur mit einer Rückfrage („was meinst du“), ist die Frage **nicht** entschieden, bis die Antwort folgt.
- Zahlen, die der Assistent vorschlug (Prozentwerte, Größenordnungen, Schätzungen), sind 🧭, auch wenn der Entwickler „passt“ schrieb, solange er nicht ausdrücklich die Zahl bestätigte.
- Aussagen mit rechtlichem Inhalt sind **ungeprüft** ([Modul 10](10-Produktion-Recht-Vertrieb.md)).

---

## 2. SK: Sicherheitskern, Alter, Zustimmung

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-SK-001 | 31.08. | PS 18; K§18 | Zustimmung ist kein boolescher Wert; Beziehung ersetzt Zustimmung nie | 📋 | Kanon; Grundlage von [Modul 01](01-Grundregeln-Alter-Zustimmung.md) |
| E-SK-002 | 28.08. | 28#25A | Teilnehmer: eindeutig erwachsen, bei Bewusstsein, handlungs- und entscheidungsfähig, können aktiv zustimmen und widerrufen | 🧭 | Entwurf; später vom Entwickler inhaltlich bestätigt (E-SK-006, E-SK-015) |
| E-SK-003 | 28.08. | 28#25A | Nicht unterstützt: Minderjährige, nicht-einvernehmliche Interaktion, Bewusstlose, Schlafende als aktive Teilnehmer, Story-/Quest-Zwang, Beziehungswert oder KI-Ziel als Zustimmung, Party-/Admin-Zustimmung | 🧭 | Entwurf; Admin-/Gruppenzustimmung in E-SK-004 bestätigt |
| E-SK-004 | 31.08. | PS 15; K§16 | Sicherheitskern nicht normal moddbar: AdultEligibility, Consent, BlockRules, ServerAuthority, AccountPermissions, AntiSpam, OnlineSecurity | 📋 | Kanon |
| E-SK-005 | 10.09. | 35#187E | Hard Limits können sich über die Zeit ändern (anfangs tabu, später erlaubt), bei Einmalbegegnungen eher weniger | 📋 | Assistent 35#188: ein *aktuelles* Hard Limit wird nie mitten in einer Situation überschrieben |
| E-SK-006 | 10.09. | 35#190A, 35#192A, 35#196A, 35#200A | Entscheidungsfähigkeit (*Capacity Gate*): Zustimmung = Zustimmung plus ausreichende Entscheidungsfähigkeit; Alkohol allein erzeugt keine Zustimmung | 🧭 → 📋 | Vom Entwickler zunächst abgelehnt (E-XX-001, E-XX-002), am 13.09. bestätigt (E-SK-015) |
| E-SK-007 | 10.09. | 35#190A | Reihenfolge: Capacity → Consent → Eskalation (nicht „keine Abwehr → erlaubt“) | 📋 | bestätigt mit E-SK-015 |
| E-SK-008 | 10.09. | 35#194A | Nähe, Zuneigung, Vertrauen beeinflussen Wahrscheinlichkeit von Angeboten, ersetzen Zustimmung nicht | 🧭 | Entwickler 35#193: „Dann sowas wie Nähe, Zuneigung und so weiter“ |
| E-SK-009 | 11.09. | 35#237E | Ab 18 Jahren und 0 Tagen zugelassen; darunter nicht erlaubt und nicht angezeigt | 📋 | |
| E-SK-010 | 11.09. | 35#238A | Alter unbekannt oder nicht eindeutig als 18+ markiert: ausgeschlossen; keine Ableitung aus Aussehen/Körper/Stimme; Prüfung vor allen anderen Regeln | 🧭 → 📋 | Vom Entwickler weiterentwickelt (E-SK-011) |
| E-SK-011 | 11.09. | 35#239E | Bei unbekanntem Alter: Figur kann gefragt werden und zeigt Ausweis oder anderen Nachweis | 📋 | |
| E-SK-012 | 11.09. | 35#240A | Nur Status „verifizierter Erwachsener“ wird gespeichert, nicht der Ausweis; Geburtsdatum statt „Alter = 18“; Tests mit 18 Jahren + 0 Tage | 🧭 | Entwickler 35#243 „Ja genau“ auf E-SK-013, nicht ausdrücklich auf diesen Vorschlag |
| E-SK-013 | 11.09. | 35#241E, 35#242A, 35#243E | Alter und Vorlieben sind getrennt: 18+ ist nur ein Eligibility-Gate | 📋 | |
| E-SK-014 | 11.09. | 35#262A | Verstorbene NPCs: Identität und wichtige History bleiben erhalten | 🧭 | Entwickler 35#261 „39 ja, aber Person kennen/Vorlieben gespeichert außer sie ist tot“ |
| E-SK-015 | 13.09. | 35#444E, 35#446E, 35#448E–454E | Betrunkene, die noch denken und antworten können, können wirksam zustimmen; ein klares „Ja“ genügt; Nicken bei klarem Verständnis; ein „komm mit zu mir“ signalisiert Interesse | 📋 | Assistent 35#445: kein pauschales Okay für jede Handlung danach |
| E-SK-016 | 13.09. | 35#447A | Entscheidungsformel `capacity + responsiveness + prior affirmative intent + current behavior` statt Promillegrenze | 🧭 | |
| E-SK-017 | 13.09. | 35#451A, 35#453A, 35#454E | Bewusstlos/nicht ansprechbar: Interaktion endet; Erkennung darf kurz dauern (Awareness-Verzögerung), dann sofort Abbruch; kein minutenlanges Weitersimulieren | 📋 | Entwickler 35#454 „Ja ok dann passt das“ |
| E-SK-018 | 13.09. | 35#453A | Awareness-/Responsiveness-System erkennt Bewusstlosigkeit über fehlende Eigenbewegung, Muskelspannung, Atemmuster, Antworten; auch für Verletzung, Ohnmacht, Alkohol, Schlaf nutzbar | 🧭 | |
| E-SK-019 | 29.08. | 23#569E, 23#570A | Vereinbartes Stopp-Signal beendet sofort | 📋 | Kanon als K§18 am 31.08. |
| E-SK-020 | 31.08. | K§18 | Hard Stop: sofortiger sicherer Ausstieg, keine Neuinterpretation; mildes Unbehagen und Intensitätspräferenz sind nicht Rückzug; manche Erwachsene bevorzugen einvernehmlich höhere Intensität bis Schmerz | 📋 | Kanon |
| E-SK-021 | 31.08. | PS 18 | Weiche Signale: Unbehagen, Nachfragen, Position, Intensität; „Hard Stop überschreibt alles“; Widerruf jederzeit | 📋 | Kanon |
| E-SK-022 | 16.09. | 35#579A, 35#580E | Sitzungsrahmen (*Session Intent Envelope*): vorher etablierte Freiheit erlaubt spontane Wechsel ohne Dauerabfrage; Mehrsignal-Auswertung; Stöhnen allein genügt nicht | 📋 | Entwickler 35#580: „Ja ok aber nicht zu streng“ |
| E-SK-023 | 16.09. | 35#581A | Keine Dauer-Popups; klares Nein/Stopp/Wegdrücken zählt sofort; ruhig, aber beteiligt: keine Unterbrechung | 🧭 | |
| E-SK-024 | 16.09. | 35#581A | Spontan Entdecktes kann neue Erfahrung und Präferenz erzeugen | 🧭 | Macht Übertritt nicht rückwirkend in Ordnung (35#632A) |
| E-SK-025 | 13.09. | 35#445A | Zustimmung bleibt innerhalb der laufenden Situation bestehen, bis Widerruf oder wesentliche Änderung; neue Begegnung = neue Berechnung | 🧭 | Entwickler 35#444 zu R10 Frage 5: „wenn einmal ja und danach nicht mehr nein, dann wird das ja übernommen“; Assistent schränkte auf die laufende Situation ein; ohne Widerspruch |
| E-SK-026 | 28.08. | 28#25A | Zustimmungszustände: unbekannt, möglicherweise interessiert, angeboten, erwogen, akzeptiert, abgelehnt, zurückgezogen, abgelaufen, vorübergehend nicht verfügbar | 🧭 | Entwurf, in [Modul 01](01-Grundregeln-Alter-Zustimmung.md#3-zustimmungsmodell) übernommen |
| E-SK-027 | 28.08. | 28#25A | Zustimmungs-Datensatz (Anfrage, Teilnehmer, erlaubte Inhalte/Varianten/Profile/Intensität, Kontext, Zeitpunkt, Gültigkeit); Scope-Stufen; Neuprüfung an Grenzen | 🧭 | Entwurf |
| E-SK-028 | 10.09. | 35#176A | Berechtigungsstatus Unknown/NotEligible/AdultEligible; kein Standardwert „wahr“; Hard Limits sind echte Invarianten (keine Story-Ausnahme); Widerruf wirkt logisch sofort, sicherer Ausstieg getrennt | 🧭 | Korrekturvorgaben an das externe Modell (Qwen-Vorphase); Entwickler bestätigte pauschal 35#179 („er soll Fragen stellen“) |
| E-SK-029 | 10.09. | 35#176A | Eligibility-/Capacity-Gate vor Präferenz- und Zustimmungsauswertung; deterministisch, inspizierbar, nicht durch Präferenz-Scores übersteuerbar | 🧭 | |
| E-SK-030 | 11.09. | 35#262A | Erregung ist ein Reaktions-/Arousal-Zustand, nicht Zustimmung oder Präferenz | 🧭 | |
| E-SK-031 | 25.09. | 35#716A | Altersregel als Build-Validator; sehr junge Erwachsene sollen auch visuell eindeutig erwachsen wirken | 🧭 | rechtlich begründet, ungeprüft |
| E-SK-032 | 25.09. | 35#716A | Kein Adult-Interaction-Entry für minderjährige Figuren; Alter als harter Charakterzustand | 🧭 | Entwickler 35#719: „ja genau“ (zu den IP-/Rechtsfolgen insgesamt) |
| E-SK-033 | 26.09. | 35#786A | Realismus-Defaultregel: Alles nicht eigens Festgelegte verhält sich wie in der Realität | 📋 | gilt laut Entwickler 35#785 für das ganze Spiel |
| E-SK-034 | 20.09. | 35#630A, 35#632A | Eine neue, außerhalb des Rahmens liegende Handlung ohne Zustimmung gilt als Grenzverletzung, nicht als Zustimmung; „technisch möglich ≠ einverstanden“ | 🧭 | Siehe E-XX-010 bis E-XX-014 |
| E-SK-035 | 24.09. | 35#708E | „Das meiste passt schon“: weitere Detailfragen reduzieren | 📋 | Prozess, keine Regel |

---

## 3. PA: Pakete, Builds, Architektur

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-PA-001 | 28.08. | 23#511E | Stripclub „wie in GTA“; Sex mit Nacktszenen, richtig realistisch; Nacktheit abschaltbar | 📋 | Ausgangswunsch; später präzisiert |
| E-PA-002 | 28.08. | 23#539E | Offizielles Spiel auf allen Plattformen gleiche, nicht-explizite Darstellung („wie Konsole“) | 📋 | Kanon K§17 |
| E-PA-003 | 28.08. | 23#539E | Explizitere Darstellung nur als getrennte Mod | 📋 | Kanon K§16 |
| E-PA-004 | 28.08. | 23#540A | Paket ist separates Content-Paket, nie in Konsolen-Cooks; kein `PornModInstalled`-Check im Kern | 📋 | Kanon K§16, PS 20 |
| E-PA-005 | 28.08. | 23#546A | Entwickler-eigener Mod darf nicht als unabhängiger Fan-Mod getarnt werden | 🧭 | Auf Entwicklerfrage 23#545 zurückgewiesen; E-XX-020 |
| E-PA-006 | 28.08. | 23#551E | Im offiziellen Online keine Mods (wegen Cheats); mit Freunden in privater Lobby möglich | 📋 | Kanon K§16 |
| E-PA-007 | 31.08. | K§16 | Singleplayer und Campaign-Koop dürfen Mods/Cheats laden | 📋 | Kanon |
| E-PA-008 | 31.08. | K§16 | Vertriebsweg (Workshop, extern usw.) und rechtliche Behandlung bleiben eine Entscheidung zum Veröffentlichungszeitpunkt | ❓ | Kanon, OD |
| E-PA-009 | 31.08. | K§17 | Nacktheit in drei Stufen konfigurierbar (voll/reduziert/aus oder gleichwertig); Intimität angedeutet durch Blocking, Kamera, Animation, Audio, Reaktionen; keine explizite anatomische Penetration im Basisspiel; Assets/Kameras/IK so authoren, dass Fehler nichts zeigen | 📋 | Kanon |
| E-PA-010 | 31.08. | CA05 | Beziehungs-/Romantik-Sandbox mit erwachsenen NPCs gehört zum Basisspiel | 📋 | Kanon |
| E-PA-011 | 10.09. | 35#187E | Kein globaler Intensitätsschalter; Mod an oder aus; wer weniger Extremes will, installiert ein anderes Paket | 📋 | |
| E-PA-012 | 10.09. | 35#187E, 35#189E | „Hardcore“ = Preset über mehrere Einstellungen; Verhalten (wie viel Figuren zulassen, Tempo); nach Preset einzeln änderbar; „kommt auf die Person an“ | 📋 | |
| E-PA-013 | 10.09. | 35#208A, 35#210A | Dreiteilung: A Framework, B Hochintensität, C Dunkle Kriminalität/Mature World Events, unabhängig schaltbar | 🧭 | Entwickler 35#211 „Ok gut“ |
| E-PA-014 | 11.09. | 35#221E | Kategorie > Unterkategorie > Variante; Rollenprofile je Kategorie; eigene Intensitätsdimensionen je Kategorie; Presets Soft/Normal/Intense/Hardcore/Custom; Mods können Presets ergänzen | 📋 | „ja, genauer“ später bestätigt (35#223) |
| E-PA-015 | 11.09. | 35#221E | „Das ist sowieso eine Mod“: alle Strukturfragen betreffen die interne Architektur des optionalen Pakets | 📋 | |
| E-PA-016 | 11.09. | 35#222A | Hierarchie dient Browsen/Filtern im Mod-Editor | 🧭 | |
| E-PA-017 | 11.09. | 35#247E | Orte (Clubs, Hotels, Wohnungen) gehören ins Hauptspiel; „ohne Mod hat man dort weniger zu tun“ | 📋 | Kein Pflicht-Location-Pack |
| E-PA-018 | 11.09. | 35#247E | Aufteilung in Packs hängt von der Größe ab; All-in-One als Standard, wenn nicht riesig; einzelne Kategorien deaktivierbar | 📋 | Größenbeispiele (15–25 GB bzw. 50–100+ GB) sind Assistentenbeispiele |
| E-PA-019 | 11.09. | 35#247E, 35#248A | Packs mit Versionen, Abhängigkeiten, Kompatibilität; requires/optional/incompatible/recommended; Mod-Manager-Status | 📋 | |
| E-PA-020 | 11.09. | 35#247E | Animationspakete nur teilweise wiederverwendbar (Körpermaße, Reaktionen) | 📋 | |
| E-PA-021 | 11.09. | 35#247E, 35#248A | Stimmpakete unabhängig (Sprachen, Qualitäten, alternative Reaktionssets) | 📋 | Entwickler fragte „einfach andere Sprachen?“ |
| E-PA-022 | 11.09. | 35#247E | Preset-Bündel (Minimal, Standard, Full…): „kann man schauen“ | ❓ | |
| E-PA-023 | 11.09. | 35#247E | Stabilität des Frameworks: „weiß noch nicht, mal schauen“ | ❓ | |
| E-PA-024 | 11.09. | 35#248A | Framework = Motor, Content-Packs = Inhalte; Hochintensitäts-Pack ändert das Framework nicht | 🧭 | Entwickler „weiß nicht, erkläre“; danach 35#249 „Ja das passt gut“ |
| E-PA-025 | 13.09. | 35#286A, 35#283E | Gesamtprojekt: Runtime-Gameplay C++-first, Blueprint optional für Modder; eigenes engine-neutrales C++20-Persistenzsystem; Unreal nur Adapter | 📋 | Entwickler 35#283 formulierte die Anforderung selbst |
| E-PA-026 | 13.09. | 35#473A | Architektur: engine-neutraler Kern, Unreal-Integrationsschicht, Daten, Offline-Pipeline, Tools, Backend; Entity-Stufen simuliert/aktiv/materialisiert | 🧭 | Zahlen (5.000/100–300) „müssen gemessen werden“ |
| E-PA-027 | 13.09. | 35#473A | Zielsprachen: C++ 80–90 %, TypeScript nur Tools, Python nur Offline-Pipeline | 🧭 | Schätzung |
| E-PA-028 | 26.09. | 35#804A, 35#805E | Ein gemeinsamer Human Core; Adult nur Erweiterungen; Modulgrenzen; Plugin-Struktur; Hauptspiel baubar ohne Adult | 📋 | 1–3 ja |
| E-PA-029 | 26.09. | 35#805E, 35#806A | Adult-Daten werden **separat gespeichert**, verknüpft über dieselben IDs | 📋 | „die Daten von Adult werden extra gespeichert“ |
| E-PA-030 | 27.09. | 35#806A–808A | Getrennter Adult-Speicherbereich/-Datei; verwaiste Referenzen markieren; Verschlüsselung/Nicht-Lesbarkeit optional; Hot Enable/Disable; Feature Flags; Crash-Isolation; Mod-Sandboxing; Versionsprüfung | 📋 | 21–32 ja |
| E-PA-031 | 27.09. | 35#806A | Build-Trennung: Basisversion, interne Entwicklungsversion, Entwicklungsversion mit Adult-Paket, später Distributionsversion | 📋 | 31 ja |
| E-PA-032 | 25.09. | 35#716A | Kein versteckter, nur per Flag deaktivierter Adult-Content im Basis-Build | 🧭 | Plattformaussage ungeprüft |
| E-PA-033 | 25.09. | 35#716A | Inhalte nach Store/Land/Altersfreigabe ein- und ausschließbar bauen | 🧭 | Architektur-Lock vorgeschlagen |
| E-PA-034 | 31.08. | K§16 | Entwickler-Mod bekommt keine Sonderrechte; gleiche öffentliche Schnittstellen | 📋 | PS 15 |
| E-PA-035 | 28.08. | 28#25A | Moddbar: Definitionen, Animationen, Presets, Tags, Profile, Übergänge, Kamera/Audio/Dialog, Umgebung, Oberflächen, Kompatibilität | 🧭 | Entwurf, in [Modul 02](02-Architektur-Pakete-Technik.md#7-modding) |
| E-PA-036 | 10.09. | 35#176A | Community-Mods ohne unbeschränkten Internetzugriff; Offline-first; `<GameRoot>/mods/` als lokale Wahrheit | 📋 | Hard Lock im Modding-Block; Hosting provider-unabhängig |

---

## 4. IA: Interaktion, Taxonomie, Ablauf

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-IA-001 | 10.09. | 35#189E | Eskalationsstufen als Zustände/Varianten, nicht als einfacher Level-Schalter | 📋 | |
| E-IA-002 | 10.09. | 35#187E | NPC↔NPC-Interaktionen in der Welt; Zuschauen und Beitreten möglich | 📋 | |
| E-IA-003 | 10.09. | 35#187E, 35#189E | Beobachter ist eine echte Rolle mit eigener KI/Gedächtnis; spontanes Beitreten möglich; Verlassen möglich | 📋 | 18–20 ja |
| E-IA-004 | 10.09. | 35#189E | Rollenabhängige Grenzen („dieselbe Handlung in Rolle A anders als in Rolle B“) | 📋 | Frage 16: Dev „verstehe ich nicht“, Assistent erklärte, Folgeantwort „ja“ (35#221 Frage 2) |
| E-IA-005 | 10.09. | 35#190A, 35#189E | Registry ohne C++-Änderung für neue Kategorien (Mod-Editor) | 📋 | Frage 24 „genauer bitte“ → erklärt → Empfehlung ja |
| E-IA-006 | 11.09. | 35#233E | Location-Tags intern, Spieler sieht sie nicht | 📋 | |
| E-IA-007 | 11.09. | 35#233E, 35#236A | Sicherheit/Polizei/NPC-Reaktionen hängen vom Ort ab (World Rules/Venue Policy), nicht vom Adult-System | 📋 | Entwickler fragte „was hat das mit adult zu tun“, dann „Ok passt jetzt mit 14“ |
| E-IA-008 | 11.09. | 35#233E, 35#234A | Nicht unterstützte Mehrpersonenkonstellation: abwechseln/Beobachter/getrennte Varianten statt kaputter Animation | 📋 | „ok, aber logisch, dann wechselt man sich ab“ |
| E-IA-009 | 11.09. | 35#233E | Locations merken sich Zustände (Besitzer, Zugang, Kameras, Beschwerden) | 📋 | |
| E-IA-010 | 13.09. | 35#443A, 35#444E | Interaktion als laufende Session aus modularen Aktionen; Intent/Action getrennt von Animation/IK/Physik; jedes Mal anders; niemals Minispiel | 📋 | R10 Fragen 31–36, 100 |
| E-IA-011 | 13.09. | 35#444E | Nicht „10.000 Szenen hardcoded“, sondern modulares Szenensystem | 📋 | |
| E-IA-012 | 13.09. | 35#444E, 35#445A | Fehlende Ablehnung bei Positionsvarianten und Eskalation: Sitzungsrahmen | 🧭 | siehe E-SK-022 |
| E-IA-013 | 13.09. | 35#464E | Übergang: Verabredung → Heimweg/Ort → sinnvoller Übergang → Szene startet realistisch | 📋 | |
| E-IA-014 | 14.09. | 35#486E | Keine Cuts: die Welt wird weiter simuliert | 📋 | |
| E-IA-015 | 14.09. | 35#486E | Erwachsenen-Interaktion kann Social Intent „nur Sex“ sein (NPC offen vorausgesetzt) | 📋 | Entwickler „2 ja, aber wieso“ → erklärt |
| E-IA-016 | 14.09. | 35#485A, 35#486E | Kleidung stückweise, Ausziehen mit IK statt Verschwinden, Schmuck bleibt, Kleidung liegt dort, wo sie abgelegt wurde | 📋 | R13 11–20 (3–50 ja) |
| E-IA-017 | 14.09. | 35#485A | Kuss freeform; Berührung über Kontaktgraph; Reaktion personenabhängig | 📋 | R13 21–30 |
| E-IA-018 | 14.09. | 35#485A, 35#486E | Manuelle/orale/penetrative Interaktion läuft über dieselbe Engine; Rhythmus nicht konstant; NPC passt sich an | 📋 | R13 51–70 |
| E-IA-019 | 14.09. | 35#485A | Orgasmus nicht garantiert, individuell, nicht als Szenenende, mehrere möglich, Recovery-Zustand; Szene darf auch ohne enden | 📋 | R13 81–90 |
| E-IA-020 | 14.09. | 35#485A | Danach: Nähe, Gespräch, Schlaf, Duschen, Essen, Anziehen, gemeinsames Einschlafen, Morgen danach | 📋 | R13 91–98 |
| E-IA-021 | 14.09. | 35#485A, 35#486E | Keine isolierte Adult-Statistik als Verhaltenstreiber (kein SexSkill); Zähler nur für Statistik/Debug | 📋 | |
| E-IA-022 | 14.09. | 35#487A | Erfahrung besteht aus mehreren unabhängigen Bereichen; Neugier ist getrennt von Erfahrung | 🧭 | Entwickler „60 ja, sag genauer“, nach Erklärung 35#488 „Ja passt“ |
| E-IA-023 | 15.09. | 35#524E, 35#525A | Allgemeine Erfahrung plus Erfahrung je Bereich plus partnerspezifisch (`General + Category + Partner-Specific`) | 📋 | |
| E-IA-024 | 16.09. | 35#573A | Interaktion klassifiziert über orthogonale Dimensionen (Kategorie, Unterart, Beteiligte, Rollen, Körperbereiche, Intensität, Tempo, Kraft, Ort, Umgebung, Kleidung, Items, Öffentlichkeit, Risiko, Erfahrung, Präferenz, Partnerkontext, Aufnahme, Gesundheitszustand, Nachzustand, Gedächtnis) | 🧭 | |
| E-IA-025 | 16.09. | 35#573A, 35#574E | R16A 1–25: Mehrfachkategorien, Unterkategorien, Tags, mehrachsige Intensität, lokal pro Kontakt, dynamisch, Soll vs Ist, Orts-Master-Typen, kein Adult-Slot, variable Teilnehmerzahl, Rollenwechsel, Initiative je Aktion, Körperbereich-Taxonomie, Aktion = Typ + Zielregion + Parameter, Items als Teilnehmer, Kleidung als Zustand, Privacy Intent vs Entdeckungswahrscheinlichkeit, mehrdimensionales Risiko, dreistufige Erfahrung, PreferenceState, nachträgliche Klassifikation, orthogonale Dimensionen | 📋 | Dev: 1 ja; 2–4 ja; 5 ja; 6 ja; 7 ja; 8–9 ja; 10 ja; 11 ja; 12 dynamisch; 13–24 ja; 25 „kommt noch die Kategorien“ |
| E-IA-026 | 16.09. | 35#583A, 35#584E | R16B 26–50: 13 Basisfamilien (Kissing/Touch, Manual, Oral, Penetrative, Toys/Items, Impact, Restraint/Control, Sensory, Roleplay, Group, Observation/Recording, High-Risk/Extreme, Aftercare/Post-State), mehrfach zuordenbar; Pain als Dimension; Group als Modifier; Parallel Actions; High-Risk-Tag verbietet nichts | 📋 | Dev: 26–50 ja (42 „was ist das“ → erklärt) |
| E-IA-027 | 16.09. | 35#585A | Nicht-einvernehmliche Gewalt gehört ins Crime/Justice-System, nicht in die Adult-Preference-Taxonomie; extreme fiktive Szenarien nur als vereinbartes Rollenspiel | 🧭 | |
| E-IA-029 | 18.09. | 35#622A, 35#623E | Kategorien-Registry wird in 16I+ ausformuliert, Qwen erst danach als Lückenfinder | 📋 | Dev: „Machen wir 16 mal fertig und dann machen wir das mit Qwen“ |
| E-IA-030 | 20.09. | 35#642A, 35#644A | Registry-Schema (ID, Kategorie, Unterkategorie, Tags, Zielregionen, Teilnehmer, Körperteil/Item, Intensitätsachsen, Positions-/Ortsanforderungen, Reaktions-/Risiko-/Präferenz-/Gedächtnis-/Statistik-Hooks, Replay-/Modding-Metadaten) ist ziemlich fest; Kategorienamen nicht final | 📋 | Dev: „201–225 ja“, „waren nur ungefähr?“ → ja |
| E-IA-031 | 20.09. | 35#650A–654A | Unterarten und Reaktionen als eigene Zustände (Würgen, Retching, Übelkeit, Erbrechen, Rückfluss, Husten, Atemnot, Atempause, Keuchen, Tränen, Speichel, Schweiß, Hautrötung, Schmerz-/Lustlaute, Schreien, Stöhnen, Krämpfe, Zittern, Erstarren), gleichzeitig kombinierbar, eigene Flüssigkeits-Registry | 📋 | Dev 35#651: Gagging allein sei zu vage; später in 16K beantwortet |
| E-IA-032 | 20.09. | 35#678A, 35#679E | Konkrete Unterarten für Küssen, Lecken, Saugen (+Tiefe), manuelle/orale/penetrative Aktionen, Toys, Körperteil-Interaktion, Fokus-Bereiche; Granularitätsregel: eigener klar benannter Eintrag, sobald technisch/physiologisch/präferenzrelevant verschieden | 📋 | 301–325 ja |
| E-IA-033 | 23.09. | 35#686E, 35#688A | Orgasmus/Ejakulation getrennt; Fluid-System (Menge, Zielregion, Übertragung, Absorption, Trocknen, Reinigung) als **Spät-Feature** | 📋 | 326–349 ja; Spät-Einordnung vom Entwickler |
| E-IA-034 | 23.09. | 35#688A, 35#689E | Missing-Family-Audit als eigener Schritt am Ende von R16 | 📋 | „Ja machen wir das extra“ |
| E-IA-035 | 23.09. | 35#692A | Ort ist Umgebungskontext, kein Szenentyp; praktisch jeder geeignete Raum; Möbel/Fahrzeuge/Outdoor/Wetter/Untergrund über normale Systeme; Privacy-Score; Schall; Zeugenerkennung; Moebelbruch; kein Adult-Raum nötig | 📋 | 351–375 ja |
| E-IA-036 | 23.09. | 35#694A, 35#695E | Kombinierte Aktionen, Layer, Unterbrechungen, Failure States, Edge-Case-Prinzip; **Default-Lock nur für Adult: Wenn nichts anderes festgelegt, verhält sich alles wie in der Realität** | 📋 | Dev: „376–400 ja; zu genau geplant“ |
| E-IA-037 | 25.09. | 35#712A, 35#713E | Funktionen werden in Core/Important/Late Polish eingeteilt; Realismus-Default; datengetrieben; Shared Systems; kleiner Release-Core zuerst | 📋 | 426–437 ja |
| E-IA-038 | 25.09. | 35#742A, 35#743E | Pre-Audit: Hauptfamilien als Top-Level ja; Body-Focus als Target/Preference-Familie; Kleidung nur Tags/State; Power/Dynamic-Dimension; Sensation als Registry-Dimension; Fantasy vs Activity getrennt; Media Production eigene Kontextfamilie („anfangs klein und immer wachsend“); Commerce/Service als Vertragskontext; Physiologie-Outcomes noch genauer; Social Outcomes; Time Context abgeleitet; Participant Structure Solo/Pair/Group; Intensität nie Kategorie | 📋 | A1–A14 ja/ok, A5 „genauer“, A9 „noch genauer“ |
| E-IA-039 | 25.09. | 35#744A, 35#745E, 35#746A | Kandidaten für weitere Familien: Erotische Kommunikation, Remote/Digital, Orgasm Control/Teasing, Body Worship/Fetischfokus, Praise/Verbale Dynamik, Mutual/Synchronized Actions, Schwangerschaftsfantasie-Kontext, Fetischobjekte | 🧭 | Dev: „Die Sachen passen gut“; Assistent: nicht eingefroren, Audit offen |
| E-IA-040 | 28.08. | 28#25A | Root-Taxonomie (NonPenetrative, Manual, Oral, Vaginal, Anal, Solo, Mutual, MultiParticipant, MixedCategory, Transition, Rest, OtherSupported); Root > Subtyp > Variante; Profile Standard/Advanced/Hardcore/ExtremeSupported; ContactDepth/ContactComplexity | 🧭 | Entwurf; Interview ersetzt Teile (E-PA-012, E-IA-030) |
| E-IA-041 | 28.08. | 28#25A | Zustandsautomat mit Fehlerzuständen, Pause-Typen, Dialogfenstern, Interruption-Prioritäten, Skip, Stopp mit ImmediateSafeExit | 🧭 | Entwurf, in [Modul 03](03-Interaktion-Taxonomie-Ablauf.md) übernommen |

---

## 5. SO: Freeform-Engine, Solver, Kontakt

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-SO-001 | 11.09. | 35#281E | Körperteile derselben Person nie exakt gleich (z. B. Bein 0,5 cm länger); jeder Mensch etwas anders | 📋 | |
| E-SO-002 | 11.09. | 35#283E | Jede Person muss „realistisch nachgebaut“ sein, nicht aus drei Elementen | 📋 | |
| E-SO-003 | 12.09. | 35#287E, 35#289E | Im Akt müssen sich Muskeln und Gewebe beider Beteiligter realistisch bewegen, nicht übertrieben | 📋 | 1–8 ja; 9 „realistisch“; 10 egal, Mods können alles |
| E-SO-004 | 12.09. | 35#291E | 90 % Realismus automatisiert reicht (Muskelaktivierung aus Pose/Bewegung approximiert) | 📋 | |
| E-SO-005 | 12.09. | 35#292A | Ein einziges High-Fidelity-Körpersystem für das gesamte Spiel; Adult nutzt es nur intensiver/näher | 📋 | 39–40 „genau das gleiche System“ |
| E-SO-006 | 13.09. | 35#444E | Gleiches Menschensystem wie im Hauptspiel; Jiggle realistisch, auch im normalen Spiel | 📋 | R10 39, 66 |
| E-SO-007 | 13.09. | 35#446E, 35#447A | Freeform statt Positionsmenü: jede beliebige Stellung auch zwischen Positionen | 📋 | |
| E-SO-008 | 13.09. | 35#447A | Kette Pose/Intent → Kontaktgraph → Ganzkörper-IK → Körperproportionen → Kollision/Weichgewebe → Balance/Gewicht → Mikrobewegung | 🧭 | Entwickler „ja, korrekt“ (35#458 Frage 100) |
| E-SO-009 | 13.09. | 35#458E | Beginn: Position selbst wählen oder NPC positioniert sich; „wie umsetzen ist die Frage“ | ❓ | Frage 1 |
| E-SO-010 | 13.09. | 35#458E | Positionen als Solver-Hilfen, müssen nie benutzt werden | 📋 | Fragen 9, 10 |
| E-SO-011 | 13.09. | 35#458E | Biomechanischer Bewegungsbereich pro Gelenk individuell; Widerstand wächst allmählich; Knochen können brechen; Kleidung schränkt ein | 📋 | Fragen 11–20 |
| E-SO-012 | 13.09. | 35#458E | Schwerpunkt, Balance, Abstützen, Gewichtsdifferenz | 📋 | Fragen 21–30 (21 „wofür?“, erklärt) |
| E-SO-013 | 13.09. | 35#458E, 35#459A | Kontaktgraph mit Fläche, Kraft, Reibung, Material; Soft Tissue sekundär zum Skelett; Beschleunigung als Auslöser von Nachbewegung | 📋 | Fragen 31–47 (31, 41, 44 erklärt) |
| E-SO-014 | 13.09. | 35#458E | Physikqualität: dieselbe Simulation mit unterschiedlicher Genauigkeit (Low/Medium/High/Ultra/Cinematic); Low darf nicht fake aussehen | 📋 | Fragen 47–50 |
| E-SO-015 | 13.09. | 35#458E, 35#459A | Muskelbewegung aus Gelenkbelastung; Fett über Masse; Asymmetrie fließt in IK ein; Generator leitet Physikparameter ab | 📋 | 51–60 (57–59 „was ist IK“, erklärt) |
| E-SO-016 | 13.09. | 35#458E | Kontinuierliche Übergänge; Gewichtsverlagerung vor Armbewegung; Mikrovariation; deterministisch (Session Seed) | 📋 | 61–70 (70 „was genau“, erklärt); 66: Unerfahrene unbeholfener, Lautstärke steigt bei ungewohnt Härterem (auch bei Geübten) |
| E-SO-017 | 13.09. | 35#458E | Steuerung über relative Bewegungsbefehle, nicht Hotkeys; Controller vollständig; VR später | 🧭 | 71, 72: „weiß ich noch nicht“, Stick-Idee „zu oberflächlich“ |
| E-SO-018 | 13.09. | 35#458E | Optionales Pose-Overlay, standardmäßig aus; Setup-Befragung beim ersten Start | 📋 | 75, 76 |
| E-SO-019 | 13.09. | 35#458E | Relax-Solver, Halten, Feinjustage, einfache Schnellbefehle | 📋 | 77–80 (80 „wieso 3D-Animationsprogramm“, erklärt) |
| E-SO-020 | 13.09. | 35#458E | NPC nutzt denselben Solver; Prioritätslogik; Stärke entscheidet nie allein; **einvernehmliche Dominanz darf kräftig aussehen** | 📋 | 81–90; 85: manche wollen dominiert werden |
| E-SO-021 | 13.09. | 35#458E | Fallback-Stufen Micro-Correction → IK-Relaxation → Neuberechnung → harter Reset; Teleportieren gibt es nicht; harter Reset praktisch nie | 📋 | 91–93 |
| E-SO-022 | 13.09. | 35#458E | Debug-Logging und Pose/Contact-Debugger im Mod-Editor | 📋 | 94 („was ist problematisch“), 95 („was ist das“) erklärt |
| E-SO-023 | 13.09. | 35#458E | Engine nicht Adult-spezifisch (Umarmen, Tragen, Medizin, Wrestling); Teile übernehmbar, Adult erweitert | 📋 | 98–100 |
| E-SO-024 | 13.09. | 35#460E, 35#461A | Jiggle bei rhythmischer Bewegung; Sekundärbewegung hängt pro Körperteil von individueller Anatomie und Bewegungs-/Kontaktzustand ab, nicht pauschal von Größe | 🧭 | Assistent „klar locken“; Entwickler weiter in 35#462 |
| E-SO-025 | 13.09. | 35#464E | Zielhaltung beschreiben, NPC bewegt sich selbst; NPC wählt eigenen Weg; Zwischenanpassung möglich | 📋 | 101–110 |
| E-SO-026 | 13.09. | 35#464E | Körperphysik pro Gebiet (Masse, Steifigkeit, Dämpfung), links/rechts, Frequenz/Amplitude, Kontakt dämpft, Muskeltonus wirkt | 📋 | 111–120 |
| E-SO-027 | 13.09. | 35#464E, 35#465A | Körper nicht statisch: Gewicht, Schwangerschaft, Alter, Training verändern Form und Physik | 📋 | 120 |
| E-SO-028 | 13.09. | 35#464E | Druckverteilung, Kleidung als Kontaktschicht | 📋 | 121–126 |
| E-SO-029 | 13.09. | 35#464E, 35#465A | Kraft folgt der normalen Körperkraft; **keine** Adult-Sonderwerte | 📋 | 130: „welche Adult-Werte?“ → erklärt |
| E-SO-030 | 13.09. | 35#464E | Steuerung wie UFC-Spiele „nur genauer“; Pose-Menü „kann richtig gut sein“ | ❓ | 131, 132 |
| E-SO-031 | 13.09. | 35#464E | VR: gleiches System, mehr in der Hand, nicht per Controller | 📋 | 133–134 |
| E-SO-032 | 13.09. | 35#464E | Katalog bekannter Positionen darf bestehen, definiert aber keine Grenzen | 📋 | 135 |
| E-SO-033 | 16.09. | 35#590E, 35#591A | Mischformen und freie Übergänge; Gelenkigkeit als echte Voraussetzung; Möbel als Interaction Surfaces; Bett/Möbel kann bei Belastung zerstört werden | 📋 | R16C 51–75 ja |
| E-SO-034 | 17.09. | 35#610E | R16D 76–100: stufenlose Intensität, Geschwindigkeit ≠ Kraft, Amplitude, Beschleunigung, Analogsteuerung, Rhythmus aus Input, Rhythmus-Assist, NPC übernimmt, gemeinsame Bewegung, Widerstand, Körpergewicht, lokaler Druck, parallele Intensitäten, Toleranzkurven, Gewöhnung ohne Magie, sekundengenaue Reaktion, Event-Stream | 📋 | 76–100 ja (81: UFC-Kombo-Bibliothek einsehbar) |
| E-SO-035 | 18.09. | 35#619E | R16F 126–150: Gewebezustand, Elastizität, Compliance, Dämpfung, vorübergehende Dehnung, Langzeitanpassung mit Grenzen, Mikrotrauma, Verletzungsakkumulation, Heilungsstufen, Narben, Schmerzgedächtnis, Schmerztoleranz ≠ Schadensresistenz, Kontaktspannung, Scherung, Reibungsschaden, Belastungsdauer, Unified Body State | 📋 | 126–150 ja |
| E-SO-036 | 23.09. | 35#694A, 35#696A | Selbstkollision, Multi-Body-Kollision mit LOD, Failure States statt Erzwingen, ungeplante Kombinationen aus Core-Regeln lösen | 📋 | 376–400 ja |
| E-SO-037 | 24.09. | 35#706A | Interaction Scheduler (Balance/Kollision vor sekundären Aktionen), World-State-Persistenz kaputter Möbel, deterministische Layer, autoritativer Interaction-State, Importance-LOD, automatisierte Stresstests | 🧭 | Konsistenzcheck des Assistenten |

---

## 6. KO: Körper, Reaktionen, Physiologie

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-KO-001 | 11.09. | 35#280A, 35#281E | Body Profile Seed; kontinuierliche Körperparameter, Asymmetrien, Muskelmasse separat, Training/Gewicht/Alterung verändern; Schwangerschaft später im allgemeinen Life-Sim-System | 📋 | R10A 1–35 (Frage 14 „weiß noch nicht“) |
| E-KO-002 | 11.09. | 35#281E | Narben, Tattoos, Piercings persistent und individuell („ich will viele NPCs mit vielen Tattoos“) | 📋 | Fragen 15–17 |
| E-KO-003 | 11.09. | 35#281E | Haare und Bart wachsen; kein sofortiger Wechsel von Kurzhaar auf Langhaar | 📋 | Frage 27 |
| E-KO-004 | 11.09. | 35#281E | Make-up als Schicht; Wasserfestigkeit „keine Ahnung wie das in echt ist“ | 🧭 | Assistentenvorschlag WaterResistance/SweatResistance |
| E-KO-005 | 11.09. | 35#281E, 35#283E | Attraction wird auf dem tatsächlich generierten Körper/Gesicht berechnet; Body-System unabhängig vom Adult-Mod | 📋 | 33–35 ja |
| E-KO-006 | 11.09. | 35#283E | Skeleton-Standard heißt nicht Standard-Körper (36: „?“) | 📋 | Erklärung 35#285 |
| E-KO-007 | 12.09. | 35#289E | Atmung zunächst nicht sichtbar wichtig; eigener Herzschlag nur bei extremem Stress hörbar („Bumpern“) | 📋 | 13, 14 |
| E-KO-008 | 12.09. | 35#289E | Schweiß ja; Druckstellen/Kleidungsdruck kein Muss-Feature | 📋 | 15, 16 (niedrige Priorität) |
| E-KO-009 | 12.09. | 35#291E | Hautfalten, Rötung, Schweiß-Roughness: ja, nicht das Wichtigste | 📋 | 31–35 |
| E-KO-010 | 12.09. | 35#291E | Soft-Tissue-Profile je Körperzusammensetzung (25: „was genau?“, erklärt) | 📋 | |
| E-KO-011 | 12.09. | 35#294A–302A | Menschen entstehen aus einem Parameterraum plus Generator, nicht aus 500.000 Einzelmodellen; Tier S = Story, Player = Tier S dynamic; MetaHuman/CC5 später testen | 🧭 | Entwickler 35#295: Tier S = wichtige Story-Figuren, Spielerfigur beim Start erstellen, kein Geld für ein Modell |
| E-KO-012 | 12.09. | 35#299E, 35#300A | Population mit Plätzen und Zeitplänen; Neugeborene mit eigener Stable ID; Genetik; Wachstumskurve statt Modellwechsel; Gesicht altert; Lebensphasen; <18 für Adult nicht sichtbar | 🧭 | Entwickler 35#301: Haar/Bart grenzenlos (2 m), Babys feinere Kurven, entfernte NPCs weiter simulieren |
| E-KO-013 | 14.09. | 35#500E, 35#501A | Hip Dips und Beckenbreite aus Beckenknochen/Fettverteilung, nicht aus einem Slider | 🧭 | Entwickler nannte Hip Dips als Beispiel |
| E-KO-014 | 14.09. | 35#500E | Brustgröße kontinuierlich, auch sehr groß, mit Gewicht veränderlich; Sport-BH dämpft; Gesäß wächst durch Training; kosmetische Eingriffe | 📋 | 31–41 |
| E-KO-015 | 14.09. | 35#500E, 35#501A | Intime Anatomie genauso individuell wie Gesicht und Körper (kontinuierliche Parameter, Asymmetrie, Grenzbereiche, Mods erweitern Grenzen); Erweiterung im selben Character Creator | 📋 | 51–61 |
| E-KO-016 | 14.09. | 35#501A | Anatomische Veränderung durch reale Faktoren (Schwangerschaft, Alter, Verletzung, Eingriff, Heilung), **nicht** durch einen `usageCount` | 🧭 | Auf Entwicklerwunsch „Gebrauchsspuren“ (35#500) |
| E-KO-017 | 14.09. | 35#500E | Beschneidung: eher nicht, nur gesundheitlich; Welt „99,9 % christlich“; Anmerkung des Assistenten: realistische USA-Setting-Abweichung | ❓ | 62; Setting-Frage |
| E-KO-018 | 14.09. | 35#500E | Körperbehaarung/Grooming kontinuierlich, regional getrennt | 📋 | 63–75 ja |
| E-KO-019 | 14.09. | 35#502E, 35#503A | Seltene Erwachsene mit sehr extremen, ausdrücklich gewünschten Praktiken: Verletzungssystem greift, kein „weil gewollt passiert nichts“; Ausgestaltung nicht als Anleitung | 🧭 | Entwickler wollte das später genauer erklären |
| E-KO-020 | 14.09. | 35#486E | Körperliche Grenzreaktionen (Würgen, Übelkeit, Erbrechen) als physiologische Zustände, individuell; extremere Darstellung als konfigurierbarer Inhalt | 📋 | R13 59 „sehr wichtig“; Assistent 35#487 |
| E-KO-021 | 14.09. | 35#486E | Stimme: jede Person anders; Schreien möglich; Schmerz kann gewollt sein | 📋 | 76 |
| E-KO-022 | 17.09. | 35#611A, 35#612E | Atmung, Stimme, Mimik, Blick, Tränen, Würgereflex, Übelkeit, Muskelzustand, Orgasmus als Multisystem-Ereignis; Reaction Graph; Content-Toggle für starke Körperreaktionen | 📋 | 101–125 ja |
| E-KO-023 | 20.09. | 35#653E | Reaktionen sind eigene, kombinierbare Zustände; Flüssigkeiten getrennt (Konsistenz, Menge) | 📋 | 251–275 ja mit Ergänzungen |
| E-KO-024 | 20.09. | 35#676A, 35#677E | Pleasure-Map lokal, Schmerz+Lust gleichzeitig, Sensibilisierung, Desensibilisierung, Überstimulation, Mischreaktionen, Orgasmus-Auslösemodell, Orgasmus-Profile, Mehrfach, Refraktärphase, Afterglow, Muskelgruppen-Ermüdung, Krämpfe, Haltungsermüdung, Muskelkater, Schwellung, Blutergüsse, Oberflächenverletzung, kumulative Erholung, Alltagsfolgen, Selbstbehandlung, medizinische Eskalation, Prognose im Inspector, persistente Historie, Unified Outcome Layer | 📋 | 276–300 ja |
| E-KO-025 | 14.09. | 35#512E, 35#513A | Piercings an äußeren Genitalregionen plausibel; innere vaginale Piercings keine Standardkategorie | 🧭 | Entwickler „ich kenne mich nicht aus“ |
| E-KO-026 | 15.09. | 35#524E | Eigene Items physisch vorhanden, nicht spawnbar; Shops/Webshops; Materialphysik; Größen kontinuierlich | 📋 | 21–23, 44–49 |
| E-KO-027 | 25.09. | 35#706A–707E | Risiko-Pipeline Action+Intensity+Contact+Anatomy+Body State+Environment → Mechanical Load → Sensation/Pain → Damage/Injury → Reaction → Recovery/Medical → Memory; für Adult, Combat, Sport, Unfälle, Medizin gemeinsam | 📋 | 401–425 ja |
| E-KO-028 | 25.09. | 35#706A | Risiko-UI subtil, keine permanenten Prozentwerte; Developer Health Inspector | 📋 | 421–422 ja |

---

## 7. GE: Gesundheit, Verletzung, Fortpflanzung

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-GE-001 | 15.09. | 35#528E | Verletzungssystem bleibt aktiv, auch bei gewolltem Risiko; Schmerzreaktionen, Blutung, Komplikationen | 📋 | 44/45 ja |
| E-GE-002 | 15.09. | 35#541A, 35#542E | Verhütung als Items/Zustände mit unterschiedlicher Wirksamkeit; NPC mit eigener Methode; Gespräch darüber; Fruchtbarkeit individuell; abstrahierter Zyklus; Schwangerschaft nur bei passender Biologie, langfristiger Weltzustand, Test-Item, medizinische Versorgung | 📋 | 126–136 ja; Kind (Junge/Mädchen) möglich |
| E-GE-003 | 15.09. | 35#542E | Hormonelle Verhütung kann Persönlichkeit verändern („die ganze Persönlichkeit“) | 🧭 | Assistent 35#543: Stimmung/Libido/Energie, nicht die ganze Persönlichkeit |
| E-GE-004 | 15.09. | 35#542E, 35#543A | **Kein detailliertes Infektionssystem (STI) in der Basisversion** | 📋 | 137–139 **abgelehnt**: „das brauchen wir nicht, es ist eh schon genug“; Architektur lässt Erweiterung zu |
| E-GE-005 | 15.09. | 35#542E | Wissen über den eigenen Gesundheitsstatus, Hygiene, Duschen als normale Handlung, Flüssigkeiten über Material-Infrastruktur, Bettwäsche/Möbel, Erschöpfung | 📋 | 140–148 ja |
| E-GE-006 | 15.09. | 35#542E | Kumulative Verletzungen: Folgesitzungen verschärfen; Heilung über Wochen; „es gibt keine Grenzen vom Spiel aus, aber der Charakter entscheidet“ | 📋 | 149 |
| E-GE-007 | 15.09. | 35#542E | Adult Health nie ein separates Minigame | 📋 | 150 |
| E-GE-008 | 15.09. | 35#543A | Praktiken mit Atem-/Halskontrolle bis Bewusstlosigkeit: seltene Hochrisiko-Präferenz im Gesundheitssystem; keine How-to-Mechanik; Awareness-Stop greift | 🧭 | Entwickler 35#542/544 wollte „Stopp-Logik greift nicht“; Assistent lehnte ab (35#545), siehe E-XX-006 |
| E-GE-009 | 18.09. | 35#619E | Gewebe-/Belastungs-/Heilungsmodell auch im normalen Spiel anwendbar (Peitschenverletzungen als Beispiel) | 📋 | 126–129 ja |
| E-GE-010 | 25.09. | 35#706A | Erfahrung ≠ Widerstandsfähigkeit; Schmerztoleranz getrennt; dynamische Verletzungsschwelle; Eskalation Minor→Major; keine Session-Heilung; Sichtschäden aus Body State; Notfallerkennung; Severe State → normales Medizinsystem | 📋 | 401–425 ja |
| E-GE-011 | 15.09. | 35#541A | Schwangerschaft im Weltzustand (Körper, Gewicht, Bewegung, Kleidung, Arbeit, Beziehungen) | 📋 | 134 |
| E-GE-012 | 13.09. | 35#464E | Gewicht/Schwangerschaft verändern Brüste und Physik; auch umgekehrt | 📋 | R11B 120 |

---

## 8. NP: NPC, Präferenzen, Beziehungen

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-NP-001 | 10.09. | 35#187E | Jede erwachsene wichtige Figur hat eigene Präferenzen und Grenzen; Frauen sind nicht pauschal bereit (Entwickler: „beim Richtigen machen sie alles“) | 🧭 | Assistent 35#188: individuell, nicht pauschal; Entwickler widersprach nicht |
| E-NP-002 | 10.09. | 35#187E | Vorlieben ändern sich durch Beziehung und Erfahrung; Hard Limits können sich ändern | 📋 | siehe E-SK-005 |
| E-NP-003 | 10.09. | 35#187E | Figuren zeigen Anzeichen wie in der realen Welt; Erinnerung an frühere Interaktionen | 📋 | |
| E-NP-004 | 11.09. | 35#227E | Präferenzprofil veränderbar; Persönlichkeit/Erfahrung getrennt; Überreden möglich; Skala neugierig bis Hard Limit; Spieler sieht Profil nicht (nur Debug) | 📋 | R4 1–15 |
| E-NP-005 | 11.09. | 35#227E | Archetypen plus Zufallsvariation bei 500.000 NPCs; handkuratierte Profile nur für Mod-unterstützte Figuren | 📋 | 13, 14 |
| E-NP-006 | 11.09. | 35#228A | Überreden verschiebt Neutral/Situativ, nicht ein aktuelles Hard Limit | 🧭 | |
| E-NP-007 | 11.09. | 35#229E | Getrennte Werte Attraction/Trust/Affection/Romance/Comfort/Familiarity; ONS auch mit niedrigem Trust; Aussehen reicht bei manchen; Chemie positiv/negativ; Eifersucht; offene/Poly-Beziehungen; Fremdgehen verschlechtert | 📋 | R5 1–12 |
| E-NP-008 | 11.09. | 35#229E, 35#230A | „Mit jedem NPC Sex haben können“; Assistent: nicht garantiert verfügbar | 🧭 | |
| E-NP-009 | 11.09. | 35#229E | „Nein“ ohne Änderungschance abgelehnt, weil Desinteresse oft nur gespielt | 🕰️ | Assistent 35#230: nicht als Systemregel; aktuelles Nein bleibt Nein |
| E-NP-010 | 11.09. | 35#229E | Starke gemeinsame Erlebnisse können seltene Wendungen erzeugen, nicht in der Hauptstory | 📋 | |
| E-NP-011 | 11.09. | 35#231E | NPC initiiert, flirtet, schlägt Orte vor, kontaktiert später; Aussehen/Outfit erster Trigger; spontane Begegnungen Kernziel; Casual-/Beziehungstypen; Faktorenmix | 📋 | R6 |
| E-NP-012 | 11.09. | 35#261E | Präferenz-DNA aus Seed, an globale NPC-ID gekoppelt, nur am Anfang erzeugt | 📋 | R9A 1–45 |
| E-NP-013 | 11.09. | 35#261E | Initial Attraction nur bei Dates; bei ONS eher weniger | 🧭 | Assistent 35#262: auch bei ONS beibehalten; Entwickler „Ok“ |
| E-NP-014 | 11.09. | 35#261E | Regionale Variation (leicht andere Stimme/Aussehen); „keine Immigranten, nur Leute von dort oder aus den USA“ | ❓ | Assistent widersprach (35#262), Entwickler „Ok“ (35#263) ohne Klärung |
| E-NP-015 | 11.09. | 35#261E | ONS genauso genau gespeichert wie Beziehung | 📋 | R9A 40 |
| E-NP-016 | 11.09. | 35#269E–273E | Langzeitveränderung, Anker-Erinnerungen, Wiedersehen, Gossip, Reputation, Alterung, Tod, Zeitreisen über Temporal-World-State-System | 📋 | R9B.1/2/3 ja |
| E-NP-017 | 11.09. | 35#271E | Gutes Date führt nicht zwingend zu Adult („kann sein, aber oft mit“) | 📋 | R9B.2 16 |
| E-NP-018 | 11.09. | 35#271E | ONS kann einmalig bleiben oder zur Beziehung werden (nicht „nein, vorbei“) | 📋 | R9B.2 21 |
| E-NP-019 | 15.09. | 35#524E, 35#525A | Mehrdimensionales Präferenzprofil; Stärke; partner-/situationsabhängig; Neugier; Erfahrung nach Bereichen; Veränderung; Kernvorlieben; Verteilungen; Wissensstufen; falsche Annahmen; Dialog; Grenzen gleich tief; Intensität ≠ Praktik; Dominanz nicht binär; Initiative ≠ Dominanz; Roleplay-System; Kleidungs-/Material-/Sinnesvorlieben | 📋 | R15A ja (Ausnahmen 3, 4, 6, 9, 10, 18, 21) |
| E-NP-020 | 15.09. | 35#524E | Extrem unterwürfige Figur lässt einem Partner „alles“ zu und einem anderen kaum etwas | 🧭 | Assistent 35#525: partner-/situationsabhängig; Kapazität, Beteiligung, harte Grenzen bleiben |
| E-NP-021 | 15.09. | 35#524E | Entwickler will Präferenzen im Entwicklungs-/Debug-Modus sehen („nur ich“) | 📋 | Adult Debug Inspector (35#525) |
| E-NP-022 | 15.09. | 35#526E | Gruppen: paarweise Beziehungen; Beteiligungsgrade; „sie soll herkommen“ möglich | 📋 | R15B 26–31 |
| E-NP-023 | 15.09. | 35#542E | Sexuelle Libido/Intensität schwankt; Zeit nicht zu schnell | 📋 | 102 |
| E-NP-024 | 15.09. | 35#544E, 35#546E, 35#548E | Langzeit-Begleitvertrag: NPC kann kündigen, tut es „fast nie“; Spieler soll eher kündigen | 🧭 | K6 in [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md) |
| E-NP-025 | 15.09. | 35#550E, 35#552E | Sexuelle/romantische Orientierung getrennt; datengetriebener Zustand; **keine LGBTQ-Inhalte in der Spielwelt** (Setting-Entscheidung) | 📋 / ❓ | 151 ja; 152: erst Scherz, dann „ich will LGBTQ nicht im Spiel haben“; Fragen zum Modell offen (K5) |
| E-NP-026 | 15.09. | 35#551A | Fiktive Region demografisch stark heterosexuell; Gewalt wegen Orientierung keine positive Weltregel | 🧭 | Assistent; Entwickler widersprach nicht |
| E-NP-027 | 15.09. | 35#550E | R15G 153–175 ja: Merkmale vor Label, Liebe vs Lust, Paar-Dynamik, Libidounterschiede, Spontanität, Dauer, Pausen, Rhythmus, Orgasmusprofile, Edging, Orgasmuskontrolle in Dominanz-Dynamik, Mehrfach, Geruch, Parfüm, Stimme, Haare, Nägel, extreme Looks, Lieblingsgegenstand, Solo-/Partnerpräferenz, Fantasien, partnerspezifische „Offenheit“ | 📋 | |
| E-NP-028 | 15.09. | 35#552E | R15H 176–200 ja: Kompatibilität aus Faktoren, Paare mit Unterschieden, Partnerwissen, Attraction folgt Aussehen, Bindung gleicht aus, Veränderung für Partner optional, Vorfreude, Abstinenz individuell, kein Sex-Cooldown, kein Sex-Counter, verborgene Vorlieben, Beruf ≠ privat, Vertragsprofil, Companion-Leistungen | 📋 | 191: zusätzlich globales Statistiksystem |
| E-NP-029 | 18.09. | 35#625E | R16G 151–175 ja: Partnerprofil, Attraction ≠ Preference, Trust, Bindung, aktuelle Lust, Initiative, Lernen durch Beobachtung/Gespräch mit Confidence, falsches Lernen, Erfahrungs-Gedächtnis, Entdeckung, Stärkenänderung, Partnerassoziation, Gewohnheit, Langeweile, Neugier, Stimmung/Stress, Konflikt, Eifersucht an Personen, NPC-Vorschläge mit Kontextprüfung, Persönlichkeitsstabilität, Persistenz, Modding | 📋 | |
| E-NP-030 | 26.09. | 35#785E | NPCs sollen ein Leben haben: Tattoos/Body Mods/Looks ändern sich über Jahre unabhängig vom Spieler | 📋 | R17I 137, 138 |

---

## 9. LE: Gedächtnis, Leben, Simulation, Speicher

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-LE-001 | 11.09. | 35#279E | Welt läuft nur, wenn man spielt (Singleplayer); Online anders | 📋 | R9C 11 |
| E-LE-002 | 11.09. | 35#279E | Globale NPC-ID; Simulationsstufen; Tier 3 nur Ereignisschritte; deterministisch aus NPC-ID+Welt-Seed+Zeitfenster; relevante Begegnung friert Zustand ein; Delta-Saves; Memory-Klassen; Migration | 📋 | R9C ja (5 „nicht der ganze Weg“, 36 „solange alles einzeln erinnert“) |
| E-LE-003 | 11.09. | 35#279E, 35#280A | Save-Größe: Dev „eher kleiner“; Assistent: Ziel 50–150 MB normal, 150–300 MB extrem | 🧭 | Zahlen sind Vorschlag |
| E-LE-004 | 11.09. | 35#283E | Kaum Blueprints im Projekt; eigenes Savegame-System | 📋 | |
| E-LE-005 | 12.09. | 35#301E | Entfernte NPCs werden weiter simuliert (Friseur, Tattoo, Altern) | 📋 | |
| E-LE-006 | 13.09. | 35#473A | Entity-Stufen: Simulated Entity / Active Simulation Entity / Fully Materialized Actor | 🧭 | |
| E-LE-007 | 13.09. | 35#461A | Persistenz-Datei-/Ordnerstruktur (manifest, player, world, regions, npc, relationships, cases, vehicles, properties, economy, temporal, mods) | 🧭 | |
| E-LE-008 | 11.09. | 35#262A | Gedächtnisstufen Minimal Record / Known NPC / Detailed Persistent / Full Persistent | 🧭 | Entwickler „40 ok, aber ONS gleiche Genauigkeit wie Beziehung“ |
| E-LE-009 | 15.09. | 35#552E | Globales Lifetime-Statistiksystem unabhängig von Adult, zentral über Ereignisse | 📋 | Core-Entscheidung; eigene Runde später |
| E-LE-010 | 18.09. | 35#609A | Interaction Event Stream, den Memory, Statistics, Replay, Audio, Animation, Health, Modding abonnieren | 🧭 | 100 ja |
| E-LE-011 | 15.09. | 35#540E, 35#541A | Spielzeit im Szenenverlauf nahe 1:1 (nicht 30 min = 12 h); Kalender/Tagesabläufe eigene Planung | 🧭 | Forderung des Entwicklers, Zahl vom Assistenten |
| E-LE-012 | 15.09. | 35#530E, 35#531A | Gebäudeplanung nach Adult-Block (hierarchische Weltplanung) | 🧭 | |

---

## 10. TA: Tattoos, Piercings, Körpermodifikation

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-TA-001 | 11.09. | 35#281E | Viele NPCs mit vielen Tattoos erwünscht | 📋 | |
| E-TA-002 | 12.09. | 35#299E | Stark Tätowierte sind eher an Tattoo-Shops und passenden Orten; Tattoos ändern sich im Leben | 📋 | |
| E-TA-003 | 14.09. | 35#494E | Tattoos nur teilweise festgelegt; eigene Planung nötig | 📋 | |
| E-TA-004 | 14.09. | 35#500E | Tattoos auf jeder Körperstelle, auch Augen und Körperöffnungen/Mundinnenseite; 0 bis praktisch 100 %; Sleeve aus Motiven oder Gesamtdesign; Lücken auffüllbar; mehrere Künstler/Stile | 📋 | R14A 76–81 |
| E-TA-005 | 14.09. | 35#500E | Studios in der Metropole „sicherlich 20–30 oder mehr“ | 🧭 | Zahl von Dev als Schätzung |
| E-TA-006 | 14.09. | 35#500E | Künstlerqualität variiert (Gefängnis-Tattoos schlecht); Fading erst nach Jahren; UV; Hautalterung; Deformation bei Gewicht/Schwangerschaft; Cover-up; Laser; Termine | 📋 | 83–99 |
| E-TA-007 | 14.09. | 35#501A | Tattoo-Lebenszyklus frisch → Heilung → settled → Fading → Touch-up/Cover-up/Laser | 🧭 | Entwickler: Fading nicht nach 2 Wochen, sondern Jahre |
| E-TA-008 | 14.09. | 35#501A | Künstlerprofil mehrdimensional; Studio-Typen | 🧭 | Dev „84: wie genau meinst du das“ → erklärt |
| E-TA-009 | 14.09. | 35#502E, 35#503A | Je mehr Tattoos, desto seltener zufällig, außer an passenden Orten; Webseiten-Baukasten im Spiel (nicht Unreal-Blueprint) | 🧭 | Spielerseiten „stark vereinfacht“ |
| E-TA-010 | 14.09. | 35#512E | Piercings auch an anderen Stellen; billig ≠ schlecht; Material; Heilung; Dehnung; Intimpiercings; Body Mods (Split Tongue, Scarification, subdermal); Kliniken für kosmetische Chirurgie | 📋 | R14B 101–140 |
| E-TA-011 | 14.09. | 35#513A | 14C Konsolidierung (Tattoos, Piercings, Eingriffe, Studios & Internet) | 🧭 | vom Assistenten zusammengefasst; Entwickler 35#514 „Gute Nacht“ |
| E-TA-012 | 15.09. | 35#528E | Nagelstudios (Hände, Füße), Nagellack im Supermarkt | 📋 | |
| E-TA-013 | 16.09. | 35#590E, 35#591A | Eigene Tattoo-Runde (90–95 % Tattoos) | 📋 | Entwickler-Wunsch |
| E-TA-014 | 26.09. | 35#749E, 35#750A | R17A: Tattoo als persistentes Objekt mit Geschichte; freies Placement; Oberflächen-Koordinaten; kontinuierliche Coverage; Überlappung; Layer History; Negative Space; Filler; Sleeve-Komposition; Patchwork; Full-Body-Projekte; Symmetrie bewusst; Anatomie beeinflusst Design; Körperveränderung dehnt; Narben; Dichte; Autonomie; Creator | 📋 | 1–12, 14–20 ja; 13 nach Rückfrage ja |
| E-TA-015 | 26.09. | 35#749E, 35#750A | **Vollständiger Tattoo-Editor und Nacktheit im Hauptspiel**, nicht hinter der Adult-Mod | 📋 | 20: Dev fragte „wieso?“; Assistent 35#750 stimmte zu |
| E-TA-016 | 26.09. | 35#749E | Verteilung: Land selten, ältere Menschen ohne Gefängnis selten, junge Erwachsene in der Stadt häufiger, einzelne Vollkörper („Tattoo-süchtige“) | 📋 | 18 |
| E-TA-017 | 26.09. | 35#761E | R17B 21–40: Stile frei, Blackout möglich, Vorschau beim Tätowierer, Realism, Yakuza/Mafia-Stile, Fine Line (oft junge Frauen), Portraits „kann sehr gut werden“, Preview, Freehand, Mischstile, Vollkörper-Masterplan, „wirklich alles tätowierbar“ | 📋 | 27/28 nach Rückfrage ja |
| E-TA-018 | 26.09. | 35#762A | Technik `Design → Body-Surface Mask → Pigment Layer → Skin Shader`; Virtual Texturing / Body-Space-Layers | 🧭 | Dev „wie wir das machen, weiß ich nicht“ |
| E-TA-019 | 26.09. | 35#767E | R17C 41–60: Artist-Skills, Spezialisierung, Handschrift, Karriere, Preisformel inkl. Standort, Fixpreis/Stundensatz, **Wartezeit 1–2 Spielwochen**, Buchung, Beratung, Anzahlung, Walk-ins, Studio-Reputation, Typen, Hygiene separat, schlechte Studios, Preis ≠ Qualität, Empfehlungen, Portfolio, berühmte Kunden, Artist-Wechsel (sichtbarer Bruch möglich) | 📋 | 41–59 ja; 45, 47, 60 mit Zusatz |
| E-TA-020 | 26.09. | 35#769E | R17D 61–80: Sitzungen (nicht zwingend, fast immer), Zeitraffer (nicht 5 h Echtzeit), Schmerz nach Körperstelle, Heilungsphasen, Aftercare, Kleidungskontakt, Wasser/Sonne, Touch-up, Fading, Refresh, Historie | 📋 | |
| E-TA-021 | 26.09. | 35#771E | R17E 81–100: selbsterklärende reale Tattoo-Logik gilt automatisch | 📋 | |
| E-TA-022 | 26.09. | 35#775E | R17F 101–112: Verteilung nach Alter/Stadt-Land/Subkultur/Beruf, Vollkörper in Metropole „mehrere, nicht einer“, Studiodichte, Identität, Konkurrenz, Trends, NPC-Entscheidung, Reaktionen, berühmte Studios; **Subkulturen inkl. extremistischer Symbole** | 📋 / ❓ | 103: Symbolik rechtlich offen |
| E-TA-023 | 26.09. | 35#776A, 35#777E | Baseline-Prozentverteilung Metropole/Städte/Land (Assistentenvorschlag) | 🧭 | Dev nur „passt“; Zahlen nicht einzeln bestätigt |
| E-TA-024 | 26.09. | 35#779E | R17G 113–122: Composer/Body-Layer, Haut-Integration, LOD (10–20 m noch klar), Generator mit Logik („manche haben einfach Lust“), Komposition zuerst, virtueller Artist, Vorlagen wiederverwendbar, **Import eigener Bilder als Pflichtfeature**, Modding, Asset-QA mit hoher Auflösung | 📋 | |
| E-TA-025 | 26.09. | 35#783E | R17H 123–134: Piercings an plausiblen Stellen, Arten, Material, Heilung, Wechsel/Entfernen, Dehnen, Narben, Intimpiercings als normale Body-Mod-Kategorie, Scarification/Branding/Split Tongue/Subdermal, selten und subkulturell, Spezialstudios, Creator bietet Piercings | 📋 | |
| E-TA-026 | 26.09. | 35#785E | R17I 135–141: Realismus-Default fürs ganze Spiel, keine künstlichen Grenzen, NPC-Autonomie, dynamische Population, Performance-Priorität, gemeinsamer Workflow, Abschlussregel | 📋 | |
| E-TA-027 | 14.09. | 35#500E | Kosmetische Eingriffe (BBL, Brust, Gesicht) als Weltmechanik mit Kliniken | 📋 | 41, 131–136 |

---

## 11. WE: Welt, Orte, Branche, Wirtschaft

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-WE-001 | 31.08. | K§19 | Adult-Service-NPCs als ein Teil von Nachtleben/Stripclubs/Bars; nicht jeder Arbeiter bietet jeden Service; wiederkehrende NPCs mit Namen, Zeitplan, Persönlichkeit, Grenzen, Präferenzen, Preisen, Erinnerungen | 📋 | Kanon |
| E-WE-002 | 31.08. | K§19 | Glamouröse Art Direction erlaubt, mit visueller Vielfalt; Relationship-NPCs behalten harte Grenzen; Transaktionen über normale Wirtschaft | 📋 | Kanon |
| E-WE-003 | 29.08. | 23#569E | Frauen, die „alles anbieten“, wenn man keine Partnerin findet (Stripclub/Bar/anderswo); Geldsystem | 📋 | Grundlage von K§19 |
| E-WE-004 | 31.08.–03.09. | 23#706A, 23#1038A | Nachtleben als breite erwachsene Weltschicht (Stripclubs, Adult Clubs, Escort-/Companion-Agenturen, je Jurisdiktion legale/illegale Sexwork-Venues, luxuriöse/zwielichtige Betriebe, unabhängige über fiktive Websites, Underground) | 📋 | Kanon-Kandidat WORLD_DESIGN_BLOCKS_1_TO_7 |
| E-WE-005 | 11.09. | 35#233E | Öffentlichkeit mit Sicht-/Hörweite echter NPCs; Beobachter-Reaktionen; Sonderorte (Adult-Hubs) | 📋 | R7 |
| E-WE-006 | 15.09. | 35#526E | Stripclubs „essentiell“, in der Metropole 15–30; ein riesiger mehrstöckiger Betrieb; Kleinstädte weniger | 🧭 | Zahlen als Schätzung des Entwicklers; Assistent 35#527: drei Betriebstypen (Stripclub, Nightclub/Adult Club, Brothel/Escort) und ein großer Komplex |
| E-WE-007 | 15.09. | 35#526E | Performer als normale persistente NPCs; Karriere; Creator-Plattform; Spieler als Creator; gemeinsame Produktion; Studios; Wirtschaft; mehrdimensionale Reputation | 📋 | R15B 33–40 |
| E-WE-008 | 15.09. | 35#526E | Mehrere Adult-Websites im Spiel-Internet; Seiten pro Betrieb | 📋 | 41 |
| E-WE-009 | 15.09. | 35#524E, 35#526E | Pornoseiten- und OnlyFans-ähnliche Plattformen in der Spielwelt; Performer-Persönlichkeit ≠ privat verfügbar | 📋 / 🧭 | Entwickler: „Performerinnen sind richtig extrem“; Assistent 35#525: Beruf ≠ Verfügbarkeit |
| E-WE-010 | 15.09. | 35#528E, 35#529A | In-Game-Videos: immer exakt dasselbe Video (feste Replay-ID, Cache rekonstruiert); Speicher durch Metadaten + Replay-Daten | 📋 / 🧭 | Entwickler: „immer das gleiche Video“; Technik vom Assistenten |
| E-WE-011 | 15.09. | 35#527A | Aufnahme und Veröffentlichung sind getrennte Zustimmungen; privat speichern / mit Partner teilen / Creator-Seite / kommerziell; Meinungsänderung vor Veröffentlichung | 🧭 | 62–64 (Entwickler 35#528: 56–72 ja) |
| E-WE-012 | 15.09. | 35#528E | Fans wollen Foto oder Chance auf Sex („73 ja“); Ruhm mit Nachteilen | 📋 | |
| E-WE-013 | 15.09. | 35#530E, 35#531A | Straftaten ohne künstliche Liste; schwere Sexualstraftaten als Welt-/Konsequenzthema, ohne Ausführungsmechanik | 🧭 | K3 |
| E-WE-014 | 15.09. | 35#532E, 35#533A | Strafsystem realistisch/logisch (Kette Tat → Verfahren → Haft); Gefängnis als soziale Simulation; Selbstverteidigung geschlechtsunabhängig; Fraktions-/Moralsystem statt Hardcode | 📋 / 🧭 | Entwickler wollte „konservative Welt“; Assistent: datenbasierte Mehrheitskultur |
| E-WE-015 | 15.09. | 35#536E, 35#537A | Welt-Simulationsprinzip: logisch statt emotional (Tat vor Identität, Kontext, Verhältnismäßigkeit, Gerüchte ≠ Fakten) | 📋 | Entwickler 35#538 „Ja genau“ |
| E-WE-016 | 15.09. | 35#540E, 35#549A | Bordell-Matching-Website, Langzeitbuchung, Vertrag; Pass-/Altersprüfung | 🧭 | K6 |
| E-WE-017 | 15.09. | 35#540E | Preisniveau verschieden; Bezahlung unterschiedlich gut; Performer-Karriere | 📋 | 110–120 ja (117: Scheitern selten) |
| E-WE-018 | 15.09. | 35#552E | Globales Statistik-Menü mit Wert „wie viele Frauen (adult)“ usw. | 📋 | 191 |
| E-WE-019 | 25.09. | 35#742A, 35#743E | Medienproduktion startet klein und wächst | 📋 | A7 |
| E-WE-020 | 23.09. | 35#692A | Venues mit eigenen Raumtypen und Hausregeln; Studios mit Kamera/Licht/Set/Crew; Workplace mit höherem Risiko; Orts-Gedächtnis | 📋 | 351–375 ja |
| E-WE-021 | 28.08. | 28#25A | Zeugen aus Sichtlinie, Entfernung, Raum, Türen, Wänden, Audio, Licht, Sichtbarkeit, Privatsphäre, Beziehung, Fraktion | 🧭 | in Interview bestätigt (E-WE-005, E-IA-035) |

---

## 12. ST: Steuerung, Oberfläche, VR, Kamera, Audio

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-ST-001 | 26.09. | 35#791E, 35#793E | Analoge Stick-Steuerung statt Button-Spammen („spammen wäre zu GTA“) | 📋 | 7 ja |
| E-ST-002 | 26.09. | 35#791E, 35#796A, 35#797E, 35#799E | Adult nutzt **ein Hybrid-Control-System**: Aktions-/Kombinationshilfen plus analoge Echtzeitsteuerung; Kampf behält separates UFC-artiges Lernsystem | 📋 | Fragen 1, 2, 4, 6 offen → Hybrid; Entwickler „ok dann weiter das passt dann“ |
| E-ST-003 | 26.09. | 35#791E, 35#792A | Combat-Lernsystem (Basismoves früh, komplexe lernen, Trainer/Gyms, Upgrades „kein normales Upgradesystem“) | 📋 | **nicht** Adult; Core-Combat-Block |
| E-ST-004 | 26.09. | 35#791E | Aktionsbibliothek erst nach Freischaltung vollständig sichtbar; Namen als Hinweis | 📋 | 3, 5 |
| E-ST-005 | 26.09. | 35#793E | R18A 8–20 ja: Intensitätsmodifikator, Körperbereichswahl, Smart Context, parallele Aktionen im UI, HUD-Stufen, Minimal-HUD filmisch, Remapping, Konfliktwarnung, Haptik, Kamera, First-Person-Stabilisierung, Barrierefreiheit, Kontrollprofile | 📋 | |
| E-ST-006 | 26.09. | 35#795E | VR fühlt sich in Bewegung wie echt an; Handtracking; Full-Body optional; IK; Menü; Haptik später; Komfort; sitzend/stehend; Kalibrierung; VR später | 📋 | 21–30 |
| E-ST-007 | 27.09. | 35#801E | R18C 31–42 ja: kontextsensitive Eingaben, Suche, Favoriten, Lern-UI, Vorschau ohne Exaktheitsversprechen, Begründung bei ungültiger Aktion, Kamera unabhängig, Cinematic nur Beobachtung/Replay, modulares HUD, VR-Fingertracking, Body Ownership | 📋 | |
| E-ST-008 | 11.09. | 35#458E | Beginn mit Setup-Befragung beim ersten Start, Standards bei Überspringen | 📋 | 76 |
| E-ST-009 | 20.09. | 35#627E | Favoriten: „weiß ich noch nicht, aber ja, glaube schon, dass das gut wäre“ | 🧭 | vorläufig |
| E-ST-010 | 17.09. | 35#610E, 35#611A | Kontextabhängige Aktions-/Kombinations-Bibliothek, jederzeit einsehbar | 📋 | 81 |
| E-ST-011 | 20.09. | 35#675E | Content-Toggle für starke Körperreaktionen und Blut | 📋 | 117 ja |
| E-ST-012 | 18.09. | 35#620A | Streamer-Filter/Namen der Nacktheitsstufen offen | ❓ | OD |

---

## 13. RP: Replay, Aufnahme, Streaming

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-RP-001 | 13.09. | 35#464E | Szene speichern; Export 4K/8K oder niedriger, extrem hohe Bitrate, einstellbar, Speicherverbrauch anzeigen | 📋 | |
| E-RP-002 | 13.09. | 35#465A, 35#467A | Replay-Datenpaket statt Video; HUD/Untertitel/Prompts/Debug getrennt schaltbar; Cinematic Render Mode; Quick Export vs Cinematic Re-Render | 🧭 | Entwickler 35#466 „Filmlook, fast echt“, 35#468 „Der Rest passt“ |
| E-RP-003 | 13.09. | 35#468E | Re-Render im Hintergrund mit Fortschritt; Weiterspielen ohne großen Performanceverlust | 📋 | |
| E-RP-004 | 13.09. | 35#468E | Aufnahme/Streaming direkt im Hauptspiel, unabhängig von Adult | 📋 | |
| E-RP-005 | 13.09. | 35#469A | Adult-Inhalte können separat als nicht streambar/versteckt konfiguriert werden (falls Plattformregeln es verlangen) | 🧭 | |
| E-RP-006 | 14.09. | 35#484E | Streaming ohne Zusatzprogramm; jede Szene aufnehmbar; Replay = Aufgenommenes abspielen; Replay-Buffer nur für kleine Clips | 📋 | R12 1–20 |
| E-RP-007 | 14.09. | 35#484E, 35#485A | Fragen 21–100 (Replay-Editor, Kameras, Render-Qualität, Export-Formate, Audio-Busse, Provider-Unabhängigkeit, gemeinsame Capture-Pipeline): Entwickler „mir ist das egal, mache was du gut findest“ | 🧭 | delegiert; nur Scene-State-Track für Re-Render |
| E-RP-008 | 18.09. | 35#626A, 35#628A | Replay ≠ Savegame; deterministische Kernspur; Cinematic Re-Render; Hintergrund-Job mit „Gameplay zuerst“ | 📋 | R16H 186–190 ja |

---

## 14. SP: Speicherstand, Modding, Builds, Technik

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-SP-001 | 27.09. | 35#804A–808A | Event-gesteuert statt Tick; Simulation-LOD; Budgets vorab; Graceful Degradation; Determinismus; Visual Physics darf nichtdeterministisch; Threads mit Ownership; Data Registry; kleine stabile Mod-API; Capabilities; CI-Massentests; Golden Tests; Telemetrie im Dev-Build; kleiner vertikaler Kern | 📋 | R19A 5–20 ja |
| E-SP-002 | 27.09. | 35#806A | Gemeinsame IDs; Hot-Enable; Feature Flags; Crash-Isolation; Mod-Sandboxing; Versionsprüfung; Build-Split | 📋 | R19B 21–32 ja |
| E-SP-003 | 18.09. | 35#626A | Settings-Kategorien (Gameplay, Controls, Visuals, Physics, Audio, Content, Replay, VR, Accessibility, Advanced); Presets + Advanced; UI-Hilfen vierstufig | 📋 | R16H 176–180 ja |
| E-SP-004 | 18.09. | 35#626A | Statistik-Events über zentralen Stream; Adult-Statistik nur bei aktiver Mod sichtbar und ausblendbar; Adult-State mit Schema-Version | 📋 | 183–185, 197 |
| E-SP-005 | 18.09. | 35#626A | Multiplayer: autoritativer Server synchronisiert Aktionen/Zustände, Soft-Tissue clientseitig; Adult nur vom Host aktivierbar | 📋 | 194–196 ja |
| E-SP-006 | 18.09. | 35#626A | Modding-Registry für Kategorien, Präferenzen, Items, Pose-Hints, Reaktionen, Audio/Visual-Profile; Konflikterkennung; Debug/Developer-Inspector | 📋 | 198–200 |
| E-SP-007 | 10.09. | 35#176A | Stable IDs ohne Version (namespace:kind:local-id); engine-neutraler Kern ohne Unreal-Typen; offene Kategorien nicht als C++-Enums; Save-Impact NONE/OPTIONAL/STATEFUL/CRITICAL | 🧭 | Korrekturvorgaben |
| E-SP-008 | 10.09. | 35#176A | Adult Experience Configuration: Global → Mod Profile → Character → Savegame → Per-NPC; Nutzer-Overrides nie destruktiv | 🧭 | Vorgabe an das externe Modell |
| E-SP-009 | 18.09. | 35#620A | Rollen der Modelle: Claude/GPT für Kernarchitektur, uncensored Qwen für Katalog, Testdaten, Lückenfinder | 🧭 | Planungswerkzeug |

---

## 15. PR: Produktion, Qualität, Reihenfolge

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-PR-001 | 10.09. | 35#252A, 35#254A | Rundenplan 16–18 (R16 = „was ist enthalten“); Teilfragen ab R9 in Blöcken | 🧭 | |
| E-PR-002 | 11.09. | 35#255E, 35#265E, 35#275E | Fragen tiefer (50–500), dann „bis 100 reicht“; ab R10 maximal 100; Ausnahme R16 | 📋 | |
| E-PR-003 | 14.09. | 35#518E, 35#520E | 25er-Pakete, ausführlich formuliert; R15 darf über 100 gehen, „solange wirklich alles beantwortet“ | 📋 | |
| E-PR-004 | 14.09. | 35#490E, 35#491A | Alle Runden Adult; Zusatzrunde 12 zählt nicht → 19 Runden | 📋 | |
| E-PR-005 | 16.09. | 35#591A | Eigene Tattoo-Runde 17 → 20 Runden | 📋 | |
| E-PR-006 | 16.09. | 35#604E | Adult-Mod soll schnell verfügbar sein, noch vor manchen anderen Systemen | 📋 | Spätere Präzisierung: E-PR-010 |
| E-PR-007 | 24.09. | 35#695E | Zu genaue Planung: Default „so wie in echt“ (nur Adult) | 📋 | |
| E-PR-008 | 25.09. | 35#709E, 35#711E | R17 kein Umfang von 500 Fragen; R16 nur die wichtigsten Fragen | 📋 | |
| E-PR-009 | 25.09. | 35#713E, 35#715E | Registry-Freeze nach Audit+Qwen („das stelle ich am Sonntag auf oder später“); Adult V1 „sehr früh implementiert und dann sehr viel erweitert“ | 📋 | |
| E-PR-010 | 25.09. | 35#715E | Anfangs nur „normaler“ Umfang; wichtiger: Ladebildschirm, kleiner Kartenteil; Adult danach | 📋 | |
| E-PR-011 | 25.09. | 35#713E, 35#715E | Performance: nicht jeder hat Spitzenhardware; stark optimieren | 📋 | |
| E-PR-012 | 27.09. | 35#810A, 35#811E | R20: V1 klein; Core/Important/Late Polish; Core-Gate; weniger Features sauber; Performance-/QA-/Visual-Tore; Registry-Freeze nach Audits; getrennte Builds; Shared Core; Modding First-Class; Spezifikation pro Subsystem; Prototype First; Production Lock | 📋 | 1–9, 11–15 ja |
| E-PR-013 | 27.09. | 35#811E, 35#812A | Öffentliche Adult-Veröffentlichung ist **Ziel**; privat-only ist Rückfall | 📋 | 10 ja, mit Präzisierung |
| E-PR-014 | 23.09. | 35#698A | Default-Lock gilt nur für Adult | 📋 | 697 |
| E-PR-015 | 28.09. | 37#83 | Entwickler: „Bro ich habe adult fertig“ | 🧭 | Planung nach Interview „im Wesentlichen abgeschlossen“ (35#812), Audit offen |
| E-PR-016 | 26.09. | 35#785E | Optimierung globales Kernziel | 📋 | |

---

## 16. RC: Recht, Plattform, KI, Vertrieb

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-RC-001 | 28.08. | 23#536E, 23#538A | Konsolen-Darstellung Cyberpunk-artig; PC gleich; Mod separat | 📋 | siehe E-PA-002 |
| E-RC-002 | 28.08. | 23#544A | Offizieller Entwickler-Mod nicht als UGC behandeln; Valve/mod.io/Anwalt vor Veröffentlichung klären | 🧭 | ungeprüft |
| E-RC-003 | 28.08. | 23#545E | Frage: „Wenn ich die Mod über z. B. mod.io hochlade und tue, als wäre das Fanwork…“ | ⛔ | Assistent 23#546 lehnt die Täuschung ab |
| E-RC-004 | 25.09. | 35#715E, 35#716A | Frage nach dem Rechtlichen; Assistent: eigener LEGAL/COMPLIANCE/IP-Block, Dreifach-Gate Gesetz ∩ Store ∩ Zahlungsdienstleister, Österreich/Deutschland-Hinweise, Marken | 🧭 | ungeprüft |
| E-RC-005 | 25.09. | 35#717E, 35#718A, 35#720A | Adult erst privat; Veröffentlichung später nur bei passender Rechts-/Store-Lage; KI-Shipping-Assets deklarieren; fiktive Automarken; eigenes Design; Motorsport mit eigener IP | 🧭 | Dev 35#719 „ja genau“ |
| E-RC-006 | 25.09. | 35#721E, 35#727E, 35#729E | Fragen nach der Sichtbarkeit der KI-Offenlegung („wie macht man, dass dort wenig steht“, „es wird fast alles mit KI sein“) | ❓ | |
| E-RC-007 | 25.09. | 35#733E, 35#734A | **„ich will das eher geheimhalten“**; Assistent: nicht verstecken oder kleinreden, wahrheitsgemäß deklarieren | ❓ | K7 |
| E-RC-008 | 25.09. | 35#735E–741E | Keine Live-Generative-KI im fertigen Spiel („genau so habe ich das im Sinn“); Pre-Generated-AI ist publishbar; Beruhigung | 📋 | |
| E-RC-009 | 25.09. | 35#736A | Provenance-Registry pro Asset (human/AI-assisted/AI-generated+edited/licensed/purchased) | 🧭 | |
| E-RC-010 | 26.09. | 35#750A | Steam-Nudity-Deskriptoren; Entwicklergrenze „erst bei Penetration“ sei zu eng | 🧭 | ungeprüft |
| E-RC-011 | 26.09. | 35#776A | Symbolik extremistischer Gruppen an konkrete Fraktionen gebunden | 🧭 | rechtliche Zulässigkeit offen |
| E-RC-012 | 27.09. | 35#812A | Planung für Adult „im Wesentlichen fertig“, offen: Audit, Qwen-Audit, Prototyping, Recht | 📋 | |

---

## 17. XX: Nicht freigegeben, historisch, überholt

Diese Einträge dokumentieren geäußerte Wünsche, damit sie **nicht verloren gehen und nicht versehentlich als Planung gelten**. Sie sind keine Spezifikation.

| ID | Datum | Quelle | Aussage | Status | Vermerk |
|---|---|---|---|---|---|
| E-XX-001 | 10.09. | 35#191E | Stark Betrunkene müssten nur einmal „ja“ sagen, auch ohne zu wissen, worum es geht | 🕰️ | Ersetzt durch Capacity Gate (E-SK-006/015) |
| E-XX-002 | 10.09. | 35#195E, 35#197E, 35#199E | Ausziehen sei okay, wenn sie „die Hände nicht dazwischen hält“; „das System blockiert nicht, wenn zu betrunken“ | 🕰️ | Assistent lehnte ab; Entwickler stellte 35#203 klar, er habe nicht gemeint, „keine Gegenwehr = Zustimmung“ sei Regel |
| E-XX-003 | 10.09. | 35#205E | „Es kann in der Welt auch Rape geben, wenn man die Frau rettet, kann man belohnt werden“ | ⛔ | Assistent 35#206: Crime-Content, kein Sex als Belohnung |
| E-XX-004 | 10.09. | 35#207E | Eine Variante der Mod, in der man ohne Zustimmung handeln kann (nicht in der normalen Mod; „ich weiß nicht, ob das jemals veröffentlicht wird“) | ⛔ | Assistent 35#208 lehnt Design ab |
| E-XX-005 | 11.09. | 35#229E | „Nein“ ohne Chance auf Änderung abgelehnt, weil Frauen „oft nur so tun, als hätten sie kein Interesse“ | 🕰️ | siehe E-NP-009 |
| E-XX-006 | 15.09. | 35#544E | Bei Bewusstlosigkeit durch Atemkontrolle: „Stopp-Logik greift nicht, aber der Spieler spürt es“ | 🕰️ | Assistent 35#545: Interaktion läuft technisch nicht weiter |
| E-XX-007 | 15.09. | 35#544E | Langzeitvertrag: NPC kündigt „in 99,99999999 % der Fälle“ nicht, weil sie sonst weitergegeben würde | 🕰️ | Assistent 35#545: nicht hardcoden; Kompromiss E-NP-024 |
| E-XX-008 | 15.09. | 35#530E | Spieler können schwere Sexualstraftaten begehen und dafür ins Gefängnis kommen; ohne Hinweis im Spiel | ❓ | Konsequenzebene (E-WE-013); K3 |
| E-XX-009 | 15.09. | 35#524E | Gespielte Nicht-Einwilligung „wie bei Produktionsfirmen“, hochgeladen auf Seiten der Spielwelt | ❓ | Assistent 35#525: als vereinbartes Rollenspiel zwischen Erwachsenen; K4 |
| E-XX-010 | 16.09. | 35#576E, 35#578E | „Wenn sie nichts sagt oder stöhnt, ist es okay, und man kann weitermachen“ (auch bei Eskalation) | 🕰️ | Assistent 35#579: Sitzungsrahmen; Entwickler 35#580 „nicht zu streng“ |
| E-XX-011 | 20.09. | 35#627E | „Man kann bei Adult richtig brutal sein … das muss sie nicht immer wollen, aber man muss mit den Konsequenzen rechnen, aber möglich sollte es sein“ | ⛔ | Assistent 35#628: Nicht-Einwilligung gehört ins Crime-System |
| E-XX-012 | 20.09. | 35#629E, 35#631E | „Wenn sie vorher nicht gesagt hat, dass sowas nicht okay ist“, „man kann es trotzdem machen und die Reaktion sehen“ | ⛔ | Assistent 35#630/632: nicht auf „nicht verboten = erlaubt“ reduzieren |
| E-XX-013 | 20.09. | 35#633E, 35#635E | „Markiere das als von mir festgelegt“; Weitermachen auch gegen den Willen („schlimmstenfalls Gefängnis“), nur in der Admin-Version | ⛔ | Assistent 35#634/636: „USER-LOCK“-Formulierung; nicht übernommen (K2) |
| E-XX-014 | 20.09. | 35#637E, 35#639E | Admin-Version mit gleichem Funktionsumfang wie das normale Adult-System; Reaktionen „vermutlich anders, vielleicht auch nicht“ | ⛔ | Assistent 35#638/640: keine Non-Consent-Kataloge |
| E-XX-015 | 20.09. | 35#653E | „Wenn die Frau beim Sex einschläft, kann man sie weiter…, das bleibt nur in der Admin-Version“ | ⛔ | Assistent 35#654/656 lehnt ab (auch Admin-Build) |
| E-XX-016 | 20.09. | 35#655E, 35#657E | „Ich habe vorher Sachen geplant, die du nicht komplett willst, jetzt will ich das mit dem Schlafen auch. Also mache das so.“ → „Ja ok dann plane ich das dann halt mit Qwen.“ | ⛔ | Keine Zustimmung zur Alternative; K2, K9 |
| E-XX-017 | 11.09. | 35#261E | „Keine Immigranten“ in der Bevölkerung | ❓ | siehe E-NP-014 |
| E-XX-018 | 15.09. | 35#524E | Performerinnen seien „leichter zu bekommen“ (abwertende Verallgemeinerung) | 🕰️ | Assistent 35#525: Beruf ≠ Verfügbarkeit; keine Systemregel |
| E-XX-019 | 15.09. | 35#532E | Politisch/gesellschaftlich „konservative Welt, Linksextremismus“ | 🕰️ | Assistent: nicht ideologisch hardcoden; Entwickler 35#534/536: „die Welt soll logisch denken“ |
| E-XX-020 | 28.08. | 23#545E | Entwickler-Mod als Fan-Mod ausgeben | ⛔ | siehe E-RC-003 |
| E-XX-021 | 25.09. | 35#733E | KI-Nutzung verheimlichen | ❓ | siehe E-RC-007 |
| E-XX-022 | 28.08. | 28#22E | Codierte Umschreibung konkreter Praktiken in der Taxonomie | 🕰️ | Assistent 28#23 lehnt ab; abstrakte Fassung |
| E-XX-023 | 08.09. | 37#21E | Links zu einschlägigen Videos als Referenz für die Adult-Mod | 🕰️ | Assistent 37#22: analysiert/baut keine konkreten Sexualakte daraus nach |

---

## 18. Zählung

Die Zählung bezieht sich auf **diese Datei in ihrem Stand zum Abschlussbericht** (siehe unten). Ein Eintrag fasst teilweise **mehrere Fragen eines Pakets** zusammen (z. B. „R16D 76–100“); die Einzelantworten stehen in den Rundenprotokollen.

| Bereich | Einträge |
|---|---:|
| SK | 35 |
| PA | 36 |
| IA | 40 |
| SO | 37 |
| KO | 28 |
| GE | 12 |
| NP | 30 |
| LE | 12 |
| TA | 27 |
| WE | 21 |
| ST | 12 |
| RP | 8 |
| SP | 9 |
| PR | 16 |
| RC | 12 |
| XX | 23 |
| **Summe** | **358** |

Statusverteilung (ungefähr): bestätigt/Kanon 📋 überwiegt; Vorschläge 🧭 knapp ein Drittel; offene Punkte ❓ und nicht freigegeben ⛔ siehe [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md) und [Modul 34](34-Offene-Entscheidungen-Katalog.md).
