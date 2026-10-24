# Fahrzeuge und Verkehr 06: Einsteigen, Aussteigen, Sitze und Kofferraum

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass früh ein einfacher Zugang mit automatischem Einsteigen genügt, dass der Sitzwechsel automatisch bleibt und dass der Kofferraum die mobile Ausrüstungsbasis ist. Sitzordnung, Besatzung, Fahrzeugtüren und die Interaktionsarchitektur sind nicht festgelegt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Interaktion allgemein: [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) und [Proposal 0007](../../Proposals/0007-pickup-items-and-minimal-inventory.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Zugangszustände des Fahrzeugs](#2-zugangszustände-des-fahrzeugs)
3. [Sitze und Besatzung](#3-sitze-und-besatzung)
4. [Kofferraum und Fahrzeuginventar](#4-kofferraum-und-fahrzeuginventar)
5. [Schnittstellen](#5-schnittstellen)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Zugang | Früh einfacher Zugang und einfache Startinteraktion mit automatischem Einsteigen; langfristig tiefere Interaktion und erweiterte Startprozeduren; der Sitzwechsel bleibt automatisch | `W-FZ-011`, `D15-RV` |
| Kofferraum | mobile Ausrüstungsbasis des Hunters; Inventar im Fahrzeug; Fahrzeuglager | `W-FZ-001`; Gameplay-Systeme §6.4 |
| Besondere Fracht | Leichen oder Tierkörper im Fahrzeug sind geplant; Passagiere, Verletzte, Bewusstlose, Leichen, Kadaver und große Ausrüstung je nach Fahrzeug (Rückbank, Kofferraum, Pickup, Van); Polizei kann sie finden | Gameplay-Systeme §6.4; `M-DC31` §48 |
| Fahrzeug-App | Schloss, Fenster, Licht, Motor, Kofferraum, Fernstart, Parkposition, Kraftstoff oder Ladung, Status; nur vorhandene Funktionen | `M-DC31` §34; `M-PS31` §48 |
| Bargeld | Bargeld im Auto oder im Safe geht nicht verloren, nur das in der Tasche | `23#613E` Nr. 17 |
| Begleiter | Begleiter fahren teils mit eigenem Auto oder Motorrad; man kann sich ‚dort treffen‘; die Fahrzeugrolle ist Teil der Einsatzplanung | `N-CO-005`, `N-CO-010` |
| Fahrzeug rufen | Fahrzeug-App für Fernsteuerung und Anliefern per Telefon | `W-FZ-010` |
| Persistenz | Fahrzeugzustand und Kofferraum-Inventar sind persistent | `W-FZ-016` |
| Offen | die Interaktionsarchitektur für Fahrzeuge | [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §6 |

---

## 2. Zugangszustände des Fahrzeugs

> **💡 VORSCHLAG.** Ein kleines Zustandsmodell des Zugangs, damit Diebstahl, Polizei, Beweissicherung und NPC-Fahrer dieselbe Sprache sprechen. Die Namen sind Arbeitsnamen.

```text
AbgestelltOffen    abgestellt, nicht abgeschlossen
AbgestelltZu       abgeschlossen (Schlüssel/Zugang nötig; Modul 18)
BesetztMotorAus    Fahrer oder Insassen im Fahrzeug, Motor aus
BesetztMotorAn     Motor läuft
Gesperrt           beschlagnahmt, abgesperrt (Tatort, Polizei)
Abtransport        abgeschleppt oder verladen
```

Zustandswechsel laufen über Ereignisse (Einsteigen, Aussteigen, Zündung, Abschließen, Aufbrechen, Abschleppen) und werden an den Eigentümer des Fahrzeugzustands gemeldet ([Modul 22](22-Persistenz-und-Lebenszyklen.md)). Der Zustand ist die Grundlage dafür, ob ein NPC ein Fahrzeug stehlen, ein Zeuge es wiedererkennen oder die Polizei es untersuchen kann.

**Früh vs. lang** (aus dem Phasenvertrag): Früh genügt eine Interaktion, die das Einsteigen und Anlassen in einem Schritt auslöst. Die tiefere Startprozedur (Zündung, Anlasser, Warmlaufen, Fehlstart) ist Langzeitziel; das Zustandsmodell sollte sie nicht ausschließen.

---

## 3. Sitze und Besatzung

Die Quellen legen fest: Der Sitzwechsel ist automatisch (`W-FZ-011`). Sie legen nicht fest, wie viele Plätze ein Fahrzeug hat, wer wo sitzt oder wie sich die Besatzung auf Plätze verteilt.

Randbedingungen, die aus beschlossenen Anforderungen folgen:

- Koop mit bis zu vier Spielern (`CLAUDE.md` §6): Ein Fahrzeug muss mehrere Spieler tragen können, oder die Besatzung muss auf mehrere Fahrzeuge verteilt werden können. Die Netzwerkfrage ist offen ([Modul 23](23-Koop-und-Netzwerk.md)).
- Begleiter (zwei bis drei für manche Einsätze, `N-CO-008`): Mitfahren und Eigenfahren sind beide vorgesehen.
- Passagiere während der Fahrt: Gespräche und Beziehungsereignisse sind ein Charakterkanal (`VEH-DEC-GP-010`).

> **💡 VORSCHLAG.** Jeder Fahrzeugtyp (Modul 03) führt eine Liste von Sitzen mit Rolle (Fahrer, Beifahrer, hinten) und Zugriffsrecht (zum Beispiel darf aus dem Fenster geschossen werden: offen im [Kampfsystem](../Kampfsystem/28-Konflikte-und-offene-Fragen.md)). Ein Sitz ist belegt durch den Verweis auf eine Person (Spieler, Begleiter, NPC), nicht durch eine Kopie ihres Zustands. Das passt zu `VEH-DEC-GP-015` (ein Fakt, ein Eigentümer).

Fahrzeuge mit Mitfahrern machen NPC-Fahrten (Taxi, Linienbus) möglich; ihre Behandlung steht in [Modul 20](20-Interaktionen-Transport-und-Jobs.md).

---

## 4. Kofferraum und Fahrzeuginventar

Das Fahrzeug trägt Ausrüstung, Beweise, Fracht, Leichen und Bargeld. Aus den Anforderungen folgt:

1. **Masse:** Ladung verändert die Masse (`VEH-DEC-PH-010`). Das Inventar liefert dem Fahrzeug also einen Massewert.
2. **Beweise und Recht:** Gegenstände im Fahrzeug sind Spuren und Beweise (`E-RE-002`: Fahrzeug und Kennzeichen sind Beweisquellen); eine Durchsuchung ist ein rechtlicher Vorgang (offen, `M-OD` ‚Crime / law / evidence‘).
3. **Bargeld im Auto:** geschützt vor dem Verlust, der nur das Bargeld in der Tasche betrifft (`23#613E` Nr. 17). Was genau den Verlust auslöst, steht in der Quelle nicht; die Aussage bezieht sich vermutlich auf Tod oder Niederlage (siehe den offenen Todesloop in [Kampfsystem 14](../Kampfsystem/14-Bewusstlosigkeit-Tod-und-Rettung.md)). Das Modul nimmt nur die Zuordnung ‚Fahrzeuginventar ist kein Taschenbargeld‘ auf.
4. **Diebstahl:** Was im Fahrzeug liegt, geht mit dem Fahrzeug, wenn es gestohlen wird (Folge der Eigentümerregel). Ob der Spieler Inhalte verlieren kann, ist eine Designfrage (`CQ-20`).
5. **Leichen und Tierkörper:** geplant, aber ohne Regel (Geruch, Beweiswert, NPC-Reaktion bei Kontrolle). Keine Spezifikation in den Quellen.

Das Inventarsystem selbst ist nicht Gegenstand dieses Ordners ([Proposal 0007](../../Proposals/0007-pickup-items-and-minimal-inventory.md) für das minimale Inventar).

---

## 5. Schnittstellen

| Partner | Schnittstelle | Richtung |
|---|---|---|
| [Modul 18](18-Besitz-Zulassung-und-Diebstahl.md) | Schlüssel, Zugangsrecht, Aufbrechen | lesen und melden |
| [Modul 21](21-Fahrzeuge-in-Ermittlungen-und-Jagden.md) | Spuren im Fahrzeug, Beschlagnahme | melden |
| [Modul 22](22-Persistenz-und-Lebenszyklen.md) | Zustand und Inventar speichern | melden |
| [Modul 23](23-Koop-und-Netzwerk.md) | Sitzvergabe und Fahrerrechte bei mehreren Spielern | lesen |
| [Kampfsystem](../Kampfsystem/README.md) | Schießen aus Fahrzeugen, Verletzung von Insassen bei Unfall | lesen |

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-EI-001 | Früh genügt ein einfacher Zugang mit automatischem Einsteigen und einfachem Start; langfristig sind tiefere Interaktion und erweiterte Startprozeduren vorgesehen; der Sitzwechsel bleibt automatisch | 📋 BESCHLOSSEN | `W-FZ-011`; `D15-RV` | FZ INT | – |
| VEH-DEC-EI-002 | Der Kofferraum ist die mobile Ausrüstungsbasis; das Fahrzeug hat Lager; Fahrzeugzustand und Kofferraum-Inventar sind persistent | 📋 BESCHLOSSEN | `W-FZ-001`; `W-FZ-016` (🧭 für die Persistenz) | FZ INV PER | Schema offen (`CQ-90`) |
| VEH-DEC-EI-003 | Leichen und Tierkörper im Fahrzeug sind vorgesehen | 📋 BESCHLOSSEN | Gameplay-Systeme §6.4; `M-DC31` §48 | FZ INV | Platzierung: `VEH-DEC-EI-015`; Beweiswert, Geruch, Reaktion bei Kontrolle: `CQ-21` |
| VEH-DEC-EI-004 | Bargeld im Fahrzeug oder Safe geht nicht verloren; verloren geht nur das Bargeld in der Tasche | 📋 BESCHLOSSEN | `23#613E` Nr. 17 | INV WIR | Auslöser des Verlusts nicht in der Quelle |
| VEH-DEC-EI-005 | Begleiter können eigene Fahrzeuge fahren, man kann sich ‚dort treffen‘, und die Fahrzeugrolle ist Teil der Einsatzplanung | 📋 BESCHLOSSEN | `N-CO-005`, `N-CO-010` | NPC FZ | – |
| VEH-DEC-EI-006 | Fahrzeug rufen oder liefern lassen per Telefon-App ist vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010` | FZ UI | Bedienung und Kosten offen |
| VEH-DEC-EI-007 | Zugangszustände des Fahrzeugs (abgestellt offen, abgestellt zu, besetzt Motor aus/an, gesperrt, Abtransport) werden über Ereignisse gewechselt | 💡 VORSCHLAG | Ableitung aus `W-FZ-011`, `W-FZ-016`, `E-RE-002` | FZ KRI | Format offen |
| VEH-DEC-EI-008 | Jeder Fahrzeugtyp führt eine Sitzliste mit Rollen; ein Sitz verweist auf die Person, die ihn belegt, ohne ihren Zustand zu kopieren | 💡 VORSCHLAG | `VEH-DEC-GP-015`; `CLAUDE.md` §6 (Koop bis vier Spieler) | FZ KOO | Sitzzahlen nicht belegt |
| VEH-DEC-EI-009 | Das Fahrzeuginventar liefert dem Fahrzeug eine Masse und ist Teil der Spuren- und Beweislage | 🧩 ABGELEITET | `VEH-DEC-PH-010`; `E-RE-002` | INV FZ ERM | – |
| VEH-DEC-EI-010 | Wie die Interaktionsarchitektur für Fahrzeuge aussieht (Türen, Kofferraum, Schlüssel, Motorstart), ist nicht festgelegt | ❓ OFFEN | [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §6; [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) | INT | `CQ-19` |
| VEH-DEC-EI-011 | Ob der Spieler Fahrzeuginhalt durch Diebstahl, Abschleppen oder Beschlagnahme verlieren kann und wie das ausgeglichen wird, ist nicht entschieden | ❓ OFFEN | – | FZ INV | `CQ-20` |
| VEH-DEC-EI-012 | Sitzzahlen, Besatzungsregeln und Fahrerwechsel mehrerer Spieler sind nicht festgelegt | ❓ OFFEN | `CLAUDE.md` §6; `E-KO-010` | KOO FZ | `CQ-22` |
| VEH-DEC-EI-013 | Die Fahrzeug-App beziehungsweise das Schnellmenü kann Schloss, Fenster, Licht, Motor, Kofferraum, Fernstart (wo unterstützt), Parkposition, Kraftstoff oder Ladung und Status bedienen, aber nur Funktionen, die das Fahrzeug wirklich besitzt; Lieferung über eine App ist denkbar | 📋 BESCHLOSSEN | `M-DC31` §34; `M-PS31` §48 | FZ UI | Bedienung beim Fahren: `CU-03` |
| VEH-DEC-EI-014 | Der Fahrzeuglagerraum nimmt schwere Waffen, Outfits, Hunter-Ausrüstung, Werkzeuge, Tiere und Kadaver, gegebenenfalls Verletzte und Körper sowie Geld und Verstecke auf | 📋 BESCHLOSSEN | `M-PS31` §45 | FZ INV | – |
| VEH-DEC-EI-015 | Das Fahrzeug kann normale Passagiere, Verletzte, Bewusstlose, Leichen, Tierkadaver und große Ausrüstung tragen; die Platzierung hängt vom Fahrzeug ab (Rückbank, Kofferraum, Pickup-Ladefläche, Van-Laderaum); bei Polizeidurchsuchungen kann verborgene Fracht entdeckt werden | 📋 BESCHLOSSEN | `M-DC31` §48; `M-PS31` §46 | FZ INV POL | Durchsuchungsregeln offen (`COMBAT-DEC-PO-023`) |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Interaktionsarchitektur für Fahrzeuge | `CQ-19` |
| Verlust von Fahrzeuginhalt bei Diebstahl, Abschleppen, Beschlagnahme | `CQ-20` |
| Regeln für Leichen und Tierkörper im Fahrzeug | `CQ-21` |
| Sitzzahlen, Besatzung, Fahrerrechte bei mehreren Spielern | `CQ-22` |
