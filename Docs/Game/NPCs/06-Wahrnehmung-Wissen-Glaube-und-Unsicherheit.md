# NPCs 06: Wahrnehmung, Wissen, Glaube und Unsicherheit

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie ein NPC überhaupt etwas erfährt, wie sicher er sich ist und wie er zu falschen Überzeugungen kommt. Beschlossen sind die Wissensgrade und Wahrnehmungszustände des Masters sowie die Regel ‚kein Allwissen‘; **Ketten, Schwellen und Verdachtsmodelle sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Die Sicht der Ermittlung steht in [Ermittlung 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md) und [12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md); die der Monster in [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §2–§3.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Die Kette von Ereignis zu Eintrag](#2-entwurf-die-kette-von-ereignis-zu-eintrag)
3. [Wissensgrade](#3-wissensgrade)
4. [Entwurf: Wissen, Glaube, Gerücht, Anerkennung](#4-entwurf-wissen-glaube-gerücht-anerkennung)
5. [Entwurf: Wahrnehmung durch Menschen](#5-entwurf-wahrnehmung-durch-menschen)
6. [Entwurf: Verdacht gegenüber dem Spieler](#6-entwurf-verdacht-gegenüber-dem-spieler)
7. [Entwurf: Zeugen](#7-entwurf-zeugen)
8. [Entwurf: Falsche Überzeugungen](#8-entwurf-falsche-überzeugungen)
9. [Wahrnehmung des Übernatürlichen](#9-wahrnehmung-des-übernatürlichen)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-WA-001 | Wahrnehmung arbeitet mit **Unsicherheit** statt mit magischen Spielerkoordinaten | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-002 | Das Wissen einer KI unterscheidet mindestens bestätigtes Wissen, starken Verdacht, schwachen Verdacht und Unbekanntes | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-003 | Wahrnehmungszustände können sein: unaware, suspicious, investigating, alert, confirmed target, searching | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-004 | Die Gewissheit über die letzte bekannte Position nimmt mit der Zeit ab; Gegner verlieren Präzision, nicht das Wissen, dass eine bestätigte Bedrohung existiert | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-005 | Adaptive Taktiken dürfen nur auf **beobachteten oder plausibel erschlossenen** Tatsachen beruhen; keine globale Telemetrie, kein verstecktes Metawissen | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-006 | Menschliche Gegner teilen Informationen nur über **plausible Kommunikation** (Stimme, Funk, Sichtzeichen); kein Schwarmgeist. Die Kommunikation von Wesen ist artspezifisch | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-007 | Intelligente Gegner können Hinweise (Geräusch, Blut, Türen, Fenster, letzte Position) zu Hypothesen kombinieren | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-008 | Wiederholte Ablenkungstricks können erkannt werden; der Gegner kann misstrauisch werden, ignorieren oder eine Falle vermuten | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-009 | Sicht berücksichtigt Richtung, Reichweite, tatsächliche Beleuchtung, Bewegung, Deckung, Silhouette, Haltung und Wetter; Dunkelheit macht nicht unsichtbar. Gehör berücksichtigt Lautstärke, Entfernung, Raumakustik, Wände und Türen, Hintergrundgeräusch, Wetter und Hörfähigkeit des Hörers | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-010 | Ein NPC weiß etwas nur, wenn er es gesehen, gehört, erzählt bekommen oder über Nachrichten und soziale Medien erfahren hat | 📋 | `23#1056A` Frage 25; „ja sicher“ in `23#1057E`; `23#1058A` |
| N-WA-011 | Zeugen können Polizei und Rettung rufen (Notruf); der Täter kann beschrieben werden (Kleidung, Fahrzeug, Kennzeichen, Richtung) | 📋 | `23#1056A` Fragen 60–61; „ja“ in `23#1057E` |
| N-WA-012 | Der Spieler kann Zeuge einer Straftat werden; er muss nicht melden, kann Schweigegeld bekommen oder bedroht werden: „ja kann er, aber er muss nicht petzen, vielleicht bekommt er auch schweigegeld, oder es wird versucht ihn zu töten“ | 📋 | `23#1056A` Frage 48; Antwort in `23#1057E` |
| N-WA-013 | Die Polizei weiß nicht magisch alles: kein sofortiger Fahndungsstern, nur weil irgendwo ohne Zeugen etwas passiert | 📋 | `23#1056A` Frage 59; „ja genau, wenn man verdächtigt wird kann es zu einem verhör kommen und so weiter“ in `23#1057E` |
| N-WA-014 | Verkleidung wirkt nicht magisch („keine Hitman-Magie“): Ausweis, Qualität der Fälschung, Verhalten, Wissen und Gespräche zählen; schlechte Fälschung kann auffallen; falsches Verhalten erzeugt Verdacht | 📋 | `23#1088A` Fragen 57–67; „41-60 super“ und „61-67 ja“ in `23#1089E`; `23#1090A` ‚Verkleidung‘ |
| N-WA-015 | NPCs reagieren unterschiedlich darauf, wo der Spieler sich aufhält (Trespassing) | 📋 | `23#1088A` Frage 59; „41-60 super“ in `23#1089E` |
| N-WA-016 | Das öffentliche Wissen über das Übernatürliche ist regional verschieden und kann sich ändern; es werden **Gerüchte, persönlicher Glaube, belegtes Wissen und offizielle Anerkennung** unterschieden | 📋 / 💡 | `48#147E` (Wahl); Auslegung `48#148A` |
| N-WA-017 | Die Polizei erklärt Dinge zunächst normal (Wildtier, Unfall, Mord), wenn sie das Übernatürliche nicht kennt | 📋 | `23#1056A` Frage 116; „ja so wie in der serie“ in `23#1057E` |
| N-WA-018 | NPCs reagieren unterschiedlich auf Übernatürliches (Unglaube, Angst, Religion, Neugier, Verdrängung); manche glauben nichts, andere an Religion, Geister, Aliens, Verschwörung oder tatsächlich an Monster | 📋 | `23#1056A` Frage 117; „genau“ in `23#1057E`; `23#1058A` ‚Supernatural-Reaktion‘ |
| N-WA-019 | Tiere reagieren teilweise früher auf Übernatürliches als Menschen | 📋 | `23#1056A` Frage 119; „ja“ in `23#1057E` |
| N-WA-020 | Begegnungen können NPCs dauerhaft verändern | 📋 | `23#1056A` Frage 118; „ja“ in `23#1057E` |
| N-WA-021 | NPCs bemerken, wenn sie beobachtet, belauscht oder verfolgt werden (in einer Bewertungstabelle des Assistenten zu GTA-VI-Ideen als ‚unbedingt‘ eingestuft, ohne einzelne Antwort des Entwicklers) | 💡 | `23#510A` Tabelle (Vorschlag des Assistenten); stützt sich auf `M-PD` |

---

## 2. Entwurf: Die Kette von Ereignis zu Eintrag

Die Ermittlungs-Dokumentation entwirft die Kette *Ereignis → Wahrnehmung → Erinnerung → Aussage* ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §2). Hier die Gegenseite (💡):

```text
1. Ereignis in der Welt (E)
2. Reichweite:     wer war nah genug?               -- Sicht-/Hörradius, Raum, Wände, Licht
3. Kanal:          sehen | hören | riechen | erzählt | Medien | Schluss
4. Aufmerksamkeit: war der Beobachter gerade bei der Sache?
                   (Beschäftigung, Müdigkeit, Rausch, Ablenkung, Erwartung)
5. Erkennen:       was nimmt er wahr?  (Unterscheidbarkeit, Fremdheit)
6. Deuten:         was glaubt er, gesehen zu haben?  (Weltbild, Erwartung, Vorwissen)
7. Bewerten:       wie wichtig ist es ihm?            -- Modul 05
8. Eintrag:        MemoryEntry (source, confidence, accuracy)
```

**Regeln (Entwurf):**

1. **Jeder Schritt kann scheitern.** Der Mensch hört etwas, ohne es zu bemerken; bemerkt es, ohne es zu deuten; deutet es, ohne es zu behalten (N-WA-001).
2. **Gewissheit ist ein Ergebnis der Schritte.** Sie wird aus Qualität von Kanal, Aufmerksamkeit und Deutung abgeleitet, nicht gewürfelt.
3. **Wahrnehmung und Aussage sind getrennt.** Was er wahrnimmt, was er glaubt, was er erzählt, sind drei Dinge (Modul 05, Ermittlung 12).
4. **Wahrnehmung wird nicht pro Frame für ferne NPCs gerechnet.** Für NPCs außerhalb der Spielernähe wird Wahrnehmung aus Zeitplan und Ereignisgeometrie ereignisbasiert abgeleitet (Modul 22). Die Qualität darf dabei sinken; das Ergebnis muss mit dem sichtbaren Verhalten im Nahbereich konsistent sein (`23#1057E` Punkte 122–123).

---

## 3. Wissensgrade

`M-PD` nennt für KI vier Wissensgrade (N-WA-002) und sechs Wahrnehmungszustände (N-WA-003). Sie gelten für Gegner; die Quellen lassen offen, ob dasselbe Modell für Zivilisten, Zeugen und Freunde gilt. Der Entwurf überträgt es (💡):

| Wissensgrad | Bedeutung | Beispiel (Entwurf) | Wirkung |
|---|---|---|---|
| **bestätigt** | direkt beobachtet oder verlässlich erfahren | ‚Ich habe ihn selbst gesehen.‘ | handelt darauf, sagt es mit Sicherheit |
| **starker Verdacht** | mehrere Hinweise passen zusammen | ‚Das kann nur er gewesen sein.‘ | handelt vorsichtig, fragt nach, meldet unter Vorbehalt |
| **schwacher Verdacht** | einzelner Hinweis oder Eindruck | ‚Irgendetwas stimmt nicht.‘ | beobachtet, prüft, bleibt reserviert |
| **unbekannt** | nichts oder nichts Passendes | – | keine Reaktion |

**Beziehung zu den Wahrnehmungszuständen:** Der Wahrnehmungszustand ist der *Augenblick* (zum Beispiel ‚alert‘), der Wissensgrad ist der *Stand der Erkenntnis* (zum Beispiel ‚starker Verdacht‘). Ein Wachmann kann ‚alert‘ sein und nur schwachen Verdacht haben.

**Verfall (N-WA-004, 📋):** Die Gewissheit über Position und Einzelheiten nimmt ab, nicht das Wissen um die Bedrohung. Entwurf: Ein bestätigtes Wissen kann zu starkem Verdacht herabgestuft werden, wenn sich Hinweise widersprechen; ein Hinweis, der nicht erneuert wird, verblasst (Modul 05).

**Verknüpfung mit Beweislogik:** Die Ermittlungs-Dokumentation führt Wissens- und Hypothesenstufen für den Spieler ([Ermittlung 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md)). Die NPC-Stufen sind **parallel**, nicht dieselben: Der Spieler hat im Archiv sein Wissen, der NPC seines im Datensatz.

---

## 4. Entwurf: Wissen, Glaube, Gerücht, Anerkennung

Das Quiz zum öffentlichen Wissen (`48#147E`) unterscheidet, laut Auslegung des Assistenten (`48#148A`, 💡), **Gerüchte, persönlichen Glauben, belegtes Wissen und offizielle Anerkennung**. Der Entwurf legt dieselben vier Ebenen auf den einzelnen NPC (💡):

| Ebene | Wer hält sie | Wie sie entsteht | Wie sie sich ändert |
|---|---|---|---|
| **Gerücht** | im Umlauf, nicht beim einzelnen | Erzählung, Medien | verzerrt sich, wandert, flaut ab (Modul 07) |
| **Persönlicher Glaube** | der einzelne NPC | Erlebnis, Weltbild, Vertrauen in Quelle | durch Erlebnis, Gespräch, Beweis |
| **Belegtes Wissen** | einzelner NPC oder Gruppe | Beweis, Dokument, Augenschein | durch neue Beweise |
| **Offizielle Anerkennung** | Behörde, Institution | förmliches Verfahren | kaum und langsam |

Der Satz des Assistenten dazu: ‚Ein virales Video bedeutet also nicht automatisch, dass eine ganze Stadt an Vampire glaubt.‘ (`48#148A`, 💡). Daraus folgt eine Prüfregel: **Ein Ereignis, das viele sehen, macht nicht automatisch alle zu Gläubigen** (N-EN-071).

**Ein NPC glaubt nicht, was er nicht überprüfen kann, und glaubt, was zu seinem Weltbild passt.** Das ist Entwurf; die Quellen verlangen nur, dass Reaktionen unterschiedlich sind (N-WA-018).

---

## 5. Entwurf: Wahrnehmung durch Menschen

`M-PD` beschreibt die Wahrnehmung für Gegner-KI. Menschliche Zivilisten, Zeugen und Freunde nutzen dasselbe Kernmodell in vereinfachter Form; Monster haben **artspezifische Sinne** ([Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §2).

| Kanal | Faktoren (aus N-WA-009) | NPC-Besonderheit (Entwurf) |
|---|---|---|
| Sehen | Richtung, Reichweite, Licht, Bewegung, Deckung, Silhouette, Haltung, Wetter | Brille, Sehschwäche, Müdigkeit, Rausch, Aufmerksamkeit |
| Hören | Lautstärke, Dauer, Entfernung, Raumakustik, Wände, Türen, Hintergrund, Wetter, Hörvermögen | Kopfhörer, Musik, Schlaf, Schwerhörigkeit |
| Gerüche | Blut, Rauch, Verwesung, Gas | menschlich schwach; Tiere und manche Wesen stärker |
| Erzählt | Gespräch, Telefon, Nachricht | Vertrauen in die Quelle, Qualität der Weitergabe |
| Medien | Zeitung, TV, Radio, Social Media, Polizeifunk | Medienkonsum des NPC; Verzögerung; Verzerrung |
| Schluss | Kombination von Hinweisen | Intelligenz, Vorwissen, Weltbild |

**Wahrnehmungszustände für Zivilisten (Entwurf).** Sechs Gegnerzustände (N-WA-003) sind für Zivilisten zu hoch gegriffen; der Entwurf nutzt drei für ruhige und drei für gefährliche Lagen:

| Zustand | Bedeutung | Verhalten |
|---|---|---|
| `unbemerkt` | nichts aufgefallen | Alltag |
| `aufmerksam` | etwas stimmt nicht, prüft | schaut hin, hält inne, spricht mit anderen |
| `besorgt` | Verdacht oder Unbehagen | meidet, beobachtet, ruft Hilfe an |
| `alarmiert` | Gefahr oder Straftat bestätigt | flieht, ruft Notruf, greift ein, versteckt sich |
| `suchend` | verfolgt eine Spur (Beamte, Gegner, Familie) | sucht |
| `fixiert` | hat das Ziel erkannt | folgt, stellt, meldet |

**Gerüche und Spuren** (`M-PD`: ‚Blood trails can be visually followed by humans where plausible; scent tracking is creature-specific‘) gehören zur Spurensuche ([Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md)).

---

## 6. Entwurf: Verdacht gegenüber dem Spieler

Verkleidung, Hausfriedensbruch, Verfolgung und Verhalten sollen **nicht magisch** wirken (N-WA-014, N-WA-015). Der Entwurf macht daraus ein Verdachtsmodell des NPC gegenüber dem Spieler (💡):

```text
Anlass (Auftreten, Ort, Gespräch, Fahrzeug)
   │
   ▼
Erwartung des NPC:  Passt dieser Mensch hierher?
   │      Rolle (Uniform, Ausweis) – Ort – Uhrzeit – Verhalten – Wissen – Begleitung
   ▼
Verdachtsbildung:  unauffällig → bemerkt → misstrauisch → überzeugt (+ Gegenmaßnahme)
   │
   ▼
Reaktion:  ignorieren | ansprechen | beobachten | Sicherheitsdienst rufen | Polizei rufen | festhalten
```

| Verdachtsquelle | Beispiel | Quelle |
|---|---|---|
| Rolle ohne Beleg | Mechaniker ohne Werkzeug, Lieferant ohne Paket | N-WA-014 |
| Falscher Ort/Zeit | Mitarbeiter in Sperrzone, nachts im Büro | N-WA-015 |
| Verhalten | falsche Fachsprache, Orientierungslosigkeit | N-WA-014 (‚Verhalten zählt‘) |
| Ausweis | schlechte Fälschung | N-WA-014 |
| Wiedererkennung | frühere Rolle erkannt | N-GE-019 |
| Auffällige Begleitung | Waffe, Blut, Verletzung | `M-PD` (Injury macht Stealth schwerer) |
| Gerücht | über den Spieler in der Gegend | Modul 07 |

**Persönlichkeit wirkt:** Misstrauische NPCs fragen früher nach, gutgläubige später (N-PS-010). Der Berufskontext wirkt: Sicherheitspersonal ist aufmerksamer als ein Lieferant.

**Gegenmaßnahmen des Spielers** sind Spielmechanik der Tarnung (siehe [Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) §10), nicht dieses Moduls.

---

## 7. Entwurf: Zeugen

Aus den Quellen folgt eine Zeugenfunktion, die das Weltsystem trägt (N-WA-011 bis N-WA-013):

```text
Straftat / Ereignis
  ├─ Beobachter vorhanden?   nein -> kein Zeuge; keine Polizei (N-WA-013)
  ├─ Beobachter nennt:       was gesehen? wen erkannt? wohin?
  ├─ Entscheidung des Zeugen (Persönlichkeit, Angst, Interessen, Beziehung)
  │       melden | schweigen | zögern | erpressen | fliehen | eingreifen
  ├─ Meldung:  Notruf (Beschreibung: Kleidung, Fahrzeug, Kennzeichen, Richtung)
  └─ Folge:    Polizei kommt, Fahndung, Befragung (Ermittlung 14, 12)
```

**Zeugenentscheidung (Entwurf):**

| Faktor | Wirkung |
|---|---|
| Persönlichkeit | Mut, Gewissenhaftigkeit, Mitgefühl, Misstrauen gegenüber Polizei |
| Beziehung zum Täter | Loyalität, Angst, Schuld |
| Beziehung zum Opfer | Hilfsbereitschaft |
| Kosten | Zeitverlust, Gefahr, Rache |
| Gesellschaft | Umfeld, in dem Melden verpönt ist (‚petzen‘) |
| Alter, Gesundheit | Fähigkeit zu helfen |
| Rauschzustand | Beobachtungsqualität |

**Der Spieler als Zeuge** (N-WA-012): Er kann schweigen, melden, erpresst oder bedroht werden, Schweigegeld nehmen oder von Tätern verfolgt werden. Die Ermittlungs-Dokumentation führt die Konsequenzen ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §11). Hier gilt: **Die Bedrohung eines Zeugen kommt von einem NPC mit Zielen**, der selbst Wahrnehmung, Gedächtnis und Beziehungen hat (Modul 11, 16).

**Schweigen ist nicht Verschweigen von Wissen.** Ein schweigender Zeuge hat das Wissen und gibt es nicht preis; das Wissen bleibt im Datensatz (Modul 05).

---

## 8. Entwurf: Falsche Überzeugungen

Die Quellen verlangen falsche Überzeugungen ausdrücklich (N-GE-009), nennen aber keine Entstehungswege. Der Entwurf (💡) führt sie auf:

| Entstehungsweg | Beispiel | Korrektur |
|---|---|---|
| **Fehlwahrnehmung** | Dunkelheit, Entfernung, Stress | Rückfrage, Beweis |
| **Falsche Deutung** | ‚schwarzer Hund‘ statt Wesen (`23#51A`) | Beweis, Autorität, eigene Erfahrung |
| **Täuschung** | Täter legt falsche Spur | Ermittlung |
| **Gerücht** | Weitererzählt, verzerrt | Quelle prüfen |
| **Medien** | Zeitung, Social Media | Gegenbericht |
| **Weltbild** | Überzeugung, die zur Deutung zwingt | Erlebnis, Vertrauensperson |
| **Manipulation** | Lüge, Beeinflussung, übernatürlich | Aufdeckung |
| **Gedächtnisfehler** | Verschmelzen, Auffüllen | Erinnerungshilfe, Dokument |

**Korrigierbarkeit (Entwurf):** Eine Überzeugung ist umso schwerer zu ändern, je stärker sie emotional gebunden (Anker), öffentlich gemacht (Gesicht verlieren) oder von der Gemeinschaft gestützt ist. Sie lässt sich leichter ändern durch eine Quelle, der der NPC vertraut ([Modul 08](08-Beziehungen-Modell-und-Wirkung.md)), und durch einen Beweis, den er selbst prüfen kann.

**Kein Zwang zur Wahrheit:** Das Spiel braucht keine Instanz, die falsche Überzeugungen auflöst. Ein NPC kann bis zum Ende falsch liegen; dies kann sogar Teil eines Falls sein (Fall-Variante ‚das offensichtliche Monster war nicht die eigentliche Ursache‘, `23#51A` Abschnitt 11).

**Testszenario (M-P05, Entwurf 035):** Ein Prompt-Titel der Spur 05 nennt ein ‚false-belief investigation test scenario‘. Der Inhalt ist nur ein Titel; eine geeignete Prüfung steht in [Modul 29](29-Testkatalog-und-Abnahmekriterien.md) (T-WA-…).

---

## 9. Wahrnehmung des Übernatürlichen

Die Quellen verlangen eine Welt, in der das Übernatürliche **verborgen, selten und widersprüchlich dokumentiert** ist ([Kreaturen 02](../Kreaturen/02-Leitsaetze-und-Abgrenzung.md) K-GR-002) und das öffentliche Wissen **regional verschieden** (N-WA-016). Für NPCs ergibt das (📋 Richtung, Ausformulierung 💡):

| Aspekt | Entwurf |
|---|---|
| **Grundannahme** | Die Standardannahme ist zunächst etwas Normales (N-WA-017); die Polizei sucht nach gewöhnlichen Erklärungen |
| **Weltbild** | Jeder NPC hat ein Weltbild zum Übernatürlichen: Unglaube, Religion, Geister, Aliens, Verschwörung, Monster, Verdrängung (N-WA-018). Es ist Teil seiner Persönlichkeit und Region |
| **Deutung** | Eine übernatürliche Beobachtung wird nach dem Weltbild gedeutet (Modul 05 Szenario ‚Übernatürlicher Zeuge‘) |
| **Regionales Wissen** | Metropole: Legenden, Kleinstadt: lokale Geschichten, ländlich: Überlieferung, Hunter-Hochburg: Netzwerke, Krisengebiet: schwer zu verbergen (`48#148A`, Beispiele, keine Festlegung) |
| **Tiere** | reagieren teilweise früher (N-WA-019); NPCs, die Tiere beobachten, schließen daraus (Hinweis; `23#603A` Abschnitt 4) |
| **Dauerhafte Veränderung** | Wer ein Ereignis erlebt, kann dauerhaft verändert sein (N-WA-020) |
| **Verdrängung** | Manche verdrängen Erlebnisse (`23#1058A`); die Erinnerung wirkt im Verhalten, nicht in der Aussage (Modul 05 §11) |
| **Dokumentation** | NPCs fotografieren und filmen; Videos sind Beweis und Hinweis (`M-PD` ‚Smartphone / social media …‘); 99 % des Feeds sind Alltag |

Das Verhältnis zur Geheimhaltung des Spielers ist in [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md) behandelt.

---

## 10. Sonderfälle

1. **Zeuge im Rausch.** Schlechte Wahrnehmungsqualität; Aussage unsicher; kann falsche Details selbstsicher behaupten (Konfidenz hoch, Genauigkeit niedrig).
2. **Zeuge ist selbst Täter.** Seine Aussage ist Aussage, nicht Gedächtnis; sein Eintrag enthält die Wahrheit.
3. **Zwei Zeugen sehen dasselbe Ereignis unterschiedlich.** Normalfall (Perspektive, Zustand, Weltbild).
4. **Zeuge kann nicht mehr aussagen** (tot, bewusstlos, verhört, ausgesperrt). Das Wissen bleibt im Datensatz, ist aber unzugänglich ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §5, ‚Unfähig‘).
5. **Gruppe als Beobachter.** Eine Menschenmenge als Zeuge: Keine einzelnen Einträge, sondern ein Ortszustand mit Tendenz (‚viele haben gefilmt‘) und einzelne materialisierte Zeugen (Modul 22).
6. **KI und Kameras.** Überwachungskameras sind Wahrnehmungsquellen mit festem Sichtfeld (`23#1088A` Frage 51); ihr ‚Wissen‘ ist Aufzeichnung, nicht Erinnerung. Kameras gehören zum Beweissystem ([Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md)).
7. **Tier als Beobachter.** Ein Hund reagiert früher; ein Haustier ist kein Zeuge im Rechtssinn, aber ein Hinweisgeber.
8. **Besessener Zeuge.** Wahrnehmung und Erinnerung folgen den Regeln der Besessenheit ([Kreaturen 05](../Kreaturen/05-Geister-Besessenheit-und-Fluecher.md)).
9. **Falsche Erkennung als Verbrechen.** Wird der Spieler wegen Verwechslung verdächtigt, ist das ein Ermittlungsfall (Ermittlung 13 §7).
10. **Informanten.** Ein NPC kann dem Spieler absichtlich falsche Information geben; das ist Aussage, nicht Gedächtnis.

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-061 | Jede Aussage eines NPC über ein Weltereignis hat einen Eintrag mit Quelle | N-WA-010 |
| N-EN-062 | Ein Beobachter ohne Reichweite (Sicht, Gehör, Kontakt) erfährt nichts | N-WA-009, N-WA-010 |
| N-EN-063 | Die Wahrnehmung hängt von Licht, Entfernung, Ablenkung und Zustand ab und liefert bei gleichen Eingaben gleiche Ergebnisse | N-WA-009 |
| N-EN-064 | Kein NPC verwendet Information, die er nicht auf einem plausiblen Weg bekommen hat (kein Schwarmwissen) | N-WA-005, N-WA-006 |
| N-EN-065 | Die Gewissheit über Position und Einzelheiten nimmt ohne neue Hinweise ab | N-WA-004 |
| N-EN-066 | Verdacht steigt nur durch einen Anlass und sinkt ohne Bestätigung | Abschnitt 6 |
| N-EN-067 | Eine Straftat ohne Zeugen löst keine Fahndung aus | N-WA-013 |
| N-EN-068 | Die Polizei beginnt mit der gewöhnlichen Erklärung, solange das Übernatürliche nicht bekannt ist | N-WA-017 |
| N-EN-069 | Das Übernatürliche wird nach dem Weltbild des NPC gedeutet; verschiedene NPCs deuten verschieden | N-WA-018 |
| N-EN-070 | Ein falscher Glaube lässt sich nur durch ein Ereignis ändern, das im Journal steht | N-GE-009 |
| N-EN-071 | Ein öffentliches Ereignis macht nicht automatisch alle Beobachter zu Gläubigen | Abschnitt 4 |
| N-EN-072 | Eine Verkleidung ändert Erwartung und Verdacht, nie die Identität im Weltzustand | N-WA-014 |
| N-EN-073 | Kameras liefern Aufzeichnungen, keine Erinnerungen, und nur im Sichtfeld | Abschnitt 10 |
| N-EN-074 | Wahrnehmung ferner NPCs wird ereignisbasiert abgeleitet und widerspricht nie dem sichtbaren Verhalten bei der Materialisierung | Modul 22, `23#1057E` Punkte 122–123 |

---

## 12. Was fehlt

- Keine Sinnesmodelle, Radien, Winkel, Lautstärkeskalen (diese gehören zur Wahrnehmungs-KI des Spielsystems, `M-PD` ‚Stealth …‘).
- Offen, ob Zivilisten dasselbe vierstufige Wissensmodell nutzen (Entwurf) oder ein einfacheres.
- Keine Entscheidung zu Verdachtsschwellen und Wirkungen.
- Offen, wie weit Weltbilder festgelegt oder erzeugt werden (Region, Alter, Glaube).
- Die Rolle von Medienkonsum (Welche Nachrichten sieht ein NPC?) ist offen (Modul 07).
