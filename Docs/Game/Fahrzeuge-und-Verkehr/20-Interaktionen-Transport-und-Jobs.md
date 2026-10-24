# Fahrzeuge und Verkehr 20: Interaktionen, Transport und Jobs

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass es Taxi, Rideshare, Lieferdienste, Trucking und Güterverkehr gibt und dass der Spieler Taxi- und Mitfahrjobs sowie Lieferungen und Trucking übernehmen kann. Bezahlung, Lizenzen, Auftragserzeugung und Priorität sind nicht festgelegt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [Welt 12](../World/12-Gesellschaft-Wirtschaft-und-Ressourcen.md), [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md), [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Aufträge aus Weltbedarf](#2-aufträge-aus-weltbedarf)
3. [Fahrgäste](#3-fahrgäste)
4. [Fracht und Gespanne](#4-fracht-und-gespanne)
5. [Reisen mit öffentlichen Verkehrsmitteln](#5-reisen-mit-öffentlichen-verkehrsmitteln)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Taxi | Taxis gibt es; der Spieler kann Taxi und Rideshare als Nebenjob fahren | `23#1025E` Nr. 20, 22; `W-WI-005` |
| Rideshare | Mehrere konkurrierende Anbieter mit unterschiedlichen Preisen und Qualitäten: ok, aber später und nicht priorisiert | `23#1025E` Nr. 21 |
| Lieferdienste | Essen, Pakete und Kurierdienste auch als Jobs | `23#1025E` Nr. 23; `W-WI-005` |
| Fracht | Frachtverkehr mit Lkw zwischen Industrie, Hafen, Lagerhäusern und Städten; Trucking als Spielerjob | `23#1025E` Nr. 37, 38; `W-VK-011` |
| Schiene und Hafen | Güterzüge im Weltverkehr sichtbar; Hafenverkehr mit Frachtschiffen, Fischerbooten, Fähren, Yachten und Servicefahrzeugen | `23#1025E` Nr. 39, 40 |
| Fernreisen | Zug oder Bus sind für lange Strecken bevorzugt, weil schneller | `W-VK-014`; `23#1025E` Nr. 36 |
| Abschlepp- und Fahrzeugtransport | Abschleppdienst als NPC-System und möglicher Spielerjob (‚eher nicht so oft‘); Fahrzeugtransport mit Anhängern physisch nutzbar | `23#1025E` Nr. 41, 42 |
| Smartphone | Taxi und Mitfahrt gehören zum Smartphone-Ökosystem | Gameplay-Systeme (Smartphone); `M-CA0905` |
| Zugfahren | Selbst fahren nur mit passendem Job oder Berechtigung | `W-VK-004` |
| Offen | Bezahlung, Lizenzen für andere Fahrzeuge als Züge, Jobumfang | – |

---

## 2. Aufträge aus Weltbedarf

> **💡 VORSCHLAG.** Aufträge entstehen **aus dem Bedarf der Welt**, nicht aus einer künstlichen Auftragstafel: NPCs wollen von A nach B (aus ihrem Tagesablauf, Modul 14), Betriebe brauchen Lieferungen (aus ihrer Logistik), Fahrzeuge brauchen Abschleppen (aus Pannen und Unfällen, Modul 16).
>
> ```text
> TransportRequest
>   request_id, requester_ref (NPC oder Betrieb)
>   kind            (Fahrgast / Lieferung / Fracht / Abschleppen / Fahrzeugtransport)
>   from_ref, to_ref, time_window, payload_ref
>   offered_pay     (Wert offen)
>   status
> ```
>
> Die Anforderung ist **ein Zustand der Welt**, den NPC-Fahrer (Berufsfahrer, Modul 14) oder der Spieler annehmen können. Unabhängig vom Spieler wird er von NPCs erfüllt (`W-TV-005`: die Welt läuft nicht vom Spieler abhängig). Das hält den Güter- und Personenverkehr plausibel (`N-LT-011`) und gibt dem Spieler echte, nicht zufällig erzeugte Aufträge.

Der Umfang der Jobs für die frühen Phasen ist nicht beschlossen (`CQ-84`). Die Bezahlung hängt an der Wirtschaft (offen, [Welt 12](../World/12-Gesellschaft-Wirtschaft-und-Ressourcen.md)).

---

## 3. Fahrgäste

> **💡 VORSCHLAG.** Ein Fahrgast ist ein echter NPC im Zustand TRAVEL (Modul 14), der den Spieler als Fahrer benutzt. Daraus ergeben sich Gespräche und Beziehungseffekte nach dem Dialog- und Beziehungssystem der NPC-Dokumentation. Der Fahrgast hat ein Ziel, eine Reaktion auf Fahrstil (Fahrt in Eile, Unfallrisiko) und ein Gedächtnis (`N-LT-002`), ohne dass dafür eine zweite Struktur nötig wäre.
>
> Mehrere Anbieter für Fahrdienste (`23#1025E` Nr. 21) entsprechen konkurrierenden Betrieben mit Preis- und Qualitätsprofil. Der Aufwand ist gering, solange sie als Betriebe derselben Wirtschaftsstruktur dienen; die Priorität ist ausdrücklich gering.

---

## 4. Fracht und Gespanne

Fracht ist physisch: Sie hat Masse (`VEH-DEC-PH-010`), Ziel und Besitzer.

> **💡 VORSCHLAG.** Fracht nutzt das Fahrzeuginventar (Modul 06) und das Inventarsystem; ihre Masse wirkt auf die Fahrphysik. Gespanne (Sattelzug, Anhänger, Autotransporter) sind ein Sonderfall der Fahrphysik mit zusätzlicher Kopplung. Dass Anhänger physisch nutzbar sein sollen, ist beschlossen (`23#1025E` Nr. 42); die Tiefe der Gespannphysik ist offen (`CQ-11`).

Güterzüge, Hafenverkehr und Lieferketten im Hintergrund laufen als Weltsimulation durch NPC-Betreiber (`W-TV-005`, `W-VK-004`) und werden hier nicht modelliert.

---

## 5. Reisen mit öffentlichen Verkehrsmitteln

Der Spieler soll lange Strecken bevorzugt mit Zug oder Bus zurücklegen (`W-VK-014`). Offen ist, **wie Reisezeit erlebt wird**: in Echtzeit auf der Fahrt, als Zeitraffer, als Überspringen mit Ankunftsladebild. Die Frage verbindet sich mit der offenen Schnellreise ([Welt 22](../World/22-Offene-Fragen-Katalog.md), OF-50). Das Modul legt sich nicht fest (`CQ-83`).

Auch ob Taxi und Mitfahrdienst als Fortbewegung des Spielers eine Rolle neben Eigenfahrzeug und ÖPNV spielen, ist nicht beschrieben (`CQ-85`).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-IT-001 | Taxis gibt es; der Spieler kann Taxi und Rideshare als Nebenjob fahren | 📋 BESCHLOSSEN | `23#1025E` Nr. 20, 22; `W-WI-005` | WIR FZ | – |
| VEH-DEC-IT-002 | Mehrere konkurrierende Rideshare-Anbieter mit unterschiedlichen Preisen und Qualitäten sind ok, aber später und nicht priorisiert | 📋 BESCHLOSSEN | `23#1025E` Nr. 21 | WIR | – |
| VEH-DEC-IT-003 | Lieferdienste (Essen, Pakete, Kurier) sind Jobs | 📋 BESCHLOSSEN | `23#1025E` Nr. 23; `W-WI-005` | WIR | – |
| VEH-DEC-IT-004 | Frachtverkehr mit Lkw zwischen Industrie, Hafen, Lagerhäusern und Städten; Trucking ist ein Spielerjob | 📋 BESCHLOSSEN | `23#1025E` Nr. 37, 38; `W-VK-011` | WIR FZ | – |
| VEH-DEC-IT-005 | Güterzüge sind im Weltverkehr sichtbar; Hafenverkehr ist dynamisch | 📋 BESCHLOSSEN | `23#1025E` Nr. 39, 40 | WLT | Bahnbetrieb: `W-VK-004` |
| VEH-DEC-IT-006 | Zug oder Bus sind für lange Strecken bevorzugt | 📋 BESCHLOSSEN | `W-VK-014`; `23#1025E` Nr. 36 | OPV | Erleben der Reisezeit: `CQ-83` |
| VEH-DEC-IT-007 | Fahrzeugtransport mit Anhänger oder Transporter ist physisch nutzbar | 📋 BESCHLOSSEN | `23#1025E` Nr. 42 | FZ | Gespannphysik: `CQ-11` |
| VEH-DEC-IT-008 | Transportaufträge entstehen aus dem Bedarf der Welt (NPC-Fahrtwünsche, Betriebslogistik, Pannen) und sind ein Weltzustand, den NPCs oder der Spieler erfüllen | 💡 VORSCHLAG | Ableitung aus `W-TV-005`, `W-TV-002` | WIR NPC | Format offen |
| VEH-DEC-IT-009 | Ein Fahrgast ist ein echter NPC im Zustand TRAVEL; Gespräche und Beziehungen laufen über das NPC-System | 💡 VORSCHLAG | Ableitung aus `N-LT-002`, `N-LT-019` | NPC DLG | – |
| VEH-DEC-IT-010 | Fracht nutzt das Fahrzeuginventar; ihre Masse wirkt auf die Fahrphysik | 🧩 ABGELEITET | `VEH-DEC-PH-010`; `VEH-DEC-EI-009` | FZ INV | – |
| VEH-DEC-IT-011 | Konkurrierende Fahrdienste sind Betriebe derselben Wirtschaftsstruktur mit Preis- und Qualitätsprofil | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 21 | WIR | Niedrige Priorität |
| VEH-DEC-IT-012 | Bezahlung, Lizenzen für andere Fahrzeuge als Züge und der Jobumfang der frühen Phasen sind nicht belegt | ❓ OFFEN | `W-VK-004` (nur Bahn); – | WIR | `CQ-84` |
| VEH-DEC-IT-013 | Ob Reisezeit im ÖPNV in Echtzeit, im Zeitraffer oder übersprungen erlebt wird, ist offen | ❓ OFFEN | `W-VK-014`; Welt OF-50 | OPV UI | `CQ-83` |
| VEH-DEC-IT-014 | Die Rolle von Taxi und Mitfahrdienst als Fortbewegung des Spielers (neben Eigenfahrzeug und ÖPNV) ist nicht beschrieben | 🔍 UNBELEGT | – | WIR UI | `CQ-85` |
| VEH-DEC-IT-015 | Das Verhältnis der Smartphone-Dienste (Taxi, Mitfahrt, Lieferung) zur Weltwirtschaft ist nicht beschrieben | 🔍 UNBELEGT | `M-CA0905` (‚may include‘) | UI WIR | `CQ-86` |
| VEH-DEC-IT-016 | Fahrzeugkosten (Kauf, Reparatur, Treibstoff, Rennen) zählen zu den Ausgaben, Motorsport und Nebenjobs zu den Einnahmen der Spielwirtschaft | 📋 BESCHLOSSEN | `M-PS31` §53 | WIR FZ | Beträge nicht belegt |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Reisezeit im ÖPNV erleben | `CQ-83` |
| Jobumfang, Bezahlung, Lizenzen | `CQ-84` |
| Taxi und Mitfahrdienst als Spielerfortbewegung | `CQ-85` |
| Smartphone-Dienste und Wirtschaft | `CQ-86` |
| Gespannphysik | `CQ-11` |
