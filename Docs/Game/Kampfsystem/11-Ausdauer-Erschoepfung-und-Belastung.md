# Kampfsystem 11: Ausdauer, Erschöpfung und Belastung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul sammelt, was die Quellen über Ausdauer, Ermüdung, Atmung und Belastung im Kampf festlegen, und führt die **Formeln als offen**, wie es der Master tut. Es nennt keine Ausdauerwerte, keine Regenerationsraten und keine Schwellen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Das Körpermodell im Allgemeinen steht in [Gameplay-Systeme §9](../Gameplay-Systeme.md#9-körper-fortschritt-und-anpassung); hier geht es nur um die Kampfseite.

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Belastungsquellen im Kampf](#3-belastungsquellen-im-kampf)
4. [Gegenüberstellung der Ausdauermodelle](#4-gegenüberstellung-der-ausdauermodelle)
5. [Ausdauer, Verletzung und Adrenalin](#5-ausdauer-verletzung-und-adrenalin)
6. [Schnittstellen und Fehlerfälle](#6-schnittstellen-und-fehlerfälle)
7. [Offene Punkte dieses Moduls](#7-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Ausdauer ist Teil eines Körpermodells,** nicht eines eigenen Kampfbalkens: Fitness, Kraft, Ausdauer, Beweglichkeit, Ermüdung und Erholung sind interne Größen, die Training, Ernährung, Schlaf, Stress, Alkohol, Rauchen, Müdigkeit und Verletzung beeinflussen (Gameplay-Systeme §9.2, `M-PD` ‚Body / physical response / recoil‘).
2. **Die Kampfseite ist qualitativ gesetzt:** Langes Halten einer Waffe ermüdet moderat, nicht nach Sekunden; schwere Nahkampfwaffen belasten mehr; ein trainierter Hunter ermüdet nicht nach drei Schlägen; Verletzung und Blutverlust wirken mit (`23#116E` P21, `23#162A` P5, `23#163E` P5).
3. **Ausdauer wächst durch Benutzung mit abnehmenden Erträgen** (Sprinten verbessert die Ausdauer; zu häufiges Überlasten baut sie langsamer auf; `23#116E` P16, `23#118E` P21). Das Gesamtmodell der Progression ist offen (`CQ-06`).
4. **Alle Formeln sind offen:** Entscheidungsstatus §5.3 und `M-OD` führen Körperzusammensetzung, Kraft, Kardio, Müdigkeit, Erholung, Flüssigkeit, Schlaf, Stress und Ernährung als nicht festgelegt.
5. **Nicht in den Quellen:** ob es einen sichtbaren Ausdauerbalken gibt, wie Traglast wirkt und welche Schwellen Handlungen verlangsamen oder verbieten (`CQ-42`, `CQ-43`, `CQ-44`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-AU-001 | **Ausdauer ist eine interne Körpergröße** neben Fitness, Kraft, Beweglichkeit, Ermüdung und Erholung (Anpassungsmodell mit regionalen Muskelgruppen); Kraft ist nicht identisch mit sichtbarer Muskelgröße | 📋 BESCHLOSSEN | Gameplay-Systeme §9.2; `M-PD` ‚Body / physical response / recoil‘ | MED | Formeln offen (AU-012) |
| COMBAT-DEC-AU-002 | **Training, Ernährung, Schlaf, Stress, Alkohol, Rauchen, Müdigkeit und Verletzung beeinflussen den Körper;** Training, Klettern und Alltag wirken passiv | 📋 BESCHLOSSEN | Gameplay-Systeme §9.2 | MED | – |
| COMBAT-DEC-AU-003 | **Langes Halten einer Langwaffe ermüdet moderat,** nicht nach wenigen Sekunden | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P21 | WAF | `COMBAT-DEC-FK-009` |
| COMBAT-DEC-AU-004 | **Nahkampf kostet Ausdauer;** schwere Waffen und wiederholte Schläge belasten mehr, die Wirkung (Geschwindigkeit, Präzision, Kraft) sinkt; ein trainierter Hunter kämpft trotzdem länger | 📋 BESCHLOSSEN | `23#162A` P5; `23#163E` P5; `COMBAT-DEC-NK-029` | AU STE | – |
| COMBAT-DEC-AU-005 | **Ausdauer entsteht durch Benutzung** (Sprinten); **abnehmende Erträge** verhindern Ausnutzen; zu häufiges Überlasten baut sie langsamer auf | 🧭 RICHTUNG | `23#116E` P16; `23#118E` P21; `M-PD` ‚Progression — OPEN…‘ | AU | `COMBAT-DEC-GP-024`, `-025`, `CQ-06` |
| COMBAT-DEC-AU-006 | **Atmung, Ermüdung und Anstrengung beeinflussen das Zielen** (Visierung unruhiger nach einem Sprint, ruhiger nach kurzer Pause); abschaltbar, auf Grounded uneindeutig | 🧭 RICHTUNG | `23#115A` P7; `23#116E` P7; `COMBAT-DEC-FK-006` | WAF | `CU-02` |
| COMBAT-DEC-AU-007 | **Verletzte Gliedmaßen wirken sich auf Hinken, Sprinten, Waffeneinsatz, Nachladen, Gleichgewicht, Atmung und Ausdauer aus;** Blutverlust senkt Kraft und beschleunigt die Erschöpfung | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#162A` P15; `COMBAT-DEC-NK-026` | MED | – |
| COMBAT-DEC-AU-008 | **Adrenalin unterdrückt wahrgenommenen Schmerz zeitweise;** nach dem Ende der Gefahr verschlechtern sich Schmerz, Zittern, Beweglichkeit und Präzision | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#101E` P7 (‚ja das ist auch gut, aber schwer umzusetzen wahrscheinlich‘) | MED | Zeitverläufe: Modul 13 |
| COMBAT-DEC-AU-009 | **Erschöpfung darf Traversal später beeinflussen** (Springen, Aufstieg) | 📋 BESCHLOSSEN | Proposal 0009 §3a | BEW | `COMBAT-DEC-BT-006` |
| COMBAT-DEC-AU-010 | **Alkohol beeinflusst Reaktion, Gleichgewicht, Wahrnehmung, Dialog, Fahrfähigkeit, Schlaf und Erholung;** Drogen haben eigene Wirkungen und Risiken | 📋 BESCHLOSSEN | Gameplay-Systeme §9.3 | MED | Wirkung im Kampf: aus Reaktion und Gleichgewicht abgeleitet |
| COMBAT-DEC-AU-011 | **Schlaf hat Folgen,** ohne ein ständiges Meter-Management zu werden; Nahrung ist optionaler Lebensstil, kein Pflicht-Hungerkreis | 📋 BESCHLOSSEN | `M-CA0905` (Spielergesundheit, Schlaf); Gameplay-Systeme §9.3 | MED | – |
| COMBAT-DEC-AU-012 | **Formeln und Grenzen für Körperzusammensetzung, Muskelzuwachs und -verlust, Kraft, Kardio, Müdigkeit, Erholung, Flüssigkeit, Schlaf, Stress und Ernährung sind nicht festgelegt** | ❓ OFFEN | Gameplay-Systeme §9.2 (letzte Zeile); Entscheidungsstatus §5.3; `M-OD` ‚Progression / body / survival‘ | MED AU | `CQ-43` |
| COMBAT-DEC-AU-013 | **Die Oberfläche kommuniziert den Körperzustand,** ohne das Spiel zur Tabellenkalkulation zu machen; ob es eine Ausdaueranzeige gibt, ist nicht entschieden | ❓ OFFEN | Gameplay-Systeme §9.2; `M-PD` ‚Human anatomy…‘ (Spieler muss Zustand spüren); `23#118E` P22 (nur Munitions-HUD) | UI | `CQ-42` |
| COMBAT-DEC-AU-014 | **Traglast** (Gewicht und Menge der getragenen Ausrüstung) und ihr Einfluss auf Bewegung, Geräusch und Ausdauer sind nicht entschieden; die Quellen nennen nur ‚begrenzten Stauraum‘ | ❓ OFFEN | Gameplay-Systeme §9.3 (Inventar); Proposal 0007 §2 (I1); `23#225A` (Schrittgeräusch abhängig von ‚Körpergewicht/Ausrüstung‘, Entwurf) | AU WAF | `CQ-44` |
| COMBAT-DEC-AU-015 | **Kälte, Hitze und Dehydrierung** sind Teil der Spielergesundheit; ihre Wirkung im Kampf folgt aus Beweglichkeit und Konzentration | 📋 BESCHLOSSEN | `M-CA0905` (Spielergesundheit); Gameplay-Systeme §4 (Kälte, Elektrizität) | MED | Werte offen |
| COMBAT-DEC-AU-016 | **Körperliche Unterschiede (zum Beispiel Geschlecht) erzeugen keinen pauschalen Nachteil;** Leistung folgt dem tatsächlichen Körper-, Trainings- und Verletzungszustand | 📋 BESCHLOSSEN | Spielvision §6.1; `COMBAT-DEC-NK-034` | MED | – |

---

## 3. Belastungsquellen im Kampf

Die Tabelle ordnet alle Belastungsquellen, die die Quellen nennen. Die Spalte ‚Wirkung‘ ist qualitativ.

| Quelle der Belastung | Wirkung nach Quellen | Status |
|---|---|---|
| Sprinten | Ermüdung, Atmung, Visierung unruhiger; trainiert Ausdauer | 🧭 |
| Halten einer Langwaffe | moderate Ermüdung über Zeit | 📋 |
| Schwingen schwerer Nahkampfwaffen | hoher Ausdauerverbrauch, langsamere und schwächere Schläge | 📋 |
| Griffkampf (Ringen, Druck halten) | ermüdet; hängt von Kraft, Technik, Position ab (Schätzung: Ausdauer und Balance beeinflussen den Erfolg) | 📋 (Erfolgsfaktoren `COMBAT-DEC-NK-009`) |
| Treten, Schlagen, Ausweichen | Ausdauerverbrauch im Verhältnis zur Aktion | 📋 (`23#162A` P5) |
| Tragen von Verletzten, Helfen | Interaktion „Tragen“ (Gameplay-Systeme §3.2); Belastung nicht beschrieben | ❓ |
| Verletzung, Blutverlust, Schmerz, Schock | senkt Kraft, Reaktion, Ausdauer | 📋 |
| Kälte, Hitze, Dehydrierung | Beweglichkeit, Konzentration | 📋 |
| Alkohol, Drogen | Reaktion, Gleichgewicht, Wahrnehmung | 📋 |
| Traglast | nicht beschrieben | ❓ |

---

## 4. Gegenüberstellung der Ausdauermodelle

Die Quellen nennen kein Ressourcenmodell; sie verlangen ein Körpermodell mit mehreren Größen. Die Gegenüberstellung bereitet `CQ-42` und `CQ-43` vor.

| Modell | Beschreibung | Vorteil | Nachteil | Bezug zu Quellen |
|---|---|---|---|---|
| A: Ein Ausdauerbalken | eine Ressource, die Aktionen verbrauchen und die sich erholt | verständlich, einfach zu testen | arcadig; widerspricht ‚Körpermodell‘ und ‚Spiel nicht zur Tabellenkalkulation‘ | Gameplay-Systeme §9.2 |
| B: Verteilte Ermüdung | Atmung, lokale Muskelermüdung je Region, Kondition, Herzfrequenz-Näherung; Handlungen werden langsamer oder verweigert, wenn die zuständige Größe erschöpft ist | passt zu Gliedmaßen- und Regionenmodell | hohe Komplexität, schwer zu balancieren | Gameplay-Systeme §9.2; `M-PD` ‚Human anatomy…‘ |
| C: Verdecktes Modell mit abgeleiteten Zuständen | interne Größen wie B, nach außen nur Zustände (außer Atem, zitternd, erschöpft) und Animation, Audio, Atmung | passt zu ‚spürbar über Animation, Audio, Atmung‘ (`M-PD`) | Nachvollziehbarkeit für den Spieler; braucht gute Rückmeldung | `M-PD` ‚Human anatomy…‘ |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen. Für alle Modelle gilt: kein Training durch Dauer-Wiederholung ohne abnehmende Erträge (`COMBAT-DEC-GP-025`), Zufall deterministisch.

---

## 5. Ausdauer, Verletzung und Adrenalin

Die Quellen verbinden Ausdauer, Schmerz und Adrenalin zu einer Abfolge (`M-PD`, `23#86E` P6, `23#101E` P7):

```text
Gefahr steigt -> Adrenalin steigt -> Schmerz teilweise unterdrückt -> mehr Handlungsfähigkeit
Gefahr endet  -> Adrenalin fällt  -> Schmerz tritt stärker hervor -> Zittern, Erschöpfung, schlechtere Präzision
```

**Prüfbare Eigenschaften (🧩 ABGELEITET aus `COMBAT-DEC-AU-008`):**

1. Dieselbe Verletzung wirkt während des Kampfes schwächer wahrgenommen als danach (zeitliche Verschiebung der Wahrnehmung, nicht des Schadens: ‚Schmerz ist vom physischen Schaden getrennt‘, `M-PD`).
2. Der physische Zustand (Blutverlust, Funktionsverlust) bleibt unabhängig vom Adrenalin bestehen; nur die Wahrnehmung und die Handlungsfähigkeit verschieben sich.
3. Der Verlauf ist deterministisch bei gleichem Seed.

---

## 6. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Körpermodell ([Modul 12](12-Treffermodell-und-Koerperphysik.md), [13](13-Verletzungen-Schmerz-und-Kampffolgen.md)) | Ausdauergrößen als Teil des Körperzustands |
| zum Nahkampf ([Modul 04](04-Nahkampf-und-Kampfstile.md)) | Kosten je Technik |
| zur Verteidigung ([Modul 07](07-Ausweichen-Blocken-Parieren-und-Konter.md)) | Kosten und Erholung der Verteidigung |
| zum Fernkampf ([Modul 05](05-Fernkampf-und-Schusswaffen.md)) | Zielunruhe |
| zur Traversal ([Modul 08](08-Kampfbewegung-und-Traversal-Integration.md)) | Sprünge, Aufstiege |
| zur Persistenz ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | gespeicherte Größen (Fitness, nicht jede Sekunde der Ermüdung) |
| zur Progression | Training durch Benutzung (`CQ-06`) |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Erschöpfung auf Null im Kampf | Handlungen werden langsamer oder abgelehnt, nicht stumm unwirksam | 💡 |
| Training durch Wiederholung derselben kurzen Handlung | abnehmender Ertrag | 🧭 |
| Extreme Verletzung bei gleichzeitigem Adrenalin | Handlungsfähigkeit folgt dem Zustand, nicht nur der Wahrnehmung | 💡 |
| Speichern bei halb erholtem Körper | Größen werden gespeichert und deterministisch fortgeschrieben | 💡 |
| Koop: Helfer trägt Verletzten | Belastung und Autorität nach Regel (offen) | ❓ |

---

## 7. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-42` | Ausdaueranzeige und Ressourcenmodell: sichtbarer Balken, verdeckte Zustände, abgeleitete Rückmeldung | `COMBAT-DEC-AU-013` | ja |
| `CQ-43` | Formeln, Schwellen und Erholung von Ausdauer, Kraft, Kardio, Müdigkeit | `COMBAT-DEC-AU-012` | ja |
| `CQ-44` | Traglast und Ausrüstungsgewicht: Einfluss auf Bewegung, Geräusch, Ausdauer | `COMBAT-DEC-AU-014` | ja |
