# NPCs 15: Reaktion auf Weltereignisse und Übernatürliches

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie einzelne NPCs und Gemeinschaften auf Ereignisse reagieren: Unfälle, Katastrophen, Verbrechen, Großereignisse und übernatürliche Vorfälle. Beschlossen sind die Richtung (Menschen meiden Orte, Gerüchte entstehen, Polizei sucht normale Erklärungen, Reaktionen sind verschieden, Begegnungen verändern, Behörden, Medien und Bevölkerung reagieren dynamisch); **Reaktionsmodell, Schwellen und Verläufe sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Ereignisfamilien, Lebenszyklus und Einsatzketten stehen in [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md), Vorfälle der Monsterökologie in [Kreaturen 15](../Kreaturen/15-Vorfaelle-und-lebendige-Welt.md), Geheimhaltung in [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Reaktionsmodell des Einzelnen](#2-entwurf-reaktionsmodell-des-einzelnen)
3. [Entwurf: Reaktionstypen](#3-entwurf-reaktionstypen)
4. [Entwurf: Reaktion nach Ereignisstufe](#4-entwurf-reaktion-nach-ereignisstufe)
5. [Entwurf: Gemeinschaften reagieren](#5-entwurf-gemeinschaften-reagieren)
6. [Entwurf: Das Übernatürliche im Alltag](#6-entwurf-das-übernatürliche-im-alltag)
7. [Entwurf: Dauerhaft veränderte NPCs](#7-entwurf-dauerhaft-veränderte-npcs)
8. [Entwurf: Meidung und Verhaltensänderung](#8-entwurf-meidung-und-verhaltensänderung)
9. [Entwurf: Behörden, Medien, Wirtschaft](#9-entwurf-behörden-medien-wirtschaft)
10. [Entwurf: Reparatur und Wiederaufbau](#10-entwurf-reparatur-und-wiederaufbau)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-WE-001 | Monster beeinflussen NPC-Routinen indirekt: Menschen meiden nach mehreren Vorfällen einen Wald oder eine Straße | 📋 | `23#1056A` Frage 114; „ja sicher“ in `23#1057E` |
| N-WE-002 | Gerüchte über Vorfälle verbreiten sich in der lokalen Community | 📋 | `23#1056A` Frage 115; „ja genau“ in `23#1057E` |
| N-WE-003 | Die Polizei erklärt Dinge zunächst normal (Wildtier, Unfall, Mord), wenn sie das Übernatürliche nicht kennt: „ja so wie in der serie“ | 📋 | `23#1056A` Frage 116; Antwort in `23#1057E` |
| N-WE-004 | NPCs reagieren unterschiedlich auf Übernatürliches (Unglaube, Angst, Religion, Neugier, Verdrängung) | 📋 | `23#1056A` Frage 117; „genau“ in `23#1057E` |
| N-WE-005 | Bestimmte NPCs können nach einer Begegnung dauerhaft verändert sein | 📋 | `23#1056A` Frage 118; „ja“ in `23#1057E` |
| N-WE-006 | Tiere reagieren teilweise früher | 📋 | `23#1056A` Frage 119; „ja“ in `23#1057E` |
| N-WE-007 | Große übernatürliche Ereignisse können vorübergehend ganze Stadtbereiche beeinflussen, bleiben aber **selten** | 📋 | `23#1056A` Frage 120; „ja genau“ in `23#1057E` |
| N-WE-008 | Der Alltag ist verbunden: Vorfälle beeinflussen Straßen, Wälder, Besucherzahlen, Gerüchte, Tourismus, Polizeiverhalten, Menschen; manche glauben gar nichts, andere an Religion, Geister, Aliens, Verschwörungen oder tatsächlich an Monster; manche verdrängen | 📋 | `23#1056A` Fragen 114–120; Antworten in `23#1057E`; `23#1058A` ‚Supernatural-Reaktion‘ |
| N-WE-009 | Kleine, mittlere, große und extrem seltene Ereignisse (Straßensperren, Unfälle, Brände, Proteste, Stürme, Überschwemmungen, Tornado, Blackout) haben echte Konsequenzen; Städte können Schäden davontragen; NPC-Teams reparieren, der Spieler kann helfen | 📋 | `23#1056A` Fragen 77–83; „ja“ und „ja, die stadt kann auch schäden davontragen“ in `23#1057E` |
| N-WE-010 | Radio, TV, Websites und Zeitung berichten über Ereignisse; passende Ereignisse können Unternehmen und Aktien beeinflussen | 📋 | `23#1056A` Fragen 84–85; „ja“ in `23#1057E` |
| N-WE-011 | Menschenmengen, Stadien, Festivals, Nachtleben und Skigebiete haben echte Besucherströme und Routen | 📋 | `23#1056A` Fragen 86–92; „ja“ in `23#1057E` |
| N-WE-012 | Die menschliche Gesellschaft reagiert dynamisch: Polizei und Behörden (Ermittlungen, Fahndungen, Absperrungen, Sondereinheiten), Politik und Regierung (Krisenentscheidungen, Sicherheitsmaßnahmen, geheime Programme), Medien, Geheimhaltung, Organisationen, Bevölkerung (Angst, Misstrauen, Fluchtbewegungen, Proteste, Alltag), Wirtschaft, Langzeitfolgen | 📋 | „Supernatural Reaktion der menschlichen Gesellschaft: Umfassende Gesellschaftssimulation mit Politik, Vertuschung und organisierten Gegenmaßnahmen“ in `48#145E`; Tabelle `48#146A` (💡) |
| N-WE-013 | Ein Beispielverlauf des Assistenten: Vermisste → Polizei sieht gewöhnliche Fälle → Eskalation mit Medien → Vertuschung durch einflussreiche Gruppe → Gegenmaßnahmen von Behörden oder Hunter-Organisationen → Langzeitfolgen für die Stadt auch ohne Zutun des Spielers | 💡 | `48#146A` (Beispiel) |
| N-WE-014 | Das öffentliche Wissen ist regional verschieden und kann sich ändern | 📋 | „Regionales Wissen und dynamische Veränderungen der öffentlichen Wahrnehmung“ in `48#147E` |
| N-WE-015 | Ereignisse haben Vorlaufphasen mit schwachen Signalen (Zeitungsmeldungen, Vermisstenanzeigen, Polizeifunk, Gerüchte, lokale Gespräche, Tierverhalten, ungewöhnliche Verletzungen, Umweltanomalien); ein ignoriertes Ereignis kann mehr Opfer, Polizeieinsatz, Eingreifen eines anderen Hunters, Wandern der Kreatur, veränderte Beweise oder einen späteren ungelösten Fall zur Folge haben | 📋 | `M-PD` ‚Organic world-event / case emergence‘ |
| N-WE-016 | Nicht jeder Mensch im Monsterkostüm ist ein Monster; nicht jedes Halloween-Event ist übernatürlich | 📋 | „159-166 ja“ in `23#1089E`; `23#1090A` ‚Feiertage & Kalender‘ |
| N-WE-017 | Bevölkerung, Wirtschaft, Tourismus und Immobilien reagieren auf Monsterfälle (Monsterfälle beeinflussen Immobilien, Tourismus, Polizei, Medien, Verkehr, Firmen, Krankenhaus, Tiere, lokales Wetter, NPC-Gerüchte) | 🧭 | `23#1066A` Punkt 30 (Vorschlag des Assistenten); „167-173 ja“ in `23#1089E` (Tourismus) |
| N-WE-018 | Verfluchte Objekte und Haunted Properties wirken auf Immobilienmarkt und Wanderung: ein billiges Haus wird verkauft, weil dort Dinge passieren | 🧭 | `23#1066A` Punkte 28–29 (Vorschlag des Assistenten); [Kreaturen 16](../Kreaturen/16-Uebernatuerliche-Orte-und-Geografie.md) |
| N-WE-019 | Krisen und Ereignisse unterbrechen den Alltag; NPCs reagieren je nach Persönlichkeit und Lage (Reaktionen auf Weltereignisse als Teil der unabhängigen Lebensentwicklung) | 📋 | `PE-N1022` F |

---

## 2. Entwurf: Reaktionsmodell des Einzelnen

Die Reaktion eines NPC auf ein Ereignis folgt der Kette *Wahrnehmung → Deutung → Bewertung → Handlung* (Modul 06 §2). Zusätzlich braucht es eine **Rolle** und eine **Lage** (💡).

```text
Ereignis E
  │
  ├─ Betroffenheit:  direkt (Opfer, Zeuge, Anlieger) | indirekt (Angehörige, Gemeinschaft) | fern
  ├─ Wahrnehmung:    sieht / hört / erfährt (Modul 06, 07)      -- nein: keine Reaktion
  ├─ Deutung:        was glaubt er?   (Weltbild, Vorwissen, Quelle)
  ├─ Bewertung:      Gefahr, Nutzen, Pflicht, Neugier, Mitgefühl
  ├─ Rolle:          Beruf (Polizist, Arzt, Feuerwehr), Verantwortung (Eltern), Zugehörigkeit (Gruppe)
  ├─ Persönlichkeit: Mut, Mitgefühl, Misstrauen, Neugier, Ängstlichkeit  (Modul 04)
  ├─ Lage:           Ort, Zeit, Zustand, Begleiter, Besitz
  └─ Handlung:       Reaktionstyp (Abschnitt 3), Dauer, Folgeverhalten
```

**Regeln (Entwurf):**

1. **Betroffenheit bestimmt Intensität.** Direkt Betroffene handeln, entfernte Menschen sprechen und meiden.
2. **Rolle schlägt Neigung.** Ein Feuerwehrmann läuft hin, auch wenn er ängstlich ist (Pflicht), ein Zivilist nicht.
3. **Nicht alle reagieren gleich.** Derselbe Anblick löst bei zwei NPCs verschiedene Reaktionen aus (N-WE-004).
4. **Reaktion erzeugt Erinnerung.** Jeder Beteiligte legt Einträge an (Modul 05).
5. **Reaktion wirkt auf die Welt.** Wer flieht, geht; wer filmt, erzeugt Material; wer ruft, löst Einsatzketten aus (Ermittlung 26 §4).

---

## 3. Entwurf: Reaktionstypen

| Typ | Beschreibung | Typische Persönlichkeit/Rolle | Folge |
|---|---|---|---|
| **Eingreifen, Helfen** | greift ein, rettet, leistet erste Hilfe | mutig, mitfühlend, Rettungsberuf | Dankbarkeit, Verletzung |
| **Notruf** | ruft Polizei, Rettung | die meisten | Einsatzkette (Ermittlung 26) |
| **Flucht** | verlässt den Ort | ängstlich, Eltern mit Kindern | Zeugen verlieren die Szene |
| **Schutz suchen** | versteckt sich, verbarrikadiert | ängstlich | Überlebende |
| **Beobachten** | schaut zu, wartet ab | neugierig, unentschlossen | Zeugenschaft |
| **Gaffen, Filmen** | filmt, postet | smartphone-affin | digitale Spur (N-IN-015) |
| **Verdrängen** | ignoriert, bagatellisiert | verleugnend | Erinnerung unterdrückt (Modul 05) |
| **Erklären** | sucht normale Deutung | rational, skeptisch | Gerüchte (Modul 07) |
| **Glauben** | deutet übernatürlich oder religiös | gläubig, abergläubisch | Gebet, Kirche, Magie-Rituale |
| **Verschwörung** | deutet als Komplott | misstrauisch | Online-Gruppen, Desinformation |
| **Neugier** | will mehr wissen | neugierig, Abenteurer | Dark-Tourismus, Urbex |
| **Nutzen** | nutzt Chance (Plünderung, Betrug, Verkauf) | skrupellos, bedürftig | Kriminalität |
| **Organisieren** | Nachbarschaftshilfe, Wache, Spendenaktion | gemeinschaftsorientiert | Gruppen entstehen (Modul 16) |
| **Rache, Selbstjustiz** | will bestrafen | verletzt, wütend | Gewalt, Gerüchte |
| **Zusammenbruch** | Schock, Trauma | stark betroffen | Lebenszustand (Modul 03 §7) |
| **Resignation** | zieht sich zurück | niedergeschlagen | Rückzug, Sucht |

**Verteilung.** Die Mischung der Reaktionen in einer Menge ist eine Funktion der Zusammensetzung (Alter, Rollen, Bildung), der Stimmung und des Ereignisses. Der Spieler soll in einer Menge Vielfalt sehen, keine Uniformität (N-WE-004).

---

## 4. Entwurf: Reaktion nach Ereignisstufe

Die vier Stufen sind beschlossen (klein, mittel, groß, extrem selten; N-WE-009; [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md) §2). Wie NPCs reagieren, hängt von der Stufe ab (💡):

| Stufe | Beispiele (aus den Quellen) | NPC-Reaktion (Entwurf) | Gemeinschaftsfolge |
|---|---|---|---|
| **klein** | Straßensperre, liegengebliebenes Auto, Streit, verlorenes Tier, kaputte Ampel | Hilfe, Umweg, Ärger, Gespräch | keine bleibende |
| **mittel** | Unfall, kleiner Brand, Stromausfall, Protest, Sportereignis, Fest | Anteilnahme, Gaffen, Einsatzkräfte, Umplanung | lokale Gespräche, Berichte |
| **groß** | schwerer Sturm, Überschwemmung, Großbrand, großer Unfall, wichtiges Sportereignis | Evakuierung, Hilfe, Not, Trauer, Zusammenhalt | Wochen bis Monate sichtbar (N-WE-009) |
| **extrem selten** | Tornado, Blizzard, großer Blackout | Panik, Rettung, Plünderung, Heldentum, Trauma | langfristige Veränderungen der Stadt |
| **übernatürlich (selten)** | Ereignisse mit Wirkung auf Stadtbereiche | Angst, Rückzug, Flucht, Glaube, Verschwörung | vorübergehende Beeinflussung eines Bereichs (N-WE-007) |

**Regeln (Entwurf):**

1. **Die Reaktionsstärke wächst mit Stufe und Betroffenheit**, nicht linear.
2. **Gewöhnung.** Häufige kleine Ereignisse werden zur Routine; seltene große erschüttern (Persönlichkeit moduliert).
3. **Rangfolge.** Gefahr für Leben vor Eigentum vor Bequemlichkeit.
4. **Gemeinschaft stützt.** Nach großen Ereignissen steigen Hilfe und Zusammenhalt (Q-N24 offen).
5. **Folgen auf Alltag.** Straßen, Läden, Arbeit, Schule sind betroffen (Modul 13 §4).

---

## 5. Entwurf: Gemeinschaften reagieren

Anforderung D verlangt, dass Gruppen und Gesellschaft eigene Strategien haben; das Quiz bestätigt eine umfassende Gesellschaftssimulation (N-WE-012). Auf Ebene der Gemeinschaft (Viertel, Stadt, Region) gilt (💡):

| Muster | Beschreibung | Auslöser | Wirkung |
|---|---|---|---|
| **Angstklima** | Menschen gehen nachts nicht raus, schließen früher | Vermisste, Morde, Gerüchte | Nachtleben leidet, Polizei verstärkt |
| **Meidung** | Ort wird gemieden (Wald, Straße, Haus) | wiederholte Vorfälle | Besucherzahlen sinken (N-WE-001) |
| **Dark-Tourismus** | Neugierige strömen | Legendenbildung | Hotels, Souvenirs, Konflikte |
| **Zusammenhalt** | Nachbarn helfen, Wachen, Spenden | große Not | neue Gruppen und Beziehungen |
| **Misstrauen** | Fremde werden beargwöhnt | Gerüchte, Täter unbekannt | Fremdenfeindlichkeit, Selbstjustiz |
| **Spaltung** | Lager (Gläubige/Zweifler, Schuldzuweiser) | Deutung | Konflikte, Proteste |
| **Schuldzuweisung** | Sündenböcke (Obdachlose, Fremde, Gruppen) | Angst | Gewalt, Ausgrenzung |
| **Wegzug** | Familien ziehen weg | Dauerangst | Immobilienpreise, Schulen |
| **Zuzug** | Menschen kommen (Journalisten, Neugierige, Helfer) | Medien | Belastung, Chancen |
| **Proteste, Forderungen** | Bürger fordern Maßnahmen | Unzufriedenheit | Politik reagiert (Modul 18) |
| **Verdrängung** | Gemeinschaft schweigt | Scham, Wirtschaftsinteresse | Vertuschung (Ermittlung 17) |
| **Legendenbildung** | Ereignis wird Teil der Ortsgeschichte | Zeit | Ortsgedächtnis (Modul 05 §9) |

**Mechanik (Entwurf).** Gemeinschaftszustände sind **Gruppenvariablen** (zum Beispiel Angstniveau, Misstrauen, Wirtschaftslage) mit Trägheit; sie werden aus Ereignissen und Gerüchten gefüttert und beeinflussen die Verteilung der Einzelreaktionen. Der Spieler sieht sie über Stimmung, Verkehr, Läden, Gespräche. Offen, welche Variablen es gibt (Q-N24).

**Ausmaß.** Große übernatürliche Ereignisse wirken auf einen Stadtbereich vorübergehend (N-WE-007); Daueränderungen entstehen durch wiederholte Vorfälle und Nicht-Intervention.

---

## 6. Entwurf: Das Übernatürliche im Alltag

**Beschlossen:** Das Übernatürliche ist verborgen, selten, widersprüchlich dokumentiert ([Kreaturen 02](../Kreaturen/02-Leitsaetze-und-Abgrenzung.md) K-GR-002); die Standardannahme der Polizei ist normal (N-WE-003); NPCs reagieren verschieden (N-WE-004).

**Entwurf (💡) – Wie ein NPC ein übernatürliches Ereignis erlebt:**

| Phase | Beschreibung |
|---|---|
| **Ereignis** | etwas geschieht (Angriff, Erscheinung, Spuk, Verschwinden) |
| **Wahrnehmung** | der NPC erkennt es teilweise (Dunkelheit, Angst, Verzerrung) |
| **Deutung** | nach Weltbild: Tier, Verrückter, Gas, Geist, Dämon, Alien, Drogen |
| **Erinnerung** | verzerrt, mit starker Emotion; Anker (Modul 05) |
| **Reaktion** | Abschnitt 3 |
| **Weitergabe** | erzählt oder schweigt; Gerücht (Modul 07) |
| **Folgen** | Verhalten, Orte, Beziehungen, Überzeugung |

**Weltbild-Typen (Entwurf, nach N-WE-008):**

| Typ | Haltung | Reaktion auf Beweis |
|---|---|---|
| **Skeptiker** | glaubt nichts; sucht normale Erklärung | erst bei überwältigendem Beweis; kann sich weigern |
| **Gläubiger (Religion)** | deutet im religiösen Rahmen | sieht Bestätigung |
| **Abergläubischer** | glaubt an Geister, Omen, Flüche | schneller überzeugt |
| **Verschwörungsdenker** | misstraut Behörden; sieht Komplott | integriert Beweis in sein Narrativ |
| **Neugieriger** | forscht, sammelt | wird eventuell Verbündeter oder Gefahr |
| **Verdränger** | will nichts wissen | verleugnet, Gedächtnislücken |
| **Wissende** | kennt die Wahrheit | Hunter, Eingeweihte, Beamte (Kreaturen 11) |
| **Lokaler Traditionsträger** | kennt Überlieferung der Gegend | Regionales Wissen (N-WE-014) |

**Regeln (Entwurf):**

1. **Das Weltbild ist Teil des Datensatzes**, durch Persönlichkeit, Region, Biografie erzeugt (Modul 04, 03).
2. **Ein Weltbild kann sich ändern** durch Erlebnisse (Abschnitt 7).
3. **Wissende existieren in jedem Gebiet in geringer Zahl** (Hunter-Netz selten: `M-B09`).
4. **Die Wahrheit setzt sich nicht von selbst durch.** Beweise müssen gesammelt, geglaubt und weitergegeben werden; öffentliches Wissen ändert sich langsam (N-WE-014).
5. **Der Spieler beeinflusst das öffentliche Wissen** (Geheimhaltung, Enthüllung: `48#149E`; Ermittlung 17).

---

## 7. Entwurf: Dauerhaft veränderte NPCs

N-WE-005 sagt: Bestimmte NPCs können nach einer Begegnung dauerhaft verändert sein. Der Entwurf legt Formen fest (💡):

| Veränderung | Ursache | Folge im Datensatz |
|---|---|---|
| **Traumatisiert** | Angriff, Verlust, Anblick | Angst, Meidung, Schreckhaftigkeit, Alpträume; Persönlichkeitsverschiebung (Modul 04) |
| **Überzeugt (Gläubig/Wissend)** | Beweis, Erlebnis | Weltbild verschiebt sich |
| **Paranoid** | Verfolgung, Betrug | Misstrauen, Rückzug, Aggression |
| **Besessenheit/Fluchfolgen** | übernatürlicher Einfluss | Zustände aus Kreaturen 05 |
| **Gezeichnet (körperlich)** | Verletzung, Verzauberung | Narben, Merkmale, Behinderung |
| **Verdrängend** | psychische Abwehr | Gedächtnislücke (Modul 05 §11) |
| **Neu motiviert** | Verlust, Rettung | Berufswechsel, Hunter werden, Rache |
| **Beschützer** | Rettung durch den Spieler | Loyalität, Schuld (Modul 09) |
| **Aussteiger** | Ausstieg aus einer Gruppe | Zugehörigkeit gelöst (Modul 16) |

**Regeln (Entwurf):**

1. **Nur nach relevantem Erlebnis.** Verändert ist, wer das Erlebnis hatte (N-GE-008).
2. **Mit Ursache und Zeit.** Veränderung entsteht über Wochen bis Monate (N-PS-016), nicht schlagartig, außer bei einem Schock mit sofortigem Zustand.
3. **Teilweise umkehrbar.** Therapie, Zeit, Zuwendung können Traumafolgen mildern (nicht garantiert; N-PS-017).
4. **Folgen für Beziehungen.** Das Umfeld bemerkt die Veränderung (Modul 08), fragt nach, wendet sich ab oder hilft.
5. **Hunter-Rekrutierung.** Veränderte NPCs können Kandidaten für das Hunter-Netz werden (Modul 19).

---

## 8. Entwurf: Meidung und Verhaltensänderung

N-WE-001 verlangt, dass Menschen Orte meiden. Der Entwurf legt Wirkungen auf Zeitpläne fest:

| Meidung | Wirkung auf Zeitplan (Modul 13) | Dauer |
|---|---|---|
| **Wald, Gelände** | Wanderer, Jäger, Camper reduzieren | Wochen bis Jahre |
| **Straße, Viertel** | Umwege, Fahrer meiden, Läden leiden | Wochen |
| **Haus, Ort** | Mieter ziehen weg, Haus steht leer | Monate bis Jahre (Haunted Property) |
| **Nachtstunden** | später raus, weniger Nachtleben | Wochen |
| **Gruppen** | Schüler gehen in Gruppen, Eltern holen ab | Wochen |
| **Veranstaltungen** | Absage, geringe Teilnahme | kurz |
| **Verkehrsmittel** | meiden bestimmte Linien | Wochen |
| **Beruf** | Arbeitsplatz wird gewechselt | Monate |

**Regeln (Entwurf):**

1. **Wiederholte Vorfälle schlagen einzelne.** Meidung entsteht durch Wiederholung oder durch ein einzelnes großes Ereignis (Gerücht verstärkt, Modul 07).
2. **Meidung klingt ab.** Ohne neue Vorfälle verschwindet sie (Gewöhnung); in manchen Fällen entsteht Legende (Ortsgedächtnis).
3. **Gegenreaktionen.** Neugierige und Mutige gehen gerade dorthin (Dark-Tourismus, Urbex: `23#1088A` Fragen 174–176).
4. **Meidung bleibt konsistent mit dem sichtbaren Alltag.** Ein gemiedener Wald ist weniger besucht; Hunter sehen es als Hinweis (Vorlauf, N-WE-015).

---

## 9. Entwurf: Behörden, Medien, Wirtschaft

Die Quellen bestätigen eine umfassende Gesellschaftssimulation (N-WE-012). Die Details gehören zu den Ermittlungs- und Weltmodulen; hier die NPC-Seite (💡):

| Akteur | NPC-Seite | Verweis |
|---|---|---|
| **Polizei** | Beamte als Personen: reagieren nach Persönlichkeit, Dienststelle, Zuständigkeit; suchen normale Erklärungen; Ermittler bilden Hypothesen; Korruption möglich | [Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md), [Modul 18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md) |
| **Rettung, Feuerwehr** | Einsatzkräfte als Personen mit Dienstplan | [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md) |
| **Politik** | Entscheidungen auf Ereignisse (Maßnahmen, geheime Programme) | `48#146A` |
| **Medien** | Redaktionen und Journalisten als Akteure | Modul 07 §8 |
| **Unternehmen** | Betriebe reagieren (Schließung, Verlegung, Sicherheit) | Modul 13 §7 |
| **Organisationen** | Hunter-Netze, staatliche Sondergruppen, private Sicherheitsdienste | [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) |
| **Bevölkerung** | Angst, Misstrauen, Flucht, Proteste, veränderter Alltag | Abschnitt 5 |

**Wirtschaft.** Ereignisse beeinflussen Tourismus, Immobilien, Geschäfte, Löhne (N-WE-017). Das ist Tiefe der Weltsimulation; die Quellen sagen ausdrücklich, Monsterfälle sollten innerhalb derselben Simulation existieren und nicht in einer getrennten Questblase (`23#1066A` Punkt 30, Vorschlag des Assistenten).

**Vertuschung.** Ein Teil der Akteure verbirgt Wahrheit (Behörden, Unternehmen, einflussreiche Wesen). Der Spieler kann Vertuschung bemerken (Widersprüche, verschwundene Beweise: `48#146A`); sie ist ein Fall für die Ermittlung (Ermittlung 17).

---

## 10. Entwurf: Reparatur und Wiederaufbau

Die Quellen verlangen, dass die Welt nach Ereignissen nicht sofort perfekt wird; NPC-Teams (Bauarbeiter, Utility Crews, Feuerwehr, Straßenarbeiter, Techniker) reparieren; der Spieler kann helfen; Schäden bleiben Tage, Wochen, Monate sichtbar (N-WE-009, N-LE-025). Für NPCs bedeutet das (💡):

| Aspekt | Entwurf |
|---|---|
| **Arbeitskräfte** | Einsatzkräfte und Handwerker sind NPCs mit Betrieben, Zeitplänen und Anfahrten |
| **Priorisierung** | Gefahr vor Verkehr vor Komfort; Reparaturfolge nach Bedeutung (Ermittlung 26 §5) |
| **Dauer** | abhängig von Schaden, Material, Personal, Wetter; Zahlen offen |
| **Betroffene** | Evakuierte, Obdachlose, Geschäftsbesitzer; Versicherung (P3) |
| **Hilfe des Spielers** | freiwillige Mithilfe; Dankbarkeit der Beteiligten (Modul 09) |
| **Erinnerung** | Gedenken, Straßenfeste, Gedenktafeln (Ortsgedächtnis) |
| **Dauerhafte Spuren** | Narben im Stadtbild; geschlossene Betriebe, Umzüge |

---

## 11. Sonderfälle

1. **Ereignis während eines Falls.** Reaktion der Zeugen hat Auswirkung auf Beweise (Spuren, Aussagen); Ermittlung 26 §9.
2. **Ereignis im Ausland des Spielers (fern).** NPCs reagieren; der Spieler erfährt über Medien (Modul 07).
3. **Ereignis mit vielen Todesopfern.** Verdichtung von Hintergrunddaten (Modul 22); wichtige NPCs sind geschützt oder betroffen nach Regeln (Modul 03 §8).
4. **Kollektive Hysterie.** Gerüchte lösen Panik ohne reales Ereignis (Modul 07 §12).
5. **Falscher Alarm.** Viele Reaktionen auf ein nicht stattgefundenes Ereignis (Fehlmeldung, Fake-Video).
6. **Heldentat des Spielers vor Publikum.** Medienbekanntheit ist selten („ja, kann passieren aber nicht oft“, `23#1057E` Punkt 45); sie wirkt auf Kreise.
7. **Täuschung.** Täter nutzen Katastrophen zur Tarnung.
8. **Hunter bei Ereignissen.** Hunter kommen nach Gerüchten; sie bemerken Anzeichen früher.
9. **Zusammenbruch von Kommunikation.** Funkloch, Blackout: Gerüchte laufen langsamer, Panik anders.
10. **Ereignis zu Feiertagen.** Menschenmenge, Familienbesuche, Verkehr; Halloween: Kostüme verwechselt (N-WE-016).
11. **Katastrophen im Untergrund.** Offen (Q-N25).
12. **Behörden leugnen.** NPCs glauben offiziellen Erklärungen unterschiedlich (Modul 06 §4).

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-286 | Ein NPC reagiert nur auf Ereignisse, die er wahrgenommen oder erfahren hat | N-GE-008 |
| N-EN-287 | Dasselbe Ereignis kann verschiedene Reaktionen auslösen, jede ist reproduzierbar | N-WE-004 |
| N-EN-288 | Die Rolle eines NPC (Beruf, Verantwortung) wirkt auf die Reaktion nachvollziehbar | Abschnitt 2 |
| N-EN-289 | Meidung entsteht aus Ereignissen und klingt ohne neue Ereignisse ab | N-WE-001 |
| N-EN-290 | Eine Gemeinschaftsvariable ändert sich nur durch Ereignisse und Gerüchte | Abschnitt 5 |
| N-EN-291 | Die Polizei beginnt mit der gewöhnlichen Erklärung | N-WE-003 |
| N-EN-292 | Eine dauerhafte Veränderung eines NPCs hat ein auslösendes Erlebnis | N-WE-005 |
| N-EN-293 | Große übernatürliche Ereignisse sind selten und zeitlich begrenzt | N-WE-007 |
| N-EN-294 | Ein Ereignis ohne Beobachter erzeugt keine Reaktion bei NPCs | N-GE-008 |
| N-EN-295 | Reparaturen werden von NPC-Teams ausgeführt und brauchen Weltzeit | N-WE-009 |
| N-EN-296 | Ein Schaden bleibt bis zur Reparatur sichtbar | N-WE-009 |
| N-EN-297 | Das Weltbild eines NPC bestimmt die Deutung, nicht die Wahrheit | N-WE-004 |
| N-EN-298 | Menschenmengen entstehen aus Ortszustand und Plänen, nicht aus Zufall | N-WE-011 |
| N-EN-299 | Ein Monsterfall verändert mindestens einen Alltagsparameter einer Gemeinschaft, wenn er öffentlich wahrnehmbar war | N-WE-008, N-WE-017 |

---

## 13. Was fehlt

- Keine Schwellen und Dauer für Meidung, Angstklima, Wegzug.
- Offen: Welche Gemeinschaftsvariablen es gibt (Q-N24).
- Offen: Wie Katastrophen im Untergrund behandelt werden (Q-N25).
- Keine Aussage der Quellen, ob Todesfälle in Katastrophen für wichtige NPCs ausgeschlossen sind (siehe C-N1).
- Kein Entwurf für die mechanische Wirkung von Gläubigkeit (Rituale, Segen) im Spiel.
