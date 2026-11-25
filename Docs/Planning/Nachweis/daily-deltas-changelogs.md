# 99_DAILY_DELTAS: Prüfung der CHANGELOGs 14. bis 19.10.2026 (MASTER-MIGRATION-17, 09.11.2026)

**Quelle (nur lesend):** `99_DAILY_DELTAS/2026-10-14` bis `2026-10-19`, je `SPN_DAILY_DELTA_<Datum>/CHANGELOG.md`. Gesamtordner: 102 Dateien in 6 Tagesordnern.

**Prüfregel:** Jede Entscheidung wird gegen die Zielstelle in `Docs/` gelesen. Die Quellen selbst sind keine Entscheidungen, wenn sie als „nicht bestätigt“, „offen“ oder „Vorschlag“ markiert sind. Berichte über Ergebnisse (Tests, Commits) sind keine Nachweise und bleiben außerhalb von `Docs/`.

## Tagesbefund

| Tag | Quelle | Entscheidungen (bestätigt) | Zielstellen in `Docs/` | Befund |
|---|---|---|---|---|
| 14.10. | CHANGELOG | Familienschlüssel: Ursprung zurückgestellt (SPN-20261014-001); Bevölkerung der größten Untergrundstadt vorläufig (-002) | `Planning/DECISIONS.md` C-44; `Game/Spielwelt-und-Untergrund.md` §Untergrund; N-02 (08.11.2026) | belegt. Beispiel „30 km² / 600.000 Einwohner“ wurde nicht akzeptiert; das ist durch N-02 gegenstandslos (Untergrundstadt gestrichen) |
| 15.10. | CHANGELOG | U24 bis U32 (Vertrauen, Haushalte, Lernen, Grundlagen, Medizin, Lebensdauer, Verjüngung, Körpermodifikation, KI und Robotik); R01 bis R05 (Schienenmengen, Depots, Elektrifizierung, Simulation, Befugnis zum Fahren); V01 bis V05 (Fahrphysik, DualSense, Start, Interaktion, Werkstätten) | `Game/World/12` §3 (U24 bis U32); `Game/World/09` §3 (Schiene, Mengen, Befugnis); `Game/Fahrzeuge-und-Verkehr/05` (DualSense), `04` (Kupplung); `Planning/DECISIONS.md` C-47 | belegt. U24 bis U32 gelten nach N-02 als Frage der Geltung (K-29) und bleiben so dokumentiert |
| 15.10. | CHANGELOG | offen: U33, V06 (Schadensmodell), Bevölkerung, Fläche, Lebensdauer, Fahrplanlayout | `Game/World/12` §3 (U33 offen); `Game/Entscheidungsstatus.md` (Schadensmodell der Fahrzeuge offen) | belegt |
| 16.10. | CHANGELOG | U33 (unterschiedliche Entwicklungsstufen), U34 (individuelle Anerkennung) | `Game/World/12` Zeilen 53–54 (D16-AI) | belegt; die Beispielstufen des Assistenten sind kein Gesetz und nicht übernommen |
| 16.10. | CHANGELOG | U35 offen (Aufsicht) | `Game/World/12` §3 (KI-Aufsicht je nach Aufgabe) | belegt; keine Entscheidung |
| 17.10. | CHANGELOG | U35 bestätigt: Regeln je nach KI und Aufgabe (Ergänzung: keine); U36 bestätigt: Kombination verschiedener Technologien, optionale Neuro-Schnittstellen; U37 offen | `Game/World/12` §3 (U35, U36, U37 offen, Zeile 57); Zeile 69 (Folgeliste U37); Zeile 103 (keine Gateway-Entscheidung vorab) | belegt. Die nicht bestätigten Punkte (universeller Veto, Regulator, Autonomiestufen) sind in `12` als nicht festgelegt geführt |
| 18.10. | CHANGELOG | Kartenrichtung ~2.200 km² (Arbeitsstand); Metropolis 110 km²; Kartenregeln (eine Quelle, keine Beschriftung des geheimen HQ, native Auflösung); V3/V4 Referenzen, V5 verworfen, V6 Kandidat; Projektpolitik (Phasen ohne Festlegung der Zahlen, kein Commit durch Assistenten) | `Game/Entscheidungsstatus.md` §3 (Metropolis 110 km²; Hauptwelt 2.200 Arbeitsziel); `Game/World/17` §6 (Abnahmeregeln); `Game/World/19` (V5 abgelehnt); `Game/World/20` W-ST-010 | belegt. Die Metropolis-Zahl ist durch K-20 (08.11.) als Arbeitsmaß markiert; keine Geometrie-Festlegung |
| 19.10. | CHANGELOG | ~25 Größenvarianten; 2.200 km² trockenes Festland, Metropolis 110 km² darin, 2.090 km² außerhalb; Ostinsel 80–100 km² (90 als Test); Sicht zweidimensional orthografisch; Flussanschluss nördlich; Siedlungshierarchie; Inselform bewahren | `Game/Spielwelt-und-Untergrund.md` Zeile 78 (25 Varianten); `Game/World/02`, `03`, `05`, `07`, `17`, `19`, `20` (Festland, Atlas, Fluss, Siedlungen, Inselform) | belegt. Keine Karten-Master-Freigabe und keine Inselform als final |
| 19.10. | CHANGELOG | Phase 0.3 WorldFacts (`StateAt`, Golden-v1-Speicherstand, Tests); Git-Status (privates Remote); Tripo-Recherche | `Docs/Decisions/0004-world-facts-save-format-v1.md` §6 (Phase-0.3-Risiken); `Docs/Development/Produktions-Referenzen-und-Prompts.md` §6 (KI-Pipeline, Herstellerangaben); Git-Status ist kein Designnachweis | belegt bzw. nicht als Anforderung übernommen (Berichte über Tests und Commits sind Statusangaben) |

