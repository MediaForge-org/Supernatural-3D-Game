# Nachweis: Auswertung der Tagesdeltas 2026-10-14 bis 2026-10-19

## 1. Kopf

- **Stand:** 08.11.2026
- **Bearbeiter:** Claude (KI-Agent, Subagent) im Auftrag des Entwicklers. Keine Entscheidung wurde getroffen; alle Klassifikationen sind Vorschläge zur Prüfung.
- **Quelle (nur lesend):** `/mnt/Festplatte/SPN_FULL_MASTER_2026-09-27/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/99_DAILY_DELTAS/`
- **Basis je Datei:** `<Datum>/SPN_DAILY_DELTA_<Datum>/`, Tabellen geben den Pfad ab dieser Basis an.
- **Umfang:** 6 Tagesordner, 102 Dateien insgesamt. Davon 71 Markdown-Dateien (Auftrag: 71; bestätigt) und 31 andere Dateien (Installer, Manifeste, Prüfskripte, JSON-Ledger, ein ZIP).
- **Gelesen:** alle Markdown-Dateien unter `docs/`, alle CHANGELOG-, README- und Ledger-JSON-Dateien, `assets/prior_canon_refs/` (Kanon-Kopien), `data/PACKAGE_INPUT_HASHES.md`.
- **Nicht inhaltlich ausgewertet:** `install.sh`, `manifest.json`, `SHA256SUMS`, `tests/*.py`, `tools/safe_install.py` (nur Name und Größe), `assets/source_archives/SPN_CANON_MERGE_2026-10-18.zip` (Binärarchiv, nicht geöffnet).
- **Abgleich:** `Docs/Planning/DECISIONS.md`, `CANON.md`, `CONFLICTS.md` sowie `Docs/Game/` (Stichproben per Suche, nicht vollständig gelesen).
- **Vorrang beachtet:** N-01 (Größe der Hauptwelt offen), N-02 (vollständige Untergrundzivilisation entfällt), K-20 (2.200 km² überholt), K-25 (Host-Persistenz bestätigt). Offen bleiben K-19, K-22, K-23, K-24, K-26, K-27, K-28.
- **Nicht getan:** keine Änderung an `Docs/Game/`, am Master oder an anderen Projektdateien. Keine Git-Befehle. Keine Installer-Skripte ausgeführt.

## 2. Dateiinventar und Klassifikation

Klassen: **Canon** = Entscheidungen des Entwicklers im Review-Delta (Delta selbst als REVIEW REQUIRED gekennzeichnet); **Spezifikation-Vorschlag** = Anforderungen mit Vorschlagsanteilen; **Bericht** = berichtete Ergebnisse, Vergleiche, Recherche; **Prompt** = keine Datei dieser Klasse vorhanden; **Sonstiges** = Paketbeschreibung, Fragenliste, Herkunft, Anleitung, Asset-Register.

