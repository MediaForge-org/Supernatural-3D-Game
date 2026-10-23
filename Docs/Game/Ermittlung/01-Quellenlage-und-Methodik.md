# Ermittlung 01: Quellenlage und Methodik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sagt, **woher** die Ermittlungs-Dokumentation stammt, was die Quellen können und nach welchen Regeln Aussagen einen Status bekommen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Das Register steht in [Modul 20](20-Entscheidungsregister.md), Widersprüche und offene Fragen in [Modul 21](21-Konflikte-und-offene-Fragen.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Zitierschlüssel](#2-zitierschlüssel)
3. [Statusregeln](#3-statusregeln)
4. [Was bereits dokumentiert war](#4-was-bereits-dokumentiert-war)
5. [Grenzen dieser Auswertung](#5-grenzen-dieser-auswertung)

---

## 1. Befund in Kürze

1. **Die Ermittlungs-Planung ist breit und konsistent, aber ohne Zahlen.** Fallmodell, Ermittlungsmodi, Archiv, Journal, Spuren, Forensik und Rechtssystem sind als **Richtung** festgelegt (Entwicklerantworten im Chat 23 vom 18.08. bis 04.09.2026 und der Entscheidungsstand `M-PD` vom 31.08.2026). Es gibt **keine** Straftatenliste, keine Schwellenwerte, keine Zeiten, keine Beweisgewichte.
2. **Die Entwickleraussagen sind meist kurz.** Typisch sind nummerierte Antworten („1. ja genau“) auf nummerierte Vorschlagsfragen des Assistenten. Die Inhalte stammen dann aus der **Frage** (Assistent), die **Zustimmung** aus der Antwort (Entwickler). Dieses Modul zitiert beides getrennt (`23#1056A` Frage, `23#1057E` Antwort).
3. **Der Master trägt diesen Bereich in zwei Schichten.** (a) Auf der Ebene des **aktuellen kanonischen Stands** (Tier 1) steht nur eine knappe Zusammenfassung: `M-CA0905` Abschnitt 4 (Tracking, Forensik, Spurenarten, als ‚accepted direction‘ geführt), `M-PS0905` und `M-B09` (Block 9: ‚evidence/research/forensics‘ als akzeptierter Pfeiler; **offen**: ‚exact player-death flow‘). (b) Die **Ausarbeitung** liegt im Entscheidungsstand `M-PD` (31.08.2026) und im Chat 23. Die Oktober-Deltas (14.–19.10.) behandeln Welt, Untergrund, Bahn und Straßenfahrzeuge, nicht Ermittlung und Recht. Die Suche nach `journal` im Oktober-Delta trifft nur das technische Weltzustands-Journal (WorldFacts), nicht das Falljournal.
3a. **Wichtiger Befund (Ergänzung):** Die Quiz-Entscheidungen aus `48#` (Oktober) stehen **nirgends im Master** (Suche nach `prison`, `jail`, `justice`, `wanted`, `shapeshifter` in Tier 1 und 2: keine Treffer; `M-MS1013` nennt nur Welt, Produktion und Tesla/Bunker). Sie sind **Entwicklerentscheidungen im Chat** und nach `CLAUDE.md` §2 maßgeblich, weil spätere ausdrückliche Entscheidungen ältere überschreiben. Der Master bildet sie noch nicht ab; das ist eine **Lücke des Masters**, nicht ein Widerspruch.
4. **Drei Ebenen** werden in den Quellen sauber getrennt und hier ebenfalls: **Hunter Archive** (was weiß ich), **Falljournal** (woran arbeite ich), **Hunter Record** (was habe ich erreicht), siehe [Modul 04](04-Archiv-Journal-und-Hunter-Record.md).
5. **Sensibler Inhalt** (schwere Gewalt- und Sexualstraftaten als Teil der Kriminalitätssimulation) wird hier nur auf der Systemebene geführt, die der Master selbst setzt. Die Detailregeln stehen in [Adult 09](../Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md) und werden nicht dupliziert.

---

## 2. Zitierschlüssel

Alle Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation bleibt ohne Zugriff darauf lesbar; die Schlüssel dienen der Nachprüfbarkeit.

### 2.1 Chats

Alle Chats liegen unter `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/`. Schlüssel `<Chat>#<Nachricht><Rolle>`; `E` = Rolle „user“ (Entwickler), `A` = Rolle „assistant“. Dieselbe Schreibweise wie in der [Welt-Dokumentation](../World/01-Quellenlage-und-Methodik.md#2-zitierschlüssel).

| Schlüssel | Chat | Zeitraum | Inhalt für die Ermittlung |
|---|---|---|---|
| `23#` | SPN Full Master ZIP (1.109 Nachrichten) | 18.08.–05.09.2026 | Fallmodell (46–65), Detektivmodus, Journal, Archiv-Anzeige, Block 8 (Kriminalität, Polizei), Detektivpfad (1061–1062), Block 9 und 10 (Spuren, Tracking, Forensik) |
| `35#` | Projektwissen nutzen | 06.09.–04.10.2026 | Crime-/Justice-Grundregeln (532–534) |
| `48#` | Planung ZIP Downloads (297 Nachrichten) | 05.10.–13.10.2026 | **Entscheidungs-Quiz** vom 09.–11.10.2026: Ermittlungsfreiheit (59–64), Companion und Lösungsbuch (65–95), Koop (97–111), Verletzung, Tod, Rettung (113–123), Monsterwissen und -forschung (125–139), Gesellschaft und Geheimhaltung (145–151), Ruf, Fahndung, Justiz, Haft (153–170), staatliche Forschung (171–179) |
| `25#` | Anzahl der Sammelgegenstände | 18.08.2026 | Sammelobjekte (13E) |

### 2.2 Entscheidungs- und Planungsdokumente

| Schlüssel | Datei (relativ zum kumulativen Master) |
|---|---|
| `M-PD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` (Stand 2026-08-31, Abschnitte ‚Story and cases‘, ‚Investigation modes‘, ‚Hunter Archive, collectibles and progress‘, ‚Cash / law / evidence‘, ‚Organic world-event / case emergence‘, ‚Hunter Archive epistemology‘) |
| `M-CA0905` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (Abschnitt 4 ‚Block 10 — social/life sandbox accepted direction‘, Abschnitt 6 „Collectibles/discoveries“, Abschnitt 7 ‚Still open / not locked‘) |
| `M-PS0905` | `01_CURRENT_CANONICAL_STATE/PLANNING_STATUS_2026-09-05.md` |
| `M-B09` | `01_CURRENT_CANONICAL_STATE/BLOCK_09_STATUS.md` (akzeptierte Pfeiler und offene Punkte von Block 9) |
| `M-B25` | `…/docs/planning-backlog/25-organic-world-events-cases.md` (Planungsinventar) |
| `M-OD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/OPEN_DECISIONS.md` (Stand 2026-08-31; Abschnitt ‚Crime / law / evidence‘ und Verwandtes: Liste **offener** Punkte) |
| `M-P03` | `…/docs/prompts/03-cases-investigation/` (53 Entwurfs-Arbeitspakete, Status `DRAFT_REWRITE_REQUIRED`; **nur die Titel und `TRACK_CONTEXT.md`** tragen Inhalt, der Rest ist Schablonentext; siehe [Modul 22](22-Abdeckungsprotokoll.md)) |
| `M-B37` | `…/docs/planning-backlog/37-wildlife-study-harvest-forensics.md` (Planungsinventar) |
| `M-B39` | `…/docs/planning-backlog/39-law-evidence-jurisdiction-smartphone.md` (Planungsinventar) |
| `M-B40` | `…/docs/planning-backlog/40-social-media-dynamic-world-events.md` (Planungsinventar) |

Die Backlog-Dateien tragen selbst den Status ‚PLANNING INVENTORY‘: Sie nennen **zu planende Themen**, keine Entscheidungen.

### 2.3 Dokumente dieses Repositorys

| Dokument | Rolle |
|---|---|
| [Spielvision](../Spielvision.md) §7, §8 | Überblick Fälle und Archiv; wird hier vertieft, nicht ersetzt |
| [Gameplay-Systeme](../Gameplay-Systeme.md) §10, §11 | Überblick Recht und Smartphone |
| [Entscheidungsstatus](../Entscheidungsstatus.md) | Bündelung aller Statusklassen |
| [Adult 09](../Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md) | sensible Themen auf Systemebene |

---

### 2.4 Das Quiz-Format in Chat 48

Ab `48#55E` stellte der Assistent Planungsfragen als **Multiple-Choice-Quiz** mit den Optionen A bis D und „E: eigene Antwort“. Die Antwort des Entwicklers (`48#…E`) beginnt mit einem Titel („Supernatural Gefängnissystem: …“) und enthält entweder den **Text der gewählten Option** oder einen eigenen Text. Regeln für die Auswertung:

1. Die **Frage und die Optionen** stehen in der Assistentennachricht davor (`48#<n-1>A`); sie sind dort nachlesbar und werden hier zitiert, damit auch die **nicht gewählten** Optionen bekannt sind.
2. Die **gewählte Option** und der Freitext des Entwicklers sind 📋. Wählt der Entwickler eine Option und schränkt sie ein („D klingt gut aber ist wenn es nicht in der story gescripted ist schwer umzusetzen“, `48#157E`), gilt die Einschränkung.
3. Die **Bestätigungstexte** des Assistenten danach („Bestätigt: …“) sind **Ausformulierungen** (💡), außer sie wiederholen die Aussage des Entwicklers. Tabellen, Beispiele und Phasenzuordnungen `[VS]`, `[EARLY]`, `[LATER]`, `[FUTURE]` darin sind Vorschläge.
4. Die Konzeptvorschauen in den Nachrichten („Interaktive Konzeptvorschau“, Schieberegler, Beispieleinstellungen) sind ausdrücklich **keine Festlegung** (`48#58A`: „Das ist nur ein Beispiel, keine neue Festlegung.“).

## 3. Statusregeln

Dieselben Zeichen wie in der Welt-Dokumentation. Zusätzliche Regeln für diesen Bereich:

1. **📋** gilt, wenn der Entwickler die Aussage selbst formuliert **oder** auf einen Vorschlag mit klarer Zustimmung geantwortet hat **und** die Aussage in `M-PD` steht oder in der Antwort inhaltlich wiederholt wird. Reines „ja“ auf eine Sammelantwort ohne Wiederholung gilt als **Zustimmung zur Richtung**, nicht zu jedem Detail der Frage; dann steht im Register ausdrücklich „Richtung“.
2. **💡** gilt für Beispiele, Tabellen, Bezeichnungen und Beispieltexte des Assistenten (zum Beispiel Bildschirmentwürfe wie „BLACKWATER FARM“). Diese Beispiele sind **Veranschaulichung**, kein Schema.
3. **Fallbeispiele** (Blackwater Farm, Sheriff Miller, Revenant) sind Erläuterungsbeispiele des Assistenten. Sie sind **keine** Fälle, Figuren oder Orte der Spielwelt und dürfen nicht als Lore gelesen werden.
4. Wenn der Entwickler eine Annahme **ändert**, gilt die Änderung (Beispiel: Aus dem Vorschlag eines vierstufigen Hilfesystems wurde ein Detektivmodus „der keine hilfen hat, gar keine“, `23#52E`).
5. Zahlen aus dem Master werden mit Datum übernommen. In diesem Bereich gibt es wenige: Hauptgeschichte 80–100 Stunden, 1.200–1.500 Sammelobjekte, Speicherziel 20–50 MB (alles aus Chat 23 beziehungsweise `M-PD`).

---

## 4. Was bereits dokumentiert war

Vor diesem Ordner gab es zur Ermittlung nur Überblicke:

| Thema | Wo | Umfang |
|---|---|---|
| Fälle, Modi, Beweise, organische Entstehung | [Spielvision §7](../Spielvision.md#7-fälle-und-ermittlung) | etwa 40 Zeilen |
| Hunter-Archiv | [Spielvision §8](../Spielvision.md#8-das-hunter-archiv) | etwa 12 Zeilen |
| Recht, Polizei, Beweise | [Gameplay-Systeme §10](../Gameplay-Systeme.md#10-recht-polizei-und-beweise) | etwa 12 Zeilen |
| Smartphone und Weltereignisse | [Gameplay-Systeme §11](../Gameplay-Systeme.md#11-smartphone-soziale-medien-und-weltereignisse) | etwa 10 Zeilen |
| offene Punkte | [Entscheidungsstatus §5.7](../Entscheidungsstatus.md) | Stichworte |

Dieser Ordner **vertieft** diese Überblicke mit dem Gesprächsverlauf, der Rollenverteilung (wer hat was gesagt), allen Beispielen, Grenzen und offenen Fragen. Die Überblicke bleiben unverändert; Abweichungen stehen in [Modul 21](21-Konflikte-und-offene-Fragen.md).

---

## 5. Grenzen dieser Auswertung

- **Gelesen** wurden die Nachrichten 46–65 und 1056–1062, 1074–1077, 1088–1090 des Chats 23 sowie 532–534 des Chats 35 vollständig; weitere Fundstellen (zum Beispiel `23#1066` Ideensammlung, `23#1058` Block-8-Zusammenfassung) in den relevanten Abschnitten. In der Etappe 4 kamen hinzu: eine Durchsicht **aller Entwicklernachrichten 66–1055 des Chats 23** (Anfangszeilen jeder Nachricht) mit Volltext der als relevant erkannten Bereiche (unter anderem 365–372, 427–446, 551–561, 587–629, 667–679, 719–729, 984–1039) sowie der Fragenblöcke des Assistenten, auf die diese Antworten zielen. Die Assistentennachrichten wurden nicht vollständig gelesen, sondern nach Stichwörtern und zu den beantworteten Fragen geprüft; **Details der langen Karten- und Werkzeugnachrichten** (Codex-Berichte, QGIS, Kartenvarianten) sind ohne Ermittlungsbezug und nicht ausgewertet. Details: [Modul 22](22-Abdeckungsprotokoll.md#3-gelesene-quellen-und-lesetiefe).
- Der Chat 48 wurde im Bereich 40–200 (Quiz) vollständig, ab 200 nur auf Ermittlungsbezüge durchsucht (Rest: Tesla, Dateiarbeit, technische Befehle).
- **Nicht** ausgewertet: der Rest der Chats 23 und 35 (viele tausend Nachrichten zu Welt, Fahrzeugen, Waffen, Adult), die Chats 27 und 29 (GTA- und RDR2-Merkmalslisten, reines Vergleichsmaterial), 40 und 52. Aussagen zu Ermittlung dort wären als Ergänzung möglich; ein Treffer ändert hier nichts, solange er nicht in einer Entwicklerentscheidung mündet.
- **Referenzspiele** (RDR2, GTA, TLOU2) erscheinen in den Quellen als Vergleich. Sie sind keine Vorgaben.
- Es gibt **keine** Implementierung zu Fällen, Beweisen, Journal oder Recht. Das einzige gebaute Interaktionsfundament ist der persistente Weltzustand (siehe [README](../../../README.md), Abschnitt zum Implementierungsstand).