## Ergebnis

- Die Entscheidungen aus den sechs CHANGELOGs (14.10. bis 19.10.) sind je gegen eine Zielstelle in `Docs/` geprüft. Keine fehlende bestätigte Entscheidung gefunden.
- **Nicht übernommen:** der Beispielwert 30 km² / 600.000 Einwohner (nicht akzeptiert, Untergrundstadt durch N-02 gestrichen); die Stufen-Beispiele der Assistenten zu KI (U33); das Angebot eines Oberflächen-Gateways (U37); Herstellerangaben zu KI-Riggings (nicht als Benchmark).
- **Offen bleiben:** U33 (Bewusstsein), U35 (Aufsicht, Formulierung im Detail), U37 (Netzarchitektur), V06 (Schadensmodell), Bevölkerung und Fläche der Untergrundstadt (N-02), Fahrplanlayout, Hydrologie-Akzeptanz, Karten-Master.

## Nicht geprüft in diesem Schritt

- Die Canon-, Spec-, Design- und Statusdokumente der Tagesordner (`docs/canon`, `docs/specs`, `docs/design`, `docs/status`, `docs/open_questions`, `docs/comparison`, `docs/provenance`) sind nur über die CHANGELOG-Einträge und frühere Läufe (World/17, 19, 24; Decision 0004, 0005) abgedeckt, nicht Zeile für Zeile.
- Installer, Prüfsummen, Manifeste, Tests und `tools/safe_install.py` sind Prozessdateien und begründet ausgeschlossen.
- `assets/source_archives/SPN_CANON_MERGE_2026-10-18.zip` ist ein Binärarchiv (10,8 KB), nicht geöffnet; Inhalte entsprechen den Canon-Dokumenten 10-18.

## Spezifikationen der Tage 16. bis 19.10. (Kernregeln gegen Zielstellen geprüft)

