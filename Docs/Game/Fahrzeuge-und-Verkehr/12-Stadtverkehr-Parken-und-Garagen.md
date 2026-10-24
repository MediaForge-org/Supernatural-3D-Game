# Fahrzeuge und Verkehr 12: Stadtverkehr, Parken, Garagen und Depots

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass Parkflächen realistisch existieren, der Spieler aber nicht zur Parkplatzsuche gezwungen wird, und dass Parken, Park&Ride und Straßensperren zur Welt gehören. Ob es Valet-Parken gibt, ist in den Quellen widersprüchlich (`CU-02`). Zahlen, Regeln für Falschparken und die Platzierung von Parkhäusern und Depots sind offen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Stadtstruktur: [Welt 06](../World/06-Metropolis.md), [Welt 07](../World/07-Weitere-Staedte-und-Siedlungen.md), [Welt 08](../World/08-Raeumliche-Allokation-Orte-und-Aktivitaeten.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Stadtverkehr](#2-stadtverkehr)
3. [Parken](#3-parken)
4. [Garagen, Depots, Verwahrstellen](#4-garagen-depots-verwahrstellen)
5. [Valet-Parken](#5-valet-parken)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Parkplatzsuche | Parkflächen existieren realistisch, der Spieler wird aber nicht gezwungen, lange einen Parkplatz zu suchen | `23#1025E` Nr. 46 |
| Parken und Dienste | Parken, Valet-Service an passenden Orten, Parkplatz- und Park&Ride-Bedarf, Straßensperren bei Veranstaltungen | `W-VK-013` (Valet: siehe `CU-02`) |
| Gewerbe | Parkplätze gehören zum Gewerbe- und Dienstleistungsumfang | `W-UR-005` |
| Eigene Garage | Der Spieler kann ein Haus mit Garage und Werkstatt planen | `W-UR-003` |
| Veranstaltungen | NPCs besuchen Veranstaltungen und beeinflussen Verkehr, ÖPNV, Parken, Gastronomie, Hotels und Wettmärkte | `W-UR-009` |
| Metropolis | Verkehr umfasst Autofahren, ÖPNV, Taxi und Rideshare, Parken | [Welt 06](../World/06-Metropolis.md) |
| Persistenz | Fahrzeuge sind persistent; am Falltatort abgestellte Fahrzeuge bleiben (Entwurf) | `W-FZ-016` |

---

## 2. Stadtverkehr

Beschlossen ist die Struktur des Alltags: NPCs haben Wochenroutinen, nutzen Wohnort, Arbeitsplätze, Geschäfte, Bars, Clubs, Parks, Casinos, Skiorte, Sportstätten und Verkehr (`W-TV-002`). Daraus ergibt sich der Stadtverkehr als Folge des Lebens der Menschen, nicht als zufälliger Strom.

> **💡 VORSCHLAG.** Stadtbezirke (in der Welt-Dokumentation als Datenobjekte mit Rolle und Dichte vorgeschlagen, [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §6) tragen ein **Verkehrscharakter-Profil**: Dichtekurve, typische Verkehrsrollen (Pendler, Lieferung, Taxi, Bus), Geschwindigkeitsniveau, Parkdruck, Fußgängeranteil. Downtown, Wohnviertel, Industriegebiete und Skiorte unterscheiden sich darin. Die Zahlenwerte sind nicht belegt (`CQ-47`).

Verbindungen zu anderen Modulen: Dichte und Staus ([Modul 15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md)), Fußgänger ([Modul 17](17-Fussgaenger-Radfahrer-und-weitere-Teilnehmer.md)), ÖPNV (`W-VK-006`, 🧭).

---

## 3. Parken

> **💡 VORSCHLAG.** Parken wird aus **Kapazität und Zustand** modelliert, nicht aus einzelnen Parklücken überall.

| Parkart (Beispielliste) | Bemerkung |
|---|---|
| Straßenrand | im Nahbereich als einzelne Lücken, in der Ferne als Kapazität |
| Parkplatz bei Geschäft oder Betrieb | gehört zum Gewerbe (`W-UR-005`) |
| Parkhaus | größere Kapazität |
| Private Garage und Stellplatz | gehört zum Haushalt oder Spielerhaus (`W-UR-003`) |
| Park&Ride | an ÖPNV-Knoten (`W-VK-013`) |
| Verwahrstelle | für abgeschleppte Fahrzeuge (Modul 16) |
| Valet | siehe Abschnitt 5 |

**Verhalten:**

- Das Fahrzeug jedes NPC hat einen Heimatstellplatz (Wohnung) und kennt typische Ziele (Arbeitsplatz). Daraus folgt der Standort geparkter Fahrzeuge: nachts am Wohnort, tagsüber am Arbeitsplatz oder Ziel (`N-ID-005`, `W-TV-002`).
- Die **Parkplatzsuche** der NPCs ist im Nahbereich sichtbar, in der Ferne abstrakt (`N-LE-013`). Dem Spieler wird sie nicht aufgezwungen (`VEH-DEC-SP-001`).
- Parkflächen haben eine Kapazität; ihre Belegung ist ein Zustand, der sich aus Tagesabläufen und Ereignissen ergibt. Einzelne Lücken entstehen erst beim Annähern (Rekonstruktion, `VEH-DEC-GP-006`).
- Abgestellte Spielerfahrzeuge bleiben als persistente Fahrzeuge bestehen (`W-FZ-016`).

**Offen:** Falschparken, Strafzettel, Abschleppen von Fahrzeugen des Spielers (`CQ-44`) und das Parkmodell selbst, ob einzelne Lücken oder nur Kapazität (`CQ-45`).

---

## 4. Garagen, Depots, Verwahrstellen

> **💡 VORSCHLAG.** Mehrere Orte dienen als **Heimat** für Fahrzeuge einer Verkehrsrolle (Modul 03). Sie sind Orte, an denen Fahrzeuge „zu Hause“ sind, wenn sie nicht unterwegs sind, und an denen NPCs arbeiten:

| Ort | Bezug |
|---|---|
| Private Garage | `W-UR-003`; Haushalte |
| Firmenhof, Fuhrpark | Lieferung, Trucking (`W-WI-005`) |
| Busdepot, Taxizentrale | ÖPNV, Taxi (`W-VK-006`, `W-WI-005`) |
| Bahndepots und Werkstätten | `W-VK-002` (Anlagenbereiche der Bahn; nicht Straße) |
| Abschlepphof, Verwahrstelle | Abschleppdienst (`W-TV-006`) |
| Schrottplatz | für Wracks (`VEH-DEC-ZV-014`, Modul 22) |
| Werkstatt-Höfe | Modul 08 |

Zahl und Platzierung hängen am Stadtgenerator und an der Karte und sind nicht beschlossen (`CQ-46`). Dass die Orte als Weltobjekte mit NPC-Arbeitsplätzen modelliert werden, ist Vorschlag und passt zu `W-TV-002`.

---

## 5. Valet-Parken

Der Fahrzeugblock des Chats 23 fragte nach Valet-Parken in Hotels, Casinos und Restaurants (Frage 47); die Antwort des Entwicklers lautet ‚was ist das‘ (`23#1025E` Nr. 47). Die Planungszusammenfassung führt dagegen ‚Valet parking exists at suitable venues‘ (`M-WB17` Block 5), und das Welt-Register übernimmt es als beschlossen (`W-VK-013`). Ob der Entwickler Valet-Parken bewusst will, nachdem er es erklärt bekam, ist in den Quellen nicht belegt. Geführt als `CU-02`.

Das Modul setzt Valet-Parken daher nicht als beschlossen voraus. Wenn es kommt, ist es ein Sonderfall des Standortzustands (Fahrzeug ‚bei Dienstleister‘), der mit einem Rückgabeereignis endet.

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-SP-001 | Parkflächen existieren realistisch, der Spieler wird aber nicht gezwungen, lange einen Parkplatz zu suchen | 📋 BESCHLOSSEN | `23#1025E` Nr. 46 | VKR SP | – |
| VEH-DEC-SP-002 | Parken, Park&Ride-Bedarf und Straßensperren bei Veranstaltungen gehören zur Welt | 📋 BESCHLOSSEN | `W-VK-013`; `W-UR-009` | WLT VKR | – |
| VEH-DEC-SP-003 | Parkplätze gehören zum Gewerbe- und Dienstleistungsumfang; der Spieler kann ein Haus mit Garage planen | 📋 BESCHLOSSEN | `W-UR-005`; `W-UR-003` | WLT IMM | – |
| VEH-DEC-SP-004 | Valet-Parken ist Teil der Planungszusammenfassung, der Entwickler antwortete auf die Frage aber ‚was ist das‘; eine bewusste Entscheidung nach der Erklärung ist nicht belegt | 🔍 UNBELEGT | `M-WB17` Block 5; `W-VK-013`; `23#1025E` Nr. 47 | WLT | `CU-02`, `CQ-46` |
| VEH-DEC-SP-005 | NPCs nutzen Wohnort, Arbeitsplätze und Ziele im Alltag; der Stadtverkehr ergibt sich aus ihrem Leben | 📋 BESCHLOSSEN | `W-TV-002` | NPC VKR | – |
| VEH-DEC-SP-006 | Das Fahrzeug jedes NPC hat einen Heimatstellplatz und typische Ziele; der Standort geparkter Fahrzeuge folgt dem Tagesablauf | 🧩 ABGELEITET | `N-ID-005`; `W-TV-002` | NPC VKR | Details: Modul 14 |
| VEH-DEC-SP-007 | Parken wird aus Kapazität und Zustand modelliert; einzelne Parklücken entstehen erst beim Annähern | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-006`, `N-LE-013` | SP LS | Alternative: einzelne Lücken überall (`CQ-45`) |
| VEH-DEC-SP-008 | Die Parkplatzsuche der NPCs ist im Nahbereich sichtbar und in der Ferne abstrakt | 💡 VORSCHLAG | Ableitung aus `N-LE-013` | NPC KI | – |
| VEH-DEC-SP-009 | Orte als Heimat für Fahrzeuge einer Verkehrsrolle (Garage, Firmenhof, Depot, Verwahrstelle, Schrottplatz) sind Weltobjekte mit NPC-Arbeitsplätzen | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `W-TV-006` | WLT NPC | Platzierung: `CQ-46` |
| VEH-DEC-SP-010 | Stadtbezirke tragen ein Verkehrscharakter-Profil (Dichtekurve, typische Rollen, Geschwindigkeitsniveau, Parkdruck, Fußgängeranteil) | 💡 VORSCHLAG | Ableitung aus [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §6 | WLT VKR | Werte nicht belegt: `CQ-47` |
| VEH-DEC-SP-011 | Regeln für Falschparken, Strafzettel und Abschleppen von Spielerfahrzeugen sind nicht belegt | ❓ OFFEN | – | POL VKR | `CQ-44` |
| VEH-DEC-SP-012 | Zahl, Art und Platzierung von Parkhäusern, Garagen, Depots und Verwahrstellen sind nicht beschlossen | ❓ OFFEN | – | WLT | `CQ-46` |
| VEH-DEC-SP-013 | Fußgängerzonen, Verkehrsberuhigung und Umweltzonen sind nicht erwähnt | 🔍 UNBELEGT | – | SN VKR | `CQ-48` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Falschparken und Abschleppen von Spielerfahrzeugen | `CQ-44` |
| Parkmodell (einzelne Lücken oder Kapazität) | `CQ-45` |
| Valet-Parken sowie Zahl und Platzierung von Parkhäusern, Garagen, Depots | `CQ-46`, `CU-02` |
| Verkehrscharakter-Profile der Bezirke | `CQ-47` |
| Fußgängerzonen und Verkehrsberuhigung | `CQ-48` |
