# Adult 17: Interviewprotokoll Runden 17 bis 20 und Rahmenchats

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Protokoll der Planungsgespräche und Auswertung der Rahmenquellen, keine Spezifikation. Statuszeichen: [Legende](README.md#statuslegende). Rechtliche Aussagen des Gesprächsassistenten sind **ungeprüft** und keine Rechtsauskunft.

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Vorher: [Modul 16](16-Interviewprotokoll-Runden-11-bis-16.md). Quellenschlüssel: [Modul 13](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Einträge im [Register](14-Entscheidungsregister.md); Widersprüche in [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md).

**Inhalt**

1. [Endgültige Rundenzählung](#1-endgültige-rundenzählung)
2. [Recht, KI-Offenlegung und Pre-Audit (716–748)](#2-recht-ki-offenlegung-und-pre-audit-716748)
3. [Runde 17: Tattoos, Piercings, Körpermodifikation](#3-runde-17-tattoos-piercings-körpermodifikation)
4. [Runde 18: Steuerung, Oberfläche, VR](#4-runde-18-steuerung-oberfläche-vr)
5. [Runde 19: Technische Integration](#5-runde-19-technische-integration)
6. [Runde 20: Production Lock](#6-runde-20-production-lock)
7. [Nach dem Interview (813–951)](#7-nach-dem-interview-813951)
8. [Chat 28: Rahmenentwurf „Adult Content Framework“](#8-chat-28-rahmenentwurf-adult-content-framework)
9. [Chat 23: Darstellung, Plattformen, Dienstleistungs-NPCs](#9-chat-23-darstellung-plattformen-dienstleistungs-npcs)
10. [Chat 37: Tagesplan](#10-chat-37-tagesplan)
11. [Kanonische Dokumente](#11-kanonische-dokumente)
12. [Ausgeschlossene Quellen](#12-ausgeschlossene-quellen)

---

## 1. Endgültige Rundenzählung

| Runde | Thema | Fragen (laut Gespräch) | Abschluss |
|---|---|---|---|
| 1–8 | Grundsystem, Rollen, Kategorien, Präferenzen, Beziehungen, Initiative, Orte, Pakete | je 12–15 Themenpunkte | 11.09. |
| 9 | NPC-Erzeugung, Gedächtnis, Weltsimulation | 45 + 30 + 60 + 50 + 40 | 11.–12.09. |
| 10 | Körper und Szenensystem | 35 + 35 + 100 | 12.–13.09. |
| 11 | Freeform-Engine | 100 + 35 | 13.09. |
| 12 | Replay/Aufnahme/Streaming (Zusatzrunde, nicht Adult) | 100 (20 beantwortet) | 14.09. |
| 13 | Ablauf und Reaktionen | 100 | 14.09. |
| 14 | Körper, Tattoos, Piercings | 100 + 40 | 14.09. |
| 15 | Praktiken, Präferenzen, Gesellschaft | 175 (E) / 200 (A) | 16.09. |
| 16 | Master-Runde | 437 | 25.09. |
| 17 | Tattoos/Piercings/Körpermodifikation | 141 | 26.09. |
| 18 | Steuerung, Oberfläche, VR | 42 | 27.09. |
| 19 | Technische Integration | 32 | 27.09. |
| 20 | Production Lock | 15 | 27.09. |

Zusammen **ca. 1.900 Fragen** nach Zählung der Dokumentation ([Modul 13, Abschnitt 5](13-Quelleninventar-und-Abdeckung.md#5-abdeckungsmatrix-der-interviewrunden)). Die Zahlen für R15, R16P und Pre-Audit weichen zwischen Quellen ab ([Modul 13, Abschnitt 8](13-Quelleninventar-und-Abdeckung.md#8-bekannte-abweichungen-innerhalb-der-quellen)).

## 2. Recht, KI-Offenlegung und Pre-Audit (716–748)

### 2.1 Recht (35#715–720)

Der Entwickler fragt am Ende von 715 nach der Rechtslage. Assistent 716 (**ungeprüft**, mit Quellenangaben im Chat, Stand 09/2026):

- Basisspiel und Adult technisch trennen; Steam verlange die Angabe unerreichbarer Adult-Inhalte im Build.
- Österreich: Pornographiegesetz, Rechtsprechung zu „harter Pornographie“ bei Sexualgewalt, § 207a auch für künstlich erzeugte Darstellungen Minderjähriger.
- Build-Validator; Dreifach-Tor „Gesetz ∩ Store ∩ Zahlungsdienstleister“.
- Live-generierte KI-Adult-Inhalte seien bei Steam nicht veröffentlichbar.
- Deutschland: USK/IARC.
- IP: Name nicht „Supernatural“, Marken von Warner Bros.; Automarken lizenzieren oder erfinden.
- „Nicht-einvernehmliche explizite Sexualgewalt ist ein echter Legal-Risk-Block.“ „Nur Entwickler“ sei kein magischer Rechtsschalter. Medien-/Strafrechtler konsultieren.
- Architektur-Lock: Inhalte nach Store/Land/Altersfreigabe ein-/ausschließbar.

Entwickler 717: „wenn Adult nicht gut zum Veröffentlichen ist, mache ich es mal nur privat“; Autos nicht mit echten Namen. Assistent 718/720: Adult zunächst privat; KI-Assets deklarieren; fiktive Marken; Projektname bleibt Arbeitsname. 719: „ja genau“. → [E-RC-004, E-RC-005](14-Entscheidungsregister.md#16-rc-recht-plattform-ki-vertrieb).

### 2.2 KI-Offenlegung (721–741)

- 721–732: Steam-Offenlegung für KI-generierte Inhalte; Herkunftsregister je Asset (Mensch/KI-unterstützt/KI-generiert+bearbeitet/lizenziert/gekauft) (Vorschlag). Entwickler 727: „wie können wir machen, dass dort relativ wenig davon steht“; 729: „es wird fast alles mit KI sein“.
- **733: „ok das ist nicht so gut, ich will das eher geheimhalten“.** 734: Assistent: nicht verstecken oder falsch kleinreden, wahrheitsgemäß deklarieren. → [K7](11-Offene-Fragen-und-Konflikte.md), ❓.
- 735–741: Live-Generative-KI erklärt. Entwickler 737: „ja genau so habe ich das im Sinn“ (keine Live-Generierung im fertigen Spiel); 739–741: vorab generierte Assets seien veröffentlichbar, „da bin ich beruhigt“. → [E-RC-008](14-Entscheidungsregister.md#16-rc-recht-plattform-ki-vertrieb).

### 2.3 Pre-Audit (742–748)

Assistent 742: **Pre-Audit A1–A15** (Missing-Family-Audit I): Familienliste (Küssen/Berühren, Manuell, Oral, Penetrativ, Toys/Items, Körper-zu-Körper, Impact, Fesseln/Kontrolle, Sensorik, Solo, Gruppe, Rollenspiel, Beobachtung/Aufnahme, Dienstkontext, Nachsorge); Körperfokus als Ziel-/Präferenzfamilie; Kleidung nur Tags/Zustand; Machtdimension; Empfindung als Registrydimension; Fantasie vs. Aktivität; Medienproduktion eigene Kontextfamilie; Handel/Service als Vertragskontext; Physiologie-Ergebnisse (14 Familien); Sozialfolgen; Ortskontext; Zeitkontext; Teilnehmerstruktur; Inhaltsintensität nie Kategorie.

Entwickler 743: A1–A2 ja; A3 ok; A4 ja; A5 „genauer sagen“; A6 ja; **A7 „ja genau, anfangs klein und immer wachsend“**; A8 ja; A9 „nein, noch genauer“; A10–A14 ja; A15 „fällt mir gerade nicht ein, schlag was vor“; er habe gedacht, das mache man erst mit Qwen. Assistent 744: A15-Kandidaten (erotische Kommunikation, digitale Interaktion, Verzögerungs-/Steuerungsspiele, Körperverehrung, verbale Dynamiken, synchrone Aktionen, Schwangerschaftsfantasie als Rollenspiel-Tag getrennt vom echten Schwangerschaftssystem, Fetischobjekte/-materialien). **745: „a15 die Sachen passen gut“** → 746 als **Vorab-Erweiterungen/Kandidaten**, nicht eingefroren. 747: „ich sagte dir, dass ich Qwen jetzt nicht habe, also jetzt Runde 17.“ 748: R16 normale Planung bei Frage 437 abgeschlossen; Taxonomie-/Qwen-Audit später. → [Register E-IA](14-Entscheidungsregister.md#4-ia-interaktion-taxonomie-ablauf).

## 3. Runde 17: Tattoos, Piercings, Körpermodifikation

Detaillierte Spezifikation: [Modul 07](07-Charakteranpassung-Tattoos-Koerpermodifikation.md); Register: [E-TA](14-Entscheidungsregister.md#10-ta-tattoos-piercings-körpermodifikation).

| Teil | Nachrichten | Fragen | Inhalt und Antworten des Entwicklers |
|---|---|---|---|
| 17A | 748–752 | 1–20 | Tattoo als persistentes Objekt (Motiv, Position, Größe, Rotation, Künstler, Alter, Farben, Qualität, Verlauf); freie Platzierung auf Körperoberflächen; Abdeckung kontinuierlich; Überlappung, Schichten, Negativraum, Füllmotive; Sleeves als Komposition; Ganzkörperprojekte über Jahre; Anatomie und Körperänderung verformen; Narben; Dichte als NPC-Merkmal. Entwickler 749: alles ja; Erneuerung, Laser, Blackout, Cover-up; Spieler sagt dem Tätowierer, was er will; Schwangerschaft dehnt; einige NPCs Ganzkörper; **18: auf dem Land selten, bei Älteren ohne Haft selten, bei jungen Erwachsenen häufiger, in der Stadt mehr, wenige Ganzkörper** (Formulierung des Originals nennt „junge Mädchen und Buben“; die Dokumentation nimmt **nur** die Aussage über Verbreitung in der erwachsenen Bevölkerung auf). **20: Der Tattoo-Editor soll im normalen Spiel enthalten sein, auch mit Nacktheit** („soweit ich weiß nicht Adult-only, solange kein Sex dabei ist“). Assistent 750: Ansatz bleibt im Hauptspiel; Steam-Deskriptoren für Nacktheit/Sexualinhalte korrekt angeben; die Grenze „erst bei Penetration“ sei zu eng. Entwickler 751–752: „Achso ja“, Frage 13 ja. → [E-TA-015](14-Entscheidungsregister.md#10-ta-tattoos-piercings-körpermodifikation) |
| 17B | 760–764 | 21–40 | Stile kombinierbar; Realismus von Künstlerskill abhängig; Black & Grey, Farbe mit Pigmenteigenschaften je Hautton; Blackwork/Blackout; White-over-Black; Traditional/Neo-Traditional; Japanese als zusammenhängende Komposition; Fine Line; Geometrisch; Schrift mit Tippfehlern bei schlechten Künstlern; Porträts; eigene Motivkomposition; Interpretation vs. Vorlage; Schablone/Freihand; Ganzkörper-Masterplan. Entwickler 761: 21 „jeder NPC/Spieler entscheidet selbst, Blackout möglich, aber nicht schön“; 22 schlechter Tätowierer vs. Weltstar, Preise verschieden; 23–26 „alles, was man sich vorstellen kann“, Vorschau am eigenen Körper; 29 ja, Yakuza/Mafia; 30 junge Frauen, Schmetterlinge; 33 Porträts „kann sehr gut werden“; 34 kein komplettes Rückentattoo in einer Sitzung; 35 Spieler nennt Wunsch, Künstler macht Vorlage; 39 „100 % sehr wichtig“, einige Ganzkörperanzug, Stadt mehr als Land; 40 wirklich alles tätowierbar. Assistent 762: Technik `Design → Körperoberflächenmaske → Pigmentschicht → Hautshader` (Vorschlag), Fehlerbilder bei schlechten Künstlern. |
| 17C | 766–768 | 41–60 | Künstler mit mehreren Skills, Spezialisierung, Handschrift, Laufbahn, Preisformel (Stunde/Fixpreis), Weltklasse teuer, **Wartezeit 1–2 Wochen im Spiel, nicht zu lang (47)**, Terminplanung, Beratung, Anzahlung, Walk-ins, Studioruf, Studiotypen, Hygiene getrennt, schlechte Studios, Preis ≠ Qualität, Empfehlungen, Portfolio, berühmte Kunden, Künstlerwechsel. Entwickler 767: 41–44 ja; **45 auch Ort: Innenstadt viel teurer als Land**; 46 plausibel; 48 jedes Studio eigene Regeln; 49–59 ja; 60 ja, aber nicht immer sinnvoll (sichtbarer Stilbruch). |
| 17D | 768–770 | 61–80 | Sitzungen; Schmerz nach Körperstelle; Pausen; Heilungsphasen; Nachsorge; Kleidungskontakt; Wasser/Sonne; individuelles Heilen; unvollkommene Heilung; Nachstechen; Langzeitverblassen; Farbalterung; Auffrischen; Tattoo-Verlauf. Entwickler 769: 61 nicht zwingend, aber fast immer; **62 Zeitraffer statt fünf Stunden Zuschauen**; 63–80 ja. |
| 17E | 770–772 | 81–100 | Cover-up, Blackout, Laser, Körperveränderung. Entwickler 771: „passt alles, vieles selbsterklärend“. Assistent 772: **„Selbsterklärende reale Tattoo-Logik = automatisch übernommen.“** |
| 17F | 774–776 | 101–112 | NPC-Tattoo-Kultur. Entwickler 775: 101–102 ja; **103 ja, auch Tattoos mit Hasssymbolen extremistischer Gruppen** (→ Dokumentation: nur offene Frage zur Zulässigkeit, [E-RC-011](14-Entscheidungsregister.md#16-rc-recht-plattform-ki-vertrieb)); 104 teilweise; 105 „nicht einer in ganz Metropolis, sondern mehrere, stellen wir gleich auf wie viele Prozent“; 106 große Studios in der Metropole, Dörfer klein; 107 Spezialisierungen; 110 „manche sind danach süchtig“. Assistent 776: erste Baseline (**Zahlen sind Assistentenvorschlag**): Metropole erwachsen 0 % ~45 %, 1–10 % ~30 %, 10–30 % ~14 %, 30–60 % ~7 %, 60–90 % ~3 %, 90–100 % ~1 %; Kleinstädte 55/27/11/5/1,6/0,4; Land 70/21/6/2,3/0,6/0,1; Modifikatoren; Spawn-/Populationsgewichte statt harter Quoten. Entwickler 777: „passt“. **Zahlen nur vorläufig**, nicht ausdrücklich bestätigt. |
| 17G | 778–782 | 113–122 | Technik: Composer, Körper-Haut-Schichten, Virtual Texture, Pigment in der Haut, LOD, Generator aus Demografie, Komposition zuerst, virtueller Künstler, Motivwiederverwendung, **Import eigener Designs (120)**, Modding, Asset-QA. Entwickler 779: 113 „wie genau meinst du“ (781 „ja“); 114 ja (schwer); 115 ja (aus 10–20 m erkennbar); 116 ja, manche einfach „Bock“; **120 „das ist sogar PFLICHT, und nicht nur ein paar Motive auswählen“**; 122 hohe Auflösung. |
| 17H | 782–784 | 123–134 | Piercings/Körpermodifikation: freie Platzierung, Arten, Material, Heilung, Entfernen, Dehnung, Narben, Intimpiercings als normale Körpermodifikationskategorie, Scarification/Branding/Split Tongue/Subdermal selten, Spezialstudios, Charaktereditor bietet es direkt. Entwickler 783: ja. |
| 17I | 784–786 | 135–141 | Abschluss: **135 Realismus-Default** (Entwickler 785: „so wie das komplette Spiel, nicht du das speziell“); 136 keine künstlichen Grenzen; 137 NPC-Autonomie („die NPCs sollen ein Leben haben“); 138 dynamische Tattoo-Population (manche plötzlich Ganzkörperanzug); 139 Performance („das ganze Spiel muss super optimiert sein“); 140 gemeinsamer Workflow; 141 Runde abgeschlossen. |

753–756: Reststruktur (R18 Steuerung, R19 technische Integration, R20 Audit/Production Lock); Liste weiterer Planungsbereiche (nicht Adult).

## 4. Runde 18: Steuerung, Oberfläche, VR

Details: [Modul 04](04-Steuerung-Darstellung-Kamera-Audio.md); Register [E-ST](14-Entscheidungsregister.md#12-st-steuerung-oberfläche-vr-kamera-audio).

- **18A (790–799, 1–20):** Direkt vs. unterstützt; UFC-artige Aktionsbibliothek; kontextabhängige Liste; vollständige Bibliothek; Kombos nur Hilfen; Analogsteuerung; Intensitätsmodifikator; Körperbereichswahl; Smart Context; parallele Aktionen im UI; HUD-Stufen Off/Minimal/Standard/Detailed/Developer; Minimal-HUD filmisch; Remapping; Kombo-Konflikte; Haptik; Kamera (Third Person, näher, First Person, Cinematic); First-Person-Stabilisierung; Barrierefreiheit; Kontrollprofile.
  - Entwickler 791: 1 „weiß noch nicht, ich finde das UFC-System etwas erweitert extrem krass, was ist Direct“; 2 offen; **3 „ja, aber erst wenn man diese gelernt hat“** (Kampf-Lernsystem: Grundschläge automatisch, komplexe Techniken lernen und verbessern, „kein normales Upgradesystem“, Gym-Trainer); 5 ja, nur freigeschaltet; 7 (793) „Stick bewegen ist schneller, Spammen wäre zu GTA“; 8–20 ja.
  - Assistent 792/794/796: Fragen 1, 2, 4, 6 offen; Vorschlag **Hybrid-Steuerung** statt zwei Modi; Kampf/MMA als separater Core-Lock.
  - **Entwickler 797: „bei Kampf soll das so sein wie ich gesagt habe; wenn du auf Adult meinst, das passt.“** 798: Adult = Hybrid; Kampf = separates UFC-artiges Lernsystem; gleicher Input-/Animationsunterbau. **799: „ok dann weiter, das passt dann“.**
- **18B (794–796, 21–30) VR:** gleiche Engine; Handtracking; optional Ganzkörper; IK ergänzt; räumliches Menü; Haptik später; Komfort; sitzend/stehend; Kalibrierung; VR später. Entwickler 795: 21–24 „VR soll sich anfühlen wie echt“; 25–29 ja; **30 „ja definitiv“**; viel erweiterbar, bewegliche Hände/Füße/Hüfte. Tracking-Stufen (Assistent): Headset+Controller → Handtracking → Hüfte → Füße → Ganzkörper → Haptik.
- **18C (800–801, 31–42):** kontextsensitive Eingaben, Suche, Favoriten, Lern-UI, Vorschau ohne Exaktheitsversprechen, Begründung bei ungültiger Aktion, Kamera unabhängig, Cinematic nur Beobachtung/Replay, modulares HUD, Fingertracking, Body Ownership. Entwickler 801: 31–42 ja.
- 802: R18 abgeschlossen; nur bei Prototyping-Problemen neu öffnen.

## 5. Runde 19: Technische Integration

Details: [Modul 02](02-Architektur-Pakete-Technik.md); Register [E-SP](14-Entscheidungsregister.md#14-sp-speicherstand-modding-builds-technik).

- **19A (804–806, 1–20):** ein gemeinsamer Menschenkern; strikt getrennte Adult-C++-Module (Hauptspiel ohne Adult baubar); Adult optional ladbar; Speicherstand funktioniert ohne Adult-Mod; Deaktivieren/Reaktivieren erhält kompatible Daten; Schemamigration; ereignisgesteuert; Simulations-LOD; Budgets pro Subsystem; Graceful Degradation; Determinismus (kosmetische Details dürfen nichtdeterministisch sein); Threading mit Besitzerregeln; Daten-Registry; kleine stabile Mod-API; Capability-System; CI mit tausenden Kombinationen; Golden Tests; Entwickler-Telemetrie; kleiner vertikaler Kern. Entwickler 805: 1–3 ja; **4 ja, „die Daten von Adult werden extra gespeichert“**; 5–20 ja. Assistent 806: separater Adult-Zustand, verknüpft über dieselben Figur-/Welt-IDs.
- **19B (806–808, 21–32):** gemeinsame IDs; verwaiste Referenzen; getrennter Adult-Speicherbereich (separat sicherbar/löschbar); optionale Verschlüsselung; Hot Enable/Disable; Feature Flags (Kerninteraktion, erweiterte Physik, Replay, Flüssigkeiten, VR); späte Features hinter Flags; Absturzisolierung; Mod-Sandboxing; Versionsprüfung; Build-Split (Basis, intern, Adult-Dev, später Distribution). Entwickler 807: ja. 808: R19 abgeschlossen.

## 6. Runde 20: Production Lock

- **20A (810–812, 1–15):** Adult V1 klein (Basisinteraktionen, wenige Positionen/Übergänge, grundlegende Reaktionen, einfache Präferenzen, eine Testlocation); Prioritätsklassen Core/Important/Late Polish; Core Gate (IK, Physik, Körperzustand, Reaction Graph, Speichern/Laden, stabiler NPC-Intent); lieber weniger sauber; Performance-, QA-, Visual-Gate; Registry-Freeze nach Audits; getrennte Builds; **10 „öffentliche Adult-Veröffentlichung kein Pflichtziel, Mod kann privat bleiben“**; Shared-Core-Regel; Modding First-Class; Spezifikation pro Subsystem; Prototype First; finaler Production Lock.
  - Entwickler 811: 1–9 ja; **10 „ja, aber ich will schon, dass es veröffentlicht wird, nur für mich brauche ich nicht“**; 11–15 ja.
  - Assistent 812: Ziel = öffentliche Veröffentlichung, privat-only = Rückfall; „Runden 1–20 fertig“; offen bleiben Missing-Family-/Taxonomie-Audit, Qwen-Audit, Prototyping- und Rechtserkenntnisse. → [E-PR-012, E-PR-013](14-Entscheidungsregister.md#15-pr-produktion-qualität-reihenfolge)

## 7. Nach dem Interview (813–951)

814: „Adult-Runden 1–20 abgeschlossen; offen: Missing-Family/Taxonomy-Audit, Qwen-Audit später, echte Lücken.“ 815–872: Aufbau des kumulativen Master-ZIP vom 27.09. (872: es enthalte auch die Adult-Planung). 873–951: Karten-/Weltprototyp (nicht Adult). **Ergebnis:** Das Adult-Ergebnis der Runden 16–20 ist im Master laut Chat nur in ZIP-Zusammenfassungen vom 20./27.09. enthalten; konsolidierte Dateien dazu sind im Archiv **nicht auffindbar** ([Modul 13, Abschnitt 2](13-Quelleninventar-und-Abdeckung.md#2-befund-wo-die-adult-planung-tatsächlich-liegt)).

## 8. Chat 28: Rahmenentwurf „Adult Content Framework“

28.08.2026, 25 Nachrichten. **Charakter:** Prompt, den ein früherer Assistent für einen anderen KI-Chat schrieb; **alles Vorschlag**, teils überholt. Vollständige Auswertung: [Modul 35](35-Auswertung-Rahmenentwurf-28-08.md).

Nutzerwünsche (28.08.): mehr und andere Kategorien (1, 3); „wesentlich granularer“, nicht nur vier grobe Tags (7, 10, 12, 14, 16); „du kannst doch Hardcore dazu schreiben“ (18); „andeuten, was gemeint ist, ohne es zu schreiben“ (20, 22) → Assistent 23 lehnte **codierte Umschreibung konkreter Praktiken** ab und machte es abstrakter ([E-XX-022](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt)); 24: alles in den kompletten Prompt.

Sicherheitsregeln schon im Entwurf (Nr. 25, Abschnitt 0): Teilnehmer eindeutig erwachsen, bei Bewusstsein, handlungs- und entscheidungsfähig, können aktiv zustimmen und jederzeit zurücknehmen. Nicht unterstützt: Minderjährige, nicht-einvernehmliche Interaktionen, Bewusstlose, Schlafende als aktive Teilnehmer, Story-/Quest-Zwang, Beziehungswerte oder KI-Ziele als Zustimmungsersatz, Party-Leader-/Admin-Zustimmung. Mods dürfen diese Regeln nicht deaktivieren.

## 9. Chat 23: Darstellung, Plattformen, Dienstleistungs-NPCs

Nachrichten 505–592 (27.–29.08.), 628–638, 706–789, 1038–1060.

| Nachricht | Inhalt |
|---|---|
| 511 | Entwickler (27.08.): Stripclub „wie in GTA“, realistische Nacktszenen mit abschaltbarer Nacktheit, viele Arten. 514–523 Assistent: keine explizite Choreografie, aber nichtgrafische Taxonomie; erste Fassung `IntimacyContentType`. Entwickler 516–522: „du musst es nicht detailliert ausarbeiten, nur aufstellen, was möglich sein soll“. |
| 524–535 | Entwickler legt den kompletten Prompt aus Chat 28 vor; Assistent-Review: Zustimmung kommt in der Roadmap zu spät (Phase 10 → früh), Prompt ist ein technisches Designdokument, zu groß, Overengineering-Gefahr (**Vorschlag**). Schätzungen (800–1.200 Arbeitspakete Framework, 3.000–5.000 mit Inhalt; Gesamtplanung ca. 21–22 %) sind **Schätzungen**. |
| 536 | Entwickler (28.08., 03:17): Konsolen erlauben keine Darstellung des Einführens, Steam schon; „orientiere dich an Cyberpunk“, Szene andeuten. 538 Assistent: vier Darstellungsprofile (RomanceOnly, ConsoleExplicit, PCAdult, Concealed) (**Vorschlag**, Einstufungsaussagen ungeprüft). |
| 539 | **Entwickler: Am PC genauso wie auf Konsole; explizit nur über eine separate Mod, die ggf. nicht auf Steam hochgeladen wird.** → bestätigt ([E-PA-002](14-Entscheidungsregister.md#3-pa-pakete-builds-architektur)). 540: Mod als separates Paket, nie in Konsolen-Cooks; kein `if PornModInstalled`; Official Online erzwingt Basisdarstellung. |
| 541–548 | Eigener Entwickler-Mod nicht wie ein Fan-Mod behandeln (Rating/DLC); Steam Mature-Content-Fragebogen: unerreichbarer Adult-Content im Build angeben; Vertriebsoptionen (Workshop nur nach Valve-Rückfrage, externer Download mit Altersprüfung, nur SDK). **545 Entwickler: „Wenn ich die Mod über z. B. mod.io hochlade und tue, als wäre das ein Fanwork, wie wird das geprüft?“ → 546 Assistent lehnt die Täuschung ab**, empfiehlt Transparenz (Optional Developer Adult Mod, PC-only, getrennt, gekennzeichnet). → [E-RC-003, E-XX-020](14-Entscheidungsregister.md#16-rc-recht-plattform-ki-vertrieb) |
| 549–557 | Allgemeines Mod-/Content-Package-System, kein Adult-Sonderpfad. **551 Entwickler: „Ich will online keine Mods, da es viel einfacher ist zu cheaten. Mit Freunden in privater Lobby, aber nicht online mit Randoms.“** 552: Adult-PC-Mod läuft nicht im offiziellen Online. 553–557 Koop/privates Online. |
| 567–575 | Entwickler (29.08.): im Mod/DLC sollen sich Figuren real anfühlen, keine fixe Position, Unbehagen → sie bewegt sich weg, immersive Positionswechsel. **569: Unbehagen muss reale Konsequenz haben; Stop-Zeichen beendet; manche wollen es härter und mögen Schmerz;** Frauen im Stripclub/Bar, die „alles anbieten“; Geldsystem. 570 Assistent: `HardStopSignal` beendet sofort mit sicherem Ausstieg; schwächere Hinweise (Unbehagen, Präferenz, Check-in, Intensitätsanpassung, Repositionierung); Intensitäts- und Schmerzpräferenz (einvernehmlich); **Beziehungs-NPC nicht für alle Kategorien überredbar**; Adult-Service-NPCs mit Profilen, Preisen, Zeitplan, Rechtslage; Service-Transaktionsrahmen. 571 Entwickler: „Ja genau“. 573–575: Fitness/Muskeln komplex; große Charaktererstellung am Spielbeginn; ein spielbarer Charakter, männlich/weiblich wählbar, gleich gut spielbar; kein Pay-to-win. |
| 588–589 | Entwickler: Rückstoß bei Waffen, „das Gleiche beim Sex“. Assistent: **Character Physical Response System** allgemein (Masse, Steifigkeit, Dämpfung, Trägheit, Anbindung, Kollision, Maximalauslenkung), 3 Ebenen. Später durch Freeform erweitert. |
| 636, 638, 1060 | Gore/Alter: Darstellung für Erwachsene/Minderjährige getrennt; **Lock 1060 (03.09.): extrem brutal für Erwachsene und Monster, bei Kindern ausdrücklich kein Gore.** |
| 706–789, 1038 | Stripclubs/Tattoo-/Piercingstudios in Groß- und Nebenstädten; Adult-Business-Regeln als Gesetzesdomäne; Nachtleben als breite erwachsene Nachtökonomie. → [E-WE-004](14-Entscheidungsregister.md#11-we-welt-orte-branche-wirtschaft) |

## 10. Chat 37: Tagesplan

08.09.–05.10.2026, 99 Nachrichten; Adult-relevante gezielt gelesen (16–22, 50–55, 66, 68, 79, 82–85).

- 08.09., Entwickler 16/19: Early Access auf Steam, Adult „eher eine Mod oder anderes“; „die Adult-Mod soll richtig heftig werden“. Assistent 18/20: sauber trennen; Steam-Deskriptor für Adult-Only-Inhalte.
- **08.09., Entwickler 21: Links zu einschlägigen Videos als Referenz.** Assistent 22: analysiert und baut keine konkreten Akte daraus nach; Mod-System mit Profilen. Die Links werden **nicht übernommen** ([E-XX-023](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt)).
- 16.09., Entwickler 50/54: „Adult Runde 15 ist fertig schon 175 Fragen. Also nächstes die Mega-Runde 16.“ 17.–24.09.: Tagespläne verweisen auf Stände; Zusatzentscheidungen (Lifetime-Statistik als Core, Motorsport).
- **28.09., Entwickler 83: „Bro ich habe Adult fertig“.** Assistent 84–85: „Adult-Gesamtplanung: ABGESCHLOSSEN“ als Fortschrittsmarker. Der Audit bleibt laut Interview offen ([E-PR-015](14-Entscheidungsregister.md#15-pr-produktion-qualität-reihenfolge)).

## 11. Kanonische Dokumente

| Quelle | Inhalt (Adult-relevant) |
|---|---|
| `K§16` | generische Mod-Schnittstellen von Anfang an; Entwickler-Mod nutzt dieselben; kein `if OfficialAdultModInstalled`; Official Online lädt keine Mods; explizitere PC-Darstellung = separates Paket, nicht in Konsolen-Cooks, nicht im Basis-Build; Distribution/Recht = Release-Entscheidung |
| `K§17` | offizielle Basis: konsistente nichtexplizite Darstellung; Nacktheit konfigurierbar (Voll/Reduziert/Aus); Intimität angedeutet; keine explizite anatomische Penetration; Assets/Kameras/IK so bauen, dass FOV-Fehler nichts zeigen |
| `K§18` | Reaktivität/Hard Stop: keine starren Schleifen; Haltung, Winkel, Distanz, Tempo, Pause, Repositionierung, Übergänge je nach Komfort, Bereitschaft, Müdigkeit, Stimmung, Verletzung, Präferenz; Hard Stop sofort mit sicherem Ausstieg, keine Umdeutung; mildes Unbehagen ≠ Rückzug; einvernehmlich höhere Intensität/Schmerz möglich; Positionswechsel als Erstklass-Übergänge; Mikrovariation |
| `K§19` | Adult-Service-NPCs/Nachtleben: nicht jeder bietet jeden Service; wiederkehrende NPCs mit Namen, Zeitplan, Persönlichkeit, Grenzen, Präferenzen, Preisen, Erinnerungen; glamouröse Art Direction mit Vielfalt; Beziehungs-NPCs behalten harte Grenzen; Transaktionen über Wirtschaft |
| `K§20`, `K§21` | Körperzusammensetzung als Adaptationsmodell statt Slider; Physical-Response/Secondary-Physics geschichtet, subtil, kein Arcade-Jiggle (Interview 11./13.09. erweitert zu Freeform + Weichgewebelöser) |
| `PD` | wie K§16–21; Gore/Alter: Erwachsene Gore mindestens RDR2-Niveau, systemisch; Kinder existieren, keine Unverwundbarkeit, offizielle Darstellung ohne extremes Zerstückeln, keine Belohnung für Gewalt gegen sie, Grenzen nach Plattform-/Rating-/Rechtsprüfung |
| `PS` Abschnitte 15–22 | Sicherheitskern nicht moddbar (AdultEligibility, Zustimmung, Blockregeln, Serverautorität, Kontoberechtigungen, Anti-Spam, Online-Sicherheit); ContentOrigin (BaseGame, OfficialDLC, DeveloperMod, CommunityMod), DeveloperMod ohne Sonderrechte; Zustimmung kein Bool; Beziehung ersetzt Zustimmung nie; Hard Stop übersteuert alles; weiche Hinweise; Teilnehmer als Array; Zielgröße physische Disc ca. 100 GB |
| `OD` | offen: finale Kategorien-Taxonomie und Asset-Zahl; Kamera-/Darstellungsprofile je Plattform/Region; Umfang/Orte/Preise der Service-NPCs; Nacktheit-Benennung und Streamer-Filter; Umfang/Vertrieb des optionalen PC-Pakets; Distribution/Recht des Adult-Pakets |
| `CA05` Block 10 | breites Adult-NPC-Beziehungs-/Romantiksystem, parallele Beziehungen, Eifersucht, Geheimnisse, offene Arrangements, Trennung; Zusammenleben/Ehe/Eigentum im Scope; Familiensystem niedrige Priorität/offen |
| `WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT` | Nachtleben essentiell; Adult-Venues je nach fiktiver Jurisdiktion |
| `OPTIONAL_CONTENT_PACKS_2026-09-10` | Voice-Standard + optionaler Lossless-Pack; Sprachpakete getrennt (allgemein) |
| `T44` | Gore/Alter (Planungsinventar) |

Keine **konsolidierten** Kanon-Dokumente für die Runden 1–20 gefunden. Der Master-Stand vom 17.–27.09. enthält laut Suche keinen Adult-Inhalt. Das Interview bleibt Primärquelle der Detailplanung, aber **kein Kanon**. Die Tagesdeltas 14.–19.10. enthalten keinen Adult-Treffer.

## 12. Ausgeschlossene Quellen

- Chat 40 („Nächste Schritte festlegen“): „Private/Adult Mode“ eines **anderen Projekts**, nicht SPN.
- Chat 09 (F95Zone-Recherche, 07/2026): Marktbeobachtung; als Kontext für Content-Toggles, nicht als Quelle.
- Chats 01, 05, 06, 08, 12, 15, 21, 22, 26: „adult“ in fremdem Kontext.
