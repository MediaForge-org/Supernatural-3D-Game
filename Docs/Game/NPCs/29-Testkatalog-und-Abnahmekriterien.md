# NPCs 29: Testkatalog und Abnahmekriterien

> [!IMPORTANT]
> **Es gibt keine Implementierung, also keine laufenden Tests.** Dieser Katalog leitet aus den **Prüfregeln der Module 03 bis 24**, den **Querschnittsszenarien** (Modul 25) und den beschlossenen Anforderungen Tests ab, die gelten, sobald eine Funktion gebaut wird. Er ist kein Testergebnis. Die Ebene gibt an, wo ein Test laufen könnte: **C** = portabler C++20-Kern ohne Unreal (Fedora-Laptop, `CLAUDE.md` §5), **U** = Unreal-Laufzeit oder Automatisierung (Windows-Desktop), **I** = Inhalts- oder Dokumentprüfung (Review, Validierung, Skript). Welche Tests tatsächlich liefen, steht erst im jeweiligen Arbeitsbericht.

Teil der NPC-Dokumentation ([Index](README.md)). Gleiche Methode wie [Kreaturen 23](../Kreaturen/23-Testkatalog-und-Abnahmekriterien.md) und [Ermittlung 23](../Ermittlung/23-Testkatalog-und-Abnahmekriterien.md).

**Inhalt**

