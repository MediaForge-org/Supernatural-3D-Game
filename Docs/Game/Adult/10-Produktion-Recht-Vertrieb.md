# Adult 10: Produktion, Recht und Vertrieb

> [!IMPORTANT]
> **Nichts davon ist implementiert, und nichts davon ist Rechtsberatung.** Die Rechts- und Plattformangaben in diesem Dokument stammen aus einem Gespräch mit einem KI-Assistenten (25.09.2026) und sind **ungeprüft und möglicherweise veraltet oder falsch**. Sie sind als Prüfliste für eine spätere fachkundige Klärung gedacht. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)).

**Inhalt**

1. [Produktionsplan](#1-produktionsplan)
2. [Veröffentlichung und Vertrieb](#2-veröffentlichung-und-vertrieb)
3. [Recht und Plattformen](#3-recht-und-plattformen)
4. [Umgang mit KI-Werkzeugen](#4-umgang-mit-ki-werkzeugen)
5. [Einordnung in den Projektplan](#5-einordnung-in-den-projektplan)

---

## 1. Produktionsplan

📋 (Runde 20, 27.09.2026) Die Planung des Erwachsenen-Bereichs gilt als „im Wesentlichen abgeschlossen“, vorbehaltlich echter Erkenntnisse aus Prototyping, Rechtslage und den ausstehenden Audits ([Modul 03, Abschnitt 7](03-Interaktion-Taxonomie-Ablauf.md#7-stand-der-registry-und-der-audits)).

- **Version 1 startet klein:** Grundinteraktionen, wenige Positionen und Übergänge, grundlegende Reaktionen, einfache Präferenzen, eine kleine Testumgebung. Erst danach wächst der Umfang schrittweise. Der Entwickler (25.09.): zuerst nur grundlegende Handlungsarten, „nicht das Wichtigste“; wichtiger seien zuerst Ladebildschirm und ein kleiner Teil der Karte, Adult komme danach und werde „sehr früh implementiert und dann sehr viel erweitert“.
- **Prioritätsklassen:** Kern → Wichtig → Späte Politur.
- **Kerntor:** Version 1 gilt erst als funktionierend, wenn IK, Physik, Körperzustand, Reaktionsgraph, Speichern/Laden und NPC-Absicht stabil zusammenspielen. Lieber weniger Funktionen, aber sauber, statt viele halbfertige Aktionen.
- **Qualitätstore:** *Leistungstor* (Zeit-/RAM-/VRAM-Budgets vor dem Zusammenführen), *QA-Tor* (automatisierte Solver-, Kollisions-, Speicher- und Replay-Tests), *visuelles Qualitätstor* (Animation/IK, Kontakt, Weichgewebe, Audio und Reaktionen müssen zusammen glaubwürdig wirken).
- **Registry-Einfrieren** erst nach Taxonomie-Audit und externer Gegenprüfung.
- **Dokumentation:** Vor jedem großen Teilsystem eine kurze technische Spezifikation.
- **Erst kleiner vertikaler Schnitt, dann Skalierung.** Die allgemeinen Systeme (Mensch, Physik, Gesundheit, Gedächtnis, Welt, Replay, Statistik, Modding) werden zuerst und für das ganze Spiel gebaut. Der Entwickler sagte am 16.09., die Erwachsenen-Mod solle früh verfügbar sein, noch vor manchen anderen Systemen; die Einordnung relativ zum Rest ist **❓ offen** und in der [Roadmap der README](../../../README.md#27-entwicklungsphasen-roadmap-und-meilensteine) nicht enthalten. Das Repository ist derzeit in Phase 0 (siehe [CLAUDE.md](../../../CLAUDE.md), Abschnitt 4); die Erwachsenen-Mod gehört nicht zu Phase 0 oder dem Vertical Slice.
- Eine schlanke Feinplanung ist gewollt: Der Entwickler hält sehr kleinteilige Detailplanung des Erwachsenen-Bereichs inzwischen für übertrieben („einfach wie in echt“, 23.09.) und bat, Fragen auf die wichtigsten zu beschränken.

---

## 2. Veröffentlichung und Vertrieb

- 📋 **Ziel ist eine öffentliche Veröffentlichung** des optionalen Pakets (Entwickler, 27.09.: „ich will schon, dass es veröffentlicht wird, nur für mich brauche ich es nicht“). Privat-only ist **Rückfallplan**, falls Recht oder Plattformregeln eine Veröffentlichung unpraktisch machen. Das Basisspiel darf davon in keinem Fall abhängen.
- 📋 Getrennte Builds und getrenntes Paket ([Modul 02, Abschnitt 4](02-Architektur-Pakete-Technik.md#4-builds)).
- ❓ **Vertriebsweg, Plattform- und Altersfreigaberegeln sind nicht entschieden.** Eine Prüfung erfolgt erst zum Veröffentlichungszeitpunkt, weil Regeln sich ändern. Planungsannahme: Gängige Spiele-Plattformen würden ein solches Paket im Hauptspiel nicht akzeptieren; es ist deshalb als getrenntes Paket zu behandeln.
- **Projektregel (CLAUDE.md, Abschnitt 9 und 10):** Kein Lizenztext, kein öffentliches Repository und keine Veröffentlichung des Pakets oder der Planung ohne ausdrückliche Entscheidung des Entwicklers. Die Veröffentlichung der Planung ist eine **getrennte** Frage: Sie setzt eine Prüfung jeder Datei auf Urheberrecht, Lizenzen, personenbezogene Daten und Zugangsdaten voraus.

---

## 3. Recht und Plattformen

Die folgenden Punkte wurden am 25.09.2026 vom Assistenten genannt, teils unter Verweis auf Webquellen. Sie sind **Prüfpunkte**, keine Feststellungen. Der Assistent empfahl, das Recht als eigenen Projektblock früh und nicht erst kurz vor Release zu behandeln; ein Beschluss des Entwicklers dazu ist nicht festgehalten.

| Thema | Aussage im Gespräch (ungeprüft) | Vorgeschlagene Folge (🧭) |
|---|---|---|
| Plattform-Deklaration | Eine große PC-Plattform verlange die Angabe **sämtlichen** Erwachsenen-Contents in einem hochgeladenen Build, auch wenn Spieler ihn nicht erreichen können. | Kein versteckter Erwachsenen-Content im Basis-Build; Paket als eigenes Depot/Paket. |
| Altersgrenze | Plattformen erlauben Erwachsenen-Content grundsätzlich mit Kennzeichnung und Altersschranke; ausgeschlossen seien u. a. explizite Bilder realer Personen, Kinder-Ausbeutung, rechtswidrige Inhalte und Inhalte, die Zahlungsdienstleister-Regeln verletzen. | Drei Prüfstufen: **Gesetz ∩ Plattformregel ∩ Zahlungsdienstleister**; alle müssen passen. |
| Österreich | Das Pornographiegesetz gelte weiter; pornografische Darstellungen sexueller Gewalt würden in der Rechtsprechung als „harte Pornographie“ eingeordnet. | Bevor so etwas **je außerhalb eines privaten Testsystems** verteilt wird, einen österreichischen Medien-/Strafrechtler befragen. „Nur Entwickler“ ist kein rechtlicher Schalter. (Bezug: [Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md)) |
| Darstellungen, die Minderjährige nahelegen | Das österreichische Strafrecht (§ 207a) erfasse auch wirklichkeitsnahe oder künstlich erzeugte Darstellungen, die den Eindruck Minderjähriger vermitteln. | Alter als harter Datenzustand; **Build-Validator**; Figuren im Erwachsenen-Pfad müssen eindeutig erwachsen wirken ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#2-altersgrenze-und-prüfung)). |
| Deutschland | Zusätzlicher Jugendmedienschutz; pornografische Online-Inhalte dürften nur in geschlossenen Benutzergruppen zugänglich sein; Altersfreigabe-Systeme verlangten Darstellungen unterhalb der Pornografiegrenze. | Basisspiel und Paket sauber trennen; später eine Vertriebsmatrix je Land. |
| Generative KI zur Laufzeit | Live erzeugter Erwachsenen-Content werde derzeit von der Plattform nicht veröffentlicht; vorab erzeugte KI-Inhalte müssten deklariert werden. | Keine generative KI in der Laufzeit ([Modul 08](08-Welt-Orte-Branche-Wirtschaft.md#6-spielwelt-internet-und-medien)); Herkunft aller Assets dokumentieren. |
| Symbolik | Verbotene oder rechtlich problematische Symbole (zum Beispiel Kennzeichen verfassungswidriger Organisationen) sind in Deutschland und Österreich strafbar, mit Ausnahmen. | Vor jeder Entscheidung über extremistische Symbolik in Tattoos ([Modul 07](07-Charakteranpassung-Tattoos-Koerpermodifikation.md#3-tattoos)) rechtlich klären. (Eigene Ergänzung, nicht aus dem Gespräch.) |
| Marken und Vorlagen | Der Titel „Supernatural“ und konkrete Figuren/Design der Vorlage seien geschützt; der kommerzielle Titel solle anders heißen; Fahrzeuge nicht mit Originalnamen. | Eigenes Thema des Gesamtprojekts. |

Die Rechtslage kann sich bis zu einer Veröffentlichung ändern; der Assistent sagte selbst, heutige Regeln seien nur ein Orientierungspunkt, und Herkunft und Rechte der Assets müssten kurz vor Release neu geprüft werden.

---

## 4. Umgang mit KI-Werkzeugen

- 📋 KI wird in der **Produktion** eingesetzt; die Laufzeit des Spiels soll ohne generative KI auskommen (Entwickler bestätigt).
- 🧭 Ein **nicht zensierter lokaler Sprachmodell-Lauf** („Qwen“) war im Gespräch als **Lückenfinder** für die Taxonomie und als Hilfe bei der Planung der abstrakten Erwachsenen-Kategorien vorgesehen. Er ist ein Planungswerkzeug, kein Spielbestandteil. Seine Ergebnisse haben **keine Kanon-Wirkung**, bis der Entwickler sie bestätigt. Er war zum Stand 27.09. noch nicht verfügbar; der Audit steht aus.
- ❓ Der Entwickler äußerte (25.09.), die KI-Nutzung eher nicht hervorzuheben („eher geheimhalten“). Plattformangaben zu KI-Inhalten müssen **wahrheitsgemäß** sein; das ist ein offener Punkt zwischen Wunsch und Pflicht ([Offene Fragen](11-Offene-Fragen-und-Konflikte.md)).

---

## 5. Einordnung in den Projektplan

- Die Erwachsenen-Planung beeinflusst bereits **allgemeine** Systeme (Human Core, Körperphysik, Gesundheit, Gedächtnis, Replay, Statistik, Charakter-Editor, Modding, Speicherformat). Diese Systeme sind eigenständig zu planen und zu bauen; **sie dürfen nicht um das Erwachsenen-Paket herum entworfen werden**.
- Vor jeder Implementierung gilt die Master-Konsultation (CLAUDE.md, Abschnitt 11), und alle Annahmen zu Recht und Plattform bleiben bis zur fachkundigen Prüfung offen.
- Der aktuelle Stand des Repositorys (Interaktion, Testtür, Weltzustand, Speicher-Slots) berührt den Erwachsenen-Bereich noch nicht; es gibt hier keinen Code und keine Tests.
