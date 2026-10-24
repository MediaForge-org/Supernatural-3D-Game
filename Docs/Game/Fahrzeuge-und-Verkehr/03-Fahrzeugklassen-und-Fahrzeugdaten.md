# Fahrzeuge und Verkehr 03: Fahrzeugklassen und Fahrzeugdaten

> [!IMPORTANT]
> **Es gibt keine Fahrzeugliste, keine Marken, keine Modelle und keine Werte.** Die Klassen sind in [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) erfasst und werden hier nicht wiederholt. Dieses Modul ergänzt, was das Fahrzeug- und Verkehrssystem zusätzlich braucht: eine Einordnung nach **Rolle im Verkehr** und ein **Datenmodell** für Typ und Exemplar. Beides ist Vorschlag. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Rolle im Verkehr statt nur Klasse](#2-rolle-im-verkehr-statt-nur-klasse)
3. [Datenmodell: Typ und Exemplar](#3-datenmodell-typ-und-exemplar)
4. [Fahrzeuge, die nicht auf der Straße fahren](#4-fahrzeuge-die-nicht-auf-der-straße-fahren)
5. [Register](#5-register)
6. [Offene Punkte](#6-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Spektrum | Autos, Motorräder, ATV/UTV, Schneemobile, Fahrräder, E-Scooter, Lkw, Boote, Kleinflugzeuge, Hubschrauber, Airliner als Passagierverkehr; keine Modellliste | `W-FZ-002` |
| Identität | Modell, Ausstattung, Motor, Getriebe, Antrieb, Baujahr, Optionen | `W-FZ-004` |
| Getriebe | manuell als Standard, manuelle Variante für jedes Fahrzeug, Automatiken separat | `W-FZ-005`, `W-FZ-006` |
| Markt | Gebrauchtmarkt mit Laufleistung, Historie, Verschleiß, versteckten Mängeln, Modifikationen | `W-FZ-003` |
| Namen | fiktive Namen, rechtliche Absicherung | `W-FZ-017` |
| Gelände | Supersportwagen sind nicht magisch geländetauglich | `W-FZ-009` |
| Antrieb | Verbrenner dominieren, E-Laden ist zulässig | `W-FZ-019` |
| Variantenlogik | Getriebe gehört zur Variante, kein beliebiger Menüwechsel; Trim-Familien einschließlich Police und Taxi/Fleet | `M-DC31` §30, §31 |
| Offen | Startsortiment, Marken, Modelljahre, Trims, Wartungsökonomie | `W-FZ-014` |

---

## 2. Rolle im Verkehr statt nur Klasse

> **💡 VORSCHLAG.** Die Quellen ordnen Fahrzeuge nach Bauart (Auto, Motorrad, Lkw). Für den Verkehr entscheidet aber die **Funktion**: Ein Krankenwagen, ein Taxi und ein Privatwagen derselben Bauart folgen anderen Regeln, haben andere Fahrziele und werden anders erkannt und angezeigt. Ein getrenntes Merkmal „Verkehrsrolle“ vermeidet, dass diese Unterschiede in Klassen oder Fahrzeugtypen hineinwandern.

| Verkehrsrolle (Beispielliste, nicht beschlossen) | Bezug in den Quellen |
|---|---|
| Privatfahrzeug | `N-ID-005` (NPCs benutzen wiederholt ihr Fahrzeug) |
| Berufs- und Lieferfahrzeug, Lkw | `W-WI-005`, `W-VK-011` (Trucking, Lieferung, Güterverkehr) |
| Taxi, Mitfahrdienst | `W-WI-005` |
| Linienbus | `W-VK-014`, [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) |
| Einsatzfahrzeug (Polizei, Rettung, Feuerwehr, Abschleppdienst, Bergrettung) | `W-TV-006`, `N-LE-024` |
| Wartungs- und Baustellenfahrzeug | `W-UR-012`, `N-LE-025` |
| Zweirad, Rad, E-Scooter | `W-FZ-002` |
| Rennfahrzeug | [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md) |
| Spielerfahrzeug, Begleiterfahrzeug | `N-CO-005` |

Die Rolle bestimmt: erlaubte Straßen, Verhalten an Kreuzungen und bei Rettungsgassen, Sonderrechte, Verfügbarkeit für Diebstahl, Sichtbarkeit in Fahndungen und Zugehörigkeit zu einem Besitzer (Haushalt, Firma, Stadt).

---

## 3. Datenmodell: Typ und Exemplar

> **💡 VORSCHLAG.** Zwei Ebenen. Die Namen sind Arbeitsnamen.

**Fahrzeugtyp** (statisch, versioniert, austauschbar, moddingfähig):

```text
VehicleType
  type_id            stabil, nicht aus Anzeigenamen abgeleitet
  family, trim, model_year_range
  body / layout / drive (Antrieb) / powertrain_ref / transmission_options[]
  wear_part_set      (Verschleißteile gemäß W-FZ-008, Tiefe offen)
  traffic_roles[]    erlaubte Verkehrsrollen
  road_class_access  erlaubte Straßenklassen
  dimension / mass_class
  visual_conspicuousness   (Auffälligkeit für Wiedererkennung, N-GE-018)
  simulation_lod_support   (welche Stufen aus Modul 26 der Typ unterstützt)
  source_version
```

**Fahrzeugexemplar** (veränderlich, persistent):

```text
Vehicle
  vehicle_id         stabil (W-FZ-016)
  type_id, build_year, option_set
  owner_ref          Verweis auf Haushalt, Firma, NPC, Spieler (Modul 18)
  plate_ref          Kennzeichen (Modul 18)
  odometer, history_events[]
  wear_state, damage_state
  fuel_or_charge
  location_state     geparkt / fährt / abgeschleppt / in Werkstatt / offscreen
  inventory_ref      Kofferraum (Modul 06)
  flags              gestohlen, gesucht, beschlagnahmt, ...
```

**Begründung der Trennung:**

1. Der Typ ändert sich durch Inhalts- und Balance-Updates, das Exemplar durch Spielgeschehen. Beides getrennt zu speichern hält Speicherstände klein und migrierbar (`W-TV-011` und `CK-04` zur Speichergröße).
2. Ein Typ kann für den Verkehr bereitstehen, ohne fahrbar zu sein oder in einem Shop zu erscheinen. Die Frage, ob Verkehrsfahrzeuge dieselben Typen wie fahrbare Fahrzeuge sind, ist offen (`CQ-09`).
3. Der Typ muss für Mods austauschbar sein (`M-MOD`: Mods ändern das Basisspiel nicht). Wie das konkret aussieht, ist offen.

**Nicht belegt:** Felder des Datenmodells, Wertebereiche, Detailtiefe der Verschleißteile (`VEH-DEC-ZV-…`), Anzahl der Typen. Das Beispiel ‚250 Fahrzeugtypen Metropolis, 120 ländlich‘ (`W-FZ-018`) ist ein Assistentenbeispiel und keine Entscheidung.

---

## 4. Fahrzeuge, die nicht auf der Straße fahren

Boote, Flugzeuge, Hubschrauber und Schienenfahrzeuge sind beschlossen (`W-VK-011`, `W-VK-012`, `W-VK-004`) und werden in [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) und [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) behandelt. Dieser Ordner betrachtet sie nur als **Besitz- und Persistenzfälle** (Modul 18 und 22) und als Teilnehmer an Veranstaltungen und Sperrungen (Modul 15). Die Straßenverkehrsregeln gelten für sie nicht.

---

## 5. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-KL-001 | Breites Fahrzeugspektrum über Jahrzehnte (Autos, Motorräder, ATV/UTV, Schneemobile, Fahrräder, E-Scooter, Lkw, Boote, Kleinflugzeuge, Hubschrauber, Airliner als Passagierverkehr) | 📋 BESCHLOSSEN | `W-FZ-002` | FZ | Keine Modellliste |
| VEH-DEC-KL-002 | Fahrzeugidentität aus Modell, Ausstattung, Motor, Getriebe, Antrieb, Baujahr und Optionen; Modellfamilien können Hand-, Automatik-, Doppelkupplungs-, sequenzielle und Rallye-/Homologationsvarianten haben | 📋 BESCHLOSSEN | `W-FZ-004` | FZ | – |
| VEH-DEC-KL-003 | Fahrzeuge tragen fiktive Namen und Marken | 📋 BESCHLOSSEN | `W-FZ-017` | FZ REC | Welche Autos und Marken es gibt, ist offen (`CQ-06`) |
| VEH-DEC-KL-004 | Gebrauchtwagenmarkt mit Laufleistung, Historie, Verschleiß, versteckten Mängeln und Modifikationen | 📋 BESCHLOSSEN | `W-FZ-003` | FZ WIR | Umsetzung in Modul 07 |
| VEH-DEC-KL-005 | Supersportwagen sind nicht magisch geländetauglich; Geländetauglichkeit unterscheidet sich real | 📋 BESCHLOSSEN | `W-FZ-009` | FZ PHY | Auswirkung auf zugelassene Straßenklassen: 💡 `VEH-DEC-KL-009` |
| VEH-DEC-KL-006 | Manuelles Getriebe ist Standard, andere Versionen sind kaufbar; für jedes Fahrzeug existiert eine manuelle Variante | 📋 BESCHLOSSEN | `W-FZ-005`, `W-FZ-006` | FZ | Verhältnis zu Fahrzeugen ohne Schaltgetriebe: `CU-01` |
| VEH-DEC-KL-007 | Startsortiment, Marken, Modelljahre, Trims, Wartungsökonomie und Vertrautheitsgrenze sind nicht festgelegt | ❓ OFFEN | `W-FZ-014`; `M-OD` ‚Vehicles‘ | FZ | `CQ-06` |
| VEH-DEC-KL-008 | Neben der Fahrzeugklasse wird eine **Verkehrsrolle** geführt (Privat, Lieferung, Taxi, Bus, Einsatz, Wartung, Rennen, Spieler), die Straßenzugang, Sonderrechte und Verhalten bestimmt | 💡 VORSCHLAG | Ableitung aus `W-TV-006`, `W-WI-005`, `N-LE-024` | FZ VKR | Liste und Zuschnitt offen: `CQ-09` |
| VEH-DEC-KL-009 | Der Fahrzeugtyp trägt eine Liste erlaubter Straßenklassen, damit Geländetauglichkeit und Verkehrsrolle im Netz abbildbar sind | 💡 VORSCHLAG | Ableitung aus `W-FZ-009`, `WF-D03` (Klassen) | FZ SN | – |
| VEH-DEC-KL-010 | Typ (statisch, versioniert, austauschbar) und Exemplar (veränderlich, persistent) werden getrennt geführt | 💡 VORSCHLAG | `W-FZ-016`; `M-MOD` | FZ PER | Felder und Format offen |
| VEH-DEC-KL-011 | Der Fahrzeugtyp trägt ein Auffälligkeitsmerkmal, das die Wiedererkennung durch NPCs und Zeugen speist | 💡 VORSCHLAG | `N-GE-018`; `E-SP-011` | NPC ERM | Schwellen offen |
| VEH-DEC-KL-012 | Die Anzahl der Fahrzeugtypen ist nicht beschlossen; das Beispiel 250/120 aus dem Assistentenvorschlag ist Illustration | 💡 VORSCHLAG | `W-FZ-018` | FZ | – |
| VEH-DEC-KL-013 | Antriebsarten jenseits Verbrenner und Elektro (Hybrid, Wasserstoff) sind nicht belegt | 🔍 UNBELEGT | `W-FZ-019` nennt nur Verbrenner und E-Laden | FZ | `CQ-07` |
| VEH-DEC-KL-014 | Spezialfahrzeuge der Hunter sowie Rettungs- und Feuerwehrfahrzeugtypen sind nicht belegt; Polizei- und Taxi/Fleet-Varianten nennt der Master als Trim-Familien | 🔍 UNBELEGT | [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §9; `M-PS31` §40 (Familien) | FZ | Verkehrsrolle bleibt davon unberührt; Trim-Familien: `VEH-DEC-KL-017` |
| VEH-DEC-KL-015 | Luft-, Wasser- und Schienenfahrzeuge sind Besitz- und Persistenzfälle dieses Ordners, ihre Betriebsregeln liegen in der Welt-Dokumentation | 💡 VORSCHLAG | `W-VK-004`, `W-VK-011`, `W-VK-012` | FZ PER | – |
| VEH-DEC-KL-016 | Das Getriebe gehört zur konkreten Fahrzeugvariante: Man kauft nicht ein Fahrzeug und schaltet es beliebig per Menü zwischen Handschaltung, Doppelkupplung und Automatik um; Handschaltung, Wandlerautomatik, Doppelkupplung, sequenzielle und weitere Getriebearten können nebeneinander bestehen | 📋 BESCHLOSSEN | `M-DC31` §30; `M-PS31` §39; `W-FZ-004` | FZ | Ergänzt `VEH-DEC-KL-006` |
| VEH-DEC-KL-017 | Fahrzeugfamilien können Economy, Sport, GT, Track Edition, Rally Homologation, Rally Version, Off-Road, Police, Taxi/Fleet, Utility, Luxury, Performance, Limited und Modelljahrvarianten enthalten; auch Kleinwagen können echte Rallye- und Homologationsversionen mit anderem Antrieb, Bremsen, Fahrwerk, Übersetzung, Verstärkung und Aero haben | 📋 BESCHLOSSEN | `M-DC31` §31; `M-PS31` §40 | FZ RAC | Liste nicht abschließend (‚können‘) |

---

## 6. Offene Punkte

| Frage | Verweis |
|---|---|
| Startsortiment, Marken, Modelljahre, Trims | `CQ-06` |
| Antriebsarten und ihr Anteil | `CQ-07` |
| ‚Manuelle Variante für jedes Fahrzeug‘ bei Fahrzeugen ohne Schaltgetriebe | `CQ-08`, `CU-01` |
| Sind Verkehrsfahrzeuge dieselben Typen wie fahrbare Fahrzeuge, und wie wird die Verkehrsrolle zugeschnitten | `CQ-09` |
