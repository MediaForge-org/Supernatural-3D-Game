# Fahrzeuge und Verkehr 08: Reparatur, Wartung und Werkstätten

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist das Prinzip: früh einfache Reparatur und einfaches Tuning, langfristig nahezu vollständige Wartungs- und Werkstattsimulation. Preise, Teile, Zeiten und die Tiefe des Tunings sind nicht festgelegt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Zustandsmodell: [Modul 07](07-Zustand-Verschleiss-und-Schaden.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Werkstattauftrag als Weltzustand](#2-werkstattauftrag-als-weltzustand)
3. [Wer repariert](#3-wer-repariert)
4. [Bergung, Abschleppen, Transport](#4-bergung-abschleppen-transport)
5. [Register](#5-register)
6. [Offene Punkte](#6-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Phasen | Reparatur, Tuning und Werkstatt früh einfach, langfristig nahezu vollständige Wartungs- und Werkstattsimulation | `W-FZ-012`; `D15-RV` |
| Werkstätten | spezialisierte Werkstätten für Reparatur, Leistung, Rennen, Gelände, Karosserie; ein schneller Neulack-Service ist nicht die volle Tuning-Werkstatt | `W-FZ-007`; Gameplay-Systeme §6.4 |
| Tuning | Leistungstuning ist wichtiger als kosmetisches Tuning; optisches Tuning gibt es, aber nicht so umfangreich wie Performance-Tuning | `W-FZ-008`; `23#1025E` Nr. 13 |
| Bergung | Abschleppdienst, Seilwinde, Bergefahrzeug, Crew oder anderes Fahrzeug als Bergung | `W-FZ-010`; Gameplay-Systeme §6.4 |
| Dienste | Abschleppdienst als NPC-System und möglicher Spielerjob (‚eher nicht so oft‘); Fahrzeugtransport (Abschleppwagen, Autotransporter, Anhänger) ist physisch nutzbar | `23#1025E` Nr. 41, 42; `W-TV-006` |
| Eigene Räume | Der Spieler kann Land kaufen und ein Haus mit Garage und Werkstatt planen | `W-UR-003` |
| Gewerbe | Händler, Tuning-Läden und Parkplätze gehören zum Gewerbeumfang | `W-UR-005` |
| Crew | Crewmitglieder reparieren Fahrzeuge im Hauptquartier; ‚mit echtem Fortschritt‘ ist eine Richtung | `N-CO-015` (📋), `N-CO-019` (🧭) |
| Offen | Wartungsökonomie, Teile, Preise, Tiefe | `W-FZ-014` |

Die Antwort des Entwicklers zur Tuning-Tiefe (Frage 12) lautet ‚mindestens so wenig‘ und ist nicht eindeutig lesbar. Sie ist als `CU-04` geführt und nicht in eine Aussage übersetzt.

---

## 2. Werkstattauftrag als Weltzustand

> **💡 VORSCHLAG.** Ein Auftrag in einer Werkstatt ist ein Weltzustand mit Zeitverlauf, nicht ein Menüschritt, der sofort wirkt. Das passt zu einer Welt, die weiterläuft, während man spielt (`W-TV-004`), und zu Dienstleistungen, die NPC-Personal brauchen.

```text
RepairOrder
  order_id, vehicle_id, workshop_ref
  requested_items[]       (Wartung / Austausch / Instandsetzung / Tuning)
  start_world_time, expected_end_world_time
  cost_agreed
  status                  (angenommen / in Arbeit / fertig / abgeholt / abgebrochen)
```

Eigenschaften, die sich daraus ergeben:

- Das Fahrzeug ist während des Auftrags nicht fahrbar; sein Standort ist die Werkstatt (Modul 22, `location_state`).
- Die Zeitspanne läuft in Weltzeit; sie kann kurz sein (früh: schnelle Reparatur) oder lang (Spätausbau).
- Die Werkstatt ist ein Arbeitsplatz für NPC-Mechaniker (`W-TV-002`); Auslastung und Öffnungszeiten ergeben sich aus deren Tagesabläufen, nicht aus einer eigenen Regel.

Welche Tiefe der Reparatur der Spieler **selbst** ausführt (Menü, Dienst mit Wartezeit, praktische Schraubarbeit), ist nicht beschlossen (`CQ-28`). Der Phasenvertrag trennt nur früh einfach und lang nahezu vollständig.

---

## 3. Wer repariert

| Akteur | Quelle | Aussage |
|---|---|---|
| Fachwerkstatt (NPC) | `W-FZ-007` | Reparatur, Leistung, Rennen, Gelände, Karosserie |
| Eigene Garage oder Werkstatt | `W-UR-003` | gebaut oder gekauft |
| Crewmitglied im HQ | `N-CO-015`, `N-CO-019` | repariert Fahrzeuge, im Entwurf mit echtem Fortschritt |
| Spieler | `W-FZ-012` | früh einfach, lang nahezu vollständig |
| Abschlepp- und Bergedienst | `W-TV-006` | stellt Fahrzeuge zurück auf die Straße |

Die Rolle der Crew bei Fahrzeugpflege (wer was darf, wie lange es dauert, ob der Spieler es beschleunigen kann) ist nicht beschrieben (`CQ-30`). Gleichzeitig soll Crew-Mikroverwaltung nicht erdrückend werden (`N-LT-034`).

---

## 4. Bergung, Abschleppen, Transport

Beschlossen ist die Existenz: Abschleppdienst, Winde, Bergefahrzeug und Bergung durch die Crew (`W-FZ-010`), dazu Transport von Fahrzeugen als physische Aufgabe (`23#1025E` Nr. 42). Aus den Anforderungen ergibt sich ein Lebenszyklus für gestrandete Fahrzeuge (Panne, Unfall, Abstellen, Abschleppen, Verwahrung), der für NPC- und Spielerfahrzeuge dieselbe Struktur haben kann. Dieser Lebenszyklus steht in [Modul 16](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md) und [Modul 22](22-Persistenz-und-Lebenszyklen.md).

---

## 5. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-WE-001 | Reparatur, Tuning und Werkstatt sind früh einfach und langfristig nahezu vollständig simuliert | 📋 BESCHLOSSEN | `W-FZ-012`; `D15-RV` | FZ WRK | Termine nicht beschlossen |
| VEH-DEC-WE-002 | Es gibt spezialisierte Werkstätten (Reparatur, Leistung, Rennen, Gelände, Karosserie); ein schneller Neulack-Service ist nicht die volle Tuning-Werkstatt | 📋 BESCHLOSSEN | `W-FZ-007`; Gameplay-Systeme §6.4 | FZ WRK | – |
| VEH-DEC-WE-003 | Leistungstuning ist wichtiger als kosmetisches Tuning; optisches Tuning existiert, aber weniger umfangreich | 📋 BESCHLOSSEN | `W-FZ-008`; `23#1025E` Nr. 13 | FZ | – |
| VEH-DEC-WE-004 | Abschleppdienst, Seilwinde, Bergefahrzeug sowie Crew oder anderes Fahrzeug als Bergung sind vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010`; Gameplay-Systeme §6.4 | FZ DIE | Bedienung offen |
| VEH-DEC-WE-005 | Abschleppdienst ist ein NPC-System und ein möglicher Spielerjob, aber ‚eher nicht so oft‘; Fahrzeugtransport mit Abschleppwagen, Autotransporter und Anhänger ist physisch nutzbar | 📋 BESCHLOSSEN | `23#1025E` Nr. 41, 42 | DIE FZ | Häufigkeit unscharf (`CU-05`) |
| VEH-DEC-WE-006 | Der Spieler kann ein Haus mit Garage und Werkstatt planen und bauen | 📋 BESCHLOSSEN | `W-UR-003` | IMM FZ | – |
| VEH-DEC-WE-007 | Crewmitglieder reparieren Fahrzeuge im Hauptquartier | 📋 BESCHLOSSEN | `N-CO-015` | NPC FZ | Echter Fortschritt: `N-CO-019` (🧭) |
| VEH-DEC-WE-008 | Ein Crewmitglied repariert Fahrzeuge mit echtem Fortschritt | 🧭 RICHTUNG | `N-CO-019` | NPC FZ | `CQ-30` |
| VEH-DEC-WE-009 | Die Antwort zur Tuning-Tiefe (‚mindestens so wenig‘) ist nicht eindeutig lesbar | 🔍 UNBELEGT | `23#1025E` Nr. 12 | FZ | `CU-04` |
| VEH-DEC-WE-010 | Ein Werkstattauftrag ist ein Weltzustand mit Start, erwartetem Ende, Kosten und Status; das Fahrzeug ist währenddessen nicht fahrbar | 💡 VORSCHLAG | Ableitung aus `W-TV-004`, `W-FZ-012` | FZ WRK PER | Format offen |
| VEH-DEC-WE-011 | Werkstätten sind Arbeitsplätze von NPC-Mechanikern; Öffnungszeiten und Auslastung folgen aus deren Tagesabläufen | 💡 VORSCHLAG | Ableitung aus `W-TV-002` | NPC WRK | – |
| VEH-DEC-WE-012 | Reparaturarten folgen den Zustandsschichten: Wartung (Verschleiß), Austausch (Defekt), Instandsetzung (Schaden) | 💡 VORSCHLAG | `VEH-DEC-ZV-008` | FZ WRK | – |
| VEH-DEC-WE-013 | Wartungsökonomie, Preise, Teilemarkt und Verfügbarkeit von Teilen sind nicht festgelegt | ❓ OFFEN | `W-FZ-014`; `M-OD` ‚Vehicles‘ | WIR FZ | `CQ-29` |
| VEH-DEC-WE-014 | Welche Reparaturschritte der Spieler selbst ausführt (Menü, Dienst mit Wartezeit, praktische Arbeit) ist nicht entschieden | ❓ OFFEN | `W-FZ-012` (nur früh und lang) | FZ UI | `CQ-28` |
| VEH-DEC-WE-015 | Werkstätten können sich spezialisieren: allgemeine Werkstatt, Leistung, Rennen, Gelände, Karosserie und Lack, Markenspezialisten; die Schnellwerkstatt repariert und lackiert grundlegend um, ist aber keine Volltuning-Werkstatt | 📋 BESCHLOSSEN | `M-DC31` §32; `M-PS31` §41 | FZ WRK | Ergänzt `VEH-DEC-WE-002` um Markenspezialisten |
| VEH-DEC-WE-016 | Eine Schnellwerkstatt kann gegebenenfalls die Fahndung nach einem Fahrzeug verringern (Umlackieren) | 🧭 RICHTUNG | `M-PS31` §41 (‚ggf. Fahndungsreduktion‘) | WRK POL | Wirkung auf Wiedererkennung: `CQ-73` |
| VEH-DEC-WE-017 | Optionale Arbeiten im Hauptquartier (Fahrzeugarbeit, Waffenpflege, Munition sortieren, Archiv ordnen) dürfen nicht zu verpflichtenden Fleißaufgaben werden | 📋 BESCHLOSSEN | `M-DC31` §3 | NPC WRK | Verhältnis zu `VEH-DEC-WE-008` |

---

## 6. Offene Punkte

| Frage | Verweis |
|---|---|
| Wie der Spieler repariert (Menü, Dienst, praktische Arbeit) | `CQ-28` |
| Preise, Teile, Teilemarkt | `CQ-29` |
| Rolle der Crew bei Fahrzeugpflege | `CQ-30` |
| Unklare Antworten zur Tuning-Tiefe und zur Häufigkeit des Abschleppdienstes | `CU-04`, `CU-05` |
