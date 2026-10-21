# Adult 13: Quelleninventar und Abdeckungsmatrix

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Dokument inventarisiert die Planungsquellen des Adult-Systems und sagt **ehrlich**, was wie gründlich gelesen wurde. Es enthält keine Entscheidungen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Ergänzt die kurze Quellenübersicht in [Modul 12](12-Entscheidungsverlauf-und-Quellen.md). Das Entscheidungsregister steht in [Modul 14](14-Entscheidungsregister.md), die Rundenprotokolle in den Modulen [15](15-Interviewprotokoll-Runden-1-bis-10.md), [16](16-Interviewprotokoll-Runden-11-bis-16.md) und [17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md).

**Inhalt**

1. [Zitierschlüssel](#1-zitierschlüssel)
2. [Befund: Wo die Adult-Planung tatsächlich liegt](#2-befund-wo-die-adult-planung-tatsächlich-liegt)
3. [Quelleninventar](#3-quelleninventar)
4. [Abdeckung von Chat 35 nach Nachrichtenbereichen](#4-abdeckung-von-chat-35-nach-nachrichtenbereichen)
5. [Abdeckungsmatrix der Interviewrunden](#5-abdeckungsmatrix-der-interviewrunden)
6. [Kennzahlen](#6-kennzahlen)
7. [Nicht oder nur teilweise gelesen](#7-nicht-oder-nur-teilweise-gelesen)
8. [Bekannte Abweichungen innerhalb der Quellen](#8-bekannte-abweichungen-innerhalb-der-quellen)
9. [Prüfbarkeit](#9-prüfbarkeit)

---

## 1. Zitierschlüssel

Alle Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation bleibt ohne Zugriff darauf lesbar; die Schlüssel dienen nur der Nachprüfbarkeit.

| Schlüssel | Bedeutung | Datei |
|---|---|---|
| `35#n` | Chat ‚Projektwissen nutzen‘, Nachricht Nr. `n` (Nummer in der Überschrift `## n. user/assistant`) | `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/35_Projektwissen_nutzen_6a9cb579.md` |
| `28#n` | Chat „Adult Content Framework“ | `…/GESPRAECHE/28_Adult_Content_Framework__6a90d754.md` |
| `23#n` | Chat „SPN Full Master ZIP“ | `…/GESPRAECHE/23_SPN_Full_Master_ZIP_6a83a2dc.md` |
| `37#n` | Chat „Tagesplan“ | `…/GESPRAECHE/37_Tagesplan_6aa084b5.md` |
| `K§n` | Entscheidungsänderungen vom 31.08.2026, Abschnitt `n` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/DECISION_CHANGELOG_2026-08-31.md` |
| `PD` | Projektentscheidungen (Abschnitte „Interaction / optional PC adult content“, „Body / physical response / recoil“, „Gore / age presentation“) | `…/docs/reference/PROJECT_DECISIONS.md` |
| `PS` | Kompakter Projektstand vom 31.08.2026 (Abschnitte 15 bis 22) | `…/docs/reference/PROJECT_STATE_2026-08-31_COMPACT.md` |
| `OD` | Offene Entscheidungen (Abschnitt „Adult / romance / interaction“ und Modding) | `…/docs/reference/OPEN_DECISIONS.md` |
| `CA05` | Kanonischer Nachtrag 05.09.2026, Block 10 | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` |
| `T44` | Planungs-Backlog Track 44 (Verletzung/Gore/Alter) | `…/docs/planning-backlog/44-injury-gore-presentation-age-policy.md` |
| `VD12` | **Knowledge Vault v2 des Entwicklers**, Delta 10.–12.09.2026 (Interview-Snapshot bis Runde 9C, Qwen-Referenzausgaben, Qwen-Prompts, Richtungsdokumente). **Außerhalb des Master-Archivs**, in Etappe 2.2 nur lesend ausgewertet. | `<Vault>/_SOURCE_CURRENT/DELTA_2026-09-10_to_2026-09-12/SPN_DELTA_2026-09-10_to_2026-09-12/…` (Vault lokal unter `Projekte/Supernatural/SPN_Knowledge_Vault_v2`) |
| `VD15` | **Delta-Paket 13.–15.09.2026** (`COMPLETE` und `VAULT_MERGE`) mit konsolidierten Lock-Dokumenten zu den Runden 10, 11, 13, 14, 15. **Außerhalb des Master-Archivs.** | `SPN_DELTA_2026-09-13_to_2026-09-15_COMPLETE.zip`, lokal beim Entwickler (Ordner `Supernatural_3D_Game_CPP_Master`) |

Eine Quelle der Form `35#444` bezeichnet immer **die Nachricht**, in der die Aussage steht. Aussagen des Entwicklers stehen in den `user`-Nachrichten, Vorschläge und Rückfragen in den `assistant`-Nachrichten. Der Gesprächsassistent ist ein **früherer KI-Assistent**, nicht der Autor dieser Dokumentation.

---

## 2. Befund: Wo die Adult-Planung tatsächlich liegt

| Befund | Beleg |
|---|---|
| Im Master gibt es **kein Verzeichnis und keine Datei, die die Interviewrunden 1 bis 20 konsolidiert**. Die Suche nach Verzeichnissen mit „adult“ im Namen und nach den Stichworten der Rundenüberschriften (außerhalb der Gesprächsprotokolle) liefert nichts. | Suche am 21.10.2026 über `SPN_FULL_MASTER_2026-09-27` |
| Die **Detailplanung liegt ausschließlich im Gesprächsprotokoll** `35#185` bis `35#812`. | dieses Dokument, Abschnitt 4 |
| Die konsolidierten Statusdokumente (`MASTER_STATUS_2026-09-10` bis `2026-09-27`, `CURRENT_CANON_2026-09-16`) enthalten **keine Adult-Inhalte**. Der Master-Status vom 27.09. nennt ausdrücklich, dass Material **nicht eingebettet** wurde. | `01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-09-27.md` |
| Der Gesprächsassistent erklärte (`35#816`, `35#818`), das Paket vom 27.09. enthalte „Adult Runden 1–20“. Diese Dateien sind im vorliegenden Archiv **nicht auffindbar** (auch nicht in Vault, Delta-Ordner oder den kumulativen ZIPs 23.–26.09.). Das ZIP-Dokument `USER_REQUESTED_BUT_NOT_CANONICALIZED.md` (`35#662`) fehlt ebenfalls. | Suche nach Dateiname: ein einziger Treffer, nämlich die Erwähnung im Chat selbst |
| **Etappe 2.2:** Außerhalb des Master-Archivs liegen zwei projekteigene Konsolidierungen: der **Knowledge Vault** mit dem Delta 10.–12.09. (`VD12`) und das **Delta-Paket 13.–15.09.** (`VD15`, Lock-Dokumente zu den Runden 10, 11, 13, 14, 15). Beim Abgleich mit dem Register fand sich kein inhaltlicher Widerspruch; Ergänzungen und Statushinweise stehen im [Register](14-Entscheidungsregister.md) und in [Modul 41](41-Audit-Etappe-2-2.md). Für die **Runden 16 bis 20** wurde **keine** konsolidierte Datei gefunden. | Suche in Vault, Delta-Paketen, kumulativen Master-ZIPs 23.–26.09. und ChatGPT-Export |
| Das **konsolidierte Fundament** (31.08. bis 10.09.) liegt dagegen in Kanon-Dokumenten vor: `K§16` bis `K§21`, `PD`, `PS`, `OD`, `CA05`. | Abschnitt 3 |

**Folgerung:** Die Interviewantworten des Entwicklers sind die einzige Quelle für die Detailplanung. Sie sind laut Quellenregeln `CLAUDE.md` §2 **Belege einer Entwicklerentscheidung, aber nicht in den Kanon konsolidiert**. Vorschläge des Assistenten in denselben Gesprächen sind **Vorschläge**, solange der Entwickler nicht ausdrücklich zustimmt.

---

## 3. Quelleninventar

Spalte „Lesestand“: **vollständig** = alle Nachrichten des genannten Bereichs gelesen; **gezielt** = nur die genannten Nachrichten gelesen, der Rest per Stichwortsuche gesichtet; **Gliederung** = nur Überschriften und Nutzernachrichten; **nicht** = nicht gelesen.

### 3.1 Primärquellen für Entscheidungen

| Nr. | Quelle | Umfang | Rolle | Gewicht | Lesestand |
|---|---|---|---|---|---|
| Q1 | Chat 35 ‚Projektwissen nutzen‘ | 951 Nachrichten, 1,89 MB; Interview `35#185` bis `35#812` | Entwicklerinterview zu Runden 1 bis 20, Rechtsgespräch, Qwen-Vorphase | Entwicklerantworten: bestätigte Entwicklerplanung (nicht konsolidiert); Assistent: Vorschlag | siehe Abschnitt 4 |
| Q2 | `K§16` bis `K§21` (31.08.) | je 5 bis 12 Zeilen | Konsolidierte Entscheidungen zu Modding/Paket, Darstellung, Hard Stop, Dienstleistungs-NPCs, Körperzusammensetzung, Körperreaktions-Physik | **bestätigt** (Kanon-Kandidat) | vollständig |
| Q3 | `PD` (Abschnitte Interaction, Body, Gore) | 3 Abschnitte | Bestätigung der Punkte aus Q2; Gore/Alter | bestätigt | vollständig |
| Q4 | `PS` (kompakt, deutsch), Abschnitte 15 bis 22 | rund 80 Zeilen | Sicherheitskern (nicht moddbar), Consent-Regeln, Offizielle Version, optionaler PC-Mod, Reaktivität, Dienstleistungs-NPCs | bestätigt | vollständig |
| Q5 | `OD` Abschnitt „Adult / romance / interaction“ | 5 Zeilen | offene Entscheidungen zu Taxonomie, Präsentationsprofilen, Dienstleistungs-NPC-Umfang, Nacktheit-Benennung, Paketumfang | offen | vollständig |
| Q6 | `CA05` Block 10 | 1 Absatz | Beziehungs-/Romantik-Sandbox, Mehrfachbeziehungen | bestätigt | vollständig |
| Q7 | `T44` | 20 Zeilen | Verletzung/Gore/Alter (allgemein) | Planungsinventar | vollständig |

| Q8 | `VD12`: `ADULT_INTERVIEW_DECISIONS_SNAPSHOT_2026-09-12.md`, `QWEN_ADULT_ARCHITECTURE_REVIEW_STATUS_2026-09-12.md`, Richtungsdokumente (Körperreaktion, Population, C++-first/Persistenz), Modding-Blöcke 4 und 5 | 8 Dokumente, ca. 28 KB | Projekteigene Konsolidierung bis Runde 9C und frühe Runde 10 (Status „IN PROGRESS, NOT FINAL“); Richtungen „CURRENT DIRECTION“ bzw. „LOCKED“ | bestätigt/Richtung (Etappe 2.2) | vollständig |
| Q9 | `VD15`: `01_ADULT/R10…R15_*_LOCK_2026-09-15.md`, `MASTER_DELTA_SUMMARY`, `ADULT_ROUNDS_16_TO_19_NEXT`, `CONTENT_SETTING_SOCIAL_DIRECTION`, `CRIME_JUSTICE_LOGIC_DIRECTION`, Replay-/Internet-/Architektur-Dokumente | 19 Dokumente, ca. 33 KB | Projekteigene Konsolidierung der Runden 10, 11, 13, 14, 15 (Stand 15.09.) | bestätigt/Richtung (Etappe 2.2) | vollständig |

### 3.2 Sekundärquellen (Entwürfe, Vorarbeit)

| Nr. | Quelle | Umfang | Rolle | Gewicht | Lesestand |
|---|---|---|---|---|---|
| S1 | Chat 28 „Adult Content Framework“ (28.08.) | 25 Nachrichten, 362 KB | Vom früheren Assistenten geschriebener Architektur-Masterprompt (Endfassung 175 Abschnitte) mit Nutzerrückmeldungen | **Vorschlag**, teilweise überholt | Nutzernachrichten 1, 3, 5, 7, 10, 12, 14, 16, 18, 20, 22, 24 vollständig; Assistentennachrichten 17, 19, 21, 23, 25 (Endfassung) vollständig; Fassungen 2, 4, 6, 9, 11 in Etappe 2.2 auf Sicherheitsregeln und Gliederungsentwicklung geprüft |
| S2 | Chat 23 „SPN Full Master ZIP“ (24.08. bis 03.09.) | 1109 Nachrichten, 3,4 MB | Entstehung der Adult-Idee (27.08.), Konsole/PC-Trennung, Mod-Frage, Hard Stop, Dienstleistungs-NPCs, Nightlife | Vorschlag/Entwicklerwunsch | gezielt: Nachrichten 505 bis 592 und die Adult-Passagen der Nachrichten 628, 636, 638, 706, 720, 730, 787, 789, 1038, 1043, 1048, 1060; Etappe 2.2: Stichwortscan aller übrigen Nachrichten (84 mit Treffern), gelesen u. a. 86, 598, 627, 640–676, 882, 1035, 1037, 1074–1076, 1088–1090 |
| S3 | Chat 37 „Tagesplan“ (08.09. bis 05.10.) | 99 Nachrichten, 198 KB | Tagespläne; Entwicklerangaben zum Fortschritt („Adult fertig“, 28.09.) | Fortschrittsangaben | gezielt: Nachrichten 16 bis 22, 50 bis 55, 66, 68, 79, 82 bis 85; Etappe 2.2: Adult-Zeilen aller Tagespläne 53 bis 90 |
| S4 | Qwen-Vorphase in Chat 35 (`35#58` bis `35#184`, 09.–10.09.) | rund 130 Nachrichten | Modding-Blöcke 1 bis 5; Qwen-Masterprompt und Review der Adult-Architektur | Vorschlag (Rahmen, Korrekturvorgaben) | Adult-Nachrichten `35#63` bis `35#77`, `35#133`, `35#141`, `35#143` bis `35#184` gelesen; übrige Modding-Nachrichten per Stichwortscan |
| S5 | Master-Dokumente zu Welt und Nachtleben (`WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT.md`, `MASTER_PLANUNGSSTAND.md`) | Einzelzeilen | Nightlife als Weltschicht, Adult-Venues | bestätigt (Kanon-Kandidat) | Treffer gelesen |
| S6 | `OPTIONAL_CONTENT_PACKS_2026-09-10.md` | 25 Zeilen | Voice-/Texturpakete allgemein | bestätigt, nicht Adult-spezifisch | vollständig |
| S7 | `VD12`: `QWEN_V0_9_PARTIAL_SECTIONS_1_TO_28_REFERENCE.md` (59 KB), `QWEN_CONTINUATION_PARTIAL_REFERENCE.md` (31 KB), Qwen-Prompts `01`–`03` | 5 Dateien | Referenzausgaben eines lokalen Modells: Architekturentwurf V0.9 bis Abschnitt 28 (Kopf: „qwen3.8-27b-uncensored“) und eine abgebrochene Fortsetzung (Denkprotokoll zu den Abschnitten 28 bis 43) | **Referenzmaterial, nicht kanonisch** (laut `VD12`) | Gliederung vollständig, Abschnitte 1–2, 7–8 gelesen; übrige Abschnitte nur Überschriften |

### 3.3 Geprüft und ausgeschlossen

| Quelle | Befund | Entscheidung |
|---|---|---|
| Chat 40 ‚Nächste Schritte festlegen‘ | Treffer ‚Private/Adult Mode‘ betreffen ein **anderes Projekt** (Medien-Bibliothek, Szenen-Index). | nicht Teil des SPN-Adult-Systems |
| Chats 01, 05, 06, 08, 21, 26 | „adult“ im Zusammenhang mit Medienverwaltung/Tarifen. | nicht relevant |
| Chats 12, 15 | „adult“ in Romanvorgaben („erwachsene Figuren“). | nicht relevant |
| Chat 09 ‚Beste F95Zone Spiele‘ (07/2026) | Marktrecherche zu Spielen mit Erbrechens-Mechanik und einer Option ‚Vomiting deaktivieren‘. | nur Kontext; Beleg, dass Abschaltbarkeit solcher Darstellungen üblich ist |
| `99_DAILY_DELTAS/2026-10-14` bis `2026-10-19` | kein Treffer für „adult“ | nichts auszuwerten |
| `ADDENDA_2026/2026-10-18/03_ENGINEERING_PROPOSALS.md` | „consent“ im Zusammenhang mit Neuralschnittstellen | nicht relevant |
| `MASTER_STATUS_2026-09-22.md` | erwähnt fremde KI-Tattoo-Bilder, nicht Teil des Projekts | nicht relevant |
| Alle 56 Gespräche des Rekonstruktionspakets (Etappe 2.2) | Volltext-Treffer „adult“ außer 23, 28, 35, 37 nur in fremden Projekten (Medienverwaltung, Romanvorgaben, Tarifvergleich, Chat 22) | nicht relevant; Details werden nicht dokumentiert |
| ChatGPT-Gesamtexport (521 Gespräche) | Die SPN-Gespräche 23, 28, 35, 37 stimmen mit dem Rekonstruktionspaket überein (Nachrichtenzahlen: 35 = 948 Text-Nachrichten im Export gegenüber 951 im Paket, Differenz durch Platzhalter für Anhänge); weitere SPN-Gespräche (u. a. ‚Korrigieren‘, ‚Schreiben‘, ‚Planung, ZIP Downloads‘) ohne Adult-Treffer | nichts zusätzlich auszuwerten |
| Kumulative Master-ZIPs 23.–26.09. und Tages-Deltas Oktober im Ordner `SPN_VAULT_V2` | kein Adult-Dateiname, nur Wiederholungen derselben Kanon-Dokumente | nichts zusätzlich auszuwerten |
| `SPN_CANON_MERGE_2026-10-18.zip` (einzige ZIP im Master) | 12 Dateien, Addenda 18.10.; ‚consent‘ nur im Kontext von Neuralschnittstellen | nicht relevant |

---

## 4. Abdeckung von Chat 35 nach Nachrichtenbereichen

Chat 35 hat **951 Nachrichten** (Pfad 6. September bis 4. Oktober 2026). Die folgende Tabelle teilt **alle 951** auf.

| Bereich | Datum 2026 | Inhalt | Adult-relevant | Lesestand |
|---|---|---|---|---|
| 1 bis 57 | 06.–09.09. | Projektwissen, Voice-Produktion, Modding-Planung (Mod-Manager, Paketformat, Mod-Typen) | gering (Profil „Adult“ im Mod-Manager, `35#58`) | Stichwortscan (Etappe 2.2: alle Treffer gelesen), sonst nicht gelesen |
| 58 bis 62 | 09.09. | Modding-Block 1: Master-Dokument des Entwicklers | Content-Rating/Adult-Flag in Mod-Metadaten (`35#60`) | gezielt |
| 63 bis 78 | 09.09. | Modding-Blöcke 2 und 3; „Wann kommt Adult genauer?“; Adult-Überlegungen des Entwicklers (`35#71`) | ja | vollständig |
| 79 bis 142 | 09.–10.09. | Modding-Blöcke 3 bis 5 (Savegame, Security, Distribution), Tagespläne | gering (Content-Rating-Filter, `35#141`) | Stichwortscan (Etappe 2.2: alle Treffer gelesen), sonst nicht gelesen |
| 143 bis 184 | 10.09. | Block-5-Abschluss; **Qwen-Masterprompt, Review, Korrekturprompt, Interviewprompt** | ja | vollständig (Nachricht 146 zu rund 40 %, die Fortsetzung ist der Masterprompt-Text) |
| **185 bis 304** | 10.–12.09. | **Interview Runden 1 bis 10B**, Körper-Pipeline, Blueprints, Speicher | ja | **vollständig** |
| 305 bis 439 | 12.09. | Obsidian-Vault, Command-Center-Werkzeug (Fremdthema) | nein (Etappe 2.2: bestätigt) | **geprüft in Etappe 2.2:** alle 55 Entwicklernachrichten aufgelistet, alle Nachrichten per Stichwortscan mit Treffern gelesen; Treffer betreffen nur „Adult-Interview-Stand“ als Dateiname bzw. Zusammenfassungszeile und einen Dateipfad zu Qwen-Referenzausgaben (`35#366`). Keine Adult-Entscheidung. |
| **440 bis 561** | 13.–15.09. | **Runden 10 (Szene), 11, 11B, 12, 13, 14, 14B, 15A bis 15D**; Replay, Architektur-Abstecher | ja | **vollständig** |
| 562 bis 571 | 16.09. | Motorsport-Straßenstrecken (Fremdthema) | nein | Nachricht 562 und Beginn 563 gelesen, Rest nicht |
| **572 bis 812** | 16.–27.09. | **Runden 16 bis 20**, Pre-Audit, Rechtsgespräch, Qwen-Verschiebung, Tattoo-Runde | ja | **vollständig** |
| 813 bis 951 | 27.09.–04.10. | ZIP-/Master-Erstellung, Karten-Prototypen | Aussagen zum Adult-Abschluss (814, 816, 818, 872) | gezielt (diese vier); Rest Fremdthema, nicht gelesen |

**Gelesen:** 245 Nutzernachrichten (75,9 Tsd. Zeichen) und 248 Assistentennachrichten (730 Tsd. Zeichen) im Interviewbereich 185 bis 812 ohne die Fremdthemen-Bereiche (305 bis 439, 563 bis 571 großteils), dazu die oben genannten Vorphase- und Nachphase-Nachrichten. **Auch lange Einzelnachrichten wurden vollständig gelesen**, nicht nur die ersten Zeichen.

---

## 5. Abdeckungsmatrix der Interviewrunden

Die Rundennummern ändern sich im Gespräch (siehe [Abschnitt 8](#8-bekannte-abweichungen-innerhalb-der-quellen)). Die Tabelle nutzt die **Pakete so, wie der Assistent sie nannte**. „Fragen“ = nummerierte Fragen des Assistenten. „Antworten“ = Antwortnachrichten des Entwicklers; „delegiert“ heißt, der Entwickler überließ die Entscheidung dem Assistenten.

| Runde/Paket | Thema (Kurzfassung) | Fragennummern | Fragen | Nachrichten (Frage → Antwort) | Antwortstand |
|---|---|---|---:|---|---|
| R1 | Gesamtrichtung | 1 bis 12 | 12 | 186 → 187 | beantwortet |
| R2 | Content-Struktur, Hardcore-System | 13 bis 24 | 12 | 188 → 189, 191, 195, 197, 199, 201, 203, 205, 207 | beantwortet, Zustimmungsdiskussion |
| R3 | Kategorienstruktur | 1 bis 12 | 12 | 220 → 221, 223 | beantwortet |
| R4 | Präferenzen, NPC-Individualität | 1 bis 15 | 15 | 226 → 227 | beantwortet |
| R5 | Beziehung, Anziehung, One-Night-Stand | 1 bis 15 | 15 | 228 → 229 | beantwortet |
| R6 | Initiative, Flirt, spontane Begegnung | 1 bis 15 | 15 | 230 → 231 | beantwortet |
| R7 | Orte, Öffentlichkeit, Beobachter | 1 bis 15 | 15 | 232 → 233, 235 | beantwortet |
| (Alter) | Altersgrenze 18+, Verifikation | – | – | 237 bis 244 | Dialog |
| R8 | Pakete, Modularität | 1 bis 15 | 15 | 246 → 247, 249 | beantwortet, teils „weiß noch nicht“ |
| R9A | NPC-Generierung, Präferenz-DNA | 1 bis 45 | 45 | 260 → 261 | beantwortet |
| R9B.1 | Langzeitveränderung, Gedächtnis | 1 bis 30 | 30 | 268 → 269 | beantwortet |
| R9B.2 | Präferenzlernen, Dates, ONS, Eifersucht, Trennung, Gossip | 1 bis 60 | 60 | 270 → 271 | beantwortet |
| R9B.3 | Alter, Tod, Weltzustand | 1 bis 50 | 50 | 272 → 273 | beantwortet („alles passt“) |
| R9C | 500.000 NPCs, Simulation-LOD, Saves | 1 bis 40 | 40 | 278 → 279 | beantwortet |
| R10A | Körper, Aussehen | 1 bis 35 | 35 | 280 → 281 | beantwortet |
| R10B | Rigging, Asymmetrie, IK | 36 bis 70 | 35 | 282 → 283 | beantwortet |
| HFBR | High-Fidelity-Körperreaktion | 1 bis 40 | 40 | 288, 290 → 289, 291 | beantwortet |
| R10 | Szene und Interaktionssystem | 1 bis 100 | 100 | 443 → 444, 446, 448 bis 454 | beantwortet, Zustimmungsdiskussion |
| R11 | Freeform-Engine | 1 bis 100 | 100 | 457 → 458 | beantwortet |
| R11B | Solver, Physik, Steuerung, VR | 101 bis 135 | 35 | 463 → 464 | beantwortet |
| R12 | Replay, Cinematic, Streaming | 1 bis 100 | 100 | 483 → 484 | 1 bis 20 beantwortet; **21 bis 100 delegiert** |
| R13 | Ablauf, Reaktionen | 1 bis 100 | 100 | 485 → 486 | beantwortet |
| R14A | Körper, Anatomie, Tattoos | 1 bis 100 | 100 | 499 → 500, 502 | beantwortet |
| R14B | Piercings, Körpermodifikation | 101 bis 140 | 40 | 511 → 512 | beantwortet |
| R15A bis R15D | Präferenzen, Gruppen, Branche, Orte, Items | 1 bis 100 | 100 | 523 bis 531 → 524, 526, 528, 530, 532 | beantwortet |
| R15E | Solo, Berufe, Medien | 101 bis 125 | 25 | 539 → 540 | beantwortet |
| R15F | Gesundheit | 126 bis 150 | 25 | 541 → 542 | beantwortet (137 bis 139 abgelehnt) |
| R15G | Orientierung, Orgasmus-Dynamik | 151 bis 175 | 25 | 549 → 550 | beantwortet |
| R15H | Paardynamik, Branche, Verträge | 176 bis 200 | 25 | 551 → 552 | beantwortet |
| R16A bis R16C | Master-Taxonomie, Kategorien, Positionen | 1 bis 75 | 75 | 573 bis 591 | beantwortet |
| R16D bis R16H | Intensität, Reaktionen, Gewebe, NPC, UI/Technik | 76 bis 200 | 125 | 609 bis 628 | beantwortet |
| R16I bis R16L | Registry, Unterarten, Reaktionen, Outcome-Schicht | 201 bis 300 | 100 | 642 bis 680 | beantwortet |
| R16M bis R16P | Registry II, Fluide, Orte, kombinierte Aktionen | 301 bis 400 | 100 | 678 bis 696 | beantwortet |
| R16Q, R16R | Risiko/Gesundheit; Abschlussentscheidungen | 401 bis 437 | 37 | 706 bis 715 | beantwortet |
| Pre-Audit | Missing-Family-Audit I | A1 bis A15 | 15 | 742 → 743, 745 | beantwortet, vom Entwickler als ‚nicht der offizielle Audit‘ bezeichnet |
| R17A bis R17I | Tattoos, Piercings, Körpermodifikation | 1 bis 141 | 141 | 748 bis 786 | beantwortet |
| R18A bis R18C | Steuerung, Oberfläche, VR | 1 bis 42 | 42 | 790 bis 802 | beantwortet (1, 2, 4, 6 zunächst offen, dann Hybrid) |
| R19A, R19B | Technische Integration | 1 bis 32 | 32 | 804 bis 808 | beantwortet |
| R20A | Produktionssperre | 1 bis 15 | 15 | 810 → 811 | beantwortet |

Detailprotokolle mit den einzelnen Antworten: [Modul 15](15-Interviewprotokoll-Runden-1-bis-10.md), [Modul 16](16-Interviewprotokoll-Runden-11-bis-16.md), [Modul 17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md).

---

## 6. Kennzahlen

| Kennzahl | Wert | Hinweis |
|---|---:|---|
| Nummerierte Fragen des Assistenten in 66 Fragenpaketen | **1.903** (exakt, Etappe 2.2) | R15 enthält 200 Fragen (nicht 175, siehe Abschnitt 8); Aufstellung: [Modul 36 bis 40](41-Audit-Etappe-2-2.md) |
| Davon ohne Entwicklerantwort („delegiert“) | 80 | R12 Fragen 21 bis 100 |
| Davon vom Entwickler mit „nein“ beantwortet | 7 | R15F 137 bis 139 (Infektionssimulation, 3), R1-7, R5-14, R9B.2-21, Pre-Audit A9 |
| Davon mit „weiß ich nicht“, „was meinst du“ oder Rückfrage | 28 ohne belegte Bestätigung, 9 danach pauschal bestätigt | heuristisch gezählt; Liste in [Modul 41](41-Audit-Etappe-2-2.md#5-rückfragen-ohne-belegte-bestätigung) |
| Entwickler-Nachrichten im gelesenen Interviewbereich | 245 | Anteil mit Zusatzaussagen außerhalb der Frageliste nicht gezählt |
| Entscheidungsregister-Einträge | siehe [Modul 14](14-Entscheidungsregister.md) | |
| Nachrichten mit Entwickleraussagen, die dem Sicherheitskern widersprechen | 16 in K1 und K2 aufgeführt (weitere in K3, K4 und im Register, Abschnitt XX; nicht abschließend gezählt) | [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md) |

Die Zahl 1.903 ist eine **maschinelle Zählung der nummerierten Fragen** aus den Fragenpaketen (Etappe 2.2); sie ersetzt die frühere Angabe „rund 1.900“ und die noch frühere Schätzung von rund 1.000 in Modul 12.

---

## 7. Nicht oder nur teilweise gelesen

Stand nach **Etappe 2.2**.

| Bereich | Grund | Auswirkung |
|---|---|---|
| Chat 35, Nachrichten 1 bis 57 und 79 bis 142 | Modding-Planung, Fremdthema | Alle Stichwort-Treffer gelesen (u. a. Content-Rating-Filter `35#141`); das Modding selbst ist **nicht** Teil dieser Dokumentation. |
| Chat 35, Nachrichten 563 bis 571 | Motorsport | keine Adult-Planung (Stichwortscan ohne Treffer) |
| Chat 35, Nachrichten 813 bis 951 (außer 814, 816, 818, 872) | ZIP-Erstellung, Kartenprototypen | keine Adult-Planung (Stichwortscan: nur diese vier Nachrichten) |
| Chat 28, Fassungen 2, 4, 6, 9, 11, 15, 17, 21 | Überarbeitungsstufen derselben Vorlage | Sicherheitsregeln und Gliederungsentwicklung geprüft ([Modul 35](35-Auswertung-Rahmenentwurf-28-08.md)); Abschnitte nicht Zeile für Zeile verglichen. |
| Chat 23, außerhalb der in Tabelle 3.2 genannten Bereiche | Fremdthemen | Stichwortscan; gelesen sind nur die in 3.2 genannten Nachrichten |
| Chat 37, außerhalb der genannten Nachrichten | Tagespläne | Adult-Zeilen der Tagespläne 53–90 gelesen; Rest nicht |
| Das restliche Archiv (2,9 GB; 4.864 Dateien, davon 4.576 mit Textendungen) | Volltextsuche (Etappe 2.2) in allen Dateien mit Endung md, json, txt, csv, yaml, yml, html nach „adult“, „erwachsenen“, „sexual“, „nsfw“ (außerhalb von `_SOURCE_ARCHIVE`, `_SOURCE_RESEARCH`, `02_WORLD_700` zusätzlich „consent“, „zustimmung“); die drei großen Unterordner ergaben nur `CURRENT_SUPERSESSION_2026-09-03.md` (Kopien) | Binärdateien (png, tif, gpkg, mp4 …), Karten und Bilder: nicht ausgewertet. Früher genannte Zahl 4.562 war eine ungenauere Zählung. |
| `VD12` (Vault, Delta 10.–12.09.) | Qwen-Referenzausgaben | Gliederung vollständig, aber nur Abschnitte 1–2 und 7–8 des V0.9-Entwurfs gelesen; Fortsetzung ist ein Denkprotokoll |
| Qwen-Läufe | Ergebnisse existieren nur als **Teilausgaben bis Abschnitt 28 bzw. ca. 30** im Vault (`VD12`), nicht im Master-Archiv | Kein Qwen-Inhalt wird als Entscheidung verwendet; Referenz für die Architektur-Review ([E-SP-010](14-Entscheidungsregister.md#14-sp-speicherstand-modding-builds-technik)). Das **vollständige** Architekturdokument (Abschnitte 30–43) und die **inhaltliche Qwen-Befragung** wurden nie ausgeführt. |
| Pakete „Adult Runden 1–20“ des 27.09., Master-ZIP 20.09. mit `USER_REQUESTED_BUT_NOT_CANONICALIZED.md` | Nur im Chat beschrieben | **Nicht auffindbar** (Master, Vault, Delta-Ordner, kumulative ZIPs 23.–26.09., ChatGPT-Export geprüft). Delta 13.–15.09. wurde gefunden und gelesen (`VD15`). |

**Eine vollständige Auswertung des gesamten Archivs wird nicht behauptet.** Für die Adult-Planung gilt: Alle Nachrichten, in denen das Interview oder seine Vorarbeiten stattfinden, sind gelesen; Fremdthemen-Nachrichten wurden per Stichwortscan gesichtet und alle Treffer gelesen.

---

## 8. Bekannte Abweichungen innerhalb der Quellen

| Nr. | Abweichung | Belege | Behandlung |
|---|---|---|---|
| Q-A1 | **Rundenzahl:** zuerst 16 bis 18 (`35#252`, `35#443`), dann 19 (`35#491`, `35#493`), dann 20 (`35#591`). Der Assistent korrigierte sich mehrfach (`35#447`, `35#491`). | Chronologie in [Modul 18](18-Chronologie-Detail.md) | Die Dokumentation nutzt die Paketbezeichnungen (R16A usw.) und nennt die Rundenzahl nur im Verlauf. |
| Q-A2 | **R15 hat 200 Fragen**, der Entwickler und der Tagesplan-Chat sprechen von **175** (`35#608`, `37#54`, `37#55`). Das 15H-Paket (Fragen 176 bis 200) wurde am 15.09. um 21:11 beantwortet (`35#552`). Auch die projekteigene Konsolidierung vom 15.09. (`VD15`, R15-Lock) führt Runde 15 als „1–200“. | `35#549`, `35#551`, `35#552`, `35#553` | beide Angaben dokumentiert; die Fragenliste gilt als maßgeblich (200). Konflikt K10 in [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md). |
| Q-A3 | **Zählung von R16:** Der Entwickler-Prompt vom 24.09. beschreibt 16P (376 bis 400) als ‚noch nicht beantwortet‘ (`35#705`), obwohl der Entwickler am 23.09. „376-400 ja“ geschrieben hatte (`35#695`). Der Assistent stellte das am 24.09. richtig (`35#706`). | `35#695`, `35#705`, `35#706` | 16P gilt als beantwortet. Konflikt K11. |
| Q-A4 | **Zählung der Pre-Audit-Antworten:** Der Entwickler antwortete auf die Fragen A1 bis A15 (`35#743`) und bemerkte danach, er habe geglaubt, das Audit komme erst mit dem externen Modell (`35#743`, `35#744`). Der Assistent erklärte A1 bis A15 zu „Pre-Audit-Notizen“. | `35#743` bis `35#746` | Die Antworten gelten als Meinungsäußerung, nicht als Abschluss des Audits. |
| Q-A5 | **Planungsstand „Adult fertig“:** `35#812` (27.09.): Runden 1 bis 20 „im Wesentlichen abgeschlossen“, Audit und externer Lauf ausstehend. `37#83` (28.09., Entwickler): „Bro ich habe adult fertig“. `37#84` leitet daraus „komplett fertig“ ab. | `35#812`, `37#83`, `37#84` | In dieser Dokumentation gilt der konkrete Stand aus `35#812`: Audit und externer Lauf stehen aus. |
| Q-A6 | **Mehrfach vorhandene Fassungen:** `PS` (deutsch, kompakt) und `K§` (englisch) enthalten dieselben Punkte in verschiedener Formulierung. | `PS` 15 bis 22, `K§16` bis `K§21` | Beide stimmen sachlich überein; es wird nach `K§` zitiert. |

---

## 9. Prüfbarkeit

Wer eine Aussage in dieser Dokumentation nachprüfen möchte:

1. Zitierschlüssel in [Abschnitt 1](#1-zitierschlüssel) auflösen und im Chat nach der Überschrift `## n. user` oder `## n. assistant` suchen.
2. Bei Entwicklerantworten in Kurzform (`1-8 ja, 9 realistisch…`) die **vorangehende Assistentennachricht** `n−1` lesen: Dort stehen die Fragen mit denselben Nummern.
3. Bei Kanon-Dokumenten den Abschnitt `K§n` direkt lesen; die Datei ist kurz.

**Anführungszeichen-Konvention (seit Etappe 2.2):** Doppelte Zeichen „…“ geben die Quelle **wörtlich** wieder (Groß-/Kleinschreibung, Zeichensetzung und offensichtliche Tippfehler bereinigt; maschinell gegen die Quellen geprüft). Einfache Zeichen ‚…‘ markieren **sinngemäße, gekürzte oder zusammengezogene** Wiedergabe oder Bezeichnungen. Hat der Entwickler eine Frage nicht beantwortet oder nur mit Nachfrage („was meinst du“, „weiß ich nicht“), ist das im Rundenprotokoll vermerkt.