| Nr | Datum | Datei (relativ zur Basis) | Bytes | Klasse | Anmerkung |
|---:|---|---|---:|---|---|
| 1 | 10-14 | assets/README.md | 601 | Sonstiges | Keine Binärassets |
| 2 | 10-14 | CHANGELOG.md | 2880 | Canon | Familienschlüssel-Ursprung offen; Bevölkerung vorläufig |
| 3 | 10-14 | docs/canon/HUNTER_BUNKER_AND_FAMILY_KEY.md | 2105 | Canon | Erzählrahmen; Vorschlags-Variablen nicht bestätigt |
| 4 | 10-14 | docs/canon/UNDERGROUND_CITIES_DECISIONS.md | 2906 | Canon | Untergrundzivilisation durch N-02 überholt |
| 5 | 10-14 | docs/comparison/PRIOR_DOCUMENTED_CANON_COMPARISON.md | 3076 | Bericht | Vergleich mit älteren Dokumenten |
| 6 | 10-14 | docs/design/POPULATION_AND_SPATIAL_SCENARIOS.md | 2782 | Spezifikation-Vorschlag | Rechenbeispiele, keine Werte |
| 7 | 10-14 | docs/design/UNDERGROUND_NETWORK_AND_SECURITY.md | 1953 | Spezifikation-Vorschlag | Vorschläge, nicht genehmigt |
| 8 | 10-14 | docs/open_questions/OPEN_QUESTIONS_2026-10-14.md | 2077 | Sonstiges | 13 offene Punkte |
| 9 | 10-14 | docs/provenance/SCOPE_AND_EVIDENCE.md | 2093 | Sonstiges | Herkunft, Grenzen |
| 10 | 10-14 | README.md | 2584 | Sonstiges | Status REVIEW REQUIRED |
| 11 | 10-15 | assets/ASSET_REGISTER.md | 1119 | Sonstiges | Keine neuen Assets |
| 12 | 10-15 | CHANGELOG.md | 4951 | Canon | U24–U32, Rail R01–R05, Fahrzeug V01–V05 |
| 13 | 10-15 | docs/canon/UNDERGROUND_SOCIETY_DECISIONS_24-32.md | 3292 | Canon | U24–U32; Konflikt mit N-02 (WD-01) |
| 14 | 10-15 | docs/comparison/LIMITED_PRIOR_CANON_DIFF.md | 3549 | Bericht | Begrenzter Vergleich |
| 15 | 10-15 | docs/open_questions/OPEN_QUESTIONS_2026-10-15.md | 2253 | Sonstiges | Frage 33 (KI) offen, Schaden offen |
| 16 | 10-15 | docs/provenance/SCOPE_AND_EVIDENCE.md | 2365 | Sonstiges | Herkunft |
| 17 | 10-15 | docs/specs/RAILWAY_SYSTEM_2026-10-15.md | 2672 | Spezifikation-Vorschlag | Mengenbereiche sind Entwicklerangaben; Technik ist Vorschlag |
| 18 | 10-15 | docs/specs/ROAD_VEHICLES_2026-10-15.md | 1933 | Spezifikation-Vorschlag | Steuerung bestätigt; Schadensmodell offen |
| 19 | 10-15 | README.md | 1904 | Sonstiges | Paketbeschreibung |
| 20 | 10-16 | assets/ASSET_REGISTER.md | 970 | Sonstiges | Keine neuen Assets |
| 21 | 10-16 | CHANGELOG.md | 3633 | Canon | U33, U34 bestätigt; U35 offen |
| 22 | 10-16 | docs/canon/AI_DECISIONS_33_34.md | 2239 | Canon | U33 Option D, U34 Option D |
| 23 | 10-16 | docs/comparison/DIFF_AGAINST_PRIOR_DELTAS.md | 2313 | Bericht | Vergleich |
| 24 | 10-16 | docs/integration/MANUAL_REVIEW_GUIDE.md | 1170 | Sonstiges | Prüfablauf |
| 25 | 10-16 | docs/open_questions/OPEN_QUESTIONS_2026-10-16.md | 2300 | Sonstiges | U35 offen; Freitext-Regel (DL-001) |
| 26 | 10-16 | docs/provenance/SCOPE_AND_EVIDENCE.md | 2178 | Sonstiges | Herkunft |
| 27 | 10-16 | docs/specs/AI_RECOGNITION_AND_CONFLICTS.md | 2013 | Spezifikation-Vorschlag | Ausdrücklich unverbindlich |
| 28 | 10-16 | docs/specs/AI_SIMULATION_BOUNDARIES.md | 3026 | Spezifikation-Vorschlag | Ingenieursmodell, nicht Kanon |
| 29 | 10-16 | README.md | 2214 | Sonstiges | Paketbeschreibung |
| 30 | 10-17 | assets/ASSET_REGISTER.md | 1091 | Sonstiges | Keine neuen Assets |
| 31 | 10-17 | CHANGELOG.md | 3821 | Canon | U35 Option D, U36 Option E bestätigt; U37 offen |
| 32 | 10-17 | docs/canon/UNDERGROUND_AI_AND_COMMUNICATION_U35_U36.md | 2832 | Canon | Bindende Richtung U35/U36 |
| 33 | 10-17 | docs/comparison/DIFF_AGAINST_2026-10-14_TO_16.md | 2166 | Bericht | Vergleich |
| 34 | 10-17 | docs/design/PROPOSED_CPLUSPLUS_IMPLEMENTATION_NOTES.md | 2217 | Spezifikation-Vorschlag | Ausdrücklich NON-CANON |
| 35 | 10-17 | docs/integration/MERGE_REVIEW.md | 1392 | Sonstiges | Prüfschritte |
| 36 | 10-17 | docs/open_questions/OPEN_QUESTIONS_2026-10-17.md | 2550 | Sonstiges | U37 nächste Frage |
| 37 | 10-17 | docs/provenance/SCOPE_AND_EVIDENCE.md | 2227 | Sonstiges | Herkunft |
| 38 | 10-17 | docs/specs/AI_AUTONOMY_BOUNDARIES.md | 3686 | Spezifikation-Vorschlag | Mechanismen nicht genehmigt |
| 39 | 10-17 | docs/specs/COMMUNICATION_MODALITIES.md | 2828 | Spezifikation-Vorschlag | Netzwerk offen (U37) |
| 40 | 10-17 | docs/specs/PRIVACY_SECURITY_AND_FAILURE_REQUIREMENTS.md | 2201 | Spezifikation-Vorschlag | Vorschläge, nicht ratifiziert |
| 41 | 10-17 | README.md | 1814 | Sonstiges | Paketbeschreibung |
| 42 | 10-17 | tests/README.md | 726 | Sonstiges | Testhinweise |
| 43 | 10-18 | assets/ASSET_REGISTER.md | 1204 | Sonstiges | Keine Kartenbinärdaten |
| 44 | 10-18 | assets/prior_canon_refs/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md | 3787 | Canon | Kopie (Referenz), nicht Live-Kanon |
| 45 | 10-18 | assets/prior_canon_refs/01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/02_DECISION_REGISTER.md | 1846 | Canon | Kopie (Referenz) |
| 46 | 10-18 | assets/prior_canon_refs/01_CURRENT_CANONICAL_STATE/CURRENT_CANON_2026-09-16.md | 1432 | Canon | Kopie (Referenz) |
| 47 | 10-18 | assets/prior_canon_refs/01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md | 3349 | Canon | Kopie (Referenz) |
| 48 | 10-18 | assets/prior_canon_refs/01_CURRENT_CANONICAL_STATE/SOURCE_OF_TRUTH_AND_SUPERSESSION_2026-09-27.md | 759 | Canon | Kopie (Referenz) |
| 49 | 10-18 | CHANGELOG.md | 3401 | Canon | Karten-Richtung, Metropolis 110 km², Projektpolitik |
| 50 | 10-18 | docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md | 4245 | Canon | Kartenanforderungen für ~2.200-km²-Kandidaten |
| 51 | 10-18 | docs/comparison/PRIOR_CANON_DIFF.md | 2955 | Bericht | Vergleich |
| 52 | 10-18 | docs/design/CHARACTER_CREATOR_AND_RELEASE_POLICY.md | 1867 | Canon | Langfristige Richtung (Fotorealismus, Editor); Rest Vorschlag |
| 53 | 10-18 | docs/open_questions/OPEN_QUESTIONS_2026-10-18.md | 2357 | Sonstiges | Kartenabnahme offen |
| 54 | 10-18 | docs/provenance/SCOPE_AND_EVIDENCE.md | 3504 | Sonstiges | Herkunft, Hashes |
| 55 | 10-18 | docs/specs/MASTER_MAP_ACCEPTANCE_SPEC.md | 3133 | Spezifikation-Vorschlag | Anforderungen des Entwicklers, Prüfmethode Vorschlag |
| 56 | 10-18 | docs/status/CPP_PHASE_0_PROGRESS.md | 2003 | Bericht | Phase 0.1/0.2 berichtet (45/45), nicht geprüft |
| 57 | 10-18 | docs/status/MAP_ITERATION_REVIEW.md | 1958 | Bericht | Kartenstatus V3–V6, keine Abnahme |
| 58 | 10-18 | README.md | 2354 | Sonstiges | Paketbeschreibung |
| 59 | 10-19 | assets/ASSET_REGISTER.md | 2965 | Sonstiges | Kartenbilder nicht enthalten; Hunter-HQ-Label sichtbar |
| 60 | 10-19 | CHANGELOG.md | 3638 | Canon | Flächenbilanz, Kartenanforderungen (provisorisch) |
| 61 | 10-19 | data/PACKAGE_INPUT_HASHES.md | 508 | Sonstiges | Eingabe-Hashes |
| 62 | 10-19 | docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md | 3343 | Canon | Provisorische 2.200-km²-Vorgabe; keine Abnahme |
| 63 | 10-19 | docs/comparison/DELTA_VS_PRIOR_CANON_2026-10-19.md | 2966 | Bericht | Vergleich |
| 64 | 10-19 | docs/design/AI_3D_PIPELINE_WATCH_2026-10-19.md | 2165 | Bericht | Herstellerangaben (Recherche), keine Entscheidung |
| 65 | 10-19 | docs/design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md | 1624 | Spezifikation-Vorschlag | Phase 0.4 als Vorschlag (Widerspruch WD-04) |
| 66 | 10-19 | docs/open_questions/OPEN_QUESTIONS_2026-10-19.md | 2088 | Sonstiges | Kartenfragen, C++-Fragen |
| 67 | 10-19 | docs/provenance/SCOPE_AND_EVIDENCE.md | 2414 | Sonstiges | Herkunft |
| 68 | 10-19 | docs/specs/MAP_ACCEPTANCE_CHECKS_2026-10-19.md | 2769 | Spezifikation-Vorschlag | Messbare Prüfliste, Vorschlag |
| 69 | 10-19 | docs/specs/WORLDFACTS_PHASE_0_3_2026-10-19.md | 3199 | Bericht | Phase 0.3 berichtet (62/62), nicht geprüft |
| 70 | 10-19 | docs/status/CPP_GIT_STATUS_2026-10-19.md | 1775 | Bericht | Git-Stand berichtet (WD-05) |
| 71 | 10-19 | README.md | 2218 | Sonstiges | Paketbeschreibung |

