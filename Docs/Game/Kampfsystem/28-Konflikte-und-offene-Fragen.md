# Kampfsystem 28: Konflikte und offene Fragen

> [!IMPORTANT]
> **Dieses Modul entscheidet nichts.** Es führt (1) Widersprüche zwischen Quellen mit beiden Aussagen und ihren Pfaden, (2) unbelegte oder uneindeutige Angaben und (3) alle offenen Fragen mit Begründung, betroffenen Anforderungen, Optionen mit Vor- und Nachteilen und dem Vermerk, welche Entscheidung der Projektleitung vorbehalten bleibt. Fragen, die bestehende Dokumente bereits beantworten, stehen in Abschnitt 4 und sind **nicht** erneut offen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 27](27-Entscheidungsregister.md).

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
| `CK-01` | **Todes- und Scheiterloop** | `M-PD` ‚Death / failure — OPEN‘ (Stand 31.08.2026): ‚Final death loop is not decided‘; Spielvision §14.2 ❓; `M-OD`; NPC-Dokumentation `N-NL-013`, `N-LB-023` ❓ | Entwickler `23#364E` P2, P3, P16 (25.08.2026): Gameplay-Tod führt zum letzten Checkpoint mit minimal überlebensfähigem Zustand, Permadeath aktivierbar, Story-Tod bindend; bestätigt mit ‚der Rest passt‘ `23#366E`; übernommen als 📋 in [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) §3.1 und `K-KS-005`, als bestätigter Vorschlag (💡) in [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §15, als 🧭 in `W-RE-008` | ⚠️ offen. Beide Aussagen können nebeneinander bestehen (Zwei-Ebenen-Lesart, [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) §7), das ist aber keine Entscheidung. `CQ-07` |
| `CK-02` | **Keine Plot-Rüstung gegenüber geschützten Figuren und Friendly Fire** | `M-PD`, `23#104E` P9, P18: Spieler und menschliche NPCs teilen die Anatomie, kein Plot-Armor; Friendly Fire gegen Begleiter ist physisch möglich und kann sie töten (`M-PD` ‚Multiple-opponent combat…‘) | Wichtige NPCs sterben nur durch den Spieler (`E-ZE-007`, `23#1057E` P105), nicht durch banale Simulationsfehler (`N-LB-017`); Kernfiguren des Hunter-HQ haben keinen zufälligen Sandbox-Tod (`N-CO-007`, `M-CA0905` Z. 44) | ⚠️ offen; NPC-Konflikt C-N1. `CQ-79` |
| `CK-03` | **Rolle von Tastenkombinationen im Kampf** | `23#171E`: ‚der Spieler soll nicht eine Tabelle an Tastenkombinationen auswendig lernen‘; `M-PD` ‚Melee / grappling‘: keine riesigen Kombotabellen | `35#791E` P3: Menü mit Beschreibung, Tastenkombination und Vorschau für gelernte Techniken; `35#797E` bestätigt die Linie; `35#798A` (Assistent): Combos und Tastenkombinationen spielen im Kampf eine viel größere Rolle | ⚠️ offen; vereinbar, wenn die Bibliothek nur ein Nachschlagewerk ist. `CQ-02`, `CQ-12` |
| `CK-04` | **Gliedmaßenverlust beim Spieler** | Gameplay-Systeme §4 / `M-PD`: plausible Ergebnisse bei Erwachsenen umfassen den Verlust von Fingern, Händen, Gliedmaßen | `M-CA0905`: ‚Amputation/prosthetics only via authored story if used‘; schwere Verletzungen verursachen temporäre Funktionseinschränkung | ⚠️ offen. `CQ-53` |
| `CK-05` | **Größenziel des Spielstands** | `M-PD`: etwa 20–50 MB, Zuverlässigkeit vor Größe | Assistent (`35#280A`): 50–150 MB; [Welt K13](../World/21-Konflikte-und-Widersprueche.md#k13-save-größenziel); NPC-Konflikt C-N2 | ⚠️ offen, hier nur als Verweis. `CQ-104` |
| `CK-06` | **Fahndungsstufen oder kein Sternesystem** | `M-PD`, `E-RE-013`: kein Sternesystem | Quiz-Text `48#156A` nennt Fahndungsstufen (`E-PO-003`) | ⚠️ offen, Verweis auf [Ermittlung](../Ermittlung/21-Konflikte-und-offene-Fragen.md). `CQ-83` |
| `CK-07` | **Serienlogik als Standard gegenüber eigener Gestaltung** | `23#362E`, `23#371E`, `23#372A`: Wenn nichts anderes gesagt ist, gilt die Serienlogik (`K-IP-001`) | `23#90A`, `23#91E`, `K-IP-003`, `M-PD` ‚Gunplay / weapons‘: eigene Namen, Designs, Waffen, Lore; rechtliche Clearance vor Release | 🧭 Für Waffen und Requisiten ist die Linie ‚eigene Gestaltung mit etwas Ähnlichkeit‘ (`COMBAT-DEC-WA-009`); das Verhältnis im Ganzen bleibt bei den Kreaturen-Dokumenten |

---

## 2. Unbelegte oder uneindeutige Angaben (CU)

| ID | Gegenstand | Befund | Umgang |
|---|---|---|---|
| `CU-01` | **DualSense-Kampfbelegung** (Quadrat Kontextinteraktion/Angriff, Kreis Ausweichen, Dreieck schwerer Angriff) | Nur im Auftrag vom 23.10.2026; in Chats, Master und Repo nicht gefunden (nur Kreuz = Springen und die Fahrbelegung) | als 🔍 UNBELEGT geführt ([Modul 06](06-Steuerung-und-Eingabekontext.md)); `CQ-01` |
| `CU-02` | **‚auf Grounded würde ich aber alle solche Sachen disabled machen‘** (`23#116E` P7) | Bezug unklar: Abschaltoptionen oder Auswirkungen auf das Zielen; der Assistent legte Ersteres aus (`23#117A`) | als 🧭 geführt (`COMBAT-DEC-FK-006`); Klärung bei `CQ-04` |
| `CU-03` | **‚B wäre wie in Sniper Elite‘** (`23#141E` P8) | Zuordnung zu den Kill-Cam-Varianten in `23#140A` nicht belegt | Kill-Cam allgemein 📋, Variantenzuordnung offen (`CQ-21`) |
| `CU-04` | **Antwortnummern im Nahkampfblock** (`23#167E` P11 antwortet auf ‚Stil‘ statt auf ‚Größe und Kraft‘ in `23#166A` §11) | Die Nummerierung der Antworten weicht stellenweise von der der Fragen ab | Zuordnung mit Vorbehalt (`COMBAT-DEC-NK-015`, `-034`) |

---

## 3. Offene Fragen (CQ)

Spalten: **Warum nötig** (welche Entscheidung blockiert sie), **Betroffen** (Anforderungen und Module), **Optionen mit Vor- und Nachteilen** (Kurzfassung; Details in der verlinkten Gegenüberstellung), **Entscheidung** (wer entscheidet; PL = Projektleitung, ‚nach Prototyp‘ = erst nach einer Messung).

### 3.1 Grundprinzipien, Zustände, Nahkampf (Module 02 bis 04)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-03` | Schadenszahlen und Trefferanzeigen | Rückmeldung ohne Zahlen als Fortschrittsmotor (`GP-003`) muss definiert sein | GP-003, [02](02-Kampfvision-und-Grundprinzipien.md) | A Keine Zahlen (+ konsistent, − wenig Rückmeldung); B Trefferindikatoren ohne Zahlen (+ lesbar, − HUD); C je Schwierigkeit | PL |
| `CQ-04` | Katalog der Einstellbereiche und Realismusstufen | Presets und Grounded brauchen eine Reglerliste | GP-010 bis -015, -033, [02](02-Kampfvision-und-Grundprinzipien.md) | A Wenige Gruppen (+ übersichtlich, − grob); B Feine Regler (+ Kontrolle, − 150 Regler); C Presets plus Anpassen (+ beides, − Pflege) | PL |
| `CQ-05` | Combat Focus | Bevorzugte Alternative zur Zeitlupe, Details fehlen | GP-021, -022 | A Kurzer fokussierter Zustand mit Erholung (+ klar, − Balance); B Passiver Trainingseffekt (+ keine Taste, − unsichtbar); C Kombination | PL |
| `CQ-06` | Gesamtmodell der Kampffertigkeiten | Progression ist offen und betrifft Lehrsystem, Ausdauer, Waffenübung | GP-024, -025, AU-005 | A Reine Nutzung mit abnehmenden Erträgen (+ organisch, − Ausnutzbarkeit); B Nutzung plus Lehrer (+ trägt `NK-036`, − Struktur); C Körpermodell-gekoppelt (+ konsistent, − komplex) | PL (eigener Planungsblock) |
| `CQ-07` | Todes- und Scheiterloop | `CK-01`; ohne ihn sind Speichern, Rettung, Koop und Tests unvollständig | BW-001, -012, -027, -028, [14](14-Bewusstlosigkeit-Tod-und-Rettung.md) | Siehe Zwei-Ebenen-Gegenüberstellung in Modul 14 §7 (A Zwei Ebenen, B nur Rettung, C nur Checkpoint, D je Voreinstellung) | PL |
| `CQ-08` | Nahkampf-Paare als Interaktionssitzungen | Entscheidet Datenmodell, Netzwerk und Animation der Griffe | ZA-011, -014, AN-010 | A Interaktions-Framework (+ Wiederverwendung, − Anpassung); B Eigenes Paarmodell (+ maßgeschneidert, − Doppelstruktur) | PL nach Prototyp |
| `CQ-09` | Schwellen der Wahrnehmungszustände, Detected | Speichersperre ohne ausnutzbare Übergänge | ZA-003, GP-028, SP-004 | A Feste Sichtbestätigung (+ klar, − ausnutzbar); B Zeitfenster mit Hysterese (+ robust, − Komplexität); C Mehrfachbeleg | PL nach Prototyp |
| `CQ-10` | Stufen des Spielerzustands | Anzeige und Übergänge | ZA-002, -015 | A Abgeleitete Zusammenfassung (+ ein Primärzustand); B Eigene Größe (+ einfach, − Doppelung) | PL |
| `CQ-11` | Tiefe des Nahkampfsystems und Animationsabdeckung | M-OD offen; bestimmt Aufwand | NK-003, -010, -011 | A Kleiner Kern (+ machbar, − zu wenig Auswahl); B Große Bibliothek (+ Tiefe, − Aufwand) | PL |
| `CQ-12` | Lehrsystem | Kernpunkt des Kampfes; Details fehlen | NK-035 bis -038 | A Gym und Trainer (+ Welt, − Inhalt); B Training durch Benutzung (+ organisch, − wenig Struktur); C Beides | PL |
| `CQ-13` | Fight Club | Umfang und Folgen | NK-013, -014 | Siehe Modul 04 §6 (A Minimal, B Szene, C Lernort) | PL |
| `CQ-14` | Nichttödliche Ausschaltung | Dauer, Erholung, Rechtsfolgen | NK-011, -012, DE-012 | A Nur Körpergriffe (+ einfach); B Zusätzlich Mittel (+ Taktik, − Recht); C Dauer als Körperzustand (+ konsistent) | PL |
| `CQ-15` | Weiche Zielhilfe | Stärke je Gerät und Stufe | NK-006 | A Fest (+ einfach); B Je Schwierigkeit (+ Grounded aus); C Je Gerät | PL |
| `CQ-16` | Finisher | Umfang und Bedingungen | NK-030 | A Selten und kontextbezogen (+ Linie der Quellen); B Je Wesen und Waffe (+ trägt Schwächen); C Mit Kill-Cam (+ Inszenierung, − Frequenz) | PL |

### 3.2 Fernkampf, Steuerung, Verteidigung, Bewegung (Module 05 bis 08)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-01` | Kampfbelegung DualSense | Auftragsvorgaben ohne Quelle (`CU-01`) | ST-009 bis -013 | A Bestätigen (+ Klarheit); B Andere Belegung; C Erst nach Prototyp | PL |
| `CQ-02` | Direct oder Assisted Control im Kampf | Grundlage der Eingabe | ST-014, NK-040 | Siehe Modul 04 §9 | PL |
| `CQ-17` | Deckungssystem | Kein Quellenbeleg; Kern des Gunplays | FK-031, KI-031 | A Freies Verstecken ohne Mechanik (+ einfach, − wenig Taktik); B Lehnen und Blindfeuer (+ Standard, − Aufwand); C Gegner-Deckungslogik | PL |
| `CQ-18` | Schießen aus und auf Fahrzeuge | Fahrzeuge haben ballistische Strukturen, aber keine Kampfregel | FK-017, -032 | A Kein Schießen aus Fahrzeugen (+ einfach); B Eingeschränkt (+ kontrollierbar); C Voll (+ Tiefe, − Aufwand) | PL |
| `CQ-19` | Streuung, Rückstoß, Projektilmodell | Bestimmt Gunplay-Gefühl und Leistung | FK-013, -014, -033 | Siehe Modul 05 §6 (A bis C) | PL nach Prototyp |
| `CQ-20` | Entfernungs- und Windhilfen | Grounded verlangt Gerät, andere Stufen offen | FK-027 | A Anzeige ab Normal (+ zugänglich); B Gerät überall (+ konsistent, − hart) | PL |
| `CQ-21` | Kill-Cam-Kriterien | Häufigkeit und Auslöser | FK-028, CU-03 | A Nur Fernschüsse; B Auch Nahkampf; C Nach Qualitätswert | PL |
| `CQ-22` | Schießwettbewerbe | Umfang der ‚Events gegen andere Leute‘ | FK-026 | A Nur NPC; B Auch Spieler; C Weglassen im Start | PL |
| `CQ-23` | Haptik im Kampf | Adaptive Trigger, Rückstoß | ST-016 | A Standardvibration; B Adaptive Trigger für Waffen; C Abschaltbar | PL |
| `CQ-24` | Maus und Tastatur im Kampf | Nur Leertaste, Shift, E belegt | ST-017 | A Standardlayout; B Anlehnung an Controller | PL |
| `CQ-25` | Kontextumschaltung und Eingabeprioritäten | Doppelte Tastenrolle (Interaktion/Angriff/Fahren) | ST-010, -011, -018 | Siehe Modul 06 §5 (A bis E) | PL |
| `CQ-26` | Kampf in VR | Nur Interaktion beschrieben | ST-020 | A Später gleich; B Eigene Eingabe | PL |
| `CQ-27` | Auswertung Block, Parade, Konter | Basis der Verteidigung | DF-001, -004, -008, -015 | Siehe Modul 07 §5 (A Zeitfenster, B Physik, C Hybrid) | PL nach Prototyp |
| `CQ-28` | Ausweichkatalog | Kosten und Eingabe | DF-006 | A Wenige Bewegungen; B Katalog mit Kosten; C Rolle nur situativ | PL |
| `CQ-29` | Abwehr von Projektilen und Monsterfernangriffen | Quellen schweigen | DF-014 | A Nur Deckung (+ konsistent); B Ausweichen vor langsamen Angriffen (+ Monster); C Art-spezifisch | PL |
| `CQ-30` | Darstellung der Angriffseigenschaften | Lesbarkeit ohne Indikatoren | DF-003 | A Nur Animation und Audio; B Optionale Hinweise; C Je Schwierigkeit | PL |
| `CQ-31` | Sprungstärke, Hindernishöhen, Hindernisse im Kampf | Proposal 0009 §5 offen | BT-002, -003, -015, -017 | Siehe Modul 08 §5 | PL |
| `CQ-32` | Kampf beim Klettern und Schwimmen | Nicht beschrieben | BT-009, -017 | A Ausschließen; B Eingeschränkt; C Voll | PL |

### 3.3 Waffen, Ausdauer, Treffer, Verletzung (Module 09 bis 13)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-33` | Waffenliste, Kaliber, Herstellernamen | Assets und Recht hängen daran | WA-001, -003, -017 | Siehe Modul 09 §8 (A reale Marken, B fiktive Hersteller, C Mischung) | PL + Recht |
| `CQ-34` | Körperschutz | Projektilweg durch Kleidung ohne Schutzmodell | WA-016 | Siehe Modul 09 §8 | PL |
| `CQ-35` | Seltene Hunterwaffen | Beschaffung und Zahl | WA-009 | A Wenige, quest-gebunden; B Mehr, systemisch gefunden | PL |
| `CQ-36` | Inventarmodell für Waffen | Zugriffszeiten, Slots | WA-005, -006, -018 | Siehe Modul 09 §8 | PL |
| `CQ-37` | Komfortmodus der Munition | Zugänglichkeit | MU-026 | Siehe Modul 10 §7 | PL |
| `CQ-38` | Munitionswirtschaft | Knappheit und Grounded | MU-023, -027 | A Einfach (Preise); B Herstellung und Wiederladen | PL |
| `CQ-39` | Weitere Waffenarten | Bogen, Wurf, Brand, Sprengstoff fehlen | WA-019 | A Weglassen; B Wenige Spezial; C Volle Familie | PL |
| `CQ-40` | Störungsbeseitigung | Automatik oder Handgriff | MU-018, -025 | Siehe Modul 10 §7 (A, B, C) | PL |
| `CQ-41` | Waffenzustand | Größen und Pflege | MU-019, -020 | Siehe Modul 10 §7 | PL |
| `CQ-42` | Ausdaueranzeige | Ressourcenmodell | AU-013 | Siehe Modul 11 §4 (A Balken, B verteilte Ermüdung, C verdeckt) | PL |
| `CQ-43` | Formeln der Ausdauer | M-OD offen | AU-012 | A Einfach; B Körpermodell-gekoppelt | PL nach Prototyp |
| `CQ-44` | Traglast | Bewegung und Geräusch | AU-014 | A Keine; B Gewicht; C Volumen | PL |
| `CQ-45` | Datengrundlage Durchschlag, Gewebe, Knochen | M-OD offen | TK-017, -019, FK-016 | A Literaturwerte; B Messung am Prototyp; C Spielwerte nach Gefühl | PL nach Prototyp |
| `CQ-46` | Trefferzonengeometrie und Wundeintrag | Technische Grundlage | TK-018, -020 | Siehe Modul 12 §8 | PL nach Prototyp |
| `CQ-47` | Gore-Schwellen und Minderjährige | M-OD, Rating, Recht | TK-021, GP-020 | Siehe Modul 12 §8; Prüfung durch Plattform und Recht | PL + Recht |
| `CQ-48` | Trefferimpuls | Quellen schweigen | TK-013, -022 | Siehe Modul 12 §8 | PL nach Prototyp |
| `CQ-49` | Verletzung von Tieren und Hunden | Hund als Begleiter | TK-023 | A Gleiches Körpermodell; B Vereinfacht | PL |
| `CQ-50` | Hitzonen bei morphenden Körpern | Konsistenz zur Charaktererstellung | TK-020 | Siehe Modul 12 §8 | PL nach Prototyp |
| `CQ-51` | Heilzeiten und Zeitkompression | Weltzeitregel offen | VL-005, -021, -023 | Siehe Modul 13 §7 | PL |
| `CQ-52` | Medizinlisten, Krankenhauswirtschaft | Entscheidungsstatus §5.3 | VL-003, -004, -022 | A Wenige Gegenstände; B Umfangreich | PL |
| `CQ-53` | Gliedmaßenverlust beim Spieler | `CK-04` | VL-012, -013, -019 | Siehe Modul 13 §7 | PL |
| `CQ-54` | Infektionsmodell | Tiefe | VL-014 | Siehe Modul 13 §7 | PL |
| `CQ-55` | Behandlungsoberfläche | Aufwand der Bedienung | VL-006, -009, -020 | Siehe Modul 13 §7 | PL |

### 3.4 Tod, Kreaturen, Fallen, KI, Nemesis (Module 14 bis 19)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-56` | Zuordnung der Lagen zu Rettungsausgängen | Rettung hängt von der Lage ab, ohne Regel | BW-007, -008 | A Regelbasiert nach Ort und Begleitern; B Zufall gewichtet; C Skript je Fall | PL |
| `CQ-57` | Anatomischer Mindestzustand nach Gameplay-Tod | `23#370A` verschiebt auf den Medizin-Block | BW-012, -029 | A Fester Zustand (+ klar); B Aus dem Körper abgeleitet (+ konsistent, − aufwendig) | PL |
| `CQ-58` | Krankenhaus als Ort | Kosten, Dauer, Meldepflicht | BW-030 | A Einfach (Zeit); B Mit Wirtschaft und Recht | PL |
| `CQ-59` | Voreinstellungen der Todesfolgen, Permadeath | Death Consequences als Einstellbereich | BW-013, -019 | Siehe Modul 14 §7 | PL |
| `CQ-60` | Near Death und Abgrenzung | Schwellen und Dauer | BW-006, -020, -031 | A Kurzer Zustand ohne Timer; B Längere Notversorgung; C Je Wesen | PL |
| `CQ-61` | Niederlage durch Monster | Entführung, Fluch, Besessenheit | BW-009, -026 | A Wenige Ausgänge; B Art-spezifisch; C Als Story | PL |
| `CQ-62` | Koop-Tod | Gleichzeitiger Ausfall | BW-025 | A Gemeinsames Scheitern; B Wiederbelebung durch Dritte | PL |
| `CQ-63` | Kampf gegen Besessene | Wirt schonen oder töten | KS-012 | Siehe Modul 15 §7 | PL |
| `CQ-64` | Kampfprofile der Kernarten | Artenliste fehlt | KS-024, -025 | Siehe Modul 15 §7 | PL |
| `CQ-65` | Körperlose Wesen | Mechanik fehlt | KS-026 | Siehe Modul 15 §7 | PL |
| `CQ-66` | Große Kämpfe | Phasen und Checkpoints | KS-014, -027 | A Eine Phase; B Phasen mit Zwischenspeicherung | PL |
| `CQ-67` | Schwellen für Hypothese und Bestätigung | `K-FO-012` | SJ-016, -017 | Siehe Modul 16 §6 | PL |
| `CQ-68` | Rituale: Kosten und Kampfnähe | `K-JA-011` | SJ-018 | Siehe Modul 16 §6 | PL |
| `CQ-69` | Rückmeldung der Wirksamkeit | Lesbarkeit | SJ-019 | Siehe Modul 16 §6 | PL |
| `CQ-70` | Fallenbau im Einsatz | Bedienung | FF-005, -011 | Siehe Modul 17 §6 | PL |
| `CQ-71` | Nichttödliche Mittel | Rechts- und Balancefragen | FF-012, -013 | Siehe Modul 17 §6 | PL |
| `CQ-72` | Dauerhafte Verwahrung | Bunker offen | FF-014, -015 | Siehe Modul 17 §6 | PL |
| `CQ-73` | Furcht und Aufgabe | Schwellen | KI-021, -030 | Siehe Modul 18 §8 | PL nach Prototyp |
| `CQ-74` | Lernen und Gedächtnisgrenzen | `K-KI-019` | KI-029 | Siehe Modul 18 §8 | PL |
| `CQ-75` | KI-Rechenbudget | Leistung | KI-032 | Siehe Modul 18 §8 | PL nach Prototyp |
| `CQ-76` | Unbeteiligte im Kampf | Zivilisten, Geiseln, Tiere | KI-033 | A Flucht und Schutz; B Als Ziele; C Szenario | PL |
| `CQ-77` | Bewertungsmodell der Taktikwahl | Gewichtung und Variation | KI-016, -027 | A Regelsatz; B Nutzenfunktion; C Lernend | PL nach Prototyp |
| `CQ-78` | Lernmodell und Rückkehr | `N-NE-022` | NM-005, -019 | Siehe Modul 19 §6 | PL |
| `CQ-79` | Tötung wichtiger NPCs | `CK-02` | NM-014, -020 | Siehe Modul 19 §6 | PL |
| `CQ-80` | Ruhm aus Kämpfen | Häufigkeit | NM-021 | A Selten; B Systemisch häufig | PL |

### 3.5 Recht, Vermeidung, Animation, Koop, Speicher, Leistung (Module 20 bis 25)

| ID | Frage | Warum nötig | Betroffen | Optionen (Vor- und Nachteile) | Entscheidung |
|---|---|---|---|---|---|
| `CQ-81` | Waffenrecht | M-OD ‚Crime / law / evidence‘ | PO-024, WA-020 | Siehe Modul 20 §7 | PL + Recht |
| `CQ-82` | Selbstverteidigung | Bewertungskriterien | PO-011 | A Regelbasiert; B Fallweise | PL |
| `CQ-83` | Straftatenkatalog, Beweiswertung | M-OD offen, `CK-06` | PO-023 | Siehe Modul 20 §7 | PL |
| `CQ-84` | Polizeireaktion auf Gewalt | Eskalation | PO-025 | Siehe Modul 20 §7 | PL |
| `CQ-85` | Öffentliches Wissen nach Monsterkämpfen | Geheimhaltung | PO-026 | A Selten; B Systemisch | PL |
| `CQ-86` | Festsetzen von Menschen | Recht | PO-027 | A Nicht möglich; B Möglich mit Folgen | PL |
| `CQ-87` | Animationsansatz | Ragdoll und Paaranimation | AN-013, -015 | Siehe Modul 22 §6 | PL nach Prototyp |
| `CQ-88` | Fluchtmechanik | Quellen schweigen | DE-009, -013 | Siehe Modul 21 §6 | PL |
| `CQ-89` | Soziale Deeskalation | Dialog im Konflikt | DE-014 | Siehe Modul 21 §6 | PL |
| `CQ-90` | Verhandlung mit Wesen | Mechanik | DE-004 | Siehe Modul 21 §6 | PL |
| `CQ-91` | Fortschritt ohne Kampf | Belohnung | DE-015 | A Gleichwertig; B Mit anderer Belohnung | PL |
| `CQ-92` | Trefferreaktionen | Quellen schweigen | AN-016 | Siehe Modul 22 §6 | PL nach Prototyp |
| `CQ-93` | Herkunft der Assets | Produktion | AN-017 | A Eigene; B Gekauft; C Generiert | PL |
| `CQ-94` | Verletzung in Ego- und Third Person | Darstellung | AN-018 | A Gleich; B Je Kamera | PL |
| `CQ-95` | Animations-LOD | Leistung | AN-019 | Siehe Modul 22 | PL nach Prototyp |
| `CQ-96` | Friendly Fire und PvP | Koop und Online | KO-008, -021 | A Aus im Koop; B Optional; C PvP nur Online-Modus | PL |
| `CQ-97` | Netzwerkarchitektur | Offene Architekturfrage | KO-004, -010, -016 | Siehe Modul 23 §6 | PL nach Prototyp |
| `CQ-98` | Gegnergedächtnis bei mehreren Spielern | Zielwahl | KO-017 | A Pro Spieler; B Gemeinsam; C Nach Wahrnehmung | PL |
| `CQ-99` | Verbindungsabbruch, Mods im Koop | Konsistenz | KO-009, -018 | A Figur sichern; B Figur bleibt; C Abbruch | PL |
| `CQ-100` | Speichersperre im Koop | Mehrere Spieler | KO-019 | A Für alle; B Für Entdeckte | PL |
| `CQ-101` | Schwierigkeit im Koop | Host oder Spieler | KO-020 | A Host; B Pro Spieler (Hilfen); C Gemischt | PL |
| `CQ-102` | Permadeath-Speicher | Löschung und Backup | SP-018 | Siehe Modul 24 §6 | PL |
| `CQ-103` | Zwischen-Checkpoints | Umfang | SP-005, -019 | Siehe Modul 24 §6 | PL nach Prototyp |
| `CQ-104` | Speicherbudget | Größenband (`CK-05`) | SP-002, -013, -017 | Siehe Modul 24 §6 | PL nach Messung |
| `CQ-105` | Replays im Spielstand | Diagnose | SP-015, -020 | Siehe Modul 24 §6 | PL |
| `CQ-106` | Kampfbudgets | Leistung | PF-003, -011, -013 | Siehe Modul 25 §6 | PL nach Messung |
| `CQ-107` | Kampf in Menschenmengen | Dichte | PF-008, -014 | Siehe Modul 25 §6 | PL nach Messung |
| `CQ-108` | Plattform- und Hardwareziele | Gameplay-Systeme §14.1 offen | PF-012 | A Aus Profilierung; B Vorgabe | PL |
| `CQ-109` | Ferne Kämpfe | Fortschreiben | PF-015 | Siehe Modul 25 §6 | PL |

---

## 4. Als beantwortet geführt

Diese Punkte stehen im Auftrag als mögliche offene Fragen. Die Quellen beantworten sie bereits; sie sind daher **nicht** erneut offen (Details in den genannten Zeilen).

| Frage aus dem Auftrag | Antwort der Quellen | Register |
|---|---|---|
| Soll es Plot-Rüstung geben? | Nein, für Spieler und menschliche NPCs (Spannung zu geschützten Figuren: `CK-02`) | `GP-007` |
| Gibt es sichtbare Level oder Gegnerskalierung? | Nein | `GP-003` |
| Springen auf Kreuz? | Ja (22.10.2026) | `ST-009` |
| Nahkampf: Lock-on als Standard? | Nein, weiche Hilfe optional | `NK-006` |
| Rollen-Spam als Verteidigung? | Nein | `DF-006` |
| Globaler Munitionspool? | Nein, Magazine mit tatsächlicher Schusszahl | `MU-006` |
| Ammo-HUD dazwischen? | Nein, nur an oder aus | `FK-012` |
| Ballistik: Coriolis? | Nein | `FK-021` |
| Künstliche Maximalreichweite? | Nein | `FK-014` |
| Schwierigkeit und Anatomie? | Anatomie bleibt, Verhalten und Folgen ändern sich | `GP-014` |
| Barrierefreiheit und Schwierigkeit? | Getrennte Ebenen | `GP-016` |
| Festnahme als Game Over? | Nein | `PO-009` |
| Host-Spielstand im Koop? | Maßgeblich | `KO-002` |
| Permadeath als Standard? | Nein, aktivierbar | `BW-013` |
| Wiederkehrende Gegner? | Ja, mit Erinnerung und Narben | `NM-001`, `-004` |
| Koop-Netzwerkarchitektur? | Offen (nicht beantwortet) | `KO-004` |

---

## 5. Zuordnung zu den Pflichtthemen des Auftrags

Auftrag §8 nennt zwölf Themen, die besonders sorgfältig als offene Fragen zu führen sind:

| Thema des Auftrags | Fragen |
|---|---|
| Realismusgrad unterschiedlicher Kampfmechaniken | `CQ-04`, `CQ-19`, `CQ-27`, `CQ-43` |
| Genaue Eingabebelegung | `CQ-01`, `CQ-24`, `CQ-26` |
| Verhalten kontextabhängiger Tasten | `CQ-25`, `CQ-02` |
| Ausdauer- und Ressourcenmechaniken | `CQ-42`, `CQ-43`, `CQ-44`, `CQ-38` |
| Schaden und Verletzungsschwere | `CQ-45`, `CQ-46`, `CQ-51`, `CQ-54` |
| Nichttödliche Kampfoptionen | `CQ-14`, `CQ-71`, `CQ-86` |
| Unterschiede zwischen Menschen und übernatürlichen Wesen | `CQ-63`, `CQ-64`, `CQ-65` |
| Balance zwischen Feuerwaffen und Nahkampf | `CQ-17`, `CQ-19`, `CQ-27`, `CQ-36` (keine Balancewerte beschlossen) |
| Konsequenzen für Polizei und Öffentlichkeit | `CQ-81` bis `CQ-85` |
| Koop-Synchronisierung | `CQ-96` bis `CQ-101` |
| Persistenzumfang | `CQ-102` bis `CQ-105` |
| Performance in dicht bevölkerten Gebieten | `CQ-106` bis `CQ-109` |

---

## 6. Vorschlag für die Reihenfolge der Klärung

> **💡 VORSCHLAG.** Die Reihenfolge ordnet nach Abhängigkeiten; sie ist keine Entscheidung.

1. **Grundlagen, von denen alles abhängt:** `CQ-07` (Todesloop, `CK-01`), `CQ-02` und `CQ-01` (Steuerungsphilosophie und Belegung), `CQ-79` (Tötung wichtiger Figuren, `CK-02`).
2. **Messbare Prototyp-Fragen:** `CQ-19`, `CQ-27`, `CQ-45`, `CQ-46`, `CQ-73`, `CQ-75`, `CQ-87`, `CQ-97`, `CQ-106` (hier genügt zuerst ein kleiner Prototyp).
3. **Inhalt und Umfang:** `CQ-11`, `CQ-12`, `CQ-33`, `CQ-64`, `CQ-17`.
4. **Recht und Rating:** `CQ-47`, `CQ-81`, `CQ-83`, `CQ-33` (Markenfrage), vor Veröffentlichung mit Fachberatung.
5. **Weltintegration:** `CQ-51`, `CQ-52`, `CQ-58`, `CQ-104`.
