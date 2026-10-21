# Das Adult-System: Planungsstand

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Dokument ist der **zentrale Einstieg** in die Planung eines optionalen Erwachsenen-Bereichs. Im Repository existiert dafür weder Code noch Content. Es ist eine sachliche, technisch orientierte Rekonstruktion der dokumentierten Planung und keine Zusage, wann oder in welchem Umfang etwas umgesetzt wird. Der Bereich betrifft ausschließlich **erwachsene Figuren** und **einvernehmliche** Interaktionen. Alle Aussagen tragen eine Statuskennzeichnung ([Legende](#12-statuskennzeichnung)).

Die ausführliche Dokumentation liegt in [`Docs/Game/Adult/`](Adult/README.md). Dieses Dokument fasst zusammen und verweist. Es ergänzt die README ([Kapitel 21](../../README.md#21-das-adult-system-optional-und-modular)) und steht neben [Spielvision](Spielvision.md), [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md), [Gameplay-Systeme](Gameplay-Systeme.md) und [Entscheidungsstatus](Entscheidungsstatus.md).

**Inhalt**

1. [Einordnung und Quellenlage](#1-einordnung-und-quellenlage)
2. [Die wichtigsten Festlegungen im Überblick](#2-die-wichtigsten-festlegungen-im-überblick)
3. [Detaildokumente](#3-detaildokumente)
4. [Was ausdrücklich nicht geplant ist](#4-was-ausdrücklich-nicht-geplant-ist)
5. [Offen und im Konflikt](#5-offen-und-im-konflikt)
6. [Stand der Umsetzung](#6-stand-der-umsetzung)

---

## 1. Einordnung und Quellenlage

### 1.1 Bedeutung im Gesamtprojekt

Der Erwachsenen-Bereich ist **kein Randthema der Planung**. Er wurde zwischen Ende August und Ende September 2026 in einem eigenen, sehr ausführlichen Planungsprozess mit dem Entwickler bearbeitet: zuerst als Rahmenwerk (28. August), danach in **20 nummerierten Planungsrunden** (10. bis 27. September) mit mehreren hundert Einzelentscheidungen.

Zugleich ist er ein **optionales Subsystem** eines größeren Spiels. Er darf die Hauptarchitektur nicht dominieren, das Basisspiel darf nicht von ihm abhängen, und er nutzt dieselben Kernsysteme wie das restliche Spiel (Körper, Physik, Gesundheit, Gedächtnis, Beziehungen, Weltzustand). Ein paralleles Zweitsystem ist ausdrücklich nicht vorgesehen.

### 1.2 Statuskennzeichnung

| Kennzeichnung | Bedeutung |
|---|---|
| ✅ | Implementiert und getestet. Für diesen Bereich trifft das **auf nichts** zu. |
| 📋 | **Bestätigte Planung:** Der Entwickler hat die Richtung ausdrücklich bestätigt. Nicht implementiert. |
| 🧭 | **Vorläufig oder Vorschlag:** Arbeitsstand, Entwurf, Vorschlag (meist eines Assistenten); nicht verbindlich. |
| 🔭 | Langfristige Vision ohne Spezifikation. |
| ❓ | Offen: nicht entschieden. |
| 🕰️ | Überholt: frühere Planung, später ersetzt. |
| ⛔ | Nicht freigegeben oder ausgeschlossen. |

### 1.3 Wie belastbar ist die Planung?

- **Konsolidierte Entscheidungen (Stand 31. August 2026).** Sie betreffen die Rahmenbedingungen: Trennung vom Basisspiel, Modularität, Zustimmungsprinzipien, Darstellungsgrenzen des Basisspiels, Nachtleben, Beziehungs-Sandbox, Stopp-Signal.
- **Detailplanung aus dem Entwicklerinterview (10. bis 27. September 2026).** Sie ist der weitaus größere Teil. Die Antworten wurden dort von Runde zu Runde als „gelockt“ festgehalten, aber **bisher nicht in die kanonischen Statusdokumente übernommen**. Sie gelten hier als **📋 bestätigte Planung des Entwicklers, noch nicht in den Kanon konsolidiert**.
- **Ausstehend:** ein Missing-Family-/Taxonomie-Audit und ein externer Gegenprüfungslauf. Bis dahin ist die Kategorien-Registry **nicht eingefroren**.
- Der Planungsprozess hat mehrere frühe Äußerungen des Entwicklers korrigiert, wo sie mit den Grundregeln nicht vereinbar waren. Maßgeblich ist jeweils der **endgültige Stand**; Zwischenstände und nicht übernommene Wünsche stehen in [Offene Fragen und Konflikte](Adult/11-Offene-Fragen-und-Konflikte.md) und [Dunkle Themen und Nicht Übernommenes](Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md).
- Aussagen zu Recht und Plattformen stammen aus einem Gespräch mit einem Assistenten und sind **ungeprüft** ([Produktion, Recht, Vertrieb](Adult/10-Produktion-Recht-Vertrieb.md)).

---

## 2. Die wichtigsten Festlegungen im Überblick

### 2.1 Sicherheitskern (📋, nicht moddbar)

1. **Nur erwachsene Figuren.** Prüfung nach Geburtsdatum, ab 18 Jahren und 0 Tagen; im Zweifel gesperrt (*fail closed*); nie aus dem Aussehen abgeleitet.
2. **Zustimmung ist zwingend** und jederzeit widerrufbar. Sie ist kein einfacher Wahrheitswert, sondern ein Datensatz mit Geltungsbereich.
3. **Entscheidungsfähigkeit** ist Voraussetzung; bei Bewusstlosigkeit oder offensichtlicher Entscheidungsunfähigkeit endet die Interaktion.
4. **Beziehung, Story, KI-Ziele, Gruppen- oder Administratorentscheidungen ersetzen die Zustimmung nie.** Kein Story- oder Questzwang.
5. Ein vereinbartes **Stopp-Signal** beendet jede Interaktion sofort über einen sicheren Ausstieg.
6. Die Altersprüfung läuft **vor** allem anderen.

Details: [Modul 01](Adult/01-Grundregeln-Alter-Zustimmung.md).

### 2.2 Bereitstellung (📋)

- **Basisspiel:** Beziehungs- und Romantik-Sandbox, Nachtleben als Weltschicht, **nicht explizite** Darstellung von Intimität, konfigurierbare Nacktheit.
- **Optionales Paket:** erweiterte Interaktionen. Es ist **nicht** Teil des Basis-Builds oder der Konsolen-Builds, nutzt dieselben öffentlichen Schnittstellen wie jede Drittanbieter-Mod und ist im **offiziellen Online-Modus nicht nutzbar**.
- **Ziel ist eine öffentliche Veröffentlichung** des Pakets; privat-only ist der Rückfallplan. Vertriebsweg und Altersfreigabe sind ❓ offen.

Details: [Modul 02](Adult/02-Architektur-Pakete-Technik.md).

### 2.3 Systemidee (📋)

- Ein **allgemeines Interaktions-Framework** mit Teilnehmerliste, nicht Figur A und B; **keine fest verdrahteten Animationen**; eine **erweiterbare Taxonomie** mit orthogonalen Achsen; **Positionen als kontinuierlicher Raum** statt Katalog; Zustandsautomat mit sicherem Ausstieg.
- **Ein gemeinsamer Human Core** für das ganze Spiel; das Paket ergänzt nur Erweiterungen.
- **NPCs** mit verborgenen Präferenzprofilen, harten und weichen Grenzen, Gedächtnis statt Zählern, bei sehr vielen NPCs aus Archetypen und Zufallsvariation erzeugt.
- **Welt:** Privatsphäre und Zeugen werden simuliert; Reaktionen kommen aus allgemeinen Weltregeln, nicht aus Sonderregeln.
- **Charakteranpassung** (Tattoos, Piercings, Eingriffe) im selben allgemeinen Editor.
- **Technik:** eigener, getrennter Speicherbereich mit denselben IDs, Funktionsschalter, Crash-Isolation, Budgets, Datenschutz in der Telemetrie.

---

## 3. Detaildokumente

| Thema | Dokument |
|---|---|
| Sicherheitskern, Alter, Zustimmung, Stopp | [01 Grundregeln, Alter, Zustimmung](Adult/01-Grundregeln-Alter-Zustimmung.md) |
| Pakete, Builds, Speicher, Modding, Netzwerk | [02 Architektur, Pakete, Technik](Adult/02-Architektur-Pakete-Technik.md) |
| Framework, Taxonomie, Positionen, Ablauf | [03 Interaktion, Taxonomie, Ablauf](Adult/03-Interaktion-Taxonomie-Ablauf.md) |
| Einstellungen, Steuerung, VR, Kamera, Audio, Replay | [04 Steuerung, Darstellung, Kamera, Audio](Adult/04-Steuerung-Darstellung-Kamera-Audio.md) |
| Präferenzen, Anziehung, Beziehungen, Orientierung | [05 NPCs, Beziehungen, Präferenzen](Adult/05-NPCs-Beziehungen-Praeferenzen.md) |
| Körper, Physik, Gesundheit | [06 Körper, Physik, Gesundheit](Adult/06-Koerper-Physik-Gesundheit.md) |
| Tattoos, Piercings, Körpermodifikation, Eingriffe | [07 Charakteranpassung](Adult/07-Charakteranpassung-Tattoos-Koerpermodifikation.md) |
| Orte, Privatsphäre, Branche, Verträge, Internet | [08 Welt, Orte, Branche, Wirtschaft](Adult/08-Welt-Orte-Branche-Wirtschaft.md) |
| Kriminalität als Konsequenzthema, nicht freigegebene Wünsche | [09 Dunkle Themen und Nicht Übernommenes](Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md) |
| Produktion, Veröffentlichung, Recht | [10 Produktion, Recht, Vertrieb](Adult/10-Produktion-Recht-Vertrieb.md) |
| Offene Fragen, Konflikte, überholte Planung | [11 Offene Fragen und Konflikte](Adult/11-Offene-Fragen-und-Konflikte.md) |
| Chronologie, Quellen, Master-Basis | [12 Entscheidungsverlauf und Quellen](Adult/12-Entscheidungsverlauf-und-Quellen.md) |

### 3.1 Vertiefung (Etappe 2.1)

| Thema | Dokument |
|---|---|
| Quellen, Abdeckung, Lesestand | [13 Quelleninventar und Abdeckung](Adult/13-Quelleninventar-und-Abdeckung.md) |
| Alle Entscheidungen mit Status und Herkunft | [14 Entscheidungsregister](Adult/14-Entscheidungsregister.md) |
| Fragen und Antworten der Interviewrunden | [15](Adult/15-Interviewprotokoll-Runden-1-bis-10.md), [16](Adult/16-Interviewprotokoll-Runden-11-bis-16.md), [17](Adult/17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md) |
| Zeitleiste, Überschreibungen | [18 Chronologie im Detail](Adult/18-Chronologie-Detail.md) |
| Konflikte K1 bis K23 | [19 Konflikt- und Widerspruchsmatrix](Adult/19-Konflikt-und-Widerspruchsmatrix.md) |
| Spezifikationsskizzen (Vorschläge) | [20](Adult/20-Spezifikation-Regelkern-Zustimmung.md) bis [28](Adult/28-Spezifikation-Speicherung-Mods-Builds.md) |
| Tests, Abnahme, Randfälle | [29](Adult/29-QA-Testkatalog.md), [30](Adult/30-Abnahmekriterien.md), [31](Adult/31-Randfaelle-Katalog.md) |
| Abhängigkeiten, Glossar, offene Entscheidungen | [32](Adult/32-Systemabhaengigkeiten.md), [33](Adult/33-Glossar.md), [34](Adult/34-Offene-Entscheidungen-Katalog.md) |
| Rahmenentwurf 28.08. | [35](Adult/35-Auswertung-Rahmenentwurf-28-08.md) |
| Frage-für-Frage-Matrix aller 1.903 Interviewfragen | [36](Adult/36-Frage-Antwort-Matrix-Runden-1-bis-9.md), [37](Adult/37-Frage-Antwort-Matrix-Runden-10-bis-13.md), [38](Adult/38-Frage-Antwort-Matrix-Runden-14-und-15.md), [39](Adult/39-Frage-Antwort-Matrix-Runde-16.md), [40](Adult/40-Frage-Antwort-Matrix-Runden-17-bis-20.md) |
| Quellen-Audit Etappe 2.2 | [41](Adult/41-Audit-Etappe-2-2.md) |

Die Spezifikationsskizzen sind **Vorschläge der Dokumentation (🧭)**, kein Kanon, und lösen keine Entscheidung des Entwicklers aus.

Der Index mit Statuslegende und Konventionen: [Adult/README](Adult/README.md).

---

## 4. Was ausdrücklich nicht geplant ist

- Minderjährige in jeder Form im Erwachsenen-System.
- Nicht einvernehmliche sexuelle Handlungen als Spielmechanik, in welcher Variante auch immer, einschließlich eines internen Entwickler- oder Admin-Modus. Dazu existiert **keine Spezifikation**. Sexuelle Gewalt kommt höchstens als Kriminalitätsthema auf Konsequenzebene vor.
- Sexuelle Handlungen mit bewusstlosen oder schlafenden Figuren als aktiv Teilnehmende.
- Story- oder Questzwang zu intimen Handlungen.
- Zustimmung aus Beziehungswerten, KI-Zielen, Gruppen- oder Administratorentscheidungen.
- Sichtbare „Sex-Level“-Zähler oder eine Statistik, die NPC-Verhalten steuert.
- Der Erwachsenen-Bereich im offiziellen Online-Modus.
- Explizite Darstellung im Basisspiel und in Konsolen-Builds.

Die vollständige Liste, auch der in der Planungshistorie geäußerten, aber **nicht freigegebenen** Wünsche: [Modul 09](Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md).

---

## 5. Offen und im Konflikt

Entscheidungen, die der Entwickler treffen muss, bevor weitergearbeitet werden kann ([Modul 11](Adult/11-Offene-Fragen-und-Konflikte.md)):

- **K2:** der Wunsch nach einem internen Modus, der Handlungen gegen den Willen oder bei nicht entscheidungsfähigen Figuren erlaubt (⛔ nicht freigegeben, widerspricht dem Sicherheitskern).
- **K3, K4:** spielerbegangene Straftaten im Kriminalitätssystem; gespieltes Rollenspiel mit Nichteinwilligung (nur als vereinbartes Rollenspiel zwischen Erwachsenen, inhaltlich nicht freigegeben).
- **K5:** Setting-Entscheidung zur Orientierung und datengetriebenes Orientierungsmodell.
- **K6, K7, K9:** Kündigungsrecht im Langzeit-Begleitvertrag; wahrheitsgemäße KI-Angaben bei Plattformen; Abgrenzung eines einvernehmlichen Schlaf-Zustands.
- Offen außerdem: Vertriebsweg und Recht, Paketzuschnitt, Taxonomie-Audit, Inhaltskataloge, Steuermodi, Zahlenwerte der Gesundheitssimulation, zeitliche Einordnung.

---

## 6. Stand der Umsetzung

Im Repository ist **nichts** vom Erwachsenen-System umgesetzt. Implementiert sind nur die allgemeinen Grundlagen (Interaktion mit Testtür, persistenter Weltzustand, Speicher-Slots, Aussehens-Fundament), die in der [README](../../README.md) beschrieben sind. Die Erwachsenen-Planung gehört nicht zu Phase 0 oder dem Vertical Slice und bestimmt dort keine Entscheidung. Die allgemeinen Systeme, die der Bereich voraussetzt (Human Core, Körperphysik, Gesundheit, Gedächtnis, Replay, Charakter-Editor, Modding), werden für das **gesamte Spiel** geplant und nicht um das Paket herum entworfen.