**Nicht-Markdown-Dateien (31, nur Name und Größe):**

| Datum | Dateien (Bytes) | Klasse |
|---|---|---|
| 10-14 | install.sh (4391), manifest.json (2730), SHA256SUMS (1189), tests/verify_package.py (1104) | Sonstiges |
| 10-15 | install.sh (4014), manifest.json (2202), SHA256SUMS (1069), tests/verify_package.py (1620) | Sonstiges |
| 10-16 | data/decision_ledger.json (1926), install.sh (4660), manifest.json (2607), SHA256SUMS (1254), tests/verify_package.py (1615) | Sonstiges (Ledger: Entscheidungsnachweis, s. Anmerkung) |
| 10-17 | data/decision_ledger.json (1712), install.sh (4638), manifest.json (3212), SHA256SUMS (1592), tests/verify_package.py (1763) | Sonstiges |
| 10-18 | data/decision_ledger.json (4067), assets/source_archives/SPN_CANON_MERGE_2026-10-18.zip (10789, nicht geöffnet), install.sh (148), manifest.json (5587), SHA256SUMS (2435), tests/verify_package.py (204), tools/safe_install.py (6410) | Sonstiges |
| 10-19 | data/decision_ledger.json (2285), install.sh (123), manifest.json (3318), SHA256SUMS (1784), tests/verify_package.py (248), tools/safe_install.py (6410) | Sonstiges |

