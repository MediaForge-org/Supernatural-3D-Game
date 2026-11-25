# Ermittlung 14: Polizei, Behörden und Fahndung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul vertieft [Modul 06](06-Kriminalitaet-Polizei-und-Justiz.md) für den Teil **Strafverfolgung**: wie Behörden von Taten erfahren, wie eine Ermittlung gegen den Spieler (oder gegen NPCs) entsteht und endet und wie Fahndung, Durchsuchung, verdeckte Ermittlung und Korruption zusammenspielen. Beschlossen sind die Richtung (vollständig dynamische Strafverfolgung, Option D) und die Grundsätze. Zustände, Stufen und Abläufe sind **Entwurf** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Konflikt: „Fahndungsstufen“ und ‚kein Sternesystem‘](#2-konflikt-fahndungsstufen-und-kein-sternesystem)
3. [Entwurf: Behörden und Zuständigkeiten](#3-entwurf-behörden-und-zuständigkeiten)
4. [Entwurf: Der Weg einer Tat zur Behörde](#4-entwurf-der-weg-einer-tat-zur-behörde)
5. [Entwurf: Zustandsmaschine einer Polizeiakte](#5-entwurf-zustandsmaschine-einer-polizeiakte)
6. [Entwurf: Fahndungsintensität statt Sterne](#6-entwurf-fahndungsintensität-statt-sterne)
7. [Entwurf: Begegnungen mit der Polizei](#7-entwurf-begegnungen-mit-der-polizei)
8. [Entwurf: Durchsuchung, Beschlagnahme und Haftbefehl](#8-entwurf-durchsuchung-beschlagnahme-und-haftbefehl)
9. [Entwurf: Verdeckte Ermittlung und Informanten](#9-entwurf-verdeckte-ermittlung-und-informanten)
10. [Entwurf: Korruption und Einflussnahme](#10-entwurf-korruption-und-einflussnahme)
11. [Entwurf: Polizei und das Übernatürliche](#11-entwurf-polizei-und-das-übernatürliche)
12. [Entwurf: Verfolgung, Straßensperren, Hunde](#12-entwurf-verfolgung-straßensperren-hunde)
13. [Entwurf: Simulationsstufen und Zeit](#13-entwurf-simulationsstufen-und-zeit)
14. [Koop](#14-koop)
15. [Sonderfälle](#15-sonderfälle)
16. [Abhängigkeiten](#16-abhängigkeiten)
17. [Ergänzungen aus der Quellenauswertung](#17-ergänzungen-aus-der-quellenauswertung)
18. [Ergänzung: Kontrollen und Warn-Apps](#18-ergänzung-kontrollen-und-warn-apps)
19. [Was fehlt](#19-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| P1 | Gesetz folgt tatsächlicher Information und Beweiskonfidenz über Zuständigkeiten hinweg, nicht omniszienter Fahndungslogik | 📋 | `M-PD` ‚Cash / law / evidence‘ |
| P2 | Beweisquellen: Zeugen, Kameras, Kleidung, Gesicht, Fahrzeug, Kennzeichen, Waffen, Ballistik, Fingerabdrücke, DNA, digitale Beweise | 📋 | `M-PD` |
| P3 | Aufgeben und Festnahme sind echte Alternativen zur tödlichen polizeilichen Lösung | 📋 | `M-PD` |
| P4 | Polizei-K9 und ein wiederverwendbares Geruchs- und Windmodell sind gewünscht | 📋 Wunsch | `M-PD` |
| P5 | Durchsuchung, Beschlagnahme, Fahrzeugdurchsuchung nach Regeln; Beweiskette und Aufräumen | 📋 Wunsch | `M-PD`; `M-B39` (Themenliste) |
| P6 | Mehrere Behörden (City Police, County Sheriff, State Police/Highway Patrol, Rangers), unterschiedliche Zuständigkeiten | 📋 Richtung | `23#1057E` Punkte 57–58 |
| P7 | Notruf mit Meldelatenz; Täterbeschreibung (Kleidung, Fahrzeug, Kennzeichen, Richtung); Kennzeichenabfrage | 📋 | Punkte 60–62 |
| P8 | Verkehrskontrolle statt sofortiger Verfolgung; Eskalation hängt vom Spieler ab; auf frischer Tat kann beides sein | 📋 | Punkte 63–64 |
| P9 | Schwere Fahndung bleibt bis zur glaubwürdigen Auflösung; landesweit gesucht werden ist möglich | 📋 | Punkte 55, 65 |
| P10 | Detectives bei schweren Delikten; Verhör bei Verdacht | 📋 | Punkte 59, 66 |
| P11 | Option D der Strafverfolgung: **vollständige dynamische Strafverfolgung** mit regionalen Zuständigkeiten, verdeckten Ermittlungen, Identitätsdiebstahl, Beweismanipulation, Korruption und langfristigen Konsequenzen | 📋 | `48#154A`, `48#155E` |
| P12 | Die Polizei ist nicht allwissend; sie kommt durch Hinweise, Zeugenaussagen und Ermittlungen zum Spieler; falsche Schlussfolgerungen sind möglich | 💡 des Assistenten, deckt sich mit P1 | `48#156A` |
| P13 | Die Polizei erklärt Vorfälle zunächst normal | 📋 | Punkt 116 („ja so wie in der serie“) |
| P14 | Kleine geheime Einheiten kennen Teile der Wahrheit | 📋 | `23#1075E` Punkt 21 |
| P15 | Gesellschaft reagiert dynamisch auf Übernatürliches (Behörden, Medien, Politik, Vertuschung, organisierte Gegenmaßnahmen) | 📋 | `48#145E` zu `48#144A` Option D |
| P16 | Polizei-Minikarte und -HUD richten sich nach der Schwierigkeit | 📋 Richtung | `M-PD`, `M-B39` |
| P17 | Nach größeren NPC-Verbrechen können Orte abgesperrt sein | 📋 | Punkt 50 |
| P18 | Schwere Verbrechen werden nur über Konsequenzsysteme behandelt; keine eigene Mechanik | 📋 | `35#533A`; Adult 09 |
| P19 | NPC-Kriminelle werden von der Polizei verfolgt; NPCs reagieren (rufen die Polizei, greifen ein) | 📋 | Block 8K Frage 98; Block 8F Frage 49 |

Zusätzlich in `M-OD` (31.08.2026) **offen** geführt: Straftatenkatalog, Festnahmeschwellen, Strafen, Kaution; Zuständigkeitskarte und Informationsaustauschverzögerungen; Verdachts- und Beweiswertungsmodell; Kameradichte, Eigentümer, Aufbewahrung; Fingerabdruck/DNA/Ballistik/Digitalbeweis-Regeln; Geruchs- und Windmodell des Hundes; Haftbefehl, Durchsuchung, Beschlagnahme, Fahrzeugdurchsuchung; Polizeiminikarte je Schwierigkeit.

---

## 2. Konflikt: „Fahndungsstufen“ und ‚kein Sternesystem‘

| Aussage A | Aussage B |
|---|---|
| `M-PD`: ‚Strafsystem statt Sterne‘ (Überblick in Gameplay-Systeme §10); `23#1058A`: „Kein magisches GTA-Sternesystem.“ | `48#154A` Option A „GTA-ähnlich: Fahndungsstufen, Polizeiverfolgungen und Fluchtmöglichkeiten“; die Option D sagt „Alles Genannte plus …“; `48#156A` führt in der Tabelle „Fahndungsstufen“ unter Polizeiverfolgungen |

Der Entwickler wählte D (`48#155E`) ohne Kommentar zu den Stufen. Die Aufzählung `48#156A` ist eine Ausformulierung des Assistenten. Es ist **nicht festgelegt**, ob „Fahndungsstufen“ ein sichtbares Spielelement oder ein interner Begriff sind. Der Widerspruch ist in [Modul 21](21-Konflikte-und-offene-Fragen.md) als K-E8 geführt. Der Entwurf in Abschnitt 6 löst ihn **ohne Entscheidung** auf, indem er die Intensität intern führt und ohne Sterne darstellt.

---

## 3. Entwurf: Behörden und Zuständigkeiten

> [!NOTE]
> **Nur Vorschlag (💡).** Die Quellen nennen die Behörden (P6), aber keine Zuständigkeitsregeln (`M-OD`).

| Behörde (Arbeitsname) | Zuständigkeit (Vorschlag) | Besonderheiten |
|---|---|---|
| **City Police** | Stadtgebiet | größte Dichte, schnelle Reaktion, viele Kameras (Dichte offen) |
| **County Sheriff** | Landkreis außerhalb der Städte; Gefängnis des Kreises | in Kleinstädten oft die einzige Behörde; persönliche Beziehungen wirken stark (`23#52E` Punkt 9, Beispiel Sheriff) |
| **State Police / Highway Patrol** | Straßen, Staat, schwere Delikte, Unterstützung der Kommunen | Verkehrskontrollen, Verfolgungen |
| **Rangers** (Parks, Wald) | Naturgebiete, Jagd, Camping | verknüpft mit Wildnis, Jagd, Suchaktionen |
| **Bundesbehörde** | zwischenstaatliche und schwere Delikte | die Quellen nennen „Bundesbehörden“ (`48#156A`) und „landesweit gesucht“ (Punkt 55) |
| **Spezialeinheiten** | geheime Einheiten, die Teile der Wahrheit kennen | P14; Modul 17 |
| **Privatsicherheit** | privates Gelände, Firmen | nicht in den Quellen als Polizei; als Wirtschaftsfaktor möglich |

### 3.1 Zuständigkeit und Übergabe (Entwurf)

| Situation | Vorschlag |
|---|---|
| Tat im Stadtgebiet | City Police ist zuständig; bei schweren Delikten kommt State oder Bundesbehörde hinzu |
| Tat auf dem Land | Sheriff; State Police auf Straßen |
| Tat über mehrere Bereiche | Zuständigkeiten gehen über (Verfolgungen, Verbrechen mit Bewegung) |
| Informationsaustausch | **verzögert** (`M-OD`: ‚information-sharing delays‘); neue Information braucht Zeit, bis sie in anderen Bereichen ankommt |
| Konkurrenz | Behörden können sich blockieren oder ergänzen (Option D: Zuständigkeiten) |

**Folge für das Spiel:** Der Spieler kann sich einer Fahndung durch **Ortswechsel** teilweise entziehen, aber nur solange die Information nicht weitergegeben ist (Abschnitt 4). Landesweite Fahndung (P9) ist die Ausnahme für schwere Delikte.

---

## 4. Entwurf: Der Weg einer Tat zur Behörde

Das Prinzip ‚Polizei ist nicht allwissend‘ (P1, P12) verlangt eine Kette von Informationen. Entwurf:

```text
Tat
 -> Wahrnehmung (Zeuge, Kamera, Sensor, Opfer)            [Modul 12, Modul 10]
 -> Meldung (Notruf, Anzeige, Funk, Medien)                [Latenz, P7]
 -> Aufnahme (Disponent, Beamter)                          [Qualität der Meldung]
 -> Einsatz (Streife, Absperrung)                          [Reaktionszeit]
 -> Spurensicherung                                        [Modul 11]
 -> Ermittlung (Zeugen, Kameras, Kennzeichen, Labor)       [Dauer]
 -> Verdacht (Konfidenz gegen Person)                      [offen: Modell]
 -> Fahndung (regional/landesweit)                         [Zuständigkeit]
 -> Haftbefehl / Festnahme                                 [Abschnitt 8]
```

| Schritt | Entscheidend (Vorschlag) | Quelle der Anforderung |
|---|---|---|
| Wahrnehmung | nur, was beobachtet wurde; Kamera nur, wo vorhanden; Qualität; Beobachter erkennt oder nicht (Z6) | P1, Z3, Z6 |
| Meldung | Zeuge muss melden wollen und können; Latenz | P7 |
| Aufnahme | Beschreibung kann ungenau sein (Kleidung, Fahrzeug, Kennzeichen, Richtung) | P7 |
| Einsatz | Reaktionszeit hängt von Dichte, Entfernung, Zuständigkeit, Last ab | – (offen) |
| Ermittlung | Kameras, Kennzeichen, Labor mit Dauer | P2, Modul 11 |
| Verdacht | **Konfidenz** (`M-PD`), die wächst, wenn Belege sich verdichten | P1 |
| Fahndung | beginnt mit Konfidenz gegen eine konkrete Person oder Beschreibung | P9 |
| Festnahme | nach Beweislage; auf frischer Tat sofort | P8 |

**Regel (Entwurf):** *Jeder Schritt kann scheitern.* Ein Zeuge meldet nicht, eine Kamera fehlt, die Beschreibung stimmt nicht, die Spur verfällt, das Labor ist langsam. So hat der Spieler Spielraum (Verwischen von Spuren, Zeugen beeinflussen), und die Polizei bleibt nicht allwissend.

---

## 5. Entwurf: Zustandsmaschine einer Polizeiakte

> [!NOTE]
> **Nur Vorschlag (💡).** Jede Tat, die eine Behörde bearbeitet, wird eine **Akte**. Die Zustände sind Arbeitsnamen.

```text
[Unbekannt] -> [Gemeldet] -> [In Ermittlung] -> [Verdächtiger benannt] -> [Fahndung]
                                  |                      |                    |
                                  v                      v                    v
                              [Kalt]            [Entlastet/Falsch]   [Haftbefehl] -> [Festgenommen]
                                  |                                          |             |
                                  v                                          v             v
                          [Wieder aufgenommen]                          [Untergetaucht]  [Anklage]
                                                                                              |
                                                                                    [Verfahren] -> [Urteil]
                                                                                                      |
                                                                                    [Abgeschlossen] / [Berufung]
```

| Zustand | Bedeutung |
|---|---|
| **Unbekannt** | Tat geschehen, Behörde weiß nichts |
| **Gemeldet** | Anzeige oder Notruf liegt vor |
| **In Ermittlung** | Beamte arbeiten; Spuren, Zeugen, Kameras |
| **Verdächtiger benannt** | Behörde hat eine Person im Fokus (Behördenverdacht, [Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md#71-zwei-verdächtigenlisten)) |
| **Fahndung** | aktiv gesucht (regional/landesweit) |
| **Haftbefehl** | Rechtsgrundlage für Festnahme |
| **Untergetaucht** | Person ist nicht auffindbar; Fahndung bleibt |
| **Festgenommen** | in Gewahrsam |
| **Anklage / Verfahren / Urteil** | siehe [Modul 15](15-Gerichte-und-Verfahren.md) |
| **Entlastet/Falsch** | Verdacht war falsch (Gegenbeweis) |
| **Kalt** | ohne neue Spur ruht die Akte; kann später wieder aufgenommen werden (Cold Case, `23#1062A`) |
| **Wieder aufgenommen** | neue Spur oder neuer Anlass |
| **Abgeschlossen** | rechtskräftig oder eingestellt |

### 5.1 Übergänge (Auswahl)

| Von | Nach | Auslöser | Bedingung |
|---|---|---|---|
| Unbekannt | Gemeldet | Welt | Zeuge oder System meldet; Latenz abgelaufen |
| Gemeldet | In Ermittlung | Behörde | Ressourcen vorhanden; Schwere ausreichend |
| In Ermittlung | Verdächtiger benannt | Behörde | Konfidenz gegen eine Person erreicht Schwelle |
| In Ermittlung | Kalt | Zeit | keine neue Spur, lange Zeit |
| Verdächtiger benannt | Fahndung | Behörde | Schwere und Konfidenz genügen; Zuständigkeit klar |
| Fahndung | Haftbefehl | Behörde/Gericht | Beweislage genügt (Modell offen) |
| Haftbefehl | Festgenommen | Polizei | Person gefunden oder stellt sich |
| beliebig | Entlastet | Beweis | Gegenbeweis überzeugt die Behörde (Voreingenommenheit und Korruption können das verhindern) |
| Festgenommen | Anklage | Staatsanwaltschaft | Beweislage |
| Kalt | Wieder aufgenommen | Welt | neue Spur, Spieler-Eingriff, neuer Fall |

### 5.2 Mehrere Akten

Eine Person kann **mehrere Akten** haben (verschiedene Taten, Behörden). Eine Akte kann **mehrere Verdächtige** haben. Die Akten sind **Weltzustand**, nicht Spieler-Zustand, weil die Polizei auch ohne den Spieler ermittelt (`48#159E`: Ermittlungen laufen während der Haft weiter).

---

## 6. Entwurf: Fahndungsintensität statt Sterne

### 6.1 Prinzip

Statt einer Zahl 1–5 führt jede Fahndung eine **Intensität**, die sich aus mehreren Faktoren ergibt und mehrere **Wirkungen** hat. Die Intensität ist **intern**; was der Spieler sieht, hängt von Schwierigkeit und Wissen ab (P16).

### 6.2 Faktoren (Katalog)

| Faktor | Wirkung auf die Intensität |
|---|---|
| Schwere der Tat | höher bei Gewalt gegen Menschen, Polizei |
| Beweislage | höher, wenn Konfidenz hoch |
| Opfer | höher bei Beamten, Kindern, bekannten Personen (Beispiele des Assistenten sind nicht in den Quellen; diese Zeile ist reiner Entwurf) |
| Medienaufmerksamkeit | höher bei Berichten, Aufnahmen |
| Politik und Region | höher bei Druck auf Behörden |
| Ressourcen | niedriger, wenn Behörde ausgelastet ist |
| Zuständigkeit | höher, wenn mehrere Behörden beteiligt (landesweit) |
| Zeit | sinkt ohne neue Spuren (Kalt), steigt bei neuen Taten |

### 6.3 Wirkungen (Katalog)

| Wirkung | Beispiel |
|---|---|
| Streifendichte und Kontrollen | mehr Fahrzeuge, Kontrollen in der Region |
| Kameraauswertung | Zugriff auf mehr Aufzeichnungen |
| Kennzeichenabfrage | Fahrzeug wird bei Kontrollen erkannt |
| Absperrungen | Straßensperren, Sperrgebiete |
| Medienpräsenz | Bild in den Nachrichten, Fahndungsaufrufe |
| Spezialkräfte | Eingreiftruppen, Hunde, Hubschrauber |
| Haftbefehl | Rechtsgrundlage für Festnahmen und Durchsuchungen |
| Zuständigkeit | Bundesbehörde schaltet sich ein |
| Informanten und Kopfgeld | Zivilisten melden den Spieler; Kopfgeldjäger (Idee in `23#598E`, Satz: „Wanted-System → Kopfgeld → spätere Kopfgeldjäger“, Beispielkette; nicht beschlossen) |

Die Kopfgeld-Idee stammt aus der Feature-Analyse des Entwicklers (`23#598E`) als ‚Zusammenhang, der ausdrücklich wichtig ist‘; ein beschlossenes Kopfgeldsystem gibt es nicht. Punkt 65 nennt Kopfgeld als Bedingung für das Ende einer Fahndung (`23#1057E`: „bis man kopfgeld bezahlt“), also als **Strafzahlung**, nicht als Jäger. Beides ist offen (❓ Q-E55).

### 6.4 Anzeige für den Spieler (Entwurf)

| Schwierigkeit/Hilfe | Anzeige |
|---|---|
| leicht/Geführt | Hinweis auf Suchgebiet, Polizeipräsenz in der Nähe |
| mittel | ungefähre Zone |
| schwer/Detektiv | nur, was der Spieler sieht und hört (Streifen, Funk, Medien) |

Die Zuordnung ist ein Entwurf (P16: Polizeikarte je Schwierigkeit). Das Motiv: Die Intensität wirkt **in der Welt**, nicht als Zahl (P1).

### 6.5 Abklingen

Die Intensität klingt ab, wenn neue Spuren fehlen und der Spieler sich ruhig verhält; schwere Fahndung bleibt aber bestehen, bis es eine glaubwürdige Auflösung gibt (P9, Punkt 65). Das ist ein Zielkonflikt: Eine schwere Fahndung hat eine **Untergrenze**, die nur durch Auflösung (Verfahren, Freispruch, Zahlung, Rücknahme) verschwindet. Der Entwurf führt deshalb zwei Größen: die **Intensität** (schwankt) und den **Status** (Fahndung besteht bis Auflösung).

---

## 7. Entwurf: Begegnungen mit der Polizei

| Begegnung | Auslöser | Verlauf (Vorschlag) | Quelle |
|---|---|---|---|
| **Verkehrskontrolle** | Streife stoppt den Spieler | Papiere, Fahrzeug, Fragen; Entdeckung bei Fahndung (Kennzeichen) | Punkt 63 |
| **Anhalten zu Fuß** | Beamter spricht den Spieler an | Ausweis, Fragen; Verdacht, Durchsuchung (Abschnitt 8) | – |
| **Verfolgung** | Spieler flieht oder wird verfolgt | Abschnitt 12 | `48#156A` (Polizeiverfolgungen) |
| **Festnahme** | Haftbefehl, frische Tat, Gewalt | Aufgeben oder Flucht; **Aufgeben** ist echte Alternative zur tödlichen Lösung | P3 |
| **Tatortbegegnung** | Spieler am Tatort | Polizei sperrt ab (P17); Zugang nur mit Berechtigung | Punkt 50 |
| **Zeugenbefragung** | Polizei befragt den Spieler als Zeugen | Aussage kann Verdacht begründen | Z14 |
| **Hunter trifft Polizei** | Zusammenarbeit/Konflikt | Polizei ahnt nicht, was los ist (P13); kleine Einheiten könnten es wissen (P14) | Punkte 116, 21 |

### 7.1 Eskalationsstufen (Entwurf)

Das Verhalten des Spielers bestimmt, ob eine Situation harmlos endet oder eskaliert (P8). Ein Entwurf, welche **Stufen** der Reaktion es gibt:

| Stufe | Reaktion der Beamten |
|---|---|
| 0 | freundlich/neutral |
| 1 | misstrauisch, fragt nach |
| 2 | verlangt Kooperation, ruft Verstärkung |
| 3 | zieht die Waffe, fordert zum Aufgeben auf |
| 4 | setzt Gewalt ein; Verfolgung |
| 5 | tödliche Gewalt, Spezialkräfte |

Die Stufen sind **kein Zahlenmodell**, sondern eine Ordnung für Dialoge, Animationen und KI-Verhalten. Die Quellen legen nur fest, dass Aufgeben möglich bleibt (P3).

### 7.2 Folgen von Gewalt gegen Polizei

Der Spieler, der Polizisten angreift oder tötet, riskiert eine ernsthafte, **landesweite** Fahndung (`23#1058A`: „Polizei angreift“; Punkt 55). Details (zum Beispiel Sonderstatus bei getöteten Beamten) fehlen. Ob NPC-Polizisten **permanent sterben** (Tier B/C), folgt aus den NPC-Regeln (wichtige NPCs nur durch den Spieler, Punkt 105).

---

## 8. Entwurf: Durchsuchung, Beschlagnahme und Haftbefehl

> [!NOTE]
> **Nur Vorschlag (💡).** `M-OD` führt ‚warrant/search/seizure/vehicle-search rules‘ ausdrücklich als offen. Die Welt ist eine **fiktive US-Welt** (`35#533A`), es gibt also eine Anlehnung an reale Muster, aber keine Entscheidung. Die Kategorien unten sind Anlehnungen, keine Rechtsberatung.

| Rechtsgrundlage (Kategorie) | Vorschlag |
|---|---|
| **Einwilligung** | Der Spieler stimmt zu (oder kann ablehnen); Ablehnung kann Verdacht erhöhen |
| **Haftbefehl** | Behörde hat Beweislage; Festnahme und Durchsuchung der Wohnung erlaubt |
| **Gefahr im Verzug / frische Tat** | Beamte handeln sofort |
| **Festnahme** | Durchsuchung der Person und der Umgebung |
| **Fahrzeug** | bei Kontrolle mit Verdacht; Kofferraum ist wichtiger Ausrüstungsplatz (`23#1088A` Frage 134, Block 10), daher Folgen für Hunter-Ausrüstung |

### 8.1 Wirkungen auf den Spieler

| Wirkung | Beschreibung |
|---|---|
| **Beschlagnahme** | Waffen, Ausrüstung, Beweisstücke werden eingezogen; bei Waffenbesitz und Ausrüstung eines Hunters (Salz, Eisen, Waffen, Chemikalien) Konfliktstoff |
| **Beweisfund** | Gefundene Gegenstände werden Beweise (Modul 10 §9) |
| **Hunter-Ausrüstung** | Vieles ist legal, manches nicht; Hunter müssen Fahrzeuge und Verstecke schützen (`23#1090A`: „Verstecken im Fahrzeug“, Frage 144) |
| **Rückgabe** | Eingezogenes kann zurückgeholt werden (Verfahren, Anwalt) – nicht in den Quellen |

### 8.2 Haftbefehl für NPCs

Dieselbe Logik gilt für NPC-Täter (P19). Der Spieler kann Hinweise liefern, anzeigen, eingreifen. Eine Festnahme durch den Spieler (Bürgerfestnahme) ist nicht in den Quellen.

---

## 9. Entwurf: Verdeckte Ermittlung und Informanten

Beschlossen: Verdeckte Ermittlungen sind Teil des Systems (P11); der Assistent nennt Undercover-Beamte, Informanten und längerfristige Operationen (`48#156A`, 💡).

| Rolle | Entwurf |
|---|---|
| **Undercover-Beamter** | NPC in einer Gruppe; sammelt Beweise; kann den Spieler beobachten; kann enttarnt werden; ist Gegenstand des Spielers als Mitglied (Modul 13 §9) |
| **Informant** | NPC, der Informationen verkauft (an Polizei, Hunter, Gruppe); Vertrauen, Bezahlung, Gefahr |
| **Spieler als Informant** | Spieler gibt Informationen an die Polizei oder arbeitet als Spitzel (Modul 13); offen |
| **Operation** | langfristige Ermittlung gegen Gruppe; Zeitverlauf; Abschluss mit Razzia |
| **Gegenspionage** | Gruppe sucht Spitzel; Verdacht kann falsch sein |

Die **Folgen** für Fälle sind: Die Polizei kann über Informanten Dinge wissen, die keine Zeugen gesehen haben. Der Entwurf behandelt das als **zusätzlichen Informationsweg** in Abschnitt 4 (Meldung kommt vom Informanten), mit eigener Zuverlässigkeit (Modul 10 §6).

---

## 10. Entwurf: Korruption und Einflussnahme

Beschlossen: Korruption gehört zu Option D (`48#154A`, `48#155E`); der Assistent nennt „manipulierte Ermittlungen, korrupte Beamte und übernatürliche Einflussnahme“ (`48#156A`, 💡). Zusätzlich beschlossen: Bestechung und Bedrohung von Zeugen (Frage 48).

### 10.1 Formen (Katalog)

| Form | Beispiel (erfunden) | Wirkung |
|---|---|---|
| **Bestechung** | Beamter nimmt Geld und „verliert“ Beweise | Akte bleibt unvollständig |
| **Erpressung** | Beamter wird erpresst | wirkt in Richtung des Erpressers |
| **Loyalität** | Beamter schützt Freund, Gruppe, Stadt | Vertuschung |
| **Ideologie** | Beamter gehört zu einer Gruppe | voreingenommene Ermittlung |
| **Übernatürliche Einflussnahme** | Wesen beeinflusst oder besetzt Beamte | Beweise verschwinden, falsche Spuren |
| **Politischer Druck** | Behörde unter Druck | Fall wird beschleunigt oder verzögert |

### 10.2 Wirkung auf die Zustandsmaschine

Korruption ist ein **Störfaktor** in den Übergängen von Abschnitt 5: Sie kann Übergänge verhindern (Beweise unterschlagen), erzwingen (falscher Verdacht) oder verzögern. Der Spieler kann Korruption entdecken (Widersprüche in Akten, Verhalten) und ausnutzen oder bekämpfen. Die Quellen enthalten **keine** Regeln, wie Korruption aufgedeckt wird (❓ Q-E56).

---

## 11. Entwurf: Polizei und das Übernatürliche

| Aussage | Status | Quelle |
|---|:---:|---|
| Die Polizei erklärt Vorfälle zunächst normal (Wildtier, Unfall, Mord) | 📋 | Punkt 116 |
| Die meisten Polizisten kennen die Wahrheit nicht; kleine geheime Einheiten kennen Teile | 📋 | `23#1075E` Punkt 21; Spielvision §10 |
| Regionale Wahrnehmung: Kleinstadt-Bewohner kennen ungewöhnliche Vorfälle, Metropolen halten Monster für Legenden | 💡 Beispiele | `48#148A` |
| Behörden können vertuschen; Berichte werden manipuliert, Beweise verschwinden | 📋 (Option D der Frage `48#146A`) | `48#145E` zu `48#146A` |
| Eine einflussreiche Gruppe von Wesen kann Ermittlungen manipulieren | 💡 Beispiel | `48#146A` |
| Polizei erlebt Übernatürliches selbst und ändert ihr Verhalten | 💡 Beispiel (Polizistin, die einen Vampir sah) | `48#150A` |

### 11.1 Entwurf: Wissensstufen einer Behörde

| Stufe | Bedeutung | Verhalten |
|---|---|---|
| 0 | kennt kein Übernatürliches | erklärt alles normal; sucht menschliche Täter |
| 1 | hat Unerklärliches gesehen | misstrauisch, vorsichtig, schweigt |
| 2 | einzelne Beamte wissen | informelle Zusammenarbeit mit Huntern möglich |
| 3 | Sondereinheit | Fachkenntnis, Vertuschung, Gegenmaßnahmen |
| 4 | institutionelles Wissen | Programme, Labore (Modul 17) |

Die Stufe gehört einer **Behörde und Einzelpersonen** (Tier-System). Sie kann sich ändern (Ereignisse, Beweise, Spielerhandlungen, Modul 17). Wie viele Beamte welche Stufe haben, ist offen.

### 11.2 Vertuschung als Ermittlungsgegner

Behörden oder Gruppen, die vertuschen, sind Gegner in der Ermittlung: Beweise werden verlegt, Zeugen beeinflusst, Berichte geändert (`48#146A` Tabelle, 💡). Der Spieler muss das **erkennen** (Lücken, Widersprüche) und umgehen. Es entsteht der Fall ‚Verfahren gegen den Spieler als Folge einer Vertuschung‘ (verknüpft mit Modul 13).

---

## 12. Entwurf: Verfolgung, Straßensperren, Hunde

### 12.1 Verfolgung

Beschlossen: Polizeiverfolgungen (`48#156A`, Ausformulierung), NPC-Kriminelle können verfolgt werden (Block 8K Frage 98), Verkehr macht Rettungsfahrzeugen Platz (Frage 97). Entwurf:

| Phase | Beschreibung |
|---|---|
| Sichtung | Streife erkennt das Fahrzeug (Kennzeichen, Beschreibung) |
| Verfolgung | Streifen folgen, Funk, Verstärkung |
| Sperrung | Straßensperre vorausschauend (Autobahn, Brücke) |
| Verlust | Spieler entkommt (Sichtlinie bricht; Information bleibt) |
| Festnahme/Aufgabe | Aufgeben (P3) |

Nach dem Verlust bleibt **Wissen** bei der Polizei (Fahrzeug, Richtung, Beschreibung), das Fahndung begründet. Ein Fahrzeugwechsel hilft gegen das Fahrzeugwissen, nicht gegen die Beschreibung der Person.

### 12.2 Straßensperren

Straßensperren sind weltliche Objekte (Fahrzeuge, Absperrband). Sie entstehen durch Fahndung (Intensität) oder Tatortabsperrung (P17). Sie haben Zuständigkeit, Lebensdauer und können umgangen werden.

### 12.3 Hunde

P4 wünscht Polizei-K9 mit wiederverwendbarem Geruchs- und Windmodell. Der Hund der Polizei folgt einer **Geruchsspur** (Modul 05). Das Geruchsmodell ist offen (`M-OD`: ‚Exact K9 scent persistence/wind model‘). Der Hund des Spielers nutzt dasselbe Modell (`23#1027E` Punkt 22: Hund kann Begleiter bei Fällen sein).

---

## 13. Entwurf: Simulationsstufen und Zeit

| Aspekt | Entwurf |
|---|---|
| **Polizisten als NPCs** | Tier A (Sheriff, Detective) mit Gedächtnis; Tier B (Streifenbeamte, bekannte Gesichter); Tier C/D (Hintergrund) |
| **Hintergrundsimulation** | Akten werden **in Schritten** fortgeschrieben; die Welt bleibt konsistent (`48#160A`: größere Simulationsschritte) |
| **Zeitsprung** | Ermittlungen und Verfahren laufen weiter (`48#159E`); der Zustand nach dem Sprung ergibt sich aus der Aktenfortschreibung |
| **Unbeobachtete Gebiete** | Polizei handelt abstrakt (Zustände), bei Annäherung wird detailliert (`23#1056A` Fragen 122–123) |
| **Determinismus** | Fortschreibung ist deterministisch bei gleichem Zustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) |

---

## 14. Koop

Nach den Entscheidungen zum Koop-Betrieb (`48#161E`, `48#163E`):

| Aspekt | Beschlossen/Entwurf |
|---|---|
| Nur der betroffene Hunter wird festgenommen | 📋 |
| Die anderen können weiterspielen | 📋 |
| Ein Zeitsprung der Host-Welt braucht Zustimmung aller aktiven Spieler | 📋 |
| Fahndung gilt für den Charakter, nicht für den Host | 💡 (Charaktere gehören zum Host-Spielstand; Folgerung aus `48#109E`) |
| Mitspieler können Beweise sammeln, einen Anwalt organisieren, eine Befreiung versuchen | 📋 (Option C der Frage `48#160A`, gewählt in `48#161E` mit eigenem Text; die Wahl ist nicht einem Buchstaben zugeordnet) |

Die Wahl der Koop-Verhaftung (`48#161E`): ‚der spieler ist halt im häfn aber man kann die zeit vorspulen und die welt wird halt dann weiter laufen, im singleplayer auch so aber wenn man dort nebenmissionen hat dürfen die nicht weggehen‘. Die Zuordnung zu den Optionen A–D ist **nicht benannt**; sie entspricht inhaltlich B (individuelle Verhaftung); die Rettungsmöglichkeiten (C) sind vom Assistenten als Ergänzung bestätigt worden (`48#162A`).

---

## 15. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Spieler stellt sich freiwillig | – | verkürzt Verfahren; keine Fahndung; Reputationsfolgen |
| Spieler flieht aus Gewahrsam | Ausbruch aus Polizeigewahrsam | Fahndung wird schwerer (Modul 16: Ausbruch) |
| Spieler wird auf dem Weg zum Hunter-Fall festgenommen | Fall pausiert | Missionsschutz ([Modul 09](09-Falllebenszyklus-und-Generierung.md#9-entwurf-zeitsprünge-und-missionsschutz)) |
| Zwei Behörden mit widersprüchlicher Akte | – | Konflikt der Zuständigkeiten; Spieler kann es nutzen |
| Spieler besitzt Hunter-Ausrüstung im Fahrzeug bei Kontrolle | – | siehe Abschnitt 8 |
| Polizei findet einen Leichnam, den ein Wesen hinterließ | – | Vorfall, Fallwahrheit; Polizei sucht menschlichen Täter (P13) |
| Polizist ist ein Wesen oder besessen | – | Korruption, übernatürlich (Abschnitt 10) |
| Polizei gerät in Gefahr | – | Spieler kann helfen; Wirkung auf Ruf |
| Schwerer Unfall mit Spielerbeteiligung | – | Rettung, Polizei, Feuerwehr (Block 8H); Unfallermittlung |
| NPC-Polizei wird vom Spieler getötet | – | Gewicht der Fahndung; NPC permanent tot |
| Ausnahmezustand/Krise durch Monster | – | Behörden reagieren, Absperrungen (`48#146A`) |
| Kinder- und Altersschutz | – | siehe Adult 09 / Gameplay-Systeme §12 |

---

## 16. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Konfidenz, Beweiskette |
| Forensik ([Modul 11](11-Forensik-und-Labor.md)) | Labor der Behörde |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Meldung, Aussagen |
| Identität ([Modul 13](13-Identitaet-Ruf-und-Formwandler.md)) | E4: Behördenüberzeugung |
| Gerichte ([Modul 15](15-Gerichte-und-Verfahren.md)) | Haftbefehl, Anklage |
| Haft ([Modul 16](16-Haft-und-Strafvollzug.md)) | Gewahrsam |
| Geheimhaltung ([Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md)) | Wissensstufen, Vertuschung |
| NPC-Verkehr und Fahrverhalten (Welt 16) | Verfolgung, Kontrollen |
| Fahrzeuge und Kennzeichen (Welt 13) | Kennzeichenabfrage |
| Waffen und Kampf | Gewalt gegen Polizei |
| Wirtschaft | Geldstrafe, Kaution, Kopfgeld |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Akten sind Zustand |
| Kartenansicht und HUD | Polizeikarte je Schwierigkeit |
| Spielzeit | Zeitsprung, Fortschreibung |

---

## 17. Ergänzungen aus der Quellenauswertung

Dieser Abschnitt trägt Quellenfunde aus dem Chat 23 (August und September 2026) nach, die das Polizeisystem **vor** dem Quiz vom Oktober beschreiben. Sie sind überwiegend Vorschläge des Assistenten, auf die der Entwickler mit Einzelantworten oder pauschaler Zustimmung reagierte; die Art der Zustimmung ist jeweils vermerkt.

### 17.1 Der Wissenskatalog der Polizei und die Informationskette

Der Assistent (`23#612A` Punkt 14) erweiterte ein Beispielbild zu einem **Investigation-Knowledge**-Katalog: Identität, Fahrzeug, Kennzeichen, Kleidung, Gesicht, Waffentyp, Ballistik, CCTV, Zeugenbeschreibung, Fingerabdrücke, DNA (wo passend), Telefon-/digitale Beweise (wo passend), bekannte Komplizen. „Aber nicht jede Polizeistreife kennt automatisch alles. Information muss erfasst, gemeldet, verarbeitet, verteilt werden.“ Das passe zum zuständigkeitsabhängigen System. Status: 💡; die Kette ist die Quelle der Informationskette in [§4](#4-entwurf-der-weg-einer-tat-zur-behörde). Der Entwickler antwortete auf diese Nachricht nur zu den Punkten 4, 17 und 23 (`23#613E`) und widersprach den Punkten 14 bis 16 nicht.

### 17.2 Polizei nicht mehr exakt auf der Minimap

Der Assistent (`23#612A` Punkt 15, „Gefällt mir sehr“): Normal vielleicht ungefähre Suchgebiete und bekannte Einheiten, keine perfekte Position hinter Gebäuden; **Hardcore/immersiv**: keine Polizei auf der Minimap, der Spieler muss Sirenen hören, Licht sehen, Funk und Umgebung deuten. Status: 💡; der Entwickler widersprach nicht; `M-PD`/`M-OD` führen die Darstellung je Schwierigkeit als Wunsch bzw. offen. Beantwortet Q-E25 **teilweise** (Richtung, keine Festlegung).

### 17.3 Interne Eskalationsstufen statt Wanted-Sterne

> **Statusvermerk (Entscheidung 39, 31.08.):** Das Changelog formuliert interne Eskalationsstufen als Option („kann“) und die Kartenbeschränkung auf immersiven Schwierigkeiten als Soll-Regel („sollte“). Der Abschnitt bleibt 💡 Vorschlag, bis der Entwickler die Stufenanzeige und die Minimap-Regel entscheidet. Bis dahin ist die Richtung dokumentiert, nicht bestätigt.

Der Assistent (`23#612A` Punkt 16): Das konkrete Sternsystem würde er „nicht kopieren“, aber intern **sechs oder mehr Threat/Escalation Levels** führen: 0 keine, 1 lokale Reaktion, 2 aktive Suche, 3 große Polizeireaktion, 4 taktische Reaktion, 5 State/Federal-Eskalation, 6 extreme Fahndung; „UI muss nicht zwingend sechs Sterne zeigen. Die Simulation dahinter ist wichtiger.“ Dazu die Liste der Fahndungsinformation (Abschnitt 17.1) und `23#510A`: Eine Tabelle zu GTA-Merkmalen bewertet „6 Wanted Stars“ als „Nicht übernehmen“, „Wir bauen ein realistischeres Behörden-/Threat-System ohne GTA-Sterne“. Status: 💡; der Entwickler äußerte sich weder in `23#511E` noch in `23#613E` dagegen.

**Wirkung auf K-E8:** Die „Fahndungsstufen“ im Quiz-Text (`48#154A`, `48#156A`) sind damit mit hoher Wahrscheinlichkeit die **internen** Stufen aus `23#612A`, nicht sichtbare Sterne. Der Konflikt ist dadurch **entschärft, aber nicht entschieden**: Es gibt keine Entwickleraussage, die Sterne ausschließt oder Stufen anzeigt. Die Anzeige ist weiterhin offen.

### 17.4 Die Ermittlungsakte (Case Record) und der Verdachtsgrad

Der Assistent (`23#618A` Punkte 22 bis 25), mit der Antwort des Entwicklers „Die anderen finde ixh gut“ (`23#619E`; einzeln beantwortet wurden dort nur die Punkte 1, 12, 26 und 28):

| Punkt | Inhalt | Status |
|---|---|:---:|
| 22 Ermittlungsakte | Statt nur eines Wanted Levels besitzt ein schwerer Vorfall intern einen **Case Record** mit Zeugenaussagen, CCTV, Fahrzeug, Kennzeichen, Kleidungsbeschreibung, Waffenbeweisen, Ballistik, Fingerabdrücken, Verdachtsgrad und Zuständigkeit; er kann Stunden oder Tage später noch relevant sein | 📋 Richtung (pauschale Zustimmung) |
| 23 Verdachtsgrad | Die Polizei kennt zum Beispiel „männlich, ca. 185 cm, dunkle Jacke, schwarzer SUV“, aber nicht „definitiv der Spieler“. Stufen: **Unknown, Possible Match, Likely Match, Identified, Confirmed** | 📋 Richtung (pauschale Zustimmung) |
| 24 Fahrzeug wechseln | hilft gegen das Kennzeichen, löscht aber keine Beweise; bei bekanntem Gesicht oder gleicher Kleidung bleibt der Hinweis | 📋 Richtung |
| 25 Smartphone | Digitale Spuren (Verbindungen, Standortdaten, soweit zugänglich, Posts, Fotos) können relevant werden, aber kein magischer Vollzugriff | 📋 Richtung |

Das ist die Quelle für die Aktenzustände in [§5](#5-entwurf-zustandsmaschine-einer-polizeiakte) und die Konfidenzidee in [Modul 10](10-Beweislogik-und-Wissensmodell.md). Die fünf Stufen des Verdachtsgrads ersetzen den bisher offenen Begriff der Konfidenz **für die Behördenseite** als Namen (Zahlenwerte bleiben offen, `M-OD`).

### 17.5 Zuständigkeitsbeispiel und Festnahme statt Flucht

Der Assistent (`23#606A` Punkte 6 und 7, Auswahl neuer RDR2-Anregungen): **Festnahme statt nur Flucht oder Tod** („Hände hoch, Festnahme, Durchsuchung, Transport, Polizeistation/Gefängnis“, danach Kaution, Geldstrafe, Anwalt oder Kontakt, Crew holt dich ab); Zuständigkeitsbeispiel (County A: Person gesucht; County B: nur Fahndungsinformation; State Police: landesweiter Haftbefehl; FBI: bundesweit). Der Entwickler antwortete auf diese Nachricht mit `23#607E` zu anderen Punkten (Punkt 5, Tiere) und ohne Einwand gegen 6 und 7. Status: 💡; der Kern ‚Festnahme ist kein Game Over‘ ist später ausdrücklich bestätigt (`23#1057E` Punkt 68).

### 17.6 Wanted und Kopfgeld in der modernen Welt

Der Assistent (`23#603A` Punkt 15): „Wir brauchen keine RDR2-Kopfgeldjäger.“ Stattdessen: lokale Polizei, State Police, Sheriff Departments, FBI, möglicherweise Homeland Security, private Security, Fahndung, Haftbefehl, Fahrzeugfahndung, Personenbeschreibung. „Und langfristig sollte ein Problem nicht einfach verschwinden, weil man 500 Dollar beim Postamt bezahlt.“ Dem steht gegenüber: der Entwickler selbst nannte in seinem Prompt zur Feature-Analyse als wichtige Systemkette „Wanted-System → Kopfgeld → spätere Kopfgeldjäger“ (`23#598E`, als Beispiel für die Analyse eines **anderen** Spiels), und er sagte zur Dauer einer Fahndung, sie bleibe bestehen, „bis man kopfgeld bezahlt, oder die klage zurückgezogen oder man freigesprochen wird“ (`23#1057E` Punkt 65). **Auslegung (ohne Entscheidung):** Das Wort „Kopfgeld“ erscheint beim Entwickler als **Bedingung des Endes** einer Fahndung (Zahlung) und als Beispiel in einer Analyse; ein Kopfgeldjäger-System als Spielziel ist nirgends von ihm festgelegt, und der Assistent riet davon ab. Q-E55 bleibt damit **offen mit klarerer Lage**; K-E13 in Modul 21.

### 17.7 Beweisstücke, Waffen und Leichen in der Welt

Der Assistent (`23#628A` Punkte 4, 6, 7; Antwort `23#629E`: „Die anderen punkte sind gut“, mit einer Ausnahme zu Punkt 12 über Kinder):

| Punkt | Inhalt |
|---|---|
| 4 Körpertransport | Fahrzeuge können Passagiere, Verletzte, Bewusstlose, Leichen, Tierkadaver und große Ausrüstung transportieren; die Polizei kann einen Kofferraum durchsuchen |
| 6 Fallen gelassene Waffen | bleiben physisch in der Welt; andere können sie aufheben; die Polizei kann sie als Beweismittel sichern; ‚intelligenter Cleanup‘: wichtige und Beweisgegenstände bleiben, irrelevante alte Gegenstände werden aufgeräumt |
| 7 Beweise einsammeln | Die Polizei sperrt ab, findet die Waffe, markiert sie als Beweis, sammelt sie ein (Ballistik, Fingerabdrücke); eine weggeworfene Tatwaffe ist nicht ‚aus dem Inventar gelöscht‘ |

Status: 📋 Richtung (pauschale Zustimmung). Das ist die Quelle der Aussage zu abgelegten Gegenständen in `M-PD` und von Modul 05 §8.

---

## 18. Ergänzung: Kontrollen und Warn-Apps

Zu Polizei im Straßenverkehr (`23#1024A` Punkte 18, 19: Geschwindigkeitskontrollen, Rotlicht, gefährliches Fahren, Kennzeichen, Verfolgungen; Radar) antwortete der Entwickler: Es könne passieren, und man könne sich „apps aufs handy downloaden wo man das angezeigt bekommt“; es sei „wgal ob polizei oder blitzer“ (`23#1025E` Punkte 18, 19, 📋 Richtung; die Antwort bezieht sich auf die Listennummern in `23#1024A`, Zuordnung 🧭). Für die Fahndung heißt das: **Verkehrsdelikte sind ein Eintrittspunkt in den Polizeikontakt** ([Modul 25](25-Kriminalitaet-in-der-Welt.md)); die Warn-App ist ein Smartphone-Werkzeug ([Modul 27](27-Medien-und-digitale-Spuren.md)). Schwelle und Strafe sind ❓.

---

## 19. Was fehlt
- **Straftatenkatalog**, Schweregrade, Festnahmeschwellen, Strafen (`M-OD`).
- **Konfidenzmodell**, Beweiswertung, Schwellen für Fahndung und Haftbefehl.
- **Zuständigkeitskarte** und Informationsverzögerungen.
- **Kameradichte** und Aufbewahrung.
- **Regeln** zu Durchsuchung, Beschlagnahme, Fahrzeugdurchsuchung.
- **Fahndungsstufen**: Begriff und Darstellung (K-E8).
- **Kopfgeld**: Strafzahlung oder Jäger (Q-E55).
- **Aufdecken von Korruption** (Q-E56).
- **Polizeikarte/HUD** je Schwierigkeit.
- Das Verhältnis zu **Mod-Behörden**.
