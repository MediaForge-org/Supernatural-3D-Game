# NPCs 26: Entscheidungsregister

> [!IMPORTANT]
> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 ist vom Entwickler bestätigt oder im Master als akzeptiert geführt; 🧭, 💡, ❓ sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuszeichen: [Legende](README.md#statuslegende).
> **Dieses Dokument ist erzeugt** (`Tools/world-audit/render_npc_register.py`) aus den Tabellen der Module 02 bis 24. Änderungen gehören in die Module, nicht hierher.

Teil der NPC-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Kürzel in den Quellen: `P<n>` und ‚Frage n‘ meinen die nummerierte Frage in der A-Nachricht, die Antwort steht in der E-Nachricht.

**Inhalt**

1. [LT: Leitsätze und Produktanforderungen](#lt-leitsätze-und-produktanforderungen)
2. [ID: Identität, Persistenzstufen, Datensatztiefe](#id-identität-persistenzstufen-datensatztiefe)
3. [PS: Persönlichkeit, Zustände, Zeitskalen](#ps-persönlichkeit-zustände-zeitskalen)
4. [GE: Gedächtnis, Erinnerung, Vergessen](#ge-gedächtnis-erinnerung-vergessen)
5. [WA: Wahrnehmung, Wissen, Glaube](#wa-wahrnehmung-wissen-glaube)
6. [IN: Informationsfluss, Gerüchte, Kommunikation](#in-informationsfluss-gerüchte-kommunikation)
7. [BE: Beziehungen: Modell und Wirkung](#be-beziehungen-modell-und-wirkung)
8. [BV: Beziehungsentwicklung](#bv-beziehungsentwicklung)
9. [FA: Familien, Haushalte, Netzwerke](#fa-familien-haushalte-netzwerke)
10. [NE: Nemesis: Modell und Datensatz](#ne-nemesis-modell-und-datensatz)
11. [NL: Nemesis: Lebenszyklus](#nl-nemesis-lebenszyklus)
12. [LE: Autonomes Leben](#le-autonomes-leben)
13. [LB: Lebensereignisse, Alterung, Tod](#lb-lebensereignisse-alterung-tod)
14. [WE: Weltereignisse und Übernatürliches](#we-weltereignisse-und-übernatürliches)
15. [FR: Fraktionen: Modell, Mitgliedschaft, Hierarchie](#fr-fraktionen-modell-mitgliedschaft-hierarchie)
16. [TE: Fraktionen: Territorien, Konflikte, Macht](#te-fraktionen-territorien-konflikte-macht)
17. [BH: Behörden, Hunter-Organisationen, Wesen](#bh-behörden-hunter-organisationen-wesen)
18. [CO: Begleiter, Crew, Rekrutierung](#co-begleiter-crew-rekrutierung)
19. [DI: Dialog und Sichtbarkeit](#di-dialog-und-sichtbarkeit)
20. [PE: Weltpersistenz und Speicherung](#pe-weltpersistenz-und-speicherung)
21. [SI: Simulationsstufen und Skalierung](#si-simulationsstufen-und-skalierung)
22. [KO: Koop, Host-Speicherstand, Mods](#ko-koop-host-speicherstand-mods)
23. [KS: Konsistenz zwischen Systemen](#ks-konsistenz-zwischen-systemen)
24. [EN: Entwurfsregeln (unverbindliche Vorschläge)](#en-entwurfsregeln-unverbindliche-vorschläge)

---

## LT: Leitsätze und Produktanforderungen

Gruppe zu [Modul 02](02-Leitsaetze-und-Abgrenzung.md).

| ID | Aussage | Status | Quelle |
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

## ID: Identität, Persistenzstufen, Datensatztiefe

Gruppe zu [Modul 03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-ID-001 | Wichtige und wiedererkennbare NPCs bleiben dieselben Personen und werden nicht durch zufällige neue Modelle ersetzt | 📋 | `23#1056A` Frage 16; „ja genau, außer du ballerst sie weg, dann ist die person weg“ in `23#1057E` |
| N-ID-002 | Auch viele normale Bewohner haben eine persistente Identität, selbst ohne Storyrelevanz | 📋 | `23#1056A` Frage 17; „ja“ in `23#1057E` |
| N-ID-003 | Viele normale NPCs haben echte Namen | 📋 | `23#1056A` Frage 18; „ja schon“ in `23#1057E` |
| N-ID-004 | Stufen A–C haben intern Wohnort und Job; die meisten NPCs haben eine plausible Wohnung, manche sind obdachlos | 📋 | `23#1056A` Fragen 4, 19; „ja, die meisten schon, manche sind obdachlos“ in `23#1057E` |
| N-ID-005 | NPCs benutzen wiederholt dasselbe eigene Fahrzeug statt jeden Tag ein anderes | 📋 | `23#1056A` Frage 20; „natürlich, sollen sie sogar“ in `23#1057E` |
| N-ID-006 | NPCs besitzen mehrere Outfits und wechseln nach Wetter, Arbeit und Anlass | 📋 | `23#1056A` Frage 21; „ja“ in `23#1057E` |
| N-ID-007 | Zumindest wichtige NPCs besitzen Wohnungen, Fahrzeuge, Haustiere und andere Gegenstände persistent | 📋 | `23#1056A` Frage 22; „ja“ in `23#1057E` |
| N-ID-008 | Jeder NPC hat grundsätzlich eine dauerhafte **globale NPC-ID**, auch wenn er gerade nicht geladen ist | 📋 | `35#278A` Frage 1; „1-4 ja“ in `35#279E` |
| N-ID-009 | Die Persönlichkeits-DNA entsteht aus einem Seed, der an die globale NPC-ID gekoppelt ist, nicht an Spawn-Zeit oder Position; sie wird nur am Anfang erzeugt und kann sich später ändern | 📋 | `35#260A` Fragen 1–3; Antworten in `35#261E` („es wird nur am anfang generiert“, „das wäre blöd in einer beziehung auf die wäre dann zurückgesetzt weil sich alles geändert hat“) |
| N-ID-010 | Ein Archetyp ist nur ein Start-Bias, niemals die ganze Person; zwei NPCs desselben Archetyps können sich stark unterscheiden | 📋 | `35#260A` Fragen 6–8; „ja archetyp ist nicht alles oder“ in `35#261E`; Klarstellung `35#262A` |
| N-ID-011 | Die Generierung nutzt ein korreliertes Modell statt reinem Würfeln; ungewöhnliche Persönlichkeiten sind erlaubt, logisch widersprüchliche Profile werden vom Generator geprüft oder bewusst selten gemacht | 📋 | `35#260A` Fragen 9–10; „10-20 ja“ in `35#261E`; `35#262A` ‚Zu 9‘ |
| N-ID-012 | Das Aussehen wird ebenfalls reproduzierbar erzeugt; der Seed bleibt gespeichert, damit materialisierte Werte exakt reproduzierbar sind | 📋 Richtung | `35#278A` Frage 23; „12-35 ja“ in `35#279E` |
| N-ID-013 | Bei einer **relevanten Begegnung** wird der aktuell generierte Zustand eingefroren und persistent, sodass spätere Generator-Updates ihn nicht verändern | 📋 Richtung | `35#278A` Frage 24; „12-35 ja“ in `35#279E` |
| N-ID-014 | Ein unbekannter zufälliger NPC braucht deutlich weniger Zustand als jemand, den der Spieler kennt; detaillierte Werte werden erst materialisiert, wenn der NPC relevant wird | 📋 Richtung | `35#278A` Fragen 7, 21–22; „6-10 ja“ und „12-35 ja“ in `35#279E` |
| N-ID-015 | Wer dem Spieler relevant begegnet (Beziehung, Story, einmalige bedeutende Begegnung), steigt in eine höhere Persistenzstufe und behält sie dauerhaft; nach Jahrzehnten Irrelevanz ist Herabstufung möglich, die Historie bleibt | 📋 | `35#278A` Fragen 8–10; „6-10 ja“ in `35#279E` |
| N-ID-016 | Zufällige Begegnung → Minimal Record; kurzes Gespräch → Known NPC Record; relevante Begegnung → Detailed Persistent Profile; Beziehung oder wichtiger NPC → Full Persistent Profile | 📋 Richtung | `35#262A` ‚Zu 39–40‘ (Ergänzung des Entwicklers in `35#261E` Punkte 39–40, Ausformulierung des Assistenten) |
| N-ID-017 | Eine relevante Einzelbegegnung wird so genau gespeichert wie eine Beziehung | 📋 | „aber wenn man einen one night stand hat dann muss das genau so sein wie bei einer beziehung von der Genauigkeit“ in `35#261E` Punkt 40 |
| N-ID-018 | Der Tod eines NPCs bleibt dauerhaft im Weltzustand; relevante Identität und Historie bleiben erhalten, nur unnötiger technischer Detailzustand darf später verdichtet werden | 📋 | `35#272A` Fragen 31–33; „1-50 passt alles“ in `35#273E`; `35#274A` |
| N-ID-019 | Ein NPC kann den Wohnort wechseln (auch zwischen den Bundesstaaten) und behält dieselbe Identität und Historie | 📋 | `35#272A` Fragen 44–45; „1-50 passt alles“ in `35#273E` |
| N-ID-020 | Unveränderte NPCs sollen praktisch nur `NPC_ID + Seed` brauchen; erst Abweichungen werden gespeichert | 📋 Richtung | `35#278A` Fragen 31–33; „12-35 ja“ in `35#279E`; Präzisierung `35#280A` |
| N-ID-021 | Bundesstaaten und Regionen dürfen die Bevölkerung subtil beeinflussen (Stimme, Aussehen, Stil, Berufe), ohne feste Persönlichkeit je Gebiet | 📋 | `35#260A` Fragen 41–45; „41-44 ja“ und Punkt 45 in `35#261E` |
| N-ID-022 | Die Persistenzstufen A bis D gelten (siehe [Modul 02](02-Leitsaetze-und-Abgrenzung.md) N-LT-016) | 📋 Richtung | `23#1056A` Fragen 17, 121; „ja“ in `23#1057E` |
| N-ID-023 | Sollen Einwanderer in der Bevölkerung vorkommen? Der Entwickler: „es soll keine immigranten geben nur leute die von dort kommen oder aus der usa“; der Assistent empfahl einzelne Einwanderer, Studierende, Touristen und Geschäftsleute; Entwickler: „Ok“ | ⚠️ | `35#261E` Punkt 45; `35#262A`; `35#263E` |
| N-ID-024 | Soll der Datensatz einer toten Person für die Beziehungsdaten gelöscht werden („außer sie ist tod“ in `35#261E` Punkt 39) oder erhalten bleiben (`35#273E`)? | ⚠️ | `35#261E` Punkt 39; `35#273E` |

---

## PS: Persönlichkeit, Zustände, Zeitskalen

Gruppe zu [Modul 04](04-Persoenlichkeit-Zustaende-und-Zeitskalen.md).

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

## GE: Gedächtnis, Erinnerung, Vergessen

Gruppe zu [Modul 05](05-Gedaechtnis-Erinnerung-und-Vergessen.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-GE-001 | NPCs erinnern sich an relevante Dinge, die der Spieler getan hat (geholfen, bedroht, gerettet, bestohlen, verletzt, Fahrzeug beschädigt, bei einem Fall belogen) | 📋 | `23#1056A` Frage 23; „ja genau“ in `23#1057E` |
| N-GE-002 | Wichtige NPCs haben Gedächtnis; **wichtige Ereigniserinnerungen bleiben** | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-GE-003 | Das Gedächtnis ist **selektiv**: nicht jede Sekunde wird gespeichert; kurzfristige Informationen verschwinden, wichtige Ereignisse können praktisch dauerhaft bleiben | 📋 Richtung | `23#51A` Abschnitt 9 („Das soll nicht bedeuten, dass jeder NPC jede Sekunde seines Lebens speichert“); bestätigt in `23#52E` Punkt 9 |
| N-GE-004 | Erinnerungen können mit der Zeit **schwächer oder ungenauer** werden | 📋 | `23#1056A` Frage 24; „ja“ in `23#1057E`; `35#268A` Frage 7; „1-13 ja“ in `35#269E` |
| N-GE-005 | Es gibt **Ankererinnerungen**: besonders wichtige Erlebnisse behalten dauerhaft Gewicht; ein NPC vergisst bestimmte Dinge nie komplett, auch wenn andere Werte abklingen | 📋 | `35#268A` Fragen 5–6; „1-13 ja“ in `35#269E` |
| N-GE-006 | Besonders positive oder negative Erlebnisse bleiben stärker haften; ein Memory-Trait bestimmt, wie viele Details ein NPC behält | 📋 | `35#268A` Fragen 8–9; „1-13 ja“ in `35#269E` |
| N-GE-007 | Persönlichkeit bestimmt, wie stark Ereignisse emotional gespeichert werden | 📋 | `35#268A` Frage 10; „1-13 ja“ in `35#269E` |
| N-GE-008 | Ein NPC muss etwas **tatsächlich gesehen, gehört oder erzählt bekommen** haben (oder aus Nachrichten und sozialen Medien erfahren), bevor er davon weiß; kein magisches Allwissen | 📋 | `23#1056A` Frage 25; „ja sicher“ in `23#1057E`; `23#1058A` |
| N-GE-009 | Ein NPC kann eine falsche Überzeugung haben; eine Zeugenaussage ist Information, keine Wahrheit | 📋 | `M-PD` ‚NPCs and AI‘; `23#51A` Abschnitt 9 (‚NPCs können auch falsches Wissen besitzen‘) |
| N-GE-010 | Ein einmaliges, relevantes Treffen macht aus einer Person eine **Known Person**; ein belangloses Gespräch hinterlässt nur eine sehr kleine Erinnerung; ein bedeutsamer Abend erzeugt einen detaillierten Beziehungsdatensatz | 📋 | `35#268A` Fragen 11–13; „1-13 ja“ in `35#269E` |
| N-GE-011 | Beim Wiedersehen erinnert sich ein NPC an Identität, Dauer seit dem letzten Treffen, Verlauf, Stimmung und relevante Absprachen, **aber nicht zwingend perfekt**: „ja aber es kann sein dass er sich nicht mehr genau erinnert“ | 📋 | `35#268A` Frage 14; Antwort in `35#269E`; Präzisierung `35#270A` |
| N-GE-012 | Es gibt Unterschiede zwischen ‚ich kenne dich‘, ‚ich erinnere mich gut‘ und ‚ich kenne dich sehr gut‘ | 📋 | `35#268A` Frage 15; „ja genau“ in `35#269E` |
| N-GE-013 | Ein NPC kann nach langer Zeit überrascht sein, den Spieler wiederzusehen, ihn nach Jahren anders bewerten (wenn der Spieler sich stark verändert hat), und lange Funkstille kann besondere Reaktionen auslösen | 📋 | `35#268A` Fragen 16–20; „16-30 ja“ in `35#269E` |
| N-GE-014 | Nach Jahren ohne Kontakt vergisst ein NPC manche Details, behält aber wichtige Erinnerungen; die Stärke einer alten Beziehung beeinflusst, wie schnell Erinnerungen verblassen | 📋 | `35#272A` Fragen 5–6; „1-50 passt alles“ in `35#273E` |
| N-GE-015 | Ein früherer relevanter Kontakt kann nach 10 Jahren noch erkannt werden; ein NPC erkennt jemanden gelegentlich erst nach kurzem Überlegen; stark verändertes Aussehen erschwert das Wiedererkennen; Stimme, Gang, Name und andere Hinweise helfen | 📋 | `35#272A` Fragen 7–10; „1-50 passt alles“ in `35#273E` |
| N-GE-016 | Ein NPC kann nach langer Funkstille selbst Kontakt aufnehmen; mögliche Wiedersehensreaktionen sind Freude, Überraschung, Distanz, Nostalgie, Ärger oder erneute Anziehung | 📋 | `35#272A` Fragen 11–12; „1-50 passt alles“ in `35#273E` |
| N-GE-017 | Alte Chemie kann sofort wieder da sein oder verschwunden sein; die Beziehung kann sich beim Wiedersehen neu entwickeln, statt den alten Status einfach wiederherzustellen | 📋 | `35#272A` Fragen 13–15; „1-50 passt alles“ in `35#273E` |
| N-GE-018 | NPCs können den Spieler und **auffällige Fahrzeuge** wiedererkennen; Kleidung und Maskierung beeinflussen das | 📋 | `23#1056A` Fragen 28–30; „ja“ beziehungsweise „passt“ in `23#1057E` |
| N-GE-019 | Derselbe Mitarbeiter kann sich später daran erinnern, dass der Spieler schon einmal unter anderer Identität dort war | 📋 | `23#1088A` Frage 67; „61-67 ja“ in `23#1089E` |
| N-GE-020 | Informationen verbreiten sich über Freunde, Familie, soziale Medien und Nachrichten und können dabei falsch, verzerrt, übertrieben oder unvollständig werden | 📋 | `23#1056A` Fragen 26–27; „ja“ in `23#1057E`; `23#1058A` |
| N-GE-021 | Das Gedächtnis wird in Klassen gehalten (permanent, long-term, short-term, disposable); Kurzzeitgedächtnis kann zusammengefasst werden | 📋 Richtung | `35#278A` Fragen 34–35; „12-35 ja“ in `35#279E` |
| N-GE-022 | Aus vielen ähnlichen kleinen Ereignissen entsteht eine kompakte Erinnerung (‚generell positive Geschichte mit X‘), **solange der NPC sich an alles Wichtige einzeln erinnern kann**; Zusammenfassungen ersetzen keine wichtigen Einzelereignisse | 📋 | `35#278A` Frage 36; „ja solange der npc sich an alles einzeln erinnern kann“ in `35#279E` Punkt 36; `35#280A` |
| N-GE-023 | Wichtige Detail-Historie für spielerbezogene Beziehungen bleibt einzeln erhalten | 📋 | `35#278A` Frage 37; „37-39 ja“ in `35#279E` |
| N-GE-024 | Erinnerungsschwund betrifft den NPC, nicht das, was der Spieler im Archiv festhielt | 💡 (Ermittlungs-Auslegung) | [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §4 |
| N-GE-025 | NPCs erinnern sich an verstorbene Personen und Ereignisse; ein Tod kann andere NPCs langfristig beeinflussen; Trauer ist je nach Beziehung unterschiedlich stark und lang | 📋 | `23#1056A` Frage 112; „ja“ in `23#1057E`; `35#272A` Fragen 34–36; „1-50 passt alles“ in `35#273E` |
| N-GE-026 | Orte entwickeln Geschichte (ein Restaurant wird ein bekannter Ort oder schließt) | 📋 | `23#1056A` Frage 113; „ja“ in `23#1057E`; `23#1058A` |
| N-GE-027 | Historische Beziehungen bleiben auch nach Jahrzehnten für Dialoge, Fälle, Familiengeschichten und Weltlore abfragbar | 📋 | `35#272A` Fragen 38–39; „1-50 passt alles“ in `35#273E` |
| N-GE-028 | In der Vergangenheit weiß ein Charakter nichts von Ereignissen seiner Zukunft; die Beziehungshistorie wird zwischen Zeitständen sauber getrennt; Änderungen der Vergangenheit laufen über das allgemeine Temporal-World-State-System | 📋 | `35#272A` Fragen 40–43; „1-50 passt alles“ in `35#273E` |
| N-GE-029 | Die Erinnerung eines NPC wird als Datensatz im Weltzustand geführt; Erinnerungen persistieren wichtige Ergebnisse; ein NPC-Gedächtnis ist von der Zustandslogik getrennt (StateTree: ‚Was mache ich?‘, Gedächtnis: ‚Was weiß ich?‘) | 📋 Richtung | `23#48A` Abschnitt 9; `M-PD` ‚NPCs and AI‘ |
| N-GE-030 | Monster mit individuellem Gedächtnis erinnern sich an Begegnungen, Waffen, Fallen, Verletzungen, Gnade und können Wissen weitergeben | 📋 | `48#139E`; Details in [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §8 |
| N-GE-031 | Zeugenaussagen und Erinnerungen sind keine Wahrheit: derselbe Spieler kann unter verschiedenen Beobachtern unterschiedlich erinnert werden | 📋 | `M-PD` ‚NPCs and AI‘; `23#603A` Abschnitt 14 (Aussage des Assistenten) |
| N-GE-032 | Wie lange ein NPC etwas behält, welche Genauigkeit gilt und was als ‚relevant genug‘ gilt, ist nicht festgelegt | ❓ | `M-OD`; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11 |

---

## WA: Wahrnehmung, Wissen, Glaube

Gruppe zu [Modul 06](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-WA-001 | Wahrnehmung arbeitet mit **Unsicherheit** statt mit magischen Spielerkoordinaten | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-002 | Das Wissen einer KI unterscheidet mindestens bestätigtes Wissen, starken Verdacht, schwachen Verdacht und Unbekanntes | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-003 | Wahrnehmungszustände können sein: unaware, suspicious, investigating, alert, confirmed target, searching | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-004 | Die Gewissheit über die letzte bekannte Position nimmt mit der Zeit ab; Gegner verlieren Präzision, nicht das Wissen, dass eine bestätigte Bedrohung existiert | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-005 | Adaptive Taktiken dürfen nur auf **beobachteten oder plausibel erschlossenen** Tatsachen beruhen; keine globale Telemetrie, kein verstecktes Metawissen | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-006 | Menschliche Gegner teilen Informationen nur über **plausible Kommunikation** (Stimme, Funk, Sichtzeichen); kein Schwarmgeist. Die Kommunikation von Wesen ist artspezifisch | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-007 | Intelligente Gegner können Hinweise (Geräusch, Blut, Türen, Fenster, letzte Position) zu Hypothesen kombinieren | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-008 | Wiederholte Ablenkungstricks können erkannt werden; der Gegner kann misstrauisch werden, ignorieren oder eine Falle vermuten | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-009 | Sicht berücksichtigt Richtung, Reichweite, tatsächliche Beleuchtung, Bewegung, Deckung, Silhouette, Haltung und Wetter; Dunkelheit macht nicht unsichtbar. Gehör berücksichtigt Lautstärke, Entfernung, Raumakustik, Wände und Türen, Hintergrundgeräusch, Wetter und Hörfähigkeit des Hörers | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-WA-010 | Ein NPC weiß etwas nur, wenn er es gesehen, gehört, erzählt bekommen oder über Nachrichten und soziale Medien erfahren hat | 📋 | `23#1056A` Frage 25; „ja sicher“ in `23#1057E`; `23#1058A` |
| N-WA-011 | Zeugen können Polizei und Rettung rufen (Notruf); der Täter kann beschrieben werden (Kleidung, Fahrzeug, Kennzeichen, Richtung) | 📋 | `23#1056A` Fragen 60–61; „ja“ in `23#1057E` |
| N-WA-012 | Der Spieler kann Zeuge einer Straftat werden; er muss nicht melden, kann Schweigegeld bekommen oder bedroht werden: „ja kann er, aber er muss nicht petzen, vielleicht bekommt er auch schweigegeld, oder es wird versucht ihn zu töten“ | 📋 | `23#1056A` Frage 48; Antwort in `23#1057E` |
| N-WA-013 | Die Polizei weiß nicht magisch alles: kein sofortiger Fahndungsstern, nur weil irgendwo ohne Zeugen etwas passiert | 📋 | `23#1056A` Frage 59; „ja genau, wenn man verdächtigt wird kann es zu einem verhör kommen und so weiter“ in `23#1057E` |
| N-WA-014 | Verkleidung wirkt nicht magisch („keine Hitman-Magie“): Ausweis, Qualität der Fälschung, Verhalten, Wissen und Gespräche zählen; schlechte Fälschung kann auffallen; falsches Verhalten erzeugt Verdacht | 📋 | `23#1088A` Fragen 57–67; „41-60 super“ und „61-67 ja“ in `23#1089E`; `23#1090A` ‚Verkleidung‘ |
| N-WA-015 | NPCs reagieren unterschiedlich darauf, wo der Spieler sich aufhält (Trespassing) | 📋 | `23#1088A` Frage 59; „41-60 super“ in `23#1089E` |
| N-WA-016 | Das öffentliche Wissen über das Übernatürliche ist regional verschieden und kann sich ändern; es werden **Gerüchte, persönlicher Glaube, belegtes Wissen und offizielle Anerkennung** unterschieden | 📋 / 💡 | `48#147E` (Wahl); Auslegung `48#148A` |
| N-WA-017 | Die Polizei erklärt Dinge zunächst normal (Wildtier, Unfall, Mord), wenn sie das Übernatürliche nicht kennt | 📋 | `23#1056A` Frage 116; „ja so wie in der serie“ in `23#1057E` |
| N-WA-018 | NPCs reagieren unterschiedlich auf Übernatürliches (Unglaube, Angst, Religion, Neugier, Verdrängung); manche glauben nichts, andere an Religion, Geister, Aliens, Verschwörung oder tatsächlich an Monster | 📋 | `23#1056A` Frage 117; „genau“ in `23#1057E`; `23#1058A` ‚Supernatural-Reaktion‘ |
| N-WA-019 | Tiere reagieren teilweise früher auf Übernatürliches als Menschen | 📋 | `23#1056A` Frage 119; „ja“ in `23#1057E` |
| N-WA-020 | Begegnungen können NPCs dauerhaft verändern | 📋 | `23#1056A` Frage 118; „ja“ in `23#1057E` |
| N-WA-021 | NPCs bemerken, wenn sie beobachtet, belauscht oder verfolgt werden (in einer Bewertungstabelle des Assistenten zu GTA-VI-Ideen als ‚unbedingt‘ eingestuft, ohne einzelne Antwort des Entwicklers) | 💡 | `23#510A` Tabelle (Vorschlag des Assistenten); stützt sich auf `M-PD` |

---

## IN: Informationsfluss, Gerüchte, Kommunikation

Gruppe zu [Modul 07](07-Informationsfluss-Geruechte-und-Kommunikation.md).

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

## BE: Beziehungen: Modell und Wirkung

Gruppe zu [Modul 08](08-Beziehungen-Modell-und-Wirkung.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-BE-001 | Interne Beziehungswerte wie Vertrauen, Angst, Respekt und Misstrauen **können existieren**, werden aber **nicht als Zahlen oder Balken** angezeigt | 📋 | `M-PD` ‚NPCs and AI‘; `23#1056A` Frage 40; „ja haben wir schon gesagt“ in `23#1057E`; `23#53A`, Zustimmung `23#54E` |
| N-BE-002 | Beziehungen kommen zum Ausdruck über Ton, Wortwahl, Körpersprache, Zusammenarbeit, Informationen, Zugang, Gefallen und Folgen | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-BE-003 | Weitere interne Größen neben den vier genannten: Zuneigung, Anziehung, Vertrautheit, Bindung, Konflikt, jüngste Ereignisse, Beziehungsstufe | 📋 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8; [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §3 |
| N-BE-004 | Anziehung, Vertrauen, Zuneigung, Komfort und Vertrautheit entwickeln sich unabhängig voneinander; emotionale Bindung ist von Anziehung und Vertrauen getrennt | 📋 | [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §3 (Entwickler: ja) |
| N-BE-005 | Beziehung bildet sich durch Qualität der Interaktion, gemeinsame Erlebnisse, Vertrauen, Aufmerksamkeit, Konflikt, Kommunikation und Zeit; gemeinsame Stille, Autofahrt, Fernsehen, Lagerfeuer, Recherche oder Rückfahrt formen Beziehung | 📋 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8; `M-PD` ‚Smartphone …‘ (‚Long vehicle travel can contain dynamic companion conversation/relationship content‘) |
| N-BE-006 | Nicht jede ignorierte Nachricht ist ein Malus; wichtige wiederholte Ignorierung kann je nach Charakter reagieren | 📋 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8 |
| N-BE-007 | Beziehungen verändern sich langfristig (Freundschaften, Beziehungen, Konflikte, Trennungen) | 📋 | `23#1056A` Frage 7; „ja“ in `23#1057E` |
| N-BE-008 | Der Spieler kann mit vielen wiederkehrenden NPCs echte Freundschaften entwickeln; sie entwickeln sich natürlich durch gemeinsame Erlebnisse, Gespräche, Hilfe, Zuverlässigkeit und Streit | 📋 | `23#1088A` Fragen 1–3; „1-3 ja“ in `23#1089E` |
| N-BE-009 | Beziehungen zwischen NPCs persistieren; Haushalte, Besitz, Betreiber und Nachbarschaften verändern sich | 📋 | `23#1056A` Fragen 106–111; „ja“ in `23#1057E` |
| N-BE-010 | Beziehungsstufen: aktive Beziehung, enge Bindung, lockere Verbindung, frühere Beziehung, gelegentlicher Kontakt, praktisch vergessen | 📋 | `35#268A` Frage 30; „16-30 ja“ in `35#269E` |
| N-BE-011 | Vertrauen kann sich deutlich schneller ändern als tiefe Bindung; Anziehung schwankt, ohne dass die Beziehung zerbricht; Komfort wächst durch wiederholt positive Interaktion; Vertrautheit macht Interaktionen weniger ‚awkward‘ | 📋 | `35#268A` Fragen 21–24; „16-30 ja“ in `35#269E` |
| N-BE-012 | Beziehungshistorie beeinflusst, wie schnell Missverständnisse verziehen werden; nach mehreren schlechten Erfahrungen wird ein NPC dauerhaft vorsichtiger, nach guten offener; zwei NPCs können auseinanderdriften | 📋 | `35#268A` Fragen 26–29; „16-30 ja“ in `35#269E` |
| N-BE-013 | Vergebung hängt von Persönlichkeit, Schwere des Ereignisses, Zeit und späterem Verhalten ab; manche Dinge verzeihen manche NPCs nie | 📋 | `35#270A` Fragen 49–50; „22-52 ja“ in `35#271E` |
| N-BE-014 | Hass oder Groll kann neben Anziehung bestehen; Ex-Partner können später wieder Casual werden oder die Beziehung kann erneut entstehen; eine zweite Beziehung verläuft anders, weil beide die erste erinnern | 📋 | `35#270A` Fragen 44–48; „22-52 ja“ in `35#271E` |
| N-BE-015 | Trennungen speichern unterschiedliche Gründe statt ‚Beziehung beendet‘; ein NPC kann selbst Schluss machen | 📋 | `35#270A` Fragen 41–43; „22-52 ja“ in `35#271E` |
| N-BE-016 | Ein Relationship-Event kann gemeinsame Freunde beeinflussen; Freundeskreise können Wissen über Beziehungen verbreiten | 📋 | `35#270A` Fragen 51–52; „22-52 ja“ in `35#271E` |
| N-BE-017 | Beziehungen reagieren auf den Lebensstil (häufiges Verschwinden, Kriminalität, ständige Verletzungen); ein normaler Partner weiß anfangs nichts vom Hunter-Leben; der Spieler entscheidet, ob und wann er die Wahrheit sagt; die Reaktion reicht von Unglauben über Angst bis Akzeptanz | 📋 | `23#1088A` Fragen 11–14; „11-15 genau“ in `23#1089E` |
| N-BE-018 | Begleiter und Hunter erinnern, ob der Spieler sie gerettet, im Stich gelassen, belogen oder einen Fall gut gelöst hat; das beeinflusst, ob sie nachts mitkommen. Dabei hängt es **nicht nur** von einer sichtbaren Beziehungsanzeige ab | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-BE-019 | Vertrauen beeinflusst Zusammenarbeit von Begleitern ohne sichtbaren Balken | 📋 | `23#1088A` Frage 20; „16-24 ja“ in `23#1089E` |
| N-BE-020 | **Gruppenruf ≠ individuelle Beziehung**: man kann bei einer Gruppe allgemein unbeliebt sein und ein Mitglied als besten Freund haben, und umgekehrt | 🧭 | `23#678A` (‚Zwei unterschiedliche Beziehungen gleichzeitig‘, Vorschlag des Assistenten); gestützt durch `48#153E` (Wahl D „D plus noch extras“: guter Ruf unter Huntern bei gleichzeitiger Fahndung) |
| N-BE-021 | Jeder wichtige NPC kann eine eigene Meinung über den Spieler entwickeln; es gibt Hunter-Reputation, Polizei, Monster, regionale Bekanntheit | 📋 | `48#153E` (Wahl); Auffächerung `48#154A` (💡) |
| N-BE-022 | Zwischen Fraktionen gelten Beziehungen wie Verbündet, Freundlich, Neutral, Angespannt, Feindlich, Krieg; diese Beziehungen verändern sich | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-BE-023 | Ein Companion kann auf Handlungen unterschiedlich reagieren, kommentieren und verweigern (eigene Moral) | 📋 | `23#1088A` Frage 19; „16-24 ja“ in `23#1089E` |
| N-BE-024 | Das Datenmodell für Kommunikationsstil und Beziehungsfolgen ist offen | ❓ | `M-OD` |

---

## BV: Beziehungsentwicklung

Gruppe zu [Modul 09](09-Beziehungsentwicklung-Freundschaft-Feindschaft-und-Vertrauen.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-BV-001 | Beziehungen entstehen durch gemeinsame Erlebnisse, Gespräche, Hilfe, Zuverlässigkeit, Streit und Zeit | 📋 | `23#1088A` Frage 3; „1-3 ja“ in `23#1089E`; [Gameplay-Systeme](../Gameplay-Systeme.md) §8 |
| N-BV-002 | Hunter-NPCs: andere Hunter treffen → zusammenarbeiten → Vertrauen entsteht → Freundschaft oder Rivalität → später möglicherweise als Begleiter verfügbar | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-BV-003 | Hunter-NPCs können nicht nur helfen, sondern auch Rivalen werden, korrumpiert oder besessen werden oder dauerhaft sterben | 📋 | „Jäger-NPCs: können nicht nur helfen, sondern auch Rivalen werden, korrumpiert/besessen werden oder dauerhaft sterben“ in `19#4E` |
| N-BV-004 | Beziehung zu Begleitern beeinflusst, ob sie nachts mitkommen; sie erinnern sich an Rettung, Im-Stich-Lassen, Lüge, gemeinsame Fälle und Verletzungen wegen Entscheidungen | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-BV-005 | Wer als Hunter-Rekrut ins HQ kommt, wird nicht automatisch von allen akzeptiert; es entstehen Reaktionen der Crew, Konflikte und Vertrauensentwicklung; manche akzeptieren ihn nie ganz | 🧭 | `23#678A` (Abschnitt ‚Nicht jeder Recruit passt automatisch ins HQ‘, Vorschlag des Assistenten) |
| N-BV-006 | Vertrauensverlust, Untreue, Eifersucht, Streit, unterschiedliche Lebensziele und das Hunter-Leben können Beziehungen scheitern lassen | 📋 | `23#1088A` Frage 6; „6-9 ja“ in `23#1089E` |
| N-BV-007 | Partner reagieren auf Lebensstil: Verschwinden, Kriminalität, ständige Verletzungen | 📋 | `23#1088A` Frage 11; „11-15 genau“ in `23#1089E` |
| N-BV-008 | Der Spieler kann Mehrfachbeziehungen führen; ob sie geheim, offen oder akzeptiert sind, beeinflusst die Konsequenzen; Eifersucht, Streit, Vertrauensverlust und Trennung können entstehen | 📋 | `23#1090A` ‚Beziehungen & Sexualität‘; Details [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §3 |
| N-BV-009 | Zusammenziehen, Heirat und gemeinsame Immobilien sind möglich | 📋 | `23#1088A` Fragen 7–9; „6-9 ja“ in `23#1089E` |
| N-BV-010 | Missverständnisse können geklärt werden; falsche Anschuldigungen können Beziehungen beschädigen; ein tatsächlicher Betrug senkt Vertrauen stark, beendet aber nicht automatisch jede Beziehung | 📋 | `35#270A` Fragen 36–40, 59–60; „22-52 ja“ und „54-60 ja“ in `35#271E` |
| N-BV-011 | Eifersucht ist individuell; ein sehr eifersüchtiger NPC reagiert schon auf Flirten, ein wenig eifersüchtiger ignoriert vieles, solange vereinbarte Grenzen nicht überschritten werden | 📋 | `35#270A` Fragen 33–35; „22-52 ja“ in `35#271E` |
| N-BV-012 | Ein NPC kann Betrug auf verschiedene Weise herausfinden (selbst sehen, Freunde, Nachrichten, Gerüchte, Geständnis); die Sicherheit der Information spielt eine Rolle (Gerücht ≠ Beweis) | 📋 | `35#270A` Fragen 36–37; „22-52 ja“ in `35#271E` |
| N-BV-013 | Ein NPC kann den Spieler zu Unrecht verdächtigen; der Spieler kann versuchen, Missverständnisse aufzuklären | 📋 | `35#270A` Fragen 38–39; „22-52 ja“ in `35#271E` |
| N-BV-014 | Geheime Untreue kann langfristig unentdeckt bleiben oder Jahre später durch neue Informationen auffliegen | 📋 | `35#272A` Fragen 22–23; „1-50 passt alles“ in `35#273E` |
| N-BV-015 | Ein NPC kann nach Trennung eine neue Beziehung beginnen und behält Erinnerungen an den Ex; Ex-Beziehungen beeinflussen spätere Beziehungen indirekt; schlechte Behandlung macht vorsichtiger, gute erhöht Erwartungen; eine gute neue Beziehung kann alte negative Muster teilweise abbauen; das ist nicht garantiert | 📋 | `35#272A` Fragen 24–30; „1-50 passt alles“ in `35#273E` |
| N-BV-016 | Der Tod eines Partners kann langfristige Auswirkungen haben; Trauer ist unterschiedlich stark und lang; ein Charakter kann später neue Beziehungen eingehen | 📋 | `35#272A` Fragen 35–37; „1-50 passt alles“ in `35#273E` |
| N-BV-017 | Eine sehr lange Beziehung prägt stärker als viele kurze Begegnungen | 📋 | `35#272A` Frage 4; „1-50 passt alles“ in `35#273E` |
| N-BV-018 | NPCs bauen auch ohne den Spieler neue Beziehungen und ein neues Leben auf | 📋 | `35#272A` Frage 16; `35#274A`; „1-50 passt alles“ in `35#273E`; [Modul 02](02-Leitsaetze-und-Abgrenzung.md) N-LT-006 |
| N-BV-019 | Persönliche Item- und Gefallen-Wünsche von Crewmitgliedern (Buch, Werkzeug, Schallplatte, Ersatzteil, Waffe, Foto) führen nach Erfüllung zu persönlichem Dialog und Erinnerung; sie sind nicht wegen absurder Zeitfenster verpassbar | 🧭 | `23#606A` Abschnitt 5 (Vorschlag des Assistenten); `M-PD` ‚Acts, chapters and controlled story reactivity‘ (Crew personal requests) |
| N-BV-020 | Crew-Aktivitäten (gemeinsam trainieren, essen, Billard, Bar, Rennen, Schießstand, Angeln, Jagd, Kino, Roadtrip, Auto reparieren, Recherche, kleinere Jagd, einkaufen, Sport) formen Beziehung; sie sind nicht unnötig verpassbar | 🧭 | `23#603A` Abschnitt 3 (Vorschlag des Assistenten) |
| N-BV-021 | Road-Trip-Gespräche im Fahrzeug sind ein wichtiger Kanal der Charakterentwicklung und der Beziehung | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; ‚Smartphone / social media …‘ |
| N-BV-022 | Konsequenzen bei Fraktionen: Mehrfachmitgliedschaft ist möglich, aber nicht folgenlos (zum Beispiel zwei rivalisierende Clubs oder Polizei und Gang); große Folgen brauchen große Ursachen | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-BV-023 | Aufhören mit einer Gruppe: freiwillig, Rauswurf, Suspendierung, Verrat, Zerfall der Gruppe; die Vergangenheit hat sichtbare Folgen (zum Beispiel bleibt ein Tattoo) | 🧭 | `23#676A` (Vorschlag des Assistenten) |

---

## FA: Familien, Haushalte, Netzwerke

Gruppe zu [Modul 10](10-Familien-Haushalte-und-soziale-Netzwerke.md).

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

## NE: Nemesis: Modell und Datensatz

Gruppe zu [Modul 11](11-Nemesis-Modell-und-Datensatz.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-NE-001 | **Nemesis und Rivalität (C):** Überlebende Gegner können zurückkehren, persönliche Feindschaften entwickeln, aus früheren Begegnungen lernen und ihre Taktik anpassen; das gilt ausdrücklich auch für intelligente übernatürliche Kreaturen | 📋 | `PE-N1022` C |
| N-NE-002 | Intelligente Monster haben ein **umfassendes individuelles Gedächtnis** mit Beziehungen, Rache, Angst und Informationsaustausch | 📋 | „Supernatural Monster-Gedächtnis: Umfassendes individuelles Gedächtnis mit Beziehungen, Rache, Angst und Informationsaustausch“ in `48#139E` |
| N-NE-003 | Das Monstergedächtnis umfasst Begegnungen, Kampferfahrungen (Waffen, Fallen, Strategien), emotionale Reaktionen, Beziehungen, Informationsaustausch und langfristige Entwicklung | 💡 (Auslegung der 📋 Wahl) | `48#140A` |
| N-NE-004 | Monster reagieren taktisch auf vorbereitete Fallen: erkennen, untersuchen, umgehen, sabotieren, täuschen, Gegenstrategien entwickeln; die Mythologie gilt weiter (ein vollständig gefangener Dämon entkommt nicht durch KI) | 📋 / 💡 | „Supernatural Monster-KI bei Fallen: Taktische Monster mit Sabotage und Gegenstrategien“ in `48#137E`; Auslegung `48#138A` |
| N-NE-005 | Ein überlebendes oder geflohenes Wesen kann später mit Erinnerung und früheren Verletzungen wiederkehren; „es kann auftauchen aber es muss nicht“ | 📋 | `23#293A` Punkt 20; „es kann auftauchen aber es muss nicht“ in `23#294E` |
| N-NE-006 | Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf; manche beobachten den Spieler und greifen zu einem besseren Zeitpunkt an; sehr gefährliche Wesen können den Spieß umdrehen und den Spieler jagen | 📋 | `23#1074A` Punkte 58–60; „55-60 ja“ in `23#1075E` |
| N-NE-007 | Das HQ und die Safehouses sind nicht absolut sicher; sehr seltene Gegner können Spielerstandorte herausfinden, wenn man unvorsichtig ist; selten, sonst nervt es | 📋 | `23#1074A` Punkt 61; „61 ja das kann etwa so sein wie bei rdr2, mit dem umziehen“ in `23#1075E` |
| N-NE-008 | Monster lernen nur aus tatsächlich **beobachteten** Informationen, nicht aus globaler Telemetrie; es hängt von Monster und Intelligenz ab | 📋 | `23#293A` Punkt 12; „12. ja aber das kommt auf das monster und die intelligenz ab“ in `23#294E`; `M-PD` ‚Stealth, perception and horror AI‘ |
| N-NE-009 | Die Monster sollen sich **anhand des Spielerverhaltens** schlau anpassen | 📋 | „7. genau, die monster sollen sich anhand von meinem verhalten schlau anpassen“ in `23#254E` |
| N-NE-010 | Besonders intelligente Gegner passen sich an wiederholte Taktiken an | 📋 | „55-60 ja“ in `23#1075E` |
| N-NE-011 | Einzelwesen haben persistente Identität: Name, Alter, Gebiet, Beziehungen, Opfer, Verletzungen, Feinde, Vergangenheit; benannte Wesen haben eigene Archivseite, mehrere Fälle und Storyrelevanz | 📋 | `23#1076A`; „31. ja kann sein, sowie bei leviathanen“ in `23#1075E`; „29. cool“ in `23#308E` |
| N-NE-012 | Monster respawnen nicht; tot ist tot (Ausnahme: Befreiung aus dem Fegefeuer) | 📋 | `23#1075E` Punkte 30, 37; `M-B09` |
| N-NE-013 | Narben und Verletzungen von Wesen bleiben bestehen; zurückkehrende Individuen zeigen die Folgen früherer Kämpfe | 📋 | [Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §6; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §2 L4 |
| N-NE-014 | Menschliche Gegner können fliehen, erstarren oder sich ergeben, wenn sie genug eingeschüchtert sind | 📋 | `M-PD` ‚Multiple-opponent combat, improvised actions and surrender‘ |
| N-NE-015 | Hunter-NPCs können Rivalen werden; sie haben Ruf, können vor dem Spieler an Fällen arbeiten, Fälle falsch behandeln oder konkurrieren | 📋 | `19#4E`; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §3.1 |
| N-NE-016 | Ein verschontes friedliches Wesen wird nicht automatisch zum Feind; andere Hunter können es dennoch töten | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |
| N-NE-017 | Lernen ist Wissensaufbau, nicht Stärkerwerden; kein Wert steigt mit jeder Begegnung; keine sichtbaren Stufen | 📋 | `M-PD` ‚Genre and progression identity‘; [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §6 |
| N-NE-018 | Bestimmte kriminelle Gruppen können wissen, wer der Spieler ist; wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | 📋 | `23#1056A` Fragen 43–44; „ja“ in `23#1057E` |
| N-NE-019 | Monster können dem Spieler Verbündete angreifen (Beispiel des Assistenten: der Dämon könnte ‚sich zu rächen, dich zu meiden oder gezielt deine Verbündeten anzugreifen‘) | 💡 | `48#140A` |
| N-NE-020 | Die Anpassung von Wesen ist durch Artregeln und Schwächen des Bestiariums begrenzt, nie durch freie Erfindung | 💡 (Vorschlag, deckt sich mit den Quellen der Kreaturen) | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §4 Punkt 4; [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §14 |
| N-NE-021 | Wiederkehrende Rivalen können im Underground-Fight-Club entstehen (zusammen mit Beziehungen, Trainern, Reputation, Hunter-Kontakten) | 💡 | `23#164A`, `23#166A` (Vorschläge des Assistenten); „ja das es nicht ufc 2.0 ist ist klar.genau so will ich das haben“ in `23#165E` |
| N-NE-022 | Wie ein Nemesis lernt (Regeln oder Daten), welche Historie gespeichert wird, wie häufig er auftritt, was der Spieler von der Feindschaft erkennen kann | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11 |

---

## NL: Nemesis: Lebenszyklus

Gruppe zu [Modul 12](12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-NL-001 | Monster müssen nicht bis zum Tod kämpfen; sie fliehen, wechseln den Bau oder das Opfer, verstecken sich, suchen Verstärkung | 📋 | `23#293A` Punkt 13; „13. ja das sowieso“ in `23#294E` |
| N-NL-002 | Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf; manche beobachten den Spieler und greifen später an | 📋 | `23#1074A` Punkte 58–59; „55-60 ja“ in `23#1075E` |
| N-NL-003 | Ein Wesen, das nur vertrieben wurde oder entkam, kann später mit Erinnerung und früheren Verletzungen auftauchen; es muss nicht | 📋 | `23#294E`: „es kann auftauchen aber es muss nicht“ |
| N-NL-004 | Der Spieler entscheidet, ob er ein Wesen tötet oder freilässt; Ausgänge reichen von töten, vertreiben, bannen, fangen, heilen, Fluch brechen bis verhandeln und retten | 📋 | `23#293A` Punkt 15; `23#294E` Punkt 15; `M-PD` ‚Story and cases‘ |
| N-NL-005 | Gefangene Wesen können verhört und verwahrt werden; Verhandeln und Kooperation sind möglich | 📋 | `48#129E`; [Kreaturen 13](../Kreaturen/13-Lebendfang-Fallen-und-Containment.md) |
| N-NL-006 | Bleibende Verletzungen und Narben von Wesen bleiben; Regeneration ist artabhängig | 📋 | [Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §5–§6 |
| N-NL-007 | Bei Menschen: Verletzte NPCs werden behandelt (EMS, Krankenhaus) und genesen; schwere Verletzungen haben bei wichtigen NPCs dauerhafte Folgen (große Narben, verlorene Hand) | 📋 | `23#1056A` Fragen 71–72, 100–101; „ja sicher, große narben, eine hand verloren, alles soll es geben“ in `23#1057E` |
| N-NL-008 | Menschliche Gegner können fliehen, erstarren oder sich ergeben | 📋 | `M-PD` ‚Multiple-opponent combat …‘ |
| N-NL-009 | Safehouses sind nicht absolut sicher; bestimmte sehr seltene Gegner können Spielerstandorte herausfinden; selten | 📋 | `23#1074A` Punkt 61; Antwort in `23#1075E` |
| N-NL-010 | Gegner kehren verändert zurück (Narben, besser vorbereitet, mit Verbündeten, wissend, was gewirkt hat) | 📋 Richtung | `PE-N1022` C; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §3 (Zielbild, 💡) |
| N-NL-011 | Die Weltzeit läuft nur während des Spielens; Zeitsprünge (Schlaf, Reise, Haft) werden von der Hintergrundsimulation ereignisbasiert nachgeholt | 📋 | `35#279E` Punkt 11 und 12–35; [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §8 |
| N-NL-012 | Festnahme des Spielers ist kein Game Over; Folgen sind Verhör, Geldstrafe, Kaution, Gerichtsverfahren, Haft, Zeitsprung | 📋 | `23#1058A` ‚Festnahme ≠ Game Over‘; „68. ja genau, game over soll es nicht geben, das wäre bei so einem spiel zu brutal“ in `23#1057E` |
| N-NL-013 | Der genaue Ablauf des Spielertods bzw. des Scheiterns ist offen | ❓ | `M-PD` ‚Death / failure — OPEN‘ |
| N-NL-014 | Hauptbedrohungen überleben mehrere Begegnungen, entwickeln Strategien und treten über Akte auf | 📋 | `M-PD` ‚Long-term story threats‘ |
| N-NL-015 | Wiederkehrende Rivalen: Kämpfer der Kampfszene, Trainer, Reputation, Kontakte | 💡 | `23#164A`, `23#166A` |

---

## LE: Autonomes Leben

Gruppe zu [Modul 13](13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md).

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

## LB: Lebensereignisse, Alterung, Tod

Gruppe zu [Modul 14](14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md).

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

## WE: Weltereignisse und Übernatürliches

Gruppe zu [Modul 15](15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md).

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

## FR: Fraktionen: Modell, Mitgliedschaft, Hierarchie

Gruppe zu [Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-FR-001 | **Autonome Fraktionen und Organisationen (D):** Monstergruppen, Hunter-Organisationen, kriminelle Netzwerke, Behörden und andere Gruppen verfolgen eigene Ziele, bilden Allianzen, beanspruchen Gebiete, bekämpfen einander und ändern ihre Strategie | 📋 | `PE-N1022` D |
| N-FR-002 | Optionale Mitgliedschaften neben der Hunter-Crew: Motorradclub, Racing Club, Gun Club, Jagdverein, Kampfszene, kriminelle Organisation, religiöse oder okkulte Gruppe, politische oder ideologische Gruppe, extremistische Organisation, weitere lokale Gruppierungen. Die Hunter-Crew bleibt das eigentliche Zuhause | 🧭 | `23#676A` (Vorschlag des Assistenten); Entwickler: „ich würde die ganze mc oder extremisten sachen alle optional machen“ in `23#677E` |
| N-FR-003 | Die optionalen Fraktionen sind für die Hauptstory **nicht nötig**; „es können sich nur die dialoge etwas ändern“; man kann auch ein Spitzel sein; nach Abschluss der Hauptstory kann man dort weitermachen | 📋 | `23#677E`; `23#678A` (‚Faction-System klar getrennt von der Hauptstory‘) |
| N-FR-004 | Clubs, Sekten und Extremisten können rekrutiert werden und dann im Hunter-HQ sein: „die clubs, sekten extremisten können möglicherweise auch rekrutiert werden und dann in der richtigen hq sein“; das kann etwas zur Story beitragen, wenn man den Charakter bei einer Mission zusätzlich verfügbar hat | 📋 | `23#677E` |
| N-FR-005 | Die Fraktionen werden **sehr realistisch** nachgebildet; es soll unbegrenzten Zusatzinhalt geben (Wortlaut mit Tippfehler: ‚uimimitiert extra content‘), auch nach der Story, mit Aufgaben und der Möglichkeit, zum Leader aufzusteigen | 📋 | „ich würde das sehr realistisch nachmachen“ und „man kann vielleicht zum leader aufsteigen“ in `23#679E` |
| N-FR-006 | Das Fraktionssystem ist **extrem tief**, die Hauptstory davon praktisch unabhängig; Fraktionen schalten optional frei: Nebencontent, World Events, spezielle Jobs, Beziehungen, Shops, Fahrzeuge, Kleidung, Safehouses/Clubhäuser, Informationen, Kontakte | 🧭 | `23#676A` Abschnitt ‚Sehr wichtig für unsere Hauptstory‘; `23#678A` (Vorschlag des Assistenten) |
| N-FR-007 | Es gibt **mehrere eigene kriminelle Organisationen** statt generischer Gang-NPCs, einschließlich extremistischer Gruppen (nach dem Planungsstand von Block 8) | 📋 | `23#1056A` Frage 52; Antwort in `23#1057E`; `23#1058A` ‚Kriminelle Organisationen‘ |
| N-FR-008 | Kriminelle Gruppen haben Einflussbereiche, aber nicht zwingend bunte Gangflächen; auf der Karte bunt nur dann, wenn man selbst in einer solchen Gruppe ist („nur wenn man selber in einer dieser gangs ist, dann kann man mehr gebiet erobern“) | 📋 | `23#1056A` Frage 53; Antwort in `23#1057E`; `23#1058A` ‚Territorien‘ |
| N-FR-009 | Illegale Geschäfte (Chop Shops, Underground-Casinos, illegale Rennen, Fight Clubs, Schmuggel) gehören zur Welt | 📋 | `23#1056A` Frage 54; „ja“ in `23#1057E` |
| N-FR-010 | Nicht jeder Motorradclub ist eine kriminelle Gang: normaler MC, Outlaw MC, extremistischer MC, Veteranen- oder lokaler MC unterscheiden sich | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-FR-011 | Hierarchie ist mehr als ein Ruf-Balken: je Gruppe eigene Ränge (Beispiel MC: Prospect, Member, Patch Member, Sergeant-at-Arms, Road Captain, Vice President, President); **Rang ≠ nur Level**; Aufstieg hängt ab von Vertrauen, geleisteten Jobs, Loyalität, Beziehungen zu Mitgliedern, internen Abstimmungen, Fähigkeiten, Verhalten, Entscheidungen | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-FR-012 | Führung hängt ab von langer Mitgliedschaft, Vertrauen, Beziehungen, Aufgaben, wichtigen Entscheidungen, Einfluss und Unterstützung; nicht „1000 XP gesammelt → jetzt Präsident“; manche Gruppen stimmen demokratisch ab, andere sind autoritär oder folgen Nachfolge, Machtkampf oder Ernennung | 🧭 | `23#680A` (Vorschlag des Assistenten) |
| N-FR-013 | Der Spieler bekommt in der Gruppe **tatsächliche Jobs**, keine kosmetische Mitgliedschaft | 🧭 | `23#676A` |
| N-FR-014 | Mitglieder haben untereinander Beziehungen: Leader mag den Spieler, der VP misstraut ihm, Mitglied A ist enger Freund, Mitglied B hasst ihn, Mitglied C schuldet einen Gefallen; daraus entsteht interne Politik | 🧭 | `23#676A` |
| N-FR-015 | Hierarchiekämpfe sind vorgesehen: Führungswechsel, interne Streitigkeiten, Ausschlüsse, Verrat, Abspaltungen, Austritte, neue Chapter, Fusionen, rivalisierende Führungsfraktionen; ein Teil davon passiert über den World State, nicht über Hauptmissionen | 🧭 | `23#676A` |
| N-FR-016 | Der Spieler kann mehreren Gruppen angehören, aber nicht folgenlos (zwei rivalisierende MCs, Polizei-nahe Organisation und Gang) | 🧭 | `23#676A` |
| N-FR-017 | Beziehungen zwischen Fraktionen: Allied, Friendly, Neutral, Tense, Hostile, War; sie verändern sich | 🧭 | `23#676A` |
| N-FR-018 | Mitgliedschaft kann enden: freiwillig, Rauswurf, Suspendierung, Verrat, Zerfall der Organisation, später eventuell eigene Gruppe; nicht jede falsche Dialogantwort zerstört 50 Stunden Inhalt | 🧭 | `23#676A` |
| N-FR-019 | Drei Wege: ehrliche Mitgliedschaft, Infiltration/Spitzel (mit Konflikt zwischen Loyalitäten), lose Verbindung ohne formelle Mitgliedschaft | 🧭 | `23#678A` (Vorschlag des Assistenten); „man kann theoretisch auch ein spitzel sein“ in `23#677E` |
| N-FR-020 | Eine Faction-Questline kann Dutzende Stunden zusätzlichen Inhalt haben und vollständig optional sein | 🧭 | `23#678A` |
| N-FR-021 | Rekrutierte Faction-NPCs brauchen echte Fähigkeiten, nicht nur Aussehen (Ex-MC-Mechaniker, ehemaliges Sektenmitglied, Racer, Arzt …); nicht jeder Rekrut passt automatisch ins HQ | 🧭 | `23#678A` |
| N-FR-022 | Der Spieler kann bei geeigneten Gruppen die Führung übernehmen und bekommt dann Mitgliederverwaltung, Clubhaus, Jobs, Ressourcen, Events, Rivalitäten – **ohne Strategiespiel-Micromanagement** | 🧭 | `23#678A`; `23#680A` |
| N-FR-023 | Fraktion-Reputation ist nicht dasselbe wie individuelle Beziehung (siehe [Modul 08](08-Beziehungen-Modell-und-Wirkung.md) N-BE-020) | 🧭 | `23#678A` |
| N-FR-024 | Extremistische Gruppen: Menschen mit unterschiedlichen Motiven (Fanatiker, Opportunist, Familie/Freunde, Krimineller, junger Rekrut, älteres Mitglied, Aussteiger, Zweifler, Führungsperson, die Ideologie für Macht benutzt); das Spiel zeigt Glauben, Tun, Behandlung anderer und Folgen und überlässt die Bewertung dem Spieler; die Strukturen sind eigene fiktive | 🧭 | `23#676A`, `23#678A`, `23#680A` (Vorschläge des Assistenten); siehe N-LT-029, N-LT-030 |
| N-FR-025 | Monster bilden Rudel, Clans, Hierarchien mit Anführern, Untergebenen, Rivalen und Nachfolgern, Territorien, Machtkämpfe, Allianzen; sie tauschen Wissen über Hunter aus; Veränderungen bleiben im Spielstand | 📋 | „Supernatural Monster-Gesellschaft: Organisationen, Hierarchien, Territorien und Machtkämpfe“ in `48#141E`; Tabelle `48#142A` (💡) |
| N-FR-026 | Es gibt mehrere Hunter-Organisationen, ein loses Netz und eine wissenszentrierte Hauptorganisation mit eigenem Namen; kleine Behördeneinheiten kennen Teile der Wahrheit; die konkrete Fraktionsliste ist offen | 📋 / ❓ | `M-B09`; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §4–§5 |
| N-FR-027 | Die Hunter-Crew lebt im HQ; optionale Rekruten kommen von außen; die Hauptstory bleibt unabhängig | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; `23#677E` |
| N-FR-028 | Online/Koop: Fraktionsmitgliedschaft und Fortschritt des Spielers gehören zum Host-Speicherstand | 📋 | `CLAUDE.md` §6 |
| N-FR-029 | Die konkrete Liste der Fraktionen, ihre Namen, Ziele und Besetzung sind nicht festgelegt | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11; `M-B09`; `M-OD` |

---

## TE: Fraktionen: Territorien, Konflikte, Macht

Gruppe zu [Modul 17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-TE-001 | Monstergruppen beanspruchen Gebiete und verteidigen sie; Machtkämpfe bestehen aus internen Konflikten, Führungswechsel und Abspaltungen; Allianzen: Gruppen können zusammenarbeiten oder sich bekämpfen | 📋 | „Supernatural Monster-Gesellschaft: Organisationen, Hierarchien, Territorien und Machtkämpfe“ in `48#141E`; Tabelle `48#142A` (💡) |
| N-TE-002 | **Umfassende Territoriumssimulation mit Ressourcen und regionaler Entwicklung:** Eroberung, Verteidigung, Expansion, Gebietsverluste; Ressourcen (Nahrung, Verstecke, sichere Rückzugsorte, artspezifische Bedürfnisse); Populationen (Wachstum, Verluste, Wanderungen); strategische KI (Expansion, Rückzug, Konflikte); Machtverhältnisse; regionale Folgen (Angriffe, Vermisstenfälle, NPC-Verhalten, Sicherheitslage); Persistenz im Host-Spielstand | 📋 | „Supernatural dynamische Monsterterritorien: Umfassende Territoriumssimulation mit Ressourcen und regionaler Entwicklung“ in `48#143E`; Tabelle `48#144A` (💡) |
| N-TE-003 | Diese Entwicklungen geschehen auch ohne direkte Beteiligung des Spielers; Rudel A kann Rudel B bedrängen, wenn es mehr Mitglieder und Ressourcen hat; der Ausgang hängt auch von Führung, Verletzungen, Jagderfolg und dem Eingriff des Spielers ab | 💡 (Beispiel) | `48#142A`, `48#144A` |
| N-TE-004 | Entfernte Regionen werden vereinfacht, aktive Gebiete detailliert simuliert | 💡 (Hinweis des Assistenten) | `48#144A` ‚Technisch wichtig‘ |
| N-TE-005 | Der Tod eines Rudelführers lässt das Rudel nicht verschwinden: Nachfolger, Machtkampf oder Übernahme des Territoriums durch ein Nachbarrudel | 💡 (Beispiel) | `48#142A` |
| N-TE-006 | Territorien menschlicher Gruppen: kriminelle Gruppen haben Einflussbereiche; Geschäfte, rivalisierende Gebiete und Einflussbereiche sind für Mitglieder sichtbar; Gebietserweiterung nur für Mitglieder | 📋 | `23#1056A` Fragen 52–54; Antworten in `23#1057E`; `23#1058A` ‚Territorien‘ |
| N-TE-007 | Gangkonflikte gehören zur unabhängig vom Spieler laufenden Kriminalität; Täter können fliehen oder festgenommen werden; die Polizei reagiert auch ohne Spieler | 📋 | `23#1056A` Fragen 46, 49; „ja schon, sehr komplex“ und „ja sicher“ in `23#1057E` |
| N-TE-008 | Viertel unterscheiden sich in Art und Häufigkeit der Kriminalität, nicht als simple Zonen; Faktoren: Armut, Arbeitslosigkeit, organisierte Kriminalität, Drogenmärkte, Infrastruktur, Polizeipräsenz, Stadtentwicklung, soziale Stabilität; Ethnie ist keine mechanische Ursache | 📋 | `23#1056A` Frage 47; `23#1058A`; [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13 |
| N-TE-009 | Kriminalität kann sich langfristig verändern (Stadtentwicklung, Wirtschaft, Polizeipräsenz) | 📋 | `23#1056A` Frage 51; „ja“ in `23#1057E` |
| N-TE-010 | Fraktions-Rivalitäten, Territorialkonflikte, Verhaftungen und Führungskrisen erzeugen Probleme ohne vorgeschriebenen Questmarker | 🧭 | `23#680A` (Vorschlag des Assistenten) |
| N-TE-011 | Gruppen können sich verbünden, verraten, spalten und fusionieren | 🧭 | `23#676A` |
| N-TE-012 | Monster tauschen Wissen über Hunter und Gefahren aus; Monster haben Beziehungen zu Menschen, anderen Monstern und Hunter-Gruppen | 📋 | `48#139E`, `48#141E`; [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §5 |
| N-TE-013 | Territorium und Wanderung folgen Nahrung, Hunter-Druck, Feuer, Überschwemmung, Rivalen, übernatürlichen Ereignissen; vollständig vernichtete Nester bleiben lange weg; Wiederbesiedlung Jahre später durch andere Individuen, Gruppen oder Arten ist möglich | 📋 | `23#1074A`/`23#1075E` Punkte 33–38; [Kreaturen 08](../Kreaturen/08-Oekologie-Populationen-und-Lebenszyklus.md) §1 |
| N-TE-014 | Eine Fraktion hält kein Territorium, das ihre Mitglieder nicht erreichen können (Konsistenzregel) | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 3 |
| N-TE-015 | Ein ignoriertes Ereignis kann zu mehr Opfern, Polizeieinsatz, Eingreifen eines anderen Hunters, Abwandern einer Kreatur oder einem späteren historischen Fall führen | 📋 | `M-PD` ‚Organic world-event / case emergence‘ |
| N-TE-016 | Hunter-Organisationen, kriminelle Netzwerke, Behörden und andere Gruppen sind Teil dieser Dynamik (Anforderung D) | 📋 | `PE-N1022` D |
| N-TE-017 | Rivalisierende Organisationen können unterschiedliche Fragmente des Wissens halten | 📋 (Planungslinie) | `CANONICAL_ADDENDUM_2026-09-05.md` (Modul 01 §6 F8) |
| N-TE-018 | Konkrete Karten, Grenzen, Gebiete und Fraktionen sind nicht festgelegt; keine Territorien auf Grundlage einer finalen Karte | ❓ | `CLAUDE.md` §3; [Modul 27](27-Konflikte-und-offene-Fragen.md) |

---

## BH: Behörden, Hunter-Organisationen, Wesen

Gruppe zu [Modul 18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-BH-001 | Es gibt mehrere Behörden (City Police, County Sheriff, State Police/Highway Patrol, Rangers usw.) mit unterschiedlichen geografischen Zuständigkeiten (Jurisdiktionen) | 📋 | `23#1056A` Fragen 57–58; „ja sicherlich“ und „ja“ in `23#1057E` |
| N-BH-002 | Die Polizei weiß nicht magisch alles; bei Verdacht kann es zu einem Verhör kommen | 📋 | `23#1056A` Frage 59; „ja genau, wenn man verdächtigt wird kann es zu einem verhör kommen und so weiter“ in `23#1057E` |
| N-BH-003 | Zeugen rufen Notruf; Täterbeschreibung (Kleidung, Fahrzeug, Kennzeichen, Richtung); Kennzeichenabfrage; Verkehrskontrollen (normale Stops oder Verfolgung, je nach Lage); Verhalten des Spielers entscheidet, ob eine Situation harmlos endet oder eskaliert | 📋 | `23#1056A` Fragen 60–64; Antworten in `23#1057E` |
| N-BH-004 | Schwere Fahndung bleibt lange bestehen, bis das Kopfgeld bezahlt, die Klage zurückgezogen oder der Beschuldigte freigesprochen wird: „sicher nicht, sie bleiben lange bestehen bis man kopfgeld bezahlt, oder die klage zurückgezogen oder man freigesprochen wird“ | 📋 | `23#1056A` Frage 65; Antwort in `23#1057E`; `23#1058A` ‚Man kann tatsächlich zum landesweit Gesuchten werden‘ |
| N-BH-005 | Bei schweren Taten folgen Ermittlungen und Detectives; Beweise (Kameras, Zeugen, Kennzeichen, Spuren) werden selektiv simuliert; Festnahme führt zu Verhör, Geldstrafe, Kaution, Gerichtsverfahren, Haft, Zeitsprung statt Game Over; Anwälte und Kaution später ausbaubar | 📋 | `23#1056A` Fragen 66–69; Antworten in `23#1057E`; `23#1058A` |
| N-BH-006 | Das Rechtssystem verfolgt tatsächliche Information und Beweissicherheit über Jurisdiktionen hinweg statt allwissender Fahndungslogik; Beweise sind Zeugen, Kameras, Kleidung, Gesicht, Fahrzeuge, Kennzeichen, Waffen/Ballistik, Fingerabdrücke/DNA, digitale Beweise; Festnahme ist eine echte Alternative zur tödlichen Polizeigewalt; Polizei-K9 und Geruchs-Wind-Modell erwünscht | 📋 | `M-PD` ‚Cash / law / evidence‘ |
| N-BH-007 | Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen; bestimmte kriminelle Gruppen können wissen, wer der Spieler ist | 📋 | `23#1056A` Fragen 43–44; „ja“ in `23#1057E` |
| N-BH-008 | Die Polizei erklärt Dinge zunächst normal; die meisten Polizisten wissen nichts vom Übernatürlichen; kleine geheime Einheiten in Polizei und Bundesbehörden wissen teilweise davon | 📋 | `23#1056A` Frage 116; „ja so wie in der serie“ in `23#1057E`; „21. ja“, „22. genau“ in `23#1075E` ([Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §5) |
| N-BH-009 | Die Polizei reagiert auch ohne den Spieler: Notruf → Beamte kommen → Täter kann fliehen oder festgenommen werden; nach größeren Verbrechen können Orte abgesperrt sein | 📋 | `23#1056A` Fragen 49–50; „ja sicher“ und „ja genau“ in `23#1057E` |
| N-BH-010 | Polizeifahndung und Strafverfolgung sind vollständig dynamisch: Zuständigkeiten, verdeckte Ermittlungen, Identitätsdiebstahl, Beweismanipulation, Korruption, Langzeitfolgen | 📋 | „D“ in `48#155E`; Auffächerung `48#154A`, `48#156A` (💡); [Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md) |
| N-BH-011 | Behörden, Hunter-Organisationen und private Sicherheitsorganisationen reagieren auf Ereignisse und passen ihr Verhalten langfristig an; es gibt geheime Hunter-Netzwerke, staatliche Sondergruppen und private Sicherheitsorganisationen | 📋 | `48#145E`; Tabelle `48#146A` (💡) |
| N-BH-012 | Mehrere Hunter-Organisationen (pragmatische Hunter, Forscher, religiöse Orden, alte Geheimorganisationen, kleine staatliche Programme, private Gruppen); eine wissenszentrierte Hauptorganisation mit eigener IP; sie weiß mehr als der normale Hunter, aber nicht alles; interne Fraktionen, Korruption, Verrat | 📋 | `23#1075E` Punkte 16–20; `M-B09`; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §4 |
| N-BH-013 | NPC-Hunter haben Ruf, können sterben, Fälle vor dem Spieler bearbeiten, falsch behandeln, konkurrieren oder kooperieren; ignorierte Fälle werden manchmal von ihnen gelöst; sie haben eigene Handlungsfreiheit | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘; `23#1074A` Punkte 11–15, 66; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §3 |
| N-BH-014 | Das Hunter-Netz ist selten; Hunter sind kein Massenphänomen | 📋 | `M-B09` (‚rare hunter network‘) |
| N-BH-015 | Menschenähnliche übernatürliche Wesen, die niemandem schaden, können ein normales, dauerhaftes Leben mit Beruf, Haus, Familie, Freunden, Routinen führen, wenn der Spieler sie verschont | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |
| N-BH-016 | Ein verschontes friedliches Wesen wird nicht automatisch zum Feind; ein anderer Hunter kann es töten, wenn es zu seinem Wissen, Charakter, seiner Ideologie und Lage passt | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |
| N-BH-017 | Die Gesellschaft reagiert auf das Übernatürliche mit Behördenermittlungen, Absperrungen, Sondereinheiten, politischen Entscheidungen, geheimen Programmen, Medienberichten, Vertuschung; die Welt entwickelt sich auch ohne den Spieler | 📋 | `48#145E`; `48#146A` (💡) |
| N-BH-018 | Staatliche Monsterforschung existiert (unterirdische Labore u. a.) und wird geheim gehalten, weil öffentliches Wissen Angst erzeugt | 📋 | `48#171E` ([Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §5; [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md)) |
| N-BH-019 | Eine Elite im Hintergrund verfolgt viele verschiedene Ziele und will manches um keinen Preis öffentlich machen; der Entwickler hält es für möglich, dass Monster teilweise von ihr stammen. Nach dem Vorschlag des Assistenten wirkt sie fast allmächtig, ist es aber nicht, und ihre wahre Struktur wird nicht von Anfang an erklärt | 📋 Richtung / 💡 | `48#179E`; Vorschlag `48#178A` |
| N-BH-020 | Bestimmte NPCs, darunter Polizisten, sind Teil der Hunter-Crew oder des Netzes (Rekrutierung aus Polizei, Militär, Medizin, Werkstatt, Wissenschaft, Okkultismus, Opfern, Zivilisten) | 🧭 | `23#678A` (Vorschlag des Assistenten) |

---

## CO: Begleiter, Crew, Rekrutierung

Gruppe zu [Modul 19](19-Begleiter-Crew-und-Rekrutierung.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-CO-001 | Es gibt einen frei erstellbaren Hauptcharakter und eine feste, familienähnliche **Hunter-Crew** aus ausgearbeiteten Figuren mit Persönlichkeit, Vergangenheit, Beziehungen, Konflikten, Tagesabläufen, Fähigkeiten, Fahrzeugen, Zimmern, Ausrüstung, Meinungen, Kontakten und Erinnerungen | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-002 | Das Spiel beginnt mit einem **Zwei-Jäger-Kern**; später werden weitere Jäger getroffen, gewonnen und können als Begleiter verfügbar werden | 📋 | `23#117A` Punkt 17; „17 ja genau“ in `23#118E`; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-003 | Menschliche Begleiter sind eigenständige intelligente Akteure, keine ferngesteuerten Puppen; sie unterscheiden sich in Kampf, Recherche, Medizin, Technik, Tracking, Persönlichkeit, Moral, Ausrüstung, Fahrzeug, Erfahrung | 📋 | `M-PD` ‚Companion autonomy, planning and field commands‘; `23#1088A` Fragen 16–18; „16-24 ja“ in `23#1089E`; `23#1090A` ‚Companions‘ |
| N-CO-004 | Begleiter haben eigene Persönlichkeit, kommentieren Entscheidungen, handeln nicht wie stumme Bots; sie können Handlungen verweigern („Nein, da mache ich nicht mit.“); Vertrauen beeinflusst Zusammenarbeit ohne sichtbaren Balken | 📋 | `23#1088A` Fragen 18–20; „16-24 ja“ in `23#1089E`; `23#1066A` Abschnitt 2 |
| N-CO-005 | Begleiter besitzen echte Waffen, Kleidung und Inventar; einige fahren mit eigenem Auto oder Motorrad; man kann sich ‚dort treffen‘ (im Wortlaut der Frage: ‚Triff mich dort‘) statt immer gemeinsam zu reisen | 📋 | `23#1088A` Fragen 21–23; „16-24 ja“ in `23#1089E` |
| N-CO-006 | Begleiter können ernsthaft verletzt werden und ausfallen; sie können den Spieler retten (Erste Hilfe, Deckung, Herausziehen) und vom Spieler gerettet werden | 📋 | `23#1088A` Fragen 24, 27–28; „16-24 ja“, „27-29 ja“ in `23#1089E` |
| N-CO-007 | **Tod:** Normale und rekrutierbare Begleiter können dauerhaft sterben („würde ich schon machen außer bei den richtigen aus dem hunter hq“); Kernfiguren des Hunter-HQ haben keinen zufälligen Sandbox-Tod; stirbt eine Hauptfigur laut Story, bleibt sie tot („ja wenn es ein story tod ist dann ist er tod“) | 📋 | `23#1088A` Fragen 25–26; Antworten in `23#1089E`; `23#1090A` ‚Tod‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-008 | Mehrere Begleiter (2–3) sind für manche Einsätze möglich, aber kein ständiges Riesen-Squad; Solo bleibt möglich; Storymissionen dürfen mit Begleiter geschrieben sein: „die story missionen können mit begleiter sein“ | 📋 | `23#1088A` Fragen 29–30; „27-29 ja“ und Antwort 30 in `23#1089E` |
| N-CO-009 | Der Spieler kann entscheiden, ob er einen verfügbaren Begleiter mitnimmt, auch einen schwächeren oder unerfahrenen nicht | 📋 | `M-PD` ‚Companion autonomy …‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-010 | Vor dem Einsatz kann der Spieler mit dem Begleiter planen (Rollen, Eingänge, Positionen, laut oder leise, Rückzug, Treffpunkt, Fahrzeugrolle); im Einsatz sind Befehle möglich (Folgen, Warten, Decken, Beobachten, Flankieren, Ausgang sichern, Vorrücken, Zurückziehen, Leise sein); Handzeichen, ersatzweise Stimme, Funk, Telefon mit hörbaren Folgen | 📋 | `M-PD` ‚Companion autonomy …‘; [Spielvision](../Spielvision.md) §6.3 |
| N-CO-011 | Begleiter folgen Plan und Befehl, dürfen aber abweichen, wenn unmittelbare Gefahr oder neue Information die Anweisung offensichtlich unsicher oder unmöglich machen; unerfahrene Begleiter machen plausible Fehler, sabotieren aber nie zufällig | 📋 | `M-PD` ‚Companion autonomy …‘ |
| N-CO-012 | Ein erfahrener Hunter kann eine bessere spontane Entscheidung treffen als der Plan; er bewertet Plan, Befehl, Wahrnehmung, Gefahr, Persönlichkeit, Erfahrung, Vertrauen und Verletzungen | 🧭 | `23#255A` (Beschreibung des Assistenten); [Spielvision](../Spielvision.md) §6.3 |
| N-CO-013 | Eigenfeuer auf Begleiter ist physikalisch möglich und konfigurierbar; im härtesten Modus standardmäßig voll aktiv | 📋 | `M-PD` ‚Multiple-opponent combat …‘ |
| N-CO-014 | Der Hund ist ein besonderer Begleiter: adoptierbar, Rassen, Persönlichkeit, Training (Gehorsam, Rückruf, Suchen, Anzeigen, Warten), Geruchsverfolgung ohne magische Navigation, Reaktion auf Tiere und teils Übernatürliches, Verletzung, Alterung, Tierarzt; Impfungen optional; er kann zuhause bleiben | 📋 | `23#1088A` Fragen 31–40; „31-40 ja, impfungen sind optional“ in `23#1089E`; `23#1090A` ‚Hund‘ |
| N-CO-015 | Crewmitglieder leben im Hauptquartier: schlafen, essen, trainieren, recherchieren, Fahrzeuge reparieren, Waffen pflegen, telefonieren, fernsehen, reden, streiten, feiern, gehen und kommen zurück | 📋 | [Spielvision](../Spielvision.md) §6.4; `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; Ausformulierung `23#603A` Abschnitt 2 (💡) |
| N-CO-016 | Das Hunter-HQ ist der primäre soziale Ort; Versorgung, Beiträge, Aufgaben und Feiern bleiben organisch und nicht erdrückend; die Crew kann selbst Geld, Vorräte, Ersatzteile und Informationen beisteuern | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; [Spielvision](../Spielvision.md) §6.4 |
| N-CO-017 | Persönliche Anfragen der Crew haben mehrere faire Auslöser und sind nur unverfügbar, wenn die Figur stirbt oder dauerhaft geht | 📋 | `M-PD` ‚Acts, chapters and controlled story reactivity‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-018 | Crew-Aktivitäten (Training, Essen, Billard, Bar, Rennen, Schießstand, Angeln, Jagd, Kino, Roadtrip, Reparatur, Recherche, kleinere Jagd, einkaufen, Sport) sollen nicht unnötig verpassbar sein | 🧭 | `23#603A` Abschnitt 3 (Vorschlag des Assistenten) |
| N-CO-019 | Ein Crewmitglied recherchiert tatsächlich und informiert den Spieler, repariert Fahrzeuge mit echtem Fortschritt, liegt verletzt mehrere Tage im HQ, und andere Crew übernimmt Aufgaben | 🧭 | `23#603A` Abschnitt 2 (Vorschlag des Assistenten) |
| N-CO-020 | Ein Ledger der Beiträge im HQ (Aufträge, Munition, Vorräte), Hausarbeiten ohne XP, spontane Feiern nach Erfolgen, persönliche Item-Wünsche | 🧭 | `23#606A` Abschnitte 1–5 (Vorschläge des Assistenten); [Spielvision](../Spielvision.md) §6.4 |
| N-CO-021 | **Rekrutierung:** Optionale Faction-NPCs (Club, Sekte, kriminelle Gruppe, Extremisten) können rekrutiert werden und dann im HQ sein; Rekruten stehen für Missionen optional zur Verfügung; die Mission funktioniert immer ohne sie | 📋 | `23#677E`; `23#678A`; „Optional Recruit → mehr Optionen / Dialog / Unterstützung“ (Regel des Assistenten) |
| N-CO-022 | Nur spezielle, definierte Kandidaten sind rekrutierbar; mögliche Herkünfte: MC, Sekte, kriminelle Organisation, extremistische Gruppe, Polizei, Militär, Arzt/Paramedic, Mechaniker, Racer, Wissenschaftler, Okkultist, ehemalige Monsteropfer, normale Zivilisten | 🧭 | `23#678A` (Vorschlag des Assistenten) |
| N-CO-023 | Rekruten bringen echte Fähigkeiten mit (Fahrzeuge, Insiderwissen, Symbolerkennung, Netzwerke, Driving, Tuning, Medizin, Forensik); nicht jeder Rekrut passt automatisch ins HQ; es entstehen Reaktionen der Crew, Konflikte und Vertrauensentwicklung | 🧭 | `23#678A` |
| N-CO-024 | Das Recruitment-System bleibt nach dem Storyende aktiv; neue Leute können ins HQ kommen; das HQ ist kein eingefrorenes Museum | 📋 | `23#679E`; `23#680A` |
| N-CO-025 | Auch NPC-Hunter und Freunde können Begleiter werden (andere Hunter treffen → zusammenarbeiten → Vertrauen → Freundschaft oder Rivalität → später möglicherweise als Begleiter verfügbar) | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-CO-026 | Die Besetzung der Crew, Biografien, Spezialisierungen, Todesfälle und Beziehungen sind nicht festgelegt | ❓ | [Spielvision](../Spielvision.md) §6.2; `M-OD` |
| N-CO-027 | Feldbasen (Motels, Hütten, Häuser, Unterschlupfe, Bunker) dienen entfernten Handlungsbögen; ein mobiles Wildnislager ist möglich | 📋 | [Spielvision](../Spielvision.md) §6.4; `M-PD` |
| N-CO-028 | Partner können in Gefahr geraten, aber nicht ständig als Entführungswerkzeug | 📋 | `23#1088A` Frage 15; „11-15 genau“ in `23#1089E` |
| N-CO-029 | Mehr Hunter außerhalb des Kerns erscheinen im Laufe des Spiels; das Hunter-Netz ist selten | 📋 | `M-B09`; [Spielvision](../Spielvision.md) §6.2 |

---

## DI: Dialog und Sichtbarkeit

Gruppe zu [Modul 20](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-DI-001 | Dialog ist ein datengetriebenes Inhaltssystem; jede gesprochene Zeile hat eine stabile Dialog-ID und kann Sprecher, Text, Kontext, Emotion, Teilnehmer, Fall-/Weltzustandsbedingungen, Wissensbedingungen, Beziehungs-/Erinnerungsbedingungen, Antworten, Stimm-Asset, Lip-Sync/Gesicht/Körper, Untertitel, Lokalisierung und Prüfstatus referenzieren | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; Beispiel `23#313A` Abschnitt 1 |
| N-DI-002 | Beziehungs- und NPC-Gedächtniszustand können verschiedene Dialogvarianten auswählen, **ohne** versteckte Beziehungszahlen offenzulegen | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-003 | Das Dialogsystem liest intern Vertrauen, Angst, Respekt, Verdacht, vergangene Entscheidungen, Fallstatus und Erinnerungen und wählt passende Gesprächsvarianten; die Beziehung wird nie als Zahl angezeigt | 📋 Richtung | `23#313A` Abschnitt 4 (Beschreibung des Assistenten); `23#52E` Punkt 9 |
| N-DI-004 | Beispiele der Quelle: Trust niedrig: ‚Das geht Sie nichts an.‘; Trust mittel: ‚Mehr kann ich Ihnen derzeit nicht sagen.‘; Trust hoch: ‚Offiziell habe ich Ihnen das nie gezeigt.‘ | 💡 (Beispiele) | `23#313A` Abschnitt 4 |
| N-DI-005 | Dialogformen: natürliches Gespräch; Investigationsdialog über **tatsächlich bekannte** Themen und Beweise; Entscheidungsdialog bei wichtigen Lagen; dynamischer Dialog beim Fahren, Kämpfen, Erkunden | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; `23#313A` Abschnitt 3 |
| N-DI-006 | Wichtige Figuren brauchen große Mengen kontextabhängiger Barks (Kampf, Stealth, Erkundung, Verletzung, Fahren, Weltereignisse, Begleiterinteraktion); die Variation muss Wiederholung vermeiden | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-007 | Gespräche sind unterbrechbar und setzen kontextgerecht fort, statt lange Zeilen neu zu starten; Dialog findet beim Gehen, Fahren, Ausrüsten, Verbinden, Ermitteln statt | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-008 | Die Schreibweise bewahrt die individuelle Stimme: Alter, Region und Akzent, Wortschatz, Bildung, Persönlichkeit, Wissen, Humor, Stressreaktion, Tabu- und private Themen; wichtige Figuren haben ein Voice/Character Bible | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-009 | **Dialogtiefe nach Bedeutung:** Haupt- und Story-Figuren extrem tief; wiederkehrende Nebenfiguren tief; gewöhnliche Einwohner deutlich flacher, sprechen aber kontextbezogen über Ort, Wetter, Gerüchte, aktuelle Ereignisse, Polizeiaktivität und Weltzustandsfolgen | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-010 | Praktisch alle relevanten NPCs sind ansprechbar und gesprächsfähig, aber nicht jeder braucht gigantische Verzweigungen; mit jedem normalen NPC kann man reden („nicht sehr viel aber schon“) | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; „ja man soll mit jedem npc reden können, nicht sehr viel aber schon“ in `23#1057E` Punkt 31 |
| N-DI-011 | NPCs reden untereinander, damit Gerüchte und lokale Ereignisse sich organisch verbreiten und der Spieler nützliche Informationen belauschen kann | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; `23#1056A` Fragen 34–35 |
| N-DI-012 | Dialogvolumen: Hauptfiguren erreichen am Ende Hunderttausende Textzeilen im Basisspiel samt Kontextvarianten; wichtige wiederkehrende Nebenfiguren 10.000–50.000+; die genauen Endzahlen sind offen; Architektur muss auf sehr große Datenmengen skalieren | 📋 / ❓ | `M-PD` ‚Dialogue volume / scalability‘ |
| N-DI-013 | Der Dialoglaufzeit lädt nur die kontextuell relevanten Teile des Korpus, nicht alles beim Start | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-014 | Dialog-IDs und Revisionen machen veraltete Text-, Audio-, Gesichts- und Lokalisierungsassets erkennbar; geänderter Text markiert die frühere Aufnahme als neu aufzunehmen | 📋 | `M-PD` ‚Dialogue volume / scalability‘ |
| N-DI-015 | Dialogsystem zuerst mit klassischen Auswahlmöglichkeiten; später möglicherweise freie KI-Gespräche | 📋 | `48#151E` (Z18 in [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md)) |
| N-DI-016 | Die Spielengine entscheidet über Fakten und Konsequenzen, nicht ein Sprachmodell; ein Faktenpaket aus dem Weltzustand wird in Worte gefasst, eine KI-Formulierung darf nichts hinzuerfinden | 💡 (im Einklang mit `M-PD`) | `48#150A`; [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §13 |
| N-DI-017 | Der Spieler kann anderes erfahren als das, was ein NPC fühlt: er sieht nicht, ob ein NPC ihn mag, er merkt es am Ton | 📋 | „der spieler sieht nicht ob der npc ihn mag es merkt es am ton“ in `23#52E` |
| N-DI-018 | Hidden values brauchen sichtbare Folgen; Debug-Werkzeuge, die erklären, warum ein NPC handelt, sind vom Shipping-UI verborgen | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §4 Punkt 8, §9 |
| N-DI-019 | Eine Werte-Ansicht der Beziehungs- und Präferenzwerte gibt es nur in Mod-, Debug- und Cheat-Modus | 📋 (für Beziehungs-/Präferenzwerte) | [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §2 |
| N-DI-020 | Prompt-Entwurfstitel der Spur 05 nennen: Beziehung→Dialogkontext-Schnittstelle, Beziehung→Kooperationsschnittstelle, Beziehung→Zugangs-/Gefallen-Schnittstelle, Hook für erinnerte Ereignisse, Regel ‚keine sichtbare Beziehungszahl‘, Debug-Inspektoren | Inventar (kein Beschluss) | `M-P05` (DRAFT ONLY) |
| N-DI-021 | NPCs kommunizieren in verschiedenen Stilen (Telefon, Text, Sprachnachricht, Tempo, Länge, Emojis, Initiative); das Datenmodell ist offen | 📋 / ❓ | `M-PD` ‚Smartphone / social media …‘; `M-OD` |
| N-DI-022 | Die Ausgestaltung der Sprachproduktion (Voice, Facial, Body) und das Verhältnis zu freien KI-Dialogen sind eigene Produktionsbereiche | – | `M-PD` ‚Voice production strategy‘, ‚Voice / facial / body production‘ (hier nicht ausgeführt) |

---

## PE: Weltpersistenz und Speicherung

Gruppe zu [Modul 21](21-Weltpersistenz-und-Speicherentwurf.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-PE-001 | Der zentrale persistente Weltzustand ist das ‚Gedächtnis der Welt‘; er verfolgt Fallausgänge, NPC lebt/tot, Beziehungen, veränderte oder zerstörte Orte, entdecktes Wissen, Monsteraktivität, Ruf, Fahrzeuge, Safehouses, Weltereignisse, Zeit und andere dauerhafte Zustände | 📋 | `M-PD` ‚World state and persistence‘ |
| N-PE-002 | Wichtige persistente Entitäten haben stabile IDs; World Partition lädt Laufzeitinhalte, der Weltzustand bestimmt die richtige persistente Variante | 📋 | `M-PD` ‚World state and persistence‘; `23#48A` Abschnitte 12–13 |
| N-PE-003 | Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert statt ein monolithischer Block | 📋 | `M-PD` ‚World state and persistence‘ |
| N-PE-004 | Das Langzeit-Speicherziel liegt bei etwa 20–50 MB, wenn vertretbar erreichbar; Korrektheit, Zuverlässigkeit, Ladezeit und Wiederherstellung gehen vor Größe; Speicheroptimierung ist in der Politur verpflichtend | 📋 (Zielwert) | `M-PD` ‚World state and persistence‘ |
| N-PE-005 | Gespeichert werden nur Änderungen gegenüber der unveränderten Welt: „Wir speichern nicht die Welt. Wir speichern nur, was sich gegenüber der unveränderten Welt geändert hat.“ Mehr Größe ist nicht tragisch, solange alles geladen und perfekt funktioniert; in der Politur wird optimiert | 📋 Richtung | `23#51A` Abschnitt 17; „17 es ist nicht so tragisch wenn es mehr ist solange alles geladen ist und perfekt funktioniert“ in `23#52E` |
| N-PE-006 | Der Entwickler wünscht Backups etwa im Rahmen üblicher Spiele: „die meisten spiele schaffen es ein backup in 10mb zu speichern, schaffen wir das auch“ | 🧭 | `23#49E` Punkt 17 |
| N-PE-007 | Das Speichersystem arbeitet deltabasiert: Es speichert hauptsächlich Änderungen gegenüber deterministisch erzeugten Standardwerten; ein NPC ohne Änderung braucht praktisch nur `NPC_ID + Seed`; erst echte Änderungen kosten Speicher | 📋 Richtung | `35#278A` Fragen 31–33; „12-35 ja“ in `35#279E` |
| N-PE-008 | Zufall im Hintergrund entsteht deterministisch aus NPC-ID, Weltseed und Zeitfenster, damit Speichern und Laden reproduzierbar ist | 📋 Richtung | `35#278A` Frage 15; „12-35 ja“ in `35#279E` |
| N-PE-009 | Die Speicherung teilt Gedächtnis in permanent, long-term, short-term, disposable und fasst Kurzzeitgedächtnis zusammen; wichtige Einzelereignisse bleiben einzeln | 📋 Richtung | `35#278A` Fragen 34–37; „12-35 ja“, „37-39 ja“ in `35#279E` |
| N-PE-010 | Spielstandkompression wird genutzt, solange sie Ladezeiten nicht unerträglich macht | 📋 | `35#278A` Frage 38; „37-39 ja“ in `35#279E` |
| N-PE-011 | Alte Spielstände werden beim Laden **deterministisch migriert**, wenn NPC- und Präferenzsysteme später geändert werden | 📋 | `35#278A` Frage 39; „37-39 ja“ in `35#279E` |
| N-PE-012 | Für etwa 500.000 persistente NPC-Identitäten gilt als Architekturziel ein Spielstand, der „eher kleiner“ ist (Entwickler); der Assistent setzt ein Ziel von ungefähr 50–150 MB für weit fortgeschrittene normale Spielstände und 150–300 MB für sehr lange, extrem veränderte Welten; mehrere GB wären ein Architekturproblem | 🧭 / ⚠️ | „ich habe das so gefunden: also ja eher kleiner“ in `35#279E` Punkt 40; Ausformulierung `35#280A` (siehe [Modul 27](27-Konflikte-und-offene-Fragen.md) C-N2) |
| N-PE-013 | Die Welt läuft nur, während man spielt; keine heimliche Echtzeitsimulation zwischen Sitzungen; Zeitsprünge (Schlafen, Reisen) werden von der Hintergrundsimulation ereignisbasiert nachgeholt | 📋 Richtung | „die welt läuft nur wenn man gerade spielt, im online wird das anders sein“ in `35#279E` Punkt 11; `35#278A` Frage 12; „12-35 ja“ |
| N-PE-014 | Bei einer relevanten Begegnung wird der generierte Zustand eingefroren und persistent; spätere Generatoränderungen verändern ihn nicht | 📋 Richtung | `35#278A` Frage 24; „12-35 ja“ in `35#279E` (siehe N-ID-013) |
| N-PE-015 | Der Tod eines NPCs bleibt dauerhaft im Weltzustand; relevante Historie bleibt, technischer Detailzustand darf verdichtet werden | 📋 | `35#272A` Fragen 31–33; „1-50 passt alles“ in `35#273E` (siehe N-ID-018) |
| N-PE-016 | Der Weltzustand und die stabilen IDs müssen so gebaut sein, dass spätere DLCs einen alten Spielstand sauber erweitern können; der Entwickler meinte das ausdrücklich so | 📋 | `23#51A` (Ende); „ja das mit den dlcs meinte ich eh genau so“ in `23#52E` |
| N-PE-017 | Strukturierte Backend-/Online-/Serverdaten bevorzugt in PostgreSQL; **nicht** als Pflicht-Datenbank für lokale Einzelspieler-Saves | 📋 | `M-PD` ‚World state and persistence‘; „15. ok macht sinn“ in `23#49E` |
| N-PE-018 | Mods können pro Spielstand aktiviert und deaktiviert werden; deaktivierte Mods und ihr Zustand bleiben erhalten; kritisch fehlende Mods blockieren das Laden standardmäßig, mit Expertenübersteuerung nach Backup | 📋 | [Decision 0005](../../Decisions/0005-character-appearance-foundation.md) §1 (Master basis `MODDING`) |
| N-PE-019 | Im Koop ist der Host-Spielstand maßgeblich; der Fortschritt von Gästen gehört zum Host-Spielstand | 📋 | `CLAUDE.md` §6; `48#142A`, `48#144A` (Persistenz im Host-Spielstand) |
| N-PE-020 | Die Journal-Größe von `WorldFacts` wächst unbegrenzt; Verdichtung (Snapshot plus Journal-Ende) ist offen; hochfrequenter und abgeleiteter Simulationszustand läuft nicht durch `WorldFacts`; neue Datensatzarten brauchen ein versioniertes, migrierbares Format | 📋 (technische Randbedingung) | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md); [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6 |
| N-PE-021 | Ein Spielstand aus Voll-Snapshot plus inkrementellem Journal (Delta) ist schneller, platzsparender, absturzsicher und für die Wiederherstellung besser; gelegentlich wird konsolidiert | 💡 | `23#51A` Abschnitt 17 (Beispiel: Snapshot 18 MB, Delta 312 KB) |
| N-PE-022 | Wiederherstellung nach Laden gleicht dem Zustand vor dem Speichern | 💡 (Invariante) | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 3 |
| N-PE-023 | Die Aufteilung in Segmente wie Manifest, Spieler, Welt global, Fälle, NPC-Regionen, Orte, Fahrzeuge, Safehouses ist ein konzeptionelles Beispiel des Assistenten | 💡 | `23#48A` Abschnitt 17 |

---

## SI: Simulationsstufen und Skalierung

Gruppe zu [Modul 22](22-Simulationsstufen-Skalierung-und-Performance.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-SI-001 | **Skalierbare Weltsimulation (G):** hohe Genauigkeit nahe am Spieler, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben nicht geladener NPCs und Gebiete, Persistenz wichtiger Ergebnisse; nicht jeder NPC läuft als voller Unreal-Actor; die Weltgröße von etwa 2.200 km² ist ein Szenario und darf nirgends fest verdrahtet sein | 📋 | `PE-N1022` G |
| N-SI-002 | Nicht jeder NPC braucht die volle Simulation: wichtige Figuren sind vollständige Akteure mit StateTree-Verhalten und Gedächtnis; Hintergrund und ferne Bevölkerung sind leichter beziehungsweise abstrahiert | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-SI-003 | Persistenzstufen A bis D legen fest, wie viel Persistenz und Simulation eine Figur bekommt | 📋 Richtung | `23#1056A` Fragen 17, 121; „ja“ in `23#1057E`; `23#1058A` |
| N-SI-004 | **Realismus und glaubwürdige Konsistenz sind wichtiger als eine mathematisch lückenlose Simulation jeder Sekunde:** „glaubwürdig, das wichtigste von diesem spiel ist realismus“; die Simulation ist so tief, dass der Spieler keinen offensichtlichen Fake bemerkt | 📋 | `23#1057E` Punkt 125; `23#1058A` ‚Oberste Regel aus Block 8‘ |
| N-SI-005 | Ein weit entfernter NPC wird als Zustand (`AT_WORK`, `HOME`, `TRAVEL` …) geführt, nicht als exakte Gehbewegung; beim Annähern entsteht daraus wieder ein konsistenter detaillierter NPC; die Welt ist beim Betreten eines Gebietes nicht leer | 📋 | `23#1056A` Fragen 122–123; „ja sowieso nicht, aber wenn man dann in das disktikt kommt darf die straße nicht leer sein, sondern auch voll“ und „ja sicher“ in `23#1057E` |
| N-SI-006 | Die Hierarchie ist für eine sehr große Welt „besonders konsequent“ nötig | 📋 | `23#1056A` Frage 124; „ja genau“ in `23#1057E` |
| N-SI-007 | Crowd LOD: in der Ferne vereinfachte Simulation, in Spielnähe detaillierte NPCs; Menschenmengen werden aus dem vorhandenen Simulationszustand rekonstruiert | 📋 | `23#1056A` Frage 87; „ja“ in `23#1057E`; `23#1058A` |
| N-SI-008 | Jeder NPC hat eine dauerhafte globale ID, auch wenn nicht geladen; mehrere Simulationsstufen sind vorgesehen (Beispiel des Assistenten: Tier 0 Hero/direkt beim Spieler, Tier 1 lokal aktiv, Tier 2 regionale Hintergrundsimulation, Tier 3 globale Dormant-Simulation); ein NPC wechselt ohne Verlust von Identität und Historie; Tier 3 wird nicht frameweise, sondern über vereinfachte Ereignis-/Zeitschritte simuliert | 📋 Richtung (Namen: 🧭) | `35#278A` Fragen 1–4; „1-4 ja“ in `35#279E`; W-TV-010 der Welt-Dokumentation behandelt die Stufen als 🧭/💡 |
| N-SI-009 | Dormante NPCs entwickeln Jobwechsel, Umzug, Beziehung, Trennung, Tod grob weiter; nicht „der jetzige ort, nicht der ganze weg“ | 📋 | `35#278A` Frage 5; „5 ja aber halt nicht der jetzige ort nicht der ganze weg und so weiter“ in `35#279E` |
| N-SI-010 | Die Genauigkeit steigt, sobald ein NPC für den Spieler wichtiger wird; ein zufälliger unbekannter NPC braucht deutlich weniger Zustand; Herabstufung nach Jahrzehnten Irrelevanz ist möglich, Historie bleibt | 📋 | `35#278A` Fragen 6–10; „6-10 ja“ in `35#279E` |
| N-SI-011 | Hintergrundsimulation arbeitet ereignisbasiert statt sekündlich (statt 86.400 Sekunden eines Tages wenige Ereignisse wie arbeiten, Person treffen, heimfahren, schlafen); ein wichtiger NPC wird genauer simuliert als 50.000 Unbekannte; ein NPC außerhalb hat eine plausible Routine und beim plötzlichen Besuch einen konsistenten aktuellen Zustand; NPCs sind nicht an zwei Orten gleichzeitig; Reisen kostet Zeit | 📋 Richtung | `35#278A` Fragen 12–20; „12-35 ja“ in `35#279E` |
| N-SI-012 | Zufall entsteht deterministisch aus NPC-ID, Weltseed und Zeitfenster | 📋 Richtung | `35#278A` Frage 15; „12-35 ja“ in `35#279E` |
| N-SI-013 | Relationship-/Präferenzzustand skaliert: kompaktes generiertes Profil für Unbekannte; detaillierte Werte erst bei Relevanz; Seed bleibt; Einfrieren bei relevanter Begegnung; detaillierte Kanten nur für relevante Personen; echte Beziehungen zwischen zwei NPCs werden automatisch detaillierter; kurzfristige Anziehung zwischen Hintergrund-NPCs darf verschwinden; wichtige Historie nie | 📋 Richtung | `35#278A` Fragen 21–30; „12-35 ja“ in `35#279E` |
| N-SI-014 | Die Welt läuft nur, während man spielt; Zeitsprünge werden nachgeholt | 📋 | „11 die welt läuft nur wenn man gerade spielt“ in `35#279E`; `35#278A` Frage 12 |
| N-SI-015 | Optimierung ist ein globales Kernziel: „ja genau, das ganze spiel muss super optimiert sein“; hohe Qualität, aber nicht für jeden High-End-Hardware | 📋 | `35#785E` Punkt 139; `35#786A` |
| N-SI-016 | Eine Bevölkerung von mehreren Hunderttausend NPCs (etwa 500.000) ist Planungsgröße, nicht beschlossen; Simulationsstufen und Budgets sind unabhängig von der Lore-Bevölkerung zu planen: ‚The entire population need not be individually active at once.‘ | 🧭 | [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13; `M-PO` Punkt 5; `35#227E` |
| N-SI-017 | **Simulations-LOD, Save-Schema, deterministische Offscreen-Simulation, Tests und Leistungsbudgets sind offen** | ❓ | `M-OQ16` Punkt 9; `M-AS` Kandidat 6 |
| N-SI-018 | Promotion und Demotion zwischen den Stufen sind für wichtige Ergebnisse verlustfrei; Stufenschwellen sind Konfiguration | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §4 Punkt 6, §5 |
| N-SI-019 | Es gibt eine Weltuhr; Fortschreiben läuft nie der Weltuhr voraus | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 5 |
| N-SI-020 | Zahlen (Anzahl Entitäten, Tick-Budgets, Speicher je Datensatz) werden nicht gewählt, sondern an einem Prototyp gemessen; keine Zahl darf die Weltgröße festschreiben | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §5 |
| N-SI-021 | Portable C++20-Kernbibliotheken für technische Logik mit klarem Nutzen (Regeln, Zustandsübergänge, Speicherschemata, Teile der Antriebslogik) und Unreal-Adapter für Actors, Komponenten, Navigation und Laufzeitintegration | 📋 | `CLAUDE.md` §5 |
| N-SI-022 | Fedora-Laptop für Code und schnelle Tests (16 GB, integrierte Grafik, Editor eventuell unpraktisch); Windows-Desktop (RTX 3090, 32 GB) für Unreal-Integration und Rendering-Tests | 📋 | `CLAUDE.md` §5 |
| N-SI-023 | Das System darf nicht gestartet werden, bevor seine Mindest-Eingaben existieren | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §8 |
| N-SI-024 | Ressourcen für NPC-Verkehr, Menschenmengen und Bahnbetrieb werden mit Detailstufen abstrahiert; Untergrundbevölkerung wird unabhängig von der Lore-Zahl budgetiert; Budgets, Performance-Stufen und deterministische Resynchronisation sind offen | 📋 / ❓ | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §8 |

---

## KO: Koop, Host-Speicherstand, Mods

Gruppe zu [Modul 23](23-Koop-Host-Speicherstand-und-Mods.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-KO-001 | **Host-Spielstand ist maßgeblich;** der Fortschritt von Gästen gehört zum Host-Spielstand; die Welt ist die Welt des Hosts | 📋 | `CLAUDE.md` §6; „es können sich die leute immer ändern aber der fortschritt ist auf dem spielstand gebunden“ in `48#105E` ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) C6, C16) |
| N-KO-002 | Was ein Gast auf seinem eigenen Spielstand hat, bleibt dort; bei einem Freund hat er nur, was er auf **dessen** Spielstand hat | 📋 | `48#107E` ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) C7) |
| N-KO-003 | Koop: 2–4 Spieler, kampagnenbasiert, Mods und Cheats erlaubt, von der offiziellen Online-Progression getrennt | 📋 | `M-PD` ‚Multiplayer separation‘ |
| N-KO-004 | Offizielles Online: eine persistente Online-Slot-Progression; keine Mods und Cheats; die Zeit-/Serverlogik für Online ist später eigen („im online wird das anders sein“) | 📋 | `M-PD` ‚Multiplayer separation‘; „11 die welt läuft nur wenn man gerade spielt, im online wird das anders sein“ in `35#279E` |
| N-KO-005 | **Entscheidungen im Story-Koop sind situationsabhängig:** persönliche (Dialoge, Beziehungen zu NPCs, moralische Entscheidungen) trifft jeder Spieler selbst; gemeinsame (Ermittlungsentscheidungen, Schicksal eines gemeinsamen Falls, Entscheidungen mit Wirkung auf die gemeinsame Story) werden abgestimmt | 📋 (Wahl) / 💡 (Tabelle) | „Supernatural Story-Koop Entscheidungen: Situationsabhängig: persönlich individuell, gemeinsam per Abstimmung“ in `48#97E`; Tabelle `48#98A` |
| N-KO-006 | Bei Gleichstand gelten situationsabhängige Regeln | 📋 | `48#99E` ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) C3) |
| N-KO-007 | Das gemeinsame Journal löst unterschiedliche Meinungen und Theorien der Spieler nicht automatisch auf | 💡 | `48#98A` |
| N-KO-008 | Individuelle Verhaftung; die Welt läuft weiter; ein Welt-Zeitsprung im Koop nur mit Zustimmung aller aktiven Spieler; bei Zustimmung entwickeln sich Monster, NPCs, Organisationen und Ermittlungen weiter | 📋 | `48#161E`, „Supernatural Koop-Haft-Zeitsprung: Gemeinsamer Welt-Zeitsprung mit Zustimmung aller“ in `48#163E`; Auslegung `48#164A` |
| N-KO-009 | Jeder Koop-Spieler erstellt seinen Hunter über die Charaktererstellung | 📋 | `48#109E` |
| N-KO-010 | Modding ist ausdrücklich unterstützt und soll keine Reverse-Engineering-Arbeit verlangen; Mods sind standardmäßig additiv und nicht zerstörend über APIs, Registries und Hooks; kontrollierte Überschreibungen laufen über explizite Mod-Layer-/Ladereihenfolge-Regeln | 📋 | `M-PD` ‚Modding‘ |
| N-KO-011 | Vorläufige Mod-Ebenen: Daten-/Inhaltsmods, Unreal-/Blueprint-/visuelle Mods, native C++20-Mods; native Mods sind vertrauter Code; öffentliche APIs nach dem Prinzip der geringsten Rechte | 📋 | `M-PD` ‚Modding‘ |
| N-KO-012 | Das technische Mod-Kern ist von keinem Anbieter abhängig (Steam Workshop ist nicht gewählt oder verpflichtend); die lokal installierte Menge bleibt ohne Online-Anbieter nutzbar; Standardort `<GameRoot>/mods/`; `.spnmod` als portables Paket; installiert und aktiviert sind getrennt; pro Profil und pro Spielstand aktivierbar | 📋 | `M-MOD` (Block 5, CONFIRMED / CANONICAL) |
| N-KO-013 | Kritisch fehlende Mods blockieren das Laden des Spielstands standardmäßig; eine ausdrückliche Expertenübersteuerung erlaubt das Laden auf eigenes Risiko erst nach automatischem Backup und Risikohinweis; deaktivierte Mods bleiben installiert, ihre Dateien und ihr Zustand bleiben erhalten; Deaktivierung im Spiel ist zuerst logisch (keine neuen Aktivierungen, sichere Neuladung/Abgleich) | 📋 | `M-MOR` (Rekonstruktion 2026-09-09) |
| N-KO-014 | Mods sollen leicht zugänglich sein: dokumentierte APIs, stabile IDs, Datenformate, Beispielmods, Dokumentation, Debugger, Validierung, einfaches Packaging, verständliche Fehlermeldungen; „ja ich will dass modding erlaubt und einfach zu machen ist“ | 📋 | `23#48A` Abschnitt 18; Antwort „18 ja ich will dass modding erlaubt und einfach zu machen ist“ in `23#49E` |
| N-KO-015 | Mögliche Mod-Inhalte (Beispiele des Assistenten): neue Kreaturen, Fälle, Waffen, Rituale, Items, Codex-Einträge, Orte, **NPCs**, **Weltereignisse**, Karten | 💡 | `23#48A` Abschnitt 18 |
| N-KO-016 | Wie weit Native-C++-Modding gehen soll, wird gesondert entschieden; Lua ist nur optional, wenn es eine echte Lücke schließt | 📋 / ❓ | `23#48A` Abschnitt 18 („später gesondert entscheiden“); `M-PD` ‚Modding‘; Entwickler: „so wie beim minecraft spiel, nur ohne lua alles in c++“ in `23#52E` Punkt 18 (früher als `M-PD`; dort ist Lua nur optional) |
| N-KO-017 | Neue Datensatzarten müssen additiv registriert werden; ein Spielstand mit unbekannten Arten muss unverändert laden und wieder speichern | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6; [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) |
| N-KO-018 | Die Simulation läuft nur auf der Autorität; die Replikation des Simulationszustands ist offen; `WorldFacts` hat noch kein Compare-and-Set | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6; [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) |
| N-KO-019 | Die Mod-Erweiterungspunkte des NPC-Systems sind offen | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11 |

---

## KS: Konsistenz zwischen Systemen

Gruppe zu [Modul 24](24-Integration-und-Konsistenzregeln.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-KS-001 | **Integration (H):** Weltzustand (WorldFacts), Ermittlung/Caseboard, Monster-KI und Bestiarium, Verletzungen und Medizin, Ruf, Polizei und Justiz, Organisationen und Territorien, Dialog und Beziehungen, Speichern/Laden, Koop und Host-Speicherstand; die Simulation darf keine widersprüchlichen Zustände zwischen diesen Systemen erzeugen | 📋 | `PE-N1022` H |
| N-KS-002 | **Ein Besitzer je Tatsache:** Jede Art von Zustand hat genau einen maßgeblichen Speicher; andere halten Verweise über stabile IDs und lesen, kopieren nicht | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 1 |
| N-KS-003 | **Ereignisse statt Direktschreibzugriffe:** Querwirkungen (Tod → Beziehungen, Ruf, Fall, Territorium) werden als Ereignisse in definierter Reihenfolge verarbeitet, sodass Wiederabspielen dasselbe Ergebnis liefert | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 2 |
| N-KS-004 | **Invarianten als Tests:** ein toter Einzelner handelt nicht; eine Verletzung ist in jedem System sichtbar, das die Person zeigt; ein Zeuge weiß nur, was er beobachtet oder gehört hat; ein Fall zitiert nur Geschehenes; eine Fraktion hält kein Gebiet, das ihre Mitglieder nicht erreichen; der Zustand nach Laden gleicht dem vor dem Speichern | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 3 |
| N-KS-005 | **Beobachtungskonsistenz:** Was der Spieler nach dem Laden einer Region sieht, entspricht dem persistenten Datensatz, fortgeschrieben bis jetzt; es gibt keine Rücksetzer wichtiger Ergebnisse | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 4 |
| N-KS-006 | **Eine Weltuhr;** Fortschreiben läuft nie vor ihr; Geschichte und Zeitreise sind getrennte offene Themen | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 5 |
| N-KS-007 | Die Welt, die Monsterwelt und die Gesellschaft sind **dieselbe Simulation:** Monsterfälle beeinflussen normale Systeme (Immobilien, Tourismus, Polizei, Medien, Verkehr, Firmen, Krankenhäuser, Tiere, lokales Wetter, Gerüchte) | 🧭 | `23#1066A` Punkt 30 (Vorschlag des Assistenten); `23#1058A` ‚Supernatural-Reaktion‘ |
| N-KS-008 | Das Hauptstory-Gerüst bleibt kontrolliert; Spielerentscheidungen ändern Wege, Informationen, lokale Ausgänge und Beziehungen stärker als das Überleben vieler kritischer Hauptfiguren | 📋 | `M-PD` ‚Acts, chapters and controlled story reactivity‘ |
| N-KS-009 | Ein Wesen lernt aus dem Beobachteten; Behörden handeln aus tatsächlicher Information; Menschen teilen Informationen nur über plausible Kommunikation | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-KS-010 | Der Weltzustand bestimmt beim Laden die richtige persistente Variante (zerstörte Kirche, toter NPC wird nicht gespawnt) | 📋 | `23#48A` Abschnitt 12; `M-PD` ‚World state and persistence‘ |
| N-KS-011 | Die Welt wird von Systemen getragen, die miteinander sprechen („Nicht 500 isolierte Features bauen, sondern dieselben Systeme ständig miteinander sprechen lassen.“) | 💡 | `23#603A` (Schlussbemerkung des Assistenten) |
| N-KS-012 | Das System darf nicht begonnen werden, bevor seine Mindest-Eingaben existieren | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §8 |

---

## EN: Entwurfsregeln (unverbindliche Vorschläge)

| ID | Regel | Modul | Bezug |
|---|---|:---:|---|
| N-EN-001 | Eine NPC-ID wird nie wiederverwendet und ändert sich nie | 03 | N-ID-008 |
| N-EN-002 | Gleiche ID und gleicher Weltseed liefern denselben abgeleiteten Zustand | 03 | N-ID-009, N-ID-020 |
| N-EN-003 | Ein eingefrorener NPC wird durch ein Generator-Update nicht verändert | 03 | N-ID-013 |
| N-EN-004 | Ein Aufstieg der Datensatztiefe erfolgt nur durch ein Ereignis, nie durch Zeitablauf | 03 | Abschnitt 5 |
| N-EN-005 | Ein Abstieg verdichtet Detailzustand, aber verliert weder Identität noch Beziehungsgeschichte noch wichtige Einzelereignisse | 03 | N-ID-015 |
| N-EN-006 | Ein toter NPC nimmt keine Handlung vor und erscheint nicht als lebende Figur | 03 | N-LT-017 |
| N-EN-007 | Ein D-Mensch ohne Gespräch, Ereignis oder Beobachtung hat keine ID im Weltzustand | 03 | Abschnitt 10 |
| N-EN-008 | Eine Materialisierung am selben Ort zur selben Zeit mit demselben Weltzustand liefert dieselbe Person | 03 | Abschnitt 10 |
| N-EN-009 | Ein `sim_safe`-NPC stirbt nie durch reine Simulationsartefakte | 03 | N-LT-033 |
| N-EN-010 | Ein Wechsel der Persistenzstufe löscht nie die Beziehungen zum Spieler | 03 | N-ID-015 |
| N-EN-011 | Ein Umzug ändert Wohnort, Arbeit und Routine, nicht ID, Seed oder Historie | 03 | N-ID-019 |
| N-EN-012 | Der Datensatz enthält keine Zahlen für den Spieler; Sichtbarkeit nur über Verhalten und Archiv | 03 | N-LT-012 |
| N-EN-021 | Jede Änderung des Persönlichkeitskerns hat ein Ereignis als Ursache im Journal | 04 | N-PS-006, N-PS-016 |
| N-EN-022 | Schnelle Zustände kehren ohne neue Ereignisse zur Grundlinie des Kerns zurück | 04 | Abschnitt 2 |
| N-EN-023 | Dasselbe Ereignis kann für zwei NPCs gegensätzliche Wirkung haben, aber jedes Ergebnis ist deterministisch reproduzierbar | 04 | N-LT-027 |
| N-EN-024 | Beziehungswerte sind Zustände einer Beziehung, nicht einer Person | 04 | Abschnitt 2 |
| N-EN-025 | Ein Zustandswert bewegt sich pro Zeitschritt höchstens um eine feste Schrittweite (kein Sprung) | 04 | N-PS-016 |
| N-EN-026 | Die Persönlichkeit eines NPC ändert sich nie durch Zeit allein | 04 | Abschnitt 6 |
| N-EN-027 | Rauschzustand verschlechtert Wahrnehmung und Erinnerung messbar | 04 | N-PS-011 |
| N-EN-028 | Persönlichkeitsdimensionen sind kontinuierlich; Archetypen sind abgeleitet | 04 | N-PS-005 |
| N-EN-029 | Kein Zustandswert erscheint in der Spieler-Oberfläche | 04 | N-LT-012 |
| N-EN-041 | Ein NPC weiß nur Dinge, für die ein Eintrag mit Quelle (gesehen, gehört, erzählt, Medien, abgeleitet) existiert | 05 | N-GE-008 |
| N-EN-042 | Der Inhalt eines Eintrags darf von der Wahrheit abweichen, der Verweis auf das Ereignis nicht | 05 | N-GE-009 |
| N-EN-043 | Ein Eintrag der Klasse `permanent` wird nie verworfen und verblasst im Kern nie | 05 | N-GE-005 |
| N-EN-044 | Ein NPC im Zeitstand T kennt keine Ereignisse nach T | 05 | N-GE-028 |
| N-EN-045 | Verdichtung entfernt nie einen Eintrag ab Wichtigkeit `notable` | 05 | N-GE-022 |
| N-EN-046 | Dieselbe Frage an denselben Zeugen im selben Weltzustand liefert dieselbe Antwort | 05 | Ermittlung 12 §4 |
| N-EN-047 | Verblassen verbraucht Weltzeit, keine Realzeit; ein beendetes Spiel verändert keinen Eintrag | 05 | N-LT-020 |
| N-EN-048 | Ein Ereignis ohne Beobachter erzeugt keinen Eintrag bei NPCs | 05 | N-GE-008 |
| N-EN-049 | Der Tod eines NPCs entfernt weder die Erinnerungen anderer an ihn noch die Beziehungsgeschichte | 05 | N-GE-025, N-ID-018 |
| N-EN-050 | Wiedererkennung ist abhängig von Bekanntheitsstufe, Zeit, Veränderung und Kontext und ist reproduzierbar | 05 | N-GE-015 |
| N-EN-051 | Konfidenz und Genauigkeit eines Eintrags sind nie in der Spieler-Oberfläche sichtbar | 05 | N-LT-012 |
| N-EN-052 | Ein wichtiger Eintrag mit Spielerbezug bleibt nach Speichern und Laden unverändert einzeln abrufbar | 05 | N-GE-023 |
| N-EN-053 | Ein NPC kann nicht über einen Weg erfahren, den es im Weltzustand nicht gibt (Ort, Kontakt, Medium) | 05 | N-GE-008, Modul 07 |
| N-EN-054 | Bei Verkleidung erinnert ein NPC die erlebte Rolle, nicht die wahre Identität | 05 | N-GE-019 |
| N-EN-055 | Ein Eintrag mit `suggested` als Quelle hat ein Ereignis im Journal, das die Manipulation belegt | 05 | Abschnitt 11 |
| N-EN-061 | Jede Aussage eines NPC über ein Weltereignis hat einen Eintrag mit Quelle | 06 | N-WA-010 |
| N-EN-062 | Ein Beobachter ohne Reichweite (Sicht, Gehör, Kontakt) erfährt nichts | 06 | N-WA-009, N-WA-010 |
| N-EN-063 | Die Wahrnehmung hängt von Licht, Entfernung, Ablenkung und Zustand ab und liefert bei gleichen Eingaben gleiche Ergebnisse | 06 | N-WA-009 |
| N-EN-064 | Kein NPC verwendet Information, die er nicht auf einem plausiblen Weg bekommen hat (kein Schwarmwissen) | 06 | N-WA-005, N-WA-006 |
| N-EN-065 | Die Gewissheit über Position und Einzelheiten nimmt ohne neue Hinweise ab | 06 | N-WA-004 |
| N-EN-066 | Verdacht steigt nur durch einen Anlass und sinkt ohne Bestätigung | 06 | Abschnitt 6 |
| N-EN-067 | Eine Straftat ohne Zeugen löst keine Fahndung aus | 06 | N-WA-013 |
| N-EN-068 | Die Polizei beginnt mit der gewöhnlichen Erklärung, solange das Übernatürliche nicht bekannt ist | 06 | N-WA-017 |
| N-EN-069 | Das Übernatürliche wird nach dem Weltbild des NPC gedeutet; verschiedene NPCs deuten verschieden | 06 | N-WA-018 |
| N-EN-070 | Ein falscher Glaube lässt sich nur durch ein Ereignis ändern, das im Journal steht | 06 | N-GE-009 |
| N-EN-071 | Ein öffentliches Ereignis macht nicht automatisch alle Beobachter zu Gläubigen | 06 | Abschnitt 4 |
| N-EN-072 | Eine Verkleidung ändert Erwartung und Verdacht, nie die Identität im Weltzustand | 06 | N-WA-014 |
| N-EN-073 | Kameras liefern Aufzeichnungen, keine Erinnerungen, und nur im Sichtfeld | 06 | Abschnitt 10 |
| N-EN-074 | Wahrnehmung ferner NPCs wird ereignisbasiert abgeleitet und widerspricht nie dem sichtbaren Verhalten bei der Materialisierung | 06 | Modul 22, `23#1057E` Punkte 122–123 |
| N-EN-081 | Eine Information erreicht einen NPC nur über einen Kanal, der im Weltzustand existiert und den NPC erreicht | 07 | N-IN-001, N-IN-022 |
| N-EN-082 | Ein Gerücht kann die Wahrheit verzerren, aber nicht aus dem Nichts eine Person erfinden, die es nicht gibt | 07 | N-IN-002 |
| N-EN-083 | Ein belauschtes Gespräch löst keinen Fall und keinen Hinweis aus, der nicht im Weltzustand begründet ist | 07 | N-IN-004, N-LT-008 |
| N-EN-084 | Die Ausbreitung ist durch den sozialen Graphen begrenzt; ein Gerücht erreicht keinen NPC ohne Verbindung | 07 | N-IN-007, N-IN-008 |
| N-EN-085 | Dasselbe Gerücht wird bei gleicher Eingabe gleich verzerrt (deterministisch) | 07 | Ermittlung 12 §4 |
| N-EN-086 | Ein Funkloch verzögert oder verhindert Nachrichten | 07 | N-IN-016 |
| N-EN-087 | Medienberichte erscheinen nach dem Ereignis, nie davor | 07 | N-IN-019 |
| N-EN-088 | Ein widerlegtes Gerücht bleibt in den Erinnerungen derer, die es glaubten, bis sie davon erfahren | 07 | N-GE-009 |
| N-EN-089 | Ein Geheimnisträger gibt ein Geheimnis nicht ohne Anlass weiter; ein Plauderer häufiger | 07 | N-IN-009 |
| N-EN-090 | Der Spieler kann ein Gerücht nicht ohne Mittel, Aufwand und Quelle korrigieren | 07 | N-IN-012 |
| N-EN-091 | Der Social-Media-Feed ist nie Voraussetzung für Hauptstory oder handgeschriebene Nebenfälle | 07 | N-IN-014 |
| N-EN-092 | Ein NPC verbreitet Wissen, das er hat; er erfindet keines | 07 | N-GE-008 |
| N-EN-093 | Öffentliche Bekanntheit erhöht die Verbreitungsrate, ändert aber nicht die Wahrheit des Inhalts | 07 | N-IN-010 |
| N-EN-111 | Ein Typwechsel einer Beziehung hat ein Ereignis im Journal als Ursache | 08 | Abschnitt 4 |
| N-EN-112 | Eine Beziehungskante enthält nur die Wahrnehmung des Quell-NPCs, nie die Wahrheit über das Ziel | 08 | Abschnitt 2 |
| N-EN-113 | Dimensionen sind unabhängig; keine Operation darf sie zu einem einzigen Wert zusammenfassen, der Verhalten allein bestimmt | 08 | N-BE-004 |
| N-EN-114 | Keine Beziehungsdimension erscheint als Zahl oder Balken in der Spieler-Oberfläche | 08 | N-BE-001 |
| N-EN-115 | Eine Beziehungsänderung hat ein Ereignis, das der Beteiligte erfahren hat | 08 | N-GE-008 |
| N-EN-116 | Dasselbe Ereignis ändert zwei verschiedene Kanten unterschiedlich, wenn Kern, Erwartung und Zuschreibung unterschiedlich sind | 08 | N-LT-027 |
| N-EN-117 | Gruppenhaltung und individuelle Beziehung können widersprechen | 08 | N-BE-020 |
| N-EN-118 | Wird eine Kante auf falscher Zuschreibung aufgebaut, ändert sich die Wahrheit im Weltzustand nicht | 08 | Abschnitt 6 |
| N-EN-119 | Eine Beziehungsstufe wird nicht durch einen einzelnen Datenpunkt verlassen (Hysterese) | 08 | Abschnitt 5 |
| N-EN-120 | Der Tod einer Person friert die Kante ein; die Dimensionen ändern sich danach nur durch Erinnerung | 08 | N-GE-025 |
| N-EN-121 | Eine detaillierte Kante zwischen zwei Hintergrund-NPCs entsteht nur durch ein Ereignis, das sie rechtfertigt | 08 | `35#278A` Frage 27 |
| N-EN-122 | Eine einseitige Beziehung wird von beiden Seiten verschieden erlebt und gespielt | 08 | Abschnitt 10 |
| N-EN-123 | Eine Beziehung zu einem NPC hängt nicht von einem Zufallswurf ab, sondern von Ereignissen und Zustand | 08 | N-PS-016 |
| N-EN-131 | Jede Änderung einer Beziehungsdimension hat ein Ereignis, das der Beteiligte kennt | 09 | N-BE-001, N-GE-008 |
| N-EN-132 | Vergebung ist nicht allein von der Zeit abhängig | 09 | N-BE-013 |
| N-EN-133 | Ein NPC verzeiht, was in seinem Kern ‚nie verziehen‘ ist, nicht | 09 | N-BE-013 |
| N-EN-134 | Eine Feindschaft eskaliert nur durch ein Ereignis | 09 | N-LT-023 |
| N-EN-135 | Eine falsche Anschuldigung kann eine Beziehung beschädigen, ohne dass die Wahrheit sich ändert | 09 | N-BV-010, N-BV-013 |
| N-EN-136 | NPC-NPC-Beziehungen verändern sich im Hintergrund mit einer Ursache im Journal | 09 | N-BV-018 |
| N-EN-137 | Die Welt zählt Beziehungsereignisse nicht als ‚Punkte‘; es gibt kein Freischalten durch Beziehungsstufe | 09 | Abschnitt 4 |
| N-EN-138 | Ein verpasstes Zeitfenster entfernt keine Beziehungsaktivität | 09 | N-BV-020, N-LT-035 |
| N-EN-139 | Ein Ereignis wirkt auf Betroffene, Zeugen und Dritte getrennt und deterministisch | 09 | Abschnitt 2 |
| N-EN-140 | Ein Verhaltensbild (Reaktionstabelle) hängt von Persönlichkeit, Beruf und Lage ab, nicht von einem Zufallswurf | 09 | Abschnitt 3 |
| N-EN-141 | Schuld sinkt durch Einlösung und kann nicht ohne Ereignis verschwinden | 09 | Abschnitt 7 |
| N-EN-142 | Eine Fernbeziehung bleibt eine Beziehung; ihre Pflege geschieht durch Kontakt | 09 | Abschnitt 8 |
| N-EN-161 | Jeder NPC mit Stufe A bis C gehört zu genau einem Haushalt (oder ist obdachlos mit Stammplatz) | 10 | N-FA-015 |
| N-EN-162 | Ein Haushalt hat keine Mitglieder, die tot oder ausgezogen sind, ohne entsprechendes Ereignis | 10 | N-FA-003 |
| N-EN-163 | Heirat, Trennung, Geburt, Tod und Umzug sind Ereignisse im Journal | 10 | N-FA-003, N-FA-008 |
| N-EN-164 | Eine Verwandtschaftskante ändert sich nur durch ein Ereignis (Adoption, Entdeckung) | 10 | Abschnitt 3 |
| N-EN-165 | Die Bevölkerung wird bei Tod, Zuzug und Geburt konsistent bilanziert, sobald Generationen aktiviert sind | 10 | N-FA-005 |
| N-EN-166 | Kinder sind vom Adult-System ausgeschlossen | 10 | N-FA-019 |
| N-EN-167 | Ein Umzug ändert Netz und Routine, nicht ID und Historie | 10 | N-ID-019 |
| N-EN-168 | Feiertage verändern Routinen von Familien nachvollziehbar | 10 | N-FA-012 |
| N-EN-169 | Ein Ort behält seine Geschichte bei Betreiberwechsel | 10 | N-GE-026 |
| N-EN-170 | Die Familie eines Opfers erfährt vom Tod über einen existierenden Weg | 10 | N-GE-008 |
| N-EN-171 | Keine Genealogie wird für NPCs vorausgesetzt, die sie nicht brauchen | 10 | Abschnitt 3 |
| N-EN-181 | Ein Nemesis lernt nur Dinge, für die es im Weltzustand eine Beobachtung oder plausible Weitergabe gibt | 11 | N-NE-008, N-WA-005 |
| N-EN-182 | Kein Wert des Nemesis steigt allein durch die Zahl der Begegnungen | 11 | N-NE-017 |
| N-EN-183 | Eine Anpassung hat eine Ursache in einem Begegnungs-Datensatz | 11 | Abschnitt 5 |
| N-EN-184 | Eine Anpassung bleibt innerhalb der Artregeln und Schwächen | 11 | N-NE-020 |
| N-EN-185 | Ein toter Nemesis erscheint nicht wieder (außer artspezifische Ausnahme laut Bestiarium) | 11 | N-NE-012 |
| N-EN-186 | Eine zurückkehrende Figur zeigt ihre früheren Verletzungen | 11 | N-NE-013 |
| N-EN-187 | Dasselbe Modell und derselbe Weltzustand liefern dieselbe Anpassung | 11 | Abschnitt 9 |
| N-EN-188 | Ein Nemesis verwendet keine Information über den Spieler, die er nie erhielt | 11 | N-WA-005 |
| N-EN-189 | Die Zahl gleichzeitig aktiver Nemesis-Figuren und ihre Auftrittsabstände sind begrenzt | 11 | Abschnitt 11 |
| N-EN-190 | Ein Nemesis kann keine Hauptmission blockieren | 11 | Abschnitt 12 |
| N-EN-191 | Der Tod eines Anführers löst die Gruppe nicht auf | 11 | Abschnitt 10 |
| N-EN-192 | Ein Nemesis hat keinen Marker in der Spieler-Oberfläche | 11 | Abschnitt 11 |
| N-EN-193 | Die Wirkung einer Rettung oder Verschonung ändert die Beziehungskante nachvollziehbar | 11 | Modul 08 |
| N-EN-194 | Jeder Nemesis hat mindestens ein Vorzeichen (Spur, Gerücht, Handschrift), bevor er auftritt | 11 | Abschnitt 11 |
| N-EN-195 | Ein Nemesis in Gefangenschaft nimmt nicht an Außenaktionen teil | 11 | Modul 12 |
| N-EN-196 | Ein Nemesis mit `story_safe` oder `authored` kann nur durch Spieler oder Geschichte sterben | 11 | Modul 03 §8 |
| N-EN-211 | Jede Phase des Lebenszyklus erzeugt ein Ereignis im Journal | 12 | Abschnitt 2 |
| N-EN-212 | Ein Gegner, der die Begegnung überlebt, behält seine Verletzungen | 12 | N-NL-006, N-NL-007 |
| N-EN-213 | Eine Rückkehr geschieht nur, wenn Motiv, Mittel, Wissen und Gelegenheit bestehen | 12 | Abschnitt 6 |
| N-EN-214 | Eine Rückkehr erfolgt nicht in Story-Sequenzen oder Hauptmissionen, wenn dies die Erzählung stört | 12 | Abschnitt 6.1 |
| N-EN-215 | Die Hintergrundentwicklung ist deterministisch und verbraucht nur Weltzeit | 12 | N-NL-011 |
| N-EN-216 | Eine Hintergrundaktivität erzeugt eine überprüfbare Spur im Weltzustand | 12 | Abschnitt 5 |
| N-EN-217 | Der Nemesis kann Mittel nur aus dem Weltzustand beziehen | 12 | Abschnitt 5 |
| N-EN-218 | Ein verletzter Nemesis tritt nicht vor Heilung oder Kompensation in Konfrontationen auf | 12 | Abschnitt 4 |
| N-EN-219 | Ein Wendepunkt hat eine Ursache im Journal | 12 | Abschnitt 8 |
| N-EN-220 | Der Tod eines Nemesis löscht keine Erinnerungen anderer an ihn | 12 | N-GE-025 |
| N-EN-221 | Eine Anpassung kann durch einen Methodenwechsel des Spielers unwirksam werden | 12 | Abschnitt 7 |
| N-EN-222 | Gleichzeitig aktive Nemesis-Figuren liegen unter der festgelegten Obergrenze | 12 | Abschnitt 11 |
| N-EN-223 | Ein gefangener Nemesis führt keine Außenaktionen aus | 12 | Abschnitt 3 |
| N-EN-224 | Ein Nemesis erscheint nicht in einer Region, die er nicht erreichen kann | 12 | Abschnitt 6.1 |
| N-EN-225 | Ein Nemesis erkennt den Spieler nur mit Hilfe einer Erinnerung oder neuen Wahrnehmung | 12 | N-GE-015 |
| N-EN-226 | Die Verschonung eines Gegners erzeugt eine Beziehungsänderung, deren Richtung von Persönlichkeit und Deutung abhängt | 12 | Abschnitt 8 |
| N-EN-241 | Ein NPC befindet sich zu jeder Weltzeit an genau einem Ort oder unterwegs | 13 | N-LE-014 |
| N-EN-242 | Ein Weg zwischen zwei Orten kostet Weltzeit, auch im Hintergrund | 13 | N-LE-014 |
| N-EN-243 | Der Zeitplan eines NPC ist bei gleichem Weltzustand und gleichem Seed reproduzierbar | 13 | N-LE-015 |
| N-EN-244 | Eine Abweichung vom Normalplan hat eine Ursache und ein Ende | 13 | N-LE-002 |
| N-EN-245 | Ein Arbeitsplatz-NPC erscheint zu den Schichtzeiten am Arbeitsplatz oder hat eine dokumentierte Abweichung | 13 | N-LE-003 |
| N-EN-246 | Eine Materialisierung ist mit der letzten Zustandsaufzeichnung vereinbar | 13 | N-LE-013 |
| N-EN-247 | Beim Betreten eines Gebiets sind Straßen entsprechend der Tageszeit belebt | 13 | N-LE-026 |
| N-EN-248 | Ein NPC reist nur mit einem verfügbaren Verkehrsmittel | 13 | Abschnitt 6 |
| N-EN-249 | Wer verreist ist, ist an seinem Heimatort abwesend | 13 | Abschnitt 6 |
| N-EN-250 | Ein Umzug ist selten und hat Ursachen | 13 | N-LE-009 |
| N-EN-251 | Der tatsächliche Aufenthalt wichtiger Personen ist für Alibi-Prüfungen abrufbar | 13 | Ermittlung 12 §8 |
| N-EN-252 | Ein Betrieb hat Mitarbeiter, die zu seinen Öffnungszeiten arbeiten, oder einen dokumentierten Ausfall | 13 | N-LE-010 |
| N-EN-253 | Feiertage verändern Öffnungszeiten und Routinen deterministisch | 13 | N-LE-018 |
| N-EN-254 | Begleiter und Crew haben Abläufe außerhalb der Anwesenheit des Spielers | 13 | N-LE-020, N-LE-022 |
| N-EN-255 | Die Hintergrundsimulation verbraucht nur Weltzeit | 13 | N-LE-027 |
| N-EN-261 | Jede Änderung im Lebenslauf eines NPCs hat ein Ereignis mit Ursache | 14 | N-LB-011 |
| N-EN-262 | Das Alter eines NPCs ergibt sich aus Geburts- und Weltdatum | 14 | Abschnitt 3 |
| N-EN-263 | Haar- und Bartlänge sind Funktionen der Weltzeit seit dem letzten Schnitt | 14 | N-LB-004 |
| N-EN-264 | Eine Frisur kann nicht länger sein als die vorhandene Haarlänge | 14 | N-LB-004 |
| N-EN-265 | Eine Narbe verschwindet nicht ohne Ereignis | 14 | N-LB-005 |
| N-EN-266 | Ein NPC im Zeitstand T hat sein damaliges Alter | 14 | N-GE-028 |
| N-EN-267 | Ein Umzug ist ein Ereignis mit Ursache und ändert Haushalt, Wohnort, Netz | 14 | N-LE-009 |
| N-EN-268 | Ein verletzter NPC durchläuft die Genesungsstufen in Weltzeit | 14 | N-LB-014 |
| N-EN-269 | Ein schwer Verletzter ohne Behandlung kann sterben | 14 | N-LB-013 |
| N-EN-270 | Die Zahl der Todesfälle ohne Ursache beträgt null | 14 | N-LB-016 |
| N-EN-271 | Ein toter NPC hat einen Datensatz, aber keine Handlungen | 14 | N-ID-018 |
| N-EN-272 | Ein geschützter NPC stirbt nicht außerhalb der erlaubten Wege | 14 | N-LB-017 |
| N-EN-273 | Eine Bevölkerungsbilanz (aktiviert) bleibt innerhalb vorgegebener Grenzen | 14 | N-LB-019 |
| N-EN-274 | Die Todesnachricht erreicht Angehörige über einen existierenden Weg und erzeugt Trauer, Erbe und Haushaltsänderung | 14 | N-LB-016 |
| N-EN-286 | Ein NPC reagiert nur auf Ereignisse, die er wahrgenommen oder erfahren hat | 15 | N-GE-008 |
| N-EN-287 | Dasselbe Ereignis kann verschiedene Reaktionen auslösen, jede ist reproduzierbar | 15 | N-WE-004 |
| N-EN-288 | Die Rolle eines NPC (Beruf, Verantwortung) wirkt auf die Reaktion nachvollziehbar | 15 | Abschnitt 2 |
| N-EN-289 | Meidung entsteht aus Ereignissen und klingt ohne neue Ereignisse ab | 15 | N-WE-001 |
| N-EN-290 | Eine Gemeinschaftsvariable ändert sich nur durch Ereignisse und Gerüchte | 15 | Abschnitt 5 |
| N-EN-291 | Die Polizei beginnt mit der gewöhnlichen Erklärung | 15 | N-WE-003 |
| N-EN-292 | Eine dauerhafte Veränderung eines NPCs hat ein auslösendes Erlebnis | 15 | N-WE-005 |
| N-EN-293 | Große übernatürliche Ereignisse sind selten und zeitlich begrenzt | 15 | N-WE-007 |
| N-EN-294 | Ein Ereignis ohne Beobachter erzeugt keine Reaktion bei NPCs | 15 | N-GE-008 |
| N-EN-295 | Reparaturen werden von NPC-Teams ausgeführt und brauchen Weltzeit | 15 | N-WE-009 |
| N-EN-296 | Ein Schaden bleibt bis zur Reparatur sichtbar | 15 | N-WE-009 |
| N-EN-297 | Das Weltbild eines NPC bestimmt die Deutung, nicht die Wahrheit | 15 | N-WE-004 |
| N-EN-298 | Menschenmengen entstehen aus Ortszustand und Plänen, nicht aus Zufall | 15 | N-WE-011 |
| N-EN-299 | Ein Monsterfall verändert mindestens einen Alltagsparameter einer Gemeinschaft, wenn er öffentlich wahrnehmbar war | 15 | N-WE-008, N-WE-017 |
| N-EN-306 | Keine Hauptmission ist von Fraktionsmitgliedschaft oder Rekrutierung abhängig | 16 | N-FR-003 |
| N-EN-307 | Eine Fraktionsentscheidung folgt der Entscheidungsform der Gruppe, nicht dem Spielerwunsch | 16 | Abschnitt 4 |
| N-EN-308 | Ein Rang ändert sich nur durch ein Ereignis mit Ursache | 16 | N-FR-011 |
| N-EN-309 | Der Rang ist nicht allein durch Zeit oder Erfahrungspunkte bestimmt | 16 | N-FR-012 |
| N-EN-310 | Gruppenruf und individuelle Beziehung können verschieden sein | 16 | N-FR-023 |
| N-EN-311 | Eine Fraktion hat jederzeit eine gültige Führung oder einen dokumentierten Führungsausfall | 16 | Abschnitt 7 |
| N-EN-312 | Ein Mitglied verlässt die Gruppe mit einem Ereignis; Besitz, Rechte und Beziehungen ändern sich nachvollziehbar | 16 | N-FR-018 |
| N-EN-313 | Eine verdeckte Mitgliedschaft ist für Nichteingeweihte nicht erkennbar, aber im Datensatz vorhanden | 16 | Abschnitt 5 |
| N-EN-314 | Fraktionsbeziehungen wechseln nur über Ereignisse und mit Hysterese | 16 | Abschnitt 8 |
| N-EN-315 | Zwei rivalisierende Mitgliedschaften des Spielers erzeugen Prüfung und Folgen | 16 | N-FR-016 |
| N-EN-316 | Eine Fraktion verwendet nur Mittel aus dem Weltzustand | 16 | N-LT-008 |
| N-EN-317 | Gruppenwissen ist durch Rollen begrenzt; ein Mitglied weiß nicht automatisch, was die Führung weiß | 16 | Modul 05 §10 |
| N-EN-318 | Ein Führungswechsel erzeugt Nachfolge, Machtkampf oder Zerfall, nie Leere | 16 | N-FR-015 |
| N-EN-319 | Der Austritt löscht keine Tattoos, Narben oder Erinnerungen | 16 | N-FR-018 |
| N-EN-320 | Ein Spieler in Führungsposition löst Aufgaben selbst; es gibt kein erzwungenes Mikromanagement | 16 | N-FR-022 |
| N-EN-321 | Fraktionen reagieren auf Spielerhandlungen nur, wenn sie davon erfahren haben | 16 | N-GE-008 |
| N-EN-336 | Eine Fraktion hält nur Gebiete, die ihre Mitglieder erreichen können | 17 | N-TE-014 |
| N-EN-337 | Ein Anführer-Tod führt zu Nachfolge, Machtkampf oder Zerfall, nie zum spurlosen Verschwinden der Gruppe | 17 | N-TE-005 |
| N-EN-338 | Eskalationsstufen werden auch in groben Zeitschritten nacheinander durchlaufen | 17 | Abschnitt 5 |
| N-EN-339 | Ein Strategiezustand wechselt nicht ohne Auslöser und Mindestdauer | 17 | Abschnitt 4 |
| N-EN-340 | Relative Macht ist abgeleitet und erscheint nicht in der Spieler-Oberfläche | 17 | Abschnitt 6 |
| N-EN-341 | Die Fernsimulation führt Fraktionen als Zustand; beim Betreten der Region sind Mitglieder konsistent zu diesem Zustand | 17 | N-TE-004, Modul 22 |
| N-EN-342 | Eine Fraktion handelt nur mit Ressourcen, die sie hat | 17 | Abschnitt 3 |
| N-EN-343 | Ein Konflikt hat einen Anlass und eine Ursache im Journal | 17 | Abschnitt 5 |
| N-EN-344 | Folgen eines Konflikts (Vorfälle, Meidung, Gerüchte) treten zeitlich nach dem Konflikt auf | 17 | Abschnitt 7 |
| N-EN-345 | Ein vollständig vernichtetes Nest erscheint nicht sofort wieder | 17 | N-TE-013 |
| N-EN-346 | Monsterterritorien erscheinen nicht als Kartenebene | 17 | Abschnitt 2 |
| N-EN-347 | Menschliche Einflussbereiche sind nur für Mitglieder auf der Karte sichtbar | 17 | N-TE-006 |
| N-EN-348 | Ethnische Zugehörigkeit ist kein Faktor der Kriminalitäts- oder Machtberechnung | 17 | N-TE-008 |
| N-EN-349 | Dieselben Eingaben liefern dieselbe Machtverschiebung | 17 | Abschnitt 9 |
| N-EN-350 | Eine Gruppe wird nicht gleichzeitig an zwei weit entfernten Orten stark simuliert, ohne dass Mitglieder dort sind | 17 | Abschnitt 9 |
| N-EN-366 | Eine Behörde handelt nur auf Grundlage tatsächlich vorliegender Information | 18 | N-BH-006 |
| N-EN-367 | Eine Zuständigkeit bestimmt, welche Behörde eine Meldung bearbeitet; Informationsweitergabe zwischen Behörden braucht einen Weg | 18 | N-BH-001 |
| N-EN-368 | Wiederholte Konflikte mit einer Dienststelle verändern deren Verhalten gegenüber dem Spieler nachvollziehbar | 18 | N-BH-007 |
| N-EN-369 | Eine schwere Fahndung endet nur durch einen definierten Abschluss (Kopfgeld, Rücknahme, Freispruch, Verfahren) | 18 | N-BH-004 |
| N-EN-370 | Die Polizei kennt das Übernatürliche nur, wenn der Datensatz es erlaubt | 18 | N-BH-008 |
| N-EN-371 | Ein Beamter handelt nach Persönlichkeit, Gedächtnis und Beziehung, nicht nur nach Dienstregel | 18 | Abschnitt 3 |
| N-EN-372 | Ein NPC-Hunter folgt eigenen Zielen; er löst keine Fälle zum Vorteil des Spielers, nur weil dieser sie ignoriert | 18 | N-BH-013 |
| N-EN-373 | Ein NPC-Hunter kann sterben und hinterlässt Spuren (Erinnerungen, Archivlücken, Fälle) | 18 | N-BH-013 |
| N-EN-374 | Ein verschontes Wesen wird nicht gezwungen, Feind zu werden | 18 | N-BH-016 |
| N-EN-375 | Das Wissen einer Behörde ist ein Pool mit Rollenrechten | 18 | Modul 05 §10 |
| N-EN-376 | Staatliche Geheimprogramme sind Spielinhalt der Welt, nicht Pflicht für Hauptstory-Abläufe | 18 | N-BH-018 |
| N-EN-377 | Medienberichte und Behördenmitteilungen widersprechen sich nur mit erkennbarem Grund | 18 | N-BH-017 |
| N-EN-378 | Wissenszentrierte Organisation weiß nicht alles | 18 | N-BH-012 |
| N-EN-379 | Ein Wesen mit menschlicher Identität hat auffindbare Spuren dieser Identität | 18 | Kreaturen 09 |
| N-EN-380 | Die Aufdeckung eines Geheimnisses ändert Beziehungskanten nachvollziehbar | 18 | Abschnitt 8 |
| N-EN-391 | Ein Begleiter lehnt eine Handlung nur aus Persönlichkeit, Doktrin oder Lage ab, mit sichtbarem Grund | 19 | N-CO-004 |
| N-EN-392 | Ein Begleiter weicht nur bei unmittelbarer Gefahr oder neuer Information vom Plan ab | 19 | N-CO-011 |
| N-EN-393 | Ein Begleiter sabotiert nie zufällig | 19 | N-CO-011 |
| N-EN-394 | Ein verletzter Begleiter steht in der Zeit seiner Genesung nicht zur Verfügung | 19 | N-CO-006 |
| N-EN-395 | Ein gewöhnlicher Begleiter bleibt nach dem Tod tot | 19 | N-CO-007 |
| N-EN-396 | Ein Kernmitglied des HQ stirbt nicht durch Sandbox-Zufall | 19 | N-CO-007 |
| N-EN-397 | Ein Todesfall in der Crew führt zu Reaktionen von Crew, HQ und Beziehungen | 19 | N-CO-007, N-LB-016 |
| N-EN-398 | Persönliche Anfragen entfallen nur durch Tod oder dauerhaftes Gehen | 19 | N-CO-017 |
| N-EN-399 | Crew-Aktivitäten verschwinden nicht durch ein verpasstes Zeitfenster | 19 | N-CO-018 |
| N-EN-400 | Nur als rekrutierbar definierte NPCs können rekrutiert werden | 19 | N-CO-022 |
| N-EN-401 | Rekrutierung folgt einer Kette von Beziehung, Wissen, Gelegenheit und Entscheidung | 19 | Abschnitt 7 |
| N-EN-402 | Ein Rekrut bringt die Fähigkeiten, Beziehungen und Konflikte seiner Herkunft mit | 19 | N-CO-023 |
| N-EN-403 | Die Crew reagiert auf einen Rekruten nach Persönlichkeit und Erinnerung, nicht einheitlich | 19 | N-CO-023 |
| N-EN-404 | Crewmitglieder haben Abläufe außerhalb der Anwesenheit des Spielers | 19 | N-CO-015 |
| N-EN-405 | Der Hund ist umweltbezogener Spurenfolger, kein Zielpfeil | 19 | N-CO-014 |
| N-EN-406 | Kein Hauptmissionsabschnitt hängt von einem Rekruten ab | 19 | N-CO-021 |
| N-EN-407 | Ein Begleiter, der abwesend ist, wird bei Rückkehr aus seinem Zustand konsistent dargestellt | 19 | N-LE-013 |
| N-EN-408 | Ein Begleiter erinnert Rettung, Verlassen, Lüge und Verletzungen wegen Entscheidungen | 19 | N-BV-004 |
| N-EN-421 | Dialogbedingungen verwenden Bänder und Tags, keine Rohzahlen | 20 | N-DI-002 |
| N-EN-422 | Eine Änderung eines Beziehungsbandes ist über mindestens einen Kanal wahrnehmbar | 20 | N-DI-017 |
| N-EN-423 | Keine Zeile zeigt eine Beziehungszahl, Prozent oder Balken | 20 | N-LT-012 |
| N-EN-424 | Ein Erinnerungs-Hook nennt nur Ereignisse, die im Weltzustand existieren und die der NPC wahrgenommen hat | 20 | N-GE-008 |
| N-EN-425 | Ein Gespräch behauptet nichts, was der Dialogkontext nicht stützt | 20 | N-DI-016 |
| N-EN-426 | Dieselbe Eingabe liefert dieselbe Zeilenauswahl | 20 | Modul 22 |
| N-EN-427 | Es gibt für jedes Thema eine Fallback-Zeile | 20 | Abschnitt 3 |
| N-EN-428 | Ein Hook wird nicht bei jedem Gespräch wiederholt | 20 | Abschnitt 5 |
| N-EN-429 | Erinnerungsunsicherheit wirkt auf die Formulierung | 20 | N-GE-011 |
| N-EN-430 | Eine Zeile mit geändertem Text markiert vorhandene Aufnahmen als veraltet | 20 | N-DI-014 |
| N-EN-431 | Die Inspektoren sind im Shipping-Modus nicht erreichbar | 20 | N-DI-018, N-DI-019 |
| N-EN-432 | Das Dialogsystem verändert den Weltzustand ausschließlich über Ereignisse | 20 | Abschnitt 2 |
| N-EN-433 | NPCs ohne Wissen über ein Thema antworten nicht, als wüssten sie es | 20 | N-GE-008 |
| N-EN-434 | Wichtige Zustände sind nicht ausschließlich über Ton oder Farbe vermittelt | 20 | Abschnitt 6 |
| N-EN-441 | Gleicher Weltseed und gleiche NPC-ID liefern auf allen Plattformen denselben abgeleiteten Standardzustand | 21 | N-PE-007, N-PE-008 |
| N-EN-442 | Ein unverändert gebliebener NPC erzeugt keinen Speicher außer ID/Seed-Bezug | 21 | N-PE-007 |
| N-EN-443 | Speichern und Laden ergibt gleichen Zustand (Round-Trip) für alle Segmente | 21 | N-PE-022 |
| N-EN-444 | Ein alter Spielstand wird deterministisch migriert und danach in der neuen Version unverändert geladen | 21 | N-PE-011 |
| N-EN-445 | Ein unbekanntes Segment oder eine unbekannte Datenart wird unverändert gespeichert | 21 | N-PE-018 |
| N-EN-446 | Ein abgebrochener Schreibvorgang lässt den letzten gültigen Spielstand intakt | 21 | N-PE-003 |
| N-EN-447 | Jeder Verweis in einem Segment zeigt auf einen existierenden Datensatz oder einen Tombstone | 21 | Abschnitt 8 |
| N-EN-448 | Verdichtung entfernt keinen Anker und kein Ereignis, auf das ein Fall verweist | 21 | Abschnitt 4.2 |
| N-EN-449 | Der abgeleitete Zustand aus dem Journal gleicht dem gespeicherten Cache | 21 | Abschnitt 4.3 |
| N-EN-450 | Ein eingefrorener NPC wird durch eine Generatorversion nicht verändert | 21 | N-PE-014 |
| N-EN-451 | Zeitsprünge verbrauchen Weltzeit, keine Realzeit, und holen Ereignisse nach | 21 | N-PE-013 |
| N-EN-452 | Der Tod eines NPCs bleibt nach Speichern und Laden erhalten | 21 | N-PE-015 |
| N-EN-453 | Ein Spielstand mit fehlendem kritischem Mod lädt nicht ohne Expertenübersteuerung nach Backup | 21 | N-PE-018 |
| N-EN-454 | Hochfrequenzzustände (Positionen, Animation) werden nicht gespeichert | 21 | N-PE-020 |
| N-EN-455 | Gespeicherte Datenmenge steigt nicht mit der Zahl unveränderter Identitäten | 21 | N-PE-007 |
| N-EN-456 | Eine Obergrenze je NPC verhindert unbegrenztes Wachstum, ohne Anker zu verlieren | 21 | Abschnitt 5.3 |
| N-EN-457 | Jedes Segment trägt eine Schemaversion | 21 | Abschnitt 6.3 |
| N-EN-458 | Der Weltzustand lädt eine Region nicht vor dem Manifest | 21 | Abschnitt 6.2 |
| N-EN-461 | Das Nachholen von Ereignissen für nicht geladene Entitäten folgt einer definierten Reihenfolge (Weltzeit, dann ID) und liefert dasselbe Ergebnis wie der Live-Verlauf | 21 | Abschnitt 7 |
| N-EN-471 | Fortschreiben ist reihenfolgeunabhängig: Das Ergebnis hängt von Seed, Fenster und Weltzustand ab, nicht von der Aufrufreihenfolge | 22 | N-SI-012 |
| N-EN-472 | Ein Ereignis wird erst verarbeitet, wenn alle Ereignisse mit früherem Zeitstempel für diesen Empfänger verarbeitet sind | 22 | Abschnitt 3.3 |
| N-EN-473 | Ein Zustandswechsel S0↔S1↔S2↔S3 verliert keine wichtigen Ergebnisse (Verletzung, Zusage, Gedächtnis) | 22 | N-SI-018 |
| N-EN-474 | Die Materialisierung ist mit dem letzten Zustand vereinbar (Ort, Aktivität, Zustand) | 22 | N-SI-005 |
| N-EN-475 | Beim Betreten eines Gebiets ist die Straße der Tageszeit gemäß belebt | 22 | N-SI-005 |
| N-EN-476 | Kein NPC ist in der Simulation an zwei Orten gleichzeitig | 22 | N-SI-011 |
| N-EN-477 | Wege kosten Weltzeit, auch ohne Route | 22 | N-SI-011 |
| N-EN-478 | Das Fortschreiben läuft nie der Weltuhr voraus | 22 | N-SI-019 |
| N-EN-479 | Dieselben Eingaben liefern auf Windows und Linux dieselben Weltzustandsentscheidungen | 22 | Abschnitt 7 |
| N-EN-480 | Ein NPC im Gespräch wechselt nicht die Stufe | 22 | Abschnitt 5 |
| N-EN-481 | Die Weltgröße ist in keiner Simulationsgröße fest verdrahtet | 22 | N-SI-001 |
| N-EN-482 | Unter Last verändert sich Darstellung und Dichte, nicht der Weltzustand | 22 | Abschnitt 8 |
| N-EN-483 | Lange Pausen prüfen Eskalationsstufen nacheinander | 22 | Abschnitt 3.4 |
| N-EN-484 | Die Kosten der Fortschreibung wachsen nicht mit der Zahl unveränderter Identitäten | 22 | N-PE-007 |
| N-EN-485 | Die Simulation läuft nur auf der Autorität (Host) | 22 | Abschnitt 10 |
| N-EN-486 | Ein NPC mit höherer Priorität verdrängt einen mit niedrigerer aus dem Budget, nie umgekehrt | 22 | Abschnitt 5 |
| N-EN-487 | Hysterese verhindert Stufenwechsel bei Grenzbewegung | 22 | Abschnitt 5 |
| N-EN-488 | Der Replay-Test eines Zeitraums liefert denselben Zustand | 22 | Abschnitt 7 |
| N-EN-489 | Gleichzeitig materialisierte Personen derselben Gruppe wechseln gemeinsam die Stufe | 22 | Abschnitt 5 |
| N-EN-501 | Ein NPC existiert pro Welt genau einmal und ist für alle Spieler derselbe | 23 | Abschnitt 2 |
| N-EN-502 | Beziehungskanten zu verschiedenen Spielern sind unabhängig | 23 | N-KO-005 |
| N-EN-503 | Gastfortschritt (Beziehungen, Erinnerungen, Ruf) ist im Host-Spielstand gespeichert und wird nicht in den Gast-Spielstand übernommen | 23 | N-KO-001, N-KO-002 |
| N-EN-504 | Ein Welt-Zeitsprung im Koop findet nur mit Zustimmung aller aktiven Spieler statt | 23 | N-KO-008 |
| N-EN-505 | Individuelle Verhaftung eines Spielers hält die Welt nicht an | 23 | N-KO-008 |
| N-EN-506 | Ein Mod-Datensatz wird bei Deaktivierung nicht gelöscht und bei Reaktivierung unverändert wiederverwendet | 23 | N-KO-013 |
| N-EN-507 | Ein Spielstand mit unbekannten Datenarten lädt und speichert sie unverändert | 23 | N-KO-017 |
| N-EN-508 | Mod-IDs kollidieren nicht mit Kern-IDs | 23 | Abschnitt 6 |
| N-EN-509 | Kein Mod bricht Hauptstory-Pfade oder Schutzstufen | 23 | Abschnitt 6 |
| N-EN-510 | Mod-Inhalte unterliegen denselben Simulationsbudgets | 23 | Abschnitt 6 |
| N-EN-511 | Das Offizielle Online lädt keine Mod-Daten | 23 | N-KO-004 |
| N-EN-512 | Die Simulation läuft nur auf der Autorität | 23 | N-KO-018 |
| N-EN-513 | Ein Gast ohne Begegnung mit einem NPC hat bei diesem keine Erinnerung | 23 | N-GE-008 |
| N-EN-514 | Eine Teamentscheidung wirkt auf NPCs nur über wahrgenommene Handlungen | 23 | Abschnitt 4 |
| N-EN-515 | Die Altersregeln des Adult-Systems sind durch Mods nicht abschaltbar | 23 | Abschnitt 7 |
| N-EN-521 | Jede Tatsache hat genau einen Besitzer-Speicher | 24 | N-KS-002 |
| N-EN-522 | Leser kopieren keine Tatsachen; sie verweisen per ID | 24 | Abschnitt 2 |
| N-EN-523 | Systemübergreifende Wirkungen entstehen nur als Ereignisse, nicht als Direktschreibzugriff | 24 | N-KS-003 |
| N-EN-524 | Die Verarbeitung der Ereignisse ist idempotent und in definierter Reihenfolge | 24 | Abschnitt 3.2 |
| N-EN-525 | Ein Folgeereignis hat nie einen früheren Zeitstempel als seine Ursache | 24 | Abschnitt 3.2 |
| N-EN-526 | Zusammengehörige Änderungen (Tod: Lebenszustand, Haushalt, Fall) gelten alle oder keine | 24 | Abschnitt 3.2 |
| N-EN-527 | Der Invariantenkatalog (Abschnitt 4) läuft als automatisierter Test | 24 | N-KS-004 |
| N-EN-528 | Ein Autorenschutz verhindert systemische Ereignisse, die ihm widersprechen, und vermerkt sie als blockiert | 24 | Abschnitt 7 |
| N-EN-529 | Kein sichtbarer Widerspruch zwischen Systemen nach dem Laden einer Region | 24 | N-KS-005 |
| N-EN-530 | Die Umsetzungsreihenfolge beginnt erst, wenn die Mindest-Eingaben existieren | 24 | N-KS-012 |
| N-EN-531 | Ein Fall zitiert nur Ereignisse, die im Journal stehen | 24 | N-KS-004 |
| N-EN-532 | Eine Verletzung ist in jedem System sichtbar, das die Person darstellt | 24 | N-KS-004 |
| N-EN-533 | Mitgliedschaft in Fraktions- und NPC-Datensatz stimmt überein | 24 | Abschnitt 2 |
| N-EN-534 | Ein Ereignis ohne Besitzer wird abgelehnt | 24 | Abschnitt 9 |
| N-EN-535 | Dieselbe Ereignisfolge ergibt auf jeder Plattform dasselbe Ergebnis | 24 | N-SI-012 |

---

## Zählung

| Gruppe | Anzahl |
|---|---:|
| LT | 35 |
| ID | 24 |
| PS | 18 |
| GE | 32 |
| WA | 21 |
| IN | 22 |
| BE | 24 |
| BV | 23 |
| FA | 19 |
| NE | 22 |
| NL | 15 |
| LE | 28 |
| LB | 25 |
| WE | 19 |
| FR | 29 |
| TE | 18 |
| BH | 20 |
| CO | 29 |
| DI | 22 |
| PE | 23 |
| SI | 24 |
| KO | 19 |
| KS | 12 |
| EN | 320 |
| **gesamt** | **843** |
