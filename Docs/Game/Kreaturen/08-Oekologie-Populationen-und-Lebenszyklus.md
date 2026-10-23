# Kreaturen 08: Ökologie, Populationen und Lebenszyklus

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Kreaturen **in der Welt leben**: Lebensraum, Territorium, Nester, Wanderung, Nahrung, Entstehung, Lebensphasen und Aktivitätsbedingungen. Es gibt keine Zahlen und keine Artenliste. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Keine Spawnpunkte](#2-keine-spawnpunkte)
3. [Individuen und Populationen](#3-individuen-und-populationen)
4. [Lebensraum, Territorium, Nester, Wanderung](#4-lebensraum-territorium-nester-wanderung)
5. [Nahrung und Opfer](#5-nahrung-und-opfer)
6. [Entstehung und Fortpflanzung](#6-entstehung-und-fortpflanzung)
7. [Lebensphasen und Jungwesen](#7-lebensphasen-und-jungwesen)
8. [Aktivitätsbedingungen](#8-aktivitätsbedingungen)
9. [Seltenheit und Glaubwürdigkeit](#9-seltenheit-und-glaubwürdigkeit)
10. [Beziehungen zwischen Arten](#10-beziehungen-zwischen-arten)
11. [Entwurf: Simulationsmodell](#11-entwurf-simulationsmodell)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Sonderfälle](#13-sonderfälle)
14. [Was fehlt](#14-was-fehlt)

---

## 1. Was beschlossen ist

Die folgenden Aussagen stammen aus Block 9D–G (`23#1074A` Punkte 30–44 und 62–68) mit den Antworten des Entwicklers in `23#1075E` und aus `23#307A`/`23#308E`; der Master fasst sie in `M-B09` und `M-PD` zusammen.

| Aussage | Quelle | Status |
|---|---|:---:|
| Monster sind **nicht** einfach Spawnpunkte (kein ‚alle 20 Minuten neuer Vampir‘); stirbt ein Individuum, ist es weg | „30. wenn das jeweilige monster tot ist dann ist es weg“ in `23#1075E`; `M-B09` | 📋 |
| Individuen mit persistenter Identität; Populationen bei häufigeren Arten | „31. ja kann sein, sowie bei leviathanen“, „32. ja“ in `23#1075E` | 📋 |
| Manche Wesen haben ein reales Jagd- und Lebensgebiet | „33. ja“ in `23#1075E` | 📋 |
| Manche Arten bilden Nester, Rudel, Familien oder Kolonien | „34. ja“ in `23#1075E` | 📋 |
| Wanderung (Nahrungsmangel, Hunter-Druck, Feuer, Überschwemmung, Rivalen, übernatürliche Ereignisse) | „35. ja“ in `23#1075E` | 📋 |
| Einige Arten können sich vermehren bzw. neue Mitglieder erzeugen | „36. genau“ in `23#1075E` | 📋 |
| Keine ewigen Respawns: ein vollständig vernichtetes Nest bleibt für lange Zeit weg; „die gleichen monster werden nie wieder da sein“ | „37. die gleichen monster werden nie wieder da sein, außer sie wurden aus dem fegefeuer befreit…“ in `23#1075E` | 📋 |
| Wiederbesiedlung Jahre später durch andere Individuen, eine andere Gruppe oder eine andere Art | „38. ja das schon“ in `23#1075E` | 📋 |
| Die Population hängt teilweise von Nahrung und Opfern ab | „39. ja“ in `23#1075E` | 📋 |
| Verschiedene Arten bevorzugen verschiedene Lebensräume (Stadt, Suburbs, Kanalisation, Industrie, Wald, Berge, Höhlen, Seen, Wetlands, Friedhöfe, alte Häuser) | „40. ja“ in `23#1075E` | 📋 |
| Tageszeit: manche Arten nachtaktiv, andere nicht | „41. genau“ in `23#1075E` | 📋 |
| Wetter und Jahreszeit (Winter, Vollmond, Sturm, Hitze, Nebel, bestimmte Jahreszeiten) | „42. korrekt“ in `23#1075E` | 📋 |
| Monster kämpfen untereinander; Arten haben Konkurrenz oder Feindschaft | „43. ja sicher“ in `23#1075E` | 📋 |
| Monster meiden andere Monster; Territorialverhalten liefert indirekt Hinweise | „44. ja“ in `23#1075E` | 📋 |
| Soziale Strukturen: einzeln, Paar, Familie, Rudel, Kolonie, Hierarchie, kultähnliche Gemeinschaft, Herr/Diener, Schwarm | `23#307A` Punkt 16, „16. cool“ in `23#308E` | 📋 |
| Artspezifische Entstehung: Geburt, Übertragung durch Verletzung, Fluch, Ritual, Besitz, künstliche Erschaffung, Verwandlung, Vererbung, Entstehung aus bestimmten Todesumständen | `23#307A` Punkt 18, „18. genau“ in `23#308E` | 📋 |
| Manche Arten haben Lebensphasen (neu erschaffen, jung, reif, alt, uralt), nicht zwingend jede | `23#307A` Punkt 19, „19. passt“ in `23#308E` | 📋 |
| Aktivitätsbedingungen: Tag/Nacht, Zeitbereich, Jahreszeit, Wetter, Temperatur, Mondphase, Ortsbedingungen, Sonderzustand | `23#307A` Punkt 20, „20. gut“ in `23#308E` | 📋 |
| Ein Fall entsteht, wenn der Weltzustand die Voraussetzungen erfüllt, nicht als Zufallsspawn | `23#307A` Punkt 21; Antwort in `23#308E` (siehe [Modul 15](15-Vorfaelle-und-lebendige-Welt.md)) | 📋 |
| Monsterterritorien entwickeln sich bis zu einer **umfassenden Territoriumssimulation** mit Ressourcen, Bevölkerungsentwicklung, strategischen Entscheidungen und langfristigen regionalen Veränderungen | `48#142A` (Frage), „Supernatural dynamische Monsterterritorien: Umfassende Territoriumssimulation mit Ressourcen und regionaler Entwicklung“ in `48#143E` | 📋 |
| Entwicklungen bleiben im jeweiligen **Host-Spielstand** gespeichert und finden auch ohne Beteiligung des Spielers statt | `48#142A`, `48#144A` (Auslegung der gewählten Option) | 📋 / 💡 |

---

## 2. Keine Spawnpunkte

### 2.1 Regel

`23#1074A` Punkt 30: ‚Nicht `MonsterSpawner_03 → alle 20 Minuten neuer Vampir`.‘ Der Entwickler antwortete mit der Konsequenz: tot ist tot (`23#1075E` Punkt 30). Der Master führt es als ‚individual death permanence‘ (`M-B09`).

### 2.2 Wiederbesiedlung

Eine Region kann später von anderen Individuen derselben Art, einer anderen Gruppe oder einer anderen Art besiedelt werden (`23#1076A`, 💡 Ausformulierung der 📋 Punkte 37–38; Formulierungen „für lange Zeit“, „Jahre später“ aus `23#1074A`). Ein vollständig vernichtetes Nest bleibt **wirklich vernichtet** (`23#1076A`). Der Zeitrahmen („für lange Zeit“, „Jahre später“) ist nicht festgelegt (❓ Q-K10).

### 2.3 Die einzige genannte Ausnahme

Ein Individuum kann nur dann „wieder da sein“, wenn es aus dem Fegefeuer befreit wurde; das sei „sehr unwahrscheinlich“ (`23#1075E` Punkt 37; zum Fegefeuer-Mechanismus siehe [Modul 06](06-Daemonen-Engel-und-Leviathane.md) §5).

---

## 3. Individuen und Populationen

| Ebene | Inhalt | Quelle |
|---|---|---|
| Individuum | stärkere oder intelligentere Wesen sind vollständig persistent (Name/Identität, Alter, Gebiet, Beziehungen, Opfer, Verletzungen, Feinde, Vergangenheit) | `23#1076A`; „31“ in `23#1075E` |
| Population | häufigere Arten werden zusätzlich populationsbasiert simuliert | `23#1076A`; „32“ |
| Reaktion der Populationen auf die Welt | Nahrung/Opfer, Jahreszeit, Wetter, Konkurrenz, Hunter-Druck, Brände, Überschwemmungen, menschliche Expansion, andere Monster | `23#1076A` (💡 Zusammenfassung der 📋 Punkte 33–44) |

Die Grenze zwischen Individuum und Population ist nicht festgelegt (❓ Q-K02, [Modul 03](03-Klassifikation-Herkunft-und-Individuen.md) §8).

---

## 4. Lebensraum, Territorium, Nester, Wanderung

### 4.1 Lebensräume

`23#293A` Punkt 18: Monster spawnen nicht zufällig irgendwo; je nach Art Wald, verlassene Gebäude, Kanalisation, Stadt, Friedhof, Straßen, Gewässer, bestimmte Orte. Manche haben Revier, Bau, Jagdroute, bevorzugte Opfer, Ruhezeiten; ein guter Jäger kann daraus Vorhersagen treffen (Bestätigung „18. ja genau“ in `23#294E`). Die Bandbreite der Lebensräume benennt `23#1074A` Punkt 40; „40. ja“.

Unterwasser, Seen und Höhlen: eigene Arten, versunkene Artefakte, Höhlen (`23#1074A` Punkt 132, „132 ja“ in `23#1075E`); tiefe Wälder und Berge: eigene Arten und alte Legenden (Punkt 131, „131 ja genau“); Städte mit Urban Legends, Gebäuden, Tunneln, Kanalisation, Clubs, Krankenhäusern und eigenen Typen (Punkt 133, „133 sicher“).

### 4.2 Nester und Gruppen

Nester, Rudel, Familien und Kolonien (`23#1074A` Punkt 34). Ein Nest, das vollständig vernichtet wurde, bleibt zerstört (siehe oben). Gruppen sind nicht moralisch einheitlich (`23#307A` Punkt 17, „17. genau“ in `23#308E`; [Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md)).

### 4.3 Wanderung

`23#1074A` Punkt 35: Wesen verlassen ihr Gebiet bei Nahrungsknappheit, Hunter-Druck, Feuer/Überschwemmung, rivalisierenden Wesen oder übernatürlichen Ereignissen. Konsequenzen für Fälle: Ein nicht erledigtes Monster wechselt das Jagdgebiet, neue Opfer, NPC- und Weltzustand ändern sich; die **Spur wandert** (`23#293A` Punkt 20, `23#1074A` Punkt 65; Bestätigungen „20. genau“ in `23#294E`, „62-68 ja“ in `23#1075E`). Wird ein Wesen nur vertrieben, kann Monate später ein anderer Fall entstehen.

### 4.4 Territoriumssimulation (Quiz-Entscheidung)

`48#142A` stellte die Frage, wie sich Monsterterritorien entwickeln sollen (A feste Gebiete, B ereignisgesteuert, C dynamische Expansion, D umfassende Simulation mit Ressourcen, Bevölkerungsentwicklung, strategischen Entscheidungen und regionalen Veränderungen). Der Entwickler wählte **D** (`48#143E`). Der Assistent deutet das so (`48#144A`, 💡 Auslegung):

| Bereich | Mechanik (Auslegung) |
|---|---|
| Territorien | Eroberung, Verteidigung, Expansion, Gebietsverluste |
| Ressourcen | Nahrung, Verstecke, sichere Rückzugsorte, artspezifische Bedürfnisse |
| Populationen | Wachstum, Verluste, Wanderungen |
| Strategische KI | Monsterorganisationen entscheiden über Expansion, Rückzug, Konflikte |
| Machtverhältnisse | Allianzen, Rivalitäten, Führungswechsel |
| Regionale Folgen | Angriffe, Vermisstenfälle, NPC-Verhalten, Sicherheitslage |
| Persistenz | Entwicklungen im Host-Spielstand |

Beispiel (💡): Zwei Werwolfrudel in Waldgebiet und Bergregion mit einem Grenzkonflikt, der auch **ohne** den Spieler weiterläuft. Die Umsetzung braucht eine **abgestufte Simulation**, damit große Karten performant bleiben (`48#146A`, Folgerung des Assistenten zur Gesellschaftssimulation, 💡).

---

## 5. Nahrung und Opfer

Die Population hängt teilweise davon ab, ob genug geeignete Opfer oder Nahrung existieren („39. ja“, `23#1075E`). Daraus entstehen Vorfälle: Monsterpopulation → Opfer → Vermisstenmeldung → Polizei → Zeitung/Social Media → Hunter-Hinweis (`23#1074A` Punkt 62, „62-68 ja“; [Modul 15](15-Vorfaelle-und-lebendige-Welt.md)). Die Ernährung einzelner Arten (zum Beispiel Urwesen) ist artspezifisch und nicht über einen ‚Hungermeter‘ zu modellieren (`23#467A` Punkt 13, 💡).

---

## 6. Entstehung und Fortpflanzung

| Aussage | Quelle | Status |
|---|---|:---:|
| Jede Art hat eigene Entstehungsregeln: geboren, übertragen durch Verletzung, Fluch, Ritual, Besitz, künstlich erschaffen, Transformation, Vererbung, aus Todesumständen | `23#307A` Punkt 18, „18. genau“ in `23#308E` | 📋 |
| Entstehung ist **Lore und Gameplay**: Wer den Mechanismus versteht, kann vielleicht verhindern, dass neue Wesen entstehen | `23#307A` Punkt 18 | 📋 / 💡 |
| Einige Arten können neue Mitglieder erzeugen | „36. genau“ in `23#1075E` | 📋 |
| Fortpflanzung ist **nicht** universell | `M-PD` | 📋 |

Die Frage, welche Art sich wie vermehrt, ist nicht beantwortet (❓ Q-K11).

---

## 7. Lebensphasen und Jungwesen

### 7.1 Lebensphasen

`23#307A` Punkt 19: neu erschaffen → jung → reif → alt → uralt, nur bei geeigneten Arten; die Phasen können Fähigkeiten ändern, „nicht als Levelsystem, sondern biologisch/übernatürlich“ (`M-PD`: ‚without becoming visible RPG levels‘).

### 7.2 Jungwesen und Kinder

Der Assistent fragte, ob es Monster-Kinder und Jungtiere grundsätzlich geben dürfe (erzählerisch stark, aber moralisch heikel, `23#307A` Frage B). Der Entwickler antwortete: „B: ja aber nur ausgewählt“ (`23#308E`). Das ist **eine Beschränkung** (📋): nur bei ausgewählten Arten, bewusst eingesetzt, nicht als universeller Mechanismus (`M-PD`: ‚this is not a universal mechanic for every species and must be used deliberately‘).

Die Darstellungsregeln für Gewalt gegen Kinder gelten auch hier: Kinder werden ohne Gore und ohne Verstümmelungsdarstellung dargestellt (`23#1074A` Punkt 148; „148 ja weil es auf xbox und ps vielleicht probleme gibt, mir ist es persönlich egal, aber es sollte halt realistisch sein“ in `23#1075E`). Ob das für **Monster**-Kinder genauso gilt, sagt keine Quelle (❓ Q-K12; Gore-Darstellung: [Gameplay-Systeme](../Gameplay-Systeme.md)).

---

## 8. Aktivitätsbedingungen

### 8.1 Bedingungsmodell

`23#307A` Punkt 20 (📋 Gedanke): Eine Art definiert Aktivitätsbedingungen aus Tag/Nacht, Zeitbereich, Jahreszeit, Wetter, Temperatur, Mondphase, Ortsbedingungen und Sonderzustand. Beispiele des Assistenten (`23#295A`, 💡): jagt nur nachts; besonders aktiv im Winter; Fall beginnt nur zwischen 23:00 und 04:00; nur bei starkem Gewitter; nur bei bestimmter Mondphase; nur im Herbst. Der Entwickler: „manche Nebenmissionen oder andere fälle/aufgaben spawnen nur in gewisser uhrzeit oder sogar jahreszeit“ (`23#294E` Punkt 19).

### 8.2 Zeitfenster vernichten keine Inhalte

Der Spieler darf nicht unwissentlich Inhalt dauerhaft verlieren. Ein Winterfall, der nicht entdeckt wurde, ist vielleicht im nächsten Winter wieder möglich; oder der Vorfall geschieht trotzdem, aber mit anderen Folgen (`23#307A` Punkt 22, „22. ja gut“ in `23#308E`; `M-PD` ‚Organic world-event / case emergence‘). Einmalige Folgen müssen **bewusst** und weltzustandsbewusst sein. Details zum Vorfall-Lebenszyklus: [Modul 15](15-Vorfaelle-und-lebendige-Welt.md) und [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md).

### 8.3 Übergang statt Sprung

Ein Fall entsteht nicht von einer Sekunde auf die andere. Der Entwickler: „es muss einen übergang geben, und vielleicht schon in der zeitung vermehrte komische hinweise und erst später sieht man mehr. ich würde nicht mal wirklich questmarker auf der map machen, wenn man es einstellt“ (`23#308E` Punkt 21).

---

## 9. Seltenheit und Glaubwürdigkeit

Das Übernatürliche ist **verborgen und ungewöhnlich**; die Welt besteht hauptsächlich aus normalen Menschen und Orten (`23#293A` Punkt 19, „19. ja das ist auch wichtig…“ in `23#294E`; `23#359A` Punkt 2, „2. ja sehr gut“ in `23#360E`). Viele Fälle sind seltene Ereignisse. Die Zahl der Kreaturen pro Fläche in der normalen Welt ist **nicht festgelegt** und ausdrücklich niedrig gedacht. Im Fegefeuer können deutlich mehr existieren, ohne ‚alle 20 Meter ein Gegner‘ (`23#465A` Punkt 6, 💡).

---

## 10. Beziehungen zwischen Arten

| Aussage | Quelle | Status |
|---|---|:---:|
| Konkurrenz und Feindschaft zwischen Arten | „43. ja sicher“ in `23#1075E` | 📋 |
| Wesen meiden andere Wesen; das ergibt indirekte Hinweise für Jäger | „44. ja“ in `23#1075E` | 📋 |
| Andere Monster fürchten besonders mächtige Wesen (Ruhe in einem Bereich als Hinweis) | `23#467A` Punkte 25–26 (💡 Beispiel zu Urwesen) | 💡 |

---

## 11. Entwurf: Simulationsmodell

> 💡 **Vorschlag.** Er übersetzt die beschlossene Ökologie in drei Detailstufen (analog zur Staffelung für die Gesellschaft, `48#146A`).

```text
Ebene A  Individuum (Detailstufe: Spieler in der Nähe oder Individuum wichtig)
         vollständige Wahrnehmung/Verhalten (Modul 10)
Ebene B  Gruppe / Nest / Rudel (Detailstufe: Region geladen)
         Aufgaben: Jagd, Rast, Wanderung, Konflikt; periodisch aktualisiert
Ebene C  Region / Population (Detailstufe: Ferne)
         Zähler und Bedingungen: Population, Nahrung, Druck, Territorienkarte, Ereignisse
```

Übergang zwischen den Ebenen **ohne Widerspruch**: Was Ebene C entscheidet (zum Beispiel ‚Rudel A verliert Gebiet‘), muss bei Ladung der Region in Ebene B/A als Zustand ankommen (siehe [Modul 17](17-Persistenz-Daten-und-Modding.md)).

Zeitbedingungen werden als **Weltzustandsprüfung** formuliert (‚Winter UND Nacht UND Wetter ≥ X‘), nicht als Uhr im Monster.

---

## 12. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Ein getötetes Individuum erscheint nie wieder, außer über den benannten Fegefeuer-Weg | `23#1075E` Punkte 30, 37 |
| Ein vernichtetes Nest ist für den Mindestzeitraum nicht wieder aktiv; eine Wiederbesiedlung stammt von anderen Individuen oder Gruppen | `23#1076A` |
| Wanderung wegen Hunter-Druck verschiebt den Fall; die Spur existiert am neuen Ort | `23#293A` Punkt 20; `23#1074A` Punkt 65 |
| Ein Fall entsteht nur, wenn die Aktivitätsbedingungen erfüllt sind, mit Vorlauf-Hinweisen | `23#307A` Punkt 21 |
| Ein verpasster Zeitfenster-Fall ist wieder möglich oder besteht verändert, aber nicht gelöscht | `23#307A` Punkt 22; `M-PD` |
| Kein Kind-Wesen ohne Auswahlmarkierung der Art (nur ‚ausgewählt‘) | `23#308E` Frage B |
| Territoriumsänderungen ohne Spieler sind im Spielstand gespeichert und nach Laden vorhanden | `48#144A`; `M-PD` ‚World state and persistence‘ |

---

## 13. Sonderfälle

| Fall | Behandlung |
|---|---|
| Spieler zerstört ein Nest, ein Mitglied entkommt | das Mitglied bleibt ein Individuum; Nest bleibt zerstört |
| Eine Art ist regional unterschiedlich | „87 ja genau“ in `23#1075E`: regionale Varianten und Mutationen sind möglich |
| Ein Wesen wird von einem NPC-Hunter getötet | es ist tot; Weltzustand ändert sich ([Modul 11](11-Hunterwelt-Organisationen-und-Kulte.md)) |
| Monster nehmen Futter von Menschen, die verschwinden | Vermisstenfall → Polizei → Medien ([Modul 15](15-Vorfaelle-und-lebendige-Welt.md)) |
| Eine Art nimmt zu, weil Beute zunimmt | Populationsmodell (Ebene C) |

---

## 14. Was fehlt

- Zahlen: Populationsgrößen, Regenerationsdauer von Gebieten, Mindestzeit der Wiederbesiedlung (❓ Q-K10).
- Welche Arten Individuen und welche nur Population sind (❓ Q-K02).
- Welche Arten sich vermehren und wie (❓ Q-K11).
- Ob Monster-Kinder denselben Darstellungsregeln wie Menschenkinder unterliegen (❓ Q-K12).
- Technische Staffelung der Simulation (❓, Performance- und Architekturentscheidung).
