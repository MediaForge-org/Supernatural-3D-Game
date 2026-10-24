# Fahrzeuge und Verkehr 26: Simulationsstufen, LOD und Streaming

> [!IMPORTANT]
> **Es gibt keine Budgets, Radien, Fahrzeugzahlen oder Messwerte.** Beschlossen sind Prinzipien: Verkehr nutzt LOD und Abstraktion, die Straße ist beim Betreten nie leer, ferne NPCs sind Zustand statt Bewegung, nicht jeder NPC ist ein voller Actor, die Weltgröße ist nirgends fest verdrahtet. Die Stufen und Verfahren hier erweitern den **NPC-Entwurf** (Stufen S0 bis S3 und SD) auf Fahrzeuge; sie sind Vorschlag. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Grundlage: [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md), [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md), [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Stufen für Fahrzeuge](#2-stufen-für-fahrzeuge)
3. [Materialisierung und Rückschreiben](#3-materialisierung-und-rückschreiben)
4. [Streaming und Verkehr](#4-streaming-und-verkehr)
5. [Sonderfälle](#5-sonderfälle)
6. [Prüfregeln](#6-prüfregeln)
7. [Register](#7-register)
8. [Offene Punkte](#8-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| LOD | Crowd und Verkehr nutzen LOD und Abstraktion und rekonstruieren eine glaubwürdige lokale Bevölkerung beim Betreten eines Gebiets | `W-UR-011`; `W-TV-009` |
| Zustand statt Bewegung | Ferne NPCs werden als Zustand (`AT_WORK`, `HOME`, `TRAVEL`) geführt; beim Annähern entsteht ein konsistenter detaillierter NPC | `N-LE-013` |
| Nie leer | Bei Besuch einer Gegend darf die Straße nicht leer sein | `N-LE-026` |
| Nicht jeder Actor | Nicht jeder NPC läuft als voller Unreal-Actor; die Weltgröße (rund 2.200 km², Szenario) ist nirgends fest verdrahtet | `N-LT-007` |
| Ereignisbasiert | Hintergrundsimulation arbeitet ereignisbasiert; Reisen kostet Zeit; keine NPCs an zwei Orten | `N-SI-011` |
| Stufen | Mehrere Simulationsstufen sind vorgesehen; die Namen S0 bis S3 und SD sind Arbeitsnamen | `N-SI-008` (🧭) |
| Offen | Leistungsstufen der Offscreen-Simulation und deterministische Resynchronisation von Zügen, Verkehr und NPCs; Budgets | `D15-OQ` Punkt 9; `M-OQ16` Punkt 9; `W-TV-008` |
| Vorhanden | Konfigurierbare Distanz-zu-Stufe-Tabelle im Kern | `Core/include/spn/core/DetailTiers.h` |
| Streaming | World Partition ist das Streaming-Grundverfahren; Fahrzeug-Streaming als Quelle ist ein Entwurfsthema, keine Entscheidung | `M-PD`; [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §3, §10 |

---

## 2. Stufen für Fahrzeuge

> **💡 VORSCHLAG.** Die Zuordnung benutzt die Stufen des NPC-Entwurfs ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §2), damit ein Fahrzeug und sein Fahrer dieselbe Stufe teilen und nie in zwei Systemen unterschiedlich geführt werden.

| Stufe | Fahrzeug | Verhalten | Inhalt |
|---|---|---|---|
| **S0** | Spielerfahrzeug, Fahrzeuge in unmittelbarer Interaktion (Rammen, Beschuss, Einsteigen, Zeugen) | volle Fahrphysik, volle Verkehrs-KI (Taktik und Operation) | Modul 04, 13 |
| **S1** | Fahrzeuge im geladenen Bereich um den Spieler | vereinfachtes wegbasiertes Modell, Taktik in Daten, sichtbare Folgen (Abwürgen, Rutschen) | `VEH-DEC-PH-016` |
| **S2** | Fahrzeuge benachbarter, nicht geladener Regionen | Zustand je Fahrzeug (geparkt, TRAVEL mit erwarteter Ankunft, Werkstatt), Stau als Verzögerung je Kante | Modul 14, 15 |
| **S3** | Fahrzeuge in der übrigen Welt | Datensatz (ID, Seed, Overrides), Fahrten nur als Zeitplan | Modul 22 |
| **SD** | Verkehrsmenge ohne Identität | instanziert aus Straße, Zeit und Dichtekurve | Modul 14 §5 |

Distanzgrenzen kommen aus einer **konfigurierbaren Tabelle** nach dem Muster von `DetailTierTable` (Distanz in Zentimetern, geordnete Schwellen, Zahl der Stufen nicht beschlossen). Für Verkehr empfiehlt sich eine eigene Tabelle für die **Darstellung** und eine für die **Simulation**, weil beide unterschiedlich skalieren. Die Werte sind nicht belegt (`CQ-110`).

---

## 3. Materialisierung und Rückschreiben

> **💡 VORSCHLAG.**

**Materialisierung** (Gebiet wird geladen oder der Spieler nähert sich):

```text
Eingabe:  Kanten im Radius, Weltzeit, Dichtekurven, NPC-Zustände TRAVEL und geparkt, Sperrungen und Baustellen
Schritte: 1. persönlicher Verkehr: NPCs im Zustand TRAVEL, deren Route im Bereich liegt -> Fahrzeug auf der Kante,
             Position aus Startzeit und Dauer
          2. geparkte Fahrzeuge: aus Parkkapazität und Heimatstellplätzen (Modul 12)
          3. Verkehrsmenge: fehlende Dichte deterministisch aufgefüllt (Seed aus Kante und Zeitfenster)
          4. Kantenzustand: Stau, Baustellen, Unfallstellen als Objekte
Ausgabe:  eine Szene, die zur Weltzeit und zum Zustand passt
```

Reihenfolge und Zufall sind deterministisch (`DeterministicRandom`, `VEH-DEC-KI-010`). Wer dieselbe Kante im selben Zeitfenster wieder besucht, sieht dieselbe Verkehrslage, solange sich die Welt dort nicht verändert hat.

**Rückschreiben** (Fahrzeug verlässt den Bereich): Identität und Fahrstil bleiben; Position wird zu **Kante und Fortschritt**; geparkte Fahrzeuge bleiben bei ihrer Parkfläche; Schaden und Zustand werden ins Fahrzeug zurückgeschrieben. Beförderte Statisten (Modul 14) bleiben NPCs und werden S2 oder S3.

---

## 4. Streaming und Verkehr

Fahrzeuge dürfen nur dort physisch existieren, wo Boden und Kollision geladen sind.

> **💡 VORSCHLAG.**
>
> - **Verkehrsmaterialisierung folgt dem Streaming:** Fahrzeuge der Stufe S1 werden nur auf geladenen Zellen erzeugt und bewegt. Beim Verlassen einer Zelle gehen sie in S2 über (oder wandern in die nächste geladene Zelle).
> - **Vorlauf entlang der Route:** Fährt der Spieler schnell, müssen die Zellen **vor ihm** geladen sein, und der Verkehr entlang der Route wird vorab materialisiert (`NPC 22` §11: Streaming-Vorhersage, S1-Agenten entlang der Strecke). Die nötige Vorlaufstrecke hängt von Geschwindigkeit und Ladezeit ab; Werte sind nicht belegt.
> - **Sicherung:** Reicht der Vorlauf nicht (zu schnelle Fahrt, langsames Laden), braucht es eine definierte Reaktion. Optionen sind: Geschwindigkeit begrenzen, Verkehr dünner halten, Fahrzeug anhalten, bis geladen ist. Keine Option ist gewählt (`CQ-111`).
> - **Straßen als Streaming-Inhalt:** Straßen, Brücken und Tunnel sind Inhalte von World Partition ([Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md)). Zeitzustände (Baustellen, Sperrungen) können als Laufzeitobjekte oder über Data Layers abgebildet werden; die Wahl ist offen (`CQ-113`).
> - **Mehrere Quellen:** Jeder Spieler (im Koop) und jedes Spielerfahrzeug ist eine Streaming-Quelle; das Verkehrsbudget bleibt global begrenzt (`VEH-DEC-KO-009`).

---

## 5. Sonderfälle

| Fall | Verhalten (Vorschlag) |
|---|---|
| **Schnelle Spielerfortbewegung** (Auto, später Zug, Flug) | Vorlauf nach Geschwindigkeit; Verkehr entlang der Route vorab; Luft- und Wasserfahrzeuge brauchen eigene Streamingregeln (außerhalb dieses Ordners) |
| **Tunnel, Brücken, Tiefgaragen, Innenräume** | Zellen und Verkehr folgen dem Wechsel; Fahrzeuge können in Garagen verschwinden und wieder erscheinen |
| **Zeitsprung** | Verkehrslage neu ableiten, NPC-Zustände nach dem NPC-Verfahren fortschreiben (`VEH-DEC-VD-013`) |
| **Schnellreise** | falls vorhanden: Zielregion vorab fortschreiben, gestaffelt materialisieren ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §11); Schnellreise ist offen (`CQ-03`) |
| **Unfall oder Verfolgung nahe der Grenze** | Beteiligte bleiben S0, auch wenn sie den Radius verlassen, bis das Ereignis endet |
| **Koop mit getrennten Spielern** | Vereinigung der Nahbereiche (Modul 23) |
| **Wiederbesuch** | gleiche Lage im gleichen Zeitfenster; Grad der Gleichheit offen (`CQ-114`) |

---

## 6. Prüfregeln

> **💡 VORSCHLAG.** Eigenschaften, die jede Umsetzung erfüllen muss (Tests in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md)):

1. **Erhaltung:** Ein Fahrzeug ist zu jedem Zeitpunkt in genau einer Stufe und an genau einem Ort.
2. **Kontinuität:** Beim Stufenwechsel verändern sich Identität, Besitzer, Zustand und Fahrstil nicht.
3. **Konsistenz:** Der Verkehr beim Wiederbesuch im selben Zeitfenster stimmt überein, wenn nichts geändert wurde.
4. **Nie leer, nie überfüllt:** Die Dichte beim Betreten liegt innerhalb der Dichtekurve (kein Pop-in in Sichtweite, `N-LE-026`).
5. **Zeit:** Fortschreiben läuft der Weltuhr nie voraus (`N-SI-019`, 💡).
6. **Determinismus:** Gleicher Seed und gleiche Zeit führen zum gleichen Ergebnis (`VEH-DEC-KI-010`).

---

## 7. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-LS-001 | Crowd und Verkehr nutzen LOD und Abstraktion und rekonstruieren eine glaubwürdige lokale Bevölkerung beim Betreten eines Gebiets | 📋 BESCHLOSSEN | `W-UR-011`; `W-TV-009` | LS VKR | – |
| VEH-DEC-LS-002 | Ferne NPCs werden als Zustand geführt; die Hintergrundsimulation arbeitet ereignisbasiert; Reisen kostet Zeit; kein NPC ist an zwei Orten | 📋 BESCHLOSSEN | `N-LE-013`; `N-SI-011` | LS NPC | – |
| VEH-DEC-LS-003 | Nicht jeder NPC läuft als voller Unreal-Actor; die Weltgröße ist nirgends fest verdrahtet | 📋 BESCHLOSSEN | `N-LT-007` | LS | – |
| VEH-DEC-LS-004 | Mehrere Simulationsstufen sind vorgesehen (Arbeitsnamen S0 bis S3, SD) | 🧭 RICHTUNG | `N-SI-008` | LS | – |
| VEH-DEC-LS-005 | Leistungsstufen der Offscreen-Simulation und deterministische Resynchronisation von Zügen, Verkehr und NPCs sind offen | ❓ OFFEN | `D15-OQ` Punkt 9; `M-OQ16` Punkt 9; `W-TV-008` | LS | `CQ-110`, `CQ-112` |
| VEH-DEC-LS-006 | Fahrzeug und Fahrer teilen dieselbe Stufe: S0 Spielerfahrzeug und unmittelbare Interaktion, S1 Nahverkehr, S2 Region, S3 Rest, SD Verkehrsmenge | 💡 VORSCHLAG | Ableitung aus `N-SI-008` | LS FZ NPC | – |
| VEH-DEC-LS-007 | Distanzgrenzen kommen aus konfigurierbaren Tabellen nach dem Muster von `DetailTierTable`, getrennt für Darstellung und Simulation | 💡 VORSCHLAG | `Core/include/spn/core/DetailTiers.h` | LS | Werte: `CQ-110` |
| VEH-DEC-LS-008 | Die Materialisierung setzt zuerst persönlichen Verkehr (TRAVEL), dann geparkte Fahrzeuge, dann deterministisch die Verkehrsmenge und den Kantenzustand | 💡 VORSCHLAG | Ableitung aus `N-LE-026`, `VEH-DEC-NF-012` | LS VKR | – |
| VEH-DEC-LS-009 | Das Rückschreiben speichert Kante und Fortschritt; Identität, Fahrstil und Zustand bleiben | 💡 VORSCHLAG | Ableitung aus `N-SI-010` | LS PER | – |
| VEH-DEC-LS-010 | Fahrzeuge der Stufe S1 existieren nur auf geladenen Zellen; der Verkehr entlang der Route wird vor dem schnellen Spieler materialisiert | 💡 VORSCHLAG | Ableitung aus [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §11 | LS UE | Sicherung: `CQ-111` |
| VEH-DEC-LS-011 | Zeitzustände der Straße (Baustellen, Sperrungen) sind Laufzeitobjekte oder Data Layers | 💡 VORSCHLAG | – | LS SN | `CQ-113` |
| VEH-DEC-LS-012 | Prüfregeln: Erhaltung, Kontinuität, Konsistenz beim Wiederbesuch, nie leer und nie überfüllt, Weltuhr, Determinismus | 💡 VORSCHLAG | Ableitung aus `N-LE-026`, `N-SI-019` | LS TST | Tests: Modul 31 |
| VEH-DEC-LS-013 | Radien, Taktung, Fahrzeugzahlen je Stufe und Vorlaufstrecken sind nicht belegt | 🔍 UNBELEGT | – | LS PF | `CQ-110` |
| VEH-DEC-LS-014 | Wie weit die Szene beim Wiederbesuch gleich sein muss, ist nicht entschieden | ❓ OFFEN | `N-PE-022` | LS PER | `CQ-114` |

---

## 8. Offene Punkte

| Frage | Verweis |
|---|---|
| Radien, Taktung, Zahlen je Stufe | `CQ-110` |
| Sicherung bei zu schnellem Spieler | `CQ-111` |
| Determinismus über Plattformen | `CQ-112` |
| Zeitzustände als Laufzeitobjekte oder Data Layers | `CQ-113` |
| Grad der Gleichheit beim Wiederbesuch | `CQ-114` |
| Schnellreise | `CQ-03` |
