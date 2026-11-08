# Planung: zentrales Register

Stand: 08.11.2026 (Paket 1 von mehreren, siehe `CHANGELOG.md`).

Dieses Verzeichnis ist der Einstieg für jede Claude-Sitzung, bevor Planungsfragen beantwortet oder Entscheidungen umgesetzt werden. Die Inhalte in `Docs/Game/` und im Master bleiben die Quellen. Dieses Verzeichnis fasst zusammen, **wo** eine Entscheidung steht, **welchen Status** sie hat und **was offen ist**.

## Dateien

| Datei | Inhalt |
|---|---|
| `DECISIONS.md` | Entscheidungsregister mit Status, Datum, Quelle (Pfad und Zeile) |
| `SOURCES.md` | Quellenindex: welche Dokumente gelesen wurden und welche nicht |
| `CONFLICTS.md` | Widersprüche zwischen Quellen, ohne Entscheidung, welche Version gilt |
| `OPEN_QUESTIONS.md` | Offene Fragen, nach Thema |
| `CHANGELOG.md` | Protokoll der Konsolidierungspakete mit geänderten Dateien |
| `CANON.md` | Aktueller gültiger Stand, kurz, mit Verweisen auf `DECISIONS.md` |
| `ROADMAP.md` | Phasen (neue Namen und Zuordnung zur alten Roadmap), Stand Phase 0 |
| `TASKS.md` | Aufgabenregister mit stabilen IDs, Status und Quelle |
| `DEPENDENCIES.md` | Abhängigkeiten zwischen Aufgaben und Entscheidungen |
| `Milestones/` | Bestehende Meilensteine (M1, M2-A, M2-B, NightBuild) mit Quelle und Stand |
| `PLANUNGSFRAGEN.md` | Regel für strukturierte Planungsfragen (Freitextoption, Vorrang, keine eigenmächtige Konfliktauflösung) |
| `NAMING_PROPOSAL.md` | Namens- und Phasensystem (genehmigt, Entwicklerwünsche eingearbeitet) |

## Statusklassen

Dieselben Klassen wie in `Docs/Game/Entscheidungsstatus.md` §1.1:

- **Verbindlich:** vom Entwickler entschieden. Änderung nur mit neuer ausdrücklicher Entscheidung.
- **Vorläufig:** Arbeitsstand, Testszenario oder Richtung. Keine Festlegung.
- **Vorschlag:** Entwurf eines Assistenten, nicht bestätigt.
- **Offen:** bewusst nicht entschieden.
- **Überholt:** durch eine spätere Entscheidung ersetzt. Der Eintrag bleibt zur Nachvollziehbarkeit erhalten.
- **Vision:** langfristiges Ziel ohne Spezifikation.
- **Verworfen:** ausdrücklich nicht mehr umgesetzt.

## Vorrang

1. Spätere ausdrückliche Entscheidung des Entwicklers gilt vor früheren.
2. Kanonische Statusdokumente gelten vor Archiven, Prompts und Experimenten.
3. Ein neueres Datum allein macht eine Aussage nicht verbindlich. Maßgeblich ist der Statusbegriff der Quelle.
4. Bei Widersprüchen wird nichts stillschweigend zusammengeführt. Der Widerspruch steht in `CONFLICTS.md`.

## Regeln für Sitzungen

- Vor einer Planungsantwort zuerst `DECISIONS.md` lesen, dann die verlinkte Quelle.
- Keine Entscheidung aus Prompt-Bibliothek, Tagesdeltas oder Assistentenvorschlägen ableiten.
- Keine Zahl als Festlegung behandeln, wenn der Status „Vorläufig“ oder „Vorschlag“ ist.
- Neue Entscheidungen des Entwicklers werden hier mit Datum eingetragen, bevor sie umgesetzt werden.
- **Leseregel (verbindlich):** Neuere bestätigte Entwicklerentscheidungen und Supersession-Vermerke haben Vorrang vor älteren Masterquellen (Vorrangregel 1 oben). Historische Masterangaben (z. B. 1.500–1.800 km², 2.000–2.200 km², Untergrundstädte mit 50–100 km²) sind keine aktuellen Anforderungen, solange kein aktueller Eintrag sie bestätigt. Vor einer Umsetzung zuerst `DECISIONS.md` und `CONFLICTS.md` prüfen, dann die Quelle.

## Stand der Konsolidierung

- Paket 1 (dieses): Grundstruktur, Entscheidungsregister mit den bestätigten Festlegungen aus `Entscheidungsstatus.md`, die Angaben vom November 2026, Konfliktliste, offene Fragen.
- Noch nicht gelesen und nicht übernommen: die 57 kanonischen Masterdateien, der Rest von `Docs/Game/` (221 Dateien insgesamt), die Tagesdeltas, die Historie. Details in `SOURCES.md`.
