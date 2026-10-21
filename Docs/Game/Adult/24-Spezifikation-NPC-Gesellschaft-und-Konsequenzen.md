# Adult 24: Spezifikation NPC, Gesellschaft und Konsequenzen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register](14-Entscheidungsregister.md)). Kein Kanon. Gewalt- und Straftatsthemen werden **nur als Konsequenzebene** beschrieben (kein Interaktionsmodus, keine Ausführungsmechanik), siehe [Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachlich: [Modul 05](05-NPCs-Beziehungen-Praeferenzen.md), [Modul 08](08-Welt-Orte-Branche-Wirtschaft.md). Daten: [Modul 21](21-Spezifikation-Datenmodell.md). Regelkern: [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [NPC-Identität und Erzeugung](#3-npc-identität-und-erzeugung)
4. [Persönlichkeit, Stimmung, Attraktion](#4-persönlichkeit-stimmung-attraktion)
5. [Präferenzen und Lernen](#5-präferenzen-und-lernen)
6. [Beziehungen](#6-beziehungen)
7. [Initiative und Verabredung](#7-initiative-und-verabredung)
8. [Gedächtnis](#8-gedächtnis)
9. [Simulation vieler NPCs](#9-simulation-vieler-npcs)
10. [Gesellschaft: Zeugen, Gerüchte, Ruf](#10-gesellschaft-zeugen-gerüchte-ruf)
11. [Recht und Konsequenzen](#11-recht-und-konsequenzen)
12. [Fraktionen, Moral, Kultur](#12-fraktionen-moral-kultur)
13. [Dienstleistung und Verträge](#13-dienstleistung-und-verträge)
14. [Technische Komponenten](#14-technische-komponenten)
15. [Randfälle](#15-randfälle)
16. [Tests und Abnahmekriterien](#16-tests-und-abnahmekriterien)
17. [Ungeklärtes](#17-ungeklärtes)

---

## 1. Ziel und Abgrenzung

NPCs sind **Menschen mit eigenem Leben** (E-TA-026, E-NP-030): Sie haben Persönlichkeit, Grenzen, Gedächtnis, Beziehungen, Alltagsabläufe und verändern sich über Jahre unabhängig vom Spieler. Das Adult-Paket nutzt dieselben NPC-Systeme; es gibt keinen separaten „Adult-NPC-Typ“ (E-NP-028 / R15H 200).

Dieses Modul beschreibt, **wie sich NPCs zueinander und zum Spieler verhalten** und wie die Welt auf Ereignisse reagiert. Es definiert keine Handlung und keine Inhalte.

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Jede erwachsene wichtige Figur hat eigene Präferenzen und Grenzen; Frauen/Figuren sind nicht pauschal bereit | 🧭 / 📋 | E-NP-001 |
| Präferenz-DNA aus Seed, an globale NPC-ID gekoppelt, nur bei Erzeugung | 📋 | E-NP-012 |
| Vorlieben ändern sich durch Beziehung und Erfahrung | 📋 | E-NP-002 |
| Überreden verschiebt Neutral/Situativ, nicht ein aktuelles Hard Limit | 🧭 | E-NP-006 |
| Beziehungssystem breit: parallele Beziehungen, Eifersucht, Geheimnisse, offene Arrangements, Trennung | 📋 | CA05 |
| NPCs initiieren, flirten, schlagen Orte vor, melden sich später | 📋 | E-NP-011 |
| Gedächtnis: Stufen, Anker, Vergessen, Zusammenfassung ersetzt keine Einzelerinnerung | 📋 / 🧭 | E-LE-002, E-LE-008 |
| Welt logisch statt emotional (Tat vor Identität, Kontext, Verhältnismäßigkeit, Gerüchte ≠ Fakten) | 📋 | E-WE-015 |
| Strafsystem realistisch (Tat → Verfahren → Haft); Gefängnis als soziale Simulation | 📋 | E-WE-014 |
| Spielerbegangene Sexualstraftaten als Weltkonsequenz | 🧭 / ⚠ vorläufig | E-WE-013, [K3](19-Konflikt-und-Widerspruchsmatrix.md#k3-spielerbegangene-sexualstraftaten) |
| Orientierung: Setting-Wunsch vs. datengetriebenes Modell | ❓ | [K5](19-Konflikt-und-Widerspruchsmatrix.md#k5-orientierung) |
| Zuwanderung in der Bevölkerung | ❓ | [K12](19-Konflikt-und-Widerspruchsmatrix.md#k12-zuwanderung) |
| Kündigung des Langzeitvertrags | 🧭 | [K6](19-Konflikt-und-Widerspruchsmatrix.md#k6-langzeitvertrag) |

## 3. NPC-Identität und Erzeugung

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-ID-01 | Globale NPC-ID, stabil über Speicherstände, Zeit und Simulationsstufen. | E-LE-002 |
| NP-ID-02 | **Archetypen + Zufallsvariation** statt 500.000 handgeschriebener Profile; handkuratierte Profile nur für wichtige Figuren (Tier S) und Mod-unterstützte. | E-NP-005 |
| NP-ID-03 | Archetyp wirkt nur als **Bias**; ein Plausibilitätscheck verhindert unsinnige Kombinationen. | 35#260 (R9A 7–9) |
| NP-ID-04 | Hierarchie: Persönlichkeitskern > Sozial > Attraktion > Adult-Präferenz > situative Überschreibungen. | 35#260 (R9A 5) |
| NP-ID-05 | Regionale Variation nur als Bias (Stimme, Aussehen leicht anders). | E-NP-014 |
| NP-ID-06 | Zuwanderung/Touristen: ❓ ungeklärt ([K12](19-Konflikt-und-Widerspruchsmatrix.md#k12-zuwanderung)). | E-NP-014 |
| NP-ID-07 | Neugeborene erhalten eigene ID; Genetik aus Eltern; Wachstumskurve; Gesicht altert. | E-KO-012 |
| NP-ID-08 | Minderjährige sind für das Adult-System unsichtbar. | E-KO-012, E-SK-032 |
| NP-ID-09 | NPCs steigen/fallen zwischen Fidelity-Stufen ohne Neuwürfeln (Beförderung). | E-KO-011 |
| NP-ID-10 | Tod: Daten und wichtige Geschichte bleiben. | E-SK-014 |

## 4. Persönlichkeit, Stimmung, Attraktion

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-PS-01 | Persönlichkeitsdimensionen kontinuierlich; kurzfristige Stimmung und Stress getrennt von Kernpersönlichkeit. | 35#260 (R9A 10–20) |
| NP-PS-02 | Attraktion mehrkomponentig (Aussehen, Stimme, Auftreten, Stil, Hygiene, Reputation …); **Aussehen reicht bei manchen als erster Trigger**. | E-NP-011, 35#230 |
| NP-PS-03 | Getrennte Werte: Attraktion, Vertrauen, Zuneigung, Romantik, Komfort, Vertrautheit. | E-NP-007 |
| NP-PS-04 | **Chemie** bidirektional, positiv/negativ. | E-NP-007 |
| NP-PS-05 | Initiale Attraktion wirkt vor allem bei Dates; bei Einmalbegegnungen eher weniger (Assistent: auch dort beibehalten). | E-NP-013 |
| NP-PS-06 | Erregung ≠ Attraktion ≠ Zustimmung. | E-SK-030 |
| NP-PS-07 | Libido und gewünschte Intensität sind nicht konstant (Zeit, Stress, Stimmung, Hormone); es gibt keinen universellen Cooldown. | E-NP-023, E-NP-028 |
| NP-PS-08 | Hormonelle Verhütung kann Stimmung/Libido beeinflussen (Stärke Vorschlag). | E-GE-003 |
| NP-PS-09 | „Typ“ ist unscharf; Persönlichkeit kann Optik ausgleichen (und umgekehrt). | E-NP-028 |
| NP-PS-10 | Orientierung als Datenmodell: ❓ (K5). | E-NP-025 |

## 5. Präferenzen und Lernen

### 5.1 Präferenzprofil

Siehe [Modul 21, Abschnitt 4](21-Spezifikation-Datenmodell.md#4-präferenzmodell). Stufenskala: neugierig, offen, neutral, situativ, ungern, weiche Grenze, **harte Grenze** (E-NP-004). Das Profil ist **partner- und situationsabhängig** (E-NP-019/020).

### 5.2 Lernen

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-PL-01 | Der Spieler lernt Präferenzen **über Verhalten und Gespräch**, nicht über Menüs; Spieler sieht das Profil nie (außer Entwicklung). | E-NP-004, E-NP-021 |
| NP-PL-02 | Der NPC lernt den Spieler (Partnerprofil) mit **Konfidenz** und kann sich irren („falsches Lernen“). | E-NP-029 |
| NP-PL-03 | Wissen hat Stufen: unbekannt, vermutet, direkt erzählt, gemeinsam erlebt. | E-NP-019 |
| NP-PL-04 | Spontan Entdecktes kann neue Präferenzen erzeugen; Gewöhnung ohne Magie. | E-SK-024, E-SO-034 |
| NP-PL-05 | Gelernte Präferenz ersetzt **nie** aktuelle Zustimmung. | E-NP-006, [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) |
| NP-PL-06 | Vorlieben können verborgen und später offenbart werden. | E-NP-028 |
| NP-PL-07 | Eine Figur kann sich für einen Partner verändern (optional) oder dies ablehnen. | E-NP-028 |
| NP-PL-08 | Fantasien müssen nie umgesetzt werden (Fantasie vs. Aktivität getrennt). | E-IA-038 |

## 6. Beziehungen

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-BZ-01 | Beziehungsformen: Bekanntschaft, Date, Beziehung, offene Beziehung/Poly, Ehe/Zusammenleben, getrennt; pro Beziehung individuell. | CA05, E-NP-007, E-NP-022 |
| NP-BZ-02 | Eifersucht auch bei Zustimmung möglich; Geheimnisse, Fremdgehen verschlechtern die Beziehung; Trennung als Prozess. | E-NP-007, CA05 |
| NP-BZ-03 | Eine Einmalbegegnung kann einmalig bleiben oder zu einer Beziehung werden. | E-NP-018 |
| NP-BZ-04 | Ein gutes Date führt **nicht zwingend** zu Adult. | E-NP-017 |
| NP-BZ-05 | Gruppen: paarweise Beziehungen bleiben; Beteiligungsgrade; Eifersucht trotz Zustimmung. | E-NP-022 |
| NP-BZ-06 | Gemeinsame Erlebnisse können seltene Wendungen erzeugen, **nicht** in der Hauptstory. | E-NP-010 |
| NP-BZ-07 | Beziehung ersetzt Zustimmung nie. | E-SK-001 |
| NP-BZ-08 | Die Zahl der Beziehungen/Zusammenlebens mit mehreren ist begrenzt durch Zeit/Alltag der Figuren (Alltag ist Teil der Simulation). | CA05 (Scope) |

## 7. Initiative und Verabredung

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-IV-01 | NPCs flirten, laden ein, schlagen Orte vor, melden sich später. | E-NP-011 |
| NP-IV-02 | Spontane Begegnungen sind Kernziel (Bar, Club, Straße). | E-NP-011 |
| NP-IV-03 | Typen: nur Casual / nur Beziehung / Mittelding; Faktorenmix statt eines Schalters. | E-NP-011 |
| NP-IV-04 | Mehrere Interessenten gleichzeitig; Kontaktdaten/Nummern; Kontakt später. | E-NP-011 |
| NP-IV-05 | Ortswechsel (Date → Zuhause) als Alltagsablauf mit Übergang. | E-IA-013 |
| NP-IV-06 | Initiative ist von Dominanz getrennt. | E-NP-019 |
| NP-IV-07 | NPC kann ablehnen; **ein aktuelles Nein bleibt Nein** (nicht als Spiel umdeutbar). | E-NP-009 (überholt) |

## 8. Gedächtnis

Aus [Modul 21, Abschnitt 5](21-Spezifikation-Datenmodell.md#5-beziehungs--und-gedächtnisdaten). Ergänzend:

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-GD-01 | Gedächtnisklassen: permanent, lang, kurz, verwerfbar. | E-LE-002 |
| NP-GD-02 | Ankererinnerungen für außergewöhnlich gute oder schlechte Erlebnisse. | E-NP-016 |
| NP-GD-03 | Vergessen: unscharfe Erinnerung (Details verblassen) – ein Entwickler-Wunsch („ungenau erinnern“). | 35#268/14 |
| NP-GD-04 | Ein Widerruf oder eine Grenzverletzung wirkt in der Erinnerung nach und beeinflusst Vertrauen und Reaktion. | [Modul 01](01-Grundregeln-Alter-Zustimmung.md) |
| NP-GD-05 | Erinnerungen aktualisieren Beziehungswerte nicht pauschal (+50), sondern erzählerisch/kontextabhängig (28#25). | 28#25 |
| NP-GD-06 | Erlebnisse fließen in Beziehung und Vertrauen ein (gut/schlecht). | E-IA-020 |
| NP-GD-07 | Gerüchte und Gossip sind eigene Informationsflüsse mit Wahrheitsgehalt. | E-NP-016, E-WE-015 |

## 9. Simulation vieler NPCs

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-SM-01 | Simulationsstufen: Held/lokal/regional/ruhend; Tier 3 nur Ereignisschritte. | E-LE-002 |
| NP-SM-02 | Zufall deterministisch aus NPC-ID + Weltseed + Zeitfenster. | E-LE-002 |
| NP-SM-03 | Eine relevante Begegnung friert den NPC-Zustand ein („materialisiert“). | E-LE-002 |
| NP-SM-04 | Entfernte NPCs entwickeln sich weiter (Friseur, Tattoo, Altern). | E-LE-005 |
| NP-SM-05 | Die Welt läuft im Singleplayer nur, solange gespielt wird; Online anders. | E-LE-001 |
| NP-SM-06 | NPC↔NPC-Interaktionen sind möglich; Beobachtung möglich; **dieselbe Prüfkette**. | E-IA-002 |
| NP-SM-07 | Größenordnungen (500.000 NPCs; ~5.000 regional; ~100–300 materialisiert) sind Vorschläge „zu messen“. | E-LE-006 |

## 10. Gesellschaft: Zeugen, Gerüchte, Ruf

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-GS-01 | Öffentlichkeit hat Sicht- und Hörweite echter NPCs; Reaktion nach Persönlichkeit, Beziehung, Fraktion, Recht. | E-WE-005, E-WE-021 |
| NP-GS-02 | Zeugen: Sichtlinie, Distanz, Raum, Türen, Wände, Audio, Licht, Sichtbarkeit, Privatsphäre. | E-WE-021 |
| NP-GS-03 | Ruf ist **mehrdimensional** (Beruf, Szene, Freundeskreis, Familie, Behörden, Fans). | E-WE-007 |
| NP-GS-04 | Folgen für Spieler und NPCs: Reputation, Beziehung, Arbeit, Wohnort. | E-WE-012 |
| NP-GS-05 | Medienverbreitung (Handys, Kameras) ist eine Quelle für Ruf und Recht. | 35#527 (R15C) |
| NP-GS-06 | Aufnahme und Veröffentlichung getrennte Zustimmungen. | E-WE-011 |
| NP-GS-07 | Gerüchte ≠ Fakten. | E-WE-015 |

## 11. Recht und Konsequenzen

### 11.1 Grundsatz

📋 E-WE-014/E-WE-015: Das Strafsystem folgt einer **realistischen Kette**: Tat → Schweregrad → Vorsatz → Opfer → Zeugen/Beweise → Notwehr → Vorstrafen → Festnahme → Staatsanwaltschaft → Prozess → Urteil → Haft. Es gibt keine „künstliche Liste von Verbrechen“; Tat-Typen ergeben sich aus Weltregeln. Das Gefängnis ist eine soziale Simulation. Selbstverteidigung ist geschlechtsunabhängig. Prinzip: **Tat vor Identität**.

### 11.2 Sexualstraftaten (⚠ K3, nur Konsequenzebene)

| Nr. | Aussage | Status | Basis |
|---|---|---|---|
| NP-RK-01 | Solche Straftaten können in der Welt vorkommen (durch NPCs, auch durch Spieler). | 🧭 vorläufig | E-WE-013 |
| NP-RK-02 | Modelliert werden **nur Konsequenzen**: Reaktion von Opfer/Zeugen, Spuren, Erinnerungen, Anzeige, Polizei, Fahndung, Verfahren, Haft, Ruf. | 🧭 | E-WE-013 |
| NP-RK-03 | Es gibt **keine Handlungsmechanik**, keine Darstellung als erotisch, keine Bewerbung, keinen Menüpunkt und kein Tutorial. | 🧭 | 35#531/533 |
| NP-RK-04 | Der Sicherheitskern bleibt unverändert; „technisch möglich“ ist nie „einverstanden“. | 📋 | E-SK-034 |
| NP-RK-05 | Offene Frage, ob und wie solche Straftaten für Spielerfiguren überhaupt als Weltereignis dargestellt werden dürfen (Rating, Plattform, Haltung des Entwicklers). | ❓ | [K3](19-Konflikt-und-Widerspruchsmatrix.md#k3-spielerbegangene-sexualstraftaten), [Modul 10](10-Produktion-Recht-Vertrieb.md) |
| NP-RK-06 | Ein Testpfad für das Zusammenspiel Justiz/Beweise/Polizei **ohne sexuelle Handlungsmechanik** ist eine Möglichkeit, aber nicht beschlossen. | ❓ | [K2](19-Konflikt-und-Widerspruchsmatrix.md#k2-user-lock-vom-2009) |

### 11.3 Rollenspiel-Produktionen

Gespielte Nichteinwilligung in Produktionen (K4) ist **nicht freigegeben**; sie wäre nur als vereinbartes Rollenspiel zwischen Erwachsenen mit gespeicherter Vereinbarung und getrenntem Weltzustand denkbar (E-IA-027) und braucht eine eigene Spezifikation und Rechtsprüfung.

### 11.4 Rechtslage in der Welt

- Jurisdiktionen der fiktiven Welt bestimmen, welche Betriebe legal sind (E-WE-004).
- Öffentliches Handeln hat Rechtsfolgen; Wohnung, legale Venues und öffentliche Orte unterscheiden sich (35#527).
- Aufnahmen/Veröffentlichungen können strafbar sein.
- Alters- und Identitätsprüfung ist in Verträgen zwingend (E-NP-024).

## 12. Fraktionen, Moral, Kultur

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| NP-FM-01 | Kultur und Moral als **Daten** pro Fraktion/Region (Mehrheitskultur der fiktiven Region kann traditionell/konservativ sein), nicht als feste Regel. | E-WE-014, 35#533–537 |
| NP-FM-02 | Reaktionen auf Verstöße werden aus Fraktions-/Moral-/Aggressionssystem abgeleitet. | E-WE-014 |
| NP-FM-03 | Gewalt gegen Menschen aufgrund ihrer Orientierung ist **keine** gewünschte Weltregel. | E-NP-026 |
| NP-FM-04 | Extremistische Gruppen mit Symbolik: nur an konkrete fiktive Fraktionen gebunden; Zulässigkeit ungeprüft. | [K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik) |
| NP-FM-05 | Religion/Beschneidung/Weltzusammensetzung: Setting-Entscheidung offen. | [K14](19-Konflikt-und-Widerspruchsmatrix.md#k14-beschneidung-und-religion) |

## 13. Dienstleistung und Verträge

Fachlich: [Modul 08](08-Welt-Orte-Branche-Wirtschaft.md). Technisch (🧭):

| Nr. | Anforderung | Basis |
|---|---|---|
| NP-DV-01 | Berufe im Nachtleben sind normale NPC-Berufe; **Beruf ≠ private Verfügbarkeit**; nicht jeder bietet jeden Service. | K§19, E-WE-009 |
| NP-DV-02 | Dienstleistungs-NPCs haben Namen, Zeitplan, Persönlichkeit, Grenzen, Präferenzen, Preise, Erinnerungen; „bezahlt“ ≠ „alles erlaubt“. | K§19, 35#539 |
| NP-DV-03 | Preise marktbasiert; Geld ersetzt keine Attraktion; Premium/VIP; Konkurrenz; Trends. | 35#539 |
| NP-DV-04 | Transaktionen laufen über den allgemeinen Service-Transaktionsrahmen (Reservierung → Abschluss). | 23#570 |
| NP-DV-05 | Zustimmung bleibt von Bezahlung unabhängig; harte Grenzen gelten; Abbruch ist jederzeit möglich. | E-SK-003 |
| NP-DV-06 | Langzeitverträge (Begleitung): Leistungen, Vergütung, optional Unterkunft/Verpflegung, **Kündigungsrechte beider Seiten**; kein „Eigentum“; Alters-/Identitätsprüfung zwingend. | E-NP-024, 35#543 |
| NP-DV-07 | Eigenkündigung des NPC ist technisch immer möglich und folgt Bleibemotivation (Bezahlung, Beziehung, Lebensqualität, Sicherheit, Vertrauen) minus Konflikte, schlechte Behandlung, Alternativen. Die Wahrscheinlichkeit „extrem niedrig“ ist ein **Wunsch**, keine Zahl. | 35#545/547/549, K6 |
| NP-DV-08 | Der Begriff „Sklavin“ aus den Quellen wird **nicht** übernommen. | 35#540 |

## 14. Technische Komponenten

🧭:

| Komponente | Aufgabe |
|---|---|
| `NpcGenerator` (offline + Laufzeit) | Archetyp + Seed + Plausibilität |
| `PreferenceEngine` | Definition, Zustand, Lernen |
| `AttractionModel` | Komponenten, Chemie |
| `RelationshipGraph` | Beziehungen, Status |
| `MemoryStore` | Stufen, Konsolidierung, Anker |
| `InitiativePlanner` | Flirt, Einladung, Kontakt |
| `WitnessModel` | Sicht, Gehör, Aufmerksamkeit |
| `ReputationLedger` | Mehrdimensionaler Ruf |
| `JusticePipeline` | Tat → Haft (gemeinsam mit allgemeinem Kriminalitätssystem) |
| `ContractService` | Verträge, Kündigung |
| `PopulationSimulator` | Tiers, Ereignisse, Seed |

## 15. Randfälle

[Modul 31](31-Randfaelle-Katalog.md#5-npc-und-gesellschaft): Zwei NPCs mit Interesse am selben Partner; NPC mit Gedächtnislücke; Partnerprofil falsch gelernt; NPC wechselt Tier während eines Gesprächs; Zeuge ohne Sicht, aber Gehör; Vertrag mit Person, deren Alter nicht verifizierbar ist; Verstorbener Partner mit offenem Vertrag; Gerücht verbreitet sich in Gebieten mit getrennter Simulationsstufe; Beziehungsfaktoren kollidieren (Eifersucht + Zustimmung).

## 16. Tests und Abnahmekriterien

| Test | Erwartung |
|---|---|
| Generierung 100.000 NPCs | Verteilungen plausibel, deterministisch aus Seed |
| Präferenz-Hard-Limit unter wiederholtem Überreden | bleibt bestehen |
| Gedächtnis: Widerruf | Beziehung/Vertrauen verändert sich nachhaltig |
| Zeugen-Sichtlinie | Entscheidungen konsistent mit Geometrie/Audio |
| Vertrag: Kündigungsrecht beider Seiten | beide Seiten können beenden; keine Zwangsflags |
| Alterungs- und Todesfälle | Daten bleiben, Interaktion gesperrt |
| Rechtskette | Reihenfolge der Stufen unverletzt; keine Handlungsmechanik in den Daten |
| Population über 10 Spieljahre | keine Stabilitätsprobleme, Speichergröße im Budget |

[Modul 29](29-QA-Testkatalog.md#5-npc-und-gesellschaft), [Modul 30](30-Abnahmekriterien.md#5-npc-und-gesellschaft).

## 17. Ungeklärtes

- [K5](19-Konflikt-und-Widerspruchsmatrix.md#k5-orientierung), [K12](19-Konflikt-und-Widerspruchsmatrix.md#k12-zuwanderung), [K14](19-Konflikt-und-Widerspruchsmatrix.md#k14-beschneidung-und-religion), [K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik).
- [K3](19-Konflikt-und-Widerspruchsmatrix.md#k3-spielerbegangene-sexualstraftaten) und [K4](19-Konflikt-und-Widerspruchsmatrix.md#k4-gespielte-nichteinwilligung).
- Zahlen: Anteil der Gelegenheitsbereitschaft, Häufigkeit von Eigenkündigungen, Gerüchtegeschwindigkeit.
- Datenschutz der Gedächtnisse im Mehrspielermodus.
- Wie viele Beziehungen gleichzeitig sinnvoll sind (Balance).
