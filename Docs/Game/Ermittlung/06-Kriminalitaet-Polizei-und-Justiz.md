# Ermittlung 06: Kriminalität, Polizei und Justiz

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt das **Rechtssystem der Spielwelt** als Planungsrichtung. Die Quellen nennen **keine** Straftatenliste, keine Schwellen, keine Strafmaße. Sensible Themen (schwere Gewalt- und Sexualstraftaten) stehen hier nur auf der Systemebene, die der Master selbst setzt; die Detailregeln stehen in [Adult 09](../Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Überblick: [Gameplay-Systeme §10](../Gameplay-Systeme.md#10-recht-polizei-und-beweise). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Grundsatz: Gesetz folgt tatsächlicher Information](#1-grundsatz-gesetz-folgt-tatsächlicher-information)
2. [Kriminalität unabhängig vom Spieler](#2-kriminalität-unabhängig-vom-spieler)
3. [Organisierte Gruppen und Gebiete](#3-organisierte-gruppen-und-gebiete)
4. [Polizei, Behörden und Zuständigkeiten](#4-polizei-behörden-und-zuständigkeiten)
5. [Fahndung und Beweislage](#5-fahndung-und-beweislage)
6. [Das Strafsystem](#6-das-strafsystem)
7. [Festnahme, Haft und Gefängnis](#7-festnahme-haft-und-gefängnis)
8. [Selbstverteidigung und NPC-Reaktionen](#8-selbstverteidigung-und-npc-reaktionen)
9. [Rettung und Notfälle](#9-rettung-und-notfälle)
10. [Polizei und das Übernatürliche](#10-polizei-und-das-übernatürliche)
11. [Geld, Pfandläden und Hehler](#11-geld-pfandläden-und-hehler)
12. [Gesellschaftliche Prägung der Region](#12-gesellschaftliche-prägung-der-region)
13. [Ergänzungen aus der Quellenauswertung](#13-ergänzungen-aus-der-quellenauswertung)
14. [Ergänzung: Rechtslage je Bundesstaat](#14-ergänzung-rechtslage-je-bundesstaat)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Grundsatz: Gesetz folgt tatsächlicher Information

📋 `M-PD` ‚Cash / law / evidence‘: ‚Modern law system tracks actual information and evidence confidence across jurisdictions rather than omniscient wanted logic.‘ Beweisquellen: Zeugen, Überwachungskameras, Kleidung, Gesicht, Fahrzeuge und Kennzeichen, Waffen und Ballistik, Fingerabdrücke und DNA wo passend, digitale Beweise (Komplizen sind im Überblick ergänzt). Dazu kommen Meldelatenz beim Notruf, Verarbeitungs- und Verteilungsverzögerungen und ein Vertrauensmodell der Verdächtigung (`M-B39`, nur als Themenliste).

Der Entwickler bestätigte die Richtung ausdrücklich (`23#1057E`, Fragen 59 und 67 in `23#1056A`): Die Polizei weiß nicht „magisch alles“; kein sofortiger Stern, nur weil irgendwo ohne Zeugen etwas passiert; Beweise, so der Assistent, mindestens „abstrakt glaubwürdig“, nicht als vollständiger Crime-Simulator (Antwort zu Frage 67: „ja kann man auch machen“ – Sinn der Antwort nicht ganz eindeutig, siehe [Modul 21](21-Konflikte-und-offene-Fragen.md)).

---

## 2. Kriminalität unabhängig vom Spieler

Block 8F (Fragen 46–56 in `23#1056A`; Antworten `23#1057E`):

| Aussage | Status | Antwort |
|---|:---:|---|
| Es gibt echte Straftaten **unabhängig vom Spieler** (Diebstahl, Einbruch, Raub, Autodiebstahl, Vandalismus, Körperverletzung, Drogenhandel, Gangkonflikte, organisierte Kriminalität) | 📋 | Frage 46: „ja schon, sehr komplex“ |
| Bestimmte Viertel haben andere Arten und Häufigkeiten von Kriminalität, **keine** simplen „rote Crime Zones“ | 📋 | Frage 47: „manche viertel sind gefährlicher“ (siehe unten) |
| Der Spieler kann Zeuge werden; er darf eingreifen, ignorieren, die Polizei rufen oder schweigen | 📋 | Frage 48: „ja kann er, aber er muss nicht petzen“ |
| Ein Täter kann Zeugen mit Schweigegeld ruhigstellen oder bedrohen | 📋 | Frage 48 (siehe [Modul 05 §6](05-Spuren-Tracking-Forensik-und-Zeugen.md#6-zeugen-und-gerüchte)) |
| Nach größeren NPC-Verbrechen können Orte zeitweise abgesperrt sein | 📋 | Frage 50: „ja“ |
| Kriminalität ändert sich langfristig mit Stadtentwicklung, Wirtschaft und Polizeipräsenz | 📋 | Frage 51: „ja“ |
| Der Spieler **kann** normale Kriminalität begehen (Autodiebstahl, Einbruch); **Hunter bleibt der Kern**, das Spiel wird kein „GTA-Klon“ | 📋 | Fragen 55–56: „ja genau, wenn man will kann man auch ein richtig gesuchter charakter werden“ / „ja genau, aber es gibt auch andere wege, aber hauptstory ist hunter“ |

**Modellierungsregel des Assistenten (💡, `23#1058A`):** Unterschiede zwischen Vierteln sollen über **Faktoren** wie Armut, Arbeitslosigkeit, organisierte Kriminalität, Drogenmärkte, Infrastruktur, Polizeipräsenz, Stadtentwicklung und soziale Stabilität modelliert werden, nicht über die Herkunft der Bewohner als Ursache. Der Entwickler nannte in Frage 47 eine demografische Zuschreibung als Vergleich mit dem realen Land (Wortlaut `23#1057E` Punkt 47) und nahm zur Regel des Assistenten nicht ausdrücklich Stellung. **Die Regel ist daher nicht als Entscheidung geführt** (🧭, offen; K-E3 in [Modul 21](21-Konflikte-und-offene-Fragen.md)).

---

## 3. Organisierte Gruppen und Gebiete

| Aussage | Status | Quelle |
|---|:---:|---|
| Mehrere eigene kriminelle Organisationen statt generischer „Gang NPCs“; unter ihnen auch extremistische Gruppen; sie sind Teil der Welt und möglicher Konflikte, nicht etwas, das das Spiel verherrlichen soll | 📋 Richtung, Gruppennamen offen | Frage 52, Antwort „ja haben wir schon alles aufgestellt“ (`23#1057E`); Assistent `23#1058A` |
| Gebiete der Gruppen sind **nicht** standardmäßig bunt auf der Karte; wer selbst Mitglied ist, kann eine Einflussansicht (Gebiete, Geschäfte, Rivalen) nutzen und mehr Gebiet erobern | 📋 | Frage 53: „bunt kann es auf der karte sein, aber nur wenn man selber in einer dieser gangs ist“ |
| Illegale Geschäfte (Chop Shops, Untergrund-Casinos, illegale Rennen, Fight Clubs, Schmuggel) | 📋 | Frage 54 „ja“; Verweis [Welt 15](../World/15-Motorsport-und-Rennaktivitaeten.md) |
| Bestimmte Gruppen können wissen, wer der Spieler ist (kriminelle Reputation) | 📋 | Block 8E Frage 44 „ja“ |

---

## 4. Polizei, Behörden und Zuständigkeiten

Block 8G (Fragen 57–69; Antworten `23#1057E`):

| Aussage | Status | Quelle |
|---|:---:|---|
| Mehrere Behörden: City Police, County Sheriff, State Police/Highway Patrol, Rangers | 📋 Richtung | Frage 57 „ja sicherlich“ |
| Behörden haben **unterschiedliche geografische Zuständigkeiten** (lokal, Bezirk, Bundesstaat, Bund) | 📋 Richtung | Frage 58 „ja“; `M-PD` |
| Notruf: Zeugen können Polizei und Rettung rufen; **Meldelatenz** | 📋 | Frage 60 „ja“; `M-PD` |
| Verkehrskontrollen statt sofortiger Verfolgung; Eskalation hängt vom Verhalten des Spielers ab | 📋 | Fragen 63–64: „ja aber es kann beides sein, wenn man halt auf frischer tat ertappt wird kann das auch so sein“ / „ja“ |
| Kennzeichenabfrage durch die Polizei | 📋 | Frage 62 „ja“ |
| Polizei-Verfolgungen von NPC-Kriminellen | 📋 | Block 8K Frage 98 „ja“ |
| Spürhunde und Wind-/Geruchsmodell | 📋 Wunsch | `M-PD` ‚Police K9 and a reusable scent/wind model are desired.‘ |
| Die Polizeikarte (Minikarte) richtet sich nach der Schwierigkeit | 📋 Richtung | `M-PD`, `M-B39` |

Die Zahl der Behörden, Reviere, Streifen und die **Karte der Zuständigkeiten** fehlen (❓). Die Welt-Dokumentation behandelt die Bundesstaaten als Planungsmaß ([Welt 07](../World/07-Weitere-Staedte-und-Siedlungen.md)).

---

## 5. Fahndung und Beweislage

| Aussage | Status | Quelle |
|---|:---:|---|
| Täterbeschreibung (Kleidung, Fahrzeug, Kennzeichen, Richtung) kann Teil der Fahndung sein | 📋 | Frage 61 „ja sicher“ |
| **Schwere Fahndung bleibt bestehen**: Sie verschwindet nicht nach wenigen Sekunden, sondern bleibt „lange bestehen bis man kopfgeld bezahlt, oder die klage zurückgezogen oder man freigesprochen wird“ | 📋 | Frage 65 (`23#1057E`); `M-PD` |
| Landesweit gesucht werden ist möglich (wer dauerhaft Menschen ermordet oder die Polizei angreift) | 📋 | Frage 55; `23#1058A` |
| Spätere Ermittlungen durch Detectives bei wirklich schweren Delikten | 📋 | Frage 66 „ja“ |
| Kein GTA-Sternesystem | 📋 | `M-PD`; `23#1058A` („Kein magisches GTA-Sternesystem“) |
| Wenn der Spieler **verdächtigt** wird, kann es zu einem Verhör kommen | 📋 | Frage 59: „ja genau, wenn man verdächtigt wird kann es zu einem verhör kommen und so weiter“ |
| Wichtige abgelegte Waffen oder Gegenstände bleiben physisch und können Beweise werden; Durchsuchung, Beschlagnahme, Fahrzeugdurchsuchung nach Regeln; Beweiskette und Aufräumen | 📋 Wunsch | `M-PD`; `M-B39` (nur Themenliste) |
| Anti-Exploit-Regeln und deterministische Testfälle | 💡 | `M-B39` (‚deterministic test cases and anti-exploit rules‘) |

---

## 6. Das Strafsystem

📋 Rahmen aus `M-PD` und `35#533A` (der Entwickler gab in `35#532E` die Richtung vor: „das strafsystem soll realistisch sein und logisch“):

```text
Tat -> Schweregrad -> Vorsatz oder Fahrlässigkeit -> Opfer -> Zeugen und Beweise
-> Notwehr/Nothilfe -> Vorstrafen -> Festnahme -> Staatsanwaltschaft
-> Prozess oder Absprache -> Urteil -> Haft, Bewährung, Geldstrafe oder Freispruch
```

Das ist die Kette des Assistenten (`35#533A`); `M-PD` hat dieselbe Kette übernommen (📋 als Struktur). Beispielhaft nannte der Assistent für die vorsätzliche Tötung der eigenen Kinder in der fiktiven US-Welt eine lebenslange Haft ohne realistische Aussicht auf Entlassung (`35#533A`); der Entwickler hatte gesagt, dass man dann „für immer weggesperrt“ sei (`35#532E`). Konkrete Strafmaße sind **nicht** festgelegt (❓).

---

## 7. Festnahme, Haft und Gefängnis

| Aussage | Status | Quelle |
|---|:---:|---|
| **Festnahme ist kein Game Over**: Verhör, Geldstrafe, Kaution, Gericht, Gefängnis, Zeitsprung | 📋 | Frage 68: „ja genau, game over soll es nicht geben, das wäre bei so einem spiel zu brutal“; `23#1058A` („gelockt“) |
| Aufgeben und Festnahme sind echte Alternativen zur tödlichen polizeilichen Lösung | 📋 | `M-PD` |
| Anwälte und Kaution können **später** ausgebaut werden | 📋 | Frage 69: „ja kann man später machen“ |
| Das Gefängnis ist eine eigene soziale Simulation; Insassen können wissen, wofür jemand sitzt; was passiert, hängt von Personal, Sicherheitsstufe, Ruf, Beziehungen, Gangs und Situation ab, es ist **kein garantiertes Script** | 📋 Richtung | `35#533A`; [Gameplay-Systeme §10](../Gameplay-Systeme.md#10-recht-polizei-und-beweise) (Gefängnis als eigene soziale Simulation) |
| Verhalten im Gefängnis kann soziale Isolation, Bedrohung, Erpressung, Schutzsuche oder Gangkonflikte auslösen | 💡 | `35#533A` |

---

## 8. Selbstverteidigung und NPC-Reaktionen

| Aussage | Status | Quelle |
|---|:---:|---|
| Das System bewertet **Handlungen**, nicht das Geschlecht; **Selbstverteidigung ist unabhängig vom Geschlecht** | 📋 | `35#533A`; Überblick in Gameplay-Systeme §10; Entwickler `35#532E` („wenn man sich verteidigt gegen eine frau wird man nicht gehated“) |
| NPC-Urteile hängen vom beobachteten Beginn des Konflikts, vorhandenen Beweisen, Gewaltgrad, der Beziehung zu den Beteiligten, eigenen Vorurteilen, Medienberichten und Gerüchten ab | 💡 | `35#533A` |
| Beobachten NPCs ein schweres Verbrechen, können sie die Polizei rufen, eingreifen, festhalten, fliehen, angreifen oder (bei besonders gewalttätigen Gruppen) stark eskalieren, **je nach Persönlichkeit, Fraktion und Moral** | 📋 Richtung | `35#533A`; Gameplay-Systeme §10 |
| Das ist ein **Faction/Morality/Aggression-System**, keine feste Regel wie ‚Gruppe X tötet immer‘ | 💡, vom Entwickler nicht widersprochen | `35#533A`; zum Entwickler siehe `35#532E` |
| Noch beim Eingreifen bewertet das Rechtssystem Notwehr/Nothilfe, Verhältnismäßigkeit und Beweislage | 📋 | `35#533A` |
| Schwere Straftaten sind Teil der Kriminalitäts- und Justizsimulation und laufen **nur** über Konsequenzsysteme (Zeugen, Spuren, Ermittlung, Festnahme, Prozess, Haft, Ruf); es gibt dafür **keine eigene Spielmechanik** | 📋 | `35#533A`; [Adult 09](../Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md) |

---

## 9. Rettung und Notfälle

Block 8H (Fragen 70–76), vom Entwickler mit „70–79 ja“ als Gruppe bestätigt (`23#1057E`: einzeln „70 ja sicher“ bis „79 ja, sicher“):

- **Polizei, Feuerwehr und Rettungsdienst** sind Teil derselben Simulation (Verkehrsunfall: Notruf, Einsatzfahrzeuge, Straßensperre, Abschleppdienst, Aufräumen/Reparatur; `23#1058A` Beispiel, 💡).
- EMS holt verletzte NPCs ab; Verletzte werden in Krankenhäuser gebracht.
- Bergrettung (Mountain Region), Wasserrettung (Küste, See) und **Suchaktionen**; vermisste Wanderer müssen nicht übernatürlich sein.
- Dynamische Ereignisse mit Folgen (gesperrte Straßen, Reparaturen, Nachrichten): Details und Stufen in [Modul 07](07-Detektivlaufbahn-und-organische-Faelle.md#6-dynamische-weltereignisse-und-soziale-medien).

Status: 📋 als Richtung; die Anzahl der Einsatzkräfte und Wachen fehlt (❓).

---

## 10. Polizei und das Übernatürliche

| Aussage | Status | Quelle |
|---|:---:|---|
| Die Polizei erklärt Vorfälle zunächst **normal** (Wildtier, Unfall, Mord), solange sie das Übernatürliche nicht kennt | 📋 | Block 8N Frage 116, Antwort „ja so wie in der serie“ (`23#1057E`) |
| Die meisten Polizisten kennen die Wahrheit nicht; **kleine geheime Behörden-, Polizei-, Bundes- oder Militär-Forschungseinheiten** kennen Teile davon | 📋 | Block 9 Frage 21, Antwort „21 ja“ (`23#1075E`); [Spielvision §10](../Spielvision.md#10-jäger-organisationen-und-fraktionen) |
| NPCs reagieren unterschiedlich auf Übernatürliches (Unglaube, Angst, Religion, Neugier, Verdrängung) | 📋 | Frage 117, Antwort „genau“ |
| Menschen meiden nach mehreren Vorfällen einen Wald oder eine Straße; Gerüchte verbreiten sich | 📋 | Fragen 114–115, Antworten „ja sicher“ und „ja genau“ |
| Ein Fall, in dem die Polizei den Spieler für den Täter hält, während er ein Monsterproblem löst | 💡 | `23#55A` Fall F |

---

## 11. Geld, Pfandläden und Hehler

📋 `M-PD`: Nur Bargeld, das der Spieler tatsächlich bei sich trägt, ist bei üblichen Bargeldverlusten gefährdet; Geld im Auto, Safe, Hauptquartier oder auf der Bank geht nicht automatisch verloren. Pfand-, Second-Hand- und Hehlerläden mit wechselndem Bestand führen Alltägliches und gelegentlich Jäger-Relevantes.

---

## 12. Gesellschaftliche Prägung der Region

Der Entwickler wünschte eine eher **konservative** Welt und beschrieb, wie Randgruppen darin behandelt werden (`35#532E`, `35#534E`; Wortlaut dort nachlesbar). Der Assistent riet davon ab, eine politische Seite als pauschal „irre“ zu behandeln und Randgruppen verschiedener Art nur als konkrete Personen und Fraktionen zu simulieren (`35#533A`); der Entwickler fügte hinzu, dass er nicht meine, eine Seite sei schlechter (`35#534E`).

Status: **🧭 offen.** Festgelegt ist nur, dass NPCs **individuell** reagieren und die Region bestimmte Mehrheitswerte haben kann. Der Master enthält keine Beschreibung der gesellschaftlichen Prägung. Siehe [Modul 21](21-Konflikte-und-offene-Fragen.md), K-E4.

---

## 13. Ergänzungen aus der Quellenauswertung

### Weltlogik statt Gruppenzugehörigkeit (15.09.2026)

Auf den Vorschlag, die Welt „daten-/weltbasiert statt ideologisch“ zu bauen und die Region mit einer traditionellen Mehrheitskultur zu versehen (`35#535A`), antwortete der Entwickler: „Nein die welt soll hauptsächlich logisch denken und nicht emotional wenn du das verstehst. Du kannst dir ja anschauen was abgeht in der welt“ (`35#536E`). Der Assistent fasste das als **World-Simulation-Lock** zusammen (`35#537A`), und der Entwickler bestätigte: „Ja genau“ (`35#538E`).

| Inhalt des Locks (`35#537A`) | Status |
|---|:---:|
| Die Welt urteilt nach **Ursache, Kontext, Beweisen und Verhältnismäßigkeit**, nicht nach reflexhaften emotionalen Reaktionen oder Gruppenzugehörigkeit | 📋 |
| **Tat vor Identität**: Die Tat zählt mehr als Geschlecht, Herkunft oder politische Gruppe | 📋 |
| **Kontext zählt**: Angriff, Notwehr, Nothilfe, Unfall, Vorsatz, Provokation, Zeugen und Beweise werden unterschieden | 📋 |
| **Verhältnismäßigkeit zählt**: Verteidigung ist etwas anderes als weiteres Zusammenschlagen danach | 📋 |
| Polizei und Gericht urteilen anhand des tatsächlichen Falls, nicht nach einem einfachen Moralwert; **Gerüchte sind keine Fakten**, vernünftige Personen und Institutionen gewichten Beweise stärker | 📋 |
| Emotionale oder ideologische NPCs gibt es trotzdem, aber als **individuelle Eigenschaft**, nicht als Grundlogik der Welt | 📋 |
| Prüfreihenfolge im Crime-System: Was ist passiert, warum, wer begann es, welche Gefahr bestand, welche Kraft wurde eingesetzt, was wussten die Beteiligten, welche Beweise gibt es, was sagt das Gesetz; **erst danach** die Reaktion | 💡 (bestätigt als Linie durch `35#538E`) |

**Wirkung auf K-E4 (gesellschaftliche Prägung der Region):** Die frühere Aussage über eine konservative Welt (`35#532E`, `35#534E`) bleibt stehen; sie wird aber durch `35#536E` und das Lock **eingehegt**: Die Grundlogik ist Ursache und Beweis, nicht Ideologie. Wie stark die regionale Mehrheitskultur ausgeprägt ist, bleibt offen. Der Assistent kündigte an, für Gesellschaft, Politik, Medien, Polizei und Justiz später reale Daten zu recherchieren (`35#537A`); das ist nicht geschehen.

### Keine künstliche Verbrechensliste (15.09.2026)

Der Entwickler (`35#530E`): Es soll „keine Einschränkung geben bei Verbrechen“; dass etwas möglich ist, „wird einem aber nicht gesagt“, man muss es selbst herausfinden. Der Assistent (`35#531A`): Das Crime-System soll **keine künstliche Liste** „nur diese Verbrechen existieren“ für die Weltwirkung haben; schwere Delikte laufen über Straftat, Zeugen und Beweise, Trauma und Erinnerung, Fahndung, Ermittlung, Festnahme, Prozess, Haft, Beziehungen und Ruf; das Crime-System darf Dinge ermöglichen, die das Spiel nicht als Menüpunkt bewirbt. Status: 📋 (Wortlaut des Entwicklers), 💡 (Ausformulierung). **Wirkung auf Q-E20:** Der Straftatenkatalog bleibt offen, aber die Vorgabe lautet **systemisch offen statt abschließend**; Details zu schweren Delikten in [Adult 09](../Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md).

### Weltgestaltung: Polizei, Gefängnisse, Rettung

Block 4 (03.09.2026): „Sollen wir realistisch verteilte Police Stations, Fire Stations und EMS-Standorte haben?“ (`23#1022A` Frage 39, „Kein Regierungsfokus, aber für Weltreaktionen wichtig“) mit der Antwort „ok“; Frage 40: „Gefängnis: Ein größeres Gefängnis und kleinere Jails?“, Antwort „ja sicher“ (`23#1023E` Punkte 39 und 40). Status: 📋. **Wirkung auf Q-E61:** Die **Art** (ein größeres Gefängnis plus kleinere Jails, daneben Polizeistationen, Feuerwachen, Rettungsstandorte) ist bestätigt; die **Zahl** und die Orte bleiben offen. Zur Frage, ob die Metropole auch ein Regierungs- und Verwaltungszentrum sein soll, antwortete der Entwickler „ok, aber kein fan von regierung im spiel“ (`23#1011E` Punkt 9; Frage in `23#1010A`). Das betrifft den Regierungsfokus des Spiels, nicht das Polizeisystem.

---

## 14. Ergänzung: Rechtslage je Bundesstaat

Beim Entwurf der Weltkarte äußerte der Entwickler (31.08.2026), die Welt solle mehrere Bundesstaaten simulieren und „kleinere gesetzänderungen“ enthalten: in einem Staat werde man gesucht, im anderen nicht (`23#719E`, 📋 Richtung). Er wünschte „5-10 bundesstaaten“ mit Unterregionen, damit Sammelobjekte besser sichtbar sind (`23#721E`); die Welt müsse nicht wie die USA sein, aber „nie nur eine große stadt ein bundesstaat“ (`23#723E`); später legte er sich für den Planungsstand auf 7 Bundesstaaten fest (`23#729E`, 🧭 Planungsstand der Karte, kein Geometrie-Lock). Für die Ermittlung folgt daraus als **Quellenbasis**, dass Fahndung und Rechtslage je Staat unterschiedlich sein können (Auslieferung, Anerkennung der Lizenz: [Modul 15](15-Gerichte-und-Verfahren.md), [Modul 24](24-Privatdetektiv-Laufbahn.md)). Der Umfang der Unterschiede ist ❓ (Q-E21, Q-E20). Die Zahl der Staaten ist ein **Kartenplanungswert**; gemäß Projektregeln ist die Weltgeometrie nicht festgelegt und wird hier nicht festgelegt.

---

## 15. Was fehlt
- **Straftatenkatalog**, Schweregrade, Festnahmeschwellen, Strafen, Kaution.
- **Zuständigkeitskarte** und Zahl der Behörden.
- Verdachts- und Beweismodell (Konfidenz), Überwachungsdichte, Verteilungsverzögerungen.
- Spürhund-, Durchsuchungs- und Beschlagnahmeregeln.
- Die Darstellung der **Polizeikarte** je Schwierigkeit.
- Anwälte, Kaution, Bewährung, Berufung (später).
- Verbindung zum Weltzustand: Wie Fahndung, Anklage und Haft gespeichert werden.

Siehe [Modul 21](21-Konflikte-und-offene-Fragen.md).
