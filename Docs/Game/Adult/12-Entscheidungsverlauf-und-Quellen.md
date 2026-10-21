# Adult 12: Entscheidungsverlauf und Quellen

> [!IMPORTANT]
> Dieses Dokument ist die **Quellenbasis („Master basis“)** der Adult-Dokumentation und enthält den zeitlichen Entscheidungsverlauf. Es kopiert keine Master-Inhalte. Die Detailplanung liegt überwiegend als Gesprächsprotokoll vor und ist **nicht in die kanonischen Statusdokumente konsolidiert**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation ist ohne Zugriff darauf lesbar; die Pfade dienen nur der Nachprüfbarkeit.

**Inhalt**

1. [Gewichtung der Quellen](#1-gewichtung-der-quellen)
2. [Zeitlicher Entscheidungsverlauf](#2-zeitlicher-entscheidungsverlauf)
3. [Master-Basis-Tabelle](#3-master-basis-tabelle)
4. [Was gelesen wurde und was nicht](#4-was-gelesen-wurde-und-was-nicht)

---

## 1. Gewichtung der Quellen

| Schicht | Inhalt | Gewicht |
|---|---|---|
| Konsolidierte Entscheidungen (31.08.2026) und Nachträge | Rahmen: Trennung vom Basisspiel, Modularität, Darstellungsgrenzen, Beziehungs-Sandbox, Dienstleistungs-Grundsätze, Stopp-Signal | **Bestätigt** (Kanon-Kandidat) |
| Entwicklerinterview (10. bis 27.09.2026) | Detailplanung in 20 Runden; Antworten des Entwicklers | **Bestätigte Planung des Entwicklers**, nicht konsolidiert; Aussagen des Assistenten darin sind Vorschläge, solange der Entwickler nicht ausdrücklich zustimmt |
| Rahmenentwurf (28.08.2026) | Architektur-Entwurf mit 58 Abschnitten und Master-Prompt | **Vorschlag**, teilweise überholt |
| Tageslieferungen 14. bis 19.10. | Review-Pakete | Kein Adult-Inhalt gefunden |
| Spätere ZIP-Zusammenfassungen (20.09., 27.09.) | im Chat erwähnt, nicht im Archiv | nicht auswertbar |

Regeln aus `CLAUDE.md` §11: Ein Dateiname oder Suchtreffer macht nichts zum Kanon; bei Konflikt zwei Quellen nennen und fragen. Genau das geschieht in [Modul 11](11-Offene-Fragen-und-Konflikte.md).

---

## 2. Zeitlicher Entscheidungsverlauf

| Datum (2026) | Ereignis |
|---|---|
| 25. bis 31.08. | Entscheidungen zu Basisspiel/Paket, Online-Modus ohne Mods, Darstellung, Stopp-Signal, Dienstleistungs-NPCs. Letzter konsolidierter Stand: 31.08. |
| 28.08. | Chat „Adult Content Framework“: großer Architekturentwurf (58 Abschnitte) und Master-Prompt; später teilweise überholt. |
| 10.09. | Interview Runde 1 und 2 (Gesamtrichtung, Hardcore als Preset, Kategorienstruktur). **Frühe Zustimmungsäußerungen des Entwicklers, die später ersetzt wurden** ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#52-überholte-zwischenstände)). Entwickler lehnt einen globalen Intensitätsschalter ab. |
| 11.09. | Runden 3 bis 9: Kategorien-Struktur (ohne Einzelakte), Präferenzen und Individualität, Beziehungs-/Attraction-Logik, Initiative und Spontanbegegnungen, Orte, Öffentlichkeit, Observer/Join, Altersgrenze 18 Jahre 0 Tage, Pakete, NPC-Generierung bei sehr vielen NPCs, Speicherung. |
| 11./12.09. | Körper, Aussehen, Kompatibilität; Mensch-Pipeline; Wunsch nach C++ statt Blueprints und eigenem Speichersystem. |
| 13.09. | Freeform-Interaktion, Positionen, Physik, Entscheidungsfähigkeit (Capacity Gate, bestätigt), Kamera, Replay und Aufnahme, Streaming. |
| 14.09. | Runde 14: Körper-Anpassung, Tattoos und Piercings im ersten Entwurf, kosmetische Eingriffe, Studios, Internet. |
| 15.09. | Runde 15 (175 Fragen) abgeschlossen: Rollen, Präferenzen, Dienstleistung, Orte, Gesundheit, Verhütung, Orientierung, Anziehung, Statistik-System (global), Setting-Entscheidung zur Orientierung. |
| 16. bis 25.09. | Mega-Runde 16 (über 400 Fragen in Paketen zu 25): Kategorien, Intensität, Orte, Items, Reaktionen, Master-Registry; **20.09.: „USER-LOCK“-Äußerungen** ([Modul 11, K2](11-Offene-Fragen-und-Konflikte.md#1-konflikte-die-eine-entscheidung-brauchen)); 25.09.: Audit-Fragen (A1 bis A15), Rechtsgespräch, Entwickler bittet, Runde 16 zu begrenzen. |
| 26.09. | Runde 17 (Tattoos, Piercings, Körpermodifikation) abgeschlossen; Runde 18 (Steuerung/UI/VR) begonnen. |
| 27.09. | Runde 18 abgeschlossen, Runde 19 (technische Integration) abgeschlossen, Runde 20 (Produktionsabschluss): Planung „im Wesentlichen fertig“; Veröffentlichungsziel bestätigt. Offen: Missing-Family-Audit, externer Lauf, Nachträge aus Prototyping und Recht. |
| 20./27.09. | Der Assistent erstellte Gesamt-ZIPs; ein Dokument „vom Benutzer gewünscht, nicht kanonisiert“ wurde angekündigt, ist im Archiv aber **nicht** enthalten. |

---

## 3. Master-Basis-Tabelle

Status-Klassen nach `CLAUDE.md` §11: **CONFIRMED / LOCKED**, **PROVISIONAL**, **PROPOSAL**, **OPEN**, **unclear / contradictory**.

| Quelle (Pfad) | Abschnitt | Verwendet in | Klasse |
|---|---|---|---|
| `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/DECISION_CHANGELOG_2026-08-31.md` | §16 Modding und optionale PC-Inhalte; §17 Darstellung im Basisspiel; §18 Reaktivität und Hard-Stop; §19 Dienstleistungs-NPCs und Nachtleben; §20 Körperzusammensetzung | Module 02, 04, 06, 08 | CONFIRMED / LOCKED |
| `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` | Abschnitt „Interaction / optional PC adult content“; „Body / physical response / recoil“ | Module 02, 06 | CONFIRMED |
| `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/OPEN_DECISIONS.md` | Abschnitt „Adult / romance / interaction“ | Modul 11 | OPEN |
| `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` | Beziehungs-/Romantik-Sandbox mit erwachsenen NPCs | Modul 05 | CONFIRMED |
| `01_CURRENT_CANONICAL_STATE/DISTRIBUTION_AND_OPTIONAL_PACKS/OPTIONAL_CONTENT_PACKS_2026-09-10.md` | Paket- und Sprachpaket-Logik (allgemein) | Modul 02 | CONFIRMED (allgemein, nicht Adult-spezifisch) |
| `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/planning-backlog/44-injury-gore-presentation-age-policy.md` | Alters- und Darstellungsprofile Verletzung (allgemein) | Modul 04 | PLANNING INVENTORY |
| `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/35_Projektwissen_nutzen_6a9cb579.md` | Interviewrunden 1 bis 20 (Nachrichten 185 bis 812); Rechtsgespräch (Nachrichten 715 bis 741) | alle Module | Entwicklerantworten: bestätigte Planung des Entwicklers, nicht konsolidiert; Assistentenaussagen: PROPOSAL; Rechtsaussagen: ungeprüft |
| `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/28_Adult_Content_Framework__6a90d754.md` | Rahmenentwurf, 58 Abschnitte | Module 02, 03 (Architektur) | PROPOSAL, teilweise überholt |
| `99_CANON_AUDITS/2026-10-15/01_SOURCE_COVERAGE.md` und `.../CONSOLIDATION_REVIEW/02_EVIDENCE_INDEX.md` | Abdeckung der Adult-Quellen | Modul 12 | Evidenz (Review-Paket, kein Kanon) |
| `99_DAILY_DELTAS/2026-10-14` bis `2026-10-19` | Suche nach „adult“ und „erwachsen“ | nichts | **nichts gefunden** |

---

## 4. Was gelesen wurde und was nicht

**Gelesen (soweit aufgeführt):**

- das vorhandene Dokument `Docs/Game/Adult-System.md` (Stand vor dieser Überarbeitung),
- alle **Antworten des Entwicklers** im Interview (Chat 35, Nachrichten 185 bis 812; Nachrichten 305 bis 439 behandeln andere Themen und wurden nur nach Stichworten geprüft),
- die Lock-Zusammenfassungen und Rückmeldungen des Assistenten zu den Runden 1, 5, 14, 17 bis 20 sowie zu den Konfliktstellen (Zustimmung, Admin-Build, Schlaf, Begleitvertrag, Orientierung, Kriminalität, Recht),
- die Adult-Passagen der Dokumente in der Tabelle oben (Entscheidungsänderungen 31.08., Projektentscheidungen, offene Entscheidungen, Nachtrag 05.09., Pakete, Track 44).

**Nur teilweise gelesen:**

- **Stand Etappe 2.1:** Die Fragetexte der Runden 6 bis 16 (ca. 1.900 Einzelfragen insgesamt) wurden gelesen und in den Modulen [15](15-Interviewprotokoll-Runden-1-bis-10.md) bis [17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md) ausgewertet. Die genaue Abdeckung steht in [Modul 13](13-Quelleninventar-und-Abdeckung.md).
- Der Rahmenentwurf (Chat 28) wurde in der Endfassung (Nachricht 25) sowie in den Nachrichten 17, 19, 21 und 23 und den Nutzernachrichten gelesen, die früheren Fassungen nur über Gliederung und Stichproben.
- Chat 35 vor Nachricht 185 (Beginn des Interviews) wurde nachgelesen (Adult-Passagen); Chat 23 und Chat 37 wurden, soweit sie Adult betreffen, gezielt gelesen. Genaue Bereiche: [Modul 13, Abschnitt 7](13-Quelleninventar-und-Abdeckung.md#7-nicht-oder-nur-teilweise-gelesen).

**Nicht ausgewertet:** das 2.9 GB umfassende restliche Archiv; die später angekündigte, aber im Archiv fehlende Datei „vom Benutzer gewünscht, nicht kanonisiert“; die Ergebnisse der Missing-Family-/Qwen-Audits (stehen aus).
