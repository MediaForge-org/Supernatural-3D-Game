# NPCs 04: Persönlichkeit, Zustände und Zeitskalen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wodurch sich NPCs unterscheiden und auf welchen Zeitskalen sich ihr Inneres verändert. Die Schichtung (Kern, schnell, mittel, lang) ist beschlossen; **Dimensionen, Schwellen und Abklingformeln sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Die vier Schichten](#2-entwurf-die-vier-schichten)
3. [Entwurf: Persönlichkeitskern](#3-entwurf-persönlichkeitskern)
4. [Entwurf: Schnelle Zustände](#4-entwurf-schnelle-zustände)
5. [Entwurf: Mittlere und lange Zustände](#5-entwurf-mittlere-und-lange-zustände)
6. [Entwurf: Wie Ereignisse wirken](#6-entwurf-wie-ereignisse-wirken)
7. [Entwurf: Kommunikationsstil und Stimme](#7-entwurf-kommunikationsstil-und-stimme)
8. [Entwurf: Angst, Moral und Aufgabe bei Menschen](#8-entwurf-angst-moral-und-aufgabe-bei-menschen)
9. [Sonderfälle](#9-sonderfälle)
10. [Entwurf: Prüfregeln](#10-entwurf-prüfregeln)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-PS-001 | Jeder NPC hat intern eine **Core Personality**, die sich nur sehr langsam oder durch starke Ereignisse verändert | 📋 | `35#268A` 9B.1 Frage 1; „1-13 ja“ in `35#269E` |
| N-PS-002 | Daneben gibt es **schnell wechselnde Zustände** wie Stimmung, Stress, Nervosität, Selbstvertrauen | 📋 | `35#268A` Frage 2; „1-13 ja“ in `35#269E` |
| N-PS-003 | Es gibt **mittelfristige Zustände** über Tage und Wochen (Vertrauen, Vertrautheit, Bindung, Eifersucht) | 📋 | `35#268A` Frage 3; „1-13 ja“ in `35#269E` |
| N-PS-004 | Es gibt **langfristige Zustände** über Monate und Jahre (tiefe Bindung, Groll, Gewohnheiten, langfristige Vorlieben) | 📋 | `35#268A` Frage 4; „1-13 ja“ in `35#269E` |
| N-PS-005 | Persönlichkeiten haben kontinuierliche Dimensionen (zum Beispiel Offenheit, Impulsivität, Schüchternheit, Dominanz im Auftreten, Risikobereitschaft, Eifersucht, Bindungsorientierung) statt grober Klassen; daraus lassen sich lesbare Archetypen ableiten | 📋 | `35#260A` Fragen 11–13; „10-20 ja“ in `35#261E` |
| N-PS-006 | Persönlichkeit bleibt langfristig relativ stabil; starke Story- und Lebensereignisse können kleine dauerhafte Änderungen bewirken | 📋 | `35#260A` Fragen 14–15; „10-20 ja“ in `35#261E`; `35#272A` Frage 3; „1-50 passt alles“ in `35#273E` |
| N-PS-007 | Stimmung schwankt kurzfristig unabhängig von der Persönlichkeit; ein schüchterner NPC kann in bestimmten Kontexten direkt werden, ein offener bei bestimmten Personen oder Themen zurückhaltend | 📋 | `35#260A` Fragen 16–18; „10-20 ja“ in `35#261E` |
| N-PS-008 | Persönlichkeit bestimmt, wie stark Ereignisse **emotional gespeichert** werden, und ein NPC mit guter Erinnerung (Memory-Trait) behält mehr Details | 📋 | `35#268A` Fragen 9–10; „1-13 ja“ in `35#269E` |
| N-PS-009 | Besonders positive oder negative Erinnerungen bleiben stärker haften | 📋 | `35#268A` Frage 8; „1-13 ja“ in `35#269E` |
| N-PS-010 | Unterschiedliche Persönlichkeiten reagieren verschieden (freundlich, aggressiv, ängstlich, hilfsbereit) | 📋 | `23#1056A` Frage 37; „ja“ in `23#1057E` |
| N-PS-011 | Betrunkene NPCs verhalten sich tatsächlich anders | 📋 | `23#1056A` Frage 38; „ja sicher“ in `23#1057E` |
| N-PS-012 | Eigene Persönlichkeit, Moral, Kompetenz und Meinung von Begleitern; sie kommentieren Entscheidungen, handeln nicht wie stumme Bots und können Handlungen verweigern | 📋 | `23#1088A` Fragen 17–19; „16-24 ja“ in `23#1089E`; `M-PD` ‚Companion autonomy …‘ |
| N-PS-013 | Menschliche Gegner können fliehen, erstarren oder sich ergeben, wenn sie genug eingeschüchtert sind; Furcht und Moral bleiben intern und zeigen sich im Verhalten | 📋 | `M-PD` ‚Multiple-opponent combat …‘ |
| N-PS-014 | NPCs haben unterschiedliche Kommunikationsstile (Schreiben, Anrufen, Sprachnachricht, Antworttempo, Länge, Initiative); eine Figur kann Schreiben hassen und lieber anrufen. Das genaue Datenmodell ist offen | 📋 / ❓ | `M-PD` ‚Smartphone / social media …‘; [Gameplay-Systeme](../Gameplay-Systeme.md) §8; `M-OD` |
| N-PS-015 | Wichtige Figuren behalten ihre individuelle Stimme: Alter, Region und Akzent, Wortschatz, Bildung, Persönlichkeit, Wissen, Humor, Stressreaktion, Tabu- und private Themen | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-PS-016 | Langfristige Veränderungen sollen **langsam und nachvollziehbar** passieren, nicht als Zufallschaos | 📋 | `35#272A` Frage 50; „1-50 passt alles“ in `35#273E` |
| N-PS-017 | Veränderung durch Erfahrung ist nicht garantiert, sondern hängt stark von der Persönlichkeit ab | 📋 | `35#272A` Frage 30; „1-50 passt alles“ in `35#273E` |
| N-PS-018 | Süchte (Alkohol, Drogen, Rauchen) gehören zur Welt, stehen aber nicht dauerhaft im Mittelpunkt | 📋 | `23#1057E` Punkt 39; `23#1058A` ‚Alkohol, Rauchen, Drogen‘ |

---

## 2. Entwurf: Die vier Schichten

Die Quellen unterscheiden Kern, schnelle, mittlere und lange Zustände (N-PS-001 bis N-PS-004). Das Modell unten legt **Rollen** fest, keine Zahlen (💡).

| Schicht | Zeitskala (Quelle) | Beispiele (Quelle) | Wer ändert sie (Entwurf) | Wie sie zurückfällt (Entwurf) |
|---|---|---|---|---|
| **Kern** | Jahre; nur durch starke Ereignisse | Offenheit, Impulsivität, Schüchternheit, Bindungsorientierung | starke Lebensereignisse (Modul 14), lange Entwicklungen | fällt nicht zurück; verschiebt sich langsam |
| **Schnell** | Minuten bis Stunden | Stimmung, Stress, Nervosität, Selbstvertrauen | aktuelle Situation, Substanzen, Müdigkeit, Ereignisse | zurück zu einer vom Kern abhängigen Grundlinie |
| **Mittel** | Tage bis Wochen | Vertrauen, Vertrautheit, Bindung, Eifersucht | Interaktionen, Erlebnisse, Konflikte | langsamer Rückgang ohne Kontakt, nicht auf null |
| **Lang** | Monate bis Jahre | tiefe Bindung, Groll, Gewohnheiten | wiederholte Muster, prägende Ereignisse | sehr langsam; Anker bleiben (Modul 05) |

**Regeln (Entwurf):**

1. **Einfluss läuft nach oben.** Schnelle Zustände färben Wahrnehmung und Handlung; wiederholte schnelle Zustände verschieben mittlere; wiederholte mittlere verschieben lange; nur außergewöhnliche Ereignisse den Kern (N-PS-006).
2. **Der Kern wirkt nach unten.** Er bestimmt Grundlinie, Empfindlichkeit und Verarbeitungsgeschwindigkeit (N-PS-008, N-PS-017).
3. **Beziehungswerte sind mittelfristige oder langfristige Zustände zwischen zwei Personen**, nicht Merkmale einer Person ([Modul 08](08-Beziehungen-Modell-und-Wirkung.md)).
4. **Zustand und Wissen sind getrennt.** Wer wütend ist, weiß nicht automatisch mehr; Wissen steht im Gedächtnis ([Modul 05](05-Gedaechtnis-Erinnerung-und-Vergessen.md), [06](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md)).
5. **Alles bleibt intern.** Auch hier gibt es keine Anzeige (N-LT-012).

---

## 3. Entwurf: Persönlichkeitskern

> 💡 **Vorschlag.** Die Dimensionen entstammen den Beispielen in `35#260A` Frage 11 (‚Offenheit, Impulsivität, Schüchternheit, Dominanz im Auftreten, Risikobereitschaft, Eifersucht, Bindungsorientierung usw.‘). Ergänzungen sind als solche gekennzeichnet.

| Dimension | Wirkung im Alltag | Wirkung gegenüber dem Spieler | Quelle |
|---|---|---|---|
| Offenheit | probiert Neues, spricht Fremde an | gibt eher Auskunft, lässt sich ansprechen | `35#260A` |
| Impulsivität | handelt ohne Abwägung | reagiert schneller aggressiv oder hilfsbereit | `35#260A` |
| Schüchternheit | meidet Aufmerksamkeit | schweigt bei Fragen, antwortet knapp | `35#260A` |
| Dominanz im Auftreten | setzt Positionen durch | verlangt, widerspricht, führt Gespräch | `35#260A` |
| Risikobereitschaft | geht Wagnisse ein | begleitet eher, bricht Regeln | `35#260A` |
| Eifersucht | misstraut Dritten | reagiert auf Kontakt des Spielers zu anderen | `35#260A`, `35#268A` |
| Bindungsorientierung | sucht Nähe und Dauer | bindet sich schneller an | `35#260A` |
| Gewissenhaftigkeit (Ergänzung) | Zuverlässigkeit, Pünktlichkeit | hält Absprachen, ist dabei zu erreichen | 💡 |
| Misstrauen (Ergänzung) | erwartet Täuschung | Verdacht gegenüber Fremden, fragt nach | 💡 |
| Mitgefühl (Ergänzung) | hilft anderen | hilft eher, auch ohne Gegenleistung | 💡 |
| Moral/Wertegerüst (Ergänzung) | Grenzen dessen, was er tut | verweigert Handlungen (N-PS-012) | `23#1088A` Frage 19 |
| Gedächtnisgabe (Ergänzung) | behält Details | erinnert sich genauer (N-PS-008) | `35#268A` Frage 9 |
| Ideologie und Glaube (Ergänzung) | Weltsicht, religiöse oder politische Haltung | reagiert auf Übernatürliches unterschiedlich (Modul 15) | `23#1058A` ‚Supernatural-Reaktion‘ |

**Kontinuierlich, nicht in Klassen** (N-PS-005): Jede Dimension ist ein Wert in einem Bereich; Archetypen (zum Beispiel ‚gesellig‘, ‚vorsichtig‘) werden daraus *abgeleitet* und wirken nur als Start-Bias der Generierung (N-ID-010).

**Kontextabhängige Ausnahmen (N-PS-007)** verlangen, dass die Persönlichkeit nicht in jedem Kontext gleich wirkt. Entwurf: Jeder Kern hat *Kontextmodifikatoren* (Thema, Person, Ort, Rolle), die den Ausdruck verschieben, ohne den Kern zu ändern. Beispiel: Ein schüchterner Mechaniker spricht fachlich offen über Motoren.

**Generierung:** korreliertes Modell (N-ID-011) mit Wahrscheinlichkeitsverteilungen je Beruf, Alter, Region; handgeschriebene Figuren haben feste Werte.

---

## 4. Entwurf: Schnelle Zustände

| Zustand | Auslöser | Auswirkung | Rückkehr |
|---|---|---|---|
| Stimmung | Tagesereignisse, Wetter, Gespräche, Substanzen | Tonlage, Hilfsbereitschaft, Geduld | zur Grundlinie des Kerns |
| Stress | Termindruck, Gefahr, Konflikt, Schlafmangel | Fehler, Reizbarkeit, Rückzug | durch Ruhe, Schlaf, Entlastung |
| Nervosität | Gefahr, Prüfung, Verdacht, Spieler als Fremder | knappe Antworten, Blickkontakt, Flucht | nach Abklingen des Auslösers |
| Selbstvertrauen | Erfolg, Misserfolg, Zuspruch | Initiative, Direktheit | langsam |
| Müdigkeit | Arbeit, Schlafentzug, Reisen | Aufmerksamkeit, Reaktion | Schlaf |
| Rausch | Alkohol, Drogen | Enthemmung, Urteil, Wahrnehmung | Zeit; Folgen bleiben im Gedächtnis (Modul 05) |
| Schmerz, Krankheit | Verletzung, Infektion | Verhalten, Mobilität, Stimmung | Heilung |

**Regeln (Entwurf):**

1. **Schnelle Zustände sind nicht Teil des Gedächtnisses.** Ein Ereignis, das einen Zustand auslöste, kann Gedächtnis werden (zum Beispiel Verletzung); der Zustand selbst nicht.
2. **Rauschzustand wirkt auf Wahrnehmung und Erinnerung.** Betrunkene Zeugen nehmen schlechter wahr und erinnern sich ungenauer ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §4; N-PS-011).
3. **Zustandsänderungen sind begrenzt pro Zeitschritt**, damit keine Sprünge (Zufallschaos, N-PS-016) entstehen.
4. **Ein unbeobachteter NPC wird nicht frameweise gerechnet** (Modul 22): Schnelle Zustände werden bei der Materialisierung aus dem Zeitplan und den letzten Ereignissen abgeleitet.

---

## 5. Entwurf: Mittlere und lange Zustände

Diese Zustände bilden die **Beziehungsdynamik** (Modul 08, 09). Die Quellen unterscheiden mehrere Aspekte, die nicht zu einer Zahl verschmelzen sollen (`35#268A` Fragen 21–30; „16-30 ja“ in `35#269E`):

| Aspekt | Verhalten laut Quelle | Frage |
|---|---|---|
| Vertrauen | verändert sich deutlich schneller als tiefe Bindung | Frage 21 |
| Anziehung | kann stark schwanken, ohne dass eine Beziehung sofort zerbricht | Frage 22 |
| Komfort | wächst, wenn Interaktionen wiederholt positiv verlaufen | Frage 23 |
| Vertrautheit | macht bestimmte Interaktionen weniger ‚awkward‘ | Frage 24 |
| Gewohnheit, emotionale Bindung, romantische Bindung, allgemeine Vertrautheit | sind **verschiedene** Dinge | Frage 25 |
| Offenheit durch gute Erfahrung | NPC wird offener, ohne dass die Persönlichkeit sich ändert | Frage 26 |
| Vorsicht durch schlechte Erfahrung | NPC wird dauerhaft vorsichtiger | Frage 27 |
| Verzeihen | Beziehungshistorie beeinflusst, wie schnell Missverständnisse verziehen werden | Frage 28 |
| Auseinanderdriften | zwei NPCs können sich nach langer Beziehung entfremden, obwohl nichts Großes passiert | Frage 29 |
| Beziehungsstufen | aktive Beziehung, enge Bindung, lockere Verbindung, frühere Beziehung, gelegentlicher Kontakt, praktisch vergessen | Frage 30 |

**Entwurf – Rolle der langen Zustände:**

1. **Ressentiment und Bindung sind getrennte Größen.** Ein NPC kann jemanden anziehend finden und gleichzeitig nicht verzeihen (`35#270A` Frage 45: ‚Soll auch Hass/Ressentiment neben Attraction existieren können?‘; „22-52 ja“ in `35#271E`).
2. **Gewohnheiten** (zum Beispiel Stammlokal, Wochentag, Route, Rituale) sind lange Zustände, die den Zeitplan prägen (Modul 13).
3. **Langfristige Vorlieben ändern sich** durch Erfahrung (`35#270A` Fragen 1–10; „1-15 ja“ in `35#271E`); sie sind keine Zufallsdrift.
4. **Prägung** durch eine sehr lange Beziehung ist stärker als viele kurze Begegnungen (`35#272A` Frage 4; „1-50 passt alles“ in `35#273E`).

---

## 6. Entwurf: Wie Ereignisse wirken

Aus den Quellen folgt: Ereignisse sind der **einzige Weg**, auf dem sich langlebige Zustände ändern; Zeit verdünnt, ändert aber nicht von allein die Richtung (N-PS-016).

```text
Ereignis e  (Ursache, Zeit, Ort, Beteiligte)       -- Modul 05
   │
   ├── Wahrnehmung: hat der NPC es beobachtet / erzählt bekommen?   -- Modul 06
   │      nein -> keine Wirkung (N-LT-013)
   │
   ├── Bewertung durch den Kern:  Empfindlichkeit * Relevanz * Erwartung
   │      (Persönlichkeit bestimmt, wie stark gespeichert wird: N-PS-008)
   │
   ├── schnelle Zustände (sofort)
   ├── mittlere Zustände (Beziehung, Vertrauen)
   ├── Gedächtniseintrag (wenn relevant)
   └── ggf. lange Zustände / Kernverschiebung (nur bei starken Ereignissen)
```

**Entwurf – Gewichtung:**

| Einflussgröße | Wirkung |
|---|---|
| Schwere | stärkere Wirkung, höhere Speicherwahrscheinlichkeit, mehr Details |
| Neuheit | erste Erfahrung wirkt stärker als die zehnte |
| Betroffenheit | Ereignis betrifft den NPC, Familie, Freund, Besitz oder Weltbild |
| Erwartung | erwartetes Ereignis wirkt schwächer, überraschendes stärker |
| Kontext | Rausch, Stress, Müdigkeit, Anwesenheit anderer ändern die Verarbeitung |
| Verlauf | wiederholte ähnliche Ereignisse verdichten sich zu Mustern (‚generell positive Geschichte mit X‘; `35#280A`) |
| Persönlichkeit | Gedächtnisgabe, Empfindlichkeit, Vergebungsneigung |

**Wichtig:** Ein Ereignis kann für **zwei NPCs gegensätzlich** wirken (N-LT-027: eine Handlung kann für Person A gut und für Person B schlecht sein). Es gibt also keine globale Moralwertung.

---

## 7. Entwurf: Kommunikationsstil und Stimme

**Beschlossen:** Unterschiedliche Stile (N-PS-014); der Spielstil des Gesprächs wird über Dialog-IDs gesteuert, die Gedächtnis- und Beziehungsbedingungen enthalten (`M-PD` ‚Dialogue / conversation architecture‘). Wichtige Figuren haben ein Voice/Character Bible (N-PS-015).

**Offen:** Das Datenmodell für Kommunikationsstil und seine Beziehungsfolgen ist im Master ausdrücklich offen (`M-OD`).

**Entwurf (💡):**

| Merkmal | Bedeutung | Wirkung |
|---|---|---|
| Kanal-Präferenz | Anruf, Text, Sprachnachricht, persönlich | wie ein NPC den Spieler kontaktiert und welche Antworten er erwartet |
| Antworttempo | schnell, träge, selten | ob Schweigen als Desinteresse gilt (nicht automatisch: nicht jede ignorierte Nachricht ist ein Malus, [Gameplay-Systeme](../Gameplay-Systeme.md) §8) |
| Länge und Ton | knapp, ausführlich, förmlich, mit Emojis | Textbausteine für Nachrichten |
| Initiative | meldet sich von selbst oder wartet | Häufigkeit eigener Kontaktaufnahmen (Modul 13) |
| Akzent, Wortschatz, Humor | Region, Bildung, Persönlichkeit | Dialogvarianten |
| Tabuthemen | worüber er nicht spricht | Gesprächsverweigerung, Ausweichen (Modul 20) |
| Stressreaktion | wie sich Sprache unter Druck ändert | Fluchen, Verstummen, Übertreibung |

**Regel:** Kommunikationsstil beeinflusst nur *wie* ein NPC kontaktiert, nicht *ob* er Wissen hat.

---

## 8. Entwurf: Angst, Moral und Aufgabe bei Menschen

Beschlossen ist: Menschliche Gegner können fliehen, erstarren oder sich ergeben, Furcht und Moral bleiben intern und sichtbar nur im Verhalten (N-PS-013); Beziehungen haben Angst als eine Dimension (Vertrauen, Angst, Respekt, Verdacht). Für Nemesis und Fraktionen ([Modul 11](11-Nemesis-Modell-und-Datensatz.md), [16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md)) braucht es ein gemeinsames Modell (💡):

| Größe | Gilt für | Auslöser | Wirkung |
|---|---|---|---|
| Angst vor dem Spieler | Person | Gewalt, Ruf, Verletzungen, Drohung | Flucht, Gehorsam, Schweigen, Hass |
| Moral (Gruppe) | Gruppe im Gefecht | Verluste, Überzahl, Anführer fällt | Rückzug, Aufgabe, Zerfall |
| Loyalität | Mitglied → Gruppe | Behandlung, Schuld, Bindung | bleibt oder verlässt |
| Mut | Person | Persönlichkeit, Erfahrung, Verteidigung von Nahestehenden | steht trotz Angst |

Die Kopplung mit dem Kampfsystem gehört in die Kampfdokumentation; dieses Modul legt nur fest, dass diese Größen **intern** sind und **im Datensatz** persistieren, wenn sie langfristige Wirkung haben (zum Beispiel ein Überlebender, der Angst vor dem Spieler behält).

---

## 9. Sonderfälle

1. **Traumatisierung.** Ein NPC, der Gewalt erlebte, kann einen veränderten Kern oder dauerhafte Vorsicht tragen (N-PS-006, `35#272A` Fragen 26–30). Es braucht dafür eine *Ursache* im Ereignisjournal, nicht nur einen Wert.
2. **Sucht.** Abhängigkeit ist grundsätzlich möglich (`23#1090A` ‚Schlaf, Essen, Alkohol, Drogen‘ gilt für den Spieler; für NPCs `23#1057E` Punkt 39). Sucht ändert Zeitpläne, Einkommen, Beziehungen und Aufenthaltsorte.
3. **Besessenheit und Flüche.** Wahrnehmung und Persönlichkeit können verändert sein ([Kreaturen 05](../Kreaturen/05-Geister-Besessenheit-und-Fluecher.md)). Der Kern bleibt im Datensatz erhalten; der Einfluss ist eine Überlagerung, kein Überschreiben.
4. **Demenz und Krankheit im Alter.** Eine Folge der Alterung (Modul 14) kann Gedächtnisgabe und Kern verändern; sie ist eine Spielentscheidung der Tiefe (❓ Q-N12).
5. **Zwei NPCs mit identischem Kern.** Erlaubt; Unterschiede kommen aus Geschichte, Zustand und Beziehungen (N-ID-010).
6. **Unerklärliche Persönlichkeitsänderung ohne Ereignis.** Verboten (N-EN-021); ein Spieler soll jede sichtbare Veränderung auf Ursachen zurückführen können (Archiv, Gespräch, Ermittlung).
7. **NPC vollkommen in der Ferne.** Zustände werden nicht gerechnet; die nächste Materialisierung leitet sie aus dem Zeitplan ab (Modul 22).

---

## 10. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-021 | Jede Änderung des Persönlichkeitskerns hat ein Ereignis als Ursache im Journal | N-PS-006, N-PS-016 |
| N-EN-022 | Schnelle Zustände kehren ohne neue Ereignisse zur Grundlinie des Kerns zurück | Abschnitt 2 |
| N-EN-023 | Dasselbe Ereignis kann für zwei NPCs gegensätzliche Wirkung haben, aber jedes Ergebnis ist deterministisch reproduzierbar | N-LT-027 |
| N-EN-024 | Beziehungswerte sind Zustände einer Beziehung, nicht einer Person | Abschnitt 2 |
| N-EN-025 | Ein Zustandswert bewegt sich pro Zeitschritt höchstens um eine feste Schrittweite (kein Sprung) | N-PS-016 |
| N-EN-026 | Die Persönlichkeit eines NPC ändert sich nie durch Zeit allein | Abschnitt 6 |
| N-EN-027 | Rauschzustand verschlechtert Wahrnehmung und Erinnerung messbar | N-PS-011 |
| N-EN-028 | Persönlichkeitsdimensionen sind kontinuierlich; Archetypen sind abgeleitet | N-PS-005 |
| N-EN-029 | Kein Zustandswert erscheint in der Spieler-Oberfläche | N-LT-012 |

---

## 11. Was fehlt

- Keine Skalen, Abklingzeiten, Schwellen oder Zufallsverteilungen.
- Keine abgeschlossene Liste der Persönlichkeitsdimensionen (die Quelle sagt ‚usw.‘).
- Das Kommunikationsstil-Modell (`M-OD`) und seine Beziehungsfolgen.
- Tiefe von Alterskrankheiten (Q-N12).

## Ergänzung aus dem Entscheidungsabgleich 31.08. (Stand 09.11.2026)

- 📋 **Kommunikation ohne Zahlenabzug (Entscheidung 44):** Kommunikationsstile sind Verhaltensdaten, keine sichtbaren Rollenspiel-Balken. Die Wirkung auf Beziehungen hängt vom Kontext und der Qualität der Kommunikation ab. Wer eine Nachricht ignoriert, erzeugt **keinen** numerischen Malus.
