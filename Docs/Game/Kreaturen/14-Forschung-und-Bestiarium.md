# Kreaturen 14: Forschung und Bestiarium

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie der Spieler Wissen über Kreaturen gewinnt, prüft, notiert und im Hunter Archive ablegt, und was das Bestiarium zeigt. Das **allgemeine** Wissens- und Beweismodell steht in der Ermittlungs-Dokumentation ([Modul 04](../Ermittlung/04-Archiv-Journal-und-Hunter-Record.md), [Modul 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md)); hier steht, was für **Kreaturen** hinzukommt. Schwellen für Hypothese und Bestätigung sind im Master offen (`M-OD`). Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Wissensquellen](#2-wissensquellen)
3. [Der Eintrag einer Kreatur](#3-der-eintrag-einer-kreatur)
4. [Statuswörter](#4-statuswörter)
5. [Art und Individuum](#5-art-und-individuum)
6. [Forschungsmethoden](#6-forschungsmethoden)
7. [Spuren von Kreaturen](#7-spuren-von-kreaturen)
8. [Falsches Wissen und regionale Varianten](#8-falsches-wissen-und-regionale-varianten)
9. [Der Anfang des Spiels](#9-der-anfang-des-spiels)
10. [Begleiter, NPCs und Companion-App](#10-begleiter-npcs-und-companion-app)
11. [Fortschritt im Hunter Record](#11-fortschritt-im-hunter-record)
12. [Entwurf: Datenmodell des Wissens](#12-entwurf-datenmodell-des-wissens)
13. [Entwurf: Prüfregeln](#13-entwurf-prüfregeln)
14. [Sonderfälle](#14-sonderfälle)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Das Hunter Archive ist das **Wissen des Hunters**, kein Monster-Debugger; es zeigt, was der Charakter weiß | `23#295A`; `M-PD` ‚Hunter Archive epistemology‘ | 📋 |
| Interne technische Daten (KI-Gewichte, Wahrnehmungsschwellen, versteckte Zustände, genaue Regenerationsformeln, Speicher-IDs, Spawn-Regeln, interne Wahrscheinlichkeiten) werden nicht automatisch angezeigt | `23#295A`; `M-PD` | 📋 |
| Wissensbehauptungen können **bestätigt**, **vermutet**, **bestritten**, **widerlegt** oder **historisch/unbelegt** sein | `M-PD` ‚Hunter Archive epistemology‘ | 📋 |
| Hunter-Bücher, Familienwissen, Legenden und erfahrene Hunter können **falsch, unvollständig oder veraltet** sein; Beobachtung, moderne Tests, widersprüchliche Quellen und Behauptungen anderer Hunter aktualisieren das Archiv | `M-PD`; „C“ in `23#307A` (‚Hunter können falsche Überlieferungen weitergeben‘), „C: ja das ist gut so“ in `23#308E` | 📋 |
| Ein erfahrener Begleiter kann eine informierte, aber **falsche** Identifikation abgeben; NPC-Expertise ist keine Allwissenheit | `23#307A` Punkt 26, „26. passt gut“ in `23#308E`; `M-PD` | 📋 |
| Das Archiv kann Art und bekannte Individuen trennen | `23#307A` Punkt 30, „30. ja“ in `23#308E` | 📋 |
| Wissen über Monster entwickelt sich aus **Vorwissen, eigenständiger Forschung, persönlichen Notizen und auch möglicherweise falschen Informationen**; ein wachsendes Bestiarium im jeweiligen Spielstand | „Supernatural Monsterwissen: Vorwissen, Forschung, persönliche Notizen und auch möglicherweise falsche Informationen“ in `48#125E` | 📋 |
| **Umfassende, freie Monsterforschung:** alles Sinnvolle, was man sich als Hunter vorstellen kann (Recherche, Tatortarbeit, Labor, praktische Experimente, Untersuchung lebender oder toter Wesen, eigene Hypothesen, Konsequenzen) | „Supernatural Monsterforschung und Experimente: was man sich vorstellen kann“ in `48#127E`; Auslegung `48#128A` | 📋 |
| Nicht jede Untersuchung liefert automatisch die richtige Antwort | `48#128A` | 📋 / 💡 |
| Monsterforschung ist **systemisch**, nicht nur geskriptet | `48#128A` | 📋 / 💡 |
| Das Archiv beginnt mit **Lücken**; der Spieler startet fast ohne Monsterwissen und wird durch ein einschneidendes Ereignis mit einem Wesen in die Welt gezogen | „84 am anfang weiß man noch nichts, man wird in die welt reingeworfen durch ein ereignis mit einem monster, genau weiß ich es nicht aber ungefähr so“ in `23#1075E` | 📋 / 🧭 |
| Wissen wird evidenzbasiert erweitert (Begegnungen, Bücher, Zeugen, Autopsien, Experimente); alte Lore kann falsch sein, „man kann trotzdem viel wissen dadurch finden“; regionale Varianten und Mutationen; eigene Notizen; Bilder, Audio, Video verlinkbar; Wissensgraph | „85 ja genau“, „86 ja das kann sein…“, „87 ja genau…“, „88 ja sicher“, „89 ja kann es“, „90 ok“ in `23#1075E` | 📋 |
| Unbekannte Dinge verraten ihre Kategorie nicht (‚Unknown‘ statt Leerstellen mit Typ) | `M-PD` ‚Hunter Archive, collectibles and progress‘ | 📋 |
| Das Monsterstudium spiegelt das Tierstudium auf tieferer erkenntnistheoretischer Ebene (Hypothese, Wahrscheinlichkeit, Bestätigung, Opferanalyse, Proben, Fähigkeiten, Schwächen); intelligente Monster können sich anpassen, wenn sie gejagt werden | `M-PD` ‚Wildlife / study / harvesting / monsters‘ | 📋 |
| Schwellen für Hypothese/Bestätigung und die Bedienung bei falscher Klassifikation sind **offen** | `M-OD` ‚Exact monster hypothesis/confirmation thresholds and false-classification UX‘ | ❓ |

---

## 2. Wissensquellen

### 2.1 Quiz-Entscheidung

`48#124A` fragte: ‚Wie soll sich das Wissen des Hunters über Monster entwickeln?‘ Optionen: A vollständiges Bestiarium; B freischaltbares Bestiarium; C eigenständige Forschung; D Kombination aus Vorwissen, Forschung, Notizen, wachsendem Bestiarium und überprüfbaren, möglicherweise falschen Informationen. Der Entwickler wählte **D** (`48#125E`). Der Assistent führt aus (`48#126A`, 💡 Auslegung):

| Quelle | Inhalt |
|---|---|
| Vorwissen | der Hunter kennt bereits bestimmte Kreaturen, Legenden und Jagdmethoden |
| Eigenständige Forschung | Bücher, Archive, Zeugen, Tatorte, Experimente |
| Persönliche Notizen | eigene Theorien, Beobachtungen, Schlüsse |
| Unzuverlässige Quellen | nicht jede Legende, jeder Zeuge, jedes Dokument sagt die Wahrheit |
| Überprüfbare Erkenntnisse | weitere Ermittlung bestätigt oder widerlegt |
| Wachsendes Bestiarium | entwickelt sich mit dem Spielstand |

### 2.2 Das Bestiarium ist pro Spielstand

Der Wissensstand ist **Eigentum des Spielstands** (und im Koop der Person bzw. der Teamakte, siehe [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md)). Die Kreatur trägt kein Wissen über sich selbst.

### 2.3 Beispiel (💡)

`48#126A`: Ein altes Buch behauptet, Silber sei die einzige wirksame Waffe gegen einen vermeintlichen Werwolf. Ein Zeuge berichtet von einer Kreatur, die durch Feuer verletzt wurde. Das Spiel verrät nicht sofort, welche Aussage stimmt; der Spieler dokumentiert, forscht weiter und bereitet sich nach eigener Entscheidung vor.

---

## 3. Der Eintrag einer Kreatur

### 3.1 Sichtbare Ebene

`23#307A` Punkt 24 (💡 Zusammenstellung, „24. passt“ in `23#308E`) und `M-PD`: Der sichtbare Eintrag **kann** enthalten:

| Rubrik | Inhalt (nur soweit bekannt) |
|---|---|
| Identifikation | bekannte Bezeichnung, alternative Namen, Klassifikation, erste bekannte Begegnung, eigene erste Begegnung |
| Erscheinung | sichtbare Merkmale, Größe, bekannte Variationen, Fotos/Zeichnungen |
| Anatomie | nur soweit erforscht |
| Verhalten | Jagdverhalten, Aktivitätszeiten, soziale Struktur, Opfermuster, bekannte Taktiken |
| Fähigkeiten | nur bekannte |
| Schwächen | bestätigte/vermutete (mit Status) |
| Spuren | körperlich, chemisch, akustisch, übernatürlich, Opferverletzungen |
| Habitat | bekannte Regionen, typische Umgebung |
| Ursprung | bekannte Theorie, historische Quellen, widersprüchliche Versionen |
| Fälle | historische, aktuelle, eigene Begegnungen |
| Individuen | wiederkehrende, identifizierte Einzelwesen |
| Medien | Bilder, Audio, Video, Dokumente |
| Persönliche Notizen | eigene Ergänzungen |

Der Master nennt die Kategorien ebenso (‚identification, names, classification as known, appearance, anatomy as researched, behavior, hunting patterns, activity times, social structure, abilities, weaknesses, traces, habitat, origin theories, historical/current cases, known individuals, images/audio/video/documents and personal notes‘, `M-PD`).

### 3.2 Anzeigeform

Qualitativ, nicht prozentual (`M-PD`: ‚Default UI uses qualitative knowledge, not constant completion percentages‘). Beispiel der Quelle (`23#295A`): ‚Regeneration: Unbekannt. Schwäche: Silber. Ursprung: Unbekannt. Verhalten: Teilweise bekannt.‘ Die Feldmenge eines Eintrags darf **nicht verraten, was fehlt** (‚Unknown‘ ohne Kategorie, `M-PD`).

### 3.3 Interne Ebene

Nicht sichtbar: KI-Gewichte, Wahrnehmungsschwellen, interne Verletzungsparameter, Taktikwahrscheinlichkeiten, Regenerationsformeln, Hidden States, Runtime-Tags, Speicher-IDs, Spawn-Regeln (`23#295A`). Sie gehören dem Spiel und dem Entwicklerwerkzeug.

---

## 4. Statuswörter

`M-PD`: **Confirmed**, **Suspected**, **Disputed**, **Disproven**, **Historical/Unverified** (nach `23#307A` Punkt 24: ‚Confirmed / Suspected / Disproven‘; Beispiel Punkt 25: ‚Historical claim: Fire is lethal, Status: Disputed‘). Für Kreaturen gilt:

| Behauptung | Typischer Verlauf (💡) |
|---|---|
| Aus altem Buch (Historical/Unverified) | wird getestet → Confirmed oder Disproven oder Disputed |
| Aus Zeugenaussage | Suspected → Confirmed, wenn zweite Quelle oder Probe |
| Aus eigenem Experiment | Confirmed mit Bedingungen (‚nur bei diesem Individuum‘) |
| Von einem NPC-Hunter | Suspected, gekennzeichnet mit Quelle (`23#307A` Punkt 26) |
| Eine Behauptung betrifft ein Individuum | steht beim Individuum, nicht bei der Art |

Die detaillierte Statuslogik gilt gemeinsam mit [Ermittlung 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md).

---

## 5. Art und Individuum

`23#307A` Punkt 30 (💡 Struktur, 📋 Zustimmung):

```text
Creature Species
└── Known Individuals
    ├── Individual A
    ├── Individual B
    └── Unknown Individual
```

Der Spieler lernt: ‚Diese Art ist normalerweise X.‘ und ‚Dieses Individuum verhält sich anders.‘ Das ist die Grundlage für **Ausnahmeindividuen** ([Modul 03](03-Klassifikation-Herkunft-und-Individuen.md) §7) und für die Wiedererkennung (Narben, [Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §6).

---

## 6. Forschungsmethoden

### 6.1 Was erlaubt ist (Quiz-Entscheidung)

`48#126A` fragte: ‚Wie sollen Experimente und praktische Tests funktionieren?‘ (A nur Recherche, B praktische Tests, C wissenschaftliche Untersuchung, D umfassende freie Forschung). Der Entwickler: ‚Monsterforschung und Experimente: was man sich vorstellen kann‘ (`48#127E`), der Assistent legt es als Option D aus (`48#128A`, 💡 Auslegung):

| Bereich | Inhalt |
|---|---|
| Recherche | Bücher, alte Zeitungen, Polizeiberichte, Archive, Internet, Augenzeugen, Hunter-Journale |
| Tatortarbeit | Blutspuren, Gewebe, Haare, Fingerabdrücke, Rückstände |
| Laborforschung | Mikroskopie, chemische Analysen, DNA-Untersuchungen, Vergleiche mit bekannten Proben |
| Praktische Experimente | Waffen, Materialien, Substanzen, Fallen, Rituale auf Wirksamkeit testen |
| Kreaturen untersuchen | lebende oder tote Wesen, soweit Situation und Mythologie es erlauben |
| Eigene Hypothesen | Theorien formulieren, testen, vergleichen |
| Konsequenzen | Fehlversuche können gefährlich sein; erfolgreiche Forschung eröffnet Behandlungen und Jagdstrategien |

### 6.2 Beziehung zu Wissenschaft und Alt-Wissen

Moderne Messung und altes Wissen existieren nebeneinander (`23#359A` Punkte 23–24, „23. super“, „24. das ist cool“ in `23#360E`). Forschung kann alte Überlieferung verbessern („Das Buch behauptet Silber. Warum?“, Punkt 24); sie muss nichts wissenschaftlich erklären.

### 6.3 Labor, Proben, Sektion

Die Laborarbeit ist in [Ermittlung 11](../Ermittlung/11-Forensik-und-Labor.md) behandelt. Für Kreaturen gilt die Grenze: Forensik kann zeigen, dass **etwas physikalisch keinen Sinn ergibt** („83 … ja“, `23#1075E` für `23#1074A` Punkt 83). Eine Probe ist keine Aussage über die ganze Art (Modul 11 in Ermittlung: Aussagegrenzen).

---

## 7. Spuren von Kreaturen

`23#293A` Punkte 7–9 und `23#1074A` Punkte 76–83 (Antworten „76-83 ja“):

| Punkt | Aussage |
|---|---|
| 76 | **Normale Spuren zuerst**: Der Tatort zeigt nicht automatisch eine leuchtende Monsterspur |
| 77 | Körperliche Spuren: Blut, Haare, Fußspuren, Krallen, Bisse, Geruch, DNA-Anomalien, zerstörte Umgebung |
| 78 | Verhaltensmuster (Opferauswahl, Tatzeit, Ort) sind so wichtig wie physische Beweise |
| 79 | Historische Recherche: alte Fälle können ein Muster über Jahrzehnte oder Jahrhunderte zeigen |
| 80 | Wetter zerstört Beweise (Regen, Schnee, Verkehr, Zeit) |
| 81 | Hund als Tracking-Companion: folgt Gerüchen und Spuren, nicht allwissend |
| 82 | Falsche Beweise: Monster oder Menschen legen absichtlich falsche Spuren |
| 83 | Normale Forensik kann manchmal zeigen, dass etwas physikalisch keinen Sinn ergibt |

Dazu aus `23#293A`: Spuren hängen von der Art ab (Fuß-/Klauenspuren, Haare, Hautreste, Blut, Geruch, Schleim, Temperaturveränderungen, elektromagnetische Störungen, beschädigte Umgebung, Verletzungsmuster, Tierverhalten, Audioaufnahmen, Kameraartefakte, Zeugenberichte, chemische oder übernatürliche Rückstände), aber **nie** ‚dieselben drei Hinweisarten für jedes Monster‘ (Punkt 7, „7. gut“ in `23#294E`); Spuren sind **mehrdeutig** (Punkt 8, „8. super“): Tiefe Schnittwunden passen zu mehreren Wesen; mehrere Hinweise grenzen ein; falsche Identifikation darf passieren, aber das Spiel bestraft nicht künstlich (Punkt 9, „9. ja genau“).

Der **Hund** als Begleiter bei Fällen ist erwünscht („sehr wichtig, hund kann begleiter sein bei fällen“, `23#1027E` Punkt 22); die Hund-Mechanik ist in [Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md) behandelt.

---

## 8. Falsches Wissen und regionale Varianten

| Aussage | Quelle | Status |
|---|---|:---:|
| Alte Hunter-Texte können falsch oder übertrieben sein | `23#1074A` Punkt 86; „86 ja das kann sein, aber man kann trotzdem viel wissen dadurch finden“ in `23#1075E` | 📋 |
| Derselbe Monstertyp kann **regionale Unterschiede** und Mutationen haben | „87 ja genau und das gleiche monster hat vielleicht noch mutationen“ in `23#1075E` | 📋 |
| Falsche Lore kann zu falscher Vorbereitung und gefährlichen Fehlern führen | `23#293A` Punkt 9; `23#1074A` Punkt 95 | 📋 |
| Nicht-absolutes Wissen innerhalb der Spielwelt | „C“ in `23#307A`, „C: ja das ist gut so“ in `23#308E` | 📋 |
| Konkurrierende Modelle der Kosmologie, ein Rest Mysterium | `23#359A` Punkt 6 | 📋 |
| Beispiel für falsche Überlieferung: ‚Nur Feuer kann es töten.‘ → später ‚Disputed‘ | `23#307A` Punkt 25 (💡 Beispiel) | 💡 |

Regionale Varianten betreffen auch Gesetze und Wissen der Bevölkerung ([Welt](../World/README.md), [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md)).

---

## 9. Der Anfang des Spiels

`23#1074A` Punkt 84 fragte, ob das Archiv mit Lücken beginnt; der Entwickler: ‚am anfang weiß man noch nichts, man wird in die welt reingeworfen durch ein ereignis mit einem monster‘ (`23#1075E`). Der Assistent hält die Richtung fest (`23#1076A`, 💡): Der Spieler startet **fast ohne echtes Monsterwissen**; ein einschneidendes Ereignis zieht ihn in die Welt; Charakter und Spieler lernen gemeinsam; das Archiv ist nicht ‚die perfekte Monster-Wikipedia‘; Wissen entsteht aus Büchern, alten Hunter-Unterlagen, Zeugen, Autopsien, Experimenten, Begegnungen, Archiven, anderen Hunters und eigener Beobachtung. Damit wird Recherche nicht zu ‚Wiki öffnen, Schwäche lesen‘. Der Entwickler sagte „genau weiß ich es nicht aber ungefähr so“: die Einstiegsstory ist 🧭, nicht festgelegt.

---

## 10. Begleiter, NPCs und Companion-App

- Ein Begleiter darf irren („Ich glaube, das ist X“) und ist umso besser, je erfahrener er ist, aber nie garantiert (`23#307A` Punkt 26, 📋).
- Die Companion-KI darf **unbestätigte Informationen** nicht als gesicherte Wahrheit präsentieren, wenn sie den spoilerfreien Spielstand begleitet (`48#126A`, 💡 Folgerung; Plattform: [Spielvision](../Spielvision.md), Begleitdienste).
- Hunter-NPCs liefern Wissen mit Status ‚Suspected‘ oder ‚Historical‘ (Modul 11).

---

## 11. Fortschritt im Hunter Record

`M-PD` ‚Hunter Archive, collectibles and progress‘: Der Monsterfortschritt ist reicher als ‚getötet‘: **begegnet, identifiziert, erforscht, vertrieben, gebannt, gefangen, getötet, Fluch gebrochen**. Hinzu kommen Fälle (auch gescheiterte und ungelöste), Orte, Artefakte, Waffen, Rituale, Personen. Welche Status ein Individuum und welche eine Art trägt, ist nicht festgelegt (❓ Q-K04; [Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §15).

---

## 12. Entwurf: Datenmodell des Wissens

> 💡 **Vorschlag.** Er macht die beschlossene Wissenslogik für Kreaturen prüfbar.

```text
KnowledgeClaim (im Spielstand)
├── claim_id, subject {species_id | individual_id | artifact_id | place_id}
├── field            weakness | regeneration | origin | behavior | ability | habitat | ...
├── content          strukturierter Inhalt (oder Text)
├── status           confirmed | suspected | disputed | disproven | historical_unverified
├── sources[]        {kind: book|witness|npc_hunter|lab|experiment|observation, ref, reliability_hint}
├── conditions       z. B. "nur bei diesem Individuum", "nur nachts"
├── first_seen_at, last_updated_at
└── player_notes[]

ArchiveEntryView (abgeleitet)
└── zeigt je Feld den stärksten bestätigten Claim, sonst "Unbekannt",
    zusätzlich Widersprüche (disputed), nie interne Daten
```

**Quellenbezug:** Statuswörter `M-PD`; Quellenarten `23#307A` Punkt 24; Bedingungen (Individuum, Ausnahmen) `23#307A` Punkt 8.

---

## 13. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Der Eintrag zeigt kein Feld, für das kein Claim existiert, mit seinem Inhalt | `23#295A`; `M-PD` |
| Interne Daten (Gewichte, Formeln, Spawn-Regeln) erscheinen in keiner Spieler-UI | `23#295A` |
| Eine bestätigte Schwäche gilt nur mit den Bedingungen, unter denen sie bestätigt wurde | `23#307A` Punkt 8 |
| Ein Experiment kann ein falsches Ergebnis liefern, wenn der Aufbau unsauber ist | `48#128A` ‚Nicht jede Untersuchung liefert automatisch die richtige Antwort‘ |
| Ein alter Text kann zu ‚Disproven‘ werden, ohne dass der Text verschwindet | `23#307A` Punkt 25 |
| Der Begleiter nennt keine Schwäche als Tatsache, die nur ‚Suspected‘ ist | `48#126A` |
| Kategorie unbekannter Inhalte bleibt verborgen | `M-PD` |
| Wissen wird nicht zwischen Spielständen übertragen | `48#108A` (Host-Welt; [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md)) |

---

## 14. Sonderfälle

| Fall | Behandlung |
|---|---|
| Der Spieler besitzt Wissen aus dem Online-/Koop-Raum eines Freundes | Wissen ist profilbezogen; Übertragung nicht entschieden ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md)) |
| Ein Individuum widerspricht der Art-Aussage | Eintrag am Individuum; Art-Aussage bleibt (L12) |
| Zwei Quellen widersprechen sich, beide ‚Historical‘ | zeigt beide mit ‚Disputed‘ |
| Der Spieler schreibt eine falsche Notiz | Notiz bleibt als Notiz; Claim wird nicht bestätigt |
| Eine Kreatur wird erst durch ein Ereignis in der Welt bekannt | Archiv-Eintrag entsteht mit Status ‚Suspected‘ |

---

## 15. Was fehlt

- Schwellen für Hypothese und Bestätigung; Bedienung bei falscher Klassifikation (❓, `M-OD`).
- Die Rubrikenliste ist eine Zusammenstellung (💡); die endgültige Eintragsstruktur fehlt.
- Wie viele Quellen für ‚Confirmed‘ nötig sind (❓ Q-K21).
- Die Einstiegsgeschichte (🧭 `23#1075E` Punkt 84).
- Wie persönliche Notizen im Koop geteilt werden (❓, [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) Q-E64).
