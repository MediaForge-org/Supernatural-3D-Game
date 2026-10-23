# Ermittlung 12: Zeugen, Verdächtige und Verhör

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Menschen (und Wesen) **Wissen über einen Fall haben**, wie sie es weitergeben und wie Befragung und Verhör funktionieren könnten. Beschlossen sind die Grundsätze (kein Allwissen, Gerüchte verzerren, Wiedererkennung, Verhör bei Verdacht). Die Zustände, Absichten und Abläufe sind **Entwurf** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Voraussetzungen: [Modul 10](10-Beweislogik-und-Wissensmodell.md) (Wissensmodell), Gameplay-Systeme §8 (NPC-Gedächtnis). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Von der Wahrnehmung zur Aussage](#2-entwurf-von-der-wahrnehmung-zur-aussage)
3. [Entwurf: Beobachtungseintrag](#3-entwurf-beobachtungseintrag)
4. [Entwurf: Erinnerung, Verfall und Verzerrung](#4-entwurf-erinnerung-verfall-und-verzerrung)
5. [Entwurf: Aussagebereitschaft](#5-entwurf-aussagebereitschaft)
6. [Entwurf: Befragung durch den Spieler](#6-entwurf-befragung-durch-den-spieler)
7. [Entwurf: Verdächtige](#7-entwurf-verdächtige)
8. [Entwurf: Alibi und Tagesabläufe](#8-entwurf-alibi-und-tagesabläufe)
9. [Entwurf: Verhör des Spielers durch Behörden](#9-entwurf-verhör-des-spielers-durch-behörden)
10. [Entwurf: Verhör von Wesen](#10-entwurf-verhör-von-wesen)
11. [Entwurf: Bedrohung, Bestechung, Zeugenschutz](#11-entwurf-bedrohung-bestechung-zeugenschutz)
12. [Entwurf: Gerüchte als Informationsnetz](#12-entwurf-gerüchte-als-informationsnetz)
13. [Dialogumfang und Handschrift](#13-dialogumfang-und-handschrift)
14. [Sonderfälle](#14-sonderfälle)
15. [Abhängigkeiten](#15-abhängigkeiten)
16. [Was fehlt](#16-was-fehlt)

---

## 1. Was beschlossen ist

Alle Nummern in der Spalte „Frage“ beziehen sich auf die nummerierten Fragen des Assistenten in `23#1056A` (Block 8), die der Entwickler in `23#1057E` einzeln beantwortet hat, sofern nicht anders genannt.

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| Z1 | NPCs erinnern sich an relevante Dinge, die der Spieler getan hat (geholfen, bedroht, gerettet, bestohlen, verletzt, belogen) | 📋 | Frage 23, „ja“ |
| Z2 | Erinnerungen können schwächer oder ungenauer werden | 📋 | Frage 24, „ja“ |
| Z3 | NPCs müssen etwas gesehen, gehört oder erzählt bekommen haben, bevor sie davon wissen; sie erfahren auch über Nachrichten und soziale Medien | 📋 | Frage 25, `23#1058A` |
| Z4 | Informationen verbreiten sich über Freunde, Familie, soziale Medien, Nachrichten | 📋 | Frage 26 |
| Z5 | Gerüchte können verzerrt sein; Gerücht ist nicht gleich Wahrheit | 📋 | Frage 27 |
| Z6 | NPCs können Spieler und Fahrzeuge wiedererkennen; Kleidung und Maskierung beeinflussen das | 📋 | Fragen 28–30 |
| Z7 | Der Spieler kann mit praktisch jedem normalen NPC sprechen; nicht jeder bekommt viele Dialogzeilen; wichtige NPCs haben tiefere Dialoge | 📋 | Frage 31: „ja man soll mit jedem npc reden können, nicht sehr viel aber schon“; `23#1058A` |
| Z8 | NPCs unterhalten sich auch untereinander, telefonieren, schreiben Nachrichten, fotografieren | 📋 | Fragen 34, 36 |
| Z9 | Lauschen liefert gelegentlich Informationen | 📋 | Frage 35 (bereits bestätigt) |
| Z10 | Unterschiedliche Persönlichkeit führt zu unterschiedlichen Reaktionen (freundlich, aggressiv, ängstlich, hilfsbereit) | 📋 | Frage 37 |
| Z11 | Betrunkene NPCs verhalten sich anders | 📋 | Frage 38 |
| Z12 | Es gibt ein Tier-System für die Persistenz (A: Story/Hunter/Nebenfall, B: Nachbarn/Händler/häufige Kontakte, C: normale Einwohner mit leichter Persistenz, D: Crowd) | 📋 Richtung | Fragen 17, 121 (Tier-Vorschlag in `23#1056A`) |
| Z13 | Ein Zeuge kann Schweigegeld annehmen, bedroht werden oder durch Mordversuch bedroht sein | 📋 | Frage 48 |
| Z14 | Bei Verdacht kann es zu einem **Verhör** kommen | 📋 | Frage 59, „ja genau, wenn man verdächtigt wird kann es zu einem verhör kommen und so weiter“ |
| Z15 | Gefangene Wesen können **verhört** werden (Kommunikation, Verhandlungen, Kooperation) | 📋 Richtung | Frage 98 `23#1074A` („94-98 ja“); `48#129E`, Ausformulierung `48#130A` |
| Z16 | Intelligente Monster haben individuelles Gedächtnis (Beziehungen, Rache, Angst, Informationsaustausch) | 📋 | `48#139E` |
| Z17 | Der Spieler entscheidet selbst, wie er Geheimhaltung handhabt (lügen, schweigen, enthüllen); das Dialogproblem ist erkannt | 📋 (Richtung); Lösung offen | `48#149E` |
| Z18 | Dialogsystem: zuerst klassische Auswahl, später möglicherweise freie KI-Gespräche | 📋 | `48#151E` |
| Z19 | Wichtige NPCs, die nicht durch Spielerhandlung sterben, bleiben am Leben („wichtige NPCs sollten eher nicht sterben außer du tötest sie“) | 📋 | Frage 105, „105. wichtige npcs sollten eher nicht sterben außer du tötest sie, würde ich sagen“ |

---

## 2. Entwurf: Von der Wahrnehmung zur Aussage

> [!NOTE]
> **Nur Vorschlag (💡).** Der Master verlangt, dass NPCs nur wissen, was sie erfahren haben (Z3). Für die Ermittlung muss daraus eine Kette werden: **Ereignis → Wahrnehmung → Erinnerung → Aussage.**

```text
Ereignis in der Welt
   |  (wer war in Sicht-/Hörweite? Licht, Entfernung, Hindernisse, Lärm)
   v
Wahrnehmung (Beobachtungseintrag pro NPC)
   |  (Zeit vergeht, Stress, Alkohol, Gespräche mit anderen)
   v
Erinnerung (kann verblassen, sich verändern)
   |  (Persönlichkeit, Furcht, Interessen, Beziehung zum Fragenden)
   v
Aussage (was der NPC dem Spieler sagt, vielleicht anders als er glaubt)
```

Warum vier Schritte? Jeder Schritt ist eine eigene Quelle von Irrtum oder Täuschung (siehe [Modul 10](10-Beweislogik-und-Wissensmodell.md#6-entwurf-unzuverlässige-quellen)): Man kann etwas nicht sehen, vergessen, falsch erinnern oder bewusst verschweigen. Die Quellen erwähnen die Ebenen Wahrnehmung (Stealth-Modell), Gedächtnis (Block 8C) und Aussage (Zeugenaussage als Information), ohne sie zu verbinden.

**Verbindung zum Wahrnehmungsmodell.** Die Tarn- und Wahrnehmungsregeln des Spiels (Gameplay-Systeme §7: Sicht abhängig von Richtung, Reichweite, Licht, Bewegung, Deckung; Gehör abhängig von Lautstärke, Entfernung, Wänden, Hintergrundlärm) sind für **Gegner** beschlossen. Für **Zivilisten als Zeugen** ist nirgends festgelegt, dass dasselbe Modell gilt. Der Entwurf geht davon aus, dass **dieselbe Wahrnehmungslogik** verwendet wird, damit Zeugen und Tarnung konsistent sind (❓ Q-E48).

---

## 3. Entwurf: Beobachtungseintrag

Ein Katalog möglicher Felder für das, was ein NPC wahrgenommen hat (kein Datenformat):

| Feld | Inhalt |
|---|---|
| Beobachter | NPC (mit Tier A bis D) |
| Ereignis | Verweis auf Weltereignis (Tat, Gespräch, Fahrzeug, Anomalie) |
| Zeitpunkt, Ort | wann und wo der Beobachter war |
| Sinn | gesehen, gehört, gerochen, gefühlt, erzählt bekommen |
| Qualität | wie deutlich (Entfernung, Licht, Lärm, Stress, Alkohol) |
| Identifizierte Personen | wen der Beobachter erkannt oder zu erkennen glaubt (Wiedererkennung, Z6) |
| Interpretation | was der Beobachter daraus macht (kann falsch sein) |
| Weitergabe | wem es erzählt wurde |
| Gewissheit | wie sicher der Beobachter ist (intern; der Spieler sieht sie nicht als Zahl, vgl. `23#53A`/`23#52E`: der Spieler merkt Haltung am Ton) |

Der Entwickler wollte, dass Zahlen wie Vertrauen und Angst **nicht sichtbar** sind; der Spieler soll sie an Ton, Wortwahl, Bereitschaft und Zugang bemerken (`23#52E` Punkt 9; `23#53A`). Das gilt auch für die Sicherheit einer Aussage: Der Spieler soll sie **aus dem Auftreten** schließen, nicht aus einer Zahl.

---

## 4. Entwurf: Erinnerung, Verfall und Verzerrung

Beschlossen: Erinnerungen werden schwächer oder ungenauer (Z2). Der Entwurf nennt Mechanismen, die zu Ungenauigkeit führen können:

| Mechanismus | Beispiel (erfunden) | Wirkung auf die Aussage |
|---|---|---|
| **Verblassen** | Zeit vergeht | Details gehen verloren (Zeitpunkt, Farbe, Zahl) |
| **Verschmelzen** | zwei ähnliche Ereignisse | Details werden vermischt |
| **Auffüllen** | Lücken werden mit Erwartetem ergänzt | falsche, aber plausible Details |
| **Beeinflussung** | andere erzählen etwas | Aussage übernimmt Fremdes (Gerücht, Abschnitt 12) |
| **Stress und Angst** | Gewalt, Gefahr | Details werden verzerrt, Aufmerksamkeit eng |
| **Alkohol und Drogen** | Z11, `23#1058A` | Beobachtungsqualität sinkt |
| **Voreingenommenheit** | Vorurteile, Interessen | Deutung färbt Aussage |
| **Übernatürlicher Einfluss** | Besessenheit, Bann, Manipulation | Erinnerung fehlt oder ist falsch (`23#1074A` Fragen 142–143: Besessenheit mit Erinnerungslücken) |

**Wichtig (Entwurf):** Der Verfall betrifft **die Erinnerung des NPC**, nicht das, was der Spieler schon aufgeschrieben hat. Eine einmal aufgenommene Aussage bleibt im Journal wie sie war (mit Zeitstempel).

**Keine Wahrheit durch Wiederholung.** Fragt der Spieler zweimal, bekommt er dieselbe Antwort, solange sich nichts geändert hat (Determinismus). Erst Zeit, Ereignisse oder Gespräche ändern die Antwort. So lässt sich das Verhalten testen ([Modul 23](23-Testkatalog-und-Abnahmekriterien.md)).

---

## 5. Entwurf: Aussagebereitschaft

Ein Zeuge sagt nicht automatisch aus, was er weiß. Bereitschaft ist ein eigener Zustand:

| Zustand (Arbeitsname) | Bedeutung | Typische Auslöser |
|---|---|---|
| **Offen** | sagt aus, was er weiß | Vertrauen, Hilfsbereitschaft, Gefallen |
| **Zurückhaltend** | sagt wenig, antwortet auf gezielte Fragen | Misstrauen, Unsicherheit |
| **Ängstlich** | sagt aus unter Vorbehalt, kann fliehen | Bedrohung, Gefahr im Spiel |
| **Schweigend** | verweigert | Loyalität, Schweigegeld, Angst, Interessen |
| **Feindselig** | lügt oder greift an | Konflikt, Ruf, Verdacht |
| **Bestochen** | schweigt oder lügt für Bezahlung | Geld, Gefallen (Z13) |
| **Bedroht** | wird zum Schweigen gebracht | Täter (Z13) |
| **Kooperativ mit Bedingung** | sagt aus, wenn Bedingung erfüllt wird | Handel, Hilfe |
| **Unfähig** | kann nicht aussagen (Schock, Verletzung, Bewusstlosigkeit, tot) | Medizin; Krankenhaus (`48#117E`) |
| **Verändert** | Wahrnehmung oder Persönlichkeit beeinflusst (Besessenheit, Bann) | übernatürlich |

Die **Auslöser** hängen von Persönlichkeit (Z10), Beziehung, Ruf und Situation ab (`23#52E` Punkt 9: Sympathie zeigt sich im Ton). Der Entwurf legt keine Formeln fest.

---

## 6. Entwurf: Befragung durch den Spieler

### 6.1 Der Gesprächsrahmen aus dem Quiz

Der Assistent schlug als Gerüst vor (`48#150A`, 💡 als Auslegung der Entwicklerfragen `48#149E`/`48#151E`): Ein Gespräch besteht aus **Spielerabsicht**, **NPC-Persönlichkeit**, **Weltzustand**, **Erinnerungen** und **Beweisen**; daraus entsteht eine passende **Dialogreaktion mit echten Spielkonsequenzen**. Absichten im Beispiel: Wahrheit sagen, lügen, drohen, schweigen, Beweise zeigen. Die Spielengine entscheidet über Fakten und Konsequenzen, nicht ein Sprachmodell.

Der Entwickler wollte, dass das Dialogsystem **von Anfang an erweiterbar** ist; zuerst klassische Dialogauswahl, später möglicherweise freie Gespräche (`48#151E`: „anfangs einfach dialog mit auswahlmöglichkeiten später wenn alles gut läuft kann man das so machen“).

### 6.2 Entwurf: Befragungsabsichten

| Absicht | Beschreibung | Wirkt auf |
|---|---|---|
| **Fragen** | offene Frage zu Ereignis, Person, Ort, Zeit | Aussage (Z3) |
| **Nachhaken** | konkrete Rückfrage zu einer Aussage | Aussage, Widerspruchsprüfung |
| **Vorhalten** | Beweis zeigen (Foto, Dokument, Aussage eines anderen) | Aussage, Bereitschaft |
| **Täuschen** | Falsches behaupten (Bluff, Rolle, Ausweis) | Aussage, Ruf (bei Entdeckung) |
| **Überreden** | Argument, Appell, Gefallen | Bereitschaft |
| **Bezahlen** | Geld oder Gefallen für Information | Bereitschaft, Gerüchte (der Zeuge erzählt weiter) |
| **Drohen** | Druck ausüben | Bereitschaft, Angst, Anzeige, Ruf, Recht |
| **Schweigen** | absichtlich nichts sagen; Gelegenheit lassen | Aussage (Schweigen bricht manchmal Zurückhaltung) |
| **Schützen** | zusichern, nichts weiterzugeben | Bereitschaft, Vertrauen |
| **Beobachten** | zuhören, Körpersprache lesen (kein Dialog) | Eindruck (Ton, Verhalten) |

Alle Absichten sind aus den Beispielen des Assistenten (`48#150A`) und den Entwicklerantworten zu Bestechung und Bedrohung (Frage 48) zusammengesetzt. **Es ist keine Absichtenliste beschlossen** (❓ Q-E49). Insbesondere das „Drohen“ berührt das Strafrecht ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)); die Quellen erwähnen es nur als Beispiel des Assistenten und indirekt in Frage 48.

### 6.3 Folgen einer Befragung

| Folge | Beispiel |
|---|---|
| Wissen | neue Behauptung im Wissensstand |
| Beziehung | Vertrauen, Angst, Verdacht ändern sich (intern) |
| Gedächtnis | Der NPC erinnert sich an das Gespräch (Z1) |
| Gerüchte | Der NPC erzählt weiter (Z4); der Spieler wird Thema |
| Recht | Anzeige bei Bedrohung, Verleumdung, Behinderung |
| Fall | Zeuge fühlt sich bedrängt und wendet sich an die Polizei oder den Täter |

### 6.4 Kein Verschwinden von Wissen

Wissen, das der Spieler aus einem Gespräch hat, bleibt auch, wenn der NPC später stirbt oder umzieht. Nur der **Zugang** zu weiteren Auskünften entfällt (Z19 betrifft das Sterben wichtiger NPCs).

---

## 7. Entwurf: Verdächtige

### 7.1 Zwei Verdächtigenlisten

Es gibt zwei getrennte Sichten, die die Quellen nicht benennen, aber voraussetzen (B12–B13 in Modul 10):

| Liste | Eigner | Inhalt |
|---|---|---|
| **Spielerverdacht** | Spieler (Journal) | Personen, die der Spieler als relevant oder verdächtig **markiert hat** (`23#54E`; Prompt-Entwurf Titel 021) |
| **Behördenverdacht** | Polizei/Behörde | Personen, die die Behörde aufgrund ihrer Beweislage verdächtigt (`M-PD`: Konfidenzmodell) |

Beide Listen dürfen sich unterscheiden und widersprechen. Der Spieler kann **zu Unrecht** verdächtigt werden und selbst falsche Verdächtige verfolgen (Z14; `23#1062A`: „falschen Verdächtigen beschuldigen“).

### 7.2 Entwurf: Merkmale eines Verdächtigen (Katalog)

| Merkmal | Bedeutung |
|---|---|
| Motiv | gibt es einen Grund? (Beziehung, Geld, Rache, Angst, Fluch, Zwang) |
| Mittel | hatte die Person die Möglichkeit/Fähigkeit/Werkzeug? |
| Gelegenheit | war die Person zur Tatzeit am Ort? (Alibi, Abschnitt 8) |
| Verhalten | auffällig, nervös, aggressiv, hilfsbereit |
| Verbindung | Beziehung zu Opfern, Orten, anderen Verdächtigen |
| Vorgeschichte | frühere Vorfälle |
| Glaubwürdigkeit der Verdachtsquelle | wer hat den Verdacht gestreut (Gerücht, Zeuge, Polizei) |

Wichtig: Das System **bewertet** Verdächtige nicht für den Spieler. Der Spieler sieht, was er erfahren hat. Das Spiel kennt die Wahrheit (Täter/Unschuldiger/Mitwisser) im Fall ([Modul 09](09-Falllebenszyklus-und-Generierung.md)).

### 7.3 Wer kann Täter sein? (Entwurf)

Aus [Modul 02](02-Falltypen-und-Fallmodell.md#8-mischfälle-falsche-alarme-und-täuschung): Mensch, Wesen, Kult, Hunter, Serienmörder, Tier, Mischformen, **Wesen in menschlicher Tarnung**. Letzteres folgt aus `23#1076A` (intelligente Wesen leben unter Menschen mit Haus, Beruf, Bankkonto usw.) und `48#154A` (Formwandler). Die Verdächtigensuche muss das zulassen: Ein „normaler“ Nachbar kann ein Wesen sein (Fall H aus `23#55A`: scheinbar normaler Mord, spät übernatürlich).

### 7.4 Unschuldige und Folgen falscher Beschuldigung

Falsch beschuldigte Personen haben Folgen (Ruf, Rache, Selbstmord, Anzeige, Gerüchte). Die Quellen nennen „falschen Verdächtigen beschuldigen“ als mögliche Spielerfehler mit Folgen (`23#1062A`) ohne Details. Ein Entwurf: Ein falsch Beschuldigter ist ein **NPC mit Gedächtnis** (Z1), der den Spieler später anders behandelt.

---

## 8. Entwurf: Alibi und Tagesabläufe

Beschlossen ist: NPCs haben Tagesabläufe, Arbeitsplätze, Wohnorte, Fahrzeuge (Block 8A/8B, `23#1057E` Fragen 1–22 bis auf die Generationen, die später kommen, Frage 9). Daraus ergibt sich eine **natürliche Alibi-Quelle**: Die Simulation weiß, wo eine Person war.

| Aspekt | Entwurf |
|---|---|
| **Wahres Alibi** | Die Weltsimulation hat den Aufenthalt der Person aufgezeichnet (nur für wichtige NPCs, Tier A/B; andere abstrahiert, `23#1056A` Fragen 121–123). |
| **Alibi als Aussage** | Die Person behauptet einen Aufenthalt. Der Spieler prüft sie über Zeugen, Kameras, Belege, Gerüchte. |
| **Prüfbarkeit** | Hängt davon ab, wer die Person gesehen hat (Beobachtungseinträge) und ob Aufzeichnungen existieren (Kameradichte offen, `M-OD`). |
| **Falsches Alibi** | Person oder Komplize lügt; das System braucht ein Mittel, die Lüge zu entlarven (Widerspruch, Beleg). |
| **Tier-Grenzen** | Für Tier C und D (leichte Persistenz, Crowd) gibt es nur grobe Zustände (`AT_WORK`, `HOME`, `TRAVEL`, Frage 122 in `23#1056A`; Antwort `23#1057E`: „ja sowieso nicht, aber wenn man dann in das disktikt kommt darf die straße nicht leer sein, sondern auch voll“); ein detailliertes Alibi wird **bei Bedarf materialisiert** (Frage 123). |

Die Materialisierung ist eine **Konsistenzaufgabe**: Wenn der Spieler ein Alibi eines Tier-C-NPC prüft, muss das System einen glaubwürdigen Tagesablauf **konsistent rekonstruieren**, der zu bereits Bekanntem passt (Frage 125, Antwort des Entwicklers: „glaubwürdig, das wichtigste von diesem spiel ist realismus“). Das ist ein harter Entwurfspunkt (❓ Q-E50).

---

## 9. Entwurf: Verhör des Spielers durch Behörden

### 9.1 Beschlossen

Wer verdächtigt wird, kann verhört werden (Z14). Festnahme bedeutet **kein** Game Over (`23#1057E` Punkt 68; `48#155E`). Option C/D der Justiz nennt Verhöre, Durchsuchungen, Beschlagnahmungen und verdeckte Ermittlungen als Teile (`48#154A` Optionen, gewählt D mit `48#155E`; die Aufzählung der Funktionen in `48#156A` ist Ausformulierung).

### 9.2 Entwurf: Ablauf

```text
Verdacht -> Vorladung/Festnahme -> Gewahrsam -> Verhör -> Entscheidung der Behörde
```

| Phase | Möglichkeiten des Spielers (Vorschlag) |
|---|---|
| Vorladung/Anhalten | kooperieren, fliehen, verweigern, Anwalt verlangen (Anwälte sind später ausbaubar, Frage 69) |
| Gewahrsam | warten, Kontakte nutzen, kooperieren |
| Verhör | wahrheitsgemäß, schweigen, lügen, Beweise zeigen, auf Anwalt bestehen |
| Entscheidung | Freilassung, Anklage, Gewahrsam |

**Wahrheitsprüfung.** Das Spiel kennt die Wahrheit und die Beweislage der Behörde. Die Aussage des Spielers wird **gegen die Beweislage der Behörde** geprüft, nicht gegen eine verborgene ‚Wahrheitsprüfung‘. Lügt der Spieler und die Behörde kann es widerlegen, folgen Konsequenzen; kann sie es nicht, bleibt die Lüge ohne Wirkung. Das ist die Umsetzung von ‚Gesetz folgt tatsächlicher Information‘ (`M-PD`) auf das Verhör.

### 9.3 Sonderfälle

| Fall | Verhalten |
|---|---|
| Der Spieler ist unschuldig (Formwandler) | Das Verhör prüft die Beweislage (gefälschte Beweise, Kamera); der Spieler muss Gegenbeweise liefern oder auf Hilfe von außen setzen (`48#158A`-Beispiel des Assistenten) |
| Der Spieler kennt den wahren Täter, darf es aber nicht sagen (Geheimhaltung) | Konflikt zwischen Geheimhaltung und Entlastung; Spielerentscheidung (`48#149E`) |
| Korrupter Beamter | Verhör wird manipuliert (Option D, `48#155E`) |
| Koop | nur der Betroffene wird festgenommen (`48#161E`) |

---

## 10. Entwurf: Verhör von Wesen

### 10.1 Beschlossen

Intelligente Wesen können befragt werden (Frage 98 in `23#1074A`, Antwort „94-98 ja“). Gefangennahme ist als umfassendes System beschlossen (Option D der Frage `48#128A`, gewählt in `48#129E`); der Assistent nennt Verhöre, Gespräche, Verhandlungen und mögliche Kooperation als Teil (`48#130A`, 💡). Monster haben individuelles Gedächtnis (Z16) und können sich an Fallen und Hunter erinnern (`48#139E`).

### 10.2 Entwurf

| Aspekt | Vorschlag |
|---|---|
| **Wissen des Wesens** | Wie bei NPCs: nur, was es erfahren hat (Z3). Es kennt Territorien, Mitglieder, Pläne, eigene Taten, Schwächen anderer. |
| **Wahrhaftigkeit** | Wesen können lügen, ausweichen, manipulieren; der Spieler muss Aussagen wie jede andere Quelle prüfen ([Modul 10](10-Beweislogik-und-Wissensmodell.md)). |
| **Druckmittel** | abhängig von der Mythologie (Schwäche, Bann, Wunsch); die Wirkung hängt von der Art ab (`48#130A`: keine universelle Zelle). |
| **Sprache** | `M-PD`: Intelligenz hat Dimension Sprache; manche Wesen sprechen nicht. |
| **Risiko** | Flucht, Sabotage, Angriffe, fehlerhafte Schutzmaßnahmen (`48#130A`, 💡) |
| **Folgen** | Auswirkungen auf Story, Beziehungen, Moral und Welt (`48#130A`, 💡) |
| **Gnade und Feindseligkeit** | Intelligente Wesen erinnern sich an Gnade und Feindseligkeit (`M-PD`) |

Die Quellen enthalten **keine** Regeln für Zwangsmittel bei Verhören. Ob und wie Zwang gespielt wird, ist nicht beschlossen (❓ Q-E51). Auch das Verhältnis zur Altersfreigabe und zur Gewaltdarstellung ist dort nicht festgelegt.

---

## 11. Entwurf: Bedrohung, Bestechung, Zeugenschutz

Beschlossen: Ein Täter kann Zeugen ruhigstellen (Schweigegeld, Einschüchterung); der Spieler selbst kann bestochen oder bedroht werden (`23#1058A`, Antwort Frage 48 „ja kann er, aber er muss nicht petzen, vielleicht bekommt er auch schweigegeld, oder es wird versucht ihn zu töten“). Die Quellen sagen nichts über **Zeugenschutz**.

| Vorgang | Entwurf |
|---|---|
| Täter bedroht Zeugen | Zeuge ändert Bereitschaft (Abschnitt 5); kann fliehen, schweigen, lügen |
| Täter beseitigt Zeugen | Zeuge stirbt; Fall verliert diesen Ansatz (Prüfregel V3 in Modul 09) |
| Spieler schützt Zeugen | Begleiten, Unterbringen (HQ, Bunker); Zeuge bleibt verfügbar |
| Behörde schützt Zeugen | Zeuge wird verlegt; Zugang nur über Behörde |
| Bestechung des Spielers | Entscheidung: annehmen (Folgen im Ruf/Recht) oder ablehnen (Folgen durch den Täter) |
| Rache | Zeuge erinnert sich (Z1) an Bedrohung; kann den Spieler anzeigen |

---

## 12. Entwurf: Gerüchte als Informationsnetz

Beschlossen: Information verbreitet sich über Freunde, Familie, soziale Medien und Nachrichten (Z4) und verzerrt sich (Z5). Auch Gespräche der NPCs untereinander sind Teil der Welt (Z8). Die Oktoberentscheidung unterscheidet **Gerüchte, persönlichen Glauben, belegtes Wissen und offizielle Anerkennung** (`48#148A`, 💡 als Auslegung von `48#147E`).

### 12.1 Entwurf: Gerüchtekette

```text
Ereignis -> Beobachter -> (erzählt) -> NPC 2 -> (verzerrt) -> NPC 3 -> ... -> Medien
```

| Eigenschaft | Wirkung |
|---|---|
| **Reichweite** | hängt von Beziehungen und Orten ab (Nachbarschaft, Arbeitsplatz, Stadt) |
| **Verzerrung** | pro Schritt kleine Änderungen (Z5) |
| **Geschwindigkeit** | schneller bei dramatischen Ereignissen (nicht quantifiziert) |
| **Rückverfolgung** | Der Spieler kann eine Gerüchtekette zurückverfolgen, indem er Quellen befragt |
| **Wahrheitsgehalt** | variiert; Gerüchte können vom Täter gestreut sein (Täuschung) |
| **Einfluss auf Verdacht** | Gerüchte lenken Verdacht auf Unschuldige; Behörden reagieren |

### 12.2 Regional

Der Wissensstand über das Übernatürliche ist regional verschieden (`48#147E`, `48#148A`). Gerüchte über Monster haben in manchen Regionen mehr Gewicht. Modul 17 behandelt das.

---

## 13. Dialogumfang und Handschrift

Der Entwickler fürchtet eine Explosion des Dialogumfangs („100000 seiten pro hauptcharakter“, `48#149E`). Die Antwort des Assistenten war ein Hybridsystem (`48#150A`, 💡): handgeschriebene Hauptgeschichte, modulare Dialoge für wiederkehrende NPCs, wiederverwendbare Gesprächsabsichten für gewöhnliche NPCs, optionale KI-Formulierungen für spontane Gespräche. Für die Ermittlung heißt das:

| Dialogtyp | Wer schreibt | Beispiel für Ermittlung |
|---|---|---|
| Schlüsselzeugen eines Hauptfalls | handgeschrieben | zentrale Aussagen |
| Zeugen großer Nebenfälle | handgeschriebene Kerndialoge mit Varianten | Aussagen mit Bedingungen |
| Wiederkehrende NPCs (Sheriff, Gerichtsmediziner, Informant) | modulare Dialoge mit Gedächtnis | Auskünfte nach Beziehung |
| Gewöhnliche Zeugen dynamischer Aufträge | wiederverwendbare Absichten und Aussagebausteine | Beobachtungen in Textbausteinen |
| Spontane Gespräche | optional KI-gestützte Formulierung | Rahmen durch Regeln, **Fakten durch das Spiel** |

**Grundregel (aus `48#150A`, 💡, aber im Einklang mit `M-PD`):** *Die Spielengine entscheidet über Fakten und Konsequenzen, nicht das Sprachmodell.* Eine Zeugenaussage ist ein **Faktenpaket** (Beobachtungseintrag), das in Worte gefasst wird. Wenn später KI-Formulierung kommt, darf sie nichts hinzuerfinden, was nicht im Eintrag steht. Ob Texte und Stimmen für alle dynamischen Aussagen vertont werden, ist offen (`48#150A`: Trennung Dialoglogik und Sprachausgabe).

---

## 14. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Zeuge ist ein Wesen in Tarnung | Er sagt aus, um zu lenken | Aussage ist Täuschung (Modul 10 §6); Entlarvung über Widerspruch, Anomalie, Reaktion auf Gegenstände |
| Zeuge ist besessen oder verwandelt | Aussage durch Wesen | Zustand „Verändert“; Hinweise im Verhalten |
| Formwandler imitiert einen Zeugen | zweiter Auftritt widerspricht dem ersten | Modul 13 |
| Zeuge verwechselt den Spieler | Wiedererkennung ist unsicher (Z6) | Folge: Fahndung auf falsche Person; Gegenbeweise |
| Mehrere Zeugen stimmen ab | abgesprochene Aussagen | Auffälligkeit: zu gleich; Entlarvung durch Detailfragen |
| Zeuge ist Kind | – | **nicht in den Quellen**; Altersgrenzen der Darstellung gelten (Adult 09, Gameplay-Systeme §12) |
| Zeuge stirbt während der Ermittlung | – | Fall bleibt lösbar (V3); Hinweis im Journal bleibt |
| Zeuge im Krankenhaus | – | Zustand „Unfähig“ bis zur Genesung; Zeitsprung beachten |
| Zeuge ist in Haft | – | Zugang über Besuch, Anwalt, Behörde |
| Zeugen in einer Region mit anderem Wissen | – | regionale Wahrnehmung (Modul 17) |
| Gespräche im Koop | mehrere Spieler befragen | Wer führt? `48#97E`: persönliche Entscheidungen individuell, gemeinsame per Abstimmung ([Modul 18](18-Koop-Ermittlung.md)) |

---

## 15. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| NPC-Gedächtnis und Beziehungen (Gameplay-Systeme §8) | Quelle aller Beobachtungen und Erinnerungen |
| Tarn- und Wahrnehmungsmodell (Gameplay-Systeme §7) | Qualität der Beobachtung |
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Aussage als Beleg |
| Polizei und Recht ([Module 14](14-Polizei-Behoerden-und-Fahndung.md), [15](15-Gerichte-und-Verfahren.md)) | Verhör, Anzeige, Zeugenschutz |
| Identität ([Modul 13](13-Identitaet-Ruf-und-Formwandler.md)) | Wiedererkennung, falsche Beschuldigung |
| Dialogsystem | Absichten, Reaktionen |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Beobachtungseinträge und Erinnerungen sind Zustand |
| Simulationsstufen | Tier A bis D steuert, wie viel Zustand existiert |
| Hunter-Netzwerk | andere Hunter als Quellen; Rufwirkung |
| Medien und soziale Medien | Verbreitung; Fälschungen |

---

## 16. Was fehlt

- Die **Wahrnehmungslogik für Zivilisten** (Q-E48).
- Eine **Absichtenliste** und die Behandlung von **Druck** (Q-E49).
- Die **Konsistenz** bei Materialisierung von Alibis (Q-E50).
- Regeln für **Zwang** bei Verhören von Wesen (Q-E51).
- **Zeugenschutz** (nicht in den Quellen).
- Die **Tiefe der Aussagen** je Tier (Z12).
- Ob **Gedächtnisverfall** pro NPC individuell ist.
