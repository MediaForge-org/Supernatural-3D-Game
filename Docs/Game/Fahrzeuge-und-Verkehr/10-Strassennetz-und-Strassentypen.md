# Fahrzeuge und Verkehr 10: Straßennetz und Straßentypen

> [!IMPORTANT]
> **Es gibt kein festgelegtes Straßennetz und keine Verkehrsregeln.** Das Netz der Hauptwelt ist wie die Karte offen (`CLAUDE.md` §3). Beschlossen sind Absichten: ein sparsames Netz, Straßen im Gebirge mit kleinen Dörfern, Brücken und Tunnel dort, wo das Gelände sie verlangt, und der Straßenmaßstab einer nordamerikanisch geprägten Welt. Nirgends in den Quellen steht, auf welcher Seite gefahren wird. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Das Straßennetz als Karte ist in [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) und [Welt 17](../World/17-Weltgenerierung-und-GIS-Daten.md) behandelt; die Kopplung an WorldForge in [Modul 24](24-WorldForge-Schnittstelle.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Straßenklassen](#2-straßenklassen)
3. [Straßenprofil als Datenobjekt](#3-straßenprofil-als-datenobjekt)
4. [Autobahnen und Schnellstraßen](#4-autobahnen-und-schnellstraßen)
5. [Land- und Bergstraßen](#5-land--und-bergstraßen)
6. [Brücken, Tunnel, Fähren](#6-brücken-tunnel-fähren)
7. [Was nicht festgelegt ist](#7-was-nicht-festgelegt-ist)
8. [Register](#8-register)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Status | Quelle |
|---|---|---|---|
| Dichte | Ländliche Gebiete haben wenig Straßen und Gebäude, Metropolen dichte Netze; Brücken und Tunnel entstehen, wo das Gelände sie verlangt | 🧭 | `W-VK-015` |
| Makronetz | Das Makronetz ist sparsam; ländliche Straßen sind zurückzunehmen; Bergstraßen folgen Tälern und Pässen | 🧭 (Arbeitsaufträge an Kartenläufe) | `W-VK-007` |
| Berge | Die Berge sollen Straßen mit kleinen Dörfern besitzen, sonst seien sie unnötig | 📋 (Antwort auf einen Kartenentwurf) | `23#705E` |
| Auto am Berg | Mit dem Auto kann man auf nicht so hohe Berge fahren; auf dem Berg gibt es eine Gondel | 🧭 (Rückmeldung zu einem Kartenentwurf) | `23#823E` |
| Maßstab | Europäisch geprägte Fußgänger- und Verkehrslogik mit nordamerikanischem Straßenmaßstab (für die 70-km²-Linie formuliert) | 🧭 | `W-UR-015` |
| Welt | fiktive US-artige Welt mit fiktiven Bundesstaaten | 📋 | [Ermittlung 24](../Ermittlung/24-Privatdetektiv-Laufbahn.md) (Verweis auf `35#533A`) |
| Wildtiere | Wildtiere, darunter Wildunfälle (‚roadkill‘), gehören zur Welt | 📋 | `M-WB17` Block 6 |
| Regeln | Straßenverkehrsrecht der Spielwelt | ❓ | `M-OD` ‚Crime / law / evidence‘ |

Die Korridore des Prototypauftrags (Autobahn West–Ost, Nord–Süd über einen Pass, Küstenkorridor; `W-VK-008`) und der berichtete Korridorstand der Testwelt (`W-VK-009`) sind Arbeitsaufträge und Berichte von Kartenläufen. Sie sind für das Spiel nicht verbindlich.

---

## 2. Straßenklassen

WorldForge nennt als Arbeitsklassen: Autobahn, Schnellstraße, Haupt-/Bundesstraße, Regional-/Landstraße, Stadtstraße, Wohnstraße, Wirtschafts- und Feldweg; optional Brücken, Tunnel, Fähren, Service- und Zufahrten (`WF-D03`, `WF-RG`). Jede Klasse besitzt Breiten-, Steigungs-, Kurven-, Knoten-, Geschwindigkeits- und Bauprofilregeln; die Werte hängen vom Design- oder Regionsprofil ab (`WF-RG`).

> **💡 VORSCHLAG.** Das Spiel übernimmt diese Liste als **Arbeitsgrundlage**. Welche Klassen das Spiel tatsächlich unterscheidet, ist nicht beschlossen und bleibt `CQ-36`. Jede zusätzliche Klasse, die das Spiel braucht (zum Beispiel Parkplatzzufahrt, Rennstrecken-Zubringer, Industriezufahrt), wird als Wunsch an WorldForge formuliert, nicht stillschweigend ergänzt.

---

## 3. Straßenprofil als Datenobjekt

> **💡 VORSCHLAG.** Verhalten und Darstellung einer Straße folgen aus ihrer Klasse und aus Regionsprofilen. Statt der Klasse Regeln einzubrennen, wird ein **Straßenprofil** als Datenobjekt geführt (`VEH-DEC-GP-014`).

| Feld (Arbeitsname) | Wirkung | Herkunft |
|---|---|---|
| lanes, width | Fahrstreifen, Breite | Karte (`WF-RC01`) |
| surface | Belag (Asphalt, Schotter, Pflaster, Feldweg) | Karte oder Spielprofil; wirkt auf Haftung (Modul 04) |
| speed_limit | Höchstgeschwindigkeit | **nicht belegt**; Spielprofil (Modul 11) |
| allowed_roles | erlaubte Verkehrsrollen (Modul 03) | Spielprofil |
| pedestrian_cyclist_access | Fußgänger und Radverkehr | Spielprofil (Modul 17) |
| lighting, signage | Beleuchtung, Beschilderung | Spielprofil (Modul 11) |
| density_curve_ref | Verweis auf die Dichtekurve (Tageszeit, Wochentag, Wetter) | Spielprofil (Modul 15) |
| shoulder, emergency_lane | Standstreifen, Rettungsgasse | Karte (`shoulder_type`); Rettungsgassen sind beschlossen (`N-LE-024`) |

Welche dieser Felder die Karte liefert und welche das Spiel dazugibt, behandelt [Modul 24](24-WorldForge-Schnittstelle.md).

---

## 4. Autobahnen und Schnellstraßen

Beschlossen ist nicht mehr als: Es gibt Fernverbindungen und Stau auf ihnen ist denkbar (Frage 2 des Fahrzeugblocks nannte Metropolis und Interstates; Antwort: kann man machen, ein Stau wird dann im Radio angesagt, `23#1025E` Nr. 2). Der Kartenauftrag hatte eine Autobahn West–Ost vorgesehen (`W-VK-008`, 🧭).

> **💡 VORSCHLAG.** Eine Autobahnklasse hat eigene Verkehrsmerkmale, die sich aus beschlossenen Anforderungen ergeben:
>
> - Kein Fußgänger- und Radverkehr (Folge der Klasse; nicht belegt, aber unstrittig).
> - Anschlussstellen als eigene Knoten (`Intersection` in `WF-RC01` mit eigener Anschluss-Semantik; ‚eine 2D-Kreuzung ist nicht automatisch ein verkehrstechnischer Anschluss‘).
> - Standstreifen und Rettungsgasse für Pannen und Einsatzfahrzeuge (`N-LE-024`, `W-TV-001`).
> - Die höchsten Geschwindigkeiten, damit die größte Unfallwucht und die strengsten Anforderungen an die Verkehrs-KI.
> - Maut, Mautstellen und Rastanlagen: nicht belegt.

---

## 5. Land- und Bergstraßen

- **Steigung und Kurven:** Gebirgsstraßen sind der Fall, in dem Fahrphysik, Wetter und Jahreszeit am stärksten zusammenwirken (Modul 04). Die Kartenreferenz WORLD_700 enthält eine Bergzufahrt und eine Hillclimb-Strecke ([Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) §2, §9); sie ist ein Referenzfall, keine Vorgabe für die Hauptwelt.
- **Dörfer:** Die Berge sollen Straßen mit kleinen Dörfern haben (`23#705E`). Das steht in einer Spannung zur Anweisung, ländliche Straßen zurückzunehmen (`W-VK-007`); beide sind Aussagen zu Kartenentwürfen (`CK-05`).
- **Saison:** Jahreszeiten wirken auf Verkehr und Straßen (`23#1025E` Nr. 49); Schnee in der Mountain Region ist dynamisch (`M-WB17` Block 6). Daraus ergibt sich, dass Gebirgsstraßen zeitweise schwer befahrbar oder gesperrt sein können (💡, Modul 15).
- **Wild:** Wildtiere queren Straßen; Wildunfälle gehören zur Welt (`M-WB17` Block 6). Wildwechselzonen als Streckenmerkmal sind ein Vorschlag (`VEH-DEC-SN-012`).
- **Rennen:** Eine Rallye- oder Hillclimb-Straße im Gebirge ist gewünscht (`23#822E`; [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md)); sie ist ein Sonderfall der Sperrung (Modul 15).

---

## 6. Brücken, Tunnel, Fähren

`WF-RC01` typisiert Querungen als `at_grade`, `bridge`, `tunnel`, `underpass`, `ferry` und `causeway`. Für das Spiel folgen daraus Anforderungen, die über die Geometrie hinausgehen:

> **💡 VORSCHLAG.**
>
> - Fahrzeug- und Navigationslogik müssen Brücken und Tunnel als Teil des Netzes kennen (Pfad in der Höhe, nicht nur in der Ebene).
> - Tunnel brauchen eigene Streaming-, Beleuchtungs- und Sichtregeln (`WF-RG`: Tunnel benötigen eigenen 3D-Körper, Portale und Freistellung).
> - Brücken und Tunnel sind Orte für Sperrungen, Unfälle und Ermittlungsszenen.
> - Fähren verbinden zum Beispiel Inseln; ihre Zahl hängt an der Karte. Die Verbindung zu Passagier- und Fahrzeugfähren ist ein Betriebsproblem außerhalb dieses Moduls.

---

## 7. Was nicht festgelegt ist

| Gegenstand | Befund |
|---|---|
| Fahrtrichtung (Rechts- oder Linksverkehr) | **in keiner Quelle belegt**. Eine US-artige Welt legt Rechtsverkehr nahe; das ist eine Ableitung ohne Beleg (`CU-06`). Der Maßstab ‚nordamerikanisch‘ in `W-UR-015` nennt keine Seite. |
| Höchstgeschwindigkeiten, Vorfahrtsregeln, Verkehrsschilder | nicht belegt |
| Anzahl der Spuren je Klasse | nicht belegt; WorldForge hat ein Feld, aber keine Standardwerte |
| Gesamtnetz, Straßenkilometer, Dichte | offen (Welt 09 §10; `CLAUDE.md` §3) |
| Mautstraßen, Kreisverkehre, Einbahnstraßen | nicht erwähnt |

Das Reise-Richtmaß (90 km/h und 20 % Umweg, West–Ost 54 Minuten bei 2.200 km², `45#60A`) stammt aus einem Assistentenvorschlag und dient der Kalibrierung der Weltgröße (`W-GE-026`), nicht als Verkehrsregel.

---

## 8. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-SN-001 | Ländliche Gebiete haben wenig Straßen und Gebäude, Metropolen dichte Netze; Brücken und Tunnel entstehen dort, wo das Gelände sie verlangt | 🧭 RICHTUNG | `W-VK-015` | WLT SN | – |
| VEH-DEC-SN-002 | Das Makronetz ist sparsam, die ländliche Straßendichte soll abnehmen, Bergstraßen folgen Tälern und Pässen | 🧭 RICHTUNG | `W-VK-007` (Arbeitsaufträge an Kartenläufe) | WLT SN | Spannung zu `VEH-DEC-SN-003`: `CK-05` |
| VEH-DEC-SN-003 | Die Berge sollen Straßen mit kleinen Dörfern besitzen | 📋 BESCHLOSSEN | `23#705E` (Antwort auf einen Kartenentwurf) | WLT SN | `CK-05` |
| VEH-DEC-SN-004 | Mit dem Auto kann man auf nicht so hohe Berge fahren; hohe Berge sind nur mit Spezialfahrzeugen oder Gondeln erreichbar | 🧭 RICHTUNG | `23#823E`; [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §1 (`23#821E`) | WLT SN | Höhengrenze nicht belegt |
| VEH-DEC-SN-005 | Straßenmaßstab nordamerikanisch, Fußgänger- und Verkehrslogik europäisch geprägt | 🧭 RICHTUNG | `W-UR-015` | WLT SN | Für die 70-km²-Linie formuliert, nicht erneut bestätigt |
| VEH-DEC-SN-006 | Wildunfälle gehören zur Welt | 📋 BESCHLOSSEN | `M-WB17` Block 6 | WLT VKR | Häufigkeit nicht belegt |
| VEH-DEC-SN-007 | Rettungsgassen sind vorgesehen | 📋 BESCHLOSSEN | `N-LE-024` | VKR | Verhalten: Modul 16 |
| VEH-DEC-SN-008 | Die Arbeitsklassen der Straßen sind Autobahn, Schnellstraße, Haupt-/Bundesstraße, Regional-/Landstraße, Stadtstraße, Wohnstraße, Wirtschafts- und Feldweg | 💡 VORSCHLAG | `WF-D03`; `WF-RG` | SN WF | Welche das Spiel unterscheidet: `CQ-36` |
| VEH-DEC-SN-009 | Verhalten und Darstellung einer Straße folgen aus einem Straßenprofil (Datenobjekt), nicht aus festen Klassenregeln | 💡 VORSCHLAG | `VEH-DEC-GP-014`; `WF-RG` | SN | Felder: Abschnitt 3 |
| VEH-DEC-SN-010 | Die Autobahnklasse hat eigene Merkmale: kein Fuß- und Radverkehr, eigene Anschlussstellen, Standstreifen und Rettungsgasse | 💡 VORSCHLAG | Ableitung aus `WF-RC01`, `N-LE-024` | SN VKR | – |
| VEH-DEC-SN-011 | Brücken, Tunnel und Fähren sind Teil des Fahrzeug- und Navigationsnetzes und brauchen eigene Streaming-, Licht- und Sperrungsregeln | 💡 VORSCHLAG | Ableitung aus `WF-RC01`, `WF-RG` | SN KI LS | – |
| VEH-DEC-SN-012 | Wildwechselzonen sind ein Streckenmerkmal ländlicher und Bergstraßen | 💡 VORSCHLAG | Ableitung aus `M-WB17` Block 6 | SN VKR | – |
| VEH-DEC-SN-013 | Gebirgsstraßen können saisonal schwer befahrbar oder gesperrt sein | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 49, `M-WB17` Block 6 | SN WET | Modul 15 |
| VEH-DEC-SN-014 | Die Fahrtrichtung (Rechts- oder Linksverkehr) ist nirgends festgelegt | 🔍 UNBELEGT | – | SN KI | `CU-06`, `CQ-37` |
| VEH-DEC-SN-015 | Höchstgeschwindigkeiten, Spurzahlen je Klasse und Verkehrsschilder sind nicht belegt | 🔍 UNBELEGT | – | SN KR | `CQ-38` |
| VEH-DEC-SN-016 | Das Gesamtnetz der Hauptwelt (Autobahnen, Pässe, Fähren, Dichte) ist offen | ❓ OFFEN | [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) §10; `CLAUDE.md` §3 | WLT | `CQ-39` |
| VEH-DEC-SN-017 | Mautstraßen, Kreisverkehre und Einbahnstraßen sind nicht erwähnt | 🔍 UNBELEGT | – | SN KR | `CQ-40` |

---

## 9. Offene Punkte

| Frage | Verweis |
|---|---|
| Welche Straßenklassen unterscheidet das Spiel | `CQ-36` |
| Fahrtrichtung | `CQ-37` |
| Höchstgeschwindigkeiten, Spurzahlen, Schilder | `CQ-38` |
| Gesamtnetz | `CQ-39` |
| Mautstraßen, Kreisverkehre, Einbahnstraßen | `CQ-40` |
| Spannung ‚Berge mit Straßen und Dörfern‘ gegenüber ‚ländliche Straßen reduzieren‘ | `CK-05` |
