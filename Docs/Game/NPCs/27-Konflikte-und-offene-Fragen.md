# NPCs 27: Konflikte und offene Fragen

> [!IMPORTANT]
> **Dieses Modul entscheidet nichts.** Es führt Widersprüche, Spannungen, unklare Antworten, Begriffskollisionen, Abweichungen zu den Nachbardokumenten und offene Fragen **mit beiden Quellen**. Wo eine spätere ausdrückliche Entscheidung des Entwicklers vorliegt, steht sie dabei. Alles andere bleibt für den Entwickler offen; keine Zeile hier wurde eigenmächtig aufgelöst (`CLAUDE.md` §2.5 und §11.5).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md).

**Inhalt**

1. [Konflikte und Spannungen](#1-konflikte-und-spannungen)
2. [Unklare Antworten](#2-unklare-antworten)
3. [Begriffskollisionen](#3-begriffskollisionen)
4. [Abweichungen zu Nachbardokumenten](#4-abweichungen-zu-nachbardokumenten)
5. [Offene Fragen](#5-offene-fragen)
6. [Beziehung zu bestehenden Fragenlisten](#6-beziehung-zu-bestehenden-fragenlisten)
7. [Vorschlag zur Reihenfolge der Klärung](#7-vorschlag-zur-reihenfolge-der-klärung)

---

## 1. Konflikte und Spannungen

| Nr. | Thema | Aussage A | Aussage B | Stand |
|---|---|---|---|---|
| C-N1 | **Wer darf einen wichtigen NPC töten?** (Plot Armor gegen unabhängige Lebensentwicklung) | Der Entwickler: wichtige NPCs sollen nicht sterben, „außer du tötest sie“ (`23#1057E` Punkt 105); wichtige NPCs sind vor trivialen Simulationsunfällen geschützt (`23#1058A`); Kernfiguren des HQ haben keinen zufälligen Sandbox-Tod, ein geschriebener Story-Tod bleibt (`23#1089E` Punkte 25–26; `23#1090A`) | `M-PD`: ‚Player and human NPCs share the same fundamental human anatomy/trauma rules. No plot armor.‘; NPC-Hunter können sterben (`23#1074A` Punkt 12; Kreaturen 11 §3); Anforderungen D und F (`PE-N1022`): Fraktionen kämpfen, NPCs entwickeln sich ohne den Spieler, Tode sind Konsequenzen (E); Hunter-NPCs können ein Wesen töten (`M-PD`) | ⚠️ keine Ersetzung belegt. Offen ist, ob **andere Akteure** (Fraktionen, Hunter, Wesen, Behörden) wichtige NPCs töten dürfen und welche Stufe der Schutz hat (Modul 03 §8: `sim_safe`, `story_safe`, `narrative_only`). Nicht eigenmächtig aufgelöst |
| C-N2 | **Größenziel des Spielstands mit NPC-Zustand** | `M-PD`: langfristig etwa 20–50 MB, Korrektheit vor Größe; Entwickler: Backups „in 10mb“ als Wunsch (`23#49E` Punkt 17); „so lange alles geladen ist und perfekt funktioniert“ (`23#52E` Punkt 17) | Entwickler zu ~500.000 Identitäten: „ja eher kleiner“ (`35#279E` Punkt 40); Assistent: 50–150 MB, im Extremfall 150–300 MB (`35#280A`); gleiche Spannung in [Welt K13](../World/21-Konflikte-und-Widersprueche.md#k13-save-größenziel) | ⚠️. Die Zahlen des Assistenten sind 💡 und von keiner Entscheidung getragen; der Master koppelt die NPC-Zahl nicht. Rechenbeispiel: [Modul 21](21-Weltpersistenz-und-Speicherentwurf.md) §5 |
| C-N3 | **Weltzeit außerhalb des Spielens** | Entwickler: „die welt läuft nur wenn man gerade spielt, im online wird das anders sein“ (`35#279E` Punkt 11); keine heimliche Echtzeitsimulation | Zeitsprünge (Schlaf, Reise, Haft) werden nachgeholt (`35#278A` Frage 12; `48#164A`); Koop mit Host (`CLAUDE.md` §6); Online später „anders“ | kein Widerspruch im Singleplayer; **offen** für Koop-Host-Sitzungen (läuft die Welt, solange der Host nicht spielt? Antwort: nein nach Singleplayer-Logik, aber nicht ausdrücklich gesagt) und für Online (Q-N28) |
| C-N4 | **Datensätze Toter: löschen oder erhalten?** | „außer sie ist tod“ (`35#261E` Punkt 39; im Zusammenhang mit der Verdichtung von Partnerprofilen: Person und Vorlieben bleiben gespeichert, **außer** sie ist tot); [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §6 übernimmt das als ‚außer die Person ist gestorben‘ | Tod bleibt persistent; relevante Beziehungs- und Historiedaten werden nicht gelöscht, nur technischer Detailzustand wird verdichtet (`35#272A` Fragen 31–33; „1-50 passt alles“ in `35#273E`; `35#274A`) | ⚠️ Lesart: Die spätere, ausführlichere Entscheidung (`35#273E`) gilt für die Historie; was „außer sie ist tod“ ursprünglich meinte (Verdichtung erlaubt?), ist unklar. Verhalten in [Modul 03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md) N-ID-024 |
| C-N5 | **Einstufung der Simulationsebenen** | Welt-Dokumentation: ‚Pauschalantwort 12-35 ja‘ stützt keine Einzelentscheidung; Tier 0–3 sind 🧭/💡 (`W-TV-010`) | Diese Dokumentation: Das Prinzip (mehrere Ebenen, Verlustfreiheit, ereignisbasiert, Determinismus) ist als 📋 **Richtung** geführt; Stufennamen, Schwellen, Zahlen 🧭/💡 ([Modul 01](01-Quellenlage-und-Methodik.md) §3 Regel 7) | Unterschied der Einstufung, kein Inhaltsstreit. Der Entwickler kann das Prinzip bestätigen oder die Welt-Einstufung übernehmen lassen |
| C-N6 | **Bevölkerung: Einwanderer** | Entwickler: „es soll keine immigranten geben nur leute die von dort kommen oder aus der usa“ (`35#261E` Punkt 45) | Assistent widerspricht (`35#262A`): einzelne Einwanderer, internationale Studenten, Touristen, Geschäftsleute für Glaubwürdigkeit; Antwort „Ok“ (`35#263E`) | ⚠️ unklar, ob „Ok“ eine Annahme oder nur eine Kenntnisnahme ist; Touristen sind in jedem Fall beschlossen (`23#1089E` 167–173). Siehe N-ID-023 |
| C-N7 | **Lua im Modding** | Entwickler: „so wie beim minecraft spiel, nur ohne lua alles in c++“ (`23#52E` Punkt 18) | `M-PD` (31.08.): ‚Lua is optional only if it solves a real gap‘; Ebenen: Daten-, Unreal-/Blueprint-, native C++20-Mods | Kein Widerspruch im Kern (beide halten Lua für entbehrlich); die Aussage ist **zeitlich früher**. Betrifft die NPC-Modulebene nur am Rand (Modul 23) |
| C-N8 | **Ruhm des Spielers** | Entwickler: Medienbekanntheit durch große Ereignisse „kann passieren aber nicht oft“ (`23#1057E` Punkt 45) | Der Assistent erwähnt Fälle mit Medienberichten über Heldentaten (`23#603A` Abschnitt 27) und Social-Media-Verbreitung; öffentliche Bekanntheit erhöht die Verbreitung stark (`35#270A` Fragen 56–57) | kein Widerspruch: Selten, aber wirksam (Modul 07 §8). Offen ist die Häufigkeit |
| C-N9 | **Gewalt gegen Kinder und Minderjährige** | Kinder-NPCs sind möglich (`23#1056A` Frage 5); „nur bei kindern keine gore“ (`23#1059E`); harte Darstellungssperre für Minderjährige (`23#1060A`, Vorschlag) | Ob Kinder darüber hinaus **vollständig vor Spielergewalt geschützt** werden oder nur nicht grafisch dargestellt werden, soll nach dem Assistenten „später separat“ entschieden werden (`23#1060A`: ‚können wir später separat entscheiden‘); `M-OD`: ‚Exact minor-NPC violence presentation after ESRB/PEGI/USK/Sony/Microsoft/Steam/legal review‘ | ❓ offen (Q-N30) |

---

## 2. Unklare Antworten

| Nr. | Stelle | Problem | Behandlung |
|---|---|---|---|
| U-N1 | `35#271E` Punkt 53 („was ist gossip“) | Der Entwickler kannte das Wort nicht; der Assistent erklärte und empfahl „klar JA“ (`35#272A`); eine ausdrückliche Antwort fehlt; die Punkte 54–60, die den Klatsch voraussetzen, wurden mit „ja“ beantwortet | N-IN-007 als 🧭 geführt |
| U-N2 | `35#263E` („Ok“) | Reaktion auf den Widerspruch zu Punkt 45 (Einwanderer) | siehe C-N6 |
| U-N3 | `23#1057E` Punkt 104 („ja kann sein“) | Beerdigungen und Trauerveranstaltungen: unverbindlich | als „möglich“ geführt, nicht als Pflicht |
| U-N4 | `23#1089E` Punkt 10 („weiß nicht, als hunter eher schwierig“) | Familiengründung des Spielers | ❓ N-FA-010 |
| U-N5 | `35#281E` Punkt 14 („weiß noch nicht“) | Schwangerschaft und Familienplanung | ❓, verwiesen auf ein allgemeines Lebenssimulationssystem (`35#282A`) |
| U-N6 | `23#677E`/`23#679E` | Der Entwickler fragte nach der Bedeutung von „Radikalisierungssimulator“, antwortete danach „ich würde das sehr realistisch nachmachen“ ohne ausdrückliche Bestätigung der vom Assistenten gezogenen Grenze | Grenze (N-LT-029) als 🧭 geführt |
| U-N7 | `23#604E` („Sag mir nur das was wir nicht schon selber aufgestellt haben. Und bisschen genauer was gemeint ist“) | Reaktion auf die RDR2-Auswahl: keine Zustimmung zu Einzelpunkten | RDR2-Auswahl bleibt 💡 |
| U-N8 | `35#279E` Punkt 40 („ich habe das so gefunden: also ja eher kleiner“) | Der Entwickler gab eine Tabelle üblicher Savegrößen; ein Zielwert wurde nicht genannt | siehe C-N2 |
| U-N9 | `35#269E` Punkt 14 („ja aber es kann sein dass er sich nicht mehr genau erinnert“) | klare Antwort, aber Umfang der Ungenauigkeit offen | N-GE-011 |
| U-N10 | `35#261E` Punkte 24–26 | Antworten zu Anziehung bei Dates und Einmalbegegnungen betreffen das Adult-System | nicht hier geführt ([Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md)) |
| U-N11 | `23#1089E` Punkt 4 („man kann wenn man krass ist mehrere frauen gleichzeitig haben“) | Betrifft Mehrfachbeziehungen im Adult-/Beziehungssystem | verwiesen auf [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) und `23#1090A` |

---

## 3. Begriffskollisionen

Das Wort **‚Tier‘** (und die Buchstaben A–D) bezeichnet in den Quellen mehrere verschiedene Dinge. Diese Dokumentation hält sie getrennt und verwendet **Stufe** (Persistenz), **Simulationsstufe** und **Datensatztiefe**.

| Begriff | Bedeutung | Quelle | In diesem Ordner |
|---|---|---|---|
| Persistenz **A–D** (Tier A bis D) | Wichtigkeit der Bevölkerungsfigur | `23#1056A` Fragen 17, 121 | **Persistenzstufe** (Modul 03 §4) |
| Simulations-**Tier 0–3** | Genauigkeit der Berechnung | `35#278A` Frage 2 | **Simulationsstufe S0–S3** (Modul 22) |
| Datensatz: Minimal/Known/Detailed/Full | Tiefe der gespeicherten Daten | `35#262A` | **Datensatztiefe** (Modul 03 §5) |
| **Tier S** | Qualität der visuellen Figur (Story/Hero) | `35#296A` | gehört zum Aussehenssystem ([Decision 0005](../../Decisions/0005-character-appearance-foundation.md)); nicht hier |
| **A–D der Gebäude** | Hero-, Case-, normale Innenräume und Kulissen | `23#48A` Abschnitt 10; `23#51A` | gehört zur Weltdokumentation |
| ‚Rang‘ / ‚Rank‘ | Stellung in einer Gruppe | `23#676A` | **Rang** (Modul 16) |
| ‚Level‘ | Stufe der Fähigkeit, Gefahr | `M-PD` ‚Genre and progression identity‘ | **kein Level** (N-LT-026) |
| ‚Nemesis‘ | Arbeitsbegriff für wiederkehrende Gegner | `PE-N1022` C (Wort in Proposal 0010) | Kurzform; nicht für Menüs (Modul 11 §3) |
| ‚Faction‘ / Fraktion | Gruppen als Akteure | `23#676A`; `PE-N1022` D | **Fraktion** (Modul 16) |

---

## 4. Abweichungen zu Nachbardokumenten

| Nr. | Dokument | Abweichung | Behandlung |
|---|---|---|---|
| V-N1 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §2 | Das Proposal meldete keine Treffer zu ‚nemesis‘, ‚rivalry‘, ‚grudge‘, ‚living world‘ in Master-Tier 1 und 2; diese Auswertung bestätigt das für alle vier Tiers und **ergänzt** Treffer in den Gesprächen (‚Rivalen‘, ‚Rivalität‘: `19#4E`, `23#117A`, `23#164A`, `23#467A`, `23#676A`) und in Tier 1 (‚rival organizations‘, `CANONICAL_ADDENDUM_2026-09-05.md`) | Ergänzung, kein Widerspruch (Modul 01 §6, F8) |
| V-N2 | [Welt 20](../World/20-Entscheidungsregister.md) W-TV-010 | siehe C-N5 | – |
| V-N3 | [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §14 | Kreaturen verweisen auf offene Q-K15, Q-K25, Q-K33 | dieser Ordner führt sie als Q-N08, Q-N03/Q-N04, Q-N33 weiter (Abschnitt 6) |
| V-N4 | [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §1 Z12 | führt Persistenzstufen als „📋 Richtung“ | gleiche Einstufung (N-LT-016) |
| V-N5 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8 und [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13 | führen die Stufen A–D und Beziehungswerte | gleiche Inhalte; die NPC-Dokumentation ergänzt Tiefe. **Keine Änderung** an diesen Dateien; Ergänzungsvorschläge in [Modul 28](28-Abdeckungsprotokoll.md) §7 |
| V-N6 | [Spielvision](../Spielvision.md) §6.4 | Crew-Alltag als 📋 | N-CO-015 übernimmt; die Ausformulierung `23#603A` bleibt 💡 |
| V-N7 | [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §8 W8 | ‚Die Welt läuft während Haft und Zeitsprung weiter‘ | vereinbar mit N-LT-020 (Welt friert beim Beenden ein; Zeitsprünge werden nachgeholt) |
| V-N8 | [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §4 | enthält eine Setting-Entscheidung zur Orientierung | hier **nicht** übernommen oder bewertet; verweist nur auf die Quelle |

---

## 5. Offene Fragen

Die Fragen sind **nicht** entschieden. Reihenfolge: nach Gruppen, nicht nach Dringlichkeit (Dringlichkeit: Abschnitt 7).

### 5.1 Planung und Reihenfolge

| Nr. | Frage | Bezug |
|---|---|---|
| Q-N01 | In welcher Reihenfolge sollen die Ausbaustufen L1–L5 (Proposal 0010 §10) umgesetzt werden, und welche Teile gehören in den Vertical Slice? | Modul 02 §6, Modul 24 §8 |
| Q-N02 | Welche Mindest-Eingaben (Dialog, Ermittlung, Verletzung, Weltuhr) müssen existieren, bevor L1 beginnt? | N-KS-012 |

### 5.2 Daten, Gedächtnis, Beziehung

| Nr. | Frage | Bezug |
|---|---|---|
| Q-N03 | Welche Historie behält das Ereignisjournal (Verdichtungsregeln), und wie verhält sich das zur Journalgrenze von `WorldFacts`? | Modul 21 §4 |
| Q-N04 | Gibt es eine Obergrenze für Ereignisse je NPC und Zeitraum, und wie bleibt dabei alles Wichtige erhalten? | Modul 21 §5 |
| Q-N05 | Wie lange dauert ‚Jahrzehnte irrelevant‘ (Herabstufung), und gibt es eine Obergrenze für Detailed- und Full-Profile? | Modul 03 §5 |
| Q-N06 | Gedächtnis: Obergrenzen je Klasse, Verblassens-Raten, Schwelle für „relevant“ | Modul 05 |
| Q-N07 | Beziehung: Skalen, Gewichte, Hysterese, Verhaltensbänder | Modul 08 |
| Q-N13 | Haben NPCs Bedürfnis-Treiber (Needs) für die Planauswahl? | Modul 13 §5 |
| Q-N12 | Wie tief werden Alterskrankheiten (Demenz) modelliert? | Modul 04, 14 |
| Q-N11 | Sollen Einwanderer in der Bevölkerung vorkommen (C-N6)? | Modul 03 |
| Q-N20 | Sprachen und Mehrsprachigkeit der Bevölkerung | Modul 20 |
| Q-N27 | Dürfen sich NPCs, die nie ein Ereignis hatten, bei einem Generatorupdate ändern? | Modul 21 §10 |

### 5.3 Nemesis

| Nr. | Frage | Bezug |
|---|---|---|
| Q-N08 | Lernt ein Nemesis über Regeln, Daten oder eine Mischform; soll es handgeschrieben pro Wesen oder systemisch aus Daten sein? | Modul 11 §9 |
| Q-N09 | Welche Wesen und Menschen können Nemesis werden; wie viele gleichzeitig; welche Mindestabstände | Modul 11 §4, §11 |
| Q-N10 | Wie wird eine Niederlage oder der Tod des Spielers durch einen Nemesis behandelt? | Modul 11 §13; `M-PD` ‚Death / failure — OPEN‘ |
| Q-N19 | Wie lange dauern Erholung und Heilung (Nemesis, Crew, NPCs) in Weltzeit? | Modul 12 §4, Modul 14 §5 |
| Q-N31 | Was sieht der Spieler von einer Feindschaft (Archiv, Vorzeichen), und welche Gegenmaßnahmen soll es geben? | Modul 11 §11 |
| Q-N32 | Welche Regeln gelten für geschützte und handgeschriebene NPCs (Schutzstufen), und wer darf sie töten (C-N1)? | Modul 03 §8 |

### 5.4 Fraktionen und Gesellschaft

| Nr. | Frage | Bezug |
|---|---|---|
| Q-N14 | Wie wird der Aufstieg in Gruppen technisch bewertet? | Modul 16 §4.3 |
| Q-N16 | Wie viele Gerüchte sind je Region aktiv; werden sie als Weltobjekte oder je NPC geführt? | Modul 07 §11 |
| Q-N24 | Welche Gemeinschaftsvariablen gibt es (Angst, Misstrauen, Wirtschaft)? | Modul 15 §5 |
| Q-N23 | Wie viele Rekruten sind möglich; wie groß ist das HQ? | Modul 19 §7 |
| Q-N22 | Stirbt der Hund dauerhaft? | Modul 19 §4 |
| Q-N17 | Familie und Herkunft des Spielercharakters | Modul 09, 10 |
| Q-N18 | Altersverteilung und Bevölkerungsbilanz bei aktivierten Generationen | Modul 10 §7 |
| Q-N21 | Geschwindigkeit der Weltuhr; altern Spieler und Crew mit? | Modul 14 §3 |
| Q-N25 | Wie verhalten sich die Untergrundzivilisation und KI-Bewohner zum NPC-System? | Modul 01 §6 F5; Modul 10, 15, 17, 18 |
| Q-N29 | Zeitreise: Verhalten von NPC-Zuständen über Zeitstände | Modul 24 |
| Q-N30 | Gewalt, Schutz und Darstellung von Kinder-NPCs (C-N9) | Modul 10 |

### 5.5 Technik, Koop, Mods

| Nr. | Frage | Bezug |
|---|---|---|
| Q-N15 | Koop: Wie wird das Gedächtnis mehrerer Spieler gespeichert (Subjektmodell als Entwurf)? | Modul 23 §3 |
| Q-N26 | Koop: Teamentscheidungen im Datenmodell, Hostwechsel, KI-Begleiter | Modul 23 §4, §5 |
| Q-N28 | Hat das Offizielle Online eine NPC-Welt; welche Zeitlogik? | Modul 23 §9 |
| Q-N33 | Wo liegen die Mod-Erweiterungspunkte des NPC-Kerns? | Modul 23 §7; Proposal 0010 §11 |
| Q-N34 | Welche technischen Bausteine (StateTree, Datenorientierte Entitäten, eigene Kerne) sind für S0–S3 geeignet; welche Budgets? | Modul 22 §6, §9 |

---

## 6. Beziehung zu bestehenden Fragenlisten

| Frage hier | Verwandte Fragen anderer Ordner |
|---|---|
| Q-N03, Q-N04 | `Q-K25` (Kreaturen), `Q-E67`, `Q-E71` (Ermittlung): Speicherlinie und Verdichtung |
| Q-N08 | `Q-K15` (Kreaturen): Lernen pro Individuum, Art oder Gruppe |
| Q-N32, C-N1 | `Q-K33` (Kreaturen): geschützte NPCs |
| Q-N25 | Fragen zur Untergrundzivilisation in [Welt 22](../World/22-Offene-Fragen-Katalog.md) |
| Q-N28, Q-N15, Q-N26 | Ermittlung 18 (Koop), `K-E9` |
| C-N2 | Welt `K13` |
| Q-N34 | `M-OQ16` Punkt 9; Welt 18 §14 |

Die Zuordnungen sind Hinweise, keine Identitäten: Die Fragen anderer Ordner sind nicht automatisch beantwortet, wenn die hiesige beantwortet wird.

---

## 7. Vorschlag zur Reihenfolge der Klärung

> 💡 **Vorschlag**, kein Beschluss. Er ordnet nach dem Nutzen für die ersten Umsetzungsschritte (Proposal 0010 L1 bis L3), nicht nach der Wichtigkeit für das Gesamtspiel.

1. **C-N1 / Q-N32:** Wer darf wichtige NPCs töten? Davon hängen Schutzstufen, Fraktions- und Nemesis-Regeln ab.
2. **Q-N03 / Q-N04 / C-N2:** Verdichtung des Journals und Größenziel. Davon hängt das Speicherkonzept ab (Modul 21).
3. **Q-N01 / Q-N02:** Reihenfolge der Umsetzung und Mindest-Eingaben. Davon hängt ab, was zuerst entsteht.
4. **Q-N08 / Q-N09 / Q-N10:** Nemesis-Lernmodell, Eignung, Häufigkeit, Spielerniederlage.
5. **Q-N15 / Q-N26:** Koop-Subjektmodell und Teamentscheidungen.
6. **Q-N16 / Q-N24:** Gerüchte und Gemeinschaftsvariablen als Grundlage der lebenden Gesellschaft.
7. **Q-N14 / Q-N23 / Q-N17:** Fraktionsaufstieg, Rekruten, Spielerherkunft.
8. **Q-N30:** Kinder-NPCs (Rechtsfrage, früh klären, weil sie Datenmodell und Darstellung berührt).
9. **Q-N34 und Q-N05–Q-N07:** Messungen am Prototyp, danach Zahlen.
10. **Rest:** Alterskrankheiten, Sprachen, Einwanderer, Online, Mods.
