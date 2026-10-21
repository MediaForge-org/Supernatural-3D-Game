# Adult 07: Charakteranpassung, Tattoos und Körpermodifikation

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Im Repository existiert nur ein kleines Aussehens-Fundament (siehe [Entscheidung 0005](../../Decisions/0005-character-appearance-foundation.md)); Tattoos, Piercings und Eingriffe sind Planung. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Der größte Teil dieses Themas ist **allgemeine Charaktergestaltung** und nicht auf den Erwachsenen-Bereich beschränkt. Er wurde in den Interviewrunden 14 (Körper, 14.09.) und 17 (Tattoos, Piercings, Körpermodifikation, 26.09.) geplant. Runde 17 wurde am 26.09. als **abgeschlossen** markiert und wird nur bei echten Lücken wieder geöffnet.

**Inhalt**

1. [Grundsatz: ein Charaktereditor](#1-grundsatz-ein-charaktereditor)
2. [Körper und Anatomie](#2-körper-und-anatomie)
3. [Tattoos](#3-tattoos)
4. [Verteilung in der Bevölkerung](#4-verteilung-in-der-bevölkerung)
5. [Piercings und weitere Körpermodifikation](#5-piercings-und-weitere-körpermodifikation)
6. [Kosmetische Eingriffe](#6-kosmetische-eingriffe)
7. [Studios, Kliniken und Internet](#7-studios-kliniken-und-internet)
8. [Technik und Produktion](#8-technik-und-produktion)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Grundsatz: ein Charaktereditor

📋 Es gibt **keinen zweiten Charaktergenerator**. Das Erwachsenen-Paket ergänzt denselben Editor nur um zusätzliche anatomische Detailoptionen für Erwachsene (Entscheidung vom 31.08.: kosmetische Anatomie liefert keinen Spielvorteil). Spielerfigur und NPCs nutzen dieselbe Pipeline; die Spielerfigur wird zu Spielbeginn erstellt, wichtige Story-Figuren sind handgefertigt, der Rest wird erzeugt ([Modul 05](05-NPCs-Beziehungen-Praeferenzen.md#7-generierung-bei-sehr-vielen-npcs)). Zu Kosten und Werkzeugen der Figurenerzeugung hat der Entwickler ausdrücklich keinen hohen Geldbetrag für ein einzelnes Modell eingeplant (12.09.).

Alles Gestaltbare folgt der Grundregel: **so realistisch und logisch wie in der echten Welt**, wenn nichts anderes festgelegt ist.

---

## 2. Körper und Anatomie

📋 (Runde 14, zusammengefasst als „14C“.) Der Körper ist ein persistenter, veränderlicher Zustand mit kontinuierlichen Parametern ([Modul 06](06-Koerper-Physik-Gesundheit.md#2-körpermodell)).

- Maße und Formen sind **ohne feste Stufen** und ohne künstliche Obergrenze kontinuierlich, nur physikalisch und anatomisch plausibel gekoppelt. Das gilt auch für große Maße; der Entwickler wünscht ausdrücklich auch **sehr große** Varianten (nicht nur zehn Größenstufen), die sich mit dem Gewicht verändern, solange Form, Position, Asymmetrie und Physik konsistent bleiben.
- Becken, Hüfte, Muskel- und Fettverteilung, Haltung, Asymmetrien und individuelle Anatomie sind getrennt einstellbar. Sport-Kleidung verändert die Physik (zum Beispiel stützende Wäsche).
- Kosmetische Eingriffe und Training können Form verändern ([Abschnitt 6](#6-kosmetische-eingriffe)).
- Intime Merkmale haben eine individuelle Vielfalt und können Gebrauchsspuren zeigen; das System behandelt sie wie jedes andere Körpermerkmal. Weitere Details liegen im Editor des Pakets und werden hier nicht beschrieben.
- Haare und Bart wachsen grenzenlos (zwei Meter wären theoretisch möglich); Frisuren folgen dem Haarwuchs (Entwickler, 11.09., 12.09.).
- Zusätzliche anatomische Detailoptionen gehören zum erweiterten Editor-Menü des Pakets, nicht zum Basisspiel.

---

## 3. Tattoos

Ziel des Entwicklers: Tattoos **extrem detailliert**, mit etwa 90 bis 95 Prozent des Planungsumfangs der Runde 17. 📋

- **Platzierung und Umfang:** an jeder Stelle des Körpers, einschließlich Gesicht, Hände, Füße, Hals und Augen (selten); von keinem bis zu praktisch 100 Prozent der Hautfläche (bis auf den letzten Quadratmillimeter). Einzelne Motive, Sleeves, Rückenstücke, zusammenhängende Ganzkörperkonzepte, Auffüllen freier Flächen, Mischung verschiedener Künstler und Stile.
- **Motive:** Der Spieler kann dem Künstler sagen, was er will, und der Künstler stellt es zusammen oder verwendet Vorlagen. Es gibt keine Einschränkung des Motivs (Entwickler: „wirklich alles soll tätowierbar sein“) und eine **Vorschau** am eigenen Körper (Größe, Platzierung, Wirkung neben anderen Tattoos) vor dem Stechen. Der Künstler macht die Vorlage; bei Nichtgefallen ändert der Spieler oder der Künstler. Ein importiertes oder eigenes Motiv wird vom Künstler interpretiert. Hohe Auflösung ist Pflicht (Entwickler: kein bloßes Auswählen weniger Motive). Auf 10 bis 20 Meter Entfernung sollen Tattoos noch klar erkennbar sein.
- **Handwerk:** Künstler haben **mehrdimensionale Fähigkeiten** (Linienführung, Schattierung, Realismus, Blackwork, Cover-ups, Narbenabdeckung); gute Künstler sind nicht in jedem Stil gut. Schlechte Künstler stechen schlechter als weltbekannte Profis, und die Preise unterscheiden sich entsprechend. Ein Tattoo kann von mehreren Künstlern nacheinander fertiggestellt werden, was sichtbar sein kann.
- **Dauer:** Große Stücke entstehen über **mehrere Sitzungen** und Wartezeiten (Entwickler: Wartezeit ein bis zwei Wochen Spielzeit, nicht länger). Die Sitzung kann als **Zeitraffer** erlebt werden, nicht in Echtzeit.
- **Lebenszyklus:** frisch → Heilung → stabil → Alterung/Verblassen → Auffrischung/Cover-up/Laser. Auffrischen früh nach Jahren statt nach zwei Wochen. Sonne beschleunigt das Verblassen. Haut verändert sich: Narben, Schwangerschaft (ein Bauchtattoo dehnt sich mit; ein Tattoo während der Schwangerschaft passt danach nicht mehr), Gewichtsänderung verzerrt Tattoos sichtbar. Tattoos können über Narben und alte Tattoos gestochen, entfernt (Laser, braucht Zeit) oder zu **Blackout** überdeckt werden; jedes Tattoo kennt sein Alter, seinen Künstler und seinen Zustand.
- **Persönlichkeit:** Jeder NPC oder der Spieler entscheidet selbst; Gründe reichen von Mode über Szene bis zur Sucht („Tattoo-Obsession“). Manche bereuen oder entfernen später. Ein Gefängnis-Tattoo ist weniger gut ausgeführt.
- **Szene und Symbolik:** Gruppen wie Biker, Gangs, Mafia oder Yakuza haben eigene Symbolsprachen; das bleibt an konkrete Fraktionen und Figuren gebunden und wird nicht zufällig verteilt. Der Entwickler nannte dabei auch extremistische Symbole bei bestimmten Gruppen. ❓ Die rechtliche Zulässigkeit solcher Symbole in der Zielregion und auf den Plattformen ist **ungeprüft** und vor einer Entscheidung abzuklären ([Modul 10](10-Produktion-Recht-Vertrieb.md#3-recht-und-plattformen)).

---

## 4. Verteilung in der Bevölkerung

🧭 Erste Baseline des Assistenten (26.09.), vom Entwickler mit „passt“ bestätigt; es sind **Spawn-/Populationsgewichte, keine harten Quoten**. Prozentwerte beziehen sich auf die erwachsene Bevölkerung.

| Gebiet | keine | leicht (1–10 %) | mittel (10–30 %) | stark (30–60 %) | sehr stark (60–90 %) | nahezu Vollkörper (90–100 %) |
|---|---:|---:|---:|---:|---:|---:|
| Metropole | ~45 % | ~30 % | ~14 % | ~7 % | ~3 % | ~1 % |
| Kleinere Städte | 55 % | 27 % | 11 % | 5 % | 1,6 % | 0,4 % |
| Land / Dörfer | 70 % | 21 % | 6 % | 2,3 % | 0,6 % | 0,1 % |

Modifikatoren: Alter (18 bis 30 deutlich höher, ältere Generationen niedriger), Szene und Beruf (Biker, Musik, Gang, Tattoo-Künstler stark erhöht). Wer extrem tätowiert ist, begegnet einem eher in Studios, auf Messen, in Clubs und Szene-Orten. Auf dem Land und bei älteren Menschen, die nie im Gefängnis waren, sind Tattoos selten; bei jungen Erwachsenen in der Stadt häufig. Tattoo-Quoten ändern sich über die Jahre mit Trends. Die Werte sind Richtwerte; ein Viertel kann ganz anders aussehen.

---

## 5. Piercings und weitere Körpermodifikation

📋

- **Piercings:** von keinem bis sehr vielen, an vielen anatomisch **plausiblen** Stellen (Ohr, Nase, Lippen, Zunge, Augenbrauen, Bauchnabel, Brustwarzen, äußere Genitalregion, gedehnte Ohrläppchen und weitere). Das System erzeugt nicht beliebige Stellen, nur weil dort Geometrie ist.
- **Schmuck ist getrennt vom Piercing:** Form, Größe, Material, Gewicht, Zustand; wechselbar. Schmuck reagiert auf die Physik.
- **Heilung:** Heilungszustände, Reizung, Verletzung, kleine Narben. Qualität hängt von Material, Verarbeitung, Größe, Hygiene, Kompetenz des Piercers, Allergien und Pflege ab. „Billig ≠ schlecht, teuer ≠ sicher“ (Entwickler).
- **Weitere Modifikationen** (Narbengestaltung, gespaltene Zunge, subdermale Implantate und weitere real existierende Formen) kommen vor, meist seltener als Tattoos und Piercings.
- **Nägel:** Nagelstudios (Hände und Füße), lange künstliche Nägel und Nagellack als Läden und Anziehungsmerkmale (Entwickler, 15.09.).

---

## 6. Kosmetische Eingriffe

📋 Kosmetische Eingriffe sind eine **echte Weltmechanik**: Brustvergrößerung/-verkleinerung, Gesäßoperationen, Fettabsaugung, Gesichtschirurgie, Nasenkorrekturen und weitere plausible Eingriffe. Es gibt Kliniken, Ärzte, Preise, Ruf, Termine, Heilungszeit und mögliche Komplikationen. Ein Eingriff verändert den **Körperzustand wirklich**: Maße, Kleidung, Weichgewebe-Physik, Schwerpunkt, Animation und IK, Aussehen. NPCs können solche Entscheidungen selbst treffen.

---

## 7. Studios, Kliniken und Internet

📋

- **Studios** sind echte Betriebe: in der Metropole 20 bis 30 oder mehr (Entwickler), von kleinen Shops bis zu riesigen mehrstöckigen Studios und weltberühmten Künstlern; auf dem Land eher kleine Studios und private Künstler. Jedes Studio hat eigene Regeln, Preise und Terminplanung; bessere Künstler arbeiten meist nach Termin. Preise hängen vom Ort ab (Innenstadt teurer). Spezialisierungen (Blackout, Farbe) existieren.
- **Internet:** Portfolio, Künstler, Bewertungen, Termine, Preise, Spezialisierungen, Online-Buchung. Spieler können später **vereinfachte Webseiten** erstellen (Teil des Wirtschaftssystems), und damit Geld verdienen ([Modul 08](08-Welt-Orte-Branche-Wirtschaft.md#6-spielwelt-internet-und-medien)).
- Beide Bereiche nutzen die normale Wirtschaft.

---

## 8. Technik und Produktion

- 📋 Jedes Tattoo bleibt im Spielzustand ein **eigenes Objekt** (Motiv, Künstler, Position, Größe, Farbe, Alter, Zustand), wird fürs Rendering aber zu optimierten **Haut-/Körper-Schichten** zusammengefasst, damit Vollkörper-Bedeckung nicht Hunderte Draw Calls erzeugt (Frage 113, bestätigt). Aggressive Bündelung und Detailstufen; hohe Nahqualität.
- 📋 **Gemeinsamer Produktionsablauf** für Spieler, NPCs, Mods und Werkzeuge: Entwurf oder Import → Vorschau → Interpretation durch den Künstler → Platzierung → Sitzungen → Heilung → Langzeitzustand.
- 📋 **Optimierung** ist globales Kernziel: hohe Qualität, aber nicht „jeder braucht Spitzenhardware“.
- 🔭 Die technische Umsetzung des realistischen Tätowierens (Auftragen, Verlaufsdarstellung) ist offen („wie wir das machen, weiß ich nicht“, Entwickler).

---

## 9. Offene Punkte

- ❓ Konkrete Parameterlisten des Editors, Rendering-Technik der Tattoos, Piercing-Physik.
- ❓ Zulässigkeit von Symbolik (siehe [Abschnitt 3](#3-tattoos)).
- ❓ Zeitliche Einordnung: Der Editor ist Teil der frühen Charaktergrundlagen, Tattoos sind laut Plan ein späteres Ausbaupaket.
- ❓ Einfluss von Körperform und Eingriffen auf Kleidungs- und Animationspipeline (Kompatibilität der Kleidung zu extremen Maßen).

Weitere Punkte: [Offene Fragen](11-Offene-Fragen-und-Konflikte.md).
