# Fahrzeuge und Verkehr 16: Unfälle, Pannen, Einsatzfahrzeuge und Sonderverkehr

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass NPCs selten Unfälle verursachen (außer der Spieler rammt sie), dass Fahrzeuge liegenbleiben können, dass Notdienste in geeigneter Tiefe simuliert werden und dass der Verkehr Rettungsfahrzeugen Platz macht. Raten, Zeiten und die Tiefe der Dienste sind nicht festgelegt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [Modul 07](07-Zustand-Verschleiss-und-Schaden.md), [Modul 19](19-Polizei-und-Verkehrsueberwachung.md), [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Unfall als Ereignis](#2-unfall-als-ereignis)
3. [Pannen](#3-pannen)
4. [Einsatzkette der Dienste](#4-einsatzkette-der-dienste)
5. [Einsatzfahrzeuge im Verkehr](#5-einsatzfahrzeuge-im-verkehr)
6. [Sonderverkehr: Treffen, Straßenrennen, Trackdays](#6-sonderverkehr-treffen-straßenrennen-trackdays)
7. [Register](#7-register)
8. [Offene Punkte](#8-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Unfälle | NPCs können Unfälle verursachen, Polizei, Rettung und Abschleppdienst reagieren; selten, außer der Spieler rammt die NPC-Fahrzeuge | `23#1025E` Nr. 3; `W-TV-001` |
| Pannen | NPC-Fahrzeuge können liegenbleiben, Reifenpannen haben, überhitzen | `23#1025E` Nr. 4 (Antwort ‚ja schon‘) |
| Dienste | Feuerwehr, Rettung, Polizei, Abschleppdienst, Bergrettung und Wasserrettung werden in geeigneter Tiefe simuliert | `W-TV-006`; `E-EV-001` |
| Platz machen | Der Verkehr macht Rettungsfahrzeugen Platz; Rettungsgassen; NPC-Kriminelle können von der Polizei verfolgt werden | `E-PO-009`; `N-LE-024` |
| Notruf | Zeugen rufen den Notruf, mit Latenz, Täterbeschreibung und Kennzeichenabfrage | `E-RE-010`; `N-WA-011` |
| Kontrollen | Verkehrskontrollen (normale Stops oder Verfolgung je nach Lage) | `N-BH-003` |
| Folgen | Crash-Energie und Sicherheitsausrüstung verzahnen sich mit dem Verletzungssystem | `W-FZ-015` |
| Zwischenfälle | Dynamische Zwischenfälle verursachen dauerhafte Schäden, NPC-Trupps reparieren | `W-UR-012` |
| Treffen und Rennen | Autotreffen, Straßenrennen (illegal, getrennt vom professionellen Motorsport) und Trackdays auf den richtigen Strecken sind wichtig | `23#1025E` Nr. 43, 44, 45; `W-UR-009` |
| Offen | Raten von Unfällen und Pannen, Einsatzzeiten, Tiefe der Dienste | – |

---

## 2. Unfall als Ereignis

> **💡 VORSCHLAG.** Ein Unfall ist ein **Ereignis** mit klarer Zuständigkeitsverteilung, damit kein System die Folgen eines anderen nachbaut.

```text
TrafficIncident
  incident_id, world_time, location_ref (Kante, Position)
  cause              (Fahrerfehler, Technik, Wetter, Spieler, Straftat, ...)
  parties[]          (Fahrzeuge, Insassen, Fußgänger)
  severity_inputs    (Energie, Geschwindigkeit, Schutzausrüstung)
```

| Folge | Zuständig |
|---|---|
| Fahrzeugschaden | [Modul 07](07-Zustand-Verschleiss-und-Schaden.md) |
| Verletzung der Insassen | [Kampfsystem 13](../Kampfsystem/13-Verletzungen-Schmerz-und-Kampffolgen.md), `W-FZ-015` |
| Kapazitätsverlust, Stau | [Modul 15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md) |
| Zeugen, Notruf, Beweise | [Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md), `E-RE-010` |
| Einsatzkette | Abschnitt 4 |
| Strafbarkeit (Fahrerflucht, Verursacher) | [Modul 19](19-Polizei-und-Verkehrsueberwachung.md) |
| Wrack, Abschleppen | [Modul 22](22-Persistenz-und-Lebenszyklen.md) |

**Tod bei Unfällen:** Wichtige NPCs sterben nicht durch banale Simulationsfehler (`N-LT-033`); der Tod von Spieler und gewöhnlichen NPCs durch Unfälle folgt dem Verletzungssystem und dem noch offenen Todesloop ([Kampfsystem 14](../Kampfsystem/14-Bewusstlosigkeit-Tod-und-Rettung.md)).

**Spieler im Unfall:** Der Spieler kann Verursacher, Opfer, Zeuge oder Helfer sein. Dass er Zeuge werden kann, ist beschlossen (`COMBAT-DEC-PO-006`); die übrigen Rollen und ihre Regeln sind nicht beschrieben (`CQ-67`).

---

## 3. Pannen

Die Pannenarten ergeben sich aus dem Fahrzeugzustand (Modul 07) und aus Energie (Modul 09).

> **💡 VORSCHLAG.** Pannen entstehen aus **Zustand und Belastung**: Reifendruck und -zustand, Kühlung, Batterie, Kupplung, Kraftstoff. Ein Fahrzeug nahe am Ausfall bekommt eine Wahrscheinlichkeit pro Zeit oder Strecke, deren Zufall deterministisch aus Fahrzeug-ID, Seed und Zeitfenster abgeleitet wird (`VEH-DEC-KI-010`). So sind Pannen reproduzierbar, erklärbar (das Fahrzeug war marode) und für NPC- und Spielerfahrzeuge identisch. Ein liegengebliebenes NPC-Fahrzeug ist eine Kapazitätsminderung und ein möglicher Anlass für Hilfe oder Abschleppen (`W-FZ-010`).

---

## 4. Einsatzkette der Dienste

Beschlossen ist, dass die Dienste in geeigneter Tiefe simuliert werden und dass der Notruf mit Latenz erfolgt. Die Zuordnung zu Wachen, Fahrzeugen und Personal ist offen.

> **💡 VORSCHLAG.** Jeder Einsatz folgt einer kleinen Zustandsfolge:
>
> ```text
> Meldung -> Alarmierung -> Anfahrt -> Vor Ort -> Räumung -> Wiederherstellung
> ```
>
> - **Meldung:** durch Zeugen (`E-RE-010`) mit Latenz.
> - **Anfahrt:** Fahrzeug des Dienstes aus Wache oder Depot; Dauer aus Graph und Verkehrslage; Sonderrechte (Abschnitt 5).
> - **Vor Ort:** Absperrung, Erstversorgung, Spurensicherung (Ermittlung), Bergung.
> - **Räumung:** Abschleppen, Reinigung; Tatort bleibt bis zur Reinigung (`COMBAT-DEC-PO-004`).
> - **Wiederherstellung:** Straße öffnet, Kapazität steigt.
>
> Je Dienst gibt es eine Zuständigkeit und eine Dauer (Zahlen nicht belegt, `CQ-65`). Mehrere Dienste an einem Ort sind parallele Teilfolgen. Die Fahrzeuge der Dienste sind Fahrzeuge mit Verkehrsrolle ‚Einsatz‘ und gehören einer Behörde (Modul 03, 18).

Die Tiefe der Dienste (Wachen, Besatzungen, Hubschrauber, Krankenhauseinlieferung) ist unentschieden (`CQ-66`). Weitere Ereignisse und Notfälle der Welt: [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md).

---

## 5. Einsatzfahrzeuge im Verkehr

> **💡 VORSCHLAG.** Einsatzfahrzeuge tragen ein **Sonderrechte-Merkmal** (Blaulicht und Signal an oder aus). Mit Sonderrechten darf die Verkehrs-KI sie vor bestimmte Regeln stellen (Rotlicht, Tempo); andere Fahrer reagieren nach Fahrstil: Rettungsgasse bilden (beschlossen, `N-LE-024`), rechts anhalten, im Stau Platz machen. Wie die Regeln der Spielwelt Sonderrechte regeln, ist nicht belegt (`CQ-42`). Verfolgungen von NPC-Kriminellen durch die Polizei sind beschlossen (`N-LE-024`); ihre Eskalationsstufen im Verkehr liegen in Modul 19.

---

## 6. Sonderverkehr: Treffen, Straßenrennen, Trackdays

| Gegenstand | Stand |
|---|---|
| Autotreffen | spontan und organisiert durch NPCs, ‚ja auf jeden fall‘; Teil der Veranstaltungen (`23#1025E` Nr. 43; `W-UR-009`) |
| Straßenrennen | illegale Rennen sind wichtig, getrennt vom professionellen Motorsport (`23#1025E` Nr. 44); illegale Rennen gehören zur unabhängig vom Spieler laufenden Kriminalität (`M-WB89` Block 8) |
| Trackdays | eigene Autos auf Rennstrecken, ‚aber nur auf den richtigen strecken‘ (`23#1025E` Nr. 45) |
| Rennstrecken, Motorsport | [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md) |
| Regeln, Wetten, Polizeifolgen der Straßenrennen | nicht beschrieben (`CQ-68`) |

Treffen und Straßenrennen sind Quellen **lokaler Verkehrsverdichtung und Sperrung** (Modul 15) und ein Ort für Zeugen und Verfolgungen (Modul 19).

---

## 7. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-UP-001 | NPCs können Unfälle verursachen, Polizei, Rettung und Abschleppdienst reagieren; Unfälle sind selten, außer der Spieler rammt die NPCs | 📋 BESCHLOSSEN | `23#1025E` Nr. 3; `W-TV-001` | VKR DIE | Rate nicht belegt: `CQ-65` |
| VEH-DEC-UP-002 | NPC-Fahrzeuge können liegenbleiben (Panne, Reifenpanne, Überhitzung) | 📋 BESCHLOSSEN | `23#1025E` Nr. 4; `W-TV-001` | VKR | Rate: `CQ-65` |
| VEH-DEC-UP-003 | Feuerwehr, Rettung, Polizei, Abschleppdienst, Bergrettung und Wasserrettung werden in geeigneter Tiefe simuliert | 📋 BESCHLOSSEN | `W-TV-006`; `E-EV-001` | DIE | Tiefe: `CQ-66` |
| VEH-DEC-UP-004 | Der Verkehr macht Rettungsfahrzeugen Platz; NPC-Kriminelle können von der Polizei verfolgt werden | 📋 BESCHLOSSEN | `E-PO-009`; `N-LE-024` | VKR POL | – |
| VEH-DEC-UP-005 | Zeugen setzen einen Notruf mit Latenz, Täterbeschreibung (Kleidung, Fahrzeug, Kennzeichen, Richtung) und Kennzeichenabfrage ab | 📋 BESCHLOSSEN | `E-RE-010`; `N-WA-011`; `N-BH-003` | NPC POL | – |
| VEH-DEC-UP-006 | Autotreffen, Straßenrennen (illegal, getrennt vom Profi-Motorsport) und Trackdays (nur auf den richtigen Strecken) sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 43, 44, 45 | WLT RAC | Regeln: `CQ-68` |
| VEH-DEC-UP-007 | Ein Unfall ist ein Ereignis mit Ursache, Beteiligten und Schwere-Eingaben; Folgen laufen an die zuständigen Systeme (Fahrzeugschaden, Verletzung, Kapazität, Zeugen, Einsatz, Strafbarkeit, Wrack) | 💡 VORSCHLAG | Ableitung aus `W-FZ-015`, `E-RE-010`, `W-UR-012` | VKR ALL | – |
| VEH-DEC-UP-008 | Pannen entstehen aus Zustand und Belastung; der Zufall ist deterministisch aus Fahrzeug-ID, Seed und Zeitfenster abgeleitet | 💡 VORSCHLAG | Ableitung aus `W-TV-001`, `VEH-DEC-KI-010` | FZ VKR | – |
| VEH-DEC-UP-009 | Jeder Einsatz folgt der Zustandsfolge Meldung, Alarmierung, Anfahrt, Vor Ort, Räumung, Wiederherstellung; Dienste laufen als parallele Teilfolgen | 💡 VORSCHLAG | Ableitung aus `E-RE-010`, `W-TV-006` | DIE | Dauern: `CQ-65` |
| VEH-DEC-UP-010 | Einsatzfahrzeuge tragen ein Sonderrechte-Merkmal; andere Fahrer reagieren nach Fahrstil (Rettungsgasse, Anhalten) | 💡 VORSCHLAG | Ableitung aus `N-LE-024` | VKR KI | Sonderrechte-Regeln: `CQ-42` |
| VEH-DEC-UP-011 | Eine Unfallstelle bleibt als Weltobjekt (Spuren, Wrack) bis zur Räumung | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-PO-004` | WLT ERM | – |
| VEH-DEC-UP-012 | Fahrerflucht (Entfernen vom Unfallort) ist ein Delikt, das über Zeugen, Kennzeichen und Beschreibung verfolgt wird | 💡 VORSCHLAG | Ableitung aus `E-RE-010`; Rechtslage offen | POL ERM | `CQ-80` |
| VEH-DEC-UP-013 | Der Spieler kann im Unfall Zeuge werden; weitere Rollen (Verursacher, Helfer, Opfer) und ihre Regeln sind nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-PO-006` | SPI POL | `CQ-67` |
| VEH-DEC-UP-014 | Unfall- und Pannenraten, Einsatzzeiten und die Tiefe der Dienste (Wachen, Besatzungen, Hubschrauber) sind nicht belegt | 🔍 UNBELEGT | – | DIE VKR | `CQ-65`, `CQ-66` |
| VEH-DEC-UP-015 | Katastrophenlagen auf Straßen (Hochwasser, Sturm, Waldbrand, Evakuierung) sind als Weltereignisse genannt, ihre Wirkung auf den Verkehr ist nicht beschrieben | 🔍 UNBELEGT | `M-WB17` Block 6 | WET VKR | `CQ-69` |

---

## 8. Offene Punkte

| Frage | Verweis |
|---|---|
| Unfall- und Pannenraten, Einsatzzeiten | `CQ-65` |
| Tiefe der Dienste | `CQ-66` |
| Spielerrollen im Unfall | `CQ-67` |
| Regeln der Straßenrennen | `CQ-68` |
| Katastrophen und Evakuierungen | `CQ-69` |
