# Kreaturen 17: Persistenz, Daten und Modding

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie der Zustand von Kreaturen gespeichert werden könnte und wie Kreaturen modbar sein sollen. Der **einzige gebaute Baustein** ist `WorldFacts` (Schlüssel-Wert-Journal im Kern); das Kreaturenmodell ist größer, als dieser Baustein heute trägt. Zahlen sind **Rechnungen aus dokumentierten Eigenschaften**, keine Messungen. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Die technische Grundlage ist in [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) beschrieben; hier steht nur, was für Kreaturen hinzukommt.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Was gespeichert werden muss](#2-was-gespeichert-werden-muss)
3. [Passt das in WorldFacts?](#3-passt-das-in-worldfacts)
4. [Entwurf: Schlüsselschema und Größenrechnung](#4-entwurf-schlüsselschema-und-größenrechnung)
5. [Detailstufen der Simulation](#5-detailstufen-der-simulation)
6. [Modding](#6-modding)
7. [Koop und Online](#7-koop-und-online)
8. [Entwurf: Prüfregeln](#8-entwurf-prüfregeln)
9. [Sonderfälle](#9-sonderfälle)
10. [Proposal 0010: Grenzen und Vorschläge zur Speicherung](#10-proposal-0010-grenzen-und-vorschläge-zur-speicherung)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Der zentrale persistente Weltzustand ist das ‚Gedächtnis der Welt‘: Fallergebnisse, NPC lebt/tot, Beziehungen, veränderte oder zerstörte Orte, entdecktes Wissen, **Monsteraktivität**, Ruf, Fahrzeuge, Safehouses, Weltereignisse, Zeit | `M-PD` ‚World state and persistence‘ | 📋 |
| Stabile IDs für wichtige persistente Entitäten | `M-PD` | 📋 |
| Speicherstände sind segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert (kein monolithischer Blob); Langzeitziel etwa 20–50 MB, wenn vernünftig erreichbar; Korrektheit, Zuverlässigkeit, Ladezeit und Wiederherstellung gehen vor Größe | `M-PD` | 📋 |
| Wichtige NPCs haben Gedächtnis; Hintergrundpopulationen sind leichter | `M-PD` ‚NPCs and AI‘ | 📋 |
| Das Monster-Framework muss **stark modbar** sein: Creature Definition, Verhalten, Anatomie, Fähigkeiten, Schwächen, Archiv-Einträge, Assets, Animationen, Sounds, Fall-Integration; neue Kreaturen ohne Überschreiben bestehender; native C++ für hochkomplexe Kreaturen möglich | `23#293A` Punkt 23, „23. passt“ in `23#294E` | 📋 |
| Mods sind standardmäßig additiv und nicht-destruktiv über APIs, Registries und Hooks; kontrollierte Überschreibungen über explizite Mod-Schichten und Ladereihenfolge; Schichten: Daten-/Inhalts-Mods, Unreal/Blueprint, native C++20 | `M-PD` ‚Modding‘ | 📋 |
| Native Mods sind vertrauenswürdiger nativer Code; öffentliche APIs mit geringsten Rechten | `M-PD` ‚Modding‘ | 📋 |
| Entwicklungen der Monsterterritorien bleiben im jeweiligen Host-Spielstand | `48#144A` | 📋 / 💡 |
| Fallen brauchen persistenten Zustand mit nachvollziehbaren Veränderungen, ohne entfernte Gebiete permanent voll zu simulieren | `48#134A` | 📋 / 💡 |
| Das Monster hat ein persistentes Ereignis- und Erinnerungssystem; nicht jede Kleinigkeit wird gespeichert | `48#140A` | 📋 / 💡 |
| Die Welt läuft während Haft und Zeitsprung weiter; lange Zeiträume werden in größeren Schritten zusammengefasst | „Weltentwicklung während Haft: C ja D ist longterm“ in `48#159E`; [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) | 📋 |
| Kein Mod im Online (Cheat-Gefahr); im privaten Koop Cheats und Mods erlaubt | `23#551E`, `23#553E`, `23#440E` | 📋 |

---

## 2. Was gespeichert werden muss

Aus den Kreaturenmodulen ergibt sich folgender Zustandsumfang (💡 Zusammenstellung):

| Bereich | Zustand | Modul |
|---|---|---|
| Individuum | ID, Art, Alter, Erfahrung, Temperament, Narben/Verletzungen, Ort, Status (lebt, tot, gefangen, geflohen, gebunden), Ausnahmemerkmale | 03, 04 |
| Gedächtnis | Begegnungen, gelernte Taktiken, Gefühle, Weitergabe, Wiedererkennung | 10 |
| Beziehungen | zwischen Individuen, Gruppen, Spieler | 09 |
| Gruppe/Nest | Mitglieder, Struktur, Territorium, Ressourcen, Haltung zu anderen | 08, 09 |
| Population | Zähler je Region und Art, Druckfaktoren | 08 |
| Besessenheit, Fluch, Geist | Zustände mit Eskalationsplan, Anker, Auslöser | 05 |
| Ritual, Artefakt, Falle | Fortschritt, Besitzerkette, Integrität | 12, 13 |
| Wissen des Spielers | KnowledgeClaims mit Status und Quellen | 14 |
| Vorfälle | Samen, Signale, Fristen, Ausgang | 15 |
| Ortsgeschichte | Ereignisse, Besitzerkette | 16 |
| Seele/Tod/Reise | Zustand der Personen, Reise, Zeit | 07 |
| NPC-Hunter | Wissen, Haltung, Fall, Leben/Tod | 11 |

---

## 3. Passt das in WorldFacts?

`WorldFacts` kann heute (`Core/include/spn/core/WorldFacts.h`, [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §2): `bool`, `int64`, `string` bis 4096 Bytes; Schlüssel 1–128 Zeichen mit `a–z`, `0–9`, `_`, `.`; Journal, `StateAt(n)`, atomare Mehrfachbefehle über das Interaktionssystem. Es ist ein **flaches** Modell ohne Listen, Tabellen und Verweise.

| Zustandsart | Eignung | Anmerkung |
|---|---|---|
| Status, Zähler, Flags (lebt/tot, Population) | gut | direkte Abbildung |
| Kurze Texte (Archiv-Notiz, Name) | gut | bis 4096 Bytes |
| Listen (Narben, Gedächtniseinträge, Besitzerkette) | mittel | als nummerierte Schlüssel (`creature.<id>.scar.0001`), unhandlich |
| Beziehungsgraphen | schlecht | viele Schlüssel, Abfragen schwer |
| Gedächtnis mit vielen Ereignissen | schlecht | Journal wächst; Verdichtung offen |
| Sehr große Zahl von Individuen | offen | Ladezeit wächst linear mit dem Journal |

Die Entscheidung 0004 sagt, dass ‚Fall- und Beweisfortschritt‘ zum Weltzustand gehören kann; **wie** ein Gedächtnis- oder Beziehungsmodell gespeichert wird, ist nicht entschieden (❓ Q-K25, vgl. [Ermittlung 21](../Ermittlung/21-Konflikte-und-offene-Fragen.md) Q-E67, Q-E71). Es gibt hier nur drei Linien (💡, alle mit Prüfung vor Nutzung):

1. **Alles in WorldFacts** (flach, nummeriert): einfach, aber Journal-Wachstum und Abfragen kritisch.
2. **WorldFacts für Status, getrennte Segmente für Gedächtnis und Beziehungen** (passt zu ‚segmentiert‘ im Master).
3. **Ein eigener Kreaturen-Store** mit Schnittstelle zu WorldFacts (Status/Wirkung dorthin, Details dort).

Welche Linie gilt, bleibt **dem Entwickler** vorbehalten (Architekturfrage, `CLAUDE.md` §7).

---

## 4. Entwurf: Schlüsselschema und Größenrechnung

> 💡 **Vorschlag.** Er zeigt, wie Kreaturenstatus als Fakten aussehen könnte, und rechnet ungefähr, was das kostet. Er legt nichts fest.

### 4.1 Schlüssel

```text
creature.<individual_id>.status          string   alive|dead|captured|fled|bound|unknown
creature.<individual_id>.species         string
creature.<individual_id>.region          string
creature.<individual_id>.age_years       int
creature.<individual_id>.scar.<n>        string   z. B. "eye_left_lost"
creature.<individual_id>.memory.<n>      string   z. B. "player_used_shotgun"
group.<group_id>.leader                  string
group.<group_id>.territory.<n>           string
population.<region_id>.<species_id>      int
curse.<id>.stage                         int
ghost.<id>.anchor.<n>.status             string
knowledge.<claim_id>.status              string
```

Alle Beispiele erfüllen das Schlüsselmuster (Kleinbuchstabe zuerst, nur `a–z`, `0–9`, `_`, `.`, höchstens 128 Zeichen); ob Identifikatoren mit Bindestrich oder Großbuchstaben zulässig sein sollen, ist eine Frage des Schemas, nicht dieser Dokumentation.

### 4.2 Größenrechnung (Annahmen offen)

Die Ermittlungs-Dokumentation nennt für das Journal etwa **60 Bytes je protokolliertem Befehl** (Decision 0004; eine Messung auf einer Maschine). Daraus folgt arithmetisch:

| Annahme | Rechnung | Ergebnis |
|---|---|---:|
| 1.000 persistente Individuen mit je 30 Fakten beim ersten Setzen | 1.000 × 30 × 60 B | 1,8 MB |
| zusätzlich 10 Änderungen je Individuum und Spielstunde über 100 Stunden | 1.000 × 10 × 100 × 60 B | 60 MB |

Die zweite Zeile liegt **über dem Langzeitziel von 20–50 MB** (`M-PD`), und das ist nur eine Annahme; die Aussage, dass das Journal ohne Verdichtung (Compaction) ungebremst wächst, steht im Kernproblem der Ermittlung 19 (Format v2 offen). Die Rechnung zeigt nur: **Die Anzahl der Änderungen pro Individuum ist die entscheidende Größe**, nicht die Anzahl der Individuen. Eine Obergrenze für Schreibvorgänge und eine Verdichtung sind nötig (❓ Q-K25).

---

## 5. Detailstufen der Simulation

Der Master verlangt, dass nicht alles voll simuliert wird (‚Not every NPC needs full simulation‘, `M-PD`). Für Kreaturen:

| Ebene | Wann | Zustand im Speicher |
|---|---|---|
| A: Individuum voll | Spieler in der Nähe; wichtige Individuen | vollständiges Gedächtnis, Wahrnehmung, Taktik |
| B: Gruppe | Region geladen | Aufgaben, Position grob, Beziehungen |
| C: Population/Region | Ferne | Zähler, Territorienkarte, Druck |
| Persistent | immer | Status, Narben, relevante Erinnerungen, Gruppenstruktur |

Die Regel ‚Entwicklungen auch ohne Spieler‘ (`48#144A`) verlangt, dass Ebene C Ergebnisse produziert, die beim Laden einer Region **konsistent** in Ebene B/A erscheinen. Das Staffelungsprinzip nennt der Assistent für die Gesellschaft ausdrücklich: „abgestufte Simulation“ (`48#146A`, 💡).

---

## 6. Modding

### 6.1 Linien aus den Quellen

- Neue Kreaturen additiv hinzufügen, ohne bestehende zu überschreiben (`23#293A` Punkt 23).
- Native C++ darf für hochkomplexe Kreaturen möglich sein (ebd.).
- Mods nicht im Online (`23#551E`), im privaten Koop erlaubt (`23#440E`).
- Mods pro Spielstand ein- und ausschaltbar; Daten und Mod bleiben installiert, sind aber im Spiel nicht benutzbar, wenn ausgeschaltet (`35#71E`, Entwickler; Auslegung beim Entfernen: offen).
- Der Entwickler wünscht keinen externen Internetzugriff bei Mods, nur optional auf Steam, und das Spiel muss komplett ohne gespielt, gespeichert und gespielt werden können (`35#137E`).

### 6.2 Was ein Mod-Monster liefern müsste (💡)

Nach der Creature Definition ([Modul 02](02-Leitsaetze-und-Abgrenzung.md) §4): Identität, Dimensionen, Anatomie, Sinne, Verhalten, Fähigkeiten, Schwächen, Widerstände, Regeneration, Aktivitätsbedingungen, Archiv-Wissensquellen und Tests (‚Test Contracts‘, `23#307A` Punkt 27).

### 6.3 Grenze der Absicherung

Die Regel ‚keine zufällig zusammengewürfelte Schwäche/Fähigkeit‘ (`23#307A` Punkt 28) gilt für das **Basisspiel**; ein Mod kann Lore-widrige Kombinationen definieren. Das Spiel kann das nicht verhindern, sondern nur kennzeichnen (💡, ❓ Q-K26).

---

## 7. Koop und Online

| Aussage | Quelle | Status |
|---|---|:---:|
| Koop ist die Singleplayer-Kampagne für 2–4 Spieler; Welt und Fortschritt des Hosts | `23#442E`; `48#105E`, `48#107E`; [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) | 📋 |
| SPN Online: jeder Spieler hat sein eigenes Online (eigene Fälle); gemeinsame Jobs für Teilnehmer | `23#444E`, `23#446E` | 📋 |
| Online: Leviathan-Jobs nur als seltene große Jobs für 4–8 erfahrene Hunter, kein 100-Spieler-Raidboss | `23#467A` Punkt 31 (💡) | 💡 |
| Kreaturen-KI und -Zustand brauchen in beiden Modi eine **autoritative Instanz** (Host bzw. Server) | Architekturfolgerung (kein Quellenbeleg) | 💡 |

Wie Individuenzustand in Online (persönlich je Spieler) und die Wirkung geteilter Welt (Population) zusammenpassen, ist offen (❓ Q-K27).

---

## 8. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Speichern und Laden erhalten Status, Narben, Gedächtnis und Gruppenstruktur eines Individuums | `M-PD`; `23#293A` Punkt 24 |
| Ein getötetes Individuum bleibt nach Laden tot | `23#1075E` Punkt 30 |
| Ein Mod-Monster überschreibt keine Basisdaten | `23#293A` Punkt 23 |
| Eine Territoriumsänderung ohne Spieler ist nach Laden vorhanden | `48#144A` |
| Ein Fallenzustand schreitet voran, auch wenn der Bereich nicht geladen war | `48#134A` |
| Die Zahl der Schreibvorgänge je Individuum bleibt unter einer Obergrenze | Rechnung in Abschnitt 4.2 (❓ Obergrenze) |
| Das Journal ist nach Verdichtung gleichwertig zum Original | [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) (Compaction offen) |

---

## 9. Sonderfälle

| Fall | Behandlung |
|---|---|
| Ein Mod wird entfernt, dessen Individuen im Spielstand sind | Zustand bleibt; die Individuen sind unbenutzbar (❓) |
| Ein Individuum ist in zwei Spielständen (Koop-Gast) | Gast hat keinen Wissensstand des Hosts (`48#108A`) |
| Zeitsprung (Haft, Reise) | Population in größeren Schritten (`48#159E`) |
| Region wird nie geladen | Ebene C genügt |

---

## 10. Proposal 0010: Grenzen und Vorschläge zur Speicherung

`Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` (Stand 22.10.2026) behandelt die Speicherfrage für Gedächtnis, Nemesis und Fraktionen. **Entscheidung** ist nur Abschnitt 1 (`PE-N1022`: das Produktziel, [Modul 10](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §14). Alles Folgende ist **Ingenieurvorschlag** des Proposals (💡):

| Aussage des Proposals | Quelle im Proposal | Bedeutung für Kreaturen |
|---|---|---|
| Das Journal von `WorldFacts` wächst ohne Grenze; **hochfrequenter oder abgeleiteter Simulationszustand läuft nicht durch WorldFacts**; Snapshot plus Journal-Ende und Verdichtung sind offen | §6 | stützt die Linien 2 und 3 aus Abschnitt 3 (getrennte Segmente oder eigener Store) und ordnet Q-K25 in einen laufenden Entscheidungsrahmen ein |
| Entitätsdatensätze mit stabiler ID, nicht Actors; Actors sind die vorübergehende Darstellung | §4.1 | Individuen und Gruppen sind Datensätze (Abschnitt 2) |
| Ereignisjournal als Quelle der Wahrheit, Gedächtnis und Beziehung daraus ableitbar | §4.2 | passt zu ‚Gedächtnis aus Ereignissen‘ ([Modul 10](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §8) |
| Gedächtnis begrenzt (Stärke, beobachtet/gehört, Genauigkeit, Verfall, Wichtigkeit); nur plausible Beobachtung erzeugt Erinnerung | §4.3 | entspricht `M-PD` und `23#307A` P15 |
| Je Entität deterministisch fortschreibbar (Saat je Entität und Zeitschritt), damit Koop und Tests dasselbe Ergebnis haben | §4.7 | zur Ebene C in [Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §11 |
| **Ein Besitzer je Tatsache**; Querwirkungen als Ereignisse in definierter Reihenfolge | §7 | verhindert widersprüchlichen Kreaturenzustand zwischen Verletzung, Fall, Territorium, Ruf |
| Invarianten als Tests (zum Beispiel: ein toter Einzelner handelt nicht; eine Verletzung ist in jedem System sichtbar; ein Zeuge weiß nur, was er beobachtet hat; ein Fall zitiert nur Geschehenes; eine Fraktion hält kein Gebiet, das ihre Mitglieder nicht erreichen; Zustand nach Laden gleich Zustand vor Speichern) | §7 | siehe T-N in [Modul 23](23-Testkatalog-und-Abnahmekriterien.md) |
| Koop: Simulation nur auf der Autorität; Replikation offen; Mods registrieren neue Arten additiv; unbekannte Arten in Spielständen müssen unverändert laden und speichern | §6 | Abschnitte 6 und 7 |
| Das System startet nicht, bevor seine Mindest-Eingaben existieren | §8 | Reihenfolge ❓ |

Die Anzahl der Änderungen je Individuum bleibt die entscheidende Größe (Abschnitt 4.2). Welche Linie gilt, bleibt Entscheidung des Entwicklers.

---

## 11. Was fehlt
- Entscheidung über das Speichermodell für Gedächtnis und Beziehungen (❓ Q-K25).
- Verdichtung und Obergrenzen (❓, Format v2).
- Wie Mod-Kreaturen gekennzeichnet werden (❓ Q-K26).
- Online-Zustand je Spieler gegenüber geteilter Population (❓ Q-K27).
- Autorität und Synchronisation der KI im Mehrspielerbetrieb (❓).