**Klassen-Zusammenfassung (Markdown, 71):** Canon 19 · Spezifikation-Vorschlag 13 · Bericht 11 · Prompt 0 · Sonstiges 28.
Die Ledger-JSON-Dateien dienen nur als strukturierte Kopie der Textentscheidungen; ihre Inhalte wurden mit den Markdown-Quellen abgeglichen.

## 3. Bereits erfasst (Abgleich Delta gegen Repo)

Die meisten bestätigten Delta-Punkte sind im Repo bereits erfasst. Der Abgleich zeigt, wo sie stehen. "Erfasst" heißt nicht, dass die Quelle geprüft oder neu bestätigt wurde.

| Delta-Punkt | Repo-Stand | Fundstelle im Repo |
|---|---|---|
| Familienschlüssel-Ursprung reserviert (10-14) | erfasst | `Docs/Game/World/20-Entscheidungsregister.md:266` (W-UG-016); `Docs/Planning/DECISIONS.md` C-44 |
| Bevölkerung Untergrundstadt vorläufig (10-14) | erfasst, durch N-02 historisch | `World/20…:259` (W-UG-009); `Docs/Game/Entscheidungsstatus.md:102` |
| U24–U32 (10-15) | erfasst, Konflikt offen | `Docs/Game/Entscheidungsstatus.md:90`; `Docs/Game/Spielwelt-und-Untergrund.md:362–366`; siehe WD-01 |
| U33, U34 Option D (10-16) | erfasst | `World/20…:296` (W-GS-013); `Spielwelt…:364` |
| U35 Option D (10-17) | erfasst | `World/20…:297` (W-GS-014); `Spielwelt…:365` |
| U36 Option E (10-17) | erfasst | `World/20…:298` (W-GS-015); `Spielwelt…:366` |
| U37 offen (10-17) | erfasst | `World/20…:299` (W-GS-016) |
| Rail R01–R05 (10-15) | erfasst | `Spielwelt…:290–299`; `Entscheidungsstatus.md` §2.3 "Rail" |
| Fahrzeug V01–V05, DualSense (10-15) | erfasst | `Docs/Game/Gameplay-Systeme.md:158, 166–167` |
| Schadensmodell offen (10-15, V06) | erfasst | `Docs/Game/Gameplay-Systeme.md:184` |
| 2.200 km² trockenes Festland + Ostinsel 80–100 km² (10-18/19) | erfasst, durch N-01 überholt | `Spielwelt…:93`; `Entscheidungsstatus.md:100` (Update ausstehend, WD-02) |
| Metropolis 110 km², 20 Bezirke, 20/12 km² (10-18) | erfasst als Arbeitsmaß | `Entscheidungsstatus.md` §3 (WD-03) |
| Kartenstatus CANDIDATE / VISUAL_NOT_ACCEPTED (10-18) | erfasst | `World/20…:93, 100` |
| Kein Hochskalieren als "8K" (10-18) | erfasst | `World/20…:96` (W-KA-006) |
| Hunter HQ nicht auf öffentlichen Karten (10-18/19) | erfasst; Label-Befund erfasst | `Entscheidungsstatus.md` §2.2 und Zeile 228 |
| Hauptfluss nördlich Metropolis, nicht Downtown (10-18/19) | erfasst | `Spielwelt…:121` |
| Glenhaven/Ashford binnen, Greywater Hafenstadt (10-18/19) | erfasst | `Spielwelt…:180–182`; `World/20…:128` |
| Siedlungshierarchie (10-19) | erfasst | `Spielwelt…:184` |
| Fotorealistische Figuren, Charaktereditor (10-18) | erfasst (langfristig) | `Gameplay-Systeme.md:280` |
| Open-Source nur nach Audit, keine Agenten-Git-Operationen (10-18/19) | erfasst | `CLAUDE.md` §9–10 |
| Phase 0.3 WorldFacts (10-19) | Stand-Frage | `Docs/Decisions/0004`; `Docs/Planning/TASKS.md:16` (siehe WD-04) |

