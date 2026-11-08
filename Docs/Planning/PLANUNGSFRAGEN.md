# Regel für strukturierte Planungsfragen

- **Stand:** 08.11.2026
- **Status:** Verbindlich (Entwicklerangabe 08.11.2026, Bestätigung von Q-DELTA-1)
- **Geltungsbereich:** Jede strukturierte Planungsfrage an den Entwickler (Auswahl, Entscheidungsfrage, Optionsliste) in `Docs/Planning/` und in Planungsantworten. Gewöhnliche Implementierungsaufträge sind ausdrücklich ausgenommen: Dafür ist keine Multiple-Choice-Frage nötig.

## Regeln

1. **Freitextoption ist Pflicht.** Jede Planungsfrage mit vorgegebenen Optionen enthält eine Freitextoption, mit der der Entwickler eigene Entscheidungen, Ergänzungen oder Änderungen formulieren kann.
2. **Freitext hat Vorrang.** Eine ausdrückliche Freitextantwort geht vor den vorgegebenen Auswahlmöglichkeiten.
3. **Kombinationen sind zulässig.** Mehrere Optionen dürfen zusammen gewählt und mit Freitext ergänzt werden.
4. **Keine eigenmächtige Auflösung.** Ist die Antwort mehrdeutig oder widerspricht sie einer bestehenden verbindlichen Entscheidung, wird der Konflikt nicht eigenmächtig aufgelöst. Stattdessen werden beide Quellen genannt (`CONFLICTS.md`) und der Entwickler wird gefragt.
5. **Keine Vorauswahl als Entscheidung.** Eine Assistenten-Empfehlung ist keine Entscheidung und wird nicht als Antwort gewertet, solange der Entwickler sie nicht ausdrücklich übernimmt.
6. **Dokumentation.** Die Antwort wird mit Datum in `DECISIONS.md` eingetragen, bevor sie umgesetzt wird (siehe `README.md`).

## Anwendung

- Offene Fragen in `OPEN_QUESTIONS.md` werden als Freitextfragen formuliert, mit einer Optionsliste aus den Quellen als Vorschlag.
- Für Fragen aus Tagesdeltas gilt dieselbe Regel: Die Deltas sind Vorschläge und keine Antworten.
- Wenn eine Frage bereits in einer Quelle beantwortet ist, wird die Quelle genannt und die Frage nicht erneut gestellt.

## Quelle

- Q-DELTA-1 (Entwicklerangabe 08.11.2026): Die Freitext-Regel ist bestätigt und gilt als Planungsregel.
