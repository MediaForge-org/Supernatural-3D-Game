# Kreaturen 07: Kosmologie, Seelen und Reiche

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die kosmologischen Regeln, soweit sie **Kreaturen, Seelen, Geister und Tod** betreffen. Die Reiche selbst (Größe, Orte, Zeitreise, historische Weltzustände) sind in der [Welt-Dokumentation](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md) geführt und werden hier **nicht wiederholt**. Die Kosmologie ist im Master ausdrücklich offen (‚exact realm cosmology/time ratios‘, `M-B09`). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Körper, Bewusstsein und Seele](#2-körper-bewusstsein-und-seele)
3. [Tod und Ziel der Seele](#3-tod-und-ziel-der-seele)
4. [Rückkehr und Wiederbelebung](#4-rückkehr-und-wiederbelebung)
5. [Reisen zwischen den Ebenen](#5-reisen-zwischen-den-ebenen)
6. [Seele ohne Körper, Körper ohne Seele](#6-seele-ohne-körper-körper-ohne-seele)
7. [Die Reiche aus Sicht der Wesen](#7-die-reiche-aus-sicht-der-wesen)
8. [Entwurf: Zustandsmodell](#8-entwurf-zustandsmodell)
9. [Entwurf: Prüfregeln](#9-entwurf-prüfregeln)
10. [Sonderfälle](#10-sonderfälle)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Übernatürliche Ebenen existieren tatsächlich als **eigene Welträume**, nicht als Farbfilter; sie sind echte Open-World-Regionen, aber kleiner als die Hauptkarte; im Basisspiel nur die wichtigsten | `23#359A` Punkt 3; „3. ja das muss sein, aber die maps werden nicht so groß sein“ in `23#360E`; „A: das sollen wirkliche openworld regionen werden, im hauptspiel sind nur die wichtigsten und dann in dlcs kommen dann andere“ in `23#360E` | 📋 |
| Mindestens **Himmel, Hölle und Fegefeuer** im Hauptspiel | „aber im hauptspiel soll es noch den himmel geben“ in `23#1075E`; `23#1103E`; `M-CA0905` §5 | 📋 |
| Ebenen können **eigene Regeln** haben (Zeit, Licht, Temperatur, Raum, Navigation, Tod, Regeneration, Wahrnehmung, Kommunikation, Elektronik, Fahrzeuge, Waffen, Kräfte), intern konsistent, nicht ‚alles ist möglich‘ | `23#359A` Punkt 4; Antwort: „4. ja, das soll sogar so sein, in der hölle vergeht die zeit viel schneller als auf der erde. 40 jahre in der hölle sind 4 monate auf der erde“ in `23#360E` | 📋 |
| Übergänge brauchen Regeln, kein ‚Fast Travel → Hell‘; erster Zugang kann storygebunden sein, danach Wissen und Ritual | `23#359A` Punkt 5; „ja so soll es sein, oder man kann nur in der story hin“ in `23#360E`; „man kann aber erst dorthin reisen wenn man mal dort war und etwas herausgefunden hat wie man hinreist“ in `23#360E` | 📋 |
| Nicht jeder Hunter kennt die wahre Kosmologie; das Archiv kann konkurrierende Modelle führen; ein Rest Mysterium bleibt | `23#359A` Punkt 6, „6. ja“ in `23#360E` | 📋 |
| Der Tod hat einen **echten übernatürlichen Ablauf** (Seele trennt sich, Übergang, Ziel); normale Gameplay-Tode zählen **nicht** als kanonische Story-Tode | `23#458A` Punkt 1 (Assistent); `23#459E` Punkt 1: „für npcs passt es“; `23#460A` | 📋 |
| Nicht jeder Tote wird zum Geist | `23#458A` Punkt 2, „2. ja“ in `23#459E` | 📋 |
| Himmel und Hölle sind **reale Orte** | `23#458A` Punkt 3, „3. ja genau“ in `23#459E` | 📋 |
| Tote Seelen altern nicht körperlich | „7. ja die person ist tot gewesen also kann sie nicht altern“ in `23#459E` | 📋 |
| Jeder Mensch hat im Himmel seinen eigenen Himmel („in der serie hat jeder mensch seine eigenen himmel“) | „17. ja, in der serie hat jeder mensch seine eigenen himmel“ in `23#459E` | 📋 |
| Wiederbelebung ist möglich, aber **außergewöhnlich** | „15. genau“ in `23#459E` zu `23#458A` Punkt 15 | 📋 |
| Hin- und Rückweg können unterschiedliche Regeln haben | „12. ja wäre möglich“ in `23#459E` | 📋 |
| Hölle ist eine echte Open World mit Collectibles und anderem Inhalt; der Begleiter kann mitkommen oder zurückbleiben | „A: ja schon, es ist openworld und hat auch collectibles und anderes“, „B: ja genau“ in `23#459E` | 📋 |
| Tod in anderen Ebenen: Checkpoint „ok, vielleicht aber noch anders“ | „26. ok, vielleicht aber noch anders“ in `23#459E` | ❓ |
| Die Reisen können Zeit auf der Erde kosten: „ja aber halt viel weniger“ | „22. ja aber halt viel weniger“ in `23#459E` | 📋 / ❓ |
| Das Hunter-Archiv dokumentiert Ebenen wie Forschungsgebiete | „28. ja perfekt“ in `23#459E` | 📋 |

**Hinweis zu Zeile 3:** Die Antwort `23#360E` Punkt 4 gehört zu `23#359A` Punkt 4 (‚Ebenen können unterschiedliche physikalische Regeln haben‘) und enthält die Zeitrelation der Hölle. Die Richtungsfrage (schneller oder langsamer als die Erde) ist durch `PE-E1022` entschieden, siehe [Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md).

---

## 2. Körper, Bewusstsein und Seele

`23#361A` Punkt 1 (💡 Ausformulierung, 📋 Grundlinie durch Gesamtzustimmung in `23#362E`):

```text
Person
├── Physical Body
├── Conscious Mind
└── Soul
```

Im lebenden Menschen wirken die drei zusammen; übernatürliche Ereignisse können sie teilweise trennen. Beispielzustände der Quelle:

| Zustand | Bedeutung |
|---|---|
| Körper lebt, Seele vorhanden | normal |
| Körper lebt, Seele teilweise getrennt | außergewöhnlich |
| Körper lebt, fremde Entität kontrolliert | Besessenheit ([Modul 05](05-Geister-Besessenheit-und-Fluecher.md)) |
| Körper stirbt, Seele verbleibt | möglicher Geist oder Übergang |
| Körper stirbt, Seele verlässt die Welt | tatsächlicher Tod |

Die Seele ist **kein Trefferpunktwert** (‚Soul HP‘ wird abgelehnt; `23#361A` Punkt 9, 💡). Übernatürliche Erlebnisse können sie verändern (Erinnerungsfragmente, Halluzinationen, Schlafprobleme, emotionale Veränderungen, übernatürliche Wahrnehmung, Dissoziation); psychische und übernatürliche Wirkungen dürfen interagieren, aber nicht gleichgesetzt werden.

**Der Entwickler sagte dazu:** Die Tod-/Seele-Mechanik soll als Mittelweg bleiben („wir müssen einen mittelweg finden vielleicht“, `23#362E`) und nicht überkomplex werden; Details der 22 nummerierten Antworten in `23#362E` sind keiner Frage der Vorgängernachricht eindeutig zuzuordnen (Modul 21, U-K3).

---

## 3. Tod und Ziel der Seele

### 3.1 Gameplay-Tod und Story-Tod

| Art | Behandlung | Quelle |
|---|---|---|
| Gameplay-Tod des Hauptcharakters | nicht kanonisch; Checkpoint; keine Jenseitsreise; nur ein minimal überlebensfähiger Gesundheitszustand | `23#460A` Punkt 1; `23#365A`; `23#366E` |
| Story-Tod des Hauptcharakters | kanonisch; Jenseitsregeln gelten | `23#460A` Punkt 1 (💡 Formulierung, 📋 über `23#459E`) |
| Tod eines NPCs | läuft durch das Jenseitssystem (geht weiter, Himmel, Hölle, Geist, Sonderfall) | `23#460A` Punkt 1 |
| Schwierigkeitsgrad ändert die kosmologischen Regeln nicht | ‚Grounded macht Schaden schwerer, aber die Seele verschwindet nicht schneller‘ | `23#361A` Punkt 25 (💡) |
| Permadeath ist optional, nie Standard | `23#361A` Punkt 26 (Assistent); `M-PD` ‚Death / failure — OPEN‘ (‚Permadeath only optional if included‘) | 💡 / ❓ |

### 3.2 Ziel der Seele

Der Assistent schlägt einen Mittelweg vor (`23#460A`, 💡; der Entwickler erinnerte in `23#459E` Frage C an Serienbeispiele, in denen Verträge mit Dämonen die Seele in die Hölle brachten):

```text
Natürlicher Tod  →  Leben + Handlungen der Person  →  natürliches Ziel (Himmel oder Hölle)
Sonderregeln     →  Dämonenvertrag, Fluch, kosmologischer Eingriff, Seelenmanipulation,
                    besondere Storyregel  →  kann das natürliche Ziel überschreiben
Seele weigert sich / bleibt gebunden  →  Geist
```

Dazu: **kein Karma-Balken** und keine sichtbare Wahrscheinlichkeit; bei NPCs entscheidet das System aus Biografie und Ereignissen, beim Hauptcharakter nur bei kanonischem Tod (`23#460A`, 💡). Die Frage C ist nur teilweise beantwortet (❓ Q-K08): ob dieses Zuordnungsmodell gilt, hat der Entwickler nicht ausdrücklich bestätigt; er bestätigte, dass Verträge die Seele in die Hölle bringen können (`23#459E`, Antwort C).

---

## 4. Rückkehr und Wiederbelebung

| Aussage | Quelle | Status |
|---|---|:---:|
| Wiederbelebung existiert, ist aber kein Verbrauchsgegenstand | „15. genau“ in `23#459E`; `23#361A` Punkt 6 | 📋 / 💡 |
| Je länger tot, desto schwieriger; nach einer kosmologischen Grenze unmöglich; keine festen Zeiten | `23#361A` Punkt 7, `23#458A` Punkt 16, „16. ja“ in `23#459E` | 📋 Richtung / 💡 Inhalt |
| Rückkehr kann Konsequenzen haben (Erinnerungslücken, körperliche Schäden, psychologische Folgen, Seelenverletzung, übernatürliche Aufmerksamkeit, Bindungen) | `23#361A` Punkt 8 | 💡 |
| Klinischer Tod ist nicht automatisch endgültig; kurzer Zeitraum für medizinische Reanimation | `23#361A` Punkt 3 | 💡 |
| Seltene Near-Death-Erlebnisse mit übernatürlichem Inhalt, nicht bei jedem Herzstillstand dieselbe Szene | `23#361A` Punkt 4 | 💡 |
| Wer lebend per Portal reist, ist etwas anderes als jemand, der stirbt und im Jenseits ankommt; die Regeln für Tote sind strenger; ‚fahr halt kurz hin und hol ihn‘ ist ausgeschlossen | `23#458A` Punkt 14, „14. genau“ in `23#459E`; `23#361A` Punkt 32 | 📋 |

Das Zeitverhältnis (4 Monate Erde gegen 40 Jahre Hölle) ist eine kanonische kosmologische Regel, die nicht je Mission variiert; die Antwort auf die Frage, ob sie konstant bleibt, lautet „A: ja so in etwa, so ist es in der serie“ (`23#362E`, zu `23#361A` Frage A).

---

## 5. Reisen zwischen den Ebenen

| Aussage | Quelle | Status |
|---|---|:---:|
| Der erste Besuch kann storygebunden sein (unfreiwillig, Überleben, Erkenntnis, Rückkehr) | `23#458A` Punkt 8, „8. ja“ in `23#459E` | 📋 |
| Freies Reisen wird erst später freigeschaltet: Ritual, Ort, Materialien, Bedingungen, Artefakt, Risiken müssen herausgefunden werden | `23#458A` Punkt 9, „9. genau“ in `23#459E` | 📋 |
| Die Zutaten sind selten; das Finden ist Ermittlung, **kein Grinding** | `23#458A` Punkte 10–11, „10. ja das werden wir noch aufstellen“, „11. genau“ in `23#459E`; „13 ja das soll im ganzen spiel nicht so sein“ in `23#466E` | 📋 |
| Die Hinreise kann das Portal schließen; die Rückreise braucht ein zweites Ritual, einen Ausgang, jemanden, der zurückbleibt, oder einen Gegenstand | `23#458A` Punkt 12 (💡 Inhalt), „12. ja wäre möglich“ in `23#459E` | 📋 / 💡 |
| Manchmal ist die Rückkehr ungewiss, aber nicht unfair (Hinweise, nachvollziehbares Risiko) | `23#458A` Punkt 13, „13. ok“ in `23#459E` | 📋 / 💡 |
| Ebenen sind nicht sofort komplett offen: erst ein kleiner Bereich, später mehr durch Wissen und Portale | `23#458A` Punkt 23, „23. gut“ in `23#459E` | 📋 |
| Begleiter kann mitkommen oder zurückbleiben | „B: ja genau“ in `23#459E` | 📋 |
| Online: Party instanziert; andere Spieler des Servers werden nicht teleportiert | `23#458A` Punkt 25 (💡), „25. ja passt“ in `23#459E` | 📋 / 💡 |
| Das Fegefeuer ist schwer zu verlassen; der erste Aufenthalt vermittelt: ‚Ich weiß nicht, wie ich hier rauskomme‘ | `23#465A` Punkt 11 (💡), „11 super“ in `23#466E` | 📋 / 💡 |
| Tod in der Hölle bleibt gewöhnlich gefährlich, nicht ‚du bist schon in der Hölle, also passiert nichts‘ | `23#458A` Punkt 26; „26. ok, vielleicht aber noch anders“ in `23#459E` | ❓ |
| Manche Dinge funktionieren dort anders (Wesen reagieren anders, bestimmte Rituale gehen nicht, Elektronik schlecht), aber nur mit konkreter Lore-Begründung | `23#458A` Punkt 27, „27. gut“ in `23#459E` | 📋 / 💡 |

---

## 6. Seele ohne Körper, Körper ohne Seele

Der Assistent fragte, ob eine Seele grundsätzlich aus einem noch lebenden Körper entfernt werden könne (`23#361A`, Frage B); der Entwickler antwortete „B: ja so wie bei sam in s6“ (`23#362E`). Das ist eine Entscheidung (📋): **Ja, grundsätzlich möglich**, mit Verweis auf die Serie als Standard. Was das für Körper und Persönlichkeit bedeutet, sagt keine Quelle:

- Der Assistent skizziert mögliche Folgen (`23#361A` Punkte 21–22, 💡): leerer Körper, veränderte Persönlichkeit, kein normales Bewusstsein, leichtere Übernahme durch Fremde; **kein Zombie-Modus**; die genaue Folge sei eine große Loreentscheidung.
- Der Körper darf nicht willkürlich altern, wenn nur die Seele in der Hölle war (`23#458A` Punkt 7, „7. ja die person ist tot gewesen…“ in `23#459E`).

Beides ist ❓ (Q-K09): Es fehlt jede Regel dafür, **wie** Körper und Seele in einzelnen Storyfällen getrennt werden und was mit dem Körper währenddessen geschieht (tot, konserviert, anderweitig existent, besessen, `23#361A` Punkt 11).

---

## 7. Die Reiche aus Sicht der Wesen

Die Reichs-Dokumentation ([Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md)) führt Größe und Orte. Aus der Kreaturenperspektive sind folgende Aussagen relevant:

| Reich | Kreaturenbezogene Aussagen | Quelle |
|---|---|---|
| Fegefeuer | Wildnis-, Überlebens- und **Monsterökologie**; stärkster Kandidat für Territorien, Raubtiere, Wanderung, Nahrungsketten; Welt der Monsterseelen; Leviathan-Gefängnis | `M-CA0905` §5; `M-RS`; `23#465A` (💡) |
| Hölle | vertikal, labyrinthisch, Machtstrukturen, Gefängnisse, Ruinen; Gesellschaft mit Hierarchien, Deals, Gefangenen; Zeitdehnung | `M-CA0905` §5; `23#458A` Punkte 18–19 |
| Himmel | gemeinsame Welt plus persönliche Erinnerungs- und Taschendomänen; kein Wolkenklischee; Engel mit Hierarchie und eigenen Zielen | `M-CA0905` §5; `23#458A` Punkte 17, 21 |
| Alle Reiche | eigene Ökologie statt Kampfarenen: „136 ja sie sind ganz anders. wir machen es so wie in der serie würde ich sagen“; eigene Orte, Regionen und Strukturen; **persistenter Weltzustand** („139 ja das ist fix im spiel“) | `23#1074A` Punkte 136–139; `23#1075E` |
| Weitere Ebenen | im Basisspiel nur wenige, weitere als DLC/Expansion | „140 ja habe ich gesagt“ in `23#1075E`; `23#458A` Punkt 24, „24. ja das ist fix“ in `23#459E` |

Alle Reiche brauchen einzeln eine **Weltbibel, räumliche Zuordnung, Bewegungsregeln, Ökologie/Gesellschaft, Persistenz und ein Karten-Master** (`M-CA0905` §5, `M-PS0905`); sie sind offen (`M-B09`, ‚exact realm cosmology/time ratios; final realm map sizes‘).

---

## 8. Entwurf: Zustandsmodell

> 💡 **Vorschlag.** Er hält die beschlossenen Unterscheidungen auseinander; er legt keine Kosmologie fest.

```text
PersonCosmicState
├── body            alive | clinically_dead | dead | preserved | unknown
├── mind_presence   present | suppressed | displaced | shared | absent
├── soul            present | partial | removed | gone_on | bound | in_realm(realm_id)
├── death_record    {canonical: bool, cause, time, destination | ghost | unknown}
├── realm_time      Zuordnung subjektiver Zeit je Reich (aus Welt-Dokumentation)
└── history[]       Ereignisse (Besessenheit, Reise, Wiederbelebung)

TravelAttempt
├── party, mode: story | ritual | portal | death
├── requirements[]  gefunden/nicht gefunden (Wissen, Ort, Zutat, Zeit)
├── return_plan     known | uncertain | none
└── outcome         arrived | failed | stranded | returned
```

Diese Zustände sind Weltzustand und müssen speichern und laden überstehen ([Modul 17](17-Persistenz-Daten-und-Modding.md)).

---

## 9. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Ein Gameplay-Tod des Hauptcharakters löst keine Jenseitsreise aus | `23#460A` Punkt 1 |
| Ein NPC-Tod erzeugt einen Eintrag im Jenseitssystem (Ziel oder Geist), nicht nur ‚alive = false‘ | `23#460A` |
| Ein lebender Reisender kann nicht ohne Methode einen Toten abholen | `23#458A` Punkt 14 |
| Freies Reisen wird erst nach Entdeckung der Methode verfügbar (Zustand ‚Methode bekannt‘ und Zutaten vorhanden) | `23#458A` Punkt 9 |
| Nach Rückkehr aus einer Ebene wird das Zeitverhältnis auf Weltzustand und NPC-Alter angewandt | `W-RE-005`, `W-RE-011`; `23#361A` Punkt 10 |
| Keine UI zeigt ein Karma, ‚Soul HP‘ oder eine Himmelswahrscheinlichkeit | `23#460A`; `23#361A` Punkt 9 |
| Der Online-Partyjob verschiebt keine fremden Spieler in eine Ebene | `23#458A` Punkt 25 |

---

## 10. Sonderfälle

| Fall | Behandlung |
|---|---|
| Ein Spieler stirbt in einer anderen Ebene | ❓ („vielleicht aber noch anders“, `23#459E` Punkt 26) |
| Ein Begleiter bleibt auf der Erde, während die Zeit läuft | Zeitverhältnis; offene Fälle laufen weiter (`23#458A` Punkt 22) |
| Ein Toter wird von einem mächtigen Wesen gerettet | Wiederbelebung als außergewöhnlicher Vorgang; Konsequenzen (`23#361A` Punkt 8) |
| Eine Seele wird im Fegefeuer zum zweiten Mal getötet | endgültig; metaphysische Folge unbekannt ([Modul 06](06-Daemonen-Engel-und-Leviathane.md)) |
| Ein Hunter reist freiwillig in die Hölle und trifft dort ein Individuum, das er auf der Erde nicht kannte | Ereignis im Weltzustand der Ebene; Persistenz („139 ja das ist fix im spiel“) |

---

## 11. Was fehlt

- Die vollständige Kosmologie (Reichsbibeln), Kriterien für das natürliche Ziel einer Seele (❓ Q-K08).
- Der Mechanismus der Trennung von Körper und Seele und der Zustand des Körpers (❓ Q-K09).
- Gameplay-Tod in anderen Ebenen (❓ `23#459E` Punkt 26).
- Das Zeitverhältnis des Fegefeuers ([Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md), ❓).
- Eigene Bezeichnungen für Seelenbegleiter, Jenseits und Reiche (❓ IP).