## 4. Neue Entscheidungen oder Anforderungen (DL-NNN)

Es gibt keine neue Geschäftsentscheidung, die im Repo fehlt. Die folgenden Einträge sind neu oder nur teilweise erfasst.

| ID | Inhalt | Quelle (Datei:Zeile) | Klassifikation | Ziel | Status |
|---|---|---|---|---|---|
| DL-001 | Freitext hat Vorrang vor Voreinstellungen. "Keine Ergänzung" heißt: keine Zusatzbedingung, keine Zustimmung zu Assistenten-Vorschlägen. | `2026-10-14/CHANGELOG.md:27`; `2026-10-16/docs/open_questions/OPEN_QUESTIONS_2026-10-16.md:31`; `2026-10-17/CHANGELOG.md:15`; `2026-10-17/docs/open_questions/OPEN_QUESTIONS_2026-10-17.md:3` | Bestätigt (Entwicklerregel, Prozess) | `Docs/Planning/DECISIONS.md` (neuer Prozessabschnitt) | Neu in Planning. In `Docs/Game/World/20…` nur verstreut (W-UG-018, W-GS-005). |
| DL-002 | Insel-Stabilität: Die bevorzugte frühere Ostinsel-Form wird bei Karteniterationen nicht umgezeichnet. Endgültige Form und Name bleiben offen. | `2026-10-19/CHANGELOG.md`, Punkt 6; `2026-10-19/docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md`, Abschnitt "Geometry" (Insel) | Bestätigt als Anforderung; Referenzvariante nicht eindeutig identifiziert | `Docs/Game/World/20…` (neuer W-KA-Eintrag) | Teilweise: nur in `Docs/Planning/Konsolidierung/B-welt.md:197` (L4), nicht als Anforderung in `Docs/Game/`. |
| DL-003 | Kartenabnahme braucht Maßprüfung: Flächenbilanz (Festland, Metropolis, Insel, Seen), Flusskonnektivität, eine Geometrie für alle Exporte. Ergebnisse als Artefakte (area_report, river_graph_report, georeference). | `2026-10-19/docs/specs/MAP_ACCEPTANCE_CHECKS_2026-10-19.md:5–26` | Spezifikation-Vorschlag (Prüfverfahren); Flächenbilanz selbst ist Entwicklervorgabe | `Docs/Game/World/17-Weltgenerierung-und-GIS-Daten.md` (Prüfabschnitt) | Teilweise: Grundsätze in `World/17` (Zeilen 41, 58–59); Prüfschritte und Artefakte fehlen. |
| DL-004 | Phase-0.3-Regeln: Golden-Datei nie zur Testrettung neu erzeugen. Windows/MSVC und Git-EOL offen. Journal-Wachstum und `StateAt` (O(n)) sind ein offenes Risiko. | `2026-10-19/docs/specs/WORLDFACTS_PHASE_0_3_2026-10-19.md:34–39` | Bericht (technische Risiken) mit Regelvorschlag | `Docs/Decisions/0004` (Ergänzung) oder `Docs/Planning/OPEN_QUESTIONS.md` | Teilweise: Versionsregel in `Docs/Decisions/0004:65–67`; Windows/EOL-Test und Risiko `StateAt` nicht erfasst (Ansatz in `Ermittlung/19…:37`). |
| DL-005 | Recherche KI-3D-Pipeline (Tripo Rigging v3.0, Retopology v3, ComfyUI-API-Migration): keine Entscheidung. Vor Nutzung Vergleich am gleichen Referenzcharakter und Lizenzprüfung. | `2026-10-19/docs/design/AI_3D_PIPELINE_WATCH_2026-10-19.md:5–19` | Bericht (Herstellerangaben, keine Entscheidung) | `Docs/Planning/` (Recherche-Notiz, nicht `Docs/Game/`) | Neu. Kein Treffer zu "Tripo" in `Docs/`. |
| DL-006 | U37 als offene Frage mit Optionsliste A–F; Assistenten-Präferenz D ist keine Entscheidung. | `2026-10-17/docs/open_questions/OPEN_QUESTIONS_2026-10-17.md:5–11` | Offen (Status bestätigt, Optionsliste Vorschlag) | `Docs/Planning/OPEN_QUESTIONS.md` | Teilweise: Status in `World/20…:299`; Optionsliste A–F nur im Delta. |

