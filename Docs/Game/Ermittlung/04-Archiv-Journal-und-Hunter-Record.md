# Ermittlung 04: Archiv, Journal und Hunter Record

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die drei Wissens- und Fortschrittsebenen des Spiels und die Regeln, nach denen sie Wissen anzeigen. Menü- und Bildschirmentwürfe der Quellen sind Veranschaulichung (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Überblick: [Spielvision §8](../Spielvision.md#8-das-hunter-archiv). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Drei Ebenen](#1-drei-ebenen)
2. [Das Hunter Archive](#2-das-hunter-archive)
3. [Das Falljournal](#3-das-falljournal)
4. [Hunter Record: das Fortschrittsmenü](#4-hunter-record-das-fortschrittsmenü)
5. [Anzeige von Unbekanntem](#5-anzeige-von-unbekanntem)
6. [Wissensstatus und falsche Lore](#6-wissensstatus-und-falsche-lore)
7. [Sammelobjekte und Entdeckungen](#7-sammelobjekte-und-entdeckungen)
8. [Persistenz](#8-persistenz)
9. [Was fehlt](#9-was-fehlt)

---

## 1. Drei Ebenen

`23#55A` Abschnitt 13 trennt (💡 als Formulierung; die Struktur entspricht `M-PD`):

| Ebene | Frage | Inhalt |
|---|---|---|
| **Hunter Archive** | Was weiß ich? | wachsender Wissensgraph über Orte, Personen, Monster, Flüche, Rituale, Symbole, Artefakte, Waffen, Fälle, Dokumente, Medien |
| **Falljournal** (Case Journal) | Woran arbeite ich gerade? | angenommene Fälle, aktueller Ermittlungsstand, später der tatsächliche Lösungsweg |
| **Hunter Record / Fortschritt** | Was habe ich im gesamten Spiel erreicht? | Metaübersicht des Spielstands |

Der Assistent nannte diese Trennung „die sauberste Struktur“ (`23#55A`); der Entwickler äußerte keinen Einwand (`23#56E`). Die Namen „Chronicle“ und „Hunter Record“ stammen vom Assistenten (`23#55A`); `M-PD` kennt ‚Hunter Record menu‘.

---

## 2. Das Hunter Archive

### 2.1 Wissensgraph

📋 `M-PD` ‚Hunter Archive, collectibles and progress‘: zentraler Wissens-, Ermittlungs- und Entdeckungsgraph. Er verknüpft Orte, Regionen, relevante Straßen, Gebäude, Personen, Monster, Flüche, Rituale, Symbole, Artefakte, Waffen, Fälle, historische Fälle, Dokumente, Fotos, Audio und Video, Zeitungsartikel, Polizeiberichte, Zeugenaussagen, Ereignisse und Anomalien. Querverweise sind wichtig; der Spieler soll Zusammenhänge selbst erschließen. Block 9J ergänzt Beziehungen im Wissensgraph (Monster, Opfer, Ort, Artefakt, Ritual, Person; Frage 90 `23#1074A`, Antwort „90 ok“ `23#1075E`).

### 2.2 Wissen, nicht Debug-Ansicht

📋 `M-PD` ‚Hunter Archive epistemology‘: Das Archiv ist das **entdeckte** Wissen der Figur, kein ‚raw creature debugger‘. Nicht automatisch sichtbar sind interne Daten (KI-Gewichte, genaue Wahrnehmungsschwellen, verborgene Laufzeitzustände, Regenerationsgleichungen, Speicher-IDs, Spawn-Bedingungen, interne Wahrscheinlichkeiten).

### 2.3 Das Archiv beginnt mit Lücken

| Aussage | Status | Quelle |
|---|:---:|---|
| Das Archiv beginnt mit Lücken; es ist keine perfekte Monster-Wikipedia | 📋 | `23#1074A` Frage 84, Antwort `23#1075E` („am anfang weiß man noch nichts“); `23#1076A` |
| Wissen wächst aus Begegnungen, Büchern, Zeugen, Autopsien, Experimenten, alten Hunter-Unterlagen, Archiven, anderen Jägern und eigener Beobachtung | 📋 | `23#1074A` Frage 85, Antwort „85 ja genau“; `23#1076A` |
| Forschung soll nicht heißen „Wiki öffnen, Schwäche lesen“ | 💡 (Begründung des Assistenten) | `23#1076A` |
| Auch bei falscher Lore kann man „trotzdem viel wissen dadurch finden“ | 📋 | `23#1075E` Punkt 86 |
| Regionale Varianten und Mutationen derselben Art; bei Dämonen und Engeln eine Rangordnung | 📋 | `23#1075E` Punkt 87 |
| Der Spieler kann eigene Notizen ergänzen | 📋 | `23#1075E` Punkt 88 |
| Bilder, Audio und Video lassen sich als Belege direkt im Archiv verknüpfen | 📋 | `23#1075E` Punkt 89 („ja kann es“) |

### 2.4 Orte sammeln Wissen über die Zeit

📋 `M-PD`: Orte akkumulieren Information. Straßen werden nur dann Archiv-Einträge, wenn sie Geschichte, Fallbezug oder übernatürliche Relevanz haben. Die Kartenhierarchie führt von der Welt über Region oder County, Stadt, Bezirk und relevante Straße oder Ort bis zum Gebäude oder Komplex.

---

## 3. Das Falljournal

### 3.1 Aufnahme und Inhalt

| Aussage | Status | Quelle |
|---|:---:|---|
| Angenommene Fälle erscheinen im Journal („im questmenü oder so ähnlich“) | 📋 | `M-PD`; `23#52E` („wenn man die quest mal angenommen hat dann erscheint sie einem im questmenü“) |
| Das Journal dokumentiert nur, was der Spieler **tatsächlich herausgefunden hat** | 📋 | `23#55A`; `M-PD` |
| Journal und Archiv sagen nie, was aus den Fakten zu schließen ist (im Detektivmodus) | 📋 | siehe [Modul 03](03-Ermittlungsmodi-und-Hilfen.md#4-was-im-detektivmodus-erlaubt-bleibt) |

### 3.2 Der Spieler baut seine Ermittlung selbst auf

Der Assistent nannte das eine Kernmechanik (`23#55A`, 💡; die Selbstmarkierung entspricht den Entwickleraussagen `23#54E`):

| Menüpunkt (Beispiel) | Zweck |
|---|---|
| Beweise | Gefundenes anheften und verbinden (zum Beispiel Kratzspuren mit Zeugenaussage und Autopsiebericht) |
| Personen | als relevant markieren („Für Fall merken“) |
| Orte | als persönliches Ermittlungsziel markieren |
| Dokumente | gefundene Unterlagen |
| Zeitlinie | Ereignisse ordnen |
| Hypothesen | **eigene** Hypothese setzen oder aus bekanntem Monsterwissen wählen; das Spiel sagt **nicht**, ob sie stimmt |
| Eigene Notizen | Freitext |

Wichtig: Die Menüliste ist **kein beschlossener Aufbau**. Festgelegt ist, dass der Spieler selbst auswählt und verknüpft und das Spiel keine Bewertung abgibt (📋, `M-PD` ‚Player manually marks relevant clues/people/places, forms hypotheses‘).

### 3.3 Der tatsächliche Lösungsweg im Journal

📋 Der Entwickler (`23#60E` Punkt 1): „im journal soll später auch stehen welchen lösungsweg du genommen hast direkt über deine entscheidungen“. `M-PD`: Nach Abschluss hält das Journal den tatsächlichen Ermittlungsweg, Entscheidungen, Vorbereitung und Folgen fest.

Der Assistent entwarf einen **Case Record** als Beispiel (💡; Fallname, Ort und Figuren sind erfundene Beispiele und keine Lore): Ergebnis, „Dein Ermittlungsweg“ (befragt, ausgewertet, verzichtet auf), Schlussfolgerung, Vorbereitung (Ausrüstung), Entscheidungen, Auflösung, Folgen (`23#61A`, `23#63A`, `23#65A`). Gemeint ist nach dem Assistenten „fast zu einem persönlichen Tagebuch deiner gesamten Karriere“ (`23#61A`); er dokumentiert nur, was der Spieler getan oder erfahren hat.

### 3.4 Der vorgesehene Weg

Optional, nie als der einzig richtige Weg: siehe [Modul 03, Abschnitt 8](03-Ermittlungsmodi-und-Hilfen.md#8-referenzweg-nach-dem-fall).

---

## 4. Hunter Record: das Fortschrittsmenü

📋 Der Entwickler (`23#54E`): „es soll auch ein menü geben wo der komplette spielfortschritt dort steht, welche collectibles man schon hat, welche fehler, welche orte schon entdeckt sind, welche monster schon erledigt wurden“. `M-PD` führt es als ‚separate full progress/Hunter Record menu‘.

| Bereich | Inhalt laut Quellen |
|---|---|
| Story | Aktstand; **keine** zukünftigen Aktnamen, wenn diese Spoiler wären (💡 `23#55A`) |
| Fälle | abgeschlossen, aktiv, fehlgeschlagen, abgebrochen, ungeklärt, historische Fälle (💡 `23#55A`); `M-PD`: ‚failures/unresolved cases‘ |
| Fehlschläge | gehören ausdrücklich dazu; der Spielstand soll sie nicht vergessen (💡 `23#55A`, Beispiel: „2 Opfer konnten nicht gerettet werden“ als erfundenes Beispiel) |
| Monster | reicher als „getötet“: begegnet, identifiziert, erforscht, vertrieben, gebannt, gefangen, getötet, Fluch gebrochen (📋 `M-PD`, `23#55A`) |
| Orte, Sammelobjekte, Artefakte, Waffen, Rituale, Personen, Erkundung, Geheimnisse, Erfolge | aufgeführt als Menübereiche (📋 `M-PD`: ‚story, cases, failures/unresolved cases, monsters, places, collectibles, exploration, artifacts, weapons, rituals, people etc.‘) |

Das Menü zeigt, **was der Spieler kennt und erreicht hat**, ohne unbekannten Inhalt standardmäßig zu verraten (`23#55A` Abschnitt 4).

---

## 5. Anzeige von Unbekanntem

Der Entwickler präzisierte die Regel zweimal (`23#56E` Punkt 4 und `23#60E` Punkt 4):

1. Man zeigt „generell nur anzeigen was man weiß“, nicht einmal „weitere Geheimnisse“, sondern nur „Unbekannt“ (`23#56E`).
2. Kennt man eine **Kategorie** nicht, steht nicht einmal ihr Name da, „wirklich nichts“. Gibt es in einem Gebiet zehn Einträge und sind drei unbekannt, erscheinen diese drei als „Unbekannt“: „so viel weiß man schon man weiß nur nicht was fehlt“ (`23#60E`).
3. **Jedes Gebiet hat nicht dieselben Sammelobjekte**, damit man nicht erraten kann, was fehlt (`23#60E`).

Status: 📋 (`M-PD` ‚Unknown content does not reveal its category…‘, dort identisch). Der Assistent illustrierte das mit Gebieten wie einem Friedhof (Bekannte Fälle, Personen, Dokumente, „Unterirdische Bereiche: Unbekannt“) (💡, `23#59A`); die Qualität des Wissens soll **qualitativ** statt prozentual dargestellt werden (`M-PD`). Eine optionale **exakte Fortschrittsanzeige** ist später denkbar (`M-PD`), nicht beschlossen.

---

## 6. Wissensstatus und falsche Lore

📋 `M-PD` ‚Hunter Archive epistemology‘: Aussagen können **Confirmed**, **Suspected**, **Disputed**, **Disproven** oder **Historical/Unverified** sein. Jägerbücher, Familienwissen, Legenden und erfahrene Jäger können falsch, unvollständig oder veraltet sein; Beobachtung, moderne Tests, widersprüchliche Quellen und andere Jäger können das Archiv berichtigen. Ein erfahrener Begleiter kann sich plausibel irren.

`23#1074A` Frage 86 („Alte Hunter-Texte können teilweise falsch oder übertrieben sein“) beantwortete der Entwickler mit „ja das kann sein, aber man kann trotzdem viel wissen dadurch finden“ (`23#1075E`). Bedeutung für die Ermittlung: Eine Fehlmethode kann dazu führen, dass ein vermeintlich getöteter Gegner zurückkehrt (Frage 95; „94-98 ja“ `23#1075E`).

Das Archiv unterscheidet **Wissen über die Art** und **Wissen über bekannte Einzelwesen** (📋 `M-PD`, [Spielvision §9.3](../Spielvision.md#93-lebendige-wesen-statt-spawnpunkte)).

---

## 7. Sammelobjekte und Entdeckungen

Hier gibt es eine **Spannung zwischen Quellen**, die dokumentiert und nicht aufgelöst wird:

| Aussage | Quelle | Datum | Status |
|---|---|---|:---:|
| Das Basisspiel enthält immer die vollständigen etwa 1.200–1.500 physischen Sammelobjekte; kein Spieler wird gezwungen, sie zu sammeln; sie bleiben getrennt von Orten und Weltentdeckungen | `23#55A` (Assistent); `M-PD` | 18.08. / 31.08.2026 | 📋 als ‚Basisziel‘ (für die damalige Weltgröße) |
| „da die map so groß geworden ist, sollten es noch mehr collectibles geben“ | `23#1077E` (Entwickler) | 04.09.2026 | 📋 (Richtung: mehr) |
| Die 1.200–1.500 gehörten zum **alten, etwa 200-km²-Konzept**; für WORLD_700 und die größere Welt sind die Zahlen **offen** und müssen je Kategorie zugeordnet werden, nicht linear skaliert | `M-CA0905` Abschnitt 6 | 05.09.2026 | 📋 gilt als Korrektur |
| Gesamtzahl der verfolgten Einträge war früher etwa 5.000 | `M-CA0905` Abschnitt 6 | 05.09.2026 | 🕰️/❓ |

Folgerung: Die jüngste Aussage des Masters (05.09.) **überschreibt** die Zahl 1.200–1.500 als historisch für die größere Welt; die Entwickleraussage `23#1077E` verlangt zusätzlich „mehr“. Die tatsächliche Zahl ist **offen** (siehe [Modul 21](21-Konflikte-und-offene-Fragen.md), K-E1). Die [Spielvision](../Spielvision.md#8-das-hunter-archiv) führt das bereits als 🕰️/❓.

Sonstige Regeln (📋): Sammelobjekte sind optional; wichtige Inhalte für die Hauptgeschichte dürfen **nicht** auf Sammelobjekten beruhen („storykritisches Wissen“ so, dass niemand 1.300 Zettel sammeln muss, `23#55A`, 💡); Entdeckungen (Orte, Straßen, Gebäude, Höhlen, Gewässer, Geschichte) sind eine eigene Ebene, nicht alle physische Funde (`M-PD`).

---

## 8. Persistenz

Archiv, Journal und Record sind Teil des **persistenten Spielstands**. Die einzige heute gebaute Grundlage ist der Weltzustand (`WorldFacts` mit gestempeltem, ordnungstreuem Speicherformat). Es gibt **keine** Archiv-, Journal- oder Record-Datenstruktur. Das Speicherziel: 20–50 MB für einen langen Spielstand als Optimierungsziel, mit Vorrang für Zuverlässigkeit (`23#55A`, `23#54E`: „20-50mb sollten in ordnung sein“, 💡 als Ausformulierung). Die Reihenfolge der Prioritäten (Korrektheit, Zuverlässigkeit, keine verlorenen Zustände, Ladezeit, Absturzwiederherstellung, Speicherkompatibilität, Größe) ist ein Vorschlag des Assistenten (💡). Siehe [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md).

---

## 9. Was fehlt

- Eine **Datenstruktur** für Archiv, Journal und Record (nichts im Master, nichts im Code).
- Die **Menge** der Sammelobjekte und Entdeckungen für die größere Welt (offen).
- Die Gestaltung der **Beweisverknüpfung** und der Hypothesen-Eingabe.
- Das Verhältnis des **Guide-Dienstes** zum Archiv ([Spielvision §16](../Spielvision.md#16-begleitdienste-guide-plattform-und-profil)).
- Die Datenstruktur für **Mod-Beiträge** zum Archiv (Mod-Ebene, [Gameplay-Systeme §15](../Gameplay-Systeme.md#15-modding)).

Siehe [Modul 21](21-Konflikte-und-offene-Fragen.md).
