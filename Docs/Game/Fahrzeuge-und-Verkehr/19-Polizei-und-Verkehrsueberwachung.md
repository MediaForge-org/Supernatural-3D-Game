# Fahrzeuge und Verkehr 19: Polizei und Verkehrsüberwachung

> [!IMPORTANT]
> **Nichts davon ist implementiert, und das Recht der Spielwelt ist offen.** Beschlossen ist, dass Verkehrskontrollen und Verfolgungen vorkommen können, dass Warn-Apps sie anzeigen, dass es feste und mobile Geschwindigkeitskontrollen geben darf (gleich ob Polizei oder Kamera) und dass die Polizei nicht allwissend ist. Straftatenkatalog, Strafmaß und Zuständigkeiten sind offen (`M-OD`). Dieses Modul beschreibt nur die **Verkehrsseite** der Polizeiarbeit; das Rechtssystem gehört der [Ermittlung](../Ermittlung/README.md). Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md), [Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md), [Kampfsystem 20](../Kampfsystem/20-Fraktionen-Polizei-und-Kampfkonsequenzen.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Verkehrsüberwachung als Wahrnehmung](#2-verkehrsüberwachung-als-wahrnehmung)
3. [Ablauf einer Verkehrskontrolle](#3-ablauf-einer-verkehrskontrolle)
4. [Verfolgung](#4-verfolgung)
5. [Verstöße und Strafen](#5-verstöße-und-strafen)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Kontrollen | Geschwindigkeitskontrollen, Rotlicht, gefährliches Fahren, Kennzeichen und Verfolgungen ‚können passieren‘; Warn-Apps zeigen sie an | `23#1025E` Nr. 18; `E-QA-042` |
| Radar | Feste und mobile Geschwindigkeitskontrollen: Polizei oder Blitzer ist gleich | `23#1025E` Nr. 19 |
| Verkehrskontrolle | Normale Stops oder Verfolgung je nach Lage; das Verhalten des Spielers entscheidet, ob eine Situation harmlos endet oder eskaliert | `N-BH-003`; `E-RE-011` |
| Information | Das Gesetz folgt tatsächlicher Information, nicht allwissender Fahndungslogik; Polizei ist nicht allwissend | `COMBAT-DEC-PO-001`, `COMBAT-DEC-PO-013` |
| Fahndung | Schwere Fahndung bleibt bis zur glaubwürdigen Auflösung; man kann landesweit gesucht werden; kein Sternesystem | `COMBAT-DEC-PO-008`; Widerspruch `E-PO-003` |
| Festnahme | Festnahme ist kein automatisches Game Over; Strafkette | `COMBAT-DEC-PO-009` |
| Flucht | Aufgeben und Flucht sind Wege bei Polizeikontakt | `COMBAT-DEC-PO-010` (🧭) |
| Jurisdiktionen | Mehrere Polizeitypen und Zuständigkeiten (lokal, Bezirk, Bundesstaat, Bund) | `M-WB89` Block 8; `COMBAT-DEC-PO-001` |
| Verfolgung | NPC-Kriminelle können von der Polizei verfolgt werden | `E-PO-009`; `N-LE-024` |
| Lokale Folgen | Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | `COMBAT-DEC-PO-015` |
| Fahrzeug als Beweis | Fahrzeug, Kennzeichen und Täterbeschreibung | `E-RE-002`; `E-RE-010` |
| Offen | Straftatenkatalog, Festnahmeschwellen, Strafmaß, Zuständigkeiten, Überwachungsdichte | `COMBAT-DEC-PO-023` (`M-OD`) |

---

## 2. Verkehrsüberwachung als Wahrnehmung

> **💡 VORSCHLAG.** Weil die Polizei nicht allwissend ist (`COMBAT-DEC-PO-013`), wird ein Verstoß nur geahndet, wenn ihn ein **Wahrnehmer** feststellt. Wahrnehmer im Verkehr:

| Wahrnehmer | Bezug |
|---|---|
| Streifenfahrzeug oder Polizist | NPC-Polizei als Wahrnehmung (Ermittlung 14) |
| Mobile Kontrolle (Radar-Falle) | `23#1025E` Nr. 19 |
| Feste Kamera (Geschwindigkeit, Rotlicht) | `23#1025E` Nr. 19; Überwachungskameras als Beweisquelle (`E-RE-002`) |
| Zeuge (Notruf mit Kennzeichen) | `E-RE-010` |

Eine festgestellte Übertretung wird als **Beobachtung** mit Beweiskonfidenz geführt (Ermittlung 10), nicht als Gewissheit. Das passt zum beschlossenen Prinzip, dass Beweise und Information zählen. Die Dichte der Kameras und Streifen ist nicht belegt (`CQ-82`).

**Warn-Apps:** Sie zeigen Kontrollen an (`23#1025E` Nr. 18). Ein Vorschlag, der zum Verbot des Allwissens passt (`N-LT-013`): Die App liest **bekannte** Kontrollen aus dem Weltzustand, mit Verzögerung und Unschärfe (wie von Nutzern gemeldete Standorte). Kontrollen, die sich verlegt haben, sind veraltet. So bleibt die App nützlich, aber nicht perfekt.

Geschwindigkeitskontrolle setzt Höchstgeschwindigkeiten voraus, die in den Quellen nicht stehen (`VEH-DEC-SN-015`, `CQ-38`).

---

## 3. Ablauf einer Verkehrskontrolle

> **💡 VORSCHLAG.** Eine Kontrolle ist eine kurze Zustandsfolge, die auf das Verhalten des Spielers reagiert (`N-BH-003`).

```text
Wahrnehmung (Verstoß / Verdacht / Fahndung nach Kennzeichen)
  -> Anhaltesignal
  -> Reaktion des Fahrers: anhalten / weiterfahren (Verfolgung) / Gewalt (Kampfsystem)
  -> Kontrolle: Ausweis, Fahrzeug (Kennzeichen, Zulassung), Gespräch
  -> Ergebnis: Verwarnung / Strafe / Festnahme / Weiterfahrt
```

Jeder Schritt kann von Fahrstil, Fahrzeug (auffällig, gestohlen gemeldet), Inhalt (Waffen, Leichen: [Modul 06](06-Einsteigen-Aussteigen-Sitze-und-Kofferraum.md)) und Vorgeschichte abhängen. Durchsuchungsregeln sind offen (`COMBAT-DEC-PO-023`). Der Dialog der Kontrolle gehört dem Dialogsystem.

---

## 4. Verfolgung

Verfolgungen sind genannt (`23#1025E` Nr. 18) und für NPC-Kriminelle beschlossen (`N-LE-024`). Mit Fahrzeugen zu verfolgen ist außerdem eine Ermittlungsmethode ([Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §6).

> **💡 VORSCHLAG.** Eine Verfolgung ist ein Eskalationszustand der Kontrolle oder eines Einsatzes: Verfolger, Verstärkung, mögliche Absperrungen, Beendigung. Eine Verfolgung endet durch Anhalten, Verlust des Kontakts, Unfall, Aufgabe oder Gewalt. Die Stufen sind nicht belegt (`CQ-81`). Wichtig für die Systeme:
>
> - Der Verfolgte bleibt ein Fahrzeug im Verkehr; die Verkehrs-KI muss mit hoher Geschwindigkeit und Rettungsgassen umgehen (Module 13, 16).
> - Verfolgungen erzeugen Unfälle (Modul 16) und Zeugen.
> - Die Fahndung nach einem Fahrzeug folgt dem Kennzeichen (Modul 18) und wirkt über die Information, die tatsächlich bei der Polizei angekommen ist (`COMBAT-DEC-PO-001`).

---

## 5. Verstöße und Strafen

Der Straftatenkatalog, die Festnahmeschwellen und das Strafmaß sind offen (`COMBAT-DEC-PO-023`, `M-OD` ‚Crime / law / evidence‘). Für den Verkehr fehlen zusätzlich: ein Verstoßkatalog, Bußgelder, ein Punktesystem, Führerschein, Fahrerflucht, Fahren unter Einfluss.

> **💡 VORSCHLAG.** Verstöße und Folgen liegen als **Daten je Jurisdiktion** (Katalog), nicht im Code (`VEH-DEC-GP-014`). Das ermöglicht den Wechsel zwischen Bundesstaaten und spätere Anpassungen. Ein Führerschein des Spielers ist in den Quellen nicht erwähnt (`CQ-79`).

Das Spiel benötigt diese Fragen für die Verkehrsüberwachung; es beantwortet sie nicht (`CQ-78`).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-PV-001 | Feste und mobile Geschwindigkeitskontrollen sind zulässig, gleich ob Polizeikontrolle oder Blitzer | 📋 BESCHLOSSEN | `23#1025E` Nr. 19 | POL UI | Höchstgeschwindigkeiten fehlen: `CQ-38` |
| VEH-DEC-PV-002 | Verkehrskontrollen sind normale Stops oder Verfolgungen je nach Lage; das Verhalten des Spielers entscheidet über die Eskalation | 📋 BESCHLOSSEN | `N-BH-003`; `E-RE-011` | POL | – |
| VEH-DEC-PV-003 | Das Gesetz folgt tatsächlicher Information und Beweiskonfidenz; die Polizei ist nicht allwissend | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-001`; `COMBAT-DEC-PO-013` | POL ERM | – |
| VEH-DEC-PV-004 | Festnahme ist kein automatisches Game Over; es gibt eine Strafkette von der Tat bis zum Urteil | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-009` | POL | – |
| VEH-DEC-PV-005 | Schwere Fahndung bleibt bis zur glaubwürdigen Auflösung; kein Sternesystem | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-008` | POL | Widerspruch Quiz-Text: `CK-03` |
| VEH-DEC-PV-006 | Es gibt mehrere Polizeitypen und Zuständigkeiten (lokal, Bezirk, Bundesstaat, Bund) | 📋 BESCHLOSSEN | `M-WB89` Block 8; `COMBAT-DEC-PO-001` | POL WLT | – |
| VEH-DEC-PV-007 | NPC-Kriminelle können von der Polizei verfolgt werden; der Verkehr macht Rettungsfahrzeugen Platz | 📋 BESCHLOSSEN | `E-PO-009`; `N-LE-024` | POL VKR | – |
| VEH-DEC-PV-008 | Aufgeben und Flucht sind Wege bei Polizeikontakt | 🧭 RICHTUNG | `COMBAT-DEC-PO-010` | POL | – |
| VEH-DEC-PV-009 | Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-015` | POL NPC | – |
| VEH-DEC-PV-010 | Ein Verstoß wird nur geahndet, wenn ihn ein Wahrnehmer feststellt (Streife, mobile Kontrolle, Kamera, Zeuge); die Feststellung ist eine Beobachtung mit Beweiskonfidenz | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-PO-013`, `E-RE-002` | POL ERM | Dichte: `CQ-82` |
| VEH-DEC-PV-011 | Warn-Apps lesen bekannte Kontrollen aus dem Weltzustand, mit Verzögerung und Unschärfe | 💡 VORSCHLAG | Ableitung aus `E-QA-042`, `N-LT-013` | UI POL | – |
| VEH-DEC-PV-012 | Eine Verkehrskontrolle ist eine Zustandsfolge: Wahrnehmung, Anhaltesignal, Reaktion, Kontrolle, Ergebnis | 💡 VORSCHLAG | Ableitung aus `N-BH-003` | POL | Schwellen offen |
| VEH-DEC-PV-013 | Eine Verfolgung ist ein Eskalationszustand der Kontrolle oder eines Einsatzes mit Verfolger, Verstärkung, möglichen Absperrungen und Beendigung | 💡 VORSCHLAG | Ableitung aus `E-PO-009` | POL KI | Stufen: `CQ-81` |
| VEH-DEC-PV-014 | Verstöße und Folgen liegen als Daten je Jurisdiktion (Katalog) | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-014`, `M-WB89` Block 8 | POL | – |
| VEH-DEC-PV-015 | Der Verstoßkatalog, Bußgelder, Punkte, Fahren unter Einfluss und Haft für Verkehrsdelikte sind offen | ❓ OFFEN | `COMBAT-DEC-PO-023`; `M-OD` ‚Crime / law / evidence‘ | POL | `CQ-78` |
| VEH-DEC-PV-016 | Ein Führerschein des Spielers oder von NPCs ist nicht erwähnt | 🔍 UNBELEGT | – | POL WIR | `CQ-79` |
| VEH-DEC-PV-017 | Behördenwissen muss erfasst, verarbeitet und verteilt werden; die Beweiskonfidenz hat fünf Stufen (Unbekannt, möglicher Treffer, wahrscheinlicher Treffer, identifiziert, bestätigt); die Eskalation ist mehrstufig statt zwingend ein Sternesystem | 📋 BESCHLOSSEN | `M-PS31` §54; `M-DC31` §39 | POL ERM | Widerspruch Quiz-Text: `CK-03` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Verstoßkatalog, Bußgelder, Punkte | `CQ-78` |
| Führerschein | `CQ-79` |
| Fahrerflucht und Verkehrsdelikte | `CQ-80` |
| Verfolgungsstufen und Kontrollschwellen | `CQ-81` |
| Dichte von Streifen, Kameras und Kontrollen | `CQ-82` |
| Fahndungsstufen oder kein Sternesystem | `CK-03` |
