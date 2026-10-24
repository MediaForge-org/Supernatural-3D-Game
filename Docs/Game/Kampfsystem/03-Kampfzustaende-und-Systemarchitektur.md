# Kampfsystem 03: Kampfzustände und Systemarchitektur

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul trennt scharf: Was die Quellen über **Zustände** entscheiden (📋), und was dieser Ordner als **konzeptionelles Zustands- und Architekturmodell** vorschlägt (💡). Das Modell ist ein Entwurf zur Prüfung, kein Beschluss. Es enthält weder produktiven Code noch Unreal-Klassen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Was die Quellen über Zustände festlegen](#2-was-die-quellen-über-zustände-festlegen)
3. [Schichtenmodell (Entwurf)](#3-schichtenmodell-entwurf)
4. [Zustandsmodell eines Kämpfers (Entwurf)](#4-zustandsmodell-eines-kämpfers-entwurf)
5. [Datenverantwortung: ein Besitzer je Tatsache (Entwurf)](#5-datenverantwortung-ein-besitzer-je-tatsache-entwurf)
6. [Ereignisse und Schnittstellen (Entwurf)](#6-ereignisse-und-schnittstellen-entwurf)
7. [Fehlerfälle](#7-fehlerfälle)
8. [Testbarkeit und Portierbarkeit](#8-testbarkeit-und-portierbarkeit)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Quellen fordern Zustände, nicht Zustandsautomaten.** Sie nennen Wahrnehmungszustände, Verletzungs- und Erholungsstufen, Lade- und Positionszustände. Eine Zustandsmaschine mit Übergängen, Schwellen oder Zeiten steht nirgends.
2. **Zwei Grundsätze sind eindeutig:** Der Zustand entsteht **aus dem tatsächlichen Treffer und Körperzustand**, nicht aus einem abstrakten Schadenswert (`M-PD` ‚Ballistics‘, ‚Human anatomy…‘), und der Zustand der Welt bleibt **persistent**, wenn er für die Folgen wichtig ist (`M-PD` ‚World state and persistence‘, Gameplay-Systeme §16).
3. **Der Entwurf in diesem Modul** ist so gebaut, dass er die Quellen nicht verengt: Er legt eine Schichtung, Besitzverhältnisse und Ereignisse fest, aber **keine Zahlen** und **keine Eingabebelegung**.
4. **Die Interaktionsarchitektur ist bereits beschlossen** (gemeinsames Interaktions-Framework mit Sitzungen, Teilnehmern, Reservierungen, Annäherung, Ausrichtung, Motion Warping, IK, Synchronisation, Netzwerk und Persistenz, Gameplay-Systeme §3.2). Ob Nahkampf-Paare (Clinch, Bodenkampf, Würgegriff) diese Sitzungen benutzen, ist **nicht entschieden** (`CQ-08`).

---

## 2. Was die Quellen über Zustände festlegen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-ZA-001 | **Zustand folgt aus Trefferpfad und Körperzustand,** nicht aus einem linearen Abstands-Schadenswert. Die Verletzung beruht auf Restzustand des Projektils, Durchdringung und anatomischem Weg | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#107A` (Grundsatz, vom Entwickler mit ‚Perfekt‘ im Block bestätigt, `23#106E`); Gameplay-Systeme §5.2 | TRF MED | Entwurf der Kette: [Modul 12](12-Treffermodell-und-Koerperphysik.md) |
| COMBAT-DEC-ZA-002 | **Ein klassischer HP-Wert darf intern existieren, ist aber nicht die Mechanik.** Der Körperzustand umfasst Blutvolumen, Blutungsrate, Schmerz, Schock, Bewusstsein, Atmung, Adrenalin, Beweglichkeit, Gliedmaßenfunktion und Infektionsrisiko | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (Systemzustand); `23#85A` P4 (HP optional intern); `23#86E` P4 | MED | Liste ist die Mindestmenge (Modul 13) |
| COMBAT-DEC-ZA-003 | **Wahrnehmungszustände** können mindestens unaware, suspicious, investigating, alert, confirmed target und searching sein; Kampf kann in Suche zurückfallen, wenn die Sichtlinie bricht und die genaue Position verloren ist | 🧭 RICHTUNG | `M-PD` ‚Stealth, perception and horror AI‘ (‚may include‘); `23#225A`–`23#226E`; `23#253A` P13; `K-KI-005`, `K-KI-010` | KI | Übergangsregeln und Detected-Schwelle: `CQ-09` |
| COMBAT-DEC-ZA-004 | **Wissensgrade der KI** sind getrennt: bestätigt, starker Verdacht, schwacher Verdacht, unbekannt; adaptive Taktiken nutzen nur Beobachtetes oder plausibel Gefolgertes | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#242E`; `K-KI-003` | KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-ZA-005 | **Lade- und Waffenzustände sind persistent und unterbrechbar:** Patronenlager und Magazin sind getrennte Zustände, der Nachladezustand überlebt Speichern und Laden | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ | WAF PER | [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md), [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| COMBAT-DEC-ZA-006 | **Bodenkampf kennt stabile Positionen und umkämpfte Übergangszustände** (Scrambles), in denen keiner volle Kontrolle hat | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P9 (‚nicht immer eindeutige Positionen, sondern so ein Mittelding‘); `23#169E` P8–9 | BEW ANI | [Modul 04](04-Nahkampf-und-Kampfstile.md) |
| COMBAT-DEC-ZA-007 | **Genesungsstufen** akut → stabilisiert → heilend → erholend → erholt; erneute Verletzung einer noch nicht ausgeheilten Region erreicht schwere Zustände schneller | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` D; `N-LB-014` | MED | [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| COMBAT-DEC-ZA-008 | **Kampf, Verletzung, Tarnung und KI sind Risikosysteme:** Sie brauchen deterministische Simulationen, Szenariomatrizen, Replay-Tests, Fuzzing und Leistungsregression; Zufall ist über Seeds reproduzierbar | 📋 BESCHLOSSEN | Gameplay-Systeme §17.2; `23#254E` P16; `23#256E`; `M-PD` ‚Testing philosophy‘ | TST | Verpflichtet die Architektur zur Determinismusfähigkeit (ZA-012) |
| COMBAT-DEC-ZA-009 | **Rein logische Kampfregeln liegen in portablem C++20;** Unreal-Aktoren, Animation, Physik und Eingabe liegen hinter Adaptern. Engine-abhängige Systeme werden nicht künstlich engine-unabhängig gemacht | 📋 BESCHLOSSEN | Gameplay-Systeme §17.1; `CLAUDE.md` §5 | alle | Grenzziehung im Entwurf: Abschnitt 3 |
| COMBAT-DEC-ZA-010 | **StateTree organisiert Zustände der KI; eigener C++-Code übernimmt Wahrnehmung, Gedächtnis, Kampf und Sonderregeln** | 📋 BESCHLOSSEN | Gameplay-Systeme §8 (erste Aufzählung); `N-SI-002` | KI | Gilt für KI-Zustände; ob auch Spielerzustände StateTree nutzen, steht nicht fest |
| COMBAT-DEC-ZA-011 | **Der Zustand während einer laufenden Interaktion bleibt konsistent über Speichern, Laden und Streaming** (Interaktionssitzungen sind persistent) | 🧩 ABGELEITET | Prämissen: Gameplay-Systeme §3.2 (Sitzungen, Netzwerk, Persistenz); `COMBAT-DEC-ZA-005` | PER KOO | Sperre im aktiven Kampf macht den Fall selten (`COMBAT-DEC-GP-028`) |
| COMBAT-DEC-ZA-012 | **Schichtenmodell** Eingabe → Absicht → Kämpferzustand → Aktion → Trefferauflösung → Verletzung → Folgeereignisse → Darstellung | 💡 VORSCHLAG | Abschnitt 3 (Entwurf dieses Ordners) | alle | Prüfung durch Projektleitung |
| COMBAT-DEC-ZA-013 | **Eine Tatsache hat einen Besitzer;** Querwirkungen laufen als Ereignisse, nicht als direkte Schreibzugriffe | 💡 VORSCHLAG | Übernommen aus [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7; `K-LW-007` | alle | Abschnitt 5 |
| COMBAT-DEC-ZA-014 | **Ein Zustandsmodell für Kämpfer** mit Hauptmodus (Fortbewegung, Fernkampfbereitschaft, Schlagabtausch, Clinch, Boden, Gegriffen, Kampfunfähig, Bewusstlos, tot) und überlagernden Aktivitäten (Nachladen, Störungsbeseitigung, Behandeln, Tragen, Fahren) | 💡 VORSCHLAG | Abschnitt 4 | BEW ANI KI | Namen, Zahl und Übergänge nicht beschlossen |
| COMBAT-DEC-ZA-015 | **Die Stufenleiter ‚gesund … kritisch … Kampfunfähigkeit … Tod‘ ist eine abgeleitete Zusammenfassung des Körperzustands,** kein Primärzustand | 💡 VORSCHLAG | Leiter aus `23#63A`, `23#87A` (G), `23#98A`; Entwickler bejaht die Folgen, nicht die Leiter (`23#62E`, `23#88E`) | MED UI | Schwellen: `CQ-10` |
| COMBAT-DEC-ZA-016 | **Hochfrequente Kampfdaten (Projektile im Flug, Trefferdetails, Animationszeit) laufen nicht durch das Weltzustandsjournal;** nur Folgen mit langfristiger Bedeutung werden dort festgehalten | 💡 VORSCHLAG | Anwendung von [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6; `K-LW-008` | PER PRF | Abschnitt 5; Modul 24 |

---

## 3. Schichtenmodell (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-ZA-012`).** Die Schichtung beantwortet die Frage ‚wer darf was ändern‘. Sie ist kein Klassendiagramm.

```text
L0  Eingabe                Rohgeräte (Controller, Maus, Tastatur) -> engine-neutrale Aktionen
L1  Absichtsauflösung      Aktion + Kontext (Position, Distanz, Griff, Richtung) -> gemeinte Handlung
L2  Kämpferzustand         Hauptmodus + Aktivitäten; prüft Voraussetzungen und Übergänge
L3  Aktionsausführung      Zeitverlauf einer Handlung (Anlauf, Wirkung, Erholung); Nachladen, Schlag, Schuss
L4  Trefferauflösung       Weg des Projektils/Schlags durch Welt und Körper -> Trefferkette
L5  Verletzungsmodell      Trefferkette -> Wunden, Blutung, Schmerz, Schock, Funktionsverlust
L6  Folgeereignisse        Meldungen an NPC-Gedächtnis, Ermittlung/Beweise, Fraktionen, Persistenz
L7  Darstellung            Animation, Ton, Kamera, Effekte, Haptik, HUD, Kill-Cam
```

**Portabler Kern (C++20, ohne Unreal):** L1-Regeln, L2, L3-Zeitverlauf, die Berechnung der Flugbahn und des Materialdurchschlags in L4, L5 und die Ereignisdefinitionen in L6. Das sind Regeln und Zustandsübergänge im Sinne von Gameplay-Systeme §17.1.
**Unreal-Adapter:** L0 (Enhanced Input, vorhanden: `IMC_Default`, Proposal 0009 §3a), die Welttraces und Physik in L4, die Animation in L7, Audio, UI und die Replikation in [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md).
**Grenzregel (💡):** Der Kern kennt keine Aktoren und keine Zeit der Engine; er bekommt einen Zeitschritt und eine Zufallsquelle hineingereicht. Das macht deterministische Tests möglich (`COMBAT-DEC-ZA-008`).

---

## 4. Zustandsmodell eines Kämpfers (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-ZA-014`, `-015`).** Namen sind Arbeitsbezeichnungen. Quellenbasierte Zustände sind mit ihrer Entscheidung verknüpft.

### 4.1 Hauptmodi

| Hauptmodus | Bedeutung | Quellenbasis |
|---|---|---|
| Fortbewegung | Gehen, Laufen, Sprinten, Schleichen über kontinuierliche Stickauslenkung, Traversal | `M-PD` ‚Stealth…‘ (Stick), Proposal 0009; [Modul 08](08-Kampfbewegung-und-Traversal-Integration.md) |
| Fernkampfbereitschaft | tiefe Bereitschaft (low ready), Punktschießen, Visieren (ADS), unter Raumbeschränkung | `M-PD` ‚Gunplay / weapons‘ |
| Schlagabtausch | Stehen, Distanzmanagement, Schläge, Tritte, Fußarbeit, Verteidigung | `M-PD` ‚Melee / grappling‘ |
| Clinch | stehender Griffkampf (Unterhaken, Überhaken, Körperkontrolle, Wand) | `M-PD` ‚Melee / grappling‘ |
| Boden | stabile Position oder umkämpfter Übergang; Aufstehen, Befreiung | `M-PD` ‚Melee / grappling‘ |
| Gegriffen | ein Gegner oder Monster hat gegriffen (Würgen, Wand, Arm, Waffe) | `23#162A` (Vorschlag), `M-PD` (Gegner greift Waffe) |
| Kampfunfähig | Handlung weitgehend unmöglich, noch bei Bewusstsein (kriechen, Hilfe rufen) | `23#87A` G (Vorschlag) |
| Bewusstlos | keine Handlung | `M-PD` (Bewusstsein als Systemzustand) |
| Tot | keine Handlung; Folgen laufen | `M-PD` |

### 4.2 Überlagernde Aktivitäten

Nachladen (unterbrechbar), Störungsbeseitigung (eine kontextabhängige Eingabe), Behandeln (Verband, Reinigung), Tragen/Helfen, Fahren, Werfen, Ausrüstungswechsel (Zugriffszeit nach Trageort: Holster oder Gürtel schnell, Rucksack langsamer, Kofferraum nicht im Kampf; `M-PD` ‚Multiple-opponent combat…‘). Die Aktivität verändert, **welche Übergänge erlaubt sind**, nicht den Hauptmodus.

### 4.3 Beispielhafte Übergangsregeln

| Von | Nach | Voraussetzung (Entwurf) | Bemerkung |
|---|---|---|---|
| Fortbewegung | Fernkampfbereitschaft | Waffe gezogen; Raum erlaubt Haltung | Zugriffszeit nach Trageort |
| Fernkampfbereitschaft | Schlagabtausch | Gegner in Nahdistanz; Waffe wird gehalten, weggedrückt oder abgelegt | ‚fließender Übergang‘ (`M-PD`); Waffenhaltung im Handgemenge bleibt physikalisch bedeutsam |
| Schlagabtausch | Clinch | Griff gelingt; Distanz klein | Erfolg hängt von Timing, Balance, Kraft, Skill, Verletzung, Ausdauer ab (`23#166A`, Vorschlag) |
| Clinch | Boden | Takedown, Trip, Wurf | Untergrund wirkt (Asphalt, Schlamm; `23#166A`) |
| Boden | Boden (anderer Zustand) | Sweep, Pass, Escape | umkämpfter Zwischenzustand möglich |
| irgendein Modus | Gegriffen | Gegner gelingt der Griff | Befreiung ohne Quick-Time-Spam (`23#162A` Vorschlag) |
| irgendein Modus | Kampfunfähig | Funktionsverlust durch Körperzustand | nicht durch Trefferzahl (`M-PD`) |
| Kampfunfähig | Bewusstlos | Schock/Blutverlust/Schlag | → [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| irgendein Modus | Tot | Körperzustand tödlich | Folgen: Modul 14, 19, 20 |

### 4.4 Beispiel: Zustandsabfolge in einer Begegnung (💡)

```text
Fortbewegung (Schleichen)
  -> Wahrnehmung der Gegner: unaware -> suspicious -> investigating   [KI-Seite]
  -> Spieler wird entdeckt: confirmed target                          [Speichersperre, GP-028]
  -> Fernkampfbereitschaft (Pumpgun, ADS)
  -> Schuss -> Magazin leer -> Nachladen (Aktivität, unterbrechbar)
  -> Gegner erreicht Nahdistanz -> Waffe wegdrücken -> Schlagabtausch
  -> Gegner greift -> Gegriffen (Wand) -> Befreiung durch Messer
  -> Verletzung summiert sich -> Kampfunfähig -> Rettung (Modul 14) oder Tod
```

Jeder Pfeil ist ein Ereignis (Abschnitt 6). Das Beispiel ist ein Entwurf, kein Spielablauf.

---

## 5. Datenverantwortung: ein Besitzer je Tatsache (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-ZA-013`, `-016`).** Die Tabelle legt fest, welche Instanz eine Tatsache **besitzt**; andere lesen oder erhalten Ereignisse.

| Tatsache | Besitzer | Gespeichert? | Quelle der Anforderung |
|---|---|---|---|
| Körperzustand eines Kämpfers (Wunden, Blutvolumen, Schmerz, Schock, Bewusstsein, Genesungsstufe) | Verletzungsmodell des Kämpfers | ja, solange der Kämpfer persistent ist | `M-PD` ‚Human anatomy…‘; `N-LB-012` |
| Zustand einer Waffeninstanz (Patronenlager, Magazin-IDs, Verschleiß, Verschmutzung, Beschädigung) | Waffeninstanz im Inventar oder in der Welt | ja | `M-PD` ‚Gunplay / weapons‘ |
| Füllstand eines Magazins | Magazinobjekt (stabile Identität) | ja | `M-PD` (‚Magazines retain their actual round count‘) |
| Lage abgelegter Waffen und Leerhülsen als Beweismittel | Weltobjekt | ja, bis zur Bereinigung | `M-PD` ‚Cash / law / evidence‘; `E-QA-023` |
| Projektile im Flug, laufende Schläge, Animationszeit | Kampf-Laufzeit | nein | Entwurf (`COMBAT-DEC-ZA-016`) |
| Position und Zustand eines Griffpaares | gemeinsame Sitzung beider Beteiligter (oder Kampfsystem, `CQ-08`) | nur wenn Speichern im Griff möglich ist | Gameplay-Systeme §3.2 |
| Wissen, Verdacht, letzte bekannte Position, Taktik der KI | KI-Gedächtnis der Figur | nach Persistenzstufe ([NPCs 03](../NPCs/03-Identitaet-Persistenzstufen-und-Datensatztiefe.md)) | `M-PD`; `K-KI-014` |
| Furcht und Moral eines Menschen | intern, nicht sichtbar | nach Persistenzstufe | `M-PD` (‚Fear/morale remains internal‘) |
| Erinnerung an die Begegnung (Waffen, Fallen, Verletzungen, Gnade) | NPC-/Monster-Gedächtnis | ja bei persistenten Figuren | `PE-N1022` B, C; `N-NE-003` |
| Lasting-Folgen (Tod, bleibende Verletzung, zerstörte Infrastruktur) | Weltzustand (WorldFacts) | ja | `PE-N1022` E; Gameplay-Systeme §16 |
| Spielergebnis des Kampfes im Fallbericht | Fall/Journal | ja | `M-PD`; Ermittlung |

**Regel (💡):** Die Verletzung gehört dem Körper, die Erinnerung der Figur, die Strafverfolgung der Ermittlung; das Kampfsystem **meldet**, es **schreibt nicht** in fremde Datensätze.

---

## 6. Ereignisse und Schnittstellen (Entwurf)

> **💡 VORSCHLAG.** Die Ereignisliste ist die Mindestmenge, die Module 12 bis 25 voraussetzen. Namen sind Arbeitsbezeichnungen.

| Ereignis | Auslöser | Empfänger (Beispiele) |
|---|---|---|
| `NoiseEmitted` | Schuss, Nachladen, Magazinabwurf, Schlag, zerbrechendes Glas, Schritt | Wahrnehmung der KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)), Zeugen |
| `WeaponFired`, `ReloadStarted/Interrupted/Completed`, `MalfunctionOccurred/Cleared` | Waffenhandhabung | Waffeninstanz, Audio, Beweise (Hülsen) |
| `HitResolved` | Trefferauflösung | Verletzungsmodell, Darstellung |
| `WoundCreated`, `WoundWorsened`, `BleedingChanged` | Verletzungsmodell | Körperzustand, HUD, Animationsschichten |
| `ConsciousnessChanged`, `Incapacitated`, `Died` | Körperzustand | Kämpferzustand, KI, Begleiter, [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| `ThreatStateChanged` (z. B. entdeckt) | KI-Wahrnehmung | Speichersperre, Musik, Begleiter |
| `GripEstablished`, `PositionChanged`, `GripReleased`, `SubmissionOutcome` | Nahkampf | Verletzungsmodell, Fight-Club-Regeln, Zeugen |
| `Surrendered`, `Fled`, `FrozeUp` | menschliche Gegner | Spielerzustand, Ermittlung (Festnahme) |
| `EvidenceCreated` | Waffe abgelegt, Blutspur, Einschussloch | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md), Ermittlung |
| `CombatEncounterEnded` | Bedrohungszustand beendet | Autosave, Ruhe-Erholung |
| `WorldConsequenceRecorded` | Tod, bleibende Verletzung, zerstörte Struktur | Weltzustand, NPC-Gedächtnis, Nemesis |

**Schnittstellen (konzeptionell):**

```text
HitResolver.Resolve(Projektilzustand | Schlag, Welt)        -> Trefferkette
Verletzungsmodell.Anwenden(Körper, Trefferkette, Zufall)    -> WundÄnderung
Verletzungsmodell.Fortschreiben(Körper, Zeit)               -> Zustandsänderung
Absichtsauflösung.Auflösen(Aktion, Kontext)                 -> Handlung | abgelehnt(Grund)
Kämpferzustand.Übergang(Handlung)                           -> neuer Zustand | abgelehnt(Grund)
```

Jede Funktion bekommt Zeit und Zufall hinein, liefert Ergebnis oder begründete Ablehnung (Grund als Kennung, wie bei der vorhandenen Interaktion: `placeholder:feedback.door.locked`). Das entspricht dem bestehenden Muster ‚Ergebniswerte statt Ausnahmen‘ (README §26.2: typsichere IDs, Ergebniswerte).

---

## 7. Fehlerfälle

| Fall | Erwartetes Verhalten | Status |
|---|---|---|
| Doppelte Trefferauflösung desselben Projektils (Netzwerk, Wiederholung) | Ein Treffer pro Projektil-ID; zweiter wird verworfen | 💡 |
| Speichern während unterbrochenem Nachladen | Nachladezustand wird gespeichert und nach Laden fortgesetzt | 📋 (`M-PD`: persistent und unterbrechbar) |
| Tod eines Beteiligten während eines Griffs | Griff endet; anderer Beteiligter wird freigegeben; Ereignis `GripReleased` | 💡 |
| Kämpfer wird durch Streaming entladen, während er verletzt ist | Körperzustand bleibt erhalten; Fortschreibung nach Zeitmodell des Weltzustands | 💡 (Modul 25) |
| Projektil trifft unbekannten Materialtyp | definiertes Standardmaterial mit Protokoll, kein Absturz | 💡 |
| Waffenstörung und Zielverlust gleichzeitig | Störung hat Vorrang, Auflösung der Handlung abgelehnt mit Grund | 💡 |
| Spieler stirbt in einer Phase, in der kein sicherer Checkpoint existiert | Softlock-Schutz: älterer sicherer Zustand oder manueller Spielstand | 🧭 (`23#365A`, siehe Modul 14) |
| Zwei Systeme wollen denselben Körper verändern (Treffer und Behandlung gleichzeitig) | Reihenfolge deterministisch nach Ereignisreihenfolge im Schritt | 💡 |
| Ungültige oder fehlende Moddaten (Waffen, Munition, Wesen) | Ablehnung mit Meldung, Basisspiel unverändert | 💡 (`M-MOD`, Mods ändern die Basis nicht) |

---

## 8. Testbarkeit und Portierbarkeit

1. **Alles im Kern ist mit Zeitschritt und Zufall von außen deterministisch** (`COMBAT-DEC-ZA-008`, `COMBAT-DEC-ZA-009`). Ein Kampf ist mit demselben Seed und denselben Eingaben reproduzierbar; ein Fehler wird zum Regressionstest (Gameplay-Systeme §17.2).
2. **Kleine Testkarten und Fixtures:** Die Quellen verlangen kleine deterministische Testkarten für Waffen und Ballistik, Nahkampf, Verletzung, KI und Tarnung (Gameplay-Systeme §17.2). [Modul 30](30-Testkatalog-und-Abnahmekriterien.md) leitet daraus die Testfälle ab.
3. **Zufall plattformunabhängig (💡):** Wie bei der NPC-Dokumentation wird eine ganzzahlige, plattformunabhängige Zufallsquelle vorgeschlagen, damit Replays zwischen Fedora und Windows übereinstimmen ([NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md)).
4. **Inspektoren:** Ein KI-Inspektor zeigt Wissen, Verdacht, Hörschätzung, Suchphase und Taktik (Gameplay-Systeme §7, `K-KI-012`); dasselbe Prinzip für Verletzungen (Wundliste mit Ursache) wird vorgeschlagen ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)).
5. **Sperren gegen Missbrauch:** Die Prüfung des Speichers im Kampf, des Checkpoints und des Todeszustands ist Teil der Tests ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)).

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-08` | Nutzen Nahkampf-Paare (Clinch, Boden, Würgegriff) die Sitzungen des gemeinsamen Interaktions-Frameworks, oder bekommt das Kampfsystem ein eigenes Paarmodell? | `COMBAT-DEC-ZA-011`, `-014` | ja |
| `CQ-09` | Schwellen und Übergänge der Wahrnehmungszustände, insbesondere ‚Detected‘ (Speichersperre ohne ausnutzbare Millisekunden) | `COMBAT-DEC-ZA-003`, `COMBAT-DEC-GP-028` | ja |
| `CQ-10` | Stufen und Schwellen des Spielerzustands: Namen, Zahl, Übergangsregeln, Anzeige (abgeleitet vs. eigene Größe) | `COMBAT-DEC-ZA-002`, `-015` | ja |
