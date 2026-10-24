# Fahrzeuge und Verkehr 09: Kraftstoff, Energie, Tankstellen und Rastplätze

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen sind reale Kraftstoffe mit echtem Verbrauch, Preise je Tankstelle, E85 und Rennkraftstoffe sowie die Rolle des Verbrennungsmotors als dominanter Antrieb. Tankstellenzahl, Preismodell, Lade- und Tankzeiten und Rastplätze sind nicht festgelegt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Energiezustand von Fahrzeugen](#2-energiezustand-von-fahrzeugen)
3. [Tankstellen, Ladeorte und Rastplätze](#3-tankstellen-ladeorte-und-rastplätze)
4. [Register](#4-register)
5. [Offene Punkte](#5-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Verbrauch | Kraftstofftank und realer Verbrauch; Verbrauch hängt von Fahrzeug, Geschwindigkeit, Fahrstil, Beladung und Temperatur ab; Reichweiten realistisch genug, damit Tanken keine ständige Fleißarbeit ist | `23#1025E` Nr. 10; `W-FZ-007`; `M-DC31` §34 |
| Sorten und Preise | Regular, Premium, Diesel und weitere; unterschiedliche Preise je Tankstelle; E85 (‚109 Oktan‘) mit Mehrleistung | `23#1025E` Nr. 11; `W-FZ-007` |
| E85 | kann bei geeigneten Motoren und Abstimmungen etwa 50 PS und mehr bringen | `W-FZ-007`; `M-WB17` Block 5 |
| Rennkraftstoffe | vorgesehen | `W-FZ-007` |
| Elektro | Ladeinfrastruktur zulässig, aber „verbrenner ist die macht im spiel“ | `W-FZ-019` |
| Offen | Lade- und Tankzeiten, Kraftstoffverbrauch je Fahrzeug, Antriebsanteil | `W-FZ-014`; Gameplay-Systeme §6.6 |

---

## 2. Energiezustand von Fahrzeugen

Jedes Fahrzeug trägt einen Energiezustand (Kraftstoffmenge und -sorte oder Ladung), der mit der Fahrleistung sinkt (`23#1025E` Nr. 10). Daraus ergibt sich ohne weitere Annahme:

- Der Spieler kann **ohne Kraftstoff liegen bleiben**. Das ist eine Folge von echtem Verbrauch und verbindet sich mit Pannen und Abschleppdienst ([Modul 16](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md)).
- Die Sorte bestimmt die Leistung (E85 gegenüber Premium) nur, wo Motor und Abstimmung es zulassen (`W-FZ-007`). Die Wirkung falscher Sorten ist nicht beschrieben.

> **💡 VORSCHLAG.** NPC-Fahrzeuge erhalten einen **groben Energiezustand**. Nahe Fahrzeuge dürfen sichtbar tanken. Ferne Fahrzeuge ändern ihren Energiezustand abstrakt über ihre Fahrtzyklen, damit ein NPC, der morgens zur Arbeit fährt, nicht mit leerem Tank dort ankommt, wenn der Spieler ihn beobachtet. Die Detailtiefe richtet sich nach Modul 26. Der Vorschlag macht Tanken zu einem Teil des NPC-Alltags (`W-TV-002`), ohne eine zweite Simulation einzuführen.

---

## 3. Tankstellen, Ladeorte und Rastplätze

### 3.1 Tankstellen und Ladeorte

Die Quellen verlangen unterschiedliche Preise je Tankstelle und Ladeinfrastruktur; sie sagen nichts über Zahl, Lage und Ausstattung. In Block 4 des Chats 23 hat der Assistent folgende Richtung formuliert (💡, vom Entwickler nur indirekt gebilligt): Tankstellen sind wesentlich verbreiteter als Ladepunkte, Schnellladeparks existieren, moderne Stadtteile besitzen mehr Ladepunkte (`23#1024A`).

> **💡 VORSCHLAG.** Eine Tankstelle ist ein **Weltobjekt mit Funktionen**, nicht nur ein Punkt zum Auftanken. Mögliche Funktionen aus anderen beschlossenen Systemen:
>
> | Funktion | Bezug |
> |---|---|
> | Kraftstoff und Laden | `W-FZ-007`, `W-FZ-019` |
> | Kleiner Laden (Gewerbe) | `W-UR-005` |
> | Parken, kurzer Aufenthalt | `W-VK-013` |
> | Zeugen und Kameras | `E-RE-002` (Überwachungskameras und Kennzeichen als Beweisquellen) |
> | Arbeitsplatz für NPCs | `W-TV-002` |
> | Treffpunkt, Ort von Zwischenfällen | `W-TV-001` |

Die Platzierung folgt dem Straßennetz und den Siedlungen. Welche Daten die Karte dazu liefert, steht in [Modul 24](24-WorldForge-Schnittstelle.md) (Dienstleistungs- und Rastpunkte als Kandidaten).

### 3.2 Rastplätze

Rastplätze und Raststätten an Fernstraßen sind in den Quellen **nicht** erwähnt. Lange Fahrten sollen spontane Stopps, Gespräche, Schlaf und Streit enthalten (Gameplay-Systeme §6.1), und Trucking ist ein Job (`W-WI-005`). Beides spricht dafür, dass Haltepunkte an Fernstraßen nützlich sind. Das ist ein Vorschlag und keine Anforderung.

---

## 4. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-KE-001 | Fahrzeuge haben einen Kraftstofftank mit realem Verbrauch | 📋 BESCHLOSSEN | `23#1025E` Nr. 10; `W-FZ-007` | FZ | Werte nicht belegt |
| VEH-DEC-KE-002 | Es gibt Benzinsorten, Diesel, E85 (‚109 Oktan‘) und weitere Sorten, mit unterschiedlichen Preisen je Tankstelle | 📋 BESCHLOSSEN | `23#1025E` Nr. 11; `W-FZ-007` | FZ WIR | Preismodell: `CQ-32` |
| VEH-DEC-KE-003 | E85 oder hochoktanige Kraftstoffe können bei geeigneten Motoren und Abstimmungen etwa 50 PS und mehr bringen | 📋 BESCHLOSSEN | `W-FZ-007`; `M-WB17` Block 5 | FZ | Wirkung falscher Sorten nicht beschrieben |
| VEH-DEC-KE-004 | Rennkraftstoffe sind vorgesehen | 📋 BESCHLOSSEN | `W-FZ-007` | FZ RAC | Bezugsort offen (`CQ-34`) |
| VEH-DEC-KE-005 | Ladeinfrastruktur für Elektrofahrzeuge ist zulässig, Verbrenner dominieren das Spiel | 📋 BESCHLOSSEN | `W-FZ-019` | FZ | Antriebsanteil: `CQ-07` |
| VEH-DEC-KE-006 | Ohne Kraftstoff oder Ladung bleibt ein Fahrzeug liegen | 🧩 ABGELEITET | `VEH-DEC-KE-001`; `W-TV-001` (Pannen) | FZ UP | – |
| VEH-DEC-KE-007 | Tankstellen sind wesentlich verbreiteter als Ladepunkte; Schnellladeparks existieren; moderne Stadtteile haben mehr Ladepunkte | 💡 VORSCHLAG | `23#1024A` (Assistent) | WLT FZ | Anteil, Zahlen offen |
| VEH-DEC-KE-008 | Eine Tankstelle ist ein Weltobjekt mit Funktionen (Kraftstoff, Laden, Shop, Parken, Kameras, Arbeitsplatz, Treffpunkt) | 💡 VORSCHLAG | Ableitung aus `W-UR-005`, `W-VK-013`, `E-RE-002`, `W-TV-002` | WLT NPC | Ausstattung offen |
| VEH-DEC-KE-009 | NPC-Fahrzeuge haben einen groben Energiezustand; nahe Fahrzeuge tanken sichtbar, ferne ändern ihn abstrakt über Fahrtzyklen | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `N-LE-013` | NPC VKR | Umfang: `CQ-33` |
| VEH-DEC-KE-010 | Rastplätze und Haltepunkte an Fernstraßen sind nicht belegt; ihr Nutzen ergibt sich aus langen Fahrten mit spontanen Stopps und Trucking | 💡 VORSCHLAG | Gameplay-Systeme §6.1; `W-WI-005` | WLT FZ | `CQ-35` |
| VEH-DEC-KE-011 | Lade- und Tankzeiten sind nicht festgelegt | ❓ OFFEN | Gameplay-Systeme §6.6; `W-FZ-014` | FZ | `CQ-31` |
| VEH-DEC-KE-012 | Preisniveau, Preisschwankung und Verbindung zur Wirtschaft sind nicht belegt | 🔍 UNBELEGT | – | WIR | `CQ-32` |
| VEH-DEC-KE-013 | Zahl, Dichte und Platzierung von Tankstellen, Ladeparks und Rastplätzen sind nicht beschlossen und hängen am Straßennetz | ❓ OFFEN | – | WLT | `CQ-35` |
| VEH-DEC-KE-014 | Der Verbrauch hängt von Fahrzeug, Geschwindigkeit, Fahrstil, Beladung und Temperatur ab; die Reichweiten sind realistisch genug, dass das Tanken keine ständige Fleißarbeit ist (‚kein Tankstellen-Spam‘); Elektrofahrzeuge nutzen Batterie, Ladezustand und Laden | 📋 BESCHLOSSEN | `M-DC31` §34; `M-PS31` §47 | FZ | Grenzt `CQ-31` ein: Reichweiten sind beschlossen, Zeiten nicht |

---

## 5. Offene Punkte

| Frage | Verweis |
|---|---|
| Lade- und Tankzeiten, Reichweite, Verbrauch | `CQ-31` |
| Preismodell | `CQ-32` |
| Energiezustand von NPC-Fahrzeugen | `CQ-33` |
| Bezug von Rennkraftstoff und Spezialkraftstoffen | `CQ-34` |
| Platzierung von Tankstellen, Ladeorten, Rastplätzen | `CQ-35` |
