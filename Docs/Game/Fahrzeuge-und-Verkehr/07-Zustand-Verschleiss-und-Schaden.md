# Fahrzeuge und Verkehr 07: Zustand, Verschleiß und Schaden

> [!IMPORTANT]
> **Der Detailgrad des Schadensmodells ist unbeantwortet.** Das Gespräch vom 15.10.2026 endete bei der Frage nach Verformung, Antriebs- und Reifenfehlern, Reparaturökonomie und bleibendem Verschleiß (`W-FZ-013`). Beschlossen sind die Verschleißteile, mechanische Schäden zusätzlich zur Verformung und die Verzahnung mit dem Verletzungssystem. Alles darüber hinaus ist Vorschlag, und die Optionen für den Detailgrad stehen nebeneinander, ohne dass eine gewählt wird. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Spannung in den Quellen](#2-spannung-in-den-quellen)
3. [Dreischichtiges Zustandsmodell](#3-dreischichtiges-zustandsmodell)
4. [Sichtbarer und verborgener Zustand](#4-sichtbarer-und-verborgener-zustand)
5. [Optionen für den Detailgrad des Schadens](#5-optionen-für-den-detailgrad-des-schadens)
6. [Schnittstellen](#6-schnittstellen)
7. [Register](#7-register)
8. [Offene Punkte](#8-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Verschleißteile | Motor, Öl, Kühlung, Getriebe, Kupplung, Differenzial, Batterie, Bremsen, Reifen, Federung; Verschleiß wird zumindest teilweise real simuliert | `W-FZ-008`; `23#1025E` Nr. 9 |
| Mechanischer Schaden | Mechanische Schäden treten zusätzlich zur visuellen Deformation auf; auch die Leistung kann sich ändern, etwa nach grober Fehlschaltung | `23#1025E` Nr. 15 |
| Gebrauchtmarkt | Laufleistung, Historie, Verschleiß, versteckte Mängel, Modifikationen | `W-FZ-003` |
| Unfallenergie | Crash-Energie und Sicherheitsausrüstung verzahnen sich mit dem Verletzungssystem | `W-FZ-015` |
| Ballistik | Fahrzeuge haben unterschiedliche ballistische Strukturen (Türblech ist nicht Motorblock); Glas kann brechen und ablenken | `COMBAT-DEC-FK-017` |
| Persistenz | Fahrzeugzustand ist persistent (🧭) | `W-FZ-016` |
| Offen | Schadensmodell in seinem Detailgrad; Schießen aus und auf Fahrzeuge | `W-FZ-013`, `COMBAT-DEC-FK-032` |

---

## 2. Spannung in den Quellen

Die Antwort des Entwicklers auf die Frage 15 des Fahrzeugblocks bejaht mechanische Schäden **zusätzlich** zur visuellen Deformation (`23#1025E` Nr. 15). Der Phasenvertrag vom 15.10.2026 führt dagegen das realistische Schadensmodell mit Verformung, Antriebs- und Reifenfehlern, Reparaturökonomie und bleibendem Verschleiß als **nicht gewählt** (`D15-RV`, `W-FZ-013`).

Beide Aussagen lassen sich so lesen, dass das **Ob** beschlossen und das **Wie tief** offen ist. Ob das die Absicht war, sagen die Quellen nicht. Die Dokumentation entscheidet das nicht; sie führt es als `CK-06`.

---

## 3. Dreischichtiges Zustandsmodell

> **💡 VORSCHLAG.** Eine Trennung nach Ursache, damit Verschleiß, Defekte und Schäden nicht in einer Zahl verschmelzen. Das Modell ist unabhängig vom noch offenen Detailgrad.

| Schicht | Ursache | Zeitverlauf | Beispiele (aus den beschlossenen Teilen) |
|---|---|---|---|
| **Verschleiß** | Nutzung und Zeit | kontinuierlich, langsam | Reifen, Bremsen, Öl, Kupplung, Batterie |
| **Defekt** | Überlastung, Fehlbedienung oder Zufall in Verbindung mit Verschleiß | diskret, kann plötzlich auftreten | Reifenpanne, Überhitzung (Pannen, `W-TV-001`), Kupplungsausfall nach grober Fehlschaltung |
| **Schaden** | Ereignis: Kollision, Beschuss, Umwelt | diskret | Verformung, gebrochenes Glas, beschädigter Antriebsstrang |

Jede Schicht kann den Antriebsstrang beeinflussen (Modul 04). Die Reparatur (Modul 08) behandelt jede Schicht anders: Verschleiß wird durch Wartung begrenzt oder zurückgesetzt, Defekte durch Austausch behoben, Schäden durch Instandsetzung.

**Integration statt Pro-Frame-Simulation.** Verschleiß lässt sich aus Nutzungsereignissen (Strecke, Gangwahl, Last, Temperatur) und Zeit über Intervalle fortschreiben. Das hält die Kosten niedrig und erlaubt dem Speicherstand, nur den Zustand zu halten (Modul 22 und 27).

---

## 4. Sichtbarer und verborgener Zustand

Ein Gebrauchtmarkt mit versteckten Mängeln (`W-FZ-003`) verlangt, dass der Zustand eines Fahrzeugs **mehr enthält, als der Spieler sieht**.

> **💡 VORSCHLAG.**

- Der Spieler erfährt den Zustand über **Symptome** (Geräusch, Fahrverhalten, Warnleuchten, Rauch) und über **Prüfung** (Werkstatt, Gutachten, Probefahrt), nicht über Zustandszahlen. Das passt zum Grundsatz, innere Werte nicht als Balken zu zeigen (`N-LT-012` für Beziehungen; für Fahrzeuge ist die Übertragung ein Vorschlag).
- Ein verborgener Mangel hat einen Auslöser (Alter, Laufleistung, Belastung) und eine Entdeckungsart. Wie viel davon deterministisch aus dem Fahrzeugdatensatz folgt und wie viel Zufall ist, ist offen (`CQ-24`).
- Die Historie des Fahrzeugs (Unfälle, Reparaturen, Besitzerwechsel) wird als Ereignisliste geführt (Modul 03, Feld `history_events`).

---

## 5. Optionen für den Detailgrad des Schadens

> **💡 VORSCHLAG.** Die Optionen sind ein Entscheidungshilfsmittel für `CQ-23`. Keine ist beschlossen.

| Option | Inhalt | Vorteil | Nachteil |
|---|---|---|---|
| A: Stufen | Wenige Schadensstufen pro Zone, Wirkung auf Leistung und Aussehen, kein Verformungsmodell | geringer Aufwand, leicht zu speichern und zu synchronisieren | wenig Eindruck von Wucht, begrenzt die Bedeutung der Ballistik |
| B: Zonen mit Teilen | Schaden je Zone und je Bauteil (Antriebsstrang, Reifen, Kühlung, Glas), sichtbare Stufen, kein volles Verformungsmodell | deckt `23#1025E` Nr. 15 (mechanisch zusätzlich zur Verformung) ab, speicherbar | Aufwand für Teile und Zonen, Balancing |
| C: Verformung | physikalische Verformung der Karosserie plus mechanische Schäden | größte Wirkung | hoher Aufwand, Netzwerk und Speicherbarkeit unklar, Rechenzeit |
| F: Freitext | Eigene Beschreibung | – | Inhalt hat Vorrang vor A bis C |

Die Wahl wirkt auf Modul 08 (Reparaturaufwand), Modul 16 (Unfälle), Modul 22 (Speicherformat), Modul 23 (Replikation) und Modul 27 (Leistung). Sie sollte nach einem Prototyp zu Kollision und Rechenzeit getroffen werden, nicht davor.

---

## 6. Schnittstellen

| Partner | Schnittstelle |
|---|---|
| [Modul 04](04-Fahrphysik-und-Fahrverhalten.md) | Zustand ändert Fahrverhalten (Leistung, Ziehen, Haftung) |
| [Modul 08](08-Reparatur-Wartung-und-Werkstaetten.md) | Reparatur setzt Zustand zurück; Verschleißteile |
| [Modul 16](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md) | Unfälle und Pannen erzeugen Schäden und Defekte |
| [Kampfsystem 05](../Kampfsystem/05-Fernkampf-und-Schusswaffen.md) | Beschuss; Ballistik der Fahrzeugstruktur |
| [Kampfsystem 13](../Kampfsystem/13-Verletzungen-Schmerz-und-Kampffolgen.md) | Insassenverletzung bei Unfall |
| [Modul 22](22-Persistenz-und-Lebenszyklen.md) | Zustand speichern; Wrack-Lebenszyklus |

---

## 7. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-ZV-001 | Verschleißteile: Motor, Öl, Kühlung, Getriebe, Kupplung, Differenzial, Batterie, Bremsen, Reifen, Federung; Verschleiß wird zumindest teilweise real simuliert | 📋 BESCHLOSSEN | `W-FZ-008`; `23#1025E` Nr. 9 | FZ | Tiefe je Teil offen (`CQ-25`) |
| VEH-DEC-ZV-002 | Mechanische Schäden treten zusätzlich zur visuellen Deformation auf; Leistung kann sich ändern | 📋 BESCHLOSSEN | `23#1025E` Nr. 15 | FZ | Detailgrad: `VEH-DEC-ZV-003` |
| VEH-DEC-ZV-003 | Detailgrad des Schadensmodells (Verformung, Antriebs- und Reifenfehler, Reparaturökonomie, bleibender Verschleiß) ist nicht gewählt | ❓ OFFEN | `W-FZ-013`; `D15-RV` | FZ | `CQ-23` |
| VEH-DEC-ZV-004 | Die Antwort `23#1025E` Nr. 15 (Schäden ja) und der Vermerk ‚nicht gewählt‘ im Phasenvertrag lassen sich als ‚Ob beschlossen, Wie tief offen‘ lesen; ob das gemeint war, ist nicht belegt | ⚠️ KONFLIKT | `23#1025E` Nr. 15; `D15-RV`; `W-FZ-013` | FZ | `CK-06` |
| VEH-DEC-ZV-005 | Gebrauchtfahrzeuge haben Laufleistung, Historie, Verschleiß, versteckte Mängel und Modifikationen | 📋 BESCHLOSSEN | `W-FZ-003` | FZ WIR | Verborgener Zustand: 💡 `VEH-DEC-ZV-009` |
| VEH-DEC-ZV-006 | Fahrzeuge haben unterschiedliche ballistische Strukturen; Glas kann brechen und ablenken | 📋 BESCHLOSSEN | `COMBAT-DEC-FK-017` | FZ KMP | – |
| VEH-DEC-ZV-007 | Schießen aus und auf Fahrzeuge (Reifen, Motor, Fahrer, Innenraum) ist als Kampfmechanik nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-FK-032` | KMP FZ | Frage liegt im Kampfsystem |
| VEH-DEC-ZV-008 | Der Fahrzeugzustand wird in drei Schichten geführt: Verschleiß (kontinuierlich), Defekt (diskret, überlastungs- oder zufallsbedingt), Schaden (ereignisbedingt) | 💡 VORSCHLAG | Ableitung aus `W-FZ-008`, `W-TV-001`, `23#1025E` Nr. 15 | FZ | – |
| VEH-DEC-ZV-009 | Der Zustand hat sichtbare und verborgene Anteile; der Spieler erfährt ihn über Symptome und Prüfung statt über Zahlen | 💡 VORSCHLAG | Ableitung aus `W-FZ-003` | FZ UI | Zufallsanteil: `CQ-24` |
| VEH-DEC-ZV-010 | Verschleiß wird aus Nutzungsereignissen und Zeit über Intervalle fortgeschrieben, nicht pro Bild simuliert | 💡 VORSCHLAG | Ableitung aus `N-LT-011`; Leistung Modul 27 | FZ PF | – |
| VEH-DEC-ZV-011 | Schaden wird je Zone und Bauteil geführt (Front, Heck, Seiten, Dach, Unterboden, Glas, Reifen, Antriebsstrang); Zonenzahl ist nicht beschlossen | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-FK-017` | FZ KMP | Teil von `CQ-23` |
| VEH-DEC-ZV-012 | Optionen für den Schadensdetailgrad: A Stufen, B Zonen mit Teilen, C Verformung, F Freitext; keine ist gewählt | 💡 VORSCHLAG | – | FZ | `CQ-23` |
| VEH-DEC-ZV-013 | Reparaturökonomie und bleibender mechanischer Verschleiß sind nicht gewählt | ❓ OFFEN | `D15-RV`; `W-FZ-014` | FZ WIR | `CQ-26` |
| VEH-DEC-ZV-014 | Totalschaden, Wrack, Verschrottung, Brand und Explosion von Fahrzeugen sind in den Quellen nicht behandelt | 🔍 UNBELEGT | – | FZ PER | `CQ-27`; Wrack-Lebenszyklus: [Modul 22](22-Persistenz-und-Lebenszyklen.md) |
| VEH-DEC-ZV-015 | Wartungsintervalle und Verschleißraten haben keine Quelle und werden hier nicht festgelegt | 🔍 UNBELEGT | – | FZ | `CQ-25` |
| VEH-DEC-ZV-016 | Sicherheits- und Assistenzsysteme (ABS, ESC, Traktionskontrolle, Airbags, Kollisionsvermeidung, Spurhalteassistent, Notbremsassistent) unterscheiden sich nach Fahrzeuggeneration und Ausstattung und sind dort abschaltbar, wo es realistisch ist, besonders im Rennsport; alte und moderne Fahrzeuge verhalten sich unterschiedlich | 📋 BESCHLOSSEN | `M-DC31` §36; `M-PS31` §50 | FZ MED | Wirkung auf Verletzung: `W-FZ-015` |

---

## 8. Offene Punkte

| Frage | Verweis |
|---|---|
| Detailgrad des Schadensmodells | `CQ-23` |
| Zufallsanteil bei verborgenen Mängeln | `CQ-24` |
| Tiefe und Verschleißraten je Teil | `CQ-25` |
| Reparaturökonomie und bleibender Verschleiß | `CQ-26` |
| Totalschaden, Wrack, Brand | `CQ-27` |
| Spannung ‚Ob beschlossen, Wie offen‘ | `CK-06` |
