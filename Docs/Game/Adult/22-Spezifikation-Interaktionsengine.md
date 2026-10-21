# Adult 22: Spezifikation Interaktionsengine

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register](14-Entscheidungsregister.md)). Kein Kanon. Keine Entscheidung über Engine-Technik (Solver-Bibliotheken, Unreal-Versionen) ist getroffen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Vorbedingung: der [Regelkern](20-Spezifikation-Regelkern-Zustimmung.md) erlaubt die Sitzung. Datenbasis: [Modul 21](21-Spezifikation-Datenmodell.md). Körper und Reaktion: [Modul 23](23-Spezifikation-Reaktions-und-Koerpersystem.md). Steuerung: [Modul 27](27-Spezifikation-Steuerung-UI-VR-Replay.md).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Gesamtkette](#3-gesamtkette)
4. [Sitzung und Zustandsautomat](#4-sitzung-und-zustandsautomat)
5. [Absicht und Aktionen](#5-absicht-und-aktionen)
6. [Pose, Kontakt, IK](#6-pose-kontakt-ik)
7. [Balance, Gewicht, Biomechanik](#7-balance-gewicht-biomechanik)
8. [Weichgewebe und Sekundärbewegung](#8-weichgewebe-und-sekundärbewegung)
9. [Mikrovariation und Determinismus](#9-mikrovariation-und-determinismus)
10. [Übergänge, Fallbacks, Fehlerzustände](#10-übergänge-fallbacks-fehlerzustände)
11. [Mehrere Teilnehmer und Beobachter](#11-mehrere-teilnehmer-und-beobachter)
12. [Umgebung und Möbel](#12-umgebung-und-möbel)
13. [Interaktionsplaner (Scheduler)](#13-interaktionsplaner-scheduler)
14. [Qualitätsstufen und Leistung](#14-qualitätsstufen-und-leistung)
15. [Debugging und Werkzeuge](#15-debugging-und-werkzeuge)
16. [Technische Komponenten](#16-technische-komponenten)
17. [Randfälle](#17-randfälle)
18. [Tests und Abnahmekriterien](#18-tests-und-abnahmekriterien)
19. [Ungeklärtes](#19-ungeklärtes)

---

## 1. Ziel und Abgrenzung

Ziel der Engine ist eine **Interaktionssitzung aus modularen Aktionen**, nicht eine Auswahl fertiger Szenen (E-IA-010, E-IA-011). Die Positionen entstehen als **kontinuierlicher Ganzkörperzustand**, nicht aus einer Liste (E-SO-007). Der Entwickler wünscht ‚fast wie in echt‘, mit Positionen „auch zwischen Positionen“ (35#446).

**Die Engine ist nicht Adult-spezifisch**: Umarmen, Tragen, Medizin, Ringen und Bodenkampf nutzen dieselben Bausteine; Adult erweitert sie (E-SO-023). Der Entwickler: ‚Teile davon sollen übernommen und für Adult erweitert werden‘.

Nicht Gegenstand dieses Moduls: Zustimmung ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)), Darstellung ([Modul 04](04-Steuerung-Darstellung-Kamera-Audio.md)).

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Keine Cuts, die Welt wird weiter simuliert | 📋 | E-IA-014 |
| Freeform statt Positionsmenü; Posen als Hilfen | 📋 | E-SO-007, E-SO-010 |
| Biomechanischer Bewegungsbereich pro Gelenk und Person | 📋 | E-SO-011 |
| Kontaktgraph mit Fläche, Kraft, Reibung, Material | 📋 | E-SO-013 |
| Qualitätsstufen = gleiche Simulation, andere Genauigkeit; Low darf nicht fake aussehen | 📋 | E-SO-014 |
| Kein Teleportieren; harter Reset praktisch nie | 📋 | E-SO-021 |
| NPC nutzt denselben Solver; Stärke entscheidet nie allein | 📋 | E-SO-020 |
| Jedes Mal etwas anders, aber deterministisch bei gleichem Seed | 📋 | E-SO-016 |
| Die konkrete Kette Intent → Kontakt → IK → Proportionen → Kollision → Balance → Mikrobewegung | 🧭 | E-SO-008 |
| Anfangsposition: selbst wählen oder NPC positioniert sich | ❓ | E-SO-009 |
| Steuerung ‚wie UFC-Spiele nur genauer‘ (Eingabekonzept) | ❓ / ✅ Hybrid für Adult | E-SO-030, E-ST-002 |
| Konkrete Solver, Rigs, Bibliotheken | ❓ | – |

## 3. Gesamtkette

🧭 (E-SO-008, bestätigt als Denkmodell in 35#447):

```
Absicht (Intent) und Pose-Ziele
   │
   ▼
Kontaktgraph (wer berührt wen, wo, wie stark)
   │
   ▼
Ganzkörper-IK  ◄── Körperproportionen / Gelenkgrenzen / Asymmetrie
   │
   ▼
Kollision + Weichgewebe
   │
   ▼
Balance / Gewicht / Abstützung
   │
   ▼
Mikrobewegung (deterministische Variation)
   │
   ▼
Ausgabe: Pose → Animation/Rendering/Audio/Reaktionen
```

Grundsatz (E-SO-008, 35#283 Runde 10B): **Animation bestimmt grobe Bewegung, IK korrigiert Kontakte, Physik wirkt nur sekundär** – erweitert durch die Freeform-Entscheidung: die Pose entsteht aus Absichten und Constraints, nicht aus einer fixen Clip-Auswahl. Wie viel davon authored (Mocap, Motion Matching) und wie viel prozedural ist, bleibt **Prototyp-Frage** (E-PA-026 nennt Animation „hybrid“).

## 4. Sitzung und Zustandsautomat

### 4.1 Zustände (🧭 Vorschlag, abgeleitet aus E-IA-041 und 28#25)

Der Rahmenentwurf nannte ca. 28 Zustände; die Liste ist nicht im Interview bestätigt. Diese Dokumentation schlägt eine **kleinere logische Fassung** vor:

| Zustand | Bedeutung |
|---|---|
| `Unavailable` | Voraussetzungen nicht erfüllt ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)) |
| `Approach` | Annäherung, soziale Phase, Dialog |
| `Offer` | Angebot/Anfrage, Zustimmung wird gebildet |
| `Transition` | Übergang in Nähe/Position (Heimweg, Zimmer, Hinlegen) |
| `Active` | laufende Sitzung mit Aktionsschichten |
| `Adjust` | Haltungs-/Intensitätswechsel innerhalb des Rahmens |
| `Pause` | Pausen (Typen: sozial, körperlich, Umwelt) |
| `Interrupted` | Unterbrechung von außen (Klingel, Tür, Alarm, Angriff) |
| `Recover` | Erholung/Ruhephase innerhalb der Sitzung |
| `Ending` | Auslaufen (nicht abrupt) |
| `Aftercare` | Nachspiel (Nähe, Gespräch, Duschen, Schlaf) |
| `SafeExit` | sicherer Ausstieg (bei Stopp, Notfall) |
| `Failed(reason)` | technischer/inhaltlicher Fehlerzustand mit Fallback |
| `Completed` | abgeschlossen, Ergebnisse persistiert |

Regeln (🧭):
- `HardStop` führt aus **jedem** Zustand nach `SafeExit` ([Modul 20, Abschnitt 8](20-Spezifikation-Regelkern-Zustimmung.md#8-stopp-unbehagen-widerruf)).
- Dialog ist in `Pause` und `Aftercare` Standard; in `Active` nur kontextabhängig (28#25 ‚Dialog nur in Pausen standardmäßig‘, E-IA-041).
- Skip/Zeitraffer löst einen **narrativen Ausgang** auf; er überspringt nie den Regelkern (E-IA-041).
- Die Sitzung kann beliebig kurz oder lang sein; es gibt **keinen Fortschrittsbalken** und kein Minispiel (E-IA-010).
- Eine Sitzung darf ‚nicht gut laufen‘ (35#444/47–50) und trotzdem sauber enden.

### 4.2 Einstieg

📋 Der Beginn folgt dem Alltag: Verabredung → Weg → Ort → sinnvoller Übergang → Beginn (E-IA-013). ‚Direkt nur Sex‘ ist ein weiterer sozialer Intent, wenn der NPC offen ist (E-IA-015). Es gibt keinen Szenenwechsel durch Schnitt (E-IA-014).

### 4.3 Ende

📋 Eine Sitzung endet nicht automatisch beim Höhepunkt; Orgasmus ist ein Ereignis innerhalb der Sitzung, nicht ihr Ende (E-IA-019, E-IA-033). Sie darf ohne Orgasmus normal enden. Danach: Nähe, Gespräch, Schlaf, Duschen, Essen, Anziehen, gemeinsames Einschlafen, ‚Morgen danach‘ (E-IA-020).

## 5. Absicht und Aktionen

### 5.1 Absichtsebenen

| Ebene | Frage | Quelle |
|---|---|---|
| `InteractionIntent` | Was ist gewollt und zulässig? | 35#459, E-SO-020 |
| `Physics` | Welche Kräfte treten auf? | 35#459 |

Grundsatz: Eine **vereinbarte dominante Interaktion** darf physisch kraftvoller aussehen; **Physik entscheidet nie über Grenzen**, Zustimmung entscheidet das (E-SO-020).

### 5.2 Aktion

🧭 `Action = InteractionType + TargetBodyRegion + Tool/BodyPart + Parameters` (E-IA-025). Mehrere Aktionen laufen gleichzeitig als **Layer** (Hände, Mund, Becken, Items) mit Priorität und Konfliktauflösung (E-IA-036, E-SO-037).

Parameter (mehrdimensional, E-IA-025/E-SO-034): Geschwindigkeit, Kraft, Druck, Amplitude/Bewegungsumfang, Beschleunigung, Rhythmus, Dauer, Richtung. **Geschwindigkeit ≠ Kraft**; **Intensität ≠ Kontrollabgabe**; **Intensität ≠ Praktik** (E-NP-019).

### 5.3 Wer führt?

- Spieler und NPC liefern Absichten; der **Solver kombiniert** sie zu einer gemeinsamen Bewegung (E-SO-034, 35#609).
- NPCs wählen eigene Wege, passen Rhythmus an und korrigieren (E-IA-018, E-SO-025).
- Stärke entscheidet nie allein (E-SO-020).

## 6. Pose, Kontakt, IK

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| IE-PO-01 | Der Pose-Zustand ist je Teilnehmer ein kontinuierlicher Ganzkörperzustand (Gelenkwinkel, Orientierung, Stützpunkte). | E-SO-007 |
| IE-PO-02 | **Pose-Templates** sind Hinweise für den Solver und für die Aktionsbibliothek; sie sind nie Voraussetzung. | E-SO-010 |
| IE-PO-03 | Zwischenpositionen und Mischformen sind gültig. | E-SO-033 |
| IE-PO-04 | Der **Kontaktgraph** hat pro Kontakt: Flächengröße, Normalkraft, Reibung, Materialien (Haut, Stoff, Leder, Metall …), Temperatur, Dauer. | E-SO-013 |
| IE-PO-05 | Kontakte haben **Anker**, die aus Körpermaßen berechnet werden (Toleranzen, primär strenger), damit Hände, Münder, Körper zueinander passen; Asymmetrie fließt ein. | E-SO-015, 35#283 |
| IE-PO-06 | Paar- und Gruppenanimationen von Hauptfiguren dürfen handgefertigt sein; für Population entstehen sie über Retargeting + Validator. | 35#283 |
| IE-PO-07 | Ein Validator bewertet Kompatibilität (Beispielwert „94 %“ im Gespräch ist **nicht** beschlossen); unterhalb der Toleranz wird eine **Alternative** gewählt, nicht ein Clipping akzeptiert. | E-IA-010, 35#283 |
| IE-PO-08 | Kleidung ist **Schicht**: sie begrenzt Beweglichkeit, verteilt Druck, wird verschoben/abgelegt und bleibt dort liegen, wo sie abgelegt wurde; sie verschwindet nie. | E-IA-016, E-SO-028 |
| IE-PO-09 | Schmuck bleibt am Körper (physisch vorhanden). | E-IA-016 |
| IE-PO-10 | Start aus der **aktuellen Haltung** der Beteiligten, nicht aus einer Referenzpose. | 35#463/101 |

Offen: Beginn der Position (selbst wählen vs. NPC positioniert sich) ist **nicht entschieden** (E-SO-009).

## 7. Balance, Gewicht, Biomechanik

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| IE-BM-01 | Jeder Gelenkbereich ist **individuell** (Flexibilität, Verletzung, Alter, Training). | E-SO-011 |
| IE-BM-02 | **Widerstand wächst allmählich** an der Grenze; Überschreiten führt zu Unbehagen, Verletzung, im Extremfall Knochenbruch (auch im Bodenkampf). | E-SO-011 |
| IE-BM-03 | Schwerpunkt, Abstützen, Gewichtsunterschiede, Hebel und Reibung bestimmen, welche Haltungen **möglich** sind. | E-SO-012 |
| IE-BM-04 | Erschöpfung, Krampf, Haltungsermüdung verändern die Pose im Verlauf. | E-KO-024, E-IA-036 |
| IE-BM-05 | Gewichtsverlagerung geht Armbewegungen voraus (natürliche Bewegung). | E-SO-016 |
| IE-BM-06 | Kräfte folgen den **normalen Körperwerten** (Muskelkraft, Gewicht, Ausdauer, …); keine Adult-Sonderwerte. | E-SO-029 |
| IE-BM-07 | Wenn eine Konfiguration physisch unmöglich ist, **wechselt** die Engine (andere Variante, Korrektur, Abwechseln) oder die Sitzung endet sauber. | E-IA-008, E-SO-021 |

## 8. Weichgewebe und Sekundärbewegung

Die Details stehen in [Modul 23](23-Spezifikation-Reaktions-und-Koerpersystem.md#4-weichgewebe-und-sekundärbewegung). Schnittstelle zur Engine:

- Weichgewebe ist **sekundär** zum Skelett (E-SO-013), reagiert auf **Beschleunigung, Kontakt, Druck** und Haltung.
- Sekundärbewegung wird **pro Körperteil** aus individueller Anatomie berechnet, nicht aus Größe pauschal (E-SO-024).
- Sie wirkt auch bei rhythmischer Bewegung, subtil und realistisch (E-SO-006).
- Muskelaktivierung wird aus Pose, Bewegung und Last **approximiert** (‚90 % Realismus reicht‘, E-SO-004, E-SO-015).

## 9. Mikrovariation und Determinismus

| Anforderung (🧭) | Basis |
|---|---|
| Jede Sitzung hat einen `session_seed`; Mikrovariation (Kopf-, Handplatzierung, Blick, Atmung, Gewichtsverlagerung, Timing, Pausen) ergibt sich daraus. | E-SO-016, K§18 |
| Gleiche Sitzung + Speichern/Laden → **gleiche** Mikrovariation. | E-SO-016 |
| Der Kern ist deterministisch; **sichtbare Physik** (Stoff, Haar, kosmetische Details) darf nichtdeterministisch sein. | E-SP-001 |
| Jedes Mal etwas anders, auch bei gleichen Personen. | 35#444/100 |

## 10. Übergänge, Fallbacks, Fehlerzustände

### 10.1 Übergänge

Positionswechsel sind **Erstklass-Übergänge** (K§18, E-IA-041): fließend, mit Gewichtsverlagerung; ohne Szenenreset (E-SO-033).

### 10.2 Fallback-Leiter (📋 E-SO-021)

1. **Mikrokorrektur** (kleine Anpassung der Pose).
2. **IK-Lockerung** (Toleranzen vergrößern, Constraints priorisieren).
3. **Neuberechnung** (neue Zielpose aus aktueller Lage).
4. **Harter Reset** – praktisch nie; **kein sichtbares Teleportieren**; im Release lieber **Interaktion sauber beenden** (35#459).

### 10.3 Fehlerzustände (🧭)

| Fehler | Reaktion |
|---|---|
| fehlende Übergangsanimation | Fallback-Übergang, sonst `SafeExit` (kein Softlock) |
| Solver ohne Lösung | Leiter nach 10.2 |
| Selbstkollision/Mehrkörperkollision | Korrektur; LOD der Kollisionsform nach Wichtigkeit (E-SO-036) |
| Platzmangel | Alternative Haltung, sonst Abbruch |
| Möbelbruch | Welt-Zustand ändern, neue Haltung |
| Umweltunterbrechung | `Interrupted`, Weltpriorität (Klingel, Eintreten, Stromausfall, Feueralarm, Monsterangriff; E-IA-036) |

„Problematische Pose“ bedeutet **technisch** (Clipping, unlösbar), nicht inhaltlich (35#459).

## 11. Mehrere Teilnehmer und Beobachter

| Anforderung (🧭) | Basis |
|---|---|
| Teilnehmerzahl 2..N; jede Konstellation nutzt dieselbe Engine. | E-SP-007, E-NP-022 |
| Rollen wechseln pro Moment; Initiative wird pro Aktion gespeichert. | E-IA-025 |
| Eine nicht unterstützte Konstellation → **Abwechseln**, Beobachter-Rolle oder getrennte Varianten, nie kaputte Animation. | E-IA-008 |
| Beobachter sind echte Figuren mit eigenem Gedächtnis; sie können später beitreten oder gehen. | E-IA-003 |
| Gruppen: paarweise Beziehungen bleiben; jede Beteiligung ist einzeln zustimmungspflichtig. | E-NP-022, [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) |

## 12. Umgebung und Möbel

| Anforderung (🧭) | Basis |
|---|---|
| Ort ist **Umgebungskontext**, kein Szenentyp; es gibt keinen speziellen „Adult-Slot“ im Raum. | E-IA-035 |
| Möbel sind **Interaktionsflächen** (Eigenschaften: Sitz-/Stützfläche, Weichheit, Greifbarkeit, Zerbrechlichkeit); Bett verformt sich; Möbel können bei Belastung beschädigt werden. | E-SO-033, E-IA-035 |
| Fahrzeuge werden anhand ihres realen Innenraums berücksichtigt; Outdoor-Untergrund, Wetter, Tageszeit beeinflussen Körper und Sichtbarkeit. | E-IA-035 |
| Türen, Schlösser, Vorhänge sind normale Weltaktionen. | E-IA-035 |
| **Privacy Score** dynamisch aus Sichtlinie, Fenstern, Licht, Geräuschen, Zeitplan der NPCs. | E-IA-035 |
| Zeugenerkennung: Sichtlinie + Gehör + Aufmerksamkeit; Beobachter reagieren nach Persönlichkeit, Beziehung, Recht. | E-WE-005, E-WE-021 |
| Orts-Gedächtnis: Orte merken sich Ereignisse und Zustände. | E-IA-009 |
| Adult braucht **keine speziell gebauten Räume** (jeder geeignete Raum). | E-IA-035 |

## 13. Interaktionsplaner (Scheduler)

🧭 Abgeleitet aus dem Assistenten-Audit (35#706, E-SO-037): Prioritäten, damit mehrere Schichten stabil bleiben.

| Priorität | Inhalt |
|---|---|
| 1 | Sicherheit (Stopp, Notfall, Bewusstlosigkeit) |
| 2 | Balance, Kollision, Stabilität |
| 3 | Kontakt-Constraints (Pflichtkontakte) |
| 4 | Aktionsabsichten |
| 5 | Sekundäraktionen, Items |
| 6 | Mikrovariation, kosmetische Details |

Weitere Anforderungen: autoritativer Interaktionszustand im Mehrspielermodus (E-SP-005), Importance-/Distanz-LOD (E-SO-037), automatisierte Stresstests (E-SP-001).

## 14. Qualitätsstufen und Leistung

| Stufe | Beschreibung (🧭) |
|---|---|
| Low | vereinfachte Kollision/Weichgewebe, aber **nicht fake** (E-SO-014) |
| Medium / High | mehr Kollisionskörper, genauere Weichgewebe |
| Ultra / Cinematic | höchste Genauigkeit, auch für Replay-Re-Render (E-RP-002) |

- Alle Stufen simulieren **dasselbe** Verhalten; nur die Genauigkeit unterscheidet sich (35#459).
- Performance-Budgets sind Ziele **pro Subsystem** (E-SP-001); Zahlen existieren nicht. ‚Nicht jeder hat Spitzenhardware‘ (E-PR-011).
- Entfernung und Wichtigkeit steuern den Detailgrad (E-SO-037).

## 15. Debugging und Werkzeuge

| Werkzeug (📋 E-SO-022) | Funktion |
|---|---|
| Pose-/Kontakt-Debugger | Skelett, Kontaktflächen, Schwerpunkt, Kraftvektoren, IK-Ziele, Gelenkgrenzen, Kollisionsformen, Weichgewebezonen |
| Solver-Log | speichert Körpermaße, Pose, Gelenkwinkel, Kontaktgraph, Kräfte, Umgebung, Solver-Zustand (Debug-Builds) |
| Gate-Inspector | Begründung der Prüfkette ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)) |
| Adult Debug Inspector | Präferenzen, Reaktionsgraph, Gesundheit (nur Entwicklung, E-NP-021) |
| Mod-Editor-Integration | Kompatibilitätsprüfung, Validator |

## 16. Technische Komponenten

🧭 (engine-neutraler Kern, Unreal-Adapter; E-PA-025/026):

| Komponente | Verantwortung |
|---|---|
| `SessionManager` | Sitzungen, Zustandsautomat, Teilnehmer |
| `IntentResolver` | kombiniert Absichten, Rollen, Initiative |
| `ActionLayerStack` | Aktionsschichten, Prioritäten |
| `PoseGoalSolver` | Zielposen aus Absichten |
| `ContactGraph` | Kontakte, Anker, Kräfte |
| `BodyKinematics` | Gelenkgrenzen, Proportionen, Asymmetrie |
| `BalanceModel` | Schwerpunkt, Abstützung |
| `SoftTissueInterface` | Schnittstelle zum Weichgewebe-Solver ([Modul 23](23-Spezifikation-Reaktions-und-Koerpersystem.md)) |
| `FallbackLadder` | Fehlerbehandlung |
| `SessionSeedService` | deterministische Variation |
| `EnvironmentQuery` | Flächen, Platz, Privatheit |
| `UnrealAdapter` | Rig/IK/Animation/Physik der Engine |

Hinweis: Welche Teile im Kern (portabel testbar) und welche im Unreal-Adapter liegen, muss im Prototyp entschieden werden; die Dokumentation gibt nur die Trennlinie vor (‚Regeln, Zustände, Entscheidungen im Kern, Rig/IK/Physik im Adapter‘).

## 17. Randfälle

Siehe [Modul 31](31-Randfaelle-Katalog.md#3-interaktionsengine). Auszug:
- Teilnehmer mit stark unterschiedlicher Größe oder Gewicht.
- Verletzter Teilnehmer, eingeschränkte Gelenke, Gipsarm.
- Ungewöhnliche Orte (Fahrzeug, enge Räume, Hanglage).
- Zwei Aktionsschichten wollen dieselbe Hand.
- Item geht verloren oder wird fallen gelassen.
- Möbelbruch mitten in der Sitzung.
- Netzwerk-Latenz im Koop.
- Laden aus einem Savegame, das mitten in einem `SafeExit` entstand.

## 18. Tests und Abnahmekriterien

Siehe [Modul 29](29-QA-Testkatalog.md#3-interaktionsengine) und [Modul 30](30-Abnahmekriterien.md#3-interaktionsengine).

| Test | Erwartung |
|---|---|
| Gleiche Eingaben, gleicher Seed | gleiche Pose-Folge (innerhalb Toleranz), kosmetische Physik ausgenommen |
| 1.000 zufällige Konstellationen (Größe, Gewicht, Proportionen) | keine Penetration über Toleranz, kein Softlock |
| Abbruch aus jedem Zustand | `SafeExit` erreicht |
| fehlende Übergangsanimation | Fallback ohne Teleport |
| Unmögliche Gelenkposition | Alternative oder sauberes Ende |
| Möbel bricht | neuer Zustand, Persistenz des Schadens |
| Qualitätsstufe Low vs. Cinematic | gleiche Entscheidungen, unterschiedliche Genauigkeit |
| Layer-Konflikt | Prioritäten halten |

## 19. Ungeklärtes

| Frage | Bezug |
|---|---|
| Wer wählt die Anfangsposition (Spieler oder NPC)? | E-SO-009 |
| Konkrete Steuerung der Bewegungsintentionen (Stick-Belegung, Modifier, Menüs) | [Modul 27](27-Spezifikation-Steuerung-UI-VR-Replay.md), E-SO-030 |
| Verhältnis von Mocap, Motion Matching, Prozeduralem | E-PA-026 |
| Echtzeit-Budgets für Weichgewebe, Mehrkörperkollision | Prototyp |
| Welche Sitzungsparameter in den Replay-Datenstrom gehören | [Modul 27](27-Spezifikation-Steuerung-UI-VR-Replay.md) |
| Wie Hauptspiel-Aktionen (Umarmen, Tragen, Ringen) den Kern teilen | E-SO-023 |
| Zeitmaßstab einer Sitzung | [K15](19-Konflikt-und-Widerspruchsmatrix.md#k15-zeitmaßstab) |
