# Fahrzeuge und Verkehr 23: Koop und Netzwerk

> [!IMPORTANT]
> **Die Netzwerkarchitektur ist offen** (`E-KO-010`, `N-KO-018`), und die Mehrspieler-Synchronisation von Fahrzeugen ist nicht beschlossen (`W-FP-010`; `D15-RV`). Beschlossen sind der Rahmen des Koops (2 bis 4 Spieler, Host-Spielstand maßgeblich) und dass Mods und Cheats im Koop erlaubt sind. Dieses Modul sammelt, was das Fahrzeug- und Verkehrssystem von der Netzwerkentscheidung braucht, und stellt Optionen nebeneinander, **ohne eine zu wählen**. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [NPCs 23](../NPCs/23-Koop-Host-Speicherstand-und-Mods.md), [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md), [Kampfsystem 23](../Kampfsystem/23-Koop-Synchronisierung-und-Autoritaet.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Was das Fahrzeugsystem vom Netzwerk braucht](#2-was-das-fahrzeugsystem-vom-netzwerk-braucht)
3. [Optionen für die Autorität der Fahrphysik](#3-optionen-für-die-autorität-der-fahrphysik)
4. [Verkehr im Koop](#4-verkehr-im-koop)
5. [Besitz, Sitze, Entscheidungen](#5-besitz-sitze-entscheidungen)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Rahmen | Koop: 2 bis 4 Spieler, kampagnenbasiert, Mods und Cheats erlaubt, getrennt von der offiziellen Online-Progression | `N-KO-003`; `CLAUDE.md` §6 |
| Host | Host-Spielstand maßgeblich; der Fortschritt von Gästen gehört zum Host-Spielstand; die Welt ist die Welt des Hosts | `N-KO-001`; `E-KO-004` |
| Gast | Was ein Gast auf seinem eigenen Spielstand hat, bleibt dort; bei einem Freund hat er nur, was auf dessen Spielstand ist | `N-KO-002` |
| Zeitsprung | Ein Welt-Zeitsprung im Koop nur mit Zustimmung aller aktiven Spieler; Verhaftung ist individuell, die Welt läuft weiter | `N-KO-008` |
| Autorität | Die Simulation läuft nur auf der Autorität; die Replikation des Simulationszustands ist offen | `N-KO-018`; `E-KO-010` |
| Online | Offizielles Online hat eine eigene Zeit- und Serverlogik, später | `N-KO-004` |
| Offen | Fahrzeugsynchronisation | `W-FP-010`; `D15-RV` |
| Leitlinie | Unnötig teure Einzelspieler-Annahmen vermeiden | `CLAUDE.md` §6 |

---

## 2. Was das Fahrzeugsystem vom Netzwerk braucht

Die Aufgaben lassen sich benennen, bevor die Architektur gewählt ist:

| Aufgabe | Bezug |
|---|---|
| Fahrphysik des Fahrzeugs eines Spielers auf allen Rechnern darstellen | Module 04, 05 |
| Mehrere Spieler in einem Fahrzeug (Sitze, Fahrer) | Modul 06 |
| NPC-Verkehr in der Nähe jedes Spielers | Module 13, 14, 26 |
| Weltzustand (Baustellen, Sperrungen, Unfälle) auf allen Rechnern konsistent | Modul 15, 22 |
| Fahrzeugeigentum von Gästen im Host-Spielstand | Modul 18, 22 |
| Zeitsprünge im Fahrzeug (Schlafen, lange Fahrt) | `N-KO-008` |
| Streaming-Quellen mehrerer Spieler | Modul 26 |

> **🧩 ABGELEITET.** Weil unnötig teure Einzelspieler-Annahmen vermieden werden sollen (`CLAUDE.md` §6), verweisen Fahrzeug-, Sitz- und Besitzdaten auf **Personen per ID** und nicht auf ‚den Spieler‘. Das kostet im Einzelspieler nichts und erspart später Umbauten.

---

## 3. Optionen für die Autorität der Fahrphysik

> **💡 VORSCHLAG.** Die Optionen sind eine Entscheidungshilfe für `CQ-95`. Keine ist gewählt. Die Wahl sollte nach einem Prototyp mit zwei Spielern erfolgen.

| Option | Idee | Vorteil | Nachteil |
|---|---|---|---|
| A: Host-autoritativ | Der Host simuliert alle Fahrzeuge, Clients senden Eingaben und sehen das Ergebnis | konsistent, einfache Regeln | Latenz im eigenen Fahrzeug der Gäste, Vorhersage nötig |
| B: Besitzer-autoritativ | Der Spieler am Steuer simuliert sein Fahrzeug lokal, der Host prüft grob | flüssiges Fahrgefühl für den Fahrer | Fehler und Abweichungen müssen abgeglichen werden |
| C: Hybrid mit Vorhersage | Lokale Vorhersage des eigenen Fahrzeugs, Host korrigiert | gutes Gefühl bei klarer Autorität | höchster Entwicklungsaufwand |
| F: Freitext | Eigene Beschreibung | – | Inhalt hat Vorrang vor A bis C |

**Hinweis zum Vertrauensmodell:** Im Koop sind Cheats ausdrücklich erlaubt (`N-KO-003`). Das senkt die Anforderung an Betrugsschutz und macht Optionen mit lokaler Autorität des Fahrers weniger riskant als in einem kompetitiven Online-Spiel. Das ist eine Ableitung, keine Entscheidung. Für das offizielle Online, das keine Mods und Cheats erlaubt (`N-KO-004`), gilt diese Überlegung nicht.

---

## 4. Verkehr im Koop

Die Simulation von NPC-Verkehr läuft nur auf der Autorität (`N-KO-018`). Aus dem NPC-Entwurf folgt: S0 und S1 werden für die Vereinigung der Nahbereiche aller Spieler materialisiert; das Budget wächst nicht linear mit der Spielerzahl ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §10).

> **💡 VORSCHLAG.**
>
> - **Replikation nach Relevanz:** Fahrzeuge in der Nähe eines Spielers werden mit Position, Geschwindigkeit und Zustand repliziert; fernere Fahrzeuge nicht.
> - **Determinismus als Hebel:** Die Verkehrsmenge (Fahrzeuge ohne Identität, `VEH-DEC-NF-012`) entsteht aus Seed, Straße und Zeit. Sie könnte auf Gästen **lokal** erzeugt werden, ohne jede Position zu senden; nur Interaktionen (Berührung, Beteiligung) würden repliziert. Das setzt plattformübergreifenden Determinismus voraus (`CQ-112`) und kostet Beförderungsregeln für Statisten (Modul 14). Es ist eine Option zum Prüfen.
> - **Mehrere Spieler an verschiedenen Orten:** Jeder Spieler und jedes Spielerfahrzeug ist eine Streaming-Quelle (Modul 26). Das Gesamtbudget ist begrenzt.

---

## 5. Besitz, Sitze, Entscheidungen

- **Besitz:** Neue Fahrzeuge von Gästen entstehen im Host-Spielstand (`N-KO-001`, `N-KO-002`). Wie Gäste ihre eigenen Fahrzeuge aus einem eigenen Spielstand mitbringen können, ist nicht beschrieben (`CQ-96`).
- **Sitze:** Ein Fahrer steuert, die anderen sind Insassen. Welche Fähigkeiten Beifahrer haben (schießen, navigieren, Radio), und wie der Fahrer wechselt, ist offen (`CQ-97`, `CQ-22`).
- **Gemeinsame Entscheidungen:** Der Koop unterscheidet persönliche und gemeinsame Entscheidungen (`N-KO-005`). Ob ‚wohin fahren wir‘ eine gemeinsame Entscheidung ist, ist nicht festgelegt.
- **Verhaftung und Beschlagnahme:** Verhaftung ist individuell (`N-KO-008`). Ob das Fahrzeug eines verhafteten Spielers im Verkehr bleibt oder beschlagnahmt wird, ist offen.
- **Zeitsprung im Fahrzeug:** Schlafen oder lange Fahrten als Zeitsprung brauchen die Zustimmung aller aktiven Spieler (`N-KO-008`).
- **Mods:** Fahrzeugtypen aus Mods müssen bei allen Spielern vorhanden sein; ein Spielstand mit fehlenden Mods lädt standardmäßig nicht (`N-KO-013`).
- **Online:** Rennen, Fahrzeugnutzung und Zeitlogik im offiziellen Online sind nicht beschrieben (`CQ-98`).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-KO-001 | Koop: 2 bis 4 Spieler, Host-Spielstand maßgeblich, Mods und Cheats erlaubt, getrennt von der offiziellen Online-Progression | 📋 BESCHLOSSEN | `N-KO-001`; `N-KO-003`; `CLAUDE.md` §6 | KOO | – |
| VEH-DEC-KO-002 | Was ein Gast auf seinem eigenen Spielstand hat, bleibt dort; bei einem Freund hat er nur, was auf dessen Spielstand ist | 📋 BESCHLOSSEN | `N-KO-002` | KOO PER | Fahrzeugmitnahme: `CQ-96` |
| VEH-DEC-KO-003 | Ein Welt-Zeitsprung im Koop braucht die Zustimmung aller aktiven Spieler; Verhaftung ist individuell | 📋 BESCHLOSSEN | `N-KO-008` | KOO | – |
| VEH-DEC-KO-004 | Die Simulation läuft nur auf der Autorität; die Replikation des Simulationszustands ist offen | 📋 BESCHLOSSEN | `N-KO-018`; `E-KO-010` | KOO LS | Als offen geführt: `VEH-DEC-KO-013` |
| VEH-DEC-KO-005 | Die Mehrspieler-Synchronisation von Fahrzeugen ist nicht beschlossen | ❓ OFFEN | `W-FP-010`; `D15-RV` | KOO FZ | `CQ-95` |
| VEH-DEC-KO-006 | Fahrzeug-, Sitz- und Besitzdaten verweisen auf Personen per ID statt auf ‚den Spieler‘ | 🧩 ABGELEITET | `CLAUDE.md` §6 | KOO FZ | – |
| VEH-DEC-KO-007 | Optionen für die Autorität der Fahrphysik: A host-autoritativ, B besitzer-autoritativ, C hybrid mit Vorhersage, F Freitext; keine ist gewählt | 💡 VORSCHLAG | – | KOO FZ | `CQ-95` |
| VEH-DEC-KO-008 | Cheats im Koop senken die Anforderung an Betrugsschutz; für das offizielle Online (ohne Mods und Cheats) gilt das nicht | 💡 VORSCHLAG | Ableitung aus `N-KO-003`, `N-KO-004` | KOO | – |
| VEH-DEC-KO-009 | Fahrzeuge in der Nähe eines Spielers werden nach Relevanz repliziert; das Verkehrsbudget wächst nicht linear mit der Spielerzahl | 💡 VORSCHLAG | Ableitung aus [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §10 | KOO LS | – |
| VEH-DEC-KO-010 | Die Verkehrsmenge könnte auf Gästen lokal aus Seed, Straße und Zeit erzeugt und nur bei Interaktion repliziert werden | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-NF-012`, `VEH-DEC-KI-010` | KOO VKR | Setzt plattformübergreifenden Determinismus voraus: `CQ-112` |
| VEH-DEC-KO-011 | Jeder Spieler und jedes Spielerfahrzeug ist eine Streaming-Quelle | 💡 VORSCHLAG | Ableitung aus [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §10 | LS KOO | – |
| VEH-DEC-KO-012 | Fahrzeugtypen aus Mods müssen bei allen Spielern vorhanden sein; ein Spielstand mit kritisch fehlenden Mods lädt standardmäßig nicht | 📋 BESCHLOSSEN | `N-KO-013` | MOD KOO | – |
| VEH-DEC-KO-013 | Netzwerkarchitektur und Replikation sind offen | ❓ OFFEN | `E-KO-010`; `N-KO-018` | KOO | `CQ-95` |
| VEH-DEC-KO-014 | Fahrerwechsel, Beifahrerfähigkeiten, gemeinsame Fahrentscheidungen und Beschlagnahme beim verhafteten Spieler sind nicht beschrieben | ❓ OFFEN | `N-KO-005`; `N-KO-008` | KOO FZ | `CQ-97` |
| VEH-DEC-KO-015 | Fahrzeug-, Renn- und Zeitlogik im offiziellen Online sind nicht beschrieben | 🔍 UNBELEGT | `N-KO-004` | KOO | `CQ-98` |
| VEH-DEC-KO-016 | Im offiziellen Online bleiben Geld, Fahrzeuge, Immobilien und Fortschritt beim Wechsel zwischen öffentlicher, Freunde- und privater Sitzung erhalten; es werden keine absurd dominanten Fahrzeuge zur Leistungssteigerung eingeführt; als langfristiges Ziel gelten 64 bis 100 gleichzeitige Spieler, falls technisch machbar | 📋 BESCHLOSSEN | `M-DC31` §12, §13 | KOO FZ | Spielerzahl nur Zielrichtung; Verkehr im Online: `CQ-98` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Netzwerkarchitektur und Autorität der Fahrphysik | `CQ-95` |
| Fahrzeugmitnahme von Gästen | `CQ-96` |
| Fahrerwechsel, Beifahrerfähigkeiten, gemeinsame Fahrentscheidungen | `CQ-97`, `CQ-22` |
| Online-Modus | `CQ-98` |
| Plattformübergreifender Determinismus | `CQ-112` |
