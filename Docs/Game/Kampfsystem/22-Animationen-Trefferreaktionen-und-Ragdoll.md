# Kampfsystem 22: Animationen, Trefferreaktionen und Ragdoll

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul sammelt die Animationsanforderungen des Kampfsystems und trennt, was der Master fordert (Schichten, IK, Motion Warping, prozedurale Anpassung), von dem, was **nicht entschieden** ist: Ragdoll, physikalische Animation, Trefferreaktionen, Animationstechnik in Unreal, Herkunft der Assets. Es gibt keine Animationsliste. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Treffermodell: [Modul 12](12-Treffermodell-und-Koerperphysik.md); Nahkampf: [Modul 04](04-Nahkampf-und-Kampfstile.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Zustand zu Animationsparametern (Entwurf)](#3-zustand-zu-animationsparametern-entwurf)
4. [Paaranimationen und die Kombinationsexplosion](#4-paaranimationen-und-die-kombinationsexplosion)
5. [Trefferreaktion, Ragdoll, Tod](#5-trefferreaktion-ragdoll-tod)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Komplexe Verletzungsanimation ist vor der Veröffentlichung nötig** (Schichten, Ganzkörper-IK, prozedurale Anpassung); frühe Builds dürfen einfacher sein (`M-PD`, Gameplay-Systeme §4, §14.3).
2. **Der Entwickler weiß, dass Animation der teuerste Teil wird:** ‚ja das wird nicht leicht‘ (`23#101E` P17), ‚wieso teuer, ja das soll komplex sein‘ (`23#104E` P17).
3. **Animation und tatsächliche Bewegung müssen übereinstimmen** (Proposal 0009 §3a).
4. **Zu Ragdoll, aktiver Ragdoll und Motion Matching gibt es keine Entscheidung** im Master oder in den Chats (Suche am 23.10.2026).
5. **Paarinteraktionen (Griffe, Boden) brauchen das gemeinsame Interaktions-Framework** mit Sitzungen, Motion Warping, IK, Synchronisation (Gameplay-Systeme §3.2); ob der Nahkampf es nutzt, ist offen (`CQ-08`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-AN-001 | **Komplexe Verletzungsanimation (Schichten, IK, prozedurale Anpassung) ist vor der Veröffentlichung nötig,** auch wenn frühe Builds einfacher sind | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; Gameplay-Systeme §4 | ANI | – |
| COMBAT-DEC-AN-002 | **Komplexe Verletzungsanimation, Ganzkörper-IK, Motion Warping, prozedurale Anpassung und Gesichtsanimation** (Augen, Blick, Brauen, Wangen, Kopf, Atmung, Mikroausdruck) | 📋 BESCHLOSSEN | Gameplay-Systeme §14.3 | ANI | – |
| COMBAT-DEC-AN-003 | **Verletzungen wirken in Schichten:** Basisbewegung plus Beinverletzung (Hinken) plus Armverletzung plus Schmerz und Atmung (Beispiel des Assistenten); der Entwickler: ‚das wird nicht leicht‘ | 🧭 RICHTUNG | `23#103A` P17 (Entwurf); `23#101E` P17; `23#104E` P17 | ANI MED | Parameter: Abschnitt 3 |
| COMBAT-DEC-AN-004 | **Animation und tatsächliche Bewegung müssen übereinstimmen** | 📋 BESCHLOSSEN | Proposal 0009 §3a | ANI BEW | – |
| COMBAT-DEC-AN-005 | **Waffenbedienung ist mechanisch korrekt animiert** (Pumpgun, Pistole, Revolver) | 📋 BESCHLOSSEN | `COMBAT-DEC-FK-011` | ANI WAF | – |
| COMBAT-DEC-AN-006 | **Verletzte Gegner zeigen Folgen** (Arm schützen, humpeln, das Bein halten) in späteren Versionen | 📋 BESCHLOSSEN | `23#86E` P3; `COMBAT-DEC-TK-015` | ANI KI | – |
| COMBAT-DEC-AN-007 | **Sichtbare Wunden, offene Brüche, beschädigte Kleidung und Blut sind am Spieler und an Gegnern darstellbar** | 📋 BESCHLOSSEN | `COMBAT-DEC-TK-007`, `-008`; `M-PD` ‚Human anatomy…‘ | ANI TRF | Technik: Gameplay-Systeme §14.3 |
| COMBAT-DEC-AN-008 | **Gore-Einstellungen voll, reduziert, minimal** und einstellbare Kill-Cams | 📋 BESCHLOSSEN | Gameplay-Systeme §4 (Darstellung) | UI ANI | Schwellen: `CQ-47` |
| COMBAT-DEC-AN-009 | **Gemeinsame Sekundärphysik** für Rückstoß, Laufen, Kampf, Fahrzeug, Stürze, Tragen, Haar, Kleidung; realistische Dämpfung und Masse | 📋 BESCHLOSSEN | `COMBAT-DEC-TK-013` | ANI | Technik offen (`CQ-48`) |
| COMBAT-DEC-AN-010 | **Gemeinsames Interaktions-Framework** mit Teilnehmern, Sitzungen, stabilen IDs, Anfragen, Reservierungen, Annäherung, Ausrichtung, Motion Warping, IK, Synchronisation, Übergängen, Netzwerk und Persistenz; kein Monolith | 📋 BESCHLOSSEN | Gameplay-Systeme §3.2 | ANI KOO | Nutzung für Griffe: `CQ-08` |
| COMBAT-DEC-AN-011 | **Manuelle QA bleibt für Gefühl, Animation, Audio und Bildqualität wichtig;** der Entwickler ist der endgültige Entscheider für Subjektives | 📋 BESCHLOSSEN | Gameplay-Systeme §17.2, §17.3 | TST | – |
| COMBAT-DEC-AN-012 | **Zustandsparameter statt Animationsaufrufe:** Der portable Kern liefert Parameter (Hinkfaktor, Armschutz, Atemrate, Zittern, Haltung), die Animation interpretiert; so bleiben Kern und Engine getrennt | 💡 VORSCHLAG | Abschnitt 3; `COMBAT-DEC-ZA-009` | ANI | – |
| COMBAT-DEC-AN-013 | **Mindestmenge einer Paaranimations-Bibliothek und ein Plan gegen die Kombinationsexplosion** (Positionen mal Techniken mal Körpergrößen) | 💡 VORSCHLAG | `M-P14` (Themeninventar ‚animation-combination-explosion-mitigation-plan‘); `23#171E` (Position bestimmt Aktion) | ANI | `CQ-87` |
| COMBAT-DEC-AN-014 | **Vollständigkeitsprüfung der Verletzungsanimationen vor dem Release** (Validator) | 💡 VORSCHLAG | `M-P14` (‚pre-release-injury-animation-completeness-validator‘) | TST | – |
| COMBAT-DEC-AN-015 | **Ragdoll, aktive Ragdoll, physikalische Animation, Motion Matching und Anim-Blueprint-Ansatz** sind nicht entschieden | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | ANI | `CQ-87` |
| COMBAT-DEC-AN-016 | **Trefferreaktionen** (Reaktion auf Region und Wucht, Stolpern, Niederreißen) sind nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-TK-022` | ANI | `CQ-92` |
| COMBAT-DEC-AN-017 | **Herkunft der Animationen und Assets** (eigene, gekauft, generiert) ist nicht entschieden | ❓ OFFEN | Proposal 0009 §5 (3); `M-OD` | ANI | `CQ-93` |
| COMBAT-DEC-AN-018 | **Darstellung von Verletzungen in der Egoperspektive** (Hände, Atem, Sichtfeld) und in der Third-Person-Kamera ist nicht beschrieben | ❓ OFFEN | Gameplay-Systeme §2 (Kamera); `M-PD` ‚Human anatomy…‘ (Sicht, Zittern) | ANI UI | `CQ-94` |
| COMBAT-DEC-AN-019 | **Animationskosten im Gedränge und aus der Ferne** (LOD, Gruppen) sind nicht entschieden | ❓ OFFEN | Gameplay-Systeme §14.3; Modul 25 | ANI PRF | `CQ-95` |

---

## 3. Zustand zu Animationsparametern (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-AN-012`).** Die Tabelle zeigt, welche Körperzustände die Quellen sichtbar haben wollen, und leitet daraus Parameter ab.

| Körperzustand (Quelle) | Sichtbare Wirkung (Quelle) | Parameter (Entwurf) |
|---|---|---|
| Beinverletzung | Humpeln, eingeschränkter Sprint, Bein halten (`M-PD`, `23#86E` P3) | Hinkfaktor links/rechts, Tempo-Obergrenze |
| Armverletzung | Arm schützen, schlechtere Waffenführung, Reloads (`M-PD`) | Armschutz links/rechts, Griffstärke |
| Rippen, Atemproblem | Rippenschmerz-Haltung, Atemgeräusch (`23#100A`) | Atemrate, Brustkorbbewegung |
| Blutverlust, Schock | Zittern, Tunnelblick, Koordination (`M-PD`) | Zitteramplitude, Gleichgewicht |
| Adrenalin | Handlungsfähigkeit trotz Schmerz, späterer Einbruch (`M-PD`) | Schmerzunterdrückung |
| Bewusstsein | Benommenheit, Zusammenbruch (`M-PD`) | Benommenheit, Standfestigkeit |
| Offene Fraktur | sichtbarer Knochen (`M-PD`) | Wundoverlay-Kennung |
| Kleidung, Blut | Kleidungsschaden, Blutspuren (`M-PD`, Gameplay-Systeme §9.3) | Overlay-Masken |
| Ermüdung | langsamere Schläge, unruhige Visierung (`23#162A` P5) | Müdigkeitsfaktor |
| Waffe, Zustand | Nachladen unterbrochen, Störung (`M-PD`) | Zustandskennung |

**Regel (💡):** Der Kern gibt Zahlen und Kennungen aus; die Animation entscheidet die Pose. Kein Kernmodul kennt Montagen oder Blueprints.

---

## 4. Paaranimationen und die Kombinationsexplosion

Die Quellen verlangen Griffe, Würfe, Bodenpositionen, Scrambles und Aufgabegriffe mit Körperzuständen (`COMBAT-DEC-NK-008` bis `-012`). Das ergibt eine große Menge von **Paarzuständen**.

| Dimension | Beitrag zur Menge |
|---|---|
| Position (stehend, Clinch, Boden, Zwischenzustand) | mehrere Dutzend Zustände (Zahl nicht entschieden) |
| Technik und Übergang | Familien nach `COMBAT-DEC-NK-003` bis `-011` |
| Körpergröße und Kraft beider Beteiligter | Skalierung über Motion Warping und IK (Gameplay-Systeme §14.3) |
| Verletzungszustand | Schichten (Abschnitt 3) |
| Untergrund und Umgebung | Wand, Treppe, Möbel (`COMBAT-DEC-NK-033`) |
| Waffen | Messer, Flasche, Pistole (`COMBAT-DEC-NK-023`) |

**Folgerung (🧩 ABGELEITET):** Eine vollständig handanimierte Matrix ist nicht tragbar; die Quellen fordern daher genau die Verfahren, die sie auch nennen: Motion Warping, IK, prozedurale Anpassung (Gameplay-Systeme §14.3). Wie viel handgemacht und wie viel prozedural entsteht, ist offen (`CQ-87`). Der Entwickler selbst wünscht Tiefe (`23#167E` P2: ‚mehr Auswahl‘) bei wenigen Eingaben (`23#171E`).

---

## 5. Trefferreaktion, Ragdoll, Tod

**Was beschlossen ist:** Tod und Bewusstlosigkeit sind Körperzustände (`COMBAT-DEC-BW-005`, `-022`); Gegner zeigen Folgen von Verletzung (`COMBAT-DEC-AN-006`); eine gemeinsame Sekundärphysik besteht (`COMBAT-DEC-AN-009`).

**Was nicht beschrieben ist:**

- ob der Körper bei Tod oder Bewusstlosigkeit in eine Physiksimulation übergeht;
- wie ein Treffer den Körper physisch bewegt (Impuls, Stolpern);
- wie Trefferreaktionen auf Region und Ursache abgestimmt werden (Schrot gegen Messer);
- ob und wie Körper (Leichen) in der Welt liegen bleiben, bewegt werden, Spuren erzeugen (Beweise: `E-QA-023`: Waffen und Leichen bleiben bis zur Bereinigung).

**Prüfbare Eigenschaften, die ohne Entscheidung gelten (🧩 ABGELEITET):** (1) Ein toter Körper ist ein Weltobjekt mit Verletzungsbefund. (2) Eine Trefferreaktion widerspricht dem Körperzustand nicht (kein munteres Weiterlaufen bei Beinfunktionsverlust). (3) Die Reaktion ist vom Seed unabhängig reproduzierbar oder protokolliert.

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Tod und Bewusstlosigkeit (`CQ-87`) | A: Vorgefertigte Fallanimationen | kontrollierbar | wenig Variation |
| | B: Vollständige Ragdoll | physikalisch | instabil, unkontrolliertes Aussehen |
| | C: Aktive Ragdoll (Animation plus Physik) | glaubwürdig | hoher Aufwand |
| Paaranimationen (`CQ-87`) | A: Handgebaute Paare | beste Qualität | Menge |
| | B: Prozedural (IK, Motion Warping) | skalierbar | Qualitätsgrenze |
| | C: Hybrid | realistisch | doppelte Pipeline |
| Trefferreaktionen (`CQ-92`) | A: Additive Reaktionsanimationen nach Region | günstig | wirkt generisch |
| | B: Physikalischer Impuls | glaubwürdig | Stabilität, Netzwerk |
| | C: Hybrid | Balance | Komplexität |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Kern ([Modul 03](03-Kampfzustaende-und-Systemarchitektur.md)) | Zustandsparameter |
| zum Treffermodell | Region, Wucht, Mechanismus |
| zum Interaktions-Framework | Sitzungen, Warping, IK |
| zur Kamera ([Modul 08](08-Kampfbewegung-und-Traversal-Integration.md)) | Wandnähe, Egoperspektive |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Replikation von Posen, Reaktionen |
| zur Performance ([Modul 25](25-Performance-Skalierung-und-Simulationsstufen.md)) | Animations-LOD |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Fehlende Animation für einen Zustand | sichtbarer Platzhalter, Protokoll, kein Absturz | 💡 |
| Paar mit stark verschiedenen Körpergrößen | Anpassung durch Warping, Grenzen mit Ablehnung | 💡 |
| Verletzungsschichten widersprechen sich | Prioritätsregel, deterministisch | 💡 |
| Netzwerk verliert Posenupdate | Auflösung zum letzten Zustand | 💡 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-87` | Animationsansatz in Unreal: Ragdoll, aktive Ragdoll, Motion Matching, Paaranimationen, Mengenplanung | `COMBAT-DEC-AN-013`, `-015` | ja |
| `CQ-92` | Trefferreaktionen: Region, Wucht, Impuls, Stolpern, Niederreißen | `COMBAT-DEC-AN-016` | ja |
| `CQ-93` | Herkunft der Animationen und Assets: eigene, gekaufte, generierte | `COMBAT-DEC-AN-017` | ja |
| `CQ-94` | Darstellung der Verletzung in Ego- und Third-Person-Kamera | `COMBAT-DEC-AN-018` | ja |
| `CQ-95` | Animationskosten: LOD, Gruppen, Fernbereich | `COMBAT-DEC-AN-019` | ja |
