# Kreaturen 04: Anatomie, Verletzung, Schwächen und Regeneration

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie der Körper einer Kreatur, ihre Verletzungen, ihre Schwächen und Widerstände sowie ihre Regeneration modelliert werden sollen. Es sagt nichts über konkrete Arten und keine Zahlen. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Physischer Schaden und übernatürliche Wirkung](#2-physischer-schaden-und-übernatürliche-wirkung)
3. [Modulare Anatomie und funktionale Verletzung](#3-modulare-anatomie-und-funktionale-verletzung)
4. [Schmerz](#4-schmerz)
5. [Regeneration](#5-regeneration)
6. [Bleibende Verletzungen und Narben](#6-bleibende-verletzungen-und-narben)
7. [Schwächen](#7-schwächen)
8. [Resistenzen und Immunität](#8-resistenzen-und-immunität)
9. [Endgültiges Töten und andere Lösungen](#9-endgültiges-töten-und-andere-lösungen)
10. [Übernatürliche Verletzungen beim Menschen](#10-übernatürliche-verletzungen-beim-menschen)
11. [Übernatürliche Heilung](#11-übernatürliche-heilung)
12. [Entwurf: Wirkungsmodell](#12-entwurf-wirkungsmodell)
13. [Entwurf: Prüfregeln](#13-entwurf-prüfregeln)
14. [Sonderfälle](#14-sonderfälle)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Jede Art hat eigene Regeln für Anatomie, Wahrnehmung, Fortbewegung, Schwächen, Immunitäten, Regeneration und Tötungsmethode; kein einheitlicher ‚Gesundheitswert mit anderen Zahlen‘ | `23#293A` Punkte 1–2; „1. ja das ist wichtig 2. passt“ in `23#294E` | 📋 |
| **Physischer Schaden** und **übernatürliche Wirkung** bleiben getrennt; Spezialmaterial wirkt nur, wenn die konkrete Kreaturenregel es begründet, nicht als Bonusschaden | `23#293A` Punkt 3, „3. genau“ in `23#294E` | 📋 |
| Manche Wesen können verletzt, aber **nicht damit getötet** werden; physische Verletzung verschafft Zeit, ersetzt aber nicht die richtige Methode | `23#293A` Punkt 4, „4. passt gut“ in `23#294E` | 📋 |
| Regeneration ist **monsterabhängig** und wird in Teilregeln zerlegt | `23#293A` Punkt 5, „5. ja“ in `23#294E`; `23#307A` Punkt 13, „13. passt“ in `23#308E`; `M-PD` | 📋 |
| Schmerzempfinden ist individuell; kein Schmerz bedeutet **nicht** keine Beeinträchtigung | `23#293A` Punkt 6, „6. genau“ in `23#294E` | 📋 |
| Anatomie ist modular und artspezifisch | `23#307A` Punkt 11, „11. ok“ in `23#308E`; `M-PD` | 📋 |
| Funktionale Verletzungen gelten auch für Kreaturen, solange die Anatomie physisch funktioniert | `23#307A` Punkt 12, „12. genau so“ in `23#308E`; `M-PD` | 📋 |
| Bleibende Narben und Verletzungen überdauern Begegnungen, wenn die Art nicht regeneriert; wiederkehrende Individuen sind daran erkennbar | `23#307A` Punkt 14, „14. das wäre cool“ in `23#308E`; `M-PD` | 📋 |
| Eine Schwäche wirkt nicht nur als Bonusschaden; sie kann tödlich, stark schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, wahre Gestalt enthüllend oder fähigkeitsstörend sein | `23#307A` Punkt 9, „9. gut“ in `23#308E`; `M-PD` | 📋 |
| Resistenz ist intern abgestuft; die Oberfläche zeigt beobachtete Belege, keine Prozentwerte | `23#307A` Punkt 10, „10. passt“ in `23#308E`; `M-PD` | 📋 |
| Übernatürliche Verletzungen **erweitern** das normale Traumasystem (Modifikatoren wie Heilungsunterdrückung oder Fluch), sie sind kein eigenes Gesundheitssystem | `M-PD` ‚Human anatomy, injury and medicine‘ | 📋 |
| Verletzungen durch Wesen sind **mythologieabhängig** mit eigenen Krankheiten, Flüchen und Behandlungen, die der Hunter erst verstehen muss (Option C, vom Entwickler gewählt) | `48#122A` (Frage), `48#123E` (Wahl), `48#124A` | 📋 |
| Gore ist ursachenabhängig (Pistole ≠ Schrotflinte aus der Nähe ≠ Messer ≠ Axt ≠ Kralle); bei Kindern keine Gore-Darstellung | `M-PD`; „147 ja genau, sehr realistisch“, „148 ja“ in `23#1075E` | 📋 |
| Unterschiedliche Arten können unterschiedliche Verletzungs- und Zerstörungssysteme benötigen; Tatorte behalten plausibel Blut und Schäden bis zur Reinigung | „149-150 ja“ in `23#1075E` | 📋 |

---

## 2. Physischer Schaden und übernatürliche Wirkung

Die Quelle (`23#293A` Punkt 3) trennt:

```text
normales Projektil → physische Verletzung
Silber-/Spezialprojektil → physische Verletzung + spezifische übernatürliche Wirkung
```

**Regel (📋):** Hat das Wesen keine besondere Schwäche gegen das Material, bekommt es keinen Bonusschaden. Die Wirkung ist **eine Aussage über die Art** (Wirkungsart, siehe Abschnitt 7), nicht über die Waffe.

Daraus folgen für die Anzeige im Archiv zwei getrennte Beobachtungen: ‚Was die Waffe körperlich getan hat‘ und ‚Was sie übernatürlich bewirkt hat‘. Beides kann der Spieler falsch deuten (L7, [Modul 02](02-Leitsaetze-und-Abgrenzung.md)).

**Beispiel der Quelle (💡):** Ein Schuss lässt das Wesen zurückfallen, es entsteht eine Wunde, es ist kurz beeinträchtigt, aber eine normale Kugel kann den endgültigen Tod nicht verursachen. Der Spieler gewinnt Zeit, kann fliehen, das Wesen verlangsamen oder einen Begleiter retten (`23#293A` Punkt 4).

---

## 3. Modulare Anatomie und funktionale Verletzung

### 3.1 Aufbau

`23#307A` Punkt 11 (💡 Struktur, 📋 Richtung): Kreaturenanatomie besteht aus Körperregionen, strukturellen Teilen, lebenswichtigen Strukturen, Fortbewegungsstrukturen, Sinnesorganen, übernatürlichen Organen und Regenerationsregeln. Ein vierbeiniges Wesen hat andere Regionen als ein humanoides; ein Geist möglicherweise gar keine normale Anatomie.

Nach `23#293A` Punkt 2 gilt:

| Typ | Anatomie |
|---|---|
| Humanoides Wesen | menschliche Grundanatomie plus übernatürliche Modifikatoren |
| Vierbeinige Kreatur | eigene Anatomie |
| Sehr großes Wesen | eigene Anatomie |
| Geist | keine normale physische Anatomie |
| Besessener Mensch | menschlicher Körper plus übernatürlicher Besessenheitszustand |

### 3.2 Funktionale Verletzung

Ein Wesen mit zerstörtem Bein darf nicht perfekt sprinten, nur weil es ein Monster ist, **außer** seine Art hat Regeneration, alternative Fortbewegung, Schmerzignoranz oder übernatürliche Kompensation (`23#307A` Punkt 12). Der Master formuliert dieselbe Regel als ‚A destroyed leg should impair movement unless the creature has a specific regeneration/compensation rule‘ (`M-PD`).

### 3.3 Verbindung zum Stealth-System

Verletzungen beeinflussen auch Verhalten und Spuren: Humpeln, schwere Atmung, Schmerzlaute, Blutspuren (`M-PD` ‚Stealth, perception and horror AI‘ für den Spieler; `23#293A` Punkt 6 für Wesen). Kreaturen, die verletzt sind, werden dadurch **aufspürbar** (siehe [Modul 10](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md)).

---

## 4. Schmerz

`23#293A` Punkt 6:

- Ein Wesen kann Schmerz wie ein Mensch wahrnehmen (reagiert ähnlich).
- Ein anderes spürt kaum Schmerz und kämpft trotz schwerer Verletzung weiter.
- **Aber:** ‚kein Schmerz ≠ keine physische Beeinträchtigung‘; ein strukturell zerstörtes Bein bleibt beeinträchtigend.

Das Verhalten einer Kreatur nach einem Treffer (zurückfallen, schreien, weiterkämpfen, fliehen) ist damit Teil der Artregeln und eine **Beobachtung**, aus der der Spieler Schlüsse ziehen kann (Modul 14).

---

## 5. Regeneration

### 5.1 Zerlegung

`23#307A` Punkt 13 und `M-PD`: Regeneration wird zerlegt in

| Teilregel | Inhalt |
|---|---|
| Rate | wie schnell |
| regenerierbare Gewebe | welche |
| Knochen, Organe, Gliedmaßen | ob und welche |
| Ressourcen/Energiebedarf | was gebraucht wird |
| Umweltbedingungen | wann (zum Beispiel nur nachts) |
| Unterdrücker | was sie hemmt |
| absolute Grenzen | was nie regeneriert |

### 5.2 Beispiele der Quelle (💡)

`23#293A` Punkt 5 nennt als Beispiele: Wunden heilen innerhalb von Minuten; Regeneration nur nachts; Regeneration endet durch bestimmtes Material; keine Regeneration, dafür extrem widerstandsfähig; der Körper geht verloren und kehrt später zurück. `23#307A` Punkt 13 ergänzt: schwere strukturelle Zerstörung kann Regeneration verlangsamen.

### 5.3 Sichtbarkeit

Der Spieler sieht Regeneration körperlich (Wunden schließen sich, Gewebe verbindet sich) und nicht als Zahl (`23#467A` Punkte 5–6, zum Beispiel eines Urwesens: ‚erschreckend physisch‘, schnell bei kleinen, langsam und grotesk bei großen Wunden; 💡 Beispiel, Bestätigung als Ganzes durch `23#468E` „Die anderen punkte passen gut“). Das Archiv zeichnet die **Beobachtung** auf, nicht die Formel (`23#295A`, ‚Interne technische Ebene‘).

### 5.4 Wirkung auf den Kampf

Auch bei starker Regeneration gilt: sie ist nicht grenzenlos schnell, sonst wird der Kampf bedeutungslos; ein Wesen kann durch schwere Verletzung vorübergehend bewegungsunfähig werden, und der Spieler kann diese Zeit nutzen (Falle, Ritual, Methode vorbereiten; `23#467A` Punkt 6, 💡).

---

## 6. Bleibende Verletzungen und Narben

`23#307A` Punkt 14 („14. das wäre cool“ in `23#308E`) und `M-PD`:

- Wird ein Wesen schwer verletzt (zum Beispiel Auge verloren, tiefe Narbe, Finger fehlt) und entkommt, können **dieselben Folgen** Monate später noch vorhanden sein, oder regeneriert, wenn die Art das kann.
- Dadurch ist das Individuum **wiedererkennbar**.
- Sichtbare Spuren früherer Kämpfe sind Teil der Individualität und Teil des Archivs (Individuum, Modul 03 §8).

Verbindung zur Persistenz: Ein Individuum trägt seine Verletzungen im Weltzustand ([Modul 17](17-Persistenz-Daten-und-Modding.md)). Die Archivierung dieser Merkmale als wiedererkennbare Hinweise gehört zu Modul 14.

---

## 7. Schwächen

### 7.1 Wirkungsarten

`23#307A` Punkt 9 nennt die folgenden **Wirkungsarten** (💡 Bezeichnungen, 📋 Gedanke):

| Wirkungsart | Bedeutung |
|---|---|
| tödlich | beendet das Wesen |
| stark schädigend | verursacht schweren Schaden, tötet aber nicht sicher |
| regenerationshemmend | unterdrückt Regeneration |
| abwehrend | hält das Wesen zurück |
| lähmend | immobilisiert |
| schutzbrechend | bricht Schutz, Schild oder Hülle |
| manifestationszwingend | zwingt zur Manifestation |
| wahre Gestalt enthüllend | zeigt die tatsächliche Form |
| fähigkeitsstörend | stört eine Fähigkeit |

Beispiel der Quelle: Ein Material tötet die Art normalerweise; bei einem außergewöhnlichen Individuum unterdrückt es nur noch die Regeneration, so dass zusätzliche Recherche nötig ist (`23#307A` Punkt 9).

### 7.2 Mehrere Methoden, Vorbereitung, Improvisation

| Aussage | Quelle | Status |
|---|---|:---:|
| Bestimmte Wesen sind ohne die richtige Methode kaum oder gar nicht endgültig zu töten; Research oder eigenes Testen ist nötig: „91 korrekt, da muss man davor research machen oder es selbst testen“ | `23#1074A` Punkt 91; `23#1075E` | 📋 |
| Manche Arten haben mehr als eine wirksame Strategie | „92 ja“ in `23#1075E` | 📋 |
| Vorbereitung wird belohnt (Fallen, Schutzkreise, Salz, Spezialmunition, Rituale) | „93 ja“ in `23#1075E` | 📋 |
| Improvisation ist erlaubt, solange sie zur Weltlogik passt | „94-98 ja“ in `23#1075E` | 📋 |
| Falsche Lore kann dazu führen, dass ein vermeintlich getötetes Wesen zurückkommt | Punkt 95, ebenfalls „94-98 ja“ | 📋 |
| Bei manchen Arten müssen Überreste speziell behandelt werden | Punkt 96, „94-98 ja“ | 📋 |
| Gefangennahme und Verhör sind möglich | Punkte 97–98, „94-98 ja“; [Modul 13](13-Lebendfang-Fallen-und-Containment.md) | 📋 |
| Richtige Waffe, Munition und Kreaturenschwäche sind **Kern** der Jagd; kein Kugelschwamm | `M-PD` ‚Gunplay / weapons‘ | 📋 |

### 7.3 Falsche Information

Alte Texte können eine Schwäche falsch angeben (zum Beispiel ‚nur Feuer tötet es‘). Der Spieler testet sie; der Archiv-Eintrag wird dann auf ‚Disputed‘ oder ‚Disproven‘ gesetzt (`23#307A` Punkt 25, „25. genau“ in `23#308E`; `M-PD` ‚Hunter Archive epistemology‘). Beispielverlauf mit einem vermeintlichen Werwolf: Ein altes Buch behauptet Silber, ein Zeuge berichtet von Feuer; das Spiel verrät nicht, welche Aussage stimmt (`48#126A`, 💡 Beispiel zu einem bestätigten Prinzip).

---

## 8. Resistenzen und Immunität

`23#307A` Punkt 10 und `M-PD`:

- Nicht binär, sondern gestuft: vollständig immun, stark resistent, teilweise resistent, normale Wirkung, besonders empfindlich (intern).
- Zahlen bleiben intern. Der Spieler liest: ‚Scheint kaum Wirkung zu zeigen‘, nicht ‚73 % Resistenz‘ (Beispiel der Quelle).
- Ein Ausnahmeindividuum kann eine Schwäche **teilweise überwinden** (Modul 03 §7).

Beim Urwesen-Beispiel ([Modul 06](06-Daemonen-Engel-und-Leviathane.md)) wirken gewöhnliche Waffen **verletzend, aber nicht tötend**, bekannte Monsterwaffen mit auffallend geringer Wirkung; es gibt aber **keine unsichtbare Boss-Immunität** (`23#465A` Punkt 24, 💡; Gesamtzustimmung `23#468E`).

---

## 9. Endgültiges Töten und andere Lösungen

Die Quellen unterscheiden mehrere Ausgänge, die schon im Archiv-Fortschritt vorkommen: begegnet, identifiziert, erforscht, vertrieben, gebannt, gefangen, getötet, Fluch gebrochen (`M-PD` ‚Hunter Archive, collectibles and progress‘).

| Ausgang | Hinweis aus den Quellen |
|---|---|
| töten | bei vielen Arten nur mit richtiger Methode endgültig |
| vertreiben | das Wesen kann zurückkehren (Modul 08) |
| bannen | je nach Art, Ritual (Modul 12) |
| gefangen nehmen | [Modul 13](13-Lebendfang-Fallen-und-Containment.md) |
| Fluch brechen | Fall endet ohne Tod des Betroffenen ([Modul 05](05-Geister-Besessenheit-und-Fluecher.md)) |
| Person retten (Besessenheit) | Wirtskörper wird nicht getötet |
| Bindung zerstören | bei gebundenen Wesen |
| verschonen | „50 Spieler kann Wesen verschonen“, bestätigt als Teil von „45-54 passt“ (`23#1074A`, `23#1075E`) |

Zusätzlich gilt (`23#293A` Punkt 13): Monster müssen nicht bis zum Tod kämpfen; sie können fliehen, den Bau wechseln, das Opfer wechseln, sich verstecken, Verstärkung suchen oder später überraschen (Bestätigung „13. ja das sowieso“ in `23#294E`).

---

## 10. Übernatürliche Verletzungen beim Menschen

Die Quiz-Frage in `48#122A`: ‚Wie sollen sich Verletzungen durch übernatürliche Wesen von gewöhnlichen unterscheiden?‘ Die Optionen waren: A hauptsächlich Schaden; B individuelle Effekte; C mythologieabhängige Konsequenzen mit eigenen Verletzungen, Krankheiten, Flüchen und Behandlungsmethoden, die der Hunter erst verstehen muss; D vollständiges übernatürliches Medizinsystem. Der Entwickler wählte: ‚Mythologieabhängige Verletzungen, Flüche und Behandlungsmethoden‘ (`48#123E`, Option C).

Konsequenzen (Auslegung des Assistenten in `48#124A`, 💡 als Auslegung, 📋 als Wahl):

- Nicht jeder übernatürliche Angriff verursacht nur Lebenspunkteverlust.
- Der Spieler muss die richtige Behandlung gegebenenfalls **selbst herausfinden**; ein unbekannter Fluch wird nicht mit Questmarker und Heilmittel erklärt.
- Welche Effekte zu welcher Kreatur gehören, ist **nicht festgelegt**; die Tabelle des Assistenten (Werwolf, Vampir, Geist, Hexe, Dämon) ist ein Beispiel (💡). Die Quelle schreibt es selbst: ‚Das sind mögliche Mechaniken.‘

Weitere Quellen: Infektion und Verwandlung sind für manche Arten möglich, Gegenmittel oder Heilung können existieren, **ein Game Over soll es nicht geben, nur den Tod** (`23#1074A` Punkte 145–146; „144-146 genau, aber game over soll es nicht geben, nur den tod“ in `23#1075E`).

---

## 11. Übernatürliche Heilung

Der Entwickler bat darum, dass man „wie in der serie manchmal instant geheilt werden können, von einem engel oder anderem“ (`35#26E`). Der Assistent entwarf daraus (`35#27A`, 💡 Ausformulierung): eine **seltene Ausnahme** zum normalen Heilungsablauf, eine Hierarchie

```text
Medizin → übernatürliche Stabilisierung → Regeneration → Sofortheilung
        → vollständige körperliche Wiederherstellung → Wiederbelebung
```

mit drei Stufen der Heilung (Stabilisierung, beschleunigte Heilung, vollständige Sofortheilung) und Grenzen (alte Verletzungen, Flüche, bestimmte Gifte, dämonische oder celestiale Waffen, Seelenschäden, irreversible Veränderungen); dazu die Trennung **Verletzung heilen** (solange lebendig) und **Toten zurückbringen** (eine andere Machtklasse).

| Teil | Status |
|---|:---:|
| Der Wunsch nach seltener übernatürlicher Sofortheilung | 📋 (`35#26E`) |
| Dreistufiges Modell, Hierarchie, Fähigkeitsliste (`CanStabilize…CanResurrect`) | 💡 (`35#27A`; keine Bestätigung des Entwicklers in `35#28E`) |
| ‚Nicht jede Kreatur kann das‘ | 💡 (`35#27A`) |
| Wiederbelebung bleibt seltener und bedeutender als Heilung | 📋 Richtung (`23#458A` Punkt 15, „15. genau“ in `23#459E`) |

Die Fähigkeitsliste ist ein brauchbares **Datenfeld pro Wesen** (siehe Abschnitt 12), keine Entscheidung.

---

## 12. Entwurf: Wirkungsmodell

> 💡 **Vorschlag.** Er macht die beschlossene Trennung von physischem Schaden und übernatürlicher Wirkung prüfbar.

```text
AttackEvent
├── physical_effect       Gewebe, Knochen, Blutung ... (Menschen-Traumasystem)
└── supernatural_effects  [ {tag, source_material, trigger} ]

SpeciesRule
├── weakness[]    {tag, effect_kind, conditions, evidence_hints[]}
├── resistance[]  {tag, grade, conditions}
├── regeneration  {rate, tissues, bones, organs, limbs, resource, environment,
│                  suppressors[], hard_limits[]}
├── pain          {model, ignores_function: false}
└── kill_methods[] {id, preconditions, remains_handling, reliability_hint}

Beobachtung (im Spielstand, nicht in der Definition)
└── {species_or_individual_id, rule_id, observed_effect, certainty, source_event}
```

**Auswertungsreihenfolge (💡):** (1) physische Wirkung wie beim Menschen/Tier; (2) prüfe `supernatural_effects` gegen `weakness`/`resistance` des Individuums (Ausnahmemerkmale haben Vorrang); (3) wende Regenerationsregel an; (4) schreibe eine Beobachtung, die Spieler und Archiv lesen; (5) aktualisiere Individualzustand (Narben, Funktionsverlust).

---

## 13. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Spezialmaterial verursacht ohne passende Schwäche **keinen** Zusatzschaden, aber die physische Wirkung bleibt | `23#293A` Punkt 3 |
| Ein Wesen, das nicht mit normaler Munition tötbar ist, bleibt physisch verletzbar | `23#293A` Punkt 4 |
| Ausfall eines Beins verlangsamt ein physisches Wesen, außer die Art kompensiert | `23#307A` Punkt 12 |
| Eine Regenerationsunterdrückung stoppt Regeneration nur, wenn die Art sie als Wirkungsart hat | `23#307A` Punkte 9, 13 |
| Narben eines nicht regenerierenden Individuums sind nach Speichern und Laden und im nächsten Treffer vorhanden | `23#307A` Punkt 14 |
| Das Archiv zeigt keine Zahl für Resistenz und keine nicht beobachtete Schwäche | `23#307A` Punkt 10 |
| Ein Ausnahmemerkmal verändert die Wirkung einer bekannten Schwäche | `23#307A` Punkt 8 |

---

## 14. Sonderfälle

| Fall | Behandlung |
|---|---|
| Besessener Mensch wird verletzt | Schaden trifft den Wirtskörper; die Entität stirbt nicht dadurch ([Modul 05](05-Geister-Besessenheit-und-Fluecher.md)) |
| Geist wird physisch angegriffen | je nach Zustand wirkungslos, störend oder destabilisierend; Spezialmaterialien nach Art der Geistregel |
| Wesen ohne normale Anatomie | keine funktionale Verletzung; die Wirkung läuft über Bindung, Manifestation und Fähigkeiten |
| Tote Hülle eines Wesens | Behandlung der Überreste ist artspezifisch (Abschnitt 7.2) |
| Kind-Kreatur | Gore-Darstellung bei Kindern entfällt; „148 ja weil es auf xbox und ps vielleicht probleme gibt“ (`23#1075E`), Kinder-Kreaturen sind selten und nur bei ausgewählten Arten (siehe [Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §7) |

---

## 15. Was fehlt

- Die Wirkungsart jeder konkreten Schwäche (❓, an Artenentscheidung gebunden).
- Schwellen: ab wann gilt ein Wesen als ‚kaum wirkungsbeeinflusst‘, ab wann sieht der Spieler es (❓).
- Ob es eine Kategorie ‚tot, aber nicht endgültig‘ gibt, die das Archiv als eigenen Status führt (❓ Q-K04).
- Die Anatomie einzelner Arten (Bögen) und die Gore-Zerstörungssysteme pro Art.