## 5. Widersprüche

Keine Widerspruchsentscheidung wurde getroffen. Jeder Punkt braucht eine Entwicklerantwort.

| ID | Widerspruch | Quellen | Vorschlag |
|---|---|---|---|
| WD-01 | **U24–U36 gegen N-02.** U24–U36 gelten als bestätigte Gesellschaftsentscheidungen. Sie setzen eine unabhängige unterirdische Gesellschaft mit mehreren Städten voraus (Delta 10-15, Kanon-Datei, Punkt 2 bis 4). N-02 streicht diese Zivilisation. | `Docs/Game/Entscheidungsstatus.md:90` (verbindlich) gegen `Entscheidungsstatus.md:267` (N-02); `2026-10-15/docs/canon/UNDERGROUND_SOCIETY_DECISIONS_24-32.md` (Interoperabilität) | Entwickler entscheidet, welche U-Punkte ohne Zivilisation gelten (z. B. für Bunker, Höhlen, Geheimlabore). Bis dahin: Status "unklar" statt "verbindlich". |
| WD-02 | **2.200 km² gegen N-01 und K-20.** Die Delta-Kanon-Dateien 10-18 und 10-19 führen 2.200 km² als aktive Vorgabe. N-01 (08.11.2026) hat die Größe als offen erklärt. | `2026-10-18/docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md:3, 41`; `2026-10-19/docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md:3`; `Entscheidungsstatus.md:100` | N-01 gilt (Vorrangregel 1). `Entscheidungsstatus.md:100` steht noch als Arbeitsziel und braucht ein Update (nicht geändert). |
| WD-03 | **Metropolis 110 km² gegen C-04.** 110 km² gelten "für diesen Kandidaten" (10-18). C-04 nennt 20–25 km² bei 2.200 km². Mit offener Weltgröße ist die Bezugsgröße unklar. | `2026-10-18/CHANGELOG.md`, Punkt 2; `DECISIONS.md` C-04; `CONFLICTS.md` K-06 | Entwickler legt die Metropolis-Fläche für die nächste Weltgröße fest. |
| WD-04 | **Phase 0.4 Status.** Delta 10-19 nennt die Erscheinungsdaten als Vorschlag ohne Umsetzung. Repo `Docs/Planning/TASKS.md:17` führt FOUNDATION-APPEARANCE als "abgeschlossen" mit `Docs/Decisions/0005`. Ob der Repo-Stand neuer ist, lässt sich ohne Git-Prüfung nicht klären. | `2026-10-19/docs/design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md:3`; `Docs/Planning/TASKS.md:17` | Repo-Stand mit Entwickler prüfen; Delta-Stand als Stichtag 19.10. kennzeichnen. |
| WD-05 | **Git-Stand.** Deltas 10-18 und 10-19 nennen lokale Commits `3d8a5e3`, `24c9b3b`, `b52d129` und einen Push auf ein privates Remote. Der Git-Snapshot der Sitzung zeigt andere letzte Commits (ad5ade2, 704e54d, 2be43fd, 3f92c69, a99849f) ohne diese Hashes. | `2026-10-18/docs/status/CPP_PHASE_0_PROGRESS.md`; `2026-10-19/docs/status/CPP_GIT_STATUS_2026-10-19.md` | Entwickler bestätigt den Stand. Keine Git-Befehle in dieser Auswertung. |

