# Ermittlung 09: Falllebenszyklus und Fallgenerierung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie ein Fall **entsteht, verläuft und endet**. Beschlossene Anforderungen sind getrennt von **Entwürfen** (💡) geführt. Die Zustände, Übergänge und Generatorschritte unten sind **unverbindliche Vorschläge**, die zeigen, welche Fragen ein Fallsystem beantworten muss. Fallbeispiele sind erfunden und keine Lore. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Grundlagen: [Modul 02](02-Falltypen-und-Fallmodell.md) (Fallklassen, Auflösungen), [Modul 07](07-Detektivlaufbahn-und-organische-Faelle.md) (organische Entstehung). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Begriffe](#1-begriffe)
2. [Beschlossene Anforderungen im Überblick](#2-beschlossene-anforderungen-im-überblick)
3. [Entwurf: Herkunftstypen eines Falls](#3-entwurf-herkunftstypen-eines-falls)
4. [Entwurf: Fallwahrheit und Wissensstand](#4-entwurf-fallwahrheit-und-wissensstand)
5. [Entwurf: Zustandsmaschine des Falls](#5-entwurf-zustandsmaschine-des-falls)
6. [Entwurf: Übergangstabelle](#6-entwurf-übergangstabelle)
7. [Entwurf: Eskalation, Zeit und die Welt](#7-entwurf-eskalation-zeit-und-die-welt)
8. [Entwurf: Generierung dynamischer Aufträge](#8-entwurf-generierung-dynamischer-aufträge)
9. [Entwurf: Zeitsprünge und Missionsschutz](#9-entwurf-zeitsprünge-und-missionsschutz)
10. [Entwurf: Abbruch und Wiederaufnahme](#10-entwurf-abbruch-und-wiederaufnahme)
11. [Entwurf: Folgen für Opfer, Monster und Orte](#11-entwurf-folgen-für-opfer-monster-und-orte)
12. [Sonderfälle](#12-sonderfälle)
13. [Prüfregeln für Fallautoren (Entwurf)](#13-prüfregeln-für-fallautoren-entwurf)
14. [Ergänzungen aus der Quellenauswertung](#14-ergänzungen-aus-der-quellenauswertung)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Begriffe

Die Quellen benutzen die Wörter Fall, Mission, Auftrag, Lead und Ereignis nicht einheitlich. Dieses Modul verwendet sie wie folgt (Arbeitsdefinition, 💡):

| Begriff | Bedeutung | Quelle der Unterscheidung |
|---|---|---|
| **Vorfall** | etwas, das in der Spielwelt geschieht (Verschwinden, Tod, Anomalie), unabhängig davon, ob jemand es bemerkt | `M-PD` ‚Organic world-event / case emergence‘ |
| **Signal** | ein schwaches Anzeichen eines Vorfalls (Zeitungsmeldung, Vermisstenanzeige, Polizeifunk, Gerücht, Tierverhalten) | `M-PD` (Aufzählung) |
| **Muster** | mehrere Signale, die zusammen als Zusammenhang erkennbar werden | `M-PD` (‚until a pattern becomes identifiable‘) |
| **Lead** | ein erkennbarer Ansatz, den der Spieler verfolgen kann | `23#1076A` („potenzieller Hunter Lead“) |
| **Fall** | ein vom Spieler angenommenes oder verfolgtes Problem | `23#52E`, `23#59A` |
| **Fallwahrheit** | die verborgene Wirklichkeit des Falls (Ursache, Täter, Opfer, Zeitablauf) | Folgerung aus `23#59A` (Wissen und Auflösungen) und `48#154A` (vier Ebenen) |
| **Wissensstand** | das, was der Spieler im Fall erfahren hat | `M-PD` ‚Hunter Archive epistemology‘ |
| **Auflösung** | eine Bedingung, bei der der Fall als gelöst gilt | `23#59A` |

Das Journal führt nur den **Wissensstand** ([Modul 04](04-Archiv-Journal-und-Hunter-Record.md)). Die Fallwahrheit liegt im Weltzustand und ist nie direkt sichtbar.

---

## 2. Beschlossene Anforderungen im Überblick

Die folgende Tabelle bündelt, was schon in anderen Modulen im Detail steht, und nennt nur die für den Lebenszyklus wichtigen Punkte. Alle 📋.

| Nr. | Anforderung | Quelle | Modul |
|---|---|---|---|
| L1 | Angenommene Fälle erscheinen im Journal | `23#52E` | 04 |
| L2 | Ein Fall hat Wissen, mehrere Ansätze, mehrere Auflösungen; keine versteckten Pflichtziele | `23#56E`, `23#57E`, `23#59A` | 02 |
| L3 | Falsche und verfrühte Schlüsse haben Folgen | `M-PD`, `23#1062A` | 02 |
| L4 | Fälle können scheitern, ungelöst bleiben, abgebrochen werden; der Fortschritts-Record führt sie | `23#55A`, `M-PD` | 04 |
| L5 | Zeitgebundene Fälle haben eine **Vorlaufphase**; kein Quest-Spawn von einer Sekunde auf die andere | `M-PD`; `23#308E` | 07 |
| L6 | Bei reduzierter Hilfe brauchen neue Fälle keine automatischen Marker | `M-PD`; `23#308E` | 07 |
| L7 | Ignorierte Entwicklungen haben Folgen (mehr Opfer, Polizei, anderer Jäger, Abwanderung des Monsters, veränderte Beweise, Altfall) | `M-PD`; `23#1074A` Fragen 63–68 | 07 |
| L8 | Das Monster kann weiterziehen; die Spur wandert | `23#1074A` Frage 65 | 07 |
| L9 | Andere Hunter können Fälle übernehmen, lösen, falsch lösen oder sterben | `23#1074A` Frage 66; `23#1076A` | 07 |
| L10 | Ein Fall kann sich selbst lösen (Monster stirbt, zieht weg, Täter wird zufällig entdeckt) | `23#1074A` Frage 67 | 07 |
| L11 | Manche Fälle werden schlimmer (Nest wächst, Fluch verbreitet sich, Kult gewinnt Mitglieder) | `23#1074A` Frage 68 | 07 |
| L12 | Eine verfügbare Nebenmission darf nicht allein durch einen Haft-Zeitsprung dauerhaft verschwinden | `48#161E` | 16 |
| L13 | Wenn der Spieler im Gefängnis ist, laufen Ermittlungen und Gerichtsverfahren weiter | `48#159E` | 16 |
| L14 | Nebenmissionen sind „sehr wichtig“ und „davon soll es sehr viele geben“ | `23#1075E` Punkte 69–70 | 07 |
| L15 | Die Spielzeit ist saisonal und uhrzeitabhängig: manche Fälle spawnen nur zu bestimmten Zeiten oder Jahreszeiten | `23#294E` Punkt 19: „manche Nebenmissionen oder andere fälle/aufgaben spawnen nur in gewisser uhrzeit oder sogar jahreszeit“ | 07 |
| L16 | Der Hauptgegner eines Aktes wird nicht in einer Mission getötet, sondern zieht sich über den ganzen Akt oder mehrere Akte | `23#294E` Punkt 21 | 02 |
| L17 | Kein Fall fühlt sich wie ein anderer an | `23#49E` Punkt 11; `23#54E` | 02 |
| L18 | Hunter können Fälle melden und an NPC-Hunter weitergeben | `23#1075E` (zu 62–68) | 07 |
| L19 | Das Spiel soll im Koop Fälle mit Fallleiter und Abstimmung unterstützen | `48#97E`, `48#99E` | 18 |

Aus `M-OD` (Stand 31.08.2026) bleiben **offen**: die genaue Zahl der Nebenfälle und die konkreten handgeschriebenen Fälle (`M-OD` ‚Exact side-case count and concrete authored cases‘), die genaue Verlustbehandlung nach Tod oder Beinahe-Tod, Rettung, Krankenhaus und Checkpoint und die Zuordnung zu Schwierigkeit und Permadeath.

---

## 3. Entwurf: Herkunftstypen eines Falls

> [!NOTE]
> **Nur Vorschlag (💡).** Die Quellen nennen vier **Missionstypen** (`23#1076A`: Hauptmissionen, Nebenfälle, dynamische Hunter-Aufträge, mehrdeutige Fälle) und die Klasse der gewöhnlichen Detektivfälle (`23#1062A`). Die Tabelle ordnet das nach **Herkunft**, damit ein Fallsystem die Unterschiede bei Zeit, Speicherung und Schutz berücksichtigen kann.

| Herkunft (Arbeitsname) | Autor | Start | Zeitverhalten | Schutz bei Zeitsprüngen | Beispielquelle |
|---|---|---|---|---|---|
| **Hauptfall** | handgeschrieben | Akt/Kapitel | gerichtet, Folgen stark, ein Akt kann um ein Hauptmonster kreisen | geschützter Storyzustand (Abschnitt 9) | `23#46E`, `23#294E` |
| **Großer Nebenfall** | handgeschrieben, Mehrwege | Lead aus der Welt oder NPC-Kontakt | eigene Vorlaufphase, eigene Uhr | Fortbestand garantiert (Abschnitt 9) | `23#1076A`, `23#1075E` Punkt 69 |
| **Dynamischer Hunter-Auftrag** | Weltsimulation | Signal und Muster aus der Ökologie | endlich, kann ablaufen, übernommen oder erledigt werden | nicht geschützt; kann verfallen | `23#1075E` Punkt 70, `23#1076A` |
| **Gewöhnlicher Detektivauftrag** | prozedural unterstützt oder handgeschrieben | Kunde, Kontakt, Zeitung | eigene Fristen (Kunde) | nicht geschützt | `23#1062A` |
| **Altfall (historisch)** | Folge eines früheren Falls oder Fund | Dokument, Sammelobjekt | zeitlos, oft ohne Frist | nicht nötig | `23#59A` Fall D; `M-PD` |

Die Herkunft ist **unabhängig** von der Fallklasse (Hunter, gewöhnlich, mehrdeutig): ein dynamischer Auftrag kann mehrdeutig sein, ein Hauptfall übernatürlich.

---

## 4. Entwurf: Fallwahrheit und Wissensstand

### 4.1 Warum die Trennung nötig ist

`23#59A` verlangt, dass das Spiel prüft, ob der Spieler „genug getan“ hat, nicht ob eine Liste abgearbeitet wurde. Dazu muss das System die **tatsächliche Wahrheit** kennen. `M-PD` verlangt gleichzeitig, dass das Archiv nur Entdecktes zeigt. Beides geht nur, wenn Wahrheit und Wissen getrennt gespeichert werden.

### 4.2 Felder der Fallwahrheit (Entwurf)

Die folgende Liste ist ein **Katalog möglicher Felder**, kein Datenformat:

| Feldgruppe | Inhalt |
|---|---|
| Ursache | tatsächliche Ursache (Monsterart, Fluch, Mensch, Unfall, Tier, Täuschung, Mischung), eventuell mehrere |
| Beteiligte | Täter, Opfer, Zeugen, Mitwisser, Hinterleute; jeweils mit Beziehungen und Wissen |
| Orte | Tatorte, Verstecke, Orte der Herkunft; Verweis auf Welt-Orte (Orte sind einmalig, `23#48A`) |
| Zeitablauf | wahre Ereignisse mit Zeitpunkten (Basis für Todeszeit, Alibi, Forensik) |
| Spuren | welche Spuren tatsächlich entstanden sind, wie sie verfallen (Wetter, Zeit), welche gefälscht sind |
| Auflösungen | die gültigen Auflösungsbedingungen (`23#59A`) |
| Wirksame Methoden | was gegen die Ursache hilft, was nicht (aus der Monster-Mythologie, [Modul 05](05-Spuren-Tracking-Forensik-und-Zeugen.md)) |
| Eskalationsuhr | Regeln, wie sich die Lage ohne Eingreifen verschlechtert (Abschnitt 7) |
| Täuschung | falsche Fährten, Manipulation, absichtliche Spuren (`23#1074A` Frage 82) |

### 4.3 Der Wissensstand

Der Wissensstand besteht aus Belegen (Spuren, Aussagen, Dokumente, Medien), Behauptungen (Aussagen über die Ursache mit Wissensstatus) und Spielerinhalten (Hypothesen, Notizen, Marker). Er wird in [Modul 10](10-Beweislogik-und-Wissensmodell.md) beschrieben.

### 4.4 Grundregel (Entwurf)

> **Der Wissensstand wird nie aus der Fallwahrheit „aufgefüllt“.** Er wächst nur durch Handlungen des Spielers (und seiner Begleiter, der Welt, anderer Hunter).

Das schließt Hilfen aus, die ‚die richtige Spur‘ zeigen, weil die Wahrheit sie kennt (vgl. R3 in [Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md#41-regeln-zwischen-den-achsen-vorschlag)).

---

## 5. Entwurf: Zustandsmaschine des Falls

> [!NOTE]
> **Nur Vorschlag (💡).** Die Zustandsnamen sind Arbeitsnamen. Die **Journal-Kategorien** der Quellen sind festgelegt (`23#55A`, 💡 als Beispielliste: abgeschlossen, aktiv, fehlgeschlagen, abgebrochen, ungeklärt, historische Fälle); die internen Zustände unten verfeinern sie.

```text
                    (Vorfall tritt ein)
                           |
                      [ Latent ]            <- Wahrheit existiert, niemand weiß es
                           | Signale entstehen
                      [ Signal ]            <- schwache Hinweise in der Welt
                           | Muster wird erkennbar
                      [ Lead ]              <- Spieler kann Ansatz verfolgen
              ______________|_______________
             |              |               |
   (Spieler nimmt an)  (NPC-Hunter übernimmt)  (Zeit läuft ab / Vorfall endet)
             |              |               |
        [ Aktiv ]      [ Fremd bearbeitet ]  [ Verlaufen ]
             |
   ----------+-----------------------------------------------
   |         |            |            |             |
[Pausiert] [Gelöst]  [Falsch gelöst] [Gescheitert] [Abgebrochen]
   |                                                  |
   +--------------(Wiederaufnahme)--------------------+
                       |
              [ Ungeklärt / Historisch ]
```

### 5.1 Zustandsbeschreibung

| Zustand | Bedeutung | Journal-Kategorie (Vorschlag) |
|---|---|---|
| **Latent** | Der Vorfall hat sich ereignet oder bahnt sich an; der Spieler hat keinen Anhaltspunkt | nicht sichtbar |
| **Signal** | Zeitung, Funk, Gerücht, Tierverhalten usw.; für sich genommen kein Fall | nicht sichtbar oder „Beobachtet“ (Sammlung von Hinweisen ohne Fall) |
| **Lead** | genug Muster, um einen Ansatz zu nennen; der Spieler kann annehmen | „Verfügbar“ |
| **Aktiv** | angenommen und verfolgt | „Aktiv“ |
| **Pausiert** | angenommen, aber vom Spieler zurückgestellt oder blockiert (Haft, Krankenhaus, andere Fälle); Situation entwickelt sich weiter | „Aktiv“ oder „Pausiert“ |
| **Gelöst** | Auflösungsbedingung erfüllt, Folgen eingetreten | „Abgeschlossen“ |
| **Falsch gelöst** | der Spieler hat den Fall für gelöst gehalten und abgeschlossen oder gehandelt, aber die Ursache war eine andere; die Lage kann weiterlaufen | ‚Abgeschlossen (falsch)‘ oder zurück zu „Aktiv“, siehe Abschnitt 6 |
| **Gescheitert** | der Fall endet mit Verlust (Opfer sterben, Kreatur entkommt) | „Fehlgeschlagen“ |
| **Abgebrochen** | vom Spieler verworfen | „Abgebrochen“ |
| **Ungeklärt** | die Welt ging weiter, die Ursache blieb unbekannt | „Ungeklärt“ |
| **Historisch** | Altfall; Wissen darüber ist nur noch Geschichte | „Historische Fälle“ |
| **Fremd bearbeitet** | ein NPC-Hunter hat den Fall übernommen; Ausgang offen | Hinweis im Journal (‚Von anderem Jäger übernommen‘) |
| **Verlaufen** | der Lead ist ohne Spielerbeteiligung verfallen (Frist, Monster zog weiter) | nicht sichtbar oder „Verpasst“ |

### 5.2 Warum „Falsch gelöst“ ein eigener Zustand ist

Die Quellen verlangen, dass ein falscher Schluss **Folgen** hat und dass die Lage eskalieren oder sich neu ausrichten kann (`23#59A` Fall E: „falsche Kreatur, Situation eskaliert, neue Ermittlungsrichtung“; `23#1062A`). Ein Fall, der als „gelöst“ abgeschlossen wird, obwohl die Ursache eine andere war, muss daher zurück in die Welt: Der Vorfall bleibt im Zustand ‚Latent/Signal‘, die Eskalationsuhr läuft weiter. Der Spieler erkennt das über neue Signale (zum Beispiel ein weiteres Opfer), nicht über eine Meldung „Falsche Antwort“ (`23#1062A`: kein „Lade letzten Checkpoint“).

---

## 6. Entwurf: Übergangstabelle

Die Spalte „Wer“ nennt den Auslöser. Alle Übergänge sind 💡; die Quellenspalte nennt, welche **beschlossene Anforderung** den Übergang rechtfertigt.

| Von | Nach | Auslöser (Wer) | Bedingung (Guard) | Wirkung (Effects) | Anforderung |
|---|---|---|---|---|---|
| Latent | Signal | Welt (Uhr) | Eskalationsstufe erreicht | erste Signale werden in die Welt gesetzt | L5, L6 |
| Signal | Lead | Welt | mehrere Signale desselben Vorfalls; Muster erkennbar | Lead wird für den Spieler auffindbar (ohne Marker, wenn Hilfe aus) | L5, L6 |
| Lead | Aktiv | Spieler | Spieler nimmt an (Gespräch, Eintrag, Anruf) | Eintrag im Journal; Wissensstand beginnt | L1 |
| Lead | Fremd bearbeitet | NPC-Hunter | Spieler hat den Fall gemeldet oder nicht angenommen; NPC-Hunter verfügbar | Fall läuft ohne Spieler; Ausgang offen | L9, L18 |
| Lead | Verlaufen | Welt | Frist abgelaufen oder Vorfall beendet | Lead verschwindet; eventuell Folge (Altfall) | L7, L10 |
| Aktiv | Pausiert | Spieler oder System | Haft, Krankenhaus, Reise, anderer Fall | Fall bleibt im Journal; Welt entwickelt sich weiter | L12, L13 |
| Aktiv | Gelöst | Spieler | eine Auflösungsbedingung ist erfüllt und die Lage ist stabil | Folgen werden angewandt; Journal erzeugt Fallbericht | L2 |
| Aktiv | Falsch gelöst | Spieler | der Spieler handelt (beschließt, tötet, schließt ab), die Ursache war aber eine andere | Fall kehrt in den Vorfallzustand zurück; Folgen eskalieren | L3 |
| Aktiv | Gescheitert | System | Verlustbedingung (zum Beispiel alle Opfer tot, Kreatur entkommen und unauffindbar) | Fehlschlag wird im Record geführt | L4 |
| Aktiv | Abgebrochen | Spieler | bewusstes Verwerfen | Vorfall läuft in der Welt weiter | L4, L7 |
| Pausiert | Aktiv | System | Pausengrund endet | Situation kann sich geändert haben (Abschnitt 9) | L12 |
| Abgebrochen | Aktiv | Spieler | erneute Aufnahme, Vorfall noch bestehend | Wissensstand bleibt; Situation aktualisiert | (M-P03 Titel 038) |
| Gescheitert / Gelöst | Historisch | Zeit | genug Zeit vergangen | Eintrag wird Altfall im Archiv | `M-PD` historische Fälle |
| Falsch gelöst | Aktiv | Welt/Spieler | neue Signale, die den Fehlschluss entlarven | Spieler nimmt neue Ermittlung auf; vorheriges Wissen bleibt | `23#59A` Fall E |
| beliebig | Ungeklärt | Zeit | Fall bleibt offen, ohne dass etwas geschieht | bleibt als ungelöst | `23#55A` |

### 6.1 Sonderfälle der Übergänge

| Sonderfall | Verhalten (Vorschlag) |
|---|---|
| Der Spieler löst einen Fall, **bevor** er ihn annimmt (zum Beispiel durch Zufall) | Der Fall wechselt direkt von Signal/Lead zu Gelöst; im Journal erscheint ein Eintrag, der nur das Erfahrene enthält. Eine Annahme ist nicht erforderlich. |
| Zwei Spieler (Koop) treffen gegensätzliche Entscheidungen | Siehe [Modul 18](18-Koop-Ermittlung.md). |
| Ein Zeuge stirbt, bevor der Spieler ihn befragt | Der Fall bleibt lösbar (L2); der Ansatz „Zeuge“ entfällt. Das System muss prüfen, dass wenigstens eine Auflösung erreichbar bleibt (Abschnitt 13). |
| Der Täter stirbt durch einen anderen Hunter | Übergang Fremd bearbeitet; das Journal erfasst das Ergebnis, wenn der Spieler es erfährt. |
| Ein dynamischer Auftrag und ein handgeschriebener Fall betreffen dasselbe Monster | Der Vorfall wird nur **einmal** modelliert; beide Aufträge sind Sichten darauf (Abschnitt 12). |

---

## 7. Entwurf: Eskalation, Zeit und die Welt

### 7.1 Eskalationsuhr

`M-PD` und `23#1074A` (Fragen 63–68) verlangen, dass ein Vorfall sich ohne Eingreifen verändern kann: mehr Opfer, Wanderung, Verschlimmerung, Selbstlösung. Ein Entwurf dafür:

| Element | Beschreibung |
|---|---|
| **Stufen** | benannte Phasen des Vorfalls (zum Beispiel unbemerkt, erste Signale, Häufung, öffentlich bekannt, Gegenmaßnahmen), je nach Vorfallsart verschieden |
| **Uhr** | Spielzeit, nicht Echtzeit; die Stufe wechselt bei Erreichen von Zeitpunkten **oder** Ereignissen |
| **Bedingungen** | Wetter, Jahreszeit, Mond (`23#294E` Punkt 19; `M-PD`) |
| **Wirkungen** | Weltzustand wird verändert (Opferzahl, Spurenqualität, Ort des Monsters) |
| **Unterbrecher** | Eingriffe des Spielers oder anderer Hunter, die die Uhr anhalten oder umlenken |

Die **Geschwindigkeit** ist nirgends festgelegt (❓ Q-E35). Der Master verlangt nur, dass Übergänge glaubwürdig sind („nicht von einer Sekunde auf die andere“, `23#308E`).

### 7.2 Abhängigkeiten zur Weltsimulation

Die Oktoberentscheidungen machen die Welt dynamischer als die Septemberstände:

| Entscheidung | Wirkung auf Fälle | Quelle |
|---|---|---|
| Dynamische Monsterterritorien (umfassend) | Vorfälle entstehen durch Expansion, Verlust, Verteidigung, Aufgabe von Gebieten | `48#143E` zu `48#142A` Option D |
| Monstergesellschaft (Organisationen, Hierarchien, Machtkämpfe) | Vorfälle können Folge interner Konflikte sein | `48#141E` |
| Menschliche Gesellschaft reagiert dynamisch (Politik, Vertuschung, Gegenmaßnahmen) | Behörden und Medien verändern Signale und Beweise | `48#145E` |
| Monster-Gedächtnis (Rache, Angst, Informationsaustausch) | Monster reagieren auf frühere Begegnungen; Folgefälle | `48#139E` |

Für die Ermittlung heißt das: **Fälle sind nicht isoliert**, sie sind Sichten auf Zustände einer simulierten Welt. Das Fallmodell muss auf Weltzustand zugreifen, nicht umgekehrt (Abschnitt „Persistenz“, [Modul 19](19-Weltzustand-und-Persistenz.md)).

### 7.3 Zusammengefasste Simulation bei großen Sprüngen

Bei langen Sprüngen (Haft, Krankenhaus) soll die Welt „in größeren Simulationsschritten“ weiterlaufen und nur Ergebnisse speichern (`48#160A`: „Ein Zeitsprung von sechs Monaten darf nicht einfach sechs Monate Echtzeitsimulation ausführen“; Ausformulierung des Assistenten, 💡, mit der Entscheidung `48#159E` „C ja D ist longterm“ als Grundlage). Für Fälle bedeutet das: Eskalationsuhren müssen sich **in Schritten** fortschreiben lassen, nicht nur pro Tick. Sie brauchen eine Fortschreibungsfunktion ‚von Zeit A nach Zeit B‘, die deterministisch ist (vgl. Determinismus-Anforderung des Weltzustands, [Decision 0002](../../Decisions/0002-world-facts.md)).

---

## 8. Entwurf: Generierung dynamischer Aufträge

> [!NOTE]
> **Nur Vorschlag (💡).** Der Master beschreibt die **Wirkung** (viele dynamische Aufträge, nicht geskriptet, aus der Welt entstehend, nicht wie ein MMO-Spawn), aber **keinen Generator**. `M-OD` führt ‚Exact dynamic World Event generator schema, event families, persistence and cooldowns‘ als offen. Der Entwurf unten zeigt eine mögliche Pipeline und die Randbedingungen aus den Quellen.

### 8.1 Randbedingungen aus den Quellen

1. **Kein MMO-Spawn**: Vorlaufphasen mit schwachen Signalen (`M-PD`).
2. **Viele, nicht geskriptet** (`23#1075E` Punkt 70).
3. **Die Monsterökologie erzeugt Vorfälle** (`23#1074A` Frage 62).
4. **Wichtige Arten sind handgeschrieben**, prozedurale Variation betrifft Alter, Temperament, Narben, Erfahrung, Größe in plausiblen Grenzen und Tendenzen, ohne lorebrechende Kombinationen (`M-PD` „Creature / supernatural framework“).
5. **Orte sind einmalig** (`23#48A`); ein Fallort darf nicht recycelt werden. Für dynamische Aufträge heißt das: Sie nutzen **bestehende Orte** der Welt, keine eigens kopierten Fallorte.
6. **Fälle dürfen sich nicht gleich anfühlen** (`23#49E` Punkt 11).
7. **Der Spieler muss nicht alles übernehmen**; die Welt läuft auch ohne ihn (`23#1076A`).
8. **Keine Zwangsabhängigkeit von sozialen Medien** (`M-B40`).
9. **Grundregel Detektivmodus**: ohne Hilfe lösbar.

### 8.2 Mögliche Pipeline

| Schritt | Aufgabe | Eingaben | Ausgabe |
|---|---|---|---|
| 1. Auslöser | Ereignis in der Ökologie oder Gesellschaft löst einen Vorfall aus (Hunger, Revier, Fluch, Mord) | Territorien, Zeit, Wetter, Mondphase, Bevölkerung | ‚Vorfallsabsicht‘ (Art, Region, Zeitpunkt) |
| 2. Vorlage wählen | Wahl aus einer **handgeschriebenen** Bibliothek von Vorfallsmustern (Ablauf der Signale, mögliche Verläufe) | Art, Region, Variationsmarken | gewähltes Muster |
| 3. Instanziieren | Muster wird mit Welt-Elementen gefüllt (Opfer, Zeugen, Orte aus dem Bestand) | Welt-NPCs, Orte | konkrete Fallwahrheit |
| 4. Prüfen | Automatische Validierung: lösbar ohne Hilfe, kein verstecktes Pflichtziel, Orte existieren, Zeugen leben | Regeln aus Abschnitt 13 | gültig oder verworfen |
| 5. Planen | Signale und Leads werden zeitlich in die Welt gesetzt | Eskalationsuhr | Signalplan |
| 6. Registrieren | Fall wird als „Latent“ in den Weltzustand geschrieben | – | Fall-Eintrag |
| 7. Beobachten | Welt führt die Uhr; Spieler oder NPCs handeln | – | Zustandswechsel |

Wichtig: Schritt 2 benutzt **handgeschriebene Muster**, damit Fälle Qualität und Sinn behalten. Das entspricht der Aussage, dass Hauptmissionen und große Nebenfälle handgebaut sind und dynamische Aufträge „kleinere“ Aufträge sind (`23#1076A` Missionstypen; `23#1075E` Punkte 69–70). Eine freie, KI-gestützte Fallerzeugung ist nicht beschlossen und wäre eine eigene Entscheidung.

### 8.3 Vielfalt und Wiederholung

`M-P03` listet als Entwurfsthemen Wiederholungs-Markierungen (Titel 046), ein Editor-Werkzeug zur Wiederholungsanalyse (047) und eine Abdeckungsprüfung der Pfade (048). Das ist ein **Entwurfsrahmen** für Fallautoren, kein Beschluss. Ein möglicher Kern: Jeder Fall trägt Eigenschaften (Beginn, Hinweisart, Täterart, Auflösungsart, Ort), und ein Prüfwerkzeug warnt, wenn die Mischung zu ähnlich zu den letzten Fällen ist (💡).

### 8.4 Zahlen

| Größe | Stand |
|---|---|
| Zahl der handgeschriebenen Nebenfälle | offen (`M-OD`) |
| Rate dynamischer Aufträge | offen |
| Gleichzeitig aktive Leads | offen |
| Zahl der Vorfallsmuster | offen |

---

## 9. Entwurf: Zeitsprünge und Missionsschutz

### 9.1 Beschlossene Regeln

| Regel | Quelle |
|---|---|
| Die Welt läuft während einer Haft weiter (Monsterterritorien, NPCs, Organisationen, Hunter, Polizei, Gesellschaft) | `48#159E`; Auslegung `48#160A` |
| Bereits verfügbare Nebenmissionen dürfen nicht allein deshalb **dauerhaft** verschwinden, weil der Spieler im Gefängnis Zeit überspringt | `48#161E` |
| Ein Welt-Zeitsprung im Koop braucht die Zustimmung aller aktiven Spieler | `48#163E` |

### 9.2 Der Unterschied zwischen Fortbestand und Ablauf (Vorschlag des Assistenten)

`48#162A` (💡, aber als Auslegung der Entscheidung `48#161E`): Der **Fortbestand** einer Mission und ihr **aktueller Ablauf** sind zu trennen. Die Mission bleibt im Journal; die **Situation** kann sich geändert haben (die vermisste Person wurde verlegt, ein anderer Hunter fand Hinweise, die Spur muss neu aufgenommen werden). Besonders wichtige Nebenmissionen können „geschützte Storyzustände“ erhalten, damit entscheidende NPCs oder Missionsgegenstände nicht in der Hintergrundsimulation verloren gehen.

### 9.3 Schutzstufen (Entwurf)

| Stufe | Wirkung bei langem Sprung | Für welche Fälle |
|---|---|---|
| **S0 – kein Schutz** | Fall kann verfallen, übernommen oder gelöst werden | dynamische Aufträge, einfache Detektivaufträge |
| **S1 – Fortbestand** | Der Eintrag bleibt; Ablauf kann sich ändern; „Spur erneut aufnehmen“ | große Nebenfälle |
| **S2 – geschützte Schlüsselfiguren** | S1 plus: Schlüsselfiguren und -gegenstände werden von der Simulation nicht dauerhaft entfernt | wichtige Nebenfälle |
| **S3 – Storyzustand** | Fall steht in einem geschützten Zustand; der Sprung ändert ihn nicht oder nur nach Regel | Hauptfälle |

Die Zuordnung ist **eine Entscheidung des Fallautors** und nicht festgelegt (❓ Q-E36). Auch die Frage, ob der Spieler gewarnt wird, bevor er einen Sprung bestätigt, ist offen (❓ Q-E37).

### 9.4 Weitere Zeitsprünge

Ein Zeitsprung entsteht nicht nur durch Haft:

| Anlass | Quelle | Fallwirkung |
|---|---|---|
| Krankenhaus (Zeit vergeht schneller, Heilung schneller) | `48#117E` | wie Haft, aber kürzer; Koop-Regel beachten (kurze Dauer, bis ca. 15 min) |
| Schlaf | `M-CA0905` | kleine Sprünge; keine Besonderheit |
| Reise | – | Zeit vergeht im Spiel normal |
| Zeitreise in die Vergangenheit | `48#183E`, `48#185E` (Tesla-Linie) | eigener Themenbereich (Reise, nicht Gegenstand dieses Ordners) |

Nach `48#117E` gilt im Koop: Der ausgefallene Spieler ist nur ‚ein paar Minuten bis 15 min‘ weg; man kann das auch **klassisch** einstellen („also wie D“). Das ist eine Gesamtregel für Tod und Krankenhaus; die Auswirkung auf Fälle ist unbeschrieben.

---

## 10. Entwurf: Abbruch und Wiederaufnahme

| Frage | Vorschlag |
|---|---|
| Darf der Spieler einen Fall verwerfen? | Ja (L4). Der Fall wechselt zu Abgebrochen, der Vorfall läuft in der Welt weiter (L7). |
| Was bleibt vom Wissensstand? | Alles Erfahrene bleibt im Archiv. Journal-Notizen bleiben erhalten (der Spieler hat sie geschrieben). |
| Kann er wieder aufgenommen werden? | Ja, solange der Vorfall besteht; sonst ist der Fall zu Ungeklärt, Gelöst-durch-andere oder Historisch geworden. |
| Darf ein abgebrochener Fall **Strafen** haben? | Nur über die Eskalation der Welt, nicht durch ein Strafsystem des Spiels (kein „Mission Failed“-Bildschirm; `23#1062A`). |

Die Prompt-Sammlung führt ‚Case abandonment and reactivation rules‘ als Entwurfsthema (`M-P03`, Titel 038). Konkrete Regeln stehen dort nicht.

---

## 11. Entwurf: Folgen für Opfer, Monster und Orte

Die Entwurfsthemen der Prompt-Sammlung (`M-P03`, Titel 039–045) nennen als Anschlussstellen eines Falls: Weltzustand, NPC-Gedächtnis, Hunter-Archiv, Wiederverwendung von Orten, zeitabhängige Folgen, Umzug des Monsters und Opferzustände. Sie sind **Themenüberschriften**, keine Spezifikation. Ein Entwurf, wie diese Anschlussstellen **inhaltlich** gefüllt werden könnten:

| Anschlussstelle | Wirkung (Vorschlag) | Beschlossene Grundlage |
|---|---|---|
| Opfer | Opfer können sterben, verschwinden, gerettet werden, traumatisiert sein; dauerhafte Verletzungen persistieren | `23#1058A` („NPC-Verletzungen“, ‚wichtige NPCs sterben nicht zufällig‘); Gameplay-Systeme §8 |
| Zeugen | Zeugen können sich erinnern, falsch erinnern, umziehen, sterben | `23#1056A` Fragen 23–30 |
| Monster | zieht weiter, versteckt sich, wird vorsichtiger, lernt aus Begegnungen | `23#1074A` Fragen 56–61, 65; `48#139E` |
| Orte | Blut und Schäden bleiben bis zur Reinigung; Absperrung durch Polizei; Menschen meiden Orte | `23#1074A` Frage 150; `23#1056A` Frage 50; Frage 114 |
| Polizei | ermittelt weiter; führt eigene Akten; erklärt zunächst normal | `23#1057E` Punkt 116; [Modul 14](14-Polizei-Behoerden-und-Fahndung.md) |
| Öffentlichkeit | Medien berichten; Gerüchte; Wissen der Region ändert sich | `48#147E`; [Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md) |
| Hunter-Netzwerk | Ruf, Rivalen, Übernahme von Fällen | `23#1074A` Frage 66; `48#153E` |

---

## 12. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| **Gleicher Vorfall, mehrere Aufträge** | Ein Monster löst einen dynamischen Auftrag und einen Kundenauftrag aus | Ein Vorfall, mehrere Sichten. Die Lösung eines Auftrags wirkt auf den Vorfall und damit auf den zweiten Auftrag. |
| **Fall ohne Täter** | Eine Anomalie ohne Verursacher (Ort, Fluch, Naturphänomen) | Die Auflösung nennt eine Bedingung wie „Fluch gebrochen“ (`23#59A` Beispiel B). |
| **Fall mit Wirt** | Besessenheit/Verwandlung eines Menschen (`23#1074A` Fragen 142–146) | Fallwahrheit enthält Wirt und Wesen getrennt; Auflösungen unterscheiden Retten und Töten. |
| **Fall über mehrere Akte** | Hauptgegner, der sich über Akte zieht (`23#294E` Punkt 21) | Fall besteht aus mehreren Stufen; Zwischenstufen sind eigene Fälle mit Verweis auf die Hauptlinie. |
| **Zeitreise** | Fall in der Vergangenheit | Eigenes Thema (Reiche, Zeitreise, [Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md)). |
| **Schattenelite / staatliche Forschung** | Ein Fall führt zu verdeckten Strukturen | Entdeckungslogik in [Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md). |
| **Der Spieler wird selbst verdächtigt** | Fall F aus `23#55A` | Verknüpfung mit [Modul 13](13-Identitaet-Ruf-und-Formwandler.md) und [Modul 14](14-Polizei-Behoerden-und-Fahndung.md). |
| **Fall in der Hölle/Fegefeuer/Himmel** | Reiche als eigene Welten | Offen (Realmplanung in Welt 26). |
| **Modfälle** | Mods registrieren Fälle | Fälle müssen dieselben Zustände unterstützen; Mods dürfen Basisfälle nicht überschreiben, nur ergänzen (Override-Schicht, `23#59A`). |
| **Mehrere Spieler, ein Fall** | Koop | [Modul 18](18-Koop-Ermittlung.md). |

---

## 13. Prüfregeln für Fallautoren (Entwurf)

> [!NOTE]
> **Nur Vorschlag (💡).** Die Prompt-Sammlung nennt als Entwurfsthemen einen Validator ‚no hidden mandatory objective‘ (Titel 049), Detektivmodus-Regressionstests (050), Tests für Speichern und Laden mitten in der Ermittlung (051), Migrations-Fixtures (052) und einen Validierungsbericht (053). Sie sind nicht spezifiziert. Die folgende Liste leitet **Prüfregeln aus den beschlossenen Anforderungen** ab.

| Nr. | Prüfregel | Ableitung |
|---|---|---|
| V1 | Jeder Fall hat mindestens eine Auflösung, die **ohne** Pflichtbesuch bestimmter Orte oder Personen erreichbar ist | L2 |
| V2 | Kein Auflösungspfad hängt von einem einzigen Zeugen, einem einzigen Gegenstand oder einem einzigen Sammelobjekt ab, der verloren gehen kann | L2, L10; `M-PD` (Hauptwissen nicht auf Sammelobjekten) |
| V3 | Wenn ein Weg stirbt (Zeuge tot, Ort zerstört), bleibt mindestens eine Auflösung erreichbar, oder der Fall führt bewusst zu Gescheitert | L3, L4 |
| V4 | Kein Hinweis, der nur über soziale Medien erreichbar ist, ist für eine Auflösung nötig | `M-PD`, `M-B40` |
| V5 | Der Fall ist im Detektivmodus ohne Hilfe lösbar | `23#52E` |
| V6 | Hilfen lesen nur Wissensstand (R3) | [Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md) |
| V7 | Kein Fallort ist eine Kopie eines anderen wichtigen Ortes | `23#48A` |
| V8 | Alle Orte des Falls existieren in der Welt; es gibt keinen ‚Fallort nur für den Fall‘ ohne Welt-Eintrag | `23#48A`, `M-PD` |
| V9 | Der Fall kann im Zustand „Pausiert“ weiterleben (Abschnitt 9) | L12 |
| V10 | Speichern und Laden zu jedem Zeitpunkt ändert nichts am Fall (gleiche Zustände und gleiche Wahrheit) | Anforderung an den Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) |
| V11 | Ein Mod-Fall überschreibt keine Daten des Basisspiels | `23#59A` (Overrides in eigener Schicht) |
| V12 | Der Fall nennt seine Wiederholungsmarken (Beginn, Hinweisart, Täterart, Auflösungsart) | L17 |

Eine Tabelle der entsprechenden Testfälle steht in [Modul 23](23-Testkatalog-und-Abnahmekriterien.md).

---

## 14. Ergänzungen aus der Quellenauswertung

Dieser Abschnitt trägt Quellenfunde nach, die den Entwurf in §8 betreffen. Wo ein Fund eine Aussage dort stützt, steht es ausdrücklich; keine Zeile des Entwurfs wird dadurch zur Entscheidung.

### 14.1 Der Dynamic Case Generator im Chat (30.08.2026)

Der Assistent schlug einen endlosen, prozeduralen Social-Media-Feed vor, aus dem **dynamische Fälle** entstehen können (`23#622A`). Die Reaktion des Entwicklers beschränkte sich auf Vorbehalte: „Ich will keinen socialmedia zwang für story nur für optionale weltevents das sind auch keine nebenmissionen.“ (`23#623E`). Die Struktur selbst wurde nicht widersprochen. Status: 💡 Vorschlag mit den Entwicklergrenzen aus `23#623E`.

| Element (`23#622A`) | Inhalt |
|---|---|
| Kette | Weltzustand, etwas Auffälliges geschieht, ein NPC erlebt, filmt oder hört es, ein Beitrag entsteht, der Spieler ignoriert oder untersucht ihn, erst beim Hingehen wird das Weltereignis konkretisiert, daraus kann ein dynamischer Fall entstehen |
| Grundsatz | „Der **Post ist nicht die Mission**.“ Er ist eine mögliche Informationsquelle; die meisten Beiträge führen zu nichts |
| Größenklassen | Beobachtung (kleiner Fund), Weltereignis (5 bis 15 Minuten), dynamischer Fall (20 bis 60 Minuten), seltener erweiterter Fall (mehrere Schritte oder Tage) |
| Hintergrunddaten des Beispiels | Ort, Zeit, Kandidatenpool der Ursache, Zuverlässigkeit des Zeugen, Qualität der Belege |
| Varianten beim Hingehen | nur ein Tier, ein Betrüger, ein Geist, etwas anderes Übernatürliches, das Ereignis ist vorbei, aber Spuren bleiben |
| Definition eines dynamischen Falls | Vorfallstyp, Kandidatenpool der Kreatur, Ortsanforderungen, Opferprofil, Zeugen, Beweisarten, Pool falscher Fährten, Eskalationsregeln, Auflösungsmethoden, Fehlschlag- und Ablaufregeln, Belohnungen, Archivwirkungen |
| Art des Systems | „regelbasiertes, datengetriebenes System“, **keine** KI, die Storytext unkontrolliert erfindet; „aus Systemen und Templates“ |
| Rückwirkung | nach der Lösung Folgeposts, Kommentare, Richtigstellungen; öffentliche Tötung eines Monsters erzeugt Videos; Behörden geben andere Erklärungen heraus (Verbindung zu [Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md)) |
| Pflicht | keiner (Hauptgeschichte und handgeschriebene Nebenfälle ohne Feed spielbar) |

Das stützt die Entwurfsschritte in §8.2 (Vorlage aus handgeschriebener Bibliothek, Instanziierung mit Welt-Elementen, Prüfung, Signalplan). **Neu gegenüber dem Entwurf:** die Größenklassen als Spielzeitrahmen und die Variantenliste ‚Tier, Betrüger, Geist, anderes Übernatürliches, Ereignis vorbei‘, die der Entwurf als falsche Alarme und Mischfälle aufgreift ([Modul 02 §8](02-Falltypen-und-Fallmodell.md#8-mischfälle-falsche-alarme-und-täuschung)).

### 14.2 Fall-Entstehung ist ein Weltzustand, kein Zufallsspawn

Der Assistent (`23#307A` Punkt 21): Wenn ein zeitabhängiger Fall „spawned“, soll das heißen, dass der **Weltzustand jetzt die Voraussetzungen erfüllt**; dann verschwinden Opfer, NPC-Berichte entstehen, Nachrichten erscheinen, die Polizei reagiert, Gerüchte entstehen. Der Entwickler: „ja genau, aber nicht von einer sekunde auf die andere, es muss einen übergang geben, und vielleicht schon in der zeitung vermehrte komische hinweise und erst später sieht man mehr. ich würde nicht mal wirklich questmarker auf der map machen, wenn man es einstellt“ (`23#308E` Punkt 21). Status: 📋 (Aussage des Entwicklers); vgl. Anforderungen L5 und L6.

### 14.3 Zeitfenster vernichten keinen Inhalt

Der Assistent (`23#307A` Punkt 22): Wenn ein Winterfall nicht entdeckt wird, ist er vielleicht im nächsten Winter wieder möglich; oder der Vorfall geschieht trotzdem, aber die Konsequenzen ändern sich (‚Spieler ignoriert Hinweise, Opfer stirbt, Fall ist später noch untersuchbar, aber die Situation ist anders‘). Der Entwickler: „ja gut“ (`23#308E` Punkt 22). Status: 📋 (bestätigt). Das entspricht Anforderung L15 und der Schutzlogik in §9.

### 14.4 Aktualisierte Fallvariante statt Löschen

Wenn die Welt sich so ändert, dass ein Nebenfall nicht mehr existieren kann: „Original Case, World State changed, Updated Case Variant“ statt Löschen (`23#583A`, 💡; bestätigt in der Gesamtlinie durch `23#584E`). Das ist die Quelle der Idee ‚Situation hat sich geändert‘ in §9.2.

### 14.5 Hauptbedrohungen sind langfristige Entitäten

Der Assistent (`23#307A` Punkt 23): Ein Akt-Boss wird nicht in einer Quest eingeführt und kurz danach erschossen; Bedrohungen wirken indirekt, über Hinweise, Agenten, frühe Begegnungen, Ermittlung, gescheiterte Versuche, wechselnde Strategien, schließlich Konfrontation; ein vermeintlicher Hauptgegner kann Teil eines größeren Problems sein. Der Entwickler: „toll“ (`23#308E` Punkt 23). Passt zu `23#294E` Punkt 21 (Anforderung L16).

### 14.6 Das Archiv eines Eintrags: Bestätigt, vermutet, widerlegt

`23#307A` Punkt 24 nennt für den Kreatureneintrag Identifikation, Erscheinung, Anatomie (nur erforscht), Verhalten, Fähigkeiten (nur bekannte), Schwächen (nur bestätigte oder vermutete), Spuren, Habitat, Ursprung (Theorie, historische Quellen, widersprüchliche Versionen), Fälle; Status „Confirmed, Suspected, Disproven“. Der Entwickler: „passt“ (`23#308E` Punkt 24). Das bestätigt die Statusliste der Wissensanzeige ([Modul 04 §6](04-Archiv-Journal-und-Hunter-Record.md#6-wissensstatus-und-falsche-lore)); die Fassung des Masters führt zusätzlich „Disputed“ und ‚Historical/Unverified‘ (`M-PD`).

### 14.7 Testbereiche aus dem Chat

Der Assistent listete Tests für das Monsterframework (`23#293A` Punkt 24), darunter: nicht beobachtetes Verhalten wird nicht magisch gewusst; beobachtetes Verhalten kann gelernt werden; Archiv zeigt nichts Unbekanntes; Case Outcome aktualisiert den Weltzustand; Save/Load erhält Monsterzustand; Modmonster lässt sich registrieren. Sie sind in [Modul 23](23-Testkatalog-und-Abnahmekriterien.md) als T-F19 bis T-F22 aufgenommen.

---

## 15. Was fehlt
- Eine **Fallliste**, die **Zahl** der Nebenfälle und die konkreten Fälle (`M-OD`).
- Ein **Datenformat** für Fallwahrheit, Eskalation und Fallzustand; ob es aus den Weltzustandsfakten abgeleitet werden kann ([Modul 19](19-Weltzustand-und-Persistenz.md)).
- **Zahlen** für Eskalationsgeschwindigkeit, Rate dynamischer Aufträge, Fristen, Zahl gleichzeitiger Leads.
- Die Entscheidung über eine **KI-gestützte** Fallerzeugung (nicht beschlossen).
- **Schutzstufen** und Warnungen vor Zeitsprüngen (Q-E36, Q-E37).
- Die Behandlung von **Medaillen** bei falsch gelösten Fällen (`M-OD` ‚Exact medal weighting‘).
- Die Zuordnung zu den Schwierigkeitsgraden (Folgen eines falschen Schlusses).
