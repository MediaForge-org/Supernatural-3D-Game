# Kreaturen 02: Leitsätze und Abgrenzung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt die Leitsätze, die für alle Kreaturen gelten, die ausdrücklichen **Nicht-Ziele** und den Rahmen für den Umfang. Es erfindet nichts: Jede Zeile hat eine Quelle. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Leitsätze](#1-leitsätze)
2. [Nicht-Ziele](#2-nicht-ziele)
3. [Umfang: Tiefe vor Breite](#3-umfang-tiefe-vor-breite)
4. [Die Kreaturendefinition als Datenrahmen](#4-die-kreaturendefinition-als-datenrahmen)
5. [Ein Wesen aus Sicht des Spielers](#5-ein-wesen-aus-sicht-des-spielers)
6. [Abhängigkeiten](#6-abhängigkeiten)
7. [Was fehlt](#7-was-fehlt)

---

## 1. Leitsätze

| Nr. | Leitsatz | Quelle | Status |
|---|---|---|:---:|
| L1 | **Monster sind nicht ‚Gegner mit anderen Werten‘.** Jede Art hat eigene Regeln für Anatomie, Wahrnehmung, Intelligenz, Verhalten, Fortbewegung, Jagdmethode, Schwäche, Immunität, Regeneration, Tötungsmethode, Spuren, Reaktion auf Menschen und Jäger, Tag-/Umweltabhängigkeit, soziale Struktur und übernatürliche Regeln. Die erste Herausforderung ist, **herauszufinden, womit man es zu tun hat.** | `23#293A` Punkte 1–2; Zustimmung „1. ja das ist wichtig 2. passt“ in `23#294E` | 📋 |
| L2 | **Keine Stufen und Level.** Gefahr entsteht aus Monsterkenntnis, Fähigkeiten, Umgebung, Vorbereitung, Anzahl, aktuellen Verletzungen, Ausrüstung und Intelligenz. Kein sichtbares ‚Monster Level‘. | `M-PD` ‚Genre and progression identity‘; `23#293A` Punkt 16; Genre-Linie bestätigt „ok das passt gut“ in `23#296E` | 📋 |
| L3 | **Die Welt ist normal; das Übernatürliche ist verborgen, selten, widersprüchlich dokumentiert und regional unterschiedlich.** Die Welt soll nicht wie ein Freizeitpark voller Monster wirken (Zahl der Fälle als seltene Ereignisse). | `23#359A` Punkt 2, bestätigt „2. ja sehr gut“ in `23#360E`; `23#293A` Punkt 19, „ja das ist auch wichtig“ in `23#294E` | 📋 |
| L4 | **Kein universelles Magiesystem und kein Mana.** Verschiedene Phänomene (körperliche Wesen, Geister, Flüche, Besessenheit, dämonische Wesen, rituell erzeugte Phänomene, dimensionale Wesen, Artefakte, Anomalien) haben jeweils **eigene Regeln**. Rituale haben Zutaten, Vorbereitung, Zeit, Reihenfolge und Risiko, aber keinen Magiebalken. | `23#359A` Punkt 1, „1. gut“ in `23#360E`; `23#1074A` Punkte 99–106, „99-105 ja“, „106 ja hunter ist hunter“ in `23#1075E` | 📋 |
| L5 | **Der Spieler bleibt ein Mensch.** Auch nach 100 Stunden bleibt der Hunter ein gut ausgebildeter, erfahrener Mensch (Wissen, Erfahrung, Ausrüstung, Kontakte); Verletzungsregeln gelten weiter. Vorübergehende übernatürliche Zustände (Artefakt, Ritual, Fluch, Besessenheit, Vertrag, Story) sind möglich und haben Konsequenzen. | `23#359A` Punkte 28–29; Antwort „28. ja er bleibt ein mensch aber es kann sein dass man in der story oder in missionen besessen von einem dämon wird…“ in `23#360E`; „B ja passt, aber er kann auch teilweise kurz bessesen sein oder anderes“ in `23#360E` | 📋 |
| L6 | **Regeln müssen beobachtbar sein.** Gilt eine Regel (‚nur bei Mondlicht verwundbar‘), muss es Hinweise geben: historische Quellen, frühere Fälle, Verhalten, Experimente, Beobachtung. Nicht: dreißig Tode, bis zufällig die richtige Kombination gefunden ist. | `23#359A` Punkt 21, „21. perfekt“ in `23#360E` | 📋 |
| L7 | **Nicht alle Regeln sind sicher bekannt.** Hinweise können widersprüchlich sein; der Spieler bildet aus Quelle A, Quelle B und eigener Beobachtung eine Hypothese. | `23#359A` Punkt 22; `M-PD` ‚Hunter Archive epistemology‘ | 📋 |
| L8 | **Nicht jede Begegnung ist ein Kampf.** Beobachten, Verfolgen, Belauschen, Fotografieren, Markieren, Proben sammeln und bewusstes Nicht-Angreifen sind gültige Handlungen. | `23#293A` Punkt 17, „17. ja das ist sogar wichtig“ in `23#294E` | 📋 |
| L9 | **Nicht jedes Wesen ist böse.** Es gibt Wesen, die Menschen meiden, nur aus Selbstverteidigung töten, Opfer eines Fluchs sind oder verhandelbar sind; Fälle enden entsprechend mit Töten, Vertreiben, Heilen, Fluch brechen, Verhandeln, Ursache beseitigen oder Person retten. | `23#293A` Punkt 15, „15. genau, das passt gut“ in `23#294E`; `23#1074A` Punkte 49–52, „45-54 passt“ in `23#1075E` | 📋 |
| L10 | **Monster respawnen nicht.** Stirbt ein Individuum, ist es weg. Gleiche Monster kommen nie wieder, außer sie wurden aus dem Fegefeuer befreit („das ist sehr unwahrscheinlich“). | „30. wenn das jeweilige monster tot ist dann ist es weg“; „37. die gleichen monster werden nie wieder da sein, außer sie wurden aus dem fegefeuer befreit“ in `23#1075E`; `M-B09` ‚individual death permanence‘ | 📋 |
| L11 | **Wichtige Arten sind handgeschrieben.** Prozedural variieren dürfen Alter, Temperament, kleine körperliche Variationen, Narben, Erfahrung, Verhaltenstendenzen und Größe in plausiblen Grenzen, **nicht** Schwäche, Kopf oder Fähigkeit nach Zufall. | `23#307A` Punkt 28, „28. ok“ in `23#308E`; `M-PD` | 📋 |
| L12 | **Das Archiv zeigt Wissen, keine Daten.** Der Spieler sieht, was der Charakter weiß; AI-Gewichte, Wahrnehmungsschwellen, Regenerationsformeln, Spawn-Regeln, Hidden States sind intern. | `23#307A` Punkte 24, 27; `23#295A` (Archivfrage 22); `M-PD` ‚Hunter Archive epistemology‘ | 📋 |
| L13 | **Kern des Blocks:** Monster sollen sich wie **Wesen dieser Welt** anfühlen und nicht wie Gegnergruppen, die auf der Karte auf den Spieler warten. | `23#1074A` Punkt 156, „156 genau“ in `23#1075E` | 📋 |

| L14 | **Lebendige Welt, Gedächtnis und Nemesis sind verbindliche Langzeitvision** (kein M1-Feature, Umsetzung offen): persistente Identitäten, Langzeitgedächtnis, überlebende Gegner kehren zurück und lernen (auch intelligente übernatürliche Kreaturen), autonome Fraktionen, persistente Folgen, skalierbare Simulation, Integration ohne Widersprüche | `PE-N1022` (Anforderungen A–H, `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` §1) | 📋 |

**Hinweis zu L1/L2:** Der Entwickler schwankte zunächst („ich überlege ob ich ein rpg oder adventure will“, `23#294E` Punkt 16). Der Assistent schlug daraufhin ‚Open-World-Action-Adventure als Fundament mit RPG-, Investigations-, Immersive-Sim- und Survival-Horror-Säulen‘ vor (`23#295A`), und der Entwickler bestätigte „ok das passt gut“ (`23#296E`). Der Master führt das als Genre-Festlegung (`M-PD` ‚Genre and progression identity‘).

---

## 2. Nicht-Ziele

Ausdrücklich ausgeschlossen oder vermieden (jeweils mit Quelle):

| Nicht-Ziel | Quelle | Status |
|---|---|:---:|
| Sichtbare Monsterlevel, farbcodierte Seltenheit, Level-Gating, automatische Gegnerskalierung, große abstrakte Schadenszahlen | `M-PD` ‚Genre and progression identity‘ | 📋 |
| ‚Elite = +300 % HP‘; ‚Alter = HP × 5‘ | `23#293A` Punkt 11; `23#295A`; Zustimmung „D ja natürlich sonst wären alle monster gleich“ (bezogen auf Individualität, `23#294E`) | 📋 |
| Kugelschwamm-Bosse; Boss als ‚normales Monster × 10 HP‘ | `23#293A` Punkt 21, „21. ja aber der wird auch nicht in einer mission gekillt…“ in `23#294E` | 📋 |
| Mana- oder Soul-Ressource im Sinne von Zahlenwerten | `23#359A` Punkt 1; `23#465A` Punkt 14 (Monsterseelen sind kein Mana), „14 passt“ in `23#466E` | 📋 |
| Quick-Time-Rituale als Standard | `23#359A` Punkt 18, „18. ja passt“ in `23#360E` | 📋 |
| Zufallsgenerator würfelt Schwäche, Kopf und Fähigkeit | `23#307A` Punkt 28 | 📋 |
| Dauerhafte Superkräfte im Skilltree | `23#359A` Punkt 29, „29. gut“ in `23#360E` | 📋 |
| Wiederbelebung als Verbrauchsgegenstand: Tote zurückholen ist außergewöhnlich (Story, seltene Rituale, mächtige Wesen, Artefakte, Verträge) | `23#458A` Punkt 15, „15. genau“ in `23#459E`; Auslegung `23#361A` Punkt 6 und `35#27A` (💡) | 📋 / 💡 |
| Eine „universelle Gefängniszelle für alle Monster“ | `48#130A` (Option D gewählt in `48#129E`) | 📋 |
| Universelle humanoide KI für alle Wesen | `23#1074A` Punkt 55, „55-60 ja“ in `23#1075E` | 📋 |
| Wesen, die dauernd „alle 200 Meter“ herumlaufen | `23#293A` Punkt 19 | 📋 |
| Das Übernatürliche durch ‚Magie löscht alle Videos‘ verbergen | `23#359A` Punkt 31, „31. ja, oder weil die leute das nicht glauben wollen…“ in `23#360E` | 📋 |
| Serienspezifische Namen, Wesen, Artefakte und Storybögen 1:1 | `23#377E`, `23#379E`, [Modul 01 §4](01-Quellenlage-und-Methodik.md#4-zwei-regeln-des-entwicklers-zum-umgang-mit-der-serie) | 📋 |

---

## 3. Umfang: Tiefe vor Breite

| Aussage | Quelle | Status |
|---|---|:---:|
| Lieber **20–30 wirklich tief unterschiedliche Kernarten** im Basisspiel als 100 oberflächliche; Antwort „151 ok“ | `23#1074A` Punkt 151; `23#1075E` | 🧭 |
| Der Master führt die Anzahl der Basisarten als **offen** (‚Final base-game monster roster/count‘) | `M-CA0905` §7; `M-B09` ‚Open‘; `M-OD` | ❓ |
| Weitere Monster, Organisationen, Ebenen und Lore sollen große DLCs erweitern; „154 genau“ | `23#1074A` Punkt 154; `23#1075E` | 📋 |
| Jede Art unterscheidet sich mindestens in Ökologie, Verhalten, Spuren, Schwäche, Jagdmethode, Kampf-KI, Lore und Auswirkung auf die NPC-Welt; „155 korrekt“ | `23#1074A` Punkt 155; `23#1075E` | 📋 |
| Variationen innerhalb einer Art (Alter, Stärke, Verhalten, teilweise Fähigkeiten): „152 sehr wichtig“ | `23#1074A` Punkt 152; `23#1075E` | 📋 |
| Seltene Einzelwesen mit eigener Geschichte; es gibt „vielleucht auch alphas, also die ersten ihrer art“ | `23#1074A` Punkt 153; `23#1075E` | 📋 / ❓ (Name) |
| Wildtiere: ca. 220–280 Tier-, Vogel- und Fischarten auf der Hauptkarte (getrennt von Monstern) | `M-PD` ‚Wildlife / study / harvesting / monsters‘; [Modul 18](18-Tiere-und-Abgrenzung.md) | 📋 |

**Ausdrücklich nicht beschlossen:** eine feste Artenliste, die Zahl der Arten im Basisspiel, die Reihenfolge der Ausarbeitung. Modul 19 führt deshalb eine **Kandidatenliste ohne Zusage**.

---

## 4. Die Kreaturendefinition als Datenrahmen

Der Assistent listete in `23#307A` Punkt 27 die Bestandteile einer internen ‚Creature Definition‘; der Entwickler antwortete „27. perfekt“ (`23#308E`). Die **Gliederung** ist damit Richtung (📋); die Feldnamen sind Arbeitsbegriffe (💡).

| Bestandteil | Inhalt | Modul |
|---|---|---|
| Species Identity | Art, Namen, `ip_provenance` | 03 |
| Physical Form, Origin | Existenzform und Ursprung getrennt | 03 |
| Anatomy | modulare Körperregionen und Strukturen | 04 |
| Senses | Sinne und Wahrnehmung | 10 |
| Intelligence, Memory | mehrdimensionale Intelligenz, Gedächtnis | 03, 10 |
| Motivations | Antriebe statt Aggro-Wert | 03 |
| AI Behaviors, Tactical Capabilities | Verhalten, Taktikvorrat | 10 |
| Abilities | Fähigkeiten, getrennt von physischem Schaden | 04 |
| Weakness Rules, Resistance Rules | Schwächen und Widerstände mit Wirkungsart | 04 |
| Regeneration | zerlegt in Teilregeln | 04 |
| Social Rules | soziale Struktur | 08, 09 |
| Activity Conditions | Tag, Zeit, Jahreszeit, Wetter, Temperatur, Mond, Ort, Sonderzustand | 08 |
| Lifecycle | Lebensphasen und Entstehung | 08 |
| Spawn/Event Conditions | Bedingungen für Vorfälle (kein Spawnpunkt) | 08, 15 |
| World-State Effects | Auswirkungen auf Welt, NPCs, Archiv | 15, 17 |
| Archive Knowledge Sources | woher Wissen kommt | 14 |
| Modding Hooks | Erweiterbarkeit | 17 |
| Test Contracts | prüfbare Zusagen | 23 |

Dazu die Modding-Linie: Ein Modder soll neue Kreaturen hinzufügen können (Definition, Verhalten, Anatomie, Fähigkeiten, Schwächen, Archiv-Einträge, Assets, Animationen, Sounds, Fall-Integration), **ohne bestehende überschreiben zu müssen**; native C++ darf für hochkomplexe Kreaturen möglich sein (`23#293A` Punkt 23, „23. passt“ in `23#294E`). Details in [Modul 17](17-Persistenz-Daten-und-Modding.md).

---

## 5. Ein Wesen aus Sicht des Spielers

Aus den Quellen ergibt sich eine einheitliche Reihenfolge, in der der Spieler eine Kreatur kennenlernt. Sie ist **eine Zusammenstellung** (💡), jede Stufe hat einen Quellenhinweis.

| Stufe | Was der Spieler weiß | Quelle der Stufe |
|---|---|---|
| 0 | Ein Vorfall (Weltzustand erfüllt Bedingungen, Hinweise in Zeitung, Polizeifunk, Gerüchten, Tierverhalten) | `23#307A` Punkt 21, „21. ja genau, aber nicht von einer sekunde auf die andere…“ in `23#308E`; `M-PD` ‚Organic world-event / case emergence‘ |
| 1 | ‚Unbekannte Kreatur‘ oder gar nicht, ob etwas Übernatürliches vorliegt | `23#293A` Punkt 10; `23#1074A` Punkte 71–72, „71 ja genau“, „72 ja genau“ in `23#1075E` |
| 2 | ‚wahrscheinlich Kategorie X‘ (Verdacht, Hypothese) | `23#293A` Punkt 10 |
| 3 | ‚identifiziert: Art Y‘, aber einzelne Felder bleiben unbekannt (zum Beispiel Schwäche bekannt, Regeneration unbekannt) | `23#293A` Punkt 10 |
| 4 | Wissen über **dieses** Individuum (anders als die Art), Narben, Gewohnheiten | `23#307A` Punkt 30; `M-PD` |
| 5 | Bestätigt, vermutet, bestritten, widerlegt, historisch/unbelegt je Behauptung | `M-PD` ‚Hunter Archive epistemology‘ |

Die Anzeigen (‚Unknown‘, ‚Confirmed‘) sind Statuswörter der Quellen. Das Statusmodell des Archivs steht in [Ermittlung 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md); dieses Modul hält nur fest, dass für Kreaturen **dieselbe** Logik gilt.

---

## 6. Abhängigkeiten

| Abhängigkeit | Wirkung auf Kreaturen |
|---|---|
| Persistenter Weltzustand | Individuen, Populationen, Nester, Narben, Erinnerungen müssen als Zustand überleben (Modul 17) |
| Verletzungs- und Medizinsystem | Anatomie und Regeneration bauen auf dem Verletzungsmodell der Menschen auf (`23#293A` Punkt 2) |
| Stealth-/Horror-KI | Kreaturen-KI setzt auf Wahrnehmung, Wissensmodell und Suche (`M-PD` ‚Stealth…‘) |
| Fallgenerator | Ökologie erzeugt Vorfälle (Modul 15; [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md)) |
| Dialog und NPC-Gedächtnis | sprechende Wesen, Verhör, Verhandlung (Modul 13) |
| Gesellschaft, Polizei, Medien | Reaktion der Menschen (Modul 15; [Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md)) |
| Reiche | Fegefeuer als Monster-Reich; Hölle und Himmel als Heimat intelligenter Wesen ([Modul 07](07-Kosmologie-Seelen-und-Reiche.md)) |
| Asset-Produktion | Wahre Formen und Anatomien brauchen Concept, Anatomiebögen, Blender-Prototyp und Abnahme durch den Entwickler (`23#467A` Punkt 8; `23#465A` Punkt 23) |

---

## 7. Was fehlt

- Eine **Artenliste** und eine Reihenfolge der Ausarbeitung (❓, Q-K01).
- Eigennamen für Organisation, Ursprungswesen und Kategorien (❓, `M-B09`).
- Schwellen für Hypothese und Bestätigung (❓, `M-OD`).
- Zahlenwerte aller Art (Regenerationszeiten, Erkennungsreichweiten): bewusst nicht Teil dieses Ordners.
