# Fahrzeuge und Verkehr 04: Fahrphysik und Fahrverhalten

> [!IMPORTANT]
> **Es gibt kein Fahrmodell und keine Konstanten.** Beschlossen sind Anforderungen (manuelles Getriebe mit Kupplung, echte Nutzung des Antriebsstrangs, Wetterwirkung). Die Grundlagen stehen in [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) und werden hier nicht wiederholt. Dieses Modul ergänzt die **Schichtung**, die **Schnittstellen zu Straße und Wetter** und die **Prüfbarkeit**. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Schichtung der Fahrphysik](#2-schichtung-der-fahrphysik)
3. [Straße, Wetter und Jahreszeit als Eingaben](#3-straße-wetter-und-jahreszeit-als-eingaben)
4. [Fahrphysik für Spieler, NPC-Nahbereich und Ferne](#4-fahrphysik-für-spieler-npc-nahbereich-und-ferne)
5. [Prüfbarkeit](#5-prüfbarkeit)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Getriebe | Kupplung, Schaltvorgang, Drehzahl, Motorbremse, Schlupf, Abwürgen; freie Gangwahl mit Folgen für Drehzahl, Motorbremse, Instabilität und Verschleiß | `W-FP-001` |
| Fehlbedienung | Ein Schaltfehler bei hoher Geschwindigkeit schadet Kupplung und Getriebe; Leistungsänderungen können auftreten; der Verschleiß akkumuliert realistisch und zerstört die Kupplung nicht nach wenigen Fehlern | `23#1025E` Nr. 15; `M-WB17` Block 5; `M-DC31` §30 |
| Kupplung | deaktivierbar, Schalten bleibt; eine Kupplungsautomatik darf nur die Kupplung übernehmen | `23#1025E` Nr. 14; `M-WB17` Block 5 |
| Modell | Antriebsstrang, Reifen, Federung, Lenkung, Differenzial, Bremsen/Sicherheitssysteme, Aerodynamik, Last, Schaden; Reifenmodell mit Mischung, Temperatur, Druck, Verschleiß, Nässe | `W-FP-003` |
| Fidelity | Straßenfahren relativ realistisch, Rennsport deutlich simulationslastiger | `W-FP-002`, `W-FP-008` |
| Wetter | Regen, Schnee, Eis, Aquaplaning, Sicht und Reifentemperatur beeinflussen die Fahrzeugphysik, im Rennsport noch mehr; Jahreszeiten wirken auf Verkehr und Straßen | `23#1025E` Nr. 48, 49 |
| Unfall | Crash-Energie und Sicherheitsausrüstung verzahnen sich mit dem Verletzungssystem; kein magisches Lenken in der Luft, kein Umdreh-Reset | `W-FZ-015` |
| Fahrer | Fahrzeug-Vertrautheit ohne magisches Wachstum von Werten; Bergen, Abschleppen, Winde | `W-FZ-010` |
| Offen | Motor-, Getriebe-, Kupplungs- und Reifenkonstanten, Schleifpunkt-Algorithmus, Verkehrsmodell | `W-FP-010` |

---

## 2. Schichtung der Fahrphysik

> **💡 VORSCHLAG.** Eine Schichtung, die zu `CLAUDE.md` §5 passt (portabler Kern für Antriebsstrangteile, Unreal-Adapter für Physik). Es ist keine Entscheidung über die Physik-Engine.

```text
Eingabe / Fahrerabsicht   (Spieler: Controller; NPC: Verkehrs-KI)  -> Modul 05, 13
        |
Fahrerabsicht -> Bedienung (Gas, Bremse, Lenkung, Kupplung, Gang)
        |
Antriebsstrang            (Motorkennlinie, Kupplung, Getriebe, Differenzial; Drehmoment zu den Rädern)
        |
Rad / Reifen / Aufstandsfläche   (Schlupf, Haftung, Temperatur, Verschleiß; Straßenoberfläche aus Modul 10)
        |
Chassis / Federung / Masse / Aerodynamik   (Gewichtstransfer, Ladung)
        |
Kollision und Schaden     (Energie -> Modul 07; Verletzung -> Kampfsystem 13)
```

| Schicht | Ort der Umsetzung (Vorschlag) | Begründung |
|---|---|---|
| Antriebsstrang (Motorkennlinie, Kupplung, Getriebe, Differenzial) | portable C++20-Bibliothek, fester Zeitschritt, Zufall und Zeit von außen | deterministisch standalone testbar, wie in `CLAUDE.md` §5 für ‚portions of drivetrain logic‘ vorgesehen |
| Rad, Federung, Kollision, Aufstandsfläche | Unreal-Adapter | engineabhängig (Physik, Spuren, Materialien) |
| Fahrerabsicht | Schnittstelle zwischen Eingabe/KI und Fahrzeug | derselbe Antriebsstrang soll Spieler und NPC-Fahrer bedienen (Modul 14) |
| Straßenzustand, Wetter | Datenquellen, die die Reibung beeinflussen | Modul 10, 15 |

Welche Unreal-Fahrzeugphysik dafür benutzt wird, ist **nicht entschieden** (`CQ-10`). Weder Chaos Vehicles noch eine eigene Lösung ist beschlossen; beide Wege sind zu prüfen, bevor etwas festgelegt wird (Modul 25).

---

## 3. Straße, Wetter und Jahreszeit als Eingaben

> **💡 VORSCHLAG.**

Die Fahrphysik braucht pro Stelle der Straße und des Geländes eine Beschreibung des Untergrunds. Die Quellen verlangen die Wirkung (Nässe, Schnee, Eis, Aquaplaning, Steigung im Gebirge; `W-FP-003`, [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §3), nennen aber keine Werte.

| Eingabe | Quelle der Eingabe | Wirkung auf die Physik (qualitativ, aus den Anforderungen) |
|---|---|---|
| Belag der Straßenklasse (Asphalt, Schotter, Feldweg, Pflaster) | Straßennetz ([Modul 10](10-Strassennetz-und-Strassentypen.md)) | Haftung, Rollwiderstand, Geräusch; Geländetauglichkeit (`W-FZ-009`) |
| Nässe, Schnee, Eis, Pfützen | Wetterzustand (Weltlayer), lokal | Haftung, Aquaplaning, Sicht; Reifentemperatur |
| Steigung, Kurvenradius, Querneigung | 3D-Trasse ([Modul 24](24-WorldForge-Schnittstelle.md)) | Gewichtstransfer, Bremsweg, Kühlung, Kupplungsbelastung |
| Jahreszeit, Höhenlage | Weltzeit, Höhe | Schneelage in der Mountain Region, Eis |
| Straßenschäden, Baustellen | Weltzustand ([Modul 15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md)) | Haftung, Hindernisse |

**Beispiele für Prüffragen** (keine Anforderungen): Verlängert nasser Untergrund den Bremsweg monoton? Verschlechtert Schnee die Haftung gegenüber Nässe? Ändert sich die Reifentemperatur plausibel mit Bremsen und Kurvenfahrt?

Wie genau Wetter, Straßenoberfläche und Reifenmodell zusammenspielen, ist offen (`CQ-12`).

---

## 4. Fahrphysik für Spieler, NPC-Nahbereich und Ferne

> **💡 VORSCHLAG.** Die Quellen sagen nicht, ob NPC-Fahrer dieselbe Reifen-, Schlupf- und Kupplungsphysik benutzen ([Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §2: nicht festgelegt). Sie legen aber fest, dass Anfänger manuelle Autos abwürgen können und dass das Wetter das Fahrverhalten ändert (`W-FP-009`).

| Ebene | Vorschlag | Grund |
|---|---|---|
| Spielerfahrzeug | volle Fahrphysik (Fidelity-Stufe nach Kontext) | Kernsystem (`W-FZ-001`) |
| NPC-Fahrzeug nahe beim Spieler | vereinfachtes, wegbasiertes Fahrmodell, das dieselben **sichtbaren Folgen** zeigt (Abwürgen, längerer Bremsweg bei Nässe, Rutschen) und bei Kollision oder Berührung auf volle Physik umschaltet | Kosten (Modul 27), Glaubwürdigkeit (`N-LT-011`) |
| NPC-Fahrzeug in der Ferne | kein Fahrmodell, abstrakter Zustand ([Modul 26](26-Simulationsstufen-LOD-und-Streaming.md)) | `N-LE-013` |

Der Übergang zwischen den Ebenen braucht eine Regel für Zustandsübernahme (Position, Geschwindigkeit, Gang, Schaden), damit ein Fahrzeug beim Umschalten nicht springt. Diese Regel ist zu entwerfen und zu prüfen (`CQ-14`).

---

## 5. Prüfbarkeit

Die Fahrphysik ist ein Bereich, in dem Gefühl und Zahlen auseinanderfallen. Der Auftrag und `CLAUDE.md` verlangen, keine Zahlen zu erfinden und Ergebnisse nie zu fabrizieren. Das führt zu drei Testarten, die in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md) ausgearbeitet sind:

1. **Eigenschaftstests im portablen Kern** (zum Beispiel: Gangwechsel ohne Kupplung erzeugt Schaden oder Drehzahlsprung; derselbe Eingabeverlauf ergibt dasselbe Ergebnis).
2. **Szenarientests in Unreal** auf der Fahrzeug-Testkarte (`M-PD`: kleine deterministische Testkarten für Fahrzeuge).
3. **Manuelle Gefühlsabnahme** durch den Entwickler (`M-PD`: der Entwickler ist Endabnehmer bei subjektiven Ergebnissen).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-PH-001 | Manuelles Getriebe simuliert Kupplung, Schalten, Drehzahl, Motorbremse, Schlupf und Abwürgen; freie Gangwahl mit Folgen | 📋 BESCHLOSSEN | `W-FP-001` | FZ | Konstanten offen (`VEH-DEC-PH-013`) |
| VEH-DEC-PH-002 | Straßenfahren und Motorsport benutzen zwei Wiedergabetreue-Stufen derselben Fahrphysik, die über Profile unterschieden werden | 💡 VORSCHLAG | Ableitung aus `W-FP-002`, `W-FP-008` | FZ | Stufenzahl: `CQ-02` |
| VEH-DEC-PH-003 | Die Physik nutzt tatsächlich Antriebsstrang, Reifen, Federung, Lenkung, Differenzial, Bremsen und Sicherheitssysteme, Aerodynamik, Last und Schaden; das Reifenmodell kennt Mischung, Temperatur, Druck, Verschleiß und Nässe | 📋 BESCHLOSSEN | `W-FP-003` | FZ | Tiefe im Straßenverkehr: `CQ-12` |
| VEH-DEC-PH-004 | Fehlschaltungen und Missbrauch (zum Beispiel Herunterschalten bei hoher Geschwindigkeit) können Kupplung, Getriebe, Antriebsstrang und Motor beschädigen und Kontrollverlust verursachen | 📋 BESCHLOSSEN | `23#1025E` Nr. 15; `M-WB17` Block 5 | FZ ZV | Schwellen offen |
| VEH-DEC-PH-005 | Die Kupplung ist deaktivierbar, das Schalten bleibt; eine Kupplungsautomatik darf nur die Kupplung automatisieren | 📋 BESCHLOSSEN | `23#1025E` Nr. 14; `M-WB17` Block 5 | FZ ST | – |
| VEH-DEC-PH-006 | Getriebemodi: Automatik, Handschaltung mit Auto-Kupplung, mit digitaler Kupplung, mit voller Kupplung; Lenkrad, Pedale und H-Schalter als Enthusiastenoption | 🧭 RICHTUNG | `W-FP-006` | FZ ST | Ergänzt durch `W-FP-005` (DualSense) |
| VEH-DEC-PH-007 | Regen, Schnee, Eis, Aquaplaning, Sicht und Reifentemperatur beeinflussen die Fahrzeugphysik, im Rennsport noch stärker | 📋 BESCHLOSSEN | `23#1025E` Nr. 48 | FZ WET | Zusammenspiel: `CQ-12` |
| VEH-DEC-PH-008 | Jahreszeiten beeinflussen Verkehr und Straßen, besonders in der Mountain Region, aber auch in der übrigen Welt | 📋 BESCHLOSSEN | `23#1025E` Nr. 49 | FZ WET VKR | – |
| VEH-DEC-PH-009 | Crash-Energie und Sicherheitsausrüstung verzahnen sich mit dem Verletzungssystem; kein magisches Lenken in der Luft, kein Umdreh-Reset | 📋 BESCHLOSSEN | `W-FZ-015` | FZ MED | Schnittstelle Modul 07, [Kampfsystem 13](../Kampfsystem/13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| VEH-DEC-PH-010 | Ladung verändert die Masse; Kupplung, Motor und Getriebe haben einen thermischen Zustand und Verschleiß | 📋 BESCHLOSSEN | Gameplay-Systeme §6.4 (`M-PD`) | FZ INV | – |
| VEH-DEC-PH-011 | Fahrzeug-Vertrautheit ohne magisches Wachstum von Fahrzeugwerten; Bergen, Abschleppen und Winde sind vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010` | FZ | Grenze der Vertrautheit offen (`W-FZ-014`) |
| VEH-DEC-PH-012 | Der Antriebsstrang (Motorkennlinie, Kupplung, Getriebe, Differenzial) wird als portabler C++20-Kern mit festem Zeitschritt gebaut; Räder, Federung und Kollision liegen im Unreal-Adapter | 💡 VORSCHLAG | `CLAUDE.md` §5 | FZ UE | Hängt an `CQ-10` |
| VEH-DEC-PH-013 | Motor-, Getriebe-, Kupplungs- und Reifenkonstanten sowie der Schleifpunkt-Algorithmus sind nicht beschlossen | ❓ OFFEN | `W-FP-010`; `D15-RV` | FZ | `CQ-13`; erst nach Prototyp |
| VEH-DEC-PH-014 | Welche Unreal-Fahrzeugphysik (Chaos Vehicles, Anpassung, eigene Lösung) verwendet wird, ist nicht entschieden | ❓ OFFEN | `CLAUDE.md` §5 (Unreal-Version offen); `WF-UE` | FZ UE | `CQ-10` |
| VEH-DEC-PH-015 | Jede Straßen- und Geländeoberfläche trägt Haftungs- und Zustandsparameter (trocken, nass, Schnee, Eis, Schlamm), die Wetter und Straßenzustand verändern | 💡 VORSCHLAG | Ableitung aus `W-FP-003`, `23#1025E` Nr. 48 | FZ SN WET | Werte offen |
| VEH-DEC-PH-016 | NPC-Fahrzeuge im Nahbereich nutzen ein vereinfachtes, wegbasiertes Fahrmodell mit denselben sichtbaren Folgen und wechseln bei Berührung auf volle Physik | 💡 VORSCHLAG | Ableitung aus `W-FP-009`, `N-LT-011` | FZ VKR | Zustandsübernahme: `CQ-14` |
| VEH-DEC-PH-017 | Ob NPC-Fahrer dieselbe Reifen-, Schlupf- und Kupplungsphysik wie der Spieler verwenden, ist nicht festgelegt | ❓ OFFEN | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §2; `W-FP-010` | FZ VKR | `CQ-14` |
| VEH-DEC-PH-018 | Wie stark Schaden das Fahrverhalten verändert (Leistungsverlust, Ziehen, Reifenfehler), hängt am unbeantworteten Detailgrad des Schadensmodells | ❓ OFFEN | `W-FZ-013`; `D15-RV` | FZ ZV | `CQ-23` |
| VEH-DEC-PH-019 | Ausbrechen und Übersteuern sind physikalisch möglich, nicht jeder Fehler wird automatisch stabilisiert; realistisch hohe Höchstgeschwindigkeiten statt einer niedrigen globalen Arcade-Grenze | 📋 BESCHLOSSEN | `M-DC31` §32; `M-PS31` §38 | FZ PHY | Kein Wert belegt |
| VEH-DEC-PH-020 | Straßen-, Semislick-, Winter- und Geländereifen verhalten sich tatsächlich verschieden | 📋 BESCHLOSSEN | `M-DC31` §32; `M-PS31` §42 | FZ WET | Reifenwahl und Jahreszeit: Modul 10, 15 |
| VEH-DEC-PH-021 | Motor- und Getriebetemperatur spielen bei Rennen, Volllast, Hitze und Tuning eine größere Rolle als im normalen Fahren; Kupplungsverschleiß und -temperatur akkumulieren realistisch und zerstören die Kupplung nicht nach wenigen Fehlern | 📋 BESCHLOSSEN | `M-DC31` §30, §32; `M-PS31` §39 | FZ ZV | Grenzt `VEH-DEC-PH-004` ein |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Fahrzeugphysik-Grundlage (Unreal-System oder eigene Lösung) | `CQ-10` |
| Zahl und Zuschnitt der Wiedergabetreue-Stufen | `CQ-02` |
| Tiefe von Reifen- und Wettermodell im Straßenverkehr | `CQ-12` |
| Konstanten und Schleifpunkt (nach Prototyp) | `CQ-13` |
| Physik der NPC-Fahrer und Zustandsübernahme beim Umschalten | `CQ-14` |
