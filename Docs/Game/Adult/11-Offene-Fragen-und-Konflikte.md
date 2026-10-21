# Adult 11: Offene Fragen, Konflikte und überholte Planung

> [!IMPORTANT]
> Dieses Dokument **trifft keine Entscheidungen.** Es sammelt, was offen ist, wo sich Quellen widersprechen und was durch spätere Festlegungen ersetzt wurde. Entscheidungen trifft ausschließlich der Entwickler. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)).

**Inhalt**

1. [Konflikte, die eine Entscheidung brauchen](#1-konflikte-die-eine-entscheidung-brauchen)
2. [Offene Fragen nach Bereich](#2-offene-fragen-nach-bereich)
3. [Überholte oder aufgelöste Planung](#3-überholte-oder-aufgelöste-planung)
4. [Vorschlag zur nächsten Klärung](#4-vorschlag-zur-nächsten-klärung)

---

## 1. Konflikte, die eine Entscheidung brauchen

| Nr. | Konflikt | Quellen | Stand |
|---|---|---|---|
| **K1** | **Frühe Zustimmungsvorstellungen vs. Sicherheitskern.** Einzelne frühe Entwickleraussagen (10./11./16.09.: Nicken oder einmaliges „Ja“ bei starker Trunkenheit genüge, Schweigen oder Stöhnen heiße weitermachen, ein Nein sei oft nur Spiel) widersprechen dem Sicherheitskern. | Interview, Antworten vom 10.09., 11.09., 16.09.; Gegenpositionen im selben Gespräch | **Aufgelöst** durch Capacity Gate und Sitzungsrahmen; Entwickler bestätigte: „Ja ok dann passt das“ (13.09.), „nicht zu streng“ (16.09.). Ob die **Gewichtung der Signale** passt, ist offen. |
| **K2** | **„USER-LOCK“ vom 20.09.: Handeln gegen den Willen im internen Admin-Build, Weiterführen bei schlafenden oder nicht entscheidungsfähigen Figuren.** Der Entwickler markierte dies als von ihm festgelegt und wollte denselben Funktionsumfang wie das Normalsystem. Der Assistent lehnte die Ausarbeitung ab. | Interview 20.09., Nachrichten 633 bis 660 | **⛔ Nicht freigegeben.** Widerspricht Sicherheitskern Regeln 2 und 3. Keine Spezifikation, keine Tests. **Entscheidungsbedarf:** Ob es einen reinen **Konsequenz-Testpfad** (Zustände, Beweise, Polizei, Gericht) **ohne** sexuelle Handlungsmechanik geben soll, und ob der Entwickler den USER-LOCK zurückzieht oder ausdrücklich bekräftigt. Eine Bekräftigung bräuchte zusätzlich eine Rechtsprüfung. |
| **K3** | **Spielerbegangene Sexualstraftaten im Kriminalitätssystem.** Vom Entwickler gewünscht (15.09.), vom Assistenten als Welt- und Konsequenzthema ohne Ausführungsmechanik formuliert. | Interview 15.09. | 🧭 **Vorläufig, prüfungsbedürftig** ([Modul 09, Abschnitt 2.2](09-Dunkle-Themen-und-Nicht-Uebernommenes.md#22-vorläufig-und-prüfungsbedürftig)). |
| **K4** | **Rollenspiel mit gespielter Nichteinwilligung** (der Entwickler: wie bei Produktionsfirmen, mit Veröffentlichung auf Plattformen in der Spielwelt). Der Assistent stimmte als **vereinbartes Rollenspiel zwischen Erwachsenen** zu. | Interview 15.09., 16.09. | 🧭 Als Kontext-Klassifizierung gedacht, **inhaltlich nicht freigegeben**; Abgrenzung zur realen Nichteinwilligung (K2) muss in einer Spezifikation messbar sein. |
| **K5** | **Setting-Entscheidung zur Orientierung** (keine entsprechenden Inhalte in der Spielwelt) vs. **datengetriebenes Orientierungsmodell** (Fragen 151 bis 153). | Interview 15.09. | ❓ Entscheidung des Entwicklers ausstehend: Soll das Modell ausschließlich die vorgesehene Demografie abbilden (und Mods dürfen mehr), oder gibt es zusätzliche Vorgaben? ([Modul 05, Abschnitt 4](05-NPCs-Beziehungen-Praeferenzen.md#4-orientierung)). |
| **K6** | **Langzeit-Begleitvertrag:** Wunsch, dass die Figur praktisch nie kündigt; Gegenposition, dass sie ihr Kündigungsrecht behalten muss. | Interview 15.09. | 🧭 Kompromiss: Figur kann kündigen („sehr selten“); Details offen ([Modul 08](08-Welt-Orte-Branche-Wirtschaft.md#52-langzeit-begleitvertrag)). |
| **K7** | **KI-Nutzung „eher geheimhalten“** vs. wahrheitsgemäße Plattformangaben. | Interview 25.09. | ❓ Entscheidung vor Release. Angaben müssen korrekt sein. |
| **K8** | **Frühe Annahme ‚Frauen sind bei den richtigen immer bereit‘** (10.09.) vs. harte Grenzen jeder Figur. | Interview 10.09. | **Aufgelöst:** jede Figur hat eigene Grenzen; sehr weite Präferenzprofile sind möglich, aber partner- und situationsabhängig ([Modul 05](05-NPCs-Beziehungen-Praeferenzen.md#2-präferenzprofil)). |
| **K9** | **Einvernehmlicher Schlaf-Zustand** (Vorschlag des Assistenten 20.09.) vs. Regel ‚Schlafende nehmen nicht aktiv teil‘. | Interview 20.09. | ❓ Abgrenzung offen; ohne Entscheidung gilt die strengere Regel ([Modul 01, Abschnitt 5.1](01-Grundregeln-Alter-Zustimmung.md#51-endstand)). |

---

## 2. Offene Fragen nach Bereich

### Recht, Vertrieb, Alter

- Vertriebsweg, Plattform- und Altersfreigabe; rechtliche Prüfung des Pakets (Österreich, Deutschland, weitere Märkte); Zahlungsdienstleister ([Modul 10](10-Produktion-Recht-Vertrieb.md)).
- Altersmarkierung von Mod-Inhalten und Verifizierungsablauf in der Praxis; Behandlung von „sehr jungen Erwachsenen“ im Design (Build-Validator als Vorschlag).
- Zulässigkeit extremistischer Symbolik in Tattoos.

### Paket und Technik

- Finaler Zuschnitt der Pakete, Gesamtgröße, Preset-Bündel.
- Netzwerkdetails für Koop; Vertragsfragen zur Stimmenproduktion.
- Speicherformat des Adult-Zustands (eigene Datei, Verschlüsselung, Migration); Verhältnis zum bestehenden Weltfakten-Format.
- Einbindung in den Unreal-Modulschnitt.

### Inhalt und Taxonomie

- **Missing-Family-/Taxonomie-Audit** und **externe Gegenprüfung**; Einfrieren der Registry.
- Konkrete Inhaltskataloge, Animationsumfang, Kameraprofile, Stimmenproduktion (Besetzung, Produktionsweg).
- Mehrstufige Mehrpersonen-Konstellationen ohne gemeinsame Animation (Rückfallverhalten festgelegt, Abdeckung nicht).
- Welche extremen einvernehmlichen Inhalte tatsächlich produziert werden (Inhalts- und Rechtsentscheidung).
- Feste In-Game-Videos ohne großen Speicherbedarf.

### Steuerung und Oberfläche

- Ausgestaltung der Modi „Direkt“ und „Assistiert“, Tastenbelegung, Lernsystem für Aktionen.
- Streamer-Filter und Benennung der Nacktheitsstufen.

### Gesundheit und Körper

- Konkrete Raten, Zeiträume und Zahlenwerte der Gesundheitssimulation (Heilung, Risiko, Infektion, Fruchtbarkeit).
- Detailregeln zur Zustimmung bei Drogen und Medikamenten.
- Hörbarkeit von Herzschlag anderer.

### Welt und Wirtschaft

- Anzahl, Lage und Rechtslage der Betriebe in der fiktiven Region; Preismodell; Vertragsdetails.
- Zeitmaßstab, Kalender und Tagesabläufe (eigene Planung).
- Zeitliche Einordnung der Umsetzung im Projektverlauf (Entwickler wünscht frühe Verfügbarkeit des Pakets, aber nach den Grundlagen).

### Spielerfigur

- Welche Spielerfigur-Geschlechter und Konstellationen es gibt (hängt mit K5 zusammen).

---

## 3. Überholte oder aufgelöste Planung

| Frühere Planung | Nachfolgender Stand |
|---|---|
| Rahmenentwurf (28.08.): Erwachsenen-Darstellungsmodi (inklusive ‚voll erlaubt‘) als Teil des Spielkerns | Seit dem 31.08. und den September-Runden: Der erweiterte Teil ist ein **getrenntes optionales Paket**; das Basisspiel zeigt nicht explizit. |
| Rahmenentwurf: Anfrage-Fluss und Server-Architektur auch für großen Online-Betrieb | Der offizielle Online-Modus lädt keine Mods; das Paket ist dort nicht nutzbar. Der Netzwerkteil bleibt für Koop und als Architekturgrundsatz. |
| Frühe Zwischenstände zur Zustimmung bei starker Trunkenheit (10.09.) | *Capacity Gate* ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#5-entscheidungsfähigkeit-alkohol-und-drogen)). |
| ‚Schweigen oder Stöhnen heißt weitermachen‘ (16.09.) | Sitzungsrahmen mit Mehrsignal-Bewertung ([Modul 01, Abschnitt 4](01-Grundregeln-Alter-Zustimmung.md#4-sitzungsrahmen-statt-dauerabfrage)). |
| „Frauen tun nur so, als hätten sie kein Interesse“ (11.09.) | Ein aktuelles Nein bleibt ein Nein ([Modul 05](05-NPCs-Beziehungen-Praeferenzen.md#3-anziehung-und-beziehung)). |
| Standardwert ‚Alter bestätigt = wahr‘ in einem externen Entwurf | Ersetzt durch *fail closed* ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#2-altersgrenze-und-prüfung)). |
| Interner Entwurf mit versionsabhängigen IDs und Unreal-Typen im engine-unabhängigen Kern | Nicht übernommen: stabile, versionsfreie IDs; Kern engine-unabhängig. |
| Ein einzelner globaler Intensitätsschalter (Entwurf mit Stufen von „aus“ bis „hardcore“) | Kein globaler Schalter: Paket an/aus, Kategorien und Presets einzeln ([Modul 02](02-Architektur-Pakete-Technik.md#3-paketstruktur)). |
| „Fehlende Kategorien“ der Registry über Qwen sofort klären | Reihenfolge: eigener Audit → externer Lauf → Einfrieren ([Modul 03](03-Interaktion-Taxonomie-Ablauf.md#7-stand-der-registry-und-der-audits)). |

Frühere Formulierungen von KI-Assistenten (Entwürfe, Datenstrukturen, Listen) sind **Vorschläge**, keine Entscheidungen; hier wurden Vorschläge nur als solche (🧭) geführt oder als bestätigt, wenn der Entwickler sie ausdrücklich akzeptierte.

---

## 4. Vorschlag zur nächsten Klärung

Ein Vorschlag (🧭) zur Reihenfolge, wenn der Entwickler entscheiden will:

1. **K2** klären (Rücknahme oder Konsequenz-Testpfad ohne Handlungsmechanik).
2. **K5** und die Spielerfigur-Konstellationen klären, weil sie das Datenmodell der Beziehungen bestimmen.
3. Rechtliche und Plattformfragen mit fachkundiger Hilfe angehen, bevor Inhalte produziert werden.
4. Erst danach Taxonomie-Audit, externer Lauf und Registry-Freeze.
5. Danach eine technische Spezifikation des Regelkerns (Alter, Zustimmung, Zustände) als engine-unabhängige Bibliothek mit den Tests aus [Modul 01](01-Grundregeln-Alter-Zustimmung.md#9-ableitbare-tests), wenn das Projekt so weit ist.
