# Fahrzeuge und Verkehr 11: Kreuzungen, Signale und Schilder

> [!IMPORTANT]
> **Die Quellen sagen zu Kreuzungen, Signalen, Schildern und Vorfahrt fast nichts.** Es gibt keine Regeln der Spielwelt. Beschlossen ist nur indirekt, dass Rotlichtverstöße und gefährliches Fahren polizeiliche Folgen haben können. Dieses Modul beschreibt daher **Datenobjekte und Eigenschaften**, die jede künftige Regel braucht, und trennt klar zwischen dem, was die Karte liefern kann, und dem, was das Spiel festlegen muss. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verhalten der Fahrer an Kreuzungen: [Modul 13](13-Verkehrs-KI-und-Navigation.md), [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Was die Karte liefert](#2-was-die-karte-liefert)
3. [Kreuzung als Datenobjekt](#3-kreuzung-als-datenobjekt)
4. [Vorfahrt und Signale](#4-vorfahrt-und-signale)
5. [Schilder und Markierungen](#5-schilder-und-markierungen)
6. [Besondere Kreuzungsfälle](#6-besondere-kreuzungsfälle)
7. [Register](#7-register)
8. [Offene Punkte](#8-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Rotlicht, gefährliches Fahren | Kontrollen von Geschwindigkeit, Rotlicht, gefährlichem Fahren und Kennzeichen sowie Verfolgungen ‚können passieren‘; Warn-Apps im Smartphone zeigen sie an | `23#1025E` Nr. 18; `E-QA-042` |
| Baustellen und Umleitungen | Temporäre Baustellen, Sperren und Umleitungen passend zum Stadtwachstum | `23#1025E` Nr. 5; `W-TV-001` |
| Sperrungen bei Veranstaltungen | Stadtrennen und Rallye-Etappen sperren Straßen an Ereigniszeiten | `W-UR-010` |
| Wachsende Städte | Städte wachsen über Jahre bis Jahrzehnte | `W-UR-018` |
| Offen | Verkehrsregeln der Spielwelt, Schilder, Vorfahrt, Signalisierung | – |

Aus ‚Rotlicht‘ folgt, dass es Verkehrssignale gibt (🧩); über ihre Verbreitung, Programme und Ausfälle ist nichts beschlossen.

---

## 2. Was die Karte liefert

WorldForge beschreibt in `RC-01` für Kreuzungen:

```text
Intersection: stable_id, attached_edge_ids, junction_type, turn_constraints,
              vertical_grade_relations, mesh/spline output_ref
```

und hält fest: ‚Eine 2D-Kreuzung ist nicht automatisch ein verkehrstechnischer Anschluss; unterschiedliches RoadLevel benötigt eigenes Connect-Semantik-Modell‘ (`WF-RC01`). Die Spezifikation sagt außerdem, dass endgültige Schilder, Fahrbahnmarkierungen, Laternen und Geländer aus einem Asset-Katalog des Meilensteins M4 kommen; M2 exportiert funktionale Proxy-Meshes (`WF-RG`).

**Daraus folgt:** Die Karte liefert Topologie, Geometrie, Abbiegebeschränkungen und Höhenbeziehungen. Sie liefert **keine** Vorfahrtsregeln, Signalprogramme, Schilder oder Haltelinien. Diese sind Spielinhalt und müssen von der Spielseite ergänzt werden ([Modul 24](24-WorldForge-Schnittstelle.md)).

---

## 3. Kreuzung als Datenobjekt

> **💡 VORSCHLAG.** Eine Kreuzung wird im Spiel um Daten ergänzt, die die Karte nicht hat. Namen sind Arbeitsnamen.

```text
IntersectionRules  (Spielseite; verweist auf Intersection.stable_id der Karte)
  approaches[]       je Zufahrt: Priorität (bevorrechtigt / wartepflichtig / Halt / Signal)
  allowed_movements  Abbiegebeziehungen (Schnitt aus Kartenbeschränkung und Spielregel)
  conflict_zones[]   Bereiche, in denen sich Bewegungen kreuzen
  signal_program_ref optional
  sign_refs[]        Schilder mit Regelwirkung
  stop_lines[]       Haltelinien, Fußgängerüberwege
```

Die Regeln liegen in Daten, nicht im KI-Code. So lassen sich Regionen, Epochen und Mods anpassen (`VEH-DEC-GP-014`), und die Verkehrs-KI ([Modul 13](13-Verkehrs-KI-und-Navigation.md)) liest nur.

---

## 4. Vorfahrt und Signale

> **💡 VORSCHLAG.**

- **Vorfahrt als Daten:** Je Zufahrt wird ein Prioritätsmerkmal geführt. Das Verhalten (Anhalten, Lücke abwarten, Vortritt gewähren) ist Aufgabe der KI, die Regel kommt aus den Daten.
- **Signalprogramme als Daten:** Ein Programm besteht aus Phasen mit Dauern und freigegebenen Bewegungen. Der Signalzustand ist eine **Funktion von Weltzeit und Programm**; er muss nicht je Ampel gespeichert oder simuliert werden. Das ist deterministisch, billig und speichertauglich (Modul 22 und 27).
- **Abweichungen** (Ausfall, Handsteuerung, Sonderprogramme bei Veranstaltungen) sind Ereignisse im Weltzustand, die das Programm überschreiben. Ob und wie Ausfälle vorkommen (zum Beispiel bei Unfällen, Stromausfällen), steht nicht in den Quellen (`CQ-42`).
- **Einsatzfahrzeuge** mit Sonderrechten: Rettungsgassen sind beschlossen (`N-LE-024`); die Regelung von Signalen für Einsatzfahrzeuge ist nicht belegt.

Die Frage, welche Kreuzungsarten in der Welt vorkommen (Signalkreuzung, Vorfahrtsstraße, Kreisverkehr, Anschlussstelle), ist offen (`CQ-41`).

---

## 5. Schilder und Markierungen

> **💡 VORSCHLAG.** Ein Schild ist ein **Träger einer Regel** (zum Beispiel Haltepflicht, Höchstgeschwindigkeit, Einbahn, Sperrung), nicht nur ein Objekt für die Optik. Dadurch lassen sich Baustellenschilder, Umleitungen und Sperrungen (Modul 15) als Regeländerung mit sichtbarer Darstellung umsetzen: Das Schild und die Regel entstehen und vergehen gemeinsam.

Die Gestaltung der Schilder (Schriftzüge, Formen, Farben) ist nicht belegt. Ob die Spielwelt sie an reale Vorbilder anlehnt oder eigenständig gestaltet, ist offen. Die Quellen verlangen fiktive Fahrzeugnamen (`W-FZ-017`); für Schilder gibt es keine entsprechende Aussage (`CQ-43`).

---

## 6. Besondere Kreuzungsfälle

| Fall | Stand |
|---|---|
| Bahnübergänge | Die Bahn bleibt im Kartenbereich (`W-VK-001`), Querungen sind in `WF-RC01` als `at_grade` und `underpass` typisiert. Regeln und Sicherung an Bahnübergängen sind nicht belegt (🔍). |
| Fußgängerüberwege | siehe [Modul 17](17-Fussgaenger-Radfahrer-und-weitere-Teilnehmer.md) |
| Parkplatzzufahrten, Tankstellenausfahrten | siehe Module 09 und 12 |
| Rennstrecken-Zufahrten | siehe [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md) |
| Straßenänderungen über Spieljahre | Neue Straßen und Umbauten ändern Kreuzungen; die Daten sind versioniert zu führen (Modul 22, 24) |

---

## 7. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-KR-001 | Kontrollen von Geschwindigkeit, Rotlicht, gefährlichem Fahren und Kennzeichen sowie Verfolgungen können vorkommen; Warn-Apps im Smartphone zeigen sie an | 🧭 RICHTUNG | `23#1025E` Nr. 18; `E-QA-042` | POL UI | Details: [Modul 19](19-Polizei-und-Verkehrsueberwachung.md) |
| VEH-DEC-KR-002 | Es gibt Verkehrssignale (Rotlicht ist Teil der Verkehrsvergehen) | 🧩 ABGELEITET | `VEH-DEC-KR-001` | SN KR | Verbreitung und Programme offen |
| VEH-DEC-KR-003 | Temporäre Baustellen, Sperren und Umleitungen passend zum Stadtwachstum sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 5; `W-TV-001` | VKR | Modul 15 |
| VEH-DEC-KR-004 | Städte wachsen über Jahre bis Jahrzehnte; Netz und Kreuzungen können sich über Spieljahre ändern | 🧩 ABGELEITET | `W-UR-018`; `W-TV-001` | WLT SN | Versionierung: Modul 22, 24 |
| VEH-DEC-KR-005 | Die Karte liefert Topologie, Geometrie, Abbiegebeschränkungen und Höhenbeziehungen; sie liefert keine Vorfahrt, Signale, Schilder oder Haltelinien | 🧩 ABGELEITET | `WF-RC01`; `WF-RG` | KR WF | Ergänzung durch das Spiel: Modul 24 |
| VEH-DEC-KR-006 | Endgültige Schilder, Markierungen, Laternen und Geländer kommen aus einem Asset-Katalog (Meilenstein M4); M2 exportiert funktionale Proxy-Meshes | 🧭 RICHTUNG | `WF-RG` | WF ASS | Gehört WorldForge, nicht dem Spiel |
| VEH-DEC-KR-007 | Jede Kreuzung erhält Spielseiten-Daten (Prioritäten, Abbiegebeziehungen, Konfliktbereiche, Signalprogramm, Schilder, Haltelinien), die auf die Karten-ID verweisen | 💡 VORSCHLAG | Ableitung aus `WF-RC01` | KR KI | Format offen |
| VEH-DEC-KR-008 | Vorfahrt wird als Priorität je Zufahrt in Daten geführt, das Verhalten liegt in der KI | 💡 VORSCHLAG | – | KR KI | – |
| VEH-DEC-KR-009 | Der Signalzustand ist eine Funktion von Weltzeit und Signalprogramm und wird nicht je Ampel gespeichert | 💡 VORSCHLAG | Ableitung aus `W-TV-004`; Leistung | KR PER PF | Abweichungen als Ereignisse |
| VEH-DEC-KR-010 | Ein Schild ist ein Träger einer Regel; Schild und Regel entstehen und vergehen gemeinsam (Baustellen, Umleitungen, Sperrungen) | 💡 VORSCHLAG | – | KR VKR | – |
| VEH-DEC-KR-011 | Welche Kreuzungsarten vorkommen (Signal, Vorfahrtsstraße, Kreisverkehr, Anschlussstelle) und wie verbreitet sie sind, ist nicht belegt | 🔍 UNBELEGT | – | SN KR | `CQ-41` |
| VEH-DEC-KR-012 | Ausfall und Sonderschaltung von Signalen sowie Signalvorrechte für Einsatzfahrzeuge sind nicht belegt | 🔍 UNBELEGT | – | KR UP | `CQ-42` |
| VEH-DEC-KR-013 | Gestaltung der Schilder (Anlehnung an Vorbilder oder eigenständig) und Regeln an Bahnübergängen sind nicht belegt | 🔍 UNBELEGT | – | KR ASS | `CQ-43` |

---

## 8. Offene Punkte

| Frage | Verweis |
|---|---|
| Kreuzungsarten und Verbreitung | `CQ-41` |
| Ausfall und Sonderschaltung von Signalen, Vorrechte | `CQ-42` |
| Gestaltung von Schildern, Bahnübergänge | `CQ-43` |
