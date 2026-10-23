# Kreaturen 15: Vorfälle und die lebendige Welt

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul verbindet die Kreaturen-Ökologie mit der Fallgenerierung und den Folgen für die Welt: wie aus Monsteraktivität Vorfälle werden, was geschieht, wenn der Spieler sie ignoriert, und wie Menschen reagieren. Der **Falllebenszyklus** selbst steht in [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md), Notfälle und Weltereignisse in [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md); hier steht nur die **Kreaturenseite**. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Hauptquelle ist Block 9G–H (`23#1074A` Punkte 62–75, Antworten `23#1075E`), `23#307A` Punkte 20–22 und `M-PD` ‚Organic world-event / case emergence‘.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Von der Ökologie zum Vorfall](#2-von-der-ökologie-zum-vorfall)
3. [Vorlauf statt Spawn](#3-vorlauf-statt-spawn)
4. [Ignorierte Vorfälle](#4-ignorierte-vorfälle)
5. [Falltypen](#5-falltypen)
6. [Folgen für die Welt](#6-folgen-für-die-welt)
7. [Entwurf: Vorfallmodell](#7-entwurf-vorfallmodell)
8. [Entwurf: Prüfregeln](#8-entwurf-prüfregeln)
9. [Beispiele (Entwurf)](#9-beispiele-entwurf)
10. [Sonderfälle](#10-sonderfälle)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Nicht jeder Monsterfall wird manuell gespawnt; die Monsterökologie kann selbst Vorfälle erzeugen (Population → Opfer → Vermisstenmeldung → Polizei → Zeitung/Social Media → Hunter-Hinweis) | `23#1074A` Punkt 62; „62-68 ja…“ in `23#1075E` | 📋 |
| Fälle entstehen, ohne dass der Spieler sie annimmt; wenn man sie ignoriert, passiert die Welt weiter | Punkt 63; „62-68 ja“ | 📋 |
| Die Opferzahl kann steigen, wenn ein gefährliches Wesen nicht gestoppt wird | Punkt 64 | 📋 |
| Ein Monster kann weiterziehen; die Spur wandert | Punkt 65 | 📋 |
| Andere Hunter können eingreifen; ignorierte Fälle werden manchmal von NPC-Huntern gelöst | Punkt 66 | 📋 |
| Ein Fall kann sich selbst „lösen“ (Monster stirbt, zieht weg, Täter wird entdeckt) | Punkt 67 | 📋 |
| Manche Fälle werden schlimmer (Nest wächst, Fluch verbreitet sich, Kult gewinnt Mitglieder) | Punkt 68 | 📋 |
| Man kann auch im HQ oder im Hunter-Forum sagen, dass man etwas gesehen hat, aber keine Zeit hat | „62-68 ja, man kann aber auch im hq sagen, oder im internet forum…“ in `23#1075E` | 📋 |
| Nebenmissionen sind „sehr wichtig“ | „69 nebenmissionen sind sehr wichtig“ in `23#1075E` | 📋 |
| Kleinere dynamische Hunter-Aufträge aus der Weltsimulation, „sehr viele“, nicht gescripted | „70 ja davon soll es sehr viele geben, und immer wieder da die nicht gescripted sind sondern aus der welt passieren“ in `23#1075E` | 📋 |
| Mehrdeutige Fälle (man weiß nicht, ob überhaupt Übernatürliches vorliegt) | Punkt 71; „71 ja genau, aber nur haupt und nebenmissionen ist das aber nicht so“ in `23#1075E` | 📋 (Einschränkung beachten) |
| False Positives: manche Monsterfälle haben keine übernatürliche Ursache | „72ja genau“ in `23#1075E` | 📋 |
| Ein Fall kann gleichzeitig normale Kriminalität **und** ein übernatürliches Element haben | „73 genau“ in `23#1075E` | 📋 |
| Selten zwei unabhängige Wesen in einem Fall | „74 ja“ in `23#1075E` | 📋 |
| Ein Mensch kann der Täter sein und das Übernatürliche als Tarnung nutzen | „75 kann sehr gut sein“ in `23#1075E` | 📋 |
| Zeit-/Wetter-/Mond-/Jahreszeitbedingte Fälle und Kreaturenaktivität dürfen **nicht** wie MMO-Quest-Spawn auftreten; Vorlaufphasen (Zeitung, Vermisstenmeldungen, Polizeifunk, Gerüchte, Tierverhalten, ungewöhnliche Verletzungen, Umweltanomalien) | `M-PD` ‚Organic world-event / case emergence‘; „21. ja genau, aber nicht von einer sekunde auf die andere…“ in `23#308E` | 📋 |
| Ohne Karten-/Questhilfe brauchen neue Fälle keine automatischen Marker; Entdeckung über Information, Kontakte, Beobachtung, manuelle Kartierung | `M-PD`; „ich würde nicht mal wirklich questmarker auf der map machen, wenn man es einstellt“ in `23#308E` | 📋 |
| Ein ignoriertes Ereignis kann mehr Opfer, Polizeieinsatz, Eingreifen eines anderen Hunters, Abwandern der Kreatur, veränderte Beweise oder einen späteren historischen/ungelösten Fall zur Folge haben | `M-PD` | 📋 |
| Zeitbegrenzte Bedingungen löschen keinen Inhalt; manche Ereignisse kehren wieder, wirklich einmalige Folgen sind bewusst weltzustandsbewusst | `M-PD`; `23#307A` Punkt 22, „22. ja gut“ in `23#308E` | 📋 |
| Neben handgebauten Nebenfällen gibt es **Hauptmissionen** und handgebaute große Nebenfälle (Multi-Path) | `23#1074A` Punkt 69; `23#1076A` | 📋 |

---

## 2. Von der Ökologie zum Vorfall

### 2.1 Die Kette

```text
Monsterpopulation / Individuum / Gruppe (Modul 08)
 → Handlung (Jagd, Territorialkampf, Hunger, Rache, Fluchzwang ...)
 → Opfer / Spur / Anomalie
 → Vermisstenmeldung, Notruf, Fund
 → Polizei (normale Erklärung zuerst)
 → Zeitung, Forum, Social Media, Gerüchte
 → Hunter-Hinweis (HQ, Netzwerk, eigene Beobachtung)
 → Fall (angenommen oder nicht)
```

Die Kette ist die Quelle (`23#1074A` Punkt 62; `23#1076A`, ‚Dynamisch entstehende Hunter-Aufträge‘). Die Polizeiseite (Meldeketten, Ermittlungen, normale Erklärungen) steht in [Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md) und [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md).

### 2.2 Welche Handlung erzeugt welchen Vorfall

Die Zuordnung ist Artregel. Bedeutsame Verbindungen aus den Quellen:

| Handlung | Vorfall | Quelle |
|---|---|---|
| Jagd/Tötung | Vermisste, Leichen, Verletzungsmuster | `23#1074A` Punkt 62 |
| Territorialkonflikt zwischen Gruppen | Angriffe, Vermisste, veränderte Sicherheitslage | `48#144A` (💡) |
| Fluch wandert mit einem Objekt | neuer Fall am neuen Ort | `23#1074A` Punkt 109 |
| Haus mit übernatürlicher Geschichte | Preis-/Besitzerwechsel-Muster, später Vorfall | `23#1076A`; [Modul 16](16-Uebernatuerliche-Orte-und-Geografie.md) |
| Kult wächst | Rekrutierung, Verschwinden, Rituale | Punkt 124 |
| Wanderung (Hunter-Druck, Nahrung) | neuer Ort, neue Opfer | `23#1074A` Punkt 35, 65 |

---

## 3. Vorlauf statt Spawn

### 3.1 Aussagen

- **Weltzustand erfüllt Voraussetzungen**, nicht ‚Spawn‘: Opfer verschwinden, NPC-Berichte entstehen, Nachrichten erscheinen, Polizei reagiert, Gerüchte (`23#307A` Punkt 21, 💡 Formulierung). Der Fall fühlt sich an, als wäre er **in der Welt entstanden**.
- Der Übergang hat Stufen: „vielleicht schon in der zeitung vermehrte komische hinweise und erst später sieht man mehr“ (`23#308E` Punkt 21).
- Hinweise können schwach sein (Zeitung, Vermisstenmeldung, Polizeifunk, Gerüchte, Tiere, Verletzungen, Umweltanomalien) und sich verdichten, bis ein Muster erkennbar wird (`M-PD`).

### 3.2 Bedingungen

Aktivitätsbedingungen (Tag/Nacht, Zeitbereich, Jahreszeit, Wetter, Mond, Ort, Sonderzustand) sind in [Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §8 geführt. Beispiele des Assistenten (`23#295A`): nur nachts, im Winter, zwischen 23:00 und 04:00, nur bei Gewitter, bei bestimmter Mondphase, im Herbst (💡).

### 3.3 Zeitfenster und Wiederkehr

- Ein verpasstes Zeitfenster zerstört den Fall nicht (`M-PD`). Er kann beim nächsten Fenster wiederkommen oder mit anderer Lage bestehen (`23#307A` Punkt 22).
- Ein Beispiel (`23#307A` Punkt 22, 💡): Spieler ignoriert Hinweise → Opfer stirbt → Fall ist später noch untersuchbar, aber die Situation ist anders.

---

## 4. Ignorierte Vorfälle

| Mögliche Folge | Quelle |
|---|---|
| mehr Opfer | `23#1074A` Punkt 64; `M-PD` |
| Polizeieinsatz | `M-PD` |
| Eingreifen eines anderen Hunters | Punkt 66; `M-PD` |
| Abwandern der Kreatur | Punkt 65; `M-PD` |
| geänderte Beweise | `M-PD` |
| späterer historischer oder ungelöster Fall | `M-PD` |
| Fall löst sich von selbst | Punkt 67 |
| Fall wird schlimmer | Punkt 68 |

Der Spieler darf Inhalt nicht **unwissentlich dauerhaft verlieren**: Hauptmissionen sind nie dauerhaft verpassbar, Nebenfälle normalerweise nicht (außer aus echten Weltzustandsgründen; `M-PD` ‚Acts, chapters and controlled story reactivity‘; [Ermittlung 02](../Ermittlung/02-Falltypen-und-Fallmodell.md)). Ein Fall mit veränderter Welt wird zur Fallvariante (`23#582E`/`23#583A`, siehe [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md)).

---

## 5. Falltypen

Die Quellen unterscheiden (`23#1074A` Block 9H, `23#1076A` ‚Missionstypen‘):

| Typ | Beschreibung |
|---|---|
| Hauptmissionen | handgebaut, stark narrativ |
| Große handgebaute Nebenfälle | höchste Qualität, Multi-Path, Investigation („69 nebenmissionen sind sehr wichtig“) |
| Dynamische Hunter-Aufträge | aus der laufenden Welt, potenziell unbegrenzt („ja davon soll es sehr viele geben“) |
| Mehrdeutige Fälle | man weiß zuerst nicht, ob etwas Übernatürliches vorliegt (nur wo passend) |
| Fälle ohne Übernatürliches | False Positive („72ja genau“) |
| Gemischte Fälle | normale Kriminalität plus übernatürliches Element (73) |
| Mehrere Wesen | selten (74) |
| Menschlicher Täter mit Tarnung | (75) |

„71 ja genau, aber nur haupt und nebenmissionen ist das aber nicht so“: Der Entwickler schränkt ein, dass **die Mehrdeutigkeit nur für Haupt- und Nebenmissionen** gilt (die Auslegung dieser Formulierung ist unscharf und in der Ermittlungs-Dokumentation unter ‚unklare Antworten‘ zu vergleichen: ❓ Q-K22).

---

## 6. Folgen für die Welt

### 6.1 Menschliche Gesellschaft

Aus der Quiz-Entscheidung zur Gesellschaftsreaktion (`48#145E`) folgt, dass Vorfälle Wirkung auf Polizei, Politik, Medien, Geheimhaltung, Organisationen, Bevölkerung, Wirtschaft und Langzeitfolgen haben (siehe [Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md) §7). Beispiele der Quelle (`48#146A`, 💡): Vermisstenmeldungen werden zunächst normal behandelt; Eskalation führt zu Medienberichten und verängstigten Bewohnern; Vertuschung durch einflussreiche Gruppen; Gegenmaßnahmen.

### 6.2 Orte und Märkte

Orte mit Geschichte beeinflussen Preise und Besitzerwechsel ([Modul 16](16-Uebernatuerliche-Orte-und-Geografie.md)). Tatorte behalten Blut und Schäden bis zur Reinigung (`23#1074A` Punkt 150; „149-150 ja“ in `23#1075E`).

### 6.3 Kein Gedächtnislöschen

Die Welt erinnert sich; nicht jedes Ereignis wird magisch vergessen (`23#1074A` Punkt 29, „29. ja“ in `23#1075E`). Bei großen Ereignissen kann die Geheimhaltung zeitweise zusammenbrechen (Punkt 27), und über Jahre kann der Glaube wachsen (Punkt 28).

### 6.4 Der Spieler als Teil der Welt

Der Spieler wird nicht zum Mittelpunkt: Er kann Fälle weitergeben, NPC-Hunter lösen sie, und die Welt entwickelt sich weiter (`23#1076A`). Die moralische Entscheidung des Spielers bestimmt nicht allein, was mit einem Wesen geschieht (`M-PD`).

---

## 7. Entwurf: Vorfallmodell

> 💡 **Vorschlag.** Er fügt die Kreaturenseite in den Lebenszyklus der Ermittlungs-Dokumentation ein.

```text
IncidentSeed (aus Ökologie)
├── cause           {creature_id | group_id | curse_id | cult_id | human_actor | none}
├── trigger         Weltzustand erfüllt (Bedingungen aus Aktivitätsregel)
├── weak_signals[]  [{kind: news|police|rumor|animal|injury|anomaly, intensity, due_at}]
└── hypothesis_space  mögliche Erklärungen (supernatural | mundane | mixed | false_positive)

Case (aus Ermittlung 09)
├── seed_ref, truth (Weltseite), knowledge (Spielerseite)
├── escalation_clock
└── resolution_branches[]

Zusammenhang: CreatureAction -> IncidentSeed -> weak_signals -> Case -> Outcome -> WorldFacts
```

**Quellenbezug:** weak_signals und Trigger folgen `M-PD` und `23#307A` Punkt 21; ‚hypothesis_space‘ folgt Punkten 71–75; die Trennung ‚Wahrheit gegen Wissen‘ ist das Fallmodell der [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md).

---

## 8. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Ein dynamischer Vorfall entsteht nur aus einem Weltzustand, nicht aus einem Zufallsspawn | `23#307A` Punkt 21 |
| Vor einem erkannten Fall gibt es mindestens ein schwaches Signal in der Welt | `M-PD` |
| Ohne Marker bleibt jeder Pflichtfall lösbar | `23#308E` Punkt 21; [Ermittlung 23](../Ermittlung/23-Testkatalog-und-Abnahmekriterien.md) T-F27 |
| Ein ignorierter Vorfall hat mindestens ein Weltzustandsergebnis nach Ablauf der Frist | `23#1074A` Punkte 63–68 |
| Ein ausgelassenes Zeitfenster löscht den Fall nicht | `M-PD`; `23#307A` Punkt 22 |
| Ein weitergegebener Fall erzeugt ein Ergebnis durch NPC-Hunter | `23#1075E` (Punkte 62–68) |
| Ein Fall kann ohne Monster enden (False Positive) | Punkt 72 |
| Ein Fall mit gemischter Ursache lässt sich als gemischt erkennen | Punkt 73 |

---

## 9. Beispiele (Entwurf)

Skizzen (💡) zur Prüfung des Modells, keine geplanten Fälle.

**A. Wanderndes Wesen.** Hunter-Druck im Wald → ein Wesen zieht zur Stadt → Vermisstenmeldungen am Stadtrand → Polizei: „Ausreißer" → Forum: Tierverhalten → Spieler sieht Spur: der ursprüngliche Fall ist nicht beendet, die Spur wandert (Punkte 35, 65).

**B. Estate Sale.** Ein Objekt wird versteigert, der Käufer stirbt, das Haus wird verkauft → Preismuster → Vorfall im neuen Haus → Ermittlung zur Besitzgeschichte (Punkte 108–111, 115–118).

**C. False Positive mit realer Gefahr.** Mehrere Vermisste, übernatürlicher Verdacht, tatsächlicher Täter ist ein Mensch, der das Übernatürliche als Tarnung nutzt (Punkte 72, 75).

**D. Machtvakuum.** Der Spieler tötet einen Rudelführer; Nachfolgekampf, mehr Überfälle am Rand des Gebiets, Fall entsteht (`48#142A`, Beispiel Rudel).

---

## 10. Sonderfälle

| Fall | Behandlung |
|---|---|
| Ein Fall ohne Bedingungen im Spiel (kein Winter) | wartet auf die Bedingung; Vorlauf ist ausgesetzt |
| Zwei Fälle im selben Gebiet | gemeinsame Ursache möglich (Punkt 74) |
| Der Spieler kennt die Lösung schon (aus Archiv) | Fall mit wenigen Hinweisen lösbar (Master: ‚kein unnötiges Pflichtziel‘, `M-PD` ‚Story and cases‘) |
| Ein dynamischer Fall und eine Hauptmission kollidieren | Hauptmissionen sind nie verpassbar; dynamische Fälle verschieben sich |
| Co-op: Der Vorfall entsteht im Host-Raum | Host-Weltzustand ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md)) |

---

## 11. Was fehlt

- Rate, Schwellen und Schema der dynamischen Auftragsgenerierung (❓, `M-OD`).
- Die Zuordnung ‚Handlung → Vorfall‘ pro Art.
- Auslegung von „ja genau, aber nur haupt und nebenmissionen ist das aber nicht so“ (❓ Q-K22).
- Dauer von Zeitfenstern und Fristen (nicht Teil dieses Ordners).
