# Kreaturen 13: Lebendfang, Fallen und Containment

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Kreaturen gefangen, verwahrt, verhört und beforscht werden und wie der Spieler Fallen und Schutzvorrichtungen in der Welt aufstellt. Die Grundentscheidungen stammen aus dem **Quiz vom 10.–11.10.2026** (Chat 48) und stehen **nicht im Master**. Wirksamkeit gegen konkrete Wesen ist nicht festgelegt. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Gefangennahme als Prozess](#2-gefangennahme-als-prozess)
3. [Fallen und Schutzvorrichtungen in der Welt](#3-fallen-und-schutzvorrichtungen-in-der-welt)
4. [Verwahrung und Einrichtungen](#4-verwahrung-und-einrichtungen)
5. [Verhör, Gespräch, Verhandlung](#5-verhör-gespräch-verhandlung)
6. [Fluchtversuche, Sabotage, Risiken](#6-fluchtversuche-sabotage-risiken)
7. [Konsequenzen](#7-konsequenzen)
8. [Entwurf: Containment-Modell](#8-entwurf-containment-modell)
9. [Entwurf: Prüfregeln](#9-entwurf-prüfregeln)
10. [Sonderfälle](#10-sonderfälle)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Kreaturen können lebend gefangen und befragt werden | „97-98“ im Bereich „94-98 ja“ in `23#1075E` (Block 9K) | 📋 |
| Gefangennahme ist ein **umfassendes** System mit maximaler Freiheit: Fallen, Fesseln, Betäubung, Rituale, Transport, sichere Verwahrung, selbst gebaute Sicherheitsmaßnahmen, Forschung, Verhandlungen, Konsequenzen je nach Kreatur | „Supernatural Monstergefangennahme: Maximale Freiheit bei Gefangennahme, Sicherheitsmaßnahmen, Forschung und Konsequenzen“ in `48#129E` (Option D) | 📋 |
| **Keine universelle Gefängniszelle** für alle Monster; die Wirksamkeit hängt von der Mythologie ab | `48#130A` (Auslegung der gewählten Option) | 📋 / 💡 |
| Man braucht keine besondere Einrichtung: an **beliebigen Orten** kann man zum Beispiel eine Teufelsfalle bauen und so weiter | „man kann an beliebigen orten z.b eine teufelsfalle bauen und so weiter“ in `48#131E` | 📋 |
| Der Hunter-Bunker bleibt ein **eigenes Großthema** mit eigener Planung | „der bunker muss noch einzeln geplant werden weil er so super sein soll“ in `48#131E`; `48#132A` | 📋 |
| Fallen bleiben **persistent** und werden durch Wetter, Abnutzung, NPCs, Monster, Spieler und Zeit verändert; der Zustand entwickelt sich auch weiter, wenn der Spieler das Gebiet verlassen hat | „Supernatural Fallen-Persistenz: Realistische Persistenz mit Umwelt- und NPC-Einflüssen“ in `48#133E`; Tabelle `48#134A` | 📋 |
| Fallenbau: **vorgefertigte** Vorlagen mit verschiedenen Größen, Material, Ausrichtung und Position; freihändiges Zeichnen ist keine Pflicht | „theoretisch beliebig aber das wird halt sehr schwer zum zeichnen also würde ich eher das vorgefertigt haben mit verschiedenen größrn und so“ in `48#135E`; `48#136A` | 📋 |
| Monster reagieren **taktisch** auf Fallen | „Supernatural Monster-KI bei Fallen: Taktische Monster mit Sabotage und Gegenstrategien“ in `48#137E` | 📋 |
| Ein in einer wirksamen Falle gefangener Dämon befreit sich nicht durch KI | `48#138A` | 📋 / 💡 |
| Koop: Mehrere Hunter können gemeinsam einen Ort vorbereiten und sichern | `48#132A` (Auslegung) | 💡 |
| Gefängnissicherheit passt sich dynamisch an übernatürliche Bedrohungen an | „Supernatural übernatürliche Gefängnissicherheit: Dynamische Anpassung von Gefängnissicherheit an übernatürliche Bedrohungen“ in `48#169E`; [Ermittlung 16](../Ermittlung/16-Haft-und-Strafvollzug.md) | 📋 |
| Staatliche Stellen forschen geheim an Wesen (Untergrundlabore) | `48#171E` | 📋 |

---

## 2. Gefangennahme als Prozess

Die Quiz-Frage `48#128A` (Option D): ‚Maximale Freiheit: umfangreiche Gefangennahme, selbst gebaute Sicherheitsmaßnahmen, Forschung, Verhandlungen und unterschiedliche Konsequenzen je nach Kreatur.‘ Der Assistent gliedert (`48#130A`, 💡 Auslegung):

| Bereich | Möglichkeiten |
|---|---|
| Gefangennahme | Fallen, Fesseln, Betäubung, Rituale, monsterspezifische Methoden |
| Transport | gefangene Kreaturen sicher zu geeigneten Einrichtungen bringen |
| Verwahrung | Zellen, Schutzsymbole, Fallen, Barrieren, individuell angepasste Sicherheitsmaßnahmen |
| Forschung | Untersuchungen, Probenentnahmen, Experimente |
| Kommunikation | Verhöre, Gespräche, Verhandlungen, mögliche Kooperation |
| Sicherheitsrisiken | Fluchtversuche, Sabotage, Angriffe, fehlerhafte Schutzmaßnahmen |
| Konsequenzen | Story, NPC-Beziehungen, Moral, Spielwelt |

Das ist ein **Zielbild**, nicht ein Meilenstein. Die Anforderungen an Persistenz (Gefangener als Individuum im Weltzustand) und an Szenarien (Transport, Ausbruch) sind groß; die Reihenfolge der Ausarbeitung ist nicht entschieden (❓ Q-K18).

---

## 3. Fallen und Schutzvorrichtungen in der Welt

### 3.1 Freie Platzierung

`48#132A` (💡 Auslegung der 📋 Wahl): Teufelsfalle (und ähnliche) an beliebigen geeigneten Orten: verlassene Häuser, Scheunen, Keller, Waldhütten, Lagerhallen und andere Gebäude. Vorbereitung vor der Jagd. Wirkung hängt von Kreatur, Mythologie und korrekter Ausführung ab. Fallen können unter Umständen beschädigt, unterbrochen oder unwirksam gemacht werden. Bei Koop bereiten Hunter gemeinsam vor.

### 3.2 Bau und Vorlagen

- Vorgefertigte Vorlagen: Teufelsfallen, Schutzsymbole, Bannkreise, weitere mythologische Konstruktionen (`48#136A`).
- Verschiedene Größen, bis zu Fallen, die ganze Räume abdecken.
- Position: auf geeigneten Böden, Wänden, Decken und anderen Oberflächen.
- Materialwahl: Kreide, Farbe, Gravuren oder andere mythologisch geeignete Materialien.
- Drehen und Anpassen vor der Erstellung.
- Der Hunter braucht gegebenenfalls Materialien und Zeit.
- Freihändiges Zeichnen: keine Pflichtmechanik; langfristig optional, falls es einen Mehrwert bringt.

Alles davon ist Auslegung (💡) der Wahl ‚vorgefertigt, flexibel anpassbar‘ (`48#135E`).

### 3.3 Persistenz (`48#134A`, 💡 Auslegung)

| Einfluss | Wirkung |
|---|---|
| Regen und Schnee | Zeichnungen können beschädigt oder ausgewaschen werden |
| Wind und Abnutzung | lose Materialien und Konstruktionen verändern sich |
| NPCs | können Fallen entdecken, entfernen, versehentlich beschädigen, manipulieren |
| Monster | können Schutzmaßnahmen sabotieren |
| Spieler | repariert, verstärkt, verändert, baut ab |
| Zeit | der Zustand entwickelt sich weiter, auch wenn der Spieler das Gebiet verlassen hat |

Beispiel der Quelle: Eine Teufelsfalle in einer Scheune ist drei Ingame-Wochen später vielleicht intakt; hat jemand den Boden gereinigt oder die Zeichnung beschädigt, funktioniert sie nicht mehr. Technisch: Fallen brauchen persistent gespeicherten Zustand mit nachvollziehbaren Veränderungen, **ohne** dass entfernte Gebiete permanent voll simuliert werden müssen (`48#134A`).

### 3.4 Reaktion von Wesen

Siehe [Modul 10](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §7. Besonders: Ein Wesen darf eine Falle durchschauen und umgehen, sabotieren oder sich scheinbar ahnungslos geben; ein Wesen, das in einer **wirksamen** Falle bereits gefangen ist, entkommt nicht durch KI allein (`48#138A`).

---

## 4. Verwahrung und Einrichtungen

### 4.1 Aussage des Entwicklers

Die Frage `48#130A` fragte, ob alle Gefangenen im Bunker bleiben (A), mehrere feste Einrichtungen (B), ausbaubare Einrichtungen (C) oder ein freies Netzwerk (D). Der Entwickler antwortete **nicht mit einer Option**, sondern mit einem Freitext: „man kann an beliebigen orten z.b eine teufelsfalle bauen und so weiter, der bunker muss noch einzeln geplant werden weil er so super sein soll“ (`48#131E`). Daraus folgt: **keine Bindung an eine bestimmte Einrichtung**, der Bunker bleibt ein getrenntes Planungsfeld.

### 4.2 Folgerungen (💡)

- Verwahrung an beliebigen Orten ist möglich (Falle am Ort).
- Eine dauerhafte, ausgebaute Verwahrung (Zellen, Labore) hängt an späterer Planung von Bunker und Einrichtungen (`48#132A`).
- Staatliche Stellen betreiben eigene Verwahrung und Forschung (`48#171E`; [Ermittlung 11](../Ermittlung/11-Forensik-und-Labor.md), [Ermittlung 16](../Ermittlung/16-Haft-und-Strafvollzug.md)).
- Ob der Spieler eigene dauerhafte Einrichtungen bauen, kaufen und vernetzen kann (Option D der Frage), ist **nicht** entschieden (❓ Q-K19).

### 4.3 Rechtlicher Rahmen

Das Festhalten von Wesen berührt das Recht (Freiheitsberaubung, Entführung, Hausfriedensbruch, Verwahrung von Menschen mit Besessenheit). Die Quellen enthalten dazu nichts Spezielles; das Rechtssystem der Ermittlungs-Dokumentation gilt ([Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md), [Ermittlung 15](../Ermittlung/15-Gerichte-und-Verfahren.md)). Die Frage, wie das Spiel den Fall behandelt, dass jemand einen **besessenen Menschen** festhält, ist offen (❓ Q-K20).

---

## 5. Verhör, Gespräch, Verhandlung

| Aussage | Quelle | Status |
|---|---|:---:|
| Intelligente Wesen können befragt werden | „94-98 ja“ in `23#1075E` | 📋 |
| Wesen können lügen und manipulieren; Aussagen über die eigene Art sind nicht automatisch wahr | `23#1074A` Punkt 53; „45-54 passt“ in `23#1075E` | 📋 |
| Ein Gefangener kann kommunizieren, verhandeln, mit ihm kooperieren | `48#130A` (Auslegung) | 💡 |
| Dämonen können Verträge anbieten und Menschen benutzen | `23#359A` Punkt 14; [Modul 06](06-Daemonen-Engel-und-Leviathane.md) | 📋 |

Das Verhör folgt den Regeln der Ermittlungs-Dokumentation ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §Verhör eines Wesens, T-Z09: ein verhörtes Wesen gibt nur Wissen preis, das es erworben hat). Dialog-Architektur: `M-PD` ‚Dialogue / conversation architecture‘; die Erweiterung durch freie KI-Gespräche mit Wesen ist 🔭 (`48#151E`/`48#152A`).

---

## 6. Fluchtversuche, Sabotage, Risiken

`48#130A` (💡): Fluchtversuche, Sabotage, Angriffe und fehlerhafte Schutzmaßnahmen sind Risiken. Daraus folgt (💡):

- Eine Verwahrung hat einen **Zustand** (intakt, beschädigt, durchbrochen).
- Ein Wesen kann Sabotage vorbereiten (zum Beispiel Gefängnispersonal beeinflussen, Symbole beschädigen), soweit seine Fähigkeiten es erlauben.
- Eine Flucht ist ein Ereignis mit Folgen für Welt, Fall und Beziehungen; der Spieler wird gegebenenfalls gejagt (`23#1074A` Punkte 57–60).
- Der Ablauf einer Flucht verletzt nie die Mythologie (zum Beispiel ein wirksam gefangener Dämon bleibt gefangen, solange die Falle intakt ist, `48#138A`).

---

## 7. Konsequenzen

`48#130A` nennt Auswirkungen auf Story, NPC-Beziehungen, Moral und Spielwelt. Konkret (💡 Ableitungen):

| Bereich | Mögliche Folge |
|---|---|
| Story | Gefangene Wesen können Informationen liefern oder die Story verändern |
| NPC-Beziehungen | Hunter-Verbündete billigen oder verurteilen das Festhalten |
| Moral | Das Festhalten eines Wesens, das niemandem geschadet hat, kann für andere Hunter oder die Beteiligten ein Problem sein (L9) |
| Welt | Ein gefangenes Individuum fehlt in Ökologie und Gruppen (Machtkampf, Nachfolger, Rache; [Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md)) |
| Recht | siehe 4.3 |

---

## 8. Entwurf: Containment-Modell

> 💡 **Vorschlag.** Er macht das Zielbild als Zustand prüfbar.

```text
ContainmentSite
├── site_id, location, owner (player | hunter_org | state | none)
├── wards[]           {ward_id, template, size, material, placed_at, integrity, exposed_to_weather}
├── barriers[]        physische Linien/Räume; Zustand intakt/beschädigt
├── occupant_id | none
├── security_state    secure | degraded | breached
├── personnel[]       NPC-Wachen (optional)
└── event_log[]       Prüf-/Sabotage-/Fluchtereignisse

Ward-Wirkung = f(species_rule, ward.integrity, execution_quality, environment)
```

**Quellenbezug:** Vorlagen, Materialien, Größe, Position (`48#136A`); Verfall durch Wetter/NPCs/Monster/Zeit (`48#134A`); Wirkung abhängig von Kreatur und korrekter Ausführung (`48#132A`).

---

## 9. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Eine Falle wirkt nur bei Arten, deren Regel sie nennt | `48#130A`: ‚Eine universelle Zelle gibt es nicht‘; `23#293A` Punkt 3 |
| Eine beschädigte Falle ist unwirksam | `48#134A` |
| Regen verschlechtert eine ungeschützte Zeichnung im Freien; im Gebäude nicht | `48#134A` |
| Ein NPC kann eine Falle beschädigen, ohne dass der Spieler es erfährt | `48#134A` |
| Der Zustand einer Falle schreitet voran, auch wenn der Bereich nicht geladen ist | `48#134A` |
| Ein Gefangener wird als Individuum im Spielstand gehalten und erinnert sich | `48#139E`; `23#307A` Punkt 15 |
| Ein gefangenes Individuum fehlt in der Populationssimulation seiner Gruppe | `23#1076A` (Individuen) |
| Eine wirksame Falle hält auch bei intelligenter Taktik | `48#138A` |

---

## 10. Sonderfälle

| Fall | Behandlung |
|---|---|
| Besessener Mensch in Verwahrung | Wirt retten; rechtlicher Rahmen offen (❓ Q-K20) |
| Wesen ohne Körper (Geist) | Bindung ist die Lösung, nicht Verwahrung ([Modul 05](05-Geister-Besessenheit-und-Fluecher.md)) |
| Ein anderer Hunter befreit oder tötet den Gefangenen | unabhängige Handlung ([Modul 11](11-Hunterwelt-Organisationen-und-Kulte.md)) |
| Eine Falle wird von einem Wesen zu seinen Gunsten genutzt | möglich, wenn die Art es kann |
| Ein Gefangener ist ein Ausnahmeindividuum | Standardwirkung kann teilweise versagen ([Modul 03](03-Klassifikation-Herkunft-und-Individuen.md) §7) |

---

## 11. Was fehlt

- Wirksamkeit je Art und Falle (❓, Artendefinition).
- Der Bunker als Planungsfeld, dauerhafte Einrichtungen (❓ Q-K19).
- Rechtlicher Rahmen für Verwahrung (❓ Q-K20).
- Reihenfolge der Ausarbeitung des Zielbilds (❓ Q-K18).
- Persistenz-Kosten von Fallen in großer Zahl (Architekturfrage, [Modul 17](17-Persistenz-Daten-und-Modding.md)).
