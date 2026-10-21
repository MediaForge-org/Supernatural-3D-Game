# Adult 01: Grundregeln, Alter und Zustimmung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Dokument beschreibt Planungsstand, keinen Code. Es betrifft ausschließlich **erwachsene Figuren** und **einvernehmliche** Interaktionen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Dieses Modul ist die Grundlage aller anderen: Wo ein anderes Dokument einer Regel hier widerspricht, gilt dieses.

**Inhalt**

1. [Sicherheitskern](#1-sicherheitskern)
2. [Altersgrenze und Prüfung](#2-altersgrenze-und-prüfung)
3. [Zustimmungsmodell](#3-zustimmungsmodell)
4. [Sitzungsrahmen statt Dauerabfrage](#4-sitzungsrahmen-statt-dauerabfrage)
5. [Entscheidungsfähigkeit, Alkohol und Drogen](#5-entscheidungsfähigkeit-alkohol-und-drogen)
6. [Stopp, Unbehagen und Widerruf](#6-stopp-unbehagen-und-widerruf)
7. [Grenzüberschreitung: nur als Konsequenz, nicht als Mechanik](#7-grenzüberschreitung-nur-als-konsequenz-nicht-als-mechanik)
8. [Was Mods nicht ändern dürfen](#8-was-mods-nicht-ändern-dürfen)
9. [Ableitbare Tests](#9-ableitbare-tests)

---

## 1. Sicherheitskern

Diese Regeln gelten für jede intime Interaktion und sind als **Kernregeln** geplant, die weder Mods noch Datenpakete abschalten können. 📋

| Nr. | Regel |
|---|---|
| 1 | **Nur erwachsene Figuren.** Alle Beteiligten sind eindeutig erwachsen. Minderjährige existieren in diesem System nicht. |
| 2 | **Zustimmung ist zwingend** und jederzeit widerrufbar. |
| 3 | **Entscheidungsfähigkeit.** Alle Beteiligten sind bei Bewusstsein, handlungsfähig und entscheidungsfähig. |
| 4 | **Beziehung ersetzt keine Zustimmung.** Beziehungswerte, Anziehung, Story-Ereignisse und KI-Ziele ersetzen sie nie. Story- oder Questzwang zu intimen Handlungen ist ausgeschlossen. |
| 5 | **Kein stellvertretendes Einverständnis.** Weder eine Gruppe noch ein Server-Administrator kann für andere zustimmen. |
| 6 | **Altersprüfung zuerst.** Sie läuft vor Präferenzen, Anziehung, Beziehung und allen weiteren Regeln und arbeitet *fail closed*: Zugelassen ist nur, wer eindeutig als erwachsen definiert ist. |

Die Regeln stammen aus den konsolidierten Entscheidungen vom 31. August 2026 und wurden im Interview (September) mehrfach bestätigt und präzisiert. Frühere, abweichende Zwischenstände des Entwicklers sind in [Abschnitt 5](#5-entscheidungsfähigkeit-alkohol-und-drogen) und in [Offene Fragen und Konflikte](11-Offene-Fragen-und-Konflikte.md) dokumentiert und **überholt**.

---

## 2. Altersgrenze und Prüfung

- 📋 Maßgeblich ist das **Geburtsdatum** der Figur und die Spielzeit, kein fester „Alter“-Wert. Zugelassen ist ab **exakt 18 Jahren und 0 Tagen**. Der Entwickler formulierte das am 11. September ausdrücklich so: Personen darunter sind nicht erlaubt und werden auch nicht angezeigt.
- 📋 **Unter 18** ist vollständig ausgeschlossen: kein Eintrag in Auswahlmenüs, Flirt- oder Interaktionskatalogen, kein Zugang zu irgendeinem Teil des Systems.
- 📋 Das Alter wird **nicht aus Aussehen, Körperbau oder Stimme abgeleitet**, sondern nur aus eindeutigen Figurendaten.
- 📋 **Unbekanntes Alter** bedeutet gesperrt. Es gibt eine **Altersbestätigung in der Spielwelt**: Die Figur nennt ihr Alter oder zeigt ein Dokument (zum Beispiel einen Ausweis). Gespeichert wird danach nur der Status „verifiziert“, nicht das Dokument. Ohne Bestätigung oder unter 18 bleibt alles gesperrt.
- 📋 Bei **frisch volljährigen** Figuren ist die Prüfung zwingend (Dienstleistungs- und Vertragskontexte eingeschlossen, siehe [Welt und Branche](08-Welt-Orte-Branche-Wirtschaft.md)).
- 📋 **Alter und Vorlieben sind vollständig getrennt.** Das Alter entscheidet nur, ob eine Figur überhaupt im System vorkommen darf, nicht, was sie mag. Der Entwickler hielt fest: Nur weil jemand 18 ist, heißt das nicht, dass die Person keine intensiven Inhalte mag, und umgekehrt sagt Unerfahrenheit nichts über Vorlieben.
- 📋 Die Berechtigung ist **kein frei veränderbares Tag**. Figurenkategorien in den Stammdaten dürfen Mods nicht beliebig umschreiben (siehe [Abschnitt 8](#8-was-mods-nicht-ändern-dürfen)).

Vorschläge aus der Rechtsdiskussion vom 25. September (🧭, vom Entwickler nicht ausdrücklich bestätigt, aber im Einklang mit den obigen Regeln):

- Die Altersregel als **Build-Validator**: Ein Build schlägt fehl, wenn eine Figur ohne eindeutiges Erwachsenen-Alter im Erwachsenen-Pfad erreichbar ist.
- Keine Figur, die nach Lore „volljährig“, aber erkennbar kindlich gestaltet ist. Sehr junge Erwachsene sollen auch **visuell eindeutig erwachsen** wirken.

Verworfen (🕰️): ein externer Entwurf mit dem Standardwert ‚Alter bestätigt = wahr‘. Ersetzt durch *fail closed* mit explizitem Status.

---

## 3. Zustimmungsmodell

Zustimmung ist **kein boolescher Wert**. 📋

Ein Zustimmungs-Datensatz enthält:

- die Anfrage und den Teilnehmer,
- erlaubte Inhalte, Varianten, Profile und Intensitäten,
- den Kontext (Situation, Ort, Beziehung),
- Zeitpunkt und Gültigkeit.

**Zustandsfolge:** unbekannt → möglicherweise interessiert → angeboten → wird erwogen → akzeptiert, abgelehnt, zurückgezogen, abgelaufen oder vorübergehend nicht verfügbar.

**Geltungsbereich (Scope)** in aufsteigender Feinheit: Zuneigung, Romantik, Sinnlich, Erwachsenen-Kategorie, Unterart, Profil, Variante, Intensität, Positionsprofil, Sitzung. Die Erlaubnis für eine Wurzelkategorie ist **nicht automatisch** die Erlaubnis für alle Unterarten oder Profile.

**Neu geprüft** wird nur an echten Grenzen:

- Wechsel der Wurzelkategorie, von Romantik zu Erwachsenen-Inhalt, auf eine nicht freigegebene Unterart oder Variante,
- von Standard oder Fortgeschritten zu Hochintensität,
- neue Teilnehmer,
- deutliche Intensitätsänderung oder relevante Positionsänderung,
- längere Unterbrechung oder wesentliche Zustandsänderung.

**Nicht** neu geprüft wird bei kleinen Variationen. Ein Dialogfenster alle paar Sekunden ist ausdrücklich unerwünscht. Eine einmal gegebene Zustimmung bleibt in der laufenden Situation bestehen, bis sie widerrufen wird oder sich die Situation wesentlich ändert. Am nächsten Tag oder in einer neuen Begegnung zählt sie **nicht** automatisch.

**Zustimmungspfad:** Entscheidungsfähigkeit → Zustimmung → Eskalation. Aus fehlender Gegenwehr wird nie Zustimmung abgeleitet. Deutlich positive Signale: Mitmachen, Initiieren, ein klares Ja, ein eindeutiges Nicken. Rückzug, Abwehr, Weggehen und Ablehnung stoppen.

**Voraussetzungen einer Sitzung:** Alle Beteiligten sind als Erwachsene verifiziert, bei Bewusstsein, entscheidungsfähig, zustimmend, verfügbar, nicht im Kampf, nicht gefesselt, nicht kritisch verletzt, nicht story-gesperrt und mit Umgebung und Interaktion kompatibel.

---

## 4. Sitzungsrahmen statt Dauerabfrage

Der Entwickler wollte spontane Wechsel ohne ständige Bestätigungsfenster, aber „nicht zu streng“ geregelt. Aus dem Gespräch vom 16. September ergab sich ein Konzept, das der Entwickler mit „Ja ok, aber nicht zu streng“ bestätigte (📋, die Idee stammt vom Assistenten, die Zustimmung vom Entwickler):

```text
Sitzungsrahmen (Session Intent) → Live-Signale → dynamische Eskalation
                                → Präferenz-Lernen → Gedächtnis
```

- Ist zwischen den Beteiligten vorher klar, dass die Sitzung frei oder experimentell laufen darf (zum Beispiel „du kannst führen“), dürfen Position, Tempo, Intensität und viele Handlungen spontan wechseln.
- Das System wertet mehrere Signale zusammen aus: etablierter Rahmen der Sitzung, bekannte harte Grenzen, partnerspezifische Präferenzen, aktive Beteiligung oder Gegenbewegung, Körperhaltung (Näherziehen oder Wegdrücken), Stimme und Reaktion, Schmerz-/Stressreaktion gegenüber positiver Reaktion.
- **Schweigen allein ist keine Zustimmung.** Der Entwickler hatte vorgeschlagen, dass fehlende Ablehnung oder Stöhnen genüge; das wurde als Systemregel **nicht** übernommen. Stöhnen kann auch Schmerz oder Überforderung sein. Ruhig, aber normal beteiligt bleiben, führt nicht zum Abbruch.
- Eine **deutlich neue Handlung außerhalb des etablierten Rahmens** bekommt nicht durch Schweigen Zustimmung. Klare Ablehnung oder aktives Wegdrücken beendet oder verändert die Handlung sofort.
- Eine spontane Handlung, auf die die Person positiv reagiert, kann danach eine **neue Erfahrung oder Präferenz** erzeugen. Das macht einen vorherigen Grenzübertritt aber nicht rückwirkend in Ordnung.

Offen (❓): die konkrete Gewichtung der Signale, die Schwellen für „aktiv beteiligt“ und die Darstellung in der Oberfläche. Siehe [Offene Fragen](11-Offene-Fragen-und-Konflikte.md).

---

## 5. Entscheidungsfähigkeit, Alkohol und Drogen

Dies war einer der am längsten diskutierten Punkte. Es gibt einen **Endstand** und frühere Zwischenstände.

### 5.1 Endstand

Status: 📋

- Leichte bis mittlere Beeinträchtigung (nüchtern, leicht angetrunken, betrunken, aber klar ansprechbar und beteiligt): Die Person kann entscheiden, initiieren und zustimmen. Alkohol kann Hemmungen, Tempo, Dialog und Initiative verändern.
- Alkohol und Stimmung können **Vorlieben beeinflussen**, aber die **Zustimmung nicht ersetzen**. Es gilt ein *Capacity Gate*: Wer nicht mehr sinnvoll entscheiden kann, für den ist die Interaktion gesperrt. Ein bloßes Nicken genügt dann nicht.
- Ist eine Person **bewusstlos, nicht mehr ansprechbar oder offensichtlich nicht mehr entscheidungsfähig**, wird die Interaktion beendet beziehungsweise jede weitere sexuelle Handlung blockiert. Es folgt normales Gameplay (ansprechen, Zustand prüfen, hinlegen, Hilfe holen).
- Die Erkennung darf **etwas Zeit** brauchen (der Entwickler: bei leisen Personen und fehlendem Blick von vorn kann es einige Minuten unbemerkt bleiben), aus Eigenbewegung, Muskelspannung, Atemmuster und ausbleibenden Antworten. Sobald der Zustand erkannt ist, wird **sofort** abgebrochen. Kein bewusstes Weitersimulieren.
- Der Abbruch soll **diegetisch** wirken (fehlende Reaktion, Körperspannung, Atmung) statt als großes Stopp-Fenster.
- Dasselbe `Reaktionsfähigkeit`-System dient bei Verletzung, Ohnmacht und Schlaf.
- Bei **starker Atem- oder Halskontrolle**, die ein Erwachsener einvernehmlich wünscht, bleibt das Gesundheitssystem voll aktiv (Bewusstseinsverlust, Verletzung bis zur Lebensgefahr). Kommt es zur Bewusstlosigkeit, greift die obige Regel: die Interaktion endet. Das Spiel liefert dafür **keine Anleitung und keine „optimale“ Mechanik**, nur Zustand und Folgen.
- **Einvernehmlicher Schlaf-Zustand** (zuvor ausdrücklich vereinbart): ist als Zustand denkbar (Schlaf-State mit natürlicher Atmung, möglichem Aufwachen, reduzierter aktiver Beteiligung). 🧭 Vorschlag des Assistenten vom 20. September; **nicht vom Entwickler als Funktion bestätigt** und im Widerspruch zur Regel ‚Bewusstlose oder Schlafende nehmen nicht aktiv teil‘, solange die Abgrenzung nicht entschieden ist. Siehe [Offene Fragen](11-Offene-Fragen-und-Konflikte.md).

### 5.2 Überholte Zwischenstände

Status: 🕰️

Am 10. und 11. September äußerte der Entwickler abweichende Vorstellungen, die im selben Gespräch vom Assistenten als nicht tragfähig eingeordnet und später durch den Endstand ersetzt wurden:

| Frühere Äußerung (10./11.09.) | Ersetzt durch |
|---|---|
| Stark Betrunkene müssten nur einmal „ja“ sagen, auch ohne zu wissen, was gemeint ist; ein Nicken genüge. | *Capacity Gate*: Wer nicht mehr entscheiden kann, kann nicht zustimmen. |
| Das System solle bei zu starker Trunkenheit nicht blockieren. | Blockiert wird bei Bewusstlosigkeit und offensichtlicher Entscheidungsunfähigkeit; leichte bis mittlere Trunkenheit bleibt erlaubt (Entwickler, 13.09.: gemeint sei Betrunkenheit, bei der man „noch denken kann“). |
| Fehlende Ablehnung oder Stöhnen bedeute, dass man weitermachen kann (16.09.). | Sitzungsrahmen mit mehreren Signalen ([Abschnitt 4](#4-sitzungsrahmen-statt-dauerabfrage)). |
| „Frauen tun oft nur so, als hätten sie kein Interesse“ als Beziehungsregel (11.09.). | Ein aktuelles Nein bleibt ein Nein; Interesse kann sich über Zeit und gemeinsame Erlebnisse entwickeln. |

Der Endstand zur Bewusstlosigkeit wurde vom Entwickler bestätigt („Ja ok dann passt das“, 13.09.).

### 5.3 Weitere Beeinträchtigungen

Detailregeln für Drogen und Medikamente: ❓ offen (siehe [Offene Fragen](11-Offene-Fragen-und-Konflikte.md)). Grundsatz: Sie beeinflussen Verhalten, aber ersetzen nie die Zustimmung, und dieselbe Entscheidungsfähigkeits-Schranke gilt.

---

## 6. Stopp, Unbehagen und Widerruf

- 📋 **Hartes Stopp-Signal:** Wird das vereinbarte Signal gegeben, endet die Interaktion **sofort** über einen sicheren Ausstieg. Es gibt keine Neuinterpretation. Ein Stopp überschreibt alles, auch Sitzungsrahmen und Rollenspiel.
- 📋 **Weiche Signale** (Unbehagen, Nachfragen, Position ändern, Intensität anpassen) führen zu Anpassungen, nicht zwangsläufig zum Abbruch. Manche Erwachsene bevorzugen einvernehmlich höhere Intensität bis hin zu Schmerz; das ist keine Zustimmungsrücknahme.
- 📋 **Widerruf** jederzeit: kritische Unterbrechung → sicherer Übergang → Ausstieg → Aufräumen.
- 📋 Ein früherer Widerruf wirkt in der **Erinnerung** der Figur nach (siehe [NPCs und Beziehungen](05-NPCs-Beziehungen-Praeferenzen.md)).
- 📋 Ein sicherer Ausstieg ist **immer** erreichbar; fehlende Animationen oder Übergänge lösen eine Rückfallleiter aus, keinen Softlock (siehe [Interaktion und Ablauf](03-Interaktion-Taxonomie-Ablauf.md)).

---

## 7. Grenzüberschreitung: nur als Konsequenz, nicht als Mechanik

Am 20. September wollte der Entwickler („markiere das als von mir festgelegt“, 35#633), dass der Spieler technisch eine **neue oder weitergehende Handlung beginnen können** soll, auch wenn der NPC sie nicht bestätigt hat, und in einem internen Admin-Build auch gegen den erkennbaren Willen oder bei nicht entscheidungsfähigen Personen fortfahren kann. Der Assistent lehnte die Ausarbeitung dieser Handlungsmechanik ab und formulierte stattdessen einen Rahmen, den der Entwickler **nicht ausdrücklich bestätigte** (zuletzt: „Ja ok dann plane ich das dann halt mit Qwen“, 35#657). Der Stand ist daher:

- ⛔ **Keine freigegebene Spielmechanik.** Es gibt keine Spezifikation, keine Tests und keine Abnahmekriterien für das Fortsetzen von Handlungen gegen erkennbaren Willen oder bei nicht entscheidungsfähigen Personen, weder im Spiel noch in einem Admin-Build. Das widerspricht den Regeln 2 und 3 des Sicherheitskerns.
- 🧭 Was als **Konsequenzebene** vorgeschlagen wurde (Assistent, 35#634/636): „technisch möglich“ ist nicht „einverstanden“. Wo ein Spieler eine Grenze überschreitet, entstehen Folgen ausschließlich über bestehende Systeme: Gedächtnis und Beziehung der Figur, Verletzung, Beweise und bei Schwere das **Kriminalitäts- und Justizsystem** ([Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md)). Ob und wie das ohne Handlungsmechanik darstellbar ist, ist offen ([K2, K3](19-Konflikt-und-Widerspruchsmatrix.md#3-sicherheitsrelevante-konflikte)).
- 📋 Kommt ein klares Stopp-Signal oder eine deutliche Abwehr, endet die Interaktion nach [Abschnitt 6](#6-stopp-unbehagen-und-widerruf).
- ⛔ Der Begriff „USER-LOCK“ aus den Quellen ist **keine** Entscheidung dieser Dokumentation.

Einordnung und Entscheidungsbedarf: [Dunkle Themen](09-Dunkle-Themen-und-Nicht-Uebernommenes.md#3-historische-entwicklerwünsche-nicht-freigegeben), [Konfliktmatrix](19-Konflikt-und-Widerspruchsmatrix.md).

---

## 8. Was Mods nicht ändern dürfen

📋 Sicherheitskern, nicht normal moddbar: Erwachsenen-Berechtigung, Zustimmungspflicht, Spieler-Opt-out, Serverautorität, Blockier-Regeln, Anfrage-Berechtigungen, Sitzungssicherheit. Moddbar sind dagegen Inhalte, Kategorien, Animationen und Profile (siehe [Architektur](02-Architektur-Pakete-Technik.md#7-modding)).

Inhaltsbewertungen und Altersmarkierungen von Mod-Inhalten gehören ins Manifest; der praktische Prüfablauf ist ❓ offen.

---

## 9. Ableitbare Tests

Falls dieses Modul einmal implementiert wird, folgen aus den **bestätigten** Regeln deterministische, engine-unabhängige Tests (portable C++20-Logik, kein Unreal nötig). Dies ist ein Vorschlag (🧭), keine Zusage:

- Alter exakt 18 Jahre 0 Tage: zugelassen; einen Tag davor: gesperrt; unbekanntes Alter: gesperrt (*fail closed*).
- Alter wird nur aus Figurendaten gelesen, nie aus Erscheinungsparametern.
- Beziehungswert, Questflag, KI-Ziel, Gruppen- oder Admin-Entscheidung erzeugen keine Zustimmung.
- Zustimmung für Wurzelkategorie gilt nicht für nicht freigegebene Unterart oder Profil.
- Bewusstlos oder nicht entscheidungsfähig: Sitzung wird beendet; kein Übergang zurück in eine aktive Sitzung ohne neue Zustimmung.
- Hartes Stopp-Signal beendet in jedem Sitzungszustand über den sicheren Ausstieg.
- Zustimmung am Vortag gilt nicht in einer neuen Begegnung.
- Mod-Manifest kann Sicherheitskern-Felder nicht überschreiben.
