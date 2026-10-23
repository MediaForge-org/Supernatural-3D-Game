# NPCs 02: Leitsätze und Abgrenzung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt die Leitsätze, die für alle NPCs und die lebendige Welt gelten, die ausdrücklichen **Nicht-Ziele** und den Rahmen für Umfang und Sicht des Spielers. Jede Zeile hat eine Quelle. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Die acht verbindlichen Produktanforderungen](#1-die-acht-verbindlichen-produktanforderungen)
2. [Leitsätze aus den Planungsrunden](#2-leitsätze-aus-den-planungsrunden)
3. [Nicht-Ziele und Schranken](#3-nicht-ziele-und-schranken)
4. [Umfang: wer ein NPC ist](#4-umfang-wer-ein-npc-ist)
5. [Die Welt aus Sicht des Spielers](#5-die-welt-aus-sicht-des-spielers)
6. [Reihenfolge ist nicht Umfang](#6-reihenfolge-ist-nicht-umfang)
7. [Abhängigkeiten](#7-abhängigkeiten)
8. [Was fehlt](#8-was-fehlt)

---

## 1. Die acht verbindlichen Produktanforderungen

Am 22.10.2026 hat der Entwickler bestätigt, dass das fertige Spiel ein tiefes, persistentes Weltsimulationssystem mit NPC-Gedächtnis und Nemesis-/Rivalitätssystem enthält (`PE-N1022`). Es ist ein **verpflichtender Teil der Langzeitvision**, **kein M1-Feature**, und die Reihenfolge der Umsetzung ist offen. Die Zeilen unten geben den Inhalt der Anforderungen A–H nach [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §1 knapp wieder; die ausführliche Fassung steht dort.

| ID | Anforderung | Status | Quelle |
|---|---|:---:|---|
| N-LT-001 | **A, Persistente NPC-Identitäten.** Wichtige NPCs behalten dauerhaft Identität, persönliche Eigenschaften, soziale Beziehungen, Ziele und eine individuelle Lebensgeschichte | 📋 | `PE-N1022` A |
| N-LT-002 | **B, Langzeitgedächtnis.** NPCs können sich an relevante Begegnungen, Entscheidungen, Hilfe, Verrat, Verletzungen und Konflikte erinnern; Erinnerungen beeinflussen späteres Verhalten | 📋 | `PE-N1022` B |
| N-LT-003 | **C, Nemesis und Rivalität.** Überlebende Gegner können zurückkehren, persönliche Feindschaften entwickeln, aus früheren Begegnungen lernen und ihre Taktik anpassen; ausdrücklich auch intelligente übernatürliche Kreaturen | 📋 | `PE-N1022` C |
| N-LT-004 | **D, Autonome Fraktionen und Organisationen.** Monstergruppen, Hunter-Organisationen, kriminelle Netzwerke, Behörden und andere Gruppen verfolgen eigene Ziele, bilden Allianzen, beanspruchen Gebiete, bekämpfen einander und ändern ihre Strategie | 📋 | `PE-N1022` D |
| N-LT-005 | **E, Persistente Konsequenzen.** Verletzungen, Tode, Rettungen, zerstörte Infrastruktur, Konflikte und andere relevante Ereignisse können dauerhafte Wirkung haben | 📋 | `PE-N1022` E |
| N-LT-006 | **F, Unabhängige Lebensentwicklung.** Wichtige NPCs entwickeln sich ohne direkte Spielerinteraktion weiter: Beziehungen ändern sich, sie ziehen um, arbeiten, treten Organisationen bei oder verlassen sie, reagieren auf Weltereignisse | 📋 | `PE-N1022` F |
| N-LT-007 | **G, Skalierbare Weltsimulation.** Hohe Genauigkeit in Spielernähe, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben nicht geladener NPCs und Gebiete, Persistenz wichtiger Ergebnisse; nicht jeder NPC läuft als voller Unreal-Actor. Die Weltgröße (rund 2.200 km², ein Arbeitsszenario) darf nirgends fest verdrahtet sein | 📋 | `PE-N1022` G |
| N-LT-008 | **H, Integration.** Anbindung an Weltzustand, Ermittlung/Caseboard, Monster-KI und Bestiarium, Verletzungen und Medizin, Ruf, Polizei und Justiz, Organisationen und Territorien, Dialog und Beziehungen, Speichern/Laden, Koop und Host-Speicherstand; die Simulation darf keine widersprüchlichen Zustände zwischen diesen Systemen erzeugen | 📋 | `PE-N1022` H |
| N-LT-009 | Die Reihenfolge der Umsetzung ist offen; Phasen bestimmen die Reihenfolge, nicht den Umfang. Das System ist **nicht Teil von M1** | 📋 | `PE-N1022`; `CLAUDE.md` §1, §4 |
| N-LT-010 | Die RDR2-Modifikation, die der Entwickler als Gedankenvergleich nannte, ist weder Abhängigkeit noch Codequelle; die eigenen Systeme werden unabhängig spezifiziert | 📋 | `PE-N1022` (Proposal 0010 §1, Referenzabsatz) |

Die Anforderungen sind **Produktrichtung**, keine technische Spezifikation. Die Abschnitte 3–11 des Proposals (Bausteine, Skalierung, Konsistenzregeln, Phasen) sind Ingenieurvorschlag und stehen hier nur als Entwurf (💡) in den jeweiligen Fachmodulen.

---

## 2. Leitsätze aus den Planungsrunden

| ID | Leitsatz | Status | Quelle |
|---|---|:---:|---|
| N-LT-011 | **Glaubwürdige Konsistenz vor vollständiger Simulation.** Die Welt soll nur so tief simuliert werden, dass der Spieler keinen offensichtlichen Fake bemerkt; das ist eine Architekturregel. Der Entwickler: „glaubwürdig, das wichtigste von diesem spiel ist realismus“ | 📋 | `23#1057E` Punkt 125; `23#1058A` ‚Oberste Regel aus Block 8‘ |
| N-LT-012 | **Keine sichtbaren Beziehungsbalken.** Vertrauen, Angst, Respekt, Verdacht und verwandte Werte sind intern; sie zeigen sich über Ton, Wortwahl, Körpersprache, Zusammenarbeit, Information, Zugang, Gefallen und Folgen | 📋 | `M-PD` ‚NPCs and AI‘; `23#1057E` Punkt 40; „aber der spieler sieht nicht ob der npc ihn mag es merkt es am ton“ in `23#52E` |
| N-LT-013 | **Kein magisches Allwissen.** Ein NPC weiß etwas nur, wenn er es gesehen, gehört, erzählt bekommen oder über Nachrichten und soziale Medien erfahren hat | 📋 | `23#1057E` Punkt 25; `23#1058A` ‚NPC-Memory und Informationen‘ |
| N-LT-014 | **NPC-Überzeugungen können falsch sein.** Zeugenaussagen sind Information, keine garantierte Wahrheit | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-LT-015 | **Nicht jeder NPC braucht die volle Simulation.** Wichtige Figuren sind vollständige Akteure mit Zustandslogik und Gedächtnis; Hintergrund und ferne Bevölkerung sind leichter oder abstrakt | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-LT-016 | **Persistenzstufen A bis D** gelten für die Bevölkerung (A Story/Hunter/Nebenfall/enge Beziehungen maximal; B Händler/Nachbarn/Arbeitgeber/wiederkehrende Kontakte stark; C normale Einwohner leicht; D Menschenmengen abstrahiert) | 📋 Richtung | `23#1056A` Fragen 17, 121; „ja“ in `23#1057E`; `23#1058A` |
| N-LT-017 | **Wichtige NPCs bleiben dieselben Personen** und werden nicht durch neue Zufallsmodelle ersetzt; „außer du ballerst sie weg, dann ist die person weg“ | 📋 | `23#1057E` Punkt 16; `23#1058A` |
| N-LT-018 | **Die Welt reagiert auf einen Tod** (Ersatz im Geschäft, Reaktion der Familie, Erinnerung anderer NPCs; bei bedeutenden Figuren möglicherweise Beerdigung, Antwort des Entwicklers: „ja kann sein“) | 📋 | `23#1057E` Punkte 103–104, 112 |
| N-LT-019 | **Normale Einwohner sind ansprechbar.** Der Spieler soll mit jedem normalen NPC reden können, „nicht sehr viel aber schon“; wichtige NPCs haben tiefere Dialogmöglichkeiten | 📋 | `23#1057E` Punkte 31–32; `M-PD` ‚Dialogue …‘ |
| N-LT-020 | **Die Welt läuft nur, während man spielt.** Im Einzelspieler friert sie beim Beenden ein; im Online-Betrieb gelten später andere Zeit- und Serverregeln | 📋 | „die welt läuft nur wenn man gerade spielt, im online wird das anders sein“ in `35#279E` Punkt 11; Bestätigung in `35#280A` |
| N-LT-021 | **Wenn nichts anderes definiert ist, verhält sich die Welt möglichst realistisch und logisch wie in echt**; dieser Grundsatz gilt für das gesamte Spiel | 📋 | „ja so wie das komplette spiel nicht du das speziell“ in `35#785E` Punkt 135; `35#786A` |
| N-LT-022 | **NPCs leben wirklich weiter.** Jemand, den man Jahre später trifft, kann sich äußerlich und im Leben verändert haben (Beispiel des Entwicklers: ein Full-Body-Tattoo) | 📋 | „ja sowieso, die npcs sollen ein leben haben“ in `35#785E` Punkt 137; `35#786A` |
| N-LT-023 | **Große Konsequenzen brauchen große Ursachen.** Eine einzelne falsche Dialogantwort soll nicht Stunden von Fraktionsinhalt zerstören | 🧭 | `23#676A` (Abschnitt ‚Mitgliedschaft darf auch beendet werden‘, Vorschlag des Assistenten, im Zusammenhang bestätigt durch `23#677E`) |
| N-LT-024 | **Optionale Systeme sind von der Hauptstory entkoppelt.** Fraktionen, Clubs, Sekten und Rekrutierte erweitern Dialoge, Begleiter, Informationen und Vorbereitung; keine Hauptmission hängt davon ab | 📋 | `23#677E`; `23#678A`; `M-PD` ‚Acts, chapters and controlled story reactivity‘ |
| N-LT-025 | **Nach der Hauptstory bleibt die Welt aktiv.** Nach dem Abspann bleiben freie Welt, Fraktionsinhalte, Rekrutierung und Beziehungen aktiv; das Hunter-HQ ist kein eingefrorenes Museum | 📋 | `23#679E`; `23#680A`; „man kann vielleicht zum leader aufsteigen“ in `23#679E` |
| N-LT-026 | **Lernen ist Wissensaufbau, nicht Stärkerwerden.** Kein verborgener Wert steigt mit jeder Begegnung; das gilt auch für Nemesis-Figuren | 📋 | `M-PD` ‚Genre and progression identity‘; [Kreaturen 02](../Kreaturen/02-Leitsaetze-und-Abgrenzung.md) L2; `PE-N1022` C (nur Anpassung der Taktik) |

---

## 3. Nicht-Ziele und Schranken

| ID | Schranke | Status | Quelle |
|---|---|:---:|---|
| N-LT-027 | **Kein globales Gut/Böse-Ehrensystem** (RDR2-Honor); eine Handlung kann für Person A gut und für Person B schlecht sein | 🧭 | `23#603A` Abschnitt 14 (Aussage des Assistenten); stützt sich auf `M-PD` ‚NPCs and AI‘ |
| N-LT-028 | **Kein Strategiespiel-Micromanagement** für Fraktionsführung; der Spieler bleibt der Charakter in der Welt und erledigt selbst Aufgaben | 🧭 | `23#678A`; `23#680A` (Vorschlag des Assistenten) |
| N-LT-029 | **Kein Radikalisierungssimulator.** Mitgliedschaft, Personen, Konflikte und Folgen werden glaubwürdig simuliert; reale extremistische Organisationen werden nicht als Handbuch ihrer Rekrutierungs- oder Operationsmethoden nachgebaut, sondern als eigene fiktionale Strukturen | 🧭 | `23#678A`, `23#680A` (Vorschlag des Assistenten; der Entwickler fragte nach der Bedeutung in `23#677E` und antwortete in `23#679E` mit ‚sehr realistisch‘, ohne die Grenze zu widerlegen) |
| N-LT-030 | **Keine Wertung durch das Spiel.** Das Spiel zeigt Überzeugung, Verhalten, Behandlung anderer und Folgen; die Bewertung einer Person oder Gruppe bleibt beim Spieler | 🧭 | `23#676A` (Einleitung und Abschnitt ‚Extremistische Gruppen‘) |
| N-LT-031 | **Ethnische Zugehörigkeit ist keine mechanische Ursache** von Kriminalität; Unterschiede zwischen Vierteln entstehen aus Armut, Arbeitslosigkeit, organisierter Kriminalität, Drogenmärkten, Infrastruktur, Polizeipräsenz, Stadtentwicklung und sozialer Stabilität | 📋 | [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13; `23#1058A` ‚Eine wichtige Realismusregel‘ |
| N-LT-032 | **Partner als Entführungsplot nur selten.** Ein Partner kann in Gefahr geraten, aber das ist kein ständiges Handlungswerkzeug | 📋 | `23#1088A` Frage 15; „11-15 genau“ in `23#1089E` |
| N-LT-033 | **Wichtige NPCs sterben nicht durch banale Simulationsfehler** (zum Beispiel Verkehrs-KI); ein vom Spieler bewusst herbeigeführter Tod ist etwas anderes | 📋 | `23#1057E` Punkt 105; `23#1058A` ‚Verletzungen und permanente Folgen‘ |
| N-LT-034 | **Keine Pflicht-Mikroverwaltung** der Crew (HQ-Versorgung, Beiträge, Aufgaben bleiben organisch und nicht erdrückend) | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘ |
| N-LT-035 | **Persönliche Wünsche der Crew sind nicht verpassbar wegen absurder Zeitfenster**; sie können nur entfallen, wenn die Figur stirbt oder dauerhaft geht | 📋 | `M-PD` ‚Acts, chapters and controlled story reactivity‘ (Crew personal requests); `23#603A` Abschnitt 3 |

---

## 4. Umfang: wer ein NPC ist

„NPC“ ist hier jede handelnde Figur der Welt außer dem Spielercharakter. Die Quellen nennen folgende Gruppen; jede bekommt in den Fachmodulen eine Stelle:

| Gruppe | Beispiele aus den Quellen | Persistenz (Entwurf) | Modul |
|---|---|---|---|
| Hunter-Crew und Begleiter | Kernteam aus zwei Hunter am Anfang, später weitere; menschliche Begleiter mit Persönlichkeit, Moral, Ausrüstung, Fahrzeug | A | [19](19-Begleiter-Crew-und-Rekrutierung.md) |
| Andere Hunter | konkurrieren, kooperieren, werden Rivalen, sterben | A/B | [11](11-Nemesis-Modell-und-Datensatz.md), [18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md) |
| Story- und Fallfiguren | Zeugen, Verdächtige, Opfer, Auftraggeber | A | [05](05-Gedaechtnis-Erinnerung-und-Vergessen.md), [06](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md) |
| Wiederkehrende Kontakte | Händler, Nachbarn, Arbeitgeber, Mechaniker | B | [13](13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md) |
| Normale Einwohner | Familien, Arbeiter, Studenten, Touristen, Obdachlose | C | [10](10-Familien-Haushalte-und-soziale-Netzwerke.md), [13](13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md) |
| Menschenmengen | Stadion, Festival, Nachtleben, Skigebiet | D | [22](22-Simulationsstufen-Skalierung-und-Performance.md) |
| Beamte und Einsatzkräfte | Polizei, Sheriff, Feuerwehr, Rettung, Detectives | A bis C | [18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md) |
| Kriminelle und Gruppen | Straßengangs, Motorradclubs, Sekten, extremistische Gruppen, organisierte Kriminalität | B, Mitglieder je Rolle | [16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md), [17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md) |
| Wesen mit menschlicher Identität | friedliche Wesen unter Menschen, getarnte Wesen | A/B | [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md); [Modul 15](15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md) |
| Monster als Individuen | intelligente Wesen, Anführer, Überlebende | A/B | [11](11-Nemesis-Modell-und-Datensatz.md), [12](12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md) |
| Tiere | Hund als besonderer Begleiter; Wildtiere | je Rolle | [Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md); [Modul 19](19-Begleiter-Crew-und-Rekrutierung.md) §6 |

**Nicht Teil des NPC-Kerns:** Roboter und fortgeschrittene KI als Bewohner (siehe [Modul 01](01-Quellenlage-und-Methodik.md) §6 F5), Spielerfiguren anderer Spieler (Koop: [Modul 23](23-Koop-Host-Speicherstand-und-Mods.md)).

---

## 5. Die Welt aus Sicht des Spielers

Das Erlebnis ist das Ziel; die Datenmodelle sind Mittel. Aus den Quellen folgt (📋 Richtung, Ausformulierung 💡):

1. **Der Spieler sieht Verhalten, keine Zahlen.** Er merkt Zuneigung, Misstrauen oder Angst am Ton, an der Bereitschaft zu helfen, an Informationen, die er bekommt oder nicht (N-LT-012).
2. **Die Welt hat ein Gedächtnis.** Wer einem Sheriff das Leben rettete, hört Jahrzehnte später etwas Konkretes statt eines Schwellenwerts: Das Beispiel stammt vom Assistenten (`23#51A` Abschnitt 9, 💡), die Richtung („das Spiel weiß warum er dir vertraut“) ist als NPC-Memory bestätigt (`23#52E`, Punkt 9 „ja genau“).
3. **Die Welt bewegt sich ohne den Spieler.** Ein Diner kann Jahre später geschlossen, renoviert oder berühmt sein (`23#1058A` ‚Sozialer World State‘).
4. **Feinde sind Individuen.** Ein Gegner, der überlebte, kann verändert zurückkehren; er ist kein Respawn ([Modul 11](11-Nemesis-Modell-und-Datensatz.md)).
5. **Nichts davon zwingt zum Mitspielen.** Alle Systeme laufen im Hintergrund und treten in Erscheinung, wenn der Spieler hinsieht oder betroffen ist (N-LT-024, N-LT-025).
6. **Verstehbarkeit.** Hidden values brauchen sichtbare Folgen, sonst bleibt Feindschaft oder Freundschaft unlesbar (Risiko in Proposal 0010 §9, ‚Fun und Lesbarkeit‘, 💡). Welche Zeichen die Welt dafür gibt, beschreibt [Modul 20](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md).

---

## 6. Reihenfolge ist nicht Umfang

`CLAUDE.md` §1 und §4 verlangen, dass Phasen die Reihenfolge der Umsetzung bestimmen, nicht den Umfang. Die Quellen sortieren selbst:

| Quelle | Aussage | Status |
|---|---|:---:|
| `23#1057E` Punkt 9 | Generationen: „ja soll sie, aber das soll erst spät gemacht werden“ | 📋 |
| `23#1090A` ‚Priorisierung‘ (Block 10) | P1 Beziehungs-Grundsystem, Begleiter, Hund; P2 tiefere Romanzen, Sozialstealth, Tourismus, Feiertage; P3 Generationen, Familiengründung, extrem tiefe Lebenssimulation. „P2/P3 bedeutet nicht ‚wird nie eingebaut‘“ | 📋 |
| `23#1089E` Punkt 10 | Familiengründung: „weiß nicht, als hunter eher schwierig“ | ❓ |
| Proposal 0010 §10 | Phasen L0 bis L5 (unverbindlich) | 💡 |

Der Plan für diesen Ordner: ein **kleiner, testbarer Kern zuerst** (Datenmodell von Identität, Ereignis und begrenztem Gedächtnis in portablem C++, Proposal 0010 L1), danach ein benannter NPC mit Gedächtnis im Vertical Slice (L2), ein wiederkehrender Gegner (L3), gestufte Simulation (L4), Fraktionen und Skalierung (L5). Das ist **💡** und folgt dem Proposal; es verkleinert nicht den Umfang.

---

## 7. Abhängigkeiten

| Braucht | Aus | Zweck |
|---|---|---|
| Stabile IDs und Weltzustand | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md), `WorldFacts` | Identität, Konsequenzen, Speicherung |
| Aussehens-Rezepte | [Decision 0005](../../Decisions/0005-character-appearance-foundation.md) | individuelles Aussehen aus Seed plus Abweichungen |
| Dialogsystem mit Beziehungs- und Gedächtniskontext | `M-PD` ‚Dialogue …‘ | sichtbare Wirkung |
| Verletzungen und Medizin | [Spielvision](../Spielvision.md) §14 (Überblick) | dauerhafte Folgen, Narben |
| Ermittlung | [Ermittlung](../Ermittlung/README.md) | Zeugen, Beweise, Ruf |
| Kreaturenregeln | [Kreaturen](../Kreaturen/README.md) | Artgrenzen der Nemesis-Anpassung |
| Weltuhr | [Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md) | einheitliche Zeit |

---

## 8. Was fehlt

- Kein Datenschema, keine Felder, keine Zahlen: alles Entwurf (💡) oder offen ([Modul 27](27-Konflikte-und-offene-Fragen.md)).
- Keine Liste benannter NPCs, Fraktionen oder Antagonisten. Das Proposal 0010 §12 nennt das ausdrücklich ein Nicht-Ziel.
- Keine Entscheidung zu Budgets, Schwellen und Messwerten (`M-OQ16`).
- Keine Entscheidung, welche Weltereignisse welche Persistenzfolgen haben müssen.
