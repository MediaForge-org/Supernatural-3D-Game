# adult-audit

Reproduzierbare Hilfsskripte für den Quellen-Audit der Adult-Dokumentation (Etappe 2.2). Sie **lesen** das Planungs-Master (`SPN_MASTER`) und die Dokumentation unter `Docs/Game/Adult/`; sie schreiben nur in ein Arbeitsverzeichnis. Kein C++, kein Unreal-Bezug, keine Bibliotheken außer der Python-Standardbibliothek.

| Skript | Aufgabe |
|---|---|
| `split_chats.py` | zerlegt die Gesprächsprotokolle 23, 28, 35, 37 des Rekonstruktionspakets in JSON-Nachrichtenlisten |
| `parse_packages.py` | extrahiert die nummerierten Fragenpakete des Interviews (Chat 35, Nachrichten 186–810) und die Entwicklerantworten |
| `overrides_and_register_map.py` | trägt dokumentierte Sonderfälle nach (z. B. in einer Folgenachricht nachgereichte Antworten) und bildet Nachrichten auf Register-Einträge ab |
| `render_rows.py` | leitet je Frage einen **heuristischen** Status ab, abstrahiert explizite Formulierungen automatisch und verknüpft Register-Einträge |
| `write_markdown.py` | schreibt die Matrixmodule (Docs/Game/Adult/36 bis 40) nach `./stage` |
| `check_citations.py` | prüft Quellenangaben `35#n` usw. (Existenz, Rolle bei Suffix `E`/`A`) |
| `check_quotes.py` | prüft, ob wörtliche Zitate „…“ in den Quellen vorkommen |
| `check_links.py` | prüft relative Links und Markdown-Anker |

## Aufruf

```bash
SPN_MASTER=/pfad/zu/SPN_FULL_CUMULATIVE_MASTER_2026-09-27 bash Tools/adult-audit/run.sh [ARBEITSVERZEICHNIS]
# danach, im Arbeitsverzeichnis:
SPN_ADULT_DOCS=<repo>/Docs/Game/Adult SPN_MASTER=... python3 <repo>/Tools/adult-audit/check_citations.py
SPN_ADULT_DOCS=<repo>/Docs/Game/Adult SPN_MASTER=... python3 <repo>/Tools/adult-audit/check_quotes.py
python3 <repo>/Tools/adult-audit/check_links.py <repo>/Docs/Game
```

## Grenzen

- Der Status je Frage ist eine **Heuristik** über den Antworttext (ja/nein/offen …) und keine Entscheidung.
- Die Abstraktion expliziter Formulierungen ist **lexikonbasiert** und kann zu viel oder zu wenig schwärzen.
- Der Register-Bezug beruht auf Nachrichtennummern und Fragenummern im Text der Register-Einträge; `–` bedeutet nur, dass kein Eintrag die Frage namentlich nennt.
- Quelle und Ergebnis dürfen nicht ohne Prüfung als Kanon gelten (siehe `CLAUDE.md`, Abschnitte 2 und 11).
