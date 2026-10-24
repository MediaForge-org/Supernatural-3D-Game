# Fahrzeuge und Verkehr 14: NPC-Fahrer und NPC-Fahrzeuge

> [!IMPORTANT]
> **Dieses Modul schafft keine zweite NPC-Architektur.** Verkehr ist Teil der NPC-Gesellschaft ([NPCs](../NPCs/README.md)). NPCs besitzen Fahrzeuge, pendeln, verreisen und reagieren auf Ereignisse. Beschlossen ist der Rahmen; die Einbindung der Fahrzeuge in Persistenzstufen und Simulationsstufen ist Vorschlag und benutzt die Begriffe des NPC-Entwurfs (Persistenzstufen A bis D, Simulationsstufen S0 bis S3 und SD). Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [NPCs 03](../NPCs/03-Identitaet-Persistenzstufen-und-Datensatztiefe.md), [NPCs 13](../NPCs/13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md), [NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md), [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Fahrzeuge in den Persistenzstufen](#2-fahrzeuge-in-den-persistenzstufen)
3. [Fahrten im Tagesablauf](#3-fahrten-im-tagesablauf)
4. [Berufsfahrer und Betriebsfahrzeuge](#4-berufsfahrer-und-betriebsfahrzeuge)
5. [Verkehrsmenge und Statisten](#5-verkehrsmenge-und-statisten)
6. [Verlassen des Simulationsbereichs](#6-verlassen-des-simulationsbereichs)
7. [Reaktion auf Unfälle und Blockaden](#7-reaktion-auf-unfälle-und-blockaden)
8. [Register](#8-register)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Eigenes Fahrzeug | NPCs benutzen wiederholt dasselbe eigene Fahrzeug statt jeden Tag ein anderes | `N-ID-005` |
| Besitz | Wichtige NPCs besitzen Wohnungen, Fahrzeuge, Haustiere und Gegenstände persistent | `N-ID-007`, `N-FA-016` |
| Reisen | NPCs machen Wochenendtrips, Urlaub und Besuche in andere Städte: „ja natürlich, sonst könnten nie staus sein“ | `N-LE-006` |
| Ferne | Ferne NPCs werden als Zustand (`AT_WORK`, `HOME`, `TRAVEL`) geführt, nicht als exakte Bewegung; beim Annähern entsteht ein konsistenter detaillierter NPC | `N-LE-013` |
| Straße nie leer | Bei Besuch einer Gegend soll die Straße nicht leer sein | `N-LE-026` |
| Fahrer | Können und Fahrstile, Wetteranpassung, Rettungsgassen | `N-LE-024`, `W-FP-009` |
| Wiedererkennung | NPCs können auffällige Fahrzeuge wiedererkennen | `N-GE-018` |
| Persistenzstufen | A Story bis D Menge; wichtige NPCs bleiben dieselben Personen | `N-LT-016` (🧭), `N-LT-017` |
| Schutz | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI | `N-LT-033` |
| Zeugen | Zeugen können Polizei und Rettung rufen | `N-WA-011` |
| Arbeitsplätze | Wohnort, Arbeitsplätze, Geschäfte, Verkehr als Teil der Alltagsroutinen | `W-TV-002` |
| Begleiter | Begleiter fahren teils mit eigenem Auto oder Motorrad | `N-CO-005` |

---

## 2. Fahrzeuge in den Persistenzstufen

> **💡 VORSCHLAG.** Der NPC-Entwurf führt kompakte Datensätze für unbekannte NPCs (ID, Seed, Overrides; `N-SI-008`, `N-SI-013`). Fahrzeuge folgen derselben Linie.

| Persistenzstufe (NPC) | Fahrzeug (Vorschlag) |
|---|---|
| **A** Story, Hunter, enge Beziehungen | voller Fahrzeugdatensatz (Modul 03), eigene Historie, wiedererkennbar |
| **B** Händler, Nachbarn, Arbeitgeber, wiederkehrende Kontakte | voller Fahrzeugdatensatz |
| **C** normale Einwohner | Fahrzeug **aus NPC-ID und Seed erzeugt**; gespeichert wird nur, was vom Erzeugten abweicht (Kauf, Schaden, Umbau, Besitzerwechsel) |
| **D** Menge | kein eigenes Fahrzeug; Verkehrsmenge (Abschnitt 5) |

Der Vorteil: Ein C-NPC kostet im Speicherstand kein volles Fahrzeug, aber ein Fahrzeug, das der Spieler berührt (beschädigt, stiehlt, durchsucht), wird zum vollen Datensatz. Dieser Übergang entspricht der beschlossenen Linie, dass die Genauigkeit steigt, sobald ein NPC für den Spieler wichtiger wird (`N-SI-010`). Wie viele Abweichungen im Save landen und in welchem Format, ist offen (`CQ-57`).

**Besitzer:** Ein Fahrzeug hat genau einen Eigentümer (NPC, Haushalt, Firma, Stadt, Spieler); Besitzerwechsel sind Ereignisse (`VEH-DEC-GP-015`, Modul 18).

---

## 3. Fahrten im Tagesablauf

> **💡 VORSCHLAG.** Eine Fahrt ist für den NPC ein **Zustandswechsel** zwischen Orten.

```text
HOME  --Aufbruch-->  TRAVEL(start, ziel, route_ref, erwartete_ankunft, fahrzeug_ref)  --Ankunft-->  AT_WORK
```

- **Ferne (S2, S3):** keine Bewegung, nur Zustand und erwartete Ankunftszeit. Die Dauer ergibt sich aus dem Straßengraph und der Verkehrslage (Modul 15), nicht aus einer Simulation. NPCs sind nicht an zwei Orten gleichzeitig; Reisen kostet Zeit (`N-SI-011`).
- **Nähe (S1, S0):** Der Zustand TRAVEL wird in eine Fahrt auf der Straße übersetzt. Position entlang der Route ergibt sich aus Start, Dauer und Zeit; danach übernimmt die Verkehrs-KI (Modul 13).
- **Parken:** Am Ziel entsteht ein geparktes Fahrzeug (Modul 12).
- **Zufall:** Aufbruchzeit und Routenvariante entstehen deterministisch aus NPC-ID, Welt-Seed und Zeitfenster (`VEH-DEC-KI-010`).

Der Pendlerverkehr der Stoßzeiten (Modul 15) entsteht dadurch **aus den Tagesabläufen**, nicht aus einer eigenen Verkehrsregel. Das ist die Antwort auf die Anforderung, dass es ohne Reisen der NPCs keine Staus gäbe (`N-LE-006`).

---

## 4. Berufsfahrer und Betriebsfahrzeuge

Taxi, Mitfahrdienst, Lieferdienst und Trucking sind als Spielerjobs beschlossen (`W-WI-005`), Güterverkehr per Lkw, Bahn und Hafen ist Teil der Welt (`W-VK-011`), Abschleppdienst und Notdienste sind Teil der Simulation (`W-TV-006`).

> **💡 VORSCHLAG.** Dieselben Rollen sind **NPC-Berufe**: Der Taxifahrer ist ein NPC mit Arbeitsplatz, sein Fahrzeug gehört dem Betrieb (Eigentümer: Firma), sein Standort ist ein Depot ([Modul 12](12-Stadtverkehr-Parken-und-Garagen.md)). Er fährt zu Zeiten seines Dienstes. Das Verkehrsangebot (Taxi, Bus, Lieferung) wird so aus der Wirtschaft abgeleitet. Fahrpläne für Busse sind in den Quellen nicht belegt (nur Bahn-Bedienungsdichten, `W-VK-005`).

---

## 5. Verkehrsmenge und Statisten

Nicht jedes Fahrzeug auf der Straße gehört einem persistenten NPC. Die Hauptwelt ist groß, und die Quellen verlangen glaubwürdige Straßen beim Betreten (`N-LE-026`). Der NPC-Entwurf kennt dafür **SD**: instanzierte Mengen ohne eigene Identität, aus Ortszustand erzeugt (`N-SI-007`).

> **💡 VORSCHLAG.** Verkehr wird in zwei Quellen zusammengesetzt:
>
> 1. **Persönlicher Verkehr:** NPCs im Zustand TRAVEL, deren Fahrt in die Nähe des Spielers fällt, werden zu Fahrzeugen mit Insassen (S1 oder S0).
> 2. **Verkehrsmenge:** Die verbleibende Dichte, die nach Straßenklasse, Tageszeit und Lage glaubwürdig ist, wird mit Fahrzeugen **ohne eigene Identität** aufgefüllt (Durchgangsverkehr, Gäste, Lieferungen von außen).
>
> Statisten sind wegwerfbar, solange nichts mit ihnen geschieht. Werden sie Zeuge, Unfallbeteiligter, Opfer oder Gesprächspartner, wird der Statist zu einem NPC mit einem kleinen Datensatz **befördert** (Beförderung ohne Verlust, `N-SI-018`). Das ist nötig, weil Ermittlung Zeugen mit Identität braucht (`E-ZE-002`, Ermittlung) und weil der Spieler ‚essentially every NPC‘ ansprechen kann (`M-WB89` Block 8).
>
> Das Verhältnis von persönlichem Verkehr zu Verkehrsmenge ist nicht belegt (`CQ-58`).

---

## 6. Verlassen des Simulationsbereichs

NPCs verreisen, machen Urlaub und besuchen andere Städte (`N-LE-006`). Die Quellen sagen nicht, ob diese Ziele **auf der Karte** oder **außerhalb** liegen. Alle Züge bleiben innerhalb der Karte (`W-VK-001`); für Straßenreisen und Flüge gibt es keine Aussage.

> **💡 VORSCHLAG.** Reisen innerhalb der Karte laufen als TRAVEL-Fahrten. Reisen **außerhalb** werden als Zustand `ABWESEND(bis t)` geführt: Der NPC ist nicht in der Welt, hat eine Rückkehrzeit, sein Fahrzeug steht am Heimatort oder reist mit (offen). Solche Abwesenheiten erklären, warum nicht alle NPCs ständig im Verkehr sind, und sie sind billig (nur ein Zeitstempel).
>
> Ob die Spielwelt überhaupt ‚Außenwelt‘ kennt, ist eine Weltfrage (`CQ-55`).

---

## 7. Reaktion auf Unfälle und Blockaden

Beschlossen ist, dass die Welt auf Ereignisse reagiert, dass Zeugen Notrufe absetzen und dass Rettungsgassen gebildet werden (`N-LE-025`, `N-WA-011`, `N-LE-024`).

> **💡 VORSCHLAG.** NPC-Fahrer in der Nähe eines Unfalls oder einer Blockade wählen je nach Fahrstil und Lage: anhalten und warten, umfahren (Routenkosten steigen, Modul 13), Notruf absetzen (nur wenn Zeuge im Sinn der Ermittlung), weiterfahren. Die Verteilung ist nicht belegt. Fahrer entfernter Gebiete erfahren von Staus und Sperrungen durch Verkehrsmeldungen im Radio und im Smartphone (`W-VK-014`, `M-CA0905`) und planen entsprechend um, ohne den Unfall zu ‚sehen‘ (kein magisches Allwissen, `N-LT-013`).

---

## 8. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-NF-001 | NPCs benutzen wiederholt dasselbe eigene Fahrzeug | 📋 BESCHLOSSEN | `N-ID-005` | NPC FZ | – |
| VEH-DEC-NF-002 | Wichtige NPCs besitzen Fahrzeuge persistent | 📋 BESCHLOSSEN | `N-ID-007`, `N-FA-016` | NPC FZ PER | Grenze ‚wichtig‘ folgt Persistenzstufen |
| VEH-DEC-NF-003 | NPCs verreisen, machen Urlaub und besuchen andere Städte, sonst gäbe es keine Staus | 📋 BESCHLOSSEN | `N-LE-006` | NPC VKR | Ziele auf oder außerhalb der Karte: `CQ-55` |
| VEH-DEC-NF-004 | Ferne NPCs werden als Zustand geführt (`AT_WORK`, `HOME`, `TRAVEL`), nicht als exakte Bewegung | 📋 BESCHLOSSEN | `N-LE-013` | NPC LS | – |
| VEH-DEC-NF-005 | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI | 📋 BESCHLOSSEN | `N-LT-033` | NPC KI | `CK-02`, `CQ-52` |
| VEH-DEC-NF-006 | Zeugen können Polizei und Rettung rufen; die Welt reagiert auf Ereignisse | 📋 BESCHLOSSEN | `N-WA-011`; `N-LE-025` | NPC POL | – |
| VEH-DEC-NF-007 | Fahrzeuge der Persistenzstufen A und B sind volle Fahrzeugdatensätze; Fahrzeuge der Stufe C werden aus NPC-ID und Seed erzeugt und speichern nur Abweichungen | 💡 VORSCHLAG | Ableitung aus `N-SI-008`, `N-SI-013`, `N-LT-016` | NPC FZ PER | Format: `CQ-57` |
| VEH-DEC-NF-008 | Ein berührtes Fahrzeug (beschädigt, gestohlen, durchsucht) wird zu einem vollen Datensatz | 💡 VORSCHLAG | Ableitung aus `N-SI-010` | FZ PER | – |
| VEH-DEC-NF-009 | Eine Fahrt ist ein Zustand TRAVEL (Start, Ziel, Route, erwartete Ankunft, Fahrzeug); in der Ferne ohne Bewegung, in der Nähe als Fahrt auf der Straße | 💡 VORSCHLAG | Ableitung aus `N-LE-013`, `N-SI-011` | NPC LS | – |
| VEH-DEC-NF-010 | Der Pendlerverkehr der Stoßzeiten entsteht aus den Tagesabläufen der NPCs, nicht aus einer eigenen Verkehrsregel | 🧩 ABGELEITET | `N-LE-006`; `W-TV-001`; `W-TV-002` | NPC VKR | Dichte: Modul 15 |
| VEH-DEC-NF-011 | Berufsfahrer (Taxi, Bus, Lieferung, Trucking) sind NPCs mit Arbeitsplatz; ihre Fahrzeuge gehören dem Betrieb | 💡 VORSCHLAG | Ableitung aus `W-WI-005`, `W-TV-002`, `W-TV-006` | NPC WIR | Busfahrpläne nicht belegt |
| VEH-DEC-NF-012 | Der Verkehr setzt sich aus persönlichem Verkehr (NPCs im Zustand TRAVEL) und einer Verkehrsmenge ohne Identität zusammen; Statisten werden bei Beteiligung befördert | 💡 VORSCHLAG | Ableitung aus `N-SI-007`, `N-SI-018`, `N-LE-026` | NPC VKR LS | Verhältnis: `CQ-58` |
| VEH-DEC-NF-013 | Reisen außerhalb der Karte werden als Zustand `ABWESEND(bis t)` geführt | 💡 VORSCHLAG | Ableitung aus `N-LE-006` | NPC | Hängt an `CQ-55` |
| VEH-DEC-NF-014 | NPC-Fahrer reagieren auf Unfälle und Blockaden nach Fahrstil und Lage (Anhalten, Umfahren, Notruf, Weiterfahren); Ferne erfährt von Störungen über Meldungen | 💡 VORSCHLAG | Ableitung aus `N-LE-024`, `N-LT-013`, `W-VK-014` | NPC KI | Verteilung nicht belegt |
| VEH-DEC-NF-015 | Der Anteil der NPCs mit eigenem Fahrzeug, Mehrfahrzeughaushalte und Fahrgemeinschaften sind nicht belegt | 🔍 UNBELEGT | – | NPC FZ | `CQ-56` |
| VEH-DEC-NF-016 | Wie NPCs über Jahre Fahrzeuge kaufen, verkaufen, wechseln und altern, ist nicht belegt | 🔍 UNBELEGT | `N-LT-006` (Umzug, Beruf) ohne Fahrzeugaussage | NPC FZ | `CQ-59` |

---

## 9. Offene Punkte

| Frage | Verweis |
|---|---|
| Ziele außerhalb der Karte | `CQ-55` |
| Anteil Fahrzeugbesitz, Haushalte | `CQ-56` |
| Speicherformat der abweichenden Fahrzeuge der Stufe C | `CQ-57` |
| Verhältnis persönlicher Verkehr zu Verkehrsmenge | `CQ-58` |
| Fahrzeugwechsel und Alterung bei NPCs | `CQ-59` |
