# Adult 21: Spezifikation Datenmodell

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug in der Spalte „Master-Basis“ ([Register](14-Entscheidungsregister.md)). Kein Kanon, kein Auftrag zur Implementierung. Die Datenmodelle sind **logische Modelle**, kein Dateiformat; ein Format wurde nicht beschlossen (Review 35#176 Punkt 10). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Regelkern: [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md). Speicherung und Mods: [Modul 28](28-Spezifikation-Speicherung-Mods-Builds.md).

**Inhalt**

1. [Grundsätze](#1-grundsätze)
2. [Kennungen und Registries](#2-kennungen-und-registries)
3. [Figurendaten (Basis vs. Adult)](#3-figurendaten-basis-vs-adult)
4. [Präferenzmodell](#4-präferenzmodell)
5. [Beziehungs- und Gedächtnisdaten](#5-beziehungs--und-gedächtnisdaten)
6. [Interaktionssitzung](#6-interaktionssitzung)
7. [Taxonomie-Registry](#7-taxonomie-registry)
8. [Körper- und Reaktionszustand](#8-körper--und-reaktionszustand)
9. [Tattoo- und Körpermodifikationsdaten](#9-tattoo--und-körpermodifikationsdaten)
10. [Weltdaten (Orte, Betriebe, Verträge, Medien)](#10-weltdaten-orte-betriebe-verträge-medien)
11. [Konfiguration der Spieler](#11-konfiguration-der-spieler)
12. [Ereignisse](#12-ereignisse)
13. [Versionierung und Migration](#13-versionierung-und-migration)
14. [Größen- und Leistungsvorgaben](#14-größen--und-leistungsvorgaben)
15. [Randfälle](#15-randfälle)
16. [Tests und Abnahme](#16-tests-und-abnahme)
17. [Ungeklärtes](#17-ungeklärtes)

---

## 1. Grundsätze

| Nr. | Grundsatz | Master-Basis | Status |
|---|---|---|---|
| D1 | **Ein** Menschenmodell für das ganze Spiel; Adult-Daten erweitern es, ersetzen es nicht. | E-SO-005, E-PA-028 | 📋 |
| D2 | Adult-Daten werden **separat gespeichert**, verknüpft über dieselben stabilen IDs wie die Basisdaten. | E-PA-029, E-PA-030 | 📋 |
| D3 | IDs enthalten **keine Version**; Versionen sind eigene Felder. | E-SP-007 | 🧭 |
| D4 | Offene Kategorien sind **Daten** (Registry), keine geschlossenen Enums im Kern. | E-SP-007, E-IA-005 | 🧭 |
| D5 | Speichere **Ergebnisse, Zustände und Ereignisse**, nicht Frames, Schleifen, Kamera oder IK-Zwischenstände. | 28#25, E-LE-002 | 🧭 |
| D6 | **Default-Welt wird nicht gespeichert**, nur das Delta gegenüber dem aus Seed erzeugten Ausgangszustand. | E-PA-025, E-LE-002 | 📋 / 🧭 |
| D7 | Speichergröße ist Designziel; Vorschlag des Assistenten 50–150 MB normal (nur Orientierung). | E-LE-003 | 🧭 |
| D8 | Mods schreiben **nie** direkt in Basis-Speicherstrukturen; sie haben eigene Partitionen. | E-SP-007, E-PA-030 | 🧭 |
| D9 | Kein Datenfeld darf die Altersprüfung umgehen; Eligibility wird abgeleitet, nicht gesetzt. | E-SK-012, E-SK-028 | 🧭 |
| D10 | Daten Verstorbener bleiben (Identität, wichtige Geschichte), werden nicht komplett gelöscht. | E-SK-014 | 🧭 |

## 2. Kennungen und Registries

### 2.1 Stabile Kennungen (🧭, E-SP-007)

Muster `namespace:kind:local-id`, zum Beispiel `spn:interaction_family:touch` (Beispielwerte, **keine** festgelegten Namen). Regeln:

- Eine Kennung ändert sich nie; Umbenennung nur über eine **Alias-Tabelle**.
- Namespaces trennen Basisspiel, offizielle Erweiterungen und Mods (`ContentOrigin`: BaseGame, OfficialDLC, DeveloperMod, CommunityMod; PS-Abschnitt, E-PA-034).
- Versionsfelder: `contentVersion`, `schemaVersion`, `modVersion`, `apiVersion`.

### 2.2 Registries (🧭)

| Registry | Inhalt | Quelle der Anforderung |
|---|---|---|
| `InteractionFamily`, `InteractionType`, `Variant` | Taxonomie | E-IA-024 bis E-IA-040 |
| `BodyRegion` | hierarchisch, links/rechts | E-IA-025 |
| `PreferenceDefinition` | Präferenzdefinitionen (siehe Abschnitt 4) | E-NP-019, 35#543 |
| `ReactionType`, `OutcomeType`, `FluidType` | Reaktions- und Ergebnisfamilien | E-IA-031, E-KO-022/023 |
| `SurfaceType`, `LocationTag` | Flächen und Orte | E-IA-006, E-IA-035 |
| `ItemDefinition` | Items, Materialien | E-KO-026 |
| `TattooMotif`, `TattooStyle`, `PigmentType` | Tattoos | E-TA-014, E-TA-017 |
| `ModificationType` | Piercings, Körpermodifikation | E-TA-010, E-TA-025 |
| `RiskHook`, `ReactionHook` | Haken für Risiko und Reaktion | E-IA-030 |
| `VenueType`, `ServiceType`, `ContractType` | Betriebe und Verträge | E-WE-001, E-NP-024 |
| `ContentProfile` | Inhalts-/Darstellungsprofile | E-PA-009, E-IA-040 |

Die Registry-Struktur steht unter dem **Registry-Freeze-Vorbehalt**: Sie wird erst nach Missing-Family- und Qwen-Audit eingefroren (E-PR-009). Die Kategorienamen sind nicht final (E-IA-030).

## 3. Figurendaten (Basis vs. Adult)

### 3.1 Basisdaten (gehören zum Hauptspiel)

| Feld | Bedeutung | Basis |
|---|---|---|
| `character_id` | globale NPC-/Spieler-ID | E-LE-002 |
| `birth_date` | Geburtsdatum | E-SK-012 |
| `life_state` | lebendig, tot, verschollen | E-SK-014 |
| `body_profile_seed`, `body_params[]` | kontinuierliche Körperparameter, Asymmetrie | E-KO-001, E-SO-001 |
| `appearance_layers` | Haare, Haut, Make-up, Kleidung | E-KO-003/004 |
| `tattoos[]`, `modifications[]` | Tattoos, Piercings (persistent) | E-KO-002, E-TA-014 |
| `personality_core` | stabile Dimensionen | E-NP-012 |
| `physiology` | Gewicht, Muskel, Ausdauer, Verletzungen | E-SO-029 |

### 3.2 Adult-Erweiterung (nur im Adult-Speicherbereich)

| Feld | Bedeutung | Basis |
|---|---|---|
| `eligibility_cache` | abgeleitet, nicht Quelle der Wahrheit | E-SK-028 |
| `preference_state[]` | Präferenzzustände je `PreferenceDefinition` | Abschnitt 4 |
| `experience[]` | Erfahrung (allgemein, je Bereich, partnerspezifisch) | E-IA-023 |
| `consent_ledger` | Zustimmungshistorie | [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) |
| `boundary_history` | Grenzen/Widerrufe | E-SK-021 |
| `adult_memories[]` | Erinnerungen | Abschnitt 5 |
| `body_additions` | zusätzliche, nur für Erwachsene sichtbare Körperoptionen (z. B. Intimanatomie) | E-KO-015 |
| `adult_item_ownership` | Items/Verlauf | E-KO-026 |
| `media_credits` | Beteiligung an Medienproduktionen | E-WE-007 |

Regel D9/E-SK-032: Für nicht `AdultEligible` Figuren existiert **kein** Adult-Erweiterungsdatensatz.

## 4. Präferenzmodell

### 4.1 Definition und Zustand (🧭 aus 35#543)

`PreferenceDefinition`: `id`, `category`, `tags`, `intensity_range`, `required_features`, `risks`, `related_preferences`, `provider` (Mod-Herkunft).

`PreferenceState` je Figur und Definition:

| Feld | Bedeutung | Basis |
|---|---|---|
| `interest` | Stärke des Interesses (kontinuierlich, auch negativ) | E-NP-019 |
| `curiosity` | getrennt von Erfahrung | E-IA-022 |
| `experience` | Erfahrung in diesem Bereich | E-IA-023 |
| `preferred_intensity` | mehrdimensional | E-NP-019 |
| `partner_modifiers[]` | partnerspezifische Abweichungen | E-NP-020 |
| `context_modifiers[]` | Kontext (Ort, Stimmung, Zeit) | E-NP-019 |
| `risk_tolerance` | Risikoakzeptanz | E-GE-001 |
| `knowledge_state` | `unknown/suspected/told/experienced_together` je Partner | E-NP-019 |
| `limit_class` | Stufen: neugierig, offen, neutral, situativ, ungern, weiche Grenze, **harte Grenze** | E-NP-004 |
| `memories[]` | Verweise auf Erinnerungen | E-LE-008 |

Wichtig:
- Präferenzen sind **kein** Katalog sichtbarer Fetische (E-NP-019); der Spieler sieht sie nicht (nur Entwicklungsmodus, E-NP-021).
- `limit_class = hard` ist eine **Invariante** (E-SK-028); Überreden verschiebt nur Neutral/Situativ (E-NP-006).
- Hard Limits können sich über lange Zeit ändern (E-SK-005), aber nur durch einen **eigenen Veränderungsvorgang** der Figur (Zeit, Beziehung, Erlebnis), nicht durch Wiederholen von Anfragen und nicht durch Mods.
- Präferenzen entstehen aus **Präferenz-DNA** (Seed, einmal bei Erzeugung) und verändern sich durch Erlebnisse (E-NP-012, E-NP-002).

### 4.2 Attraktion und Chemie

Getrennte Werte: Attraktion, Vertrauen, Zuneigung, Romantik, Komfort, Vertrautheit (E-NP-007). Chemie bidirektional. Attraktion wird auf dem **tatsächlich erzeugten** Körper berechnet (E-KO-005).

## 5. Beziehungs- und Gedächtnisdaten

| Datensatz | Inhalt | Basis |
|---|---|---|
| `PartnerProfile` | Wissen einer Figur über eine andere (kann fehlerhaft sein, mit Konfidenz) | E-NP-029 |
| `RelationshipRecord` | Werte, Status (Beziehung, offen, getrennt), Eifersucht, Geheimnisse, Vereinbarungen | CA05, E-NP-007 |
| `Memory` | Rohereignis → Gedächtniseintrag mit Wichtigkeit, Emotion, Konfidenz | E-LE-008, 35#473 |
| `AnchorMemory` | starke Erinnerungen, dauerhaft | E-NP-016 |
| `KnownPersonRecord` | minimal bekannte Personen | E-LE-008 |

Gedächtnisstufen (🧭 Vorschlag, E-LE-008): `Minimal`, `Known`, `Detailed`, `Full`. Eine Zusammenfassung ersetzt **keine** Einzelerinnerung, solange der NPC sich „an alles einzeln erinnern kann“ (35#279/36, E-LE-002). Einmalbegegnungen werden genauso genau gespeichert wie Beziehungen (E-NP-015). Kein Gedächtnis darf Zustimmung ersetzen.

## 6. Interaktionssitzung

### 6.1 Sitzung (Laufzeit)

| Feld | Bedeutung |
|---|---|
| `session_id`, `session_seed` | Kennung, Seed für deterministische Mikrovariation (E-SO-016) |
| `participants[]` | 2..N Teilnehmer mit `role` pro Moment (E-IA-004, E-PA-014) |
| `intent_stack` | aktuelle Absichten je Teilnehmer, Initiative pro Aktion (E-IA-025) |
| `actions[]` | laufende Aktionen als Schichten (Action Layer) mit Typ, Zielregion, Körperteil/Item, Parametern (E-IA-025) |
| `pose_state` | kontinuierlicher Ganzkörperzustand je Teilnehmer, keine Auswahl aus einer Liste (E-SO-007) |
| `contact_graph` | Kontaktpunkte mit Fläche, Kraft, Reibung, Material (E-SO-013) |
| `intensity_axes` | Geschwindigkeit, Kraft, Druck, Umfang, Rhythmus, Dauer, psychologisch (E-IA-025) |
| `environment_ref` | Ort, Flächen, Privatheit (E-IA-035) |
| `envelope_ref` | Sitzungsrahmen ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)) |
| `state` | Zustandsautomat (siehe [Modul 22](22-Spezifikation-Interaktionsengine.md)) |
| `witnesses[]` | Zeugen mit Sichtlinie, Hörweite (E-WE-021) |

### 6.2 Persistierte Sitzungsergebnisse

Nach Ende: Dauer, Teilnehmer, Kategorien (nachträglich klassifiziert, E-IA-025), Erlebnisqualität je Teilnehmer, Verletzungen, Beziehungsänderungen, Erinnerungen, Statistik-Ereignisse, ggf. Replay-Referenz (E-RP-002). **Nicht** gespeichert: Animationszustände, IK-Ziele, Kameradaten (D5).

## 7. Taxonomie-Registry

🧭 Aus dem Registry-Schema (E-IA-030): `id → category → subcategory → tags → target_regions → participants → body_part/item → intensity_axes → position_requirements → location_requirements → reaction_hooks → risk_hooks → preference_hooks → memory/statistics_hooks → replay/modding_metadata`.

Eigenschaften:
- **Orthogonale Dimensionen** (E-IA-024): Kategorie, Unterart, Beteiligte, Rollen, Körperbereiche, Intensität, Tempo, Kraft, Ort, Umgebung, Kleidung, Items, Öffentlichkeit, Risiko, Erfahrung, Präferenz und weitere; Mehrfachzuordnung.
- **Granularitätsregel** (E-IA-032): ein eigener, klar benannter Eintrag nur, wenn er technisch, physiologisch, präferenz-, statistik- oder gesundheitsrelevant ist.
- **Intensität ist nie eine Kategorie** (E-IA-038).
- **Schmerz ist Dimension, nicht Kategorie** (E-IA-026).
- Root-Taxonomie und Familienliste: [Modul 03](03-Interaktion-Taxonomie-Ablauf.md#2-taxonomie).
- Neue Kategorien über Daten ohne Codeänderung (E-IA-005).

Die konkreten Einträge werden in dieser Dokumentation **nicht** aufgezählt (Entscheidung der Etappe; siehe [Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md)).

## 8. Körper- und Reaktionszustand

| Datensatz | Inhalt | Basis |
|---|---|---|
| `TissueState` je Region | Elastizität, Compliance, Dämpfung, Dehnung (vorübergehend/dauerhaft), Mikrotrauma, Heilungsstufe | E-SO-035 |
| `PainState` | lokal pro Region, Toleranz getrennt | E-KO-024, E-GE-010 |
| `ArousalState` | kontinuierlich, verborgen, kann sinken | E-IA-019, E-SK-030 |
| `FatigueState` | pro Muskelgruppe, Krampf, Muskelkater | E-KO-024 |
| `InjuryRecord` | Verletzungen mit Verlauf, Narben | E-GE-006 |
| `ReactionComposition` | gleichzeitige Reaktionen, aus Reaction Graph | E-KO-022/023 |
| `FluidState` | Spät-Feature, Flüssigkeitszustände von Körper und Oberflächen | E-IA-033 |
| `HealthEvent` | an Replay und Gesundheit | E-KO-027 |

Alle Werte sind Instanzen des **Unified Body State** (E-SO-035), der auch Kampf, Sport, Unfälle und Medizin bedient. Es gibt keinen Adult-Sonderkörper (E-SO-029).

## 9. Tattoo- und Körpermodifikationsdaten

Siehe [Modul 25](25-Spezifikation-Tattoo-und-Koerpermodifikation.md) für Details. Kern:

| Datensatz | Felder (🧭) |
|---|---|
| `Tattoo` | `id`, `motif_ref`, `style_tags`, `artist_id`, `session_log[]`, `body_surface_mask`, `rotation`, `scale`, `pigments[]`, `quality`, `age`, `layer_index`, `lifecycle_state`, `meaning` |
| `TattooComposition` | Sleeve/Patchwork/Ganzkörper als Sammlung mit Plan |
| `Piercing` / `Modification` | `type`, `site`, `material`, `jewelry`, `healing_state`, `scar` |
| `ArtistProfile` | mehrdimensionale Fähigkeiten, Handschrift, Reputation, Studio |
| `Studio` | Typ, Standort, Preise, Hygiene, Reputation, Termine |

Quelle: E-TA-007/008/014/018/019/025.

## 10. Weltdaten (Orte, Betriebe, Verträge, Medien)

| Datensatz | Inhalt | Basis |
|---|---|---|
| `Venue` | Typ, Hausregeln, Sicherheit, Gesetzeslage der Jurisdiktion, Raumtypen, Zugang, Kameras, Ruf | E-WE-001, E-IA-007, E-IA-009 |
| `ServiceProfile` | Leistungen, Grenzen, Preise, Zeitplan, Persönlichkeit | K§19, E-WE-001 |
| `Contract` | Leistungen, Vergütung, optional Unterkunft/Verpflegung, **Kündigungsrechte beider Seiten**, Alters-/Identitätsprüfung | E-NP-024, 35#543 |
| `MediaProduction` | Aufnahme/Veröffentlichung als getrennte Zustimmungen, Rechte, Einnahmen | E-WE-011 |
| `MediaAsset` | Replay-Referenz, feste Replay-ID, Cache-Schlüssel | E-WE-010 |
| `Reputation` | mehrdimensional (Beruf, Szene, Ruf, Fans) | E-WE-007 |

Regeln: Verträge verändern **nie** Zustimmung ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md#64-was-ausdrücklich-keine-zustimmung-ist)). Der NPC behält das Kündigungsrecht (E-NP-024, K6).

## 11. Konfiguration der Spieler

🧭 Konfigurationsschichten (Review 35#176 Punkt 15, E-SP-008): `Global → Mod-Profil → Charakter → Speicherstand → pro NPC`. Nutzer-Overrides sind **nie destruktiv**. Settings-Kategorien (E-SP-003): Gameplay, Steuerung, Grafik, Physik, Audio, Inhalt, Replay, VR, Barrierefreiheit, Erweitert.

| Einstellung | Bedeutung | Basis |
|---|---|---|
| `adult_package_enabled` | pro Speicherstand | E-PA-006/007 |
| `nudity_level` | voll/reduziert/aus oder gleichwertig | E-PA-009 |
| `content_toggles` | z. B. starke Körperreaktionen, Blut | E-ST-011 |
| `ui_help_level` | aus/minimal/normal/ausführlich | E-SP-003 |
| `hud_level` | Off/Minimal/Standard/Detailed/Developer | E-ST-005 |
| `setup_answers` | Antworten des Erststart-Assistenten | E-ST-008 |
| `stream_visibility` | Adult-Inhalte separat streambar/versteckt | E-RP-005 |

Es gibt **keinen globalen Intensitätsschalter** (E-PA-011): Intensität steckt im Paket und in den Profilen.

## 12. Ereignisse

Interaction Event Stream (E-LE-010): Abonnenten Gedächtnis, Statistik, Replay, Audio, Animation, Gesundheit, Modding. Ereignis-Hüllen enthalten `event_id`, `session_id`, `time`, `participants`, `payload_type`, `schema_version`. Die Ereignisse des Regelkerns stehen in [Modul 20, Abschnitt 9](20-Spezifikation-Regelkern-Zustimmung.md#9-zustände-ereignisse-daten).

## 13. Versionierung und Migration

| Anforderung (🧭) | Basis |
|---|---|
| Jeder Adult-Speicherbereich hat eine `schemaVersion`. | E-SP-004 |
| Migrationen sind erforderlich, vorwärtsgerichtet, getestet (Golden Files). | E-SP-001 |
| Deaktivieren und Reaktivieren des Pakets erhält kompatible Daten (Hot Enable/Disable). | E-PA-030 |
| Verwaiste Referenzen (gelöschtes Paket, entfernte Mods) werden markiert, nicht gelöscht. | E-PA-030 |
| Speicherstand ohne Paket bleibt vollständig spielbar. | E-PA-028 |
| Der Adult-Bereich ist getrennt sicher- und löschbar; optional verschlüsselt bzw. nicht menschenlesbar. | E-PA-030 |

## 14. Größen- und Leistungsvorgaben

- Orientierung des Assistenten: 50–150 MB normal, 150–300 MB extrem, mehrere GB wären ein Architekturproblem (35#280). Der Entwickler wollte „eher kleiner“ (35#279/40). **Keine Messung existiert.**
- Dynamik: nur Langzeitrelevantes persistieren (E-PR-012/436).
- Delta-Saves: unveränderte NPCs speichern nur ID und Seed (E-LE-002).
- Streaming/LOD siehe [Modul 28](28-Spezifikation-Speicherung-Mods-Builds.md).

## 15. Randfälle

Siehe [Modul 31](31-Randfaelle-Katalog.md#2-datenmodell). Auszug: Figur wird während des Speicherns befördert (Tier-Wechsel); Mod entfernt, aber Präferenzdefinition wird referenziert; Geburtsdatum fehlt (Altlast); gleiche Figur in zwei Speicherständen mit unterschiedlicher Mod-Liste; Zeitreise-Zustand (Temporal-World-State).

## 16. Tests und Abnahme

| Test | Erwartung |
|---|---|
| Round-Trip Speichern/Laden einer Sitzung mit Ergebnissen | gleiche Ergebnisse, kein Frame-Zustand |
| Laden ohne Adult-Paket | Basisspiel intakt, Adult-Daten bleiben erhalten |
| Migration alter Schemata | Golden-File-Vergleich |
| Altersfeld manuell setzen (Modversuch) | abgelehnt |
| Hard Limit per Mod ändern | abgelehnt, protokolliert |
| Größe nach 10.000 simulierten Begegnungen | unter Budget (Budget **noch festzulegen**) |

## 17. Ungeklärtes

- Dateiformat und Speicher-Backend (JSON/Binär/Datenbank) – nicht beschlossen.
- Verschlüsselung: Umfang, Schlüsselverwaltung (E-PA-030 „optional“).
- Konkrete Größenbudgets.
- Wie lange Zustimmungshistorie aufbewahrt wird (Datenschutz, Speicher).
- Behandlung geklonter oder importierter Figuren (Mods).
- Alters-/Eligibility-Prüfung für Figuren, die ein Mod einführt: nur über Geburtsdatum; Mods ohne gültiges Datum bleiben `Unknown`.
