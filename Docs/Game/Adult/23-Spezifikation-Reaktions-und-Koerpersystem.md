# Adult 23: Spezifikation Reaktions- und Körpersystem

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register](14-Entscheidungsregister.md)). Kein Kanon. Explizite Einzelheiten der Quellen werden **nur abstrakt** als Zustandsfamilien beschrieben; Detailtiefe und Darstellung sind Entscheidungen des Entwicklers. Es werden keine Ausführungsanleitungen für gefährliche Handlungen entworfen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachlich: [Modul 06](06-Koerper-Physik-Gesundheit.md). Engine: [Modul 22](22-Spezifikation-Interaktionsengine.md). Daten: [Modul 21, Abschnitt 8](21-Spezifikation-Datenmodell.md#8-körper--und-reaktionszustand).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Körpermodell](#3-körpermodell)
4. [Weichgewebe und Sekundärbewegung](#4-weichgewebe-und-sekundärbewegung)
5. [Physiologische Zustände](#5-physiologische-zustände)
6. [Reaktionsgraph](#6-reaktionsgraph)
7. [Stimme, Mimik, Blick, Atmung](#7-stimme-mimik-blick-atmung)
8. [Körperliche Grenzreaktionen und Inhaltsschalter](#8-körperliche-grenzreaktionen-und-inhaltsschalter)
9. [Risiko, Verletzung, Heilung](#9-risiko-verletzung-heilung)
10. [Schwangerschaft, Verhütung, Gesundheitswissen](#10-schwangerschaft-verhütung-gesundheitswissen)
11. [Flüssigkeiten und Aufräumen (Spätes Feature)](#11-flüssigkeiten-und-aufräumen-spätes-feature)
12. [Technische Komponenten](#12-technische-komponenten)
13. [Randfälle](#13-randfälle)
14. [Tests und Abnahmekriterien](#14-tests-und-abnahmekriterien)
15. [Ungeklärtes](#15-ungeklärtes)

---

## 1. Ziel und Abgrenzung

Es gibt **ein** hochwertiges Menschensystem für das ganze Spiel (E-SO-005). Dieses Modul beschreibt, wie Körperzustand, Physiologie und Reaktionen **zusammenhängen** und wie die Adult-Erweiterung sie nutzt. Der Entwickler: „Das Spiel simuliert die Welt, es gibt keine Cuts“ und ‚jede Person nicht nur aus drei Elementen, realistisch nachgebaut‘ (35#283, 35#486). Reaktionen sind **keine Animationen**, sondern das Ergebnis des Zusammenspiels von Handlung, Körper, Person und Situation (E-KO-022).

Nicht Gegenstand: Zustimmung ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)), Aktionsablauf ([Modul 22](22-Spezifikation-Interaktionsengine.md)).

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Ein Körpersystem für alles; Adult nutzt exakt dasselbe | 📋 | E-SO-005, E-SO-006 |
| Kontinuierliche Körperparameter, Asymmetrie, nie komplett gleich | 📋 | E-KO-001, E-SO-001 |
| Körper verändert sich über Gewicht, Training, Alter, Schwangerschaft, Eingriffe | 📋 | E-SO-027, E-GE-012 |
| 90 % Realismus durch Approximation reicht | 📋 | E-SO-004 |
| Reaktionen als eigene, kombinierbare Zustände; Reaction Graph | 📋 | E-KO-022, E-KO-023 |
| Content-Toggle für starke Körperreaktionen | 📋 | E-ST-011, E-KO-022 |
| Verletzungssystem bleibt aktiv, auch bei gewolltem Risiko | 📋 | E-GE-001 |
| Kein `usageCount`; Zustand ändert sich nur durch reale Faktoren | 🧭 | E-KO-016 |
| Kein detailliertes STI-System | 📋 | E-GE-004 |
| Flüssigkeitssystem ist Spät-Feature | 📋 | E-IA-033 |
| Detailtiefe extremer Reaktionen | ❓ | [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md) |
| Welche Reaktionsfamilien im ersten Release | ❓ | E-PR-012 |

## 3. Körpermodell

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| KS-KM-01 | Körper = Parameterraum aus kontinuierlichen Größen (Knochenbau, Proportionen, Muskel, Fettverteilung, Haltung) plus Seed; keine festen Stufen. | E-KO-001, E-KO-011 |
| KS-KM-02 | Knochenbau, Muskel und Fett sind getrennt; Asymmetrie links/rechts ist Standard. | E-KO-013 |
| KS-KM-03 | Hüft-/Beckenform ergibt sich aus Knochen und Fettverteilung, nicht aus einem Schieber. | E-KO-013 |
| KS-KM-04 | Brust- und Gesäßform sind kontinuierlich, aus Gewebe/Muskel/Knochen abgeleitet, veränderlich mit Gewicht, Training, Schwangerschaft, Alter. | E-KO-014 |
| KS-KM-05 | Body Composition ist ein **Adaptationsmodell** mit vielen Zuständen, nicht Gewicht/Muskel-Schieber (K§20). | K§20 |
| KS-KM-06 | Intimanatomie wird wie jedes Körpermerkmal behandelt (kontinuierlich, asymmetrisch, mit plausiblen Grenzbereichen), erweitert im **selben** Editor. | E-KO-015 |
| KS-KM-07 | Zustandsänderungen der Anatomie nur über physiologische Ursachen (Schwangerschaft, Alter, Verletzung, Eingriff, Heilung); kein Nutzungszähler. | E-KO-016 |
| KS-KM-08 | Haare/Bart wachsen kontinuierlich; Grooming regional; Hygiene und Geruch sind Zustände. | E-KO-003, E-KO-018 |
| KS-KM-09 | Haut: Poren, Muttermale, Sommersprossen, Narbenmaterial, Akne dynamisch (Gesicht/Haut-Block). | 35#499 |
| KS-KM-10 | Attraktion wird auf dem **tatsächlichen** Körper berechnet. | E-KO-005 |
| KS-KM-11 | Der Spieler wählt am Spielbeginn Körper und Aussehen; wichtige Figuren sind Tier S, Population wird generiert; Beförderung ohne Neuwürfeln. | E-KO-011 |
| KS-KM-12 | Beschneidung und ähnliche Merkmale sind **Daten- und Setting-Entscheidungen**, hier ❓. | E-KO-017 |

## 4. Weichgewebe und Sekundärbewegung

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| KS-WG-01 | Weichgewebe ist **sekundär zum Skelett**; Auslöser sind Beschleunigung, Kontakt, Druck. | E-SO-013 |
| KS-WG-02 | Parameter je Körperzone: Masse, Steifigkeit, Dämpfung, Trägheit, Anbindung, Kollision, Maximalauslenkung (28#25/23#589), links/rechts getrennt. | E-SO-026 |
| KS-WG-03 | Kontakt dämpft; Muskeltonus wirkt; Kleidung (z. B. Sport-BH) verändert die Mechanik. | E-SO-026, E-KO-014 |
| KS-WG-04 | Sekundärbewegung ist **pro Körperteil** abhängig von Anatomie und Zustand, nicht von der Größe allein. | E-SO-024 |
| KS-WG-05 | **Realistisch, nicht übertrieben**; dieselbe Simulation im Alltag (Laufen, Training) und in der Intimität. | E-SO-006 |
| KS-WG-06 | Weichgewebe-Profile gehören zur Körperzusammensetzung; Veränderungen (Gewicht, Schwangerschaft) verändern die Parameter. | E-KO-010, E-SO-027 |
| KS-WG-07 | Qualitätsstufen ändern Genauigkeit, nicht Verhalten. | E-SO-014 |
| KS-WG-08 | Verformung ist poseabhängig, mit Korrekturformen (Corrective Morphs). | 35#283 |
| KS-WG-09 | Druckverteilung zwischen Körper, Kleidung und Fläche (Haut, Stoff, Matratze). | E-SO-028 |

Offen: Solver (analytisch, Mass-Spring, XPBD, vorberechnet?), Budgets, Netzwerkregel (‚Soft-Tissue clientseitig‘ laut E-SP-005).

## 5. Physiologische Zustände

🧭 Eine gemeinsame **Ergebnisschicht** (Unified Outcome Layer, E-KO-024) über dem Körpersystem; auch Kampf, Sport, Medizin lesen sie.

| Zustand | Beschreibung | Basis |
|---|---|---|
| Erregung (Arousal) | kontinuierlich, verborgen, kann sinken, parallel zu Stress, körperlich ≠ emotional; **kein Zustimmungssignal** | E-IA-019, E-SK-030 |
| Lokale Empfindung | Karte pro Region; Schmerz und Lust können gleichzeitig bestehen; Sensibilisierung/Desensibilisierung; Überstimulation; Mischreaktionen | E-KO-024 |
| Schmerz | lokal, mit Schmerztoleranz **getrennt** vom Schaden | E-GE-010 |
| Höhepunkt | Multisystem-Ereignis; nicht garantiert; individuell; mehrfach möglich; Refraktär-/Erholungsphase; Nachglühen | E-IA-019, E-KO-024 |
| Ermüdung | pro Muskelgruppe, Haltungsermüdung, Krämpfe, Muskelkater | E-KO-024 |
| Atmung | Frequenz/Tiefe getrennt; mit Audio und Brustbewegung synchron | E-KO-022 |
| Temperatur/Schweiß/Hautrötung | sichtbare Hautreaktionen, ja, nicht das Wichtigste | E-KO-008, E-KO-009 |
| Herzschlag | eigener nur bei Extremstress hörbar, fremder nur in Nähe | E-KO-007 |
| Muskelspannung | lokal, beeinflusst Pose/Reaktion | E-KO-022 |
| Stimmung/Stress | kurzfristige Zustände | E-NP-012 |
| Erfahrung | mehrere unabhängige Bereiche (nicht ein „SexSkill“) | E-IA-022 |

Regeln:
- Es gibt **keinen sichtbaren Erregungsbalken** und keine Arcade-Statistik als Verhaltenstreiber (E-IA-021, E-NP-027). Zähler sind für Statistik/Debug erlaubt.
- Die Intensität wird aus Kraft, Geschwindigkeit, Druck und Bewegung abgeleitet, nicht aus einem Zweifachschalter (35#485).
- Tagesform, Gewöhnung, Stimmung, Alkohol, Müdigkeit beeinflussen Reaktionen (35#485/E-NP-023).
- Vorlieben sind tageabhängig; Libido ist nicht konstant; es gibt keinen universellen Cooldown (E-NP-028).

## 6. Reaktionsgraph

### 6.1 Prinzip

📋 E-KO-022 / E-KO-027: Die sichtbare Reaktion entsteht aus *Handlung + Körperzustand + Persönlichkeit + Beziehung + Präferenzen + aktueller Absicht + Zustimmungs-/Grenzzustand + Schmerz/Stress + Umgebung*, nicht aus einer fertigen Animation.

```
Kontakt / Bewegung (aus der Engine)
        │
        ▼
Physiologie (Last, Gewebe, Atmung, Ermüdung)
        │
        ▼
Lokale Empfindung (Schmerz / Lust / Mischzustand)
        │
        ▼
Interpretation: Emotion + Präferenz + Zustimmungs-/Grenzzustand
        │
        ▼
Ausdruck: Mimik · Stimme · Atmung · Muskelreaktion · Gegenbewegung · Sprache
        │
        ▼
Gedächtnis / Beziehung / Statistik
```

### 6.2 Regeln (🧭)

| Nr. | Regel | Basis |
|---|---|---|
| KS-RG-01 | Mehrere Reaktionen laufen **gleichzeitig** (Reaction Composition). | E-KO-023 |
| KS-RG-02 | Der **Grenz-/Zustimmungszustand** ist Eingabe des Graphen: ein Stopp-Signal erzeugt eine eindeutige Reaktion und löst den Regelkern aus, nicht umgekehrt. | E-SK-019, [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) |
| KS-RG-03 | Positive Reaktion macht einen Grenzübertritt **nicht rückwirkend** in Ordnung. | 35#632 |
| KS-RG-04 | Erstarren/Stillstand ist **nicht automatisch positiv oder negativ**; es wird als Signal für Check-ins genutzt. | 35#652 (R16K), E-SK-023 |
| KS-RG-05 | Reaktionsprofile sind **personenspezifisch** (Stimme, Lautstärke, Latenz, Mimikmuster, Blickverhalten). | E-KO-021, 35#611 |
| KS-RG-06 | Unerfahrene sind unbeholfener, manche lauter bei ungewohnt Hartem; Erfahrung ≠ Intensitätswunsch. | 35#458/66, E-IA-022 |
| KS-RG-07 | Reaktionen sind deterministisch bei gleichem Seed und Zustand. | E-SO-016 |
| KS-RG-08 | Mods registrieren Reaktionen über Registry-Haken; sie ändern den Graphen nicht direkt. | E-SP-006 |

## 7. Stimme, Mimik, Blick, Atmung

| Bereich | Anforderung (🧭) | Basis |
|---|---|---|
| Stimme | Reaktionsprofil aus Stimmmodell + Persönlichkeit + Physiologie; Lautstärke situationsabhängig; Sprache und Nonverbales gemischt; jede Person anders; auch lautes Schreien möglich | E-KO-021, 35#487 |
| Mimik | gewichtete Zustände aus den allgemeinen Emotionssystemen; individuelle Muster | 35#611 |
| Blick | Blickkontakt wird simuliert (Vermeiden, Suchen, Schließen) | 35#611 |
| Atmung | Frequenz/Tiefe getrennt; synchron mit Brustbewegung und Audio | E-KO-022 |
| Latenz | Reaktion folgt dem Reiz verzögert, aber „sekundengenau“ | 35#609 |
| Audio | prozedural; Stimm-Packs getrennt; Sprachen/Qualitäten | E-PA-021 |
| Dialog | in Pausen; kurze Äußerungen auch aktiv | E-IA-041 |

Die Audioarchitektur ist in [Modul 04, Abschnitt 7](04-Steuerung-Darstellung-Kamera-Audio.md#7-audio-und-stimmen) beschrieben.

## 8. Körperliche Grenzreaktionen und Inhaltsschalter

Der Entwickler hat körperliche Grenzreaktionen (z. B. Würgereflex, Brechreiz, Tränen, Atemnot bei bestimmten Handlungen) ausdrücklich als **sehr wichtig** bezeichnet (35#486/59) und sehr detaillierte Wünsche geäußert (35#653). Die Dokumentation fasst das abstrakt zusammen:

| Nr. | Aussage | Status | Basis |
|---|---|---|---|
| KS-GR-01 | Solche Reaktionen sind **physiologische Zustände** aus Belastung, Körperzustand, Reflexempfindlichkeit, Erfahrung und Anpassung; **individuell**; keine feste Effektanimation. | 📋 | E-KO-020 |
| KS-GR-02 | Die Zustände sind **kontinuierlich** mit Schwellen (z. B. Brechreiz als Zustand mit Schwelle); mehrere gleichzeitig. | 🧭 | E-KO-023 |
| KS-GR-03 | Es gibt ein **Inhaltsschalter-Konzept**: starke Körperreaktionen sind separat reduzier- oder abschaltbar, ohne die zugrundeliegende Physiologie zu entfernen (z. B. nur die Darstellung). | 📋 | E-ST-011 |
| KS-GR-04 | Folgen laufen über normale Systeme (Flüssigkeiten, Sauberkeit, Hygiene, Gesundheit, Weltreaktion). | 🧭 | E-IA-031 |
| KS-GR-05 | Die **Detailtiefe** (wie drastisch dargestellt, welche Mengen) ist **offen** und eine Entscheidung des Entwicklers, mit Blick auf Plattform und Recht. | ❓ | [Modul 10](10-Produktion-Recht-Vertrieb.md) |
| KS-GR-06 | Marktbeobachtung der Quellen: andere Spiele bieten solche Optionen mit Deaktivierungsschaltern (Chat 09). | 📋 Kontext | [Modul 17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md#12-ausgeschlossene-quellen) |

Die Dokumentation beschreibt keine einzelnen Darstellungsdetails und keine Anleitung.

## 9. Risiko, Verletzung, Heilung

### 9.1 Unified Risk Pipeline

📋 E-KO-027:

```
Aktion + Intensität + Kontakt + Anatomie + aktueller Körperzustand + Umgebung
        ▼
mechanische Last
        ▼
Empfindung / Schmerz
        ▼
Schaden / Verletzung
        ▼
Reaktion
        ▼
Erholung / Medizin
        ▼
Gedächtnis
```

Sie dient auch Kampf, Sport, Unfällen, Medizin. Es gibt **keinen** Adult-Sonderpfad (E-KO-027).

### 9.2 Regeln

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| KS-RV-01 | **Hochrisiko-Markierung** sperrt nie automatisch; sie gewichtet Risiko im Gesundheitssystem. | E-IA-026, E-GE-010 |
| KS-RV-02 | Risiko ergibt sich aus Basisrisiko, Intensität, Anatomie, Erfahrung/Anpassung, bestehenden Verletzungen, Items, Umgebung. | E-GE-010 |
| KS-RV-03 | Risikoklassen: Gewebe, Gelenk/Muskel, Atmung, Impact, Reibung, Sturz/Umwelt, Ermüdung. | E-GE-010 |
| KS-RV-04 | **Erfahrung ≠ Widerstandsfähigkeit**; Schmerztoleranz ≠ Schadensminderung; Anpassung ist **lokal** und hat biologische Grenzen („keine magische Anpassung“). | E-GE-010, E-SO-035 |
| KS-RV-05 | Dynamische Verletzungsschwelle; Eskalation klein → groß; **keine Heilung während einer Sitzung**; Langzeitfolgen nur, wenn plausibel. | E-GE-010 |
| KS-RV-06 | Verletzungen akkumulieren; Folgesitzungen werden riskanter; Heilung über Wochen; das Spiel setzt keine feste Obergrenze, **die Figur entscheidet**, alle Folgen laufen über das Gesundheitssystem. | E-GE-006 |
| KS-RV-07 | Gesundheitszustand beeinflusst KI, Reaktion, Präferenz. Eine Präferenz kann Risiko überwiegen (Figur nimmt es in Kauf), **ohne** Sicherheitskern und Verletzungssystem zu verändern. | E-GE-010, E-GE-001 |
| KS-RV-08 | **Notfallerkennung**: schwere Zustände gehen ins normale Medizinsystem (Krankenhaus, Notruf) ohne Adult-Sonderbehandlung; NPCs reagieren individuell. | E-GE-010 |
| KS-RV-09 | Praktiken mit Atem-/Halskontrolle: seltene Hochrisiko-Präferenz **im Gesundheitssystem**; Bewusstlosigkeit beendet die Interaktion; **keine How-to-Mechanik**. | E-GE-008, E-SK-017 |
| KS-RV-10 | Risiko-UI ist **subtil** (keine permanenten Prozentwerte); Entwickler-Inspector zeigt Details. | E-KO-028 |
| KS-RV-11 | Replay speichert Gesundheitsereignisse. | E-SP-001 |
| KS-RV-12 | Sehr extreme, ausdrücklich gewünschte Praktiken seltener Erwachsener sind modellierbar, aber **das Verletzungssystem greift immer**; „weil gewollt, passiert nichts“ gilt nicht. | E-KO-019 |
| KS-RV-13 | Die Dokumentation definiert **keine** Ausführungsregeln oder „optimalen“ Ausführungen riskanter Handlungen; modelliert werden Zustand und Folgen. | E-GE-001 |

### 9.3 Heilung

Heilungsstufen, Narbengewebe, Schmerzgedächtnis (E-SO-035), Alltagseffekte (Sitzen, Schlaf, Sport, Arbeit; E-KO-024), Selbstpflege, medizinische Eskalation, Erholungsprognose (nur Inspector). Ärztliche Versorgung ist möglich; Krankenhausaufenthalt bei schweren Verletzungen realistisch (Modul 06).

## 10. Schwangerschaft, Verhütung, Gesundheitswissen

| Nr. | Aussage | Status | Basis |
|---|---|---|---|
| KS-FP-01 | Verhütung als **Items/Zustände** mit Wirksamkeit und Anwendungsfehlern; NPC mit eigener Methode; Gespräch darüber möglich. | 📋 | E-GE-002 |
| KS-FP-02 | Fruchtbarkeit individuell; Zyklus **abstrahiert** (kein Kalender-Mikromanagement). | 📋 | E-GE-002 |
| KS-FP-03 | Schwangerschaft entsteht nur bei passender Biologie und Situation; **langfristiger Weltzustand** (Körper, Gewicht, Bewegung, Kleidung, Termine, Arbeit, Beziehungen). | 📋 | E-GE-002, E-GE-011 |
| KS-FP-04 | Tests als echte Gegenstände; Ärzte/Kliniken. | 📋 | E-GE-002 |
| KS-FP-05 | Hormonelle Verhütung kann Stimmung/Libido beeinflussen (Entwickler: ‚ganze Persönlichkeit‘). | 🧭 | E-GE-003 |
| KS-FP-06 | Geburt: Junge oder Mädchen; wenn der Spieler es will. | 📋 | E-GE-002 |
| KS-FP-07 | **Kein detailliertes STI-System** in der Basisversion; Architektur lässt Erweiterung zu. | 📋 | E-GE-004 |
| KS-FP-08 | Wissen über den eigenen Gesundheitsstatus, Hygiene, Duschen als normale Handlung. | 📋 | E-GE-005 |
| KS-FP-09 | Familiensystem/Generationen: niedrige Priorität/offen. | ❓ | CA05 |

## 11. Flüssigkeiten und Aufräumen (Spätes Feature)

📋 Der Entwickler (35#687): „die Sachen sind wirklich cool, aber erst ganz spät zu implementieren“. Prioritäten laut Vorschlag (35#688, bestätigt 35#689/690):

| Phase | Inhalt |
|---|---|
| Früh | Interaktionskern, Pose/IK, Körperzustand, NPC-Intent, einfache Reaktionen |
| Mittel | Kleidung, Items, Verletzungen, Präferenzen, Orte, Replay |
| **Spät** | detaillierte Flüssigkeiten (Körper und Oberflächen), Absorption, Übertragung, Trocknen, Aufräumen, High-End-Rendering |

Anforderungen (🧭): Flüssigkeitszustand liegt auf der **Material-/Physik-Infrastruktur** (E-GE-005); Menge und Viskosität sind individuell; Aufräumen läuft über normale Weltaktionen; Kleidung/Bettwäsche zeigt sauber/verschmutzt; Speicher: persistiert wird nur Langzeitrelevantes.

## 12. Technische Komponenten

🧭 Kern engine-neutral:

| Komponente | Aufgabe |
|---|---|
| `BodyStateModel` | Parameter, Veränderungen, Adaptation |
| `TissueModel` | Gewebezustände, Heilung |
| `ArousalModel`, `PainModel`, `FatigueModel` | Zustandsmodelle |
| `ReactionGraph` | Eingaben → Reaktionen |
| `RiskPipeline` | Last, Schaden, Folgen |
| `MedicalEscalation` | Übergabe an Medizinsystem |
| `ReactionProfile` | personenspezifische Ausdrücke |
| `ContentToggleFilter` | Darstellung nach Nutzereinstellung |
| `SoftTissueSolver` (Adapter) | Echtzeit-Weichgewebe |
| `UnifiedOutcomeLayer` | gemeinsame Ergebnisschicht für Adult, Kampf, Sport, Medizin |

## 13. Randfälle

[Modul 31](31-Randfaelle-Katalog.md#4-körper-und-reaktion): Teilnehmer mit Behinderung oder Prothese; Schwangerschaft in späten Stadien; gleichzeitige Verletzung und Alkohol; Medikamente verändern Reaktionen; Körper ändert sich während einer Sitzung (Zeitsprung); Content-Toggle wird mitten in der Sitzung geändert; Mod-Reaktion erzeugt Widerspruch zum Zustimmungszustand.

## 14. Tests und Abnahmekriterien

| Test | Erwartung |
|---|---|
| Körperparameter-Fuzz (1.000 Körper) | keine unplausiblen Kopplungen, Asymmetrie vorhanden |
| Reaktions-Determinismus | gleicher Seed → gleiche Reaktionszusammensetzung |
| `HardStop` in jedem Reaktionszustand | Reaktion und Abbruch konsistent |
| Content-Toggle an/aus | Physiologie identisch, Darstellung unterschiedlich |
| Verletzung → Folgesitzung | höheres Risiko, längere Heilung |
| Risikoklassen | monotone Zunahme bei Last |
| Schwangerschaft über 9 Monate | Körper-/Physikparameter verändern sich plausibel |
| Kein `usageCount` | Anatomie ändert sich nicht durch Wiederholung |
| Keine STI-Zustände | Datenmodell enthält keine Felder dafür |

Siehe [Modul 29](29-QA-Testkatalog.md#4-körper-und-reaktion), [Modul 30](30-Abnahmekriterien.md#4-körper-und-reaktion).

## 15. Ungeklärtes

- Detailtiefe von Grenzreaktionen und deren Darstellung (Plattform, Recht).
- Solver-Wahl und Budgets.
- Zeitliche Skalierung von Heilung (hängt von [K15](19-Konflikt-und-Widerspruchsmatrix.md#k15-zeitmaßstab) ab).
- Beschneidung/ähnliche Datenmerkmale ([K14](19-Konflikt-und-Widerspruchsmatrix.md#k14-beschneidung-und-religion)).
- Hormonelle Effekte: Stärke ist Vorschlag.
- Familiensystem und Generationen.
- Verletzungsdarstellung (Gore-Niveau) im Adult-Kontext: ausdrücklich an Plattform-/Recht-Prüfung gebunden (PD, T44).
