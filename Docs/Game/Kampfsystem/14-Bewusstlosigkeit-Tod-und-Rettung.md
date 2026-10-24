# Kampfsystem 14: Bewusstlosigkeit, Tod und Rettung

> [!IMPORTANT]
> **Nichts davon ist implementiert, und der Todesloop ist nicht endgültig entschieden.** Dieses Modul legt die Aussagen der Quellen **nebeneinander**: die Richtung aus dem Master, die spätere Entwicklerantwort zum Gameplay-Tod im Chat 23 und die Rettungs- und Krankenhauspfade. Es erfindet keinen Todesloop. Zwei Aussagen stehen im Widerspruch und sind in Modul 28 als `CK-01` geführt. Der Auftrag verlangt ausdrücklich, dass der Spieler **nicht** durch gewöhnlichen Permadeath aus dem Spiel entfernt wird; das entspricht der Richtung der Quellen (Permadeath nur optional). Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Verletzungsfolgen: [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md); Speichern und Checkpoints: [Modul 24](24-Speicherstaende-und-Weltpersistenz.md); Koop: [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Die Ansätze A bis H und die Antworten des Entwicklers](#3-die-ansätze-a-bis-h-und-die-antworten-des-entwicklers)
4. [Gameplay-Tod, Story-Tod, Permadeath](#4-gameplay-tod-story-tod-permadeath)
5. [Kampfunfähigkeit, Near Death und Bewusstlosigkeit](#5-kampfunfähigkeit-near-death-und-bewusstlosigkeit)
6. [Rettung und Krankenhaus](#6-rettung-und-krankenhaus)
7. [Entwurf: ein Zwei-Ebenen-Modell als Gegenüberstellung](#7-entwurf-ein-zwei-ebenen-modell-als-gegenüberstellung)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Schwere Verletzung ist nicht automatisch Niederlage, Kampfunfähigkeit nicht automatisch Tod** (`23#62E`, `23#85A`, `COMBAT-DEC-GP-026`, `-027`).
2. **Der Entwickler lehnt einen folgenlosen Checkpoint ab** (`23#60E`) und ein ‚GTA-artiges‘ Krankenhaus als zu leicht (`23#88E` B). Er will Folgen in der Welt (`23#62E`).
3. **Die Richtung ist ein Hybrid:** kritischer Zustand, Rettung je nach Lage (Begleiter, Anruf, Rettungskräfte), monsterabhängige Ausgänge, medizinisch plausibler Tod (`23#88E` C, D, G, H; `23#90A`; Spielvision §14.2).
4. **Spätere Entwicklerantwort zum Gameplay-Tod:** Nach einem Tod wird zum letzten Checkpoint zurückgekehrt, mit nur einem minimal überlebensfähigen Gesundheitszustand; Permadeath ist aktivierbar; ein Story-Tod ist storybindend (`23#364E` P2, P3, P16; bestätigt mit ‚der Rest passt‘ `23#366E`). Das ist in `M-PD` und Spielvision §14.2 nicht berücksichtigt; dort steht der Todesloop weiter als OPEN (**`CK-01`**).
5. **Die Nachbarordner sind uneinheitlich:** [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) §3.1 und `K-KS-005` führen den Gameplay-Tod als 📋; die NPC-Dokumentation (`N-NL-013`, `N-LB-023`) und [Spielvision §14.2](../Spielvision.md#142-tod-und-scheitern-noch-offen) führen ihn als ❓.
6. **Offen sind alle Zahlen und Zuordnungen:** was ‚minimal überlebensfähig‘ anatomisch heißt, welche Lage zu welchem Ausgang führt, wie Krankenhaus, Kosten und Zeit aussehen, welche Voreinstellung welche Folgen hat (`CQ-07`, `CQ-56` bis `CQ-62`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-BW-001 | **Kein folgenloser Checkpoint als Todesmodell:** ‚es darf nicht einfach ein Checkpoint sein und du startest von dort‘; man kann das ausschalten, aber es soll nicht Standard sein | 📋 BESCHLOSSEN | `23#60E`; `23#88E` A (‚kann man in den Einstellungen andrehen‘); `M-PD` ‚Death / failure — OPEN‘ | PER SCH | Verhältnis zu BW-009: `CK-01` |
| COMBAT-DEC-BW-002 | **Ein klassischer Checkpoint-Neustart kann als optionaler Komfortmodus existieren** | 📋 BESCHLOSSEN | `M-PD` ‚Death / failure — OPEN‘; Spielvision §14.2; `23#88E` A | PER SCH | – |
| COMBAT-DEC-BW-003 | **Kein Krankenhaus-Reset ohne Folgen ‚wie in GTA‘ als Standard** | 📋 BESCHLOSSEN | `23#88E` B (‚wie in GTA, also zu leicht‘); Spielvision §14.2 | PER | – |
| COMBAT-DEC-BW-004 | **Ein Soulslike-Wiederherstellungsmodell ist nicht das Ziel** (‚es soll kein Souls Game sein‘) | 📋 BESCHLOSSEN | `23#88E` F; `M-PD` ‚Death / failure — OPEN‘ | PER | – |
| COMBAT-DEC-BW-005 | **Der Tod wird medizinisch bestimmt** (Blutvolumen, Trauma, Organschaden, Schock, Bewusstsein), nicht durch ‚HP ≤ 0‘; der Entwickler: ‚ist sowieso klar, dass man das haben soll‘ | 📋 BESCHLOSSEN | `23#88E` H; `23#87A` H (Entwurf); `M-PD` ‚Death / failure — OPEN‘ (medically plausible death risk) | MED | – |
| COMBAT-DEC-BW-006 | **Kritische und sterbende Zustände** (Near Death) liegen vor dem Tod; der Entwickler findet das ‚auch nicht schlecht‘ | 🧭 RICHTUNG | `23#87A` G; `23#88E` G; `23#90A`; `M-PD` ‚Death / failure — OPEN‘ (favored direction) | MED UI | Ausgestaltung: BW-020; `CQ-60` |
| COMBAT-DEC-BW-007 | **Rettung hängt von der Lage ab:** Begleiter, Rettungsdienste, Passanten, Polizei und Ort | 🧭 RICHTUNG | `M-PD` ‚Death / failure — OPEN‘; Spielvision §14.2; `23#88E` C (‚hört sich gut an, nur weiß man nicht, was passiert, wenn man wirklich stirbt‘) | NPC POL | Zuordnung: `CQ-56` |
| COMBAT-DEC-BW-008 | **Der Spieler kann sich aus unmittelbarer Gefahr kämpfen, einen Freund oder Jäger anrufen, zusammenbrechen, gerettet werden und im Krankenhaus aufwachen** | 🧭 RICHTUNG | `23#88E` G (‚ruft man noch seinen Kumpel an… er rettet einen und man wird ohnmächtig und wacht im Spital auf‘); `23#90A`; Spielvision §14.2 | NPC STE | Ob der Anruf eine Taste, ein Menü oder ein Ereignis ist: `CQ-56` |
| COMBAT-DEC-BW-009 | **Monsterabhängige Ausgänge einer Niederlage:** das Wesen geht, entführt, verflucht, besitzt oder tötet, je nach seinen Regeln | 🧭 RICHTUNG | `23#87A` D; `23#88E` D (‚sehr interessant‘); `M-PD` ‚Death / failure — OPEN‘ | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md); `CQ-61` |
| COMBAT-DEC-BW-010 | **Folgen einer Niederlage:** Zeitverlust, neue Opfer, Wechsel des Monsterortes, veränderter Fallzustand, zurückgelassene Ausrüstung, am Ort zurückgebliebenes Fahrzeug, Krankenhaus, bleibende Verletzungen. Der Entwickler: ‚aber die Folgen sind gut‘ | 🧭 RICHTUNG | Spielvision §14.2; `M-PD` ‚Death / failure — OPEN‘; `23#61A` §1; `23#62E` | PER NPC | Welt entwickelt sich weiter (`23#61A`); keine ‚Mission Failed‘-Bildschirme (`23#63A`) |
| COMBAT-DEC-BW-011 | **Ein Fall scheitert nicht zwangsläufig bei einer Niederlage:** Fall und Kreatur bleiben aktiv, der Spieler behandelt, recherchiert und versucht es erneut; die Lage kann sich geändert haben (Versteck gewechselt, weiteres Opfer, Polizei sperrt ab, Zeuge verschwindet, Monster wird vorsichtiger) | 🧭 RICHTUNG | `23#63A` §1 (Entwurf); `23#62E` | ERM NPC | – |
| COMBAT-DEC-BW-012 | **Gameplay-Tod: Rückkehr zum letzten geeigneten Checkpoint mit nur einem minimal überlebensfähigen Gesundheitszustand,** ‚gerade dass man überlebt, das ist dann in Kämpfen extrem schwierig‘; keine kostenlose Vollheilung. Das Heilmodell bleibt unverändert | 📋 BESCHLOSSEN | `23#364E` P2; `23#365A` §1 (Ausformulierung); `23#366E` (‚der Rest passt‘); `K-KS-005`; Kreaturen 07 §3.1 | PER MED | Widerspruch zu `M-PD` OPEN: `CK-01`; Anatomie des Mindestzustands: `CQ-57` |
| COMBAT-DEC-BW-013 | **Permadeath ist aktivierbar, nie Standard** | 📋 BESCHLOSSEN | `23#364E` P3 (‚ja, permadeath ist aktivierbar‘); `23#88E` E; `M-PD` ‚Death / failure — OPEN‘ (nur optional) | PER SCH | Varianten: `CQ-59` |
| COMBAT-DEC-BW-014 | **Ein Story-Tod ist erzählerisch bindend** (‚bei Story-Tod ist das dann storybindend‘); ein Gameplay-Tod ist nicht kanonisch | 📋 BESCHLOSSEN | `23#364E` P16; `23#365A` §2–§3; `K-KS-005`; `W-RE-008` | PER NPC | Jenseitsregeln: [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) |
| COMBAT-DEC-BW-015 | **Es gibt einen Hauptcharakter plus Begleiter über das gesamte Spiel;** nach einem Tod wird nicht jedes Mal ein neuer Charakter erzeugt | 📋 BESCHLOSSEN | `23#362E` P1; `23#365A` §8 | PER | – |
| COMBAT-DEC-BW-016 | **Softlock-Schutz:** kein unendlicher Todes-Loop; im absoluten Ausnahmefall ein älterer sicherer Checkpoint oder der Spieler wählt einen früheren Spielstand, ohne kostenlose Heilung | 🧭 RICHTUNG | `23#365A` §1 (Vorschlag); `23#366E` P2–P13 (‚der Rest passt‘); `23#370A` | PER | Details: Modul 24 |
| COMBAT-DEC-BW-017 | **Safe-Checkpoint-Validator:** vor einem Checkpoint wird geprüft, dass kein Angriff unmittelbar eintrifft, keine tödliche Projektilbahn besteht, kein Griff oder Takedown läuft, keine unmittelbare Explosion, keine nicht reproduzierbare Transition und der Zustand reproduzierbar ist | 💡 VORSCHLAG | `23#367A` (Entwurf des Assistenten) | PER TST | [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| COMBAT-DEC-BW-018 | **Autosaves entstehen vor und nach Kämpfen, gelegentlich bei längeren Fahrten und in der Natur; bei wirklich langen Kämpfen kann ein Autosave zwischen den Phasen liegen** | 📋 BESCHLOSSEN | `23#368E`; `23#371E`; `E-QA-039` | PER | Phasenkriterien: Modul 24 |
| COMBAT-DEC-BW-019 | **Todesfolgen sind ein eigener Einstellbereich** (zum Beispiel Persistent, Reduziert, Checkpoint) und unabhängig von der Kampfschwierigkeit | 🧭 RICHTUNG | `23#61A` §1 (Auswahlfeld im Entwurf); `23#65A` §1; `23#66E` P1; `M-PD` ‚Difficulty / configuration‘ (independently configurable) | SCH PER | Voreinstellungen: `CQ-59` |
| COMBAT-DEC-BW-020 | **Beschreibung eines Near-Death-Zustands (Entwurf):** starke Sichtbeeinträchtigung, extrem eingeschränkte Bewegung, kaum Waffenhandling, Blutverlust, eingeschränkte Wahrnehmung, eventuell Kriechen, Notversorgung möglich, Begleiter kann helfen; kein Arcade-Timer, sondern Vermittlung über Bild, Geräusche, Atmung und Animation | 💡 VORSCHLAG | `23#87A` G | MED ANI UI | `CQ-60` |
| COMBAT-DEC-BW-021 | **Auf Grounded führt derselbe Treffer schneller in den Near-Death-Zustand, und ein weiterer Treffer kann töten;** der Tod ist dort leichter möglich | 📋 BESCHLOSSEN | `23#64E` P1; `23#88E` G (‚das mit Grounded auch nicht schlecht‘); `E-FA-017` | SCH MED | – |
| COMBAT-DEC-BW-022 | **Bewusstlosigkeit entsteht durch Blutverlust, Schock, Schläge gegen den Kopf, Würgegriffe** u. a. und ist ein Systemzustand (Bewusstsein) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (consciousness); `COMBAT-DEC-NK-011`, `-028` | MED | – |
| COMBAT-DEC-BW-023 | **Festnahme ist kein automatisches Game Over;** Verhör, Strafen, Gericht, Gefängnis und vergehende Zeit können auftreten | 📋 BESCHLOSSEN | `E-RE-015`; Gameplay-Systeme §10 | POL | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-BW-024 | **Begleiter können den Spieler retten** (erste Hilfe, Deckung, Herausziehen) und werden dabei selbst verwundbar | 📋 BESCHLOSSEN | `N-CO-006` | NPC | – |
| COMBAT-DEC-BW-025 | **Koop:** Krankenhaus ist überspringbar; der Ausfall dauert wenige Minuten bis etwa 15 Minuten; ein klassischer Modus existiert; es gibt ein detailliertes Rettungssystem mit vereinfachter Wiederbelebung als Option | 📋 BESCHLOSSEN | `E-KO-007`; `E-KO-008` | KOO | `CQ-62` |
| COMBAT-DEC-BW-026 | **Der Spielercharakter kann besessen werden;** dann geht die Kontrolle zum Begleiter, der den Hauptcharakter retten muss | 🧭 RICHTUNG | `23#364E` P8; `23#365A` §8 (Entwurf) | NPC KRE | [Kreaturen 05](../Kreaturen/05-Geister-Besessenheit-und-Fluecher.md) |
| COMBAT-DEC-BW-027 | **Was geschieht bei einem tatsächlichen Tod** (Todesloop, Verhältnis von Rettung, Krankenhaus, Checkpoint und Permadeath je Voreinstellung) ist nicht endgültig entschieden | ❓ OFFEN | `M-PD` ‚Death / failure — OPEN‘; `M-OD` (‚Exact final death / near-death / rescue / hospital / checkpoint behavior…‘); Entscheidungsstatus §5.1; `23#88E` C (‚nur weiß man nicht, was passiert, wenn man wirklich stirbt‘) | PER | `CQ-07` |
| COMBAT-DEC-BW-028 | **Widerspruch:** `M-PD`, `M-OD`, Spielvision §14.2 und die NPC-Dokumentation führen den Todesloop als OPEN; `23#364E` (später, 25.08.2026) und die Dokumentationen Kreaturen 07, Ermittlung 19 und Welt `W-RE-008` führen den Gameplay-Tod als Checkpoint mit Mindestzustand | ⚠️ KONFLIKT | `M-PD` ‚Death / failure — OPEN‘; `23#364E` P2, P16; `N-NL-013`; `K-KS-005` | PER | `CK-01` |
| COMBAT-DEC-BW-029 | **Der anatomische Mindestzustand nach einem Gameplay-Tod** (nicht ‚ein magischer roter HP-Balken‘) ist nicht definiert | ❓ OFFEN | `23#370A` (‚definieren wir beim finalen Death-/Medical-Integration-Block‘) | MED PER | `CQ-57` |
| COMBAT-DEC-BW-030 | **Krankenhaus als Ort** (Aufwachen, Kosten, Dauer, Wirtschaft, Meldung von Schussverletzungen) ist nicht entschieden | ❓ OFFEN | Entscheidungsstatus §5.3 (Krankenhauswirtschaft); `23#61A` (Folgenliste) | PER POL | `CQ-58` |
| COMBAT-DEC-BW-031 | **Abgrenzung zwischen Bewusstlosigkeit, Kampfunfähigkeit und Tod** (Schwellen, Dauer des Near-Death-Zustands) | ❓ OFFEN | `23#61A` (‚wie genau wir Tod von bloßer Bewusstlosigkeit… unterscheiden, sollten wir später noch ausführlich entscheiden‘) | MED | `CQ-60` |

---

## 3. Die Ansätze A bis H und die Antworten des Entwicklers

Der Assistent stellte am 20.08.2026 acht Ansätze zum Scheitern vor (`23#87A`); der Entwickler antwortete mit `23#88E`. Die Tabelle zeigt beides ohne Wertung.

| Ansatz (Assistent, `23#87A`) | Inhalt | Antwort des Entwicklers (`23#88E`) | Stand |
|---|---|---|---|
| A: klassischer Reload | Tod → letzter Checkpoint | ‚kann man in den Einstellungen andrehen, wenn man es nicht so realistisch haben will‘ | optional (`COMBAT-DEC-BW-002`) |
| B: Krankenhaus mit Zeitverlust | kampfunfähig → gefunden → Krankenhaus → Zeit vergeht, Welt läuft weiter | ‚nicht so schlechtes Design, aber das ist dann wie in GTA, also zu leicht‘ | allein nicht ausreichend (`COMBAT-DEC-BW-003`) |
| C: Rescue-System | Folgen hängen von Begleiter, Stadtnähe, Wald ab; manchmal Tod | ‚hört sich gut an, nur weiß man nicht, was passiert, wenn man wirklich stirbt‘ | Richtung (`COMBAT-DEC-BW-007`) |
| D: Monsterabhängige Niederlagen | lässt zurück, entführt, verflucht, besitzt, tötet | ‚sehr interessant‘ | Richtung (`COMBAT-DEC-BW-009`) |
| E: Tod bleibt Tod, Welt wird zurückgesetzt | Save-Auswahl, letzter vollständiger Save | ‚gefällt mir nicht, weil das wie Permadeath ist und das optional sein sollte‘ | abgelehnt als Standard |
| F: Souls-artige Recovery | Ausrüstung am Todesort, zurückholen | ‚könnte man, aber es soll kein Souls Game sein‘ | abgelehnt als Modell (`COMBAT-DEC-BW-004`); Ausrüstung am Ort bleibt Folge (`COMBAT-DEC-WA-015`) |
| G: Near Death | Gesund → … → Near Death → Tod; Notversorgung, Begleiter hilft; Anruf an einen Freund | ‚ist auch nicht schlecht, dann vielleicht ruft man noch seinen Kumpel an… wacht im Spital auf, das mit Grounded auch nicht schlecht‘ | Richtung (`COMBAT-DEC-BW-006`, `-008`) |
| H: Todeswahrscheinlichkeit aus Verletzung | Blutvolumen, Trauma, Organschaden, Schock, Bewusstsein | ‚ist sowieso klar, dass man das haben soll‘ | beschlossen (`COMBAT-DEC-BW-005`) |

Der Assistent fasste zusammen: ‚Hybrid aus B + C + D + G + H‘ (`23#87A`) und später ‚C + D + G + H‘ (`23#89A`, `23#90A`). Das ist eine Auslegung des Assistenten; der Entwickler bestätigte die Richtung nicht gesondert, sondern in der Einzelantwort.

---

## 4. Gameplay-Tod, Story-Tod, Permadeath

**Die spätere Entwicklerantwort (`23#364E`, 25.08.2026):**

| Punkt | Antwort (wörtlich zitiert, soweit in ‚…‘) | Zuordnung |
|---|---|---|
| P1 | ‚genau, aber es soll sich realistisch anfühlen‘ | Gameplay-Tod realistisch erleben |
| P2 | Heilen bleibt, wie es ist; ‚deswegen hat man, wenn man stirbt, direkt dann nur die Mindest-HP, gerade dass man überlebt, das ist dann in Kämpfen extrem schwierig‘ | `COMBAT-DEC-BW-012` |
| P3 | ‚ja, permadeath ist aktivierbar‘ | `COMBAT-DEC-BW-013` |
| P16 | ‚es wird einfach so, wie ich vorher gesagt habe, an den letzten Checkpoint zurückgebracht, bei Story-Tod ist das dann storybindend‘ | `COMBAT-DEC-BW-012`, `-014` |

**Vergleich der Aussagen (ohne Auflösung):**

| Aussage | Quelle | Datum |
|---|---|---|
| ‚es darf nicht einfach ein Checkpoint sein und du startest von dort‘ | `23#60E` | 18.08.2026 |
| ‚gleich Niederlage wäre zu hart‘ | `23#62E` | 18.08.2026 |
| Reload nach Tod: ‚kann man in den Einstellungen andrehen‘; Tod → älterer Save ‚gefällt mir nicht, weil das wie Permadeath ist‘ | `23#88E` | 20.08.2026 |
| Tod → letzter Checkpoint mit Mindestzustand; Permadeath aktivierbar | `23#364E` | 25.08.2026 |
| Todesloop OPEN | `M-PD` ‚Death / failure — OPEN‘ | Stand 31.08.2026 |

**Mögliche Lesart (💡, keine Entscheidung):** `23#60E` und `23#364E` sind vereinbar, wenn ‚nicht einfach ein Checkpoint‘ den **folgenlosen** Checkpoint meint (der Mindestzustand ist die Folge). `23#88E` E (Tod führt zu einem **älteren Spielstand**) und `23#364E` P16 (**letzter Checkpoint**) unterscheiden sich durch den Abstand des Rücksprungs. Ob `M-PD` den späteren Stand nur nicht nachgeführt hat oder ihn bewusst als offen hält, ist aus den Quellen nicht ableitbar. Die Entscheidung bleibt der Projektleitung (`CQ-07`).

---

## 5. Kampfunfähigkeit, Near Death und Bewusstlosigkeit

**Aus den Quellen:**

- Zustandsleiter als Entwurf des Assistenten: Gesund → leicht verletzt → verletzt → schwer verletzt → kritisch → kampfunfähig (`23#63A`), später mit Near Death vor dem Tod (`23#87A`, `23#98A`). Der Entwickler bejaht die Folgen (`23#62E`), die Leiter selbst nicht ausdrücklich.
- Schwere Verletzung kann weiterkämpfen oder fliehen erlauben, aber merklich eingeschränkt (Arm → Sway, Nachladen, Nahkampf; Bein → langsamer, Sprint, Hinken; `23#63A`).
- Der Entwickler räumt ein, ‚es kann durchaus vorkommen, wenn man den Gegner nicht perfekt kennt, dass er einen verletzt und man nicht mehr weiterkämpfen kann‘ (`23#86E` P5).
- Bewusstlos werden: Konsequenz hängt von der Situation ab (Verbündeter dabei, Stadtnähe, allein im Monsterversteck, Monster will nicht töten; `23#63A`).

**Prüfbare Eigenschaften (🧩 ABGELEITET aus `COMBAT-DEC-BW-005`, `COMBAT-DEC-TK-006`):**

1. Der Tod folgt aus dem Körperzustand, nicht aus der Trefferzahl.
2. Ein Kämpfer in Kampfunfähigkeit ist nicht automatisch tot; Handlungen sind eingeschränkt, nicht gelöscht.
3. Derselbe Körperzustand führt auf allen Voreinstellungen zu denselben anatomischen Folgen; unterschiedlich sind Gegnerverhalten, Ressourcen, Rettungswahrscheinlichkeit und Folgen (`COMBAT-DEC-GP-013`, `-014`).

---

## 6. Rettung und Krankenhaus

| Lage | Mögliche Auflösung (Quelle) | Status |
|---|---|---|
| Verbündeter dabei | rettet; Safehouse oder Krankenhaus (`23#63A`, `23#87A` C) | 🧭 |
| Stadtnähe | Passant, Polizei oder Rettung findet den Spieler (`23#87A` C) | 🧭 |
| Allein im Monsterversteck | deutlich gefährlicher; Monster lässt zurück, entführt, verflucht, besitzt oder tötet (`23#63A`, `23#87A` D) | 🧭 |
| Selbst stabilisiert | Möglichkeit 1 (Notversorgung; `23#87A` Favoritenkombination) | 🧭 |
| Anruf an Freund oder Jäger | Kreislauf bricht zusammen; Krankenhaus (`23#90A`) | 🧭 |
| Monster lässt aus eigenen Gründen leben | `23#63A`, `23#87A` (Möglichkeit 4) | 🧭 |
| Besondere Monsterfolge | Möglichkeit 5, `23#87A` | 🧭 |
| Tatsächlicher Tod | Möglichkeit 6, `23#87A` | 🧭 |

Die Möglichkeiten 1 bis 6 sind ein **Entwurf des Assistenten** (`23#87A`, `23#90A`); der Entwickler hat C, D und G bejaht, nicht die Zuordnung von Lagen zu Ausgängen.

**Krankenhaus:** Zeit vergeht, die Welt entwickelt sich weiter, Kosten und Verletzungen können bleiben (`23#61A`, `23#87A` B, Entwurf). Im Koop ist das Krankenhaus überspringbar (`COMBAT-DEC-BW-025`). Die Krankenhauswirtschaft und die Rechtsfolgen (Meldung einer Schussverletzung) sind offen (`CQ-58`).

---

## 7. Entwurf: ein Zwei-Ebenen-Modell als Gegenüberstellung

> **💡 VORSCHLAG (keine Empfehlung).** Das Modell zeigt, dass die Aussagen aus Abschnitt 4 nebeneinander bestehen können. Es wird nicht festgelegt.

```text
Ebene 1: Niederlage ohne Tod (Kampfunfähigkeit, Bewusstlosigkeit)
  -> Rettung oder monsterabhängiger Ausgang (C, D, G)
  -> Zeit vergeht, Welt entwickelt sich, Krankenhaus, Folgen (Spielvision §14.2)

Ebene 2: Tatsächlicher Tod (medizinisch bestimmt, H)
  -> Gameplay-Tod: letzter geeigneter Checkpoint, Mindestzustand (364E)
  -> Permadeath nur, wenn aktiviert
  -> Story-Tod: erzählerisch bindend
```

| Alternative | Beschreibung | Vorteil | Nachteil |
|---|---|---|---|
| A: Zwei Ebenen wie oben | Rettung als Normalfall der Niederlage, Checkpoint nur beim echten Tod | verbindet `23#88E` und `23#364E` | Tod wird seltener ‚erlebt‘, Rückfall-Checkpoints brauchen Validator |
| B: Nur Rettung/Folgen, Tod nur Permadeath | kein Gameplay-Tod-Checkpoint | maximale Konsequenz | widerspricht `23#364E` P16 |
| C: Nur Checkpoint mit Mindestzustand | alle Niederlagen laufen über den Tod | einfach | widerspricht `23#88E` G, C, D und Spielvision §14.2 |
| D: Voreinstellungen entscheiden | Story: Checkpoint; Normal: reduzierte Folgen; Survival: persistente Folgen; Grounded: schwere Folgen (`23#87A` Tabelle) | passt zur Einstellbarkeit | viele Pfade zu testen; Entscheidung steht aus |

Die Wahl bleibt der Projektleitung (`CQ-07`, `CQ-59`).

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Verletzung ([Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md)) | Körperzustand als Grundlage von Kampfunfähigkeit, Bewusstlosigkeit, Tod |
| zu NPCs und Begleitern | Rettung, Anruf, Gedächtnis ([NPCs 19](../NPCs/19-Begleiter-Crew-und-Rekrutierung.md)) |
| zu Kreaturen ([Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md)) | monsterabhängige Ausgänge, Besessenheit |
| zur Ermittlung und Recht | Zeit im Krankenhaus, Festnahme, Meldungen |
| zur Persistenz ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | Checkpoint, Validator, Softlock-Schutz, Permadeath |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Ausfall, Wiederbelebung, Zustimmung |
| zur Oberfläche | Darstellung von Near Death, Bewusstlosigkeit, Aufwachen |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Tod ohne sicheren Checkpoint | Softlock-Schutz greift (`COMBAT-DEC-BW-016`) | 🧭 |
| Tod während eines Griffs, Takedowns oder einer Explosion | Checkpoint-Wahl verwendet den Validator, keine Wiederholung derselben tödlichen Situation | 💡 |
| Wiederholter Tod am selben Checkpoint | Hilfe/Alternative ohne Gratisheilung | 🧭 |
| Permadeath aktiv, Spieler stirbt | Spielstand endet gemäß Regel; Mehrfachspeicherstände nach Voreinstellung | ❓ |
| Gleichzeitiger Tod beider Koop-Spieler | Koop-Regel (offen) | ❓ |
| Rettung durch einen toten Begleiter | nicht möglich; Fallback | 💡 |
| Rettung, aber die Welt läuft weiter (Zeit vergeht) | Journal zeigt Folgen | 🧭 |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-07` | Endgültiger Todes- und Scheiterloop: Verhältnis von `23#364E` zu `M-PD` ‚Death / failure — OPEN‘, Rolle von Rettung, Krankenhaus, Checkpoint und Permadeath je Voreinstellung | `COMBAT-DEC-BW-001`, `-012`, `-027`, `-028` | ja (`CK-01`) |
| `CQ-56` | Zuordnung der Lagen zu Rettungsausgängen, Eingabe des Anrufs, Wahrscheinlichkeiten | `COMBAT-DEC-BW-007`, `-008` | ja |
| `CQ-57` | Anatomischer Mindestzustand nach einem Gameplay-Tod | `COMBAT-DEC-BW-012`, `-029` | ja |
| `CQ-58` | Krankenhaus: Aufwachen, Kosten, Dauer, Wirtschaft, Meldung von Schussverletzungen | `COMBAT-DEC-BW-030` | ja |
| `CQ-59` | Voreinstellungen der Todesfolgen und Varianten des Permadeath | `COMBAT-DEC-BW-013`, `-019` | ja |
| `CQ-60` | Near Death: Schwellen, Dauer, Handlungsspielraum; Abgrenzung Bewusstlosigkeit–Tod | `COMBAT-DEC-BW-006`, `-020`, `-031` | ja |
| `CQ-61` | Niederlage durch Monster (Entführung, Fluch, Besessenheit des Spielers): Liste und Regeln | `COMBAT-DEC-BW-009`, `-026` | ja |
| `CQ-62` | Koop-Tod: Gleichzeitiger Ausfall, Wiederbelebungsregeln, Zeitsprung | `COMBAT-DEC-BW-025` | ja |
