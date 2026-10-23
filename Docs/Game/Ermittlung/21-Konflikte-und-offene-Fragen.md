# Ermittlung 21: Konflikte und offene Fragen

> [!IMPORTANT]
> **Dieses Modul entscheidet nichts.** Es sammelt Widersprüche, unklare Antworten und offene Fragen der Ermittlungs-Dokumentation mit Fundstellen. Eine Auflösung steht dem Entwickler zu. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Konflikte und Abweichungen](#1-konflikte-und-abweichungen)
2. [Unklare Antworten](#2-unklare-antworten)
3. [Abweichungen zu den Überblicksdokumenten](#3-abweichungen-zu-den-überblicksdokumenten)
4. [Veraltete Annahmen](#4-veraltete-annahmen)
5. [Offene Fragen](#5-offene-fragen)
6. [Prüfung der offenen Fragen gegen die Quellen](#6-prüfung-der-offenen-fragen-gegen-die-quellen)

---

## 1. Konflikte und Abweichungen

| ID | Gegenstand | Aussage A | Aussage B | Stand |
|---|---|---|---|---|
| K-E1 | Zahl der Sammelobjekte | „Das Basisspiel enthält immer die vollständigen: ca. 1.200–1.500 physischen Collectibles“ (`23#55A`, 18.08.2026); `M-PD` (31.08.): ‚Base game target: ~1,200–1,500 physical collectibles‘ | `M-CA0905` Abschnitt 6 (05.09.): Die Zahl gehörte zum **alten ~200-km²-Konzept**; für die größere Welt offen, je Kategorie zuzuordnen; Entwickler `23#1077E` (04.09.): „da die map so groß geworden ist, sollten es noch mehr collectibles geben“ | Das jüngste Dokument des Masters setzt die Zahl auf historisch (🕰️); die Entwicklerforderung geht auf **mehr**. Neue Zahl ❓ |
| K-E2 | Größe und Art der Nebenfälle | Entwickler `23#46E`: „nur so kleine fälle so wie in dr serie solche filler folgen“ | `M-PD`: ‚Numerous high-quality side cases‘; `23#1076A`: ‚große handgebaute Nebenfälle‘ plus „kleinere dynamische Hunter-Aufträge“ | Keine echte Gegenaussage: Die Hierarchie Hauptmission, großer Nebenfall, kleiner dynamischer Auftrag erklärt beide. Zahlen und Umfang je Ebene ❓ |
| K-E3 | Kriminalität nach Vierteln | Entwickler `23#1057E` Punkt 47: manche Viertel sind gefährlicher, mit einem Vergleich zum realen Land (Wortlaut dort) | Assistent `23#1058A`: Modellierung über Faktoren (Armut, Arbeitslosigkeit, Polizeipräsenz …), nicht über die Herkunft der Bewohner als Ursache | Der Entwickler nahm zur Regel nicht Stellung. Welche Ursachen gelten, ist ❓ (Entscheidung nötig) |
| K-E4 | Gesellschaftliche Prägung der Region | Entwickler `35#532E`, `35#534E`: eher konservative Welt, Umgang mit Randgruppen (Wortlaut dort) | Assistent `35#533A`: keine politische Seite pauschal hartkodieren; Randgruppen als konkrete Personen und Fraktionen | Der Entwickler ergänzte `35#534E`, er meine nicht, dass eine Seite schlechter sei. Umsetzung ❓ |
| K-E5 | Hilfestufen der Ermittlung | `23#51A`: vier Stufen (Investigativ bis Sehr geführt) | `23#52E`: „es soll einen detektiv modus geben der keine hilfen hat, gar keine“ | Die vier Stufen sind durch die Entwicklerkorrektur überholt (🕰️), Zwischenstufen ❓ |
| K-E6 | Niederlage nach Verletzung | Assistent `23#61A`: Niederlage als Alternative zum Neuladen, mit Folgen | Entwickler `23#62E`: „gleich niederlage wäre zu hart, weil dann wäre das spiel zu schwer“; `23#64E`: im härtesten Modus „ein kleiner fehler und du bist im krankenhause“ | Gelöst für die Verletzungsstufen; der **Ablauf des Spielertods** bleibt offen (`M-B09`, `M-CA0905` Abschnitt 7) |
| K-E7 | Konsequenzen bei Fehlschlag und Neuladen | `23#57E` Punkt 3: der Entwickler möchte das Fehlschlagen abschalten können und dann einen Checkpoint neu laden | `23#62E`/`23#61A`: Konsequenzen als Standard | Beides ist vereinbar (Standard mit Wahl). Auswahlmenü und Bindung an die Schwierigkeit ❓ |
| K-E8 | „Fahndungsstufen“ gegen ‚kein Sternesystem‘ | `M-PD` (31.08.): ‚Strafsystem statt Sterne‘ (Überblick Gameplay-Systeme §10); `23#1058A`: „Kein magisches GTA-Sternesystem.“ | `48#154A` Option A „GTA-ähnlich: Fahndungsstufen …“; `48#156A` (Ausformulierung der gewählten Option D) führt „Fahndungsstufen“ in der Tabelle | Entwickler wählte D ohne Kommentar zu Stufen (`48#155E`). Ob Stufen sichtbar sind, ist offen; [Modul 14 §2](14-Polizei-Behoerden-und-Fahndung.md#2-konflikt-fahndungsstufen-und-kein-sternesystem) löst es im Entwurf über eine interne Intensität |
| K-E9 | Gemeinsamer Koop-Fortschritt gegen Host-Welt | `48#103E`: „Gemeinsamer Missionsfortschritt für alle“ (Option B); `48#104A`: „für alle teilnehmenden Spieler als abgeschlossen gespeichert“ | `48#105E`, `48#107E`: Welt des Hosts, Fortschritt am Spielstand, Gast behält seinen Stand; `48#108A`: „Keine Übertragung zwischen Spielständen.“; `CLAUDE.md` §6 | Die spätere Aussage präzisiert die frühere. Was ein Gast als Erfahrung oder Belohnung in seinem eigenen Spielstand erhält, ist offen; [Modul 18 §2](18-Koop-Ermittlung.md#2-klärung-gemeinsamer-fortschritt-gegen-host-welt); August-Entscheidungen zu Koop und SPN Online (`23#440E`, `23#444E`, `23#446E`) betreffen einen anderen Raum, siehe [Modul 18 §14](18-Koop-Ermittlung.md#14-ergänzungen-aus-der-quellenauswertung-koop-und-online) |
| K-E10 | Hauptkarte in `M-OD` | `M-OD` (31.08.): Hauptkarte ‚within the current ~200–300 km² direction‘ | spätere Entscheidungen: größere Welt (`M-CA0905` 1.500–1.800 km²; `M-MS1013` ~2.000–2.200 km²) | `M-OD` ist für diesen Bereich veraltet; die Offenpunkte zu Recht und Beweisen gelten weiter (Abschnitt 4) |
| K-E11 | Zwei Beschreibungen des Journals | `23#55A`: Falljournal, Hunter Archive, Hunter Record als drei Ebenen (Assistent) | `48#62A`: Hunter-Journal, Ermittlungswand, Forensik und Beweisarchiv, Kartenmarkierungen als vier Werkzeuge (Assistent) | Keine Gegenaussage: zwei Gliederungen derselben Funktionen; die Zuordnung zu Menüs ist offen |

---

### 1.1 Nachtrag aus der Quellenauswertung (Etappe 4)

| Konflikt | Neue Fundstellen | Neuer Stand |
|---|---|---|
| **K-E1** Sammelobjekte | `25#13E` (18.08.): Der Entwickler wollte die Sammelobjekte „auf 1200-1500 erhöhen“, „dafür nur richtig gute und überlegte“, und erwog einen „spielmodus mit weniger collectibles für faule leute“; `23#55A`/`23#54E` ließen den reduzierten Modus fallen („immer vollständig“) | Die Zahl 1.200–1.500 ist eine **Erhöhung** vom 18.08. für das frühere Weltkonzept; `M-CA0905` (05.09.) stuft sie als historisch ein. Offen bleibt die Zahl für die größere Welt. Der Modus ‚weniger Sammelobjekte‘ ist **nicht** beschlossen |
| **K-E4** Prägung der Region | `35#535A`, `35#536E`: „Nein die welt soll hauptsächlich logisch denken und nicht emotional“; `35#537A`, `35#538E` „Ja genau“ | **Teilweise aufgelöst:** Die Grundlogik ist Ursache, Kontext, Beweis, Verhältnismäßigkeit ([Modul 06 §14](06-Kriminalitaet-Polizei-und-Justiz.md#13-ergänzungen-aus-der-quellenauswertung)). Offen bleibt, wie stark die regionale Mehrheitskultur gezeichnet wird |
| **K-E5** Hilfestufen | `48#61E`, `48#63E`: Tipps in den Einstellungen mehr oder weniger, viele Einzelregler | Verfeinert, nicht aufgelöst (Zwischenstufen offen) |
| **K-E6** Niederlage | `23#87A`/`23#88E` (acht Ansätze), `23#361A`/`23#362E` | Siehe K-E12 |
| **K-E8** Fahndungsstufen | `23#612A` Punkt 16, `23#510A`: interne Threat/Escalation Levels 0–6 „ohne GTA-Sterne“; `23#613E` ohne Einwand | **Entschärft, nicht entschieden:** Wahrscheinlich sind die „Fahndungsstufen“ des Quiz die internen Stufen; ob etwas angezeigt wird, ist offen |
| **K-E9** Koop-Fortschritt | Chat 23 (25.–28.08.): SPN Online getrennt vom Koop, ein Fall gilt für beide Teilnehmer als abgeschlossen (`23#446E`) | betrifft Online, löst den Konflikt im privaten Koop nicht; Chat 35 enthält dazu nichts |

| ID | Gegenstand | Aussage A | Aussage B | Stand |
|---|---|---|---|---|
| K-E12 | Tod des Spielers | `23#361A` D, `23#362E`: normales Gameplay: echter Tod führt zu Checkpoint oder Save, Near-Death nur wo plausibel; `23#60E`: „es darf nicht einfach ein checkpoint sein“ (August) | `48#117E`: Krankenhaus mit beschleunigter Heilung als Standard, klassische Einstellung „wie D“, optional Permadeath | Der Oktoberstand ist der jüngste ausdrückliche; der **Ablauf** ist offen (`23#86E`: „tod definieren wir noch“; `M-B09`). Zwischenstand der acht Ansätze: [Modul 02 §11.1](02-Falltypen-und-Fallmodell.md#111-niederlage-und-tod-die-acht-ansätze-vom-20082026) |
| K-E13 | Kopfgeld und Wanted | `23#603A` Punkt 15: „Wir brauchen keine RDR2-Kopfgeldjäger.“; ein Problem verschwindet nicht durch eine kleine Zahlung | `23#598E` (Analyse-Prompt) nennt „Kopfgeld → spätere Kopfgeldjäger“ als wichtige Systemkette eines **anderen** Spiels; `23#1057E` Punkt 65: Fahndung bleibt, „bis man kopfgeld bezahlt, oder die klage zurückgezogen oder man freigesprochen wird“ | Kein Entwicklerbeschluss für ein Kopfgeldjäger-System; „Kopfgeld“ steht beim Entwickler als Bedingung des Endes einer Fahndung. [Modul 14 §17.6](14-Polizei-Behoerden-und-Fahndung.md#176-wanted-und-kopfgeld-in-der-modernen-welt) |
| K-E14 | Dauer des Unglaubens und Wissen | `48#147E`: Wissen ist regional und dynamisch | `23#361A`: Menschen glauben Unglaubliches nicht (Assistent) | vereinbar; keine Gegenaussage |

## 2. Unklare Antworten

| ID | Stelle | Problem |
|---|---|---|
| U-E1 | `23#1057E` Punkt 67 („ja kann man auch machen“) zur Frage, ob Beweise nur selektiv simuliert werden oder das zu viel wäre (`23#1056A` Frage 67) | Die Antwort lässt offen, ob „selektiv“ oder „ausführlich“ gemeint ist. Der Überblick im Master (`M-PD`, `M-CA0905`) listet viele Beweisarten, ohne Tiefe. |
| U-E2 | `23#1075E` Punkt 71 („ja genau, aber nur haupt und nebenmissionen ist das aber nicht so“) | Der Satz wird nicht aufgelöst; er scheint die Aussage zu mehrdeutigen Fällen zu begrenzen. Der Bezug ist nicht gesichert. |
| U-E3 | Sammelantworten „45-54 passt“, „76-83 ja“ usw. | Pauschalzustimmung zu Gruppen von Vorschlägen; der Inhalt einzelner Fragen ist nur durch `23#1074A` bestimmt. In den Modulen als **Richtung** geführt. |
| U-E4 | `48#149E` (Geheimhaltung), `48#161E` (Koop-Verhaftung), `48#171E` (staatliche Forschung), `48#177E` (Elite), `48#179E` (Ziele der Elite) | Die Antworten benennen keinen Buchstaben. Sie sind als Richtung geführt; die Zuordnung zu den Optionen A bis D ist Auslegung. |
| U-E5 | `48#117E` („man kann das aber auch einstellen das man es klassisch hat also wie D“) | „wie D“ bezieht sich auf eine Option der Frage `48#116A` (D: Unterschiedliche Spielmodi); gemeint ist vermutlich ein klassischer Modus. |
| U-E6 | `48#59E` (Beispielfrage zur Komplexität) | Die Frage war ausdrücklich als Beispiel gestellt (`48#58A`); die Antwort wurde vom Assistenten als Festlegung behandelt (`48#60A`). Sie ist hier als Entscheidung geführt, weil `48#61E` sie bestätigt und ausführt. |
| U-E7 | `48#63E` („so viele einstellungen mehr noch als bei tlou2“) | Keine Zuordnung zu den Notizoptionen A bis D von `48#62A`; die Aussage gilt dem Gesamtumfang der Einstellungen. |

---

## 3. Abweichungen zu den Überblicksdokumenten

Beim Vergleich mit [Spielvision](../Spielvision.md), [Gameplay-Systeme](../Gameplay-Systeme.md) und [Entscheidungsstatus](../Entscheidungsstatus.md) wurden **keine inhaltlichen Widersprüche** gefunden. Die Überblicke sind kürzer, nicht anders. Zwei Hinweise:

1. [Spielvision §8](../Spielvision.md#8-das-hunter-archiv) nennt die 1.200–1.500 Sammelobjekte als historisch (🕰️) für das frühere Weltkonzept. Das stimmt mit `M-CA0905` Abschnitt 6 überein; die Entwicklerforderung `23#1077E` nach **mehr** Sammelobjekten steht dort nicht und wurde hier ergänzt (K-E1).
2. Die Überblicke nennen den **vorgesehenen Referenzweg** und die Journalfunktion als 📋. Dieses Modul hält zusätzlich fest, dass die genaue Menüaufteilung des Falljournals nur eine Beispielstruktur des Assistenten ist (💡).

Die Überblicksdokumente wurden **nicht geändert**.

---

## 4. Veraltete Annahmen

| Annahme | Stand | Hinweis |
|---|---|---|
| Hauptkarte etwa 200–300 km² | `M-OD`, `M-PD` (31.08.2026) | überholt durch die Entscheidungen zur größeren Welt (K-E10) |
| 1.200–1.500 Sammelobjekte für das Basisspiel | `23#55A`, `M-PD` | historisch für das frühere Weltkonzept (K-E1) |
| Vier Hilfestufen mit festen Namen | `23#51A` | überholt (K-E5) |
| ‚Niederlage statt Neuladen‘ als Standard | `23#61A` | vom Entwickler abgeschwächt (`23#62E`, K-E6) |
| Gemeinsamer Koop-Fortschritt als Übernahme in Gast-Spielstände | `48#104A` | durch Host-Welt-Regel präzisiert (K-E9) |
| Gefängnis nur als Zeitsprung | Option A (`48#156A`) | nicht gewählt; Ziel ist D |

## 5. Offene Fragen

### 5.1 Fallmodell

| ID | Frage |
|---|---|
| Q-E01 | Welche Datenstruktur hat ein Fall (Wissen, Ansätze, Auflösungen)? Reicht das Beispiel mit Auflösungsbedingungen? |
| Q-E02 | Wie wird ein Schluss als **richtig** oder **falsch** bewertet, wenn mehrere Lösungswege gelten? |
| Q-E03 | Welche Folgen hat ein falscher oder verfrühter Schluss, und gibt es Stufen? |
| Q-E04 | Welcher Anteil der Fälle ist gewöhnlich, mehrdeutig, übernatürlich? |
| Q-E05 | Wie wird der Anfang des Spiels gestaltet (Ereignis mit einem Monster)? |
| Q-E06 | Wie läuft der Tod des Spielers ab (permanent, Ersatz, Reduktion)? |

### 5.2 Ermittlungsmodi

| ID | Frage |
|---|---|
| Q-E07 | Wie viele geführte Stufen gibt es, und was ergänzt jede? |
| Q-E08 | Zeigt die Karte im Detektivmodus **bekannte** Orte, nur selbst markierte, oder keine? |
| Q-E09 | Können Begleiter im Detektivmodus Hinweise geben, und wie viel? |
| Q-E10 | Soll es eine Auswahl ‚Fehlschlag oder Neuladen‘ geben und wie ist sie an die Schwierigkeit gebunden? |

### 5.3 Archiv, Journal, Record

| ID | Frage |
|---|---|
| Q-E11 | Wie viele Sammelobjekte und Entdeckungen hat die größere Welt (je Kategorie)? |
| Q-E12 | Welche Menüaufteilung hat das Falljournal (Beweise, Personen, Orte, Dokumente, Zeitlinie, Hypothesen, Notizen)? |
| Q-E13 | Soll eine exakte Fortschrittsanzeige (Prozentzahlen) später als Option kommen? |
| Q-E14 | Wie werden Mod-Inhalte im Archiv gekennzeichnet? |

### 5.4 Spuren, Forensik, Zeugen

| ID | Frage |
|---|---|
| Q-E15 | Wie lange brauchen Labore, wie präzise ist die Todeszeit, wie lange halten Spuren? |
| Q-E16 | Welche Gegenstände tragen Fingerabdrücke, DNA oder Fasern? |
| Q-E17 | Wie wird das Vertrauensmodell für Zeugenaussagen und digitale Beweise gebaut? |
| Q-E18 | Wie lange bleibt ein Tatort bestehen, und wer reinigt ihn? |
| Q-E19 | Wie genau funktioniert die Beweiskette bei Beweisen, die der Spieler bewegt? |

### 5.5 Recht und Polizei

| ID | Frage |
|---|---|
| Q-E20 | Welche Straftaten gibt es, mit welchen Schweregraden und Strafen? |
| Q-E21 | Wie sieht die Zuständigkeitskarte aus (Städte, Counties, Bundesstaaten, Bund)? |
| Q-E22 | Wie wirken Kaution, Anwälte, Bewährung und Berufung? |
| Q-E23 | Wie wird Kriminalität nach Vierteln begründet (K-E3)? |
| Q-E24 | Wie ist die Region gesellschaftlich geprägt (K-E4)? |
| Q-E25 | Wie sieht die Polizeikarte je Schwierigkeit aus? |
| Q-E26 | Wie werden Fahndung, Anklage und Haft im Weltzustand gespeichert? |

### 5.6 Detektivpfad und organische Fälle

| ID | Frage |
|---|---|
| Q-E27 | Welche Voraussetzungen hat die Lizenz, und wie funktioniert das Büro wirtschaftlich? |
| Q-E28 | Wie entsteht ein dynamischer Auftrag (Generator, Zufall, Simulation) und wie oft? |
| Q-E29 | Wie entscheidet das Spiel, wann ein gewöhnlicher Fall zum Hunter-Fall wechselt? |
| Q-E30 | Wie lange bleiben ignorierte Fälle offen, und was passiert mit Altfällen? |
| Q-E31 | Wie ist das Hunter-Forum technisch und im Online-Modus eingebunden? |

### 5.7 Einstellungen und Hilfen (Etappe 2)

| ID | Frage |
|---|---|
| Q-E32 | Dürfen einzelne Fälle eigene Freiheitsgrade haben, oder gilt der Grundsatz C für alle Fälle gleich? |
| Q-E33 | Beeinflussen Hilfestufen oder die Nutzung des Bots Medaillen und Erfolge? |
| Q-E34 | Darf der Bot Hypothesen des Spielers bewerten? |

### 5.8 Fallmodell und Zeit (Etappe 2)

| ID | Frage |
|---|---|
| Q-E35 | Wie schnell eskalieren Vorfälle? Wie viele Leads, Aufträge, Fristen? |
| Q-E36 | Welcher Fall bekommt welche Schutzstufe (S0 bis S3)? |
| Q-E37 | Wird der Spieler vor einem Zeitsprung gewarnt, wenn Fälle verfallen könnten? |
| Q-E60 | Kann ein Zeitsprung durch ein wichtiges Ereignis unterbrochen werden? |

### 5.9 Beweise, Forensik, Zeugen (Etappe 2)

| ID | Frage |
|---|---|
| Q-E38 | Ist erkennbar, wenn ein Spieler richtig, aber ohne Begründung gehandelt hat (Glück)? |
| Q-E39 | Wer setzt den Wissensstatus einer Behauptung: Spiel, Spieler oder beide (V-A, V-B, V-C)? |
| Q-E40 | Hebt das Journal Widersprüche zwischen Belegen hervor, oder führt es sie nur nebeneinander? |
| Q-E41 | Gibt es einen ausdrücklichen „Fall abschließen“-Befehl oder ergibt sich der Abschluss aus Handlungen? |
| Q-E42 | Darf der Spieler Beweise vor Gericht einbringen, und wie wird die Beweiskette geprüft? |
| Q-E43 | Sind Sprache und Schrift (alte Dokumente) ein Hindernis? |
| Q-E44 | Wie lange dauern Analysen, wie sicher sind Ergebnisse, wie schnell verderben Proben? |
| Q-E45 | Wie sicher und wiederholbar sind Experimente an Proben und Wesen? |
| Q-E46 | Welche Referenzdaten über den Spieler gibt es und wann entstehen sie? |
| Q-E47 | Sind Autopsie und Tatortarbeit spielbar oder Dokumente? |
| Q-E48 | Gilt für Zivilisten als Zeugen dasselbe Wahrnehmungsmodell wie für Gegner? |
| Q-E49 | Welche Befragungsabsichten gibt es, und wie wird Druck behandelt? |
| Q-E50 | Wie wird bei der Materialisierung eines Alibis die Konsistenz mit Bekanntem gesichert? |
| Q-E51 | Gibt es Zwangsmittel beim Verhör von Wesen? |

### 5.10 Identität, Recht, Haft (Etappe 2)

| ID | Frage |
|---|---|
| Q-E52 | Welche Fähigkeitsstufen haben Formwandler, und wie sind sie erkennbar? |
| Q-E53 | Arbeitet der Spieler als Undercover-Informant der Polizei, und wie? |
| Q-E54 | Gibt es Tarnidentitäten im Basisspiel? |
| Q-E55 | Ist Kopfgeld eine Strafzahlung, ein Kopfgeldjäger-System oder beides? |
| Q-E56 | Wie wird Korruption aufgedeckt? |
| Q-E57 | Sind rechtswidrig erlangte Beweise vor Gericht unzulässig? |
| Q-E58 | Was geschieht bei lebenslanger Haft des Spielers? |
| Q-E59 | Gibt es Geschworene? |
| Q-E61 | Wie viele Haftanstalten gibt es, und welche Arten? |

### 5.11 Geheimhaltung, Koop, Persistenz (Etappe 2)

| ID | Frage |
|---|---|
| Q-E62 | Wie schnell und wie stark ändert sich öffentliches Wissen? |
| Q-E63 | Kann das Übernatürliche vollständig öffentlich werden? |
| Q-E64 | Fließt Wissen im Koop automatisch in die Teamakte oder teilt es der Spieler aktiv? |
| Q-E65 | Welche Entscheidungen sind persönlich, welche gemeinsam? |
| Q-E66 | Wie behandelt das Recht Komplizenschaft und Aussagen gegen Mitspieler? |
| Q-E67 | Wo liegen Notizen, Beweisgraph, Journaltexte und NPC-Überzeugungen, wenn nicht im Weltzustand? |
| Q-E68 | Welche Namens- und Längenregeln gelten für Fall-, Beleg- und Aktenkennungen? |
| Q-E69 | Gibt es Transaktionsgrenzen im Journal? |
| Q-E70 | Wie wird Zufall im Zeitsprung reproduzierbar? |
| Q-E71 | Welche Historie bleibt bei einer Journal-Verdichtung erhalten, und gehört der Fallbericht dazu? |

### 5.12 Detektivpfad, Kriminalität, Ereignisse, Medien (Etappe 3)

| ID | Frage |
|---|---|
| Q-E72 | Welche Voraussetzungen hat die Privatdetektiv-Lizenz, und ist sie für bestimmte Handlungen zwingend? |
| Q-E73 | Welche rechtlichen Grenzen gelten für Observation, Abhören, Register und Täuschung? |
| Q-E74 | Wie oft entstehen NPC-Verbrechen, und wie wird die Rate bestimmt? |
| Q-E75 | Welche Häufigkeit, Dauer und Reichweite haben Ereignisse je Stufe? |
| Q-E76 | Hat der Spieler Werkzeuge für Bild- und Tonanalyse zur Echtheitsprüfung? |
| Q-E77 | Wird Handyortung gespielt, und welche Spuren des Spielers entstehen im Netz? |

## 6. Prüfung der offenen Fragen gegen die Quellen

Stand der Prüfung (Etappe 4): Für jede der 77 Fragen wurde geprüft, ob die systematisch ausgewerteten Quellen (Chat 23 Nachrichten 66–1055 über Fragenblöcke, Antworten und Stichwortsuche; Chat 35 Stichwortsuche und die Crime-Runden; Chat 48 Quiz; `M-OD`) sie beantworten. Ergebnis: **keine** Frage ist durch eine bisher ungelesene Quelle vollständig beantwortet; **zwölf** sind teilweise beantwortet. Die Prüfung belegt keinen Nichtbefund für ungelesene Quellen (siehe [Modul 22](22-Abdeckungsprotokoll.md)).

### 6.1 Teilweise beantwortet

| Frage | Fund | Reste |
|---|---|---|
| Q-E06 Tod des Spielers | acht Ansätze mit Reaktionen (`23#87A`, `23#88E`), `23#362E`, Oktoberstand | Ablauf offen; K-E12 |
| Q-E08 Karte im Detektivmodus | Begleit-Karte zeigt nur den Spielstand (`23#691E`); eigene Marker erlaubt (`23#55A`) | Karte im Spiel |
| Q-E10 Fehlschlag/Neuladen | klassischer Reload ist über die Einstellungen schaltbar (`23#88E` Ansatz A) | Bindung an Schwierigkeit |
| Q-E20 Straftatenkatalog | keine künstliche Liste, nicht beworben (`35#530E`, `35#531A`) | Katalog, Schwellen, Strafen |
| Q-E21 Zuständigkeitskarte | Rechtslage je Bundesstaat verschieden, 5–10 Staaten, Planungsstand 7 (`23#719E`, `23#721E`, `23#729E`) | Zuständigkeitsgrenzen, Kartenlage |
| Q-E24 / K-E4 Prägung der Region | Weltlogik-Lock (`35#536E`–`35#538E`) | Stärke der Mehrheitskultur |
| Q-E25 Polizeikarte je Schwierigkeit | Normal: ungefähre Gebiete; Hardcore: keine Positionen (`23#612A` Punkt 15, 💡) | Festlegung |
| Q-E28 Entstehung dynamischer Aufträge | Generator-Struktur (`23#622A`, 💡) | Rate, Schema (`M-OD`) |
| Q-E30 ignorierte Fälle, Altfälle | Zeitfenster vernichten nichts (`23#308E` Punkt 22) | Dauer, Altfallregeln |
| Q-E33 Hilfe und Medaillen | Medaillenziele missionsabhängig, Ermittlungsleistungen möglich (`23#587A`) | Einfluss der Hilfestufe |
| Q-E55 Kopfgeld | Lage geklärt, K-E13 | Entscheidung |
| Q-E61 Zahl der Haftanstalten | Art: ein größeres Gefängnis plus kleinere Jails (`23#1023E` Punkt 40) | Zahl, Orte |

### 6.2 Weiterhin offen, mit neuem Befund

| Frage | Befund |
|---|---|
| Q-E39 Wer setzt den Wissensstatus | keine Quelle; `23#307A` Punkt 24 und `M-PD` nennen Statuswörter ohne Zuständigkeit |
| Q-E67 / Q-E71 Persistenz, Verdichtung | keine Quelle; `23#1024A` nennt, dass der Weltzustand Entwicklungszustände speichert (Modul 19) |
| Q-E09 Begleiter und Hinweise | `M-PD`: Begleiter nicht allwissend; keine Regel |
| Q-E36, Q-E37 Schutzstufen, Warnung | `23#583A` stützt ‚nie verpassbar‘; Stufen offen |
| Q-E41 Fallabschluss | keine Quelle |
| Q-E53 Spieler als Informant | `23#678A` (Spitzel für Polizei, FBI oder eigene Motivation, 💡) und `23#677E`: ‚man kann theoretisch ein spitzel sein‘; Polizeibezug offen |
| Q-E72 Lizenz | keine Quelle |
| alle übrigen | kein Befund |

Die Fragen Q-E01 bis Q-E77 bleiben als Liste unverändert; ihre Nummern werden nicht umgeordnet.
