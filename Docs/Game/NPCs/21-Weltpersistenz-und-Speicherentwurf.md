# NPCs 21: Weltpersistenz und Speicherentwurf

> [!IMPORTANT]
> **Nichts davon ist implementiert, und nichts davon ist ein Format.** Dieses Modul sammelt, was die Quellen zur Speicherung des NPC- und Weltzustands festlegen, und entwirft darauf **Speicherbereiche, Lebenszyklus der Datensätze, Journal-Regeln und Größenrechnungen als technischen Entwurf (💡)**. Die Rechnungen benutzen **erfundene Annahmen** und sind keine Messungen. Das Speicherformat `WorldFacts` v1 bleibt unverändert ([Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md)). Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Verwandt: [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md), [Kreaturen 17](../Kreaturen/17-Persistenz-Daten-und-Modding.md), [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Speicherbereiche](#2-entwurf-speicherbereiche)
3. [Entwurf: Lebenszyklus eines Datensatzes](#3-entwurf-lebenszyklus-eines-datensatzes)
4. [Entwurf: Journal, Ableitung, Verdichtung](#4-entwurf-journal-ableitung-verdichtung)
5. [Entwurf: Größenrechnung](#5-entwurf-größenrechnung)
6. [Entwurf: Schreiben, Laden, Migrieren](#6-entwurf-schreiben-laden-migrieren)
7. [Entwurf: Region, Streaming und Ereigniseingang](#7-entwurf-region-streaming-und-ereigniseingang)
8. [Entwurf: Integrität und Wiederherstellung](#8-entwurf-integrität-und-wiederherstellung)
9. [Entwurf: Zeit und Zeitstände](#9-entwurf-zeit-und-zeitstände)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-PE-001 | Der zentrale persistente Weltzustand ist das ‚Gedächtnis der Welt‘; er verfolgt Fallausgänge, NPC lebt/tot, Beziehungen, veränderte oder zerstörte Orte, entdecktes Wissen, Monsteraktivität, Ruf, Fahrzeuge, Safehouses, Weltereignisse, Zeit und andere dauerhafte Zustände | 📋 | `M-PD` ‚World state and persistence‘ |
| N-PE-002 | Wichtige persistente Entitäten haben stabile IDs; World Partition lädt Laufzeitinhalte, der Weltzustand bestimmt die richtige persistente Variante | 📋 | `M-PD` ‚World state and persistence‘; `23#48A` Abschnitte 12–13 |
| N-PE-003 | Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert statt ein monolithischer Block | 📋 | `M-PD` ‚World state and persistence‘ |
| N-PE-004 | Das Langzeit-Speicherziel liegt bei etwa 20–50 MB, wenn vertretbar erreichbar; Korrektheit, Zuverlässigkeit, Ladezeit und Wiederherstellung gehen vor Größe; Speicheroptimierung ist in der Politur verpflichtend | 📋 (Zielwert) | `M-PD` ‚World state and persistence‘ |
| N-PE-005 | Gespeichert werden nur Änderungen gegenüber der unveränderten Welt: „Wir speichern nicht die Welt. Wir speichern nur, was sich gegenüber der unveränderten Welt geändert hat.“ Mehr Größe ist nicht tragisch, solange alles geladen und perfekt funktioniert; in der Politur wird optimiert | 📋 Richtung | `23#51A` Abschnitt 17; „17 es ist nicht so tragisch wenn es mehr ist solange alles geladen ist und perfekt funktioniert“ in `23#52E` |
| N-PE-006 | Der Entwickler wünscht Backups etwa im Rahmen üblicher Spiele: „die meisten spiele schaffen es ein backup in 10mb zu speichern, schaffen wir das auch“ | 🧭 | `23#49E` Punkt 17 |
| N-PE-007 | Das Speichersystem arbeitet deltabasiert: Es speichert hauptsächlich Änderungen gegenüber deterministisch erzeugten Standardwerten; ein NPC ohne Änderung braucht praktisch nur `NPC_ID + Seed`; erst echte Änderungen kosten Speicher | 📋 Richtung | `35#278A` Fragen 31–33; „12-35 ja“ in `35#279E` |
| N-PE-008 | Zufall im Hintergrund entsteht deterministisch aus NPC-ID, Weltseed und Zeitfenster, damit Speichern und Laden reproduzierbar ist | 📋 Richtung | `35#278A` Frage 15; „12-35 ja“ in `35#279E` |
| N-PE-009 | Die Speicherung teilt Gedächtnis in permanent, long-term, short-term, disposable und fasst Kurzzeitgedächtnis zusammen; wichtige Einzelereignisse bleiben einzeln | 📋 Richtung | `35#278A` Fragen 34–37; „12-35 ja“, „37-39 ja“ in `35#279E` |
| N-PE-010 | Spielstandkompression wird genutzt, solange sie Ladezeiten nicht unerträglich macht | 📋 | `35#278A` Frage 38; „37-39 ja“ in `35#279E` |
| N-PE-011 | Alte Spielstände werden beim Laden **deterministisch migriert**, wenn NPC- und Präferenzsysteme später geändert werden | 📋 | `35#278A` Frage 39; „37-39 ja“ in `35#279E` |
| N-PE-012 | Für etwa 500.000 persistente NPC-Identitäten gilt als Architekturziel ein Spielstand, der „eher kleiner“ ist (Entwickler); der Assistent setzt ein Ziel von ungefähr 50–150 MB für weit fortgeschrittene normale Spielstände und 150–300 MB für sehr lange, extrem veränderte Welten; mehrere GB wären ein Architekturproblem | 🧭 / ⚠️ | „ich habe das so gefunden: also ja eher kleiner“ in `35#279E` Punkt 40; Ausformulierung `35#280A` (siehe [Modul 27](27-Konflikte-und-offene-Fragen.md) C-N2) |
| N-PE-013 | Die Welt läuft nur, während man spielt; keine heimliche Echtzeitsimulation zwischen Sitzungen; Zeitsprünge (Schlafen, Reisen) werden von der Hintergrundsimulation ereignisbasiert nachgeholt | 📋 Richtung | „die welt läuft nur wenn man gerade spielt, im online wird das anders sein“ in `35#279E` Punkt 11; `35#278A` Frage 12; „12-35 ja“ |
| N-PE-014 | Bei einer relevanten Begegnung wird der generierte Zustand eingefroren und persistent; spätere Generatoränderungen verändern ihn nicht | 📋 Richtung | `35#278A` Frage 24; „12-35 ja“ in `35#279E` (siehe N-ID-013) |
| N-PE-015 | Der Tod eines NPCs bleibt dauerhaft im Weltzustand; relevante Historie bleibt, technischer Detailzustand darf verdichtet werden | 📋 | `35#272A` Fragen 31–33; „1-50 passt alles“ in `35#273E` (siehe N-ID-018) |
| N-PE-016 | Der Weltzustand und die stabilen IDs müssen so gebaut sein, dass spätere DLCs einen alten Spielstand sauber erweitern können; der Entwickler meinte das ausdrücklich so | 📋 | `23#51A` (Ende); „ja das mit den dlcs meinte ich eh genau so“ in `23#52E` |
| N-PE-017 | Strukturierte Backend-/Online-/Serverdaten bevorzugt in PostgreSQL; **nicht** als Pflicht-Datenbank für lokale Einzelspieler-Saves | 📋 | `M-PD` ‚World state and persistence‘; „15. ok macht sinn“ in `23#49E` |
| N-PE-018 | Mods können pro Spielstand aktiviert und deaktiviert werden; deaktivierte Mods und ihr Zustand bleiben erhalten; kritisch fehlende Mods blockieren das Laden standardmäßig, mit Expertenübersteuerung nach Backup | 📋 | [Decision 0005](../../Decisions/0005-character-appearance-foundation.md) §1 (Master basis `MODDING`) |
| N-PE-019 | Im Koop ist der Host-Spielstand maßgeblich; der Fortschritt von Gästen gehört zum Host-Spielstand | 📋 | `CLAUDE.md` §6; `48#142A`, `48#144A` (Persistenz im Host-Spielstand) |
| N-PE-020 | Die Journal-Größe von `WorldFacts` wächst unbegrenzt; Verdichtung (Snapshot plus Journal-Ende) ist offen; hochfrequenter und abgeleiteter Simulationszustand läuft nicht durch `WorldFacts`; neue Datensatzarten brauchen ein versioniertes, migrierbares Format | 📋 (technische Randbedingung) | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md); [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6 |
| N-PE-021 | Ein Spielstand aus Voll-Snapshot plus inkrementellem Journal (Delta) ist schneller, platzsparender, absturzsicher und für die Wiederherstellung besser; gelegentlich wird konsolidiert | 💡 | `23#51A` Abschnitt 17 (Beispiel: Snapshot 18 MB, Delta 312 KB) |
| N-PE-022 | Wiederherstellung nach Laden gleicht dem Zustand vor dem Speichern | 💡 (Invariante) | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 3 |
| N-PE-023 | Die Aufteilung in Segmente wie Manifest, Spieler, Welt global, Fälle, NPC-Regionen, Orte, Fahrzeuge, Safehouses ist ein konzeptionelles Beispiel des Assistenten | 💡 | `23#48A` Abschnitt 17 |

---

## 2. Entwurf: Speicherbereiche

Die Ermittlungs- und Kreaturen-Dokumentation stellen fest, dass `WorldFacts` flach und diskret ist (bool, int64, Zeichenfolge bis 4096 Bytes) und für Sammlungen, Graphen und umfangreiche Gedächtnisse **nicht geeignet** ist ([Kreaturen 17](../Kreaturen/17-Persistenz-Daten-und-Modding.md) §3; [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §4). Für die NPC-Welt wird daher ein Entwurf getrennter **Speicherbereiche (Segmente)** vorgeschlagen (💡). Die Aufteilung folgt dem Ansatz des Assistenten (N-PE-023) und der Regel ‚ein Besitzer je Tatsache‘ (Modul 24).

| Segment | Inhalt | Änderungsrate (Orientierung) | Verweis |
|---|---|---|---|
| `manifest` | Version, Generatorversion, Weltseed, Inhaltspakete (Mods/DLC), Segmentliste, Prüfsummen | selten | Modul 23 |
| `world_clock` | Weltzeit, Kalender, Zeitstand-Kennung | oft (klein) | Abschnitt 9 |
| `npc_overrides` | Abweichungen vom abgeleiteten Standard je NPC (Aussehen-Überschreibungen, Name, Wohnort, Job, Status) | mittel | Modul 03 |
| `npc_records` | eingefrorene Datensätze (Known/Detailed/Full), Persönlichkeitszustand, Lebenszustand, Besitz | mittel | Modul 03 |
| `npc_memory` | Erinnerungseinträge (Klassen), Anker, Zusammenfassungen | hoch bei wichtigen NPCs | Modul 05 |
| `npc_relations` | Beziehungskanten (relevant), Stufen, Anker, Summaries | mittel | Modul 08 |
| `households` | Haushalte (nur veränderte), Mitglieder, Besitz | selten bis mittel | Modul 10 |
| `schedules` | Abweichungen von Zeitplänen, Commitments, Abwesenheiten | hoch bei A/B | Modul 13 |
| `rumors` | Gerüchteobjekte, Halter (auf Kreisebene), Status | mittel | Modul 07 |
| `factions` | Fraktionsdatensätze, Mitglieder, Beziehungen, Strategie, Territorien | mittel | Modul 16, 17 |
| `nemesis` | Nemesis-Datensätze, Modelle, Anpassungen | mittel | Modul 11, 12 |
| `population_state` | aggregierter Zustand der Stufe D und der Ferne je Region (Zeitfenster, Seeds, Menge) | niedrig | Modul 22 |
| `sim_cursors` | pro Region/Entität: bis wann fortgeschrieben (Weltzeit) | oft (klein) | Modul 22 |
| `event_journal_npc` | Ereignisse der Lebenswelt (Ursache, Zeit, Ort, Beteiligte) | hoch | Abschnitt 4 |
| `locations_history` | Ortsgeschichte, Betreiberwechsel, Schäden | mittel | Modul 05 §9 |
| `worldfacts` | bestehendes `WorldFacts`-Dokument (Fall-, Beweis-, Objektzustand) | je nach Fakt | Decision 0004 |
| `unknown_kinds` | Daten unbekannter oder deaktivierter Erweiterungen, unverändert aufbewahrt | – | N-PE-018 |

**Regeln (Entwurf):**

1. **Ein Segment gehört einem Besitzer** (Modul 24). Segmente verweisen über stabile IDs.
2. **Segmente sind einzeln schreibbar und prüfbar** (N-PE-003); ein Fehler in einem Segment verwirft nicht den ganzen Spielstand.
3. **Hochfrequente Ableitungen sind nicht gespeichert** (Positionen, Animationszustand, kurzzeitige Wahrnehmung: Decision 0004 W3). Sie werden bei Bedarf aus dem Zustand rekonstruiert.
4. **`worldfacts` bleibt für diskrete Fakten.** Viele Weltobjekte (Türen, Gegenstände, Fälle) nutzen es weiter; NPC-Gedächtnis und -Beziehungen nutzen es nicht (N-PE-020).
5. **Verweise statt Kopien.** Der NPC-Datensatz enthält die Haushalts-ID, nicht eine Kopie des Haushalts.

---

## 3. Entwurf: Lebenszyklus eines Datensatzes

```text
 (nichts)  ─────────────►  Standard (abgeleitet aus NPC_ID + Seed)       Speicher: 0 B
    │ erste Abweichung (Name, Job, Umzug, Verletzung ...)
    ▼
 Override (npc_overrides)                                                 Speicher: klein
    │ erste relevante Begegnung  -> Einfrieren (N-ID-013)
    ▼
 Known / Detailed / Full  (npc_records, npc_memory, npc_relations)       Speicher: wächst mit Tiefe
    │ lange Irrelevanz, Verdichtung
    ▼
 Verdichtet (Summary + Anker)                                             Speicher: sinkt
    │ Tod
    ▼
 Archiv (Identität + wichtige Historie; Detailzustand verdichtet)        Speicher: klein, dauerhaft
```

**Regeln (Entwurf):**

1. **Rückkehr zum Standard ist ausgeschlossen**, sobald ein Datensatz eingefroren wurde (N-PE-014). Nur Verdichtung innerhalb des Datensatzes.
2. **Standardzustand ist deterministisch** und plattformunabhängig (Abschnitt 6).
3. **Tote bleiben Archivdatensätze**; Verknüpfungen aus Fällen, Erinnerungen und Orten bleiben gültig (N-PE-015).
4. **Der Wechsel zwischen Stufen ist verlustfrei für wichtige Ergebnisse** (Proposal 0010 §5: ‚Promotion/Demotion must be lossless for important outcomes‘).
5. **Migration.** Jede Stufe hat eine Schemaversion (N-PE-011).

---

## 4. Entwurf: Journal, Ableitung, Verdichtung

Das Proposal 0010 §4 Punkt 2 schlägt vor: **Ereignisjournal als Quelle der Wahrheit**, aus dem Gedächtnis, Beziehung, Ruf und Feindschaft ableitbar sind. Das passt zur Journal-Logik von `WorldFacts` (Wiederabspielen, `StateAt`), kollidiert aber mit der Grenze, dass das Journal unbegrenzt wächst und die Verdichtung offen ist (N-PE-020). Der Entwurf trennt (💡):

| Ebene | Inhalt | Speicherung |
|---|---|---|
| **Ereignisjournal** (`event_journal_npc`) | Ereignisse der Lebenswelt: Ursache, Zeit, Ort, Beteiligte, Art | Anhängend; verdichtbar nach Regeln |
| **Abgeleiteter Zustand** (`npc_*`, `factions`, `rumors`) | Ergebnis der Ereignisse: Erinnerungen, Beziehungen, Zustände | gespeichert als Cache; neu ableitbar, soweit das Journal reicht |
| **Anker** | prägende Einzelereignisse | dauerhaft, nicht verdichtet |

### 4.1 Welche Historie bleibt

Das Proposal nennt als offene Entscheidung, welche Historie das Journal behält (‚which history to keep‘; Q-N03). Quellenaussagen, die die Wahl begrenzen:

1. **Wichtige Einzelereignisse bleiben** (N-GE-022, N-GE-023).
2. **Kleinkram darf verdichtet werden** (N-GE-022).
3. **Historische Beziehungen bleiben abfragbar** (N-FA-013, N-GE-027).
4. **Tod und seine Folgen bleiben** (N-ID-018).
5. **Rekonstruktion ‚aus Änderungsereignissen‘** und **Verdichtung** stehen im Konflikt (Proposal 0010 §6), die Lösung ist nicht entschieden.

### 4.2 Verdichtungsregeln (Entwurf)

| Klasse | Behandlung |
|---|---|
| **Anker** | nie verdichten |
| **Ereignisse mit Wichtigkeit ab `notable`** | einzeln behalten |
| **Ereignisse `trivial` oder `minor` ähnlicher Art** | zu Summary zusammenfassen (Tendenz, Häufigkeit, Zeitraum) |
| **Ereignisse ohne Beteiligte, die noch leben oder erinnern** | verwerfbar, wenn kein Fall darauf verweist |
| **Ereignisse, auf die ein Fall, ein Archiv oder ein Ort verweist** | behalten, solange der Verweis lebt |
| **Wiederholte Ereignisse (Tagesroutine)** | nie als Journal, sondern als Regel/Seed |

### 4.3 Wiederherstellung aus dem Journal

1. Der abgeleitete Zustand ist **ein Cache**: Bei Beschädigung eines Segments wird er aus dem Journal neu berechnet, soweit das Journal reicht.
2. Der Rest (verdichtete Summaries, Anker) ist **Quelle**: Er kann nicht neu berechnet werden und braucht deshalb Prüfsummen und Backup (Abschnitt 8).
3. Replay-Tests prüfen: Journal ablaufen lassen → gleicher abgeleiteter Zustand (Proposal 0010 §4 Punkt 7).

### 4.4 Verhältnis zu WorldFacts

| Frage | Entwurf |
|---|---|
| **Ersetzt das NPC-Journal `WorldFacts`?** | Nein. `WorldFacts` bleibt für diskrete Weltfakten (Objekte, Fälle). |
| **Spiegeln sich beide?** | Wo ein Ereignis beides betrifft (Tod eines Zeugen, Fallstatus), schreibt ein **atomarer Übergang** beide Seiten (Ermittlung 19 §7). |
| **Gleiche Zeit?** | Beide benutzen dieselbe Weltuhr. |
| **Eine Ereignis-ID** | Ereignisse haben IDs, auf die beide Seiten verweisen können. |

---

## 5. Entwurf: Größenrechnung

> ⚠️ **Rechenbeispiel mit erfundenen Annahmen.** Die Zahlen sind keine Messungen und keine Quellenaussagen. Sie zeigen, **welche Größen die Spielstandgröße bestimmen**, und stellen sie den Zielwerten der Quellen gegenüber (N-PE-004, N-PE-012). Rechnung: 1 MB = 10⁶ Byte; Kompressionsfaktor 3 bis 5 ist ebenfalls eine Annahme. Als Referenz für ein Journal nennt Decision 0004 etwa 60 Bytes je protokolliertem Befehl; hier gelten 80 Byte je NPC-Ereignis.

### 5.1 Annahmen je Datensatzart

| Art | Annahme je Stück |
|---|---:|
| Override-Eintrag | 100 B |
| Known-Datensatz | 300 B |
| Detailed-Profil | 3.000 B |
| Full-Profil | 25.000 B |
| Beziehungskante (Mittelwert) | 80 B |
| Ereignis im Journal | 80 B |
| veränderter Haushalt | 300 B |

### 5.2 Drei Szenarien (Orientierung)

| Posten | wenig verändert | typisch lang | extrem verändert |
|---|---:|---:|---:|
| Overrides | 10.000 → 1,0 MB | 40.000 → 4,0 MB | 200.000 → 20,0 MB |
| Known | 3.000 → 0,9 MB | 20.000 → 6,0 MB | 100.000 → 30,0 MB |
| Detailed | 500 → 1,5 MB | 3.000 → 9,0 MB | 15.000 → 45,0 MB |
| Full | 100 → 2,5 MB | 300 → 7,5 MB | 1.500 → 37,5 MB |
| Beziehungskanten | 10.000 → 0,8 MB | 60.000 → 4,8 MB | 300.000 → 24,0 MB |
| Journal-Ereignisse | 40.000 → 3,2 MB | 300.000 → 24,0 MB | 1.500.000 → 120,0 MB |
| Haushalte (verändert) | 2.000 → 0,6 MB | 15.000 → 4,5 MB | 75.000 → 22,5 MB |
| Sonstiges (Gerüchte, Fraktionen, Nemesis, Orte) | 0,5 MB | 2,0 MB | 10,0 MB |
| **Summe roh** | **11,0 MB** | **61,8 MB** | **309,0 MB** |
| **komprimiert (3×–5×)** | **2,2–3,7 MB** | **12,4–20,6 MB** | **61,8–103,0 MB** |

*Rechenprobe: 1,0 + 0,9 + 1,5 + 2,5 + 0,8 + 3,2 + 0,6 + 0,5 = 11,0; 4,0 + 6,0 + 9,0 + 7,5 + 4,8 + 24,0 + 4,5 + 2,0 = 61,8; 20,0 + 30,0 + 45,0 + 37,5 + 24,0 + 120,0 + 22,5 + 10,0 = 309,0.*

### 5.3 Folgerungen (Entwurf)

1. **Die Zahl der Identitäten (500.000) ist nicht die Größe.** Sie wird mit `NPC_ID + Seed` bezahlt (N-PE-007). Entscheidend ist die Zahl der **Abweichungen**, besonders der **Ereignisse** (im Beispiel 29 % bis 39 % der Rohsumme; das Journal ist der größte Posten).
2. **Das Journal dominiert.** Verdichtung (Abschnitt 4.2) wirkt dort am stärksten. Eine Obergrenze für Ereignisse je NPC und Zeitraum, ohne wichtige Ereignisse zu verlieren, ist die wichtigste Stellschraube (❓ Q-N04).
3. **Full-Profile sind teuer pro Stück, aber selten.** Ihre Zahl wächst nur durch Spielerkontakt.
4. **Zielvergleich.** Im typischen Szenario liegt der komprimierte Spielstand (12,4–20,6 MB) an oder unter der Untergrenze des Langzeitziels von 20–50 MB (N-PE-004). Im extremen Szenario (61,8–103,0 MB) wird dieses Ziel überschritten, die Annahme des Assistenten von 50–150 MB (N-PE-012) aber eingehalten. Die Quellen sind sich nicht einig, welches Ziel für den NPC-Zustand gilt (C-N2).
5. **Alles hängt an Annahmen.** Die Annahmen müssen am Prototyp gemessen werden (Proposal 0010 §5: ‚measure with a prototype before fixing them‘).

---

## 6. Entwurf: Schreiben, Laden, Migrieren

### 6.1 Schreiben

| Aspekt | Entwurf |
|---|---|
| **Segmentweise** | nur geänderte Segmente werden geschrieben (N-PE-003) |
| **Schmutzmarkierung** | je Region und Segment; Autosave schreibt Delta (N-PE-021) |
| **Atomarität** | Segmentwechsel mit Zwischendatei und Umbenennung; Manifest zuletzt |
| **Konsolidierung** | gelegentlich Delta in Snapshot überführen |
| **Hintergrund** | Schreiben im Hintergrund; Spielfluss nicht blockiert |
| **Backup** | Vorversion bleibt, bis die neue Version geprüft ist |

### 6.2 Laden

| Aspekt | Entwurf |
|---|---|
| **Manifest zuerst**, dann benötigte Segmente | Spielstart lädt Spieler, Weltuhr, nahe Regionen |
| **Regionen nachladen** | beim Streaming (Abschnitt 7) |
| **Verzögertes Laden** | Fernsegmente erst bei Bedarf |
| **Ableitung** | Default-NPCs entstehen aus Seed |
| **Prüfsummen** | pro Segment |
| **Fehler** | Segment unlesbar → Wiederherstellung (Abschnitt 8) |

### 6.3 Migration (N-PE-011)

1. **Versionsfeld pro Segment und Datensatzart.**
2. **Deterministische Migration**: gleiche alte Daten → gleiche neue Daten.
3. **Eingefrorene NPCs bleiben unverändert** (N-PE-014); die Migration ändert Struktur, nicht Inhalt.
4. **Golden-File-Tests**: bekannte alte Spielstände werden in jeder neuen Version geladen und verglichen (Proposal 0010 §9).
5. **Unbekannte Daten bleiben unverändert** (N-PE-018).

### 6.4 Plattformunabhängige Determinismus (Entwurf)

Der Standardzustand und der Hintergrundzufall (N-PE-008) müssen auf Linux und Windows dasselbe Ergebnis liefern (`CLAUDE.md` §5: quelltextportabel). Folgerungen:

1. **Ganzzahlbasierte, spezifizierte Zufallsfunktionen** (Seed, ID, Zeitfenster), keine plattformabhängigen Bibliotheksgeneratoren.
2. **Keine Gleitkomma-Entscheidungen** in Zuständen, die Verzweigungen im Weltzustand bestimmen, oder eine strikt definierte Arithmetik.
3. **Feste Reihenfolge** von Iterationen (keine unordentlichen Container).
4. **Referenzvektoren**: gleiche Eingaben → gleiche Ausgaben als Test (Modul 29).

---

## 7. Entwurf: Region, Streaming und Ereigniseingang

World Partition lädt Gebiete dynamisch (`23#48A` Abschnitt 12). Ein NPC kann in einem nicht geladenen Gebiet existieren. Das System braucht:

| Baustein | Beschreibung (Entwurf) |
|---|---|
| **ID → Region-Index** | schneller Zugriff auf alle NPCs einer Region |
| **Ladelisten** | beim Laden einer Region werden die NPCs aus Index, Seed und Overrides materialisiert (Modul 22 §4) |
| **Cursor** | pro Region/Entität `sim_cursor` = bis wann fortgeschrieben |
| **Nachholen** | beim Laden wird von `sim_cursor` bis jetzt ereignisbasiert fortgeschrieben (Modul 22) |
| **Ereigniseingang** | Ereignisse für nicht geladene Entitäten (z. B. ‚NPC X erfährt von Y‘) werden in einem **Eingang** gesammelt und beim nächsten Laden angewendet |
| **Orte** | Ortszustände wirken beim Laden (zerstörte Kirche, geschlossenes Geschäft: `23#48A`) |
| **Verlassen der Region** | Zustand wird zurückgeschrieben; Cursor aktualisiert |
| **Gleichzeitige Regionen** | mehrere Regionen werden parallel geladen; Querverweise über IDs |

**Regeln (Entwurf):**

1. **Kein Ereignis geht verloren**, nur weil die Region nicht geladen ist (N-PE-001; Eingang).
2. **Die Reihenfolge der Ereignisse ist definiert** (Weltzeit, dann ID), damit Nachholen dasselbe Ergebnis wie Live-Verlauf liefert (N-EN-461).
3. **Nachholen ist begrenzt.** Eine sehr lange Pause (viele Weltjahre) wird in groben Schritten verdichtet (Modul 22 §3).

---

## 8. Entwurf: Integrität und Wiederherstellung

| Aspekt | Entwurf |
|---|---|
| **Prüfsummen** | je Segment (und Manifest); Fehler werden erkannt |
| **Referenzielle Integrität** | jeder Verweis (NPC-ID, Haushalt-ID, Fraktions-ID) zeigt auf einen existierenden Datensatz oder auf ein ausdrückliches ‚gelöscht/gestorben‘; verwaiste Verweise sind Fehler |
| **Reparatur** | fehlendes Segment: Ableitung aus dem Journal, wo möglich; sonst Standardzustand mit Warnung; nie stilles Verfälschen |
| **Backups** | Vorversionen (Zahl und Rhythmus offen) |
| **Replay** | Journal erneut abspielen und Ergebnis vergleichen (Test) |
| **Fehlermeldungen** | verständlich (Mod-Dokumentation: `23#51A` Abschnitt 18) |
| **Absturzsicherheit** | Teilschreiben darf nie einen halben Zustand hinterlassen (N-PE-003) |
| **Tombstones** | gestorbene/entfernte Entitäten bleiben als Marker, damit Verweise auflösbar sind |
| **Prüfregeln beim Laden** | Invarianten (Modul 24 §4) laufen als Validierung im Entwicklungs- und optional im Spielbetrieb |

---

## 9. Entwurf: Zeit und Zeitstände

Die Quellen verlangen: eine Weltuhr (Proposal 0010 §7 Punkt 5), Zeitreise und Reiche über ein allgemeines Temporal-World-State-System (N-GE-028), Weltzeit läuft nur im Spiel (N-PE-013).

| Aspekt | Entwurf |
|---|---|
| **Weltuhr** | im Segment `world_clock`; alle Zeitfenster, Cursor, Alterungen beziehen sich darauf |
| **Zeitstände** | Zeitstand-Kennung im Manifest und in Datensätzen; NPC-Datensätze in anderen Zeitständen sind getrennt (Welt 26) |
| **Zeitsprünge** | Schlafen, Reisen, Haft: die Hintergrundsimulation wird bis zum Zielzeitpunkt nachgeholt |
| **Pausen** | Pausieren verändert keine Weltzeit |
| **Online** | eigene Zeit- und Serverlogik (N-PE-013) |
| **Weltzeit und Realzeit** | getrennt; die Weltgeschwindigkeit ist offen (Welt 26) |
| **Temporale Änderungen** | Änderungen der Vergangenheit laufen über das Temporal-World-State-System, nicht über NPC-Sonderlogik (N-GE-028) |

---

## 10. Sonderfälle

1. **DLC oder Karten-Erweiterung zu altem Spielstand** (N-PE-016): neue Regionen, neue NPC-IDs mit neuem Seed-Raum; alte IDs bleiben.
2. **Generatorupdate:** eingefrorene NPCs unverändert; nicht eingefrorene können sich ändern, wenn sie nie ein Ereignis hatten (Standardwerte neu abgeleitet) — mit Warnung, weil es Wiedererkennung berührt; Entscheidung offen (Q-N27).
3. **Mod entfernt:** Zustand bleibt erhalten (N-PE-018).
4. **Spielstand mit Mods A, B auf Rechner ohne B:** Laden blockiert nach Regel oder Expertenübersteuerung.
5. **Beschädigter Spielstand:** Wiederherstellung (Abschnitt 8).
6. **Sehr alter Spielstand:** Migration mehrerer Schemastufen nacheinander.
7. **Koop-Beitritt:** Gast sieht den Host-Zustand; nach dem Beitritt Replikationsfragen (Modul 23).
8. **Gleichzeitige Änderung:** `WorldFacts` hat noch kein Compare-and-Set (Proposal 0010 §6); Autorität auf dem Host.
9. **Speicherplatz knapp:** Verdichtung hat Vorrang vor Verwerfen; Warnung.
10. **Cloud-Saves, Plattformgrenzen:** Größe und Anzahl der Segmente (Dateien) berücksichtigen; offen.
11. **Replay-Aufzeichnungen** nicht speichern: ‚Replaydaten speichern‘ nennt `23#51A` als Größenrisiko, das das Projekt nicht will.
12. **Zehntausende lose Gegenstände:** `23#51A` nennt auch sie als Risiko; Bereinigung (Spielwelt §13: ‚mit Bereinigung und Persistenz‘).

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-441 | Gleicher Weltseed und gleiche NPC-ID liefern auf allen Plattformen denselben abgeleiteten Standardzustand | N-PE-007, N-PE-008 |
| N-EN-442 | Ein unverändert gebliebener NPC erzeugt keinen Speicher außer ID/Seed-Bezug | N-PE-007 |
| N-EN-443 | Speichern und Laden ergibt gleichen Zustand (Round-Trip) für alle Segmente | N-PE-022 |
| N-EN-444 | Ein alter Spielstand wird deterministisch migriert und danach in der neuen Version unverändert geladen | N-PE-011 |
| N-EN-445 | Ein unbekanntes Segment oder eine unbekannte Datenart wird unverändert gespeichert | N-PE-018 |
| N-EN-446 | Ein abgebrochener Schreibvorgang lässt den letzten gültigen Spielstand intakt | N-PE-003 |
| N-EN-447 | Jeder Verweis in einem Segment zeigt auf einen existierenden Datensatz oder einen Tombstone | Abschnitt 8 |
| N-EN-448 | Verdichtung entfernt keinen Anker und kein Ereignis, auf das ein Fall verweist | Abschnitt 4.2 |
| N-EN-449 | Der abgeleitete Zustand aus dem Journal gleicht dem gespeicherten Cache | Abschnitt 4.3 |
| N-EN-450 | Ein eingefrorener NPC wird durch eine Generatorversion nicht verändert | N-PE-014 |
| N-EN-451 | Zeitsprünge verbrauchen Weltzeit, keine Realzeit, und holen Ereignisse nach | N-PE-013 |
| N-EN-452 | Der Tod eines NPCs bleibt nach Speichern und Laden erhalten | N-PE-015 |
| N-EN-453 | Ein Spielstand mit fehlendem kritischem Mod lädt nicht ohne Expertenübersteuerung nach Backup | N-PE-018 |
| N-EN-454 | Hochfrequenzzustände (Positionen, Animation) werden nicht gespeichert | N-PE-020 |
| N-EN-455 | Gespeicherte Datenmenge steigt nicht mit der Zahl unveränderter Identitäten | N-PE-007 |
| N-EN-456 | Eine Obergrenze je NPC verhindert unbegrenztes Wachstum, ohne Anker zu verlieren | Abschnitt 5.3 |
| N-EN-457 | Jedes Segment trägt eine Schemaversion | Abschnitt 6.3 |
| N-EN-458 | Der Weltzustand lädt eine Region nicht vor dem Manifest | Abschnitt 6.2 |
| N-EN-461 | Das Nachholen von Ereignissen für nicht geladene Entitäten folgt einer definierten Reihenfolge (Weltzeit, dann ID) und liefert dasselbe Ergebnis wie der Live-Verlauf | Abschnitt 7 |

---

## 12. Was fehlt

- Kein Format, kein Schema, keine Schlüssel. Der Kern `WorldFacts` bleibt unverändert; neue Datensatzarten sind ein offener Beschluss (Proposal 0010 §11).
- Offen: welche Historie das Journal behält, Verdichtung, Obergrenzen (Q-N03, Q-N04).
- Offen: das Größenziel für NPC-Zustand (C-N2).
- Offen: ob und wie Eingefrorenes bei Generatorwechsel behandelt wird (Q-N27).
- Offen: Cloud-Speicherung, Backups, Segmentzahl.
- Keine Messungen: alle Größen sind Annahmen.
