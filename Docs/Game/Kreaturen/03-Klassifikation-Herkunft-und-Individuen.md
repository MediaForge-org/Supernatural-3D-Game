# Kreaturen 03: Klassifikation, Herkunft und Individuen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, **wie Kreaturen beschrieben werden**: Existenzform, Herkunft, Intelligenz, Antriebe, Temperament, Alter und Erfahrung, Ausnahmeindividuen und Einzelwesen. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Existenzform](#2-existenzform)
3. [Herkunft](#3-herkunft)
4. [Intelligenz in mehreren Dimensionen](#4-intelligenz-in-mehreren-dimensionen)
5. [Antriebe und Temperament](#5-antriebe-und-temperament)
6. [Alter und Erfahrung](#6-alter-und-erfahrung)
7. [Ausnahmeindividuen](#7-ausnahmeindividuen)
8. [Einzelwesen und Art](#8-einzelwesen-und-art)
9. [Entwurf: Datenmodell der Klassifikation](#9-entwurf-datenmodell-der-klassifikation)
10. [Entwurf: Prüfregeln](#10-entwurf-prüfregeln)
11. [Sonderfälle](#11-sonderfälle)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

Quelle für alle Punkte dieses Abschnitts ist der Master (`M-PD` ‚Creature / supernatural framework — expanded‘) und der Gesprächsverlauf, auf dem er beruht (`23#307A`, Antworten `23#308E`, Punkte 1–30 jeweils mit ‚ja/gut/passt‘ beantwortet). Wo eine Antwort mehr als Zustimmung enthält, steht sie wörtlich.

| Aussage | Quelle | Status |
|---|---|:---:|
| Kreaturen werden **nicht primär nach Stärke** eingeteilt (kein Tier 1/2/3/Boss als Hauptsystem); mehrere unabhängige Klassifikationen gleichzeitig | `23#307A` Punkt 1; „1. ja super“ in `23#308E` | 📋 |
| Klassifikationsdimensionen können sein: Existenz-/Körperform, Herkunft, Intelligenz, Erscheinungsart, soziales Muster, Bedrohungs-/Motivationsmuster, Regenerationsmodell und aktueller Wissensstand | `M-PD`; `23#307A` Punkt 1 | 📋 |
| **Existenzform und Herkunft sind getrennt** | `23#307A` Punkt 3, „3. genau“ in `23#308E`; `M-PD` | 📋 |
| ‚Unknown‘ ist eine **gültige Information**; nicht alles muss das Spiel erklären | `23#307A` Punkt 3 | 📋 |
| Intelligenz hat mehrere unabhängige Dimensionen | `23#307A` Punkt 4, „4. ja passt“ in `23#308E` | 📋 |
| Antriebe statt eines einzelnen Aggro-Werts | `23#307A` Punkt 5, „5. super“ in `23#308E` | 📋 |
| Individuen wichtiger Wesen haben Temperament (intern, kein sichtbarer Wert) | `23#307A` Punkt 6, „6. gut“ in `23#308E` | 📋 |
| Alter und Erfahrung sind getrennt; Alter vervielfacht nicht einfach die Stärke | `23#307A` Punkt 7, „7. ja passt“ in `23#308E` | 📋 |
| Außergewöhnliche, uralte oder ‚Alpha‘-artige Individuen sind **keine generische Elite-Klasse** | `23#307A` Punkt 8, „8. sehr gut“ in `23#308E` | 📋 |
| Sie dürfen die Standard-Schwäche ihrer Art teilweise überwinden; der Spieler lernt das durch Beobachtung, Misserfolg und Recherche, nicht durch Beschriftung | `23#293A`/`23#295A`; „A: ja man sollte es merken aber nicht direkt…“ in `23#294E`; `M-PD` | 📋 |
| Einzelne Individuen derselben Art unterscheiden sich in Alter, Stärke, Verhalten und teilweise Fähigkeiten | „152 sehr wichtig“ in `23#1075E` | 📋 |
| Es kann benannte Einzelwesen geben (eigene Geschichte, Modell, Fähigkeiten, Verhalten, Archivseite, Fälle) | `23#307A` Punkt 29, „29. cool“ in `23#308E`; `M-PD` | 📋 |
| Es gibt „vielleucht auch alphas, also die ersten ihrer art“ | „153“ in `23#1075E` | 📋 (Namensfrage ❓) |
| Das Archiv trennt **Art** und **bekannte Individuen** | `23#307A` Punkt 30, „30. ja“ in `23#308E`; `M-PD` | 📋 |
| Wichtige Arten sind handgeschrieben | `23#307A` Punkt 28, „28. ok“ in `23#308E` | 📋 |

---

## 2. Existenzform

Aus `23#307A` Punkt 2 (Antwort „2. das ist sehr gut“ in `23#308E`) und `M-PD`:

| Form | Beschreibung der Quelle | Folge für die Mechanik (Quelle) |
|---|---|---|
| **Physische Wesen** | echter Körper (Knochen, Organe oder artspezifische Strukturen), Masse, Bewegung, Verletzungen, physische Spuren | Ballistik-, Gore- und Nahkampfsystem greifen stark |
| **Transformierte Menschen** | ursprünglich Mensch; durch Fluch, Infektion, Ritual, Einwirkung, Verwandlung oder Vererbung verändert | „Nicht jede transformierte Person hat diese Existenz freiwillig gewählt“; Töten ist **nicht automatisch** die richtige Lösung |
| **Besessene oder kontrollierte Menschen** | Wirtskörper plus übernatürlicher Insasse bzw. Einfluss | Eine Kugel verletzt zuerst den **Wirtskörper**; die Entität muss dadurch nicht sterben; Rettung des Menschen ist möglich |
| **Geistartige / nichtkörperliche Wesen** | kein dauerhaft normaler Körper; Zustände nicht-körperlich, Teilmanifestation, volle Manifestation, gebundene Manifestation | normale Angriffe können wirkungslos sein, kurz stören oder die Manifestation destabilisieren |
| **Zeitweilig manifestierte Entitäten** | existieren normalerweise nicht dauerhaft in der physischen Welt, erzeugen zeitweise Körper oder Form | technisch anders als ein normales Tiermonster |
| **Gebundene Entitäten** | an Objekt, Gebäude, Ort, Person, Blutlinie, Ritual, Grab oder Ereignis gebunden | die Lösung kann sein, die **Bindung zu zerstören**, nicht Schaden zuzufügen |

**Humanoide Wesen** können das menschliche Anatomiesystem teilweise wiederverwenden (menschliche Grundanatomie plus übernatürliche Modifikatoren); vierbeinige, sehr große und geistartige Wesen haben eigene Anatomien (`23#293A` Punkt 2, „2. passt“ in `23#294E`). Die Anatomie behandelt [Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md).

**Zustände der Besessenheit** und der Geister behandelt [Modul 05](05-Geister-Besessenheit-und-Fluecher.md).

---

## 3. Herkunft

`23#307A` Punkt 3: Ursprungsfamilien **können später** umfassen (kein Katalog, keine Zahl):

| Ursprungsfamilie | Anmerkung |
|---|---|
| natürlich übernatürliche Spezies | |
| verfluchter Mensch | |
| verwandelter Mensch | |
| erschaffenes Wesen | |
| beschworene Entität | |
| gebundener Geist | |
| historische Anomalie | |
| dimensionsfremdes Wesen | |
| künstlich erzeugtes übernatürliches Experiment | `M-CA0905` §3: ‚Artificial/ritually constructed supernatural entities and guardians are within scope‘ |
| unbekannter Ursprung | gültige Angabe |

Die Entstehung wird **artspezifisch** geregelt (siehe [Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §6). Die Frage, ob ein Wesen in Wirklichkeit ein anderes ist („Es sieht nach einem Geist aus“ ist zunächst nur eine Hypothese), steht in [Modul 05](05-Geister-Besessenheit-und-Fluecher.md) und [Modul 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md).

---

## 4. Intelligenz in mehreren Dimensionen

`23#307A` Punkt 4 („4. ja passt“ in `23#308E`); `M-PD`:

| Dimension | Frage, die sie beantwortet |
|---|---|
| Problemlösen | Kann es Fallen verstehen oder improvisieren? |
| Soziale Intelligenz | Kann es Menschen manipulieren? |
| Taktische Intelligenz | Kann es Kampfmuster erkennen? |
| Gedächtnis | Wie gut erinnert es sich? |
| Sprache | Versteht oder spricht es Sprache? |
| Selbstkontrolle | Kann es Triebe kontrollieren? |
| Übernatürliches Wissen | Kennt es Rituale, Jäger, andere Wesen? |

Die Dimensionen sind **unabhängig**. Die Quelle nennt als Beispiele ‚extrem guter Jäger, aber kaum Sprache‘ und ‚körperlich schwach, aber extrem manipulierend‘ (`23#307A` Punkt 4). Das Lernverhalten hängt laut Entwickler „auf das monster und die intelligenz“ ab (`23#294E` Punkt 12, Antwort auf `23#293A` Punkt 12, ‚Monster können lernen‘).

---

## 5. Antriebe und Temperament

### 5.1 Antriebe

`23#307A` Punkt 5 („5. super“ in `23#308E`) und `M-PD`: Hunger, Territorialität, Selbstverteidigung, Rache, Schutz einer Person oder eines Gebietes, Fortpflanzung, Fluchzwang, Befehl eines anderen Wesens, Sadismus, Neugier, Überleben, religiös oder rituell motiviertes Verhalten, unbekannt. Ein Wesen kann **je nach Lage** aggressiv oder defensiv reagieren; ein einzelner ‚Aggro‘-Wert wird nicht verwendet.

### 5.2 Temperament

`23#307A` Punkt 6 („6. gut“ in `23#308E`): wichtige Individuen haben zusätzlich zur Art Eigenschaften wie vorsichtig, impulsiv, aggressiv, geduldig, neugierig, territorial, manipulativ, ängstlich, selbstbewusst, grausam, empathisch, kontrolliert. Das sind **interne Verhaltensfaktoren**, keine sichtbaren Werte. Die Individualität bestätigte der Entwickler ausdrücklich: „ja natürlich sonst wären alle monster gleich“ (`23#294E`, Buchstabe D; die Zuordnung ist im Letter-Versatz unsicher, Modul 21, U-K1).

Mitglieder derselben Gruppe dürfen verschiedene Temperamente haben; Gruppen sind nicht moralisch einheitlich (`23#307A` Punkt 17, siehe [Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md)).

---

## 6. Alter und Erfahrung

`23#307A` Punkt 7 („7. ja passt“ in `23#308E`) und `M-PD`:

- **Biologisches/übernatürliches Alter** und **Erfahrung** sind zwei Größen. Ein 200 Jahre altes, isoliertes Wesen kann weniger taktisch erfahren sein als ein 40 Jahre altes, das ständig gejagt wurde (Beispiel der Quelle, 💡 als Beispiel).
- Erfahrung kann umfassen: Kampf gegen Menschen, Kampf gegen Jäger, Fallenkenntnis, Kenntnis moderner Waffen, soziale Manipulation, Überleben, Beherrschung der eigenen Fähigkeiten.
- Sehr alte Wesen können gefährlicher sein (Wissen, Erfahrung, eigene Methoden, Selbstbeherrschung, seltene Fähigkeiten, teilweise Überwindung normaler Schwächen, Umgang mit Verletzungen, Menschenkenntnis), aber **nicht** durch ‚Alter × Statwerte‘.

Zur Frage, ob Alter und Erfahrung innerhalb einer Art spürbar sein sollen, antwortete der Entwickler: „ja man sollte es merken aber nicht direkt“ (`23#294E`, Antwort A auf `23#293A`, ‚Alter von Monstern‘). Dazu sein Beispiel: Versucht man, das Wesen mit der Waffe zu töten, mit der man andere getötet hat, schwächt sie es nur oder bewirkt nichts (Bezug auf die Serie, im Wortlaut siehe `23#294E`).

---

## 7. Ausnahmeindividuen

Die Quelle (`23#307A` Punkt 8) nennt als mögliche Ursachen für Außergewöhnlichkeit: extremes Alter, seltene Mutation, ursprüngliche Blutlinie, besonderes Ritual, ungewöhnlicher Ursprung, besondere Ernährung, übernatürlicher Vertrag, Erfahrung, Dimensionskontakt, individuelle Fähigkeit. Unterschiede **müssen nicht direkt sichtbar** sein.

Beispielverlauf der Quelle (💡 als Beispiel):

1. Der Spieler kennt die übliche Tötungsmethode der Art.
2. Er wendet sie an.
3. Das Wesen reagiert, wird geschwächt, stirbt aber nicht.
4. Der Spieler vermutet: ‚Irgendetwas stimmt nicht.‘
5. Er recherchiert (Hunter-Aufzeichnungen, erfahrener Jäger, Wunde untersuchen, historische Fälle vergleichen).
6. Er erfährt: Das Individuum besitzt eine Besonderheit.

Das Spiel zeigt **keine** Beschriftungen wie ‚ALPHA‘ oder ‚IMMUN‘ (`23#307A` Punkt 8; `23#295A`; `M-PD`).

**Namensfrage.** Der Entwickler sagte „alphas, also die ersten ihrer art“ (`23#1075E`, 153). Der Assistent schlug vor, aus Gründen der eigenen IP andere Begriffe zu verwenden (‚Progenitor‘, ‚Firstborn‘, ‚Primordial‘; `23#1076A`, 💡). Der Master führt die Namensfrage als offen (`M-B09`). Diese Dokumentation verwendet **Ursprungswesen** als Arbeitsbegriff (💡).

---

## 8. Einzelwesen und Art

| Aussage | Quelle | Status |
|---|---|:---:|
| Eine Art definiert Regeln, aber nicht jedes Exemplar vollständig | `23#293A` Punkt 11, „11. ja klar“ in `23#294E` | 📋 |
| Unter Individuen derselben Art gibt es solche mit persistenter Identität: Name, Alter, Gebiet, Beziehungen, Opfer, Verletzungen, Feinde, Vergangenheit | `23#1076A` (‚Individuen‘); „31. ja kann sein, sowie bei leviathanen“ in `23#1075E` | 📋 |
| Häufigere Arten werden zusätzlich auf Populationsebene simuliert | „32. ja“ in `23#1075E` | 📋 |
| Benannte, einzigartige Wesen haben eigene Archivseite, mehrere Fälle und Storyrelevanz | `23#307A` Punkt 29, „29. cool“ in `23#308E` | 📋 |
| Hauptbedrohungen sind langfristige Entitäten (über Akte), keine Ein-Mission-Bosse | `23#307A` Punkt 23, „23. toll“ in `23#308E`; `M-PD` ‚Long-term story threats‘ | 📋 |

Die Frage, welche Wesen Individuen sind und welche nur Population, ist nicht beantwortet (❓ Q-K02). Die Quellen nennen als Kriterium die Stärke und Intelligenz („Viele stärkere Monster“, `23#1074A` Punkt 31).

---

## 9. Entwurf: Datenmodell der Klassifikation

> 💡 **Vorschlag.** Er ordnet die beschlossenen Dimensionen; Feldnamen und Wertemengen sind unverbindlich.

```text
SpeciesDefinition
├── species_id            stabile ID (Modding-Namespace)
├── ip_provenance         folklore | genre | series_specific
├── existence_forms[]     physical | transformed_human | possessed_human
│                         | non_corporeal | temporary_manifestation | bound
├── origin[]              natural | cursed_human | transformed_human | created
│                         | summoned | bound_spirit | historical_anomaly
│                         | dimensional | artificial_experiment | unknown
├── intelligence          problem_solving, social, tactical, memory, language,
│                         self_control, supernatural_knowledge   (je intern abgestuft)
├── motivations[]         Antriebsmenge
├── social_patterns[]     solitary | pair | family | pack | colony | hierarchy
│                         | cult_like | master_servant | swarm
├── regeneration_model    siehe Modul 04
├── lifecycle             siehe Modul 08
├── activity_conditions   siehe Modul 08
└── knowledge_status      (pro Spieler im Spielstand, nicht in der Definition)

IndividualCreature
├── individual_id         stabile ID
├── species_id
├── biological_age, experience           getrennt
├── temperament[]
├── scars_and_injuries[]                 siehe Modul 04
├── memory[]                             siehe Modul 10
├── relationships[]                      siehe Modul 09
├── special_traits[]                     Ausnahmemerkmale (Modul 03 §7)
└── status                alive | dead | bound | contained | fled | unknown
```

**Begründungen (Quellenbezug):** Trennung von Definition und Individuum folgt `23#307A` Punkte 27–30; die getrennte Führung von Alter und Erfahrung folgt Punkt 7; die Auslagerung des Wissensstands in den Spielstand folgt Punkt 24 und `M-PD` ‚Hunter Archive epistemology‘ (Wissen gehört dem Charakter, nicht der Kreatur).

### 9.1 Mehrfachformen

Ein Wesen kann **mehrere** Existenzformen haben (zum Beispiel ‚transformierter Mensch‘ mit ‚gebundener Manifestation‘, `23#307A` Punkt 2: die Dimensionen überlappen). Die Felder sind deshalb Mengen. Die Quelle sagt ausdrücklich: Wenn ein Wesen in keine Kategorie passt, wird **das Framework erweitert**, nicht das Wesen als ‚DemonType3‘ gezwungen (`23#359A` Punkt 32, „32. super“ in `23#360E`).

---

## 10. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Jede Art hat mindestens die acht Unterscheidungen aus `23#1074A` Punkt 155 (Ökologie, Verhalten, Spuren, Schwäche, Jagdmethode, Kampf-KI, Lore, Auswirkung auf die NPC-Welt) | `23#1075E` „155 korrekt“ |
| Existenzform und Herkunft sind getrennte Felder; keine Vermischung in einem Enum | `23#307A` Punkt 3 |
| Es gibt keine sichtbare Stufe; eine Prüfung sucht in Archivdarstellung und HUD nach Feldern ‚level‘, ‚tier‘, ‚rank‘ für Kreaturen | `M-PD`, L2 |
| Keine Schwäche-/Fähigkeits-Kombination darf zur Laufzeit zufällig erzeugt werden | `23#307A` Punkt 28 |
| Erfahrung eines Individuums kann nicht ohne belegbare Quelle (Begegnung, Beobachtung) steigen | `23#307A` Punkte 7, 15 |
| Ausnahmemerkmale sind im Archiv erst nach Beobachtung sichtbar | `23#307A` Punkt 8 |

Die zugehörigen Tests stehen in [Modul 23](23-Testkatalog-und-Abnahmekriterien.md).

---

## 11. Sonderfälle

| Fall | Behandlung in den Quellen |
|---|---|
| Ein Wesen ist eigentlich ein anderes (falsche Identifikation) | Der Spieler darf auf Basis verfügbarer Informationen falsch liegen; das Spiel soll ihn aber nicht künstlich bestrafen, wenn der Schluss sinnvoll war, es muss faire Hinweise geben (`23#293A` Punkt 9, „9. ja genau“ in `23#294E`) |
| Ein Wesen ist menschlich genug, um unter Menschen zu leben | [Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md) |
| Ein Wesen ist ein Tarn-Mensch (menschlicher Täter nutzt das Übernatürliche als Tarnung) | „75 kann sehr gut sein“ in `23#1075E`; [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md) |
| Zwei Arten in einem Fall | „74 ja“ in `23#1075E`, selten |
| Ein Fall mit gleichzeitig normaler und übernatürlicher Ursache | „73 genau“ in `23#1075E` |
| Ein Fall hat gar keine übernatürliche Ursache (False Positive) | „72ja genau“ in `23#1075E` |
| Unbekannter Ursprung (‚Unknown‘ bleibt dauerhaft) | `23#307A` Punkt 3, ‚Ein bisschen echtes Mysterium‘ (`23#359A` Punkt 6) |

---

## 12. Was fehlt

- Eine Entscheidung, **welche Arten** Individuen sind (❓ Q-K02).
- Der Eigenname für das ‚erste Exemplar einer Art‘ (❓, `M-B09`).
- Wie viele Dimensionen der Intelligenz je Art überhaupt benötigt werden (❓).
- Die Abstufungen der Dimensionen (intern, Zahlenwerte bewusst nicht Teil dieses Ordners).
- Ob das Alter einer Art **sichtbar** ist (Hinweise im Aussehen) oder nur erschlossen wird (❓ Q-K03).
