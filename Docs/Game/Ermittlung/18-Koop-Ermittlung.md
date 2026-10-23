# Ermittlung 18: Ermittlung im Koop

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Ermittlung mit bis zu vier Spielern funktioniert: wer was weiß, wer entscheidet, wie Beweise, Hypothesen und rechtliche Folgen verteilt sind. Beschlossen sind die Grundregeln des Story-Koops (Host-Welt, eigener Charakter pro Spielstand, Entscheidungsarten, Zeitentscheidungen). Alles Weitere ist **Entwurf** (💡). Netzwerkarchitektur und Replikation sind laut `CLAUDE.md` §6 **offen** und kein Gegenstand dieses Moduls. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Klärung: gemeinsamer Fortschritt gegen Host-Welt](#2-klärung-gemeinsamer-fortschritt-gegen-host-welt)
3. [Entwurf: Was geteilt und was persönlich ist](#3-entwurf-was-geteilt-und-was-persönlich-ist)
4. [Entwurf: Entscheidungen im Team](#4-entwurf-entscheidungen-im-team)
5. [Entwurf: Aufgabenteilung bei der Ermittlung](#5-entwurf-aufgabenteilung-bei-der-ermittlung)
6. [Entwurf: Hypothesen und Meinungsverschiedenheit](#6-entwurf-hypothesen-und-meinungsverschiedenheit)
7. [Entwurf: Beweise, Inventar und Beweiskette](#7-entwurf-beweise-inventar-und-beweiskette)
8. [Entwurf: Rechtliche Folgen im Koop](#8-entwurf-rechtliche-folgen-im-koop)
9. [Entwurf: Zeit im Koop](#9-entwurf-zeit-im-koop)
10. [Entwurf: Beitritt, Austritt und Unterbrechung](#10-entwurf-beitritt-austritt-und-unterbrechung)
11. [Hilfen und Einstellungen im Koop](#11-hilfen-und-einstellungen-im-koop)
12. [Sonderfälle](#12-sonderfälle)
13. [Abhängigkeiten](#13-abhängigkeiten)
14. [Ergänzungen aus der Quellenauswertung (Koop und Online)](#14-ergänzungen-aus-der-quellenauswertung-koop-und-online)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| C1 | Story-Koop mit bis zu vier Spielern | 📋 Richtung | `CLAUDE.md` §6; `M-MS1013` |
| C2 | Entscheidungen im Story-Koop sind **situationsabhängig**: persönliche entscheidet jeder selbst, gemeinsame Teamentscheidungen werden abgestimmt | 📋 | `48#97E` (Option C der Frage `48#96A`) |
| C3 | Bei Gleichstand gelten situationsabhängige Regeln (Option D) | 📋 | `48#99E` |
| C4 | Zeitkritische Entscheidungen: anpassbare Echtzeit-, Zeitlupen- und Abstimmungsregeln | 📋 | `48#101E` |
| C5 | Gemeinsamer Missionsfortschritt „für alle“ (Option B der Frage `48#102A`) | 📋, **später durch C6/C7 eingeschränkt** (Abschnitt 2) | `48#103E` |
| C6 | Es ist grundsätzlich die **Welt des Hosts**; alles, was auf dem Spielstand des Hosts passiert, bleibt dort; Mitspieler können wechseln; der Fortschritt ist an den **Spielstand gebunden** | 📋 | `48#105E` |
| C7 | Was der Gast auf seinem eigenen Spielstand hat, bleibt dort; spielt er bei einem Freund, hat er nur, was er auf **dessen** Spielstand hat | 📋 | `48#107E` |
| C8 | Jeder Koop-Spieler erstellt seinen Hunter über die Charaktererstellung | 📋 | `48#109E` |
| C9 | Im Koop zeigt die Companion-App **gemeinsame Ermittlungsakten, Beweise, Kartenmarkierungen und Teamfortschritt** (Option C „Gemeinsames Hunter-Team“) | 📋 | `48#91E` zu `48#90A` |
| C10 | Das gemeinsame Journal löst unterschiedliche Meinungen nicht automatisch auf (Spieler dürfen verschiedene Theorien haben) | 💡 des Assistenten | `48#98A` |
| C11 | Krankenhaus im Koop: Ausfall nur wenige Minuten bis ca. 15 Minuten; überspringbar; klassischer Modus einstellbar | 📋 | `48#117E`, `48#119E` |
| C12 | Detailliertes Rettungssystem, vereinfachte Wiederbelebung als Option | 📋 | `48#121E` |
| C13 | Individuelle Verhaftung; die Welt läuft weiter; Welt-Zeitsprung mit Zustimmung aller | 📋 | `48#161E`, `48#163E` |
| C14 | Koop-Gefängnis-Gameplay: Plan C, D langfristig | 📋 | `48#165E` |
| C15 | Freie Fallen, Schutzvorrichtungen und Einrichtungen können gemeinsam vorbereitet werden | 💡 des Assistenten | `48#132A` |
| C16 | Die Host-Welt ist maßgeblich; Gastfortschritt ist auf den Host-Spielstand beschränkt | 📋 | `CLAUDE.md` §6 (‚host-save-authoritative progress and guest progress scoped to the host save‘) |

---

## 2. Klärung: gemeinsamer Fortschritt gegen Host-Welt

Die Quelle enthält zwei scheinbar gegensätzliche Aussagen:

| Aussage | Stelle |
|---|---|
| Option B der Frage `48#102A`: „Gemeinsamer Fortschritt: Abgeschlossene Missionen werden grundsätzlich für alle beteiligten Spieler übernommen.“ Gewählt mit `48#103E` | 10.10.2026, 19:40 |
| `48#105E`: Es ist die Welt des Hosts; was auf dem Spielstand des Hosts passiert, bleibt dort; „es können sich die leute immer ändern aber der fortschritt ist auf dem spielstand gebunden“. `48#107E`: Gäste behalten, was sie auf ihrem Spielstand haben | 10.10.2026, 19:43 und 19:51 |

**Auslegung:** Die spätere Aussage (`48#105E`, `48#107E`) **präzisiert und überschreibt** die frühere, weil der Entwickler selbst schreibt, wie es gemeint war. Der Assistent hat das ebenfalls so gelesen: „Verbindlich: Host-basierter Story-Koop“ (`48#106A`) und ‚Jeder Spielstand ist vollständig unabhängig. … Keine Übertragung zwischen Spielständen. Keine automatische Synchronisierung. Kein Überschreiben fremder Welten.‘ (`48#108A`, bestätigt durch `48#109E`). Auch `CLAUDE.md` §6 hält die gleiche Linie fest.

Die Option B (`48#103E`) wirkt damit **nur innerhalb eines Host-Spielstands**: Der Missionsfortschritt gilt für alle Spieler, die in dieser Welt spielen. Eine **Übertragung** auf den eigenen Spielstand des Gastes findet nicht statt. Was das für Belohnungen und Erfahrung des Gastes heißt, ist nicht eindeutig („Persönlicher Fortschritt von Gästen: Noch zu definieren“, `48#106A`; später entschieden durch `48#107E` im Sinne der Trennung). Eintrag in [Modul 21](21-Konflikte-und-offene-Fragen.md) als K-E9.

---

## 3. Entwurf: Was geteilt und was persönlich ist

> [!NOTE]
> **Nur Vorschlag (💡).** Ableitung aus C2, C6–C10. Es gibt keinen Beschluss, welche Ermittlungsdaten geteilt sind.

| Datenart | Geteilt (Host-Welt/Team) | Persönlich (Charakter) |
|---|:---:|:---:|
| Fallwahrheit | ✔ (Welt) | – |
| Fallzustand (aktiv, gelöst usw.) | ✔ | – |
| Wissensstand zum Fall | **Teamakte** (gemeinsam zusammengetragen) | **persönlich** (was der Charakter selbst erfahren oder notiert hat) |
| Notizen, Hypothesen | beide möglich | beide möglich |
| Persönliche Marker | – | ✔ |
| Gemeinsame Marker/Karte | ✔ | – |
| Beweise (Objekte) | in der Welt oder beim Besitzer | im Inventar des Charakters |
| Archivwissen über Monster | gemeinsame Sicht (Hunter-Archiv der Gruppe) oder persönlich | – (offen) |
| Ruf, Beziehungen, Verdacht | pro Charakter (Beobachter hat Überzeugung pro Person) | ✔ |
| Fahndung, Akte, Vorstrafen | – | ✔ (pro Charakter) |
| Einstellungen/Hilfen | – | ✔ (Profil der Person) |

Der Vorschlag: **Teamakte** und **persönliche Akte** laufen nebeneinander. Die Companion-Option C (`48#91E`) entspricht der Teamakte. Ob Wissen **automatisch** in die Teamakte fließt oder der Spieler es aktiv teilt, ist offen (❓ Q-E64). Die Regel ‚Spielerwissen entsteht durch Handlung‘ (Modul 09 §4.4) gilt für jeden Charakter: Er kennt, was er erfahren oder was ihm mitgeteilt wurde (Gespräch, geteilte Akte).

---

## 4. Entwurf: Entscheidungen im Team

### 4.1 Beschlossene Grundlage

Persönliche Dialoge, Beziehungen und moralische Entscheidungen entscheidet der jeweilige Spieler; gemeinsame Ermittlungsentscheidungen, Entscheidungen über das Schicksal eines gemeinsamen Falls und wichtige Storyentscheidungen werden per Teamabstimmung getroffen (Tabelle `48#98A`, 💡 Ausformulierung der Wahl `48#97E`).

### 4.2 Entwurf: Welche Entscheidung ist welcher Art?

| Entscheidung | Art (Vorschlag) |
|---|---|
| Was sage ich einem Zeugen? | persönlich |
| Wie verhalte ich mich im Verhör? | persönlich |
| Welche Hypothese halte ich fest? | persönlich (Teamhypothese: gemeinsam) |
| Fall abschließen, indem das Wesen getötet wird? | gemeinsam |
| Wen verfolgen wir? Was tun wir mit einem Gefangenen? | gemeinsam |
| Polizei informieren oder nicht? | gemeinsam, wenn es alle betrifft; sonst persönlich |
| Geheimhaltung oder Offenlegung? | gemeinsam (große Wirkung), persönlich (kleine Wirkung) |
| Haftzeit überspringen? | gemeinsam (Weltzeit), persönlich (Warteanimation) |
| Wem helfen wir zuerst (Rettung)? | gemeinsam, zeitkritisch |

Die Klassifikation ist **ein Vorschlag**, den Fallautoren pro Entscheidungspunkt setzen müssten (❓ Q-E65).

### 4.3 Gleichstand

Die Optionen der Frage `48#98A` waren: A erneut diskutieren und abstimmen, B Host-Stichentscheid, C Fallleiter entscheidet, D situationsabhängig. Gewählt: **D** (`48#99E`). Beispiele des Assistenten (`48#100A`, ausdrücklich „zunächst Vorschläge, keine endgültig festgelegten Regeln“): bei moralischen Entscheidungen erneute Diskussion; bei zeitkritischen Situationen entscheidet der gerade handelnde Spieler.

| Art der Entscheidung | Gleichstand-Regel (Beispiele aus der Quelle, unverbindlich) |
|---|---|
| moralisch, Zeit vorhanden | erneut diskutieren und abstimmen |
| zeitkritisch | der gerade handelnde Spieler entscheidet |
| Fallführung | Fallleiter (Option C der Frage, nicht gewählt) |
| Rest | offen |

### 4.4 Zeit

Die Entscheidungsmodi sind **Echtzeit**, **Zeitlupe**, **Abstimmungsphase**, situationsabhängig verfügbar, vom Host konfigurierbar (`48#101E`, Ausformulierung `48#102A`). Für Ermittlung: Gespräche und Fallentscheidungen sind meist nicht zeitkritisch; Verfolgungen, Verhöre unter Druck und Rettungen sind es.

---

## 5. Entwurf: Aufgabenteilung bei der Ermittlung

Die Companion-Beschreibung (`48#92A`, 💡) nennt: „Einer untersucht einen Tatort, ein anderer recherchiert Monsterwissen, und beide können ihre Erkenntnisse in derselben Companion-Fallakte zusammentragen.“ Das ist ein Beispiel für **Parallelarbeit**. Ein Entwurf möglicher Rollen (nicht festgelegt):

| Rolle (Arbeitsname) | Tätigkeit | Wissensfluss |
|---|---|---|
| **Tatort** | Spuren sichern, Zeugen befragen | Beweise und Aussagen → Teamakte |
| **Recherche** | Archiv, Bibliothek, Internet, Hunter-Netz | Dokumente → Teamakte |
| **Labor/Medizin** | Proben, Autopsie, Behandlung | Befunde → Teamakte |
| **Verbindung** | Polizei, Informanten, Anwalt, Hunter-Kontakte | Informationen → Teamakte |
| **Fahrer/Logistik** | Transport, Fahrzeuge, Versorgung | – |
| **Schutz** | Sicherung, Wache, Kampfbereitschaft | – |

Es gibt **keine** festgelegten Klassen (kein Klassenzwang, `M-CA0905`: ‚do not enforce artificial class lockouts‘). Die Rollen sind Beschreibungen dessen, was Spieler tun.

Begleiter-NPCs haben unterschiedliche Kompetenzen (Kampf, Recherche, Medizin, Technik, Tracking, `M-CA0905`); im Koop sind menschliche Mitspieler an deren Stelle.

---

## 6. Entwurf: Hypothesen und Meinungsverschiedenheit

Beschlossen (💡, `48#98A`): Die Spieler dürfen unterschiedliche Meinungen und Theorien haben. Das gemeinsame Journal soll diese Unterschiede **nicht automatisch auflösen**.

| Regel (Entwurf) | Erläuterung |
|---|---|
| **Mehrere Hypothesen nebeneinander** | Jeder Charakter hat eigene; die Teamakte darf mehrere führen |
| **Keine Mehrheitsvorgabe der Wahrheit** | Mehrheit bestimmt das Handeln, nicht die Wahrheit; die Fallwahrheit bleibt unabhängig |
| **Handeln statt Notieren** | Die Teamabstimmung wird nur bei **Handlungen** nötig, die die Gruppe binden (Fall abschließen, Wesen töten, Gefangenen freilassen) |
| **Minderheitsvotum** | Wer überstimmt wird, kann dennoch eigene Notizen führen; Journal führt den Verlauf (wer stimmte wie), soweit der Detektivmodus es erlaubt |
| **Vertrauen im Team** | Kooperations- und Konfliktverhalten wirkt auf Beziehungen der Charaktere (Beziehungsmodell der Crew; offen) |

---

## 7. Entwurf: Beweise, Inventar und Beweiskette

| Frage | Vorschlag |
|---|---|
| **Wer besitzt Beweise?** | Der Charakter, der sie hält; Weitergabe ist ein Ereignis (Übergabe) mit Beweiskette (Modul 10 §9) |
| **Gemeinsames Lager** | HQ-Lager und Fahrzeug sind gemeinsame Orte (Hunter-HQ, `M-CA0905`); Beweise dort gehören dem Team |
| **Gleichzeitige Spuren** | Zwei Charaktere sichern dieselbe Spur: ein Eintrag, zwei Finder; Doppelsicherung ist keine Verdopplung |
| **Verderb im Rucksack eines Offline-Spielers** | Spielzeit läuft nur, wenn die Welt läuft (Host) |
| **Zerstörte Beweise** | Wer Beweise zerstört (Absicht oder Unfall), trägt rechtliche Folgen (Modul 14/15) |
| **Beweise gegen Teammitglieder** | möglich, da Charaktere eigene Akten haben (Abschnitt 8) |

Die Frage, wie Gegenstände im Koop **repliziert** werden, gehört zur Netzwerkarchitektur und ist hier nicht entschieden.

---

## 8. Entwurf: Rechtliche Folgen im Koop

| Aussage | Status |
|---|:---:|
| Nur der verhaftete Hunter wird eingesperrt; die anderen spielen weiter | 📋 (`48#161E`) |
| Andere Hunter können Beweise sammeln, einen Anwalt organisieren, eine Befreiung versuchen | 📋 Plan (`48#165E` C; `48#162A`) |
| Fahndung, Akten und Vorstrafen sind **pro Charakter** | 💡 (Folgerung aus C8/C16) |

Fragen, die der Koop an das Rechtssystem stellt (alle ❓ Q-E66):

| Frage | Optionen (Entwurf) |
|---|---|
| **Komplizenschaft** | Wenn zwei Charaktere gemeinsam eine Tat begehen: gemeinsame Akte mit mehreren Verdächtigen |
| **Aussage gegen Mitspieler** | Darf ein inhaftierter Charakter einen anderen belasten (Absprache, Modul 15 §6)? Ein Spielerentscheid gegen den anderen Spieler ist riskant für das Spielerlebnis |
| **Einzelne Fahndung, gemeinsame Verfolgung** | Polizei verfolgt ein Fahrzeug mit mehreren Insassen |
| **Reputation im Team** | Ein Charakter mit hoher Fahndung belastet alle bei gemeinsamen Auftritten |
| **Schutz der Mitspieler** | Spielerwunsch ‚PvP-Schutz‘ ist nicht in den Quellen |
| **Befreiung** | Mitspieler brechen den Kollegen aus (Modul 16 §10) |

---

## 9. Entwurf: Zeit im Koop

| Aspekt | Regel | Quelle |
|---|---|---|
| Welt-Zeitsprung (Haft, Wartezeit) | nur mit Zustimmung aller aktiven Spieler | 📋 `48#163E` |
| Individueller Skip der Warteanimation | davon zu unterscheiden | 💡 `48#164A` |
| Krankenhaus | überspringbar; wenige Minuten bis ca. 15 Minuten | 📋 `48#117E`, `48#119E` |
| Zeit läuft beschleunigt in Krankenhaus und für die Heilung | 📋 | `48#117E` |
| Nebenmissionen verfallen nicht durch den Sprung | 📋 | `48#161E` |
| Gemeinsame Weltzeit | eine Zeit pro Host-Welt (zwei Weltzeiten gleichzeitig sind ausgeschlossen) | 💡 `48#162A` |
| Eskalationsuhren | laufen auf der Host-Weltzeit | Entwurf (Modul 09 §7) |

---

## 10. Entwurf: Beitritt, Austritt und Unterbrechung

| Situation | Vorschlag |
|---|---|
| **Spieler tritt mitten im Fall bei** | Er erhält die Teamakte (soweit der Host teilt) und spielt mit seinem Hunter im Host-Spielstand (C8). Wissen, das er aus seinem eigenen Spielstand hat, wird **nicht** übernommen (C7). |
| **Spieler verlässt das Spiel** | Sein Charakter verlässt die Szene (offline); Fälle, die ihn betreffen, laufen weiter (Fahndung, Haft) |
| **Host beendet die Sitzung** | Weltzustand wird gespeichert; Zeit steht |
| **Verbindungsabbruch im Verhör** | Der Verhörzustand wird im Host-Weltzustand gehalten; Rückkehr möglich (offen) |
| **Alleinspiel mit Host-Welt** | Alle Fälle bleiben möglich; Teamabstimmungen entfallen |
| **Neuer Mitspieler spät** | Mitspieler kennt nichts vom bisherigen Fall; Teamakte kann ihn einarbeiten (Zeitaufwand) |

Technische Fragen wie Replikation, Autorität über Beweise, Rechte am Weltzustand sind **bewusst offen** (`CLAUDE.md` §6; Kommentar im Kernmodul `WorldFacts`: ‚The store makes no assumption about who is allowed to issue commands (co-op authority is OPEN)‘).

---

## 11. Hilfen und Einstellungen im Koop

| Frage | Vorschlag |
|---|---|
| Hat jeder Spieler eigene Hilfen? | Ja, Einstellungen sind ein **Profil der Person** ([Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md)), nicht der Welt |
| Was sieht ein Spieler mit „Detektiv“, wenn der andere „Geführt“ nutzt? | Die Hilfe ist Darstellung (R4), sie wirkt nur beim betreffenden Spieler; die Teamakte zeigt nur, was geteilt wurde |
| Darf der Host Hilfen sperren? | Offen; Konfigurierbarkeit durch den Host ist bei Entscheidungsmodi beschlossen (`48#101E`), bei Hilfen nicht |
| Bot und Companion | Jedes Gerät hat eine eigene Sicht (`48#89E`: Mehrere Geräte mit individuellen Ansichten) |
| Spoiler | Companion-Spoilerstufe pro Person (Modul 08) |

---

## 12. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Zwei Spieler geben widersprüchliche Aussagen | derselbe Zeuge hört beide | Zeuge merkt Widerspruch; Misstrauen steigt |
| Spieler A verrät Spieler B (Absprache, Aussage) | – | rechtlich möglich (Abschnitt 8), spielerisch heikel; keine Regel in den Quellen |
| Spieler vernichtet Beweis ohne Absprache | – | Folgen für alle, wenn Fall betroffen |
| Ein Spieler stirbt, der andere hat den Beweis | – | Beweis bleibt im Inventar des Verstorbenen (Weltgegenstand) oder wird übergeben |
| Gast ohne Wissen über Mythologie | – | Archiv des Hosts wächst; Gast profitiert nur im Host-Spiel |
| Mehrere Hunter im selben Verhör | – | Wer fragt? `48#97E`: persönliche Dialoge persönlich; ein Gespräch ist ein Szenenanker, kein Team-Votum |
| Gleichzeitige Gefangennahme mehrerer | – | mehrere Haftzustände; Zeitsprung mit allen Aktiven |
| Zwei Spieler am selben Gerät | – | nicht in den Quellen |
| Offline-Betrieb | – | `48#65E`: kein Online-Zwang für Companion; Koop-Netzwerk offen |
| Unterschiedliche Schwierigkeiten im selben Spiel | – | nur für Hilfen sinnvoll; Kampf/Verletzung offen |

---

## 13. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Host-Welt als Autorität |
| Journal und Archiv ([Modul 04](04-Archiv-Journal-und-Hunter-Record.md)) | Teamakte, persönliche Akte |
| Entscheidungssystem | Abstimmung, Gleichstand, Zeit |
| Companion-App | geteilte Fallakten |
| Haft ([Modul 16](16-Haft-und-Strafvollzug.md)) | individuell, Zeitsprung gemeinsam |
| Recht ([Module 14](14-Polizei-Behoerden-und-Fahndung.md), [15](15-Gerichte-und-Verfahren.md)) | Akten pro Charakter |
| Hilfen ([Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md)) | Profil der Person |
| Charaktererstellung | Hunter pro Spieler im Host-Spielstand |
| Netzwerk | offen |

---

## 14. Ergänzungen aus der Quellenauswertung (Koop und Online)

### Koop und Online: zwei getrennte Räume (August 2026)

Die Koop-Entwürfe dieses Moduls stützen sich auf Chat 48. Chat 23 enthält davon unabhängige Entscheidungen vom 25. bis 28.08.2026, die den Rahmen setzen. Sie betreffen primär „SPN Online“ und das Verhältnis von Koop und Online; für die Ermittlung sind sie relevant, weil sie bestimmen, **in welchem Vertrauensraum** Beweise, Fahndung und Fallfortschritt gelten.

| Entscheidung | Quelle | Status |
|---|---|---|
| Es soll einen Online-Modus mit Serververbindung geben; Details offen | `23#427E` | 📋 (Details ❓) |
| Online-Koop mit 2–8 Personen angeregt; „bis 4 sollte es gehen“ | `23#429E` | 📋 Richtung, 8 ❓ |
| Session selbst hosten „wie bei minecrsft“; Cheaten erlaubt, weil es nur um Freunde geht; Voicechat (auch über das Ingame-Handy) | `23#431E` | 📋 für privaten Koop |
| Online soll langfristig „besser als gta online“ sein | `23#435E` | 📋 Langzeitziel |
| Eine Lobby mit bis zu 64 oder 100 Spielern „wenn das möglich ist“; private Freundeslobbys gegen Griefing | `23#437E` | 📋 Langzeitziel, Machbarkeit ❓ |
| Koop privat **und** Online (Online privat/Freundeslobby/öffentlich); in Online-Welten kein Cheaten, da diese „miteinander verküpft“ sind; Cheats nur im Koop | `23#440E` | 📋 |
| Koop = Singleplayer für 2–4 Personen, „keine eigene storys“; Fokus eher auf Online | `23#442E` | 📋 |
| Online: Jeder hat sein eigenes Online, unabhängig von anderen; bei gemeinsamem Job bekommen nur Teilnehmer die Cutscenes | `23#444E` | 📋 |
| Ein Fall gilt für beide Beteiligten als abgeschlossen, kann aber „anders“ nochmals gespielt werden | `23#446E` | 📋 (Frage des Assistenten: `23#445A`, Varianten A–C) |
| Online ohne Mods; mit Freunden in privater Lobby möglich, nicht mit Fremden | `23#551E` | 📋 |
| Privates Online hat keine Cheats; der Spielstand wird wie in GTA übernommen | `23#553E` | 📋 |
| Fortschritt bleibt beim Lobbywechsel gleich (Beispiel mit Geld, Fahrzeug, Safehouse, Rang) | `23#555E`, `23#557E` | 📋 (Auslegung: `23#556A`, `23#558A`) |
| Permadeath nicht als Basisspiel-Erfolg, sondern als Zusatzpaket | `23#557E` | 📋 |

**Folgerungen für die Ermittlung (💡, nicht beschlossen):**

- Fahndungsstufe, Case Record und Beweisstand (Module 14, 15) gelten pro **Welt-/Profilraum**. Ein Koop-Spielstand ist von SPN Online getrennt; Koop-Fortschritt darf nach der Aussage des Assistenten niemals in das offizielle Online-Konto fließen (`23#441A`, 💡 Vorschlag; die Trennung selbst führte der Entwickler in `23#440E` aus, die Aussage zur Konten-Isolierung bestätigte er nicht wörtlich).
- Der Aufbau ‚Jeder hat sein eigenes Online‘ (`23#444E`) bedeutet für Fälle: **Fall- und Beweiszustand ist personen-, nicht serverweit**. Wer in Online einen Geist untersucht, ändert den Fall des Nachbarn nicht. Gemeinsame Beweiskette gibt es nur im Gruppenjob.
- Die Frage, was ein Teilnehmer erhält, der einem fremden Fall hilft, ist teilweise offen: `23#446E` sagt „abgeschlossen“ für beide. Die Varianten ‚nur Hilfe‘ und „gemeinsamer Fortschritt“ aus `23#445A` (Möglichkeit C) wurden nicht ausdrücklich gewählt.

### Konflikt K-E9: Kontext

Die Aussage von `48#103E` („Gemeinsamer Missionsfortschritt“) und die Host-Welt-Regel aus `48#105E`/`48#107E` betreffen den privaten Koop. Die August-Entscheidung `23#446E` („als abgeschlossen“) betrifft **SPN Online** und kann daher K-E9 nicht auflösen; sie ist aber ein zweiter Beleg, dass beim Zusammenspiel Fälle für beide als erledigt zählen. Beide Räume haben unterschiedliche Datenhoheit. ❓ Was ein Gast in seinem eigenen Spielstand erhält, bleibt für den Koop offen (Abschnitt 2); keine eigene Q-Nummer.

---

## 15. Was fehlt
- **Teilung des Wissens** (automatisch oder aktiv, Q-E64).
- **Klassifikation** der Entscheidungen (Q-E65).
- **Rechtliche Fragen** im Koop (Q-E66).
- **Vertrauens- und Konfliktmodell** unter Mitspielern.
- **Netzwerk, Replikation, Autorität**: bewusst offen.
- **Gast-Belohnungen** und Fortschritt (K-E9).
- **Schutz vor Missbrauch** (Beweise zerstören, Abstimmungen blockieren).
