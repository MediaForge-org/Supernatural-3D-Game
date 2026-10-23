# NPCs 13: Autonomes Leben – Tagesabläufe, Arbeit, Freizeit und Reisen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie NPCs ohne den Spieler leben: Tages- und Wochenabläufe, Arbeit, Einkaufen, Freizeit, Reisen und eigenständige Entscheidungen. Beschlossen ist, **dass** NPCs echte Abläufe haben und woran sie sich ändern; **Zeitplanmodell, Entscheidungslogik und Hintergrundzustände sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Verkehrsverhalten und Fahrertypen stehen in [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md), Alibi in [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §8, Wirtschaft in [Welt 12](../World/12-Gesellschaft-Wirtschaft-und-Ressourcen.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Das Zeitplanmodell](#2-entwurf-das-zeitplanmodell)
3. [Entwurf: Orte und Aktivitäten](#3-entwurf-orte-und-aktivitäten)
4. [Entwurf: Abweichungen vom Normalplan](#4-entwurf-abweichungen-vom-normalplan)
5. [Entwurf: Entscheidungslogik](#5-entwurf-entscheidungslogik)
6. [Entwurf: Reisen und Mobilität](#6-entwurf-reisen-und-mobilität)
7. [Entwurf: Arbeit, Einkommen und Betriebe](#7-entwurf-arbeit-einkommen-und-betriebe)
8. [Entwurf: Hintergrundzustände und Materialisierung](#8-entwurf-hintergrundzustände-und-materialisierung)
9. [Entwurf: Eigenständige Lebensentscheidungen](#9-entwurf-eigenständige-lebensentscheidungen)
10. [Entwurf: Sichtbarkeit für den Spieler](#10-entwurf-sichtbarkeit-für-den-spieler)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

Alle Nummern in der Spalte ‚Quelle‘ beziehen sich auf die nummerierten Fragen des Assistenten in `23#1056A` (Block 8), die der Entwickler in `23#1057E` einzeln beantwortet hat.

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-LE-001 | Jeder relevante NPC hat einen echten Tages- und Wochenablauf (schlafen, arbeiten, einkaufen, Freizeit, nach Hause) | 📋 | Frage 1; „ja schon“ in `23#1057E` |
| N-LE-002 | Nicht jeder Tag ist gleich: Wochenende, Urlaub, Krankheit, Überstunden, Events, Wetter verändern Routinen | 📋 | Frage 2; „genau“ in `23#1057E` |
| N-LE-003 | NPCs fahren oder gehen zu ihrem Arbeitsplatz; sie verschwinden nicht einfach | 📋 | Frage 3; „ja“ |
| N-LE-004 | NPCs gehen einkaufen (Supermärkte, Shops, Restaurants) | 📋 | Frage 13; „ja“ |
| N-LE-005 | NPCs benutzen Freizeitorte wirklich (Bars, Clubs, Sportanlagen, Parks, Casinos, Skipisten, Kinos): „ja, die welt muss realistisch sein“ | 📋 | Frage 14; Antwort in `23#1057E` |
| N-LE-006 | NPCs machen Wochenendtrips, Urlaub und Besuche in andere Städte: „ja natürlich, sonst könnten nie staus sein“ | 📋 | Frage 15; Antwort in `23#1057E` |
| N-LE-007 | Arbeitsplatzwechsel, Kündigung, Entlassung, Karriere, Arbeitgeberwechsel sind möglich | 📋 | Frage 11; „ja“ |
| N-LE-008 | Einkommen und Wohlstand beeinflussen Wohnort, Auto und Tätigkeit zumindest abstrakt | 📋 | Frage 12; „ja“ |
| N-LE-009 | NPCs ziehen um, aber nicht zu oft | 📋 | Frage 10; „können sie, aber nicht zu oft“ |
| N-LE-010 | Unternehmen stellen ein und entlassen; Geschäfte bekommen neue Betreiber; Häuser wechseln Bewohner | 📋 | Fragen 108–110; „ja“ |
| N-LE-011 | Nachbarschaften verändern sich über Jahre; Orte entwickeln Geschichte | 📋 | Fragen 111, 113; „ja“ |
| N-LE-012 | NPCs besitzen Kleidung für verschiedene Anlässe und wechseln sie nach Wetter, Arbeit, Anlass | 📋 | Frage 21; „ja“ |
| N-LE-013 | Hintergrundsimulation weit entfernter NPCs berechnet **nicht** die exakte Gehbewegung, sondern nur den Zustand (`AT_WORK`, `HOME`, `TRAVEL` …); beim Annähern wird daraus wieder ein konsistenter detaillierter NPC | 📋 | Fragen 122–123; „ja sowieso nicht, aber wenn man dann in das disktikt kommt darf die straße nicht leer sein, sondern auch voll“ und „ja sicher“ |
| N-LE-014 | Ein NPC außerhalb der geladenen Welt hat eine plausible Tagesroutine; bei plötzlicher Reise des Spielers zu ihm bekommt er einen konsistenten aktuellen Zustand aus der Hintergrundsimulation; der Hintergrund verhindert, dass ein NPC zwei Orte zugleich besetzt; Reisen zwischen Städten kosten auch im Hintergrund Zeit | 📋 Richtung | `35#278A` Fragen 17–20; „12-35 ja“ in `35#279E` |
| N-LE-015 | Hintergrundsimulation ist **ereignisbasiert** (statt jede Sekunde); Zufall entsteht deterministisch aus NPC-ID, Weltseed und Zeitfenster | 📋 Richtung | `35#278A` Fragen 13–15; „12-35 ja“ in `35#279E` |
| N-LE-016 | Nicht der gesamte Weg wird im Hintergrund gerechnet: „ja aber halt nicht der jetzige ort nicht der ganze weg und so weiter“ | 📋 | `35#279E` Punkt 5 (Frage 5 in `35#278A`) |
| N-LE-017 | Auch Obdachlose haben Abläufe; die meisten haben eine Wohnung, manche sind obdachlos | 📋 | Frage 4; Antwort in `23#1057E` |
| N-LE-018 | Feiertage und lokale Feste verändern Dekoration, Routinen, Öffnungszeiten, Verkehr, Veranstaltungen und Familienbesuche; einzelne Events finden nur einmal im In-Game-Jahr statt | 📋 | `23#1088A` Fragen 159–166; „159-166 ja“ in `23#1089E`; `23#1090A` |
| N-LE-019 | Touristen als eigener Populationstyp mit saisonalen Spitzen; Hotels reagieren mit Preisen und Belegung | 📋 | `23#1088A` Fragen 167–170; „167-173 ja“ in `23#1089E` |
| N-LE-020 | Crewmitglieder des Hunter-HQ schlafen, essen, trainieren, recherchieren, reparieren, streiten, feiern, verlassen das HQ und kommen später zurück | 📋 | [Spielvision](../Spielvision.md) §6.4; `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; Ausformulierung `23#603A` Abschnitt 2 (💡) |
| N-LE-021 | Das Hunter-HQ ist der primäre soziale Ort; Versorgung, Beiträge, Aufgaben und Feiern sind organisch und nicht erdrückend | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘ |
| N-LE-022 | Begleiter haben ein eigenes Leben und warten nicht permanent auf den Spieler; man kann sie anrufen und treffen | 💡 / 📋 Richtung | `23#510A` (Bewertungstabelle des Assistenten); `23#1088A` Frage 23; „16-24 ja“ in `23#1089E` |
| N-LE-023 | Kriminalität läuft unabhängig vom Spieler (Diebstahl, Einbruch, Raub, Autodiebstahl, Körperverletzung, Vandalismus, Drogenhandel, organisierte Kriminalität, Schmuggel, Glücksspiel, Rennen); Polizei reagiert auch ohne Spieler | 📋 | Fragen 46, 49; „ja schon, sehr komplex“, „ja sicher“ |
| N-LE-024 | NPC-Fahrer unterscheiden sich in Können (Anfänger, vorsichtig, normal, aggressiv, schlecht) und passen sich Wetter an; Rettungsgassen; NPC-Kriminelle können von der Polizei verfolgt werden | 📋 | Fragen 93–99; „ja“ [mit Zusätzen] in `23#1057E`; [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) |
| N-LE-025 | Die Welt wird nach Ereignissen repariert, wirklich von NPCs; der Spieler kann helfen | 📋 | Frage 81; „ja genau, und die straßen, gebäude,... werden auch von wirklich npcs repariert, man kann auch hilfe anbieten“ in `23#1057E` |
| N-LE-026 | Der Spieler soll bei Besuch einer Gegend nicht auf leere Straßen treffen; Menschenmengen werden aus dem Zustand rekonstruiert | 📋 | Frage 122 und Antwort in `23#1057E`; `23#1058A` ‚Technische NPC-Architektur‘ |
| N-LE-027 | Im Singleplayer läuft die Weltzeit nur im Spiel; Online bekommt später eigene Zeitlogik | 📋 | `35#279E` Punkt 11 |
| N-LE-028 | NPCs reagieren auf Weltereignisse, folgen dem Alltag und ändern ihn bei Gefahr | 📋 | `PE-N1022` F; `23#1056A` Fragen 114, 117 |

---

## 2. Entwurf: Das Zeitplanmodell

Ein Tagesablauf ist **kein Skript**, sondern ein **Plan aus Anker-Terminen und flexiblen Lücken**, der aus Rolle, Haushalt, Persönlichkeit und Lage erzeugt wird (💡).

```text
SchedulePlan (je NPC, abgeleitet; nur Abweichungen werden gespeichert, N-ID-020)
├── template        Rolle: Schichtarbeiter | Büroangestellter | Händler | Schüler | Rentner | Hunter ...
├── anchors[]       feste Termine:  Arbeit (Zeit, Ort), Schule, Gottesdienst, Training, Schicht
├── routine slots   Aufstehen, Frühstück, Fahrt, Arbeit, Pause, Einkauf, Freizeit, Heimfahrt, Abend, Schlaf
├── week pattern    Werktage, Wochenende, Feiertage
├── habits          Stammlokal, Stammgäste, Wege, Rituale (Langzustände, Modul 04)
├── commitments     Absprachen mit Anderen (Treffen, Dates, Fahrten)
└── exceptions      Urlaub, Krankheit, Überstunden, Ereignisse (Abschnitt 4)
```

**Entwurfsregeln:**

1. **Anker zuerst, Lücken flexibel.** Arbeit und Schule sind fest; Freizeit füllt, was bleibt (Persönlichkeit und Gelegenheit).
2. **Zeitplan aus Vorlage plus Rauschen.** Ein Büroangestellter beginnt um 8 Uhr, aber nicht jeder genau gleich. Vorlage und Seed ergeben einen individuellen Plan (N-ID-020).
3. **Ein Plan hat Folgen für andere.** Wenn A mit B verabredet ist, hängen ihre Pläne zusammen (Commitments).
4. **Haushaltspläne sind verschränkt.** Wer fährt die Kinder, wer kocht, wer holt ab (Modul 10 §2).
5. **Sparsamkeit.** Für Stufe C und D genügen Vorlage und Seed; Stufe A/B halten konkrete Anker und Commitments im Datensatz.
6. **Aufzeichnung.** Für Alibi und Ermittlung wird der *tatsächliche* Aufenthalt wichtiger Personen mit Zeitfenstern festgehalten (Ermittlung 12 §8).

**Zeitebene.** Der Plan arbeitet auf der Weltuhr (N-LT-020). Er kennt Wochentag, Datum, Feiertage, Jahreszeit, Tageslicht.

---

## 3. Entwurf: Orte und Aktivitäten

Die Quellen nennen Orte und Aktivitäten (Arbeitsplatz, Wohnung, Supermärkte, Shops, Restaurants, Bars, Clubs, Sportanlagen, Parks, Casinos, Skipisten, Kinos). Der Entwurf ordnet sie (💡):

| Ortsklasse | Beispiele | Aktivitäten | Wirkung |
|---|---|---|---|
| **Wohnen** | Wohnung, Haus | schlafen, essen, Familie | Privatsphäre; Familienroutinen |
| **Arbeit** | Büro, Fabrik, Werkstatt, Laden, Krankenhaus | arbeiten, Pause | Einkommen, Kollegenkreis |
| **Versorgung** | Supermarkt, Tankstelle, Apotheke, Bank | einkaufen, tanken | Alltagsbewegung |
| **Gastronomie** | Diner, Restaurant, Bar, Imbiss | essen, treffen | Stammgäste, Klatsch |
| **Freizeit** | Park, Kino, Sportanlage, Club, Casino, Skipiste, Strand | Entspannung, Training | Freizeitkreise |
| **Gemeinschaft** | Kirche, Verein, Clubhaus, Schule | Gottesdienst, Treffen | Zugehörigkeit |
| **Gesundheit** | Arzt, Krankenhaus, Tierarzt | Behandlung | Verletzung, Krankheit |
| **Verkehr** | Haltestelle, Bahnhof, Flughafen, Autobahn | reisen, pendeln | Verkehrsfluss (Welt 16) |
| **Verbrechen/Grauzone** | Schwarzmarkt, Untergrundkämpfe, Rennen | Geschäft, Kampf | Fraktionsorte (Modul 16, 17) |
| **Obdach** | Brücke, Park, Notunterkunft | Schlafen, Essen | Stammplatz |

**Regeln (Entwurf):**

1. **Kapazität.** Orte haben begrenzte Plätze; zu viele Besucher führen zu Warteschlangen, Ausweichen, Ausfall.
2. **Öffnungszeiten.** Folgen Wochentag, Feiertag, Saison, Ereignissen (N-LE-018).
3. **Auswahl nach Persönlichkeit, Geld, Beziehung.** Ein NPC geht in die Bar, in der seine Freunde sind.
4. **Orte haben Rollen in Fällen.** Zeugenorte, Tatorte, Treffpunkte; der Ort liefert Gelegenheit (Ermittlung).
5. **Orte können schließen** (Modul 10 §5, N-LE-010).

---

## 4. Entwurf: Abweichungen vom Normalplan

N-LE-002 nennt Wochenende, Urlaub, Krankheit, Überstunden, Events, Wetter. Der Entwurf gliedert sie (💡):

| Abweichung | Auslöser | Wirkung auf den Plan |
|---|---|---|
| **Wochenende, Feiertag** | Kalender | anderer Rhythmus; Familie, Feste, Reisen |
| **Urlaub** | Planung, Saison | längere Abwesenheit; Hotel, Reise |
| **Krankheit** | Lage, Alter, Gesundheit | Arbeit entfällt; Arzt; Isolation |
| **Überstunden, Schichtwechsel** | Arbeitgeber | Zeit verschiebt sich |
| **Wetter** | Schnee, Sturm, Hitze | weniger Wege, Verkehr, Schadensfolgen |
| **Event** | Sport, Fest, Konzert, Unfall, Stromausfall | Anreise, Straßensperren, Menschenmengen (Ermittlung 26) |
| **Arbeitsverlust** | Entlassung | Alltag ändert sich, Geld knapp (Abschnitt 7) |
| **Familie** | Geburt, Tod, Besuch | andere Prioritäten |
| **Beziehung** | Date, Streit | Treffen, Abwesenheit |
| **Gefahr** | Verbrechen, Monster | Meidung, Flucht (Modul 15) |
| **Reparatur** | Schäden | Umleitung, Verspätung |
| **Zufall** | Panne, Verspätung | kurzfristige Änderung (deterministisch) |

**Regeln (Entwurf):**

1. **Abweichung ist Ereignis oder Zustand.** Sie hat eine Ursache (Kalender, Ereignis, Zustand) und endet (N-PS-016).
2. **Prioritäten.** Sicherheit vor Pflicht vor Freizeit, mit Persönlichkeitsverschiebung (Mutige bleiben).
3. **Reihenfolge.** Wenn mehrere Abweichungen zusammenfallen, gilt eine feste Rangfolge.
4. **Nachwirkungen.** Eine Abweichung kann den Plan dauerhaft ändern (neuer Job, neue Stammkneipe).

---

## 5. Entwurf: Entscheidungslogik

Für die Entscheidungen zwischen Plan, Gelegenheit und Gefahr braucht es eine klare, **testbare** Logik. Die Quellen legen fest: Wichtige NPCs sind vollständige Akteure mit StateTree-Verhalten und Gedächtnis (`M-PD` ‚NPCs and AI‘); StateTree organisiert, Gedächtnis liefert Wissen (`23#48A` Abschnitt 9). Der Entwurf (💡):

```text
Jeder Zeitschritt für Sim 0/1 (voller Actor) oder Ereignisschritt für Sim 2/3:
  1. Zustand lesen:  Ort, Zeit, Bedürfnis-Treiber (Schlaf, Geld, Soziales), Verletzung, Rausch
  2. Auslöser prüfen: Ereignis in Wahrnehmung? Anruf? Gefahr?     -- Modul 06
  3. Verpflichtungen: Anker und Commitments  (hartes Ziel)
  4. Ziele abwägen:  eigene Ziele (Karriere, Rache, Beziehung)    -- Modul 11, 14
  5. Gelegenheit:    verfügbare Optionen am Ort, Kosten, Nutzen
  6. Persönlichkeit: Gewichtung, Risikobereitschaft, Gewohnheit
  7. Wahl:           nächste Aktivität  (deterministischer Seed)
```

**Bedürfnis-Treiber** sind hier *nicht* Tamagotchi-Balken: Sie sind Gründe für Planauswahl (Müdigkeit → Schlaf, Geldmangel → Arbeit oder Kredit, Einsamkeit → Treffen). Der Spieler sieht keine Balken (N-LT-012). Ob NPCs Bedürfnisse im Sinne eines Needs-Systems haben, entscheiden die Quellen nicht (❓ Q-N13).

**Eigenschaften der Logik:**

| Eigenschaft | Bedeutung |
|---|---|
| **Hierarchisch** | grobe Zielwahl getrennt von feiner Handlung (StateTree, `M-PD`) |
| **Erklärbar** | zu jeder Handlung gibt es einen Grund (Debug, Proposal 0010 §4 Punkt 8) |
| **Deterministisch** | Zufall über Seeds (N-LE-015) |
| **Konsistent** | NPC kann nicht an zwei Orten sein (N-LE-014) |
| **Unterbrechbar** | Ereignisse können Pläne stören; wiederaufnehmbar |
| **Sparsam** | Sim 2/3 nutzen Pläne, nicht jede Handlung |

---

## 6. Entwurf: Reisen und Mobilität

**Quellen:** NPCs reisen (Wochenendtrips, Urlaub, Besuche; N-LE-006), haben eigene Fahrzeuge (N-ID-005), Reisen kosten auch im Hintergrund Zeit (N-LE-014). Verkehrsregeln und Fahrer stehen in [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) und [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md).

| Aspekt | Entwurf |
|---|---|
| **Verkehrsmittel** | Auto (eigenes), Motorrad, Fahrrad, zu Fuß, Bus, Bahn, Flugzeug, Taxi, Fahrdienst |
| **Wahl** | nach Entfernung, Geld, Besitz, Wetter, Gewohnheit |
| **Dauer** | Reisezeit folgt Strecke und Verkehr (Welt 16), auch im Hintergrund |
| **Fernreisen** | NPC verlässt die Region; ist für die Dauer abwesend (`TRAVEL`, dann `AWAY`) |
| **Besuche** | Familie, Freunde, Dates; Gastgeber und Besucher tauschen Orte |
| **Urlaub** | Hotels, Ferienorte, Skigebiete, Seen (Tourismus) |
| **Pendeln** | tägliche Fahrt; Staus, Ausfälle, Verspätung |
| **Verlust des Fahrzeugs** | Panne, Unfall, Diebstahl ändert den Plan |
| **Verkehr als Wirkung** | Ferienverkehr, Pendlerspitzen, Event-Verkehr (`23#1056A` Fragen 88–89) |

**Regel:** Wer reist, ist **abwesend**; seine Rolle am Heimatort ruht (Arbeit, Familie). Telefon und Nachricht bleiben (Modul 07).

---

## 7. Entwurf: Arbeit, Einkommen und Betriebe

Die Quellen verlangen Arbeitsplätze, Einkommen (abstrakt), Wechsel, Einstellung und Entlassung, Betreiberwechsel (N-LE-003, 007, 008, 010). Der Entwurf (💡):

| Baustein | Beschreibung |
|---|---|
| **Betrieb** | Unternehmen mit Standort, Branche, Größe, Besitzer, Mitarbeitern, Öffnungszeiten, Finanzlage |
| **Stelle** | Rolle mit Qualifikationen, Schicht, Lohn |
| **Einstellung, Entlassung** | Folge von Betriebslage, Leistung, Beziehungen, Ereignissen |
| **Beförderung, Karriere** | Folge von Leistung, Zeit, Beziehungen |
| **Betreiberwechsel** | Erbe, Verkauf, Insolvenz; Ort bleibt, Besitzer wechselt (Modul 10 §5) |
| **Branchen** | Handel, Gastronomie, Industrie, Handwerk, Verkehr, Gesundheit, Sicherheit, Behörden, Tourismus, Unterhaltung, Landwirtschaft, Bau |
| **Schwarzmarkt** | illegale Betriebe als Fraktionsvermögen (Modul 16, 17) |
| **Wirtschaftslage** | Aufschwung/Abschwung beeinflusst Beschäftigung (Welt 12) |
| **Ereignisfolgen** | Schäden, Gerüchte, Monsterfälle beeinflussen Betriebe (`23#1058A`, Tourismus) |

**Wichtig:** Arbeit ist Quelle von **Zeitplänen, Kreisen und Rollen**, nicht ein Wirtschaftsspiel. Die Quellen schließen tiefe Wirtschaftsmodelle als Pflichtmechanik aus (`23#1066A` Abschnitt ‚Was ich NICHT jetzt hinzufügen würde‘, Vorschlag des Assistenten; eigene Unternehmen des Spielers sind P3: `23#1090A`).

---

## 8. Entwurf: Hintergrundzustände und Materialisierung

Die Quellen geben ein Beispiel (Assistent in `23#1058A`): Statt der exakten Gehbewegung genügt ein Zustandseintrag wie

```text
NPC_193822
STATE = AT_WORK
LOCATION = FACTORY_A
SHIFT_END = 17:00
```

und beim Annähern wird daraus wieder ein vollständiger NPC. Das Beispiel ist eine Skizze (💡), aber die Richtung ist beschlossen (N-LE-013, N-LE-014).

### 8.1 Zustände

| Zustand | Bedeutung | Mindestdaten |
|---|---|---|
| `HOME` | zuhause | Haushalt, Raum (grob), Zeit |
| `AT_WORK` | am Arbeitsplatz | Betrieb, Schicht, Ende |
| `TRAVEL` | unterwegs | Ziel, Ankunft, Verkehrsmittel |
| `LEISURE` | in Freizeit | Ort, Aktivität, Zeit |
| `SHOPPING`, `EATING` | Versorgung | Ort, Dauer |
| `SLEEP` | schlafend | Ort, Dauer |
| `AWAY` | verreist | Ziel, Rückkehr |
| `HOSPITAL`, `CAPTIVE`, `MISSING` | Sonderlage | siehe Lebenszustand (Modul 03 §7) |
| `EVENT` | in einem Ereignis | Ort, Rolle |
| `FACTION` | in Gruppenaufgabe | Gruppe, Auftrag |

### 8.2 Materialisierung

Wenn ein NPC aus Zustand in volle Simulation geht, muss er **konsistent** erscheinen (N-LE-014):

1. **Wo ist er?** Ort und Raum aus Zustand, Zeitplan und Ereignissen. `AT_WORK` heißt: am Arbeitsplatz, nicht am Eingang.
2. **Was tut er?** Aktivität passend zur Phase der Routine.
3. **Wie fühlt er sich?** Schnelle Zustände aus Plan und letzten Ereignissen (Modul 04 §4).
4. **Mit wem?** Kollegen, Familie, Begleiter aus dem Plan.
5. **Was trägt er?** Outfit nach Wetter, Rolle, Anlass (N-LE-012).
6. **Mit welchem Fahrzeug?** Eigenes Fahrzeug an einem plausiblen Ort (N-ID-005).
7. **Was weiß er?** Gedächtnis, Gerüchte, Informationsstand (Modul 05, 07).
8. **Mit welchem Gesicht, Körper, welcher Narbe?** Datensatz (Modul 03).
9. **Unabhängig vom Betrachter.** Die Materialisierung ist reproduzierbar (N-EN-008).

### 8.3 Dematerialisierung

Beim Verlassen der Nähe wird der Zustand **zurückgeschrieben**: Position, Aktivität, Rest der Aufgabe, Zustand. Wichtige Ergebnisse (Gespräche, Verletzungen, Zusagen) bleiben als Ereignisse (Modul 21).

### 8.4 Menschenmengen

Menschenmengen (Stufe D) entstehen aus dem **Ortszustand** (Stadion, Festival, Downtown). Nach einem Spiel strömen Zuschauer zu Parkplatz, U-Bahn, Bars (`23#1058A` ‚Crowd Simulation‘). Die Menge ist Ergebnis von Plänen der Besucher und Tourismus, nicht Zufall.

---

## 9. Entwurf: Eigenständige Lebensentscheidungen

Anforderung F (N-LT-006) verlangt, dass wichtige NPCs sich ohne Spielerkontakt entwickeln: Beziehungen ändern sich, sie ziehen um, arbeiten, treten Organisationen bei oder verlassen sie. Der Entwurf (💡) fasst das als **Lebensentscheidungen** mit Auslösern:

| Entscheidung | Auslöser (Beispiele) | Folge |
|---|---|---|
| **Job wechseln** | Konflikt, Angebot, Kündigung, Karriere | neuer Arbeitsort, Einkommen, Kreise |
| **Umziehen** | Job, Einkommen, Beziehung, Stadtentwicklung, Gefahr | neuer Haushalt, Weg |
| **Beziehung eingehen oder beenden** | Begegnung, Zeit, Streit | Haushalt, Kreise (Modul 08, 09) |
| **Heiraten, zusammenziehen** | Beziehung | Haushaltsfusion |
| **Gruppe beitreten oder verlassen** | Interesse, Not, Überzeugung, Druck | Rang, Pflichten (Modul 16) |
| **Straftat begehen** | Not, Gier, Gelegenheit, Gruppe | Polizei, Opfer (Ermittlung 25) |
| **Sucht beginnen/beenden** | Belastung, Umgebung | Zeit, Geld, Beziehungen |
| **Hilfe leisten** | Charakter, Beziehung | Gefallen, Ruf |
| **Rache suchen** | Groll, Verlust | Nemesis-Entwicklung (Modul 11) |
| **Fliehen, untertauchen** | Gefahr, Schuld | `MISSING` |
| **Auswandern, Region verlassen** | Not, Chance | Abwesenheit |
| **Hunter werden** | Ereignis, Erlebnis | Netz (Kreaturen 11) |
| **Aufgeben, Rückzug** | Alter, Krankheit | Ruhestand |

**Wahrscheinlichkeit.** Entscheidungen entstehen aus Persönlichkeit, Lage und Gelegenheit, mit Seeds (N-LE-015). Seltene Ereignisse (Umzug: „nicht zu oft“) werden **selten** gewählt.

**Konsistenz mit Autorenfiguren.** Geschützte und handgeschriebene NPCs folgen ihrem Autorenplan; systemische Entscheidungen dürfen die Story nicht stören (Modul 03 §8).

**Spieler als Auslöser und Zeuge.** Der Spieler kann diese Entscheidungen auslösen (Rache, Hilfe, Geld) oder bemerken (Besuch, Telefonat, Zeitung).

---

## 10. Entwurf: Sichtbarkeit für den Spieler

Das Leben der NPCs muss **erkennbar** sein, sonst ist es unsichtbare Simulation. Die Quellen betonen, dass die Welt lebendig wirken soll (N-LT-011, N-LT-022, N-LE-026). Hinweise für die Umsetzung (💡):

| Zeichen | Beispiel |
|---|---|
| **Beobachtung im Alltag** | dieselben Leute zur selben Zeit am selben Ort |
| **Veränderung über Zeit** | neue Frisur, Tattoo, Fahrzeug, Wohnort, Job (N-LT-022) |
| **Gespräche** | NPC erzählt vom Wochenende, Umzug, Krankheit (Modul 07) |
| **Telefonate, Nachrichten** | NPCs melden sich (Kommunikationsstil, Modul 04 §7) |
| **Zeitungen, Social Media** | Ereignisse im Leben bekannter Personen |
| **Einladungen** | Feiern, Dates, Treffen |
| **Abwesenheit** | der Verkäufer ist im Urlaub; der Laden hat einen Vertretungs-Mitarbeiter |
| **Folgen von Entscheidungen** | geschlossene Geschäfte, neue Besitzer |
| **Zeitstempel im Archiv** | Spielerwissen über Personen mit Datum (Ermittlung 04) |

**Erreichbarkeit.** Wichtige Kontakte des Spielers (Stufe A/B) sollen zu erwartbaren Zeiten erreichbar sein (Anruf, Besuch), sonst weiß er nicht, dass das Leben weiterging (Modul 09 §8, Punkt 6).

**Ermittlung.** Der Spieler kann Alibis prüfen und Tagesabläufe rekonstruieren (Ermittlung 12 §8). Die Simulation muss dafür die tatsächlichen Aufenthalte wichtiger Personen konsistent halten.

---

## 11. Sonderfälle

1. **Plötzlicher Todesfall im Hintergrund.** Ein NPC stirbt, während der Spieler weit weg ist; die Welt reagiert (Familie, Betrieb); der Spieler erfährt es später (Modul 07).
2. **Zwei Spieler im Koop in verschiedenen Orten.** Beide lösen Materialisierungen aus; der Host-Zustand bleibt konsistent (Modul 23).
3. **Schnelle Reise des Spielers (Flug, Teleport durch Story).** Hintergrund holt Ereignisse nach, bevor der Spieler ankommt (Modul 22).
4. **NPC gleichzeitig in zwei Zeitplänen (Doppelrolle).** Konflikt wird deterministisch entschieden (Rangfolge).
5. **NPC mit ungewöhnlichem Leben (Nachtschwärmer, Wanderarbeiter, Hunter).** Eigene Vorlagen.
6. **Schichtarbeit, 24-Stunden-Betrieb.** Mehrere Personen decken einen Betrieb; Vertretungen.
7. **Naturkatastrophen.** Pläne brechen; Evakuierung, Rettung (Ermittlung 26).
8. **NPC wird vom Spieler beschäftigt oder entführt.** Plan wird durch Spielerhandlung überlagert; die Welt läuft weiter (Vermisstenfälle).
9. **NPC im Ausland, nicht erreichbar.** `AWAY` mit Kontaktgrenzen.
10. **Gewohnheitsänderung nach Trauma.** Orte werden gemieden (N-WA-020).
11. **Tourist.** Kein fester Wohnsitz in der Region; Hotel; Saison.
12. **Obdachloser.** Plan aus Stammplätzen, Hilfsangeboten (Notunterkünfte, Suppenküche), Polizeikontakt.
13. **Haustier.** Ruft Tierarztbesuche, Gassigehen als Planbausteine hervor.
14. **Mod-Berufe.** Mods registrieren neue Vorlagen und Branchen (Modul 23).

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-241 | Ein NPC befindet sich zu jeder Weltzeit an genau einem Ort oder unterwegs | N-LE-014 |
| N-EN-242 | Ein Weg zwischen zwei Orten kostet Weltzeit, auch im Hintergrund | N-LE-014 |
| N-EN-243 | Der Zeitplan eines NPC ist bei gleichem Weltzustand und gleichem Seed reproduzierbar | N-LE-015 |
| N-EN-244 | Eine Abweichung vom Normalplan hat eine Ursache und ein Ende | N-LE-002 |
| N-EN-245 | Ein Arbeitsplatz-NPC erscheint zu den Schichtzeiten am Arbeitsplatz oder hat eine dokumentierte Abweichung | N-LE-003 |
| N-EN-246 | Eine Materialisierung ist mit der letzten Zustandsaufzeichnung vereinbar | N-LE-013 |
| N-EN-247 | Beim Betreten eines Gebiets sind Straßen entsprechend der Tageszeit belebt | N-LE-026 |
| N-EN-248 | Ein NPC reist nur mit einem verfügbaren Verkehrsmittel | Abschnitt 6 |
| N-EN-249 | Wer verreist ist, ist an seinem Heimatort abwesend | Abschnitt 6 |
| N-EN-250 | Ein Umzug ist selten und hat Ursachen | N-LE-009 |
| N-EN-251 | Der tatsächliche Aufenthalt wichtiger Personen ist für Alibi-Prüfungen abrufbar | Ermittlung 12 §8 |
| N-EN-252 | Ein Betrieb hat Mitarbeiter, die zu seinen Öffnungszeiten arbeiten, oder einen dokumentierten Ausfall | N-LE-010 |
| N-EN-253 | Feiertage verändern Öffnungszeiten und Routinen deterministisch | N-LE-018 |
| N-EN-254 | Begleiter und Crew haben Abläufe außerhalb der Anwesenheit des Spielers | N-LE-020, N-LE-022 |
| N-EN-255 | Die Hintergrundsimulation verbraucht nur Weltzeit | N-LE-027 |

---

## 13. Was fehlt

- Keine Zahlen für Schichtzeiten, Pendlerströme, Besuchsfrequenzen, Entscheidungswahrscheinlichkeiten.
- Offen: ob NPCs Bedürfnisse im Needs-Sinn haben (Q-N13).
- Offen: Detailtiefe der Wirtschaft (Betriebe, Lohn, Preise) jenseits abstrakter Einkommensklassen (Welt 12).
- Offen: Häufigkeit der Hunter-Netzwerkaktivität und ihre Sichtbarkeit.
- Offen: die Taktung der Hintergrundsimulation (Modul 22).
