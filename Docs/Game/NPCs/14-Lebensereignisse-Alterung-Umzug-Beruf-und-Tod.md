# NPCs 14: Lebensereignisse, Alterung, Umzug, Beruf und Tod

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die großen Wendepunkte im Leben eines NPCs und die langsame Veränderung über Jahre: Alterung, äußere Veränderung, Krankheit und Verletzung, Umzug, Berufswechsel, Tod und Folgen. Beschlossen sind die Richtung (NPCs altern, ziehen um, wechseln Jobs, sterben dauerhaft, die Welt erinnert sich) und die Regeln der Medizin im Master; **Ereigniskatalog, Taktung und Wirkung sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Lebenszustände sind in [Modul 03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md) §7 definiert, Familie in [Modul 10](10-Familien-Haushalte-und-soziale-Netzwerke.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Ereignisse im Lebenslauf](#2-entwurf-ereignisse-im-lebenslauf)
3. [Entwurf: Alterung](#3-entwurf-alterung)
4. [Entwurf: Äußere Veränderung](#4-entwurf-äußere-veränderung)
5. [Entwurf: Verletzung, Krankheit, Genesung](#5-entwurf-verletzung-krankheit-genesung)
6. [Entwurf: Umzug](#6-entwurf-umzug)
7. [Entwurf: Beruf und Status](#7-entwurf-beruf-und-status)
8. [Entwurf: Tod und seine Folgen](#8-entwurf-tod-und-seine-folgen)
9. [Entwurf: Generationen und Bevölkerung](#9-entwurf-generationen-und-bevölkerung)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-LB-001 | NPCs altern sichtbar; Alterung ändert Aussehen, Stimme, Fitness, Verhalten und Lebensstil langsam | 📋 | `23#1056A` Frage 8; „ja genau“ in `23#1057E`; `35#272A` Fragen 1–2; „1-50 passt alles“ in `35#273E` |
| N-LB-002 | Alterung verändert Körperform und Haut subtil | 📋 | `35#280A`/`35#281E`: Frage 13 der Runde 10A („Soll Alterung Körperform und Haut subtil verändern?“); „10-13 ja“ in `35#281E` |
| N-LB-003 | Fitness und Training verändern den Körper langfristig; Gewicht kann zu- und abnehmen; Verletzung und Krankheit beeinflussen Haltung und sichtbaren Zustand | 📋 | `35#280A` Runde 10A Fragen 10–12; „10-13 ja“ in `35#281E` |
| N-LB-004 | Haare und Bart wachsen über In-Game-Zeit; Frisuren entstehen nur aus der vorhandenen Länge | 📋 | `35#281E` Punkt 27 („haare und bart wachsen mit der zeit und man kann nicht einfach von buzzcut auf dauerwelle gehen ohne die haare eine zeit lang wachsen zu lassen“); `35#282A` |
| N-LB-005 | Narben, Tattoos und Piercings sind persistent und individuell; NPCs können Tattoos und Körpermodifikationen unabhängig vom Spieler bekommen, ändern oder entfernen; Raten ändern sich über Jahre | 📋 | `35#281E` Punkte 15–17; `35#785E` Punkte 137–138; `35#786A` |
| N-LB-006 | Ein NPC kann Jahre später äußerlich verändert sein, ohne dass der Spieler dabei war („manche leute die man vor paar jahren getroffen hat können auf einmal ein fullbodysuit haben“) | 📋 | `35#785E` Punkt 138 |
| N-LB-007 | Persönlichkeit bleibt im Kern stabil; starke Lebensereignisse verschieben sie teilweise; eine sehr lange Beziehung prägt stärker als viele kurze Begegnungen | 📋 | `35#272A` Fragen 3–4; „1-50 passt alles“ in `35#273E` |
| N-LB-008 | Heirat, Langzeitpartnerschaft, Kinder, Familienstatus, Umzug (auch zwischen den Bundesstaaten), Beruf und Einkommen können sich ändern und wirken auf Verhalten, Orte und Zeit | 📋 | `35#272A` Fragen 17–19, 44–48; „1-50 passt alles“ in `35#273E` |
| N-LB-009 | Der Beruf kann sich ändern, ebenso die Lebenssituation; Statusverlust oder -gewinn verändert Anziehung bei manchen Personen, bei anderen kaum | 📋 | `35#272A` Fragen 46–48; „1-50 passt alles“ in `35#273E` |
| N-LB-010 | Äußere Präferenzen können sich im Lauf des Lebens etwas verändern | 📋 | `35#272A` Frage 49; „1-50 passt alles“ in `35#273E` |
| N-LB-011 | Veränderungen laufen **langsam und nachvollziehbar** | 📋 | `35#272A` Frage 50; „1-50 passt alles“ in `35#273E` |
| N-LB-012 | Normale NPCs können verletzt werden und genesen; schwere Verletzungen haben bei wichtigen NPCs dauerhafte Folgen; Krankenhaus und Rettungsdienst holen verletzte NPCs | 📋 | `23#1056A` Fragen 71–72, 100–101; „ja genau“ und „ja sicher, große narben, eine hand verloren, alles soll es geben“ in `23#1057E` |
| N-LB-013 | Spieler und menschliche NPCs teilen dieselbe grundlegende Anatomie- und Traumaregel; **kein Plot Armor** | 📋 | `M-PD` ‚Human anatomy, injury and medicine‘ |
| N-LB-014 | Genesung läuft in Stufen (akut → stabilisiert → heilend → erholend → erholt); wiederholte Verletzung einer nicht ausgeheilten Region erreicht schneller schwere Zustände | 📋 | `M-PD` ‚Human anatomy, injury and medicine‘ |
| N-LB-015 | Behandlungsstufen: Feldversorgung für leichte Verletzungen und Stabilisierung; medizinisch geschulte Personen für einige mittlere Fälle; Krankenhaus für schwere innere Verletzungen, komplizierte Schüsse, schwere offene Brüche, schwere Verbrennungen, kritische Infektionen, Massenblutverlust | 📋 | `M-PD` ‚Human anatomy, injury and medicine‘ |
| N-LB-016 | Der Tod eines persistenten NPCs bleibt dauerhaft; ein Geschäft bekommt neuen Mitarbeiter oder Eigentümer, die Familie reagiert; bei bedeutenden NPCs möglicherweise Beerdigung („ja kann sein“); andere NPCs erinnern sich an Verstorbene | 📋 | `23#1056A` Fragen 102–104, 112; Antworten in `23#1057E` |
| N-LB-017 | Wichtige NPCs sterben nicht durch banale Simulationsfehler | 📋 | `23#1056A` Frage 105; Antwort in `23#1057E` |
| N-LB-018 | Nach dem Tod bleiben Identität und wichtige Historie im Weltzustand; nur unnötiger technischer Detailzustand darf später verdichtet werden; andere reagieren auf den Tod; Trauer ist je nach Beziehung unterschiedlich stark und lang | 📋 | `35#272A` Fragen 31–37; „1-50 passt alles“ in `35#273E` |
| N-LB-019 | Generationen: Die Welt soll theoretisch über Jahrzehnte Nachwuchs hervorbringen, **erst spät** umgesetzt | 📋 | `23#1056A` Frage 9; Antwort in `23#1057E` |
| N-LB-020 | Das Spiel soll auch nach Jahrzehnten noch wissen, wer früher mit wem verbunden war | 📋 | `35#272A` Frage 38; „1-50 passt alles“ in `35#273E` |
| N-LB-021 | Der Tod einer Person ist ein historisches Weltereignis; Orte, Fälle und Dialoge können darauf Bezug nehmen | 📋 Richtung | `35#272A` Frage 39; „1-50 passt alles“ in `35#273E`; `23#1058A` ‚Sozialer World State‘ |
| N-LB-022 | Tiere (Hunde) altern; Tierarzt | 📋 | `23#1088A` Fragen 39–40; „31-40 ja, impfungen sind optional“ in `23#1089E` |
| N-LB-023 | Der Spielertod bzw. das Scheitern ist offen; Festnahme ≠ Game Over | ❓ / 📋 | `M-PD` ‚Death / failure — OPEN‘; `23#1058A` |
| N-LB-024 | Medizinische Berufe, Rettungsdienste und Krankenhäuser sind Teil der Weltsimulation (Feuerwehr, EMS, Krankenhaus, Bergrettung, Wasserrettung, Suchaktionen) | 📋 | `23#1056A` Fragen 70–76; „ja“ in `23#1057E` |
| N-LB-025 | Amputation beim Spieler: nur in der Story; Prothesen als Teil von Story/Progression | 📋 | „107 nur in der story“ und „108 wird man in der story bekommen“ in `23#1089E` |

---

## 2. Entwurf: Ereignisse im Lebenslauf

Das Journal des NPCs enthält **Lebensereignisse** (`life_log`, Modul 03 §3). Ein Ereigniskatalog mit Auslösern (💡):

| Kategorie | Ereignis | Typische Auslöser | Wirkung auf Datensatz |
|---|---|---|---|
| **Familie** | Heirat, Zusammenzug, Trennung, Scheidung, Geburt, Tod eines Angehörigen | Beziehung, Alter, Lage | Haushalt, Kanten (Modul 10) |
| **Arbeit** | Einstellung, Kündigung, Entlassung, Beförderung, Berufswechsel, Ruhestand | Betrieb, Leistung, Alter | Einkommen, Zeitplan, Kreise |
| **Wohnen** | Umzug, Kauf, Verkauf, Räumung, Obdachlosigkeit | Einkommen, Beruf, Beziehung | Wohnort, Nachbarn |
| **Gesundheit** | Krankheit, Verletzung, Operation, Genesung, chronische Folge, Sucht, Entzug | Lage, Alter, Zufall | Zustand, Kosten, Aussehen |
| **Gesellschaft** | Gruppenbeitritt/-austritt, Ehrenamt, Verurteilung, Haft, Entlassung | Interesse, Tat | Zugehörigkeit, Ruf |
| **Persönlich** | Trauma, Bekehrung, Glaubenswechsel, Wendepunkt, Entschluss | Erlebnis | Persönlichkeitsverschiebung (Modul 04) |
| **Äußeres** | Tattoo, Piercing, Gewicht, Frisur, Bart, Narbe, Kleidungsstil | Lebensstil, Trend, Geld | Aussehen (Abschnitt 4) |
| **Finanzen** | Gewinn, Verlust, Schulden, Erbe | Zufall, Handlung | Klasse, Optionen |
| **Verbrechen** | Täter, Opfer, Zeuge | Gelegenheit, Not | Polizei, Folgen |
| **Übernatürlich** | Begegnung, Besessenheit, Fluch, Verwandlung | Welt (Kreaturen) | Zustand (Modul 03 §7) |
| **Tod** | Ende des Lebens | Alter, Krankheit, Gewalt, Unfall, Übernatürliches | Weltzustand (Abschnitt 8) |

**Regeln (Entwurf):**

1. **Wenige, wirksame Ereignisse.** Nur Ereignisse mit Folge sind im Lebenslauf (Sparsamkeit, Modul 05 §5.3).
2. **Taktung.** Ereignisse entstehen aus Wahrscheinlichkeiten pro Zeitschritt (Wochen/Monate) abhängig von Alter, Lage, Persönlichkeit; die Hintergrundsimulation holt sie nach (Modul 22).
3. **Verkettung.** Ereignisse haben Folgeereignisse (Heirat → Umzug → neuer Job).
4. **Sichtbarkeit.** Wichtige Ereignisse erreichen den Spieler über Gerüchte, Nachrichten, Begegnungen (Modul 13 §10).
5. **Schutz.** Ereignisse mit Folgen für geschützte NPCs unterliegen den Schutzstufen (Modul 03 §8).
6. **Vorhersehbarkeit.** Ein Ereignis ohne erkennbaren Anlass ist verboten (N-EN-021).

---

## 3. Entwurf: Alterung

**Quelle:** NPCs altern; die Alterung ändert Aussehen, Stimme, Fitness, Verhalten, Lebensstil (N-LB-001). Die Welt soll über Jahre bis Jahrzehnte laufen können (`23#1056A` Frage 8).

**Entwurf (💡):**

| Bestandteil | Beschreibung |
|---|---|
| **Alter** | Funktion des Weltdatums und des Geburtsdatums; kein gespeicherter Zähler (Modul 03) |
| **Lebensphasen** | Kind, Jugend, junger Erwachsener, Erwachsener, mittleres Alter, Alter, hohes Alter |
| **Alterung nach Phase** | Haut, Haare, Haltung, Stimme, Beweglichkeit, Kraft, Ausdauer; Lebensstil und Genetik modulieren |
| **Schwelle der Sichtbarkeit** | Veränderungen sind nur über Jahre sichtbar; im Spielverlauf von Wochen kaum |
| **Gesundheit** | Altersbedingte Krankheiten werden wahrscheinlicher (Modul 04 §9 Q-N12) |
| **Verhalten** | langsame Veränderung von Vorlieben, Risikobereitschaft, Tempo (Modul 04) |
| **Beruf** | Ruhestand, Teilzeit, Wechsel |
| **Tod** | Wahrscheinlichkeit steigt mit Alter; ist nicht zufällig im Sinn von Würfelwurf-Streuung ohne Ursache (Abschnitt 8) |
| **Gedächtnis** | Verblassen und Nachlassen (Modul 05); optional Demenz (Q-N12) |

**Zeitskala.** Wie schnell die Weltzeit läuft, ist offen; die Planung rechnet in Jahren und Jahrzehnten (Diner nach 15 Jahren: `23#1058A`). Ob die Welt 1:1 oder mit eigener Zeitskala läuft, ist nicht Gegenstand dieser Dokumentation ([Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md)).

**Zeitreise.** In der Vergangenheit sind die NPCs in ihrem damaligen Alter (N-GE-028).

**Alter der Spielerfiguren und Begleiter.** Auch der Spieler und die Crew unterliegen der Zeit, soweit die Welt über Jahre läuft; ob und wie schnell, entscheidet die Weltuhr-Planung (❓ Q-N21).

---

## 4. Entwurf: Äußere Veränderung

Der Aussehensgenerator ([Decision 0005](../../Decisions/0005-character-appearance-foundation.md)) erzeugt Aussehen aus Rezepten. Die Quellen verlangen, dass Aussehen sich über Zeit ändert (N-LB-001 bis N-LB-006). Der Entwurf (💡) legt fest, **welche Änderungen als Ereignisse** entstehen und welche als Funktion der Zeit:

| Änderung | Art | Anlass (Entwurf) | Dauer |
|---|---|---|---|
| **Alter** (Haut, Haare grau, Haltung) | Funktion der Zeit | Alter | Jahre |
| **Gewicht, Muskeln** | Funktion von Lebensstil, Beruf, Training | Gewohnheit | Wochen bis Jahre |
| **Haare, Bart** | Funktion der Weltzeit (N-LB-004) | Wuchs | Tage bis Monate |
| **Frisur** | Ereignis | Friseurbesuch, Stil | sofort aus vorhandener Länge |
| **Narbe** | Ereignis | Verletzung | dauerhaft |
| **Tattoo, Piercing, Körpermodifikation** | Ereignis | Szene, Mode, Geld, Persönlichkeit | dauerhaft; heilt (Sitzungen) |
| **Kleidungsstil** | Gewohnheit/Ereignis | Beruf, Geld, Mode, Beziehung | Wochen bis Jahre |
| **Verletzungsspuren** | Zustand | akute Verletzung | bis Genesung |
| **Make-up** | Anlass | Arbeit, Freizeit | Stunden |
| **Prothese** | Ereignis | Verlust | dauerhaft |
| **Krankheitsbild** | Zustand | Krankheit | Wochen bis dauerhaft |

**Regeln (Entwurf):**

1. **Das Rezept bleibt, Abweichungen kommen als Überschreibungen** (Modul 03 §2; N-ID-020).
2. **Keine sprunghafte Veränderung** außerhalb von Ereignissen (N-LB-011): Haare wachsen, Gewicht ändert sich langsam.
3. **Sichtbare Veränderung ist erkennbar** für Wiedererkennung (N-GE-015): Jahre ändern, was jemand erkennt.
4. **Tattoo-Raten** der Bevölkerung ändern sich über Jahre/Trends (`35#785E` Punkt 138; N-LB-005).
5. **Gruppenzugehörigkeit** zeigt sich in Symbolen, Tattoos, Patches (Modul 16 §9).

**Zuständigkeit.** Die Technik der Veränderung (Rezept, Layer, Overrides) gehört zum Aussehenssystem ([Decision 0005](../../Decisions/0005-character-appearance-foundation.md)); dieses Modul legt nur fest, **welches Ereignis was bewirkt**.

---

## 5. Entwurf: Verletzung, Krankheit, Genesung

**Quelle:** Menschen teilen dieselbe Anatomie- und Traumaregeln; keine Plot Armor (N-LB-013); Genesung in Stufen; Behandlungsstufen (N-LB-014, N-LB-015). NPC-Verletzungen werden von EMS abgeholt und im Krankenhaus behandelt (N-LB-012).

**Entwurf – Verlauf für NPCs:**

```text
Verletzung ──► Akut ──► Stabilisiert ──► Heilend ──► Erholend ──► Erholt
                 │          │
        Behandlung?  Feld | Arzt | Krankenhaus      (N-LB-015)
                 │
                 ├── Tod (ohne Behandlung oder bei schwerem Trauma)
                 └── Dauerfolge (Narbe, Behinderung, Prothese)
```

| Aspekt | Entwurf |
|---|---|
| **Schwere** | Zustand aus Trauma und Behandlung; Verletzungen akkumulieren |
| **Behandlung** | abhängig von Verfügbarkeit (Rettungsdienst, Arzt, Krankenhaus) und Mitteln; Versicherung und Geld (abstrakt) |
| **Dauer** | Heilung dauert Wochen bis Monate; Re-Injury früher schwerer (N-LB-014) |
| **Folgen** | Narben und Einschränkungen bleiben (N-LB-012); Auswirkung auf Arbeit, Fahrzeug, Hobby |
| **Krankheit** | Infektion, chronische Erkrankung, Sucht; Zeit, Kosten, Isolierung |
| **Arbeit** | Krankschreibung, Verlust von Schichten, Berufswechsel |
| **Psyche** | Trauma, Angst (Modul 04 §9) |
| **Hunter-Verletzung** | Hunter haben dieselbe Medizin; ein Krankenhaus kann bei auffälligen Verletzungen Fragen stellen oder melden (💡, Folge des Zeugen- und Behördenmodells, Ermittlung 14) |
| **Übernatürliche Verletzung** | durch Modifikatoren (Heilungsunterdrückung, Fluch); eigene Regeln ([Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §10–§11) |

**Regeln (Entwurf):**

1. **Keine Plot Armor.** Tödliche Treffer töten wichtige NPCs ebenso wie den Spieler, **wenn** sie vom Spieler oder von einer erzählerisch gewollten Gewalt stammen; der Schutz gegen *triviale Simulationsunfälle* (N-LB-017) bleibt davon getrennt (⚠️ Spannung, siehe [Modul 27](27-Konflikte-und-offene-Fragen.md) C-N1).
2. **Behandlung braucht einen Weg.** Ein Verletzter, den niemand findet, kann sterben (Vermisstenfall).
3. **Krankenhäuser sind Orte der Weltsimulation** (N-LB-024): Patienten, Personal, Besucher.
4. **Verletzungen sind Spuren** für Ermittlungen (Ermittlung 11).
5. **Genesung bewegt sich in Weltzeit.**

---

## 6. Entwurf: Umzug

**Quelle:** NPCs ziehen um bei Jobwechsel, Einkommen, Beziehung oder Stadtentwicklung, aber nicht zu oft (N-LE-009). Ein NPC kann zwischen den Bundesstaaten umziehen und behält Identität (N-ID-019).

| Aspekt | Entwurf |
|---|---|
| **Auslöser** | Jobangebot oder -verlust, Beziehung (Heirat, Trennung), Familie, Geld, Wohnungsmarkt, Katastrophe, Gefahr, Stadtentwicklung (Abriss, Neubau), Flucht |
| **Zielwahl** | passende Wohnung/Region nach Einkommen, Beruf, Netz, Preisen |
| **Ablauf** | Entscheidung → Suche → Umzug → Eingewöhnung; Zwischenzustand (`AWAY`, bei Verwandten, Hotel) |
| **Haushalt** | folgt als Ganzes oder teilt sich (Modul 10 §6) |
| **Netz** | Kanten zu Nachbarn und Kollegen werden schwächer; Familie und Freunde bleiben als Fernbeziehungen |
| **Besitz** | Haus wird frei, neuer Bewohner (N-FA-003) |
| **Spieler** | erfährt es über Nachrichten, Besuch, Briefe; kann den Umzug beeinflussen (Geld, Hilfe) |
| **Selten** | „nicht zu oft“ (N-LE-009): niedrige Eintrittswahrscheinlichkeit |
| **Region** | Umzug in andere Bundesstaaten ist möglich (N-LB-008) |

**Regeln (Entwurf):**

1. **Umzug ändert nicht die Identität** (N-EN-167).
2. **Ein leerer Raum wird neu besetzt** (Hauswechsel).
3. **Beziehungen zum Spieler bleiben** und werden gepflegt oder verblassen (Modul 08, 09).
4. **Umzug aus Gefahr.** Nach einem Vorfall ziehen Menschen weg (N-IN-006), was Nachbarschaften verändert.
5. **Umzug in die Nähe des Spielers.** Kann geschehen; nicht gesteuert.

---

## 7. Entwurf: Beruf und Status

Die Quellen nennen Berufswechsel, Statusveränderung und Einkommen (N-LB-009). Der Entwurf (💡):

| Aspekt | Beschreibung |
|---|---|
| **Berufsverlauf** | Ausbildung/Einstieg, Aufstieg, Wechsel, Arbeitslosigkeit, Ruhestand |
| **Status** | Einkommensklasse, Besitz, Bekanntheit, Position in Gruppen |
| **Status-Wirkung** | beeinflusst Anziehung bei manchen NPCs (N-LB-009), Wohnort, Freizeit, Kriminalitätsrisiko (mit N-LT-031 als Schranke) |
| **Hunter** | berufen sich nicht auf klassische Karriere; Hunter-Netz hat eigene Struktur (Kreaturen 11) |
| **Unternehmer** | Betriebe wechseln Besitzer (Modul 10 §5); der Spieler kann später Unternehmer sein (P3, `23#1090A`) |
| **Behörden** | Beamte steigen auf, wechseln Dienststelle, kündigen, werden korrupt (Modul 18) |
| **Kriminelle Laufbahn** | Einstieg, Aufstieg, Haft, Ausstieg (Modul 16) |

**Regeln (Entwurf):**

1. **Beruf bestimmt Zeitplan, Kreise, Wissen.** Ein Mechaniker kennt Fahrzeuge; ein Arzt Verletzungen (Ermittlung-Fähigkeiten).
2. **Berufswechsel ist Ereignis** (Journal).
3. **Statuswechsel wirkt auf Beziehungskanten** (Modul 08).

---

## 8. Entwurf: Tod und seine Folgen

### 8.1 Ursachen

| Ursache | Beispiel | Besonderheit |
|---|---|---|
| **Alter, Krankheit** | natürlicher Tod | Wahrscheinlichkeit nach Alter und Gesundheit; Ereignis im Journal |
| **Unfall** | Verkehr, Haushalt, Arbeit | für geschützte NPCs gesperrt (`sim_safe`) |
| **Gewalt** | Raub, Streit, Krieg zwischen Gruppen | Täter, Zeugen, Polizei |
| **Spieler** | bewusst herbeigeführt | N-ID-001: „dann ist die person weg“ |
| **Story** | erzählerisch gewollt | bleibt tot (`23#1089E` Punkt 26) |
| **Übernatürlich** | Angriff, Fluch, Besessenheit | Kreaturen |
| **Suizid, Überdosis** | Not, Sucht | sensibel, in Darstellung zu prüfen |
| **Hinrichtung, Haft** | Justiz | Ermittlung 15, 16 |
| **Katastrophe** | Sturm, Feuer, Blackout | Ermittlung 26 |

### 8.2 Folgen

| Folge | Beschreibung | Quelle |
|---|---|---|
| **Weltzustand** | `dead` dauerhaft (N-ID-018) | N-LB-016 |
| **Haushalt** | Mitglieder reagieren; Einkommen, Wohnen ändern sich | N-FA-014 |
| **Betrieb** | Ersatz, Nachfolger, Schließung | N-LB-016 |
| **Beziehungen** | Kanten frozen; Trauer; Groll; Schuld (Modul 08) | N-LB-018 |
| **Gerüchte, Nachrichten** | Bekanntwerden, Verzerrung (Modul 07) | N-IN-001 |
| **Behörden** | Meldung, Untersuchung, Totenschein, Obduktion (Ermittlung 11, 14) | – |
| **Beerdigung** | bei bedeutenden Figuren möglicherweise (Friedhof, Trauerfeier) | „ja kann sein“ in `23#1057E` Punkt 104 |
| **Erbe** | Besitz und Schuld gehen über (Familie, Gläubiger) | 💡 |
| **Erinnerung** | Gedächtnis anderer (Modul 05 §9) | N-LB-018 |
| **Orte** | Ort erhält Geschichte (Haus, Grab, Stammlokal) | N-GE-026 |
| **Fälle** | Fälle schließen, öffnen, verändern sich | Ermittlung 09 |
| **Übernatürliches** | Geist, Anker, Wiederkehr möglich (Kreaturen 05, 07) | – |

### 8.3 Regeln (Entwurf)

1. **Tod ist endgültig im Weltzustand** (N-EN-006).
2. **Tod ohne Zeugen.** Der Weltzustand ändert sich; Wissen verbreitet sich nur über Wege (N-GE-008); eine Leiche kann lange ungefunden sein (Vermisstenfall).
3. **Tod auf Distanz.** Der Spieler erfährt vom Tod eines Freundes über Nachricht, Anruf, Familie; Zeitverzögerung (Modul 07).
4. **Schutz.** Schutzstufen (Modul 03 §8) regeln, wer wie sterben kann; offen bleibt, wer außer dem Spieler töten darf (C-N1).
5. **Hunter-NPCs.** Sie können sterben (Kreaturen 11 §3); der Tod ist ein Weltereignis.
6. **Kein Tod als Mittel der Erzählung ohne Weltzustand.** Auch Story-Tode sind Ereignisse im Journal.

---

## 9. Entwurf: Generationen und Bevölkerung

**Quelle:** Generationen sind gewollt und **spät** (N-LB-019). Vorbereitungen: Modul 10 §7.

**Entwurf (💡):**

1. **Bevölkerungsbilanz** statt Einzelmodell: Geburten, Todesfälle, Zuzug, Wegzug je Region und Jahr; Haushalte tragen die Details.
2. **Heranwachsende** werden aus Kindern bekannter Haushalte zu eigenständigen Erwachsenen mit ID und Rezept (Datensatz erweitert).
3. **Altersverteilung** bleibt stabil, damit die Bevölkerung nicht altert oder schrumpft.
4. **Bekannte Kinder.** Ein Kind, das der Spieler kannte, wird später ein Erwachsener mit Erinnerungen (Modul 05).
5. **Aktivierung** erst, wenn Hintergrundsimulation (Modul 22) und Haushalte (Modul 10) belastbar sind (Proposal 0010 Phasen L4/L5).

---

## 10. Sonderfälle

1. **Sehr langlebige Wesen.** Wesen altern nach Artregeln; ein Wesen mit menschlicher Identität muss seine Langlebigkeit verbergen (Wohnortwechsel, Identitätswechsel; Kreaturen 09).
2. **Jung gebliebene Figuren.** Verjüngung ist in den Quellen als begrenzte Möglichkeit fortgeschrittener Medizin erwähnt ([Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §14.2); Einzelheiten sind Teil der Hauptgeschichte und werden hier nicht ausgeführt.
3. **Todesfall des Spielercharakters.** Offen (`M-PD`).
4. **Tod mehrerer NPCs in Katastrophen.** Das Weltereignis erzeugt viele Todesfälle; Verdichtung für Hintergrund (Modul 22).
5. **Zeitreise.** Ein lebender NPC in einer Zeit, in der er tot ist, ist ein Zeitparadoxon; die Regeln stehen in Welt 26.
6. **Tote kehren zurück (Geister, Wiedergänger).** Regeln aus Kreaturen 05/07; die ID bleibt, die Rolle ändert sich.
7. **Falsche Todesmeldung.** Ein NPC gilt als tot (Gerücht, falsche Identifizierung); er lebt (Ermittlung 13).
8. **Verschwinden statt Tod.** `missing` (Modul 03 §7).
9. **Mehrere Todesursachen.** Eine Ursache wird als führend geführt; Ermittlung kann weitere finden.
10. **Alterstod vor Spielerbegegnung.** Eine Figur, die in der Hauptstory vorgesehen ist, stirbt nicht außerhalb der Erzählung (Schutz, Modul 03 §8).
11. **Alter der Crew.** Siehe Q-N21.

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-261 | Jede Änderung im Lebenslauf eines NPCs hat ein Ereignis mit Ursache | N-LB-011 |
| N-EN-262 | Das Alter eines NPCs ergibt sich aus Geburts- und Weltdatum | Abschnitt 3 |
| N-EN-263 | Haar- und Bartlänge sind Funktionen der Weltzeit seit dem letzten Schnitt | N-LB-004 |
| N-EN-264 | Eine Frisur kann nicht länger sein als die vorhandene Haarlänge | N-LB-004 |
| N-EN-265 | Eine Narbe verschwindet nicht ohne Ereignis | N-LB-005 |
| N-EN-266 | Ein NPC im Zeitstand T hat sein damaliges Alter | N-GE-028 |
| N-EN-267 | Ein Umzug ist ein Ereignis mit Ursache und ändert Haushalt, Wohnort, Netz | N-LE-009 |
| N-EN-268 | Ein verletzter NPC durchläuft die Genesungsstufen in Weltzeit | N-LB-014 |
| N-EN-269 | Ein schwer Verletzter ohne Behandlung kann sterben | N-LB-013 |
| N-EN-270 | Die Zahl der Todesfälle ohne Ursache beträgt null | N-LB-016 |
| N-EN-271 | Ein toter NPC hat einen Datensatz, aber keine Handlungen | N-ID-018 |
| N-EN-272 | Ein geschützter NPC stirbt nicht außerhalb der erlaubten Wege | N-LB-017 |
| N-EN-273 | Eine Bevölkerungsbilanz (aktiviert) bleibt innerhalb vorgegebener Grenzen | N-LB-019 |
| N-EN-274 | Die Todesnachricht erreicht Angehörige über einen existierenden Weg und erzeugt Trauer, Erbe und Haushaltsänderung | N-LB-016 |

---

## 12. Was fehlt

- Keine Zeitskala der Weltuhr und keine Altersraten (Q-N21).
- Keine Sterbewahrscheinlichkeiten, keine Krankheitsmodelle, keine Preise.
- Offen: Demenz und Alterskrankheiten (Q-N12).
- Offen: Umgang mit Suizid und sensiblen Todesursachen in der Darstellung.
- Offen: Wie sich die Bevölkerung nach Katastrophen und Monsterfällen bilanziert.
