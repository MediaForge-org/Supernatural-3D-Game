# Fahrzeuge und Verkehr 22: Persistenz und Lebenszyklen

> [!IMPORTANT]
> **Nichts davon ist implementiert, und es gibt kein Speicherformat für Fahrzeuge.** Beschlossen sind die Grundsätze der Weltpersistenz (stabile IDs, nur Änderungen speichern, Segmente, Migration, Host-Speicherstand). Dass Fahrzeuge persistent sind, ist eine Richtung. Der Engineering-Vorschlag [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) §6 ordnet Fahrzeugzustand **nicht** in `WorldFacts` ein. Alle Segmente und Lebenszyklen hier sind Vorschlag und knüpfen an den NPC-Entwurf an. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Verwandt: [NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md), [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Was gespeichert wird und was abgeleitet bleibt](#2-was-gespeichert-wird-und-was-abgeleitet-bleibt)
3. [Speicherbereiche](#3-speicherbereiche)
4. [Lebenszyklus eines Fahrzeugs](#4-lebenszyklus-eines-fahrzeugs)
5. [Speichern und Laden im Verkehr](#5-speichern-und-laden-im-verkehr)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Fahrzeuge | Fahrzeuge sind persistent: stabile Identität, Zustand, am Falltatort abgestellt, Kofferraum-Inventar (🧭) | `W-FZ-016`; `N-PE-001` |
| Delta | Gespeichert wird, was sich gegenüber der unveränderten Welt geändert hat; ein NPC ohne Änderung braucht praktisch nur ID und Seed | `N-PE-005`, `N-PE-007` |
| Segmente | Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar, komprimiert | `N-PE-003` |
| Migration | Alte Spielstände werden deterministisch migriert | `N-PE-011` |
| Host | Im Koop ist der Host-Spielstand maßgeblich; der Fortschritt von Gästen gehört zum Host-Spielstand | `N-PE-019` |
| Mods | Mods sind pro Spielstand aktivierbar; deaktivierte Mods und ihr Zustand bleiben erhalten | `N-PE-018` |
| Größe | Langzeitziel etwa 20–50 MB; Korrektheit geht vor Größe; Konflikt mit dem Assistentenvorschlag | `N-PE-004`; `CK-04` |
| WorldFacts | `WorldFacts` ist für diskrete Fakten; hochfrequenter und abgeleiteter Zustand läuft nicht durch `WorldFacts` | `N-PE-020`; [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) §6 |
| Nicht doppelt | Wiederherstellung nach Laden gleicht dem Zustand vor dem Speichern (Invariante, 💡) | `N-PE-022` |

---

## 2. Was gespeichert wird und was abgeleitet bleibt

Decision 0004 §6 führt als Beispiel, was nicht in `WorldFacts` gehört: ‚High-frequency simulation, physics, positions, vehicle state, animation state: derived or transient, owned by the simulation / engine and saved by it.‘ Das ist ein **Engineering-Vorschlag, kein Kanon**. Er trennt zwei Dinge:

| Art | Beispiel | Ablage (Vorschlag) |
|---|---|---|
| **Diskreter Fakt** | Fahrzeug X wurde gestohlen, verkauft, verschrottet, beschlagnahmt | `WorldFacts` (diskret, selten) |
| **Fahrzeugdatensatz** | Typ, Besitzer, Verschleiß, Schaden, Inventar, Historie | eigenes Segment (Fahrzeugsystem) |
| **Abgeleiteter Zustand** | Verkehrsdichte, Stau, Signalstand | nicht gespeichert, aus Kurven, Ereignissen und Weltzeit abgeleitet |
| **Transienter Zustand** | Position, Geschwindigkeit, Gang eines fahrenden Fahrzeugs | nur beim Spielerfahrzeug beim Speichern (Abschnitt 5) |

**Konsequenz:** Dass Fahrzeuge im Weltzustand geführt werden (`M-PD`: ‚vehicles‘ gehören zum Weltzustand), bedeutet hier ein **Segment des Spielstands** neben `WorldFacts`, nicht Einträge in `WorldFacts`. Das ist kein Widerspruch, sondern eine Konkretisierung.

---

## 3. Speicherbereiche

> **💡 VORSCHLAG.** Zusätzliche Segmente neben denen des NPC-Entwurfs ([NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md) §2). Die Namen sind Arbeitsnamen.

| Segment | Inhalt | Änderungsrate (Orientierung) |
|---|---|---|
| `vehicle_overrides` | Abweichungen vom erzeugten Fahrzeug eines C-NPC (Kauf, Schaden, Umbau, Besitzerwechsel) | mittel |
| `vehicle_records` | volle Fahrzeugdatensätze (A- und B-NPCs, Spielerfahrzeuge, berührte Fahrzeuge) | mittel |
| `vehicle_events` | Eigentums- und Rechtsereignisse (Kauf, Verkauf, Diebstahl, Beschlagnahme, Verschrottung), Historie verdichtet | niedrig bis mittel |
| `road_state` | Netzversion, Baustellen, Sperrungen, Straßenschäden, Unfallstellen | niedrig bis mittel |
| `service_state` | laufende Einsätze der Dienste (Abschnitt: Modul 16) | niedrig |
| `worldfacts` | diskrete Fakten (bestehend) | je nach Fakt |

Regeln (Vorschlag):

1. **Ein Segment gehört einem Eigentümer**, Verweise über stabile IDs, keine Kopien (`VEH-DEC-GP-015`).
2. **Segmente sind einzeln schreibbar und prüfbar**; ein Fehler verwirft nicht den ganzen Spielstand (`N-PE-003`).
3. **Der Standardzustand ist deterministisch** aus ID und Seed; nur Abweichungen kosten Speicher (`N-PE-007`).
4. **Fahrzeugtypen** werden über ihre Typ-ID referenziert; Daten unbekannter oder deaktivierter Typen bleiben unverändert erhalten (`N-PE-018`, `unknown_kinds`).

---

## 4. Lebenszyklus eines Fahrzeugs

> **💡 VORSCHLAG.** Der Lebenszyklus folgt dem des NPC-Datensatzes ([NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md) §3).

```text
 (nichts)  ---->  Standard (aus NPC-ID + Seed, bzw. aus Händlerbestand)   Speicher: 0 B
    | erste Abweichung (Kauf, Unfall, Umbau, Besitzerwechsel)
    v
 Override (vehicle_overrides)                                             klein
    | Berührung durch Spieler / Beteiligung an Ermittlung  -> Einfrieren
    v
 Voll (vehicle_records, mit Historie)                                     wächst mit Tiefe
    | Totalschaden / Unfall
    v
 Wrack  ->  Abschleppen  ->  Verwahrung  ->  Schrott
    |
    v
 Archiv (Identität, Kennzeichen, Historie; Detailzustand verdichtet)      klein, dauerhaft
```

Regeln (Vorschlag):

1. **Rückkehr zum Standard ist ausgeschlossen**, sobald ein Fahrzeugdatensatz eingefroren wurde (entspricht `N-PE-014`).
2. **Verschrottete Fahrzeuge bleiben Archivdatensätze**, weil Kennzeichen und Historie in Ermittlungen noch auftauchen können (analog zu Toten, `N-PE-015`).
3. **Besitzerwechsel, Diebstahl, Verkauf sind atomare Ereignisse**: ein Fahrzeug hat nie zwei Eigentümer und nie zwei Standorte.
4. **Wracks und abgestellte Fahrzeuge** werden nach einer intelligenten Bereinigung begrenzt, damit die Welt nicht zugemüllt wird. Für abgelegte Waffen und Gegenstände gilt diese Linie bereits (`COMBAT-DEC-PO-003`: eine intelligente Bereinigung verhindert unbegrenzten Müll). Wie sie für Fahrzeuge aussieht, ist offen (`CQ-94`).
5. **Standort** ist ein Zustand mit festen Werten: geparkt, fährt, abgeschleppt, in Werkstatt, in Verwahrung, abwesend, im Archiv (Modul 06, 08, 12).

---

## 5. Speichern und Laden im Verkehr

> **💡 VORSCHLAG.**

- **Spielerfahrzeug in Fahrt:** Das Fahrzeug des Spielers wird mit dem Spieler gespeichert, einschließlich des transienten Zustands (Position, Geschwindigkeit, Gang), damit der Spieler dort weitermacht, wo er stand. Ob das Speichern während der Fahrt erlaubt ist, ist offen.
- **NPC-Verkehr:** wird nicht gespeichert, sondern nach dem Laden aus abgeleitetem Zustand rekonstruiert (`VEH-DEC-GP-006`). Es wird also nicht dasselbe Fahrzeug an derselben Stelle stehen. Die Invariante ‚Wiederherstellung gleicht dem Zustand vor dem Speichern‘ (`N-PE-022`) muss deshalb für Verkehr als **logische** Gleichheit (gleiche Zustände, gleiche Ereignisse, gleiche Zeitpläne) gelesen werden und nicht als bitgleiche. Das ist eine Auslegung, die entschieden werden muss (`CQ-91`).
- **Zeitsprünge:** Parkdauer, Reparaturen, Fahrten und Einsätze werden mit der Weltuhr fortgeschrieben (`N-PE-013`). Verkehrslage wird neu abgeleitet (Modul 15).
- **Streaming:** Fahrzeuge, die den Ladebereich verlassen, wechseln in einen abstrakten Zustand und kommen wieder (Modul 26).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-PE-001 | Fahrzeuge sind persistent: stabile Identität, Zustand, am Falltatort abgestellt, Kofferraum-Inventar | 🧭 RICHTUNG | `W-FZ-016`; `N-PE-001` | PER FZ | Schema offen (`CQ-90`) |
| VEH-DEC-PE-002 | Gespeichert wird nur, was von der unveränderten Welt abweicht; ein unverändertes Objekt braucht praktisch nur ID und Seed | 📋 BESCHLOSSEN | `N-PE-005`; `N-PE-007` | PER | – |
| VEH-DEC-PE-003 | Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar und wiederherstellbar; alte Spielstände werden deterministisch migriert | 📋 BESCHLOSSEN | `N-PE-003`; `N-PE-011` | PER | – |
| VEH-DEC-PE-004 | Im Koop ist der Host-Spielstand maßgeblich | 📋 BESCHLOSSEN | `N-PE-019`; `CLAUDE.md` §6 | PER KOO | – |
| VEH-DEC-PE-005 | Mods sind pro Spielstand aktivierbar, deaktivierte Mods und ihr Zustand bleiben erhalten | 📋 BESCHLOSSEN | `N-PE-018` | PER MOD | Verwaiste Fahrzeuge: `CQ-93` |
| VEH-DEC-PE-006 | Fahrzeugzustand läuft nicht durch `WorldFacts`; dort stehen nur diskrete Fakten (zum Beispiel gestohlen, verkauft, verschrottet) | 🧩 ABGELEITET | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) §6; `N-PE-020` | PER | Decision 0004 ist ein Engineering-Vorschlag, kein Kanon |
| VEH-DEC-PE-007 | Fahrzeuge erhalten eigene Segmente (`vehicle_overrides`, `vehicle_records`, `vehicle_events`, `road_state`, `service_state`) | 💡 VORSCHLAG | Ableitung aus `N-PE-003`, `N-PE-023` | PER | Format: `CQ-90` |
| VEH-DEC-PE-008 | Lebenszyklus eines Fahrzeugs: Standard, Override, Voll, Wrack, Abschleppen, Verwahrung, Schrott, Archiv | 💡 VORSCHLAG | Ableitung aus [NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md) §3 | PER FZ | – |
| VEH-DEC-PE-009 | Verkehrslage, Stau und Signalstand sind abgeleitet und werden nicht gespeichert; gespeichert werden Objekte und Ereignisse mit Zeit | 💡 VORSCHLAG | Ableitung aus `N-PE-005`, `VEH-DEC-KR-009` | PER VKR | – |
| VEH-DEC-PE-010 | Verschrottete Fahrzeuge bleiben als Archivdatensatz mit Kennzeichen und Historie bestehen | 💡 VORSCHLAG | Ableitung aus `N-PE-015` | PER ERM | – |
| VEH-DEC-PE-011 | Besitzerwechsel, Diebstahl und Verkauf sind atomare Ereignisse; ein Fahrzeug hat nie zwei Eigentümer oder Standorte | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-015` | PER FZ | – |
| VEH-DEC-PE-012 | Das Spielerfahrzeug in Fahrt wird mit transientem Zustand gespeichert; der NPC-Verkehr wird nach dem Laden rekonstruiert | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-006`, [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) §6 | PER VKR | `CQ-91` |
| VEH-DEC-PE-013 | Wracks und abgestellte Fahrzeuge werden durch eine intelligente Bereinigung begrenzt | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-PO-003` | PER WLT | Regel: `CQ-94` |
| VEH-DEC-PE-014 | Die Invariante ‚Laden gleicht Speichern‘ muss für Verkehr als logische, nicht bitgleiche Gleichheit gelesen werden | ❓ OFFEN | `N-PE-022` | PER VKR | `CQ-91` |
| VEH-DEC-PE-015 | Speichergröße und Größenanteil von Fahrzeugen und Verkehr sind nicht festgelegt; das Gesamtziel ist umstritten | ⚠️ KONFLIKT | `N-PE-004`; `W-TV-011` | PER | `CK-04`, `CQ-92` |
| VEH-DEC-PE-016 | Wie Straßenänderungen und Netzversionen zum Re-Import von Kartenregionen passen, ist nicht entschieden | ❓ OFFEN | `WF-C09`; `WF-UE` | PER WF | `CQ-92` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Schema und Format der Fahrzeugsegmente | `CQ-90` |
| Auslegung der Invariante ‚Laden gleicht Speichern‘, Speichern in Fahrt | `CQ-91` |
| Netzversion, Speichergröße, Re-Import der Karte | `CQ-92` |
| Verwaiste Fahrzeuge aus deaktivierten Mods | `CQ-93` |
| Bereinigung von Wracks und abgestellten Fahrzeugen | `CQ-94` |