| Quelle | Zielstelle in `Docs/` | Befund |
|---|---|---|
| 16.10. `docs/specs/AI_RECOGNITION_AND_CONFLICTS.md` (sechs Dimensionen, Beispiele, offene Fragen) | `Game/World/12` §3, Zeilen 76–79 | belegt; Beispiele als 💡 ohne Quest-Status |
| 16.10. `docs/specs/AI_SIMULATION_BOUNDARIES.md` (bestätigter Rahmen, Architektur 💡, Abnahme, Verbote) | `Game/World/12` §3, Zeilen 81–86 | belegt |
| 17.10. `docs/specs/AI_AUTONOMY_BOUNDARIES.md` (Einsatzbereiche, zwei Grundregeln, Modell 💡, fünf Ausfall-Szenarien, offene Fragen) | `Game/World/12` §3, Zeilen 88–96 (fünf Szenarien in Zeile 94) | belegt; Zählung 5 = 5 |
| 17.10. `docs/specs/COMMUNICATION_MODALITIES.md`, `UNDERGROUND_AI_AND_COMMUNICATION_U35_U36.md` | `Game/World/12` §3 (U36, Kommunikation) | belegt |
| 17.10. `docs/specs/PRIVACY_SECURITY_AND_FAILURE_REQUIREMENTS.md` | `Game/World/12` §3 (Datenschutz, 💡 mit offenen Entscheidungen) | belegt |
| 17.10. `docs/design/PROPOSED_CPLUSPLUS_IMPLEMENTATION_NOTES.md` | nicht übernommen | ausdrücklich nicht kanonisch (Implementierungsvorschlag); keine C++-Umsetzung in diesem Lauf |
| 18.10. `docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md`, `docs/specs/MASTER_MAP_ACCEPTANCE_SPEC.md` | `Game/World/17` §6 (Abnahme-Gates, Stil, keine Hochskalierung als Detail, Reihenfolge der Gates, Nicht-Ziele) | belegt |
| 18.10. `docs/design/CHARACTER_CREATOR_AND_RELEASE_POLICY.md` | `Game/Gameplay-Systeme.md` §12 (Charaktereditor, Fotorealismus als langfristige Richtung); `Decisions/0005-character-appearance-foundation.md` | belegt; der Rest bleibt Vorschlag |
| 19.10. `docs/canon/WORLD_MAP_REQUIREMENTS_2026-10-19.md` | `Game/Spielwelt-und-Untergrund.md` Zeile 78 (25 Varianten), `Game/World/02`, `03`, `05` (Festland, Insel, Fluss) | belegt |
| 19.10. `docs/specs/MAP_ACCEPTANCE_CHECKS_2026-10-19.md` (13 Prüfpunkte, Prüfartefakte) | `Game/World/17` §6 (Flächenbilanz, Insel, Seen, Flussgraph, Geheimnisebene, Siedlungspolygone, gleiche Master-Geometrie) | belegt. Die Liste der Prüfartefakte (`area_report.json` usw.) ist ein Werkzeugvorschlag und nicht übernommen |
| 19.10. `docs/specs/WORLDFACTS_PHASE_0_3_2026-10-19.md` | `Docs/Decisions/0004` §6 | belegt; Phasenstatus ist Bericht, keine Anforderung |
| 19.10. `docs/design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md` | `Docs/Decisions/0005` | Vorschlag; nicht als Entscheidung übernommen |
| 19.10. `docs/design/AI_3D_PIPELINE_WATCH_2026-10-19.md` | `Development/Produktions-Referenzen-und-Prompts.md` §6 | Herstellerangaben, nicht benchmarkt |

**Stand nach dieser Prüfung:** Die Entscheidungen und die Kernregeln der Spezifikationen vom 16. bis 19.10. sind gegen `Docs/` belegt. Nicht Zeile für Zeile geprüft: Design- und Statusdateien der Tage 14. bis 19. (`docs/status`, `docs/comparison`, `docs/provenance`) und die Asset-Register.

## Klassifikation der Berichtsdateien (nicht Zeile für Zeile gelesen, nach Kopf und Prüfgrenze eingeordnet)

| Ordner | Dateien | Art | Umgang |
|---|---:|---|---|
| `docs/status` (14.–19.10.) | 4 | Statusberichte; C++-Fortschritt und Git-Stand vom Entwickler gemeldet, **nicht unabhängig geprüft** | keine Anforderung; die gemeldeten Testzahlen (45/45, 62/62) werden **nicht** als Nachweis übernommen |
| `docs/comparison` | 6 | begrenzte Vergleiche mit älteren Canon-Dateien, ausdrücklich „keine vollständige Prüfung“ | keine Anforderung; die Abweichungen sind in den Zielstellen oben geprüft |
| `docs/provenance` | 6 | Herkunfts- und Evidenznotizen | Prüfgrenzen übernommen; keine Anforderung |
| `docs/open_questions` | 6 | Fragenregister je Tag | Fragen gegen Zielstellen geprüft (U33, U35, U37, V06, Untergrundstadt, Fahrplan, Hydrologie, Karten-Master) |
| `assets/`, `data/`, `tools/`, `tests/`, `install.sh`, `manifest.json`, `SHA256SUMS`, `README.md` | rest | Prozess- und Metadaten, Installation | begründet ausgeschlossen |

## Phase 2 (MASTER-MIGRATION-18): Ledger und Restdateien