1. [Methode und Ebenen](#1-methode-und-ebenen)
2. [Regeltests aus den Modulen](#2-regeltests-aus-den-modulen)
3. [Eigenschaftstests (Property Tests)](#3-eigenschaftstests-property-tests)
4. [Szenariotests](#4-szenariotests)
5. [Dokument- und Inhaltsprüfungen](#5-dokument--und-inhaltsprüfungen)
6. [Messungen statt Sollwerte](#6-messungen-statt-sollwerte)
7. [Abnahmekriterien nach Ausbaustufe](#7-abnahmekriterien-nach-ausbaustufe)
8. [Abdeckung der beschlossenen Anforderungen](#8-abdeckung-der-beschlossenen-anforderungen)

---

## 1. Methode und Ebenen

1. **Jeder Test hat eine Herkunft:** eine Prüfregel (`N-EN-…`) mit ihrem Bezug auf beschlossene (📋) oder Entwurfsaussagen (💡). Ein Test, der nur aus einer Entwurfsaussage stammt, **ist selbst Entwurf** und kann sich mit der Entscheidung des Entwicklers ändern.
2. **Keine Zahlen.** Wo die Quellen keine Zahlen nennen, nennt auch der Test keine; er prüft Eigenschaften und Beziehungen (Gleichheit, Ordnung, Monotonie, Verfügbarkeit), nicht Sollwerte. Sollwerte kommen aus den Messungen (Abschnitt 6) und werden erst nach Entscheidung des Entwicklers verbindlich.
3. **Determinismus ist Testbasis.** Alle Simulationstests laufen mit festem Weltseed und festen Eingaben; ein Test, der von Systemzeit oder Threadplanung abhängt, ist fehlerhaft (Modul 22 §7).
4. **Portable Tests zuerst.** Tests der Ebene C laufen ohne Unreal und sind die Grundlage der Ausbaustufe L1 (Proposal 0010 §10).
5. **Keine erfundenen Ergebnisse.** Der Katalog meldet nichts als bestanden.

| Ebene | Bedeutung | Läuft auf |
|---|---|---|
| **C** | portabler Kern (Datensätze, Ereignisse, Gedächtnis, Beziehung, Zeitplan, Fortschreiben, Speicher) | Standalone-Testprogramm (CMake), Fedora oder Windows |
| **U** | Unreal-Anbindung (Materialisierung, Wahrnehmung, Dialog-Laufzeit, Streaming, Oberfläche) | Unreal-Automatisierung, Windows-Desktop |
| **I** | Inhalt und Dokumentation (Validierung von Daten, Review, Skript-Prüfung) | Skripte, Review |

---

## 2. Regeltests aus den Modulen

Die folgende Tabelle wird aus den Prüfregeln der Module erzeugt (`Tools/world-audit/render_npc_tests.py`) und nicht von Hand gepflegt. Jede Zeile ist ein Test; die Nummer `T-<Modul>-<laufende Nummer>` bleibt stabil, solange die Prüfregeln ihre Reihenfolge behalten. Die Ebene folgt dem Modul und dem Regelwortlaut (Heuristik; maßgeblich ist die Prüfregel).

<!-- BEGIN GENERATED RULE TESTS -->

### T-03: Identität, Persistenzstufen, Datensatztiefe

Gruppe zu [Modul 03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-03-01 | Eine NPC-ID wird nie wiederverwendet und ändert sich nie | C | N-EN-001; N-ID-008 |
| T-03-02 | Gleiche ID und gleicher Weltseed liefern denselben abgeleiteten Zustand | C | N-EN-002; N-ID-009, N-ID-020 |
| T-03-03 | Ein eingefrorener NPC wird durch ein Generator-Update nicht verändert | C | N-EN-003; N-ID-013 |
| T-03-04 | Ein Aufstieg der Datensatztiefe erfolgt nur durch ein Ereignis, nie durch Zeitablauf | C | N-EN-004; Abschnitt 5 |
| T-03-05 | Ein Abstieg verdichtet Detailzustand, aber verliert weder Identität noch Beziehungsgeschichte noch wichtige Einzelereignisse | C | N-EN-005; N-ID-015 |
| T-03-06 | Ein toter NPC nimmt keine Handlung vor und erscheint nicht als lebende Figur | C | N-EN-006; N-LT-017 |
| T-03-07 | Ein D-Mensch ohne Gespräch, Ereignis oder Beobachtung hat keine ID im Weltzustand | C | N-EN-007; Abschnitt 10 |
| T-03-08 | Eine Materialisierung am selben Ort zur selben Zeit mit demselben Weltzustand liefert dieselbe Person | C, U | N-EN-008; Abschnitt 10 |
| T-03-09 | Ein `sim_safe`-NPC stirbt nie durch reine Simulationsartefakte | C | N-EN-009; N-LT-033 |
| T-03-10 | Ein Wechsel der Persistenzstufe löscht nie die Beziehungen zum Spieler | C | N-EN-010; N-ID-015 |
| T-03-11 | Ein Umzug ändert Wohnort, Arbeit und Routine, nicht ID, Seed oder Historie | C | N-EN-011; N-ID-019 |
| T-03-12 | Der Datensatz enthält keine Zahlen für den Spieler; Sichtbarkeit nur über Verhalten und Archiv | C, U | N-EN-012; N-LT-012 |

### T-04: Persönlichkeit, Zustände, Zeitskalen

Gruppe zu [Modul 04](04-Persoenlichkeit-Zustaende-und-Zeitskalen.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-04-01 | Jede Änderung des Persönlichkeitskerns hat ein Ereignis als Ursache im Journal | C | N-EN-021; N-PS-006, N-PS-016 |
| T-04-02 | Schnelle Zustände kehren ohne neue Ereignisse zur Grundlinie des Kerns zurück | C | N-EN-022; Abschnitt 2 |
| T-04-03 | Dasselbe Ereignis kann für zwei NPCs gegensätzliche Wirkung haben, aber jedes Ergebnis ist deterministisch reproduzierbar | C | N-EN-023; N-LT-027 |
| T-04-04 | Beziehungswerte sind Zustände einer Beziehung, nicht einer Person | C | N-EN-024; Abschnitt 2 |
| T-04-05 | Ein Zustandswert bewegt sich pro Zeitschritt höchstens um eine feste Schrittweite (kein Sprung) | C | N-EN-025; N-PS-016 |
| T-04-06 | Die Persönlichkeit eines NPC ändert sich nie durch Zeit allein | C | N-EN-026; Abschnitt 6 |
| T-04-07 | Rauschzustand verschlechtert Wahrnehmung und Erinnerung messbar | C | N-EN-027; N-PS-011 |
| T-04-08 | Persönlichkeitsdimensionen sind kontinuierlich; Archetypen sind abgeleitet | C | N-EN-028; N-PS-005 |
| T-04-09 | Kein Zustandswert erscheint in der Spieler-Oberfläche | C, U | N-EN-029; N-LT-012 |

### T-05: Gedächtnis, Erinnerung, Vergessen

Gruppe zu [Modul 05](05-Gedaechtnis-Erinnerung-und-Vergessen.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-05-01 | Ein NPC weiß nur Dinge, für die ein Eintrag mit Quelle (gesehen, gehört, erzählt, Medien, abgeleitet) existiert | C | N-EN-041; N-GE-008 |
| T-05-02 | Der Inhalt eines Eintrags darf von der Wahrheit abweichen, der Verweis auf das Ereignis nicht | C | N-EN-042; N-GE-009 |
| T-05-03 | Ein Eintrag der Klasse `permanent` wird nie verworfen und verblasst im Kern nie | C | N-EN-043; N-GE-005 |
| T-05-04 | Ein NPC im Zeitstand T kennt keine Ereignisse nach T | C | N-EN-044; N-GE-028 |
| T-05-05 | Verdichtung entfernt nie einen Eintrag ab Wichtigkeit `notable` | C | N-EN-045; N-GE-022 |
| T-05-06 | Dieselbe Frage an denselben Zeugen im selben Weltzustand liefert dieselbe Antwort | C | N-EN-046; Ermittlung 12 §4 |
| T-05-07 | Verblassen verbraucht Weltzeit, keine Realzeit; ein beendetes Spiel verändert keinen Eintrag | C | N-EN-047; N-LT-020 |
| T-05-08 | Ein Ereignis ohne Beobachter erzeugt keinen Eintrag bei NPCs | C | N-EN-048; N-GE-008 |
| T-05-09 | Der Tod eines NPCs entfernt weder die Erinnerungen anderer an ihn noch die Beziehungsgeschichte | C | N-EN-049; N-GE-025, N-ID-018 |
| T-05-10 | Wiedererkennung ist abhängig von Bekanntheitsstufe, Zeit, Veränderung und Kontext und ist reproduzierbar | C | N-EN-050; N-GE-015 |
| T-05-11 | Konfidenz und Genauigkeit eines Eintrags sind nie in der Spieler-Oberfläche sichtbar | C, U | N-EN-051; N-LT-012 |
| T-05-12 | Ein wichtiger Eintrag mit Spielerbezug bleibt nach Speichern und Laden unverändert einzeln abrufbar | C | N-EN-052; N-GE-023 |
| T-05-13 | Ein NPC kann nicht über einen Weg erfahren, den es im Weltzustand nicht gibt (Ort, Kontakt, Medium) | C | N-EN-053; N-GE-008, Modul 07 |
| T-05-14 | Bei Verkleidung erinnert ein NPC die erlebte Rolle, nicht die wahre Identität | C | N-EN-054; N-GE-019 |
| T-05-15 | Ein Eintrag mit `suggested` als Quelle hat ein Ereignis im Journal, das die Manipulation belegt | C | N-EN-055; Abschnitt 11 |

### T-06: Wahrnehmung, Wissen, Glaube

Gruppe zu [Modul 06](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-06-01 | Jede Aussage eines NPC über ein Weltereignis hat einen Eintrag mit Quelle | C | N-EN-061; N-WA-010 |
| T-06-02 | Ein Beobachter ohne Reichweite (Sicht, Gehör, Kontakt) erfährt nichts | C | N-EN-062; N-WA-009, N-WA-010 |
| T-06-03 | Die Wahrnehmung hängt von Licht, Entfernung, Ablenkung und Zustand ab und liefert bei gleichen Eingaben gleiche Ergebnisse | C | N-EN-063; N-WA-009 |
| T-06-04 | Kein NPC verwendet Information, die er nicht auf einem plausiblen Weg bekommen hat (kein Schwarmwissen) | C | N-EN-064; N-WA-005, N-WA-006 |
| T-06-05 | Die Gewissheit über Position und Einzelheiten nimmt ohne neue Hinweise ab | C | N-EN-065; N-WA-004 |
| T-06-06 | Verdacht steigt nur durch einen Anlass und sinkt ohne Bestätigung | C | N-EN-066; Abschnitt 6 |
| T-06-07 | Eine Straftat ohne Zeugen löst keine Fahndung aus | C | N-EN-067; N-WA-013 |
| T-06-08 | Die Polizei beginnt mit der gewöhnlichen Erklärung, solange das Übernatürliche nicht bekannt ist | C | N-EN-068; N-WA-017 |
| T-06-09 | Das Übernatürliche wird nach dem Weltbild des NPC gedeutet; verschiedene NPCs deuten verschieden | C | N-EN-069; N-WA-018 |
| T-06-10 | Ein falscher Glaube lässt sich nur durch ein Ereignis ändern, das im Journal steht | C | N-EN-070; N-GE-009 |
| T-06-11 | Ein öffentliches Ereignis macht nicht automatisch alle Beobachter zu Gläubigen | C | N-EN-071; Abschnitt 4 |
| T-06-12 | Eine Verkleidung ändert Erwartung und Verdacht, nie die Identität im Weltzustand | C | N-EN-072; N-WA-014 |
| T-06-13 | Kameras liefern Aufzeichnungen, keine Erinnerungen, und nur im Sichtfeld | C | N-EN-073; Abschnitt 10 |
| T-06-14 | Wahrnehmung ferner NPCs wird ereignisbasiert abgeleitet und widerspricht nie dem sichtbaren Verhalten bei der Materialisierung | C, U | N-EN-074; Modul 22, `23#1057E` Punkte 122–123 |

### T-07: Informationsfluss, Gerüchte, Kommunikation

Gruppe zu [Modul 07](07-Informationsfluss-Geruechte-und-Kommunikation.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-07-01 | Eine Information erreicht einen NPC nur über einen Kanal, der im Weltzustand existiert und den NPC erreicht | C | N-EN-081; N-IN-001, N-IN-022 |
| T-07-02 | Ein Gerücht kann die Wahrheit verzerren, aber nicht aus dem Nichts eine Person erfinden, die es nicht gibt | C | N-EN-082; N-IN-002 |
| T-07-03 | Ein belauschtes Gespräch löst keinen Fall und keinen Hinweis aus, der nicht im Weltzustand begründet ist | C | N-EN-083; N-IN-004, N-LT-008 |
| T-07-04 | Die Ausbreitung ist durch den sozialen Graphen begrenzt; ein Gerücht erreicht keinen NPC ohne Verbindung | C | N-EN-084; N-IN-007, N-IN-008 |
| T-07-05 | Dasselbe Gerücht wird bei gleicher Eingabe gleich verzerrt (deterministisch) | C | N-EN-085; Ermittlung 12 §4 |
| T-07-06 | Ein Funkloch verzögert oder verhindert Nachrichten | C | N-EN-086; N-IN-016 |
| T-07-07 | Medienberichte erscheinen nach dem Ereignis, nie davor | C | N-EN-087; N-IN-019 |
| T-07-08 | Ein widerlegtes Gerücht bleibt in den Erinnerungen derer, die es glaubten, bis sie davon erfahren | C | N-EN-088; N-GE-009 |
| T-07-09 | Ein Geheimnisträger gibt ein Geheimnis nicht ohne Anlass weiter; ein Plauderer häufiger | C | N-EN-089; N-IN-009 |
| T-07-10 | Der Spieler kann ein Gerücht nicht ohne Mittel, Aufwand und Quelle korrigieren | C | N-EN-090; N-IN-012 |
| T-07-11 | Der Social-Media-Feed ist nie Voraussetzung für Hauptstory oder handgeschriebene Nebenfälle | C, I | N-EN-091; N-IN-014 |
| T-07-12 | Ein NPC verbreitet Wissen, das er hat; er erfindet keines | C | N-EN-092; N-GE-008 |
| T-07-13 | Öffentliche Bekanntheit erhöht die Verbreitungsrate, ändert aber nicht die Wahrheit des Inhalts | C | N-EN-093; N-IN-010 |

### T-08: Beziehungen: Modell und Wirkung

Gruppe zu [Modul 08](08-Beziehungen-Modell-und-Wirkung.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-08-01 | Ein Typwechsel einer Beziehung hat ein Ereignis im Journal als Ursache | C | N-EN-111; Abschnitt 4 |
| T-08-02 | Eine Beziehungskante enthält nur die Wahrnehmung des Quell-NPCs, nie die Wahrheit über das Ziel | C | N-EN-112; Abschnitt 2 |
| T-08-03 | Dimensionen sind unabhängig; keine Operation darf sie zu einem einzigen Wert zusammenfassen, der Verhalten allein bestimmt | C | N-EN-113; N-BE-004 |
| T-08-04 | Keine Beziehungsdimension erscheint als Zahl oder Balken in der Spieler-Oberfläche | C, U | N-EN-114; N-BE-001 |
| T-08-05 | Eine Beziehungsänderung hat ein Ereignis, das der Beteiligte erfahren hat | C | N-EN-115; N-GE-008 |
| T-08-06 | Dasselbe Ereignis ändert zwei verschiedene Kanten unterschiedlich, wenn Kern, Erwartung und Zuschreibung unterschiedlich sind | C | N-EN-116; N-LT-027 |
| T-08-07 | Gruppenhaltung und individuelle Beziehung können widersprechen | C | N-EN-117; N-BE-020 |
| T-08-08 | Wird eine Kante auf falscher Zuschreibung aufgebaut, ändert sich die Wahrheit im Weltzustand nicht | C | N-EN-118; Abschnitt 6 |
| T-08-09 | Eine Beziehungsstufe wird nicht durch einen einzelnen Datenpunkt verlassen (Hysterese) | C | N-EN-119; Abschnitt 5 |
| T-08-10 | Der Tod einer Person friert die Kante ein; die Dimensionen ändern sich danach nur durch Erinnerung | C | N-EN-120; N-GE-025 |
| T-08-11 | Eine detaillierte Kante zwischen zwei Hintergrund-NPCs entsteht nur durch ein Ereignis, das sie rechtfertigt | C | N-EN-121; `35#278A` Frage 27 |
| T-08-12 | Eine einseitige Beziehung wird von beiden Seiten verschieden erlebt und gespielt | C | N-EN-122; Abschnitt 10 |
| T-08-13 | Eine Beziehung zu einem NPC hängt nicht von einem Zufallswurf ab, sondern von Ereignissen und Zustand | C | N-EN-123; N-PS-016 |

### T-09: Beziehungsentwicklung

Gruppe zu [Modul 09](09-Beziehungsentwicklung-Freundschaft-Feindschaft-und-Vertrauen.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-09-01 | Jede Änderung einer Beziehungsdimension hat ein Ereignis, das der Beteiligte kennt | C | N-EN-131; N-BE-001, N-GE-008 |
| T-09-02 | Vergebung ist nicht allein von der Zeit abhängig | C | N-EN-132; N-BE-013 |
| T-09-03 | Ein NPC verzeiht, was in seinem Kern ‚nie verziehen‘ ist, nicht | C | N-EN-133; N-BE-013 |
| T-09-04 | Eine Feindschaft eskaliert nur durch ein Ereignis | C | N-EN-134; N-LT-023 |
| T-09-05 | Eine falsche Anschuldigung kann eine Beziehung beschädigen, ohne dass die Wahrheit sich ändert | C | N-EN-135; N-BV-010, N-BV-013 |
| T-09-06 | NPC-NPC-Beziehungen verändern sich im Hintergrund mit einer Ursache im Journal | C | N-EN-136; N-BV-018 |
| T-09-07 | Die Welt zählt Beziehungsereignisse nicht als ‚Punkte‘; es gibt kein Freischalten durch Beziehungsstufe | C | N-EN-137; Abschnitt 4 |
| T-09-08 | Ein verpasstes Zeitfenster entfernt keine Beziehungsaktivität | C | N-EN-138; N-BV-020, N-LT-035 |
| T-09-09 | Ein Ereignis wirkt auf Betroffene, Zeugen und Dritte getrennt und deterministisch | C | N-EN-139; Abschnitt 2 |
| T-09-10 | Ein Verhaltensbild (Reaktionstabelle) hängt von Persönlichkeit, Beruf und Lage ab, nicht von einem Zufallswurf | C | N-EN-140; Abschnitt 3 |
| T-09-11 | Schuld sinkt durch Einlösung und kann nicht ohne Ereignis verschwinden | C | N-EN-141; Abschnitt 7 |
| T-09-12 | Eine Fernbeziehung bleibt eine Beziehung; ihre Pflege geschieht durch Kontakt | C | N-EN-142; Abschnitt 8 |

### T-10: Familien, Haushalte, Netzwerke

Gruppe zu [Modul 10](10-Familien-Haushalte-und-soziale-Netzwerke.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-10-01 | Jeder NPC mit Stufe A bis C gehört zu genau einem Haushalt (oder ist obdachlos mit Stammplatz) | C | N-EN-161; N-FA-015 |
| T-10-02 | Ein Haushalt hat keine Mitglieder, die tot oder ausgezogen sind, ohne entsprechendes Ereignis | C | N-EN-162; N-FA-003 |
| T-10-03 | Heirat, Trennung, Geburt, Tod und Umzug sind Ereignisse im Journal | C | N-EN-163; N-FA-003, N-FA-008 |
| T-10-04 | Eine Verwandtschaftskante ändert sich nur durch ein Ereignis (Adoption, Entdeckung) | C | N-EN-164; Abschnitt 3 |
| T-10-05 | Die Bevölkerung wird bei Tod, Zuzug und Geburt konsistent bilanziert, sobald Generationen aktiviert sind | C | N-EN-165; N-FA-005 |
| T-10-06 | Kinder sind vom Adult-System ausgeschlossen | C, I | N-EN-166; N-FA-019 |
| T-10-07 | Ein Umzug ändert Netz und Routine, nicht ID und Historie | C | N-EN-167; N-ID-019 |
| T-10-08 | Feiertage verändern Routinen von Familien nachvollziehbar | C | N-EN-168; N-FA-012 |
| T-10-09 | Ein Ort behält seine Geschichte bei Betreiberwechsel | C | N-EN-169; N-GE-026 |
| T-10-10 | Die Familie eines Opfers erfährt vom Tod über einen existierenden Weg | C | N-EN-170; N-GE-008 |
| T-10-11 | Keine Genealogie wird für NPCs vorausgesetzt, die sie nicht brauchen | C | N-EN-171; Abschnitt 3 |

### T-11: Nemesis: Modell und Datensatz

Gruppe zu [Modul 11](11-Nemesis-Modell-und-Datensatz.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-11-01 | Ein Nemesis lernt nur Dinge, für die es im Weltzustand eine Beobachtung oder plausible Weitergabe gibt | C | N-EN-181; N-NE-008, N-WA-005 |
| T-11-02 | Kein Wert des Nemesis steigt allein durch die Zahl der Begegnungen | C | N-EN-182; N-NE-017 |
| T-11-03 | Eine Anpassung hat eine Ursache in einem Begegnungs-Datensatz | C | N-EN-183; Abschnitt 5 |
| T-11-04 | Eine Anpassung bleibt innerhalb der Artregeln und Schwächen | C | N-EN-184; N-NE-020 |
| T-11-05 | Ein toter Nemesis erscheint nicht wieder (außer artspezifische Ausnahme laut Bestiarium) | C | N-EN-185; N-NE-012 |
| T-11-06 | Eine zurückkehrende Figur zeigt ihre früheren Verletzungen | C | N-EN-186; N-NE-013 |
| T-11-07 | Dasselbe Modell und derselbe Weltzustand liefern dieselbe Anpassung | C | N-EN-187; Abschnitt 9 |
| T-11-08 | Ein Nemesis verwendet keine Information über den Spieler, die er nie erhielt | C | N-EN-188; N-WA-005 |
| T-11-09 | Die Zahl gleichzeitig aktiver Nemesis-Figuren und ihre Auftrittsabstände sind begrenzt | C | N-EN-189; Abschnitt 11 |
| T-11-10 | Ein Nemesis kann keine Hauptmission blockieren | C, I | N-EN-190; Abschnitt 12 |
| T-11-11 | Der Tod eines Anführers löst die Gruppe nicht auf | C | N-EN-191; Abschnitt 10 |
| T-11-12 | Ein Nemesis hat keinen Marker in der Spieler-Oberfläche | C, U | N-EN-192; Abschnitt 11 |
| T-11-13 | Die Wirkung einer Rettung oder Verschonung ändert die Beziehungskante nachvollziehbar | C | N-EN-193; Modul 08 |
| T-11-14 | Jeder Nemesis hat mindestens ein Vorzeichen (Spur, Gerücht, Handschrift), bevor er auftritt | C | N-EN-194; Abschnitt 11 |
| T-11-15 | Ein Nemesis in Gefangenschaft nimmt nicht an Außenaktionen teil | C | N-EN-195; Modul 12 |
| T-11-16 | Ein Nemesis mit `story_safe` oder `authored` kann nur durch Spieler oder Geschichte sterben | C | N-EN-196; Modul 03 §8 |

### T-12: Nemesis: Lebenszyklus

Gruppe zu [Modul 12](12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-12-01 | Jede Phase des Lebenszyklus erzeugt ein Ereignis im Journal | C | N-EN-211; Abschnitt 2 |
| T-12-02 | Ein Gegner, der die Begegnung überlebt, behält seine Verletzungen | C | N-EN-212; N-NL-006, N-NL-007 |
| T-12-03 | Eine Rückkehr geschieht nur, wenn Motiv, Mittel, Wissen und Gelegenheit bestehen | C | N-EN-213; Abschnitt 6 |
| T-12-04 | Eine Rückkehr erfolgt nicht in Story-Sequenzen oder Hauptmissionen, wenn dies die Erzählung stört | C, I | N-EN-214; Abschnitt 6.1 |
| T-12-05 | Die Hintergrundentwicklung ist deterministisch und verbraucht nur Weltzeit | C | N-EN-215; N-NL-011 |
| T-12-06 | Eine Hintergrundaktivität erzeugt eine überprüfbare Spur im Weltzustand | C | N-EN-216; Abschnitt 5 |
| T-12-07 | Der Nemesis kann Mittel nur aus dem Weltzustand beziehen | C | N-EN-217; Abschnitt 5 |
| T-12-08 | Ein verletzter Nemesis tritt nicht vor Heilung oder Kompensation in Konfrontationen auf | C | N-EN-218; Abschnitt 4 |
| T-12-09 | Ein Wendepunkt hat eine Ursache im Journal | C | N-EN-219; Abschnitt 8 |
| T-12-10 | Der Tod eines Nemesis löscht keine Erinnerungen anderer an ihn | C | N-EN-220; N-GE-025 |
| T-12-11 | Eine Anpassung kann durch einen Methodenwechsel des Spielers unwirksam werden | C | N-EN-221; Abschnitt 7 |
| T-12-12 | Gleichzeitig aktive Nemesis-Figuren liegen unter der festgelegten Obergrenze | C | N-EN-222; Abschnitt 11 |
| T-12-13 | Ein gefangener Nemesis führt keine Außenaktionen aus | C | N-EN-223; Abschnitt 3 |
| T-12-14 | Ein Nemesis erscheint nicht in einer Region, die er nicht erreichen kann | C | N-EN-224; Abschnitt 6.1 |
| T-12-15 | Ein Nemesis erkennt den Spieler nur mit Hilfe einer Erinnerung oder neuen Wahrnehmung | C | N-EN-225; N-GE-015 |
| T-12-16 | Die Verschonung eines Gegners erzeugt eine Beziehungsänderung, deren Richtung von Persönlichkeit und Deutung abhängt | C | N-EN-226; Abschnitt 8 |

### T-13: Autonomes Leben

Gruppe zu [Modul 13](13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-13-01 | Ein NPC befindet sich zu jeder Weltzeit an genau einem Ort oder unterwegs | C | N-EN-241; N-LE-014 |
| T-13-02 | Ein Weg zwischen zwei Orten kostet Weltzeit, auch im Hintergrund | C | N-EN-242; N-LE-014 |
| T-13-03 | Der Zeitplan eines NPC ist bei gleichem Weltzustand und gleichem Seed reproduzierbar | C | N-EN-243; N-LE-015 |
| T-13-04 | Eine Abweichung vom Normalplan hat eine Ursache und ein Ende | C | N-EN-244; N-LE-002 |
| T-13-05 | Ein Arbeitsplatz-NPC erscheint zu den Schichtzeiten am Arbeitsplatz oder hat eine dokumentierte Abweichung | C | N-EN-245; N-LE-003 |
| T-13-06 | Eine Materialisierung ist mit der letzten Zustandsaufzeichnung vereinbar | C, U | N-EN-246; N-LE-013 |
| T-13-07 | Beim Betreten eines Gebiets sind Straßen entsprechend der Tageszeit belebt | C, U | N-EN-247; N-LE-026 |
| T-13-08 | Ein NPC reist nur mit einem verfügbaren Verkehrsmittel | C | N-EN-248; Abschnitt 6 |
| T-13-09 | Wer verreist ist, ist an seinem Heimatort abwesend | C | N-EN-249; Abschnitt 6 |
| T-13-10 | Ein Umzug ist selten und hat Ursachen | C | N-EN-250; N-LE-009 |
| T-13-11 | Der tatsächliche Aufenthalt wichtiger Personen ist für Alibi-Prüfungen abrufbar | C | N-EN-251; Ermittlung 12 §8 |
| T-13-12 | Ein Betrieb hat Mitarbeiter, die zu seinen Öffnungszeiten arbeiten, oder einen dokumentierten Ausfall | C | N-EN-252; N-LE-010 |
| T-13-13 | Feiertage verändern Öffnungszeiten und Routinen deterministisch | C | N-EN-253; N-LE-018 |
| T-13-14 | Begleiter und Crew haben Abläufe außerhalb der Anwesenheit des Spielers | C | N-EN-254; N-LE-020, N-LE-022 |
| T-13-15 | Die Hintergrundsimulation verbraucht nur Weltzeit | C | N-EN-255; N-LE-027 |

### T-14: Lebensereignisse, Alterung, Tod

Gruppe zu [Modul 14](14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-14-01 | Jede Änderung im Lebenslauf eines NPCs hat ein Ereignis mit Ursache | C | N-EN-261; N-LB-011 |
| T-14-02 | Das Alter eines NPCs ergibt sich aus Geburts- und Weltdatum | C | N-EN-262; Abschnitt 3 |
| T-14-03 | Haar- und Bartlänge sind Funktionen der Weltzeit seit dem letzten Schnitt | C | N-EN-263; N-LB-004 |
| T-14-04 | Eine Frisur kann nicht länger sein als die vorhandene Haarlänge | C | N-EN-264; N-LB-004 |
| T-14-05 | Eine Narbe verschwindet nicht ohne Ereignis | C | N-EN-265; N-LB-005 |
| T-14-06 | Ein NPC im Zeitstand T hat sein damaliges Alter | C | N-EN-266; N-GE-028 |
| T-14-07 | Ein Umzug ist ein Ereignis mit Ursache und ändert Haushalt, Wohnort, Netz | C | N-EN-267; N-LE-009 |
| T-14-08 | Ein verletzter NPC durchläuft die Genesungsstufen in Weltzeit | C | N-EN-268; N-LB-014 |
| T-14-09 | Ein schwer Verletzter ohne Behandlung kann sterben | C | N-EN-269; N-LB-013 |
| T-14-10 | Die Zahl der Todesfälle ohne Ursache beträgt null | C | N-EN-270; N-LB-016 |
| T-14-11 | Ein toter NPC hat einen Datensatz, aber keine Handlungen | C | N-EN-271; N-ID-018 |
| T-14-12 | Ein geschützter NPC stirbt nicht außerhalb der erlaubten Wege | C | N-EN-272; N-LB-017 |
| T-14-13 | Eine Bevölkerungsbilanz (aktiviert) bleibt innerhalb vorgegebener Grenzen | C | N-EN-273; N-LB-019 |
| T-14-14 | Die Todesnachricht erreicht Angehörige über einen existierenden Weg und erzeugt Trauer, Erbe und Haushaltsänderung | C | N-EN-274; N-LB-016 |

### T-15: Reaktion auf Weltereignisse

Gruppe zu [Modul 15](15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-15-01 | Ein NPC reagiert nur auf Ereignisse, die er wahrgenommen oder erfahren hat | C | N-EN-286; N-GE-008 |
| T-15-02 | Dasselbe Ereignis kann verschiedene Reaktionen auslösen, jede ist reproduzierbar | C | N-EN-287; N-WE-004 |
| T-15-03 | Die Rolle eines NPC (Beruf, Verantwortung) wirkt auf die Reaktion nachvollziehbar | C | N-EN-288; Abschnitt 2 |
| T-15-04 | Meidung entsteht aus Ereignissen und klingt ohne neue Ereignisse ab | C | N-EN-289; N-WE-001 |
| T-15-05 | Eine Gemeinschaftsvariable ändert sich nur durch Ereignisse und Gerüchte | C | N-EN-290; Abschnitt 5 |
| T-15-06 | Die Polizei beginnt mit der gewöhnlichen Erklärung | C | N-EN-291; N-WE-003 |
| T-15-07 | Eine dauerhafte Veränderung eines NPCs hat ein auslösendes Erlebnis | C | N-EN-292; N-WE-005 |
| T-15-08 | Große übernatürliche Ereignisse sind selten und zeitlich begrenzt | C | N-EN-293; N-WE-007 |
| T-15-09 | Ein Ereignis ohne Beobachter erzeugt keine Reaktion bei NPCs | C | N-EN-294; N-GE-008 |
| T-15-10 | Reparaturen werden von NPC-Teams ausgeführt und brauchen Weltzeit | C | N-EN-295; N-WE-009 |
| T-15-11 | Ein Schaden bleibt bis zur Reparatur sichtbar | C, U | N-EN-296; N-WE-009 |
| T-15-12 | Das Weltbild eines NPC bestimmt die Deutung, nicht die Wahrheit | C | N-EN-297; N-WE-004 |
| T-15-13 | Menschenmengen entstehen aus Ortszustand und Plänen, nicht aus Zufall | C | N-EN-298; N-WE-011 |
| T-15-14 | Ein Monsterfall verändert mindestens einen Alltagsparameter einer Gemeinschaft, wenn er öffentlich wahrnehmbar war | C, U | N-EN-299; N-WE-008, N-WE-017 |

### T-16: Fraktionen: Modell, Mitgliedschaft, Hierarchie

Gruppe zu [Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-16-01 | Keine Hauptmission ist von Fraktionsmitgliedschaft oder Rekrutierung abhängig | C, I | N-EN-306; N-FR-003 |
| T-16-02 | Eine Fraktionsentscheidung folgt der Entscheidungsform der Gruppe, nicht dem Spielerwunsch | C | N-EN-307; Abschnitt 4 |
| T-16-03 | Ein Rang ändert sich nur durch ein Ereignis mit Ursache | C | N-EN-308; N-FR-011 |
| T-16-04 | Der Rang ist nicht allein durch Zeit oder Erfahrungspunkte bestimmt | C | N-EN-309; N-FR-012 |
| T-16-05 | Gruppenruf und individuelle Beziehung können verschieden sein | C | N-EN-310; N-FR-023 |
| T-16-06 | Eine Fraktion hat jederzeit eine gültige Führung oder einen dokumentierten Führungsausfall | C | N-EN-311; Abschnitt 7 |
| T-16-07 | Ein Mitglied verlässt die Gruppe mit einem Ereignis; Besitz, Rechte und Beziehungen ändern sich nachvollziehbar | C | N-EN-312; N-FR-018 |
| T-16-08 | Eine verdeckte Mitgliedschaft ist für Nichteingeweihte nicht erkennbar, aber im Datensatz vorhanden | C | N-EN-313; Abschnitt 5 |
| T-16-09 | Fraktionsbeziehungen wechseln nur über Ereignisse und mit Hysterese | C | N-EN-314; Abschnitt 8 |
| T-16-10 | Zwei rivalisierende Mitgliedschaften des Spielers erzeugen Prüfung und Folgen | C | N-EN-315; N-FR-016 |
| T-16-11 | Eine Fraktion verwendet nur Mittel aus dem Weltzustand | C | N-EN-316; N-LT-008 |
| T-16-12 | Gruppenwissen ist durch Rollen begrenzt; ein Mitglied weiß nicht automatisch, was die Führung weiß | C | N-EN-317; Modul 05 §10 |
| T-16-13 | Ein Führungswechsel erzeugt Nachfolge, Machtkampf oder Zerfall, nie Leere | C | N-EN-318; N-FR-015 |
| T-16-14 | Der Austritt löscht keine Tattoos, Narben oder Erinnerungen | C, I | N-EN-319; N-FR-018 |
| T-16-15 | Ein Spieler in Führungsposition löst Aufgaben selbst; es gibt kein erzwungenes Mikromanagement | C | N-EN-320; N-FR-022 |
| T-16-16 | Fraktionen reagieren auf Spielerhandlungen nur, wenn sie davon erfahren haben | C | N-EN-321; N-GE-008 |

### T-17: Fraktionen: Territorien, Konflikte, Macht

Gruppe zu [Modul 17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-17-01 | Eine Fraktion hält nur Gebiete, die ihre Mitglieder erreichen können | C | N-EN-336; N-TE-014 |
| T-17-02 | Ein Anführer-Tod führt zu Nachfolge, Machtkampf oder Zerfall, nie zum spurlosen Verschwinden der Gruppe | C | N-EN-337; N-TE-005 |
| T-17-03 | Eskalationsstufen werden auch in groben Zeitschritten nacheinander durchlaufen | C | N-EN-338; Abschnitt 5 |
| T-17-04 | Ein Strategiezustand wechselt nicht ohne Auslöser und Mindestdauer | C | N-EN-339; Abschnitt 4 |
| T-17-05 | Relative Macht ist abgeleitet und erscheint nicht in der Spieler-Oberfläche | C, U | N-EN-340; Abschnitt 6 |
| T-17-06 | Die Fernsimulation führt Fraktionen als Zustand; beim Betreten der Region sind Mitglieder konsistent zu diesem Zustand | C | N-EN-341; N-TE-004, Modul 22 |
| T-17-07 | Eine Fraktion handelt nur mit Ressourcen, die sie hat | C | N-EN-342; Abschnitt 3 |
| T-17-08 | Ein Konflikt hat einen Anlass und eine Ursache im Journal | C | N-EN-343; Abschnitt 5 |
| T-17-09 | Folgen eines Konflikts (Vorfälle, Meidung, Gerüchte) treten zeitlich nach dem Konflikt auf | C | N-EN-344; Abschnitt 7 |
| T-17-10 | Ein vollständig vernichtetes Nest erscheint nicht sofort wieder | C | N-EN-345; N-TE-013 |
| T-17-11 | Monsterterritorien erscheinen nicht als Kartenebene | C, U | N-EN-346; Abschnitt 2 |
| T-17-12 | Menschliche Einflussbereiche sind nur für Mitglieder auf der Karte sichtbar | C, U | N-EN-347; N-TE-006 |
| T-17-13 | Ethnische Zugehörigkeit ist kein Faktor der Kriminalitäts- oder Machtberechnung | C | N-EN-348; N-TE-008 |
| T-17-14 | Dieselben Eingaben liefern dieselbe Machtverschiebung | C | N-EN-349; Abschnitt 9 |
| T-17-15 | Eine Gruppe wird nicht gleichzeitig an zwei weit entfernten Orten stark simuliert, ohne dass Mitglieder dort sind | C | N-EN-350; Abschnitt 9 |

### T-18: Behörden, Hunter-Organisationen, Wesen

Gruppe zu [Modul 18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-18-01 | Eine Behörde handelt nur auf Grundlage tatsächlich vorliegender Information | C | N-EN-366; N-BH-006 |
| T-18-02 | Eine Zuständigkeit bestimmt, welche Behörde eine Meldung bearbeitet; Informationsweitergabe zwischen Behörden braucht einen Weg | C | N-EN-367; N-BH-001 |
| T-18-03 | Wiederholte Konflikte mit einer Dienststelle verändern deren Verhalten gegenüber dem Spieler nachvollziehbar | C | N-EN-368; N-BH-007 |
| T-18-04 | Eine schwere Fahndung endet nur durch einen definierten Abschluss (Kopfgeld, Rücknahme, Freispruch, Verfahren) | C | N-EN-369; N-BH-004 |
| T-18-05 | Die Polizei kennt das Übernatürliche nur, wenn der Datensatz es erlaubt | C | N-EN-370; N-BH-008 |
| T-18-06 | Ein Beamter handelt nach Persönlichkeit, Gedächtnis und Beziehung, nicht nur nach Dienstregel | C | N-EN-371; Abschnitt 3 |
| T-18-07 | Ein NPC-Hunter folgt eigenen Zielen; er löst keine Fälle zum Vorteil des Spielers, nur weil dieser sie ignoriert | C | N-EN-372; N-BH-013 |
| T-18-08 | Ein NPC-Hunter kann sterben und hinterlässt Spuren (Erinnerungen, Archivlücken, Fälle) | C | N-EN-373; N-BH-013 |
| T-18-09 | Ein verschontes Wesen wird nicht gezwungen, Feind zu werden | C | N-EN-374; N-BH-016 |
| T-18-10 | Das Wissen einer Behörde ist ein Pool mit Rollenrechten | C | N-EN-375; Modul 05 §10 |
| T-18-11 | Staatliche Geheimprogramme sind Spielinhalt der Welt, nicht Pflicht für Hauptstory-Abläufe | C, I | N-EN-376; N-BH-018 |
| T-18-12 | Medienberichte und Behördenmitteilungen widersprechen sich nur mit erkennbarem Grund | C | N-EN-377; N-BH-017 |
| T-18-13 | Wissenszentrierte Organisation weiß nicht alles | C | N-EN-378; N-BH-012 |
| T-18-14 | Ein Wesen mit menschlicher Identität hat auffindbare Spuren dieser Identität | C | N-EN-379; Kreaturen 09 |
| T-18-15 | Die Aufdeckung eines Geheimnisses ändert Beziehungskanten nachvollziehbar | C | N-EN-380; Abschnitt 8 |

### T-19: Begleiter, Crew, Rekrutierung

Gruppe zu [Modul 19](19-Begleiter-Crew-und-Rekrutierung.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-19-01 | Ein Begleiter lehnt eine Handlung nur aus Persönlichkeit, Doktrin oder Lage ab, mit sichtbarem Grund | C, U | N-EN-391; N-CO-004 |
| T-19-02 | Ein Begleiter weicht nur bei unmittelbarer Gefahr oder neuer Information vom Plan ab | C | N-EN-392; N-CO-011 |
| T-19-03 | Ein Begleiter sabotiert nie zufällig | C | N-EN-393; N-CO-011 |
| T-19-04 | Ein verletzter Begleiter steht in der Zeit seiner Genesung nicht zur Verfügung | C | N-EN-394; N-CO-006 |
| T-19-05 | Ein gewöhnlicher Begleiter bleibt nach dem Tod tot | C | N-EN-395; N-CO-007 |
| T-19-06 | Ein Kernmitglied des HQ stirbt nicht durch Sandbox-Zufall | C | N-EN-396; N-CO-007 |
| T-19-07 | Ein Todesfall in der Crew führt zu Reaktionen von Crew, HQ und Beziehungen | C | N-EN-397; N-CO-007, N-LB-016 |
| T-19-08 | Persönliche Anfragen entfallen nur durch Tod oder dauerhaftes Gehen | C | N-EN-398; N-CO-017 |
| T-19-09 | Crew-Aktivitäten verschwinden nicht durch ein verpasstes Zeitfenster | C | N-EN-399; N-CO-018 |
| T-19-10 | Nur als rekrutierbar definierte NPCs können rekrutiert werden | C, I | N-EN-400; N-CO-022 |
| T-19-11 | Rekrutierung folgt einer Kette von Beziehung, Wissen, Gelegenheit und Entscheidung | C, I | N-EN-401; Abschnitt 7 |
| T-19-12 | Ein Rekrut bringt die Fähigkeiten, Beziehungen und Konflikte seiner Herkunft mit | C, I | N-EN-402; N-CO-023 |
| T-19-13 | Die Crew reagiert auf einen Rekruten nach Persönlichkeit und Erinnerung, nicht einheitlich | C, I | N-EN-403; N-CO-023 |
| T-19-14 | Crewmitglieder haben Abläufe außerhalb der Anwesenheit des Spielers | C | N-EN-404; N-CO-015 |
| T-19-15 | Der Hund ist umweltbezogener Spurenfolger, kein Zielpfeil | C | N-EN-405; N-CO-014 |
| T-19-16 | Kein Hauptmissionsabschnitt hängt von einem Rekruten ab | C, I | N-EN-406; N-CO-021 |
| T-19-17 | Ein Begleiter, der abwesend ist, wird bei Rückkehr aus seinem Zustand konsistent dargestellt | C | N-EN-407; N-LE-013 |
| T-19-18 | Ein Begleiter erinnert Rettung, Verlassen, Lüge und Verletzungen wegen Entscheidungen | C | N-EN-408; N-BV-004 |

### T-20: Dialog und Sichtbarkeit

Gruppe zu [Modul 20](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-20-01 | Dialogbedingungen verwenden Bänder und Tags, keine Rohzahlen | C, U | N-EN-421; N-DI-002 |
| T-20-02 | Eine Änderung eines Beziehungsbandes ist über mindestens einen Kanal wahrnehmbar | C, U | N-EN-422; N-DI-017 |
| T-20-03 | Keine Zeile zeigt eine Beziehungszahl, Prozent oder Balken | C, U | N-EN-423; N-LT-012 |
| T-20-04 | Ein Erinnerungs-Hook nennt nur Ereignisse, die im Weltzustand existieren und die der NPC wahrgenommen hat | C | N-EN-424; N-GE-008 |
| T-20-05 | Ein Gespräch behauptet nichts, was der Dialogkontext nicht stützt | C, U | N-EN-425; N-DI-016 |
| T-20-06 | Dieselbe Eingabe liefert dieselbe Zeilenauswahl | C, U | N-EN-426; Modul 22 |
| T-20-07 | Es gibt für jedes Thema eine Fallback-Zeile | C, U | N-EN-427; Abschnitt 3 |
| T-20-08 | Ein Hook wird nicht bei jedem Gespräch wiederholt | C | N-EN-428; Abschnitt 5 |
| T-20-09 | Erinnerungsunsicherheit wirkt auf die Formulierung | C | N-EN-429; N-GE-011 |
| T-20-10 | Eine Zeile mit geändertem Text markiert vorhandene Aufnahmen als veraltet | C, U | N-EN-430; N-DI-014 |
| T-20-11 | Die Inspektoren sind im Shipping-Modus nicht erreichbar | C, U | N-EN-431; N-DI-018, N-DI-019 |
| T-20-12 | Das Dialogsystem verändert den Weltzustand ausschließlich über Ereignisse | C, U | N-EN-432; Abschnitt 2 |
| T-20-13 | NPCs ohne Wissen über ein Thema antworten nicht, als wüssten sie es | C | N-EN-433; N-GE-008 |
| T-20-14 | Wichtige Zustände sind nicht ausschließlich über Ton oder Farbe vermittelt | C | N-EN-434; Abschnitt 6 |

### T-21: Weltpersistenz und Speicher

Gruppe zu [Modul 21](21-Weltpersistenz-und-Speicherentwurf.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-21-01 | Gleicher Weltseed und gleiche NPC-ID liefern auf allen Plattformen denselben abgeleiteten Standardzustand | C | N-EN-441; N-PE-007, N-PE-008 |
| T-21-02 | Ein unverändert gebliebener NPC erzeugt keinen Speicher außer ID/Seed-Bezug | C | N-EN-442; N-PE-007 |
| T-21-03 | Speichern und Laden ergibt gleichen Zustand (Round-Trip) für alle Segmente | C | N-EN-443; N-PE-022 |
| T-21-04 | Ein alter Spielstand wird deterministisch migriert und danach in der neuen Version unverändert geladen | C | N-EN-444; N-PE-011 |
| T-21-05 | Ein unbekanntes Segment oder eine unbekannte Datenart wird unverändert gespeichert | C | N-EN-445; N-PE-018 |
| T-21-06 | Ein abgebrochener Schreibvorgang lässt den letzten gültigen Spielstand intakt | C | N-EN-446; N-PE-003 |
| T-21-07 | Jeder Verweis in einem Segment zeigt auf einen existierenden Datensatz oder einen Tombstone | C | N-EN-447; Abschnitt 8 |
| T-21-08 | Verdichtung entfernt keinen Anker und kein Ereignis, auf das ein Fall verweist | C | N-EN-448; Abschnitt 4.2 |
| T-21-09 | Der abgeleitete Zustand aus dem Journal gleicht dem gespeicherten Cache | C | N-EN-449; Abschnitt 4.3 |
| T-21-10 | Ein eingefrorener NPC wird durch eine Generatorversion nicht verändert | C | N-EN-450; N-PE-014 |
| T-21-11 | Zeitsprünge verbrauchen Weltzeit, keine Realzeit, und holen Ereignisse nach | C | N-EN-451; N-PE-013 |
| T-21-12 | Der Tod eines NPCs bleibt nach Speichern und Laden erhalten | C | N-EN-452; N-PE-015 |
| T-21-13 | Ein Spielstand mit fehlendem kritischem Mod lädt nicht ohne Expertenübersteuerung nach Backup | C | N-EN-453; N-PE-018 |
| T-21-14 | Hochfrequenzzustände (Positionen, Animation) werden nicht gespeichert | C, U | N-EN-454; N-PE-020 |
| T-21-15 | Gespeicherte Datenmenge steigt nicht mit der Zahl unveränderter Identitäten | C | N-EN-455; N-PE-007 |
| T-21-16 | Eine Obergrenze je NPC verhindert unbegrenztes Wachstum, ohne Anker zu verlieren | C | N-EN-456; Abschnitt 5.3 |
| T-21-17 | Jedes Segment trägt eine Schemaversion | C | N-EN-457; Abschnitt 6.3 |
| T-21-18 | Der Weltzustand lädt eine Region nicht vor dem Manifest | C | N-EN-458; Abschnitt 6.2 |
| T-21-19 | Das Nachholen von Ereignissen für nicht geladene Entitäten folgt einer definierten Reihenfolge (Weltzeit, dann ID) und liefert dasselbe Ergebnis wie der Live-Verlauf | C | N-EN-461; Abschnitt 7 |

### T-22: Simulationsstufen und Skalierung

Gruppe zu [Modul 22](22-Simulationsstufen-Skalierung-und-Performance.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-22-01 | Fortschreiben ist reihenfolgeunabhängig: Das Ergebnis hängt von Seed, Fenster und Weltzustand ab, nicht von der Aufrufreihenfolge | C | N-EN-471; N-SI-012 |
| T-22-02 | Ein Ereignis wird erst verarbeitet, wenn alle Ereignisse mit früherem Zeitstempel für diesen Empfänger verarbeitet sind | C | N-EN-472; Abschnitt 3.3 |
| T-22-03 | Ein Zustandswechsel S0↔S1↔S2↔S3 verliert keine wichtigen Ergebnisse (Verletzung, Zusage, Gedächtnis) | C | N-EN-473; N-SI-018 |
| T-22-04 | Die Materialisierung ist mit dem letzten Zustand vereinbar (Ort, Aktivität, Zustand) | C, U | N-EN-474; N-SI-005 |
| T-22-05 | Beim Betreten eines Gebiets ist die Straße der Tageszeit gemäß belebt | C, U | N-EN-475; N-SI-005 |
| T-22-06 | Kein NPC ist in der Simulation an zwei Orten gleichzeitig | C | N-EN-476; N-SI-011 |
| T-22-07 | Wege kosten Weltzeit, auch ohne Route | C | N-EN-477; N-SI-011 |
| T-22-08 | Das Fortschreiben läuft nie der Weltuhr voraus | C | N-EN-478; N-SI-019 |
| T-22-09 | Dieselben Eingaben liefern auf Windows und Linux dieselben Weltzustandsentscheidungen | C | N-EN-479; Abschnitt 7 |
| T-22-10 | Ein NPC im Gespräch wechselt nicht die Stufe | C | N-EN-480; Abschnitt 5 |
| T-22-11 | Die Weltgröße ist in keiner Simulationsgröße fest verdrahtet | C | N-EN-481; N-SI-001 |
| T-22-12 | Unter Last verändert sich Darstellung und Dichte, nicht der Weltzustand | C | N-EN-482; Abschnitt 8 |
| T-22-13 | Lange Pausen prüfen Eskalationsstufen nacheinander | C | N-EN-483; Abschnitt 3.4 |
| T-22-14 | Die Kosten der Fortschreibung wachsen nicht mit der Zahl unveränderter Identitäten | C | N-EN-484; N-PE-007 |
| T-22-15 | Die Simulation läuft nur auf der Autorität (Host) | C | N-EN-485; Abschnitt 10 |
| T-22-16 | Ein NPC mit höherer Priorität verdrängt einen mit niedrigerer aus dem Budget, nie umgekehrt | C | N-EN-486; Abschnitt 5 |
| T-22-17 | Hysterese verhindert Stufenwechsel bei Grenzbewegung | C | N-EN-487; Abschnitt 5 |
| T-22-18 | Der Replay-Test eines Zeitraums liefert denselben Zustand | C | N-EN-488; Abschnitt 7 |
| T-22-19 | Gleichzeitig materialisierte Personen derselben Gruppe wechseln gemeinsam die Stufe | C, U | N-EN-489; Abschnitt 5 |

### T-23: Koop, Host-Speicherstand, Mods

Gruppe zu [Modul 23](23-Koop-Host-Speicherstand-und-Mods.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-23-01 | Ein NPC existiert pro Welt genau einmal und ist für alle Spieler derselbe | C | N-EN-501; Abschnitt 2 |
| T-23-02 | Beziehungskanten zu verschiedenen Spielern sind unabhängig | C | N-EN-502; N-KO-005 |
| T-23-03 | Gastfortschritt (Beziehungen, Erinnerungen, Ruf) ist im Host-Spielstand gespeichert und wird nicht in den Gast-Spielstand übernommen | C | N-EN-503; N-KO-001, N-KO-002 |
| T-23-04 | Ein Welt-Zeitsprung im Koop findet nur mit Zustimmung aller aktiven Spieler statt | C | N-EN-504; N-KO-008 |
| T-23-05 | Individuelle Verhaftung eines Spielers hält die Welt nicht an | C | N-EN-505; N-KO-008 |
| T-23-06 | Ein Mod-Datensatz wird bei Deaktivierung nicht gelöscht und bei Reaktivierung unverändert wiederverwendet | C, I | N-EN-506; N-KO-013 |
| T-23-07 | Ein Spielstand mit unbekannten Datenarten lädt und speichert sie unverändert | C | N-EN-507; N-KO-017 |
| T-23-08 | Mod-IDs kollidieren nicht mit Kern-IDs | C, I | N-EN-508; Abschnitt 6 |
| T-23-09 | Kein Mod bricht Hauptstory-Pfade oder Schutzstufen | C, I | N-EN-509; Abschnitt 6 |
| T-23-10 | Mod-Inhalte unterliegen denselben Simulationsbudgets | C, I | N-EN-510; Abschnitt 6 |
| T-23-11 | Das Offizielle Online lädt keine Mod-Daten | C, I | N-EN-511; N-KO-004 |
| T-23-12 | Die Simulation läuft nur auf der Autorität | C | N-EN-512; N-KO-018 |
| T-23-13 | Ein Gast ohne Begegnung mit einem NPC hat bei diesem keine Erinnerung | C | N-EN-513; N-GE-008 |
| T-23-14 | Eine Teamentscheidung wirkt auf NPCs nur über wahrgenommene Handlungen | C | N-EN-514; Abschnitt 4 |
| T-23-15 | Die Altersregeln des Adult-Systems sind durch Mods nicht abschaltbar | C, I | N-EN-515; Abschnitt 7 |

### T-24: Integration und Konsistenz

Gruppe zu [Modul 24](24-Integration-und-Konsistenzregeln.md).

| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |
|---|---|:---:|---|
| T-24-01 | Jede Tatsache hat genau einen Besitzer-Speicher | C | N-EN-521; N-KS-002 |
| T-24-02 | Leser kopieren keine Tatsachen; sie verweisen per ID | C | N-EN-522; Abschnitt 2 |
| T-24-03 | Systemübergreifende Wirkungen entstehen nur als Ereignisse, nicht als Direktschreibzugriff | C | N-EN-523; N-KS-003 |
| T-24-04 | Die Verarbeitung der Ereignisse ist idempotent und in definierter Reihenfolge | C | N-EN-524; Abschnitt 3.2 |
| T-24-05 | Ein Folgeereignis hat nie einen früheren Zeitstempel als seine Ursache | C | N-EN-525; Abschnitt 3.2 |
| T-24-06 | Zusammengehörige Änderungen (Tod: Lebenszustand, Haushalt, Fall) gelten alle oder keine | C | N-EN-526; Abschnitt 3.2 |
| T-24-07 | Der Invariantenkatalog (Abschnitt 4) läuft als automatisierter Test | C | N-EN-527; N-KS-004 |
| T-24-08 | Ein Autorenschutz verhindert systemische Ereignisse, die ihm widersprechen, und vermerkt sie als blockiert | C | N-EN-528; Abschnitt 7 |
| T-24-09 | Kein sichtbarer Widerspruch zwischen Systemen nach dem Laden einer Region | C, U | N-EN-529; N-KS-005 |
| T-24-10 | Die Umsetzungsreihenfolge beginnt erst, wenn die Mindest-Eingaben existieren | C | N-EN-530; N-KS-012 |
| T-24-11 | Ein Fall zitiert nur Ereignisse, die im Journal stehen | C | N-EN-531; N-KS-004 |
| T-24-12 | Eine Verletzung ist in jedem System sichtbar, das die Person darstellt | C, U | N-EN-532; N-KS-004 |
| T-24-13 | Mitgliedschaft in Fraktions- und NPC-Datensatz stimmt überein | C | N-EN-533; Abschnitt 2 |
| T-24-14 | Ein Ereignis ohne Besitzer wird abgelehnt | C | N-EN-534; Abschnitt 9 |
| T-24-15 | Dieselbe Ereignisfolge ergibt auf jeder Plattform dasselbe Ergebnis | C | N-EN-535; N-SI-012 |

**Insgesamt 320 Regeltests** aus 22 Modulen.

<!-- END GENERATED RULE TESTS -->

---

## 3. Eigenschaftstests (Property Tests)

Eigenschaftstests prüfen Gesetzmäßigkeiten über viele zufällig erzeugte, aber seed-reproduzierbare Eingaben. Sie sind die wichtigste Absicherung gegen die Fehlerklassen des Proposals 0010 §9 (Inkohärenz, Nichtdeterminismus, Speicherwachstum).

| Nr. | Eigenschaft | Ebene | Herkunft |
|---|---|:---:|---|
| T-PR01 | **Determinismus:** Gleicher Weltseed, gleiche Eingaben und gleiche Ereignisfolge liefern bitgleiche Weltzustandsentscheidungen, auch bei wiederholtem Lauf und auf verschiedenen Plattformen | C | N-SI-012, N-EN-441, N-EN-479, N-EN-535 |
| T-PR02 | **Round-Trip:** Speichern, Laden, erneutes Speichern ergibt dieselben Segmente; Laden nach Speichern liefert gleichen Zustand | C | N-PE-022, N-EN-443 |
| T-PR03 | **Reihenfolgeunabhängigkeit:** Fortschreiben einer Menge von Entitäten in beliebiger Reihenfolge liefert dasselbe Ergebnis, sofern die Zeitordnung gilt | C | N-EN-471, N-EN-472 |
| T-PR04 | **Zeitmonotonie:** Weltzeit, Cursor und Zeitstempel nehmen nie ab; Folgeereignisse sind nicht älter als ihre Ursache | C | N-EN-478, N-EN-525 |
| T-PR05 | **Referenzielle Integrität:** Jeder Verweis in jedem Segment zeigt auf einen existierenden Datensatz oder Tombstone, nach jeder zufälligen Ereignisfolge | C | N-EN-447, N-EN-534 |
| T-PR06 | **Wissensgrenze:** Für jede erzeugte Aussage eines NPCs existiert ein Erinnerungseintrag mit Quelle; kein Eintrag ohne Wahrnehmungsweg | C | N-EN-041, N-EN-053, N-EN-064 |
| T-PR07 | **Gedächtnisbegrenzung:** Bei Millionen simulierter Ereignisse bleibt die Zahl der Einträge je NPC unter der Obergrenze; Anker gehen nicht verloren | C | N-EN-043, N-EN-045, N-EN-456 |
| T-PR08 | **Verlustfreie Stufenwechsel:** Zufällige Folgen von Auf- und Abstufungen verlieren keine wichtigen Ergebnisse (Verletzung, Zusage, Anker) | C | N-EN-473, N-EN-005 |
| T-PR09 | **Idempotenz:** Wiederholtes Anwenden desselben Ereignisses ändert den Zustand nicht erneut | C | N-EN-524 |
| T-PR10 | **Migration:** Alte Spielstände (Golden Files) werden deterministisch migriert und anschließend unverändert geladen; unbekannte Daten bleiben erhalten | C | N-EN-444, N-EN-445, N-EN-507 |
| T-PR11 | **Konsistenz der Ortsbelegung:** Kein NPC ist zu irgendeiner Weltzeit an zwei Orten | C | N-EN-476, N-EN-241 |
| T-PR12 | **Speicherwachstum:** Die gespeicherte Datenmenge wächst nicht mit der Zahl unveränderter Identitäten | C | N-EN-455, N-EN-484 |
| T-PR13 | **Beziehungsgrenzen:** Keine Folge von Ereignissen führt zu einem Beziehungswert außerhalb des zulässigen Bereichs oder zu Sprüngen größer als die Schrittweite | C | N-EN-025, N-EN-119 |
| T-PR14 | **Invariantenkatalog:** Der Katalog aus Modul 24 §4 gilt nach jeder zufälligen Ereignisfolge | C | N-EN-527 |
| T-PR15 | **Rückbau durch Journal:** Der abgeleitete Zustand entspricht dem Zustand, der aus dem Journal neu berechnet wird (soweit das Journal reicht) | C | N-EN-449 |

---

## 4. Szenariotests

Jedes Szenario aus [Modul 25](25-Querschnittsszenarien.md) wird ein Test mit fester Anfangslage (Seed, Datensätze), festem Eingriff und einer Prüfliste. Die Ausgänge sind Zustandsvergleiche, keine Zahlenvorgaben.

| Nr. | Szenario | Prüfung (kurz) | Ebene | Regeln |
|---|---|---|:---:|---|
| T-SZ01 | **S1 Zeuge überlebt:** Beobachtung, Verzerrung, Gerücht, Meidung | Eintrag mit niedriger Genauigkeit; Polizei nennt gewöhnliche Erklärung; Gerücht verzerrt deterministisch; Meidung entsteht und klingt ohne neue Ereignisse ab | C | N-EN-041, 046, 061, 069, 089, 289, 291 |
| T-SZ02 | **S2 Gefallen nach Jahren:** Rettung, Wiederbegegnung, konkreter Hook | Anker bleibt; Hook nennt nur Existierendes; keine Zahl sichtbar | C, U | N-EN-043, 052, 114, 421, 424 |
| T-SZ03 | **S3 Menschlicher Gegner kehrt zurück:** Verletzung, Heilung, Modell, Vorzeichen | Verletzung sichtbar bei Rückkehr; Rückkehr nur bei Motiv, Mitteln, Wissen, Gelegenheit; Vorzeichen vorhanden | C, U | N-EN-181, 183, 186, 194, 213, 218 |
| T-SZ04 | **S4 Intelligentes Wesen überlebt:** Fallenerkennung, Gedächtnis, Anpassung | Anpassung nur innerhalb der Artregeln; kein Wert steigt allein durch Begegnungen | C | N-EN-181, 182, 184, 187, 188 |
| T-SZ05 | **S5 Clubmitglied wird Crew:** Kette bis HQ-Integration | Hauptmission läuft ohne Rekrut; Crew reagiert unterschiedlich; Austritt löscht keine Tattoos | C, I | N-EN-306, 312, 319, 400, 402, 403, 406 |
| T-SZ06 | **S6 Haft und Zeitsprung:** Nachholen in groben Schritten | Eskalationsstufen nacheinander; keine Schwelle übersprungen; Weltzeit statt Realzeit | C | N-EN-338, 451, 478, 483 |
| T-SZ07 | **S7 Region nach langer Abwesenheit:** Materialisierung | Straße belebt; Zustand vereinbar mit Cursor; Bekannte erkennen zögernd; Ortswandel vorhanden | C, U | N-EN-246, 247, 262, 263, 461, 471 |
| T-SZ08 | **S8 Koop, zwei Spieler, ein Zeuge:** Subjekte | Kanten unabhängig; Gastfortschritt im Host-Spielstand; Simulation nur auf Host | C, U | N-EN-501, 502, 503, 512, 514 |
| T-SZ09 | **S9 Mod deaktiviert:** Zustand erhalten | Mod-Datensatz bleibt; Verweise gültig; Reaktivierung holt Zeit nach | C | N-EN-445, 447, 453, 506, 507 |
| T-SZ10 | **S10 Friedliches Wesen und anderer Hunter:** Verschonen, Fremdhandlung | Verschontes Wesen wird nicht zum Feind gezwungen; Hunter handelt nach eigener Doktrin; Folgen sichtbar | C | N-EN-372, 374, 379, 380 |
| T-SZ11 | **S11 Falsches Gerücht:** Verbreitung und Korrektur | Beziehungen Dritter ändern sich ohne Wahrheitsänderung; Korrektur braucht Quelle und Weg | C | N-EN-082, 088, 090, 118, 135 |
| T-SZ12 | **S12 Tod einer Crew-Figur:** Sandbox gegen Story | Kernfigur stirbt nicht durch Sandbox-Zufall; Tod ist endgültig; persönliche Anfragen entfallen | C | N-EN-272, 395, 396, 397, 398 |

---

## 5. Dokument- und Inhaltsprüfungen

| Nr. | Prüfung | Ebene | Werkzeug |
|---|---|:---:|---|
| T-DK01 | Zitate im Format `NN#m[E\|A]` verweisen auf vorhandene Nachrichten, die Rolle stimmt | I | `Tools/world-audit/check_docs.py` |
| T-DK02 | Wörtliche Zitate in „…“ sind in den Quelltexten auffindbar | I | `check_docs.py` |
| T-DK03 | Alle verwendeten Quellenschlüssel (`M-…`, `PE-…`) sind in Modul 01 definiert | I | `check_docs.py` |
| T-DK04 | Registerkennungen `N-XX-nnn` sind im Register genau einmal definiert; jede Verwendung verweist auf einen definierten Eintrag | I | `check_docs.py`, `render_npc_register.py` |
| T-DK05 | Relative Links und Anker sind gültig | I | `Tools/world-audit/check_links.py` |
| T-DK06 | `git diff --check` ohne Befunde | I | Git |
| T-DK07 | Kein Dokument des Ordners enthält benannte Fraktionen, benannte Antagonisten oder eine Crew-Besetzung | I | Review, Textsuche |
| T-DK08 | Kein Dokument setzt eine Zahl als Entscheidung; Rechenbeispiele sind als Annahmen gekennzeichnet | I | Review |
| T-DK09 | Jede 📋-Zeile nennt eine Quelle, die diese Aussage trägt; Beispiele und Tabellen des Assistenten sind 💡 | I | Review |
| T-DK10 | Keine Datei außerhalb von `Docs/Game/NPCs/` (und der genannten Werkzeuge) wurde verändert | I | `git status` |
| T-DK11 | Inhaltsregeln: keine Beziehungszahl in Beispieltexten der Spieler-Oberfläche; keine Level für Kreaturen; keine Wertung von Gruppen durch das Spiel | I | Review |
| T-DK12 | Kinder-NPCs erscheinen in keinem Adult-Bezug | I | Review, Textsuche |

---

## 6. Messungen statt Sollwerte

Die Quellen nennen keine Budgets (`M-OQ16`). Der Katalog verlangt daher **Messungen**, deren Ergebnis dem Entwickler berichtet wird und erst danach zu Sollwerten werden kann.

| Nr. | Messgröße | Szenario | Bericht |
|---|---|---|---|
| T-ME01 | Fortschreibungskosten je Entität und Weltzeitraum (S2/S3) | wachsende Population (10², 10³, 10⁴, 10⁵) | Zeit, Speicher |
| T-ME02 | Materialisierungskosten je Kopf und je Region | Schnellreise in eine dichte Region | Zeit, Spitzen |
| T-ME03 | Speicherstandgröße nach den Szenarien aus Modul 21 §5 mit gemessenen Datensatzgrößen | wenig, typisch, extrem | Bytes roh und komprimiert |
| T-ME04 | Ladezeit der Segmente und Regionen | kalter Start, warmer Start | Zeit |
| T-ME05 | KI-Zeit pro Frame bei S0/S1-Mengen | Menge im Stadtzentrum | ms, Verteilung |
| T-ME06 | Größe von Datensatztiefen (Known, Detailed, Full) | Stichprobe | Bytes |
| T-ME07 | Journalwachstum je Spielstunde | Beispielsitzungen | Ereignisse, Bytes |
| T-ME08 | Replikationsbandbreite im Koop | 2 bis 4 Spieler | Bytes/s |
| T-ME09 | Verhalten auf der schwächeren Zielkonfiguration | Laptop mit integrierter Grafik (Fedora) | Zeit, Speicher |

---

## 7. Abnahmekriterien nach Ausbaustufe

Die Stufen folgen dem Proposal 0010 §10 (unverbindlich). **Abnahme heißt:** Die genannten Tests laufen und die Ergebnisse sind dem Entwickler berichtet; nicht: das Ergebnis ist ein gewünschtes Spielgefühl.

| Stufe | Inhalt | Tests, die laufen müssen |
|---|---|---|
| **L1** Kerndatenmodell | IDs, Ereignisse, begrenztes Gedächtnis, Seeds, Round-Trip, Migration (portabel) | T-03-*, T-05-*, T-21-*, T-PR01, T-PR02, T-PR04, T-PR05, T-PR07, T-PR09, T-PR10, T-PR12, T-DK01 bis T-DK06 |
| **L2** Ein benannter NPC im Vertical Slice | Zeuge mit Gedächtnis, Folgen in Dialog und Fall | T-05-*, T-06-*, T-08-*, T-20-*, T-SZ01, T-SZ02, T-PR06 |
| **L3** Ein wiederkehrender Gegner | Nemesis-Prototyp für eine Art | T-11-*, T-12-*, T-SZ03, T-SZ04 |
| **L4** Gestufte Simulation, kleine Testregion | S0 bis S3, Fortschreiben, Materialisierung, Messungen | T-13-*, T-22-*, T-PR03, T-PR08, T-PR11, T-PR14, T-PR15, T-SZ06, T-SZ07, T-ME01 bis T-ME09 |
| **L5** Fraktionen, Territorien, Skalierung, Koop | Fraktionsmodell, Konflikte, Subjektmodell, Mods | T-16-*, T-17-*, T-18-*, T-19-*, T-23-*, T-SZ05, T-SZ08, T-SZ09, T-SZ10, T-SZ11, T-SZ12 |

---

## 8. Abdeckung der beschlossenen Anforderungen

Die Tabelle zeigt, welche Produktanforderungen A–H (`PE-N1022`) durch welche Tests abgesichert sind.

| Anforderung | Kernaussage | Hauptsächliche Tests |
|---|---|---|
| A Persistente Identitäten | stabile ID, Seed, Einfrieren, Archiv der Toten | T-03-*, T-PR01, T-PR05 |
| B Langzeitgedächtnis | Erinnerung, Verblassen, Anker, Wiedererkennung | T-05-*, T-06-*, T-PR06, T-PR07, T-SZ01, T-SZ02 |
| C Nemesis und Rivalität | Rückkehr, Lernen, Anpassung, Narben | T-11-*, T-12-*, T-SZ03, T-SZ04 |
| D Autonome Fraktionen | Ziele, Territorien, Konflikte, Strategie | T-16-*, T-17-*, T-18-* |
| E Persistente Konsequenzen | Verletzungen, Tode, Reparaturen | T-14-*, T-15-*, T-PR13, T-SZ12 |
| F Unabhängige Lebensentwicklung | Zeitpläne, Lebensereignisse, Weggehen | T-13-*, T-14-*, T-SZ06 |
| G Skalierbare Simulation | Stufen, Fortschreiben, Materialisierung, Budgets | T-22-*, T-PR03, T-PR12, T-ME01 bis T-ME09 |
| H Integration | Besitzer, Ereignisse, Invarianten | T-24-*, T-21-*, T-PR14 |
