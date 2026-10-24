# Kampfsystem 26: Querschnittsszenarien

> [!IMPORTANT]
> **Alle Szenarien sind hypothetisch.** Sie zeigen, wie mehrere Systeme in einer Spielsituation zusammenwirken, und dienen als Prüfhilfe für die Konsistenz der Module 02 bis 25. Jedes Szenario trennt, was bereits **beschlossen** ist (mit Register-ID), was **abgeleitet** ist und was nur **hypothetisch** (Entwurf, offene Frage). Kein Szenario ist eine Missionsbeschreibung, ein Level oder eine Balance-Vorgabe. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Szenario-IDs `CS-01` bis `CS-12`; die abgeleiteten Prüfungen stehen in [Modul 30](30-Testkatalog-und-Abnahmekriterien.md) (Kategorie SZ).

**Inhalt**

1. [CS-01 Unbewaffneter Kampf gegen einen menschlichen Gegner](#cs-01-unbewaffneter-kampf-gegen-einen-menschlichen-gegner)
2. [CS-02 Nahkampf mit improvisierter Waffe](#cs-02-nahkampf-mit-improvisierter-waffe)
3. [CS-03 Schusswechsel in einer städtischen Umgebung](#cs-03-schusswechsel-in-einer-städtischen-umgebung)
4. [CS-04 Kampf gegen ein übernatürlich starkes Wesen](#cs-04-kampf-gegen-ein-übernatürlich-starkes-wesen)
5. [CS-05 Jagd auf eine Kreatur mit unbekannten Schwächen](#cs-05-jagd-auf-eine-kreatur-mit-unbekannten-schwächen)
6. [CS-06 Nichttödliche Gefangennahme](#cs-06-nichttödliche-gefangennahme)
7. [CS-07 Flucht vor einem überlegenen Gegner](#cs-07-flucht-vor-einem-überlegenen-gegner)
8. [CS-08 Schwer verletzter Spieler braucht medizinische Versorgung](#cs-08-schwer-verletzter-spieler-braucht-medizinische-versorgung)
9. [CS-09 Ein besiegter Rivale überlebt und kehrt später zurück](#cs-09-ein-besiegter-rivale-überlebt-und-kehrt-später-zurück)
10. [CS-10 Kampf mit Zeugen und anschließender Polizeiermittlung](#cs-10-kampf-mit-zeugen-und-anschließender-polizeiermittlung)
11. [CS-11 Koop-Kampf mit unterschiedlichen Verletzungszuständen](#cs-11-koop-kampf-mit-unterschiedlichen-verletzungszuständen)
12. [CS-12 Eine persistente Falle wird nach langer Abwesenheit relevant](#cs-12-eine-persistente-falle-wird-nach-langer-abwesenheit-relevant)
13. [Zusammenschau: welche Szenarien welche Lücken zeigen](#zusammenschau-welche-szenarien-welche-lücken-zeigen)

---

## CS-01 Unbewaffneter Kampf gegen einen menschlichen Gegner

**Lage:** Zwei Personen geraten in einem Motelflur in einen Faustkampf. Kein Fight Club, keine Waffe.

| Schritt | Verhalten | Beschlossen (Register) | Hypothetisch / offen |
|---|---|---|---|
| 1 | Der Spieler hat keinen Lock-on; Schläge können verfehlen | `COMBAT-DEC-NK-006` | Zielhilfe: `CQ-15` |
| 2 | Schlagabtausch mit leichten und schweren Angriffen, Distanz und Fußarbeit | `COMBAT-DEC-NK-001`, `-002`, `-005` | Tasten: `CQ-01` |
| 3 | Der Gegner blockt oder weicht aus; die Kombination zerfällt | `COMBAT-DEC-NK-004`, `COMBAT-DEC-DF-008` | Zeitfenster: `CQ-27` |
| 4 | Enger Flur: beide behindern sich, der Gegner wird gegen die Wand gedrückt | `COMBAT-DEC-NK-019`, `-033` | – |
| 5 | Clinch, Takedown, Boden; Aufgabegriff | `COMBAT-DEC-NK-008` bis `-011` | Umfang: `CQ-11` |
| 6 | Treffer am Kopf: Benommenheit, möglicherweise Bewusstlosigkeit | `COMBAT-DEC-NK-028` | Wirkstärke offen |
| 7 | Der Gegner gibt auf, flieht oder wird bewusstlos | `COMBAT-DEC-NK-017` | Schwellen: `CQ-73` |
| 8 | Verletzungen beider bleiben (Wundeinträge, Genesung) | `COMBAT-DEC-VL-001`, `-005` | Zeiten: `CQ-51` |

**Abgeleitet:** Der Ausgang hängt vom Körperzustand beider Beteiligter ab (`COMBAT-DEC-NK-026`, `-034`), nicht von einem Level.
**Zeugen:** Ein Zeuge im Flur kann die Polizei rufen (siehe CS-10).

---

## CS-02 Nahkampf mit improvisierter Waffe

**Lage:** Drei Gegner in einer Bar; der Spieler hat nur eine Glasflasche und einen Stuhl.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Die Gegner greifen gleichzeitig an, flankieren, schneiden den Ausgang ab | `COMBAT-DEC-NK-018` | – |
| 2 | Der Spieler wirft die Flasche (Ablenkung) | `COMBAT-DEC-NK-021`, `COMBAT-DEC-DE-007` | Wurfmechanik offen |
| 3 | Ein harter Flaschenschlag zerbricht die Flasche; der Rest kann bleiben | `COMBAT-DEC-NK-022` | Verletzungswirkung des Rests: Entwurf |
| 4 | Der Spieler geht durch eine enge Tür; nicht mehr alle drei können angreifen | `COMBAT-DEC-NK-019` | – |
| 5 | Ein Gegner zieht eine Pistole; der Spieler drückt sie weg, die Mündungsrichtung zählt | `COMBAT-DEC-NK-024` | – |
| 6 | Grappling gegen mehrere: gefährlich, nicht gesperrt | `COMBAT-DEC-NK-020` | – |
| 7 | Der Spieler wird verletzt (Arm); Waffenführung und Blocken sinken | `COMBAT-DEC-NK-026` | – |

**Abgeleitet:** Mehr Gegner bedeutet mehr Druck durch Raum und Gleichzeitigkeit, nicht durch einen Bonus (`23#221A` §2). **Offen:** Gegenstandsliste und Haltbarkeit (`CQ-33`).

---

## CS-03 Schusswechsel in einer städtischen Umgebung

**Lage:** Nachts in einer Gasse mit parkenden Autos, einem Holzzaun und einer Betonwand; zwei Bewaffnete; Anwohner in den Fenstern.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Der Spieler zieht die Pistole aus dem Holster (schnell), die Schrotflinte liegt im Rucksack (langsamer) | `COMBAT-DEC-WA-005` | Zeiten: `CQ-36` |
| 2 | Deckung: Zaun ist durchschlagbar, Beton stoppt; Autotür dünn, Motorblock schwer | `COMBAT-DEC-FK-016`, `-017` | Materialtabelle: `CQ-45` |
| 3 | Gegner hören das Nachladen; auf Grounded gnadenlos | `COMBAT-DEC-MU-013`, `COMBAT-DEC-KI-022` | – |
| 4 | Abpraller an Beton: selten, nur Grounded standardmäßig | `COMBAT-DEC-FK-018` | – |
| 5 | Treffer am Oberarm: Blutung, Schmerz, Zielen schlechter | `COMBAT-DEC-VL-016`, `COMBAT-DEC-FK-006` | – |
| 6 | Gegner nutzen Sicht und Schall, verlieren die Position, suchen | `COMBAT-DEC-KI-005`, `-006` | – |
| 7 | Anwohner rufen die Polizei; Hülsen und Einschüsse bleiben | `COMBAT-DEC-PO-007`, `-003` | – |

**Offen:** Deckungssystem (`CQ-17`), Schießen auf Fahrzeuge (`CQ-18`), Polizeireaktion (`CQ-84`), Kampf in dichter Umgebung (`CQ-107`).

---

## CS-04 Kampf gegen ein übernatürlich starkes Wesen

**Lage:** Ein schweres Wesen verfolgt den Spieler durch ein Lagerhaus. Die Art und ihre Schwäche sind **hypothetisch** (keine Artenliste).

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Das Wesen zerstört Deckung und zwingt zum Ausweichen statt Tanken | `COMBAT-DEC-DF-011` | – |
| 2 | Normale Kugeln verletzen, töten aber nicht | `COMBAT-DEC-KS-002` | – |
| 3 | Nahkampf: Blocken wirkt nicht, Griffe funktionieren nicht automatisch | `COMBAT-DEC-DF-002`, `COMBAT-DEC-KS-022` | – |
| 4 | Das Wesen nutzt Taktik (Licht ausschalten, schweigen, Fluchtweg blockieren) | `COMBAT-DEC-KI-015`, `-017` | Art-Vorrat offen |
| 5 | Das Wesen wird verletzt, zieht sich zurück, versteckt sich | `COMBAT-DEC-KS-016`, `-017` | – |
| 6 | Der Spieler ist schwer verletzt; Rettung oder Niederlage | `COMBAT-DEC-BW-007` bis `-010` | Todesloop: `CK-01` |

**Abgeleitet:** Die Begegnung kann als Kampf enden, als Rückzug oder als späteres Wiedersehen (CS-09). **Offen:** Konfrontation mit kaum tötbaren Wesen (`CQ-65`), große Kämpfe (`CQ-66`).

---

## CS-05 Jagd auf eine Kreatur mit unbekannten Schwächen

**Lage:** Eine Serie von Vorfällen deutet auf ein unbekanntes Wesen hin. Der Spieler hat Hypothesen, aber keine Bestätigung.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Ermittlung, Archiv-Status ‚Suspected‘ | `COMBAT-DEC-SJ-005`, `-006` | Schwellen: `CQ-67` |
| 2 | Vorbereitung: Waffe und Munition aus dem Kofferraum nach Hypothese | `COMBAT-DEC-SJ-012`, `COMBAT-DEC-WA-004` | – |
| 3 | Konfrontation: die gewählte Munition wirkt kaum; das Wesen reagiert | `COMBAT-DEC-MU-004`, `COMBAT-DEC-SJ-013` | Rückmeldung: `CQ-69` |
| 4 | Beobachtung aktualisiert das Archiv (zum Beispiel ‚Disproven‘) | `COMBAT-DEC-SJ-005` | – |
| 5 | Rückzug, erneute Recherche, zweiter Versuch mit anderer Methode | `COMBAT-DEC-SJ-008`, `-009` | – |
| 6 | Das Wesen hat gelernt, was es gesehen hat | `COMBAT-DEC-NM-002`, `COMBAT-DEC-KI-003` | Lernmodell: `CQ-78` |

**Abgeleitet:** Eine falsche Hypothese ist ein Spielinhalt (`COMBAT-DEC-SJ-015`). **Offen:** Ritualkosten (`CQ-68`).

---

## CS-06 Nichttödliche Gefangennahme

**Lage:** Ein intelligentes Wesen soll lebend gefangen und verhört werden.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Vorbereitung: Falle in einem verlassenen Haus aus einer Vorlage | `COMBAT-DEC-FF-003`, `-005` | Bedienung: `CQ-70` |
| 2 | Das Wesen untersucht, umgeht oder sabotiert die Falle | `COMBAT-DEC-FF-006` | – |
| 3 | Es wird gefangen; es befreit sich nicht durch KI allein | `COMBAT-DEC-FF-007` (💡) | – |
| 4 | Transport zu einer Verwahrung | `COMBAT-DEC-FF-001` | Einrichtungen: `CQ-72` |
| 5 | Verhör und Verhandlung | `COMBAT-DEC-FF-008` | Mechanik: `CQ-90` |
| 6 | Folgen in der Welt (Gedächtnis, Ruf, Story) | `COMBAT-DEC-NM-002` | – |

**Abgeleitet:** Die Falle ist ein persistentes Weltobjekt (`COMBAT-DEC-FF-004`). **Offen:** Rechtlicher Rahmen des Festhaltens (`CQ-72`), nichttödliche Mittel gegen Menschen (`CQ-71`).

---

## CS-07 Flucht vor einem überlegenen Gegner

**Lage:** Der Spieler ist mit einer Verletzung am Bein von mehreren Bewaffneten umstellt.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Die Beinverletzung senkt Tempo und Ausweichen; der Spieler hinkt | `COMBAT-DEC-VL-008`, `COMBAT-DEC-NK-026` | – |
| 2 | Der Spieler bricht die Sichtlinie, die Gegner verlieren die genaue Position | `COMBAT-DEC-KI-005`, `-007` | – |
| 3 | Blutspuren verraten ihn; Hinken und Atmen sind hörbar | `COMBAT-DEC-VL-017`, `COMBAT-DEC-KI-014` | – |
| 4 | Die Gegner suchen lokal, dann erweitert | `COMBAT-DEC-KI-006` | Dauer: Schwierigkeit |
| 5 | Der Spieler ruft einen Verbündeten an | `COMBAT-DEC-BW-008` | Eingabe: `CQ-56` |
| 6 | Zusammenbruch, Rettung, Krankenhaus | `COMBAT-DEC-BW-007` | – |

**Offen:** Fluchtmechanik (`CQ-88`), Hindernisse als Fluchtmittel (`CQ-31`).

---

## CS-08 Schwer verletzter Spieler braucht medizinische Versorgung

**Lage:** Der Spieler hat eine Schusswunde im Oberschenkel mit Blutung und einen offenen Bruch.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Feldbehandlung: Blutung stoppen, reinigen, verbinden | `COMBAT-DEC-VL-001`, `-002` | Menü: `CQ-55` |
| 2 | Offener Bruch und Gefäßtreffer brauchen Krankenhaus | `COMBAT-DEC-VL-003` | Zuordnung: `CQ-52` |
| 3 | Adrenalin lässt nach, Schmerz steigt | `COMBAT-DEC-VL-007` | – |
| 4 | Diagnose bleibt zunächst unsicher | `COMBAT-DEC-VL-009` | – |
| 5 | Krankenhaus: Zeit vergeht, Welt entwickelt sich weiter | `COMBAT-DEC-BW-010` | Wirtschaft: `CQ-58` |
| 6 | Genesung in Stufen; erneute Verletzung wirkt schneller | `COMBAT-DEC-VL-005` | Dauer: `CQ-51` |

**Abgeleitet:** Zwischen Verletzung und Genesung liegen Weltereignisse (Fall, Zeugen, Polizei), die das Journal festhält.

---

## CS-09 Ein besiegter Rivale überlebt und kehrt später zurück

**Lage:** Ein Hunter-Rivale wird im Kampf verletzt und flieht.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Der Rivale hat Ruf, Stil und Gedächtnis | `COMBAT-DEC-NM-008`, `COMBAT-DEC-NK-015` | – |
| 2 | Verletzung und Narbe bleiben | `COMBAT-DEC-NM-004` | – |
| 3 | Der Rivale erinnert sich an Waffen, Methoden, Gnade | `COMBAT-DEC-NM-002` | – |
| 4 | Weltzeit vergeht; der Rivale erholt sich, sucht Verbündete | `COMBAT-DEC-NM-005` | Häufigkeit: `CQ-78` |
| 5 | Wiedersehen: Narben sichtbar, besser vorbereitet | `COMBAT-DEC-NM-005` | Taktik: Artregeln |
| 6 | Der Spieler kann ihn verschonen, vertreiben, töten | `COMBAT-DEC-DE-002` | – |

**Abgeleitet:** Lernen ist Wissensaufbau, kein Stärkerwerden (`COMBAT-DEC-NM-006`). **Konflikt:** Ob der Rivale von Dritten getötet werden darf, ist offen (`CK-02`, `CQ-79`).

---

## CS-10 Kampf mit Zeugen und anschließender Polizeiermittlung

**Lage:** Eine Schlägerei vor einer Tankstelle; Kamera, zwei Zeugen, ein vorbeifahrendes Auto.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Zeugen sehen und hören; Wiedererkennung von Spieler, Fahrzeug, Kleidung | `COMBAT-DEC-PO-005` | – |
| 2 | Notruf mit Latenz und Beschreibung | `COMBAT-DEC-PO-007` | Zeiten offen |
| 3 | Kamera liefert Beweis; Spur am Boden bleibt | `COMBAT-DEC-PO-002`, `-004` | Dichte: `CQ-83` |
| 4 | Polizei sichert Beweise; abgelegte Waffen bleiben | `COMBAT-DEC-PO-003` | – |
| 5 | Fahndung folgt der Beweislage, kein Sternesystem | `COMBAT-DEC-PO-008` | – |
| 6 | Selbstverteidigung wird bewertet; Festnahme ist kein Game Over | `COMBAT-DEC-PO-009`, `-011` | Kriterien: `CQ-82` |
| 7 | Wiederholte Probleme beeinflussen die lokale Polizei | `COMBAT-DEC-PO-015` | – |

**Abgeleitet:** Die Folgen entstehen aus Information, nicht aus Allwissen. **Offen:** Straftatenkatalog (`CQ-83`).

---

## CS-11 Koop-Kampf mit unterschiedlichen Verletzungszuständen

**Lage:** Zwei Spieler jagen gemeinsam; einer ist schwer verletzt, der andere unverletzt.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Beide haben eigene Hunter; Host-Spielstand ist maßgeblich | `COMBAT-DEC-KO-002`, `-003` | – |
| 2 | Körperzustand je Spieler; Behandlung durch den anderen | `COMBAT-DEC-VL-018`, `COMBAT-DEC-KO-012` (🧩) | Netzwerk: `CQ-97` |
| 3 | Der Schwerverletzte fällt aus; das Krankenhaus ist überspringbar | `COMBAT-DEC-KO-007` | – |
| 4 | Friendly Fire gegen Begleiter ist konfigurierbar; gegen Spieler offen | `COMBAT-DEC-GP-023` | `CQ-96` |
| 5 | Gegner merken sich Spieler (Ziel, Gedächtnis) | `COMBAT-DEC-NM-002` | `CQ-98` |
| 6 | Verhaftung betrifft nur einen Spieler; die Welt läuft weiter | `COMBAT-DEC-KO-006` | – |
| 7 | Die Speichersperre bei Entdeckung | `COMBAT-DEC-SP-004` | `CQ-100` |

**Abgeleitet:** Weltobjekte aus dem Kampf gehören dem Host-Weltzustand (`COMBAT-DEC-KO-013`). **Offen:** Schwierigkeit pro Spieler (`CQ-101`), Verbindungsabbruch (`CQ-99`).

---

## CS-12 Eine persistente Falle wird nach langer Abwesenheit relevant

**Lage:** Eine Teufelsfalle in einer Scheune wurde Wochen zuvor vorbereitet; der Spieler kehrt zurück, weil das Wesen in der Gegend wieder auftaucht.

| Schritt | Verhalten | Beschlossen | Hypothetisch / offen |
|---|---|---|---|
| 1 | Die Falle ist ein persistentes Weltobjekt | `COMBAT-DEC-FF-004`, `COMBAT-DEC-SP-010` | Speicherung: `CQ-104` |
| 2 | Regen, Zeit, ein Landwirt, ein Tier haben sie verändert | `COMBAT-DEC-FF-004` | Bereinigungsregel offen |
| 3 | Das Wesen kennt die Falle aus früheren Begegnungen und umgeht sie | `COMBAT-DEC-FF-006`, `COMBAT-DEC-NM-002` | – |
| 4 | Der Spieler repariert oder verstärkt sie | `COMBAT-DEC-FF-004` | Bedienung: `CQ-70` |
| 5 | Im Kampf wird sie ausgelöst, beschädigt oder verwischt | `COMBAT-DEC-FF-011` (💡) | – |
| 6 | Das Ergebnis wird im Weltzustand festgehalten | `COMBAT-DEC-SP-011` | – |

**Abgeleitet:** Entfernte Gebiete werden nicht ständig voll simuliert; der Zustand wird ereignisbasiert fortgeschrieben (`COMBAT-DEC-PF-004`, `-007`).

---

## Zusammenschau: welche Szenarien welche Lücken zeigen

| Offene Frage | Szenarien |
|---|---|
| Eingabebelegung, Zielhilfe, Zeitfenster der Verteidigung | CS-01, CS-02 |
| Deckungssystem, Materialwerte, Polizeireaktion | CS-03, CS-10 |
| Todesloop, Rettung, Krankenhaus | CS-04, CS-07, CS-08 |
| Schwellen für Bestätigung, Rückmeldung der Wirkung | CS-05 |
| Nichttödliche Mittel, Verwahrung, Verhandlung | CS-06 |
| Fluchtmechanik | CS-07 |
| Lernmodell, Tötung wichtiger Figuren | CS-09 |
| Netzwerkarchitektur, Friendly Fire, Speichersperre | CS-11 |
| Speicherbudget, Fallenbedienung | CS-12 |
