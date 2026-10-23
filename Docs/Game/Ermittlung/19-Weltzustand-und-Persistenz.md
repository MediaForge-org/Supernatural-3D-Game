# Ermittlung 19: Weltzustand und Persistenz

> [!IMPORTANT]
> **Dieses Modul verbindet die Planung mit dem einzigen gebauten Fundament: dem persistenten Weltzustand (`WorldFacts`).** Es ist ein **Engineering-Entwurf** (💡), keine Entscheidung und kein Format. Er zeigt, welche Teile der Ermittlung in den Weltzustand gehören, welche nicht, und welche Größenfragen sich aus den bestehenden Grenzen ergeben. Die Grenzen des Weltzustands stammen aus dem Code und aus [Decision 0002](../../Decisions/0002-world-facts.md) und [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) und sind dort als **Vorschläge zur Prüfung** gekennzeichnet. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was feststeht](#1-was-feststeht)
2. [Was der Weltzustand heute kann](#2-was-der-weltzustand-heute-kann)
3. [Entwurf: Was in den Weltzustand gehört](#3-entwurf-was-in-den-weltzustand-gehört)
4. [Entwurf: Was nicht hineingehört](#4-entwurf-was-nicht-hineingehört)
5. [Entwurf: Schlüsselschema](#5-entwurf-schlüsselschema)
6. [Entwurf: Größenrechnung](#6-entwurf-größenrechnung)
7. [Entwurf: Atomare Übergänge](#7-entwurf-atomare-übergänge)
8. [Entwurf: Zeitsprünge und Fortschreibung](#8-entwurf-zeitsprünge-und-fortschreibung)
9. [Entwurf: Historie und Fallbericht](#9-entwurf-historie-und-fallbericht)
10. [Entwurf: Koop und Besitzer](#10-entwurf-koop-und-besitzer)
11. [Entwurf: Mods und unbekannte Daten](#11-entwurf-mods-und-unbekannte-daten)
12. [Entwurf: Prüfregeln für Speichern und Laden](#12-entwurf-prüfregeln-für-speichern-und-laden)
13. [Sonderfälle](#13-sonderfälle)
14. [Ergänzungen aus der Quellenauswertung](#14-ergänzungen-aus-der-quellenauswertung)
15. [Speicherregeln und Medienquellen (August 2026)](#15-speicherregeln-und-medienquellen-august-2026)
16. [Was fehlt](#16-was-fehlt)

---

## 1. Was feststeht

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| W1 | Ein zentraler persistenter Weltzustand ist das ‚Gedächtnis der Welt‘; dauerhafte Änderungen (veränderte oder zerstörte Orte, entdecktes Wissen) werden verfolgt | 📋 | [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) M1 (Tier 3) |
| W2 | Fall- und Beweisfortschritt gehören zu den Dingen, die ‚in den Weltzustand gehören‘ können | 💡 (Engineering-Richtlinie) | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md), Abschnitt ‚Was gehört in WorldFacts‘ |
| W3 | Hochfrequente Simulation, Positionen, Fahrzeug- und Animationszustand gehören **nicht** hinein | 💡 | Decision 0004 |
| W4 | Große Datenblöcke und Sammlungen gehören nicht hinein (Zeichenfolgen bis 4096 Bytes) | 💡 | Decision 0004 |
| W5 | Der Journal-Speicher wächst unbegrenzt; Verdichtung (Snapshot plus Journal-Ende) ist eine **offene Architekturentscheidung** und braucht ein neues Format | ❓ | Decision 0004 |
| W6 | Co-op-Autorität und Compare-and-Set sind **offen**; der Host-Spielstand ist maßgeblich | ❓ / 📋 (Host) | Decision 0004; `CLAUDE.md` §6; `48#105E` |
| W7 | Speicherziel: 20–50 MB für einen langen Spielstand; Zuverlässigkeit hat Vorrang | 📋 Zielwert | `23#54E`, `23#55A` |
| W8 | Die Welt läuft während Haft und Zeitsprung weiter; lange Zeiträume werden in größeren Schritten zusammengefasst | 📋 (C) / 💡 (Rechenvorgabe) | `48#159E`, `48#160A` |
| W9 | Die Weltsimulation ist deterministisch testbar | 💡 | Decision 0002 |

---

## 2. Was der Weltzustand heute kann

Die folgenden Eigenschaften stammen aus dem gebauten Kern (`Core/include/spn/core/WorldFacts.h`) und den Decisions. Alles ist **Vorschlag zur Prüfung**, nicht Kanon.

| Eigenschaft | Wert/Regel |
|---|---|
| Werttypen | `bool`, `int64`, `string` |
| Schlüssel | 1 bis 128 Zeichen, erstes Zeichen `a`–`z`, Rest `a`–`z`, `0`–`9`, `_`, `.` |
| Typ pro Schlüssel | fest; Wechsel nur nach Entfernen |
| Zeichenfolge | bis 4096 Bytes |
| Ursache (`cause`) | bis 128 Bytes, **ohne Bedeutung** (nur Protokolltext) |
| Befehle | `SetFact` (anlegen oder ersetzen), `RemoveFact`, `AdjustInt` (nur auf vorhandenen Ganzzahlfakt) |
| Journal | jeder gültige Befehl wird protokolliert, einschließlich Setzen auf denselben Wert; Folge 1-basiert und lückenlos |
| Zustand | wird durch Wiederabspielen des Journals abgeleitet |
| Historie | `StateAt(n)` rekonstruiert den Zustand nach Eintrag n |
| Format | Textformat ‚SPNFACTS 1‘; kanonisch; ein Fehler lehnt das ganze Dokument ab |
| Ordnung | Fakten nach Schlüssel geordnet; Ergebnis deterministisch |
| Kosten | etwa 60 Bytes je protokolliertem Befehl auf der Platte; Ladezeit wächst linear mit dem Journal (Decision 0004, eine Messung auf einer Maschine) |
| Wechselwirkung | Das Interaktionssystem wendet **mehrere Befehle atomar** an (alle oder keiner); das ist die Grundlage der Testtür und der Pickups |

Das bestehende Muster für Spielobjekte: `placeholder.pickup.<id>.taken` (bool), `placeholder.inventory.<holder>.<item>.count` (int), Halterkennung bis 24 Zeichen, Gegenstandskennung bis 48 Zeichen ([Proposal 0007](../../Proposals/0007-pickup-items-and-minimal-inventory.md)). Das Muster zeigt, wie Fakten für Spielsysteme benannt werden (Namensraum, Kennung, Feld). Es ist ein **Platzhalter für Tests**, kein Namensschema des Spiels.

---

## 3. Entwurf: Was in den Weltzustand gehört

Aus W2: ‚Fall- und Beweisfortschritt‘ kann in den Weltzustand. Eine Zuordnung nach **Änderungshäufigkeit**:

| Gegenstand | Beispiel für Fakten | Typ | Änderungen je Spielstunde (Orientierung) | Im Journal? |
|---|---|---|---|:---:|
| **Fallzustand** | Zustand (Latent, Signal, Lead, Aktiv, …), Schutzstufe, Auslösezeit | int/string | wenige je Fall | ja |
| **Fallwahrheit (Kern)** | Ursachenart, Täterkennung, Auflösung erreicht | int/string/bool | sehr selten | ja |
| **Eskalationsstufe** | Stufe je Fall | int | selten | ja |
| **Beweisbesitz** | Beleg gefunden (bool), Besitzer, Ort | bool/string | je Fund | ja |
| **Beweisstatus** | Wissensstatus je Behauptung (Zahlcode) | int | je Änderung | ja |
| **Probe/Labor** | Zustand der Probe (Zahlcode), Abgabezeit, Ergebniszeit | int | je Schritt | ja |
| **Polizeiakte** | Aktenzustand, Verdächtigenkennung, Fahndungsstatus | int/string | selten | ja |
| **Verfahren** | Phase, Urteilcode | int | selten | ja |
| **Haft** | Haftzustand, Einrichtung, Beginn, Ende | int/string | selten | ja |
| **Öffentliches Wissen (Region)** | Wissensstufenzahl je Region | int | langsam | ja |
| **Zugehörigkeit/Rang** | Gruppe, Rang | int/string | selten | ja |
| **Meldungen** | ‚Spieler hat Zeitungsartikel Z gelesen‘ | bool | je Fund | ja |
| **Fremdbearbeitung** | Fall übernommen von NPC-Hunter K | string | selten | ja |

**Regel (Entwurf):** Ein Fakt gehört hinein, wenn er **diskret** ist, **selten** wechselt und **Historie** haben soll (Kriterium der Decision 0004: ‚Entscheide pro Fakt-Familie‘).

---

## 4. Entwurf: Was nicht hineingehört

Weil der Weltzustand diskret, klein und journalisiert ist (W3, W4), brauchen folgende Ermittlungsdaten eine **andere Ablage**, die diese Dokumentation **nicht** entwirft:

| Gegenstand | Warum nicht in den Weltzustand | Mögliche Ablage (offen) |
|---|---|---|
| **Notizen des Spielers** (Freitext, Sprachnotizen) | beliebig lang; Zeichenfolge höchstens 4096 Bytes; hohe Änderungsrate beim Schreiben | eigener Spielstandsbereich |
| **Beweisgraph** (Verknüpfungen, Wand-Layout, Positionen) | Sammlung großer Struktur; hochfrequent beim Anordnen | eigener Bereich (die Decision 0004 nennt „Sammlungen“) |
| **Journaltexte** | Text, Mehrsprachigkeit | Datenbestand plus Verweise (Kennungen) im Weltzustand |
| **Beobachtungseinträge pro NPC** (Wer hat was gesehen) | sehr viele; vgl. Abschnitt 6 | NPC-Gedächtnissystem |
| **Überzeugungen pro Beobachter über den Spieler** (Vertrauen, Furcht, Verdacht) | Zahl der Beobachter × Felder; Änderungsrate | NPC-Gedächtnissystem (aggregiert) |
| **Kameraaufnahmen, Medien** | große Daten | Dateispeicher, Verweis im Weltzustand |
| **Position von Spuren**, Blutverteilung, Tatortszene | räumliche Daten, Szene | Szenenzustand der Engine |
| **Zeitleiste** | Liste | eigener Bereich |
| **Labor-Warteschlangen** | Liste, Priorität | eigener Bereich oder abgeleitet aus Fakten |
| **Charakteraussehen** | [Decision 0005](../../Decisions/0005-character-appearance-foundation.md) (`AppearanceSpec`) | im Weltzustand höchstens eine Kennung |

**Folgerung:** Die Ermittlung braucht neben dem Weltzustand einen oder mehrere **weitere Persistenzbereiche** für Sammlungen und Text. Das ist eine **Architekturentscheidung**, die offen ist (Q-E67). Die Konsistenz zwischen beiden Bereichen (Verweise, gleichzeitige Speicherung, Rücksetzen) ist Teil davon.

---

## 5. Entwurf: Schlüsselschema

> [!NOTE]
> **Nur Vorschlag (💡).** Bezeichnungen sind Arbeitsnamen. Die Grammatik (a–z, 0–9, `_`, `.`, ≤128 Zeichen) ist durch den Kern vorgegeben.

### 5.1 Aufbau

`<bereich>.<objekt>.<kennung>.<feld>` – wie das bestehende Muster. Bereiche (Arbeitsnamen):

| Bereich | Objekt | Beispiel für Schlüssel (erfunden) |
|---|---|---|
| `case` | Fall | `case.main.blackwater.state` (int) |
| `case` | Fall | `case.main.blackwater.stage` (int) |
| `evid` | Beleg | `evid.blackwater.autopsy_report.found` (bool) |
| `claim` | Behauptung | `claim.blackwater.cause_is_supernatural.status` (int) |
| `lab` | Probe | `lab.sample_017.state` (int) |
| `police` | Akte | `police.file_0042.state` (int) |
| `court` | Verfahren | `court.case_0042.phase` (int) |
| `jail` | Haft | `jail.player.local.state` (int) |
| `region` | Region | `region.north_county.awareness` (int) |
| `rep` | Gruppe | `rep.hunter_net.player.local.level` (int, nur aggregiert) |

### 5.2 Längenbudget

Der Schlüssel darf höchstens 128 Zeichen haben. Folgerung aus dem Muster: Kennungen **müssen kurz sein** und in der Länge begrenzt, damit der längste Schlüssel gültig bleibt (wie bei Halter ≤ 24 und Gegenstand ≤ 48 im Pickup-Muster: längster Schlüssel 101 Zeichen). Für Fälle und Beweise müssten ähnliche Grenzen festgelegt werden, zum Beispiel Fallkennung ≤ 32, Belegkennung ≤ 48. Offen (Q-E68).

### 5.3 Typen

| Wertart | Darstellung (Vorschlag) |
|---|---|
| Zustände (Fall, Akte, Haft) | **int** mit Zahlencodes (Tabelle im Inhalt, nicht im Kern) |
| Ja/Nein (gefunden, übernommen) | **bool** |
| Kennungen (Täter, Einrichtung) | **string** (kurze Kennung) |
| Zeitpunkte | **int** (Spielzeit als ganze Zahl, Einheit zu klären) |
| Wissensstufen | **int** |

Weil der Typ je Schlüssel fest ist, müssen **Codes stabil** bleiben (neue Zustände bekommen neue Zahlen). Das betrifft Mods und Migration (Abschnitt 11).

### 5.4 Keine Bedeutung in der Ursache

Die Ursache (`cause`, ≤128 Bytes) hat **keine Semantik** (Kern: ‚free-form audit text, no semantics‘). Eine Logik darf sich nicht auf `cause` verlassen. Sie eignet sich für ein Protokoll (‚case.accept‘, ‚lab.result‘), das der Entwickler lesen kann (Abschnitt 9).

---

## 6. Entwurf: Größenrechnung

> [!NOTE]
> **Rechenbeispiel mit angenommenen Zahlen (💡).** Die Zahlen sind keine Planungswerte, nur eine Übung, die zeigt, **welche Datenarten** das Speicherziel gefährden.

### 6.1 Grundrechnung

Aus Decision 0004: etwa **60 Bytes** je protokolliertem Befehl. Aus `23#54E`/`23#55A`: Zielgröße **20–50 MB**.

| Zielgröße | Anzahl protokollierter Befehle bei 60 Bytes |
|---|---:|
| 20 MB | rund 333.000 |
| 50 MB | rund 833.000 |

(Rechnung: 20.000.000 / 60 und 50.000.000 / 60, gerundet. Das ist nur der Weltzustand; weitere Persistenzbereiche und Engine-Daten kommen hinzu.)

### 6.2 Beispielhafte Verbraucher (angenommen)

| Verbraucher | Annahme (erfunden) | Befehle |
|---|---|---:|
| 300 Fälle über 100 Stunden, 40 Änderungen je Fall | 300 × 40 | 12.000 |
| 3.000 Belege, je 3 Änderungen (gefunden, Status, Besitzer) | 3.000 × 3 | 9.000 |
| 2.000 Proben, je 6 Zustandswechsel | 2.000 × 6 | 12.000 |
| 500 Polizeiakten, je 15 Änderungen | 500 × 15 | 7.500 |
| Region-Wissen: 100 Regionen, je 200 Änderungen | 100 × 200 | 20.000 |
| **Summe Ermittlung (ohne NPC-Überzeugungen)** | | **rund 60.000** |

Das wären etwa 3,6 MB bei 60 Bytes je Befehl. Die Ermittlung im engeren Sinn passt also bequem hinein.

### 6.3 Gefährliche Verbraucher (angenommen)

| Verbraucher | Annahme (erfunden) | Befehle |
|---|---|---:|
| Überzeugungen: 3.000 NPCs (Tier A bis C) × 8 Felder × 30 Änderungen im Spiel | 3.000 × 8 × 30 | 720.000 |
| Erinnerungen: 3.000 NPCs × 40 Erinnerungen je 4 Felder | 3.000 × 40 × 4 | 480.000 |
| Zeitsprung ohne Verdichtung: 6 Monate, 3.000 NPCs, 1 Änderung je NPC und Tag | 3.000 × 180 | 540.000 je Sprung |

Jede dieser drei Zeilen allein übersteigt das Ziel von 20 MB (bei 60 Bytes: 43 MB, 29 MB, 32 MB). **Folgerungen (Entwurf):**

1. **NPC-Überzeugungen und -Erinnerungen** gehören **nicht** einzeln in den Journal-Weltzustand (Abschnitt 4), oder die Verdichtung (W5) muss entschieden sein.
2. **Zeitsprünge** dürfen nicht jede Zwischenänderung protokollieren (Abschnitt 8).
3. Das Journal darf nicht unbegrenzt wachsen; die offene Frage ‚Welche Historie bleibt?‘ (alles, die letzten N je Schlüssel, nur Meilensteine; Decision 0004) ist **für die Ermittlung zentral**, weil sie bestimmt, ob ein ‚tatsächlicher Weg‘ des Spielers rekonstruierbar bleibt (Abschnitt 9).

Die Zahlen sind **keine Prognose**, sondern zeigen die Größenordnung: Fälle und Beweise sind unkritisch; NPC-Wissen und Zeitsprungsimulation sind kritisch.

---

## 7. Entwurf: Atomare Übergänge

Das Interaktionssystem wendet **mehrere Befehle zusammen oder gar nicht** an (Testtür und Pickup: Beispiel `taken=true` und Zähler erhöhen). Dasselbe Prinzip braucht jeder Fallübergang.

| Übergang | Fakten, die gemeinsam wechseln müssen (Vorschlag) |
|---|---|
| Fall annehmen | Zustand → Aktiv; Annahmezeit setzen; Journal-Eintragskennung setzen |
| Fall lösen | Zustand → Gelöst; Lösungswegcode; Folgen (Opferzustände, Ruf, Archiv) |
| Probe abgeben | Zustand → Eingereicht; Labor; Abgabezeit; Probe aus dem Besitz entfernen |
| Festnahme | Haftzustand → Gewahrsam; Akte → Festgenommen; Besitz → beschlagnahmt |
| Urteil | Verfahren → Urteil; Haftzustand; Fahndung → aufgelöst |

Der Kern kennt heute zwei Eigenschaften: ein Befehlsbündel wird **atomar angewendet** (Interaktionssystem) und jeder Befehl wird einzeln protokolliert. Ob es eine **Transaktionsgrenze im Journal** gibt (Markierung ‚diese Befehle gehören zusammen‘), ist nicht festgelegt. Das ist für die Rekonstruktion von Ereignissen aus der Historie (Abschnitt 9) relevant (Q-E69).

---

## 8. Entwurf: Zeitsprünge und Fortschreibung

Beschlossen: Die Welt läuft weiter; Zeitsprünge werden in größeren Schritten simuliert und nur die Ergebnisse gespeichert (`48#159E`, `48#160A`, 💡 für die Rechenvorgabe). Der Weltzustand verlangt dazu:

| Anforderung | Begründung |
|---|---|
| **Fortschreibungsfunktion je Bestandteil** | Fälle, Akten, Verfahren, Wissen, NPC-Zustände schreiben sich von Zeit A zu Zeit B fort |
| **Deterministisch** | gleicher Zustand, gleiche Zielzeit, gleiches Ergebnis (W9) |
| **Wenige Befehle** | nur der **Endzustand** und wichtige Ereignisse werden protokolliert, nicht jeder Tag |
| **Ereignisliste** | wichtige Ereignisse während des Sprungs (Opfer, Entscheidungen anderer Hunter, Verfahrensausgang) werden als Ereignis erfasst, damit das Spiel dem Spieler berichten kann (Modul 16 §6) |
| **Reihenfolge** | feste Verarbeitungsreihenfolge (zum Beispiel nach Schlüsselordnung oder System), damit Ergebnisse unabhängig von Plattform und Frame sind |
| **Zufall** | Wo Zufall nötig ist, muss er aus dem Weltzustand reproduzierbar sein (Saat im Zustand); ob und wie, ist offen (Q-E70) |
| **Grenzen** | Der Sprung kann an einem Ereignis anhalten (Q-E60) |
| **Koop** | Nur mit Zustimmung (`48#163E`); alle Clients erhalten denselben Endzustand |

Diese Punkte gelten **auch** für kürzere Wartezeiten (Schlafen, Krankenhaus).

---

## 9. Entwurf: Historie und Fallbericht

### 9.1 Möglichkeit

`StateAt(n)` rekonstruiert den Zustand nach dem n-ten Eintrag. Decision 0004 nennt als Zweck ‚Debugging, historische Abfragen, Historie aus Änderungsereignissen‘. Für den **Fallbericht** (Journal-Anzeige „Dein Ermittlungsweg“, [Modul 04](04-Archiv-Journal-und-Hunter-Record.md)) liegt nahe, den tatsächlichen Weg **aus den Änderungen abzuleiten**: Welche Belege wann gefunden, welche Entscheidungen wann getroffen.

### 9.2 Entwurf: Bedingungen

| Bedingung | Erläuterung |
|---|---|
| **Ereignisse als Fakten** | Jede relevante Spielerhandlung hinterlässt einen Fakt (zum Beispiel ‚Beleg gefunden‘, ‚Zeuge befragt‘, ‚Methode gewählt‘) |
| **Ursachentext** | Die `cause`-Zeichenfolge (≤128 Bytes) trägt eine **lesbare Kennzeichnung** der Handlung, aber keine Logik |
| **Historienumfang** | Wenn das Journal verdichtet wird (W5), müssen **Meilensteine** erhalten bleiben; sonst geht der Weg verloren |
| **Eigene Ablage** | Alternativ führt ein separater Bereich den Fallbericht (Abschnitt 4: Journaltexte); der Weltzustand enthält nur Kennungen |
| **Kosten** | Je Handlung ein Befehl (60 Bytes); 300 Fälle × 30 Handlungen ≈ 9.000 Befehle: vertretbar |

Der Fallbericht ist ein **Anwendungsfall, der die Verdichtungsentscheidung beeinflusst**: Wird alles aufbewahrt, ist die Historie vollständig; werden nur Meilensteine aufbewahrt, muss der Fallbericht dazu gehören (Q-E71).

### 9.3 Nutzen für Tests

`StateAt(n)` erlaubt Tests, die einen Fall in einem Zwischenzustand prüfen und wiederherstellen. Das ist Teil der Testkataloge ([Modul 23](23-Testkatalog-und-Abnahmekriterien.md)).

---

## 10. Entwurf: Koop und Besitzer

| Frage | Vorschlag |
|---|---|
| **Wer besitzt den Zustand?** | Der Host-Spielstand (W6; `48#105E`). Gäste haben keinen eigenen Weltzustand in der Host-Welt. |
| **Fakten pro Charakter** | Mit Haltern pro Charakter (`player.local` ist im Pickup-Muster die Standardkennung für Einzelspieler; Halter bis 24 Zeichen). Für Fahndung, Haft, Vorstrafen, Ruf: Halter gleich Charakterkennung. |
| **Gemeinsame Fakten** | Fallzustand, Region, Polizeiakte (gehört zur Welt, nicht zu einem Charakter) |
| **Autorität** | Der Kern trifft keine Annahme darüber, wer Befehle ausgeben darf (Kommentar in `WorldFacts.h`). Die Entscheidung gehört zur Netzwerkarchitektur (offen). |
| **Konflikte** | Zwei Spieler lösen gleichzeitig denselben Übergang aus: Compare-and-Set oder Reihenfolge durch den Host; offen (Decision 0004 ‚compare-and-set‘) |
| **Teamabstimmung** | Das Ergebnis wird **ein** Befehlsbündel (Host) |

---

## 11. Entwurf: Mods und unbekannte Daten

Das Pickup-Muster hat gezeigt: Ein Speicherstand mit einer **nicht registrierten Art** (Mod) lädt unverändert (Tests im Kern: `Mod/unregistered kind`). Für die Ermittlung heißt das:

| Frage | Vorschlag |
|---|---|
| **Mod-Fall in einem Spielstand** | Fakten gehören dem Mod-Namensraum; fehlt der Mod, bleiben sie unverändert und werden ignoriert |
| **Namensräume** | Der Bereich wird durch einen Präfix bestimmt (Mod-Kennung); die Kennung ist Teil der Länge (≤128) |
| **Codes** | Zustandscodes des Basisspiels bleiben stabil; Mods fügen eigene Codes in eigenen Schlüsseln hinzu |
| **Basisdaten** | Mods überschreiben keine Basisdateien, sie legen Überschreibungen in einer Schicht an (`23#59A`) |
| **Migration** | Änderungen am Fallmodell brauchen Migration oder Versionierung; die Prompt-Entwürfe nennen ‚Case-Save-Migrationsfixtures‘ als Thema (`M-P03` Titel 052, ohne Inhalt) |
| **Sicherheit** | Native Mods sind „Trusted Native Mods“ (`23#59A`) |

---

## 12. Entwurf: Prüfregeln für Speichern und Laden

| Nr. | Regel (Vorschlag) | Ableitung |
|---|---|---|
| S1 | Speichern und Laden in jedem Fallzustand führt zu identischem Zustand (byte-identisches erneutes Speichern, wie beim Kern) | Kernvertrag |
| S2 | Ein beschädigter oder abgeschnittener Spielstand verändert den laufenden Zustand nicht | Kern (Test `FailedLoadKeepsState`) |
| S3 | Ein Übergang ist atomar: Es gibt keinen gespeicherten Zwischenzustand mit halbem Übergang | Abschnitt 7 |
| S4 | Wiederholtes Laden eines Spielstands ergibt dieselbe Fortschreibung bei Zeitsprüngen | W9 |
| S5 | Zeitsprung vor und nach dem Speichern liefert denselben Endzustand | W9 |
| S6 | Ein Spielstand mit unbekanntem Mod-Fall lädt, Basisfälle bleiben spielbar | Abschnitt 11 |
| S7 | Hilfen und Einstellungen verändern den gespeicherten Fallzustand nicht | Modul 08 R4 |
| S8 | Die Größe eines Spielstands bleibt unter dem Ziel bei den Beispielwerten | Abschnitt 6 |
| S9 | Die Fallwahrheit erscheint in keiner Datei, die dem Companion oder der Hilfe offensteht | Modul 08 R3 |

Eine Zuordnung zu Tests steht in [Modul 23](23-Testkatalog-und-Abnahmekriterien.md).

---

## 13. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| **Fall wird gelöscht/Mod entfernt** | Mod-Fall war aktiv | Fakten bleiben; Fall erscheint im Journal als „nicht verfügbar“ |
| **Zeichenkette zu lang** | Name, Notiz | Weltzustand nimmt nur Kennungen; Text in anderem Bereich (Abschnitt 4) |
| **Zahlenüberlauf** | Zähler | Kern lehnt ab (Überlaufregel); Zähler sind klein |
| **Schlüssel zu lang** | lange Kennungen | Längenbudget (5.2) |
| **Typwechsel** | Feld soll vom bool zum int werden | nur nach Entfernen; Migration |
| **Gleichzeitiger Fund** | zwei Spieler finden denselben Beleg | zweiter Befehl ist ein Setzen auf denselben Wert (wird protokolliert, ändert nichts) |
| **Rückwärtssprung** | Spielstand laden | gehört dem Speichersystem; Weltzeit folgt dem geladenen Zustand |
| **Plattformunterschiede** | gleiche Fortschreibung auf anderen Systemen | Determinismus (W9), Sortierung nach Schlüssel |
| **Sehr großer Spielstand** | über 50 MB | Verdichtung (W5) |

---

## 14. Ergänzungen aus der Quellenauswertung

### Entwicklungszustände der Welt

Block 4 (03.09.2026): WORLD_700 und WORLD_1500 sollen über Spieljahre bis Jahrzehnte nicht statisch bleiben (Baustellen, Neubauten, Abrisse, Schließungen, Wohnviertel- und Gewerbewachstum, Umzüge, Immobilienpreise, Gentrifizierung). „Der World State speichert den Entwicklungszustand eines Grundstücks bzw. Projekts“; es werden nicht jeder Bauarbeiter und jeder Ziegelstein simuliert (`23#1024A`, 💡 Ausformulierung; Entwickleraussage: ‚über jahre bis jahrzehnte [können] die städte wachsen‘, `23#1023E` Punkt 35). Der Assistent hatte schon im August Weltprojekte vorgeschlagen (Haus wird gebaut, Straße saniert, Geschäft eröffnet oder schließt, Unfallstelle geräumt; `23#603A` Punkt 16, 💡); eine ausdrückliche Einzelantwort darauf gibt es nicht. Für die Ermittlung heißt das: **Tatorte und Orte verändern sich**; ein Beleg an einem Ort kann durch Umbau verschwinden (vgl. Abschnitt 8 und Modul 05 §8). Die Größenrechnung in Abschnitt 6 enthält diese Entwicklungszustände nicht.

### Produktionsklassen der Wirtschaft

Der Assistent führte für große Systeme drei Klassen ein (`23#1012A`): **P1** (muss funktionieren), **P2** (vorhanden, aber selektiv simuliert; Beispiel: Zeitungsmeldung ‚Werk 3 schließt‘ ohne 1.200 simulierte Mitarbeiter), **P3** (später oder DLC). Der Entwickler bestätigte die Linie in `23#1013E` („später müssen wir noch mal genau aufstellen was in die map kommt“). Für die Persistenz gilt dieselbe Haltung wie in Abschnitt 6: Die Simulation täuscht eine große Welt an und rechnet nur, was sichtbar wird. Status: 💡 / Richtung.

### Aufräumen

Wichtige und Beweisgegenstände bleiben persistent, irrelevante alte Gegenstände werden aufgeräumt („World Cleanup“, `23#628A` Punkt 6, 💡; `23#629E`: „Die anderen punkte sind gut“). Das entlastet die Größenrechnung für abgelegte Gegenstände.

---

## 15. Speicherregeln und Medienquellen (August 2026)

### Speicherregeln aus August 2026

Chat 23 enthält Speicherentscheidungen, die für die Beweiskette gelten (Fälle können nicht durch Speichern/Laden beliebig wiederholt werden):

- Kein Speichern „mitten im kampf“, um mit viel Leben zu speichern und einen bekannten Treffer zurückzusetzen (`23#366E`, Antwort auf `23#365A`, 📋).
- Eingeschränkt wird erst, „wenn die gegner einen entdeckt haben“; Autosaves vor und nach Kämpfen sowie gelegentlich beim Fahren in der Natur (`23#368E`, 📋 Richtung; Auslegung `23#370A`: Monster, das nur misstrauisch ist, sperrt nicht, 💡).
- Bei langen Kämpfen kann ein Autosave zwischen Phasen liegen (`23#371E`; Ausformulierung `23#372A`, 💡). Derselbe Beitrag setzt die Regel, dass die Spielwelt der Serie folgt, solange nichts anderes gesagt wird (vgl. Modul 06 §13).
- Tod lädt einen Checkpoint ohne kostenlose Vollheilung (`23#365A`, 💡; vom Entwickler mit `23#366E` „der rest passt“ bestätigt).

**Folgerung (💡):** Eine Fall-Aktion, die der Spieler durch Laden rückgängig macht, bleibt rückgängig; WorldFacts-Einträge zu Beweisen gehören zum Spielstand. Ob Fahndungs- oder Fallzustand beim Tod zurückgesetzt wird, ist nicht beschlossen (❓, nicht beantwortet; verwandt mit Q-E67).

### Soziale Medien als Missionsquelle

`23#621E`: Soziale Medien sollen „endlos sein“, mehrere Missionen erzeugen, aber keine „fixe missionen“. Das stützt den dynamischen Fallgenerator (Modul 09 §14) und bedeutet für die Persistenz: Medienquellen erzeugen Hinweise, keine festen Missionseinträge (Status 📋 Richtung, Umsetzung ❓).

---

## 16. Was fehlt
- **Verdichtung** und Umfang der Historie (W5, Q-E71).
- **Persistenzbereich für Sammlungen und Text** (Q-E67).
- **Namens- und Längenschema** für Fall-, Beleg- und Aktenkennungen (Q-E68).
- **Transaktionsgrenzen** im Journal (Q-E69).
- **Zufall im Zeitsprung** (Q-E70).
- **Netzwerk, Autorität, Compare-and-Set**.
- Eine Entscheidung, **wo NPC-Überzeugungen und -Erinnerungen** leben (Abschnitt 6.3).
