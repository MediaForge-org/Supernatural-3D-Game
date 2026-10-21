# Adult 16: Interviewprotokoll, Runden 11 bis 16

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Protokoll der Planungsgespräche, keine Spezifikation. Statuszeichen: [Legende](README.md#statuslegende). Explizite Einzelheiten der Quellen werden abstrakt wiedergegeben; nicht übernommene Entwicklerwünsche stehen im [Register, Abschnitt XX](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Vorher: [Modul 15](15-Interviewprotokoll-Runden-1-bis-10.md). Danach: [Modul 17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md). Quellenschlüssel: [Modul 13](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Konfliktnummern K1 bis K9: [Modul 11](11-Offene-Fragen-und-Konflikte.md), K10 ff.: [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md).

**Inhalt**

1. [Runde 11: Freeform-Engine](#1-runde-11-freeform-engine)
2. [Runde 11B: Ergänzungen 101–135](#2-runde-11b-ergänzungen-101135)
3. [Replay-Wunsch und Architekturvorschläge](#3-replay-wunsch-und-architekturvorschläge)
4. [Runde 12: Replay, Aufnahme, Streaming](#4-runde-12-replay-aufnahme-streaming)
5. [Runde 13: Ablauf und Reaktionen](#5-runde-13-ablauf-und-reaktionen)
6. [Rundenzählung 18/19/20](#6-rundenzählung-181920)
7. [Runde 14: Körper, Tattoos, Piercings](#7-runde-14-körper-tattoos-piercings)
8. [Runde 15: Praktiken, Präferenzen, Gesellschaft](#8-runde-15-praktiken-präferenzen-gesellschaft)
9. [Runde 16: Master-Runde](#9-runde-16-master-runde)

---

## 1. Runde 11: Freeform-Engine

Nachrichten 35#457–459, 100 Fragen.

| Thema | Inhalt der Fragen | Antwort des Entwicklers |
|---|---|---|
| Grundprinzip | Kein Positionsmenü, kontinuierlicher Ganzkörper-Posenzustand, Pose-Templates nur als Solver-Hilfen | 458: ‚es gibt Positionen, muss aber nicht sein, dass sie benutzt werden‘ |
| Gelenke | biomechanischer Bewegungsbereich pro Gelenk und Person | ja |
| Widerstand | wächst allmählich; Verletzungen möglich (auch im Bodenkampf) | ja |
| Kleidung | begrenzt Beweglichkeit | ja |
| Gleichgewicht | Schwerpunkt, Abstützen, Gewichtsunterschied | ja |
| Kontakt | Contact Graph (Fläche, Kraft, Reibung, Material, Temperatur) | ja |
| Weichgewebe | sekundär zum Skelett | ja |
| Qualität | Low/Medium/High/Ultra/Cinematic | „Low darf nicht fake aussehen“ |
| Muskulatur | Last auf Gelenke | ja |
| Übergänge | kontinuierlich | ja |
| Mikrovariation | deterministisch aus Session-Seed: gleiche Session + Laden = gleiche Mikrovariation | ja |
| Steuerung | relativ statt Hotkeys | 71: ‚weiß nicht, linker Stick …, VR später‘ |
| Pose-Overlay | optional abschaltbar | 75 ja |
| Entspannen/Halten | Relax-Solver, Halten | 77–79 ja |
| NPC | nutzt denselben Solver; Stärke entscheidet nie allein | 85: manche wünschen Dominanz → einvernehmlich dominante Interaktion, **innerhalb der Präferenzen** |
| Fallbacks | Mikrokorrektur → IK-Lockerung → Neuberechnung → harter Reset | 91: „Teleportieren gibt es nicht“; 92: Reset so gut wie nie |
| Debug | Pose-/Kontakt-Debugger im Mod-Editor | ja |
| Mods | ergänzen; Kernsolver zentral | 96–97 ja |
| Wiederverwendung | 98: Engine nicht Adult-spezifisch (Umarmen, Tragen, Medizin, Ringen) | ‚Teile davon sollen übernommen und für Adult erweitert werden‘ |
| Erfahrung | 66: Unerfahrene unbeholfener, lauter bei ungewohnt Hartem (auch Geübte) | 📋 |
| Setup | 76: beim ersten Start des Mods Setup-Fragen, bei Überspringen Standardwerte | 📋 |

Assistent 459: Posen als Hinweise; VR ändert den Eingang, nicht die Simulation; Qualitätsstufen = gleiche Simulation, andere Genauigkeit; Kraft/Dominanz in zwei Ebenen (Interaction Intent vs. Physik): vereinbarte dominante Interaktion darf kräftiger aussehen, **Physik entscheidet nie über Grenzen**; kein sichtbares Teleportieren, im Release lieber sauber beenden; „problematische Pose“ ist technisch (Clipping, Solver ohne Lösung). Debug-Builds speichern Körpermaße, Pose, Gelenkwinkel, Kontaktgraph, Kräfte, Umgebung, Solver-Zustand. 460–461: Sekundärbewegung abhängig von individueller Anatomie und Kontaktzustand, nicht pauschal.

## 2. Runde 11B: Ergänzungen 101–135

Nachrichten 35#463–465. 101 Startpose aus aktueller Haltung (Entwickler 464: Verabredung, Heimweg, sinnvoller Übergang, danach realistischer Start); 102 Ziel beschreiben, NPC bewegt sich selbst (ja); 103–107 ja; 108 Kategorien optional „weiß ich noch nicht“; 109 ja; 110 Unerfahrene wollen nicht automatisch hart (Erfahrung ≠ Intensität); 111–113 je Körperteil leicht anders; 114–119 ja; 120 Körper nicht statisch (z. B. Gewichtsänderung, Schwangerschaft verändern Proportionen); 121–126 Druckverteilung, Kleidung als Schicht; 127–129 ja; **130 „welche Adult-Werte?“ → Assistent: keine Adult-Extra-Werte, normale Charakterdaten** (Muskelkraft, Gewicht, Ausdauer, Ermüdung, Verletzungen, Hebel, Balance, Beweglichkeit, Koordination); 131 ‚weiß nicht, vielleicht wie UFC-Spiele Positionswechsel‘ (nicht gelockt); 132 „dieses Menü kann richtig gut sein“; 133–134 VR gleiches System, mehr in der Hand; 135 ja, Katalog trotzdem möglich. 📋 / 🧭.

## 3. Replay-Wunsch und Architekturvorschläge

- **35#464 (Entwickler):** Szene speichern; Export in 4K/8K oder niedriger, sehr hohe Bitrate, einstellbar, Speicherverbrauch anzeigen. 📋
- **35#465 (Assistent, 466 „Ja passt“):** Replay-Datenpaket (Figuren, Session-Seed, Zeitlinie, Pose/Eingabe, NPC-Entscheidungen, Physikereignisse, Kamera, Umwelt, Audioreferenzen) plus Video-Export (720p bis 8K, 30/60/120 fps, Qualitätsprofile, Codec/Bitrate/CRF/10-Bit/HDR), Speicherschätzung live; nachträgliche freie Kamera. 🧭
- **35#466 (Entwickler):** Beim Neurendern soll es wie ein Film aussehen (ohne HUD), „fast echt“. 📋
- **35#467–470:** HUD/Untertitel/Prompts/Debug getrennt schaltbar; Cinematic Render Mode; Quick Export vs. Cinematic Re-Render; Hintergrund-Re-Render mit Fortschritt; Aufnahme/Streaming im Hauptspiel unabhängig von Adult („ein Button“); gemeinsame Capture-Pipeline; Adult-Inhalte können separat als nicht streambar konfiguriert werden, falls Plattformregeln es verlangen. Entwickler 470: „Ja genau so machen wir es“. → [E-RP-001 ff.](14-Entscheidungsregister.md#13-rp-replay-aufnahme-streaming)
- **35#470–473:** LOC- und Asset-Schätzungen des Assistenten (2–4,5 Mio. Zeilen, 10–20+ Jahre Solo+KI): **Schätzungen, kein Beschluss.** Architekturvorschlag 473: engine-neutraler Kern, Unreal als Integrationsschicht, Entitätsstufen Simulated/Active/Materialized (Größenordnungen ‚müssen gemessen werden‘), `HumanInteractionRuntime` statt „AdultSystem“, hybride Animation, Gedächtniskonsolidierung, Persistenz mit Ereignislog und Snapshots, Mod-API mit Typen, UI ohne Spielregeln. Entwickler antwortete nur auf Punkt 17.
- **35#474–479:** Welt „per Hand“ = KI-gebaut; Planungsebenen Makro→Region→Ort→Gebäude→Detail. Nicht Adult.

## 4. Runde 12: Replay, Aufnahme, Streaming

Nachrichten 35#483–485, 100 Fragen. Entwickler 484: 1 ja (Streaming ohne Zusatzprogramm, jede Szene aufnehmbar, auch nachträglich); 2–4 ja; 5 ja, aber nur kleine Clips statt großer Puffer; 6–20 ja (‚Replay = Aufgenommenes abspielen‘); **21–100: „mir ist das egal, mach was du gut findest. Ich möchte jetzt wieder Adult planen.“** Assistent 485 vereinfacht: normales Gameplay = echte Aufnahme/Replay-Puffer; nur für Cinematic Re-Render optionaler Scene-State-Track. Fragen 21–100 (Replay-Editor, Kameras, Render-Qualität inkl. Path-Tracing, Export-Formate, Audiobusse, Provider-Unabhängigkeit) bleiben **Vorschläge nach Ermessen des Assistenten**. → [E-RP-007](14-Entscheidungsregister.md#13-rp-replay-aufnahme-streaming).

## 5. Runde 13: Ablauf und Reaktionen

Nachrichten 35#485–489, 100 Fragen (nur erwachsene, entscheidungsfähige Figuren).

**Themen:** natürlicher Einstieg und Übergang; ‚direkt nur Sex‘ als weiterer sozialer Intent; Kleidung stückweise, IK statt Verschwinden, Kleidung bleibt liegen; Küssen freeform; Berührung über den Kontaktgraphen mit personenabhängiger Reaktion; **Erregung als kontinuierlicher, verborgener physiologischer Zustand** (kann sinken, parallel Stress, körperlich ≠ emotional); Variation wie beim Generator; Schwangerschaft/Genesung langfristig; Training; Gewicht → Weichgewebe; manuelle/orale/penetrative Interaktionen über dieselbe Engine (Rhythmus nicht konstant, Druck variabel, Finger-IK, Kiefer-Constraints); Intensität aus Kraft/Geschwindigkeit/Druck (kein Zweifachschalter); Tagesform, Gewöhnung, Lautstärke, Ort; Orgasmus nicht garantiert, variabel, mehrfach möglich, Erholungszustand, Session endet nicht automatisch; Nachspiel (Nähe, Gespräch, Schlaf, Duschen, Essen, Kleidung, Beziehungswirkung); 99 keine isolierte Adult-Statistik; 100 niemals Minispiel.

**Entwickler 486:** 1 ja („Spiel simuliert die Welt, es gibt keine Cuts“); 2 ja aber wieso; 3–50 ja; 56 „welchen Druck meinst du“ (Assistent: Kontaktkraft); 59 **„sehr wichtig“** (körperliche Grenzreaktionen bei oralen Handlungen, Detailtiefe als Wunsch des Entwicklers); 60 „sag genauer“ (Assistent: Erfahrung = mehrere unabhängige Bereiche statt eines „SexSkill“); 65, 66 Rückfragen; 76 Stimme individuell, auch ‚manche wollen den Schmerz‘ (einvernehmlich, Präferenzprofil); 99 „wie meinst du das“ (Assistent: keine Arcade-Statistik als Verhaltenstreiber); sonst ja.

Assistent 487: körperliche Grenzreaktionen als physiologisches Zustandsmodell aus Körperzustand, Belastung und Reflexempfindlichkeit, individuell; extremere Reaktionen als **konfigurierbarer** Inhalt; Entwickler 488 „Ja passt“. **Dokumentationsentscheidung:** nur abstrakte Zustandsfamilien, Detailtiefe offen ([Register E-KO](14-Entscheidungsregister.md#6-ko-körper-reaktionen-physiologie)).

## 6. Rundenzählung 18/19/20

| Nr. | Inhalt |
|---|---|
| 446, 447 | Entwickler: „Sind es nicht 18 Runden gewesen?“ Assistent: 18. |
| 490–493 | Entwickler: alle 18 sollen Adult sein. Assistent: 19 nummerierte Runden, Runde 12 (Replay) ist Zusatz. **Rekonstruktion** der Themen 1–19 (nicht überall als Überschrift gespeichert). |
| 494–497 | Entwickler: Tattoos noch nicht festgelegt, „ich habe mehreres noch nicht gesehen“. Assistent 495 listet Lücken (Tattoos, Piercings, Anatomie, Grooming, Hygiene, Schwangerschaft/Verhütung, Gesundheit, Flüssigkeiten, Items, Fotos/Videos, Solo, Gruppen, Orte, Fetische, Lernen, Mod-Konfiguration). Neue Reststruktur (Entwickler „Ja passt“): R14 Körper/Tattoos/Piercings/Grooming; R15 Praktiken/Fetische/Varianten/NPC-Vorlieben; R16 Mega-Runde; R17 Steuerung/UI/VR/Inhaltseinstellungen; R18 technische Integration/Randfälle; R19 Audit und Locks. |
| 591 | Später: eigene Tattoo-Runde als **Runde 17**, daher insgesamt **20 Runden**. |

Ergebnis: Modul 17, Abschnitt 1 hält die **endgültige** Zählung fest (R16 Master, R17 Tattoos, R18 Steuerung/UI/VR, R19 technische Integration, R20 Audit/Production Lock).

## 7. Runde 14: Körper, Tattoos, Piercings

### 7.1 Runde 14 (35#499–503, 100 Fragen)

Blöcke: A Körperaufbau; B Gesicht/Haut; C Brust/Gesäß/Weichgewebe; D Adult-relevante Anatomievariation; E Behaarung/Grooming; F Tattoos (76–100).

Antworten des Entwicklers (500): 1–3 ja; 4 Becken breiter/Hüftdellen; 5–29 ja; 30 Sonne ist gut für den Körper; 31 Brust kontinuierlich, sehr große möglich, nicht 10 Größen; 39–41 Training und kosmetische Operationen (Brust, Gesäß, Gesicht und mehr); 42–50 ja; **51 (individuelle Genitalanatomie, „Gebrauchsspuren“ u. a.)** → Assistent 501 korrigiert: **kein Nutzungszähler**; Zustand ändert sich über physiologische Faktoren, Verletzung, Heilung; 502: Entwickler wünscht sehr harte Praktiken bei manchen Erwachsenen im Spiel, später genauer; Assistent 503: Verletzungs-/Schmerz-/Gewebesystem greift, kein „weil gewollt, passiert nichts“; Ausgestaltung ohne Real-Life-Anleitungen. 52–61 ja; 56 derselbe Charaktereditor, erweitert; 62 **Beschneidung ‚eher nicht, nur gesundheitlich, Welt zu 99,9 % christlich‘** (Assistent: Realismus anders; Abweichung bewusst) → [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md); 63–75 ja.

Tattoos 76–100 (Antwort 500): überall (inkl. Mundinnenseite; Augen auf Nachfrage), je mehr Tattoos desto seltener zufällig außer an passenden Orten, Spanne 0 bis voll bei gleichem NPC, Sleeves teils einzeln teils aus Motiven, Lücken füllen, 100 % bis zum letzten Millimeter, Studios (Metropole ‚sicher 20–30 oder mehr‘), Qualität (z. B. Gefängnistattoos schlecht), Künstlerprofil, Verblassen erst nach Jahren, UV, Haut, Deformation, Schwangerschaft, Narben überstechen, Entfernen per Laser, spontan aber meist mit Termin, Bedeutung/Reue/Partnername, Online-Buchung. Assistent 501: Lebenszyklus frisch→Heilung→gesetzt→verblassen→Nachstechen/Überstechen/Laser; Studiotypen und Websites. Entwickler 502–503: Spieler programmiert Webseiten (stark vereinfacht); Assistent: eigener Website-Builder in C++ (Vorschlag). 504–507: Wirtschaft, Casinos usw. später, nicht Adult.

### 7.2 Runde 14B Piercings/Körpermodifikation (35#509–513, Fragen 101–140)

Jede plausible Stelle, viele Piercings, persistent, austauschbarer Schmuck (Form, Größe, Material, Gewicht, Zustand), Unverträglichkeit, Heilung, Komplikationen, Narben, Physik/Kleidung, Hängenbleiben (selten), Entfernen vor Sport/Kampf, Dehnung, Split Tongue/Scarification/Implantate selten und subkulturabhängig, Spezialstudios, Online-Buchung, kosmetische Chirurgie über Kliniken, Eingriff ändert Generatorparameter, Erholung, Arztqualität, Revisionen, NPC-Entscheidung aus Persönlichkeit/Geld/Umfeld/Körperbild, Intimoptionen im Editor bei aktivem Mod, 140 gemeinsames Aussehens-/Körperzustandssystem.

Entwickler 512: 101 „auch auf der Vagina oder anderen Stellen, ich kenne mich nicht aus“ (Assistent 513: äußere Genitalregion; keine beliebigen Stellen); 109 billig ≠ schlechter; 124 „und noch vieles mehr“; 125 selten; 131 Kliniken ja; sonst ja. 513: „14C Finaler Lock“ als Konsolidierung (Vorschlag).

## 8. Runde 15: Praktiken, Präferenzen, Gesellschaft

Rundenplanung: 509–511 R15 ca. 100 Fragen; 518 und 520 immer 25er-Pakete, über 100 erlaubt, ‚solange wirklich alles beantwortet wird‘. Endstand: **175 Fragen (Entwickler) bzw. 200 (Assistent), siehe [K10](19-Konflikt-und-Widerspruchsmatrix.md#k10-fragenzahl-runde-15).**

### 8.1 R15A (523–525, Fragen 1–25)

Mehrdimensionales Präferenzprofil (kontinuierliche Stärke), partner- und situationsabhängig; Neugier getrennt von Erfahrung; Erfahrung pro Bereich; Wissensstand (unbekannt/vermutet/direkt erzählt/gemeinsam erlebt); falsche Annahmen; Präferenzen im Dialog; Grenzen unterschiedlich tief; Intensität getrennt von Praktik; Dominanz nicht binär; Initiative ≠ Dominanz; Rollenspiel als System; Items als persistente Objekte; Shops; mehr als zwei Beteiligte von Anfang an.

Entwickler 524: 1–2 ja; 3 (eine unterwürfige Figur lässt bei einem Partner viel zu, bei anderen selten) → Assistent 525: partner-/situationsabhängig, Kapazität und harte Grenzen bleiben; 4 ja; 6 allgemeine Erfahrung plus Bereiche; 9 seltene Interessen selten, Stripclubs höher; Darstellerinnen können „extrem“ sein wie andere auch, Seiten in der Spielwelt, Spieler kann mit ihnen schlafen (zahlen oder überzeugen) → Assistent: Branche existiert, **Beruf ≠ privat verfügbar**; 10 Präferenzen sehen: nur der Entwickler im Entwicklungs-/Debugmodus („Adult Debug Inspector“); **18 Rollenspiel ‚wie bei Produktionsfirmen, gespielte Vergewaltigung, hochgeladen‘ → vereinbartes Rollenspiel zwischen Erwachsenen, als solches gespeichert ([K4](11-Offene-Fragen-und-Konflikte.md))**; 21 Items „können nicht einfach spawnen“.

### 8.2 R15B (525–527, 26–50)

Gruppen mit paarweisen Beziehungen, Beteiligungsgrade, Eifersucht trotz Zustimmung, offene Beziehungen/Poly, Swinger-/Adult-Clubs, funktionale Stripclubs mit Schichten, Darsteller als persistente NPCs mit Karriere, Creator-Plattform, Spieler als Creator, gemeinsame Produktion, Studios, mehrdimensionale Reputation, mehrere Adult-Websites, nutzergenerierte 18+-Seiten, Shops (Preis ≠ Qualität), Itemgröße kontinuierlich, Materialphysik, Verschleiß, Hygiene.

Entwickler 526: 26 ‚bei Gruppensex wohl nur zum Spaß; bei zwei Frauen und einem Mann können beide den Mann lieben und er beide‘; 28 ja; **32 Stripclubs „essentiell“, in der Metropole alle 15–30, Kleinstädte weniger, ein riesiger mehrstöckiger mit Premium-Frauen** (Assistent: Stripclub / Nightclub-Adult Club / Brothel-Escort, ein Adult Entertainment Complex); 34 ‚man kann sie dazu bringen, sie muss es von sich aus entscheiden‘; 41 jede Seite eigene Videos (Speicherfrage → Assistent 527: Szenen-/Replaydaten + Cache, Cachelimit einstellbar); 44 ja; 45 „wie in echt“. R15C-Entwickleranmerkung 528: Nagelstudios usw. (Hauptspiel).

### 8.3 R15C (527–529, 51–75)

Ortsrisiko, Exhibitionismus/Voyeurismus als Präferenz, Sichtlinie/Fenster/Licht/Geräusch, Clubbereiche, Rechtslage (privat/legale Venues/illegal öffentlich), Kameras und Smartphones als Quellen, **Aufnahme und Veröffentlichung getrennte Zustimmungen** (privat/Partner/Creator-Seite/kommerziell, Meinungsänderung vor Veröffentlichung), Verträge, Amateur vs. Studio, Nischenmarkt, Popularität multifaktoriell, Ruhm mit Nachteilen, Metadaten + Replay statt fertiger Videos.

Entwickler 528: 41 ‚Video soll jedes Mal das gleiche sein‘ (→ feste Replay-ID); 44–45 ja, Verletzungen möglich (Entwickler: manche gehen das Risiko ein); 51–54 ja; 55 „kann sein“; 56–72 ja; 73 Fans wollen Foto/Chance; 74 „immer das gleiche Video“; 75 ja.

### 8.4 R15D (529–531, 76–100) und Konsequenzteil

Körperbereichspräferenzen, Grooming, Kleidungsstücke/Materialien, Führen/Unterwerfen fein abgestuft, Intensität ≠ Kontrollabgabe, **einvernehmliches Fesseln als eigenes System mit physischen Gegenständen und Constraints**, Augenbinde mit Wahrnehmungseffekt, Rollenspiel mit getrenntem Weltzustand, Zuschauen/Beobachtet-Werden getrennte Präferenzen, KI schätzt Belastung/Verletzungsgefahr, keine Obergrenze der Vorlieben, **100: Präferenzmodell = Interesse + Stärke + Erfahrung + Neugier + gewünschte Intensität + Partnerkontext + Situationskontext + Risikoakzeptanz + Wissensstand + Erinnerungen**.

Entwickler 530: 44/45 ja; 76–95 ja; 96 Gefahren passen sich Übung an; 98 „eine Frau kann theoretisch alles wollen, was der Mann will“; **zusätzlich: Spieler kann Sexualstraftaten begehen und ins Gefängnis kommen, keine Einschränkung bei Verbrechen, im Spiel nicht erklärt ([K3](11-Offene-Fragen-und-Konflikte.md))**. Assistent 531–537: Crime-System ohne künstliche Verbrechensliste; schwere Sexualstraftaten **nur als Konsequenzebene** (Schreie, Zeugen, Spuren, Erinnerungen, Polizei, Verfahren, Haft, Ruf), keine Ausführungsmechanik, nicht erotisiert, kein Menü/Tutorial. Entwickler 532: konservative Welt; Selbstverteidigung geschlechtsunabhängig; Strafsystem realistisch. Assistent 533–537: Fraktions-/Moral-/Aggressionssystem statt Hardcode; Welt-Simulationsprinzip ‚logisch statt emotional‘; Entwickler 534/536: keine Seite schlechter, „die Welt soll hauptsächlich logisch denken“ (bestätigt).

### 8.5 R15E (539–540, 101–125)

Solo-Verhalten als NPC-Zustand, Medienpräferenz getrennt vom Verhalten, private Konten/Verlauf, Abos/Trinkgeld, parasoziale Beziehungen, Escort/Bordell/Unabhängige/Performer/Club differenziert, Sexarbeiter als individuelle NPCs (‚bezahlt ≠ alles erlaubt‘), marktbasierte Preise, Premium/VIP, Geld ersetzt keine Attraktion, Karrieren, Konkurrenz, Trends, Alterspräferenz innerhalb 18+, Körpermerkmale, Persönlichkeit vs. Optik, Präferenz-Interaktionen, 125 Präferenzframework ohne festes Ende.

Entwickler 540: 101 ja; 102 Solo-Häufigkeit variabel, **Zeitmaßstab: 30 Minuten Realzeit dürfen nicht 12 Stunden Spielzeit sein – muss festgelegt werden** (Assistent 541: nahe 1:1, größere Sprünge nur bei Schlaf/Reise/Warten; **Vorschlag**); 106 später; 110 mehrere Bordelle mit Premium; **111 manche Tarife „alles erlaubt“, manche ‚bis zur Ohnmacht‘** (nicht übernommen; Assistent 541/543/545: Bewusstlosigkeit = Capacity-Stop, Entwickler 544: Stopp-Logik greife nicht, der Spieler spüre es → Assistent 545: **nein**, Interaktion läuft technisch nicht weiter); 117 Scheitern selten; **121 Bordell-Bestellung per Website/Telefon, Langzeitvertrag mit Haushaltsleistungen, ‚frisch 18, Pass kontrollieren‘** ([K6](11-Offene-Fragen-und-Konflikte.md)); 125 „was würdest du machen“ (→ PreferenceRegistry).

### 8.6 R15F (541–543, 126–150) Gesundheit

Verhütung als Items/Zustände, Anwendungsfehler, Fruchtbarkeit, abstrahierter Zyklus, Schwangerschaft nur bei passender Biologie, Tests, medizinische Versorgung, **STI (137–139)**, Hygiene, Duschen, Flüssigkeitssimulation auf Material-/Physikbasis, Erschöpfung, kumulative Verletzungen, 150 nie Minispiel.

Entwickler 542: 126 ja, Kind kann Junge oder Mädchen sein; 129 ‚Pille kann die ganze Persönlichkeit verändern‘ (Assistent: beeinflusst Stimmung/Libido); 137–139 **‚nein, würde ich jetzt nicht machen, das brauchen wir nicht, ist eh schon genug‘** → [E-GE-004](14-Entscheidungsregister.md#7-ge-gesundheit-verletzung-fortpflanzung) (kein detailliertes STI-System in der Basisversion; Architektur lässt Erweiterung zu); 149 kumulative Verletzungen (‚keine Grenzen vom Spiel aus, der Charakter entscheidet‘).

Vertragsdiskussion 542–549 ([K6](11-Offene-Fragen-und-Konflikte.md)): Entwickler wollte, dass der NPC in „99,99999999 %“ der Fälle nicht kündigt; Assistent 545: nicht hardcoden, Verbleibmotivation aus Bezahlung, Beziehung, Lebensqualität, Sicherheit, Vertrauen minus Konflikte; 547: Kündigung jederzeit möglich, Wahrscheinlichkeit extrem niedrig; Entwickler 548: bevorzugt, dass der Spieler kündigt; 549: NPC behält technisch das Kündigungsrecht, Eigenkündigung muss aus Präferenz-/Beziehungszustand stammen, nicht aus unsichtbarem Zwangsflag.

### 8.7 R15G (549–551) und R15H (551–553)

R15G (151–175): Orientierung (romantisch/sexuell getrennt), kontinuierliches Profil, Liebe vs. Lust, Paardynamik, Libido, Spontaneität, Dauer, Pausen, Orgasmus als Reaktionsprofil (kein Erregungsbalken), freiwillige Orgasmuskontrolle, Geruch, Parfüm, Stimme, Haare, Nägel, Lieblingsgegenstände, Fantasien nie umgesetzt, partnerspezifische Offenheit. Entwickler 550: 151 ja; **152 ‚es darf keine LGBTQ-Inhalte geben‘ + zynischer Zusatz; 552: „das war ein Spaß, ich will LGBTQ nicht im Spiel haben“** → [K5](11-Offene-Fragen-und-Konflikte.md); 153–175 ja. Assistent 551: Orientierung als datengetriebener Zustand, stark heterosexuell geprägte Demografie möglich, Gewalt wegen Orientierung keine gewünschte Weltregel.

R15H (176–200): Kompatibilität aus Faktoren, Paare mit unterschiedlichen Vorlieben, Partnerwissen lernen (fehlerbehaftet), Attraktion ändert sich mit Aussehen, Bindung gleicht aus, NPC ändert sich optional für Partner, Vorfreude, Abstinenz, saubere Weltzeit (188), kein universeller Cooldown (189), kein sichtbarer Zähler, Vorlieben verbergen/offenbaren, Beruf ≠ private Vorlieben (194–196), Leistungsprofil im Vertrag (197), nichtsexuelle Begleitung (198), Beziehungen aus Kommerz (199), kein einzelner Adult-NPC-Typ (200). Entwickler 552: 176–190 ja; **191 ja + unabhängiges globales Statistik-Menü (wie viele Frauen, Internetverbrauch, Geld, Dämonen getötet), „richtig komplex“**; 192–200 ja. 553: Runde 15 abgeschlossen.

554–555: ZIP-Lieferungen (Delta und Vault-Merge), **nicht ausgewertet**.

## 9. Runde 16: Master-Runde

Planung 556–561: mindestens 200 Fragen in 25er-Paketen; „Runde 15 = Breite, Runde 16 = Tiefe/Kategorisieren/technisch definieren/Lücken schließen“; Taxonomie-Skizze 561 (Vorschlag). 562–573 Motorsport (nicht Adult). Entwickler 572: ‚Runde 16 soll wirklich alles sehr genau haben: Kategorie, Intensität, Ort …‘.

### 9.1 R16A–C (573–591, 1–75)

- **16A (573–580):** Mehrfachkategorien, Unterkategorien, Tags, mehrachsige Intensität (Geschwindigkeit, Kraft, Druck, Umfang, Rhythmus, Dauer, psychologisch), lokal pro Kontakt, dynamisch, gewünscht vs. tatsächlich; Ortstypen; kein Adult-Slot im Raum; Teilnehmer variabel; Rollen pro Moment; Initiative pro Aktion gespeichert; hierarchische Körperfokus-Taxonomie; Aktion = Typ + Zielregion + Parameter; Items als Teilnehmer/Modifikator; Kleidung kontinuierlich; Privatheitsabsicht vs. Entdeckungswahrscheinlichkeit; mehrdimensionales Risiko; Erfahrung in 3 Ebenen; Zustandsfelder inkl. hard/soft limit; nachträgliche Sitzungsklassifikation; 25 alle Dimensionen orthogonal. Entwickler 574: 1–4 ja; 5 Intensität sehr oft anpassbar (Steuerung unentschieden); 7 **‚gewünscht vs. tatsächlich‘: unabsichtlicher Kontakt möglich** (Assistent 575/577: versehentlicher Kontakt ≠ Zustimmung zu neuer Handlung); 10 ‚Adult überall machbar‘; 25 bestimmte Kategorien und Reaktionen „100 % haben“.
  - **576/578 (Entwickler):** „Wenn sie nichts sagt oder stöhnt, ist es okay und man kann weitermachen.“ → Assistent 577/579: nicht ‚Schweigen = Ok‘, sondern **Sitzungs-Absichtsrahmen** (Session Intent Envelope) mit Live-Signalen; Stöhnen allein genügt nicht. **580 Entwickler: „Ja ok aber nicht zu streng.“** 581: keine Pop-ups, klares Nein/Stopp/Wegdrücken zählt sofort, ruhig und beteiligt muss nicht unterbrechen. → [K1](11-Offene-Fragen-und-Konflikte.md), [E-SK](14-Entscheidungsregister.md#2-sk-sicherheitskern-alter-zustimmung).
- **16B (583–585, 26–50):** Basiskategorien als mehrfach zuordenbare Familien (Küssen/Berühren → Manuell → Oral → Penetrativ → Toys/Items → Impact → Fesseln/Kontrolle → Sensorik → Rollenspiel → Gruppe → Beobachtung/Aufnahme → Hochrisiko → Nachsorge); **Schmerz als Dimension, nicht Kategorie**; Hochrisiko-Tag verbietet nie automatisch, gewichtet Gesundheit/Verletzung. Entwickler 584: großteils ja; mehrere Punkte ‚sehr wichtig/muss perfekt sein/sehr genau und aufwendig simuliert‘ (Detailwünsche, hier nicht wiedergegeben); 37 **keine Größengrenzen außer physikalisch unmöglich**; 38 Impact: Rötung, Narben, Bluten; 42 „was ist das“ (Sensorik, 585 erklärt); 43 Rollenspiel inkl. gespielter Nicht-Einwilligung; 44 Gruppen, Mann+Frau zuerst; 48 Hochrisiko ja. Assistent 585: sehr extreme fiktive Szenarien als Rollenspiel klassifizierbar; **tatsächliche nicht-einvernehmliche Gewalt bleibt im Crime/Justice-System**; „Master Category Registry“ als 16I geplant.
- **16C (589–591, 51–75):** Haltungsfamilien nur Klassifikation/Solver-Hilfe; Mischformen; Orientierung kontinuierlich; Körperteile unabhängig; Schwerpunkt; Komfort vs. möglich; Größe/Gewicht/Kraft/Flexibilität; Möbel als Interaktionsflächen (Bett verformt, Sofa, Stuhl, Wand/Tür, Fahrzeuge, enge Räume, Boden/Outdoor); freie Wechsel ohne Szenenreset; Favoriten; Positionsregister als offene Bibliothek. Entwickler 590: alle ja; 65 Matratze/Bett kann kaputtgehen; 75 UFC-artiges System (Aktion mit Tastenkombination und Vorschau), noch nicht fix. Assistent 591: Möbelschaden-/Strukturschadensystem.
- **590/591:** Entwickler: „komplette Runde nur für Tattoos, Piercing, Bodymods, Tattoos 90–95 %“ → Runde 17.

### 9.2 Fortschrittsschätzungen und Unterbau (594–608)

594–604: Fortschrittsschätzungen des Assistenten (Planung ~40 %, Vision ~80 %) sind **Einschätzungen, keine Entscheidungen**. Entwickler 598: alles genau definieren (Welt, Gameplay, Figuren, Adult, Orte), extrem viele Bilder (Visual Bible), Story 80–100 h, Karte später. **604: „Die Adult-Mod soll auch schnell verfügbar sein, noch vor manchen anderen Systemen“** ([E-PR-006](14-Entscheidungsregister.md#15-pr-produktion-qualität-reihenfolge)). 605: Unterbau-Liste für Adult V1 (Kern, Menschengenerator, Rendering, Animation+IK, Interaktion/Kontakt, Weichgewebe, NPC-Intent, Beziehung/Gedächtnis minimal, Eingabe/UI, Modlader, Audio/Mimik, Gesundheit, Testwohnung), grob 120–250k Zeilen (Schätzung).
608 (17.09.): Entwickler-Kontextprompt: Runde 15 „175/175 fertig“; 16A–C fertig; weiter ab Frage 76; Runde 16 ‚größte Adult-Runde, keine festgelegte Endzahl, keine Füllfragen‘; Lifetime-Statistiksystem als **Core-System** beschlossen.

### 9.3 R16D–H (609–627, 76–200)

- **16D (76–100):** stufenlose Intensität; Geschwindigkeit ≠ Kraft; Amplitude; Beschleunigung; direkte Controllersteuerung; UFC-artiges Kontextmenü; UI abschaltbar; Spamming nicht optimal; Rhythmus-Assistenz; gemeinsame Bewegung (Solver kombiniert Intents); Körpergewicht; lokaler Druck; Toleranzkurven pro Region; Gewöhnung ohne Magie; Reaktion sekundengenau; Schmerz ≠ Ablehnung; Performance-LOD; Determinismus; Interaction Event Stream. Entwickler 610: 76–80 ja; 81 Kombos mit Tastenkombinationen, alle verfügbaren im Menü ansehbar; 82–100 ja.
- **16E (101–125):** Atmung, Stimmprofil, Lautstärke, Latenz, gewichtete Mimik, Blick, Tränen, Würgereflex-Kette (Hals/Kiefer/Atmung/Husten/Tränen/Gesicht/Stimme/Übelkeit), Übelkeit als kontinuierlicher Zustand mit Schwelle, Erbrechen als Welt-/Flüssigkeitsereignis, **117: Content-Toggle für starke Körperreaktionen**, lokaler Schmerz, Muskelspannung, Zittern, Orgasmus als Multi-System-Ereignis, 125 Unified Reaction Graph. Entwickler 612: ja.
- **16F (126–150):** Gewebezustand pro Region, Dehnung, Langzeitanpassung nur im biologischen Rahmen, Mikrotrauma, Heilungsstadien, Narbengewebe, Schmerzgedächtnis, Schmerztoleranz ≠ Schadensreduktion, Kontaktstress, Scherkraft, Reibungsschaden, Erholung, deterministischer Speicherzustand, validierte Mod-Grenzen, 150 Unified Body State. Entwickler 619: ja; ‚beim normalen Spiel auch anwendbar‘; Frage „Wofür brauche ich dann Qwen?“ → Assistent 620: Qwen als Spezialist (Inhalte, synthetische Daten, Lückenprüfung, lokale Batch-Produktion), nicht für Kernarchitektur.
- 621–623: Kategorien noch nicht ausformuliert; Master Category Registry (16I+); Qwen als Lückenfinder (‚ändert nichts am Kanon‘); Entwickler 623: „Machen wir 16 mal fertig und dann machen wir das mit Qwen“.
- **16G (151–175):** Partnerprofil, Attraktion getrennt von Präferenz, Vertrauen, Bindung, aktuelles Verlangen, Initiativprofil, Lernen durch Beobachtung/Gespräch mit Konfidenz, Fehlerlernen, Erfahrungsgedächtnis, Gewohnheit, Langeweile, Neuheitssuche, Stimmung/Stress, Konflikt, Eifersucht, Vorschläge mit Kontextbewusstsein, Stabilität, Persistenz, Mod-API. Entwickler 625: ja.
- **16H (176–200):** Setup-Assistent; Einstellungskategorien; Presets + Erweitert; UI-Hilfen 4-stufig; Kontextmenü filterbar; Favoriten; Statistik über zentralen Eventstrom; Adult-Statistik nur bei aktivem Mod; Replay ≠ Savegame; Cinematic Re-Render im Hintergrund; VR gleicher Kern; **Mehrspieler: Adult-Zustand nicht nur lokal, autoritativer Server, Adult nur vom Host aktivierbar (196)**; Schemaversion; Registry; Abhängigkeiten/Konflikte; Debug-Inspector. Entwickler 627: 176–180 ja; 181 ‚weiß noch nicht, glaube gut‘; 182–200 ja.
  - **627 (Entwickler):** Wunsch nach Brutalität ohne Zustimmung der Partnerin im Adult-Kontext, mit Konsequenzen → Beginn des Konfliktblocks [K2](11-Offene-Fragen-und-Konflikte.md).

### 9.4 Der Konfliktblock 628–660 (20.09.)

| Nr. | Entwickler | Assistent |
|---|---|---|
| 628 | – | R16 ~300–350 Fragen (16I–L); einvernehmlich extrem hart = detailliert simulierbar; **Nicht-Einwilligung gehört ins Crime/Justice-System**, nicht in Preference/Scene-Mechanik. |
| 629, 631 | ‚wenn sie vorher nicht gesagt hat, dass das nicht okay ist‘; „man kann es trotzdem machen und die Reaktion sehen“ | 630/632: nicht ‚nicht verboten = erlaubt‘; positive Reaktion macht Grenzübertritt nicht rückwirkend ok. |
| 633 | „Markiere das als von mir festgelegt“ | 634: Formulierung „USER-LOCK“ mit Crime-Konsequenzen. |
| 635 | Weitermachen auch gegen den Willen, ‚schlimmstenfalls Gefängnis‘, nur in der Admin-Version des Entwicklers | 636: „nur interne Admin-/Developer-Version“, keine Gameplay-Mauer, Konsequenzen simulieren. |
| 637, 639 | Gleicher Funktionsumfang wie das Normalsystem | 638/640: gleiche technische Tiefe möglich, aber **kein** gleich großer expliziter Non-Consent-Katalog. |
| 653 | Einschlafen der Partnerin: weitermachen nur in Admin-Version | 654/656: **abgelehnt, auch für Admin-Build**; Alternative: vereinbartes einvernehmliches Schlafspiel (Zustand, Atmung, Aufwachen). |
| 655 | „Ich habe vorher Sachen geplant, die du nicht komplett willst, jetzt will ich das mit dem Schlafen auch. Also mach das so.“ | 656: abgelehnt. |
| 657 | „Ja ok dann plane ich das dann halt mit Qwen.“ | – |

**Dokumentationsentscheidung:** Der Block ist als **historischer Konflikt** dokumentiert ([Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md)). Er wird nicht als Anforderung übernommen, es gibt dazu keine Mechanik, keinen Testfall und keine Abnahmekriterien. Die Rückfrage, ob der Entwickler die Forderung zurückzieht, bleibt offen ([K2](11-Offene-Fragen-und-Konflikte.md)). 657–664: Assistent erstellte ein Master-ZIP mit einer Datei `01_ADULT/USER_REQUESTED_BUT_NOT_CANONICALIZED.md`, die **im Archiv nicht auffindbar** ist ([Modul 13](13-Quelleninventar-und-Abdeckung.md)).

### 9.5 R16I–L (642–677, 201–300)

- **16I (201–225) Registry I:** Familien (Küssen/Berühren, Manuell, Oral, Penetrativ, Toys, Impact, Fesseln, Sensorik, Rollenspiel, Gruppe, Solo, Beobachtung/Aufnahme, Berufs-/Dienstkontext, Nachsorge); Mehrfachzuordnung; Aktion = Typ + Zielregion + Werkzeug/Körperteil + Parameter; Gruppe als Teilnehmerschicht; Solo gleiche Registry; Dienstkontext = Vertrags-/Serviceschicht; **225 Registry-Schema** (ID → Kategorie → Unterkategorie → Tags → Zielregionen → Teilnehmer → Körperteil/Item → Intensitätsachsen → Positionsanforderungen → Ortsanforderungen → Reaktionshaken → Risikohaken → Präferenzhaken → Gedächtnis/Statistik-Haken → Replay/Modding-Metadaten). Entwickler 643: ja; 644: Schema ziemlich fest, Kategorienamen nicht final.
- 645–648: Qwen-Phasen (nach R16 Lücken; nach R17 Tattoo-Lücken; später Inhalte/Testdaten); „Ok passt“.
- **16J (226–250):** Unterkategorien (Küssen, Berührungsarten, manuell außen/innen, Handkonfiguration, tiefes Oral mit Parametern, Toy-Geometrie, Beißen/Kratzen/Greifen, Fesselmaterialien, Sensorik), Fluid-/Reaktionstaxonomie als Outcome-Schicht; 250 Unterkategorie nur, wenn sie technisch, präferenz-, statistik- oder gesundheitsrelevant ist. Entwickler 651: ja; vage formuliert, 16K zuerst für Reaktionen.
- **16K (251–275):** Reaktions-/Outcome-Familien (u. a. Würgen, Brechreiz, Übelkeit, Erbrechen, Husten, Atemnot, Atemunterbrechung, Tränen, Speichel, Schweiß, Hautrötung, Schmerz-/Lustlaute, Zittern, Erstarren), Fluid Registry, Reaction Composition. Entwickler 653: ja mit sehr detaillierten Zusätzen (hier nicht wiedergegeben).
- **16L (276–300):** lokale Lustkarte, Schmerz und Lust gleichzeitig, Sensibilisierung/Desensibilisierung, Mixed State, Orgasmusmodell und Profile, Refraktärphase, Nachglühen, Ermüdung pro Muskelgruppe, Krämpfe, Muskelkater, Schwellung, Bluterguss, kleine Oberflächenverletzung, kumulative Erholung, Alltagseffekte, Selbstpflege, medizinische Eskalation, Erholungsprognose (nur Inspector), 300 Unified Outcome Layer. Entwickler 677: ja.

### 9.6 R16M–R (678–715, 301–437)

- 679–680: **16M (301–325)** konkrete Unterarten als Struktur (Küssen, Lecken, Saugen mit Tiefe, Hand, Finger, Faust-Untergruppe getrennt, orale Familien, Kernaktionstypen mit freier Pose, Toys, externe Toys, Mehrfunktionsitems, Körperteil-Familien, Mixed Actions); 325 Granularitätsregel. 680: Zählung 325; geplant 16N–16R, R16 ~425–475.
- 683 (23.09.): Entwickler: ‚letzte Tage keine Lust, heute ein bisschen‘.
- **16N (686–690, 326–350):** Höhepunkt als Ereignis innerhalb der Session (nicht Szenenende); Ejakulation getrennt vom Orgasmus; Menge individuell; Zielregion/Fläche/Item exakt gespeichert; innerer/äußerer Flüssigkeitszustand; Übertragung, Absorption, Trocknen, Aufräumen über normale Weltaktionen; Kleidung (Schicht, Verschiebung, Schaden, Materialgefühl); Nacktheit = normaler Kleidungs-/Sichtbarkeitszustand; Solo mit denselben Typen; Medien + Solo; Körper-zu-Körper-Familie; 350 Missing-Family-Audit. Entwickler 687: 326–349 ja; 350 „was genau meinst du“; **Flüssigkeitssysteme „erst ganz spät zu implementieren“** → Assistent 688: Fluid-/Politur-System **Late Implementation** (Early: Interaktionskern, Pose/IK, Körperzustand, NPC-Intent, einfache Reaktionen; Mid: Kleidung, Items, Verletzungen, Präferenzen, Orte, Replay; Late: detaillierte Flüssigkeiten, Absorption, Transfer, Aufräumen, High-End-Rendering). Entwickler 689–690: „Ja machen wir das extra“ → eigener Missing-Family-Audit am Ende von R16, danach Qwen.
- **16O (692–694, 351–375) Orte:** Ort = Umgebungskontext, kein Szenentyp; jeder Wohnraum geeignet; Bettgröße/Matratze; Bad/Dusche; Hotels (Schallschutz, Privatsphäre); Fahrzeuge anhand realen Innenraums; Outdoor, Untergrund, Wetter, Tageszeit/Beleuchtung bestimmen Sichtbarkeit; dynamischer Privacy Score; Schallausbreitung; Zeugenerkennung (Sichtlinie + Gehör + Aufmerksamkeit); NPC-Zeitpläne; Türen/Schlösser/Vorhänge als Weltaktionen; Adult-Venues mit Raumtypen/Hausregeln; Studios; Arbeitsplatz mit höherem Risiko; ungewöhnliche Orte; Flächenregister; zerstörbare Umwelt; Aufräumen/Persistenz; Ortsgedächtnis; **375 Adult braucht keine speziell gebauten Räume**. Entwickler 693: ja.
- **16P (694–696, 376–400) kombinierte Aktionen und Randfälle:** parallele Aktionen, Prioritäten, Aktionsschichten, Wechsel ohne Reset, unterbrochene Übergänge, NPC übernimmt Teilaktion, Konflikt der Absichten, Handwechsel, Item-Transfer/-Verlust, Umwelt-Unterbrechungen (Klingeln, Eintreten, Stromausfall, Feueralarm, Monsterangriff), Pause, Kleidung während der Interaktion, Verletzung/Krampf ändern die Pose, Möbelbruch, Platzmangel, Selbstkollision, Mehrkörper-Kollisions-LOD, Fehlerzustände, 400 Randfall-Prinzip. Entwickler 695: 376–400 ja; „zu genau geplant, das juckt keinen, einfach wie in echt“. **696/698: Default-Lock nur für Adult – ohne Sonderregel verhalten sich Körper/Physik/Material/Umgebung so realistisch wie in der Realität; Mikrophysik wird nicht mehr einzeln abgefragt.** Entwickler 697: „Das gilt jetzt für Adult. Für Normal kann das noch interessanter werden.“ → [E-PR-014](14-Entscheidungsregister.md#15-pr-produktion-qualität-reihenfolge).
- 699–704: Zeitplan der Planung ‚etwas mehr als 3 Monate bis Jahresende‘; Monatsplan (Vorschlag).
- **705 (24.09.):** Entwickler-Kontextprompt: R15 „175/175“; R16 Fragen 1–375 abgeschlossen; 16P ‚vorbereitet, nicht vollständig beantwortet‘. **Widerspruch** zu 695 (dort „376–400 ja“) → [K11](19-Konflikt-und-Widerspruchsmatrix.md#k11-zählung-r16p). 706: Assistent stellt fest, 16P sei bereits beantwortet; neue Abhängigkeiten (Interaktionsplaner mit Prioritäten, Weltzustand, Persistenz/Replay, Netzwerk, Performance, automatisierte Stresstests) für den Audit markiert.
- **16Q (706–707, 401–425) Risiko/Gesundheit/Körperzustand:** Hochrisiko-Tag sperrt nie automatisch; Risiko aus Basisrisiko, Intensität, Anatomie, Erfahrung, bestehender Verletzung, Items, Umgebung; Risikoklassen (Gewebe, Gelenk/Muskel, Atmung, Impact, Reibung, Sturz/Umwelt, Ermüdung); Anpassung lokal; Erfahrung ≠ Widerstand; dynamische Verletzungsschwelle; Eskalation klein→groß; keine Heilung während der Session; Langzeitfolgen nur plausibel; Gesundheitszustand beeinflusst KI; Präferenz kann Risiko überwiegen; Notfallerkennung; **416 schwere Zustände → normales Medizinsystem ohne Adult-Sonderbehandlung**; subtile Risiko-UI (keine Dauerprozentwerte); Developer Health Inspector; **425 Unified Risk Pipeline** (Aktion + Intensität + Kontakt + Anatomie + Körperzustand + Umgebung → mechanische Last → Empfindung/Schmerz → Schaden → Reaktion → Erholung/Medizin → Gedächtnis) für Adult, Kampf, Sport, Unfälle, Medizin. Entwickler 707: 401–425 ja; ‚das meiste passt, Adult ist sehr langwierig, Fragen reduzieren‘.
- 708–712: Fragen werden komprimiert (R16 30–60; R17 60–90 statt 500; Entwickler 709 „nur die wichtigsten“, 711).
- **16R (712–715, 426–437, 12 Fragen):** Prioritätsklassen Core/Important/Late Polish; Realismus-Default; **Registry-Freeze erst nach Missing-Family-Audit und Qwen-Audit**; datengetrieben; geteilte Systeme; Veröffentlichungsumfang: erst kleiner hochwertiger Adult-Kern; modulare Inhaltseinstellungen ohne Speicherschaden; zentraler Adult-Debugger; automatisierte QA (tausende Kombinationen); feste Performance-Budgets; Persistenz nur Langzeitrelevantes; Production Gate. Entwickler 713: 426–427 ja; 428 ja („stelle ich am Sonntag auf oder später, aber wir gehen davor schon weiter“); 431 ja, „sehr früh implementiert und dann sehr viel erweitert“; 435 ja (Beispiel Battlefield); 436–437 ja. Entwickler 715 (Präzisierung): 431 „am Anfang nur normaler Sex, also Oral, vaginal und vielleicht anal, aber auch nicht das Wichtigste; am wichtigsten ist als Erstes der Ladebildschirm und ein kleiner Teil der Karte; Adult kommt erst danach“; 435: nicht jeder hat Spitzenhardware. → [E-PR-009, E-PR-010, E-PR-011](14-Entscheidungsregister.md#15-pr-produktion-qualität-reihenfolge). Am Ende von 715: Frage zur Rechtslage → [Modul 17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md#2-recht-ki-offenlegung-und-pre-audit-716748).
