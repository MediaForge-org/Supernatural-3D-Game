# Fahrzeuge und Verkehr 15: Verkehrsdichte, Stau, Baustellen und Sperrungen

> [!IMPORTANT]
> **Es gibt keine Dichtewerte und keine Staumodelle.** Beschlossen ist die Absicht: Verkehr schwankt dynamisch (Uhrzeit, Wochentag, Stadtteil, Veranstaltungen, Wetter, Ferien), aber „nicht zu extrem“. Stoßzeiten mit Staus werden im Radio angesagt. Baustellen, Sperren und Umleitungen passen zum Stadtwachstum. Alles Weitere ist Vorschlag. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md), [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Dichte als Profil](#2-dichte-als-profil)
3. [Stau](#3-stau)
4. [Baustellen](#4-baustellen)
5. [Sperrungen und Umleitungen](#5-sperrungen-und-umleitungen)
6. [Meldungen](#6-meldungen)
7. [Zeit und Zeitsprünge](#7-zeit-und-zeitsprünge)
8. [Register](#8-register)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Dichte | Dynamisch nach Uhrzeit, Wochentag, Stadtteil, Veranstaltungen, Wetter und Ferien, „aber nicht zu extrem“ | `23#1025E` Nr. 1 (Antwort: ‚ja aber nicht zu extrem‘); `W-TV-001` |
| Stoßzeit | Staus zu Stoßzeiten sind möglich; ein Stau wird im Radio angesagt | `23#1025E` Nr. 2; `W-VK-014` |
| Baustellen | Temporäre Baustellen, Sperren und Umleitungen passend zum Stadtwachstum | `23#1025E` Nr. 5; `W-TV-001` |
| Veranstaltungen | Autotreffen, Sport, Clubnächte, Konzerte, Märkte, Kämpfe, Motorsport, Festivals beeinflussen Verkehr, ÖPNV und Parken | `W-UR-009` |
| Rennsperren | Stadtrennen und Rallyes sperren Straßen an Ereigniszeiten | `W-UR-010` |
| Zwischenfälle | Dynamische Zwischenfälle verursachen dauerhafte Schäden; NPC-Trupps reparieren über glaubhafte Zeit; der Spieler kann helfen | `W-UR-012`; `N-LE-025` |
| Tatorte | Absperrungen nach größeren Verbrechen; Tatorte bleiben bis zur Reinigung oder Reparatur | `COMBAT-DEC-PO-016`, `COMBAT-DEC-PO-004` |
| Routinen | NPC-Routinen nach Wochentag, Arbeit, Wetter, Krankheit, Feiertag, Ereignis | `W-TV-002`; `M-WB89` Block 8 |
| Zeit | Vollkalender, beschleunigt; etwa zwei Echtstunden je Spieltag sind nicht gesperrt | `M-WB17` Block 6 |

Die Antwort ‚kann man machen‘ zur Rush-Hour-Frage ist weicher als ein „ja“. `W-TV-001` führt Stoßzeiten als beschlossen; die Dokumentation übernimmt das und vermerkt den Wortlaut.

---

## 2. Dichte als Profil

> **💡 VORSCHLAG.**

Die Dichte einer Straße zu einem Zeitpunkt ist eine **Funktion** aus:

| Eingabe | Bezug |
|---|---|
| Straßenklasse und Bezirk | [Modul 10](10-Strassennetz-und-Strassentypen.md), [Modul 12](12-Stadtverkehr-Parken-und-Garagen.md) |
| Tageszeit und Wochentag | `W-TV-002` |
| Feiertag und Ferien | `M-WB89` Block 8; Frage 1 des Fahrzeugblocks |
| Veranstaltungen und Sperrungen | `W-UR-009`, `W-UR-010` |
| Wetter | Frage 1 des Fahrzeugblocks; [Modul 04](04-Fahrphysik-und-Fahrverhalten.md) |
| Nachfrage aus den Tagesabläufen | [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md) |

Die Funktion liegt als **Dichtekurven-Profil** in Daten (`VEH-DEC-GP-014`). Die Kurven selbst sind nicht belegt. Ein Ferienkalender ist nirgends beschlossen (`CQ-60`).

**„Nicht zu extrem“** ist eine Anforderung an die Wirkung. Sie lässt sich als **Dämpfung oder Obergrenze** umsetzen, deren Wert einstellbar bleibt (`CQ-61`). Ob sie vom Schwierigkeitsgrad oder von den Einstellungen abhängt, ist nicht festgelegt.

---

## 3. Stau

Die Quellen verlangen, dass es Staus gibt und dass sie gemeldet werden. Sie verlangen nicht, wie sie entstehen.

> **💡 VORSCHLAG.**
>
> - **Entstehung aus Nachfrage und Kapazität:** Die Nachfrage ergibt sich aus den Tagesabläufen (Modul 14), die Kapazität aus Spuren, Klasse, Sperrungen und Baustellen. Ein Stau ist dann die Folge, wenn Nachfrage die Kapazität übersteigt, und kein skriptbares Ereignis. Das passt zu `N-LE-006` (ohne Reisen keine Staus).
> - **Ferne:** Der Stau ist ein **Zustand je Straßenkante** (Verzögerungsfaktor), keine Menge einzelner Fahrzeuge. Er wird für Routenwahl und Fahrzeit benutzt (Modul 13, 14).
> - **Nähe:** Beim Annähern wird aus dem Zustand eine Fahrzeugschlange rekonstruiert (`VEH-DEC-GP-006`).
> - **Spielerwirkung:** Ein Unfall, den der Spieler auslöst (`W-TV-001`), verringert die Kapazität und kann so einen Stau erzeugen. Der Spieler kann Staus sehen, hören (Radio) und umfahren.
>
> Welches Staumodell im Detail gilt (Agenten oder Fluss), hängt am Verkehrsmodell (`CQ-49`) und an der Ferne-Darstellung (`CQ-63`).

---

## 4. Baustellen

Baustellen, Sperren und Umleitungen sollen zum langfristigen Stadtwachstum passen (`23#1025E` Nr. 5; `W-UR-018`).

> **💡 VORSCHLAG.** Eine Baustelle ist ein **Weltobjekt mit Zeitraum**:
>
> ```text
> RoadWork
>   work_id, edge_refs[], start_world_time, end_world_time
>   effect          (Sperrung / Spurreduktion / Tempoverringerung)
>   detour_ref      (optional)
>   signage_refs[]  (Modul 11)
>   source          (Stadtentwicklung / Reparatur nach Zwischenfall / Wartung)
> ```
>
> Zwei Quellen:
>
> 1. **Stadtentwicklung:** geplante Projekte, die fertig werden (`W-UR-018`).
> 2. **Reparatur nach Zwischenfällen:** NPC-Trupps reparieren Straßen über glaubhafte Zeit, der Spieler kann helfen (`W-UR-012`, `N-LE-025`).
>
> Beide erzeugen dieselbe Struktur. Die Straße verändert sich, und das Netz ist nach Ende der Baustelle ein anderes. Die Netzversion gehört in den Weltzustand (Modul 22, 24).

---

## 5. Sperrungen und Umleitungen

> **💡 VORSCHLAG.** Eine **Sperrung** ist ein Ereignis mit Ursache, Geltungsbereich (Kanten) und Dauer. Mögliche Ursachen aus den Quellen:

| Ursache | Quelle |
|---|---|
| Stadtrennen, Rallye-Etappen | `W-UR-010` |
| Veranstaltungen | `W-UR-009`, `W-VK-013` |
| Unfall, Zwischenfall | `W-TV-001`, `W-UR-012` |
| Tatort, Absperrung nach größerem Verbrechen | `COMBAT-DEC-PO-016`, `COMBAT-DEC-PO-004` |
| Baustelle | `23#1025E` Nr. 5 |
| Wetter, Jahreszeit (Schnee, Hochwasser, Sturm) | `M-WB17` Block 6; `23#1025E` Nr. 49 |

Routenplanung (Modul 13) liest Sperrungen als Kosten- oder Verbotsänderung, Beschilderung (Modul 11) macht sie sichtbar, und Meldungen (Abschnitt 6) tragen sie in die Welt. Eine Umleitung ist eine Route, die durch Schilder und NPC-Routenwahl entsteht, nicht ein eigener Pfad.

---

## 6. Meldungen

Verkehrsmeldungen im Radio bei Stau sind beschlossen (`W-VK-014`). Die Dokumentation schlägt vor, dass Meldungen aus der **Verkehrslage** abgeleitet werden (`VEH-DEC-KI-007`), mit Verzögerung und ohne Allwissen (`N-LT-013`: NPCs wissen, was sie gesehen oder gehört haben). Umfang, Sender und Latenz sind nicht belegt (`CQ-64`). Warn-Apps für Polizeikontrollen sind beschlossen als Richtung (`E-QA-042`); ihr Zusammenhang mit Stau und Sperrungen ist offen.

---

## 7. Zeit und Zeitsprünge

Die Welt läuft nur, während man spielt (`W-TV-004`); Zeitsprünge werden nachgeholt (`N-SI-014`). Eine Spielzeit von etwa zwei Echtstunden je Spieltag ist nicht gesperrt (`M-WB17` Block 6). Für den Verkehr folgt:

> **💡 VORSCHLAG.** Nach einem Zeitsprung (Schlaf, Warten) wird die Verkehrslage nicht nachsimuliert, sondern aus Dichtekurven, Ereignissen und Baustellen **neu abgeleitet**. Die Tagesablauf-Zustände der NPCs werden nach dem NPC-Verfahren fortgeschrieben ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md)).

Die Zeitskala wirkt auf Pendeldauern: Zwei Echtstunden je Spieltag bedeuten, dass eine Stunde Fahrzeit im Spiel wenige Echtminuten ist. Ob die Straßenverkehrszeiten in Weltzeit oder in einer anderen Skala laufen, ist nicht entschieden (`CQ-60`).

---

## 8. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-VD-001 | Die Verkehrsdichte schwankt dynamisch nach Uhrzeit, Wochentag, Stadtteil, Veranstaltungen, Wetter und Ferien, aber nicht zu extrem | 📋 BESCHLOSSEN | `23#1025E` Nr. 1; `W-TV-001` | VKR | Werte: `CQ-60`, `CQ-61` |
| VEH-DEC-VD-002 | Staus zu Stoßzeiten sind möglich und werden im Radio angesagt | 📋 BESCHLOSSEN | `23#1025E` Nr. 2; `W-VK-014`; `W-TV-001` | VKR UI | Der Wortlaut ‚kann man machen‘ ist weicher als ‚ja‘ |
| VEH-DEC-VD-003 | Temporäre Baustellen, Sperren und Umleitungen passend zum Stadtwachstum sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 5; `W-TV-001` | VKR WLT | – |
| VEH-DEC-VD-004 | Veranstaltungen, Stadtrennen und Rallye-Etappen beeinflussen oder sperren den Verkehr | 📋 BESCHLOSSEN | `W-UR-009`; `W-UR-010` | VKR WLT | – |
| VEH-DEC-VD-005 | Dynamische Zwischenfälle verursachen dauerhafte Schäden, NPC-Trupps reparieren über glaubhafte Zeit, der Spieler kann helfen | 📋 BESCHLOSSEN | `W-UR-012`; `N-LE-025` | WLT NPC | – |
| VEH-DEC-VD-006 | Nach größeren Verbrechen gibt es Absperrungen; Tatorte bleiben bis zur Reinigung oder Reparatur | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-016`; `COMBAT-DEC-PO-004` | POL ERM | – |
| VEH-DEC-VD-007 | Die Dichte ist eine Funktion aus Straßenklasse, Bezirk, Tageszeit, Wochentag, Feiertag und Ferien, Ereignissen, Wetter und Nachfrage und wird als Dichtekurven-Profil in Daten geführt | 💡 VORSCHLAG | Ableitung aus `W-TV-001`, `W-TV-002` | VKR | – |
| VEH-DEC-VD-008 | ‚Nicht zu extrem‘ wird als einstellbare Dämpfung oder Obergrenze der Dichte umgesetzt | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 1 | VKR UI | Wert und Abhängigkeit: `CQ-61` |
| VEH-DEC-VD-009 | Staus entstehen aus Nachfrage (Tagesabläufe) und Kapazität (Spuren, Sperrungen); in der Ferne sind sie ein Verzögerungsfaktor je Kante, in der Nähe werden sie als Fahrzeugschlange rekonstruiert | 💡 VORSCHLAG | Ableitung aus `N-LE-006`, `N-LE-013` | VKR LS | `CQ-63` |
| VEH-DEC-VD-010 | Eine Baustelle ist ein Weltobjekt mit Zeitraum, Wirkung, Umleitung und Beschilderung; Quellen sind Stadtentwicklung und Reparatur nach Zwischenfällen | 💡 VORSCHLAG | Ableitung aus `W-UR-018`, `W-UR-012` | VKR WLT | Erzeugung: `CQ-62` |
| VEH-DEC-VD-011 | Eine Sperrung ist ein Ereignis mit Ursache, Geltungsbereich und Dauer; Routenplanung liest sie, Beschilderung zeigt sie | 💡 VORSCHLAG | – | VKR KI | – |
| VEH-DEC-VD-012 | Verkehrsmeldungen werden aus der Verkehrslage abgeleitet, mit Verzögerung und ohne Allwissen | 💡 VORSCHLAG | Ableitung aus `W-VK-014`, `N-LT-013` | UI VKR | Umfang: `CQ-64` |
| VEH-DEC-VD-013 | Nach einem Zeitsprung wird die Verkehrslage neu abgeleitet statt nachsimuliert | 💡 VORSCHLAG | Ableitung aus `N-SI-014` | VKR LS | – |
| VEH-DEC-VD-014 | Konkrete Dichtekurven, Ferienkalender und Staugrenzen sind nicht belegt | 🔍 UNBELEGT | – | VKR | `CQ-60` |
| VEH-DEC-VD-015 | Wie Wetter die Verkehrsdichte und die Unfallrate verändert, ist nur für die Fahrphysik belegt, nicht für die Dichte | 🔍 UNBELEGT | `23#1025E` Nr. 1, 48 | VKR WET | `CQ-60` |
| VEH-DEC-VD-016 | Wie viele Baustellen und Sperrungen es gibt und wie sie erzeugt werden, ist nicht belegt | ❓ OFFEN | – | WLT VKR | `CQ-62` |

---

## 9. Offene Punkte

| Frage | Verweis |
|---|---|
| Dichtekurven, Ferienkalender, Zeitskala | `CQ-60` |
| Dämpfung ‚nicht zu extrem‘ | `CQ-61` |
| Erzeugung von Baustellen und Sperrungen | `CQ-62` |
| Staumodell in der Ferne | `CQ-63` |
| Umfang der Verkehrsmeldungen | `CQ-64` |
