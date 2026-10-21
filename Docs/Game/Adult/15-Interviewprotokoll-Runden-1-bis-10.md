# Adult 15: Interviewprotokoll, Runden 1 bis 10

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dies ist ein Protokoll der Planungsgespräche, keine Spezifikation. Fragen stammen vom früheren KI-Assistenten (Vorschläge), Antworten vom Entwickler (E) oder vom Assistenten (A). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Quellenschlüssel: [Modul 13](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Fortsetzung: [Modul 16](16-Interviewprotokoll-Runden-11-bis-16.md), [Modul 17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md). Einträge im [Register](14-Entscheidungsregister.md).

**Inhalt**

1. [Lesehinweise](#1-lesehinweise)
2. [Vorphase 09.–10.09.](#2-vorphase-091009)
3. [Runde 1: Grundsystem](#3-runde-1-grundsystem)
4. [Runde 2: Dating und Rollen](#4-runde-2-dating-und-rollen)
5. [Zwischenphase: Zustimmung und Fähigkeit](#5-zwischenphase-zustimmung-und-fähigkeit)
6. [Runde 3: Kategorien und Intensität](#6-runde-3-kategorien-und-intensität)
7. [Runde 4: Präferenzprofile](#7-runde-4-präferenzprofile)
8. [Runde 5: Beziehungen und Attraktion](#8-runde-5-beziehungen-und-attraktion)
9. [Runde 6: Initiative und Kontakt](#9-runde-6-initiative-und-kontakt)
10. [Runde 7: Orte und Öffentlichkeit](#10-runde-7-orte-und-öffentlichkeit)
11. [Altersregel](#11-altersregel)
12. [Runde 8: Pakete und Modularität](#12-runde-8-pakete-und-modularität)
13. [Planung der Rundenzahl](#13-planung-der-rundenzahl)
14. [Runde 9: NPC-Erzeugung, Gedächtnis, Simulation](#14-runde-9-npc-erzeugung-gedächtnis-simulation)
15. [Runde 10: Körper und Szenensystem](#15-runde-10-körper-und-szenensystem)

---

## 1. Lesehinweise

- Die Rundenzählung folgt dem Gespräch. Sie ist **nicht überall als feste Überschrift gespeichert**; Abweichungen siehe [Modul 13, Abschnitt 8](13-Quelleninventar-und-Abdeckung.md#8-bekannte-abweichungen-innerhalb-der-quellen).
- „Ja“ des Entwicklers auf eine Fragenliste bedeutet: Zustimmung zur Fragestellung bzw. zum Vorschlag in der Formulierung des Assistenten, **nicht** automatisch zu Zahlen oder Details, die der Assistent als Beispiel nannte.
- Explizite Einzelheiten der Quellen werden **abstrakt** wiedergegeben. Die Quellen enthalten Aussagen, die diese Dokumentation nicht als Entwurf übernimmt; sie stehen im [Register, Abschnitt XX](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt) und in [Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md).
- Alle Aussagen: Nachweis `35#n` = Chat ‚Projektwissen nutzen‘, Nachricht `n`.

## 2. Vorphase 09.–10.09.

| Nachricht | Inhalt | Status |
|---|---|---|
| 35#58 | Master-Prompt des Entwicklers (09.09.): Modmanager-Profile ‚Vanilla, Realism, Hardcore, Testing, Custom, evtl. Adult‘. „Hardcore“ meint hier ein **Modding-Profil**, nicht die Adult-Intensität. | 📋 |
| 35#60, 35#141 | Vorschlag: Inhaltsbewertung und Adult-Flag in den Mod-Metadaten, Nutzerfilter. Empfehlung „ja“. | 🧭 |
| 35#63, 35#64 | Entwickler fragt, wann Adult genauer geplant wird. Assistent: eigener großer Block nach den Modding-Blöcken 1–7. | 🧭 |
| 35#71, 35#72 | Entwickler (09.09.): Adult-Mod „richtig immersiv, hardcore“; Spielerin/Spieler wählt aus; der Partner muss sich teilweise an die Partnerin halten; manche Figuren wünschen härtere Praktiken. Assistent: Rollen-/Präferenzprofile, Intensitätsstufen, Hard/Soft Limits, individuelle Grenzen, vollständig separate Adult-Profile, komplette Deaktivierbarkeit; **keine explizite Einzelakt-Ausarbeitung**. | 📋 / 🧭 |
| 35#73, 35#74 | Rollenteilung: Architektur beim Assistenten, konkrete Kategorien später mit einem anderen, ungefilterten Modell (Qwen). | 🧭 |
| 35#143, 35#144 | Mod-Verzeichnis `<GameRoot>/mods/`, Steam Workshop nicht festgelegt, mod.io/Nexus nur Kandidaten (providerbasiert). | 🧭 |
| 35#145–184 | Qwen-Masterprompt mit 39 Kapiteln (Adult-Mod-Architektur). Qwen-Entwurf V0.9 brach bei Kapitel 28 ab, später bei 30. Assistenten-Review 35#176: 16 Korrekturvorgaben (u. a. engine-neutraler Kern, IDs ohne Version, keine geschlossenen Enums, Eligibility Unknown/NotEligible/AdultEligible, Hard Limits als echte Invarianten, Widerruf logisch sofort, mehrdimensionale Intensität, kein Internetzugriff für Community-Mods, Teilnehmerzahl 2..N). Der Entwickler: „ok passt“ und Qwen solle Fragen stellen. | 🧭 |
| 35#177–180 | Auftrag: Qwen soll Fragen zu 20 Themenblöcken stellen. | 🧭 |
| 35#185 | Entwickler (10.09., 23:21): „Ok bin jetzt am Handy, sag die Prompts, ich antworte“. **Das Interview führte tatsächlich der Assistent, nicht Qwen.** Es ist kein Qwen-Ergebnis im Archiv. | 📋 |

## 3. Runde 1: Grundsystem

Nachrichten 35#186–187. Frage 1 bis 12.

| Thema | Antwort des Entwicklers | Anmerkung |
|---|---|---|
| NPC mit NPC | ja; Zuschauen und Beitreten möglich | 📋 |
| Häufigkeit | „komplett aussuchen“; Mod an/aus | 📋 |
| Steam | ‚Steam gibt es nicht frei‘ → Mod (35#187) | 📋, Rechtslage ungeprüft |
| Globaler Intensitätsschalter | **nein**; stattdessen anderes Paket installieren | 📋 |
| „Hardcore“ | Mehrdimensional: wie viel sich Figuren gefallen lassen, Tempo; als Preset | 📋 |
| Harte Grenzen | können sich ändern (Beispiel: früher tabu → später ok; Einmalbegegnung eher weniger) | 📋, Grenze zu [E-SK-009](14-Entscheidungsregister.md#2-sk-sicherheitskern-alter-zustimmung) beachten |
| Anzeichen | „wie real“ | 📋 |
| Pakete | Packs und All-in-One | 📋 |

## 4. Runde 2: Dating und Rollen

Nachrichten 35#188–190. Tiers (13): ja. Hardcore mehrdimensional (14): ja. Einzeln einstellbar (15): „es kommt auf die Person an“. Partnerabhängige Grenzen (17): „ja sicher, so wie in echt“. Beobachterrolle (18), spontanes Beitreten (19), Verlassen (20), Öffentlichkeits-Eigenschaft (21): ja. 📋.

**Rückfragen ohne ausdrückliche Bestätigung (Etappe 2.2):** Frage 16 (Rollenabhängige Grenzen): „verstehe ich nicht“ → vom Assistenten erklärt (35#190); eine Entwicklerantwort auf Frage 16 gibt es nicht, inhaltlich später durch Runde 3 Frage 2 „ja sicher“ (35#221) gedeckt. Frage 23 (Eskalationsstufen): „wie genau?“ → erklärt (35#190), vom Assistenten als gelockt geführt (35#220), nicht ausdrücklich bestätigt. Frage 24 (Registry ohne C++-Änderung): ‚genauer noch bitte‘ → erklärt, Assistentenempfehlung „ja“, nicht ausdrücklich bestätigt. Siehe [Register E-IA-001, E-IA-004, E-IA-005](14-Entscheidungsregister.md#4-ia-interaktion-taxonomie-ablauf). Frage 22 (Alkohol): Konfliktstelle [K1](11-Offene-Fragen-und-Konflikte.md). Die vollständige Frage-Antwort-Liste steht in [Modul 36](36-Frage-Antwort-Matrix-Runden-1-bis-9.md).

## 5. Zwischenphase: Zustimmung und Fähigkeit

Nachrichten 35#191–210. Hier entstanden die ersten, später mehrfach verworfenen Entwicklerwünsche. **Sie werden nur als Konflikt dokumentiert**, nicht als Design.

| Nr. | Entwickler | Assistent |
|---|---|---|
| 191, 193 | Ein „Ja“ solle auch bei Nichtwissen oder starker Trunkenheit genügen; Nicken als Zustimmung. | 190–202: abgelehnt; stattdessen Fähigkeitsprüfung (Capacity Gate). |
| 195 | Nach einem Kuss gehe es schneller; Ausziehen sei okay, wenn die andere Person nicht abwehrt. | Zustimmung nie aus Passivität ableiten. |
| 199 | „System blockiert nicht, wenn zu betrunken.“ | Fähigkeitsprüfung bleibt harte Voraussetzung. |
| 201, 203 | Klarstellung: Er habe nicht gesagt, Passivität sei Zustimmung. | 204 korrigiert die eigene Darstellung. |
| 205 | Vergewaltigung als Weltereignis; Retter werde belohnt. | 206: Verbrechen wird nicht erotisiert, kein Sex als Belohnung. |
| 207 | Eine Variante ohne Zustimmungsprüfung (nicht in der normalen Mod; ‚weiß nicht, ob jemals veröffentlicht‘). | 208: abgelehnt als Design. |
| 210 | – | Gliederung in Pack A (Framework), B (hohe Intensität, einvernehmlich), C (dunkle Verbrechensthemen, kein Adult-Katalog). |

Ergebnis: Capacity Gate und Session Intent Envelope ([Modul 01](01-Grundregeln-Alter-Zustimmung.md)).

## 6. Runde 3: Kategorien und Intensität

Nachrichten 35#220–224. Kategorie → Unterkategorie → Variante; Rollenprofile; kategorieeigene Intensitätsdimensionen; Presets Soft/Normal/Intense/Hardcore/Custom; NPC-Einstellung pro Kategorie; Partnerabhängigkeit; spontane Ausnahmen; Orte pro Kategorie; Beobachter nach Absicht (zufällig/absichtlich/eingeladen); Animations-/Audio-/Kamera-Packs; All-in-One. Frage 9 (Ort beeinflusst): „kann sein“. 📋.

## 7. Runde 4: Präferenzprofile

Nachrichten 35#226–228.

| Frage | Antwort | Anmerkung |
|---|---|---|
| Profil | Präferenzprofil, änderbar | 📋 |
| 7 | Überreden ja | Assistent: Hard Limit verschwindet nicht durch Wiederholen |
| 8 | Deltas „kann schon sein“ | 🧭 |
| 9 | Stabile Dinge „bei manchen“ | 🧭 |
| 10 | Stufenskala Curious … Hard Limit: ja | 📋 |
| 11 | Spieler lernt über Verhalten | 📋 |
| 12 | Debug/Cheat sieht alles | 📋 |
| 13 | Archetypen (500.000 NPCs) | 📋 |
| 14 | Handkuratiert nur für die Mod | 📋 |
| 15 | Gedächtnis ja | 📋 |

## 8. Runde 5: Beziehungen und Attraktion

Nachrichten 35#228–230. Getrennte Werte; Gelegenheitsbegegnung bei niedrigem Vertrauen; Aussehen reicht bei manchen; Chemistry positiv/negativ; Eifersucht; offene Beziehung/Poly, Fremdgehen verschlechtert die Beziehung. Frage 13 „mit jedem NPC Sex haben können“: Assistent stellt klar, das ist nicht garantiert. Frage 14: Entwickler „nein“ (Annahme, Frauen täuschten oft Desinteresse vor); Assistent: nicht als Systemregel, ein aktuelles Nein bleibt ein Nein ([E-XX-005](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt)). Frage 15: seltene Fälle in eigenen Fällen, nicht Story.

## 9. Runde 6: Initiative und Kontakt

Nachrichten 35#230–232. NPC-Initiative; Stile; Aussehen als erster Auslöser; Spontaneität Kernziel; Club erhöht die Chance, überall möglich; Ortswechsel; mehrere Interessenten; Kontakte/Nummern; NPC meldet sich; Casual-only/Relationship-only/Mittelding; Faktorenmix. 📋.

## 10. Runde 7: Orte und Öffentlichkeit

Nachrichten 35#232–236. Ortstags intern; ortsabhängige Reaktion; Sicht-/Hörweite echter NPCs; Beobachter-Reaktionen; Einladen; spontanes Beitreten; nicht unterstützte Konstellation → Abwechseln/Fallback (Entwickler 233: ‚ok aber logisch, dann wechselt man sich ab‘); ‚Person A ok, B nein‘; Orte mit eigenen Regeln; Adult-Hubs; Frage 14 (Security/Hausregeln): Entwickler „was hat das mit Adult zu tun“ → 235 „ok passt“ → World Rules/Venue Policy; Ort merkt sich Dinge. 📋.

## 11. Altersregel

Nachrichten 35#237–244.

| Nr. | Aussage | Status |
|---|---|---|
| 237 | Mindestalter **18 Jahre 0 Tage**. Darunter weder erlaubt noch angezeigt. | 📋 |
| 239 | Entwickler: Unbekanntes Alter → fragen, Ausweis. Assistent: Zustand „AdultVerified“; keine Ausweisdaten speichern; Geburtsdatum statt „Age“-Feld. | 📋 / 🧭 |
| 241–244 | Alter ist keine Präferenz. | 📋 |

## 12. Runde 8: Pakete und Modularität

Nachrichten 35#246–250. Basisframework; Packs; Fragen 2/3 abhängig von Größe; 4 All-in-One als Standard, wenn nicht riesig; Versionen/Abhängigkeiten ja; 7 requires/optional/incompatible/recommended ja; 8 Charakterpack ja; 9 Animationspack nur teilweise wiederverwendbar (andere Körpermaße); 10 Stimme (Sprachen, Qualität); **11 Orte gehören ins Hauptspiel**; 12 Hardcore-Pack läuft über das Framework unverändert (Assistent: ja); 13 Mod-Manager-Status ja; 14 Preset-Bundles später; 15 Kategorien deaktivierbar. Größenbeispiele des Assistenten (15–25 GB vs. 50–100+ GB) sind nur Beispiele. 📋 / 🧭.

## 13. Planung der Rundenzahl

| Nr. | Aussage |
|---|---|
| 252 | Assistent plant 16–18 Runden (Vorschlag). |
| 255–258 | Entwickler: komplexere Runden, 50–500 Fragen, ohne Wiederholungen. |
| 265 | Entwickler: bis 500 sei zu lang, bis 100 genüge. |
| 275, 276 | Ab Runde 10 maximal 100 Fragen; Ausnahme Runde 16. |

## 14. Runde 9: NPC-Erzeugung, Gedächtnis, Simulation

### 14.1 Runde 9A: Präferenz-DNA (35#260–263, 45 Fragen)

- Präferenz-DNA aus Seed, an die globale NPC-ID gekoppelt (Entwickler 261/3: sonst Reset in Beziehung). 1 ‚kann sich ändern‘, 2 „nur am Anfang generiert“.
- Hierarchie Persönlichkeitskern > Sozial > Attraktion > Adult-Präferenz > situative Überschreibungen (5 ok). Archetyp nur als Bias (7, 8). 9 Plausibilitätsprüfung.
- 10–20 ja (Dimensionen kontinuierlich; Stimmung kurzfristig). 21–23 Attraktionsprofil mehrkomponentig. 24 „bei Frauen kann das sehr schnell gehen“. 25–26 anfängliche Attraktion nur bei Dates, bei Einmalbegegnungen eher weniger (Assistent: auch dort beibehalten). Erregung ist ein Zustand, keine Zustimmung.
- 27–38 ja (Styling, Hygiene, Stimme, Reputation, partnerspezifische Deltas, Chemistry bidirektional, Partner-Gedächtnisprofil). 39 ja; Bekanntschaft und Vorlieben bleiben gespeichert, außer die Person ist tot. 40 Einmalbegegnung genauso genau gespeichert wie Beziehung. Gedächtnisstufen Minimal/Known/Detailed/Full (Assistent). Daten Verstorbener nicht komplett löschen (Assistent).
- 41–44 ja (Regionalität nur Bias). 45: Entwickler ‚leichte Unterschiede (Stimme/Aussehen), keine Immigranten, nur Leute von dort oder aus den USA‘. Assistent widerspricht (einzelne Zugewanderte und Touristen seien glaubwürdig). Entwickler 263: „Ok“ ohne klare Zustimmung/Ablehnung → [E-NP-014](14-Entscheidungsregister.md#8-np-npc-präferenzen-beziehungen), ❓.

### 14.2 Runde 9B (35#268–274)

| Teil | Fragen | Antworten |
|---|---|---|
| 9B.1 | 30 | Kernpersönlichkeit, schnelle/mittlere/lange Zustände, Ankererinnerungen, Vergessen, bekannte Person, detaillierter Beziehungsdatensatz; 1–13 ja, 14 ja aber ungenau erinnern, 15 ja, 16–30 ja |
| 9B.2 | 60 | Präferenzlernen, Dates, Einmalbegegnung, Fremdgehen, Eifersucht, Trennung, Klatsch; 1–15 ja; 16 „kann sein, aber oft mit“ (gutes Date endet nicht automatisch in Adult); 17–20 ja; 21 **nein** (Einmalbegegnung kann zur Beziehung werden); 22–52 ja; 53 „was ist Gossip“; 54–60 ja |
| 9B.3 | 50 | Langzeit, Alterung, Tod persistent, Zeitreisen über Temporal-World-State, Umzug zwischen 7 Zuständen; „1–50 passt alles“ |

### 14.3 Runde 9C: Weltsimulation (35#278–280, 40 Fragen)

Globale NPC-ID; Tiers 0–3 (Held/lokal/regional/ruhend); Tier 3 nur Ereignisschritte; Welt läuft nur beim Spielen, Online anders; ereignisbasierte Hintergrundsimulation; Zufall deterministisch aus NPC-ID, Weltseed, Zeitfenster; Seed bleibt; relevante Begegnung friert den Zustand ein; Delta-Saves (nur NPC-ID und Seed für Unveränderte); Gedächtnisklassen permanent/lang/kurz/verwerfbar; Zusammenfassung ersetzt keine Einzelerinnerung („solange der NPC sich an alles einzeln erinnern kann“); Migration. Frage 40 Speichergröße: Entwickler postet eine Genretabelle (5–20 MB für Open-World-RPGs) und sagt „eher kleiner“. Assistent 280: Ziel ca. 50–150 MB normal, 150–300 MB extrem; mehrere GB = Architekturproblem. **Zahlen sind Vorschlag.** 📋 / 🧭.

## 15. Runde 10: Körper und Szenensystem

### 15.1 Runde 10A: Körperprofil (35#280–282, 35 Fragen)

Körper-Seed; kontinuierliche Parameter; Asymmetrie (Entwickler 281: nie komplett gleich); Muskelmasse; Training/Gewicht/Alterung (Entwickler: soll nichts mit Adult zu tun haben, allgemeine Lebenssimulation); Schwangerschaft „weiß noch nicht“; Narben/Tattoos/Piercings ja; viele NPCs mit vielen Tattoos; Haar/Bart wachsen, kein sofortiger Wechsel; Friseur im Hauptspiel; Make-up als Schicht; Attraktion auf tatsächlich erzeugtem Körper; Körpersystem unabhängig vom Adult-Mod. 📋.

### 15.2 Runde 10B: Skelett und Kontakt (35#282–285, Fragen 36–70)

Standardisiertes Skelett mit Proportionen/Morphs; Skelettfamilien; Messwerte links/rechts; Gang; poseabhängige Verformung; Korrektur-Morphs; Kontaktanker aus Maßen mit Toleranzen; Auswahl passender Varianten; Paar-Animationen für Hauptfiguren; Retargeting; Validator (Beispielwert 94 %); Kompatibilitätsgrenzen; Kleidungs-/Haarphysik; sichtbarer vs. simulierter Kontakt; Regel ‚Animation bestimmt Bewegung, IK korrigiert Kontakt, Physik nur sekundär‘. Entwickler 283: „37–70 ja“; jede Person „realistisch nachgebaut“.

**Architektur-Lock des Entwicklers (35#283):** im ganzen Projekt kaum Blueprints (C++), eigenes Speichersystem wegen Unreal-Problemen mit Größe. → [E-PA-025](14-Entscheidungsregister.md#3-pa-pakete-builds-architektur), [E-LE-004](14-Entscheidungsregister.md#9-le-gedächtnis-leben-simulation-speicher).

### 15.3 Körperreaktion und Menschenpipeline (35#286–302)

- 286: Zwei Projekt-Locks vorgeschlagen (C++-first, eigenes engine-neutrales Persistenzsystem; Default-Welt nicht gespeichert, Delta gegenüber generiertem Ausgangszustand). Save-Ordner (Manifest, Spieler, Welt, Regionen, NPCs, Beziehungen, Fälle, Fahrzeuge, Eigentum, Wirtschaft, Zeitlinie, Mods). **Alles Vorschlag**, vom Entwickler in 283 sinngemäß befürwortet.
- 287: Entwickler wünscht realistische Muskel-/Weichgewebebewegung im Akt und sehr explizite Anatomiedetails. Assistent 288: **keine Detailgestaltung expliziter Genital-/Analmechanik**; Unterbau technisch; Explizites später mit Qwen. → Block „High-Fidelity Body Response“ (20 Punkte).
- 289–292: Antworten: 1–8 ja; 9 realistisch (Standard); 10 egal, Mods dürfen alles; 13 sichtbare Atmung anfangs nicht wichtig; 14 Herzschlag: eigener bei Extremstress, fremder nur in Nähe; 15 Schweiß ja; 16 Druckstellen kein Muss; 21 ja (90 % Realismus reicht); 31–32, 34–35 ja, aber nicht das wichtigste Feature; 39–40 ja: **Adult nutzt exakt dasselbe Körpersystem wie das Spiel.**
- 293–302: Vorschläge zur Menschenpipeline (Human Master, Parameterraum statt Presets, Gesichtsraum, KI-gebaute Generatoren, Tier S/A/Population, MetaHuman/CC5 prüfen, Hybrid). Entwickler 295: Tier S = wichtige Story-Figuren; Spieler erstellt Figur zum Start; ‚kein extra Geld nur für ein Modell, keine 500 Euro‘. 296: Spieler = Tier S dynamisch; Beförderung eines NPC ohne Neuwürfeln. 297 ‚Ok also einmal Generator‘. 299–300: NPC Life DNA, Korrelationen statt harter Regeln (stark tätowiert ≠ Tätowierer), Neugeborene mit eigener ID, Genetik aus Eltern, Wachstumskurve, Gesicht altert, Lebensphasen; **das Adult-System sieht Minderjährige nicht** (Pipeline Alter/Eligibility). 301: Haar/Bart kontinuierlich grenzenlos; entfernte NPCs simulieren weiter. 302: nichtlineare Alterskurven, Delta-Ereignisse, „Simulation ja, volle Echtzeitsimulation nein“. 🧭 mit Entwickler-Bestätigungen.

### 15.4 Runde 10 Szenensystem (35#440–446, 100 Fragen)

Entwickler 444: 1–4 ja; **5 und 11 sind Konfliktstellen** (siehe [Modul 19, K1](19-Konflikt-und-Widerspruchsmatrix.md)); 12–38 ja; 39 ja, Jiggle-Physik realistisch wichtiger; 40–65 ja; 66 ja, spezialisierter für stark beanspruchte Körperteile, aber auch im Hauptspiel; 67–99 ja; 100 „jedes Mal anders“, komplette Freiheit der Position.

Themen der 100 Fragen: Initiierung/Ablehnung/Grenzen; Orte und Privatsphäre (20 Tür abschließen); 29 spontan vs. Date; 31 keine starre Szene, sondern **Interaction Session aus modularen Aktionen**; 34 Intent/Action vs. Animation/IK/Physik; 36 jedes Mal leicht anders; 40 Alternative statt Clipping; 47–50 beliebig kurz/lang, kein Fortschrittsbalken, kann ‚nicht gut laufen‘; 51–65 Präferenzen (58 keine sichtbare Checkliste); 66–80 Körper/Physik; 81–90 Audio/Dialog/Mimik/Blick; 91–100 Folgen (99 Gesundheit nur über das allgemeine Gesundheitssystem; 100 niemals Minispiel).

Assistenten-Antworten 445–454: Zustimmung bleibt in der laufenden Situation bis Widerruf oder wesentlicher Änderung; neue Begegnung wird neu berechnet. Leichte Trunkenheit okay, starke Beeinträchtigung keine Zustimmung, „komm mit zu mir“ ≠ pauschales Ja. Entwickler 446/448: Betrunkene sollen ‚genug Aussagekraft‘ haben, wenn sie noch denken können. 447: Formel `Fähigkeit + Ansprechbarkeit + vorherige bejahende Absicht + aktuelles Verhalten`. 450–454: Bewusstlosigkeit beendet die Interaktion; eine kurze Erkennungsverzögerung (fehlende Eigenbewegung, Muskelspannung, Atem, Antworten) ist erlaubt, kein minutenlanges Weiterlaufen; Entwickler 454 **„Ja ok dann passt das“** → bestätigt ([E-SK-015 bis E-SK-018](14-Entscheidungsregister.md#2-sk-sicherheitskern-alter-zustimmung)).

Globale Locks, zusätzlich in 445/446 (nicht Adult): Horror-Ton früher Staffeln; Jäger-Hauptquartier unterirdisch, nicht von Beginn, Entdeckung nach einigen Stunden Story.
