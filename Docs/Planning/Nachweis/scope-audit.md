# Architektur- und Scope-Audit (NIGHT-AUDIT-03, Priorität 4)

- **Stand:** 08.11.2026
- **Nur Befunde.** Architekturänderungen werden als Vorschläge geführt, nicht beschlossen.

## 1. Historische Weltgrößen mit Verbindlichkeitsanmutung
- `Docs/Planning/CANON.md` Zeile 20 und 56: Die Größe ist offen (N-01), historische Zahlen sind keine Richtwerte mehr. Korrekt.
- Die Masterdateien CA5, CA6, CU8 (1.500–1.800 km² als Standard) und A15 (~2.000 km²) stehen vor N-01 (08.11.2026). Sie sind historisch und in den Docs als überholt markiert. **Ein Risiko besteht nur, wenn ein Agent den Master ohne die Supersession-Regel liest.** Vorschlag: In `Docs/Planning/README.md` die Lesereihenfolge um den Hinweis „Master-Größen sind überholt durch N-01“ ergänzen (nicht geändert).
- `Entscheidungsstatus.md` Zeile 207 führt 1.500–1.800 km² als historischen Eintrag (🕰️). Korrekt.

## 2. Untergrundkonzepte mit Verbindlichkeitsanmutung
- Die Gesellschaftsentscheidungen U24–U36 (`World/12`) gelten nach N-02 als Geltungsfrage (K-29, K-12). Die Docs markieren sie entsprechend. **Restrisiko:** `Entscheidungsstatus.md` Abschnitt 2.3 (Zeile 90, „Untergrund (Gesellschaft)“) führt sie noch ohne diesen Vermerk. Vorschlag: Hinweis „Geltung nach N-02 offen (K-29)“ dort ergänzen. Nicht geändert, weil der Abschnitt ein Entwicklerregister ist.

## 3. Unreal-Abhängigkeit im unabhängigen C++-Core
- Keine Unreal-Includes, keine UCLASS/UPROPERTY/GENERATED_BODY-Makros in `Core/include` und `Tests` (geprüft per Suche).
- Erwähnungen von Unreal in 37 Zeilen sind Kommentare. Einzige inhaltliche Brücke: `Core/include/spn/core/Coordinates.h` enthält eine **Konfiguration für die Unreal-Achsen** (`UnrealFrame`), die als Vorschlag markiert ist (`DEC-UNREAL-AXES`, geplant). Das ist akzeptabel, weil es Daten statt Konstanten sind.
- **Bewertung:** Keine unnötige Tiefe.

## 4. Unreal-Performance-Risiken (Dokumentation)
- Die Planung nennt Ziele (4K, 30/60 FPS, PS5-Modi, Budgets) als Richtung (GS §14.1). Konkrete Budgets fehlen (GP-GRAFIK-003: „Konkrete Budgets nicht gewählt“). **Risiko:** Ohne Budgetzahlen kann eine Umsetzung nicht gegen Ziele getestet werden. Vorschlag: Budgetfestlegung als Entwicklerentscheidung.
- Realm-Größen (350–750 km² je Reich, CA6/CU8) sind Planungsbereiche und haben große Auswirkung auf Streaming. Kein Performance-Nachweis vorhanden.

## 5. Abgrenzung Prototyp, Vertical Slice, Release, Ideen
- `ROADMAP.md`, `MS13` und `CA6` definieren Phase 0 (2–4 Monate, Placeholder in 2–4 Wochen), Phase 1 (1–3 km², 30–60 min) und die langfristigen Realms. Die Abgrenzung ist vorhanden.
- **Befund:** `TASKS.md` führt den Vertical Slice als `SLICE-CASE-01` (geplant, Alias `P2-VERTICAL-SLICE`), aber ohne überprüfbare Abnahmekriterien. Die Bemerkung verweist nur auf `OPEN_QUESTIONS.md`. Der Eintrag ist eine Quelle, kein Akzeptanzkriterium.
- Vorschlag: Die Abnahmekriterien für Phase 1 aus `ROADMAP.md` und `README` Kapitel 27.1 in den Eintrag übernehmen oder als eigene Aufgabe führen (nicht geändert).

## 6. Doppelte oder widersprüchliche Aufgaben
- `Phase 0.3` ist doppelt belegt (K-18): Interaktion (PROTOTYPE-DOOR) und WorldFacts (FOUNDATION-WORLDFACTS). Die Bezeichnung ist im Register vermerkt, aber nicht bereinigt.
- `PROTOTYPE-TESTMAP` ist „abgeschlossen“ mit offener PIE (siehe Trockenlauf). Widerspruch zwischen Status und Bemerkung.
- Keine doppelten Kennungen gefunden (geprüft: 45 Aufgaben, eindeutige IDs).

## 7. Status, die mehr Fortschritt suggeriert als belegt
- `PROTOTYPE-TESTMAP` (siehe oben).
- `FOUNDATION-WORLDFACTS`: Die Testzahl „62 pro Preset“ im Delta ist nicht belegt; der Status „abgeschlossen“ stützt sich auf den Code und den Vorschlag 0004 §9. Vertretbar.
