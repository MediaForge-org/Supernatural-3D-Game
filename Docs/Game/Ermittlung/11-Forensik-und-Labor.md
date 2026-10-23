# Ermittlung 11: Forensik und Labor

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die Forensik als **Spielablauf**: wie eine Spur zur Probe wird, wie ein Labor arbeitet, was Ergebnisse aussagen dürfen und was sie nicht verraten dürfen. Der Master legt den **Katalog** der Forensik fest, aber **keine** Regeln, Zeiten oder Genauigkeiten (`M-OD`). Alles Konkrete hier ist **Entwurf** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Voraussetzungen: [Modul 05](05-Spuren-Tracking-Forensik-und-Zeugen.md) (Katalog), [Modul 10](10-Beweislogik-und-Wissensmodell.md) (Wissensmodell). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Was ausdrücklich offen ist](#2-was-ausdrücklich-offen-ist)
3. [Entwurf: Lebenslauf einer Probe](#3-entwurf-lebenslauf-einer-probe)
4. [Entwurf: Übergänge und Fehlerzustände](#4-entwurf-übergänge-und-fehlerzustände)
5. [Entwurf: Analysearten und ihre Aussagegrenzen](#5-entwurf-analysearten-und-ihre-aussagegrenzen)
6. [Entwurf: Form eines Ergebnisses](#6-entwurf-form-eines-ergebnisses)
7. [Entwurf: Laborarten und Zugang](#7-entwurf-laborarten-und-zugang)
8. [Entwurf: Zeit, Warteschlange und Zeitsprünge](#8-entwurf-zeit-warteschlange-und-zeitsprünge)
9. [Entwurf: Übernatürliche Proben und Anomalien](#9-entwurf-übernatürliche-proben-und-anomalien)
10. [Entwurf: Spuren des Spielers selbst](#10-entwurf-spuren-des-spielers-selbst)
11. [Entwurf: Autopsie und Tatortarbeit](#11-entwurf-autopsie-und-tatortarbeit)
12. [Sonderfälle](#12-sonderfälle)
13. [Abhängigkeiten](#13-abhängigkeiten)
14. [Was fehlt](#14-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| F1 | Forensik umfasst Fingerabdrücke, DNA, Ballistik, Schuh- und Reifenabdrücke, Fasern, Toxikologie, Todeszeit als Bereich, Autopsieberichte, Tatortarbeit, Labore mit Bearbeitungszeit | 📋 | `M-CA0905` Abschnitt 4; `23#1090A` |
| F2 | Fingerabdrücke: „realistisch genug, aber nicht an jedem Objekt der Welt simuliert“ | 📋 (Frage 80, „80-92 ja“) | `23#1088A`, `23#1089E` |
| F3 | DNA: Blut, Haare, Speichel usw. bei wichtigen Fällen | 📋 Richtung | Frage 81 |
| F4 | Reifenprofile bei Fahrzeugfällen | 📋 Richtung | Frage 84 |
| F5 | Toxikologie: Gifte, Medikamente, Drogen | 📋 Richtung | Frage 86 |
| F6 | Autopsie: Berichte erhalten oder teilweise Untersuchungen begleiten | 📋 Richtung | Frage 88 |
| F7 | Labore: professionelle Ergebnisse brauchen Zeit | 📋 | Frage 90 |
| F8 | Ergebnisse können unklar sein; kein Forensik-Knopf, der den Täter nennt | 📋 | Frage 91; `23#1090A` |
| F9 | Supernatürliche Anomalien können in DNA, Blut, Verletzungen erscheinen; normale Forensik kann zeigen, dass etwas physikalisch keinen Sinn ergibt | 📋 | Frage 92 und `23#1074A` Frage 83 |
| F10 | Monsterforschung umfasst Tatortarbeit (Blutspuren, Gewebe, Haare, Fingerabdrücke, Rückstände) und Laborforschung (Mikroskopie, chemische Analysen, DNA, Vergleiche mit bekannten Proben) als Möglichkeit | 📋 Richtung („was man sich vorstellen kann“) | `48#127E`; Ausformulierung `48#128A` (💡) |
| F11 | Forensik zielt auf Tiere mit (Kadaver, Aasfresser) | 📋 Wunsch | `M-PD` ‚Wildlife / study / harvesting / monsters‘ |
| F12 | Spieler dokumentiert Spuren und Proben im Beweisarchiv; „Forensik & Beweisarchiv“ als Werkzeug | 💡 Konzept | `48#62A` |

---

## 2. Was ausdrücklich offen ist

`M-OD` (‚Crime / law / evidence‘, Stand 31.08.2026) führt als offen:

- ‚Exact CCTV density/ownership/retention, fingerprint/DNA/ballistics/digital-evidence rules.‘
- ‚Exact suspect-confidence and evidence-scoring model.‘
- ‚Exact warrant/search/seizure/vehicle-search rules.‘

Daraus folgt: Es gibt **keine** Regel, welche Gegenstände Spuren tragen, wie lange eine Analyse dauert, wie sicher ein Ergebnis ist, wie groß Datenbanken sind oder wie viele Kameras es gibt. Der Entwurf unten bietet eine **Struktur**, in die man diese Werte später einsetzen kann.

---

## 3. Entwurf: Lebenslauf einer Probe

> [!NOTE]
> **Nur Vorschlag (💡).** Die Zustände sind Arbeitsnamen.

```text
[Unentdeckt] -> [Entdeckt] -> [Dokumentiert] -> [Gesichert] -> [Transportiert]
                                                                      |
   [Verloren/Zerstört] <- (jederzeit)                    [Eingereicht/In Warteschlange]
   [Kontaminiert]      <- (jederzeit)                                 |
   [Verdorben]         <- (Zeit)                              [In Analyse]
                                                                      |
                                                              [Ergebnis liegt vor]
                                                                      |
                                                       [Gesichtet] -> [Archiviert]
```

| Zustand | Bedeutung | Spielerhandlung |
|---|---|---|
| **Unentdeckt** | Spur existiert in der Welt, der Spieler weiß nichts | – |
| **Entdeckt** | Spieler sieht sie (Fokus, Zufall, Suche) | Untersuchen |
| **Dokumentiert** | Foto, Notiz, Position; noch nicht entnommen | Fotografieren, Skizzieren |
| **Gesichert** | entnommen oder abgenommen (Beutel, Abdruck) | Sichern |
| **Transportiert** | unterwegs (Fahrzeug, Tasche); Verderb und Kontamination möglich | Fahren, Lagern |
| **Eingereicht** | im Labor angekommen, Auftrag ist aufgegeben | Abgeben |
| **In Analyse** | Labor arbeitet (Zeit läuft) | Warten |
| **Ergebnis liegt vor** | Befund steht bereit | Abholen, Erhalten |
| **Gesichtet** | Spieler hat den Befund gelesen; er wird zum Beleg im Wissensstand | Lesen |
| **Archiviert** | im Beweisarchiv geführt | – |

Fehlerzustände (**Verloren**, **Zerstört**, **Kontaminiert**, **Verdorben**) können zu jedem Zeitpunkt eintreten (Abschnitt 4).

---

## 4. Entwurf: Übergänge und Fehlerzustände

| Von | Nach | Auslöser | Bedingung | Wirkung |
|---|---|---|---|---|
| Unentdeckt | Entdeckt | Spieler | Suche mit Sinn oder Werkzeug; Zufall | Beleg-Kandidat erscheint im Wissen (nicht zwingend im Archiv) |
| Entdeckt | Dokumentiert | Spieler | Foto oder Notiz | Spuren-Eintrag mit Ort und Zeit |
| Dokumentiert | Gesichert | Spieler | Werkzeug vorhanden (zum Beispiel Beutel, Pinsel, Abdruckmasse) | Probe im Inventar; Beweiskette beginnt ([Modul 10](10-Beweislogik-und-Wissensmodell.md#9-entwurf-provenienz-und-beweiskette)) |
| Dokumentiert | Zerstört | Welt | Wetter, Tiere, Menschen, Zeit | Spur nicht mehr sicherbar; Foto bleibt |
| Gesichert | Kontaminiert | Spieler/Welt | falsche Handhabung, falsche Lagerung, Vermischung | Ergebnis unbrauchbar oder verfälscht |
| Gesichert | Verdorben | Zeit | Probenart verderblich (Blut, Gewebe, Fleisch; `M-PD` ‚moderate sample/meat spoilage‘) | Aussagekraft sinkt |
| Gesichert | Transportiert | Spieler | bewegt die Probe | Zeitverlauf läuft weiter |
| Transportiert | Eingereicht | Spieler | Labor erreichbar und willig | Auftrag; Kosten möglich (offen) |
| Eingereicht | In Analyse | Labor | Auftrag angenommen | Zeitzähler beginnt |
| In Analyse | Ergebnis | Labor | Zeit abgelaufen | Befund erzeugt (Abschnitt 6) |
| beliebig | Verloren | Welt/Spieler | Diebstahl, Verlust, Beschlagnahme | Probe weg; Foto, Notiz bleiben |
| Ergebnis | Gesichtet | Spieler | Befund geöffnet | Beleg tritt in den Wissensstand ein |

**Regel (Vorschlag):** Ein Fehlerzustand ist **für den Spieler nicht immer erkennbar**. Ein kontaminiertes Ergebnis darf unauffällig falsch sein (Abschnitt 6, „Zuverlässigkeit“), sonst wäre es automatisch erkennbar und damit ein Hinweis. Das System entscheidet dies pro Fall; die Quellen sagen dazu nichts.

---

## 5. Entwurf: Analysearten und ihre Aussagegrenzen

Die Analysearten sind beschlossen (F1). Entscheidend für die Spielwirkung ist, **was sie nicht sagen**. Der Entwurf legt Aussagegrenzen fest, damit kein ‚Täter-Knopf‘ (F8) entsteht.

| Analyseart | Kann aussagen | Darf nicht aussagen | Notwendige Vergleichsdaten |
|---|---|---|---|
| **Fingerabdruck** | Abdruck ist brauchbar oder nicht; stimmt mit Referenz überein; Alter nur grob | den Täter ohne Vergleich benennen; Besitzer unbekannter Abdrücke | Referenz (bekannte Person, Registerabdruck) |
| **DNA** | Spender gleich oder verschieden von Referenz; Verwandtschaft; Spezies-Indizien; Anomalie | den Namen eines Unbekannten; Zeitpunkt der Spur | Referenz, Datenbank (Abdeckung offen) |
| **Ballistik** | Waffentyp, Kaliber, ob zwei Geschosse aus derselben Waffe stammen | den Schützen | Waffe oder Vergleichsgeschoss |
| **Schuhprofil** | Modell, Größe, Abnutzung | den Träger | Vergleichsschuh, Katalog |
| **Reifenprofil** | Reifentyp, Fahrzeugklasse (Abschnitt 5 in [Modul 05](05-Spuren-Tracking-Forensik-und-Zeugen.md)) | den Fahrer | Fahrzeug, Katalog |
| **Fasern** | Material, Herkunftsklasse | den einzelnen Stoff ohne Vergleich | Vergleichsstoff |
| **Toxikologie** | Stoffe, Mengenbereich, Hinweis auf Zeitrahmen | den Täter; Absicht | Referenzstoffe |
| **Todeszeit** | **Bereich** (`M-CA0905`: ‚time-of-death ranges‘), nie ein Zeitpunkt | genaue Minute | Umgebung (Temperatur, Zustand) |
| **Autopsie** | Todesursache (wahrscheinlich/unsicher), Verletzungsmuster, Anomalien | Täteridentität; Gewissheit bei Unklarem | Leichnam, Fachkenntnis |
| **Mikroskopie, Chemie** (Monsterforschung, `48#128A`) | Zusammensetzung, Auffälligkeiten | die Spezies ohne Referenz | Referenzproben im Archiv |
| **Tatortarbeit** | Verteilung, Rekonstruktion, Spurenreihenfolge | die Wahrheit direkt | Tatort in unverändertem Zustand |

### 5.1 Regel der Referenzen (Entwurf)

> **Ein Labor nennt nie ein Ergebnis, für das der Spieler keine Referenz geliefert hat.**

Das setzt `F8` um: Fingerabdruck- und DNA-Vergleiche liefern ‚Übereinstimmung mit Referenz R‘ oder ‚keine Übereinstimmung‘, nicht den Namen einer unbekannten Person. Wo das Spiel NPCs mit Akten hat, kann die Polizei Referenzen aus Registern liefern. Der Spieler erhält sie nur über **Beziehungen und Zugang** (Abschnitt 7). Die Größe der Register ist offen (`M-OD`).

### 5.2 Regel der Artenreferenz (Entwurf)

Bei Monsterproben gilt entsprechend: Eine Probe wird nicht mit „Werwolf“ beschriftet, es sei denn, der Spieler besitzt eine **Referenzprobe oder Referenzbeschreibung** im Archiv, die das Ergebnis erklärt. Sonst lautet der Befund „Anomalie“ (Abschnitt 9). Das ist eine Folgerung aus `M-PD` (‚Hunter Archive … Confirmed/Suspected/Disputed‘) und `48#126A` (Forschung statt Freischaltung).

---

## 6. Entwurf: Form eines Ergebnisses

Ein Befund hat **keine** Zahlen als Pflicht (die Quellen nennen keine). Eine mögliche Form (💡):

| Feld | Inhalt |
|---|---|
| Gegenstand | Welche Probe, welche Analyse |
| Aussage | Text in Fachsprache, so wie ein Labor schreibt |
| Gewissheit | Wortstufen statt Prozent: eindeutig / wahrscheinlich / möglich / unbestimmt / widersprüchlich |
| Anomalie | ja/nein; falls ja, nur Beschreibung (‚ungewöhnliche Struktur‘), nie die Ursache |
| Vorbehalte | Zustand der Probe (zum Beispiel stark verdorben), Menge, Qualität |
| Quelle | Labor, Bearbeiter, Zeit |
| Empfehlung | optional: Folgeanalyse vorschlagen (nur in Hilfe-Stufen, die es erlauben, A1/A6) |

**Zuverlässigkeit.** Ein Befund kann falsch sein (Kontamination, Fehler, Bestechung, Manipulation). Die Wortstufe bezieht sich auf das, was das Labor **meint**. Ob sie die Wahrheit trifft, hängt von den Fehlerzuständen ab (Abschnitt 4). Im Detektivmodus gibt es **keine** Markierung ‚dieser Befund könnte fehlerhaft sein‘; das Spiel sagt nur, was das Labor schrieb (Konsequenz aus `23#55A`).

**Formulierung.** Ergebnisse sollen **Fachtexte** sein, die man als Spieler lesen und deuten muss. Die Prompt-Sammlung kennt einen Beweistyp „Dokument“ (Titel 015). Ein Befund ist ein Dokument im Wissensstand.

---

## 7. Entwurf: Laborarten und Zugang

Die Quellen nennen **Labore mit Bearbeitungszeit** (F7), und Monsterforschung im Hunter-Kontext umfasst Laborarbeit (F10), nennen aber nicht, **wer** das Labor betreibt. Ein Entwurf möglicher Laborarten:

| Laborart (Arbeitsname) | Betreiber | Zugang | Eigenschaften (Vorschlag) |
|---|---|---|---|
| **Feldtest** | Spieler | Ausrüstung im Inventar | schnell, grob, wenige Analysearten; kein Gerichtswert |
| **Polizei-/Landeslabor** | Behörde | über Beziehung, Anzeige oder Auftrag | gerichtsverwertbar; Warteschlange; Zuständigkeit nach Behörde ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)); Einfluss von Korruption |
| **Gerichtsmedizin** | Behörde | über Zuständigkeit | Autopsie, Todeszeit, Toxikologie |
| **Privatlabor** | Unternehmen | Bezahlung | schneller, teurer, weniger Fragen; ggf. fragwürdig |
| **Hunter-Labor / HQ** | Spieler/Crew | Ausbau im HQ | ausbaubar; Spezialanalysen für Monsterproben; Detail abhängig vom Bunker (eigenes Planungsthema, `48#131E`: „der bunker muss noch einzeln geplant werden“) |
| **Staatliche Spezialeinheit** | geheime Behörde | über Kontakte oder Story | kennt das Übernatürliche (teilweise); `48#171E`, `48#173E` |

Alle Zugänge sind **Vorschläge**. Beschlossen ist nur, dass die Behörden zu verschiedenen Zuständigkeiten gehören (Option D der Frage `48#154A`, gewählt mit `48#155E`) und dass geheime Forschungseinrichtungen existieren (`48#171E`, `48#175E`).

---

## 8. Entwurf: Zeit, Warteschlange und Zeitsprünge

| Frage | Vorschlag |
|---|---|
| In welcher Zeit läuft die Analyse? | **Spielzeit.** Der Spieler kann etwas anderes tun oder schlafen. |
| Wird die Dauer ein Fallhindernis? | Ja, Dauer ist Teil der Fallgestaltung (`23#1088A` Frage 90: „Professionelle Ergebnisse benötigen Zeit“). Konkrete Dauer **offen** (Q-E44). |
| Gibt es Warteschlangen? | Mögliche Folge von Last und Priorität (nicht beschlossen). Ein Fall kann ein Labor „belegen“. |
| Wie wirken Zeitsprünge? | Analysen laufen weiter; bei Haft (`48#159E`) bleibt die Welt dynamisch, daher können Ergebnisse eintreffen, ohne dass der Spieler anwesend ist. |
| Wie wirken Zeitsprünge im Koop? | Nur mit Zustimmung (`48#163E`); Ergebnisse treffen dann gemeinsam ein. |
| Kann das Labor „abbrechen“? | Möglich: Überlastung, Raub, Brand, Beamte werden versetzt. Das System muss das als Zustandswechsel (**Eingereicht → Verloren**) abbilden. |
| Kann die Dauer abgekürzt werden? | Durch Geld, Beziehungen, Korruption (Option D der Justiz, `48#155E`) oder eigenes Labor. |

**Rückmeldung.** Der Spieler muss erfahren, **dass** ein Ergebnis da ist, über einen diegetischen Weg (Anruf, Brief, E-Mail, persönlich). Ob es dafür Hilfen gibt (A1, Modul 08) ist ein Detail; im Detektivmodus gibt es keine automatische Benachrichtigung außer Weltkanälen (Handy, Post), die zum Alltag gehören.

---

## 9. Entwurf: Übernatürliche Proben und Anomalien

### 9.1 Was beschlossen ist

Forensik kann eine **Anomalie** zeigen (F9). Mythologieabhängige Verletzungen, Flüche und Behandlungsmethoden sollen vom Spieler erst verstanden werden (`48#123E`; Ausformulierung `48#124A`). Das passt zur Regel, dass die Mythologie jeder Art ihre eigenen Wirkungen hat.

### 9.2 Entwurf: Arten von Anomalien

| Anomalie (Arbeitsname) | Beispiel (erfunden) | Wirkung auf Wissen |
|---|---|---|
| Strukturanomalie | Probe zeigt Merkmale, die keiner bekannten Art entsprechen | Hinweis ‚nicht gewöhnlich‘, keine Art |
| Zeitanomalie | Blut ist älter oder jünger, als es sein kann | Hinweis auf Manipulation oder Übernatürliches |
| Mengenanomalie | Zu wenig/zu viel Blut für die Wunde | Hinweis auf Besonderheit |
| Verhaltensanomalie | Probe zerfällt bei Licht, verändert sich im Kontakt | Hinweis auf Mythologie (Silber, Licht, Salz) |
| Verschwindende Probe | Probe löst sich auf oder wechselt den Zustand | Verlust, aber auch Hinweis |
| Widersprüchliche Probe | Zwei Analysen derselben Probe ergeben Verschiedenes | bewusst unklar |
| Wirksamkeit von Mitteln | Probe reagiert auf Substanz | Hinweis auf wirksame Methode (Forschung, `48#127E`) |

Alle sind **Beispiele**; die tatsächliche Liste hängt von der Monster-Mythologie ab, die erst festgelegt werden muss (`48#124A` selbst: „Welche Effekte tatsächlich zu welcher Kreatur gehören, legen wir anhand unserer endgültigen Monster-Mythologie fest.“).

### 9.3 Regel (Entwurf)

> **Anomalien zeigen, dass etwas nicht stimmt, aber nie, was es ist.** Die Art wird erst durch Forschung, Archiv, Zeugen und Tests erkannt (`48#125E`).

### 9.4 Experimente

Die Quellen erlauben praktische Tests (Waffen, Substanzen, Rituale, Fallen) an Kreaturen und Proben (`48#127E`). Als Mechanik bedeutet das: Eine Probe oder Kreatur wird einer Substanz ausgesetzt, die Reaktion wird beobachtet und als **Beleg** festgehalten. Eine falsche Hypothese kann gefährlich sein (`48#128A`, 💡: „Fehlgeschlagene Versuche können gefährlich sein“). Wie sicher oder wiederholbar Experimente sind, ist offen (Q-E45).

---

## 10. Entwurf: Spuren des Spielers selbst

Weil die Polizei „nicht allwissend“ ist (`48#156A`, `M-PD`), muss das System entscheiden, welche **Spuren der Spieler hinterlässt**. Der Master sagt dazu nur, dass Fingerabdrücke und DNA „wo passend“ Beweisquellen sind (`M-PD`).

| Frage | Vorschlag |
|---|---|
| Hinterlässt der Spieler Spuren? | Ja, wo sie passen (Tatorte, Einbruch, Gewalt). Sie sind ein Ereignis in der Welt, nicht automatisch ein Treffer. |
| Wird ein Treffer ausgelöst? | Nur, wenn die Polizei eine **Referenz** vom Spieler hat (zum Beispiel nach einer früheren Festnahme, Verhör, Arbeitsstelle). Sonst bleibt die Spur „unbekannt“. |
| Wie kann der Spieler Spuren vermeiden? | Handschuhe, Kleidung, Reinigung, Vorsicht; kostet Zeit und Aufmerksamkeit (kein Zwang, `23#55A`-Philosophie ‚kein Mikromanagement‘ ist hier nicht festgelegt) |
| Hinterlassen Begleiter Spuren? | Ja, Regel wie für den Spieler. |
| Wie wirken Formwandler? | Sie können Spuren erzeugen, die auf den Spieler zeigen (`48#154A`, Szenarioideen des Assistenten); siehe [Modul 13](13-Identitaet-Ruf-und-Formwandler.md). |

Die Frage, **wie viele Referenzdaten** es über den Spieler gibt und was zur Referenz wird, ist ungeklärt (Q-E46). Diese Frage entscheidet, ob ein früher Fehler später zu Treffern führt (`48#156A`: „Langzeitfolgen“).

---

## 11. Entwurf: Autopsie und Tatortarbeit

### 11.1 Autopsie

Beschlossen: Der Spieler kann **Berichte erhalten** oder **teilweise Untersuchungen begleiten** (F6). Mögliche Abstufung (💡):

| Stufe | Beschreibung |
|---|---|
| Bericht lesen | Befund als Dokument, Ergebnis nach Zeit |
| Gespräch mit dem Gerichtsmediziner | Fragen stellen, Rückfragen; Person hat Wissen und Grenzen |
| Teilnahme | Der Spieler sieht und kommentiert; Mitwirkung nur wo erlaubt (Behörden) |
| Eigene Untersuchung | Voraussetzung: Zugang zum Leichnam, Wissen, Werkzeug (legal/illegal) |

Die Quellen führen **keine** Mini-Spiele auf. Ob Autopsien spielbar sind, ist offen (Q-E47).

### 11.2 Tatortarbeit

Tatortarbeit (`M-CA0905`: ‚scene work‘) bedeutet, einen Tatort zu betrachten, zu dokumentieren und zu sichern. Beschlossen ist die Arbeit selbst, nicht die Oberfläche. Entwurfsgedanken:

1. **Reihenfolge zählt.** Wer zuerst Blut berührt und danach fotografiert, hat den Tatort verändert (Beweiskette).
2. **Polizei am Tatort.** Wenn die Polizei da ist, ist der Tatort abgesperrt (`23#1056A` Frage 50, „ja“); der Spieler braucht Zugang (Beziehung, Ausweis, Täuschung, Einbruch).
3. **Mehrere Personen.** Im Koop können mehrere Hunter Aufgaben teilen (`48#91E`: geteilte Ermittlungsakten).
4. **Zeitdruck.** Spuren verfallen (Wetter), Tatorte werden gereinigt (Frage 150).

---

## 12. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Polizei hat die Probe bereits | Beweis ist beim Labor der Behörde | Spieler braucht Zugang oder eigene Probe |
| Doppelte Analyse | Spieler reicht dieselbe Probe zweimal ein | gleiches Ergebnis (Determinismus), eventuell Hinweis auf bereits Eingereichtes |
| Ergebnis ändert sich | Datenbank wird erweitert, neue Referenz | Neuanalyse möglich; alte Befunde bleiben als Dokument |
| Labor bestochen | Befund absichtlich falsch | Befund ist zuverlässig auf dem Papier; Widerspruch später erkennbar (Korruption, Modul 14) |
| Probe vom Monster | Spieler besitzt Probe eines bekannten Wesens | Referenz im Archiv möglich; Wissen wächst (`48#126A`) |
| Probe aus fremder Realm | Hölle/Fegefeuer/Himmel | offen |
| Verderb im Fahrzeug | Probe im Kofferraum | Fahrzeug kann Kühlung haben (Teil Fahrzeugausstattung, offen) |
| Spieler besitzt kein Gerät | Feldtest nicht möglich | Weg zur Behörde oder Labor |
| Tier statt Mensch | Forensik an Tieren und Kadavern | `M-PD` Wunsch; Studienhandlung nötig (`M-PD`) |
| Probe ist illegal beschafft | Einbruch, Leichenschändung | Strafrecht ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)); Gericht nimmt Beweis eventuell nicht an |

---

## 13. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Befund wird Beleg mit Status; Provenienz und Kontamination |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Labor kann Aussagen stützen oder widerlegen |
| Polizei ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) | Zugang, Zuständigkeit, Register, Korruption |
| Gerichte ([Modul 15](15-Gerichte-und-Verfahren.md)) | Verwertbarkeit, Beweiskette |
| Monsterwissen (Archiv) | Referenzproben, Mythologie |
| Fahrzeuge | Transport, Lagerung (Kühlung) |
| Inventar | Proben als Gegenstände mit Zustand (Verderb); begrenzter Platz (`M-CA0905`) |
| Wirtschaft | Kosten für Labore, Gerät (offen) |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Proben, Aufträge, Ergebnisse sind Spielstand; deterministische Ergebnisse |
| Koop ([Modul 18](18-Koop-Ermittlung.md)) | geteilte Beweisakten, Zeitsprung nur mit Zustimmung |

---

## 14. Was fehlt

- **Zahlen**: Dauer, Genauigkeit, Kosten, Verderbzeiten (Q-E44).
- **Datenbanken**: wer ist registriert, was ist Referenz (Q-E46).
- **Spielbarkeit** von Autopsie und Tatortarbeit (Q-E47).
- **Laborzugang** und Ausbau (Bunker/HQ ist ein eigenes Planungsthema).
- **Fehlerquoten** und deren Sichtbarkeit.
- **Mythologie-abhängige Anomalien**, abhängig von der Artenliste.
- Die **Experimentregeln** (Q-E45).