## 6. Offene Punkte

| ID | Offener Punkt | Delta-Bezug | Nächste Frage an den Entwickler |
|---|---|---|---|
| K-19 | Freigabe Phase A WORLD_700 (9 oder 11 Regionen) | keiner | Welche Fassung gilt? |
| K-22 | Amputation und Gliedmaßen | keiner | Nur Story-gebunden oder physikalisches Ergebnis ohne Automatismus? |
| K-23 | Romantik im Basisspiel gegen Adult-Paket | keiner (K-10 verwandt) | Abgrenzung festlegen |
| K-24 | Geltung "individueller Tod bleibt endgültig" | keiner | Gilt es für den Spieler? |
| K-26 | Todesmodell des Spielers | keiner | Checkpoint nach echtem Tod oder Permadeath? |
| K-27 | Todesursachen wichtiger NPCs | keiner | Welche Todesursachen sind zulässig? |
| K-28 | Bunker gegen Hunter-HQ (oberirdisch) | 10-14 `OPEN_QUESTIONS_2026-10-14` Punkt 4; `DECISIONS.md` C-46 | Beziehung festlegen (identisch, getrennt, verbunden) |
| U35-R | Restmechanismen U35: Notstopp, Interlocks, Protokolle, Verantwortung | 10-17 `AI_AUTONOMY_BOUNDARIES` | Welche Sicherung ist für kritische Infrastruktur Pflicht? |
| U33/U34-R | Bewusstseinsnachweis, konkrete Rechte, Kopieren/Löschen | 10-16, 10-17 | Welche Kriterien gelten? |
| U37 | Anbindung unterirdischer Netze ans Oberflächen-Internet | 10-17 (DL-006) | Option A–F wählen oder Freitext |
| V06 | Realistisches Schadensmodell | 10-15 `ROAD_VEHICLES` | Tiefe von Schaden und Reparatur |
| V02-T | Machbarkeit L1+L2 als analoge Kupplung | 10-15 | Prototyp; keine Umbelegung ohne Auftrag |
| R-OFF | Genaue Strecken, Signal- und Strom-Standard, Flottengrößen | 10-15 `RAILWAY_SYSTEM` | Welche Ebene zuerst? |
| MED-OFF | Lebensdauer, Verjüngungsgrenzen, Gründer-Ursprung | 10-15 U28–U30 | Zahlen festlegen oder offen lassen |
| MAP-1 | Weltgröße (N-01) und Metropolis-Fläche (WD-03) | 10-18, 10-19 | Neue Größe festlegen |
| MAP-2 | Ostinsel-Vorlage nicht eindeutig identifiziert | 10-19 (DL-002) | Referenzbild bestätigen |
| MAP-3 | Keine Karte akzeptiert (VISUAL_NOT_ACCEPTED) | 10-18, 10-19 | Akzeptierte Fassung benennen |
| MAP-4 | Hunter-HQ-Label auf erzeugten Bildern | 10-19 `ASSET_REGISTER` | Schutzebene für Karten einrichten |
| MAP-5 | Quellgeometrie `SPN_SCALE_MASTER_V1` nicht geprüft (kein Rohzugriff) | 10-19 | Rohdatei bereitstellen |
| TOOL-1 | Tripo v2-Abschaltung laut Spiegel 31.10.2026. Dieses Datum liegt vor dem Stand 08.11.2026, der Status ist ungeprüft. | 10-19 `AI_3D_PIPELINE_WATCH` | Vendor-Status prüfen (nur nach Entwicklerfreigabe) |
| PUB-1 | Lizenz und Publikationskopie | 10-18, 10-19 | Nach Audit; keine Lizenz festlegen |

## 7. Grenzen

- Die Delta-Dateien sind selbst als Review-Pakete gekennzeichnet (REVIEW REQUIRED). Ihre Kanon-Angaben sind keine verifizierten Master-Merges.
- Berichtete Testergebnisse (45/45, 62/62) und Kartenangaben (8K, Pixelzahlen, Flächen) wurden nicht reproduziert. Sie sind nur als Bericht klassifiziert.
- Der Master (2,9 GB) wurde nicht vollständig geprüft. Nur die Kanon-Dateien der Deltas und die Kopien unter `assets/prior_canon_refs/` wurden gelesen.
- Keine Datei unter `Docs/Game/`, `Planning/Master` oder dem Master wurde verändert. Keine Git-Befehle wurden ausgeführt.