| Datei | Art | Befund | Umgang |
|---|---|---|---|
| `data/decision_ledger.json` 16.10. | verbindliche Nutzerentscheidungen U33, U34 (Freitext: keine Ergänzung); U35 offen | belegt in `Game/World/12` §3 | übernommen; die Assistenten-Beispiele bleiben ausgeschlossen |
| `data/decision_ledger.json` 17.10. | U35 (Option D), U36 (Option E) bestätigt; U37 offen; Ausnahme: keine Ableitung weiterer Entscheidungen aus Beispielen | belegt in `Game/World/12` §3 | übernommen |
| `data/decision_ledger.json` 18.10. | Kartenrichtung (2.200 km² Arbeitsziel, Meer nur Osten, Gebirge nur Norden), Metropolis 110 km², Wasserrollen, Kartenabnahme, Phasen ohne Zahlenfestlegung, Fotorealismus als langfristige Richtung, Open-Source-Absicht ohne Veröffentlichung | belegt in `World/02`, `05`, `07`, `17`, `Entscheidungsstatus`, `CANON` | übernommen; C++-Phasenstand (45/45 Tests) ist vom Entwickler gemeldet und **nicht** übernommen |
| `data/decision_ledger.json` 19.10. | ~25 Größenvarianten, 2.200/2.090/110 km², Ostinsel 80–100 km², orthografische 2D-Karte, Flussführung, Siedlungshierarchie, Inselform | belegt in `Spielwelt` Zeile 78, `World/02`, `03`, `05`, `07`, `17` | übernommen; Phase-0.3-Berichte und Tripo-Herstellerangaben nicht als Anforderung |
| `docs/design/PROPOSED_CPLUSPLUS_IMPLEMENTATION_NOTES.md` 17.10. | Vorschlag, ausdrücklich nicht kanonisch | nicht übernommen (keine C++-Umsetzung in diesem Lauf) |
| `docs/design/CHARACTER_APPEARANCE_PHASE_0_4_PROPOSAL.md` 19.10. | Vorschlag; offene Punkte (Morphanzahl, Rig, Mesh-Technik, Kollision, Editor-UI, Consent-Regeln) | `Docs/Decisions/0005` Zeile 57 führt sie als offen | Vorschlag bleibt Vorschlag |
| `docs/integration/MANUAL_REVIEW_GUIDE.md` 16.10., `MERGE_REVIEW.md` 17.10. | Prozessanleitung für die Übernahme | ausgeschlossen (Prozess) |
| `assets/ASSET_REGISTER.md` (14.–19.10.), `SHA256SUMS`, `manifest.json`, `install.sh`, `tests/`, `tools/safe_install.py`, `data/PACKAGE_INPUT_HASHES.md` | Inventare, Installation, Prüfsummen | ausgeschlossen (Metadaten, keine Inhaltsquelle) |

**Ergebnis Phase 2:** Alle Entscheidungs-Ledger sind belegt. Keine fehlende verbindliche Entscheidung. Vorschläge und Berichte sind als solche gekennzeichnet und nicht als Anforderung übernommen.

## Phase 2, Nachtrag: Kommunikations- und Datenschutzspezifikation (17.10.)

- `docs/specs/COMMUNICATION_MODALITIES.md` und `docs/specs/PRIVACY_SECURITY_AND_FAILURE_REQUIREMENTS.md` **vollständig gelesen** und Wort für Wort gegen `Docs/Game/World/12` §3 (Zeilen 64–69 Kommunikation, 98–103 Datenschutz) geprüft. Belegt: die fünf Modalitäten (persönliche Geräte, Hologramm, intelligente Räume, freiwillige neurale Schnittstelle, Wahl der Mittel), die Trennung von Absicht und Darstellung, freiwillige neurale Links ohne Pflichtimplantat, die offenen Punkte der U37-Liste, die Sicherheitsanforderungen als Vorschlag mit offener Entscheidung und die Prüffälle. Nicht übernommen (ausdrücklich NON-CANON): Protokoll- und Netzwerkdiagramm, konkrete Klassen- und Architekturnamen, Gameplay-Ideen mit Holoplanung als Pflichtbestandteil.
- U37 bleibt offen; keine Oberflächen-Gateway-Entscheidung vorweggenommen.

## Arbeitspaket A (MASTER-MIGRATION-19): docs/status und docs/comparison

