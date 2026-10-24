# Kampfsystem 25: Performance, Skalierung und Simulationsstufen

> [!IMPORTANT]
> **Nichts davon ist implementiert, und es gibt keine Budgets.** Die Quellen verlangen, dass Optimierung eine Architekturanforderung von Anfang an ist und dass CPU, GPU, VRAM, RAM, Streaming, KI, Physik, Animation, Audio und Simulation gemeinsam budgetiert werden. Zahlen (Bilder pro Sekunde, Gegnerzahl, Projektilzahl, Rechenzeit) nennt keine Quelle, und dieses Modul erfindet keine. Es ordnet die **Kostentreiber des Kampfes** und die **Simulationsstufen** und stellt Messverfahren auf. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Simulationsstufen allgemein: [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Kostentreiber des Kampfes](#3-kostentreiber-des-kampfes)
4. [Kampf und Simulationsstufen](#4-kampf-und-simulationsstufen)
5. [Messverfahren statt Zahlen](#5-messverfahren-statt-zahlen)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Optimierung ist ein Kernziel,** gemeinsam budgetiert über alle Systeme; das ganze Spiel ‚muss super optimiert sein‘ (Gameplay-Systeme §14.1; `N-SI-015`).
2. **Plattformziele:** 4K primär, PS5 mit Grafik- und Leistungsmodus, kein 40-FPS-Modus; natives 4K mit 60 FPS und maximalem Raytracing auf der Basis-PS5 wird nicht versprochen (Gameplay-Systeme §14.1). Die Hardware-Mindestanforderungen sind offen.
3. **Simulationsstufen sind beschlossen als Richtung** (hohe Genauigkeit nahe am Spieler, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben), ohne Budgets (`N-SI-001`, `N-SI-008`, `N-SI-011`).
4. **Der Kampf hat die höchsten Spitzenlasten:** Projektile ohne künstliche Maximalreichweite, KI-Wahrnehmung mit Schall und Sicht, Verletzungsberechnung, Paaranimationen, Ragdoll.
5. **Messen vor Festlegen:** Zahlen sollen an einem Prototyp gemessen und nicht gewählt werden (`N-SI-020`, 💡).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-PF-001 | **Optimierung ist Architekturanforderung von Anfang an;** CPU, GPU, VRAM, RAM, Streaming, KI, Physik, Animation, Audio und Simulation werden gemeinsam budgetiert | 📋 BESCHLOSSEN | Gameplay-Systeme §14.1; `N-SI-015` (`35#785E` P139) | PRF | – |
| COMBAT-DEC-PF-002 | **Physik mit leistungsbewusster Näherung,** keine offensichtlich unrealistischen Abkürzungen | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘ | PRF TRF | `COMBAT-DEC-GP-006` |
| COMBAT-DEC-PF-003 | **Keine künstliche Maximalreichweite** für Projektile | 📋 BESCHLOSSEN | `COMBAT-DEC-FK-014` | PRF | Lebensdauer ohne Entscheidung (`CQ-19`, `CQ-106`) |
| COMBAT-DEC-PF-004 | **Skalierbare Weltsimulation:** hohe Genauigkeit nahe am Spieler, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben nicht geladener NPCs und Gebiete, Persistenz wichtiger Ergebnisse | 📋 BESCHLOSSEN | `N-SI-001` (`PE-N1022` G) | PRF PER | – |
| COMBAT-DEC-PF-005 | **Nicht jeder NPC braucht die volle Simulation;** wichtige Figuren sind vollständige Akteure, Hintergrund und Ferne sind leichter | 📋 BESCHLOSSEN | `N-SI-002`; Gameplay-Systeme §8 | KI PRF | – |
| COMBAT-DEC-PF-006 | **Persistenzstufen A bis D** legen fest, wie viel Persistenz und Simulation eine Figur bekommt | 🧭 RICHTUNG | `N-SI-003`, `N-ID-022` | KI PER | – |
| COMBAT-DEC-PF-007 | **Hintergrundsimulation arbeitet ereignisbasiert,** Zufall deterministisch aus Figur-ID, Weltseed und Zeitfenster | 🧭 RICHTUNG | `N-SI-011`, `N-SI-012` | PRF | – |
| COMBAT-DEC-PF-008 | **Crowd-LOD:** in der Ferne vereinfachte Simulation, in Spielnähe detaillierte NPCs; Menschenmengen werden aus dem Simulationszustand rekonstruiert | 📋 BESCHLOSSEN | `N-SI-007` | PRF | Kampf in Menschenmengen: `CQ-107` |
| COMBAT-DEC-PF-009 | **Zufall ist über Seeds reproduzierbar;** Risikosysteme bekommen deterministische Simulationen und Leistungsregressionstests | 📋 BESCHLOSSEN | Gameplay-Systeme §17.2 | TST PRF | – |
| COMBAT-DEC-PF-010 | **Kampfsystem-Kostentreiber** (Projektile, Trefferauflösung, Verletzungsmodell, KI-Wahrnehmung, Animationen, Ragdoll, Netzwerk, Audio) werden einzeln gemessen, bevor Budgets festgelegt werden | 💡 VORSCHLAG | Abschnitt 3, 5; `N-SI-020` | PRF TST | – |
| COMBAT-DEC-PF-011 | **Kampf-Detailstufen:** voller Kampf in Spielnähe, vereinfachte Auflösung außerhalb (zum Beispiel Kämpfe zwischen NPC-Gruppen als Ergebnisereignis, nicht als Schuss für Schuss) | 💡 VORSCHLAG | Abschnitt 4; `N-SI-005`, `N-SI-011` | PRF KI | `CQ-106` |
| COMBAT-DEC-PF-012 | **Hardware-Mindest-, Empfohlen- und Ultra-Anforderungen, Bildraten, Raytracing-Umfang und Profile für PS5 Pro und Series S** sind offen | ❓ OFFEN | Gameplay-Systeme §14.1 (letzte Zeile) | PRF | `CQ-108` |
| COMBAT-DEC-PF-013 | **Budgets für Simulation und Kampf** (Gegner gleichzeitig, Projektile, Wahrnehmungstraces, Ragdolls, Wundberechnungen) sind nicht festgelegt und hängen am Prototyp | ❓ OFFEN | `N-SI-017` (`M-OQ16`, `M-AS`); `N-SI-020` | PRF | `CQ-106` |
| COMBAT-DEC-PF-014 | **Kampf in dicht bevölkerten Gebieten** (Leistung, Zeugen, Zivilisten) ist nicht entschieden | ❓ OFFEN | Auftrag §8 (Performance in dicht bevölkerten Gebieten); `N-SI-007` | PRF NPC | `CQ-107` |
| COMBAT-DEC-PF-015 | **Offscreen-Kämpfe** (Fraktionskonflikte, Monsterterritorien) und ihr Fortschreiben sind nicht im Detail festgelegt; beschlossen sind Territoriumssimulation und Konflikte unabhängig vom Spieler | 🧭 RICHTUNG | `N-TE-001`, `N-TE-002`; `K-OE-016` | PRF KI | `CQ-109` |
| COMBAT-DEC-PF-016 | **Leistung beeinflusst nie die Spielregeln:** Detailstufen verändern Darstellung und Genauigkeit der Auflösung, nicht die Gültigkeit beschlossener Regeln (Anatomie, Beweise) | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-GP-014` (Anatomie gleich), `PE-N1022` H (Integration ohne Widerspruch) | PRF | Prüfziel |

---

## 3. Kostentreiber des Kampfes

| Treiber | Warum teuer | Quelle der Anforderung | Mögliche Entlastung (Entwurf) |
|---|---|---|---|
| Projektile und Schrot | Flugzeit, Wind, Durchschlag, keine Maximalreichweite | `COMBAT-DEC-FK-013` bis `-021` | Hybridmodell (Modul 05), Lebensdauer |
| Trefferauflösung durch Schichten | Material, Körper, Gefäße | `COMBAT-DEC-TK-002` | Hit-Volumina grob, Feinauflösung nur bei Körpertreffer |
| Verletzungsmodell | viele Wunden, Blut, Schmerz, Schock | `COMBAT-DEC-TK-006` | Zeitschritt gröber außerhalb des Kampfes |
| KI-Wahrnehmung | Sicht, Schall mit Raumakustik, Hypothesen | `COMBAT-DEC-KI-008`, `-009` | Stufen nach Nähe, Zeitscheiben |
| Mehrere Gegner | Flankieren, Kollision | `COMBAT-DEC-NK-018`, `-019` | Gruppentaktik statt Einzelplanung |
| Paaranimationen | Motion Warping, IK | `COMBAT-DEC-AN-010` | Animations-LOD |
| Ragdoll, Sekundärphysik | Physikkosten | `COMBAT-DEC-AN-009` | Aktivierung nur nahe der Kamera |
| Audio-Ausbreitung | Raum-/Portalmodell | `COMBAT-DEC-KI-008` | Vereinfachung außerhalb |
| Netzwerk | Replikation vieler Zustände | [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md) | Idempotente, seltene Ereignisse |
| Persistenz | Segmentgrößen, Laden | [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) | Verdichtung nach Relevanz |

Die Spalte ‚Mögliche Entlastung‘ ist Entwurf; keine ist beschlossen.

---

## 4. Kampf und Simulationsstufen

Die Simulationsstufen sind in der NPC-Dokumentation beschlossen als Richtung mit Beispielnamen (S0 bis S3, nur Entwurf; `N-SI-008`). Für den Kampf folgt (🧩 ABGELEITET aus `COMBAT-DEC-PF-004`, `-005`, `-016`):

| Stufe (Arbeitsbegriff) | Kampfdetail | Beispiel |
|---|---|---|
| Nahe, aktive Kämpfe | volle Trefferauflösung, Verletzungsmodell, Animation, Wahrnehmung | der Kampf des Spielers |
| Lokale Umgebung | vereinfachte Wahrnehmung, grobe Gegnerpositionen | Gegner im Nachbargebäude |
| Regionale Ereignisse | Ergebnisse statt Schüsse (Sieg, Verlust, Verletzung) | Fraktionskonflikt am Stadtrand |
| Ferne Welt | ereignisbasierte Zustandsänderung | Territorium eines Rudels |

**Regeln (🧩 ABGELEITET):** (1) Der Kampf des Spielers läuft immer in der höchsten Stufe. (2) Wechselt ein ferner Kampf in die Nähe, entsteht ein konsistenter Zustand aus den gespeicherten Ergebnissen. (3) Das Ergebnis ferner Kämpfe ist an dieselben Anatomie- und Beweisregeln gebunden (`COMBAT-DEC-PF-016`). (4) Hochfrequentes bleibt außerhalb der Weltfakten (`COMBAT-DEC-SP-014`).

---

## 5. Messverfahren statt Zahlen

Die Quellen fordern deterministische Simulationen, Leistungsregression und Replays (`COMBAT-DEC-PF-009`). Ein Messplan für spätere Meilensteine (💡):

| Messung | Zweck | Verfahren (Entwurf) |
|---|---|---|
| Projektile pro Sekunde bei Dauerfeuer und Schrot | Obergrenze finden | Testkarte, feste Seeds, Profiling |
| Gleichzeitige Gegner bei Wahrnehmungsstufen | Budget der KI | Szenariomatrix (Dichte mal Schwierigkeit) |
| Wundberechnungen pro Treffer | Kosten des Körpermodells | Mikrobenchmark im portablen Kern |
| Paaranimationen mit Warping | Kosten der Nahkampfpaare | Testkarte mit n Paaren |
| Ragdoll-Anzahl | Physikbudget | Stufenplan |
| Netzwerklast | Replikation | Simulation von 2 bis 4 Spielern |
| Ladezeit und Segmentgröße | Persistenz | Golden-Dateien, Roundtrip |

Jeder Messwert wird mit Plattform, Build, Seed und Szenario protokolliert; nur Gemessenes wird in Dokumenten genannt (`CLAUDE.md` §7: keine erfundenen Leistungszahlen).

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Kampf in Menschenmengen (`CQ-107`) | A: Wenige aktive Kämpfer, Rest als Menge mit grober Reaktion | günstig | Zeugenqualität sinkt |
| | B: Zeugen als Daten, nicht als Akteure | skalierbar | Reaktionen wirken blass |
| | C: Mehr Akteure in Schüben | lebendig | Spitzenlast |
| Ferne Kämpfe (`CQ-109`) | A: Ergebniswürfe aus Stärken und Lage | günstig | Nachvollziehbarkeit |
| | B: Vereinfachte Simulation | glaubwürdig | Aufwand |
| Budgetierung (`CQ-106`) | A: Feste Budgets je Stufe | planbar | starr |
| | B: Adaptive Budgets nach Last | flexibel | Verhalten ändert sich |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Simulation ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md)) | Stufen, Promotion und Demotion |
| zur Persistenz | verdichtete Ergebnisse |
| zur KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)) | Wahrnehmungsstufen |
| zur Animation ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) | LOD |
| zum Koop | Netzwerklast |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Überlast im Gedränge | Stufenabbau, nie Wegfall beschlossener Regeln | 🧩 |
| Detailstufenwechsel mitten im Kampf | keine sprunghafte Änderung von Verletzungen, Wissen, Waffenzustand | 🧩 |
| Projektil verlässt die geladene Region | definierte Beendigung | 💡 |
| Fernkampf ergibt ein Ergebnis, das einer Regel widerspricht | Regel hat Vorrang | 🧩 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-106` | Budgets und Detailstufen des Kampfes (Gegner, Projektile, Wahrnehmung, Ragdoll, Wunden) | `COMBAT-DEC-PF-003`, `-011`, `-013` | ja |
| `CQ-107` | Kampf in dicht bevölkerten Gebieten | `COMBAT-DEC-PF-008`, `-014` | ja |
| `CQ-108` | Plattform- und Hardwareziele für den Kampf (Bildrate, Mindestanforderung) | `COMBAT-DEC-PF-012` | ja |
| `CQ-109` | Fortschreiben ferner Kämpfe (Fraktionen, Territorien) | `COMBAT-DEC-PF-015` | ja |
