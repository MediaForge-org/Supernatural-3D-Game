# Adult 41: Audit Etappe 2.2 (Quellen, Matrix, Register-Abgleich)

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dies ist der Bericht über den Quellen- und Vollständigkeits-Audit der Adult-Dokumentation. Er trifft **keine** Entscheidungen und ändert **keinen Kanon**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Vorgänger: [Modul 13](13-Quelleninventar-und-Abdeckung.md) (Quelleninventar), [Modul 14](14-Entscheidungsregister.md) (Register). Ergebnisse: [Matrix 36](36-Frage-Antwort-Matrix-Runden-1-bis-9.md), [37](37-Frage-Antwort-Matrix-Runden-10-bis-13.md), [38](38-Frage-Antwort-Matrix-Runden-14-und-15.md), [39](39-Frage-Antwort-Matrix-Runde-16.md), [40](40-Frage-Antwort-Matrix-Runden-17-bis-20.md). Werkzeuge: `Tools/adult-audit/`.

**Inhalt**

1. [Ausgangslage](#1-ausgangslage)
2. [Neu ausgewertete Quellen](#2-neu-ausgewertete-quellen)
3. [Methode der Frage-Antwort-Matrix](#3-methode-der-frage-antwort-matrix)
4. [Ergebnisse der Matrix](#4-ergebnisse-der-matrix)
5. [Rückfragen ohne belegte Bestätigung](#5-rückfragen-ohne-belegte-bestätigung)
6. [Abgleich mit dem Entscheidungsregister](#6-abgleich-mit-dem-entscheidungsregister)
7. [Korrekturen an bestehenden Modulen](#7-korrekturen-an-bestehenden-modulen)
8. [Durchgeführte Prüfungen](#8-durchgeführte-prüfungen)
9. [Weiterhin offene Lücken](#9-weiterhin-offene-lücken)

---

## 1. Ausgangslage

| Punkt | Stand |
|---|---|
| Repository | Branch `main`, letzter Commit `5550b06` (‚docs: expand adult planning with sources and specifications‘), Arbeitsbaum zu Beginn sauber |
| Adult-Dokumentation zu Beginn | 35 Module unter `Docs/Game/Adult/` plus Hub `Docs/Game/Adult-System.md`: 7.345 Zeilen |
| Register zu Beginn | 358 Einträge |
| Konflikte zu Beginn | K1 bis K22; offene Entscheidungen OE-01 bis OE-85 |

## 2. Neu ausgewertete Quellen

| Quelle | Umfang | Befund | Folge |
|---|---|---|---|
| Chat 35, Nachrichten **305–439** | 135 Nachrichten (55 vom Entwickler), 116 Tsd. Zeichen | Obsidian-Vault, Command-Center, Git-Einrichtung, Übersetzungen (Fremdthema). Stichwortscan: 20 Nachrichten mit Treffern, alle gelesen: nur „Adult-Interview-Stand“ als Listenpunkt von ZIP-Inhalten und ein Dateipfad `QWEN_V0_9_PARTIAL_SECTIONS_1_TO_28_REFERENCE.md` (35#366). **Keine Adult-Entscheidung.** | Hinweis auf Qwen-Referenzausgaben führte zum Vault-Fund (unten) |
| Chat 35, Nachrichten 1–57, 79–142, 813–951 | Stichwortscan, alle Treffer gelesen | Adult-Profil im Mod-Manager (35#58, 35#133), Content-Rating-Filter (35#141), Adult-Abschlussmeldungen (35#814, 816, 818, 872) | E-PA-037 |
| Chat 28, **frühe Fassungen** 2, 4, 6, 9, 11 (und 15, 17, 21) | Gliederung 58, 166, 221, 246 Überschriften | Die Sicherheitsausschlüsse stehen seit Fassung 2; Party-Leader-/Admin-Consent und Story-/Quest-Zwang kamen in den Fassungen 4, 6, 9 hinzu | E-SK-038; [Modul 35](35-Auswertung-Rahmenentwurf-28-08.md) |
| Chat 23, **84 Nachrichten außerhalb der bisher gelesenen Bereiche** mit Stichworttreffern | gelesen u. a. 86, 598, 627, 640–676, 882, 1035, 1037, 1074–1076, 1088–1090 | Neu: Block 10A (mehrere parallele Beziehungen, „nahezu jeder erwachsene NPC“, Familiengründung offen), Nachtleben-Erweiterung (23#1037), Gore-/Kinder-Entscheidung (23#1074–1075) | E-NP-031, E-NP-032, E-WE-022, E-SK-036, E-SK-037 |
| Chat 37, Tagespläne 53–90 | Adult-Zeilen von 20 Nachrichten | Der Tagesplan-Assistent nennt eine **ältere Reihenfolge** (Audit, Qwen, erst dann Runde 17); Chat 35 hat Runde 17 bereits am 25./26.09. durchgeführt | [K23](19-Konflikt-und-Widerspruchsmatrix.md#k23-reihenfolge-der-abschlussschritte), E-PR-018, E-PR-019 |
| ZIP-Lieferungen im Chat 35 (554/555, 657–664) | Chat-Beschreibungen | 13.–15.09.: **Delta-Paket gefunden** (unten). 20.09.: Master-ZIP mit `USER_REQUESTED_BUT_NOT_CANONICALIZED.md` **nicht auffindbar**; Assistent 35#660 beschreibt den Inhalt (nur sichere Architekturzusammenfassung) | E-XX-024, OE-85 |
| Einzige ZIP im Master (`SPN_CANON_MERGE_2026-10-18.zip`) | 12 Dateien | Addenda vom 18.10.; kein Adult-Bezug | – |
| **Knowledge Vault v2** des Entwicklers (außerhalb Master): Delta 10.–12.09. (`VD12`) | 8 Dokumente (Interview-Snapshot bis Runde 9C, Review der Qwen-Architektur, Richtungsdokumente, Modding-Blöcke 4 und 5), 2 Qwen-Referenzausgaben (59 KB und 31 KB), 3 Qwen-Prompts | Der Snapshot (Status „IN PROGRESS – NOT FINAL“) deckt sich mit Register und Interview. Die Qwen-Ausgaben sind **Teilausgaben** (bis Abschnitt 28 bzw. ca. 30), „Referenzmaterial“. Die Aussage ‚kein Qwen-Ergebnis im Archiv‘ gilt nur für den Master. | E-SP-010, E-PA-037, Hinweise an E-PA-025 und E-LE-003 |
| **Delta-Paket 13.–15.09.** (`VD15`, außerhalb Master): `COMPLETE` und `VAULT_MERGE` | je ca. 25–28 KB, 19 Dokumente | Lock-Dokumente zu den Runden **10, 11, 13, 14, 15**, Plan der Runden 16–19, Crime-, Content- und Setting-Richtung. Kein Widerspruch zum Register; bestätigt E-NP-025, E-GE-004, E-WE-006 | E-PR-020; Hinweise an E-LE-011, E-NP-025, E-GE-004, [K3, K5, K10, K15](19-Konflikt-und-Widerspruchsmatrix.md) |
| Kumulative Master-ZIPs 23.–26.09. und Tages-Deltas Oktober (`SPN_VAULT_V2`) | 4 + 5 ZIPs | kein Adult-Dateiname, keine Runde-16-Konsolidierung | – |
| ChatGPT-Gesamtexport (521 Gespräche) | Abgleich der Nachrichtenzahlen | Chats 23, 28, 35, 37 stimmen mit dem Rekonstruktionspaket überein; keine weiteren SPN-Gespräche mit Adult-Planung | – |
| Master-Archiv, Volltextsuche nach Adult-Stichworten | 4.576 Textdateien | zusätzliche Fundstellen: `DECISION_CHANGELOG_2026-08-31` §52 und §53 (Kinder/Gore), `WORLD_DESIGN_BLOCKS_8_TO_9_CURRENT` (04.09.: Gewalt „substantially beyond RDR2“), `CURRENT_SUPERSESSION_2026-09-03` (Nachtleben und Adult-Unterhaltung) | E-SK-036, E-SK-037 |

## 3. Methode der Frage-Antwort-Matrix

1. **Pakete:** Die Chat-35-Nachrichten 186–810 wurden maschinell nach nummerierten Fragelisten durchsucht (längste aufsteigende Folge je Assistentennachricht, mindestens fünf Einträge). Ergebnis: **66 Fragenpakete mit 1.903 Fragen** (Runden 1–20, Körperreaktions-Block, Pre-Audit). Nicht-Frage-Listen (Rundenpläne, Aufwandsschätzungen, Zusammenfassungen) wurden ausgeschlossen.
2. **Antworten:** Entwicklernachrichten zwischen einem Paket und dem nächsten wurden nach Zeilen der Form `n`, `n-m`, `n, m` (beim Pre-Audit mit `a`-Präfix) geparst. Bereichsantworten (‚1-9 ja, aber …‘) gelten für den ganzen Bereich und erscheinen mehrfach. Spätere Nachträge erscheinen als `A → B`.
3. **Manuelle Nachträge** (im Skript dokumentiert): Runde 17A Frage 13 (35#751), 17B Fragen 23–28 (35#761, 35#763), 17E Fragen 81–100 (Pauschalantwort 35#771: „ja passt alles, aber vieles davon ist selbsterklärend“), 18A Frage 4 („4?“).
4. **Status:** heuristisch aus dem Antworttext (siehe Lesehilfe in den Matrixmodulen). ‚Rückfrage → pauschal bestätigt‘ nur, wenn direkt nach der Erklärung des Assistenten ein kurzes „Ja/Ok passt“ folgt, die Erklärung die Fragenummer nennt und das Dankeswort keine Zahl enthält.
5. **Register:** Je Frage werden Register-Einträge genannt, deren Text die Fragenummer oder einen Bereich mit dieser Nummer **ausdrücklich** nennt und deren Quelle die Antwortnachricht (oder die unmittelbare Assistentenreaktion) ist. Ohne ausdrückliche Nennung steht `–`; die Paketebene steht im Kopf jedes Abschnitts.
6. **Abstraktion:** Explizite Formulierungen der Fragen und Antworten werden lexikonbasiert durch Hinweise ersetzt. Das ist eine Schutzmaßnahme der Dokumentation, kein Urteil über den Inhalt.
7. **Modulzuordnung:** auf **Paketebene**, nicht je Frage belegt; das ist in den Matrixmodulen ausdrücklich vermerkt.

**Grenzen:** Das Parsing kann einzelne Zeilen falsch zuordnen (z. B. Zahlen in Freitexten). Beim Durchlesen von rund 540 inhaltlichen Antworten fanden sich keine Fehlzuordnungen außer den oben genannten und korrigierten Fällen. Der Status „📋“ bedeutet **Entwicklerantwort**, nicht Kanon und nicht Zustimmung zu Zahlen oder Details des Assistenten.

## 4. Ergebnisse der Matrix

**Gesamt:** 66 Pakete, 1.903 Fragen, **1.903** mit mindestens einer Antwort im Chat (davon 20 Pauschalantworten in 17E und 80 delegierte in Runde 12). Bei 37 Fragen wurde die Antwort später ergänzt oder geändert. Für **1.243** Fragen nennt mindestens ein Register-Eintrag die Frage ausdrücklich.

| Paket | Fragen-Nachricht | Fragen | ja | ja mit Zusatz | Freitext | delegiert | bedingt | Rückfrage bestätigt | offen | nein | mit Register-Bezug je Frage |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Runde 1: Gesamtrichtung | 35#186 | 12 | 1 | 8 | 2 | 0 | 0 | 0 | 0 | 1 | 0 |
| Runde 2: Content-Struktur und Rollen | 35#188 | 12 | 5 | 2 | 2 | 0 | 0 | 0 | 3 | 0 | 6 |
| Runde 3: Kategorien-Struktur | 35#220 | 12 | 7 | 4 | 0 | 0 | 1 | 0 | 0 | 0 | 0 |
| Runde 4: Präferenzprofile | 35#226 | 15 | 10 | 3 | 1 | 0 | 1 | 0 | 0 | 0 | 15 |
| Runde 5: Beziehung, Attraktion | 35#228 | 15 | 10 | 2 | 1 | 0 | 1 | 0 | 0 | 1 | 12 |
| Runde 6: Initiative, Flirt | 35#230 | 15 | 11 | 4 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 7: Orte, Öffentlichkeit, Observer | 35#232 | 15 | 13 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 8: Pakete, Modularität | 35#246 | 15 | 6 | 2 | 3 | 0 | 0 | 4 | 0 | 0 | 1 |
| Runde 9A: Präferenz-DNA | 35#260 | 45 | 37 | 3 | 5 | 0 | 0 | 0 | 0 | 0 | 45 |
| Runde 9B.1: Kern/Gedächtnis | 35#268 | 30 | 29 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 9B.2: Lernen, Dates, Beziehungen | 35#270 | 60 | 57 | 0 | 0 | 0 | 1 | 0 | 1 | 1 | 0 |
| Runde 9B.3: Langzeit, Alter, Tod | 35#272 | 50 | 50 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 9C: Weltsimulation, Speicher | 35#278 | 40 | 36 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 35 |
| Runde 10A: Körperprofil | 35#280 | 35 | 19 | 13 | 1 | 0 | 0 | 0 | 2 | 0 | 35 |
| Runde 10B: Skelett, Kontakt | 35#282 | 35 | 0 | 34 | 0 | 0 | 0 | 0 | 1 | 0 | 0 |
| High-Fidelity Body Response 1–20 | 35#288 | 20 | 14 | 3 | 3 | 0 | 0 | 0 | 0 | 0 | 8 |
| High-Fidelity Body Response 21–40 | 35#290 | 20 | 15 | 1 | 2 | 0 | 0 | 0 | 2 | 0 | 7 |
| Runde 10: Szenensystem | 35#443 | 100 | 95 | 4 | 1 | 0 | 0 | 0 | 0 | 0 | 8 |
| Runde 11: Freeform-Engine | 35#457 | 100 | 67 | 12 | 10 | 0 | 1 | 0 | 10 | 0 | 82 |
| Runde 11B: Ergänzungen | 35#463 | 35 | 24 | 3 | 4 | 0 | 1 | 0 | 3 | 0 | 28 |
| Runde 12: Replay, Aufnahme, Streaming | 35#483 | 100 | 3 | 16 | 1 | 80 | 0 | 0 | 0 | 0 | 100 |
| Runde 13: Ablauf, Reaktionen | 35#485 | 100 | 93 | 2 | 2 | 0 | 0 | 3 | 0 | 0 | 68 |
| Runde 14: Körper, Tattoos | 35#499 | 100 | 72 | 20 | 8 | 0 | 0 | 0 | 0 | 0 | 69 |
| Runde 14B: Piercings, Körpermodifikation | 35#511 | 40 | 37 | 1 | 2 | 0 | 0 | 0 | 0 | 0 | 40 |
| Runde 15A: Präferenzmodell | 35#523 | 25 | 18 | 4 | 3 | 0 | 0 | 0 | 0 | 0 | 3 |
| Runde 15B: Gruppen, Branche, Medien | 35#525 | 25 | 17 | 5 | 3 | 0 | 0 | 0 | 0 | 0 | 14 |
| Runde 15C: Orte, Öffentlichkeit, Produktion | 35#527 | 25 | 22 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 15D: Praktiken-Präferenzen, Fesseln, Rollenspiel | 35#529 | 25 | 23 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 15E: Solo, Medien, Dienstleistung | 35#539 | 25 | 18 | 3 | 3 | 0 | 0 | 0 | 1 | 0 | 11 |
| Runde 15F: Gesundheit, Hygiene | 35#541 | 25 | 19 | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 23 |
| Runde 15G: Orientierung, Dynamik | 35#549 | 25 | 24 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 23 |
| Runde 15H: Kompatibilität, Statistik | 35#551 | 25 | 24 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16A: Master-Taxonomie | 35#573 | 25 | 19 | 3 | 3 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16B: Basiskategorien | 35#583 | 25 | 10 | 13 | 1 | 0 | 0 | 1 | 0 | 0 | 25 |
| Runde 16C: Positionen | 35#589 | 25 | 21 | 4 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16D: Steuerung, Intensität | 35#609 | 25 | 24 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16E: Reaktionen | 35#611 | 25 | 25 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16F: Gewebe, Verletzung | 35#618 | 25 | 19 | 6 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16G: NPC-Verhalten | 35#624 | 25 | 25 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16H: UI, Einstellungen, Technik | 35#626 | 25 | 23 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 5 |
| Runde 16I: Registry I | 35#642 | 25 | 0 | 25 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16J: Unterkategorien | 35#650 | 25 | 0 | 25 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 16K: Reaktions-/Outcome-Familien | 35#652 | 25 | 11 | 13 | 0 | 0 | 0 | 0 | 1 | 0 | 25 |
| Runde 16L: Empfindung, Erholung | 35#676 | 25 | 25 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16M: Registry II | 35#678 | 25 | 24 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16N: Höhepunkt, Kleidung, Solo | 35#686 | 25 | 24 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 24 |
| Runde 16O: Orte | 35#692 | 25 | 24 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Runde 16P: Kombinierte Aktionen | 35#694 | 25 | 24 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16Q: Risiko, Körperzustand | 35#706 | 25 | 24 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 25 |
| Runde 16R: Produktions-Locks | 35#712 | 12 | 9 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 12 |
| Pre-Audit A1–A15 | 35#742 | 15 | 11 | 1 | 1 | 0 | 0 | 0 | 1 | 1 | 0 |
| Runde 17A: Tattoo-Grundmodell | 35#748 | 20 | 12 | 7 | 1 | 0 | 0 | 0 | 0 | 0 | 19 |
| Runde 17B: Stile, Motive | 35#760 | 20 | 3 | 12 | 5 | 0 | 0 | 0 | 0 | 0 | 18 |
| Runde 17C: Künstler, Studios | 35#766 | 20 | 15 | 4 | 1 | 0 | 0 | 0 | 0 | 0 | 20 |
| Runde 17D: Prozess, Heilung | 35#768 | 20 | 18 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 20 |
| Runde 17E: Cover-up, Laser | 35#770 | 20 | 0 | 20 | 0 | 0 | 0 | 0 | 0 | 0 | 20 |
| Runde 17F: NPC-Tattoo-Kultur | 35#774 | 12 | 5 | 7 | 0 | 0 | 0 | 0 | 0 | 0 | 12 |
| Runde 17G: Tattoo-Technik | 35#778 | 10 | 3 | 6 | 1 | 0 | 0 | 0 | 0 | 0 | 10 |
| Runde 17H: Piercings, Körpermodifikation | 35#782 | 12 | 12 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 12 |
| Runde 17I: Abschluss | 35#784 | 7 | 3 | 4 | 0 | 0 | 0 | 0 | 0 | 0 | 7 |
| Runde 18A: Steuerung | 35#790 | 20 | 13 | 3 | 1 | 0 | 0 | 0 | 3 | 0 | 14 |
| Runde 18B: VR | 35#794 | 10 | 5 | 1 | 4 | 0 | 0 | 0 | 0 | 0 | 10 |
| Runde 18C: Bedienung | 35#800 | 12 | 12 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 12 |
| Runde 19A: Technik | 35#804 | 20 | 19 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 19 |
| Runde 19B: Speicher, Builds | 35#806 | 12 | 12 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 12 |
| Runde 20A: Production Lock | 35#810 | 15 | 14 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 14 |
| **Summe** | | **1903** | **1364** | **327** | **82** | **80** | **6** | **9** | **28** | **7** | **1243** |

Die Spalte ‚mit Register-Bezug je Frage‘ misst nur die **namentliche Nennung** im Registertext; alle Pakete sind auf Paketebene durch Register-Einträge gedeckt.

## 5. Rückfragen ohne belegte Bestätigung

Die folgenden Fragen wurden vom Entwickler nur mit Rückfrage, „weiß nicht“ oder „kann sein“ beantwortet; es folgt **keine** belegte ausdrückliche Bestätigung. Aussagen des Registers, die sich allein auf diese Fragen stützen, tragen höchstens den Status 🧭 (zwei Einträge wurden entsprechend angepasst: E-IA-001, E-IA-005). Die übrigen Register-Einträge stützen sich zusätzlich auf Antworten zu Nachbarfragen oder auf spätere Zustimmung.

| Paket | Nr. | Frage (gekürzt) | Antwort | Betroffene Register-Einträge |
|---|---:|---|---|---|
| Runde 2 | 16 | Soll ein Character unterschiedliche Grenzen je Rolle haben? Also etwas | verstehe ich nicht | E-IA-004 |
| Runde 2 | 23 | Soll eine Interaction mehrere Eskalationsstufen besitzen, zwischen den | wie genau? | E-IA-001 |
| Runde 2 | 24 | Soll das Mod-System praktisch unbegrenzt neue Kategorien hinzufügen kö | genauer noxh bitte | E-IA-005 |
| Runde 3 | 9 | Soll jede Location zusätzlich beeinflussen: | kann sein | – |
| Runde 4 | 8 | Soll ein NPC nach positiven/negativen Erfahrungen dauerhafte Preferenc | kann schon sein | E-NP-004 |
| Runde 5 | 8 | Soll ein One-Night-Stand später: | kann alles sein | E-NP-007 |
| Runde 9B.2 | 16 | Soll ein gutes Date nicht automatisch in Adult-Content enden? | kann sein aber oft mit | – |
| Runde 9B.2 | 53 | Soll Gossip realistisch begrenzt sein, also nicht sofort die ganze 2.0 | was ist gossip | – |
| Runde 10A | 14 | Soll Schwangerschaft/Familienplanung später nur über ein separates all | weiß noch nicht | E-KO-001 |
| Runde 10A | 30 | Soll Make-up mit Regen, Wasser, Zeit usw. reagieren können, wenn das j | keine ahnung wie das in echt ist | E-KO-001 |
| Runde 10B | 36 | Soll das Skeleton selbst weitgehend standardisiert bleiben, während un | ? | – |
| High-Fidelity Body Response 21–40 | 22 | Sollen Artists für Hero-Characters trotzdem Muskelaktivierungen manuel | welche artists | – |
| High-Fidelity Body Response 21–40 | 25 | Soll es unterschiedliche Soft-Tissue-Profile geben, abhängig von Körpe | was genau | – |
| Runde 11 | 41 | Soll Soft-Tissue-Physik komplett sekundär zur Skelettbewegung laufen,  | was ist das | E-SO-013 |
| Runde 11 | 57 | Soll auch deine bereits festgelegte Möglichkeit geringfügig unterschie | was ist IK und das andere | E-SO-015 |
| Runde 11 | 58 | Soll der Solver solche Unterschiede automatisch kompensieren? | was ist IK und das andere | E-SO-015 |
| Runde 11 | 59 | Soll sich dadurch dieselbe Haltung bei zwei verschiedenen NPCs leicht  | was ist IK und das andere | E-SO-015 |
| Runde 11 | 70 | Soll die Variation deterministisch aus Session-Seed + Character-State  | was genau | E-SO-016 |
| Runde 11 | 73 | Soll es einen Modifier geben, mit dem ausgewählt wird, welcher Körperb | weiß ich nicht, was genau meinst du | – |
| Runde 11 | 74 | Soll die Kamera dabei helfen, den gewählten Bereich verständlich hervo | vielleicht, ui kann man mehr oder weniger einstellen können | – |
| Runde 11 | 80 | Soll es trotzdem einfache Schnellbefehle geben, damit normale Spieler  | wieso sollte man ein 3d animationsprogramm bedienen | E-SO-019 |
| Runde 11 | 92 | Soll es mehrere Fallback-Stufen geben: Micro-Correction → IK-Relaxatio | harter reset würde ich sogut wie nie machen, oder wie meinst du das | E-SO-021 |
| Runde 11 | 94 | Soll jede problematische Pose im Debug-Build automatisch mit Körperpar | was ist problematisch für dich | – |
| Runde 11 | 95 | Soll der Mod Editor später einen Pose-/Contact-Debugger besitzen? | was ist das | – |
| Runde 11B | 108 | Oder willst du langfristig sogar, dass selbst diese groben Kategorien  | das weiß ich noch nicht | E-SO-025 |
| Runde 11B | 110 | Soll eine sehr unerfahrene Person solche Absichten manchmal schlechter | das kann sein, nur weil unerfahrenen oder sogar jungfrauen heißt nicht | E-SO-025 |
| Runde 11B | 130 | Soll Kraft immer über die echte Körperkraft des Charakters laufen – al | welche adult werte meinst du | – |
| Runde 11B | 131 | Soll Controller-Steuerung grundsätzlich auf Bewegen + Modifier + Konte | weiß ich noch nicht, vielleicht auch wie bei ufc spielen mit den posit | – |
| Runde 15E | 125 | Präferenzsystem ohne festes Ende | weiß nicht was eine fetisch db ist | – |
| Runde 16K | 257 | Regurgitation: Soll Regurgitation – Hochkommen von Magen-/Speiseröhren | was genau ist gemeint | E-KO-023 |
| Pre-Audit A1–A15 | 5 | Sensation Family: Haben wir mit Pain, Temperatur, Textur, Druck, Sicht | genauer sagen | – |
| Runde 18A | 1 | Zwei Steuerungsmodi: Soll es grundsätzlich Direct Control und Assisted | ich weiß noch nicht, ich finde das ufc system etwas erweitert ist scho | E-ST-002 |
| Runde 18A | 2 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung] | schaue auf 1 weiß nicht genau noch | – |
| Runde 18A | 4 | Kontextabhängige Liste: Soll dieses Menü automatisch nur Dinge zeigen, | 4? | – |

## 6. Abgleich mit dem Entscheidungsregister

### 6.1 Neue Einträge (Stand 371, vorher 358)

| ID | Inhalt | Quelle |
|---|---|---|
| E-SK-036 | Kinder als Weltbewohner, eingeschränktes Gore-Profil, keine Belohnung für Gewalt gegen Kinder | K§52, PD, T44 |
| E-SK-037 | Gewalt gegen Erwachsene/Monster „substantially beyond RDR2“, Kinder ohne expliziten Gore; Begründung Plattformrisiko | WORLD_DESIGN_BLOCKS_8_TO_9, 23#1074/1075 |
| E-SK-038 | Sicherheitsausschlüsse seit der ersten Fassung des Rahmenentwurfs | 28#2, 4, 6, 9 |
| E-NP-031 | mehrere parallele Beziehungen; nahezu jeder erwachsene NPC ansprechbar | 23#1089, 23#1090 |
| E-NP-032 | Zusammenziehen, Heirat, Immobilien: ja; Familiengründung offen | 23#1089, 23#1090 |
| E-WE-022 | Nachtleben „essentiell“; Clubs mit bezahlter Dienstleistung in vielen Stärken | 23#1035, 23#1037 |
| E-PR-017 | alles komplett genau definieren (Welt, Gameplay, Charaktere, Adult, Locations) | 35#598 |
| E-PR-018 | Qwen nicht verfügbar, Runde 17 jetzt; abweichende Tagesplan-Reihenfolge | 35#747, 37#82 |
| E-PR-019 | Runde 16: Frage 400 kein geplantes Ende, keine Fragen nur zur Vergrößerung | 37#79, 37#80 |
| E-PR-020 | Konsolidierung vom 15.09. (Runden 1–11, 13–15 abgeschlossen) | VD15 |
| E-PA-037 | Content-Rating-/Adult-Metadaten in Mod-Metadaten | VD12 (Modding Block 1 Frage 28) |
| E-SP-010 | Qwen-Referenzausgaben (Teilausgaben, nicht kanonisch) | VD12 |
| E-XX-024 | Assistent 35#660: Inhalt des ZIPs vom 20.09. (nur sichere Architektur), Datei nicht auffindbar | 35#660, 35#662 |

### 6.2 Statusänderungen

| ID | vorher | nachher | Grund |
|---|---|---|---|
| E-IA-001 | 📋 | 🧭 | Frage 23 „wie genau?“; keine ausdrückliche Entwicklerantwort nach der Erklärung |
| E-IA-005 | 📋 | 🧭 | Frage 24 ‚genauer noch bitte‘; keine ausdrückliche Zustimmung nach der Erklärung |

### 6.3 Quellenkorrekturen und -ergänzungen

| ID | Änderung |
|---|---|
| E-SK-035 | Quelle 35#708 (Assistentennachricht) korrigiert zu 35#707 (Entwickler) |
| E-IA-033 | Quelle 35#686 (Assistentennachricht) korrigiert zu 35#687 (Entwickler) |
| E-RC-006 | zusätzlich 35#723, 725, 731 |
| E-SP-009 | zusätzlich 35#619, 645 |
| E-XX-019 | zusätzlich 35#534 |
| E-TA-014 | zusätzlich 35#751 |
| E-XX-002 | zusätzlich 35#574 (R16A Frage 10) |
| E-PA-025, E-LE-003, E-LE-011, E-NP-025, E-GE-004 | Hinweise aus VD12/VD15 ergänzt |

### 6.4 Gegenprobe mit den Vault-Dokumenten

Die Lock-Dokumente `VD15` (Runden 10, 11, 13, 14, 15) wurden gegen das Register geprüft. Ihre Aussagen finden sich im Register wieder (Initiative, Zustimmung in der laufenden Situation, Alkohol und Entscheidungsfähigkeit, Freeform-Engine, Contact Graph, Soft Tissue, Tattoos, Piercings, Nägel, Chirurgie, Preference-Framework, Gruppen, Branche, Creator, deterministische Videos, Items, Verträge, Hygiene, Verhütung, Zeit). Inhaltliche Abweichungen zwischen Vault und Register: **keine**. Die Vermerke aus 6.3 halten fest, wo die Vault-Dokumente eine Aussage zusätzlich stützen oder einschränken.

### 6.5 Zitate

Von 618 Zitaten in „…“ der Module 01–35 waren 240 nicht wörtlich in den Quellen auffindbar (überwiegend verdichtete oder sinngemäße Wiedergaben). Sie stehen seitdem in ‚…‘ (einfache Zeichen). „…“ bedeutet: wörtlich, Tippfehler und Zeichensetzung bereinigt.

## 7. Korrekturen an bestehenden Modulen

| Modul | Korrektur |
|---|---|
| 12 | Hinweis auf Nicht-Ausgewertetes und Stand der Qwen-/ZIP-Dateien aktualisiert |
| 13 | neue Quellenschlüssel `VD12`, `VD15`; Inventar Q8, Q9, S7; Lesestand 305–439 und frühe Fassungen; Kennzahlen exakt (1.903 Fragen); Abschnitt 7 neu; Zitat-Konvention; Archivzählung |
| 14 | 13 neue Einträge, 2 Statusänderungen, Quellenkorrekturen |
| 15 | Runde 2: die unbelegte Aussage ‚16 erklärt: ja‘ ersetzt durch den tatsächlichen Verlauf (Rückfragen 16, 23, 24 ohne ausdrückliche Antwort) |
| 17 | Hinweis auf `VD15` im Abschnitt ‚Nach dem Interview‘ |
| 19 | K23 neu; Belege zu K3, K5, K10, K15 ergänzt; Zählabweichung Runde 17 |
| 34 | OE-86 neu; OE-81, OE-84, OE-85 aktualisiert |
| 35 | frühe Fassungen des Rahmenentwurfs geprüft |
| 01–35 | Zitat-Konvention (240 Stellen) |
| README, Hub | neue Module 36–41, Zählungen |

## 8. Durchgeführte Prüfungen

(wird nach den Prüfläufen ergänzt)

## 9. Weiterhin offene Lücken

| Lücke | Stand |
|---|---|
| Runden **16–20**: keine konsolidierte Projektdatei | nur Chat 35 als Quelle |
| ZIP vom 20.09. mit `USER_REQUESTED_BUT_NOT_CANONICALIZED.md` | nicht auffindbar |
| Pakete „Adult Runden 1–20“ vom 27.09. | nicht auffindbar |
| Qwen-Architektur Abschnitte 30–43 und inhaltliche Qwen-Befragung | nie ausgeführt |
| Missing-Family-/Taxonomie-Audit (zweiter, kurzer Unterkategorien-Audit), Qwen-Audit | offen ([OE-80 bis OE-86](34-Offene-Entscheidungen-Katalog.md#8-audits-und-externe-läufe)) |
| Binärdateien des Masters (Bilder, Karten, Videos) | nicht ausgewertet |
| Vollständiger Zeilenvergleich der Chat-28-Fassungen | nicht durchgeführt |
| Frage-für-Frage-Zuordnung zu **Modulen** und **Register-Einträgen** | nur auf Paketebene bzw. bei namentlicher Nennung belegt |
| 28 Rückfragen ohne belegte Bestätigung (Abschnitt 5) | beim Entwickler zu klären |
| Heuristik der Matrix (Status, Abstraktion) | stichprobenhaft geprüft, nicht vollständig manuell verifiziert |