Geprüft: alle 3 Statusberichte und alle 6 Vergleichsberichte **vollständig** gelesen. Entscheidung je Datei:

| Datei | Befund | Entscheidung |
|---|---|---|
| `status/CPP_PHASE_0_PROGRESS.md` (18.10.) | Entwicklerbericht über Phase 0.1/0.2 (45/45 Tests, Commit `3d8a5e3`), „not independent code inspection“. Enthält eine Entwicklungs-Richtlinie: Weltdaten parametrisiert halten, keine Hartkodierung eines Karten-Geometriewerts; C++-first; KI-Assistenten committen nicht. | **Ausgeschlossen (Statusbericht).** Die Richtlinien sind keine neue Spielanforderung; die Git-Regel steht in der Projekt-Richtlinie, die Kartenregel in `Spielwelt` (Zeile 78, Größenvarianten). |
| `status/MAP_ITERATION_REVIEW.md` (18.10.) | Kandidatenstatus: V3 Zwischenreferenz, V4 nicht bestätigt, V5 verworfen, V6 nur Kandidat. Keine Binärdaten vorhanden. | **Ausgeschlossen (Statusbericht)**, Inhalt belegt in `World/19` §6 (Bildkandidaten V3–V7) und `World/17` §6. |
| `status/CPP_GIT_STATUS_2026-10-19.md` | Entwicklerbericht über Git-Stand (privates Remote, Commits). Kontrollregel: nur der Entwickler committet, pusht, taggt und veröffentlicht. | **Ausgeschlossen (Statusbericht).** Repository-Stand ist kein Designnachweis; die Kontrollregel ist Projektrichtlinie, nicht Spielanforderung. |
| `comparison/PRIOR_DOCUMENTED_CANON_COMPARISON.md` (14.10.) | Vergleich: Surface-Variante 1 von 25, Metropolis 22,5 km², Glenhaven 9 km², Hunter-HQ-Hypothese südlich (10–15 km), Hydrologie-Varianten. | **Ausgeschlossen (Vergleichsbericht).** Belegt: Größen und Varianten in `Spielwelt` Zeile 78 und `World/03`, `World/07` (9 km²), HQ-Lage in `Spielwelt` §15/§12 (Hypothese, Lage offen). |
| `comparison/LIMITED_PRIOR_CANON_DIFF.md` (15.10.) | Vergleich mit V3.1o-Audit (`NOT_ACCEPTED`) und V3.1q-Runbook; Untergrund und Bahn als getrennte Systeme. | **Ausgeschlossen.** Terrain-Reihe in `World/19` §4 belegt; Untergrund-Trennung in `Gameplay`/`Spielwelt` (N-02). |
| `comparison/DIFF_AGAINST_PRIOR_DELTAS.md` (16.10.) | U33/U34 bestätigt, U35 offen. | **Ausgeschlossen.** Inhalt in `World/12` §3 belegt. |
| `comparison/DIFF_AGAINST_2026-10-14_TO_16.md` (17.10.) | U35 bestätigt (D), U36 bestätigt (E), U37 offen. | **Ausgeschlossen.** Inhalt in `World/12` §3 belegt. |
| `comparison/PRIOR_CANON_DIFF.md` (18.10.) | WORLD_700 unverändert; 2.200-km²-Arbeitsziel; Metropolis 110 km²; Kartenabnahme; Supersession-Disziplin. | **Ausgeschlossen.** Alle Punkte in `World/02`, `03`, `17`, `19`, `Entscheidungsstatus` §3 belegt. |
| `comparison/DELTA_VS_PRIOR_CANON_2026-10-19.md` (19.10.) | ~25 Varianten, 2.200/2.090/110 km², Ostinsel 80–100, orthografische Karte, Fluss, Siedlungshierarchie; Warnhinweis: Hunter HQ auf generierten Vorschauen sichtbar (QA-Befund). | **Ausgeschlossen.** Inhalte belegt in `Spielwelt` Zeile 78, `World/02`, `05`, `07`, `17` §6. Der HQ-Hinweis ist Kartenregel (Entscheidungsstatus §6.5) und keine neue Anforderung. |

**Ergebnis A:** Keine verbindliche Anforderung nur in diesen Berichten gefunden. Ausschluss begründet: Berichte mit Entwicklerangaben oder Vergleichsaussagen, deren Inhalte an anderer Stelle in `Docs/` belegt sind. Verzeichnisnamen allein waren nicht Grundlage.
