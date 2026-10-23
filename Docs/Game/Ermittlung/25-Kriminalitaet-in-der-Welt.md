# Ermittlung 25: Kriminalität in der Welt

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die **Kriminalität, die unabhängig vom Spieler geschieht**: Taten von NPCs, Organisationen, Gebiete, Märkte und ihre Wirkung auf Fälle, Polizei und Spieler. Beschlossen ist, dass es sie gibt, wie sie ungefähr aussieht und wie der Spieler hineingeraten kann. Wie oft, wo und nach welchen Zahlen, ist **offen**. Abschnitte mit „Entwurf“ sind **Vorschläge** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Ergänzt [Modul 06 §2–3](06-Kriminalitaet-Polizei-und-Justiz.md) und [Modul 14](14-Polizei-Behoerden-und-Fahndung.md). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Verbrechensfamilien](#2-entwurf-verbrechensfamilien)
3. [Entwurf: Wie Verbrechen entstehen](#3-entwurf-wie-verbrechen-entstehen)
4. [Entwurf: Ein Verbrechen als Weltereignis](#4-entwurf-ein-verbrechen-als-weltereignis)
5. [Entwurf: Organisationen und Gebiete](#5-entwurf-organisationen-und-gebiete)
6. [Entwurf: Illegale Märkte und Geschäfte](#6-entwurf-illegale-märkte-und-geschäfte)
7. [Entwurf: Wirkung auf Fälle und Tarnung des Übernatürlichen](#7-entwurf-wirkung-auf-fälle-und-tarnung-des-übernatürlichen)
8. [Entwurf: Der Spieler und die Kriminalität](#8-entwurf-der-spieler-und-die-kriminalität)
9. [Entwurf: Langzeitentwicklung](#9-entwurf-langzeitentwicklung)
10. [Sonderfälle](#10-sonderfälle)
11. [Abhängigkeiten](#11-abhängigkeiten)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

Fragenummern beziehen sich auf `23#1056A` (Block 8), Antworten auf `23#1057E`, sofern nicht anders genannt.

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| K1 | Es gibt echte Straftaten unabhängig vom Spieler: Diebstahl, Einbruch, Raub, Autodiebstahl, Vandalismus, Körperverletzung, Drogenhandel, Gangkonflikte, organisierte Kriminalität | 📋 | Frage 46, „ja schon, sehr komplex“ |
| K2 | Kriminalität unterscheidet sich nach Vierteln in Art und Häufigkeit; keine simplen „roten Crime Zones“; die Begründung ist offen | 📋 / ⚠️ | Frage 47; [Modul 21](21-Konflikte-und-offene-Fragen.md) K-E3 |
| K3 | Der Spieler kann Zeuge spontaner Verbrechen werden | 📋 | Frage 48 |
| K4 | Die Polizei reagiert auch ohne den Spieler (NPC ruft an, Beamte kommen, Täter flieht oder wird festgenommen) | 📋 | Frage 49 |
| K5 | Orte können nach NPC-Verbrechen zeitweise abgesperrt sein | 📋 | Frage 50 |
| K6 | Kriminalität verändert sich langfristig mit Stadtentwicklung, Wirtschaft und Polizeipräsenz | 📋 | Frage 51 |
| K7 | Mehrere eigene kriminelle Organisationen statt generischer „Gang NPCs“; extremistische Gruppen sind darunter und werden **nicht verherrlicht** | 📋 / 💡 | Frage 52; `23#1058A` |
| K8 | Territorien nicht standardmäßig bunt auf der Karte; nur wer Mitglied ist, sieht und erobert Einflussbereiche | 📋 | Frage 53 |
| K9 | Illegale Geschäfte: Chop Shops, Untergrund-Casinos, illegale Rennen, Fight Clubs, Schmuggel | 📋 | Frage 54 |
| K10 | Der Spieler kann normale Straftaten begehen; das Spiel wird kein GTA-Klon; Hunter bleibt der Kern | 📋 | Fragen 55–56 |
| K11 | Die Kriminalität eines Viertels soll über Faktoren wie Armut, Arbeitslosigkeit, Drogenmärkte, Infrastruktur, Polizeipräsenz, Stadtentwicklung, soziale Stabilität modelliert werden, nicht über die Herkunft der Bewohner | 💡 | `23#1058A` |
| K12 | Zeugen können Schweigegeld erhalten, bedroht oder beseitigt werden | 📋 | Frage 48 |
| K13 | Bikergruppen, Clubs, Sekten, Extremisten haben Hierarchien; Mitgliedschaft optional; unterschiedliche Wege (Mitglied, Spitzel, lose Verbindung) | 📋 / 💡 | `23#675E`, `23#677E`; Ausformulierung `23#676A`, `23#678A` |
| K14 | Normale Motorradclubs sind nicht automatisch kriminell (legale, Outlaw-, extremistische, Veteranen-Clubs sind verschiedene Dinge) | 💡 | `23#676A` |
| K15 | Der Spieler beurteilt Gruppen selbst; Personen können „eigentlich gute Leute“ sein | 📋 | `23#675E` |
| K16 | Der Anteil der Kriminalität an der Welt ist nicht festgelegt | ❓ | `M-OD` |

---

## 2. Entwurf: Verbrechensfamilien

> [!NOTE]
> **Nur Vorschlag (💡).** Die Quellen nennen Delikte als Aufzählung (K1, K9). Ein Katalog nach **Muster**, damit ein Fallsystem sie unterscheiden kann. Kein Straftatenkatalog im Sinne von `M-OD`.

| Familie (Arbeitsname) | Delikte aus den Quellen | Typisches Muster | Zeugen/Spuren (Tendenz) |
|---|---|---|---|
| **Eigentumsdelikte** | Diebstahl, Einbruch, Raub, Autodiebstahl, Vandalismus | meist nachts, kurze Dauer, Beute | Kameras, Fingerabdrücke, Fahrzeug, Aufbruchsspuren |
| **Gewaltdelikte** | Körperverletzung, Gangkonflikte | Konflikt, Alkohol, Gruppen | Zeugen, Verletzungen, Waffen |
| **Drogen** | Drogenhandel, Abhängigkeit | feste Orte, Händlernetze | Informanten, Observation |
| **Organisierte Kriminalität** | Schutzgeld, Schmuggel, Chop Shops, illegale Casinos | wiederholte Strukturen, Hierarchie | Informanten, Finanzspuren, Beobachtung |
| **Illegale Veranstaltungen** | illegale Rennen, Fight Clubs, Glücksspiel | Termine, Treffpunkte | Gerüchte, Medien, Polizei |
| **Verkehrsdelikte** | Unfallflucht, Trunkenheit, Raserei | Fahrzeuge | Kennzeichen, Kameras |
| **Digitale Delikte** | Betrug, Identitätsdiebstahl, Cybercrime | online | digitale Spuren |
| **Gewalt gegen Personen** | schwere Delikte | – | Konsequenzsystem; Details in [Adult 09](../Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md) |
| **Übernatürlich getarnt** | Taten, die Wesen oder Kulte als gewöhnliche Kriminalität erscheinen lassen | Tarnung | Anomalien (Modul 11), Muster (Modul 05) |

Jede Familie bestimmt, welche **Belegarten** ein Fall hat (Modul 10) und welche **Zuständigkeit** greift (Modul 14).

---

## 3. Entwurf: Wie Verbrechen entstehen

Kriminalität entsteht aus Welt-Zuständen (K6, K11), nicht aus Zufallswürfen am Spieler.

| Faktor (aus `23#1058A`) | Wirkung (Entwurf) |
|---|---|
| Armut, Arbeitslosigkeit | erhöht Eigentumsdelikte und Drogenmärkte |
| Organisierte Kriminalität, Drogenmärkte | erzeugen Gewalt- und Schutzgelddelikte |
| Infrastruktur | Beleuchtung, Zugänglichkeit, Verkehr |
| Polizeipräsenz | senkt Gelegenheit, verlagert Taten |
| Stadtentwicklung | verändert Gebiete über Jahre |
| Soziale Stabilität | beeinflusst Konflikte |
| Zeit und Wetter | Nacht, Feiertage, Großveranstaltungen (Modul 26) |

**Persönliche Ursachen (Entwurf):** NPC-Motive aus Tagesabläufen und Beziehungen (Geld, Konflikt, Rache, Sucht, Angst); wichtige NPCs sind gegen triviale Simulationsunfälle geschützt (Punkt 105). Die Taten **wichtiger** NPCs sind eher handgeschrieben oder Fallbestandteile (Modul 09); Hintergrundkriminalität ist abstrahiert (Tier C und D, Punkte 121–125).

**Erzeugung (Entwurf):** Ein Verbrechensgenerator ist **eine Spezialisierung** der Fall- und Ereignisgenerierung (Modul 09 §8): Auslöser aus Faktoren, Muster aus der Familie, Instanziierung mit Welt-NPCs und -Orten, Meldekette (Modul 14 §4). Die Rate ist offen (Q-E74).

**Regionale Unterschiede (Entwurf):** Art und Häufigkeit unterscheiden sich nach Gebiet (K2), aber das Modell nutzt die Faktoren, nicht Zuschreibungen. Die Frage, welche Ursachen der Entwickler wünscht, ist offen (K-E3).

---

## 4. Entwurf: Ein Verbrechen als Weltereignis

Ein NPC-Verbrechen ist ein **Weltereignis** wie jedes andere. Der Zustandsfluss nutzt die Bausteine aus den Modulen 12 bis 15:

```text
Absicht (NPC) -> Tat -> Wahrnehmung (Zeugen, Kameras, Spieler) -> Meldung -> Einsatz
   -> Flucht/Festnahme -> Ermittlung -> Verfahren -> Folgen (Absperrung, Medien, Gerüchte)
```

| Phase | Wer reagiert | Beispiele aus den Quellen |
|---|---|---|
| Tat | Täter | – |
| Wahrnehmung | Zeugen, Spieler | Spieler kann Zeuge werden (K3) |
| Meldung | NPC ruft Polizei | Frage 49; Notruf (Modul 14) |
| Einsatz | Polizei, Rettung | Polizei, Feuerwehr, EMS, Abschleppdienst (`23#1058A`) |
| Absperrung | Behörde | Orte zeitweise gesperrt (K5) |
| Ermittlung | Behörde | Detectives bei schweren Fällen (Modul 14) |
| Folgen | Welt | Gerüchte, Nachrichten, Ängste, Vorsicht, Meiden von Orten (Frage 114) |

Die **Wahrscheinlichkeit** jedes Schritts ist offen; jeder kann scheitern (Modul 14 §4). Der Spieler kann in jede Phase eingreifen (Abschnitt 8).

### 4.1 Reaktionen von NPCs auf Verbrechen vor ihren Augen

Aus `35#533A` (Richtung vom Entwickler vorgegeben `35#532E`): NPCs, die ein schweres Verbrechen beobachten, können die Polizei rufen, eingreifen, festhalten, fliehen, angreifen oder (bei besonders gewalttätigen Gruppen) stark eskalieren, **je nach Persönlichkeit, Fraktion und Moral**, nicht nach einer festen Regel. Siehe [Modul 06 §8](06-Kriminalitaet-Polizei-und-Justiz.md#8-selbstverteidigung-und-npc-reaktionen).

---

## 5. Entwurf: Organisationen und Gebiete

### 5.1 Beschlossene Richtung

Mehrere eigene Organisationen (K7). Gruppen haben Einflussbereiche, aber keine standardmäßig farbigen Gebiete auf der Karte (K8). Hierarchien sind echt (K13). Hierarchische Aufstiege hängen von Vertrauen, Jobs, Loyalität, Beziehungen, Abstimmungen, Fähigkeiten, Verhalten und Entscheidungen ab (Ausformulierung `23#676A`, 💡: „Rang ≠ nur Level“).

### 5.2 Entwurf: Organisationsmodell

| Element | Inhalt (Katalog) |
|---|---|
| **Art** | legal (Club), Outlaw (illegale Geschäfte), extremistisch (Ideologie), Veteran, Sekte, Netzwerk |
| **Struktur** | Rangleiter (je Gruppe verschieden), Anführer, Mitglieder mit Beziehungen untereinander (`23#676A`) |
| **Gebiet** | Einflussbereich (Viertel, Straßen, Clubhaus), nicht als bunter Layer, außer der Spieler ist Mitglied |
| **Geschäfte** | Einnahmen (legal/illegal), Treffpunkte, Fahrzeuge |
| **Konflikte** | Rivalitäten, Bündnisse; Territorien verschieben sich (Entwurf) |
| **Wissen** | was die Gruppe über den Spieler und die Polizei weiß (Wissensmarken) |
| **Verhältnis zum Übernatürlichen** | ahnungslos, ahnend, wissend, benutzend (Wissensstufen, Modul 14 §11) |
| **Bezug zur Polizei** | infiltriert, korrumpiert, gejagt (Modul 14) |

### 5.3 Zugehörigkeit des Spielers

Wege (aus `23#678A`): ehrliche Mitgliedschaft, Infiltration, lose Verbindung. Siehe [Modul 13 §9](13-Identitaet-Ruf-und-Formwandler.md#9-zugehörigkeiten-infiltration-und-spitzel). Die Hauptgeschichte ist unabhängig (`23#677E`). Nach dem Ende der Hauptgeschichte läuft die Gruppenhandlung weiter; „unlimitiert extra content“ ist erwünscht (`23#679E`).

### 5.4 Darstellung

Die Quellen verlangen, dass extremistische Gruppen als problematische, feindselige Teile der Welt gezeigt werden, **nicht glorifiziert** (`23#674A`, `23#676A`), und dass ein realistischer Anwerbe- oder Radikalisierungssimulator nicht gebaut wird (`23#678A`). Das Spiel zeigt Personen, Konflikte und Konsequenzen. Die Bewertung bleibt beim Spieler (`23#675E`).

---

## 6. Entwurf: Illegale Märkte und Geschäfte

Beschlossen (K9): Chop Shops, Untergrund-Casinos, illegale Rennen, Fight Clubs, Schmuggel. Weitere Verknüpfungen aus den Quellen: Pfand-, Second-Hand- und Hehlerläden (`M-PD`), Drogenhandel und Milieus (`23#1058A`), Motorsport und Fight Clubs (Welt 15).

| Markt | Beziehung zur Ermittlung (Entwurf) |
|---|---|
| **Hehlerei** | Gestohlene Ware taucht bei Hehlern auf; Spuren des Besitzes (Fahrzeugteile, Gegenstände); Hunter-relevante Gegenstände „gelegentlich“ (`M-PD`) |
| **Chop Shops** | Fahrzeuge verschwinden; Kennzeichen, Fahrzeugteile (Reifen, Seriennummern) |
| **Untergrund-Casinos, Fight Clubs, illegale Rennen** | Treffpunkte, Gerüchte, Medien; Zeugen, Informanten; Polizei-Razzien |
| **Schmuggel** | Verkehrsrouten (Welt 09), Kontrollen |
| **Drogen** | Milieus, Abhängigkeit, Händlernetze |
| **Waffenmarkt** | nicht in den Quellen (Waffenhandel/-recht offen) |

Die **Preise**, die **Mengen** und die **Zahl der Orte** sind offen.

---

## 7. Entwurf: Wirkung auf Fälle und Tarnung des Übernatürlichen

Gewöhnliche Kriminalität ist für die Ermittlung **Hintergrundrauschen und Tarnung** zugleich:

| Wirkung | Erläuterung |
|---|---|
| **Falsche Fährten** | Ein Verbrechen im selben Umfeld erzeugt Spuren, die den Fall verwirren (Modul 10 §7) |
| **Tarnung** | Wesen oder Kulte nutzen gewöhnliche Kriminalität als Deckung (Fall G, `23#55A`); die Polizei erklärt zunächst normal (Punkt 116) |
| **Kandidatenpool** | gewöhnliche Verdächtige (Gangmitglieder, Drogenhändler) lenken Verdacht (Modul 12 §7) |
| **Signale** | Vermisstenanzeigen, Polizeifunk, Gerüchte können gewöhnlich oder übernatürlich sein (Modul 09 §7); das Spiel muss beide Möglichkeiten glaubhaft halten (mehrdeutige Fälle, `23#1074A` Frage 71) |
| **Informanten** | Gruppen kennen Menschen, die verschwunden sind, und Wege (Modul 14 §9) |
| **Schutzgeld und Einfluss** | Organisationen können Zeugen beeinflussen (Modul 12 §11) |
| **Interessenkonflikt** | Gruppen können Hunter helfen, wenn ein Wesen ihre Geschäfte stört (Entwurf) |

---

## 8. Entwurf: Der Spieler und die Kriminalität

| Handlung | Entwurf (Quellen in Klammern) |
|---|---|
| Zeuge werden | melden, schweigen, eingreifen, ignorieren (Frage 48); Folgen: Bedrohung, Bestechung (Frage 48) |
| Eingreifen | Gewalt, Festhalten, Verfolgen; Notwehr/Nothilfe wird bewertet (`35#533A`); rechtliche Folgen |
| Melden | Notruf, Anzeige; Polizei reagiert (Modul 14) |
| Selbst Straftaten begehen | möglich (K10); Zeugen, Spuren, Fahndung (Module 11 bis 14); Hunter bleibt Kern |
| In eine Gruppe eintreten | Abschnitt 5.3 |
| Gegen eine Gruppe vorgehen | Konflikte, Rache (NPC-Gedächtnis) |
| Informant der Polizei werden | offen (Q-E53) |
| Illegale Märkte nutzen | Hehler, Chop Shops, Rennen (Welt 15) |
| Bestechung anbieten oder annehmen | Folgen (Modul 14 §10) |

Wichtig: Es gibt **kein** Pflichtverbrechen. Die Hauptgeschichte hängt nicht davon ab (K10 und `23#677E`).

---

## 9. Entwurf: Langzeitentwicklung

Beschlossen: Kriminalität verändert sich mit Stadtentwicklung, Wirtschaft und Polizeipräsenz (K6). Stadtteile verändern sich über Jahre (Block 8M Frage 111). Ein Entwurf der Entwicklung:

| Treiber | Wirkung über Jahre |
|---|---|
| Wirtschaftsaufschwung | weniger Eigentumsdelikte, mehr Wirtschaftsdelikte |
| Verfall | mehr Armutskriminalität |
| Polizeischwerpunkte | Verlagerung |
| Machtverschiebung der Gruppen | neue Konflikte |
| Ereignisse (Katastrophen, Skandale) | Sprünge |
| Spielerhandlungen | Zerschlagung, Stärkung, Verlagerung |

Weil die Welt Jahre und Jahrzehnte laufen kann (Block 8A–8M), muss die Entwicklung in **Fortschreibungsschritten** möglich sein (Modul 19 §8). Generationen sind laut Entwickler **später** (Frage 9: „soll sie, aber das soll erst spät gemacht werden“).

---

## 10. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Spieler stört Geschäfte einer Gruppe unwissentlich | – | Gruppe reagiert mit Verdacht; Polizeifall |
| Wichtiger NPC wird Verbrechensopfer | – | geschützte NPCs (Punkt 105); Folgen für Fälle |
| Zeuge ist selbst Kriminelle/r | – | Aussage mit Interesse (Modul 10 §6) |
| Gang hilft dem Spieler | – | Beziehung, Preis |
| Verbrechen am Ort eines Falls | – | Spuren überlagern sich |
| Mehrere Behörden | – | Zuständigkeit (Modul 14) |
| Koop | Mitspieler in verschiedenen Gruppen | Beziehungen pro Charakter; Konflikt im Team (Modul 18) |
| Mods | neue Organisationen | Daten, keine Sonderlogik |
| Kinder als Opfer oder Täter | – | nicht in den Quellen; Darstellung: Adult 09, Gameplay-Systeme §12 |

---

## 11. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Polizei ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) | Meldekette, Akten |
| Gerichte und Haft ([Module 15](15-Gerichte-und-Verfahren.md), [16](16-Haft-und-Strafvollzug.md)) | Verfahren gegen NPCs |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Wahrnehmung, Bedrohung |
| Identität ([Modul 13](13-Identitaet-Ruf-und-Formwandler.md)) | Mitgliedschaft, Spitzel |
| Fallmodell ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) | Signale, falsche Fährten |
| Notfälle und Ereignisse ([Modul 26](26-Notfaelle-und-Weltereignisse.md)) | Absperrung, Rettung |
| Wirtschaft | Märkte, Einkommen |
| Motorsport und Verkehr (Welt 15, 16) | illegale Rennen, Verfolgung |
| NPC-Simulation (Tier A bis D) | Täter und Opfer |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Organisationen, Gebiete als Zustand |

---

## 12. Was fehlt

- **Straftatenkatalog**, Raten, Orte, Häufigkeiten (`M-OD`).
- **Ursachenmodell** der Viertelunterschiede (K-E3).
- **Generatorentwurf** und Rate (Q-E74).
- **Namen und Art der Organisationen** (Entwickler: „das haben wir schon aufgestellt“, `23#1057E` Punkt 52; in den Dateien der Tier 1 und 2 des Masters nicht auffindbar; Tier 3 wurde dafür nicht durchsucht).
- **Waffenmarkt** und Waffenrecht.
- **Gebietssystem** der Gruppen (Einflusskarte nur für Mitglieder).
