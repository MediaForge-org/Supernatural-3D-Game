# Fahrzeuge und Verkehr 29: Konflikte und offene Fragen

> [!IMPORTANT]
> **Dieses Modul entscheidet nichts.** Es führt (1) Widersprüche zwischen Quellen mit beiden Aussagen und ihren Pfaden, (2) unbelegte oder uneindeutige Angaben und (3) alle offenen Fragen mit Begründung, betroffenen Anforderungen, Optionen mit Vor- und Nachteilen und dem Vermerk, wer entscheidet. **Jede Frage trägt die Option ‚F Freitext‘; deren Inhalt hat Vorrang vor den vorgegebenen Auswahlmöglichkeiten.** Fragen, die die Quellen bereits beantworten, stehen in Abschnitt 4 und sind nicht erneut offen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 28](28-Entscheidungsregister.md).

**Inhalt**

1. [Konflikte (CK)](#1-konflikte-ck)
2. [Unbelegte oder uneindeutige Angaben (CU)](#2-unbelegte-oder-uneindeutige-angaben-cu)
3. [Offene Fragen (CQ)](#3-offene-fragen-cq)
4. [Als beantwortet geführt](#4-als-beantwortet-geführt)
5. [Zuordnung zu den Pflichtthemen des Auftrags](#5-zuordnung-zu-den-pflichtthemen-des-auftrags)
6. [Vorschlag für die Reihenfolge der Klärung](#6-vorschlag-für-die-reihenfolge-der-klärung)

---

## 1. Konflikte (CK)

| ID | Gegenstand | Aussage A (Quelle) | Aussage B (Quelle) | Einordnung |
|---|---|---|---|---|
| `CK-01` | **Tastenbelegung: Fahren gegenüber Kampf** | Kreuz = Hochschalten, Quadrat = Herunterschalten (`W-FP-005`, `D15-RV`, 15.10.2026) | Kreuz = Springen (Proposal 0009 §3a, 22.10.2026); Quadrat = Kontextinteraktion beziehungsweise Angriff (Auftrag Kampfsystem 23.10.2026, dort 🔍 UNBELEGT; [Kampfsystem 06](../Kampfsystem/06-Steuerung-und-Eingabekontext.md)) | ⚠️ offen; lösbar über getrennte Eingabekontexte (Fußgänger, Fahrzeug); der Konflikt bleibt beim Schießen aus dem Fahrzeug (`VEH-DEC-ST-011`) |
| `CK-02` | **Wichtige NPCs, Verkehrsunfälle und unabhängige Lebensentwicklung** | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI (`N-LT-033`); NPC-Unfälle sind selten (`W-TV-001`) | NPCs entwickeln sich ohne den Spieler weiter (`N-LT-006`); Verletzungen und Tode sind persistent (`N-LT-005`, `M-WB89` Block 8); Spannung im NPC-Konflikt C-N1 | ⚠️ offen; Vorschlag trennt Tod von Verletzung (`VEH-DEC-KI-011`), `CQ-52` |
| `CK-03` | **Fahndungsstufen oder kein Sternesystem** | Kein Sternesystem, schwere Fahndung bleibt, mehrstufige Eskalation (`COMBAT-DEC-PO-008`, `E-RE-013`, `M-PS31` §54) | Quiz-Text nennt Fahndungsstufen (`E-PO-003`, `48#156A`) | ⚠️ offen; Verweis auf [Ermittlung](../Ermittlung/21-Konflikte-und-offene-Fragen.md); Verkehrsüberwachung betroffen (`VEH-DEC-PV-005`) |
| `CK-04` | **Größenziel des Spielstands** | Etwa 20–50 MB, Zuverlässigkeit vor Größe (`N-PE-004`, `M-PD`) | Assistent: 50–150 MB, in Extremfällen 150–300 MB (`N-PE-012`, `W-TV-011`); Entwickler ‚eher kleiner‘ | ⚠️ offen; hier nur als Verweis; Fahrzeuge und Verkehr tragen dazu bei (`VEH-DEC-PE-015`) |
| `CK-05` | **Ländliche Straßen und Bergstraßen** | Ländliche Straßen um etwa 30–40 % reduzieren, sparsames Makronetz (`W-VK-007`: Arbeitsaufträge `30#232E`, `30#241E`) | Die Berge sollen Straßen mit kleinen Dörfern besitzen, sonst seien sie unnötig (`23#705E`) | ⚠️ offen; vereinbar, wenn Bergstraßen als Ausnahme gelten (`VEH-DEC-SN-002`, `VEH-DEC-SN-003`) |
| `CK-06` | **Schadensmodell: ob und wie tief** | Mechanische Schäden zusätzlich zur Deformation: ja (`23#1025E` Nr. 15) | Realistisches Schadensmodell nicht gewählt (`D15-RV`, `W-FZ-013`) | ⚠️ offen; lesbar als ‚Ob beschlossen, Wie tief offen‘ (`VEH-DEC-ZV-004`), `CQ-23` |
| `CK-07` | **Status der Fahrzeugdetails des Oktober-Pakets** | `D15-RV`: Phasenvertrag ‚user-approved‘, DualSense-Belegung ‚user specified‘ | `M-EP1018`: ‚Preserve these as **review-required specification inputs**, not silently confirmed numerical constraints‘; `M-OC1018`: Fahrzeugdetails sind Vorschläge neben etablierten Vorlieben, einzeln zu prüfen | ⚠️ offen; die Dokumentation übernimmt den Status der Welt-Dokumentation (📋) und markiert die Betroffenen (`VEH-DEC-GP-019`) |

---

## 2. Unbelegte oder uneindeutige Angaben (CU)

| ID | Gegenstand | Befund | Umgang |
|---|---|---|---|
| `CU-01` | **‚Manuelle Variante für jedes Fahrzeug‘** (`W-FZ-006`) | Gilt wörtlich auch für Elektrofahrzeuge und Fahrräder, die kein Schaltgetriebe haben | als offene Frage geführt (`CQ-08`); keine Ableitung |
| `CU-02` | **Valet-Parken** | `M-WB17` Block 5 führt Valet-Parken; die Frage 47 beantwortete der Entwickler mit ‚was ist das‘ (`23#1025E` Nr. 47); `W-VK-013` übernimmt es als beschlossen | als 🔍 UNBELEGT geführt (`VEH-DEC-SP-004`); `CQ-46` |
| `CU-03` | **DualSense-Vorgabe deckt acht Funktionen** | Handbremse, Rückwärtsgang, Zündung, Licht, Hupe, Blinker, Aussteigen, Kofferraum und Radio sind nicht belegt | als 🔍 UNBELEGT geführt (`VEH-DEC-ST-005`); `CQ-15`; keine Ersatzbelegung |
| `CU-04` | **Tuning-Tiefe: ‚mindestens so wenig‘** (`23#1025E` Nr. 12) | Die Antwort ist nicht eindeutig lesbar (möglich: ‚mindestens so viel‘, ‚so wenig wie möglich‘) | nicht in eine Aussage übersetzt (`VEH-DEC-WE-009`) |
| `CU-05` | **Abschleppdienst ‚eher nicht so oft‘** (`23#1025E` Nr. 41) | Unklar, ob die Häufigkeit des Spielerjobs oder des NPC-Systems gemeint ist | als Aussage ohne Häufigkeit geführt (`VEH-DEC-WE-005`) |
| `CU-06` | **Fahrtrichtung (Rechts- oder Linksverkehr)** | In keiner Quelle belegt; nur ‚nordamerikanischer Straßenmaßstab‘ (`W-UR-015`) | als 🔍 UNBELEGT geführt (`VEH-DEC-SN-014`); `CQ-37` |
| `CU-07` | **Fahren unter Einfluss (Rechtsfolgen und NPCs)** | Für die Spielfigur belegt: Alkohol beeinflusst Reaktion, Gleichgewicht, Wahrnehmung und Fahren (`M-CA0905`); Rechtsfolgen und NPC-Verhalten nicht | als 🔍 UNBELEGT geführt (`VEH-DEC-KI-016`); `CQ-53` |
| `CU-08` | **Versicherung ‚eher in einer fertigeren version‘** (`23#1025E` Nr. 16) | Unklar, ob ‚später im Spiel‘ oder ‚in der ausgereifteren Fassung‘ gemeint ist | als ‚vorgesehen, später‘ geführt (`VEH-DEC-BZ-001`); `CQ-74` |
| `CU-09` | **Rush-Hour ‚kann man machen‘** (`23#1025E` Nr. 2) | Weicher als ‚ja‘; `W-TV-001` führt Stoßzeiten als beschlossen | übernommen mit Vermerk (`VEH-DEC-VD-002`) |

---

## 3. Offene Fragen (CQ)

Spalten: **Warum nötig** (welche Entscheidung blockiert sie), **Betroffen** (Register-Gruppen und Einträge; ‚ZV-009‘ steht für `VEH-DEC-ZV-009`), **Optionen mit Vor- und Nachteilen** (Kurzfassung; **jede Frage enthält die Option F Freitext, deren Inhalt Vorrang hat**), **Entscheidung** (wer entscheidet; PL = Projektleitung, ‚nach Prototyp‘ oder ‚nach Messung‘ = erst nach einer Messung oder einem Spike, ‚WorldForge‘ = Abstimmung mit dem Nachbarprojekt, ‚Recht‘ = Fachberatung).

### 3.1 Grundprinzipien, Klassen, Fahrphysik (Module 02 bis 05)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-01` | In welchem Format werden Verhaltens- und Regelprofile geführt (Fahrstile, Straßenprofile, Dichtekurven, Rechtsprofile)? | `VEH-DEC-GP-014` setzt Datenprofile voraus; Mods und Migration brauchen ein stabiles Format | GP-014, SN-009, KI-006, VD-007, PV-014 | A eine Datei je Domäne (+ klar, − viele Formate); B ein gemeinsames Schema (+ gemeinsame Werkzeuge, − Zwang); C erst nach Prototyp (+ weniger Fehlfestlegung, − Verzögerung); F Freitext (Inhalt hat Vorrang) | PL nach Prototyp |
| `CQ-02` | Wie viele Wiedergabetreue-Stufen und Assistenzoptionen bietet der Straßenverkehr? | `W-FP-007` nennt nur Hooks; Presets, Einstellungen und Tests hängen davon ab | GP-016, PH-002, ST-008 | A wenige Presets (+ übersichtlich, − grob); B feine Regler (+ Kontrolle, − viele Optionen); C Presets plus Anpassen (+ beides, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-03` | Gibt es Schnellreise (Camps, Safehouses) und wie wird lange Fahrzeit behandelt? | Beeinflusst Fahrzeit, ÖPNV-Erleben, Materialisierung und Streaming; Welt OF-50 ist offen | GP-017, IT-013, LS | A keine (+ Fahren bleibt Kern, − lange Wege); B zu besuchten Orten (+ Komfort, − entwertet Weg); C ÖPNV als diegetischer Ersatz (+ passt zur Welt, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-04` | In welcher Reihenfolge wachsen Fahrzeug- und Verkehrssystem? | Phasenplanung in Modul 31 braucht eine Linie | Modul 31 | A erst Fahrzeug auf leerer Straße, dann Verkehr (+ Fokus, − tote Straße); B früh minimaler Verkehr (+ lebendiger, − Mehraufwand); C parallel (+ konsistent, − Risiko); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-05` | Wie viel Verkehr braucht der Vertical Slice? | Der Slice beginnt mit Ankunft und Fahren (`CLAUDE.md` §4) | Modul 31 | A keiner, nur Teststrecke (+ billig, − leer); B minimaler Hintergrundverkehr (+ Atmosphäre, − Aufwand); C inklusive Stoßzeit und Zwischenfall (+ Vollbild, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-06` | Welches Startsortiment, welche Marken, Modelljahre und Trims gibt es? | `W-FZ-014` offen; fiktive Namen sind beschlossen (`W-FZ-017`) | KL-007, KL-003 | A kleine Startliste (+ machbar, − wenig Auswahl); B zuerst Taxonomie und Markenstrategie (+ saubere Basis, − langsamer, vgl. `W-FZ-018`); C Marken aus Mods ergänzbar (+ erweiterbar, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-07` | Welche Antriebsarten gibt es und mit welchem Anteil? | Nur Verbrenner und E-Laden sind belegt; Hybrid und Wasserstoff nicht | KL-013, KE-005 | A Verbrenner und Elektro (+ belegt, − eng); B zusätzlich Hybrid (+ Realismus, − Aufwand); C Anteil nach Epoche (+ glaubhaft, − Daten); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-08` | Wie ist ‚manuelle Variante für jedes Fahrzeug‘ bei Fahrzeugen ohne Schaltgetriebe zu lesen (Elektro, Rad)? | `CU-01`; sonst unklar, ob Elektro simulierte Schaltung bekommt | KL-006, `CU-01` | A nur Fahrzeuge mit Schaltgetriebe (+ logisch, − Ausnahme); B Elektro mit simulierter Schaltung (+ gleiches Gefühl, − künstlich); C Ausnahme je Antrieb (+ flexibel, − Regeln); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-09` | Sind Verkehrsfahrzeuge dieselben Typen wie fahrbare Fahrzeuge, und wie wird die Verkehrsrolle zugeschnitten? | Datenumfang und Pflege | KL-008, KL-010 | A gleiche Typen (+ konsistent, − viel Daten für Statisten); B eigene Verkehrstypen (+ leicht, − Doppelung); C Mischung (+ pragmatisch, − Regeln); F Freitext (Inhalt hat Vorrang) | PL nach Prototyp |
| `CQ-10` | Welche Fahrzeugphysik-Grundlage wird verwendet? | Hängt Antriebsstrang, Netzwerk und Leistung an; `PH-014` | PH-012, PH-014, UE-009 | A eingebaute Unreal-Fahrzeugphysik anpassen (+ vorhanden, − Grenzen); B eigene Lösung (+ Kontrolle, − Aufwand); C Hybrid mit eigenem Antriebsstrang (+ testbar, − Schnittstellen); F Freitext (Inhalt hat Vorrang) | nach Spike |
| `CQ-11` | Wie tief werden Zweiräder, Lkw, Gespanne und Geländefahrzeuge simuliert? | Motorräder ‚tiefer wie Autos‘ (`23#1025E` Nr. 27), Anhänger physisch nutzbar | VT-003, IT-007 | A eigene Modelle je Klasse (+ glaubhaft, − Aufwand); B gemeinsames Modell mit Parametern (+ weniger Aufwand, − Gefühl); C Stufenplan (+ planbar, − Übergangsmängel); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-12` | Wie tief ist das Reifen- und Wettermodell im Straßenverkehr? | `W-FP-003` verlangt Reifenmodell, Wetter wirkt auf Physik | PH-003, PH-007, PH-015 | A einfache Haftungsstufen (+ billig, − wenig Gefühl); B Reifenmodell mit Temperatur und Nässe (+ glaubhaft, − Aufwand); C Straße einfach, Rennsport tief (+ passt zu `W-FP-008`, − zwei Stufen); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-13` | Welche Konstanten und welcher Kupplungs-Greifpunkt-Algorithmus gelten? | `W-FP-010`; erst messbar nach Prototyp | PH-013 | A feste Parameter je Fahrzeug (+ einfach, − starr); B Kennlinien aus Daten (+ flexibel, − Pflege); C lernt nicht (kein magisches Wachstum, `W-FZ-010`); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-14` | Wie physikalisch fahren NPC-Fahrzeuge nahe beim Spieler, und wie wird beim Umschalten die Zustandsübernahme gelöst? | `PH-016`, `PH-017` | PH-016, PH-017 | A volle Physik für alle Nahen (+ einheitlich, − teuer); B vereinfachtes Modell mit Umschalten bei Berührung (+ billig, − Übergang); C vereinfacht ohne Umschalten (+ einfach, − Rammen unglaubwürdig); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-15` | Wie werden Handbremse, Rückwärtsgang, Zündung, Licht, Hupe, Blinker, Aussteigen, Kofferraum und Radio beim Fahren belegt? | `CU-03`; die DualSense-Vorgabe deckt acht Funktionen; die Fahrzeug-App nennt Schloss, Licht, Motor und Kofferraum als Funktionen (`VEH-DEC-EI-013`) | ST-005, EI-013 | A Tastenkombinationen (+ schnell, − viel lernen); B Radialmenü oder Kontextmenü (+ übersichtlich, − Unterbrechung); C Mischung (+ pragmatisch, − Regeln). Keine Ersatzbelegung der acht vorgegebenen Funktionen ohne Zustimmung; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-16` | Welche Kameraansichten und welches Kameraverhalten gibt es? | Nur rechter Stick = Kamera ist belegt | ST-014 | A Verfolger plus Cockpit (+ Standard, − Aufwand Innenraum); B mehrere Ansichten (+ Auswahl, − Aufwand); C eine adaptive Ansicht (+ billig, − weniger Kontrolle); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-17` | Wie sehen Anzeigen und Navigation aus (diegetisch oder HUD)? | `W-FP-007`: HUD-Realismus als Einstellungs-Hook | ST-016, KI-015 | A diegetisch im Fahrzeug (+ immersiv, − Aufwand); B HUD (+ klar, − weniger Realismus); C beides einstellbar (+ Wahl, − doppelt); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-18` | Wie sehen Tastatur, Maus, Lenkrad und Barrierefreiheit beim Fahren aus? | `M-OD` offen | ST-007, ST-002 | A Tastatur plus Controller zuerst (+ machbar, − Lenkrad später); B Lenkrad früh (+ Enthusiasten, − Aufwand); C Barrierefreiheit von Anfang an (+ breit, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |

### 3.2 Zugang, Zustand, Werkstatt, Energie (Module 06 bis 09)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-19` | Wie sieht die Interaktionsarchitektur für Fahrzeuge aus (Türen, Kofferraum, Schlüssel, Motorstart)? | Allgemeine Interaktionsarchitektur ist nur für Türen entworfen ([Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md)) | EI-010 | A Erweiterung des bestehenden Interaktionssystems (+ Wiederverwendung, − Anpassung); B eigenes Fahrzeugsystem (+ maßgeschneidert, − Doppelung); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-20` | Kann der Spieler Fahrzeuginhalt durch Diebstahl, Abschleppen oder Beschlagnahme verlieren? | `EI-011` | EI-011 | A ja mit Folgen (+ Spannung, − Frust); B Inhalt geschützt (+ komfortabel, − weniger Risiko); C je Schwierigkeitsgrad (+ Wahl, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-21` | Welchen Beweiswert, Geruch und welche Reaktion bei Kontrolle haben Leichen, Tierkörper und Verletzte im Fahrzeug? | Beschlossen ist, dass das Fahrzeug sie tragen kann (Platzierung je Fahrzeug) und die Polizei sie finden kann (`M-DC31` §48); Wirkung offen | EI-003, EI-015 | A Beweiswert, Geruch, Reaktion bei Kontrolle (+ glaubwürdig, − Aufwand); B nur Inventarobjekt (+ einfach, − flach); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-22` | Wie viele Sitze haben Fahrzeuge, wer fährt, und wie wechselt der Fahrer? | Koop bis vier Spieler, Begleiter 2 bis 3 | EI-012, KO-014 | A feste Sitzlisten je Typ (+ klar, − Daten); B generische Sitzzahl (+ einfach, − unglaubwürdig); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-23` | Welchen Detailgrad hat das Schadensmodell? | `W-FZ-013` unbeantwortet; `CK-06` | ZV-003, PH-018, WE | A Stufen (+ billig, − wenig Wucht); B Zonen mit Teilen (+ deckt `23#1025E` Nr. 15, − Aufwand); C Verformung (+ größte Wirkung, − Aufwand, Netzwerk); Entscheidung nach Prototyp zu Kollision und Rechenzeit; F Freitext (Inhalt hat Vorrang) | PL nach Prototyp |
| `CQ-24` | Wie viel Zufall steckt in verborgenen Mängeln gebrauchter Fahrzeuge? | `W-FZ-003` verlangt versteckte Mängel | ZV-009 | A deterministisch aus Datensatz (+ erklärbar, − vorhersagbar); B mit Zufall aus Seed (+ Überraschung, − Fairness); C Mischung (+ ausgewogen, − Regeln); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-25` | Welche Tiefe und welche Raten haben die einzelnen Verschleißteile? | Zehn Teile sind beschlossen, Tiefe nicht | ZV-001, ZV-015 | A alle Teile gleich einfach (+ übersichtlich, − flach); B Kernteile tief, Rest einfach (+ Fokus, − Auswahl); C alle tief (+ Realismus, − Aufwand); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-26` | Wie sehen Reparaturökonomie und bleibender mechanischer Verschleiß aus? | `D15-RV` nicht gewählt | ZV-013, WE-013 | A Reparatur setzt zurück (+ einfach, − kein Verschleißgewicht); B teilweiser bleibender Verschleiß (+ Gewicht, − Komplexität); C vollständig (+ Realismus, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-27` | Wie werden Totalschaden, Wrack, Brand und Verschrottung behandelt? | Quellen schweigen | ZV-014, PE-008 | A Wrack bleibt und wird abgeschleppt (+ glaubhaft, − Aufwand); B Fahrzeug verschwindet (+ einfach, − unglaubwürdig); C Brand und Explosion als Ereignis (+ Wucht, − Aufwand, Rating); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-28` | Wie repariert der Spieler (Menü, Dienst mit Wartezeit, praktische Arbeit)? | Phasenvertrag trennt nur früh und lang | WE-014, WE-001 | A Menü (+ früh machbar, − flach); B Dienst mit Wartezeit (+ Weltzeit nutzt, − Wartefrust); C praktische Arbeit (+ Tiefe, − Aufwand); Stufenfolge möglich; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-29` | Wie sind Preise, Teile und Teilemarkt gestaltet? | `W-FZ-014` Wartungsökonomie offen | WE-013 | A einfache Preise (+ schnell, − flach); B Teile und Markt (+ Tiefe, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-30` | Welche Rolle hat die Crew bei der Fahrzeugpflege? | `N-CO-019` ist nur eine Richtung; Mikroverwaltung soll nicht erdrücken (`N-LT-034`) | WE-008 | A passive Hintergrundpflege (+ leicht, − wenig Spiel); B aktive Aufträge an die Crew (+ Tiefe, − Verwaltung); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-31` | Wie lange dauern Tanken und Laden? | Reichweiten sind beschlossen (realistisch, ‚kein Tankstellen-Spam‘, `VEH-DEC-KE-014`); Zeiten nicht (`W-FZ-014`) | KE-011, KE-014 | A kurz, abstrahiert (+ Fluss, − wenig Realismus); B realistisch lang beim Laden (+ Charakter, − Frust); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-32` | Wie bilden sich Kraftstoffpreise (Niveau, Schwankung, Verbindung zur Wirtschaft)? | Preise je Tankstelle sind beschlossen | KE-002, KE-012 | A feste Preise je Tankstelle (+ einfach, − tot); B dynamisch mit Wirtschaft (+ lebendig, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-33` | Wie weit wird der Energiezustand von NPC-Fahrzeugen simuliert? | Vorschlag `KE-009` | KE-009 | A grob abstrakt (+ billig, − unsichtbar); B sichtbar tankende NPCs im Nahbereich (+ Atmosphäre, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-34` | Wo gibt es Rennkraftstoffe und Spezialkraftstoffe? | `W-FZ-007` nennt sie ohne Bezugsort | KE-004 | A Rennstrecke und Fachhandel (+ logisch, − Orte); B jede Tankstelle (+ einfach, − unlogisch); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-35` | Wie viele Tankstellen, Ladeparks und Rastplätze gibt es und wo? | Hängt am Straßennetz | KE-010, KE-013, WF-009 | A aus dem Generator (+ konsistent, − Auftrag an WorldForge); B vom Spiel gesetzt (+ Kontrolle, − Pflege); C Überlagerung (+ Trennung, − Verweise); F Freitext (Inhalt hat Vorrang) | PL + WorldForge |

### 3.3 Straßennetz und Stadt (Module 10 bis 12)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-36` | Welche Straßenklassen unterscheidet das Spiel? | WorldForge nennt sieben Arbeitsklassen | SN-008 | A WorldForge-Liste übernehmen (+ passt, − evtl. zu fein); B reduzierte Liste (+ einfach, − Lücken); C Liste plus Wünsche (+ passgenau, − Absprache); F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-37` | Auf welcher Seite wird gefahren? | In keiner Quelle belegt (`CU-06`); Verkehrs-KI, Schilder, Karte und Fahrphysik hängen davon ab | SN-014, KR, KI | A Rechtsverkehr (+ passt zu US-artiger Welt, Ableitung ohne Beleg); B Linksverkehr (+ eigenständig, − untypisch); C je Region (+ flexibel, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-38` | Welche Höchstgeschwindigkeiten, Spurzahlen und Verkehrsschilder gelten? | Geschwindigkeitskontrollen setzen Limits voraus | SN-015, PV-001 | A US-artige Werte (+ stimmig, − Quellen fehlen); B eigene Werte (+ frei, − Aufwand); C aus Profilen je Region (+ flexibel, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-39` | Wie sieht das Gesamtnetz der Hauptwelt aus (Autobahnen, Pässe, Fähren, Dichte)? | Karte offen (`CLAUDE.md` §3) | SN-016 | Entscheidung hängt an der Karte; keine Optionen ohne Kartenentscheidung; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-40` | Gibt es Mautstraßen, Kreisverkehre und Einbahnstraßen? | Quellen schweigen | SN-017, KR-011 | A ja alle (+ Realismus, − Regeln); B nur Einbahn (+ einfach, − weniger Vielfalt); C später (+ Fokus, − Nachrüsten); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-41` | Welche Kreuzungsarten kommen vor und wie verbreitet? | Keine Quelle | KR-011 | A Signal und Vorfahrt (+ Kern, − weniger Vielfalt); B plus Kreisverkehr und Anschlussstellen (+ Vielfalt, − KI-Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-42` | Fallen Signale aus, gibt es Sonderschaltungen, und welche Vorrechte haben Einsatzfahrzeuge? | Keine Quelle | KR-012 | A keine Ausfälle (+ einfach, − weniger Welt); B Ausfall bei Ereignissen (+ lebendig, − Regeln); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-43` | Wie sind Schilder gestaltet und wie werden Bahnübergänge geregelt? | Fiktive Namen sind nur für Fahrzeuge beschlossen | KR-013 | A eigene Gestaltung (+ rechtlich sicher, − Aufwand); B reale Vorbilder (+ vertraut, − rechtlich prüfen); F Freitext (Inhalt hat Vorrang) | PL + Recht |
| `CQ-44` | Gibt es Strafzettel und Abschleppen für Falschparken? | Keine Quelle | SP-011 | A nein (+ Komfort, − weniger Welt); B ja für Spielerfahrzeuge (+ Konsequenz, − Frust); C je Einstellung (+ Wahl, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-45` | Wird Parken über einzelne Lücken oder über Kapazität modelliert? | Vorschlag `SP-007` | SP-007 | A einzelne Lücken überall (+ sichtbar, − teuer); B Kapazität, Lücken beim Annähern (+ billig, − Rekonstruktion); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-46` | Gibt es Valet-Parken, und wie viele Parkhäuser, Garagen, Depots gibt es? | `CU-02`; Platzierung offen | SP-004, SP-012 | A kein Valet (+ einfach, − weniger Welt); B Valet an Hotels und Casinos (+ Welt, − Aufwand); Orte aus Generator oder Spiel (siehe `CQ-102`); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-47` | Welche Verkehrscharakter-Profile haben die Bezirke? | Vorschlag `SP-010` | SP-010 | A pro Bezirksart wenige Profile (+ einfach, − grob); B pro Bezirk (+ Kontrolle, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-48` | Gibt es Fußgängerzonen, Verkehrsberuhigung und Umweltzonen? | Quellen schweigen | SP-013 | A ja in Innenstädten (+ Stadtgefühl, − Netzregeln); B nein (+ einfach, − weniger Welt); F Freitext (Inhalt hat Vorrang) | PL |

### 3.4 Verkehrs-KI und dynamischer Verkehr (Module 13 bis 17)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-49` | Welches Verkehrsmodell gilt (Agenten, Fluss, Mischform)? | `W-FP-010`, `W-TV-008`; bestimmt Staus, Rekonstruktion und Kosten | KI-012, VD-009 | A Agenten (+ Detail, − teuer); B Fluss (+ billig, − weniger Einzelfahrzeug); C Agenten nah und Fluss fern (+ passt zu Stufen, − Übergang); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-50` | Wird hierarchische Routensuche benötigt? | Netzgröße unbekannt | KI-009 | A flache Suche (+ einfach, − bei großem Netz teuer); B hierarchisch (+ skaliert, − Aufwand); F Freitext (Inhalt hat Vorrang) | nach Messung |
| `CQ-51` | Wie reagieren NPC-Fahrer auf Hupen, Drängeln, Blockieren, Verfolgung und Schüsse? | Keine Quelle | KI-013 | A keine besondere Reaktion (+ einfach, − flach); B Reaktionen nach Fahrstil (+ glaubhaft, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-52` | Sind wichtige NPCs bei Verkehrsunfällen nur vor dem Tod oder auch vor Verletzung geschützt? | `CK-02`; `N-LT-033` nennt Tod | KI-011, NF-005 | A nur Tod (+ lebendige Welt, − Verletzungen möglich); B auch Verletzung (+ sicher, − weniger Konsequenz); C Schutz nur in Hauptfällen (+ Fokus, − Lücken); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-53` | Welche Rechtsfolgen hat Fahren unter Einfluss, und fahren NPCs unter Einfluss (Alkohol, Müdigkeit, Ablenkung)? | Für die Spielfigur wirkt Alkohol auf Fahren (`M-CA0905`, `VEH-DEC-KI-014`); Rechtsfolgen und NPC-Verhalten nicht belegt (`CU-07`) | KI-014, KI-016 | A nur Wirkung auf den Spieler (+ einfach, − weniger Welt); B zusätzlich NPC-Verhalten (+ Welt, − Rating); C mit Rechtsfolgen (+ Konsequenz, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL + Recht |
| `CQ-54` | Wie navigiert der Spieler (Karte, Zielführung, Schilder)? | Smartphone-Navigation ist ‚may include‘ | KI-004, KI-015 | A Karte plus Zielmarkierung (+ einfach, − keine Führung); B vollständige Zielführung (+ komfortabel, − Aufwand); C Schilder und Orientierung (+ immersiv, − Frust); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-55` | Liegen NPC-Reiseziele auch außerhalb der Karte? | `N-LE-006` nennt Urlaub und Besuche | NF-003, NF-013 | A nur auf der Karte (+ geschlossen, − enge Welt); B auch außerhalb als ABWESEND (+ billig, − Außenwelt nötig); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-56` | Wie viele NPCs besitzen ein eigenes Fahrzeug, und gibt es Mehrfahrzeughaushalte? | Keine Quelle | NF-015 | A realistische Verteilung (+ stimmig, − Daten); B pauschal (+ einfach, − flach); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-57` | Wie werden Abweichungen von Fahrzeugen der Stufe C gespeichert? | Speichergröße ist umstritten (`CK-04`) | NF-007 | A nur Abweichungen (+ klein, − Format); B volles Fahrzeug bei Berührung (+ einfach, − größer); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-58` | Wie verhält sich persönlicher Verkehr zur Verkehrsmenge? | Beeinflusst Identität, Zeugen, Kosten | NF-012 | A viel persönlich, wenig Menge (+ Identität, − Kosten); B viel Menge (+ billig, − Zeugen nachbefördern); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-59` | Wie wechseln, kaufen, verkaufen und altern NPC-Fahrzeuge über Jahre? | `N-LT-006` ohne Fahrzeugaussage | NF-016 | A statisch (+ einfach, − tot); B Ereignisse im Lebenslauf (+ lebendig, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-60` | Welche Dichtekurven, welcher Ferienkalender und welche Zeitskala gelten? | Keine Quelle | VD-001, VD-014 | A reale Rhythmen (+ stimmig, − Kalender nötig); B vereinfachte Kurven (+ einfach, − weniger Welt); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-61` | Wie wird ‚nicht zu extrem‘ umgesetzt (Dämpfung, Obergrenze, Abhängigkeit vom Schwierigkeitsgrad)? | `23#1025E` Nr. 1 | VD-008, PF-007 | A feste Obergrenze (+ einfach, − starr); B einstellbar (+ Wahl, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-62` | Wie werden Baustellen und Sperrungen erzeugt und in welcher Menge? | `W-UR-018`, `W-UR-012` | VD-016 | A aus Stadtentwicklung und Zwischenfällen (+ begründet, − Aufwand); B aus Zufall (+ billig, − unbegründet); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-63` | Wie wird Stau in der Ferne und beim Annähern dargestellt? | Vorschlag `VD-009` | VD-009, LS | A Verzögerung je Kante, Schlange beim Annähern (+ billig, − Rekonstruktion); B Einzelfahrzeuge überall (+ einfach, − teuer); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-64` | Welchen Umfang haben Verkehrsmeldungen (Radio, Smartphone)? | `W-VK-014` | VD-012 | A Radio mit Verzögerung (+ passt, − Aufwand); B zusätzlich Smartphone (+ Komfort, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-65` | Welche Raten haben Unfälle und Pannen, und welche Einsatzzeiten gelten? | Keine Quelle | UP-001, UP-014 | A seltene Raten, einstellbar (+ passt zu ‚selten‘, − Werte nötig); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-66` | Wie tief sind die Dienste simuliert (Wachen, Besatzungen, Hubschrauber)? | `W-TV-006`: geeignete Tiefe | UP-003, UP-014 | A Zustandsfolge ohne Wachen (+ billig, − abstrakt); B Wachen und Fahrzeuge (+ glaubhaft, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-67` | Welche Rollen hat der Spieler bei Unfällen (Verursacher, Zeuge, Helfer, Opfer)? | Nur Zeuge ist beschlossen | UP-013 | A alle Rollen (+ Tiefe, − Aufwand); B nur Zeuge (+ einfach, − flach); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-68` | Welche Regeln gelten für Straßenrennen und Wetten? | `23#1025E` Nr. 44 | UP-006 | Entscheidung an Motorsport-Dokumentation ([Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md)); A eigene Regeln, B Teil des Untergrundsystems; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-69` | Wie wirken Katastrophen (Hochwasser, Sturm, Waldbrand) auf Verkehr und Evakuierung? | Wetter-Weltereignisse ohne Verkehrswirkung | UP-015 | A nur Sperrungen (+ einfach, − flach); B Evakuierungsverkehr (+ Wucht, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-70` | Welche Regeln gelten bei Kollisionen mit Fußgängern und Tieren? | Quellen schweigen | VT-011 | Hängt am Verletzungs- und Rechtssystem; A volle Konsequenz (+ Realismus, − Rating); B vereinfacht (+ einfach, − flach); F Freitext (Inhalt hat Vorrang) | PL + Recht |
| `CQ-71` | Braucht die Kartenschnittstelle Gehweg- und Radwegdaten? | `WF-RC01` kennt sie nicht | VT-010 | A Wunsch an WorldForge (+ konsistent, − Auftrag); B Spiel leitet ab (+ schlank, − Doppelung); F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-72` | Wie werden Kinder und Minderjährige im Verkehr dargestellt? | Kampfsystem hat die Frage offen | VT-012 | Folgt der im Kampfsystem zu klärenden Darstellungsfrage; A ausgeschlossen aus Gefahrenszenen, B vorgesehen mit Regeln; F Freitext (Inhalt hat Vorrang) | PL + Recht |

### 3.5 Besitz, Recht, Ermittlung (Module 18 bis 21)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-73` | Welche Methoden hat Fahrzeugdiebstahl, welche Folgen, und gibt es Kennzeichenmanipulation oder Umlackieren zur Verringerung der Fahndung? | Nur ‚ggf. Fahndungsreduktion‘ bei der Schnellwerkstatt ist genannt (`VEH-DEC-WE-016`) | BZ-013, WE-016 | A einfache Zugangsarten (+ machbar, − flach); B tiefe Methoden (+ Realismus, − Aufwand, Rating); F Freitext (Inhalt hat Vorrang) | PL + Recht |
| `CQ-74` | Welchen Umfang und Zeitpunkt hat die Versicherung? | `23#1025E` Nr. 16: später | BZ-001, `CU-08` | A späte Phase mit Schadensfall und Diebstahl (+ passt, − Aufwand); B nie (+ einfach, − widerspricht Antwort); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-75` | Welche Kennzeichenformate und Zulassungsregeln gelten je fiktivem Bundesstaat? | Keine Quelle | BZ-014 | A pro Bundesstaat Format (+ stimmig, − Daten); B ein Format (+ einfach, − flach); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-76` | Wie bringen Gäste im Koop eigene Fahrzeuge mit? | `N-KO-002`: bei einem Freund nur, was auf dessen Spielstand ist | KO-002, BZ-015 | A nur Host-Fahrzeuge (+ einfach, − Gäste ohne eigene); B Mitbringen mit Regeln (+ Wahl, − Komplexität); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-77` | Gibt es Grenzen des Besitzes (Anzahl, Garagenkapazität, laufende Kosten, Steuern)? | Keine Quelle | BZ-016 | A keine Grenzen (+ Freiheit, − Überfluss); B Garagenkapazität (+ Begrenzung, − Verwaltung); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-78` | Welche Verstöße, Bußgelder und Punkte kennt das Verkehrsrecht der Spielwelt? | `M-OD` offen | PV-015 | A einfacher Katalog (+ machbar, − flach); B ausgearbeitet je Bundesstaat (+ Tiefe, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL + Recht |
| `CQ-79` | Gibt es einen Führerschein für Spieler und NPCs? | Keine Quelle | PV-016 | A nein (+ einfach, − flach); B ja mit Prüfung (+ Welt, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-80` | Wie wird Fahrerflucht und werden Verkehrsdelikte verfolgt? | Vorschlag `UP-012` | UP-012 | A über Zeugen und Kennzeichen (+ passt, − Rechtsbasis offen); F Freitext (Inhalt hat Vorrang) | PL + Recht |
| `CQ-81` | Welche Stufen haben Kontrollen und Verfolgungen? | Eskalation durch Spielerverhalten (`N-BH-003`) | PV-012, PV-013 | A wenige feste Stufen (+ lesbar, − starr); B kontinuierliche Eskalation (+ flexibel, − Tuning); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-82` | Wie dicht sind Streifen, Kameras und Kontrollen? | Keine Quelle | PV-010 | A wenige, gezielt (+ spannend, − leichte Umgehung); B dicht (+ Druck, − Frust); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-83` | Wie wird Reisezeit im ÖPNV erlebt (Echtzeit, Zeitraffer, Überspringen)? | Verbunden mit Schnellreise | IT-013 | A Echtzeit auf der Fahrt (+ Welt, − Länge); B Zeitraffer (+ Kompromiss, − Aufwand); C Überspringen (+ Komfort, − entwertet Welt); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-84` | Welchen Umfang, welche Bezahlung und welche Lizenzen haben Jobs in frühen Phasen? | Keine Quelle für Lizenzen | IT-012 | A wenige Jobs früh (+ fokussiert, − wenig Auswahl); B alle früh (+ Vielfalt, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-85` | Spielen Taxi und Mitfahrdienst eine Rolle als Fortbewegung des Spielers? | Keine Quelle | IT-014 | A ja neben Eigenfahrzeug (+ Alternative, − Wirtschaft); B nein (+ einfach); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-86` | Wie hängen Smartphone-Dienste (Taxi, Mitfahrt, Lieferung) mit der Wirtschaft zusammen? | `M-CA0905` nur ‚may include‘ | IT-015 | A Dienste als Betriebe der Weltwirtschaft (+ konsistent, − Aufwand); B eigenständig (+ einfach, − Doppelung); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-87` | Wie interagieren Monster mit Fahrzeugen (Angriff, Verfolgung, Behinderung)? | Keine Quelle | EJ-013 | A nicht vorgesehen (+ einfach); B als Jagdelement (+ Atmosphäre, − Aufwand, Physik); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-88` | Gibt es Observation aus dem Fahrzeug, Ortung und digitale Fahrtdaten? | Keine Quelle | EJ-014 | A nein (+ einfach); B Observation (+ Ermittlung, − Aufwand); C Ortung und Telematik (+ Tiefe, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-89` | Wie tief sind Fahrzeugspuren ausgearbeitet? | Vorschlag `EJ-009` | EJ-009 | A Kennzeichen und Wiedererkennung (+ billig); B plus Reifen-, Lack-, Innenraumspuren (+ Tiefe, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |

### 3.6 Persistenz und Koop (Module 22 und 23)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-90` | Wie sehen Schema und Format der Fahrzeugsegmente aus? | Neue Datensatzarten brauchen ein versioniertes, migrierbares Format | PE-001, PE-007 | A eigenes Segmentformat (+ klar, − Aufwand); B erweiterte WorldFacts (+ vorhanden, − `Decision 0004` §6 widerspricht); C Entscheidung nach Prototyp; F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-91` | Wie wird ‚Laden gleicht Speichern‘ für Verkehr gelesen, und darf in Fahrt gespeichert werden? | `N-PE-022` | PE-012, PE-014 | A logische Gleichheit (+ billig, − Verkehr verändert sich); B Verkehr mitspeichern (+ exakt, − groß); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-92` | Wie passen Netzversion, Speichergröße und Karten-Neuimport zusammen? | `CK-04`; WorldForge-Reimport | PE-015, PE-016 | Entscheidung zusammen mit WorldForge; A Netzversion mit Migration, B Neustart bei Kartenwechsel; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-93` | Was geschieht mit Fahrzeugen deaktivierter Mods? | `N-PE-018` | PE-005 | A erhalten, unverändert unsichtbar (+ sicher, − Platz); B ersetzt (+ aufgeräumt, − Verlust); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-94` | Wie werden Wracks und abgestellte Fahrzeuge bereinigt? | Analog zu `COMBAT-DEC-PO-003` | PE-013 | A zeitbasierte Bereinigung (+ einfach, − Beweisverlust); B bereinigen nur ohne Beweiswert (+ sicher, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-95` | Welche Netzwerkarchitektur und Fahrphysik-Autorität gelten im Koop? | `E-KO-010`, `W-FP-010` | KO-005, KO-007 | A host-autoritativ; B besitzer-autoritativ; C Hybrid mit Vorhersage (Details in Modul 23); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-96` | Wie bringen Gäste Fahrzeuge in den Host-Spielstand? | `N-KO-002` | KO-002 | siehe `CQ-76`; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-97` | Wie funktionieren Fahrerwechsel, Beifahrerfähigkeiten und gemeinsame Fahrentscheidungen im Koop? | `N-KO-005` | KO-014 | A freie Wahl (+ einfach, − Streit); B Abstimmung (+ fair, − Umständlich); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-98` | Wie sehen Fahrzeug-, Renn-, Verkehrs- und Zeitlogik im offiziellen Online aus? | `N-KO-004`; Fahrzeuge bleiben beim Sitzungswechsel erhalten, Zielgröße 64 bis 100 Spieler falls machbar (`VEH-DEC-KO-016`) | KO-015, KO-016 | Entscheidung erst bei Planung des Online-Modus; A später planen; F Freitext (Inhalt hat Vorrang) | PL |

### 3.7 WorldForge, Unreal, Stufen, Leistung (Module 24 bis 27)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-99` | Wer liefert die Fahrspur-Ebene (Spurverbindungen, Parkflächen, Dienstpunkte)? | `RC-01` enthält sie nicht | WF-010, WF-015 | A WorldForge; B Spiel leitet ab; C Überlagerung auf stabilen IDs (Details in Modul 24 §5); F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-100` | Welches Austauschformat und welche Versionierung gelten? | Kein Format beschlossen | WF-009, WF-016 | A Erweiterung des WorldForge-Exports (+ eine Quelle, − Kopplung); B eigene Datei des Spiels (+ Trennung, − Abgleich); F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-101` | Welche Achsenkonvention und Einheit gilt im Spiel (Meter oder Zentimeter)? | `WF-DC01` offen bis zum Unreal-Test | WF-006 | A Meter im Kern, Umrechnung im Adapter (+ klar, − Pflege); B Zentimeter überall (+ nah an Unreal, − bricht `DC-01`); F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-102` | Werden Parkflächen, Tankstellen und Depots vom Generator oder vom Spiel erzeugt? | M3 erzeugt Parzellen mit Straßenzugängen | WF-015, KE-013 | A Generator (M3); B Spiel; C Überlagerung; F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-103` | Kommen Geschwindigkeits- und Regelprofile aus der Karte oder aus dem Spiel? | Entwurfsgeschwindigkeit ist kein Limit | WF-016 | A Spiel (+ Regeln bei Rechtsbereich); B Karte (+ eine Quelle, − vermischt Recht); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-104` | Darf die Referenzwelt WORLD_700 als Testbasis dienen? | `CLAUDE.md` §2: keine Geometrieübertragung ohne Genehmigung | WF-014 | A nein, Platzhalter von Hand (+ sauber, − Aufwand); B ja mit Genehmigung (+ realistischer, − Ausnahmefall); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-105` | Wie werden ungültige Verweise nach einem Karten-Neuimport behandelt? | Straßenverlegungen sind vorgesehen (`WF-C09`) | WF-011 | A melden und migrieren nach Regel; B Spielstand ungültig (hart); C Karte schreibt Alias-IDs fort; F Freitext (Inhalt hat Vorrang) | PL + WorldForge |
| `CQ-106` | Welche Unreal-Version und welche Plugins gelten? | `CLAUDE.md` §5: vor dem Festlegen prüfen | UE-001, UE-012 | A bei 5.8.x bleiben (+ Stand der Integration, − Plugin-Reife); B später festlegen; F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-107` | Beruht der Verkehr auf Mass Entity, einem eigenen Kern oder einer Mischung? | Auch im NPC-Entwurf als Kandidat offen | UE-013 | A Mass Entity (+ vorhanden, − Abhängigkeit); B eigener Kern (+ portabel, − Aufwand); C Mischung (+ pragmatisch, − Schnittstellen); F Freitext (Inhalt hat Vorrang) | nach Spike und Genehmigung |
| `CQ-108` | Nutzt die Navigation einen eigenen Graph aus Karten-IDs oder das Navigationsmesh von Unreal? | Verkehr braucht Spurebene | UE-014 | A eigener Graph (+ Kontrolle, ID-Bezug, − Aufwand); B Navigationsmesh (+ vorhanden, − Spurebene fehlt); F Freitext (Inhalt hat Vorrang) | nach Spike |
| `CQ-109` | Wie nimmt die Verkehrs-KI wahr (Physik-Traces oder Graphnachbarschaft)? | Kostenfrage | UE-014, PF | A Traces (+ genau, − teuer); B Graphnachbarschaft (+ billig, − ungenau); C Mischung; F Freitext (Inhalt hat Vorrang) | nach Messung |
| `CQ-110` | Welche Radien, Taktungen, Fahrzeugzahlen und Stufenschwellen gelten? | Keine Quelle | LS-005, LS-013 | Nur durch Messung (siehe `CQ-118`); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-111` | Was geschieht, wenn der Spieler schneller fährt, als gestreamt wird? | Sicherung nötig | LS-010 | A Geschwindigkeit begrenzen; B Verkehr dünn halten; C anhalten bis geladen; F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-112` | Wird plattformübergreifender Determinismus verlangt? | Zufall: `DeterministicRandom`; Koop-Option `KO-010` | KI-010, KO-010 | A ja für Verkehrsmenge (+ erlaubt lokales Erzeugen, − strenge Regeln); B nein (+ frei, − Replikation nötig); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-113` | Werden Baustellen und Sperrungen als Laufzeitobjekte oder Data Layers geführt? | Streaming-Wahl | LS-011 | A Laufzeitobjekte (+ flexibel); B Data Layers (+ streamingfreundlich, − starr); F Freitext (Inhalt hat Vorrang) | nach Spike |
| `CQ-114` | Wie gleich muss die Szene beim Wiederbesuch sein? | `N-PE-022` | LS-014 | A gleiche Lage im Zeitfenster (+ glaubwürdig, − Aufwand); B nur ähnliche Dichte (+ billig, − Brüche sichtbar); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-115` | Welche Zielhardware und Qualitätsstufen gelten für den Verkehr? | Hardwarestufen sind offen ([Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md)) | PF-002 | A eine Stufe (+ einfach); B mehrere Stufen mit Dichteregler (+ breit, − Pflege); F Freitext (Inhalt hat Vorrang) | PL |
| `CQ-116` | Welche Budgets (Bildzeit, Speicher) gelten je Stufe? | Keine Quelle | PF-002 | Nur durch Messung (`CQ-118`); F Freitext (Inhalt hat Vorrang) | nach Prototyp |
| `CQ-117` | Wie skalieren Verkehr und Fußgänger in großen Städten? | Metropolis ist der Spitzenfall | PF-011 | A nur Profile ändern (+ einfach); B zusätzliche Stadtsysteme (+ Detail, − Aufwand); F Freitext (Inhalt hat Vorrang) | nach Messung |
| `CQ-118` | Welche Messmethodik und Referenzszenarien gelten? | `PF-010` | PF-010 | A Szenarien aus Modul 31 (+ vorhanden); B eigene (+ passgenau, − Aufwand); F Freitext (Inhalt hat Vorrang) | PL |

---

## 4. Als beantwortet geführt

Diese Punkte stehen im Auftrag als mögliche offene Fragen. Die Quellen beantworten sie bereits; sie sind **nicht** erneut offen.

| Frage | Antwort der Quellen | Register |
|---|---|---|
| Dynamischer Verkehr, wie extrem? | Dynamisch nach Uhrzeit, Wochentag, Stadtteil, Veranstaltungen, Wetter, Ferien, aber nicht zu extrem | `VEH-DEC-VD-001` |
| Stoßzeit-Staus und Meldungen? | Möglich, im Radio angesagt (weicher Wortlaut: `CU-09`) | `VEH-DEC-VD-002` |
| Unfälle durch NPCs? | Ja, selten, außer der Spieler rammt | `VEH-DEC-UP-001` |
| Pannen, Baustellen, Sperrungen? | Ja | `VEH-DEC-UP-002`, `VEH-DEC-VD-003` |
| Gebrauchtmarkt, Verschleiß, Kraftstoff? | Ja; Verschleißteile, Sorten, E85, Verbrauch beschlossen | `VEH-DEC-KL-004`, `VEH-DEC-ZV-001`, `VEH-DEC-KE-001` |
| Manuelles Getriebe als Standard? | Ja, mit Kupplung, Schleifpunkt, Abwürgen; Automatik erhältlich | `VEH-DEC-PH-001`, `VEH-DEC-KL-006` |
| DualSense-Belegung des Fahrens? | Acht Funktionen vorgegeben, frei belegbar (Lücken: `CU-03`) | `VEH-DEC-ST-001` |
| Elektrofahrzeuge? | Zulässig, Verbrenner dominieren | `VEH-DEC-KE-005` |
| Fiktive Fahrzeugnamen? | Ja | `VEH-DEC-KL-003` |
| Parkplatzsuche erzwingen? | Nein | `VEH-DEC-SP-001` |
| Taxi, Rideshare, Lieferung, Trucking als Jobs? | Ja; Rideshare-Wettbewerb später | `VEH-DEC-IT-001` bis `IT-004` |
| Polizeikontrollen und Warn-Apps? | Möglich, Apps zeigen sie an | `VEH-DEC-KR-001`, `VEH-DEC-PV-001` |
| Zulassung und Papierkram? | Stark abstrahieren | `VEH-DEC-BZ-002` |
| Darf man ein Fahrzeug per Menü zwischen Handschaltung, Doppelkupplung und Automatik umschalten? | Nein, das Getriebe gehört zur Variante | `VEH-DEC-KL-016` |
| Wie oft muss man tanken? | Realistische Reichweiten, kein Tankstellen-Spam (Zeiten offen: `CQ-31`) | `VEH-DEC-KE-014` |
| Welche Funktionen hat die Fahrzeug-App? | Schloss, Fenster, Licht, Motor, Kofferraum, Fernstart, Parkposition, Kraftstoff oder Ladung, Status; nur vorhandene Funktionen | `VEH-DEC-EI-013` |
| Können Körper, Verletzte und Tiere im Fahrzeug transportiert werden? | Ja, je nach Fahrzeug an verschiedenen Plätzen; Polizei kann sie finden | `VEH-DEC-EI-015` |
| Beeinflusst Alkohol das Fahren der Spielfigur? | Ja | `VEH-DEC-KI-014` |
| Wetter und Jahreszeiten? | Wirken auf Physik, Verkehr und Straßen | `VEH-DEC-PH-007`, `VEH-DEC-PH-008` |
| Host-Spielstand im Koop? | Ja; Gäste behalten, was auf dem Host-Spielstand ist | `VEH-DEC-KO-001`, `VEH-DEC-KO-002` |
| Fahrzeuge persistent? | Ja, als Richtung | `VEH-DEC-PE-001` |

---

## 5. Zuordnung zu den Pflichtthemen des Auftrags

| Pflichtthema | Wo behandelt | Offene Fragen |
|---|---|---|
| Glaubwürdiger Verkehr (Fahrstile, Regeln, Spurwechsel, Überholen, Abbiegen, Vorfahrt, Signalphasen, Parkplatzsuche, Staus, Unfälle, Einsatzfahrzeuge, Wetter) | Module 11, 13, 15, 16; Katalog in Modul 13 §5 | `CQ-37`, `CQ-38`, `CQ-41`, `CQ-49`, `CQ-51`, `CQ-60` bis `CQ-65` |
| Persistente NPC-Gesellschaft (NPC-Fahrzeuge, Tagesabläufe, Pendeln, Verlassen des Gebiets, Fahrzeugzustand, Reaktionen) | Module 14, 22, 26 | `CQ-55` bis `CQ-59`, `CQ-90`, `CQ-91` |
| WorldForge-Schnittstelle (Straßentyp, Spuren, Richtung, Geschwindigkeit, Kreuzungen, Schilder, Navigation, Parken, Brücken, Tunnel) | Module 10, 11, 24 | `CQ-36`, `CQ-99` bis `CQ-105` |
| Leistung bei rund 2.200 km² (nah, mittel, fern, Aktivierung, LOD, Mass-Optionen, Speicher, Streaming, Determinismus, Persistenz, Skalierung großer Städte) | Module 25, 26, 27 | `CQ-106` bis `CQ-118` |
| Wechselwirkungen (Kampf, Verletzung, Ermittlung, Polizei, NPC-Verhalten, Wetter, Tageszeit, Wirtschaft, Koop, Speichern) | Module 07, 16, 18, 19, 20, 21, 23, 22 | `CQ-23`, `CQ-67`, `CQ-70`, `CQ-73`, `CQ-78`, `CQ-84`, `CQ-95` |

---

## 6. Vorschlag für die Reihenfolge der Klärung

> **💡 VORSCHLAG.** Die Reihenfolge ordnet nach Abhängigkeiten; sie ist keine Entscheidung. Jede Frage kann mit Freitext beantwortet werden.

1. **Grundlagen, von denen vieles abhängt:** `CQ-37` (Fahrtrichtung), `CQ-38` (Geschwindigkeiten, Spuren, Schilder), `CQ-99` (Fahrspur-Ebene), `CQ-49` (Verkehrsmodell), `CQ-06` und `CQ-07` (Sortiment, Antriebsarten), `CQ-23` (Schadensmodell, `CK-06`); vorab der Status der Oktober-Details (`CK-07`).
2. **Prototyp- und Messfragen:** `CQ-10` (Fahrzeugphysik), `CQ-13`, `CQ-14`, `CQ-95` (Koop-Autorität), `CQ-107`, `CQ-108`, `CQ-109` (Unreal-Optionen), `CQ-110`, `CQ-111`, `CQ-116`, `CQ-118`.
3. **Umfang und Inhalt:** `CQ-04`, `CQ-05` (Reihenfolge, Verkehr im Vertical Slice), `CQ-62` bis `CQ-66`, `CQ-84`, `CQ-28` bis `CQ-30`, `CQ-15` bis `CQ-17`.
4. **Recht und Rating** (vor Veröffentlichung mit Fachberatung): `CQ-53`, `CQ-70`, `CQ-72`, `CQ-73`, `CQ-78`, `CQ-79`, `CQ-80`, `CQ-43`.
5. **Weltintegration und Kartenabstimmung:** `CQ-35`, `CQ-36`, `CQ-39`, `CQ-55`, `CQ-100` bis `CQ-105`.
6. **Später:** `CQ-74` (Versicherung), `CQ-85`, `CQ-86`, `CQ-98` (Online).
