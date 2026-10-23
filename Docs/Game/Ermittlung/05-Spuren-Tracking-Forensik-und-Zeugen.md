# Ermittlung 05: Spuren, Tracking, Forensik und Zeugen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die **Beweisarten** der Ermittlung und die Regeln, nach denen sie Wissen liefern. Es nennt **keine** Werte, Zeiten oder Wahrscheinlichkeiten, weil die Quellen keine enthalten. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Überblick: [Spielvision §7.3](../Spielvision.md#73-beweise-forensik-und-spuren). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Rangfolge: normale Spuren zuerst](#1-rangfolge-normale-spuren-zuerst)
2. [Spuren und Tracking](#2-spuren-und-tracking)
3. [Fehlleitung und Manipulation](#3-fehlleitung-und-manipulation)
4. [Der Hund als Spurensucher](#4-der-hund-als-spurensucher)
5. [Forensik](#5-forensik)
6. [Zeugen und Gerüchte](#6-zeugen-und-gerüchte)
7. [Verhaltensmuster und historische Recherche](#7-verhaltensmuster-und-historische-recherche)
8. [Persistente Tatorte](#8-persistente-tatorte)
9. [Tiere, Kadaver und Wildtier-Forensik](#9-tiere-kadaver-und-wildtier-forensik)
10. [Digitale Spuren](#10-digitale-spuren)
11. [Ergänzung: Hund als Begleiter bei Fällen](#11-ergänzung-hund-als-begleiter-bei-fällen)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Rangfolge: normale Spuren zuerst

Block 9I (`23#1074A`) setzt als Frage 76: „Normale Spuren zuerst: Tatort zeigt nicht automatisch leuchtende „Monster-Spur“.“ Der Entwickler antwortete auf die Gruppe 76–83 mit „ja“ (`23#1075E`). Status: 📋 (Richtung). Konsequenz: Ein Tatort liefert zunächst **gewöhnliche** Beweise; das Übernatürliche zeigt sich über Anomalien (Abschnitt 5) und Muster (Abschnitt 7).

---

## 2. Spuren und Tracking

Block 10F (Fragen 68–79 in `23#1088A`) fragt die Spurenarten ab; der Entwickler bestätigte „68-79 ja“ (`23#1089E`). Der Assistent fasste die Bestätigung als „Vollständig bestätigt“ zusammen (`23#1090A`). Die Spurenarten (📋 Richtung; `M-CA0905` Abschnitt 4 führt denselben Katalog als ‚accepted direction‘ im Tier-1-Stand):

| Spur | Hinweise der Quellen |
|---|---|
| Fußabdrücke | auf Schlamm, Schnee, Sand und ähnlichem (Frage 68) |
| Blut | abhängig von Verletzung und Bewegung (69) |
| Reifenspuren | Profil, Richtung, Fahrzeugklasse teilweise analysierbar (70) |
| Schleifspuren | bei Tatorten (71) |
| gebrochene Vegetation | (`M-CA0905`; `23#1090A`) |
| Haare, Fell, Federn | Tiere und Wesen können Spuren hinterlassen (73) |
| Geruch | siehe Hund (Abschnitt 4) und kreaturspezifische Geruchsverfolgung (`M-PD`) |
| Krallen, Bisse, zerstörte Umgebung | als körperliche Spuren im Block 9I (Frage 77) |

### 2.1 Wetter und Zeit

| Aussage | Status | Quelle |
|---|:---:|---|
| Regen zerstört Spuren (Frage 75) | 📋 | `23#1088A`, „68-79 ja“ `23#1089E` |
| Schnee überdeckt ältere Spuren (Frage 76 des Blocks 10F) | 📋 | dito |
| Regen, Schnee, Verkehr und Zeit verschlechtern Beweise tatsächlich (Frage 80 Block 9I) | 📋 | `23#1074A`, „76-83 ja“ `23#1075E`; `M-CA0905` Abschnitt 4 (‚weather degradation, snow‘) |

### 2.2 Erfahrung statt Arcade-Marker

📋 „Tracking-Erfahrung verbessert den Spieler organisch, nicht über Arcade-Marker“ (`23#1090A`, Zusammenfassung; Frage 78 in `23#1088A`: Der Spieler wird mit Übung besser, aber nicht durch blinkende RPG-Level). Das passt zur Regel des Masters, dass Tracking ‚environmental, not a magic objective arrow‘ ist (`M-CA0905` Abschnitt 4).

---

## 3. Fehlleitung und Manipulation

| Aussage | Status | Quelle |
|---|:---:|---|
| Spuren können **manipuliert** und **falsch gedeutet** werden | 📋 | `M-CA0905` (‚manipulated/misread traces‘); `23#1090A` |
| Monster oder Menschen können absichtlich falsche Spuren legen | 📋 | Frage 82 `23#1074A`; „76-83 ja“ |
| NPCs und Monster können Spuren verwischen (intelligentere Täter) | 📋 | Frage 77 Block 10F `23#1088A`; „68-79 ja“ |
| Tracking kann **komplett in die falsche Richtung** führen, wenn Spuren falsch interpretiert werden | 📋 | Frage 79 Block 10F; „68-79 ja“ |
| Der Spieler kann Beweise falsch deuten, einen Verdächtigen falsch beschuldigen oder eine Spur übersehen; die Welt läuft weiter | 📋 | `23#1062A`, siehe [Modul 02 §5](02-Falltypen-und-Fallmodell.md#5-falsche-und-verfrühte-schlüsse) |

---

## 4. Der Hund als Spurensucher

`M-CA0905` Abschnitt 4 (📋 Richtung): Der **Hund** besitzt Rasse, Persönlichkeit, Training (Gehorsam, Rückruf, Suche, Geruchsverfolgung), Reaktionen, Verletzung, Altern und tierärztliche Versorgung; Impfungen sind optional (`23#1089E` Punkte 31–40: „impfungen sind optional“). Tracking mit dem Hund ist **umweltbezogen, kein magischer Zielpfeil**. Einzelheiten:

- Der Hund kann Gerüche und Spuren verfolgen, ist aber nicht allwissend (Frage 81 Block 9I `23#1074A`; „76-83 ja“).
- Regen, Wasser, Verkehr, Zeit und viele Gerüche erschweren das Tracking (Frage 35 Block 10, `23#1088A`; „31-40 ja“ `23#1089E`).
- Geruchstracking berücksichtigt unter anderem kontaminierte Spuren (`23#1090A`).
- Polizei-K9 und ein wiederverwendbares Geruchs- und Windmodell sind gewünscht (`M-PD`; [Modul 06](06-Kriminalitaet-Polizei-und-Justiz.md)).

Hunde sind **Begleiter**, nicht die Ermittlung; die Begleiter-Kompetenzen (Kampf, Recherche, Medizin, Technik, Tracking) sind unterschiedlich verteilt (`M-CA0905` Abschnitt 4).

---

## 5. Forensik

Katalog laut `M-CA0905` Abschnitt 4, wortgleich als „Vollständig bestätigt“ in `23#1090A` wiederholt (📋 Richtung):

| Bereich | Quelle der Aufzählung |
|---|---|
| Fingerabdrücke | `M-CA0905`; Frage 80 Block 10G `23#1088A`: „realistisch genug, aber nicht an jedem Objekt der Welt simuliert“ |
| DNA | Frage 81 (Blut, Haare, Speichel bei wichtigen Fällen) |
| Ballistik | `M-CA0905`, `M-PD` |
| Schuh- und Reifenabdrücke, Reifenprofile bei Fahrzeugfällen | `M-CA0905`; Frage 84 |
| Fasern | `M-CA0905` |
| Toxikologie | Gifte, Medikamente, Drogen (Frage 86) |
| Todeszeit als realistischer **Bereich** | `M-CA0905` (‚time-of-death ranges‘) |
| Autopsieberichte | Frage 88: Berichte erhalten oder teilweise Untersuchungen begleiten |
| Tatortarbeit vor Ort | `M-CA0905` |
| **Labore mit Bearbeitungszeit** | Frage 90; Ergebnisse brauchen Zeit |
| **Uneindeutige Ergebnisse** | Frage 91: kein Forensik-Knopf, der einen Täternamen ausspuckt |
| übernatürliche Anomalien | Frage 92; Frage 83 Block 9I: Forensik kann zeigen, dass etwas physikalisch keinen Sinn ergibt |

Der Entwickler bestätigte Block 10G mit „80-92 ja“ (`23#1089E`). Der Assistent formulierte die Konsequenz als Gegenbild: „Analyse drücken → Täter: John Smith“ ist **ausgeschlossen** (`23#1090A`, 💡 als Formulierung, 📋 als Richtung).

> [!NOTE]
> Es gibt in den Quellen **keine** Laufzeiten für Labore, keine Genauigkeit der Todeszeit, keine Regel, welche Gegenstände Fingerabdrücke tragen. Das sind offene Designwerte (❓).

---

## 6. Zeugen und Gerüchte

| Aussage | Status | Quelle |
|---|:---:|---|
| Zeugenaussagen sind **Informationen, keine garantierte Wahrheit**; NPC-Überzeugungen können falsch sein; Gerüchte verzerren sich | 📋 | [Spielvision §7.3](../Spielvision.md#73-beweise-forensik-und-spuren); `23#1056A` Fragen 25–27 (Zeugen müssen etwas gesehen/gehört/erzählt bekommen haben, Gerüchte ≠ Wahrheit; Antworten „ja“ in `23#1057E`) |
| NPCs wissen nur, was sie gesehen, gehört, erzählt bekommen oder über Nachrichten erfahren haben; kein magisches Allwissen | 📋 | `23#1058A` Block-8-Zusammenfassung; `M-PD` |
| NPCs können den Spieler und seine Fahrzeuge **wiedererkennen**; Kleidung und Maskierung beeinflussen die Wiedererkennung | 📋 | `23#1056A` Fragen 28–30, Antworten „ja“ |
| Informationen verbreiten sich über Freunde, Familie, soziale Medien, Nachrichten und können sich verzerren | 📋 | Fragen 26–27 |
| Lauschen: Gespräche können belauscht werden und gelegentlich Informationen liefern | 📋 | Frage 35 Block 8 („bereits bestätigt“, `23#1056A`) |
| Ein Zeuge kann Schweigegeld annehmen oder bedroht werden; es kann versucht werden, ihn zu töten | 📋 | Antwort auf Frage 48 `23#1057E` („ja kann er, aber er muss nicht petzen, vielleicht bekommt er auch schweigegeld, oder es wird versucht ihn zu töten“) |

Ein Zeuge kann also sowohl **Quelle** als auch **Risiko** sein. Wie belastbar eine Aussage ist, lässt sich aus den Quellen nicht ableiten (❓ Vertrauensmodell; siehe [Modul 06](06-Kriminalitaet-Polizei-und-Justiz.md)).

---

## 7. Verhaltensmuster und historische Recherche

| Aussage | Status | Quelle |
|---|:---:|---|
| Opferauswahl, Tatzeit und Ort sind **genauso wichtig** wie physische Beweise | 📋 | Frage 78 `23#1074A`; „76-83 ja“ |
| Alte Fälle können ein Muster über **Jahrzehnte oder Jahrhunderte** zeigen | 📋 | Frage 79; „76-83 ja“ |
| Bücher, Tagebücher, Kirchenarchive, Zeitungen und Bibliotheken bleiben wichtig, weil nicht alles digitalisiert ist | 📋 | Frage 10 Block 9, Antwort „10 ja genau“ `23#1075E` |
| Zeitungsartikel, Polizeiberichte, Zeugenaussagen und historische Dokumente sind Archivknoten | 📋 | `M-PD` |
| Fluch und Artefakt können einen neuen Fall an einem anderen Ort erzeugen; Spur wandert | 📋 | Fragen 65 und 109 `23#1074A`; Antworten „62-68 ja“ und „107-114 ja, aber würde ich nicht so häufig machen“ `23#1075E` |

---

## 8. Persistente Tatorte

📋 Frage 150 `23#1074A`: Blut und Schäden bleiben plausibel bestehen, bis Reinigung oder Reparatur erfolgt; der Entwickler antwortete „149-150 ja“ (`23#1075E`). Passend: Wichtige abgelegte Waffen oder Gegenstände bleiben physisch und können Polizeibeweise werden, mit ‚intelligenter Bereinigung‘ gegen unbegrenzten Müll (`M-PD`). Wie lange ein Tatort besteht und wer ihn reinigt, ist **offen**.

---

## 9. Tiere, Kadaver und Wildtier-Forensik

`M-PD` ‚Wildlife / study / harvesting / monsters‘ (📋): Zerfall von Kadavern, Aasfresser und Forensik sind gewünscht; mäßige Verderblichkeit von Proben und Fleisch. Monsterforschung „spiegelt“ die Tierstudie auf einer tieferen epistemischen Stufe: Hypothese, Wahrscheinlichkeit, Bestätigung, Opferanalyse, Proben, Fähigkeiten, Schwächen; intelligente Monster können sich der Jagd anpassen. Die Tierstudie verlangt eine bewusste **Studienhandlung** (‚Deliberate Study input is required; merely seeing a species is insufficient for full documentation‘). Siehe [Spielwelt und Untergrund §9](../Spielwelt-und-Untergrund.md#9-natur-tiere-und-jagd) für Natur und Jagd. Das Planungsinventar `M-B37` nennt als Zukunftsarbeit ‚decomposition, scavengers and forensic value‘ und ‚disease/parasite hooks‘ (nur Themenliste).

---

## 10. Digitale Spuren

| Aussage | Status | Quelle |
|---|:---:|---|
| NPCs können filmen, fotografieren, streamen und posten; Aufnahmen können digitale Beweise werden | 📋 | `M-PD` ‚Smartphone / social media / dynamic world events‘ |
| Digitale Spuren sind bei schweren Ermittlungen möglich, aber **kein magischer Vollzugriff** | 📋 | [Gameplay-Systeme §11](../Gameplay-Systeme.md#11-smartphone-soziale-medien-und-weltereignisse) |
| Soziale Medien enthalten falsche und irrelevante Inhalte, damit der Feed kein perfekter Detektor ist; sie sind für Hauptgeschichte und normale Nebenfälle **nie erforderlich** | 📋 | `M-PD`; `M-B40` (‚Hard content rule‘) |
| Überwachungskameras zählen zu den Beweisquellen der Rechtsseite | 📋 | `M-PD` ‚Cash / law / evidence‘ |

---

## 11. Ergänzung: Hund als Begleiter bei Fällen

Auf die Frage nach Haustieren (`23#1026A`, Punkt 22: Hunde/Katzen in Städten und Häusern) antwortete der Entwickler: „sehr wichtig, hund kann begleiter sein bei fällen“ (`23#1027E` Punkt 22, 📋). Das stützt den Spürhund (Abschnitt 5, Polizei-K9 in [Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) als **Spielerbegleiter**, nicht nur als Behördenwerkzeug. Die Mechanik (Geruchsspur, Befehle, Verletzbarkeit, Pflege) ist nicht festgelegt (❓).

---

## 12. Was fehlt
- **Zahlen**: Labordauer, Unsicherheit der Todeszeit, Zerfallsraten von Spuren, Reichweiten.
- Welche **Objekte** Fingerabdrücke, DNA oder Fasern tragen.
- Das **Vertrauensmodell** für Zeugenaussagen und digitale Beweise.
- Regeln für **Beweisketten**, **Fälschung** und **Manipulation** durch den Spieler (nur im Rechtsinventar `M-B39` als Thema genannt).
- Die Verbindung zur **Weltzustands-Technik**: Wie ein Beweis als Fakt gespeichert wird, ist nicht entworfen.

Siehe [Modul 21](21-Konflikte-und-offene-Fragen.md).
