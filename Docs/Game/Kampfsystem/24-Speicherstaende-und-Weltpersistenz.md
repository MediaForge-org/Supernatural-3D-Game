# Kampfsystem 24: Speicherstände und Weltpersistenz

> [!IMPORTANT]
> **Nichts davon ist implementiert, außer dem allgemeinen Weltzustand (WorldFacts, Format v1, Speicher-Slots).** Das Modul beschreibt, welche Kampfzustände persistent sein müssen, wie Speichern, Autosave und Checkpoints im Kampf geregelt sind und was offen ist. Das Speicherformat v1 enthält keinerlei Kampfdaten; es wird hier **nicht** erweitert. Größenangaben gibt es nicht. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Format: [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md); NPC-Speicherentwurf: [NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Was im Kampf persistent sein muss](#3-was-im-kampf-persistent-sein-muss)
4. [Speichern, Autosave, Checkpoint im Kampf](#4-speichern-autosave-checkpoint-im-kampf)
5. [Entwurf: Segmentierung der Kampfdaten](#5-entwurf-segmentierung-der-kampfdaten)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Der Weltzustand ist das ‚Gedächtnis der Welt‘** mit stabilen IDs; Spielstände sollen segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert sein; das Zielgrößenband etwa 20–50 MB gilt, **Zuverlässigkeit geht vor Größe** (Gameplay-Systeme §16, `E-WZ-004`, `K-PD-002`). Das Größenziel ist in der NPC-Dokumentation als Konflikt geführt (`C-N2`).
2. **Implementiert sind nur** Fakten, Journal, Format v1, historische Abfrage, atomare Slots und das sichere Laden (README §26.2). Ein segmentiertes Speichersystem, Journal-Kompaktierung, Metadaten und ein Compare-and-Set für Koop sind offen (Gameplay-Systeme §16).
3. **Kampfregeln zum Speichern sind beschlossen:** kein Speichern im aktiven Kampf, erst nach Entdeckung gesperrt, Autosaves vor und nach Kämpfen, in langen Kämpfen zwischen Phasen (`23#366E`, `23#368E`, `23#371E`).
4. **Reload-Zustände sind persistent und unterbrechbar** (`M-PD`).
5. **Hochfrequente Kampfdaten gehören nicht in die Weltfakten** (Vorschlag, `E-WZ-002`, `K-LW-008`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-SP-001 | **Ein zentraler persistenter Weltzustand ist das ‚Gedächtnis der Welt'** (Fallergebnisse, NPC lebt oder tot, Beziehungen, veränderte Orte, Wissen, Monsteraktivität, Ruf, Fahrzeuge, Unterschlupfe, Weltereignisse, Zeit) mit stabilen IDs | 📋 BESCHLOSSEN | `E-WZ-001`; `K-PD-001`; Gameplay-Systeme §16 | PER | – |
| COMBAT-DEC-SP-002 | **Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert;** Größenband etwa 20–50 MB bei langer Laufzeit, wo vernünftig; Zuverlässigkeit vor Größe | 📋 BESCHLOSSEN | Gameplay-Systeme §16; `E-WZ-004`; `K-PD-002` | PER | Widerspruch zum Größenziel: NPC-Konflikt C-N2; `CQ-104` |
| COMBAT-DEC-SP-003 | **Der Nachladezustand ist persistent und unterbrechbar;** Patronenlager und Magazine werden getrennt gespeichert | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `COMBAT-DEC-MU-012` | PER WAF | – |
| COMBAT-DEC-SP-004 | **Kein Speichern mitten im aktiven Kampf,** um nicht mit hoher Gesundheit zu speichern und einen bekannten Treffer zurückzusetzen; die Sperre gilt erst, wenn Gegner den Spieler entdeckt haben | 📋 BESCHLOSSEN | `23#366E` P1; `23#368E`; `E-QA-039`; `COMBAT-DEC-GP-028` | PER | Übergang Investigating zu Detected: `CQ-09` |
| COMBAT-DEC-SP-005 | **Autosaves liegen vor und nach Kämpfen, gelegentlich bei längerer Fahrt und in der Natur;** bei wirklich langen Kämpfen kann ein Autosave zwischen den Phasen liegen | 📋 BESCHLOSSEN | `23#368E`; `23#371E`; `COMBAT-DEC-BW-018` | PER | Kriterien der Phasen: `CQ-103` |
| COMBAT-DEC-SP-006 | **Ein Autosave vor einem Kampf entsteht, bevor der Spieler entdeckt wurde;** nach dem Kampf erst, wenn der Bedrohungszustand wirklich beendet ist | 🧭 RICHTUNG | `23#370A` (Auslegung des Assistenten); `23#368E` | PER KI | – |
| COMBAT-DEC-SP-007 | **Nach einem Gameplay-Tod kehrt das Spiel zum letzten geeigneten Checkpoint zurück** mit einem minimal überlebensfähigen Gesundheitszustand; der Spieler kann einen älteren Spielstand laden | 📋 BESCHLOSSEN | `23#364E` P2, P16; `COMBAT-DEC-BW-012`; `23#370A` | PER MED | Widerspruch zu `M-PD`: `CK-01` |
| COMBAT-DEC-SP-008 | **Safe-Checkpoint-Validator** (kein unmittelbarer Angriff, keine tödliche Projektilbahn, kein Griff oder Takedown, keine unmittelbare Explosion, keine nicht reproduzierbare Transition, Encounter-Zustand reproduzierbar) | 💡 VORSCHLAG | `23#367A` | PER TST | – |
| COMBAT-DEC-SP-009 | **Softlock-Schutz:** kein unendlicher Todes-Loop; ein älterer sicherer Checkpoint ohne kostenlose Heilung | 🧭 RICHTUNG | `COMBAT-DEC-BW-016`; `23#365A` | PER | – |
| COMBAT-DEC-SP-010 | **Fallen bleiben persistent** und verändern sich durch Umwelt, NPCs, Monster, Spieler und Zeit | 📋 BESCHLOSSEN | `K-CO-004` | PER KRE | – |
| COMBAT-DEC-SP-011 | **Wichtige abgelegte Waffen, Magazine, Leichen und Tatorte bleiben als Weltobjekte** bis zur Bereinigung | 📋 BESCHLOSSEN | `COMBAT-DEC-WA-007`, `COMBAT-DEC-PO-003`, `-004`; `E-QA-023` | PER | Bereinigungsregel offen |
| COMBAT-DEC-SP-012 | **Individuelle Wesen sind persistent** (Name, Gebiet, Beziehungen, Verletzungen, Feinde, Vergangenheit, Gedächtnis); der Tod eines NPCs bleibt dauerhaft | 📋 BESCHLOSSEN | `K-OE-001`; `N-NE-011`; `N-LB-018` | PER NPC | – |
| COMBAT-DEC-SP-013 | **Körperzustand, Wundeinträge und Behandlungsstand von Spieler und persistenten NPCs bleiben im Spielstand;** nicht jeder Zwischenzustand der Ermüdung | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-VL-005`, `COMBAT-DEC-NM-012`, `COMBAT-DEC-ZA-016` | PER MED | Umfang: `CQ-104` |
| COMBAT-DEC-SP-014 | **Hochfrequenter Kampfzustand (Projektile im Flug, Animationszeit, Trefferdetails) wird nicht im Weltzustandsjournal geführt,** nur langfristig bedeutsame Folgen | 💡 VORSCHLAG | `E-WZ-002`; `K-LW-008`; `COMBAT-DEC-ZA-016` | PER PRF | – |
| COMBAT-DEC-SP-015 | **Zufallszustand und Seeds** werden mitgespeichert, damit Wiederholungen und Fehlerberichte reproduzierbar sind | 💡 VORSCHLAG | Gameplay-Systeme §17.2 (Zufall über Seeds und Replays reproduzierbar); `COMBAT-DEC-ZA-008` | PER TST | `CQ-105` |
| COMBAT-DEC-SP-016 | **Neue Datensatzarten (Mods) werden additiv registriert;** ein Spielstand mit unbekannten Arten lädt unverändert und speichert wieder | 💡 VORSCHLAG | `N-KO-017` (Proposal 0010); `E-WZ-006` | MOD PER | – |
| COMBAT-DEC-SP-017 | **Journal-Kompaktierung, Metadaten, Reichstrennung und Compare-and-Set für Koop** sind nicht entschieden | ❓ OFFEN | Gameplay-Systeme §16; `E-WZ-003`; `N-KO-018` | PER KOO | `CQ-104` |
| COMBAT-DEC-SP-018 | **Permadeath-Speicherstände:** Behandlung von Backups, Zeitpunkt des Löschens, Mehrfachspeicher | ❓ OFFEN | `COMBAT-DEC-BW-013`; Suche (keine Fundstelle) | PER | `CQ-102` |
| COMBAT-DEC-SP-019 | **Was in einem Zwischen-Checkpoint eines langen Kampfes gespeichert wird** (Gegnerzustand, Taktikzustand, Projektile) ist nicht beschrieben | ❓ OFFEN | `23#372A` (Beispiel, Entwurf) | PER KI | `CQ-103` |
| COMBAT-DEC-SP-020 | **Kampf-Replays und Fehlerberichte im Spielstand** (Seed, Eingaben, Zustand) sind gewünscht, aber nicht festgelegt | 🧭 RICHTUNG | Gameplay-Systeme §17.2; `M-PD` ‚Testing philosophy‘ | TST PER | `CQ-105` |

---

## 3. Was im Kampf persistent sein muss

| Bereich | Persistent? | Begründung (Quelle) |
|---|---|---|
| Körperzustand des Spielers (Wunden, Blutvolumen, Schmerz, Genesung) | ja | Heilung braucht Zeit (`COMBAT-DEC-VL-001`) |
| Ausrüstung, Magazine, Waffenzustand | ja | `COMBAT-DEC-SP-003` |
| Nachladezustand | ja | `M-PD` |
| Tote Körper, Waffen am Boden, Einschüsse, Blut | ja bis Bereinigung | Beweise (`COMBAT-DEC-PO-003`) |
| Fallen | ja | `COMBAT-DEC-SP-010` |
| Individuen (Wesen, Rivalen, wichtige NPCs) | ja | `COMBAT-DEC-SP-012` |
| Gedächtnis aus dem Kampf | ja bei persistenten Figuren | `COMBAT-DEC-NM-002` |
| Folgen (Tod, Verletzungen, Strukturen) | ja, im Journal | `PE-N1022` E |
| Fahndungs- und Rechtsstand | ja | Ermittlung 14, 19 |
| Projektile im Flug | nein | Entwurf (`COMBAT-DEC-SP-014`) |
| Animationszeit, Trefferreaktion | nein | Entwurf |
| Aktuelle Ermüdung in Sekunden | nein, nur Fitnessniveau und grober Zustand | Entwurf (`COMBAT-DEC-SP-013`) |

---

## 4. Speichern, Autosave, Checkpoint im Kampf

```text
Spieler unentdeckt          -> manuelles Speichern erlaubt
Gegner misstrauisch/sucht   -> Speichern noch möglich, solange kein Detected-Zustand (23#370A)
Gegner entdeckt, Kampf aktiv-> manuelles und Quick-Save gesperrt (GP-028)
Phasenwechsel (langer Kampf)-> Autosave nur bei stabilem, reproduzierbarem Zustand (23#372A)
Kampf vorbei (Bedrohung weg)-> Autosave erlaubt
Gameplay-Tod                -> letzter geeigneter Checkpoint, Mindestzustand (364E)
Kein sicherer Checkpoint    -> Softlock-Schutz (älterer Checkpoint / eigener Spielstand)
```

Die Zeilen zu Speichersperre, Autosave und Gameplay-Tod stehen in den Quellen; die Übergangsschwelle ‚Detected‘ legt der Assistent ausdrücklich als später technisch festzulegen fest (`23#370A`).

**Prüfbare Eigenschaften (🧩 ABGELEITET):** (1) Es gibt keinen Weg, einen bekannten Treffer durch Speichern und Laden zu umgehen, ohne Zustandsverlust. (2) Ein Laden nach Tod ergibt keinen vollständig geheilten Körper. (3) Ein Spielstand, der im Kampf gespeichert wurde (Phasen-Checkpoint), lädt in einen reproduzierbaren Zustand. (4) Wiederholtes Laden desselben Zustands mit demselben Seed führt zu demselben Ergebnis.

---

## 5. Entwurf: Segmentierung der Kampfdaten

> **💡 VORSCHLAG.** Der Entwurf folgt dem Prinzip der segmentierten Speicherung (Gameplay-Systeme §16) und enthält **keine Größenangaben**, weil die Quellen keine nennen und die NPC-Dokumentation ihre Schätzungen ausdrücklich als Annahmen führt.

| Segment | Inhalt | Besitzer | Änderungsrhythmus |
|---|---|---|---|
| Spielerkörper | Wunden, Blutvolumen, Genesungsstufen, Fitnessniveau | Spieler | bei Verletzung, Behandlung, Zeit |
| Ausrüstung | Waffeninstanzen, Magazine, Zustände, Trageorte | Spieler | bei Gebrauch |
| Weltobjekte aus Kämpfen | abgelegte Waffen, Magazine, Leichen, Blut, Fallen | Weltzustand | bei Kampfereignissen, Bereinigung |
| Individuen | Narben, Verletzungen, Gedächtnis aus Kämpfen | jeweilige Figur | bei Begegnung, Weltzeit |
| Folgen | Tod, bleibende Verletzung, Zerstörung | Journal | bei Ereignis |
| Rechts-/Fahndungsstand | Zeugen, Beweise, Fahndung | Ermittlung | bei Ereignis |
| Replay-Daten (optional) | Seed, Eingaben | Test | bei Aufzeichnung |

**Regel (💡):** Jedes Segment hat eine Version und eine Migrationsregel; unbekannte Segmente werden erhalten (`COMBAT-DEC-SP-016`).

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Speicherort der Kampffolgen (`CQ-104`) | A: Alle Folgen als WorldFacts | einheitlich, journalbasiert | Journal wächst; Hochfrequentes ungeeignet |
| | B: Eigene Segmente für Körper und Objekte, Journal nur für Folgen | skalierbar | zwei Systeme |
| Speichern in langen Kämpfen (`CQ-103`) | A: Nur Ergebnis jeder Phase (Gegner tot, Fortschritt) | klein | Zustand nach Laden vereinfacht |
| | B: Vollständiger Gegnerzustand | genau | groß, schwer reproduzierbar |
| Permadeath (`CQ-102`) | A: Spielstand wird beim Tod gelöscht | konsequent | Datenverlust bei Fehlern |
| | B: Spielstand wird gesperrt, Backup bleibt (Sicherheit) | sicher | nicht „perma“ |
| Replays (`CQ-105`) | A: Immer aufzeichnen | Fehlerdiagnose | Speicher |
| | B: Nur in Entwickler-Builds | günstig | Spieler-Fehler schwerer reproduzierbar |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Weltzustand (Decision 0004) | Folgen als Fakten |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Host-Spielstand |
| zur Performance ([Modul 25](25-Performance-Skalierung-und-Simulationsstufen.md)) | Segmentgrößen, Laden |
| zu Mods | additive Arten |
| zu Tests ([Modul 30](30-Testkatalog-und-Abnahmekriterien.md)) | Golden-Dateien, Roundtrip |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Absturz beim Speichern | atomares Schreiben; alter Stand bleibt | ✅ Bestand (README §26.2: atomares Schreiben) |
| Beschädigte Datei | sicheres Laden lehnt ab (`CorruptData`) | ✅ Bestand |
| Spielstand mit unbekannter Kampf-Segmentversion | Meldung oder additive Behandlung | 💡 |
| Mod entfernt, Spielstand verweist darauf | kritisch fehlend blockiert das Laden (`N-KO-013`) | 📋 |
| Laden während unterbrochenem Nachladen | Handlung wird fortgesetzt | 📋 |
| Tod ohne Checkpoint | Softlock-Schutz | 🧭 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-102` | Permadeath: Löschung, Backup, Mehrfachspeicher | `COMBAT-DEC-SP-018` | ja |
| `CQ-103` | Zwischen-Checkpoints in langen Kämpfen: Phasenkriterien und gespeicherter Umfang | `COMBAT-DEC-SP-005`, `-019` | ja |
| `CQ-104` | Speicherbudget und Verdichtung der Kampfdaten; Verhältnis zu Journal-Kompaktierung und Größenband | `COMBAT-DEC-SP-002`, `-013`, `-017` | ja |
| `CQ-105` | Replay-Daten und Seeds im Spielstand | `COMBAT-DEC-SP-015`, `-020` | ja |
