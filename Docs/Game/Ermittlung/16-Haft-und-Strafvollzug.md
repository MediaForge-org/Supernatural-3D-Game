# Ermittlung 16: Haft und Strafvollzug

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt die Entscheidungen des Quiz vom 11.10.2026 (`48#`) zu Haft, Zeitsprung, Koop-Verhaftung, Ausbrüchen und übernatürlicher Gefängnissicherheit und führt sie mit älteren Aussagen zusammen. Die Phasenzuordnung `[VS]`/`[EARLY]`/`[LATER]`/`[FUTURE]` in den Quelltexten stammt vom Assistenten und ist ein Vorschlag. Zustände und Abläufe sind **Entwurf** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Vorgänger: [Modul 15](15-Gerichte-und-Verfahren.md). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Die Entscheidungen im Quiz](#1-die-entscheidungen-im-quiz)
2. [Ältere Aussagen](#2-ältere-aussagen)
3. [Entwurf: Haftmodi für den Spieler](#3-entwurf-haftmodi-für-den-spieler)
4. [Entwurf: Zustandsmaschine der Haft](#4-entwurf-zustandsmaschine-der-haft)
5. [Entwurf: Die Welt während der Haft](#5-entwurf-die-welt-während-der-haft)
6. [Entwurf: Missionsschutz](#6-entwurf-missionsschutz)
7. [Entwurf: Gefängnisalltag und soziale Simulation](#7-entwurf-gefängnisalltag-und-soziale-simulation)
8. [Entwurf: Besitz, Körper und Beziehungen während der Haft](#8-entwurf-besitz-körper-und-beziehungen-während-der-haft)
9. [Entwurf: Ermittlung aus dem Gefängnis](#9-entwurf-ermittlung-aus-dem-gefängnis)
10. [Entwurf: Ausbruch](#10-entwurf-ausbruch)
11. [Entwurf: Übernatürliche Gefangene und Sicherheit](#11-entwurf-übernatürliche-gefangene-und-sicherheit)
12. [Koop](#12-koop)
13. [Phasen und Aufwand](#13-phasen-und-aufwand)
14. [Sonderfälle](#14-sonderfälle)
15. [Abhängigkeiten](#15-abhängigkeiten)
16. [Ergänzungen aus der Quellenauswertung](#16-ergänzungen-aus-der-quellenauswertung)
17. [Was fehlt](#17-was-fehlt)

---

## 1. Die Entscheidungen im Quiz

### 1.1 Gefängnissystem

Frage `48#156A`: „Was soll passieren, wenn der Hunter ins Gefängnis kommt?“

| Option | Inhalt |
|---|---|
| A | Zeitsprung: Haft wird übersprungen; danach geht das Spiel mit Konsequenzen weiter |
| B | Kurze spielbare Haft: Verhöre, Kontakte und einzelne Ausbruchsmöglichkeiten, danach optionaler Zeitsprung |
| C | Vollständig spielbares Gefängnis: Alltag, Insassen, Beziehungen, Aufgaben, Ausbrüche, rechtliche Möglichkeiten |
| D | Kombination mit Spielerfreiheit: vollständig spielbare Haft mit Ermittlungen, Ausbrüchen, Anwälten und NPC-Interaktionen, aber jederzeit Möglichkeiten zur Zeitbeschleunigung oder zum Überspringen längerer Haftabschnitte |

Antwort `48#157E`: „D klingt gut aber ist wenn es nicht in der story gescripted ist schwer umzusetzen“.

| Aussage | Status |
|---|:---:|
| **D ist das Ziel** („klingt gut“) | 📋 |
| Umsetzbarkeit ohne Skript in der Story ist schwierig; das ist ein Vorbehalt des Entwicklers, kein Verzicht | 📋 (Vorbehalt) |
| Der Assistent bestätigte D als **langfristiges Ziel** mit hybridem Aufbau (wiederverwendbare Systeme statt Missionen für jede Verhaftung) | 💡 (`48#158A`) |

### 1.2 Welt während der Haft

Frage `48#158A`: „Wie soll sich die Spielwelt während einer längeren Haftstrafe verhalten?“ Optionen: A (Welt pausiert), B (begrenzter Fortschritt), C (Welt entwickelt sich weiter, bei Zeitsprüngen zusammengefasst simuliert), D (vollständige Simulation mit Eingriffsmöglichkeiten über Kontakte, Briefe, Anwälte, Mitspieler).

Antwort `48#159E`: „C ja D ist longterm“.

| Aussage | Status |
|---|:---:|
| **C** ist festes System: Monster, NPCs und Organisationen handeln auch während der Haft; bei Zeitsprüngen zusammengefasst simuliert | 📋 |
| **D** (begrenzter Einfluss von der Haft aus) ist **langfristig** | 📋 (Langzeit) |

### 1.3 Koop-Verhaftung und Zeitsprung

| Frage | Antwort | Inhalt |
|---|---|---|
| `48#160A` Wie soll eine Verhaftung im Koop funktionieren? (A gemeinsam, B individuell, C individuell mit Rettungsmöglichkeiten, D vollständig dynamisch) | `48#161E`: ‚der spieler ist halt im häfn aber man kann die zeit vorspulen und die welt wird halt dann weiter laufen, im singleplayer auch so aber wenn man dort nebenmissionen hat dürfen die nicht weggehen‘ | 📋 Individuelle Haft; Weltzeit läuft weiter; Nebenmissionen bleiben |
| `48#162A` Wie überspringt ein Inhaftierter die Haft, während andere aktiv sind? (A gemeinsamer Zeitsprung mit Zustimmung, B individueller Skip, C Kombination, D eigene Lösung) | `48#163E`: „Gemeinsamer Welt-Zeitsprung mit Zustimmung aller“ | 📋 A: Option A gewählt |
| `48#164A` Wie umfangreich ist das Gameplay in der Koop-Haft? (A Wartephase, B Aktivitäten, C eigenständiges Gefängnis-Gameplay, D maximale Freiheit) | `48#165E`: „C und d longterm“ | 📋 C als Plan, D langfristig |
| `48#166A` Wie frei sind Gefängnisausbrüche? (A gescriptet, B mehrere vorbereitete Lösungen, C systemisch, D Vollsandbox) | `48#167E`: „Systemische Ausbrüche mit Sicherheitsroutinen, Werkzeugen und Beziehungen“ | 📋 C |
| `48#168A` Wie gehen Gefängnisse mit übernatürlichen Gefangenen um? (A gewöhnlich, B einzelne Spezialgefängnisse, C Sicherheitsstufen nebeneinander, D dynamisches Sicherheitssystem) | `48#169E`: „Dynamische Anpassung von Gefängnissicherheit an übernatürliche Bedrohungen“ | 📋 D |

Der Assistent hat die Folgen ausformuliert (`48#162A`, `48#164A`, `48#166A`, `48#168A`, `48#170A`); diese Ausformulierungen (Tabellen, Beispiele, Phasenzuordnungen) sind 💡.

---

## 2. Ältere Aussagen

| Aussage | Status | Quelle |
|---|:---:|---|
| Festnahme ist kein Game Over; Verhör, Geldstrafe, Kaution, Gericht, Gefängnis, Zeitsprung | 📋 | `23#1058A`; `23#1057E` Punkt 68 |
| Das Gefängnis ist eine eigene soziale Simulation; Insassen können wissen, wofür jemand sitzt; was passiert, hängt von Personal, Sicherheitsstufe, Ruf, Beziehungen, Gangs und Situation ab (kein garantiertes Skript) | 📋 Richtung | `35#533A`; Gameplay-Systeme §10 |
| Schwerer Fahndungsstatus bleibt bis zur glaubwürdigen Auflösung | 📋 | Punkt 65 |
| Hospital im Koop: nicht „drei Monate weg“; Zeit vergeht schneller, Heilung schneller; „nur paar minuten bis 15min“; klassische Variante einstellbar | 📋 | `48#117E` |
| Krankenhaus im Koop überspringen, sonst Abbruch („ragequitted man nach 10 min“) | 📋 | `48#119E` |
| Hunter-Bunker und Verwahrung von Gefangenen sind ein eigenes Planungsthema | 📋 | `48#131E` |

---

## 3. Entwurf: Haftmodi für den Spieler

Das Ziel D (`48#157E`) verlangt alle Stufen, aber **nacheinander** ausbaubar (Phasen: Abschnitt 13). Ein Entwurf, welche **Modi** der Spieler in der Haft hat:

| Modus (Arbeitsname) | Inhalt | Entspricht Option |
|---|---|---|
| **Überspringen** | Haft wird mit Zeitsprung abgewickelt; Folgen werden eingeblendet (Welt hat sich verändert) | A |
| **Verkürzt spielbar** | Verhöre, Kontakte, einzelne Entscheidungen, optionaler Zeitsprung | B |
| **Vollständig spielbar** | Alltag, Insassen, Beziehungen, Aufgaben, Ausbruch, Rechtsmittel | C |
| **Kombination (Ziel)** | jederzeit zwischen den Modi wechselbar; Zeitbeschleunigung | D |

Die Möglichkeit, **jederzeit** zu beschleunigen oder zu überspringen (D), ist der Kern. Der Spieler soll nie in einer Haft ‚feststecken‘ (G11 in Modul 15). Im Koop gilt für einen **Welt**-Zeitsprung Zustimmung aller (`48#163E`); ein **individueller** Skip der eigenen Warteanimation ist davon zu unterscheiden (`48#164A`: „Das Überspringen einer Warteanimation ist nicht dasselbe wie das Vorspulen der gesamten Weltzeit.“, 💡 Auslegung).

---

## 4. Entwurf: Zustandsmaschine der Haft

> [!NOTE]
> **Nur Vorschlag (💡).** Zustandsnamen sind Arbeitsnamen.

```text
[Freiheit] -> [Festnahme] -> [Polizeigewahrsam] -> [Untersuchungshaft/Kaution]
                                      |                      |
                                  (freigelassen)       [Strafhaft] -> [Bewährung/Entlassung]
                                                          |  \
                                                          |   [Überstellung]
                                                          |   [Sonderhaft]
                                                          v
                                                      [Ausbruch] -> [Flucht/Fahndung]
                                                          |
                                                      [Tod in Haft]
```

| Zustand | Beschreibung | Spielerrolle |
|---|---|---|
| **Polizeigewahrsam** | Zelle bei der Polizei; Verhör; kurzer Zeitraum | warten, kooperieren, Kontakt |
| **Untersuchungshaft/Kaution** | bis zum Verfahren; bei Kaution entlassen | Anwalt, Zeugen, Ermittlung |
| **Strafhaft** | Vollzug des Urteils in einer Einrichtung | Alltag oder Skip |
| **Überstellung** | Wechsel der Einrichtung (Sicherheitsstufe, Zuständigkeit) | Ereignis |
| **Sonderhaft** | gesicherter Bereich für besondere Gefangene (zum Beispiel übernatürliche Bedrohung) | Abschnitt 11 |
| **Bewährung/Entlassung** | Auflagen, Meldepflicht; Rehabilitierung (Modul 15 §9) | Auflagen beachten |
| **Ausbruch** | Flucht aus der Haft | Abschnitt 10 |
| **Tod in Haft** | Gewalt, Krankheit, Unfall | Konsequenz (offen) |

### 4.1 Übergänge (Auswahl)

| Von | Nach | Auslöser | Bedingung |
|---|---|---|---|
| Festnahme | Polizeigewahrsam | Polizei | Person festgenommen |
| Polizeigewahrsam | Untersuchungshaft | Gericht | Anklage; keine Kaution |
| Untersuchungshaft | Strafhaft | Gericht | Urteil mit Haft |
| Strafhaft | Entlassung | Zeit/Behörde | Haftzeit abgelaufen, Begnadigung, Berufung |
| Strafhaft | Überstellung | Behörde | Sicherheitslage, Verlegung |
| beliebig | Ausbruch | Spieler | Plan erfolgreich |
| Ausbruch | Flucht/Fahndung | Welt | Flucht bemerkt; Fahndung steigt, Sicherheit verschärft (`48#168A`: „Flucht löst neue Fahndungen, Ermittlungen und möglicherweise strengere Sicherheitsmaßnahmen aus“, 💡) |
| beliebig | Skip | Spieler | Zeitsprung (Einzel oder, im Koop, mit Zustimmung) |

---

## 5. Entwurf: Die Welt während der Haft

### 5.1 Was beschlossen ist (C)

Monster, NPCs und Organisationen handeln während der Haft weiter (`48#159E`). Die Tabelle in `48#160A` (💡 Ausformulierung der Entscheidung) nennt:

| Bereich | Mögliche Entwicklung während der Haft |
|---|---|
| Monsterterritorien | Rudel und Organisationen expandieren, verlieren Gebiete, tragen Konflikte aus |
| NPCs | Beziehungen, Aufenthaltsorte, persönliche Entwicklungen ändern sich |
| Hunter-Gemeinschaft | andere Hunter übernehmen Fälle oder reagieren auf die Abwesenheit |
| Polizei und Justiz | Ermittlungen und Verfahren laufen weiter |
| Gesellschaft | Nachrichten, Gerüchte, regionale Krisen |
| Zeitübersprung | lange Zeiträume werden effizient zusammengefasst simuliert |

### 5.2 Die Rechenvorgabe

`48#160A` (💡 des Assistenten): Ein Zeitsprung von sechs Monaten darf nicht sechs Monate Echtzeitsimulation ausführen; die relevanten Entwicklungen werden **in größeren Simulationsschritten** berechnet und die Ergebnisse gespeichert. Das stimmt mit der Abstufung der NPC-Simulation überein (Tier A bis D, `23#1056A` Frage 121; Hintergrundsimulation Fragen 122–125). Für die Ermittlung heißt das:

| Folgerung | Erläuterung |
|---|---|
| **Fortschreibungsfunktion** | Jeder Weltzustandsbestandteil (Akten, Fälle, Eskalationsuhren, NPC-Zustände, Territorien) braucht eine deterministische Funktion ‚Zustand zu Zeit A, Zielzeit B‘ |
| **Ereignisse bleiben nachvollziehbar** | Was während des Sprungs geschah, muss dem Spieler danach erklärbar sein (Journal, Nachrichten, Gespräche), sonst entsteht ein unverständlicher Weltzustand |
| **Grobheit** | Der Sprung simuliert Ereignisse in Schritten, nicht jede Bewegung; Verluste (zum Beispiel NPC-Tod) sind echte Weltzustände |
| **Schutz** | Missionsschutz (Modul 09 §9) und Schutz wichtiger NPCs (Punkt 105) gelten im Sprung |
| **Unterbrechung** | Ein Sprung kann unterbrochen werden, wenn ein wichtiges Ereignis geschieht (offen, Q-E60) |

### 5.3 D langfristig

D (Einfluss aus der Haft über Kontakte, Briefe, Anwälte, Mitspieler) ist Langzeit. Beispiele (Assistent, `48#160A`): Kontakte, Anwälte, Briefe und Mitspieler.

---

## 6. Entwurf: Missionsschutz

Beschlossen (`48#161E`, `48#163E`): Nebenmissionen dürfen nicht allein durch einen Haft-Zeitsprung dauerhaft verschwinden. Die Ausformulierung des Assistenten (`48#162A`): Fortbestand der Mission bleibt, der Ablauf kann sich ändern; für besonders wichtige Nebenmissionen gibt es geschützte Storyzustände.

Die Stufenfassung steht in [Modul 09 §9](09-Falllebenszyklus-und-Generierung.md#9-entwurf-zeitsprünge-und-missionsschutz). Zusätzlich für die Haft (Entwurf):

| Frage | Vorschlag |
|---|---|
| Was sieht der Spieler nach der Haft? | Ein **Bericht** über Veränderungen, soweit er sie erfahren darf (Journal, Gespräche). Unbekanntes bleibt unbekannt. |
| Dürfen Fälle **von anderen gelöst** werden? | Ja (L9); sie sind dann ‚Fremd bearbeitet‘ oder „Gelöst“; geschützte Fälle nicht. |
| Verfallen **Fristen** von Kundenaufträgen? | Möglich; der Spieler wird informiert (Q-E37). |
| Verfallen **dynamische Aufträge**? | Ja, wenn nicht geschützt (S0). |
| Bleiben **Spuren** erhalten? | Nein, sie verfallen in der Welt; das Wissen bleibt. |

---

## 7. Entwurf: Gefängnisalltag und soziale Simulation

### 7.1 Beschlossen

Das Gefängnis ist mehr als ein Wartebildschirm (`48#166A` zur Koop-Haft; Plan C). Funktionen des geplanten Gefängnis-Gameplays (Tabelle in `48#166A`, 💡): Gespräche mit Insassen und Wärtern, Beziehungen, Konflikte und Routinen, eigene Ermittlungen, Ausbrüche vorbereiten und durchführen, Befreiung durch andere Hunter (alle `[LATER]` laut Assistent), dynamische Geschichten und komplexe Innen-/Außenoperationen (`[FUTURE]`).

Aus `35#533A` (Assistent, Richtung vom Entwickler vorgegeben `35#532E`): Insassen können wissen, wofür jemand sitzt; besonders verachtete Delikte können Isolation, Erpressung, Drohungen, Prügeleien, Schutzsuche und Gangkonflikte auslösen; kein garantiertes Skript.

### 7.2 Entwurf: Bausteine

| Baustein | Inhalt |
|---|---|
| **Routine** | Tagesablauf (Wecken, Zählung, Mahlzeiten, Hofgang, Arbeit, Einschluss); Schichtwechsel der Wärter (`48#168A`) |
| **Personal** | Wärter mit Persönlichkeit, Beziehungen, Bestechlichkeit; Leitung |
| **Insassen** | NPCs (Tier A/B/C) mit Gedächtnis, Gruppen (Gangs), Ruf |
| **Wissen im Gefängnis** | Insassen wissen, wofür jemand sitzt (35#533A); Gerüchte (Modul 12 §12) |
| **Aufgaben** | Arbeit, Programme, Dienste, Gefälligkeiten |
| **Konflikte** | Konflikte zwischen Gruppen, Auseinandersetzungen, Schutzsuche |
| **Besuch und Post** | Anwälte, Familie, Hunter-Kontakte; Briefe (Langzeit D) |
| **Gesundheit** | Verletzungen, Krankheit, Krankenstation (Medizin) |
| **Sicherheitsstufen** | verschiedene Einrichtungen (Abschnitt 4: Überstellung) |
| **Zugang zu Gegenständen** | Beschaffen und Zweckentfremden (`48#168A`), Kontrabande |

### 7.3 Einrichtungen

Die Quelle spricht von ‚einigen detaillierten, spielbaren Einrichtungen statt Hunderter einzigartiger Gefängnisse‘ (`48#158A`, 💡) und von ‚wiederverwendbaren Zellen, Verhören und Freilassungsbedingungen‘ im Polizeigewahrsam. Das ist ein Vorschlag zur **Produktionsökonomie**. Die Zahl der Haftanstalten in der Welt ist nicht festgelegt (❓ Q-E61).

---

## 8. Entwurf: Besitz, Körper und Beziehungen während der Haft

Die Quellen beantworten diese Fragen nicht. Sie sind **Folgen** der beschlossenen Weltsimulation (Abschnitt 5) und der beschlossenen Körper- und Besitzsysteme.

| Bereich | Frage | Vorschlag |
|---|---|---|
| **Persönlicher Besitz** | Was geschieht mit Waffen, Ausrüstung, Fahrzeug, Geld? | Beschlagnahme, Lagerung, Rückgabe (Modul 14 §8). Geld im Auto, Safe, HQ, Bank geht nicht automatisch verloren (`M-PD`). Fahrzeuge und Immobilien: offen (Steuern, Miete, Verfall?) |
| **Körper** | Verändern Training, Schlaf, Ernährung, Verletzungen den Körper? | Detaillierte Körpersimulation ist beschlossen (`48#113E`, `48#115E`); in der Haft können Fitness, Gewicht, Narben, Verletzungen weiterlaufen (Entwurf). Mit Skip als Zusammenfassung |
| **Beziehungen draußen** | Was geschieht mit Partnern, Crew, Companions? | Sie handeln weiter (NPC-Entwicklung). Eine lange Haft kann Beziehungen verändern oder beenden (Block 8M Frage 106: Beziehungen persistieren) |
| **Hunter-HQ** | Wer führt das HQ? | Crew handelt; Hunter-Gemeinschaft kann Fälle übernehmen (Modul 09) |
| **Arbeit/Detektivbüro** | Läuft das Büro weiter? | Entwurf: Büro ruht oder wird von Angestellten geführt (Mitarbeiter sind „deutlich später“, Modul 07) |
| **Geheimnis** | Droht die Enttarnung als Hunter? | Möglich durch Verhöre, Gespräche (Modul 17) |
| **Geld und Schulden** | Anwalt, Kaution, Strafen | Wirtschaft (offen) |

---

## 9. Entwurf: Ermittlung aus dem Gefängnis

Beschlossen als Plan: „Eigene Ermittlungen aus dem Gefängnis“ (`48#166A`, `[LATER]`, 💡) und die Entscheidung `48#165E` (C: eigenständiges Gefängnis-Gameplay mit Ermittlungen). Das Beispiel „Du wirst durch einen Formwandler unschuldig verhaftet“ (`48#158A`, 💡) beschreibt die Richtung: Ein Hunter-Kontakt sucht draußen den Formwandler, der Inhaftierte sammelt Hinweise.

| Möglichkeit | Entwurf |
|---|---|
| **Gespräche** | Mit Insassen und Wärtern; Informationsnetz (Gerüchte) |
| **Besuch** | Anwalt und Kontakte bringen Beweise und Informationen |
| **Unterlagen** | Akten und Berichte über den Anwalt |
| **Zeugen** | Inhaftierte können Zeugen sein (Insassen, die etwas wissen) |
| **Koop** | Mitspieler ermitteln draußen, Austausch über Besuch oder Post |
| **Grenzen** | Eingeschränkte Reichweite, Zeitverzögerung, Überwachung der Post |

Die Haft ist also ein **Ermittlungsort** mit eigenen Quellen und Einschränkungen. Der Inhaftierte hat Zugriff auf andere Wissensquellen als der freie Hunter; sein Wissensstand wächst entsprechend (Modul 10).

---

## 10. Entwurf: Ausbruch

### 10.1 Beschlossen (Option C)

Systemische Ausbrüche mit Sicherheitsroutinen, Werkzeugen und Beziehungen (`48#167E`). Die Ausformulierung (`48#168A`, 💡): Sicherheitsroutinen (Schichtwechsel, Patrouillen), Zugangskontrollen (Türen, Schlüssel, Ausweise, Sicherheitsbereiche), Werkzeuge (beschaffen, kombinieren, zweckentfremden), Beziehungen (helfen oder verraten), Schleichen (Sichtlinien, Geräusche, Kameras, Alarm), Hunter-Fähigkeiten, Koop-Unterstützung von außen, Konsequenzen (neue Fahndung, strengere Sicherheit).

**Nicht gewählt:** gescriptete Ausbruchsmissionen (A), nur mehrere vorbereitete Lösungen (B) und die Vollsandbox mit baulichen Veränderungen, langfristigen Tunnelprojekten, Gefangenenaufständen (D). Der Entwickler hat C gewählt.

### 10.2 Entwurf: Ausbruchsphasen

```text
Beobachten -> Beschaffen -> Planen -> Vorbereiten -> Ausführen -> Flucht -> Folgen
```

| Phase | Inhalt |
|---|---|
| **Beobachten** | Routinen, Schichten, Kameras, Zugänge erkennen (Wahrnehmungssystem) |
| **Beschaffen** | Gegenstände, Zugänge, Kontakte |
| **Planen** | Der Spieler wählt einen Plan (Beispiele aus `48#168A`: unauffällige Flucht, Beziehungen ausnutzen, Hilfe durch andere Hunter) |
| **Vorbereiten** | Ablenkung, Absprachen, Zeitpunkt |
| **Ausführen** | Schleichen, Überwinden, Kämpfen, Überreden |
| **Flucht** | Außerhalb: Fahndung, Verfolgung (Modul 14 §12) |
| **Folgen** | Verschärfte Sicherheit, Verfolgung, Gefährdung von Helfern, Rufwirkung |

### 10.3 Moralische und rechtliche Seite

Ein unschuldig Verurteilter, der ausbricht, wird zum Flüchtling (Fahndung steigt); ein schuldiger, der ausbricht, ebenso. Das Spiel unterscheidet das **in der Wirklichkeit (E1)** nicht für die Behörde (E4). Die Ausbruchsentscheidung ist daher ein Spiel-Dilemma (`48#158A`: Entlastung abwarten, Ermittlungen unterstützen oder Ausbruch).

---

## 11. Entwurf: Übernatürliche Gefangene und Sicherheit

### 11.1 Beschlossen (Option D)

„Dynamische Anpassung von Gefängnissicherheit an übernatürliche Bedrohungen“ (`48#169E`). Der Assistent (`48#170A`, 💡): Gefängnisse haben kein automatisch perfektes Wissen über Monster; Sicherheitsmaßnahmen hängen von dem ab, was die Verantwortlichen wissen, welche Ressourcen sie besitzen und welche Vorfälle passiert sind.

Entwicklungsstufe (Tabelle `48#170A`, 💡): gewöhnliches Gefängnis; erste Verdachtsfälle (zusätzliche Überwachung, medizinische Untersuchungen, Sonderermittlungen); bestätigte Monsteraktivität (artspezifische Maßnahmen, geschulte Kräfte, besondere Haftbereiche); spezialisierte Einrichtung (Schutzsymbole, Rituale, besondere Materialien, ausgebildetes Personal); wiederholte Zwischenfälle (Anpassung).

Die Einordnung der Produktion: dynamische Erweiterung von Haftanstalten `[LATER]`, in der tiefsten Form `[FUTURE/AI]` (`48#170A`, Vorschlag).

### 11.2 Entwurf: Verbindung zu Wissensstufen

Die Wissensstufen einer Behörde (Modul 14 §11) gelten für Gefängnisse:

| Wissensstufe | Haft |
|---|---|
| 0 | behandelt Gefangene als Menschen; ein Wesen kann unbemerkt einsitzen oder entkommen |
| 1 | Verdacht, zusätzliche Maßnahmen |
| 2–3 | artspezifische Maßnahmen, Spezialabteilung |
| 4 | institutionelle Einrichtungen (Modul 17) |

Wirksamkeit richtet sich nach der Art: „Eine Teufelsfalle ist beispielsweise nicht automatisch gegen jedes übernatürliche Wesen wirksam“ (`48#170A`). Es gibt keine universelle Zelle (`48#130A`).

### 11.3 Für den Spieler

| Situation | Möglichkeiten |
|---|---|
| Spieler sitzt neben einem Wesen | Beobachten, Informationen, Gefahr |
| Spieler befreit ein Wesen | Verbrechen, Folgen (Fahndung, Verhältnis zum Wesen) |
| Spieler bringt ein gefangenes Wesen in ein Gefängnis | unüblich; Verwahrung beim Hunter (`48#129E`–`48#132A`: beliebige Orte, Bunker eigenes Thema) |
| Spieler bricht aus einem übernatürlich gesicherten Bereich aus | Schwerer als gewöhnlicher Ausbruch |

---

## 12. Koop

| Aussage | Status | Quelle |
|---|:---:|---|
| Nur der betroffene Hunter sitzt ein; die anderen können weiterspielen | 📋 (Einzelhaft, Inhalt von `48#161E`) | `48#161E`, bestätigt `48#162A` |
| Ein echter Welt-Zeitsprung benötigt die Zustimmung aller aktiven Mitspieler | 📋 | `48#163E` |
| Bei Ablehnung läuft die Weltzeit normal weiter; die Mitspieler spielen weiter | 💡 (Ausformulierung `48#164A`) | – |
| Der Verhaftete kann einen Zeitsprung vorschlagen | 💡 | `48#164A` |
| Nebenmissionen dürfen durch den Sprung nicht dauerhaft verloren gehen | 📋 | `48#161E` |
| Plan C: Gefängnis-Gameplay mit Ermittlungen, Beziehungen, Ausbruchsplanung, Befreiung durch Mitspieler; D langfristig | 📋 | `48#165E` |
| Charaktere gehören zum Host-Spielstand; jeder Spieler erstellt seinen Hunter | 📋 | `48#109E` |

Für die Ermittlung: Mitspieler draußen können Beweise sammeln, einen Anwalt organisieren oder eine Befreiung versuchen (Option C der Frage `48#160A`; die Wahl `48#161E` nennt diese Optionen nicht ausdrücklich, der Assistent bestätigte sie `48#162A` als Teil des Plans). Das erzeugt eine **Aufgabenteilung** (Innen/Außen), die das Koop-Modell der geteilten Akten (Modul 18) tragen muss.

---

## 13. Phasen und Aufwand

Der Assistent ordnete die Produktion zu (`48#158A`, 💡 Vorschlag; der Entwickler stimmte dem Aufbau über `48#159E` und `48#165E` weitgehend zu: „longterm“ für D):

| Phase | Inhalt (Vorschlag) |
|---|---|
| `[VS]` | einfache Festnahme mit Konsequenzen |
| `[EARLY]` | Gewahrsam, Verhöre, rechtliche Entscheidungen |
| `[LATER]` | spielbare Haftanstalten und wiederverwendbare Gefängnismechaniken; Gespräche, Konflikte, Ermittlungen, Ausbruch; dynamische Sicherheitsanpassung |
| `[FUTURE]` | dynamische Gefängnisgesellschaft, komplexe Ausbrüche, tiefere Justizsimulation; komplexe Koop-Operationen |

Für den **Vertical Slice** wird nach dieser Zuordnung kein vollständiges Gefängnis benötigt (`48#168A`: „Für den Vertical Slice benötigen wir noch kein vollständiges Gefängnis“). Das passt zur Mindestfassung in Modul 15 §8.

---

## 14. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| **Tod in Haft** | Gewalt, Krankheit | Der Tod folgt den allgemeinen Todesregeln (offen; `M-B09`); in der Haft liegt die Konsequenz bei Insassen und Personal |
| **Spieler hat schwere Verletzungen** | – | Krankenstation; Zeit; Genesung (Modul 09 §9) |
| **Mehrfach inhaftiert** | – | Vorstrafen steigern die Strafe; Gewöhnung (Beziehungen im Gefängnis) |
| **Besucher werden bedroht** | – | Anwalt/Angehörige als Druckmittel |
| **Rache für frühere Taten** | Insasse kennt den Spieler | NPC-Gedächtnis (Z1), `48#139E` für Wesen |
| **Spieler tötet im Gefängnis** | – | Strafverfahren; Verlängerung; Sonderhaft |
| **Aufstand/Unruhe** | – | Ereignis der Welt (mittel/groß); nicht beschlossen |
| **Gefängnis als Falltatort** | Fall spielt im Gefängnis | Zugang eingeschränkt; Modul 09 |
| **Wesen als Wärter oder Insasse** | – | übernatürliche Tarnung (Modul 13) |
| **Hunter in Haft, Fall dringend** | – | Missionsschutz (Modul 09 §9); andere Hunter übernehmen |
| **Kind in Haft** | – | nicht in den Quellen |
| **Admin-/Entwicklerbuild** | Crime-Pfade für Tests | Nur interne Builds (`35#636A`): Konsequenzen bis Prison werden getestet; nicht in der Veröffentlichung |

---

## 15. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Polizei und Gerichte ([Module 14](14-Polizei-Behoerden-und-Fahndung.md), [15](15-Gerichte-und-Verfahren.md)) | Gewahrsam, Verfahren, Vollzug |
| Fallmodell ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) | Missionsschutz, Fremdbearbeitung |
| Körper- und Medizinsystem | Verletzungen, Genesung, Fitnessänderung |
| NPC-Simulation (Tiers) | Insassen, Personal |
| Wahrnehmung und Tarnung (Gameplay-Systeme §7) | Ausbruch |
| Monster-Mythologie | übernatürliche Sicherheit |
| Wirtschaft | Besitz, Kosten |
| Koop ([Modul 18](18-Koop-Ermittlung.md)) | Zeitsprung, Aufgabenteilung |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Haftstatus, Fortschreibung |
| Geheimhaltung ([Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md)) | Wissensstufen, staatliche Einrichtungen |
| Speichern/Laden | Haft mitten im Spielstand; Sprung-Simulation reproduzierbar |

---

## 16. Ergänzungen aus der Quellenauswertung

### Einrichtungen in der Welt

Aus dem Weltplanungs-Block 4 (`23#1022A` Frage 40, `23#1023E` Punkt 40, bestätigt mit „ja sicher“): Es soll **ein größeres Gefängnis und kleinere Jails** geben. Der Blockentwurf nannte außerdem Courthouse, County Jail, größere Justizvollzugsanstalt und eventuell eine Federal/High-Security-Einrichtung (`23#1006A` Frage 14), die der Entwickler dort nicht beantwortete („wir haben das von oben nicht gemacht“, `23#1007E`). Status: Art 📋, Zahl und Standorte ❓. Der Entwurf in §7.3, wenige detaillierte Einrichtungen statt vieler einzigartiger Gefängnisse, ist damit **nicht widersprochen**: Er deckt sich mit einer kleinen Zahl.

### Haft und Festnahme als Alternative zur Flucht

Der Assistent (`23#606A` Punkt 6): Die Spielfigur gibt auf, wird festgenommen, durchsucht, transportiert; danach Kaution, Geldstrafe, Anwalt oder Kontakt, die Crew holt sie ab, bei schweren Fällen größere Folgen; „viel glaubwürdiger als: Polizei erschießt dich wegen jeder Kleinigkeit“. Der Entwickler antwortete darauf nicht gegen den Punkt (`23#607E`) und bestätigte später den Kern (`23#1057E` Punkt 68).

### Die Hauptstory ist kein Gefängnisinhalt

Der Entwickler wollte Firmenjobs nur als Geldquelle und nicht als Haupt- oder Nebenmission (`23#1011E` Punkt 31). Für die Haft folgt daraus nichts Neues; Missionsschutz und Zeitsprungregeln stehen in Abschnitt 6.

---

## 17. Was fehlt
- **Zahl und Art** der Haftanstalten (Q-E61).
- **Haftzeiten** und Zeitumrechnung.
- **Besitz, Körper, Beziehungen** während der Haft (Abschnitt 8): keine Quelle.
- **Unterbrechung** eines Zeitsprungs (Q-E60).
- **Tod in Haft** und Ablauf des Spielertods allgemein (`M-B09`).
- **Aufstände** und Gefängnisereignisse.
- **Briefe, Kontakte und Anwälte** (D, langfristig).
- **Verhältnis** zur Altersfreigabe der Darstellung im Gefängnis (nicht in den Quellen).
