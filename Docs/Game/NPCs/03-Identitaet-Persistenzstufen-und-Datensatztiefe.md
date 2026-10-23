# NPCs 03: Identität, Persistenzstufen und Datensatztiefe

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wer ein NPC ist, wie lange und wie genau die Welt ihn behält und wie ein NPC zwischen den Stufen wechselt. Die Stufen A–D und die Grundregeln sind beschlossen; **Felder, Schwellen und Übergangsformeln sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Stabile Identität und Seed](#2-stabile-identität-und-seed)
3. [Entwurf: Bestandteile eines NPC-Datensatzes](#3-entwurf-bestandteile-eines-npc-datensatzes)
4. [Persistenzstufen A bis D](#4-persistenzstufen-a-bis-d)
5. [Datensatztiefe: von ‚gesehen‘ bis ‚Full‘](#5-datensatztiefe-von-gesehen-bis-full)
6. [Entwurf: Die drei Achsen zusammen](#6-entwurf-die-drei-achsen-zusammen)
7. [Entwurf: Lebenszustand](#7-entwurf-lebenszustand)
8. [Geschützte NPCs](#8-geschützte-npcs)
9. [Entwurf: Wer steigt auf, wer ab](#9-entwurf-wer-steigt-auf-wer-ab)
10. [Bevölkerungserzeugung für Stufe C und D](#10-bevölkerungserzeugung-für-stufe-c-und-d)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

Alle Nummern in der Spalte ‚Quelle‘ beziehen sich auf die nummerierten Fragen des Assistenten in `23#1056A` (Block 8), die der Entwickler in `23#1057E` einzeln beantwortet hat, beziehungsweise auf `35#260A`/`35#278A` mit den Antworten `35#261E`/`35#279E`.

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

## 2. Stabile Identität und Seed

**Quellenaussage (📋):** Jeder NPC hat eine globale ID, die nicht von seiner Anwesenheit im Speicher abhängt (N-ID-008). Der Seed gehört zur ID (N-ID-009). Der Weltzustand soll mit stabilen IDs arbeiten (`M-PD` ‚World state and persistence‘: ‚Use stable IDs for important persistent entities‘).

**Entwurf (💡):**

| Eigenschaft | Vorschlag | Begründung |
|---|---|---|
| ID-Form | `npc.<region>.<laufende Nummer>` oder für handgeschriebene Figuren ein sprechender Name im Raum `[a-z][a-z0-9_.]*`, höchstens 96 Zeichen | passt zur technischen Regel D2 ([Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) §2.1) und zur Schlüsselgrenze von `WorldFacts` v1 |
| Unveränderlichkeit | Die ID ändert sich nie, auch nicht bei Heirat, Umzug, Namenswechsel, Verwandlung oder Tod | sonst brechen Beziehungen, Erinnerungen und Fälle |
| Wiederverwendung | Eine ID wird nie neu vergeben, auch nicht nach dem Tod | Fälle und Erinnerungen verweisen auf sie |
| Seed | `Seed(npc) = f(World-Seed, NPC-ID)`; unverändert, solange der NPC nicht eingefroren wurde | N-ID-009, N-ID-020 |
| Einfrieren | Zum Zeitpunkt der ersten relevanten Begegnung wird der aktuell abgeleitete Zustand als Abweichung gespeichert | N-ID-013 |
| Generatorversion | Der Datensatz kennt die Version des Generators, der ihn erzeugte; Generator-Updates ändern eingefrorene NPCs nicht | N-ID-013 |
| Mods | Mod-NPCs erhalten ID-Präfixe der Mod-Kennung; unbekannte Datensätze bleiben beim Speichern unverändert erhalten | [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §11 |
| Zeitreise | Dieselbe ID in verschiedenen Zeitständen verweist auf den jeweiligen Weltzustand | N-ID-024-Kontext; [Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md) |

**Warum der Seed nicht von der Position abhängt** (die Antwort des Entwicklers, `35#261E` Punkt 3): Würde der Seed vom Spawnort abhängen, könnte ein Umzug die Persönlichkeit einer Partnerin ändern.

---

## 3. Entwurf: Bestandteile eines NPC-Datensatzes

> 💡 **Vorschlag.** Er ordnet die beschlossenen Bausteine in Gruppen. Feldnamen sind unverbindlich.

```text
NpcRecord
├── identity         id, seed, generator_version, name, birth, origin_region, gender/age-class
├── persistence      tier (A-D), record_depth (minimal | known | detailed | full), promoted_at, pinned
├── life_state       alive | injured | sick | missing | captured | dead | transformed (siehe 7)
├── household        household_id, role, home_id, rent/own, co_residents[]
├── work             employer_id, job, shift pattern, income class
├── mobility         vehicle_id(s), typical routes, travel habits
├── appearance       recipe ref (Decision 0005), outfit set, overrides (scars, tattoos, hair length)
├── personality      core (slow), traits, communication style (M-OD offen)
├── state            fast: mood, stress, confidence; mid: trust, familiarity ...; long: attachment, resentment, habits
├── memory           events[], knowledge[], beliefs[] (Modul 05, 06)
├── relations        edges[] (Modul 08, 09); only for relevant persons
├── affiliations     faction memberships, rank, loyalty (Modul 16)
├── goals            current goals, plans (Modul 13, 14)
├── injuries         persistent injuries/scars (Spielvision 14)
├── possessions      home, vehicles, pets, items (N-ID-007)
├── life_log         major life events (Modul 14)
└── flags            protected, story_critical, recruitable, nemesis_state (Modul 11)
```

**Quellenbezug:** Wohnung, Arbeit, Fahrzeug, Kleidung, Besitz: N-ID-004 bis N-ID-007. Persönlichkeit, Zustände: `35#268A` Fragen 1–4 (Kern, schnelle, mittlere, lange Zustände), siehe [Modul 04](04-Persoenlichkeit-Zustaende-und-Zeitskalen.md). Gedächtnisarten: `23#51A` ‚Unterschiedliche Erinnerungsarten‘ (kurz, Ereignis, Wissen, Beziehung), siehe [Modul 05](05-Gedaechtnis-Erinnerung-und-Vergessen.md). Der Aufbau aus Gruppen statt einer flachen Liste folgt dem Gedanken, dass nur **Abweichungen vom Standard** gespeichert werden (N-ID-020).

**Datenschutz im Spiel:** Der Datensatz ist Weltzustand, kein Spielerwissen. Was der Spieler sieht, kommt aus dem Archiv ([Ermittlung 04](../Ermittlung/04-Archiv-Journal-und-Hunter-Record.md)); der Datensatz selbst ist nie sichtbar (`M-PD` ‚Hunter Archive epistemology‘ gilt analog).

---

## 4. Persistenzstufen A bis D

| Stufe | Wer (📋 Quelle) | Zustand | Typische Folgen im Spiel | Beschlossen |
|---|---|---|---|---|
| **A** | Story-, Hunter-, Nebenfall-NPCs und enge Beziehungsfiguren | maximale Persistenz und Simulation | volle Erinnerung, individuelle Beziehungen, Lebensverlauf, Tod und Verletzung dauerhaft | `23#1056A` Frage 121; „ja“ in `23#1057E`; `23#1058A` |
| **B** | Händler, Nachbarn, Arbeitgeber, wiederkehrende Personen | starke Persistenz | Gedächtnis an den Spieler, Wohn- und Arbeitsroutine, eigene Beziehungen | ebenda |
| **C** | normale lokale Einwohner | Name, Haushalt, Job, Fahrzeug, vereinfachte Routine und Erinnerung | erkennen den Spieler nach relevanter Begegnung, vergessen Unwichtiges | ebenda; `23#1058A` ‚Technische NPC-Architektur‘ |
| **D** | Crowds und Füll-NPCs | billig simuliert, in der Ferne abstrahiert; temporär | keine eigene Historie; rekonstruiert aus dem Zustand | ebenda; `23#1056A` Frage 17 (‚Tier D: reine Crowd, nur temporär‘) |

**Praxisregel (📋):** Betritt der Spieler ein Stadtviertel, darf es nicht leer sein, nur weil seine Bewohner vorher abstrahiert waren: „ja sowieso nicht, aber wenn man dann in das disktikt kommt darf die straße nicht leer sein, sondern auch voll“ (`23#1057E` Punkt 122; `23#1058A` ‚Technische NPC-Architektur‘).

**Entwurf (💡) zur Abgrenzung:**

| Frage | Vorschlag |
|---|---|
| Wer ist B statt C? | Wer dem Spieler **wiederholt** begegnet oder im Alltag eine Rolle spielt, die andere NPCs oder Fälle berührt (Verkäufer, Nachbar, Vorgesetzter, Fahrer) |
| Wann wird aus C ein A? | Wenn der NPC Teil eines Falls wird, Hunter-Wissen erlangt, mit dem Spieler eine Beziehung eingeht oder rekrutiert wird (siehe Abschnitt 9) |
| Was ist mit D? | D-Personen haben **keine** ID im Weltzustand. Spricht der Spieler eine D-Person an, wird aus ihr aus dem Zustand des Ortes ein C-NPC *materialisiert* (Abschnitt 10) |
| Sind Tiere NPCs? | Nur der Hund als Begleiter und ausgewählte Tiere (Haustiere wichtiger NPCs); Wildtiere sind Ökologie ([Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md)) |

---

## 5. Datensatztiefe: von ‚gesehen‘ bis ‚Full‘

Die vier Tiefen stammen aus der Ausformulierung des Assistenten in `35#262A` (N-ID-016). Die Zeichen unten sind **Entwurf (💡)**, die vier Namen sind Quelle.

| Tiefe | Auslöser (Quelle) | Inhalt (Entwurf) | Verdichtung |
|---|---|---|---|
| **Minimal Record** | ‚Random NPC gesehen‘ | ID, Seed, Ort der Begegnung, Zeitpunkt; keine Beziehung | stark verdichtbar |
| **Known NPC Record** | ‚kurzes Gespräch‘ | zusätzlich Name (falls genannt), Eindruck, Anlass | verdichtbar nach langer Zeit |
| **Detailed Persistent Profile** | ‚relevante Begegnung‘ | eingefrorener Zustand (N-ID-013), erlebte Ereignisse einzeln, Eindruck des Spielers, relevante Absprachen | Kleinkram verdichtbar, Einzelereignisse bleiben |
| **Full Persistent Profile** | ‚Beziehung / wichtiger NPC‘ | alles aus Abschnitt 3, volle Historie | nur technischer Detailzustand verdichtbar |

**Übergangsregeln (Entwurf):**

1. **Aufstieg nur durch Ereignis.** Ein Gespräch, das nichts über Beziehung, Wissen oder Konsequenz aussagt, hebt nicht an. Ein Datensatz wird nie *wegen Zeitablauf* tiefer.
2. **Kein Abstieg bei Spielerbezug.** Wer Ereignisse mit dem Spieler hat, die als wichtig markiert sind (N-GE-005, siehe Modul 05), wird nicht unter Detailed herabgestuft. Das ist die Quelle `35#279E` Punkt 36: „ja solange der npc sich an alles einzeln erinnern kann“.
3. **Herabstufung erhält Identität.** Herabstufung verdichtet Detailzustand, nie Identität, Beziehungsgeschichte oder wichtige Einzelereignisse (N-ID-015).
4. **Einfrieren bei Aufstieg.** Der Aufstieg zu Detailed friert den Zustand ein (N-ID-013).

**Wichtig:** Die Quellen sagen **nicht**, wie lange ‚Jahrzehnte irrelevant‘ ist, und nennen keine Obergrenze für Detailed- oder Full-Profile. Beides ist offen (❓ Q-N05).

---

## 6. Entwurf: Die drei Achsen zusammen

Persistenzstufe, Simulationsstufe und Datensatztiefe sind verschiedene Achsen ([Modul 01](01-Quellenlage-und-Methodik.md) §4). Das folgende Schema zeigt, welche Kombinationen sinnvoll sind (💡).

| | Sim 0 (beim Spieler) | Sim 1 (lokal aktiv) | Sim 2 (regional) | Sim 3 (dormant) |
|---|---|---|---|---|
| **A, Full** | Spielfigur-nah, voller Actor | voller Actor in Sicht, sonst reduziert | Zeitplan mit Ereignissen | Zustand + Ereignisfortschreibung |
| **B, Detailed** | voller Actor | reduzierter Actor | Zeitplan | Zustand |
| **C, Known/Detailed** | Gespräch möglich, einfache Routine | einfache Routine | Zeitplan | Zustand |
| **D** | materialisiert aus Ortszustand | Menge | statistisch | existiert nicht einzeln |

Daraus folgt (💡): Ein C-NPC *im Gespräch mit dem Spieler* ist in Simulation 0, bleibt aber Persistenz C; sein Datensatz steigt nur, wenn Ereignisse es rechtfertigen.

---

## 7. Entwurf: Lebenszustand

Die Quellen verlangen: Verletzungen, dauerhafte Schäden, Tod, Erinnerung an Tote, Aufenthalt außerhalb der Welt (Haft, Krankenhaus) (`23#1056A` Fragen 100–104; `35#272A` Fragen 31–39). Ein Lebenszustand ordnet das (💡).

| Zustand | Bedeutung | Beispiel | Übergänge |
|---|---|---|---|
| `alive` | handlungsfähig | Alltag | → `injured`, `sick`, `missing`, `captured`, `dead` |
| `injured` | vorübergehend eingeschränkt, heilt | Knochenbruch | → `alive` (Heilung), → `alive` mit Dauerfolge (Narbe, Behinderung), → `dead` |
| `sick` | krank ohne Verletzung | Grippe, Sucht | → `alive`, → `dead` |
| `hospitalized` | im Krankenhaus | nach Unfall | → `alive`, → `dead` |
| `missing` | Aufenthalt unbekannt | Vermisstenfall | → `alive`, → `dead`, bleibt offen |
| `captured` | festgehalten (Haft, Gefangennahme) | Haft, Verlies | → `alive`, → `dead` |
| `transformed` | verwandelt oder besessen | Besessenheit, Fluch | → `alive`, → `dead` (siehe [Kreaturen 05](../Kreaturen/05-Geister-Besessenheit-und-Fluecher.md)) |
| `dead` | tot, bleibt tot | | endgültig (außer artspezifische Ausnahme für Wesen, siehe [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md)) |

**Regeln (Entwurf):**

1. **Dauerfolgen sind Zustand, nicht Fehler.** Narben, verlorene Gliedmaßen und Einschränkungen bleiben im Datensatz (`23#1057E` Punkt 101: „ja sicher, große narben, eine hand verloren, alles soll es geben“). Sie wirken auf Aussehen, Beruf, Fahrzeug und Dialog.
2. **Tod ist endgültig im Weltzustand** (N-ID-018). Das Aussehen im Sarg, Friedhof, Beerdigung und Trauer sind Folgen, keine Zustände ([Modul 14](14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md)).
3. **Vermisst ist kein Tod.** Ein Vermisstenfall kann ewig offen bleiben; die Identität bleibt und kann auftauchen.
4. **Ein Verwandelter bleibt dieselbe ID**, auch wenn seine Rolle sich ändert.

---

## 8. Geschützte NPCs

Die Quellen verlangen Schutz **vor trivialen Simulationsunfällen**, nicht Unsterblichkeit:

| Aussage | Status | Quelle |
|---|:---:|---|
| Wichtige persistente NPCs sollen nicht durch einen banalen Verkehrs-AI-Fehler sterben; sie bekommen Schutz gegen triviale Simulationsunfälle | 📋 | `23#1056A` Frage 105; „wichtige npcs sollten eher nicht sterben außer du tötest sie, würde ich sagen“ in `23#1057E`; `23#1058A` |
| Kernfiguren des Hunter-HQ: kein zufälliger Sandbox-Tod, wenn dadurch die Story zerstört würde; stirbt eine Hauptfigur laut Story, **bleibt sie tot** | 📋 | `23#1090A` ‚Tod‘; „ja wenn es ein story tod ist dann ist er tod“ in `23#1089E` Punkt 26 |
| Normale und rekrutierbare Begleiter können dauerhaft sterben; für „den richtigen aus dem hunter hq“ gilt eine Ausnahme | 📋 | „würde ich schon machen außer bei den richtigen aus dem hunter hq“ in `23#1089E` Punkt 25 |
| Ein anderer Hunter kann ein friedliches Wesen töten; die Welt hängt nicht allein von der moralischen Entscheidung des Spielers ab | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |

**Ungeklärt (⚠️ [Modul 27](27-Konflikte-und-offene-Fragen.md) C-N1):** Wer außer dem Spieler einen *wichtigen* NPC töten darf. Der Entwickler sagte „außer du tötest sie“ (nur der Spieler?); das Proposal verlangt aber unabhängige Lebensentwicklung (F) und Fraktionskonflikte (D) mit Konsequenzen (E); der Master führt, dass NPC-Hunter sterben können (Kreaturen 11 §3).

**Entwurf (💡) – Schutzstufen:**

| Schutz | Wer | Was verhindert wird | Was erlaubt bleibt |
|---|---|---|---|
| `none` | D, die meisten C | – | alles |
| `sim_safe` | B, wichtige C | Tod durch reine Simulationsartefakte (Verkehrsfehler, Physikbug, Stolpern) | Tod durch den Spieler, Gewalt, Krankheit im Rahmen der Regeln |
| `story_safe` | Kernfiguren (Hunter-HQ) | zufälliger Sandbox-Tod | Tod durch Spieler *und* Tod durch die Story |
| `narrative_only` | ausgewählte A-Figuren | jeder Tod außerhalb einer geschriebenen Szene | der Tod innerhalb dieser Szene |

Die Zuordnung ist **Entwurf**; die Quellen nennen nur die zwei Pole.

---

## 9. Entwurf: Wer steigt auf, wer ab

| Auslöser | Von → nach (Entwurf) | Quelle/Grundlage |
|---|---|---|
| Der Spieler führt ein längeres Gespräch, bekommt Wissen | C/D → Known | N-ID-016 |
| Zeuge eines Falls | → Detailed (A für die Falldauer) | `23#1056A` Frage 17; Stufe A umfasst Nebenfall-NPCs |
| Relevante Begegnung (Hilfe, Bedrohung, Rettung, Diebstahl, Lüge) | → Detailed | `23#1056A` Frage 23 |
| Beziehung (Freundschaft, Partnerschaft, Feindschaft) | → Full | N-ID-016 |
| Rekrutierung in die Crew | → A | `23#677E`; Modul 19 |
| Fraktionsführer oder wichtiges Mitglied | B/A | Modul 16 |
| Überlebender Gegner mit Nemesis-Potenzial | → A/B | [Modul 11](11-Nemesis-Modell-und-Datensatz.md) |
| Jahrzehnte ohne Bezug zum Spieler | A/B → herabstufbar, Historie bleibt | N-ID-015 |
| Tod | Identität und Historie bleiben, Detailzustand wird verdichtet | N-ID-018 |

**Nicht Quelle:** Zahlen (wie viele A/B/C), Zeiten (wie viele Spielstunden), Gewichte.

---

## 10. Bevölkerungserzeugung für Stufe C und D

**Quelle:** Die Planungsgröße sind mehrere Hunderttausend NPCs (`35#227E` ‚500000 npcs‘ als Entwickleranahme, `35#278A`; [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13: 🧭 und ❓ offen). Die Begründung der Skalierung ist in [Modul 22](22-Simulationsstufen-Skalierung-und-Performance.md).

**Entwurf (💡):**

1. **Rezept, nicht Datensatz.** Ein C-NPC besteht aus ID, Seed und Überschreibungen (N-ID-020); das Aussehen kommt aus dem Rezept ([Decision 0005](../../Decisions/0005-character-appearance-foundation.md)).
2. **Haushalte zuerst.** Die Bevölkerung wird in Haushalten erzeugt (Wohnung, Familienmitglieder, Arbeitsplatz), damit Familien, Nachbarn und Arbeitsplätze konsistent sind ([Modul 10](10-Familien-Haushalte-und-soziale-Netzwerke.md)).
3. **Korrelierte Eigenschaften.** Persönlichkeit, Beruf, Alter, Einkommen und Region hängen statistisch zusammen (N-ID-011). Der Generator verwirft oder entschärft unmögliche Kombinationen.
4. **Regionalbias statt Regionalregel** (N-ID-021). Der Spieler sieht den Bias nur über Stimme, Aussehen, Stil, Beruf und Umgebung.
5. **Materialisierung aus dem Ortszustand.** Eine D-Person, die angesprochen wird, wird aus dem Zustand ihres Ortes (Tageszeit, Anlass, Zusammensetzung der Menge) zu einem C-NPC; erst jetzt entsteht eine ID.
6. **Reproduzierbarkeit.** Zwei Materialisierungen des gleichen Ortes zur gleichen Zeit mit gleichem Weltzustand liefern denselben Menschen (Seed aus Ort, Zeitfenster, Weltseed; vgl. `35#278A` Frage 15, „1-4 ja“ und „12-35 ja“ in `35#279E`).
7. **Einwanderer.** Offen (N-ID-023, ❓ Q-N11).

---

## 11. Sonderfälle

1. **Namensgleiche NPCs.** Namen sind nicht eindeutig; nur IDs sind es. Dialoge und Archiv zeigen bei Doppeldeutigkeit Zusatzmerkmale (Beruf, Straße).
2. **Doppelgänger und Formwandler.** Ein Formwandler hat **seine eigene ID** und eine oder mehrere *Erscheinungen*, die auf die ID eines anderen NPCs verweisen ([Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) §6). Die Welt muss zwischen ‚wer ist es wirklich‘ und ‚wer scheint es zu sein‘ unterscheiden (vier Ebenen der Wahrheit, Ermittlung 13 §3).
3. **Tarnidentität, Zeugenschutz.** Ein Mensch kann unter neuem Namen weiterleben; die ID bleibt, der Name und das öffentliche Wissen ändern sich (Ermittlung 12 §11, 13 §10).
4. **Besessenheit.** Während der Besessenheit gelten die Gedächtnisgrenzen der Art ([Kreaturen 05](../Kreaturen/05-Geister-Besessenheit-und-Fluecher.md)); der Datensatz markiert Lücken, nicht Löschung.
5. **Kinder.** Familien können Kinder haben (`23#1056A` Frage 5; „ja“). Kinder sind NPCs mit Haushalt und Schule/Tagesablauf; **sie sind vollständig vom Adult-System ausgeschlossen** (Altersregel in [Adult 01](../Adult/01-Grundregeln-Alter-Zustimmung.md)). Darstellung von Gewalt gegen Minderjährige ist eine offene Rechts- und Politikfrage (`M-OD`: ‚Exact minor-NPC violence presentation after ESRB/PEGI/USK/Sony/Microsoft/Steam/legal review‘).
6. **Obdachlose.** Haben keinen Wohnsitz, aber eine ID, bekannte Orte und Beziehungen (N-ID-004).
7. **NPC wird zum Begleiter.** Die ID bleibt, die Persistenz steigt auf A, Ausrüstung und Fahrzeug werden persistent (Modul 19).
8. **Tod ohne Zeugen.** Der Weltzustand ändert sich; das Wissen darüber verbreitet sich nur über Beobachtung und Kommunikation (Modul 07). Ein Toter *kann* lange unentdeckt sein.
9. **Mehrere Menschen mit demselben Beruf an einem Ort.** Eigene Zeitpläne; kein ‚Geschäftsinhaber ist immer da‘.
10. **Verlust des Datensatzes durch Mod-Entfernung.** Unbekannte oder deaktivierte Mod-NPCs bleiben im Save erhalten, aber inaktiv ([Modul 23](23-Koop-Host-Speicherstand-und-Mods.md)).

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-001 | Eine NPC-ID wird nie wiederverwendet und ändert sich nie | N-ID-008 |
| N-EN-002 | Gleiche ID und gleicher Weltseed liefern denselben abgeleiteten Zustand | N-ID-009, N-ID-020 |
| N-EN-003 | Ein eingefrorener NPC wird durch ein Generator-Update nicht verändert | N-ID-013 |
| N-EN-004 | Ein Aufstieg der Datensatztiefe erfolgt nur durch ein Ereignis, nie durch Zeitablauf | Abschnitt 5 |
| N-EN-005 | Ein Abstieg verdichtet Detailzustand, aber verliert weder Identität noch Beziehungsgeschichte noch wichtige Einzelereignisse | N-ID-015 |
| N-EN-006 | Ein toter NPC nimmt keine Handlung vor und erscheint nicht als lebende Figur | N-LT-017 |
| N-EN-007 | Ein D-Mensch ohne Gespräch, Ereignis oder Beobachtung hat keine ID im Weltzustand | Abschnitt 10 |
| N-EN-008 | Eine Materialisierung am selben Ort zur selben Zeit mit demselben Weltzustand liefert dieselbe Person | Abschnitt 10 |
| N-EN-009 | Ein `sim_safe`-NPC stirbt nie durch reine Simulationsartefakte | N-LT-033 |
| N-EN-010 | Ein Wechsel der Persistenzstufe löscht nie die Beziehungen zum Spieler | N-ID-015 |
| N-EN-011 | Ein Umzug ändert Wohnort, Arbeit und Routine, nicht ID, Seed oder Historie | N-ID-019 |
| N-EN-012 | Der Datensatz enthält keine Zahlen für den Spieler; Sichtbarkeit nur über Verhalten und Archiv | N-LT-012 |

---

## 13. Was fehlt

- Kein Schema, keine Feldgrößen, keine Speichergröße je Datensatz (siehe [Modul 21](21-Weltpersistenz-und-Speicherentwurf.md) für Rechenmodelle als Entwurf).
- Keine Zahlen für A/B/C/D-Anteile, keine Zeitangaben für ‚Jahrzehnte Irrelevanz‘.
- Keine Entscheidung zu Schutzstufen im Einzelfall und zu der Frage, wer wichtige NPCs töten darf (C-N1).
- Offen: Einwanderer (N-ID-023), Datensatz der Toten (N-ID-024).
