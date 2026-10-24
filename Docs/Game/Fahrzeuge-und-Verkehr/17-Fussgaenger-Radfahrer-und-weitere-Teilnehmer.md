# Fahrzeuge und Verkehr 17: Fußgänger, Radfahrer und weitere Verkehrsteilnehmer

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass es Fahrräder, E-Bikes, E-Scooter (mit Sharing), tiefer simulierte Motorräder, ATVs und Schneemobile gibt und dass Menschenmengen an bestimmten Orten wesentlich sind. Zu Fußgängern im Straßenverkehr, zu Gehwegen und Radwegen und zu den Folgen von Kollisionen mit Menschen sagen die Quellen nichts. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [Modul 10](10-Strassennetz-und-Strassentypen.md), [Modul 11](11-Kreuzungen-Signale-und-Schilder.md), [Modul 13](13-Verkehrs-KI-und-Navigation.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Verkehrsteilnehmer und Straßenzugang](#2-verkehrsteilnehmer-und-straßenzugang)
3. [Fußgänger](#3-fußgänger)
4. [Zweiräder, Scooter, Geländefahrzeuge](#4-zweiräder-scooter-geländefahrzeuge)
5. [Tiere](#5-tiere)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Fahrräder | Fahrräder gibt es (Stadtrad, Mountainbike, Rennrad, BMX standen zur Frage) | `23#1025E` Nr. 24 (Antwort ‚gibt es‘) |
| E-Bikes | ja, aber seltener | `23#1025E` Nr. 25 |
| E-Scooter | Sharing-Systeme und private Scooter: ok | `23#1025E` Nr. 26 |
| Motorräder | tiefer als in typischen Open-World-Spielen, so wie bei Autos | `23#1025E` Nr. 27 |
| Geländefahrzeuge | ATVs/UTVs für ländliche, Wald- und Bergregionen, Schneemobile für die kalte Bergregion | `23#1025E` Nr. 28, 29; `W-FZ-002` |
| Menschenmengen | wesentlich an Stadien, Festivals, Nachtleben, Downtown, Skiorten, Motorsport und Parks; Crowd und Verkehr nutzen LOD | `W-UR-011` |
| Logik | europäisch geprägte Fußgänger- und Verkehrslogik mit nordamerikanischem Straßenmaßstab | `W-UR-015` (🧭) |
| Tiere | Wildtiere, Haustiere, Nutztiere, Pferde, Wildunfälle | `M-WB17` Block 6 |

---

## 2. Verkehrsteilnehmer und Straßenzugang

> **💡 VORSCHLAG.** Jeder Teilnehmertyp hat eine **Verkehrsrolle** (Modul 03). Das Straßenprofil (Modul 10, Feld `allowed_roles`) legt fest, wo er sich bewegen darf:

| Teilnehmer | Typischer Raum (Arbeitsannahme, nicht beschlossen) |
|---|---|
| Fußgänger | Gehwege, Überwege, Plätze, Wohnstraßen; keine Autobahn |
| Fahrrad, E-Bike | Straße, Radweg, Seitenstreifen; keine Autobahn |
| E-Scooter | wie Fahrrad; Gehweg je nach Regel (offen) |
| Motorrad | Straße, Autobahn, Rennstrecke |
| ATV, UTV, Schneemobil | Feld, Wald, Berg, Pisten, Wege; öffentliche Straße nur eingeschränkt |
| Tiere | Wildwechsel, Weiden, Ortsrand |

Die Karte kennt Gehwege und Radwege nach heutigem Stand nicht: `WF-RC01` führt Spuren, Seitenstreifen und Breite, aber keine Fußgänger- oder Radinfrastruktur. Wenn das Spiel sie braucht, ist es ein Wunsch an die Schnittstelle ([Modul 24](24-WorldForge-Schnittstelle.md), `CQ-71`).

---

## 3. Fußgänger

Fußgänger sind NPCs mit Alltagsablauf. Sie erscheinen als S1- oder S0-Agenten in Spielernähe, als Menge (SD) in dichten Orten ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md), `W-UR-011`).

> **💡 VORSCHLAG.**
>
> - **Querungen:** An Überwegen und Signalen (Modul 11) queren Fußgänger nach Regel; ihre Regeltreue folgt Persönlichkeit und Lage, wie bei Fahrern.
> - **Vorrang und Reaktion:** Die Verkehrs-KI kennt Fußgänger als Konfliktpartner an Überwegen und Einmündungen. Fußgänger reagieren auf Fahrzeuge (Ausweichen, Stehenbleiben, Flucht bei Gefahr).
> - **Verkehrsmenge:** Passanten der Verkehrsmenge sind wie Fahrzeuge ohne Identität, bis sie beteiligt sind (Beförderung, [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md) §5).
> - **Konsequenzen:** Die Verletzung oder der Tod von Passanten durch Fahrzeuge folgt dem Verletzungssystem und dem Konsequenzsystem (Zeugen, Notruf, Polizei). Das Verhalten des Spielers entscheidet, ob eine Situation harmlos endet oder eskaliert (`N-BH-003`). Die Regeln für Fußgängerunfälle sind nicht beschrieben (`CQ-70`).

Zur Darstellung von Minderjährigen und Gewalt gegen sie gibt es offene Fragen und eine nicht aufgelöste Spannung im Kampfsystem ([Kampfsystem 28](../Kampfsystem/28-Konflikte-und-offene-Fragen.md)). Die Dokumentation verengt das nicht; sie führt es als `CQ-72`.

---

## 4. Zweiräder, Scooter, Geländefahrzeuge

- **Motorräder** sind tiefer simuliert als bei vergleichbaren Spielen (`23#1025E` Nr. 27; Klassen und Rennsport: [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md)). Die Fahrphysik eines Zweirads (Balance, Kippen) ist ein eigener Fall; Wiedergabetreue für Zweiräder, Lkw und Geländefahrzeuge ist offen (`CQ-11`).
- **E-Scooter-Sharing** ist ein Weltdienst: Scooter stehen im Raum, werden genommen und abgestellt. 💡 Als Bestand je Zone mit Abstraktion in der Ferne, nicht als einzelne persistente Fahrzeuge überall. Menge und Standorte sind nicht belegt.
- **Geländefahrzeuge** (ATV/UTV, Schneemobil) sind besitzbare Fahrzeuge (`W-FZ-002`) und bewegen sich abseits des Straßennetzes; die Verkehrs-KI und die Navigation auf Straßen decken sie nicht ab. Das Gelände-Navigationsnetz ist offen.

---

## 5. Tiere

Wildtiere, Haustiere, Nutztiere und Pferde gehören zur Welt, Wildunfälle (‚roadkill‘) ebenfalls (`M-WB17` Block 6). Wie Tiere die Straße queren und welche Folgen eine Kollision hat, ist nicht beschrieben. Wildwechselzonen sind ein Vorschlag (`VEH-DEC-SN-012`). Reiter und Tierführer im Straßenverkehr sind nicht erwähnt.

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-VT-001 | Fahrräder gibt es; E-Bikes sind ebenfalls vorgesehen, aber seltener | 📋 BESCHLOSSEN | `23#1025E` Nr. 24, 25 | FZ VKR | – |
| VEH-DEC-VT-002 | E-Scooter gibt es als Sharing-Systeme und als private Scooter | 📋 BESCHLOSSEN | `23#1025E` Nr. 26 | FZ WLT | Menge, Standorte offen |
| VEH-DEC-VT-003 | Motorräder sind tiefer simuliert als in typischen Open-World-Spielen, so wie Autos | 📋 BESCHLOSSEN | `23#1025E` Nr. 27 | FZ PHY | Wiedergabetreue: `CQ-11` |
| VEH-DEC-VT-004 | ATVs und UTVs für ländliche, Wald- und Bergregionen sowie Schneemobile für die kalte Bergregion sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 28, 29; `W-FZ-002` | FZ | Geländenavigation offen |
| VEH-DEC-VT-005 | Menschenmengen sind an Stadien, Festivals, Nachtleben, Downtown, Skiorten, Motorsport und Parks wesentlich; Crowd und Verkehr nutzen LOD | 📋 BESCHLOSSEN | `W-UR-011` | NPC LS | – |
| VEH-DEC-VT-006 | Fußgänger- und Verkehrslogik ist europäisch geprägt, der Straßenmaßstab nordamerikanisch | 🧭 RICHTUNG | `W-UR-015` | WLT SN | Für die 70-km²-Linie formuliert |
| VEH-DEC-VT-007 | Der Straßenzugang jedes Teilnehmertyps folgt aus der Verkehrsrolle und dem Straßenprofil (`allowed_roles`) | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-KL-008`, `VEH-DEC-SN-009` | SN VKR | – |
| VEH-DEC-VT-008 | Fußgänger sind NPCs mit Alltagsablauf; Querungen folgen Regeln, die Regeltreue folgt der Persönlichkeit; die Verkehrs-KI kennt Fußgänger als Konfliktpartner | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `W-UR-011` | NPC KI | – |
| VEH-DEC-VT-009 | E-Scooter-Sharing wird als Bestand je Zone modelliert und in der Ferne abstrahiert | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 26 | WLT LS | – |
| VEH-DEC-VT-010 | Gehweg- und Radwegdaten fehlen in der Kartenschnittstelle nach heutigem Stand | 🧩 ABGELEITET | `WF-RC01` (Felder) | WF SN | Wunsch an die Schnittstelle: `CQ-71` |
| VEH-DEC-VT-011 | Regeln für Kollisionen mit Fußgängern und Tieren (Verletzung, Tod, Recht, Reaktion der Umgebung) sind nicht beschrieben | ❓ OFFEN | `N-BH-003` (nur allgemeine Eskalation) | KMP POL | `CQ-70` |
| VEH-DEC-VT-012 | Darstellung und Verhalten von Kindern und Minderjährigen im Verkehr hängen an einer im Kampfsystem offenen Frage | ❓ OFFEN | [Kampfsystem 28](../Kampfsystem/28-Konflikte-und-offene-Fragen.md); `M-OD` ‚Combat / injury / gore‘ | KMP NPC | `CQ-72` |
| VEH-DEC-VT-013 | Reiter, Tierführer und Kutschen im Straßenverkehr sind nicht erwähnt | 🔍 UNBELEGT | – | NPC | – |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Fußgänger- und Tierunfälle | `CQ-70` |
| Gehweg- und Radwegdaten in der Kartenschnittstelle | `CQ-71` |
| Minderjährige im Verkehr | `CQ-72` |
| Wiedergabetreue für Zweiräder, Lkw, Geländefahrzeuge | `CQ-11` |
