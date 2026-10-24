# Fahrzeuge und Verkehr 27: Performance und Skalierung

> [!IMPORTANT]
> **Es gibt keine Messwerte, keine Budgets und keine Zielhardware für Fahrzeuge und Verkehr.** Beschlossen ist, dass Leistung von Anfang an mitgeplant wird und dass keine Zahl die Weltgröße festschreiben darf. Die Planungsgröße von rund 500.000 NPC-Identitäten (🧭) ist **keine** Zahl für Fahrzeuge. Dieses Modul benennt Kostentreiber, ordnet sie nach dem, womit sie wachsen, und beschreibt einen Messplan. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md), [Modul 26](26-Simulationsstufen-LOD-und-Streaming.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Kostentreiber und womit sie wachsen](#2-kostentreiber-und-womit-sie-wachsen)
3. [Maßnahmen im Entwurf](#3-maßnahmen-im-entwurf)
4. [Messplan](#4-messplan)
5. [Große Städte](#5-große-städte)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Frühzeitig | Performance-Budgets sind von Anfang an gemeinsam zu planen; Optimierung ist ein globales Kernziel | `N-SI-015`; `M-PD` (Rendering) |
| Hardware | Hardwarestufen und konkrete Budgets sind offen | [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §1, §8 |
| Skalierung | Hohe Genauigkeit in Spielernähe, reduzierte Simulation in der Ferne; die Weltgröße ist nirgends fest verdrahtet | `N-LT-007` |
| NPC-Zahl | Rund 500.000 persistente NPC-Identitäten sind Planungsgröße, nicht beschlossen | `W-TV-010` (🧭) |
| Verkehr | Ressourcen für Verkehr, Menschenmengen und Bahnbetrieb werden mit Detailstufen abstrahiert; Budgets und Leistungsstufen sind offen | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §8; `D15-OQ` |
| Speicher | Langzeitziel für den Spielstand etwa 20–50 MB, Zahl umstritten | `N-PE-004`; `CK-04` |
| Tests | Streaming-, Last- und Leistungs-Regressionstests sind verlangt | `M-PD` (Testing) |

Eine Bevölkerung von Hunderttausenden NPC-Identitäten bedeutet **nicht** Hunderttausende Fahrzeuge in Bewegung. Ein Fahrzeug gehört nur zu einem NPC, wenn seine Stufe es verlangt (`VEH-DEC-NF-007`), und die meisten sind zu jedem Zeitpunkt geparkt oder in S2 und S3.

---

## 2. Kostentreiber und womit sie wachsen

> **💡 VORSCHLAG.** Die Tabelle ordnet Kosten danach, womit sie wachsen. Das hilft, Maßnahmen dort anzusetzen, wo sie wirken, ohne Zahlen zu erfinden.

| Kostentreiber | Wächst mit | Stufe |
|---|---|---|
| Fahrphysik (Räder, Federung, Kollision) | Zahl der Fahrzeuge in S0 | S0 |
| Verkehrs-KI Taktik und Operation | Zahl der Fahrzeuge in S1 und deren Entscheidungsrate | S0, S1 |
| Wahrnehmung und Abfragen (Traces, Nachbarn) | Fahrzeuge mal Abfragen je Fahrzeug | S0, S1 |
| Darstellung (Instanzen, Materialien, Beleuchtung) | sichtbare Fahrzeuge, Szenentiefe | S0, S1, SD |
| Audio | hörbare Fahrzeuge | S0, S1 |
| Routenplanung | Zahl der Neuplanungen mal Netzgröße | S1, S2 |
| Fortschreiben der Ferne | Zahl der Fahrzeuge und Ereignisse je Weltzeit | S2, S3 |
| Materialisierung | Zahl der Fahrzeuge, die gleichzeitig entstehen | Übergänge |
| Streaming von Straßen | Straßenlänge und Geometriedichte im Ladebereich | – |
| Speicherstand | Zahl der Abweichungen und gespeicherten Datensätze | S3, Archiv |
| Netzwerk | Fahrzeuge in Relevanz mal Spielerzahl | Koop |
| Weltzustand der Straße | Baustellen, Sperrungen, Unfallstellen | – |

**Folgerungen** (aus den Kostentreibern, nicht aus Messungen):

1. Die teuren Stufen (S0, S1) müssen **klein und geschützt** bleiben; die Menge liegt in S2 und S3.
2. Alles, was mit **Netzgröße** wächst (Streaming, Routenplanung), braucht Hierarchie und Zwischenspeicher (`CQ-50`).
3. Alles, was mit **Bevölkerung** wächst, muss am Datensatz sparen (Standard aus ID und Seed).
4. Alles, was mit **Zeit** wächst (Fortschreiben), muss ereignisbasiert und mit Verdichtung arbeiten (`N-SI-011`).

---

## 3. Maßnahmen im Entwurf

> **💡 VORSCHLAG.**

- **Dichte als Einstellung:** Die Obergrenze der Verkehrsdichte (Modul 15, `CQ-61`) ist zugleich ein Leistungsregler. So lässt sich ‚nicht zu extrem‘ gegen die Hardware abwägen, ohne die Welt zu verändern.
- **Degradation:** Bei Lastspitzen werden zuerst Aktualisierungsraten gesenkt, dann Stufenradien verkleinert, dann Verkehrsmenge (SD) verringert. Die Mindestgarantie ‚die Straße ist nie leer‘ (`N-LE-026`) gilt als untere Schranke ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §8).
- **Gestaffelte Materialisierung:** Beim Betreten einer dichten Region entstehen Fahrzeuge über mehrere Bilder, zuerst Nahbereich und wichtige Fahrzeuge.
- **Gemeinsame Daten:** Fahrzeugtypen und Verhaltensprofile sind geteilte Daten, keine Kopien je Fahrzeug.
- **Kein Fahrzeug pro NPC erzwungen:** Datensätze entstehen erst bei Abweichung (`VEH-DEC-NF-007`, `VEH-DEC-PE-008`).

---

## 4. Messplan

`CLAUDE.md` §7 verlangt, keine Leistungszahlen zu fabrizieren. Die Zahlen werden gemessen, nicht angenommen.

> **💡 VORSCHLAG.**

| Messgröße | Szenario (Beispiele) |
|---|---|
| Bildzeit nach Stufen (S0, S1, SD getrennt) | Innenstadt zur Stoßzeit; Autobahn mit hoher Geschwindigkeit; leere Landstraße als Referenz |
| Speicher je Fahrzeug und Stufe | Region mit vielen geparkten Fahrzeugen |
| Zeit der Routenplanung | lange Fernroute; viele gleichzeitige Neuplanungen nach einer Sperrung |
| Materialisierungszeit | Betreten einer dichten Region; Schnellreise (falls vorhanden) |
| Fortschreiben | Zeitsprung von mehreren Spielstunden |
| Speicherstandgröße und Ladezeit | Spielstand nach langer Spielzeit |
| Netzwerklast | 4 Spieler an verschiedenen Orten |
| Streaming | schnelle Fahrt über Zellgrenzen |

Der Messplan benutzt dieselben Szenarien wie die Tests in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md); Messwerte stehen erst in Berichten nach ihrer Ausführung, nicht in diesem Dokument.

---

## 5. Große Städte

Metropolis soll eine starke Hochhausdichte haben (`W-UR-015`, 🧭) und ist der Ort mit dem höchsten Verkehrs- und Fußgängeraufkommen. Die Quellen nennen keine Verkehrsmenge für die Stadt. Die Flächen sind Szenarien: Bei 2.200 km² werden etwa 20 bis 25 km² für die Metropole und 8 bis 10 km² für andere große Städte als Testszenarien genannt, nicht als Entscheidung (`M-A1015`, PROVISIONAL).

> **💡 VORSCHLAG.** Große Städte erhöhen die Kosten vor allem durch: dichte Kreuzungen und Signale (Weltzeit-abgeleitet, billig), viele Fußgänger (SD), viele geparkte Fahrzeuge (Kapazität statt Lücken, `VEH-DEC-SP-007`), Beschilderung und Darstellung. Die Bezirksprofile (`VEH-DEC-SP-010`) steuern die Dichte je Bezirk. Eine Stadt braucht kein anderes System, nur andere Profile und einen strengeren Blick auf die Darstellungskosten.

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-PF-001 | Performance-Budgets werden von Anfang an mitgeplant; Optimierung ist ein globales Kernziel | 📋 BESCHLOSSEN | `N-SI-015`; `M-PD` (Rendering) | PF | – |
| VEH-DEC-PF-002 | Hardwarestufen und konkrete Budgets sind offen | ❓ OFFEN | [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §1, §8 | PF | `CQ-115`, `CQ-116` |
| VEH-DEC-PF-003 | Die Planungsgröße von rund 500.000 NPC-Identitäten bindet keine Fahrzeugzahl und keine Zahl aktiver Fahrzeuge | 🧩 ABGELEITET | `W-TV-010`; `N-SI-016` | PF NPC | – |
| VEH-DEC-PF-004 | Ressourcen für Verkehr werden mit Detailstufen abstrahiert; Leistungsstufen sind offen | 📋 BESCHLOSSEN | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §8; `D15-OQ` | PF LS | `CQ-110` |
| VEH-DEC-PF-005 | Streaming-, Last- und Leistungs-Regressionstests sind verlangt | 📋 BESCHLOSSEN | `M-PD` (Testing) | TST PF | Modul 31 |
| VEH-DEC-PF-006 | Kostentreiber wachsen mit Zahl der S0- und S1-Fahrzeuge, Netzgröße, Bevölkerung, Zeit und Spielerzahl; teure Stufen bleiben klein, die Menge liegt in S2 und S3 | 💡 VORSCHLAG | – | PF | Keine Messung |
| VEH-DEC-PF-007 | Die Obergrenze der Verkehrsdichte ist zugleich ein Leistungsregler | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 1 | PF VKR | `CQ-61` |
| VEH-DEC-PF-008 | Bei Lastspitzen wird in dieser Reihenfolge degradiert: Aktualisierungsraten, Stufenradien, Verkehrsmenge; die Straße bleibt nie leer | 💡 VORSCHLAG | Ableitung aus `N-LE-026`, [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §8 | PF LS | Schwellen offen |
| VEH-DEC-PF-009 | Materialisierung geschieht gestaffelt; Fahrzeugtypen und Verhaltensprofile sind geteilte Daten | 💡 VORSCHLAG | Ableitung aus [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §4 | PF LS | – |
| VEH-DEC-PF-010 | Ein Messplan mit Szenarien (Innenstadt, Autobahn, Zeitsprung, Koop, Streaming) liefert die Zahlen; sie stehen erst nach der Ausführung in Berichten | 💡 VORSCHLAG | Ableitung aus `CLAUDE.md` §7 | PF TST | `CQ-118` |
| VEH-DEC-PF-011 | Große Städte brauchen kein anderes System, nur andere Profile (Bezirksprofile) und einen strengeren Blick auf Darstellungskosten | 💡 VORSCHLAG | Ableitung aus `W-UR-015`, `VEH-DEC-SP-010` | PF WLT | `CQ-117` |
| VEH-DEC-PF-012 | Für Fahrzeuge und Verkehr liegen weder Leistungsmessungen noch Budgets noch Zielhardware vor | 🔍 UNBELEGT | – | PF | – |
| VEH-DEC-PF-013 | Stadtgrößen sind Szenarien: bei 2.200 km² etwa 20 bis 25 km² für die Metropole und 8 bis 10 km² für andere große Städte; das städtische Flächenwachstum soll mit der Weltgröße unterlinear steigen | 🧭 RICHTUNG | `M-A1015` (PROVISIONAL) | PF WLT | Keine Entscheidung; ältere Kandidaten weichen ab (Welt 03) |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Zielhardware und Qualitätsstufen für den Verkehr | `CQ-115` |
| Budgets je Stufe (Bildzeit, Speicher) | `CQ-116` |
| Verkehrsdichte und Fußgänger in großen Städten | `CQ-117` |
| Messmethodik und Referenzszenarien | `CQ-118` |
