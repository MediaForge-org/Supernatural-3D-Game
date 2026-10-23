# Ermittlung 26: Notfälle und Weltereignisse

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die **dynamischen Ereignisse der Welt** (Unfälle, Brände, Stürme, Veranstaltungen, Krisen) und die **Einsatzketten** von Polizei, Feuerwehr und Rettung, soweit sie Ermittlung, Fälle und Spuren berühren. Beschlossen ist die Richtung (Ereignisstufen, Folgen, Reparatur, Nachrichten). Alles Konkrete ist **Entwurf** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Verwandt: [Modul 07 §6](07-Detektivlaufbahn-und-organische-Faelle.md#6-dynamische-weltereignisse-und-soziale-medien) (Stufen), [Modul 09](09-Falllebenszyklus-und-Generierung.md) (Fälle), [Modul 25](25-Kriminalitaet-in-der-Welt.md). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Ereignisfamilien und Stufen](#2-entwurf-ereignisfamilien-und-stufen)
3. [Entwurf: Lebenszyklus eines Ereignisses](#3-entwurf-lebenszyklus-eines-ereignisses)
4. [Entwurf: Einsatzketten](#4-entwurf-einsatzketten)
5. [Entwurf: Folgen, Schäden und Reparatur](#5-entwurf-folgen-schäden-und-reparatur)
6. [Entwurf: Menschenmengen und Veranstaltungen](#6-entwurf-menschenmengen-und-veranstaltungen)
7. [Entwurf: Rettung in der Wildnis](#7-entwurf-rettung-in-der-wildnis)
8. [Entwurf: Berichterstattung und Wirtschaft](#8-entwurf-berichterstattung-und-wirtschaft)
9. [Entwurf: Ereignisse als Quellen von Spuren und Fällen](#9-entwurf-ereignisse-als-quellen-von-spuren-und-fällen)
10. [Entwurf: Der Spieler im Ereignis](#10-entwurf-der-spieler-im-ereignis)
11. [Übernatürliche Ereignisse](#11-übernatürliche-ereignisse)
12. [Sonderfälle](#12-sonderfälle)
13. [Abhängigkeiten](#13-abhängigkeiten)
14. [Ergänzung: Aktivitäten und Hinweise](#14-ergänzung-aktivitäten-und-hinweise)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

Fragenummern beziehen sich auf `23#1056A` (Block 8), Antworten auf `23#1057E`.

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| N1 | Feuerwehr reagiert auf Fahrzeugbrände, Gebäudebrände, Unfälle | 📋 | Frage 70, „ja sicher“ |
| N2 | Krankenwagen holen verletzte NPCs ab; Verletzte werden in Krankenhäuser gebracht | 📋 | Fragen 71–72 |
| N3 | Verkehrsunfälle: Polizei, Feuerwehr, EMS und Abschleppdienst gemeinsam | 📋 | Frage 73; `23#1058A` (Beispielkette mit Notruf, Absperrung, Reparatur) |
| N4 | Bergrettung (Mountain Region mit Ski-, Kletter-, Lawinenunfällen) | 📋 | Frage 74 |
| N5 | Wasserrettung (Küste, See) | 📋 | Frage 75 |
| N6 | Suchaktionen (Vermisste Wanderer können auch normale Ereignisse sein) | 📋 | Frage 76; `23#1058A` |
| N7 | Kleine Ereignisse: Straßensperren, liegen gebliebene Autos, Streit, verlorene Tiere, kaputte Ampeln | 📋 | Frage 77 |
| N8 | Mittlere Ereignisse: Unfälle, kleine Brände, Stromausfälle, Proteste, Sportveranstaltungen, lokale Feste | 📋 | Frage 78 |
| N9 | Große Ereignisse: schwere Stürme, Überschwemmungen, Großbrände, große Unfälle, wichtige Sportevents | 📋 | Frage 79 |
| N10 | Extrem seltene Ereignisse: Tornado, massiver Blizzard, großer Blackout | 📋 | Frage 80 |
| N11 | Ereignisse haben echte Konsequenzen; die Stadt kann Schäden davontragen | 📋 | Frage 80, Antwort „ja, die stadt kann auch schäden davontragen“ (Nummern der Frage und der Antwort sind laut Liste um eine Zeile versetzt; die Aussage gilt für Ereigniskonsequenzen) |
| N12 | Straßen und Gebäude werden von NPCs repariert; der Spieler kann Hilfe anbieten | 📋 | Frage 81 |
| N13 | Die Welt kehrt nicht nach 30 Sekunden zurück; Reparaturen dauern; schwere Ereignisse bleiben Wochen oder Monate sichtbar | 📋 | Fragen 82–83 |
| N14 | Radio, TV, Websites und Zeitung berichten über Ereignisse | 📋 | Frage 84 |
| N15 | Passende Ereignisse beeinflussen Aktien und Unternehmen | 📋 | Frage 85 |
| N16 | Große Menschenmengen (Stadien, Festivals, Freizeitparks, Downtown); vereinfachte Simulation in der Ferne, detailliert in der Nähe | 📋 | Fragen 86–87, 86 „ja das ist sehr wichtig“ |
| N17 | Menschenströme folgen Ereignissen (nach einem Spiel zu Parkplatz, Metro, Bars); Verkehr und ÖPNV reagieren auf Großveranstaltungen | 📋 | Fragen 88–89 |
| N18 | NPC-Fahrer unterscheiden sich; Wetter beeinflusst; selten Fehler; Betrunkene Fahrer „sehr selten“; Verkehr macht Rettungsfahrzeugen Platz | 📋 | Fragen 93–97 |
| N19 | Große übernatürliche Ereignisse können temporär ganze Stadtbereiche beeinflussen, bleiben aber selten | 📋 | Frage 120, „115-120“-Gruppe (Einzelantwort `23#1057E`: „120. ja genau“) |
| N20 | Die Gesellschaft reagiert dynamisch auf übernatürliche Vorfälle (Behörden, Medien, Politik, Bevölkerung, Wirtschaft) | 📋 | `48#145E` |
| N21 | Dynamische Weltereignisse sind aus sozialen Medien erreichbar, aber sie sind keine Nebenmissionen | 📋 | `M-PD`; `M-B40` |

> [!NOTE]
> Zur Zählung der Antworten: In `23#1057E` sind die Antworten nummeriert; bei einigen Nummern lässt sich nicht sicher erkennen, ob eine Zeile zu der Frage derselben Nummer gehört. Die Zusammenfassung dieses Moduls stützt sich auf **Sinn und Reihenfolge**, nicht auf eine Zeile pro Nummer. Wo ein Unterschied stören könnte (N11), ist das ausdrücklich vermerkt. Für die Aussagen selbst gibt es keine Gegenaussage.

---

## 2. Entwurf: Ereignisfamilien und Stufen

> [!NOTE]
> **Nur Vorschlag (💡).** Die vier **Stufen** sind beschlossen (N7 bis N10); die **Familien** sind eine Ordnung der Beispiele aus den Quellen.

| Familie (Arbeitsname) | Beispiele aus den Quellen | Stufen |
|---|---|---|
| **Verkehr** | liegen gebliebene Autos, Unfälle, große Unfälle, Straßensperren | klein bis groß |
| **Technik und Versorgung** | kaputte Ampeln, Stromausfälle, großer Blackout | klein bis extrem |
| **Feuer** | kleine Brände, Großbrände, Fahrzeugbrände | mittel bis groß |
| **Wetter und Natur** | schwere Stürme, Überschwemmungen, Tornado, Blizzard, Lawinen | groß bis extrem |
| **Gesellschaft** | Streit, Proteste, Sportveranstaltungen, lokale Feste, Großveranstaltungen | klein bis groß |
| **Rettung und Suche** | Bergunfälle, Wasserunfälle, vermisste Wanderer | mittel |
| **Kriminalität** | siehe [Modul 25](25-Kriminalitaet-in-der-Welt.md) | klein bis groß |
| **Übernatürlich** | große Ereignisse mit Wirkung auf Stadtbereiche (N19) | selten |
| **Tiere** | verlorene Tiere (N7) | klein |

**Stufe** bestimmt (Entwurf): Reichweite (wie viele Bereiche), Dauer, Zahl der Einsatzkräfte, Auswirkung auf Verkehr und Wirtschaft, Auffälligkeit in Medien, Dauer der Folgen. Konkrete Werte sind offen (Q-E75).

---

## 3. Entwurf: Lebenszyklus eines Ereignisses

```text
[Auslöser] -> [Entstehung] -> [Entdeckung] -> [Einsatz] -> [Höhepunkt] -> [Abklingen]
                                                                              |
                                      [Aufräumen und Reparatur] -> [Nachwirkung/Gedächtnis]
```

| Phase | Inhalt (Vorschlag) |
|---|---|
| **Auslöser** | Zustand der Welt (Wetter, Zufall, Täter, Technik, Veranstaltungskalender) |
| **Entstehung** | Ereignis tritt ein; Zustand wird gesetzt |
| **Entdeckung** | Wahrnehmung durch NPCs, Systeme, Spieler; Meldung (Latenz) |
| **Einsatz** | Polizei, Feuerwehr, Rettung, Abschleppdienst (Abschnitt 4) |
| **Höhepunkt** | Zeitraum der größten Auswirkung |
| **Abklingen** | Ereignis endet |
| **Aufräumen und Reparatur** | Dauer (N13); NPCs führen aus (N12) |
| **Nachwirkung** | Gerüchte, Nachrichten, Ängste, Beziehungen, Wirtschaft, gegebenenfalls Fälle (Abschnitt 9) |

Ereignisse sind **Zustände der Welt** mit Beginn, Ort, Stufe, Phase und Dauer. Ihre Fortschreibung bei Zeitsprüngen folgt den Regeln aus [Modul 19 §8](19-Weltzustand-und-Persistenz.md#8-entwurf-zeitsprünge-und-fortschreibung).

---

## 4. Entwurf: Einsatzketten

`23#1058A` zeigt eine Beispielkette für einen schweren Verkehrsunfall: Notruf, Polizei, Feuerwehr, Krankenwagen, Straßensperre, Abschleppdienst, Reparatur und Aufräumen. Der Entwurf verallgemeinert sie:

| Schritt | Beteiligte | Hinweis für die Ermittlung |
|---|---|---|
| **Meldung** | Zeugen, Systeme (911) | Latenz (Modul 14 §4); Qualität der Meldung |
| **Erste Einsatzkräfte** | nächste Einheit | Reaktionszeit hängt von Dichte und Entfernung (offen) |
| **Rettung** | EMS, Feuerwehr | Verletzte werden transportiert (N2); Zustand der Verletzten wird dokumentiert |
| **Sicherung** | Polizei | Absperrung (Modul 25 §4); Zugang beschränkt |
| **Spurensicherung** | Polizei/Gutachter | Beweisaufnahme (Modul 11); Spuren verändern sich |
| **Verkehr** | Verkehr macht Platz, Umleitung | Rettungsgasse (Frage 97) |
| **Räumung** | Abschleppdienst, Reinigung | Spuren verschwinden |
| **Bericht** | Polizei, Medien | Akte (Modul 14 §5); Nachrichten (Abschnitt 8) |

Alle Rollen sind **NPCs** (Tier A bis C) mit Persönlichkeit und Gedächtnis. Einsatzkräfte, die dem Spieler öfter begegnen, kennen ihn (Modul 12).

---

## 5. Entwurf: Folgen, Schäden und Reparatur

Beschlossen: Konsequenzen sind echt; Schäden sind möglich; Reparatur dauert; schwere Schäden bleiben Wochen und Monate (N11–N13).

| Folge (Katalog) | Dauer-Kategorie (Entwurf, keine Werte) |
|---|---|
| Sperrungen von Straßen | Stunden bis Tage |
| Beschädigte Gebäude | Tage bis Monate |
| Stromausfall | Stunden bis Tage |
| Verletzte und Tote | dauerhaft (NPC-Tod ist permanent, Frage 102) |
| Wirtschaftliche Folgen | Tage bis Jahre |
| Gerüchte und Angst | Tage bis Wochen |
| Spuren | verschwinden durch Räumung und Wetter |
| Gesellschaftliche Veränderung | Jahre (Modul 25 §9) |

**Reparatur** wird von NPCs ausgeführt (N12). Es ist nicht festgelegt, **wer** die Aufträge vergibt (Stadt, Firmen), wie sie bezahlt werden und ob der Spieler sie auslösen kann. Er kann nach N12 „Hilfe anbieten“. Das System braucht Reparaturaufträge als **Weltzustand** mit Beginn, Dauer und Verantwortlichem (Entwurf).

**Persistente Tatorte.** Blut und Schäden bleiben bestehen, bis sie gereinigt oder repariert sind (`23#1074A` Frage 150, „149-150 ja“). Das ist die Verbindung zwischen Ereignissen und Spuren (Modul 05 §8).

---

## 6. Entwurf: Menschenmengen und Veranstaltungen

Beschlossen (N16, N17): Große Menschenmengen wirken dicht; in der Ferne vereinfachte Simulation, in Spielnähe detaillierte NPCs; Ströme folgen Ereignissen; Verkehr und ÖPNV reagieren. Block 8J nennt zusätzlich Clubs um drei Uhr nachts, Freizeitparks mit Warteschlangen, Skigebiete mit Besucherfluss (Fragen 90–92, Antworten „ja“).

| Aspekt | Bedeutung für die Ermittlung (Entwurf) |
|---|---|
| **Viele Zeugen, schlechte Zeugen** | In Menschenmengen gibt es viele Beobachter mit geringer Aufmerksamkeit (Modul 12 §4) |
| **Kameradichte** | Stadien und Festivals haben mehr Kameras (offen) |
| **Spurenverlust** | viele Menschen zerstören Spuren |
| **Tarnung** | Täter können in der Menge untertauchen |
| **Polizeipräsenz** | höher bei Großveranstaltungen (Modul 14 §6) |
| **Medien** | Aufnahmen, Livestreams (Modul 27) |
| **Fälle in Menschenmengen** | Vermisste, Taschendiebstahl, Unfälle |

Die Menschenmenge ist **Stufe D** der NPC-Simulation (billig simuliert, `23#1056A` Frage 121); Zeugen aus einer Menge müssen bei Bedarf **materialisiert** werden (Modul 12 §8, Q-E50).

---

## 7. Entwurf: Rettung in der Wildnis

Beschlossen (N4–N6): Bergrettung, Wasserrettung, Suchaktionen. Vermisste Wanderer müssen nicht übernatürlich sein.

| Aspekt | Entwurf |
|---|---|
| **Suche** | Rettungskräfte, Hunde (Spurensuche, Modul 05 §4), Hubschrauber; Spurenlage (Wetter, Schnee) |
| **Fälle** | Ein Vermisstenfall kann gewöhnlich, übernatürlich oder mehrdeutig sein (Modul 02) |
| **Spieler** | kann an der Suche teilnehmen, sie anführen, ablehnen oder aufklären |
| **Zeit** | Suchaktionen laufen in Spielzeit; Überlebenschance sinkt (Entwurf) |
| **Zuständigkeit** | Rangers, Sheriff, Bergrettung (Modul 14 §3) |
| **Folgen** | Gefundene Tote, Überlebende, Gerüchte |

---

## 8. Entwurf: Berichterstattung und Wirtschaft

Beschlossen: Ereignisse werden in Radio, TV, Websites, Zeitung berichtet (N14); sie beeinflussen Aktien (N15). Für die Ermittlung sind die Berichte **Belege** (Zeitungsartikel, Polizeiberichte, Aufnahmen, `M-PD`) und **Signale** für Fälle (Modul 09 §7).

| Aussage | Entwurf |
|---|---|
| **Berichte sind Quellen** | Sie haben Zuverlässigkeit und Interessen (Modul 10 §6) |
| **Berichte verzerren** | Sie folgen Gerüchten und Behörden; Vertuschung (Modul 17) |
| **Verzögerung** | Berichte erscheinen nach dem Ereignis (Latenz) |
| **Archiv** | Zeitungen und Archive sind dauerhafte Quellen (`23#1075E` Punkt 10: analoge Quellen bleiben wichtig) |
| **Wirtschaft** | Ereignisse bewegen Aktien (N15); Unternehmensnachrichten sind eine weitere Quelle |

---

## 9. Entwurf: Ereignisse als Quellen von Spuren und Fällen

| Verbindung | Beschreibung |
|---|---|
| **Signal** | Ein Ereignis ist ein Signal eines Vorfalls (Unfall, Brand, Verschwinden) und kann zum Muster gehören (Modul 09 §3–7) |
| **Tarnung** | Ein Ereignis kann eine Tat verdecken (Brand nach Mord) |
| **Spuren** | Ereignisse hinterlassen und zerstören Spuren |
| **Zeugen** | Ereignisse erzeugen viele Zeugen; sie bilden Gerüchte |
| **Zeitfenster** | Ereignisse legen Zeitfenster fest (Stromausfall, Veranstaltung) für Alibis (Modul 12 §8) |
| **Fälle durch Ereignisse** | Ein Ereignis eröffnet Fälle (Einbrüche beim Stromausfall) |
| **Zufallsfunde** | Der Spieler entdeckt bei einem Ereignis Hinweise auf alte Fälle (`23#59A` Fall D) |

Ereignisse dürfen **keine Pflichtfälle** sein (`M-PD`, `M-B40`: dynamische Weltereignisse sind keine Nebenmissionen).

---

## 10. Entwurf: Der Spieler im Ereignis

| Handlung | Entwurf |
|---|---|
| Beobachten | Zeuge werden (Modul 12) |
| Melden | Notruf; Qualität der Meldung beeinflusst Reaktion |
| Helfen | Erste Hilfe, Rettung, Hilfe bei Reparatur (N12); Verletzungssystem (Rettung nach Verletzungsart) |
| Ausnutzen | Diebstahl bei Stromausfall (Straftat) |
| Fliehen | aus der Gefahr |
| Untersuchen | Ereignis als Tatort (Modul 11) |
| Ignorieren | Welt läuft weiter |
| Kameras und Aufnahmen | Beweise erzeugen (Modul 27) |

Der Spieler **muss** nicht eingreifen (Oberste Regel, E-FA-019). Wer hilft, kann Ruf und Beziehungen gewinnen (Modul 13).

---

## 11. Übernatürliche Ereignisse

Beschlossen: Große übernatürliche Ereignisse können temporär ganze Stadtbereiche beeinflussen, sind aber selten (N19); Menschen meiden nach mehreren Vorfällen einen Ort (Frage 114); Gerüchte verbreiten sich (Frage 115); Tiere reagieren teils früher (Frage 119, bereits festgelegt); Polizei erklärt zunächst normal (Frage 116).

| Aspekt (Entwurf) | Beschreibung |
|---|---|
| **Sichtbarkeit** | Ereignis wirkt auf die Welt (Wetter, Strom, Tiere, Anomalien), ohne dass die Ursache bekannt ist |
| **Erklärung** | Behörden und Medien liefern normale Erklärungen (Naturereignis, Gasleck, Unfall) |
| **Stufe** | selten, hoch |
| **Zusammenhang mit Fällen** | Eine Häufung von Ereignissen ist ein Muster (Modul 09) |
| **Regionales Wissen** | Ereignisse verändern das öffentliche Wissen (Modul 17 §3) |
| **Wiederkehr** | Bestimmte Ereignisse hängen an Zeit, Mond, Jahreszeit (`23#294E` Punkt 19) |

---

## 12. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Ereignis am Ort eines aktiven Falls | – | Spuren überlagert; Fallwahrheit enthält das Ereignis (Modul 09) |
| Mehrere Ereignisse gleichzeitig | Sturm und Unfall | Einsatzkräfte überlastet; längere Reaktionszeit (Entwurf) |
| Wichtiger NPC im Ereignis | – | Schutz wichtiger NPCs (Punkt 105) gilt für Simulationsunfälle, nicht für Handlungen des Spielers |
| Ereignis im Zeitsprung | Haft, Krankenhaus | Ergebnis wird fortgeschrieben (Modul 19 §8); Spieler erfährt davon (Bericht) |
| Koop | Mitspieler an verschiedenen Orten | Ereignis in der Host-Welt; beide können beteiligt sein |
| Katastrophen-Ereignisse | Tornado, Blizzard, Blackout | selten (N10); Spielerwirkung ohne Fallbezug |
| Ereignis ohne Zeugen | – | Medien melden später oder nie |
| Mods | neue Ereignisse | Daten (Modul 19 §11) |

---

## 13. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Fallmodell ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) | Signale, Eskalation |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Beobachtungen, Gerüchte |
| Polizei ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) | Meldekette, Absperrung |
| Kriminalität ([Modul 25](25-Kriminalitaet-in-der-Welt.md)) | Ereignisfamilie |
| Medien ([Modul 27](27-Medien-und-digitale-Spuren.md)) | Berichte, Aufnahmen |
| Wetter und Zeit | Auslöser |
| Verkehr und Fahrverhalten (Welt 14, 16) | Unfälle, Rettungsgasse |
| Verletzungssystem | Rettung, Krankenhaus |
| Wirtschaft | Aktien, Reparatur |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Ereigniszustände |
| NPC-Simulation | Einsatzkräfte, Menschenmengen |

---

## 14. Ergänzung: Aktivitäten und Hinweise

Zu Block 7 fragte der Assistent, ob normale Aktivitäten gelegentlich organisch zu Gerüchten, Beobachtungen oder übernatürlichen Hinweisen führen dürfen, „damit nicht hinter jedem Billardtisch ein Monster steckt“ (`23#1032A`, Punkt 125). Der Entwickler antwortete: ja, aber das soll „nur zu aufträgen führen nicht nebenmissionen, weil diese auf der karte markiert sind, denke ich“ (`23#1035E`, 📋; die Begründung „denke ich“ ist eine Vermutung des Entwicklers). Außerdem bejahte er, dass NPCs Aktivitäten wirklich nutzen („sonst würde die welt nicht lebendig wirken“, `23#1035E` Punkt 124) und dass die Welt Aktivitäten dynamisch organisiert (`23#1035E` Punkt 123, „ja sicher“). Das ist die Quellenbasis für Hinweise aus Freizeit-Ereignissen: Aufträge ja, markierte Nebenmissionen nein.

---

## 15. Was fehlt
- **Zahlen**: Häufigkeit, Dauer, Reichweite je Stufe (Q-E75).
- **Einsatzdichte** und Reaktionszeiten.
- **Reparaturorganisation** (wer beauftragt, wer zahlt).
- **Ereigniskatalog** und Kalender.
- **Wetterereignisse** im Detail (Wetter, Klima: Welt 04, 18).
- **Darstellung** der Ereignisse für den Spieler (Detektivmodus ohne Hilfe).
