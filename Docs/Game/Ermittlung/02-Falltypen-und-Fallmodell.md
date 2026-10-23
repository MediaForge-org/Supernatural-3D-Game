# Ermittlung 02: Falltypen und Fallmodell

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, **was ein Fall ist**, welche Arten es gibt und wie ein Fall aufgelöst wird. Es erfindet keinen Fall, keine Figur und keinen Ort; die Beispiele der Quellen sind Veranschaulichung (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Überblick im Spielvision-Dokument: [§7](../Spielvision.md#7-fälle-und-ermittlung). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Rahmen: Hauptgeschichte, Nebenfälle, dynamische Aufträge](#1-rahmen-hauptgeschichte-nebenfälle-dynamische-aufträge)
2. [Die Fallklassen](#2-die-fallklassen)
3. [Ein Fall ist ein Problem mit Auflösungen](#3-ein-fall-ist-ein-problem-mit-auflösungen)
4. [Mehrere Wege und frühes Lösen](#4-mehrere-wege-und-frühes-lösen)
5. [Falsche und verfrühte Schlüsse](#5-falsche-und-verfrühte-schlüsse)
6. [Auflösungsarten](#6-auflösungsarten)
7. [Variation: kein Schema F](#7-variation-kein-schema-f)
8. [Mischfälle, falsche Alarme und Täuschung](#8-mischfälle-falsche-alarme-und-täuschung)
9. [Der Einstieg ins Spiel](#9-der-einstieg-ins-spiel)
10. [Niederlage, Verletzung und Fallfolgen](#10-niederlage-verletzung-und-fallfolgen)
11. [Ergänzungen aus der Quellenauswertung](#11-ergänzungen-aus-der-quellenauswertung)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Rahmen: Hauptgeschichte, Nebenfälle, dynamische Aufträge

| Ebene | Aussage | Status | Quelle |
|---|---|:---:|---|
| Hauptgeschichte | Zielumfang etwa 80–100 Stunden; in Akten/Bögen gegliedert; ein Akt kann um eine große Bedrohung, ein „Hauptmonster“, kreisen | 📋 | `M-PD` ‚Story and cases‘; `23#46E`, `23#49E` Punkt 2 |
| Hauptgeschichte | darf **gerichteter** sein als Nebenfälle, ohne sinnvolle Wahl zu verbieten; bei den Nebenmissionen „freie Wahl“ | 📋 | `M-PD`; `23#57E` Punkt 6 |
| Nebenfälle | zahlreich, hochwertig, handgeschrieben, Mehrwege-Ermittlungen; wie eigenständige ‚Monster der Woche‘-Folgen | 📋 | `M-PD`; `23#46E` („nur so kleine fälle so wie in dr serie solche filler folgen“); `23#1076A` („Nebenfälle“) |
| Dynamische Hunter-Aufträge | zusätzlich viele kleinere Aufträge, die aus der laufenden Weltsimulation entstehen, nicht geskriptet sind und „immer wieder“ vorkommen | 📋 | `23#1075E` Punkt 70; `23#1076A`; Details in [Modul 07](07-Detektivlaufbahn-und-organische-Faelle.md) |
| Mehrdeutige Fälle | eigene Klasse, siehe [Abschnitt 2](#2-die-fallklassen) | 📋 | `23#1075E` Punkt 71; `23#1062A` |

Der Spieler soll bei den Fällen **nachdenken müssen**; die Welt soll „nicht wie bei Ubisoft vollgespammt“ wirken (`23#46E`: „man soll bei den fällen nachdenken müssen“). Das ist ein Gestaltungsziel, keine Zahl.

> [!NOTE]
> Der Bezug zur Spielwelt-Regel „Kein wichtiger Ort darf als identische Kopie an anderer Stelle wieder auftauchen.“ (Vorschlag `23#48A`, zu dem der Entwickler in `23#49E` Stellung nahm: „ja“ zu mehreren Punkten, Forderung nach vielen Gebäuden in `23#46E`) gilt auch für Fallorte: Ein Fallort, eine wichtige Landmarke oder ein Story-Gebäude wird handkuratiert und nicht recycelt (`23#48A`). Siehe [Welt-Dokumentation](../World/README.md).

---

## 2. Die Fallklassen

Die Quellen unterscheiden **drei Klassen** (`23#1062A`, vom Entwickler mit „ja passt“ `23#1063E` bestätigt; `23#1061E`: „detektiv können wir noch in supernatural reingeben“):

| Klasse | Inhalt | Status |
|---|---|:---:|
| **Hunter-Fälle** | tatsächlich übernatürlich | 📋 |
| **Gewöhnliche Detektivfälle** | normale Kriminalität, Privatdetektiv-Arbeit, kein Monster | 📋 |
| **Mehrdeutige Fälle** | zunächst unklar, ob gewöhnlich oder übernatürlich; der Spieler muss „wirklich ermitteln“, statt schon beim Annehmen zu wissen, dass es ein Geist ist | 📋 |

Der Assistent ordnete das als Architektur ein: Das Ermittlungs-Framework (Beweise, Zeugen, Zeitleisten, Überwachung, Recherche, Fall-Logik) bleibt allgemein, das Übernatürliche setzt zusätzliche Spurtypen darauf (EMF-artige Hinweise, Schwefel, ungewöhnliche Temperatur, Symbole, Lore, Flüche, Monsterverhalten, Rituale; `23#1062A`). Das ist ein **technischer Vorschlag** (💡), keine Entscheidung. Die Ausarbeitung der gewöhnlichen Fälle steht in [Modul 07](07-Detektivlaufbahn-und-organische-Faelle.md).

---

## 3. Ein Fall ist ein Problem mit Auflösungen

**Die zentrale Regel** formulierte der Assistent am Ende von `23#59A` als „feste Regel“: „Der Fall definiert ein Problem und mögliche Auflösungen – nicht eine vorgeschriebene Reihenfolge von Questschritten.“ Der Inhalt entspricht den eigenen Aussagen des Entwicklers (`23#56E`, `23#57E`, siehe Abschnitt 4) und `M-PD`; der Entwickler widersprach in der folgenden Antwort `23#60E` nicht. Status: 📋 (Prinzip; die Formulierung stammt vom Assistenten).

### 3.1 Begründung und Gegenbild

Der Assistent warnte vor einer Checkliste (Tatort untersuchen, Sheriff befragen, Krankenhaus besuchen, vier Beweise finden, Monster identifizieren, Monster töten), weil „die Freiheit nur vorgetäuscht“ wäre (`23#59A`). Stattdessen besitzt ein Fall (💡 als Beispiel):

- **Wissen**: was über den Fall bekannt sein kann,
- **mögliche Ansätze**: Informationsquellen (im Beispiel Sheriff, Überlebender, Tatort, Autopsie, Zeitungsarchiv, Hunter Archive, Geräuschaufnahme, direkte Monsterbegegnung),
- **mögliche Auflösungen**: Bedingungen, bei denen der Fall als gelöst gilt.

### 3.2 Auflösungsbedingungen (Beispielstruktur)

`23#59A` zeigte drei **Beispiel**-Auflösungen als Veranschaulichung (💡):

| Beispiel | Bedingung |
|---|---|
| A | Monster korrekt identifiziert **und** passende Methode erfolgreich eingesetzt |
| B | Fluchursprung entdeckt **und** Fluch gebrochen |
| C | betroffene Person gerettet **und** Artefakt zerstört |

Die Hinweise sind „Wege zu diesem Wissen“, keine Pflichtschritte (`23#59A`). Ob das später als Datenmodell so gebaut wird, ist **offen** (❓, siehe [Modul 21](21-Konflikte-und-offene-Fragen.md)). Für den technischen Anschluss gibt es heute nur das Weltzustands-Fundament (`WorldFacts`), noch kein Fallmodell.

---

## 4. Mehrere Wege und frühes Lösen

Der Entwickler (`23#56E`, wortgleich `23#57E`): Man soll „mehrere ansätze haben können und mit allen kann man den fall lösen, jedemfalls mit einigen“. Wenn jemand meint, „nur durch hören“ genug zu wissen, soll er den Fall theoretisch abschließen können. Ausdrücklich: „keine unnötigen versteckten questziele sondern nur das was nötig ist“.

Daraus ergibt sich (📋, so auch in `M-PD` ‚Story and cases‘):

1. **Keine versteckten Pflichtziele.** Der Spieler darf einen Fall nach wenigen Hinweisen lösen, wenn diese genügen **und** der Schluss richtig ist.
2. **Unterschiedliche Informationswege** können zu einer gültigen Lösung führen.
3. **Das Spiel prüft nicht, ob eine Liste abgearbeitet wurde**, sondern ob genug getan wurde (`23#59A`, Einleitung).
4. **Dieselbe Spielerin kann denselben Fall anders spielen**, wenn die Entscheidungspunkte anders ausfallen (`23#59A`, Abschnitt 5; 💡 als Folgerung).

Beispiel des Assistenten (💡): Man erkennt auf einer Tonaufnahme eine Stimme, schließt auf Kreatur X, fährt zum Unterschlupf, rüstet sich aus und jagt. Hat man recht, ist der Fall lösbar; liegt man falsch, gibt es Folgen (`23#59A`).

---

## 5. Falsche und verfrühte Schlüsse

| Aussage | Status | Quelle |
|---|:---:|---|
| Falsche oder verfrühte Schlüsse können **Folgen** haben; die Welt läuft weiter | 📋 | `M-PD` ‚Story and cases‘; `23#1062A` |
| Statt „Falsche Antwort. Lade letzten Checkpoint.“ gilt: man kann falsch beschuldigen, Spuren übersehen, einen Fall zu früh schließen, Beweise falsch deuten, jemanden nicht finden oder zu spät kommen | 📋 | `23#1062A` Abschnitt „Ermittlungen dürfen scheitern“ |
| Beispiel: Der Spieler hält einen Fall früh für gelöst, es war die falsche Kreatur, die Lage eskaliert, es ergibt sich eine neue Ermittlungsrichtung | 💡 | `23#59A` Fall E |
| Es soll **abschaltbar** sein, dass man „dort fehlgeschlagen“ wird; stattdessen lädt man einen Checkpoint neu, „vielleicht verknüpfen wir das an den schwierigkeitsgrad“ | 🧭 (Optionswunsch; Bindung an die Schwierigkeit **offen**) | `23#57E` Punkt 3 |

Die Bewertung der Gesamtfolge („Fall verloren“, ‚Opfer gestorben‘) ist **nicht** spezifiziert. Der Master verlangt nur, dass Fehlschläge im Spielstand bleiben (siehe [Modul 04](04-Archiv-Journal-und-Hunter-Record.md#4-hunter-record-das-fortschrittsmenü)).

---

## 6. Auflösungsarten

Fälle können enden durch (📋, `M-PD` ‚Story and cases‘): töten, vertreiben, bannen, fangen, Fluch brechen, Opfer oder Wirt retten, Artefakt zerstören oder sichern, verhandeln, überleben, evakuieren, überlisten. `23#55A` ergänzt als Finalvarianten (💡): Falle bauen, Ritual durchführen, Monster verfolgen, selbst entkommen, mehrere Orte gleichzeitig berücksichtigen.

Zur Tötungsmethode (Block 9K, `23#1074A`, vom Entwickler mit „korrekt, da muss man davor research machen oder es selbst testen“ `23#1075E` Punkt 91 bestätigt):

- Research ist wichtig; manche Wesen sind ohne die richtige Methode kaum endgültig zu töten. Der Entwickler: Man muss vorher Research betreiben oder es selbst testen.
- Manche Arten haben mehr als eine wirksame Methode; Vorbereitung (Fallen, Schutzkreise, Salz, spezielle Munition, Rituale) wird belohnt; kreative Lösungen sind möglich, solange sie zur Weltlogik passen; eine falsche Methode kann dazu führen, dass ein vermeintlich getötetes Wesen zurückkommt; Überreste bestimmter Arten müssen speziell behandelt werden; manche Wesen können lebend gefangen, intelligente Wesen verhört werden (Fragen 92–98 in `23#1074A`, Antwort „94-98 ja“ `23#1075E`).

Status: 📋 als Richtung. Welche Arten welche Methode haben, ist **offen** (die Artenliste fehlt, [Entscheidungsstatus §5.4](../Entscheidungsstatus.md)).

---

## 7. Variation: kein Schema F

Der Entwickler: „es darf sich kein fall gleich anfühlen sonst ist es langweilig“ (`23#49E` Punkt 11) und „es darf nicht immer der gleiche ablauf sein sondern immer einzigartig sein“ (`23#54E`). Der Assistent formulierte daraus (`23#55A`) das **Case-Design-Prinzip**: „Wiederkehrende Systeme ja. Wiederkehrende Abläufe nein.“ Status: 📋 (Prinzip; Umsetzung offen).

Als Veranschaulichung nannte `23#55A` acht Verläufe (alle 💡, **keine** Fallliste):

| Beispiel | Verlauf |
|---|---|
| A | Zeitungsartikel, Recherche, Tatort, Jagd |
| B | zufällige Begegnung auf dem Highway, Verletzten retten, Krankenhaus, unbekanntes Problem eskaliert |
| C | NPC bittet um Hilfe, Familiengeschichte, historische Dokumente, Ritual |
| D | zufälliger Sammelobjekt-Fund, alter ungelöster Fall wird entdeckt |
| E | Das Monster greift den Spieler an, Grund unbekannt, rückwärts ermitteln |
| F | Polizei hält den Spieler für den Täter, gleichzeitig Monsterproblem lösen, ohne verhaftet zu werden |
| G | scheinbar übernatürlich, tatsächlich menschlicher Täter |
| H | scheinbar normaler Mord, wird erst sehr spät übernatürlich |

Ergänzend nannte `23#59A` Beispiel D (Sammelobjekt, alter Fall, Nachfahren, Familiengeschichte). Die Verläufe zeigen die **geforderte Streuung**, nicht verbindliche Inhalte.

---

## 8. Mischfälle, falsche Alarme und Täuschung

Aus Block 9 (Fragen 71–75 in `23#1074A`; Antworten in `23#1075E`: 72 „ja genau“, 73 „genau“, 74 „ja“, 75 „kann sehr gut sein“; bei 71 lautet die Antwort „ja genau, aber nur haupt und nebenmissionen ist das aber nicht so“, deren Sinn unklar ist; der Assistent wiederholt den Inhalt in `23#1076A`):

| Aussage | Status |
|---|:---:|
| Mehrdeutige Fälle: Anfangs weiß man nicht, ob überhaupt etwas Übernatürliches vorliegt | 📋 |
| **Falscher Alarm**: Einige vermeintliche Monsterfälle haben keine übernatürliche Ursache (Serienkiller, Tier, Betrug, psychische Krise, Inszenierung, normale Kriminalität) | 📋 (Richtung) |
| Ein Fall kann gewöhnliche Kriminalität **und** ein übernatürliches Element enthalten | 📋 (Richtung) |
| Selten können zwei unabhängige Wesen beteiligt sein | 📋 (Richtung, „selten“) |
| Der Täter kann ein Mensch sein, der das Übernatürliche als Tarnung benutzt (Kultisten, Jäger, Serienkiller) | 📋 (Richtung) |
| Hinweise können gefälscht sein; Monster oder Menschen legen absichtlich falsche Spuren | 📋 (Richtung, Frage 82 `23#1074A` mit „76-83 ja“) |

Der Anteil ist nicht festgelegt. Der Assistent schlug als Gefühl vor, dass von **20 gewöhnlichen Fällen nur einer** wirklich übernatürlich endet, damit der Überraschungsmoment wirkt (`23#1062A`); der Entwickler antwortete darauf nur mit „ja passt“ (`23#1063E`). Das ist eine **Größenordnung zur Orientierung**, keine Quote (🧭).

---

## 9. Der Einstieg ins Spiel

Aus Block 9 (`23#1076A`, Entwicklerantwort `23#1075E` Punkt 84): „am anfang weiß man noch nichts, man wird in die welt reingeworfen durch ein ereignis mit einem monster, genau weiß ich es nicht aber ungefähr so“. Die Richtung (🧭, ausdrücklich „noch nicht exakt entschieden“): Der Spieler startet fast ohne Monsterwissen, wird durch ein einschneidendes Ereignis hineingezogen, und das Hunter Archive beginnt mit Lücken ([Modul 04](04-Archiv-Journal-und-Hunter-Record.md)). Der konkrete Anfang der Hauptgeschichte steht in [Spielvision §5.2](../Spielvision.md#52-der-anfang) und wird hier nicht dupliziert.

---

## 10. Niederlage, Verletzung und Fallfolgen

Dieser Abschnitt bündelt nur, was die **Ermittlung** berührt. Das Verletzungssystem selbst steht in [Gameplay-Systeme §4](../Gameplay-Systeme.md#4-verletzungen-und-medizin), Tod und Scheitern in [Spielvision §14](../Spielvision.md#14-schwierigkeit-scheitern-und-tod).

| Aussage | Status | Quelle |
|---|:---:|---|
| Verwundung wird behandelt (verbinden, desinfizieren, warten), kein „Energy Drink“; der Tod darf **kein bloßer Checkpoint-Neustart** sein, ist aber abschaltbar | 📋 | `23#60E` Punkt 1 |
| Eine Niederlage ist **nicht gleich Fallverlust**: „gleich niederlage wäre zu hart, weil dann wäre das spiel zu schwer“; die Folgen (Fall verändert sich, Opfer stirbt, Monster flieht oder wechselt das Versteck, Ausrüstung geht teilweise verloren, Kosten, Tage vergehen, Verletzungen bleiben) sind „gut“ | 📋 | `23#62E`; Folgenliste `23#61A` (💡 als Aufzählung) |
| Gestufte Verletzung (gesund bis kampfunfähig); erst im kritischen Zustand greift eine situationsabhängige Konsequenz; ein Fall bleibt nach einer Flucht aktiv, die Lage kann sich ändern (Monster wechselt das Versteck, weiteres Opfer, Polizei sperrt ab, Zeuge verschwindet, Monster wird vorsichtiger) | 💡 (Vorschlag; der Entwickler stimmte den **Folgen** zu: „aber die folgen sind gut“; keine Einzelbestätigung der Stufen) | `23#63A`; `23#62E` |
| Der Tod bleibt „besonders“, selten, „was genau danach passiert, entscheiden wir später“; kein „Game Over“, nur der Tod | 📋 (kein Game Over) / ❓ (Ablauf) | `23#63A`; `23#1075E` Punkte 144–146 („game over soll es nicht geben, nur den tod“) |
| Härtester Modus: kleiner Fehler, Krankenhaus; der Tod kann viel leichter passieren | 📋 | `23#64E` Punkt 1 |
| Im Block 9 des Masters ist der **genaue Ablauf des Spielertods ausdrücklich offen** | ❓ | `M-B09`; `M-CA0905` Abschnitt 7 |

Wahlmöglichkeit für die Konsequenzstärke (💡, Vorschlag des Assistenten `23#61A`): „Persistent / Reduziert / Checkpoint“. Der Entwickler bestätigte nur die Richtung der Folgen (`23#62E`). Die Bezeichnungen sind nicht festgelegt.

---

## 11. Ergänzungen aus der Quellenauswertung

Dieser Abschnitt trägt Quellenfunde nach, die bei der systematischen Auswertung von Chat 23 (Nachrichten 66–1055) zum Vorschein kamen und in den früheren Abschnitten fehlten. Er ändert keine frühere Aussage; Abweichungen stehen in [Modul 21](21-Konflikte-und-offene-Fragen.md).

### 11.1 Niederlage und Tod: die acht Ansätze vom 20.08.2026

Der Assistent stellte acht Ansätze für den Tod des Spielers vor (`23#87A`); der Entwickler reagierte auf jeden einzeln (`23#88E`). Der Entwickler selbst hatte vorher geschrieben: „tod definieren wir noch, aber gib mal ein paar ansätze“ (`23#86E` Punkt 14).

| Ansatz (Assistent, `23#87A`) | Inhalt | Reaktion des Entwicklers (`23#88E`) | Status |
|---|---|---|:---:|
| **A** Klassischer Reload | Tod, letzter Checkpoint | „kann man in den einstellungen andrehen, wenn man es nicht so realistisch haben will“ | 📋 als **Option** |
| **B** Krankenhaus und Zeitverlust | Kampfunfähig, gerettet, Krankenhaus, Zeit vergeht, Welt läuft weiter | „nicht so schlechtes design, aber das ist dann wie in gta also zu leicht“ | 🧭 (mit Vorbehalt) |
| **C** Rescue-System | Folge hängt von der Situation ab (Begleiter, Passant, Polizei, allein im Wald) | „hört sich gut an, nur weiß man nicht was passiert wenn man wirklich stirbt“ | 🧭 (Zustimmung mit offener Frage) |
| **D** Monsterabhängige Niederlagen | Monster lässt zurück, entführt, verflucht, besitzt oder tötet | „sehr interessant“ | 🧭 |
| **E** Tod bleibt Tod, Welt zurückgesetzt | letzter vollständiger Save | „gefällt mir nicht, weil das wie permadeath ist und das optional sein sollte“ | abgelehnt als Standard |
| **F** Dark-Souls-artige Recovery | Ausrüstung am Todesort | „könnte man aber es soll kein souls game sein“ | nicht als Standard |
| **G** Near-Death vor echtem Tod | gestufter Zustand; Rettung möglich | „nicht schlecht“; Beispiel: den Kumpel anrufen, der rettet einen, man wacht im Spital auf; „das mit grounded auch nicht schlecht“ | 🧭 |
| **H** Todesursache aus Verletzung | Tod ergibt sich aus Blutvolumen, Trauma, Organschaden, Schock, Bewusstsein | „ist sowieso klar dass man das haben soll“ | 📋 Richtung |

Der Assistent nannte als Favoriten einen Hybrid aus B, C, D, G und H mit einstellbaren Todesfolgen (Story: Checkpoint; Normal: reduzierte Folgen; Survival: persistente Folgen; Grounded: schwer; `23#87A`, 💡).

### 11.2 Weitere Aussagen zum Tod (Chat 23, 25.08.2026)

| Aussage | Status | Quelle |
|---|:---:|---|
| Gameplay-„Downed“ ist noch kein Tod; klinischer Tod ist nicht automatisch endgültig; Permadeath ist **optional**, nie Standard | 💡 des Assistenten (Wiedergabe früherer Festlegungen) | `23#361A` Punkte 2–3, 26 |
| Für **normales Gameplay**: echter Tod führt zum letzten Checkpoint oder Save; Near-Death und Rettung nur, wenn die Situation sie plausibel zulässt | 💡 Vorschlag, vom Entwickler bestätigt („ja das wollte ich sagen, es darf nicht im spiel so extrem sein, da man sonst den spaß verliert“) | `23#361A` Frage D; `23#362E` |
| Man soll einen Hauptcharakter haben und nicht bei jedem Tod einen neuen; es bleibt ein Hauptcharakter plus Begleiter | 📋 | `23#362E` Punkte 1 und 6 |
| Der Schwierigkeitsgrad ändert die kosmologischen Regeln des Todes nicht | 💡 | `23#361A` Punkt 25 |
| NPC-Hunter haben keine Plot Armor und können sterben; Tod erzeugt Weltzustand (Beziehungen, Wohnort, Familie, Kontakte, Archiv, Dialoge, künftige Fälle) | 💡 / Richtung | `23#361A` Punkte 29–30 |
| Beerdigungen bei wichtigen Personen | 💡 | `23#361A` Punkt 31 |

Die Antworten des Entwicklers in `23#362E` reichen nur bis Punkt 22 (und die Fragen A bis D); die Punkte 23 bis 32 des Assistenten (Tod des Spielers, Permadeath, Krankenhaus, Partner, andere Hunter, Weltzustand, Beerdigung) sind daher **nicht einzeln beantwortet** und als Vorschläge geführt.

### 11.3 Verhältnis zur Entscheidung vom Oktober

Im Oktober wählte der Entwickler für Tod und Respawn Krankenhaus mit beschleunigter Heilung und eine klassische Einstellung „wie D“ (`48#117E`; Option D der Frage `48#116A`: unterschiedliche Spielmodi mit klassischem Respawn, Verletzungs- und Rettungssystem und optionalem Permadeath). Damit sind **drei Stände** zu vergleichen (K-E12 in [Modul 21](21-Konflikte-und-offene-Fragen.md)): (1) 20.08.: Ansätze, Standard offen; (2) 25.08.: normales Gameplay Checkpoint/Save, Near-Death wo plausibel; (3) 10.10.: Krankenhaus und Rettung als Standard, klassischer Respawn und Permadeath als Spielmodi. Der jüngste Stand gilt, soweit er ausdrücklich ist; der **genaue Ablauf** bleibt offen (`23#86E`: „tod definieren wir noch“; `M-B09`).

### 11.4 Stabile Story-Wirbelsäule (29.08.2026)

Der Entwickler (`23#582E`): In der Story sollen nicht so viele Dinge passieren können, weil das die Geschichte zu sehr beeinflusst; keine Storymissionen und Nebenmissionen sind verpassbar, ‚außer man hat irgendwelche Leute nicht retten können‘, aber nicht so extrem und nicht so oft; wenn jemand stirbt, dann bei jedem Spieler; Storymissionen sind auf verschiedenen Wegen lösbar, aber **wichtige Hauptcharaktere sterben nicht**, nur NPCs können; man kann Informationen verlieren, „aber keinen riesigen einfluss“. Die Ausformulierung (`23#583A`, 💡, vom Entwickler bestätigt mit `23#584E`: „Deine punkte passen sehr gut“):

| Punkt | Inhalt |
|---|---|
| Feste Story-Spine | Aktstruktur, Kapitel, große Antagonisten, zentrale Crew-Ereignisse, wichtige Todesfälle, kosmologische Ereignisse, finales Ziel |
| Variable Oberfläche | Missionsreihenfolge innerhalb erlaubter Bereiche, Ermittlungsweg, Vorgehensweise, Dialoge, Informationsstand, kleinere NPC-Schicksale, optionale Ziele, Beziehungen, Belohnungen |
| Hauptmissionen | nie dauerhaft verpassbar |
| Nebenmissionen | normalerweise nicht verpassbar; bleiben nach Entdeckung verfügbar; wenn die Welt sich logisch so ändert, dass der Fall nicht mehr existieren kann: „Original Case → World State changed → Updated Case Variant“ statt Löschen |
| Informationen | man kann Informationen verpassen, falsche erhalten oder einen schlechteren Weg nehmen; wichtige Informationen können später über andere glaubwürdige Wege verfügbar werden |

Das stützt die Prüfregeln V1 bis V3 ([Modul 09 §13](09-Falllebenszyklus-und-Generierung.md#13-prüfregeln-für-fallautoren-entwurf)) und begrenzt die Folgen falscher Schlüsse (Abschnitt 5): Sie treffen den **Weg**, die Opfer und kleinere NPCs, nicht die Hauptgeschichte. Der Entwickler ergänzte zu einem Beispiel: eine Person kann verpassbar sein, wenn sie stirbt, aber „wenn man paar nebenmissionen macht bekommt man das fix“ (`23#607E` Punkt 5).

### 11.5 Freischaltung durch Story-Zustand

Der Entwickler (`23#578E` Punkt 9): Der Spieler kann schon an einem Ort gewesen sein, „aber nicht für story oder nebenmissionen, aber die welt steht einem offen“; die Story hat mehrere Akte oder Kapitel. Und (`23#580E`): manche Haupt- und Nebenmissionen entstehen erst, wenn vorherige Missionen abgeschlossen sind, „wie in rdr2 aber noch besser und komplexer“. Der Assistent unterschied drei Klassen von Nebenfällen (`23#579A`, 💡): `WORLD_CASE` (unabhängig von der Hauptstory), `CHAPTER_CASE` (durch einen Story-Zustand plausibel), `CONSEQUENCE_CASE` (aus einer Entscheidung oder einem früheren Fall); Freischaltung über Story State, Akt und Kapitel, **nicht** über unsichtbare geografische Schranken. Die Abhängigkeiten regeln, **wann und in welchem Zustand** eine Mission verfügbar wird, nicht, ob die Hälfte der Story verschwindet (`23#583A`).

### 11.6 Begegnungen müssen nicht immer Kampf sein

Der Assistent (`23#293A` Punkt 17): ein Wesen aus der Entfernung beobachten, verfolgen, belauschen, fotografieren, markieren, Proben sammeln, absichtlich nicht angreifen; im Detektivmodus wäre ein vorschneller Angriff manchmal eine schlechte Idee. Der Entwickler: „ja das ist sogar wichtig“ (`23#294E` Punkt 17). Status: 📋. Das ist eine Bedingung an die Fallgestaltung: Ermittlung am Wesen selbst (Beobachtung, Proben) muss ohne Kampf möglich sein (vgl. [Modul 11 §9.4](11-Forensik-und-Labor.md#94-experimente)).

### 11.7 Fälle sind die Missionen

Der Entwickler (`35#87E` Punkt 5): „die fälle sind die missionen außer man hat gespräche mit charakteren“; Missionen in andere Gebiete können lange dauern, sonst eher kürzer. Status: 📋 (Gleichsetzung von Fall und Mission für die normale Spielzeit).

---

## 12. Was fehlt
- Eine **Fallliste** oder ein Fallkatalog (nichts im Master).
- Die **Datenstruktur** eines Falls und die Bewertung ‚gelöst/falsch gelöst‘ (nur Beispiele).
- Die Zuordnung **Monsterart zu Methode** (keine Artenliste).
- Quoten für gewöhnliche gegenüber übernatürlichen Fällen.
- Die **Fallfolgen** bei Misserfolg als verbindliche Liste.
- Der konkrete Ablauf von **Niederlage und Tod**.

Siehe die Fragen in [Modul 21](21-Konflikte-und-offene-Fragen.md).
