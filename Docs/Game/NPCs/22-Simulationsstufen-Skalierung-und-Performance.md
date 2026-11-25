# NPCs 22: Simulationsstufen, Skalierung und Performance

> [!IMPORTANT]
> **Nichts davon ist implementiert, und es gibt keine Messwerte.** Dieses Modul beschreibt, wie eine Bevölkerung von mehreren Hunderttausend Identitäten glaubwürdig lebt, ohne dass jede einzelne voll gerechnet wird. Beschlossen sind die Richtung (nicht jeder NPC ist ein voller Actor, hohe Genauigkeit in Spielernähe, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben, kein leerer Stadtteil, glaubwürdige Konsistenz vor Vollständigkeit) und die Anforderung G (`PE-N1022`); **Stufen, Übergänge, Fortschreibungsalgorithmus und Budgetgrößen sind Entwurf (💡)**. **Budgets sind unausgewählt** (`M-PO`, `M-AS`, `M-OQ16`). Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Verwandt: [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §4 und §8, [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §10, [Kreaturen 08](../Kreaturen/08-Oekologie-Populationen-und-Lebenszyklus.md) §11.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Die Simulationsstufen](#2-entwurf-die-simulationsstufen)
3. [Entwurf: Ereignisbasiertes Fortschreiben](#3-entwurf-ereignisbasiertes-fortschreiben)
4. [Entwurf: Materialisierung und Rückschreiben](#4-entwurf-materialisierung-und-rückschreiben)
5. [Entwurf: Übergänge zwischen Stufen](#5-entwurf-übergänge-zwischen-stufen)
6. [Entwurf: Budgets und Messgrößen](#6-entwurf-budgets-und-messgrößen)
7. [Entwurf: Determinismus und Reihenfolge](#7-entwurf-determinismus-und-reihenfolge)
8. [Entwurf: Lastspitzen und Degradation](#8-entwurf-lastspitzen-und-degradation)
9. [Entwurf: Architektur und Kandidaten](#9-entwurf-architektur-und-kandidaten)
10. [Entwurf: Koop-Autorität](#10-entwurf-koop-autorität)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-SI-001 | **Skalierbare Weltsimulation (G):** hohe Genauigkeit nahe am Spieler, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben nicht geladener NPCs und Gebiete, Persistenz wichtiger Ergebnisse; nicht jeder NPC läuft als voller Unreal-Actor; die Weltgröße von etwa 2.200 km² ist ein Szenario und darf nirgends fest verdrahtet sein | 📋 | `PE-N1022` G |
| N-SI-002 | Nicht jeder NPC braucht die volle Simulation: wichtige Figuren sind vollständige Akteure mit StateTree-Verhalten und Gedächtnis; Hintergrund und ferne Bevölkerung sind leichter beziehungsweise abstrahiert | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-SI-003 | Persistenzstufen A bis D legen fest, wie viel Persistenz und Simulation eine Figur bekommt | 📋 Richtung | `23#1056A` Fragen 17, 121; „ja“ in `23#1057E`; `23#1058A` |
| N-SI-004 | **Realismus und glaubwürdige Konsistenz sind wichtiger als eine mathematisch lückenlose Simulation jeder Sekunde:** „glaubwürdig, das wichtigste von diesem spiel ist realismus“; die Simulation ist so tief, dass der Spieler keinen offensichtlichen Fake bemerkt | 📋 | `23#1057E` Punkt 125; `23#1058A` ‚Oberste Regel aus Block 8‘ |
| N-SI-005 | Ein weit entfernter NPC wird als Zustand (`AT_WORK`, `HOME`, `TRAVEL` …) geführt, nicht als exakte Gehbewegung; beim Annähern entsteht daraus wieder ein konsistenter detaillierter NPC; die Welt ist beim Betreten eines Gebietes nicht leer | 📋 | `23#1056A` Fragen 122–123; „ja sowieso nicht, aber wenn man dann in das disktikt kommt darf die straße nicht leer sein, sondern auch voll“ und „ja sicher“ in `23#1057E` |
| N-SI-006 | Die Hierarchie ist für eine sehr große Welt „besonders konsequent“ nötig | 📋 | `23#1056A` Frage 124; „ja genau“ in `23#1057E` |
| N-SI-007 | Crowd LOD: in der Ferne vereinfachte Simulation, in Spielnähe detaillierte NPCs; Menschenmengen werden aus dem vorhandenen Simulationszustand rekonstruiert | 📋 | `23#1056A` Frage 87; „ja“ in `23#1057E`; `23#1058A` |
| N-SI-008 | Jeder NPC hat eine dauerhafte globale ID, auch wenn nicht geladen; mehrere Simulationsstufen sind vorgesehen (Beispiel des Assistenten: Tier 0 Hero/direkt beim Spieler, Tier 1 lokal aktiv, Tier 2 regionale Hintergrundsimulation, Tier 3 globale Dormant-Simulation); ein NPC wechselt ohne Verlust von Identität und Historie; Tier 3 wird nicht frameweise, sondern über vereinfachte Ereignis-/Zeitschritte simuliert | 📋 Richtung (Namen: 🧭) | `35#278A` Fragen 1–4; „1-4 ja“ in `35#279E`; W-TV-010 der Welt-Dokumentation behandelt die Stufen als 🧭/💡 |
| N-SI-009 | Dormante NPCs entwickeln Jobwechsel, Umzug, Beziehung, Trennung, Tod grob weiter; nicht „der jetzige ort, nicht der ganze weg“ | 📋 | `35#278A` Frage 5; „5 ja aber halt nicht der jetzige ort nicht der ganze weg und so weiter“ in `35#279E` |
| N-SI-010 | Die Genauigkeit steigt, sobald ein NPC für den Spieler wichtiger wird; ein zufälliger unbekannter NPC braucht deutlich weniger Zustand; Herabstufung nach Jahrzehnten Irrelevanz ist möglich, Historie bleibt | 📋 | `35#278A` Fragen 6–10; „6-10 ja“ in `35#279E` |
| N-SI-011 | Hintergrundsimulation arbeitet ereignisbasiert statt sekündlich (statt 86.400 Sekunden eines Tages wenige Ereignisse wie arbeiten, Person treffen, heimfahren, schlafen); ein wichtiger NPC wird genauer simuliert als 50.000 Unbekannte; ein NPC außerhalb hat eine plausible Routine und beim plötzlichen Besuch einen konsistenten aktuellen Zustand; NPCs sind nicht an zwei Orten gleichzeitig; Reisen kostet Zeit | 📋 Richtung | `35#278A` Fragen 12–20; „12-35 ja“ in `35#279E` |
| N-SI-012 | Zufall entsteht deterministisch aus NPC-ID, Weltseed und Zeitfenster | 📋 Richtung | `35#278A` Frage 15; „12-35 ja“ in `35#279E` |
| N-SI-013 | Relationship-/Präferenzzustand skaliert: kompaktes generiertes Profil für Unbekannte; detaillierte Werte erst bei Relevanz; Seed bleibt; Einfrieren bei relevanter Begegnung; detaillierte Kanten nur für relevante Personen; echte Beziehungen zwischen zwei NPCs werden automatisch detaillierter; kurzfristige Anziehung zwischen Hintergrund-NPCs darf verschwinden; wichtige Historie nie | 📋 Richtung | `35#278A` Fragen 21–30; „12-35 ja“ in `35#279E` |
| N-SI-014 | Die Welt läuft nur, während man spielt; Zeitsprünge werden nachgeholt | 📋 | „11 die welt läuft nur wenn man gerade spielt“ in `35#279E`; `35#278A` Frage 12 |
| N-SI-015 | Optimierung ist ein globales Kernziel: „ja genau, das ganze spiel muss super optimiert sein“; hohe Qualität, aber nicht für jeden High-End-Hardware | 📋 | `35#785E` Punkt 139; `35#786A` |
| N-SI-016 | Eine Bevölkerung von mehreren Hunderttausend NPCs (etwa 500.000) ist Planungsgröße, nicht beschlossen; Simulationsstufen und Budgets sind unabhängig von der Lore-Bevölkerung zu planen: ‚The entire population need not be individually active at once.‘ | 🧭 | [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13; `M-PO` Punkt 5; `35#227E` |
| N-SI-017 | **Simulations-LOD, Save-Schema, deterministische Offscreen-Simulation, Tests und Leistungsbudgets sind offen** | ❓ | `M-OQ16` Punkt 9; `M-AS` Kandidat 6 |
| N-SI-018 | Promotion und Demotion zwischen den Stufen sind für wichtige Ergebnisse verlustfrei; Stufenschwellen sind Konfiguration | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §4 Punkt 6, §5 |
| N-SI-019 | Es gibt eine Weltuhr; Fortschreiben läuft nie der Weltuhr voraus | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 5 |
| N-SI-020 | Zahlen (Anzahl Entitäten, Tick-Budgets, Speicher je Datensatz) werden nicht gewählt, sondern an einem Prototyp gemessen; keine Zahl darf die Weltgröße festschreiben | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §5 |
| N-SI-021 | Portable C++20-Kernbibliotheken für technische Logik mit klarem Nutzen (Regeln, Zustandsübergänge, Speicherschemata, Teile der Antriebslogik) und Unreal-Adapter für Actors, Komponenten, Navigation und Laufzeitintegration | 📋 | `CLAUDE.md` §5 |
| N-SI-022 | Fedora-Laptop für Code und schnelle Tests (16 GB, integrierte Grafik, Editor eventuell unpraktisch); Windows-Desktop (RTX 3090, 32 GB) für Unreal-Integration und Rendering-Tests | 📋 | `CLAUDE.md` §5 |
| N-SI-023 | Das System darf nicht gestartet werden, bevor seine Mindest-Eingaben existieren | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §8 |
| N-SI-024 | Ressourcen für NPC-Verkehr, Menschenmengen und Bahnbetrieb werden mit Detailstufen abstrahiert; Untergrundbevölkerung wird unabhängig von der Lore-Zahl budgetiert; Budgets, Performance-Stufen und deterministische Resynchronisation sind offen | 📋 / ❓ | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §8 |

---

## 2. Entwurf: Die Simulationsstufen

Die Beispielstufen des Assistenten (`35#278A`) werden hier als Arbeitsmodell **S0 bis S3** geführt, ergänzt um **SD** für Menschenmengen (💡). Die Namen sind Arbeitsnamen; die Welt-Dokumentation führt sie als 🧭 (W-TV-010).

| Stufe | Name | Darstellung | Aktualisierung | Inhalt der Berechnung | Wer ist hier (Entwurf) |
|---|---|---|---|---|---|
| **S0** | nah / Held | voller Actor mit Zustandslogik (StateTree), Wahrnehmung, Gedächtnis live | pro Frame bis wenige Hz | Wahrnehmung, Pfad, Animation, Dialog, Interaktion | Figuren im Gespräch oder im Sichtbereich des Spielers; Story-Figuren in Szene; Begleiter |
| **S1** | lokal aktiv | reduzierte Actors oder leichte Agenten (vereinfachte Animation, vereinfachte Wahrnehmung) | niedrige Frequenz, gestaffelt | Zeitplan, grobe Pfade, einfache Reaktionen | Bewohner und Passanten der geladenen Region, Verkehr, Läden |
| **S2** | regional | kein Actor; Zustandsdatensatz | Minuten bis Stunden Weltzeit | Zustand (`AT_WORK` …), Wechsel nach Plan, Ereignisse | NPCs und Gruppen in benachbarten, nicht geladenen Regionen |
| **S3** | dormant / global | nur Datensatz (ID, Seed, Overrides) | Tage bis Wochen Weltzeit, bei Bedarf | grobe Lebensereignisse, Umzüge, Berufswechsel, Beziehungsänderungen | Rest der Welt |
| **SD** | Menge | instanziert, ohne eigene Identität; aus Ortszustand erzeugt | gemeinsam | Dichte nach Tageszeit, Ereignis, Ort; Routen | Stadion, Festival, Innenstadt, Nachtleben |

**Zuordnung zu Persistenzstufen (Entwurf).** Persistenz (A–D) und Simulation (S0–S3) sind zwei Achsen ([Modul 01](01-Quellenlage-und-Methodik.md) §4; [Modul 03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md) §6). Ein A-NPC kann in S3 ruhen, ein C-NPC in S0 im Gespräch stehen.

**Was jede Stufe leisten muss (Entwurf):**

1. **S0:** das Verhalten ist das, was der Spieler sieht und prüft.
2. **S1:** Vorübergehende Plausibilität; die Hintergrundfiguren zeigen Alltag (kein Stillstehen, keine Wiederholungsschleife).
3. **S2:** Zustand ist jederzeit in S1/S0 überführbar.
4. **S3:** Der Zustand bleibt konsistent und macht Fortschritt (N-SI-009).
5. **SD:** Dichte glaubwürdig, ohne Identitätskosten.

---

## 3. Entwurf: Ereignisbasiertes Fortschreiben

Die Quellen verlangen Fortschreiben ohne Sekundenrechnung (N-SI-011), deterministisch (N-SI-012) und konsistent (N-SI-005).

### 3.1 Grundoperation

```text
advance(entity, t_from, t_to):
    seed       = H(world_seed, entity_id, window)            -- N-SI-012
    state      = read(entity)                                  -- Datensatz
    while t < t_to:
        next   = next_scheduled_change(state, plan, calendar)  -- Zeitplan (Modul 13)
        events = sample_events(state, [t, next), seed)         -- Lebensereignisse, Begegnungen
        apply(state, events)                                   -- Zustand ändern (Modul 03, 14)
        publish(events_of_interest_to_others)                  -- an andere Entitäten, Gerüchte, Fraktionen
        t      = next
    write(entity, state); cursor(entity) = t_to
```

**Eigenschaften (Entwurf):**

1. **Zeitfenster.** Der Zufall hängt vom Fenster ab, nicht von der Aufrufreihenfolge (N-EN-471).
2. **Größere Schritte bei längerer Pause.** Bei langen Zeiträumen werden Tagesrouten nicht abgespielt, nur Lebensereignisse mit ihren Wahrscheinlichkeiten (Abschnitt 3.3).
3. **Keine doppelten Aufenthalte.** Der Plan lässt eine Entität nicht an zwei Orten sein (N-SI-011).
4. **Reisezeit.** Ortsänderungen verbrauchen Zeit (N-SI-011); die Route wird nicht berechnet, nur Dauer und Verkehrsmittel.
5. **Veröffentlichung statt Eingriff.** Ereignisse mit Wirkung auf andere werden veröffentlicht, nicht direkt geschrieben (Modul 24).

### 3.2 Wann wird fortgeschrieben?

| Modus | Beschreibung | Eignung |
|---|---|---|
| **Global tick (grob)** | alle Entitäten regelmäßig | zu teuer für Hunderttausende |
| **Lazy (bei Bedarf)** | nur wenn jemand den Zustand braucht (Ladevorgang, Dialog, Ermittlung) | billig, aber kausal heikel |
| **Hybrid (Entwurf)** | wenige wichtige Akteure (Fraktionen, Nemesis, Gerüchte, A/B-NPCs) grob regelmäßig; die Masse (C, S3) lazy | trägt N-SI-001 |

### 3.3 Lazy-Fortschreiben und Kausalität

Lazy-Fortschreiben birgt ein Problem: Entität A reagiert auf Ereignisse von B, aber B wurde noch nicht fortgeschrieben. Der Entwurf löst das mit **Ereigniseingängen** (Modul 21 §7):

1. **Ereignisse haben Zeitstempel und Quelle** und werden in den Eingang der Empfänger gelegt, sobald sie entstehen (auch für nicht geladene Empfänger).
2. **Beim Fortschreiben eines Empfängers** werden die Eingangsereignisse in Weltzeitreihenfolge verarbeitet.
3. **Abhängigkeiten werden vor dem Fortschreiben aufgelöst:** Wer für das Fortschreiben von A den Zustand von B braucht, schreibt B zuerst bis zum benötigten Zeitpunkt fort (begrenzte Rekursion, zyklenfrei durch Zeitordnung).
4. **Das Ergebnis ist reihenfolgeunabhängig**, sofern die Zeitordnung gilt (N-EN-461, N-EN-472).

### 3.4 Lange Pausen

Wenn der Spieler viele Weltwochen oder -jahre überspringt (Zeitsprung, Haft, Schlaf): Das System arbeitet in groben Schritten (Tage, Wochen), prüft **Eskalationsstufen nacheinander** (Fraktionen: N-EN-338) und überspringt keine Schwellen. Die Alltagsroutine wird nicht abgespielt, sondern als Rauschen modelliert (N-SI-011).

---

## 4. Entwurf: Materialisierung und Rückschreiben

Die Materialisierung macht aus Zustand wieder Akteure (N-SI-005); Modul 13 §8.2 beschreibt die Inhalte. Das Performance-Thema ist, **wer** materialisiert wird und **wie viele** (💡):

| Frage | Entwurf |
|---|---|
| **Wer?** | Alle Figuren, die der Spieler sehen oder berühren kann: nach Priorität (Entfernung, Sichtbarkeit, Bedeutung, Interaktion, Relevanz) |
| **Wie viele?** | Obergrenze aus dem Budget (Abschnitt 6); der Rest bleibt S1/SD |
| **Auswahl** | Wichtigkeit zuerst (A/B), dann Sichtbarkeit, dann Auffüllung |
| **Dichte** | folgt dem Ortszustand (Tageszeit, Wochentag, Ereignis, Wetter), nicht dem Budget allein; die Menge sieht belebt aus (N-SI-005) |
| **Identität** | S1-Agenten haben ihre ID; SD-Personen erhalten bei Ansprache eine ID (Modul 03 §10) |
| **Aussehen** | aus dem Rezept ([Decision 0005](../../Decisions/0005-character-appearance-foundation.md)); ein Pool von Modellen/Instanzen wird wiederverwendet; die Persistenz gehört der ID, nicht dem Modell |
| **Wiederholungen** | gleiche Gesichter nebeneinander sind zu vermeiden (Rezeptvielfalt) |
| **Vorhersage** | Bei schnellen Bewegungen (Fahrzeug) wird die Zielregion vorgeladen (Streaming) |
| **Rückschreiben** | Beim Verlassen der Nähe wird Zustand gespeichert (Ort, Aktivität, Verletzungen, Zusagen); Sim-Cursor auf jetzt |
| **Verlustfreiheit** | wichtige Ergebnisse (Gespräche, Verletzungen, Zusagen) gehen nicht verloren (N-SI-018) |

**Besonderheiten.**

1. **Zeugen bei Verbrechen:** Die Materialisierung berücksichtigt, wer zur Tatzeit wo war (Alibi-Konsistenz: Modul 13 §8; Ermittlung 12 §8).
2. **Zeitleisten:** Wer im Hintergrund schon „unterwegs“ war, taucht nicht „frisch“ aus dem Nichts auf, sondern am passenden Punkt der Route (soweit sichtbar).
3. **Innenräume:** Hier gilt dieselbe Logik mit Räumen statt Straßen (Besetzung nach Plan; `23#1058A`).
4. **Popping:** sichtbare Sprünge sind zu vermeiden (Proposal 0010 §7 Punkt 4).

---

## 5. Entwurf: Übergänge zwischen Stufen

| Übergang | Auslöser (Entwurf) | Bedingungen |
|---|---|---|
| S3 → S2 | Region wird „nahe“ (Streaming-Vorlauf) | Fortschreiben bis jetzt |
| S2 → S1 | Region wird geladen | Materialisierung als leichter Agent |
| S1 → S0 | Spieler nähert sich, spricht an, sieht lange, Ereignis | höhere Detailstufe |
| S0 → S1 | Entfernung, außer Sicht | Rückschreiben, Hysterese |
| S1 → S2 | Region wird entladen | Rückschreiben als Zustand |
| S2 → S3 | lange Ferne | Verdichtung |
| beliebig → S0 | Story-Event, Ermittlungsprüfung (zum Beispiel Alibi), Koop-Spieler | Priorität |
| Persistenz-Aufstieg (Datensatztiefe) | Begegnung/Relevanz | Einfrieren (N-ID-013) |

**Regeln (Entwurf):**

1. **Hysterese.** Auf- und Abstieg nutzen verschiedene Schwellen, um Flackern zu vermeiden.
2. **Verlustfreiheit.** Wichtige Ergebnisse gehen nicht verloren (N-SI-018).
3. **Konsistenz.** Der Zustand nach einem Übergang ist mit dem davor vereinbar (Position, Aktivität, Verletzung).
4. **Rangfolge.** Bei knappem Budget gewinnen wichtige Figuren (A) und Figuren mit laufendem Gespräch.
5. **Schwellen sind Konfiguration** (N-SI-018); sie werden gemessen, nicht gesetzt (N-SI-020).
6. **Keine Entladung mitten in Interaktion.** Ein NPC im Gespräch bleibt S0.
7. **Gruppen als Einheit.** Wer zu einer Gruppe gehört (Familie auf dem Weg), wechselt gemeinsam (vermeidet getrennte Familien).

---

## 6. Entwurf: Budgets und Messgrößen

Das Proposal 0010 §5: ‚Numbers are not chosen; measure with a prototype before fixing them.‘ Der Entwurf benennt **Messgrößen**, nicht Werte (💡):

| Größe | Beschreibung | Wer misst |
|---|---|---|
| **Max S0-Actors** | gleichzeitige volle Akteure | Laufzeit, Profiling |
| **Max S1-Agenten** | gleichzeitige leichte Agenten | Laufzeit |
| **KI-Zeit pro Frame** | ms für Wahrnehmung, Entscheidung, Pfad | Profiling |
| **Fortschreibungskosten** | µs oder ms je Entität und Weltstunde/-tag in S2/S3 | Mikro-Benchmark |
| **Fortschreiben beim Laden** | Dauer für eine Region (Nachholen von x Tagen) | Ladezeit-Tests |
| **Speicher je Datensatz** | Bytes je Stufe und Tiefe | Messung (Modul 21 §5) |
| **Speicher in RAM** | residente Datensätze je Region | Messung |
| **Ladezeit** | Segmente, Regionen | Messung |
| **Materialisierung** | Zeit und Aufbau je Kopf | Profiling |
| **Speicherstandgröße** | nach Szenario | Modul 21 |
| **Bandbreite im Koop** | Replikation von S0/S1 | Netztests |

**Form des Budgets (Entwurf).** Ein Frame-Budget ist die Summe der Kosten der Stufen:

`B_frame ≥ n0·c0 + n1·c1 + (Anteil der Fortschreibung)·c_adv + (Materialisierungen)·c_mat`

mit unbekannten Größen n (Anzahl) und c (Kosten). Keine Werte werden hier gesetzt.

**Messplan (Entwurf):**

1. **Prototyp L4 (Proposal 0010 §10):** gestufte Simulation über eine kleine Testregion; Messung der Kosten.
2. **Zwei Zielplattformen:** Windows-Desktop (RTX 3090, 32 GB) und eine schwächere Konfiguration; der Fedora-Laptop eignet sich für portable Logik und schnelle Headless-Tests (N-SI-022).
3. **Reproduzierbare Szenarien:** feste Seeds, feste Eingaben, gespeicherte Messläufe (Regression).
4. **Skalierung:** gleiche Tests mit wachsender Population (10², 10³, 10⁴, 10⁵ Identitäten), um das Skalierungsverhalten zu erkennen — **ohne** die Weltgröße festzulegen (N-SI-001).

---

## 7. Entwurf: Determinismus und Reihenfolge

Die Quellen verlangen Reproduzierbarkeit (N-SI-012). Für Tests, Koop und Spielstand braucht das:

| Maßnahme | Beschreibung |
|---|---|
| **Seeds aus IDs und Fenstern** | `H(world_seed, id, window)`; keine Systemzeit, keine Zufallsquelle des Betriebssystems |
| **Spezifizierte Zufallsfunktion** | eigene, plattformunabhängige Ganzzahl-Funktion (siehe Modul 21 §6.4) |
| **Strikte Reihenfolge** | Ereignisse nach Weltzeit, dann ID, dann Sequenznummer |
| **Keine Frame-Abhängigkeit** | Ergebnisse hängen nicht von Framerate oder Threadplanung ab |
| **Parallelität ohne Folge** | parallele Fortschreibung nur, wenn Ergebnisse reihenfolgeunabhängig sind; sonst serielle Sequenz an den Schnittstellen |
| **Gleitkomma** | keine Entscheidung im Weltzustand hängt von plattformabhängigen Gleitkommaergebnissen ab |
| **Logging** | Fortschreibungen sind als Ereignisse im Journal nachvollziehbar (Modul 21 §4) |
| **Replay-Test** | gleiche Eingaben → gleiche Zustände (Modul 29) |

**Wichtig:** Determinismus gilt für den **Weltzustand** (Entscheidungen, Ereignisse, Zustände). Darstellung (Animation, Partikel, genaues Pfadfolgen im Raum) darf variieren.

---

## 8. Entwurf: Lastspitzen und Degradation

| Lastspitze | Beispiel | Reaktion (Entwurf) |
|---|---|---|
| **Viele Materialisierungen** | Schnellreise in eine dichte Stadt | gestaffelte Materialisierung über mehrere Frames; zuerst Nahbereich und wichtige Figuren |
| **Lange Fortschreibung** | Region nach langer Abwesenheit | grobe Schritte; Fortschreibung im Hintergrund; Ladebildschirm nur als letztes Mittel |
| **Großereignis** | Stadion, Katastrophe, Evakuierung | SD statt S1 für die Masse; Einzelfiguren begrenzt |
| **Koop** | vier Spieler an verschiedenen Orten | Vereinigung der Nahbereiche; globales Budget |
| **Massenhafte Abhängigkeiten** | eine Nachricht erreicht tausende | Weitergabe auf Kreisebene (Modul 07 §11) |
| **Schwache Hardware** | Laptop mit integrierter Grafik | niedrigere Dichten, größere Schritte; Weltzustand unverändert |

**Prinzipien.**

1. **Degradiere zuerst die Sichtbarkeit, dann die Detailtiefe, zuletzt die Korrektheit.** Wichtige Ergebnisse gehen nie verloren (N-SI-018).
2. **Weltzustand und Darstellung getrennt.** Weniger Statisten bedeuten nicht anderen Weltzustand.
3. **Qualitätsstufen in den Einstellungen** (`M-PD`: Leistungsziele und Grafikeinstellungen); Dichte und Distanz sind einstellbar, die Logik bleibt gleich.
4. **Keine stillen Verluste.** Wird ein Ereignis zurückgestellt, wird es später nachgeholt.

---

## 9. Entwurf: Architektur und Kandidaten

`CLAUDE.md` §5: Portable C++20-Bibliotheken dort, wo es klaren Nutzen bringt; Unreal-spezifische Dinge hinter Adaptern. Für die NPC-Welt (💡):

| Schicht | Inhalt | Ort |
|---|---|---|
| **Kern (portabel)** | Datensätze, Ereignisse, Gedächtnis, Beziehung, Zeitplan, Fortschreibung, Fraktionsmodell, Seeds, Migration | portable C++20-Bibliothek mit Standalone-Tests (läuft auf dem Fedora-Laptop ohne Unreal) |
| **Adapter (Unreal)** | Materialisierung als Actors, StateTree-Integration, Wahrnehmung, Navigation, World-Partition-Streaming, Speicher-IO | Unreal-Module |
| **Daten** | Vorlagen (Berufe, Zeitpläne, Fraktionstypen, Taktikvorräte), Mod-Pakete | datengetrieben |
| **Werkzeuge** | Inspektoren, Replay, Messläufe | Editor/Dev |

**Kandidaten zur Prüfung (nicht entschieden, keine Empfehlung ohne Prototyp):**

| Kandidat | Zweck | Anmerkung |
|---|---|---|
| **StateTree** | hierarchische Zustandslogik für wichtige NPCs | in den Quellen genannt (`23#47A`/`23#48A`; `M-PD`) |
| **Unreal-Mass-Systeme / Datenorientierte Entitäten** | große Mengen leichter Agenten (S1, SD) | als Kandidat zu prüfen; Abhängigkeit/Architektur braucht Genehmigung (`CLAUDE.md` §7) |
| **Smart Objects / Zonen** | Orte mit Aktivitäten für Agenten | Kandidat |
| **World Partition / Data Layers** | Streaming und Ortszustand | durch Welt-Dokumentation gesetzt (Welt 18) |
| **Eigener Fortschreibungskern** | S2/S3, portabel | wahrscheinlich Eigenentwicklung, da engine-unabhängig |
| **Job-System/Parallelität** | parallele Fortschreibung | nur mit Determinismusgarantie |

**Wichtig:** Diese Liste ist **kein Beschluss**; keine Abhängigkeit wird dadurch eingeführt.

---

## 10. Entwurf: Koop-Autorität

Die Quellen: Host-Speicherstand maßgeblich; Gäste im Host-Speicherstand; bis zu vier Spieler (`CLAUDE.md` §6); die Replikation des Simulationszustands ist offen (Proposal 0010 §6).

| Aspekt | Entwurf |
|---|---|
| **Autorität** | die Simulation (S2/S3, Fortschreiben, Fraktionen, Nemesis) läuft nur auf dem Host |
| **Nahbereiche** | S0/S1 werden für die Vereinigung der Nahbereiche aller Spieler materialisiert; Replikation an Gäste nach Relevanz |
| **Deterministisch** | gleicher Seed → gleiches Ergebnis; Gäste müssen den Weltzustand nicht berechnen, nur darstellen |
| **Beitritt mitten im Spiel** | Gast erhält Zustand aus Host-Segmenten |
| **Zeit** | gemeinsame Weltuhr; Zeitsprünge nur nach den Koop-Regeln (`48#164A`: gemeinsame Welt; Ermittlung 19) |
| **Budget** | wächst nicht linear mit Spielern; globale Obergrenze |
| **Offen** | Replikationsformat, Latenz, Bandbreite (Q-N15, Q-N26) |

---

## 11. Sonderfälle

1. **Schnellreise.** Ziel wird mit Vorlauf geladen; Fortschreibung für Zielregion; Materialisierung gestaffelt.
2. **Fahrzeug mit hoher Geschwindigkeit.** Streaming-Vorhersage; S1-Agenten entlang der Strecke (Welt 16).
3. **Sehr lange Pause (Jahre).** Grobe Schritte; Eskalationsstufen werden nacheinander geprüft (N-EN-338).
4. **Zeitreise/Reiche.** Eigene Zustände je Zeitstand (Modul 21 §9).
5. **Cutscene.** Betroffene NPCs werden S0; danach Rückschreiben.
6. **Fotomodus, Pause.** Keine Weltzeit.
7. **Mod mit sehr vielen NPCs.** Budget und Obergrenzen gelten für Mod-Inhalte (Modul 23).
8. **Katastrophe mit tausenden Betroffenen.** Verdichtung der Hintergrundopfer; wichtige NPCs einzeln (Modul 15).
9. **Spieler beobachtet einen Hintergrund-NPC über Stunden.** Der NPC wird S0/S1 und bleibt konsistent; kein Wechsel mitten im Blick.
10. **Ermittlung prüft Alibi eines S3-NPC.** Aufenthalt wird konsistent rekonstruiert (Ermittlung 12 §8).
11. **Mehrere Spieler, ein NPC.** Der NPC ist S0 beim nächsten Spieler; die anderen sehen repliziertes Verhalten.
12. **Untergrund.** **historisch (N-02, 08.11.2026)** für die Untergrundzivilisation; Restfrage für Bunker, Höhlen und kleinere Untergrundbereiche bleibt offen (Q-N25). Die unabhängige Budgetierung (N-SI-024) ist ebenfalls historisch.

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-471 | Fortschreiben ist reihenfolgeunabhängig: Das Ergebnis hängt von Seed, Fenster und Weltzustand ab, nicht von der Aufrufreihenfolge | N-SI-012 |
| N-EN-472 | Ein Ereignis wird erst verarbeitet, wenn alle Ereignisse mit früherem Zeitstempel für diesen Empfänger verarbeitet sind | Abschnitt 3.3 |
| N-EN-473 | Ein Zustandswechsel S0↔S1↔S2↔S3 verliert keine wichtigen Ergebnisse (Verletzung, Zusage, Gedächtnis) | N-SI-018 |
| N-EN-474 | Die Materialisierung ist mit dem letzten Zustand vereinbar (Ort, Aktivität, Zustand) | N-SI-005 |
| N-EN-475 | Beim Betreten eines Gebiets ist die Straße der Tageszeit gemäß belebt | N-SI-005 |
| N-EN-476 | Kein NPC ist in der Simulation an zwei Orten gleichzeitig | N-SI-011 |
| N-EN-477 | Wege kosten Weltzeit, auch ohne Route | N-SI-011 |
| N-EN-478 | Das Fortschreiben läuft nie der Weltuhr voraus | N-SI-019 |
| N-EN-479 | Dieselben Eingaben liefern auf Windows und Linux dieselben Weltzustandsentscheidungen | Abschnitt 7 |
| N-EN-480 | Ein NPC im Gespräch wechselt nicht die Stufe | Abschnitt 5 |
| N-EN-481 | Die Weltgröße ist in keiner Simulationsgröße fest verdrahtet | N-SI-001 |
| N-EN-482 | Unter Last verändert sich Darstellung und Dichte, nicht der Weltzustand | Abschnitt 8 |
| N-EN-483 | Lange Pausen prüfen Eskalationsstufen nacheinander | Abschnitt 3.4 |
| N-EN-484 | Die Kosten der Fortschreibung wachsen nicht mit der Zahl unveränderter Identitäten | N-PE-007 |
| N-EN-485 | Die Simulation läuft nur auf der Autorität (Host) | Abschnitt 10 |
| N-EN-486 | Ein NPC mit höherer Priorität verdrängt einen mit niedrigerer aus dem Budget, nie umgekehrt | Abschnitt 5 |
| N-EN-487 | Hysterese verhindert Stufenwechsel bei Grenzbewegung | Abschnitt 5 |
| N-EN-488 | Der Replay-Test eines Zeitraums liefert denselben Zustand | Abschnitt 7 |
| N-EN-489 | Gleichzeitig materialisierte Personen derselben Gruppe wechseln gemeinsam die Stufe | Abschnitt 5 |

---

## 13. Was fehlt

- **Keine Budgets, Schwellen, Radien, Schrittweiten** (N-SI-017, `M-OQ16`).
- Offen: Reichweite von Hunderttausenden Identitäten und die Zahl aktiver Entitäten (W-TV-010).
- Offen: Wahl der technischen Bausteine (Abschnitt 9).
- Offen: Replikation im Koop (Q-N26).
- Offen: Untergrund und Reiche (Q-N25).
- Offen: Hardwarefloor und Qualitätsstufen (`M-PD` Rendering-/Leistungsziele; Welt 18).
