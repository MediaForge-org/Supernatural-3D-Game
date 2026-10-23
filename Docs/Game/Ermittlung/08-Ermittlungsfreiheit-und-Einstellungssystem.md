# Ermittlung 08: Ermittlungsfreiheit und Einstellungssystem

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt die Entscheidungen des Quiz vom 09.10.2026 (`48#`) zur Ermittlungsfreiheit, zum Notiz- und Hilfesystem und zu Bot und Lösungsbuch und führt sie mit den älteren Entscheidungen zusammen. Abschnitte mit „Entwurf“ im Titel sind **unverbindliche Vorschläge** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Ergänzt [Modul 03](03-Ermittlungsmodi-und-Hilfen.md) (Detektivmodus, Hilfen). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Die Entscheidungen im Quiz](#1-die-entscheidungen-im-quiz)
2. [Zusammenführung mit den älteren Aussagen](#2-zusammenführung-mit-den-älteren-aussagen)
3. [Einstellungstiefe: was der Entwickler verlangt](#3-einstellungstiefe-was-der-entwickler-verlangt)
4. [Entwurf: Einstellungsachsen der Ermittlung](#4-entwurf-einstellungsachsen-der-ermittlung)
5. [Entwurf: Voreinstellungen (Presets)](#5-entwurf-voreinstellungen-presets)
6. [Entwurf: Wechsel der Einstellung mitten im Fall](#6-entwurf-wechsel-der-einstellung-mitten-im-fall)
7. [Ermittlungswerkzeuge im Spiel](#7-ermittlungswerkzeuge-im-spiel)
8. [Bot, Lösungsbuch und Companion-Wissen](#8-bot-lösungsbuch-und-companion-wissen)
9. [Sonderfälle und offene Wechselwirkungen](#9-sonderfälle-und-offene-wechselwirkungen)
10. [Auswirkungen auf andere Systeme](#10-auswirkungen-auf-andere-systeme)
11. [Ergänzungen aus der Quellenauswertung](#11-ergänzungen-aus-der-quellenauswertung)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Die Entscheidungen im Quiz

### 1.1 Komplexität der Ermittlung

| Punkt | Inhalt | Quelle |
|---|---|---|
| Frage | „Wie komplex sollen übernatürliche Ermittlungen sein?“ (Beispielfrage, ausdrücklich als Beispiel gestellt) | `48#58A` |
| Antwort | „Extrem tiefgehend mit Forensik und eigenen Schlussfolgerungen“ (Option C der Beispielfrage) | `48#59E` |
| Bestätigung (Assistent) | Hunter-Ermittlungen „extrem tiefgehend“, mit „echter Forensik, Beweisanalysen, widersprüchlichen Zeugenaussagen, falschen Fährten und eigenen Schlussfolgerungen“ | `48#60A` |

Status: 📋 (Antwort des Entwicklers). Die Aufzählung im Bestätigungstext (widersprüchliche Zeugen, falsche Fährten) ist die Ausformulierung des Assistenten (💡), deckt sich aber mit `M-PD` und den Entscheidungen aus Chat 23 (siehe [Modul 05](05-Spuren-Tracking-Forensik-und-Zeugen.md)).

### 1.2 Ermittlungsfreiheit

Frage `48#60A`: „Wie viel Freiheit soll der Spieler bei seinen Schlussfolgerungen haben?“ Optionen:

| Option | Inhalt |
|---|---|
| A | Geführte Ermittlungen: Hinweise selbst finden, aber das Spiel führt zur richtigen Lösung |
| B | Echte Fehler möglich: falsche Verdächtige und Monsteridentifikationen mit Konsequenzen |
| **C** | **Vollständig freie Ermittlungen: keine automatische Auflösung, der Spieler muss selbst kombinieren** |
| D | Dynamisches System: Fälle mit verschiedenen Freiheitsgraden, echten Fehlern und Konsequenzen |

Antwort `48#61E`: „Es soll C sein aber kann kann es in den einstellungen einstellen das man mehr oder weniger tipps bekommt. Man kann questmarker auch komplett abschalten und sich alles selber notieren dafür soll es dann natürlicu auch verschiedene sachen geben damitnman nicht mitnstift und papier aufschreiben muss das ist ein spiel und soll großteils spaß machen“.

| Aussage | Status |
|---|:---:|
| Grundsatz ist **C**: vollständig freie Ermittlung; das Spiel verrät nicht automatisch, welches Monster verantwortlich ist oder wie der Fall gelöst wird | 📋 |
| Die Menge der Tipps ist in den Einstellungen **einstellbar** (mehr oder weniger) | 📋 |
| Questmarker lassen sich **komplett abschalten**; der Spieler notiert selbst | 📋 |
| Dafür gibt es Werkzeuge, damit man nicht ‚mit Stift und Papier‘ schreiben muss; das Spiel „soll großteils Spaß machen“ | 📋 (Zielsatz) |
| Der Fall bleibt frei; Hilfen sind optional | 📋 |

Wichtig für die Auslegung: Die Antwort wählt C **und** erlaubt Hilfen. Es gilt also **nicht** ‚D (dynamisch je Fall)‘. Die Freiheitsgrade unterscheiden sich nach **Spielereinstellung**, nicht automatisch nach Fall. Ob einzelne Fälle trotzdem verschiedene Freiheitsgrade haben dürfen, ist nicht beantwortet (❓ Q-E32).

### 1.3 Notizsystem

Frage `48#62A`: „Wie sollen eigene Notizen funktionieren?“ Optionen A (automatisch), B (manuell), C (Hybrid), D (vollständig anpassbar mit Automatik, manuellen Notizen, Sprachnotizen, Beweisverknüpfungen).

Antwort `48#63E`: „Man soll alles einstellen können wie man es will es wird nicht nur wählbar sein an oder aus sondern so viele einstellungen mehr noch als bei tlou2 bei den sachen. Wir haben ja später noch den bot und auch das lösungsbuch da muss dann alles drinnen stehen“.

| Aussage | Status |
|---|:---:|
| Alles ist **einstellbar**, nicht nur Ein/Aus; mehr Einstellungen als in *The Last of Us Part II* | 📋 |
| Bot und Lösungsbuch (später) müssen **alles** enthalten | 📋 |
| Die gewählte Notizvariante entspricht dem Geist von D (vollständig anpassbar) | 🧭 (Zuordnung durch den Wortlaut; die Option wurde nicht ausdrücklich benannt) |

Der Assistent nannte daraufhin das Ziel „Advanced Customization & Assistance System“ mit den Ebenen normale Einstellungen, detaillierte Expertenoptionen und vollständige benutzerdefinierte Profile sowie Suchfunktion, Presets und Profile (`48#64A`). Das ist eine **Ausformulierung** (💡); ausdrücklich bestätigt ist nur die Aussage von `48#63E`.

---

## 2. Zusammenführung mit den älteren Aussagen

| Älterer Stand | Neuer Stand (Oktober) | Verhältnis |
|---|---|---|
| Vier Hilfestufen vorgeschlagen (`23#51A`), vom Entwickler zu ‚Detektivmodus ohne Hilfen‘ geändert (`23#52E`) | Grundsatz C plus einstellbare Tipps (`48#61E`) | vereinbar: Der Detektivmodus ist die **Ausprägung ohne jede Hilfe**, die Einstellungen ergänzen die Zwischenstufen |
| Ermittlungshilfe unabhängig von Kampfschwierigkeit (`M-PD`, `23#51A`) | „so viele Einstellungen … mehr noch als bei TLOU2“ (`48#63E`) | vereinbar und verschärft: Es geht um **viele einzelne Regler** |
| Journal und Archiv zeigen nur Erfahrenes (`23#55A`) | Notizen, Verknüpfungen, Sprachnotizen als Optionen (`48#62A` Optionen) | vereinbar |
| Hilfe auch **abschaltbar** („wie ein depp der markierung folgen“, `23#49E`) | Questmarker komplett abschaltbar (`48#61E`) | vereinbar: Marker sind **ein-** und **ausschaltbar** |

Die Zwischenstufen (Namen, Zahl) bleiben **offen**; `23#51A` bleibt als überholter Vorschlag stehen ([Modul 21](21-Konflikte-und-offene-Fragen.md), K-E5).

---

## 3. Einstellungstiefe: was der Entwickler verlangt

1. **Vergleichsreferenz** ist *The Last of Us Part II* (`48#63E`; auch `23#64E` für die Vielzahl der Schwierigkeitsgrade). Gefordert wird **mehr** davon.
2. **Unabhängige Einzeloptionen** (`48#64A` als Ziel formuliert: „möglichst unabhängigen Einzeloptionen“, Entwickler zustimmend durch Schweigen und Folgefragen; als Anforderung 🧭).
3. **Alle Hilfen gehören zum Wissen von Bot und Lösungsbuch**: Was eine Einstellung bewirkt, muss dort nachlesbar sein (`48#63E`).
4. **Alles ist konfigurierbar**, wenn man das Spiel realistischer oder weniger realistisch haben will (`23#64E`; gilt für das ganze Spiel).
5. Das Spiel soll ausdrücklich **nicht einfach** sein, „außer man will es einfach“ (`23#64E`).

---

## 4. Entwurf: Einstellungsachsen der Ermittlung

> [!NOTE]
> **Nur Vorschlag (💡).** Der Master und der Entwickler legen keine Achsenliste fest. Die Achsen unten leiten sich aus den **entschiedenen Verboten und Erlaubnissen** ab ([Modul 03](03-Ermittlungsmodi-und-Hilfen.md)) und aus den Konzeptbeispielen von `48#62A` und `48#64A`. Jede Achse nennt ihren Ursprung.

Jede Achse ist **unabhängig** von den anderen und von der Kampfschwierigkeit. Wertebereiche sind Beispiele.

| Nr. | Achse | Mögliche Werte (Vorschlag) | Ursprung |
|---|---|---|---|
| A1 | Hinweisangebot | aus / nur auf Anfrage / gelegentlich / umfangreich | Beispiel `48#62A` („Hinweissystem“) |
| A2 | Hinweisintensität (stufenlos) | 0–100 % in Schritten | Beispiel `48#64A` (Schieberegler 0–100 %, Schritt 5) |
| A3 | Questmarker | komplett aus / nur eigene / ungefähre Suchgebiete / genaue Ziele, soweit bekannt | Beispiele `48#62A`, `48#64A` |
| A4 | Anzeige des verfolgten Falls | nur Name und Status / zusätzlich Kurzstand / aus | `M-PD`: Name und Status dürfen sichtbar sein; `48#61E` |
| A5 | Objekthervorhebung | aus / bei Fokus-Fähigkeit / dauerhaft | Verbot im Detektivmodus (`M-PD`); Hunter-Vision/Focus als Idee `23#51A` |
| A6 | Automatische Theorie-Vorschläge | aus / auf Anfrage / an | Beispiel `48#64A` (Kontrollkästchen) |
| A7 | Beweisverknüpfung | nur manuell / Vorschläge / automatisch | Verbot automatischer Verknüpfung (`M-PD`) |
| A8 | Beweiszähler (‚3/4 gefunden‘) | aus / an | Verbot im Detektivmodus (`M-PD`, `23#55A`) |
| A9 | Dokumentationsmodus | manuell / hybrid / automatisch / benutzerdefiniert | Beispiel `48#64A`; Optionen `48#62A` |
| A10 | Gespräche als Transkript speichern | an / aus | Beispiel `48#64A` |
| A11 | Beweise automatisch archivieren | an / aus | Beispiel `48#64A` |
| A12 | Sprachnotizen | an / aus | Option D in `48#62A` |
| A13 | Zeugenaussagen per Knopfdruck ins Journal | an / aus | Aussage des Assistenten `48#62A` („Man soll beispielsweise Zeugenaussagen per Knopfdruck ins Journal übernehmen können“), als Komfort gedacht |
| A14 | Eigene Kartenmarker | an / aus | `M-PD`, `23#55A` („Der Spieler setzt den Marker, nicht das Spiel.“) |
| A15 | Navigation zu selbst gesetzten Markern | keine / Richtungsanzeige / Route | `23#55A` (Navigation komfortabel, ohne zu lösen) |
| A16 | Hilfe des Bots (extern) | aus / nur Regeln erklären / Hinweise / Lösung | `48#64A`; `48#73E` |
| A17 | Konsequenzen falscher Schlüsse | Standard / reduziert / Checkpoint-Neuladen | `23#57E` Punkt 3; `23#61A` |
| A18 | Referenzweg nach Abschluss anzeigen | aus / an | `23#64E` Punkt 2 |
| A19 | Companion-Wissen im Gespräch | nur Bestätigtes / auch Vermutungen gekennzeichnet | `48#126A` (Hinweis, dass Unbestätigtes nicht als Wahrheit dargestellt wird) |
| A20 | Spoilerstufe der Companion-App | klein / mittel / vollständig | `48#64A`, `48#73E` |

### 4.1 Regeln zwischen den Achsen (Vorschlag)

R1. **Abhängigkeit statt Verbot.** Eine Achse darf eine andere nicht „kaputt“ machen. Beispiel: A6 (Theorie-Vorschläge) setzt A7 (Beweisverknüpfung) mindestens auf „Vorschläge“ voraus; sonst gäbe es nichts zu schlagen. Das Menü soll solche Fälle erklären statt sie auszublenden.

R2. **Ein Schalter, der Wissen verrät, steht nie in der Standardposition.** Alle Achsen, die Lösungswissen erzeugen (A3 ‚genaue Ziele‘, A5 „dauerhaft“, A6 „an“, A7 „automatisch“, A16 „Lösung“), sind in der Standardeinstellung aus. Das ist ein Gestaltungsprinzip, keine Zahl.

R3. **Hilfe zeigt nur, was die Spielwelt weiß.** Jede Hilfe darf nur auf Daten zugreifen, die der Spieler im Spiel erworben hat (Wissensstand), nicht auf die verborgene Fallwahrheit. Ausnahme ist nur, wenn der Entwickler eine Achse ausdrücklich als ‚Lösungshilfe‘ definiert (A16 „Lösung“). Dieser Grundsatz entspricht der Regel „Der Bot darf nicht automatisch die Lösung verraten“ (`48#64A`) und der Regel des Archivs ‚Wissen, nicht Debug-Ansicht‘ (`M-PD`).

R4. **Die Hilfe wirkt nur auf die Darstellung**, nicht auf den Fall (vgl. `23#51A`: „Der gesamte Fall bleibt derselbe.“). Es gibt keine Änderung der Fallwahrheit durch Hilfseinstellungen.

R5. **Suche und Profile.** Alle Achsen sind durchsuchbar und in benannten Profilen speicherbar (`48#64A` als Ziel).

### 4.2 Was die Achsen nicht entscheiden

- Die **Zahl** der Achsen: Die Tabelle ist eine Anfangsliste, keine Obergrenze.
- Die **Bezeichnungen** der Werte.
- Ob einzelne Achsen **je Spielmodus** (Einzelspieler, Koop, Online) abweichen. Siehe [Modul 18](18-Koop-Ermittlung.md).

---

## 5. Entwurf: Voreinstellungen (Presets)

> [!NOTE]
> **Nur Vorschlag (💡).** Namen und Inhalt der Voreinstellungen sind nicht beschlossen. Der **Detektivmodus** ist als Name und Inhalt beschlossen (`23#52E`, `M-PD`).

| Preset (Arbeitsname) | Zweck | Wirkung auf die Achsen |
|---|---|---|
| **Detektiv** (beschlossen) | hilfefrei | A1 aus, A2 0 %, A3 „nur eigene“ oder „aus“, A5 aus, A6 aus, A7 manuell, A8 aus, A16 aus, A19 ‚nur Bestätigtes‘; A4 zeigt nur Name und Status (`M-PD`) |
| Unterstützt (Arbeitsname) | etwas Hilfe | A1 auf Anfrage, A3 ungefähre Suchgebiete, A7 Vorschläge |
| Geführt (Arbeitsname) | deutliche Hilfe | A1 gelegentlich, A3 genaue Ziele, A5 bei Fokus-Fähigkeit, A7 Vorschläge |
| Story (Arbeitsname) | Fokus auf Erzählung | weitgehend automatisch (A7 automatisch, A9 automatisch) |
| Benutzerdefiniert | eigenes Profil | alle Achsen frei |

Die Zwischenpresets stehen nur als Hilfe zum Verständnis; die Quelle von `23#51A` (Investigativ, Unterstützt, Geführt, Sehr geführt) ist **überholt** (🕰️) und wird nicht übernommen.

---

## 6. Entwurf: Wechsel der Einstellung mitten im Fall

> [!NOTE]
> **Nur Vorschlag (💡).** Es gibt keine Entscheidung, ob und wie Einstellungen während eines Falls geändert werden dürfen. Der Entwurf zeigt, **welche Fragen** sich stellen. Eine Entwurfsvorgabe der Prompt-Sammlung ist ausdrücklich: Änderungen der Schwierigkeit oder Barrierefreiheit ‚must not silently rewrite saved gameplay truth‘ (`M-P03`, Schablonentext der Arbeitspakete, Status Entwurf).

| Fall | Frage | Vorschlag |
|---|---|---|
| Hilfe wird **verstärkt** (zum Beispiel A3 von „aus“ auf ‚genaue Ziele‘) | Darf das Spiel Zielmarker für Wissen zeigen, das der Spieler hat? | Ja, aber nur für **bekanntes** Wissen (R3). Ein Marker erscheint nur für Orte, die der Spieler im Fall kennt. |
| Hilfe wird **verringert** | Verschwinden bereits gesetzte Hilfen? | Ja, die Darstellung verschwindet; **selbst gesetzte** Marker und Notizen bleiben. |
| Auf **Detektiv** wechseln | Was passiert mit automatisch erstellten Einträgen? | Sie bleiben im Journal (es sind erlangte Tatsachen). Zukünftige Einträge folgen A9. |
| Hilfe-Wechsel **nach Abschluss** | Beeinflusst das Auszeichnungen? | **Offen** (❓ Q-E33): Der Master legt nicht fest, ob Hilfestufen Medaillen oder Erfolge beeinflussen. `M-OD`: ‚Exact medal weighting/scoring formula‘ ist offen. |
| Wechsel im **Koop** | Wer darf ändern? | Siehe [Modul 18](18-Koop-Ermittlung.md). |

Die Grundregel: **Einstellungen ändern die Darstellung, nie die Wahrheit des Falls und nie das, was der Spieler erfahren hat.**

---

## 7. Ermittlungswerkzeuge im Spiel

Der Assistent stellte vier Werkzeuge als Konzept vor (`48#62A`; „Konzeptvorschau“, 💡), die Entwicklerantwort bestätigte die Richtung (`48#63E`, nicht die Einzelheiten):

| Werkzeug | Inhalt laut Konzept |
|---|---|
| Hunter-Journal | eigene Notizen, Aussagen dokumentieren, Monsterinformationen sammeln, Verdachtsmomente festhalten, Fälle verknüpfen |
| Interaktive Ermittlungswand | Beweise, Fotos, Personen und Orte frei anordnen und verbinden; eigene Theorien, ohne dass das Spiel die Lösung markiert |
| Forensik und Beweisarchiv | Spuren sichern, Proben analysieren, Fotos vergleichen, Audioaufnahmen speichern, Ergebnisse jederzeit abrufen |
| Eigene Kartenmarkierungen | Orte markieren, Wegpunkte setzen, Verdachtsgebiete einzeichnen, Routen planen, ohne Questmarker |

Zusätzlich: Die Bedienung soll mit **Tastatur und Maus sowie mit Controller** angenehm sein (`48#62A`). Das passt zur Vorgabe „100 % Controller-Unterstützung“ (`23#60E`).

Die **Ermittlungswand** ist ein Vorschlag des Assistenten; der Entwickler sagte nichts Gegenteiliges. Ob sie so gebaut wird, ist **offen** (❓). In [Modul 04](04-Archiv-Journal-und-Hunter-Record.md) steht die ältere Beispielstruktur des Falljournals; die Wand ist eine mögliche Darstellung derselben Daten (Beweise, Personen, Orte, Verknüpfungen).

Die Karte der Companion-App zeigt nur, was auch im Spiel sichtbar wäre (`48#71E`: „aber ich will nicht bei der livemap das man alle collectibles sieht man sieht nur das was man auf der in game mal auch sehen würde“).

---

## 8. Bot, Lösungsbuch und Companion-Wissen

Dieser Abschnitt hält fest, was für die **Ermittlung** daran wichtig ist. Das Companion-System als Ganzes ist ein eigener Themenbereich (nicht Teil dieses Ordners).

### 8.1 Entscheidungen des Entwicklers

| Aussage | Status | Quelle |
|---|:---:|---|
| Es gibt eine **Website mit dem Bot**, eine **Handy-App** und ein **Kompendium mit „wirklich allem“**; Zugriff setzt das gekaufte Spiel voraus; bei einer nicht legalen Kopie kann man den Zugriff separat kaufen (einmalig 10–30 EUR, „irgendwie so“); **kein Online-Zwang** | 📋 (Preis und Schlüsselverfahren **offen**, ‚weiß ich noch nicht genau‘) | `48#65E` |
| Das **Lösungsbuch** ist „noch mal extra“, es gibt es physisch (Softcover, Hardcover) und als PDF; das PDF ist im Kaufpreis enthalten | 📋 | `48#65E` |
| Das Kompendium soll **vollständig herunterladbar** sein; dazu eine **lokale KI**, die nur über das Spiel Bescheid weiß; der Bot soll den **Spielstand abrufen** können („wieso komme ich dort nicht weiter“) | 📋 | `48#67E` |
| Die Companion-App zeigt auch die **Live-Karte**; sie ist „extrem riesig“, auch im Browser, mit gesammelten Sammelobjekten | 📋 | `48#69E` |
| Das **Lösungsbuch dient zum Lösen**: Dort kann man nicht einstellen, dass man keine Spoiler haben will; das geht nur beim **Companion** | 📋 | `48#73E` |
| Die KI soll **nichts für den Spieler erledigen**: „ich will grundsätzlich nicht das die ki einem arbeit macht, vielleicht machen wir das als mod oder so“; die KI nur **extern** | 📋 | `48#81E` |
| Die KI darf theoretisch handeln, aber nur mit Zustimmung und nach Einstellung; normalerweise nur Erklären | 📋 (durch `48#81E` eingeschränkt) | `48#79E`, `48#81E` |
| Die Companion-KI soll **alles unterstützen, was im Spiel ist** | 📋 | `48#77E` |
| Das Spielarchiv der Companion-App soll maximal konfigurierbar sein (Datenschutz, Speicheroptionen) | 📋 | `48#83E` |
| Im Koop: „Gemeinsames Hunter-Team“ mit geteilten Ermittlungsakten, Beweisen, Kartenmarkierungen, Teamfortschritt | 📋 (Option C) | `48#91E` zu `48#90A` |

### 8.2 Folgerungen für die Ermittlung (Entwurf)

1. **Zwei Hilfsebenen**: Das Lösungsbuch ist **immer** vollständig und nicht spoilerfrei (`48#73E`). Der Companion ist **einstellbar**. Der Detektivmodus im Spiel verhindert nur Hilfen **im Spiel**; er sperrt weder Bot noch Buch. Wer den Detektivmodus wirklich „echt“ spielen will, verzichtet freiwillig auf externe Hilfen. Das Spiel kann den Bot nicht zwingend sperren (Folgerung, 💡).
2. **Spielstandabruf des Bots** liest den **Wissensstand des Spielers**, nicht die Fallwahrheit (vgl. R3 in Abschnitt 4.1). Das ist eine Designvoraussetzung, sonst würde ‚Wieso komme ich nicht weiter?‘ die Lösung verraten. Hier ist die Quelle `48#126A`: Die Companion-KI darf unbestätigte Informationen „nicht einfach als gesicherte Wahrheit präsentieren“ (Aussage des Assistenten, 💡, passend zu `48#125E`).
3. **Spoiler und Detektivmodus** sind getrennte Dinge: Spoiler betreffen Story und Lösung, der Detektivmodus betrifft Hilfen im Spiel.
4. **Offen**: ob der Bot Hypothesen des Spielers bewerten darf (❓ Q-E34), ob die Antworten des Bots für Medaillen als Hilfe zählen (❓ Q-E33), und wie das mit dem Spielstand im Koop zusammenspielt.

---

## 9. Sonderfälle und offene Wechselwirkungen

| Sonderfall | Beschreibung | Stand |
|---|---|---|
| Mehrere Profile | Zwei Spieler am selben Spielstand (zum Beispiel Koop-Gäste) mit unterschiedlichen Hilfen | ❓ (siehe Modul 18) |
| Hilfe erzeugt Widerspruch | Ein Marker zeigt auf einen Ort, an dem der Fall inzwischen nichts mehr bietet (Monster zog weiter) | 💡 Marker nur auf **Wissen mit Zeitstempel**; veraltete Hinweise werden als „älter“ gekennzeichnet (vgl. Wandernde Spur, `23#1074A` Frage 65) |
| Falsche Lore und Hilfe | Hilfe stützt sich auf Archivwissen, das falsch ist | 💡 Hilfe kennzeichnet „vermutet“ oder „umstritten“ (Wissensstatus, [Modul 04](04-Archiv-Journal-und-Hunter-Record.md#6-wissensstatus-und-falsche-lore)) |
| Barrierefreiheit | Hilfen, die mit Sehen, Hören oder Motorik zusammenhängen | ❓: `M-OD` führt „Accessibility“ nicht für die Ermittlung; die Quellen nennen nur Barrierefreiheit als künftige Kategorie (`48#64A`) |
| Mods | Mod-Fälle müssen alle Hilfen unterstützen | 💡: Fälle liefern Daten, Hilfen sind Darstellung (R4); Modder müssen nichts doppelt bauen |
| Lokalisierung | Hinweise und Notizen in vielen Sprachen | ❓ |

---

## 10. Auswirkungen auf andere Systeme

| System | Auswirkung |
|---|---|
| Fallmodell ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) | Jeder Fall muss ohne Hilfe lösbar sein; Hilfen müssen aus Falldaten **und dem Wissensstand** berechenbar sein. |
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Wissensstatus und Provenienz müssen so gespeichert sein, dass Hilfen sie lesen können, ohne Verborgenes zu sehen. |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Einstellungen sind **Profilsache**, nicht Weltzustand. Das Wissen des Spielers ist Spielstand. |
| Mod-API | Hilfen sind Darstellung; ein Mod braucht keine eigene Hilfelogik. |
| Companion-App | Teilt Wissensstand, nicht Fallwahrheit. |
| Medaillen und Erfolge | Abhängigkeit von Hilfestufen offen. |
| Tests ([Modul 22](22-Abdeckungsprotokoll.md), [Modul 23](23-Testkatalog-und-Abnahmekriterien.md)) | Jede Achse braucht Regressionstests; ‚Detektivmodus gibt keine Hilfe‘ ist ein eigener Test. |

---

## 11. Ergänzungen aus der Quellenauswertung

### 11.1 Missionsbewertung (Medaillen): was beschlossen ist

Die Missionsbewertung ist ein eigenes System, berührt aber die Ermittlung, weil Hinweise, Schlüsse und Beweise zu den Bewertungszielen gehören können. Entscheidungen des Entwicklers im Chat 23 (29.08.2026):

| Aussage | Status | Quelle |
|---|:---:|---|
| Nach dem Vorbild von RDR2, ‚nur würde ich es noch besser machen‘: jede Mission kann **beim ersten Durchgang** schon eine von **5 Stufen** erhalten; die **Ziele sieht man erst nachher**, nicht davor; für die beste Stufe kann man die Mission erneut spielen | 📋 | `23#582E`, `23#584E` |
| Wo die Ziele nachlesbar sind (Journal oder anderswo), ist nicht festgelegt | ❓ | `23#584E`: „im journal nachlesen oder woanders ist noch nicht aufgestellt“ |
| Bei kleineren Missionen wenige Ziele, bei größeren **10–20** | 📋 | `23#586E` |
| Die Stufen heißen nach Entscheidung des Entwicklers **Bronze, Silber, Gold, Platin, Diamant** („das hört sich besser an“); der Assistent hatte eigene Namen (Completed, Proven, Expert, Master, Legendary) vorgeschlagen | 📋 | `23#588E` zu `23#587A` |
| Ziele sind fair und natürlich (gründlich ermitteln, Zivilisten schützen, Informationen sichern, Begleiter behandeln, Kollateralschäden vermeiden, optionale Lösungswege entdecken); bei mehreren legitimen Lösungswegen können Kriterien alternativ sein; Primär-, Sekundär- und Meisterziele mit unterschiedlicher Gewichtung; kritische Bedingungen als Obergrenze; ein sehr guter Spieler kann schon im ersten Durchgang die höchste Stufe erreichen | 💡 des Assistenten | `23#585A`, `23#587A` |
| Beispielziele einer Investigation: alle wichtigen Hinweise gefunden, optionale Spur entdeckt, richtige Schlussfolgerung gezogen, entscheidendes Beweisstück gesichert, keine falsche Monsteridentifikation | 💡 Beispiele | `23#585A`, `23#587A` |
| Das Replay ist vom kanonischen Story-Zustand getrennt | 💡 | `23#585A` |
| Es soll eine Plattform wie ein Social Club geben, „aber noch besser“, mit Missionsstand und Fortschritt | 📋 Wunsch | `23#582E` |
| Challenges und Achievements werden getrennt geführt | 💡 | `23#603A` Punkt 23 |

### 11.2 Was das für die Hilfen heißt (Q-E33)

Frage Q-E33 lautet, ob Hilfestufen oder die Bot-Nutzung Medaillen beeinflussen. **Quellenlage:** Die Ziele sind **missionsabhängig** und können Ermittlungsleistungen enthalten (Hinweise, Schlussfolgerung, Beweise). Keine Quelle sagt, ob die gewählte Hilfestufe die Bewertung verändert. **Folgerung ohne Entscheidung:** Wenn ein Ziel „richtige Schlussfolgerung gezogen“ lautet, wäre es bei einer Hilfe, die Schlüsse vorschlägt (A6 in Abschnitt 4), nicht vergleichbar. Mögliche Lösungen (💡): (a) Medaillen gelten unabhängig von der Hilfe; (b) Ziele der Ermittlung werden nur ohne bestimmte Hilfen gewertet; (c) die Hilfestufe wird als Hinweis in der Bewertung geführt. **Die Frage bleibt offen** (siehe [Modul 21](21-Konflikte-und-offene-Fragen.md)).

### 11.3 Die interaktive Karte der Begleit-App

Der Entwickler (`23#691E`): Die interaktive Karte soll nur zeigen, was der Spieler in seinem Spielstand hat; für die komplette Karte gibt es Lösungsbuch und Webseite; die Webseite soll für Handys und Tablets optimiert sein; mit der Handy-App ist das ganze Spiel synchronisiert. Das passt zu `48#71E` und `48#73E` (Abschnitt 8) und beantwortet Q-E08 **teilweise** für die Begleit-App (nicht für die Karte im Spiel).

---

## 12. Was fehlt
- Eine **beschlossene** Achsenliste und Voreinstellungen.
- Regeln für den **Wechsel mitten im Fall** und für **Medaillen**.
- Die Entscheidung, ob die **Ermittlungswand** gebaut wird.
- Die Rolle des Bots für die **Hypothesenprüfung**.
- Die Entscheidung, ob einzelne **Fälle** eigene Freiheitsgrade haben dürfen (Q-E32).
- Der **Zugriffsweg** (Schlüssel, Preis, Online-Prüfung): ausdrücklich offen (`48#65E`).
