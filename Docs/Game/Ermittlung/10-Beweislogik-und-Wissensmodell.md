# Ermittlung 10: Beweislogik und Wissensmodell

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, **was ein Beweis ist**, wie Wissen entsteht, sich ändert und bewertet wird. Der Master legt **keine** Beweislogik fest (`M-OD`: ‚Exact suspect-confidence and evidence-scoring model‘ ist offen). Beschlossen sind die Beweisarten, die Wissensstatus und einige Grundsätze; alles andere ist **Entwurf** (💡) und als solcher gekennzeichnet. Beispiele sind erfunden. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Voraussetzungen: [Modul 04](04-Archiv-Journal-und-Hunter-Record.md) (Archiv), [Modul 05](05-Spuren-Tracking-Forensik-und-Zeugen.md) (Spurenarten), [Modul 09](09-Falllebenszyklus-und-Generierung.md) (Fallwahrheit). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Begriffsleiter von der Quelle zur Schlussfolgerung](#2-entwurf-begriffsleiter-von-der-quelle-zur-schlussfolgerung)
3. [Entwurf: Beweisarten und ihre Eigenschaften](#3-entwurf-beweisarten-und-ihre-eigenschaften)
4. [Entwurf: Wissensatome und Auflösungen](#4-entwurf-wissensatome-und-auflösungen)
5. [Entwurf: Behauptungen und ihr Status](#5-entwurf-behauptungen-und-ihr-status)
6. [Entwurf: Unzuverlässige Quellen](#6-entwurf-unzuverlässige-quellen)
7. [Entwurf: Widersprüche und ihre Auflösung](#7-entwurf-widersprüche-und-ihre-auflösung)
8. [Entwurf: Hypothesen des Spielers](#8-entwurf-hypothesen-des-spielers)
9. [Entwurf: Provenienz und Beweiskette](#9-entwurf-provenienz-und-beweiskette)
10. [Entwurf: Veränderung von Beweisen über die Zeit](#10-entwurf-veränderung-von-beweisen-über-die-zeit)
11. [Entwurf: Nicht obligatorische Hinweise und Redundanz](#11-entwurf-nicht-obligatorische-hinweise-und-redundanz)
12. [Durchgespieltes Beispiel (abstrakt)](#12-durchgespieltes-beispiel-abstrakt)
13. [Sonderfälle](#13-sonderfälle)
14. [Abhängigkeiten](#14-abhängigkeiten)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| B1 | Beweisarten: Spuren (Fußabdrücke, Blut, Reifen, Schleifspuren, Vegetation, Haare, Fell, Federn, Geruch), Forensik (Fingerabdrücke, DNA, Ballistik, Schuh- und Reifenabdrücke, Fasern, Toxikologie, Todeszeitbereich, Autopsie, Tatortarbeit, Labor), Zeugen, Dokumente, Medien | 📋 | `M-PD`; `M-CA0905`; [Modul 05](05-Spuren-Tracking-Forensik-und-Zeugen.md) |
| B2 | Wissensstatus einer Aussage: bestätigt, vermutet, umstritten, widerlegt, historisch/unverifiziert | 📋 | `M-PD` ‚Hunter Archive epistemology‘ |
| B3 | Quellen können falsch, unvollständig oder veraltet sein (Jägerbücher, Familienwissen, Legenden, erfahrene Jäger) | 📋 | `M-PD` |
| B4 | Beobachtung, moderne Tests, widersprüchliche Quellen und andere Jäger können das Archiv berichtigen | 📋 | `M-PD` |
| B5 | Ein Begleiter kann sich plausibel irren; NPC-Expertise ist keine Allwissenheit | 📋 | `M-PD` |
| B6 | Zeugenaussagen sind Informationen, keine garantierte Wahrheit; NPC-Überzeugungen können falsch sein | 📋 | Spielvision §7.3; `23#1056A` Fragen 25–27 |
| B7 | Das Archiv zeigt nur Entdecktes, keine internen Daten | 📋 | `M-PD` |
| B8 | Spuren können manipuliert und falsch gedeutet werden; Monster und Menschen legen falsche Spuren | 📋 | `M-CA0905`; `23#1074A` Frage 82 |
| B9 | Forensik liefert Zeit, Unsicherheit und gelegentlich Anomalien; kein ‚Täter-Knopf‘ | 📋 | `23#1088A` Fragen 90–92; `23#1090A` |
| B10 | Der Spieler markiert Beweise, Personen und Orte selbst und bildet Hypothesen; das Spiel bewertet sie nicht | 📋 | `M-PD`; `23#54E`; `23#55A` |
| B11 | Der Fall hat mehrere gültige Informationswege; keine versteckten Pflichtziele | 📋 | `23#56E`, `23#57E`, `23#59A` |
| B12 | Die Bewertung der Beweislage durch **Behörden** folgt einem Konfidenzmodell der Verdächtigung | 📋 (Richtung) | `M-PD` ‚Cash / law / evidence‘ |
| B13 | Unterschied: Was geschah, was beobachtet wurde, wen Beteiligte für verantwortlich halten, was Behörden und Organisationen glauben | 💡 des Assistenten im Rahmen der gewählten Option D | `48#154A` |
| B14 | Monster- und Behandlungswissen: Ein altes Buch und ein Zeuge können widersprechen; das Spiel verrät nicht, welcher stimmt | 💡 Beispiel des Assistenten; Grundsatz 📋 durch `48#125E` | `48#126A`, `48#125E` |
| B15 | Monsterforschung: Recherche, Tatortarbeit, Laborforschung, praktische Experimente, Kreaturen untersuchen, eigene Hypothesen; „Nicht jede Untersuchung liefert automatisch die richtige Antwort“ | 📋 (`48#127E`: „was man sich vorstellen kann“) | `48#128A`, `48#127E` |

Zur Einordnung von B15: Der Entwickler wählte die offene Formulierung „was man sich vorstellen kann“ (`48#127E`). Die Liste in `48#128A` ist eine Ausformulierung des Assistenten (💡). Beschlossen ist die Offenheit, nicht die Liste.

Die Entwurfsthemen der Prompt-Sammlung ([Modul 22](22-Abdeckungsprotokoll.md)) nennen für die Beweislogik: nicht obligatorische Hinweise (Titel 011), Hinweisentdeckung (012), Herkunft des Beweises (013), Beweisarten Zeugenaussage, Dokument, Audio/Video, Spur am Tatort, historischer Eintrag (014–018), Spielerhypothese (019), vom Spieler markierte Beweise, Personen, Orte (020–022), persönliche Marker (023), Zeitleiste (024), Notizen (025). Das ist eine **Zerlegung in Themen**, keine Spezifikation.

---

## 2. Entwurf: Begriffsleiter von der Quelle zur Schlussfolgerung

> [!NOTE]
> **Nur Vorschlag (💡).** Die Leiter trennt, was die Quellen verlangen, aber nicht benennen: Wo kommt etwas her, was sagt es aus, wie sicher ist es, was folgt daraus?

```text
Quelle ──► Beleg ──► Behauptung ──► Wissensatom ──► Schlussfolgerung
 (wer/was)  (Ding)    (Aussage)     (Bedeutung)     (Hypothese, Handlung)
```

| Stufe | Bedeutung | Beispiel (erfunden) |
|---|---|---|
| **Quelle** | Wer oder was liefert Information? | ein Nachbar, ein Hunter-Tagebuch, ein Laborbericht, eine Fußspur |
| **Beleg** | Das konkrete, aufbewahrbare Ding oder Ereignis: Aussage, Dokument, Foto, Probe, Spur, Messung | die aufgenommene Aussage des Nachbarn; das Foto der Spur |
| **Behauptung** | Eine konkrete Aussage, die aus dem Beleg folgt (mit Zuverlässigkeit) | ‚Um 01:30 hörte ich eine Stimme.‘ |
| **Wissensatom** | Eine Einheit von Fallwissen, die für Auflösungen und Fähigkeiten zählt | ‚Der Täter war nachts am Haus‘ |
| **Schlussfolgerung** | Was der Spieler daraus macht: Hypothese, Verdacht, Vorbereitung, Anklage | ‚Es muss Wesen X sein‘ |

Warum diese Stufen? Die Quellen verlangen drei Dinge, die ohne diese Trennung nicht zusammengehen:

1. **Mehrere Wege zum selben Wissen** (`23#59A`): Verschiedene Belege können dasselbe Wissensatom stützen.
2. **Unzuverlässige Quellen** (`M-PD`): Dieselbe Behauptung kann aus einer guten und einer schlechten Quelle stammen.
3. **Keine Bewertung durch das Spiel** (`23#55A`): Die Stufe Schlussfolgerung gehört dem Spieler.

---

## 3. Entwurf: Beweisarten und ihre Eigenschaften

Die Beweisarten sind beschlossen (B1). Die **Eigenschaften** pro Art sind ein Vorschlag, welche Fragen das System je Beweis beantworten muss.

| Beweisart | Entsteht | Besondere Eigenschaften (Vorschlag) |
|---|---|---|
| **Physische Spur am Tatort** | Tatort wird untersucht | Verfall durch Wetter und Zeit; kann zerstört oder verändert werden (Reinigung, Tiere, Menschen); ortsgebunden; kann gefälscht sein |
| **Probe** (Blut, Haar, Gewebe, Faser, Rückstand) | Spieler sichert eine Spur | Verderb und Kontamination möglich; braucht Labor oder Werkzeug zur Aussage ([Modul 11](11-Forensik-und-Labor.md)); Beweiskette zählt vor Gericht |
| **Laborbefund** | Analyse einer Probe | hat Dauer, Unsicherheit und Anomalien; kann falsch sein (Fehler, Kontamination); Format uneindeutig (`23#1088A` Frage 91) |
| **Autopsiebericht** | Leichnam wird untersucht | Todeszeitbereich, Ursache, Anomalien; stammt von einer Person (Gerichtsmediziner), die irren oder gedrängt werden kann |
| **Zeugenaussage** | Gespräch mit NPC | Wissen des Zeugen (gesehen, gehört, erzählt bekommen), Zuverlässigkeit, Absicht (lügen, schweigen, schützen); Erinnerung verändert sich ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) |
| **Dokument** | gefunden, beschafft, erhalten | Echtheit, Alter, Autor, Vollständigkeit; Zeitung, Polizeibericht, Tagebuch, Kirchenregister, Brief (`23#55A`, `23#1074A`) |
| **Medium** (Foto, Audio, Video) | selbst aufgenommen oder gefunden | kann bearbeitet, fehlinterpretiert oder falsch datiert sein; auch NPC-Aufnahmen und Livestreams (`M-PD`) |
| **Historischer Eintrag** | Archivrecherche | Muster über lange Zeiträume (`23#1074A` Frage 79); Vertrauen in die Aufzeichnung |
| **Eigene Beobachtung** | Spieler sieht oder hört etwas | gilt als Beleg des Spielers (Spielerbeobachtung), nicht als ‚geprüfte Tatsache‘ ohne Gegenprobe |
| **Verhaltensmuster** | aus mehreren Vorfällen abgeleitet | Opferauswahl, Tatzeit, Ort (`23#1074A` Frage 78) |
| **Anomalie** | Forensik, Tatort, Tierverhalten | etwas, das physikalisch keinen Sinn ergibt (`23#1074A` Frage 83; `23#1088A` Frage 92); erzeugt Hinweise auf das Übernatürliche, nicht auf die Art |

### 3.1 Gemeinsame Eigenschaften (Entwurf)

Jeder Beleg trägt (als Katalog möglicher Felder):

| Eigenschaft | Frage |
|---|---|
| Herkunft | Woher stammt er (Ort, Person, Zeit, Weg)? |
| Zeitpunkt | Wann entstand der Beleg, wann wurde er gefunden? |
| Gegenstand | Worauf bezieht er sich (Person, Ort, Ereignis)? |
| Zugänglichkeit | Wie leicht ist er zu finden? (Bedeutung für Hilfe-Systeme; die Hilfe darf die Zugänglichkeit nicht verändern, R4) |
| Echtheit | echt, gefälscht, unbestimmt (nur dem Fall bekannt, nicht dem Spieler) |
| Verfall | verändert er sich mit der Zeit, durch Eingriffe? |
| Verknüpfung | welche Behauptungen stützt oder widerlegt er? |
| Sichtbarkeit | ist er schon im Wissensstand des Spielers? |

---

## 4. Entwurf: Wissensatome und Auflösungen

### 4.1 Idee

`23#59A` beschreibt einen Fall als **Wissen, Ansätze, Auflösungen**. Ein Wissensatom ist eine kleine, überprüfbare Aussage über den Fall (‚Das Opfer starb vor Mitternacht‘, ‚Das Wesen scheut Eisen‘, ‚Der Nachbar lügt‘). Auflösungen und Fähigkeiten verlangen **Mengen** von Atomen.

### 4.2 Beispielhafte Nutzung (Entwurf)

| Verwendung | Bedingung |
|---|---|
| **Auflösung A** (aus `23#59A`: ‚Monster korrekt identifiziert **und** passende Methode erfolgreich eingesetzt‘) | Der Spieler kennt (oder glaubt) die Ursache korrekt **und** handelt mit einer wirksamen Methode. Die **Methode** wirkt in der Welt; das Spiel prüft die Wahrheit, nicht die Notizen. |
| **Auflösung B** (‚Fluchursprung entdeckt und Fluch gebrochen‘) | Der Spieler führt die Handlung aus, die den Fluch bricht. Wissen allein genügt nicht. |
| **Frühes Lösen** (`23#56E`) | Der Spieler hat nach wenigen Hinweisen die richtigen Atome und handelt. Es zählt, ob das Handeln **zur Wahrheit passt**, nicht wie viele Hinweise gesammelt wurden. |

### 4.3 Die entscheidende Regel (Entwurf)

> **Das Spiel prüft nicht, ob der Spieler ‚genug Beweise‘ gesammelt hat, sondern ob sein Handeln mit der Wahrheit zusammenpasst.**

Das ist die Umsetzung von „das Spiel prüft nicht, ob der Spieler eine Questliste abgearbeitet hat“ (`23#59A`). Folge: Der Spieler kann auch **mit schwacher oder falscher Begründung richtig** handeln (Glück), und er kann **mit starker, aber falscher Begründung falsch** handeln. Das System muss beides zulassen. Ob Glück eine Auswirkung hat (zum Beispiel auf das Journal), ist ❓ Q-E38.

### 4.4 Handeln gegen die Wahrheit

Für den Fall, dass Wissen und Wahrheit auseinanderfallen, gelten die Folgen aus [Modul 02 §5](02-Falltypen-und-Fallmodell.md#5-falsche-und-verfrühte-schlüsse). Die Wirkung ist **in der Welt** (falsche Waffe wirkt nicht, falsches Ritual hat Folgen), nicht in der Benutzeroberfläche. `23#1074A` Frage 95: eine falsche Methode kann dazu führen, dass ein vermeintlich getötetes Wesen zurückkehrt.

---

## 5. Entwurf: Behauptungen und ihr Status

### 5.1 Statuswerte

Beschlossen sind die fünf Wörter (B2). Ein Entwurf für ihre **Bedeutung im Spiel**:

| Status | Bedeutung (Vorschlag) | Beispiel (erfunden) |
|---|---|---|
| **Bestätigt** | mehrfach oder verlässlich gestützt, ohne verbleibenden Widerspruch | Die Zeitung und der Polizeibericht nennen denselben Zeitpunkt, die Autopsie stimmt überein |
| **Vermutet** | von mindestens einer Quelle gestützt, nicht überprüft | Ein Zeuge hat eine Stimme gehört |
| **Umstritten** | Quellen widersprechen sich | Buch sagt ‚nur Silber‘, Zeuge sah Wirkung von Feuer |
| **Widerlegt** | ein verlässlicher Gegenbeleg liegt vor | Labor zeigt, dass die Probe nicht von der behaupteten Person stammt |
| **Historisch/unverifiziert** | alte Überlieferung ohne Prüfmöglichkeit | Legende über das Haus |

### 5.2 Wie sich der Status ändert (Entwurf)

| Auslöser | Wirkung |
|---|---|
| neuer stützender Beleg einer unabhängigen Quelle | vermutet → bestätigt (wenn kein Widerspruch) |
| neuer widersprechender Beleg | vermutet/bestätigt → umstritten |
| eigener Test oder verlässliche Gegenprobe | umstritten → bestätigt oder widerlegt |
| Quelle wird als unzuverlässig erkannt | alle ihre Behauptungen sinken eine Stufe (falls nicht anderweitig gestützt) |
| Quelle wird als gefälscht erkannt | Behauptung wird **widerlegt**; zusätzlich Hinweis auf Täuschung |
| Zeit vergeht (Beleg verfällt) | Status ändert sich nicht, die Möglichkeit der Überprüfung kann sich ändern |

### 5.3 Wer setzt den Status?

Das ist die zentrale Frage dieses Abschnitts, und die Quellen beantworten sie nur teilweise:

| Möglichkeit | Quelle | Stand |
|---|---|---|
| **Das Spiel** setzt den Status aufgrund von Belegen und Regeln | `M-PD` listet Status als Teil des Archivs | 📋 Richtung |
| **Der Spieler** setzt den Status selbst (markiert, bewertet) | `23#54E`: „diese dinge muss der detektiv aber alle selber auswählen“ | 📋 für Relevanzmarkierung; ❓ für Status |
| Der Detektivmodus verbietet automatische Schlüsse und automatische Beweisverknüpfung | `M-PD`, `23#55A` | 📋 |

Daraus folgt ein **Konflikt, der aufgelöst werden muss** (❓ Q-E39): Wenn das Spiel einen Status „bestätigt“ ausweist, ist das ein automatisch gezogener Schluss. Im Detektivmodus ist das verboten. Mögliche Auflösungen (Vorschlag):

| Variante | Beschreibung | Folge |
|---|---|---|
| **V-A: Spielerstatus** | Der Status ist **vom Spieler gesetzt**; das Spiel sagt nie, ob er stimmt | Voll vereinbar mit dem Detektivmodus; Archiv für fremdes Wissen (Bücher) trägt ‚Quellenstatus‘ (aus der Quelle, nicht vom Spiel bewertet) |
| **V-B: Zwei Spuren** | Das Spiel führt einen **Quellenstatus** (‚nach Buch: …‘) und der Spieler führt seinen eigenen **Eigenstatus** | vermeidet Auto-Schlüsse; zeigt dem Spieler Widersprüche (ohne Urteil) |
| **V-C: Hilfe-abhängig** | Das Spiel setzt den Status automatisch, wenn die Hilfe (A7) es erlaubt | Im Detektivmodus gilt V-A |

V-B scheint den Quellen am nächsten zu kommen, weil sie sowohl ‚Wissensstatus‘ (`M-PD`) als auch „das Spiel sagt nicht, ob du richtig liegst“ (`23#55A`) tragen. Es bleibt ein Vorschlag.

---

## 6. Entwurf: Unzuverlässige Quellen

`M-PD` nennt Jägerbücher, Familienwissen, Legenden, erfahrene Jäger. Der Chat ergänzt Zeugen, Dokumente und Gerüchte (B6; `23#1056A` Fragen 25–27). Ein Entwurf, die Arten der Unzuverlässigkeit zu unterscheiden:

| Art | Beschreibung | Wie erkennbar? |
|---|---|---|
| **Irrtum** | Die Quelle glaubt es und irrt sich (falsche Beobachtung, schlechtes Gedächtnis) | Widerspruch zu anderen Belegen |
| **Lücke** | Die Quelle weiß nur einen Teil | fehlende Details; Rückfrage |
| **Veraltung** | früher gültig, heute nicht mehr | Datum, Gegenbeleg |
| **Übertreibung** | Legenden, Gerüchte | Vergleich mit Fakten |
| **Verzerrung** | Gerüchte über Stationen verändern die Aussage | Kette zurückverfolgen |
| **Lüge** | Die Quelle sagt wissentlich Falsches | Motiv, Widerspruch, Verhör ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) |
| **Fälschung** | Beleg ist hergestellt (Dokument, Spur, Medium) | Forensik, Herkunft |
| **Täuschung durch Wesen** | Monster legt falsche Spuren oder gibt sich als anderer aus | Anomalien; Identitätsprüfung (Modul 13) |
| **Gestörte Wahrnehmung** | Besessenheit, Einfluss, Trauma, Drogen | Verhalten, Medizin |
| **Interessenlage** | Sheriff schützt die Stadt, Behörde vertuscht | Hintergrund des Zeugen; Modul 17 |

Eine Quelle kann **mehrere Arten** gleichzeitig haben. Das System muss pro Behauptung wissen, **welche** Art zutrifft, und der Spieler darf sie nur über Handlungen erkennen.

---

## 7. Entwurf: Widersprüche und ihre Auflösung

Die Quellen verlangen ‚widersprüchliche Zeugenaussagen‘ und „falsche Fährten“ (`48#60A`, Ausformulierung des Assistenten, deckt sich mit `M-PD`). Der Entwurf unterscheidet:

| Widerspruchsart | Beispiel | Erwartete Spielerhandlung |
|---|---|---|
| **Aussage gegen Aussage** | zwei Zeugen nennen verschiedene Zeiten | weitere Quelle suchen, Motiv prüfen |
| **Aussage gegen Spur** | der Zeuge sah niemanden, die Fußspuren zeigen jemanden | Zeuge befragen, Spur prüfen |
| **Quelle gegen Quelle** | altes Buch gegen Zeitungsbericht | Alter, Autor, Gegenprobe |
| **Labor gegen Augenschein** | Probe passt zu keinem Verdächtigen | Kontamination, Herkunft, Wiederholung |
| **Wahrheit gegen Wissen** | der Spieler glaubt etwas Falsches | Weltfolge (Modul 09) |
| **Anomalie gegen Erwartung** | normale Analyse ergibt Unmögliches | Hinweis auf Übernatürliches (`23#1074A` Frage 83) |

Das Spiel soll Widersprüche **sichtbar machen, ohne sie zu lösen** (R3 in Modul 08 und `23#55A`). Das heißt: Es darf anzeigen, dass zwei Behauptungen **nicht beide wahr sein können** (wenn der Spieler beide kennt)? Dies ist selbst ein automatischer Schluss. Offen (❓ Q-E40): ob das Journal Widersprüche **hervorhebt** (Hilfe) oder nur beide Aussagen nebeneinander führt (Detektivmodus).

---

## 8. Entwurf: Hypothesen des Spielers

### 8.1 Was beschlossen ist

Der Spieler setzt Hypothesen selbst (`M-PD`: ‚Player manually marks relevant clues/people/places, forms hypotheses‘; `23#55A`: eigene Hypothese setzen oder aus bekanntem Monsterwissen auswählen; das Spiel sagt nicht, ob sie stimmt).

### 8.2 Entwurf: Aufbau

| Element | Beschreibung |
|---|---|
| **Gegenstand** | Worauf bezieht sich die Hypothese? (Ursache, Täter, Methode, Motiv, Ort, Zeitablauf) |
| **Inhalt** | gewählter Wert (aus bekannten Möglichkeiten oder Freitext) |
| **Stützende Belege** | vom Spieler verknüpfte Belege |
| **Gegenbelege** | ebenfalls vom Spieler verknüpft |
| **Vertrauen** | vom Spieler gesetzt (frei) oder nicht vorhanden |
| **Zustand** | offen, verworfen, bestätigt (vom Spieler), im Handeln verwendet |

### 8.3 Wann prüft das Spiel die Wahrheit?

Nur wenn der Spieler **handelt**, nicht wenn er notiert. Beispiele für Handlungen, bei denen die Wahrheit zählt:

| Handlung | Wahrheitsprüfung |
|---|---|
| Waffe, Substanz oder Ritual gegen ein Wesen einsetzen | Wirkung nach Mythologie, nicht nach Notiz |
| Eine Person konfrontieren, anzeigen oder festnehmen | Reaktion hängt von Wahrheit und Beweislage ab |
| Fall schließen (falls es einen solchen Befehl gibt) | siehe [Modul 09](09-Falllebenszyklus-und-Generierung.md) „Falsch gelöst“ |
| Einen Ort betreten, absichern, fallen stellen | Wirkung nach Wahrheit |
| Hilfe anfordern (Polizei, Hunter) | Reaktion hängt von dem ab, was der Spieler **sagt** und belegt (Dialog) |

Ob es einen ausdrücklichen **„Fall abschließen“-Befehl** gibt oder der Fall sich aus Handlungen ergibt, ist offen (❓ Q-E41). `23#59A` beschreibt den Fall der Entscheidung („Mir reicht das“) als natürliche Handlung (Ausrüsten, Jagen). Der Entwurf geht davon aus, dass **Handlungen** den Fall abschließen.

---

## 9. Entwurf: Provenienz und Beweiskette

> [!NOTE]
> **Nur Vorschlag (💡).** `M-B39` nennt als zu planende Themen ‚evidence cleanup/chain-of-custody‘, `M-OD` listet ‚Exact warrant/search/seizure/vehicle-search rules‘ als offen. Konkrete Regeln gibt es nicht.

### 9.1 Zweck

Beweise werden an zwei Orten gebraucht: für die **eigene Ermittlung** (Wissensstand) und für die **Strafverfolgung** (Polizei und Gericht, [Modul 14](14-Polizei-Behoerden-und-Fahndung.md), [Modul 15](15-Gerichte-und-Verfahren.md)). Beide nutzen dieselben Belege, aber unterschiedliche Ansprüche an ihre Herkunft.

### 9.2 Felder der Beweiskette (Katalog)

| Feld | Bedeutung |
|---|---|
| Fundort und Zeit | wo und wann gefunden |
| Finder | wer hat gefunden (Spieler, Polizei, NPC) |
| Sicherung | wie gesichert (Handschuhe, Beutel, Foto) |
| Weg | wer hatte den Beleg danach |
| Lagerung | wo und wie lange |
| Kontamination | wurde etwas vermischt oder berührt |
| Dokumentation | Foto, Protokoll |

### 9.3 Wirkungen (Entwurf)

| Situation | Wirkung |
|---|---|
| Der Spieler sichert eine Probe **ordentlich** | Laborergebnis verwertbar im Verfahren (wenn Spieler Beweis einbringen darf; offen) |
| Der Spieler nimmt etwas vom Tatort, bevor die Polizei kommt | Die Polizei findet den Tatort verändert; Spieler hinterlässt Spuren; Fahndung kann ihn betreffen |
| Der Spieler manipuliert Spuren | Beweismanipulation ist ein Delikt (Option D der Justiz, `48#154A`); Folgen vor Gericht |
| Beweise gehen verloren (Brand, Regen, Reinigung) | Wissensstand bleibt, aber Beweislage vor Gericht sinkt |

Ob der Spieler Beweise **vor Gericht** einbringen darf und was die Polizei annimmt, ist **nicht beschlossen** (❓ Q-E42).

---

## 10. Entwurf: Veränderung von Beweisen über die Zeit

| Ursache | Wirkung | Quelle der Anforderung |
|---|---|---|
| Wetter (Regen, Schnee), Verkehr, Zeit | Spuren verschlechtern sich | `23#1074A` Frage 80; `M-CA0905` |
| Reinigung und Reparatur | Tatort-Spuren verschwinden | `23#1074A` Frage 150 |
| Tiere, Aasfresser | Spuren und Kadaver verändern sich | `M-PD` ‚Carcass decomposition/scavengers/forensics‘ |
| Verderb von Proben | Aussagekraft sinkt | `M-PD` ‚moderate sample/meat spoilage‘ |
| Manipulation durch Täter | Spuren werden verwischt | `23#1088A` Frage 77 |
| Zeugengedächtnis | Erinnerungen werden schwächer oder ungenauer | `23#1056A` Frage 24, „ja“ |
| Digitale Löschung | Daten verschwinden | `M-OD`: ‚CCTV density/ownership/retention‘ offen |

**Grundsatz (Entwurf):** Beweise verfallen in der **Welt**, nicht im Journal. Was der Spieler schon erfahren hat (Foto, Notiz, Aufnahme), bleibt; was er nicht gesichert hat, kann verschwinden. Wie lange der Spieler einen Beleg sichern kann, ist von Zeit und Umständen abhängig (keine Zahlen).

---

## 11. Entwurf: Nicht obligatorische Hinweise und Redundanz

`23#56E` verlangt „keine unnötigen versteckten questziele sondern nur das was nötig ist“. Daraus ergeben sich Anforderungen an die **Gestaltung** der Hinweise:

| Anforderung | Erläuterung |
|---|---|
| **Redundanz** | Wichtige Wissensatome sollen auf mehr als einem Weg erreichbar sein (Abschnitt 4, V1–V3 in Modul 09). |
| **Nicht obligatorisch** | Kein einzelner Beleg ist für alle Auflösungen nötig, außer der Fall ist bewusst ein enger Hauptfall (`23#57E` Punkt 6). |
| **Unterschiedliche Tiefe** | Ein Beleg kann Grundwissen oder Detailwissen liefern. |
| **Zufallsfunde** | Ein Beleg kann zufällig gefunden werden und neuen Fall eröffnen (`23#59A` Fall D). |
| **Keine „toten“ Hinweise** | Hinweise, die auf nichts führen, sind erlaubt (falsche Fährte), aber nicht als Pflicht. |

Ein Entwurfsthema der Prompt-Sammlung ist ‚Alternative sufficient information path support‘ (`M-P03`, Titel 008). Es ist nicht spezifiziert.

---

## 12. Durchgespieltes Beispiel (abstrakt)

> **Hinweis:** Alle Namen, Orte und Ursachen sind erfunden und nur zur Erläuterung der Logik.

**Fallwahrheit:** Ursache U (ein Wesen), das nachts ein Haus aufsucht; wirksame Methode M1 (Substanz S) oder M2 (Ritual R); Opfer 1 ist tot, Opfer 2 gefährdet. Zeuge Z1 (Nachbar) hörte eine Stimme um 01:30; Z2 (Polizist) glaubt an einen Menschen als Täter; ein altes Buch B behauptet, nur Substanz S wirke (falsch: M2 wirkt ebenso).

**Wissensatome (Beispiel):** A1 ‚nächtlicher Angriff‘, A2 ‚Wesen scheut S‘, A3 ‚Wesen ist kein Mensch‘, A4 ‚Opfer 2 ist gefährdet‘.

| Schritt | Spielerhandlung | Neuer Beleg | Behauptung | Status (V-B: Quellenstatus) |
|---|---|---|---|---|
| 1 | Zeitung lesen | Artikel | ‚Ein Todesfall im Haus‘ | bestätigt (verlässliche Quelle) |
| 2 | Z1 befragen | Aussage | ‚Stimme um 01:30‘ | vermutet |
| 3 | Tatort untersuchen | Fußspuren (verwischt, Regen) | ‚Spuren führen zum Wald‘ | vermutet |
| 4 | Z2 befragen | Aussage | ‚Täter war ein Mensch‘ | vermutet, **umstritten** gegenüber Schritt 3 |
| 5 | Autopsiebericht lesen | Befund mit Anomalie | ‚Verletzung passt zu keiner Waffe‘ | vermutet (Anomalie → A3 gestützt) |
| 6 | Hunter-Buch B lesen | Dokument | ‚Wesen scheut S‘ | historisch/unverifiziert |
| 7 | Eigene Hypothese setzen | – | „Ursache U“ | vom Spieler gesetzt |

**Der Spieler beschließt zu handeln (Schritt 8):** Er rüstet sich mit S aus (aus Buch B) und stellt eine Falle.

- **Wahrheitsprüfung:** S wirkt tatsächlich (M1). Der Fall kann nach Auflösung A (identifiziert und wirksame Methode) gelöst werden.
- **Variation 1:** Der Spieler verwendet S gegen U, aber U wurde durch eine Spur falsch gedeutet, und die wahre Ursache ist ein anderes Wesen; S wirkt nicht. → Weltfolge (Opfer 2 stirbt, Monster flieht), „Falsch gelöst“.
- **Variation 2:** Der Spieler hat nach Schritt 5 (Anomalie) die Ursache „übernatürlich“ geschlossen, kennt die Methode aber nicht; er geht ohne Vorbereitung hinein. → Gefahr, Verletzung, Rückzug (Fall bleibt aktiv, Modul 02 §10).
- **Variation 3:** Der Spieler löst den Fall nur über die Aufnahme einer Stimme (Schritt 2) und seine Erfahrung (`23#59A`); wenn er richtig liegt, ist die Lösung gültig, auch wenn er nie die Autopsie gesehen hat.

---

## 13. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Beleg wird zweimal gefunden | Der Spieler findet dasselbe Dokument an zwei Orten (Kopie) | Ein Beleg-Eintrag mit mehreren Fundorten; zweite Fundstelle erhöht ggf. Vertrauen in Echtheit, nicht Aussagekraft |
| Beleg ist **nur** durch Zufall zu finden | Zufallsfund (zum Beispiel Sammelobjekt) | darf nicht zwingend sein (V2 in Modul 09) |
| Zeuge verändert die Aussage | Er wird bedroht oder bestochen | Aussage wird zu zwei Belegen mit Zeitstempel; der Spieler kann den Wechsel erfahren oder nicht |
| Falsches Foto | Medium zeigt etwas, das nicht war (Bearbeitung, Formwandler) | Echtheit ist verborgen bis Prüfung; Modul 13 |
| Beleg wird vom Spieler vernichtet | Absichtlich zerstört | Beweismanipulation (Justiz); Wissen bleibt |
| Zwei Spieler (Koop) haben unterschiedliche Belege | Geteilte Akten | [Modul 18](18-Koop-Ermittlung.md) |
| Mod-Beleg | Mod fügt Belegart hinzu | Muss denselben Eigenschaftenkatalog liefern; keine versteckten Felder |
| Übersetzung | Beleg in fremder Sprache (alte Schrift) | ❓ (Q-E43): ob Sprachkenntnis ein Hindernis ist |
| Beleg in einer anderen Realm | Hölle/Fegefeuer/Himmel | offen (Welt 26) |
| Beleg ohne Täterbezug | Spur, die zu keinem Fall gehört | bleibt im Beweisarchiv; kann später relevant werden (Zufallsfall, `23#59A` Fall D) |

---

## 14. Abhängigkeiten

| Anderes System | Abhängigkeit |
|---|---|
| NPC-Gedächtnis (Gameplay-Systeme §8) | Zeugenwissen und Gerüchte kommen aus dem NPC-Gedächtnis; Quelle der Behauptung ist der NPC mit Wissensstand |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Belege, Status und Verknüpfungen sind Spielstand; Fallwahrheit ist Weltzustand |
| Hunter-Archiv ([Modul 04](04-Archiv-Journal-und-Hunter-Record.md)) | Archiv enthält Belege und Wissensstatus |
| Monster-Mythologie | Wirksame Methoden und Anomalien kommen aus der Artbeschreibung |
| Forensik ([Modul 11](11-Forensik-und-Labor.md)) | liefert Befunde mit Unsicherheit |
| Recht ([Module 14](14-Polizei-Behoerden-und-Fahndung.md), [15](15-Gerichte-und-Verfahren.md)) | verwendet dieselben Belege mit anderer Gewichtung |
| Hilfen ([Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md)) | lesen Wissensstand, nie Wahrheit |
| Companion-App | zeigt Wissensstand; unbestätigtes Wissen nicht als Wahrheit (`48#126A`) |
| Medaillen und Erfolge | Abhängigkeit von Hilfestufen und Beweisqualität offen |

---

## 15. Was fehlt

- Ein beschlossenes **Konfidenz- und Bewertungsmodell** (`M-OD`).
- Die **Zuständigkeit für den Status** einer Behauptung (Q-E39).
- Die Anzeige von **Widersprüchen** (Q-E40).
- Ein **Fallabschluss-Befehl** oder rein handlungsbasierte Auflösung (Q-E41).
- Regeln zur **Beweiskette** und **Zulässigkeit** (Q-E42).
- Sprach- und Schrifthindernisse (Q-E43).
- Ob **Glück** (richtig ohne Begründung) im Journal erkennbar ist (Q-E38).
