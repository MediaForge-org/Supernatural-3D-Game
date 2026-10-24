# Fahrzeuge und Verkehr 02: Vision und Grundprinzipien

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul fasst zusammen, was für Fahrzeuge und Verkehr beschlossen ist, benennt die Grundhaltung, die daraus folgt, und legt fest, **wer welches Thema besitzt**, damit kein System ein zweites Mal entschieden wird. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 28](28-Entscheidungsregister.md).

**Inhalt**

1. [Befund](#1-befund)
2. [Drei Verantwortungsbereiche](#2-drei-verantwortungsbereiche)
3. [Grundprinzipien](#3-grundprinzipien)
4. [Eigentümer je Thema](#4-eigentümer-je-thema)
5. [Register](#5-register)
6. [Offene Punkte](#6-offene-punkte)

---

## 1. Befund

Die Planung beschreibt Fahrzeuge aus zwei Richtungen:

- **Das einzelne Fahrzeug** ist ein Kernsystem. Es ist die mobile Ausrüstungsbasis des Hunters, hat eine physikalisch gemeinte Fahrdynamik mit manuellem Getriebe als Standard, verschleißt, wird repariert, getankt, gekauft und gebraucht gehandelt, und es ist persistent (`W-FZ-001`, `W-FP-001`, `W-FZ-008`, `W-FZ-016`). Lange Fahrten sind ein Kanal für Figurenentwicklung (Gespräche, Radio, Telefon, Schlaf, Streit, Fallbesprechung; Gameplay-Systeme §6.1).
- **Der Verkehr** ist ein Weltlayer. Er soll dynamisch sein, aber nicht übertrieben, Stoßzeiten kennen, über das Radio gemeldet werden, selten Unfälle und gelegentlich Pannen und Baustellen enthalten und mit Fahrern, die sich in Können und Wesen unterscheiden (`W-TV-001`, `W-FP-009`, `N-LE-024`). Er ist Teil der Lebenden Welt: NPCs haben eigene Fahrzeuge, fahren zur Arbeit, in den Urlaub und zu Besuchen, und „sonst könnten nie staus sein“ (`N-LE-006`).

Die Fahrzeugdetails des Oktober-Pakets (`D15-RV`) gelten nach dem Zusatz vom 18.10.2026 als ‚review-required specification inputs‘, nicht als stillschweigend bestätigte numerische Vorgaben (`M-EP1018`). `D15-RV` selbst nennt den Phasenvertrag ‚user-approved‘ und die Belegung ‚user specified‘. Die Dokumentation übernimmt den Status der Welt-Dokumentation (📋) und führt die Uneinheitlichkeit als `CK-07`.

Dazwischen liegt, was **nicht** festgelegt ist: das Verkehrsmodell, die Fahrzeugliste, das Schadensmodell, die Verkehrsregeln der Spielwelt und die technische Umsetzung (`W-FP-010`, `W-FZ-013`, `W-FZ-014`, `W-TV-008`). Diese Dokumentation entwirft dafür **Strukturen und Prüfwege**, keine Werte.

---

## 2. Drei Verantwortungsbereiche

| Bereich | Gegenstand | Wo behandelt |
|---|---|---|
| **Fahrzeug** | ein einzelnes Objekt mit Identität, Zustand, Besitzer, Inventar, Fahrdynamik | Module 03 bis 09, 18, 20 bis 22 |
| **Straße** | die Infrastruktur: Netz, Typen, Kreuzungen, Schilder, Signale, Parken, Tankstellen | Module 10 bis 12, 24 |
| **Verkehr** | die Population aus fahrenden und stehenden Fahrzeugen und anderen Teilnehmern samt Verhalten | Module 13 bis 17, 19, 26, 27 |

Die technische Klammer sind die Module 22 (Persistenz), 25 (Unreal), 26 (Simulationsstufen) und 27 (Leistung).

---

## 3. Grundprinzipien

Die Prinzipien sind entweder beschlossene Aussagen (📋), daraus Abgeleitetes (🧩) oder Vorschläge (💡). Sie sind **Prüfsteine für die Module**, keine Mechaniken.

| Nr. | Prinzip | Status | Herkunft |
|---|---|---|---|
| P1 | **Glaubwürdige Konsistenz vor vollständiger Simulation.** Die Welt soll nur so tief simuliert werden, dass der Spieler keinen offensichtlichen Fake bemerkt. | 📋 | `N-LT-011` |
| P2 | **Die Straße ist nie leer, wenn man dort ankommt, und nie unglaubwürdig voll.** Verkehr wird beim Betreten eines Gebiets aus abstraktem Zustand rekonstruiert. | 📋 | `W-TV-009`, `N-LE-013`, `N-LE-026` |
| P3 | **Verkehr ist Teil der NPC-Gesellschaft, keine eigene Welt.** NPC-Fahrzeuge gehören NPC-Haushalten und Firmen; es gibt keine separate ‚Verkehrspopulation‘ mit eigenen Identitäten. | 🧩 | `N-ID-005`, `N-ID-007`, `N-LE-006`; Begründung in [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md) |
| P4 | **Wichtige NPCs sterben nicht durch Fehler der Verkehrssimulation.** Ein vom Spieler herbeigeführter Unfall ist etwas anderes. | 📋 | `N-LT-033` |
| P5 | **Ein Fakt, ein Eigentümer, Änderungen über Ereignisse.** Besitzer, Zustand, Standort und Rechtslage eines Fahrzeugs liegen je an genau einer Stelle. | 💡 | Folge aus `N-LT-008` (keine widersprüchlichen Zustände zwischen Systemen) |
| P6 | **Verhalten über Profile, nicht über Konstanten.** Fahrstile, Geschwindigkeits- und Kurvenwerte, Dichtekurven sind austauschbare Datenprofile je Region und Straßenklasse. | 💡 | Linie von `CLAUDE.md` §4 (datengetrieben, ersetzbar) und `WF-RG` (Werte vom Regionsprofil abhängig, kein globales Raster) |
| P7 | **Die Weltgröße ist nirgends fest verdrahtet.** Alle Mengen skalieren mit Netz und Siedlung, nicht mit einer Zahl. | 📋 | `N-LT-007` (rund 2.200 km² als Arbeitsszenario) |
| P8 | **Keine Zahl ohne Quelle.** Fahrzeugmengen, Dichten, Geschwindigkeiten und Zeiten stehen nur als markierte Beispielwerte oder gar nicht. | 🧩 | `CLAUDE.md` §3 und §7 |
| P9 | **Straßenverkehr ist relativ realistisch, Motorsport deutlich simulationslastiger.** Zwei Fidelity-Stufen derselben Fahrphysik, nicht zwei Systeme. | 📋 | `W-FP-002`, `W-FP-008` |
| P10 | **Fahren ist kein Zwang.** Parken, Fahren und Mitfahren sind Angebote; Zug und Bus sind für lange Strecken bevorzugt, weil schneller. | 📋 | `W-VK-014` und Block-5-Antworten (`23#1025E`) |

---

## 4. Eigentümer je Thema

Damit keine Doppelverantwortung entsteht, gilt diese Aufteilung. „Eigentümer“ heißt: Hier ist die Regel zu entscheiden; andere Systeme **lesen** sie.

| Thema | Eigentümer | Dieser Ordner |
|---|---|---|
| Fahrzeugklassen, Marken, Märkte, Rennklassen | [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md), [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md) | verweist (Modul 03) |
| Fahrphysik-Grundlagen, Eingabe | [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) | verweist, ergänzt Prüfwege (Module 04, 05) |
| Straßennetz als Karte, Reisezeit, ÖPNV, Bahn, Flug | [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md), [Welt 17](../World/17-Weltgenerierung-und-GIS-Daten.md) | verweist (Modul 10, 24) |
| Streaming, World Partition | [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) | verweist (Modul 26) |
| NPC-Identität, Tagesablauf, Persistenzstufen | [NPCs 03, 13, 21, 22](../NPCs/README.md) | **keine** zweite NPC-Architektur (Module 14, 22, 26) |
| Zeugen, Kennzeichen, Polizei, Haft, Spuren | [Ermittlung 05, 06, 14, 16](../Ermittlung/README.md) | Verkehrsrechtliche Anwendung (Module 19, 21) |
| Schießen aus und auf Fahrzeuge, Unfallverletzung | [Kampfsystem 05, 13](../Kampfsystem/README.md) | verweist (Module 07, 16, 21) |
| Fahrzeugverkehr: Straßenregeln, Signale, Fahrerverhalten, Staus, Unfälle, Parken, Tankstellen | **dieser Ordner** | Module 07 bis 17 |
| Fahrzeugbesitz, Zulassung, Diebstahl | **dieser Ordner** (Rechtsfolgen: Ermittlung) | Modul 18 |
| Kopplung Straßengraph, WorldForge | **dieser Ordner** (Vorschlag; Gegenstück bei WorldForge) | Modul 24 |

---

## 5. Register

Nummerierung `VEH-DEC-GP-nnn`. Das erzeugte Gesamtregister: [Modul 28](28-Entscheidungsregister.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-GP-001 | Fahren ist ein Kernsystem und eine frühe Ambition für den Vertical Slice; das Fahrzeug ist mobile Ausrüstungsbasis | 📋 BESCHLOSSEN | `W-FZ-001`; Gameplay-Systeme §6.1 | FZ INV | – |
| VEH-DEC-GP-002 | Phasenvertrag: früh sinnvolles Schaltgetriebe mit funktionierender Kupplung, einfacher Zugang, einfache Reparatur; langfristig nahezu vollständige Fahrdynamik, tiefere Interaktion, nahezu vollständige Werkstattsimulation | 📋 BESCHLOSSEN | `W-FP-004`, `W-FZ-011`, `W-FZ-012`; `D15-RV` | FZ | Termine, Motormodell und Wiedergabetreue-Maßstäbe sind nicht beschlossen |
| VEH-DEC-GP-003 | Straßenfahren ist relativ realistisch, Motorsport deutlich simulationslastiger | 📋 BESCHLOSSEN | `W-FP-002`, `W-FP-008` | FZ | Zwei Wiedergabetreue-Stufen derselben Physik: 💡 `VEH-DEC-PH-002` |
| VEH-DEC-GP-004 | Verkehr ist dynamisch, aber nicht absurd; Stoßzeiten, Radiomeldungen bei Stau, seltene NPC-Unfälle (außer der Spieler rammt), Pannen, Baustellen | 📋 BESCHLOSSEN | `W-TV-001` | VKR | Keine Dichte beschlossen |
| VEH-DEC-GP-005 | Die Welt wird nur so tief simuliert, dass kein offensichtlicher Fake auffällt; glaubwürdige Konsistenz geht vor vollständiger Simulation | 📋 BESCHLOSSEN | `N-LT-011` | ALL | Leitlinie für Stufen in Modul 26 |
| VEH-DEC-GP-006 | Verkehr wird beim Betreten eines Gebiets lokal rekonstruiert, die Straße darf weder leer noch unglaubwürdig sein | 📋 BESCHLOSSEN | `W-TV-009`, `N-LE-013`, `N-LE-026` | VKR NPC | Rekonstruktionsverfahren: 💡 [Modul 26](26-Simulationsstufen-LOD-und-Streaming.md) |
| VEH-DEC-GP-007 | Die Welt läuft im Einzelspielerbetrieb nur, während man spielt | 📋 BESCHLOSSEN | `W-TV-004`, `N-LT-020` | VKR | Online-Verhalten später anders |
| VEH-DEC-GP-008 | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI; ein bewusst herbeigeführter Tod ist etwas anderes | 📋 BESCHLOSSEN | `N-LT-033` | NPC VKR | Konflikt mit unabhängiger Lebensentwicklung: `CK-02` |
| VEH-DEC-GP-009 | Verbrenner dominieren das Spiel; E-Ladeinfrastruktur ist zulässig | 📋 BESCHLOSSEN | `W-FZ-019` | FZ | Antriebsanteil nicht festgelegt (`W-FZ-014`) |
| VEH-DEC-GP-010 | Lange Fahrten sind ein Charakterkanal (Gespräche, Radio, Telefon, Schlaf, Streit, Fallbesprechung, spontane Stopps) | 📋 BESCHLOSSEN | Gameplay-Systeme §6.1; `M-PD` | DLG FZ | Dialogsystem: Nachbardokumentation |
| VEH-DEC-GP-011 | Die Weltgröße ist nirgends fest verdrahtet; das Fahrzeug- und Verkehrssystem skaliert mit Netz und Siedlung | 📋 BESCHLOSSEN | `N-LT-007`; `M-PD` ‚Final map size … calibrated‘ | ALL | Kalibrierung der Weltgröße an Fahrzeiten: `W-GE-026` |
| VEH-DEC-GP-012 | Zug und Bus sind für lange Strecken bevorzugt; Radio-Verkehrsmeldungen bei Stau | 📋 BESCHLOSSEN | `W-VK-014` | VKR OPV | – |
| VEH-DEC-GP-013 | Verkehr ist Teil der NPC-Gesellschaft: NPC-Fahrzeuge gehören NPC-Haushalten und Firmen, es gibt keine eigenständige Verkehrspopulation | 🧩 ABGELEITET | `N-ID-005`, `N-ID-007`, `N-LE-006`, `N-LT-008` | NPC VKR | Begründung und Ausnahmen (Statisten) in Modul 14 |
| VEH-DEC-GP-014 | Verhalten wird über austauschbare Datenprofile je Region und Straßenklasse beschrieben, nicht über feste Konstanten | 💡 VORSCHLAG | `CLAUDE.md` §4; `WF-RG` | ALL | Profilformat offen: `CQ-01` |
| VEH-DEC-GP-015 | Besitzer, Zustand, Standort und Rechtslage eines Fahrzeugs haben je genau einen Eigentümer; Änderungen laufen über Ereignisse | 💡 VORSCHLAG | Folge aus `N-LT-008` | FZ PER | Eigentümerliste in Modul 22 |
| VEH-DEC-GP-016 | Wie viele Wiedergabetreue-Stufen und Assistenzoptionen der Straßenverkehr bietet, ist nicht beschlossen | ❓ OFFEN | `W-FP-007` (🧭 Hooks); `W-FP-010` | FZ UI | `CQ-02` |
| VEH-DEC-GP-017 | Schnellreise (Camps, Safehouses) und der Umgang mit langer Fahrzeit sind nicht entschieden | ❓ OFFEN | Welt [OF-50](../World/22-Offene-Fragen-Katalog.md); `35#87E` (Richtung ‚Durchquerung merkbar lang‘) | FZ WLT | `CQ-03` |
| VEH-DEC-GP-018 | Lange Fahrten sind nicht automatisch tote Zeit: Mitfahrende sprechen, reagieren auf das Radio, telefonieren, schlafen, streiten, besprechen den Fall, bitten um Stopps oder schweigen; Beziehungen können sich entwickeln, ohne dass ständige Minispiele nötig sind | 📋 BESCHLOSSEN | `M-DC31` §47 | DLG FZ NPC | Dialogsystem: Nachbardokumentation |
| VEH-DEC-GP-019 | Der Status der Oktober-15-Fahrzeugdetails ist uneinheitlich: `D15-RV` führt den Phasenvertrag und die DualSense-Belegung als ‚user-approved‘ beziehungsweise ‚user specified‘, der Zusatz vom 18.10.2026 führt die Fahrzeugdetails des Oktober-Pakets als ‚review-required specification inputs‘ | ⚠️ KONFLIKT | `D15-RV`; `M-EP1018`; `M-OC1018` | FZ | `CK-07`; betrifft GP-002, ST-001 bis ST-003, WE-001 |

---

## 6. Offene Punkte

| Frage | Verweis |
|---|---|
| Format der Verhaltensprofile (Datenformat, Versionierung, Mod-Zugriff) | `CQ-01` |
| Wiedergabetreue-Stufen und Assistenzoptionen im Straßenverkehr | `CQ-02` |
| Schnellreise und Umgang mit Fahrzeit | `CQ-03` |
| Reihenfolge der Ausbaustufen für Verkehr gegenüber Fahrzeug | `CQ-04` |
| Umfang des Verkehrs im Vertical Slice (nur das nötige Minimum oder bereits Stoßzeiten) | `CQ-05` |
