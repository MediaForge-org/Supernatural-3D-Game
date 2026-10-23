# Ermittlung 03: Ermittlungsmodi und Hilfen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, **wie viel Hilfe** das Spiel bei der Ermittlung gibt und was der Detektivmodus verbietet. Bildschirmentwürfe der Quellen sind Veranschaulichung (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Überblick: [Spielvision §7.2](../Spielvision.md#72-ermittlungsmodi). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Grundsatz: Ermittlungshilfe ist eine eigene Einstellung](#1-grundsatz-ermittlungshilfe-ist-eine-eigene-einstellung)
2. [Der Vorschlag mit vier Stufen und was daraus wurde](#2-der-vorschlag-mit-vier-stufen-und-was-daraus-wurde)
3. [Der Detektivmodus](#3-der-detektivmodus)
4. [Was im Detektivmodus erlaubt bleibt](#4-was-im-detektivmodus-erlaubt-bleibt)
5. [Geführte Modi](#5-geführte-modi)
6. [Derselbe Fall für alle Modi](#6-derselbe-fall-für-alle-modi)
7. [Verhältnis zu Schwierigkeit und Realismus](#7-verhältnis-zu-schwierigkeit-und-realismus)
8. [Referenzweg nach dem Fall](#8-referenzweg-nach-dem-fall)
9. [Was fehlt](#9-was-fehlt)

---

## 1. Grundsatz: Ermittlungshilfe ist eine eigene Einstellung

| Aussage | Status | Quelle |
|---|:---:|---|
| Ermittlungshilfe wird **nicht** an den Kampf-Schwierigkeitsgrad gekoppelt, sondern unabhängig einstellbar; jemand kann harte Kämpfe, aber keine komplizierten Ermittlungen wollen, oder umgekehrt | 📋 | `M-PD` ‚Investigation modes‘ (‚Investigation guidance is independently configurable from combat difficulty and other realism systems.‘); `23#51A` (Vorschlag), `23#52E`, `23#65A` |
| Es gibt allgemein viele Schwierigkeitsgrade und eine freie Konfiguration; „es ist generell alles konfigurierbar wenn man es weniger/mehr realistisch haben will“ | 📋 | `23#64E` |
| „Das Spiel darf sehr anspruchsvoll, realistisch und komplex sein. Vereinfachungen existieren als Optionen – nicht als Grenze des eigentlichen Systems.“ | 💡 (Formulierung des Assistenten) | `23#65A` |

Der Entwickler wollte die Hilfe auch **abschaltbar**: Wer nicht nachdenken will, kann „wie ein depp der markierung folgen“ (`23#49E` Punkt 4, deutlich gesagt, für die Hilfe als Option).

---

## 2. Der Vorschlag mit vier Stufen und was daraus wurde

`23#51A` schlug vier Ermittlungsmodi vor (💡):

| Vorschlag | Verhalten (laut Assistent) |
|---|---|
| Investigativ | kaum Marker; der Spieler kombiniert Hinweise selbst |
| Unterstützt | Suchgebiete, optionale Hinweise, Journal hilft etwas |
| Geführt | genaue Wegpunkte, relevante Objekte markiert, nächste Schritte deutlich |
| Sehr geführt | Beweisketten werden weitgehend automatisch interpretiert |

Der Entwickler änderte das in `23#52E` Punkt 4: „es soll einen detektiv modus geben der keine hilfen hat, gar keine“. Damit gilt (📋):

1. Es gibt einen **Detektivmodus ohne jede Hilfe**.
2. Die **Zwischenstufen** (geführte Modi) sind offen. Die Namen, die Anzahl und der Inhalt der Stufen aus `23#51A` sind **nicht** übernommen; es steht nur fest, dass geführte Modi ‚schrittweise Hilfe‘ ergänzen können (`M-PD`).

---

## 3. Der Detektivmodus

`M-PD` ‚Investigation modes‘ (📋): ‚Detective Mode is uncompromising: tracked case name/status can be visible, but no automatic investigation guidance.‘ Der Entwickler ergänzte, dass bei aller Strenge ersichtlich sein muss, **dass** ein Fall verfolgt wird (`23#54E`: „ja beim detektiv modus muss aber trotzdem ersichtlich sein dass eine quest verfolgt wird, aber halt sonst keine infos“).

### 3.1 Verbotene Hilfen

Zusammenführung von `M-PD` und der strengen Fassung `23#55A` (📋 für die Liste in `M-PD`, 💡 für die zusätzlichen Punkte aus `23#55A`, die inhaltlich dasselbe sagen):

| Verboten im Detektivmodus | `M-PD` | `23#55A` |
|---|:---:|:---:|
| Questmarker | ja | ja (Kartenmarker) |
| Suchzonen | ja | ja |
| hervorgehobene relevante Objekte | ja | ja (keine „gelb markierten Gegenstände“) |
| automatische Schlüsse | ja | ja (keine automatisch bewertete Hypothese) |
| vorgeschlagene Hypothesen | ja | ja (keine automatisch vorgeschlagene Monsterart) |
| ‚Such hier‘-Hinweise | ja | ja (kein „Durchsuche das Polizeirevier“, kein „Sprich als Nächstes mit …“, kein „Gehe zu …“) |
| Beweiszähler | ja | ja (kein „3/4 Beweise gefunden“) |
| automatische Beweisverknüpfungen | ja | ja |
| versteckte Führung | ja | ja (kein verstecktes UI, das den richtigen Weg indirekt hervorhebt) |
| Entfernungsanzeige zum Ziel | – | ja |
| Spürsinn-Ersatz, der Beweise verrät | – | ja („kein Witcher-Sense-Ersatz“) |

### 3.2 Sichtbar bleibt der aktive Fall

Ein Eintrag des Typs ‚Aktiver Fall: <Name>‘ ist **keine** Ermittlungshilfe, sondern UI-Zustand (`23#55A`, 💡 als Formulierung; die Regel selbst 📋 nach `M-PD` und `23#54E`).

---

## 4. Was im Detektivmodus erlaubt bleibt

Der Detektivmodus ist hilfefrei, aber nicht orientierungslos. Die Quellen halten fest:

| Erlaubt | Status | Quelle |
|---|:---:|---|
| Journal und Archiv halten fest, **was der Spieler tatsächlich weiß**; sie sagen nicht, was daraus zu schließen ist | 📋 | `23#55A`; `M-PD` ‚Hunter Archive epistemology‘ |
| Der Spieler markiert relevante Hinweise, Personen und Orte **selbst**, bildet Hypothesen und setzt persönliche Marker und Ziele | 📋 | `M-PD`; `23#54E` („diese dinge muss der detektiv aber alle selber auswählen, nicht das spiel vorgeben“) |
| Ein vom Spieler gesetzter Marker darf angezeigt werden: „Der Spieler setzt den Marker, nicht das Spiel.“ (Navigationskomfort ohne Lösungshilfe) | 💡 Formulierung, 📋 Prinzip der Selbstmarkierung | `23#55A`; `M-PD` |
| Das Fortschrittsmenü (Hunter Record) bleibt vollständig verfügbar | 📋 | `23#54E`, siehe [Modul 04](04-Archiv-Journal-und-Hunter-Record.md) |
| Eigene Notizen | 📋 | `23#1074A` Frage 88 (Antwort „88 ja sicher“ `23#1075E`) |

Ob im Detektivmodus **Kartenmarker nur für Orte** erlaubt sind, die der Spieler selbst gesetzt hat, oder ob die Karte ohnehin alle bekannten Orte zeigt, bleibt offen (❓, [Modul 21](21-Konflikte-und-offene-Fragen.md)).

---

## 5. Geführte Modi

`M-PD`: ‚Guided modes may progressively add assistance.‘ (📋 Richtung.) Mögliche Hilfen laut `23#51A` (💡, nicht beschlossen): Suchgebiete, optionale Hinweise, genaue Wegpunkte, hervorgehobene relevante Objekte per „Hunter-Vision/Focus“, automatisch interpretierte Beweisketten, „NEUES ZIEL“-Einträge mit Entfernung. Die Details sind unbestimmt.

---

## 6. Derselbe Fall für alle Modi

Der Assistent legte fest (`23#51A`, 💡 als Architekturaussage): „Der gesamte Fall bleibt derselbe.“ Es werden **nicht** zwei Fassungen jeder Quest gebaut; die Modi unterscheiden sich in der Darstellung der Hilfe, nicht im Fall. Der Entwickler widersprach nicht (`23#52E`). Konsequenz für die Autorenarbeit: Jeder Fall muss ohne Hilfe lösbar sein und die Hilfen müssen aus denselben Daten ableitbar sein (🧭, Folgerung, nicht ausdrücklich bestätigt).

---

## 7. Verhältnis zu Schwierigkeit und Realismus

`23#65A` (💡) zeigt Beispielkombinationen, die der Entwickler als Philosophie bestätigte (`23#64E`: „es ist generell alles konfigurierbar“):

- Kampf „Grounded“, Verletzungssimulation realistisch, Ermittlung „Geführt“, Fahren „Assistiert“, Todeskonsequenz „Reduziert“.
- Kampf „Normal“, Ermittlung „Detektiv“, Verletzungssimulation Maximum, Fahren „Simulation“, Todeskonsequenz „Persistent“.

Die **Bezeichnungen** der Einstellungen sind Beispiele. Festgelegt ist nur: mehrere unabhängige Regler statt einem (📋 Richtung, `23#65A` Kernsatz „nicht ein Regler kontrolliert alles“).

---

## 8. Referenzweg nach dem Fall

Nach Abschluss zeigt das Journal zuerst den **tatsächlichen** Weg des Spielers (siehe [Modul 04](04-Archiv-Journal-und-Hunter-Record.md#3-das-falljournal)). Optional kann ein **vorgesehener Weg** angezeigt werden:

| Aussage | Status | Quelle |
|---|:---:|---|
| Der vorgesehene Weg ist optional | 📋 | `23#64E` Punkt 2 („wir könnten dann aber noch den eigentlich vorgesehenen weg sagen aber das wäre eher optional“); `M-PD` |
| Er darf **nie als „der richtige Weg“** erscheinen; wenn der kürzere Weg funktioniert hat, war er ebenfalls richtig | 📋 | `M-PD` (‚never framed as the only ‘correct” path“); `23#65A` |
| Mögliche Begriffe: Referenzweg, ausführlicher Ermittlungsweg, alternativer Lösungsweg, Designpfad; vielleicht mehrere bekannte Möglichkeiten | 💡 | `23#65A` |

---

## 9. Was fehlt

- Zahl, Namen und Inhalt der **geführten Stufen**.
- Ob und wie die Karte im Detektivmodus **bekannte** Orte zeigt.
- Das Verhältnis zu **Begleitern**, die Hinweise geben könnten (Companion-Wissen ist laut `M-PD` nicht allwissend).
- Die technische Umsetzung der Hilfen (Darstellungsebene gegenüber Falldaten).

Siehe [Modul 21](21-Konflikte-und-offene-Fragen.md).
