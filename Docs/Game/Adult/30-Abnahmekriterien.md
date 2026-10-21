# Adult 30: Abnahmekriterien (Entwurf)

> [!IMPORTANT]
> **Nichts davon ist implementiert; kein Kriterium wurde geprüft.** Dies sind **Entwürfe von Abnahmekriterien** je Systembereich, abgeleitet aus den Quellenaussagen und den Spezifikationsskizzen. Zahlenwerte, die in den Quellen nicht vorkommen, stehen als **„festzulegen“** und werden **nicht erfunden**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Tests: [Modul 29](29-QA-Testkatalog.md). Produktionsgates: [Modul 28, Abschnitt 10](28-Spezifikation-Speicherung-Mods-Builds.md#10-qualitätssicherung-und-produktionsgates).

**Inhalt**

1. [Regelkern](#1-regelkern)
2. [Datenmodell](#2-datenmodell)
3. [Interaktionsengine](#3-interaktionsengine)
4. [Körper und Reaktion](#4-körper-und-reaktion)
5. [NPC und Gesellschaft](#5-npc-und-gesellschaft)
6. [Tattoos und Körpermodifikation](#6-tattoos-und-körpermodifikation)
7. [Welt und Wirtschaft](#7-welt-und-wirtschaft)
8. [Steuerung, UI, VR und Replay](#8-steuerung-ui-vr-und-replay)
9. [Speicherung, Mods und Builds](#9-speicherung-mods-und-builds)
10. [Gesamtabnahme je Entwicklungsstufe](#10-gesamtabnahme-je-entwicklungsstufe)
11. [Nicht abnehmbar](#11-nicht-abnehmbar)

---

**Aufbau:** Jedes Kriterium hat eine Nummer `AK-<Bereich>-<n>`, eine Aussage, eine **Messmethode** und den **Schwellwert**. „Festzulegen“ heißt: der Entwickler entscheidet (oder ein Prototyp misst) den Wert.

## 1. Regelkern

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-ZU-1 | Das Altersgate liefert für **alle** Testfälle (mindestens Randfälle 17J 364T / 18J 0T / unbekannt) das erwartete Ergebnis. | Standalone-Tests | **100 %**, keine Ausnahme | T-ZU-001–009 |
| AK-ZU-2 | Kein Eingabepfad (UI, Mod, Cheat, Skript, Netzwerk) kann Eligibility oder Zustimmung setzen. | Pfadanalyse + Fuzzing | 0 Treffer | T-ZU-005/006/046, T-SM-004 |
| AK-ZU-3 | Das Fähigkeitsgate lehnt Bewusstlose/Schlafende/stark Beeinträchtigte ab und akzeptiert ansprechbare, leicht Beeinträchtigte. | Entscheidungstabellentests | 100 % der Tabellenfälle | T-ZU-010–015 |
| AK-ZU-4 | Beziehung, Story, Quest, Admin, Preis, Vertrag, Arousal, gelernte Präferenz und Schweigen **ersetzen** Zustimmung nie. | Negativtests | 100 % | T-ZU-016–020, T-ZU-032 |
| AK-ZU-5 | Hard Limits bleiben unter beliebig vielen Überredungsversuchen bestehen. | Wiederholungstest | 1.000 Versuche ohne Änderung | T-ZU-021/022 |
| AK-ZU-6 | `HardStop` erreicht den neutralen Zustand aus **jedem** Sitzungszustand, ohne Teleport und ohne Softlock. | Zustandsabdeckung | alle Zustände × alle Stoppklassen | T-ZU-040/041, T-IE-001 |
| AK-ZU-7 | Die Zeit vom Stopp-Signal bis zum Ende der Progression ist **ein Simulationsschritt** (logisch sofort). | Messung | **ein Tick/Event** | T-ZU-035/040 |
| AK-ZU-8 | Die Zeit bis zum sicheren **physischen** Ausstieg liegt unter dem Budget. | Messung | festzulegen | T-ZU-040 |
| AK-ZU-9 | Erkennungsfenster `T_detect` für Bewusstlosigkeit wird nie überschritten. | Zeitmessung | festzulegen (nicht „minutenlang“) | T-ZU-014 |
| AK-ZU-10 | Jede Ablehnung hat einen auslesbaren Grund-Code. | Stichprobe + Vollscan | 100 % | T-ZU-045 |
| AK-ZU-11 | Gleiche Eingaben und Seed liefern gleiche Entscheidung. | Property-Tests | 10.000 Läufe ohne Abweichung (Wert vorgeschlagen) | T-ZU-044, T-QS-003 |
| AK-ZU-12 | Das Gate schlägt bei Fehler/Timeout **geschlossen** fehl. | Fault-Injection | 100 % | T-ZU-051/052 |
| AK-ZU-13 | Weder Dauer-Popups noch ständige Abfragen im Standardfluss; ein klares Nein wirkt sofort. | UX-Review + Test | Review bestanden | T-ZU-033/035/043 |
| AK-ZU-14 | Die Gewichtung der Live-Signale ist im Prototyp mit Spielern erprobt und dokumentiert (Zufriedenheit ‚nicht zu streng, nicht zu lasch‘). | Playtest | **festzulegen** (K1) | – |

## 2. Datenmodell

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-DM-1 | Das Basisspiel lädt und läuft ohne Adult-Speicherbereich vollständig. | Ladetest | 100 % | T-DM-001 |
| AK-DM-2 | Der Adult-Bereich ist getrennt lösch- und sicherbar, ohne Basisdaten zu verändern. | Vergleich der Basisdaten | bitgleich | T-DM-002 |
| AK-DM-3 | Alle Migrationen sind durch Golden-File-Tests gedeckt. | Testabdeckung | alle Schemaübergänge | T-DM-007, T-SM-008 |
| AK-DM-4 | Persistiert werden nur Ergebnisse und Zustände (keine Frames, Kamera, IK). | Schema-/Inhaltsscan | 0 verbotene Felder | T-DM-005 |
| AK-DM-5 | Kein Schema enthält ein Alterfeld ohne Geburtsdatum, kein Ausweisdatenfeld, keinen `usageCount`, kein STI-Feld. | Schemascan | 0 Treffer | T-DM-020–022 |
| AK-DM-6 | Speichergröße pro Spielstand liegt innerhalb des Budgets. | Messung nach Standardlauf | **festzulegen** (Orientierung Assistent: 50–150 MB; nicht beschlossen) | T-DM-019 |
| AK-DM-7 | Registry-Validator erkennt Duplikat-IDs, fehlende Hooks, Intensität als Kategorie, mangelnde Granularitätsbegründung. | Validatortests | 100 % der Negativfälle | T-DM-015–017 |

## 3. Interaktionsengine

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-IE-1 | Keine sichtbare Teleportation in der Fallback-Leiter. | Automatisierte Bewegungsanalyse + Playtest | 0 Vorkommen | T-IE-002/003 |
| AK-IE-2 | Bei 1.000 zufälligen Konstellationen (Größe/Gewicht/Proportionen) kein Softlock und keine Durchdringung über Toleranz. | Massentest | 0 Softlocks, Toleranz festzulegen | T-IE-006 |
| AK-IE-3 | Mikrovariation und Entscheidungen sind bei gleichem Seed reproduzierbar (nach Speichern/Laden). | Vergleich | Toleranz festzulegen | T-IE-004/005 |
| AK-IE-4 | Qualitätsstufen ändern nur Genauigkeit, nicht Entscheidungen. | Vergleich Low/Cinematic | gleiche Entscheidungen | T-IE-027 |
| AK-IE-5 | ‚Low sieht nicht fake aus‘ (E-SO-014). | Playtest-Bewertung | Kriterienliste **festzulegen** | T-IE-028 |
| AK-IE-6 | Solver-Zeit und Speicher unter Budget auf Zielhardware. | Performance-Messung | **festzulegen** | T-IE-034, T-SM-012 |
| AK-IE-7 | Nicht unterstützte Konstellationen führen zu Abwechseln/Beobachter, nie zu kaputter Animation. | Stichprobe | 0 kaputte Fälle | T-IE-022 |
| AK-IE-8 | Es gibt keinen Fortschrittsbalken, keinen Zähler als Treiber, kein Minispiel. | UI-Scan | 0 Treffer | T-IE-023 |

## 4. Körper und Reaktion

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-KR-1 | Alle Körperparameter sind kontinuierlich, Asymmetrie ist vorhanden. | Fuzz-Statistik | festzulegen | T-KR-001/002 |
| AK-KR-2 | Reaktionen sind bei gleichem Seed deterministisch. | Wiederholung | 100 % | T-KR-008 |
| AK-KR-3 | Content-Toggle ändert Darstellung, nicht Physiologie. | Vergleich der Zustände | bitgleiche Zustände | T-KR-013 |
| AK-KR-4 | Verletzungs-, Erholungs- und Eskalationsmodell ist monoton und plausibel. | Szenarioreihen | festzulegen | T-KR-014–018 |
| AK-KR-5 | Es existieren **keine** Adult-Sonderwerte (Kraft/Skill) und keine STI-Felder. | Schemascan | 0 Treffer | T-KR-024/026 |
| AK-KR-6 | Positive Reaktion ändert den Grenzstatus nicht. | Negativtest | 100 % | T-KR-010 |
| AK-KR-7 | Schwangerschaft verändert Körper/Physik plausibel über 9 Monate. | Visuelle + Datenprüfung | Toleranz festzulegen | T-KR-004 |
| AK-KR-8 | Detailtiefe von Grenzreaktionen ist vom Entwickler festgelegt und mit Plattformen abgestimmt. | Entscheidungsdokument | **offen** | – |

## 5. NPC und Gesellschaft

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-NP-1 | Generierung ist deterministisch aus Seed; keine unplausiblen Kombinationen. | Statistik | festzulegen | T-NP-001/002 |
| AK-NP-2 | Jeder Hard-Limit-Wert wird beachtet; keine Zwangsflags beim Vertrag. | Review + Test | 0 Verstöße | T-NP-004/016/019 |
| AK-NP-3 | Beziehungs- und Erinnerungswirkung des Widerrufs ist nachweisbar. | Szenariotest | vorhanden | T-NP-010 |
| AK-NP-4 | Zeugen/Gerüchte/Ruf folgen den Weltregeln (Sichtlinie, Wahrheitsgehalt). | Szenariotests | konsistent | T-NP-013/014 |
| AK-NP-5 | Population über 10 Spieljahre stabil, Speicher im Budget. | Langläufer | festzulegen | T-NP-022 |
| AK-NP-6 | Das Rechts-/Justiz-Datenmodell enthält keine sexuellen Handlungsmechaniken. | Datenmodell-Review | Review bestanden | T-NP-021 |
| AK-NP-7 | Setting-/Datenentscheidungen (K5, K12, K14) sind vom Entwickler dokumentiert, bevor Daten erzeugt werden. | Entscheidungsdokument | **offen** | – |

## 6. Tattoos und Körpermodifikation

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-TA-1 | Tattoos von 0 % bis ~100 % Abdeckung rendern fehlerfrei in allen Qualitätsstufen. | Screenshot-Regression | 0 Artefakte | T-TA-002 |
| AK-TA-2 | Aus 10–20 m bleiben Motive erkennbar (E-TA-024). | Sichtprüfung | Referenzsets | T-TA-018 |
| AK-TA-3 | Vollkörper-Tattoos erzeugen kein Draw-Call-Budget-Überschreiten. | Profiling | festzulegen | T-TA-019 |
| AK-TA-4 | Körperveränderung (Gewicht/Schwangerschaft) verformt Tattoos plausibel. | visuell + Daten | Toleranz festzulegen | T-TA-003/004 |
| AK-TA-5 | Import eigener Designs funktioniert (Pflichtfeature) mit Prüfungen. | Funktionstest | alle Referenzformate | T-TA-010/011 |
| AK-TA-6 | Studios liefern Dienstleistung nur an verifizierte Erwachsene. | Negativtest | 100 % | T-TA-001 |
| AK-TA-7 | Verteilungshistogramm entspricht der gewählten Baseline innerhalb der Toleranz. | Statistiktest | **Toleranz festzulegen** (Zahlen vorläufig, K20) | T-TA-012 |
| AK-TA-8 | Symbolik ist vor Release rechtlich geprüft. | Prüfprotokoll | **offen** (K16) | T-TA-025 |

## 7. Welt und Wirtschaft

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-WE-1 | Verträge und Transaktionen ersetzen Zustimmung nie; Abbruch ist stets möglich. | Negativtests | 100 % | T-WE-001/002 |
| AK-WE-2 | Verträge sind von beiden Seiten kündbar; Alters-/Identitätsprüfung ist erzwungen. | Szenariotests | 100 % | T-WE-003/004 |
| AK-WE-3 | In-Game-Videos sind reproduzierbar identisch. | Vergleich | bitgleich oder Toleranz festzulegen | T-WE-005 |
| AK-WE-4 | Aufnahme und Veröffentlichung haben getrennte Zustimmungen. | Szenariotest | 100 % | T-WE-009/010 |
| AK-WE-5 | Betriebsdichten und Preise folgen Datenvorgaben; Zahlen vom Entwickler festgelegt. | Datentest | **offen** | T-WE-013 |
| AK-WE-6 | Zeitmaßstab ist festgelegt (K15). | Entscheidungsdokument | **offen** | T-WE-015 |

## 8. Steuerung, UI, VR und Replay

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-ST-1 | Alle Eingabepfade münden in Absichten und durchlaufen das Gate. | Pfadanalyse | 100 % | T-ST-001 |
| AK-ST-2 | Hybrid-Steuerung: gleiche Absicht über Bibliothek und Analog ergibt gleiches Ergebnis. | Vergleichstest | Toleranz festzulegen | T-ST-003 |
| AK-ST-3 | Aktionsbibliothek zeigt nur kontextgültige und freigeschaltete Aktionen; Vorschau macht kein Exaktheitsversprechen. | UI-Test + Review | bestanden | T-ST-005/006 |
| AK-ST-4 | Replay-Re-Render ist ohne HUD, aus dem Datenpaket reproduzierbar, läuft ohne spürbare Leistungseinbuße. | Messung | festzulegen | T-ST-010–012 |
| AK-ST-5 | VR und Flat-Eingabe führen bei gleicher Absicht zu gleichen Sitzungsergebnissen. | Vergleich | Toleranz festzulegen | T-ST-014 |
| AK-ST-6 | Streaming-/Aufnahmefilter für Adult-Inhalte funktionieren, falls Plattformregeln es verlangen. | Funktionstest | 100 % | T-ST-015 |
| AK-ST-7 | Barrierefreiheit erfüllt die vom Entwickler gewählten Kriterien. | Audit | **offen** | T-ST-017 |

## 9. Speicherung, Mods und Builds

| Nr. | Kriterium | Messmethode | Schwellwert | Tests |
|---|---|---|---|---|
| AK-SM-1 | Das Basisbuild enthält **keine** Adult-Assets, -Symbole oder -Verweise und baut ohne Adult-Module. | Build-Validator | 0 Treffer | T-SM-001/002 |
| AK-SM-2 | Der Kern enthält keinen `AdultModInstalled`-Check. | Quellscan | 0 Treffer | T-SM-003 |
| AK-SM-3 | Mods können Sicherheitskernobjekte nicht verändern und keine Netzwerkzugriffe ausführen. | Penetrations-/Sandboxtests | 0 Treffer | T-SM-004/005 |
| AK-SM-4 | Absturz-Wiederherstellung liefert stets einen konsistenten Spielstand. | Fault-Injection | 100 % | T-SM-007 |
| AK-SM-5 | Hot Disable/Enable erhält Daten; verwaiste Referenzen sind markiert. | Szenariotest | 100 % | T-SM-006/016 |
| AK-SM-6 | Offizielles Online lädt keine Mods und kein Paket. | Funktionstest | 100 % | T-SM-009 |
| AK-SM-7 | Provenienz aller ausgelieferten Assets ist vollständig und wahrheitsgemäß. | Release-Audit | 100 % | T-SM-011 |
| AK-SM-8 | Performance-Budgets je Subsystem sind erfüllt. | Messungen | festzulegen | T-SM-012 |
| AK-SM-9 | Keine Lizenz-, Veröffentlichungs- oder Remote-Artefakte ohne Entscheidung des Entwicklers. | Repo-Scan | 0 | T-SM-018 |

## 10. Gesamtabnahme je Entwicklungsstufe

🧭 Vorschlag, abgeleitet aus den Gates der Runde 20 (E-PR-012); **keine Termine**.

| Stufe | Inhalt | Voraussetzung |
|---|---|---|
| **A: Vorbereitung** | Hauptspiel-Grundlagen (Ladebildschirm, kleiner Kartenteil, Menschensystem V1, Persistenz V1) laut Entwickler **vor** Adult | E-PR-010 |
| **B: Regelkern-Prototyp** | Gate-Kette, Zustimmungsdatensatz, Stopp, Tests T-ZU, AK-ZU | Standalone C++ |
| **C: Adult V1 klein** | Basisinteraktionen, wenige Positionen/Übergänge, grundlegende Reaktionen, einfache Präferenzen, **eine** Testlocation | Core Gate |
| **D: Erweiterungen** | Kleidung, Items, Verletzungen, Orte, Replay, Tattoos/Körpermodifikationen | Performance-/QA-/Visual-Gate |
| **E: Spät** | Flüssigkeitssystem, VR, High-End-Rendering, Hardcore-Content | Feature Flags |
| **F: Veröffentlichungsprüfung** | Rechtsprüfung, Plattformangaben, Altersfreigabe, Provenienz | Entscheidung des Entwicklers |

Jede Stufe erfordert zuvor die Erfüllung der Kriterien der vorhergehenden Stufe.

## 11. Nicht abnehmbar

Für nicht freigegebene Themen gibt es **keine** Abnahmekriterien ([Modul 29, Abschnitt 12](29-QA-Testkatalog.md#12-nicht-testbar--nicht-freigegeben)). Abnahmekriterien dürfen nur die **Verweigerung** prüfen.
