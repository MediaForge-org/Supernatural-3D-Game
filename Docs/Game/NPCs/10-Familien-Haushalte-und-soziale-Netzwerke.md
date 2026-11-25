# NPCs 10: Familien, Haushalte und soziale Netzwerke

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie NPCs in Haushalten, Familien und sozialen Netzwerken leben und wie dieses Geflecht Information, Konflikte und Folgen trägt. Beschlossen sind: Familien, Freunde, Kollegen, Nachbarn, Haushaltswandel, Heirat, Zusammenziehen; Generationen und Familiengründung sind **spät** beziehungsweise **offen**. **Haushalts- und Netzmodell sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Der Haushalt](#2-entwurf-der-haushalt)
3. [Entwurf: Verwandtschaft](#3-entwurf-verwandtschaft)
4. [Entwurf: Soziale Kreise](#4-entwurf-soziale-kreise)
5. [Entwurf: Gemeinschaften und Orte](#5-entwurf-gemeinschaften-und-orte)
6. [Entwurf: Familienereignisse](#6-entwurf-familienereignisse)
7. [Entwurf: Generationen vorbereiten](#7-entwurf-generationen-vorbereiten)
8. [Entwurf: Familie und Spielercharakter](#8-entwurf-familie-und-spielercharakter)
9. [Entwurf: Familie als Faktor in Konflikten](#9-entwurf-familie-als-faktor-in-konflikten)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-FA-001 | NPCs können Partner, Kinder, Eltern, Geschwister haben | 📋 | `23#1056A` Frage 5; „ja“ in `23#1057E` |
| N-FA-002 | Zusätzlich zu Familien gibt es Freunde, Kollegen, Nachbarn und Bekannte | 📋 | `23#1056A` Frage 6; „ja“ in `23#1057E` |
| N-FA-003 | Beziehungen, Haushalte, Besitzer und Mieter von Häusern, Betreiber von Geschäften, Mitarbeiter und Nachbarschaften verändern sich | 📋 | `23#1056A` Fragen 106–111; „ja“ in `23#1057E`; `23#1058A` ‚Sozialer World State‘ |
| N-FA-004 | NPCs altern sichtbar, bekommen Kinder, ziehen aus und sterben irgendwann | 📋 | `23#1056A` Frage 8; „ja genau“ in `23#1057E` |
| N-FA-005 | Die Welt soll theoretisch über Jahrzehnte Nachwuchs hervorbringen, sodass neue Erwachsene nachkommen; das soll **erst spät** umgesetzt werden, weil es technisch sehr aufwendig ist | 📋 | `23#1056A` Frage 9; „ja soll sie, aber das soll erst spät gemacht werden“ in `23#1057E`; `23#1058A` |
| N-FA-006 | Umzüge sind möglich bei Jobwechsel, Einkommen, Beziehung oder Stadtentwicklung, aber nicht zu oft: „können sie, aber nicht zu oft“ | 📋 | `23#1056A` Frage 10; Antwort in `23#1057E` |
| N-FA-007 | Einkommen und Wohlstand beeinflussen Wohnort, Fahrzeug und Tätigkeit zumindest abstrakt | 📋 | `23#1056A` Frage 12; „ja“ in `23#1057E` |
| N-FA-008 | Ein NPC kann heiraten oder in einer Langzeitpartnerschaft leben, Kinder oder Familie haben (unabhängig vom Adult-System); der Familienstatus kann Entscheidungen und verfügbare Zeit beeinflussen | 📋 | `35#272A` Fragen 17–19; „1-50 passt alles“ in `35#273E` |
| N-FA-009 | Ein NPC kann eine Beziehung geheim halten und mehrere parallele Beziehungen führen, wenn seine Regeln es zulassen | 📋 | `35#272A` Fragen 20–21; „1-50 passt alles“ in `35#273E` |
| N-FA-010 | Familiengründung des **Spielers** ist grundsätzlich möglich, aber offen und niedrig priorisiert: „weiß nicht, als hunter eher schwierig“ | ❓ | `23#1088A` Frage 10; Antwort in `23#1089E`; `23#1090A` ‚Beziehungen & Sexualität‘ |
| N-FA-011 | Der Spieler kann mit einem Partner zusammenziehen, heiraten und gemeinsame Immobilien besitzen | 📋 | `23#1088A` Fragen 7–9; „6-9 ja“ in `23#1089E` |
| N-FA-012 | Familienbesuche und Feiern an Feiertagen verändern Routinen von NPC-Familien | 📋 | `23#1088A` Frage 162; „159-166 ja“ in `23#1089E` |
| N-FA-013 | Das Spiel weiß auch nach Jahrzehnten, wer früher mit wem verbunden war; historische Beziehungen sind abfragbar für Dialoge, Fälle, Familiengeschichten und Weltlore | 📋 | `35#272A` Fragen 38–39; „1-50 passt alles“ in `35#273E` |
| N-FA-014 | Der Tod eines NPCs löst Reaktionen aus: Ersatz in Geschäft oder Eigentum, Reaktion der Familie; Erinnerung anderer NPCs; bedeutende Figuren möglicherweise mit Beerdigung | 📋 | `23#1056A` Fragen 103–104, 112; Antworten in `23#1057E` |
| N-FA-015 | Familien- und Wohnformen sind vielfältig; Haushalte bestehen aus Familien, Freunden, Alleinlebenden usw. | 📋 | [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13 (Oberflächenwelt: Familien, Freunde, Kollegen); §14.2 (Untergrund: ‚Natürliche Vielfalt aller Familien- und Wohnformen‘) |
| N-FA-016 | NPCs besitzen Wohnungen, Fahrzeuge, Haustiere und Gegenstände persistent | 📋 | `23#1056A` Frage 22; „ja“ in `23#1057E` |
| N-FA-017 | Ordnung in Gesellschaft: Mitgliedschaften in Clubs, Sekten, Motorradclubs, Gangs, Racing Clubs, Jagdvereinen, Fight-Szenen usw. sind eigene soziale Gruppen mit Mitgliedschaftsregeln | 🧭 | `23#676A`–`23#680A` |
| N-FA-018 | Touristen sind ein eigener Populationstyp mit saisonalen Spitzen | 📋 | `23#1088A` Fragen 167–168; „167-173 ja“ in `23#1089E` |
| N-FA-019 | Kinder dürfen NPCs sein, sind aber vom Adult-System ausgeschlossen; die Darstellung von Gewalt gegen Minderjährige ist eine offene Rechts- und Politikfrage | 📋 / ❓ | [Adult 01](../Adult/01-Grundregeln-Alter-Zustimmung.md); `M-OD` |

---

## 2. Entwurf: Der Haushalt

Die Quellen führen Wohnung, Familie, Einkommen, Fahrzeug und Besitz je NPC. Ein **Haushalt** bündelt davon, was mehrere Personen teilen (💡):

```text
Household
├── id, home_id (Wohnung/Haus), tenure (Miete/Eigentum/obdachlos)
├── members[]          NPC-IDs mit Rolle (Haupteinkommen, Partner, Kind, Mitbewohner ...)
├── income_class       abstrakt (N-FA-007)
├── vehicles[]         geteilte Fahrzeuge
├── pets[]
├── routines           gemeinsame Zeiten (Essen, Kirche, Schule, Fahrten)
├── economy            Schulden, Rücklagen (abstrakt)
├── status             stabil | angespannt | in Krise | aufgelöst
└── history            Haushaltsereignisse (Einzug, Auszug, Geburt, Tod, Trennung)
```

**Funktionen (Entwurf):**

1. **Ein Haushalt ist die Einheit des Alltags.** Zeitpläne (Modul 13) greifen auf ihn zu: Wer fährt wen, wer ist wann zuhause.
2. **Persistenz.** Der Haushalt ist der Träger persistenter Dinge (Haus, Auto, Haustier), nicht jeder einzelne NPC.
3. **Verdichtung.** Hintergrundhaushalte werden aus Seed und Haushaltsrezept erzeugt; Ereignisse (Tod, Umzug) erzeugen Abweichungen (N-ID-020).
4. **Obdachlose.** Haushalte ohne Wohnung; der Ort ist ein Stammplatz; sie haben Beziehungen und Erinnerung (Modul 03).
5. **Eigentum und Miete.** Hauswechsel an neue Bewohner (N-FA-003); ein Haus hat Geschichte (Bewohnerliste), die für Ermittlung und übernatürliche Orte zählt ([Kreaturen 16](../Kreaturen/16-Uebernatuerliche-Orte-und-Geografie.md)).
6. **Wohlstandsklasse.** Bestimmt Wohnen, Fahrzeug, Freizeit, Kriminalitätsrisiko (N-LT-031 verbietet Ethnie als Ursache), nicht den Wert eines Menschen.

**Haushaltsrollen (Entwurf):** Haupteinkommen, Zweitverdiener, Versorgter, Kind, Hausgast, Pflegebedürftiger, Mitbewohner.

---

## 3. Entwurf: Verwandtschaft

Verwandtschaft ist die dauerhafteste Beziehungsform. Die Quellen nennen Eltern, Kinder, Geschwister, Partner (N-FA-001); der Entwurf legt Strukturen fest, ohne eine Genealogie zu fordern (💡).

| Struktur | Inhalt | Bedeutung |
|---|---|---|
| **Eltern/Kind-Kanten** | gerichtete Kanten (Modul 08) mit Typ *Verwandtschaft* | Erbe, Fürsorge, Konflikt, Erinnerung |
| **Geschwister** | ergeben sich aus gemeinsamen Eltern | Rivalität, Loyalität |
| **Partner** | Typ *Partner* | Haushalt, Besitz, Exklusivität oder Vereinbarung |
| **Erweiterte Familie** | Großeltern, Onkel, Tante, Cousinen | Feiertage, Beerdigungen, Konflikte |
| **Patchwork, Adoption** | andere Kanten desselben Typs | Vielfalt der Familienformen (N-FA-015) |
| **Familienname** | gemeinsame Kennung | Vererbung des Rufs (Familie XY) |

**Genealogie nur in Rudimenten.** Das Spiel braucht nicht bei jedem NPC eine vollständige Ahnenreihe. Es genügt, dass **die für den Spieler und für Fälle relevanten** Verwandtschaften existieren und dass ein später zugeschaltetes Generationensystem (Abschnitt 7) daran anknüpfen kann.

**Familiengeschichte als Ermittlungsquelle.** Kirchenregister, Familienbibeln, Fotos und Gerichtsakten (`23#51A` Abschnitt 20) nutzen Familiendaten. Die NPC-Familien sollen diese Quellen plausibel füllen können.

---

## 4. Entwurf: Soziale Kreise

Kreise ordnen, wer wen kennt und über wen Information läuft (Modul 07). Die Quellen nennen Kategorien (Freunde, Kollegen, Nachbarn, Bekannte, N-FA-002) und Zugehörigkeiten (N-FA-017).

| Kreis | Eigenschaften | Pflege (Entwurf) |
|---|---|---|
| **Familie/Haushalt** | enge Kanten, tägliche Interaktion | automatisch durch Alltag |
| **Freunde** | gewählt, Zuneigung/Vertrauen | gemeinsame Zeit; Funkstille lockert |
| **Kollegen** | durch Arbeit | Arbeitszeiten; Wegfall bei Jobwechsel |
| **Nachbarn** | durch Nähe | Dauer; Wegfall bei Umzug |
| **Vereine, Szenen** | Interesse (Motorrad, Sport, Kirche, Jagd) | Termine, Rang |
| **Berufsnetze** | Branche (Polizei, Medizin, Werkstatt) | Kontakte |
| **Online-Gruppen** | Social Media | Nutzung (Modul 07) |
| **Kriminelle Netze** | Geschäft, Territorium | Fraktionsregeln (Modul 16) |
| **Hunter-Netz** | selten, verdeckt | Vertrauen ([Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md)) |

**Begrenzung (Entwurf).** Jeder NPC hat nur eine begrenzte Zahl pflegender Kanten (Dunbar-ähnlich, ohne Zahl); darüber hinaus nur schwache Kanten (Bekanntschaft). Das hält Daten und Dynamik klein (Modul 22).

**Brücken und Randlagen.** Menschen mit vielen Kreisen (Barkeeper, Pfarrer, Taxifahrer, Journalist, Friseur) tragen Information weit; isolierte Menschen kaum (Modul 07).

**Netzwerkwirkung auf den Spieler.** Wer bei einem Kreis verloren hat (Skandal, Gewalt), verliert dort Zugang; wer in einem Kreis Freund ist, hat Türen. Das ist die sichtbare Folge der Beziehungskanten (Modul 08 §7).

---

## 5. Entwurf: Gemeinschaften und Orte

Orte sind soziale Einheiten (N-GE-026, `23#1058A`: Diner nach 15 Jahren): Stammlokale, Kirchen, Werkstätten, Vereinsheime, Clubhäuser, Sportstätten.

| Merkmal | Entwurf |
|---|---|
| **Stammgäste** | Menschen mit regelmäßigem Aufenthalt (Zeitpläne, Modul 13) |
| **Besitzer, Betreiber** | NPC (Stufe B/A); Wechsel möglich (N-FA-003) |
| **Atmosphäre** | Ergebnis von Gästen, Geschichte, Ereignissen |
| **Ortsgedächtnis** | öffentliche Ereignisse des Ortes (Modul 05 §9) |
| **Hausordnung, Normen** | wer willkommen ist; Hausverbot als Folge |
| **Kalender** | Feiertage, Feste, Sportereignisse (N-FA-012; `23#1088A` Fragen 159–166) |

**Wirkung.** Wer einen Ort regelmäßig besucht, wird bekannt (Stammgast). Wer dort Gewalt verübt, wird bekannt (Gerücht); der Ort kann schließen, wechseln oder sich verändern.

**Tourismus.** Touristen ändern die Zusammensetzung eines Ortes saisonal (N-FA-018); Monstergerüchte können sie abschrecken oder anziehen (`23#1088A` Frage 171).

---

## 6. Entwurf: Familienereignisse

Ereignisse in Familien sind eine Hauptquelle von Lebensentwicklung (N-LT-006) und gehören ins Journal (Modul 14).

| Ereignis | Wirkung (Entwurf) |
|---|---|
| **Geburt** | neuer Haushaltsteil; Zeitplan, Einkommen und Prioritäten ändern sich (spät, N-FA-005) |
| **Heirat, Zusammenzug** | Haushaltsfusion; Wohnortwechsel; Name möglicherweise; Netz verbindet sich |
| **Trennung, Scheidung** | Haushaltsteilung; Konflikte; Freundeskreise teilen sich (N-BE-016) |
| **Krankheit, Pflege** | Zeitplan, Kosten; Besuche |
| **Tod** | Trauer, Erbe, Beerdigung, Haushaltsumbau (N-FA-014) |
| **Umzug** | Netz verändert sich; Fernbeziehungen |
| **Jobverlust, Erfolg** | Einkommensklasse; Wohnort; Stimmung |
| **Streit, Bruch** | Kante sinkt; Familienbesuche entfallen |
| **Besuch, Fest** | Routinenänderung an Feiertagen (N-FA-012) |
| **Verbrechen** | Opfer oder Täter in der Familie; Behörden, Gemeinschaft |
| **Verschwinden** | Vermisstenfall; Haushalt in Krise |

**Reaktionen anderer.** Familienereignisse sind Gesprächsstoff für Kreise (Modul 07) und Anlass für Hilfsangebote (Nachbarn) oder Klatsch.

---

## 7. Entwurf: Generationen vorbereiten

**Quelle:** Generationen sind gewollt und **spät** (N-FA-005); Familiengründung des Spielers ist offen (N-FA-010). Die Prioritätsliste in `23#1090A` führt „Generationen“ und „Familiengründung“ unter P3, mit dem Zusatz „P2/P3 bedeutet nicht ‚wird nie eingebaut‘“.

**Entwurf (💡) – was jetzt schon so gebaut werden sollte, dass das spätere System andocken kann, ohne es heute zu bauen:**

1. **Geburtsdatum im Datensatz** (Modul 03): Alter ist eine Funktion des Weltdatums.
2. **Eltern-Kanten optional** (Abschnitt 3): sodass Kinder später Eltern haben können.
3. **Haushaltsrollen und -ereignisse** (Abschnitt 2, 6): sodass Geburt und Erwachsenwerden im Journal vorkommen.
4. **Bevölkerungsbilanz.** Ein späteres System muss Bevölkerung stabil halten (Tod/Zuzug/Geburt). Die Planungsgröße ‚mehrere Hunderttausend‘ ist offen (Spielwelt §13); ein Modell der Altersverteilung ist offen (Q-N18).
5. **Keine Pflicht für Altersstufen unter Erwachsenenalter** außerhalb von Familie und Schule (Rechtsfrage: N-FA-019).
6. **Wiedereintritt.** Aus einem Kind, das der Spieler kannte, wird ein Erwachsener derselben ID (Modul 14).

**Nicht Teil dieses Moduls:** Die Mechanik von Schwangerschaft und Familienplanung. Der Entwickler ließ die Frage der Schwangerschaft als „weiß noch nicht“ offen (`35#281E` Punkt 14) und verwies auf ein allgemeines Lebenssimulationssystem (`35#282A`).

---

## 8. Entwurf: Familie und Spielercharakter

Der Spielercharakter ist in der Quelle ein **frei erstellter Hauptcharakter** (`M-PD` ‚Player-created protagonist, Hunter crew and HQ‘). Herkunft, Familie und Vergangenheit sind in den Quellen nicht festgelegt.

| Aspekt | Stand |
|---|---|
| **Familie des Spielercharakters** | nicht festgelegt (❓ Q-N17) |
| **Partner, Zusammenziehen, Heirat** | möglich (N-FA-011) |
| **Haustiere** | Hund als Begleiter ([Modul 19](19-Begleiter-Crew-und-Rekrutierung.md) §6) |
| **Familie wird Teil von Fällen** | Verwandte als Zeugen, Opfer, Täter, Betroffene |
| **Offenlegung des Hunter-Lebens** | Spielerentscheidung (N-BE-017) |
| **Gefährdung der Familie** | Partner und Familie können in Gefahr geraten, aber ohne ständige Entführungsplots (N-LT-032) |

**Fürsorge als Lebensthema.** Wer Familie hat, hat Verpflichtungen (Zeit, Geld, Sicherheit); das Spiel zeigt sie als Folgen (verpasste Termine, Sorge), nicht als Pflichtmenü.

---

## 9. Entwurf: Familie als Faktor in Konflikten

Familien tragen Konflikte weiter (Racheketten, Loyalitäten). Das ist die Verbindung zu Nemesis und Fraktionen (Modul 11, 17).

| Muster | Beschreibung (Entwurf) |
|---|---|
| **Rache für Angehörige** | Familie eines Getöteten verfolgt den Täter (hoher Groll, Anker) |
| **Loyalität über Gruppengrenzen** | Familienmitglieder in rivalisierenden Gruppen |
| **Erbe und Schuld** | Schulden, Besitz, Feindschaft werden weitergegeben |
| **Schutz der Familie** | NPCs fliehen, zahlen, schweigen, um Angehörige zu schützen |
| **Familie als Druckmittel** | Täter bedrohen Familien von Zeugen |
| **Familiengeheimnis** | Wissen über Herkunft, Verbrechen, Übernatürliches |
| **Familienfehde** | dauerhafte Spannung zwischen Familien, historisch begründet |

**Folgerung für die Ermittlung.** Die Familie eines Opfers oder Täters ist Zeuge, Verdächtiger, Informationsquelle (Ermittlung 12). Das Familiennetz bestimmt, wer wann von Ereignissen erfährt (Modul 07).

---

## 10. Sonderfälle

1. **Zuwanderung und Wegzug.** Haushalte kommen und gehen (Stadtentwicklung, `23#1056A` Frage 10); Einwanderer sind offen (N-ID-023).
2. **Familien ohne Kontakt.** Getrennte Verwandte kennen einander kaum; die Kante ist schwach, aber vorhanden.
3. **Familiengeheimnisse im Spiel.** Verborgene Verwandtschaft (Doppelleben, Unehelichkeit, Wesenstarnung); Aufdeckung ändert Kanten.
4. **Wesen in Familien.** Ein Wesen mit menschlicher Identität kann Teil einer Familie sein ([Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §2); die Familie weiß es oder nicht.
5. **Verstorbene Familienmitglieder** bleiben Teil von Erinnerung, Fotos, Gräbern, Erbe.
6. **Besessenes Familienmitglied.** Familie als Betroffene und Mitwisser.
7. **Hunter-Familien.** Hunter können Familie haben; Wissen wird vererbt ([Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md)); die Quellen nennen ‚Jägerfamilien‘ (`CANONICAL_ADDENDUM_2026-09-05.md`, Modul 01 F8).
8. **Haustiere.** Verstorbene Haustiere, Neuanschaffung, Tierarzt (Ereignisse).
9. **Kinder und Schule.** Tagesabläufe von Kindern (Modul 13) sind Teil der Familienroutinen; Darstellung ist rechtlich zu prüfen (N-FA-019).
10. **Haushalte in der Untergrundzivilisation** (**historisch, N-02**). Früher galten dort abweichende Gesellschaftsregeln abweichende Gesellschaftsregeln (Spielwelt §14.2: Zugehörigkeit durch Vertrauen, individuelles Lernen); dieser Ordner führt sie nicht (Q-N25).

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-161 | Jeder NPC mit Stufe A bis C gehört zu genau einem Haushalt (oder ist obdachlos mit Stammplatz) | N-FA-015 |
| N-EN-162 | Ein Haushalt hat keine Mitglieder, die tot oder ausgezogen sind, ohne entsprechendes Ereignis | N-FA-003 |
| N-EN-163 | Heirat, Trennung, Geburt, Tod und Umzug sind Ereignisse im Journal | N-FA-003, N-FA-008 |
| N-EN-164 | Eine Verwandtschaftskante ändert sich nur durch ein Ereignis (Adoption, Entdeckung) | Abschnitt 3 |
| N-EN-165 | Die Bevölkerung wird bei Tod, Zuzug und Geburt konsistent bilanziert, sobald Generationen aktiviert sind | N-FA-005 |
| N-EN-166 | Kinder sind vom Adult-System ausgeschlossen | N-FA-019 |
| N-EN-167 | Ein Umzug ändert Netz und Routine, nicht ID und Historie | N-ID-019 |
| N-EN-168 | Feiertage verändern Routinen von Familien nachvollziehbar | N-FA-012 |
| N-EN-169 | Ein Ort behält seine Geschichte bei Betreiberwechsel | N-GE-026 |
| N-EN-170 | Die Familie eines Opfers erfährt vom Tod über einen existierenden Weg | N-GE-008 |
| N-EN-171 | Keine Genealogie wird für NPCs vorausgesetzt, die sie nicht brauchen | Abschnitt 3 |

---

## 12. Was fehlt

- Keine Zahlen: Haushaltsgrößen, Altersverteilung, Kinderzahl (Q-N18).
- Offen: Familie des Spielercharakters (Q-N17), Familiengründung (N-FA-010), Schwangerschaft (`35#281E` Punkt 14), Einwanderer (N-ID-023).
- Offen: Rechtsfrage zur Darstellung von Kindern (N-FA-019).
- Haushalte im Untergrund: historisch (N-02). Restfrage für Bunker und kleinere Untergrundbereiche offen (Q-N25).
- Kein Entwurf für Beerdigungen als Ereignisse (nur „ja kann sein“, `23#1057E` Punkt 104).
