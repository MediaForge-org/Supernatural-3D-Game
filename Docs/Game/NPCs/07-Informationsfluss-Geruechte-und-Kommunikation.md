# NPCs 07: Informationsfluss, Gerüchte und Kommunikation

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Wissen zwischen NPCs wandert, verzerrt wird und die Welt erreicht. Beschlossen sind: Information verbreitet sich über soziale Kanäle und Medien, Gerüchte können falsch sein, NPCs sprechen miteinander, die Verbreitung ist begrenzt. **Gerüchtemodell, Reichweiten und Geschwindigkeiten sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Die Ermittlungssicht der Gerüchtekette steht in [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §12, Medien in [Ermittlung 27](../Ermittlung/27-Medien-und-digitale-Spuren.md), öffentliches Wissen in [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Kanäle](#2-entwurf-kanäle)
3. [Entwurf: Wer erzählt was wem](#3-entwurf-wer-erzählt-was-wem)
4. [Entwurf: Das Gerüchteobjekt](#4-entwurf-das-gerüchteobjekt)
5. [Entwurf: Verzerrung](#5-entwurf-verzerrung)
6. [Entwurf: Soziale Kreise und Reichweite](#6-entwurf-soziale-kreise-und-reichweite)
7. [Gespräche zwischen NPCs und Mithören](#7-gespräche-zwischen-npcs-und-mithören)
8. [Entwurf: Medien](#8-entwurf-medien)
9. [Entwurf: Spielerhandlungen](#9-entwurf-spielerhandlungen)
10. [Entwurf: Wirkungen](#10-entwurf-wirkungen)
11. [Entwurf: Skalierung](#11-entwurf-skalierung)
12. [Sonderfälle](#12-sonderfälle)
13. [Entwurf: Prüfregeln](#13-entwurf-prüfregeln)
14. [Was fehlt](#14-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-IN-001 | Informationen verbreiten sich über Freunde, Familie, soziale Medien, Nachrichten und andere Kanäle | 📋 | `23#1056A` Frage 26; „ja“ in `23#1057E`; `23#1058A` |
| N-IN-002 | Gerüchte können falsch, verzerrt, übertrieben oder unvollständig sein; Gerücht ist nicht Wahrheit | 📋 | `23#1056A` Frage 27; „ja“ in `23#1057E`; `23#1058A` |
| N-IN-003 | NPCs unterhalten sich auch miteinander, ohne dass der Spieler beteiligt ist; sie telefonieren, schreiben Nachrichten, fotografieren, schauen Social Media | 📋 | `23#1056A` Fragen 34, 36; „ja korrekt“ und „ja“ in `23#1057E` |
| N-IN-004 | Der Spieler kann Gespräche belauschen und gelegentlich Informationen erhalten | 📋 | `23#1056A` Frage 35 (‚bereits bestätigt‘); „ja“ in `23#1057E`; `M-PD` ‚Dialogue / conversation architecture‘ |
| N-IN-005 | NPCs besprechen Wetter, Sport, Arbeit, Nachrichten, Verkehr, lokale Ereignisse, andere NPCs, Monstergerüchte und Spieleraktionen | 📋 | `23#1056A` Frage 33; „ja“ in `23#1057E` |
| N-IN-006 | Gerüchte über Übernatürliches verbreiten sich in der lokalen Community; Menschen meiden nach mehreren Vorfällen einen Wald oder eine Straße | 📋 | `23#1056A` Fragen 114–115; „ja sicher“ und „ja genau“ in `23#1057E` |
| N-IN-007 | Der Klatsch (Gossip) soll sich **realistisch über soziale Kontakte verbreiten**, aber nicht sofort die ganze Welt wissen; Beispiel: A sieht etwas, erzählt Freundin B, B vielleicht C, ein Mensch 500 km entfernt weiß nichts | 🧭 | `35#270A` Frage 53 und `35#272A` Klärung des Assistenten („ich würde 53 daher klar JA empfehlen“); der Entwickler fragte „was ist gossip“ in `35#271E` und bestätigte „54-60 ja“, die das voraussetzen |
| N-IN-008 | Reputation und Gerüchte wirken nur in tatsächlich verbundenen sozialen Kreisen | 📋 | `35#270A` Frage 54; „54-60 ja“ in `35#271E` |
| N-IN-009 | Manche NPCs behalten Geheimnisse zuverlässig, andere erzählen alles weiter | 📋 | `35#270A` Frage 55; „54-60 ja“ in `35#271E` |
| N-IN-010 | Öffentliche Bekanntheit des Spielers erhöht die Verbreitung stark; eine berühmte Person kann Dinge schwerer privat halten | 📋 | `35#270A` Fragen 56–57; „54-60 ja“ in `35#271E` |
| N-IN-011 | Social Media, Nachrichten und Fotos spielen je nach Kontext eine Rolle | 📋 | `35#270A` Frage 58; „54-60 ja“ in `35#271E` |
| N-IN-012 | Der Spieler kann versuchen, Gerüchte zu korrigieren oder zu ignorieren; ein falsches Gerücht kann Beziehungen real beschädigen, bis es aufgeklärt ist | 📋 | `35#270A` Fragen 59–60; „54-60 ja“ in `35#271E` |
| N-IN-013 | Beziehungsereignisse (Fremdgehen, Trennung) können sich über Freundeskreise verbreiten; Freundeskreise können Beziehungswissen weitergeben | 📋 | `35#270A` Fragen 51–52; „22-52 ja“ in `35#271E` |
| N-IN-014 | Social-Media-Feed: praktisch endlos, **nie** für Hauptstory oder normale handgeschriebene Nebenfälle erforderlich; kann optionale dynamische Weltereignisse zeigen; Posts können echten aktuellen Ereignissen entsprechen; falsche oder irrelevante Inhalte sind nötig, damit der Feed kein perfekter Übernatürlich-Detektor ist | 📋 | `M-PD` ‚Smartphone / social media / dynamic world events‘ |
| N-IN-015 | NPC-Aufnahmen und Livestreams können digitale Beweise sein und die Geheimhaltung erschweren | 📋 | `M-PD` ‚Smartphone / social media / dynamic world events‘ |
| N-IN-016 | Mobilfunkabdeckung variiert; in Wäldern, Höhlen und Bergen gibt es Funklöcher; Satellitenkommunikation ist eine teure Lösung | 📋 | `23#1088A` Fragen 148–150; „145-158 ja sehr gut“ in `23#1089E`; `23#1090A` |
| N-IN-017 | Vermisstenmeldungen können in sozialen Medien erscheinen; es gibt echte und falsche Monsterposts, Scams, Hidden-Web-artige Bereiche | 📋 | `23#1088A` Fragen 152–157; „145-158 ja sehr gut“ in `23#1089E` |
| N-IN-018 | Neue Ereignisse brauchen Vorlaufphasen mit schwachen Signalen (Zeitungsmeldungen, Vermisstenanzeigen, Polizeifunk, Gerüchte, lokale Gespräche, Tierverhalten) | 📋 | `M-PD` ‚Organic world-event / case emergence‘ |
| N-IN-019 | Medien berichten über tatsächliche Weltereignisse (Radio, TV, Websites, Zeitung); ein Monsterangriff kann offiziell als Tierattacke bezeichnet werden | 🧭 | `23#603A` Abschnitt 27 (Vorschlag des Assistenten); gestützt durch `23#1056A` Frage 84; „ja“ in `23#1057E` |
| N-IN-020 | Behörden, Medien, Unternehmen und Bevölkerung reagieren auf Ereignisse und passen ihr Verhalten langfristig an; dazu gehören Medienberichte, Gerüchte, Vertuschung, Zeugeneinschüchterung | 📋 | `48#145E`; Auslegung `48#146A` (💡) |
| N-IN-021 | Monster können Informationen über Hunter und Gefahren weitergeben | 📋 | `48#139E`, `48#141E`; [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §5 |
| N-IN-022 | Menschliche Gegner teilen Informationen nur über plausible Kommunikation | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |

---

## 2. Entwurf: Kanäle

Die Quellen nennen viele Kanäle, aber nie ihre Eigenschaften. Eine Tabelle der Eigenschaften (💡):

| Kanal | Reichweite | Geschwindigkeit | Treue | Dauer | Voraussetzung |
|---|---|---|---|---|---|
| **Gespräch von Angesicht** | Anwesende | sofort | hoch bei Augenzeugen, sinkt bei Weitergabe | flüchtig | Begegnung (Alltag, Arbeit, Familie) |
| **Telefon, Sprachnachricht** | Kontaktliste | sofort bis Stunden | hoch | flüchtig | Empfang (N-IN-016) |
| **Textnachricht, Chat** | Kontaktliste, Gruppen | Minuten bis Stunden | hoch (wörtlich) | dauerhaft | Empfang, Kommunikationsstil (Modul 04) |
| **Soziale Medien** | Follower, Gruppen, Öffentlichkeit | Minuten | gering bis hoch | dauerhaft | Konto, Nutzungsgewohnheiten |
| **Lokale Zeitung, Radio, TV** | Region | Stunden bis Tage | mittel (Redaktion, Offizielles) | Archiv | Berichterstattung (Modul 15, Ermittlung 26) |
| **Polizeifunk, Behördenmitteilung** | Behörde, Eingeweihte | sofort | hoch, intern | dokumentiert | Funkzugang |
| **Brief, Notiz, Aushang** | Orte, Adressaten | Tage | hoch | dauerhaft | Medium vorhanden |
| **Mundpropaganda in Gemeinschaften** | Dorf, Viertel, Verein | Tage | niedrig | oft Jahre | Sozialer Kreis |
| **Organisationsnetz** (Gang, Hunter, Kult) | Mitglieder | je nach Struktur | je nach Disziplin | intern | Mitgliedschaft (Modul 16) |

**Regeln (Entwurf):**

1. **Kein Kanal ohne Weg im Weltzustand.** Ein NPC erfährt etwas nur über einen Kanal, der ihn erreicht (N-IN-022 für Gegner; allgemein N-GE-008).
2. **Technische Hindernisse zählen:** Funkloch, leerer Akku (stark vereinfacht), ausgeschaltetes Telefon, kein Internet (N-IN-016).
3. **Jeder Kanal hat einen Verzerrungstyp:** Gespräch vereinfacht und dramatisiert; Social Media verkürzt und verdreht; Medien rahmen und vertuschen; Behörde ordnet und schweigt.

---

## 3. Entwurf: Wer erzählt was wem

Weitergabe ist eine **Entscheidung** des NPC (N-IN-009). Der Entwurf (💡):

```text
NPC besitzt Eintrag E                                    -- Modul 05
   │
   ├─ Anlass zu reden:  Begegnung, Telefonat, Treffen, Feier, Arbeit
   ├─ Interesse:        Neuigkeitswert, Dramatik, betrifft Zuhörer, betrifft Spieler
   ├─ Zuhörer:          Beziehung, Vertrauen, Gemeinsamkeit
   ├─ Hemmung:          Geheimhaltung, Scham, Loyalität, Angst vor Folgen
   └─ Neigung:          Gesprächigkeit des Kerns (Modul 04)
   ▼
erzählt | verschweigt | verschleiert | verfälscht | verkauft
```

| Einflussgröße | Wirkung |
|---|---|
| Gesprächigkeit (Kern) | Menge der Weitergabe; ‚Plaudertasche‘ gegen ‚Geheimnisträger‘ |
| Relevanz für Zuhörer | höhere Weitergabe |
| Dramatik | Geschichten mit Gefahr, Skandal, Liebe, Geld, Tod werden häufiger erzählt |
| Vertraulichkeit | Wer ein Geheimnis versprach, erzählt es seltener, nach Persönlichkeit |
| Eigeninteresse | Er erzählt, was ihm nützt; verschweigt, was ihm schadet |
| Beziehung zum Betroffenen | Loyalität senkt, Rache erhöht |
| Zustand | Rausch, Aufregung, Stress erhöhen Weitergabe |

**Konsequenz für den Spieler:** Was er einem NPC anvertraut, kann weitergegeben werden. Wem er es sagt, bestimmt, wie weit es kommt (Abschnitt 9).

---

## 4. Entwurf: Das Gerüchteobjekt

Gerüchte sind in den Quellen ein Phänomen, kein Datentyp. Für die Simulation braucht es ein Objekt, das **nicht in jedem NPC kopiert** wird, sondern als Weltobjekt mit Verweisen existiert (💡):

```text
Rumor
├── id
├── origin_event       Weltereignis, aus dem es stammt (kann fehlen: erfunden)
├── origin_source      wer hat es zuerst gestreut (NPC, Medium, Fraktion, Spieler)
├── content            aktuelle Fassung (Aussage, Subjekte, Ort, Zeit, Wertung)
├── truth_distance     wie weit entfernt von der Wahrheit (nur für Tests)
├── holders[]          wer es kennt (NPC-IDs oder soziale Kreise)
├── strength           wie stark es im Umlauf ist
├── channel_mix        wie es sich verbreitet
├── age                Weltzeit seit Entstehen
├── status             rising | stable | fading | refuted | confirmed | settled
└── links              betroffene Personen, Orte, Fälle, Fraktionen
```

**Entwurfsentscheidungen:**

1. **Gerücht und Erinnerung sind getrennt.** Ein NPC, der ein Gerücht hört, legt eine Erinnerung der Art `told` an, die auf das Gerüchteobjekt verweist (Modul 05). Das Objekt hält die gemeinsame Fassung; der NPC seine eigene Deutung.
2. **Ein Gerücht hat eine Fassung.** Verzerrung ändert die Fassung, nicht die Geschichte des Ursprungs (`origin_event`).
3. **Bestätigen und Widerlegen.** Ein Gerücht wird `confirmed`, wenn belegtes Wissen oder eine offizielle Aussage es stützt; `refuted`, wenn Beweis oder Dementi es widerlegt; `settled`, wenn es als Legende in die Ortsgeschichte eingeht (Modul 05 §9).
4. **Besitzer sind Kreise.** Für Hintergrundbevölkerung sind Halter soziale Kreise (Haushalt, Arbeitsplatz, Viertel), nicht jeder einzelne NPC (Abschnitt 11).

---

## 5. Entwurf: Verzerrung

Die Quellen verlangen falsche, verzerrte, übertriebene, unvollständige Gerüchte (N-IN-002). Die Ermittlungs-Dokumentation entwirft eine Kette ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §12.1). Hier die Typen (💡):

| Typ | Wirkung | Beispiel |
|---|---|---|
| **Kürzung** | Details fallen weg | ‚ein Mann‘ statt ‚ein Mann in blauer Jacke‘ |
| **Übertreibung** | Maßstäbe wachsen | ‚zwei Verletzte‘ → ‚ein Massaker‘ |
| **Vereinfachung** | Komplexes wird eindeutig | Ursache wird auf eine Person gelegt |
| **Verwechslung** | Person, Ort oder Zeit werden vertauscht | der falsche Bruder |
| **Ausschmückung** | Details werden erfunden | Waffe, Motiv |
| **Einfärbung** | Wertung des Erzählers | Angst, Hass, Anerkennung |
| **Anpassung an das Weltbild** | Ereignis wird so erzählt, dass es passt | ‚Dämon‘, ‚Alien‘, ‚Drogendealer‘ |
| **Vertuschung** | Teile werden weggelassen | offizielle Version |

**Regeln (Entwurf):**

1. **Begrenzte Verzerrung je Schritt** (N-PS-016): pro Weitergabe höchstens eine Änderung je Merkmal.
2. **Deterministisch** (Seed aus Gerüchte-ID und Weitergabeschritt), damit Verlauf und Tests reproduzierbar sind.
3. **Verzerrung hängt vom Erzähler ab:** Persönlichkeit, Weltbild, Interesse, Zustand.
4. **Sinneswahrnehmung vor Gerücht:** Augenzeugen haben hohe Treue (N-GE-008), Weitererzähler nicht.
5. **Gegenmaßnahmen.** Rückverfolgung zur Quelle (Ermittlung 12 §12.1), Dementi, Gegenbeweis, Zeit.

---

## 6. Entwurf: Soziale Kreise und Reichweite

Das Quellenbild: Information reist **entlang echter Beziehungen** und erreicht nicht plötzlich die ganze Welt (N-IN-007, N-IN-008). Das setzt ein Netz aus Beziehungen voraus (Modul 10). Der Entwurf (💡):

| Kreis | Typische Mitglieder | Reichweite einer Nachricht |
|---|---|---|
| Haushalt | Partner, Kinder, Eltern, Mitbewohner | sofort, nahezu verlustfrei |
| Enger Freundeskreis | enge Freunde | Stunden bis Tage |
| Arbeitsplatz | Kollegen, Vorgesetzte, Kunden | Tage |
| Nachbarschaft, Viertel | Nachbarn, Stammgäste | Tage bis Wochen |
| Verein, Szene, Kirche | Mitglieder | Wochen |
| Stadt, Region | Bewohner, lokale Medien | Wochen bis Monate |
| Land | nationale Medien, Social Media | nur bei großen Ereignissen |

**Brückenfiguren (Entwurf).** Wenige NPCs verbinden Kreise und beschleunigen Verbreitung: Journalisten, Friseure, Taxifahrer, Barkeeper, Pfarrer, Lehrer, Streamer. Sie haben im Datensatz eine Verbindungsrolle (Modul 10).

**Öffentliche Bekanntheit** erhöht die Zahl der Kreise, die sich für eine Person interessieren (N-IN-010, 📋). Ein bekannter Spieler hat ein größeres „Publikum“: Sein Tun wird eher weitererzählt, ob er es will oder nicht.

**Begrenzung.** Die Welt reagiert auf Sichtbarkeit: Was nur in einem Kreis geschah, bleibt dort, bis jemand es herausträgt.

---

## 7. Gespräche zwischen NPCs und Mithören

**Beschlossen:** NPCs reden untereinander (N-IN-003), der Spieler kann lauschen (N-IN-004), Gespräche behandeln lokale Ereignisse, andere NPCs, Gerüchte (N-IN-005). Eine Bewertung des Assistenten sagt, Ambient-Gespräche könnten Hinweise, Fälle, Gerüchte oder falsche Fährten erzeugen (`23#510A`).

**Entwurf (💡):**

| Frage | Vorschlag |
|---|---|
| Woher kommen die Inhalte? | aus den Einträgen der Sprecher (Modul 05), nicht aus einer Zufallsliste; unbekannte Hintergrundgespräche aus Ortszustand und Themenpool |
| Wer redet mit wem? | Menschen aus demselben Kreis (Modul 10) bei Pausen, Treffen, Wartezeiten |
| Was hört der Spieler? | Fragmente, abhängig von Entfernung, Lärm, Sprache; Verständnis hängt vom Spieler (Kontext) ab |
| Was löst es aus? | Hinweise, Gerüchte, Fallauslöser, falsche Fährten |
| Was bemerken die Sprecher? | Dass jemand lauscht (N-WA-021), mit Folgen (Verstummen, Ansprechen) |
| Welche Gespräche sind echt? | Nur dann „echt“, wenn die Inhalte einem Weltzustand entsprechen; erfundene Gespräche sind **Atmosphäre**, nicht Information |

**Regel:** Ein Mithörgespräch darf keinen Fall auslösen, den die Welt nicht hergibt (Prüfregel N-EN-083). Das ergibt sich aus der Konsistenzanforderung H (N-LT-008).

**Barks vs. Gespräch.** Kurze Gameplay-Zeilen (Barks) sind etwas anderes als Gespräche zwischen NPCs; beide folgen dem Dialogsystem (`M-PD` ‚Dialogue / conversation architecture‘; [Modul 20](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md)).

---

## 8. Entwurf: Medien

Die Quellen nennen Medien als Kanal und als Vertuschungsinstrument (N-IN-018 bis N-IN-020). Für NPCs gilt (💡):

| Aspekt | Entwurf |
|---|---|
| **Medienkonsum** | Jeder NPC hat Gewohnheiten (Zeitung, Lokal-TV, Radio, Social Media, Podcasts). Sie bestimmen, was er mitbekommt und wann |
| **Redaktionen** | Medienorganisationen sind Fraktionsähnliche Akteure mit Zielen, Quellen, Haltung und Einflüssen (Modul 17) |
| **Verzögerung** | Medienbericht folgt Ereignis nach Stunden bis Tagen (Ermittlung 26 §8) |
| **Rahmung** | Berichte geben eine Version; Behörden können sie beeinflussen (`48#146A`) |
| **Dementi und Folgebericht** | ein Bericht kann berichtigt werden; die berichtigte Fassung erreicht nicht alle |
| **Archive** | Berichte sind dauerhaft (Zeitungsarchiv) und für die Ermittlung nutzbar |
| **Fehlinformation** | Zeitungsenten, Fake-Videos, Satire; die Mehrzahl der Social-Media-Inhalte ist Alltag (N-IN-014) |
| **Wirkung auf NPCs** | Nachrichten verändern Meinung, Angst, Verhalten und Tourismus (N-IN-006, `23#1088A` Frage 171) |

**Hinweis zum Spieler.** Wenn der Spieler eine öffentliche Tat begeht (Brand, Rettung, Schießerei), kann ein Medium darüber berichten (`23#603A` Abschnitt 27, Beispiel: Rettung aus einem brennenden Hotel). Das ist eine Folge der Weltsimulation, nicht ein Beleg für Spieler-Ruhm (selten: `23#1057E` Punkt 45).

---

## 9. Entwurf: Spielerhandlungen

| Handlung | Wirkung | Quelle |
|---|---|---|
| Etwas anvertrauen | Information gelangt in den Kreis des NPC | N-IN-009 |
| Gerücht streuen | gezielte Fassung, mit Quelle Spieler (kann auffliegen) | 💡 |
| Gerücht korrigieren | Dementi, Beweis, Zeugen; ist Mühe und nicht garantiert | N-IN-012 |
| Gerücht ignorieren | läuft weiter; Wirkung bleibt | N-IN-012 |
| Schweigegeld zahlen | Zeuge schweigt, kann aber auch später reden | N-WA-012 |
| Einen Zeugen einschüchtern | kurzfristig Schweigen; langfristig Groll (Modul 08) | N-WA-012 |
| Medien nutzen | Pressemitteilung, Tipp, Leak | 💡 |
| Verbreitung bremsen | Netzbrücke ausschalten, Quelle ausschalten | 💡 |
| Im Verborgenen handeln | keine Zeugen, keine Gerüchte (N-WA-013) | N-WA-013 |
| Berühmtheit vermeiden | Medienbekanntheit kommt vor, aber nicht oft | „ja, kann passieren aber nicht oft“ in `23#1057E` Punkt 45 |

---

## 10. Entwurf: Wirkungen

1. **Ruf und Beziehung.** Gerüchte verändern die Sicht auf den Spieler in den erreichten Kreisen ([Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) §4). Ein falsches Gerücht kann reale Beziehungen beschädigen, bis es aufgeklärt ist (N-IN-012).
2. **Verhalten von Gemeinschaften.** Menschen meiden Orte (N-IN-006); Besucherzahl und Tourismus ändern sich (`23#1088A` Frage 171, „167-173 ja“ in `23#1089E`); Geschäfte reagieren.
3. **Fallentstehung.** Gerüchte können Fälle auslösen oder verdecken ([Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md)): Vorlaufphasen (N-IN-018).
4. **Fraktionen.** Gerüchte beeinflussen Mitgliedschaft, Misstrauen, Spaltung (Modul 16). Fraktionen können Desinformation streuen (Modul 17).
5. **Behörden.** Behörden reagieren auf Gerüchte und Medien; die Polizei untersucht oder erklärt weg (N-WA-017).
6. **Persönliche Folgen.** Ein Gerücht über Untreue, Verschuldung oder Krankheit hat Folgen für den Betroffenen und seine Umgebung (N-IN-013).

---

## 11. Entwurf: Skalierung

Die Quellen verlangen glaubwürdige Gesellschaft bei bis zu mehreren Hunderttausend NPCs, ohne alle individuell zu rechnen (Modul 22). Für Information gilt (💡):

1. **Gerüchte sind Weltobjekte mit Haltern auf Kreisebene.** Erst bei Kontakt mit dem Spieler werden einzelne Halter materialisiert (z. B. wer im Diner davon spricht).
2. **Ausbreitung ist ereignisbasiert.** Zeit zwischen Weitergaben, Stärke und Reichweite werden in Schritten (Stunden/Tage) gerechnet; wer im Spielraum ist, rechnet genauer.
3. **Rundläufer und Verdünnung.** Ein Gerücht wird mit der Zeit schwächer, außer es wird erneuert; der Eintrag wandelt sich zu Ortswissen oder verschwindet.
4. **Obergrenze der aktiven Gerüchte je Region**, mit Verdrängung des Unwichtigsten (Zahl offen, Q-N16).
5. **Rekonstruierbarkeit.** Bei Materialisierung eines Kreises ergibt sich dieselbe Verteilung wie bei einer früheren Materialisierung unter gleichem Weltzustand (Seed, Modul 22).
6. **Co-op.** Der Host führt Gerüchte; Gäste sehen deren Wirkung (Modul 23).

---

## 12. Sonderfälle

1. **Funkloch.** Eine Nachricht wird erst zugestellt, wenn Empfang besteht; Verzögerung ist Teil des Ablaufs (N-IN-016).
2. **Gefälschte Beweise (Deepfake, Fotomontage).** Eine falsche Aufnahme ist ein Ereignis mit Quelle (wer hat sie hergestellt); sie wird als `refuted` geführt, wenn aufgedeckt ([Ermittlung 27](../Ermittlung/27-Medien-und-digitale-Spuren.md)).
3. **Zensur und Vertuschung.** Eine Behörde unterdrückt eine Meldung; Gerücht und Gegengerücht entstehen (`48#146A`, Beispiel ‚Vertuschung‘).
4. **Verleumdung.** Ein falsches Gerücht über eine Person kann rechtliche Folgen haben (Klage) — Ermittlung 15.
5. **Gerücht über den Spieler als Täter.** Das Gerücht kann Fahndung und Verdacht auslösen (Ermittlung 12 §12.1).
6. **Gerücht über Übernatürliches in einer Region mit starken Legenden.** Es wird eher geglaubt (`48#148A`).
7. **Gerücht aus der Zukunft** (Zeitreise). Gilt nur in dem Zeitstand, in dem es entstand (N-GE-028).
8. **Kollektive Hysterie.** Viele NPCs reagieren ungewöhnlich stark, weil ein Gerücht Schwellen überschreitet (Panik, Flucht, Proteste, Selbstjustiz). Das ist eine Fraktions-/Gemeinschaftsdynamik (Modul 15, 17), kein Einzelverhalten.
9. **Sprachbarriere.** Region und Akzent können die Weitergabe erschweren; offen, ob zweisprachige Gemeinden modelliert werden (Q-N20).
10. **Tote Gerüchte.** Ein Gerücht kann erlöschen, wenn alle Halter tot oder vergessen sind (Modul 05).

---

## 13. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-081 | Eine Information erreicht einen NPC nur über einen Kanal, der im Weltzustand existiert und den NPC erreicht | N-IN-001, N-IN-022 |
| N-EN-082 | Ein Gerücht kann die Wahrheit verzerren, aber nicht aus dem Nichts eine Person erfinden, die es nicht gibt | N-IN-002 |
| N-EN-083 | Ein belauschtes Gespräch löst keinen Fall und keinen Hinweis aus, der nicht im Weltzustand begründet ist | N-IN-004, N-LT-008 |
| N-EN-084 | Die Ausbreitung ist durch den sozialen Graphen begrenzt; ein Gerücht erreicht keinen NPC ohne Verbindung | N-IN-007, N-IN-008 |
| N-EN-085 | Dasselbe Gerücht wird bei gleicher Eingabe gleich verzerrt (deterministisch) | Ermittlung 12 §4 |
| N-EN-086 | Ein Funkloch verzögert oder verhindert Nachrichten | N-IN-016 |
| N-EN-087 | Medienberichte erscheinen nach dem Ereignis, nie davor | N-IN-019 |
| N-EN-088 | Ein widerlegtes Gerücht bleibt in den Erinnerungen derer, die es glaubten, bis sie davon erfahren | N-GE-009 |
| N-EN-089 | Ein Geheimnisträger gibt ein Geheimnis nicht ohne Anlass weiter; ein Plauderer häufiger | N-IN-009 |
| N-EN-090 | Der Spieler kann ein Gerücht nicht ohne Mittel, Aufwand und Quelle korrigieren | N-IN-012 |
| N-EN-091 | Der Social-Media-Feed ist nie Voraussetzung für Hauptstory oder handgeschriebene Nebenfälle | N-IN-014 |
| N-EN-092 | Ein NPC verbreitet Wissen, das er hat; er erfindet keines | N-GE-008 |
| N-EN-093 | Öffentliche Bekanntheit erhöht die Verbreitungsrate, ändert aber nicht die Wahrheit des Inhalts | N-IN-010 |

---

## 14. Was fehlt

- Keine Zahlen: Reichweiten, Geschwindigkeiten, Verzerrungsraten, Grenzen aktiver Gerüchte (Q-N16).
- Keine Festlegung, ob Gerüchte als Weltobjekte oder als Einträge pro NPC geführt werden (Entwurf: Weltobjekte).
- Der Antwortstand zu `35#270A` Frage 53 ist unvollständig (N-IN-007).
- Keine Aussage der Quellen zur Verbreitung **zwischen Bundesstaaten** oder zu nationalen Medien.
- Kein Entwurf für die Schnittstelle zwischen Social-Media-Inhalten (Spieler-Smartphone) und der Gerüchtesimulation.
