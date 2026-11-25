# Trockenlauf Aufgabensteuerung (NIGHT-AUDIT-03, Priorität 3)

- **Stand:** 08.11.2026
- **Ziel:** Feststellen, ob ein Coding-Agent eine Aufgabe nur über ihre Kennung korrekt verstehen und ausführen könnte. **Keine Implementierung, kein Build, kein Testlauf.** Testzählungen sind statisch aus dem Quelltext ermittelt, nicht ausgeführt.
- **Vorgehen je Aufgabe:** Zeile in `TASKS.md` über die Kennung, referenzierte Quellen auf Existenz, Abhängigkeiten in `DEPENDENCIES.md`, Implementierungsbelege im Repo, offene Entscheidungen.

## 1. PROTOTYPE-DOOR (in Arbeit)
- **Auffindbar über Kennung:** ja. Quelle `Docs/Proposals/0004-interaction-and-persistent-door.md` (existiert).
- **Abhängigkeit:** `FOUNDATION-WORLDFACTS` (Türzustand als Fakt; `DEPENDENCIES.md` Zeile 12).
- **Implementierungsstand (Belege):** `Core/include/spn/core/Interaction.h`, `InteractionSystem.h`, `TestDoor.h`; Tests `Tests/Core/DoorPersistenceTests.cpp` (9 `TEST_CASE`, statisch), `InteractionTests.cpp` (19), `InteractionSystemTests.cpp` (21), `InteractionReachTests.cpp` (7). Unreal-Seite laut Vorschlag §9 umgesetzt.
- **Stand laut Vorschlag §9 (22.10.2026, nicht in dieser Sitzung ausgeführt):** Core-Tests „181 von 181 auf vier Presets“, Unreal-Automation 14/14. Manuelle PIE-Abnahme offen.
- **Passt der Status?** Ja. „in Arbeit“ ist korrekt, solange die manuelle PIE-Abnahme offen ist.
- **Nächster Schritt:** Manuelle PIE-Abnahme nach Vorschlag §5.2 durch den Entwickler; danach Entscheidung über den Abschluss.
- **Offene Entscheidungen (Vorschlag §7):** Persistenzklasse der Türen (Nr. 2), Schlossmodell (3), Mehrspieler-Autorität (5), Bridge vs. Export (6), Eingabe-UI (9).
- **Hindernis:** Keine. Der Titel nennt „Inkremente 1–4“, der Vorschlag hat sechs Schritte (§6), davon 1–4 Code und 5 Dokumentation. Konsistent, aber der Titel könnte präziser sein.

## 2. PROTOTYPE-TESTMAP (abgeschlossen)
- **Auffindbar:** ja. Quellen `Docs/Proposals/0003`, `0008` §13: Dateien vorhanden.
- **Status-Befund:** „abgeschlossen“ bei gleichzeitiger Bemerkung „Editor-PIE offen“. Das überzeichnet den Stand, wenn die PIE zur Abnahme gehört. Der Entwickler muss das entscheiden. **Nicht geändert.**
- **Nächster Schritt:** Entwickler klärt, ob die Editor-PIE zur Abnahme zählt. Dann Status entweder bestätigen oder auf „in Arbeit“ setzen.
- **Akzeptanzkriterien:** Technical Acceptance PASS (23.10.2026) laut TASKS.

## 3. FOUNDATION-WORLDFACTS (abgeschlossen)
- **Auffindbar:** ja. Quellen `Docs/Decisions/0002-world-facts.md` und `0004-world-facts-save-format-v1.md` (beide vorhanden; in TASKS nur verkürzt).
- **Implementierungsstand (Belege):** `Core/include/spn/core/WorldFacts.h`, Golden-Datei `Tests/Core/Data/worldfacts_v1_golden.spnfacts`, Tests `WorldFactsTests.cpp` (34 `TEST_CASE`, statisch), `WorldFactsCompatibilityTests.cpp` (21).
- **Passt der Status?** Ja, soweit die Belege reichen. Der Tagesdelta 19.10. nennt „62 Tests pro Preset“ (Entwicklerangabe), das ist durch die statische Zählung nicht belegt und wurde nicht ausgeführt.
- **Hindernis:** Der Name „Phase 0.3“ ist doppelt belegt (CONFLICTS K-18). Der Nachweis ist über die Dokumente eindeutig, für einen Agenten aber verwirrend.
- **Abhängigkeiten:** Nachfolger `PROTOTYPE-DOOR`, `SIM-NPC-MEMORY`, `FOUNDATION-SAVE`.

## 4. P0-PLAYABLE-PLACEHOLDER (geplant)
- **Auffindbar:** ja. Quelle `README.md` Abschnitt 27.1.
- **Implementierungsstand:** Kein Dialogsystem im Core (`grep` über `Core/include` ohne Treffer für eigenständige Dialogkomponente; `InteractionReach.h`/`NpcMemory.h` erwähnen Dialog nur in Kommentaren). Status „geplant“ ist korrekt.
- **Fehlende Informationen:** Die Reihenfolge laut 27.3 ist nicht festgelegt (Bemerkung der Aufgabe). Der Agent kann nicht entscheiden, welche Elemente zuerst kommen.
- **Hindernis:** Ohne Reihenfolge und ohne Dialogdesign ist die Aufgabe nicht ausführbar. Das ist eine Entwicklerentscheidung.

## 5. DEC-UNDERGROUND-SCOPE (bereit)
- **Auffindbar:** ja. Quelle `OPEN_QUESTIONS.md` 0.3 (existiert) und `N-02` in `DECISIONS.md`.
- **Befund:** Die Aufgabe ist eine Entscheidungsaufgabe, keine Implementierung. Sie ist als „bereit“ markiert, weil die Textfassung von N-02 noch bestätigt werden muss. Das ist korrekt.
- **Abhängigkeiten:** `DEPENDENCIES.md` Zeile 31: Untergrund-Inhalte warten darauf.
- **Nächster Schritt:** Entwickler bestätigt den Wortlaut von N-02. Keine Codeänderung.

## Ergebnis des Trockenlaufs

| Aufgabe | Kennung auffindbar | Quellen existieren | Status passt | Hindernis |
|---|---|---|---|---|
| PROTOTYPE-DOOR | ja | ja | ja | PIE-Abnahme (Entwickler) |
| PROTOTYPE-TESTMAP | ja | ja | **fraglich** (PIE offen) | Entwicklerfrage zur Abnahme |
| FOUNDATION-WORLDFACTS | ja | ja | ja | K-18 (Doppelname), Testzahl nicht ausgeführt |
| P0-PLAYABLE-PLACEHOLDER | ja | ja | ja | Reihenfolge nicht festgelegt |
| DEC-UNDERGROUND-SCOPE | ja | ja | ja | Wortlaut N-02 bestätigen |

**Gesamturteil:** Die Kennungen sind eindeutig und die Quellen sind auffindbar. Ein Agent versteht die Aufgaben, sofern er den Hinweisen folgt (Abhängigkeiten, Bemerkungen). Für Implementierung fehlen jedoch oft Abnahmekriterien in der Quelle und die Reihenfolge der Phasen. Die Kennung „Phase 0.3“ ist mehrdeutig.
