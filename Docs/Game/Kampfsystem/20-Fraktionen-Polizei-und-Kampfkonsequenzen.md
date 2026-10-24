# Kampfsystem 20: Fraktionen, Polizei und Kampfkonsequenzen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul beschreibt, welche Folgen ein Kampf in Gesellschaft, Recht und Fraktionswelt hat: Zeugen, Spuren, Beweise, Notruf, Fahndung, Festnahme, Selbstverteidigung, Fight-Club-Verstöße, Rache. Die Entscheidungen liegen in der [Ermittlungs-](../Ermittlung/README.md) und [NPC-Dokumentation](../NPCs/README.md); hier steht nur die **Kampfseite**. Der Master führt Straftatenkatalog, Festnahmeschwellen, Strafmaß und Beweiswertung als **offen**. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Vertiefung: [Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md), [14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md), [05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Spuren und Beweise eines Kampfes](#3-spuren-und-beweise-eines-kampfes)
4. [Zeugen, Notruf, Fahndung, Festnahme](#4-zeugen-notruf-fahndung-festnahme)
5. [Selbstverteidigung und Fight Club](#5-selbstverteidigung-und-fight-club)
6. [Fraktionen, Rache, Ruf](#6-fraktionen-rache-ruf)
7. [Gegenüberstellung offener Lösungsansätze](#7-gegenüberstellung-offener-lösungsansätze)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Das Gesetz folgt tatsächlicher Information:** Zeugen, Kameras, Kleidung, Gesicht, Fahrzeug, Kennzeichen, Waffen und Ballistik, Fingerabdrücke und DNA, digitale Spuren. Es gibt keine allwissende Fahndung und kein Sternesystem (`E-RE-001`, `E-RE-002`, `E-RE-013`).
2. **Festnahme ist kein Game Over.** Verhör, Strafen, Gericht, Gefängnis und vergehende Zeit können auftreten (`E-RE-015`).
3. **Waffen und Leichen bleiben in der Welt;** die Polizei sichert Beweise, eine intelligente Bereinigung verhindert unbegrenzten Müll (`E-QA-023`, `E-RE-024`).
4. **Selbstverteidigung** ist unabhängig vom Geschlecht, Handlungen werden bewertet (`E-RE-018`).
5. **Fight-Club-Verstöße** haben ernste soziale, rechtliche und Spielfolgen (`COMBAT-DEC-NK-013`).
6. **Offen:** Straftatenkatalog, Festnahmeschwellen, Strafmaß, Zuständigkeitskarte, Beweiswertung, Durchsuchungsregeln (`M-OD` ‚Crime / law / evidence‘), die Rechtslage bei Waffenbesitz und die Frage, wer wichtige Figuren töten darf (`CQ-79`, `CQ-81`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-PO-001 | **Das Gesetz folgt tatsächlicher Information und Beweiskonfidenz über Zuständigkeiten hinweg** (lokal, Bezirk, Bundesstaat, Bund), nicht omniszienter Fahndungslogik | 📋 BESCHLOSSEN | `E-RE-001`; `M-PD` ‚Cash / law / evidence‘; Gameplay-Systeme §10 | POL ERM | – |
| COMBAT-DEC-PO-002 | **Beweisquellen:** Zeugen, Überwachungskameras, Kleidung, Gesicht, Fahrzeug und Kennzeichen, Waffen und Ballistik, Fingerabdrücke, DNA wo passend, digitale Beweise | 📋 BESCHLOSSEN | `E-RE-002`; `M-PD` | POL ERM | Katalog der Spuren: [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) §3 |
| COMBAT-DEC-PO-003 | **Wichtige abgelegte Waffen und Gegenstände bleiben physisch und können Beweise werden;** eine intelligente Bereinigung verhindert unbegrenzten Müll | 📋 BESCHLOSSEN | `E-RE-024`; `E-QA-023`; `M-PD` | PER POL | Bereinigungsregel offen |
| COMBAT-DEC-PO-004 | **Tatorte bleiben bis zur Reinigung oder Reparatur bestehen** | 📋 BESCHLOSSEN | `E-SP-013` | PER ERM | – |
| COMBAT-DEC-PO-005 | **Zeugenaussagen sind keine garantierte Wahrheit;** NPCs wissen nur, was sie erfahren haben; Spieler, Fahrzeug und Kleidung können wiedererkannt werden | 📋 BESCHLOSSEN | `E-SP-010`, `E-SP-011`; `E-ZE-002`, `E-ZE-003` | NPC ERM | – |
| COMBAT-DEC-PO-006 | **Der Spieler kann Zeuge werden** (schweigen, melden, bestochen oder bedroht werden) | 📋 BESCHLOSSEN | `E-RE-005` | NPC POL | – |
| COMBAT-DEC-PO-007 | **Notruf mit Latenz, Täterbeschreibung, Kennzeichenabfrage;** Polizei reagiert auch ohne den Spieler | 📋 BESCHLOSSEN | `E-RE-010`; `N-BH-009` | POL NPC | – |
| COMBAT-DEC-PO-008 | **Schwere Fahndung bleibt bis zur glaubwürdigen Auflösung;** man kann landesweit gesucht werden; kein Sternesystem; Kopfgeld als Bedingung für das Ende | 📋 BESCHLOSSEN | `E-RE-012`, `E-RE-013`, `E-PO-007` | POL | Widerspruch Quiz-Text: `E-PO-003` ⚠️ |
| COMBAT-DEC-PO-009 | **Festnahme ist kein automatisches Game Over;** Strafkette von Tat über Schweregrad, Vorsatz, Opfer, Zeugen und Beweise, Notwehr, Vorstrafen, Festnahme, Staatsanwaltschaft, Prozess oder Absprache, Urteil | 📋 BESCHLOSSEN | `E-RE-014`, `E-RE-015`; Gameplay-Systeme §10 | POL | [Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md) |
| COMBAT-DEC-PO-010 | **Aufgeben und Flucht sind Wege bei Kontakt mit der Polizei;** Festnahme statt Flucht ist ein Vorschlag, nicht widersprochen | 🧭 RICHTUNG | Gameplay-Systeme §10; `E-QA-021` | POL | – |
| COMBAT-DEC-PO-011 | **Selbstverteidigung ist unabhängig vom Geschlecht;** Handlungen werden bewertet | 📋 BESCHLOSSEN | `E-RE-018` (`35#533A`, `35#532E`); Gameplay-Systeme §10 | POL | Bewertungsregeln offen (`CQ-82`) |
| COMBAT-DEC-PO-012 | **NPCs, die ein schweres Verbrechen beobachten, können Polizei rufen, eingreifen, festhalten, fliehen, angreifen oder massiv eskalieren,** je nach Persönlichkeit, Fraktion und Moral | 📋 BESCHLOSSEN | Gameplay-Systeme §10; `E-RE-019` | NPC KI | – |
| COMBAT-DEC-PO-013 | **Die Polizei ist nicht allwissend;** bei Verdacht kann ein Verhör des Spielers erfolgen | 📋 BESCHLOSSEN | `N-BH-002`; `E-ZE-009` | POL | – |
| COMBAT-DEC-PO-014 | **Die Polizei erklärt Vorfälle zunächst normal;** die meisten Polizisten wissen nichts vom Übernatürlichen; kleine geheime Einheiten kennen Teile | 📋 BESCHLOSSEN | `E-RE-022`; `N-BH-008` | POL KRE | Folge: Wesen- und Monsterkämpfe haben „normale“ Erklärungen |
| COMBAT-DEC-PO-015 | **Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen;** bestimmte kriminelle Gruppen können wissen, wer der Spieler ist | 📋 BESCHLOSSEN | `N-NE-018`; `N-BH-007` | POL NPC | – |
| COMBAT-DEC-PO-016 | **Kriminalität läuft unabhängig vom Spieler;** Gangkonflikte, Festnahmen, Absperrungen nach größeren Verbrechen | 📋 BESCHLOSSEN | `E-RE-003`; `E-PO-008`; `N-TE-007` | NPC POL | – |
| COMBAT-DEC-PO-017 | **Fight-Club-Verstöße** (absichtliches Töten, schwere vermeidbare Verletzung, Weiterwürgen) haben ernste soziale, rechtliche und Spielfolgen | 📋 BESCHLOSSEN | `COMBAT-DEC-NK-013`; `M-PD` ‚Melee / grappling‘ | POL NPC | Details 💡 (`COMBAT-DEC-NK-014`) |
| COMBAT-DEC-PO-018 | **Schwere Straftaten sind nur Konsequenzsystem** (Zeugen, Spuren, Ermittlung, Festnahme, Prozess, Haft, Ruf), keine eigene Spielmechanik | 📋 BESCHLOSSEN | `E-RE-020`; Gameplay-Systeme §10 | POL | – |
| COMBAT-DEC-PO-019 | **Nur interne Entwicklerbuilds dürfen Crime-Pfade bis zur Haft testen,** nicht die Veröffentlichung | 📋 BESCHLOSSEN | `E-HA-011` | TST | Testkatalog beachten |
| COMBAT-DEC-PO-020 | **Wichtige NPCs sterben nur durch den Spieler** (Aussage des Entwicklers zur Welt); Spannung zur unabhängigen Lebensentwicklung und zu Fraktionskämpfen | ⚠️ KONFLIKT | `E-ZE-007`; NPC-Konflikt C-N1; `COMBAT-DEC-GP-007` | NPC | `CK-02`, `CQ-79` |
| COMBAT-DEC-PO-021 | **Fraktionen, Wesen und Hunter können Rache nehmen** (Gedächtnis, Rufweitergabe) | 🧭 RICHTUNG | `N-NE-002`, `N-NE-018`; `PE-N1022` C, D | NPC KI | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-PO-022 | **Kampfspuren als Katalog** (Blut, Hülsen, Einschüsse, Waffen, Kleidungsschäden, Kampfspuren am Körper, Zeugen, Kameras) | 💡 VORSCHLAG | Abschnitt 3 (Ableitung aus `E-SP-002`, `E-SP-007`, `E-RE-002`) | ERM | – |
| COMBAT-DEC-PO-023 | **Straftatenkatalog, Festnahmeschwellen, Strafmaß, Kaution, Zuständigkeitskarte, Überwachungsdichte, Beweiswertung, Durchsuchungsregeln** sind nicht entschieden | ❓ OFFEN | `M-OD` ‚Crime / law / evidence‘; Gameplay-Systeme §10 (letzte Zeile) | POL | `CQ-83` |
| COMBAT-DEC-PO-024 | **Waffenbesitz, offenes Tragen, illegale Waffen und die Meldung von Schussverletzungen** im Recht sind nicht entschieden | ❓ OFFEN | `23#96A` (Aufgabe ‚Waffen offen tragen‘); `M-OD` | POL WAF | `CQ-81` |
| COMBAT-DEC-PO-025 | **Polizeireaktion auf Gewalt im öffentlichen Raum** (Eskalationsstufen, Kontaktregeln, tödliche Gewalt der Polizei gegenüber dem Spieler) ist nicht beschrieben; beschlossen ist nur, dass Festnahme und Aufgabe Alternativen zu tödlicher Polizeilösung sind | ❓ OFFEN | `M-PD` ‚Cash / law / evidence‘ (arrest/jail/surrender); `E-EN-027` (💡 Eskalationsstufen) | POL KI | `CQ-84` |
| COMBAT-DEC-PO-026 | **Verhältnis zwischen Monsterkampf und öffentlichem Wissen** (Videos, Viralität, Geheimhaltung; kein Gedächtnislöschen) ist offen entschieden bis auf Grundsätze | 🧭 RICHTUNG | `K-HU-008`; Kreaturen 11 | POL KRE | `CQ-85` |
| COMBAT-DEC-PO-027 | **Gefangene Menschen** (Festsetzen durch den Spieler) und die Folgen sind nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-FF-013` | POL | `CQ-86` |

---

## 3. Spuren und Beweise eines Kampfes

> **💡 VORSCHLAG (`COMBAT-DEC-PO-022`).** Der Katalog ordnet Kampfspuren den Beweisquellen aus `E-RE-002` und `E-SP-002`.

| Kampfspur | Beweisquelle (`E-RE-002`) | Modul |
|---|---|---|
| Blutspuren, Wundflüssigkeit | DNA, Spurenbild | 13 |
| Hülsen, Magazine, Waffen | Waffe und Ballistik | 05, 10, 09 |
| Einschüsse und Geschosse | Ballistik | 05 |
| Kleidungsschäden, Fremdfasern | Kleidung, Fasern | 12 |
| Verletzungen am Körper | Befund (Arzt, Krankenhaus) | 13 |
| Zeugenaussagen | Zeugen | 20 |
| Überwachungsvideo | Kameras, Gesicht, Fahrzeug | 20 |
| Digitale Spuren (Handyvideo) | digitale Beweise | 20 |

**Prüfbare Eigenschaften (🧩 ABGELEITET aus `COMBAT-DEC-PO-001`, `-003`):** (1) Eine Spur existiert nur, wenn ein physischer Vorgang sie erzeugt hat. (2) Die Polizei kennt nur, was Zeugen, Kameras oder Spuren hergeben. (3) Eine abgelegte Waffe ist ein Weltobjekt, bis sie geborgen oder bereinigt wird. (4) Bereinigung entfernt Müll, nicht belegte Beweise unmittelbar.

---

## 4. Zeugen, Notruf, Fahndung, Festnahme

```text
Kampf im öffentlichen Raum
  -> Zeugen (gesehen/gehört), Kameras, Spuren
  -> Notruf mit Latenz und Beschreibung (E-RE-010)
  -> Polizei trifft ein (Zuständigkeit), Absperrung, Beweissicherung
  -> Spielerseitig: fliehen, aufgeben, aufklären, verstecken
  -> Fahndung (Beweislage, nicht Sterne), Verhör bei Verdacht
  -> Festnahme: Verhör, Strafen, Gericht, Haft (kein Game Over)
```

Die Kette folgt `E-RE-010` bis `-015`, `E-ZE-009`; sie ist als Ablauf Entwurf, die Teilaussagen sind beschlossen. Zeitwerte und Schwellen fehlen.

---

## 5. Selbstverteidigung und Fight Club

- **Selbstverteidigung** wird bewertet, unabhängig vom Geschlecht; die Strafkette berücksichtigt Notwehr (`E-RE-014`, `E-RE-018`). Die Kriterien sind nicht festgelegt (`CQ-82`).
- **Fight Club:** gleiches Kampfsystem, Ehrenkodex. Das Töten oder schwere Verletzen im Fight Club ist ein ‚massiver Regelbruch‘ (Formulierung des Assistenten, `23#168A`) mit Ausschluss, Rufverlust, Feindschaften, Ermittlungen und gegebenenfalls einem Nebenfall – nur als Entwurf (`COMBAT-DEC-NK-014`).
- **Sportliche Kämpfe sind keine Straftat; Straßenkampf kann eine sein.** Die Abgrenzung (Einwilligung, Ort, Regeln) ist nicht beschrieben (`CQ-83`).

---

## 6. Fraktionen, Rache, Ruf

- Fraktionen sind autonom (Anforderung D, `PE-N1022`); Hunter-Organisationen, kriminelle Netzwerke und Behörden reagieren auf Ereignisse und passen sich langfristig an (`N-BH-011`).
- Kampfereignisse fließen als Gerüchte und Beobachtungen in das Wissen der Fraktionen; ein einzelner Kampf ändert nicht sofort die Fraktionsbeziehung (Modell: [NPCs 17](../NPCs/17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md)).
- **Rache** ist an Gedächtnis und Beziehung gebunden (Modul 19), nicht an eine Zahl.
- Wesen und Hunter besitzen einen **Ruf** (`K-GS-013`, `N-NE-015`), den Kämpfe beeinflussen können.

---

## 7. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Polizeireaktion (`CQ-84`) | A: Gestaffelte Eskalation (Kontakt, Aufforderung, Zwang) | glaubwürdig | viel Verhalten zu bauen |
| | B: Einfache Reaktion (Verfolgung, Festnahme) | günstig | wirkt arcadig |
| Beweiswertung (`CQ-83`) | A: Konfidenz aus Quellen addiert | nachvollziehbar | mechanisch |
| | B: Narrative Bewertung durch Regeln pro Fall | flexibel | schwer zu testen |
| Waffenrecht (`CQ-81`) | A: Waffen tragen ist generell erlaubt, Einsatz bewertet | passt zu US-Setting | rechtlich uneinheitlich |
| | B: Zustandsabhängiges Recht (je Staat) | glaubwürdig | Datenaufwand |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Ermittlung | Spuren, Zeugen, Beweiskette |
| zu NPCs und Fraktionen | Reaktion, Rache, Ruf |
| zur Persistenz | Tatorte, Waffen, Fahndungsstand |
| zum Koop | wer wird verhaftet (individuell, `N-KO-008`) |
| zum Kampf | Ereignisse (Modul 03 §6) |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Zeuge ist ein Monster oder Besessener | Wissen und Bericht folgen Figur | 💡 |
| Kampf in unbewohntem Gebiet ohne Zeugen | keine Fahndung, Spuren bleiben | 🧩 |
| Tod während Festnahme | Kein stilles Zurücksetzen der Fahndung | ❓ (`E-WZ`, offen) |
| Leichen und Waffen häufen sich | Bereinigung nach Regel | 📋 Wunsch |
| Mod verändert Straftat | additiv, kein Eingriff in den Spielstand | 💡 |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-81` | Waffenrecht: Besitz, offenes Tragen, illegale Waffen, Meldung von Schussverletzungen | `COMBAT-DEC-PO-024` | ja |
| `CQ-82` | Selbstverteidigung: Bewertungskriterien | `COMBAT-DEC-PO-011` | ja |
| `CQ-83` | Straftatenkatalog, Beweiswertung, Zuständigkeit, Fight-Club-Abgrenzung | `COMBAT-DEC-PO-023` | ja |
| `CQ-84` | Polizeireaktion auf Gewalt: Eskalation, Zwang | `COMBAT-DEC-PO-025` | ja |
| `CQ-85` | Öffentliches Wissen nach Monsterkämpfen (Videos, Vertuschung) | `COMBAT-DEC-PO-026` | ja |
| `CQ-86` | Festsetzen und Gefangenhalten von Menschen durch den Spieler | `COMBAT-DEC-PO-027` | ja |
