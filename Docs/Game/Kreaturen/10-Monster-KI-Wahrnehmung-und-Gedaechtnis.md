# Kreaturen 10: Monster-KI, Wahrnehmung und Gedächtnis

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Kreaturen wahrnehmen, wissen, suchen, Taktiken wählen, lernen, sich erinnern, fliehen und jagen. Das **Grundsystem** der Stealth-/Horror-KI (Wahrnehmung mit Unsicherheit, Wissensmodell, Suchphasen) ist im Master als Entscheidung geführt und hier nur soweit wiederholt, wie es Kreaturen betrifft. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Wahrnehmung](#2-wahrnehmung)
3. [Wissensmodell und Suche](#3-wissensmodell-und-suche)
4. [Kommunikation](#4-kommunikation)
5. [Taktikvorrat](#5-taktikvorrat)
6. [Lernen und Anpassung](#6-lernen-und-anpassung)
7. [Reaktion auf Fallen und Schutz](#7-reaktion-auf-fallen-und-schutz)
8. [Gedächtnis und Beziehungen](#8-gedächtnis-und-beziehungen)
9. [Flucht, Rückzug, Hinterhalt, Jagd auf den Spieler](#9-flucht-rückzug-hinterhalt-jagd-auf-den-spieler)
10. [Übernatürliche Wahrnehmung und falsche Sinneseindrücke](#10-übernatürliche-wahrnehmung-und-falsche-sinneseindrücke)
11. [Entwurf: Architektur und Prüfwerkzeug](#11-entwurf-architektur-und-prüfwerkzeug)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Sonderfälle](#13-sonderfälle)
14. [Nemesis und Rivalität (Produktentscheidung vom 22.10.2026)](#14-nemesis-und-rivalität-produktentscheidung-vom-22102026)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Monster-KI variiert **stark nach Monsterart**; keine humanoide Universal-KI | `M-PD` ‚NPCs and AI‘; „55-60 ja“ in `23#1075E` | 📋 |
| Arten kämpfen unterschiedlich; Lernfähigkeit besonders intelligenter Gegner; Flucht; verletzte Monster verstecken sich und erholen sich; Hinterhalt; Jagd auf den Spieler | `23#1074A` Punkte 55–60; „55-60 ja“ in `23#1075E` | 📋 |
| Wahrnehmung mit Unsicherheit statt magischer Spielerkoordinaten; kein globales Anti-Spieler-Wissen | `M-PD` ‚Stealth, perception and horror AI‘ | 📋 |
| Vier Wissensgrade: bestätigtes Wissen, starker Verdacht, schwacher Verdacht, unbekannt | `23#253A` Punkt 1; „1. ja genau“ in `23#254E`; `M-PD` | 📋 |
| Die letzte bekannte Position wird mit der Zeit ungenauer; der Gegner verliert die Genauigkeit, nicht das Wissen, dass eine bestätigte Bedrohung existiert | `23#253A` Punkt 2; „2. passt“ in `23#254E`; `M-PD` | 📋 |
| Suchphasen: Sofortverfolgung → lokale Suche → erweiterte Suche → Eindämmung → vorsichtige Wachsamkeit; Dauer und Intelligenz hängen vom Schwierigkeitsgrad ab; auch Grounded ist nie ‚allwissend‘ | `23#253A` Punkt 3; „3. ja haben wir schon besprochen“ in `23#254E`; `M-PD` | 📋 |
| Monster bekommen **Taktikvorräte**; nicht jede Kreatur hat jede Fähigkeit; Taktiken werden gewechselt, wenn sie nicht funktionieren | `23#253A` Punkt 6; „6. ja kommt aufs monster an“ in `23#254E` | 📋 |
| Kein billiges Zufallsverhalten: Taktikwahl bewertet die Situation, mit Variation, damit sie nicht perfekt vorhersagbar ist | `23#253A` Punkt 7; „7. genau, die monster sollen sich anhand von meinem verhalten schlau anpassen“ in `23#254E` | 📋 |
| Monster dürfen **bewusst nichts tun** (Stille, Warten), aber nicht jedes Mal | `23#253A` Punkt 8, „8. ja stimmt“ in `23#254E`; „C. ja das kann es machen, aber nicht zu oft, und es sollte viele verschiedene taktiken haben“ in `23#240E` | 📋 |
| Drei Arten von Geräuschen: physisches Weltgeräusch, vom Monster erzeugtes physisches Geräusch, rein übernatürliche Wahrnehmung nur für das Ziel | `23#253A` Punkt 9, „9. ok gut“ in `23#254E`; `M-PD` | 📋 |
| Kampf kann in Stealth/Suche zurückfallen, wenn die Sichtlinie verloren ist | `23#253A` Punkt 13, „13. passt gut“ in `23#254E`; `M-PD` | 📋 |
| Verletzung macht den Spieler leichter wahrnehmbar (Humpeln, Atmung, Schmerzlaute, Blutspur) | `23#253A` Punkt 14, „14. ja passt“ in `23#254E`; `M-PD` | 📋 |
| Beleuchtung ist Teil der Wahrnehmung, und der Spieler ist nicht ‚unsichtbar im Schatten‘ | `23#253A` Punkt 15, „15. ja sehr wichtig“ in `23#254E` | 📋 |
| Die KI muss **testbar** sein (Inspektor mit bestätigten Fakten, Verdacht, letzter Position, Hörschätzungen, Suchphase, Taktik, Begründung, geteilte Informationen); „es müssen auch unendlich viele tests geben“ | `23#253A` Punkt 16; „16. ja passt, es müssen auch unendlich viele tests geben weil ich das nie alles 100% überprüfen kann“ in `23#254E` | 📋 |
| Monster merken sich Waffen, Fallen und Jagdmethoden und erkennen den Hunter wieder (**umfassendes individuelles Gedächtnis**) | „Supernatural Monster-Gedächtnis: Umfassendes individuelles Gedächtnis mit Beziehungen, Rache, Angst und Informationsaustausch“ in `48#139E` | 📋 |
| Monster reagieren **taktisch** auf vorbereitete Fallen (erkennen, untersuchen, umgehen, sabotieren, täuschen, Gegenstrategien) | „Supernatural Monster-KI bei Fallen: Taktische Monster mit Sabotage und Gegenstrategien“ in `48#137E` | 📋 |
| Ein Wesen, das bereits wirksam gefangen ist, darf sich nicht durch intelligente KI befreien; die Mythologie gilt weiter | `48#138A` (Auslegung zur gewählten Option) | 📋 / 💡 |
| Intelligente Monster können Lichter und Strom manipulieren, falsche Geräusche und Stimmen erzeugen, schweigen, Ausgänge sperren, Jäger isolieren, Verletzte verfolgen, Begleiter angreifen, sich zurückziehen | `M-PD` ‚Stealth, perception and horror AI‘ | 📋 |
| Das Monster merkt sich nur, was es **plausibel beobachtet oder erfahren** hat | `23#307A` Punkt 15, „15. super“ in `23#308E`; `M-PD` | 📋 |

---

## 2. Wahrnehmung

### 2.1 Grundsatz (Master)

Wahrnehmung nutzt Unsicherheit. Hören schätzt Richtung, Entfernung und Unsicherheit aus tatsächlicher Schallausbreitung; Sehen berücksichtigt Richtung, Reichweite, tatsächliches Licht, Bewegung, Deckung, Silhouette, Haltung und Sicht (`M-PD`). Dunkelheit verringert die Sicht, ohne Unsichtbarkeit zu gewähren; ein gehörter Spieler im Dunkeln kann der Richtung nach geortet werden, ohne optisch erkannt zu sein. Zustände: unaware, suspicious, investigating, alert, confirmed target, searching (`M-PD`).

### 2.2 Artspezifische Sinne

`M-PD`: Kreaturenkommunikation ist artspezifisch; Geruchsverfolgung ist artspezifisch; Blutspuren verfolgen Menschen sichtbar, wo plausibel. Die Quelle `23#253A` (Schluss) verweist auf **konkrete Wahrnehmungs- und Taktikregeln jedes Monsters** als späteren Teil. In `23#1074A`/`23#1075E` gibt es dazu keine Einzelregeln.

### 2.3 Kreaturen und Licht

Kreaturenspezifische KI darf Lichter und Strom manipulieren (`M-PD`). Das Licht des Spielers (Taschenlampe) ist Werkzeug und Risiko. Raytracing ist als Ziel gesetzt, die Umsetzung (hardwaregeraytraced, Hybrid) später (`23#241A`); die KI braucht keinen vollständigen Pfadverfolger, die Sichtbarkeitsmetrik soll aber mit der Darstellung korrelieren (`23#253A` Punkt 15, „15. ja sehr wichtig“ in `23#254E`).

---

## 3. Wissensmodell und Suche

### 3.1 Wissensgrade

`23#253A` Punkt 1 (Beispiel, 💡): Sieht das Monster den Spieler in einen Schrank steigen, ist das **bestätigt**; endet die Blutspur am Schrank, ist es **starker Verdacht**; ist der Spieler im Schlafzimmer verschwunden, ist es **schwacher Verdacht**; ohne Hinweis **unbekannt**. Das gilt auch für Position, Verletzungen, verwendete Waffen, bevorzugte Fluchtwege, Ablenkungen, Begleiter und Verstecke.

### 3.2 Suche

Die Suchphasen und ihre Bedeutung stehen in `23#253A` Punkt 3 und `M-PD`. Für Kreaturen gilt zusätzlich: Verstecke sind nie garantiert sicher; ein Gegner darf einen Schrank, ein Bett oder einen Nebenraum auch ohne Beweis prüfen; wenn er gesehen hat, wie der Spieler ein bestimmtes Versteck nutzt, prüft er solche Verstecke später gezielter (`M-PD`; Bestätigung „12. genau, er darf aber einfach so nachschauen, wenn er dich findet ist es halt blöd“ in `23#240E`).

### 3.3 Ablenkungen

Wiederholte Ablenkungen werden von intelligenten Gegnern erkannt; sie werden skeptischer, ignorieren sie, untersuchen anders oder vermuten eine Falle (`M-PD`; `23#241A`, 💡).

---

## 4. Kommunikation

Menschliche Gegner teilen Informationen nur durch plausible Kommunikation (Stimme, Funk, Sichtzeichen); es gibt keinen Schwarmverstand (`M-PD`; `23#253A` Punkte 4–5, „4. ja passt“, „5. passt“ in `23#254E`). **Kreaturen:** Kommunikation ist artspezifisch (`M-PD`). Das Prinzip ‚kein Schwarmverstand‘ gilt auch für Rudel: Ein Rudelmitglied kennt die Position des Spielers nur, wenn die Art einen Kanal hat (Laut, Geruch, Blickkontakt, Befehl, übernatürliche Verbindung). Welche Arten welche Kanäle haben, ist Artdefinition (❓).

Informationsweitergabe über **längere Distanz und Zeit** (ein Dämon erzählt anderen von den Methoden des Hunters) gehört zum Gedächtnis (Abschnitt 8) und zur Gesellschaft ([Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md)).

---

## 5. Taktikvorrat

### 5.1 Der Vorrat

`23#253A` Punkt 6 (💡 Liste, 📋 Prinzip; „6. ja kommt aufs monster an“ in `23#254E`) und `M-PD`: beobachten, direkte Jagd, Hinterhalt, Flanke, Ausgang blockieren, falsche Geräusche, Stimmen imitieren, Strom abschalten, Licht manipulieren, vollständig still bleiben, Jäger trennen, verletzten Jäger verfolgen, Rückzug, Neupositionierung, Begleiter angreifen, auf Nachladen warten, Verletzungen ausnutzen. Nicht jede Kreatur besitzt jede Fähigkeit.

### 5.2 Auswahl

Die Auswahl ist **kontextabhängig** (`23#253A` Punkt 7):

| Lage | Folge (Beispiele der Quelle, 💡) |
|---|---|
| Spieler verletzt | Verfolgen und Druck attraktiver |
| Zwei Jäger zusammen | Isolation versuchen |
| Großer Abstandsvorteil des Spielers | Sichtlinie brechen |
| Strom noch an | Sabotage erwägen |
| Wiederholte Ablenkungen | Misstrauen steigt |

Dazu etwas Variation, damit das Verhalten nicht perfekt vorhersagbar wird. Ein starkes Monster darf nach fünf Begegnungen nicht komplett durchschaubar sein (`23#241A`, 💡; der Entwickler: „es sollte viele verschiedene taktiken haben“, `23#240E`).

### 5.3 Bewusste Stille

Monster dürfen sich verfolgt zurückziehen und zwei Minuten lang nichts tun, ohne Geräusch und Angriff (`23#253A` Punkt 8). Das darf nicht jedes Mal geschehen.

---

## 6. Lernen und Anpassung

| Aussage | Quelle | Status |
|---|---|:---:|
| Monster können lernen: Wenn dasselbe Wesen mehrere Begegnungen überlebt, kann es sich an wiederholte Taktiken erinnern (Schrotflinte, Licht an lassen, Verstecken, Partner) | `23#293A` Punkt 12 | 📋 |
| Es hängt vom Monster und von der Intelligenz ab | „12. ja aber das kommt auf das monster und die intelligenz ab“ in `23#294E` | 📋 |
| Es wird nur aus **tatsächlich beobachteten** Informationen gelernt, nicht aus globaler Telemetrie oder Metawissen | `23#293A` Punkt 12; `M-PD` | 📋 |
| Die Monster sollen sich **anhand des Spielerverhaltens** schlau anpassen | „7. genau, die monster sollen sich anhand von meinem verhalten schlau anpassen“ in `23#254E` | 📋 |
| Besonders intelligente Gegner passen sich an wiederholte Taktiken an | „55-60 ja“ in `23#1075E` | 📋 |
| Höhere Schwierigkeit (besonders Grounded): aufmerksamer, aggressiver, schneller beim Ausnutzen von Lücken; Nachladegeräusche werden gehört und bestraft | `M-PD`; „1. auf höherem schwierigkeitsgrad sind die gegner viel aufmerksamer und gnadenlos, du lädst in der nähe nach, rest in piece so in etwa“ in `23#86E` | 📋 |

Wichtig: Lernen ist **Wissensaufbau**, nicht Stärkerwerden. Es gibt keinen Wert, der mit jeder Begegnung steigt (L2). Ob ein Lerneffekt pro Individuum, pro Art oder pro Gruppe gilt, ist offen (❓ Q-K15).

---

## 7. Reaktion auf Fallen und Schutz

Die Quiz-Entscheidung `48#136A` fragte: ‚Wie intelligent sollen Monster auf vorbereitete Fallen reagieren?‘ Optionen: A einfach (Fallen auslösen, nicht aktiv erkennen), B unterschiedliche Aufmerksamkeit, C taktisches Verhalten (untersuchen, umgehen, sabotieren, täuschen, Gegenstrategien), D individuelle Monsterintelligenz (Art, Erfahrung, Wissen, Persönlichkeit, Situation, bisherige Begegnungen). Der Entwickler wählte mit der Beschreibung der Option C: ‚Taktische Monster mit Sabotage und Gegenstrategien‘ (`48#137E`).

Der Assistent leitet daraus ab (`48#138A`, 💡 Auslegung): Monster können Fallen erkennen, untersuchen, umgehen, sabotieren, den Hunter täuschen (eine Falle scheinbar übersehen) und Gegenstrategien entwickeln. Es gilt aber weiter die Mythologie: **Ein Dämon, der bereits vollständig in einer wirksamen Teufelsfalle gefangen ist, befreit sich nicht durch intelligente KI.**

| Frage | Antwort in den Quellen |
|---|---|
| Wie sabotieren Monster Schutzmaßnahmen? | `48#134A`: Monster können versuchen, Schutzmaßnahmen zu sabotieren (Tabelle zur Fallen-Persistenz, 💡 Auslegung zur Option ‚Realistische Persistenz‘) |
| Kann ein Wesen, das die Falle durchschaut, sie ohne Folgen umgehen? | gewollt: Ja, wenn die Art es kann; sonst nicht (Prinzip aus `48#138A`) |
| Was erkennen Monster? | symbolische Zeichen, Materialien, Veränderungen der Umgebung (`48#138A`, 💡) |
| Wie viel bringt Intelligenz? | Option D wurde **nicht** gewählt; die Individualität bleibt Teil der Mythologie (Option C gewählt). Wie stark Persönlichkeit und Erfahrung die Reaktion ändern, ist offen (❓ Q-K15) |

Das Gesamtsystem der Fallen und Schutzvorrichtungen steht in [Modul 13](13-Lebendfang-Fallen-und-Containment.md).

---

## 8. Gedächtnis und Beziehungen

### 8.1 Entscheidung

`48#138A` stellte die Frage ‚Soll sich ein Monster an frühere Begegnungen mit dem Spieler erinnern?‘ (A nur innerhalb einer Begegnung, B einfache Erinnerung, C langfristiges taktisches Gedächtnis, D umfassende individuelle Erinnerungen mit Beziehungen, Rache, Angst, Informationsaustausch, langfristigen Verhaltensänderungen). Gewählt: **D** (`48#139E`).

### 8.2 Inhalte

Aus der Quelle (`48#140A`, 💡 Auslegung der 📋 Wahl; `23#307A` Punkt 15, 📋):

| Gedächtnisfeld | Inhalt |
|---|---|
| Wer | den Spieler, Begleiter, andere Hunter wiedererkennen |
| Verletzungen | wie das Wesen verletzt wurde |
| Waffen | welche Waffe wirksam war |
| Fallen | welche Fallen benutzt wurden |
| Gnade | ob es verschont wurde |
| Gruppe | ob seine Familie/Gruppe getötet wurde |
| Gefühle | Angst, Hass, Misstrauen, Respekt, Rache |
| Weitergabe | Wissen über Hunter und Fallen an andere Wesen |
| Langfristigkeit | Verhaltensänderungen über Begegnungen |

### 8.3 Grenzen

- Nur plausibel erworbenes Wissen (`23#307A` Punkt 15; `M-PD`).
- „Nicht jede Kleinigkeit muss dauerhaft gespeichert werden; entscheidend sind relevante Erfahrungen und ihre Auswirkungen“ (Folgerung in `48#140A`, 💡). Für die technische Seite siehe [Modul 17](17-Persistenz-Daten-und-Modding.md).
- Das Gedächtnis eines Wesens im **Fegefeuer** kann Erinnerungen aus dem früheren Leben enthalten ([Modul 06](06-Daemonen-Engel-und-Leviathane.md) §5).

### 8.4 Die Wiederkehr eines Gegners

`23#293A` Punkt 20 und Frage C: Ein Wesen, das der Spieler nur vertreibt oder das entkommt, kann später wieder auftauchen, mit Erinnerung und früheren Verletzungen. Entwickler: „es kann auftauchen aber es muss nicht“ (`23#294E`, Antwort C); es entscheidet der Spieler, ob er ein Wesen tötet oder freilässt.

---

## 9. Flucht, Rückzug, Hinterhalt, Jagd auf den Spieler

| Aussage | Quelle | Status |
|---|---|:---:|
| Monster müssen nicht bis zum Tod kämpfen; sie fliehen, wechseln den Bau, das Opfer, verstecken sich, suchen Verstärkung | `23#293A` Punkt 13, „13. ja das sowieso“ in `23#294E` | 📋 |
| Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf | `23#1074A` Punkt 58, „55-60 ja“ in `23#1075E` | 📋 |
| Manche beobachten den Spieler und greifen zu einem besseren Zeitpunkt an | Punkt 59 | 📋 |
| Sehr gefährliche Wesen können den Spieß umdrehen und den Spieler jagen | Punkt 60 | 📋 |
| Safehouses sind nicht absolut sicher; bestimmte sehr seltene Gegner können Spielerstandorte herausfinden, wenn man unvorsichtig ist; selten, sonst nervt es | Punkt 61; „61 ja das kann etwa so sein wie bei rdr2, mit dem umziehen“ in `23#1075E` | 📋 |
| Monster können Hunter-Standorte angreifen (HQ nicht dauerhaft sicher) | `23#1074A` Punkt 61, derselbe Eintrag | 📋 |

---

## 10. Übernatürliche Wahrnehmung und falsche Sinneseindrücke

`23#253A` Punkt 9 (📋 Prinzip „9. ok gut“): drei Arten von Geräuschen. Zum Beispiel hört nur das Ziel eine übernatürliche Stimme seines Begleiters; die normale KI erhält **keinen** Hörreiz. Das ist für Horror wichtig und für Tests (Abschnitt 11): Die Welt hat dann kein physisches Geräusch erzeugt.

Falsche Sinneseindrücke durch Kreaturen (Halluzination, Imitation, Stimmen) sind damit zwischen **Weltereignis** und **Wahrnehmungsereignis** getrennt.

---

## 11. Entwurf: Architektur und Prüfwerkzeug

> 📋 **Technikrahmen (verbindlich, PROJECT_DECISIONS „NPCs and AI“; keine spätere Nutzerentscheidung überholt):** StateTree organisiert den hierarchischen Zustand, eigenes C++ übernimmt Wahrnehmung, Gedächtnis, Kampf und Sonderregeln.
>
> 💡 **Vorschlag:** die Aufteilung, wo die einzelnen Teile liegen.

```text
CreatureBrain
├── Perception        Sinne laut Artdefinition → PerceptionEvents (mit Unsicherheit)
├── KnowledgeModel    confirmed | strong | weak | unknown  (pro Fakt)
├── Memory            episodisch (Begegnungen), semantisch (Spieler/Methoden), relational
├── TacticSelector    bewertet Lage → Taktik aus dem Vorrat der Art
├── SearchState       Phasen gemäß Master
├── SocialChannels    welche Informationen wie weitergegeben werden
└── SpecialRules      artspezifisch (C++), z. B. Licht/Strom manipulieren

AIInspector (Entwicklerwerkzeug)
├── bestätigte Fakten, Verdacht, letzte Position, Hörschätzungen
├── aktuelle Suchphase und Taktik, "Warum diese Taktik?"
└── Weitergegebene Informationen (mit Kanal und Zeitpunkt)
```

**Quellenbezug:** StateTree für hierarchischen Zustand, eigenes C++ für Wahrnehmung, Gedächtnis, Kampf und Sonderregeln: `M-PD` ‚NPCs and AI‘; Inspector: `23#253A` Punkt 16, „16. ja passt…“ in `23#254E`. Das Beispiel der Quelle für den Inspektor: ‚Heard door at 05:17, Saw blood trail at 05:18, Received radio call at 05:18 → Strong Suspicion: Basement‘ (`23#253A`, 💡). Wenn der Inspektor keinen Grund findet, ist es ein Fehler.

---

## 12. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Eine KI kennt die Position des Spielers nur aus Wahrnehmungsereignissen, Gedächtnis oder erlaubter Kommunikation | `M-PD`; `23#253A` Punkt 1 |
| Ein Lerneffekt entsteht nur aus einem Ereignis, das das Wesen beobachtet hat | `23#293A` Punkt 12 |
| Ein Hörreiz aus übernatürlicher Wahrnehmung erreicht nur das benannte Ziel | `23#253A` Punkt 9 |
| Bewusste Stille tritt nicht in jeder Begegnung auf | `23#253A` Punkt 8; `23#240E` Antwort C |
| Der Spieler wird nicht aus fremder Erfahrung ‚verraten‘ (kein Telemetriewissen) | `M-PD` |
| Ein in einer wirksamen Falle gefangenes Wesen kann sich nicht aus ihr befreien, außer die Mythologie lässt es zu | `48#138A` |
| Dasselbe Individuum erinnert sich an einen früher gefundenen Trick nach Speichern und Laden | `48#139E`; `23#307A` Punkt 15 |
| Ein Rudelmitglied weiß nur dann vom Spieler, wenn die Art einen Kanal hat | `M-PD` ‚Creature communication is species-specific‘ |

---

## 13. Sonderfälle

| Fall | Behandlung |
|---|---|
| Wesen ohne Augen | Sinne laut Artdefinition (❓) |
| Wesen kopiert die Stimme des Begleiters | Fake-Sound-Regel (Abschnitt 10); Imitation ist Taktik, kein Weltzustand |
| Zwei Wesen verschiedener Arten teilen sich ein Revier | Beziehungen zwischen Arten ([Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §10) |
| Ein Wesen erkennt den Spieler an Narben oder Wunden wieder | über Gedächtnis (Abschnitt 8) und Narben ([Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §6) |
| Spieler verkleidet sich | dieselben Wiedererkennungsregeln wie bei Menschen (Sicht, Kleidung, Maskierung, [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md)) |

---

## 14. Nemesis und Rivalität (Produktentscheidung vom 22.10.2026)

Am 22.10.2026 hat der Entwickler eine **verbindliche Produktvision** zu einer lebendigen Welt mit NPC-Gedächtnis und Nemesis-/Rivalitätssystem bestätigt (`PE-N1022`; Wortlaut der acht Anforderungen A–H in `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` §1; Zusammenfassung in [Gameplay-Systeme §8](../Gameplay-Systeme.md)). Sie ist **kein M1-Feature**, die Reihenfolge der Umsetzung ist offen, und es wird **keine Umsetzung** davon begonnen. Für Kreaturen ist besonders relevant:

| Anforderung | Inhalt (nach Proposal 0010 §1) | Bezug in diesem Ordner |
|---|---|---|
| A Persistente Identitäten | wichtige NPCs behalten Identität, Eigenschaften, Beziehungen, Ziele, Lebensgeschichte | [Modul 03](03-Klassifikation-Herkunft-und-Individuen.md) §8, [Modul 09](09-Intelligente-Wesen-und-Monstergesellschaft.md) |
| B Langzeitgedächtnis | Erinnerung an Begegnungen, Entscheidungen, Hilfe, Verrat, Verletzungen, Konflikte | Abschnitt 8 |
| C **Nemesis und Rivalität** | überlebende Gegner können zurückkehren, persönliche Feindschaften entwickeln, aus früheren Begegnungen lernen und Taktiken anpassen; ausdrücklich auch **intelligente übernatürliche Kreaturen** | Abschnitte 6, 8.4, 9 |
| D Autonome Fraktionen | Monstergruppen, Hunter-Organisationen, Netzwerke, Behörden verfolgen Ziele, bilden Allianzen, beanspruchen Gebiete, kämpfen, ändern Strategien | [Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §4.4, Modul 09 §4 |
| E Persistente Konsequenzen | Verletzungen, Tode, Rettungen, zerstörte Infrastruktur wirken lange | [Modul 15](15-Vorfaelle-und-lebendige-Welt.md) |
| F Eigenständige Lebensentwicklung | wichtige NPCs entwickeln sich ohne Spielerkontakt | Modul 09, [Modul 11](11-Hunterwelt-Organisationen-und-Kulte.md) |
| G Skalierbare Simulation | hohe Genauigkeit nahe am Spieler, reduziert in der Ferne, ereignisbasiertes Fortschreiben ungeladener Gebiete | Modul 08 §11, [Modul 17](17-Persistenz-Daten-und-Modding.md) §5 |
| H Integration | ohne widersprüchliche Zustände zwischen Weltzustand, Ermittlung, Monster-KI/Bestiarium, Verletzungen, Ruf, Polizei, Organisationen, Dialog, Speichern/Laden, Koop | alle Module |

**Grenze für Kreaturen (aus Proposal 0010 §4, 💡 Vorschlag):** Bei übernatürlichen Wesen ist die Anpassung durch die Artregeln und Schwächen des Bestiariums begrenzt, **nie durch freie Erfindung**. Das deckt sich mit dem, was die Quellen dieses Ordners ohnehin sagen: Ein Wesen lernt nur aus Beobachtetem (Abschnitt 6), und Schwächen bleiben Artregel ([Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md)).

**Der Vorschlag des Proposals (alles 💡, nichts entschieden):** Entitätsdatensätze statt Actors; ein Ereignisjournal als Quelle der Wahrheit, aus dem Gedächtnis und Beziehung abgeleitet werden; ein begrenztes Gedächtnismodell je Individuum (Stärke, beobachtet oder gehört, Genauigkeit, Verfall, Wichtigkeit); ein Nemesis-Datensatz mit Begegnungshistorie, gelerntem Modell der Spielermethoden, Verletzungen, Ressourcen, Verbündeten und Zielen; ein Fraktionsmodell als Zustandsmaschine mit grober Zeitschrittweite; Simulationsstufen (volle Akteure, reduzierte Orte, statistische Ferne, ereignisbasiertes Nachholen); deterministisches, saatbasiertes Fortschreiben; Debug-Werkzeuge, die erklären, warum ein NPC handelt.

**Phasen im Proposal (§10, unverbindlich):** L3 sieht ‚einen wiederkehrenden Gegner (Nemesis-Prototyp) für einen einzelnen Kreaturentyp, begrenzt durch seine Artregeln‘ vor, erst nach Datenmodell (L1) und einem benannten NPC mit Gedächtnis (L2).

**Offen (Proposal 0010 §11):** Entitäts- und Ereignisschema, Gedächtnismodell und Verfall, welche Historie das Journal behält (Verdichtung), Schwellen und Budgets der Simulationsstufen, **wie ein Nemesis lernt (Regeln oder Daten)**, Fraktionsliste und Ziele, Regeln für geschützte/autorisierte NPCs, was der Spieler von Feindschaft beobachten kann, Replikation des Simulationszustands, Weltuhr und Zeitreise, Mod-Erweiterungspunkte (❓ Q-K15, Q-K25, Q-K33).

---

## 15. Was fehlt
- Konkrete Sinne, Taktikvorräte und Gedächtnisgrenzen **pro Art** (❓).
- Ob Lerneffekte für Individuum, Gruppe oder Art gelten (❓ Q-K15).
- Zahlen: Reichweiten, Zeiten, Schwellen (nicht Teil dieses Ordners).
- Wie weit Persönlichkeit und Erfahrung die Reaktion auf Fallen ändern (❓ Q-K15).
- Die Anbindung an Netzwerk und Koop (Autorität der KI im Mehrspielerbetrieb, [Modul 17](17-Persistenz-Daten-und-Modding.md)).

## Ergänzung aus dem Entscheidungsabgleich 31.08. (Stand 09.11.2026)

- 📋 **Reaktion auf Verfolgung (Entscheidung 29, Richtung):** Intelligente Monster können auf die Jagd reagieren, indem sie ihr Lager verlegen, Beweise verbergen, ihre Opfermuster ändern, Fallen stellen, Jäger beobachten oder falsche Fährten legen. Nicht jede Art kann alles; die Auswahl hängt von Art und Individuum ab.
- 📋 **Art-spezifische Ernte (Entscheidung 29):** Monsterernte richtet sich nach der Art und ist **nicht** immer Häuten. Proben können Blut, Gewebe, Knochen, Zahn, Kralle, Organe, Sekrete oder übernatürliche Rückstände umfassen.
